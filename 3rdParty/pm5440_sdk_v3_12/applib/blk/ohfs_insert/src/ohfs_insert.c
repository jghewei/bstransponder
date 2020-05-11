/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "ohfs_insert_loc.h"


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
** Log Messaging
*/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char OHFS_INSERT_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    OHFS_INSERT_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char OHFS_INSERT_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
                                      

PRIVATE PMC_ERROR ohfs_insert_fs_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 chnl,
                                     util_global_odukp_type_t oduk_type);
                                                                     
PRIVATE UINT32 ohfs_insert_pad_cnt_calc(ohfs_insert_handle_t *ohfs_insert_handle, 
                                        UINT32 chnl,
                                        util_global_odukp_type_t oduk_type,
                                        util_global_mapping_mode_t mapping_mode, 
                                        UINT32 num_ts);
                                                  
                                                  
PRIVATE PMC_ERROR ohfs_insert_int_validate(ohfs_insert_handle_t *ohfs_insert_handle,
                                           UINT32 chnl_id,
                                           void *non_null_ptr);

/*
** Public Functions
*/

/*******************************************************************************
* ohfs_insert_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an OHFS_INSERT block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the OHFS_INSERT subsystem relative to 
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ohfs_insert_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC ohfs_insert_handle_t *ohfs_insert_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    ohfs_insert_handle_t *ohfs_insert_handle;
    
    PMC_ENTRY();
    
    ohfs_insert_handle = (ohfs_insert_handle_t*)PMC_CTXT_CALLOC(sizeof(ohfs_insert_handle_t), parent);
    
    pmc_handle_init(parent, ohfs_insert_handle, sys_handle, PMC_MID_DIGI_OHFS_INSERT, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(OHFS_INSERT_LOG_ERR_STRINGS[0], OHFS_INSERT_LOG_ERR_TABLE_BASE, OHFS_INSERT_LOG_ERR_COUNT);
    
    ohfs_insert_handle->cfc_handle = cfc_ctxt_create((pmc_handle_t *)ohfs_insert_handle, base_address+BASE_ADDR_OHFS_CFC, sys_handle, "CFC"); 
    PMC_CTXT_REGISTER_PTR(&ohfs_insert_handle->cfc_handle, parent);
    
    PMC_RETURN(ohfs_insert_handle);
    
} /* ohfs_insert_ctxt_create */

 
/*******************************************************************************
* ohfs_insert_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an OHFS_INSERT block instance.
*
*
* INPUTS:
*   *ohfs_insert_handle         - pointer to OHFS_INSERT handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_insert_ctxt_destroy(ohfs_insert_handle_t *ohfs_insert_handle)
{
    PMC_ENTRY();
    
    /* Destroy handle for CFC in OHFS_INSERT */
    cfc_ctxt_destroy(ohfs_insert_handle->cfc_handle); 
    
    PMC_CTXT_FREE(&ohfs_insert_handle, ohfs_insert_handle);
    
    PMC_RETURN();
} /* ohfs_insert_ctxt_destroy */

/*******************************************************************************
* ohfs_insert_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an OHFS_INSERT block handle instance.
*
*
* INPUTS:
*   *ohfs_insert_handle         - pointer to OHFS_INSERT handle instance
*   *ohfs_insert_instance       - Enum for OHFS_INSERT instance. See ohfs_insert_instance_type_t.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_insert_handle_init(ohfs_insert_handle_t *ohfs_insert_handle,
                                    ohfs_insert_instance_type_t ohfs_insert_instance)
{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    
    PMC_ENTRY();
    
    /* configure the parent that is using this block */
    ohfs_insert_handle->cfg.instance = ohfs_insert_instance; 
    
    /* be default, MSI mode is auto */
    ohfs_insert_handle->cfg.msi_cfg_mode = UTIL_GLOBAL_OTN_MSI_CFG_AUTO;
    
    /* CFC parameters */
    ohfs_insert_handle->cfg.cfc_cfg.max_fifo_num  = OHFS_INSERT_CFC_MAX_FIFO_NUM; 
    ohfs_insert_handle->cfg.cfc_cfg.page_size     = OHFS_INSERT_CFC_PAGE_SIZE; 
     
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = ohfs_insert_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = ohfs_insert_handle->cfg.cfc_cfg.page_size;
    cfc_handle_init(ohfs_insert_handle->cfc_handle, &cfc_cfg);

    PMC_MEMSET(&ohfs_insert_handle->var.pt, 0, sizeof(UINT8) * UTIL_GLOBAL_MAX_NUM_ODU_CHNL);
    

    PMC_RETURN();
} /* ohfs_insert_handle_init */


/*******************************************************************************
* ohfs_insert_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles allocation of local memory freed in ctxt_destroy
*
*
* INPUTS:
*   *ohfs_insert_handle         - pointer to OHFS_INSERT handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_insert_alloc_init(ohfs_insert_handle_t *ohfs_insert_handle)
{
    PMC_ENTRY();

    cfc_alloc_init(ohfs_insert_handle->cfc_handle);
        
    PMC_RETURN();
} /* ohfs_insert_alloc_init */


/*******************************************************************************
* ohfs_insert_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an OHFS_INSERT block handle instance.
*
*
* INPUTS:
*   *ohfs_insert_handle         - pointer to OHFS_INSERT handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state            - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_insert_handle_restart_init(ohfs_insert_handle_t *ohfs_insert_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state)
{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    
    PMC_ENTRY();
    
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(OHFS_INSERT_LOG_ERR_STRINGS[0], OHFS_INSERT_LOG_ERR_TABLE_BASE, OHFS_INSERT_LOG_ERR_COUNT);
    }
     
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = ohfs_insert_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = ohfs_insert_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(ohfs_insert_handle->cfc_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state);
        
    PMC_RETURN();
} /* ohfs_insert_handle_restart_init */



/*******************************************************************************
*  ohfs_insert_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize OHFS_INSERT with parameters specific to this instantiation.
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   energy_state          - enum defining state to set the OHFS_INSERT module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for OHFS_INSERT module 
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_init(ohfs_insert_handle_t *ohfs_insert_handle, 
                                  pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(ohfs_insert_handle!=NULL, OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        ret_val = ohfs_insert_block_non_default_cfg(ohfs_insert_handle);
        if(PMC_SUCCESS == ret_val)
        {
            /* Initialize IPT in CFC */
            ret_val = cfc_init(ohfs_insert_handle->cfc_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {    
            ret_val = cfc_bypass(ohfs_insert_handle->cfc_handle, TRUE);
        }  
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        ret_val = cfc_init(ohfs_insert_handle->cfc_handle, energy_state);  
        
    } else {
        PMC_ASSERT(FALSE, OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);

} /* ohfs_insert_init */


/*******************************************************************************
* ohfs_insert_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   OHFS_INSERT instance.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL ohfs_insert_start_state_test(ohfs_insert_handle_t *ohfs_insert_handle) 
{
    PMC_ENTRY();
    PMC_ASSERT(ohfs_insert_handle!=NULL, OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    
    if(cfc_start_state_test(ohfs_insert_handle->cfc_handle) == FALSE)
        PMC_RETURN(FALSE);
  
    PMC_RETURN(TRUE);

} /* ohfs_insert_start_state_test */


/*******************************************************************************
* ohfs_insert_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ram_init(ohfs_insert_handle_t *ohfs_insert_handle)
{
    /* variable declaration */
    UINT32 i;   
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    for (i = 0; i < 96; i++)
    {
        ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write(NULL, ohfs_insert_handle,i , 0x00000000);
        ohfs_insert_reg_LO_CFG_REG_array_write(NULL, ohfs_insert_handle, i, 0x0000007F);
        ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write(NULL, ohfs_insert_handle, i, 0x00000000);
    }
    
    /* initialize IPT_ID = 0x7F */
    for (i = 0; i < 96; i++)
    {
        ohfs_insert_reg_JC_RD_IPT_ENTRY_array_write(NULL, ohfs_insert_handle, i, 0x00007F00);
        ohfs_insert_reg_JC_WR_IPT_ENTRY_array_write(NULL, ohfs_insert_handle, i, 0x00007F00);
        
        result = cfc_ipt_cfg(ohfs_insert_handle->cfc_handle, i, 0, 0x7F, 0);
    }

    PMC_RETURN(result);
    
} /* ohfs_insert_ram_init */

/*******************************************************************************
*  ohfs_insert_cfc_ipt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the inverted page table for the channelized FIFO
*   controller that maps virtual block addresses to physical block addresses.
*   
*
* INPUTS:
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   ipt_valid           - qualifies the inverted page table entry \n
*                         0 -- the corresponding FIFO ID specified in IPT_ID is 
*                              invalid \n
*                         1 -- the corresponding FIFO ID specified in IPT_ID is 
*                              valid \n
*   ipt_id              - channel ID associated with the physical block.
*   ipt_va              - the block number associated with the physical block.
*                         
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise..
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_cfc_ipt_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 index, 
                                         UINT32 ipt_valid, 
                                         UINT32 ipt_id, 
                                         UINT32 ipt_va)
{   
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = cfc_ipt_cfg(ohfs_insert_handle->cfc_handle, index, 
                         ipt_valid, ipt_id, ipt_va);
    
    PMC_RETURN(result);
} /*  ohfs_insert_cfc_ipt_cfg */                                


