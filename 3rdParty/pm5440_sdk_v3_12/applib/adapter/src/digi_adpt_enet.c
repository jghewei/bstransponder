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
*     The file describes the Ethernet Run-Time configuration functionality.
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
* adpt_enet_mru_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   configures the maximum receive unit of Ethernet packet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   max_length         - max length of received packet.
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
PUBLIC PMC_ERROR adpt_enet_mru_set(UINT32  dev_id,
                                 UINT32 chnl_id,
                                 UINT32 max_length)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, max_length);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_mru_set(digi_handle, chnl_handle, max_length);

    PMC_RETURN(rc);
} /* adpt_enet_mru_set */


/*******************************************************************************
* adpt_enet_mru_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the maximum receive unit of Ethernet packet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *max_length        - max length of received packet.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_mru_get(UINT32  dev_id,
                                 UINT32 chnl_id,
                                 UINT32 *max_length)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(max_length);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_mru_get(digi_handle, chnl_handle, max_length);

    PMC_RETURN(rc);
} /* adpt_enet_mru_get */


/*******************************************************************************
* adpt_enet_ipg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   configures the IPG(Inter Packet Gap) length of Ethernet packet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ipg_length         - IPG length of Ethernet packet.
*   ipg_length         - For 10G link:
*                          it is the number of octets in step of 4. Valid values 
*                          are 8, 12, 16, ... 100. 
*                          A default of 12 must be set to conform to IEEE802.3ae. 
*                          Note: When set to 8, PCS layers may not be able to 
*                          perform clock rate compensation.
*                        For 40G and 100G link:
*                          The function sets the transmit PPM compensation.
*                          When gap == 2'b00 : Normal mode of operation. Markers 
*                                              compensation active meaning 
*                                              effective +60ppm.
*                          When gap == 2'b01 : Default compensation plus 60ppm 
*                                              (total of +120ppm).
*                          When gap == 2'b10 : Default compensation plus 140ppm 
*                                              (total of +200ppm).
*                          When gap == 2'b11 : Default compensation plus 200ppm 
*                                              (total of +260ppm).
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
PUBLIC PMC_ERROR adpt_enet_ipg_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                UINT32 ipg_length)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, ipg_length);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_ipg_set(digi_handle, chnl_handle, ipg_length);

    PMC_RETURN(rc);
} /* adpt_enet_ipg_set */


/*******************************************************************************
* adpt_enet_ipg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the IPG(Inter Packet Gap) length of Ethernet packet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *ipg_length        - IPG length of Ethernet packet.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_ipg_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                UINT32 *ipg_length)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(ipg_length);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_ipg_get(digi_handle, chnl_handle, ipg_length);

    PMC_RETURN(rc);
} /* adpt_enet_ipg_get */


/*******************************************************************************
* adpt_enet_crc_fwd_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the CRC forwarding of the provisioned channel.
*
*   Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable CRC forwarding.
*                        FALSE: disable CRC forwarding.
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
PUBLIC PMC_ERROR adpt_enet_crc_fwd_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_crc_forwarding_set(digi_handle, chnl_handle, enable);

    PMC_RETURN(rc);
} /* adpt_enet_crc_fwd_set */


/*******************************************************************************
* adpt_enet_crc_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the CRC forwarding status of the provisioned channel.
*
*   Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: enable CRC forwarding.
*                        FALSE: disable CRC forwarding.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_crc_fwd_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_crc_forwarding_get(digi_handle, chnl_handle, enable);

    PMC_RETURN(rc);
} /* adpt_enet_crc_fwd_get */


/*******************************************************************************
* adpt_enet_crc_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the CRC insertion for traffic from the CPB of the provisioned 
*   channel.
*
*   Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable CRC insertion.
*                        FALSE: disable CRC insertion.
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
PUBLIC PMC_ERROR adpt_enet_crc_inst_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_insert_crc_set(digi_handle, chnl_handle, enable);

    PMC_RETURN(rc);
} /* adpt_enet_crc_inst_set */


/*******************************************************************************
* adpt_enet_crc_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the CRC insertion status for traffic from the CPB of the provisioned 
*   channel.
*
*   Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: enable CRC insertion.
*                        FALSE: disable CRC insertion.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_crc_inst_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_insert_crc_get(digi_handle, chnl_handle, enable);

    PMC_RETURN(rc);
} /* adpt_enet_crc_inst_get */


/*******************************************************************************
* adpt_enet_tx_timing_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Ethernet timing mode for a specified channel.
*
*  Timing modes are only configurable for MAC terminated datapath. The following 
*  timing modes are supported:
*   - DIGI_ENET_TX_NOMINAL_TIMING_MODE  : the default mode
*   - DIGI_ENET_TX_LOOP_TIMING_MODE     : 
*   - DIGI_ENET_TX_LINE_TIMING_MODE     : digi_m1_rate_monitor_cfg_set shall be 
*                                         used to configure the rate monitor in 
*                                         the DIGI_M1 module.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mode               - Ethernet timing mode.
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
*   Please note this API can be available only if CPB switch has been created.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_tx_timing_mode_set(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_tx_timing_mode_t mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, mode);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_tx_timing_mode_set(digi_handle, chnl_handle, mode);

    PMC_RETURN(rc);
} /* adpt_enet_tx_timing_mode_set */


/*******************************************************************************
* adpt_enet_tx_timing_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Ethernet timing mode for a specified channel.
*
*  Timing modes are only configurable for MAC terminated datapath. The following 
*  timing modes are supported:
*   - DIGI_ENET_TX_NOMINAL_TIMING_MODE  : the default mode
*   - DIGI_ENET_TX_LOOP_TIMING_MODE     : 
*   - DIGI_ENET_TX_LINE_TIMING_MODE     : digi_m1_rate_monitor_cfg_set shall be 
*                                         used to configure the rate monitor in 
*                                         the DIGI_M1 module.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *mode              - Ethernet timing mode.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_tx_timing_mode_get(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_tx_timing_mode_t *mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_tx_timing_mode_get(digi_handle, chnl_handle, mode);

    PMC_RETURN(rc);
} /* adpt_enet_tx_timing_mode_get */


/*******************************************************************************
* adpt_enet_rx_fc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the flow control function of Ethernet packet in Rx
*   direction.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   fc_enable          - TRUE: enable flow control.
*                        FALSE: disable flow control.
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
PUBLIC PMC_ERROR adpt_enet_rx_fc_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 BOOL   fc_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, fc_enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_rx_flow_control_set(digi_handle, chnl_handle, fc_enable);

    PMC_RETURN(rc);
} /* adpt_enet_rx_fc_set */


/*******************************************************************************
* adpt_enet_rx_fc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of flow control function of Ethernet 
*   packet in Rx direction.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *fc_enable         - TRUE: enable flow control.
*                        FALSE: disable flow control.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_rx_fc_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 BOOL   *fc_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL8 enable = FALSE;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(fc_enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_rx_flow_control_get(digi_handle, chnl_handle, &enable);
    *fc_enable = enable ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_enet_rx_fc_get */


/*******************************************************************************
* adpt_enet_tx_fc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/Disables Link Ethernet TX flow control of a provisioned channel.
*
*   When CPB flow control is enabled, the pause quanta should also be
*   configured.  The pause quanta is sent out in the PAUSE control
*   frame.  The recommended value to use is 0xFFF.
*   Only datapaths through the MAC block can have CPB flow control.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   pause_quanta       - pause quanta for CPB flow control
*   pause_quanta_thr   - pause quanta threshold for CPB flow control
*                        It shall be superior or equal to 2. 
*   fc_thresh          - XON/XOFF threshold.
*   fc_enable          - TRUE: enable CPB flow control.
*                        FALSE: disable CPB flow control.
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
PUBLIC PMC_ERROR adpt_enet_tx_fc_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 pause_quanta,
                                 UINT32 pause_quanta_thr,
                                 UINT32 fc_thresh,
                                 BOOL fc_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_enet_link_pause_cfg_t link_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, pause_quanta, pause_quanta_thr, fc_thresh, fc_enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&link_cfg, 0, sizeof(digi_enet_link_pause_cfg_t));
    link_cfg.pause_quanta = pause_quanta;
    link_cfg.pause_quanta_thresh = pause_quanta_thr;
    link_cfg.buffer_mode = DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL;
    link_cfg.xon_xoff_threshold = fc_thresh;

    rc = digi_enet_tx_flow_control_set(digi_handle, 
                                       chnl_handle, 
                                       &link_cfg, 
                                       fc_enable);

    PMC_RETURN(rc);
} /* adpt_enet_tx_fc_set */


