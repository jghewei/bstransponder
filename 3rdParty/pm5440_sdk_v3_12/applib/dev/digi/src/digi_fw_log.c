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
*   DESCRIPTION : This module contains functions for debugging and logging firmware.
*
*   NOTES:
*
*******************************************************************************/

#include "digi_loc.h"
#include "digi_resource_loc.h"
#include "hostmsg_loc.h"

/*******************************************************************************
* STRUCTURE: digi_fw_assert_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*       Structure defining firmware stats.
*
* ELEMENTS:
*       see structure.
*******************************************************************************/
typedef struct pmcfw_assert_info_t 
{
    UINT32 cookie;               /*!< should be 0xf00dfeed if structure contains information */
    UINT32 counter;              /*!< number of times this structure has been updated */
    UINT32 assert_error_id;      /*!< error_id of line number of assert */
    UINT32 assert_line;          /*!< line number of assert */
    char   assert_filename[100]; /*!< file name of assert */
} pmcfw_assert_info_t;

/*******************************************************************************
* STRUCTURE: dbg_log_app_entry_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*       Structure defining firmware a firmware log message.
*
* ELEMENTS:
*       see structure.
*******************************************************************************/
typedef struct dbg_log_app_entry_t
{
    UINT32  log_word0;  /*!< Log word 0                                                                               */
    UINT32  timestamp;  /*!< 2-bit system timestamp.                                                                  */
    UINT32  seq_num;    /*!< Lower 32 bit of sequence number for corresponding entry in DBG log.                      */
    UINT32  log_code;   /*!< 32-bit log code, shall be in the format: [16-bit module ID | 16-bit user defined field]  */
    UINT32  log_word2;  /*!< Generic 32-bit log word                                                                  */
    UINT32  log_word3;  /*!< Generic 32-bit log word                                                                  */
    UINT32  log_word4;  /*!< Generic 32-bit log word                                                                  */
} dbg_log_app_entry_t;


BOOL8 digi_fw_firmware_logging_enabled = FALSE;

/* ignore this section for doxygen */
#ifndef DOXYGEN_PUBLIC_ONLY
#ifndef PMC_DOX_FOR_SWIG
static void digi_fw_show_write(UINT32 addr, UINT32 mask, UINT32 value, char *buffer);
static void digi_fw_show_read(UINT32 addr, UINT32 mask, UINT32 value, char *buffer);
static void digi_fw_show_rmw(UINT32 addr, UINT32 mask, UINT32 value, char *buffer) ;
#endif

/* 
 ** Forward References 
 */

/*******************************************************************************
* digi_fw_opsa_debug_status_internal_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves debug state and configuration variables for OPSA start up.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  subsystem   - subsystem to read
*  channel     - channel of the specified subsystem to read
*
* OUTPUTS:
*  fw_opsa_status_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES: CMD_REQ_OPSA_Get_Debug_Status message.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_opsa_debug_status_internal_get(digi_handle_t *digi_handle,
                                                         hostmsg_opsa_subsystem_cfg_t subsystem,
                                                         UINT8 channel,
                                                         fw_opsa_status_t *fw_opsa_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 data[1];
    UINT32 response[9];

    PMC_ENTRY();

    PMC_ASSERT((NULL != fw_opsa_status_ptr),
               HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    data[0] = ((UINT16)subsystem << 16) | channel;    

    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_OPSA,HOSTMSG_CMD_REQ_OPSA_GET_DEBUG_STATUS_ST,data,1,response,9,NULL,stat);
    }

    if (PMC_SUCCESS == result)
    {
        PMC_LOG(PMC_LOG_SEV_LOW, DIGI_LOG_OBND_DATA, response[0], 0,
                "data[2] = %x\n", response[0]);

        /* Return requested data. */
        fw_opsa_status_ptr->mpma_enable   = response[0];
        fw_opsa_status_ptr->mpma_qs_ch_en = response[1];
        fw_opsa_status_ptr->mpma_state    = response[2];
        fw_opsa_status_ptr->mpma_tick_cnt = response[3];
        fw_opsa_status_ptr->mpmo_enable   = response[4];
        fw_opsa_status_ptr->mpmo_ch_data_target = response[5];
        fw_opsa_status_ptr->mpmo_acb_instance = response[6];
        fw_opsa_status_ptr->mpmo_state    = response[7];
        fw_opsa_status_ptr->mpmo_tick_cnt = response[8];
    }

    PMC_RETURN(result);

} /* digi_fw_opsa_debug_status_internal_get */

#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
* digi_fw_opsa_debug_status_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves debug state and configuration variables for OPSA start up.
*
* INPUTS:
*  digi_handle - DIGI device handle.
*  subsystem   - subsystem to read
*  channel     - channel of the specified subsystem to read
*
* OUTPUTS:
*  fw_opsa_status_ptr - pointer to storage for returned status.\n
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  NOTES: CMD_REQ_OPSA_Get_Debug_Status message.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_debug_status_get(digi_handle_t *digi_handle,
                                               hostmsg_opsa_subsystem_cfg_t subsystem,
                                               UINT8 channel,
                                               fw_opsa_status_t *fw_opsa_status_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_fw_opsa_debug_status_internal_get(digi_handle, (hostmsg_opsa_subsystem_cfg_t)subsystem, 
                                                    channel, fw_opsa_status_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_fw_opsa_debug_status_get */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* digi_fw_opsa_mpmo_debug_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves mpmo debug variables
*  
* INPUTS:
*  digi_handle - DIGI device handle.
*  
* OUTPUTS:
*  opsa_mpmo_debug_addr - SPRAM address of opsa mpmo debug variables.
*  opsa_mpmo_debug_size - size of opsa mpmo debug variables.
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_mpmo_debug_addr_get(digi_handle_t *digi_handle,
                                                  UINT32 *opsa_mpmo_debug_addr,
                                                  UINT32 *opsa_mpmo_debug_size)
{
    struct {
        UINT32 dbg_data_subsystem;
    } cfg_msg;
    struct {
        UINT32 addr;
        UINT32 size;
    } response;
    PMC_ERROR result;
    HOSTMSG_DECLARE_QUERY_STAT(stat);

    PMC_ENTRY();

    cfg_msg.dbg_data_subsystem = HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_OPSA_MPMO; 

    result = hostmsg_perform_fixed_length_query((pmc_handle_t*)digi_handle,
                                                MODID_DBG_LOG,
                                                DBG_LOG_HOSTMSG_STYPE_GET_DBG_DATA_ADDR,
                                                (UINT32 *)&cfg_msg, 1,
                                                (UINT32 *)&response, 2,
                                                stat);

    /* note this function returns a spram address and the size of the structure in spram */
    if (PMC_SUCCESS == result) {

        /* if the firmware reports that the size is larger than the buffer we will be using,
           then limit the size to just the size fw_opsa_mpmo_debug_t */
        if (response.size > sizeof(fw_opsa_mpmo_debug_t))
        {
            response.size = sizeof(fw_opsa_mpmo_debug_t);
        }

        /* mask the firmware addresses with 0x1FFFFFF to convert from MIPS bus addresses to PCIE bus addresses */
        response.addr = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response.addr );

        *opsa_mpmo_debug_addr = response.addr;
        *opsa_mpmo_debug_size = response.size;
    }

    PMC_RETURN(result);

} /* digi_fw_opsa_mpmo_debug_addr_get */

/*******************************************************************************
* digi_fw_opsa_mpmo_debug_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves mpmo debug variables
*  
* INPUTS:
*  digi_handle - DIGI device handle.
*  
* OUTPUTS:
*  fw_opsa_mpmo_debug_ptr - pointer to storage for returned status.
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_opsa_mpmo_debug_get(digi_handle_t *digi_handle,
                                            fw_opsa_mpmo_debug_t *fw_opsa_mpmo_debug_ptr)
{
    PMC_ERROR result;
    UINT32 addr;
    UINT32 size;

    PMC_ENTRY();

    result = digi_fw_opsa_mpmo_debug_addr_get(digi_handle,&addr,&size);

    if (PMC_SUCCESS == result) {
        pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;

        /* read debug structure from spram */
        pmc_sys_burst_read(sys_handle,addr,size/4,(UINT32*)fw_opsa_mpmo_debug_ptr);
    }

    PMC_RETURN(result);

} /* digi_fw_opsa_mpmo_debug_get */


#ifdef DIGI_FW_STATS
/*******************************************************************************
* digi_fw_stats_reset
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Reset firmware statistics
*  
* INPUTS:
*  digi_handle - DIGI device handle.

* OUTPUTS:
*    None
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_stats_reset(digi_handle_t *digi_handle)
{
    PMC_ERROR result;
    HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

    PMC_ENTRY();

    result = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,
                                      MODID_STATS,
                                      STATS_HOSTMSG_STYPE_RESET,
                                      NULL,
                                      0,
                                      stat );

    PMC_RETURN(result);

} /* digi_fw_stats_reset */


/*******************************************************************************
* digi_fw_stats_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Get SPRAM addresses of firmware statistics,
*  and SPRAM addresses of firmware assert info,
*  and SPRAM addresses of firmware logging info.
*
* INPUTS:
*  digi_handle - DIGI device handle.

* OUTPUTS:
*  *digi_fw_stats - structure holding various firmware addresses.
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*  RETURNS:
*    This also flushes the firmware statistics from MIPS cache.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_fw_stats_get(digi_handle_t *digi_handle, digi_fw_stats_t *digi_fw_stats)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 response[8];
    UINT32 actual_response_len;
    HOSTMSG_DECLARE_QUERY_STAT(stat);

    PMC_ENTRY();

    digi_fw_stats->stats_number_of_stats_addr = 0;
    digi_fw_stats->stats_list_of_statistics_addr = 0;
    digi_fw_stats->stats_command_addr = 0;
    digi_fw_stats->pmcfw_assert_info_addr = 0;

    digi_fw_stats->app_log_size = 0;               
    digi_fw_stats->app_log_ptr_addr = 0;            
    digi_fw_stats->app_log_wr_idx_addr = 0;        
    digi_fw_stats->app_log_num_entries_max = 0;     
    digi_fw_stats->stats_number_of_stats = 0;  
    digi_fw_stats->firmware_stats_buffer = NULL;  

    result = hostmsg_perform_variable_length_query((pmc_handle_t*)digi_handle,
                                                   MODID_STATS,
                                                   STATS_HOSTMSG_STYPE_GET,
                                                   NULL, 0,
                                                   response, 8,
                                                   &actual_response_len,
                                                   stat);


    if (PMC_SUCCESS == result)
    {
        /* mask the firmware addresses with 0x1FFFFFF to convert from MIPS bus addresses to PCIE bus addresses */
        digi_fw_stats->stats_number_of_stats_addr    = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response[0] );
        digi_fw_stats->stats_list_of_statistics_addr = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response[1] );
        digi_fw_stats->stats_command_addr            = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response[2] );
        digi_fw_stats->pmcfw_assert_info_addr        = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response[3] );

        digi_fw_stats->app_log_size                  = response[4];          
        digi_fw_stats->app_log_ptr_addr              = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response[5] );           
        digi_fw_stats->app_log_wr_idx_addr           = CONVERT_MIPS_ADDR_TO_HOST_ADDR( response[6] );       
        digi_fw_stats->app_log_num_entries_max       = response[7];
    }

    PMC_RETURN(result);

} /* digi_fw_stats_get */


/*******************************************************************************
* digi_fw_assert_tripped
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Has firmware generated an assert?
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    TRUE  - an assert was tripped
*    FALSE - firmware is running normally.
*
*******************************************************************************/
PRIVATE BOOL digi_fw_assert_tripped(digi_handle_t *digi_handle)
{
    PMC_ENTRY();
#ifdef DIGI_FW_STATS

    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;

    if (digi_fw_stats->pmcfw_assert_info_addr != 0) 
    {
        UINT32 cookie = pmc_sys_reg_read(sys_handle,digi_fw_stats->pmcfw_assert_info_addr);
        if (cookie == 0xf00dfeed) 
        {
            PMC_RETURN(TRUE);
        }
    }
#endif
    PMC_RETURN(FALSE);
} /* digi_fw_assert_tripped */



/*******************************************************************************
* digi_fw_show_assert_info
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware assert information
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_show_assert_info(digi_handle_t *digi_handle)
{
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    char *digi_name = digi_handle->base.tsb_name;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;

    PMC_ENTRY();

    if (digi_fw_stats->pmcfw_assert_info_addr != 0) {
        pmcfw_assert_info_t pmcfw_assert_info;

        pmc_sys_burst_read(sys_handle,digi_fw_stats->pmcfw_assert_info_addr,sizeof(pmcfw_assert_info_t)/sizeof(UINT32),(UINT32*)&pmcfw_assert_info);
        if (pmcfw_assert_info.cookie == 0xf00dfeed) {
            FW_LOG("FIRMWARE: %s assert counter = %d\n",digi_name,pmcfw_assert_info.counter);
            FW_LOG("FIRMWARE: %s assert error_id = %d (0x%x)\n",digi_name,pmcfw_assert_info.assert_error_id,pmcfw_assert_info.assert_error_id);
            FW_LOG("FIRMWARE: %s assert line = %d\n",digi_name,pmcfw_assert_info.assert_line);
            FW_LOG("FIRMWARE: %s assert file = %s\n",digi_name,pmcfw_assert_info.assert_filename);
            FW_LOG("\n");
        } else { 
            FW_LOG("FIRMWARE: %s has not hit an assert\n",digi_name);
        }
    }

    PMC_RETURN();
} /* digi_fw_show_assert_info */


/*******************************************************************************
* digi_fw_module_id
* ___________________________________________________________________________
*
* DESCRIPTION:
*   retrieve the module name and message subtype from a module id and sub type id
*  
* INPUTS:
*   module_id    - firmware module id
*   sub_type_id  - message sub type
*  
* OUTPUTS:
*   mod_name_ptr - module name as a const char *
*   sub_type_ptr - sub type name as a const char *
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_module_id(UINT32 module_id, const char **mod_name_ptr, UINT32 sub_type_id, const char**sub_type_ptr)
{
    static char mod_name_buffer[15];
    static char sub_type_buffer[15];
    const char *mod_name = (const char *)mod_name_buffer;
    const char *sub_type = (const char *)sub_type_buffer;

    PMC_ENTRY();

    PMC_SPRINTF(mod_name_buffer,"MOD:%d",module_id);
    PMC_SPRINTF(sub_type_buffer,"SUB:%d",sub_type_id);
    switch (module_id) {
    case 0x01 : mod_name = "PTP";
        switch(sub_type_id) {
            case 0x01  : sub_type = "SET_PTP_CONFIG"; break;
            case 0x02  : sub_type = "SET_IRIG_CONFIG"; break;
            case 0x03  : sub_type = "SET_PTP_IPV4_PARAM"; break;
            case 0x04  : sub_type = "SET_PTP_IPV6_PARAM"; break;
            case 0x05  : sub_type = "GET_RX_MSG_CNTS"; break;
            case 0x06  : sub_type = "GET_PATH_DELAY"; break;
            case 0x07  : sub_type = "SET_PORT_ASYMMETRY"; break;
            case 0x10  : sub_type = "SET_DEFAULT_PROPERTIES"; break;
            case 0x11  : sub_type = "SET_TIME_PROPERTIES"; break;
            case 0x12  : sub_type = "SET_ACCEPTABLE_MASTER_CLOCKS"; break;
            case 0x13  : sub_type = "START_INITIALIZE"; break;
            case 0x14  : sub_type = "GET_MASTER_CLOCK_DATABASE_STATUS"; break;
            case 0x15  : sub_type = "GET_MASTER_CLOCK_DATABASE"; break;
            case 0x16  : sub_type = "SET_PARENT_DATA"; break;
            case 0x17  : sub_type = "SET_PORT_DATA"; break;
            case 0x18  : sub_type = "SET_PORT_STATE"; break;
            case 0x19  : sub_type = "SET_PORT_EVENT"; break;
            case 0x1A  : sub_type = "GET_PORT_STATE"; break;
            case 0x1B  : sub_type = "SET_PASSIVE_PORT_PARENT"; break;
            case 0x1C  : sub_type = "SET_UNICAST_MASTER_PARAM"; break;
            case 0x1D  : sub_type = "SET_UNICAST_REQUEST"; break;
            case 0x1E  : sub_type = "SET_UNICAST_CANCEL"; break;
            case 0x1F  : sub_type = "GET_UNICAST_SLAVE_STATE"; break;
            case 0x30  : sub_type = "SET_PASS_THRU_GENERAL_PARAM"; break;
            case 0x31  : sub_type = "SET_PASS_THRU_PORT_PARAM"; break;
            case 0x40  : sub_type = "SET_TC_PARAM"; break;
            case 0x41  : sub_type = "SET_TC_PORT_PARAM"; break;
            case 0x50  : sub_type = "GET_PTP_STABILITY_STATUS"; break;
            case 0xFF  : sub_type = "SET_PTP_DEBUG_PARAM"; break;
        }
    break;
    case 0x02 : mod_name = "SYNC_E";
    break;
    case 0x03 : mod_name = "LINKOAM";
    break;
    case 0x04 : mod_name = "SERDES";
        switch(sub_type_id) {
            case 0x01 : sub_type = "CFG_C8"; break;
            case 0x02 : sub_type = "CFG_T8"; break;
            case 0x03 : sub_type = "CFG_S16"; break;
            case 0x04 : sub_type = "CFG_SFI51"; break;
            case 0x05 : sub_type = "CFG_D8"; break;
            case 0x06 : sub_type = "CFG_START_ADAPT"; break;
            case 0x07 : sub_type = "CFG_C8_WRITE_ADAPT"; break;
            case 0x08 : sub_type = "GET_C8_READ_ADAPT"; break;
            case 0x09 : sub_type = "CFG_T8_WRITE_ADAPT"; break;
            case 0x0A : sub_type = "GET_T8_READ_ADAPT"; break;
            case 0x0B : sub_type = "CFG_S16_WRITE_ADAPT"; break;
            case 0x0C : sub_type = "GET_S16_READ_ADAPT"; break;
            case 0x0D : sub_type = "GET_ADAPT_STATUS"; break;
            case 0x0E : sub_type = "CFG_SINGLE_S16"; break;
            case 0x0F : sub_type = "GET_SINGLE_S16_STATUS"; break;
            case 0x10 : sub_type = "POSTED_D8"; break;    
            case 0x11 : sub_type = "POSTED_D8_STATUS"; break;
            case 0x12 : sub_type = "POSTED_SFI51"; break;
            case 0x13 : sub_type = "POSTED_SFI51_STATUS"; break;
            case 0x14 : sub_type = "POSTED_START_ADAPT"; break;
            case 0x15 : sub_type = "POSTED_START_ADAPT_STATUS"; break;
            case 0x90 : sub_type = "CFG_DEBUG"; break;
            case 0x91 : sub_type = "GET_DEBUG_STATUS"; break;
        }
    break;
    case 0x05 : mod_name = "CMF";
        switch (sub_type_id) {
            case 0x01 : sub_type = "SEND_ONESHOT_CMF"; break;
            case 0x02 : sub_type = "REQ_CMF_EXTRACT"; break;
            case 0x03 : sub_type = "SEND_ONESHOT_OPU_CSF"; break;
            case 0x04 : sub_type = "SET_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE"; break;
            case 0x05 : sub_type = "SET_CLIENTTOTRUNK_PORT_ACTION_TABLE"; break;
            case 0x06 : sub_type = "SET_CLIENTTOTRUNK_UPDATE_TABLES"; break;
            case 0x07 : sub_type = "REQ_CLIENTTOTRUNK_DEFECT_STATUS"; break;
            case 0x08 : sub_type = "SET_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE"; break;
            case 0x09 : sub_type = "SET_TRUNKTOCLIENT_PORT_MAPPING_TABLE"; break;
            case 0x0A : sub_type = "SET_TRUNKTOCLIENT_UPDATE_TABLES"; break;
            case 0x0B : sub_type = "SET_TRIGGER_LF"; break;
            case 0x0C : sub_type = "SET_TRIGGER_RF"; break;
            case 0x0D : sub_type = "SET_ENET_EMAC_LF_RF"; break;
            case 0x0E : sub_type = "REQ_OPU_CSF_STATUS"; break;
            case 0x0F : sub_type = "REQ_TRUNKTOCLIENT_CONSQ_ACTION_TABLE "; break;
            case 0x10 : sub_type = "REQ_CLIENTTOTRUNK_CONSQ_ACTION_TABLE "; break;
            case 0x11 : sub_type = "SET_CLIENTTOTRUNK_SW_FORCE"; break;
            case 0x12 : sub_type = "SET_MAC_MAC_LF_RF_FORWARDING"; break;
            case 0x13 : sub_type = "REQ_CLIENTTOTRUNK_PORT_ACTION_TABLE"; break;
            case 0x14 : sub_type = "REQ_TRUNKTOCLIENT_PORT_MAPPING_TABLE"; break;
        }
    break;
    case 0x06 : mod_name = "COREOTN";
        switch(sub_type_id) {
            case 0x01 : sub_type = "SET_STS_SW_CFG"; break;
            case 0x02 : sub_type = "SET_STS_SW_NULL_CFG"; break;
            case 0x03 : sub_type = "CFG_DLOOMFI_MON"; break;
            case 0x04 : sub_type = "SET_STS_SW_INIT"; break;
            case 0x05 : sub_type = "SET_STS_SW_HO_CAL_NULL_CFG"; break;
            case 0x06 : sub_type = "SET_STS_SW_CFG_ALL"; break;
        }    
    break;
    case 0x07 : mod_name = "OPSA";
        switch(sub_type_id) {
            case 0x01 : sub_type = "CFG_MPMA_PORT_ADD"; break;
            case 0x02 : sub_type = "CFG_MPMA_PORT_DROP"; break;
            case 0x03 : sub_type = "CFG_MPMO_PORT_ADD"; break;
            case 0x04 : sub_type = "CFG_MPMO_PORT_DROP"; break;
            case 0x05 : sub_type = "CFG_TXJC_CFC_FIFO_CLRB"; break;
            case 0x06 : sub_type = "CFG_FIFO_CENT_TRIGGER"; break;
            case 0x07 : sub_type = "CFG_TAU_VALUE"; break;
            case 0x08 : sub_type = "CFG_MPMO_TIMES"; break;
            case 0x91 : sub_type = "REQ_GET_DEBUG_STATUS"; break;
        }
    break;
    case 0x08 : mod_name = "LED";
    break;
    case 0x09 : mod_name = "ENET";
        switch(sub_type_id) {
            case 0x01: sub_type = "SET_ENET_CLIENT_SOURCE"; break;            
            case 0x02: sub_type = "GET_ENET_CLIENT_SOURCE"; break;            
            case 0x03: sub_type = "ECLASS_INIT"; break;                       
            case 0x04: sub_type = "ECLASS_HWIRED_CONFIG"; break;              
            case 0x05: sub_type = "ECLASS_GEN_CONFIG"; break;                 
            case 0x06: sub_type = "ENET_INTR_MONITOR_CONFIG"; break;          
            case 0x07: sub_type = "ENET_GET_INTR_MONITOR_STATUS"; break;
        }
    break;
    case 0x0A : mod_name = "CBRC";
    break;
    case 0x0B : mod_name = "PMON";
        switch(sub_type_id) {
            case 0x01 : sub_type = "CFG_COLLECTION_MODE"; break;   
            case 0x02 : sub_type = "CFG_WORK_ORDER"; break;   
            case 0x03 : sub_type = "CFG_RCP"; break;   
            case 0x04 : sub_type = "CFG_DDEG_COREOTN_SEG"; break;   
            case 0x05 : sub_type = "CFG_DDEG_PARAM"; break;   
            case 0x06 : sub_type = "CFG_DDEG_EN"; break;   
            case 0x07 : sub_type = "CFG_DDEG_INT_EN"; break;   
            case 0x08 : sub_type = "REQ_GET_DDEG_INT"; break;   
            case 0x09 : sub_type = "REQ_GET_DDEG_PARAM"; break;   
            case 0x0A : sub_type = "REQ_GET_DDEG_ENABLE_STATUS"; break;   
            case 0x0B : sub_type = "CFG_FEC"; break;   
            case 0x0C : sub_type = "CFG_HARDWARE_DDEG_LCLK"; break;   
            case 0x10 : sub_type = "MANUAL_TRIGGER"; break;   
            case 0x11 : sub_type = "CFG_MANUAL_FEC_UPDATE"; break;   
            case 0x12 : sub_type = "CFG_SET_RCP_LOS"; break;   
            case 0x13 : sub_type = "CFG_PMON_SYNC_SOURCE"; break;   
            case 0x20 : sub_type = "REQ_GET_STATUS"; break;   
            case 0x21 : sub_type = "REQ_GET_DDEG_STATE"; break;   
            case 0x22 : sub_type = "CFG_PRBS_COLLECTION_MODE"; break;   
        }
    break;
    case 0x0C : mod_name = "LINEOTN";
    break;
    case 0x0D : mod_name = "PERFORMANCE";
        switch(sub_type_id) {
            case 0x01 : sub_type = "INIT_SPRAM_QUEUE"; break;   
            case 0x02 : sub_type = "INIT_HOST_QUEUE"; break; 
        }
    break;
    case 0x80 : mod_name = "INFO_DBG";
        switch(sub_type_id) {
            case 0x01 : sub_type = "GET_STATUS"; break;   
            case 0x02 : sub_type = "GET_VERSION"; break;   
            case 0x03 : sub_type = "FW_SHUTDOWN"; break;   
            case 0x04 : sub_type = "SPRAM_PCIE_WRITE_ENABLE"; break;
            case 0x05 : sub_type = "GET_SPRAM_SW_RESERVED_ADDR"; break;
        }
    break;
    case 0x81 : mod_name = "DBG_LOG";
        switch(sub_type_id) {
            case 0x01 : sub_type = "SET_APP_FILTERS"; break;   
            case 0x02 : sub_type = "GET_APP_FILTER_INFO"; break;   
            case 0x03 : sub_type = "CLEAR_APP_LOG"; break;   
            case 0x04 : sub_type = "GET_LOG_ADDR"; break;
            case 0x05 : sub_type = "GET_DBG_DATA_ADDR"; break;
        }
    break;
    case 0x82 : mod_name = "BENCH";
        switch(sub_type_id) {
            case 0x01 : sub_type = "MEM_TEST"; break;   
            case 0x02 : sub_type = "NOOP"; break;   
            case 0x03 : sub_type = "PING"; break;   
            case 0x04 : sub_type = "DELAY"; break;
        }
        break;
    break;
    case 0x83 : mod_name = "STATS";
    break;
    case 0x90 : mod_name = "UNIT_TEST";
    break;
    case 0x91 : mod_name = "HOSTMSG_TEST";
    break;
    case 0xff : mod_name = "INVALID";
    break;
    }
    *mod_name_ptr = mod_name;
    *sub_type_ptr = sub_type;

    PMC_RETURN();
} /* digi_fw_module_id */

