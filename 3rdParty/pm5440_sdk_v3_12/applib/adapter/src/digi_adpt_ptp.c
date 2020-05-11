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
*     The file describes the PTP Run-Time functionality.
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
* adpt_ptp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the PTP mode of operation as Boundary
*   Clock or Transparent Clock, and related PTP parameters. The parameters
*   configured by this message correlate to the members of the defaultDS data
*   set described in IEEE 1588v2 standard.
*
* INPUTS:
*   dev_id             - device identity.
*   *ptp_cfg           - pointer to the PTP configuration structure.
*   enet_line          - TRUE: PTP configuration in the line ENET.
*                        FALSE: PTP configuration in the system ENET.
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
PUBLIC PMC_ERROR adpt_ptp_cfg(UINT32 dev_id, ptp_config_t *ptp_cfg, BOOL enet_line)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, ptp_cfg);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_cfg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_enet_ptp_config(adpt_handle->digi_handle, ptp_cfg, enet_line);

    PMC_RETURN(rc);
} /* adpt_ptp_cfg */


/*******************************************************************************
* adpt_ptp_irig_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure IRIG port.
*
* INPUTS:
*   dev_id             - device identity.
*   port               - IRIGI or IRIGO port.
*   irig_mode          - TOD or IRIG-B B00x mode.
*   adj_mode           - Adjust frequency, phase or both.
*   sync_perd          - Frequency syntonization period, seconds.
*   sync_rate          - Frequency syntonization rate:  0 - 2.9 ppm/sec G.253 
*                        requirement
*   enable             - Enable or disable IRIG port.
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
PUBLIC PMC_ERROR adpt_ptp_irig_cfg(UINT32 dev_id, 
                               ptp_irig_port_enum port,
                               ptp_irig_mode_enum irig_mode,
                               ptp_irig_adj_mode_enum adj_mode,
                               UINT8 sync_perd,
                               UINT8 sync_rate,
                               BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_irig_config_t ptp_irig_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d].\r\n", dev_id, port,
               irig_mode, adj_mode, sync_perd, sync_rate, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    ptp_irig_cfg.port          = port;
    ptp_irig_cfg.irig_mode     = irig_mode;
    ptp_irig_cfg.adj_mode      = adj_mode;
    ptp_irig_cfg.frq_synt_prd  = sync_perd;
    ptp_irig_cfg.frq_synt_rate = sync_rate;
    ptp_irig_cfg.enable        = (BOOL8)enable;
    rc = digi_timesync_ptp_irig_config(adpt_handle->digi_handle, &ptp_irig_cfg);

    PMC_RETURN(rc);
} /* adpt_ptp_irig_cfg */


/*******************************************************************************
* adpt_ptp_ipv4_para_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP over UDP over IPv4 parameters.
*
* INPUTS:
*   dev_id             - device identity.
*   type_of_service    - Type of Service field of the IPv4 header.
*                        Bits 7:5 map to the PRECEDENCE bits, and 
*                        bits 4:2 map to the D, T and R bits of the field.
*   time_to_live       - time to live field of the IPv4 header.
*   src_addr           - 32-bit IPv4 Source Address.
*   mst_dest_addr      - 32-bit IPv4 Destination Address of the master.
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
PUBLIC PMC_ERROR adpt_ptp_ipv4_para_set(UINT32 dev_id, 
                                    UINT8 type_of_service,
                                    UINT8 time_to_live,
                                    UINT32 src_addr,
                                    UINT32 mst_dest_addr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_ipv4_param_t ptp_ipv4_para;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d].\r\n", dev_id, type_of_service,
               time_to_live, src_addr, mst_dest_addr);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    ptp_ipv4_para.typeOfService   = type_of_service;
    ptp_ipv4_para.timeToLive      = time_to_live;
    ptp_ipv4_para.sourceAddress   = src_addr;
    ptp_ipv4_para.mst_destAddress = mst_dest_addr;
    rc = digi_timesync_ptp_ipv4_param_set(adpt_handle->digi_handle, &ptp_ipv4_para);

    PMC_RETURN(rc);
} /* adpt_ptp_ipv4_para_set */