/*******************************************************************************
* adpt_enet_tx_fc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of  Link Ethernet TX flow control of 
*   a provisioned channel.
*
*   When CPB flow control is enabled, the pause quanta should also be
*   configured.  The pause quanta is sent out in the PAUSE control
*   frame.  The recommended value to use is 0xFFF.
*   Only datapaths through the MAC block can have CPB flow control.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *pause_quanta      - pause quanta for CPB flow control
*   *pause_quanta_thr  - pause quanta threshold for CPB flow control
*                        It shall be superior or equal to 2.  
*   *fc_thresh         - XON/XOFF threshold.
*   *fc_enable         - TRUE: enable CPB flow control.
*                        FALSE: disable CPB flow control.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_tx_fc_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 *pause_quanta,
                                 UINT32 *pause_quanta_thr,
                                 UINT32 *fc_thresh,
                                 BOOL *fc_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_enet_link_pause_cfg_t link_cfg;
    BOOL8 enable = FALSE;
    PMC_ENTRY();

    /* parameter verification */
    ADPT_PARA_VERIFY(pause_quanta);
    ADPT_PARA_VERIFY(pause_quanta_thr);
    ADPT_PARA_VERIFY(fc_thresh);
    ADPT_PARA_VERIFY(fc_enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    PMC_MEMSET((void*)&link_cfg, 0, sizeof(digi_enet_link_pause_cfg_t));

    rc = digi_enet_tx_flow_control_get(digi_handle,
                                       chnl_handle,
                                       &link_cfg,
                                       &enable);
    *pause_quanta = link_cfg.pause_quanta;
    *pause_quanta_thr = link_cfg.pause_quanta_thresh;
    *fc_thresh = link_cfg.xon_xoff_threshold;
    *fc_enable = enable ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_enet_tx_fc_get */


/*******************************************************************************
* adpt_enet_fc_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of flow control function of Ethernet 
*   packet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *rx_pause_frm      - TRUE: pause frame received.
*                        FALSE: pause frame not received.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_fc_status_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL   *rx_pause_frm)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(rx_pause_frm);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_flow_control_status_get(digi_handle,
                                           chnl_handle,
                                           rx_pause_frm);

    PMC_RETURN(rc);
} /* adpt_enet_fc_status_get */


/*******************************************************************************
* adpt_enet_phy_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables the physical PCS(Physical Coding Sublayer) layer of 
*   Ethernet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   phy_enable         - TRUE: enable physical PCS.
*                        FALSE: disable physical PCS.
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
PUBLIC PMC_ERROR adpt_enet_phy_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL   phy_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, phy_enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_phy_set(digi_handle, chnl_handle, phy_enable);

    PMC_RETURN(rc);
} /* adpt_enet_phy_en_set */


/*******************************************************************************
* adpt_enet_phy_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the enable/disable status of physical PCS(Physical Coding Sublayer)
*   layer of Ethernet.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *phy_enable        - TRUE: enable physical PCS.
*                        FALSE: disable physical PCS.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_phy_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL   *phy_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(phy_enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_phy_status_get(digi_handle, chnl_handle, phy_enable);

    PMC_RETURN(rc);
} /* adpt_enet_phy_en_get */


/*******************************************************************************
* adpt_enet_irig_cfg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the IRIG(Inter Range Instrumentation Group) function, which is 
*   based on device level.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ingress_enable     - enable/disable in ingress direction.
*   ingress_mode       - IRIG mode in ingress direction.
*   egress_enable      - enable/disable in egress direction.
*   egress_mode        - IRIG mode in egress direction.
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
PUBLIC PMC_ERROR adpt_enet_irig_cfg_set(UINT32           dev_id,
                                   UINT32                chnl_id,
                                   BOOL                  ingress_enable,
                                   digi_enet_irig_mode_t ingress_mode,
                                   BOOL                  egress_enable,
                                   digi_enet_irig_mode_t egress_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               ingress_enable, ingress_mode, egress_enable, egress_mode);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_irig_configuration_set(digi_handle,
                                          chnl_handle,
                                          ingress_enable,
                                          ingress_mode,
                                          egress_enable,
                                          egress_mode);

    PMC_RETURN(rc);
} /* adpt_enet_irig_cfg_set */


/*******************************************************************************
* adpt_enet_fault_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the Ethernet local/remote fault insertion of the provisioned 
*   channel.
*
*   This API can be used to specify Unidirectional Ethernet mode (IEEE 802.3 
*   clause 66.3.2.1) for a provided 10G MAC terminated link only, other
*   rates return an error.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - classification direction (ingress/egress).
*   fault_type         - fault type: clear, local fault, remote fault, auto.
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
*   1. As to the MAC datapath, only Tx direction is supported.
*   2. As to the Transparent datapath, the DIGI_ENET_FT_UNIDIR type is invalid 
*      and the DIGI_ENET_FT_AUTO type is valid only for 10GE.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_fault_inst_set(UINT32      dev_id,
                                    UINT32            chnl_id,
                                    digi_port_dir_t   dir,
                                    digi_enet_fault_t fault_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    UINT32 client_mode = 0;
    enet_dir_t direction = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               dir, fault_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    switch (client_mode)
    {
        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
        case DIGI_ENET_SERDES_10GE_TRANSPARENT:
        case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            switch (fault_type)
            {
                case DIGI_ENET_FT_CLEAR:
                    rc = digi_enet_epmm_lf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_enet_epmm_rf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         FALSE);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                case DIGI_ENET_FT_LFI:
                    rc = digi_enet_epmm_rf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_enet_epmm_lf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         TRUE);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                case DIGI_ENET_FT_RFI:
                    rc = digi_enet_epmm_lf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_enet_epmm_rf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         TRUE);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                case DIGI_ENET_FT_AUTO:
                    rc = digi_enet_epmm_lf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_enet_epmm_rf_os_insert_cfg(digi_handle,
                                                         chnl_handle,
                                                         direction,
                                                         FALSE);
                    ADPT_RETURN_VERIFY(rc);

                    rc = digi_enet_epmm_lf_os_set_auto_insert_enable(digi_handle,
                                                                     chnl_handle,
                                                                     direction,
                                                                     TRUE);
                    ADPT_RETURN_VERIFY(rc);
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;

        default:
            /* only Tx direction is supported. */
            if (DIGI_PORT_DIR_TX != dir)
            {
                rc = PMC_ERR_INVALID_PARAMETERS;
                break;
            }

            switch (fault_type)
            {
                case DIGI_ENET_FT_CLEAR:
                    rc = digi_enet_tx_mac_fault_insert_ctrl(digi_handle,
                                                            chnl_handle,
                                                            DIGI_ENET_TX_MAC_FAULT_DISABLED);
                    break;

                case DIGI_ENET_FT_LFI:
                    rc = digi_enet_tx_mac_fault_insert_ctrl(digi_handle,
                                                            chnl_handle,
                                                            DIGI_ENET_TX_MAC_FAULT_FORCE_LF);
                    break;

                case DIGI_ENET_FT_RFI:
                    rc = digi_enet_tx_mac_fault_insert_ctrl(digi_handle,
                                                            chnl_handle,
                                                            DIGI_ENET_TX_MAC_FAULT_FORCE_RF);
                    break;

                case DIGI_ENET_FT_AUTO:
                    rc = digi_enet_tx_mac_fault_insert_ctrl(digi_handle,
                                                            chnl_handle,
                                                            DIGI_ENET_TX_MAC_FAULT_AUTOMATIC);
                    break;

                case DIGI_ENET_FT_UNIDIR:
                    rc = digi_enet_tx_mac_fault_insert_ctrl(digi_handle,
                                                            chnl_handle,
                                                            DIGI_ENET_TX_MAC_FAULT_UNIDIRECTIONAL);
                    break;

                default:
                    rc = PMC_ERR_INVALID_PARAMETERS;
                    break;
            }
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_fault_inst_set */


/*******************************************************************************
* adpt_enet_fault_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of Ethernet local/remote fault insertion.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - classification direction (ingress/egress).
*
* OUTPUTS:
*   *fault_type        - fault type: clear, local fault, remote fault, auto.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   1. As to MAC datapath, only Tx direction is supported.
*   2. As to Transparent datapath, DIGI_ENET_FT_AUTO and DIGI_ENET_FT_UNIDIR 
*      types are invalid, and DIGI_ENET_FT_AUTO is valid only for 10GE.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_fault_inst_get(UINT32      dev_id,
                                    UINT32            chnl_id,
                                    digi_port_dir_t   dir,
                                    digi_enet_fault_t *fault_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    UINT32 client_mode = 0;
    digi_enet_tx_mac_fault_t mac_mode = DIGI_ENET_TX_MAC_FAULT_AUTOMATIC;
    BOOL8 rf_inst = FALSE;
    BOOL8 lf_inst = FALSE;
    BOOL8 auto_inst = FALSE;
    enet_dir_t direction = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(fault_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get client mode */
    client_mode = DIGI_CLIENT_MODE_GET(adpt_handle->traffic_info[index].traffic_mode);

    switch (client_mode)
    {
        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
        case DIGI_ENET_SERDES_10GE_TRANSPARENT:
        case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            rc = digi_enet_epmm_rf_os_insert_cfg_get(digi_handle,
                                                     chnl_handle,
                                                     direction,
                                                     &rf_inst);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_enet_epmm_lf_os_insert_cfg_get(digi_handle,
                                                     chnl_handle,
                                                     direction,
                                                     &lf_inst);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_enet_epmm_lf_os_set_auto_insert_enable_get(digi_handle,
                                                                 chnl_handle,
                                                                 direction,
                                                                 &auto_inst);
            ADPT_RETURN_VERIFY(rc);

            if (lf_inst)
            {
                *fault_type = DIGI_ENET_FT_LFI;
            }
            else if (rf_inst)
            {
                *fault_type = DIGI_ENET_FT_RFI;
            }
            else if (auto_inst)
            {
                *fault_type = DIGI_ENET_FT_AUTO;
            }
            else
            {
                *fault_type = DIGI_ENET_FT_CLEAR;
            }
            break;

        default:
            /* only Tx direction is supported. */
            if (DIGI_PORT_DIR_TX != dir)
            {
                rc = PMC_ERR_INVALID_PARAMETERS;
                break;
            }

            rc = digi_enet_tx_mac_fault_insert_ctrl_get(digi_handle,
                                                        chnl_handle,
                                                        &mac_mode);

            switch (mac_mode)
            {
                case DIGI_ENET_TX_MAC_FAULT_DISABLED:
                    *fault_type = DIGI_ENET_FT_CLEAR;
                    break;

                case DIGI_ENET_TX_MAC_FAULT_FORCE_LF:
                    *fault_type = DIGI_ENET_FT_LFI;
                    break;

                case DIGI_ENET_TX_MAC_FAULT_FORCE_RF:
                    *fault_type = DIGI_ENET_FT_RFI;
                    break;

                case DIGI_ENET_TX_MAC_FAULT_AUTOMATIC:
                    *fault_type = DIGI_ENET_FT_AUTO;
                    break;

                case DIGI_ENET_TX_MAC_FAULT_UNIDIRECTIONAL:
                    *fault_type = DIGI_ENET_FT_UNIDIR;
                    break;

                default:
                    rc = PMC_ERR_FAIL;
                    break;
            }
            break;
    }

    PMC_RETURN(rc);
} /* adpt_enet_fault_inst_get */


