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
*     The file describes the serdes functionality of DIGI device.
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
* adpt_serdes_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures serdes PRBS function of one line, system or SFI51 interface 
*   serdes.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   prbs_mode          - prbs pattern mode.
*   inv_cfg            - pattern monitor inverter configuration ON(TRUE) or 
*                        OFF(FALSE).
*   enable             - TRUE: enable serdes PRBS.
*                        FALSE: disable serdes PRBS.
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
PUBLIC PMC_ERROR adpt_serdes_prbs_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_serdes_dir_t dir,
                                   digi_serdes_prbs_t prbs_mode,
                                   BOOL inv_cfg,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    util_serdes_patt_mon_cfg_t prbs_mon_cfg;
    util_serdes_patt_gen_cfg_t prbs_gen_cfg;
    UINT32 lane_index = 0;
    UINT32 *port_lane_tx = NULL;
    UINT32 *port_lane_rx = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, prbs_mode, inv_cfg, enable);

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
        port_lane_tx  = adpt_handle->dev_info->line_port_lane_tx[serdes_port];
        port_lane_rx  = adpt_handle->dev_info->line_port_lane_rx[serdes_port];
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
        port_lane_tx  = adpt_handle->dev_info->sys_port_lane_tx[serdes_port];
        port_lane_rx  = adpt_handle->dev_info->sys_port_lane_rx[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    /* reset */
    PMC_MEMSET((void*)&prbs_mon_cfg, 0, sizeof(util_serdes_patt_mon_cfg_t));
    PMC_MEMSET((void*)&prbs_gen_cfg, 0, sizeof(util_serdes_patt_gen_cfg_t));

    if (enable)
    {
        /* configuration on generation */
        if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            prbs_gen_cfg.patt_cfg.enable  = enable;
            prbs_gen_cfg.patt_cfg.inv_cfg = inv_cfg;
            prbs_gen_cfg.patt_cfg.mode    = (util_patt_mode_t)prbs_mode;
            for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
            {
                if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_tx + lane_index))
                {
                    rc |= digi_serdes_line_sys_patt_gen_prov(adpt_handle->digi_handle,
                                                             serdes_handle,
                                                             *(port_lane_tx + lane_index),
                                                             &prbs_gen_cfg);
                }
            }    
        }

        /* configuration on monitor */
        if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            prbs_mon_cfg.patt_cfg.enable  = enable;
            prbs_mon_cfg.patt_cfg.inv_cfg = inv_cfg;
            prbs_mon_cfg.patt_cfg.mode    = (util_patt_mode_t)prbs_mode;
            for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
            {
                if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_rx + lane_index))
                {
                    rc |= digi_serdes_line_sys_patt_mon_prov(adpt_handle->digi_handle,
                                                             serdes_handle,
                                                             *(port_lane_rx + lane_index),
                                                             &prbs_mon_cfg);
                    /* PRBS monitor resynchronization */
                    rc |= digi_serdes_line_sys_patt_mon_resync(adpt_handle->digi_handle,
                                                               serdes_handle,
                                                               *(port_lane_rx + lane_index));
                }                                           
            }
        }
    }
    else
    {
        if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
            {
                if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_tx + lane_index))
                {
                    rc |= digi_serdes_line_sys_patt_gen_deprov(adpt_handle->digi_handle,
                                                               serdes_handle,
                                                               *(port_lane_tx + lane_index));
                }
            }
        }

        if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
            {
                if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_rx + lane_index))
                {
                    rc |= digi_serdes_line_sys_patt_mon_deprov(adpt_handle->digi_handle,
                                                               serdes_handle,
                                                               *(port_lane_rx + lane_index));
                }
            }
        }
    }
    
    PMC_RETURN(rc);
} /* adpt_serdes_prbs_set */


/*******************************************************************************
* adpt_serdes_tx_err_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures error insertion in the Tx direction of serdes PRBS function 
*   generated by one line, system or SFI51 interface serdes. 
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
PUBLIC PMC_ERROR adpt_serdes_tx_err_inst_set(UINT32 dev_id,
                                        UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    UINT32 lane_index = 0;
    UINT32 *port_lane_tx = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, chnl_id);

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
        port_lane_tx  = adpt_handle->dev_info->line_port_lane_tx[serdes_port];
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
        port_lane_tx  = adpt_handle->dev_info->sys_port_lane_tx[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
    {
        if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_tx + lane_index))
        {
            rc |= digi_serdes_line_sys_patt_gen_err_insert_cfg(adpt_handle->digi_handle,
                                                               serdes_handle,
                                                               *(port_lane_tx + lane_index));
        }
    }    
    
    PMC_RETURN(rc);
} /* adpt_serdes_tx_err_inst_set */


/*******************************************************************************
* adpt_serdes_rx_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves pattern monitor status and errors counter value of
*   line, system or SFI51 interface serdes.
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
PUBLIC PMC_ERROR adpt_serdes_rx_state_get(UINT32 dev_id, UINT32 chnl_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    UINT32 lane_index = 0;
    UINT32 *port_lane_rx = NULL;
    util_patt_status_t mon_status = LAST_UTIL_PATT_STATUS;
    UINT32 err_cnt = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, chnl_id);

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
        port_lane_rx  = adpt_handle->dev_info->line_port_lane_rx[serdes_port];
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
        port_lane_rx  = adpt_handle->dev_info->sys_port_lane_rx[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    for (lane_index = 0; lane_index < DIGI_SERDES_XFI_LANES_MAX; lane_index++)
    {
        if (DIGI_SERDES_PIN_NOT_USED != *(port_lane_rx + lane_index))
        {
            /* get PRBS status */
            rc |= digi_serdes_line_sys_patt_mon_err_count_get(adpt_handle->digi_handle,
                                                              serdes_handle,
                                                              *(port_lane_rx + lane_index),
                                                              &mon_status,
                                                              &err_cnt);
            printf("Lane index[%u]: Status = [%s] Error count = [0x%x].\r\n", 
                   lane_index, adpt_prbs_status_str(mon_status), err_cnt);
        }
    }    

    PMC_RETURN(rc);
} /* adpt_serdes_rx_state_get */


/*******************************************************************************
* adpt_serdes_polarity_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures serdes polarity inversion of one line, system or SFI51 interface 
*   serdes.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   lane_mask          - For line and system interface,
*                        it is the logical lane mask associated to serdes.
*                        For SFI51 interface, it is the serdes pins mask. 
*   enable             - TRUE: enable the polarity inversion.
*                        FALSE: disable the polarity inversion.
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
PUBLIC PMC_ERROR adpt_serdes_polarity_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_serdes_dir_t dir,
                                     UINT32 lane_mask,
                                     BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, chnl_id, dir, lane_mask, enable);

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
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
    {
        rc = digi_serdes_port_polarity_cfg(adpt_handle->digi_handle,
                                           serdes_handle,
                                           lane_mask,
                                           UTIL_GLOBAL_DIR_TX,
                                           enable);
        ADPT_RETURN_VERIFY(rc);
    }

    if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
    {
        rc = digi_serdes_port_polarity_cfg(adpt_handle->digi_handle,
                                           serdes_handle,
                                           lane_mask,
                                           UTIL_GLOBAL_DIR_RX,
                                           enable);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_serdes_polarity_set */


/*******************************************************************************
* adpt_serdes_lane_rx_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves pattern monitor status and errors counter value of line, system 
*   or SFI51 interface serdes per lane.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_id            - For line and system interface, it is the lane
*                        index associated to the serdes module.
*                        For SFI51 interface, it is the device pin index.
*
* OUTPUTS:
*   *status            - Pattern monitor status: Locked, trying to lock, etc.                  
*   *err_cnt           - Number of errors detected by pattern monitor. 
*                        The value shall be ignored when pattern monitor
*                        is not in locked state.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_lane_rx_state_get(UINT32 dev_id, 
                                          UINT32 chnl_id,
                                          UINT32 lane_id,
                                          util_patt_status_t *status,
                                          UINT32 *err_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 traffic_loc = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_serdes_port_t *serdes_handle = NULL;
    UINT32 serdes_port = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(status);
    ADPT_PARA_VERIFY(err_cnt);

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
    if ((serdes_port >= DIGI_SERDES_XFI_LANES_MAX) || (lane_id >= DIGI_SERDES_XFI_LANES_MAX))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    if (DIGI_LINE_SIDE == traffic_loc)
    {
        serdes_handle = adpt_handle->line_serdes[serdes_port];
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    /* get PRBS status */
    rc = digi_serdes_line_sys_patt_mon_err_count_get(adpt_handle->digi_handle,
                                                     serdes_handle,
                                                     lane_id,
                                                     status,
                                                     err_cnt);

    PMC_RETURN(rc);
} /* adpt_serdes_lane_rx_state_get */


