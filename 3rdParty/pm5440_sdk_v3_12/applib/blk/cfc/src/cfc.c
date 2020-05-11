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

#include "cfc_loc.h"
#include "cfc_io.h"
#include "cfc_io_inline.h"


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
const char CFC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    CFC_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char CFC_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE PMC_ERROR cfc_int_validate(cfc_handle_t *cfc_handle,
                                   UINT32 fifo_id,
                                   void *non_null_ptr);
PRIVATE PMC_ERROR cfc_int_chnl_enable_internal(cfc_handle_t *cfc_handle,
                                               UINT32 fifo_id,
                                               cfc_int_chnl_t *int_table_ptr,
                                               cfc_int_chnl_t *int_en_table_ptr,
                                               BOOL8 enable);

PRIVATE PMC_ERROR cfc_int_chnl_clear_internal(cfc_handle_t *cfc_handle,
                                              UINT32 fifo_id,
                                              cfc_int_chnl_t *int_table_ptr,
                                              cfc_int_chnl_t *int_en_table_ptr);
   
PRIVATE PMC_ERROR cfc_int_chnl_retrieve_internal(cfc_handle_t *cfc_handle,
                                                 UINT32 fifo_id,    
                                                 cfc_int_chnl_t *int_en_table_ptr,
                                                 cfc_int_chnl_t *int_table_ptr);
PRIVATE PMC_ERROR cfc_int_chnl_enabled_check_internal(cfc_handle_t *cfc_handle,
                                                      UINT32 fifo_d,
                                                      cfc_int_chnl_t *int_en_table_ptr,
                                                      BOOL8 *int_found_ptr);        
PRIVATE void cfc_ipt_entry_get_cfg(cfc_handle_t *cfc_handle, 
                                   UINT32 index,
                                   UINT32 *ipt_valid,
                                   UINT32 *ipt_id);
PRIVATE PMC_ERROR cfc_inverted_page_table_cfg(cfc_handle_t *cfc_handle,  
                                              UINT32 index,
                                              UINT32 ipt_valid,
                                              UINT32 ipt_id, 
                                              UINT32 ipt_va);                                         
PRIVATE PMC_ERROR cfc_inverted_page_table_var_init(cfc_handle_t *cfc_handle);                                                    
                                           
PRIVATE PMC_ERROR cfc_fifo_empty_page_get(cfc_handle_t *cfc_handle, 
                                          UINT32 num_pages, 
                                          UINT32 *available_page_array);                                                                                           
PRIVATE PMC_ERROR cfc_fifo_end_addr_cal(cfc_handle_t *cfc_handle, 
                                        UINT32 fifo_id,
                                        UINT32 num_pages, 
                                        UINT32 *end_addr_ptr);                                                                                                                                                                  
PRIVATE PMC_ERROR cfc_fifo_memory_allocate(cfc_handle_t *cfc_handle, 
                                           UINT32 fifo_id, 
                                           UINT32 num_current_pages, 
                                           UINT32 num_pages_requested, 
                                           UINT32 *available_page_array);
PRIVATE PMC_ERROR cfc_fifo_memory_deallocate(cfc_handle_t *cfc_handle, 
                                             UINT32 num_current_pages, 
                                             UINT32 num_pages_requested, 
                                             UINT32 *excess_page_array);                                                                                          
                                           
PRIVATE void cfc_ipt_page_mask_cfg(cfc_handle_t *cfc_handle);

PRIVATE void cfc_bypass_cfg(cfc_handle_t *cfc_handle, UINT32 bypass);
PRIVATE void cfc_ovr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                    UINT32 fifo_id,
                                    UINT32 enable);
PRIVATE void  cfc_udr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                     UINT32 fifo_id,
                                     UINT32 enable);
PRIVATE void cfc_ipt_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                    UINT32 fifo_id,
                                    UINT32 enable);
PRIVATE void cfc_ipt_entry_state_get_cfg(cfc_handle_t *cfc_handle, UINT32 index,
                                         UINT32 *ipt_valid);
PRIVATE PMC_ERROR cfc_fifo_overflow_mon_cfg(cfc_handle_t *cfc_handle, UINT32 fifo_id);
PRIVATE void cfc_fifo_end_addr_req_cfg(cfc_handle_t *cfc_handle, 
                                       UINT32 fifo_id,  
                                       UINT32 end_address);
PRIVATE UINT32 cfc_fifo_end_addr_req_get(cfc_handle_t *cfc_handle);
PRIVATE UINT32 cfc_fifo_end_addr_chg_success_get(cfc_handle_t *cfc_handle);
PRIVATE void cfc_fifo_end_addr_chg_abort_cfg(cfc_handle_t *cfc_handle);
PRIVATE void cfc_handle_init_internal(cfc_handle_t *cfc_handle, cfc_cfg_t *cfc_cfg_ptr, cfc_fifo_cfg_fct_table_t *fct_ptr_table, BOOL8 is_restart, util_global_restart_init_cfg_t *restart_init_cfg_ptr, pmc_energy_state_t top_energy_state);

/*
** Public Functions
*/

/*******************************************************************************
* cfc_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a CFC block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the CFC subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   cfc_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC cfc_handle_t *cfc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    cfc_handle_t *cfc_handle;

    PMC_ENTRY();

    cfc_handle = (cfc_handle_t *)PMC_CTXT_CALLOC(sizeof(cfc_handle_t), parent);
    
    pmc_handle_init(parent, cfc_handle, sys_handle, PMC_MID_DIGI_CFC, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(CFC_LOG_ERR_STRINGS[0], CFC_LOG_ERR_TABLE_BASE, CFC_LOG_ERR_COUNT);
    
    PMC_RETURN(cfc_handle);
    
} /* cfc_ctxt_create */


/*******************************************************************************
* cfc_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a CFC block instance.
*
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
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
PUBLIC void cfc_ctxt_destroy(cfc_handle_t *cfc_handle)
{
    PMC_ENTRY();

    if (NULL != cfc_handle->var.inverted_page_table)
    {
        PMC_CTXT_FREE(&(cfc_handle->var.inverted_page_table), cfc_handle);
    }

    PMC_CTXT_FREE(&(cfc_handle), cfc_handle);
        
    PMC_RETURN();
} /* cfc_ctxt_destroy */


/*******************************************************************************
* cfc_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an CFC block handle instance.
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
*   *cfc_cfg_ptr          - pointer to a config structure containing the values 
*                           used for initialization \n
*                           NOTE: this function will only modify the settings 
*                           for max_fifo_num and page_size of the config  
*                           structure from the parent block 
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
PUBLIC void cfc_handle_init(cfc_handle_t *cfc_handle, 
                            cfc_cfg_t *cfc_cfg_ptr)
{
    cfc_fifo_cfg_fct_table_t cfg_fct_table;

    /* Variable declaration */
    
    PMC_ENTRY();

    PMC_MEMSET((void *)&cfg_fct_table, 0, sizeof(cfc_fifo_cfg_fct_table_t));

    /* setup fct table with local functions */
    cfg_fct_table.cfc_fifo_end_addr_chg_abort_cfg_ptr     = cfc_fifo_end_addr_chg_abort_cfg;
    cfg_fct_table.cfc_fifo_end_addr_chg_success_get_ptr   = cfc_fifo_end_addr_chg_success_get;
    cfg_fct_table.cfc_fifo_end_addr_req_get_ptr           = cfc_fifo_end_addr_req_get;
    cfg_fct_table.cfc_fifo_end_addr_req_cfg_ptr           = cfc_fifo_end_addr_req_cfg;
    cfg_fct_table.cfc_fifo_overflow_mon_cfg_ptr           = cfc_fifo_overflow_mon_cfg;
    cfg_fct_table.cfc_ipt_entry_state_get_cfg_ptr         = cfc_ipt_entry_state_get_cfg;
    cfg_fct_table.cfc_ipt_entry_get_cfg_ptr               = cfc_ipt_entry_get_cfg;
    cfg_fct_table.cfc_ovr_int_enable_cfg_ptr              = cfc_ovr_int_enable_cfg;
    cfg_fct_table.cfc_udr_int_enable_cfg_ptr              = cfc_udr_int_enable_cfg;
    cfg_fct_table.cfc_ipt_int_enable_cfg_ptr              = cfc_ipt_int_enable_cfg;
    cfg_fct_table.cfc_bypass_cfg_ptr                      = cfc_bypass_cfg;
    cfg_fct_table.cfc_ipt_page_mask_cfg_ptr               = cfc_ipt_page_mask_cfg;
    cfg_fct_table.cfc_inverted_page_table_cfg_ptr         = cfc_inverted_page_table_cfg;
    cfg_fct_table.cfc_fifo_reset_cfg_ptr                  = cfc_fifo_reset_cfg;
    cfg_fct_table.cfc_int_chnl_enable_ptr                 = cfc_int_chnl_enable_internal;
    cfg_fct_table.cfc_int_chnl_clear_ptr                  = cfc_int_chnl_clear_internal;
    cfg_fct_table.cfc_int_chnl_retrieve_ptr               = cfc_int_chnl_retrieve_internal;
    cfg_fct_table.cfc_int_chnl_enabled_check_ptr          = cfc_int_chnl_enabled_check_internal;

    /* in this case, these fields are unused */
    cfg_fct_table.opaque                                  =  0;
    cfg_fct_table.ext_handle                              =  NULL;

    cfc_handle_init_internal(cfc_handle,cfc_cfg_ptr,&cfg_fct_table, FALSE, NULL, PMC_ENERGY_STATE_RESET);

    PMC_RETURN();
    
} /* cfc_handle_init */

/*******************************************************************************
* cfc_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function is purposefully empty as there is no local CFC memory allocated
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
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
PUBLIC void cfc_alloc_init(cfc_handle_t *cfc_handle)
{
    PMC_ENTRY();

    /*
     * There is nothing to do here as there is no non-context memory
     * allocated in this module.
     */
    PMC_RETURN();
}

