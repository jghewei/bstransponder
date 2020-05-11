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
*     This file lists all the APIs for DIGI defects detection.
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
* adpt_defect_trigger
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Triggers PMON to get defect status and interrupt information.
*
* INPUTS:
*   dev_id             - device identity.
*   blk_msk            - block mask.
*                        DIGI_BLK_ENET: ENET(line) and ENET(sys)
*                        DIGI_BLK_OTN:  LINEOTN and COREOTN
*                        DIGI_BLK_MAP:  MAPOTN
*                        DIGI_BLK_SIFD: SIFD
*                        DIGI_BLK_CBR:  CBR
*                        DIGI_BLK_ALL:  all blocks
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
PUBLIC PMC_ERROR adpt_defect_trigger(UINT32 dev_id, digi_blk_type_t blk_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL token = FALSE;

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    if (PMC_SUCCESS != rc)
    {
        PMC_ENTRY();
        PMC_RETURN(rc);
    }

    PMC_ATOMIC_ENTRY(adpt_handle->digi_handle);
    if (blk_msk & DIGI_BLK_ENET)
    {
        /* get line ENET defect */
        rc = adpt_int_cb_handle(adpt_handle->digi_handle, (void*)&token, NULL, DIGI_INT_TYPE_ENET_LINE);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);
        /* get system ENET defect */
        rc = adpt_int_cb_handle(adpt_handle->digi_handle, (void*)&token, NULL, DIGI_INT_TYPE_ENET_SYS);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        PMC_ATOMIC_YIELD(adpt_handle->digi_handle, 0);
    }

    if ((blk_msk & DIGI_BLK_OTN) || (blk_msk & DIGI_BLK_OTN_ALL))
    {
        if (!(blk_msk & DIGI_BLK_OTN_ALL))
        {
            rc = adpt_defect_opt_en_get(dev_id, (BOOL*)&token);
            ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);
        }

        rc = adpt_int_cb_handle(adpt_handle->digi_handle, (void*)&token, NULL, DIGI_INT_TYPE_OTN);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        PMC_ATOMIC_YIELD(adpt_handle->digi_handle, 0);
    }

    if (blk_msk & DIGI_BLK_MAP)
    {
        /* get mapper defect */
        rc = adpt_int_cb_handle(adpt_handle->digi_handle, NULL, NULL, DIGI_INT_TYPE_OTN_MAPPER);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        PMC_ATOMIC_YIELD(adpt_handle->digi_handle, 0);
    }

    if (blk_msk & DIGI_BLK_SIFD)
    {
        /* get SIFD defect */
        rc = adpt_int_cb_handle(adpt_handle->digi_handle, NULL, NULL, DIGI_INT_TYPE_SIFD);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        PMC_ATOMIC_YIELD(adpt_handle->digi_handle, 0);
    }

    if (blk_msk & DIGI_BLK_CBR)
    {
        /* get CBR defect */
        rc = adpt_int_cb_handle(adpt_handle->digi_handle, (void*)&token, NULL, DIGI_INT_TYPE_CBR);
        ADPT_ATOMIC_RETURN_VERIFY(adpt_handle->digi_handle, rc);

        PMC_ATOMIC_YIELD(adpt_handle->digi_handle, 0);
    }

    PMC_ATOMIC_RETURN(adpt_handle->digi_handle, rc);
} /* adpt_defect_trigger */


/*******************************************************************************
* adpt_otn_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted OTN defects at the sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The following defects are supported in each segment.
*
*   DIGI_OTN_SEG_SM:  
*   OTUk -- dLOS, dLOL, dOTU-AIS(only for 10G), dLOF, dOOF, dLOM, dOOM, dTIM,
*   dBDI, dDEG, dEDEG, dIAE, dBIAE, aTSF, aTSD and CI_SSF.
*   ODUj[/i] -- dLOFLOM and dOOF.
*
*   DIGI_OTN_SEG_TCMi:
*   dTIM, dBDI, dDEG, dEDEG, dLTC, dLCK, dOCI, dAIS, dIAE, dBIAE, aTSF, aTSD
*   and CI_SSF.
*
*   DIGI_OTN_SEG_PM:
*   dTIM, dBDI, dDEG, dEDEG, dLTC, dLCK, dOCI, dAIS, dPLM, dMSIM, dCSF, dLOOMFI,
*   aTSF, aTSD and CI_SSF.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location,
                                  digi_otn_defect_t defect_id,
                                  BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    coreotn_int_framer_chnl_t *frm_int_table = NULL;
    odu_rfrm_int_pid_chnl_t *tcm_int_table = NULL;
    lineotn_int_chnl_t *otu_int_table = NULL;
    UINT32 otu_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 client_mode = 0;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    mapotn_int_chnl_t *status = NULL;
    digi_otn_ddeg_mon_seg_t odu_seg_org = DIGI_OTN_DDEG_MON_LINE;
    digi_otn_ddeg_mon_seg_t odu_seg = DIGI_OTN_DDEG_MON_LINE;
    digi_otn_ddeg_mode_t deg_mode = DIGI_OTN_HARDWARE_DDEG;
    UINT32 tcm_index = 0; 
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(defect_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        serdes_port = serdes_port + adpt_handle->dev_info->num_line_serdes_port;
        traffic_loc = DIGI_LINE_SIDE;
    }

    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX) 
        || (traffic_loc >= DIGI_SIDE_MAX)
        || (location >= LAST_DIGI_OTN_SIDE))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* default value */
    *defect_status = FALSE;

    switch (seg_id)
    {
        case DIGI_OTN_SEG_SM:
            if ((NULL != adpt_handle->traffic_info[index].otu_handle) && (DIGI_OTN_LINE_SIDE == location))
            {
                /* get OTU channel */
                rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
                ADPT_RETURN_VERIFY(rc);

                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    otu_int_table = &(adpt_handle->pmon_data->otn_defect.otn_server_chnl[otu_chnl]);
                }
                else
                {
                    otu_int_table = &(adpt_handle->pmon_data->otn_defect.sysotn_server_chnl[otu_chnl]);
                }

                switch (defect_id)
                {
                    case DIGI_OTN_LOS:
                        /* only availabe for 10G OTN */
                        switch (client_mode)
                        {
                            case DIGI_OTN_SERVER_OTU2F:
                            case DIGI_OTN_SERVER_OTU1F:
                            case DIGI_OTN_SERVER_OTU2E:
                            case DIGI_OTN_SERVER_OTU1E:
                            case DIGI_OTN_SERVER_OTU2:
                                *defect_status = otu_int_table->opsm23_rx_lane_los_int_v[0];
                                break;

                            default:
                                *defect_status = FALSE;
                                break;
                        }
                        break;

                    case DIGI_OTN_LOL:
                        if (DIGI_OTN_SERVER_OTU4 == client_mode)
                        {
                            *defect_status = otu_int_table->opsm4_rx_lol_int_v;
                        }
                        else
                        {
                            *defect_status = otu_int_table->opsm23_rx_lol_int_v;
                        }
                        break;

                    case DIGI_OTN_OTUAIS:
                        /* only availabe for 10G OTN */
                        switch (client_mode)
                        {
                            case DIGI_OTN_SERVER_OTU2F:
                            case DIGI_OTN_SERVER_OTU1F:
                            case DIGI_OTN_SERVER_OTU2E:
                            case DIGI_OTN_SERVER_OTU1E:
                            case DIGI_OTN_SERVER_OTU2:
                                *defect_status = otu_int_table->opsm23_rx_lane_otuk_ais_int_v[0];
                                break;

                            default:
                                *defect_status = FALSE;
                                break;
                        }
                        break;

                    case DIGI_OTN_LOF:
                        if (DIGI_OTN_SERVER_OTU4 == client_mode)
                        {
                            *defect_status = otu_int_table->opsm4_rx_otu_lof_int_v;
                        }
                        else
                        {
                            if (DIGI_SERDES_SFI_51_INTF == adpt_handle->dev_info->serdes_type[serdes_port])
                            {
                                *defect_status = otu_int_table->opsm23_rx_sfi_lof_int_v;
                            }
                            else
                            {
                                switch (client_mode)
                                {
                                    case DIGI_OTN_SERVER_OTU2F:
                                    case DIGI_OTN_SERVER_OTU1F:
                                    case DIGI_OTN_SERVER_OTU2E:
                                    case DIGI_OTN_SERVER_OTU1E:
                                    case DIGI_OTN_SERVER_OTU2:
                                        *defect_status = otu_int_table->opsm23_rx_lane_lof_loflane_int_v[0];
                                        break;

                                    default:
                                        *defect_status = otu_int_table->opsm23_rx_otu_lof_int_v;
                                        break;
                                }
                            }
                        }
                        break;

                    case DIGI_OTN_OOF:
                        if (DIGI_OTN_SERVER_OTU4 == client_mode)
                        {
                            *defect_status = otu_int_table->opsm4_rx_otu_oof_int_v;
                        }
                        else
                        {
                            if (DIGI_SERDES_SFI_51_INTF == adpt_handle->dev_info->serdes_type[serdes_port])
                            {
                                *defect_status = otu_int_table->opsm23_rx_sfi_oof_int_v;
                            }
                            else
                            {
                                switch (client_mode)
                                {
                                    case DIGI_OTN_SERVER_OTU2F:
                                    case DIGI_OTN_SERVER_OTU1F:
                                    case DIGI_OTN_SERVER_OTU2E:
                                    case DIGI_OTN_SERVER_OTU1E:
                                    case DIGI_OTN_SERVER_OTU2:
                                        *defect_status = otu_int_table->opsm23_rx_lane_oof_int_v[0];
                                        break;

                                    default:
                                        *defect_status = otu_int_table->opsm23_rx_otu_oof_int_v;
                                        break;
                                }
                            }
                        }
                        break;

                    case DIGI_OTN_LOM:
                        *defect_status = otu_int_table->otu_frm.otu_frm_d_lom_v;
                        break;

                    case DIGI_OTN_OOM:
                        *defect_status = otu_int_table->otu_frm.otu_frm_oom_v;
                        break;

                    case DIGI_OTN_TIM:
                        *defect_status = otu_int_table->otu_frm.otu_sk_d_tim_v;
                        break;

                    case DIGI_OTN_BDI:
                        *defect_status = otu_int_table->otu_frm.otu_sk_d_bdi_v;
                        break;

                    case DIGI_OTN_DEG:
                        rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
                        ADPT_RETURN_VERIFY(rc);

                        if ((DIGI_OTN_HARDWARE_DDEG == deg_mode) || (odu_seg != odu_seg_org))
                        {
                            *defect_status = otu_int_table->otu_frm.otu_sk_d_deg_v;
                        }
                        else
                        {
                            *defect_status = otu_int_table->otu_frm.otu_sk_d_deg_bursty_v;
                        }
                        break;

                    case DIGI_OTN_EDEG:
                        *defect_status = otu_int_table->otu_frm.otu_sk_d_deg_enh_v;
                        break;

                    case DIGI_OTN_IAE:
                        *defect_status = otu_int_table->otu_frm.otu_sk_d_iae_v;
                        break;

                    case DIGI_OTN_BIAE:
                        *defect_status = otu_int_table->otu_frm.otu_sk_d_biae_v;
                        break;

                    case DIGI_OTN_TSF:
                        *defect_status = otu_int_table->otu_frm.otu_sk_a_tsf_v;
                        break;

                    case DIGI_OTN_TSD:
                        *defect_status = otu_int_table->otu_frm.otu_sk_a_tsd_v;
                        break;

                    case DIGI_OTN_SSF:
                        *defect_status = otu_int_table->otu_frm.otu_sk_ci_ssf_v;
                        break;

                    default:
                        rc = PMC_ERR_INVALID_PARAMETERS;
                        break;
                }
            }
            else
            {
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
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                                }
                                else
                                {
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
                                }
                            }
                            else
                            {
                                if (DIGI_OTN_LINE_SIDE == location)
                                {
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].status.framer_chnl);
                                }
                                else
                                {
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].stg3b.framer_chnl);
                                }
                            }
                            break;
                            
                        case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].stg3b.framer_chnl);
                            }
                            break;

                        case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
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

                    frm_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].fo2.status.framer_chnl);
                }
                else
                {
                    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                }

                switch (defect_id)
                {
                    case DIGI_OTN_LOFLOM:
                        *defect_status = frm_int_table->rfrm.dloflom_v;
                        break;
                        
                    case DIGI_OTN_OOF:
                        *defect_status = frm_int_table->rfrm.oof_v;
                        break;

                    default:
                        rc = PMC_ERR_INVALID_PARAMETERS;
                        break;
                }
            }
            break;

        case DIGI_OTN_SEG_TCM1:
        case DIGI_OTN_SEG_TCM2:
        case DIGI_OTN_SEG_TCM3:
        case DIGI_OTN_SEG_TCM4:
        case DIGI_OTN_SEG_TCM5:
        case DIGI_OTN_SEG_TCM6:
        case DIGI_OTN_SEG_PM_NIM:
            tcm_index = (DIGI_OTN_SEG_PM_NIM == seg_id) ? DIGI_OTN_SEG_PM : seg_id;
            if (!(DIGI_TCM_EN_GET(adpt_handle->traffic_info[index].tcm_pool_id[location*2+DIGI_OTN_SINK_POS][tcm_index])))
            {
                *defect_status = FALSE;
                break;
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
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                            else
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                        }
                        else
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                            else
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                        }
                        break;
                        
                    case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        else
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        else
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
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

                tcm_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].fo2.status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                odu_seg_org = DIGI_OTN_DDEG_MON_CLIENT;
            }
            else
            {
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }

            switch (defect_id)
            {
                case DIGI_OTN_TIM:
                    *defect_status = tcm_int_table->dtim_v;
                    break;

                case DIGI_OTN_BDI:
                    *defect_status = tcm_int_table->dbdi_v;
                    break;

                case DIGI_OTN_DEG:
                    rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
                    ADPT_RETURN_VERIFY(rc);

                    if ((DIGI_OTN_HARDWARE_DDEG == deg_mode) || (odu_seg != odu_seg_org))
                    {
                        *defect_status = tcm_int_table->ddeg_v;
                    }
                    else
                    {
                        *defect_status = tcm_int_table->ddeg_bursty_v;
                    }
                    break;

                case DIGI_OTN_EDEG:
                    *defect_status = tcm_int_table->ddeg_enh_v;
                    break;

                case DIGI_OTN_LTC:
                    *defect_status = tcm_int_table->dltc_v;
                    break;

                case DIGI_OTN_LCK:
                    *defect_status = tcm_int_table->dlck_v;
                    break;

                case DIGI_OTN_OCI:
                    *defect_status = tcm_int_table->doci_v;
                    break;

                case DIGI_OTN_AIS:
                    *defect_status = tcm_int_table->dais_v;
                    break;

                case DIGI_OTN_IAE:
                    *defect_status = tcm_int_table->diae_v;
                    break;
                    
                case DIGI_OTN_BIAE:
                    *defect_status = tcm_int_table->dbiae_v;
                    break;

                case DIGI_OTN_TSF:
                    *defect_status = tcm_int_table->atsf_v;
                    break;

                case DIGI_OTN_TSD:
                    *defect_status = tcm_int_table->atsd_v;
                    break;

                case DIGI_OTN_SSF:
                    *defect_status = tcm_int_table->ci_ssf_v;
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
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
                            }
                        }
                        else
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].stg3b.framer_chnl);
                            }
                        }
                        break;
                        
                    case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.framer_chnl);
                        }
                        else
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].stg3b.framer_chnl);
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                        }
                        else
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
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

                frm_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].fo2.status.framer_chnl);
                status = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].status);
                odu_seg_org = DIGI_OTN_DDEG_MON_CLIENT;
            }
            else
            {
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }

            switch (defect_id)
            {
                case DIGI_OTN_TIM:
                    *defect_status = frm_int_table->rfrm.dtim_pm_v;
                    break;

                case DIGI_OTN_BDI:
                    *defect_status = frm_int_table->rfrm.dbdi_pm_v;
                    break;

                case DIGI_OTN_DEG:
                    rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
                    ADPT_RETURN_VERIFY(rc);

                    if ((DIGI_OTN_HARDWARE_DDEG == deg_mode) || (odu_seg != odu_seg_org))
                    {
                        *defect_status = frm_int_table->rfrm.ddeg_pm_v;
                    }
                    else
                    {
                        *defect_status = frm_int_table->rfrm.ddeg_bursty_pm_v;
                    }
                    break;

                case DIGI_OTN_EDEG:
                    *defect_status = frm_int_table->rfrm.ddeg_enh_pm_v;
                    break;

                case DIGI_OTN_LCK:
                    *defect_status = frm_int_table->rfrm.dlck_pm_v;
                    break;

                case DIGI_OTN_OCI:
                    *defect_status = frm_int_table->rfrm.doci_pm_v;
                    break;

                case DIGI_OTN_AIS:
                    *defect_status = frm_int_table->rfrm.dais_pm_v;
                    break;

                case DIGI_OTN_PLM:
                    if (NULL != adpt_handle->traffic_info[index].odu_handle)
                    {
                        *defect_status = frm_int_table->odtu_dmx.dplm_v;
                    }
                    else if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (status->rgmp_valid)
                        {
                            *defect_status = status->rgmp.dplm_v;
                        }
                        else
                        {
                            *defect_status = status->dplm_v;
                        }
                    }
                    break;

                case DIGI_OTN_MSIM:
                    *defect_status = frm_int_table->odtu_dmx.dmsim_v;
                    break;

                case DIGI_OTN_CSF:
                    if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (status->rgmp_valid)
                        {
                            *defect_status = status->rgmp.csf_v;
                        }
                        else
                        {
                            *defect_status = status->opu_csf_v;
                        }
                    }
                    break;

                case DIGI_OTN_LOOMFI:
                    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
                    {
                        if (DIGI_LINE_SIDE == traffic_loc)
                        {
                            *defect_status = adpt_handle->pmon_data->otn_defect.coreotn.fmf1_dmx.dloomfi_v;
                        }
                        else
                        {
                            *defect_status = adpt_handle->pmon_data->otn_defect.coreotn.fmf2_dmx.dloomfi_v;
                        }
                    }
                    else
                    {
                        *defect_status = FALSE;
                    }
                    break;

                case DIGI_OTN_TSF:
                    *defect_status = frm_int_table->rfrm.atsf_pm_v;
                    break;

                case DIGI_OTN_TSD:
                    *defect_status = frm_int_table->rfrm.atsd_pm_v;
                    break;

                case DIGI_OTN_SSF:
                    *defect_status = frm_int_table->rfrm.ci_ssf_pm_v;
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
} /* adpt_otn_defect_get */




