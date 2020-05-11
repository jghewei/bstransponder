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

#include "scbs3_loc.h"


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
const char SCBS3_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    SCBS3_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char SCBS3_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE UINT32 scbs3_offline_mem_create(scbs3_handle_t *scbs3_handle);
PRIVATE UINT32 scbs3_offline_mem_destroy(scbs3_handle_t *scbs3_handle);
PRIVATE UINT32 scbs3_offline_mem_reset(scbs3_handle_t *scbs3_handle);
PRIVATE UINT32 scbs3_offline_standby_cal_mem_update(scbs3_handle_t *scbs3_handle);
PRIVATE UINT32 scbs3_page_entries_read(scbs3_handle_t *scbs3_handle,                                  
                                       scbs3_page_type_enum_t page_type, 
                                       BOOL8 is_cal_bits,
                                       UINT32 row);

/*
** Public Functions
*/

/*******************************************************************************
* scbs3_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a SCBS3 block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the SCBS3 block relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   scbs3_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC scbs3_handle_t *scbs3_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    scbs3_handle_t *scbs3_handle;

    PMC_ENTRY();
    scbs3_handle = (scbs3_handle_t*)PMC_CTXT_CALLOC(sizeof(scbs3_handle_t), parent);

    pmc_handle_init(parent, scbs3_handle, sys_handle, PMC_MID_DIGI_SCBS3, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(SCBS3_LOG_ERR_STRINGS[0], SCBS3_LOG_ERR_TABLE_BASE, SCBS3_LOG_ERR_COUNT);

    /* Create handle for child blocks */
    scbs3_handle->util_schd_handle = util_schd_ctxt_create((pmc_handle_t *)scbs3_handle, base_address, sys_handle, "UTIL_SCHD");
    PMC_CTXT_REGISTER_PTR(&scbs3_handle->util_schd_handle, parent);

    PMC_RETURN(scbs3_handle);
} /* scbs3_ctxt_create */


/*******************************************************************************
* scbs3_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a SCBS3 block instance.
*
*
* INPUTS:
*   *scbs3_handle           - pointer to SCBS3 handle instance
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
PUBLIC void scbs3_ctxt_destroy(scbs3_handle_t *scbs3_handle)
{
    PMC_ENTRY();

    /* Free offline calendar memory */
    scbs3_offline_mem_destroy(scbs3_handle);

    util_schd_ctxt_destroy(scbs3_handle->util_schd_handle);
    PMC_CTXT_FREE(&scbs3_handle, scbs3_handle);

    PMC_RETURN();
} /* scbs3_ctxt_destroy */


/*******************************************************************************
* scbs3_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an SCBS3 block handle instance and all its associated context
*   information stored in fixed cfg_t and var_t structures.
*
*
* INPUTS:
*   *scbs3_handle         - pointer to SCBS3 handle instance
*   *scbs_cfg_ptr         - pointer to a config structure containing the values 
*                           used for initialization
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
PUBLIC void scbs3_handle_init(scbs3_handle_t *scbs3_handle,
                              scbs3_cfg_t *scbs_cfg_ptr)
{
    util_schd_block_const_ctxt_t util_schd_block_const_ctxt;

    PMC_ENTRY();

    /* Initialize scbs3_var_t structure for a specific parent */
    scbs3_handle->cfg.num_grp          = scbs_cfg_ptr->num_grp;
    scbs3_handle->cfg.max_chnl         = scbs_cfg_ptr->max_chnl;
    scbs3_handle->cfg.max_rows         = scbs_cfg_ptr->max_rows;
    scbs3_handle->cfg.total_rate       = scbs_cfg_ptr->total_rate;
    scbs3_handle->cfg.total_rate_units = scbs_cfg_ptr->total_rate_units;
    scbs3_handle->cfg.toc_sync         = scbs_cfg_ptr->toc_sync;
    scbs3_handle->cfg.toc_sync_en      = scbs_cfg_ptr->toc_sync_en;
    scbs3_handle->cfg.page_swap_mode   = scbs_cfg_ptr->page_swap_mode;
    scbs3_handle->cfg.bonus_mode_enabled = scbs_cfg_ptr->bonus_mode_enabled;
    scbs3_handle->cfg.cal_entries_alloc_method = scbs_cfg_ptr->cal_entries_alloc_method;
    
    /* Disable row resizing */
    scbs3_handle->cfg.enable_row_resizing = FALSE;

    scbs3_handle->var.standby_cal_mem_ptr      = NULL;
    scbs3_handle->var.standby_userbits_mem_ptr = NULL;
    scbs3_handle->var.active_cal_mem_ptr       = NULL;
    scbs3_handle->var.active_userbits_mem_ptr  = NULL;

    /* Initialize util_schd handle */
    util_schd_block_const_ctxt.base_chnl_num    = SCBS3_BASE_CHNL_NUMBER;
    util_schd_block_const_ctxt.num_schd_chnl    = scbs_cfg_ptr->max_chnl;
    util_schd_block_const_ctxt.chnl_num_step    = SCBS3_CHNL_NUMBER_STEP;
    util_schd_block_const_ctxt.num_schd_grp     = scbs_cfg_ptr->num_grp;
    util_schd_block_const_ctxt.total_rate       = scbs_cfg_ptr->total_rate;
    util_schd_block_const_ctxt.total_rate_units = scbs_cfg_ptr->total_rate_units;
    util_schd_block_const_ctxt.max_entries      = scbs_cfg_ptr->max_rows * SCBS3_CAL_ENTRY_PER_ROW;
    util_schd_block_const_ctxt.entry_null_value = SCBS3_NULL_CHNL_NUMBER;
    util_schd_block_const_ctxt.max_dcs_rd_channel_id = 0;

    if(scbs_cfg_ptr->cal_entries_alloc_method >=1 || scbs_cfg_ptr->cal_entries_alloc_method <= MAX_CAL_DISTRIBUTION_METHODS )
        util_schd_block_const_ctxt.cal_entries_alloc_method = scbs_cfg_ptr->cal_entries_alloc_method;
    else
        util_schd_block_const_ctxt.cal_entries_alloc_method = 1; /* No override value or incorrect one - use algorithm #1 as default */

    /* Set database ID if the scheduler requires DB access */
    switch (util_schd_block_const_ctxt.cal_entries_alloc_method)
    {
        case UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL:
        case UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_UNUSED;
            break;
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_ODUKSC_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMA:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_MPMA_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMO:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_MPMO_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_CBRC:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_UNUSED;
            break;    
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA: /* case 7 - 13: to be fixed to attach to correct DB */
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_FROM_MAPOTN:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_ODUKSC_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_MPMO_CTL:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_LINEOTN_SYSOTN_MODE;
            break;
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_LINE_CORE_OTN_TX_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_TO_MAPOTN:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_MPMA_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1:
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
            break;
        case UTIL_SCHD_ALLOC_METHOD_CPB_DCS:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_UNUSED;
            break;
        case UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_CBRC_SCBS3;
            break;
        case UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_CBRC_MPMO;
            break;
        case UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_ENET_LINE_MPMA;
            break;
        case UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_ENET_SYS_MPMA;
        break;
        default:
            util_schd_block_const_ctxt.db_id = UTIL_GEN_DB_UNUSED;
            break;
    }
        
    PMC_LOG_TRACE("util_schd_block_const_ctxt.cal_entries_alloc_method = %u\n", util_schd_block_const_ctxt.cal_entries_alloc_method);
    PMC_LOG_TRACE("util_schd_block_const_ctxt.db_id = %u\n", util_schd_block_const_ctxt.db_id);
    util_schd_handle_init(scbs3_handle->util_schd_handle, &util_schd_block_const_ctxt);
    
    /* Create calendar memory and reset all calendar entries in the offline page */
    scbs3_offline_mem_create(scbs3_handle);
    PMC_RETURN();
} /* scbs3_handle_init */

