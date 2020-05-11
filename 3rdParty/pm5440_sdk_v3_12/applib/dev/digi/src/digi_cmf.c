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
*   DESCRIPTION : This module contains CMF related host messages.
*
*   NOTES:
*
*******************************************************************************/

#include "digi_loc.h"
#include "hostmsg_loc.h"


/******************************************************************************* 
 *
 * Constants.
 *
 ******************************************************************************/

/* Length of CMF host messages */
/* CMD_CFG_CMF_Send_One_Shot_CMF */
#define HOSTMSG_CMD_CFG_CMF_SEND_ONE_SHOT_CMF_LEN           0x5
/* CMD_REQ_CMF_Extract_CMF */
#define HOSTMSG_CMD_REQ_CMF_EXTRACT_CMF_LEN                 0x3
/* CMD_CFG_CMF_Send_One_Shot_OPU_CSF */
#define HOSTMSG_CMD_CFG_CMF_TRIGGER_OPU_CSF_LEN             0x3
/* CMD_CFG_CMF_Set_ClientToTrunk_Conseq_Action_Table */
#define HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_LEN 0x13
/* CMD_CFG_CMF_Set_ClientToTrunk_Port_Action_Table */
#define HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_PORT_ACTION_TABLE_LEN 0x8
/* CMD_CFG_CMF_Set_ClientToTrunk_Update_Tables */
#define HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_UPDATE_TABLES_LEN 0x3
/* CMD_CFG_CMF_Set_TrunkToClient_Conseq_Action_Table */
#define HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_LEN 0x19
/* CMD_CFG_CMF_Set_TrunkToClient_Port_Mapping_Table */
#define HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_PORT_MAPPING_TABLE_LEN 0x4
/* CMD_CFG_CMF_Set_TrunkToClient_Update_Tables */
#define HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_UPDATE_TABLES_LEN 0x3
/* CMD_CFG_CMF_Set_Trigger_LF */
#define HOSTMSG_CMD_CFG_CMF_TRIGGER_LF_LEN 0x3
/* CMD_CFG_CMF_Set_Trigger_RF */
#define HOSTMSG_CMD_CFG_CMF_TRIGGER_RF_LEN 0x3
/* CMD_CFG_CMF_SET_Enet_Emac_LF_RF_Cfg */
#define HOSTMSG_CMD_CFG_CMF_ENET_EMAC_LF_RF_LEN 0x4
/* CMD_REQ_CMF_Get_TrunkToClient_Conseq_Action_Table */
#define HOSTMSG_CMD_CMF_REQ_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_LEN 0x3
#define HOSTMSG_CMD_CMF_RSP_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_LEN 0x19
#define HOSTMSG_CMD_CMF_RSP_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_3_05_LEN 0x17
/* CMD_REQ_CMF_Get_ClientToTrunk_Conseq_Action_Table */
#define HOSTMSG_CMD_CMF_REQ_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_LEN 0x3
#define HOSTMSG_CMD_CMF_RSP_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_LEN 0x13
#define HOSTMSG_CMD_CMF_RSP_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_3_05_LEN 0x12
/* CMD_REQ_CMF_Get_ClientToTrunk_Port_Action_Table */
#define HOSTMSG_CMD_CMF_REQ_CLIENTTOTRUNK_PORT_ACTION_TABLE_LEN 0x3
#define HOSTMSG_CMD_CMF_RSP_CLIENTTOTRUNK_PORT_ACTION_TABLE_LEN 0x8
/* CMD_REQ_CMF_Get_TrunkToClient_Port_Mapping_Table */
#define HOSTMSG_CMD_CMF_REQ_TRUNKTOCLIENT_PORT_MAPPING_TABLE_LEN 0x3
#define HOSTMSG_CMD_CMF_RSP_TRUNKTOCLIENT_PORT_MAPPING_TABLE_LEN 0x4
/* CMD_CFG_CMF_CLIENTTOTRUNK_SW_FORCE */
#define HOSTMSG_CMD_CFG_CMF_CLIENTTOTRUNK_SW_FORCE_LEN             0x3
/* CMD_CFG_CMF_MAC_MAC_LF_RF_FORWARDING */
#define HOSTMSG_CMD_CFG_CMF_MAC_MAC_LF_RF_FORWARDING_LEN           0x3

/*******************************************************************************
 *
 * Private methods.
 *
 ******************************************************************************/