/*******************************************************************************
* adpt_ptp_ipv6_para_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP over UDP over ipv6 parameters.
*
* INPUTS:
*   dev_id             - device identity.
*   *ptp_ipv6          - pointer to the PTP over UDP over IPv6 parameters structure.
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
PUBLIC PMC_ERROR adpt_ptp_ipv6_para_set(UINT32 dev_id, ptp_ipv6_param_t *ptp_ipv6)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, ptp_ipv6);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_ipv6);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_ipv6_param_set(adpt_handle->digi_handle, ptp_ipv6);

    PMC_RETURN(rc);
} /* adpt_ptp_ipv6_para_set */


/*******************************************************************************
* adpt_ptp_port_asymmetry_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure port's asymmetry and latency delays.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *ptp_port          - pointer to the PTP port asymmetry parameters.
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
PUBLIC PMC_ERROR adpt_ptp_port_asymmetry_set(UINT32 dev_id, 
                                         UINT32 chnl_id, 
                                         ptp_port_asymmetry_t *ptp_port)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, ptp_port);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_port);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);
    
    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    ptp_port->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_port_asymmetry_set(adpt_handle->digi_handle, ptp_port);

    PMC_RETURN(rc);
} /* adpt_ptp_port_asymmetry_set */


/*******************************************************************************
* adpt_ptp_port_rx_msg_cnts_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to obtain the received message counter values.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *ptp_rx_msg        - pointer to the received message counters structure.
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
PUBLIC PMC_ERROR adpt_ptp_port_rx_msg_cnts_get(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           ptp_rx_msg_cnts_t *ptp_rx_msg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, ptp_rx_msg);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_rx_msg);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    ptp_rx_msg->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_port_rx_msg_cnts_get(adpt_handle->digi_handle, ptp_rx_msg);

    PMC_RETURN(rc);
} /* adpt_ptp_port_rx_msg_cnts_get */


/*******************************************************************************
* adpt_ptp_path_delay_get 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to obtain the received message counter values.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *ptp_path_delay    - pointer to the path delay data structure.
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
PUBLIC PMC_ERROR adpt_ptp_path_delay_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     ptp_path_delay_t *ptp_path_delay)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, ptp_path_delay);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_path_delay);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    ptp_path_delay->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_path_delay_get(adpt_handle->digi_handle, ptp_path_delay);

    PMC_RETURN(rc);
} /* adpt_ptp_path_delay_get */


/*******************************************************************************
* adpt_ptp_default_pro_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP Boundary Clock mode default properties.
*
* INPUTS:
*   dev_id             - device identity.
*   *default_pro       - pointer to the BC mode default properties.
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
PUBLIC PMC_ERROR adpt_ptp_default_pro_set(UINT32 dev_id, ptp_default_properties_t *default_pro)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, default_pro);

    /* check parameter validation */
    ADPT_PARA_VERIFY(default_pro);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_default_properties_set(adpt_handle->digi_handle, default_pro);

    PMC_RETURN(rc);
} /* adpt_ptp_default_pro_set */


/*******************************************************************************
* adpt_ptp_time_pro_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP Boundary Clock mode time properties.
*
* INPUTS:
*   dev_id             - device identity.
*   *time_pro          - pointer to the BC mode time properties.
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
PUBLIC PMC_ERROR adpt_ptp_time_pro_set(UINT32 dev_id, ptp_set_time_param_t *time_pro)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, time_pro);

    /* check parameter validation */
    ADPT_PARA_VERIFY(time_pro);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_time_properties_set(adpt_handle->digi_handle, time_pro);

    PMC_RETURN(rc);
} /* adpt_ptp_time_pro_set */


/*******************************************************************************
* adpt_ptp_acceptable_master_table_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure PTP Boundary Clock mode acceptable 
*   master clocks.
*
* INPUTS:
*   dev_id             - device identity.
*   *clock_para        - pointer to acceptable master clocks parameters.
*   *clock_data        - pointer to the acceptable master clocks table.
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
PUBLIC PMC_ERROR adpt_ptp_acceptable_master_table_set(UINT32 dev_id, 
                                                ptp_acceptable_master_clocks_t *clock_para,
                                                ptp_acceptable_master_clocks_data_t *clock_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p][%p].\r\n", dev_id, clock_para, clock_data);

    /* check parameter validation */
    ADPT_PARA_VERIFY(clock_para);
    ADPT_PARA_VERIFY(clock_data);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_acceptable_master_table_set(adpt_handle->digi_handle, 
                                                       clock_para,
                                                       clock_data);

    PMC_RETURN(rc);
} /* adpt_ptp_acceptable_master_table_set */