/*******************************************************************************
* scbs3_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Allocate any local memory that would be freed as a result of this
*  subsystem's ctxt_destroy
*
*
* INPUTS:
*   *h                          - pointer to SCBS3 handle instance
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
PUBLIC void scbs3_alloc_init(scbs3_handle_t *h)
{
    PMC_ENTRY();

    util_schd_alloc_init(h->util_schd_handle);

    PMC_RETURN();
}

/*******************************************************************************
* scbs3_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on scbs3 block handle instance.
*
*
* INPUTS:
*   *h                          - pointer to SCBS3 handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   energy_state                - energy state of the block relative to parent.
*   ptr_init_only               - TRUE: only initialize dynamically allocated pointers\n
*                                 FALSE: perform full handle restart
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
PUBLIC PMC_ERROR scbs3_handle_restart_init(scbs3_handle_t *h,
                                           util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                           pmc_energy_state_t energy_state,
                                           BOOL8 ptr_init_only)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();
    
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(SCBS3_LOG_ERR_STRINGS[0], SCBS3_LOG_ERR_TABLE_BASE, SCBS3_LOG_ERR_COUNT);
    }

    /* we don't care about db_id, it is not used when we only initialize pointers */
    rc = util_schd_handle_restart_init(h->util_schd_handle,UTIL_GEN_DB_UNUSED,
                                       restart_init_cfg_ptr,energy_state,ptr_init_only);

    PMC_RETURN(rc);
} /* scbs3_handle_restart_init */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
* scbs3_schd_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Takes the SCBS3 out of reset and configures the TOC and scheduler mode.
*   Allocates memory to store the offline group context, calendar and userbits pages.
*
* INPUTS:
*   *scbs3_handle         - pointer to SCBS3 handle instance
*   energy_state          - enum defining state to set the SCBS3 module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for SCBS3 module
*
*******************************************************************************/
PUBLIC UINT32 scbs3_schd_init(scbs3_handle_t *scbs3_handle, 
                              pmc_energy_state_t energy_state)
{
    /* variable declaration */
    UINT32 grp_start_addr = 0;
    UINT32 grp_end_addr;
    UINT32 row_itr;

    PMC_ENTRY();
    PMC_ASSERT(scbs3_handle!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);

    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Initialize the active page start and end address */
        /*
         * Write start and end address in group context.
         * Use the full calendar since we only use one group in SCBS3.
         */
        grp_end_addr = ((scbs3_handle->cfg.max_rows - 1) << SCBS3_GRP_ROW_OFFSET) | (SCBS3_CAL_ENTRY_PER_ROW - 1);
        scbs3_field_ACTV_START_ADDR_set(NULL, scbs3_handle, SCBS3_DEFAULT_GROUP_ID, grp_start_addr);
        scbs3_field_ACTV_END_ADDR_set(NULL, scbs3_handle, SCBS3_DEFAULT_GROUP_ID, grp_end_addr);
    
    
        /* Initial the active page with all NULL scheduler entries */
        /*
         * Write NULL_CH in actv calender
         */
        for (row_itr = 0; row_itr < scbs3_handle->cfg.max_rows; row_itr += 1) {
          /* Write the offline memory contents into the standby page of the calendar and userbits. */
          scbs3_reg_CAL_ACTV_PAGE_RAM_array_field_set(NULL, scbs3_handle, row_itr, 0xFFFFFFFF,0,0xFFFFFFFF);
          scbs3_reg_USER_BITS_ACTV_PAGE_RAM_array_field_set(NULL, scbs3_handle, row_itr, 0xFFFFFFFF, 0, 0x00000000);
        }
        
        scbs3_field_STBY_START_ADDR_set(NULL, scbs3_handle, SCBS3_DEFAULT_GROUP_ID, grp_start_addr);
        scbs3_field_STBY_END_ADDR_set(NULL, scbs3_handle, SCBS3_DEFAULT_GROUP_ID, grp_end_addr);
        
        for (row_itr = 0; row_itr < scbs3_handle->cfg.max_rows; row_itr += 1) {
          /* Write the offline memory contents into the standby page of the calendar and userbits. */
          
          scbs3_reg_CAL_STBY_PAGE_RAM_array_field_set(NULL, scbs3_handle, row_itr, 0xFFFFFFFF,0,0xFFFFFFFF);
          scbs3_reg_USER_BITS_STBY_PAGE_RAM_array_field_set(NULL, scbs3_handle, row_itr, 0xFFFFFFFF, 0, 0x00000000);
        }
    
        /* Clear SCBS_RESET */
        scbs3_field_SCBS_RESET_set(NULL, scbs3_handle, 0);
    
        /* Write TOC_SYNC value and enable */
        scbs3_field_TOC_SYNC_set(NULL, scbs3_handle, scbs3_handle->cfg.toc_sync);
        scbs3_field_TOC_SYNC_EN_set(NULL, scbs3_handle, scbs3_handle->cfg.toc_sync_en);
    
        /* Configure scheduler mode */
        scbs3_field_SCHD_MODE_set(NULL, scbs3_handle, scbs3_handle->cfg.bonus_mode_enabled);
        
        util_schd_init(scbs3_handle->util_schd_handle, energy_state);
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        util_schd_init(scbs3_handle->util_schd_handle, energy_state);
        
    } else {
        PMC_ASSERT(FALSE, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_schd_init */


/*******************************************************************************
* scbs3_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   SCBS3 instance. 
*
*
* INPUTS:
*   *scbs3_handle         - pointer to SCBS3 handle instance
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
PUBLIC BOOL scbs3_start_state_test(scbs3_handle_t *scbs3_handle)
{
    PMC_ENTRY();
    PMC_ASSERT(scbs3_handle!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);    
    
    if(util_schd_start_state_test(scbs3_handle->util_schd_handle) == FALSE)
        PMC_RETURN(FALSE);   
        

    PMC_RETURN(TRUE);

} /* scbs3_start_state_test */


/*******************************************************************************
* Run-time Operations
*******************************************************************************/
/*******************************************************************************
* scbs3_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Evaluate if the channel can be provisioned with the specified rate.
*
* INPUTS:
*   *scbs3_handle    - pointer to SCBS3 handle instance
*   chnl             - channel to provision
*   rate             - desired rate in units of total_rate_units in scbs3_cfg_t
*   rate_units       - 10^ exponent part of total_rate
*   user_bits        - Side band user bits that are output
*                      by the SCBS3 next to the channel being scheduled
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
PUBLIC PMC_ERROR scbs3_chnl_prov(scbs3_handle_t *scbs3_handle, UINT32 chnl,
                                 UINT32 rate, util_schd_chnl_rate_exp_t rate_units,
                                 UINT32 user_bits)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Configure the rate and user_bits for the channel */
    util_schd_chnl_data_set(scbs3_handle->util_schd_handle, chnl, SCBS3_DEFAULT_GROUP_ID,
                            rate, rate_units, SCBS3_UTIL_SCHD_CH_TYPE, SCBS3_DEFAULT_GROUP_ID);
    util_schd_chnl_user_bits_set(scbs3_handle->util_schd_handle, chnl, user_bits);

    /* Move channel state to UTIL_SCHD_CHNL_EQUIPPED */
    ret_val = util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_EQUIPPED);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Evaluate if the channel can be provisioned */
    ret_val = util_schd_grp_entry_eval(scbs3_handle->util_schd_handle, SCBS3_DEFAULT_GROUP_ID,
                                       scbs3_handle->cfg.max_rows * SCBS3_CAL_ENTRY_PER_ROW);

    /* Return channel state to UTIL_SCHD_CHNL_START if provisioning failed */
    if (ret_val != PMC_SUCCESS) {
        util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_START);
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_chnl_prov */

/*******************************************************************************
* scbs3_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovision the specified channel.
*
* INPUTS:
*   *scbs3_handle    - pointer to SCBS3 handle instance
*   chnl             - channel to deprovision
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
PUBLIC PMC_ERROR scbs3_chnl_deprov(scbs3_handle_t *scbs3_handle, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    BOOL8 dummy_chnl_state = FALSE;

    PMC_ENTRY();

    /* Configure the rate and user_bits for the channel to 0 */
    util_schd_chnl_data_set(scbs3_handle->util_schd_handle, chnl, SCBS3_DEFAULT_GROUP_ID,
                            0, (util_schd_chnl_rate_exp_t)0, SCBS3_UTIL_SCHD_CH_TYPE, SCBS3_DEFAULT_GROUP_ID);
    util_schd_chnl_user_bits_set(scbs3_handle->util_schd_handle, chnl, 0);

    /* Move channel state to UTIL_SCHD_CHNL_START */
    ret_val = util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_START);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    if (TRUE == pmc_coreotn_schd_extra_channel_revision(&scbs3_handle->base))
    {
        if (scbs3_handle->cfg.cal_entries_alloc_method == UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER)
        {
            ret_val = util_schd_chnl_state_is_dummy(scbs3_handle->util_schd_handle,
                                                    chnl, &dummy_chnl_state);
            
            if (PMC_SUCCESS == ret_val)
            {
                if (dummy_chnl_state)
                {
                    ret_val = util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_OPERATIONAL);
                    if (ret_val != PMC_SUCCESS) {
                      PMC_RETURN(ret_val);
                    }
                }
            }
        }
    }

    /* Deprovision channel */
    ret_val = util_schd_grp_entry_eval(scbs3_handle->util_schd_handle, SCBS3_DEFAULT_GROUP_ID,
                                       scbs3_handle->cfg.max_rows * SCBS3_CAL_ENTRY_PER_ROW);

    PMC_RETURN(ret_val);

} /* scbs3_chnl_deprov */