/*******************************************************************************
* adpt_enet_test_pkt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the Ethernet test packet generation of the provisioed channel.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   test_pkt           - data structure for test packet.
*   enable             - TRUE: enable test packet.
*                        FALSE: disable test packet.
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
PUBLIC PMC_ERROR adpt_enet_test_pkt_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_enet_test_packet_t *test_pkt,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL org_enable = FALSE;
    digi_enet_test_packet_t org_test_pkt;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p][%d].\r\n", dev_id, 
               chnl_id, test_pkt, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(test_pkt);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_send_packets_get(digi_handle, chnl_handle, &org_enable, &org_test_pkt);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        rc = digi_enet_send_packets_set(digi_handle, chnl_handle, enable, test_pkt);
        ADPT_RETURN_VERIFY(rc);
    }
    else if (enable)
    {
        if (((0 != org_test_pkt.outer_vlan) && (org_test_pkt.inner_vlan != test_pkt->inner_vlan))
            || (org_test_pkt.outer_vlan != test_pkt->outer_vlan)
            || (org_test_pkt.packet_size != test_pkt->packet_size)
            || (org_test_pkt.interval != test_pkt->interval)
            || (org_test_pkt.dest_addr_hi != test_pkt->dest_addr_hi)
            || (org_test_pkt.dest_addr_lo != test_pkt->dest_addr_lo)
            || (org_test_pkt.src_addr_hi != test_pkt->src_addr_hi)
            || (org_test_pkt.src_addr_lo != test_pkt->src_addr_lo)
            || (org_test_pkt.ether_type != test_pkt->ether_type)
            || (org_test_pkt.payload != test_pkt->payload))
        {
            rc = digi_enet_send_packets_set(digi_handle, chnl_handle, FALSE, &org_test_pkt);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_enet_send_packets_set(digi_handle, chnl_handle, enable, test_pkt);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_enet_test_pkt_set */


/*******************************************************************************
* adpt_enet_test_pkt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of Ethernet test packet generation for the provisioed 
*   channel.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *enable            - TRUE: enable test packet.
*                        FALSE: disable test packet.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_test_pkt_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_send_packets_status_get(digi_handle, chnl_handle, enable);

    PMC_RETURN(rc);
} /* adpt_enet_test_pkt_get */


/*******************************************************************************
* adpt_enet_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures PRBS generation of the provisioned channel. 
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   prbs_mode          - PRBS pattern.
*   enable             - TRUE: enable PRBS generation.
*                        FALSE: disable PRBS generation.
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
PUBLIC PMC_ERROR adpt_enet_prbs_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_enet_prbs_t prbs_mode,
                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    util_patt_mode_t org_prbs_mode;
    BOOL8 org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, prbs_mode, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_gen_prbs_get(digi_handle, chnl_handle, &org_enable, &org_prbs_mode);
    ADPT_RETURN_VERIFY(rc);

    if ((adpt_handle->traffic_info[index].prbs_mode != prbs_mode)
         || (!org_enable && enable) || (org_enable && !enable))
    {
        if (org_enable && enable)
        {
            switch (adpt_handle->traffic_info[index].prbs_mode)
            {
                case DIGI_ENET_PRBS_9:
                    rc = digi_enet_gen_prbs_set(digi_handle,
                                                chnl_handle,
                                                FALSE,
                                                UTIL_PATT_MODE_PRBS_9);
                    break;
                    
                case DIGI_ENET_PRBS_31:
                    rc = digi_enet_gen_prbs_set(digi_handle,
                                                chnl_handle,
                                                FALSE,
                                                UTIL_PATT_MODE_PRBS_31);
                    break;
                    
                default:
                    PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                    break;
            }
            ADPT_RETURN_VERIFY(rc);
        }

        switch (prbs_mode)
        {
            case DIGI_ENET_PRBS_9:
                rc = digi_enet_gen_prbs_set(digi_handle,
                                            chnl_handle,
                                            enable,
                                            UTIL_PATT_MODE_PRBS_9);
                break;
                
            case DIGI_ENET_PRBS_31:
                rc = digi_enet_gen_prbs_set(digi_handle,
                                            chnl_handle,
                                            enable,
                                            UTIL_PATT_MODE_PRBS_31);
                break;
                
            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
        ADPT_RETURN_VERIFY(rc);
    }

    rc = digi_enet_mon_prbs_get(digi_handle, chnl_handle, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if ((!org_enable && enable) || (org_enable && !enable))
    {
        rc = digi_enet_mon_prbs_set(digi_handle, chnl_handle, enable);
        ADPT_RETURN_VERIFY(rc);
    }

    /* resync monitor */
    if (enable)
    {
        rc = digi_enet_mon_prbs_resync(digi_handle, chnl_handle);
    }

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->traffic_info[index].prbs_mode = prbs_mode;
    }

    PMC_RETURN(rc);
} /* adpt_enet_prbs_set */