/*******************************************************************************
* adpt_serdes_bp_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures serdes PRBS function of one T8 or C8 or S16 serdes 
*   in the backplane interface.
*
* INPUTS:
*   dev_id             - device identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   prbs_mode          - prbs pattern mode.
*   inv_cfg            - pattern monitor inverter configuration ON(TRUE) or 
*                        OFF(FALSE).
*   enable             - TRUE: enable serdes PRBS.
*                        FALSE: disable serdes PRBS.
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
PUBLIC PMC_ERROR adpt_serdes_bp_prbs_set(UINT32 dev_id,
                                      digi_serdes_dir_t dir,
                                      digi_serdes_prbs_t prbs_mode,
                                      BOOL inv_cfg,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg = NULL;
    digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg = NULL;
    digi_sifd_ilkn_1_lane_cfg_t sifd_1_cfg;
    digi_sifd_ilkn_2_lane_cfg_t sifd_2_cfg;
    util_serdes_patt_mon_cfg_t prbs_mon_cfg;
    util_serdes_patt_gen_cfg_t prbs_gen_cfg;
    UINT32 index = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               dir, prbs_mode, inv_cfg, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->dev_info->sifd_type)
    {
        case ILKN_SIFD_1:
            sifd_1_lane_cfg = &(adpt_handle->dev_info->sifd_1_cfg.lane);
            sifd_2_lane_cfg = NULL;
            break;

        case ILKN_SIFD_2:
            sifd_1_lane_cfg = NULL;
            sifd_2_lane_cfg = &(adpt_handle->dev_info->sifd_2_cfg.lane);
            break;

        case ILKN_SIFD_1_2:
            sifd_1_lane_cfg = &(adpt_handle->dev_info->sifd_1_cfg.lane);
            sifd_2_lane_cfg = &(adpt_handle->dev_info->sifd_2_cfg.lane);
            break;

        default:
            PMC_RETURN(PMC_ERR_FAIL);
            break;
    }

    /* reset */
    PMC_MEMSET((void*)&prbs_mon_cfg, 0, sizeof(util_serdes_patt_mon_cfg_t));
    PMC_MEMSET((void*)&prbs_gen_cfg, 0, sizeof(util_serdes_patt_gen_cfg_t));

    for (index = 0; index < 192; index++)
    {
        if ((index < 96) && (NULL == sifd_1_lane_cfg))
        {
            continue;
        }
        if ((index >= 96) && (NULL == sifd_2_lane_cfg))
        {
            break;
        }

        PMC_MEMSET((void*)&sifd_1_cfg, 0, sizeof(digi_sifd_ilkn_1_lane_cfg_t));
        PMC_MEMSET((void*)&sifd_2_cfg, 0, sizeof(digi_sifd_ilkn_2_lane_cfg_t));

        if (index < 32)
        {
            sifd_1_cfg.blk_to_E1_enable_mask = sifd_1_lane_cfg->blk_to_E1_enable_mask & (1 << index); 
            if (0 == sifd_1_cfg.blk_to_E1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 64)
        {
            sifd_1_cfg.blk_to_N1_enable_mask = sifd_1_lane_cfg->blk_to_N1_enable_mask & (1 << (index - 32)); 
            if (0 == sifd_1_cfg.blk_to_N1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 96)
        {
            sifd_1_cfg.blk_to_N2_enable_mask = sifd_1_lane_cfg->blk_to_N2_enable_mask & (1 << (index - 64)); 
            if (0 == sifd_1_cfg.blk_to_N2_enable_mask)
            {
                continue;
            }
        }
        else if (index < 128)
        {
            sifd_2_cfg.blk_to_E1_enable_mask = sifd_2_lane_cfg->blk_to_E1_enable_mask & (1 << (index - 96)); 
            if (0 == sifd_2_cfg.blk_to_E1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 160)
        {
            sifd_2_cfg.blk_to_N1_enable_mask = sifd_2_lane_cfg->blk_to_N1_enable_mask & (1 << (index - 128)); 
            if (0 == sifd_2_cfg.blk_to_N1_enable_mask)
            {
                continue;
            }
        }
        else
        {
            sifd_2_cfg.blk_to_N2_enable_mask = sifd_2_lane_cfg->blk_to_N2_enable_mask & (1 << (index - 160)); 
            if (0 == sifd_2_cfg.blk_to_N2_enable_mask)
            {
                continue;
            }
        }

        if (enable)
        {
            /* configuration on generation */
            if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
            {
                prbs_gen_cfg.patt_cfg.enable  = enable;
                prbs_gen_cfg.patt_cfg.inv_cfg = inv_cfg;
                prbs_gen_cfg.patt_cfg.mode    = (util_patt_mode_t)prbs_mode;
                rc |= digi_serdes_bp_patt_gen_prov(adpt_handle->digi_handle,
                                                   &sifd_1_cfg,
                                                   &sifd_2_cfg,
                                                   &prbs_gen_cfg);
            }

            /* configuration on monitor */
            if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
            {
                prbs_mon_cfg.patt_cfg.enable  = enable;
                prbs_mon_cfg.patt_cfg.inv_cfg = inv_cfg;
                prbs_mon_cfg.patt_cfg.mode    = (util_patt_mode_t)prbs_mode;
                rc |= digi_serdes_bp_patt_mon_prov(adpt_handle->digi_handle,
                                                   &sifd_1_cfg,
                                                   &sifd_2_cfg,
                                                   &prbs_mon_cfg);
                /* PRBS monitor resynchronization */
                rc |= digi_serdes_bp_patt_mon_resync(adpt_handle->digi_handle,
                                                     &sifd_1_cfg,
                                                     &sifd_2_cfg);
            }
        }
        else
        {
            if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
            {
                rc |= digi_serdes_bp_patt_gen_deprov(adpt_handle->digi_handle,
                                                     &sifd_1_cfg,
                                                     &sifd_2_cfg);
            }

            if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
            {
                rc |= digi_serdes_bp_patt_mon_deprov(adpt_handle->digi_handle,
                                                     &sifd_1_cfg,
                                                     &sifd_2_cfg);
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_serdes_bp_prbs_set */


/*******************************************************************************
* adpt_serdes_bp_tx_err_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures error insertion in the Tx direction of serdes PRBS function 
*   generated by one T8 or C8 or S16 serdes in the backplane interface.
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
PUBLIC PMC_ERROR adpt_serdes_bp_tx_err_inst_set(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg = NULL;
    digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg = NULL;
    digi_sifd_ilkn_1_lane_cfg_t sifd_1_cfg;
    digi_sifd_ilkn_2_lane_cfg_t sifd_2_cfg;
    UINT32 index = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x].\r\n", dev_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->dev_info->sifd_type)
    {
        case ILKN_SIFD_1:
            sifd_1_lane_cfg = &(adpt_handle->dev_info->sifd_1_cfg.lane);
            sifd_2_lane_cfg = NULL;
            break;

        case ILKN_SIFD_2:
            sifd_1_lane_cfg = NULL;
            sifd_2_lane_cfg = &(adpt_handle->dev_info->sifd_2_cfg.lane);
            break;

        case ILKN_SIFD_1_2:
            sifd_1_lane_cfg = &(adpt_handle->dev_info->sifd_1_cfg.lane);
            sifd_2_lane_cfg = &(adpt_handle->dev_info->sifd_2_cfg.lane);
            break;

        default:
            PMC_RETURN(PMC_ERR_FAIL);
            break;
    }

    for (index = 0; index < 192; index++)
    {
        if ((index < 96) && (NULL == sifd_1_lane_cfg))
        {
            continue;
        }
        if ((index >= 96) && (NULL == sifd_2_lane_cfg))
        {
            break;
        }

        PMC_MEMSET((void*)&sifd_1_cfg, 0, sizeof(digi_sifd_ilkn_1_lane_cfg_t));
        PMC_MEMSET((void*)&sifd_2_cfg, 0, sizeof(digi_sifd_ilkn_2_lane_cfg_t));

        if (index < 32)
        {
            sifd_1_cfg.blk_to_E1_enable_mask = sifd_1_lane_cfg->blk_to_E1_enable_mask & (1 << index); 
            if (0 == sifd_1_cfg.blk_to_E1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 64)
        {
            sifd_1_cfg.blk_to_N1_enable_mask = sifd_1_lane_cfg->blk_to_N1_enable_mask & (1 << (index - 32)); 
            if (0 == sifd_1_cfg.blk_to_N1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 96)
        {
            sifd_1_cfg.blk_to_N2_enable_mask = sifd_1_lane_cfg->blk_to_N2_enable_mask & (1 << (index - 64)); 
            if (0 == sifd_1_cfg.blk_to_N2_enable_mask)
            {
                continue;
            }
        }
        else if (index < 128)
        {
            sifd_2_cfg.blk_to_E1_enable_mask = sifd_2_lane_cfg->blk_to_E1_enable_mask & (1 << (index - 96)); 
            if (0 == sifd_2_cfg.blk_to_E1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 160)
        {
            sifd_2_cfg.blk_to_N1_enable_mask = sifd_2_lane_cfg->blk_to_N1_enable_mask & (1 << (index - 128)); 
            if (0 == sifd_2_cfg.blk_to_N1_enable_mask)
            {
                continue;
            }
        }
        else
        {
            sifd_2_cfg.blk_to_N2_enable_mask = sifd_2_lane_cfg->blk_to_N2_enable_mask & (1 << (index - 160)); 
            if (0 == sifd_2_cfg.blk_to_N2_enable_mask)
            {
                continue;
            }
        }

        rc |= digi_serdes_bp_patt_gen_err_insert_cfg(adpt_handle->digi_handle,
                                                     &sifd_1_cfg,
                                                     &sifd_2_cfg);
    }

    PMC_RETURN(rc);
} /* adpt_serdes_bp_tx_err_inst_set */


/*******************************************************************************
* adpt_serdes_bp_rx_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves pattern monitor status and the number of errors detected by 
*   one T8 or C8 or S16 serdes in the backplane interface.
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
PUBLIC PMC_ERROR adpt_serdes_bp_rx_state_get(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg = NULL;
    digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg = NULL;
    digi_sifd_ilkn_1_lane_cfg_t sifd_1_cfg;
    digi_sifd_ilkn_2_lane_cfg_t sifd_2_cfg;
    UINT32 index = 0;
    util_patt_status_t mon_status = LAST_UTIL_PATT_STATUS;
    UINT32 err_cnt = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x].\r\n", dev_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (adpt_handle->dev_info->sifd_type)
    {
        case ILKN_SIFD_1:
            sifd_1_lane_cfg = &(adpt_handle->dev_info->sifd_1_cfg.lane);
            sifd_2_lane_cfg = NULL;
            break;

        case ILKN_SIFD_2:
            sifd_1_lane_cfg = NULL;
            sifd_2_lane_cfg = &(adpt_handle->dev_info->sifd_2_cfg.lane);
            break;

        case ILKN_SIFD_1_2:
            sifd_1_lane_cfg = &(adpt_handle->dev_info->sifd_1_cfg.lane);
            sifd_2_lane_cfg = &(adpt_handle->dev_info->sifd_2_cfg.lane);
            break;

        default:
            PMC_RETURN(PMC_ERR_FAIL);
            break;
    }

    for (index = 0; index < 192; index++)
    {
        if ((index < 96) && (NULL == sifd_1_lane_cfg))
        {
            continue;
        }
        if ((index >= 96) && (NULL == sifd_2_lane_cfg))
        {
            break;
        }

        PMC_MEMSET((void*)&sifd_1_cfg, 0, sizeof(digi_sifd_ilkn_1_lane_cfg_t));
        PMC_MEMSET((void*)&sifd_2_cfg, 0, sizeof(digi_sifd_ilkn_2_lane_cfg_t));

        if (index < 32)
        {
            sifd_1_cfg.blk_to_E1_enable_mask = sifd_1_lane_cfg->blk_to_E1_enable_mask & (1 << index); 
            if (0 == sifd_1_cfg.blk_to_E1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 64)
        {
            sifd_1_cfg.blk_to_N1_enable_mask = sifd_1_lane_cfg->blk_to_N1_enable_mask & (1 << (index - 32)); 
            if (0 == sifd_1_cfg.blk_to_N1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 96)
        {
            sifd_1_cfg.blk_to_N2_enable_mask = sifd_1_lane_cfg->blk_to_N2_enable_mask & (1 << (index - 64)); 
            if (0 == sifd_1_cfg.blk_to_N2_enable_mask)
            {
                continue;
            }
        }
        else if (index < 128)
        {
            sifd_2_cfg.blk_to_E1_enable_mask = sifd_2_lane_cfg->blk_to_E1_enable_mask & (1 << (index - 96)); 
            if (0 == sifd_2_cfg.blk_to_E1_enable_mask)
            {
                continue;
            }
        }
        else if (index < 160)
        {
            sifd_2_cfg.blk_to_N1_enable_mask = sifd_2_lane_cfg->blk_to_N1_enable_mask & (1 << (index - 128)); 
            if (0 == sifd_2_cfg.blk_to_N1_enable_mask)
            {
                continue;
            }
        }
        else
        {
            sifd_2_cfg.blk_to_N2_enable_mask = sifd_2_lane_cfg->blk_to_N2_enable_mask & (1 << (index - 160)); 
            if (0 == sifd_2_cfg.blk_to_N2_enable_mask)
            {
                continue;
            }
        }

        /* get PRBS status */
        rc |= digi_serdes_bp_patt_mon_err_count_get(adpt_handle->digi_handle,
                                                    &sifd_1_cfg,
                                                    &sifd_2_cfg,
                                                    &mon_status,
                                                    &err_cnt);
        if (index < 96)
        {
            printf("T8[0x%x]S16[0x%x]C8[0x%x]: Status = [%s] Error count = [0x%x].\r\n", 
                   sifd_1_cfg.blk_to_E1_enable_mask, sifd_1_cfg.blk_to_N1_enable_mask, 
                   sifd_1_cfg.blk_to_N2_enable_mask, adpt_prbs_status_str(mon_status), err_cnt);
        }  
        else
        {
            printf("T8[0x%x]S16[0x%x]C8[0x%x]: Status = [%s] Error count = [0x%x].\r\n", 
                   sifd_2_cfg.blk_to_E1_enable_mask, sifd_2_cfg.blk_to_N1_enable_mask, 
                   sifd_2_cfg.blk_to_N2_enable_mask, adpt_prbs_status_str(mon_status), err_cnt);
        }
    }

    PMC_RETURN(rc);
} /* adpt_serdes_bp_rx_state_get */


/*******************************************************************************
* adpt_serdes_bp_polarity_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures polarity inversion of one T8 or C8 or S16 serdes 
*   in the backplane interface.
*
* INPUTS:
*   dev_id             - device identity.
*   ilkn_type          - interlaken interface type.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   lane_msk           - the logical lane mask associated to serdes.
*
*                        63 62 ... 41 40 39 38 ... 25 24 23 22 ... 1 0
*                        _______________ _______________ _____________
*                         n2_msk[63:40]   n1_msk[39:24]   e1_msk[23:0]
*
*                        ILKN1 Lanes 23 through 16:
*
*                        ILKN1 --------------------------------[23][22][21][20][19][18][17][16]
*                           E1 ------------------------------------------------[20][21][22][23]
*                           N1 ----------------------------------------------------------------
*                           N2 --------------------------------[ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*
*                        ILKN1 Lanes 15 through 0:
*
*                        ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*                           E1 [24][25][26][27][28][29][30][31][32][33][34][35][36][37][38][39]
*                           N1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*                           N2 ----------------------------------------------------------------
*
*                        ILKN2 Lanes 23 through 16:
*
*                        ILKN2 --------------------------------[23][22][21][20][19][18][17][16]
*                           E1 --------------------------------[23][22][21][20][19][18][17][16]
*                           N1 ----------------------------------------------------------------
*                           N2 ----------------------------------------------------------------
*
*                        ILKN2 Lanes 15 through 0:
*
*                        ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*                           E1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*                           N1 --------------------------------[15][14][13][12][11][10][ 9][ 8]
*                           N2 [ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]--------------------------------
*
*   enable             - TRUE: enable the polarity inversion.
*                        FALSE: disable the polarity inversion.
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
PUBLIC PMC_ERROR adpt_serdes_bp_polarity_set(UINT32 dev_id,
                                        digi_ilkn_intf_t ilkn_type,
                                        digi_serdes_dir_t dir,
                                        UINT64 lane_msk,
                                        BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_sifd_ilkn_1_lane_cfg_t sifd_1_cfg;
    digi_sifd_ilkn_2_lane_cfg_t sifd_2_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][0x%llx][%d].\r\n", 
               dev_id, ilkn_type, dir, lane_msk, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&sifd_1_cfg, 0, sizeof(digi_sifd_ilkn_1_lane_cfg_t));
    PMC_MEMSET((void*)&sifd_2_cfg, 0, sizeof(digi_sifd_ilkn_2_lane_cfg_t));

    switch (ilkn_type)
    {
        case DIGI_ILKN1:
            sifd_1_cfg.blk_to_E1_enable_mask = (UINT32)(lane_msk & 0xFFFFFF);
            sifd_1_cfg.blk_to_N1_enable_mask = (UINT32)((lane_msk >> 24) & 0xFFFF);
            sifd_1_cfg.blk_to_N2_enable_mask = (UINT32)((lane_msk >> 40) & 0xFFFFFF);
            break;

        case DIGI_ILKN2:
            sifd_2_cfg.blk_to_E1_enable_mask = (UINT32)(lane_msk & 0xFFFFFF);
            sifd_2_cfg.blk_to_N1_enable_mask = (UINT32)((lane_msk >> 24) & 0xFFFF);
            sifd_2_cfg.blk_to_N2_enable_mask = (UINT32)((lane_msk >> 40) & 0xFFFFFF);
            break;

        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    rc = digi_serdes_bp_polarity_cfg(adpt_handle->digi_handle,
                                     &sifd_1_cfg,
                                     &sifd_2_cfg,
                                     (util_global_direction_t)dir,
                                     enable);

    PMC_RETURN(rc);
} /* adpt_serdes_bp_polarity_set */






/*******************************************************************************
* adpt_serdes_ge_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures GE(SGMII) serdes PRBS function of OHP/RI_RCP/DI_RCP/EMGMT port.
*
* INPUTS:
*   dev_id             - device identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   ge_serdes          - port type.
*   prbs_mode          - prbs pattern mode.
*   inv_cfg            - pattern monitor inverter configuration ON(TRUE) or 
*                        OFF(FALSE).
*   enable             - TRUE: enable serdes PRBS.
*                        FALSE: disable serdes PRBS.
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
*   Before enabling GE PRBS feature, the OHP/RI_RCP/DI_RCP/GE port should be 
*   intialized first.
*   OHP:    adpt_otn_ohp_en_set()
*   RI_RCP: adpt_di_rcp_en_set()
*   DI_RCP: adpt_ri_rcp_en_set()
*   GE:     adpt_fege_en_set()
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_ge_prbs_set(UINT32 dev_id,
                                      digi_serdes_dir_t dir,
                                      digi_ge_serdes_id_t ge_serdes,
                                      digi_serdes_prbs_t prbs_mode,
                                      BOOL inv_cfg,
                                      BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    util_serdes_patt_mon_cfg_t prbs_mon_cfg;
    util_serdes_patt_gen_cfg_t prbs_gen_cfg;
    util_serdes_patt_mon_cfg_t org_mon_cfg;
    util_serdes_patt_gen_cfg_t org_gen_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               dir, ge_serdes, prbs_mode, inv_cfg, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&prbs_mon_cfg, 0, sizeof(util_serdes_patt_mon_cfg_t));
    PMC_MEMSET((void*)&prbs_gen_cfg, 0, sizeof(util_serdes_patt_gen_cfg_t));

    rc = digi_serdes_ge_patt_mon_get(adpt_handle->digi_handle,
                                     ge_serdes,
                                     &org_mon_cfg);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_ge_patt_gen_get(adpt_handle->digi_handle,
                                     ge_serdes,
                                     &org_gen_cfg);
    ADPT_RETURN_VERIFY(rc);

    if (enable)
    {
        /* configuration on generation */
        if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            prbs_gen_cfg.patt_cfg.enable  = enable;
            prbs_gen_cfg.patt_cfg.inv_cfg = inv_cfg;
            prbs_gen_cfg.patt_cfg.mode    = (util_patt_mode_t)prbs_mode;

            if (org_gen_cfg.patt_cfg.enable != enable)
            {
                rc = digi_serdes_ge_patt_gen_prov(adpt_handle->digi_handle,
                                                  ge_serdes,
                                                  &prbs_gen_cfg);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (enable)
            {
                if ((org_gen_cfg.patt_cfg.inv_cfg != inv_cfg)
                     || (org_gen_cfg.patt_cfg.mode != (util_patt_mode_t)prbs_mode))
                {
                    rc = digi_serdes_ge_patt_gen_deprov(adpt_handle->digi_handle,
                                                        ge_serdes);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_serdes_ge_patt_gen_prov(adpt_handle->digi_handle,
                                                      ge_serdes,
                                                      &prbs_gen_cfg);
                    ADPT_RETURN_VERIFY(rc);
                }
            }
        }

        /* configuration on monitor */
        if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            prbs_mon_cfg.patt_cfg.enable  = enable;
            prbs_mon_cfg.patt_cfg.inv_cfg = inv_cfg;
            prbs_mon_cfg.patt_cfg.mode    = (util_patt_mode_t)prbs_mode;

            if (org_mon_cfg.patt_cfg.enable != enable)
            {
                rc = digi_serdes_ge_patt_mon_prov(adpt_handle->digi_handle,
                                                  ge_serdes,
                                                  &prbs_mon_cfg);
                ADPT_RETURN_VERIFY(rc);
            }
            else if (enable)
            {
                if ((org_mon_cfg.patt_cfg.inv_cfg != inv_cfg)
                     || (org_mon_cfg.patt_cfg.mode != (util_patt_mode_t)prbs_mode))
                {
                    rc = digi_serdes_ge_patt_mon_deprov(adpt_handle->digi_handle,
                                                        ge_serdes);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_serdes_ge_patt_mon_prov(adpt_handle->digi_handle,
                                                      ge_serdes,
                                                      &prbs_mon_cfg);
                    ADPT_RETURN_VERIFY(rc);
                }
            }

            /* PRBS monitor resynchronization */
            rc = digi_serdes_ge_patt_mon_resync(adpt_handle->digi_handle, ge_serdes);
            ADPT_RETURN_VERIFY(rc);
        }
    }
    else
    {
        if ((DIGI_SERDES_DIR_TX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            if (org_gen_cfg.patt_cfg.enable)
            {
                rc = digi_serdes_ge_patt_gen_deprov(adpt_handle->digi_handle, ge_serdes);
            }
        }

        if ((DIGI_SERDES_DIR_RX == dir) || (DIGI_SERDES_DIR_RX_TX == dir))
        {
            if (org_mon_cfg.patt_cfg.enable)
            {
                rc = digi_serdes_ge_patt_mon_deprov(adpt_handle->digi_handle, ge_serdes);
            }
        }
    }

    PMC_RETURN(rc);
} /* adpt_serdes_ge_prbs_set */


/*******************************************************************************
* adpt_serdes_ge_tx_err_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures error insertion in the Tx direction of GE(SGMII) serdes PRBS 
*   function generated by OHP/RI_RCP/DI_RCP/MGMT_GE port.
*
* INPUTS:
*   dev_id             - device identity.
*   ge_serdes          - port type.
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
PUBLIC PMC_ERROR adpt_serdes_ge_tx_err_inst_set(UINT32 dev_id,
                                           digi_ge_serdes_id_t ge_serdes)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x] ge_serdes[%d].\r\n", dev_id, ge_serdes);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_ge_patt_gen_err_insert_cfg(adpt_handle->digi_handle,
                                                ge_serdes);

    PMC_RETURN(rc);
} /* adpt_serdes_ge_tx_err_inst_set */


/*******************************************************************************
* adpt_serdes_ge_rx_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves pattern monitor status and the number of errors detected by 
*   OHP/RI_RCP/DI_RCP/EMGMT port.
*
* INPUTS:
*   dev_id             - device identity.
*   ge_serdes          - port type.
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
PUBLIC PMC_ERROR adpt_serdes_ge_rx_state_get(UINT32 dev_id,
                                         digi_ge_serdes_id_t ge_serdes)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    util_patt_status_t mon_status = LAST_UTIL_PATT_STATUS;
    UINT32 err_cnt = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x] ge_serdes[%d].\r\n", dev_id, ge_serdes);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get PRBS status */
    rc = digi_serdes_ge_patt_mon_err_count_get(adpt_handle->digi_handle,
                                               ge_serdes,
                                               &mon_status,
                                               &err_cnt);

    printf("GE SerDes[%s]: Status = [%s] Error count = [0x%x].\r\n", 
           adpt_ge_serdes_type_str(ge_serdes), adpt_prbs_status_str(mon_status), 
           err_cnt);

    PMC_RETURN(rc);
} /* adpt_serdes_ge_rx_state_get */


/*******************************************************************************
* adpt_serdes_ge_polarity_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures GE(SGMII) serdes polarity inversion of OHP/RI_RCP/DI_RCP/EMGMT port.
*
* INPUTS:
*   dev_id             - device identity.
*   dir                - traffic direction: Tx, Rx, Rx and Tx.
*   ge_serdes          - port type.
*   enable             - TRUE: enable the polarity inversion.
*                        FALSE: disable the polarity inversion.
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
PUBLIC PMC_ERROR adpt_serdes_ge_polarity_set(UINT32 dev_id,
                                        digi_serdes_dir_t dir,
                                        digi_ge_serdes_id_t ge_serdes,
                                        BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, dir, ge_serdes, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc |= digi_serdes_ge_polarity_cfg(adpt_handle->digi_handle,
                                      1,
                                      (digi_ge_serdes_id_t*)&ge_serdes,
                                      (util_global_direction_t)dir,
                                      enable);

    PMC_RETURN(rc);
} /* adpt_serdes_ge_polarity_set */


/*******************************************************************************
* adpt_serdes_start_adaptation
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to start adaptation on the specified 
*   SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   serdes_if          - SERDES interface to configure.
*   lane_0_19_msk      - Bitmask of lanes 0-19 to configure. 
*                        Bit 0 corresponds to lane 0.
*   lane_20_39_msk     - Bitmask of lanes 20-39 to configure. 
*                        Bit 0 corresponds to lane 20.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_start_adaptation(UINT32 dev_id,  
                                         hostmsg_adapt_intf_type_t serdes_if,
                                         UINT32 lane_0_19_msk,
                                         UINT32 lane_20_39_msk)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               serdes_if, lane_0_19_msk, lane_20_39_msk);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_start_adaptation(adpt_handle->digi_handle,
                                      serdes_if,
                                      lane_0_19_msk,
                                      lane_20_39_msk);

    PMC_RETURN(rc);
}/* adpt_serdes_start_adaptation */


/*******************************************************************************
* adpt_serdes_adapt_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to start adaptation on the specified 
*   SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *serdes_status     - pointer to storage for returned status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_adapt_status_get(UINT32 dev_id,  
                                         serdes_adapt_status_t *serdes_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, serdes_status);

    /* check parameter validation */
    ADPT_PARA_VERIFY(serdes_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_adapt_status_get(adpt_handle->digi_handle,
                                      serdes_status);

    PMC_RETURN(rc);
}/* adpt_serdes_adapt_status_get */


/*******************************************************************************
* adpt_serdes_cfg_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually set serdes configuration.
*
* INPUTS:
*   dev_id             - device identity.
*   *serdes_cfg        - pointer to serdes configuraiton.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_cfg_set(UINT32 dev_id, digi_serdes_cfg_t *serdes_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, serdes_cfg);

    /* check parameter validation */
    ADPT_PARA_VERIFY(serdes_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_PARA_VERIFY(adpt_handle);

    rc = digi_serdes_cfg_set(adpt_handle->digi_handle, serdes_cfg);

    PMC_RETURN(rc);
}/* adpt_serdes_cfg_set */


/*******************************************************************************
* adpt_serdes_cfg_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to retrieve serdes configuration.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *serdes_cfg        - pointer to serdes configuraiton.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_cfg_get(UINT32 dev_id, digi_serdes_cfg_t *serdes_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, serdes_cfg);

    /* check parameter validation */
    ADPT_PARA_VERIFY(serdes_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_cfg_get(adpt_handle->digi_handle, serdes_cfg);

    PMC_RETURN(rc);
}/* adpt_serdes_cfg_get */


/*******************************************************************************
* adpt_serdes_cfg_file_parse
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually set serdes configuration through
*   special file.
*
* INPUTS:
*   dev_id             - device identity.
*   *file_name         - file name.
*   *serdes_cfg        - pointer to serdes configuraiton.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_cfg_file_parse(UINT32 dev_id, 
                                       char *file_name,
                                       digi_serdes_cfg_t *serdes_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p][%p].\r\n", dev_id, file_name, serdes_cfg);

    /* check parameter validation */
    ADPT_PARA_VERIFY(file_name);
    ADPT_PARA_VERIFY(serdes_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_PARA_VERIFY(adpt_handle);

    rc = digi_serdes_cfg_file_parse(adpt_handle->digi_handle, 
                                    file_name,
                                    serdes_cfg);

    PMC_RETURN(rc);
}/* adpt_serdes_cfg_file_parse */


/*******************************************************************************
* adpt_serdes_cfg_file_save
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Save serdes adapatation parameters in a CSV file in which each line defines:
*   Serdes Interface,Rate,Lane,dp_ffe_a,dp_ffe_b,dp_ffe_c,tr_ffe_a,tr_ffe_b,
*   dfe_coef0,dfe_coef1,dfe_coef2,dfe_coef3,dfe_coef4,dfe_coef5,pga_gain,
*   pga_offset,target_amp,adc_map_scale
*
*   CSV file first contains the colums header.
*
* INPUTS:
*   dev_id             - device identity.
*   *file_name         - file name.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_cfg_file_save(UINT32 dev_id, const char *file_name)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, file_name);

    /* check parameter validation */
    ADPT_PARA_VERIFY(file_name);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    digi_serdes_cfg_file_save(adpt_handle->digi_handle, file_name);

    PMC_RETURN(rc);
}/* adpt_serdes_cfg_file_save */