/*******************************************************************************
* adpt_otn_defect_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the interrupt of OTN defects at the sink position.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   seg_id             - segment identity.
*   location           - line side(stage 1/2/3A/4/N) or system side (stage 3B).
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   1. The dLOM interrupt may be consistent under no signal input. The suitable 
*   workaround is that application softare need dynamically mask the dLOM defect
*   interrupt when a dLOF defect is detected and unmask it when the dLOF defect 
*   is cleared.
*
*   2. The following defects interrupts are supported in each segment.
*
*   DIGI_OTN_SEG_SM:  
*   OTUk -- dLOS, dLOL, dOTU-AIS(only for 10G), dLOF, dOOF, dLOM, dOOM, dTIM,
*   dBDI, dDEG, dEDEG, dIAE, dBIAE, aTSF, aTSD and CI_SSF.
*   ODUj[/i] -- dLOFLOM and dOOF.
*
*   DIGI_OTN_SEG_TCMi:
*   dTIM, dBDI, dDEG, dEDEG, dLTC, dLCK, dOCI, dAIS, dIAE, dBIAE, aTSF, aTSD.
*
*   DIGI_OTN_SEG_PM:
*   dTIM, dBDI, dDEG, dEDEG, dLTC, dLCK, dOCI, dAIS, dPLM, dMSIM, dCSF, dLOOMFI,
*   aTSF, aTSD, cnd_max_i, jc_nomtch_lossync_i/jc_err_syncloss_i.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_defect_i_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_defect_t defect_id,
                                   BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    coreotn_int_framer_chnl_t *frm_int_table = NULL;
    odu_rfrm_int_pid_chnl_t *tcm_int_table = NULL;
    lineotn_int_chnl_t *otu_int_table = NULL;
    UINT32 otu_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 client_mode = 0;
    digi_odu_struct_odu_level_t odu_stage = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    mapotn_int_chnl_t *status = NULL;
    digi_otn_ddeg_mon_seg_t odu_seg_org = DIGI_OTN_DDEG_MON_LINE;
    digi_otn_ddeg_mon_seg_t odu_seg = DIGI_OTN_DDEG_MON_LINE;
    digi_otn_ddeg_mode_t deg_mode = DIGI_OTN_HARDWARE_DDEG;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(defect_i);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* workaround on 3x40G CFP card */
    if ((DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type)
        && (DIGI_SYS_SIDE == traffic_loc))
    {
        serdes_port = serdes_port + adpt_handle->dev_info->num_line_serdes_port;
        traffic_loc = DIGI_LINE_SIDE;
    }

    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX) || (traffic_loc >= DIGI_SIDE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* default value */
    *defect_i = FALSE;

    switch (seg_id)
    {
        case DIGI_OTN_SEG_SM:
            if ((NULL != adpt_handle->traffic_info[index].otu_handle) && (DIGI_OTN_LINE_SIDE == location))
            {
                /* get OTU channel */
                rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
                ADPT_RETURN_VERIFY(rc);

                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    otu_int_table = &(adpt_handle->pmon_data->otn_defect.otn_server_chnl[otu_chnl]);
                }
                else
                {
                    otu_int_table = &(adpt_handle->pmon_data->otn_defect.sysotn_server_chnl[otu_chnl]);
                }

                switch (defect_id)
                {
                    case DIGI_OTN_LOS:
                        /* only availabe for 10G OTN */
                        switch (client_mode)
                        {
                            case DIGI_OTN_SERVER_OTU2F:
                            case DIGI_OTN_SERVER_OTU1F:
                            case DIGI_OTN_SERVER_OTU2E:
                            case DIGI_OTN_SERVER_OTU1E:
                            case DIGI_OTN_SERVER_OTU2:
                                *defect_i = otu_int_table->opsm23_rx_lane_los_int_i[0];
                                otu_int_table->opsm23_rx_lane_los_int_i[0] = FALSE;
                                break;

                            default:
                                *defect_i = FALSE;
                                break;
                        }
                        break;
                        
                    case DIGI_OTN_LOL:
                        if (DIGI_OTN_SERVER_OTU4 == client_mode)
                        {
                            *defect_i = otu_int_table->opsm4_rx_lol_int_i;
                            otu_int_table->opsm4_rx_lol_int_i = FALSE;
                        }
                        else
                        {
                            *defect_i = otu_int_table->opsm23_rx_lol_int_i;
                            otu_int_table->opsm23_rx_lol_int_i = FALSE;
                        }
                        break;

                    case DIGI_OTN_OTUAIS:
                        /* only availabe for 10G OTN */
                        switch (client_mode)
                        {
                            case DIGI_OTN_SERVER_OTU2F:
                            case DIGI_OTN_SERVER_OTU1F:
                            case DIGI_OTN_SERVER_OTU2E:
                            case DIGI_OTN_SERVER_OTU1E:
                            case DIGI_OTN_SERVER_OTU2:
                                *defect_i = otu_int_table->opsm23_rx_lane_otuk_ais_int_i[0];
                                otu_int_table->opsm23_rx_lane_otuk_ais_int_i[0] = FALSE;
                                break;

                            default:
                                *defect_i = FALSE;
                                break;
                        }
                        break;

                    case DIGI_OTN_LOF:
                        if (DIGI_OTN_SERVER_OTU4 == client_mode)
                        {
                            *defect_i = otu_int_table->opsm4_rx_otu_lof_int_i;
                            otu_int_table->opsm4_rx_otu_lof_int_i = FALSE;
                        }
                        else
                        {
                            if (DIGI_SERDES_SFI_51_INTF == adpt_handle->dev_info->serdes_type[serdes_port])
                            {
                                *defect_i = otu_int_table->opsm23_rx_sfi_lof_int_i;
                                otu_int_table->opsm23_rx_sfi_lof_int_i = FALSE;
                            }
                            else
                            {
                                switch (client_mode)
                                {
                                    case DIGI_OTN_SERVER_OTU2F:
                                    case DIGI_OTN_SERVER_OTU1F:
                                    case DIGI_OTN_SERVER_OTU2E:
                                    case DIGI_OTN_SERVER_OTU1E:
                                    case DIGI_OTN_SERVER_OTU2:
                                        *defect_i = otu_int_table->opsm23_rx_lane_lof_loflane_int_i[0];
                                        otu_int_table->opsm23_rx_lane_lof_loflane_int_i[0] = FALSE;
                                        break;

                                    default:
                                        *defect_i = otu_int_table->opsm23_rx_otu_lof_int_i;
                                        otu_int_table->opsm23_rx_otu_lof_int_i = FALSE;
                                        break;
                                }
                            }
                        }
                        break;

                    case DIGI_OTN_OOF:
                        if (DIGI_OTN_SERVER_OTU4 == client_mode)
                        {
                            *defect_i = otu_int_table->opsm4_rx_otu_oof_int_i;
                            otu_int_table->opsm4_rx_otu_oof_int_i = FALSE;
                        }
                        else
                        {
                            if (DIGI_SERDES_SFI_51_INTF == adpt_handle->dev_info->serdes_type[serdes_port])
                            {
                                *defect_i = otu_int_table->opsm23_rx_sfi_oof_int_i;
                                otu_int_table->opsm23_rx_sfi_oof_int_i = FALSE;
                            }
                            else
                            {
                                switch (client_mode)
                                {
                                    case DIGI_OTN_SERVER_OTU2F:
                                    case DIGI_OTN_SERVER_OTU1F:
                                    case DIGI_OTN_SERVER_OTU2E:
                                    case DIGI_OTN_SERVER_OTU1E:
                                    case DIGI_OTN_SERVER_OTU2:
                                        *defect_i = otu_int_table->opsm23_rx_lane_oof_int_i[0];
                                        otu_int_table->opsm23_rx_lane_oof_int_i[0] = FALSE;
                                        break;

                                    default:
                                        *defect_i = otu_int_table->opsm23_rx_otu_oof_int_i;
                                        otu_int_table->opsm23_rx_otu_oof_int_i = FALSE;
                                        break;
                                }
                            }
                        }
                        break;

                    case DIGI_OTN_LOM:
                        *defect_i = otu_int_table->otu_frm.otu_frm_d_lom_i;
                        otu_int_table->otu_frm.otu_frm_d_lom_i = FALSE;
                        break;

                    case DIGI_OTN_OOM:
                        *defect_i = otu_int_table->otu_frm.otu_frm_oom_i;
                        otu_int_table->otu_frm.otu_frm_oom_i = FALSE;
                        break;

                    case DIGI_OTN_TIM:
                        *defect_i = otu_int_table->otu_frm.otu_sk_d_tim_i;
                        otu_int_table->otu_frm.otu_sk_d_tim_i = FALSE;
                        break;

                    case DIGI_OTN_BDI:
                        *defect_i = otu_int_table->otu_frm.otu_sk_d_bdi_i;
                        otu_int_table->otu_frm.otu_sk_d_bdi_i = FALSE;
                        break;

                    case DIGI_OTN_DEG:
                        rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
                        ADPT_RETURN_VERIFY(rc);

                        if ((DIGI_OTN_HARDWARE_DDEG == deg_mode) || (odu_seg != odu_seg_org))
                        {
                            *defect_i = otu_int_table->otu_frm.otu_sk_d_deg_i;
                            otu_int_table->otu_frm.otu_sk_d_deg_i = FALSE;
                        }
                        else
                        {
                            *defect_i = otu_int_table->otu_frm.otu_sk_d_deg_bursty_i;
                            otu_int_table->otu_frm.otu_sk_d_deg_bursty_i = FALSE;
                        }
                        break;

                    case DIGI_OTN_EDEG:
                        *defect_i = otu_int_table->otu_frm.otu_sk_d_deg_enh_i;
                        otu_int_table->otu_frm.otu_sk_d_deg_enh_i = FALSE;
                        break;

                    case DIGI_OTN_IAE:
                        *defect_i = otu_int_table->otu_frm.otu_sk_d_iae_i;
                        otu_int_table->otu_frm.otu_sk_d_iae_i = FALSE;
                        break;

                    case DIGI_OTN_BIAE:
                        *defect_i = otu_int_table->otu_frm.otu_sk_d_biae_i;
                        otu_int_table->otu_frm.otu_sk_d_biae_i = FALSE;
                        break;

                    case DIGI_OTN_TSF:
                        *defect_i = otu_int_table->otu_frm.otu_sk_a_tsf_i;
                        otu_int_table->otu_frm.otu_sk_a_tsf_i = FALSE;
                        break;

                    case DIGI_OTN_TSD:
                        *defect_i = otu_int_table->otu_frm.otu_sk_a_tsd_i;
                        otu_int_table->otu_frm.otu_sk_a_tsd_i = FALSE;
                        break;

                    case DIGI_OTN_SSF:
                        *defect_i = otu_int_table->otu_frm.otu_sk_ci_ssf_i;
                        otu_int_table->otu_frm.otu_sk_ci_ssf_i = FALSE;
                        break;

                    default:
                        rc = PMC_ERR_INVALID_PARAMETERS;
                        break;
                }
            }
            else
            {
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
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                                }
                                else
                                {
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
                                }
                            }
                            else
                            {
                                if (DIGI_OTN_LINE_SIDE == location)
                                {
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].status.framer_chnl);
                                }
                                else
                                {
                                    frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].stg3b.framer_chnl);
                                }
                            }
                            break;

                        case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].stg3b.framer_chnl);
                            }
                            break;

                        case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
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

                    frm_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].fo2.status.framer_chnl);
                }
                else
                {
                    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                }
                
                switch (defect_id)
                {
                    case DIGI_OTN_LOFLOM:
                        *defect_i = frm_int_table->rfrm.dloflom_i;
                        frm_int_table->rfrm.dloflom_i = FALSE;
                        break;

                    case DIGI_OTN_OOF:
                        *defect_i = frm_int_table->rfrm.oof_i;
                        frm_int_table->rfrm.oof_i = FALSE;
                        break;

                    default:
                        rc = PMC_ERR_INVALID_PARAMETERS;
                        break;
                }
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
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                            else
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                        }
                        else
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                            else
                            {
                                tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                            }
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        else
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                        }
                        else
                        {
                            tcm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
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

                tcm_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].fo2.status.pid.rx_tcm[seg_id-DIGI_OTN_SEG_TCM1]);
                odu_seg_org = DIGI_OTN_DDEG_MON_CLIENT;
            }
            else
            {
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }

            switch (defect_id)
            {
                case DIGI_OTN_TIM:
                    *defect_i = tcm_int_table->dtim_i;
                    tcm_int_table->dtim_i= FALSE;
                    break;

                case DIGI_OTN_BDI:
                    *defect_i = tcm_int_table->dbdi_i;
                    tcm_int_table->dbdi_i = FALSE;
                    break;

                case DIGI_OTN_DEG:
                    rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
                    ADPT_RETURN_VERIFY(rc);

                    if ((DIGI_OTN_HARDWARE_DDEG == deg_mode) || (odu_seg != odu_seg_org))
                    {
                        *defect_i = tcm_int_table->ddeg_i;
                        tcm_int_table->ddeg_i = FALSE;
                    }
                    else
                    {
                        *defect_i = tcm_int_table->ddeg_bursty_i;
                        tcm_int_table->ddeg_bursty_i = FALSE;
                    }
                    break;

                case DIGI_OTN_EDEG:
                    *defect_i = tcm_int_table->ddeg_enh_i;
                    tcm_int_table->ddeg_enh_i = FALSE;
                    break;

                case DIGI_OTN_LTC:
                    *defect_i = tcm_int_table->dltc_i;
                    tcm_int_table->dltc_i = FALSE;
                    break;

                case DIGI_OTN_LCK:
                    *defect_i = tcm_int_table->dlck_i;
                    tcm_int_table->dlck_i = FALSE;
                    break;

                case DIGI_OTN_OCI:
                    *defect_i = tcm_int_table->doci_i;
                    tcm_int_table->doci_i = FALSE;
                    break;

                case DIGI_OTN_AIS:
                    *defect_i = tcm_int_table->dais_i;
                    tcm_int_table->dais_i = FALSE;
                    break;

                case DIGI_OTN_IAE:
                    *defect_i = tcm_int_table->diae_i;
                    tcm_int_table->diae_i = FALSE;
                    break;

                case DIGI_OTN_BIAE:
                    *defect_i = tcm_int_table->dbiae_i;
                    tcm_int_table->dbiae_i = FALSE;
                    break;

                case DIGI_OTN_TSF:
                    *defect_i = tcm_int_table->atsf_i;
                    tcm_int_table->atsf_i = FALSE;
                    break;

                case DIGI_OTN_TSD:
                    *defect_i = tcm_int_table->atsd_i;
                    tcm_int_table->atsd_i = FALSE;
                    break;

                case DIGI_OTN_SSF:
                    *defect_i = FALSE;
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
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
                            }
                        }
                        else
                        {
                            if (DIGI_OTN_LINE_SIDE == location)
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].status.framer_chnl);
                            }
                            else
                            {
                                frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_ho_chnl[odu_chnl].stg3b.framer_chnl);
                            }
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].status.framer_chnl);
                        }
                        else
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_mo_chnl[odu_chnl].stg3b.framer_chnl);
                        }
                        break;

                    case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                        if (DIGI_OTN_LINE_SIDE == location)
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].status.framer_chnl);
                        }
                        else
                        {
                            frm_int_table = &(adpt_handle->pmon_data->otn_defect.otn_odu_lo_chnl[odu_chnl].stg3b.framer_chnl);
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

                frm_int_table = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].fo2.status.framer_chnl);
                status = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].status);
                odu_seg_org = DIGI_OTN_DDEG_MON_CLIENT;
            }
            else
            {
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }

            switch (defect_id)
            {
                case DIGI_OTN_TIM:
                    *defect_i = frm_int_table->rfrm.dtim_pm_i;
                    frm_int_table->rfrm.dtim_pm_i = FALSE;
                    break;

                case DIGI_OTN_BDI:
                    *defect_i = frm_int_table->rfrm.dbdi_pm_i;
                    frm_int_table->rfrm.dbdi_pm_i = FALSE;
                    break;

                case DIGI_OTN_DEG:
                    rc = adpt_otn_deg_mode_get(dev_id, &odu_seg, &deg_mode);
                    ADPT_RETURN_VERIFY(rc);

                    if ((DIGI_OTN_HARDWARE_DDEG == deg_mode) || (odu_seg != odu_seg_org))
                    {
                        *defect_i = frm_int_table->rfrm.ddeg_pm_i;
                        frm_int_table->rfrm.ddeg_pm_i = FALSE;
                    }
                    else
                    {
                        *defect_i = frm_int_table->rfrm.ddeg_bursty_pm_i;
                        frm_int_table->rfrm.ddeg_bursty_pm_i = FALSE;
                    }
                    break;

                case DIGI_OTN_EDEG:
                    *defect_i = frm_int_table->rfrm.ddeg_enh_pm_i;
                    frm_int_table->rfrm.ddeg_enh_pm_i = FALSE;
                    break;

                case DIGI_OTN_LTC:
                    *defect_i = FALSE;
                    break;

                case DIGI_OTN_LCK:
                    *defect_i = frm_int_table->rfrm.dlck_pm_i;
                    frm_int_table->rfrm.dlck_pm_i = FALSE;
                    break;

                case DIGI_OTN_OCI:
                    *defect_i = frm_int_table->rfrm.doci_pm_i;
                    frm_int_table->rfrm.doci_pm_i = FALSE;
                    break;

                case DIGI_OTN_AIS:
                    *defect_i = frm_int_table->rfrm.dais_pm_i;
                    frm_int_table->rfrm.dais_pm_i = FALSE;
                    break;

                case DIGI_OTN_PLM:
                    if (NULL != adpt_handle->traffic_info[index].odu_handle)
                    {
                        *defect_i = frm_int_table->odtu_dmx.dplm_i;
                        frm_int_table->odtu_dmx.dplm_i = FALSE;
                    }
                    else if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (status->rgmp_valid)
                        {
                            *defect_i = status->rgmp.dplm_i;
                            status->rgmp.dplm_i = FALSE;
                        }
                        else
                        {
                            *defect_i = status->dplm_i;
                            status->dplm_i = FALSE;
                        }
                    }
                    break;

                case DIGI_OTN_MSIM:
                    *defect_i = frm_int_table->odtu_dmx.dmsim_i;
                    frm_int_table->odtu_dmx.dmsim_i = FALSE;
                    break;

                case DIGI_OTN_CSF:
                    if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (status->rgmp_valid)
                        {
                            *defect_i = status->rgmp.csf_i;
                            status->rgmp.csf_i = FALSE;
                        }
                        else
                        {
                            *defect_i = status->opu_csf_i;
                            status->opu_csf_i = FALSE;
                        }
                    }
                    break;

                case DIGI_OTN_LOOMFI:
                    if (DIGI_ODUK_ODU4 == adpt_handle->traffic_info[index].oduk_type)
                    {
                        if (DIGI_LINE_SIDE == traffic_loc)
                        {
                            *defect_i = adpt_handle->pmon_data->otn_defect.coreotn.fmf1_dmx.dloomfi_i;
                            adpt_handle->pmon_data->otn_defect.coreotn.fmf1_dmx.dloomfi_i = FALSE;
                        }
                        else
                        {
                            *defect_i = adpt_handle->pmon_data->otn_defect.coreotn.fmf2_dmx.dloomfi_i;
                            adpt_handle->pmon_data->otn_defect.coreotn.fmf2_dmx.dloomfi_i = FALSE;
                        }
                    }
                    else
                    {
                        *defect_i = FALSE;
                    }
                    break;

                case DIGI_OTN_TSF:
                    *defect_i = frm_int_table->rfrm.atsf_pm_i;
                    frm_int_table->rfrm.atsf_pm_i = FALSE;
                    break;

                case DIGI_OTN_TSD:
                    *defect_i = frm_int_table->rfrm.atsd_pm_i;
                    frm_int_table->rfrm.atsd_pm_i = FALSE;
                    break;

                case DIGI_OTN_SSF:
                    *defect_i = FALSE;
                    break;

                case DIGI_OTN_CND_MAX:
                    if (NULL != adpt_handle->traffic_info[index].odu_handle)
                    {
                        *defect_i = frm_int_table->odtu_dmx.cnd_max_i;
                        frm_int_table->odtu_dmx.cnd_max_i = FALSE;
                    }
                    else if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (status->rgmp_valid)
                        {
                            *defect_i = status->rgmp.cnd_max_i;
                            status->rgmp.cnd_max_i = FALSE;
                        }
                        else
                        {
                            *defect_i = FALSE;
                        }
                    }
                    break;

                case DIGI_OTN_JC_SYNCLOSS:
                    if (NULL != adpt_handle->traffic_info[index].odu_handle)
                    {
                        *defect_i = frm_int_table->odtu_dmx.jc_nomtch_lossync_i;
                        frm_int_table->odtu_dmx.jc_nomtch_lossync_i = FALSE;
                    }
                    else if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (status->rgmp_valid)
                        {
                            *defect_i = status->rgmp.jc_err_syncloss_i;
                            status->rgmp.jc_err_syncloss_i = FALSE;
                        }
                        else
                        {
                            *defect_i = FALSE;
                        }
                    }
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
} /* adpt_otn_defect_i_get */