/*******************************************************************************
* scbs3_chnl_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates the specified channel.
*
* INPUTS:
*   *scbs3_handle    - pointer to SCBS3 handle instance
*   chnl             - channel to activate
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
PUBLIC PMC_ERROR scbs3_chnl_activate(scbs3_handle_t *scbs3_handle, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Move channel state to UTIL_SCHD_CHNL_OPERATIONAL */
    ret_val = util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_OPERATIONAL);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_chnl_activate */

/*******************************************************************************
* scbs3_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates the specified channel.
*
* INPUTS:
*   *scbs3_handle    - pointer to SCBS3 handle instance
*   chnl             - channel to deactivate
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
PUBLIC PMC_ERROR scbs3_chnl_deactivate(scbs3_handle_t *scbs3_handle, UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Move channel state to UTIL_SCHD_CHNL_EQUIPPED */
    ret_val = util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_EQUIPPED);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_chnl_deactivate */

/*******************************************************************************
* scbs3_chnl_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the specified channel.
*
* INPUTS:
*   *scbs3_handle    - pointer to SCBS3 handle instance
*   chnl             - channel to reprovision
*   rate             - desired rate in units of total_rate_units in scbs3_cfg_t
*   rate_units       - 10^ exponent part of total_rate
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
PUBLIC PMC_ERROR scbs3_chnl_reprov(scbs3_handle_t *scbs3_handle, UINT32 chnl,
                                   UINT32 rate, util_schd_chnl_rate_exp_t rate_units)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Configure the new rate for the channel */
    util_schd_chnl_data_set(scbs3_handle->util_schd_handle, chnl, SCBS3_DEFAULT_GROUP_ID,
                            rate, rate_units, SCBS3_UTIL_SCHD_CH_TYPE, SCBS3_DEFAULT_GROUP_ID);

    /* Evaluate if the channel can be provisioned */
    ret_val = util_schd_grp_entry_eval(scbs3_handle->util_schd_handle, SCBS3_DEFAULT_GROUP_ID,
                                       scbs3_handle->cfg.max_rows * SCBS3_CAL_ENTRY_PER_ROW);
    /* Return channel state to UTIL_SCHD_CHNL_START if provisioning failed */
    if (ret_val != PMC_SUCCESS) {
        util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl, UTIL_SCHD_CHNL_START);
        PMC_RETURN(ret_val);
    }                                   


    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_chnl_reprov */