/*******************************************************************************
* cfc_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an CFC block handle instance.
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
*   *cfc_cfg_ptr          - pointer to a config structure containing the values 
*                           used for initialization \n
*                           NOTE: this function will only modify the settings 
*                           for max_fifo_num and page_size of the config  
*                           structure from the parent block 
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state      - The expected energy state 
**
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void cfc_handle_restart_init(cfc_handle_t *cfc_handle, 
                                    cfc_cfg_t *cfc_cfg_ptr,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                    pmc_energy_state_t top_energy_state)
{
    cfc_fifo_cfg_fct_table_t cfg_fct_table;

    /* Variable declaration */
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(CFC_LOG_ERR_STRINGS[0], CFC_LOG_ERR_TABLE_BASE, CFC_LOG_ERR_COUNT);
    }

    PMC_MEMSET((void *)&cfg_fct_table, 0, sizeof(cfc_fifo_cfg_fct_table_t));

    /* setup fct table with local functions */
    cfg_fct_table.cfc_fifo_end_addr_chg_abort_cfg_ptr     = cfc_fifo_end_addr_chg_abort_cfg;
    cfg_fct_table.cfc_fifo_end_addr_chg_success_get_ptr   = cfc_fifo_end_addr_chg_success_get;
    cfg_fct_table.cfc_fifo_end_addr_req_get_ptr           = cfc_fifo_end_addr_req_get;
    cfg_fct_table.cfc_fifo_end_addr_req_cfg_ptr           = cfc_fifo_end_addr_req_cfg;
    cfg_fct_table.cfc_fifo_overflow_mon_cfg_ptr           = cfc_fifo_overflow_mon_cfg;
    cfg_fct_table.cfc_ipt_entry_state_get_cfg_ptr         = cfc_ipt_entry_state_get_cfg;
    cfg_fct_table.cfc_ipt_entry_get_cfg_ptr               = cfc_ipt_entry_get_cfg;
    cfg_fct_table.cfc_ovr_int_enable_cfg_ptr              = cfc_ovr_int_enable_cfg;
    cfg_fct_table.cfc_udr_int_enable_cfg_ptr              = cfc_udr_int_enable_cfg;
    cfg_fct_table.cfc_ipt_int_enable_cfg_ptr              = cfc_ipt_int_enable_cfg;
    cfg_fct_table.cfc_bypass_cfg_ptr                      = cfc_bypass_cfg;
    cfg_fct_table.cfc_ipt_page_mask_cfg_ptr               = cfc_ipt_page_mask_cfg;
    cfg_fct_table.cfc_inverted_page_table_cfg_ptr         = cfc_inverted_page_table_cfg;
    cfg_fct_table.cfc_fifo_reset_cfg_ptr                  = cfc_fifo_reset_cfg;
    cfg_fct_table.cfc_int_chnl_enable_ptr                 = cfc_int_chnl_enable_internal;
    cfg_fct_table.cfc_int_chnl_clear_ptr                  = cfc_int_chnl_clear_internal;
    cfg_fct_table.cfc_int_chnl_retrieve_ptr               = cfc_int_chnl_retrieve_internal;
    cfg_fct_table.cfc_int_chnl_enabled_check_ptr          = cfc_int_chnl_enabled_check_internal;
    /* in this case, these fields are unused */
    cfg_fct_table.opaque                                  =  0;
    cfg_fct_table.ext_handle                              =  NULL;

    cfc_handle_init_internal(cfc_handle,cfc_cfg_ptr,&cfg_fct_table, TRUE, restart_init_cfg_ptr, top_energy_state);

    PMC_RETURN();
    
} /* cfc_handle_restart_init */

/*******************************************************************************
* cfc_handle_init_ext_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an CFC block handle instance with external config functions
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
*   *cfc_cfg_ptr          - pointer to a config structure containing the values 
*                           used for initialization \n
*                           NOTE: this function will only modify the settings 
*                           for max_fifo_num and page_size of the config  
*                           structure from the parent block 
*   *cfg_fct_table_ptr    - pointer to a table of function used to configure the 
*                           CFC. This flavour shall be used when the register 
*                           mapping has been modified
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
PUBLIC void cfc_handle_init_ext_cfg(cfc_handle_t *cfc_handle, 
                                    cfc_cfg_t *cfc_cfg_ptr,
                                    cfc_fifo_cfg_fct_table_t *cfg_fct_table_ptr)
{
    
    PMC_ENTRY();

    cfc_handle_init_internal(cfc_handle,cfc_cfg_ptr,cfg_fct_table_ptr, FALSE, NULL, PMC_ENERGY_STATE_RESET);

    PMC_RETURN();
    
} /* cfc_handle_init_ext_cfg */

/*******************************************************************************
* cfc_handle_restart_init_ext_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an CFC block handle instance with external config functions
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
*   *cfc_cfg_ptr          - pointer to a config structure containing the values 
*                           used for initialization \n
*                           NOTE: this function will only modify the settings 
*                           for max_fifo_num and page_size of the config  
*                           structure from the parent block 
*   *cfg_fct_table_ptr    - pointer to a table of function used to configure the 
*                           CFC. This flavour shall be used when the register 
*                           mapping has been modified
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state      - The expected energy state 
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
PUBLIC void cfc_handle_restart_init_ext_cfg(cfc_handle_t *cfc_handle, 
                                            cfc_cfg_t *cfc_cfg_ptr,
                                            cfc_fifo_cfg_fct_table_t *cfg_fct_table_ptr,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state)
{
    
    PMC_ENTRY();

    cfc_handle_init_internal(cfc_handle,cfc_cfg_ptr,cfg_fct_table_ptr, TRUE, restart_init_cfg_ptr, top_energy_state);

    PMC_RETURN();
    
} /* cfc_handle_restart_init_ext_cfg */


/*******************************************************************************
* cfc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CFC instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to cfc_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  For example, if the parent module
*   were reset and the cfc fifo tracked by the cfc module was forced
*   to the reset state than the call to this API will force this context
*   to be coherent with this starting state.
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
*   energy_state          - enum defining state to set the CFC module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for CFC module 
*
*******************************************************************************/
PUBLIC PMC_ERROR cfc_init(cfc_handle_t *cfc_handle, pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(cfc_handle!=NULL, CFC_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        ret_val = cfc_inverted_page_table_init(cfc_handle); /* This is basically the non_defualt_cfg for CFC*/  
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        ret_val = cfc_inverted_page_table_var_init(cfc_handle);   
        
    } else {
        PMC_ASSERT(FALSE, CFC_ERR_INVALID_PARAMETERS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);

} /* cfc_init */


/*******************************************************************************
* cfc_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   cfc instance.  It will test the IPT table for all resources within
*   the cfc instance if any are not in CFC_PHYSICAL_PAGE_NIL state then
*   FALSE is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the cfc fifo and it would be
*   possible for the higher layer module to put the fifo in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
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
PUBLIC BOOL cfc_start_state_test(cfc_handle_t *cfc_handle)
{
    UINT32 i;
    PMC_ENTRY();
    PMC_ASSERT(cfc_handle!=NULL, CFC_ERR_INVALID_PARAMETERS, 0, 0);

    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++) 
    {
        /* Initialize var_t context */
        if(cfc_handle->var.inverted_page_table[i].ipt_id != CFC_PHYSICAL_PAGE_NIL)
            PMC_RETURN(FALSE);  
    }

    PMC_RETURN(TRUE);

} /* cfc_start_state_test */


/*******************************************************************************
*  cfc_fifo_build_end_addr
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function builds the FIFO based on the number of pages and a specified 
*   end address
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   num_pages           - number of pages needed for FIFO
*   end_addr            - depth of the fifo
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
PUBLIC PMC_ERROR cfc_fifo_build_end_addr(cfc_handle_t *cfc_handle, 
                                         UINT32 fifo_id, 
                                         UINT32 num_pages,
                                         UINT32 end_addr)
{   
    /* Variable declaration */
    UINT32 num_current_pages;
    UINT32 *available_page_array;
    PMC_ERROR ret_val = PMC_SUCCESS;

    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    available_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*cfc_handle->cfg.max_fifo_num);
    PMC_ASSERT(available_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, num_pages = %u\n", fifo_id, num_pages);
   
       
    /* get the available page addresses from the IPT and check if there is 
       enough available pages in the IPT for the num pages to be added */
    ret_val = cfc_fifo_empty_page_get(cfc_handle, num_pages, available_page_array);
    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&(available_page_array));
        PMC_RETURN(ret_val);  /* if not enough available pages return an error */
    }


        
    /* Retrieve number of pages in IPT for FIFO_ID to see if FIFO is already created */
    cfc_fifo_current_num_pages_get(cfc_handle, fifo_id, &num_current_pages);
    
    /* if FIFO already exist return error */
    if(num_current_pages != 0)
    {
        PMC_FREE(&(available_page_array));
        PMC_RETURN(CFC_ERR_FIFO_ALREADY_EXSITS);
    }
        
    /* Build the fifo and update the page usage state in memory */
    cfc_fifo_memory_allocate(cfc_handle, fifo_id, num_current_pages, num_pages, available_page_array);      
    
    /* configure the end address */
    ret_val = cfc_fifo_end_addr_cfg(cfc_handle, fifo_id, &end_addr, FALSE);
    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&(available_page_array));
        PMC_RETURN(ret_val);
    }
    
    PMC_FREE(&(available_page_array));
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_build_end_addr */


/*******************************************************************************
*  cfc_fifo_build
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function builds the FIFO based on the number of pages.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   num_pages           - number of pages needed for FIFO
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
PUBLIC PMC_ERROR cfc_fifo_build(cfc_handle_t *cfc_handle, 
                                UINT32 fifo_id, 
                                UINT32 num_pages)
{   
    /* Variable declaration */
    UINT32 fifo_end_addr=0;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    /* compute end addr */
    cfc_fifo_end_addr_cal(cfc_handle, fifo_id, num_pages, &fifo_end_addr);
    /* build FIFO */
    ret_val = cfc_fifo_build_end_addr(cfc_handle, fifo_id, num_pages, fifo_end_addr);

    PMC_RETURN(ret_val);  

} /* cfc_fifo_build */

/*******************************************************************************
*  cfc_fifo_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function destroys the FIFO and deallocates the physical pages that are
*   already allocated for the FIFO in the inverted page table. 
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
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
PUBLIC PMC_ERROR cfc_fifo_destroy(cfc_handle_t *cfc_handle, 
                                  UINT32 fifo_id)
{   
    /* Variable declaration */
    UINT32 num_current_pages;
    /*UINT32 fifo_end_addr;*/
    UINT32 *excess_page_array;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    excess_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*cfc_handle->cfg.max_fifo_num);
    PMC_ASSERT(excess_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);
    
    /* Retrive number of pages in IPT for FIFO_ID to see if FIFO is already
       created */
    cfc_fifo_current_num_pages_get(cfc_handle, fifo_id, &num_current_pages);
    
    /* if FIFO doesn't exist return error */
    if(num_current_pages == 0)
    {
        PMC_FREE(&(excess_page_array));
        PMC_RETURN(CFC_ERR_FIFO_NOT_BUILT);
    }
    
    /* get the indices of the excess pages that needs to deallocate */
    ret_val = cfc_fifo_excess_page_get(cfc_handle, fifo_id, 0, excess_page_array);
    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&(excess_page_array));
        PMC_RETURN(ret_val);  /* this is to catch resources unavailable */
    }   
    
    /* deallocate memory for the FIFO */
    cfc_fifo_memory_deallocate(cfc_handle, num_current_pages, 0, excess_page_array);
    
    PMC_FREE(&(excess_page_array));
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_destroy */