/*******************************************************************************
* adpt_otn_mld_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted MLD layer defects status at the sink position. The lane 
*   number is 20 for OTU4, and 4 for OTU3.  
*
*   Please note that this API is available under OTL interface. The following 
*   defect ID list is available.
*    DIGI_OTN_LOS,
*    DIGI_OTN_OOF,
*    DIGI_OTN_OTUAIS,
*    DIGI_OTN_LOFLANE,
*    DIGI_OTN_OOR,
*    DIGI_OTN_LOR,
*    DIGI_OTN_OOM,
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - logical lane identity. OTU4: 0 ~ 19, OTU3: 0 ~ 3.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: MLD defect is raised.
*                        FALSE: MLD defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_mld_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                UINT32 lane_id,
                                digi_otn_defect_t defect_id,
                                BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 client_mode = 0;
    UINT32 otu_chnl = 0;
    lineotn_int_chnl_t *otu_int_table = NULL;
    UINT32 traffic_loc = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(defect_status);

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
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
    if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
    {
        if (lane_id >= 20)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        if (lane_id >= 4)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }

    /* get OTU channel */
    rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        otu_int_table = &(adpt_handle->pmon_data->otn_defect.otn_server_chnl[otu_chnl]);
    }
    else
    {
        otu_int_table = &(adpt_handle->pmon_data->otn_defect.sysotn_server_chnl[otu_chnl]);
    }

    switch (defect_id)
    {
        case DIGI_OTN_LOS:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_los_int_v[lane_id];
            }
            else
            {
                *defect_status = otu_int_table->opsm23_rx_lane_los_int_v[lane_id];
            }
            break;

        case DIGI_OTN_OOF:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_oof_int_v[lane_id];
            }
            else
            {
                *defect_status = otu_int_table->opsm23_rx_lane_oof_int_v[lane_id];
            }
            break;

        case DIGI_OTN_OTUAIS:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_otuk_ais_int_v[lane_id];
            }
            else
            {
                *defect_status = otu_int_table->opsm23_rx_lane_otuk_ais_int_v[lane_id];
            }
            break;

        case DIGI_OTN_LOFLANE:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_lof_loflane_int_v[lane_id];
            }
            else
            {
                *defect_status = otu_int_table->opsm23_rx_lane_lof_loflane_int_v[lane_id];
            }
            break;

        case DIGI_OTN_OOR:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_oor_int_v[lane_id];
            }
            else
            {
                *defect_status = otu_int_table->opsm23_rx_lane_oor_int_v[lane_id];
            }
            break;

        case DIGI_OTN_LOR:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_lor_int_v[lane_id];
            }
            else
            {
                *defect_status = otu_int_table->opsm23_rx_lane_lor_int_v[lane_id];
            }
            break;

        case DIGI_OTN_OOM:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_status = otu_int_table->opsm4_rx_lane_oom_int_v[lane_id];
            }
            else
            {
                *defect_status = FALSE;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_mld_get */




/*******************************************************************************
* adpt_otn_mld_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the MLD layer defects interrupt at the sink position. The lane 
*   number is 20 for OTU4, and 4 for OTU3.  
*
*   Please note that this API is available under OTL interface. The following 
*   defect ID list is available.
*    DIGI_OTN_LOS,
*    DIGI_OTN_OOF,
*    DIGI_OTN_OTUAIS,
*    DIGI_OTN_LOFLANE,
*    DIGI_OTN_OOR,
*    DIGI_OTN_LOR,
*    DIGI_OTN_OOM,
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - logical lane identity. OTU4: 20, OTU3: 4.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: MLD defect interrupt has been raised.
*                        FALSE: MLD defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_mld_i_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 lane_id,
                                 digi_otn_defect_t defect_id,
                                 BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 client_mode = 0;
    UINT32 otu_chnl = 0;
    lineotn_int_chnl_t *otu_int_table = NULL;
    UINT32 traffic_loc = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(defect_i);

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

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
    if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
    {
        if (lane_id >= 20)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        if (lane_id >= 4)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }

    /* get OTU channel */
    rc = adpt_otu_chnl_get(dev_id, chnl_id, &otu_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        otu_int_table = &(adpt_handle->pmon_data->otn_defect.otn_server_chnl[otu_chnl]);
    }
    else
    {
        otu_int_table = &(adpt_handle->pmon_data->otn_defect.sysotn_server_chnl[otu_chnl]);
    }

    /* default value */
    *defect_i = FALSE;

    switch (defect_id)
    {
        case DIGI_OTN_LOS:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_los_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_los_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = otu_int_table->opsm23_rx_lane_los_int_i[lane_id];
                otu_int_table->opsm23_rx_lane_los_int_i[lane_id] = FALSE;
            }
            break;

        case DIGI_OTN_OOF:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_oof_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_oof_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = otu_int_table->opsm23_rx_lane_oof_int_i[lane_id];
                otu_int_table->opsm23_rx_lane_oof_int_i[lane_id] = FALSE;
            }
            break;

        case DIGI_OTN_OTUAIS:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_otuk_ais_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_otuk_ais_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = otu_int_table->opsm23_rx_lane_otuk_ais_int_i[lane_id];
                otu_int_table->opsm23_rx_lane_otuk_ais_int_i[lane_id] = FALSE;
            }
            break;

        case DIGI_OTN_LOFLANE:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_lof_loflane_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_lof_loflane_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = otu_int_table->opsm23_rx_lane_lof_loflane_int_i[lane_id];
                otu_int_table->opsm23_rx_lane_lof_loflane_int_i[lane_id] = FALSE;
            }
            break;

        case DIGI_OTN_OOR:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_oor_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_oor_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = otu_int_table->opsm23_rx_lane_oor_int_i[lane_id];
                otu_int_table->opsm23_rx_lane_oor_int_i[lane_id] = FALSE;
            }
            break;

        case DIGI_OTN_LOR:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_lor_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_lor_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = otu_int_table->opsm23_rx_lane_lor_int_i[lane_id];
                otu_int_table->opsm23_rx_lane_lor_int_i[lane_id] = FALSE;
            }
            break;

        case DIGI_OTN_OOM:
            if (DIGI_ODUK_ODU4 == adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode))
            {
                *defect_i = otu_int_table->opsm4_rx_lane_oom_int_i[lane_id];
                otu_int_table->opsm4_rx_lane_oom_int_i[lane_id] = FALSE;
            }
            else
            {
                *defect_i = FALSE;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_otn_mld_i_get */


/*******************************************************************************
* adpt_enet_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet defect information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The ENET defects and interrupts in the Tx direction can only be retrieved 
*   correctly when the ODUk SW/CPB has been created in this direction. There is 
*   no limitation on the ENET defects and interrupts in the Rx direction.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_enet_defect_t defect_id,
                                  BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    emac_int_chnl_t *emac_ptr = NULL;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 src_index = 0;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_status);

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
                        emac_ptr = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.emac);
                    }
                    else
                    {
                        /* not support in Tx for ETRANS datapath */
                        *defect_status = FALSE;
                        PMC_RETURN(rc);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL == emac_ptr)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                emac_ptr = &(adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.emac);
            }
            else
            {
                emac_ptr = &(adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.emac);
            }
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                emac_ptr = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_pmon.emac);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    emac_ptr = &(adpt_handle->pmon_data->sifd_defect.enet_pmon_int_chnl[enet_chnl].enet_pmon.emac);
                }
            }

            if (NULL == emac_ptr)
            {
                *defect_status = FALSE;
                PMC_RETURN(rc);
            }
        }
    }

    switch (defect_id)
    {
        case DIGI_ENET_LOS:
            *defect_status = emac_ptr->phy_los_v;
            break;

        case DIGI_ENET_LF:
            *defect_status = emac_ptr->rx_loc_fault_v;
            break;

        case DIGI_ENET_RF:
            *defect_status = emac_ptr->rx_rem_fault_v;
            break;

        case DIGI_ENET_HI_BER:
            *defect_status = emac_ptr->hi_ber_v;
            break;

        case DIGI_ENET_BLK_LCK:
            *defect_status = emac_ptr->block_lock_v ? FALSE : TRUE;
            break;

        case DIGI_ENET_ALIGN_STATUS:
            *defect_status = emac_ptr->align_status_v ? FALSE : TRUE;
            break;

        case DIGI_ENET_LINK_DOWN:
            *defect_status = emac_ptr->link_status ? FALSE : TRUE;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_defect_get */


/*******************************************************************************
* adpt_enet_defect_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet defect interrupt information.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The ENET defects and interrupts in the Tx direction can only be retrieved 
*   correctly when the ODUk SW/CPB has been created in this direction. There is 
*   no limitation on the ENET defects and interrupts in the Rx direction.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_defect_i_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_enet_defect_t defect_id,
                                    BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    emac_int_chnl_t *emac_ptr = NULL;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    UINT32 src_index = 0;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_i);

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
                        emac_ptr = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.emac);
                    }
                    else
                    {
                        /* not support in Tx for ETRANS datapath */
                        *defect_i = FALSE;
                        PMC_RETURN(rc);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    if (NULL == emac_ptr)
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* PMON location */
        if (DIGI_PORT_DIR_RX == dir)
        {
            if (DIGI_LINE_SIDE == traffic_loc)
            {
                emac_ptr = &(adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.emac);
            }
            else
            {
                emac_ptr = &(adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.emac);
            }
        }
        else
        {
            if (adpt_handle->traffic_info[index].map_enable)
            {
                emac_ptr = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_pmon.emac);
            }
            else if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);
                if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
                {
                    emac_ptr = &(adpt_handle->pmon_data->sifd_defect.enet_pmon_int_chnl[enet_chnl].enet_pmon.emac);
                }
            }

            if (NULL == emac_ptr)
            {
                *defect_i = FALSE;
                PMC_RETURN(rc);
            }
        }
    }

    switch (defect_id)
    {
        case DIGI_ENET_LOS:
            *defect_i = emac_ptr->phy_los_i;
            emac_ptr->phy_los_i = FALSE;
            break;

        case DIGI_ENET_LF:
            *defect_i = emac_ptr->rx_loc_fault_i;
            emac_ptr->rx_loc_fault_i = FALSE;
            break;

        case DIGI_ENET_RF:
            *defect_i = emac_ptr->rx_rem_fault_i;
            emac_ptr->rx_rem_fault_i = FALSE;
            break;

        case DIGI_ENET_HI_BER:
            *defect_i = emac_ptr->hi_ber_i;
            emac_ptr->hi_ber_i = FALSE;
            break;

        case DIGI_ENET_BLK_LCK:
            *defect_i = emac_ptr->block_lock_i;
            emac_ptr->block_lock_i = FALSE;
            break;

        case DIGI_ENET_ALIGN_STATUS:
            *defect_i = emac_ptr->align_status_i;
            emac_ptr->align_status_i = FALSE;
            break;

        case DIGI_ENET_LINK_DOWN:
            *defect_i = FALSE;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_defect_i_get */


/*******************************************************************************
* adpt_enet_pcs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet defect information of PCS layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   lane_id            - logical lane identity. 100G: 20, 40G: 4.
*   defect_id          - defect identity list.
*                        DIGI_ENET_BLK_LCK
*                        DIGI_ENET_LOAM
*                        DIGI_ENET_HI_BER (Channel level item)
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The ETRANS PCS defects and interrupts can only be retrieved correctly
*   when the client mapping is configured through adpt_client_map_cfg().
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_pcs_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                UINT32 lane_id,
                                digi_enet_defect_t defect_id,
                                BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    BOOL ilkn_en = FALSE;
    exco4m_int_chnl_t *exco4m = NULL;
    exco3m_int_chnl_t *exco3m = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_status);

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

    /* default value */
    *defect_status = FALSE;

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* retrieve ETRANS interrupt/defect through map_handle */
        if (adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco4m_valid)
        {
            exco4m = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco4m);
        }
        else if (adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco3m_valid)
        {
            exco3m = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco3m);
        }

        /* the datapath of ODUk SW <> MAPOTN <> ETRANS <> CPB <> ILKN */
        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            ilkn_en = TRUE;
        }
    }

    if ((NULL == exco4m) && (NULL == exco3m))
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        if (DIGI_LINE_SIDE == traffic_loc)
        {
            if (adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m_valid)
            {
                exco4m = &(adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m);
            }
            else if (adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m_valid)
            {
                exco3m = &(adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m);
            }
        }
        else
        {
            if (adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m_valid)
            {
                exco4m = &(adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m);
            }
            else if (adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m_valid)
            {
                exco3m = &(adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m);
            }
        }
    }

    /* parameter check */
    if (NULL != exco4m)
    {
        if (lane_id >= 20)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else if (NULL != exco3m)
    {
        if (lane_id >= 4)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        PMC_RETURN(rc);
    }

    switch (defect_id)
    {
        case DIGI_ENET_BLK_LCK:
            if (NULL != exco4m)
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_status = exco4m->tx_pcs_lobl_v[lane_id];
                }
                else
                {
                    *defect_status = exco4m->rx_lobl_v[lane_id];
                }
            }
            else
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_status = exco3m->tx_pcs_lobl_v[lane_id];
                }
                else
                {
                    *defect_status = exco3m->rx_1027b_lobl_v;
                }
            }
            break;

        case DIGI_ENET_LOAM:
            if (NULL != exco4m)
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_status = exco4m->tx_loam_v[lane_id];
                }
                else
                {
                    *defect_status = exco4m->rx_loam_v[lane_id];
                }
            }
            else
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_status = exco3m->tx_loam_v[lane_id];
                }
                else
                {
                    *defect_status = exco3m->rx_loam_v[lane_id];
                }
            }
            break;

        case DIGI_ENET_HI_BER:
            if (NULL != exco4m)
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_status = exco4m->tx_pcs_hber_v;
                }
                else
                {
                    *defect_status = exco4m->rx_pcs_hber_v;
                }
            }
            else
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_status = exco3m->tx_pcs_hber_v;
                }
                else
                {
                    *defect_status = exco3m->rx_1027b_hber_v;
                }
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_pcs_get */


