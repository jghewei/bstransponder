/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
* --------------------------------------------------------------------------
*  This software embodies materials and concepts which are proprietary and
*  confidential to PMC-Sierra, Inc.
*  PMC-Sierra distributes this software to its customers pursuant to the
*  terms and conditions of the Software License Agreement
*  contained in the text file software.lic that is distributed along with
*  the software. This software can only be utilized if all
*  terms and conditions of the Software License Agreement are
*  accepted. If there are any questions, concerns, or if the
*  Software License Agreement text file, software.lic, is missing please
*  contact PMC-Sierra for assistance.
* --------------------------------------------------------------------------
*   DESCRIPTION:
*     This file describes the DIGI performance monitoring Run-Time function.
*
*   NOTES:
*     None.
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_adpt.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Local Functions
*/

/*
** Public Functions
*/
/*******************************************************************************
* adpt_pmon_int_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the PMON interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   *int_table         - PMON interrupt table.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_pmon_int_set(UINT32 dev_id, digi_pmon_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_int_t int_dis_table;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, int_table);

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* enable PMON interrupt */
    rc |= digi_pmon_int_enable(adpt_handle->digi_handle, int_table);
    int_dis_table.misc_i = !(int_table->misc_i);
    int_dis_table.otn_i  = !(int_table->otn_i);
    /* clear PMON interrupt */
    rc |= digi_pmon_int_clear(adpt_handle->digi_handle, &int_dis_table);
    /* disable PMON interrupt */
    rc |= digi_pmon_int_disable(adpt_handle->digi_handle, &int_dis_table);

    PMC_RETURN(rc);
} /* adpt_pmon_int_set */


/*******************************************************************************
* adpt_pmon_trigger
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Triggers PMON to get performance counts.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_pmon_trigger(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_pmon_int_t int_table;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT32 trig_msk = 0;

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    if (PMC_SUCCESS != rc)
    {
        PMC_ENTRY();
        PMC_RETURN(rc);
    }

    /* manually trigger update */
    if (DIGI_TRIGGER_MANUAL == adpt_handle->dev_info->pmon_trig_mode)
    {
        trig_msk = adpt_handle->dev_info->manual_trig_msk;
        /* This Bit1 trigger can only be triggered if RCP is not enabled. */
        if (adpt_handle->ri_rcp_en || adpt_handle->di_rcp_en)
        {
            trig_msk = trig_msk & 0xFFFFFFFD;
        }

        rc = digi_pmon_manual_trigger_cfg(adpt_handle->digi_handle, trig_msk);
        if (PMC_SUCCESS != rc)
        {
            PMC_ENTRY();
            PMC_RETURN(rc);
        }

        /* reset */
        int_table.misc_i = FALSE;
        int_table.otn_i  = FALSE;

        /* Sleep for 50ms at maximum between polling cycles. FW will transfer 
         * PMON to shared memory through DMA. */
        for (index = 0; index < 5; index++)
        {
            /* sleep for 10ms */
            PMC_OS_USLEEP(10000);

            /* retrieve interrupt table for this int type */
            rc = digi_pmon_int_retrieve(adpt_handle->digi_handle, &int_table);
            if (PMC_SUCCESS != rc)
            {
                PMC_ENTRY();
                PMC_RETURN(rc);
            }

            /* check PMON interrupt */
            if (int_table.misc_i || int_table.otn_i)
            {
                break;
            }
        }
    }

    PMC_ATOMIC_ENTRY(adpt_handle->digi_handle);
    /* get PMON */
    rc |= adpt_int_cb_handle(adpt_handle->digi_handle, NULL, NULL, DIGI_INT_TYPE_PMON);

    PMC_ATOMIC_RETURN(adpt_handle->digi_handle, rc);
} /* adpt_pmon_trigger */


/*******************************************************************************
* adpt_pmon_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears PMON to reset all performance counts. This API is used for PMON clear
*   after calling adpt_pmon_int_trigger() first time as this PMON data isn't
*   reliable. Usually, it is called after cold restart or warm/crash restart.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_pmon_clear(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset line ENET PMON */
    PMC_MEMSET((void*)&adpt_handle->pmon_data->line_enet_pmon, 0, sizeof(digi_enet_pmon_t));
    /* reset system ENET PMON */
    PMC_MEMSET((void*)&adpt_handle->pmon_data->sys_enet_pmon, 0, sizeof(digi_enet_pmon_t));
    /* reset OTN PMON */
    PMC_MEMSET((void*)&adpt_handle->pmon_data->otn_pmon, 0, sizeof(digi_otn_pmon_t));
    PMC_MEMSET((void*)&adpt_handle->pmon_data->mapper_otn_pmon, 0, sizeof(digi_mapper_pmon_fo2_t));
    /* reset MAPOTN PMON */
    PMC_MEMSET((void*)&adpt_handle->pmon_data->mapper_pmon, 0, sizeof(digi_mapper_pmon_t));
    /* reset SIFD PMON */
    PMC_MEMSET((void*)&adpt_handle->pmon_data->sifd_pmon, 0, sizeof(digi_sifd_pmon_t));
    /* reset CBR PMON */
    PMC_MEMSET((void*)&adpt_handle->pmon_data->cbr_pmon, 0, sizeof(digi_cbr_pmon_t));

    PMC_RETURN(rc);
} /* adpt_pmon_clear */