/*******************************************************************************
*  cfc_fifo_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deallocate memory and register of the the provide list of
*   FIFOs
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   *fifo_id            - array of FIFO ID
*   num_fifo_id         - number of FIFO to clean
*                         
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
PUBLIC void cfc_fifo_clean(cfc_handle_t *cfc_handle, 
                                UINT32 *fifo_id,
                                UINT32 num_fifo_id)
{   
    UINT32 ipt_itr, fifo_itr;
    UINT32 ipt_valid,ipt_id;

    PMC_ENTRY();
    
    for(fifo_itr = 0; fifo_itr < num_fifo_id; fifo_itr++)
    {
        (void) cfc_handle->cfg.cfg_fct_table.cfc_fifo_reset_cfg_ptr(cfc_handle, fifo_id[fifo_itr], 0);
    }

    for (ipt_itr = 0; ipt_itr<cfc_handle->cfg.max_fifo_num;ipt_itr++)
    {
        cfc_handle->cfg.cfg_fct_table.cfc_ipt_entry_get_cfg_ptr(cfc_handle,ipt_itr,&ipt_valid,&ipt_id);
        for(fifo_itr = 0; fifo_itr < num_fifo_id; fifo_itr++)
        {
            if (1 == ipt_valid && fifo_id[fifo_itr] == ipt_id)
            {
                cfc_handle->cfg.cfg_fct_table.cfc_inverted_page_table_cfg_ptr(cfc_handle, 
                                                                              ipt_itr, 
                                                                              CFC_PHYSICAL_PAGE_EMPTY, 
                                                                              CFC_PHYSICAL_PAGE_NIL, 
                                                                              CFC_PHYSICAL_PAGE_NIL);    
            }
#ifndef  CFC_REG_OP_CTXT_DISABLE                               
            /* Update var_t context */          
            if (cfc_handle->var.inverted_page_table[ipt_itr].ipt_valid == 1 && 
                cfc_handle->var.inverted_page_table[ipt_itr].ipt_id    == fifo_id[fifo_itr])
            {
                cfc_handle->var.inverted_page_table[ipt_itr].ipt_valid = CFC_PHYSICAL_PAGE_EMPTY;
                cfc_handle->var.inverted_page_table[ipt_itr].ipt_id = CFC_PHYSICAL_PAGE_NIL;
                cfc_handle->var.inverted_page_table[ipt_itr].ipt_va = CFC_PHYSICAL_PAGE_NIL;
            }
#endif /* CFC_REG_OP_CTXT_DISABLE */        

        }        
    }      

    PMC_RETURN();  

} /* cfc_fifo_clean */

/*******************************************************************************
*  cfc_fifo_resize
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function adjusts the number of pages used by an existing CFC FIFO.
*   Prior to calling a function must have been created using cfc_fifo_build.
*
*   This function may terminate with CFC_ERR_END_ADDR_UPDATE_INCOMPLETE.  This
*   is a recoverable failure.  The error occurs as data is not flowing through
*   fifo_id, for example if channel is backpressured by SIFD.  In such cases,
*   fifo_id is actively using the number of pages that was used at the time of
*   making this API call.
*
*   The function may be recalled to repeat the resize operation.  The function
*   may be called with num_pages as originally assigned to the fifo_id to
*   recover pages that were assigned to fifo_id by this API call.
*
*   Should calls to this API to modify the size of a FIFO pathologically fail,
*   a data path issue exists whereby data is not flowing through the FIFO.  At
*   the CFC level to force a resize of the FIFO call cfc_fifo_destroy followed
*   by cfc_fifo_build for the fifo_id.
*
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   num_pages           - number of pages needed for FIFO
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
*   This function and cfc_fifo_resize_ext are the same except that this 
*   function uses the default values for the end address change request 
*   polling interval (poll every US for a maximum of CFC_NUM_USEC_ITERATIONS).
*
*******************************************************************************/
PUBLIC PMC_ERROR cfc_fifo_resize(cfc_handle_t *cfc_handle, 
                                 UINT32 fifo_id, 
                                 UINT32 num_pages)
{ 
    PMC_ERROR ret_val = PMC_SUCCESS; 

    PMC_ENTRY();
    
    ret_val = cfc_fifo_resize_ext(cfc_handle,fifo_id,num_pages,1,CFC_NUM_USEC_ITERATIONS);

    PMC_RETURN(ret_val);  
} /* cfc_fifo_resize */

/*******************************************************************************
*  cfc_fifo_resize_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function adjusts the number of pages used by an existing CFC FIFO.
*   Prior to calling a function must have been created using cfc_fifo_build.
*
*   This function may terminate with CFC_ERR_END_ADDR_UPDATE_INCOMPLETE.  This
*   is a recoverable failure.  The error occurs as data is not flowing through
*   fifo_id, for example if channel is backpressured by SIFD.  In such cases,
*   fifo_id is actively using the number of pages that was used at the time of
*   making this API call.
*
*   The function may be recalled to repeat the resize operation.  The function
*   may be called with num_pages as originally assigned to the fifo_id to
*   recover pages that were assigned to fifo_id by this API call.
*
*   Should calls to this API to modify the size of a FIFO pathologically fail,
*   a data path issue exists whereby data is not flowing through the FIFO.  At
*   the CFC level to force a resize of the FIFO call cfc_fifo_destroy followed
*   by cfc_fifo_build for the fifo_id.
*
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   num_pages           - number of pages needed for FIFO
*   poll_interval_us    - Poll interval in US
*   poll_iterations     - Number of iterations 
*                         
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function is the same as cfc_fifo_resize except that the end address 
*   change request polling interval can be specified by the caller.
*
*******************************************************************************/
PUBLIC PMC_ERROR cfc_fifo_resize_ext(cfc_handle_t *cfc_handle, 
                                     UINT32 fifo_id, 
                                     UINT32 num_pages,
                                     UINT32 poll_interval_us,
                                     UINT32 poll_iterations)
{ 
    UINT32 num_current_pages;
    UINT32 fifo_end_addr=0;
    UINT32 *temp_page_array;
    PMC_ERROR ret_val = PMC_SUCCESS; 
    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, num_pages = %u\n", fifo_id, num_pages);
    
    temp_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*cfc_handle->cfg.max_fifo_num);
    PMC_ASSERT(temp_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);
    
    /* Retrive number of pages in IPT for fifo id */
    cfc_fifo_current_num_pages_get(cfc_handle, fifo_id, &num_current_pages);
    
    /* if FIFO doesn't exist return error */
    if(num_current_pages == 0)
    {
        PMC_FREE(&(temp_page_array));
        PMC_RETURN(CFC_ERR_FIFO_NOT_BUILT);
    }
    
    /* calculate the new end address */
    cfc_fifo_end_addr_cal(cfc_handle, fifo_id, num_pages, &fifo_end_addr); 
    
    
    /* 
     * Increase fifo size if num pages requested > current size
     *      -Add pages
     *      -Update end address
     * Decrease  fifo size if num_pages requested < current size
     *      -Update end address
     *      -Add pages
     *
     * End address Update 
     *      -Set end address requested
     *      -Set Request end address update
     *      -Wait for request to succeed or timeout 
     *          -If timeout - abort the end address update
     *          -Caller can retry
     */
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "current_pages = %u, num_pages = %u\n", num_current_pages, num_pages);
    if (num_current_pages < num_pages)
    {
        /* FIFO size increased */
    
        /* get the available page addresses from the IPT and check if there is 
           enough available pages in the IPT for the num pages to be added */
        ret_val = cfc_fifo_empty_page_get(cfc_handle, num_pages-num_current_pages, temp_page_array);

        /* Build the fifo and update the page usage state in memory */
        if (ret_val == PMC_SUCCESS)
        {
            cfc_fifo_memory_allocate(cfc_handle, fifo_id, num_current_pages, num_pages, temp_page_array);
    
            /* configure the end address */
            ret_val = cfc_fifo_end_addr_cfg_ext(cfc_handle, fifo_id, fifo_end_addr, poll_interval_us, poll_iterations, TRUE);
        }
    }   
    else if (num_current_pages > num_pages)
    {
        /* fifo size decreased - first configure the end address */
        ret_val = cfc_fifo_end_addr_cfg_ext(cfc_handle, fifo_id, fifo_end_addr, poll_interval_us, poll_iterations, TRUE);

        /* get the indices of the excess pages that need to be deallocated */
        if (ret_val == PMC_SUCCESS) 
            ret_val = cfc_fifo_excess_page_get(cfc_handle, fifo_id, num_pages, temp_page_array);
            
        /* deallocate memory for the FIFO */
        if (ret_val == PMC_SUCCESS) 
            cfc_fifo_memory_deallocate(cfc_handle, num_current_pages, num_pages, temp_page_array);
    } 
    /* An error will be returned if num_current_pages == num_pages */
    PMC_FREE(&(temp_page_array));
    PMC_RETURN(ret_val);  

} /* cfc_fifo_resize_ext */

