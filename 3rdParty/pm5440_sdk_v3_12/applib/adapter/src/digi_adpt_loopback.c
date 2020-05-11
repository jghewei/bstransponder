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
*     The file describes the Loopback Run-Time functionality of DIGI device.
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
* adpt_loopback_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures loopback type in whole DIGI device. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   loop_type          - loopback mode.
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
PUBLIC PMC_ERROR adpt_loopback_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_loopback_type_t loop_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    digi_serdes_intf_t serdes_type = DIGI_SERDES_LINE_INTF;
    digi_loopback_type_t old_loop_type = DIGI_LPBK_DISABLE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, loop_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    if (serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        serdes_handle = adpt_handle->line_serdes[serdes_port];
        serdes_type = adpt_handle->dev_info->serdes_type[serdes_port];
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
    }

    switch (loop_type)
    {
        case DIGI_LPBK_DISABLE:
            /* get original loopback type */
            rc = adpt_loopback_get(dev_id, chnl_id, &old_loop_type);
            ADPT_RETURN_VERIFY(rc);

            /* disable loopback */
            switch (old_loop_type)
            {
                case DIGI_LPBK_OUTWARD:
                    if (DIGI_SERDES_SFI_51_INTF != serdes_type)
                    {
                        rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                           serdes_handle,
                                                           DIGI_SERDES_FACILITY_LOOPBACK,
                                                           FALSE);
                    }
                    break;

                case DIGI_LPBK_INWARD:
                    rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                       serdes_handle,
                                                       DIGI_SERDES_DIAGNOSTIC_LOOPBACK,
                                                       FALSE);
                    break;

                case DIGI_LPBK_INWARD_WITH_TX:
                    rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                       serdes_handle,
                                                       DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK,
                                                       FALSE);
                    break;

                default:
                    break;
            }
            break;

        case DIGI_LPBK_OUTWARD:
            /* get original loopback type */
            rc = adpt_loopback_get(dev_id, chnl_id, &old_loop_type);
            ADPT_RETURN_VERIFY(rc);

            /* disable inward loopback */
            if (DIGI_LPBK_INWARD == old_loop_type)
            {
                rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                   serdes_handle,
                                                   DIGI_SERDES_DIAGNOSTIC_LOOPBACK,
                                                   FALSE);
            }
            /* disable inward loopback with Tx */
            else if (DIGI_LPBK_INWARD_WITH_TX == old_loop_type)
            {
                rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                   serdes_handle,
                                                   DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK,
                                                   FALSE);
            }

            /* enable outward loopback */
            if (DIGI_LPBK_OUTWARD != old_loop_type)
            {
                if (DIGI_SERDES_SFI_51_INTF != serdes_type)
                {
                    rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                       serdes_handle,
                                                       DIGI_SERDES_FACILITY_LOOPBACK,
                                                       TRUE);
                }
            }
            break;

        case DIGI_LPBK_INWARD:
            /* get original loopback type */
            rc = adpt_loopback_get(dev_id, chnl_id, &old_loop_type);
            ADPT_RETURN_VERIFY(rc);

            /* disable outward loopback */
            if (DIGI_LPBK_OUTWARD == old_loop_type)
            {
                if (DIGI_SERDES_SFI_51_INTF != serdes_type)
                {
                    rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                       serdes_handle,
                                                       DIGI_SERDES_FACILITY_LOOPBACK,
                                                       FALSE);
                }
            }
            /* disable inward loopback with Tx */
            else if (DIGI_LPBK_INWARD_WITH_TX == old_loop_type)
            {
                rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                   serdes_handle,
                                                   DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK,
                                                   FALSE);
            }

            /* enable inward loopback */
            if (DIGI_LPBK_INWARD != old_loop_type)
            {
                rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                   serdes_handle,
                                                   DIGI_SERDES_DIAGNOSTIC_LOOPBACK,
                                                   TRUE);
            }
            break;

        case DIGI_LPBK_INWARD_WITH_TX:
            /* get original loopback type */
            rc = adpt_loopback_get(dev_id, chnl_id, &old_loop_type);
            ADPT_RETURN_VERIFY(rc);

            /* disable outward loopback */
            if (DIGI_LPBK_OUTWARD == old_loop_type)
            {
                if (DIGI_SERDES_SFI_51_INTF != serdes_type)
                {
                    rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                       serdes_handle,
                                                       DIGI_SERDES_FACILITY_LOOPBACK,
                                                       FALSE);
                }
            }
            /* disable inward loopback */
            else if (DIGI_LPBK_INWARD == old_loop_type)
            {
                rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                   serdes_handle,
                                                   DIGI_SERDES_DIAGNOSTIC_LOOPBACK,
                                                   FALSE);
            }

            /* enable inward loopback with Tx */
            if (DIGI_LPBK_INWARD_WITH_TX != old_loop_type)
            {
                rc = digi_loopback_serdes_port_cfg(adpt_handle->digi_handle,
                                                   serdes_handle,
                                                   DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK,
                                                   TRUE);
            }
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_loopback_set */