/*******************************************************************************
*  scbs3_chnl_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clean a set of channels. Memory context and register space are cleaned
*
* INPUTS:
*   *scbs3_handle     - pointer to SCBS3 handle instance
*   swap              - flag that indicates if swap at the SCBS3 level
*   *chnl             - set of channels to clean
*   num_chnl          - number of channels to clean
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
PUBLIC void scbs3_chnl_clean(scbs3_handle_t *scbs3_handle, 
                             BOOL8 swap, 
                             UINT32 *chnl, 
                             UINT32 num_chnl)
{
    UINT32 chnl_itr, num_entries;
    scbs3_page_t active_page;
    util_schd_chnl_state_t util_schd_chnl_state;
    BOOL8 need_an_update = FALSE;
    BOOL8 dummy_state;
    PMC_ENTRY();

    /* get active page */
    scbs3_page_get(scbs3_handle,SCBS3_ACTIVE_MEM,&active_page);
            
    /* deactivate PHASE */
    for (chnl_itr = 0; chnl_itr < num_chnl;chnl_itr ++)
    {        
        (void) util_schd_chnl_state_get(scbs3_handle->util_schd_handle, chnl[chnl_itr], &util_schd_chnl_state);
        if (util_schd_chnl_state == UTIL_SCHD_CHNL_OPERATIONAL) 
        {
            /* clean the context and entries will be removed in next swap*/
            scbs3_chnl_deactivate(scbs3_handle, chnl[chnl_itr]);
            need_an_update = TRUE;
        } else 
        {
            num_entries = scbs3_num_entries_per_chnl_get(scbs3_handle,&active_page,chnl[chnl_itr]);
            if (num_entries != 0)
            {
                /* they are entries in reg space that are not in the context */
                /* they will be cleaned at the page swap */
                need_an_update = TRUE;
            }
        }
    }
    
    if (TRUE == need_an_update && TRUE == swap )
    {
        scbs3_page_udpate_and_swap(scbs3_handle);
    }
    
    /* deprov PHASE */
    for (chnl_itr = 0; chnl_itr < num_chnl;chnl_itr ++)
    {        
        (void) util_schd_chnl_state_is_dummy(scbs3_handle->util_schd_handle, chnl[chnl_itr],&dummy_state);
        if (dummy_state == TRUE)
        {
            util_schd_chnl_state_dummy_set(scbs3_handle->util_schd_handle, chnl[chnl_itr],FALSE);
            util_schd_chnl_state_set(scbs3_handle->util_schd_handle, chnl[chnl_itr],UTIL_SCHD_CHNL_EQUIPPED);
        }

        (void) util_schd_chnl_state_get(scbs3_handle->util_schd_handle, chnl[chnl_itr], &util_schd_chnl_state);
        if (util_schd_chnl_state == UTIL_SCHD_CHNL_EQUIPPED) 
        {
            (void) scbs3_chnl_deprov(scbs3_handle, chnl[chnl_itr]);        
        }                        
    }
    
    
    PMC_RETURN();

} /* scbs3_chnl_clean */

/*******************************************************************************
* scbs3_page_udpate_and_swap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates the calendar for active channels.
*   Updates the standby page and swaps the scheduler active/standby pages.
*
* INPUTS:
*   *scbs3_handle         - pointer to SCBS3 handle instance
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
PUBLIC PMC_ERROR scbs3_page_udpate_and_swap(scbs3_handle_t *scbs3_handle)
{
    UINT32 grp_start_addr = 0;
    UINT32 grp_end_addr;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* Generate calendar page */
    ret_val = scbs3_offline_standby_cal_mem_update(scbs3_handle);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /*
     * Write start and end address in group context.
     * Use the full calendar since we only use one group in SCBS3.
     */
    grp_end_addr = ((scbs3_handle->cfg.max_rows - 1) << SCBS3_GRP_ROW_OFFSET) | (SCBS3_CAL_ENTRY_PER_ROW - 1);
    scbs3_field_STBY_START_ADDR_set(NULL, scbs3_handle, SCBS3_DEFAULT_GROUP_ID, grp_start_addr);
    scbs3_field_STBY_END_ADDR_set(NULL, scbs3_handle, SCBS3_DEFAULT_GROUP_ID, grp_end_addr);

    /* Write standby calendar page in the scheduler */
    scbs3_standby_page_udpate(scbs3_handle);

    /* Switch pages */
    scbs3_page_swap_req(scbs3_handle);

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_page_udpate_and_swap */