/*******************************************************************************
* adpt_enet_pcs_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Ethernet defect interrupt information of PCS layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - logical lane identity. 100G: 20, 40G: 4.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*                        DIGI_ENET_BLK_LCK
*                        DIGI_ENET_LOAM
*                        DIGI_ENET_HI_BER (Channel level item)
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The ETRANS PCS defects and interrupts can only be retrieved correctly
*   when the client mapping is configured through adpt_client_map_cfg().
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_pcs_i_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT32 lane_id,
                                  digi_enet_defect_t defect_id,
                                  BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 traffic_loc = 0;
    UINT32 enet_chnl = 0;
    UINT32 gfp_chnl = 0;
    UINT32 odu_chnl = 0;
    BOOL ilkn_en = FALSE;
    exco4m_int_chnl_t *exco4m = NULL;
    exco3m_int_chnl_t *exco3m = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_i);

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

    /* default value */
    *defect_i = FALSE;

    if (adpt_handle->traffic_info[index].map_enable)
    {
        /* get GFP channel */
        rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
        ADPT_RETURN_VERIFY(rc);

        /* retrieve ETRANS interrupt/defect through map_handle */
        if (adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco4m_valid)
        {
            exco4m = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco4m);
        }
        else if (adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco3m_valid)
        {
            exco3m = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].enet_etrans.etrans.exco3m);
        }

        /* the datapath of ODUk SW <> MAPOTN <> ETRANS <> CPB <> ILKN */
        if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
        {
            ilkn_en = TRUE;
        }
    }

    if ((NULL == exco4m) && (NULL == exco3m))
    {
        /* get enet channel ID */
        rc |= adpt_enet_chnl_get(dev_id, chnl_id, &enet_chnl);
        ADPT_RETURN_VERIFY(rc);

        if (DIGI_LINE_SIDE == traffic_loc)
        {
            if (adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m_valid)
            {
                exco4m = &(adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m);
            }
            else if (adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m_valid)
            {
                exco3m = &(adpt_handle->pmon_data->line_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m);
            }
        }
        else
        {
            if (adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m_valid)
            {
                exco4m = &(adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco4m);
            }
            else if (adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m_valid)
            {
                exco3m = &(adpt_handle->pmon_data->sys_enet_defect.enet_chnl[enet_chnl].enet.epmm.exco3m);
            }
        }
    }

    /* parameter check */
    if (NULL != exco4m)
    {
        if (lane_id >= 20)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else if (NULL != exco3m)
    {
        if (lane_id >= 4)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        PMC_RETURN(rc);
    }

    switch (defect_id)
    {
        case DIGI_ENET_BLK_LCK:
            if (NULL != exco4m)
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_i = exco4m->tx_pcs_lobl_i[lane_id];
                    exco4m->tx_pcs_lobl_i[lane_id] = 0;
                }
                else
                {
                    *defect_i = exco4m->rx_lobl_i[lane_id];
                    exco4m->rx_lobl_i[lane_id] = 0;
                }
            }
            else
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_i = exco3m->tx_pcs_lobl_i[lane_id];
                    exco3m->tx_pcs_lobl_i[lane_id] = 0;
                }
                else
                {
                    *defect_i = exco3m->rx_1027b_lobl_i;
                    exco3m->rx_1027b_lobl_i = 0;
                }
            }
            break;

        case DIGI_ENET_LOAM:
            if (NULL != exco4m)
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_i = exco4m->tx_loam_i[lane_id];
                    exco4m->tx_loam_i[lane_id] = 0;
                }
                else
                {
                    *defect_i = exco4m->rx_loam_i[lane_id];
                    exco4m->rx_loam_i[lane_id] = 0;
                }
            }
            else
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_i = exco3m->tx_loam_i[lane_id];
                    exco3m->tx_loam_i[lane_id] = 0;
                }
                else
                {
                    *defect_i = exco3m->rx_loam_i[lane_id];
                    exco3m->rx_loam_i[lane_id] = 0;
                }
            }
            break;

        case DIGI_ENET_HI_BER:
            if (NULL != exco4m)
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_i = exco4m->tx_pcs_hber_i;
                    exco4m->tx_pcs_hber_i = 0;
                }
                else
                {
                    *defect_i = exco4m->rx_pcs_hber_i;
                    exco4m->rx_pcs_hber_i = 0;
                }
            }
            else
            {
                if (((DIGI_PORT_DIR_RX == dir) && !ilkn_en) 
                    || ((DIGI_PORT_DIR_TX == dir) && ilkn_en))
                {
                    *defect_i = exco3m->tx_pcs_hber_i;
                    exco3m->tx_pcs_hber_i = 0;
                }
                else
                {
                    *defect_i = exco3m->rx_1027b_hber_i;
                    exco3m->rx_1027b_hber_i = 0;
                }
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_pcs_i_get */


/*******************************************************************************
* adpt_sdh_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted SONET/SDH defect status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_sdh_defect_t defect_id,
                                  BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    sdh_pmg_int_chnl_t *sdh_pmg = NULL;
    cbr_fsgm_int_chnl_t *cbr_fsgm = NULL;
    digi_sdh_pm_t bn_type = DIGI_SDH_B1;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        sdh_pmg  = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.sdh_pmg_rx);
        cbr_fsgm = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.cbr_fsgm_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        sdh_pmg  = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.sdh_pmg_tx);
        cbr_fsgm = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.cbr_fsgm_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
    
    switch (defect_id)
    {
        case DIGI_SDH_LOS:
            *defect_status = sdh_pmg->los_v;
            break;

        case DIGI_SDH_GAIS:
            *defect_status = cbr_fsgm->pn11_det_v;
            break;

        case DIGI_SDH_LOF:
            *defect_status = sdh_pmg->lof_v;
            break;

        case DIGI_SDH_OOF:
            *defect_status = sdh_pmg->oof_v;
            break;

        case DIGI_SDH_RS_TIM:
            *defect_status = sdh_pmg->tim_v;
            break;

        case DIGI_SDH_RS_TIU:
            *defect_status = sdh_pmg->tiu_v;
            break;

        case DIGI_SDH_RS_DEG:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B1 == bn_type)
            {
                *defect_status = sdh_pmg->sd_v;
            }
            else
            {
                *defect_status = FALSE;
            }
            break;

        case DIGI_SDH_RS_EXC:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B1 == bn_type)
            {
                *defect_status = sdh_pmg->sf_v;
            }
            else
            {
                *defect_status = FALSE;
            }
            break;

        case DIGI_SDH_MS_AIS:
            *defect_status = sdh_pmg->lais_v;
            break;

        case DIGI_SDH_MS_RDI:
            *defect_status = sdh_pmg->lrdi_v;
            break;

        case DIGI_SDH_MS_DEG:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B2 == bn_type)
            {
                *defect_status = sdh_pmg->sd_v;
            }
            else
            {
                *defect_status = FALSE;
            }
            break;

        case DIGI_SDH_MS_EXC:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B2 == bn_type)
            {
                *defect_status = sdh_pmg->sf_v;
            }
            else
            {
                *defect_status = FALSE;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_sdh_defect_get */


/*******************************************************************************
* adpt_sdh_defect_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the SONET/SDH defect interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_sdh_defect_i_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_sdh_defect_t defect_id,
                                   BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    sdh_pmg_int_chnl_t *sdh_pmg = NULL;
    cbr_fsgm_int_chnl_t *cbr_fsgm = NULL;
    digi_sdh_pm_t bn_type = DIGI_SDH_B1;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_i);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        sdh_pmg  = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.sdh_pmg_rx);
        cbr_fsgm = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.cbr_fsgm_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        sdh_pmg  = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.sdh_pmg_tx);
        cbr_fsgm = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.cbr_fsgm_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
    
    switch (defect_id)
    {
        case DIGI_SDH_LOS:
            *defect_i = sdh_pmg->los_i;
            sdh_pmg->los_i = FALSE;
            break;

        case DIGI_SDH_GAIS:
            *defect_i = cbr_fsgm->pn11_det_i;
            cbr_fsgm->pn11_det_i = FALSE;
            break;

        case DIGI_SDH_LOF:
            *defect_i = sdh_pmg->lof_i;
            sdh_pmg->lof_i = FALSE;
            break;

        case DIGI_SDH_OOF:
            *defect_i = sdh_pmg->oof_i;
            sdh_pmg->oof_i = FALSE;
            break;

        case DIGI_SDH_RS_TIM:
            *defect_i = sdh_pmg->tim_i;
            sdh_pmg->tim_i = FALSE;
            break;

        case DIGI_SDH_RS_TIU:
            *defect_i = sdh_pmg->tiu_i;
            sdh_pmg->tiu_i = FALSE;
            break;

        case DIGI_SDH_RS_DEG:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B1 == bn_type)
            {
                *defect_i = sdh_pmg->sd_i;
                sdh_pmg->sd_i = FALSE;
            }
            else
            {
                *defect_i = FALSE;
            }
            break;

        case DIGI_SDH_RS_EXC:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B1 == bn_type)
            {
                *defect_i = sdh_pmg->sf_i;
                sdh_pmg->sf_i = FALSE;
            }
            else
            {
                *defect_i = FALSE;
            }
            break;

        case DIGI_SDH_MS_AIS:
            *defect_i = sdh_pmg->lais_i;
            sdh_pmg->lais_i = FALSE;
            break;

        case DIGI_SDH_MS_RDI:
            *defect_i = sdh_pmg->lrdi_i;
            sdh_pmg->lrdi_i = FALSE;
            break;

        case DIGI_SDH_MS_DEG:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B2 == bn_type)
            {
                *defect_i = sdh_pmg->sd_i;
                sdh_pmg->sd_i = FALSE;
            }
            else
            {
                *defect_i = FALSE;
            }
            break;

        case DIGI_SDH_MS_EXC:
            rc = adpt_sdh_bn_type_get(dev_id, &bn_type);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SDH_B2 == bn_type)
            {
                *defect_i = sdh_pmg->sf_i;
                sdh_pmg->sf_i = FALSE;
            }
            else
            {
                *defect_i = FALSE;
            }
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_sdh_defect_i_get */


/*******************************************************************************
* adpt_gfp_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted defects status of the GFP frame.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_gfp_defect_t defect_id,
                                  BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    mapotn_int_chnl_t *gfp_int = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get GFP channel */
    rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
    ADPT_RETURN_VERIFY(rc);

    gfp_int = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].status);

    switch (defect_id)
    {
        case DIGI_GFP_LFD:
            *defect_status = gfp_int->lofd_v;
            break;

        case DIGI_GFP_EXM:
            *defect_status = gfp_int->exi_v;
            break;

        case DIGI_GFP_UPM:
            *defect_status = gfp_int->pti_upi_msmch_v;
            break;

        case DIGI_GFP_CSF:
            *defect_status = gfp_int->csf_v;
            break;

        case DIGI_GFP_IDLE:
            *defect_status = gfp_int->idle_v;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_gfp_defect_get */


/*******************************************************************************
* adpt_gfp_defect_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the defects interrupt of the GFP layer.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_gfp_defect_i_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_gfp_defect_t defect_id,
                                   BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 odu_chnl = 0;
    UINT32 gfp_chnl = 0;
    mapotn_int_chnl_t *gfp_int = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_i);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get GFP channel */
    rc = adpt_gfp_chnl_get(dev_id, chnl_id, &gfp_chnl, &odu_chnl);
    ADPT_RETURN_VERIFY(rc);

    gfp_int = &(adpt_handle->pmon_data->mapper_defect.mapper_chnl[gfp_chnl].status);

    switch (defect_id)
    {
        case DIGI_GFP_LFD:
            *defect_i = gfp_int->lofd_i;
            gfp_int->lofd_i = FALSE;
            break;

        case DIGI_GFP_EXM:
            *defect_i = gfp_int->exi_i;
            gfp_int->exi_i = FALSE;
            break;

        case DIGI_GFP_UPM:
            *defect_i = gfp_int->pti_upi_msmch_i;
            gfp_int->pti_upi_msmch_i = FALSE;
            break;

        case DIGI_GFP_CSF:
            *defect_i = gfp_int->csf_i;
            gfp_int->csf_i = FALSE;
            break;

        case DIGI_GFP_IDLE:
            *defect_i = gfp_int->idle_i;
            gfp_int->idle_i = FALSE;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_gfp_defect_i_get */


/*******************************************************************************
* adpt_fc1200_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted FC1200 defect status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc1200_defect_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_fc1200_defect_t defect_id,
                                    BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    cbrc_int_chnl_t *cbrc = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    cbrc = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc);

    switch (defect_id)
    {
        case DIGI_FC_RX_RF:
            *defect_status = cbrc->fc1200_pcs_rx_rf_v;
            break;

        case DIGI_FC_RX_LF:
            *defect_status = cbrc->fc1200_pcs_rx_lf_v;
            break;

        case DIGI_FC_RX_HIGH_BER:
            *defect_status = cbrc->fc1200_pcs_rx_high_ber_v;
            break;

        case DIGI_FC_RX_LOSS_SYNC:
            *defect_status = cbrc->fc1200_pcs_rx_loss_sync_v;
            break;

        case DIGI_FC_RX_LOS:
            *defect_status = cbrc->fc1200_pcs_rx_los_v;
            break;

        case DIGI_FC_TX_LF:
            *defect_status = cbrc->fc1200_pcs_tx_local_fault_v;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc1200_defect_get */


/*******************************************************************************
* adpt_fc1200_defect_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FC1200 defect interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc1200_defect_i_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_fc1200_defect_t defect_id,
                                      BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    cbrc_int_chnl_t *cbrc = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_i);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    cbrc = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc);

    switch (defect_id)
    {
        case DIGI_FC_RX_RF:
            *defect_i = cbrc->fc1200_pcs_rx_rf_i;
            cbrc->fc1200_pcs_rx_rf_i = FALSE;
            break;

        case DIGI_FC_RX_LF:
            *defect_i = cbrc->fc1200_pcs_rx_lf_i;
            cbrc->fc1200_pcs_rx_lf_i = FALSE;
            break;

        case DIGI_FC_RX_HIGH_BER:
            *defect_i = cbrc->fc1200_pcs_rx_high_ber_i;
            cbrc->fc1200_pcs_rx_high_ber_i = FALSE;
            break;

        case DIGI_FC_RX_LOSS_SYNC:
            *defect_i = cbrc->fc1200_pcs_rx_loss_sync_i;
            cbrc->fc1200_pcs_rx_loss_sync_i= FALSE;
            break;

        case DIGI_FC_RX_LOS:
            *defect_i = cbrc->fc1200_pcs_rx_los_i;
            cbrc->fc1200_pcs_rx_los_i = FALSE;
            break;

        case DIGI_FC_TX_LF:
            *defect_i = cbrc->fc1200_pcs_tx_local_fault_i;
            cbrc->fc1200_pcs_tx_local_fault_i = FALSE;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc1200_defect_i_get */


/*******************************************************************************
* adpt_fc800_defect_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the accepted FC800 defect status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_status     - TRUE: defect is raised.
*                        FALSE: defect is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc800_defect_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   digi_fc800_defect_t defect_id,
                                   BOOL *defect_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    fc800_pmg_int_chnl_t *fc800_pmg = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        fc800_pmg = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.fc800_pmg_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        fc800_pmg = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.fc800_pmg_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    switch (defect_id)
    {
        case DIGI_FC_DISP_INV:
            *defect_status = fc800_pmg->rx_disp_invert_v;
            break;

        case DIGI_FC_XDET:
            *defect_status = fc800_pmg->rx_xdet_v;
            break;

        case DIGI_FC_DLOLB:
            *defect_status = fc800_pmg->rx_dlolb_v;
            break;

        case DIGI_FC_ASD:
            *defect_status = fc800_pmg->rx_asd_v;
            break;

        case DIGI_FC_SYNC:
            *defect_status = fc800_pmg->rx_sync_v;
            break;

        case DIGI_FC_LCV:
            *defect_status = fc800_pmg->rx_lcv_v;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc800_defect_get */


/*******************************************************************************
* adpt_fc800_defect_i_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FC800 defect interrupt.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   defect_id          - defect identity list.
*
* OUTPUTS:
*   *defect_i          - TRUE: defect interrupt is raised.
*                        FALSE: defect interrupt is cleared.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_fc800_defect_i_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_fc800_defect_t defect_id,
                                     BOOL *defect_i)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 cbr_chnl = 0;
    fc800_pmg_int_chnl_t *fc800_pmg = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(defect_i);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get CBR channel ID */
    rc |= adpt_cbr_chnl_get(dev_id, chnl_id, &cbr_chnl);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_PORT_DIR_RX == dir)
    {
        fc800_pmg = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.fc800_pmg_rx);
    }
    else if (DIGI_PORT_DIR_TX == dir)
    {
        fc800_pmg = &(adpt_handle->pmon_data->cbr_defect.cbrc_chnl[cbr_chnl].cbrc.fc800_pmg_tx);
    }
    else
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    switch (defect_id)
    {
        case DIGI_FC_DISP_INV:
            *defect_i = fc800_pmg->rx_disp_invert_i;
            fc800_pmg->rx_disp_invert_i = FALSE;
            break;

        case DIGI_FC_XDET:
            *defect_i = fc800_pmg->rx_xdet_i;
            fc800_pmg->rx_xdet_i = FALSE; 
            break;

        case DIGI_FC_DLOLB:
            *defect_i = fc800_pmg->rx_dlolb_i;
            fc800_pmg->rx_dlolb_i = FALSE;
            break;

        case DIGI_FC_ASD:
            *defect_i = fc800_pmg->rx_asd_i;
            fc800_pmg->rx_asd_i = FALSE;
            break;

        case DIGI_FC_SYNC:
            *defect_i = fc800_pmg->rx_sync_i;
            fc800_pmg->rx_sync_i= FALSE;
            break;

        case DIGI_FC_LCV:
            *defect_i = fc800_pmg->rx_lcv_i;
            fc800_pmg->rx_lcv_i = FALSE;
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_fc800_defect_i_get */




/*******************************************************************************
* adpt_device_int_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a device interrupt table with interrupt bits.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *int_table         - pointer to device level interrupt and defect status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_device_int_get(UINT32 dev_id, digi_device_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* retrieve interrupt table */
    rc = digi_device_int_retrieve(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    /* clear processed interrupt table */
    rc = digi_device_int_clear(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_device_int_get */


/*******************************************************************************
* adpt_otn_odu_int_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Starts OTN ODU interrupt processing.
*
*   This function triggers the ODU_RFRM interrupts to be transfered to
*   shadow registers for retrieval by SW. Interrupts are automatically cleared
*   so interrupts for all ODU levels and all channels must be captured between
*   adpt_otn_odu_int_start() and adpt_otn_odu_int_end().
*
*   Example call sequence:
*     // Trigger beginning of ODU interrupt processing 
*     adpt_otn_odu_int_start()
*
*     // Channelized OTN ODU callback called for all active chnls
*     // Retrieve a filtered list of active ints for given chnl
*     adpt_otn_odu_int_chnl_filt_retrieve()
*
*     // End ODU interrupt processing
*     adpt_otn_odu_int_end()
*
* INPUTS:
*   dev_id             - device identity.
*   latch_defect       - latch defect mode. Rev A must use COREOTN_LATCH_DEFECT_ALL 
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
PUBLIC PMC_ERROR adpt_otn_odu_int_start(UINT32 dev_id, coreotn_latch_defect_t latch_defect)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_int_start(adpt_handle->digi_handle, latch_defect);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_odu_int_start */


/*******************************************************************************
* adpt_otn_odu_int_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Ends OTN ODU interrupt processing.
*
* INPUTS:
*   dev_id             - device identity.
*   latch_defect       - latch defect mode. Rev A must use COREOTN_LATCH_DEFECT_ALL 
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
PUBLIC PMC_ERROR adpt_otn_odu_int_end(UINT32 dev_id, coreotn_latch_defect_t latch_defect)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_int_end(adpt_handle->digi_handle, latch_defect);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_odu_int_end */


/*******************************************************************************
* adpt_otn_ddeg_int_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Starts OTN dDEG interrupt processing.
*
*  This function triggers all OTN (server and ODU) dDEG interrupts to be
*  transferred to the local cache for retrieval by SW. Interrupts are
*  automatically cleared so interrupts for all channels must be captured
*  between adpt_otn_ddeg_int_start() and adpt_otn_ddeg_int_end().
*
*   Example call sequence:
*     // Trigger beginning of dDEG interrupt processing 
*     adpt_otn_ddeg_int_start()
*
*     // Trigger beginning of ODU interrupt processing 
*     adpt_otn_odu_int_start()
*
*     // End ODU interrupt processing
*     adpt_otn_odu_int_end()
*
*     // End dDEG interrupt processing 
*     adpt_otn_ddeg_int_end()
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
PUBLIC PMC_ERROR adpt_otn_ddeg_int_start(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_ddeg_int_start(adpt_handle->digi_handle, 0);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_ddeg_int_start */


/*******************************************************************************
* adpt_otn_ddeg_int_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Ends OTN dDEG interrupt processing.
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
PUBLIC PMC_ERROR adpt_otn_ddeg_int_end(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_ddeg_int_end(adpt_handle->digi_handle, 0);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_ddeg_int_end */


/*******************************************************************************
* adpt_otn_server_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel interrupt table with interrupt bits for a specified
*   line or sysotn channel.
*
*   adpt_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within 
*                        this table to indicate that the interrupt/status will 
*                        be retrieved to the output table. DIGI_INT_ALL_INTS   
*                        to retrieve all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_server_int_chnl_filt_retrieve(UINT32 dev_id,
                                               UINT32 chnl_id,
                                               lineotn_int_chnl_t *filt_table,
                                               lineotn_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_server_int_chnl_filt_retrieve(digi_handle, 
                                                otu_handle,
                                                filt_table,
                                                int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_server_int_chnl_filt_retrieve */


/*******************************************************************************
* adpt_otn_server_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the channel interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to channel interrupt table with bits set to clear
*                        interrupts. DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR adpt_otn_server_int_chnl_clear(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          lineotn_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_server_chnl_t *otu_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_otu_handle_get(dev_id, chnl_id, &digi_handle, &otu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_server_int_chnl_clear(digi_handle, otu_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_server_int_chnl_clear */


/*******************************************************************************
* adpt_otn_odu_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel interrupt table with interrupt bits for a specified
*   ODU channel.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*   adpt_otn_odu_int_start(COREOTN_LATCH_DEFECT_ODUKP) or 
*   adpt_otn_odu_int_start(COREOTN_LATCH_DEFECT_ALL) needs to be called before
*   retrieving interrupts to trigger RFRM status transfer.
*
*   adpt_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/N) or system side (stage 3B).
*   *framer_filt_table - pointer to filter table for ODU framer status. Mark 
*                        fields as TRUE within this table to indicate that
*                        the interrupt/status will be retrieved to the output 
*                        table. DIGI_INT_ALL_INTS to retrieve all interrupt/status. 
*                        Applies to stage3B as well.
*   *pid_filt_table    - pointer to filter table for ODU PID status.
*                        Mark fields as TRUE within this table to indicate that
*                        the interrupt/status will be retrieved to the output table. 
*                        DIGI_INT_ALL_INTS to retrieve all interrupt/status. 
*                        Applies to stage3b as well.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_odu_int_chnl_filt_retrieve(UINT32 dev_id,
                                             UINT32 chnl_id,
                                             digi_otn_loc_t location,
                                             coreotn_int_framer_chnl_t *framer_filt_table,
                                             odu_rfrm_int_pid_chnl_t *pid_filt_table,
                                             digi_otn_odu_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_int_chnl_filt_retrieve(digi_handle, 
                                             odu_handle,
                                             framer_filt_table,
                                             pid_filt_table,
                                             int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_odu_int_chnl_filt_retrieve */


/*******************************************************************************
* adpt_otn_odu_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the channel interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   location           - line side(stage 1/2/3A/N) or system side (stage 3B).
*   *int_table_ptr     - pointer to channel interrupt table with bits set to
*                        clear interrupts. DIGI_INT_ALL_INTS to clear all 
*                        interrupts.
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
PUBLIC PMC_ERROR adpt_otn_odu_int_chnl_clear(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_loc_t location,
                                        digi_otn_odu_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_otn_odu_chnl_t *odu_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_odu_handle_get(dev_id, chnl_id, location, &digi_handle, &odu_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_int_chnl_clear(digi_handle, odu_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_odu_int_chnl_clear */


/*******************************************************************************
* adpt_otn_int_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   Filter structure limits which blocks to retrieve to decrease retrieval time.
*
*   Note that OTN ODU interrupts (e.g. PM and TCM PID interrupts) will always
*   clear on retrieval due to a hardware limitation. This is different than the
*   rest of the system's interrupts which will always have an explict
*   int_clear() function. 
*
*   adpt_otn_odu_int_start() must be used before calling this function to
*   retrieve the latest RFRM status.
*
*   adpt_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_filter        - pointer to filter structure indicating which blocks
*                        to retrieve.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_otn_int_filt_retrieve(UINT32 dev_id,
                                     digi_otn_int_filter_t *int_filter,
                                     digi_otn_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_filter);
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_int_filt_retrieve(adpt_handle->digi_handle, int_filter, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_int_filt_retrieve */


/*******************************************************************************
* adpt_otn_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated interrupt 
*   table.
*
*   Depending on the desired behaviour this function should be proceeded by
*   a call to adpt_otn_odu_int_start and followed by a call to 
*   adpt_otn_odu_int_end if the desire is to ensure that ODU RFRM interrupts
*   are cleared before calling the next operation. Each time interrupts are
*   latched, adpt_otn_odu_int_start, the ODU RFRM are effectively cleared.
*   If the call to this function is proceeded by a adpt_otn_int_filt_retrieve
*   was used adpt_otn_odu_int_start/end then the ODU RFRM interrupts were 
*   cleared in that operation.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to aggregated interrupt table with bits set 
*                        to clear interrupts. DIGI_INT_ALL_INTS to clear all 
*                        interrupts.
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
PUBLIC PMC_ERROR adpt_otn_int_clear(UINT32 dev_id, digi_otn_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_int_clear(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_otn_int_clear */


/*******************************************************************************
* adpt_mapper_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an channel interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within 
*                        this table to indicate that the interrupt/status will  
*                        be retrieved to the output table. DIGI_INT_ALL_INTS to   
*                        retrieve all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_int_chnl_filt_retrieve(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            digi_mapper_int_chnl_t *filt_table,
                                            digi_mapper_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_int_chnl_filt_retrieve(digi_handle, 
                                            map_handle,
                                            filt_table,
                                            int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_mapper_int_chnl_filt_retrieve */


/*******************************************************************************
* adpt_mapper_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated interrupt 
*   table sor a given channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to channel interrupt table with bits set to clear
*                        interrupts. DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR adpt_mapper_int_chnl_clear(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_mapper_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_mapper_chnl_t *map_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_map_handle_get(dev_id, chnl_id, &digi_handle, &map_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_int_chnl_clear(digi_handle, map_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_mapper_int_chnl_clear */


/*******************************************************************************
* adpt_mapper_int_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits over multiple
*   channels.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*   adpt_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4 ) must be used before
*   calling this function to retrieve the latest FO2 status.
*
*   adpt_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within 
*                        this table to indicate that the interrupt/status will  
*                        be retrieved to the output table. DIGI_INT_ALL_INTS to  
*                        retrieve all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mapper_int_filt_retrieve(UINT32 dev_id,
                                        digi_mapper_int_chnl_t *filt_table,   
                                        digi_mapper_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_int_filt_retrieve(adpt_handle->digi_handle, filt_table, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_mapper_int_filt_retrieve */


/*******************************************************************************
* adpt_mapper_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated interrupt 
*   table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to aggregated interrupt table with bits set 
*                        to clear interrupts. DIGI_INT_ALL_INTS to clear all 
*                        interrupts.
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
PUBLIC PMC_ERROR adpt_mapper_int_clear(UINT32 dev_id, digi_mapper_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mapper_int_clear(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_mapper_int_clear */


/*******************************************************************************
* adpt_enet_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an channel interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be 
*                        retrieved to the output table. DIGI_INT_ALL_INTS to retrieve  
*                        all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_int_chnl_filt_retrieve(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_int_chnl_t *filt_table,   
                                          digi_enet_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_int_chnl_filt_retrieve(digi_handle,
                                          chnl_handle,   
                                          filt_table,
                                          int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_int_chnl_filt_retrieve */


/*******************************************************************************
* adpt_enet_line_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears link interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to channel interrupt table with bits set to clear
*                        interrupts. DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR adpt_enet_line_int_chnl_clear(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_enet_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_line_int_chnl_clear(digi_handle, chnl_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_line_int_chnl_clear */


/*******************************************************************************
* adpt_enet_sys_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears link interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to channel interrupt table with bits set to clear
*                        interrupts. DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR adpt_enet_sys_int_chnl_clear(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_enet_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_sys_int_chnl_clear(digi_handle, chnl_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_sys_int_chnl_clear */


/*******************************************************************************
* adpt_enet_line_int_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits over multiple
*   channels.
*
*   The adpt_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be 
*                        retrieved to the output table. DIGI_INT_ALL_INTS to retrieve  
*                        all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_line_int_filt_retrieve(UINT32 dev_id,
                                         digi_enet_int_chnl_t *filt_table,   
                                         digi_enet_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_line_int_filt_retrieve(adpt_handle->digi_handle, filt_table, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_line_int_filt_retrieve */


/*******************************************************************************
* adpt_enet_line_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated interrupt
*   table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to aggregated interrupt table with bits set 
*                        to clear interrupts. DIGI_INT_ALL_INTS to clear all 
*                        interrupts.
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
PUBLIC PMC_ERROR adpt_enet_line_int_clear(UINT32 dev_id, digi_enet_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_line_int_clear(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_line_int_clear */


/*******************************************************************************
* adpt_enet_sys_int_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits over multiple
*   channels.
*
*   The adpt_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be 
*                        retrieved to the output table. DIGI_INT_ALL_INTS to retrieve  
*                        all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_sys_int_filt_retrieve(UINT32 dev_id,
                                         digi_enet_int_chnl_t *filt_table,   
                                         digi_enet_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_sys_int_filt_retrieve(adpt_handle->digi_handle, filt_table, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_sys_int_filt_retrieve */


/*******************************************************************************
* adpt_enet_sys_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated interrupt
*   table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to aggregated interrupt table with bits set 
*                        to clear interrupts. DIGI_INT_ALL_INTS to clear all 
*                        interrupts.
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
PUBLIC PMC_ERROR adpt_enet_sys_int_clear(UINT32 dev_id, digi_enet_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_sys_int_clear(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_sys_int_clear */


/*******************************************************************************
* adpt_enet_int_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated interrupt table, retrieve a pointer
*   to the corresponding channel interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - aggregated interrupt table
*
* OUTPUTS:
*   *int_chnl_table    - pointer to channel interrupt table corresponding to channel
*                        handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_int_chnl_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_enet_int_t *int_table,
                                    digi_enet_int_chnl_t **int_chnl_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);
    ADPT_PARA_VERIFY(int_chnl_table);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_int_chnl_get(digi_handle, 
                                chnl_handle,
                                int_table,
                                int_chnl_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_int_chnl_get */


/*******************************************************************************
* adpt_cbr_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an channel interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be 
*                        retrieved to the output table. DIGI_INT_ALL_INTS to retrieve  
*                        all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_int_chnl_filt_retrieve(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_cbr_int_chnl_t *filt_table,   
                                          digi_cbr_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_int_chnl_filt_retrieve(digi_handle,
                                         cbr_handle,   
                                         filt_table,
                                         int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_cbr_int_chnl_filt_retrieve */


/*******************************************************************************
* adpt_cbr_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears link interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to channel interrupt table with bits set to clear
*                        interrupts. DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR adpt_cbr_int_chnl_clear(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_cbr_int_chnl_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_int_chnl_clear(digi_handle, cbr_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_cbr_int_chnl_clear */


/*******************************************************************************
* adpt_cbr_int_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   adpt_cbr_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *filt_table        - pointer to filter table. Mark fields as TRUE within this
*                        table to indicate that the interrupt/status will be 
*                        retrieved to the output table. DIGI_INT_ALL_INTS to retrieve  
*                        all interrupt/status.
*
* OUTPUTS:
*   *int_table         - pointer to channel interrupt table to store interrupt
*                        information. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_int_filt_retrieve(UINT32 dev_id,
                                     digi_cbr_int_chnl_t *filt_table,   
                                     digi_cbr_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_int_filt_retrieve(adpt_handle->digi_handle, filt_table, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_cbr_int_filt_retrieve */


/*******************************************************************************
* adpt_cbr_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated interrupt
*   table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - pointer to aggregated interrupt table with bits set 
*                        to clear interrupts. DIGI_INT_ALL_INTS to clear all 
*                        interrupts.
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
PUBLIC PMC_ERROR adpt_cbr_int_clear(UINT32 dev_id, digi_cbr_int_t *int_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_int_clear(adpt_handle->digi_handle, int_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_cbr_int_clear */


/*******************************************************************************
* adpt_cbr_int_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated interrupt table, retrieve a pointer
*   to the corresponding channel interrupt table.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *int_table         - aggregated interrupt table
*
* OUTPUTS:
*   *int_chnl_table    - pointer to channel interrupt table corresponding to channel
*                        handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cbr_int_chnl_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_cbr_int_t *int_table,
                                   digi_cbr_int_chnl_t **int_chnl_table)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    digi_cbr_client_chnl_t *cbr_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(int_table);
    ADPT_PARA_VERIFY(int_chnl_table);

    /* handle process */
    rc = adpt_cbr_handle_get(dev_id, chnl_id, &digi_handle, &cbr_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cbr_int_chnl_get(digi_handle, 
                               cbr_handle, 
                               int_table,
                               int_chnl_table);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_cbr_int_chnl_get */


/*
** End of file
*/