/*******************************************************************************
*  cfc_fifo_resize_one_ts_step1
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function adjusts the number of pages used by an existing CFC FIFO.
*   Prior to calling a function must have been created using cfc_fifo_build.
*
*   This function should only be called when doing G.HAO flex resizing from
*   1 TS to n TS (ramp-up) or from n TS to 1 TS (ramp-down). It is used as a
*   work around for a CFC fifo issue that may prevent it to resize in some
*   instances when it has a single page.
*   In ramp-up, this function allocates the new IPT pages but doesn't do the
*   end address change, which is done in cfc_fifo_resize_one_ts_step2().
*   In ramp-down, this function does the end address change request, but
*   doesn't deallocate the unused IPT pages, which is done in
*   cfc_fifo_resize_one_ts_step2().
*
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   num_pages           - number of pages needed for FIFO
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
PUBLIC PMC_ERROR cfc_fifo_resize_one_ts_step1(cfc_handle_t *cfc_handle,
                                              UINT32 fifo_id,
                                              UINT32 num_pages)
{
    /* Variable declaration */
    UINT32 num_current_pages;
    UINT32 fifo_end_addr;
    UINT32 *temp_page_array;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG(PMC_LOG_SEV_HIGHEST, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, num_pages = %u\n", fifo_id, num_pages);

    temp_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*cfc_handle->cfg.max_fifo_num);
    PMC_ASSERT(temp_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);

    /* Retrive number of pages in IPT for FIFO_ID to see if FIFO is already
       created */
    cfc_fifo_current_num_pages_get(cfc_handle, fifo_id, &num_current_pages);

    /* calculate the new end address */
    cfc_fifo_end_addr_cal(cfc_handle, fifo_id, num_pages, &fifo_end_addr);

    /* if FIFO doesn't exist return error */
    if( num_current_pages == 0 )
    {
        PMC_FREE(&(temp_page_array));
        PMC_RETURN(CFC_ERR_FIFO_NOT_BUILT);
    }

    if( num_current_pages < num_pages )
    {
        /* Increase */
        PMC_LOG(PMC_LOG_SEV_HIGHEST, CFC_LOG_CODE_LOG, 0, 0, "num_current_pages = %u, num_pages = %u\n", num_current_pages, num_pages);

        /* Expect 1 page before resize */
        if( num_current_pages != 1 )
        {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
        }

        /* get the available page addresses from the IPT and check if there is
           enough available pages in the IPT for the num pages to be added */
        ret_val = cfc_fifo_empty_page_get(cfc_handle, num_pages-num_current_pages, temp_page_array);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(ret_val);
        }

        /* Build the fifo and update the page usage state in memory */
        cfc_fifo_memory_allocate(cfc_handle, fifo_id, num_current_pages, num_pages, temp_page_array);
    }
    else if( num_current_pages > num_pages )
    {
        /* Decrease */
        PMC_LOG(PMC_LOG_SEV_HIGHEST, CFC_LOG_CODE_LOG, 0, 0, "num_current_pages = %u, num_pages = %u\n", num_current_pages, num_pages);

        /* Expect 1 page after resize */
        if( num_pages != 1 )
        {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
        }

        /* configure the end address */
        ret_val = cfc_fifo_end_addr_cfg(cfc_handle, fifo_id, &fifo_end_addr,TRUE);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(ret_val);  /* if not enough available pages return an error */
        }
    }
    else
    {
        PMC_FREE(&(temp_page_array));
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }

    PMC_FREE(&(temp_page_array));
    PMC_RETURN(PMC_SUCCESS);

} /* cfc_fifo_resize_one_ts_step1 */

/*******************************************************************************
*  cfc_fifo_resize_one_ts_step2
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function adjusts the number of pages used by an existing CFC FIFO.
*   Prior to calling a function must have been created using cfc_fifo_build.
*
*   This function should only be called when doing G.HAO flex resizing from
*   1 TS to n TS (ramp-up) or from n TS to 1 TS (ramp-down). It is used as a
*   work around for a CFC fifo issue that may prevent it to resize in some
*   instances when it has a single page. This function does the end address change request, assuming the IPT pages
*   have already been initialized by cfc_fifo_resize_from_one_ts_step1().
*   In ramp-up, this function does the end address change request, assuming
*   the IPT pages have already been initialized by
*   cfc_fifo_resize_one_ts_step1().
*   In ramp-down, this function does the IPT pages deallocation, assuming
*   the end address change request has been done by
*   cfc_fifo_resize_one_ts_step1()
*
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   num_pages           - number of pages needed for FIFO
*   num_current_pages   - number of pages initially allocated needed for FIFO
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
PUBLIC PMC_ERROR cfc_fifo_resize_one_ts_step2(cfc_handle_t *cfc_handle,
                                              UINT32 fifo_id,
                                              UINT32 num_pages,
                                              UINT32 num_current_pages)
{
    /* Variable declaration */
    UINT32 fifo_end_addr;
    UINT32 *temp_page_array;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG(PMC_LOG_SEV_HIGHEST, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, num_pages = %u\n", fifo_id, num_pages);

    temp_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*cfc_handle->cfg.max_fifo_num);
    PMC_ASSERT(temp_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);

    /* calculate the new end address */
    cfc_fifo_end_addr_cal(cfc_handle, fifo_id, num_pages, &fifo_end_addr);


    /* Expect increase */
    if( num_current_pages < num_pages )
    {
        PMC_LOG(PMC_LOG_SEV_HIGHEST, CFC_LOG_CODE_LOG, 0, 0, "num_current_pages = %u, num_pages = %u\n", num_current_pages, num_pages);

        /* Expect 1 page before resize */
        if( num_current_pages != 1 )
        {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
        }

        /* configure the end address */
        ret_val = cfc_fifo_end_addr_cfg(cfc_handle, fifo_id, &fifo_end_addr, TRUE);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(ret_val);  /* if not enough available pages return an error */
        }
    }
    else if( num_current_pages > num_pages )
    {
        /* Decrease */
        PMC_LOG(PMC_LOG_SEV_HIGHEST, CFC_LOG_CODE_LOG, 0, 0, "num_current_pages = %u, num_pages = %u\n", num_current_pages, num_pages);

        /* Expect 1 page after resize */
        if( num_pages != 1 )
        {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
        }

        /* get the indices of the excess pages that needs to deallocate */
        ret_val = cfc_fifo_excess_page_get(cfc_handle, fifo_id, num_pages, temp_page_array);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&(temp_page_array));
            PMC_RETURN(ret_val);
        }

        /* deallocate memory for the FIFO */
        cfc_fifo_memory_deallocate(cfc_handle, num_current_pages, num_pages, temp_page_array);
    }
    else
    {
        PMC_FREE(&(temp_page_array));
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }

    PMC_FREE(&(temp_page_array));
    PMC_RETURN(PMC_SUCCESS);

} /* cfc_fifo_resize_one_ts_step2 */

/*******************************************************************************
*  cfc_inverted_page_table_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the inverted page table valid bit to zero and 
*   IPT_ID and IPT_VA to all ones
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
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
PUBLIC PMC_ERROR cfc_inverted_page_table_init(cfc_handle_t *cfc_handle)
{   
    
    /* Variable declaration */
    UINT32 i;
    
    PMC_ENTRY();

    cfc_handle->cfg.cfg_fct_table.cfc_ipt_page_mask_cfg_ptr(cfc_handle);
    
    /* Initialize valid bit and IPT entries */
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)
    {
        cfc_handle->cfg.cfg_fct_table.cfc_inverted_page_table_cfg_ptr(cfc_handle, i, CFC_PHYSICAL_PAGE_EMPTY, CFC_PHYSICAL_PAGE_NIL, CFC_PHYSICAL_PAGE_NIL);    
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_inverted_page_table_init */


/*******************************************************************************
*  cfc_bypass
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IPT to bypass mode or not.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   bypass            - bypass mode \n
*                       0 -- the physical addresses returned by the IPT are used
*                            to drive the write/read address outputs. \n
*                       1 -- the write/read addresses of the controller are used 
*                            to drive the write/read address outputs. \n
*                       This signal must remain static after a rest of the FIFO
*                       controller. 
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
PUBLIC PMC_ERROR cfc_bypass(cfc_handle_t *cfc_handle, UINT32 bypass)
{   
    
    PMC_ENTRY();
    
    /* Apply field settings */
    cfc_handle->cfg.cfg_fct_table.cfc_bypass_cfg_ptr(cfc_handle, bypass);
    
    PMC_RETURN(PMC_SUCCESS); 

} /* cfc_bypass */


/*******************************************************************************
*  cfc_fifo_end_addr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs an end address update for fifo_id.  It will
*   monitor the success of the update and if unsuccessul abort the update.
*
*   If aborted CFC_ERR_END_ADDR_UPDATE_INCOMPLETE will be returned.  This 
*   function terminates access to the FIFO Controller Configuration Interface
*   registers for the fifo_id.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   *end_addr_ptr       - pointer to the end address that needs to be configured
*   resize_mode         - TRUE: we poll for change address completion\n
*                         FALSE: we are not polling for change address completion
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   -cfc_fifo_end_addr_cfg_ext is the same as the previous version of 
*    cfc_fifo_end_addr_cfg except except that the polling interval & number of 
*    iterations is configurable. 
*
*******************************************************************************/
PUBLIC PMC_ERROR cfc_fifo_end_addr_cfg(cfc_handle_t *cfc_handle, 
                                       UINT32 fifo_id,  
                                       UINT32 *end_addr_ptr,
                                       BOOL8 resize_mode)
{     
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    result = cfc_fifo_end_addr_cfg_ext(cfc_handle, 
                                       fifo_id,  
                                       *end_addr_ptr,
                                       1,
                                       CFC_NUM_USEC_ITERATIONS,
                                       resize_mode);
    PMC_RETURN(result);
} /* cfc_fifo_end_addr_cfg */

/*******************************************************************************
*  cfc_fifo_end_addr_cfg_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs an end address update for fifo_id. In resizing mode,
*   it is required to poll for change address completion. In this case, it
*   monitor the success of the update and if unsuccessul abort the update.
*
*   If aborted CFC_ERR_END_ADDR_UPDATE_INCOMPLETE will be returned.  This 
*   function terminates access to the FIFO Controller Configuration Interface
*   registers for the fifo_id.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   end_address         - pointer to the end address that needs to be configured
*   poll_interval_us    - Polling interval in US
*   poll_iterations     - Number of polling iterations 
*   resize_mode         - TRUE: we poll for change address completion\n
*                         FALSE: we are not polling for change address completion
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   -This is the same as the previous version cfc_fifo_end_addr_cfg except
*    that the polling interval & number of iterations is configurable.
*
*******************************************************************************/
PUBLIC PMC_ERROR cfc_fifo_end_addr_cfg_ext(cfc_handle_t *cfc_handle, 
                                           UINT32 fifo_id,  
                                           UINT32 end_address,
                                           UINT32 poll_interval_us,
                                           UINT32 poll_iterations,
                                           BOOL8 resize_mode)
{     
    UINT32 timeout_iterations;
    UINT32 end_addr_chg_req;
    UINT32 end_addr_chg_success;

    PMC_ENTRY();

    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    cfc_handle->cfg.cfg_fct_table.cfc_fifo_end_addr_req_cfg_ptr( cfc_handle, fifo_id,end_address);
    
    if (FALSE == resize_mode)
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    /* poll the END_ADDR_CHG_REQ bit */
    for (timeout_iterations = 0; timeout_iterations <= poll_iterations; timeout_iterations++)                                                 
    {
        end_addr_chg_req = cfc_handle->cfg.cfg_fct_table.cfc_fifo_end_addr_req_get_ptr(cfc_handle);

        if (end_addr_chg_req  == FALSE) /* successful case */
        {
            PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "end address success: end_addr_chg_req = %u\n", end_addr_chg_req);
            
            /* end addrss change was successful*/
            /* check FIFO_END_ADDR_CHG_SUCCESS bit */
            
            end_addr_chg_success = cfc_handle->cfg.cfg_fct_table.cfc_fifo_end_addr_chg_success_get_ptr(cfc_handle);
            if (end_addr_chg_success == TRUE) /* end address change was successful */
            { 
                PMC_RETURN(PMC_SUCCESS);
            } 
            else  /* end address change invalid device state detected */
            {               
                PMC_RETURN(CFC_ERR_CODE_ASSERT);            
            }
        } 
        else if (end_addr_chg_req  == TRUE  && timeout_iterations == CFC_NUM_USEC_ITERATIONS) /* end address update failed to terminate */
        {
            PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "end address abort: end_addr_chg_req = %u\n", end_addr_chg_req);
            
            /* force termination of end address update */    
            /* end address change was Unsuccessful */
            /* abort the change request */
            cfc_handle->cfg.cfg_fct_table.cfc_fifo_end_addr_chg_abort_cfg_ptr(cfc_handle);
            
            /* check FIFO_END_ADDR_CHG_SUCCESS bit */
            end_addr_chg_success = cfc_handle->cfg.cfg_fct_table.cfc_fifo_end_addr_chg_success_get_ptr(cfc_handle);
            
            if (end_addr_chg_success == TRUE) /* end address change was successful */
            {
                PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "end address success after abort: end_addr_chg_success = %u\n", end_addr_chg_success);
                PMC_RETURN(PMC_SUCCESS);
            } 
            else /* end address change failure confirmed */
            { 
#if defined (PMC_SW_SIMULATION)
                PMC_RETURN(PMC_SUCCESS);
#else                
                PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "end address incomplete after abort: end_addr_chg_success = %u\n", end_addr_chg_success);
                PMC_RETURN(CFC_ERR_END_ADDR_UPDATE_INCOMPLETE);                           