/*******************************************************************************
* adpt_serdes_c8_values_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually write adaptation values
*   for C8 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - lane to configure (0-7).
*   *c8_values         - pointer to adaptation values to write.
*                        Note that for dp_ffe_a, the least 
*                        significant 2 bits (1:0) will be zeroed
*                        when writing to the SERDES.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_c8_values_set(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_c8_adapt_objects_t *c8_values)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, lane_id, c8_values);

    /* check parameter validation */
    ADPT_PARA_VERIFY(c8_values);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

#if 0
    rc = digi_serdes_c8_write_adaptation_values(adpt_handle->digi_handle,
                                                lane_id,
                                                c8_values);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_c8_values_set */


/*******************************************************************************
* adpt_serdes_c8_values_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to read adaptation values for C8 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - lane to configure (0-7).
*
* OUTPUTS:
*   *c8_values         - pointer to adaptation values to write.
*                        Note that for dp_ffe_a, the least 
*                        significant 2 bits (1:0) will be zeroed
*                        when writing to the SERDES.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_c8_values_get(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_c8_adapt_objects_t *c8_values)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, lane_id, c8_values);

    /* check parameter validation */
    ADPT_PARA_VERIFY(c8_values);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

#if 0
    rc = digi_serdes_c8_read_adaptation_values(adpt_handle->digi_handle,
                                               lane_id,
                                               c8_values);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_c8_values_get */


