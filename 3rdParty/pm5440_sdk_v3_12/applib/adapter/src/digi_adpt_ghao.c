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
*     The file describes the G.HAO Run-Time functionality.
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
* adpt_ghao_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables G.HAO configuration before performing any G.HAO operations.
*
*   For increase scenerio follow these steps:
*     -- adpt_ghao_en_set
*     -- adpt_ghao_lcr_size
*     -- [call sifd knife edge sequence here if SIFD is in the path]
*     -- adpt_ghao_lcr_knife_edge
*     -- adpt_ghao_bwr_setup
*     -- adpt_ghao_bwr_ramp_start
*     -- adpt_ghao_bwr_ramp_end
*
*   For decrease scenerio follow these steps:
*     -- adpt_ghao_en_set
*     -- adpt_ghao_bwr_setup
*     -- adpt_ghao_bwr_ramp_start
*     -- adpt_ghao_lcr_knife_edge
*     -- [call sifd knife edge sequence here if SIFD is in the path]
*     -- adpt_ghao_lcr_size
*     -- adpt_ghao_bwr_ramp_end
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   new_rx_ts_mask     - new Rx tribslot mask.
*   new_tx_ts_mask     - new Tx tribslot mask.
*   enable             - TRUE: enable G.HAO feature.
*                        FALSE: disable G.HAO feature.
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
PUBLIC PMC_ERROR adpt_ghao_en_set(UINT32 dev_id, 
                                UINT32 chnl_id,
                                UINT32 new_rx_ts_mask[3],
                                UINT32 new_tx_ts_mask[3],
                                BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    UINT32 ghao_id = 0;
    UINT32 src_chnl_id = 0;
    UINT32 src_index = 0;
    BOOL switch_en = FALSE;
    digi_cfg_container_t *adpt_handle = NULL;
    util_global_switch_data_t *switch_data1 = NULL;
    util_global_switch_data_t *switch_data2 = NULL;
    UINT32 ts_cnt = 0;
    UINT32 i = 0;
    UINT32 sifd_cal_mask[3];
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d][%d][%d][%d][%d][%d].\r\n", dev_id, 
               chnl_id, new_rx_ts_mask[0], new_rx_ts_mask[1], new_rx_ts_mask[2],
               new_tx_ts_mask[0], new_tx_ts_mask[1], new_tx_ts_mask[2], enable);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    /* get ODUk SW information */
    rc = adpt_packet_switch_get(dev_id, DIGI_SWITCH_ODUK, chnl_id, &src_chnl_id, &switch_en);
    ADPT_RETURN_VERIFY(rc);
    /* ODUk SW must be enabled before initializing G.HAO */
    if (!switch_en)
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* get the source index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, src_chnl_id, &src_index);
    ADPT_RETURN_VERIFY(rc);

    /* check handle information */
    if (NULL != adpt_handle->traffic_info[index].odu_handle)
    {
        switch_data1 = (util_global_switch_data_t*)adpt_handle->traffic_info[index].odu_handle;
    }
    else if (NULL != adpt_handle->traffic_info[index].ilkn_handle)
    {
        switch_data1 = (util_global_switch_data_t*)adpt_handle->traffic_info[index].ilkn_handle;
    }
    else if (NULL != adpt_handle->traffic_info[index].map_handle)
    {
        switch_data1 = (util_global_switch_data_t*)adpt_handle->traffic_info[index].map_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    if (NULL != adpt_handle->traffic_info[src_index].odu_handle)
    {
        switch_data2 = (util_global_switch_data_t*)adpt_handle->traffic_info[src_index].odu_handle;
    }
    else if (NULL != adpt_handle->traffic_info[src_index].ilkn_handle)
    {
        switch_data2 = (util_global_switch_data_t*)adpt_handle->traffic_info[src_index].ilkn_handle;
    }
    else if (NULL != adpt_handle->traffic_info[src_index].map_handle)
    {
        switch_data2 = (util_global_switch_data_t*)adpt_handle->traffic_info[src_index].map_handle;
    }
    else
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* check TS count */
    for (i = 0; i < 96; i++)
    {
        if ((new_rx_ts_mask[i/32] >> (i % 32)) & 1)
        {
            ts_cnt++;
        }
    }

    if (enable)
    {
        rc = digi_ghao_segment_prov(adpt_handle->digi_handle, 
                                    switch_data1,
                                    switch_data2,
                                    ts_cnt,
                                    &adpt_handle->traffic_info[index].rx_ts_mask[0],
                                    &adpt_handle->traffic_info[index].tx_ts_mask[0],
                                    new_rx_ts_mask,
                                    new_tx_ts_mask,
                                    sifd_cal_mask,
                                    sifd_cal_mask,
                                    adpt_handle->dev_info->pkt_size,
                                    adpt_handle->dev_info->header_size,
                                    &ghao_id,
                                    100000);
    }
    else
    {
        rc = digi_ghao_segment_deprov(adpt_handle->digi_handle, 
                                      adpt_handle->traffic_info[index].ghao_id);
    }

    /* record information */
    if (PMC_SUCCESS == rc)
    {
        adpt_handle->traffic_info[index].ghao_id     = ghao_id;
        adpt_handle->traffic_info[index].ghao_enable = enable;
    }

    PMC_RETURN(rc);
} /* adpt_ghao_en_set */