/*******************************************************************************
* adpt_ptp_start_initialize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set PTP state machine to the INITIALIZE state.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity group. 
*                        The invalid chnl_id should be DIGI_DEFAULT_CHNL_ID.
*   *start_init        - pointer to the start initialize structure.
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
PUBLIC PMC_ERROR adpt_ptp_start_initialize(UINT32 dev_id, 
                                    UINT32 chnl_id[DIGI_ENET_CHNL_MAX],
                                    ptp_start_init_param_t *start_init)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, chnl_id);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    for (i = 0; i < DIGI_ENET_CHNL_MAX; i++)
    {
        if (DIGI_DEFAULT_CHNL_ID == chnl_id[i])
        {
            start_init->chnl_handle[i] = NULL;
            continue;
        }

        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id[i], &index);
        ADPT_RETURN_VERIFY(rc);

        /* handle verification */
        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);
        start_init->chnl_handle[i] = adpt_handle->traffic_info[index].enet_handle;
    }

    start_init->resetEnetChannelMask = FALSE;
    rc = digi_timesync_ptp_start_initialize_set(adpt_handle->digi_handle, start_init);

    PMC_RETURN(rc);
} /* adpt_ptp_start_initialize */


/*******************************************************************************
* adpt_ptp_master_clock_db_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to obtain status of the master clock database state.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *status            - pointer to the master clock database status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ptp_master_clock_db_status_get(UINT32 dev_id, ptp_master_clock_db_status_t *status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, status);

    /* check parameter validation */
    ADPT_PARA_VERIFY(status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_master_clock_db_status_get(adpt_handle->digi_handle, status);

    PMC_RETURN(rc);
} /* adpt_ptp_master_clock_db_status_get */


/*******************************************************************************
* adpt_ptp_master_clock_db_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to obtain the master clock database data.
*
* INPUTS:
*   dev_id             - device identity.
*
* OUTPUTS:
*   *clock_db          - pointer to the master clock database data.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ptp_master_clock_db_get(UINT32 dev_id, ptp_master_clock_db_t *clock_db)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, clock_db);

    /* check parameter validation */
    ADPT_PARA_VERIFY(clock_db);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_master_clock_db_get(adpt_handle->digi_handle, clock_db);

    PMC_RETURN(rc);
} /* adpt_ptp_master_clock_db_get */


/*******************************************************************************
* adpt_ptp_parent_data_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set Boundary Clock mode parent data properties.
*
* INPUTS:
*   dev_id             - device identity.
*   *parent_data       - pointer to the parent data properties.
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
PUBLIC PMC_ERROR adpt_ptp_parent_data_set(UINT32 dev_id, ptp_parent_data_t *parent_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, parent_data);

    /* check parameter validation */
    ADPT_PARA_VERIFY(parent_data);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_parent_data_set(adpt_handle->digi_handle, parent_data);

    PMC_RETURN(rc);
} /* adpt_ptp_parent_data_set */


/*******************************************************************************
* adpt_ptp_port_data_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set Boundary Clock mode port data properties.
*
* INPUTS:
*   dev_id             - device identity.
*   *port_data         - pointer to the BC mode port data properties.
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
PUBLIC PMC_ERROR adpt_ptp_port_data_set(UINT32 dev_id, ptp_port_data_t *port_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, port_data);

    /* check parameter validation */
    ADPT_PARA_VERIFY(port_data);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_port_data_set(adpt_handle->digi_handle, port_data);

    PMC_RETURN(rc);
} /* adpt_ptp_port_data_set */