/*******************************************************************************
* adpt_serdes_t8_values_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually write adaptation values
*   for T8 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - Lane to configure (0-39).
*   *t8_values         - Pointer to adaptation values to write.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_t8_values_set(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_t8_adapt_objects_t *t8_values)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, lane_id, t8_values);

    /* check parameter validation */
    ADPT_PARA_VERIFY(t8_values);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

#if 0
    rc = digi_serdes_t8_write_adaptation_values(adpt_handle->digi_handle,
                                                lane_id,
                                                t8_values);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_t8_values_set */


/*******************************************************************************
* adpt_serdes_t8_values_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to read adaptation values for T8 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - lane to configure (0-39).
*
* OUTPUTS:
*   *t8_values         - pointer to adaptation values to write.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_t8_values_get(UINT32 dev_id,  
                                       UINT32 lane_id,
                                       serdes_t8_adapt_objects_t *t8_values)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, lane_id, t8_values);

    /* check parameter validation */
    ADPT_PARA_VERIFY(t8_values);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

#if 0
    rc = digi_serdes_t8_read_adaptation_values(adpt_handle->digi_handle,
                                               lane_id,
                                               t8_values);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_t8_values_get */


/*******************************************************************************
* adpt_serdes_s16_values_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to manually write adaptation values
*   for S16 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   if_type            - S16 serdes interface type. 
*   lane_id            - Lane to configure (0:11 for Line S16,
*                        0:15 for System S16).
*   *s16_values        - Pointer to adaptation values to write.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_s16_values_set(UINT32 dev_id,  
                                        hostmsg_s16_intf_type_t if_type,
                                        UINT32 lane_id,
                                        serdes_s16_adapt_objects_t *s16_values)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, if_type,
               lane_id, s16_values);

    /* check parameter validation */
    ADPT_PARA_VERIFY(s16_values);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