/*******************************************************************************
* scbs3_standby_page_udpate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Writes the offline memory contents into the standby page of calendar memory of the scheduler.
*
* INPUTS:
*   *scbs3_handle         - pointer to SCBS3 handle instance
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
PUBLIC PMC_ERROR scbs3_standby_page_udpate(scbs3_handle_t *scbs3_handle)
{
    PMC_ENTRY();

    if (TRUE == pmc_lineotn_remove_null_chnl_id_revision(&scbs3_handle->base))
    {
        if ((scbs3_handle->cfg.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL) &&
            (scbs3_handle->cfg.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1) &&
            (scbs3_handle->cfg.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2))
        {
            /* Write the offline memory contents into the standby page of the calendar and userbits. */
            scbs3_reg_CAL_STBY_PAGE_RAM_array_burst_write(NULL, scbs3_handle, 0,
                                                          scbs3_handle->cfg.max_rows,
                                                          scbs3_handle->var.standby_cal_mem_ptr);
            scbs3_reg_USER_BITS_STBY_PAGE_RAM_array_burst_write(NULL, scbs3_handle, 0,
                                                                scbs3_handle->cfg.max_rows,
                                                                scbs3_handle->var.standby_userbits_mem_ptr);
        }
        else 
        {
            
            UINT32 size = scbs3_handle->cfg.max_rows * SCBS3_CAL_ENTRY_PER_ROW;
            UINT32  *local_cal_mem_ptr = (UINT32*)PMC_CALLOC(size);
            UINT32 chnl_index;
            UINT32 chnl;
            UINT32 i, j;
            UINT32 mask;
            UINT32 dci_chnl;          
            /* copy calendar entries to a local variable */
            for (i = 0; i < scbs3_handle->cfg.max_rows; i++)
            {
                local_cal_mem_ptr[i] = scbs3_handle->var.standby_cal_mem_ptr[i];
            }
            
            if (scbs3_handle->cfg.cal_entries_alloc_method == UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL)
            {
                /* modify the calendar entries to remove null channel IDs */
                for (i = 0; i < scbs3_handle->cfg.max_rows; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        chnl = (local_cal_mem_ptr[i] >> (j * 8)) & 0xff;
                        
                        if (chnl == SCBS3_NULL_CHNL_NUMBER)
                        {
                              dci_chnl = (i % 6) * 2 + (j % 2);
                              chnl_index = dci_chnl << (j*8);
                              mask = 0xff << (j*8);
                        
                              local_cal_mem_ptr[i] &= ~mask;
                              local_cal_mem_ptr[i] |= (mask & chnl_index);
                        }
                    }
                }
            }
            else if ((scbs3_handle->cfg.cal_entries_alloc_method == UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1) 
                     || (scbs3_handle->cfg.cal_entries_alloc_method == UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2))
            {
                /* modify the calendar entries to remove null channel IDs */
                for (i = 0; i < scbs3_handle->cfg.max_rows; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        chnl = (local_cal_mem_ptr[i] >> (j * 8)) & 0xff;
                        
                        if (chnl == SCBS3_NULL_CHNL_NUMBER)
                        {
                            if (j < 2)
                            {
                                chnl_index = (i * 2) + j ;
                                chnl_index = chnl_index << (j * 8);
                            }
                            else 
                            {
                                chnl_index = (i * 2) + 48 + (j-2);
                                chnl_index = chnl_index << (j * 8);
                            }
                            
                            mask = 0xff << (j*8);
                            
                            local_cal_mem_ptr[i] &= ~mask;
                            local_cal_mem_ptr[i] |= (mask & chnl_index);
                        }
                    }
                }
            }
          
            /* Write the offline memory contents into the standby page of the calendar and userbits. */
            scbs3_reg_CAL_STBY_PAGE_RAM_array_burst_write(NULL, scbs3_handle, 0,
                                                          scbs3_handle->cfg.max_rows,
                                                          local_cal_mem_ptr);
            scbs3_reg_USER_BITS_STBY_PAGE_RAM_array_burst_write(NULL, scbs3_handle, 0,
                                                                scbs3_handle->cfg.max_rows,
                                                                scbs3_handle->var.standby_userbits_mem_ptr);
            
            PMC_FREE(&local_cal_mem_ptr);
        }
    }
    else
    {
        /* Write the offline memory contents into the standby page of the calendar and userbits. */
        scbs3_reg_CAL_STBY_PAGE_RAM_array_burst_write(NULL, scbs3_handle, 0,
                                                      scbs3_handle->cfg.max_rows,
                                                      scbs3_handle->var.standby_cal_mem_ptr);
        scbs3_reg_USER_BITS_STBY_PAGE_RAM_array_burst_write(NULL, scbs3_handle, 0,
                                                            scbs3_handle->cfg.max_rows,
                                                            scbs3_handle->var.standby_userbits_mem_ptr);
    }
       
    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_standby_page_udpate */