/*******************************************************************************
*  digi_cmf_insert_one_shot_gfp_cmf_internal_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This method creates CMF based on user input and sends a one shot CMF on
*   the given channel.
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   mapotn_channel - MAPOTN channel\n
*                  0~96
*   payload_length_type - flag to determine whether to include payload\n
*                  0~1
*   upi_value - UPI value to be send in the CMF
*                  0~255
*   genpayloadFCS - Indicates whether a 4 byte payload FCS should be 
*                   generated if client payload field is used (0~1).
*   insertion_result - returns if the CMF was successfully inserted\n
*                      0 ~ insertion did not succeed (HW may have been busy)\n
*                      1 ~ insertion was successful\n
*   digi_cmf_one_shot_cmf_ptr - Structure containing payload infomation.
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Send_One_Shot_CMF. 
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cmf_insert_one_shot_gfp_cmf_internal_cfg(digi_handle_t *digi_handle,
                                                                UINT32 mapotn_channel,
                                                                UINT32 payload_length_type,
                                                                UINT32 upi_value,
                                                                UINT32 genpayloadFCS,
                                                                BOOL8 *insertion_result,
                                                                digi_cmf_one_shot_cmf_t *digi_cmf_one_shot_cmf_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 outbound_msg_ctrl = 0;
    UINT32 data[3];

    PMC_ATOMIC_ENTRY(digi_handle);   

    /* Set insertion_result to false unless FW return success.*/
    *insertion_result = FALSE; 

    /* Words 1,2,3 of CMF_CFG_CMF_Send_One_Shot_CMF have the following format:
     * Word   bits 24..31       bits 16..23          bits 8..15       bits 0..7
     *   0  -mapotn_channel-|-payload_length_type-|----upi_value---|-genpayloadFCS--
     *   1  -cmf_payload[0]-|----cmf_payload[1]---|-cmf_payload[2]-|-cmf_payload[3]-
     *   2  -cmf_payload[4]-|----cmf_payload[5]---|-cmf_payload[6]-|-cmf_payload[7]-
     */
    data[0] = (((UINT16)mapotn_channel) << 24) | 
                  ((UINT8)payload_length_type << 16) |
                 ((UINT8)upi_value << 8) | 
                 ((UINT8)genpayloadFCS);
    data[1] = (((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[0]) << 24) | 
                  ((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[1] << 16) |
                 ((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[2] << 8) | 
                 ((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[3]);
    data[2] = (((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[4]) << 24) | 
                  ((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[5] << 16) |
                 ((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[6] << 8) | 
                 ((UINT8)digi_cmf_one_shot_cmf_ptr->cmf_payload[7]);

    /* Send message*/
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message_with_error_info((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_SEND_ONE_SHOT_CMF_ST,data,3,&outbound_msg_ctrl,stat);
    }

    if (PMC_SUCCESS == result)
    {
        *insertion_result = TRUE;
    }

    if (PMCFW_ERR_HOSTMSG_RX_HARDWARE_UNAVAILABLE == outbound_msg_ctrl)
    {
        PMC_LOG_TRACE("FW returned HW unavailable code, HW may have been at full bandwidth and could not insert CMF frame %d\n", outbound_msg_ctrl);
        /* Return PMC_SUCCESS in this case but return failure to insert in output. */
        result = PMC_SUCCESS;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_insert_one_shot_gfp_cmf_internal_cfg*/

/*******************************************************************************
* digi_cmf_extract_gfp_cmf_internal_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Extracts the CMF from either the CPU FIFO or from the memory where the
*  last stored CMF was saved after being extracted after an interrupt 
*
* INPUTS:
*  digi_handle - DIGI device handle.\n
*  mapotn_channel   - channel in mapotn from where to retrieve CMF\n
*  extract_mode - CMF extraction mode\n
*                 0 ~ extract directly from CPU FIFO\n
*                 1 ~ extract last retrieved CMF via interrupt driven method (saved in memory).
*
* OUTPUTS:
*  digi_cmf_extract_cmf_ptr   - Structure containing the extracted CMF information\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:      CMD_REQ_CMF_Extract_CMF
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cmf_extract_gfp_cmf_internal_get(digi_handle_t *digi_handle,
                                         UINT32 mapotn_channel,
                                         UINT32 extract_mode,
                                         digi_cmf_extract_cmf_t *digi_cmf_extract_cmf_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[4];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_cmf_extract_cmf_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    memset(digi_cmf_extract_cmf_ptr, 0, sizeof(digi_cmf_extract_cmf_t));

    /* Build the rest of the message */
    data[0] = ((UINT8)mapotn_channel << 24) | ((UINT8)extract_mode << 16) | 0x0000;

    /* Send message. */
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_REQ_EXTRACT_CMF_ST,data,1,response,4,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        /* Return requested data. */
        digi_cmf_extract_cmf_ptr->mapotn_ch = (response[0] >> 24) & 0xff;
        digi_cmf_extract_cmf_ptr->cmf_length = (response[0] >> 16) & 0xff;
        digi_cmf_extract_cmf_ptr->retrieved_cmf_valid = (response[0] >> 8) & 0xff;

        *digi_cmf_extract_cmf_ptr->cmf_typeheader = (response[1] >> 24) & 0xff;
        *(digi_cmf_extract_cmf_ptr->cmf_typeheader + 1) = (response[1] >> 16) & 0xff;
        *(digi_cmf_extract_cmf_ptr->cmf_typeheader + 2) = (response[1] >> 8) & 0xff;
        *(digi_cmf_extract_cmf_ptr->cmf_typeheader + 3) = (response[1] >> 0) & 0xff;

        if (digi_cmf_extract_cmf_ptr->cmf_length == 0xC) {
            *digi_cmf_extract_cmf_ptr->cmf_payload = (response[2] >> 24) & 0xff;
            *(digi_cmf_extract_cmf_ptr->cmf_payload + 1) = (response[2] >> 16) & 0xff;
            *(digi_cmf_extract_cmf_ptr->cmf_payload + 2) = (response[2] >> 8) & 0xff;
            *(digi_cmf_extract_cmf_ptr->cmf_payload + 3) = (response[2] >> 0) & 0xff;

            *(digi_cmf_extract_cmf_ptr->cmf_payload + 4) = (response[3]>> 24) & 0xff;
            *(digi_cmf_extract_cmf_ptr->cmf_payload + 5) = (response[3]>> 16) & 0xff;
            *(digi_cmf_extract_cmf_ptr->cmf_payload + 6) = (response[3]>> 8) & 0xff;
            *(digi_cmf_extract_cmf_ptr->cmf_payload + 7) = (response[3]>> 0) & 0xff;
        }
        
    } /* end if */

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_cmf_extract_gfp_cmf_internal_get() */

/*******************************************************************************
*  digi_cmf_trigger_opu_csf_internal_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION: This function triggers to either send or stop sending an OPU CSF on the given channel.
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   mapotn_channel - MAPOTN channel 0~96
*   mapper - GFP or GMP mapper \n
*                  0~GFP mapper\n
*                  1~GMP mapper
*   trigger - Indicates whether the OPU CSF should be inserted or stop inserting OPU CSF.
*                  0~Remove OPU CSF\n
*                  1~Insert OPU CSF\n
*                  2~Release control of the OPU CSF
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*    
*  NOTES:
*   CMD_CFG_CMF_Trigger_OPU_CSF. 
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cmf_trigger_opu_csf_internal_cfg(digi_handle_t *digi_handle,
                                               UINT32 mapotn_channel,
                                               UINT32 mapper,
                                               digi_cmf_sw_control_action_t trigger)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Word 0 of CMD_CMF_CFG_TRIGGER_OPU_CSF has the following format:
     * Word   bits 24..31     bits 16..23   bits 8..15    bits 0..7
     *   0  -mapotn_channel-|---mapper---|---trigger---|---reserved---
     */        
    data[0] = (((UINT8)mapotn_channel) << 24) | 
                 ((UINT8)mapper << 16) | 
                 ((UINT8)trigger << 8);

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_TRIGGER_OPU_CSF_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trigger_opu_csf_internal_cfg*/    



/*******************************************************************************
*  digi_cmf_clienttotrunk_port_action_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               digi_cmf_clienttotrunk_port_action_get helps retrieve the port action table
*               for the client to trunk direction.
* 
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               which has the mapotn mapping.
*               client2trunk_tables_ptr - a pointer to the port action table. 
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*              This function helps set up:
*              CMD_REQ_CMF_Get_ClientToTrunk_Port_Action_Table 
* 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_port_action_get(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               digi_client2trunk_port_action_table_cmf_t  *client2trunk_tables_ptr)
{

    PMC_ERROR result = PMC_SUCCESS; 
    /* Used to retrieve the enet port and channel from the mapotn channel*/
    util_global_switch_data_def_t *local_output_sw_ptr; 
    util_global_cpb_port_data_t *local_output_pc_ptr;
    util_global_cpb_port_data_t local_input_pc;
    BOOL8 is_multicast = FALSE;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    /* Used to extract the mapping mode */
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;

    cpb_handle_t* cpb_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    local_output_sw_ptr = &(mapper_chnl_handle_ptr->switch_data);
    local_output_pc_ptr = &(local_output_sw_ptr->cpb_data);

    cpb_handle = digi_handle->dcpb_handle;

    result = cpb_chnl_source_type_and_primary_test(cpb_handle,
                                      local_output_pc_ptr->port_type,
                                      local_output_pc_ptr->channel,
                                      (UINT32*)&(local_input_pc.port_type),
                                      &(local_input_pc.channel),
                                      &is_broadcast,
                                      &is_multicast,
                                      &is_reader_primary,
                                      &is_slave_zone_primary);

    if (PMC_SUCCESS == result)
    {
        /* update FW enet client source */
        if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == local_input_pc.port_type)
        {
            result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_line_chnl[local_input_pc.channel]);
        }
        else if (UTIL_GLOBAL_CPB_PORT_ENET_SYS == local_input_pc.port_type)
        {
            result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_sys_chnl[local_input_pc.channel]);
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_cmf_clienttotrunk_port_action_table_internal_get(digi_handle,
                                                                       local_input_pc.channel,
                                                                       client2trunk_tables_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_clienttotrunk_port_action_get*/ 


/*******************************************************************************
*  digi_cmf_trunktoclient_port_mapping_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function retrieves the consequential action table
*               for the trunk to client direction.
* 
*               This API is only to be called after a full datapath provisioning
*               and activation.
* 
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               trunk2client_tables_ptr - a pointer to the port mapping table.
*                                         
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               This is a helper function to properly set up
*               CMD_REQ_CMF_Get_TrunkToClient_Port_Mapping_Table
*               
* 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_port_mapping_get(digi_handle_t *digi_handle,
                                                         digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                         cmf_trunk2client_portMapping_t  *trunk2client_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = digi_cmf_trunktoclient_port_mapping_table_internal_get(digi_handle,
                                                                     mapotn_chnl,
                                                                     trunk2client_tables_ptr );

    PMC_ATOMIC_RETURN(digi_handle, result);

} /* End of digi_cmf_trunktoclient_port_mapping_get*/ 

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
*  digi_cmf_clienttotrunk_conseq_action_table_internal_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will configure the consequential action table in FW 
*               for the client to trunk direction. This table will be stored in FW
*               but will not be activated until 'digi_cmf_clienttotrunk_tables_update' is
*               called. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               enet_link - enet link this table is attributed to.
*   			client2trunk_tables_ptr - pointer to the client to trunk
*   									conseq action table.
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_CMF_Set_ClientToTrunk_Conseq_Action_Table. 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_table_internal_set(digi_handle_t *digi_handle,
                                               UINT32 enet_link,
                                               digi_client2trunk_conseq_action_t  *client2trunk_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[18];

    PMC_ENTRY();

    /* Words 0-15 of CMF_CFG_CMF_Set_ClientToTrunk_Conseq_Action_Table have the following format:
     * Word   bits 24..31                     bits 16..23            bits 8..15        bits 0..7
     *   0  ---enet_link------|--------------------------reserved-------------------------------
     *   1  ---enet_emac_phy_los_mon_v--|--action_1_phy_los---|--action_2_phy_los---|---priority_phy_los--
     *   2   -enet_emac_block_lock_mon_v-|-action_1_block_lock-|-action_2_block_lock-|-priority_block_lock-
     *   3   ---enet_emac_hi_ber_mon_v---|---action_1_hi_ber---|---action_2_hi_ber---|---priority_hi_ber---
     *   4   enet_emac_rx_loc_fault_mon_v|action_1_rx_loc_fault|action_2_rx_loc_fault|priority_rx_loc_fault
     *   5   enet_emac_rx_rem_fault_mon_v|action_1_rx_rem_fault|action_2_rx_rem_fault|priority_rx_rem_fault
     *   6   --cbrc_8b10b_loss_sync_mon_v---|--action_1_fc800_8b10b_loss_sync---|--action_2_fc800_8b10b_loss_sync---|--priority_fc800_8b10b_loss_sync---
     *   7   fc1200_loss_sync_mon_v |action_1_fc1200_loss_sync|action_2_fc1200_loss_sync|priority_fc1200_loss_sync
     *   8   sonet_loss_signal_mon_v|action_1_sonet_loss_sync|action_2_sonet_loss_sync|priority_sonet_loss_sync
     *   9   sonet_loss_frame_mon_v|action_1_sonet_loss_frame|action_2_sonet_loss_frame|priority_sonet_loss_frame
     *   10  cbrc_8b10b_loss_signal_mon_v|action_1_cbrc_8b10b_loss_signal|action_2_cbrc_8b10b_loss_signal|priority_cbrc_8b10b_loss_signal
     *   11  fc1200_loss_signal_mon_v|action_1_fc1200_loss_signal|action_2_fc1200_loss_signal|priority_fc1200_loss_signal
     *   12  enet_epmm_loss_signal_mon_v|action_1_enet_epmm_loss_signal|action_2_enet_epmm_loss_signal|priority_enet_epmm_loss_signal
     *   13  enet_epmm_local_fault_mon_v|action_1_enet_epmm_local_fault|action_2_enet_epmm_local_fault|priority_enet_epmm_local_fault
     *   14  enet_epmm_remote_fault_mon_v|action_1_enet_epmm_remote_fault|action_2_enet_epmm_remote_fault|priority_enet_epmm_remote_fault
	 *   15  sw_force_manual_trigger_mon_v|action_1_sw_force_manual_trigger|action_2_sw_force_manual_trigger|priority_sw_force_manual_trigger
	 *   16  enet_mac_40g_100g_loam_mon_v|enet_mac_40g_100g_loam_mon_v|enet_mac_40g_100g_loam_mon_v|enet_mac_40g_100g_loam_mon_v
     */        
    data[0] = ( ( (UINT8)enet_link << 24) | 
                  0x000000 );

	data[1] = ( ((UINT8)client2trunk_tables_ptr->enet_emac_phy_los_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_phy_los_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->enet_emac_phy_los_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_phy_los_mon.priority_idx ) );

	data[2] = ( ((UINT8)client2trunk_tables_ptr->enet_emac_block_lock_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_block_lock_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->enet_emac_block_lock_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_block_lock_mon.priority_idx ) );

	data[3] = ( ((UINT8)client2trunk_tables_ptr->enet_emac_hi_ber_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_hi_ber_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->enet_emac_hi_ber_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_hi_ber_mon.priority_idx ) );

	data[4] = ( ((UINT8)client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.priority_idx ) );

	data[5] = ( ((UINT8)client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.priority_idx ) );

	data[6] = ( ((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.priority_idx ) );

	data[7] = ( ((UINT8)client2trunk_tables_ptr->fc1200_loss_sync_mon.monitor_defect << 24) | 
                        ((UINT8)client2trunk_tables_ptr->fc1200_loss_sync_mon.action1 << 16) |
                        ((UINT8)client2trunk_tables_ptr->fc1200_loss_sync_mon.action2 << 8) | 
                        ((UINT8)client2trunk_tables_ptr->fc1200_loss_sync_mon.priority_idx ) );

	data[8] = ( ((UINT8)client2trunk_tables_ptr->sonet_loss_signal_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->sonet_loss_signal_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->sonet_loss_signal_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->sonet_loss_signal_mon.priority_idx ) );

	data[9] = ( ((UINT8)client2trunk_tables_ptr->sonet_loss_frame_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->sonet_loss_frame_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->sonet_loss_frame_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->sonet_loss_frame_mon.priority_idx ) );

	data[10] = ( ((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.priority_idx ) );

	data[11] = ( ((UINT8)client2trunk_tables_ptr->fc1200_loss_signal_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->fc1200_loss_signal_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->fc1200_loss_signal_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->fc1200_loss_signal_mon.priority_idx ) );

	data[12] = ( ((UINT8)client2trunk_tables_ptr->enet_epmm_loss_signal_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->enet_epmm_loss_signal_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->enet_epmm_loss_signal_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->enet_epmm_loss_signal_mon.priority_idx ) );

	data[13] = ( ((UINT8)client2trunk_tables_ptr->enet_epmm_local_fault_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->enet_epmm_local_fault_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->enet_epmm_local_fault_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->enet_epmm_local_fault_mon.priority_idx ) );

	data[14] = ( ((UINT8)client2trunk_tables_ptr->enet_epmm_remote_fault_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->enet_epmm_remote_fault_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->enet_epmm_remote_fault_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->enet_epmm_remote_fault_mon.priority_idx ) );

	data[15] = ( ((UINT8)client2trunk_tables_ptr->sw_force_manual_trigger_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->sw_force_manual_trigger_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->sw_force_manual_trigger_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->sw_force_manual_trigger_mon.priority_idx ) );

	data[16] = ( ((UINT8)client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.monitor_defect << 24) | 
						((UINT8)client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.action1 << 16) |
						((UINT8)client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.action2 << 8) | 
						((UINT8)client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.priority_idx ) );

   /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_ST,data,17,stat);
    }

    PMC_RETURN(result);

} /* End of digi_cmf_clienttotrunk_conseq_action_table_internal_set*/ 
  
/*******************************************************************************
*  digi_cmf_clienttotrunk_conseq_action_table_internal_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will retrieve the consequential action table in FW 
*               for the client to trunk direction. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               enet_cbrc_link - enet/cbrc link this table is attributed to.
*               client2trunk_tables_ptr - clienttotrunk conseq action table
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_REQ_CMF_Get_ClientToTrunk_Conseq_Action_Table. 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 enet_cbrc_link,
                                                                         digi_client2trunk_conseq_action_t  *client2trunk_tables_ptr )
{
    UINT32 response_msg_len;
    UINT32 data_var;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[17];       

    PMC_ENTRY();

    /* Word 0 of CMF_REQ_CMF_Get_ClientToTrunk_Conseq_Action_Table have the following format:
     * Word   bits 24..31               bits 16..23            bits 8..15        bits 0..7
     *   0  --------------------------reserved-------------------------------|---enet_link------
     */
    data[0] = ( (UINT8)enet_cbrc_link & 0xFF );

    if ( digi_fw_is_ver_3_05(digi_handle) )
    {
        response_msg_len = 16;
    } else {
        response_msg_len = 17;
    }

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_REQ_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_ST,data,1,response,response_msg_len,stat);
    }

    /* Get result*/
    if (PMC_SUCCESS == result) {

        data_var = response[1];
        client2trunk_tables_ptr->enet_emac_phy_los_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) ) ;
        client2trunk_tables_ptr->enet_emac_phy_los_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_emac_phy_los_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF ));  
        client2trunk_tables_ptr->enet_emac_phy_los_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[2];
        client2trunk_tables_ptr->enet_emac_block_lock_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_emac_block_lock_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_emac_block_lock_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_emac_block_lock_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));
  
        data_var = response[3];
        client2trunk_tables_ptr->enet_emac_hi_ber_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_emac_hi_ber_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_emac_hi_ber_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_emac_hi_ber_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[4];
        client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_emac_rx_loc_fault_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[5];
        client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_emac_rx_rem_fault_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[6];
        client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->cbrc_8b10b_loss_sync_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[7];
        client2trunk_tables_ptr->fc1200_loss_sync_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->fc1200_loss_sync_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->fc1200_loss_sync_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->fc1200_loss_sync_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[8];
        client2trunk_tables_ptr->sonet_loss_signal_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->sonet_loss_signal_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->sonet_loss_signal_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->sonet_loss_signal_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[9];
        client2trunk_tables_ptr->sonet_loss_frame_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->sonet_loss_frame_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->sonet_loss_frame_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->sonet_loss_frame_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[10];
        client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->cbrc_8b10b_loss_signal_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[11];
        client2trunk_tables_ptr->fc1200_loss_signal_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->fc1200_loss_signal_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->fc1200_loss_signal_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF ));
        client2trunk_tables_ptr->fc1200_loss_signal_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[12];
        client2trunk_tables_ptr->enet_epmm_loss_signal_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_epmm_loss_signal_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_epmm_loss_signal_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_epmm_loss_signal_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[13];
        client2trunk_tables_ptr->enet_epmm_local_fault_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_epmm_local_fault_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_epmm_local_fault_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_epmm_local_fault_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[14];
        client2trunk_tables_ptr->enet_epmm_remote_fault_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->enet_epmm_remote_fault_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->enet_epmm_remote_fault_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->enet_epmm_remote_fault_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        if ( !digi_fw_is_ver_3_05(digi_handle) )
        {
            data_var = response[15];
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));
        } else{
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.monitor_defect = 0;   
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.action1 = (digi_client2trunk_actions_t)0;
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.action2 = (digi_client2trunk_actions_t)0; 
            client2trunk_tables_ptr->sw_force_manual_trigger_mon.priority_idx = 0;
        }

		data_var = response[16];
		client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
		client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.action1 = ( (digi_client2trunk_actions_t)((data_var >> 16) & 0xFF ));
		client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.action2 = ( (digi_client2trunk_actions_t)((data_var >> 8) & 0xFF )); 
		client2trunk_tables_ptr->enet_mac_40g_100g_loam_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));


    }

    PMC_RETURN(result);

} /* End of digi_cmf_clienttotrunk_conseq_action_table_internal_get*/ 
  