/*******************************************************************************
* adpt_otn_fec_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FEC performance monitoring information in sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pm_id              - performance monitoring identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_fec_pm_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_fec_pm_t pm_id,
                                   UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    UINT32 i = 0;
    UINT32 otu_chnl = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_lineotn_fec_counters_t *pmon_fec = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if (traffic_loc >= DIGI_SIDE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get OTU channel */
    rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
    ADPT_RETURN_VERIFY(rc);

    /* pmon location */
    if (DIGI_LINE_SIDE == traffic_loc)
    {
        pmon_fec = &(adpt_handle->pmon_data->otn_pmon.otn_server_chnl[otu_chnl].fec);
    }
    else
    {
        pmon_fec = &(adpt_handle->pmon_data->otn_pmon.sysotn_server_chnl[otu_chnl].fec);
    }

    /* default value */
    *pm_count = 0;

    switch (pm_id)
    {
        case DIGI_FEC_TOTAL_CORR_ERRS:
            *pm_count = pmon_fec->fec_total_corr_errs;
            pmon_fec->fec_total_corr_errs = 0;
            break;

        /* 40G,100G SWIZZLE FEC (for 100G need to sum up all lanes) */
        case DIGI_SWZ_LPA_STATS_FP:
            *pm_count = pmon_fec->fec_otuk_swz_lpa_stats_fp;
            pmon_fec->fec_otuk_swz_lpa_stats_fp = 0;
            break;
            
        case DIGI_SWZ_LPA_STATS_UNCORR_CWDS:
            *pm_count = pmon_fec->fec_otuk_swz_lpa_stats_uncorr_cwds;
            pmon_fec->fec_otuk_swz_lpa_stats_uncorr_cwds = 0;
            break;
            
        case DIGI_SWZ_LPA_STATS_CORR_1S:
            *pm_count = pmon_fec->fec_otuk_swz_lpa_stats_corr_1s;
            pmon_fec->fec_otuk_swz_lpa_stats_corr_1s = 0;
            break;
            
        case DIGI_SWZ_LPA_STATS_CORR_0S:
            *pm_count = pmon_fec->fec_otuk_swz_lpa_stats_corr_0s;
            pmon_fec->fec_otuk_swz_lpa_stats_corr_0s = 0;
            break;
        
        case DIGI_SWZ_LLSA_STATS_UNCORR_CWDS:
            *pm_count = pmon_fec->fec_otuk_swz_llsa_stats_uncorr_cwds;
            pmon_fec->fec_otuk_swz_llsa_stats_uncorr_cwds = 0;
            break;
            
        case DIGI_SWZ_LLSA_STATS_CORR_1S:
            for (i = 0; i < 20; i++)
            {
                *pm_count = *pm_count + pmon_fec->fec_otuk_swz_llsa_stats_lane_corr_1s[i];
                pmon_fec->fec_otuk_swz_llsa_stats_lane_corr_1s[i] = 0;
            }
            break;
            
        case DIGI_SWZ_LLSA_STATS_CORR_0S:
            for (i = 0; i < 20; i++)
            {
                *pm_count = *pm_count + pmon_fec->fec_otuk_swz_llsa_stats_lane_corr_0s[i];
                pmon_fec->fec_otuk_swz_llsa_stats_lane_corr_0s[i] = 0;
            }
            break;

        /* GFEC 10G,40G,100G */
        case DIGI_GFEC_LPA_STATS_FP:
            *pm_count = pmon_fec->fec_otuk_gfec_lpa_stats_fp;
            pmon_fec->fec_otuk_gfec_lpa_stats_fp = 0;
            break;
            
        case DIGI_GFEC_LPA_STATS_UNCORR_CWDS:
            *pm_count = pmon_fec->fec_otuk_gfec_lpa_stats_uncorr_cwds;
            pmon_fec->fec_otuk_gfec_lpa_stats_uncorr_cwds = 0;
            break;
            
        case DIGI_GFEC_LPA_STATS_CORR_1S:
            *pm_count = pmon_fec->fec_otuk_gfec_lpa_stats_corr_1s;
            pmon_fec->fec_otuk_gfec_lpa_stats_corr_1s = 0;
            break;
            
        case DIGI_GFEC_LPA_STATS_CORR_0S:
            *pm_count = pmon_fec->fec_otuk_gfec_lpa_stats_corr_0s;
            pmon_fec->fec_otuk_gfec_lpa_stats_corr_0s = 0;
            break;

        /* I4 FEC 10G,40G */
        case DIGI_I4_FEC_CORR_0S_COUNT:
            *pm_count = pmon_fec->fec_otuk_i4_fec_corr_0s_count;
            pmon_fec->fec_otuk_i4_fec_corr_0s_count = 0;
            break;
            
        case DIGI_I4_FEC_CORR_1S_COUNT:
            *pm_count = pmon_fec->fec_otuk_i4_fec_corr_1s_count;
            pmon_fec->fec_otuk_i4_fec_corr_1s_count = 0;
            break;
            
        case DIGI_I4_FEC_UNCORR_ERROR_COUNT:
            *pm_count = pmon_fec->fec_otuk_i4_fec_uncorr_error_count;
            pmon_fec->fec_otuk_i4_fec_uncorr_error_count = 0;
            break;
            
        case DIGI_I4_FEC_RX_FRM_PULSE_COUNT:
            *pm_count = pmon_fec->fec_otuk_i4_fec_rx_frm_pulse_count;
            pmon_fec->fec_otuk_i4_fec_rx_frm_pulse_count = 0;
            break;

        /* I7 FEC 10G,40G  */
        case DIGI_I7_FEC_CORR_0S_COUNT:
            *pm_count = pmon_fec->fec_otuk_i7_fec_corr_0s_count;
            pmon_fec->fec_otuk_i7_fec_corr_0s_count = 0;
            break;
            
        case DIGI_I7_FEC_CORR_1S_COUNT:
            *pm_count = pmon_fec->fec_otuk_i7_fec_corr_1s_count;
            pmon_fec->fec_otuk_i7_fec_corr_1s_count = 0;
            break;
            
        case DIGI_I7_FEC_UNCORR_ERROR_COUNT:
            *pm_count = pmon_fec->fec_otuk_i7_fec_uncorr_error_count;
            pmon_fec->fec_otuk_i7_fec_uncorr_error_count = 0;
            break;
            
        case DIGI_I7_FEC_RX_FRM_PULSE_COUNT:
            *pm_count = pmon_fec->fec_otuk_i7_fec_rx_frm_pulse_count;
            pmon_fec->fec_otuk_i7_fec_rx_frm_pulse_count = 0;
            break;
        


        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_fec_pm_get */


/*******************************************************************************
* adpt_otn_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the OTUk/ODUk performance monitoring information in sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   pm_id              - performance monitoring identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_pm_get(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_otn_seg_t seg_id,
                               digi_otn_loc_t location,
                               digi_otn_pm_t pm_id,
                               UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 otu_chnl = 0;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_lineotn_otu_frm_counters_t *pmon_otu = NULL;
    digi_pmon_coreotn_odukt_counters_t *pmon_odukt = NULL;
    digi_pmon_coreotn_odukp_counters_t *pmon_odukp = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if (traffic_loc >= DIGI_SIDE_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    switch (seg_id)
    {
        case DIGI_OTN_SEG_SM:
            /* get OTU channel */
            rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
            ADPT_RETURN_VERIFY(rc);

            /* pmon location */
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                pmon_otu = &(adpt_handle->pmon_data->otn_pmon.otn_server_chnl[otu_chnl].framer);
            }
            else
            {
                pmon_otu = &(adpt_handle->pmon_data->otn_pmon.sysotn_server_chnl[otu_chnl].framer);
            }
            switch (pm_id)
            {
                case DIGI_OTN_BIP:
                    *pm_count = pmon_otu->otu_frm_sk_sm_p_n_ebc;
                    pmon_otu->otu_frm_sk_sm_p_n_ebc = 0;
                    break;
                    
                case DIGI_OTN_NDS:
                    *pm_count = pmon_otu->otu_frm_sk_sm_p_n_ds;
                    pmon_otu->otu_frm_sk_sm_p_n_ds = 0;
                    break;
                    
                case DIGI_OTN_PIAE:
                    *pm_count = pmon_otu->otu_frm_sk_sm_p_iae;
                    pmon_otu->otu_frm_sk_sm_p_iae = 0;
                    break;
                    
                case DIGI_OTN_BEI:
                    *pm_count = pmon_otu->otu_frm_sk_sm_p_f_ebc;
                    pmon_otu->otu_frm_sk_sm_p_f_ebc = 0;
                    break;
                    
                case DIGI_OTN_FDS:
                    *pm_count = pmon_otu->otu_frm_sk_sm_p_f_ds;
                    pmon_otu->otu_frm_sk_sm_p_f_ds = 0;
                    break;
                    
                case DIGI_OTN_PBIAE:
                    *pm_count = pmon_otu->otu_frm_sk_sm_p_biae;
                    pmon_otu->otu_frm_sk_sm_p_biae = 0;
                    break;
                    
                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;

        case DIGI_OTN_SEG_TCM1:
        case DIGI_OTN_SEG_TCM2:
        case DIGI_OTN_SEG_TCM3:
        case DIGI_OTN_SEG_TCM4:
        case DIGI_OTN_SEG_TCM5:
        case DIGI_OTN_SEG_TCM6:
        case DIGI_OTN_SEG_PM_NIM:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                /* get odu channel ID and odu stage number */
                rc |= adpt_odu_chnl_get(dev_id, chnl_id, &odu_chnl, &odu_stage);
                ADPT_RETURN_VERIFY(rc);

                switch (odu_stage)
                {
                    case DIGI_ODU_STRUCT_LEVEL_HO_ODU:
                        if ((DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
                             && (DIGI_LINE_SIDE == traffic_loc))
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].pmon.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                            else
                            {
                                pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].stg3b.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                        }
                        else
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_ho_chnl[odu_chnl].pmon.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                            else
                            {
                                pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_ho_chnl[odu_chnl].stg3b.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                        }
                        break;
                        
                    case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_mo_chnl[odu_chnl].pmon.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        else
                        {
                            pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_mo_chnl[odu_chnl].stg3b.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].pmon.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        else
                        {
                            pmon_odukt = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].stg3b.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        break;

                    default:
                        PMC_RETURN(PMC_ERR_FAIL);
                        break;
                }
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                /* get GFP channel */
                rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
                ADPT_RETURN_VERIFY(rc);

                pmon_odukt = &(adpt_handle->pmon_data->mapper_otn_pmon.mapper_fo2_chnl[odu_chnl].pmon.tcm[seg_id-DIGI_OTN_SEG_TCM1]);
            }
            else
            {
                PMC_RETURN(PMC_ERR_FAIL);
            }

            switch (pm_id)
            {
                case DIGI_OTN_BIP:
                    *pm_count = pmon_odukt->odukt_sk_pn_ebc;
                    pmon_odukt->odukt_sk_pn_ebc = 0;
                    break;

                case DIGI_OTN_NDS:
                    *pm_count = pmon_odukt->odukt_sk_pn_ds;
                    pmon_odukt->odukt_sk_pn_ds = 0;
                    break;

                case DIGI_OTN_PIAE:
                    *pm_count = pmon_odukt->odukt_sk_piae;
                    pmon_odukt->odukt_sk_piae = 0;
                    break;

                case DIGI_OTN_BEI:
                    *pm_count = pmon_odukt->odukt_sk_pf_ebc;
                    pmon_odukt->odukt_sk_pf_ebc = 0;
                    break;

                case DIGI_OTN_FDS:
                    *pm_count = pmon_odukt->odukt_sk_pf_ds;
                    pmon_odukt->odukt_sk_pf_ds = 0;
                    break;

                case DIGI_OTN_PBIAE:
                    *pm_count = pmon_odukt->odukt_sk_pbiae;
                    pmon_odukt->odukt_sk_pbiae = 0;
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;
        
        case DIGI_OTN_SEG_PM:
            if (NULL != adpt_handle->traffic_info[index].odu_handle)
            {
                /* get odu channel ID and odu stage number */
                rc |= adpt_odu_chnl_get(dev_id, chnl_id, &odu_chnl, &odu_stage);
                ADPT_RETURN_VERIFY(rc);

                switch (odu_stage)
                {
                    case DIGI_ODU_STRUCT_LEVEL_HO_ODU:
                        if ((DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
                             && (DIGI_LINE_SIDE == traffic_loc))
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].pmon.pm);
                            }
                            else
                            {
                                pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].stg3b.pm);
                            }
                        }
                        else
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_ho_chnl[odu_chnl].pmon.pm);
                            }
                            else
                            {
                                pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_ho_chnl[odu_chnl].stg3b.pm);
                            }
                        }
                        break;
                        
                    case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_mo_chnl[odu_chnl].pmon.pm);
                        }
                        else
                        {
                            pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_mo_chnl[odu_chnl].stg3b.pm);
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].pmon.pm);
                        }
                        else
                        {
                            pmon_odukp = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].stg3b.pm);
                        }
                        break;

                    default:
                        PMC_RETURN(PMC_ERR_FAIL);
                        break;
                }
            }
            else if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                /* get GFP channel */
                rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
                ADPT_RETURN_VERIFY(rc);

                pmon_odukp = &(adpt_handle->pmon_data->mapper_otn_pmon.mapper_fo2_chnl[odu_chnl].pmon.pm);
            }
            else
            {
                PMC_RETURN(PMC_ERR_FAIL);
            }

            switch (pm_id)
            {
                case DIGI_OTN_BIP:
                    *pm_count = pmon_odukp->odukp_sk_pn_ebc_pm;
                    pmon_odukp->odukp_sk_pn_ebc_pm = 0;
                    break;

                case DIGI_OTN_NDS:
                    *pm_count = pmon_odukp->odukp_sk_pn_ds_pm;
                    pmon_odukp->odukp_sk_pn_ds_pm = 0;
                    break;

                case DIGI_OTN_BEI:
                    *pm_count = pmon_odukp->odukp_sk_pf_ebc_pm;
                    pmon_odukp->odukp_sk_pf_ebc_pm = 0;
                    break;

                case DIGI_OTN_FDS:
                    *pm_count = pmon_odukp->odukp_sk_pf_ds_pm;
                    pmon_odukp->odukp_sk_pf_ds_pm = 0;
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;
        
        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_pm_get */


/*******************************************************************************
* adpt_otn_dm_trigger
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Trigger DMti/DMp bit change from 0 to 1, or from 1 to 0 in source position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity for PM and TCMi only.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_dm_trigger(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_delay_measure_go(digi_handle, 
                                       odu_handle, 
                                       (odu_struct_odu_frm_pm_tcm_id_t)seg_id);

    PMC_RETURN(rc);
} /* adpt_otn_dm_trigger */


/*******************************************************************************
* adpt_otn_dm_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DMti/DMp delay count numbers of frames in sink position. Delay 
*   measurement count is 18 bits in precision for all ODUks (covers up to 
*   200 ms in distance).
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity for PM and TCMi only.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*
* OUTPUTS:
*   *dm_count          - delay count numbers of ODUk frames.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   1. If the delay measure is not started, *dm_count will return 0xFFFFFFFE.
*   2. If the measure counter is not ready, *dm_count will return 0xFFFFFFFF.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_dm_pm_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   UINT32 *dm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(dm_count);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_get_delay_measure_count_no_block(digi_handle,
                                                       odu_handle,
                                                       (odu_struct_odu_frm_pm_tcm_id_t)seg_id,
                                                       dm_count);

    PMC_RETURN(rc);
} /* adpt_otn_dm_pm_get */


/*******************************************************************************
* adpt_otn_prbs_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS performance monitoring information which is mapped into
*   ODUk.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_prbs_pm_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_coreotn_odukp_prbs_counter_t *pmon_prbs = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        /* get odu channel ID and odu stage number */
        rc |= adpt_odu_chnl_get(dev_id, chnl_id, &odu_chnl, &odu_stage);
        ADPT_RETURN_VERIFY(rc);

        switch (odu_stage)
        {
            case DIGI_ODU_STRUCT_LEVEL_HO_ODU:
                if ((DIGI_PROD_APP_SYSOTN_CARD == adpt_handle->dev_info->card_type)
                     && (DIGI_LINE_SIDE == traffic_loc))
                {
                    pmon_prbs = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].pmon.prbs);
                }
                else
                {
                    pmon_prbs = &(adpt_handle->pmon_data->otn_pmon.otn_odu_ho_chnl[odu_chnl].pmon.prbs);
                }
                break;

            case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                pmon_prbs = &(adpt_handle->pmon_data->otn_pmon.otn_odu_mo_chnl[odu_chnl].pmon.prbs);
                break;

            case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                pmon_prbs = &(adpt_handle->pmon_data->otn_pmon.otn_odu_lo_chnl[odu_chnl].pmon.prbs);
                break;

            default:
                PMC_RETURN(PMC_ERR_FAIL);
                break;
        }
    }
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        pmon_prbs = &(adpt_handle->pmon_data->mapper_otn_pmon.mapper_fo2_chnl[odu_chnl].pmon.prbs);
    }
    else
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    *pm_count = pmon_prbs->prbs_tse_chan_defects;
    pmon_prbs->prbs_tse_chan_defects = 0;

    PMC_RETURN(rc);
} /* adpt_otn_prbs_pm_get */