/*******************************************************************************
* scbs3_page_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function read the calendar entries and user bits of the specified page
*
* INPUTS:
*   *scbs3_handle          - pointer to SCBS3 handle instance
*    page_type             - standby or active memory
*
* OUTPUTS:
*   *scbs3_page            - The scbs3 page
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void scbs3_page_get(scbs3_handle_t *scbs3_handle,
                           scbs3_page_type_enum_t page_type,
                           scbs3_page_t *scbs3_page)
{
    UINT32 entry_itr = 0;
    UINT32 row_itr = 0;
    UINT32 row;
    PMC_ENTRY();
    PMC_ASSERT(scbs3_handle!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(scbs3_page!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
        
    /* fill the page */
    scbs3_page->num_entry = scbs3_handle->cfg.max_chnl;

    for (row_itr = 0; 
         row_itr < scbs3_handle->cfg.max_rows;
         row_itr++)
    {        
        row = scbs3_page_entries_read(scbs3_handle,page_type, TRUE,row_itr);                
        scbs3_page->calendar[entry_itr]   = (row & 0xff);        
        scbs3_page->calendar[entry_itr+1] = ((row >>  8) & 0xff);
        scbs3_page->calendar[48+entry_itr] = ((row >> 16) & 0xff);
        scbs3_page->calendar[48+entry_itr+1] = ((row >> 24) & 0xff);
       
        row=scbs3_page_entries_read(scbs3_handle,page_type, FALSE,row_itr);
                
        scbs3_page->userbits[entry_itr]   = (row & 0xff);        
        scbs3_page->userbits[entry_itr+1] = ((row >>  8) & 0xff);
        scbs3_page->userbits[48+entry_itr] = ((row >> 16) & 0xff);
        scbs3_page->userbits[48+entry_itr+1] = ((row >> 24) & 0xff);

        entry_itr += 2;
    }
           
    PMC_RETURN();
} /* scbs3_page_get */

/*******************************************************************************
* scbs3_num_entries_per_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function read 
*
* INPUTS:
*   *scbs3_handle          - pointer to SCBS3 handle instance
*   *scbs3_page            - The scbs3 page
*    chnl                  - The channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                 - The number of entries
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 scbs3_num_entries_per_chnl_get(scbs3_handle_t *scbs3_handle,
                                             scbs3_page_t *scbs3_page,
                                             UINT32 chnl)
{
    UINT32 entry_itr = 0;
    UINT32 num_entries = 0;

    PMC_ENTRY();
    PMC_ASSERT(scbs3_handle!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(scbs3_page!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
            
    /* fill the page */
    for (entry_itr = 0; 
         entry_itr < scbs3_page->num_entry;
         entry_itr++)
    {                
        if (scbs3_page->calendar[entry_itr] == chnl)
        {
            num_entries += 1;
        }
    }
                 
    PMC_RETURN(num_entries);
} /* scbs3_num_entries_per_chnl_get */

/*******************************************************************************
* scbs3_userbits_per_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function  return user bits associated to a channel
*
* INPUTS:
*   *scbs3_handle          - pointer to SCBS3 handle instance
*   *scbs3_page            - The scbs3 page
*    chnl                  - The channel
*
* OUTPUTS:

*
* RETURNS:
*   UINT32                 - The userbit value
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 scbs3_userbits_per_chnl_get(scbs3_handle_t *scbs3_handle,
                                          scbs3_page_t *scbs3_page,
                                          UINT32 chnl)
{
    UINT32 entry_itr = 0;
    BOOL8 found_bits = FALSE;
    UINT32 ub = 0;

    PMC_ENTRY();
    PMC_ASSERT(scbs3_handle!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(scbs3_page!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
            
    for (entry_itr = 0; 
         entry_itr < scbs3_page->num_entry && found_bits == FALSE;
         entry_itr++)
    {        

        if (scbs3_page->calendar[entry_itr] == chnl) 
        { 
            found_bits = TRUE; 
            ub = scbs3_page->userbits[entry_itr];
        } 
        
    }

    PMC_RETURN(ub);
} /* scbs3_userbits_per_chnl_get */

/*******************************************************************************
* scbs3_first_calendar_entry_per_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function return the first calendar entry asscoaited to a channel
*
* INPUTS:
*   *scbs3_handle          - pointer to SCBS3 handle instance
*   *scbs3_page            - The scbs3 page
*    chnl                  - The channel
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                 - The number of entries
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 scbs3_first_calendar_entry_per_chnl_get(scbs3_handle_t *scbs3_handle,
                                                      scbs3_page_t *scbs3_page,
                                                      UINT32 chnl)
{
    UINT32 entry_itr = 0;
    UINT32 first_entry = 0xFFFFFFFF;

    PMC_ENTRY();
    PMC_ASSERT(scbs3_handle!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(scbs3_page!=NULL, SCBS3_ERR_INVALID_PARAMETERS, 0, 0);
            
    /* fill the page */
    for (entry_itr = 0; 
         entry_itr < scbs3_page->num_entry && first_entry == 0xFFFFFFFF;
         entry_itr++)
    {                
        if (scbs3_page->calendar[entry_itr] == chnl)
        {
            first_entry = entry_itr;
        }
    }
                 
    PMC_RETURN(first_entry);
} /* scbs3_first_calendar_entry_per_chnl_get */

/*******************************************************************************
*  scbs3_util_schd_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return SCBS3 handle 
*
* INPUTS:
*   *hndl                - pointer to SCBS3 handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_schd_handle_t   - The util_schd handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC util_schd_handle_t* scbs3_util_schd_handle_get(scbs3_handle_t *hndl)
{
    PMC_ENTRY();
    PMC_RETURN(hndl->util_schd_handle);    
} /* scbs3_util_schd_handle_get */

/*******************************************************************************
* scbs3_page_swap_req
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Triggers a swap of the active and standby pages for the Group Context,
*   Calendar and Userbits memory.
*
* INPUTS:
*   *scbs3_handle           - pointer to SCBS3 handle instance
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
PUBLIC PMC_ERROR scbs3_page_swap_req(scbs3_handle_t *scbs3_handle)
{
    PMC_ENTRY();
    /*
     * Check to see if the block is swapped with an external pin or using the register bit.
     * Only switch here if page swap is to be done locally.
     */
    if (SCBS3_PAGE_SWAP_BIT == scbs3_handle->cfg.page_swap_mode) {
        scbs3_field_PAGE_SWAP_REQ_set ( NULL, scbs3_handle, 1 );
    }

    PMC_RETURN(PMC_SUCCESS);
} /* scbs3_page_swap_req */