/*******************************************************************************
*  digi_cmf_clienttotrunk_port_action_table_internal_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               digi_cmf_clienttotrunk_port_action_table_internal_get helps retrieve the port action table in FW 
*               for the client to trunk direction. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               enet_cbrc_link - enet/cbrc link this table is attributed to.
*               client2trunk_tables_ptr - clienttotrunk port action table
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_REQ_CMF_Get_ClientToTrunk_Port_Action_Table. 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_port_action_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 enet_cbrc_link,
                                                                         digi_client2trunk_port_action_table_cmf_t  *client2trunk_tables_ptr )
{
    UINT32 data_var;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[6];       

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    /* Words 0 of CMF_REQ_CMF_Get_ClientToTrunk_Port_Action_Table have the following format:
     * Word   bits 24..31               bits 16..23        bits 8..15           bits 0..7
     *   0  --------------------------reserved-------------------------------|---enet_link------
     */
        
    data[0] = ( (UINT8)enet_cbrc_link & 0xFF );

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_REQ_CLIENTTOTRUNK_PORT_ACTION_TABLE_ST,data,1,response,6,stat);
    }

    if (PMC_SUCCESS == result) {

        data_var = response[1];
        client2trunk_tables_ptr->enable_monitoring = ( (UINT8)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->is_send_dci = ( (UINT8)((data_var >>  0) & 0xFF));
  
        data_var = response[2];
        client2trunk_tables_ptr->is_gen_keep_alive_cmf = ( (UINT8)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->upi_for_keep_alive_cmf = ( (UINT8)((data_var >> 16) & 0xFF ));
        client2trunk_tables_ptr->is_gen_payload_fcs = ( (UINT8)((data_var >> 8) & 0xFF )); 
        client2trunk_tables_ptr->is_use_payload_csf = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[3];
        client2trunk_tables_ptr->cmf_payload[0] = ( (UINT8)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->cmf_payload[1] = ( (UINT8)((data_var >> 16) & 0xFF) ); 
        client2trunk_tables_ptr->cmf_payload[2] = ( (UINT8)((data_var >> 8) & 0xFF) ); 
        client2trunk_tables_ptr->cmf_payload[3] = ( (UINT8)((data_var >> 0) & 0xFF) ); 

        data_var = response[4];
        client2trunk_tables_ptr->cmf_payload[4] = ( (UINT8)((data_var >> 24) & 0xFF) );   
        client2trunk_tables_ptr->cmf_payload[5] = ( (UINT8)((data_var >> 16) & 0xFF) ); 
        client2trunk_tables_ptr->cmf_payload[6] = ( (UINT8)((data_var >> 8) & 0xFF) ); 
        client2trunk_tables_ptr->cmf_payload[7] = ( (UINT8)((data_var >> 0) & 0xFF) ); 
    }

    PMC_RETURN(result);

} /* End of digi_cmf_clienttotrunk_port_action_table_internal_get*/   
  
/*******************************************************************************
*  digi_cmf_clienttotrunk_port_action_table_internal_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will configure the port action table in FW 
*               for the client to trunk direction. This tables will be stored in FW
*               but will not be activated until 'digi_cmf_clienttotrunk_tables_update' is
*               called. 
*
*  INPUTS:
*               digi_handle - digi device handle.
*               enet_link - enet link this table is attributed to
*               client_type - Client type of datapath. ENET ~ 0, CBRC ~ 1
*               mapotn_chnl - mapotn corresponding to this enet link
*               enable_monitoring - Indicator whether this table should be enabled
*               mapper - bit that indicates whether this mapotn channel is GFP mapped (0)
*                        of GMP mapped (1)
*               epmm_legacy_mode - epmm legacy mode flag. Tells FW if it is in legacy mode. 
*               is_send_dci - 1 ~ Send a DCI when action is deasserted\n
*                           0 ~ Do not send a DCI when action is deasserted
*               is_gen_keep_alive_cmf - 1 ~ Generate a keep alive CMF every 100ms on this mapotn channel\n
*                                   0 ~ Do not generate a keep alive CMF
*               upi_for_keep_alive_cmf - 0 ~ 255 ~ value of UPI in CMF to be inserted in both keep alive CMF\n
*                                    and consequential action CMF if applicable
*               is_gen_payload_fcs - 1 ~ Generate FCS for the both keep alive CMF and consequential action CMF if applicable\n
*                                 0 ~ Do not generate FCS 
*               is_use_payload_csf - 1 ~ Insert payload to both keep alive CMF and consequential action CMF if applicable\n
*                                 0 ~ Do not use payload CSF
*               digi_cmf_payload_ptr - Pointer at the array holding the payload information.
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:       CMD_CFG_CMF_Set_ClientToTrunk_Port_Action_Table 
*               
*
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_port_action_table_internal_set(digi_handle_t *digi_handle,
                                                                       UINT32 enet_link,
                                                                       UINT32 client_type,
                                                                       UINT32 mapotn_chnl,
                                                                       UINT32 mapper, 
                                                                       UINT32 epmm_legacy_mode,
                                                                       UINT32 enable_monitoring,
                                                                       UINT32 is_send_dci,
                                                                       UINT32 is_gen_keep_alive_cmf,
                                                                       UINT32 upi_for_keep_alive_cmf,
                                                                       UINT32 is_gen_payload_fcs,
                                                                       UINT32 is_use_payload_csf,
                                                                       UINT8 *digi_cmf_payload_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[6];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    /* Words 0..4 of CMF_CFG_CMF_Send_One_Shot_CMF have the following format:
     * Word   bits 24..31               bits 16..23        bits 8..15           bits 0..7
     *   0  ---enet_link-----|-----client_type-----|-epmm_legacy_mode-|---reserved----
     *   1  enable_monitoring|-----mapotn_chnl-----|-----mapper-----|---is_send_dci----
     *   2  is_gen_keep_alive_cmf|--upi_for_keep_alive_cmf-|is_gen_payload_fcs-|is_use_payload_csf-
     *   3  -cmf_payload[0]--|----cmf_payload[1]---|-cmf_payload[2]-|-cmf_payload[3]-
     *   4  -cmf_payload[4]--|----cmf_payload[5]---|-cmf_payload[6]-|-cmf_payload[7]-       
     */        
    data[0] = ( ((UINT8)enet_link << 24) | 
                  (UINT8)client_type << 16 |
                    (UINT8)epmm_legacy_mode << 8 |
                    0x00 );
    data[1] = ( ((UINT8)enable_monitoring << 24) | 
                  ((UINT8)mapotn_chnl << 16) |
                 ((UINT8)mapper << 8) | 
                 ((UINT8)is_send_dci) );
    data[2] = ( ((UINT8)is_gen_keep_alive_cmf << 24) | 
                  ((UINT8)upi_for_keep_alive_cmf << 16) |
                 ((UINT8)is_gen_payload_fcs << 8) | 
                 ((UINT8)is_use_payload_csf) );
    data[3] = ( (*(digi_cmf_payload_ptr) << 24) | 
                  (*(digi_cmf_payload_ptr +1)  << 16) |
                 (*(digi_cmf_payload_ptr+2) << 8) | 
                 (*(digi_cmf_payload_ptr+3) << 0) );
    data[4] = ( (*(digi_cmf_payload_ptr +4) << 24) | 
                  (*(digi_cmf_payload_ptr +5)  << 16) |
                 (*(digi_cmf_payload_ptr+6) << 8) | 
                 (*(digi_cmf_payload_ptr+7) << 0) );
    data[5] = 0;

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_PORT_ACTION_TABLE_ST,data,6,stat);
    }

    PMC_RETURN(result);

} /* End of digi_cmf_clienttotrunk_port_action_table_internal_set*/  
 

/*******************************************************************************
*  digi_cmf_trunktoclient_conseq_action_table_internal_set
*  ___________________________________________________________________________
*
*  DESCRIPTION: This function will set up the consequential action table for the
*              trunk to client direction. This table will be stored in FW
*               but will not be activated until 'digi_cmf_trunktoclient_tables_update' is
*               called. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               mapotn_channel - mapotn channel this table is attributed to.
*               trunk2client_tables_ptr - trunk2client conseq action table
*               
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*   PMC_SUCCESS - if everything went OK, number of error codes otherwise.
*
*  NOTES:       CMD_CFG_CMF_Set_TrunkToClient_Conseq_Action_Table
*               
*
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_channel, 
                                                                         digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;      
    UINT32 data[25];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    /* Words 0..22 of CMD_CFG_CMF_Set_TrunkToClient_Conseq_Action_Table have the following format:
     * Word     bits 24..31             bits 16..23        bits 8..15           bits 0..7
     *   0  --mapotn_channel----|--------------------------reserved-----------------------------
     *   1  -ssf_sf_rcvd_mon----|--action_ssf_sf_rcvd--|--action2_ssf_sf_rcvd--|---priority_ssf_sf_rcvd----
     *   2  ----action2_holdoff_time_ssf_sf_rcvd-------|----------------reserved------------
     *   3  ----dplm_mon--------|----action_dplm-------|-----action2_dplm------|---priority_dplm----
     *   4  -------action2_holdoff_time_dplm-----------|----------------reserved-----------------
     *   5  ---opu_csf_mon------|----action_opu_csf----|----action2_opu_csf----|---priority_opu_csf----
     *   6  -----action2_holdoff_time_opu_csf----------|-----------------reserved----------------
     *   7  ----gfp_lofd_mon--------|-----action_lofd------|------action2_lofd-----|---priority_lofd----
     *   8   ---------action2_holdoff_time_lofd---------|-----------------reserved---------------
     *   9   gfp_pti_upi_mismatch_mon|-action_pti_upi_mismatch-|-action2_pti_upi_mismatch-|-priority_pti_upi_mismatch--
     *   10  ---action2_holdoff_time_pti_upi_mismatch---|------------------reserved-----------------
     *   11  gfp_csf_loss_client_mon|action_gfp_csf_loss_client|-action2_gfp_csf_loss_client-|-priority_gfp_csf_loss_client--
     *   12  -action2_holdoff_time_gfp_csf_loss_client--|------------------reserved------------------
     *   13  gfp_csf_loss_char_mon|action_gfp_csf_loss_char|-action2_gfp_csf_loss_char-|-priority_gfp_csf_loss_char--
     *   14  ----action2_holdoff_time_gfp_csf_loss_char-|------------------reserved-------------------
     *   15  ----gfp_fdi_mon----|----action_gfp_fdi----|-----action2_gfp_fdi---|----priority_gfp_fdi---
     *   16  ----action2_holdoff_time_gfp_fdi----------|-------------------reserved----------------------
     *   17  ----gfp_rdi_mon----|----action_gfp_rdi----|----action2_gfp_rdi----|---priority_gfp_rdi----
     *   18  ----action2_holdoff_time_gfp_rdi----------|-----------------reserved--------------------
     *   19  --sonet_sfis_lof_mon--|--action_sonet_sfis_lof--|--action2_sonet_sfis_lof--|---priority_sonet_sfis_lof----
     *   20  ---action2_holdoff_time_sonet_sfis_lof----|-----------------reserved---------------------
     *   21  --gfp_dexm_mon--|--action_gfp_dexm--|--action2_gfp_dexm--|---priority_gfp_dexm----
     *   22  ---action2_holdoff_time_gfp_dexm----|-----------------reserved---------------------
     *   23  --emac_lf_and_phy_los_mon--|--action_emac_lf_and_phy_los--|--action2_emac_lf_and_phy_los--|---priority_emac_lf_and_phy_los----
     *   24  ---action2_holdoff_time_emac_lf_and_phy_los----|-----------------reserved---------------------
     */        
    data[0] = ( ( (UINT8)mapotn_channel << 24) | 
                  0x000000 );
    data[1] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.ssf_sf_rcvd_mon.monitor_defect << 24) | 
                        ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.ssf_sf_rcvd_mon.action1 << 16) |
                        ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.ssf_sf_rcvd_mon.action2 << 8) | 
                        ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.ssf_sf_rcvd_mon.priority_idx << 0)  );

    data[2] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.ssf_sf_rcvd_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[3] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.dplm_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.dplm_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.dplm_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.dplm_mon.priority_idx << 0) );

    data[4] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.dplm_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[5] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.priority_idx << 0) );

    data[6] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[7] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_lofd_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_lofd_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_lofd_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_lofd_mon.priority_idx << 0) );

    data[8] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_lofd_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[9] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_pti_upi_mismatch_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_pti_upi_mismatch_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_pti_upi_mismatch_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_pti_upi_mismatch_mon.priority_idx << 0) );  
            
    data[10] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_pti_upi_mismatch_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[11] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.priority_idx << 0) ); 

    data[12] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_client_signal_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[13] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.priority_idx << 0) ); 

    data[14] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_csf_loss_char_sync_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[15] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_fdi_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_fdi_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_fdi_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_fdi_mon.priority_idx << 0) ); 

    data[16] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_fdi_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[17] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_rdi_mon.monitor_defect << 24) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_rdi_mon.action1 << 16) |
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_rdi_mon.action2 << 8) | 
                    ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_rdi_mon.priority_idx << 0) ); 

    data[18] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_cmf_rdi_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[19] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.sonet_sfis_lof_mon.monitor_defect << 24) | 
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.sonet_sfis_lof_mon.action1 << 16) |
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.sonet_sfis_lof_mon.action2 << 8) | 
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.sonet_sfis_lof_mon.priority_idx << 0) ); 

    data[20] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.sonet_sfis_lof_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[21] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_dexm_mon.monitor_defect << 24) | 
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_dexm_mon.action1 << 16) |
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_dexm_mon.action2 << 8) | 
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.gfp_dexm_mon.priority_idx << 0) ); 

    data[22] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.gfp_dexm_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

    data[23] = ( ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.emac_lf_and_phy_los_mon.monitor_defect << 24) | 
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.emac_lf_and_phy_los_mon.action1 << 16) |
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.emac_lf_and_phy_los_mon.action2 << 8) | 
                ((UINT8)trunk2client_tables_ptr->conseq_action_cmf.emac_lf_and_phy_los_mon.priority_idx << 0) ); 

    data[24] = ( ((UINT16)trunk2client_tables_ptr->conseq_action_cmf.emac_lf_and_phy_los_mon.second_action_holdoff_time << 16) | 
                    0x0000 );

                                                                             
    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_ST,data,25,stat);
    }


    PMC_RETURN(result);

} /* End of digi_cmf_trunktoclient_conseq_action_table_internal_set*/ 
  
/*******************************************************************************
*  digi_cmf_trunktoclient_port_mapping_table_internal_set
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will set up the port mapping table for the
*               trunk to client direction. This tables will be stored in FW
*               but will not be activated until 'digi_cmf_trunktoclient_tables_update' is
*               called. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               mapotn_chnl - mapotn channel this table is attributed to
*               mapper - Indicates whether this mapotn channel is GFP mapped (0)
*                        of GMP mapped (1)
*               enet_link - enet link that this mapotn channel maps to
*               opu_csf_persistance_time - variable to set for how long the opu_csf must be
*                       persistant before it can be be considered a defect (3 frames) (ODU4 ~ 900us (~1ms), ODU0 ~ 76ms).
*                       This is given in increments of 1ms.
*               client_type - Client type for the datapath. 0 ~ ENET, 1 - CBRC
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_CMF_Set_TrunkToClient_Port_Mapping_Table
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_chnl,
                                                                         UINT32 mapper, 
                                                                         UINT32 enet_link,
                                                                         UINT32 opu_csf_persistance_time,
                                                                         UINT32 client_type )
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    /* Words 0,1 of CMF_CFG_CMF_Send_One_Shot_CMF have the following format:
     * Word     bits 24..31             bits 16..23        bits 8..15           bits 0..7
     *   0  ---mapotn_chnl--|----------------------reserved---------------------------
     *   1  ----mapper------|--enet_link---|-opu_csf_persistance_time-|--client_type--  
     */
    data[0] = ( ((UINT8)mapotn_chnl << 24) | 
                  0x000000 );
    data[1] = ( ((UINT8)mapper << 24) | 
                  ((UINT8)enet_link << 16) |
                 ((UINT8)opu_csf_persistance_time << 8) | 
                 (UINT8)client_type );

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_PORT_MAPPING_TABLE_ST,data,2,stat);
    }

    PMC_RETURN(result);

} /* End of digi_cmf_trunktoclient_port_mapping_table_internal_set*/  