/*******************************************************************************
* adpt_ptp_port_event_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set the DESIGNATED_ENABLED and DESIGNATED_DISABLED
*   event on the port of the PTP Boundary Clock mode.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable PTP Boundary Clock mode port.
*                        FALSE: disable PTP Boundary Clock mode port.
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
PUBLIC PMC_ERROR adpt_ptp_port_event_set(UINT32 dev_id, UINT32 chnl_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_port_event_t port_event;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    port_event.chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    port_event.enableFlag  = (UINT8)enable;
    rc = digi_timesync_ptp_port_event_set(adpt_handle->digi_handle, &port_event);

    PMC_RETURN(rc);
} /* adpt_ptp_port_event_set */


/*******************************************************************************
* adpt_ptp_port_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set states of all Boundary Clock mode ports.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *port_state        - pointer to the mode ports' states.
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
PUBLIC PMC_ERROR adpt_ptp_port_state_set(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     port_state_t *port_state)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_port_state_t ptp_port_state;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, port_state);

    /* check parameter validation */
    ADPT_PARA_VERIFY(port_state);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    /* reset */
    PMC_MEMSET((void*)&ptp_port_state, 0, sizeof(ptp_port_state));
    ptp_port_state.chnl_handle[0] = adpt_handle->traffic_info[index].enet_handle;
    /* memory copy */
    PMC_MEMCPY(&ptp_port_state.state[0], port_state, sizeof(port_state_t));

    rc = digi_timesync_ptp_port_state_set(adpt_handle->digi_handle, &ptp_port_state);

    PMC_RETURN(rc);
} /* adpt_ptp_port_state_set */


/*******************************************************************************
* adpt_ptp_port_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to get states of all Boundary Clock mode ports.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *port_state        - pointer to the PTP ports' states.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ptp_port_state_get(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     port_state_t *port_state)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_port_state_t ptp_port_state;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(port_state);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    /* reset */
    PMC_MEMSET((void*)&ptp_port_state, 0, sizeof(ptp_port_state));
    ptp_port_state.chnl_handle[0] = adpt_handle->traffic_info[index].enet_handle;

    rc = digi_timesync_ptp_port_state_get(adpt_handle->digi_handle, &ptp_port_state);

    /* memory copy */
    PMC_MEMCPY(port_state, &ptp_port_state.state[0], sizeof(port_state_t));

    PMC_RETURN(rc);
} /* adpt_ptp_port_state_get */


/*******************************************************************************
* adpt_ptp_passive_port_parent_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to specify the port identity of the PTP master
*   that passive port should listen to.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   clock_id           - sourcePortIdentity->clockIdentity.
*   port_num           - sourcePortIdentity->portNumber.
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
PUBLIC PMC_ERROR adpt_ptp_passive_port_parent_set(UINT32 dev_id,
                                             UINT32 chnl_id,
                                             UINT64 clock_id,
                                             UINT16 port_num)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_passive_port_parent_t passive_port;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%llu][%d].\r\n", dev_id, chnl_id, 
               clock_id, port_num);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    passive_port.chnl_handle   = adpt_handle->traffic_info[index].enet_handle;
    passive_port.clockIdentity = clock_id;
    passive_port.portNumber    = port_num;
    rc = digi_timesync_ptp_passive_port_parent_set(adpt_handle->digi_handle, &passive_port);

    PMC_RETURN(rc);
} /* adpt_ptp_passive_port_parent_set */


/*******************************************************************************
* adpt_ptp_unicast_master_para_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a function to specify the parameters for the unicast message 
*   negotiation.
*
* INPUTS:
*   dev_id             - device identity.
*   *unicast_para      - pointer to the unicast parameters.
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
PUBLIC PMC_ERROR adpt_ptp_unicast_master_para_set(UINT32 dev_id, ptp_master_unicast_param_t *unicast_para)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, unicast_para);

    /* check parameter validation */
    ADPT_PARA_VERIFY(unicast_para);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_unicast_master_param_set(adpt_handle->digi_handle, unicast_para);

    PMC_RETURN(rc);
} /* adpt_ptp_unicast_master_para_set */