/*******************************************************************************
* adpt_enet_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet performance monitoring information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction (ingress/egress).
*   pm_id              - performance monitoring identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   As to the DIGI_ENET_MIB_PM_ALL item, the memory size of pm_count needs 
*   a digi_pmon_enet_mib_t length.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_pm_get(UINT32        dev_id,
                                UINT32          chnl_id,
                                digi_port_dir_t dir,
                                digi_enet_pm_t  pm_id,
                                UINT32          *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 src_index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_enet_mib_t *pmon_enet = NULL;
    digi_pmon_enet_eclass_t *pmon_eclass = NULL;
    digi_sifd_pmon_link_t *pmon_sifd = NULL;
    digi_enet_pmon_chnl_t *enet_pmon_chnl = NULL;
    digi_ilkn_link_t *ilkn_ctxt = NULL;
    UINT32 client_mode = 0;
    digi_enet_pmon_mode_t enet_mode = DIGI_ENET_PMON_MODE_NONE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if ((traffic_loc >= DIGI_SIDE_MAX) || (dir >= LAST_DIGI_PORT_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            switch (adpt_handle->traffic_info[index].mapping_mode)
            {
                case DIGI_MAPOTN_ODU4P_100_GE_GFP:
                case DIGI_MAPOTN_ODU3P_40_GE_GFP:
                case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
                case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
                case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
                case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
                case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
                case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
                    /* PMON location */
                    if (DIGI_PORT_DIR_RX == dir)
                    {
                        pmon_enet   = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].enet.enet_mib);
                        pmon_eclass = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].enet.enet_eclass);
                    }
                    else
                    {
                        /* not support in Tx for ETRANS datapath */
                        *pm_count = 0;
                        PMC_RETURN(rc);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL == pmon_enet)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                pmon_enet   = &(adpt_handle->pmon_data->line_enet_pmon.enet_chnl[enet_chnl].enet_mib);
                pmon_eclass = &(adpt_handle->pmon_data->line_enet_pmon.enet_chnl[enet_chnl].enet_eclass);
            }
            else
            {
                pmon_enet   = &(adpt_handle->pmon_data->sys_enet_pmon.enet_chnl[enet_chnl].enet_mib);
                pmon_eclass = &(adpt_handle->pmon_data->sys_enet_pmon.enet_chnl[enet_chnl].enet_eclass);
            }
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                pmon_enet   = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].enet.enet_mib);
                pmon_eclass = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].enet.enet_eclass);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    /* get client mode */
                    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[src_index].traffic_mode);
                    switch (client_mode)
                    {
                        case DIGI_ILKN_SW_COPI1:
                        case DIGI_ILKN_CPB_COPI1:
                            ilkn_ctxt = adpt_handle->ilkn_1_ctxt;
                            break;

                        case DIGI_ILKN_SW_COPI2:
                        case DIGI_ILKN_CPB_COPI2:
                            ilkn_ctxt = adpt_handle->ilkn_2_ctxt;
                            break;

                        default:
                            break;
                    }

                    if (NULL != ilkn_ctxt)
                    {
                        rc = digi_sifd_pmon_link_get(adpt_handle->digi_handle,
                                                     ilkn_ctxt,
                                                     &(adpt_handle->pmon_data->sifd_pmon),
                                                     &pmon_sifd);
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_sifd_pmon_enet_chnl_get(adpt_handle->digi_handle,
                                                          adpt_handle->traffic_info[src_index].ilkn_handle,
                                                          pmon_sifd,
                                                          &enet_mode,
                                                          &enet_pmon_chnl);
                        ADPT_RETURN_VERIFY(rc);

                        if (NULL != enet_pmon_chnl)
                        {
                            pmon_enet   = &(enet_pmon_chnl->enet_mib);
                            pmon_eclass = &(enet_pmon_chnl->enet_eclass);
                        }
                    }
                }
            }

            if (NULL == pmon_enet)
            {
                *pm_count = 0;
                PMC_RETURN(rc);
            }
        }
    }

    /* default value */
    *pm_count = 0;

    switch (pm_id)
    {
        case DIGI_ENET_MIB_PM_ALL:
            /* PMON copy */
            PMC_MEMCPY(pm_count, pmon_enet, sizeof(digi_pmon_enet_mib_t));
            /* PMON reset */
            PMC_MEMSET(pmon_enet, 0, sizeof(digi_pmon_enet_mib_t));
            break;

        case DIGI_FRAMES_OK_LSB:
            *pm_count = pmon_enet->a_frames_rx_ok_lsb;
            pmon_enet->a_frames_rx_ok_lsb = 0;
            break;

        case DIGI_FRAMES_OK_MSB:
            *pm_count = pmon_enet->a_frames_rx_ok_msb;
            pmon_enet->a_frames_rx_ok_msb = 0;
            break;

        case DIGI_FRAME_CHECK_SEQUENCE_ERRORS_LSB:
            *pm_count = pmon_enet->a_frame_check_sequence_errors_lsb;
            pmon_enet->a_frame_check_sequence_errors_lsb = 0;
            break;

        case DIGI_FRAME_CHECK_SEQUENCE_ERRORS_MSB:
            *pm_count = pmon_enet->a_frame_check_sequence_errors_msb;
            pmon_enet->a_frame_check_sequence_errors_msb = 0;
            break;

        case DIGI_ALIGNMENT_ERRORS_LSB:
            *pm_count = pmon_enet->a_alignment_errors_lsb;
            pmon_enet->a_alignment_errors_lsb = 0;
            break;

        case DIGI_ALIGNMENT_ERRORS_MSB:
            *pm_count = pmon_enet->a_alignment_errors_msb;
            pmon_enet->a_alignment_errors_msb = 0;
            break;

        case DIGI_PAUSE_MAC_CTRL_FRAMES_LSB:
            *pm_count = pmon_enet->a_pause_mac_ctrl_frames_rx_lsb;
            pmon_enet->a_pause_mac_ctrl_frames_rx_lsb = 0;
            break;

        case DIGI_PAUSE_MAC_CTRL_FRAMES_MSB:
            *pm_count = pmon_enet->a_pause_mac_ctrl_frames_rx_msb;
            pmon_enet->a_pause_mac_ctrl_frames_rx_msb = 0;
            break;

        case DIGI_FRAME_TOO_LONG_ERRORS_LSB:
            *pm_count = pmon_enet->a_frame_too_long_errors_lsb;
            pmon_enet->a_frame_too_long_errors_lsb = 0;
            break;

        case DIGI_FRAME_TOO_LONG_ERRORS_MSB:
            *pm_count = pmon_enet->a_frame_too_long_errors_msb;
            pmon_enet->a_frame_too_long_errors_msb = 0;
            break;

        case DIGI_IN_RANGE_LENGTH_ERRORS_LSB:
            *pm_count = pmon_enet->a_in_range_length_errors_lsb;
            pmon_enet->a_in_range_length_errors_lsb = 0;
            break;

        case DIGI_IN_RANGE_LENGTH_ERRORS_MSB:
            *pm_count = pmon_enet->a_in_range_length_errors_msb;
            pmon_enet->a_in_range_length_errors_msb = 0;
            break;

        case DIGI_VLAN_OK_LSB:
            *pm_count = pmon_enet->vlan_rx_ok_lsb;
            pmon_enet->vlan_rx_ok_lsb = 0;
            break;

        case DIGI_VLAN_OK_MSB:
            *pm_count = pmon_enet->vlan_rx_ok_msb;
            pmon_enet->vlan_rx_ok_msb = 0;
            break;

        case DIGI_IF_ERRORS_LSB:
            *pm_count = pmon_enet->if_in_errors_lsb;
            pmon_enet->if_in_errors_lsb = 0;
            break;

        case DIGI_IF_ERRORS_MSB:
            *pm_count = pmon_enet->if_in_errors_msb;
            pmon_enet->if_in_errors_msb = 0;
            break;

        case DIGI_IF_OCTS_LSB:
            *pm_count = pmon_enet->if_in_octs_lsb;
            pmon_enet->if_in_octs_lsb = 0;
            break;

        case DIGI_IF_OCTS_MSB:
            *pm_count = pmon_enet->if_in_octs_msb;
            pmon_enet->if_in_octs_msb = 0;
            break;

        case DIGI_IF_UCAST_PKTS_LSB:
            *pm_count = pmon_enet->if_in_ucast_pkts_lsb;
            pmon_enet->if_in_ucast_pkts_lsb = 0;
            break;

        case DIGI_IF_UCAST_PKTS_MSB:
            *pm_count = pmon_enet->if_in_ucast_pkts_msb;
            pmon_enet->if_in_ucast_pkts_msb = 0;
            break;

        case DIGI_IF_MULTICAST_PKTS_LSB:
            *pm_count = pmon_enet->if_in_multicast_pkts_lsb;
            pmon_enet->if_in_multicast_pkts_lsb = 0;
            break;

        case DIGI_IF_MULTICAST_PKTS_MSB:
            *pm_count = pmon_enet->if_in_multicast_pkts_msb;
            pmon_enet->if_in_multicast_pkts_msb = 0;
            break;

        case DIGI_IF_BROADCAST_PKTS_LSB:
            *pm_count = pmon_enet->if_in_broadcast_pkts_lsb;
            pmon_enet->if_in_broadcast_pkts_lsb = 0;
            break;

        case DIGI_IF_BROADCAST_PKTS_MSB:
            *pm_count = pmon_enet->if_in_broadcast_pkts_msb;
            pmon_enet->if_in_broadcast_pkts_msb = 0;
            break;

        case DIGI_ETHER_STATS_DROP_EVENTS_LSB:
            *pm_count = pmon_enet->ether_stats_drop_events_lsb;
            pmon_enet->ether_stats_drop_events_lsb = 0;
            break;

        case DIGI_ETHER_STATS_DROP_EVENTS_MSB:
            *pm_count = pmon_enet->ether_stats_drop_events_msb;
            pmon_enet->ether_stats_drop_events_msb = 0;
            break;

        case DIGI_ETHER_STATS_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_octs_lsb;
            pmon_enet->ether_stats_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_octs_msb;
            pmon_enet->ether_stats_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_lsb;
            pmon_enet->ether_stats_pkts_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_msb;
            pmon_enet->ether_stats_pkts_msb = 0;
            break;

        case DIGI_ETHER_STATS_UNDERSIZE_PKTS_LSB:
            *pm_count = pmon_enet->ether_stats_undersize_pkts_lsb;
            pmon_enet->ether_stats_undersize_pkts_lsb = 0;
            break;

        case DIGI_ETHER_STATS_UNDERSIZE_PKTS_MSB:
            *pm_count = pmon_enet->ether_stats_undersize_pkts_msb;
            pmon_enet->ether_stats_undersize_pkts_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_64_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_64_octs_lsb;
            pmon_enet->ether_stats_pkts_64_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_64_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_64_octs_msb;
            pmon_enet->ether_stats_pkts_64_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_65_to_127_octs_lsb;
            pmon_enet->ether_stats_pkts_65_to_127_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_65_to_127_octs_msb;
            pmon_enet->ether_stats_pkts_65_to_127_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_128_to_255_octs_lsb;
            pmon_enet->ether_stats_pkts_128_to_255_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_128_to_255_octs_msb;
            pmon_enet->ether_stats_pkts_128_to_255_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_256_to_511_octs_lsb;
            pmon_enet->ether_stats_pkts_256_to_511_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_256_to_511_octs_msb;
            pmon_enet->ether_stats_pkts_256_to_511_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_512_to_1023_octs_lsb;
            pmon_enet->ether_stats_pkts_512_to_1023_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_512_to_1023_octs_msb;
            pmon_enet->ether_stats_pkts_512_to_1023_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_1024_to_1518_octs_lsb;
            pmon_enet->ether_stats_pkts_1024_to_1518_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_1024_to_1518_octs_msb;
            pmon_enet->ether_stats_pkts_1024_to_1518_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_LSB:
            *pm_count = pmon_enet->ether_stats_pkts_1519_to_max_octs_lsb;
            pmon_enet->ether_stats_pkts_1519_to_max_octs_lsb = 0;
            break;

        case DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_MSB:
            *pm_count = pmon_enet->ether_stats_pkts_1519_to_max_octs_msb;
            pmon_enet->ether_stats_pkts_1519_to_max_octs_msb = 0;
            break;

        case DIGI_ETHER_STATS_OVERSIZE_PKTS_LSB:
            *pm_count = pmon_enet->ether_stats_oversize_pkts_lsb;
            pmon_enet->ether_stats_oversize_pkts_lsb = 0;
            break;

        case DIGI_ETHER_STATS_OVERSIZE_PKTS_MSB:
            *pm_count = pmon_enet->ether_stats_oversize_pkts_msb;
            pmon_enet->ether_stats_oversize_pkts_msb = 0;
            break;

        case DIGI_ETHER_STATS_JABBER_LSB:
            *pm_count = pmon_enet->ether_stats_jabber_lsb;
            pmon_enet->ether_stats_jabber_lsb = 0;
            break;

        case DIGI_ETHER_STATS_JABBER_MSB:
            *pm_count = pmon_enet->ether_stats_jabber_msb;
            pmon_enet->ether_stats_jabber_msb = 0;
            break;

        case DIGI_ETHER_STATS_FRAGMENTS_LSB:
            *pm_count = pmon_enet->ether_stats_fragments_lsb;
            pmon_enet->ether_stats_fragments_lsb = 0;
            break;

        case DIGI_ETHER_STATS_FRAGMENTS_MSB:
            *pm_count = pmon_enet->ether_stats_fragments_msb;
            pmon_enet->ether_stats_fragments_msb = 0;
            break;

        case DIGI_MAC_CONTROL_FRAMES_LSB:
            *pm_count = pmon_enet->a_mac_control_frames_rx_lsb;
            pmon_enet->a_mac_control_frames_rx_lsb = 0;
            break;

        case DIGI_MAC_CONTROL_FRAMES_MSB:
            *pm_count = pmon_enet->a_mac_control_frames_rx_msb;
            pmon_enet->a_mac_control_frames_rx_msb = 0;
            break;

        case DIGI_LLDP_PKT_CNT:
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                pmon_eclass = &(adpt_handle->pmon_data->line_enet_pmon.enet_chnl[enet_chnl].enet_eclass);
            }
            else
            {
                pmon_eclass = &(adpt_handle->pmon_data->sys_enet_pmon.enet_chnl[enet_chnl].enet_eclass);
            }
            
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = pmon_eclass->rx_lldp_pkt_cnt;
                pmon_eclass->rx_lldp_pkt_cnt = 0;
            }
            else
            {
                *pm_count = pmon_eclass->tx_lldp_pkt_cnt;
                pmon_eclass->tx_lldp_pkt_cnt = 0;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_pm_get */


/*******************************************************************************
* adpt_enet_errors_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet error counts information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction (ingress/egress).
*
* OUTPUTS:
*   *enet_errors       - Ethernet error counters.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_errors_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_pmon_enet_errors_t *enet_errors)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 src_index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_enet_errors_t *pmon_enet = NULL;
    digi_sifd_pmon_link_t *pmon_sifd = NULL;
    digi_enet_pmon_chnl_t *enet_pmon_chnl = NULL;
    digi_ilkn_link_t *ilkn_ctxt = NULL;
    UINT32 client_mode = 0;
    digi_enet_pmon_mode_t enet_mode = DIGI_ENET_PMON_MODE_NONE;

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    if ((PMC_SUCCESS != rc) || (NULL == enet_errors))
    {
        PMC_ENTRY();
        PMC_RETURN(rc);
    }

    PMC_ATOMIC_ENTRY(adpt_handle->digi_handle);
    /* parameter verification */
    ADPT_ATOMIC_PARA_VERIFY(adpt_handle->digi_handle, enet_errors);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        traffic_loc = DIGI_LINE_SIDE;
    }

    if ((traffic_loc >= DIGI_SIDE_MAX) || (dir >= LAST_DIGI_PORT_DIR))
    {
        PMC_ATOMIC_RETURN(adpt_handle->digi_handle, PMC_ERR_INVALID_PARAMETERS);
    }

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            switch (adpt_handle->traffic_info[index].mapping_mode)
            {
                case DIGI_MAPOTN_ODU4P_100_GE_GFP:
                case DIGI_MAPOTN_ODU3P_40_GE_GFP:
                case DIGI_MAPOTN_ODU2P_PKT_10_GE_7_3_GFP:
                case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
                case DIGI_MAPOTN_ODU3P_PKT_40_GE_GMP:
                case DIGI_MAPOTN_ODU4P_PKT_100_GE_GMP:
                case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
                case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
                    /* PMON location */
                    if (DIGI_PORT_DIR_RX == dir)
                    {
                        pmon_enet = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].enet.enet_errors);
                    }
                    else
                    {
                        /* not support in Tx for ETRANS datapath */
                        PMC_MEMSET(enet_errors, 0, sizeof(digi_pmon_enet_errors_t));
                        PMC_ATOMIC_RETURN(adpt_handle->digi_handle, rc);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL == pmon_enet)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                pmon_enet = &(adpt_handle->pmon_data->line_enet_pmon.enet_chnl[enet_chnl].enet_errors);
            }
            else
            {
                pmon_enet = &(adpt_handle->pmon_data->sys_enet_pmon.enet_chnl[enet_chnl].enet_errors);
            }
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                pmon_enet = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].enet.enet_errors);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    /* get client mode */
                    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[src_index].traffic_mode);
                    switch (client_mode)
                    {
                        case DIGI_ILKN_SW_COPI1:
                        case DIGI_ILKN_CPB_COPI1:
                            ilkn_ctxt = adpt_handle->ilkn_1_ctxt;
                            break;

                        case DIGI_ILKN_SW_COPI2:
                        case DIGI_ILKN_CPB_COPI2:
                            ilkn_ctxt = adpt_handle->ilkn_2_ctxt;
                            break;

                        default:
                            break;
                    }

                    if (NULL != ilkn_ctxt)
                    {
                        rc = digi_sifd_pmon_link_get(adpt_handle->digi_handle,
                                                     ilkn_ctxt,
                                                     &(adpt_handle->pmon_data->sifd_pmon),
                                                     &pmon_sifd);
                        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

                        rc = digi_sifd_pmon_enet_chnl_get(adpt_handle->digi_handle,
                                                          adpt_handle->traffic_info[src_index].ilkn_handle,
                                                          pmon_sifd,
                                                          &enet_mode,
                                                          &enet_pmon_chnl);
                        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

                        if (NULL != enet_pmon_chnl)
                        {
                            pmon_enet = &(enet_pmon_chnl->enet_errors);
                        }
                    }
                }
            }

            if (NULL == pmon_enet)
            {
                PMC_MEMSET(enet_errors, 0, sizeof(digi_pmon_enet_errors_t));
                PMC_ATOMIC_RETURN(adpt_handle->digi_handle, rc);
            }
        }
    }

    /* PMON copy */
    PMC_MEMCPY(enet_errors, pmon_enet, sizeof(digi_pmon_enet_errors_t));
    /* Adapter PMON reset */
    PMC_MEMSET(pmon_enet, 0, sizeof(digi_pmon_enet_errors_t));

    PMC_ATOMIC_RETURN(adpt_handle->digi_handle, rc);
} /* adpt_enet_errors_get */