/*******************************************************************************
*  digi_cmf_trunktoclient_conseq_action_table_internal_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will retrieve up the consequential action table for the
*               trunk to client direction. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               mapotn_channel - mapotn channel this table is attributed to.
*               trunk2client_tables_ptr - consequential action table pointer. 
* 
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_REQ_CMF_Get_TrunkToClient_Conseq_Action_Table
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_channel, 
                                                                         cmf_trunk2client_conseqAction_t  *trunk2client_tables_ptr)
{
    UINT32 response_msg_len;
    UINT32 data_var;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[25];        

    PMC_ENTRY();

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Add message body to the previously created header. */
    /* Words 0 of CMD_REQ_CMF_Set_TrunkToClient_Conseq_Action_Table have the following format:
     * Word     bits 24..31             bits 16..23        bits 8..15           bits 0..7
     *   0  --------------------------reserved-----------------------------|--mapotn_channel----
     */
    data[0] = ( ( (UINT8)mapotn_channel ) | 
                  0x000000 );

    /* Send message*/
    if ( digi_fw_is_ver_3_05(digi_handle) )
    {
        response_msg_len = 21;
    } else {
        response_msg_len = 25;
    }
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_REQ_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_ST,data,1,response,response_msg_len,stat);
    }

    /* Get result*/
    if (PMC_SUCCESS == result) {

        data_var = response[1];
        trunk2client_tables_ptr->ssf_sf_rcvd_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) ) ;
        trunk2client_tables_ptr->ssf_sf_rcvd_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->ssf_sf_rcvd_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) );  
        trunk2client_tables_ptr->ssf_sf_rcvd_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[2];
        trunk2client_tables_ptr->ssf_sf_rcvd_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) ); 

        data_var = response[3];
        trunk2client_tables_ptr->dplm_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->dplm_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->dplm_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->dplm_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[4];
        trunk2client_tables_ptr->dplm_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[5];
        trunk2client_tables_ptr->opu_csf_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->opu_csf_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->opu_csf_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->opu_csf_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[6];
        trunk2client_tables_ptr->opu_csf_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[7];
        trunk2client_tables_ptr->gfp_lofd_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->gfp_lofd_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->gfp_lofd_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->gfp_lofd_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[8];
        trunk2client_tables_ptr->gfp_lofd_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[9];
        trunk2client_tables_ptr->gfp_pti_upi_mismatch_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->gfp_pti_upi_mismatch_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->gfp_pti_upi_mismatch_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->gfp_pti_upi_mismatch_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[10];
        trunk2client_tables_ptr->gfp_pti_upi_mismatch_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[11];
        trunk2client_tables_ptr->gfp_cmf_csf_loss_client_signal_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->gfp_cmf_csf_loss_client_signal_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->gfp_cmf_csf_loss_client_signal_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->gfp_cmf_csf_loss_client_signal_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[12];
        trunk2client_tables_ptr->gfp_cmf_csf_loss_client_signal_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[13];
        trunk2client_tables_ptr->gfp_cmf_csf_loss_char_sync_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->gfp_cmf_csf_loss_char_sync_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->gfp_cmf_csf_loss_char_sync_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->gfp_cmf_csf_loss_char_sync_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[14];
        trunk2client_tables_ptr->gfp_cmf_csf_loss_char_sync_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[15];
        trunk2client_tables_ptr->gfp_cmf_fdi_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->gfp_cmf_fdi_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->gfp_cmf_fdi_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->gfp_cmf_fdi_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[16];
        trunk2client_tables_ptr->gfp_cmf_fdi_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[17];
        trunk2client_tables_ptr->gfp_cmf_rdi_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->gfp_cmf_rdi_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->gfp_cmf_rdi_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->gfp_cmf_rdi_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[18];
        trunk2client_tables_ptr->gfp_cmf_rdi_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        data_var = response[19];
        trunk2client_tables_ptr->sonet_sfis_lof_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
        trunk2client_tables_ptr->sonet_sfis_lof_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
        trunk2client_tables_ptr->sonet_sfis_lof_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
        trunk2client_tables_ptr->sonet_sfis_lof_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

        data_var = response[20];
        trunk2client_tables_ptr->sonet_sfis_lof_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );

        if ( !digi_fw_is_ver_3_05(digi_handle) )
        {
            data_var = response[21];
            trunk2client_tables_ptr->gfp_dexm_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
            trunk2client_tables_ptr->gfp_dexm_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
            trunk2client_tables_ptr->gfp_dexm_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
            trunk2client_tables_ptr->gfp_dexm_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

            data_var = response[22];
            trunk2client_tables_ptr->gfp_dexm_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );


            data_var = response[23];
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.monitor_defect = ( (BOOL)((data_var >> 24) & 0xFF) );   
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.action1 = ( (digi_trunk2client_actions_t)((data_var >> 16) & 0xFF) );
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.action2 = ( (digi_trunk2client_actions_t)((data_var >> 8) & 0xFF) ); 
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.priority_idx = ( (UINT8)((data_var >>  0) & 0xFF));

            data_var = response[24];
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.second_action_holdoff_time = ( (UINT16)((data_var >> 16) & 0xFFFF) );
        } else {

            trunk2client_tables_ptr->gfp_dexm_mon.monitor_defect = 0;   
            trunk2client_tables_ptr->gfp_dexm_mon.action1 = (digi_trunk2client_actions_t)0;
            trunk2client_tables_ptr->gfp_dexm_mon.action2 = (digi_trunk2client_actions_t)0; 
            trunk2client_tables_ptr->gfp_dexm_mon.priority_idx = 0;

            trunk2client_tables_ptr->gfp_dexm_mon.second_action_holdoff_time = 0;

            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.monitor_defect = 0; 
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.action1 = (digi_trunk2client_actions_t)0;
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.action2 = (digi_trunk2client_actions_t)0;
            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.priority_idx = 0;

            trunk2client_tables_ptr->emac_lf_and_phy_los_mon.second_action_holdoff_time = 0;
        }

    }


    PMC_RETURN(result);

} /* End of digi_cmf_trunktoclient_conseq_action_table_internal_get*/ 

/*******************************************************************************
*  digi_cmf_trunktoclient_port_mapping_table_internal_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               digi_cmf_trunktoclient_port_mapping_table_internal_get helps retrieve
*               the port mapping table for the trunk to client direction. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               mapotn_channel - mapotn channel this table is attributed to.
*               trunk2client_tables_ptr - port mapping table pointer. 
* 
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_REQ_CMF_Set_TrunkToClient_Port_Mapping_Table
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_port_mapping_table_internal_get(digi_handle_t *digi_handle,
                                                                         UINT32 mapotn_channel, 
                                                                         cmf_trunk2client_portMapping_t  *trunk2client_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[2];
    UINT32 data_var;

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    /* Words 0 of CMD_REQ_CMF_Set_TrunkToClient_Port_Mapping_Table have the following format:
     * Word     bits 24..31             bits 16..23        bits 8..15           bits 0..7
     *   0  --------------------------reserved-----------------------------|--mapotn_channel----
     */
    data[0] = ( ( (UINT8)mapotn_channel ) | 0x000000 );

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_REQ_TRUNKTOCLIENT_PORT_MAPPING_TABLE_ST,data,1,response,2,stat);
    }

    /* Get result*/

    if (PMC_SUCCESS == result) {

        data_var = response[1];
        trunk2client_tables_ptr->opu_csf_persistance_time = ( (UINT8)((data_var >> 8) & 0xFF) );  
    }

    PMC_RETURN(result);

} /* End of digi_cmf_trunktoclient_port_mapping_table_internal_get*/ 

/*******************************************************************************
*  digi_cmf_clienttotrunk_tables_update
*  ___________________________________________________________________________
*
*  DESCRIPTION:
* 
*               This function will update and activate the consequential action table and port
*               action table in FW. 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               mapper_chnl_id  - mapper channel id
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_CMF_Set_ClientToTrunk_Update_Tables. 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_tables_update_internal(digi_handle_t *digi_handle,
                                                               UINT32 mapper_chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ENTRY();

    /* Add message body to the previously created header. */
    /* Word 0 of CMF_CFG_CMF_Set_ClientToTrunk_Update_Tables have the following format:
     * Word     bits 24..31             bits 16..23        bits 8..15           bits 0..7
     *   0  ----enet_link---|------------------------reserved-----------------------
     */
    data[0] = (((UINT8)mapper_chnl_id) << 24) | 
        0x000000;

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_UPDATE_TABLES_ST,data,1,stat);
    }

    PMC_RETURN(result);
}

#endif /* DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
*  digi_cmf_trigger_lf_internal_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This is the internal function for triggering a LF. It is used to call
*   the hostmessage that is responsible for this action.
* 
*   This functionality is deprecated for MAC termination modes, it is only valid
*   for EPMM applications.
*  
*  INPUTS:
*   digi_handle - digi device handle. 
*   enet_link  - enet link on which to operate 
*   enet_mode - enet mode on which to insert LF.\n
*   trigger - Indicates whether an LF should be inserted, stopped, or release control.\n
*                 0 ~ stop the insertion of LF\n
*                 1 ~ start the insertion of LF\n
*   			  2 ~ release control of the LF bit\n
*   src_ss     - Source subsystem, valid values are ENET_LINE, ENET_SYS,
*                MAPOTN
*   src_ch     - Source channel, valid values 0 to 11 for ENET and 0 to 95
*                for MAPOTN
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Set_Trigger_LF.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cmf_trigger_lf_internal_cfg(digi_handle_t *digi_handle,
                                                   UINT32 enet_link, 
                                                   hostmsg_cmf_enet_mapotn_mode_t enet_mode,
                                                   digi_cmf_sw_control_action_t trigger,
										           util_global_cpb_port_t src_ss,
                                                   UINT8 src_ch)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[1];
    UINT8 source_map = 0x0;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Create source_map field from src_ss and src_ch inputs */
    /*
     *   source_map - This UINT8 has 2 definitions based on the MSB\n
     *   			 Bit 7 will inform of Demapped path or ENET EPMM RX path:\n
     *   			 1 ~ Path is demapped.\n
     *   				-> Bits 0-6 are the MAPOTN channel\n
     *   			 0 ~ Path is coming from ENET LINE or SYS.\n
     *   				-> Bit 6 is describing either ENET LINE EPMM RX or\n
     *   				   ENET SYS EPMM RX: \n
     *   				   0 ~ ENET SYS  \n
     *   				   1 ~ ENET LINE  \n
     *   				   -> Bits 0-5 are the enet link. \n
     *   			  Summary:
     *   			  Bits 7 - 6 - 5 - 4 - 3 - 2 - 1 - 0
     *   			  Demapped path:
     *   				   1 - *----- mapotn_chnl-------*
     *   			  EPMM LINE RX:
     *   			  	   0 - 1 *------enet_link-------*
     *   			  EPMM SYS RX:
     *   			       0 - 0 *------enet_link-------*
     */
    if(src_ss == UTIL_GLOBAL_CPB_PORT_MAPOTN)
    {
        source_map = 0x80 | src_ch;
    }
    else if(src_ss == UTIL_GLOBAL_CPB_PORT_ENET_LINE)
    {
        source_map = 0x40 | src_ch;
    }
    else if(src_ss == UTIL_GLOBAL_CPB_PORT_ENET_SYS)
    {
        source_map = src_ch;
    }
    /* Words 0 of CMD_CFG_CMF_Set_Trigger_LF have the following format:
     * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
     *   0  --enet_link---|--enet_mode---|---trigger---|---source_map--
     */
    data[0] = (((UINT8)enet_link << 24) | 
                 ((UINT8)enet_mode << 16) | 
                 ((UINT8)trigger << 8) |
				 ((UINT8)source_map) );

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_TRIGGER_LF_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trigger_lf_internal_cfg*/ 
   
   
/*******************************************************************************
*  digi_cmf_trigger_rf_internal_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This is the internal function for triggering a RF. It is used to call
*   the hostmessage that is responsible for this action.
*
*   This functionality is deprecated for MAC termination modes, it is only valid
*   for EPMM applications.
*  
*  
*  INPUTS:
*   digi_handle - digi device handle. 
*   enet_link  - enet link on which to operate 
*   enet_mode - enet mode on which to insert RF.\n
*   trigger - Indicates whether an RF should be inserted, stopped, or release control.\n
*                 0 ~ stop the insertion of RF\n
*                 1 ~ start the insertion of RF\n
*                 2 ~ release control of the RF bit\n
*   src_ss     - Source subsystem, valid values are ENET_LINE, ENET_SYS,
*                MAPOTN
*   src_ch     - Source channel, valid values 0 to 11 for ENET and 0 to 95
*                for MAPOTN
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Set_Trigger_RF. 
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cmf_trigger_rf_internal_cfg(digi_handle_t *digi_handle,
                                                   UINT32 enet_link, 
                                                   hostmsg_cmf_enet_mapotn_mode_t enet_mode,
                                                   digi_cmf_sw_control_action_t trigger,
										           util_global_cpb_port_t src_ss,
                                                   UINT8 src_ch)
{
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];
    UINT8 source_map = 0x0;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Create source_map field from src_ss and src_ch inputs */
    /*
     *   source_map - This UINT8 has 2 definitions based on the MSB\n
     *   			 Bit 7 will inform of Demapped path or ENET EPMM RX path:\n
     *   			 1 ~ Path is demapped.\n
     *   				-> Bits 0-6 are the MAPOTN channel\n
     *   			 0 ~ Path is coming from ENET LINE or SYS.\n
     *   				-> Bit 6 is describing either ENET LINE EPMM RX or\n
     *   				   ENET SYS EPMM RX: \n
     *   				   0 ~ ENET SYS  \n
     *   				   1 ~ ENET LINE  \n
     *   				   -> Bits 0-5 are the enet link. \n
     *   			  Summary:
     *   			  Bits 7 - 6 - 5 - 4 - 3 - 2 - 1 - 0
     *   			  Demapped path:
     *   				   1 - *----- mapotn_chnl-------*
     *   			  EPMM LINE RX:
     *   			  	   0 - 1 *------enet_link-------*
     *   			  EPMM SYS RX:
     *   			       0 - 0 *------enet_link-------*
     */
    if(src_ss == UTIL_GLOBAL_CPB_PORT_MAPOTN)
    {
        source_map = 0x80 | src_ch;
    }
    else if(src_ss == UTIL_GLOBAL_CPB_PORT_ENET_LINE)
    {
        source_map = 0x40 | src_ch;
    }
    else if(src_ss == UTIL_GLOBAL_CPB_PORT_ENET_SYS)
    {
        source_map = src_ch;
    }

    /* Words 0 of CMD_CFG_CMF_Set_Trigger_RF have the following format:
     * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
     *   0  --enet_link---|--enet_mode---|---trigger---|---source_map---
     */

    /* Increment and initialize storage. */
    data[0] = ( ((UINT8)enet_link << 24) | 
                 ((UINT8)enet_mode << 16) | 
                 ((UINT8)trigger << 8) |
				 ((UINT8)source_map) );

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_TRIGGER_RF_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trigger_rf_internal_cfg*/    

/*******************************************************************************
 *
 * Public methods.
 *
 ******************************************************************************/
      