/*******************************************************************************
* digi_fw_log_get_function_name
* ___________________________________________________________________________
*
* DESCRIPTION:
*   retrieve the module name and message subtype from the first 2 words of a firmware message.
*  
* INPUTS:
*   word0  - first word of firmware message
*   word1  - second word of firmware message
*  
* OUTPUTS:
*   mod_name_ptr - module name as a const char *
*   sub_type_ptr - sub type name as a const char *
*
*  RETURNS:
*    none
*
*******************************************************************************/
static void digi_fw_log_get_function_name(UINT32 word0, UINT32 word1, const char **mod_name_ptr, const char**sub_type_ptr)
{
    UINT32 module_id   = (word0 & 0xFF000000) >> 24;
    UINT32 sub_type_id = (word1 & 0xFF000000) >> 24;
    PMC_ENTRY();

    digi_fw_module_id(module_id,mod_name_ptr,sub_type_id,sub_type_ptr);

    PMC_RETURN();
}
/*******************************************************************************
* digi_fw_stat_name_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*   retrieve the statistic name that corresponds to the specified firmware statistic number.
*  
* INPUTS:
*   digi_handle - handle to digi device
*   stat_number  - firmware statistic number
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    stat_name - const char *, firmware statistic name - (or NULL if no stat records found)
*
*******************************************************************************/
static const char *digi_fw_stat_name_get(digi_handle_t *digi_handle, UINT32 stat_number)
{
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    const char *stat_name = NULL;
    PMC_ENTRY();

    if (stat_number < digi_fw_stats->stats_number_of_stats) {
        stat_name = digi_fw_stats->firmware_stats_buffer[stat_number].description;
    }

    PMC_RETURN(stat_name);
}

