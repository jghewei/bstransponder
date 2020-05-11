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
*     The file describes the datapath related configuration for all client and
*     OTN traffic.
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
/*******************************************************************************
* adpt_chnl_id_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates the chnl_id and ho_chnl_id saved in the Adapter context. 
*   There are two cases to update the chnl_id and ho_chnl_id.
*
*   Case #1: The input chnl_id doesn't exist in the context. The old chnl_id and 
*   ho_chnl_id will be updated by this input chnl_id.
*
*   Case #2: The input chnl_id has existed in the context. The old chnl_id will
*   switch this input chnl_id with each other, as well as the old ho_chnl_id. 
*   1. Switch the input chnl_id to DIGI_SWITCH_CHNL_ID temporarily.
*   2. Update the old chnl_id to the input chnl_id.
*   3. Switch DIGI_SWITCH_CHNL_ID to the old chnl_id. 
*
* INPUTS:
*   dev_id             - device identity.
*   index              - context index for traffic_info.
*   chnl_id            - the chnl_id to be updated.
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
PRIVATE PMC_ERROR adpt_chnl_id_update(UINT32 dev_id, UINT32 index, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 old_chnl_id = 0;
    UINT32 i = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* chnl_id update is only available during restart restoration */
    /* return directly for the same chnl_id */
    if ((DIGI_INIT_DONE == adpt_handle->init_state)
         || (adpt_handle->traffic_info[index].chnl_id == chnl_id))
    {
        PMC_RETURN(rc);
    }

    /* switch to DIGI_SWITCH_CHNL_ID temporarily */
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (!(adpt_handle->traffic_info[i].enable) || (i == index))
        {
            continue;
        }

        if (adpt_handle->traffic_info[i].chnl_id == chnl_id)
        {
            adpt_handle->traffic_info[i].chnl_id = DIGI_SWITCH_CHNL_ID;
        }

        if (adpt_handle->traffic_info[i].ho_chnl_id == chnl_id)
        {
            adpt_handle->traffic_info[i].ho_chnl_id = DIGI_SWITCH_CHNL_ID;
        }
    }

    /* update the old chnl_id / ho_chnl_id with the input chnl_id */
    old_chnl_id = adpt_handle->traffic_info[index].chnl_id;
    adpt_handle->traffic_info[index].chnl_id = chnl_id;
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (!(adpt_handle->traffic_info[i].enable) || (i == index))
        {
            continue;
        }

        if (adpt_handle->traffic_info[i].ho_chnl_id == old_chnl_id)
        {
            adpt_handle->traffic_info[i].ho_chnl_id = chnl_id;
        }
    }

    /* switch to the old chnl_id */
    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
    {
        if (!(adpt_handle->traffic_info[i].enable) || (i == index))
        {
            continue;
        }

        if (adpt_handle->traffic_info[i].chnl_id == DIGI_SWITCH_CHNL_ID)
        {
            adpt_handle->traffic_info[i].chnl_id = old_chnl_id;
        }

        if (adpt_handle->traffic_info[i].ho_chnl_id == DIGI_SWITCH_CHNL_ID)
        {
            adpt_handle->traffic_info[i].ho_chnl_id = old_chnl_id;
        }
    }

    PMC_RETURN(rc);
} /* adpt_chnl_id_update */