/*******************************************************************************
*
*  digi_cmf_insert_one_shot_gfp_cmf_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This method creates CMF based on user input and sends a one shot CMF on the
*   given channel.
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   *chnl_ctxt_ptr        - pointer to mapper channel handle to be operated on
*   payload_length_type - flag to determine whether to include payload\n
*                  0~1\n
*   upi_value - UPI value to be send in the CMF\n
*                  0~255\n
*   genpayloadFCS - Indicates whether a 4 byte payload FCS should be 
*                   generated if client payload field is used (0~1).
*   insertion_result - returns if the CMF was successfully inserted\n
*                      0 ~ insertion did not succeed (HW may have been busy)\n
*                      1 ~ insertion was successful\n
*   digi_cmf_one_shot_cmf_ptr - Structure containing payload infomation.
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Send_One_Shot_CMF. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_insert_one_shot_gfp_cmf_cfg(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               UINT32 payload_length_type,
                                               UINT32 upi_value,
                                               UINT32 genpayloadFCS,
                                               BOOL8 *insertion_result,
                                               digi_cmf_one_shot_cmf_t *digi_cmf_one_shot_cmf_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 mapotn_chnl;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = digi_cmf_insert_one_shot_gfp_cmf_internal_cfg(digi_handle, mapotn_chnl, payload_length_type, upi_value, genpayloadFCS, insertion_result, digi_cmf_one_shot_cmf_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_insert_one_shot_gfp_cmf_cfg*/ 
      
/*******************************************************************************
* digi_cmf_extract_gfp_cmf_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Extracts the CMF from either the CPU FIFO or from the memory where the
*  last stored CMF was saved after being extracted after an interrupt 
*
* INPUTS:
*  digi_handle    - DIGI device handle.\n
*  chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on
*  extract_mode   - CMF extraction mode\n
*                   0 ~ extract directly from CPU FIFO\n
*                   1 ~ extract last retrieved CMF via interrupt driven method (saved in memory).
*
* OUTPUTS:
*  digi_cmf_extract_cmf_ptr   - Structure containing the extracted CMF information\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES:      CMD_REQ_CMF_Extract_CMF
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_extract_gfp_cmf_get(digi_handle_t *digi_handle,
                                         digi_mapper_chnl_t *chnl_ctxt_ptr,
                                         UINT32 extract_mode,
                                         digi_cmf_extract_cmf_t *digi_cmf_extract_cmf_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 mapotn_chnl;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = digi_cmf_extract_gfp_cmf_internal_get(digi_handle, mapotn_chnl, extract_mode, digi_cmf_extract_cmf_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_cmf_extract_gfp_cmf_internal_get() */      

/*******************************************************************************
*  digi_cmf_trigger_opu_csf_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function triggers to either send or stop sending an OPU CSF on the
*   given channel.
*   This function will supersede any OPU-CSF consequential actions that FW may be 
*   executing. This function will continue to have control of the OPU-CSF bit until 
*   releasing control is specified.
*   Once control is released, normal operation will continue for the
*   client to trunk OPU-CSF consequential actions that are set (if any).
*   During a force insert OPU-CSF or force stop OPU-CSF, all client to trunk
*   consequential actions except OPU-CSF actions will continue to run properly.
*   Releasing control will always stop the insertion of a OPU-CSF and return control
*   to the FW (which can then reset the OPU-CSF). This only needs to be done
*   if the client to trunk consequential actions are required.
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on
*   trigger - Indicates whether the OPU CSF should be inserted or stop inserting OPU CSF.\n
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Trigger_OPU_CSF. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trigger_opu_csf_cfg(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               digi_cmf_sw_control_action_t trigger)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 mapotn_chnl;
    digi_mapping_mode_t mode;
    UINT32 mapping_mode_hostmsg;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    mode = mapper_chnl_handle_ptr->mode;
    if( 
        DIGI_MAP_ODUKP_PACKET_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode || 
        DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET == mode)
    {
        mapping_mode_hostmsg = 0;
    } 
    else{
        mapping_mode_hostmsg = 1;
    }

    result = digi_cmf_trigger_opu_csf_internal_cfg(digi_handle, mapotn_chnl, mapping_mode_hostmsg, trigger);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trigger_opu_csf_cfg*/   