#endif
            }
            
            /* need to remove the below fifo clear bit operation.  We are controlling this bit from build/destroy cfc fifo calls.
            action - need to add this to the destroy fifo procedure - email to pat on confirmation of destroy procedure.*/
            /* clear the FIFO via FIFO_CLRB */
            /* 
            cfc_handle->cfg.cfg_fct_table.cfc_fifo_reset_cfg_ptr(cfc_handle, fifo_id, 0); //why are we doing this, if we fail, we still have the old fifo working okay....
            */         
        }
        
#if !defined(PMC_SW_SIMULATION)
        PMC_Q_USLEEP(cfc_handle,poll_interval_us);
#endif
    }
    
    /* Execution doesn't get here */ 
    PMC_RETURN(PMC_SUCCESS); 

} /* cfc_fifo_end_addr_cfg_ext */


/*******************************************************************************
*  cfc_ipt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the inverted page table for the channelized FIFO
*   controller that maps virtual block addresses to physical block addresses.
*   This function is only used for ODUJAT_CFC in COREOTN.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   ipt_valid           - qualifies the inverted page table entry \n
*                         0 -- the corresponding FIFO ID specified in IPT_ID is 
*                              invalid \n
*                         1 -- the corresponding FIFO ID specified in IPT_ID is 
*                              valid
*   ipt_id              - channel ID associated with the physical block.
*   ipt_va              - the block number associated with the physical block.
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
PUBLIC PMC_ERROR cfc_ipt_cfg(cfc_handle_t *cfc_handle, UINT32 index, 
                             UINT32 ipt_valid, UINT32 ipt_id, UINT32 ipt_va)
{   
    PMC_ENTRY();
    
    cfc_handle->cfg.cfg_fct_table.cfc_inverted_page_table_cfg_ptr(cfc_handle, index, ipt_valid, ipt_id, ipt_va);
    
    PMC_RETURN(PMC_SUCCESS); 

} /* cfc_ipt_cfg */


/*******************************************************************************
*  cfc_fifo_end_addr_only_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CFC fifo end address config
*   
*
* INPUTS:
*   *cfc_handle     - pointer to CFC handle instance
*   chnl_id         - channel id
*   num_cal_entries - size of the fifo
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
PUBLIC UINT32 cfc_fifo_end_addr_only_cfg(cfc_handle_t *cfc_handle, 
                                         UINT32 chnl_id, UINT32 num_cal_entries)
{   
    /* Variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 end_addr_ptr;
   
    PMC_ENTRY();
    
    /* calculate the end address */
    ret_code = cfc_fifo_end_addr_cal(cfc_handle, chnl_id, num_cal_entries, &end_addr_ptr);
    
    
    if (PMC_SUCCESS == ret_code)
    {
        /* configure the end address */
        ret_code = cfc_fifo_end_addr_cfg(cfc_handle, chnl_id, &end_addr_ptr, FALSE);
    }     
    
    PMC_RETURN(ret_code);

} /* cfc_fifo_end_addr_only_cfg */         


/*******************************************************************************
*  cfc_fifo_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function takes the specified FIFO out of reset
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID                     
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
PUBLIC PMC_ERROR cfc_fifo_activate(cfc_handle_t *cfc_handle, UINT32 fifo_id)
{ 
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY(); 
    
    ret_val = cfc_handle->cfg.cfg_fct_table.cfc_fifo_reset_cfg_ptr(cfc_handle, fifo_id, 1);
    
    PMC_RETURN(ret_val);
} /* cfc_fifo_activate */

/*******************************************************************************
*  cfc_ipt_entry_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the current state of an IPT entry and returns whether
*   or not the entry is occupied. 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   *ipt_valid          - qualifies the inverted page table entry
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
PUBLIC PMC_ERROR cfc_ipt_entry_state_get(cfc_handle_t *cfc_handle, UINT32 index,
                                         UINT32 *ipt_valid)
{   
    PMC_ENTRY();
    cfc_handle->cfg.cfg_fct_table.cfc_ipt_entry_state_get_cfg_ptr(cfc_handle,index,ipt_valid);
    PMC_RETURN(PMC_SUCCESS);
} /* cfc_ipt_entry_state_get */


/*******************************************************************************
*  cfc_ipt_entry_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the current state of an IPT entry and returns whether
*   or not the entry is occupied. 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   *ipt_valid          - qualifies the inverted page table entry
*   *ipt_id             - The channle id associated with this entry (only valid 
*                         when ipt_valid is set)
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
PUBLIC PMC_ERROR cfc_ipt_entry_get(cfc_handle_t *cfc_handle, 
                                   UINT32 index,
                                   UINT32 *ipt_valid,
                                   UINT32 *ipt_id)
{   
    PMC_ENTRY();
    
    cfc_handle->cfg.cfg_fct_table.cfc_ipt_entry_get_cfg_ptr(cfc_handle,index,ipt_valid,ipt_id);
    PMC_RETURN(PMC_SUCCESS);

} /* cfc_ipt_entry_get*/


/*******************************************************************************
*  cfc_fifo_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function puts the specified FIFO into reset
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID  
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
PUBLIC PMC_ERROR cfc_fifo_deactivate(cfc_handle_t *cfc_handle, UINT32 fifo_id)
{   
    PMC_ERROR ret_val = PMC_SUCCESS;
    PMC_ENTRY(); 
   
    ret_val = cfc_handle->cfg.cfg_fct_table.cfc_fifo_reset_cfg_ptr(cfc_handle, fifo_id, 0);
    
    PMC_RETURN(ret_val);
} /* cfc_fifo_deactivate */

/*******************************************************************************
*  cfc_fifo_overflow_mon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function monitors CFC FIFO overflow interupt.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID                      
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
PUBLIC PMC_ERROR cfc_fifo_overflow_mon(cfc_handle_t *cfc_handle, UINT32 fifo_id)
{   
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY(); 

    ret_val = cfc_handle->cfg.cfg_fct_table.cfc_fifo_overflow_mon_cfg_ptr(cfc_handle, fifo_id);
    
    PMC_RETURN(ret_val);
} /* cfc_fifo_overflow_mon */

/*******************************************************************************
* cfc_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
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
PUBLIC PMC_ERROR cfc_int_chnl_enable(cfc_handle_t *cfc_handle,
                                     UINT32 fifo_id,
                                     cfc_int_chnl_t *int_table_ptr,
                                     cfc_int_chnl_t  *int_en_table_ptr,
                                     BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = cfc_handle->cfg.cfg_fct_table.cfc_int_chnl_enable_ptr(cfc_handle,
                                                                    fifo_id,
                                                                    int_table_ptr,
                                                                    int_en_table_ptr,
                                                                    enable);
    PMC_RETURN(result);
} /* cfc_int_chnl_enable */

/*******************************************************************************
* cfc_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PUBLIC PMC_ERROR cfc_int_chnl_clear(cfc_handle_t *cfc_handle,
                                    UINT32 fifo_id,
                                    cfc_int_chnl_t *int_en_table_ptr,
                                    cfc_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = cfc_handle->cfg.cfg_fct_table.cfc_int_chnl_clear_ptr(cfc_handle,
                                                                  fifo_id,
                                                                  int_en_table_ptr,
                                                                  int_table_ptr);
    PMC_RETURN(result);
} /* cfc_int_chnl_clear */

/*******************************************************************************
* cfc_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PUBLIC PMC_ERROR cfc_int_chnl_retrieve(cfc_handle_t *cfc_handle,
                                       UINT32 fifo_id,
                                       cfc_int_chnl_t  *int_en_table_ptr,
                                       cfc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = cfc_handle->cfg.cfg_fct_table.cfc_int_chnl_retrieve_ptr(cfc_handle,
                                                                     fifo_id,
                                                                     int_en_table_ptr,
                                                                     int_table_ptr);
    PMC_RETURN(result);
} /* cfc_int_chnl_retrieve */

/*******************************************************************************
* cfc_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PUBLIC PMC_ERROR cfc_int_chnl_enabled_check(cfc_handle_t *cfc_handle,
                                            UINT32  fifo_id,
                                            cfc_int_chnl_t *int_en_table_ptr,
                                            BOOL8 *int_found_ptr)
{
   PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = cfc_handle->cfg.cfg_fct_table.cfc_int_chnl_enabled_check_ptr(cfc_handle,
                                                                          fifo_id,
                                                                          int_en_table_ptr,
                                                                          int_found_ptr);
    PMC_RETURN(result);
} /* cfc_int_chnl_enabled_check */