/*******************************************************************************
* adpt_chnl_id_chk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks the chnl_id if it exists in the Adapter context. If it
*   is, modify this chnl_id as a temporary one. If this chnl_id has already been
*   configured with a flag enable, this function returns failure.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
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
PRIVATE PMC_ERROR adpt_chnl_id_chk(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR rc_tmp = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 index1 = 0;
    UINT32 new_chnl_id = 0;
    UINT32 serdes_port = 0;
    UINT32 i = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc_tmp = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    if (PMC_SUCCESS == rc_tmp)
    {
        /* lo_chnl_id is invalid */
        if (DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[index].enable))
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        /* substitute this channel with another one */
        for (i = DIGI_TEMP_CHNL_ID; i > 0; i--)
        {
            serdes_port = DIGI_SERDES_PORT_GET(adpt_handle->traffic_info[index].chnl_id);
            new_chnl_id = DIGI_CHNL_ID_SET(serdes_port, i);

            rc_tmp = adpt_chnl_index_get(adpt_handle, new_chnl_id, &index1);
            if (PMC_SUCCESS != rc_tmp)
            {
                rc = adpt_chnl_id_update(dev_id, index, new_chnl_id);
                ADPT_RETURN_VERIFY(rc);

                break;
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_chnl_id_chk */


/*******************************************************************************
* adpt_ilkn_grp_chk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks if these two traffic modes belong to the same ILKN group.
*
* INPUTS:
*   traffic_mode       - the first ILKN traffic mode.
*   traffic_mode1      - the second ILKN traffic mode.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE means the same group ILKN while FALSE means the different group ILKN.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL adpt_ilkn_grp_chk(digi_traffic_mode_t traffic_mode, digi_traffic_mode_t traffic_mode1)
{
    digi_ilkn_mode_t ilkn_mode  = (digi_ilkn_mode_t)DIGI_CLIENT_MODE_GET(traffic_mode);
    digi_ilkn_mode_t ilkn_mode1 = (digi_ilkn_mode_t)DIGI_CLIENT_MODE_GET(traffic_mode1);
    BOOL result = FALSE;

    switch (ilkn_mode)
    {
        case DIGI_ILKN_SW_COPI1:
        case DIGI_ILKN_CPB_COPI1:
            if ((DIGI_ILKN_SW_COPI1 == ilkn_mode1) || (DIGI_ILKN_CPB_COPI1 == ilkn_mode1))
            {
                result = TRUE;
            }
            break;

        case DIGI_ILKN_SW_COPI2:
        case DIGI_ILKN_CPB_COPI2:
            if ((DIGI_ILKN_SW_COPI2 == ilkn_mode1) || (DIGI_ILKN_CPB_COPI2 == ilkn_mode1))
            {
                result = TRUE;
            }
            break;

        default:
            break;
    }

    return result;
} /* adpt_ilkn_grp_chk */


/*******************************************************************************
* adpt_dp_traffic_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures traffic mode in line/system side. It may include
*   OTU traffic, Ethernet traffic, CBR traffic and interlaken part. It just
*   defines port mode together with serdes information.
*
*   The following block may be involved.
*   OTU:  Serdes, LINEOTN, Ho ODU of COREOTN;
*   CBR:  Serdes, CBRC;
*   ETH:  Serdes, ENET(LINE), ENET(SYS);
*   INLK: Interlaken;
*
* INPUTS:
*   dev_id             - device identity.
*
*                        31 30 ... 25 24  23 22 ... 17 16  15 14 13 ... 2 1 0
*                        _______________  _______________  __________________
*                         slot_id[31:24]  digi_dev[23:16]    card_id[15:0]       
*
*   chnl_id            - channel identity.
*
*                        31 30 29 ... 25 24  23 22 21 20 ... 5 4 3 2 1 0
*                        __________________  ___________________________
*                        serdes_port[31:24]      tributary_port[23:0]
*
*                        serdes_port starts from 0 on either the line side or the 
*                        system side.
*                        Usually, tributary_port is used for the ODUk resource 
*                        and ILKN. For others, it can be zero.
*                        In addition, the chnl_id shouldn't be DIGI_DEFAULT_CHNL_ID
*                        or DIGI_SWITCH_CHNL_ID. 
*   traffic_mode       - traffic mode.
*   fec_type           - FEC type. Only available under OTU type.
*   ilkn_info          - ILKN channel information. Only available under ILKN type.
*
*                        31 30 ... 25 24  23 22 ... 9 8  7 6 5 ... 2 1 0
*                        ________________ ______________ _______________
*                        enet_pmon[31:24] chnl_num[23:8]  chnl_rate[7:0]
*
*                        enet_pmon is used for ENET <> CPB <> ILKN datapath.
*                        chnl_num is used for the MPMA/MPMO of interlaken.
*                        valid range: 0 ~ 191.
*                        chnl_rate is specified as an integer fraction of 96. 
*                        Used to allocate scheduling for signal transferred 
*                        towards backplane. The following must be true.
*                        chnl_rate/96 >= (signal_rate Gpbs)/(120 Gbps)
*   enable             - TRUE: enable traffic mode configuration.
*                        FALSE: disable traffic mode configuration.
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
*   1. Once the traffic mode is disabled, the S16 SerDes loopback will be also 
*   disabled accordingly if it is enabled.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dp_traffic_mode_cfg(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_traffic_mode_t traffic_mode,
                                      digi_otn_server_fec_t fec_type,
                                      UINT32 ilkn_info,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 traffic_type = 0;
    UINT32 client_mode = 0;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sfis_deskew_type_t deskew_type = DIGI_OTN_SERVER_NO_DESKEW;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, traffic_mode, fec_type, ilkn_info, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(traffic_mode);
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(traffic_mode);
    /* get traffic type */
    traffic_type = DIGI_TRAFFIC_TYPE_GET(traffic_mode);
    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
        || (traffic_loc >= DIGI_SIDE_MAX)
        || (traffic_type >= DIGI_TYPE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* index handling */
    if (enable)
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
        if (PMC_SUCCESS != rc)
        {
            /* create a traffic container */
            rc = adpt_chnl_index_create(adpt_handle, chnl_id, &index);
        }
    }
    else
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    }
    ADPT_RETURN_VERIFY(rc);

    /* workaround: S16 SerDes loopback should be disabled before datapath de-provision */
    if (!enable && (DIGI_TYPE_INLK != traffic_type))
    {
        rc = adpt_loopback_set(dev_id, chnl_id, DIGI_LPBK_DISABLE);
        ADPT_RETURN_VERIFY(rc);
    }

    switch (traffic_type)
    {
        case DIGI_TYPE_OTU:
            if (enable)
            {
                /* get deskew type */
                rc = adpt_otu_deskew_get(dev_id, chnl_id, traffic_mode, &deskew_type);
                ADPT_RETURN_VERIFY(rc);

                /* OTUk layer configuration */
                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    rc |= digi_otn_server_line_prov(adpt_handle->digi_handle,
                                                    adpt_handle->line_serdes[serdes_port],
                                                    (digi_otn_server_otuk_t)client_mode,
                                                    fec_type,
                                                    deskew_type,
                                                    &(adpt_handle->traffic_info[index].otu_handle));
                }
                else
                {
                    if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD != adpt_handle->dev_info->card_type)
                    {
                        rc |= digi_otn_server_sys_prov(adpt_handle->digi_handle,
                                                       adpt_handle->sys_serdes[serdes_port],
                                                       fec_type,
                                                       deskew_type,
                                                       &(adpt_handle->traffic_info[index].otu_handle));
                    }
                    else
                    {
                        rc |= digi_otn_server_line_prov(adpt_handle->digi_handle,
                                                        adpt_handle->sys_serdes[serdes_port],
                                                        (digi_otn_server_otuk_t)client_mode,
                                                        fec_type,
                                                        deskew_type,
                                                        &(adpt_handle->traffic_info[index].otu_handle));
                    }
                }
                ADPT_RETURN_VERIFY(rc);
                /* HO ODU channel provision */
                rc |= digi_otn_server_ho_prov(adpt_handle->digi_handle,
                                              adpt_handle->traffic_info[index].otu_handle,
                                              DIGI_OTN_DIRECT_DEVICE_CONFIG,
                                              DIGI_ODU_UNCHANNELIZED,
                                              &(adpt_handle->traffic_info[index].odu_handle));
            }
            else
            {
                /* handle verification */
                ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].odu_handle);

                /* HO ODU deprovision*/
                rc = digi_otn_server_ho_deprov(adpt_handle->digi_handle, 
                                               &(adpt_handle->traffic_info[index].odu_handle),
                                               &(adpt_handle->traffic_info[index].otu_handle));
                ADPT_RETURN_VERIFY(rc);

                /* OTUk layer deprovision */
                if ((DIGI_LINE_SIDE == traffic_loc) || (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type))
                {
                    /* workaround: MLD loopback should be disabled before datapath de-provision */
                    rc = adpt_internal_loopback_set(dev_id, chnl_id, DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK, FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc |= digi_otn_server_line_deprov(adpt_handle->digi_handle, 
                                                      &(adpt_handle->traffic_info[index].otu_handle));
                }
                else
                {
                    /* workaround: MLD loopback should be disabled before datapath de-provision */
                    rc = adpt_internal_loopback_set(dev_id, chnl_id, DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK, FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc |= digi_otn_server_sys_deprov(adpt_handle->digi_handle, 
                                                     &(adpt_handle->traffic_info[index].otu_handle));
                }
                ADPT_RETURN_VERIFY(rc);
            }
            break;

        case DIGI_TYPE_ETH:
            /* workaround on DIGI-60 */
            if ((DIGI60_PROD_APP_LINE_CARD == adpt_handle->dev_info->card_type)
                || (DIGI60_PROD_APP_TRANSMUXPONDER_CARD == adpt_handle->dev_info->card_type))
            {
                switch (client_mode)
                {
                    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
                        client_mode = DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON;
                        break;

                    case DIGI_ENET_SERDES_10GE_TRANSPARENT:
                        client_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
                        break;

                    case DIGI_ENET_SERDES_40GE_TRANSPARENT:
                        client_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
                        break;

                    default:
                        break;
                }
            }

            if (enable)
            {
                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    rc |= digi_enet_line_prov(adpt_handle->digi_handle,
                                              adpt_handle->line_serdes[serdes_port],
                                              (digi_enet_mode_t)client_mode,
                                              &(adpt_handle->traffic_info[index].enet_handle));
                }
                else
                {
                    if (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD != adpt_handle->dev_info->card_type)
                    {
                        rc |= digi_enet_sys_prov(adpt_handle->digi_handle,
                                                 adpt_handle->sys_serdes[serdes_port],
                                                 (digi_enet_mode_t)client_mode,
                                                 &(adpt_handle->traffic_info[index].enet_handle));
                    }
                    else
                    {
                        rc |= digi_enet_line_prov(adpt_handle->digi_handle,
                                                  adpt_handle->sys_serdes[serdes_port],
                                                  (digi_enet_mode_t)client_mode,
                                                  &(adpt_handle->traffic_info[index].enet_handle));
                    }
                }
            }
            else
            {
                /* handle verification */
                ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

                if ((DIGI_LINE_SIDE == traffic_loc) || (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == adpt_handle->dev_info->card_type))
                {
                    rc |= digi_enet_line_deprov(adpt_handle->digi_handle,
                                                &(adpt_handle->traffic_info[index].enet_handle));
                }
                else
                {
                    rc |= digi_enet_sys_deprov(adpt_handle->digi_handle,
                                               &(adpt_handle->traffic_info[index].enet_handle));
                }
            }
            break;

        case DIGI_TYPE_CBR:
            if (enable)
            {
                if (DIGI_LINE_SIDE == traffic_loc)
                {
                    rc |= digi_cbr_client_prov(adpt_handle->digi_handle,
                                               adpt_handle->line_serdes[serdes_port],
                                               (digi_cbr_client_mode_t)client_mode,
                                               &(adpt_handle->traffic_info[index].cbr_handle));
                }
                else
                {
                    rc |= digi_cbr_client_prov(adpt_handle->digi_handle,
                                               adpt_handle->sys_serdes[serdes_port],
                                               (digi_cbr_client_mode_t)client_mode,
                                               &(adpt_handle->traffic_info[index].cbr_handle));
                }
            }
            else
            {
                /* handle verification */
                ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].cbr_handle);

                rc |= digi_cbr_client_deprov(adpt_handle->digi_handle,
                                             &(adpt_handle->traffic_info[index].cbr_handle));
            }
            break;

        case DIGI_TYPE_INLK:
            switch (client_mode)
            {
                case DIGI_ILKN_SW_COPI1:
                    if (enable)
                    {
                        rc = digi_ilkn_chnl_cfg(adpt_handle->digi_handle,
                                                adpt_handle->ilkn_1_ctxt,
                                                DIGI_ILKN_CHNL_TARGET_ODUK,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_oduk_chnl_prov(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[index].ilkn_handle,
                                                      DIGI_ILKN_RATE_GET(ilkn_info),
                                                      &(adpt_handle->dev_info->sifd_1_tx_fc),
                                                      &(adpt_handle->dev_info->sifd_1_rx_fc));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    else
                    {
                        /* handle verification */
                        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].ilkn_handle);

                        rc = digi_ilkn_oduk_chnl_deprov(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].ilkn_handle,
                                                        &(adpt_handle->dev_info->sifd_1_tx_fc),
                                                        &(adpt_handle->dev_info->sifd_1_rx_fc));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_chnl_uncfg(adpt_handle->digi_handle,
                                                  &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    break;

                case DIGI_ILKN_CPB_COPI1:
                    if (enable)
                    {
                        rc = digi_ilkn_chnl_cfg(adpt_handle->digi_handle,
                                                adpt_handle->ilkn_1_ctxt,
                                                DIGI_ILKN_CHNL_TARGET_CPB,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_cpb_chnl_prov(adpt_handle->digi_handle,
                                                     adpt_handle->traffic_info[index].ilkn_handle,
                                                     DIGI_ILKN_RATE_GET(ilkn_info),
                                                     DIGI_ILKN_PMON_GET(ilkn_info),
                                                     FALSE,
                                                     FALSE,
                                                     FALSE,
                                                     &(adpt_handle->dev_info->sifd_1_tx_fc),
                                                     &(adpt_handle->dev_info->sifd_1_rx_fc));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    else
                    {
                        /* handle verification */
                        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].ilkn_handle);

                        rc = digi_ilkn_cpb_chnl_deprov(adpt_handle->digi_handle,
                                                       adpt_handle->traffic_info[index].ilkn_handle,
                                                       &(adpt_handle->dev_info->sifd_1_tx_fc),
                                                       &(adpt_handle->dev_info->sifd_1_rx_fc));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_chnl_uncfg(adpt_handle->digi_handle,
                                                  &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    break;

                case DIGI_ILKN_SW_COPI2:
                    if (enable)
                    {
                        rc = digi_ilkn_chnl_cfg(adpt_handle->digi_handle,
                                                adpt_handle->ilkn_2_ctxt,
                                                DIGI_ILKN_CHNL_TARGET_ODUK,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_oduk_chnl_prov(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[index].ilkn_handle,
                                                      DIGI_ILKN_RATE_GET(ilkn_info),
                                                      &(adpt_handle->dev_info->sifd_2_tx_fc),
                                                      &(adpt_handle->dev_info->sifd_2_rx_fc));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    else
                    {
                        /* handle verification */
                        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].ilkn_handle);

                        rc = digi_ilkn_oduk_chnl_deprov(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].ilkn_handle,
                                                        &(adpt_handle->dev_info->sifd_2_tx_fc),
                                                        &(adpt_handle->dev_info->sifd_2_rx_fc));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_chnl_uncfg(adpt_handle->digi_handle,
                                                  &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    break;

                case DIGI_ILKN_CPB_COPI2:
                    if (enable)
                    {
                        rc = digi_ilkn_chnl_cfg(adpt_handle->digi_handle,
                                                adpt_handle->ilkn_2_ctxt,
                                                DIGI_ILKN_CHNL_TARGET_CPB,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                DIGI_ILKN_CHNL_GET(ilkn_info),
                                                0,
                                                0,
                                                &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_cpb_chnl_prov(adpt_handle->digi_handle,
                                                     adpt_handle->traffic_info[index].ilkn_handle,
                                                     DIGI_ILKN_RATE_GET(ilkn_info),
                                                     DIGI_ILKN_PMON_GET(ilkn_info),
                                                     FALSE,
                                                     FALSE,
                                                     FALSE,
                                                     &(adpt_handle->dev_info->sifd_2_tx_fc),
                                                     &(adpt_handle->dev_info->sifd_2_rx_fc));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    else
                    {
                        /* handle verification */
                        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].ilkn_handle);

                        rc = digi_ilkn_cpb_chnl_deprov(adpt_handle->digi_handle,
                                                       adpt_handle->traffic_info[index].ilkn_handle,
                                                       &(adpt_handle->dev_info->sifd_2_tx_fc),
                                                       &(adpt_handle->dev_info->sifd_2_rx_fc));
                        ADPT_RETURN_VERIFY(rc);

                        rc = digi_ilkn_chnl_uncfg(adpt_handle->digi_handle,
                                                  &(adpt_handle->traffic_info[index].ilkn_handle));
                        ADPT_RETURN_VERIFY(rc);
                    }
                    break;



                default:
                    ADPT_ERROR("Input wrong client mode[%d].\r\n", client_mode);
                    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                    break;
            }
            break;

        default:
            ADPT_ERROR("Input wrong traffic type[%d].\r\n", traffic_type);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->traffic_info[index].enable       = DIGI_EN_SET(enable, DIGI_DP_RX_TX);
        adpt_handle->traffic_info[index].fec_type     = fec_type;
        adpt_handle->traffic_info[index].traffic_mode = traffic_mode;
        adpt_handle->traffic_info[index].oduk_type    = (DIGI_TYPE_OTU == traffic_type) ? adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode) : (digi_oduk_t)0;
        adpt_handle->traffic_info[index].oduflex_rate = 0;
        adpt_handle->traffic_info[index].payload_type = DIGI_ODU_UNCHANNELIZED;
        adpt_handle->traffic_info[index].odu_stage    = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
        adpt_handle->traffic_info[index].ilkn_info    = ilkn_info;
        if (!enable)
        {
            /* destroy this ODUk container */
            rc = adpt_chnl_index_destroy(adpt_handle, chnl_id, index);
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_traffic_mode_cfg */


/*******************************************************************************
* adpt_dp_odu_mux_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure ODUk multiplexing/demultiplexing. The
*   maximum datapath for ODUk is two stages altogeter. before calling this
*   function, adpt_traffic_mode_cfg interface should be called beforehand.
*
* INPUTS:
*   dev_id             - device identity.
*   ho_chnl_id         - parent channel identity.
*   lo_chnl_id         - self channel identity.
*   odu_mux            - ODUk mux/demux related information.
*                        The oduflex_rate with bps unit is only available for 
*                        ODUflex(CBR). It should be more than 1000bps.
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
PUBLIC PMC_ERROR adpt_dp_odu_mux_cfg(UINT32 dev_id,
                                    UINT32 ho_chnl_id,
                                    UINT32 lo_chnl_id,
                                    digi_odu_mux_t odu_mux)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lo_index = 0;
    UINT32 ho_index = 0;
    UINT32 index = 0;
    BOOL lo_exit = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 tx_ts_cnt = 0;
    UINT32 rx_ts_cnt = 0;
    UINT32 i = 0;
    BOOL tp_remove = FALSE;
    digi_otn_msi_cfg_mode_t msi_mode = DIGI_OTN_MSI_CFG_AUTO;
    digi_otn_odu_chnl_prov_mode_t prov_mode = DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][%d][%lf][%d].\r\n", 
               dev_id, ho_chnl_id, lo_chnl_id, odu_mux.oduk_type,
               odu_mux.mapping_type, odu_mux.payload_type, odu_mux.otn_dir,
               odu_mux.tx_ts_mask[0], odu_mux.tx_ts_mask[1], odu_mux.tx_ts_mask[2],
               odu_mux.rx_ts_mask[0], odu_mux.rx_ts_mask[1], odu_mux.rx_ts_mask[2],
               odu_mux.tp_enable, odu_mux.oduflex_rate, odu_mux.self_ts_type);

    if (((DIGI_ODU_TS_1G25 != odu_mux.payload_type) && (DIGI_ODU_TS_2G5 != odu_mux.payload_type))
         || ((odu_mux.oduflex_rate > 0.001) && (odu_mux.oduflex_rate < 1000.0))
         || (odu_mux.otn_dir >= LAST_DIGI_OTN_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* check Tx TS count with Rx TS count */
    for (i = 0; i < 96; i++)
    {
        if ((odu_mux.tx_ts_mask[i/32] >> (i % 32)) & 1)
        {
            tx_ts_cnt++;
        }

        if ((odu_mux.rx_ts_mask[i/32] >> (i % 32)) & 1)
        {
            rx_ts_cnt++;
        }
    }

    if (((tx_ts_cnt != rx_ts_cnt) && (0 != tx_ts_cnt) && (0 != rx_ts_cnt))
         || ((0 == tx_ts_cnt) && (0 == rx_ts_cnt) && odu_mux.tp_enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_prov_mode_get(dev_id, &prov_mode);
    ADPT_RETURN_VERIFY(rc);

    /* set the self TS type as DIGI_ODU_UNCHANNELIZED for a non-bulk provision operation */
    if (DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT == prov_mode)
    {
        odu_mux.self_ts_type = DIGI_ODU_UNCHANNELIZED;
    }

    if (odu_mux.tp_enable)
    {
        /* get the index in traffic container */
        rc |= adpt_chnl_index_get(adpt_handle, lo_chnl_id, &lo_index);
        if (PMC_SUCCESS != rc)
        {
            /* create a traffic container */
            rc = adpt_chnl_index_create(adpt_handle, lo_chnl_id, &lo_index);
        }
        rc |= adpt_chnl_index_get(adpt_handle, ho_chnl_id, &ho_index);
        ADPT_RETURN_VERIFY(rc);

        if (odu_mux.otn_dir & adpt_handle->traffic_info[lo_index].otn_dir)
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        /* copy existed TS mask */
        switch (odu_mux.otn_dir)
        {
            case DIGI_OTN_DIR_RX:
                for (i = 0; i < 3; i++)
                {
                    odu_mux.tx_ts_mask[i] = adpt_handle->traffic_info[lo_index].tx_ts_mask[i];
                }
                break;

            case DIGI_OTN_DIR_TX:
                for (i = 0; i < 3; i++)
                {
                    odu_mux.rx_ts_mask[i] = adpt_handle->traffic_info[lo_index].rx_ts_mask[i];
                }
                break;

            default:
                break;
        }

        /* payload type process */
        if (adpt_handle->traffic_info[ho_index].enable
            && (DIGI_ODU_UNCHANNELIZED == adpt_handle->traffic_info[ho_index].payload_type))
        {
            /* workaround in the bulk provision mode */
            /* switch to the non-bulk provision mode before the digi_otn_payload_type_transition() */
            if (DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode)
            {
                rc = adpt_prov_mode_set(dev_id, DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT);
                ADPT_RETURN_VERIFY(rc);
            }

            /* HO ODUk payload type process */
            if (NULL != adpt_handle->traffic_info[ho_index].otu_handle)
            {
                /* payload type transition */
                rc = digi_otn_payload_type_transition(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[ho_index].odu_handle,
                                                      odu_mux.payload_type);
                ADPT_RETURN_VERIFY(rc);
            }
            /* MO ODUk payload type process */
            else
            {
                /* workaround on MSI configuration mode */
                rc |= digi_otn_msi_mode_get(adpt_handle->digi_handle, &msi_mode);
                rc |= digi_otn_msi_mode_set(adpt_handle->digi_handle, DIGI_OTN_MSI_CFG_MANUAL);
                ADPT_RETURN_VERIFY(rc);

                /* payload type transition */
                rc = digi_otn_payload_type_transition(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[ho_index].odu_handle,
                                                      odu_mux.payload_type);
                ADPT_RETURN_VERIFY(rc);

                /* workaround on MSI configuration mode */
                rc |= digi_otn_msi_mode_set(adpt_handle->digi_handle, msi_mode);
                ADPT_RETURN_VERIFY(rc);
            }

            /* reset TCM pool ID */
            rc = adpt_tcm_pool_reset(dev_id, ho_chnl_id);
            ADPT_RETURN_VERIFY(rc);

            /* payload type */
            adpt_handle->traffic_info[ho_index].payload_type = odu_mux.payload_type;

            /* workaround in the bulk provision mode */
            /* switch to the bulk provision mode after the digi_otn_payload_type_transition() */
            if (DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode)
            {
                rc = adpt_prov_mode_set(dev_id, prov_mode);
                ADPT_RETURN_VERIFY(rc);
            }
        }

        /* tributary port assignment */
        rc = digi_otn_server_lo_prov_asym(adpt_handle->digi_handle,
                                          adpt_handle->traffic_info[ho_index].odu_handle,
                                          DIGI_OTN_DIRECT_DEVICE_CONFIG,
                                          odu_mux.oduk_type,
                                          odu_mux.oduflex_rate,
                                          odu_mux.mapping_type,
                                          &(odu_mux.tx_ts_mask[0]),
                                          &(odu_mux.rx_ts_mask[0]),
                                          odu_mux.self_ts_type,
                                          &(adpt_handle->traffic_info[lo_index].odu_handle));
    }
    else
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, lo_chnl_id, &lo_index);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_chnl_index_get(adpt_handle, ho_chnl_id, &ho_index);
        ADPT_RETURN_VERIFY(rc);

        if (!(odu_mux.otn_dir & adpt_handle->traffic_info[lo_index].otn_dir))
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        /* handle verification */
        ADPT_PARA_VERIFY(adpt_handle->traffic_info[lo_index].odu_handle);

        /* tributary port deprovision */
        rc = digi_otn_server_lo_deprov_asym(adpt_handle->digi_handle,
                                            &(adpt_handle->traffic_info[lo_index].odu_handle),
                                            &(adpt_handle->traffic_info[ho_index].odu_handle),
                                            adpt_otn_dir_cvt(odu_mux.otn_dir));
        ADPT_RETURN_VERIFY(rc);

        /* HO ODUk payload type process */
        if ((DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT == prov_mode)
            && adpt_handle->traffic_info[ho_index].enable
            && (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[ho_index].payload_type)
            && !(~(odu_mux.otn_dir) & adpt_handle->traffic_info[lo_index].otn_dir))
        {
            for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
            {
                if ((index != lo_index)
                    && adpt_handle->traffic_info[index].enable
                    && (adpt_handle->traffic_info[index].ho_chnl_id == adpt_handle->traffic_info[ho_index].chnl_id))
                {
                    lo_exit = TRUE;
                    break;
                }    
            }

            if (!lo_exit)
            {
                /* payload type transition */
                rc = digi_otn_payload_type_transition(adpt_handle->digi_handle,
                                                      adpt_handle->traffic_info[ho_index].odu_handle,
                                                      DIGI_ODU_UNCHANNELIZED);
                ADPT_RETURN_VERIFY(rc);

                /* reset TCM pool ID */
                rc = adpt_tcm_pool_reset(dev_id, ho_chnl_id);
                ADPT_RETURN_VERIFY(rc);

                /* payload type */                              
                adpt_handle->traffic_info[ho_index].payload_type = DIGI_ODU_UNCHANNELIZED;
            }
        }
    }

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        if (odu_mux.tp_enable)
        {
            adpt_handle->traffic_info[lo_index].ho_chnl_id    = ho_chnl_id;
            adpt_handle->traffic_info[lo_index].mapping_type  = odu_mux.mapping_type;
            adpt_handle->traffic_info[lo_index].oduk_type     = odu_mux.oduk_type;
            adpt_handle->traffic_info[lo_index].payload_type  = odu_mux.self_ts_type;
            adpt_handle->traffic_info[lo_index].otn_dir       = (digi_otn_dir_t)(adpt_handle->traffic_info[lo_index].otn_dir | odu_mux.otn_dir);
            switch (odu_mux.otn_dir)
            {
                case DIGI_OTN_DIR_RX:
                    for (i = 0; i < 3; i++)
                    {
                        adpt_handle->traffic_info[lo_index].rx_ts_mask[i] = odu_mux.rx_ts_mask[i];
                    }
                    break;

                case DIGI_OTN_DIR_TX:
                    for (i = 0; i < 3; i++)
                    {
                        adpt_handle->traffic_info[lo_index].tx_ts_mask[i] = odu_mux.tx_ts_mask[i];
                    }
                    break;

                case DIGI_OTN_DIR_RX_TX:
                    for (i = 0; i < 3; i++)
                    {
                        adpt_handle->traffic_info[lo_index].rx_ts_mask[i] = odu_mux.rx_ts_mask[i];
                        adpt_handle->traffic_info[lo_index].tx_ts_mask[i] = odu_mux.tx_ts_mask[i];
                    }
                    break;

                default:
                    break;
            }
            adpt_handle->traffic_info[lo_index].oduflex_rate  = odu_mux.oduflex_rate;
            adpt_handle->traffic_info[lo_index].odu_stage     = (digi_odu_struct_odu_level_t)(adpt_handle->traffic_info[ho_index].odu_stage + 1);
            adpt_handle->traffic_info[lo_index].enable        = DIGI_EN_SET(odu_mux.tp_enable, adpt_handle->traffic_info[lo_index].otn_dir);
            adpt_handle->traffic_info[lo_index].traffic_mode  = adpt_handle->traffic_info[ho_index].traffic_mode;
        }
        else
        {
            switch (odu_mux.otn_dir)
            {
                case DIGI_OTN_DIR_RX:
                    for (i = 0; i < 3; i++)
                    {
                        adpt_handle->traffic_info[lo_index].rx_ts_mask[i] = 0;
                    }
                    break;

                case DIGI_OTN_DIR_TX:
                    for (i = 0; i < 3; i++)
                    {
                        adpt_handle->traffic_info[lo_index].tx_ts_mask[i] = 0;
                    }
                    break;

                case DIGI_OTN_DIR_RX_TX:
                    for (i = 0; i < 3; i++)
                    {
                        adpt_handle->traffic_info[lo_index].rx_ts_mask[i] = 0;
                        adpt_handle->traffic_info[lo_index].tx_ts_mask[i] = 0;
                    }
                    break;

                default:
                    break;
            }

            if (odu_mux.otn_dir == adpt_handle->traffic_info[lo_index].otn_dir)
            {
                tp_remove = TRUE;
            }
            adpt_handle->traffic_info[lo_index].otn_dir = (digi_otn_dir_t)(adpt_handle->traffic_info[lo_index].otn_dir & ~(odu_mux.otn_dir));
            adpt_handle->traffic_info[lo_index].enable  = DIGI_EN_SET(TRUE, DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[lo_index].enable) & adpt_handle->traffic_info[lo_index].otn_dir);

            /* destroy this ODUk container */
            if (tp_remove)
            {
                rc = adpt_chnl_index_destroy(adpt_handle, lo_chnl_id, lo_index);
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_odu_mux_cfg */


/*******************************************************************************
* adpt_dp_client_map_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures client end point and then maps the client to its
*   end point.
*   In Transponder and Muxponder applications,
*   this function configures ENET or CBRC client and then maps it to CPB and
*   ODUk switch.
*
*   In Line Card applications,
*   this function configures back plane (e.g. Interlaken) channel, and then
*   maps it to CPB and ODUk switch.
*
*   In Tributary Card applications,
*   this function configures ENET or CBRC client and then maps it to CPB.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mapping_mode       - client mapping mode.
*   ts_number          - tributary slot number. Only available for ODUflex.
*   enable             - TRUE: enable the client mapping configuration.
*                        FALSE: disable the client mapping confguration.
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
PUBLIC PMC_ERROR adpt_dp_client_map_cfg(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_mapotn_mode_t mapping_mode,
                                     UINT32 ts_number,
                                     BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    BOOL b_pmon = FALSE;
    digi_oduk_t odu_type = DIGI_ODUK_ODU0;
    UINT8 num_lanes = 0;
    UINT8 primary_grp = DIGI_SERDES_PIN_NOT_USED;
    UINT8 secondary_grp = DIGI_SERDES_PIN_NOT_USED;
    /* Serdes ports available */
    digi_port_usage_grp_struct_t port_grp_usage;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT8 upi_value = 0;
    UINT8 upi_value1 = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, mapping_mode, ts_number, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (enable)
    {
        /* get mapper parameter */
        rc |= adpt_mapper_para_get(adpt_handle, adpt_handle->traffic_info[index].traffic_mode, 
                                   mapping_mode, &b_pmon, &odu_type, &num_lanes);
        /* find out where we could potentialy cause issues with ENET LINE */
        rc |= digi_mapper_group_usage_get(adpt_handle->digi_handle, &port_grp_usage);
        ADPT_RETURN_VERIFY(rc);

        /* find out where we should allocate this to Primary/Secondary */
        rc = adpt_mapper_lane_create(dev_id,
                                     chnl_id,
                                     mapping_mode,
                                     ts_number,
                                     &port_grp_usage,
                                     &primary_grp,
                                     &secondary_grp);
        ADPT_RETURN_VERIFY(rc);

        /* prov mapotn bidirectional data path */
        rc |= digi_otn_mapper_prov(adpt_handle->digi_handle,
                                   (digi_mapper_primary_group_t)primary_grp,
                                   (digi_mapper_secondary_group_t)secondary_grp,
                                   adpt_mapping_mode_cvt(mapping_mode),
                                   b_pmon,
                                   odu_type,
                                   ts_number,
                                   &(adpt_handle->traffic_info[index].map_handle));
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        /* workaround: ETRANS loopback should be disabled before de-mapping */
        switch (adpt_handle->traffic_info[index].mapping_mode)
        {
            case DIGI_MAPOTN_ODU2P_10_GE_6_2_GFP:
            case DIGI_MAPOTN_ODU2EP_PKT_10G3_7_1_BMP:
            case DIGI_MAPOTN_ODU1EP_PKT_10G3_7_2_BMP:
            case DIGI_MAPOTN_ODU4P_100_GE_GFP:
                rc = adpt_internal_loopback_set(dev_id, chnl_id, DIGI_ENET_MAPOTN_GMP_LOOPBACK, FALSE);
                ADPT_RETURN_VERIFY(rc);
                break;

            case DIGI_MAPOTN_ODU3P_40_GE_GFP:
                rc = adpt_internal_loopback_set(dev_id, chnl_id, DIGI_ENET_MAPOTN_G709_LOOPBACK, FALSE);
                ADPT_RETURN_VERIFY(rc);
                break;

            default:
                break;
        }

        /* handle verification */
        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].map_handle);

        /* mapotn deprovision */
        rc = digi_otn_mapper_deprov(adpt_handle->digi_handle, 
                                    &(adpt_handle->traffic_info[index].map_handle));
        ADPT_RETURN_VERIFY(rc);

        /* destroy Primary/Secondary */
        rc = adpt_mapper_lane_destroy(dev_id, chnl_id);
    }

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->traffic_info[index].mapping_mode = mapping_mode;
        adpt_handle->traffic_info[index].ts_number    = ts_number;
        adpt_handle->traffic_info[index].map_enable   = enable ? DIGI_EN_SET(enable, DIGI_DP_RX_TX) : enable;
    }

    /* workaround on expected UPI and filter out CMFs to CPU FIFO */
    if (enable)
    {
        if (DIGI_MAP_GFP == adpt_gfp_mapping_get(mapping_mode))
        {
            rc = adpt_gfp_tx_upi_get(dev_id, chnl_id, &upi_value);
            ADPT_RETURN_VERIFY(rc);

            rc = adpt_gfp_exp_upi_set(dev_id, chnl_id, upi_value);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (DIGI_MAP_7_3_GFP == adpt_gfp_mapping_get(mapping_mode))
        {
            rc = adpt_gfp_legacy_mode_get(dev_id, chnl_id, &upi_value, &upi_value1);
            ADPT_RETURN_VERIFY(rc);

            rc = adpt_gfp_exp_upi_ext_set(dev_id, chnl_id, upi_value, upi_value1);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_client_map_cfg */


/*******************************************************************************
* adpt_dp_cpb_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Acitvates or deactivates CPB switch which goes through MAPOTN.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - direction to MAPOTN.
*   enable             - TRUE: acticate CPB switch.
*                        FALSE: deactivate CPB switch.
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
PRIVATE PMC_ERROR adpt_dp_cpb_chnl_enable(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_cpb_dir_t dir,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_type = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_cpb_stream_t cpb_stream = DIGI_CPB_STREAM_10G;
    UINT32 halt_buffer = 0;
    util_global_switch_data_t *client_data = NULL;
    UINT32 ts_number = 0;
    digi_cpb_src_t source = LAST_DIGI_CPB_SRC;
    UINT8 map_sw_info = 0;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_cpb_chnl_en_get(dev_id, chnl_id, dir, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if (enable && org_enable)
    {
        ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, dir, enable);
        PMC_RETURN(rc);
    }

    /* get traffic type */
    traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[index].traffic_mode);

    switch (traffic_type)
    {
        case DIGI_TYPE_ETH:
            client_data = adpt_handle->traffic_info[index].enet_handle;
            ts_number = adpt_handle->traffic_info[index].ts_number;
            source = (DIGI_CPB_CBR_TO_MAP == dir) ? DIGI_CPB_SRC_ENET : DIGI_CPB_SRC_MAP;
            break;

        case DIGI_TYPE_CBR:
            client_data = adpt_handle->traffic_info[index].cbr_handle;
            ts_number = adpt_handle->traffic_info[index].ts_number;
            source = (DIGI_CPB_CBR_TO_MAP == dir) ? DIGI_CPB_SRC_CBR : DIGI_CPB_SRC_MAP;
            break;

        case DIGI_TYPE_INLK:
            client_data = adpt_handle->traffic_info[index].ilkn_handle;
            ts_number = DIGI_ILKN_RATE_GET(adpt_handle->traffic_info[index].ilkn_info);
            source = (DIGI_CPB_CBR_TO_MAP == dir) ? DIGI_CPB_SRC_ILKN : DIGI_CPB_SRC_MAP;
            break;

        default:
            ADPT_ERROR("Wrong traffic type[%d].\r\n", traffic_type);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    /* get cpb parameter */
    rc = adpt_cpb_map_para_get(adpt_handle->traffic_info[index].mapping_mode,
                               &cpb_stream, 
                               &halt_buffer);
    ADPT_RETURN_VERIFY(rc);

    /* get cpb buffer */
    rc = adpt_cpb_fifo_get(cpb_stream, source, ts_number, &halt_buffer);
    ADPT_RETURN_VERIFY(rc);

    if (enable)
    {
        switch (dir)
        {
            case DIGI_CPB_MAP_TO_CBR:
                /* cpb channel connection */
                rc |= digi_cpb_chnl_map(adpt_handle->digi_handle,
                                        adpt_handle->traffic_info[index].map_handle,
                                        client_data,
                                        cpb_stream,
                                        ts_number,
                                        halt_buffer);
                ADPT_RETURN_VERIFY(rc);

                /* cpb channel activation */                        
                rc |= digi_cpb_chnl_activate(adpt_handle->digi_handle, 
                                             client_data);
                ADPT_RETURN_VERIFY(rc);
                break;

            case DIGI_CPB_CBR_TO_MAP:
                /* cpb channel connection */
                rc |= digi_cpb_chnl_map(adpt_handle->digi_handle,
                                        client_data,
                                        adpt_handle->traffic_info[index].map_handle,
                                        cpb_stream,
                                        ts_number,
                                        halt_buffer);
                ADPT_RETURN_VERIFY(rc);

                /* cpb channel activation */                        
                rc |= digi_cpb_chnl_activate(adpt_handle->digi_handle, 
                                             adpt_handle->traffic_info[index].map_handle);
                ADPT_RETURN_VERIFY(rc);
                break;

            case DIGI_CPB_MAP_TO_MAP:
                /* cpb channel connection */
                rc |= digi_cpb_chnl_map(adpt_handle->digi_handle,
                                        adpt_handle->traffic_info[index].map_handle,
                                        adpt_handle->traffic_info[index].map_handle,
                                        cpb_stream,
                                        ts_number,
                                        halt_buffer);
                ADPT_RETURN_VERIFY(rc);

                /* cpb channel activation */                        
                rc |= digi_cpb_chnl_activate(adpt_handle->digi_handle, 
                                             adpt_handle->traffic_info[index].map_handle);
                ADPT_RETURN_VERIFY(rc);
                break;

            default:
                ADPT_ERROR("Wrong direction type[%d].\r\n", dir);
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
    }
    else
    {
        switch (dir)
        {
            case DIGI_CPB_MAP_TO_CBR:
                rc |= digi_cpb_chnl_deactivate(adpt_handle->digi_handle, 
                                               client_data);
                ADPT_RETURN_VERIFY(rc);

                rc |= digi_cpb_chnl_demap(adpt_handle->digi_handle, 
                                          client_data);
                ADPT_RETURN_VERIFY(rc);
                break;

            case DIGI_CPB_CBR_TO_MAP:
            case DIGI_CPB_MAP_TO_MAP:
                rc |= digi_cpb_chnl_deactivate(adpt_handle->digi_handle, 
                                               adpt_handle->traffic_info[index].map_handle);
                ADPT_RETURN_VERIFY(rc);

                rc |= digi_cpb_chnl_demap(adpt_handle->digi_handle, 
                                          adpt_handle->traffic_info[index].map_handle);
                ADPT_RETURN_VERIFY(rc);
                break;

            default:
                ADPT_ERROR("Wrong direction type[%d].\r\n", dir);
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
    }

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        map_sw_info = adpt_handle->traffic_info[index].map_sw_info;
        switch (dir)
        {
            case DIGI_CPB_MAP_TO_CBR:
                adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_CBR_SET(map_sw_info, enable, enable, dir);
                break;

            case DIGI_CPB_CBR_TO_MAP:
            case DIGI_CPB_MAP_TO_MAP:
                adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_MAP_SET(map_sw_info, enable, enable, dir);
                break;

            default:
                break;
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_cpb_chnl_enable */


/*******************************************************************************
* adpt_dp_packet_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure unidirectional switch function in 
*   DIGI device. There are two switch modes altogether: ODUk switch and CPB 
*   switch. 
*
* INPUTS:
*   dev_id             - device identity.
*   switch_mode        - switch mode: ODUk switch, CPB switch.
*   src_chnl_id        - source channel identity.
*   dest_chnl_id       - destination channel identity.
*   enable             - TRUE: enable cross connection.
*                        FALSE: disable cross connection.
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
PUBLIC PMC_ERROR adpt_dp_packet_switch(UINT32 dev_id,
                                    digi_switch_mode_t switch_mode,
                                    UINT32 src_chnl_id,
                                    UINT32 dest_chnl_id,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 src_index = 0;
    UINT32 dest_index = 0;
    UINT32 src_traffic_type = 0;
    UINT32 dest_traffic_type = 0;
    UINT32 halt_buffer = 0;
    digi_oduksw_stream_t oduksw_stream = DIGI_ODUKSW_STREAM_CBR_ODU0;
    digi_cpb_stream_t cpb_stream = DIGI_CPB_STREAM_10G;
    UINT32 ts_number = 0;
    util_global_switch_data_t *src_switch_data = NULL;
    util_global_switch_data_t *dest_switch_data = NULL;
    digi_cfg_container_t *adpt_handle = NULL;
    DOUBLE oduflex_rate = 0;
    UINT32 latency_s = 0;
    digi_cpb_src_t source = LAST_DIGI_CPB_SRC;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               switch_mode, src_chnl_id, dest_chnl_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, src_chnl_id, &src_index);
    rc |= adpt_chnl_index_get(adpt_handle, dest_chnl_id, &dest_index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    src_traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[src_index].traffic_mode);
    dest_traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->traffic_info[dest_index].traffic_mode);
    if ((src_traffic_type >= DIGI_TYPE_MAX) || (dest_traffic_type >= DIGI_TYPE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if ((DIGI_SWITCH_ODUK == switch_mode) || (DIGI_SWITCH_ODUK_CPB == switch_mode))
    {
        /* ODUk switch information */
        switch (src_traffic_type)
        {
            case DIGI_TYPE_OTU:
                /* get oduksw parameter from oduk type */
                oduflex_rate = adpt_handle->traffic_info[src_index].oduflex_rate;
                rc = adpt_oduksw_odu_para_get(adpt_handle->traffic_info[src_index].oduk_type,
                                              adpt_handle->traffic_info[src_index].oduflex_rate,
                                              &(adpt_handle->traffic_info[src_index].rx_ts_mask[0]),
                                              &oduksw_stream,
                                              &ts_number);
                /* switch data */
                src_switch_data  = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].odu_handle);
                break;

            case DIGI_TYPE_ETH:
            case DIGI_TYPE_CBR:
                /* get oduksw parameter from mapotn */
                rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[src_index].mapping_mode,
                                              &oduksw_stream,
                                              &oduflex_rate);
                ts_number = adpt_handle->traffic_info[src_index].ts_number;
                /* switch data */
                src_switch_data  = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].map_handle);
                break;

            case DIGI_TYPE_INLK:
                switch (dest_traffic_type)
                {
                    case DIGI_TYPE_OTU:
                        /* get oduksw parameter from oduk type */
                        oduflex_rate = adpt_handle->traffic_info[dest_index].oduflex_rate;
                        rc = adpt_oduksw_odu_para_get(adpt_handle->traffic_info[dest_index].oduk_type,
                                                      adpt_handle->traffic_info[dest_index].oduflex_rate,
                                                      &(adpt_handle->traffic_info[dest_index].tx_ts_mask[0]),
                                                      &oduksw_stream,
                                                      &ts_number);
                        break;

                    case DIGI_TYPE_ETH:
                    case DIGI_TYPE_CBR:
                        /* get oduksw parameter from mapotn */
                        rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[dest_index].mapping_mode,
                                                      &oduksw_stream,
                                                      &oduflex_rate);
                        ts_number = adpt_handle->traffic_info[dest_index].ts_number;
                        break;

                    case DIGI_TYPE_INLK:
                        if (NULL != adpt_handle->traffic_info[dest_index].map_handle)
                        {
                            /* get oduksw parameter from dest MAPOTN */
                            rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[dest_index].mapping_mode,
                                                          &oduksw_stream,
                                                          &oduflex_rate);
                            ts_number = adpt_handle->traffic_info[dest_index].ts_number;
                        }
                        else if (NULL != adpt_handle->traffic_info[src_index].map_handle)
                        {
                            /* get oduksw parameter from source MAPOTN */
                            rc = adpt_oduksw_map_para_get(adpt_handle->traffic_info[src_index].mapping_mode,
                                                          &oduksw_stream,
                                                          &oduflex_rate);
                            ts_number = adpt_handle->traffic_info[src_index].ts_number;
                        }
                        else
                        {
                            /* get oduksw parameter from interlaken */
                            rc = adpt_oduksw_odu_para_get(adpt_handle->traffic_info[dest_index].oduk_type,
                                                          adpt_handle->traffic_info[dest_index].oduflex_rate,
                                                          &(adpt_handle->traffic_info[dest_index].tx_ts_mask[0]),
                                                          &oduksw_stream,
                                                          &ts_number);
                            oduflex_rate = adpt_handle->traffic_info[dest_index].oduflex_rate;
                            ts_number    = DIGI_ILKN_RATE_GET(adpt_handle->traffic_info[dest_index].ilkn_info);
                        }
                        break;

                    default:
                        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                        break;
                }
                /* switch data */
                if (NULL != adpt_handle->traffic_info[src_index].map_handle)
                {
                    src_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].map_handle);
                }
                else
                {
                    src_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].ilkn_handle);
                }
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }

        switch (dest_traffic_type)
        {
            case DIGI_TYPE_OTU:
                dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].odu_handle);
                break;

            case DIGI_TYPE_ETH:
            case DIGI_TYPE_CBR:
                dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].map_handle);
                break;

            case DIGI_TYPE_INLK:
                if (NULL != adpt_handle->traffic_info[dest_index].map_handle)
                {
                    dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].map_handle);
                }
                else
                {
                    dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].ilkn_handle);
                }
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
        ADPT_RETURN_VERIFY(rc);

        /* parameter verification */
        ADPT_PARA_VERIFY(src_switch_data);
        ADPT_PARA_VERIFY(dest_switch_data);

        if (enable)
        {
            UINT32 opsa_pkt_size = 0;
            UINT32 opsa_header_size = 0;
            UINT32 pkt_size;
            UINT32 pkt_per_int;
            UINT32 pkt_per_n;
            UINT32 pkt_per_d;
            DOUBLE eps;

            /* get customer parameter */
            rc = adpt_opsa_pkt_size_get(dev_id, 
                                        oduksw_stream,
                                        &opsa_pkt_size,
                                        &opsa_header_size);
            ADPT_RETURN_VERIFY(rc);

            /* Calculate B,T values */
            rc = digi_oif_pkt_period_calc(adpt_handle->digi_handle,
                                          oduksw_stream,
                                          ts_number,
                                          oduflex_rate,
                                          opsa_pkt_size,
                                          opsa_header_size,
                                          &pkt_size,
                                          &pkt_per_int,
                                          &pkt_per_n,
                                          &pkt_per_d,
                                          &eps);
            ADPT_RETURN_VERIFY(rc);



            /* OPSA FIFO and latency */
            rc |= adpt_opsa_oduksw_fifo_get(dev_id, 
                                            src_chnl_id,
                                            oduksw_stream,
                                            ts_number,
                                            &halt_buffer,
                                            &latency_s);
            ADPT_RETURN_VERIFY(rc);

            /* oduksw connection from source to destination */
            rc |= digi_oduksw_chnl_map(adpt_handle->digi_handle,
                                       src_switch_data,
                                       dest_switch_data,
                                       oduksw_stream,
                                       ts_number,
                                       oduflex_rate,
                                       pkt_size,
                                       pkt_per_int,
                                       pkt_per_n,
                                       pkt_per_d,
                                       halt_buffer,
                                       latency_s);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SWITCH_ODUK_CPB == switch_mode)
            {
                /* CPB activation should be excuted after oduksw activation */
                if (dest_switch_data == adpt_handle->traffic_info[dest_index].map_handle)
                {
                    rc |= adpt_dp_cpb_chnl_enable(dev_id, dest_chnl_id, DIGI_CPB_MAP_TO_CBR, TRUE);
                }
                ADPT_RETURN_VERIFY(rc);

                if (src_switch_data == adpt_handle->traffic_info[src_index].map_handle)
                {
                    rc |= adpt_dp_cpb_chnl_enable(dev_id, src_chnl_id, DIGI_CPB_CBR_TO_MAP, TRUE);
                }
                ADPT_RETURN_VERIFY(rc);
            }

            /* oduksw activation */
            rc |= digi_oduksw_chnl_activate(adpt_handle->digi_handle, dest_switch_data);
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            /* oduksw deactivation */
            rc |= digi_oduksw_chnl_deactivate(adpt_handle->digi_handle, dest_switch_data);
            ADPT_RETURN_VERIFY(rc);

            if (DIGI_SWITCH_ODUK_CPB == switch_mode)
            {
                /* CPB deactivation/demap */
                if (dest_switch_data == adpt_handle->traffic_info[dest_index].map_handle)
                {
                    rc |= adpt_dp_cpb_chnl_enable(dev_id, dest_chnl_id, DIGI_CPB_MAP_TO_CBR, FALSE);
                    ADPT_RETURN_VERIFY(rc);
                }

                if (src_switch_data == adpt_handle->traffic_info[src_index].map_handle)
                {
                    rc |= adpt_dp_cpb_chnl_enable(dev_id, src_chnl_id, DIGI_CPB_CBR_TO_MAP, FALSE);
                    ADPT_RETURN_VERIFY(rc);
                }
            }

            /* oduksw deconnection*/
            rc |= digi_oduksw_chnl_demap(adpt_handle->digi_handle, dest_switch_data);
            ADPT_RETURN_VERIFY(rc);
        }

        /* record information */
        if (PMC_SUCCESS == rc)
        {
            adpt_handle->traffic_info[dest_index].oduk_src_index = DIGI_SWT_SET(enable, enable, src_index);
        }
    }
    else if (DIGI_SWITCH_CPB == switch_mode)
    {
        /* CPB switch information */
        switch (src_traffic_type)
        {
            case DIGI_TYPE_ETH: 
                /* get CPB parameter from Ethernet */
                rc = adpt_cpb_eth_para_get(adpt_handle->traffic_info[src_index].traffic_mode,
                                           &cpb_stream,
                                           &halt_buffer);
                ts_number = adpt_handle->traffic_info[src_index].ts_number;
                /* switch data */
                src_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].enet_handle);
                source = DIGI_CPB_SRC_ENET;
                break;
                
            case DIGI_TYPE_CBR:
                /* get CPB parameter from CBR */
                rc = adpt_cpb_cbr_para_get(adpt_handle->traffic_info[src_index].traffic_mode,
                                           &cpb_stream,
                                           &halt_buffer);
                ts_number = adpt_handle->traffic_info[src_index].ts_number;
                /* switch data */
                src_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].cbr_handle);
                source = DIGI_CPB_SRC_CBR;
                break;

            case DIGI_TYPE_INLK:
                switch (dest_traffic_type)
                {
                    case DIGI_TYPE_ETH:
                        /* get CPB parameter from Ethernet */
                        rc = adpt_cpb_eth_para_get(adpt_handle->traffic_info[dest_index].traffic_mode,
                                                   &cpb_stream,
                                                   &halt_buffer);
                        ts_number = adpt_handle->traffic_info[dest_index].ts_number;
                        break;

                    case DIGI_TYPE_CBR:
                        /* get CPB parameter from CBR */
                        rc = adpt_cpb_cbr_para_get(adpt_handle->traffic_info[dest_index].traffic_mode,
                                                   &cpb_stream,
                                                   &halt_buffer);
                        ts_number = adpt_handle->traffic_info[dest_index].ts_number;
                        break;

                    case DIGI_TYPE_INLK:
                        /* get CPB parameter from interlaken */
                        rc = adpt_cpb_ilkn_para_get(adpt_handle->traffic_info[dest_index].ilkn_info,
                                                    &cpb_stream,
                                                    &halt_buffer,
                                                    &ts_number);
                        break;

                    default:
                        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                        break;
                }
                /* switch data */
                src_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[src_index].ilkn_handle);
                source = DIGI_CPB_SRC_ILKN;
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
        ADPT_RETURN_VERIFY(rc);

        /* get cpb buffer */
        rc = adpt_cpb_fifo_get(cpb_stream, source, ts_number, &halt_buffer);
        ADPT_RETURN_VERIFY(rc);

        switch (dest_traffic_type)
        {
            case DIGI_TYPE_ETH:
                dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].enet_handle);
                break;

            case DIGI_TYPE_CBR:
                dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].cbr_handle);
                break;

            case DIGI_TYPE_INLK:
                dest_switch_data = (util_global_switch_data_t *)(adpt_handle->traffic_info[dest_index].ilkn_handle);
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }

        /* parameter verification */
        ADPT_PARA_VERIFY(src_switch_data);
        ADPT_PARA_VERIFY(dest_switch_data);
        
        if (enable)
        {
            /* cpb connection from source to destination */
            rc |= digi_cpb_chnl_map(adpt_handle->digi_handle,
                                    src_switch_data,
                                    dest_switch_data,
                                    cpb_stream,
                                    ts_number,
                                    halt_buffer);
            ADPT_RETURN_VERIFY(rc);

            /* cpb activation */
            rc |= digi_cpb_chnl_activate(adpt_handle->digi_handle, dest_switch_data);
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            /* cpb deactivation */
            rc |= digi_cpb_chnl_deactivate(adpt_handle->digi_handle, dest_switch_data);
            ADPT_RETURN_VERIFY(rc);

            /* cpb demap */
            rc |= digi_cpb_chnl_demap(adpt_handle->digi_handle, dest_switch_data);
            ADPT_RETURN_VERIFY(rc);
        }

        /* record information */
        if (PMC_SUCCESS == rc)
        {
            adpt_handle->traffic_info[dest_index].cpb_src_index = DIGI_SWT_SET(enable, enable, src_index);
        }
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_dp_packet_switch */