/*******************************************************************************
* adpt_enet_prbs_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet PRBS error counts.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pm_count          - Ethernet PRBS error counters.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API is obsolete. Please use the adpt_enet_prbs_status_get() instead.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_prbs_pm_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL prbs_syn = FALSE;
    PMC_ENTRY();

    rc = adpt_enet_prbs_status_get(dev_id, chnl_id, &prbs_syn, pm_count);

    PMC_RETURN(rc);
} /* adpt_enet_prbs_pm_get */


/*******************************************************************************
* adpt_sdh_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the SONET/SDH performance monitoring information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pm_id              - perfromance identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_pm_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_sdh_pm_t pm_id,
                                UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    digi_pmon_cbrc_sdh_pmg_t *sdh_pmg = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    sdh_pmg = &(adpt_handle->pmon_data->cbr_pmon.cbr_chnl[cbr_chnl].sdh_pmg);

    switch (pm_id)
    {
        case DIGI_SDH_B1:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = sdh_pmg->rx_section_bip_err_count;
                sdh_pmg->rx_section_bip_err_count = 0;
            }
            else 
            {
                *pm_count = sdh_pmg->tx_section_bip_err_count;
                sdh_pmg->tx_section_bip_err_count = 0;
            }
            break;

        case DIGI_SDH_B2:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = (sdh_pmg->rx_line_bip_err_count_msb << 16) + sdh_pmg->rx_line_bip_err_count_lsb;
                sdh_pmg->rx_line_bip_err_count_msb = 0;
                sdh_pmg->rx_line_bip_err_count_lsb = 0;
            }
            else 
            {
                *pm_count = (sdh_pmg->tx_line_bip_err_count_msb << 16) + sdh_pmg->tx_line_bip_err_count_lsb;
                sdh_pmg->tx_line_bip_err_count_msb = 0;
                sdh_pmg->tx_line_bip_err_count_lsb = 0;
            }
            break;

        case DIGI_SDH_REI:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = (sdh_pmg->rx_line_rei_err_count_msb << 16) + sdh_pmg->rx_line_rei_err_count_lsb;
                sdh_pmg->rx_line_rei_err_count_msb = 0;
                sdh_pmg->rx_line_rei_err_count_lsb = 0;
            }
            else 
            {
                *pm_count = (sdh_pmg->tx_line_rei_err_count_msb << 16) + sdh_pmg->tx_line_rei_err_count_lsb;
                sdh_pmg->tx_line_rei_err_count_msb = 0;
                sdh_pmg->tx_line_rei_err_count_lsb = 0;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_sdh_pm_get */