/*******************************************************************************
* adpt_enet_prbs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the status of PRBS generation of the provisioned channel. 
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *prbs_mode         - PRBS pattern.
*   *enable            - TRUE: enable PRBS generation.
*                        FALSE: disable PRBS generation.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_prbs_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_enet_prbs_t *prbs_mode,
                                 BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL8 prbs_en = FALSE;    
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(prbs_mode);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_mon_prbs_get(digi_handle, chnl_handle, &prbs_en);
    ADPT_RETURN_VERIFY(rc);

    *prbs_mode = adpt_handle->traffic_info[index].prbs_mode;
    *enable    = prbs_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_enet_prbs_get */


/*******************************************************************************
* adpt_enet_prbs_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the PRBS monitor status and error counts. 
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *prbs_syn          - TRUE: PRBS synchronization.
*                        FALSE: loss of PRBS synchronization.
*   *err_cnt           - error count.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_prbs_status_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *prbs_syn,
                                       UINT32 *err_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    util_patt_status_t status = LAST_UTIL_PATT_STATUS;
    BOOL8 prbs_en = FALSE;    
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(prbs_syn);
    ADPT_PARA_VERIFY(err_cnt);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_mon_prbs_get(digi_handle, chnl_handle, &prbs_en);
    ADPT_RETURN_VERIFY(rc);

    *prbs_syn = FALSE;
    *err_cnt  = 0;

    if (prbs_en)
    {
        rc = digi_enet_prbs_status_get(digi_handle, chnl_handle, &status);
        ADPT_RETURN_VERIFY(rc);

        *prbs_syn = (UTIL_PATT_STATUS_LOCKED == status) ? TRUE : FALSE;

        switch (adpt_handle->traffic_info[index].prbs_mode)
        {
            case DIGI_ENET_PRBS_9:
                rc = digi_enet_prbs_error_cnt_get(digi_handle,
                                                  chnl_handle,
                                                  UTIL_PATT_MODE_PRBS_9,
                                                  err_cnt);
                break;

            case DIGI_ENET_PRBS_31:
                rc = digi_enet_prbs_error_cnt_get(digi_handle,
                                                  chnl_handle,
                                                  UTIL_PATT_MODE_PRBS_31,
                                                  err_cnt);
                break;

            default:
                PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
                break;
        }
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_enet_prbs_status_get */


/*******************************************************************************
* adpt_enet_cpu_pkt_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables packets insertion from CPU to the ENET LINE or ENET_SYS
*   output interface.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   enable             - TRUE: enable packet insertion from CPU.
*                        FALSE: disable packet insertion from CPU.
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
PUBLIC PMC_ERROR adpt_enet_cpu_pkt_inst_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = ENET_EGRESS;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;

    rc = digi_enet_cpu_insert_pkt_cfg(digi_handle,
                                      enet_dir,
                                      chnl_handle,
                                      (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_enet_cpu_pkt_inst_set */


/*******************************************************************************
* adpt_enet_cpu_pkt_ext_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables packets extraction from ENET LINE or ENET SYS
*   input interface to CPU.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   is_reflect         - TRUE: extract packets using MCPB reflect queue
*                        FALSE: extract packets out of MCPB reflect queue.
*   enable             - TRUE: enable packet insertion from CPU.
*                        FALSE: disable packet insertion from CPU.
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
PUBLIC PMC_ERROR adpt_enet_cpu_pkt_ext_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       BOOL is_reflect,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = ENET_EGRESS;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, is_reflect, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;

    rc = digi_enet_cpu_extract_pkt_cfg(digi_handle,
                                       enet_dir,
                                       chnl_handle,
                                       (BOOL8)enable,
                                       (BOOL8)is_reflect);

    PMC_RETURN(rc);
} /* adpt_enet_cpu_pkt_ext_set */


/*******************************************************************************
* adpt_enet_cmf_lf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function force inserts of force stops a LF on the given enet link. 
*   This function will supersede any LF consequential actions that FW may be 
*   executing. This function will continue to have control of the LF bit until 
*   releasing control is specified.
*
*   Once control is released, normal operation will continue for the
*   trunk to client LF consequential actions that are set (if any).
*
*   During a force insert LF or force stop LF, all trunk to client consequential
*   actions except LF actions will continue to run properly.
*
*   Releasing control will always stop the insertion of a LF and return control
*   to the FW (which can then reset the LF). This only needs to be done if the
*   trunk to client consequential actions are required.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   trigger            - indicates whether an LF should be inserted, stopped, 
*                        or release control.
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
PUBLIC PMC_ERROR adpt_enet_cmf_lf_cfg(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_cmf_sw_control_action_t trigger)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, trigger);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_trigger_lf_cfg(digi_handle,
                                 chnl_handle,
                                 trigger);

    PMC_RETURN(rc);
} /* adpt_enet_cmf_lf_cfg */


/*******************************************************************************
* adpt_enet_cmf_rf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function force inserts of force stops a RF on the given enet link. 
*   This function will supersede any RF consequential actions that FW may be 
*   executing. This function will continue to have control of the RF bit until 
*   releasing control is specified.
*
*   Once control is released, normal operation will continue for the
*   trunk to client RF consequential actions that are set (if any).
*
*   During a force insert RF or force stop RF, all trunk to client consequential
*   actions except RF actions will continue to run properly.
*
*   Releasing control will always stop the insertion of a RF and return control
*   to the FW (which can then reset the LF). This only needs to be done if the
*   trunk to client consequential actions are required.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   trigger            - indicates whether an RF should be inserted, stopped, 
*                        or release control.
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
PUBLIC PMC_ERROR adpt_enet_cmf_rf_cfg(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_cmf_sw_control_action_t trigger)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, trigger);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cmf_trigger_rf_cfg(digi_handle,
                                 chnl_handle,
                                 trigger);

    PMC_RETURN(rc);
} /* adpt_enet_cmf_rf_cfg */


/*******************************************************************************
* adpt_enet_mgmt_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the forwarding of a specific type of packets 
*   from ENET LINE or ENET SYS input interface to management FEGE output interface.
*   Packet could be also filtered out by their VLAN tag.
*
*   Incoming packets VLAN ID is replaced by a destination VLAN ID when 
*   replace_vlan_id is TRUE.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pkt_type           - packet type.
*   is_vlan_tagged     - TRUE:  packet is VLAN-tagged and its VLAN will be replaced.
*                        FALSE: packet isn't VLAN-tagged. 
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl       - VLAN ID value replaced in forwarded packet. 
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl_en    - TRUE:  enable packet VLAN ID replacement.
*                        FALSE: disable packet VLAN ID replacement.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_pkt_type_t pkt_type,
                                         BOOL is_vlan_tagged,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 vlan_id_repl,
                                         BOOL vlan_id_repl_en,
                                         BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = ENET_EGRESS;
    UINT16 org_vlan_id_repl = 0;
    BOOL8 org_vlan_id_repl_en = FALSE;
    BOOL8 org_enable = FALSE;    
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, pkt_type, is_vlan_tagged, vlan_id_repl, 
               vlan_id_repl_en, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;
    rc = digi_enet_to_mgmt_fege_pkt_fwd_cfg_single_get(digi_handle,
                                                       chnl_handle,
                                                       enet_dir,
                                                       pkt_type,
                                                       (BOOL8)is_vlan_tagged,
                                                       vlan_src_cfg,
                                                       &org_enable,
                                                       &org_vlan_id_repl_en,
                                                       &org_vlan_id_repl);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        rc = digi_enet_to_mgmt_fege_pkt_fwd_cfg(digi_handle,
                                                chnl_handle,
                                                enet_dir,
                                                pkt_type,
                                                (BOOL8)is_vlan_tagged,
                                                vlan_src_cfg,
                                                (BOOL8)enable,
                                                (BOOL8)vlan_id_repl_en,
                                                vlan_id_repl);
        ADPT_RETURN_VERIFY(rc);
    }
    else if (enable)
    {
        if ((org_vlan_id_repl_en && vlan_id_repl_en && (org_vlan_id_repl != vlan_id_repl))
            || (org_vlan_id_repl_en != vlan_id_repl_en))
        {
            rc = digi_enet_to_mgmt_fege_pkt_fwd_cfg(digi_handle,
                                                    chnl_handle,
                                                    enet_dir,
                                                    pkt_type,
                                                    (BOOL8)is_vlan_tagged,
                                                    vlan_src_cfg,
                                                    FALSE,
                                                    org_vlan_id_repl_en,
                                                    org_vlan_id_repl);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_enet_to_mgmt_fege_pkt_fwd_cfg(digi_handle,
                                                    chnl_handle,
                                                    enet_dir,
                                                    pkt_type,
                                                    (BOOL8)is_vlan_tagged,
                                                    vlan_src_cfg,
                                                    (BOOL8)enable,
                                                    (BOOL8)vlan_id_repl_en,
                                                    vlan_id_repl);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_enet_mgmt_pkt_fwd_cfg */