/*******************************************************************************
* adpt_loopback_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves loopback type in DIGI device. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *loop_type         - loopback mode.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_loopback_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_loopback_type_t *loop_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    digi_serdes_intf_t serdes_type = DIGI_SERDES_LINE_INTF;
    BOOL8 out_loop_en = FALSE;
    BOOL8 in_loop_en = FALSE;
    BOOL8 in_loop_tx_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(loop_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get traffic location */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);
    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    if (serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        serdes_handle = adpt_handle->line_serdes[serdes_port];
        serdes_type = adpt_handle->dev_info->serdes_type[serdes_port];
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    /* get outward(facility) loopback */
    if (DIGI_SERDES_SFI_51_INTF != serdes_type)
    {
        rc = digi_loopback_serdes_port_status_get(adpt_handle->digi_handle,
                                                  serdes_handle,
                                                  DIGI_SERDES_FACILITY_LOOPBACK,
                                                  &out_loop_en);
        ADPT_RETURN_VERIFY(rc);
    }

    /* get inward(diagnostic) loopback */
    rc = digi_loopback_serdes_port_status_get(adpt_handle->digi_handle,
                                              serdes_handle,
                                              DIGI_SERDES_DIAGNOSTIC_LOOPBACK,
                                              &in_loop_en);
    ADPT_RETURN_VERIFY(rc);

    /* get inward(diagnostic) loopback with Tx */
    rc = digi_loopback_serdes_port_status_get(adpt_handle->digi_handle,
                                              serdes_handle,
                                              DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK,
                                              &in_loop_tx_en);
    ADPT_RETURN_VERIFY(rc);

    if (out_loop_en)
    {
        *loop_type = DIGI_LPBK_OUTWARD;
    }
    else if (in_loop_en)
    {
        *loop_type = DIGI_LPBK_INWARD;
    }
    else if (in_loop_tx_en)
    {
        *loop_type = DIGI_LPBK_INWARD_WITH_TX;
    }
    else
    {
        *loop_type = DIGI_LPBK_DISABLE;
    }

    PMC_RETURN(rc);
} /* adpt_loopback_get */