/*******************************************************************************
* scbs3_page_entries_read
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reads the specified row in the userbits memory
*
* INPUTS:
*   *scbs3_handle         - pointer to SCBS3 handle instance
*   page_type             - standby or active memory
*   is_cal_bits           - calendar or user bits
*   row                   - row number to read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                - The row value
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 scbs3_page_entries_read(scbs3_handle_t *scbs3_handle, 
                                       scbs3_page_type_enum_t page_type,
                                       BOOL8 is_cal_bits,
                                       UINT32 row)
{
    
    UINT32 val;
    PMC_ENTRY();
    if (is_cal_bits == TRUE)
    {
        /* Read specified row and return values */
        if (SCBS3_STANDBY_MEM == page_type) {
            val = scbs3_reg_CAL_STBY_PAGE_RAM_array_read(NULL, scbs3_handle, row);
        } else {
            val = scbs3_reg_CAL_ACTV_PAGE_RAM_array_read(NULL, scbs3_handle, row);
        }
    } else 
    {
        /* Read specified row and return values */
        if (SCBS3_STANDBY_MEM == page_type) {
            val = scbs3_reg_USER_BITS_STBY_PAGE_RAM_array_read(NULL, scbs3_handle, row);
        } else {
            val = scbs3_reg_USER_BITS_ACTV_PAGE_RAM_array_read(NULL, scbs3_handle, row);
        }
    }
    PMC_RETURN(val);
} /* scbs3_page_entries_read */


/*
** Private Functions
*/
/*******************************************************************************
* scbs3_offline_mem_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates context structure for calendar and userbits memory
*   by allocating memory for the specified calendar and userbits lengths.
*
* INPUTS:
*   *scbs3_handle   - pointer to SCBS3 handle instance
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
PRIVATE UINT32 scbs3_offline_mem_create(scbs3_handle_t *scbs3_handle)
{
    UINT32 size;

    PMC_ENTRY();

    /* Allocate context memory for offline memory */
    size = scbs3_handle->cfg.max_rows * SCBS3_CAL_ENTRY_PER_ROW;
    scbs3_handle->var.standby_cal_mem_ptr      = (UINT32*)PMC_CTXT_CALLOC(size, scbs3_handle);
    PMC_CTXT_REGISTER_PTR(&scbs3_handle->var.standby_cal_mem_ptr, scbs3_handle);
    scbs3_handle->var.standby_userbits_mem_ptr = (UINT32*)PMC_CTXT_CALLOC(size, scbs3_handle);
    PMC_CTXT_REGISTER_PTR(&scbs3_handle->var.standby_userbits_mem_ptr,
    scbs3_handle);

    scbs3_handle->var.active_cal_mem_ptr      = (UINT32*)PMC_CTXT_CALLOC(size, scbs3_handle);
    PMC_CTXT_REGISTER_PTR(&scbs3_handle->var.active_cal_mem_ptr, scbs3_handle);
    scbs3_handle->var.active_userbits_mem_ptr = (UINT32*)PMC_CTXT_CALLOC(size, scbs3_handle);
    PMC_CTXT_REGISTER_PTR(&scbs3_handle->var.active_userbits_mem_ptr,
    scbs3_handle);

    scbs3_offline_mem_reset(scbs3_handle);

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_offline_mem_create */