/*******************************************************************************
* adpt_enet_mgmt_pkt_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the status of the forwarding of a specific type of packets 
*   from ENET LINE or ENET SYS input interface to management FEGE output interface.
*   Packet could be also filtered out by their VLAN tag.
*
*   Incoming packets VLAN ID is replaced by a destination VLAN ID when 
*   replace_vlan_id is TRUE.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pkt_type           - packet type.
*
* OUTPUTS:
*   *is_vlan_tagged    - TRUE:  packet is VLAN-tagged and its VLAN will be replaced.
*                        FALSE: packet isn't VLAN-tagged. 
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   *vlan_id_repl      - VLAN ID value replaced in forwarded packet. 
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   *vlan_id_repl_en   - TRUE:  enable packet VLAN ID replacement.
*                        FALSE: disable packet VLAN ID replacement.
*   *enable            - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_mgmt_pkt_fwd_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_pkt_type_t pkt_type,
                                         BOOL *is_vlan_tagged,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 *vlan_id_repl,
                                         BOOL *vlan_id_repl_en,
                                         BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = ENET_EGRESS;
    digi_enet_to_mgmt_fege_extract_cfg_t extract_cfg[28];
    BOOL8 vlan_replace_en = FALSE;
    UINT32 index = 0;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(is_vlan_tagged);
    ADPT_PARA_VERIFY(vlan_src_cfg);
    ADPT_PARA_VERIFY(vlan_id_repl);
    ADPT_PARA_VERIFY(vlan_id_repl_en);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;

    *enable = (BOOL)digi_enet_to_mgmt_fege_pkt_fwd_cfg_get(digi_handle,
                                                           chnl_handle,
                                                           enet_dir,
                                                           &extract_cfg[0],
                                                           &vlan_replace_en,
                                                           vlan_id_repl);

    for (index = 0; index < 28; index++)
    {
        if ((pkt_type == extract_cfg[index].pkt_type) && *enable)
        {
            *is_vlan_tagged         = extract_cfg[index].is_vlan_tagged;
            *enable                 = extract_cfg[index].valid;
            vlan_src_cfg->vlan_type = extract_cfg[index].vlan_src_cfg.vlan_type;
            vlan_src_cfg->vlan_pcp  = extract_cfg[index].vlan_src_cfg.vlan_pcp;
            vlan_src_cfg->vlan_cfi  = extract_cfg[index].vlan_src_cfg.vlan_cfi;
            vlan_src_cfg->vlan_id   = extract_cfg[index].vlan_src_cfg.vlan_id;
            break;
        }
    }
    *vlan_id_repl_en        = vlan_replace_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_enet_mgmt_pkt_fwd_get */


/*******************************************************************************
* adpt_mgmt_enet_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   input interface to ENET LINE or ENET_SYS output interface. Packets are
*   filtered in by their VLAN ID.  
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_pri_ow        - TRUE:  priority bit in VLAN TCI is overwriten by zeroes.
*                        FALSE: keep priority bit in VLAN TCI unchange.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mgmt_enet_pkt_fwd_cfg(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL vlan_pri_ow,
                                         BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = ENET_EGRESS;
    BOOL org_enable = FALSE;
    BOOL org_vlan_pri_ow = FALSE;
    util_global_vlan_cfg_t org_vlan_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, dir, vlan_pri_ow, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;
    rc = adpt_mgmt_enet_pkt_fwd_get(dev_id, chnl_id, dir, &org_vlan_cfg, 
                                    &org_vlan_pri_ow, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        rc = digi_mgmt_fege_to_enet_pkt_fwd_cfg(digi_handle,
                                                chnl_handle,
                                                enet_dir,
                                                vlan_src_cfg,
                                                (BOOL8)enable,
                                                (BOOL8)vlan_pri_ow);
        ADPT_RETURN_VERIFY(rc);
    }
    else if (enable)
    {
        if ((0 != PMC_MEMCMP(&org_vlan_cfg, vlan_src_cfg, sizeof(util_global_vlan_cfg_t)))
            || (org_vlan_pri_ow != vlan_pri_ow))
        {
            rc = digi_mgmt_fege_to_enet_pkt_fwd_cfg(digi_handle,
                                                    chnl_handle,
                                                    enet_dir,
                                                    &org_vlan_cfg,
                                                    FALSE,
                                                    (BOOL8)org_vlan_pri_ow);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_mgmt_fege_to_enet_pkt_fwd_cfg(digi_handle,
                                                    chnl_handle,
                                                    enet_dir,
                                                    vlan_src_cfg,
                                                    (BOOL8)enable,
                                                    (BOOL8)vlan_pri_ow);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_mgmt_enet_pkt_fwd_cfg */


/*******************************************************************************
* adpt_mgmt_enet_pkt_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the status of packets forwarding from management FEGE 
*   input interface to ENET LINE or ENET_SYS output interface. Packets are
*   filtered in by their VLAN ID.  
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   *vlan_pri_ow       - TRUE:  priority bit in VLAN TCI is overwriten by zeroes.
*                        FALSE: keep priority bit in VLAN TCI unchange.
*   *enable            - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mgmt_enet_pkt_fwd_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL *vlan_pri_ow,
                                         BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = ENET_EGRESS;
    BOOL8 ow_vlan_pri = FALSE;
    BOOL8 fwd_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);
    ADPT_PARA_VERIFY(vlan_pri_ow);
    ADPT_PARA_VERIFY(enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;

    rc = digi_mgmt_fege_to_enet_pkt_fwd_vlan_pri_cfg_get(digi_handle,
                                                         chnl_handle,
                                                         enet_dir,
                                                         &fwd_en,
                                                         vlan_src_cfg,
                                                         &ow_vlan_pri);
    *vlan_pri_ow = ow_vlan_pri ? TRUE : FALSE;
    *enable      = fwd_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_mgmt_enet_pkt_fwd_get */


/*******************************************************************************
* adpt_cpu_mgmt_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the forwarding of packets from CPU
*   to management FEGE output interface.
*
*   Packets VLAN ID is replaced by a destination VLAN ID when replace_vlan_id
*   is TRUE.
*
* INPUTS:
*   dev_id             - device identity.
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl       - VLAN ID value replaced in forwarded packet. 
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl_en    - TRUE:  enable packet VLAN ID replacement.
*                        FALSE: disable packet VLAN ID replacement.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpu_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 vlan_id_repl,
                                         BOOL vlan_id_repl_en,
                                         BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, vlan_id_repl, 
               vlan_id_repl_en, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_cpu_to_mgmt_fege_pkt_fwd_cfg(adpt_handle->digi_handle,
                                           vlan_src_cfg,
                                           (BOOL8)enable,
                                           (BOOL8)vlan_id_repl_en,
                                           vlan_id_repl);

    PMC_RETURN(rc);
} /* adpt_cpu_mgmt_pkt_fwd_cfg */