/*******************************************************************************
* adpt_ptp_unicast_request_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to start the transmission of the 
*   REQUEST_UNICAST_TRANSMISSION TLV for a port in the LISTENING, PASSIVE, 
*   UNCALIBRATED or SLAVE state.
*   A port will resend the REQUEST_UNICAST_TRANSMISSION TLV two more times at 
*   one second interval if a GRANT_UNICST_TRANSMISSION TLV is not received.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *request_para      - pointer to the unicast slave parameters.
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
PUBLIC PMC_ERROR adpt_ptp_unicast_request_set(UINT32 dev_id, 
                                         UINT32 chnl_id, 
                                         ptp_unicast_request_t *request_para)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, request_para);

    /* check parameter validation */
    ADPT_PARA_VERIFY(request_para);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    request_para->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_unicast_request_set(adpt_handle->digi_handle, request_para);

    PMC_RETURN(rc);
} /* adpt_ptp_unicast_request_set */


/*******************************************************************************
* adpt_ptp_unicast_cancel_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to trigger master or slave to transmit the 
*   CANCEL_UNICAST_TRANSMISSION TLV.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   msg_type           - PTP message type.
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
PUBLIC PMC_ERROR adpt_ptp_unicast_cancel_set(UINT32 dev_id, UINT32 chnl_id, UINT8 msg_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    ptp_unicast_cancel_t unicast_cancel; 
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, msg_type);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    unicast_cancel.chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    unicast_cancel.messageType = msg_type;
    rc = digi_timesync_ptp_unicast_cancel_set(adpt_handle->digi_handle, &unicast_cancel);

    PMC_RETURN(rc);
} /* adpt_ptp_unicast_cancel_set */


/*******************************************************************************
* adpt_ptp_unicast_slave_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to get the unicast slave state of the PTP Boundary 
*   Clock mode.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *unicast_para      - pointer to the unicast slave parameters.
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
PUBLIC PMC_ERROR adpt_ptp_unicast_slave_state_get(UINT32 dev_id, 
                                            UINT32 chnl_id,
                                            ptp_slave_unicast_param_t *unicast_para)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, unicast_para);

    /* check parameter validation */
    ADPT_PARA_VERIFY(unicast_para);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    unicast_para->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_unicast_slave_state_get(adpt_handle->digi_handle, unicast_para);

    PMC_RETURN(rc);
} /* adpt_ptp_unicast_slave_state_get */


/*******************************************************************************
* adpt_ptp_partial_ptp_para_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is to set the general PTP pass-thru parameters.
*
* INPUTS:
*   dev_id             - device identity.
*   *ptp_para          - pointer to the PTP pass-thru parameters.
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
PUBLIC PMC_ERROR adpt_ptp_pass_thru_general_param_set(UINT32 dev_id, ptp_pass_thru_general_param_t *ptp_para)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, ptp_para);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_para);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    rc = digi_timesync_ptp_pass_thru_general_param_set(adpt_handle->digi_handle, ptp_para);

    PMC_RETURN(rc);
} /* adpt_ptp_pass_thru_general_param_set */


/*******************************************************************************
* adpt_ptp_pass_thru_port_param_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is to set the port PTP pass-thru parameters.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *ptp_para          - pointer to the PTP port pass-thru parameters.
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
PUBLIC PMC_ERROR adpt_ptp_pass_thru_port_param_set(UINT32 dev_id, 
                                              UINT32 chnl_id, 
                                              ptp_pass_thru_port_param_t *ptp_para)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, ptp_para);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_para);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    ptp_para->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_pass_thru_port_param_set(adpt_handle->digi_handle, ptp_para);

    PMC_RETURN(rc);
} /* adpt_ptp_pass_thru_port_param_set */