/*******************************************************************************
* ohfs_insert_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize common configuration register for all channels. That is required
*   for proper operation of this block.
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_block_non_default_cfg(ohfs_insert_handle_t *ohfs_insert_handle)
{
    UINT32 chnl;

    PMC_ENTRY();

    /* Initialize XOFF_WM_CFG for all channels */
    for (chnl = 0; chnl < OHFS_INSERT_MAX_CHNL_NUM; ++chnl)
    {
        ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write(NULL, ohfs_insert_handle, chnl, 0x00000000);
        ohfs_insert_reg_XOFF_WM_CFG_array_write(NULL, ohfs_insert_handle, chnl, 0);
        ohfs_insert_reg_LO_CFG_REG_array_write(NULL, ohfs_insert_handle, chnl, 0x0000007F);
        ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write(NULL, ohfs_insert_handle, chnl, 0x00000000);        
    }

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_block_non_default_cfg */

/*******************************************************************************
* ohfs_insert_chnl_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   state                 - OHFS_INSERT channel register state \n
*                           0 -- ACCESSIBLE make channel registers accessible and 
*                                apply any non-default configuration to the set 
*                                of registers associated with this channel
*                                (PMC_BLOCK_ALTERABLE) \n
*                           1 -- DEFAULT force channel config registers to default 
*                                (these registers are unmodifiable by write access)
*                                (PMC_BLOCK_DEFAULT)
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_chnl_default_init(ohfs_insert_handle_t *ohfs_insert_handle,
                                               UINT32 chnl,
                                               pmc_block_reg_access_state_enum state)
{
    /* variable declaration */
    
    PMC_ENTRY();

    if (state == PMC_BLOCK_ALTERABLE)
    {
    }
    else if (state == PMC_BLOCK_DEFAULT)
    {
        ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_write(NULL, ohfs_insert_handle, chnl, 0);
        ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write(NULL, ohfs_insert_handle, chnl, 0);
        ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write(NULL, ohfs_insert_handle, chnl, 0);
        ohfs_insert_reg_XOFF_WM_CFG_array_write(NULL, ohfs_insert_handle, chnl, 0);
    }

    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_chnl_default_init */


/*******************************************************************************
* ohfs_insert_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures PT value and fixed stuff configuration in 
*   OHFS_INSERT based on the channel set-up inherited from TGMP or ODTU_MUX.
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*   server_type           - for MUX, HO ODU rate. See util_global_odukp_type_t. \n
*                           for TGMP, ODUkp or ODUjp type see odukp_type_t, 
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   client_type           - for MUX, LO ODU rate. See odukp_type_enum. \n 
*                           for TGMP, adaptaion_function. See
*                           adaptaion_function_enum for valid types
*   client_rate           - client data rate bits/sec for the case of: \n
*                           ODUJP_CBRX_GMP \n
*                           ODUKP_CBRX_B_A \n
*                           ODUKP_CBRX_A
*   mapping_mode          - Enum for mapping mode. Refer to util_global_mapping_mode_t
*   ts_type               - tributary slot type. See util_global_odu_line_payload_t. \n
*                           NOTE: Only valid for MUX. This argument is 
*                           ignored in TGMP.
*   num_cal_entries       - number of calendar enrties required for the channel
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_chnl_prov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                       UINT32 chnl,
                                       util_global_odukp_type_t server_type,
                                       UINT32 client_type,
                                       DOUBLE client_rate,
                                       util_global_mapping_mode_t mapping_mode,
                                       util_global_odu_line_payload_t ts_type, 
                                       UINT32 num_cal_entries)
{
    /* variable declaration */
    UINT32 ext_payload_type;
    UINT32 max_slot_cnt; 
    UINT32 map_type;
    UINT32 pad_cnt;
    UINT32 pt;
    PMC_ERROR ret_val = PMC_SUCCESS;  
    ohfs_insert_buffer_t ohfs_insert_buf[1];
    
    PMC_ENTRY();
    
    /* variable initialization */
    ext_payload_type = 0;
    map_type = 0;
    pad_cnt = 0;
    max_slot_cnt = 0;
    pt = 0;
    
    /* argument checking */
    if ((chnl > 95) || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || (mapping_mode > UTIL_GLOBAL_NO_MAP) 
        || (ts_type > UTIL_GLOBAL_ODU_TS_2G5))
    {
        PMC_LOG_TRACE("chnl = %u, server_type = %u, client_type = %u, mapping_mode = %u, ts_type = %u...\n", chnl, server_type, client_type, mapping_mode, ts_type);   
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG_TRACE("parent = %u...\n", ohfs_insert_handle->cfg.instance); 
    
    /* Initialize channel configuration bits - why set everything to 0?*/
    
    if(ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
    {
        ohfs_insert_chnl_default_init(ohfs_insert_handle, chnl, PMC_BLOCK_DEFAULT); 
    }
        
    /* Determine PT */
    ret_val = util_global_opu_pt_assign((util_global_parent_t)ohfs_insert_handle->cfg.instance, 
                                        server_type, client_type,
                                        mapping_mode, ts_type, (util_global_opu_payload_type_t*)&pt);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    } 
   
    /* If used by TGFPF */
    if (mapping_mode == UTIL_GLOBAL_GFP)
    {
        if (client_type == UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP)
        {
            ext_payload_type  = 1;
        }
    } 
    else if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
    {
        PMC_LOG_TRACE("instance:%u\n", ohfs_insert_handle->cfg.instance);   
        
        switch (mapping_mode)
        {
            case UTIL_GLOBAL_BMP:
                map_type = 0;
                max_slot_cnt = 1;
                break;
            case UTIL_GLOBAL_AMP:
                map_type = 1;
                max_slot_cnt = 1;
                break;
            case UTIL_GLOBAL_GMP:
                map_type = 4;
                max_slot_cnt = 1;
                break;
            case UTIL_GLOBAL_NO_MAP:
                PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
                break;
            default:
                PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
        } 
    }
    else if(ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_MUX)
    {
        PMC_LOG_TRACE("instance:%u\n", ohfs_insert_handle->cfg.instance);                         
        /* configure MAX_SLOT_NUM */
        ret_val =  util_global_max_server_num_ts_get((util_global_parent_t)ohfs_insert_handle->cfg.instance, server_type,
                                                     ts_type, &max_slot_cnt);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }                    
    }
    else
    {
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* configure PT value */
    if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
    {
        ohfs_insert_pt_cfg(ohfs_insert_handle, chnl, pt);
    }
    
    if (mapping_mode != UTIL_GLOBAL_GFP)
    {
        if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
        {
            /* configure PAD_CNT */ 
            pad_cnt = ohfs_insert_pad_cnt_calc(ohfs_insert_handle, chnl, server_type, mapping_mode, max_slot_cnt);
            ohfs_insert_buffer_init(ohfs_insert_buf, ohfs_insert_handle);
            ohfs_insert_field_PAD_CNT_set(ohfs_insert_buf, ohfs_insert_handle, chnl, pad_cnt);
            /* configure MAX_SLOT_CNT */
            ohfs_insert_field_MAX_SLOT_NUM_set(ohfs_insert_buf, ohfs_insert_handle, chnl, max_slot_cnt);
            ohfs_insert_buffer_flush(ohfs_insert_buf);
            
            /* configure fixed stuff */
            if (mapping_mode == UTIL_GLOBAL_AMP 
                || mapping_mode == UTIL_GLOBAL_BMP 
                || (mapping_mode == UTIL_GLOBAL_GMP && server_type == UTIL_GLOBAL_ODU4))
            { 
                ohfs_insert_fs_cfg(ohfs_insert_handle, chnl, server_type);    
            }
            /* disable stuff generation. This is added for reconfig test as above condition is not true in case we reconfig from bmp/amp to GMP*/
            else 
            { 
               ohfs_insert_field_STUFF_EN_set(NULL, ohfs_insert_handle, chnl, 0);
            }
        }
    }
    else 
    {
        if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
        {
            /* configure MAX_SLOT_CNT */
            ohfs_insert_field_MAX_SLOT_NUM_set(NULL, ohfs_insert_handle, chnl, 1);
        }
        /* configure ext payload type */
        ohfs_insert_field_EXT_PAYLOAD_TYPE_set(NULL, ohfs_insert_handle, chnl, ext_payload_type);                
    }
    
    if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
    {
        /* configure xoff WM level */
        ohfs_insert_xoff_wm_cfg(ohfs_insert_handle, chnl, mapping_mode, num_cal_entries);
    
        /* build CFC FIFO */
        ret_val = cfc_fifo_build(ohfs_insert_handle->cfc_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    }
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_chnl_prov */

/*******************************************************************************
* ohfs_insert_ho_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures HO channel based configurations in ODTU_MUX.
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*   server_type           - for MUX, HO ODU rate. See util_global_odukp_type_t. \n
*                           for TGMP, ODUkp or ODUjp type see odukp_type_t, 
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   ts_type               - tributary slot type. See util_global_odu_line_payload_t. \n
*                           NOTE: Only valid for MUX. This argument is 
*                           ignored in TGMP.
*   num_cal_entries       - number of calendar enrties required for the channel
*   offline_update        - Used for MUX only. Indication for programming 
*                           active/standby pages
*                           0 -- active page configuration
*                           1 -- offline page configuration
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ho_chnl_prov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                          UINT32 chnl,
                                          util_global_odukp_type_t server_type,
                                          util_global_odu_line_payload_t ts_type, 
                                          UINT32 num_cal_entries,
                                          BOOL8 offline_update)
{
    /* variable declaration */
    UINT32 ext_payload_type;
    UINT32 max_slot_cnt; 
    UINT32 map_type;
    UINT32 pad_cnt;
    UINT32 pt;
    PMC_ERROR ret_val = PMC_SUCCESS;  
    ohfs_insert_buffer_t ohfs_insert_buf[1];
    
    PMC_ENTRY();
    
    /* variable initialization */
    ext_payload_type = 0;
    map_type = 0;
    pad_cnt = 0;
    max_slot_cnt = 0;
    pt = 0;
    
    /* argument checking */
    if ((chnl > 95) || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || (ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD))
    {
        PMC_LOG_TRACE("chnl = %u, server_type = %u, ts_type = %u...\n", chnl, server_type, ts_type);   
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG_TRACE("parent = %u...\n", ohfs_insert_handle->cfg.instance); 
     
        
    /* Determine PT */
    ret_val = util_global_opu_pt_assign((util_global_parent_t)ohfs_insert_handle->cfg.instance, 
                                        server_type, UTIL_GLOBAL_ODUK_DONT_CARE,
                                        UTIL_GLOBAL_NO_MAP, ts_type, (util_global_opu_payload_type_t*)&pt);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    } 
   
    if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_MUX)
    {
        PMC_LOG_TRACE("instance:%u\n", ohfs_insert_handle->cfg.instance);                         
       
        /* configure MAX_SLOT_NUM */
        ret_val =  util_global_max_server_num_ts_get((util_global_parent_t)ohfs_insert_handle->cfg.instance, server_type,
                                                     ts_type, &max_slot_cnt);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        } 
        
        /* if ODUFlex is the server in FMF2 */
        if (server_type == UTIL_GLOBAL_ODUFLEX_CBR || server_type == UTIL_GLOBAL_ODUFLEX_GFP)
        {
            max_slot_cnt = num_cal_entries;
        }                   
    }
    else
    {
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }
    
    ohfs_insert_buffer_init(ohfs_insert_buf, ohfs_insert_handle);
    /* configure PT value */
    ohfs_insert_field_PT_set(ohfs_insert_buf, ohfs_insert_handle, chnl, pt);
     
    /* configure PAD_CNT */
    pad_cnt = ohfs_insert_pad_cnt_calc(ohfs_insert_handle, chnl, server_type, UTIL_GLOBAL_NO_MAP, max_slot_cnt);
    ohfs_insert_field_PAD_CNT_set(ohfs_insert_buf, ohfs_insert_handle, chnl, pad_cnt);
   
    /* configure MAX_SLOT_CNT */
    ohfs_insert_field_MAX_SLOT_NUM_set(ohfs_insert_buf, ohfs_insert_handle, chnl, max_slot_cnt);
    
    ohfs_insert_buffer_flush(ohfs_insert_buf);
    if (server_type == UTIL_GLOBAL_ODU4 && offline_update == FALSE)
    {
        ohfs_insert_fs_cfg(ohfs_insert_handle, chnl, server_type);    
    }
    
    if (offline_update == FALSE)
    {
        /* configure xoff WM level */
        ohfs_insert_xoff_wm_cfg(ohfs_insert_handle, chnl, UTIL_GLOBAL_NO_MAP, num_cal_entries);
    }
    
  
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_ho_chnl_prov */