#if 0
    rc = digi_serdes_s16_write_adaptation_values(adpt_handle->digi_handle,
                                                 if_type,
                                                 lane_id,
                                                 s16_values);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_s16_values_set */


/*******************************************************************************
* adpt_serdes_s16_values_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to read adaptation values
*   for S16 SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   if_type            - S16 serdes interface type. 
*   lane_id            - Lane to configure (0:11 for Line S16,
*                        0:15 for System S16).
*
* OUTPUTS:
*   *s16_values        - Pointer to adaptation values to write.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_s16_values_get(UINT32 dev_id,  
                                        hostmsg_s16_intf_type_t if_type,
                                        UINT32 lane_id,
                                        serdes_s16_adapt_objects_t *s16_values)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%p].\r\n", dev_id, lane_id, 
               if_type, s16_values);

    /* check parameter validation */
    ADPT_PARA_VERIFY(s16_values);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

#if 0
    rc = digi_serdes_s16_read_adaptation_values(adpt_handle->digi_handle,
                                                if_type,
                                                lane_id,
                                                s16_values);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_s16_values_get */


/*******************************************************************************
* adpt_serdes_debug_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Sends host message to firmware to configure debug variables for SERDES.
*
* INPUTS:
*   dev_id             - device identity.
*   skip_cfg           - TRUE: Skip SERDES configuration when processing
*                              SERDES host messages.
*                        FALSE: Perform SERDES configuration when processing
*                               SERDES host messages (default).
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_debug_cfg(UINT32 dev_id, BOOL skip_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, skip_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_debug_cfg(adpt_handle->digi_handle, skip_cfg);

    PMC_RETURN(rc);
}/* adpt_serdes_debug_cfg */