/*******************************************************************************
* adpt_internal_loopback_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures internal loopback in each subsystem block.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   loop_type          - loopback type.
*   enable             - TRUE: Enable internal loopback.
*                        FALSE: Disable internal loopback.
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
*   As to COPI loopback, it will affect all channels in this interlaken.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_internal_loopback_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_loopback_t loop_type,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 client_mode = 0;
    digi_ilkn_link_t *ilkn_link = NULL;
    BOOL loop_en = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", 
               dev_id, chnl_id, loop_type, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get loopback status */
    rc = adpt_internal_loopback_get(dev_id, chnl_id, loop_type, &loop_en);
    ADPT_RETURN_VERIFY(rc);

    if ((loop_en && enable) || (!loop_en && !enable))
    {
        PMC_RETURN(rc);
    }

    switch (loop_type) 
    {
        case DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK: 
        case DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK: 
            /* handle verification */
            ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].otu_handle);

            rc |= digi_loopback_internal_cfg(adpt_handle->digi_handle,
                                             adpt_handle->traffic_info[index].otu_handle,
                                             loop_type,
                                             (BOOL8)enable);
            break;

        case DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK: 
        case DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK: 
        case DIGI_SIFD_COPI_FACILITY_LOOPBACK: 
            /* get client mode */
            client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
            switch (client_mode)
            {
                case DIGI_ILKN_SW_COPI1:
                case DIGI_ILKN_CPB_COPI1:
                    ilkn_link = adpt_handle->ilkn_1_ctxt;
                    break;

                case DIGI_ILKN_SW_COPI2:
                case DIGI_ILKN_CPB_COPI2:
                    ilkn_link = adpt_handle->ilkn_2_ctxt;
                    break;

                default:
                    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                    break;
            }

            /* handle verification */
            ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].ilkn_handle);
            ADPT_PARA_VERIFY(ilkn_link);

            rc |= digi_loopback_internal_cfg(adpt_handle->digi_handle,
                                             ilkn_link,
                                             loop_type,
                                             (BOOL8)enable);
            break;

        case DIGI_CBRC_DIAG_LOOPBACK: 
            /* handle verification */
            ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].cbr_handle);

            rc |= digi_loopback_internal_cfg(adpt_handle->digi_handle,
                                             adpt_handle->traffic_info[index].cbr_handle,
                                             loop_type,
                                             (BOOL8)enable);
            break;

        case DIGI_ENET_MAC_MLD_LOOPBACK: 
            /* handle verification */
            ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

            rc |= digi_loopback_internal_cfg(adpt_handle->digi_handle,
                                             adpt_handle->traffic_info[index].enet_handle,
                                             loop_type,
                                             (BOOL8)enable);
            break;

        case DIGI_ENET_MAPOTN_G709_LOOPBACK: 
        case DIGI_ENET_MAPOTN_GMP_LOOPBACK: 
            /* handle verification */
            ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].map_handle);

            rc |= digi_loopback_internal_cfg(adpt_handle->digi_handle,
                                             adpt_handle->traffic_info[index].map_handle,
                                             loop_type,
                                             (BOOL8)enable);
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_internal_loopback_set */


/*******************************************************************************
* adpt_internal_loopback_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of internal loopback in each subsystem 
*   block.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   loop_type          - loopback type.
*
* OUTPUTS:
*   *enable            - TRUE: Enable internal loopback.
*                        FALSE: Disable internal loopback.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_internal_loopback_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_loopback_t loop_type,
                                       BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 loop_en = FALSE;
    UINT32 client_mode = 0;
    digi_ilkn_link_t *ilkn_link = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    switch (loop_type) 
    {
        case DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK: 
        case DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK: 
            if (NULL != adpt_handle->traffic_info[index].otu_handle)
            {
                rc |= digi_loopback_internal_status_get(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].otu_handle,
                                                        loop_type,
                                                        &loop_en);
            }
            *enable = loop_en ? TRUE : FALSE;
            break;

        case DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK: 
        case DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK: 
        case DIGI_SIFD_COPI_FACILITY_LOOPBACK: 
            if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
            {
                /* get client mode */
                client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);
                switch (client_mode)
                {
                    case DIGI_ILKN_SW_COPI1:
                    case DIGI_ILKN_CPB_COPI1:
                        ilkn_link = adpt_handle->ilkn_1_ctxt;
                        break;

                    case DIGI_ILKN_SW_COPI2:
                    case DIGI_ILKN_CPB_COPI2:
                        ilkn_link = adpt_handle->ilkn_2_ctxt;
                        break;

                    default:
                        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                        break;
                }

                /* handle verification */
                ADPT_PARA_VERIFY(ilkn_link);

                rc |= digi_loopback_internal_status_get(adpt_handle->digi_handle,
                                                        ilkn_link,
                                                        loop_type,
                                                        &loop_en);
            }
            *enable = loop_en ? TRUE : FALSE;
            break;

        case DIGI_CBRC_DIAG_LOOPBACK: 
            if (NULL != adpt_handle->traffic_info[index].cbr_handle)
            {
                rc |= digi_loopback_internal_status_get(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].cbr_handle,
                                                        loop_type,
                                                        &loop_en);
            }
            *enable = loop_en ? TRUE : FALSE;
            break;

        case DIGI_ENET_MAC_MLD_LOOPBACK: 
            if (NULL != adpt_handle->traffic_info[index].enet_handle)
            {
                rc |= digi_loopback_internal_status_get(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].enet_handle,
                                                        loop_type,
                                                        &loop_en);
            }
            *enable = loop_en ? TRUE : FALSE;
            break;

        case DIGI_ENET_MAPOTN_G709_LOOPBACK: 
        case DIGI_ENET_MAPOTN_GMP_LOOPBACK: 
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                rc |= digi_loopback_internal_status_get(adpt_handle->digi_handle,
                                                        adpt_handle->traffic_info[index].map_handle,
                                                        loop_type,
                                                        &loop_en);
            }
            *enable = loop_en ? TRUE : FALSE;
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_internal_loopback_get */