/*******************************************************************************
*  ohfs_insert_cfc_fifo_end_addr_only_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OHFS_INSERT:CFC fifo end address config
*   
*
* INPUTS:
*   *ohfs_insert_handle     - pointer to OHFS_INSERT handle instance
*   chnl_id                 - channel ID to be configured
*                             Valid range: 0 - 95  
*   num_cal_entries         - size of the fifo
*                           
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 ohfs_insert_cfc_fifo_end_addr_only_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                     UINT32 chnl_id, UINT32 num_cal_entries)
{   
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    result = cfc_fifo_end_addr_only_cfg(ohfs_insert_handle->cfc_handle, 
                                        chnl_id, num_cal_entries);
    
    PMC_RETURN(result);
} /* ohfs_insert_cfc_fifo_end_addr_only_cfg */
                                         
/*******************************************************************************
* ohfs_insert_chnl_cfc_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the CFC fifos with a new number of calendar slots for the
*   specified channel. This function is used for G.HAO ODUflex resizing. The
*   function ohfs_insert_chnl_thresh_reprov() should be called just after
*   (ramp-up) or just before (ramp-down).
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   num_cal_entries       - number of calendar enrties required for the channel
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_chnl_cfc_reprov(ohfs_insert_handle_t *ohfs_insert_handle,
                                             UINT32 chnl,
                                             UINT32 num_cal_entries)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* argument checking */
    if (chnl > 95)
    {
        PMC_LOG_TRACE("chnl = %u...\n", chnl);   
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG_TRACE("parent = %u...\n", ohfs_insert_handle->cfg.instance);  

    /* build CFC FIFO */
    ret_val = cfc_fifo_resize(ohfs_insert_handle->cfc_handle,
                              chnl,
                              num_cal_entries);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_chnl_cfc_reprov */

/*******************************************************************************
* ohfs_insert_chnl_thresh_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the CFC fifo thresholds with a new number of calendar slots for
*   the specified channel. This function is used for G.HAO ODUflex resizing. The
*   function mapotn_tgfpf_chnl_cfc_reprov() should be called just before
*   (ramp-up) or just after (ramp-down).
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   mapping_mode          - mapping mode
*   num_cal_entries       - number of calendar enrties required for the channel
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_chnl_thresh_reprov(ohfs_insert_handle_t *ohfs_insert_handle,
                                                UINT32 chnl,
                                                util_global_mapping_mode_t mapping_mode,
                                                UINT32 num_cal_entries)
{
    PMC_ENTRY();

    /* argument checking */
    if (chnl > 95)
    {
        PMC_LOG_TRACE("chnl = %u...\n", chnl);
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG_TRACE("parent = %u...\n", ohfs_insert_handle->cfg.instance);

    /* configure xoff WM level */
    ohfs_insert_xoff_wm_cfg(ohfs_insert_handle, chnl, mapping_mode, num_cal_entries);

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_chnl_thresh_reprov */

/*******************************************************************************
* ohfs_insert_ho_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ho_chnl_deprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                            UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* configure ext payload type */
    ohfs_insert_field_EXT_PAYLOAD_TYPE_set(NULL, ohfs_insert_handle, chnl, 0);
    ohfs_insert_field_NULL_CHAN_EN_set(NULL, ohfs_insert_handle, chnl, 0);
    ohfs_insert_field_PAD_CNT_set(NULL, ohfs_insert_handle, chnl, 0);
    
    ohfs_insert_field_STUFF_EN_set(NULL, ohfs_insert_handle, chnl, 0);
    ohfs_insert_field_STUFF_MODE_set(NULL, ohfs_insert_handle, chnl, 0);
    ohfs_insert_field_STUFF_POS_SEL_set(NULL, ohfs_insert_handle, chnl, 0);
    ohfs_insert_field_STUFF_COL_POS_1_set(NULL, ohfs_insert_handle, chnl, 0);  
    ohfs_insert_field_STUFF_COL_POS_2_set(NULL, ohfs_insert_handle, chnl, 0);
    
    ohfs_insert_field_XOFF_WM_LEVEL_set(NULL, ohfs_insert_handle, chnl, 0);
    
    ret_val = ohfs_insert_pt_cfg(ohfs_insert_handle, chnl, 0);
    
    if (PMC_SUCCESS == ret_val)
    {
        ohfs_insert_field_MAX_SLOT_NUM_set(NULL, ohfs_insert_handle, chnl, 0);
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = ohfs_insert_omfi_cfg(ohfs_insert_handle, chnl, 0);
    }
                                                      
    PMC_RETURN(ret_val);
    
} /* ohfs_insert_ho_chnl_deprov */

/*******************************************************************************
* ohfs_insert_chnl_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_chnl_unprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* Destroy CFC FIFO */
    ret_val = cfc_fifo_destroy(ohfs_insert_handle->cfc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_chnl_unprov */

/*******************************************************************************
* ohfs_insert_lo_calendar_cfg_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   cal_entry             - calendar entry to be configured
*                           Valid range: 0 - 95    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_lo_calendar_cfg_deprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                    UINT32 cal_entry)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    ohfs_insert_buffer_t ohfs_insert_buf[1];

    PMC_ENTRY();
    
    ohfs_insert_buffer_init(ohfs_insert_buf, ohfs_insert_handle);
    ohfs_insert_field_RCOH_set(ohfs_insert_buf, ohfs_insert_handle, cal_entry, 0);
    ohfs_insert_buffer_flush(ohfs_insert_buf);
    
    ret_val = ohfs_insert_lo_ipt_cfg(ohfs_insert_handle, cal_entry, 0x7f, 0, 0);
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = ohfs_insert_ts_cfg(ohfs_insert_handle, cal_entry, 0, 0);
    }
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = ohfs_insert_chnl_mapping_cfg(ohfs_insert_handle, cal_entry, 0);
    }      
    
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = ohfs_insert_lo_chnl_id_cfg(ohfs_insert_handle, cal_entry, 0x7F);
    }  
    
    ohfs_insert_buffer_flush(ohfs_insert_buf);   
                                     
    PMC_RETURN(ret_val);
    
} /* ohfs_insert_lo_calendar_cfg_deprov */

/*******************************************************************************
* ohfs_insert_ho_calendar_cfg_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   cal_entry             - calendar entry to be configured
*                           Valid range: 0 - 95    
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ho_calendar_cfg_deprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                    UINT32 cal_entry)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = ohfs_insert_ho_ipt_cfg(ohfs_insert_handle, cal_entry, 0, 0, 0);
    
    PMC_RETURN(result);
} /* ohfs_insert_ho_calendar_cfg_deprov */

/*******************************************************************************
* ohfs_insert_ncs_en_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the NCS_EN register field for the specified
*   channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   ncs_en                - NCS_EN value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ncs_en_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                        UINT32 chnl,
                                        UINT32 ncs_en)
{
    /* variable declaration */

    PMC_ENTRY();

    ohfs_insert_field_NCS_EN_set ( NULL, ohfs_insert_handle, chnl, ncs_en );

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_ncs_en_set */


/*******************************************************************************
* ohfs_insert_rcoh_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the RCOH register field for the specified
*   channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   rcoh                  - RCOH value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_rcoh_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                      UINT32 chnl,
                                      UINT32 rcoh)
{
    /* variable declaration */

    PMC_ENTRY();

    ohfs_insert_field_RCOH_set( NULL, ohfs_insert_handle, chnl, rcoh);

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_rcoh_set */


/*******************************************************************************
* ohfs_insert_rp_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the RP register field for the specified
*   channel.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   rp                    - RP value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_rp_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                    UINT32 chnl,
                                    BOOL8 rp)
{
    /* variable declaration */

    PMC_ENTRY();

    ohfs_insert_field_RP_set( NULL, ohfs_insert_handle, chnl, rp);

    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_rp_set */


/*******************************************************************************
* ohfs_insert_csf_insert_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the CSF_INSERT configuration for the specified channel.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*
* OUTPUTS:
*   *csf_insert          - pointer to CSF_INSERT value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_csf_insert_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                    UINT32 chnl,
                                    UINT32 *csf_insert)
{
  /* variable declaration */
    
  PMC_ENTRY();
    
  *csf_insert = ohfs_insert_field_CSF_INSERT_get(NULL, ohfs_insert_handle, chnl);
     
  PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_csf_insert_get */


/*******************************************************************************
* ohfs_insert_lo_chnl_id_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the LO channel ID.
*
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95   
*   cycle                - calendar cycle which this channel ID belongs to
*                          Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_lo_chnl_id_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                            UINT32 cycle, UINT32 chnl)                                 
{
    /* variable declaration */
     
    PMC_ENTRY();
    
    ohfs_insert_field_LO_CHID_set(NULL, ohfs_insert_handle, cycle, chnl);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_lo_chnl_id_cfg */


/*******************************************************************************
* ohfs_insert_ho_ipt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures inverted page table that maps virtual addresses to 
*   physical addresses.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   cycle                 - calendar cycle to be configured
*                           Valid range: 0 - 95
*   chnl                  - channel ID which belongs to this cycle
*                           Valid range: 0 - 95
*   frm_num               - number of tributary slots in the HO channel
*   valid                 - qualifies the table entry \n
*                           0 -- table entry not valid \n
*                           1 -- table entry is valid
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ho_ipt_cfg(ohfs_insert_handle_t *ohfs_insert_handle,
                                        UINT32 cycle,
                                        UINT32 chnl,
                                        UINT32 frm_num,
                                        UINT32 valid)                                 
{
    /* variable declaration */
    ohfs_insert_buffer_t ohfs_insert_buffer[1];
    
    PMC_ENTRY();
    
    ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);

    if(1 == valid)
    {
        ohfs_insert_field_JC_RD_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, chnl);
        ohfs_insert_field_JC_RD_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, frm_num);
        ohfs_insert_buffer_flush(ohfs_insert_buffer);

        ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
        ohfs_insert_field_JC_RD_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, valid); 
        ohfs_insert_field_JC_RD_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, chnl);
        ohfs_insert_field_JC_RD_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, frm_num);
    } 
    else
    {
        ohfs_insert_field_JC_RD_IPT_VALID_set(NULL, ohfs_insert_handle, cycle, valid); 

        ohfs_insert_field_JC_RD_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, valid); 
        ohfs_insert_field_JC_RD_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, chnl);
        ohfs_insert_field_JC_RD_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, frm_num);
    }
    ohfs_insert_buffer_flush(ohfs_insert_buffer);
   
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_ho_ipt_cfg */