/*******************************************************************************
* adpt_ptp_tc_param_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to set the PTP parameters for the Terminate or Pass 
*   Through modes.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity group. 
*                        The invalid chnl_id should be DIGI_DEFAULT_CHNL_ID.
*   ptp_tc_para        - pointer to the PTP TC parameters.
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
PUBLIC PMC_ERROR adpt_ptp_tc_param_set(UINT32 dev_id, 
                                    UINT32 chnl_id[DIGI_ENET_CHNL_MAX],
                                    ptp_tc_param_t *ptp_tc_para)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id[0], ptp_tc_para);

    /* check parameter validation */
    ADPT_PARA_VERIFY(ptp_tc_para);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    for (i = 0; i < DIGI_ENET_CHNL_MAX; i++)
    {
        if (DIGI_DEFAULT_CHNL_ID == chnl_id[i])
        {
            ptp_tc_para->chnl_handle[i] = NULL;
            continue;
        }

        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id[i], &index);
        ADPT_RETURN_VERIFY(rc);

        /* handle verification */
        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);
        ptp_tc_para->chnl_handle[i] = adpt_handle->traffic_info[index].enet_handle;
    }

    ptp_tc_para->resetEnetChannelMask = FALSE;
    rc = digi_timesync_ptp_tc_param_set(adpt_handle->digi_handle, ptp_tc_para);

    PMC_RETURN(rc);
} /* adpt_ptp_tc_param_set */


/*******************************************************************************
* adpt_ptp_tc_port_set 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure parameters for each port of a Transparent 
*   Clock.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity group. 
*                        The invalid chnl_id should be DIGI_DEFAULT_CHNL_ID.
*   req_interval       - Log base 2 of the Pdelay_Req message transmission interval.
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
PUBLIC PMC_ERROR adpt_ptp_tc_port_set(UINT32 dev_id, 
                                  UINT32 chnl_id[DIGI_ENET_CHNL_MAX], 
                                  UINT8 req_interval)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    UINT32 i = 0;
    ptp_tc_port_param_t tc_port_para;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id[0], req_interval);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    for (i = 0; i < DIGI_ENET_CHNL_MAX; i++)
    {
        if (DIGI_DEFAULT_CHNL_ID == chnl_id[i])
        {
            tc_port_para.chnl_handle[i] = NULL;
            continue;
        }

        /* get the index in traffic container */
        rc = adpt_chnl_index_get(adpt_handle, chnl_id[i], &index);
        ADPT_RETURN_VERIFY(rc);

        /* handle verification */
        ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);
        tc_port_para.chnl_handle[i] = adpt_handle->traffic_info[index].enet_handle;
    }

    tc_port_para.logMinPdelayReqInterval = req_interval;
    rc = digi_timesync_ptp_tc_port_param_set(adpt_handle->digi_handle, &tc_port_para);

    PMC_RETURN(rc);
} /* adpt_ptp_tc_port_set */


/*******************************************************************************
* adpt_ptp_stability_status_get  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to get the PTP algorithm statibility status.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *status            - pointer to PTP stability status structure.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_ptp_stability_status_get(UINT32 dev_id, 
                                        UINT32 chnl_id,
                                        ptp_stability_status_t *status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%p].\r\n", dev_id, status);

    /* check parameter validation */
    ADPT_PARA_VERIFY(status);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* handle verification */
    ADPT_PARA_VERIFY(adpt_handle->traffic_info[index].enet_handle);

    status->chnl_handle = adpt_handle->traffic_info[index].enet_handle;
    rc = digi_timesync_ptp_stability_status_get(adpt_handle->digi_handle, status);

    PMC_RETURN(rc);
} /* adpt_ptp_stability_status_get */


/*******************************************************************************
* adpt_synce_ssm_cfg  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the SyncE mode and SSM quality level.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *synce_ctrl        - Pointer to the SyncE configurations.
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
PUBLIC PMC_ERROR adpt_synce_ssm_cfg(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_synce_ch_cfg_t *synce_ctrl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_synce_ctrl_t synce_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, synce_ctrl);

    /* check parameter validation */
    ADPT_PARA_VERIFY(synce_ctrl);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&synce_cfg, 0, sizeof(synce_cfg));
    /* memory copy */
    PMC_MEMCPY(&synce_cfg.ch_cfg[0], synce_ctrl, sizeof(digi_synce_ch_cfg_t));
    synce_cfg.ch_cfg[0].chnl_handle = chnl_handle;

    rc = digi_timesync_synce_cfg_ssm(digi_handle, &synce_cfg);

    PMC_RETURN(rc);
} /* adpt_synce_ssm_cfg */