/*******************************************************************************
* adpt_cpu_mgmt_pkt_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the status of the forwarding of packets from CPU
*   to management FEGE output interface.
*
*   Packets VLAN ID is replaced by a destination VLAN ID when replace_vlan_id
*   is TRUE.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   *vlan_id_repl      - VLAN ID value replaced in forwarded packet. 
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   *vlan_id_repl_en   - TRUE:  enable packet VLAN ID replacement.
*                        FALSE: disable packet VLAN ID replacement.
*   *enable            - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_cpu_mgmt_pkt_fwd_get(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 *vlan_id_repl,
                                         BOOL *vlan_id_repl_en,
                                         BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 vlan_replace_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);
    ADPT_PARA_VERIFY(vlan_id_repl);
    ADPT_PARA_VERIFY(vlan_id_repl_en);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    *enable = (BOOL)digi_cpu_to_mgmt_fege_pkt_fwd_cfg_get(adpt_handle->digi_handle,
                                                          vlan_src_cfg,
                                                          &vlan_replace_en,
                                                          vlan_id_repl);
    *vlan_id_repl_en = vlan_replace_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_cpu_mgmt_pkt_fwd_get */


/*******************************************************************************
* adpt_mgmt_cpu_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   input interface to CPU output interface. Packets are filtered in by their
*   VLAN ID.  
*
* INPUTS:
*   dev_id             - device identity.
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   enable             - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
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
*   The adpt_fege_en_set() should be enabled prior to this function use.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mgmt_cpu_pkt_fwd_cfg(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    util_global_vlan_cfg_t org_vlan_cfg;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d].\r\n", dev_id, enable);

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_mgmt_cpu_pkt_fwd_get(dev_id, &org_vlan_cfg, &org_enable); 
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        rc = digi_mgmt_fege_to_cpu_pkt_fwd_cfg(adpt_handle->digi_handle,
                                               vlan_src_cfg,
                                               (BOOL8)enable);
        ADPT_RETURN_VERIFY(rc);
    }
    else if (enable)
    {
        if (0 != PMC_MEMCMP(&org_vlan_cfg, vlan_src_cfg, sizeof(util_global_vlan_cfg_t)))
        {
            rc = digi_mgmt_fege_to_cpu_pkt_fwd_cfg(adpt_handle->digi_handle,
                                                   &org_vlan_cfg,
                                                   FALSE);
            ADPT_RETURN_VERIFY(rc);

            rc = digi_mgmt_fege_to_cpu_pkt_fwd_cfg(adpt_handle->digi_handle,
                                                   vlan_src_cfg,
                                                   (BOOL8)enable);
            ADPT_RETURN_VERIFY(rc);
        }
    }

    PMC_RETURN(rc);
} /* adpt_mgmt_cpu_pkt_fwd_cfg */


/*******************************************************************************
* adpt_mgmt_cpu_pkt_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the enable/disable status of packets forwarding from  
*   management FEGE input interface to CPU output interface. 
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *vlan_src_cfg      - VLAN-tagged packet configuration.
*                        It is ignored when enable or is_vlan_tagged are FALSE.
*   *enable            - TRUE:  enable a forwarding path.
*                        FALSE: disable a forwarding path. 
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_mgmt_cpu_pkt_fwd_get(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 fwd_en = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(vlan_src_cfg);
    ADPT_PARA_VERIFY(enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_mgmt_fege_to_cpu_pkt_fwd_cfg_get(adpt_handle->digi_handle,
                                               vlan_src_cfg,
                                               &fwd_en);
    *enable = fwd_en ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_mgmt_cpu_pkt_fwd_get */