/*******************************************************************************
* ohfs_insert_lo_ipt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures inverted page table that maps virtual addresses to 
*   physical addresses.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   cycle                 - calendar cycle to be configured
*                           Valid range: 0 - 95
*   chnl                  - channel ID which belongs to this cycle
*                           Valid range: 0 - 95
*   frm_num               - number of tributary slots in the LO channel
*   valid                 - qualifies the table entry \n
*                           0 -- table entry not valid \n
*                           1 -- table entry is valid
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_lo_ipt_cfg(ohfs_insert_handle_t *ohfs_insert_handle,
                                        UINT32 cycle,
                                        UINT32 chnl,
                                        UINT32 frm_num,
                                        UINT32 valid)                                 
{
    /* variable declaration */
    ohfs_insert_buffer_t ohfs_insert_buffer[1];
    
    PMC_ENTRY();

    ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
    
    if(1 == valid)
    {
        ohfs_insert_field_JC_WR_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, chnl);
        ohfs_insert_field_JC_WR_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, frm_num);
        ohfs_insert_buffer_flush(ohfs_insert_buffer);

        ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
        ohfs_insert_field_JC_WR_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, valid); 
        ohfs_insert_field_JC_WR_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, chnl);
        ohfs_insert_field_JC_WR_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, frm_num);
    }
    else
    {
        ohfs_insert_field_JC_WR_IPT_VALID_set(NULL, ohfs_insert_handle, cycle, valid); 

        ohfs_insert_field_JC_WR_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, valid); 
        ohfs_insert_field_JC_WR_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, chnl);
        ohfs_insert_field_JC_WR_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, cycle, frm_num);
    }
    ohfs_insert_buffer_flush(ohfs_insert_buffer);
   
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_lo_ipt_cfg */

/*******************************************************************************
* ohfs_insert_ts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures trib slot based configurations for trib slot valid
*   and last trib slot.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   cycle                 - calendar cycle instance that corresponds to the  
*                           tributary slot
*                           Valid range: 0 - 95
*   last_ts               - indicates if a tributary slot for a given Channel
*                           is the last tributary slot \n
*                           0 -- not last trib slot \n
*                           1 -- last trib slot
*   valid                 - qualifies the table entry \n
*                           0 -- table entry not valid \n
*                           1 -- table entry is valid
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_ts_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                    UINT32 cycle,
                                    UINT32 last_ts,
                                    UINT32 valid)                                 
{
    /* variable declaration */
    ohfs_insert_buffer_t ohfs_insert_buf[1];
    
    PMC_ENTRY();
    
    ohfs_insert_buffer_init(ohfs_insert_buf, ohfs_insert_handle);
    ohfs_insert_field_TS_LAST_set(ohfs_insert_buf, ohfs_insert_handle, cycle, last_ts);
    ohfs_insert_field_TS_VALID_set(ohfs_insert_buf, ohfs_insert_handle, cycle, valid);
    ohfs_insert_buffer_flush(ohfs_insert_buf);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_ts_cfg */

/*******************************************************************************
* ohfs_insert_msi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one tributary slot in a LO
*   ODU channel. 
*
*
* INPUTS:
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   cycle               - Calendar cycle to be configured
*                         Valid range: 0 - 95
*   server_type         - HO ODU rate. See util_global_odukp_type_t.
*   client_type         - LO ODU rate. See util_global_odukp_type_t.
*   ts_type             - tributary slot type. See util_global_odu_line_payload_t.
*   ts_port             - tributary port number that the tributary slot will be
*                         transported in. Only valid if client_type is set to 
*                         ODUFLEX_CBR or ODUFLEX_GFP or if the tribuatry port
*                         assignment is flexible as in the following payload types. \n
*                          -- ODTU12 with 1.25G TS \n
*                          -- ODTU13 with 1.25G TS \n
*                          -- ODTU23 with 1.25G TS \n
*                          -- ODTU2.ts \n
*                          -- ODTU3.ts \n
*                          -- ODTU4.ts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_msi_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 cycle, 
                                     util_global_odukp_type_t server_type, 
                                     UINT32 client_type, 
                                     util_global_odu_line_payload_t ts_type, 
                                     UINT32 ts_port)                                 
{
    /* variable declaration */
    UINT32 msi_byte;
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (ohfs_insert_handle->cfg.msi_cfg_mode == UTIL_GLOBAL_OTN_MSI_CFG_AUTO)
    {
        ret_val = util_global_msi_cfg(ts_port, server_type, client_type, ts_type, 
                                      ts_port, &msi_byte);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
   
        ohfs_insert_field_MSI_set(NULL, ohfs_insert_handle, cycle, msi_byte);
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_msi_cfg */

/*******************************************************************************
* ohfs_insert_msi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one tributary slot in a LO
*   ODU channel. 
*
*
* INPUTS:
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   cycle               - cycle used for a LO ODU 
*                         Valid range: 0 - 95
*   msi_val             - MSI 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_msi_set(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 cycle, 
                                     UINT8 msi_val)                                 
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ohfs_insert_field_MSI_set(NULL, ohfs_insert_handle, cycle, msi_val);
    
    PMC_RETURN(result);

} /* ohfs_insert_msi_set */

/*******************************************************************************
* ohfs_insert_msi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one tributary slot in a LO
*   ODU channel. 
*
*
* INPUTS:
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   cycle               - channel ID of a LO ODU 
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *msi_val            - MSI byte of this LO channel
*
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_msi_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 cycle, 
                                     UINT8 *msi_val)                                 
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    *msi_val = ohfs_insert_field_MSI_get(NULL, ohfs_insert_handle, cycle);
    
    PMC_RETURN(result);

} /* ohfs_insert_msi_set */

/*******************************************************************************
* ohfs_insert_omfi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the OMFI overhead in OPU4 frame. This API is used
*   only for ODTU_MUX.
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - For TGMP, channel ID to be configured.
*                           For MUX, HO channel ID to be configured.
*                           Valid range: 0 - 95
*   omfi_insrt            - indicates if the OMFI overhead to be inserted for 
*                           the channel \n
*                           0 -- OMFI overhead is not inserted \n
*                           1 -- OMFI overhead is to be inserted for the channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_omfi_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                      UINT32 chnl, 
                                      UINT32 omfi_insrt)                                 
{
    /* variable declaration */
    PMC_ENTRY();
    
    /* argument checking */
      
    ohfs_insert_field_OMFI_INSERT_set(NULL, ohfs_insert_handle, chnl, omfi_insrt);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_omfi_cfg */

/*******************************************************************************
* ohfs_insert_null_frame_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function can be used to insert NULL frames into the channel. When 
*   NULL frame mode is enabled.  This function does not reconfigure the
*   payload type.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   null_frm_mode        - null frame mode \n
*                          0 -- channel operates normally to process incoming data \n
*                          1 -- NULL frames are inserted into the stream and
*                               incoming data is ignored for processing.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_null_frame_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                            UINT32 chnl,
                                            UINT32 null_frm_mode)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ohfs_insert_field_NULL_CHAN_EN_set(NULL, ohfs_insert_handle, chnl, null_frm_mode);

     
    PMC_RETURN(result);
    
} /* ohfs_insert_null_frame_cfg */



/*******************************************************************************
* ohfs_insert_chnl_mapping_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the mapping mode of a channel in the content 
*   memory. 
*
*
* INPUTS:
*   *ohfs_insert_handle   - pointer to OHFS_INSERT handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   mapping_mode          - the mapping mode of the channel \n
*                           0 -- BMP \n
*                           1 -- AMP \n
*                           2 -- ODTU13 \n
*                           3 -- ODTU2e3e1 \n
*                           4 -- GMP \n
*                           5 -- RESERVED \n
*                           6 -- Special Mode 1 \n
*                           7 -- Special Mode 2
*   
*   
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_chnl_mapping_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                              UINT32 chnl,
                                              UINT32 mapping_mode)
{
    /* variable declaration */
    
    PMC_ENTRY();
       
    ohfs_insert_field_MAP_TYPE_set(NULL, ohfs_insert_handle, chnl, mapping_mode);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_chnl_mapping_cfg */


/*******************************************************************************
* ohfs_insert_pt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the payload type of the ODTU payload.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   pt                   - payload type value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_pt_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                    UINT32 chnl,
                                    UINT8 pt)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    ohfs_insert_field_PT_set(NULL, ohfs_insert_handle, chnl, pt);
     
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_pt_cfg */


/*******************************************************************************
* ohfs_insert_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the payload type of the ODTU payload.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*
* OUTPUTS:
*   *pt                  - pointer to payload type value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_pt_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                    UINT32 chnl,
                                    UINT8 *pt)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    *pt = ohfs_insert_field_PT_get(NULL, ohfs_insert_handle, chnl);
     
    PMC_RETURN(PMC_SUCCESS);
    
} /* ohfs_insert_pt_get */


/*******************************************************************************
* ohfs_insert_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel.
*                                                                               
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                 
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR ohfs_insert_chnl_activate(ohfs_insert_handle_t *ohfs_insert_handle, 
                                           UINT32 chnl) 

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
     
    /* take the specified CFC FIFO out of reset */
    ret_val = cfc_fifo_activate(ohfs_insert_handle->cfc_handle,chnl);
    
    PMC_RETURN(ret_val);

} /* ohfs_insert_chnl_activate */