/*******************************************************************************
* adpt_synce_ch_en_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable or disable the SyncE SSM feature after 
*   configuration the channel's SSM. If a channel is enabled and disabled at 
*   the same time, this channel will be disabled. It is important to ensure that
*   digi_fw_enet_client_source_cfg() and digi_timesync_synce_cfg_ssm()
*   are called before calling this method to enable the SyncE SSM feature. 
*   By design, the firmware only processes the Ethernet packets from either 
*   ENET_LINE or ENET_SYS interface, but not from both interfaces at the same time.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   enable             - TRUE: enable the SyncE.
*                        FALSE: disable the SyncE.
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
PUBLIC PMC_ERROR adpt_synce_ch_en_set(UINT32 dev_id, UINT32 chnl_id, BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 i = 0;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle[DIGI_ENET_LINE_CHNL_MAX];
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, enable);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle[0]);
    ADPT_RETURN_VERIFY(rc);

    for (i = 1; i < DIGI_ENET_CHNL_MAX; i++)
    {
        chnl_handle[i] = NULL;
    }

    rc = digi_timesync_synce_ch_en(digi_handle, chnl_handle, (BOOL8)enable);

    PMC_RETURN(rc);
} /* adpt_synce_ch_en_set */


/*******************************************************************************
* adpt_synce_req_ssm_get  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to request SSM status from all the Ethernet channels 
*   on the device. Once the SyncE channels are enabled, this method should be 
*   called periodically to determine if there is a change of the SSM code from 
*   the remote network element. There is also an indicator provided by this 
*   function to indicate the lack of reception of an ESMC information PDU within 
*   a five-second period. 
*
*   Note that the Event Flag returned by this function goes to '1' when an Event 
*   PDU is received, and the Event Flag stored in the firmware is cleared after
*   the host message is sent to firmware.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*
* OUTPUTS:
*   *synce_status      - the replied SyncE status.
*
* RETURNS:
*   PMC_ERROR          - PMC_SUCCESS: get valid SW status.
*                        PMC_ERR_FAIL: get invalid SW status.
*                        PMC_ERR_INVALID_PARAMETERS: get invalid parameters.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR adpt_synce_req_ssm_get(UINT32 dev_id, 
                                      UINT32 chnl_id, 
                                      digi_synce_ch_status_t *synce_status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_synce_info_t synce_info;
    PMC_ENTRY();

    /* check parameter validation */
    ADPT_PARA_VERIFY(synce_status);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&synce_info, 0, sizeof(synce_info));
    synce_info.ch_status[0].chnl_handle = chnl_handle;

    rc = digi_timesync_synce_req_ssm(digi_handle, &synce_info);

    /* memory copy */
    PMC_MEMCPY(synce_status, &synce_info.ch_status[0], sizeof(digi_synce_ch_status_t));

    PMC_RETURN(rc);
} /* adpt_synce_req_ssm_get */


/*******************************************************************************
* adpt_synce_tx_ssm_set  
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to send an event ESMC PDU.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   *synce_ctrl        - the SyncE configurations.
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
PUBLIC PMC_ERROR adpt_synce_tx_ssm_set(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    digi_synce_ch_cfg_t *synce_ctrl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_handle_t *digi_handle = NULL;
    util_global_switch_data_t *chnl_handle = NULL;
    digi_synce_ctrl_t synce_cfg;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%p].\r\n", dev_id, chnl_id, synce_ctrl);

    /* check parameter validation */
    ADPT_PARA_VERIFY(synce_ctrl);

    /* handle process */
    rc = adpt_enet_handle_get(dev_id, chnl_id, &digi_handle, &chnl_handle);
    ADPT_RETURN_VERIFY(rc);

    /* reset */
    PMC_MEMSET((void*)&synce_cfg, 0, sizeof(synce_cfg));
    /* memory copy */
    PMC_MEMCPY(&synce_cfg.ch_cfg[0], synce_ctrl, sizeof(digi_synce_ch_cfg_t));
    synce_cfg.ch_cfg[0].chnl_handle = chnl_handle;

    rc = digi_timesync_synce_tx_ssm(digi_handle, &synce_cfg);

    PMC_RETURN(rc);
} /* adpt_synce_tx_ssm_set */


/*
** End of file
*/