/*******************************************************************************
* adpt_ghao_step_lcr_size
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs the link connection resize step.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ghao_dir           - G.HAO datapath direction (TX or RX).
*   ghao_step          - step to perform in G.HAO sequence.
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
PUBLIC PMC_ERROR adpt_ghao_step_lcr_size(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_ghao_dir_t ghao_dir,
                                     digi_ghao_step_t ghao_step)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               ghao_dir, ghao_step);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (!(adpt_handle->traffic_info[index].ghao_enable))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    rc = digi_ghao_step_lcr_size(adpt_handle->digi_handle, 
                                 adpt_handle->traffic_info[index].ghao_id,
                                 ghao_dir,
                                 ghao_step);

    PMC_RETURN(rc);
} /* adpt_ghao_step_lcr_size */


/*******************************************************************************
* adpt_ghao_step_lcr_knife_edge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs the link connection resize knife edge step.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ghao_dir           - G.HAO datapath direction (TX or RX).
*   ghao_step          - step to perform in G.HAO sequence.
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
PUBLIC PMC_ERROR adpt_ghao_step_lcr_knife_edge(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_ghao_dir_t ghao_dir,
                                           digi_ghao_step_t ghao_step)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               ghao_dir, ghao_step);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (!(adpt_handle->traffic_info[index].ghao_enable))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    rc = digi_ghao_step_lcr_knife_edge(adpt_handle->digi_handle, 
                                       adpt_handle->traffic_info[index].ghao_id,
                                       ghao_dir,
                                       ghao_step);

    PMC_RETURN(rc);
} /* adpt_ghao_step_lcr_knife_edge */


/*******************************************************************************
* adpt_ghao_step_bwr_setup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets up the datapath for BWR step.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ghao_dir           - G.HAO datapath direction (TX or RX).
*   ghao_step          - step to perform in G.HAO sequence.
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
PUBLIC PMC_ERROR adpt_ghao_step_bwr_setup(UINT32 dev_id, 
                                       UINT32 chnl_id,
                                       digi_ghao_dir_t ghao_dir,
                                       digi_ghao_step_t ghao_step)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               ghao_dir, ghao_step);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (!(adpt_handle->traffic_info[index].ghao_enable))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    rc = digi_ghao_step_bwr_setup(adpt_handle->digi_handle, 
                                  adpt_handle->traffic_info[index].ghao_id,
                                  ghao_dir,
                                  ghao_step);

    PMC_RETURN(rc);
} /* adpt_ghao_step_bwr_setup */


/*******************************************************************************
* adpt_ghao_step_bwr_ramp_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Starts BWR ramp step.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ghao_dir           - G.HAO datapath direction (TX or RX).
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
PUBLIC PMC_ERROR adpt_ghao_step_bwr_ramp_start(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_ghao_dir_t ghao_dir)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d].\r\n", dev_id, chnl_id, ghao_dir);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (!(adpt_handle->traffic_info[index].ghao_enable))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    rc = digi_ghao_step_bwr_ramp_start(adpt_handle->digi_handle, 
                                       adpt_handle->traffic_info[index].ghao_id,
                                       ghao_dir);

    PMC_RETURN(rc);
} /* adpt_ghao_step_bwr_ramp_start */


/*******************************************************************************
* adpt_ghao_step_bwr_ramp_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This need to be called when the ramp is complete to put to datapath
*   back into normal operating more.  This API will deprovision the G.HAO
*   segment and free up the ghao_segment_id.
*   
*   For source nodes, call once RAMP #2 done interrupt is seen in MAPOTN-ODUKSC.
*   For intermediate nodes, call after source nodes have completed.
*
* INPUTS:
*   dev_id             - device identity.
*   chnl_id            - channel identity.
*   ghao_dir           - G.HAO datapath direction (TX or RX).
*   ghao_step          - step to perform in G.HAO sequence.
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
PUBLIC PMC_ERROR adpt_ghao_step_bwr_ramp_end(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_ghao_dir_t ghao_dir,
                                           digi_ghao_step_t ghao_step)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 index = 0;
    digi_cfg_container_t *adpt_handle = NULL;
    PMC_ENTRY();

    ADPT_TRACE("Input para[0x%x][%d][%d][%d].\r\n", dev_id, chnl_id,
               ghao_dir, ghao_step);

    /* get adapter container handle */
    rc = adpt_handle_get(dev_id, &adpt_handle);
    ADPT_RETURN_VERIFY(rc);

    /* get the index in traffic container */
    rc = adpt_chnl_index_get(adpt_handle, chnl_id, &index);
    ADPT_RETURN_VERIFY(rc);

    if (!(adpt_handle->traffic_info[index].ghao_enable))
    {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    rc = digi_ghao_step_bwr_ramp_end(adpt_handle->digi_handle, 
                                     adpt_handle->traffic_info[index].ghao_id,
                                     ghao_dir,
                                     ghao_step);

    PMC_RETURN(rc);
} /* adpt_ghao_step_bwr_ramp_end */


/*
** End of file
*/