/*******************************************************************************
* ohfs_insert_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                               
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC PMC_ERROR ohfs_insert_chnl_deactivate(ohfs_insert_handle_t *ohfs_insert_handle, 
                                             UINT32 chnl) 
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
     
    /* put the specified CFC FIFO into reset */
    ret_val = cfc_fifo_deactivate(ohfs_insert_handle->cfc_handle,chnl);
   
    PMC_RETURN(ret_val);
   
} /* ohfs_insert_chnl_deactivate */

/*******************************************************************************
* ohfs_insert_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of OHFS_INSERT channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   *chnl               - set of channel ID to be cleaned
*   num_chnl            - number of channel ID to be cleaned
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void ohfs_insert_chnl_clean(ohfs_insert_handle_t *ohfs_insert_handle, 
                                   UINT32 *chnl,
                                   UINT32 num_chnl) 
{
    UINT32 chnl_itr, ipt_itr,valid, id;
    BOOL8   hit;
    ohfs_insert_buffer_t ohfs_insert_buffer[1];

    PMC_ENTRY();

    /* clean sub-blocks */
    cfc_fifo_clean(ohfs_insert_handle->cfc_handle,chnl,num_chnl);

    /* clean IPTs */
    for(ipt_itr=0; ipt_itr<OHFS_INSERT_MAX_CHNL_NUM; ipt_itr++)
    {
        /* HO IPT */
        valid = ohfs_insert_field_JC_RD_IPT_VALID_get(NULL, ohfs_insert_handle, ipt_itr); 
        id    = ohfs_insert_field_JC_RD_IPT_ID_get(NULL, ohfs_insert_handle, ipt_itr); 
        hit = FALSE;
        for(chnl_itr = 0; chnl_itr < num_chnl && hit == FALSE;chnl_itr++)
        {  
            if (1 == valid && id == chnl[chnl_itr])
            {
                hit = TRUE;
                ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
                ohfs_insert_field_JC_RD_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0x7F);
                ohfs_insert_field_JC_RD_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0);
                ohfs_insert_buffer_flush(ohfs_insert_buffer);
                ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
                ohfs_insert_field_JC_RD_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0x7F);
                ohfs_insert_field_JC_RD_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0);
                ohfs_insert_field_JC_RD_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0); 
                ohfs_insert_buffer_flush(ohfs_insert_buffer);
            }
        }

        /* LO IPT */
        valid = ohfs_insert_field_JC_WR_IPT_VALID_get(NULL, ohfs_insert_handle, ipt_itr); 
        id    = ohfs_insert_field_JC_WR_IPT_ID_get(NULL, ohfs_insert_handle, ipt_itr); 
        hit = FALSE;
        for(chnl_itr = 0; chnl_itr < num_chnl && hit == FALSE;chnl_itr++)
        {  
            if (1 == valid && id == chnl[chnl_itr])
            {
                hit = TRUE;
                ohfs_insert_field_JC_WR_IPT_VALID_set(NULL, ohfs_insert_handle, ipt_itr, 0); 
                ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
                ohfs_insert_field_JC_WR_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0x7F);
                ohfs_insert_field_JC_WR_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0);
                ohfs_insert_field_JC_WR_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0); 
                ohfs_insert_buffer_flush(ohfs_insert_buffer);
            }
        }
    }
    
    PMC_RETURN();

} /* ohfs_insert_chnl_clean */

/*******************************************************************************
* ohfs_insert_null_frame_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the NULL FRAME mode for a channel. 
*
* INPUTS:
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8               - TRUE: NULL frame mode is set\n
*                         FALSE:NULL frame mode is not set
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 ohfs_insert_null_frame_mode_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                             UINT32 chnl)
{
    BOOL8 null_frame_mode;

    PMC_ENTRY();
    
    null_frame_mode = ohfs_insert_field_NULL_CHAN_EN_get(NULL, ohfs_insert_handle, chnl);

    PMC_RETURN(null_frame_mode);

} /* ohfs_insert_null_frame_mode_get */

/*******************************************************************************
* ohfs_insert_reg_ctxt_recover
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function recovers a part of the OHFS_INSERT
*                                                                               
* INPUTS:                                                                       
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*                                                                 
* OUTPUTS:
*   *ohfs_insert_recover_reg - OHFS_INSERT reg context 
*
* RETURNS:
*   None.
*                                                                               
* NOTES:                                                                        
*                                                                               
****************************************************************************/
PUBLIC void ohfs_insert_reg_ctxt_recover(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         ohfs_insert_recover_var_t *ohfs_insert_recover_reg)
{
    UINT32 chnl_cycle_itr;

    PMC_ENTRY();

    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_INSERT_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        ohfs_insert_recover_reg->max_slot_num[chnl_cycle_itr]         = ohfs_insert_field_MAX_SLOT_NUM_get(NULL, ohfs_insert_handle, chnl_cycle_itr);
        ohfs_insert_recover_reg->has_fixed_stuff[chnl_cycle_itr]      = ohfs_insert_fixed_stuff_enable_get(ohfs_insert_handle,chnl_cycle_itr);

        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id     = ohfs_insert_field_JC_RD_IPT_ID_get(NULL, ohfs_insert_handle, chnl_cycle_itr);
        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_cycle_valid = ohfs_insert_field_JC_RD_IPT_VALID_get(NULL, ohfs_insert_handle, chnl_cycle_itr);
        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_frm_num     = ohfs_insert_field_JC_RD_IPT_VA_get(NULL, ohfs_insert_handle, chnl_cycle_itr);
        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_map_type    = ohfs_insert_field_MAP_TYPE_get(NULL, ohfs_insert_handle, chnl_cycle_itr);     
        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_chnl_id     = ohfs_insert_field_LO_CHID_get(NULL, ohfs_insert_handle, chnl_cycle_itr);
        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_last_ts     = ohfs_insert_field_TS_LAST_get(NULL, ohfs_insert_handle, chnl_cycle_itr);
        ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_ts_valid    = ohfs_insert_field_TS_VALID_get(NULL, ohfs_insert_handle, chnl_cycle_itr);        
    }
    
    PMC_RETURN();
} /* ohfs_insert_reg_ctxt_recover */