/*******************************************************************************
* adpt_enet_inbound_ft_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs configuration of a CBRC resource and DCPB datapath
*   so that the CBRC resource may be used as a source of nominal rate
*   client replacement signal for a 10GE CBR application in an ENET SS to 
*   MAPOTN data path.
*
*   The replacement signal is configured for LF/RF. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   fault_lf           - TRUE: generate local fault
*                        FALSE: generate remote fault
*   enable             - TRUE: enable ENET inbound feature with LF/RF insertion.
*                        FALSE: disable ENET inbound feature without LF/RF insertion.
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
*   1. The ENET to MAPOTN datapath must be mapped and activated.
*   2. This feature must be disabled prior to deactivating the datapath.
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_inbound_ft_en_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         BOOL fault_lf,
                                         BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL fault_type = FALSE;
    BOOL org_enable = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = adpt_enet_inbound_ft_en_get(dev_id, chnl_id, &fault_type, &org_enable);
    ADPT_RETURN_VERIFY(rc);

    if (org_enable != enable)
    {
        if (enable)
        {
            /* CBRC provision/deprovision */
            rc = digi_enet_inbound_fault_prov(digi_handle, chnl_handle, (BOOL8)enable);
            ADPT_RETURN_VERIFY(rc);

            /* fault type */
            rc = digi_enet_inbound_fault_type_cfg(digi_handle, chnl_handle, fault_lf);
            ADPT_RETURN_VERIFY(rc);

            /* fault insertion */
            rc = digi_enet_inbound_fault_force(digi_handle, chnl_handle, (BOOL8)enable);
            ADPT_RETURN_VERIFY(rc);
        }
        else
        {
            /* fault insertion */
            rc = digi_enet_inbound_fault_force(digi_handle, chnl_handle, (BOOL8)enable);
            ADPT_RETURN_VERIFY(rc);

            /* fault type */
            rc = digi_enet_inbound_fault_type_cfg(digi_handle, chnl_handle, fault_lf);
            ADPT_RETURN_VERIFY(rc);

            /* CBRC provision/deprovision */
            rc = digi_enet_inbound_fault_prov(digi_handle, chnl_handle, (BOOL8)enable);
            ADPT_RETURN_VERIFY(rc);
        }
    }
    else if (fault_lf != fault_type)
    {
        /* fault type */
        rc = digi_enet_inbound_fault_type_cfg(digi_handle, chnl_handle, fault_lf);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_enet_inbound_ft_en_set */


/*******************************************************************************
* adpt_enet_inbound_ft_en_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves configuration detail of the CBRC SS used for
*   inbound fault replacement of 10GE CBR applications.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *fault_lf          - TRUE: Fault type configured with LF insertion.
*                        FALSE: Fault type configured with RF insertion. 
*   *enable            - TRUE: CBRC generating fault stream replacement.
*                        FALSE: ENET generating data stream.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_inbound_ft_en_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         BOOL *fault_lf,
                                         BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL8 cfg_state = FALSE;
    BOOL8 lf_type = FALSE;
    BOOL8 active_st = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(enable);
    ADPT_PARA_VERIFY(fault_lf);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_inbound_fault_state_get(digi_handle,
                                           chnl_handle,
                                           &cfg_state,
                                           &lf_type,
                                           &active_st);
    *enable   = active_st ? TRUE : FALSE;
    *fault_lf = lf_type ? TRUE : FALSE;

    PMC_RETURN(rc);
} /* adpt_enet_inbound_ft_en_get */


/*******************************************************************************
* adpt_enet_eclass_filter_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function applies a filter config to the eclass subsystem.
*   The function can be called mulitple times to keep adding more filters
*   to the eclass, provided there are adequate resources. The eclass filter is
*   applied per enet link.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   eclass_cfg         - configuration on the filter mode.
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
PUBLIC PMC_ERROR adpt_enet_eclass_filter_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_enet_eclass_filter_config_t eclass_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               eclass_cfg.direction, eclass_cfg.encap, eclass_cfg.mode, eclass_cfg.target);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_eclass_filter_config(digi_handle, chnl_handle, eclass_cfg);

    PMC_RETURN(rc);
} /* adpt_enet_eclass_filter_set */


/*******************************************************************************
* adpt_enet_lpd_pkt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to retrieve the address of the first 32 bytes of the 
*   latest packet from the LPD buffer for the given channel. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*
* OUTPUTS:
*   *pkt_addr          - packet address. If the buffer has no packet, an address 
*                        of 0 is returned.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_lpd_pkt_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   UINT32 *pkt_addr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_enet_eclass_dir_t direction = DIGI_ENET_ECLASS_DIR_INGRESS;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(pkt_addr);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    direction = (DIGI_PORT_DIR_RX == dir) ? DIGI_ENET_ECLASS_DIR_INGRESS : DIGI_ENET_ECLASS_DIR_EGRESS;
    rc = digi_enet_lpd_pkt_fetch(digi_handle, chnl_handle, direction, pkt_addr);

    PMC_RETURN(rc);
} /* adpt_enet_lpd_pkt_get */


/*******************************************************************************
* adpt_enet_lpd_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to release the LPD buffer for the given channel and 
*   allow the device to update the buffer with the latest LPD packet.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
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
PUBLIC PMC_ERROR adpt_enet_lpd_release(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   digi_port_dir_t dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_enet_eclass_dir_t direction = DIGI_ENET_ECLASS_DIR_INGRESS;
    PMC_ENTRY();

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    direction = (DIGI_PORT_DIR_RX == dir) ? DIGI_ENET_ECLASS_DIR_INGRESS : DIGI_ENET_ECLASS_DIR_EGRESS;
    rc = digi_enet_lpd_release(digi_handle, chnl_handle, direction);

    PMC_RETURN(rc);
} /* adpt_enet_lpd_release */


/*******************************************************************************
* adpt_enet_lldp_pkt_ext_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables packets extraction from the ENET LINE or ENET SYS
*   input interface to CPU. 
*
*   This API is only used for the LLDP feature.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   enable             - TRUE: enable packet extraction from ENET.
*                        FALSE: disable packet extraction from ENET.
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
PUBLIC PMC_ERROR adpt_enet_lldp_pkt_ext_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_handle_t *digi_handle = NULL;
    digi_cfg_container_t *adpt_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    enet_dir_t enet_dir = (DIGI_PORT_DIR_RX == dir) ? ENET_INGRESS : ENET_EGRESS;
    BOOL tx_pmon = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, dir, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);
    
    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    if ((ENET_EGRESS == enet_dir) 
        && ((DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP == adpt_handle->traffic_info[index].mapping_mode)))
    {
        if (DIGI_PROD_APP_TRANSMUXPONDER_CARD == adpt_handle->dev_info->card_type)
        {
            tx_pmon = TRUE;
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].map_handle;
            }
        }

        /* get the ENET handle of Tx PMON */
        chnl_handle = digi_enet_lldp_companion_pmon_get(digi_handle, chnl_handle, tx_pmon);
        ADPT_PARA_VERIFY(chnl_handle);

        rc = digi_enet_cpu_extract_pkt_cfg(digi_handle,
                                           ENET_INGRESS,
                                           chnl_handle,
                                           (BOOL8)enable,
                                           FALSE);
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        rc = digi_enet_cpu_extract_pkt_cfg(digi_handle,
                                           enet_dir,
                                           chnl_handle,
                                           (BOOL8)enable,
                                           FALSE);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_enet_lldp_pkt_ext_set */


/*******************************************************************************
* adpt_enet_lldp_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure LLDP filtering.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   pkt_encap_type     - Packet is encapsulated by Ethernet or Ethernet over IPV4
*                        or Ethernet over IPV6.
*   lldp_filter_mode   - eclass lldp filter mode(only lldp types are valid).
*   eth_type           - EtherType value to filter(only needed for LLDP_ETH filter mode).
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
PUBLIC PMC_ERROR adpt_enet_lldp_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_enet_eclass_encap_t pkt_encap_type,
                                digi_enet_eclass_mode_t lldp_filter_mode,
                                UINT32 eth_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_handle_t *digi_handle = NULL;
    digi_cfg_container_t *adpt_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    UINT32 traffic_loc = 0;
    BOOL tx_pmon = FALSE;
    digi_enet_eclass_dir_t direction = DIGI_ENET_ECLASS_DIR_INGRESS;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               dir, pkt_encap_type, lldp_filter_mode, eth_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);
    
    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /* eclass direction */    
    direction = (DIGI_PORT_DIR_RX == dir) ? DIGI_ENET_ECLASS_DIR_INGRESS : DIGI_ENET_ECLASS_DIR_EGRESS;
    /* get traffic loc */
    traffic_loc = DIGI_TRAFFIC_POS_GET(adpt_handle->traffic_info[index].traffic_mode);

    /**********************************************************************
     * As to 10GE G.sup 43 7.1/7.3 datapaths, there is a special process 
     * for the LLDP monitoring in the Tx direction:
     * 1. If the client SERDES is from ENET_SYS, take ENET_LINE as Tx PMON.
     * 2. If the client SERDES is from ENET_LINE, take ENET_SYS as Tx PMON. 
     **********************************************************************/
    if ((DIGI_ENET_ECLASS_DIR_EGRESS == direction) 
        && ((DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU3P_CBR_40_GE_GMP == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU4P_CBR_100_GE_GMP == adpt_handle->traffic_info[index].mapping_mode)))
    {
        if (DIGI_PROD_APP_TRANSMUXPONDER_CARD == adpt_handle->dev_info->card_type)
        {
            tx_pmon = TRUE;
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].map_handle;
            }
        }

        /* get the ENET handle of Tx PMON */
        chnl_handle = digi_enet_lldp_companion_pmon_get(digi_handle, chnl_handle, tx_pmon);
        ADPT_PARA_VERIFY(chnl_handle);

        rc = digi_enet_lldp_config(digi_handle, 
                                   chnl_handle, 
                                   pkt_encap_type,
                                   lldp_filter_mode,
                                   eth_type,
                                   DIGI_ENET_ECLASS_DIR_INGRESS,
                                   ((DIGI_SYS_SIDE == traffic_loc) ? 0x1 : 0x2));
        ADPT_RETURN_VERIFY(rc);
    }
    else
    {
        rc = digi_enet_lldp_config(digi_handle, 
                                   chnl_handle, 
                                   pkt_encap_type,
                                   lldp_filter_mode,
                                   eth_type,
                                   direction,
                                   0);
        ADPT_RETURN_VERIFY(rc);
    }

    PMC_RETURN(rc);
} /* adpt_enet_lldp_set */