/*******************************************************************************
*  cfc_fifo_ipt_id_get
* ______________________________________________________________________________
*
*   DESCRIPTION:
*    This API retrieves the channel ID associated with the page number within
*    the FIFO.
*
*
* INPUTS:
*   *cfc_handle     - pointer to CFC handle instance
*   fifo_id         - fifo id
*   frm_num         - frame number within a fifo 
*   
*
* OUTPUTS:
*  *ipt_id          - channel ID associated with this frame number of the fifo 
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  
*
*******************************************************************************/
PUBLIC PMC_ERROR cfc_fifo_ipt_id_get(cfc_handle_t *cfc_handle,
                                     UINT32 fifo_id,
                                     UINT32 frm_num,
                                     UINT32 *ipt_id)
{

    /* Variable declaration */
    UINT32 i;
    UINT32 num_current_pages;
    
    PMC_ENTRY();

    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    num_current_pages = 0;
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)
    {
        if(cfc_handle->var.inverted_page_table[i].ipt_id == fifo_id)
        {
            if(num_current_pages == frm_num) {
              *ipt_id = i;
              PMC_LOG_TRACE("tgmp_cfc_fifo_get_ipt_id: ipt_id[%u,%u] = %u",fifo_id,frm_num,*ipt_id);
              PMC_RETURN(PMC_SUCCESS);  
            }
            num_current_pages++;
        }
    }
    
    PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
} /* cfc_fifo_ipt_id_get */


/*
** Private Functions
*/

/*******************************************************************************
* cfc_int_chnl_enable_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
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
PRIVATE PMC_ERROR cfc_int_chnl_enable_internal(cfc_handle_t *cfc_handle,
                                               UINT32 fifo_id,
                                               cfc_int_chnl_t *int_table_ptr,
                                               cfc_int_chnl_t *int_en_table_ptr,
                                               BOOL8 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = fifo_id;
    UINT32          link2 = fifo_id;

    PMC_ENTRY();

    result = cfc_int_validate(cfc_handle, fifo_id, int_en_table_ptr);

    #ifdef CFC_INT_RANGE
    #undef CFC_INT_RANGE
    #endif
    #define CFC_INT_RANGE    UTIL_GLOBAL_INT_ENABLE_RANGE


    CFC_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cfc_int_chnl_enable_internal */

/*******************************************************************************
* cfc_int_chnl_clear_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PRIVATE PMC_ERROR cfc_int_chnl_clear_internal(cfc_handle_t *cfc_handle,
                                              UINT32  fifo_id,
                                              cfc_int_chnl_t *int_en_table_ptr,
                                              cfc_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = fifo_id;
    UINT32          link2 = fifo_id;

    PMC_ENTRY();

    result = cfc_int_validate(cfc_handle, fifo_id, int_table_ptr);
    
    #ifdef CFC_INT_RANGE
    #undef CFC_INT_RANGE
    #endif
    #define CFC_INT_RANGE UTIL_GLOBAL_INT_CLEAR_RANGE
    
    CFC_INT_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* cfc_int_chnl_clear_internal */

/*******************************************************************************
* cfc_int_chnl_retrieve_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PRIVATE PMC_ERROR cfc_int_chnl_retrieve_internal(cfc_handle_t *cfc_handle,
                                                 UINT32 fifo_id,
                                                 cfc_int_chnl_t *int_en_table_ptr,
                                                 cfc_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = cfc_int_validate(cfc_handle, fifo_id, int_table_ptr);

    if (PMC_SUCCESS == result)
    {      
        int_table_ptr->fifo_ovr_i = cfc_lfield_range_FIFO_OVR_I_get(NULL,cfc_handle,fifo_id,fifo_id);
        int_table_ptr->fifo_udr_i = cfc_lfield_range_FIFO_UDR_I_get(NULL,cfc_handle,fifo_id,fifo_id);
        int_table_ptr->ipt_err_i = cfc_lfield_range_IPT_ERR_I_get(NULL,cfc_handle,fifo_id,fifo_id);
    }

    PMC_RETURN(result);
} /* cfc_int_chnl_retrieve_internal */

/*******************************************************************************
* cfc_int_chnl_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
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
PRIVATE PMC_ERROR cfc_int_chnl_enabled_check_internal(cfc_handle_t *cfc_handle,
                                                      UINT32 fifo_id,
                                                      cfc_int_chnl_t *int_en_table_ptr,
                                                      BOOL8 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = fifo_id;
    UINT32          link2 = fifo_id;

    PMC_ENTRY();

    result = cfc_int_validate(cfc_handle, fifo_id, int_found_ptr);

    #ifdef CFC_INT_RANGE
    #undef CFC_INT_RANGE
    #endif
    #define CFC_INT_RANGE UTIL_GLOBAL_INT_CHECK_RANGE
    
    CFC_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* cfc_int_chnl_enabled_check_internal */

/*******************************************************************************
* cfc_handle_init_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an CFC block handle instance.
*
*
* INPUTS:
*   *cfc_handle           - pointer to CFC handle instance
*   *cfc_cfg_ptr          - pointer to a config structure containing the values 
*                           used for initialization \n
*                           NOTE: this function will only modify the settings 
*                           for max_fifo_num and page_size of the config  
*                           structure from the parent block 
*   *fct_ptr_table        - pointer to the configuration functions 
*   is_restart            - TRUE if this is a restart, FALSE if this is a cold
*                           start.
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state      - The expected energy state 
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
PUBLIC void cfc_handle_init_internal(cfc_handle_t *cfc_handle, 
                                     cfc_cfg_t *cfc_cfg_ptr,
                                     cfc_fifo_cfg_fct_table_t *fct_ptr_table,
                                     BOOL8 is_restart,
                                     util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                     pmc_energy_state_t top_energy_state)
{
    
    PMC_ENTRY();

    /* Required regardless of start/restart.  The function pointers may have
     * moved. */
    cfc_handle->cfg.cfg_fct_table   = *fct_ptr_table;

    if (FALSE == is_restart)
    {
        cfc_handle->cfg.max_fifo_num    = cfc_cfg_ptr->max_fifo_num;
        cfc_handle->cfg.page_size       = cfc_cfg_ptr->page_size;
        cfc_handle->cfg.fifo_null_entry = CFC_PHYSICAL_PAGE_NIL; 
        cfc_handle->cfg.bus_width       = CFC_BUS_WIDTH;
        /* We really only need this on cold start.  Ext_handle will always
         * point to something in the context */
        PMC_CTXT_REGISTER_PTR(&cfc_handle->cfg.cfg_fct_table.ext_handle, cfc_handle);

        cfc_handle->var.inverted_page_table = (inverted_page_table_t *)PMC_CTXT_CALLOC(cfc_handle->cfg.max_fifo_num*sizeof(inverted_page_table_t), cfc_handle);
        PMC_CTXT_REGISTER_PTR(&cfc_handle->var.inverted_page_table, cfc_handle);
        PMC_ASSERT(cfc_handle->var.inverted_page_table != NULL, CFC_ERR_CODE_ASSERT, 0, 0);
        /* NOTE: This is for fresh start up.  for Warm reboot, we don't want to do this 
            can we add argument to suppress this operation and instead do a var_update of
            the register space? new argument handle_init? 
            Instead of forcing value, we can use the update of actual device state. 
            We can then have a function to reset the IPT to it's default state. 
            So when the CFC is brought up at device level we can get this state in sync */
          
        /*cfc_inverted_page_table_var_update(cfc_handle, i);*/
         
        /* For now leave it at _init since cfc_fifo_empty_page_get check against 
           CFC_PHYSICAL_PAGE_EMPTY */
        cfc_inverted_page_table_var_init(cfc_handle);
    }

    PMC_RETURN();
    
} /* cfc_handle_init_internal */

/*******************************************************************************
*  cfc_fifo_end_addr_chg_abort_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function abort an end of address change request
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
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
PRIVATE void cfc_fifo_end_addr_chg_abort_cfg(cfc_handle_t *cfc_handle)
{
    PMC_ENTRY();
    cfc_field_FIFO_END_ADDR_CHG_ABORT_set( NULL, cfc_handle, 1);
    PMC_RETURN();
}

/*******************************************************************************
*  cfc_fifo_end_addr_req_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the outcome of the previous address change request
*   
*
* INPUTS:
*   *cfc_handle    - pointer to CFC handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32        - FIFO_END_ADDR_CHG_SUCCESS field setting from Hardware
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 cfc_fifo_end_addr_chg_success_get(cfc_handle_t *cfc_handle)
{
    UINT32 ret_val;
    PMC_ENTRY();
    
    ret_val = cfc_field_FIFO_END_ADDR_CHG_SUCCESS_get( NULL, cfc_handle);

    PMC_RETURN(ret_val);
}


/*******************************************************************************
*  cfc_fifo_end_addr_req_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the fifo end address req status 
*   
*
* INPUTS:
*   *cfc_handle   - pointer to CFC handle instance
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32        - FIFO_END_ADDR_CHG_REQ field setting from Hardware
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 cfc_fifo_end_addr_req_get(cfc_handle_t *cfc_handle)
{
    UINT32 ret_val;
    PMC_ENTRY();
    
    ret_val = cfc_field_FIFO_END_ADDR_CHG_REQ_get( NULL, cfc_handle);

    PMC_RETURN(ret_val);
}

/*******************************************************************************
*  cfc_fifo_end_addr_req_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function create an end address change request
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID                      
*   end_address         - the end address
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
PRIVATE void cfc_fifo_end_addr_req_cfg(cfc_handle_t *cfc_handle, 
                                       UINT32 fifo_id,  
                                       UINT32 end_address)
{     
    cfc_buffer_t cfc_buffer[1];


    PMC_ENTRY();
    
    
    cfc_buffer_init(cfc_buffer, cfc_handle);
    cfc_field_FIFO_END_ADDR_CHG_ID_set( cfc_buffer, cfc_handle, fifo_id);
    cfc_field_FIFO_END_ADDR_CHG_REQ_set( cfc_buffer, cfc_handle, 0);
    cfc_field_FIFO_END_ADDR_CHG_ABORT_set( cfc_buffer, cfc_handle, 0);
    cfc_buffer_flush(cfc_buffer);

    cfc_field_FIFO_END_ADDR_set( NULL, cfc_handle, end_address);

    cfc_buffer_init(cfc_buffer, cfc_handle);
    cfc_field_FIFO_END_ADDR_CHG_ID_set( cfc_buffer, cfc_handle, fifo_id);
    cfc_field_FIFO_END_ADDR_CHG_ABORT_set( cfc_buffer, cfc_handle, 0);
    cfc_field_FIFO_END_ADDR_CHG_REQ_set( cfc_buffer, cfc_handle, 1);
    cfc_buffer_flush(cfc_buffer);
    
    
    PMC_RETURN();
}

/*******************************************************************************
*  cfc_fifo_overflow_mon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function monitors CFC FIFO overflow interupt.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID                      
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
PRIVATE PMC_ERROR cfc_fifo_overflow_mon_cfg(cfc_handle_t *cfc_handle, UINT32 fifo_id)
{   
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32  num_iter;


    PMC_ENTRY(); 
    
    cfc_lfield_range_FIFO_OVR_I_set_to_clear(NULL,cfc_handle,fifo_id, fifo_id, 1);
#ifndef CFC_IO_VERSION
    ret_val = cfc_lfield_range_FIFO_OVR_I_poll_bit(NULL, cfc_handle, fifo_id, fifo_id, 1, CFC_NUM_USEC_ITERATIONS,  &num_iter, 1) ;
#else /* CFC_IO_VERSION */
    ret_val = cfc_lfield_range_FIFO_OVR_I_poll(NULL, cfc_handle, fifo_id, fifo_id, 1, PMC_POLL_EQUAL, CFC_NUM_USEC_ITERATIONS,  &num_iter, 1) ;