/*******************************************************************************
* ohfs_insert_reg_ctxt_ho_chnl_cfg_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the channel configuration based on the recovered 
*   register context 
*                                                                               
* INPUTS:                                                                       
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*   *ohfs_insert_recover_reg - OHFS_INSERT reg context 
*   chnl                     - Channel ID
*
* OUTPUTS:
*   *num_tribslot  - Number of trib slot for this channel 
*   *cal_array     - The calendar entries for this channel 
*
* RETURNS:
*   None. 
*                                                                               
* NOTES:                                                                        
*                                                                               
****************************************************************************/
PUBLIC void ohfs_insert_reg_ctxt_ho_chnl_cfg_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                 ohfs_insert_recover_var_t *ohfs_insert_recover_reg,
                                                 UINT32 chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS])
{
    UINT32 chnl_cycle_itr;
    UINT8 tribslot = 0;

    PMC_ENTRY();
    PMC_MEMSET(cal_array,0xFF,sizeof(UINT8)*UTIL_GLOBAL_MAX_NUM_TS);
    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_INSERT_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        if (ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_cycle_valid == 1 &&
            ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id == chnl)
        {
             /* if tribslot goes beyond 80, then assert */
            PMC_ASSERT(tribslot < UTIL_GLOBAL_MAX_NUM_TS, OHFS_INSERT_ERR_CODE_ASSERT, 0, 0);

            cal_array[tribslot] = chnl_cycle_itr;
            tribslot += 1;    
        }        
    }
    *num_tribslot = tribslot;
    PMC_RETURN();
} /* ohfs_insert_reg_ctxt_ho_chnl_cfg_get */

/*******************************************************************************
* ohfs_insert_reg_ctxt_lo_chnl_cfg_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the channel configuration based on the recovered 
*   register context 
*                                                                               
* INPUTS:                                                                       
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*   *ohfs_insert_recover_reg - OHFS_INSERT reg context 
*   lo_chnl                  - LO Channel ID
*
* OUTPUTS:
*   *num_tribslot  - Number of trib slot for this channel 
*   *mapping_mode  - The mapping mode for this channel 
*   *ho_chnl       - The HO channel associated to this channel
*   *cal_array     - The calendar entries for this channel 
*   *ts_array      - The lo tribslot entries for this channel 
*
* RETURNS:
*   None.
*                                                                               
* NOTES:                                                                        
*                                                                               
****************************************************************************/
PUBLIC void ohfs_insert_reg_ctxt_lo_chnl_cfg_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                 ohfs_insert_recover_var_t *ohfs_insert_recover_reg,
                                                 UINT8 lo_chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 *mapping_mode,
                                                 UINT8 *ho_chnl,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS],
                                                 UINT8 ts_array[UTIL_GLOBAL_MAX_NUM_TS])
{
    UINT32 chnl_cycle_itr;
    UINT8 lo_tribslot = 0;
    UINT8 ho_tribslot = 0;

    PMC_ENTRY();

    PMC_MEMSET(cal_array,0xFF,sizeof(UINT8)*UTIL_GLOBAL_MAX_NUM_TS);
    PMC_MEMSET(ts_array,0xFF,sizeof(UINT8)*UTIL_GLOBAL_MAX_NUM_TS);
    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_INSERT_MAX_CHNL_NUM;chnl_cycle_itr++)
    {

        if (ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_ts_valid == 1 &&
            ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_chnl_id == lo_chnl)
        {
            PMC_ASSERT(lo_tribslot < UTIL_GLOBAL_MAX_NUM_TS, OHFS_INSERT_ERR_CODE_ASSERT, 0, 0);
            cal_array[lo_tribslot] = chnl_cycle_itr;
            if (lo_tribslot == 0)
            {
                *ho_chnl = ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id;
                *mapping_mode  = ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_map_type;
            }
            lo_tribslot +=1;      
        }        
    }
    *num_tribslot = lo_tribslot;

    /* search trib slot ID for this LO channel */
    lo_tribslot = 0;
    ho_tribslot = 0;    
    for(chnl_cycle_itr = 0;chnl_cycle_itr < OHFS_INSERT_MAX_CHNL_NUM;chnl_cycle_itr++)
    {
        if (ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_cycle_valid == 1 &&
            ohfs_insert_recover_reg->cycle[chnl_cycle_itr].ho_chnl_id == *ho_chnl )
        {
            if (ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_ts_valid == 1 &&
                ohfs_insert_recover_reg->cycle[chnl_cycle_itr].lo_chnl_id == lo_chnl)
            {
                 /* if lo_tribslot goes beyond 80, then assert */
                PMC_ASSERT(lo_tribslot < UTIL_GLOBAL_MAX_NUM_TS, OHFS_INSERT_ERR_CODE_ASSERT, 0, 0);

                ts_array[lo_tribslot] = ho_tribslot;
                lo_tribslot += 1;          
            }            
            ho_tribslot += 1;
        }        
    }
    
    PMC_RETURN();
} /* ohfs_insert_reg_ctxt_lo_chnl_cfg_get */

/*******************************************************************************
* ohfs_insert_chnl_cycle_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of OHFS_INSERT channels and cycles (demux mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   *ho_chnl             - set of HO channel ID to be cleaned
*   num_ho_chnl          - number of channel ID to be cleaned
*   *lo_chnl             - set of channel ID to be cleaned
*   num_lo_chnl          - number of channel ID to be cleaned
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void ohfs_insert_chnl_cycle_clean(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 *ho_chnl,
                                         UINT32 num_ho_chnl,
                                         UINT32 *lo_chnl,
                                         UINT32 num_lo_chnl) 
{
    UINT32 ho_chnl_itr, lo_chnl_itr,ipt_itr, valid, id;
    BOOL8 hit;
    ohfs_insert_buffer_t ohfs_insert_buffer[1];

    PMC_ENTRY();
   
    /* clean sub-blocks */
    cfc_fifo_clean(ohfs_insert_handle->cfc_handle,ho_chnl,num_ho_chnl);
    

    /* Disable channel stuffs */
    for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl;ho_chnl_itr++)
    {
        ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
        ohfs_insert_field_STUFF_EN_set(ohfs_insert_buffer, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
        ohfs_insert_field_STUFF_MODE_set(ohfs_insert_buffer, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
        ohfs_insert_field_STUFF_POS_SEL_set(ohfs_insert_buffer, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
        ohfs_insert_field_STUFF_COL_POS_1_set(ohfs_insert_buffer, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);  
        ohfs_insert_field_STUFF_COL_POS_2_set(ohfs_insert_buffer, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);       
        ohfs_insert_buffer_flush(ohfs_insert_buffer);
        ohfs_insert_field_XOFF_WM_LEVEL_set(NULL, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
    }
                           
    
    /* clean HO IPT */
    for(ipt_itr=0; ipt_itr<OHFS_INSERT_MAX_CHNL_NUM; ipt_itr++)
    {
        valid = ohfs_insert_field_JC_RD_IPT_VALID_get(NULL, ohfs_insert_handle, ipt_itr); 
        hit = FALSE;
        if (1 == valid)
        {
            id    = ohfs_insert_field_JC_RD_IPT_ID_get(NULL, ohfs_insert_handle, ipt_itr); 
            for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl && hit == FALSE;ho_chnl_itr++)
            {  
                if (id == ho_chnl[ho_chnl_itr])
                {
                    hit = TRUE;
                    ohfs_insert_field_JC_RD_IPT_VALID_set(NULL, ohfs_insert_handle, ipt_itr, 0); 
                    ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
                    ohfs_insert_field_JC_RD_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0); 
                    ohfs_insert_field_JC_RD_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0x7F);
                    ohfs_insert_field_JC_RD_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0);
                    ohfs_insert_buffer_flush(ohfs_insert_buffer);
                }
            }
        }
    }
    /* clean HO IPT */
    for(ipt_itr=0; ipt_itr<OHFS_INSERT_MAX_CHNL_NUM; ipt_itr++)
    {
        valid = ohfs_insert_field_JC_WR_IPT_VALID_get(NULL, ohfs_insert_handle, ipt_itr); 
        if (1 == valid)
        {
             id    = ohfs_insert_field_JC_WR_IPT_ID_get(NULL, ohfs_insert_handle, ipt_itr); 
             hit = FALSE;
             for(lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl && hit == FALSE;lo_chnl_itr++)
             {  
                 if (id == lo_chnl[lo_chnl_itr])
                 {
                     hit = TRUE;
                     ohfs_insert_field_JC_WR_IPT_VALID_set(NULL, ohfs_insert_handle, ipt_itr, 0); 
                     ohfs_insert_buffer_init(ohfs_insert_buffer, ohfs_insert_handle);
                     ohfs_insert_field_JC_WR_IPT_VALID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0); 
                     ohfs_insert_field_JC_WR_IPT_ID_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0x7F);
                     ohfs_insert_field_JC_WR_IPT_VA_set(ohfs_insert_buffer, ohfs_insert_handle, ipt_itr, 0);
                     ohfs_insert_buffer_flush(ohfs_insert_buffer);
                 }
             }
        }
    }

    PMC_RETURN();

} /* ohfs_insert_chnl_cycle_clean */