/*******************************************************************************
* adpt_sdh_prbs_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS performance monitoring information which is mapped into
*   SONET/SDH.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   This API is obsolete. Please use the adpt_sdh_prbs_status_get() instead.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_prbs_pm_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    *pm_count = 0;

    PMC_RETURN(rc);
} /* adpt_sdh_prbs_pm_get */


/*******************************************************************************
* adpt_gfp_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the performance monitoring information of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pm_id              - performance identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_pm_get(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_gfp_pm_t pm_id,
                               UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_pmon_mapotn_counts_t *pmon_gfp = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get GFP channel */
    rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
    ADPT_RETURN_VERIFY(rc);

    /* pmon GFP */
    pmon_gfp = &(adpt_handle->pmon_data->mapper_pmon.mapper_chnl[gfp_chnl].counts);
    ADPT_PARA_VERIFY(pmon_gfp);

    switch (pm_id)
    {
        case DIGI_GFP_SK_RX_BYTE_COUNT_LSB:
            *pm_count = pmon_gfp->gfp_sink_rx_byte_count_lsb;
            pmon_gfp->gfp_sink_rx_byte_count_lsb = 0;
            break;

        case DIGI_GFP_SK_RX_BYTE_COUNT_MSB:
            *pm_count = pmon_gfp->gfp_sink_rx_byte_count_msb;
            pmon_gfp->gfp_sink_rx_byte_count_msb = 0;
            break;

        case DIGI_GFP_SK_RX_CLIENT_DATA_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_client_data_frame_count;
            pmon_gfp->gfp_sink_rx_client_data_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_IDLE_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_idle_frame_count;
            pmon_gfp->gfp_sink_rx_idle_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_CTRL_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_ctrl_frame_count;
            pmon_gfp->gfp_sink_rx_ctrl_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_CHEC_ERR_DROP_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_chec_err_drop_frame_count;
            pmon_gfp->gfp_sink_rx_chec_err_drop_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_CHEC_SINGLE_CORR_ERR_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_chec_single_corr_err_frame_count;
            pmon_gfp->gfp_sink_rx_chec_single_corr_err_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_UNPACK_DROP_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_unpack_drop_frame_count;
            pmon_gfp->gfp_sink_rx_unpack_drop_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_N_FDIS_THEC:
            *pm_count = pmon_gfp->gfp_sink_n_fdis_thec;
            pmon_gfp->gfp_sink_n_fdis_thec = 0;
            break;
            
        case DIGI_GFP_SK_RX_THEC_SINGLE_CORR_ERR_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_thec_single_corr_err_frame_count;
            pmon_gfp->gfp_sink_rx_thec_single_corr_err_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_FILTER_DROP_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_filter_drop_frame_count;
            pmon_gfp->gfp_sink_rx_filter_drop_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_N_FDIS_EHEC_EXI:
            *pm_count = pmon_gfp->gfp_sink_n_fdis_ehec_exi;
            pmon_gfp->gfp_sink_n_fdis_ehec_exi = 0;
            break;
            
        case DIGI_GFP_SK_P_FDIS:
            *pm_count = pmon_gfp->gfp_sink_p_fdis;
            pmon_gfp->gfp_sink_p_fdis = 0;
            break;
            
        case DIGI_GFP_SK_RX_GENERIC_FILTER_DROP_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_generic_filter_drop_frame_count;
            pmon_gfp->gfp_sink_rx_generic_filter_drop_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_CSF_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_csf_frame_count;
            pmon_gfp->gfp_sink_rx_csf_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_P_FCS_ERROR:
            *pm_count = pmon_gfp->gfp_sink_p_fcs_error;
            pmon_gfp->gfp_sink_p_fcs_error = 0;
            break;
            
        case DIGI_GFP_SK_RX_PAUSE_FRM_DROP_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_pause_frame_drop_count;
            pmon_gfp->gfp_sink_rx_pause_frame_drop_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_PAUSE_FRM_FROWARDED_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_pause_frame_frowarded_count;
            pmon_gfp->gfp_sink_rx_pause_frame_frowarded_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_CPU_FIFO_OVR_FLW_DROP_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_cpu_fifo_ovr_flw_drop_frame_count;
            pmon_gfp->gfp_sink_rx_cpu_fifo_ovr_flw_drop_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_TX_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_tx_frame_count;
            pmon_gfp->gfp_sink_tx_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_UNPACKED_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_unpacked_frame_count;
            pmon_gfp->gfp_sink_rx_unpacked_frame_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_MAX_FRM_LENGTH_FRM_DROP_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_max_frame_length_frm_drop_count;
            pmon_gfp->gfp_sink_rx_max_frame_length_frm_drop_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_FRM_LENGTH_FRM_DROP_COUNT:
            *pm_count = pmon_gfp->gfp_sink_rx_min_frame_length_frm_drop_count;
            pmon_gfp->gfp_sink_rx_min_frame_length_frm_drop_count = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_0:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[0];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[0] = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_1:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[1];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[1] = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_2:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[2];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[2] = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_3:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[3];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[3] = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_4:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[4];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[4] = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_5:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[5];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[5] = 0;
            break;
            
        case DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_6:
            *pm_count = pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[6];
            pmon_gfp->gfp_sink_rx_min_length_chk_fail_forwarded_frm_count[6] = 0;
            break;
            
        case DIGI_GFP_SO_TX_0_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_byte_0_count;
            pmon_gfp->gfp_src_tx_byte_0_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_1_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_byte_1_count;
            pmon_gfp->gfp_src_tx_byte_1_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_MGMT_FRMS:
            *pm_count = pmon_gfp->gfp_src_tx_mgmt_frames;
            pmon_gfp->gfp_src_tx_mgmt_frames = 0;
            break;
            
        case DIGI_GFP_SO_TX_CLIENT_DATA_FRAME_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_client_data_frame_count;
            pmon_gfp->gfp_src_tx_client_data_frame_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_PAUSE_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_pause_frame_count;
            pmon_gfp->gfp_src_tx_pause_frame_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_IDLE_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_idle_frame_count;
            pmon_gfp->gfp_src_tx_idle_frame_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_ERR_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_err_frame_count;
            pmon_gfp->gfp_src_tx_err_frame_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_MIN_FRM_LENGTH_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_min_frame_length_frm_count;
            pmon_gfp->gfp_src_tx_min_frame_length_frm_count = 0;
            break;
            
        case DIGI_GFP_SO_TX_MAX_FRM_LENGTH_FRM_COUNT:
            *pm_count = pmon_gfp->gfp_src_tx_max_frame_length_frm_count;
            pmon_gfp->gfp_src_tx_max_frame_length_frm_count = 0;
            break;

        case DIGI_GFP_DCPB_SLV_DPI_PKT_DROP_COUNT:
            *pm_count = pmon_gfp->cpb_dcpb_slv_dpi_packet_drop_count;
            pmon_gfp->cpb_dcpb_slv_dpi_packet_drop_count = 0;
            break;

        case DIGI_GFP_OCPB_SLV_DPI_PKT_DROP_COUNT:
            *pm_count = pmon_gfp->cpb_ocpb_slv_dpi_packet_drop_count;
            pmon_gfp->cpb_ocpb_slv_dpi_packet_drop_count = 0;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_gfp_pm_get */


/*******************************************************************************
* adpt_fc1200_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FC-1200 performance monitoring information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pm_id              - perfromance identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc1200_pm_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_fc1200_pm_t pm_id,
                                  UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    digi_pmon_cbrc_fc1200_t *fc1200 = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    fc1200 = &(adpt_handle->pmon_data->cbr_pmon.cbr_chnl[cbr_chnl].fc1200);

    switch (pm_id)
    {
        case DIGI_RX_HIGH_ERR_CNT:
            *pm_count = fc1200->src_rx_err_count >> 16;
            fc1200->src_rx_err_count = fc1200->src_rx_err_count & 0xFFFF;
            break;

        case DIGI_RX_BLK_TYPE_ERR_CNT:
            *pm_count = fc1200->src_rx_block_err_count >> 16;
            fc1200->src_rx_block_err_count = fc1200->src_rx_block_err_count & 0xFFFF;
            break;

        case DIGI_RX_INVLD_BLK_CNT:
            *pm_count = fc1200->src_rx_block_err_count & 0xFFFF;
            fc1200->src_rx_block_err_count = fc1200->src_rx_block_err_count & 0xFFFF0000;
            break;

        case DIGI_RX_DATA_BYTE_CNT_LSB:
            *pm_count = fc1200->src_rx_data_byte_count_lsb;
            fc1200->src_rx_data_byte_count_lsb = 0;
            break;

        case DIGI_RX_DATA_BYTE_CNT_MSB:
            *pm_count = fc1200->src_rx_data_byte_count_msb;
            fc1200->src_rx_data_byte_count_msb = 0;
            break;

        case DIGI_RX_OS_CNT:
            *pm_count = fc1200->src_rx_ordered_set_count;
            fc1200->src_rx_ordered_set_count = 0;
            break;

        case DIGI_RX_VALID_FRM_CNT:
            *pm_count = fc1200->src_rx_valid_frame_count;
            fc1200->src_rx_valid_frame_count = 0;
            break;

        case DIGI_RX_INVALID_FRM_CNT:
            *pm_count = fc1200->src_rx_invalid_frame_count;
            fc1200->src_rx_invalid_frame_count = 0;
            break;

        case DIGI_RX_VALID_FRM_BYTE_CNT:
            *pm_count = fc1200->src_rx_valid_frame_byte_count;
            fc1200->src_rx_valid_frame_byte_count = 0;
            break;

        case DIGI_RX_GFP_FRM_CNT:
            *pm_count = fc1200->tx_gfp_frame_count;
            fc1200->tx_gfp_frame_count = 0;
            break;

        case DIGI_RX_GFP_ERR_FRM_CNT:
            *pm_count = fc1200->tx_gfp_errored_frame_count;
            fc1200->tx_gfp_errored_frame_count = 0;
            break;

        case DIGI_TX_BYTE_CNT_LSB:
            *pm_count = fc1200->snk_tx_byte_count_lsb;
            fc1200->snk_tx_byte_count_lsb = 0;
            break;

        case DIGI_TX_BYTE_CNT_MSB:
            *pm_count = fc1200->snk_tx_byte_count_msb;
            fc1200->snk_tx_byte_count_msb = 0;
            break;

        case DIGI_TX_OS_CNT:
            *pm_count = fc1200->snk_tx_ordered_set_count;
            fc1200->snk_tx_ordered_set_count = 0;
            break;

        case DIGI_TX_ERR_CTRL_BLK_CNT:
            *pm_count = fc1200->snk_err_ctrl_block_count;
            fc1200->snk_err_ctrl_block_count = 0;
            break;

        case DIGI_TX_LF_OS_CNT:
            *pm_count = fc1200->snk_local_fault_ordered_set_count;
            fc1200->snk_local_fault_ordered_set_count = 0;
            break;

        case DIGI_TX_VALID_FRM_CNT:
            *pm_count = fc1200->snk_rx_valid_frame_count;
            fc1200->snk_rx_valid_frame_count = 0;
            break;

        case DIGI_TX_INVALID_FRM_CNT:
            *pm_count = fc1200->snk_rx_invalid_frame_count;
            fc1200->snk_rx_invalid_frame_count = 0;
            break;

        case DIGI_TX_VALID_FRM_BYTE_CNT:
            *pm_count = fc1200->snk_rx_valid_frame_byte_count;
            fc1200->snk_rx_valid_frame_byte_count = 0;
            break;

        case DIGI_TX_CRC_24_ERR_CNT:
            *pm_count = fc1200->crc_24_err_count;
            fc1200->crc_24_err_count = 0;
            break;

        case DIGI_TX_INVALID_BLK_HDR_CNT:
            *pm_count = fc1200->invalid_blk_hdr_count;
            fc1200->invalid_blk_hdr_count = 0;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc1200_pm_get */


/*******************************************************************************
* adpt_fc800_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FC-800 performance monitoring information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pm_id              - perfromance identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc800_pm_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_port_dir_t dir,
                                 digi_fc800_pm_t pm_id,
                                 UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    digi_pmon_cbrc_fc_cbr_t *fc800 = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    fc800 = &(adpt_handle->pmon_data->cbr_pmon.cbr_chnl[cbr_chnl].fc_cbr);

    switch (pm_id)
    {
        case DIGI_GOOD_FRM_CNT:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = fc800->src_rx_good_frame_count;
                fc800->src_rx_good_frame_count = 0;
            }
            else 
            {
                *pm_count = fc800->snk_rx_good_frame_count;
                fc800->snk_rx_good_frame_count = 0;
            }
            break;

        case DIGI_BAD_FRM_CNT:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = fc800->src_rx_bad_frame_count;
                fc800->src_rx_bad_frame_count = 0;
            }
            else 
            {
                *pm_count = fc800->snk_rx_bad_frame_count;
                fc800->snk_rx_bad_frame_count = 0;
            }
            break;

        case DIGI_BYTE_CNT:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = fc800->src_rx_byte_count;
                fc800->src_rx_byte_count = 0;
            }
            else 
            {
                *pm_count = fc800->snk_rx_byte_count;
                fc800->snk_rx_byte_count = 0;
            }
            break;

        case DIGI_BAD_CRC_CNT:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = fc800->src_rx_bad_crc_count;
                fc800->src_rx_bad_crc_count = 0;
            }
            else 
            {
                *pm_count = fc800->snk_rx_bad_crc_count;
                fc800->snk_rx_bad_crc_count = 0;
            }
            break;

        case DIGI_TOTAL_PKT_CNT:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = fc800->src_rx_total_packet_count;
                fc800->src_rx_total_packet_count = 0;
            }
            else 
            {
                *pm_count = fc800->snk_rx_total_packet_count;
                fc800->snk_rx_total_packet_count = 0;
            }
            break;

        case DIGI_LCV_CNT:
            if (DIGI_PORT_DIR_RX == dir)
            {
                *pm_count = fc800->src_lcv_cnt;
                fc800->src_lcv_cnt = 0;
            }
            else 
            {
                *pm_count = fc800->snk_lcv_cnt;
                fc800->snk_lcv_cnt = 0;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc800_pm_get */


/*******************************************************************************
* adpt_fc_prbs_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS performance monitoring information which is mapped into
*   FC1200.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc_prbs_pm_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    digi_pmon_cbrc_fc1200_t *fc1200 = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    fc1200 = &(adpt_handle->pmon_data->cbr_pmon.cbr_chnl[cbr_chnl].fc1200);

    *pm_count = fc1200->src_rx_err_count & 0xFFFF;
    fc1200->src_rx_err_count = fc1200->src_rx_err_count & 0xFFFF0000;

    PMC_RETURN(rc);
} /* adpt_fc_prbs_pm_get */


/*******************************************************************************
* adpt_sifd_pm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the performance monitoring information in SIFD sub-system.
*
* INPUTS:
*   dev_id             - device identity.
*   sifd_id            - SIFD identity. Only ILKN_SIFD_1 and ILKN_SIFD_2 may
*                        be available.
*   pm_id              - perfromance identity list.
*
* OUTPUTS:
*   *pm_count          - performance count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   DIGI_SIFD_CPB_DCPB_PACKET_DROP_CNT: pm_count need 150 * 4 bytes memory.
*   DIGI_SIFD_CPB_OCPB_PACKET_DROP_CNT: pm_count need 104 * 4 bytes memory.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sifd_pm_get(UINT32 dev_id,
                                ilkn_sifd_type_t sifd_id,
                                digi_sifd_pm_t pm_id,
                                UINT32 *pm_count)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sifd_pmon_link_t *pmon_sifd = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pm_count);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (((ILKN_SIFD_1 == sifd_id) 
        && ((ILKN_SIFD_2 == adpt_handle->dev_info->sifd_type)
        || (ILKN_SIFD_NONE == adpt_handle->dev_info->sifd_type)))
        || ((ILKN_SIFD_2 == sifd_id) 
        && ((ILKN_SIFD_1 == adpt_handle->dev_info->sifd_type)
        || (ILKN_SIFD_NONE == adpt_handle->dev_info->sifd_type))))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    switch (sifd_id)
    {
        case ILKN_SIFD_1:
            rc = digi_sifd_pmon_link_get(adpt_handle->digi_handle,
                                         adpt_handle->ilkn_1_ctxt,
                                         &(adpt_handle->pmon_data->sifd_pmon),
                                         &pmon_sifd);
            ADPT_RETURN_VERIFY(rc);
            break;

        case ILKN_SIFD_2:
            rc = digi_sifd_pmon_link_get(adpt_handle->digi_handle,
                                         adpt_handle->ilkn_2_ctxt,
                                         &(adpt_handle->pmon_data->sifd_pmon),
                                         &pmon_sifd);
            ADPT_RETURN_VERIFY(rc);
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }
    ADPT_PARA_VERIFY(pmon_sifd);

    switch (pm_id)
    {
        case DIGI_SIFD_TX_ERR_CNT_CPB:
            *pm_count = pmon_sifd->pmon.tx_error_count_cpb;
            pmon_sifd->pmon.tx_error_count_cpb = 0;
            break;

        case DIGI_SIFD_TX_ERR_CNT_ODUK:
            *pm_count = pmon_sifd->pmon.tx_error_count_oduk;
            pmon_sifd->pmon.tx_error_count_oduk = 0;
            break;

        case DIGI_SIFD_RX_ERR_CNT:
            *pm_count = pmon_sifd->pmon.rx_error_count;
            pmon_sifd->pmon.rx_error_count = 0;
            break;

        case DIGI_SIFD_TX_DIAG_CNT_CPB:
            *pm_count = pmon_sifd->pmon.tx_diag_count_cpb;
            pmon_sifd->pmon.tx_diag_count_cpb = 0;
            break;

        case DIGI_SIFD_TX_DIAG_CNT_ODUK:
            *pm_count = pmon_sifd->pmon.tx_diag_count_oduk;
            pmon_sifd->pmon.tx_diag_count_oduk = 0;
            break;

        case DIGI_SIFD_RX_DIAG_CNT_CPB:
            *pm_count = pmon_sifd->pmon.rx_diag_count_cpb;
            pmon_sifd->pmon.rx_diag_count_cpb = 0;
            break;

        case DIGI_SIFD_RX_DIAG_CNT_ODUK:
            *pm_count = pmon_sifd->pmon.rx_diag_count_oduk;
            pmon_sifd->pmon.rx_diag_count_oduk = 0;
            break;

        case DIGI_SIFD_CPB_DCPB_PACKET_DROP_CNT:
            PMC_MEMCPY(pm_count, &pmon_sifd->pmon.cpb_dcpb_slv_dpi_packet_drop_count, 
                       DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX * sizeof(UINT32));
            PMC_MEMSET((void*)&pmon_sifd->pmon.cpb_dcpb_slv_dpi_packet_drop_count, 0,
                       DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX * sizeof(UINT32));
            break;

        case DIGI_SIFD_CPB_OCPB_PACKET_DROP_CNT:
            PMC_MEMCPY(pm_count, &pmon_sifd->pmon.cpb_ocpb_slv_dpi_packet_drop_count, 
                       DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX * sizeof(UINT32));
            PMC_MEMSET((void*)&pmon_sifd->pmon.cpb_ocpb_slv_dpi_packet_drop_count, 0,
                       DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX * sizeof(UINT32));
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_sifd_pm_get */


/*
** End of file
*/