/*******************************************************************************
* scbs3_offline_mem_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Frees the memory used for the context structure for calendar and userbits memory.
*
* INPUTS:
*   *scbs3_handle   - pointer to SCBS3 handle instance
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
PRIVATE UINT32 scbs3_offline_mem_destroy(scbs3_handle_t *scbs3_handle)
{
    PMC_ENTRY();

    /* Free offline memory */
    if (scbs3_handle->var.standby_cal_mem_ptr != NULL) {
        PMC_CTXT_FREE(&(scbs3_handle->var.standby_cal_mem_ptr), scbs3_handle);
    }
    if (scbs3_handle->var.standby_userbits_mem_ptr != NULL) {
        PMC_CTXT_FREE(&(scbs3_handle->var.standby_userbits_mem_ptr), scbs3_handle);
    }
    if (scbs3_handle->var.active_cal_mem_ptr != NULL) {
        PMC_CTXT_FREE(&(scbs3_handle->var.active_cal_mem_ptr), scbs3_handle);
    }
    if (scbs3_handle->var.active_userbits_mem_ptr != NULL) {
        PMC_CTXT_FREE(&(scbs3_handle->var.active_userbits_mem_ptr), scbs3_handle);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_offline_mem_destroy */

/*******************************************************************************
* scbs3_offline_mem_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets the offline memory entries.
*
* INPUTS:
*   *scbs3_handle       - pointer to SCBS3 handle instance
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
PRIVATE UINT32 scbs3_offline_mem_reset(scbs3_handle_t *scbs3_handle)
{
    UINT32 *offline_mem_ptr;
    UINT32 row_num;

    PMC_ENTRY();

    /* Reset standby offline memory */
    offline_mem_ptr = scbs3_handle->var.standby_cal_mem_ptr;
    if (offline_mem_ptr != NULL) {
        for (row_num = 0; row_num < scbs3_handle->cfg.max_rows; row_num++) {
            offline_mem_ptr[row_num] = SCBS3_NULL_ROW_NUMBER;
        }
    }
    offline_mem_ptr = scbs3_handle->var.standby_userbits_mem_ptr;
    if (offline_mem_ptr != NULL) {
        for (row_num = 0; row_num < scbs3_handle->cfg.max_rows; row_num++) {
            offline_mem_ptr[row_num] = SCBS3_NULL_USERBITS_VAL;
        }
    }

    /* Reset active offline memory */
    offline_mem_ptr = scbs3_handle->var.active_cal_mem_ptr;
    if (offline_mem_ptr != NULL) {
        for (row_num = 0; row_num < scbs3_handle->cfg.max_rows; row_num++) {
            offline_mem_ptr[row_num] = SCBS3_NULL_ROW_NUMBER;
        }
    }
    offline_mem_ptr = scbs3_handle->var.active_userbits_mem_ptr;
    if (offline_mem_ptr != NULL) {
        for (row_num = 0; row_num < scbs3_handle->cfg.max_rows; row_num++) {
            offline_mem_ptr[row_num] = SCBS3_NULL_USERBITS_VAL;
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_offline_mem_reset */

/*******************************************************************************
* scbs3_offline_standby_cal_mem_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generate the distributed calendar entries and update the offline standby calendar memory.
*
* INPUTS:
*   *scbs3_handle       - pointer to SCBS3 handle instance
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
PRIVATE UINT32 scbs3_offline_standby_cal_mem_update(scbs3_handle_t *scbs3_handle)
{
    UINT32    num_entries;
    UINT32    *dist_entries_ptr;
    UINT32    *scbs_entries_ptr;
    UINT32    *scbs_user_bits_entries_ptr;
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32    row_num;

    PMC_ENTRY();

    num_entries = scbs3_handle->cfg.max_rows * SCBS3_CAL_ENTRY_PER_ROW;

    /*
     * Generate distributed calendar entries.
     * The distributed calendar is an array of num_entries * 32-bit entries.
     */
    dist_entries_ptr = (UINT32*)PMC_CALLOC(sizeof(UINT32) * num_entries);
    ret_val = util_schd_group_dist_gen(scbs3_handle->util_schd_handle, SCBS3_DEFAULT_GROUP_ID,
                                       num_entries, dist_entries_ptr);

    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&dist_entries_ptr);
        PMC_RETURN(ret_val);
    }

    /*
     * Format distributed calendar entries into the SCBS3 format.
     * The SCBS3 calendar has num_entries * 8-bit entries (1/4 the size of distributed calendar)
     *                   Entry D: Bit[31:24] Entry C: Bit[23:16] Entry B: Bit[15:8] Entry A: Bit[7:0]
     * Start Address           Entry 9           Entry 8            Entry 1           Entry 0
     * Start Address + 1      Entry 11          Entry 10            Entry 3           Entry 2
     * Start Address + 2      Entry 13          Entry 12            Entry 5           Entry 4
     * End Address            Entry 15          Entry 14            Entry 7           Entry 6
     */
    scbs_entries_ptr = (UINT32*)PMC_CALLOC(num_entries);
    ret_val = util_schd_scbs3_format_gen(scbs3_handle->util_schd_handle, num_entries,
                                         dist_entries_ptr, scbs_entries_ptr);
    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&dist_entries_ptr);
        PMC_FREE(&scbs_entries_ptr);
        PMC_RETURN(ret_val);
    }

    /* Generate user_bits data */
    scbs_user_bits_entries_ptr = (UINT32*)PMC_CALLOC(num_entries);
    ret_val = util_schd_scbs3_user_bits_gen(scbs3_handle->util_schd_handle, num_entries,
                                            scbs_entries_ptr, scbs_user_bits_entries_ptr);
    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&dist_entries_ptr);
        PMC_FREE(&scbs_entries_ptr);
        PMC_FREE(&scbs_user_bits_entries_ptr);
        PMC_RETURN(ret_val);
    }

    /* Copy tables to offline memory */
    for (row_num = 0; row_num < scbs3_handle->cfg.max_rows; row_num++) {
        scbs3_handle->var.standby_cal_mem_ptr[row_num]      = scbs_entries_ptr[row_num];
        scbs3_handle->var.standby_userbits_mem_ptr[row_num] = scbs_user_bits_entries_ptr[row_num];
    }



    PMC_FREE(&dist_entries_ptr);
    PMC_FREE(&scbs_entries_ptr);
    PMC_FREE(&scbs_user_bits_entries_ptr);

    PMC_RETURN(PMC_SUCCESS);

} /* scbs3_offline_standby_cal_mem_update */




/*******************************************************************************
* scbs3_set_custom_userbit_mask
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function to set the custom userbit mask int util_schd
*
* INPUTS:
*   *scbs3_handle       - pointer to SCBS3 handle instance
*   chnl_id             - Channel ID to apply the custom
*                         make to.
*   enable              - TRUE : use the custom mask provided
*                         FALSE : use normal userbit settings
*   set_mask            - Mask to control which userbit_mask
*                         will get applied
*   userbit_mask        - The custom userbit mask to be applied
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
PUBLIC PMC_ERROR scbs3_set_custom_userbit_mask(scbs3_handle_t *scbs3_handle,
                                               UINT32 chnl_id,
                                               BOOL enable,
                                               UINT32 set_mask[3],
                                               UINT32 userbit_mask[3])
{

    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();


    ret_val = util_schd_custom_userbit_mask_set(scbs3_handle->util_schd_handle,
                                                chnl_id,
                                                enable,
                                                set_mask,
                                                userbit_mask);
    
 

    PMC_RETURN(ret_val);

} /* scbs3_set_custom_userbit_mask */




/*******************************************************************************
* scbs3_calendar_pinch_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Wrapper function to set the custom userbit mask int util_schd
*
* INPUTS:
*   *scbs3_handle     - pointer to SCBS3 handle instance
*   cal_pinch_type    - See util_global_calendar_pinch_t.  Used to set a custom
*                       calendar pinch scenario.
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
PUBLIC PMC_ERROR scbs3_calendar_pinch_mode_set(scbs3_handle_t *scbs3_handle,
                                               util_global_calendar_pinch_t cal_pinch_type)
{

    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();


    ret_val = util_schd_calendar_pinch_mode_set(scbs3_handle->util_schd_handle,
                                                cal_pinch_type);
    

    PMC_RETURN(ret_val);

} /* scbs3_set_custom_userbit_mask */


/*
** End of file
*/