/*******************************************************************************
* adpt_serdes_debug_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves debug variables for SERDES configuration.
*
* INPUTS:
*   dev_id             - device identity.
*   clear_val          - TRUE: Clear debug variables after reading.
*                        FALSE: Do not clear debug variables after reading.
*
* OUTPUTS:
*   *serdes_status     - pointer to storage for returned status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_debug_status_get(UINT32 dev_id, 
                                          BOOL clear_val,
                                          serdes_debug_t *serdes_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, clear_val, serdes_status);

    /* check parameter validation */
    ADPT_PARA_VERIFY(serdes_status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_debug_status_get(adpt_handle->digi_handle, 
                                      serdes_status,
                                      clear_val);

    PMC_RETURN(rc);
}/* adpt_serdes_debug_status_get */


/*******************************************************************************
* adpt_serdes_s16_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Function sets TX swing and de-emphasis levels for a S16 serdes.
*   TX swing and de-emphasis levels are also saved in serdes context after
*   values are successfully written in the device registers.
*
* INPUTS:
*   dev_id             - device identity.
*   s16_intf           - S16 interface: LINE or SYSTEM
*   lane_id            - lane identity to configure.
*   amplitude          - Tx launch amplitude for calibration.
*   post_cursor        - Tx de-emphasis for calibration.
*   pre_cursor         - Tx edge rate for calibration.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_s16_tx_swing_set(UINT32 dev_id,  
                                          digi_serdes_intf_t s16_intf,
                                          UINT32 lane_id,
                                          UINT8 amplitude,
                                          UINT8 post_cursor,
                                          UINT8 pre_cursor)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, s16_intf, 
               lane_id, amplitude, post_cursor, pre_cursor);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_s16_tx_swing_emphasis_set(adpt_handle->digi_handle,
                                               s16_intf, 
                                               lane_id,  
                                               amplitude,  
                                               post_cursor,  
                                               pre_cursor);

    PMC_RETURN(rc);
}/* adpt_serdes_s16_tx_swing_set */


/*******************************************************************************
* adpt_serdes_c8_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Function sets TX swing and de-emphasis levels for a C8 serdes.
*   TX swing and de-emphasis levels are also saved in serdes context after
*   values are successfully written in the device registers.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - lane identity to configure.
*   tx_dpeni1          - rirst pre-emphasis driver stages state.
*   tx_dpeni2          - second pre-emphasis driver stages state.
*   deni               - output driver stages state.
*   tx_psel            - Tx slave selector.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_c8_tx_swing_set(UINT32 dev_id,  
                                         UINT32 lane_id,
                                         UINT8 tx_dpeni1,
                                         UINT8 tx_dpeni2,
                                         UINT16 deni,
                                         UINT32 tx_psel)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               lane_id, tx_dpeni1, tx_dpeni2, deni, tx_psel);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_c8_tx_swing_emphasis_set(adpt_handle->digi_handle, 
                                              lane_id, 
                                              tx_dpeni1, 
                                              tx_dpeni2, 
                                              deni, 
                                              tx_psel);

    PMC_RETURN(rc);
}/* adpt_serdes_c8_tx_swing_set */


/*******************************************************************************
* adpt_serdes_t8_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Function sets TX swing and de-emphasis levels for a S16 serdes.
*   TX swing and de-emphasis levels are also saved in serdes context after
*   values are successfully written in the device registers.
*
* INPUTS:
*   dev_id             - device identity.
*   lane_id            - lane identity to configure.
*   tx_dpeni1          - first pre-emphasis driver stages state.
*   tx_dpeni2          - second pre-emphasis driver stages state.
*   deni               - output driver stages state.
*   tx_psel            - Tx slave selector.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_t8_tx_swing_set(UINT32 dev_id,  
                                         UINT32 lane_id,
                                         UINT8 tx_dpeni1,
                                         UINT8 tx_dpeni2,
                                         UINT32 deni,
                                         UINT32 tx_psel)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, lane_id, 
               tx_dpeni1, tx_dpeni2, deni, tx_psel);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_t8_tx_swing_emphasis_set(adpt_handle->digi_handle, 
                                              lane_id, 
                                              tx_dpeni1,
                                              tx_dpeni2,
                                              deni, 
                                              tx_psel);

    PMC_RETURN(rc);
}/* adpt_serdes_t8_tx_swing_set */


/*******************************************************************************
* adpt_serdes_sfi51_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Function sets SFI51 bus's serdes TX swing level.
*   TX swing level is also saved in serdes context after
*   value is successfully written in the device register.
*
* INPUTS:
*   dev_id             - device identity.
*   bus_id             - SFI51 bus identifier
*   lane_id            - Serdes lane to configure. Legal range 0 to 16.
*   tx_swing_level     - TX swing new value. Possible values are:
*                           D8_TX_2X_TX_SWING_LEVEL_326     =  326 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_372     =  372 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_419     =  419 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_465     =  465 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_511     =  511 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_557     =  557 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_603     =  603 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_648     =  648 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_693     =  693 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_737     =  737 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_781     =  781 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_825     =  825 mVppd (Default value)
*                           D8_TX_2X_TX_SWING_LEVEL_868     =  868 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_910     =  910 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_952     =  952 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_992     =  992 mVppd
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_sfi51_tx_swing_set(UINT32 dev_id,  
                                           UINT32 bus_id,
                                           UINT32 lane_id,
                                           d8_tx_2x_tx_swing_level_t tx_swing_level)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, bus_id, 
               lane_id, tx_swing_level);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_sfi51_tx_swing_set(adpt_handle->digi_handle, 
                                        bus_id, 
                                        lane_id,
                                        tx_swing_level);

    PMC_RETURN(rc);
}/* adpt_serdes_sfi51_tx_swing_set */