/*******************************************************************************
* digi_fw_show_log_entry
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display a firmware data log entry.
*  
* INPUTS:
*   digi_handle - handle to digi device
*   e -  firmware data log entry dbg_log_app_entry_t.
*   timestamp_offset - last timestamp.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_show_log_entry(digi_handle_t *digi_handle, dbg_log_app_entry_t *e, UINT32 timestamp_offset)
{
    UINT32 addr = e->log_word2 & 0x1FFFFFF;
    UINT32 delta_timestamp = e->timestamp - timestamp_offset;
    DOUBLE delta_microseconds = delta_timestamp / 300.0; /* convert from mips counts to microseconds */
    DOUBLE t_microseconds = e->timestamp / 300.0; /* convert from mips counts to microseconds */
    char delta_timestamp_buff[20];
    char t_timestamp_buff[20];
    char *digi_name = digi_handle->base.tsb_name;

    PMC_ENTRY();

    /* convert the timestamp to microseconds */
    pmc_sys_print_time(delta_microseconds,1.0,delta_timestamp_buff);
    pmc_sys_print_time(t_microseconds,1.0,t_timestamp_buff);
    FW_LOG("FW LOG: %s S=%d T=%-8s dt=%-7s ",digi_name,e->seq_num,t_timestamp_buff,delta_timestamp_buff);
    /*FW_LOG("%08x %08x %08x %08x ",e->log_code,e->log_word2,e->log_word3,e->log_word4);*/
    switch (e->log_code) {
        case 0x1002000b:
            {
                UINT32 value = e->log_word3;
                char buffer[800];
                digi_fw_show_read(addr,0xffffffff,value,buffer);
                FW_LOG("RD 0x%08x -> 0x%08x                   %s\n",addr,value,buffer);
            }
        break;
    case 0x1002000c:
            {
                UINT32 value = e->log_word3;
                char buffer[800];
                digi_fw_show_write(addr,0xffffffff,value,buffer);
                FW_LOG("WR 0x%08x <- 0x%08x                   %s\n",addr,value,buffer);
            }
        break;
    case 0x1002000d:
            {
                UINT32 mask = e->log_word3;
                UINT32 value = e->log_word3;
                char buffer[800];
                digi_fw_show_rmw(addr,mask,value,buffer);
                FW_LOG("SETMSK 0x%08x msk=0x%08x              %s\n",addr,e->log_word3,buffer);
            }
        break;
    case 0x1002000e:
            {
                UINT32 mask = e->log_word3;
                UINT32 value = 0;
                char buffer[800];
                digi_fw_show_rmw(addr,mask,value,buffer);
                FW_LOG("CLRMSK 0x%08x msk=0x%08x              %s\n",addr,e->log_word3,buffer);
            }
        break;
    case 0x1002000f:
            {
                UINT32 mask = e->log_word3;
                UINT32 value = e->log_word4;
                char buffer[800];
                digi_fw_show_rmw(addr,mask,value,buffer);
                FW_LOG("RMW 0x%08x msk=0x%08x val=0x%08x  %s\n",addr,e->log_word3,e->log_word4,buffer);
            }
        break;

    case 0x10020010:
        {
            UINT32 mask = e->log_word3;
            UINT32 value = e->log_word4;
            char buffer[800];
            digi_fw_show_write(addr,mask,value,buffer);
            FW_LOG("POLL 0x%08x msk=0x%08x val=0x%08x %s\n",addr,e->log_word3,e->log_word4,buffer);
        }
        break;

    case 0x100d000c:
        FW_LOG("PMON_ERR_LINEOTN_FEC_TASK_OVERRUN_LCLK - PMON: PMON LINEOTN FEC Task taking longer than LCLK interval. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d000d:
        FW_LOG("PMON_ERR_TASK_OVERRUN_LCLK - PMON: PMON Task taking longer than LCLK interval. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d001b:
        FW_LOG("PMON_DBG_INSIDE_LCLK_INTR_CALLBACK - PMON: inside pmon_lclk_intr_callback. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d001c:
        FW_LOG("PMON_DBG_RCP_DMA_CALLBACK - PMON: inside pmon_rcp_dma_callback. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d001d:
        FW_LOG("PMON_DBG_ENH_DDEG_STATE_CHANGE - PMON: enhanced ddeg channel=%d state=%d line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d001e:
        FW_LOG("PMON_DBG_ENH_DDEG_COUNTERS - PMON: enhanced ddeg nebc_val=%d tick_count=%d nebc_addr=0x%08x\n",e->log_word2,e->log_word3,e->log_word4 & 0x1FFFFFF);
        break;
    case 0x100d001f:
        FW_LOG("PMON_DBG_ENH_DDEG_LIMITS - PMON: enhanced ddeg ddeg_enh_mon_intvl_10ms_ticks=%d ddeg_enh_set_thres=%d ddeg_enh_clear_thres=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0020:
        FW_LOG("PMON_DBG_RCP - PMON: rcp debug replace_loss_of_signal_rcp_flag=0x%x, skip=0x%x, line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0023:
        FW_LOG("PMON_LOG_IPI_SET N=%d w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0024:
        FW_LOG("PMON_DBG_BURSTY_DDEG_STATE_CHANGE - PMON: bursty ddeg channel=%d state=%d line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0025:
        FW_LOG("PMON_DBG_BURSTY_DDEG_COUNTERS - PMON: bursty ddeg nebc_val=%d tick_count=%d nebc_addr=0x%08x\n",e->log_word2,e->log_word3,e->log_word4 & 0x1FFFFFF);
        break;
    case 0x100d0026:
        FW_LOG("PMON_DBG_BURSTY_DDEG_LIMITS - PMON: bursty ddeg ddeg_mon_intvl_10ms_ticks=%d ddeg_set_thres=%d ddeg_clear_thres=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040015:
        FW_LOG("GDMA_CHNL_STATE - GDMA: Current DMA channel state. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040001:
        FW_LOG("GDMA_LOG_CODE_DEBUG - GDMA: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1018000b:
        FW_LOG("OPSA_DBG_MPMO_TIME - waiting_time=%d timeout=%d line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1018000c:
        FW_LOG("OPSA_DBG_MPMO_STATE_CHANGE - channel_id=0x%08x prev_state=%d new_state=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1018000d:
        FW_LOG("OPSA_LOG_MPMO_ENABLE - OPSA: mpmo enable (ch<<24|acb_target<<16|acb_inst) channel_id=0x%0x subsystem=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1018000e:
        FW_LOG("OPSA_LOG_MPMO_DISABLE - OPSA: mpmo disable (ch<<24|acb_target<<16|acb_inst) channel_id=0x%0x subsystem=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1018000f:
        FW_LOG("OPSA_ERR_SERDES_RESOURCE_CONFLICT - OPSA: serdes resource conflict w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180010:
        FW_LOG("OPSA_LOG_MPMA_ENABLE - OPSA: mpma enable (ch<<24|qs_ch_en) channel_id=0x%0x subsystem=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180011:
        FW_LOG("OPSA_LOG_MPMA_DISABLE - OPSA: mpma disable (ch<<24|qs_ch_en) channel_id=0x%0x subsystem=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;

    case 0x101a0001:
        FW_LOG("BENCH_LOG_CODE_DEBUG - BENCH: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x101a0002:
        FW_LOG("BENCH_ERR_CODE_ASSERT - BENCH: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190001:
        FW_LOG("CBRC_LOG_CODE_DEBUG - CBRC: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190002:
        FW_LOG("CBRC_INIT_SUCCESS - CBRC: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190003:
        FW_LOG("CBRC_ERR_NUMBER_OUT_OF_RANGE - CBRC: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190004:
        FW_LOG("CBRC_ERR_INVALID_PARAMS - CBRC: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190005:
        FW_LOG("CBRC_ERR_GEN_FAIL - CBRC: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190006:
        FW_LOG("CBRC_ERR_REGISTER_FAIL - CBRC: Failed to register the unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190007:
        FW_LOG("CBRC_ERR_UNIT_TEST_FAILURE - CBRC: UNIT test failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190008:
        FW_LOG("CBRC_ERR_INIT_FAILURE - CBRC: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190009:
        FW_LOG("CBRC_ERR_NULL_POINTER - CBRC: Null pointer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1019000a:
        FW_LOG("CBRC_ERR_INTERRUPT - CBRC: Error interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1019000b:
        FW_LOG("CBRC_ERR_CONFIG - CBRC: Configuration error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1019000c:
        FW_LOG("CBRC_ERR_INVALID_CH_ID - CBRC: Invalid channel ID. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1019000d:
        FW_LOG("CBRC_ERR_ADD_PERIODIC_TASK - CBRC: Failed to add CBRC periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1019000e:
        FW_LOG("CBRC_ERR_INVALID_INT_MONITOR_STATUS - CBRC: Invalid combination of I-bit and V-bit. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1019000f:
        FW_LOG("CBRC_ERR_INVALID_INT_MONITOR_STATE - CBRC: Invalid interrupt monitoring state. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190010:
        FW_LOG("CBRC_ERR_INVALID_INT_MONITOR_ID - CBRC: Invalid interrupt monitoring ID. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10190011:
        FW_LOG("CBRC_ERR_INVALID_INT_MONITOR_LINK - CBRC: Invalid interrupt monitoring link. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170001:
        FW_LOG("CMF_FAIL_INSTALL_CALLBACK - CMF: Failed to install interrupt callback. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170002:
        FW_LOG("CMF_FAIL_ENABLE_AGGR_IRQ - CMF: Failed to enable aggr interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170003:
        FW_LOG("CMF_FAIL_INTR_UNKNOWN - CMF: got into callback function without interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170004:
        FW_LOG("CMF_FAIL_SEND_CMF - CMF: CMF was not sent successfully. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170005:
        FW_LOG("OPU_CSF_FAIL_INTR_UNKNOWN - CMF: OPU-CSF interrupt not detected. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170006:
        FW_LOG("CMF_ERR_NULL_POINTER - CMF: CMF null pointer w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170007:
        FW_LOG("CMF_DATA_LENGTH_MISMATCH - CMF: CMF data length mismatch w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170008:
        FW_LOG("CMF_MAPOTN_CH_OUT_OF_RANGE - CMF: MAPOTN channel provided is out of range w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170009:
        FW_LOG("CMF_CPU_DATA_AV_ERR - CMF: CPU FIFO data is not available w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1017000a:
        FW_LOG("CMF_CPU_DATA_EOF_ERR - CMF: CPU FIFO End of frame is not available w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1017000b:
        FW_LOG("CMF_CPU_DATA_SOF_ERR - CMF: CPU FIFO Start of frame is not available w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1017000c:
        FW_LOG("CMF_CPU_DATA_MSG_SIZE_ERR - CMF: CPU FIFO contains too large of a CMF w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1017000d:
        FW_LOG("CMF_ENET_LINK_INVALID_40G - CMF: enet link is invalid for the 40G mode. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1017000e:
        FW_LOG("CMP_OPU_CSF_ERR_ADD_PERIODIC_TASK - CMF: Failed to add OPU CSF peristancy periodic task w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1017000f:
        FW_LOG("CMF_MON_ALARM_PRD_TASK_ADD_ERR - CMF: Failed to add montitor alarm task w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170010:
        FW_LOG("CMF_INS_PRD_TASK_ADD_ERR - CMF: Failed to add insert periodic task w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170011:
        FW_LOG("CMF_ERR_INVALID_STATE_DEFECT_CHECK - CMF: Invalid state in defect_check_helper w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170012:
        FW_LOG("CMF_ENET_CHNL_OUT_OF_BOUNDS - CMF: ENET link channel is out of bounds w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170013:
        FW_LOG("CMF_LOSS_SYNC_ERR_ADD_PERIODIC_TASK - CMF: Failed to add FC1200 LOSS_SYNC peristancy periodic task w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170014:
        FW_LOG("CMF_CBRC_CH_OUT_OF_RANGE - CMF: CBRC channel provided is out of range w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170015:
        FW_LOG("CMF_ENET_EMAC_INVALID_STATE - CMF: Enet Emac Fallback state is invalid  w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10170016:
        FW_LOG("CMF_MAC_MAC_LR_RF_FORWARDING_ADD_PERIODIC_TASK - CMF: Failed to add mac to mac lf rf forwarding w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10000001:
        FW_LOG("COREOTN_FAIL_ADD_TEST - COREOTN: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10000002:
        FW_LOG("COREOTN_ERR_ADD_PERIODIC_TASK - COREOTN: Failed to add periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
        
    case 0x10000004:
        FW_LOG("COREOTN_Ohp_Monitor_CFC_FIFO_Full_TASK - COREOTN: OHP1 STUCK w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10000005:
        FW_LOG("COREOTN_Ohp_Monitor_CFC_FIFO_Full_TASK - COREOTN: OHP23 STUCK w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10000006:
        FW_LOG("COREOTN_Ohp_Monitor_CFC_FIFO_Full_TASK - COREOTN: OHP1 RESET. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10000007:
        FW_LOG("COREOTN_Ohp_Monitor_CFC_FIFO_Full_TASK - COREOTN: OHP23 RESET. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
        
    case 0x10010001:
        FW_LOG("DBG_LOG_INIT_SUCCESS - DBG_LOG: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010002:
        FW_LOG("DBG_LOG_DEFAULT_PARAM - DBG_LOG: Default parameters set. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010003:
        FW_LOG("DBG_LOG_INVALID_SEQ_NUM - DBG_LOG: Invalid sequence number. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010004:
        FW_LOG("DBG_LOG_INT_UNEXPECTED_INTERRUPT - DBG_LOG: Unexpected interrupt w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010005:
        FW_LOG("DBG_LOG_LOG_CODE_DEBUG - DBG_LOG: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010006:
        FW_LOG("DBG_LOG_INVALID_PARAMS - DBG_LOG: Invalid parameters w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010007:
        FW_LOG("DBG_LOG_ERR_CODE_ASSERT - DBG_LOG: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010008:
        FW_LOG("DBG_LOG_FAIL_MEM_ALLOC - DBG_LOG: Failed to allocate mem w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010009:
        FW_LOG("DBG_LOG_FAIL_MEM_FREE - DBG_LOG: Failed to free mem w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1001000a:
        FW_LOG("DBG_LOG_FAIL_MSG_SEND - DBG_LOG: dbg_msg_send failed w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1001000b:
        FW_LOG("DBG_LOG_FAIL_MSG_RECV - DBG_LOG: dbg_msg_recv failed w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1001000c:
        FW_LOG("DBG_LOG_FAIL_UNSUPPORTED_LOG_EVENT - DBG_LOG: Unsupported log event w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1001000d:
        FW_LOG("DBG_LOG_FAIL_VALUE_OUT_OF_RANGE - DBG_LOG: Value out of range w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1001000e:
        FW_LOG("DBG_LOG_FAIL_ADD_TEST - DBG_LOG: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1001000f:
        FW_LOG("DBG_LOG_FAIL_UNIT_TEST - DBG_LOG: One or more unit test failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10010010:
        FW_LOG("DBG_LOG_ERR_NULL_POINTER - DBG_LOG: Null pointer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020001:
        FW_LOG("DIGIHAL_LOG_CODE_DEBUG - DIGIHAL: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020002:
        FW_LOG("DIGIHAL_ERR_CODE_ASSERT - DIGIHAL: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020003:
        FW_LOG("DIGIHAL_ERR_CALLBACK - DIGIHAL: The callback should not be called. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020004:
        FW_LOG("DIGIHAL_ERR_NUMBER_OUT_OF_RANGE - DIGIHAL: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020005:
        FW_LOG("DIGIHAL_INIT_SUCCESS - DIGIHAL: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020006:
        FW_LOG("DIGIHAL_INVALID_PARAMS - DIGIHAL: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020007:
        FW_LOG("DIGIHAL_FAIL_GEN - DIGIHAL: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020008:
        FW_LOG("DIGIHAL_FAIL_ADD_TEST - DIGIHAL: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10020009:
        FW_LOG("DIGIHAL_FAIL_INIT - DIGIHAL: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1002000a:
        FW_LOG("DIGIHAL_RMW_VALUE_OUT_OF_MASK_RANGE - DIGIHAL: Supplied value takes up more bits than the mask. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030001:
        FW_LOG("ENET_LOG_CODE_DEBUG - ENET: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030002:
        FW_LOG("ENET_INIT_SUCCESS - ENET: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030003:
        FW_LOG("ENET_ERR_NUMBER_OUT_OF_RANGE - ENET: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030004:
        FW_LOG("ENET_ERR_INVALID_PARAMS - ENET: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030005:
        FW_LOG("ENET_ERR_GEN_FAIL - ENET: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030006:
        FW_LOG("ENET_ERR_REGISTER_FAIL - ENET: Failed to register the unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030007:
        FW_LOG("ENET_ERR_UNIT_TEST_FAILURE - ENET: UNIT test failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030008:
        FW_LOG("ENET_ERR_INIT_FAILURE - ENET: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030009:
        FW_LOG("ENET_ERR_NULL_POINTER - ENET: Null pointer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003000a:
        FW_LOG("ENET_ERR_INTERRUPT - ENET: Error interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003000b:
        FW_LOG("ENET_ERR_PKT_CLASSIFY - ENET: Failed to classify packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003000c:
        FW_LOG("ENET_ERR_PKT_SPACE - ENET: Not enough space for the packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003000d:
        FW_LOG("ENET_ERR_BAD_PKT - ENET: Bad packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003000e:
        FW_LOG("ENET_ERR_CONFIG - ENET: Configuration error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003000f:
        FW_LOG("ENET_ERR_PKT_EXTRACT_CHANNEL - ENET: Failed to identify the extract channel. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030010:
        FW_LOG("ENET_ERR_PKT_INSERT_CHANNEL - ENET: Failed to identify the insert channel. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030011:
        FW_LOG("ENET_ERR_PKT_BUFFER - ENET: No available packet buffers. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030012:
        FW_LOG("ENET_ERR_OSSP_PKT_WITHOUT_CALLBACK_RECEIVED - ENET: OSSP packet is received (and dropped). w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030013:
        FW_LOG("ENET_ERR_INVALID_CH_ID - ENET: Invalid channel ID. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030014:
        FW_LOG("ENET_ERR_ADD_PERIODIC_TASK - ENET: Failed to add ENET periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030015:
        FW_LOG("ENET_ERR_INVALID_RX_LOBL_STATUS - ENET: Invalid combination of RX_LOBL_I and RX_LOBL_V bits. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030016:
        FW_LOG("ENET_ERR_INVALID_INT_MONITOR_STATE - ENET: Invalid interrupt monitoring state. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030017:
        FW_LOG("ENET_ERR_INVALID_INT_MONITOR_ID - ENET: Invalid interrupt monitoring ID. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030018:
        FW_LOG("ENET_ERR_INVALID_INT_MONITOR_LANE - ENET: Invalid interrupt monitoring lane. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10030019:
        FW_LOG("ENET_ERR_INVALID_PKT_EXTRACT_QUEUE - ENET: Invalid packet extract queue w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1003001a:
        FW_LOG("ENET_ERR_INVALID_PKT_INSERT_QUEUE - ENET: Invalid packet insert queue w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040002:
        FW_LOG("GDMA_ERR_CODE_ASSERT - GDMA: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040003:
        FW_LOG("GDMA_INIT_SUCCESS - GDMA: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040004:
        FW_LOG("GDMA_FAIL_GEN - GDMA: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040005:
        FW_LOG("GDMA_FAIL_ADD_TEST - GDMA: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040006:
        FW_LOG("GDMA_FAIL_INIT - GDMA: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040007:
        FW_LOG("GDMA_NUMBER_OUT_OF_RANGE - GDMA: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040008:
        FW_LOG("GDMA_STATE_ERR - GDMA: DMA in wrong state w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040009:
        FW_LOG("GDMA_IRQ_STAT_ERR - GDMA: Unexpected interrupt w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004000a:
        FW_LOG("GDMA_FAIL_ENABLE_AGGR_IRQ - GDMA: Failed to enable aggr interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004000b:
        FW_LOG("GDMA_FAIL_ENABLE_MISC_IRQ - GDMA: Failed to enable misc interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004000c:
        FW_LOG("GDMA_FAIL_INSTALL_CALLBACK - GDMA: Failed to install interrupt callbacks. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004000d:
        FW_LOG("GDMA_INVALID_CHNL_NUM - GDMA: Invalid channel number. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004000e:
        FW_LOG("GDMA_INVALID_PROG_NUM - GDMA: Invalid program number. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004000f:
        FW_LOG("GDMA_INVALID_XFER_LEN - GDMA: Invalid transfer length. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040010:
        FW_LOG("GDMA_INVALID_CALLBACK - GDMA: Invalid callback function is called w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040011:
        FW_LOG("GDMA_ERR_MGR_ABORT - GDMA: DMA manager thread is in Faulting state w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040012:
        FW_LOG("GDMA_ERR_CHNL_ABORT - GDMA: DMA channel thread is in Faulting state w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040013:
        FW_LOG("GDMA_ERR_IRQ_ABORT - GDMA: Thread's in Faulting state and irq_abort sets w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040014:
        FW_LOG("GDMA_FAIL_STOP - GDMA: Failed to get in stopped state w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040016:
        FW_LOG("GDMA_ERR_NO_MICROCODE_DEFINED - GDMA: No microcode defined for channel w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040017:
        FW_LOG("GDMA_UNIT_TEST_LOG -  w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040018:
        FW_LOG("GDMA_UNIT_TEST_FAIL - w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10040019:
        FW_LOG("GDMA_APS_START - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004001a:
        FW_LOG("GDMA_APS_END - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004001b:
        FW_LOG("GDMA_APS_QUEUE - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1004001c:
        FW_LOG("GDMA_APS_HERE - w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060001:
        FW_LOG("HOSTMSG_LOG_CODE_DEBUG - HOSTMSG: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060002:
        FW_LOG("HOSTMSG_ERR_CODE_ASSERT - HOSTMSG: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060003:
        FW_LOG("HOSTMSG_INIT_SUCCESS - HOSTMSG: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060004:
        FW_LOG("HOSTMSG_INVALID_PARAMS - HOSTMSG: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060005:
        FW_LOG("HOSTMSG_FAIL_GEN - HOSTMSG: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060006:
        FW_LOG("HOSTMSG_FAIL_ADD_TEST - HOSTMSG: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060007:
        FW_LOG("HOSTMSG_FAIL_INIT - HOSTMSG: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060008:
        FW_LOG("HOSTMSG_FAIL_SEND_MSG_NACK - HOSTMSG: Failed to send NACK message to outbox. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10060009:
        FW_LOG("HOSTMSG_FAIL_PROCESS_HOSTMSG - HOSTMSG: Failed to process received host message. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070001:
        FW_LOG("INTR_LOG_CODE_DEBUG - INTR: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070002:
        FW_LOG("INTR_ERR_CODE_ASSERT - INTR: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070003:
        FW_LOG("INTR_ERR_CALLBACK - INTR: The callback should not be called. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070004:
        FW_LOG("INTR_ERR_NUMBER_OUT_OF_RANGE - INTR: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070005:
        FW_LOG("INTR_INIT_SUCCESS - INTR: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070006:
        FW_LOG("INTR_INVALID_PARAMS - INTR: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070007:
        FW_LOG("INTR_FAIL_GEN - INTR: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070008:
        FW_LOG("INTR_FAIL_ADD_TEST - INTR: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10070009:
        FW_LOG("INTR_FAIL_INIT - INTR: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1007000a:
        FW_LOG("INTR_LOG_IPI_SET - INTR: N=0x%0x value=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080001:
        FW_LOG("KERNEL_LOG_CODE_DEBUG - KERNEL: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080002:
        FW_LOG("KERNEL_ERR_CODE_ASSERT - KERNEL: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080003:
        FW_LOG("KERNEL_ERR_CALLBACK - KERNEL: The callback should not be called. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080004:
        FW_LOG("KERNEL_ERR_OUT_OF_RANGE - KERNEL: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080005:
        FW_LOG("KERNEL_INIT_SUCCESS - KERNEL: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080006:
        FW_LOG("KERNEL_INVALID_PARAMS - KERNEL: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080007:
        FW_LOG("KERNEL_FAIL_GEN - KERNEL: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080008:
        FW_LOG("KERNEL_FAIL_ADD_TEST - KERNEL: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10080009:
        FW_LOG("KERNEL_FAIL_INIT - KERNEL: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1008000a:
        FW_LOG("KERNEL_ERR_SHUTDOWN_ACK - KERNEL: Error sending ACK when shutting down firmware w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1008000b:
        FW_LOG("KERNEL_ERR_BG_TASK - KERNEL: Error in background task w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090001:
        FW_LOG("LED_MON_LOG_CODE_DEBUG - LED_MON: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090002:
        FW_LOG("LED_MON_ERR_CODE_ASSERT - LED_MON: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090003:
        FW_LOG("LED_MON_INVALID_CHNL_NUM - LED_MON: Invalid channel number. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090004:
        FW_LOG("LED_MON_ERR_ADD_PERIODIC_TASK - LED_MON: Failed to add periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090005:
        FW_LOG("LED_MON_ERR_NULL_PTR - LED_MON: Null pointer detected w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090006:
        FW_LOG("LED_MON_ERR_INVALID_STATUS - LED_MON: Channel status is invalid. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090007:
        FW_LOG("LED_MON_ERR_SPI_WRITE - LED_MON: SPI transfer failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090008:
        FW_LOG("LED_MON_ERR_FWREQ_POLL_TIMEOUT - LED_MON: FWREQ bit didn't get back to 0. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10090009:
        FW_LOG("LED_MON_INVALID_EN_TABLE - LED_MON: Enable table contains invalid data w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10150001:
        FW_LOG("LINEOTN_ERR_ADD_PERIODIC_TASK - LINEOTN: Failed to add periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0001:
        FW_LOG("M34KHAL_LOG_DEBUG - M34KHAL: Log debug w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0002:
        FW_LOG("M34KHAL_LOG_ERR_ASSERT - M34KHAL: Error assert w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0003:
        FW_LOG("M34KHAL_LOG_INIT_SUCCESS - M34KHAL: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0004:
        FW_LOG("M34KHAL_LOG_INVALID_PARAMS - M34KHAL: Invalid parameter w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0005:
        FW_LOG("M34KHAL_LOG_GENERAL_FAIL - M34KHAL: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0006:
        FW_LOG("M34KHAL_LOG_REGISTER_FAIL - M34KHAL: Unit test registration failed w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0007:
        FW_LOG("M34KHAL_LOG_INIT_FAIL - M34KHAL: Initialization failed w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0008:
        FW_LOG("M34KHAL_LOG_CACHE_WRITEBACK - M34KHAL: Cache Writeback w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b0009:
        FW_LOG("M34KHAL_LOG_CACHE_INVALIDATE - M34KHAL: Cache Invalidate w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b000a:
        FW_LOG("M34KHAL_LOG_CACHE_WRITEBACK_INVALIDATE - M34KHAL: Cache Writeback Invalidate w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b000b:
        FW_LOG("M34KHAL_LOG_ERR_ADDR_NOT_CACHELINE_ALIGNED - M34KHAL: Cache address line is not aligned w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100b000c:
        FW_LOG("M34KHAL_LOG_ERR_ADDR_INVALID - M34KHAL: Invalid address w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0001:
        FW_LOG("MT_LOG_CODE_DEBUG - MT: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0002:
        FW_LOG("MT_ERR_CODE_ASSERT - MT: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0003:
        FW_LOG("MT_INIT_SUCCESS - MT: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0004:
        FW_LOG("MT_INVALID_PARAMS - MT: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0005:
        FW_LOG("MT_FAIL_GEN - MT: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0006:
        FW_LOG("MT_FAIL_ADD_TEST - MT: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100c0007:
        FW_LOG("MT_FAIL_INIT - MT: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180001:
        FW_LOG("OPSA_ERR_ADD_PERIODIC_TASK - OPSA: Failed to add periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180002:
        FW_LOG("OPSA_ERR_INVALID_NUM_INT_REG - OPSA: Invalid number of interrupt registers. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180003:
        FW_LOG("OPSA_ERR_INVALID_SS - OPSA: Invalid subsystem. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180004:
        FW_LOG("OPSA_ERR_INVALID_ACB - OPSA: Invalid ACB instance. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180005:
        FW_LOG("OPSA_ERR_INVALID_ACB_CHANNEL - OPSA: Invalid ACB channel. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180006:
        FW_LOG("OPSA_ERR_INVALID_FIFO_DATA_TARGET - OPSA: Invalid data target for FIFO w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180007:
        FW_LOG("OPSA_ERR_INVALID_HANDLE - OPSA: Invalid handle w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180008:
        FW_LOG("OPSA_ERR_INVALID_MPMO_CHANNEL - OPSA: Invalid MPMO channel w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10180009:
        FW_LOG("OPSA_ERR_INVALID_MPMO_STATE - OPSA: Invalid MPMO state w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1018000a:
        FW_LOG("OPSA_ERR_INVALID_ARGUMENTS - OPSA: Invalid arguments w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160001:
        FW_LOG("PCIE_LOG_CODE_DEBUG - PCIE: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160002:
        FW_LOG("PCIE_ERR_CODE_ASSERT - PCIE: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160003:
        FW_LOG("PCIE_ERR_CALLBACK - PCIE: The callback should not be called. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160004:
        FW_LOG("PCIE_ERR_OUT_OF_RANGE - PCIE: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160005:
        FW_LOG("PCIE_INIT_SUCCESS - PCIE: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160006:
        FW_LOG("PCIE_INVALID_PARAMS - PCIE: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160007:
        FW_LOG("PCIE_FAIL_GEN - PCIE: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160008:
        FW_LOG("PCIE_FAIL_ADD_TEST - PCIE: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10160009:
        FW_LOG("PCIE_FAIL_INIT - PCIE: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x101b0001:
        FW_LOG("PERFORMANCE_BG_TASK_ENQUEUE_FAIL - PERFORMANCE: Failed enqueue performance background task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;      
    case 0x101b0002:
        FW_LOG("QUEUE_DBG_OPERATION - QUEUE: executing operation. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;      
    case 0x101b0003:
        FW_LOG("QUEUE_DBG_HERE - QUEUE: debug here. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;      
    case 0x101b0004:
        FW_LOG("QUEUE_DBG_XFER - QUEUE: xfer w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;      
    case 0x101b0005:
        FW_LOG("QUEUE_BAD_OPERATION - QUEUE: bad operation. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;      
    case 0x101b0006:
        FW_LOG("QUEUE_NO_OPERATION_IN_DMA - QUEUE: zero in a download segment. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;       
    case 0x101b0007:
        FW_LOG("QUEUE_DBG_READ - QUEUE: read debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;       
    case 0x101b0008:
        FW_LOG("QUEUE_DBG_GUARD_CORRUPT - w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;       
    case 0x101b0009:
        FW_LOG("QUEUE_DBG_INDEX - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;       
    case 0x101b000a:
        FW_LOG("QUEUE_DBG_DOWNLOAD - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;       
    case 0x101b000b:
        FW_LOG("QUEUE_DBG_SELF_TEST - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;       
    case 0x101b000c:
        FW_LOG("QUEUE_DBG_PROCESS - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;         
    case 0x101b000d:
        FW_LOG("QUEUE_DBG_PROCESS_ISR - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;      
    case 0x101b000e:
        FW_LOG("QUEUE_DBG_PROCESS_END - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;         
    case 0x101b000f:
        FW_LOG("QUEUE_DBG_PROCESS_ISR_END - w2=0x%0x w3=0x%0x w4=0x%0x\n",e->log_word2,e->log_word3,e->log_word4);
        break;         
    case 0x10140001:
        FW_LOG("PKT_CACHE_LOG_CODE_DEBUG - PKT_CACHE: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140002:
        FW_LOG("PKT_CACHE_INIT_SUCCESS - PKT_CACHE: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140003:
        FW_LOG("PKT_CACHE_ERR_NUMBER_OUT_OF_RANGE - PKT_CACHE: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140004:
        FW_LOG("PKT_CACHE_ERR_INVALID_PARAMS - PKT_CACHE: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140005:
        FW_LOG("PKT_CACHE_ERR_GEN_FAIL - PKT_CACHE: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140006:
        FW_LOG("PKT_CACHE_ERR_REGISTER_FAIL - PKT_CACHE: Failed to register the unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140007:
        FW_LOG("PKT_CACHE_ERR_UNIT_TEST_FAILURE - PKT_CACHE: UNIT test failuer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140008:
        FW_LOG("PKT_CACHE_ERR_INIT_FAILURE - PKT_CACHE: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140009:
        FW_LOG("PKT_CACHE_ERR_NULL_POINTER - PKT_CACHE: Null pointer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1014000a:
        FW_LOG("PKT_CACHE_ERR_INTERRUPT - PKT_CACHE: Error interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1014000b:
        FW_LOG("PKT_CACHE_ERR_PKT_CLASSIFY - PKT_CACHE: Failed to classify packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1014000c:
        FW_LOG("PKT_CACHE_ERR_PKT_SPACE - PKT_CACHE: Not enough space for the packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1014000d:
        FW_LOG("PKT_CACHE_ERR_BAD_PKT - PKT_CACHE: Bad packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1014000e:
        FW_LOG("PKT_CACHE_ERR_HP_QUEUE_WR_PTR_OVERFLOW - PKT_CACHE: HP Queue Write Pointer overflow. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1014000f:
        FW_LOG("PKT_CACHE_ERR_CONFIG - PKT_CACHE: Configuration error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140010:
        FW_LOG("PKT_CACHE_ERR_INSERT_BUFFER - PKT_CACHE: No insert buffer available. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10140011:
        FW_LOG("PKT_CACHE_LOG_PACKET_CACHE_WRITE - offset=%d size=%d queue=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0001:
        FW_LOG("PMON_FAIL_ADD_TEST - PMON: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0002:
        FW_LOG("PMON_FAIL_INSTALL_CALLBACK - PMON: Failed to install interrupt callback. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0003:
        FW_LOG("PMON_FAIL_ENABLE_AGGR_IRQ - PMON: Failed to enable aggr interrupt. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0004:
        FW_LOG("PMON_FAIL_BACKGROUND_TASK_ADD - PMON: Failed to add background task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0005:
        FW_LOG("PMON_FAIL_GDMA_UPDATE_MCODE - PMON: Failed to update GDMA micro-code. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0006:
        FW_LOG("PMON_FAIL_GDMA_XFER - PMON: Failed to tranfer data by DMA. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0007:
        FW_LOG("PMON_ERR_INVALID_BUFFER - PMON: Invalid PMON buffer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0008:
        FW_LOG("PMON_ERR_INVALID_IPI - PMON: Invalid IPI bit. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0009:
        FW_LOG("PMON_ERR_INVALID_TASK_STATE - PMON: Invalid PMON task state. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d000a:
        FW_LOG("PMON_ERR_DDEG_TASK_OVERRUN_LCLK - PMON: dDEG Task taking longer than LCLK interval. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d000b:
        FW_LOG("PMON_ERR_LINEOTN_FEC_TASK_OVERRUN_LCLK - PMON: PMON LINEOTN FEC Task taking longer than LCLK interval. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d000e:
        FW_LOG("PMON_ERR_INVALID_PMON_FEATURE - PMON: Invalid PMON feature. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d000f:
        FW_LOG("PMON_ERR_INVALID_LCLK_GEN - PMON: Invalid LCLK generator w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0010:
        FW_LOG("PMON_ERR_INVALID_LCLK_GEN_INTVL - PMON: Invalid LCLK generator interval w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0011:
        FW_LOG("PMON_ERR_RCP_DMA_FAIL - PMON: DMA Transfer for RCP failed w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0012:
        FW_LOG("PMON_ERR_INVALID_MANUAL_TRIG - PMON: Invalid manual trigger w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0013:
        FW_LOG("PMON_ERR_INVALID_RCP_EN - PMON: Invalid PMON rcp enable bit. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0014:
        FW_LOG("PMON_ERR_INVALID_BUFFER_PHY_ADDR - PMON: Invalid host buffer address. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0015:
        FW_LOG("PMON_ERR_INVALID_NUMBER_OF_BUFFER - PMON: Invalid number of buffer, only 1 is supported. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0016:
        FW_LOG("PMON_ERR_INVALID_HW_BLOCK - PMON: Invalid hardware block w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0017:
        FW_LOG("PMON_ERR_INVALID_DDEG_MODE - PMON: Invalid dDEG mode w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0018:
        FW_LOG("PMON_ERR_DDEG_ENH_INVALID_STATE - PMON: Invalid state in Enhanced dDEG state machine w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0019:
        FW_LOG("PMON_ERR_DDEG_INVALID_STATE - PMON: Invalid state in Bursty dDEG state machine w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d001a:
        FW_LOG("PMON_ERR_TIMEOUT_FW_ACCUM_DONE - PMON: timeout waiting for FEC ISR to set FW_ACCUM_DONE w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0021:
        FW_LOG("PMON_ERR_CBRC_TIP_TIMEOUT - PMON: cbrc tip timeout w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100d0022:
        FW_LOG("PMON_ERR_CBRC_BUSY_TIMEOUT - PMON: cbrc busy timeout w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0001:
        FW_LOG("PTP_LOG_CODE_DEBUG - PTP: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0002:
        FW_LOG("PTP_INIT_SUCCESS - PTP: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0003:
        FW_LOG("PTP_ERR_CODE_ASSERT - PTP: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0004:
        FW_LOG("PTP_ERR_CALLBACK - PTP: The callback should not be called. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0005:
        FW_LOG("PTP_ERR_NUMBER_OUT_OF_RANGE - PTP: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0006:
        FW_LOG("PTP_ERR_INVALID_PARAMS - PTP: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0007:
        FW_LOG("PTP_ERR_GEN_FAILURE - PTP: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0008:
        FW_LOG("PTP_ERR_UNIT_TEST_REGISTER_FAILURE - PTP: Failed to register the unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0009:
        FW_LOG("PTP_ERR_INIT_FAILURE - PTP: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e000a:
        FW_LOG("PTP_ERR_NULL_POINTER - PTP: Null pointer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e000b:
        FW_LOG("PTP_ERR_KERNEL_PRD - PTP: Kernel PRD task error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e000c:
        FW_LOG("PTP_ERR_INVALID_PORT - PTP: Invalid port. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e000d:
        FW_LOG("PTP_ERR_INVALID_PORT_STATE - PTP: Invalid port state. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e000e:
        FW_LOG("PTP_ERR_INVALID_DEVICE_TYPE - PTP: Invalid device type. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e000f:
        FW_LOG("PTP_ERR_UNSUPPORTED_MESSAGE_TYPE - PTP: Unsupported message type. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0010:
        FW_LOG("PTP_ERR_INVALID_CH_ID - PTP: Invalid channel ID. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0011:
        FW_LOG("PTP_ERR_INVALID_PKT_LENGTH - PTP: Invalid packet length. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0012:
        FW_LOG("PTP_ERR_INVALID_PKT_TYPE - PTP: Invalid packet type. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0013:
        FW_LOG("PTP_ERR_INVALID_QUEUE_TYPE - PTP: Invalid queue type. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0014:
        FW_LOG("PTP_ERR_UNSUPPORTED_NUM_SLAVE - PTP: Unsuported number of slaves. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0015:
        FW_LOG("PTP_ERR_INVALID_IRIG_MODE - PTP: Invalid IRIG mode. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0016:
        FW_LOG("PTP_ERR_LOW_POWER - PTP: Lower power mode. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0017:
        FW_LOG("PTP_ERR_INSERT_BUFFER - PTP: No insert buffer available. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0018:
        FW_LOG("PTP_ERR_TX_PKT_FAILURE - PTP: Failed to transmit a packet. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e0019:
        FW_LOG("PTP_ERR_RX_MSG_TYPE - PTP: Received invalid message type. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100e001a:
        FW_LOG("PTP_LOG_TOD_UPDATE - seconds64=%lld ns=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e001b:
        FW_LOG("PTP_LOG_SET_TSTMPR_OFFSET_BASE - seconds48=%lld ns=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e001c:
        FW_LOG("PTP_LOG_SET_TSTMPR_OFFSET_CFC - seconds48=%lld page=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e001d:
        FW_LOG("PTP VAR: delayAsymmetry val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e001e:
        FW_LOG("PTP VAR: correctionField val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e001f:
        FW_LOG("PTP VAR: phase_offset_seconds val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0020:
        FW_LOG("PTP VAR: phase_offset_ns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0021:
        FW_LOG("PTP VAR: phase_offset_sns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0022:
        FW_LOG("PTP VAR: adj_phase_offset_ns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0023:
        FW_LOG("PTP VAR: pdelayReqOrigin_t1_sns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0024:
        FW_LOG("PTP VAR: pdelayRespRcv_t4_sns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0025:
        FW_LOG("PTP VAR: pdelayReqRcv_t2_sns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0026:
        FW_LOG("PTP VAR: pdelayRespOrigin_t3_sns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0027:
        FW_LOG("PTP VAR: mean_path_dly_seconds val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0028:
        FW_LOG("PTP VAR: mean_path_dly_sns val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e0029:
        FW_LOG("PTP VAR: ptp_bc_currentDS.meanPathDelay val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100e002a:
        FW_LOG("PTP VAR: ptp_bc_currentDS.offsetFromMaster val64=%lld chan=%d\n",(((UINT64)e->log_word2) << 32 | e->log_word3),e->log_word4);
        break;
    case 0x100f0001:
        FW_LOG("SERDES_ERR_INVALID_DATA_RATE - SERDES: Invalid data rate. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100f0002:
        FW_LOG("SERDES_ERR_INVALID_INTERFACE - SERDES: Invalid interface. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100f0003:
        FW_LOG("SERDES_ERR_INVALID_CHANNEL - SERDES: Invalid channel. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100f0004:
        FW_LOG("SERDES_ERR_S16_IN_RESET - SERDES: S16 is in reset. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100f0005:
        FW_LOG("SERDES_LOG_ADAPT_RESULT - SERDES: Serdes adapt result. w2=0x%0x chnl=0x%0x result=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x101c0001:
        FW_LOG("STATS_LOG_CODE_DEBUG - STATS: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x101c0002:
        FW_LOG("STATS_ERR_CODE_ASSERT - STATS: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
        case 0x101c0003:
        {
            DOUBLE s_microseconds = e->log_word2 / 300.0; /* convert from mips counts to microseconds */
            char s_timestamp_buff[20];
            const char *stat_name;
            /* convert the timestamp to microseconds */
            pmc_sys_print_time(s_microseconds,1.0,s_timestamp_buff);
            stat_name = digi_fw_stat_name_get(digi_handle,e->log_word4);
            if (stat_name != NULL) {
                FW_LOG("STATS_LOG_STACK_SIZE - size=%d, dt=%s, %s\n",e->log_word3,s_timestamp_buff,stat_name);
            } else {
                FW_LOG("STATS_LOG_STACK_SIZE - size=%d, dt=%s, %d\n",e->log_word3,s_timestamp_buff,e->log_word4);
            }
        }
        break;
    case 0x10100001:
        FW_LOG("SYNCE_LOG_CODE_DEBUG - SYNCE: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100002:
        FW_LOG("SYNCE_ERR_CODE_ASSERT - SYNCE: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100003:
        FW_LOG("SYNCE_INVALID_CHNL_NUM - SYNCE: Invalid channel number. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100004:
        FW_LOG("SYNCE_ERR_ADD_PERIODIC_TASK - SYNCE: Failed to add periodic task. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100005:
        FW_LOG("SYNCE_ERR_INSERT_BUFFER - SYNCE: Failed to get insert buffer. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100006:
        FW_LOG("SYNCE_ERR_NULL_PTR - SYNCE: Null pointer detected w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100007:
        FW_LOG("SYNCE_INVALID_CH_ID - SYNCE: Received ESMC contains invalid channel id w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100008:
        FW_LOG("SYNCE_INVALID_CH_EN - SYNCE: Received ESMC channel is not enabled w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10100009:
        FW_LOG("SYNCE_INVALID_ITU_OUI - SYNCE: Received ESMC contains invalid itu_oui w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1010000a:
        FW_LOG("SYNCE_INVALID_ITU_SUBTYPE - SYNCE: Received ESMC contains invalid itu_subtype w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1010000b:
        FW_LOG("SYNCE_INVALID_VERSION - SYNCE: Received ESMC contains invalid version w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x1010000c:
        FW_LOG("SYNCE_ERR_CFG_ECLASS - SYNCE: Failed to configure ECLASS for acket extraction w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110001:
        FW_LOG("TIMER_LOG_CODE_DEBUG - TIMER: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110002:
        FW_LOG("TIMER_ERR_CODE_ASSERT - TIMER: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110003:
        FW_LOG("TIMER_ERR_CALLBACK - TIMER: The callback should not be called. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110004:
        FW_LOG("TIMER_ERR_OUT_OF_RANGE - TIMER: Number out of range error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110005:
        FW_LOG("TIMER_INIT_SUCCESS - TIMER: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110006:
        FW_LOG("TIMER_INVALID_PARAMS - TIMER: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110007:
        FW_LOG("TIMER_FAIL_GEN - TIMER: General failure w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110008:
        FW_LOG("TIMER_FAIL_ADD_TEST - TIMER: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10110009:
        FW_LOG("TIMER_FAIL_INIT - TIMER: Initialization failed. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120001:
        FW_LOG("UNIT_TEST_LOG_CODE_DEBUG - UNIT_TEST: Log debug. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120002:
        FW_LOG("UNIT_TEST_ERR_CODE_ASSERT - UNIT_TEST: Error assert. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120003:
        FW_LOG("UNIT_TEST_INIT_SUCCESS - UNIT_TEST: Initialization success. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120004:
        FW_LOG("UNIT_TEST_DEFAULT_PARAM - UNIT_TEST: Default parameters set. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120005:
        FW_LOG("UNIT_TEST_INVALID_SEQ_NUM - UNIT_TEST: Invalid sequence number. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120006:
        FW_LOG("UNIT_TEST_INVALID_PARAMS - UNIT_TEST: Invalid parameter. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120007:
        FW_LOG("UNIT_TEST_FAIL_ADD_TEST - UNIT_TEST: Failed to register unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120008:
        FW_LOG("UNIT_TEST_LOG_START - start test. id=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10120009:
        FW_LOG("UNIT_TEST_LOG_FINISH - finish test. id=0x%0x result=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10130001:
        FW_LOG("WDOG_ERR_GENERAL - WDOG: General error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10130002:
        FW_LOG("WDOG_ERR_COUNTER_LOAD - WDOG: Counter load error. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10130003:
        FW_LOG("WDOG_LOG_WATCHDOG_START - WDOG: Watchdog started. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10130004:
        FW_LOG("WDOG_LOG_WATCHDOG_STOP - WDOG: Watchdog stopped. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x10130005:
        FW_LOG("WDOG_ERR_FAIL_ADD_TEST - WDOG: Failed to add unit test. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;

    case 0x1006000a:
        {
            const char *mod_name;
            const char *sub_type;
            digi_fw_module_id(e->log_word2 >> 24,&mod_name,e->log_word2&0xff,&sub_type);
            FW_LOG("HOSTMSG_LOG_STARTED - %s %s started. w2=0x%0x w3=0x%0x line=%d\n",mod_name,sub_type,e->log_word2,e->log_word3,e->log_word4);
        }
        break;
    case 0x1006000b:
        FW_LOG("HOSTMSG_LOG_FINISHED - HOSTMSG: finished. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;
    case 0x100f0006:
        FW_LOG("SERDES_LOG_TRACE - SERDES: Serdes trace. w2=0x%0x w3=0x%0x line=%d\n",e->log_word2,e->log_word3,e->log_word4);
        break;


    case 0:
        if (e->seq_num != 0) {
            FW_LOG("lc=0x%0x w0=0x%0x w2=0x%0x w3=0x%0x line=%d\n",e->log_code,e->log_word0,e->log_word2,e->log_word3,e->log_word4);
        }
        break;
    default:
        FW_LOG("lc=0x%0x w0=0x%0x w2=0x%0x w3=0x%0x line=%d\n",e->log_code,e->log_word0,e->log_word2,e->log_word3,e->log_word4);
        break;
    }

    PMC_RETURN();
} /* of digi_fw_show_log_entry */

/*******************************************************************************
* digi_fw_show_debug_log
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware debug messages, but don't issue any firmware commands
*  in case firmware is not responsive.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void digi_fw_show_debug_log(digi_handle_t *digi_handle)
{
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    UINT32 app_log_wr_idx;
    UINT32 i;
    char *digi_name = digi_handle->base.tsb_name;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    UINT32 app_log_num_entries_max = digi_fw_stats->app_log_num_entries_max;
    pmc_sys_stat_t *cur_hostmsg_stat = digi_fw_stats->cur_hostmsg_stat;

    PMC_ENTRY();

    if (cur_hostmsg_stat != NULL) {
        FW_LOG("FW MESSAGE: %s %s%s\n",digi_name,cur_hostmsg_stat->description,cur_hostmsg_stat->description2);
    }
    if (digi_fw_stats->app_log_num_entries_max != 0) {
        dbg_log_app_entry_t *firmware_log;
        UINT32 min_seq_num = 0xffffffff;
        UINT32 min_timestamp = 0;
        UINT32 min_index = 0;
        UINT32 old_messages = 0;
        UINT32 max_seq_num = 0;
        UINT32 max_timestamp = 0;
        UINT32 write_offset;
        UINT32 remainder;

        /* FW_LOG("FW LOG: %s number of entries in debug log: %d\n",digi_name,app_log_num_entries_max); */

        app_log_wr_idx = pmc_sys_reg_read(sys_handle,digi_fw_stats->app_log_wr_idx_addr);

        firmware_log = (dbg_log_app_entry_t *)PMC_CALLOC(digi_fw_stats->app_log_num_entries_max*sizeof(dbg_log_app_entry_t));

        write_offset = app_log_wr_idx * sizeof(dbg_log_app_entry_t)/sizeof(UINT32);
        remainder = sizeof(dbg_log_app_entry_t)/sizeof(UINT32)*app_log_num_entries_max - write_offset;
        /* read in 2 sections to quickly read from the area currently being written,... and finishing with the oldest part of the buffer
         * this should help with getting mostly continuous data.
         * this should be solved by DMA'ing the buffer from digi to the host rather than reading it.
         */
        pmc_sys_burst_read(sys_handle,digi_fw_stats->app_log_ptr_addr+write_offset*sizeof(UINT32),remainder,&((UINT32*)firmware_log)[write_offset]);
        if (write_offset != 0) {
            pmc_sys_burst_read(sys_handle,digi_fw_stats->app_log_ptr_addr,write_offset,(UINT32*)firmware_log);
        }

        /* find the minimum sequence number,... sequence numbers are assumed to be monotonically increasing */
        for (i=0;i<app_log_num_entries_max;i++)
        {
            if (firmware_log[i].seq_num != 0) {
                if (firmware_log[i].seq_num < min_seq_num) {
                    min_seq_num = firmware_log[i].seq_num;
                    min_timestamp = firmware_log[i].timestamp;
                    min_index = i;
                }
                if (firmware_log[i].seq_num > max_seq_num) {
                    max_seq_num = firmware_log[i].seq_num;
                    max_timestamp = firmware_log[i].timestamp;
                }
            }
        }
        /* if the max sequence number is now smaller than the last sequence number,... then firmware has been reset */
        if (digi_fw_stats->app_log_last_sequence_number_displayed > max_seq_num) {
            digi_fw_stats->app_log_last_timestamp = 0;
            digi_fw_stats->app_log_last_sequence_number_displayed = 0;
        }
        for (i=0;i<app_log_num_entries_max;i++)
        {
            dbg_log_app_entry_t *e = &firmware_log[(i+min_index)%app_log_num_entries_max];
            if (e->seq_num != 0) {
                if (e->seq_num > digi_fw_stats->app_log_last_sequence_number_displayed) {
                    digi_fw_show_log_entry( digi_handle,e,digi_fw_stats->app_log_last_timestamp);
                    digi_fw_stats->app_log_last_timestamp = e->timestamp;
                } else {
                    old_messages++;
                }
            }
        }
        digi_fw_stats->app_log_last_sequence_number_displayed = max_seq_num;
        if (old_messages) {
            /*
            FW_LOG("FW LOG: %s old messages: %d\n",digi_name,old_messages);
            */
        }
        PMC_FREE(&firmware_log);
    }

    PMC_RETURN();
} /* digi_fw_show_debug_log */

/*******************************************************************************
* digi_fw_find_largest_sequence_id
* ___________________________________________________________________________
*
* DESCRIPTION:
*  returns the largest sequence id in the log file
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   
* OUTPUTS:
*    none
*
* RETURNS:
*    UINT32 - maximum sequence id
*
*******************************************************************************/
PRIVATE UINT32 digi_fw_find_largest_sequence_id(digi_handle_t *digi_handle)
{
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    UINT32 app_log_wr_idx;
    UINT32 i;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    UINT32 app_log_num_entries_max = digi_fw_stats->app_log_num_entries_max;
    UINT32 max_seq_num = 0;

    PMC_ENTRY();

    if (digi_fw_stats->app_log_num_entries_max != 0) {
        dbg_log_app_entry_t *firmware_log;
        UINT32 min_seq_num = 0xffffffff;
        UINT32 min_timestamp = 0;
        UINT32 min_index = 0;
        UINT32 max_timestamp = 0;

        /* FW_LOG("FW LOG: %s number of entries in debug log: %d\n",digi_name,app_log_num_entries_max); */

        app_log_wr_idx = pmc_sys_reg_read(sys_handle,digi_fw_stats->app_log_wr_idx_addr);

        firmware_log = (dbg_log_app_entry_t *)PMC_CALLOC(digi_fw_stats->app_log_num_entries_max*sizeof(dbg_log_app_entry_t));

        pmc_sys_burst_read(sys_handle,digi_fw_stats->app_log_ptr_addr,sizeof(dbg_log_app_entry_t)/sizeof(UINT32)*app_log_num_entries_max,(UINT32*)firmware_log);

        /* find the minimum sequence number,... sequence numbers are assumed to be monotonically increasing */
        for (i=0;i<app_log_num_entries_max;i++)
        {
            if (firmware_log[i].seq_num != 0) {
                if (firmware_log[i].seq_num < min_seq_num) {
                    min_seq_num = firmware_log[i].seq_num;
                    min_timestamp = firmware_log[i].timestamp;
                    min_index = i;
                }
                if (firmware_log[i].seq_num > max_seq_num) {
                    max_seq_num = firmware_log[i].seq_num;
                    max_timestamp = firmware_log[i].timestamp;
                }
            }
        }
        PMC_FREE(&firmware_log);
    }
    PMC_RETURN( max_seq_num );
} /* digi_fw_find_largest_sequence_id */


/*******************************************************************************
* digi_fw_show_mpmo_counts
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display mpmo debug counts.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void digi_fw_show_mpmo_counts(digi_handle_t *digi_handle)
{
    UINT32 i;
    char *digi_name = digi_handle->base.tsb_name;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    fw_opsa_mpmo_debug_t fw_opsa_mpmo_debug;
    const char *mpmo_state_names[7] = {
        "WAIT_FOR_SSF_GEN_INT_CLEAR",
        "WAIT_FOR_CLEAN_WINDOW",
        "WAIT_FOR_3TAU_CLEAN_WINDOW",
        "WAIT_FOR_TIME_WINDOW",
        "APPLY_ZONE_CENT_TRIGGER",
        "MONITOR_SSF_GEN_INT",
        "TRIGGER_CLEAR_DELAY/COMPLETE/BEGIN",
    };

    PMC_ENTRY();

    pmc_sys_burst_read(sys_handle,digi_fw_stats->opsa_mpmo_debug_addr,sizeof(fw_opsa_mpmo_debug_t)/sizeof(UINT32),(UINT32*)&fw_opsa_mpmo_debug);

#define SHOW_COUNTER_IF_DIFFERENT(counter) do { \
        if (fw_opsa_mpmo_debug.counter != digi_fw_stats->last_mpmo_debug.counter) { \
            FW_LOG("FW MPMO: %s %s = %d\n",digi_name,#counter,fw_opsa_mpmo_debug.counter); \
        } \
    } while (0)

#define SHOW_COUNTER_ARRAY(count_array) do { \
        for (i=0;i<7;i++) { \
            if (fw_opsa_mpmo_debug.count_array[i] != digi_fw_stats->last_mpmo_debug.count_array[i]) { \
                FW_LOG("FW MPMO: %s %s[%d \"%s\"] = %d\n",digi_name,#count_array,i,mpmo_state_names[i],fw_opsa_mpmo_debug.count_array[i]); \
            } \
        } \
    } while(0)

    SHOW_COUNTER_ARRAY(proc1_counts);
    SHOW_COUNTER_ARRAY(proc235_counts);
    SHOW_COUNTER_ARRAY(proc4_counts);
    SHOW_COUNTER_ARRAY(proc6_counts);

    SHOW_COUNTER_IF_DIFFERENT(proc6_ssf_gen_i_set_count);
    SHOW_COUNTER_IF_DIFFERENT(proc6_ssf_gen_i_clear_count);
    SHOW_COUNTER_IF_DIFFERENT(proc6_buf_underrun_i_set_count);
    SHOW_COUNTER_IF_DIFFERENT(proc6_buf_underrun_i_clear_count);

    /* save values so that next time only the changed values are displayed. */
    digi_fw_stats->last_mpmo_debug = fw_opsa_mpmo_debug;

    PMC_RETURN();

} /* digi_fw_show_mpmo_counts */

/*******************************************************************************
* digi_fw_number_of_stats_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  retrieve the number of firmware statistics
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   
* OUTPUTS:
*   none
*
*  RETURNS:
*    UINT32 - number of firmware statistics
*
*******************************************************************************/
PRIVATE UINT32 digi_fw_number_of_stats_get(digi_handle_t *digi_handle)
{
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    char *digi_name = digi_handle->base.tsb_name;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    UINT32 stats_number_of_stats;

    PMC_ENTRY();

    if (digi_fw_stats->stats_number_of_stats_addr == 0) {
        stats_number_of_stats = 0;
    } else {
        stats_number_of_stats = pmc_sys_reg_read(sys_handle,digi_fw_stats->stats_number_of_stats_addr);
        FW_LOG("FIRMWARE: %s number of statistics %d\n",digi_name,stats_number_of_stats);
    }

    PMC_RETURN(stats_number_of_stats);
} /* digi_fw_number_of_stats_get */


/*******************************************************************************
* digi_fw_stats_read
* ___________________________________________________________________________
*
* DESCRIPTION:
*  allocate space for the stastics buffer and read the statistics from firmware
*  updates the variables digi_fw_stats->stats_number_of_stats and 
*  digi_fw_stats->firmware_stats_buffer
*  note: reading the statistics is done without using the queue so that this can
*  be called from any thread without interfering with the firmware queue.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   
* OUTPUTS:
*   none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_stats_read(digi_handle_t *digi_handle )
{
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    UINT32 stats_number_of_stats;

    PMC_ENTRY();
    stats_number_of_stats = digi_fw_number_of_stats_get(digi_handle);

    /* reallocate the firmware_stats_buffer if the number of statistics has changed */
    if (digi_fw_stats->stats_number_of_stats != stats_number_of_stats) {
        digi_fw_stats->stats_number_of_stats = stats_number_of_stats;
        if (digi_fw_stats->firmware_stats_buffer != NULL) {
            PMC_FREE(&digi_fw_stats->firmware_stats_buffer);
            digi_fw_stats->firmware_stats_buffer = NULL;
        }
        if (digi_fw_stats->stats_number_of_stats > 0) {
            digi_fw_stats->firmware_stats_buffer = (pmc_sys_fw_stats_t *)PMC_CALLOC(digi_fw_stats->stats_number_of_stats*sizeof(pmc_sys_fw_stats_t));
        }
    }
    if (digi_fw_stats->stats_number_of_stats > 0) {
        pmc_sys_burst_read(sys_handle,digi_fw_stats->stats_list_of_statistics_addr,sizeof(pmc_sys_fw_stats_t)/sizeof(UINT32)*stats_number_of_stats,(UINT32*)digi_fw_stats->firmware_stats_buffer);
    }

    PMC_RETURN();
} /* digi_fw_stats_read */


/*******************************************************************************
* digi_fw_just_show_stats_internal
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware statistics, but don't issue any firmware commands
*  
* INPUTS:
*    digi_handle - DIGI device handle.
*   
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_just_show_stats_internal(digi_handle_t *digi_handle )
{
    UINT32 i;
    char *digi_name = digi_handle->base.tsb_name;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;

    PMC_ENTRY();
    digi_fw_stats_read(digi_handle);
    for (i=0;i<digi_fw_stats->stats_number_of_stats;i++)
    {
        pmc_sys_fw_stats_t *s = &digi_fw_stats->firmware_stats_buffer[i];
        if (s->num > 0 || s->mark_started != FALSE) {
            pmc_sys_fw_stat_show(s,digi_name);
        }
    }

    PMC_RETURN();
} /* digi_fw_just_show_stats_internal */

/*******************************************************************************
* digi_fw_show_stats_and_debug_log_internal
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware statistics, but don't issue any firmware commands
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_show_stats_and_debug_log_internal(digi_handle_t *digi_handle)
{
    UINT32 i;
    char *digi_name = digi_handle->base.tsb_name;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;

    PMC_ENTRY();

    digi_fw_show_assert_info(digi_handle);
    digi_fw_stats_read(digi_handle);
    for (i=0;i<digi_fw_stats->stats_number_of_stats;i++)
    {
        pmc_sys_fw_stats_t *s = &digi_fw_stats->firmware_stats_buffer[i];
        if (s->num > 0 || s->mark_started != FALSE) {
            pmc_sys_fw_stat_show(s,digi_name);
        }
    }
    digi_fw_show_debug_log(digi_handle);
    /* digi_fw_show_mpmo_counts(digi_handle); */

    PMC_RETURN();
} /* digi_fw_show_stats_and_debug_log_internal */


/*******************************************************************************
* digi_fw_stats_command_send
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Send a command to firmware to either flush the stats cache or reset the statistics.
*  The command is just a memory location that is polled periodically by firmware
*  Firmware acknowledges processing the command by writing the command location back to zero.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   cmd - command to send 1 = reset, 2 = flush.
*   description - description of command for debug messages.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PRIVATE void digi_fw_stats_command_send(digi_handle_t *digi_handle, UINT32 cmd, const char *description )
{
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;

    PMC_ENTRY();

    if (digi_fw_stats->stats_command_addr != 0) {
        PMC_SYS_STAT_DECLARE(fw_stat_command_delay,"fw_stat_command_delay");
        pmc_sys_stat_mark_start(fw_stat_command_delay);

        /* DIGI_FW_TRACE("waiting for: %s\n",description); */
        pmc_sys_reg_write(sys_handle,digi_fw_stats->stats_command_addr,cmd);
        PMC_OS_USLEEP(10);
        while (pmc_sys_stat_running_time(fw_stat_command_delay) < 1000*1000) {
            UINT32 command_reply = pmc_sys_reg_read(sys_handle,digi_fw_stats->stats_command_addr);
            if (command_reply == 0) {
                goto COMPLETE;
            }
            PMC_OS_USLEEP(1001);
        }
        FW_LOG("timeout waiting for: %s\n",description);
    COMPLETE:
        pmc_sys_stat_mark_end(fw_stat_command_delay);
    }

    PMC_RETURN();
}


/*******************************************************************************
* digi_fw_enable_firmware_stats
* ___________________________________________________________________________
*
* DESCRIPTION:
*  enable or disable the display of firmware stats when digi_shutdown is called
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   enable - TRUE to enable showing firmware stats in digi_shutdown, FALSE to disable.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void digi_fw_enable_firmware_stats(digi_handle_t *digi_handle, BOOL enable )
{
    PMC_ENTRY();

    /* check input */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

#ifdef DIGI_FW_STATS
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    digi_fw_stats->display_firmware_stats = enable;
#else
    digi_fw_stats = digi_fw_stats; /* UNUSED */
    enable = enable; /* UNUSED */
#endif
    PMC_RETURN();
}
#endif



/*******************************************************************************
* digi_fw_show_firmware_stats
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware statistics, but don't issue any firmware commands
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*  NOTE:                                                                                      
*
*    FIRMWARE: DIGI_1  assert cookie = 0xf00dfeed                                                                                                                           
*    FIRMWARE: DIGI_1  assert counter = 1                                                                                                                                   
*    FIRMWARE: DIGI_1  assert error_id = 268697609 (0x10040009)                                                                                                             
*    FIRMWARE: DIGI_1  assert line = 161                                                                                                                                    
*    FIRMWARE: DIGI_1  assert file = ../src/gdma/gdma.c                                                                                                                     
*    
*    FIRMWARE: DIGI_1 number of statistics 67
*    FIRMWARE: DIGI_1 KERN_PER_TASK duration: n=5999, total=116.0 milliseconds, min=3.623, max=144.3, avg=19.34 usec and one in progress 
*    FIRMWARE: DIGI_1 KERN_PER_TASK period  : n=5998, total=1.500 seconds, min=248.7, max=251.4, avg=250.0 usec                          
*    FIRMWARE: DIGI_1 KERN_PER_TASK percent : 7.736%                                                                                     
*    FIRMWARE: DIGI_1 KERN_STRT: 1.641 milliseconds                                                                                      
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN duration: n=2867, total=6.697 milliseconds, min=1.737, max=35.03, avg=2.336 usec                     
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN period  : n=2866, total=788.5 milliseconds, min=15.77, max=12574, avg=275.1 usec                     
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN percent : 0.8490%                                                                                    
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC duration: n=2867, total=18.66 milliseconds, min=2.007, max=834.5, avg=6.508 usec                      
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC period  : n=2866, total=788.5 milliseconds, min=15.73, max=12574, avg=275.1 usec                      
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC percent : 2.366%                                                                                      
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK duration: n=2867, total=19.64 milliseconds, min=2.357, max=834.9, avg=6.849 usec                    
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK period  : n=2866, total=788.5 milliseconds, min=15.72, max=12574, avg=275.1 usec                    
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK percent : 2.489%                                                                                    
*    FIRMWARE: DIGI_1 KERN_BG_TASK duration: n=944786, total=353.5 milliseconds, min=323.3, max=835763, avg=374.2 nanoseconds            
*    FIRMWARE: DIGI_1 KERN_BG_TASK period  : n=944785, total=1.500 seconds, min=1.113, max=838.0, avg=1.588 usec                         
*    FIRMWARE: DIGI_1 KERN_BG_TASK percent : 23.57%                                                                                      
*    FIRMWARE: DIGI_1 KERN_BG_POLL duration: n=944785, total=947.5 milliseconds, min=0.5967, max=160.8, avg=1.003 usec and one in progress 
*    FIRMWARE: DIGI_1 KERN_BG_POLL period  : n=944784, total=1.500 seconds, min=1.113, max=840.9, avg=1.588 usec                           
*    FIRMWARE: DIGI_1 KERN_BG_POLL percent : 63.17%                                                                                        
*    FIRMWARE: DIGI_1 WDOG_KICK duration: n=12, total=3.213 usec, min=210.0, max=383.3, avg=267.8 nanoseconds                              
*    FIRMWARE: DIGI_1 WDOG_KICK period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                          
*    FIRMWARE: DIGI_1 WDOG_KICK percent : 0.0002%                                                                                          
*    FIRMWARE: DIGI_1 LED_MON_TASK: 1.463 usec                                                                                             
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK duration: n=1499, total=4.803 milliseconds, min=2.900, max=4.167, avg=3.204 usec                    
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK percent : 0.3204%                                                                                   
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK duration: n=1499, total=574.4 usec, min=380.0, max=670.0, avg=383.2 nanoseconds                      
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                 
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK percent : 0.0383%                                                                                    
*    FIRMWARE: DIGI_1 CMF_FC800_TASK duration: n=1499, total=624.5 usec, min=396.7, max=746.7, avg=416.6 nanoseconds                       
*    FIRMWARE: DIGI_1 CMF_FC800_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                  
*    FIRMWARE: DIGI_1 CMF_FC800_TASK percent : 0.0417%                                                                                     
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK duration: n=119, total=85.05 usec, min=693.3, max=1137, avg=714.7 nanoseconds                        
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK period  : n=118, total=1.475 seconds, min=12.48, max=12.52, avg=12.50 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK percent : 0.0057%                                                                                    
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK duration: n=119, total=1.303 milliseconds, min=10.89, max=11.67, avg=10.95 usec                      
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK period  : n=118, total=1.475 seconds, min=12.48, max=12.52, avg=12.50 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK percent : 0.0876%                                                                                    
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK duration: n=749, total=7.192 milliseconds, min=2.880, max=56.34, avg=9.602 usec                      
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK period  : n=748, total=1.496 seconds, min=1.976, max=2.024, avg=2.000 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK percent : 0.4801%                                                                                    
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK duration: n=124, total=8.308 milliseconds, min=65.37, max=68.45, avg=67.00 usec                      
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK period  : n=123, total=1.476 seconds, min=11.98, max=12.02, avg=12.00 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK percent : 0.5583%                                                                                    
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK duration: n=5999, total=19.68 milliseconds, min=0.5967, max=14.94, avg=3.281 usec                     
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK period  : n=5998, total=1.500 seconds, min=109.4, max=390.6, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK percent : 1.313%                                                                                      
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK duration: n=5999, total=57.69 milliseconds, min=0.6067, max=15.63, avg=9.617 usec                     
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK period  : n=5998, total=1.500 seconds, min=109.4, max=390.6, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK percent : 3.847%                                                                                      
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK duration: n=16, total=9.657 usec, min=526.7, max=720.0, avg=603.5 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK percent : 0.0006%                                                                                  
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK duration: n=15, total=6.837 usec, min=400.0, max=570.0, avg=455.8 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK period  : n=14, total=1.400 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK percent : 0.0005%                                                                                  
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK duration: n=319, total=206.4 usec, min=560.0, max=1127, avg=647.0 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK period  : n=318, total=1.590 seconds, min=4.998, max=5.002, avg=5.000 milliseconds                 
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK percent : 0.0129%                                                                                  
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK duration: n=16, total=3.060 usec, min=140.0, max=286.7, avg=191.2 nanoseconds                       
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK percent : 0.0002%                                                                                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK duration: n=12, total=10.30 usec, min=643.3, max=1283, avg=858.1 nanoseconds                        
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK percent : 0.0007%                                                                                   
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK duration: n=16, total=3.383 usec, min=176.7, max=300.0, avg=211.5 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK percent : 0.0002%                                                                                  
*    FIRMWARE: DIGI_1 PTP_LOC_SEC_TASK: 230.0 nanoseconds                                                                                  
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK duration: n=15, total=16.79 usec, min=0.7467, max=2.327, avg=1.119 usec                             
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK period  : n=14, total=1.400 seconds, min=100.00, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK percent : 0.0011%                                                                                   
*    FIRMWARE: DIGI_1 SYNCE_LOST_C_TASK: 1.077 usec                                                                                        
*    FIRMWARE: DIGI_1 SYNCE_TX_TIM_TASK: 690.0 nanoseconds                                                                                 
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK duration: n=6000, total=1.506 milliseconds, min=233.3, max=1067, avg=251.0 nanoseconds                 
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK period  : n=5999, total=1.500 seconds, min=248.5, max=251.4, avg=250.0 usec                            
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK percent : 0.1004%                                                                                      
*    FIRMWARE: DIGI_1 STATS_CACHE_WB duration: n=12, total=39.30 usec, min=3.180, max=3.487, avg=3.275 usec and one in progress            
*    FIRMWARE: DIGI_1 STATS_CACHE_WB period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                     
*    FIRMWARE: DIGI_1 STATS_CACHE_WB percent : 0.0026%                                                                                     
*    FIRMWARE: DIGI_1 ENET_INTR_TASK duration: n=6000, total=1.171 milliseconds, min=193.3, max=606.7, avg=195.1 nanoseconds               
*    FIRMWARE: DIGI_1 ENET_INTR_TASK period  : n=5999, total=1.500 seconds, min=248.7, max=251.4, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 ENET_INTR_TASK percent : 0.0780%                                                                                     
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK duration: n=6000, total=1.162 milliseconds, min=190.0, max=293.3, avg=193.7 nanoseconds            
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK period  : n=5999, total=1.500 seconds, min=248.6, max=251.4, avg=250.0 usec                        
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK percent : 0.0775%                                                                                  
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI duration: n=500, total=79.99 usec, min=143.3, max=273.3, avg=160.0 nanoseconds                       
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI period  : n=499, total=1.497 seconds, min=2.999, max=3.001, avg=3.000 milliseconds                   
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI percent : 0.0053%                                                                                    
*    FIRMWARE: DIGI_1 SERDES_HMSG duration: n=2813, total=7.610 milliseconds, min=1.340, max=100.7, avg=2.705 usec                         
*    FIRMWARE: DIGI_1 SERDES_HMSG period  : n=2812, total=764.6 milliseconds, min=19.98, max=9717, avg=271.9 usec                          
*    FIRMWARE: DIGI_1 SERDES_HMSG percent : 0.9949%                                                                                        
*    FIRMWARE: DIGI_1 OPSA_HMSG duration: n=24, total=127.4 usec, min=1.550, max=28.79, avg=5.307 usec                                     
*    FIRMWARE: DIGI_1 OPSA_HMSG period  : n=23, total=319.6 milliseconds, min=0.0268, max=159.1, avg=13.90 milliseconds                    
*    FIRMWARE: DIGI_1 OPSA_HMSG percent : 0.0382%                                                                                          
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK duration: n=944785, total=499.6 milliseconds, min=163.3, max=160323, avg=528.8 nanoseconds             
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK period  : n=944784, total=1.500 seconds, min=1.113, max=841.0, avg=1.588 usec                          
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK percent : 33.31%                                                                                       
*    FIRMWARE: DIGI_1 QUEUE_DELAYS duration: n=916828, total=1.233 seconds, min=1.210, max=837.0, avg=1.345 usec and one in progress       
*    FIRMWARE: DIGI_1 QUEUE_DELAYS period  : n=916827, total=1.468 seconds, min=1.417, max=838.0, avg=1.601 usec                           
*    FIRMWARE: DIGI_1 QUEUE_DELAYS percent : 84.04%                                                                                        
*    FIRMWARE: DIGI_1 QUEUE_EXEC duration: n=11233, total=22.14 milliseconds, min=0.3067, max=159.8, avg=1.971 usec                        
*    FIRMWARE: DIGI_1 QUEUE_EXEC period  : n=11232, total=788.5 milliseconds, min=1.790, max=2003, avg=70.20 usec                          
*    FIRMWARE: DIGI_1 QUEUE_EXEC percent : 2.807%                                                                                          
*    FIRMWARE: DIGI_1 PMON_HOSTMSG duration: n=3, total=11.05 usec, min=3.493, max=4.037, avg=3.683 usec                                   
*    FIRMWARE: DIGI_1 PMON_HOSTMSG period  : n=2, total=189.6 milliseconds, min=94.76, max=94.82, avg=94.79 milliseconds                   
*    FIRMWARE: DIGI_1 PMON_HOSTMSG percent : 0.0039%                                                                                       
*     
*
*******************************************************************************/
PUBLIC void digi_fw_show_firmware_stats(digi_handle_t *digi_handle)
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

#ifdef DIGI_FW_STATS
    digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
    digi_fw_show_stats_and_debug_log_internal(digi_handle);
    digi_fw_stats_command_send(digi_handle,1,"firmware reset statistics");
#else
    digi_handle = digi_handle; /* UNUSED */
#endif

    PMC_ATOMIC_RETURN(digi_handle);
}


/*******************************************************************************
* digi_fw_just_show_firmware_stats
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Just Display firmware statistics, but don't issue any firmware commands
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*  NOTE:                                                                                                                                                               
*    
*    FIRMWARE: DIGI_1 number of statistics 67
*    FIRMWARE: DIGI_1 KERN_PER_TASK duration: n=5999, total=116.0 milliseconds, min=3.623, max=144.3, avg=19.34 usec and one in progress 
*    FIRMWARE: DIGI_1 KERN_PER_TASK period  : n=5998, total=1.500 seconds, min=248.7, max=251.4, avg=250.0 usec                          
*    FIRMWARE: DIGI_1 KERN_PER_TASK percent : 7.736%                                                                                     
*    FIRMWARE: DIGI_1 KERN_STRT: 1.641 milliseconds                                                                                      
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN duration: n=2867, total=6.697 milliseconds, min=1.737, max=35.03, avg=2.336 usec                     
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN period  : n=2866, total=788.5 milliseconds, min=15.77, max=12574, avg=275.1 usec                     
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN percent : 0.8490%                                                                                    
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC duration: n=2867, total=18.66 milliseconds, min=2.007, max=834.5, avg=6.508 usec                      
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC period  : n=2866, total=788.5 milliseconds, min=15.73, max=12574, avg=275.1 usec                      
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC percent : 2.366%                                                                                      
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK duration: n=2867, total=19.64 milliseconds, min=2.357, max=834.9, avg=6.849 usec                    
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK period  : n=2866, total=788.5 milliseconds, min=15.72, max=12574, avg=275.1 usec                    
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK percent : 2.489%                                                                                    
*    FIRMWARE: DIGI_1 KERN_BG_TASK duration: n=944786, total=353.5 milliseconds, min=323.3, max=835763, avg=374.2 nanoseconds            
*    FIRMWARE: DIGI_1 KERN_BG_TASK period  : n=944785, total=1.500 seconds, min=1.113, max=838.0, avg=1.588 usec                         
*    FIRMWARE: DIGI_1 KERN_BG_TASK percent : 23.57%                                                                                      
*    FIRMWARE: DIGI_1 KERN_BG_POLL duration: n=944785, total=947.5 milliseconds, min=0.5967, max=160.8, avg=1.003 usec and one in progress 
*    FIRMWARE: DIGI_1 KERN_BG_POLL period  : n=944784, total=1.500 seconds, min=1.113, max=840.9, avg=1.588 usec                           
*    FIRMWARE: DIGI_1 KERN_BG_POLL percent : 63.17%                                                                                        
*    FIRMWARE: DIGI_1 WDOG_KICK duration: n=12, total=3.213 usec, min=210.0, max=383.3, avg=267.8 nanoseconds                              
*    FIRMWARE: DIGI_1 WDOG_KICK period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                          
*    FIRMWARE: DIGI_1 WDOG_KICK percent : 0.0002%                                                                                          
*    FIRMWARE: DIGI_1 LED_MON_TASK: 1.463 usec                                                                                             
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK duration: n=1499, total=4.803 milliseconds, min=2.900, max=4.167, avg=3.204 usec                    
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK percent : 0.3204%                                                                                   
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK duration: n=1499, total=574.4 usec, min=380.0, max=670.0, avg=383.2 nanoseconds                      
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                 
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK percent : 0.0383%                                                                                    
*    FIRMWARE: DIGI_1 CMF_FC800_TASK duration: n=1499, total=624.5 usec, min=396.7, max=746.7, avg=416.6 nanoseconds                       
*    FIRMWARE: DIGI_1 CMF_FC800_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                  
*    FIRMWARE: DIGI_1 CMF_FC800_TASK percent : 0.0417%                                                                                     
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK duration: n=119, total=85.05 usec, min=693.3, max=1137, avg=714.7 nanoseconds                        
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK period  : n=118, total=1.475 seconds, min=12.48, max=12.52, avg=12.50 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK percent : 0.0057%                                                                                    
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK duration: n=119, total=1.303 milliseconds, min=10.89, max=11.67, avg=10.95 usec                      
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK period  : n=118, total=1.475 seconds, min=12.48, max=12.52, avg=12.50 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK percent : 0.0876%                                                                                    
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK duration: n=749, total=7.192 milliseconds, min=2.880, max=56.34, avg=9.602 usec                      
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK period  : n=748, total=1.496 seconds, min=1.976, max=2.024, avg=2.000 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK percent : 0.4801%                                                                                    
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK duration: n=124, total=8.308 milliseconds, min=65.37, max=68.45, avg=67.00 usec                      
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK period  : n=123, total=1.476 seconds, min=11.98, max=12.02, avg=12.00 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK percent : 0.5583%                                                                                    
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK duration: n=5999, total=19.68 milliseconds, min=0.5967, max=14.94, avg=3.281 usec                     
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK period  : n=5998, total=1.500 seconds, min=109.4, max=390.6, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK percent : 1.313%                                                                                      
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK duration: n=5999, total=57.69 milliseconds, min=0.6067, max=15.63, avg=9.617 usec                     
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK period  : n=5998, total=1.500 seconds, min=109.4, max=390.6, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK percent : 3.847%                                                                                      
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK duration: n=16, total=9.657 usec, min=526.7, max=720.0, avg=603.5 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK percent : 0.0006%                                                                                  
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK duration: n=15, total=6.837 usec, min=400.0, max=570.0, avg=455.8 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK period  : n=14, total=1.400 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK percent : 0.0005%                                                                                  
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK duration: n=319, total=206.4 usec, min=560.0, max=1127, avg=647.0 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK period  : n=318, total=1.590 seconds, min=4.998, max=5.002, avg=5.000 milliseconds                 
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK percent : 0.0129%                                                                                  
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK duration: n=16, total=3.060 usec, min=140.0, max=286.7, avg=191.2 nanoseconds                       
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK percent : 0.0002%                                                                                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK duration: n=12, total=10.30 usec, min=643.3, max=1283, avg=858.1 nanoseconds                        
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK percent : 0.0007%                                                                                   
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK duration: n=16, total=3.383 usec, min=176.7, max=300.0, avg=211.5 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK percent : 0.0002%                                                                                  
*    FIRMWARE: DIGI_1 PTP_LOC_SEC_TASK: 230.0 nanoseconds                                                                                  
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK duration: n=15, total=16.79 usec, min=0.7467, max=2.327, avg=1.119 usec                             
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK period  : n=14, total=1.400 seconds, min=100.00, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK percent : 0.0011%                                                                                   
*    FIRMWARE: DIGI_1 SYNCE_LOST_C_TASK: 1.077 usec                                                                                        
*    FIRMWARE: DIGI_1 SYNCE_TX_TIM_TASK: 690.0 nanoseconds                                                                                 
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK duration: n=6000, total=1.506 milliseconds, min=233.3, max=1067, avg=251.0 nanoseconds                 
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK period  : n=5999, total=1.500 seconds, min=248.5, max=251.4, avg=250.0 usec                            
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK percent : 0.1004%                                                                                      
*    FIRMWARE: DIGI_1 STATS_CACHE_WB duration: n=12, total=39.30 usec, min=3.180, max=3.487, avg=3.275 usec and one in progress            
*    FIRMWARE: DIGI_1 STATS_CACHE_WB period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                     
*    FIRMWARE: DIGI_1 STATS_CACHE_WB percent : 0.0026%                                                                                     
*    FIRMWARE: DIGI_1 ENET_INTR_TASK duration: n=6000, total=1.171 milliseconds, min=193.3, max=606.7, avg=195.1 nanoseconds               
*    FIRMWARE: DIGI_1 ENET_INTR_TASK period  : n=5999, total=1.500 seconds, min=248.7, max=251.4, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 ENET_INTR_TASK percent : 0.0780%                                                                                     
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK duration: n=6000, total=1.162 milliseconds, min=190.0, max=293.3, avg=193.7 nanoseconds            
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK period  : n=5999, total=1.500 seconds, min=248.6, max=251.4, avg=250.0 usec                        
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK percent : 0.0775%                                                                                  
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI duration: n=500, total=79.99 usec, min=143.3, max=273.3, avg=160.0 nanoseconds                       
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI period  : n=499, total=1.497 seconds, min=2.999, max=3.001, avg=3.000 milliseconds                   
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI percent : 0.0053%                                                                                    
*    FIRMWARE: DIGI_1 SERDES_HMSG duration: n=2813, total=7.610 milliseconds, min=1.340, max=100.7, avg=2.705 usec                         
*    FIRMWARE: DIGI_1 SERDES_HMSG period  : n=2812, total=764.6 milliseconds, min=19.98, max=9717, avg=271.9 usec                          
*    FIRMWARE: DIGI_1 SERDES_HMSG percent : 0.9949%                                                                                        
*    FIRMWARE: DIGI_1 OPSA_HMSG duration: n=24, total=127.4 usec, min=1.550, max=28.79, avg=5.307 usec                                     
*    FIRMWARE: DIGI_1 OPSA_HMSG period  : n=23, total=319.6 milliseconds, min=0.0268, max=159.1, avg=13.90 milliseconds                    
*    FIRMWARE: DIGI_1 OPSA_HMSG percent : 0.0382%                                                                                          
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK duration: n=944785, total=499.6 milliseconds, min=163.3, max=160323, avg=528.8 nanoseconds             
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK period  : n=944784, total=1.500 seconds, min=1.113, max=841.0, avg=1.588 usec                          
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK percent : 33.31%                                                                                       
*    FIRMWARE: DIGI_1 QUEUE_DELAYS duration: n=916828, total=1.233 seconds, min=1.210, max=837.0, avg=1.345 usec and one in progress       
*    FIRMWARE: DIGI_1 QUEUE_DELAYS period  : n=916827, total=1.468 seconds, min=1.417, max=838.0, avg=1.601 usec                           
*    FIRMWARE: DIGI_1 QUEUE_DELAYS percent : 84.04%                                                                                        
*    FIRMWARE: DIGI_1 QUEUE_EXEC duration: n=11233, total=22.14 milliseconds, min=0.3067, max=159.8, avg=1.971 usec                        
*    FIRMWARE: DIGI_1 QUEUE_EXEC period  : n=11232, total=788.5 milliseconds, min=1.790, max=2003, avg=70.20 usec                          
*    FIRMWARE: DIGI_1 QUEUE_EXEC percent : 2.807%                                                                                          
*    FIRMWARE: DIGI_1 PMON_HOSTMSG duration: n=3, total=11.05 usec, min=3.493, max=4.037, avg=3.683 usec                                   
*    FIRMWARE: DIGI_1 PMON_HOSTMSG period  : n=2, total=189.6 milliseconds, min=94.76, max=94.82, avg=94.79 milliseconds                   
*    FIRMWARE: DIGI_1 PMON_HOSTMSG percent : 0.0039%                                                                                       
*     
*
*******************************************************************************/
PUBLIC void digi_fw_just_show_firmware_stats(digi_handle_t *digi_handle)
{
    PMC_ENTRY();

    if (digi_fw_assert_tripped(digi_handle)) {
        digi_fw_show_assert_info(digi_handle);
    }
#ifdef DIGI_FW_STATS
    digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
    digi_fw_just_show_stats_internal(digi_handle);
    digi_fw_stats_command_send(digi_handle,1,"firmware reset statistics");
#endif

    PMC_RETURN();
} /* digi_fw_just_show_firmware_stats */

/*******************************************************************************
* digi_fw_show_firmware_errors
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display if firmware generated an assertion, and also display firmware statistics
*  if there was an assert.
*
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void digi_fw_show_firmware_errors(digi_handle_t *digi_handle )
{
    PMC_ENTRY();

#if defined(DIGI_FW_STATS) && (!defined(PMC_SW_SIMULATION))
    if (digi_fw_assert_tripped(digi_handle)) {
        digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
        digi_fw_show_stats_and_debug_log_internal(digi_handle);
        digi_fw_stats_command_send(digi_handle,1,"firmware reset statistics");
    }
#else
    digi_handle = digi_handle; /* UNUSED */
#endif

    PMC_RETURN();
} /* digi_fw_show_firmware_errors */


/*******************************************************************************
* digi_fw_poll_debug_log
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware statistics, but don't issue any firmware commands
*  also shows the last 2 firmware commands issued,
*  and shows a few interesting registers.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*  NOTE:
*
*******************************************************************************/
void digi_fw_poll_debug_log(digi_handle_t *digi_handle)
{
    PMC_ENTRY();

    digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
    digi_fw_show_debug_log(digi_handle);

    PMC_RETURN();
}


/*******************************************************************************
* digi_fw_wait_for_restart
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Poll the firmware looking for a lower sequence id.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*  NOTE:
*
*******************************************************************************/
void digi_fw_wait_for_restart(digi_handle_t *digi_handle)
{
    UINT32 max_seq_num;
    UINT32 last_seq_num;

    PMC_ENTRY();

    digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
    max_seq_num = digi_fw_find_largest_sequence_id(digi_handle);
    last_seq_num = max_seq_num;
    while (max_seq_num >= last_seq_num) 
    {
        last_seq_num = max_seq_num;
        digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
        max_seq_num = digi_fw_find_largest_sequence_id(digi_handle);
    }
    PMC_RETURN();
}

/*******************************************************************************
* digi_fw_show_queue_stall
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Display firmware statistics, but don't issue any firmware commands
*  also shows the last 2 firmware commands issued,
*  and shows a few interesting registers.
*  
* INPUTS:
*   digi_handle - DIGI device handle.
*   full - TRUE: display firmware statistics and interesting registers\n
*          FALSE: just display firmware log.
*
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*  NOTE:
*    addr=0x01809080 value=0x10000000 OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_0                                                                                           
*    addr=0x01809090 value=0x00000000 OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_0                                                                                           
*    addr=0x018090a0 value=0x02420000 OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_0                                                                                    
*    addr=0x018090b0 value=0x00000000 OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_0                                                                                    
*    addr=0x018090c0 value=0x0001f001 OUTBOUND_SIZE_MASK_LOWER_WINDOW_0                                                                                              
*    addr=0x018090d0 value=0x00000000 OUTBOUND_SIZE_MASK_UPPER_WINDOW_0                                                                                              
*    addr=0x01809084 value=0x12000000 OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_1                                                                                           
*    addr=0x01809094 value=0x00000000 OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_1                                                                                           
*    addr=0x018090a4 value=0x02400000 OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_1                                                                                    
*    addr=0x018090b4 value=0x00000000 OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_1                                                                                    
*    addr=0x018090c4 value=0x00000001 OUTBOUND_SIZE_MASK_LOWER_WINDOW_1                                                                                              
*    addr=0x018090d4 value=0x00000000 OUTBOUND_SIZE_MASK_UPPER_WINDOW_1                                                                                              
*    addr=0x01800188 value=0x9bd068ec APP_SCRATCH_1                                                                                                                  
*    addr=0x0180018c value=0x00000000 APP_SCRATCH_2 status                                                                                                           
*    addr=0x01800190 value=0x00000000 APP_SCRATCH_3 cause                                                                                                            
*    addr=0x01800194 value=0x00000000 APP_SCRATCH_4 EPC                                                                                                              
*    addr=0x01800198 value=0x00000000 APP_SCRATCH_5 return addr                                                                                                      
*    addr=0x0180019c value=0x00000000 APP_SCRATCH_6                                                                                                                  
*    addr=0x018001a0 value=0x5f5ff5f5 APP_SCRATCH_7                                                                                                                  
*    addr=0x018001a4 value=0x00000000 APP_USER0                                                                                                                      
*    addr=0x018001a8 value=0x00000083 APP_USER1                                                                                                                      
*    addr=0x01800000 value=0x00000000 SOFT_RESET_BITS                                                                                                                
*    addr=0x01dbfe00 value=0xaaaaaaaa IBND_MAILBOX[0]                                                                                                                
*    addr=0x01dbfe04 value=0xaaaaaaaa IBND_MAILBOX[1]                                                                                                                
*    addr=0x01dbfd00 value=0x0401000f OBND_MAILBOX[0]                                                                                                                
*    addr=0x01dbfd04 value=0x0b000b36 OBND_MAILBOX[1]                                                                                                                
*                                                                                                                                                                           
*    previous host message was: MODID_SERDES_HOSTMSG_CMD_CFG_SERDES_S16_WRITE_ADAPT_ST                                                                                      
*    current host message is: MODID_SERDES_HOSTMSG_CMD_CFG_SERDES_S16_WRITE_ADAPT_ST                                                                                        
*
*    FIRMWARE: DIGI_1  assert cookie = 0xf00dfeed                                                                                                                           
*    FIRMWARE: DIGI_1  assert counter = 1                                                                                                                                   
*    FIRMWARE: DIGI_1  assert error_id = 268697609 (0x10040009)                                                                                                             
*    FIRMWARE: DIGI_1  assert line = 161                                                                                                                                    
*    FIRMWARE: DIGI_1  assert file = ../src/gdma/gdma.c                                                                                                                     
*    
*    FIRMWARE: DIGI_1 number of statistics 67
*    FIRMWARE: DIGI_1 KERN_PER_TASK duration: n=5999, total=116.0 milliseconds, min=3.623, max=144.3, avg=19.34 usec and one in progress 
*    FIRMWARE: DIGI_1 KERN_PER_TASK period  : n=5998, total=1.500 seconds, min=248.7, max=251.4, avg=250.0 usec                          
*    FIRMWARE: DIGI_1 KERN_PER_TASK percent : 7.736%                                                                                     
*    FIRMWARE: DIGI_1 KERN_STRT: 1.641 milliseconds                                                                                      
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN duration: n=2867, total=6.697 milliseconds, min=1.737, max=35.03, avg=2.336 usec                     
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN period  : n=2866, total=788.5 milliseconds, min=15.77, max=12574, avg=275.1 usec                     
*    FIRMWARE: DIGI_1 HOSTMSG_BEGIN percent : 0.8490%                                                                                    
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC duration: n=2867, total=18.66 milliseconds, min=2.007, max=834.5, avg=6.508 usec                      
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC period  : n=2866, total=788.5 milliseconds, min=15.73, max=12574, avg=275.1 usec                      
*    FIRMWARE: DIGI_1 HOSTMSG_EXEC percent : 2.366%                                                                                      
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK duration: n=2867, total=19.64 milliseconds, min=2.357, max=834.9, avg=6.849 usec                    
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK period  : n=2866, total=788.5 milliseconds, min=15.72, max=12574, avg=275.1 usec                    
*    FIRMWARE: DIGI_1 HMSG_DISP_TASK percent : 2.489%                                                                                    
*    FIRMWARE: DIGI_1 KERN_BG_TASK duration: n=944786, total=353.5 milliseconds, min=323.3, max=835763, avg=374.2 nanoseconds            
*    FIRMWARE: DIGI_1 KERN_BG_TASK period  : n=944785, total=1.500 seconds, min=1.113, max=838.0, avg=1.588 usec                         
*    FIRMWARE: DIGI_1 KERN_BG_TASK percent : 23.57%                                                                                      
*    FIRMWARE: DIGI_1 KERN_BG_POLL duration: n=944785, total=947.5 milliseconds, min=0.5967, max=160.8, avg=1.003 usec and one in progress 
*    FIRMWARE: DIGI_1 KERN_BG_POLL period  : n=944784, total=1.500 seconds, min=1.113, max=840.9, avg=1.588 usec                           
*    FIRMWARE: DIGI_1 KERN_BG_POLL percent : 63.17%                                                                                        
*    FIRMWARE: DIGI_1 WDOG_KICK duration: n=12, total=3.213 usec, min=210.0, max=383.3, avg=267.8 nanoseconds                              
*    FIRMWARE: DIGI_1 WDOG_KICK period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                          
*    FIRMWARE: DIGI_1 WDOG_KICK percent : 0.0002%                                                                                          
*    FIRMWARE: DIGI_1 LED_MON_TASK: 1.463 usec                                                                                             
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK duration: n=1499, total=4.803 milliseconds, min=2.900, max=4.167, avg=3.204 usec                    
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                
*    FIRMWARE: DIGI_1 CMF_OPU_CSF_TASK percent : 0.3204%                                                                                   
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK duration: n=1499, total=574.4 usec, min=380.0, max=670.0, avg=383.2 nanoseconds                      
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                 
*    FIRMWARE: DIGI_1 CMF_FC1200_TASK percent : 0.0383%                                                                                    
*    FIRMWARE: DIGI_1 CMF_FC800_TASK duration: n=1499, total=624.5 usec, min=396.7, max=746.7, avg=416.6 nanoseconds                       
*    FIRMWARE: DIGI_1 CMF_FC800_TASK period  : n=1498, total=1.498 seconds, min=0.9881, max=1.012, avg=1.000 milliseconds                  
*    FIRMWARE: DIGI_1 CMF_FC800_TASK percent : 0.0417%                                                                                     
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK duration: n=119, total=85.05 usec, min=693.3, max=1137, avg=714.7 nanoseconds                        
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK period  : n=118, total=1.475 seconds, min=12.48, max=12.52, avg=12.50 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_C2T_DM_TASK percent : 0.0057%                                                                                    
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK duration: n=119, total=1.303 milliseconds, min=10.89, max=11.67, avg=10.95 usec                      
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK period  : n=118, total=1.475 seconds, min=12.48, max=12.52, avg=12.50 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_C2T_CA_TASK percent : 0.0876%                                                                                    
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK duration: n=749, total=7.192 milliseconds, min=2.880, max=56.34, avg=9.602 usec                      
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK period  : n=748, total=1.496 seconds, min=1.976, max=2.024, avg=2.000 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_T2C_DM_TASK percent : 0.4801%                                                                                    
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK duration: n=124, total=8.308 milliseconds, min=65.37, max=68.45, avg=67.00 usec                      
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK period  : n=123, total=1.476 seconds, min=11.98, max=12.02, avg=12.00 milliseconds                   
*    FIRMWARE: DIGI_1 CMF_T2C_CA_TASK percent : 0.5583%                                                                                    
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK duration: n=5999, total=19.68 milliseconds, min=0.5967, max=14.94, avg=3.281 usec                     
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK period  : n=5998, total=1.500 seconds, min=109.4, max=390.6, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 OPSA_MPMA_TASK percent : 1.313%                                                                                      
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK duration: n=5999, total=57.69 milliseconds, min=0.6067, max=15.63, avg=9.617 usec                     
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK period  : n=5998, total=1.500 seconds, min=109.4, max=390.6, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 OPSA_MPMO_TASK percent : 3.847%                                                                                      
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK duration: n=16, total=9.657 usec, min=526.7, max=720.0, avg=603.5 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_MST_QUAL_TASK percent : 0.0006%                                                                                  
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK duration: n=15, total=6.837 usec, min=400.0, max=570.0, avg=455.8 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK period  : n=14, total=1.400 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_ANNOUNCE_TASK percent : 0.0005%                                                                                  
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK duration: n=319, total=206.4 usec, min=560.0, max=1127, avg=647.0 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK period  : n=318, total=1.590 seconds, min=4.998, max=5.002, avg=5.000 milliseconds                 
*    FIRMWARE: DIGI_1 PTP_LOSSSYNC_TASK percent : 0.0129%                                                                                  
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK duration: n=16, total=3.060 usec, min=140.0, max=286.7, avg=191.2 nanoseconds                       
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_Q_TASK percent : 0.0002%                                                                                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK duration: n=12, total=10.30 usec, min=643.3, max=1283, avg=858.1 nanoseconds                        
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                   
*    FIRMWARE: DIGI_1 PTP_BC_AN_M_TASK percent : 0.0007%                                                                                   
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK duration: n=16, total=3.383 usec, min=176.7, max=300.0, avg=211.5 nanoseconds                      
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK period  : n=15, total=1.500 seconds, min=100.0, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_WAIT_NEW_TASK percent : 0.0002%                                                                                  
*    FIRMWARE: DIGI_1 PTP_LOC_SEC_TASK: 230.0 nanoseconds                                                                                  
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK duration: n=15, total=16.79 usec, min=0.7467, max=2.327, avg=1.119 usec                             
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK period  : n=14, total=1.400 seconds, min=100.00, max=100.0, avg=100.0 milliseconds                  
*    FIRMWARE: DIGI_1 PTP_UNICAST_TASK percent : 0.0011%                                                                                   
*    FIRMWARE: DIGI_1 SYNCE_LOST_C_TASK: 1.077 usec                                                                                        
*    FIRMWARE: DIGI_1 SYNCE_TX_TIM_TASK: 690.0 nanoseconds                                                                                 
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK duration: n=6000, total=1.506 milliseconds, min=233.3, max=1067, avg=251.0 nanoseconds                 
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK period  : n=5999, total=1.500 seconds, min=248.5, max=251.4, avg=250.0 usec                            
*    FIRMWARE: DIGI_1 CBRC_PRD_TASK percent : 0.1004%                                                                                      
*    FIRMWARE: DIGI_1 STATS_CACHE_WB duration: n=12, total=39.30 usec, min=3.180, max=3.487, avg=3.275 usec and one in progress            
*    FIRMWARE: DIGI_1 STATS_CACHE_WB period  : n=11, total=1.375 seconds, min=125.0, max=125.0, avg=125.0 milliseconds                     
*    FIRMWARE: DIGI_1 STATS_CACHE_WB percent : 0.0026%                                                                                     
*    FIRMWARE: DIGI_1 ENET_INTR_TASK duration: n=6000, total=1.171 milliseconds, min=193.3, max=606.7, avg=195.1 nanoseconds               
*    FIRMWARE: DIGI_1 ENET_INTR_TASK period  : n=5999, total=1.500 seconds, min=248.7, max=251.4, avg=250.0 usec                           
*    FIRMWARE: DIGI_1 ENET_INTR_TASK percent : 0.0780%                                                                                     
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK duration: n=6000, total=1.162 milliseconds, min=190.0, max=293.3, avg=193.7 nanoseconds            
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK period  : n=5999, total=1.500 seconds, min=248.6, max=251.4, avg=250.0 usec                        
*    FIRMWARE: DIGI_1 LNOTN_FRM_SL_TASK percent : 0.0775%                                                                                  
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI duration: n=500, total=79.99 usec, min=143.3, max=273.3, avg=160.0 nanoseconds                       
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI period  : n=499, total=1.497 seconds, min=2.999, max=3.001, avg=3.000 milliseconds                   
*    FIRMWARE: DIGI_1 COREOTN_DLOOMFI percent : 0.0053%                                                                                    
*    FIRMWARE: DIGI_1 SERDES_HMSG duration: n=2813, total=7.610 milliseconds, min=1.340, max=100.7, avg=2.705 usec                         
*    FIRMWARE: DIGI_1 SERDES_HMSG period  : n=2812, total=764.6 milliseconds, min=19.98, max=9717, avg=271.9 usec                          
*    FIRMWARE: DIGI_1 SERDES_HMSG percent : 0.9949%                                                                                        
*    FIRMWARE: DIGI_1 OPSA_HMSG duration: n=24, total=127.4 usec, min=1.550, max=28.79, avg=5.307 usec                                     
*    FIRMWARE: DIGI_1 OPSA_HMSG period  : n=23, total=319.6 milliseconds, min=0.0268, max=159.1, avg=13.90 milliseconds                    
*    FIRMWARE: DIGI_1 OPSA_HMSG percent : 0.0382%                                                                                          
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK duration: n=944785, total=499.6 milliseconds, min=163.3, max=160323, avg=528.8 nanoseconds             
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK period  : n=944784, total=1.500 seconds, min=1.113, max=841.0, avg=1.588 usec                          
*    FIRMWARE: DIGI_1 QUEUE_BG_TASK percent : 33.31%                                                                                       
*    FIRMWARE: DIGI_1 QUEUE_DELAYS duration: n=916828, total=1.233 seconds, min=1.210, max=837.0, avg=1.345 usec and one in progress       
*    FIRMWARE: DIGI_1 QUEUE_DELAYS period  : n=916827, total=1.468 seconds, min=1.417, max=838.0, avg=1.601 usec                           
*    FIRMWARE: DIGI_1 QUEUE_DELAYS percent : 84.04%                                                                                        
*    FIRMWARE: DIGI_1 QUEUE_EXEC duration: n=11233, total=22.14 milliseconds, min=0.3067, max=159.8, avg=1.971 usec                        
*    FIRMWARE: DIGI_1 QUEUE_EXEC period  : n=11232, total=788.5 milliseconds, min=1.790, max=2003, avg=70.20 usec                          
*    FIRMWARE: DIGI_1 QUEUE_EXEC percent : 2.807%                                                                                          
*    FIRMWARE: DIGI_1 PMON_HOSTMSG duration: n=3, total=11.05 usec, min=3.493, max=4.037, avg=3.683 usec                                   
*    FIRMWARE: DIGI_1 PMON_HOSTMSG period  : n=2, total=189.6 milliseconds, min=94.76, max=94.82, avg=94.79 milliseconds                   
*    FIRMWARE: DIGI_1 PMON_HOSTMSG percent : 0.0039%                                                                                       
*     
*
*******************************************************************************/
PUBLIC void digi_fw_show_queue_stall(digi_handle_t *digi_handle,BOOL8 full)
{
    hostmsg_handle_t *hostmsg_handle = (hostmsg_handle_t *)digi_handle->base.common_info->hostmsg_handle;
    digi_fw_stats_t *digi_fw_stats = &digi_handle->var.digi_pmon_mode_profile->digi_fw_stats;
    pmc_sys_handle_t *sys_handle = digi_handle->base.sys_handle;

    PMC_ENTRY();

    if (hostmsg_handle != NULL) {
        if (full) {
    #define PRINT_REG(addr,desc) FW_LOG("addr=0x%08x value=0x%08x %s \n",addr,pmc_sys_reg_read(sys_handle,addr),desc)
            PRINT_REG(0x1809080,"OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_0");
            PRINT_REG(0x1809090,"OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_0");
            PRINT_REG(0x18090a0,"OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_0");
            PRINT_REG(0x18090b0,"OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_0");
            PRINT_REG(0x18090c0,"OUTBOUND_SIZE_MASK_LOWER_WINDOW_0");
            PRINT_REG(0x18090d0,"OUTBOUND_SIZE_MASK_UPPER_WINDOW_0");
            PRINT_REG(0x1809084,"OUTBOUND_AXI_BASE_LOWER_ADR_WINDOW_1");
            PRINT_REG(0x1809094,"OUTBOUND_AXI_BASE_UPPER_ADR_WINDOW_1");
            PRINT_REG(0x18090a4,"OUTBOUND_PCI_TRANSLATION_LOWER_ADR_WINDOW_1");
            PRINT_REG(0x18090b4,"OUTBOUND_PCI_TRANSLATION_UPPER_ADR_WINDOW_1");
            PRINT_REG(0x18090c4,"OUTBOUND_SIZE_MASK_LOWER_WINDOW_1");
            PRINT_REG(0x18090d4,"OUTBOUND_SIZE_MASK_UPPER_WINDOW_1");
            PRINT_REG(0x1800188,"APP_SCRATCH_1");
            PRINT_REG(0x180018c,"APP_SCRATCH_2 status");
            PRINT_REG(0x1800190,"APP_SCRATCH_3 cause");
            PRINT_REG(0x1800194,"APP_SCRATCH_4 EPC");
            PRINT_REG(0x1800198,"APP_SCRATCH_5 return addr");
            PRINT_REG(0x180019c,"APP_SCRATCH_6");
            PRINT_REG(0x18001a0,"APP_SCRATCH_7");
            PRINT_REG(0x18001A4,"APP_USER0");
            PRINT_REG(0x18001A8,"APP_USER1");
            PRINT_REG(0x1800000,"SOFT_RESET_BITS");
            PRINT_REG(0x1dbFE00,"IBND_MAILBOX[0]");
            PRINT_REG(0x1dbFE04,"IBND_MAILBOX[1]");
            PRINT_REG(0x1dbFD00,"OBND_MAILBOX[0]");
            PRINT_REG(0x1dbFD04,"OBND_MAILBOX[1]");
            PRINT_REG(0x180b020,"Interrupt Enable Register: INTEN");
            PRINT_REG(0x180b028,"Interrupt Status Register: INTMIS");
            FW_LOG("\n");

            if (digi_fw_stats->prev_hostmsg_stat != NULL) {
                FW_LOG("previous host message was: %s%s\n",digi_fw_stats->prev_hostmsg_stat->description,digi_fw_stats->prev_hostmsg_stat->description2);
            }
            if (digi_fw_stats->cur_hostmsg_stat != NULL) {
                FW_LOG("current host message is: %s%s\n",digi_fw_stats->cur_hostmsg_stat->description,digi_fw_stats->cur_hostmsg_stat->description2);
            }
    #ifdef DIGI_FW_STATS
            digi_fw_stats_command_send(digi_handle,2,"firmware flush statistics");
            digi_fw_show_stats_and_debug_log_internal(digi_handle);
            digi_fw_stats_command_send(digi_handle,1,"firmware reset statistics");
    #endif
        }
        else
        {
            digi_fw_show_debug_log(digi_handle);
        }
    }

    PMC_RETURN();
} /* digi_fw_show_queue_stall */



/*******************************************************************************
* digi_fw_logging_enable
* ___________________________________________________________________________
*
* DESCRIPTION:
*   should the firmware log be displayed after each firmware message
*   and should firmware statistics be displayed during digi_shutdown()
*
* INPUTS:
*   enable - TRUE - enable, FALSE - disable
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
PUBLIC void digi_fw_logging_enable(BOOL8 enable)
{
    PMC_ENTRY();
    digi_fw_firmware_logging_enabled = enable;
    PMC_RETURN();
} /* digi_fw_logging_enable */


/* ignore this section for swig */
#ifndef PMC_DOX_FOR_SWIG

typedef struct block_definition_t {
    const char *name;
    struct block_definition_t *mtsb;
    UINT32 base_addr;
    UINT32 size;
    UINT32 range_A;
    UINT32 offset_A;
    UINT32 range_B;
    UINT32 offset_B;
    UINT32 range_C;
    UINT32 offset_C;
    BOOL8 initialized;       /*!< has registers[] been allocated? */
    BOOL8 index_initialized; /*!< has block_index been initialized? */
    UINT32 block_index;
    UINT32 num_registers;
    struct register_definition_t **registers;
    pmc_sys_dstat_t num_burst_writes[1];
    pmc_sys_dstat_t num_writes[1];
    pmc_sys_dstat_t num_reads[1];
    pmc_sys_dstat_t num_polls[1];
    pmc_sys_dstat_t num_short_rmw[1];
    pmc_sys_dstat_t num_long_rmw[1];
    pmc_sys_dstat_t num_sleeps[1];
    UINT32 sleep_time;
} block_definition_t;

static double fw_speed = 0;
void digi_fw_speed(double microseconds)
{
    fw_speed = microseconds;
}

typedef struct register_definition_t {
    block_definition_t *block;
    const char *name;
    const char *fields;
    UINT32 addr;
    UINT32 range_N;
    UINT32 offset_N;
    UINT32 range_M;
    UINT32 offset_M;
    UINT32 range_channel; /* should fix the RDA,... but for now,... adding a range_channel/offset_channel */
    UINT32 offset_channel;
} register_definition_t;

#include "registers.h"
/* this defines the array register_definition_t CHANNELS[] */
static block_definition_t **BLOCKS = NULL;
static UINT32 num_blocks = 0;
PMC_SYS_DSTAT_DECLARE( num_fw_queries, "fw_queries" );
PMC_SYS_DSTAT_DECLARE( fast_fw_cmds, "fast_fw_cmds" );
PMC_SYS_DSTAT_DECLARE( slow_fw_cmds, "slow_fw_cmds" );

/*******************************************************************************
* find_first_bit_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the number of the first bit set in the mask
*
* INPUTS:
*   mask - the mask
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The position of the first bit set.
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 find_first_bit_set(UINT32 mask) UNUSED;
static INLINE UINT32 find_first_bit_set(UINT32 mask)
{
    UINT32 value = 0;

    if ((mask & 0xFFFF) == 0)
    {
        value+= 16;
        mask >>= 16;
    }
    if ((mask & 0xFF) == 0)
    {
        value += 8;
        mask >>= 8;
    }
    if ((mask & 0xF) == 0)
    {
        value += 4;
        mask >>= 4;
    }
    if ((mask & 0x3) == 0)
    {
        value += 2;
        mask >>= 2;
    }
    if ((mask & 0x1) == 0)
    {
        value += 1;
    }
    return value;
}
/*******************************************************************************
* find_last_bit_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the number of the last bit set in the mask
*
* INPUTS:
*   mask - the mask
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The position of the last bit set.
*
* NOTES:
*******************************************************************************/
PRIVATE INLINE UINT32 find_last_bit_set(UINT32 mask) UNUSED;
PRIVATE INLINE UINT32 find_last_bit_set(UINT32 mask)
{
    UINT32 value = 0;

    PMC_ENTRY();
    if ((mask & 0xFFFF0000) != 0)
    {
        value+= 16;
        mask >>= 16;
    }
    if ((mask & 0xFF00) != 0)
    {
        value += 8;
        mask >>= 8;
    }
    if ((mask & 0xF0) != 0)
    {
        value += 4;
        mask >>= 4;
    }
    if ((mask & 0xc) != 0)
    {
        value += 2;
        mask >>= 2;
    }
    if ((mask & 0x2) != 0)
    {
        value += 1;
    }
    PMC_RETURN(value);
}

void init_registers()
{
    UINT32 i;
    UINT32 num_registers = sizeof(REGISTERS)/sizeof(register_definition_t);

    PMC_ENTRY();
    for (i=0;i<num_registers;i++) {
        register_definition_t *reg = &(REGISTERS[i]);
        block_definition_t *block = reg->block;
        block_definition_t *mtsb = block->mtsb;
        if (!block->index_initialized) {
            block->index_initialized = TRUE;
            block->initialized = FALSE;
            block->block_index = num_blocks;
            block->size = 0;
            num_blocks++;
        }
        if (mtsb != NULL && !mtsb->index_initialized) {
            mtsb->index_initialized = TRUE;
            mtsb->initialized = FALSE;
            mtsb->block_index = num_blocks;
            mtsb->size = 0;
            num_blocks++;
        }
        block->num_registers++;
        {
            UINT32 max_addr = reg->addr + reg->offset_N * reg->range_N + reg->offset_M * reg->range_M;
            if (max_addr - block->base_addr > block->size) {
                block->size = max_addr - block->base_addr;
            }
        }
    }
    BLOCKS = (block_definition_t **)PMC_MALLOC(num_blocks * sizeof(block_definition_t*));
    for (i=0;i<num_registers;i++) {
        register_definition_t *reg = &(REGISTERS[i]);
        block_definition_t *block = reg->block;
        block_definition_t *mtsb = block->mtsb;
        if (!block->initialized) {
            block->initialized = TRUE;
            block->registers = (register_definition_t **)PMC_MALLOC(sizeof(register_definition_t*)*block->num_registers);
            BLOCKS[block->block_index] = block;
            block->num_registers = 0;

            block->num_burst_writes->description = "burst_wr "; 
            block->num_writes->description       = "writes   "; 
            block->num_reads->description        = "reads    "; 
            block->num_polls->description        = "polls    "; 
            block->num_short_rmw->description    = "short_rmw"; 
            block->num_long_rmw->description     = "long_rmw "; 
            block->num_sleeps->description       = "sleeps   "; 


            block->num_burst_writes->description2 = "";
            block->num_writes->description2       = "";
            block->num_reads->description2        = "";
            block->num_polls->description2        = "";
            block->num_short_rmw->description2    = "";
            block->num_long_rmw->description2     = "";
            block->num_sleeps->description2       = "";

        }
        if (mtsb != NULL && !mtsb->initialized) {
            mtsb->initialized = TRUE;
            if (mtsb->num_registers > 0) {
                mtsb->registers = (register_definition_t **)PMC_MALLOC(sizeof(register_definition_t*)*mtsb->num_registers);
            }
            BLOCKS[mtsb->block_index] = mtsb;
            mtsb->num_registers = 0;

            mtsb->num_burst_writes->description = "burst_wr ";
            mtsb->num_writes->description       = "writes   ";
            mtsb->num_reads->description        = "reads    ";
            mtsb->num_polls->description        = "polls    ";
            mtsb->num_short_rmw->description    = "short_rmw";
            mtsb->num_long_rmw->description     = "long_rmw ";
            mtsb->num_sleeps->description       = "sleeps   ";


            mtsb->num_burst_writes->description2 = "";
            mtsb->num_writes->description2       = "";
            mtsb->num_reads->description2        = "";
            mtsb->num_polls->description2        = "";
            mtsb->num_short_rmw->description2    = "";
            mtsb->num_long_rmw->description2     = "";
            mtsb->num_sleeps->description2       = "";
        }
        block->registers[block->num_registers] = reg;
        block->num_registers++;
    }
    PMC_RETURN();
}


block_definition_t *find_block( UINT32 addr, char *block_index, UINT32 *block_offset )
{
    UINT32 i;

    PMC_ENTRY();
    if (BLOCKS == NULL) {
        init_registers();
    }
    for (i=0;i<num_blocks;i++) {
        block_definition_t *block = BLOCKS[i];
        UINT32 A,B,C;
        UINT32 range_A = block->range_A;
        UINT32 range_B = block->range_B;
        UINT32 range_C = block->range_C;
        for (A=0;A<=range_A;A++) {
            for (B=0;B<=range_B;B++) {
                for (C=0;C<=range_C;C++) {
                    UINT32 offset = A * block->offset_A + B * block->offset_B + C * block->offset_C;
                    UINT32 start_addr = block->base_addr + offset; /* start address of this instance of the block with index A,B,C */
                    UINT32 end_addr   = start_addr + block->size;         /* end address of this instance... */
                    if ((addr >= start_addr) && (addr <= end_addr)) {
                        PMC_STRCPY(block_index,"");
                        if (range_A > 0) {
                          PMC_SPRINTF(&(block_index[strlen(block_index)])," A=%d",A);
                        }
                        if (range_B > 0) {
                          PMC_SPRINTF(&(block_index[strlen(block_index)])," B=%d",B);
                        }
                        if (range_C > 0) {
                          PMC_SPRINTF(&(block_index[strlen(block_index)])," C=%d",C);
                        }
                        *block_offset = offset;
                        PMC_RETURN( block );
                    }
                }
            }
        }
    }
    PMC_RETURN( NULL );
}


register_definition_t *find_register( UINT32 addr, char *reg_index, block_definition_t *block, UINT32 block_offset )
{
    UINT32 i;
    PMC_ENTRY();
    for (i=0;i<block->num_registers;i++)
    {
        register_definition_t *reg = block->registers[i];
        UINT32 reg_addr = reg->addr;
        UINT32 reg_ofs = addr - block_offset - reg_addr;
        if (reg_ofs < 0x80000000 ) {
            UINT32 offset_N = reg->offset_N;
            UINT32 range_N = reg->range_N;
            UINT32 offset_M = reg->offset_M;
            UINT32 range_M = reg->range_M;
            UINT32 N=0,M=0;

            /*
             * figure out how many of the larger units fit into 
             * so we have an offset of 100 and offset_N is 16 and offset_M is 4,... 
             *  first we take out as many 16's and then as many 4's
             * we have to take out the larger offset first....
             */
            if (offset_N > offset_M) {
              if (offset_N > 0) {
                N = reg_ofs / offset_N;
                reg_ofs -= N * offset_N;
              }
              if (offset_M > 0) {
                M = reg_ofs / offset_M;
                reg_ofs -= M * offset_M;
              }
            } else {
              if (offset_M > 0) {
                M = reg_ofs / offset_M;
                reg_ofs -= M * offset_M;
              }
              if (offset_N > 0) {
                N = reg_ofs / offset_N;
                reg_ofs -= N * offset_N;
              }
            }

            if ((N <= range_N) && (M <= range_M) && (reg_ofs == 0)) {
                PMC_STRCPY(reg_index,"");
                if (range_N > 0) {
                    PMC_SPRINTF(&(reg_index[strlen(reg_index)])," N=%d",N);
                }
                if (range_M > 0) {
                    PMC_SPRINTF(&(reg_index[strlen(reg_index)])," M=%d",M);
                }
                PMC_RETURN( reg );
            }
        }
    }
    PMC_RETURN( NULL );
}

PRIVATE const char *digi_fw_strip_common_block_prefixes(const char *block_name)
{

    PMC_ENTRY();
    if (strncmp(block_name,"BLOCK_",6)==0) {
        block_name += 6;
    }
    if (strncmp(block_name,"DIGI120_",8)==0) {
        block_name += 8;
    }
    if (strncmp(block_name,"DIGI120_",8)==0) {
        block_name += 8;
    }
    PMC_RETURN(block_name);
}

void digi_fw_log_get_reg_name(void *param, UINT32 addr, const char **block_name_ptr, char block_index[30], const char**reg_name_ptr, char reg_index[30])
{
    UINT32 block_offset;
    block_definition_t *block;

    PMC_ENTRY();
    *block_name_ptr = NULL;
    *block_index = 0;
    *reg_name_ptr = NULL;
    *reg_index = 0;
    block = find_block(addr,block_index,&block_offset);
    if (block != NULL)
    {
        register_definition_t *reg;
        *block_name_ptr = digi_fw_strip_common_block_prefixes(block->name);
        reg = find_register( addr, reg_index, block, block_offset );
        if (reg != NULL) {
            *reg_name_ptr = reg->name;
        }
    }
    PMC_RETURN();
}

/*******************************************************************************
* num_bits_set
* ___________________________________________________________________________
*
* DESCRIPTION:
*   returns the number of bits set in the mask 
*   e.g.
*   num_bits_set( 0x0008 ) returns 1
*   num_bits_set( 0x00f0 ) returns 4
*
* INPUTS:
*   mask - the mask
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - The number of bits set in mask.
*
* NOTES:
*******************************************************************************/
static INLINE UINT32 num_bits_set( UINT32 mask )
{
    UINT32 x = 0;
    if (mask == 0)
    {
        return 0;
    }
    while ((mask & 0x1) != 1)
    {
        mask = mask >> 1;
    }
    while (mask > 0)
    {
        x++;
        mask = mask >> 1;
    }
    return x;
}

/*******************************************************************************
* extract_value
* ___________________________________________________________________________
*
* DESCRIPTION:
*   extracts the field from a register value and updates the number of bits in the mask.
*   e.g.
*   extract_value( 0x000f, 0x1234, &mask ) returns 0x4 and sets mask_bits to 4
*   extract_value( 0x0ff0, 0x1234, &mask ) returns 0x23 and sets mask_bits to 8
*
* INPUTS:
*   mask - the mask
*   value - the value
*
* OUTPUTS:
*   mask_bits - the number of bits set in the mask
*
* RETURNS:
*   int - The value of the register bit field.
*
* NOTES:
*******************************************************************************/
PRIVATE int extract_value( UINT32 mask, UINT32 value, UINT32 *mask_bits ) UNUSED;
PRIVATE int extract_value( UINT32 mask, UINT32 value, UINT32 *mask_bits )
{
    value = value & mask;

    while (((mask & 0x1) != 1) && (value != 0))
    {
        value = value >> 1;
        mask = mask >> 1;
    }
    *mask_bits = num_bits_set(mask);
    return(value);
}


PRIVATE void describe_value( char *buffer, UINT32 mask, UINT32 value, BOOL8 also_show_decimal )
{
	UINT32 first_bit = find_first_bit_set( mask );
    UINT32 bit_value = (value & mask) >> first_bit;
	UINT32 num_bits = num_bits_set( mask );
    char frmt[30];


    PMC_ENTRY();
    if (num_bits < 4) {
      PMC_SPRINTF(buffer,"%d",bit_value);
    } else {
        if (bit_value > 9 && also_show_decimal) {
            /*  if top bit is set, but not all bits, also display negative value */
            if ((bit_value & (1 << (num_bits-1))) && (value != mask)) {
                PMC_SPRINTF(frmt,"0x%%0%dx (%%d or %%d)",(num_bits+3)/4);
                PMC_SPRINTF(buffer,frmt,bit_value,bit_value,bit_value - (1 << num_bits));
            } else {
                PMC_SPRINTF(frmt,"0x%%0%dx (%%d)",(num_bits+3)/4);
                PMC_SPRINTF(buffer,frmt,bit_value,bit_value);
            }
        } else {
            PMC_SPRINTF(frmt,"0x%%0%dx",(num_bits+3)/4);
            PMC_SPRINTF(buffer,frmt,bit_value);
        }
    }
    PMC_RETURN();
}

/*
 * displays the value of the field possibly with a [start..end] 
 *
 * for example:
 *   describe_field_value( buffer, 0xff, 0xff, 0x1, 2 ) prints "=0x01"
 *   describe_field_value( buffer, 0xff, 0x08, 0x8, 2 ) prints "[3]=1"
 *
 */
PRIVATE void describe_field_value( char *buffer, UINT32 field_mask, UINT32 mask, UINT32 value, UINT32 flags )
{
    BOOL use_exact_mask = (flags & 4);
    const char *equals_or_colon = (flags & 2)?":":"=";
    char *tail = buffer;

    PMC_ENTRY();
    /* display incomplete masks using [low..hi] notation */
    if (((field_mask & mask) == field_mask) || !(use_exact_mask)) {
        PMC_SPRINTF(tail,equals_or_colon);
        tail += PMC_STRLEN(tail);
        describe_value(tail,field_mask,value,FALSE);
    } else {
        UINT32 sub_mask = mask & field_mask;
        UINT32 first_mask_bit = find_first_bit_set(sub_mask);
        UINT32 last_mask_bit = find_last_bit_set(sub_mask);
        UINT32 first_field_bit = find_first_bit_set(field_mask);

        /*
         # if the field is 0x00ffff00 [8..23]
         #  and the mask is 0x000ff000 [12..19] then we 
         #   want to show field[4..11] which is:
         #   (12-8) .. (19-8) the 23 is ignored.
         #  and we want to show get_bit_value( $sub_mask, $value )$bit_value >> 4
         */
        UINT32 start_bit = (first_mask_bit-first_field_bit);
        UINT32 stop_bit = (last_mask_bit-first_field_bit);
        if (start_bit == stop_bit) {
            PMC_SPRINTF(tail,"[%d]",start_bit);
            tail += PMC_STRLEN(tail);
        } else {
            PMC_SPRINTF(tail,"[%d..%d]",start_bit,stop_bit);
            tail += PMC_STRLEN(tail);
        }
        PMC_SPRINTF(tail,equals_or_colon);
        tail += PMC_STRLEN(tail);
        describe_value(tail,sub_mask,value,FALSE);
    }
    PMC_RETURN();
}

/*******************************************************************************
* describe_bits
* ___________________________________________________________________________
*
* DESCRIPTION:
*   updates buffer with the string "field=<value>"
*  
*    reg_bit_descriptions is a string like:
*    "0x00000ff0,TX_SM_BEI_BIAE_VALUE,0x00000008,TX_SM_BDI_VALUE,0x00000004,TX_SM_IAE_VALUE,0x00000003,TX_SM_RES_VALUE" ) 
*    
*    The following examples assume the above reg_bit_descriptions:
*    
*    describe_bits( reg_bit_descriptions, buffer, 0xfe, 0x1234, &fields_used )
*    fields_used = 0xfff;
*    buffer = "TX_SM_BEI_BIAE_VALUE=0x23 TX_SM_BDI_VALUE=0 TX_SM_IAE_VALUE=1 TX_SM_RES_VALUE=0"
*   
*   no check is made to ensure that there is enough space in the buffer.
*   the function also returns 'buffer'.
*
* INPUTS:
*   reg_bit_descriptions - array of register bit field description strings
*   buffer - the buffer to modify
*   mask - mask of relevant bits
*   value - value being operated on
*   flags - 1 - just display field names (no values).
*
* OUTPUTS:
*   buffer - string of field names and values
*
* RETURNS:
*   char - pointer to buffer.
*
* NOTES:
*******************************************************************************/
static const char *describe_bits( const char *reg_bit_descriptions, char *buffer, UINT32 mask, UINT32 value, UINT32 flags )
{
    BOOL blank = TRUE;
    char *tail = buffer; /* tail is always pointing to the last character in buffer */
    char *desc = (char *)reg_bit_descriptions;
    BOOL just_display_field_names = (flags & 1);
    BOOL is_read_modify_write = (flags & 4);
	UINT64 temp_bit_mask;
	
    PMC_STRCPY(tail," fields={");
    tail += PMC_STRLEN(tail);
    while (*desc != 0)
    {
        UINT32 bit_mask;
        char *bit_descr;
        char *temp;
        int len;
        if (strncmp(desc,"0x",2) != 0)
        {
            break;/* if *desc does not point to "0x", then exit this function */
        }
        desc += 2;

        /* extract the bit mask */
        /* first check the return value*/
		temp_bit_mask = strtoul( desc, &temp, 16 );
		PMC_ASSERT(temp_bit_mask <= MAX_UINT32 , PMC_SHARED_ERR_BIT_MASK_EXTRACT_FAIL,0,0);
		bit_mask = temp_bit_mask;
		
        if (desc == temp) {
            break; /* if *desc does not point to a valid hexadecimal number, then exit this function */
        }
        /* sscanf( desc, "%u", &bit_mask ); */
        desc = strchr(desc,',');
        desc++;
        /* mark the beginning of the description of the bit field */
        bit_descr = desc;
        desc = strchr(desc,',');
        if (desc == NULL)
            desc = bit_descr+PMC_STRLEN(bit_descr);
        /* calculate the 'len' of the description of the bit field */
        len = desc - bit_descr;
        /* if this bit field should be printed */
        if (bit_mask & mask)
        {
            if (!is_read_modify_write
                && (bit_descr[strlen(bit_descr)-2] == '_')
                 && ((bit_descr[strlen(bit_descr)-1] == 'I') || (bit_descr[strlen(bit_descr)-1] == 'E'))
                 && ((bit_mask&value) == 0))
            {
                /* don't print _I or _E fields that are zero unless part of a read/modify/write */
            }
            else
            {
                strncat(tail,bit_descr,len);
                tail[len] = 0;
                tail += len;
                blank = FALSE;
                if (!just_display_field_names) {
                    describe_field_value(tail,bit_mask,mask,bit_mask&value,flags);
                    tail += PMC_STRLEN(tail);
                }
                *tail++ = ' ';
                *tail = 0;
            }
        }
        if (*desc == ',')
            desc ++;
    }
    if (blank)
        PMC_STRCPY(buffer,"");
    else {
        tail--;
        PMC_STRCPY(tail,"}");
    }
    return(buffer);
}

static block_definition_t *last_macro = NULL;
static BOOL8 is_macro = FALSE;
static l5sys_operation_code_definitions_enum last_opcode;
static UINT32 last_count = 1;
static UINT32 access_count = 0;
static char access_pattern[20000] = {0};

PRIVATE void digi_fw_log_append_access_pattern_str( const char *str )
{
    PMC_ENTRY();
    UINT32 str_len = PMC_STRLEN(str) + 1;
    UINT32 len = PMC_STRLEN(access_pattern);
    if (len + str_len + 1 < sizeof(access_pattern)) {
        PMC_SPRINTF(&(access_pattern[len])," %s",str);
    }
    PMC_RETURN();
}

PRIVATE void digi_fw_log_append_access_pattern_count( UINT32 count )
{
    PMC_ENTRY();
    UINT32 access_len = 20;
    UINT32 len = PMC_STRLEN(access_pattern);
    if (count > 1) {
        if (len + access_len + 1 < sizeof(access_pattern)) {
            PMC_SPRINTF(&(access_pattern[len])," x%d",count);
        }
    }
    PMC_RETURN();
}

PRIVATE void digi_fw_log_flush_access_pattern(void *param)
{
    PMC_ENTRY();
    digi_fw_log_append_access_pattern_count( last_count );
    last_count = 1;
    if (last_macro != NULL) {
        pmc_sys_handle_t *sys_handle = ((digi_handle_t *)param)->base.sys_handle;
        const char *name = last_macro->name;
        if (strncmp(name,"BLOCK_",6)==0) {
            name += 6;
        }
        if (strncmp(name,"DIGI120_",8)==0) {
            name += 8;
        }
        l5sysLog(sys_handle,"pattern: %-40s%s count=%-4d %s\n",name,is_macro?" (MACRO)":"",access_count,access_pattern);
    }
    strcpy(access_pattern,"");
    last_opcode = (l5sys_operation_code_definitions_enum)0;
    access_count = 0;
    PMC_RETURN();
}


PRIVATE void digi_fw_show_access_stats(pmc_sys_handle_t *sys_handle, void *param)
{
    PMC_SYS_DSTAT_DECLARE( num_writes,       "writes   " );
    PMC_SYS_DSTAT_DECLARE( num_reads,        "reads    " );
    PMC_SYS_DSTAT_DECLARE( num_polls,        "polls    " );
    PMC_SYS_DSTAT_DECLARE( num_short_rmw,    "short_rmw" );
    PMC_SYS_DSTAT_DECLARE( num_long_rmw,     "long_rmw " );
    PMC_SYS_DSTAT_DECLARE( num_rmw,          "rmw      " );
    PMC_SYS_DSTAT_DECLARE( num_burst_writes, "burst_wr " );
    PMC_SYS_DSTAT_DECLARE( num_sleeps,       "sleeps   " );
    PMC_SYS_DSTAT_DECLARE( total,            "total    " );
    UINT32 sleep_time = 0;
    UINT32 i;
    char buffer[500];
    pmc_sys_dstat_t *stat;
    const char *prefix = "";
    const char *indent = "    ";

    PMC_ENTRY();
    num_writes->num = 0;
    num_reads->num = 0;
    num_polls->num = 0;
    num_short_rmw->num = 0;
    num_long_rmw->num = 0;
    num_burst_writes->num = 0;    
    num_sleeps->num = 0;

    digi_fw_log_flush_access_pattern(param);

    for (i=0;i<num_blocks;i++) {
        block_definition_t *block = BLOCKS[i];
        total->num = 0;
        num_rmw->num = 0;
        if ((block->num_writes->num > 0) 
            || (block->num_reads->num > 0) 
            || (block->num_polls->num > 0) 
            || (block->num_short_rmw->num > 0) 
            || (block->num_long_rmw->num > 0)
            || (block->num_sleeps->num > 0)
            || (block->num_burst_writes->num > 0)) {
            const char *name = block->name;
            if (strncmp(name,"BLOCK_",6)==0) {
                name += 6;
            }
            if (strncmp(name,"DIGI120_",8)==0) {
                name += 8;
            }
            l5sysLog(sys_handle,"%s\n",name);
            stat = block->num_writes;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
                pmc_sys_dstat_merge( num_writes, stat );
                pmc_sys_dstat_merge( total, stat );
            }
            stat = block->num_burst_writes;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
                pmc_sys_dstat_merge( num_burst_writes, stat );
                pmc_sys_dstat_merge( total, stat );
            }
            stat = block->num_short_rmw;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
                pmc_sys_dstat_merge( num_short_rmw, stat );
                pmc_sys_dstat_merge( total, stat );
                pmc_sys_dstat_merge( num_rmw, stat );
            }
            stat = block->num_long_rmw;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
                pmc_sys_dstat_merge( num_long_rmw, stat );
                pmc_sys_dstat_merge( total, stat );
                pmc_sys_dstat_merge( num_rmw, stat );
            }
            stat = num_rmw;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
            }
            stat = block->num_reads;
            if (stat->num > 0) {
                UINT32 len;
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                len = PMC_STRLEN(buffer);
                if (len > 0 && len < sizeof(buffer)-5) {
                    PMC_SPRINTF(&(buffer[len-1])," <--\n");
                }
                l5sysLog(sys_handle,"%s  --> %s",indent,buffer);
                pmc_sys_dstat_merge( num_reads, stat );
                pmc_sys_dstat_merge( total, stat );
            }
            stat = block->num_sleeps;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
                l5sysLog(sys_handle,"%s   (%d microseconds)\n",indent,block->sleep_time);
                pmc_sys_dstat_merge( num_sleeps, stat );
                pmc_sys_dstat_merge( total, stat );
                sleep_time += block->sleep_time;
            }
            stat = block->num_polls;
            if (stat->num > 0) {
                pmc_sys_dstat_show_to_buffer(stat,prefix,buffer);
                l5sysLog(sys_handle,"%s%s",indent,buffer);
                pmc_sys_dstat_merge( num_polls, stat );
                pmc_sys_dstat_merge( total, stat );
            }
            pmc_sys_dstat_show_to_buffer(total,prefix,buffer);
            l5sysLog(sys_handle,"%s%s",indent,buffer);
            block->num_writes->num = 0;
            block->num_reads->num = 0;
            block->num_polls->num = 0;
            block->num_short_rmw->num = 0;
            block->num_long_rmw->num = 0;
            block->num_burst_writes->num = 0;
            block->num_sleeps->num = 0;
            block->sleep_time = 0;
        }
    }
    l5sysLog(sys_handle,"\n");
    l5sysLog(sys_handle,"totals\n");

    total->num = 0;
    num_rmw->num = 0;
#define SHOW_STAT(stat) do { \
    pmc_sys_dstat_show_to_buffer(stat,"",buffer); \
    pmc_sys_dstat_merge( total, stat ); \
    l5sysLog(sys_handle,"%s%s",indent,buffer); \
    } while(0)
    SHOW_STAT(num_writes);
    SHOW_STAT(num_burst_writes);
    SHOW_STAT(num_short_rmw);
    SHOW_STAT(num_long_rmw);

    pmc_sys_dstat_merge( num_rmw, num_short_rmw );
    pmc_sys_dstat_merge( num_rmw, num_long_rmw );
    pmc_sys_dstat_show_to_buffer(num_rmw,"",buffer);
    l5sysLog(sys_handle,"%s%s",indent,buffer);

    SHOW_STAT(num_reads);
    SHOW_STAT(num_sleeps);
    l5sysLog(sys_handle,"  (%d microseconds)\n",sleep_time);
    SHOW_STAT(num_polls);
    SHOW_STAT(total);
    SHOW_STAT(fast_fw_cmds);
    SHOW_STAT(slow_fw_cmds);
    SHOW_STAT(num_fw_queries);
    fast_fw_cmds->num = 0;
    slow_fw_cmds->num = 0;
    num_fw_queries->num = 0;
    PMC_RETURN();
}

PRIVATE void digi_fw_show_operation(UINT32 flags, UINT32 addr, UINT32 mask, UINT32 value, char *buffer)
{
    UINT32 block_offset;
    block_definition_t *block;
    char *tail = buffer; /* tail is always pointing to the last character in buffer */
    char block_index[30];
    char reg_index[30];

    PMC_ENTRY();
    *buffer = 0;

    block = find_block(addr,block_index,&block_offset);
    if (block == NULL) {
        PMC_SPRINTF(tail,"error finding block addr=0x%08x\n",addr);
        tail += PMC_STRLEN(tail);
    } else {
        register_definition_t *reg;


        PMC_SPRINTF(tail,"block=%s%s ",digi_fw_strip_common_block_prefixes(block->name),block_index);
        tail += PMC_STRLEN(tail);
        reg = find_register( addr, reg_index, block, block_offset );
        if (reg == NULL) {
            PMC_SPRINTF(tail,"error finding reg addr=0x%08x\n",addr);
            tail += PMC_STRLEN(tail);
        } else {
            PMC_SPRINTF(tail," register=%s%s ",reg->name,reg_index);
            tail += PMC_STRLEN(tail);
            describe_bits(reg->fields,tail,mask,value,flags);
        }
    }

    PMC_RETURN();
}

PRIVATE void digi_fw_show_write(UINT32 addr, UINT32 mask, UINT32 value, char *buffer)
{
    PMC_ENTRY();

    digi_fw_show_operation(0,addr,mask,value,buffer);

    PMC_RETURN();
}

PRIVATE void digi_fw_show_rmw(UINT32 addr, UINT32 mask, UINT32 value, char *buffer)
{
    PMC_ENTRY();

    digi_fw_show_operation(4,addr,mask,value,buffer);

    PMC_RETURN();
}

PRIVATE void digi_fw_show_read(UINT32 addr, UINT32 mask, UINT32 value, char *buffer)
{
    PMC_ENTRY();

    digi_fw_show_operation(2,addr,mask,value,buffer);

    PMC_RETURN();
}

PUBLIC void pmc_sys_stat_show_to_buffer(pmc_sys_dstat_t *s,const char *prefix,char *buffer);

/* opcode==0 means this is the last operation,... just flush the access pattern */
void digi_fw_log_get_description( void *param, l5sys_operation_code_definitions_enum opcode, UINT32 addr, UINT32 mask, UINT32 value, UINT32 fw_flags, char *buffer )
{
    UINT32 block_offset;
    block_definition_t *block;
    block_definition_t *macro;
    char *tail = buffer; /* tail is always pointing to the last character in buffer */
    char block_index[30];
    char reg_index[30];
    BOOL add_stats = ((fw_flags & 1) == 0);
    UINT32 flags = 0;

    PMC_ENTRY();
    if ((opcode == OP_CODE_READ) || (opcode == OP_CODE_BURST_READ) || (opcode == OP_CODE_BATCH_READ)) {
        flags |= 1; /* just display field names */
    }
    *buffer = 0;
    if (last_opcode!=opcode) {
        digi_fw_log_append_access_pattern_count(last_count);
        last_count = 1;
    }
    switch (opcode) {
        case OP_CODE_SLEEP:
        {
            if (last_macro != NULL) {
                pmc_sys_dstat_record_duration(last_macro->num_sleeps,fw_speed);
                last_macro->sleep_time += value;
            }
            digi_fw_log_append_access_pattern_str("SLEEP");
            last_opcode = opcode;
            PMC_RETURN();
         }
        case OP_CODE_FIRMWARE_CMD:
            digi_fw_log_flush_access_pattern(param);
            digi_fw_log_append_access_pattern_str("FW_CMD");
            last_opcode = opcode;
            if (fw_speed > 40) {
                pmc_sys_dstat_record_duration(slow_fw_cmds,fw_speed);
            } else {
                pmc_sys_dstat_record_duration(fast_fw_cmds,fw_speed);
            }
            PMC_RETURN();
        case OP_CODE_FIRMWARE_QUERY:
            digi_fw_log_flush_access_pattern(param);
            digi_fw_log_append_access_pattern_str("FW_QUERY");
            last_opcode = opcode;
            pmc_sys_dstat_record_duration(num_fw_queries,fw_speed);
            PMC_RETURN();
        default:
            break;
    }

    block = find_block(addr,block_index,&block_offset);
    if (block == NULL) {
        macro = NULL;
    } else {
        macro = block->mtsb;
    }
    if (macro == NULL) {
        macro = block;
    }
    /* flush the access pattern if we are accessing a new macro or if there have been a lot of accesses already and the buffer is about to overflow */
    if (last_macro != macro || (strlen(access_pattern) > sizeof(access_pattern) - 30)) {
        digi_fw_log_flush_access_pattern(param);
        last_macro = macro;
    }
    if (last_opcode!=opcode) {
        const char *abbr;

        switch (opcode) {
            case OP_CODE_WRITE         : abbr = "WR"; break;
            case OP_CODE_SHORT_RMW     : abbr = "sRMW"; flags |= 4; /* display sub field if not complete field written */ break;
            case OP_CODE_LONG_RMW      : abbr = "lRMW"; flags |= 4; /* display sub field if not complete field written */ break;
            case OP_CODE_READ          : abbr = "RD"; break;
            case OP_CODE_POLL          : abbr = "POLL"; break;
            /* unreachable 
            case OP_CODE_SLEEP         : abbr = "SLEEP"; break;
            */
            case OP_CODE_BENCH         : abbr = "BENCH"; break;
            case OP_CODE_SEGMENT       : abbr = "SEGMENT"; break;
            case OP_CODE_BURST_WRITE   : abbr = "B_WR"; break;
            case OP_CODE_BURST_READ    : abbr = "B_RD"; break;
            case OP_CODE_BATCH_READ    : abbr = "BATCH_READ"; break;
            /* unreachable 
            case OP_CODE_FIRMWARE_CMD  : abbr = "FW_CMD"; break;
            case OP_CODE_FIRMWARE_QUERY: abbr = "FW_QUERY"; break;
            */
            case OP_CODE_PADDING       : abbr = "PADDING"; break;
            case OP_CODE_SELF_TEST     : abbr = "SELF_TEST"; break;
            case OP_CODE_CHECK_SUM     : abbr = "CHECK_SUM"; break;
            default: 
            if ((UINT32)opcode == 0x80000000) {
                abbr = "direct_READ";
            } else {
                abbr = "unknown_opcode"; 
            }
            break;
        }
        digi_fw_log_append_access_pattern_str(abbr);
        last_count = 1;
        last_opcode = opcode;
    } else {
        last_count++;
    }
    if (block == NULL) {
        PMC_SPRINTF(tail,"error finding block addr=0x%08x\n",addr);
        tail += PMC_STRLEN(tail);
    } else {
        register_definition_t *reg;

        access_count++;
        {
            pmc_sys_dstat_t *stat = NULL;
        switch(opcode) {
        case OP_CODE_BURST_WRITE: stat = macro->num_burst_writes; break;
        case OP_CODE_WRITE:       stat = macro->num_writes; break;
        case OP_CODE_BURST_READ:
        case OP_CODE_BATCH_READ:
        case OP_CODE_READ:        stat = macro->num_reads; break;
        case OP_CODE_POLL:        stat = macro->num_polls; break;
        case OP_CODE_SHORT_RMW:   stat = macro->num_short_rmw; break;
        case OP_CODE_LONG_RMW:    stat = macro->num_long_rmw; break;
        default:break;
        }
        if (add_stats && (stat != NULL)) {
            pmc_sys_dstat_record_duration(stat,fw_speed);
        }
        }
        PMC_SPRINTF(tail,"block=%s%s ",digi_fw_strip_common_block_prefixes(block->name),block_index);
        tail += PMC_STRLEN(tail);
        reg = find_register( addr, reg_index, block, block_offset );
        if (reg == NULL) {
            PMC_SPRINTF(tail,"error finding reg addr=0x%08x\n",addr);
            tail += PMC_STRLEN(tail);
        } else {
            PMC_SPRINTF(tail," register=%s%s ",reg->name,reg_index);
            tail += PMC_STRLEN(tail);
            describe_bits(reg->fields,tail,mask,value,flags);
        }
    }

    PMC_RETURN();
}
#endif /* PMC_DOX_FOR_SWIG */


/*******************************************************************************
* digi_fw_log_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   initialize various logging functions
*
* INPUTS:
*   digi_handle - DIGI device handle.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
void digi_fw_log_init(digi_handle_t * digi_handle)
{
    PMC_ENTRY();
    l5sys_install_queue_stall_callback(digi_handle,(l5sys_callback_t)digi_fw_show_queue_stall,digi_handle);
    l5sys_install_info_functions(digi_handle,digi_fw_log_get_function_name,digi_fw_log_get_description,digi_fw_show_access_stats);
    PMC_RETURN();
}

/** benchmark types */
PRIVATE const char *prov_deprov_type_str[PROV_DEPROV_NUM] = {
    "no queue        ",
    "record to file  ",
    "record to memory",
    "without atomic  ",
    "with atomic     ",
    "playback fill   ",
    "playback wait   ",
    "playback        ",
    "timed playback  ",
    "bench playback  "
};

/*******************************************************************************
* digi_fw_log_prov_with_extra_atomic
* ___________________________________________________________________________
*
* DESCRIPTION:
*   call the provision function, but wrap it inside a PMC_ATOMIC_ENTRY/RETURN
*
* INPUTS:
*   t - digi_fw_log_prov_deprov_stats_t
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    PMC_ERROR - return code from provision function
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_log_prov_with_extra_atomic(digi_fw_log_prov_deprov_stats_t *t)
{
    digi_handle_t *digi_handle = t->digi_handle;
    PMC_ATOMIC_ENTRY(digi_handle);
    t->result = t->prov_function(t->test_parameters);
    PMC_ATOMIC_RETURN(digi_handle,t->result);
}

/*******************************************************************************
* digi_fw_log_deprov_with_extra_atomic
* ___________________________________________________________________________
*
* DESCRIPTION:
*   call the deprovision function, but wrap it inside a PMC_ATOMIC_ENTRY/RETURN
*
* INPUTS:
*   t - digi_fw_log_prov_deprov_stats_t
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    PMC_ERROR - return code from deprov function
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_fw_log_deprov_with_extra_atomic(digi_fw_log_prov_deprov_stats_t *t)
{
    digi_handle_t *digi_handle = t->digi_handle;
    PMC_ATOMIC_ENTRY(digi_handle);
    t->result = t->deprov_function(t->test_parameters);
    PMC_ATOMIC_RETURN(digi_handle,t->result);
}

/*******************************************************************************
* digi_fw_log_prov_deprov_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*   initialize a prov/deprov statistics record
*
* INPUTS:
*   t                - digi_fw_log_prov_deprov_stats_t
*   name             - name of the test (e.g. "example_1xOTU4_80xODU0")
*   _prov_function   - provision function (takes 1 argument)
*   _deprov_function - deprovision function (takes 1 argument)
*   digi_handle      - DIGI device handle.
*   _test_parameters - argument to prov/deprov
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
void digi_fw_log_prov_deprov_init( digi_fw_log_prov_deprov_stats_t *t, const char *name, digi_fw_log_prov_deprov_callback_t _prov_function, digi_fw_log_prov_deprov_callback_t _deprov_function, digi_handle_t *digi_handle, void *_test_parameters )
{
    UINT32 i;
    memset(t,0,sizeof(digi_fw_log_prov_deprov_stats_t));
    for (i=0;i<PROV_DEPROV_NUM;i++) {
        t->prov_stats[i].initialized = 1;
        t->prov_stats[i].description    = "prov ";
        t->prov_stats[i].description2   = prov_deprov_type_str[i];
        t->deprov_stats[i].initialized = 1;
        t->deprov_stats[i].description  = "deprov ";
        t->deprov_stats[i].description2 = prov_deprov_type_str[i];
    }
    strcpy(t->name,name);
    t->prov_function = _prov_function;
    t->deprov_function = _deprov_function;
    t->test_parameters = _test_parameters;
    t->digi_handle = digi_handle;
    t->result = PMC_SUCCESS;
}

/*******************************************************************************
* digi_fw_log_prov_deprov_halt
* ___________________________________________________________________________
*
* DESCRIPTION:
*   free memory associated with a prov/deprov statistics record
*
* INPUTS:
*   t                - digi_fw_log_prov_deprov_stats_t
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
void digi_fw_log_prov_deprov_halt( digi_fw_log_prov_deprov_stats_t *t )
{
    l5sys_log_free(t->provision_messages);
    t->provision_messages = NULL;
    l5sys_log_free(t->deprovision_messages);
    t->deprovision_messages = NULL;
}

/*******************************************************************************
* digi_fw_log_prov_deprov_print
* ___________________________________________________________________________
*
* DESCRIPTION:
*   display statistics that have been gathered for example:
*   -------
*   -- example_10xOTU2_80xODU0: RESULTS --
*    prov no queue        : n=66, total=2.595 seconds, min=39.07, max=40.08, avg=39.31 milliseconds
*    prov record to file  : 993.7 milliseconds
*    prov record to memory: 34.40 milliseconds
*    prov without atomic  : n=82, total=2.543 seconds, min=30.66, max=33.09, avg=31.01 milliseconds
*    prov with atomic     : n=199, total=2.671 seconds, min=13.34, max=13.66, avg=13.42 milliseconds
*    prov playback fill   : n=168, total=483.2 milliseconds, min=2.869, max=2.895, avg=2.876 milliseconds
*    prov playback wait   : n=168, total=2.191 seconds, min=12.92, max=13.19, avg=13.04 milliseconds
*    prov playback        : n=203, total=2.646 seconds, min=12.95, max=13.23, avg=13.03 milliseconds
*    prov timed playback  : 13.54 milliseconds
*   -------
*    deprov no queue        : n=66, total=2.446 seconds, min=36.59, max=39.14, avg=37.05 milliseconds
*    deprov record to file  : 878.6 milliseconds
*    deprov record to memory: 33.84 milliseconds
*    deprov without atomic  : n=82, total=2.476 seconds, min=29.90, max=31.12, avg=30.20 milliseconds
*    deprov with atomic     : n=199, total=2.332 seconds, min=11.40, max=12.90, avg=11.72 milliseconds
*    deprov playback fill   : n=168, total=395.0 milliseconds, min=2.340, max=3.372, avg=2.351 milliseconds
*    deprov playback wait   : n=168, total=1.918 seconds, min=11.14, max=11.73, avg=11.42 milliseconds
*    deprov playback        : n=203, total=2.335 seconds, min=11.23, max=11.82, avg=11.50 milliseconds
*    deprov timed playback  : 11.65 milliseconds
*   -------
*   
* INPUTS:
*   t                - digi_fw_log_prov_deprov_stats_t
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
void digi_fw_log_prov_deprov_print( digi_fw_log_prov_deprov_stats_t *t )
{
    UINT32 i;
    FW_LOG("-------\n");
    FW_LOG("-- %s: RESULTS --\n",t->name);
    for (i=0;i<PROV_DEPROV_NUM;i++) {
        if (t->prov_stats[i].num > 0) {
            pmc_sys_stat_show(&t->prov_stats[i],"");
        }
    }
    FW_LOG("-------\n");
    for (i=0;i<PROV_DEPROV_NUM;i++) {
        if (t->deprov_stats[i].num > 0) {
            pmc_sys_stat_show(&t->deprov_stats[i],"");
        }
    }
    FW_LOG("-------\n");
}

/*******************************************************************************
* digi_fw_log_prov_deprov_test
* ___________________________________________________________________________
*
* DESCRIPTION:
*   perform one benchmark
*
* INPUTS:
*   t         - digi_fw_log_prov_deprov_stats_t
*   test_type - digi_fw_log_prov_deprov_test_type
*   duration  - number of seconds to repeat the test.
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
void digi_fw_log_prov_deprov_test( digi_fw_log_prov_deprov_stats_t *t, digi_fw_log_prov_deprov_test_type test_type, double duration )
{
    digi_handle_t *digi_handle = t->digi_handle;
    void *test_parameters = t->test_parameters;
    char prov_file_name[130];
    char deprov_file_name[130];
    BOOL has_deprov = t->deprov_function != NULL;
    BOOL has_prov = t->prov_function != NULL;

    PMC_SYS_STAT_DECLARE(prov_deprov_test_time,"prov_deprov_test");

    PMC_ENTRY();
    pmc_sys_stat_mark_start(prov_deprov_test_time);
    
    FW_LOG("== %s: testing %s ==\n",t->name,prov_deprov_type_str[test_type]);

    /* delay a bit to allow screen to finish updating */
    PMC_OS_USLEEP(20000);
    switch (test_type)
    {
        case PROV_DEPROV_NO_QUEUE       :
            l5sys_firmware_queue_disable(digi_handle);

            do {
                if (has_deprov) {
                    pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                    t->deprov_function(test_parameters);
                    pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
                }

                if (has_prov) {
                    pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                    t->prov_function(test_parameters);
                    pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
                }
            } while (pmc_sys_stat_running_time(prov_deprov_test_time) < duration * 1000000);
            l5sys_firmware_queue_enable(digi_handle);
            break;

        case PROV_DEPROV_RECORD_TO_FILE :
        {
            l5sys_queue_message_t *messages;

            if (has_deprov) {
                pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                l5sys_log_record(digi_handle);

                t->deprov_function(test_parameters);
                messages = l5sys_log_stop(digi_handle);
                PMC_SPRINTF(deprov_file_name,"%s_deprovision.txt",t->name);
                l5sys_log_save(digi_handle,messages,deprov_file_name);
                l5sys_log_free(messages);
                pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
                l5sys_log_read(digi_handle,deprov_file_name,&(t->deprovision_messages));
            }

            if (has_prov) {
                pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                l5sys_log_record(digi_handle);
                t->prov_function(test_parameters);
                messages = l5sys_log_stop(digi_handle);
                PMC_SPRINTF(prov_file_name,"%s_provision.txt",t->name);
                l5sys_log_save(digi_handle,messages,prov_file_name);
                l5sys_log_free(messages);
                pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
                l5sys_log_read(digi_handle,prov_file_name,&(t->provision_messages));
            }

            break;
        }

        case PROV_DEPROV_RECORD_TO_MEMORY :
            if (has_deprov) {
                pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                l5sys_log_record(digi_handle);

                t->deprov_function(test_parameters);
                t->deprovision_messages = l5sys_log_stop(digi_handle);
                pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
            }
            if (has_prov) {
                pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                l5sys_log_record(digi_handle);
                t->prov_function(test_parameters);
                t->provision_messages = l5sys_log_stop(digi_handle);
                pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
            }
            break;

        case PROV_DEPROV_WITHOUT_ATOMIC :
            do {
                if (has_deprov) {
                    pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                    t->deprov_function(test_parameters);
                    pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
                }

                if (has_prov) {
                    pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                    t->prov_function(test_parameters);
                    pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
                }
            } while (pmc_sys_stat_running_time(prov_deprov_test_time) < duration * 1000000);
            break;

        case PROV_DEPROV_WITH_ATOMIC    :
            do {
                if (has_deprov) {
                    pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                    digi_fw_log_deprov_with_extra_atomic(t);
                    pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
                }

                if (has_prov) {
                    pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                    digi_fw_log_prov_with_extra_atomic(t);
                    pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
                }
            } while (pmc_sys_stat_running_time(prov_deprov_test_time) < duration * 1000000);
            break;
        
        case PROV_DEPROV_PLAYBACK_FILL  :
        case PROV_DEPROV_PLAYBACK_WAIT  :
            do {
                if (has_deprov) {
                    /* fill host memory with instructions */
                    pmc_sys_stat_mark_start(&t->deprov_stats[PROV_DEPROV_PLAYBACK_FILL]);
                    l5sys_log_playback(digi_handle,t->deprovision_messages,TRUE);
                    pmc_sys_stat_mark_end(&t->deprov_stats[PROV_DEPROV_PLAYBACK_FILL]);

                    /* tell firmware to "go" */
                    pmc_sys_stat_mark_start(&t->deprov_stats[PROV_DEPROV_PLAYBACK_WAIT]);
                    l5sys_wait_queue_done(digi_handle);
                    pmc_sys_stat_mark_end(&t->deprov_stats[PROV_DEPROV_PLAYBACK_WAIT]);
                }

                if (has_prov) {
                    /* fill host memory with instructions */
                    pmc_sys_stat_mark_start(&t->prov_stats[PROV_DEPROV_PLAYBACK_FILL]);
                    l5sys_log_playback(digi_handle,t->provision_messages,TRUE);
                    pmc_sys_stat_mark_end(&t->prov_stats[PROV_DEPROV_PLAYBACK_FILL]);

                    /* tell firmware to "go" */
                    pmc_sys_stat_mark_start(&t->prov_stats[PROV_DEPROV_PLAYBACK_WAIT]);
                    l5sys_wait_queue_done(digi_handle);
                    pmc_sys_stat_mark_end(&t->prov_stats[PROV_DEPROV_PLAYBACK_WAIT]);
                }
            } while (pmc_sys_stat_running_time(prov_deprov_test_time) < duration * 1000000);

            break;
        case PROV_DEPROV_PLAYBACK       :
            do {
                if (has_deprov) {
                    pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                    l5sys_log_playback(digi_handle,t->deprovision_messages,FALSE);
                    l5sys_wait_queue_done(digi_handle);
                    pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
                }

                if (has_prov) {
                    pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                    l5sys_log_playback(digi_handle,t->provision_messages,FALSE);
                    l5sys_wait_queue_done(digi_handle);
                    pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
                }
            } while (pmc_sys_stat_running_time(prov_deprov_test_time) < duration * 1000000);

            break;
        case PROV_DEPROV_TIMED_PLAYBACK :

            if (has_deprov) {
                pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                l5sys_log_timed_playback(digi_handle,t->deprovision_messages);
                l5sys_wait_queue_done(digi_handle);
                pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
                PMC_SPRINTF(deprov_file_name,"timed_%s_deprovision.txt",t->name);
                l5sys_log_save(digi_handle,t->deprovision_messages,deprov_file_name);
            }

            if (has_prov) {
                pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                l5sys_log_timed_playback(digi_handle,t->provision_messages);
                l5sys_wait_queue_done(digi_handle);
                pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
                PMC_SPRINTF(prov_file_name,"timed_%s_provision.txt",t->name);
                l5sys_log_save(digi_handle,t->provision_messages,prov_file_name);
            }

            break;
        case PROV_DEPROV_BENCH_PLAYBACK :
            if (has_deprov) {
                pmc_sys_stat_mark_start(&t->deprov_stats[test_type]);
                PMC_SPRINTF(deprov_file_name,"bench_%s_deprovision.txt",t->name);
                l5sys_log_bench_each_message(digi_handle,t->deprovision_messages,deprov_file_name);
                pmc_sys_stat_mark_end(&t->deprov_stats[test_type]);
            }
            if (has_prov) {
                pmc_sys_stat_mark_start(&t->prov_stats[test_type]);
                PMC_SPRINTF(prov_file_name,"bench_%s_provision.txt",t->name);
                l5sys_log_bench_each_message(digi_handle,t->provision_messages,prov_file_name);
                pmc_sys_stat_mark_end(&t->prov_stats[test_type]);
            }

            break;
    default:
        break;
    }
    PMC_RETURN();
}

/*******************************************************************************
* digi_fw_log_prov_deprov_test_all
* ___________________________________________________________________________
*
* DESCRIPTION:
*   perform all benchmark tests
*
* INPUTS:
*   t         - digi_fw_log_prov_deprov_stats_t
*   duration  - number of seconds to repeat each test. (note: the final test takes minutes regardless of this argument).
*  
* OUTPUTS:
*    none
*
*  RETURNS:
*    none
*
*******************************************************************************/
void digi_fw_log_prov_deprov_test_all( digi_fw_log_prov_deprov_stats_t *t, double duration )
{
    PMC_ENTRY();

    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_NO_QUEUE,         duration );
    if (!l5sys_queue_enabled(t->digi_handle)) {
        digi_fw_performance_queue_init(t->digi_handle , TRUE );
    }
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_RECORD_TO_FILE,   0 );
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_RECORD_TO_MEMORY, 0 );
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_WITHOUT_ATOMIC,   duration );
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_WITH_ATOMIC,      duration );
#ifdef L5SYS_PROFILING
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_PLAYBACK_WAIT,    duration );
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_PLAYBACK,         duration );
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_TIMED_PLAYBACK,   0 );
    digi_fw_log_prov_deprov_print( t );
    digi_fw_log_prov_deprov_test( t, PROV_DEPROV_BENCH_PLAYBACK,   0 );
#endif

    digi_fw_log_prov_deprov_print( t );

    PMC_RETURN();
}


#endif /* DOXYGEN_PUBLIC_ONLY */