/*******************************************************************************
* ohfs_insert_chnl_cycle_clean_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Gte the set of cycle to clean 
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   *ho_chnl             - set of HO channel ID to be cleaned
*   num_ho_chnl          - number of channel ID to be cleaned
*   *lo_chnl             - set of channel ID to be cleaned
*   num_lo_chnl          - number of channel ID to be cleaned
*
* OUTPUTS:
*   cleaned_cycle        - the list of cycle cleaned 
*   num_cleaned_cycle    - number of cycle to be cleaned
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void ohfs_insert_chnl_cycle_clean_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                             UINT32 *ho_chnl,
                                             UINT32 num_ho_chnl,
                                             UINT32 *lo_chnl,
                                             UINT32 num_lo_chnl,                                          
                                             UINT32 cleaned_cycle[],                                          
                                             UINT32 *num_cleaned_cycle) 
{
    UINT32 ho_chnl_itr, lo_chnl_itr,ipt_itr, valid, id, lo_id;
    BOOL8 hit;

    PMC_ENTRY();
   
                               
    *num_cleaned_cycle = 0;
    /* clean HO IPT */
    for(ipt_itr=0; ipt_itr<OHFS_INSERT_MAX_CHNL_NUM; ipt_itr++)
    {
        valid = ohfs_insert_field_JC_RD_IPT_VALID_get(NULL, ohfs_insert_handle, ipt_itr); 
        hit = FALSE;
        if (1 == valid)
        {
            id    = ohfs_insert_field_JC_RD_IPT_ID_get(NULL, ohfs_insert_handle, ipt_itr); 
            for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl && hit == FALSE;ho_chnl_itr++)
            {  
                if (id == ho_chnl[ho_chnl_itr])
                {
                    hit = TRUE;
                    
                    /* capture cleaned cycle */
                    cleaned_cycle[*num_cleaned_cycle] = ipt_itr;
                    *num_cleaned_cycle += 1;                    
                }
            }
            if (FALSE == hit)
            {                
                lo_id = ohfs_insert_field_LO_CHID_get(NULL, ohfs_insert_handle, ipt_itr);  
                for(lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl && hit == FALSE;lo_chnl_itr++)
                {
                    if (lo_id == lo_chnl[lo_chnl_itr])
                    {
                        cleaned_cycle[*num_cleaned_cycle] = ipt_itr;
                        *num_cleaned_cycle += 1;           
                        hit = TRUE;
                    }
                }   
                hit = FALSE;
            }
        }
    }   

    PMC_RETURN();

} /* ohfs_insert_chnl_cycle_clean_get */

/*******************************************************************************
* ohfs_insert_lo_cycle_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of OHFS_INSERT cycles (demux mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   *ho_chnl             - set of HO channel to be cleaned
*   num_ho_chnl          - number of HO channel to be cleaned
*   *cycle               - set of cycle to be cleaned
*   num_cycle            - number of cycle to be cleaned
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void ohfs_insert_lo_cycle_clean(ohfs_insert_handle_t *ohfs_insert_handle, 
                                       UINT32 *ho_chnl,
                                       UINT32 num_ho_chnl, 
                                       UINT32 *cycle,
                                       UINT32 num_cycle) 
{
    UINT32 cycle_itr, ho_chnl_itr;

    PMC_ENTRY();

    /* clean channel stuffs */
    for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl;ho_chnl_itr++)
    {

        ohfs_insert_field_EXT_PAYLOAD_TYPE_set(NULL, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
        ohfs_insert_field_NULL_CHAN_EN_set(NULL, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
        ohfs_insert_field_PAD_CNT_set(NULL, ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);      
        (void) ohfs_insert_pt_cfg(ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);
        (void) ohfs_insert_omfi_cfg(ohfs_insert_handle, ho_chnl[ho_chnl_itr], 0);

    }

    /* clean IPT */
    for(cycle_itr=0; cycle_itr<num_cycle; cycle_itr++)
    {             

        ohfs_insert_field_TS_VALID_set(NULL, ohfs_insert_handle, cycle[cycle_itr], 0);
        ohfs_insert_field_TS_LAST_set(NULL, ohfs_insert_handle, cycle[cycle_itr], 0);
        ohfs_insert_field_MAP_TYPE_set(NULL, ohfs_insert_handle, cycle[cycle_itr], 0);        
    }

    PMC_RETURN();

} /* ohfs_insert_lo_cycle_clean */

/*******************************************************************************
* ohfs_insert_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *ohfs_insert_handle - pointer to OHFS_INSERT handle instance
*   chnl_id             - Channel ID: Valid range: 0 - 95
*   int_table_ptr       - pointer to channel interrupt table with
*                         bits set to enable interrupts. If NULL, enables/disables
*                         all interrupts
*   int_en_table_ptr    - Pointer to table tracking enabled interrupts
*   enable              - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_int_chnl_enable(ohfs_insert_handle_t *ohfs_insert_handle,
                                             UINT32 chnl_id,
                                             ohfs_insert_int_chnl_t *int_table_ptr,
                                             ohfs_insert_int_chnl_t *int_en_table_ptr,
                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = ohfs_insert_int_validate(ohfs_insert_handle, chnl_id, int_en_table_ptr);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enable(ohfs_insert_handle->cfc_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc),
                                     &int_en_table_ptr->cfc,
                                     enable);
                                     
    }

    PMC_RETURN(result);
} /* ohfs_insert_int_chnl_enable */

/*******************************************************************************
* ohfs_insert_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *ohfs_insert_handle    - pointer to OHFS_INSERT handle instance
*   chnl_id                - Channel ID: Valid range: 0 - 95
*   int_table_ptr          - pointer to channel interrupt table with
*                            bits set to clear interrupts
*   int_en_table_ptr       - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_int_chnl_clear(ohfs_insert_handle_t *ohfs_insert_handle,
                                            UINT32 chnl_id,
                                            ohfs_insert_int_chnl_t *int_table_ptr,
                                            ohfs_insert_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = ohfs_insert_int_validate(ohfs_insert_handle, chnl_id, (void *)1);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(ohfs_insert_handle->cfc_handle,
                                    chnl_id,
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc),
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc));
                                     
    }
    
    PMC_RETURN(result);
} /* ohfs_insert_int_chnl_clear */

/*******************************************************************************
* ohfs_insert_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*   chnl_id                  - Channel ID: Valid range: 0 - 95
*   filt_table_ptr           - pointer to filter table. Mark fields as TRUE within this
*                              table to indicate that the interrupt/status will be retrieved
*                              to the output table. NULL to retrieve all 
*                              interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_int_chnl_retrieve(ohfs_insert_handle_t *ohfs_insert_handle,
                                               UINT32 chnl_id,
                                               ohfs_insert_int_chnl_t *filt_table_ptr,
                                               ohfs_insert_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = ohfs_insert_int_validate(ohfs_insert_handle, chnl_id, int_table_ptr);

     /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(ohfs_insert_handle->cfc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc),
                                       &int_table_ptr->cfc);
                                     
    }


    PMC_RETURN(result);
} /* ohfs_insert_int_chnl_retrieve */

/*******************************************************************************
* ohfs_insert_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *ohfs_insert_handle    - pointer to OHFS_INSERT handle instance
*   chnl_id                - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr       - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_int_chnl_enabled_check(ohfs_insert_handle_t *ohfs_insert_handle,
                                                    UINT32 chnl_id,
                                                    ohfs_insert_int_chnl_t *int_en_table_ptr,
                                                    BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = ohfs_insert_int_validate(ohfs_insert_handle, chnl_id, int_found_ptr);

     /* CFC */
    if (PMC_SUCCESS == result && 
        *int_found_ptr == FALSE)
    {
        result = cfc_int_chnl_enabled_check(ohfs_insert_handle->cfc_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc),
                                            int_found_ptr);
                                     
    }

    PMC_RETURN(result);
} /* ohfs_insert_int_chnl_enabled_check */

/*
** Private Functions
*/

/*******************************************************************************
* ohfs_insert_fs_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures inserting Fixed Stuff columns into the stream per 
*   channel based on the ODUk rate.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   oduk_type            - OPUk rate if used by TGMP or
*                          HO ODUk rate if ODTU_MUX 
*                          Refer to odukp_type_enum
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR ohfs_insert_fs_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 chnl,
                                     util_global_odukp_type_t oduk_type)
{
    /* variable declaration */
    UINT32 fs_col_pos_2;
    UINT32 fs_col_pos_1;   
    UINT32 fs_pos_sel; 
    UINT32 fs_mode;
    UINT32 fs_en;
    
    PMC_ENTRY();
    
    if(oduk_type == UTIL_GLOBAL_ODU1 || oduk_type == UTIL_GLOBAL_ODU1F 
       || oduk_type == UTIL_GLOBAL_ODU1E || oduk_type == UTIL_GLOBAL_ODU1E_ETRANS)
    {
        fs_col_pos_2 = 0;
        fs_col_pos_1 = 0;
        fs_pos_sel = 0;  
        fs_mode = 0;     
        fs_en= 0;       
    }
    else if (oduk_type == UTIL_GLOBAL_ODU2 || oduk_type == UTIL_GLOBAL_ODU2E 
             || oduk_type == UTIL_GLOBAL_ODU2E_ETRANS || oduk_type == UTIL_GLOBAL_ODU2F)
    {
        fs_col_pos_2 = 0;
        fs_col_pos_1 = 1904;
        fs_pos_sel = 0;  
        fs_mode = 1;     
        fs_en= 1;       
    }
    else if (oduk_type == UTIL_GLOBAL_ODU3 || oduk_type ==UTIL_GLOBAL_ODU3E1 
             || oduk_type == UTIL_GLOBAL_ODU3E2)
    {
        fs_col_pos_2 = 2544;
        fs_col_pos_1 = 1264;
        fs_pos_sel = 1;  
        fs_mode = 1;     
        fs_en= 1;       
    }
    else if (oduk_type == UTIL_GLOBAL_ODU4)
    {
        fs_col_pos_2 = 0;
        fs_col_pos_1 = 3816;
        fs_pos_sel = 0;  
        fs_mode = 0;     
        fs_en= 1;       
    }
    else if (oduk_type == UTIL_GLOBAL_ODUFLEX_CBR || oduk_type == UTIL_GLOBAL_ODUFLEX_GFP)
    {
        fs_col_pos_2 = 0;
        fs_col_pos_1 = 0;
        fs_pos_sel = 0;  
        fs_mode = 0;     
        fs_en= 0;       
    }
    else 
    {
        PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
    }
    
    ohfs_insert_field_STUFF_EN_set(NULL, ohfs_insert_handle, chnl, fs_en);
    ohfs_insert_field_STUFF_MODE_set(NULL, ohfs_insert_handle, chnl, fs_mode);
    ohfs_insert_field_STUFF_POS_SEL_set(NULL, ohfs_insert_handle, chnl, fs_pos_sel);
    ohfs_insert_field_STUFF_COL_POS_1_set(NULL, ohfs_insert_handle, chnl, fs_col_pos_1);  
    ohfs_insert_field_STUFF_COL_POS_2_set(NULL, ohfs_insert_handle, chnl, fs_col_pos_2);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_fs_cfg */