/*******************************************************************************
* adpt_serdes_ge_tx_swing_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Function sets TX swing level for a supporting interface serdes 
*   (OHP, RCP, MGMT_FEGE). TX swing level is also saved in serdes context after
*   value is successfully written in the device register.
*
* INPUTS:
*   dev_id             - device identity.
*   ge_intf            - GE interface associated to serdes
*   tx_swing_level     - Tx swing level new value. Possible values are:
*                           D8_TX_2X_TX_SWING_LEVEL_326     =  326 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_372     =  372 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_419     =  419 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_465     =  465 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_511     =  511 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_557     =  557 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_603     =  603 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_648     =  648 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_693     =  693 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_737     =  737 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_781     =  781 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_825     =  825 mVppd (Default value)
*                           D8_TX_2X_TX_SWING_LEVEL_868     =  868 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_910     =  910 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_952     =  952 mVppd
*                           D8_TX_2X_TX_SWING_LEVEL_992     =  992 mVppd
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_ge_tx_swing_set(UINT32 dev_id,  
                                         digi_ge_serdes_id_t ge_intf,
                                         d8_tx_2x_tx_swing_level_t tx_swing_level)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, ge_intf, tx_swing_level);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_serdes_ge_tx_swing_set(adpt_handle->digi_handle, 
                                     ge_intf,
                                     tx_swing_level);

    PMC_RETURN(rc);
}/* adpt_serdes_ge_tx_swing_set */


/*******************************************************************************
* adpt_serdes_t8_histogram
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Dumps T8 SerDes histogram.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_t8_histogram(UINT32 dev_id)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT32 slice = 0;
    UINT64 t8_buffer[64];
    UINT64 total_point;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x].\r\n", dev_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset FIFO */
    PMC_MEMSET((void*)&t8_buffer, 0, sizeof(UINT64) * 64);
    printf("Phys under test:0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39\r\n"); 

    for (slice = 0; slice < 20; slice++)
    {
        total_point = 0;	
        adpt_t8_vert_hist(dev_id, slice, 1, (UINT64*)&t8_buffer);

        printf("phy %u\r\n", 2 * slice);
        for (index = 0; index < 64; index++)
        {
            printf("bin:%2u,0x%02x,0x%llx,0x0\r\n", index, index, t8_buffer[index]);
                   total_point += t8_buffer[index]; 
        }
        printf("phy %d total = 0x%llx num_bits = 0xFFFFFFFF\r\n", 2 * slice, total_point);

        total_point = 0;
        adpt_t8_vert_hist(dev_id, slice, 0, (UINT64*)&t8_buffer);

        printf("phy %d\r\n", 2 * slice + 1);
        for (index = 0; index < 64; index++)
        {
            printf("bin:%2u,0x%02x,0x%llx,0x0\r\n", index, index, t8_buffer[index]);
                   total_point += t8_buffer[index]; 
        }
        printf("phy %d total = 0x%llx num_bits = 0xFFFFFFFF\r\n", 2 * slice + 1, total_point);
    }

    PMC_RETURN(rc);
}/* adpt_serdes_t8_histogram */


/*******************************************************************************
* adpt_serdes_t8_histogram_per_slice
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Dumps T8 SerDes histogram per slice.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl               - T8 channel.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_t8_histogram_per_slice(UINT32 dev_id, UINT32 chnl) 
{ 
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT64 t8_buffer[64];
    UINT64 total_point = 0;
    UINT32 slice = chnl / 2;
    UINT32 sub_chnl = (chnl + 1) % 2;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, chnl);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset FIFO */
    PMC_MEMSET((void*)&t8_buffer, 0, sizeof(UINT64) * 64);

    printf("Phys under test:%u\r\n", chnl);
    adpt_t8_vert_hist(dev_id, slice, sub_chnl, (UINT64*)&t8_buffer);
    printf("phy %u\r\n", chnl);
    for (index = 0; index < 64; index++)
    { 
        printf("bin:%2u,0x%02x,0x%llx,0x0\r\n", index, index, t8_buffer[index]);
        total_point += t8_buffer[index]; 
    } 
    printf("phy %u total = 0x%llx num_bits = 0xFFFFFFFF\r\n", chnl, total_point);

    PMC_RETURN(rc);
}/* adpt_serdes_t8_histogram_per_slice */ 


/******************************************************************************* 
* adpt_serdes_vert_eye_opening 
* ______________________________________________________________________________ 
* 
* DESCRIPTION: 
*   This function is to get the vertical eye opening height in bin for S16 Line, 
*   S16 SYS and T8 serdes.
*   
* INPUTS:
*   dev_id             - device identity.
*   serdes_if          - SERDES interface to configure.
*   lane_msk           - Bitmask of lanes to configure.
*                        S16 Line mask: 0 ~ 11
*                        S16 SYS mask : 0 ~ 15
*                        T8 mask      : 0 ~ 39
*   test_length        - Enum,requested the number of samples to capture
*                        UTIL_VHIST_DEBUG   = 0
*                        UTIL_VHIST_SHORT   = 1
*                        UTIL_VHIST_LONG    = 2
*                        UTIL_VHIST_USERDEF = 3
*   userdef_length     - The vertical histgram counter-timer limit value 
*                        that is defined by user.
*                        This parameter is only valid if test_lenth is set 
*                        to UTIL_VHIST_USERDEF.
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
* 
* NOTES: 
* 
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_vert_eye_opening(UINT32 dev_id,
                                          hostmsg_adapt_intf_type_t serdes_if,
                                          UINT64 lane_msk,
                                          util_vhist_test_length_t test_length,
                                          UINT64 userdef_length)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 s16_line_lane_0_11_msk = 0; 
    UINT32 s16_sys_lane_0_15_msk = 0;
    UINT32 t8_lane_0_19_msk = 0;
    UINT32 t8_lane_20_39_msk = 0;
    digi_bin_buffer_t *bin_buffer = NULL;
    UINT32 s16_line_port_verteye_opening[12];
    UINT32 s16_sys_port_verteye_opening[16];
    UINT32 t8_port_verteye_opening[40];
    UINT32 index = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%llu][%d][%llu].\r\n", dev_id, 
               serdes_if, lane_msk, test_length, userdef_length);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    switch (serdes_if) 
    {
        case HOSTMSG_ADAPT_INTF_S16_LINE:
            s16_line_lane_0_11_msk = lane_msk & 0xFFF;
            break;

        case HOSTMSG_ADAPT_INTF_S16_SYS:
            s16_sys_lane_0_15_msk = lane_msk & 0xFFFF;
            break;

        case HOSTMSG_ADAPT_INTF_T8:
            t8_lane_0_19_msk  = lane_msk & 0xFFFFF;
            t8_lane_20_39_msk = (lane_msk >> 20) & 0xFFFFF;
            break;

        default :
            ADPT_TRACE("Input wrong serdes type[%d].\r\n", serdes_if);
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
            break;
    }

    bin_buffer = (digi_bin_buffer_t*)PMC_CALLOC(sizeof(digi_bin_buffer_t));
    ADPT_PARA_VERIFY(bin_buffer);

    rc = digi_serdes_vert_hist_get(adpt_handle->digi_handle,
                                   s16_line_lane_0_11_msk, 
                                   s16_sys_lane_0_15_msk, 
                                   t8_lane_0_19_msk, 
                                   t8_lane_20_39_msk, 
                                   test_length, 
                                   userdef_length, 
                                   bin_buffer->s16_line_bin_buffer, 
                                   bin_buffer->s16_sys_bin_buffer, 
                                   bin_buffer->t8_bin_buffer); 

    if (PMC_SUCCESS == rc)
    {
        digi_serdes_vert_eye_opening_calc(adpt_handle->digi_handle, 
                                          s16_line_lane_0_11_msk, 
                                          s16_sys_lane_0_15_msk, 
                                          t8_lane_0_19_msk, 
                                          t8_lane_20_39_msk,
                                          bin_buffer->s16_line_bin_buffer, 
                                          bin_buffer->s16_sys_bin_buffer, 
                                          bin_buffer->t8_bin_buffer,
                                          s16_line_port_verteye_opening,
                                          s16_sys_port_verteye_opening, 
                                          t8_port_verteye_opening);

        /* S16 Line serdes lanes */
        for (index = 0; index < DIGI_SERDES_XFI_LANES_MAX; index++)
        {
            if (((s16_line_lane_0_11_msk >> index) & 0x1) != 0) 
            {
                printf("S16 Line serdes lane %d eye opening bin number: %d\r\n", 
                       index, s16_line_port_verteye_opening[index]);
            }
        }

        /* S16 SYS serdes lanes */
        for (index = 0; index < DIGI_SERDES_MAX_S16_IN_BP; index++) 
        {
            if (((s16_sys_lane_0_15_msk >> index) & 0x1) != 0) 
            {
                printf("S16 SYS serdes lane %d eye opening bin number: %d\r\n", 
                       index, s16_sys_port_verteye_opening[index]);
            }
        }

        /* T8 backplane serdes lanes */
        for (index = 0; index < (DIGI_SERDES_MAX_T8_E1_IN_BP_LR / 2); index++) 
        {
            if (((t8_lane_0_19_msk >> index) & 0x1) != 0) 
            {
                printf("T8 backplane serdes lane %d eye opening bin number: %d\r\n", 
                       index, t8_port_verteye_opening[index]);
            }
        }

        for (index = 0; index < (DIGI_SERDES_MAX_T8_E1_IN_BP_LR / 2); index++) 
        {
            if (((t8_lane_20_39_msk >> index) & 0x1) != 0) 
            {
                printf("T8 backplane serdes lane %d eye opening bin number: %d\r\n",
                       index + 20, t8_port_verteye_opening[index + 20]);
            }
        }
    }

    /* free memory */
    PMC_FREE(&bin_buffer);

    PMC_RETURN(rc);
}/* adpt_serdes_vert_eye_opening */