/*******************************************************************************
*  digi_cmf_clienttotrunk_tables_update
*  ___________________________________________________________________________
*
*  DESCRIPTION:
* 
*               This function will update and activate the consequential action table and port
*               action table in FW. This message will terminate any active consequential actions
*               upon receiving this message. Note that any current active table configurations
*               will be removed and only the last table parameters sent by
*               'digi_cmf_clienttotrunk_conseq_actions_cfg' will be used.
* 
*               Use 'digi_cmf_clienttotrunk_conseq_action_cfg' before calling this function
*               (this is a helper function to configure both the consequential action and port mapping table). 
* 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_CMF_Set_ClientToTrunk_Update_Tables. 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_tables_update(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        


    /* Used to retrieve the enet port and channel from the mapotn channel*/
    util_global_switch_data_def_t *local_output_sw_ptr;
    util_global_cpb_port_data_t *local_output_pc_ptr;
    util_global_cpb_port_data_t local_input_pc;
    BOOL8 is_multicast = FALSE;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;

    cpb_handle_t* cpb_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    local_output_sw_ptr = &(mapper_chnl_handle_ptr->switch_data);
    local_output_pc_ptr = &(local_output_sw_ptr->cpb_data);

    cpb_handle = digi_handle->dcpb_handle;


    result = cpb_chnl_source_type_and_primary_test(cpb_handle,
                                      local_output_pc_ptr->port_type,
                                      local_output_pc_ptr->channel,
                                      (UINT32*)&(local_input_pc.port_type),
                                      &(local_input_pc.channel),
                                      &is_broadcast,
                                      &is_multicast,
                                      &is_reader_primary,
                                      &is_slave_zone_primary);

    switch(local_input_pc.port_type){

        /* Right now we are only available to have the trunk as ENET LINE or CBRC. */
        case UTIL_GLOBAL_CPB_PORT_CBRC:
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
        case UTIL_GLOBAL_CPB_PORT_ENET_LINE: {
            
            result = digi_cmf_clienttotrunk_tables_update_internal(digi_handle,local_input_pc.channel);
            break;
        } 
		case UTIL_GLOBAL_CPB_PORT_ILKN1: {
			result = digi_cmf_clienttotrunk_tables_update_internal(digi_handle, local_output_pc_ptr->channel);
            break;
		}
        default: {
            /* Currently, we only have the enet line to be able to monitor the defects... */
            PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_clienttotrunk_tables_update*/    

/*******************************************************************************
*  digi_cmf_clienttotrunk_conseq_action_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
* 
*               This function will help set up the port action table and the
*               consequential action table for the client to trunk direction.
* 
*               This API is only to be called after a full datapath provisioning
*               and activation.
* 
*               The configuration passed into this API will be stored in FW offline and will
*               NOT be active until 'digi_cmf_clienttotrunk_tables_update' is called.
*               If you call this API for a second time before activating (updating) the table, the
*               previous offline table will be overwritten. This API will overwrite the offline
*               consequential action table everytime it is called. 
* 
*               When the datapath is deprovisioned and provisioned again, the mapping
*               found in this API may no longer be applicable.
* 
*               Before tearing down the datapath please disable all the actions and monitoring.
* 
*               After reactivating the datapath, the consequential actions may once
*               again be re-configured.
* 
*               Steps:\n
*               1. Bring up datapath fully\n
*               2. Use 'digi_cmf_clienttotrunk_conseq_action_cfg' to set up port action
*                  table and port action table.\n
*               3. Call 'digi_cmf_clienttotrunk_tables_update' to activate the consequential
*                  actions\n
* 
*               If you want to tear down and bring up datapath again:\n
*               1a. Use 'digi_cmf_clienttotrunk_conseq_action_cfg' to set up port action
*                  table and port action table that does not monitor defects or execute
*                  consequential actions.\n
*               2a. Call 'digi_cmf_clienttotrunk_tables_update' to disable the monitoring/actions.\n
*               3a. Tear down datapath, Bring up datapath\n
*               4a. Return to step (1.)\n 
* 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               which has the mapotn mapping.
*               client2trunk_tables_ptr - a pointer to the port action table
*                                         and the conseq action table. 
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*              This function helps set up:
*              CMD_CFG_CMF_Set_ClientToTrunk_Conseq_Action_Table and
*              CMD_CFG_CMF_Set_ClientToTrunk_Port_Action_Table
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_cfg(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               digi_client2trunk_tables_cmf_t  *client2trunk_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    /* Used to retrieve the enet port and channel from the mapotn channel*/
    util_global_switch_data_def_t *local_output_sw_ptr; 
    util_global_switch_data_t *input_sw_ptr = NULL;
    digi_enet_client_chnl_def_t *enet_chnl_ptr = NULL;
    util_global_cpb_port_data_t *local_output_pc_ptr;
    util_global_cpb_port_data_t local_input_pc;

    BOOL8 is_multicast = FALSE;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    /* Used to extract the mapping mode */
    digi_mapping_mode_t mode;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapping_mode_hostmsg, mapotn_chnl;
    UINT32  epmm_legacy_mode = 0;
    UINT8 data_upi, os_upi;
    /* Used to determine the enet mode configured */
    digi_enet_mode_t enet_mode = LAST_DIGI_ENET;

    cpb_handle_t* cpb_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    local_output_sw_ptr = &(mapper_chnl_handle_ptr->switch_data);
    local_output_pc_ptr = &(local_output_sw_ptr->cpb_data);

    cpb_handle = digi_handle->dcpb_handle;

    mode = mapper_chnl_handle_ptr->mode;
    if( 
        DIGI_MAP_ODUKP_PACKET_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode || 
        DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET == mode)
    {
        mapping_mode_hostmsg = 0;
    } 
    else{
        mapping_mode_hostmsg = 1;
    }

	mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = cpb_chnl_source_type_and_primary_test(cpb_handle,
                                      local_output_pc_ptr->port_type,
                                      local_output_pc_ptr->channel,
                                      (UINT32*)&(local_input_pc.port_type),
                                      &(local_input_pc.channel),
                                      &is_broadcast,
                                      &is_multicast,
                                      &is_reader_primary,
                                      &is_slave_zone_primary);

    /* Check if we are in Legacy mode */

    if (result != PMC_SUCCESS)
    {
        PMC_ATOMIC_RETURN(digi_handle,result);
    }
    
    switch(local_input_pc.port_type){
		case UTIL_GLOBAL_CPB_PORT_ILKN1: {
			if ( (local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) ||
				 (local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) ||
				 (local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) )
			{
				/* Break apart the mesage and send it to FW. */
				result = digi_cmf_clienttotrunk_conseq_action_table_internal_set(digi_handle,
																				 local_output_pc_ptr->channel,
																				 &client2trunk_tables_ptr->conseq_action_cmf
																				 );

				if (PMC_SUCCESS == result) {
					result = digi_cmf_clienttotrunk_port_action_table_internal_set(digi_handle,
																		  local_output_pc_ptr->channel,
																		  0, /* ENET client type */
																		  mapotn_chnl,
																		  mapping_mode_hostmsg,
																		  epmm_legacy_mode,
																		  client2trunk_tables_ptr->port_action_cmf.enable_monitoring,
																		  client2trunk_tables_ptr->port_action_cmf.is_send_dci,
																		  client2trunk_tables_ptr->port_action_cmf.is_gen_keep_alive_cmf,
																		  client2trunk_tables_ptr->port_action_cmf.upi_for_keep_alive_cmf,
																		  client2trunk_tables_ptr->port_action_cmf.is_gen_payload_fcs,
																		  client2trunk_tables_ptr->port_action_cmf.is_use_payload_csf,
																		  client2trunk_tables_ptr->port_action_cmf.cmf_payload
																		  );


				}
			} else {
				PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
			}
			break;

		}
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
        case UTIL_GLOBAL_CPB_PORT_ENET_LINE: {
            /* local_input_pc.channel is the enet channel we need. */
            
            /* Get the enet mode that is configured */
            if (result == PMC_SUCCESS)
            {
                digi_enet_prov_get(digi_handle, digi_handle->enet_line_handle, &enet_mode);
            }
            /* update FW enet client source */
            if (result == PMC_SUCCESS)
            {
                if (local_input_pc.port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS)
                {
                    result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_sys_chnl[local_input_pc.channel]);
                }
                else
                {
                    result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_line_chnl[local_input_pc.channel]);
                }

                epmm_legacy_mode = FALSE;
                digi_opsa_src_chnl_hndl_for_dest_get(digi_handle, chnl_ctxt_ptr, FALSE, &input_sw_ptr);
                enet_chnl_ptr = (digi_enet_client_chnl_def_t *)input_sw_ptr;
                if (enet_chnl_ptr->mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT 
                    || enet_chnl_ptr->mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON) 
                {
                    epmm_legacy_mode = digi_enet_gsup43_7_3_legacy_mode_chk(digi_handle, input_sw_ptr, &data_upi, &os_upi);
                }
            }

            if ( (enet_mode == DIGI_ENET_SERDES_100GE_TRANSPARENT) || 
                 (enet_mode == DIGI_ENET_SERDES_40GE_TRANSPARENT) ||
                 (enet_mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT) ||
                 (enet_mode == DIGI_ENET_SERDES_10GE_TRANSPARENT)  )
            {
                PMC_ATOMIC_RETURN(digi_handle, DIGI_USE_ASSOCIATED_ENET_MODE_RX_PMON );
            }

            /* Break apart the mesage and send it to FW. */
            result = digi_cmf_clienttotrunk_conseq_action_table_internal_set(digi_handle,
                                                                             local_input_pc.channel,
                                                                             &client2trunk_tables_ptr->conseq_action_cmf
                                                                             );

            if (PMC_SUCCESS == result) {
                result = digi_cmf_clienttotrunk_port_action_table_internal_set(digi_handle,
                                                                      local_input_pc.channel,
                                                                      0, /* ENET client type */
                                                                      local_output_pc_ptr->channel,
                                                                      mapping_mode_hostmsg,
                                                                      epmm_legacy_mode,
                                                                      client2trunk_tables_ptr->port_action_cmf.enable_monitoring,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_send_dci,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_gen_keep_alive_cmf,
                                                                      client2trunk_tables_ptr->port_action_cmf.upi_for_keep_alive_cmf,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_gen_payload_fcs,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_use_payload_csf,
                                                                      client2trunk_tables_ptr->port_action_cmf.cmf_payload
                                                                      );


            }
            

            break;
        }
        case UTIL_GLOBAL_CPB_PORT_CBRC: {
            /* local_input_pc.channel is the cbrc channel we need. */
            /* CBRC input rather than an ENET input. */

            /* Break apart the mesage and send it to FW. */
            result = digi_cmf_clienttotrunk_conseq_action_table_internal_set(digi_handle,
                                                                             local_input_pc.channel,
                                                                             &client2trunk_tables_ptr->conseq_action_cmf
                                                                             );

            if (PMC_SUCCESS == result) {
                result = digi_cmf_clienttotrunk_port_action_table_internal_set(digi_handle,
                                                                      local_input_pc.channel,
                                                                      1, /* CBRC client type */
                                                                      local_output_pc_ptr->channel,
                                                                      mapping_mode_hostmsg,
                                                                      epmm_legacy_mode,
                                                                      client2trunk_tables_ptr->port_action_cmf.enable_monitoring,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_send_dci,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_gen_keep_alive_cmf,
                                                                      client2trunk_tables_ptr->port_action_cmf.upi_for_keep_alive_cmf,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_gen_payload_fcs,
                                                                      client2trunk_tables_ptr->port_action_cmf.is_use_payload_csf,
                                                                      client2trunk_tables_ptr->port_action_cmf.cmf_payload
                                                                      );


            }
            

            break;
        }
        default: {
            /* Currently, we only have the enet line to be able to monitor the defects... */
            PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_clienttotrunk_conseq_action_cfg*/ 

/*******************************************************************************
*  digi_cmf_clienttotrunk_conseq_action_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will help retrieve the consequential action table
*               for the client to trunk direction.
* 
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               which has the mapotn mapping.
*               client2trunk_tables_ptr - a pointer to the conseq action table. 
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*              This function helps set up:
*              CMD_REQ_CMF_Get_ClientToTrunk_Conseq_Action_Table 
* 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_get(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                          digi_client2trunk_conseq_action_t  *client2trunk_tables_ptr)
{

    PMC_ERROR result = PMC_SUCCESS; 
    /* Used to retrieve the enet port and channel from the mapotn channel*/
    util_global_switch_data_def_t *local_output_sw_ptr; 
    util_global_cpb_port_data_t *local_output_pc_ptr;
    util_global_cpb_port_data_t local_input_pc;
    BOOL8 is_multicast = FALSE;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    /* Used to extract the mapping mode */
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;

    cpb_handle_t* cpb_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    local_output_sw_ptr = &(mapper_chnl_handle_ptr->switch_data);
    local_output_pc_ptr = &(local_output_sw_ptr->cpb_data);

    cpb_handle = digi_handle->dcpb_handle;

    result = cpb_chnl_source_type_and_primary_test(cpb_handle,
                                      local_output_pc_ptr->port_type,
                                      local_output_pc_ptr->channel,
                                      (UINT32*)&(local_input_pc.port_type),
                                      &(local_input_pc.channel),
                                      &is_broadcast,
                                      &is_multicast,
                                      &is_reader_primary,
                                      &is_slave_zone_primary);

    result = digi_cmf_clienttotrunk_conseq_action_table_internal_get(digi_handle,
                                                                     local_input_pc.channel,
                                                                     client2trunk_tables_ptr
                                                                     );

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_clienttotrunk_conseq_action_get*/ 

/*******************************************************************************
*  digi_cmf_trunktoclient_tables_update
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will update the consequential action and port mapping table
*               in FW. This message will terminate any active consequential actions
*               upon receiving this message. Note that any current active table configurations
*               will be removed and only the last table parameters sent by
*               'digi_cmf_clienttotrunk_conseq_actions_cfg' will be used.
* 
*               Use 'digi_cmf_trunktoclient_conseq_action_cfg' before calling this function
*               (this is a helper function to configure both the consequential action and port mapping table). 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_CMF_Set_TrunkToClient_Update_Tables. 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_tables_update(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    /* Used to extract the mapotn channel */
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 data[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    /* Check if we are in GMP mapping or GFP mapping and get the mapotn channel */
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    /* Add message body to the previously created header. */
    /* Word 0 of CMD_CFG_CMF_Set_TrunkToClient_Update_Tables have the following format:
     * Word     bits 24..31        bits 16..23    bits 8..15     bits 0..7
     *   0  ----mapotn_chnl---|------------------------reserved-----------------------
     */

    data[0] = (((UINT8)mapotn_chnl) << 24) | 
                  0x000000;

    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_UPDATE_TABLES_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trunktoclient_tables_update*/    

/*******************************************************************************
*  digi_cmf_trunktoclient_conseq_action_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
* 
*               This function will set up the consequential action table and the
*               port mapping table for the trunk to client direction.
* 
*               This API is only to be called after a full datapath provisioning
*               and activation.
* 
*               The configuration passed into this API will be stored in FW offline but will
*               NOT be active until 'digi_cmf_trunktoclient_tables_update' is called.
*               If you call this API for a second time before activating (updating) the table, the
*               previous offline table will be overwritten. This API will overwrite the offline
*               consequential action table everytime it is called.
* 
*               When the datapath is deprovisioned and provisioned again, the mapping
*               found in this API may no longer be applicable.
* 
*               Before tearing down the datapath please disable all the actions and monitoring.
* 
*               After reactivating the datapath, the consequential actions may once
*               again be re-configured.
* 
*               Steps:\n
*               1. Bring up datapath fully\n
*               2. Use 'digi_cmf_trunktoclient_conseq_action_cfg' to set up port action
*                  table and port action table.\n
*               3. Call 'digi_cmf_trunktoclient_tables_update' to activate the consequential
*                  actions\n
* 
*               If you want to tear down and bring up datapath again:\n
*               1a. Use 'digi_cmf_trunktoclient_conseq_action_cfg' to set up port action
*                  table and port mapping table that does not monitor defects or execute
*                  consequential actions.\n
*               2a. Call 'digi_cmf_trunktoclient_tables_update' to disable the monitoring/actions.\n
*               3a. Tear down datapath, Bring up datapath\n
*               4a. Return to step (1.) 
* 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               trunk2client_tables_ptr - a pointer to the port mapping table
*                                         and the conseq action table. 
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               This is a helper function to properly set up
*               CMD_CFG_CMF_Set_TrunkToClient_Conseq_Action_Table and
*               CMD_CFG_CMF_Set_TrunkToClient_Port_Mapping_Table.
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_cfg(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                          digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr)
{
    PMC_ERROR result;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    result = digi_cmf_trunktoclient_conseq_action_cfg_internal(digi_handle,
                                                               chnl_ctxt_ptr,
                                                               trunk2client_tables_ptr,
                                                               FALSE);

    PMC_ATOMIC_RETURN(digi_handle, result);

} /* End of digi_cmf_trunktoclient_conseq_action_cfg*/ 

/*******************************************************************************
*  digi_cmf_trunktoclient_conseq_action_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This function will retrieve up the consequential action table
*               for the trunk to client direction.
* 
*               This API is only to be called after a full datapath provisioning
*               and activation.
* 
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               trunk2client_tables_ptr - a pointer to the conseq mapping table.
*                                         
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               This is a helper function to properly set up
*               CMD_REQ_CMF_Get_TrunkToClient_Conseq_Action_Table
*               
* 
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_get(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               cmf_trunk2client_conseqAction_t  *trunk2client_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS; 
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = digi_cmf_trunktoclient_conseq_action_table_internal_get(digi_handle,
                                                                     mapotn_chnl,
                                                                     trunk2client_tables_ptr );

    PMC_ATOMIC_RETURN(digi_handle, result);

} /* End of digi_cmf_trunktoclient_conseq_action_get*/ 

/*******************************************************************************
*  digi_cmf_trigger_lf_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   NOTE: This function is no longer valid for MAC modes starting in the 3.03 
*   release.\n\n
*
*   This function force inserts or force stops a LF on the given enet link. 
*   This function will supersede any LF consequential actions that FW may be 
*   executing. This function will continue to have control of the LF bit until 
*   releasing control is specified.\n\n
*
*   Once control is released, normal operation will continue for the
*   trunk to client LF consequential actions that are set (if any).
*   During a force insert LF or force stop LF, all trunk to client consequential
*   actions except LF actions will continue to run properly.
*   Releasing control will always stop the insertion of a LF and return control
*   to the FW (which can then reset the LF). This only needs to be done if the
*   trunk to client consequential actions are required.\n\n
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   chnl_handle  - pointer to enet link to be operated on
*   trigger - Indicates whether an LF should be inserted, stopped, or release control.\n
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Set_Trigger_LF. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trigger_lf_cfg(digi_handle_t *digi_handle,
                                         digi_enet_client_chnl_t    *chnl_handle, 
                                         digi_cmf_sw_control_action_t trigger)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 enet_link;
    hostmsg_cmf_enet_mapotn_mode_t hostmsg_enet_mode;
    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    digi_enet_mode_t mode;
    enet_handle_t *enet_handle = NULL;
    util_global_cpb_port_t src_ss = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT8 src_ch = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_enet_validate_parms(digi_handle, chnl_ptr, &enet_handle, &enet_link);

    if (PMC_SUCCESS == result)
    {
        /* check enet_line channel handle to make sure it belongs to digi handle pool */
        DIGI_ENET_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

        mode = chnl_ptr->mode;
    }

    /* update FW ENET client source if necessary */
    if (PMC_SUCCESS == result)
    {
        result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)chnl_handle);
    }

    if (PMC_SUCCESS == result)
    {
		/* retrieve the source subsystem and source channel */
        util_global_cpb_connect_t *cpb_connect_data_ptr = NULL;
        util_global_cpb_port_t dest_ss = (enet_handle == digi_handle->enet_sys_handle) ? UTIL_GLOBAL_CPB_PORT_ENET_SYS:UTIL_GLOBAL_CPB_PORT_ENET_LINE;
    	cpb_connect_data_state_get(digi_handle->dcpb_handle,
    							   &cpb_connect_data_ptr);

    	src_ss = (util_global_cpb_port_t)((util_global_cpb_port_t)cpb_connect_data_ptr->chnl[dest_ss][enet_link].mst.source_slv_port);
    	src_ch = cpb_connect_data_ptr->chnl[dest_ss][enet_link].mst.source_slv_chnl;
    }

    if (PMC_SUCCESS == result)
    {
        switch (mode)
        {
            case DIGI_ENET_SERDES_100GE_MAC:
            case DIGI_ENET_SERDES_40GE_MAC:
            case DIGI_ENET_SERDES_10GE_MAC: 
            {
                result = DIGI_ERR_ENET_EPMM_MODE_ONLY;
                break;
            }
            case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
            case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON: 
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC;
                break;
            }
            case DIGI_ENET_SERDES_10GE_TRANSPARENT:
            case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_BMP_GSUP43_10GE_MAC;
                break;
            }
            case DIGI_ENET_SERDES_40GE_TRANSPARENT:
            case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON: 
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_GMP_G709_40GE_MAC;
                break;
            }
            case DIGI_ENET_SERDES_100GE_TRANSPARENT:
            case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON: 
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_GMP_G709_100GE_MAC;
                break;
            }
            default: 
            {
                /* FW will return an invalid parameters error */
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_MAX_NUM;
            }        
        }
    }
    if (PMC_SUCCESS == result)
    {
        result = digi_cmf_trigger_lf_internal_cfg(digi_handle, enet_link, hostmsg_enet_mode, trigger, src_ss, src_ch);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trigger_lf_cfg*/   


/*******************************************************************************
*  digi_cmf_trigger_cbr_fault_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Function forces the CBRC client to insert nominal rate fault replacement signal on egress direction. 
*   This function will supersede any CBRC consequential actions that FW may be 
*   executing. This function will continue to have control on CBRC fault pattern insertion until 
*   releasing control is specified.
*
*   Once contorl is released, normal operation will continue for the trunk to client
*   CBRC consequential actions that are set (if any). 
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   chnl_handle - pointer to CBRC link to be operated on
*   enable      - Whether to enable(TRUE) or disable(FALSE) this feature.
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trigger_cbr_fault_cfg(digi_handle_t *digi_handle,
                                                digi_cbr_client_chnl_t    *chnl_handle, 
                                                digi_cmf_sw_control_action_t enable)
{
    PMC_ERROR result = PMC_SUCCESS;        
    digi_cbr_client_chnl_def_t *chnl_ptr = (digi_cbr_client_chnl_def_t *)chnl_handle;
    cbrc_handle_t *cbrc_handle;
    UINT32 cbrc_link;
    UINT32 map_ch;
    digi_cmf_sw_control_action_t action = enable;
    BOOL8 is_gfp;
    util_global_cpb_port_t src_ss = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT32 data[1];
    util_global_cpb_connect_t *cpb_connect_data_ptr = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_cbr_validate_params(digi_handle, chnl_ptr, &cbrc_handle, &cbrc_link);

    if (PMC_SUCCESS != result || UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED !=
        chnl_ptr->switch_data.header.prov_state)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_NOT_PROVISONED);
    }

    /* retrieve the source subsystem and source channel */
    cpb_connect_data_state_get(digi_handle->dcpb_handle,
                               &cpb_connect_data_ptr);
    
    src_ss = (util_global_cpb_port_t)cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][cbrc_link].mst.source_slv_port;
    
    if (UTIL_GLOBAL_CPB_PORT_MAPOTN == src_ss)
    {
        map_ch = cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][cbrc_link].mst.source_slv_chnl;
    }
    else if (UTIL_GLOBAL_CPB_PORT_ENET_LINE == src_ss)
    {
        /*
         * Check if ENET line is used in place of MAPOTN.
         */
        util_global_switch_data_def_t *switch_ptr =  
          (util_global_switch_data_def_t *)digi_cpb_retrieve_input_port(digi_handle, (UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE, cpb_connect_data_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][cbrc_link].mst.source_slv_chnl);
        
        if (NULL != switch_ptr &&
            UTIL_GLOBAL_CPB_PORT_MAPOTN == switch_ptr->cpb_data.port_type)
        {
            map_ch = switch_ptr->cpb_data.channel;
            src_ss = UTIL_GLOBAL_CPB_PORT_MAPOTN;
        }
    }

    if (UTIL_GLOBAL_CPB_PORT_MAPOTN != src_ss)
    {
        PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INVALID_ARG);
    }

    switch (digi_handle->handle_pool.mapper_chnl[map_ch].mode)
    {
      case DIGI_MAP_ODUKP_PACKET_GFP:
      case DIGI_MAP_ODU4P_100_GE_GFP:
      case DIGI_MAP_ODU4P_100_GE_GFP_ENET:
      case DIGI_MAP_ODU3P_40_GE_GFP:
      case DIGI_MAP_ODU3P_40_GE_GFP_ENET:
      case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP:
      case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET:
      case DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP:
      case DIGI_MAP_ODU2P_10_GE_6_2_GFP:
      case DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET:
        is_gfp = TRUE;
        break;
      default:
        is_gfp = FALSE;
        break;
    }

    /* Words 0 of CMD_CFG_CMF_Set_Trigger_LF have the following format:
     * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
     *   0  --enet_link---|--enet_mode---|---trigger---|---source_map--
     */
    data[0] = (((UINT8)cbrc_link << 24) | 
                 ((UINT8)map_ch << 16) | 
                 ((UINT8)is_gfp << 8) |
				 ((UINT8)action));

    HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
    result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,
                                      MODID_CMF,
                                      HOSTMSG_CMD_CMF_CFG_CBRC_FORCE_FAULT,
                                      data, 1, stat);

    PMC_ATOMIC_RETURN(digi_handle, result);

} /* End of digi_cmf_trigger_cbr_fault_cfg*/   