/*******************************************************************************
* adpt_enet_lldp_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable/disable LLDP filtering on a transparent enet 
*   path. The adpt_enet_lldp_set() needs to have been called first on a transparent 
*   enet channel to configure the lldp filters. 
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   dir                - traffic direction: Tx, Rx.
*   enable             - TRUE: enable LLDP filtering on a transparent enet path.
*                        FALSE: disable LLDP filtering on a transparent enet path.
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
PUBLIC PMC_ERROR adpt_enet_lldp_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_handle_t *digi_handle = NULL;
    digi_cfg_container_t *adpt_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL tx_pmon = FALSE;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);
    
    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /**********************************************************************
     * As to 10GE G.sup 43 7.1/7.3 datapaths, there is a special process 
     * for the LLDP monitoring in the Tx direction:
     * 1. If the client SERDES is from ENET_SYS, take ENET_LINE as Tx PMON.
     * 2. If the client SERDES is from ENET_LINE, take ENET_SYS as Tx PMON. 
     **********************************************************************/
    if ((DIGI_PORT_DIR_TX == dir) 
        && ((DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP == adpt_handle->traffic_info[index].mapping_mode)
        || (DIGI_MAPOTN_ODU2P_CBR_10_GE_7_3_GFP_ENET == adpt_handle->traffic_info[index].mapping_mode)))
    {
        if ((DIGI_PROD_APP_TRANSMUXPONDER_CARD == adpt_handle->dev_info->card_type)
            && (DIGI_MAPOTN_ODU2EP_CBR_10G3_7_1_BMP == adpt_handle->traffic_info[index].mapping_mode))
        {
            tx_pmon = TRUE;
            if (NULL != adpt_handle->traffic_info[index].map_handle)
            {
                chnl_handle = adpt_handle->traffic_info[index].map_handle;
            }
        }

        /* get the ENET handle of Tx PMON */
        chnl_handle = digi_enet_lldp_companion_pmon_get(digi_handle, chnl_handle, tx_pmon);
        ADPT_PARA_VERIFY(chnl_handle);
    }

    rc = digi_enet_transparent_lldp_set(digi_handle, chnl_handle, (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_enet_lldp_en_set */


/*******************************************************************************
* adpt_enet_idle_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the XL/CGMII idle insertion of the provisioned channel. Note that 
*   fault insertion (either using the SEND_RF/SEND_LF or through Rx-to-Tx 
*   consequential actions) has precedence over idle insertion.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable XL/CGMII insertion.
*                        FALSE: disable XL/CGMII insertion.
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
PUBLIC PMC_ERROR adpt_enet_idle_inst_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_idle_insert_set(digi_handle, chnl_handle, enable);

    PMC_RETURN(rc);
} /* adpt_enet_idle_inst_set */


/*******************************************************************************
* adpt_enet_err_frm_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function is to enable or disable EMAC ingress errored frames drop in 
*   DCPB for a given type of error. This is a global ENET configuration.
*
* INPUTS:
*   dev_id             - device identity.
*   enet_line          - TRUE: ENET LINE.
*                        FALSE: ENET SYS.
*   error_type         - type of error: FCS, packet length, LF, RF and vlan's. 
*   dpi_err_en         - TRUE: the detection of error_type asserts a DPI packet 
*                        error.
*                        FALSE: the detection of error_type does not generate a  
*                        DPI error indication.
*   drop_en            - TRUE: the EMAC is configured to drop errored frames.
*                        FALSE: the EMAC is configured to forward errored frames.
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
PUBLIC PMC_ERROR adpt_enet_err_frm_set(UINT32 dev_id,
                                   BOOL enet_line,
                                   emac_error_t error_type,
                                   BOOL dpi_err_en,
                                   BOOL drop_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, enet_line, 
               error_type, dpi_err_en, drop_en);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_emac_errored_frames_dpi_error_cfg(adpt_handle->digi_handle,
                                                     (BOOL8)enet_line,
                                                     error_type,
                                                     (BOOL8)dpi_err_en);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_emac_errored_frames_drop_cfg(adpt_handle->digi_handle,
                                                (BOOL8)enet_line,
                                                error_type,
                                                (BOOL8)drop_en);
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_err_frm_set */


/*******************************************************************************
* adpt_enet_err_frm_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function is to retrieve the enable/disable status of EMAC ingress errored 
*   frames drop in DCPB for a given type of error. This is a global ENET 
*   configuration.
*
* INPUTS:
*   dev_id             - device identity.
*   enet_line          - TRUE: ENET LINE.
*                        FALSE: ENET SYS.
*   error_type         - type of error: FCS, packet length, LF, RF and vlan's. 
*
* OUTPUTS:
*   *dpi_err_en        - TRUE: the detection of error_type asserts a DPI packet 
*                        error.
*                        FALSE: the detection of error_type does not generate a  
*                        DPI error indication.
*   *drop_en           - TRUE: the EMAC is configured to drop errored frames.
*                        FALSE: the EMAC is configured to forward errored frames.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_err_frm_get(UINT32 dev_id,
                                   BOOL enet_line,
                                   emac_error_t error_type,
                                   BOOL *dpi_err_en,
                                   BOOL *drop_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    BOOL8 dpi_err_enable = FALSE;
    BOOL8 drop_enable = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(dpi_err_en);
    ADPT_PARA_VERIFY(drop_en);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_emac_errored_frames_dpi_error_get(adpt_handle->digi_handle,
                                                     (BOOL8)enet_line,
                                                     error_type,
                                                     &dpi_err_enable);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_emac_errored_frames_drop_get(adpt_handle->digi_handle,
                                                (BOOL8)enet_line,
                                                error_type,
                                                &drop_enable);
    ADPT_RETURN_VERIFY(rc);

    /* parameter conversion */
    *dpi_err_en = dpi_err_enable;
    *drop_en    = drop_enable;

    PMC_RETURN(rc);
} /* adpt_enet_err_frm_get */


/*******************************************************************************
* adpt_enet_err_msk_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the EMAC Ingress Packet FCS and length error mask on a per-link 
*   basis. If fcs_mask is true, an fcs error on the link will not result in an 
*   ingress DPI error or DPI drop indication. If len_mask is true, a length error 
*   will not assert a DPI error or drop indication.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   err_fcs_en         - TRUE: FCS error may contribute to DPI error/drop.
*                        FALSE: FCS error will not contribute.
*   err_len_en         - TRUE: Length error may contribute to DPI error/drop.
*                        FALSE: Length error will not contribute.
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
PUBLIC PMC_ERROR adpt_enet_err_msk_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL err_fcs_en,
                                    BOOL err_len_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id, 
               err_fcs_en, err_len_en);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    if (err_fcs_en)
    {
        rc = digi_enet_emac_ingr_dpi_err_mask_cfg(digi_handle, 
                                                  chnl_handle, 
                                                  EMAC_INGR_DPI_ERR_FCS_ENABLE);
    }
    else
    {
        rc = digi_enet_emac_ingr_dpi_err_mask_cfg(digi_handle, 
                                                  chnl_handle, 
                                                  EMAC_INGR_DPI_ERR_FCS_DISABLE);
    }
    ADPT_RETURN_VERIFY(rc);

    if (err_len_en)
    {
        rc = digi_enet_emac_ingr_dpi_err_mask_cfg(digi_handle, 
                                                  chnl_handle, 
                                                  EMAC_INGR_DPI_ERR_LEN_ENABLE);
    }
    else
    {
        rc = digi_enet_emac_ingr_dpi_err_mask_cfg(digi_handle, 
                                                  chnl_handle, 
                                                  EMAC_INGR_DPI_ERR_LEN_DISABLE);
    }
    ADPT_RETURN_VERIFY(rc);

    PMC_RETURN(rc);
} /* adpt_enet_err_frm_set */


/*******************************************************************************
* adpt_enet_err_msk_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the EMAC Ingress Packet FCS and length error mask on a per-link 
*   basis. If fcs_mask is true, an fcs error on the link will not result in an 
*   ingress DPI error or DPI drop indication. If len_mask is true, a length error 
*   will not assert a DPI error or drop indication.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *err_fcs_en        - TRUE: FCS error may contribute to DPI error/drop.
*                        FALSE: FCS error will not contribute.
*   *err_len_en        - TRUE: Length error may contribute to DPI error/drop.
*                        FALSE: Length error will not contribute.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_err_msk_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *err_fcs_en,
                                    BOOL *err_len_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    BOOL8 err_fcs_enable = FALSE;
    BOOL8 err_len_enable = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(err_fcs_en);
    ADPT_PARA_VERIFY(err_len_en);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_emac_ingr_dpi_err_mask_get(digi_handle, 
                                              chnl_handle, 
                                              &err_fcs_enable,
                                              &err_len_enable);

    /* parameter conversion */
    *err_fcs_en = err_fcs_enable;
    *err_len_en = err_len_enable;

    PMC_RETURN(rc);
} /* adpt_enet_err_msk_get */


/*******************************************************************************
* adpt_enet_mac_addr_inst_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the EMAC address insertion in the Tx direction.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   mac_addr           - MAC address.
*   inst_en            - TRUE: enable MAC address insertion in the Tx direction.
*                        FALSE: disable MAC address insertion in the Tx direction.
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
PUBLIC PMC_ERROR adpt_enet_mac_addr_inst_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         UINT64 mac_addr,
                                         BOOL inst_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    UINT32 mac_addr_lsb = mac_addr & 0xFFFFFFFF;
    UINT32 mac_addr_msb = mac_addr >> 32;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, chnl_id, 
               mac_addr_lsb, mac_addr_msb, inst_en);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_ins_mac_addr_set(digi_handle,
                                    chnl_handle,
                                    (BOOL8)inst_en,
                                    mac_addr_msb,
                                    mac_addr_lsb);

    PMC_RETURN(rc);
} /* adpt_enet_mac_addr_inst_set */


/*******************************************************************************
* adpt_enet_mac_addr_inst_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the EMAC address insertion in the Tx direction.
*
*   Please note that only data path through the MAC block can support this
*   function.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *mac_addr          - MAC address.
*   *inst_en           - TRUE: enable MAC address insertion in the Tx direction.
*                        FALSE: disable MAC address insertion in the Tx direction.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_enet_mac_addr_inst_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         UINT64 *mac_addr,
                                         BOOL *inst_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    UINT32 mac_addr_lsb = 0;
    UINT32 mac_addr_msb = 0;
    BOOL8 inst_enable = FALSE;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(mac_addr);
    ADPT_PARA_VERIFY(inst_en);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_enet_ins_mac_addr_get(digi_handle,
                                    chnl_handle,
                                    &inst_enable,
                                    &mac_addr_msb,
                                    &mac_addr_lsb);

    /* parameter conversion */
    *mac_addr = ((UINT64)mac_addr_msb << 32) | mac_addr_lsb;
    *inst_en  = inst_enable;

    PMC_RETURN(rc);
} /* adpt_enet_mac_addr_inst_get */


/*
** End of file
*/