/*******************************************************************************
* adpt_ge_loopback_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures loopback in GE management interface(D8). The transmitted data 
*   is looped back to the receiver.
*
* INPUTS:
*   dev_id             - device identity.
*   intf_type          - GE serdes interface type.
*   enable             - TRUE: Enable GE loopback.
*                        FALSE: Disable GE loopback.
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
PUBLIC PMC_ERROR adpt_ge_loopback_set(UINT32 dev_id,
                                   digi_ge_serdes_id_t intf_type,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, intf_type, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_ge_loopback_get(dev_id, intf_type, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if ((org_enable && !enable) || (!org_enable && enable))
    {
        rc = digi_loopback_serdes_ge_cfg(adpt_handle->digi_handle,
                                         intf_type,
                                         (BOOL8)enable);
    }

    PMC_RETURN(rc);
} /* adpt_ge_loopback_set */


/*******************************************************************************
* adpt_ge_loopback_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of loopback in GE management interface(D8). 
*   The transmitted data is looped back to the receiver.
*
* INPUTS:
*   dev_id             - device identity.
*   intf_type          - GE serdes interface type.
*
* OUTPUTS:
*   *enable            - TRUE: Enable GE loopback.
*                        FALSE: Disable GE loopback.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ge_loopback_get(UINT32 dev_id,
                                   digi_ge_serdes_id_t intf_type,
                                   BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 loop_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_loopback_serdes_ge_status_get(adpt_handle->digi_handle,
                                            intf_type,
                                            &loop_en);
    *enable = loop_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_ge_loopback_get */


/*******************************************************************************
* adpt_ilkn_loopback_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures interlaken related loopback.
*
* INPUTS:
*   dev_id             - device identity.
*   ilkn_type          - interlaken interface type.
*   enable             - TRUE: Enable interlaken loopback.
*                        FALSE: Disable interlaken loopback.
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
PUBLIC PMC_ERROR adpt_ilkn_loopback_set(UINT32 dev_id,
                                    digi_ilkn_intf_t ilkn_type,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, ilkn_type, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_ilkn_loopback_get(dev_id, ilkn_type, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        switch (ilkn_type) 
        {
            case DIGI_ILKN1:
            case DIGI_ILKN2:
                rc |= digi_loopback_serdes_bp_cfg(adpt_handle->digi_handle,
                                                  ilkn_type,
                                                  (BOOL8)enable);
                break;



            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
    }

    PMC_RETURN(rc);
} /* adpt_ilkn_loopback_set */


/*******************************************************************************
* adpt_ilkn_loopback_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of interlaken related loopback.
*
* INPUTS:
*   dev_id             - device identity.
*   ilkn_type          - interlaken interface type.
*
* OUTPUTS:
*   *enable            - TRUE: Enable interlaken loopback.
*                        FALSE: Disable interlaken loopback.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ilkn_loopback_get(UINT32 dev_id,
                                    digi_ilkn_intf_t ilkn_type,
                                    BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 loop_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (ilkn_type) 
    {
        case DIGI_ILKN1:
        case DIGI_ILKN2:
            rc |= digi_loopback_serdes_bp_status_get(adpt_handle->digi_handle,
                                                     ilkn_type,
                                                     &loop_en);
            *enable = loop_en ? TRUE : FALSE;
            break;



        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    PMC_RETURN(rc);
} /* adpt_ilkn_loopback_get */


/*
** End of file
*/