#endif /* CFC_IO_VERSION */
    
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(CFC_POLL_TIMEOUT);
    }
    
    PMC_RETURN(ret_val);
} /* cfc_fifo_overflow_mon_cfg */


/*******************************************************************************
*  cfc_ipt_entry_state_get_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the current state of an IPT entry and returns whether
*   or not the entry is occupied. 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   *ipt_valid          - qualifies the inverted page table entry
*                         
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
PRIVATE void cfc_ipt_entry_state_get_cfg(cfc_handle_t *cfc_handle, 
                                         UINT32 index,
                                         UINT32 *ipt_valid)
{   
    PMC_ENTRY();
    
#ifndef  CFC_REG_OP_CTXT_DISABLE
    *ipt_valid = cfc_handle->var.inverted_page_table[index].ipt_valid; 
#else
    *ipt_valid = cfc_field_IPT_VALID_get(NULL, cfc_handle, index);
#endif /* CFC_REG_OP_CTXT_DISABLE */

    PMC_RETURN();
} /* cfc_ipt_entry_state_get_cfg */

/*******************************************************************************
*  cfc_ipt_entry_get_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the current state of an IPT entry and returns whether
*   or not the entry is occupied. 
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   index               - index to the physical address page. The number of  
*                         pages supported is fixed to equal the FIFO_NUM.
*   *ipt_valid          - qualifies the inverted page table entry
*   *ipt_id             - The channle id associated with this entry (only valid 
*                         when ipt_valid is set)
*                         
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
PRIVATE void cfc_ipt_entry_get_cfg(cfc_handle_t *cfc_handle, 
                                   UINT32 index,
                                   UINT32 *ipt_valid,
                                   UINT32 *ipt_id)
{   
    PMC_ENTRY();

#ifndef  CFC_REG_OP_CTXT_DISABLE
    *ipt_valid = cfc_handle->var.inverted_page_table[index].ipt_valid; 
    *ipt_id = cfc_handle->var.inverted_page_table[index].ipt_id; 
#else
    *ipt_valid = cfc_field_IPT_VALID_get(NULL, cfc_handle, index);
    *ipt_id = cfc_field_IPT_ID_get(NULL, cfc_handle, index);
#endif /* CFC_REG_OP_CTXT_DISABLE */
    
    PMC_RETURN();
} /* cfc_ipt_entry_get_cfg*/


/*******************************************************************************
*  cfc_ovr_int_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the overflow interrupt for a FIFO ID
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - FIFO ID
*   enable            - enables/disables \n
*                       0 -- disables the interrupt \n
*                       1 -- enables the interrupt \n     
*                         
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
PRIVATE void cfc_ovr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                    UINT32 fifo_id,
                                    UINT32 enable)
{   
    PMC_ENTRY();
    
    /* Apply field settings */
    cfc_lfield_range_FIFO_OVR_E_set(NULL, cfc_handle, fifo_id, fifo_id, enable);
    
    PMC_RETURN(); 

} /* cfc_ovr_int_enable_cfg */


/*******************************************************************************
*  cfc_udr_int_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the underflow interrupt for a FIFO ID
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - FIFO ID
*   enable            - enables/disables \n
*                       0 -- disables the interrupt \n
*                       1 -- enables the interrupt  \n    
*                         
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
PRIVATE void  cfc_udr_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                        UINT32 fifo_id,
                                        UINT32 enable)
{   
    PMC_ENTRY();
    
    /* Apply field settings */
    cfc_lfield_range_FIFO_UDR_E_set(NULL, cfc_handle, fifo_id, fifo_id, enable);
    
    PMC_RETURN(); 

} /* cfc_udr_int_enable_cfg */


/*******************************************************************************
*  cfc_ipt_int_enable_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables or disables the IPT error interrupt for a FIFO ID
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   fifo_id           - FIFO ID
*   enable            - enables/disables \n
*                       0 -- disables the interrupt \n
*                       1 -- enables the interrupt \n     
*                         
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
PRIVATE void cfc_ipt_int_enable_cfg(cfc_handle_t *cfc_handle, 
                                         UINT32 fifo_id,
                                         UINT32 enable)
{   
    PMC_ENTRY();
    
    /* Apply field settings */
    cfc_lfield_range_IPT_ERR_E_set(NULL, cfc_handle, fifo_id, fifo_id, enable);
    
    PMC_RETURN(); 

} /* cfc_ipt_int_enable_cfg */

/*******************************************************************************
*  cfc_bypass_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IPT to bypass mode or not.
*   
*
* INPUTS:
*   *cfc_handle       - pointer to CFC handle instance
*   bypass            - bypass mode \n
*                       0 -- the physical addresses returned by the IPT are used
*                            to drive the write/read address outputs. \n
*                       1 -- the write/read addresses of the controller are used 
*                            to drive the write/read address outputs. \n
*                       This signal must remain static after a rest of the FIFO
*                       controller. 
*                         
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
PRIVATE void cfc_bypass_cfg(cfc_handle_t *cfc_handle, UINT32 bypass)
{   
    
    PMC_ENTRY();
    
    /* Apply field settings */
    cfc_field_IPT_BYPASS_set(NULL, cfc_handle, bypass);
    
    PMC_RETURN(); 

} /* cfc_bypass_cfg*/


/*******************************************************************************
*  cfc_ipt_page_mask_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the IPT page mask
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*                         
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
PRIVATE void cfc_ipt_page_mask_cfg(cfc_handle_t *cfc_handle)
{
    PMC_ENTRY();


    PMC_RETURN();
}

/*******************************************************************************
*  cfc_inverted_page_table_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the inverted page table for the channelized FIFO
*   controller that maps virtual block addresses to physical block addresses.
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
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
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR cfc_inverted_page_table_cfg(cfc_handle_t *cfc_handle,  
                                              UINT32 index,
                                              UINT32 ipt_valid,
                                              UINT32 ipt_id, 
                                              UINT32 ipt_va)
{   
    cfc_buffer_t cfc_buffer[1];
    
    PMC_ENTRY();
    
    /* argument checking */
    if (ipt_valid > 1) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }


    
    if(1 == ipt_valid)
    {
        cfc_buffer_init(cfc_buffer, cfc_handle);
         
#ifndef  CFC_REG_OP_CTXT_DISABLE
        cfc_field_IPT_VALID_set(cfc_buffer, cfc_handle, index, cfc_handle->var.inverted_page_table[index].ipt_valid); 
#endif /* CFC_REG_OP_CTXT_DISABLE */
        cfc_field_IPT_ID_set(cfc_buffer, cfc_handle, index, ipt_id);  
        cfc_field_IPT_VA_set(cfc_buffer, cfc_handle, index, ipt_va); 
        cfc_buffer_flush(cfc_buffer);

        cfc_field_IPT_ID_set(cfc_buffer, cfc_handle, index, ipt_id);  
        cfc_field_IPT_VA_set(cfc_buffer, cfc_handle, index, ipt_va); 
        cfc_field_IPT_VALID_set(cfc_buffer, cfc_handle, index, 1); 
        cfc_buffer_flush(cfc_buffer);
    }
    else
    {
        cfc_buffer_init(cfc_buffer, cfc_handle);

#ifndef  CFC_REG_OP_CTXT_DISABLE
        cfc_field_IPT_VA_set(cfc_buffer, cfc_handle, index, cfc_handle->var.inverted_page_table[index].ipt_va); 
        cfc_field_IPT_ID_set(cfc_buffer, cfc_handle, index, cfc_handle->var.inverted_page_table[index].ipt_id); 
#endif /* CFC_REG_OP_CTXT_DISABLE */
        cfc_field_IPT_VALID_set(cfc_buffer, cfc_handle, index, 0);
        cfc_buffer_flush(cfc_buffer);

        cfc_field_IPT_VALID_set(cfc_buffer, cfc_handle, index, 0);
        cfc_field_IPT_ID_set(cfc_buffer, cfc_handle, index, ipt_id);  
        cfc_field_IPT_VA_set(cfc_buffer, cfc_handle, index, ipt_va);  
        cfc_buffer_flush(cfc_buffer);
    }
    
#ifndef  CFC_REG_OP_CTXT_DISABLE                               
     /* Update var_t context */                               
     cfc_handle->var.inverted_page_table[index].ipt_valid = ipt_valid;
     cfc_handle->var.inverted_page_table[index].ipt_id = ipt_id;
     cfc_handle->var.inverted_page_table[index].ipt_va = ipt_va;
#endif /* CFC_REG_OP_CTXT_DISABLE */ 
    
    PMC_RETURN(PMC_SUCCESS); 

} /* cfc_inverted_page_table_cfg */


/*******************************************************************************
*  cfc_inverted_page_table_var_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the inverted page table in the memory for the 
*   FIFO based on what is written in the device
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
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
PRIVATE PMC_ERROR cfc_inverted_page_table_var_init(cfc_handle_t *cfc_handle)
{   
    /* Variable declaration */
    UINT32 i;
    
    PMC_ENTRY();
    
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++) 
    {
        /* Initialize var_t context */
        cfc_handle->var.inverted_page_table[i].ipt_valid = CFC_PHYSICAL_PAGE_EMPTY;
        cfc_handle->var.inverted_page_table[i].ipt_id = CFC_PHYSICAL_PAGE_NIL;
        cfc_handle->var.inverted_page_table[i].ipt_va = CFC_PHYSICAL_PAGE_NIL;  
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_inverted_page_table_var_init */


/*******************************************************************************
*  cfc_fifo_current_num_pages_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the number of pages already allocated for a FIFO in 
*   the inverted page table. 
*   
*
* INPUTS:
*   *cfc_handle               - pointer to CFC handle instance
*   fifo_id                   - FIFO ID
*   *num_current_pages_ptr    - pointer to the current number of pages that are
*                               allocated to the FIFO                 
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
PUBLIC PMC_ERROR cfc_fifo_current_num_pages_get(cfc_handle_t *cfc_handle, 
                                                 UINT32 fifo_id, 
                                                 UINT32 *num_current_pages_ptr)
{   
    /* Variable declaration */
    UINT32 i;
    UINT32 num_current_pages;

    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    num_current_pages = 0;
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)
    {
        if(cfc_handle->var.inverted_page_table[i].ipt_id == fifo_id)
        {
            num_current_pages++;
        }
    }
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, num_current_pages= %u \n", fifo_id, num_current_pages);
    
    *num_current_pages_ptr = num_current_pages;    
    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_current_num_pages_get */