/*******************************************************************************
*  digi_cmf_trigger_rf_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   NOTE: This function is no longer valid for MAC modes starting in the 3.03 
*   release.\n\n
*
*   This function force inserts of force stops a RF on the given enet link. 
*   This function will supersede any RF consequential actions that FW may be 
*   executing. This function will continue to have control of the RF bit until 
*   releasing control is specified.\n\n
*
*   Once control is released, normal operation will continue for the
*   trunk to client RF consequential actions that are set (if any).
*   During a force insert RF or force stop RF, all trunk to client consequential
*   actions except RF actions will continue to run properly.
*   Releasing control will always stop the insertion of a RF and return control
*   to the FW (which can then reset the LF). This only needs to be done if the
*   trunk to client consequential actions are required.\n\n
*
*  INPUTS:
*   digi_handle - digi device handle. 
*   chnl_handle  - pointer to enet link to be operated on
*   trigger - Indicates whether an RF should be inserted, stopped, or release control.\n
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_Set_Trigger_RF. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trigger_rf_cfg(digi_handle_t *digi_handle,
                                         digi_enet_client_chnl_t    *chnl_handle, 
                                         digi_cmf_sw_control_action_t trigger)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 enet_link;
    hostmsg_cmf_enet_mapotn_mode_t hostmsg_enet_mode;
    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    digi_enet_mode_t mode;
    enet_handle_t *enet_handle = NULL;
    util_global_cpb_port_t src_ss = LAST_UTIL_GLOBAL_CPB_PORT;
    UINT8 src_ch = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

     /* check parameters */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = digi_enet_validate_parms(digi_handle, chnl_ptr, &enet_handle, &enet_link);

    if (PMC_SUCCESS == result)
    {
        /* check enet_line channel handle to make sure it belongs to digi handle pool */
        DIGI_ENET_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

        mode = chnl_ptr->mode;
    }

    /* update FW ENET client source if necessary */
    if (PMC_SUCCESS == result)
    {
        result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)chnl_handle);
    }

    if (PMC_SUCCESS == result)
    {
		/* retrieve the source subsystem and source channel */
        util_global_cpb_connect_t *cpb_connect_data_ptr = NULL;
        util_global_cpb_port_t dest_ss = (enet_handle == digi_handle->enet_sys_handle) ? UTIL_GLOBAL_CPB_PORT_ENET_SYS:UTIL_GLOBAL_CPB_PORT_ENET_LINE;
    	cpb_connect_data_state_get(digi_handle->dcpb_handle,
    							   &cpb_connect_data_ptr);

    	src_ss = (util_global_cpb_port_t)((util_global_cpb_port_t)cpb_connect_data_ptr->chnl[dest_ss][enet_link].mst.source_slv_port);
    	src_ch = cpb_connect_data_ptr->chnl[dest_ss][enet_link].mst.source_slv_chnl;
    }


    if (PMC_SUCCESS == result)
    {
        switch (mode)
        {
            case DIGI_ENET_SERDES_100GE_MAC:
            case DIGI_ENET_SERDES_40GE_MAC:
            case DIGI_ENET_SERDES_10GE_MAC: 
            {
                result = DIGI_ERR_ENET_EPMM_MODE_ONLY;
                break;
            }
            case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
            case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON: 
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC;
                break;
            }
            case DIGI_ENET_SERDES_10GE_TRANSPARENT:
            case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_BMP_GSUP43_10GE_MAC;
                break;
            }
            case DIGI_ENET_SERDES_40GE_TRANSPARENT:
            case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON: 
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_GMP_G709_40GE_MAC;
                break;
            }
            case DIGI_ENET_SERDES_100GE_TRANSPARENT:
            case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON: 
            {
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_GMP_G709_100GE_MAC;
                break;
            }
            default: 
            {
                /* FW will return an invalid parameters error */
                hostmsg_enet_mode = HOSTMSG_CMF_ENET_MAPOTN_MAX_NUM;
            }        
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_cmf_trigger_rf_internal_cfg(digi_handle, enet_link, hostmsg_enet_mode, trigger, src_ss, src_ch);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_trigger_rf_cfg*/   


/*******************************************************************************
*  digi_cmf_fdi_insertion_mapotn_channels_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION: This function configures CMF insertion in trunk direction for
*               mapotn channels.
*               Note that it can insert CSF_LOS, CSF_LOCS, DCI, FDI, and RDI, 
*               dispite the somehow misleading function name.
*
*  INPUTS:
*   digi_handle   - digi device handle. 
*   chnl_ctxt_ptr - pointer to Mapotn channel to act on 
*   enable        - TRUE to enable the consequential action,
*                   FALSE to disable it
*   upi_mode      - Constant value:
*                   DIGI_CMF_GFP_FRAME_CSF_LOS
*                   DIGI_CMF_GFP_FRAME_CSF_LOCS
*                   DIGI_CMF_GFP_FRAME_DCI
*                   DIGI_CMF_GFP_FRAME_FDI
*                   DIGI_CMF_GFP_FRAME_RDI
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*    
*  NOTES:
*    
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_fdi_insertion_mapotn_channels_cfg(digi_handle_t *digi_handle,
                                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                            BOOL enable,
                                                            UINT32 upi_mode)
{
    PMC_ERROR result = PMC_SUCCESS;    

    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 input_port;
    UINT32 input_chnl;
    BOOL8 dummy;
    UINT32 data[3];
    util_global_switch_data_t *src_hndl_ptr;
    UINT8 qs_ch_en = 0;
    UINT8 qs_ch_en_to_update = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = cpb_chnl_source_and_type_test(digi_handle->dcpb_handle,
                                           (UINT32)mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                           mapper_chnl_handle_ptr->switch_data.cpb_data.channel,
                                           &input_port,
                                           &input_chnl,
                                           &dummy,
                                           &dummy);
    
    if (PMC_SUCCESS == result)
    {
        digi_opsa_chnl_hndl_for_port_chnl_get(digi_handle,
                                              input_port,
                                              input_chnl,
                                              FALSE,
                                              &src_hndl_ptr);
        
        if (NULL != src_hndl_ptr)
        {
            /* 
             * Update the QSn_CH_EN for the input channel. If FDI/RDI 
             * insertion is enabled then disable the QSn_CH_EN otherwise
             * turn it back on.
             */
            UINT32 value = 0;

            cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                      ((util_global_switch_data_def_t *)src_hndl_ptr)->cpb_data.port_type,
                                      ((util_global_switch_data_def_t *)src_hndl_ptr)->cpb_data.channel,
                                      &qs_ch_en);
            
            digi_cpb_chnl_qs_ch_en_get(digi_handle,
                                       src_hndl_ptr,
                                       (util_global_switch_data_t *)chnl_ctxt_ptr,
                                       &qs_ch_en_to_update);
            /*
             * Disable the 
             */
            if (TRUE == enable)
            {
                value = qs_ch_en^qs_ch_en_to_update;
            }
            else
            {
                value = qs_ch_en|qs_ch_en_to_update;
            }

            if (DIGI_CMF_GFP_FRAME_DCI != upi_mode) {
                cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                      ((util_global_switch_data_def_t *)src_hndl_ptr)->cpb_data.port_type,
                                      ((util_global_switch_data_def_t *)src_hndl_ptr)->cpb_data.channel,
                                      value);
            }
        }
    }

    if (PMC_SUCCESS == result)
    {
        data[0] = mapotn_chnl;
        data[1] = enable;
        data[2] = upi_mode;
        
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_FDI_INS_MAPOTN_CHNLS,data,3,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_fdi_insertion_mapotn_channels_cfg*/    




#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
*  digi_cmf_enet_emac_lf_rf_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This api will call the FW to send lf, rf and still be compatible if any FW consequential\n
*   actions are set up. The only difference with having consequential actions\n
*   turned on is that FW will NOT turn off the send lf, rf if you have forced them.\n
*   If FW consequential actions are turned on, they will continue to function normally only\n
*   in the following modes:\n
* 
*   FLT_HDL_DIS   SEND_RF      SEND_LF\n
*      0             0           0      <= Enable ingress fault egress consequential action\n
*      1             0           0      <= Disable ingress fault egress consequential action\n
*      1             1           1      <= Unidirectional ENET Clause 66 compliant\n
* 
*    When FW is not forcing the send lf or send rf it will fallback into one of the above 3 states.\n
*    It will fall back into the last called of these three states.\n
* 
*   Beyond that, FW rememeber the last input for etrans_fault_on.\n
*   TRUE:  Perform ETRANS path config when forcing LF/RF \n
*   FALSE: Do not perform ETRANS path unique config.\n
*  
*  INPUTS:
*   digi_handle - Reference to digi_handle required for PMC_ATOMIC_ENTRY and
*                 RETURN
*   hndl - emac handle. 
*   link  - enet link to be operated on \n
*   flt_hdl_dis - auto fault enabling bit \n 
*   send_rf - send rf bit \n 
*   send_lf - send lf bit \n
*   etrans_fault_on - etrans traffic bit \n
*   etrans_legacy_mode - etrans legacy mode\n
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*  CMD_CFG_CMF_SET_Enet_Emac_LF_RF_Cfg. 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_enet_emac_lf_rf_cfg(digi_handle_t *digi_handle,
                                              emac_handle_t *hndl,
                                              UINT32 link, 
                                              BOOL8 flt_hdl_dis, 
                                              BOOL8 send_rf, 
                                              BOOL8 send_lf,
                                              BOOL8 etrans_fault_on, 
                                              BOOL8 etrans_legacy_mode)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ENTRY();

     /* check parameters */
    PMC_ASSERT(hndl != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Add message body to the previously created header. */
    /* Words 0,1 of CMD_CFG_CMF_Set_Trigger_LF have the following format:
     * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
     *   0  --enet_link---|--flt_hdl_dis---|---send_lf---|---send_rf---
     *   1  --etrans_fault_on---|--etrans_legacy_mode---|---reserved---|---reserved---
     */
    data[0] = ( ((UINT8)link << 24) | 
                    ((UINT8)flt_hdl_dis << 16) | 
                    ((UINT8)send_lf << 8) |
                     (UINT8)send_rf );
    data[1] = ( ((UINT8)etrans_fault_on << 24) |
                        ((UINT8)etrans_legacy_mode << 16) ) ; 

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_GFP_ENET_EMAC_LF_RF_ST,data,2,stat);
    }


    PMC_RETURN(result);

} /* End of digi_cmf_enet_emac_lf_rf_cfg() */   