/*******************************************************************************
* adpt_serdes_sfi51_tx_dclk_en
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function is to enable the SFI1_TXCKSRC_P/IN or SFI2_TXCKSRC_P/N on a 
*   SFI5.1/D8 serdes interface. For power saving reasons, this clock is not 
*   enabled by default. The correct physical resource will be modified appropriately 
*   depending on the SFI5.1 port_ctxt_ptr.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable the SFI1_TXCKSRC_P/IN or SFI2_TXCKSRC_P/N.
*                        FALSE: disable the SFI1_TXCKSRC_P/IN or SFI2_TXCKSRC_P/N.
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
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_sfi51_tx_dclk_en(UINT32 dev_id, 
                                         UINT32 chnl_id,
                                         BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT32 serdes_port = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get serdes port */
    serdes_port = DIGI_SERDES_PORT_GET(chnl_id);
    if (serdes_port >= DIGI_SERDES_XFI_LANES_MAX)
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

#if 0
    rc = digi_serdes_sfi51_tx_dclk_enable(adpt_handle->digi_handle,
                                          adpt_handle->line_serdes[serdes_port],
                                          (BOOL8)enable);
#endif

    PMC_RETURN(rc);
}/* adpt_serdes_sfi51_tx_dclk_en */


/*******************************************************************************
* adpt_serdes_tx_refclk_div_cfg
* ___________________________________________________________________________
*
* DESCRIPTION:
*   This function configures transmit reference clock divider (DIV16 or DIV64) 
*   for one or multiple serdes ports associated to a serdes port: 
*     -Line interface (S16) 
*     -System Interface (S16) 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   lane_msk           - the logical lane mask associated to serdes.
*   clk_divider        - clock divider (16 or 64) 
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
*   Please note that SFI5.1 isn't supported in this API.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_tx_refclk_div_cfg(UINT32 dev_id, 
                                         UINT32 chnl_id,
                                         UINT32 lane_msk,
                                         digi_tx_clk_div_t clk_divider)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 index = 0;
    UINT32 serdes_port = 0;
    UINT32 traffic_loc = 0;
    digi_serdes_port_t *serdes_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               lane_msk, clk_divider);

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
    }
    else
    {
        serdes_handle = adpt_handle->sys_serdes[serdes_port];
    }
    ADPT_PARA_VERIFY(serdes_handle);

    rc = digi_serdes_port_tx_refclk_div_cfg(adpt_handle->digi_handle,
                                            serdes_handle,
                                            lane_msk,
                                            (digi_s16_clk_divider_t)clk_divider);

    PMC_RETURN(rc);
}/* adpt_serdes_tx_refclk_div_cfg */


/*******************************************************************************
* adpt_serdes_port_recfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the serdes port again to initialize a 10G/40G/100G serdes port. This 
*   API is usually used for a dynamic switch of 10G/40G/100G serdes ports. For 
*   example, it should be called to initialize the serdes port when switching 
*   a 100G port to a 40G port. Before that, the adpt_traffic_mode_cfg() should be
*   called to disable the traffic mode of this port.
*
* INPUTS:
*   dev_id             - device identity.
*   serdes_id          - serdes identity.
*   serdes_type        - serdes type: DIGI_SERDES_LINE_INTF and DIGI_SERDES_SYSTEM_INTF.
*   lane_tx[]          - serdes port lane assignment in the Tx direction.
*   lane_rx[]          - serdes port lane assignment in the Rx direction.
*   los_pin            - los pin position.
*                        1. For intf DIGI_SERDES_SFI_51_INTF, this value is ignored.
*                        2. Setting this to a value of DIGI_SERDES_PORT_DLOS_LOS_SOURCE
*                           will set this serdes lane to ignore the LOS pin and use its 
*                           internal DLOS as a LOS source.  
*                        3. Setting this to a value of DIGI_SERDES_PORT_NO_LOS_SOURCE
*                           will disable the LOS feature.
*   op_mode            - operation mode to delete, add or overwrite. For example: 
*                        As to 1x100G <> 1x40G, the operation mode to overwrite is 
*                        suitable. As to 4x10G <> 1x40G, the operation mode to delete 
*                        should be used for all SERDES ports and then the operation 
*                        mode to add is required. 
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
*   Please note that serdes_type/lane_tx/lane_rx/los_pin arguments needn't to
*   be cared when the op_mode is configured as DIGI_OP_DEL.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_serdes_port_recfg(UINT32 dev_id,
                                    UINT32 serdes_id,
                                    digi_serdes_intf_t serdes_type,
                                    UINT32 lane_tx[DIGI_SERDES_XFI_LANES_MAX],
                                    UINT32 lane_rx[DIGI_SERDES_XFI_LANES_MAX],
                                    UINT8 los_pin,
                                    digi_op_mode_t op_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    /* parameter check */
    if ((serdes_id >= DIGI_SERDES_XFI_LANES_MAX)
         || (op_mode >= LAST_DIGI_OP_MODE)
         || ((DIGI_SERDES_LINE_INTF != serdes_type) 
         && (DIGI_SERDES_SYSTEM_INTF != serdes_type)))
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* send the port descriptions for system reservation */
    if (DIGI_SERDES_LINE_INTF == serdes_type)
    {
        if ((DIGI_OP_DEL == op_mode) || (DIGI_OP_OVERWRITE == op_mode))
        {
            rc = digi_serdes_port_uninit(adpt_handle->digi_handle, 
                                         &(adpt_handle->line_serdes[serdes_id]));
            ADPT_RETURN_VERIFY(rc);
        }

        if ((DIGI_OP_ADD == op_mode) || (DIGI_OP_OVERWRITE == op_mode))
        {
            rc = digi_serdes_port_init(adpt_handle->digi_handle, 
                                       serdes_id, 
                                       serdes_type, 
                                       lane_tx,
                                       lane_rx,
                                       los_pin,
                                       adpt_handle->dev_info->line_port_los_inv,
                                       NULL,
                                       &(adpt_handle->line_serdes[serdes_id]));
            ADPT_RETURN_VERIFY(rc);
        }
    }
    else
    {
        if ((DIGI_OP_DEL == op_mode) || (DIGI_OP_OVERWRITE == op_mode))
        {
            rc = digi_serdes_port_uninit(adpt_handle->digi_handle, 
                                         &(adpt_handle->sys_serdes[serdes_id]));
            ADPT_RETURN_VERIFY(rc);
        }

        if ((DIGI_OP_ADD == op_mode) || (DIGI_OP_OVERWRITE == op_mode))
        {
            rc = digi_serdes_port_init(adpt_handle->digi_handle, 
                                       serdes_id, 
                                       serdes_type, 
                                       lane_tx,
                                       lane_rx,
                                       los_pin,
                                       adpt_handle->dev_info->sys_port_los_inv,
                                       NULL,
                                       &(adpt_handle->sys_serdes[serdes_id]));
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_serdes_port_recfg */


/*
** End of file
*/