/*******************************************************************************
* ohfs_insert_pad_cnt_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function caclculates the Fixed Stuff columns into the stream per 
*   channel based on the ODUk rate.  It is assumed that the return value is used
*   to write the PAD_CNT register bit field immediately after being called.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   oduk_type            - OPUk rate if used by TGMP or
*                          HO ODUk rate if ODTU_MUX 
*                          Refer to util_global_odukp_type_t
*   mapping_mode         - mapping mode
*                          Refer to util_global_mapping_mode_t
*   num_ts               - number of tributary slots in the LO channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32               - The value to write into the PAD_CNT register bit field.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 ohfs_insert_pad_cnt_calc(ohfs_insert_handle_t *ohfs_insert_handle, 
                                        UINT32 chnl,
                                        util_global_odukp_type_t oduk_type,
                                        util_global_mapping_mode_t mapping_mode,
                                        UINT32 num_ts)
{
    /* variable declaration */
    UINT32 payload; 
    UINT32 fs_bytes;
    UINT32 pad_cnt_calc;
    UINT32 pad_cnt;
    
    /* variable initialization */
    payload = 15232;
    pad_cnt_calc = 0;
    pad_cnt = 0;
    fs_bytes = 0;
    
    
    PMC_ENTRY();
    if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_TGMP)
    {        
        if(mapping_mode == UTIL_GLOBAL_AMP || mapping_mode == UTIL_GLOBAL_BMP) 
        {
            if (oduk_type == UTIL_GLOBAL_ODU2 || oduk_type == UTIL_GLOBAL_ODU2E 
                || oduk_type == UTIL_GLOBAL_ODU2E_ETRANS || oduk_type == UTIL_GLOBAL_ODU2F)
            {
                fs_bytes = 64;    
            }
            else if (oduk_type == UTIL_GLOBAL_ODU3)
            {
                fs_bytes = 128;     
            }
            else 
            {
                fs_bytes = 0;       
            }
        }
        else if (mapping_mode == UTIL_GLOBAL_GMP)
        {
            if (oduk_type == UTIL_GLOBAL_ODU4)
            {
                fs_bytes = 32;       
            }
            else 
            {
                fs_bytes = 0;       
            }
        }
        else 
        {
            PMC_RETURN(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS);
        }  
    }
    else if (ohfs_insert_handle->cfg.instance == OHFS_INSERT_INSTANCE_MUX)
    {
        if (oduk_type == UTIL_GLOBAL_ODU4)
        {
            fs_bytes = 32;       
        }
        else 
        {
            fs_bytes = 0;       
        } 
    }

    pad_cnt_calc = ((payload - fs_bytes) * num_ts) % 48;
    
    if(pad_cnt_calc == 0) 
    {
        pad_cnt = 0;
    }
    else
    {
        pad_cnt = 48 - pad_cnt_calc;
    }
    

    
    PMC_RETURN(pad_cnt);

} /* ohfs_insert_pad_cnt_calc */


/*******************************************************************************
* ohfs_insert_xoff_wm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the Watermark level at which backpressure needs to
*   be given from OHFS INSERT.
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   mapping_mode         - Enum for mapping mode. See util_global_mapping_mode_t.
*   num_ts               - number of tributary slots in the HO channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_xoff_wm_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 chnl, 
                                         util_global_mapping_mode_t mapping_mode, 
                                         UINT32 num_ts)
{
    /* variable declaration */
    UINT32 xoff_wm_value;
    
    PMC_ENTRY();
    if (mapping_mode == UTIL_GLOBAL_GFP) 
    {
        xoff_wm_value = ((num_ts * 4) / 2) + 1;
    } else { 
       xoff_wm_value = ((num_ts * 2) + 2); 
    }
        
    PMC_LOG_TRACE("xoff_wm_value = %u\n", xoff_wm_value);   
    
    ohfs_insert_field_XOFF_WM_LEVEL_set(NULL, ohfs_insert_handle, chnl, xoff_wm_value);
    
    PMC_RETURN(PMC_SUCCESS);

} /* ohfs_insert_xoff_wm_cfg */

/*******************************************************************************
* ohfs_insert_dbg_mask_somf_sync_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures DBG_MASK_SOMF_SYNC to control frame alignment in
*   the empty HO channel in ODTU_MUX .
*
* INPUTS:
*   *ohfs_insert_handle  - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   somf_sync            - value of somf_sync 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR ohfs_insert_dbg_mask_somf_sync_set(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                    UINT32 chnl, UINT32 somf_sync)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();    

    ohfs_insert_field_DBG_MASK_SOMF_SYNC_set(NULL, ohfs_insert_handle, chnl, somf_sync);
    
    PMC_RETURN(result);
} /* ohfs_insert_dbg_mask_somf_sync_set */

/*******************************************************************************
* ohfs_insert_mapping_mode_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the mapping mode at a specified calendar cycle
*                                                                               
* INPUTS:                                                                       
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*   calendar_cycle           - The calendar cycle
*                                                                 
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_global_mapping_mode_t - The mapping mode
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC util_global_mapping_mode_t ohfs_insert_mapping_mode_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                                               UINT32 calendar_cycle)
{
    util_global_mapping_mode_t mode;
    UINT32 map_type;
    PMC_ENTRY();
    
    PMC_ASSERT(ohfs_insert_handle != NULL, OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    map_type = ohfs_insert_field_MAP_TYPE_get(NULL, ohfs_insert_handle, calendar_cycle);     
    if (map_type == 0)
    {
        mode = UTIL_GLOBAL_BMP;
    } else if (map_type == 1)
    {
        mode = UTIL_GLOBAL_AMP;
    } else if (map_type == 4)
    {
        mode = UTIL_GLOBAL_GMP;
    } else 
    {
        mode = UTIL_GLOBAL_NO_MAP;
    }

    PMC_RETURN(mode);
}  /* ohfs_insert_mapping_mode_get*/

/*******************************************************************************
* ohfs_insert_ext_payload_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the extended payload bit
*                                                                               
* INPUTS:                                                                       
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*   chnl                     - channel ID to be configured
*                              Valid range: 0 - 95
*                                                                 
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                   - The extended payload value
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC UINT32 ohfs_insert_ext_payload_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                          UINT32 chnl)
{
    UINT32 result;
    PMC_ENTRY();
    result = ohfs_insert_field_EXT_PAYLOAD_TYPE_get(NULL, ohfs_insert_handle, chnl);
    PMC_RETURN(result);
} /*ohfs_insert_ext_payload_get*/

/*******************************************************************************
* ohfs_insert_fixed_stuff_enable_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns TRUE when fixed stuff is enabled
*                                                                               
* INPUTS:                                                                       
*   *ohfs_insert_handle      - pointer to OHFS_INSERT handle instance
*   chnl                     - channel ID to be configured
*                              Valid range: 0 - 95
*                                                                 
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                    - TRUE: when fixed is enabled\n 
*                              FALSE: when it is not 
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8 ohfs_insert_fixed_stuff_enable_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                                UINT32 chnl)
{
    BOOL8 result = FALSE;
    UINT32 fs_en;
    PMC_ENTRY();
    fs_en = ohfs_insert_field_STUFF_EN_get(NULL, ohfs_insert_handle, chnl);

    if (fs_en == 1)
    {
        result = TRUE;        
    }
    PMC_RETURN(result);
} /* ohfs_insert_fixed_stuff_enable_get */


/*******************************************************************************
* ohfs_insert_msi_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets MSI configuration mode
*
*
* INPUTS:
*   *ohfs_insert_handle     - pointer to OHFS_INSERT handle instance
*   msi_mode                - Enum for MSI configuration mode. See util_global_otn_msi_cfg_mode_t.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_insert_msi_mode_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                     util_global_otn_msi_cfg_mode_t msi_mode)
{
    PMC_ENTRY();
    
    /* be default, MSI mode is auto */
    ohfs_insert_handle->cfg.msi_cfg_mode = msi_mode;
        
    PMC_RETURN();
} /* ohfs_insert_msi_mode_set */

/*******************************************************************************
* ohfs_insert_msi_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets MSI configuration mode
*
*
* INPUTS:
*   *ohfs_insert_handle     - pointer to OHFS_INSERT handle instance
*
* OUTPUTS:
*   *msi_mode               - current MSI configuration mode used in the device.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void ohfs_insert_msi_mode_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                     util_global_otn_msi_cfg_mode_t *msi_mode)
{
    PMC_ENTRY();
    
    /* be default, MSI mode is auto */
    *msi_mode = ohfs_insert_handle->cfg.msi_cfg_mode;
        
    PMC_RETURN();
} /* ohfs_insert_msi_mode_get */

/*******************************************************************************
* ohfs_insert_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *ohfs_insert_handle     - Pointer to OHFS_INSERT handle instance
*   chnl_id                 - channel ID to be configured
*                             Valid range: 0 - 95
*   non_null_ptr            - Pointer to check that shouldn't be NULL
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR ohfs_insert_int_validate(ohfs_insert_handle_t *ohfs_insert_handle,
                                           UINT32 chnl_id,
                                           void *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != ohfs_insert_handle, OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= ohfs_insert_handle->cfg.cfc_cfg.max_fifo_num)
    {
        result = OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* ohfs_insert_int_validate */

/*
** End of file
*/