/*******************************************************************************
*
*  digi_cmf_trunktoclient_conseq_action_cfg_internal
*  ___________________________________________________________________________
*
*   Description:
*               This function will set up the consequential action table and the
*               port mapping table for the trunk to client direction.
* 
*               This API is only to be called after a full datapath provisioning
*               and activation.
* 
*               The configuration passed into this API will be stored in FW offline but will
*               NOT be active until 'digi_cmf_trunktoclient_tables_update' is called.
*               If you call this API for a second time before activating (updating) the table, the
*               previous offline table will be overwritten. This API will overwrite the offline
*               consequential action table everytime it is called.
* 
*               When the datapath is deprovisioned and provisioned again, the mapping
*               found in this API may no longer be applicable.
* 
*               Before tearing down the datapath please disable all the actions and monitoring.
* 
*               After reactivating the datapath, the consequential actions may once
*               again be re-configured.
* 
*               Steps:
*               1. Bring up datapath fully
*               2. Use 'digi_cmf_trunktoclient_conseq_action_cfg' to set up port action
*                  table and port action table.
*               3. Call 'digi_cmf_trunktoclient_tables_update' to activate the consequential
*                  actions
* 
*               If you want to tear down and bring up datapath again:
*               1a. Use 'digi_cmf_trunktoclient_conseq_action_cfg' to set up port action
*                  table and port mapping table that does not monitor defects or execute
*                  consequential actions.
*               2a. Call 'digi_cmf_trunktoclient_tables_update' to disable the monitoring/actions.
*               3a. Tear down datapath, Bring up datapath
*               4a. Return to step (1.) 
* 
*
*  INPUTS:
*               digi_handle - digi device handle. 
*               chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on.
*               trunk2client_tables_ptr - a pointer to the port mapping table
*                                         and the conseq action table. 
*               mapper_chnl_activate    - when TRUE, opu_csf monitor is activated or deactivated based on opu_csf.monitor_defect value.
*                                         when FALSE, opu_csf monitor is not disabled when opu_csf.monitor_defect value is FALSE (opu csf monitor is reconfigured with its default value. The monitor is deactived only when mapper channel is demapped.).
*                                         
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS:
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*              This is a helper function to properly set up
*              CMD_CFG_CMF_Set_TrunkToClient_Conseq_Action_Table and
*              CMD_CFG_CMF_Set_TrunkToClient_Port_Mapping_Table.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_cfg_internal(digi_handle_t                   *digi_handle,
                                                                   digi_mapper_chnl_t              *chnl_ctxt_ptr,
                                                                   digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr,
                                                                   BOOL8                            mapper_chnl_activate)
{
    PMC_ERROR result = PMC_SUCCESS; 
    /* Used to retrieve the enet port and channel from the mapotn channel*/
    util_global_switch_data_def_t *local_input_sw_ptr;
    util_global_cpb_port_data_t *local_input_pc_ptr;

    /* Used to extract the mapping mode */
    digi_mapping_mode_t mode;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapping_mode_hostmsg;
    UINT32 mapotn_chnl;

    cpb_handle_t* cpb_handle = NULL;
    UINT32 opu_csf_persistance_time_bkp = 0;
    cmf_trunk2client_defect_action_t opu_csf_consq_act_bkp;
    BOOL8 opu_csf_ovr = FALSE;
	util_global_cpb_connect_t *cpb_connect_data_ptr = NULL;
	util_global_cpb_port_t output_port_type;
	UINT32 output_channel;


    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr); 

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    local_input_sw_ptr = &(mapper_chnl_handle_ptr->switch_data);
    local_input_pc_ptr = &(local_input_sw_ptr->cpb_data);

    cpb_handle = digi_handle->dcpb_handle;

    mode = mapper_chnl_handle_ptr->mode;
    if( 
        DIGI_MAP_ODUKP_PACKET_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode || 
        DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET == mode ||
        DIGI_MAP_ODU2EP_FC_1200_GFP == mode)
    {
        mapping_mode_hostmsg = 0;
    } 
    else{
        mapping_mode_hostmsg = 1;
    }


    PMC_MEMSET((void*)&opu_csf_consq_act_bkp, 0, sizeof(cmf_trunk2client_defect_action_t));
    if (FALSE == mapper_chnl_activate)
    {
        if (FALSE == trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.monitor_defect)
        {
            opu_csf_persistance_time_bkp = trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time;
            PMC_MEMCPY((void*)&opu_csf_consq_act_bkp, (void*)&trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon, sizeof(cmf_trunk2client_defect_action_t));
            result = digi_map_opu_csf_int_default_cfg_get(digi_handle,
                                                          mapper_chnl_handle_ptr,
                                                          TRUE,
                                                          trunk2client_tables_ptr);
            opu_csf_ovr = TRUE;
        }
    }


	/*get the connection infomation across the CPB*/
	cpb_connect_data_state_get(digi_handle->dcpb_handle,
							   &cpb_connect_data_ptr);

	output_port_type = (util_global_cpb_port_t)cpb_connect_data_ptr->chnl[local_input_pc_ptr->port_type][local_input_pc_ptr->channel].slv.dest_mst_port;
	output_channel = cpb_connect_data_ptr->chnl[local_input_pc_ptr->port_type][local_input_pc_ptr->channel].slv.dest_mst_chnl;

    switch(output_port_type){
        
        /* This is the check if we are in ETRANS mode..
           DEMAP -> ENETline -> DCPB
           If the other side is CPB, check that we are coming from ENET */
        
        case UTIL_GLOBAL_CPB_PORT_ILKN1:
        case UTIL_GLOBAL_CPB_PORT_ILKN2: {
            if ((local_input_pc_ptr->port_type != UTIL_GLOBAL_CPB_PORT_ENET_LINE) && (local_input_pc_ptr->port_type != UTIL_GLOBAL_CPB_PORT_MAPOTN)) {
                PMC_RETURN(DIGI_ERR_INVALID_ARG);
            }
            else {

                /* In an ODUFLEX->CPB->ILKN case, there is no enet_client and the channel numbers will be 0-95, so don't update enet client source */
                if (!(UTIL_GLOBAL_CPB_PORT_MAPOTN == local_input_pc_ptr->port_type && local_input_pc_ptr->channel >= 12)) {
                    result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_line_chnl[local_input_pc_ptr->channel]);
                }

                /* Break apart the mesage and send it to FW. */
                result = digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle,
                                                                                 mapotn_chnl,
                                                                                 trunk2client_tables_ptr
                                                                                 );
            }
            if (PMC_SUCCESS == result) {
                result = digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle,
                                                                       mapotn_chnl,
                                                                       mapping_mode_hostmsg,
                                                                       local_input_pc_ptr->channel,
                                                                       trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time,
                                                                       0 /* ENET LINE client type */
                                                                       );


            }
            
            break;
        }
        case UTIL_GLOBAL_CPB_PORT_ENET_LINE: {
            /* update FW enet client source */
            result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_line_chnl[output_channel]);
            /* local_input_pc.channel is the enet channel we need. */

            /* Break apart the mesage and send it to FW. */
            if (PMC_SUCCESS == result) {
                result = digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle,
                                                                                 mapotn_chnl,
                                                                                 trunk2client_tables_ptr
                    );
            }
                
            if (PMC_SUCCESS == result) {
                result = digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle,
                                                                       mapotn_chnl,
                                                                       mapping_mode_hostmsg,
                                                                       output_channel,
                                                                       trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time,
                                                                       0 /* ENET LINE client type */
                                                                       );


            }
            

            break;
        }
        case UTIL_GLOBAL_CPB_PORT_CBRC: {
            /* local_input_pc.channel is the cbrc channel we need. */

            /* Break apart the mesage and send it to FW. */
            result = digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle,
                                                                             mapotn_chnl,
                                                                             trunk2client_tables_ptr
                                                                             );

            if (PMC_SUCCESS == result) {
                result = digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle,
                                                                       mapotn_chnl,
                                                                       mapping_mode_hostmsg,
                                                                       output_channel,
                                                                       trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time,
                                                                       1 /* CBRC client type */
                                                                       );


            }
            

            break;
        }
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS: {
            /* update FW enet client source */
            result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, (util_global_switch_data_t  *)&digi_handle->handle_pool.enet_sys_chnl[output_channel]);

            if (PMC_SUCCESS == result)
            {
                /* Break apart the mesage and send it to FW. */
                result = digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle,
                                                                                 mapotn_chnl,
                                                                                 trunk2client_tables_ptr
                    );
                    
                if (PMC_SUCCESS == result) {
                    result = digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle,
                                                                                    mapotn_chnl,
                                                                                    mapping_mode_hostmsg,
                                                                                    output_channel,
                                                                                    trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time,
                                                                                    0 /* ENET client type */
                        );
                }

            } else{
                /* This is an invalid configuration  */
                result = DIGI_ERR_INVALID_ARG;
            }
            break;
        }
        default: {            
            /* Currently, we only have the enet/cbrc client to be able to monitor the defects... */
            result = DIGI_ERR_INVALID_ARG;
        }
    }


    if (TRUE == opu_csf_ovr)
    {
        trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time = opu_csf_persistance_time_bkp;
        PMC_MEMCPY((void*)&trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon, (void*)&opu_csf_consq_act_bkp, sizeof(cmf_trunk2client_defect_action_t));
    }

    PMC_RETURN(result);

} /* End of digi_cmf_trunktoclient_conseq_action_cfg_internal*/ 

/*******************************************************************************
*  digi_cmf_opu_csf_monitor_cfg
*  ___________________________________________________________________________
* 
*  DESCRIPTION:
*               This function configures and de/activates FW OPU_CSF monitor.
*               This function should only be called internal during mapper channel
*               activation or deactivation.
*               No consequential actions is set by the function.
*               digi_cmf_trunktoclient_conseq_action_cfg_internal() should be used to do
*               configures consequential actions associated to OPU_CSF.
*
*  INPUTS:    
*               digi_handle             - digi device handle. 
*               chnl_ctxt_ptr           - pointer to mapper channel handle to be operated on.
*               output_chnl             - identifier of output channel mapped to mapper over DCPB or ODUKSW
*               trunk2client_tables_ptr - a pointer to the port mapping table
*                                         and the conseq action table. 
*
*  OUTPUTS: 
*               None. 
* 
*  RETURNS: 
*               PMC_SUCCESS - if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:       
*
******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_opu_csf_monitor_cfg(digi_handle_t                   *digi_handle,
                                              digi_mapper_chnl_t              *chnl_ctxt_ptr,
                                              UINT32                           output_chnl,
                                              digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr)
{
    
#ifndef PMC_SW_SIMULATION
    PMC_ERROR result = PMC_SUCCESS; 
    /* Used to extract the mapping mode */
    digi_mapping_mode_t mode;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapping_mode_hostmsg;
    UINT32 mapotn_chnl;
    util_global_map_adapt_func_t map_mode;
    util_global_map_adapt_func_t demap_mode;
    util_global_mapping_mode_t mapotn_mapping_mode = UTIL_GLOBAL_NO_MAP;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle->dcpb_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    mode = mapper_chnl_handle_ptr->mode;
    if( 
        DIGI_MAP_ODUKP_PACKET_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU4P_100_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode || 
        DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET == mode ||
        DIGI_MAP_ODU2EP_FC_1200_GFP == mode)
    {
        mapping_mode_hostmsg = 0;
    } 
    else
    {
        mapping_mode_hostmsg = 1;
    }

    /* configure monitor and internal tables */
    result = digi_cmf_trunktoclient_conseq_action_table_internal_set(digi_handle,
                                                                     mapotn_chnl,
                                                                     trunk2client_tables_ptr);
    /* update opu csf persistance time in internal table */
    if (PMC_SUCCESS == result) 
    {
        /* retrieve channel mapping mode to determine if it is GFP or not */
        digi_mapotn_mode_params_convert(mode, &map_mode, &demap_mode);
        result = mapotn_mapping_mode_get(digi_handle->mapotn_handle, 
                                         map_mode,
                                         &mapotn_mapping_mode);
   
        if (PMC_SUCCESS == result)
        {
            result = digi_cmf_trunktoclient_port_mapping_table_internal_set(digi_handle,
                                                                            mapotn_chnl,
                                                                            mapping_mode_hostmsg,
                                                                            output_chnl,
                                                                            trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time,
                                                                            UTIL_GLOBAL_GFP == mapotn_mapping_mode ? 0 : 1);
        }    
    }
    
    /* activate/deactivate monitor */
    if (PMC_SUCCESS == result)
    {
        result = digi_cmf_trunktoclient_tables_update(digi_handle,
                                                      chnl_ctxt_ptr);
    }  
    PMC_RETURN(result);
#else
    PMC_ENTRY();
    PMC_RETURN(PMC_SUCCESS);

#endif /*PMC_SW_SIMULATION*/


} /* End of digi_cmf_opu_csf_monitor_cfg*/ 

#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
*  digi_cmf_clienttotrunk_sw_manual_trigger
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function forces a manual trigger on the given enet client link. This is
*   to be used in conjunction with the clienttotrunk_conseq_action tables. When
*   the sw force action is set and then the sw trigger is called, if the highest
*   priority asserted defect is the sw manual trigger, the actions will be forced
*   onto that channel. 
*  
*  INPUTS:
*   digi_handle - digi device handle.
*   chnl_ctxt_ptr - pointer to mapper channel handle to be operated on.\n
*   trigger - set or stop manual trigger on this channel\n
*             1 ~ Force SW manual trigger\n
*             0 ~ Stop SW manual trigger\n
* 
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   CMD_CFG_CMF_CLIENTTOTRUNK_SW_FORCE 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_sw_manual_trigger(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr, 
                                                          BOOL8 trigger)
{   
    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    /* Used to retrieve the enet port and channel from the mapotn channel*/
    util_global_switch_data_def_t *local_output_sw_ptr; 
    util_global_cpb_port_data_t *local_output_pc_ptr;
    util_global_cpb_port_data_t local_input_pc;

    BOOL8 is_multicast = FALSE;
    BOOL8 is_broadcast = FALSE;
    BOOL8 is_reader_primary = FALSE;
    BOOL8 is_slave_zone_primary = FALSE;
    cpb_handle_t* cpb_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);

    local_output_sw_ptr = &(mapper_chnl_handle_ptr->switch_data);
    local_output_pc_ptr = &(local_output_sw_ptr->cpb_data);

    cpb_handle = digi_handle->dcpb_handle;

    result = cpb_chnl_source_type_and_primary_test( cpb_handle,
                                                    local_output_pc_ptr->port_type,
                                                    local_output_pc_ptr->channel,
                                                    (UINT32*)&(local_input_pc.port_type),
                                                    &(local_input_pc.channel),
                                                    &is_broadcast,
                                                    &is_multicast,
                                                    &is_reader_primary,
                                                    &is_slave_zone_primary);

    if (PMC_SUCCESS == result)
    {

        /* Add message body to the previously created header. */
        /* Words 2 of CMD_CFG_CMF_CLIENTTOTRUNK_SW_FORCE have the following format:
         * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
         *   2  ------------enet_chnl-------|---------trigger----------
         */
		if ( (local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) ||
			 (local_output_pc_ptr->port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) )
		{
			data[0] = (local_output_pc_ptr->channel << 16) | trigger ; 
		} else {
			/* Increment and initialize storage. */  
			data[0] =  (local_input_pc.channel << 16) | trigger ;
		}

    }
    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_SW_FORCE_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_clienttotrunk_sw_manual_trigger*/   


/*******************************************************************************
*  digi_cmf_non_std_upi_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function will pass the FW the required non standard UPI values needed
*   in the CMF frames. The FW will then execute all actions with the requested
*   UPI values in the CMF frames. 
*  
*  INPUTS:
*   digi_handle       - pointer to DIGI handle instance.
*   chnl_ctxt_ptr     - mapotn channel handle
*   upi_values        - struct of alternate UPI values for FW to use\n
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cmf_non_std_upi_cfg(digi_handle_t *digi_handle,
                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                          digi_cmf_upi_values_t upi_values)
{   

    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data_len = 6; 
    UINT32 data[data_len];
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    if (PMC_SUCCESS == result) {
        mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);  

        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

        /* Words 0 of CMD_CFG_CMF_CLIENTTOTRUNK_SW_FORCE have the following format:
         * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
         *   0  ----------------------------mapotn_channel-----------------
         *   1  ----------------------------LOS-UPI------------------------
         *   2  ----------------------------LOSS_SYNC-UPI------------------
         *   3  ----------------------------DCI-UPI------------------------
         *   4  ----------------------------FDI-UPI------------------------
         *   5  ----------------------------RDI-UPI------------------------
         */
        data[0] = mapotn_chnl;
        data[1] = upi_values.los_upi;      
        data[2] = upi_values.loss_sync_upi;
        data[3] = upi_values.dci_upi;      
        data[4] = upi_values.fdi_upi;      
        data[5] = upi_values.rdi_upi;      
    }


    if (PMC_SUCCESS == result) {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle, MODID_CMF,HOSTMSG_CMD_CMF_CFG_NON_STD_UPI_VALUES_ST,data,6,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_cmf_non_std_upi_cfg*/   
   

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
*  digi_mac_mac_lf_rf_forwarding
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function creates a 'pseudo-transparent' operation for LF/RF
*   in MAC-MAC mode. It will monitor the LF/RF on both the ENET LINE
*   and the ENET SYS and trigger a LF/RF on the opposite ENET subsystem.
*  
*  INPUTS:
*   digi_handle - digi device handle. 
*   feature_en  - Boolean that tells FW to start or stop the LF/RF forwarding\n
*                      1 ~ Turn ON\n
*                      0 ~ Turn OFF\n
*
*  OUTPUTS: 
*   None. 
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   This is a reserved operating mode and should not be used without
*   contacting your apps representative. 
*   CMD_CFG_CMF_MAC_MAC_LF_RF_FORWARDING 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mac_mac_lf_rf_forwarding(digi_handle_t *digi_handle,
                                               BOOL8 feature_en )
{   

    PMC_ERROR result = PMC_SUCCESS; 
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Words 0 of CMD_CFG_CMF_CLIENTTOTRUNK_SW_FORCE have the following format:
     * Word   bits 24..31    bits 16..23    bits 8..15     bits 0..7
     *   0  -----------------reserved-------------------|---feature_en----
     */
    data[0] = feature_en ;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_CMF,HOSTMSG_CMD_CMF_CFG_MAC_MAC_LF_RF_FORWARDING_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* End of digi_mac_mac_lf_rf_forwarding*/   

#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** End of file
*/