/*******************************************************************************
* adpt_dp_switch_del
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deletes ODUk SW / CPB connections for a given port or channel.
*
* INPUTS:
*   dev_id             - device identity.
*   index              - context index for traffic_info.
*   switch_mode        - switch mode: ODUk SW/CPB and CPB, ODUk SW/CPB and CPB only.
*                        DIGI_SWITCH_ODUK_CPB: ODUk SW/CPB(oduk_src_index) and 
*                                              CPB(cpb_src_index).
*                        DIGI_SWITCH_ODUK:     ODUk SW/CPB(oduk_src_index).
*                        DIGI_SWITCH_CPB:      CPB(cpb_src_index).
*   dp_dir             - the Rx/Tx datapath or both to be deleted.
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
PRIVATE PMC_ERROR adpt_dp_switch_del(UINT32 dev_id,
                                  UINT32 index, 
                                  digi_switch_mode_t switch_mode,
                                  digi_dp_dir_t dp_dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 src_index = 0;
    UINT32 i = 0;
    UINT32 switch_num = 0;
    digi_switch_mode_t sw_mode = DIGI_SWITCH_ODUK_CPB;
    PMC_ENTRY();

    if ((DIGI_DP_NONE == dp_dir) 
        || (dp_dir >= LAST_DIGI_DP_DIR)
        || (switch_mode >= LAST_DIGI_SWITCH))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* return directly for the channelized ODUk */
    if ((NULL != adpt_handle->traffic_info[index].odu_handle)
        && (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type))
    {
        PMC_RETURN(rc);
    }

    if ((DIGI_DP_RX == dp_dir) || (DIGI_DP_RX_TX == dp_dir))
    {
        /* delete ODUk SW / CPB */
        if ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_ODUK == switch_mode))
        {
            if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].oduk_src_index))
            {
                /* source cross connection */
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].oduk_src_index);

                /* check the ODUk SW number from MAPOTN to COREOTN/ILKN/MAPOTN */
                switch_num = 0;
                if (NULL != adpt_handle->traffic_info[src_index].map_handle)
                {
                    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
                    {
                        if (!(adpt_handle->traffic_info[i].enable))
                        {
                            continue;
                        }

                        if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].oduk_src_index))
                        {
                            if (src_index == DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].oduk_src_index))
                            {
                               switch_num++; 
                            }
                        }
                    }
                }

                if (switch_num > 1)
                {
                    sw_mode = DIGI_SWITCH_ODUK;
                }
                else
                {
                    sw_mode = DIGI_SWITCH_ODUK_CPB;
                }

                rc = adpt_dp_packet_switch(dev_id, sw_mode, 
                                           adpt_handle->traffic_info[src_index].chnl_id,
                                           adpt_handle->traffic_info[index].chnl_id, FALSE);
                ADPT_RETURN_VERIFY(rc);

                /* delete CPB switch from MAPOTN to ENET/CBR/ILKN */
                if ((NULL != adpt_handle->traffic_info[index].map_handle)
                    && (DIGI_SWITCH_ODUK == sw_mode)
                    && DIGI_SWT_CBR_EN_GET(adpt_handle->traffic_info[index].map_sw_info))
                {
                    rc = adpt_dp_cpb_chnl_enable(dev_id, 
                                                 adpt_handle->traffic_info[index].chnl_id,
                                                 DIGI_CPB_MAP_TO_CBR,
                                                 FALSE);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
        }

        /* delete CPB */
        if ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_CPB == switch_mode))
        {
            if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
            {
                /* source cross connection */
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].cpb_src_index);

                rc = adpt_dp_packet_switch(dev_id, DIGI_SWITCH_CPB, 
                                           adpt_handle->traffic_info[src_index].chnl_id,
                                           adpt_handle->traffic_info[index].chnl_id, FALSE);
                ADPT_RETURN_VERIFY(rc);
            }
        }
    }

    if ((DIGI_DP_TX == dp_dir) || (DIGI_DP_RX_TX == dp_dir))
    {
        /* check the ODUk SW number from MAPOTN to COREOTN/ILKN/MAPOTN */
        switch_num = 0;
        if ((NULL != adpt_handle->traffic_info[index].map_handle)
            && ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_ODUK == switch_mode)))
        {
            for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
            {
                if (!(adpt_handle->traffic_info[i].enable))
                {
                    continue;
                }

                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].oduk_src_index);
                if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].oduk_src_index)
                    && (src_index == index))
                {
                   switch_num++; 
                }
            }
        }

        for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
        {
            if (!(adpt_handle->traffic_info[i].enable))
            {
                continue;
            }

            /* delete ODUk SW / CPB */
            if ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_ODUK == switch_mode))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].oduk_src_index);
                if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].oduk_src_index)
                    && (src_index == index))
                {
                    if (NULL != adpt_handle->traffic_info[index].map_handle)
                    {
                        if (switch_num > 1)
                        {
                            sw_mode = DIGI_SWITCH_ODUK;
                        }
                        else
                        {
                            sw_mode = DIGI_SWITCH_ODUK_CPB;
                        }
                        switch_num--;
                    }
                    else
                    {
                        sw_mode = DIGI_SWITCH_ODUK_CPB;
                    }

                    rc = adpt_dp_packet_switch(dev_id, sw_mode, 
                                               adpt_handle->traffic_info[index].chnl_id,
                                               adpt_handle->traffic_info[i].chnl_id,
                                               FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    /* delete CPB switch from MAPOTN to ENET/CBR/ILKN */
                    if ((NULL != adpt_handle->traffic_info[i].map_handle)
                        && (DIGI_SWITCH_ODUK == sw_mode)
                        && DIGI_SWT_CBR_EN_GET(adpt_handle->traffic_info[i].map_sw_info))
                    {
                        rc = adpt_dp_cpb_chnl_enable(dev_id, 
                                                     adpt_handle->traffic_info[i].chnl_id,
                                                     DIGI_CPB_MAP_TO_CBR,
                                                     FALSE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }
            }

            /* delete CPB */
            if ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_CPB == switch_mode))
            {
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].cpb_src_index);
                if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].cpb_src_index)
                    && (src_index == index))
                {
                    rc = adpt_dp_packet_switch(dev_id, DIGI_SWITCH_CPB, 
                                               adpt_handle->traffic_info[index].chnl_id,
                                               adpt_handle->traffic_info[i].chnl_id,
                                               FALSE);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_switch_del */


/*******************************************************************************
* adpt_dp_switch_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function adds ODUk SW / CPB connections for a given port or channel
*   during datapath restoration.
*
* INPUTS:
*   dev_id             - device identity.
*   dp_index           - context index for traffic_info.
*   rst_index          - context index for restart_info.
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
PRIVATE PMC_ERROR adpt_dp_switch_add(UINT32 dev_id, UINT32 dp_index, UINT32 rst_index)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 src_index = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* check parameter validation */
    if (adpt_handle->traffic_info[dp_index].chnl_id != adpt_handle->restart_info[rst_index]->chnl_id)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* return directly for the channelized ODUk */
    if ((NULL != adpt_handle->traffic_info[dp_index].odu_handle)
        && (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[dp_index].payload_type))
    {
        PMC_RETURN(rc);
    }

    /* add ODUk SW / CPB */
    if ((DIGI_SWT_EN_GET(adpt_handle->restart_info[rst_index]->oduk_src_index))
         && !(DIGI_SWT_EN_GET(adpt_handle->traffic_info[dp_index].oduk_src_index)))
    {
        /* source cross connection */
        src_index = DIGI_SWT_ID_GET(adpt_handle->restart_info[rst_index]->oduk_src_index);

        rc = adpt_packet_switch(dev_id, DIGI_SWITCH_ODUK_CPB, 
                                adpt_handle->restart_info[src_index]->chnl_id,
                                adpt_handle->restart_info[rst_index]->chnl_id, TRUE);
        ADPT_RETURN_VERIFY(rc);
    }

    /* add CPB */
    if ((DIGI_SWT_EN_GET(adpt_handle->restart_info[rst_index]->cpb_src_index))
         && !(DIGI_SWT_EN_GET(adpt_handle->traffic_info[dp_index].cpb_src_index)))
    {
        /* source cross connection */
        src_index = DIGI_SWT_ID_GET(adpt_handle->restart_info[rst_index]->cpb_src_index);

        rc = adpt_packet_switch(dev_id, DIGI_SWITCH_CPB, 
                                adpt_handle->restart_info[src_index]->chnl_id,
                                adpt_handle->restart_info[rst_index]->chnl_id, TRUE);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_dp_switch_add */


/*******************************************************************************
* adpt_dp_switch_chk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if there are ODUk SW / CPB connections for a given channel.
*
* INPUTS:
*   dev_id             - device identity.
*   index              - context index for traffic_info.
*   dp_dir             - the Rx/Tx datapath or both.
*
* OUTPUTS:
*   *result            - TRUE: ODUk SW / CPB exists in this channel.
*                        FALSE: ODUk SW / CPB doesn't exist in this channel.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_dp_switch_chk(UINT32 dev_id, 
                                   UINT32 index, 
                                   digi_dp_dir_t dp_dir,
                                   BOOL *result)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 src_index = 0;
    UINT32 dest_index = 0;
    PMC_ENTRY();

    if ((DIGI_DP_NONE == dp_dir) || (dp_dir >= LAST_DIGI_DP_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* check parameter validation */
    ADPT_PARA_VERIFY(result);

    /* default */
    *result = FALSE;

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* return directly for the channelized ODUk */
    if ((NULL != adpt_handle->traffic_info[index].odu_handle)
        && (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type))
    {
        PMC_RETURN(rc);
    }

    if ((DIGI_DP_RX == dp_dir) || (DIGI_DP_RX_TX == dp_dir))
    {
        if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].oduk_src_index))
        {
            *result = TRUE;
            PMC_RETURN(rc);
        }

        if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
        {
            *result = TRUE;
            PMC_RETURN(rc);
        }
    }

    if ((DIGI_DP_TX == dp_dir) || (DIGI_DP_RX_TX == dp_dir))
    {
        for (dest_index = 0; dest_index < DIGI_NUM_CHNL_MAX; dest_index++)
        {
            if (!(adpt_handle->traffic_info[dest_index].enable))
            {
                continue;
            }

            src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[dest_index].oduk_src_index);
            if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].oduk_src_index)
                && (src_index == index))
            {
                *result = TRUE;
                PMC_RETURN(rc);
            }

            src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[dest_index].cpb_src_index);
            if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].cpb_src_index)
                && (src_index == index))
            {
                *result = TRUE;
                PMC_RETURN(rc);
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_switch_chk */


/*******************************************************************************
* adpt_dp_chnl_del
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deletes datapaths for a given port or channel. As to an ODUk 
*   channel, those daught channels will be also removed, as well as ODUk SW / 
*   CPB connections. 
*
* INPUTS:
*   dev_id             - device identity.
*   index              - context index for traffic_info.
*   dp_dir             - the Rx/Tx datapath or both to be deleted. 
*   enable             - TRUE: delete all daughter channels, as well as this channel.
*                        FALSE: delete all daughter channels without this channel. 
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
PRIVATE PMC_ERROR adpt_dp_chnl_del(UINT32 dev_id, 
                                 UINT32 index, 
                                 digi_dp_dir_t dp_dir,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    digi_odu_mux_t odu_mux;
    UINT32 ho_chnl_id = 0;
    UINT32 ho_index = 0; 
    digi_dp_dir_t sw_dir = (DIGI_DP_RX_TX == dp_dir) ? dp_dir : (digi_dp_dir_t)(~dp_dir & DIGI_DP_RX_TX);
    PMC_ENTRY();

    if ((DIGI_DP_NONE == dp_dir) || (dp_dir >= LAST_DIGI_DP_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* delete ODUk SW / CPB cross connections */
    rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_ODUK_CPB, sw_dir);
    ADPT_RETURN_VERIFY(rc);

    /* ENET/CBR/ILKN channel */
    if ((NULL != adpt_handle->traffic_info[index].enet_handle)
        || (NULL != adpt_handle->traffic_info[index].cbr_handle)
        || (NULL != adpt_handle->traffic_info[index].ilkn_handle))
    {
        if (NULL != adpt_handle->traffic_info[index].map_handle)
        {
            rc = adpt_dp_client_map_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id,
                                        adpt_handle->traffic_info[index].mapping_mode,
                                        0, FALSE);
            ADPT_RETURN_VERIFY(rc);
        }

        if (enable)
        {
            rc = adpt_dp_traffic_mode_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id,
                                          adpt_handle->traffic_info[index].traffic_mode,
                                          adpt_handle->traffic_info[index].fec_type,
                                          adpt_handle->traffic_info[index].ilkn_info,
                                          FALSE);
            ADPT_RETURN_VERIFY(rc);
        }
    }
    /* OTU/ODU channel */
    else if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[index].payload_type)
        {
            for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
            {
                if ((i != index) && adpt_handle->traffic_info[i].enable
                    && (adpt_handle->traffic_info[i].ho_chnl_id == 
                    adpt_handle->traffic_info[index].chnl_id))
                {
                    rc = adpt_dp_chnl_del(dev_id, i, dp_dir, TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }    
            }
        }

        if (enable)
        {
            switch (adpt_handle->traffic_info[index].odu_stage)
            {
                case DIGI_ODU_STRUCT_LEVEL_HO_ODU:
                    rc = adpt_dp_traffic_mode_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id,
                                                  adpt_handle->traffic_info[index].traffic_mode,
                                                  adpt_handle->traffic_info[index].fec_type,
                                                  adpt_handle->traffic_info[index].ilkn_info,
                                                  FALSE);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                case DIGI_ODU_STRUCT_LEVEL_MO_ODU:
                case DIGI_ODU_STRUCT_LEVEL_LO_ODU:
                    ho_chnl_id = adpt_handle->traffic_info[index].ho_chnl_id;
                    rc = adpt_chnl_index_get(adpt_handle, ho_chnl_id, &ho_index);
                    ADPT_RETURN_VERIFY(rc);

                    odu_mux.mapping_type  = adpt_handle->traffic_info[index].mapping_type;
                    odu_mux.oduflex_rate  = adpt_handle->traffic_info[index].oduflex_rate;
                    odu_mux.oduk_type     = adpt_handle->traffic_info[index].oduk_type;
                    odu_mux.otn_dir       = (digi_otn_dir_t)(adpt_handle->traffic_info[index].otn_dir & dp_dir);
                    odu_mux.payload_type  = adpt_handle->traffic_info[ho_index].payload_type;
                    odu_mux.rx_ts_mask[0] = adpt_handle->traffic_info[index].rx_ts_mask[0];
                    odu_mux.rx_ts_mask[1] = adpt_handle->traffic_info[index].rx_ts_mask[1];
                    odu_mux.rx_ts_mask[2] = adpt_handle->traffic_info[index].rx_ts_mask[2];
                    odu_mux.tx_ts_mask[0] = adpt_handle->traffic_info[index].tx_ts_mask[0];
                    odu_mux.tx_ts_mask[1] = adpt_handle->traffic_info[index].tx_ts_mask[1];
                    odu_mux.tx_ts_mask[2] = adpt_handle->traffic_info[index].tx_ts_mask[2];
                    odu_mux.oduflex_rate  = adpt_handle->traffic_info[index].oduflex_rate;
                    odu_mux.self_ts_type  = DIGI_ODU_UNCHANNELIZED;
                    odu_mux.tp_enable     = FALSE;

                    rc = adpt_dp_odu_mux_cfg(dev_id, adpt_handle->traffic_info[index].ho_chnl_id,
                                             adpt_handle->traffic_info[index].chnl_id,
                                             odu_mux);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                default:
                    break;
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_chnl_del */


/*******************************************************************************
* adpt_dp_chnl_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function adds a datapath for a given port or channel for datapath 
*   restoration. 
*
* INPUTS:
*   dev_id             - device identity.
*   index              - context index for restart_info.
*   dp_dir             - the Rx/Tx datapath or both to be added.
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
PRIVATE PMC_ERROR adpt_dp_chnl_add(UINT32 dev_id, UINT32 index, digi_dp_dir_t dp_dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 traffic_type = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    if ((DIGI_DP_NONE == dp_dir) || (dp_dir >= LAST_DIGI_DP_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->restart_info[index]->traffic_mode);
    switch (traffic_type)
    {
        case DIGI_TYPE_INLK:
            /* workaround to use ghao_id instead of ilkn_info which is UINT16 */
            rc = adpt_dp_traffic_mode_cfg(dev_id, 
                                          adpt_handle->restart_info[index]->chnl_id,
                                          adpt_handle->restart_info[index]->traffic_mode,
                                          adpt_handle->restart_info[index]->fec_type,
                                          adpt_handle->restart_info[index]->ghao_id,
                                          TRUE);
            ADPT_RETURN_VERIFY(rc);

            if (adpt_handle->restart_info[index]->map_enable)
            {
                rc = adpt_dp_client_map_cfg(dev_id, 
                                            adpt_handle->restart_info[index]->chnl_id,
                                            adpt_handle->restart_info[index]->mapping_mode,
                                            adpt_handle->restart_info[index]->ts_number, 
                                            TRUE);
                ADPT_RETURN_VERIFY(rc);
            }

            /* configure ILKN information */
            rc = adpt_ilkn_info_set(dev_id, 
                                    adpt_handle->restart_info[index]->chnl_id,
                                    adpt_handle->restart_info[index]->oduk_type,
                                    adpt_handle->restart_info[index]->oduflex_rate);
            ADPT_RETURN_VERIFY(rc);
            break;

        default:
            ADPT_ERROR("Input wrong traffic type[%d].\r\n", traffic_type);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_dp_chnl_add */


/*******************************************************************************
* adpt_rst_traffic_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures traffic mode in the line/system side for datapath 
*   restoration. It should be called before adpt_dp_restore(dev_id, DIGI_RESTORE_END).
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   traffic_mode       - traffic mode.
*   fec_type           - FEC type. Only available under OTU type.
*   ilkn_info          - ILKN channel information. Only available under ILKN type.
*   enable             - TRUE: enable traffic mode configuration.
*                        FALSE: disable traffic mode configuration.
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
PRIVATE PMC_ERROR adpt_rst_traffic_mode_cfg(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_traffic_mode_t traffic_mode,
                                       digi_otn_server_fec_t fec_type,
                                       UINT32 ilkn_info,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 traffic_type = 0;
    UINT32 client_mode = 0;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(traffic_mode);
    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(traffic_mode);
    /* get traffic type */
    traffic_type = DIGI_TRAFFIC_TYPE_GET(traffic_mode);
    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
        || (traffic_loc >= DIGI_SIDE_MAX)
        || (traffic_type >= DIGI_TYPE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* index handling */
    if (enable)
    {
        /* get the index in restart container */
        rc = adpt_rst_chnl_index_get(adpt_handle, chnl_id, &index);
        if (PMC_SUCCESS != rc)
        {
            /* create a restart container */
            rc = adpt_rst_chnl_index_create(adpt_handle, chnl_id, &index);
        }
    }
    else
    {
        /* get the index in restart container */
        rc = adpt_rst_chnl_index_get(adpt_handle, chnl_id, &index);
        if (PMC_SUCCESS != rc)
        {
            PMC_RETURN(PMC_SUCCESS);
        }
    }
    ADPT_RETURN_VERIFY(rc);

    /* record information */
    adpt_handle->restart_info[index]->enable       = enable;
    adpt_handle->restart_info[index]->fec_type     = fec_type;
    adpt_handle->restart_info[index]->traffic_mode = traffic_mode;
    adpt_handle->restart_info[index]->oduk_type    = (DIGI_TYPE_OTU == traffic_type) ? adpt_otu_odu_cvt((digi_otn_server_otuk_t)client_mode) : (digi_oduk_t)0;
    adpt_handle->restart_info[index]->oduflex_rate = 0;
    adpt_handle->restart_info[index]->payload_type = DIGI_ODU_UNCHANNELIZED;
    adpt_handle->restart_info[index]->odu_stage    = DIGI_ODU_STRUCT_LEVEL_HO_ODU;
    /* workaround to store ilkn_info which is UINT16 */
    adpt_handle->restart_info[index]->ghao_id      = ilkn_info;
    if (!enable)
    {
        /* destroy this ODUk container */
        rc = adpt_rst_chnl_index_destroy(adpt_handle, chnl_id, index);
    }

    PMC_RETURN(rc);
} /* adpt_rst_traffic_mode_cfg */


/*******************************************************************************
* adpt_rst_odu_mux_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure ODUk multiplexing/demultiplexing for 
*   datapath restoration. It should be called before adpt_dp_restore(dev_id,
*   DIGI_RESTORE_END).
*
* INPUTS:
*   dev_id             - device identity.
*   ho_chnl_id         - parent channel identity.
*   lo_chnl_id         - self channel identity.
*   odu_mux            - ODUk mux/demux related information.
*                        The oduflex_rate with bps unit is only available for 
*                        ODUflex(CBR). It should be more than 1000bps.
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
PRIVATE PMC_ERROR adpt_rst_odu_mux_cfg(UINT32 dev_id,
                                     UINT32 ho_chnl_id,
                                     UINT32 lo_chnl_id,
                                     digi_odu_mux_t odu_mux)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 lo_index = 0;
    UINT32 ho_index = 0;
    UINT32 index = 0;
    BOOL lo_exit = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    BOOL tp_remove = FALSE;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (odu_mux.tp_enable)
    {
        /* get the index in restart container */
        rc |= adpt_rst_chnl_index_get(adpt_handle, lo_chnl_id, &lo_index);
        if (PMC_SUCCESS != rc)
        {
            /* create a restart container */
            rc = adpt_rst_chnl_index_create(adpt_handle, lo_chnl_id, &lo_index);
        }
        rc |= adpt_rst_chnl_index_get(adpt_handle, ho_chnl_id, &ho_index);
        ADPT_RETURN_VERIFY(rc);

        /* payload type process */
        if (adpt_handle->restart_info[ho_index]->enable
            && (DIGI_ODU_UNCHANNELIZED == adpt_handle->restart_info[ho_index]->payload_type))
        {
            adpt_handle->restart_info[ho_index]->payload_type = odu_mux.payload_type;
        }
    }
    else
    {
        /* get the index in restart container */
        rc = adpt_rst_chnl_index_get(adpt_handle, ho_chnl_id, &ho_index);
        ADPT_RETURN_VERIFY(rc);
        rc = adpt_rst_chnl_index_get(adpt_handle, lo_chnl_id, &lo_index);
        if (PMC_SUCCESS != rc)
        {
            PMC_RETURN(PMC_SUCCESS);
        }

        if (!(odu_mux.otn_dir & adpt_handle->restart_info[lo_index]->otn_dir))
        {
            PMC_RETURN(PMC_SUCCESS);
        }

        /* HO ODUk payload type process */
        if (adpt_handle->restart_info[ho_index]->enable
            && (DIGI_ODU_UNCHANNELIZED != adpt_handle->restart_info[ho_index]->payload_type)
            && !(~(odu_mux.otn_dir) & adpt_handle->restart_info[lo_index]->otn_dir))
        {
            for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
            {
                if ((index != lo_index)
                    && adpt_handle->restart_info[index]->enable
                    && (adpt_handle->restart_info[index]->ho_chnl_id == adpt_handle->restart_info[ho_index]->chnl_id))
                {
                    lo_exit = TRUE;
                    break;
                }    
            }

            if (!lo_exit)
            {
                adpt_handle->restart_info[ho_index]->payload_type = DIGI_ODU_UNCHANNELIZED;
            }
        }
    }

    /* record information */
    if (odu_mux.tp_enable)
    {
        adpt_handle->restart_info[lo_index]->ho_chnl_id    = ho_chnl_id;
        adpt_handle->restart_info[lo_index]->mapping_type  = odu_mux.mapping_type;
        adpt_handle->restart_info[lo_index]->oduk_type     = odu_mux.oduk_type;
        adpt_handle->restart_info[lo_index]->payload_type  = odu_mux.self_ts_type;
        adpt_handle->restart_info[lo_index]->otn_dir       = (digi_otn_dir_t)(adpt_handle->restart_info[lo_index]->otn_dir | odu_mux.otn_dir);
        switch (odu_mux.otn_dir)
        {
            case DIGI_OTN_DIR_RX:
                for (i = 0; i < 3; i++)
                {
                    adpt_handle->restart_info[lo_index]->rx_ts_mask[i] = odu_mux.rx_ts_mask[i];
                }
                break;

            case DIGI_OTN_DIR_TX:
                for (i = 0; i < 3; i++)
                {
                    adpt_handle->restart_info[lo_index]->tx_ts_mask[i] = odu_mux.tx_ts_mask[i];
                }
                break;

            case DIGI_OTN_DIR_RX_TX:
                for (i = 0; i < 3; i++)
                {
                    adpt_handle->restart_info[lo_index]->rx_ts_mask[i] = odu_mux.rx_ts_mask[i];
                    adpt_handle->restart_info[lo_index]->tx_ts_mask[i] = odu_mux.tx_ts_mask[i];
                }
                break;

            default:
                break;
        }
        adpt_handle->restart_info[lo_index]->oduflex_rate  = odu_mux.oduflex_rate;
        adpt_handle->restart_info[lo_index]->odu_stage     = (digi_odu_struct_odu_level_t)(adpt_handle->restart_info[ho_index]->odu_stage + 1);
        adpt_handle->restart_info[lo_index]->enable        = odu_mux.tp_enable;
        adpt_handle->restart_info[lo_index]->traffic_mode  = adpt_handle->restart_info[ho_index]->traffic_mode;
    }
    else
    {
        switch (odu_mux.otn_dir)
        {
            case DIGI_OTN_DIR_RX:
                for (i = 0; i < 3; i++)
                {
                    adpt_handle->restart_info[lo_index]->rx_ts_mask[i] = 0;
                }
                break;

            case DIGI_OTN_DIR_TX:
                for (i = 0; i < 3; i++)
                {
                    adpt_handle->restart_info[lo_index]->tx_ts_mask[i] = 0;
                }
                break;

            case DIGI_OTN_DIR_RX_TX:
                for (i = 0; i < 3; i++)
                {
                    adpt_handle->restart_info[lo_index]->rx_ts_mask[i] = 0;
                    adpt_handle->restart_info[lo_index]->tx_ts_mask[i] = 0;
                }
                break;

            default:
                break;
        }

        if (odu_mux.otn_dir == adpt_handle->restart_info[lo_index]->otn_dir)
        {
            tp_remove = TRUE;
        }
        adpt_handle->restart_info[lo_index]->otn_dir = (digi_otn_dir_t)(adpt_handle->restart_info[lo_index]->otn_dir & ~(odu_mux.otn_dir));

        /* destroy this ODUk container */
        if (tp_remove)
        {
            rc = adpt_rst_chnl_index_destroy(adpt_handle, lo_chnl_id, lo_index);
        }
    }

    PMC_RETURN(rc);
} /* adpt_rst_odu_mux_cfg */