/*******************************************************************************
*  cfc_fifo_empty_page_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns an array of empty physical page address indices and 
*   checks if there is enough empty rows left for the num pages.
*   
*
* INPUTS:
*   *cfc_handle             - pointer to CFC handle instance
*   num_pages               - number of pages needed for FIFO
*   *available_page_array   - a pointer to array of empty indices for the 
*                             inverted page table                     
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
PRIVATE PMC_ERROR cfc_fifo_empty_page_get(cfc_handle_t *cfc_handle, 
                                          UINT32 num_pages, 
                                          UINT32 *available_page_array)
{   
    /* Variable declaration */
    UINT32 i;
    UINT32 available_page_count;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    /* initialize the array*/
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)  
    { 
        available_page_array[i]= cfc_handle->cfg.fifo_null_entry;
    }                                         
    
    /* get the list of all empty page indices and store them in the array */
    available_page_count = 0;
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)
    { 
        if(cfc_handle->var.inverted_page_table[i].ipt_id == CFC_PHYSICAL_PAGE_NIL && \
           cfc_handle->var.inverted_page_table[i].ipt_va == CFC_PHYSICAL_PAGE_NIL)
        /*if(cfc_handle->var.inverted_page_table[i].ipt_valid == CFC_PHYSICAL_PAGE_EMPTY)*/
        {
            available_page_array[available_page_count] = i;
            available_page_count++;
        }
    }
    
    /* check if enough number of pages are available */
    if(num_pages > available_page_count)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, CFC_LOG_CODE_LOG, 0, 0, "error case: available_page_count = %u \n", available_page_count);
        PMC_LOG(PMC_LOG_SEV_HIGH, CFC_LOG_CODE_LOG, 0, 0, "error case: num_pages = %u \n", num_pages);

        /* return error the not enough pages available */
        PMC_RETURN(CFC_ERR_FIFO_PAGES_UNAVAILABLE);    
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_empty_page_get */


/*******************************************************************************
*  cfc_fifo_excess_page_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns an array of empty physical page address indices and 
*   checks if there is enough empty rows left for the num pages.
*   
*
* INPUTS:
*   *cfc_handle            - pointer to CFC handle instance
*   fifo_id                - FIFO ID
*   num_pages              - number of pages needed for FIFO
*   *excess_page_array     - a pointer to array of excess indices   
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
PUBLIC PMC_ERROR cfc_fifo_excess_page_get(cfc_handle_t *cfc_handle, 
                                           UINT32 fifo_id,
                                           UINT32 num_pages, 
                                           UINT32 *excess_page_array)
{   
    /* Variable declaration */
    UINT32 i;
    UINT32 excess_page_count;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    /* initialize the array*/
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)  
    { 
        excess_page_array[i]= cfc_handle->cfg.fifo_null_entry;
    }                                         
     
    /* get the list of all empty page indices and store them in the array */
    excess_page_count = 0;
    for(i = 0; i < cfc_handle->cfg.max_fifo_num; i++)
    { 
        if(cfc_handle->var.inverted_page_table[i].ipt_id == fifo_id && \
           cfc_handle->var.inverted_page_table[i].ipt_va >=  num_pages )
        /*if(cfc_handle->var.inverted_page_table[i].ipt_valid == CFC_PHYSICAL_PAGE_EMPTY)*/
        {
            excess_page_array[excess_page_count] = i;
            excess_page_count++;
        }
    }
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "excess_page_count = %u \n", excess_page_count);
    
    /* check if at least one page has to be deallocated */
    if(excess_page_count == 0)
    {
        /* return error */
        PMC_RETURN(CFC_ERR_DECREASE_GREATER_FIFO_SIZE);    
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_excess_page_get */

/*******************************************************************************
*  cfc_fifo_end_addr_cal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates and returns the end address that needs to be 
*   configured based on the number of pages for a FIFO.
*
* INPUTS:
*   *cfc_handle      - pointer to CFC handle instance
*   fifo_id          - FIFO ID
*   num_pages        - number of pages needed for FIFO
*   *end_addr_ptr    - pointer to the end address                 
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
PRIVATE PMC_ERROR cfc_fifo_end_addr_cal(cfc_handle_t *cfc_handle, 
                                        UINT32 fifo_id,
                                        UINT32 num_pages, 
                                        UINT32 *end_addr_ptr)
{   
    /* Variable declaration */
    UINT32 end_address;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    end_address = (num_pages*cfc_handle->cfg.page_size-1);
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, end_address= %u\n", fifo_id, end_address);
  
    *end_addr_ptr = end_address;
        
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_end_addr_cal */


/*******************************************************************************
*  cfc_fifo_memory_allocate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function allocates memory to a FIFO based on the number of pages it 
*   needs for the application using the FIFO. This is done via the inverted page
*   table, by allocating a physical page to each virtual page needed by the FIFO
*   
*
* INPUTS:
*   *cfc_handle            - pointer to CFC handle instance
*   fifo_id                - FIFO ID
*   num_current_pages      - number of pages curretnly allocated to the FIFO
*   num_pages_requested    - number of pages to be allocated to the FIFO
*   *available_page_array  - array of available page in the IPT
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
PRIVATE PMC_ERROR cfc_fifo_memory_allocate(cfc_handle_t *cfc_handle, 
                                           UINT32 fifo_id, 
                                           UINT32 num_current_pages, 
                                           UINT32 num_pages_requested, 
                                           UINT32 *available_page_array)
{   
    /* Variable declaration */
    UINT32 virtual_page;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num || num_pages_requested > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "fifo_id = %u, num_current_pages = %u, num_pages_requested = %u\n", fifo_id, num_current_pages, num_pages_requested);
        
    for(virtual_page = 0; virtual_page < (num_pages_requested-num_current_pages); virtual_page++)
    {
        /* allocate the physical page to the FIFO virtual page by programming 
           the FIFO ID and Virutal page into the inverted page table entry
           which represnet the physical page */
        
        /* Apply field settings */
        PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "virtual_page = %u, available_page_array[virtual_page]=%u\n", virtual_page, available_page_array[virtual_page]);
        cfc_handle->cfg.cfg_fct_table.cfc_inverted_page_table_cfg_ptr(cfc_handle, available_page_array[virtual_page], 1, fifo_id, num_current_pages+virtual_page);                                                                                                                                             
    }  
                                    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_memory_allocate */


/*******************************************************************************
*  cfc_fifo_memory_deallocate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deallocates memory of a FIFO based on the number of pages it 
*   needs to be removed for the FIFO. This is done via the inverted page
*   table, by marking the Valid bit, FIFO ID and Virtual page as unallocated
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   num_current_pages   - number of pages curretnly allocated to the FIFO
*   num_pages_requested - number of pages to be allocated to the FIFO
*   *excess_page_array  - a pointer to array of excess indices
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
PRIVATE PMC_ERROR cfc_fifo_memory_deallocate(cfc_handle_t *cfc_handle, 
                                             UINT32 num_current_pages, 
                                             UINT32 num_pages_requested, 
                                             UINT32 *excess_page_array)
{   
    /* Variable declaration */
    UINT32 virtual_page;
   
    PMC_ENTRY();
    
    /* argument checking */
    if (num_pages_requested > CFC_IPT_DEPTH) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "num_current_pages = %u, num_pages_requested = %u\n",num_current_pages, num_pages_requested);
    
    for(virtual_page = 0; virtual_page < (num_current_pages-num_pages_requested); virtual_page++)
    {
        /* allocate the physical page to the FIFO virtual page by programming 
           the FIFO ID and Virutal page into the inverted page table entry
           which represnet the physical page */
        
        /* Apply field settings */
        PMC_LOG(PMC_LOG_SEV_LOW, CFC_LOG_CODE_LOG, 0, 0, "virtual_page = %u, excess_page_array[virtual_page]=%u\n", virtual_page, excess_page_array[virtual_page]);
        cfc_handle->cfg.cfg_fct_table.cfc_inverted_page_table_cfg_ptr(cfc_handle, excess_page_array[virtual_page], CFC_PHYSICAL_PAGE_EMPTY, CFC_PHYSICAL_PAGE_NIL, CFC_PHYSICAL_PAGE_NIL);                                                                                                                                              
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* cfc_fifo_memory_deallocate */


/*******************************************************************************
*  cfc_fifo_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function takes the specified FIFO out of reset or 
*   alternatively puts the specified FIFO into reset
*   
*
* INPUTS:
*   *cfc_handle         - pointer to CFC handle instance
*   fifo_id             - FIFO ID
*   reset_value         - reset value \n
*                         0 -- into reset \n
*                         1 -- out of reset \n                      
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
PUBLIC PMC_ERROR cfc_fifo_reset_cfg(cfc_handle_t *cfc_handle, 
                                     UINT32 fifo_id, 
                                     UINT32 reset_value)
{   
    /* Variable declaration */

    PMC_ENTRY();
    
    /* argument checking */
    if (fifo_id > cfc_handle->cfg.max_fifo_num) {
        PMC_RETURN(CFC_ERR_INVALID_PARAMETERS);
    }
    
    cfc_lfield_range_FIFO_CLRB_set(NULL, cfc_handle, fifo_id, fifo_id, reset_value);
    
    PMC_RETURN(PMC_SUCCESS);   

} /* cfc_fifo_reset_cfg */



/*******************************************************************************
* cfc_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *cfc_handle      - pointer to CFC handle instance
*   fifo_id          - fifo id
*   non_null_ptr     - Pointer to check that shouldn't be NULL
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
PRIVATE PMC_ERROR cfc_int_validate(cfc_handle_t *cfc_handle,
                                   UINT32 fifo_id,
                                   void *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cfc_handle, CFC_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (fifo_id >= cfc_handle->cfg.max_fifo_num)
    {
        result = CFC_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* cfc_int_validate */




/*
** End of file
*/