/*******************************************************************************
* adpt_rst_client_map_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures client end point and then maps the client to its
*   end point for datapath restoration. It should be called before 
*   adpt_dp_restore(dev_id, DIGI_RESTORE_END).
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mapping_mode       - client mapping mode.
*   ts_number          - tributary slot number. Only available for ODUflex.
*   enable             - TRUE: enable the client mapping configuration.
*                        FALSE: disable the client mapping confguration.
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
PRIVATE PMC_ERROR adpt_rst_client_map_cfg(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_mapotn_mode_t mapping_mode,
                                      UINT32 ts_number,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in restart container */
    rc = adpt_rst_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    adpt_handle->restart_info[index]->mapping_mode = mapping_mode;
    adpt_handle->restart_info[index]->ts_number    = ts_number;
    adpt_handle->restart_info[index]->map_enable   = enable;

    PMC_RETURN(rc);
} /* adpt_rst_client_map_cfg */


/*******************************************************************************
* adpt_rst_cpb_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure the CPB switch which goes through 
*   MAPOTN for datapath restoration. It should be called before 
*   adpt_dp_restore(dev_id, DIGI_RESTORE_END).
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - direction to MAPOTN.
*   enable             - TRUE: acticate CPB switch.
*                        FALSE: deactivate CPB switch.
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
PRIVATE PMC_ERROR adpt_rst_cpb_chnl_enable(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_cpb_dir_t dir,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT8 map_sw_info = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in restart container */
    rc = adpt_rst_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    map_sw_info = adpt_handle->restart_info[index]->map_sw_info;
    switch (dir)
    {
        case DIGI_CPB_MAP_TO_CBR:
            adpt_handle->restart_info[index]->map_sw_info = DIGI_SWT_CBR_SET(map_sw_info, enable, enable, dir);
            break;

        case DIGI_CPB_CBR_TO_MAP:
        case DIGI_CPB_MAP_TO_MAP:
            adpt_handle->restart_info[index]->map_sw_info = DIGI_SWT_MAP_SET(map_sw_info, enable, enable, dir);
            break;

        default:
            ADPT_ERROR("Wrong direction type[%d].\r\n", dir);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_rst_cpb_chnl_enable */


/*******************************************************************************
* adpt_rst_packet_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure unidirectional switch function in 
*   DIGI device for datapath restoration. It should be called before 
*   adpt_dp_restore(dev_id, DIGI_RESTORE_END).
*
* INPUTS:
*   dev_id             - device identity.
*   switch_mode        - switch mode: ODUk switch, CPB switch.
*   src_chnl_id        - source channel identity.
*   dest_chnl_id       - destination channel identity.
*   enable             - TRUE: enable cross connection.
*                        FALSE: disable cross connection.
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
PRIVATE PMC_ERROR adpt_rst_packet_switch(UINT32 dev_id,
                                     digi_switch_mode_t switch_mode,
                                     UINT32 src_chnl_id,
                                     UINT32 dest_chnl_id,
                                     BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 src_index = 0;
    UINT32 dest_index = 0;
    UINT32 src_traffic_type = 0;
    UINT32 dest_traffic_type = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in restart container */
    rc |= adpt_rst_chnl_index_get(adpt_handle, src_chnl_id, &src_index);
    rc |= adpt_rst_chnl_index_get(adpt_handle, dest_chnl_id, &dest_index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic type */
    src_traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->restart_info[src_index]->traffic_mode);
    dest_traffic_type = DIGI_TRAFFIC_TYPE_GET(adpt_handle->restart_info[dest_index]->traffic_mode);
    if ((src_traffic_type >= DIGI_TYPE_MAX) || (dest_traffic_type >= DIGI_TYPE_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if ((DIGI_SWITCH_ODUK == switch_mode) || (DIGI_SWITCH_ODUK_CPB == switch_mode))
    {
        /* record information */
        adpt_handle->restart_info[dest_index]->oduk_src_index = DIGI_SWT_SET(enable, enable, src_index);

        /* CPB switch */
        if (DIGI_SWITCH_ODUK_CPB == switch_mode)
        {
            if (adpt_handle->restart_info[dest_index]->map_enable)
            {
                rc = adpt_rst_cpb_chnl_enable(dev_id, dest_chnl_id, DIGI_CPB_MAP_TO_CBR, TRUE);
            }
            ADPT_RETURN_VERIFY(rc);

            if (adpt_handle->restart_info[src_index]->map_enable)
            {
                rc = adpt_rst_cpb_chnl_enable(dev_id, src_chnl_id, DIGI_CPB_CBR_TO_MAP, TRUE);
            }
            ADPT_RETURN_VERIFY(rc);
        }
    }
    else if (DIGI_SWITCH_CPB == switch_mode)
    {
        /* record information */
        adpt_handle->restart_info[dest_index]->cpb_src_index = DIGI_SWT_SET(enable, enable, src_index);
    }
    else
    {
        rc = PMC_ERR_FAIL;
    }

    PMC_RETURN(rc);
} /* adpt_rst_packet_switch */


/*******************************************************************************
* adpt_traffic_mode_cmp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compares the input traffic mode with the Adapter context if 
*   they are the same. If not, the old traffic mode will be deleted, as well as
*   all resources in this channel. Otherwise, update the chnl_id if necessary. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   traffic_mode       - traffic mode.
*   fec_type           - FEC type. Only available under OTU type.
*   ilkn_info          - ILKN channel information. Only available under ILKN type.
*   enable             - TRUE: enable traffic mode configuration.
*                        FALSE: disable traffic mode configuration.
*
* OUTPUTS:
*   *match             - TRUE: the context has the same configuration.
*                        FALSE: the context has no this configuration. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_traffic_mode_cmp(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_traffic_mode_t traffic_mode,
                                     digi_otn_server_fec_t fec_type,
                                     UINT32 ilkn_info,
                                     BOOL enable,
                                     BOOL *match)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    UINT32 traffic_type = 0;
    UINT32 cxt_serdes_port = 0;
    UINT32 cxt_traffic_loc = 0;
    UINT32 index = 0;
    UINT32 ilkn_rate = 0;
    BOOL result = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][0x%x][%d][%d][%d].\r\n", dev_id,
               chnl_id, traffic_mode, fec_type, ilkn_info, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(match);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(traffic_mode);
    /* get traffic type */
    traffic_type = DIGI_TRAFFIC_TYPE_GET(traffic_mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default */
    *match = FALSE;
    /* check the index only for the disabled action */
    if (!enable)
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
        if (PMC_SUCCESS == rc)
        {
            if (((NULL != adpt_handle->traffic_info[index].otu_handle) && (DIGI_TYPE_OTU == traffic_type))
                || ((NULL != adpt_handle->traffic_info[index].enet_handle) && (DIGI_TYPE_ETH == traffic_type))
                || ((NULL != adpt_handle->traffic_info[index].cbr_handle) && (DIGI_TYPE_CBR == traffic_type))
                || ((NULL != adpt_handle->traffic_info[index].ilkn_handle) && (DIGI_TYPE_INLK == traffic_type)))
            {
                /* delete the datapath channel except traffic mode */
                rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, FALSE);
                ADPT_RETURN_VERIFY(rc);
            }
            else
            {
                /* the chnl_id or traffic type is invalid */
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }
        }
        else
        {
            *match = TRUE;
        }

        PMC_RETURN(PMC_SUCCESS);
    }

    for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
    {
        if (!(adpt_handle->traffic_info[index].enable))
        {
            continue;
        }

        /* get serdes port */
        cxt_serdes_port = DIGI_SERDES_PORT_GET(adpt_handle->traffic_info[index].chnl_id);
        /* get traffic location */
        cxt_traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);

        /* OTU/ENET/CBR channel */
        if (((NULL != adpt_handle->traffic_info[index].otu_handle)
            || (NULL != adpt_handle->traffic_info[index].enet_handle)
            || (NULL != adpt_handle->traffic_info[index].cbr_handle))
            && (DIGI_TYPE_INLK != traffic_type))
        {
            if ((cxt_serdes_port == serdes_port) && (cxt_traffic_loc == traffic_loc))
            {
                if (adpt_handle->traffic_info[index].traffic_mode == traffic_mode)
                {
                    /* update channel ID */
                    rc = adpt_chnl_id_update(dev_id, index, chnl_id);
                    ADPT_RETURN_VERIFY(rc);

                    /* update FEC type */
                    if (DIGI_TYPE_OTU == traffic_type)
                    {
                        /* get FEC type */
                        rc = adpt_otn_fec_type_get(dev_id, chnl_id, &adpt_handle->traffic_info[index].fec_type);
                        ADPT_RETURN_VERIFY(rc);

                        if (fec_type != adpt_handle->traffic_info[index].fec_type)
                        {
                            rc = adpt_otn_fec_type_set(dev_id, chnl_id, fec_type);
                            ADPT_RETURN_VERIFY(rc);
                        }
                    }

                    /* update enable flag */
                    adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_RX_TX);

                    *match = TRUE;
                }
                else
                {
                    /* delete the datapath channel */
                    rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }

                break;
            }
        }
        /* ILKN channel */
        else if ((NULL != adpt_handle->traffic_info[index].ilkn_handle)
                 && (DIGI_TYPE_INLK == traffic_type))
        {
            if ((adpt_handle->traffic_info[index].traffic_mode == traffic_mode)
                && (adpt_handle->traffic_info[index].ilkn_info == ilkn_info))
            {
                /* update channel ID */
                rc = adpt_chnl_id_update(dev_id, index, chnl_id);

                /* update enable flag */
                adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_RX_TX);

                *match = TRUE;
                break;
            }
            else
            {
                if ((DIGI_ILKN_CHNL_GET(adpt_handle->traffic_info[index].ilkn_info) == 
                    DIGI_ILKN_CHNL_GET(ilkn_info))
                    && adpt_ilkn_grp_chk(adpt_handle->traffic_info[index].traffic_mode, 
                    traffic_mode))
                {
                    if ((DIGI_ILKN_RATE_GET(adpt_handle->traffic_info[index].ilkn_info) != 
                        DIGI_ILKN_RATE_GET(ilkn_info))
                        || (adpt_handle->traffic_info[index].traffic_mode != traffic_mode))
                    {
                        /* delete the datapath channel */
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }

                    break;
                }
            }
        }
    }

    if (!(*match))
    {
        /* check if this chnl_id exists in the Adapter context */
        rc = adpt_chnl_id_chk(dev_id, chnl_id);
        ADPT_RETURN_VERIFY(rc);
    }

    /* check ILKN BCW resource */
    if ((DIGI_TYPE_INLK == traffic_type) && !(*match))
    {
        /* collect existing ILKN rate resource */
        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            if (!(adpt_handle->traffic_info[index].enable))
            {
                continue;
            }

            if ((NULL != adpt_handle->traffic_info[index].ilkn_handle)
                && adpt_ilkn_grp_chk(adpt_handle->traffic_info[index].traffic_mode, traffic_mode))
            {
                /* clean up those ILKN channels without ODUk SW/CPB */
                if (!(DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[index].enable)))
                {
                    rc = adpt_dp_switch_chk(dev_id, index, DIGI_DP_RX_TX, &result);
                    ADPT_RETURN_VERIFY(rc);

                    if (!result)
                    {
                        /* delete the datapath channel */
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }

                if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
                {
                    ilkn_rate += DIGI_ILKN_RATE_GET(adpt_handle->traffic_info[index].ilkn_info);
                }
            }
        }

        /* handle blocked ILKN resource */
        if ((ilkn_rate + DIGI_ILKN_RATE_GET(ilkn_info)) > DIGI_ILKN_TOTAL_RATE)
        {
            /* record to be handled in the adpt_dp_restore(END) */
            if (DIGI_INIT_RESTART == adpt_handle->init_state)
            {
                *match = TRUE;
            }
            else
            {
                rc = DIGI_ERR_SCHD_INSUFFICIENT_RESOURCES_AVAILABLE;
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_traffic_mode_cmp */


/*******************************************************************************
* adpt_odu_mux_cmp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compares the input ODUk multiplexing/demultiplexing information 
*   with the Adapter context if they are the same. If not, the old ODUk will be 
*   deleted, as well as all resources in this channel. Otherwise, update the 
*   chnl_id if necessary.
*
* INPUTS:
*   dev_id             - device identity.
*   ho_chnl_id         - parent channel identity.
*   lo_chnl_id         - self channel identity.
*   odu_mux            - ODUk mux/demux related information.
*                        The oduflex_rate with bps unit is only available for 
*                        ODUflex(CBR). It should be more than 1000bps.
*
* OUTPUTS:
*   *match             - the context configuration in the Rx/Tx direction or both.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_odu_mux_cmp(UINT32 dev_id,
                                   UINT32 ho_chnl_id,
                                   UINT32 lo_chnl_id,
                                   digi_odu_mux_t odu_mux,
                                   digi_dp_dir_t *match)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 tx_ts_cnt = 0;
    UINT32 rx_ts_cnt = 0;
    UINT32 ho_index = 0;
    UINT32 lo_index = 0;
    UINT32 index = 0;
    UINT32 chnl_num = 0;
    UINT32 chnl_max = 0;
    BOOL odu_rx = FALSE;
    BOOL chnl_exist = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d][0x%x][0x%x][0x%x][0x%x][0x%x][0x%x][%d][%lf][%d].\r\n", 
               dev_id, ho_chnl_id, lo_chnl_id, odu_mux.oduk_type,
               odu_mux.mapping_type, odu_mux.payload_type, odu_mux.otn_dir,
               odu_mux.tx_ts_mask[0], odu_mux.tx_ts_mask[1], odu_mux.tx_ts_mask[2],
               odu_mux.rx_ts_mask[0], odu_mux.rx_ts_mask[1], odu_mux.rx_ts_mask[2],
               odu_mux.tp_enable, odu_mux.oduflex_rate, odu_mux.self_ts_type);

    /* check parameter validation */
    ADPT_PARA_VERIFY(match);

    if (((DIGI_ODU_TS_1G25 != odu_mux.payload_type) && (DIGI_ODU_TS_2G5 != odu_mux.payload_type))
         || ((odu_mux.oduflex_rate > 0.001) && (odu_mux.oduflex_rate < 1000.0))
         || (odu_mux.otn_dir >= LAST_DIGI_OTN_DIR))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* check Tx TS count with Rx TS count */
    for (index = 0; index < 96; index++)
    {
        if ((odu_mux.tx_ts_mask[index/32] >> (index % 32)) & 1)
        {
            tx_ts_cnt++;
        }

        if ((odu_mux.rx_ts_mask[index/32] >> (index % 32)) & 1)
        {
            rx_ts_cnt++;
        }
    }

    if (((tx_ts_cnt != rx_ts_cnt) && (0 != tx_ts_cnt) && (0 != rx_ts_cnt))
         || ((0 == tx_ts_cnt) && (0 == rx_ts_cnt) && odu_mux.tp_enable))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default */
    *match = DIGI_DP_NONE;

    rc = adpt_chnl_index_get(adpt_handle, ho_chnl_id, &ho_index);
    ADPT_RETURN_VERIFY(rc);

    /* check odu_handle in high order */
    if (NULL == adpt_handle->traffic_info[ho_index].odu_handle)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (!odu_mux.tp_enable)
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, lo_chnl_id, &index);
        if (PMC_SUCCESS == rc)
        {
            if (NULL != adpt_handle->traffic_info[index].odu_handle) 
            {
                switch (adpt_handle->traffic_info[index].otn_dir & odu_mux.otn_dir)
                {
                    case DIGI_OTN_DIR_RX:
                        /* delete ODUk SW / CPB cross connections */
                        rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_ODUK, DIGI_DP_TX);
                        ADPT_RETURN_VERIFY(rc);
                        break;

                    case DIGI_OTN_DIR_TX:
                        /* delete ODUk SW / CPB cross connections */
                        rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_ODUK, DIGI_DP_RX);
                        ADPT_RETURN_VERIFY(rc);
                        break;

                    case DIGI_OTN_DIR_RX_TX:
                        /* delete ODUk SW / CPB cross connections */
                        rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_ODUK, DIGI_DP_RX_TX);
                        ADPT_RETURN_VERIFY(rc);
                        break;

                    default:
                        *match = (digi_dp_dir_t)odu_mux.otn_dir;
                        break;
                }
            }
            else
            {
                /* the chnl_id or traffic type is invalid */
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            }
        }
        else
        {
            *match = DIGI_DP_RX_TX;
        }

        PMC_RETURN(PMC_SUCCESS);
    }

    /* check if lo_chnl_id exists */
    rc = adpt_chnl_index_get(adpt_handle, lo_chnl_id, &lo_index);
    if ((PMC_SUCCESS == rc)
        && (adpt_handle->traffic_info[lo_index].ho_chnl_id == ho_chnl_id)
        && (DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[lo_index].enable)))
    {
        chnl_exist = TRUE;
    }

    /* check Tx configuration prior to Rx in order to avoid resource collision */
    if ((!chnl_exist) && (DIGI_OTN_DIR_RX == odu_mux.otn_dir))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* handle the payload type of high order */
    switch (adpt_handle->traffic_info[ho_index].payload_type)
    {
        case DIGI_ODU_UNCHANNELIZED:
            /* delete ODUk SW / CPB cross connections */
            rc = adpt_dp_switch_del(dev_id, ho_index, DIGI_SWITCH_ODUK, DIGI_DP_RX_TX);
            ADPT_RETURN_VERIFY(rc);

            /* check if this chnl_id exists in the Adapter context */
            rc = adpt_chnl_id_chk(dev_id, lo_chnl_id);
            ADPT_RETURN_VERIFY(rc);

            PMC_RETURN(rc);
            break;

        case DIGI_ODU_NULL:
            /* disable the NULL signal */
            rc = adpt_otn_null_sig_set(dev_id, ho_chnl_id, FALSE);
            ADPT_RETURN_VERIFY(rc);

            /* check if this chnl_id exists in the Adapter context */
            rc = adpt_chnl_id_chk(dev_id, lo_chnl_id);
            ADPT_RETURN_VERIFY(rc);

            PMC_RETURN(rc);
            break;

        default:
            /* check the payload type with input information */
            if (adpt_handle->traffic_info[ho_index].payload_type != odu_mux.payload_type)
            {
                /* delete all daughter TPs */
                rc = adpt_dp_chnl_del(dev_id, ho_index, DIGI_DP_RX_TX, FALSE);
                ADPT_RETURN_VERIFY(rc);

                /* check if this chnl_id exists in the Adapter context */
                rc = adpt_chnl_id_chk(dev_id, lo_chnl_id);
                ADPT_RETURN_VERIFY(rc);

                PMC_RETURN(rc);
            }
            break;
    }

    for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
    {
        if (!(adpt_handle->traffic_info[index].enable))
        {
            continue;
        }

        if (adpt_handle->traffic_info[index].ho_chnl_id != ho_chnl_id)
        {
            continue;
        }

        /* check asymmetric configuration */
        switch (adpt_handle->traffic_info[index].otn_dir & odu_mux.otn_dir)
        {
            case DIGI_OTN_DIR_RX:
                if ((adpt_handle->traffic_info[index].rx_ts_mask[0] == odu_mux.rx_ts_mask[0])
                    && (adpt_handle->traffic_info[index].rx_ts_mask[1] == odu_mux.rx_ts_mask[1])
                    && (adpt_handle->traffic_info[index].rx_ts_mask[2] == odu_mux.rx_ts_mask[2]))
                {
                     if ((adpt_handle->traffic_info[index].oduk_type == odu_mux.oduk_type)
                         && ((PMC_COMPARE_DOUBLE(adpt_handle->traffic_info[index].oduflex_rate, 
                         odu_mux.oduflex_rate, 0.0001) && (DIGI_ODUK_ODUFLEX == odu_mux.oduk_type)) 
                         || (DIGI_ODUK_ODUFLEX != odu_mux.oduk_type))
                         && (!chnl_exist || (chnl_exist && (index == lo_index)))
                         && (DIGI_DP_NONE == *match))
                     {
                        /* update channel ID */
                        rc = adpt_chnl_id_update(dev_id, index, lo_chnl_id);
                        ADPT_RETURN_VERIFY(rc);

                        /* update enable flag */
                        adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_RX);

                        *match = DIGI_DP_RX;
                     }
                     else
                     {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                     }
                }                    
                else if ((adpt_handle->traffic_info[index].rx_ts_mask[0] & odu_mux.rx_ts_mask[0])
                         || (adpt_handle->traffic_info[index].rx_ts_mask[1] & odu_mux.rx_ts_mask[1])
                         || (adpt_handle->traffic_info[index].rx_ts_mask[2] & odu_mux.rx_ts_mask[2]))
                {
                    rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }
                break;

            case DIGI_OTN_DIR_TX:
                if ((adpt_handle->traffic_info[index].tx_ts_mask[0] == odu_mux.tx_ts_mask[0])
                    && (adpt_handle->traffic_info[index].tx_ts_mask[1] == odu_mux.tx_ts_mask[1])
                    && (adpt_handle->traffic_info[index].tx_ts_mask[2] == odu_mux.tx_ts_mask[2]))
                {
                     if ((adpt_handle->traffic_info[index].oduk_type == odu_mux.oduk_type)
                         && ((PMC_COMPARE_DOUBLE(adpt_handle->traffic_info[index].oduflex_rate, 
                         odu_mux.oduflex_rate, 0.0001) && (DIGI_ODUK_ODUFLEX == odu_mux.oduk_type)) 
                         || (DIGI_ODUK_ODUFLEX != odu_mux.oduk_type))
                         && (!chnl_exist || (chnl_exist && (index == lo_index)))
                         && (DIGI_DP_NONE == *match))
                     {
                        /* update channel ID */
                        rc = adpt_chnl_id_update(dev_id, index, lo_chnl_id);
                        ADPT_RETURN_VERIFY(rc);

                        /* update enable flag */
                        adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_TX);

                        *match = DIGI_DP_TX;
                     }
                     else
                     {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                     }
                }                    
                else if ((adpt_handle->traffic_info[index].tx_ts_mask[0] & odu_mux.tx_ts_mask[0])
                         || (adpt_handle->traffic_info[index].tx_ts_mask[1] & odu_mux.tx_ts_mask[1])
                         || (adpt_handle->traffic_info[index].tx_ts_mask[2] & odu_mux.tx_ts_mask[2]))
                {
                    rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_TX, TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }
                break;

            case DIGI_OTN_DIR_RX_TX:
                if ((adpt_handle->traffic_info[index].rx_ts_mask[0] == odu_mux.rx_ts_mask[0])
                    && (adpt_handle->traffic_info[index].rx_ts_mask[1] == odu_mux.rx_ts_mask[1])
                    && (adpt_handle->traffic_info[index].rx_ts_mask[2] == odu_mux.rx_ts_mask[2])
                    && (adpt_handle->traffic_info[index].tx_ts_mask[0] == odu_mux.tx_ts_mask[0])
                    && (adpt_handle->traffic_info[index].tx_ts_mask[1] == odu_mux.tx_ts_mask[1])
                    && (adpt_handle->traffic_info[index].tx_ts_mask[2] == odu_mux.tx_ts_mask[2]))
                {
                     if ((adpt_handle->traffic_info[index].oduk_type == odu_mux.oduk_type)
                         && ((PMC_COMPARE_DOUBLE(adpt_handle->traffic_info[index].oduflex_rate, 
                         odu_mux.oduflex_rate, 0.0001) && (DIGI_ODUK_ODUFLEX == odu_mux.oduk_type)) 
                         || (DIGI_ODUK_ODUFLEX != odu_mux.oduk_type))
                         && (!chnl_exist || (chnl_exist && (index == lo_index))))
                     {
                        /* update channel ID */
                        rc = adpt_chnl_id_update(dev_id, index, lo_chnl_id);
                        ADPT_RETURN_VERIFY(rc);

                        /* update enable flag */
                        adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_RX_TX);

                        *match = DIGI_DP_RX_TX;
                     }
                     else
                     {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                     }
                }                    
                else if (((adpt_handle->traffic_info[index].rx_ts_mask[0] & odu_mux.rx_ts_mask[0])
                         || (adpt_handle->traffic_info[index].rx_ts_mask[1] & odu_mux.rx_ts_mask[1])
                         || (adpt_handle->traffic_info[index].rx_ts_mask[2] & odu_mux.rx_ts_mask[2]))
                         && ((adpt_handle->traffic_info[index].tx_ts_mask[0] & odu_mux.tx_ts_mask[0])
                         || (adpt_handle->traffic_info[index].tx_ts_mask[1] & odu_mux.tx_ts_mask[1])
                         || (adpt_handle->traffic_info[index].tx_ts_mask[2] & odu_mux.tx_ts_mask[2])))
                {
                    rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }
                else 
                {
                    if ((adpt_handle->traffic_info[index].rx_ts_mask[0] == odu_mux.rx_ts_mask[0])
                        && (adpt_handle->traffic_info[index].rx_ts_mask[1] == odu_mux.rx_ts_mask[1])
                        && (adpt_handle->traffic_info[index].rx_ts_mask[2] == odu_mux.rx_ts_mask[2]))
                    {
                         if ((adpt_handle->traffic_info[index].oduk_type == odu_mux.oduk_type)
                             && ((PMC_COMPARE_DOUBLE(adpt_handle->traffic_info[index].oduflex_rate, 
                             odu_mux.oduflex_rate, 0.0001) && (DIGI_ODUK_ODUFLEX == odu_mux.oduk_type)) 
                             || (DIGI_ODUK_ODUFLEX != odu_mux.oduk_type))
                             && (!chnl_exist || (chnl_exist && (index == lo_index)))
                             && (DIGI_DP_NONE == *match))
                         {
                            /* delete Tx datapath */
                            rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_TX, TRUE);
                            ADPT_RETURN_VERIFY(rc);

                            /* update channel ID */
                            rc = adpt_chnl_id_update(dev_id, index, lo_chnl_id);
                            ADPT_RETURN_VERIFY(rc);

                            /* update enable flag */
                            adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_RX);

                            *match = DIGI_DP_RX;
                         }
                         else
                         {
                            rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                            ADPT_RETURN_VERIFY(rc);
                         }
                    }                    
                    else if ((adpt_handle->traffic_info[index].rx_ts_mask[0] & odu_mux.rx_ts_mask[0])
                             || (adpt_handle->traffic_info[index].rx_ts_mask[1] & odu_mux.rx_ts_mask[1])
                             || (adpt_handle->traffic_info[index].rx_ts_mask[2] & odu_mux.rx_ts_mask[2]))
                    {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }

                    if ((adpt_handle->traffic_info[index].tx_ts_mask[0] == odu_mux.tx_ts_mask[0])
                        && (adpt_handle->traffic_info[index].tx_ts_mask[1] == odu_mux.tx_ts_mask[1])
                        && (adpt_handle->traffic_info[index].tx_ts_mask[2] == odu_mux.tx_ts_mask[2]))
                    {
                         if ((adpt_handle->traffic_info[index].oduk_type == odu_mux.oduk_type)
                             && ((PMC_COMPARE_DOUBLE(adpt_handle->traffic_info[index].oduflex_rate, 
                             odu_mux.oduflex_rate, 0.0001) && (DIGI_ODUK_ODUFLEX == odu_mux.oduk_type)) 
                             || (DIGI_ODUK_ODUFLEX != odu_mux.oduk_type))
                             && (!chnl_exist || (chnl_exist && (index == lo_index)))
                             && (DIGI_DP_NONE == *match))
                         {
                            /* delete Rx datapath */
                            rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                            ADPT_RETURN_VERIFY(rc);

                            /* update channel ID */
                            rc = adpt_chnl_id_update(dev_id, index, lo_chnl_id);
                            ADPT_RETURN_VERIFY(rc);

                            /* update enable flag */
                            adpt_handle->traffic_info[index].enable = DIGI_EN_SET(adpt_handle->traffic_info[index].enable, DIGI_DP_TX);

                            *match = DIGI_DP_TX;
                         }
                         else
                         {
                            rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_TX, TRUE);
                            ADPT_RETURN_VERIFY(rc);
                         }
                    }                    
                    else if ((adpt_handle->traffic_info[index].tx_ts_mask[0] & odu_mux.tx_ts_mask[0])
                             || (adpt_handle->traffic_info[index].tx_ts_mask[1] & odu_mux.tx_ts_mask[1])
                             || (adpt_handle->traffic_info[index].tx_ts_mask[2] & odu_mux.tx_ts_mask[2]))
                    {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }
                break;

            default:
                /* break for Rx only in the context with Tx only input and vice verse */
                break;
        }
    }

    if ((DIGI_DP_NONE == *match) && !chnl_exist)
    {
        /* check if this chnl_id exists in the Adapter context */
        rc = adpt_chnl_id_chk(dev_id, lo_chnl_id);
        ADPT_RETURN_VERIFY(rc);

        /* check ODUk channel resource */
        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            if (!(adpt_handle->traffic_info[index].enable))
            {
                continue;
            }

            if (adpt_handle->traffic_info[index].ho_chnl_id == ho_chnl_id)
            {
                chnl_num++;

                /* check if there is a Rx only ODUk without calling */
                if ((DIGI_OTN_DIR_RX == adpt_handle->traffic_info[index].otn_dir)
                    && !(DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[index].enable)))
                {
                    odu_rx = TRUE;
                }
            }
        }

        rc = adpt_msi_len_get(dev_id, ho_chnl_id, &chnl_max);
        ADPT_RETURN_VERIFY(rc);

        /* clean up a Rx only ODUk resource */
        if (chnl_num == chnl_max)
        {
            for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
            {
                if (!(adpt_handle->traffic_info[index].enable))
                {
                    continue;
                }

                if ((adpt_handle->traffic_info[index].ho_chnl_id == ho_chnl_id)
                    && (DIGI_OTN_DIR_RX == adpt_handle->traffic_info[index].otn_dir))
                {
                    if (odu_rx)
                    {
                        if (!(DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[index].enable)))
                        {
                            rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                            ADPT_RETURN_VERIFY(rc);

                            break;
                        }
                    }
                    else
                    {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                        ADPT_RETURN_VERIFY(rc);

                        break;
                    }
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_odu_mux_cmp */


/*******************************************************************************
* adpt_client_map_cmp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compares the input client mapping with the Adapter context if 
*   they are the same. If not, the old client mapping will be deleted, as well 
*   as ODUk SW / CPB in this channel.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mapping_mode       - client mapping mode.
*   ts_number          - tributary slot number. Only available for ODUflex.
*   enable             - TRUE: enable the client mapping configuration.
*                        FALSE: disable the client mapping confguration.
*
* OUTPUTS:
*   *match             - TRUE: the context has the same configuration.
*                        FALSE: the context has no this configuration. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_client_map_cmp(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_mapotn_mode_t mapping_mode,
                                    UINT32 ts_number,
                                    BOOL enable,
                                    BOOL *match)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    UINT32 ts_sum = 0;
    BOOL result = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               chnl_id, mapping_mode, ts_number, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(match);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default */
    *match = FALSE;

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    if (PMC_SUCCESS != rc)
    {
        if (DIGI_INIT_RESTART == adpt_handle->init_state)
        {
            /* handle blocked resources */
            *match = TRUE;
            PMC_RETURN(PMC_SUCCESS);
        }
        else
        {
            PMC_RETURN(rc);
        }
    }

    if ((NULL != adpt_handle->traffic_info[index].map_handle) && enable)
    {
        if (adpt_handle->traffic_info[index].mapping_mode == mapping_mode)
        {
            if (DIGI_MAPOTN_ODUFLEX_PACKET_GFP == mapping_mode)
            {
                if (adpt_handle->traffic_info[index].ts_number == ts_number)
                {
                    *match = TRUE;
                }
            }
            else
            {
                *match = TRUE;
            }
        }

        if (*match)
        {
            /* update enable flag */
            adpt_handle->traffic_info[index].map_enable = DIGI_EN_SET(adpt_handle->traffic_info[index].map_enable, DIGI_DP_RX_TX);
        }
        else
        {
            /* delete client mapping */
            rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, FALSE);
            ADPT_RETURN_VERIFY(rc);
        }
    }
    else if ((NULL == adpt_handle->traffic_info[index].map_handle) && !enable)
    {
        *match = TRUE;
    }

    /* check client mapping resource */
    if (!(*match) && enable)
    {
        /* collect existing client mapping resource */
        for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
        {
            if (!(adpt_handle->traffic_info[i].enable))
            {
                continue;
            }

            if (NULL != adpt_handle->traffic_info[i].map_handle)
            {
                /* clean up those channels without ODUk SW/CPB */
                if (!(DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[i].map_enable)))
                {
                    rc = adpt_dp_switch_chk(dev_id, i, DIGI_DP_RX_TX, &result);
                    ADPT_RETURN_VERIFY(rc);

                    if (!result)
                    {
                        rc = adpt_dp_client_map_cfg(dev_id, adpt_handle->traffic_info[i].chnl_id,
                                                    adpt_handle->traffic_info[i].mapping_mode,
                                                    0, FALSE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }

                if (NULL != adpt_handle->traffic_info[i].map_handle)
                {
                    if (DIGI_MAPOTN_ODUFLEX_PACKET_GFP == adpt_handle->traffic_info[i].mapping_mode)
                    {
                        ts_sum += adpt_handle->traffic_info[i].ts_number;
                    }
                    else
                    {
                        ts_sum += adpt_mapper_ts_get(adpt_handle->traffic_info[i].mapping_mode);
                    }
                }
            }
        }

        if (DIGI_MAPOTN_ODUFLEX_PACKET_GFP != mapping_mode)
        {
            ts_number = adpt_mapper_ts_get(mapping_mode);
        }

        /* handle blocked client mapping resource */
        if ((ts_number + ts_sum) > DIGI_ODU_CHNLS_MAX)
        {
            /* record to be handled in the adpt_dp_restore(END) */
            if (DIGI_INIT_RESTART == adpt_handle->init_state)
            {
                *match = TRUE;
            }
            else
            {
                rc = DIGI_ERR_SCHD_INSUFFICIENT_RESOURCES_AVAILABLE;
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_client_map_cmp */


/*******************************************************************************
* adpt_cpb_chnl_enable_cmp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compares the CPB switch with the Adapter context if they are the
*   same. If not, the old CPB switch might be deleted. Otherwise, this function 
*   returns directly.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - direction to MAPOTN.
*   enable             - TRUE: acticate CPB switch.
*                        FALSE: deactivate CPB switch.
*
* OUTPUTS:
*   *match             - TRUE: the context has the same configuration.
*                        FALSE: the context has no this configuration. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_cpb_chnl_enable_cmp(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_cpb_dir_t dir,
                                         BOOL enable,
                                         BOOL *match)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 i = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT8 map_sw_info = 0;
    PMC_ENTRY();

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default */
    *match = FALSE;

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    if (PMC_SUCCESS != rc)
    {
        if (DIGI_INIT_RESTART == adpt_handle->init_state)
        {
            /* handle blocked resources */
            *match = TRUE;
            PMC_RETURN(PMC_SUCCESS);
        }
        else
        {
            PMC_RETURN(rc);
        }
    }

    map_sw_info = adpt_handle->traffic_info[index].map_sw_info;
    switch (dir)
    {
        case DIGI_CPB_MAP_TO_CBR:
            if ((enable && DIGI_SWT_CBR_EN_GET(map_sw_info))
                 || (!enable && !DIGI_SWT_CBR_EN_GET(map_sw_info)))
            {
                *match = TRUE;

                /* update enable flag */
                adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_CBR_SET(map_sw_info, enable, enable, dir);
            }
            break;

        case DIGI_CPB_CBR_TO_MAP:
        case DIGI_CPB_MAP_TO_MAP:
            if (dir == (digi_cpb_dir_t)DIGI_SWT_MAP_DIR_GET(map_sw_info))
            {
                if ((enable && DIGI_SWT_MAP_EN_GET(map_sw_info))
                     || (!enable && !DIGI_SWT_MAP_EN_GET(map_sw_info)))
                {
                    *match = TRUE;

                    /* update enable flag */
                    adpt_handle->traffic_info[index].map_sw_info = DIGI_SWT_MAP_SET(map_sw_info, enable, enable, dir);
                }
            }
            else
            {
                if (enable && DIGI_SWT_MAP_EN_GET(map_sw_info))
                {
                    /*******************************************************
                     * Before deleting CPB switch from CBR/MAPOTN to MAPOTN,
                     * it requires to delete the ODUk SW from MAPOTN to 
                     * MAPOTN/COREOTN/ILKN. 
                     ******************************************************/

                    /* delete ODUk SW from MAPOTN to MAPOTN/COREOTN/ILKN */
                    for (i = 0; i < DIGI_NUM_CHNL_MAX; i++)
                    {
                        if (!(adpt_handle->traffic_info[i].enable))
                        {
                            continue;
                        }

                        if ((DIGI_SWT_EN_GET(adpt_handle->traffic_info[i].oduk_src_index))
                             && (index == DIGI_SWT_ID_GET(adpt_handle->traffic_info[i].oduk_src_index)))
                        {
                            rc = adpt_dp_packet_switch(dev_id, 
                                                       DIGI_SWITCH_ODUK, 
                                                       chnl_id,
                                                       adpt_handle->traffic_info[i].chnl_id,
                                                       FALSE);
                            ADPT_RETURN_VERIFY(rc);
                        }
                    }

                    /* delete CPB switch from CBR/MAPOTN to MAPOTN */
                    rc = adpt_dp_cpb_chnl_enable(dev_id, chnl_id, 
                                                 (digi_cpb_dir_t)DIGI_SWT_MAP_DIR_GET(map_sw_info),
                                                 FALSE);
                    ADPT_RETURN_VERIFY(rc);
                }

                if (!enable)
                {
                    *match = TRUE;
                }                
            }
            break;

        default:
            ADPT_ERROR("Wrong direction type[%d].\r\n", dir);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cpb_chnl_enable_cmp */


/*******************************************************************************
* adpt_packet_switch_cmp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compares the input unidirectional switch with the Adapter 
*   context if they are the same. If not, the old packet switch might be deleted.
*   As to the ODUk channel, check if the payload type is DIGI_ODU_UNCHANNELIZED. 
*   If not, this ODUk channel will be forced to DIGI_ODU_UNCHANNELIZED. Meanwhile,
*   some daughter channels of this ODUk might be cleaned. 
*   
*   Take OTU2 <> ODU2 <> 4xODU0 as an example. If an ODUk SW loopback is enabled
*   for this ODU2, those 4xODU0 will be deleted to complete the operation of OTU2 
*   <> ODU2 <> ODUk SW. 
*
* INPUTS:
*   dev_id             - device identity.
*   switch_mode        - switch mode: ODUk switch, CPB switch.
*   src_chnl_id        - source channel identity.
*   dest_chnl_id       - destination channel identity.
*   enable             - TRUE: enable cross connection.
*                        FALSE: disable cross connection.
*
* OUTPUTS:
*   *match             - TRUE: the context has the same configuration.
*                        FALSE: the context has no this configuration. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR adpt_packet_switch_cmp(UINT32 dev_id,
                                      digi_switch_mode_t switch_mode,
                                      UINT32 src_chnl_id,
                                      UINT32 dest_chnl_id,
                                      BOOL enable,
                                      BOOL *match)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 src_index = 0;
    UINT32 dest_index = 0;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_oduksw_stream_t oduksw_stream = DIGI_ODUKSW_STREAM_CBR_ODU0;
    digi_oduksw_stream_t oduksw_stream1 = DIGI_ODUKSW_STREAM_CBR_ODU0;
    DOUBLE oduflex_rate = 0;
    DOUBLE oduflex_rate1 = 0;
    UINT32 ts_number = 0; 
    UINT32 ts_number1 = 0; 
    BOOL cpb_map_match = FALSE;
    BOOL cpb_cbr_match = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id,
               switch_mode, src_chnl_id, dest_chnl_id, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(match);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* default */
    *match = FALSE;

    /* get the index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, src_chnl_id, &src_index);
    rc |= adpt_chnl_index_get(adpt_handle, dest_chnl_id, &dest_index);
    if (PMC_SUCCESS != rc)
    {
        if (DIGI_INIT_RESTART == adpt_handle->init_state)
        {
            *match = TRUE;
            PMC_RETURN(PMC_SUCCESS);
        }
        else
        {
            PMC_RETURN(rc);
        }
    }

    if (!enable)
    {
        /* ODUk SW */
        if (!(DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].oduk_src_index))
             && ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_ODUK == switch_mode)))
        {
            *match = TRUE;
        }

        /* CPB */
        if (!(DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].cpb_src_index))
             && (DIGI_SWITCH_CPB == switch_mode))
        {
            *match = TRUE;
        }

        PMC_RETURN(rc);
    }

    /* check resource collision of the client mapping */
    if ((DIGI_SWITCH_ODUK_CPB == switch_mode)
        && (((NULL != adpt_handle->traffic_info[src_index].enet_handle)
        && !(adpt_handle->traffic_info[src_index].map_enable))
        || ((NULL != adpt_handle->traffic_info[src_index].cbr_handle)
        && !(adpt_handle->traffic_info[src_index].map_enable))
        || ((NULL != adpt_handle->traffic_info[dest_index].enet_handle)
        && !(adpt_handle->traffic_info[dest_index].map_enable))
        || ((NULL != adpt_handle->traffic_info[dest_index].cbr_handle)
        && !(adpt_handle->traffic_info[dest_index].map_enable))))
    {
        *match = TRUE;
        PMC_RETURN(rc);
    }

    /* check source */
    if (NULL != adpt_handle->traffic_info[src_index].odu_handle)
    {
        if (DIGI_ODU_NULL == adpt_handle->traffic_info[src_index].payload_type)
        {
            rc = adpt_otn_null_sig_set(dev_id, src_chnl_id, FALSE);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[src_index].payload_type)
        {
            rc = adpt_dp_chnl_del(dev_id, src_index, DIGI_DP_RX_TX, FALSE);
        }
        ADPT_RETURN_VERIFY(rc);
    }

    /* check destination */
    if (NULL != adpt_handle->traffic_info[dest_index].odu_handle)
    {
        if (DIGI_ODU_NULL == adpt_handle->traffic_info[dest_index].payload_type)
        {
            rc = adpt_otn_null_sig_set(dev_id, dest_chnl_id, FALSE);
            ADPT_RETURN_VERIFY(rc);
        }
        else if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[dest_index].payload_type)
        {
            rc = adpt_dp_chnl_del(dev_id, dest_index, DIGI_DP_RX_TX, FALSE);
        }
        ADPT_RETURN_VERIFY(rc);
    }

    /* CPB switch to MAPOTN: step 1 -- comparison */
    if (DIGI_SWITCH_ODUK_CPB == switch_mode)
    {
        if (adpt_handle->traffic_info[dest_index].map_enable)
        {
            rc = adpt_cpb_chnl_enable_cmp(dev_id, dest_chnl_id, 
                                          DIGI_CPB_MAP_TO_CBR, 
                                          enable, &cpb_cbr_match);
            ADPT_RETURN_VERIFY(rc);
        }

        if (adpt_handle->traffic_info[src_index].map_enable)
        {
            rc = adpt_cpb_chnl_enable_cmp(dev_id, src_chnl_id, 
                                          DIGI_CPB_CBR_TO_MAP, 
                                          enable, &cpb_map_match);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    /* ODUk SW */
    if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].oduk_src_index))
    {
        /* source cross connection */
        src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[dest_index].oduk_src_index);

        if ((adpt_handle->traffic_info[src_index].chnl_id == src_chnl_id)
            && ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_ODUK == switch_mode)))
        {
            *match = TRUE;

            /* update enable flag */
            adpt_handle->traffic_info[dest_index].oduk_src_index = DIGI_SWT_SET(enable, enable, src_index);
        }
        else
        {
            rc = adpt_dp_switch_del(dev_id, dest_index, DIGI_SWITCH_ODUK, DIGI_DP_RX);
            ADPT_RETURN_VERIFY(rc);
        }
    } 

    /* CPB */
    if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].cpb_src_index))
    {
        /* source cross connection */
        src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[dest_index].cpb_src_index);

        if ((adpt_handle->traffic_info[src_index].chnl_id == src_chnl_id)
            && (DIGI_SWITCH_CPB == switch_mode))
        {
            *match = TRUE;

            /* update enable flag */
            adpt_handle->traffic_info[dest_index].cpb_src_index = DIGI_SWT_SET(enable, enable, src_index);
        }
        else
        {
            rc = adpt_dp_packet_switch(dev_id, 
                                       DIGI_SWITCH_CPB, 
                                       adpt_handle->traffic_info[src_index].chnl_id,
                                       dest_chnl_id,
                                       FALSE);
            ADPT_RETURN_VERIFY(rc);
        }
    } 

    /*************************************************************************
     * Check ODUk SW parameters for the multicast/broadcast ODUk SW. If the 
     * parameters are different, these ODUk SW should be deleted first. For 
     * example, there is an ODUk SW from ILKN to ODU0. When creating an 
     * ODUk SW from this ILKN channel to ODUflex(1 TS), the ODUk SW from 
     * ILKN to ODU0 should be deleted first as the rate between them is 
     * different although they have the same one TS.
     *************************************************************************/
    if (!(*match) && ((DIGI_SWITCH_ODUK_CPB == switch_mode) || (DIGI_SWITCH_ODUK == switch_mode)))
    {
        rc = adpt_oduksw_para_get(dev_id, src_chnl_id, dest_chnl_id,
                                  &oduksw_stream, &oduflex_rate, &ts_number);
        ADPT_RETURN_VERIFY(rc);

        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            if (!(adpt_handle->traffic_info[index].enable) || (index == dest_index))
            {
                continue;
            }

            if (DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].oduk_src_index))
            {
                /* source cross connection */
                src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[index].oduk_src_index);

                if (adpt_handle->traffic_info[src_index].chnl_id == src_chnl_id)
                {
                    rc = adpt_oduksw_para_get(dev_id, 
                                              adpt_handle->traffic_info[src_index].chnl_id, 
                                              adpt_handle->traffic_info[index].chnl_id,
                                              &oduksw_stream1, &oduflex_rate1, &ts_number1);
                    ADPT_RETURN_VERIFY(rc);

                    if ((oduksw_stream == oduksw_stream1)
                        && ((DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX != oduksw_stream) 
                        || ((ts_number == ts_number1) 
                        && (DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX == oduksw_stream)))
                        && ((DIGI_ODUKSW_STREAM_CBR_ODUFLEX != oduksw_stream)
                        || (PMC_COMPARE_DOUBLE(oduflex_rate, oduflex_rate1, 0.0001) 
                        && (DIGI_ODUKSW_STREAM_CBR_ODUFLEX == oduksw_stream))))
                    {
                        /* break as ODUk SW parameters are the same */
                        break;
                    }
                    else
                    {
                        rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_ODUK, DIGI_DP_RX);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }
            }
        }
    }

    /* CPB switch to MAPOTN: step 2 -- CPB switch enable */
    if (DIGI_SWITCH_ODUK_CPB == switch_mode)
    {
        if (adpt_handle->traffic_info[dest_index].map_enable)
        {
            if (*match && !cpb_cbr_match)
            {
                rc = adpt_dp_cpb_chnl_enable(dev_id, dest_chnl_id, 
                                             DIGI_CPB_MAP_TO_CBR, enable);
                ADPT_RETURN_VERIFY(rc);
            }
        }

        if (adpt_handle->traffic_info[src_index].map_enable)
        {
            if (*match && !cpb_map_match)
            {
                rc = adpt_dp_cpb_chnl_enable(dev_id, src_chnl_id, 
                                             DIGI_CPB_CBR_TO_MAP, enable);
                ADPT_RETURN_VERIFY(rc);
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_packet_switch_cmp */


/*
** Public Functions
*/
/*******************************************************************************
* adpt_traffic_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures traffic mode in line/system side. It may include
*   OTU traffic, Ethernet traffic, CBR traffic and interlaken part. It just
*   defines port mode together with serdes information.
*
*   The following block may be involved.
*   OTU:  Serdes, LINEOTN, Ho ODU of COREOTN;
*   CBR:  Serdes, CBRC;
*   ETH:  Serdes, ENET(LINE), ENET(SYS);
*   INLK: Interlaken;
*
* INPUTS:
*   dev_id             - device identity.
*
*                        31 30 ... 25 24  23 22 ... 17 16  15 14 13 ... 2 1 0
*                        _______________  _______________  __________________
*                         slot_id[31:24]  digi_dev[23:16]    card_id[15:0]       
*
*   chnl_id            - channel identity.
*
*                        31 30 29 ... 25 24  23 22 21 20 ... 5 4 3 2 1 0
*                        __________________  ___________________________
*                        serdes_port[31:24]      tributary_port[23:0]
*
*                        serdes_port starts from 0 on either the line side or the 
*                        system side.
*                        Usually, tributary_port is used for the ODUk resource 
*                        and ILKN. For others, it can be zero.
*                        In addition, the chnl_id shouldn't be DIGI_DAFAULT_CHNL_ID
*                        or DIGI_SWITCH_CHNL_ID. 
*   traffic_mode       - traffic mode.
*   fec_type           - FEC type. Only available under OTU type.
*   ilkn_info          - ILKN channel information. Only available under ILKN type.
*
*                        31 30 ... 25 24  23 22 ... 9 8  7 6 5 ... 2 1 0
*                        ________________ ______________ _______________
*                        enet_pmon[31:24] chnl_num[23:8]  chnl_rate[7:0]
*
*                        enet_pmon is used for ENET <> CPB <> ILKN datapath.
*                        chnl_num is used for the MPMA/MPMO of interlaken.
*                        valid range: 0 ~ 191.
*                        chnl_rate is specified as an integer fraction of 96. 
*                        Used to allocate scheduling for signal transferred 
*                        towards backplane. The following must be true.
*                        chnl_rate/96 >= (signal_rate Gpbs)/(120 Gbps)
*   enable             - TRUE: enable traffic mode configuration.
*                        FALSE: disable traffic mode configuration.
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
*   1. Once the traffic mode is disabled, the S16 SerDes loopback will be also 
*   disabled accordingly if it is enabled.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_traffic_mode_cfg(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_traffic_mode_t traffic_mode,
                                   digi_otn_server_fec_t fec_type,
                                   UINT32 ilkn_info,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL cfg_match = FALSE;
    PMC_ENTRY();


    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* run the idempotent operation for datapath restoration */
    if (DIGI_INIT_RESTART == adpt_handle->init_state)
    {
        rc = adpt_traffic_mode_cmp(dev_id, chnl_id, traffic_mode, fec_type, 
                                   ilkn_info, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_rst_traffic_mode_cfg(dev_id, chnl_id, traffic_mode, 
                                       fec_type, ilkn_info, enable);
        ADPT_RETURN_VERIFY(rc);
    }
    /* run the idempotent operation in the normal case */
    else if (DIGI_DP_IDEMPOTENT == adpt_handle->dp_mode)
    {
        rc = adpt_traffic_mode_cmp(dev_id, chnl_id, traffic_mode, fec_type, 
                                   ilkn_info, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);
    }

    if (!cfg_match)
    {
        rc = adpt_dp_traffic_mode_cfg(dev_id, chnl_id, traffic_mode, 
                                      fec_type, ilkn_info, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_traffic_mode_cfg */


/*******************************************************************************
* adpt_traffic_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the traffic mode information from DIGI Adapter memory.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *traffic_mode      - traffic mode.
*   *enable            - TRUE: enable traffic mode configuration.
*                        FALSE: disable traffic mode configuration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_traffic_mode_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_traffic_mode_t *traffic_mode,
                                   BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p][%p].\r\n", dev_id,
               chnl_id, traffic_mode, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(traffic_mode);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);

    if (PMC_SUCCESS == rc)
    {
        *enable = adpt_handle->traffic_info[index].enable ? TRUE : FALSE;
        *traffic_mode = adpt_handle->traffic_info[index].traffic_mode;
    }
    else
    {
        *enable = FALSE;
        *traffic_mode = LAST_DIGI_TRAFFIC;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* adpt_traffic_mode_get */


/*******************************************************************************
* adpt_odu_mux_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure ODUk multiplexing/demultiplexing. The
*   maximum datapath for ODUk is two stages altogeter. before calling this
*   function, adpt_traffic_mode_cfg interface should be called beforehand.
*
* INPUTS:
*   dev_id             - device identity.
*   ho_chnl_id         - parent channel identity.
*   lo_chnl_id         - self channel identity.
*   odu_mux            - ODUk mux/demux related information.
*                        The oduflex_rate with bps unit is only available for 
*                        ODUflex(CBR). It should be more than 1000bps.
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
PUBLIC PMC_ERROR adpt_odu_mux_cfg(UINT32 dev_id,
                                 UINT32 ho_chnl_id,
                                 UINT32 lo_chnl_id,
                                 digi_odu_mux_t odu_mux)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_dp_dir_t cfg_match = DIGI_DP_NONE;
    PMC_ENTRY();


    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* run the idempotent operation for datapath restoration */
    if (DIGI_INIT_RESTART == adpt_handle->init_state)
    {
        rc = adpt_odu_mux_cmp(dev_id, ho_chnl_id, lo_chnl_id, odu_mux, &cfg_match);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_rst_odu_mux_cfg(dev_id, ho_chnl_id, lo_chnl_id, odu_mux);
        ADPT_RETURN_VERIFY(rc);
    }
    /* run the idempotent operation in the normal case */
    else if (DIGI_DP_IDEMPOTENT == adpt_handle->dp_mode)
    {
        rc = adpt_odu_mux_cmp(dev_id, ho_chnl_id, lo_chnl_id, odu_mux, &cfg_match);
        ADPT_RETURN_VERIFY(rc);
    }

    if (DIGI_DP_NONE != ((digi_dp_dir_t)odu_mux.otn_dir & ~cfg_match))
    {
        odu_mux.otn_dir = (digi_otn_dir_t)(odu_mux.otn_dir & (~cfg_match));

        rc = adpt_dp_odu_mux_cfg(dev_id, ho_chnl_id, lo_chnl_id, odu_mux);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_odu_mux_cfg */


/*******************************************************************************
* adpt_odu_mux_del
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deletes ODUk TP and those daughter ODUk TP belongs to this ODUk TP. However,
*   for HO ODUk TP, it will not be deleted.
*
*   For example: OTU3 <> ODU3 <> ODU2 <> 4xODU1
*   1. When deleting ODU2 TP, 4xODU1 TPs will also be deleted.
*   2. When deleting ODU3 TP, ODU2 and 4xODU1 TPs will be deleted while ODU3 TP
*   will be kept the same as before.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
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
*   Please note the ODUk TP will be channelized when deleting all daughter ODUj 
*   TP one by one. For example, after deleting all 8xODU0 with this API for 
*   ODU2 <> 8xODU0, the ODU2 will be channelized and can't be connected to 
*   ODUk SW directly. It can be transferred to the unchannelized ODU2 through 
*   adpt_odu_ts_type_set().
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_mux_del(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 lo_index = 0;
    UINT32 ho_index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_otn_odu_chnl_prov_mode_t prov_mode = DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT;
    BOOL sub_tp = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, chnl_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, chnl_id, &lo_index);
    ADPT_RETURN_VERIFY(rc);    

    /* search daughter TP */
    for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
    {
        if ((index != lo_index) 
            && adpt_handle->traffic_info[index].enable
            && (adpt_handle->traffic_info[index].ho_chnl_id == chnl_id))
        {
            rc = adpt_odu_mux_del(dev_id, adpt_handle->traffic_info[index].chnl_id);
            ADPT_RETURN_VERIFY(rc);

            sub_tp = TRUE;
        }    
    }

    rc = adpt_prov_mode_get(dev_id, &prov_mode);
    ADPT_RETURN_VERIFY(rc);
    if ((DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode) && sub_tp)
    {
        rc = adpt_bulk_cfg_end(dev_id, chnl_id, DIGI_OTN_BULK_DEPROV_END);
        ADPT_RETURN_VERIFY(rc);
    }

    /* check if chnl_id belongs HO ODUk */
    if ((DIGI_DEFAULT_CHNL_ID == adpt_handle->traffic_info[lo_index].ho_chnl_id)
         && (NULL != adpt_handle->traffic_info[lo_index].otu_handle))
    {
        if (DIGI_ODU_UNCHANNELIZED != adpt_handle->traffic_info[lo_index].payload_type)
        {
            /* workaround in the bulk provision mode */
            /* switch to the non-bulk provision mode before the digi_otn_payload_type_transition() */
            if (DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode)
            {
                rc = adpt_prov_mode_set(dev_id, DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT);
                ADPT_RETURN_VERIFY(rc);
            }

            /* payload type transition */
            rc = digi_otn_payload_type_transition(adpt_handle->digi_handle,
                                                  adpt_handle->traffic_info[lo_index].odu_handle,
                                                  DIGI_ODU_UNCHANNELIZED);
            ADPT_RETURN_VERIFY(rc);

            /* reset TCM pool ID */
            rc = adpt_tcm_pool_reset(dev_id, chnl_id);
            ADPT_RETURN_VERIFY(rc);

            /* payload type */                              
            adpt_handle->traffic_info[lo_index].payload_type = DIGI_ODU_UNCHANNELIZED;

            /* workaround in the bulk provision mode */
            /* switch to the bulk provision mode after the digi_otn_payload_type_transition() */
            if (DIGI_OTN_ODU_CHNL_PROV_ALL == prov_mode)
            {
                rc = adpt_prov_mode_set(dev_id, prov_mode);
                ADPT_RETURN_VERIFY(rc);
            }
        }
        PMC_RETURN(rc);
    }

    /* get the parent index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, adpt_handle->traffic_info[lo_index].ho_chnl_id, &ho_index);
    ADPT_RETURN_VERIFY(rc);    

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[lo_index].odu_handle);

    /* tributary port deprovision */
    rc = digi_otn_server_lo_deprov_asym(adpt_handle->digi_handle,
                                        &(adpt_handle->traffic_info[lo_index].odu_handle),
                                        &(adpt_handle->traffic_info[ho_index].odu_handle),
                                        adpt_otn_dir_cvt(adpt_handle->traffic_info[lo_index].otn_dir));
    ADPT_RETURN_VERIFY(rc);

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        /* destroy this ODUk container */
        rc = adpt_chnl_index_destroy(adpt_handle, chnl_id, lo_index);
    }

    PMC_RETURN(rc);
} /* adpt_odu_mux_del */


/*******************************************************************************
* adpt_odu_mux_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the ODUk multiplexing/demultiplexing information from DIGI Adapter 
*   memory.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *odu_mux           - ODUk mux/demux related information.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_odu_mux_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_odu_mux_t *odu_mux)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, odu_mux);

    /* check parameter validation */
    ADPT_PARA_VERIFY(odu_mux);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);

    if (PMC_SUCCESS == rc)
    {
        odu_mux->mapping_type  = adpt_handle->traffic_info[index].mapping_type;
        odu_mux->oduk_type     = adpt_handle->traffic_info[index].oduk_type;
        odu_mux->payload_type  = adpt_handle->traffic_info[index].payload_type;
        odu_mux->otn_dir       = adpt_handle->traffic_info[index].otn_dir;
        odu_mux->tp_enable     = adpt_handle->traffic_info[index].enable ? TRUE : FALSE;
        odu_mux->tx_ts_mask[0] = adpt_handle->traffic_info[index].tx_ts_mask[0];
        odu_mux->tx_ts_mask[1] = adpt_handle->traffic_info[index].tx_ts_mask[1];
        odu_mux->tx_ts_mask[2] = adpt_handle->traffic_info[index].tx_ts_mask[2];
        odu_mux->rx_ts_mask[0] = adpt_handle->traffic_info[index].rx_ts_mask[0];
        odu_mux->rx_ts_mask[1] = adpt_handle->traffic_info[index].rx_ts_mask[1];
        odu_mux->rx_ts_mask[2] = adpt_handle->traffic_info[index].rx_ts_mask[2];
        odu_mux->oduflex_rate  = adpt_handle->traffic_info[index].oduflex_rate;
    }
    else
    {
        odu_mux->mapping_type  = LAST_DIGI_OTN_MAP;
        odu_mux->oduk_type     = LAST_DIGI_ODUK;
        odu_mux->payload_type  = LAST_DIGI_ODU_PAYLOAD_FORMAT;
        odu_mux->otn_dir       = LAST_DIGI_OTN_DIR;
        odu_mux->tp_enable     = FALSE;
        odu_mux->tx_ts_mask[0] = 0;
        odu_mux->tx_ts_mask[1] = 0;
        odu_mux->tx_ts_mask[2] = 0;
        odu_mux->rx_ts_mask[0] = 0;
        odu_mux->rx_ts_mask[1] = 0;
        odu_mux->rx_ts_mask[2] = 0;
        odu_mux->oduflex_rate  = 0;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* adpt_odu_mux_get */


/*******************************************************************************
* adpt_client_map_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures client end point and then maps the client to its
*   end point.
*   In Transponder and Muxponder applications,
*   this function configures ENET or CBRC client and then maps it to CPB and
*   ODUk switch.
*
*   In Line Card applications,
*   this function configures back plane (e.g. Interlaken) channel, and then
*   maps it to CPB and ODUk switch.
*
*   In Tributary Card applications,
*   this function configures ENET or CBRC client and then maps it to CPB.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mapping_mode       - client mapping mode.
*   ts_number          - tributary slot number. Only available for ODUflex.
*   enable             - TRUE: enable the client mapping configuration.
*                        FALSE: disable the client mapping confguration.
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
PUBLIC PMC_ERROR adpt_client_map_cfg(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_mapotn_mode_t mapping_mode,
                                  UINT32 ts_number,
                                  BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL cfg_match = FALSE;
    PMC_ENTRY();


    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* run the idempotent operation for datapath restoration */
    if (DIGI_INIT_RESTART == adpt_handle->init_state)
    {
        rc = adpt_client_map_cmp(dev_id, chnl_id, mapping_mode, 
                                 ts_number, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_rst_client_map_cfg(dev_id, chnl_id, mapping_mode, ts_number, enable);
        ADPT_RETURN_VERIFY(rc);
    }
    /* run the idempotent operation in the normal case */
    else if (DIGI_DP_IDEMPOTENT == adpt_handle->dp_mode)
    {
        rc = adpt_client_map_cmp(dev_id, chnl_id, mapping_mode, 
                                 ts_number, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);
    }

    if (!cfg_match)
    {
        rc = adpt_dp_client_map_cfg(dev_id, chnl_id, mapping_mode, ts_number, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_client_map_cfg */


/*******************************************************************************
* adpt_client_map_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the client mapping information in DIGI Adapter memory.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *mapping_mode      - client mapping mode.
*   *ts_number         - tributary slot number. Only available for ODUflex.
*   *enable            - TRUE: enable the client mapping configuration.
*                        FALSE: disable the client mapping confguration.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_client_map_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_mapotn_mode_t *mapping_mode,
                                  UINT32 *ts_number,
                                  BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p][%p][%p].\r\n", dev_id,
               chnl_id, mapping_mode, ts_number, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ts_number);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);

    if (PMC_SUCCESS == rc)
    {
        *mapping_mode = adpt_handle->traffic_info[index].mapping_mode;
        *ts_number    = adpt_handle->traffic_info[index].ts_number;
        *enable       = adpt_handle->traffic_info[index].map_enable ? TRUE : FALSE;
    }
    else
    {
        *mapping_mode = LAST_DIGI_MAPOTN;
        *ts_number    = 0;
        *enable       = FALSE;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* adpt_client_map_get */


/*******************************************************************************
* adpt_packet_switch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is intended to configure unidirectional switch function in 
*   DIGI device. There are two switch modes altogether: ODUk switch and CPB 
*   switch. 
*
* INPUTS:
*   dev_id             - device identity.
*   switch_mode        - switch mode: ODUk switch, CPB switch.
*   src_chnl_id        - source channel identity.
*   dest_chnl_id       - destination channel identity.
*   enable             - TRUE: enable cross connection.
*                        FALSE: disable cross connection.
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
PUBLIC PMC_ERROR adpt_packet_switch(UINT32 dev_id,
                                 digi_switch_mode_t switch_mode,
                                 UINT32 src_chnl_id,
                                 UINT32 dest_chnl_id,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL cfg_match = FALSE;
    PMC_ENTRY();


    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* run the idempotent operation for datapath restoration */
    if (DIGI_INIT_RESTART == adpt_handle->init_state)
    {
        rc = adpt_packet_switch_cmp(dev_id, switch_mode, src_chnl_id, 
                                    dest_chnl_id, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_rst_packet_switch(dev_id, switch_mode, src_chnl_id, dest_chnl_id, enable);
        ADPT_RETURN_VERIFY(rc);
    }
    /* run the idempotent operation in the normal case */
    else if (DIGI_DP_IDEMPOTENT == adpt_handle->dp_mode)
    {
        rc = adpt_packet_switch_cmp(dev_id, switch_mode, src_chnl_id, 
                                    dest_chnl_id, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);
    }

    if (!cfg_match)
    {
        rc = adpt_dp_packet_switch(dev_id, switch_mode, src_chnl_id, dest_chnl_id, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_packet_switch */


/*******************************************************************************
* adpt_packet_switch_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves packet switch information in DIGI device. There are two switch 
*   modes altogether: ODUk switch and CPB switch. 
*
* INPUTS:
*   dev_id             - device identity.
*   switch_mode        - switch mode: ODUk switch, CPB switch.
*   dest_chnl_id       - destination channel identity.
*
* OUTPUTS:
*   *src_chnl_id       - source channel identity.
*   *enable            - TRUE: enable cross connection.
*                        FALSE: disable cross connection.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_packet_switch_get(UINT32 dev_id,
                                     digi_switch_mode_t switch_mode,
                                     UINT32 dest_chnl_id,
                                     UINT32 *src_chnl_id,
                                     BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 src_index = 0;
    UINT32 dest_index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p][%p].\r\n", dev_id,
               switch_mode, dest_chnl_id, src_chnl_id, enable);

    /* parameter verification */
    ADPT_PARA_VERIFY(src_chnl_id);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc |= adpt_chnl_index_get(adpt_handle, dest_chnl_id, &dest_index);

    if (PMC_SUCCESS == rc)
    {
        if ((DIGI_SWITCH_ODUK == switch_mode) || (DIGI_SWITCH_ODUK_CPB == switch_mode))
        {
            *enable = DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].oduk_src_index);
            src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[dest_index].oduk_src_index);
        }
        else if (DIGI_SWITCH_CPB == switch_mode)
        {
            *enable = DIGI_SWT_EN_GET(adpt_handle->traffic_info[dest_index].cpb_src_index);
            src_index = DIGI_SWT_ID_GET(adpt_handle->traffic_info[dest_index].cpb_src_index);
        }
        else
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }

        if (src_index < DIGI_NUM_CHNL_MAX)
        {
            *src_chnl_id = adpt_handle->traffic_info[src_index].chnl_id;
        }
        else
        {
            PMC_RETURN(PMC_ERR_FAIL);
        }
    }
    else
    {
        *src_chnl_id = 0;
        *enable = FALSE;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* adpt_packet_switch_get */


/*******************************************************************************
* adpt_cpb_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Acitvates or deactivates CPB switch which goes through MAPOTN.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - direction to MAPOTN.
*   enable             - TRUE: acticate CPB switch.
*                        FALSE: deactivate CPB switch.
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
PUBLIC PMC_ERROR adpt_cpb_chnl_enable(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_cpb_dir_t dir,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL cfg_match = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, dir, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* run the idempotent operation for datapath restoration */
    if (DIGI_INIT_RESTART == adpt_handle->init_state)
    {
        rc = adpt_cpb_chnl_enable_cmp(dev_id, chnl_id, dir, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);

        rc = adpt_rst_cpb_chnl_enable(dev_id, chnl_id, dir, enable);
        ADPT_RETURN_VERIFY(rc);
    }
    /* run the idempotent operation in the normal case */
    else if (DIGI_DP_IDEMPOTENT == adpt_handle->dp_mode)
    {
        rc = adpt_cpb_chnl_enable_cmp(dev_id, chnl_id, dir, enable, &cfg_match);
        ADPT_RETURN_VERIFY(rc);
    }

    if (!cfg_match)
    {
        rc = adpt_dp_cpb_chnl_enable(dev_id, chnl_id, dir, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_cpb_chnl_enable */


/*******************************************************************************
* adpt_cpb_chnl_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves status of the CPB switch in a channel which goes through MAPOTN.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - direction to MAPOTN.
*
* OUTPUTS:
*   *enable            - TRUE: acticate CPB switch.
*                        FALSE: deactivate CPB switch.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpb_chnl_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_cpb_dir_t dir,
                                   BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT8 map_sw_info = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    map_sw_info = adpt_handle->traffic_info[index].map_sw_info;
    switch (dir)
    {
        case DIGI_CPB_MAP_TO_CBR:
            *enable = DIGI_SWT_CBR_EN_GET(map_sw_info);
            break;

        case DIGI_CPB_CBR_TO_MAP:
        case DIGI_CPB_MAP_TO_MAP:
            if (dir == (digi_cpb_dir_t)DIGI_SWT_MAP_DIR_GET(map_sw_info))
            {
                *enable = DIGI_SWT_MAP_EN_GET(map_sw_info);
            }
            else
            {
                *enable = FALSE;
            }
            break;

        default:
            ADPT_ERROR("Wrong direction type[%d].\r\n", dir);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_cpb_chnl_en_get */


/*******************************************************************************
* adpt_prov_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the ODUk channel provisioning mode, as well as ODUk SW. 
*   It is used to determine if LO ODU channel will be provisioned one at a time 
*   or all LO ODU channels will be provisioned into the HO ODU at once. 
*
*   The target of this feature is to optimize the processing time of ODUk channel 
*   and ODUk SW deprovision/provision. Therefore, it is usually used for the 1+1 
*   and 1:N SNC group protection, or other protections. There are three operation  
*   modes listed below.
*
*   DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT: It's the default setting for a normal 
*   operation with only one LO ODU channel into the HO ODU at a time.
*
*   DIGI_OTN_ODU_CHNL_PROV_ALL: Allows for multiple LO ODU channels into the HO ODU 
*   at the same time. It's usually used for the 1:N SNC group protection with ODUk and 
*   ODUk SW deprovision/provision. After protection completion, it should be switched
*   to the DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT mode for a normal operation.
*
*     1) Deprovision Procedure: 
*     adpt_prov_mode_set(..., DIGI_OTN_ODU_CHNL_PROV_ALL); # if necessary
*     adpt_packet_switch(..., FALSE); # multiple times
*     adpt_odu_mux_cfg(...); # multiple times, odu_mux.tp_enable = FALSE
*     adpt_bulk_cfg_end(..., DIGI_OTN_BULK_DEPROV_END);
*
*     2) Provision Procedure:
*     adpt_prov_mode_set(dev_id, DIGI_OTN_ODU_CHNL_PROV_ALL); # if necessary
*     adpt_odu_mux_cfg(...); # multiple times, odu_mux.tp_enable = TRUE
*     adpt_packet_switch(..., TRUE); # multiple times
*     adpt_bulk_cfg_end(..., DIGI_OTN_BULK_PROV_END);
*
*   DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK: Allows for multiple ODUk SW connections 
*   deprovsion/provision at the same time. It's usually used for 1+1 or 1:N SNC 
*   protection with ODUk SW deprovision/provision. After protection completion, it  
*   should be switched to the DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT mode for a normal 
*   operation.
*
*     1) Deprovision Procedure: 
*     adpt_prov_mode_set(..., DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK); # if necessary
*     adpt_packet_switch(..., FALSE); # multiple times
*     adpt_bulk_cfg_end(..., DIGI_OTN_BULK_DEPROV_END);
*
*     2) Provision Procedure:
*     adpt_prov_mode_set(..., DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK); # if necessary
*     adpt_packet_switch(..., TRUE); # multiple times
*     adpt_bulk_cfg_end(..., DIGI_OTN_BULK_PROV_END);
* 
* INPUTS:
*   dev_id             - device identity.
*   mode               - ODUk channel provisioning mode. 
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
*   1. Please note the provision procedure can't be mixed with the deprovision 
*   procedure. That is to say, the provision procedure can only be executed 
*   after the deprovision procedure. Or the deprovision procedure can only be
*   executed after the provision procedure.
*
*   2. When deleting all ODUj from OTUk <> ODUk <> ODUj in the DIGI_OTN_ODU_CHNL_PROV_ALL 
*   mode, the ODUk will still be kept the original tributory slot type (DIGI_ODU_TS_1G25
*   or DIGI_ODU_TS_2G5). In this case, if it needs to be connected with ODUk SW, 
*   the provision mode should be changed to DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT 
*   and the adpt_odu_ts_type_set() is called to change its TS type as 
*   DIGI_ODU_UNCHANNELIZED.
*
*   3. This API isn't allowed during datapath restoration.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_prov_mode_set(UINT32 dev_id,
                                  digi_otn_odu_chnl_prov_mode_t mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* bulk prov/deprov isn't allowed during datapath restoration */
    if (DIGI_INIT_DONE != adpt_handle->init_state)
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* set mode */
    rc = digi_otn_odu_chnl_prov_mode_set(adpt_handle->digi_handle, mode);

    PMC_RETURN(rc);
} /* adpt_prov_mode_set */


/*******************************************************************************
* adpt_prov_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the ODUk channel provisioning mode. This mode is used 
*   to determine if LO ODU channel will be provisioned one at a time or all LO ODU
*   channels will be provisioned into the HO ODU at once. 
* 
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *mode              - ODUk channel provisioning mode. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_prov_mode_get(UINT32 dev_id,
                                  digi_otn_odu_chnl_prov_mode_t *mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_otn_odu_chnl_prov_mode_get(adpt_handle->digi_handle, mode);

    PMC_RETURN(rc);
} /* adpt_prov_mode_get */


/*******************************************************************************
* adpt_bulk_cfg_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is to optimize the processing time of ODUk channel and ODUk SW 
*   deprovision/provision. It can be called only after the adpt_prov_mode_set()
*   has been configured as DIGI_OTN_ODU_CHNL_PROV_ALL or 
*   DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK. For more detail information, please
*   refer to the adpt_prov_mode_set() description.
*
*   Under the DIGI_OTN_ODU_CHNL_PROV_ALL mode, the provision procedure should be 
*   as follows. This also applies to the depovision procedure.
*   1) In one stage muxing datapath, this function needs to be called once after 
*      the last LO ODUk channel has been provisioned with the adpt_odu_mux_cfg().
*       - The chnl_id should be the HO ODU channel.
*   2) In two stage muxing datapath, this function needs to be called twice.
*      Once after the last MO channel has been provisioned.
*       - The chnl_id should be the HO ODU channel.
*      Once after the last LO channel has been provisioned.
*       - The chnl_id should be the MO ODU channel.
*
*   Under the DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK mode, only ODUk SW can be
*   provisioned or deprovisioned with the adpt_packet_switch(). The chnl_id used 
*   for the adpt_bulk_cfg_end() is the same as above.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   stage              - Enum for LO ODU channel bulk provision end stage. 
*                        See digi_otn_bulk_prov_stage_t.
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
*   Please use chnl_id as DIGI_DEFAULT_CHNL_ID to handle ODUk SW for those 
*   HO ODU/MAPOTN channels.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_bulk_cfg_end(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_otn_bulk_stage_t stage)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    digi_otn_odu_chnl_t* odu_chnl_ptr = NULL;
    digi_otn_odu_chnl_t** odu_chnl_pptr = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, stage);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_DEFAULT_CHNL_ID != chnl_id)
    {
        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
        ADPT_RETURN_VERIFY(rc);

        odu_chnl_ptr = adpt_handle->traffic_info[index].odu_handle;
        odu_chnl_pptr = &adpt_handle->traffic_info[index].odu_handle;
    }
    else
    {
        /* We want to pass NULL to digi_otn_server_lo_prov_end
         * and digi_otn_server_lo_deprov_end when chnl_id
         * is DIGI_DEFAULT_CHNL_ID */
        odu_chnl_ptr = NULL;
        odu_chnl_pptr = NULL;
    }

    switch (stage)
    {
        case DIGI_OTN_BULK_PROV_END:
            /* prov end */
            rc = digi_otn_server_lo_prov_end(adpt_handle->digi_handle, 
                                             odu_chnl_ptr);
            break;

        case DIGI_OTN_BULK_DEPROV_END:
            /* deprov end */ 
            rc = digi_otn_server_lo_deprov_end(adpt_handle->digi_handle, 
                                               odu_chnl_pptr);
            break;

        default:
            rc = PMC_ERR_INVALID_PARAMETERS;
            break;
    }

    PMC_RETURN(rc);
} /* adpt_bulk_cfg_end */


/*******************************************************************************
* adpt_dp_restore
* ______________________________________________________________________________
*
* DESCRIPTION:
*   During the whole crash / warm restart, NMS(Network Management System) can still 
*   be configured. Therefore, the new datapaths might be different from the old one 
*   before restart even that the chnl_id with the same datapath will be different 
*   between them. It requires the SW/HW system to be updated accordingly after 
*   restart. This API is used for datapath restoration after adpt_init() with the 
*   crash / warm restart. 
*
*   There are five kind of types between these two datapath configurations which 
*   need to be handled differently.
*
*   1. Unchanged datapaths: It shall guarantee those unchanged datapaths to be 
*      hitless during this whole process. 
*   2. Stale datapaths (i.e. negative resource): Datapaths not in the new 
*      configurations will be deleted one by one..
*   3. New datapaths: The datapaths having no resource issue will be added.
*   4. Matching conflicting datapaths: The conflicting datapath having no resource 
*      issue will be re-provisioned with the new datapath instantly.
*   5. Blocked conflicting datapaths (i.e. blocked resources): The conflicting 
*      datapaths having the resource blocking issue will be identified and 
*      provisioned after negative resource has been cleaned up. 
*
*      - Blocked resources are mainly related to the application for ILKN#1 <> 
*        ILKN#2. It will require ILKN run-time configuration APIs to be called 
*        after the completion of this procedure. That is, they need to be called 
*        after adpt_dp_restore(DIGI_RESTORE_END).
*
*   The following cases, but not limited to, are considered as conflicting datapaths.
*   1. Different ODUk type, for example, ODU2 vs ODUflex.
*   2. Different TS type, for example, TS_1G25 vs TS_2G5. 
*   3. Different Rx/Tx TS mask. 
*   4. Different ODUflex(CBR) rate.
*   5. Different client mapping mode. 
*   6. Different ILKN channel ID/rate. 
*   ...
*
* INPUTS:
*   dev_id             - device identity.
*   stage              - stage for datapath restoration after crash/warm restart.
*
*                        DIGI_RESTORE_START: Allocate a new context to save 
*                        datapath configurations.
*                        DIGI_RESTORE_END: Clean up stale datapaths which don't 
*                        exist in new configurations and apply blocked configurations
*                        which are mainly related to ILKN.
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
*   The datapath restoration procedure is as follows.
*
*   1. Start crash / warm initialization.
*      adpt_init(dev_id, base_addr, not DIGI_INIT_COLD);
*
*   2. Start datapath restoration by allocating a new context to save datapath
*      configurations. 
*      adpt_dp_restore(dev_id, DIGI_RESTORE_START);
*
*   3. Configure all the latest datapaths received from the control plane with 
*      the following datapath APIs. It will hitless update the matching  
*      configuration datapaths and identify and store blocked resources.
*      adpt_traffic_mode_cfg();
*      adpt_client_map_cfg();
*      adpt_odu_mux_cfg();
*      adpt_packet_switch();
*      adpt_cpb_chnl_enable(); <if necessary>
*
*   4. Clean up stale datapaths and apply blocked configurations. 
*      adpt_dp_restore(dev_id, DIGI_RESTORE_END);
*
*      - During this process, ILKN run-time configuration APIs are restricted 
*        while all other run-time API configuration calls may be made.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dp_restore(UINT32 dev_id, digi_dp_stage_t stage)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 j = 0;
    UINT32 index = 0;
    digi_dp_dir_t dp_add = DIGI_DP_NONE;
    digi_otn_odu_chnl_prov_mode_t prov_mode = DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, stage);

    if (stage >= LAST_DIGI_RESTORE)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    if (DIGI_RESTORE_START == stage)
    {
        if (DIGI_INIT_RESTART == adpt_handle->init_state)
        {
            PMC_RETURN(PMC_SUCCESS);
        }

        /* disable the bulk provision mode during datapath restoration */
        rc = adpt_prov_mode_get(dev_id, &prov_mode);
        ADPT_RETURN_VERIFY(rc);

        if (DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT != prov_mode)
        {
            rc = adpt_prov_mode_set(dev_id, DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT);
            ADPT_RETURN_VERIFY(rc);
        }

        /* allocate a new context */
        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            adpt_handle->restart_info[index] = (digi_traffic_info_t*)PMC_CALLOC(sizeof(digi_traffic_info_t));

            /* reset channel ID */
            adpt_handle->restart_info[index]->chnl_id    = DIGI_DEFAULT_CHNL_ID;
            adpt_handle->restart_info[index]->ho_chnl_id = DIGI_DEFAULT_CHNL_ID;
        }

        /* record initialization state */
        adpt_handle->init_state = DIGI_INIT_RESTART;
    }
    /* datapath restoration */
    else
    {
        if (DIGI_INIT_RESTART != adpt_handle->init_state)
        {
            PMC_RETURN(PMC_ERR_FAIL);
        }

        /* record initialization state as restoring */
        adpt_handle->init_state = DIGI_INIT_RESTORE;

        /* clean up negative configuration space except the map_handle */
        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            if (!(adpt_handle->traffic_info[index].enable))
            {
                continue;
            }

            /* handle the OTU/ENET/CBR/ODU channel */
            if (!(DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[index].enable)))
            {
                if ((NULL != adpt_handle->traffic_info[index].enet_handle)
                    || (NULL != adpt_handle->traffic_info[index].cbr_handle)
                    || (NULL != adpt_handle->traffic_info[index].otu_handle)
                    || (NULL != adpt_handle->traffic_info[index].ilkn_handle))
                {
                    rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                    ADPT_RETURN_VERIFY(rc);
                }
                else
                {
                    if (!(DIGI_RX_FLAG_GET(adpt_handle->traffic_info[index].enable))
                        && !(DIGI_TX_FLAG_GET(adpt_handle->traffic_info[index].enable))
                        && (adpt_handle->traffic_info[index].otn_dir & DIGI_OTN_DIR_RX_TX))
                    {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                    else if (!(DIGI_RX_FLAG_GET(adpt_handle->traffic_info[index].enable))
                             && (adpt_handle->traffic_info[index].otn_dir & DIGI_OTN_DIR_RX))
                    {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_RX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                    else if (!(DIGI_TX_FLAG_GET(adpt_handle->traffic_info[index].enable))
                             && (adpt_handle->traffic_info[index].otn_dir & DIGI_OTN_DIR_TX))
                    {
                        rc = adpt_dp_chnl_del(dev_id, index, DIGI_DP_TX, TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }
                }
            }

            /* handle ODUk SW/CPB */
            if ((DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].oduk_src_index))
                 && !(DIGI_SWT_FLAG_GET(adpt_handle->traffic_info[index].oduk_src_index)))
            {
                rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_ODUK, DIGI_DP_RX);
                ADPT_RETURN_VERIFY(rc);
            }

            if ((DIGI_SWT_EN_GET(adpt_handle->traffic_info[index].cpb_src_index))
                 && !(DIGI_SWT_FLAG_GET(adpt_handle->traffic_info[index].cpb_src_index)))
            {
                rc = adpt_dp_switch_del(dev_id, index, DIGI_SWITCH_CPB, DIGI_DP_RX);
                ADPT_RETURN_VERIFY(rc);
            }
        }

        /* clean up negative configuration space for the map_handle  */
        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            if (!(adpt_handle->traffic_info[index].enable))
            {
                continue;
            }

            /* handle CPB switch connected to MAPOTN */
            if ((DIGI_SWT_CBR_EN_GET(adpt_handle->traffic_info[index].map_sw_info))
                 && !(DIGI_SWT_CBR_FLAG_GET(adpt_handle->traffic_info[index].map_sw_info)))
            {
                rc = adpt_dp_cpb_chnl_enable(dev_id,
                                             adpt_handle->traffic_info[index].chnl_id,
                                             (digi_cpb_dir_t)DIGI_SWT_CBR_DIR_GET(adpt_handle->traffic_info[index].map_sw_info),
                                             FALSE);
                ADPT_RETURN_VERIFY(rc);
            }

            if ((DIGI_SWT_MAP_EN_GET(adpt_handle->traffic_info[index].map_sw_info))
                 && !(DIGI_SWT_MAP_FLAG_GET(adpt_handle->traffic_info[index].map_sw_info)))
            {
                rc = adpt_dp_cpb_chnl_enable(dev_id,
                                             adpt_handle->traffic_info[index].chnl_id,
                                             (digi_cpb_dir_t)DIGI_SWT_MAP_DIR_GET(adpt_handle->traffic_info[index].map_sw_info),
                                             FALSE);
                ADPT_RETURN_VERIFY(rc);
            }

            /* handle map_handle */
            if ((NULL != adpt_handle->traffic_info[index].map_handle)
                 && !(DIGI_RX_TX_FLAG_GET(adpt_handle->traffic_info[index].map_enable)))
            {
                rc = adpt_dp_client_map_cfg(dev_id, adpt_handle->traffic_info[index].chnl_id,
                                            adpt_handle->traffic_info[index].mapping_mode,
                                            0, FALSE);
                ADPT_RETURN_VERIFY(rc);
            }
        }

        /* handle blocked ILKN resources */
        /* add new datapaths' channels in new configurations */
        for (j = 0; j < DIGI_NUM_CHNL_MAX; j++)
        {
            if (!(adpt_handle->restart_info[j]->enable))
            {
                continue;
            }

            dp_add = DIGI_DP_RX_TX;
            for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
            {
                if (!(adpt_handle->traffic_info[index].enable))
                {
                    continue;
                }

                if (adpt_handle->traffic_info[index].chnl_id == adpt_handle->restart_info[j]->chnl_id)
                {
                    dp_add = DIGI_DP_NONE;

                    /* configure client mapping */
                    if ((adpt_handle->restart_info[j]->map_enable)
                        && !(adpt_handle->traffic_info[index].map_enable))
                    {
                        rc = adpt_dp_client_map_cfg(dev_id, 
                                                    adpt_handle->restart_info[j]->chnl_id,
                                                    adpt_handle->restart_info[j]->mapping_mode,
                                                    adpt_handle->restart_info[j]->ts_number, 
                                                    TRUE);
                        ADPT_RETURN_VERIFY(rc);
                    }

                    /* find the datapath channel */
                    break;
                }
            }

            if (DIGI_DP_NONE != dp_add)
            {
                rc = adpt_dp_chnl_add(dev_id, j, dp_add);
                ADPT_RETURN_VERIFY(rc);
            }
        }

        /* add new datapaths' ODUk SW / CPB in new configurations. */
        for (j = 0; j < DIGI_NUM_CHNL_MAX; j++)
        {
            if (!(adpt_handle->restart_info[j]->enable))
            {
                continue;
            }

            for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
            {
                if (!(adpt_handle->traffic_info[index].enable))
                {
                    continue;
                }

                if (adpt_handle->traffic_info[index].chnl_id == adpt_handle->restart_info[j]->chnl_id)
                {
                    rc = adpt_dp_switch_add(dev_id, index, j);
                    ADPT_RETURN_VERIFY(rc);

                    /* find datapath channel */
                    break;
                }
            }
        }

        /* record initialization state as complete */
        adpt_handle->init_state = DIGI_INIT_DONE;

        /* release this new context */
        for (index = 0; index < DIGI_NUM_CHNL_MAX; index++)
        {
            PMC_FREE(&(adpt_handle->restart_info[index]));
        }
    }

    PMC_RETURN(rc);
} /* adpt_dp_restore */


#ifdef PMC_COMEXPRESS
/*******************************************************************************
* adpt_dp_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API configures the datapath operation mode.
*
*   DIGI_DP_NORMAL: Configure the datapath APIs with the normal operation mode 
*   which requires necessary sequences.
*
*   DIGI_DP_IDEMPOTENT: Configure the datapath APIs with the itempotent operation 
*   which has the following functions:
*   1. Have the same behavior for the same input parameters.
*   2. Those conflicted resources can be deleted dynamically.
*
* INPUTS:
*   dev_id             - device identity.
*   mode               - datapath operation mode. 
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
PUBLIC PMC_ERROR adpt_dp_mode_set(UINT32 dev_id, digi_dp_mode_t mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, mode);

    /* parameter check */
    if (mode >= LAST_DIGI_DP_OP) 
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    adpt_handle->dp_mode = mode;

    PMC_RETURN(rc);
} /* adpt_dp_mode_set */


/*******************************************************************************
* adpt_dp_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API retrieves the datapath operation mode.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *mode              - datapath operation mode. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_dp_mode_get(UINT32 dev_id, digi_dp_mode_t *mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(mode);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    *mode = adpt_handle->dp_mode;

    PMC_RETURN(rc);
} /* adpt_dp_mode_get */
#endif


/*
** End of file
*/
