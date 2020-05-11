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
#include "util_schd_loc.h"
#include "pmc_sys.h"
#include "pmc_log.h"
#include "stdio.h"
#include "util_gen_db.h"
/*
** Private Function Prototypes
*/
PRIVATE util_schd_chnl_ctxt_t* util_schd_mem_pool_assign(util_schd_handle_t *h);
PRIVATE void util_schd_mem_pool_release(util_schd_handle_t *h); 
PRIVATE void util_schd_mem_pool_alloc(util_schd_handle_t *h); 

PUBLIC void util_schd_var_chnl_default_set(util_schd_chnl_ctxt_t *p_chnl_ctxt, UINT32 chnl_id);
PUBLIC void util_schd_var_grp_default_set(util_schd_grp_ctxt_t *p_grp_ctxt);

PRIVATE void util_schd_chnl_ctxt_copy(util_schd_chnl_ctxt_t *dest,
                                     util_schd_chnl_ctxt_t *source);


PUBLIC void util_schd_var_default_init(util_schd_handle_t *h);



PUBLIC PMC_ERROR util_schd_group_dist_gen_method_1(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 num_dist_entries,
                                                   UINT32 *dist_entries);

PUBLIC PMC_ERROR util_schd_grp_entry_eval_method_1(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 entries);


PUBLIC PMC_ERROR util_schd_group_dist_gen_method_2(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 num_dist_entries,
                                                   UINT32 *dist_entries);

PUBLIC PMC_ERROR util_schd_grp_entry_eval_method_2(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 entries);
PRIVATE PMC_ERROR util_schd_grp_entry_eval_db_internal(util_schd_handle_t *h, 
                                                       util_gen_db_id_t db_id, 
                                                       util_gen_db_accessor_inst_t inst,
                                                       BOOL8 do_clean);
PUBLIC PMC_ERROR util_schd_grp_entry_eval_db(util_schd_handle_t *h, 
                                             util_gen_db_id_t db_id,
                                             util_gen_db_accessor_inst_t inst);    

PUBLIC PMC_ERROR util_schd_group_dist_gen_method_6(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 num_dist_entries,
                                                   UINT32 *dist_entries);

PUBLIC PMC_ERROR util_schd_grp_entry_eval_method_6(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 entries);

PRIVATE PMC_ERROR util_schd_context_audit(util_schd_handle_t *h,
                                          util_gen_db_id_t db_id,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr);
                                          
PRIVATE void util_schd_chnl_dist_gen_db(util_schd_handle_t *h,
                                        util_gen_db_id_t db_id,  
                                        util_gen_db_accessor_inst_t inst,
                                        UINT32 chnl_id,
                                        UINT32 *dist_entries);
                                                                                  
PRIVATE void util_schd_group_dist_gen_db(util_schd_handle_t *h,
                                         util_gen_db_id_t db_id,  
                                         util_gen_db_accessor_inst_t inst,
                                         UINT32 *dist_entries);

PRIVATE void util_schd_dcs_rd_queue_id_get(util_schd_handle_t *h, 
                                           UINT32 rd_channel_id,
                                           util_schd_chnl_state_t state1, 
                                           UINT32 *rd_queue_id_ptr);


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

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char UTIL_SCHD_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    UTIL_SCHD_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char UTIL_SCHD_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* util_schd_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates a handle for an util_schd block instance.  util_schd_handle_init
*   must be called to initialize the contents of the handle with instantation
*   specific data.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the util_schd subsystem relative to
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - pointer to character string containing a unique
*                           name for this instantiation of the handle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_schd_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC util_schd_handle_t *util_schd_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{

    util_schd_handle_t *util_schd_handle;

    PMC_ENTRY();

    util_schd_handle = (util_schd_handle_t*)PMC_CTXT_CALLOC(sizeof(util_schd_handle_t), parent);

    pmc_handle_init(parent, util_schd_handle, sys_handle, PMC_MID_DIGI_UTIL_SCHD, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(UTIL_SCHD_LOG_ERR_STRINGS[0], UTIL_SCHD_LOG_ERR_TABLE_BASE, UTIL_SCHD_LOG_ERR_COUNT);

    /* set the schd_ctxt var data pointer to NULL */
    util_schd_handle->var.schd_ctxt = NULL;
    util_schd_handle->var.util_schd_mem_pool = NULL;

    PMC_RETURN(util_schd_handle);
} /* util_schd_ctxt_create */


/*******************************************************************************
* util_schd_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an util_schd block instance.
*
*
* INPUTS:
*   *util_schd_handle         - pointer to util_schd handle instance
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
PUBLIC void util_schd_ctxt_destroy(util_schd_handle_t *util_schd_handle)
{

    PMC_ENTRY();

    pmc_handle_mid_verify(util_schd_handle, PMC_MID_DIGI_UTIL_SCHD);

    if (util_schd_handle->cfg.db_handle != 0)
    {
        /* Detach from DB */
        gen_db_detach(&(util_schd_handle->base), util_schd_handle->cfg.block_ctxt.db_id,
                      util_schd_handle->cfg.db_handle);
    }
    
    if(NULL != util_schd_handle->var.util_schd_mem_pool)
    {
        PMC_FREE(&(util_schd_handle->var.util_schd_mem_pool));
    }

    /* Free dynamically sized schd_ctxt var memory */
    /* it is possible this is not created for some parents who create MPMA/MPMO 
       without a scheduler handle - do not try to free if already NULL */
    if (NULL != util_schd_handle->var.schd_ctxt)
    {
        if (NULL != util_schd_handle->var.schd_ctxt->grp_ctxt)
        {
            PMC_CTXT_FREE(&(util_schd_handle->var.schd_ctxt->grp_ctxt), util_schd_handle);
        }
        if (NULL != util_schd_handle->var.schd_ctxt->chnl_ctxt)
        {
            PMC_CTXT_FREE(&(util_schd_handle->var.schd_ctxt->chnl_ctxt), util_schd_handle);
        }
        if (NULL != util_schd_handle->var.schd_ctxt->rd_queue_ids)
        {
            PMC_CTXT_FREE(&(util_schd_handle->var.schd_ctxt->rd_queue_ids), util_schd_handle);
        }
        PMC_CTXT_FREE(&(util_schd_handle->var.schd_ctxt), util_schd_handle);
    }

    /* Free handle */
    util_schd_handle->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&util_schd_handle, util_schd_handle);

    PMC_RETURN();
}  /* util_schd_ctxt_destroy */


/*******************************************************************************
* util_schd_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an util_schd block handle instance.
*
*
* INPUTS:
*   *h                          - pointer to util_schd handle instance
*   *const_ctxt                 - pointer to data structure containing fixed
*                                 data related to scheduler instance
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
PUBLIC void util_schd_handle_init(util_schd_handle_t *h,
                                  util_schd_block_const_ctxt_t *const_ctxt)
{
    UINT32 size;
    util_schd_block_const_ctxt_t *p_block_ctxt = NULL;
    util_schd_ctxt_t *p_schd_ctxt = NULL;

    PMC_ENTRY();

    /* initialize constant data used by the instance */
    p_block_ctxt = &(h->cfg.block_ctxt);
    
    p_block_ctxt->base_chnl_num       = const_ctxt->base_chnl_num;
    p_block_ctxt->num_schd_chnl       = const_ctxt->num_schd_chnl;
    p_block_ctxt->chnl_num_step       = const_ctxt->chnl_num_step;
    p_block_ctxt->num_schd_grp        = const_ctxt->num_schd_grp;
    p_block_ctxt->total_rate          = const_ctxt->total_rate;
    p_block_ctxt->total_rate_units    = const_ctxt->total_rate_units;
    p_block_ctxt->max_entries         = const_ctxt->max_entries;
    p_block_ctxt->entry_null_value    = const_ctxt->entry_null_value;
    p_block_ctxt->cal_entries_alloc_method = const_ctxt->cal_entries_alloc_method;
    p_block_ctxt->db_id               = UTIL_GEN_DB_UNUSED;
    p_block_ctxt->max_dcs_rd_channel_id = const_ctxt->max_dcs_rd_channel_id;

    /* Attach to DB if scheduler instance needs DB access */
/*    if ( (p_block_ctxt->cal_entries_alloc_method > 2 && p_block_ctxt->cal_entries_alloc_method < 6) 
          || (p_block_ctxt->cal_entries_alloc_method > 6) )  */
    if( p_block_ctxt->cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_UNUSED &&
        p_block_ctxt->cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL &&
        p_block_ctxt->cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL &&
        p_block_ctxt->cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_CBRC && 
        p_block_ctxt->cal_entries_alloc_method < LAST_UTIL_SCHD_ALLOC_METHOD )
    {
        p_block_ctxt->db_id = const_ctxt->db_id;
    
        h->cfg.db_handle = gen_db_attach(&(h->base), p_block_ctxt->db_id);
    }
    else {
        h->cfg.db_handle = 0;
    }

    PMC_LOG_TRACE("const_ctxt->base_chnl_num...value %lu\n", (unsigned long)const_ctxt->base_chnl_num);
    PMC_LOG_TRACE("p_block_ctxt->base_chnl_num...value %lu\n", (unsigned long)p_block_ctxt->base_chnl_num);
    PMC_LOG_TRACE("p_block_ctxt->cal_entries_alloc_method...value %lu\n", (unsigned long)p_block_ctxt->cal_entries_alloc_method);
    PMC_LOG_TRACE("p_block_ctxt->db_id...value %lu\n", (unsigned long)p_block_ctxt->db_id);
    PMC_LOG_TRACE("h->cfg.db_handle...value %lu\n", (unsigned long)h->cfg.db_handle);
/*    PMC_LOG_TRACE("h->cfg->block_ctxt.base_chnl_num...value %lu\n", (unsigned long)h->cfg->block_ctxt.base_chnl_num); */

    if (NULL != h->var.schd_ctxt)
    {
        /* if memory already allocated to this pointer free it so it can be
           reconstructed for current size requirements */
        if (NULL != h->var.schd_ctxt->grp_ctxt)
        {
            PMC_CTXT_FREE(&(h->var.schd_ctxt->grp_ctxt), h);
        }
        if (NULL != h->var.schd_ctxt->chnl_ctxt)
        {
            PMC_CTXT_FREE(&(h->var.schd_ctxt->chnl_ctxt), h);
        }
        if (NULL != h->var.schd_ctxt->rd_queue_ids)
        {
            PMC_CTXT_FREE(&(h->var.schd_ctxt->rd_queue_ids), h);
        }
        PMC_CTXT_FREE(&(h->var.schd_ctxt), h);
    }

    size = sizeof(util_schd_ctxt_t);

    h->var.schd_ctxt = (util_schd_ctxt_t*)PMC_CTXT_CALLOC(size, h);
    PMC_CTXT_REGISTER_PTR(&h->var.schd_ctxt, h);

    p_schd_ctxt = h->var.schd_ctxt;

    /* allocate var memory against h->var.schd_ctxt based on channel requirements of
       scheduler */
    size = sizeof(util_schd_grp_ctxt_t) * const_ctxt->num_schd_grp;
    if (0 != size)
    {
        p_schd_ctxt->grp_ctxt = (util_schd_grp_ctxt_t *)PMC_CTXT_CALLOC(size, h);
        PMC_CTXT_REGISTER_PTR(&p_schd_ctxt->grp_ctxt, h);
    }
    else
    {
        /* Should be NULL already since PMC_CTXT_CALLOC does a memset to 0 */
        p_schd_ctxt->grp_ctxt = NULL;
    }

    size = sizeof(util_schd_chnl_ctxt_t) * const_ctxt->num_schd_chnl;
    if (0 != size)
    {
        p_schd_ctxt->chnl_ctxt = (util_schd_chnl_ctxt_t *)PMC_CTXT_CALLOC(size, h);
        PMC_CTXT_REGISTER_PTR(&p_schd_ctxt->chnl_ctxt, h);
    }
    else
    {
        /* Should be NULL already since PMC_CTXT_CALLOC does a memset to 0 */
        p_schd_ctxt->chnl_ctxt = NULL;
    }

    size = sizeof(UINT8) * const_ctxt->max_dcs_rd_channel_id;
    if (0 != size)
    {
        p_schd_ctxt->rd_queue_ids = (UINT8 *)PMC_CTXT_CALLOC(size, h);
        PMC_CTXT_REGISTER_PTR(&p_schd_ctxt->rd_queue_ids, h);
    }
    else
    {
        /* Should be NULL already since PMC_CTXT_CALLOC does a memset to 0 */
        p_schd_ctxt->rd_queue_ids = NULL;
    }

    util_schd_var_default_init(h);
    
    /* Allocate Memory for Usage by Functions to Avoid Excess Stack Data */
    util_schd_alloc_init(h);
    
    /* Initialize the flags for Memory Usage by Functions to Avoid Excess Stack Data */
    h->var.util_schd_mem_pool_in_use = FALSE;
    h->var.restarted                 = FALSE;
    
    PMC_RETURN();
} /* util_schd_handle_init */

/*******************************************************************************
* util_schd_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*  Allocate any local memory
*
* INPUTS:
*   *h                          - pointer to util_schd handle instance
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
PUBLIC void util_schd_alloc_init(util_schd_handle_t *h)
{
    PMC_ENTRY();

    util_schd_mem_pool_alloc(h);

    PMC_RETURN();

}

/*******************************************************************************
* util_schd_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Functionality described in this API is subject to change.\n\n
*
*   Performs warm or crash restart processing on util_schd block handle instance.
*
*
* INPUTS:
*   *h                          - pointer to util_schd handle instance
*   db_id                       - database ID to access
*   *restart_init_cfg_ptr       - pointer to restart init config
*   energy_state                - energy state of the block relative to parent.
*   ptr_init_only               - TRUE: only initialize dynamically allocated pointers\n
*                                 FALSE: perform full handle restart
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_handle_restart_init(util_schd_handle_t *h,
                                               util_gen_db_id_t db_id,
                                               util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                               pmc_energy_state_t energy_state,
                                               BOOL8 ptr_init_only)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();



    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(UTIL_SCHD_LOG_ERR_STRINGS[0], UTIL_SCHD_LOG_ERR_TABLE_BASE, UTIL_SCHD_LOG_ERR_COUNT);
    }

    if(ptr_init_only ==  FALSE && energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* Test if any resources are not in start state,
        if resources present, then we are mismatch and can exit for warm.  If crash,
        we can force the subsystem back to starting state.*/
        if(!util_schd_start_state_test(h))
        {
            if(restart_init_cfg_ptr->is_warm_restart)
                PMC_RETURN(UTIL_SCHD_ERR_WARM_RESTART_MISMATCH);
            else
                util_schd_init(h, PMC_ENERGY_STATE_RESET);
        }
        
        h->var.util_schd_mem_pool_in_use = FALSE;
        PMC_RETURN(PMC_SUCCESS);
    }

    /*Memory pool usage would indicate if we crashed mid API.  In warm restart, if the
    memory pool is assigned, we crashed mid API, in this case, we can fail the warm
    restart. In crash, we can continue, after we have cleared the memory usage fields.*/
    if(h->var.util_schd_mem_pool_in_use==TRUE)
    {
        if(restart_init_cfg_ptr->is_warm_restart)
        {
            h->var.util_schd_mem_pool_in_use = FALSE;
            PMC_RETURN(UTIL_SCHD_ERR_WARM_RESTART_MISMATCH);
        }
        else
            h->var.util_schd_mem_pool_in_use = FALSE;
    }

    if(ptr_init_only ==  FALSE)
    {
        /* Check the channel states against the DB */
        rc = util_schd_context_audit(h,db_id,restart_init_cfg_ptr);
    }

    
    PMC_RETURN(rc);
} /* util_schd_handle_restart_init */

/*******************************************************************************
* util_schd_context_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Analyzes util_schd context relative to database contents. For crash restart
*   mode will also resolve database and util_schd context so that the union of
*   coherent context is maintained.  Channels identified in util_schd context
*   but not in database will be removed from util_schd.\n\n
*
*   In warm and crash restart mode returns an error PMC_ERROR_CTXT_INCOHERENT if 
*   context mismatch otherwise success.  For crash scenario this error will be
*   returned if context in such a state that it cannot be resolved.
*
*
* INPUTS:
*   *h                          - pointer to util_schd handle instance
*   db_id                       - database ID to access
*   *restart_init_cfg_ptr       - pointer to restart init config
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
PRIVATE PMC_ERROR util_schd_context_audit(util_schd_handle_t *h,
                                          util_gen_db_id_t db_id,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    util_gen_db_accessor_inst_t inst = LAST_UTIL_GEN_DB_ACCESSOR;
    BOOL8 do_clean = restart_init_cfg_ptr->is_warm_restart == TRUE ? FALSE:TRUE;
    PMC_ENTRY();

    switch(h->cfg.block_ctxt.cal_entries_alloc_method)
    {
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC:
            inst = UTIL_GEN_DB_ACCESSOR_ODUKSC;
            break;
            
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMA:
            inst = UTIL_GEN_DB_ACCESSOR_MPMA;
            break;
            
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMO:
            inst = UTIL_GEN_DB_ACCESSOR_MPMO;
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA:
            inst = UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN;
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_FROM_MAPOTN:
            inst = UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN;
            break;       
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_MPMO_CTL:
            inst = UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL;
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER:
            inst = UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER;
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_TO_MAPOTN:
            inst = UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN;
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1:
            inst = UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1;
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2:
            inst = UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2;
            break;     

        case UTIL_SCHD_ALLOC_METHOD_CPB_DCS:
            inst = UTIL_GEN_DB_ACCESSOR_CPB_DCS;
            break;     

        case UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB:
            inst = UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3;
            break;     

        case UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB:
            inst = UTIL_GEN_DB_ACCESSOR_CBRC_MPMO;
            break;     
            
        case UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB:
            inst = UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA;
            break;   

        case UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB:
            inst = UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA;
            break;   

        case UTIL_SCHD_ALLOC_METHOD_UNUSED:
        case UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL:
          /* 
           * If the data base is not use and entrie allocation method 
           * matches that then that is a successful audit. If it is not the
           * case then let the case statement fall into the default and 
           * assert
           */
          if (UTIL_GEN_DB_UNUSED == h->cfg.block_ctxt.db_id)
          {
              PMC_RETURN(PMC_SUCCESS);
          }
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,h->cfg.block_ctxt.cal_entries_alloc_method,0);
            break;
    }    

    /* audit state against database */
    result = util_schd_grp_entry_eval_db_internal(h, 
                                                  db_id, 
                                                  inst,
                                                  do_clean);

    
    PMC_RETURN(result);
} /* util_schd_context_audit */

/*******************************************************************************
* util_schd_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the util_schd instance to the starting state for all
*   dynamic context.  All dynamic context will be returned to the initialization 
*   values that were generate during the call to util_schd_handle_init().
*
*   This function is for use by parent modules that have provided all other
*   provisions for ensuring the state of all resources tracked by this
*   context has coherent register state.  For example, if the parent module
*   were reset and the scheduler tracked by the util_schd module was forced
*   to the reset state than the call to this API will force this context
*   to be coherent with this starting state.
*
*
* INPUTS:
*   *h                     - pointer to util_schd handle instance
*   energy_state           - enum defining state to set the UTIL_SCHD module to
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
PUBLIC void util_schd_init(util_schd_handle_t *h, 
                           pmc_energy_state_t energy_state)
{

    PMC_ENTRY();
    PMC_ASSERT(h!=NULL, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /*add any initializtion that are needed for runnign state*/
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* initialize var_t context back to default values */
        util_schd_var_default_init(h);
        /* Initialize the flags for Memory Usage by Functions to Avoid Excess Stack Data */
        h->var.util_schd_mem_pool_in_use = FALSE;    
    } 
    else {
        PMC_ASSERT(FALSE, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    }

    PMC_RETURN();

} /* util_schd_init */


/*******************************************************************************
* util_schd_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   util_schd instance.  It will test the channel state for all resources within
*   the util_schd instance if any are not in UTIL_SCHD_CHNL_START state then
*   FALSE is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the scheduler instance and it would be
*   possible for the higher layer module to put the scheduler in lowpower standby
*   mode of operation - should this be possible based on the specific scheduler
*   device properties.
*
*
* INPUTS:
*   *h                          - pointer to util_schd handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL                      - FALSE:  Resource(s) not in start state \n
*                               TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL util_schd_start_state_test(util_schd_handle_t *h)
{
    UINT32 i;
    PMC_ENTRY();
    PMC_ASSERT(h!=NULL, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);

    for(i=0; i < h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        if(h->var.schd_ctxt->chnl_ctxt[i].state != UTIL_SCHD_CHNL_START)
            PMC_RETURN(FALSE);
    }

    PMC_RETURN(TRUE);

} /* util_schd_start_state_test */

/*******************************************************************************
* util_schd_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an util_schd block handle instance var data structure for
*   all resources (block, groups and channels) within the block.
*
*   This function must be called after register space is accessible in order
*   for var context to be recovered from the device.
*
*
* INPUTS:
*   *h                          - pointer to util_schd handle instance
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
PUBLIC void util_schd_var_default_init(util_schd_handle_t *h)
{
    util_schd_block_const_ctxt_t *p_block_const_ctxt = NULL;
    util_schd_ctxt_t *p_schd_ctxt = NULL;
    util_schd_grp_ctxt_t *p_grp_ctxt = NULL;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;
    UINT32 chnl_id;
    UINT32 i;

    PMC_ENTRY();

    p_block_const_ctxt = &(h->cfg.block_ctxt);
    p_schd_ctxt = h->var.schd_ctxt;
    /*p_grp_ctxt = p_schd_ctxt->grp_ctxt; */
    p_grp_ctxt = h->var.schd_ctxt->grp_ctxt;


    for (i = 0; i < p_block_const_ctxt->num_schd_grp; i++){
        util_schd_var_grp_default_set(p_grp_ctxt);
        p_grp_ctxt++;
    }

    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    for (i = 0; i < p_block_const_ctxt->num_schd_chnl; i++){
        chnl_id = p_block_const_ctxt->base_chnl_num +
                  i * p_block_const_ctxt->chnl_num_step;

        util_schd_var_chnl_default_set(p_chnl_ctxt, chnl_id);
        p_chnl_ctxt++;
    }

    for(i=0;i<96;i++)
    {
        h->var.use_custom_userbit_mask[i] = FALSE;
    }

    h->var.cal_pinch_type = UTIL_GLOBAL_CAL_PINCH_NONE;

    PMC_RETURN();

} /* util_schd_var_default_init */



/*******************************************************************************
* util_schd_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an util_schd block instance.
*
*
* INPUTS:
*   *util_schd_handle         - pointer to util_schd handle instance.
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
PUBLIC UINT32 util_schd_base_address_get(util_schd_handle_t *util_schd_handle)
{
    PMC_ENTRY();
    PMC_RETURN(util_schd_handle->base.base_address);
} /* util_schd_base_address_get */


/*******************************************************************************
* util_schd_chnl_data_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the group id and state for a channel in scheduler channel context data
*   structure.\n\n
*
*   For DCS chnl_id specifies the C_RAM_CHANNEL field of the DCS calendar entry.
*   This value is an amalgamation of destination DPI port number and the
*   desired destination channel - DPI channel number. For MDRR groups
*   C_RAM_CHANNEL unused, in this case when generating scheduler contents MDRR
*   groups will need to be identified and scheduler entries assigned as
*   required.  When chnl_type set to 1, chnl_rate and chnl_rate_units must
*   be set to the rate of the MDRR group.  The value must be the same for
*   all members associated with the MDRR group.
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number (rd_queue_id for dcs)
*                                     to be updated.\n
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   grp_id                          - group id chnl_id associated with
*   chnl_rate                       - Channel rate =
*                                     chnl_rate * 10^(chnl_rate_units)
*   chnl_rate_units                 - 10^ exponent part of chnl_rate
*   chnl_type                       - Channel MDRR association.  Only valid
*                                     for DCS scheduler, if not DCS scheduler
*                                     set to 0.\n
*                                     1 : MDRR Group\n
*                                     0 : Non - MDRR Group
*   dcs_rd_channel_id               - dcs read channel associated with the rd_queue_id
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
PUBLIC PMC_ERROR util_schd_chnl_data_set(util_schd_handle_t *h,
                                         UINT32 chnl_id,
                                         UINT32 grp_id,
                                         UINT32 chnl_rate,
                                         util_schd_chnl_rate_exp_t chnl_rate_units,
                                         UINT32 chnl_type,
                                         UINT32 dcs_rd_channel_id)
{

    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = NULL;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    PMC_ENTRY();

    p_block_const_ctxt = &(h->cfg.block_ctxt);
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    PMC_LOG_TRACE("chnl num step: %d\n",p_block_const_ctxt->chnl_num_step);

    PMC_ASSERT(p_block_const_ctxt->chnl_num_step!=0, UTIL_SCHD_ERR_DIVIDE_BY_ZERO,0,0);
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    p_chnl_ctxt[chnl_index].grp_id      = grp_id;
    p_chnl_ctxt[chnl_index].rate        = chnl_rate;
    p_chnl_ctxt[chnl_index].rate_units  = chnl_rate_units;
    p_chnl_ctxt[chnl_index].chnl_type   = chnl_type;
    p_chnl_ctxt[chnl_index].dcs_rd_channel_id = dcs_rd_channel_id;
    if (dcs_rd_channel_id < p_block_const_ctxt->max_dcs_rd_channel_id)
    {
        h->var.schd_ctxt->rd_queue_ids[dcs_rd_channel_id] = chnl_index;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_chnl_data_set */




/*******************************************************************************
* util_schd_dcs_chnl_data_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the group id and state for a channel in a scheduler channel context data
*   structure.  This function is specific for context needed by DCS. \n\n
*
*   For DCS chnl_id specifies the C_RAM_CHANNEL field of the DCS calendar entry.
*   This value is an amalgamation of destination DPI port number and the
*   desired destination channel - DPI channel number. For MDRR groups
*   C_RAM_CHANNEL unused, in this case when generating scheduler contents MDRR
*   groups will need to be identified and scheduler entries assigned as
*   required.  When chnl_type set to 1, chnl_rate and chnl_rate_units must
*   be set to the rate of the MDRR group.  The value must be the same for
*   all members associated with the MDRR group.
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number (rd_queue_id for dcs)
*                                     to be updated.\n
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   grp_id                          - group id chnl_id associated with
*   chnl_rate                       - Channel rate =
*                                     chnl_rate * 10^(chnl_rate_units)
*   chnl_rate_units                 - 10^ exponent part of chnl_rate
*   chnl_type                       - Channel MDRR association.  Only valid
*                                     for DCS scheduler, if not DCS scheduler
*                                     set to 0. \n
*                                     1 : MDRR Group \n
*                                     0 : Non - MDRR Group
*   dcs_rd_channel_id               - dcs read channel associated with the rd_queue_id
*   dcs_sf1_push_port_id            - SCPB push port id
*   dcs_sf2_wr_queue_id             - CFC wr_queue_id
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
PUBLIC PMC_ERROR util_schd_dcs_chnl_data_set( util_schd_handle_t *h,
                                              UINT32 chnl_id,
                                              UINT32 grp_id,
                                              UINT32 chnl_rate,
                                              util_schd_chnl_rate_exp_t chnl_rate_units,
                                              UINT32 chnl_type,
                                              UINT32 dcs_rd_channel_id,
                                              UINT32 dcs_sf1_push_port_id,
                                              UINT32 dcs_sf2_wr_queue_id )
{

    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = NULL;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    PMC_ENTRY();

    p_block_const_ctxt = &(h->cfg.block_ctxt);
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    PMC_LOG_TRACE("chnl num step: %d\n",p_block_const_ctxt->chnl_num_step);

    PMC_ASSERT(p_block_const_ctxt->chnl_num_step!=0, UTIL_SCHD_ERR_DIVIDE_BY_ZERO, 0, 0);
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    p_chnl_ctxt[chnl_index].grp_id               = grp_id;
    p_chnl_ctxt[chnl_index].rate                 = chnl_rate;
    p_chnl_ctxt[chnl_index].rate_units           = chnl_rate_units;
    p_chnl_ctxt[chnl_index].chnl_type            = chnl_type;
    p_chnl_ctxt[chnl_index].dcs_rd_channel_id    = dcs_rd_channel_id;
    p_chnl_ctxt[chnl_index].dcs_sf1_push_port_id = dcs_sf1_push_port_id;
    p_chnl_ctxt[chnl_index].dcs_sf2_wr_queue_id  = dcs_sf2_wr_queue_id;
    if (dcs_rd_channel_id < p_block_const_ctxt->max_dcs_rd_channel_id &&
        !(h->cfg.block_ctxt.db_id == UTIL_GEN_DB_DCPB_DCS_3))
    {
        h->var.schd_ctxt->rd_queue_ids[dcs_rd_channel_id] = chnl_index;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* util_schd_dcs_chnl_data_set */


/*******************************************************************************
* util_schd_dcs_chnl_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Recovers DCS context stored in util_schd associated with channel state and
*   DCS mapping.
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number (rd_queue_id for dcs)\n
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*
* OUTPUTS:
*   *chnl_state_ptr                 - channel state
*   *dcs_rd_channel_id_ptr          - dcs read channel associated with the rd_queue_id
*   *dcs_sf1_push_port_id_ptr       - SCPB push port id
*   *dcs_sf2_wr_queue_id_ptr        - CFC wr_queue_id
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_dcs_chnl_data_get( util_schd_handle_t *h,
                                              UINT32 chnl_id,
                                              util_schd_chnl_state_t *chnl_state_ptr,
                                              UINT32 *dcs_rd_channel_id_ptr,
                                              UINT32 *dcs_sf1_push_port_id_ptr,
                                              UINT32 *dcs_sf2_wr_queue_id_ptr )
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = NULL;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    PMC_ENTRY();

    p_block_const_ctxt = &(h->cfg.block_ctxt);
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    PMC_LOG_TRACE("chnl num step: %d\n",p_block_const_ctxt->chnl_num_step);

    PMC_ASSERT(p_block_const_ctxt->chnl_num_step!=0, UTIL_SCHD_ERR_DIVIDE_BY_ZERO, 0, 0);
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    if (p_block_const_ctxt->num_schd_chnl > chnl_index)
    {
        p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;
        
        *chnl_state_ptr           = p_chnl_ctxt[chnl_index].state;               
        *dcs_rd_channel_id_ptr    = p_chnl_ctxt[chnl_index].dcs_rd_channel_id;
        *dcs_sf1_push_port_id_ptr = p_chnl_ctxt[chnl_index].dcs_sf1_push_port_id;
        *dcs_sf2_wr_queue_id_ptr  = p_chnl_ctxt[chnl_index].dcs_sf2_wr_queue_id;
    }
    else
    {
        result = UTIL_SCHD_ERR_INVALID_ARG;
    }

    PMC_RETURN(result);
} /* util_schd_dcs_chnl_data_get */

/*******************************************************************************
* util_schd_chnl_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number to be updated.
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   chnl_state                      - channel state to apply to channel.
*                                     See util_schd_chnl_state_t for valid
*                                     values.
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
PUBLIC PMC_ERROR util_schd_chnl_state_set(util_schd_handle_t *h,
                                          UINT32 chnl_id,
                                          util_schd_chnl_state_t chnl_state)
{
    UINT32 chnl_index;
    util_schd_chnl_state_t cur_state;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = &(h->cfg.block_ctxt);
    util_schd_chnl_ctxt_t *p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    PMC_ENTRY();

    /* Determine the channel index */
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    PMC_ASSERT(p_block_const_ctxt->num_schd_chnl > chnl_index ,PMC_ERR_INVALID_PARAMETERS,chnl_id,0);

    /* Determine the current state and see if the requested new state is valid */
    cur_state = p_chnl_ctxt[chnl_index].state;

    switch (chnl_state) {
    case UTIL_SCHD_CHNL_START:
        p_chnl_ctxt[chnl_index].state = chnl_state;
        break;
    case UTIL_SCHD_CHNL_EQUIPPED:
        p_chnl_ctxt[chnl_index].state = chnl_state;
        break;
    case UTIL_SCHD_CHNL_OPERATIONAL:
        p_chnl_ctxt[chnl_index].state = chnl_state;
        break;
    case UTIL_SCHD_CHNL_BACKUP:
        p_chnl_ctxt[chnl_index].state = chnl_state;
        break;
    default:
        PMC_RETURN(UTIL_SCHD_ERR_INVALID_ARG);
        break;
    }


    if(h->cfg.block_ctxt.db_id == UTIL_GEN_DB_DCPB_DCS_3 && (chnl_state == UTIL_SCHD_CHNL_OPERATIONAL
       || chnl_state == UTIL_SCHD_CHNL_EQUIPPED))
    {
        if (p_chnl_ctxt[chnl_index].dcs_rd_channel_id < p_block_const_ctxt->max_dcs_rd_channel_id)
        {
            h->var.schd_ctxt->rd_queue_ids[p_chnl_ctxt[chnl_index].dcs_rd_channel_id] = chnl_index;
        }
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_chnl_state_set */

/*******************************************************************************
* util_schd_dcs_chnl_state_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the group id and state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   queue_id                        - scheduler channel number to be updated.
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   mdrr_member                     - queue MDRR group association\n
*                                     0 : Not part of an MDRR group\n
*                                     1 : Part of an MDRR group
*   mdrr_grp_id                  - MDRR group associated with queue. Valid
*                                     if mdrr_member set to 1.  Legal range
*                                     0 to 11
*   grp_id                          - group id chnl_id associated with
*   chnl_state                      - channel state to apply to channel.
*                                     See util_schd_chnl_state_t for valid
*                                     values.
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
/* likely obsolete - DCS populates based on the destination channel */
PUBLIC PMC_ERROR util_schd_dcs_chnl_state_set(util_schd_handle_t *h,
                                              UINT32 queue_id,
                                              UINT32 mdrr_member,
                                              UINT32 mdrr_grp_id,
                                              UINT32 grp_id,
                                              util_schd_chnl_state_t chnl_state)
{

    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = NULL;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    PMC_ENTRY();

    /* this code uses the chnl_id field to capture the queue_id when used for
       DCS mode of operation */

    p_block_const_ctxt = &(h->cfg.block_ctxt);
    chnl_index = queue_id - p_block_const_ctxt->base_chnl_num;
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    p_chnl_ctxt[chnl_index].grp_id = grp_id;
    p_chnl_ctxt[chnl_index].state = chnl_state;
    p_chnl_ctxt[chnl_index].chnl_type = mdrr_member;

    PMC_RETURN(PMC_SUCCESS);


} /* util_schd_dcs_chnl_state_set */

/*******************************************************************************
* util_schd_chnl_user_bits_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the user bits for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number to be updated.
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   user_bits                       - user_bits to output together with the
*                                     channel in the SCBS3 [7:0].
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   Only used for the SCBS3.
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_chnl_user_bits_set(util_schd_handle_t *h,
                                              UINT32 chnl_id, UINT32 user_bits)
{
    UINT32 chnl_index;

    util_schd_block_const_ctxt_t *p_block_const_ctxt = &(h->cfg.block_ctxt);
    util_schd_chnl_ctxt_t *p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    PMC_ENTRY();

    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    p_chnl_ctxt[chnl_index].scbs_user_bits = user_bits;

    PMC_RETURN(PMC_SUCCESS);
} /* util_schd_chnl_user_bits_set  */

/*******************************************************************************
* util_schd_grp_entry_eval
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Evaluates the entry requirements for all channels associated with a
*   scheduler group instance for the number of scheduler entries specified by
*   the calling function.
*
*   If not enough scheduler entries available to safisfy requirements, returns
*   error and channel context unmodified by function.  If successful min_entries,
*   max_entries and alloc_entries in channel context updated.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - scheduler group id
*   entries                         - number of scheduler entries to evaluate
*                                     channel entry requirements
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_grp_entry_eval(util_schd_handle_t *h,
                                          UINT32 grp_id,
                                          UINT32 entries)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 alloc_method = h->cfg.block_ctxt.cal_entries_alloc_method;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("alloc_method = %u\n", alloc_method);
    
    /* protect us from users with uninitialzed memory */   
    if( alloc_method == UTIL_SCHD_ALLOC_METHOD_UNUSED || alloc_method >= MAX_CAL_DISTRIBUTION_METHODS )
    {
        alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL;
        h->cfg.block_ctxt.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL;
    }
    
    PMC_LOG_TRACE("h->cfg.block_ctxt.cal_entries_alloc_method = %u\n", h->cfg.block_ctxt.cal_entries_alloc_method);
    PMC_LOG_TRACE("\nentries=%u\n",entries);
    
    /* do not perform the check when it is only confirming the DB has been 
    ** updated previously for the channel that is going to be scheduled in the device. 
    */
    if (h->cfg.block_ctxt.cal_entries_alloc_method > UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL
        && h->cfg.block_ctxt.cal_entries_alloc_method != UTIL_SCHD_ALLOC_METHOD_CBRC)
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    
    /*
     loop through all entries and perform min/max entries calculations.
     min/max entry calculations may use different algorithms.
     based on min/max specify the assign_entries
    */
    switch(h->cfg.block_ctxt.cal_entries_alloc_method)
    {
        case UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL:
             /* provides one method, if multiple methods we need to add them and provide
                a selection mechanism - likely part of parent initializing this module */
            result = util_schd_grp_entry_eval_method_1(h, grp_id, entries);
            break;

        case UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL:
            result = util_schd_grp_entry_eval_method_2(h, grp_id, entries);
            break;
        
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_ODUKSC_DB, 
                                                 UTIL_GEN_DB_ACCESSOR_ODUKSC);
            break;
            
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMA:
            PMC_LOG_TRACE("util_schd->alloc_method = %u\n", alloc_method);
            
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_MPMA_DB,
                                                 UTIL_GEN_DB_ACCESSOR_MPMA);
            break;
            
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMO:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_MPMO_DB,
                                                 UTIL_GEN_DB_ACCESSOR_MPMO);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_CBRC:
            result = util_schd_grp_entry_eval_method_6(h, grp_id, entries);
            break;
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                                 UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_FROM_MAPOTN:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_ODUKSC_DB, /* fix to TX when TX_DB is filled */
                                                 UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN);
            break;       
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_MPMO_CTL:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_LINEOTN_SYSOTN_MODE, 
                                                 UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_LINE_CORE_OTN_TX_DB, 
                                                 UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_TO_MAPOTN:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_MPMA_DB, 
                                                 UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1:
            PMC_LOG_TRACE("case 12\n");
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                                 UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                                 UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2);
            break;     

        case UTIL_SCHD_ALLOC_METHOD_CPB_DCS:
            result = util_schd_grp_entry_eval_db(h, h->cfg.block_ctxt.db_id, 
                                                 UTIL_GEN_DB_ACCESSOR_CPB_DCS);
            break;     

        case UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_CBRC_SCBS3, 
                                                 UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3);
            break;     

        case UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_CBRC_MPMO, 
                                                 UTIL_GEN_DB_ACCESSOR_CBRC_MPMO);
            break;     
            
        case UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_ENET_LINE_MPMA, 
                                                 UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA);
            break;   

        case UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB:
            result = util_schd_grp_entry_eval_db(h, UTIL_GEN_DB_ENET_SYS_MPMA, 
                                                 UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA);
            break;   
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,h->cfg.block_ctxt.cal_entries_alloc_method,0);
            break;
    }

    if (PMC_SUCCESS != result)
    {
        result = PMC_SUCCESS;
    }
    PMC_RETURN(result);
} /* util_schd_grp_entry_eval */

/*******************************************************************************
* util_schd_group_dist_gen
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates sequence of calendar entries to meet distribution requirements.
*   Distributes entries based on the entry requirements specified in the
*   assign_entries data.
*
*   For DCS scheduler C_RAM_CHANNEL is invalid for MDRR groups.  In the
*   case of calendar entries that define MDRR groups instead of channels is
*   required, bits [31:28] will contain the MDRR Group ID (0 to 11) and
*   bit[27] will define if the entry is a regular channel entry (bit[27] == 0)
*   or an MDRR group entry (bit[27] == 1).
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id of the channels to be formed into
*                                     distributed list
*   num_dist_entries                - number of scheduler entries to distribute
*                                     channel entries against
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_group_dist_gen(util_schd_handle_t *h,
                                          UINT32 grp_id,
                                          UINT32 num_dist_entries,
                                          UINT32 *dist_entries)
{
    UINT32 alloc_method = h->cfg.block_ctxt.cal_entries_alloc_method;

    PMC_ENTRY();

    /* based on constant block context we can select which distribution
       mode to use */

    /* protect us from users with uninitialzed memory */   
    if( alloc_method == UTIL_SCHD_ALLOC_METHOD_UNUSED || alloc_method >= MAX_CAL_DISTRIBUTION_METHODS )
    {
        alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL;
        h->cfg.block_ctxt.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL;
    }

        
    switch(h->cfg.block_ctxt.cal_entries_alloc_method)
    {
        case UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL:
            util_schd_group_dist_gen_method_1(h,
                                              grp_id,
                                              num_dist_entries,
                                              dist_entries);
            break;

        case UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL:
            util_schd_group_dist_gen_method_2(h,
                                              grp_id,
                                              num_dist_entries,
                                              dist_entries);
            break;
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC:
            util_schd_group_dist_gen_db(h, 
                                        UTIL_GEN_DB_ODUKSC_DB, 
                                        UTIL_GEN_DB_ACCESSOR_ODUKSC,
                                        dist_entries);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMA:
            util_schd_group_dist_gen_db(h, 
                                        UTIL_GEN_DB_MPMA_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMA,
                                        dist_entries);
            break; 
                   
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMO:
            util_schd_group_dist_gen_db(h, 
                                        UTIL_GEN_DB_MPMO_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMO,
                                        dist_entries);
            break;  
        case UTIL_SCHD_ALLOC_METHOD_CBRC:
            util_schd_group_dist_gen_method_6(h,
                                              grp_id,
                                              num_dist_entries,
                                              dist_entries);
            break;

        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA: /* MPMA_COREOTN */ /* case 7 - 13 for COREOTN need to be reviewed and determine if we can just look at one db */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN,
                                        dist_entries);

            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_FROM_MAPOTN: /* MPMA_COREOTN_FROM_MAPOTN */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_ODUKSC_DB, /* fix to TX when TX_DB is filled */
                                        UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN,
                                        dist_entries);
            break;       
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_MPMO_CTL: /* MPMA_MPMO_COREOTN_CTL */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINEOTN_SYSOTN_MODE, 
                                        UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL,
                                        dist_entries);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER: /* MPMO_COREOTN_MUX_FRAMER */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_TX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER,
                                        dist_entries);

            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_TO_MAPOTN: /* MPMO_COREOTN_TO_MAPOTN */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_MPMA_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN,
                                        dist_entries);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1: /* COREOTN_ODUKSC1 */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1,
                                        dist_entries);
        
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2: /* COREOTN_ODUKSC2 */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2,
                                        dist_entries);

            break;

        case UTIL_SCHD_ALLOC_METHOD_CPB_DCS:
            util_schd_group_dist_gen_db(h, h->cfg.block_ctxt.db_id, 
                                        UTIL_GEN_DB_ACCESSOR_CPB_DCS,
                                        dist_entries);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_CBRC_SCBS3, 
                                        UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3,
                                        dist_entries);
            break; 

        case UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_CBRC_MPMO, 
                                        UTIL_GEN_DB_ACCESSOR_CBRC_MPMO,
                                        dist_entries);
            break;         

        case UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_ENET_LINE_MPMA, 
                                        UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA,
                                        dist_entries);
            break;     

        case UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_ENET_SYS_MPMA, 
                                        UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA,
                                        dist_entries);
            break;                 
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,h->cfg.block_ctxt.cal_entries_alloc_method,0);
            break;
    }

    PMC_RETURN(PMC_SUCCESS);
}  /* util_schd_group_dist_gen */



/*******************************************************************************
* util_schd_scbs3_format_gen
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Format distributed calendar entries into the SCBS3 format.
*   The SCBS3 calendar has num_entries * 8-bit entries (1/4 the size of
*   distributed calendar).
*   Entry D:Bit[31:24] Entry C:Bit[23:16] Entry B:Bit[15:8] Entry A:Bit[7:0]
*        Entry 9            Entry 8            Entry 1           Entry 0
*       Entry 11           Entry 10            Entry 3           Entry 2
*       Entry 13           Entry 12            Entry 5           Entry 4
*       Entry 15           Entry 14            Entry 7           Entry 6
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   num_entries                     - number of scheduler entries to
*                                     distribute channel entries against
*   *dist_entries                   - pointer to storage for the scheduler
*                                     generated contents in order of execution.
*                                     Storage size is num_entries bytes.
*
* OUTPUTS:
*   *scbs_entries                   - pointer to storage for the scheduler
*                                     data formatted for SCBS3 calendar
*                                     Storage size is (num_entries/4) bytes.
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_scbs3_format_gen(util_schd_handle_t *h,
                                            UINT32 num_entries,
                                            UINT32 *dist_entries,
                                            UINT32 *scbs_entries)
{

    /* reference hyphy20g_cpb.c line 13579 *scbs_entries follows
       same format as HyPHY 20G pCalData */
    UINT32 i, j;
    UINT8  entry_a;
    UINT8  entry_b;
    UINT8  entry_c;
    UINT8  entry_d;

    PMC_ENTRY();

    if(num_entries%4 != 0) PMC_RETURN(UTIL_SCHD_ERR_INVALID_ARG);

    /* Format the linear calendar data into the SCBS3 scheduler calendar format */
    j = 0;
    for(i = 0; i < num_entries/4; i++){
        entry_a = *(dist_entries + j) & 0xff;
        entry_b = *(dist_entries + (j+1)) & 0xff;
        scbs_entries[i] = (entry_b << 8) | entry_a;
        j += 2;
    }

    for(i=0; i < num_entries/4; i++){
        entry_c = *(dist_entries + j) & 0xff;
        entry_d = *(dist_entries + (j+1)) & 0xff;
        scbs_entries[i] = scbs_entries[i] | (entry_d << 24) | (entry_c << 16);
        j += 2;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_scbs3_format_gen */

/*******************************************************************************
* util_schd_scbs3_user_bits_gen
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates a userbits array based on the input SCBS3 formatted set of calendar
*   data.
*
*   The SCBS3 userbits has num_entries * 8-bit entries (1/4 the size of
*   distributed calendar).
*   Entry D:Bit[31:24] Entry C:Bit[23:16] Entry B:Bit[15:8] Entry A:Bit[7:0]
*        Entry 9            Entry 8            Entry 1           Entry 0
*       Entry 11           Entry 10            Entry 3           Entry 2
*       Entry 13           Entry 12            Entry 5           Entry 4
*       Entry 15           Entry 14            Entry 7           Entry 6
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   num_entries                     - number of scheduler entries in
*                                     scbs_entries
*   *scbs_entries                   - pointer to storage for the scheduler
*                                     data formatted for SCBS3 calendar
*                                     Storage size is (num_entries/4) bytes.
*
* OUTPUTS:
*   *scbs_user_bits_entries         - pointer to storage for the userbits
*                                     data formatted for SCBS3 calendar
*                                     Storage size is (num_entries/4) bytes.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_scbs3_user_bits_gen(util_schd_handle_t *h,
                                            UINT32 num_entries,
                                            UINT32 *scbs_entries,
                                            UINT32 *scbs_user_bits_entries)
{
    util_schd_block_const_ctxt_t *p_block_const_ctxt = &(h->cfg.block_ctxt);
    util_schd_chnl_ctxt_t *p_chnl_ctxt               = h->var.schd_ctxt->chnl_ctxt;
    UINT32 row;
    UINT32 entry_idx;
    UINT8  chnl;
    UINT32 chnl_index;
    UINT32 userbits_val;
    UINT32 userbits_row;

    UINT32 cal_idx = 0;

    PMC_ENTRY();

    if(num_entries%4 != 0) PMC_RETURN(UTIL_SCHD_ERR_INVALID_ARG);

    /* Search the channel context and populate the userbits data */
    for(row = 0; row < num_entries/4; row++){
        userbits_row = 0;
        for (entry_idx = 0; entry_idx < 4; entry_idx++) {
            /* For each active channel, find the userbits value */
            chnl = (scbs_entries[row] >> (entry_idx * 8)) & 0xff;
            if (chnl != h->cfg.block_ctxt.entry_null_value) {
                chnl_index = chnl - p_block_const_ctxt->base_chnl_num;
                chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;
                
                
                if(h->var.use_custom_userbit_mask[chnl])
                {

                    cal_idx = (entry_idx/2)*(num_entries/2) + row*2 + entry_idx%2;

                    if( (h->var.custom_userbit[cal_idx/32] & (1 << (cal_idx%32))) != 0)
                    {
                        userbits_val = (1 & 0xff) << (entry_idx * 8);
                    }
                    else
                    {
                        userbits_val = 0;
                    }
                }
                else
                {
                    userbits_val = (p_chnl_ctxt[chnl_index].scbs_user_bits & 0xff) << (entry_idx * 8);
                }
            } else {
                userbits_val = 0;
            }
            /* Create the user_bits row data */
            userbits_row = userbits_row | userbits_val;
        }
        scbs_user_bits_entries[row] = userbits_row;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_scbs3_user_bits_gen */

/*******************************************************************************
* util_schd_dcs_format_gen
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates DCS calendar formatted data that generates channel requests as
*   per the order specified in the input entry distribution list.
*
*   DCS calendar data is composed of three fields:
*   C_RAM_TYPE
*   C_RAM_CHANNEL
*   C_RAM_IDENTIFIER
*
*   The input structure dist_entries provides context to set C_RAM_CHANNEL
*   and C_RAM_TYPE fields. C_RAM_IDENTIFIER is also available for cases where
*   C_RAM_TYPE is set to 1.  For case of C_RAM_TYPE set to 0, C_RAM_IDENTIFIER
*   is recovered from channel util_schd context memory.
*
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   entries                         - number of scheduler entries to distribute
*                                     channel entries againste
*   *dist_entries[]                 - pointer to storage for the scheduler
*                                     generated contents in order of execution.
*
* OUTPUTS:
*   *dcs_entries[]                  - pointer to storage for the scheduler
*                                     data formatted for DCS calendar
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_dcs_format_gen(util_schd_handle_t *h,
                                          UINT32 entries,
                                          UINT32 dist_entries[],
                                          util_schd_dcs_calendar_entry_t dcs_entries[])
{

    /* DCS is a linear set of 32 bit registers with three fields
       C_RAM_GROUP_BIT
       C_RAM_CHANNEL
       C_RAM_IDENTIFIER
       Register 0x60400 + 0x2000*A(A=0:3) + 0x4*N(N=0:255): DCS(A) - Standby Calendar RAM

       util_schd will provide the WRR scheduler contents for use with DCS.  The
       calling code will then need to associate destination channel ID with each
       of these.  The WRR scheduler can schedule a queue or an MDRR group.
    */

    util_schd_dcs_calendar_entry_t *calendar = dcs_entries;
    UINT32 i = 0;
    UINT32 rd_queue_id;

    PMC_ENTRY();

    for (i=0;i<entries;i++)
    {
        if(dist_entries[i] == 0xFF)
        {
            calendar[i].c_ram_identifier = 0x7F;
            calendar[i].c_ram_channel = 0;
            calendar[i].c_ram_sf1 = 0;
            calendar[i].c_ram_sf2 = 0;
        }
        else
        {
            rd_queue_id = dist_entries[i];
            calendar[i].c_ram_channel = h->var.schd_ctxt->chnl_ctxt[rd_queue_id].dcs_rd_channel_id;
            calendar[i].c_ram_identifier = rd_queue_id;
            calendar[i].c_ram_sf1 = h->var.schd_ctxt->chnl_ctxt[rd_queue_id].dcs_sf1_push_port_id;
            calendar[i].c_ram_sf2 = h->var.schd_ctxt->chnl_ctxt[rd_queue_id].dcs_sf2_wr_queue_id;
        }

    }

    PMC_RETURN(PMC_SUCCESS);
} /* util_schd_dcs_format_gen */


/*******************************************************************************
* util_schd_chnl_dist_gen
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates sequence of calendar entries to meet distribution requirements.
*   Distributes entries based on the entry requirements specified in the
*   assign_entries data.
*
*   For DCS scheduler C_RAM_CHANNEL is invalid for MDRR groups.  In the
*   case of calendar entries that define MDRR groups instead of channels is
*   required, bits [31:28] will contain the MDRR Group ID (0 to 11) and
*   bit[27] will define if the entry is a regular channel entry (bit[27] == 0)
*   or an MDRR group entry (bit[27] == 1).
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id of the channels to be formed into
*                                     distributed list
*   chnl_id                         - channel ID to be written in DB
*   num_dist_entries                - number of scheduler entries to distribute
*                                     channel entries against
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_chnl_dist_gen(util_schd_handle_t *h,
                                         UINT32 grp_id,
                                         UINT32 chnl_id,
                                         UINT32 num_dist_entries,
                                         UINT32 *dist_entries)
{
    UINT32 alloc_method = h->cfg.block_ctxt.cal_entries_alloc_method;

    PMC_ENTRY();

    /* based on constant block context we can select which distribution
       mode to use */

    /* protect us from users with uninitialzed memory */   
    if( alloc_method == UTIL_SCHD_ALLOC_METHOD_UNUSED || alloc_method >= MAX_CAL_DISTRIBUTION_METHODS )
    {
        alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL;
        h->cfg.block_ctxt.cal_entries_alloc_method = UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL;
    }

        
    switch(h->cfg.block_ctxt.cal_entries_alloc_method)
    {
        case UTIL_SCHD_ALLOC_METHOD_GENERAL_LOCAL:
            util_schd_group_dist_gen_method_1(h,
                                              grp_id,
                                              num_dist_entries,
                                              dist_entries);
            break;

        case UTIL_SCHD_ALLOC_METHOD_LINEOTN_LOCAL:
            util_schd_group_dist_gen_method_2(h,
                                              grp_id,
                                              num_dist_entries,
                                              dist_entries);
            break;
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_ODUKSC:
            util_schd_group_dist_gen_db(h, 
                                        UTIL_GEN_DB_ODUKSC_DB, 
                                        UTIL_GEN_DB_ACCESSOR_ODUKSC,
                                        dist_entries);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMA:
            util_schd_group_dist_gen_db(h, 
                                        UTIL_GEN_DB_MPMA_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMA,
                                        dist_entries);
            break; 
                   
        case UTIL_SCHD_ALLOC_METHOD_MAPOTN_MPMO:
            util_schd_group_dist_gen_db(h, 
                                        UTIL_GEN_DB_MPMO_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMO,
                                        dist_entries);
            break;  
        case UTIL_SCHD_ALLOC_METHOD_CBRC:
            util_schd_group_dist_gen_method_6(h,
                                              grp_id,
                                              num_dist_entries,
                                              dist_entries);
            break;

        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA: /* MPMA_COREOTN */ /* case 7 - 13 for COREOTN need to be reviewed and determine if we can just look at one db */
            util_schd_chnl_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                       UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN,
                                       chnl_id,
                                       dist_entries);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_FROM_MAPOTN: /* MPMA_COREOTN_FROM_MAPOTN */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_ODUKSC_DB, /* fix to TX when TX_DB is filled */
                                        UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN,
                                        dist_entries);
            break;       
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMA_MPMO_CTL: /* MPMA_MPMO_COREOTN_CTL */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINEOTN_SYSOTN_MODE, 
                                        UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL,
                                        dist_entries);
            break; 
            
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_MUX_FRAMER: /* MPMO_COREOTN_MUX_FRAMER */
            util_schd_chnl_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_TX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER,
                                        chnl_id,
                                        dist_entries);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_MPMO_TO_MAPOTN: /* MPMO_COREOTN_TO_MAPOTN */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_MPMA_DB, 
                                        UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN,
                                        dist_entries);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC1: /* COREOTN_ODUKSC1 */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1,
                                        dist_entries);
        
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_COREOTN_ODUKSC2: /* COREOTN_ODUKSC2 */
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_LINE_CORE_OTN_RX_DB, 
                                        UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2,
                                        dist_entries);
            break;

        case UTIL_SCHD_ALLOC_METHOD_CPB_DCS:
            util_schd_group_dist_gen_db(h, h->cfg.block_ctxt.db_id, 
                                        UTIL_GEN_DB_ACCESSOR_CPB_DCS,
                                        dist_entries);
            break; 
        
        case UTIL_SCHD_ALLOC_METHOD_CBRC_SCBS3_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_CBRC_SCBS3, 
                                        UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3,
                                        dist_entries);
            break; 

        case UTIL_SCHD_ALLOC_METHOD_CBRC_MPMO_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_CBRC_MPMO, 
                                        UTIL_GEN_DB_ACCESSOR_CBRC_MPMO,
                                        dist_entries);
            break;         

        case UTIL_SCHD_ALLOC_METHOD_ENET_LINE_MPMA_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_ENET_LINE_MPMA, 
                                        UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA,
                                        dist_entries);
            break;     

        case UTIL_SCHD_ALLOC_METHOD_ENET_SYS_MPMA_DB:
            util_schd_group_dist_gen_db(h, UTIL_GEN_DB_ENET_SYS_MPMA, 
                                        UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA,
                                        dist_entries);
            break;                 
        default:
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,h->cfg.block_ctxt.cal_entries_alloc_method,0);
            break;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_chnl_dist_gen */


/*
** Private Functions
*/

/*******************************************************************************
* util_schd_grp_entry_eval_method_1
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Evaluates the entry requirements for all channels associated with a
*   scheduler group instance for the number of scheduler entries specified by
*   the calling function.
*
*   If not enough scheduler entries available to safisfy requirements, returns
*   error and channel context unmodified by function.  If successful min_entries,
*   max_entries and alloc_entries in channel context updated.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id within scheduler to have entries
*                                     evaluated
*   entries                         - number of scheduler entries to evaluate
*                                     channel entry requirements
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_grp_entry_eval_method_1(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 entries)
{

    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    UINT32 i;
/*    UINT32 mdrr_grp_in_use = 0; */
    UINT32 total_rate;
    util_schd_chnl_rate_exp_t total_rate_units;
    UINT32 entry_rate;
    INT32 entry_rate_units;
    UINT32 min_entries;
    INT32 min_entry_units;
    UINT32 used_entries = 0;
    UINT32 result;

    PMC_ENTRY();

    /* Identifying all   */
    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;
    total_rate = h->cfg.block_ctxt.total_rate;
    total_rate_units = h->cfg.block_ctxt.total_rate_units;

    /* Calculate the rate per entry */
    result = util_schd_math_division(UTIL_SCHD_MATH_DIVISION_RAW,
                                     total_rate, total_rate_units,
                                     entries, 0,
                                     &entry_rate, &entry_rate_units);                           

    /* set eval entries to 0 for all channels */
    for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        p_chnl_ctxt[i].eval_entries = 0;
    }

    for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        if( p_chnl_ctxt[i].grp_id == grp_id &&
            p_chnl_ctxt[i].state != UTIL_SCHD_CHNL_START)
        {
            /* Calculate the number of entries required for the specified rate */
            result = util_schd_math_division(UTIL_SCHD_MATH_DIVISION_CEILING,
                                             p_chnl_ctxt[i].rate, p_chnl_ctxt[i].rate_units,
                                             entry_rate, entry_rate_units,
                                             &min_entries, &min_entry_units);
            if(min_entry_units != 0) PMC_RETURN(UTIL_SCHD_ERR_CALCULATION_FAIL);
            PMC_LOG_TRACE("\nmin_entries=%u\n",min_entries);

            p_chnl_ctxt[i].eval_entries = min_entries;
            used_entries += min_entries;


        }
    }

    /* evaluate if sufficient entries provided to the group to support the
       requested configuration */
    if(used_entries > entries) PMC_RETURN(UTIL_SCHD_ERR_INSUFFICIENT_ENTRIES);

    /* update assign_entries for all channels within the group */
    for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        if( p_chnl_ctxt[i].grp_id == grp_id &&
            p_chnl_ctxt[i].state != UTIL_SCHD_CHNL_START)
        {
            p_chnl_ctxt[i].assign_entries = p_chnl_ctxt[i].eval_entries;
        }
    }

    PMC_RETURN(PMC_SUCCESS);
} /* util_schd_grp_entry_eval_method_1 */

/*******************************************************************************
* util_schd_group_dist_gen_method_1
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates sequence of calendar entries in a distributed manner.
*   Distributes entries based on the entry requirements specified in the
*   assign_entries data.
*
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id of the channels to be formed into
*                                     distributed list
*   num_dist_entries                - number of scheduler entries to distribute
*                                     channel entries against
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_group_dist_gen_method_1(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 num_dist_entries,
                                                   UINT32 *dist_entries)
{

    /* reference hyphy20g_cpb.c line 13443 - this code reimpliments the distribution
       algorithm used in HyPHY 20G CPB subsystem */

    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;
    UINT32 num_grp_chnls = 0;
    UINT32 incr_num_grp_chnls = 0;
    UINT32 num_half_entries;
    UINT32 i, j, k, /* size, */ swap, tmp_uint32;
    UINT32 *p_linear_cal = NULL;
    UINT32 *p_empty_loc = NULL;
    util_schd_chnl_ctxt_t *p_sort_chnl_ctxt = NULL, /* *p_temp_chnl_ctxt = NULL, */ tmp;
    UINT32 populate_entries;
    UINT32 populate_chnl;
    UINT32 cal_space;
    UINT32 cal_singles;
    UINT32 empty_loc_offset;
    UINT32 linear_cal_offset;
    UINT32 num_empty_loc;
    UINT32 new_num_empty_loc;
    util_schd_chnl_ctxt_t *p;
    
    PMC_ENTRY();

    /* reference preallocated memory pool and assign local pointers*/
    p_sort_chnl_ctxt = util_schd_mem_pool_assign(h);
    p_linear_cal = (UINT32 *)p_sort_chnl_ctxt;
    p_linear_cal += (sizeof(util_schd_chnl_ctxt_t)*(h->cfg.block_ctxt.num_schd_chnl)) / sizeof(UINT32);    
    p_empty_loc = (UINT32 *)(p_linear_cal + h->cfg.block_ctxt.max_entries);
    

    /* collect the set of channels that are in operational state */
    p = p_sort_chnl_ctxt;
    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;
    for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        if( p_chnl_ctxt[i].grp_id == grp_id &&
            p_chnl_ctxt[i].state == UTIL_SCHD_CHNL_OPERATIONAL)
        {
            p->grp_id = p_chnl_ctxt[i].grp_id;
            p->state = p_chnl_ctxt[i].state;
            p->chnl_id = p_chnl_ctxt[i].chnl_id;
            p->assign_entries = p_chnl_ctxt[i].assign_entries;
            num_grp_chnls++;
            p++;
        }
    }

    for(i=0; i<num_dist_entries; i++)
    {
        p_linear_cal[i] = h->cfg.block_ctxt.entry_null_value;
    }

    /* Only do the distribution if the calendar is not all NULL */
    if (num_grp_chnls != 0) {
        for(i=0; i<num_dist_entries; i++)
        {
            *(p_empty_loc + i) = i;
        }
        num_empty_loc = new_num_empty_loc = num_dist_entries;

        /* test number of entries per channel, if a channel used > 50% of entries
           it must be split into two elements for future algorithm processing */
        num_half_entries = num_dist_entries / 2;
        for(i=0; i < num_grp_chnls; i++){
            if(p_sort_chnl_ctxt[i].assign_entries > num_half_entries)
            {
                p_sort_chnl_ctxt[i].assign_entries            -= num_half_entries;
                p_sort_chnl_ctxt[num_grp_chnls].assign_entries = num_half_entries;
                p_sort_chnl_ctxt[num_grp_chnls].grp_id         = p_sort_chnl_ctxt[i].grp_id;
                p_sort_chnl_ctxt[num_grp_chnls].chnl_id        = p_sort_chnl_ctxt[i].chnl_id;
                p_sort_chnl_ctxt[num_grp_chnls].state          = p_sort_chnl_ctxt[i].state;
                incr_num_grp_chnls = 1;

            }
        }

        if(incr_num_grp_chnls == 1) num_grp_chnls++;


        /* sort channels in group from highest to lowest assign_entries */
        do {
            swap = 0;
            for(i=0; i < num_grp_chnls-1; i++) {
                j = i + 1;
                if (p_sort_chnl_ctxt[j].assign_entries > p_sort_chnl_ctxt[i].assign_entries) {
                    /* swap */ /* not the right swap we need to swap the entire array contents */
                    util_schd_chnl_ctxt_copy(&tmp, &p_sort_chnl_ctxt[i]);
                    util_schd_chnl_ctxt_copy(&p_sort_chnl_ctxt[i], &p_sort_chnl_ctxt[j]);
                    util_schd_chnl_ctxt_copy(&p_sort_chnl_ctxt[j], &tmp);

                    swap = 1;
                }
            }
        } while (swap);

        /* distribute entries from sorted data to p_linear_cal */
        for (i=0; i < num_grp_chnls; i++) {
    /*        PMC_LOG_TRACE("DEBUG_MESSAGE 1: num_empty_loc = %lu, new_num_empty_loc = %lu\n", num_empty_loc, new_num_empty_loc); */

            populate_entries = p_sort_chnl_ctxt[i].assign_entries; /* populate entries */
            populate_chnl = p_sort_chnl_ctxt[i].chnl_id;
            /* identify the required number of calendar entries between each
               occurance of this channel number */
            PMC_ASSERT(populate_entries != 0, UTIL_SCHD_ERR_DIVIDE_BY_ZERO, 0, 0);
            cal_space = num_empty_loc/populate_entries;
            cal_singles = num_empty_loc%populate_entries;
            if (cal_space == 0) {
                PMC_LOG_TRACE("DEBUG_MESSAGE 1a: cal_space forced to 1\n");
                cal_space = 1;
            }

            PMC_LOG_TRACE("DEBUG_MESSAGE 2: LINE %d\n", __LINE__);
            empty_loc_offset = 0;
            for (j=0; j < populate_entries; j++) {
                /* fetch location of empty entry, alternating between start and
                   end of list */
                if(j==0) {
                    empty_loc_offset = 0;
                } else {
                    if(cal_singles){
                        empty_loc_offset += (cal_space+1);
                        cal_singles--;
                    } else {
                        empty_loc_offset += cal_space;
                    }
                }
                linear_cal_offset = *(p_empty_loc + empty_loc_offset);

                /* Populate calendar with channel number */
               *(p_linear_cal + linear_cal_offset) = populate_chnl;

               /* remove empty location from list */
               *(p_empty_loc + empty_loc_offset) = 0xFFFF;
               new_num_empty_loc--;
            }

            PMC_LOG_TRACE("DEBUG_MESSAGE 3: LINE %d\n", __LINE__);
            /* push all empty locations to front of list while maintaining order */
            do {
                swap = 0;
                for(j=0; j < num_dist_entries-1; j++) {
                    k = j + 1;
                    if (*(p_empty_loc + k) < *(p_empty_loc + j)) {
                        /* swap */
                        tmp_uint32 = *(p_empty_loc + j);
                        *(p_empty_loc + j) = *(p_empty_loc + k);
                        *(p_empty_loc + k) = tmp_uint32;

                        swap = 1;
                    }
                }
            } while (swap);

            num_empty_loc = new_num_empty_loc;
        }
    } /* if (num_grp_chnls != 0) { */

    /* transfer p_linear_cal to the output argument UINT32 *dist_entries[]*/
    for(i=0; i<num_dist_entries; i++)
    {
        dist_entries[i] = p_linear_cal[i];
    }

    util_schd_mem_pool_release(h);

    PMC_RETURN(PMC_SUCCESS);
} /* util_schd_group_dist_gen_method_1 */

/*******************************************************************************
* util_schd_grp_entry_eval_method_2
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Evaluates the entry requirements for all channels associated with a
*   scheduler group instance for the number of scheduler entries specified by
*   the calling function.
*
*   If not enough scheduler entries available to safisfy requirements, returns
*   error and channel context unmodified by function.  If successful min_entries,
*   max_entries and alloc_entries in channel context updated.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id within scheduler to have entries
*                                     evaluated
*   entries                         - number of scheduler entries to evaluate
*                                     channel entry requirements
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_grp_entry_eval_method_2(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 entries)
{
    /* uses the same eval method as algoritm #1 */
    PMC_ERROR result; 
    PMC_ENTRY();
    result = util_schd_grp_entry_eval_method_1(h,grp_id,entries);
    PMC_RETURN(result);
}/* util_schd_grp_entry_eval_method_2 */


/*******************************************************************************
* util_schd_group_dist_gen_method_2
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates sequence of 96 calendar entries in a distributed manner.
*   Distributes entries are spread over 8 groups of 12 entries each. \n
*   The following distribution hierachy for the 3 different rates (10G,40G,100G)
*   is implemented in order to create a "perfectly scheduled timeslots": \n
*
*       if a 100G is allocated - this is the only alocation used.
*       if no 100G is allocated - allocate first the 40G channels,and last allocate
*       10G channels if present. \n
*       This Hierarchy is implemented because of a HW requirment that the
*       40G time slots will spaced 3 timeslots apart within agroup of 12 time slots.\n
*
*   few examples:\n
*   100G alloction: timeslots 0-4 and 6-10 duplicated over 8 groups of 12 timeslots:\n
*   0-4,6-10,        1st group of 12 time slots \n
*   12-16,18-22      2nd group of 12 time slots \n
*   24-28,30-34,     3rd group of 12 time slots \n
*   36-40,42-46,                                \n
*   48-52,54-58,                                \n
*   60-64,66-70,                                \n
*   72-76,78-82                                 \n
*   84-88,90-94      8th group of 12 time slots \n
*
*   40G allocation: 4 timeslots spaced 3 time slots apart and duplicated
*   over 8 groups of 12 timeslots: \n
*   0,3,6,9,        1st group of 12 time slots  \n
*   12,15,18,21,    2nd group of 12 time slots  \n
*   24,27,30,33,    3rd group of 12 time slots  \n
*                                               \n  
*                                               \n
*   84,87,90,93,    8th group of 12 channels    \n
*
*   Second 40G allocation: 4 timeslots spaced 3 time slots apart and duplicated
*   over 8 groups of 12 timeslots:              \n
*   1,4,7,10,       1st group of 12 time slots  \n
*   13,16,119,22,   2nd group of 12 time slots  \n
*   25,28,31,34,    3rd group of 12 time slots  \n
*                                               \n
*                                               \n
*   85,88,91,94,    8th group of 12 time slots  \n
*
*   10G requires 1 calendar entries that will be distributed in the following way
*   ( assuming that the first available entry is 0 and there are no 40G allocations ): \n
*   0,12,24,36,48,60,72,84 \n
*   second 10G will be disributed to: 1,13,25,37,49,61,73,85 \n
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id of the channels to be formed into
*                                     distributed list
*   num_dist_entries                - number of scheduler entries to distribute
*                                     channel entries against
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_group_dist_gen_method_2(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 num_dist_entries,
                                                   UINT32 *dist_entries)
{
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;
    UINT32 i,k,j;
    UINT32 req_entries = 0;
    UINT32 curr_assigned_entry = 0;
    UINT32 rate_100G_bm = 0;
    UINT32 rate_40G_bm = 0;
    UINT32 rate_10G_bm = 0;
    UINT32 null_value = h->cfg.block_ctxt.entry_null_value;

    PMC_ENTRY();

    /* Go over equiped channels and look at the number of enteries
       each channel requires */
    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    /* reset all entries to be null value entries */
    for(i=0; i<num_dist_entries; i++)
    {
        dist_entries[i] = null_value;
        p_chnl_ctxt [i].allocated_ho_channel = h->cfg.block_ctxt.entry_null_value;
    }

    /* first step - map how many channel from each rate in to bit masks */
    for(i=0; i < h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        if( p_chnl_ctxt[i].grp_id == grp_id &&
            p_chnl_ctxt[i].state == UTIL_SCHD_CHNL_OPERATIONAL)
        {
            req_entries = (p_chnl_ctxt[i].assign_entries/8);
            if(p_chnl_ctxt[i].assign_entries%8 > 0) req_entries++;

            switch(req_entries)
            {
                case 1:  /* 10G channel */
                    rate_10G_bm |= (1<<i);
                    break;

                case 4:  /* 40G channel */
                    rate_40G_bm |= (1<<i);
                    break;

                case 10:  /* 100G channel */
                    rate_100G_bm |= (1<<i);
                    break;

                default:
                    PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,req_entries,0);
                    break;

            }

        }
    }

    /* allocate by the map created - keep the following hirarchy: */
    /* if a 100G is allocated - this is the only alocation used     */
    /* if no 100G is allocated - allocate first the 40G channels  */
    /* and last allocate 10G channels if present                  */
    if( rate_100G_bm > 0 )
    {
        /* find the 100G channel id ( there should be only 1 ) */
        /* max channels is 12                                  */
        for(i=0;i<12;i++)
            if( (rate_100G_bm & (1<<i)) > 0 )
                break;

        /* Sanity check */
        if( i >= 12 )
            PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,i,0);

        /* for "perfectly scheduled timeslots" we must skip */
        /* the sixth time slot and the 12th time slot       */
        /* in every group of 12 time slots.                 */
        /* See example in the documentation above           */
        for( k=0; k < 11 ; k++ )
        {
            if( 5 == k )
                continue;

            /* assign 8 entries per every required entry */
            /* space the entries 12 entries apart        */
            for( j=0 ; j < 8 ; j++ )
            {
                dist_entries[k + 12*j] = i;
                p_chnl_ctxt [k + 12*j].allocated_ho_channel = i;
            }

        }
    }
    else /* 40G + 10G allocations */
    {
        if( rate_40G_bm > 0 )
        {

            /* find the 40G channel id's ( there should be max 3 ) */
            /* max channels is 12                                  */
            for(i=0;i < 12; i++)
            {
                if( (rate_40G_bm & (1<<i)) > 0 )
                {
                    /* There is a direct link between the dci channel id to the  */
                    /* calendar slots --> dci ch id = calendar slot              */
                    /* first 12 entries are duplicted 8 times and this is why we */
                    /* look only over 12 entries                                 */

                    /* vefify that we have 4 free entries spaced 3 channels apart */
                    if( i<12 && 
                        dist_entries[i + 0] == null_value &&
                        dist_entries[i + 3] == null_value &&
                        dist_entries[i + 6] == null_value &&
                        dist_entries[i + 9] == null_value )
                    {
                        curr_assigned_entry = i;
                    }
                    else
                    {
                        PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,i,0);
                    }

                    /* all is good - start allocating */
                    for( k=0; k < 4 ; k++ )
                    {
                        /* assign 8 entries per every required entry */
                        /* space the entries 12 entries apart        */
                        for( j=0 ; j < 8 ; j++ )
                        {
                            dist_entries[curr_assigned_entry + 12*j] = i;
                            p_chnl_ctxt [curr_assigned_entry + 12*j].allocated_ho_channel = i;
                        }

                        /* next 8 entries - in 40G need to be 3 time slots apart */
                        /* for "perfectly scheduled timeslots"                   */
                        curr_assigned_entry+=3;
                    }
                }
            }
        }

        /* finished 40G allocation - allocate 10G if we have any */
        if( rate_10G_bm > 0 )
        {

            /* find the 10G channel id's ( there should be max 12 ) */
            /* max channels is 12                                   */
            for(i=0;i < 12; i++)
            {
                if( (rate_10G_bm & (1<<i)) > 0 )
                {
                    /* There is a direct link between the dci channel id to the  */
                    /* calendar slots --> dci ch id = calendar slot              */
                    /* first 12 entries are duplicted 8 times and this is why we */
                    /* look only over 12 entries                                 */

                    /* vefify that we have valid entry  */
                    if( i < 12 )
                    {
                        curr_assigned_entry = i;
                    }
                    else
                    {
                        PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,i,0);
                    }

                    /* all is good - start allocating */
                    /* assign 8 entries per every required entry */
                    /* space the entries 12 entries apart        */
                     for( j=0 ; j < 8 ; j++ )
                     {
                        dist_entries[curr_assigned_entry + 12*j] = i;
                        p_chnl_ctxt [curr_assigned_entry + 12*j].allocated_ho_channel = i;
                     }
                }
            }
        }

    }

    PMC_RETURN(PMC_SUCCESS);
}/* util_schd_group_dist_gen_method_2 */



/*******************************************************************************
* util_schd_grp_entry_eval_method_6
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Evaluates the entry requirements for all channels associated with a
*   scheduler group instance for the number of scheduler entries specified by
*   the calling function.
*
*   If not enough scheduler entries available to safisfy requirements, returns
*   error and channel context unmodified by function.  If successful min_entries,
*   max_entries and alloc_entries in channel context updated.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id within scheduler to have entries
*                                     evaluated
*   entries                         - number of scheduler entries to evaluate
*                                     channel entry requirements
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_grp_entry_eval_method_6(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 entries)
{
    /* uses the same eval method as algoritm #1 */
    PMC_ERROR result;
    PMC_ENTRY();
    result = util_schd_grp_entry_eval_method_1(h,grp_id,entries);
    PMC_RETURN(result);

} /* util_schd_grp_entry_eval_method_6 */




/*******************************************************************************
* util_schd_group_dist_gen_method_6
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Generates sequence of 96 calendar entries in a distributed manner for the 
*   CBRC. The CBRC only has 12 channels, which will be distributed evenly as
*   show below:
*
*   Schd Entry     Channel
*   1               1    
*   2               2    
*   3               3
*   4               4
*   5               5
*   6               6
*   7               7
*   8               8
*   9               9
*   10              10
*   11              11
*   12              12
*   13              1
*   14              2
*   15              3
*   16              4
*   17              5
*   ...
*
*   10G signals will require one channel (8 schd entries), while 40G signals
*   will require four channels (32 schd entries).
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   grp_id                          - grp_id of the channels to be formed into
*                                     distributed list
*   num_dist_entries                - number of scheduler entries to distribute
*                                     channel entries against
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_group_dist_gen_method_6(util_schd_handle_t *h,
                                                   UINT32 grp_id,
                                                   UINT32 num_dist_entries,
                                                   UINT32 *dist_entries)
{
    PMC_ERROR               result = PMC_SUCCESS;
    util_schd_chnl_ctxt_t  *p_chnl_ctxt = NULL;
    UINT32                  i, j;
    UINT32                  entry_val;
    UINT32                  req_entries;
    UINT32                  null_value = h->cfg.block_ctxt.entry_null_value;
    UINT8                   user_bits; /* selects source into PKT_INT */

    PMC_ENTRY();

    /* Go over equiped channels and look at the number of enteries
       each channel requires */
    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    /* reset all entries to be null value entries */
    for (i = 0; i < num_dist_entries; i++)
    {
        dist_entries[i] = null_value;
        p_chnl_ctxt [i].allocated_ho_channel = h->cfg.block_ctxt.entry_null_value;
    }

    /* first step - map how many channel from each rate in to bit masks */
    for (i = 0; i < h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        if( p_chnl_ctxt[i].grp_id == grp_id &&
            p_chnl_ctxt[i].state == UTIL_SCHD_CHNL_OPERATIONAL)
        {
            /* Determine if we are assigning a 10G or 40G signal
             * 10G = 8 entries in 96 slot calendar
             * 40G = 32 entries in 96 slot calendar
             */
            req_entries = (p_chnl_ctxt[i].assign_entries/8);

            /* Round up - this is due to the total rate of the calendar
             * being assigned slightly more than 120G */
            if(p_chnl_ctxt[i].assign_entries%8 > 0) req_entries++;

            /* User bits specify source of traffic into PKT_INT SCBS3
             * This bit is placed in the top nibble of the channel id
             * 1 - source from MPMA
             * 0 - source from FCS1200
             *
             * MPMO scheduler will always have user_bits = 0
             */
            user_bits = p_chnl_ctxt[i].scbs_user_bits;
            entry_val = (user_bits << 4) | i;

            switch (req_entries)
            {
                case 1:
                    /* Set 8 entries for 10G */
                    for (j = 0; j < 8; j++)
                    {
                       dist_entries[i + j*12] = entry_val;
                    }
                    break;
                
                case 4:
                    /* Set 32 entries for 10G */
                    for (j = 0; j < 8; j++)
                    {
                       dist_entries[i   + j*12] = entry_val;
                       dist_entries[i+1 + j*12] = entry_val;
                       dist_entries[i+2 + j*12] = entry_val;
                       dist_entries[i+3 + j*12] = entry_val;
                    }
                    break;
                
                default:
                    PMC_ASSERT(FALSE,PMC_ERR_INVALID_PARAMETERS,req_entries,0);
                    break;
            } /* end switch */
            
        }
    }

    PMC_RETURN(result);
} /* util_schd_group_dist_gen_method_6 */


/*******************************************************************************
* util_schd_grp_entry_eval_db
* ______________________________________________________________________________
*
* DESCRIPTION:
*    For operational or equipped channels it searched throught the specific database 
*    database based on the database ID and checkes if the channel exists in the 
*    database.
*  
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   db_id                           - database ID to access
*   inst                            - database instance of the type ID to access.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_grp_entry_eval_db(util_schd_handle_t *h, 
                                             util_gen_db_id_t db_id, 
                                             util_gen_db_accessor_inst_t inst)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    result = util_schd_grp_entry_eval_db_internal(h, 
                                                  db_id, 
                                                  inst,
                                                  FALSE);
            
    PMC_RETURN(result);

}/* util_schd_grp_entry_eval_db */

/*******************************************************************************
* util_schd_grp_entry_eval_db_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*    For operational or equipped channels it searched throught the specific 
*    database based on the database ID and checkes if the channel exists in the 
*    database.
*  
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   db_id                           - database ID to access
*   inst                            - database instance of the type ID to access.
*   do_clean                        - TRUE:  if channel in EQUIPPED/OPERATIONAL 
*                                            state but not in database, move 
*                                            channel to START state \n
*                                     FALSE: if channel in EQUIPPED/OPERATIONAL 
*                                            state but not in database, error 
*                                            out of function
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR util_schd_grp_entry_eval_db_internal(util_schd_handle_t *h, 
                                                       util_gen_db_id_t db_id, 
                                                       util_gen_db_accessor_inst_t inst,
                                                       BOOL8 do_clean)
{
    /* Variable declaration */
    UINT32 i;
    UINT32 num_of_recs;
    /*UINT32 db_handle;*/
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    /* Variables for mapotn */
    mapotn_db_key mapotn_key;

    /* Variables for line_core_otn */    
    line_core_otn_db_key line_core_otn_key;

    line_core_otn_db_key two_stage_key;

    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();

    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    PMC_ASSERT(p_chnl_ctxt != NULL, UTIL_SCHD_ERR_NULL_PTR, 0, 0);
    
    PMC_LOG(PMC_LOG_SEV_LOWEST, UTIL_SCHD_LOG_CODE_LOG, 0, 0, "db_id = %u\n", db_id);  
    
    /*db_handle = gen_db_attach(&(h->base), db_id);*/

    PMC_ASSERT(inst < LAST_UTIL_GEN_DB_ACCESSOR, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    
    for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
    {  
        if(p_chnl_ctxt[i].state == UTIL_SCHD_CHNL_OPERATIONAL || p_chnl_ctxt[i].state == UTIL_SCHD_CHNL_EQUIPPED)
        {
            if (inst == UTIL_GEN_DB_ACCESSOR_ODUKSC 
                || inst == UTIL_GEN_DB_ACCESSOR_MPMA 
                || inst == UTIL_GEN_DB_ACCESSOR_MPMO
                || inst == UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3
                || inst == UTIL_GEN_DB_ACCESSOR_CBRC_MPMO
                || inst == UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA
                || inst == UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA
                ) 
            {
                mapotn_key.calendar_entry = DO_NOT_USE_KEY;
                mapotn_key.channel_num = i;
                
                PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_ODUKSC/UTIL_GEN_DB_ACCESSOR_MPMA/UTIL_GEN_DB_ACCESSOR_MPMO/UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3/UTIL_GEN_DB_ACCESSOR_CBRC_MPMO\n");
                PMC_LOG_TRACE("mapotn_key.calendar_entry = %u, mapotn_key.channel_num = %u\n", mapotn_key.calendar_entry, mapotn_key.channel_num);
                
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key);

            }
            else if (inst == UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1) 
            {
                /* first stage - schedule for each HO channel */
                PMC_LOG_TRACE("Channel i = %u is operational or equipped\n", i);

                line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
                line_core_otn_key.ho_channel = DO_NOT_USE_KEY; 
                line_core_otn_key.mo_channel = i;
                line_core_otn_key.lo_channel = DO_NOT_USE_KEY;
                line_core_otn_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL/UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1\n");
                PMC_LOG_TRACE("key.calendar_entry = %u, key.mo_channel = %u\n", line_core_otn_key.calendar_entry, line_core_otn_key.mo_channel);
                           
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key);
                
                /* No muxing - schedule for each HO channel */
                if (num_of_recs == 0)
                {
                    line_core_otn_db_key no_stage_key;
                    
                    no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                    no_stage_key.ho_channel = i;
                    no_stage_key.mo_channel = DO_NOT_USE_KEY;
                    no_stage_key.lo_channel = DO_NOT_USE_KEY;
                    no_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                    PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL/UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1\n");
                    PMC_LOG_TRACE("key.calendar_entry = %u, key.ho_channel = %u\n", no_stage_key.calendar_entry, no_stage_key.ho_channel);
                       
                    num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key);

                    
                }    
            }
            else if(inst == UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL)
            {

                mapotn_key.calendar_entry = DO_NOT_USE_KEY;
                mapotn_key.channel_num = i;
                
                PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL \n");
                PMC_LOG_TRACE("key.calendar_entry = %u, key.channel_num = %u\n", mapotn_key.calendar_entry, mapotn_key.channel_num);
                PMC_LOG_TRACE("h->base = %p, db_id = %u, h->cfg.db_handle=%u\n", &(h->base), db_id, h->cfg.db_handle);
                
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key);
                
            }
            else if (inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN 
                     || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN)
            {
                PMC_LOG_TRACE("UTIL_SCHD: channel provisioned is = %u\n", i);   

                mapotn_key.calendar_entry = DO_NOT_USE_KEY;
                mapotn_key.channel_num = i;
                
                PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN / UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN\n");
                PMC_LOG_TRACE("key.calendar_entry = %u, key.channel_num = %u\n", mapotn_key.calendar_entry, mapotn_key.channel_num);
                PMC_LOG_TRACE("h->base = %p, db_id = %u, h->cfg.db_handle=%u\n", &(h->base), db_id, h->cfg.db_handle);
                
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key);
                
            }
            else if (inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN
                     || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER)
            {     
                PMC_LOG_TRACE("UTIL_SCHD: channel provisioned is = %u\n", i);   
                
                two_stage_key.calendar_entry = DO_NOT_USE_KEY;
                two_stage_key.ho_channel = DO_NOT_USE_KEY;
                two_stage_key.mo_channel = DO_NOT_USE_KEY;
                two_stage_key.lo_channel = i;
                two_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("1. Two stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN\n");
                PMC_LOG_TRACE("two_stage_key.lo_channel = %u\n", two_stage_key.lo_channel);
                           
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&two_stage_key);
                
                if (num_of_recs == 0)
                {
                    line_core_otn_db_key one_stage_key;
                
                    one_stage_key.calendar_entry = DO_NOT_USE_KEY;
                    one_stage_key.ho_channel = DO_NOT_USE_KEY;
                    one_stage_key.mo_channel = i;
                    one_stage_key.lo_channel = DO_NOT_USE_KEY;
                    one_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                    PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                    PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
                
                    num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&one_stage_key);
                    
                    if (num_of_recs == 0)
                    {     
                        line_core_otn_db_key no_stage_key;
                        
                        no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                        no_stage_key.ho_channel = i;
                        no_stage_key.mo_channel = DO_NOT_USE_KEY;
                        no_stage_key.lo_channel = DO_NOT_USE_KEY;
                        no_stage_key.trib_slot = DO_NOT_USE_KEY;
                        
                        PMC_LOG_TRACE("2. No stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                        PMC_LOG_TRACE("no_stage_key.ho_channel = %u", no_stage_key.ho_channel);
                        
                        num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key);  

                        
                    }

                 }
            }
            else if (inst == UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2)
            {
                line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
                line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
                line_core_otn_key.mo_channel = DO_NOT_USE_KEY;
                line_core_otn_key.lo_channel = i;
                line_core_otn_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2\n");
                PMC_LOG_TRACE("key.calendar_entry = %u, key.lo_channel = %u\n", line_core_otn_key.calendar_entry, line_core_otn_key.lo_channel);
                           
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key);
                /* No muxing - schedule for each HO channel */
                if (num_of_recs == 0)
                {
                    line_core_otn_db_key one_stage_key;
                
                    one_stage_key.calendar_entry = DO_NOT_USE_KEY;
                    one_stage_key.ho_channel = DO_NOT_USE_KEY;
                    one_stage_key.mo_channel = i;
                    one_stage_key.lo_channel = DO_NOT_USE_KEY;
                    one_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                    PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2\n");
                    PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
                
                    num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&one_stage_key);
                    
                    if (num_of_recs == 0)
                    {  
                        line_core_otn_db_key no_stage_key;
                        
                        no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                        no_stage_key.ho_channel = i;
                        no_stage_key.mo_channel = DO_NOT_USE_KEY;
                        no_stage_key.lo_channel = DO_NOT_USE_KEY;
                        no_stage_key.trib_slot = DO_NOT_USE_KEY;
                        
                        PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2\n");
                        PMC_LOG_TRACE("key.calendar_entry = %u, key.ho_channel = %u\n", no_stage_key.calendar_entry, no_stage_key.ho_channel);
                           
                        num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key);

                    }
                } 
            }
            else if (inst == UTIL_GEN_DB_ACCESSOR_CPB_DCS) 
            {
                mapotn_key.calendar_entry = DO_NOT_USE_KEY;
                mapotn_key.channel_num = p_chnl_ctxt[i].dcs_rd_channel_id;
                
                PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_CPB_DCS\n");
                PMC_LOG_TRACE("key.calendar_entry = %u, key.channel_num = %u\n", mapotn_key.calendar_entry, mapotn_key.channel_num);
                
                num_of_recs=gen_db_query_count(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key);
                
            }
            else 
            {
                PMC_RETURN(UTIL_SCHD_ERR_INVALID_ARG);
            }
                 
            PMC_LOG_TRACE("num_of_recs = %u\n", num_of_recs);
            
            if(num_of_recs > 0)
            {
                if (p_chnl_ctxt[i].state == UTIL_SCHD_CHNL_EQUIPPED && do_clean == TRUE)
                {
                    /* in crash mode, we set EQUIPPED channel to START state */
                    p_chnl_ctxt[i].state = UTIL_SCHD_CHNL_START;
                }
            }
            else
            {
                if(do_clean == FALSE)
                {
                    rc = UTIL_SCHD_ERR_INVALID_CH_STATE;
                } else {
                    /* set the channel state to start */
                    PMC_LOG_TRACE("util_schd_grp_entry_eval_db_internal - Setting channel %u back to start state\n",i);
                    p_chnl_ctxt[i].state = UTIL_SCHD_CHNL_START;
                }
            } 
        } 
    }
    
    PMC_RETURN(rc);

}/* util_schd_grp_entry_eval_db_internal */

/*******************************************************************************
* util_schd_group_dist_gen_db
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Accesses database based on database ID and checks through the database if 
*   channel state is OPERATIONAL it will transfers the scheduler contents 
*   from database to outputlist otherwise writes 0xFF to output list.  
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   db_id                           - database ID to access
*   inst                            - data base evaluation type processing
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PRIVATE void util_schd_group_dist_gen_db(util_schd_handle_t *h,
                                         util_gen_db_id_t db_id,  
                                         util_gen_db_accessor_inst_t inst,
                                         UINT32 *dist_entries)
{
    /* Variable declaration */
    UINT32                 i;
    UINT32                   num_of_recs;
    UINT32                 db_chnl_data;
    UINT32                 channel;
    UINT32                 null_value = h->cfg.block_ctxt.entry_null_value;
    UINT32                 rd_queue_id;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;

    /*Variables for mapotn db */
    mapotn_db_key          mapotn_key;
    mapotn_db_query_res    *mapotn_q_result = NULL;

    /*Variables for line_core_otn db */    
    line_core_otn_db_key       line_core_otn_key;
    line_core_otn_db_query_res *line_core_otn_q_result = NULL;
    BOOL8 is_schd_extra_channel = pmc_coreotn_schd_extra_channel_revision(&h->base);

    PMC_ENTRY();
    
    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;
    PMC_ASSERT(p_chnl_ctxt != NULL, UTIL_SCHD_ERR_NULL_PTR, 0, 0);
    
    PMC_LOG(PMC_LOG_SEV_LOWEST, UTIL_SCHD_LOG_CODE_LOG, 0, 0, "db_id = %u\n", db_id);  
    PMC_LOG(PMC_LOG_SEV_LOWEST, UTIL_SCHD_LOG_CODE_LOG, 0, 0, "inst = %u\n", inst); 
    
    PMC_ASSERT(inst < LAST_UTIL_GEN_DB_ACCESSOR, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    
    PMC_LOG_TRACE("h->cfg.block_ctxt.num_schd_chnl = %u\n", h->cfg.block_ctxt.num_schd_chnl);

    /* Do the DB query first */
    if (inst == UTIL_GEN_DB_ACCESSOR_CPB_DCS
        || inst == UTIL_GEN_DB_ACCESSOR_ODUKSC 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMA 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMO
        || inst == UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3
        || inst == UTIL_GEN_DB_ACCESSOR_CBRC_MPMO
        || inst == UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA
        || inst == UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA    
        || inst == UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL
        || inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN
        )
    {
        mapotn_key.calendar_entry = CALENDAR_ENTRIES_NUM;
        mapotn_key.channel_num = DO_NOT_USE_KEY;
        num_of_recs = gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key,(void **)&mapotn_q_result);
        PMC_ASSERT(num_of_recs >= h->cfg.block_ctxt.num_schd_chnl, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
    }
    else
    {
        line_core_otn_key.calendar_entry = CALENDAR_ENTRIES_NUM;
        line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
        line_core_otn_key.mo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.lo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.trib_slot  = DO_NOT_USE_KEY;
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);
        PMC_ASSERT(num_of_recs >= h->cfg.block_ctxt.num_schd_chnl, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
    }
        

    
    if (FALSE == is_schd_extra_channel &&
             (inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN 
              || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER))
    {     
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            /*
              db_chnl_data = q_result->p_recs[i]->lo_channel;
            */            
            if (line_core_otn_q_result->p_recs[i]->lo_channel == 0xFFFFFFFF)
            {        
                /* not two stage muxing */
                if (line_core_otn_q_result->p_recs[i]->mo_channel == 0xFFFFFFFF)
                {
                    /* if there is no trib_slot assigned for this calendar entry
                    ** this is one stage muxing. Calendar entry does not belong to 
                    ** the MO channel
                    */
                    if (line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }  
                    else 
                    { 
                        /* no stage muxing - index is HO */ 
                        if( line_core_otn_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data = 0xFFFFFFFF;
                        }
                        else 
                        {
                            db_chnl_data = line_core_otn_q_result->p_recs[i]->ho_channel;        
                        }   
                    }
                }
                else 
                {
                    if (line_core_otn_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data = line_core_otn_q_result->p_recs[i]->mo_channel;
                    }
                    else 
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }
                }
            }
            else 
            {
                /* two stage muxing - index is LO */
                db_chnl_data = line_core_otn_q_result->p_recs[i]->lo_channel;
            }
            
            
            /*if (db_chnl_data > UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE)
              {
              db_chnl_data = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
              }*/
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */
            PMC_ASSERT(i < 96, PMC_ERR_INVALID_PARAMETERS, 0, 0);
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else 
            {        
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } 
                else 
                {
                  dist_entries[i] = null_value;            
                }  
            }                
        }
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1) 
    {
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            /* entries are per HO channel basis */
            
            
            /*db_chnl_data = q_result->p_recs[i]->ho_channel;  */
            
            /* not one stage muxing */
            if (line_core_otn_q_result->p_recs[i]->mo_channel == 0xFFFFFFFF)
            {
                /* one stage muxing but filter out for LO channel 
                ** such that not all calendar entires for the HO
                ** is programmed
                */
                if (line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 0)
                {
                    db_chnl_data = 0xFFFFFFFF;
                }  
                else if (line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->cal_entry_status != 0)
                {
                    /* this is MO ODU3e1/3e2 case. Keep the 33rd in FMF1 */
                    db_chnl_data = line_core_otn_q_result->p_recs[i]->cal_entry_status;
                }
                else { 
                    if( line_core_otn_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }
                    else { 
                        /* no stage muxing - index is HO */
                        db_chnl_data = line_core_otn_q_result->p_recs[i]->ho_channel;
                    }                        
                }
            }
            else {
                /* one stage muxing - index is MO */
                if (line_core_otn_q_result->p_recs[i]->trib_slot > 79)
                {
                    db_chnl_data = 0xFFFFFFFF;                   
                }
                else if (line_core_otn_q_result->p_recs[i]->ho_channel == 0xFFFFFFFF)
                {
                    /* this is MO ODU3 case. Keep it to 31 in FMF1 */
                    db_chnl_data = 0xFFFFFFFF;
                }
                else { 
                    db_chnl_data = line_core_otn_q_result->p_recs[i]->mo_channel;
                }                    
            }
           
            
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */ 
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else {                      
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } else {
                    dist_entries[i] = null_value;            
                }  
            }                
            
        }
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2)
    {
                                                
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            /*
            db_chnl_data = q_result->p_recs[i]->lo_channel;
            */            
            if (line_core_otn_q_result->p_recs[i]->lo_channel == 0xFFFFFFFF)
            {        
                /* not two stage muxing */
                if (line_core_otn_q_result->p_recs[i]->mo_channel == 0xFFFFFFFF)
                {
                    /* if there is no trib_slot assigned for this calendar entry
                    ** this is one stage muxing. Calendar entry does not belong to 
                    ** the MO channel
                    */
                    if (line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }  
                    else { 
                        /* no stage muxing - index is HO */ 
                        if( line_core_otn_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data = 0xFFFFFFFF;
                        }
                        else {
                            db_chnl_data = line_core_otn_q_result->p_recs[i]->ho_channel;        
                        }
                    
                    }
                }
                else {
                    if (line_core_otn_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data = line_core_otn_q_result->p_recs[i]->mo_channel;
                    }
                    else {
                        db_chnl_data = 0xFFFFFFFF;
                    }
                }
            }
            else {
                /* two stage muxing - index is LO */
                db_chnl_data = line_core_otn_q_result->p_recs[i]->lo_channel;
            }
            
            
            /*if (db_chnl_data > UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE)
            {
                db_chnl_data = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
            }*/
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else {        
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } else {
                    dist_entries[i] = null_value;  
                }  
            }                                 
        }
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_CPB_DCS) 
    {                
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {
            db_chnl_data = mapotn_q_result->p_recs[i]->channel_num;
            

            /* the index of util_schd chnl_ctxt is rd_queue_id.  The DCS index is scheduler
               addr.  For a rd_queue_id identify the rd_channel_id and look in the */

            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */  
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else 
            {
                /* identify the rd_queue_id to index p_chnl_ctxt and
                   provide the content data for dist_entries */
                util_schd_dcs_rd_queue_id_get(h, db_chnl_data, 
                                              UTIL_SCHD_CHNL_OPERATIONAL, 
                                              &rd_queue_id);

                if( rd_queue_id < h->cfg.block_ctxt.num_schd_chnl) {
                    dist_entries[i] = rd_queue_id;
                }
                else
                {
                    dist_entries[i] = null_value;
                }
            }
        }
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_ODUKSC 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMA 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMO
        || inst == UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3
        || inst == UTIL_GEN_DB_ACCESSOR_CBRC_MPMO
        || inst == UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA
        || inst == UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA           
        ) 
    {                
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            channel      = mapotn_q_result->p_recs[i]->channel_num;
            db_chnl_data = channel;

            /* Add user bit data for CBRC ingress datapath (PKT_INT SCBS3)
             * The 5th bit of the channel id specifies where to source data from:
             *  0 - Selects FC1200 path
             *  1 - Selects MPMA path
             */
            if ((UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3 == inst) && (db_chnl_data != DO_NOT_USE_KEY))
            {
                UINT8 user_bits;
                user_bits = p_chnl_ctxt[channel].scbs_user_bits;

                db_chnl_data |= (user_bits << 4);
            }

            
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */  
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else {
                 
                if( p_chnl_ctxt[channel].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } else {
                    dist_entries[i] = null_value;            
                }
            }
        }
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL)
    {
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  


            db_chnl_data = mapotn_q_result->p_recs[i]->channel_num;
            
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */        
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else 
            {
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } else {
                    dist_entries[i] = null_value;            
                }
            }
        }
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN 
             || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN) 
    {
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            /* need to know who has the last calendar... how to get info about the mux stage here? */
            /* key. who? */

            db_chnl_data = mapotn_q_result->p_recs[i]->channel_num;
            
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */        
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else 
            {
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } else {
                    dist_entries[i] = null_value;            
                }
            }
        }
    }
    else if (TRUE == is_schd_extra_channel &&
             inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN)
    {     
            
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            
            /*
              db_chnl_data = q_result->p_recs[i]->lo_channel;
            */            
            if (line_core_otn_q_result->p_recs[i]->lo_channel == 0xFFFFFFFF)
            {        
                /* not two stage muxing */
                if (line_core_otn_q_result->p_recs[i]->mo_channel == 0xFFFFFFFF)
                {
                    /* if there is no trib_slot assigned for this calendar entry
                    ** this is one stage muxing. Calendar entry does not belong to 
                    ** the MO channel
                    */
                    if (line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }  
                    else 
                    { 
                        /* no stage muxing - index is HO */ 
                        if( line_core_otn_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data = 0xFFFFFFFF;
                        }
                        else 
                        {
                            db_chnl_data = line_core_otn_q_result->p_recs[i]->ho_channel;        
                        }   
                    }
                }
                else 
                {
                    if (line_core_otn_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                      /* one stage muxing - index is MO */
                      db_chnl_data = line_core_otn_q_result->p_recs[i]->mo_channel;
                  }
                  else 
                  {
                      db_chnl_data = 0xFFFFFFFF;
                  }
                }
            }
            else 
            {
                /* two stage muxing - index is LO */
                db_chnl_data = line_core_otn_q_result->p_recs[i]->lo_channel;
            }
            
            
            /*if (db_chnl_data > UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE)
              {
              db_chnl_data = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
              }*/
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else
            {
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } 
                else 
                {
                    dist_entries[i] = null_value;            
                }  
            }                
        }
    }
    else if (TRUE == is_schd_extra_channel &&
             inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER)
    {     
        for(i=0; i<h->cfg.block_ctxt.num_schd_chnl; i++)
        {  
            BOOL two_stage_muxing = FALSE;  
            BOOL dummy_mo_first_cycle = FALSE; 
            UINT32 dummy_mo_chnl_id = null_value;
            UINT32 mo_chnl_id = null_value;
            
            
            /*
              db_chnl_data = q_result->p_recs[i]->lo_channel;
            */            
            if (line_core_otn_q_result->p_recs[i]->lo_channel == 0xFFFFFFFF)
            {        
                /* not two stage muxing */
                if (line_core_otn_q_result->p_recs[i]->mo_channel == 0xFFFFFFFF)
                {
                    /* if there is no trib_slot assigned for this calendar entry
                    ** this is one stage muxing. Calendar entry does not belong to 
                    ** the MO channel
                    */
                    if (line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }  
                    else 
                    { 
                        /* no stage muxing - index is HO */ 
                        if( line_core_otn_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data = 0xFFFFFFFF;
                        }
                        else {
                           db_chnl_data = line_core_otn_q_result->p_recs[i]->ho_channel;        
                        }   
                    }
                }
                else 
                {
                    if (line_core_otn_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data = line_core_otn_q_result->p_recs[i]->mo_channel;
                        
                        dummy_mo_chnl_id = line_core_otn_q_result->p_recs[i]->mo_channel;
                        dummy_mo_first_cycle = TRUE;
                    }
                    else 
                    {
                        db_chnl_data = 0xFFFFFFFF;
                    }
                }
            }
            else 
            {
                /* two stage muxing - index is LO */
                db_chnl_data = line_core_otn_q_result->p_recs[i]->lo_channel;
                mo_chnl_id = line_core_otn_q_result->p_recs[i]->mo_channel;
                two_stage_muxing = TRUE;
                
            }
            
            
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */
            if(db_chnl_data == 0xFFFFFFFF)
            {
                dist_entries[i] = null_value;
            }
            else
            {
                if( p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[i] = db_chnl_data;
                } 
                else 
                {
                    dist_entries[i] = null_value;            
                }  
            } 
            
 
            if (two_stage_muxing)
            {               
                line_core_otn_db_key       mo_key;
                line_core_otn_db_query_res *mo_q_result = NULL;

                /* find the first calendar cycle of the MO */
                mo_key.calendar_entry = DO_NOT_USE_KEY;
                mo_key.ho_channel = DO_NOT_USE_KEY;
                mo_key.mo_channel = mo_chnl_id;
                mo_key.lo_channel = DO_NOT_USE_KEY;
                
                num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&mo_key,(void **)&mo_q_result);
                
                PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
                
                if (mo_q_result->p_recs[0]->calendar_entry != 0xFFFFFFFF)
                {
                    /* always keep the first calendar cycle of the MO scheduled in MPMO */
                    
                    /*if (p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_EQUIPPED && dist_entries[i] == null_value)
                      {
                      dist_entries[mo_q_result->p_recs[0]->calendar_entry] = mo_chnl_id;
                      } */
                    
                    if (dist_entries[mo_q_result->p_recs[0]->calendar_entry] == null_value)
                    {
                        dist_entries[mo_q_result->p_recs[0]->calendar_entry] = mo_chnl_id;
                        /* keep this calendar as if it is provisioned for a real channel */
                        p_chnl_ctxt[mo_q_result->p_recs[0]->calendar_entry].state = UTIL_SCHD_CHNL_OPERATIONAL;
                        p_chnl_ctxt[mo_q_result->p_recs[0]->calendar_entry].fo1_mpmo_dummy = TRUE;
                    }
                    /*if (p_chnl_ctxt[db_chnl_data].state == UTIL_SCHD_CHNL_EQUIPPED)
                    {*/
                        /* the first calendar cycle of LO is not assigned to any LO channel */
                    if (mo_q_result->p_recs[0]->lo_channel == 0xFFFFFFFF)
                    {
                            dist_entries[mo_q_result->p_recs[0]->calendar_entry] = line_core_otn_q_result->p_recs[0]->calendar_entry;
                            p_chnl_ctxt[mo_q_result->p_recs[0]->calendar_entry].state = UTIL_SCHD_CHNL_OPERATIONAL;
                    }                            
                    /*}*/
                }
                
            } /* only if two_stage muxing */    
            
                /* if it is a dummy calendar, keep it in scheduler */
            if (p_chnl_ctxt[i].fo1_mpmo_dummy)
            {
                dist_entries[i] = i;
                /* keep this calendar as if it is provisioned for a real channel */
                p_chnl_ctxt[i].state = UTIL_SCHD_CHNL_OPERATIONAL;
            }
            
        }
    }
    else 
    {
        PMC_ASSERT(FALSE, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    }


    
    if(inst == UTIL_GEN_DB_ACCESSOR_CPB_DCS &&
       h->var.cal_pinch_type == UTIL_GLOBAL_CAL_PINCH_ODU4_ODUKSW_TX_DPI)
    {
        UINT32 entries_to_remove[UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DPI_NUM] = UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DPI;

        UINT32 chnl_id = null_value;
        /*write all entries in the calendar with the channel ID*/
        for(i=0;i<h->cfg.block_ctxt.num_schd_chnl;i++)
        {
            if(dist_entries[i] != null_value)
            {
                chnl_id = dist_entries[i];
            }
        }
        
        /*write all entries in the calendar with the channel ID*/
        for(i=0;i<h->cfg.block_ctxt.num_schd_chnl;i++)
        {
            dist_entries[i] = chnl_id;
        }

        /*remove the calendar entries for optimal pinching */
        for(i=0;i<UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DPI_NUM;i++)
        {
            dist_entries[entries_to_remove[i]] = null_value;
        }


    }
    else if((inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER ||
             inst == UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL ) &&
            h->var.cal_pinch_type == UTIL_GLOBAL_CAL_PINCH_ODU4_ODUKSW_TX_DCI)
    {
        UINT32 entries_to_remove[UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DCI_NUM] = UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DCI;

        UINT32 chnl_id = null_value;
        /*write all entries in the calendar with the channel ID*/
        for(i=0;i<h->cfg.block_ctxt.num_schd_chnl;i++)
        {
            if(dist_entries[i] != null_value)
            {
                chnl_id = dist_entries[i];
            }
        }


        /*write all entries in the calendar with the channel ID*/
        for(i=0;i<h->cfg.block_ctxt.num_schd_chnl;i++)
        {
            dist_entries[i] = chnl_id;
        }

        /*remove the calendar entries for optimal pinching */
        for(i=0;i<UTIL_SCHD_CAL_PINCH_ODU4_ODUKSW_TX_DCI_NUM;i++)
        {
            dist_entries[entries_to_remove[i]] = null_value;
        }

    }

    
  
    PMC_RETURN(); 

}/* util_schd_group_dist_gen_db */


/*******************************************************************************
* util_schd_chnl_dist_gen_db
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Accesses database based on database ID and checks through the database if 
*   channel state is OPERATIONAL it will transfers the scheduler contents 
*   from database to outputlist otherwise writes 0xFF to output list.  
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   db_id                           - database ID to access
*   inst                            - data base evaluation type processing
*   chnl_id                         - channel ID to be written in db
*
* OUTPUTS:
*   *dist_entries[]                 - pointer to storage for the generated
*                                     scheduler contents.
*
* RETURNS:
*   PMC_SUCCESS
*   UTIL_SCHD_ERR_INSUFFICIENT_CALENDAR_ENTRIES
*
* NOTES:
*
*******************************************************************************/
PRIVATE void util_schd_chnl_dist_gen_db(util_schd_handle_t *h,
                                        util_gen_db_id_t db_id,  
                                        util_gen_db_accessor_inst_t inst,
                                        UINT32 chnl_id,
                                        UINT32 *dist_entries)
{
    /* Variable declaration */
    UINT32                 i;
    UINT32                   num_of_recs;
    UINT32                 *db_chnl_data_ptr;
    UINT32                 null_value = h->cfg.block_ctxt.entry_null_value;
    UINT32                 rd_queue_id;
    util_schd_chnl_ctxt_t *p_chnl_ctxt = NULL;    
    UINT32                  cal_entry;

    /*Variables for mapotn db */
    mapotn_db_key          mapotn_key;
    mapotn_db_query_res    *mapotn_q_result = NULL;

    /*Variables for line_core_otn db */    
    line_core_otn_db_key       line_core_otn_key;
    line_core_otn_db_query_res *line_core_otn_q_result = NULL;
    BOOL8 is_schd_extra_channel = pmc_coreotn_schd_extra_channel_revision(&h->base);

    PMC_ENTRY();
    
    p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;
    PMC_ASSERT(p_chnl_ctxt != NULL, UTIL_SCHD_ERR_NULL_PTR, 0, 0);
    
    PMC_LOG(PMC_LOG_SEV_LOWEST, UTIL_SCHD_LOG_CODE_LOG, 0, 0, "db_id = %u\n", db_id);  
    PMC_LOG(PMC_LOG_SEV_LOWEST, UTIL_SCHD_LOG_CODE_LOG, 0, 0, "inst = %u\n", inst); 
    
    PMC_ASSERT(inst < LAST_UTIL_GEN_DB_ACCESSOR, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(chnl_id < h->cfg.block_ctxt.num_schd_chnl, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    
    PMC_LOG_TRACE("h->cfg.block_ctxt.num_schd_chnl = %u\n", h->cfg.block_ctxt.num_schd_chnl);
    
    db_chnl_data_ptr = (UINT32*)PMC_CALLOC(sizeof(UINT32) * h->cfg.block_ctxt.num_schd_chnl);   
    
    /* initialize dist_entries array */
    for (i = 0; i < h->cfg.block_ctxt.num_schd_chnl; i++)
    {
        dist_entries[i] = null_value;
        db_chnl_data_ptr[i] = null_value;
    }
    
    if (inst == UTIL_GEN_DB_ACCESSOR_CPB_DCS) 
    {                
        PMC_LOG_TRACE("instance = %u\n", inst);
        /* look at entry index 'i' of data base */
        mapotn_key.calendar_entry = DO_NOT_USE_KEY;
        mapotn_key.channel_num = chnl_id;
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key,(void **)&mapotn_q_result);
        
        PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);

        PMC_LOG_TRACE("db_id %d chnl_id %d, q_result->p_recs[0]->channel_num %d\n", db_id, chnl_id, mapotn_q_result->p_recs[0]->channel_num);
        
        /* the index of util_schd chnl_ctxt is rd_queue_id.  The DCS index is scheduler
           addr.  For a rd_queue_id identify the rd_channel_id and look in the */

        /* if channel state of 'i' is OPERATIONAL write to output list
           else write 0xFF to output list */  
        for (i = 0; i < num_of_recs; i++)
        {
            cal_entry = mapotn_q_result->p_recs[i]->calendar_entry;
            db_chnl_data_ptr[cal_entry] = chnl_id;
             
            /* identify the rd_queue_id to index p_chnl_ctxt and
               provide the content data for dist_entries */
            util_schd_dcs_rd_queue_id_get(h, db_chnl_data_ptr[cal_entry], 
                                          UTIL_SCHD_CHNL_OPERATIONAL, 
                                          &rd_queue_id);
            
            if( rd_queue_id < h->cfg.block_ctxt.num_schd_chnl) {
                dist_entries[cal_entry] = rd_queue_id;
            }
            else
            {
                dist_entries[cal_entry] = null_value;
            }
        }
        
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_ODUKSC 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMA 
        || inst == UTIL_GEN_DB_ACCESSOR_MPMO
        || inst == UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3
        || inst == UTIL_GEN_DB_ACCESSOR_CBRC_MPMO
        || inst == UTIL_GEN_DB_ACCESSOR_ENET_LINE_MPMA
        || inst == UTIL_GEN_DB_ACCESSOR_ENET_SYS_MPMA) 
    {                
        PMC_LOG_TRACE("instance = %u\n", inst);
        /* search all calendar entries for this channel id */
        mapotn_key.calendar_entry = DO_NOT_USE_KEY;
        mapotn_key.channel_num = chnl_id;
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key,(void **)&mapotn_q_result);

        PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);

        PMC_LOG_TRACE("db_id %d i %d, q_result->p_recs[0]->channel_num %d\n", db_id, i, mapotn_q_result->p_recs[0]->channel_num);
        
        for (i = 0; i < num_of_recs; i++)
        {               
            cal_entry = mapotn_q_result->p_recs[i]->calendar_entry;
            db_chnl_data_ptr[cal_entry] = chnl_id;
                
            /* Add user bit data for CBRC ingress datapath (PKT_INT SCBS3)
             * The 5th bit of the channel id specifies where to source data from:
             *  0 - Selects FC1200 path
             *  1 - Selects MPMA path
             */
            if ((UTIL_GEN_DB_ACCESSOR_CBRC_SCBS3 == inst) && (db_chnl_data_ptr[cal_entry] != DO_NOT_USE_KEY))
            {
                UINT8 user_bits;
                
                user_bits = p_chnl_ctxt[chnl_id].scbs_user_bits;
                
                db_chnl_data_ptr[cal_entry] |= (user_bits << 4);
            }
        
                
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */  
        
            if(db_chnl_data_ptr[cal_entry] == 0xFFFFFFFF)
            {
                dist_entries[cal_entry] = null_value;
            }
            else {
                 
                if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                } else {
                    dist_entries[cal_entry] = null_value;            
                }
            }
        }
        
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1) 
    {
        /* entries are per HO channel basis */
        PMC_LOG_TRACE("i = %u, instance = %u\n", i, inst);
        
        line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
        line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
        line_core_otn_key.mo_channel = chnl_id;
        line_core_otn_key.lo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.trib_slot  = DO_NOT_USE_KEY;
        
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);

        PMC_LOG_TRACE("inside dist_gen: num_of_recs = %u\n", num_of_recs);
        
        if (num_of_recs == 0)
        {
            /* no muxing */
            line_core_otn_db_key no_stage_key;
            line_core_otn_db_query_res *no_stage_line_core_otn_q_result = NULL;
                    
            no_stage_key.calendar_entry = DO_NOT_USE_KEY;
            no_stage_key.ho_channel = chnl_id;
            no_stage_key.mo_channel = DO_NOT_USE_KEY;
            no_stage_key.lo_channel = DO_NOT_USE_KEY;
            no_stage_key.trib_slot = DO_NOT_USE_KEY;
            
            PMC_LOG_TRACE("UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL/UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC1\n");
            PMC_LOG_TRACE("key.calendar_entry = %u, key.ho_channel = %u\n", no_stage_key.calendar_entry, no_stage_key.ho_channel);
               
            num_of_recs = gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key,(void **)&no_stage_line_core_otn_q_result); 
            
            /* one stage muxing but filter out for LO channel 
            ** such that not all calendar entires for the HO
            ** is programmed
            */
            for (i = 0; i < num_of_recs; i++)
            {
                cal_entry = no_stage_line_core_otn_q_result->p_recs[i]->calendar_entry;
                
                if (no_stage_line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && no_stage_line_core_otn_q_result->p_recs[i]->ts_port_ho_mo != 0)
                {
                    db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                }  
                else if (no_stage_line_core_otn_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && no_stage_line_core_otn_q_result->p_recs[i]->cal_entry_status != 0)
                {
                    /* this is MO ODU3e1/3e2 case. Keep the 33rd in FMF1 */
                    db_chnl_data_ptr[cal_entry] = no_stage_line_core_otn_q_result->p_recs[i]->cal_entry_status;   
                    
                    if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL) 
                    {
                        dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry]; 
                    }
                    else {
                        dist_entries[cal_entry] = null_value;
                    }
                }
                else { 
                    if( no_stage_line_core_otn_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }
                    else { 
                        /* no stage muxing - index is HO */
                        db_chnl_data_ptr[cal_entry] = no_stage_line_core_otn_q_result->p_recs[i]->ho_channel;
                        
                        if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL) 
                        {
                            dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry]; 
                        }
                        else {
                            dist_entries[cal_entry] = null_value;
                        }                        
                    }                        
                }
            }
            
            
        }
        else {
            /* this is one stage muxing */
            for (i = 0; i < num_of_recs; i++)
            {
                cal_entry = line_core_otn_q_result->p_recs[i]->calendar_entry;
                
                /* one stage muxing - index is MO */
                if (line_core_otn_q_result->p_recs[i]->trib_slot > 79)
                {
                    db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;                   
                }
                else if (line_core_otn_q_result->p_recs[i]->ho_channel == 0xFFFFFFFF)
                {
                    /* this is MO ODU3 case. Keep it to 31 in FMF1 */
                    db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                }
                else { 
                    db_chnl_data_ptr[cal_entry] = line_core_otn_q_result->p_recs[i]->mo_channel;
                    
                    if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL) 
                    {
                        dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry]; 
                    }
                    else {
                        dist_entries[cal_entry] = null_value;
                    }
                }
            }
            
        }            
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_MPMA_MPMO_COREOTN_CTL)
    {

        PMC_LOG_TRACE("instance = %u\n", inst);
        /* look at entry index 'i' of data base */
        mapotn_key.calendar_entry = DO_NOT_USE_KEY;
        mapotn_key.channel_num = chnl_id;
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key,(void **)&mapotn_q_result);

        PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);

        PMC_LOG_TRACE("db_id %d i %d, q_result->p_recs[0]->channel_num %d\n", db_id, i, mapotn_q_result->p_recs[0]->channel_num);
                
        for (i = 0; i < num_of_recs; i++)
        {               
            cal_entry = mapotn_q_result->p_recs[i]->calendar_entry;
            db_chnl_data_ptr[cal_entry] = chnl_id;
                    
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */  
        
            if(db_chnl_data_ptr[cal_entry] == 0xFFFFFFFF)
            {
                dist_entries[cal_entry] = null_value;
            }
            else {
                 
                if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                } else {
                    dist_entries[cal_entry] = null_value;            
                }
            }
        }
             

    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN_FROM_MAPOTN 
             || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_TO_MAPOTN) 
    {
        PMC_LOG_TRACE("instance = %u\n", inst);
        /* look at entry index 'i' of data base */
        mapotn_key.calendar_entry = DO_NOT_USE_KEY;
        mapotn_key.channel_num = chnl_id;
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&mapotn_key,(void **)&mapotn_q_result);

        PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);

        PMC_LOG_TRACE("db_id %d chnl_id %d, q_result->p_recs[0]->channel_num %d\n", db_id, chnl_id, mapotn_q_result->p_recs[0]->channel_num);
        
        for (i = 0; i < num_of_recs; i++)
        {               
            cal_entry = mapotn_q_result->p_recs[i]->calendar_entry;
            db_chnl_data_ptr[cal_entry] = chnl_id;
                    
            /* if channel state of 'i' is OPERATIONAL write to output list
               else write 0xFF to output list */  
        
            if(db_chnl_data_ptr[cal_entry] == 0xFFFFFFFF)
            {
                dist_entries[cal_entry] = null_value;
            }
            else {
                 
                if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                } else {
                    dist_entries[cal_entry] = null_value;            
                }
            }
        }
        
    }
    else if (TRUE == is_schd_extra_channel &&
             inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN)
    {     
        PMC_LOG_TRACE("instance = %u\n", inst);   
        
        line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
        line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
        line_core_otn_key.mo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.lo_channel = chnl_id;
        
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);
                    
        if (num_of_recs == 0)
        {
            line_core_otn_db_key one_stage_key;
            line_core_otn_db_query_res *one_stage_q_result = NULL;
                                 
            one_stage_key.calendar_entry = DO_NOT_USE_KEY;
            one_stage_key.ho_channel = DO_NOT_USE_KEY;
            one_stage_key.mo_channel = chnl_id;
            one_stage_key.lo_channel = DO_NOT_USE_KEY;
            one_stage_key.trib_slot = DO_NOT_USE_KEY;
            
            PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
            PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
            
            num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&one_stage_key,(void **)&one_stage_q_result);
            
            if (num_of_recs == 0)
            {     
                line_core_otn_db_key no_stage_key;
                line_core_otn_db_query_res *no_stage_q_result = NULL;
                                     
                no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                no_stage_key.ho_channel = chnl_id;
                no_stage_key.mo_channel = DO_NOT_USE_KEY;
                no_stage_key.lo_channel = DO_NOT_USE_KEY;
                no_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("2. No stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                PMC_LOG_TRACE("no_stage_key.ho_channel = %u", no_stage_key.ho_channel);
                
                num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key,(void **)&no_stage_q_result);
                
                PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
                
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = no_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (no_stage_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && no_stage_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }  
                    else 
                    { 
                        /* no stage muxing - index is HO */ 
                        if( no_stage_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                        }
                        else 
                        {
                            db_chnl_data_ptr[cal_entry] = no_stage_q_result->p_recs[i]->ho_channel;   
                            
                            if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                            {
                                dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                                PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                            } 
                            else 
                            {
                                dist_entries[cal_entry] = null_value;            
                            }       
                        }   
                    }
                   /* PMC_LOG_TRACE("No stage muxing: db_chnl_data = %u\n", db_chnl_data);*/
                }
                
            }
            else {
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = one_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (one_stage_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && one_stage_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data_ptr[cal_entry] = one_stage_q_result->p_recs[i]->mo_channel;
                        
                        if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                        {
                            dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                            PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                        } 
                        else 
                        {
                            dist_entries[cal_entry] = null_value;            
                        }  
                    }
                    else 
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }
                    PMC_LOG_TRACE("One stage muxing: cal_entyr = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                }
                
            }            
        }
        else {
            /* two stage muxing */
            /* two stage muxing - index is LO */
            for (i = 0; i < num_of_recs; i++)
            {
                cal_entry = line_core_otn_q_result->p_recs[i]->calendar_entry;
                
                db_chnl_data_ptr[cal_entry] = line_core_otn_q_result->p_recs[i]->lo_channel;
                PMC_LOG_TRACE("Two stage muxing: cal_entry = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                
                if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                    PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                } 
                else 
                {
                    dist_entries[cal_entry] = null_value;            
                }  
            } 
             
        }                                   
    }
    else if (TRUE == is_schd_extra_channel &&
             inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER)
    {     
        PMC_LOG_TRACE("instance = %u\n", inst);  
        BOOL two_stage_muxing = FALSE;  
        BOOL dummy_mo_first_cycle = FALSE; 
        UINT32 dummy_mo_chnl_id = null_value;
        UINT32 mo_chnl_id = null_value;
        
        line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
        line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
        line_core_otn_key.mo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.lo_channel = chnl_id;
        
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);
        
        /*PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);*/
                    
        if (num_of_recs == 0)
        {
            line_core_otn_db_key one_stage_key;
            line_core_otn_db_query_res *one_stage_q_result = NULL;
                                 
            one_stage_key.calendar_entry = DO_NOT_USE_KEY;
            one_stage_key.ho_channel = DO_NOT_USE_KEY;
            one_stage_key.mo_channel = chnl_id;
            one_stage_key.lo_channel = DO_NOT_USE_KEY;
            one_stage_key.trib_slot = DO_NOT_USE_KEY;
            
            PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
            PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
            
            num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&one_stage_key,(void **)&one_stage_q_result);
            
            if (num_of_recs == 0)
            {     
                line_core_otn_db_key no_stage_key;
                line_core_otn_db_query_res *no_stage_q_result = NULL;
                                     
                no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                no_stage_key.ho_channel = chnl_id;
                no_stage_key.mo_channel = DO_NOT_USE_KEY;
                no_stage_key.lo_channel = DO_NOT_USE_KEY;
                no_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("2. No stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                PMC_LOG_TRACE("no_stage_key.ho_channel = %u", no_stage_key.ho_channel);
                
                num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key,(void **)&no_stage_q_result);
                
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = no_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (no_stage_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && no_stage_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }  
                    else 
                    { 
                        /* no stage muxing - index is HO */ 
                        if( no_stage_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                        }
                        else 
                        {
                            db_chnl_data_ptr[cal_entry] = no_stage_q_result->p_recs[i]->ho_channel;   
                            
                            if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                            {
                                dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                                PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                            } 
                            else 
                            {
                                dist_entries[cal_entry] = null_value;            
                            }       
                        }   
                    }
                   /* PMC_LOG_TRACE("No stage muxing: db_chnl_data = %u\n", db_chnl_data);*/
                }
                
            }
            else {
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = one_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (one_stage_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && one_stage_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data_ptr[cal_entry] = one_stage_q_result->p_recs[i]->mo_channel;
                        dummy_mo_chnl_id = one_stage_q_result->p_recs[i]->mo_channel;
                        dummy_mo_first_cycle = TRUE;
                        
                        if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                        {
                            dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                            PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                        } 
                        else 
                        {
                            dist_entries[cal_entry] = null_value;            
                        }  
                    }
                    else 
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }
                    PMC_LOG_TRACE("One stage muxing: cal_entry = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                }
                
            }            
        }
        else {
            /* two stage muxing */
            /* two stage muxing - index is LO */
            for (i = 0; i < num_of_recs; i++)
            {
                cal_entry = line_core_otn_q_result->p_recs[i]->calendar_entry;
                
                db_chnl_data_ptr[cal_entry] = line_core_otn_q_result->p_recs[i]->lo_channel;
                
                mo_chnl_id = line_core_otn_q_result->p_recs[i]->mo_channel;
                two_stage_muxing = TRUE;
                
                PMC_LOG_TRACE("Two stage muxing: cal_entry = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                
                if( p_chnl_ctxt[cal_entry].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[chnl_id] = db_chnl_data_ptr[cal_entry];
                    PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                } 
                else 
                {
                    dist_entries[cal_entry] = null_value;            
                }  
            }
            
        }                 
                
         
            
 
        if (two_stage_muxing)
        {               
            /* find the first calendar cycle of the MO */
            line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
            line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
            line_core_otn_key.mo_channel = mo_chnl_id;
            line_core_otn_key.lo_channel = DO_NOT_USE_KEY;
            
            num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);
            
            PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
            
            if (line_core_otn_q_result->p_recs[0]->calendar_entry != 0xFFFFFFFF)
            {
                /* always keep the first calendar cycle of the MO scheduled in MPMO */
                
                if (dist_entries[line_core_otn_q_result->p_recs[0]->calendar_entry] == null_value)
                {
                    dist_entries[line_core_otn_q_result->p_recs[0]->calendar_entry] = mo_chnl_id;
                    /* keep this calendar as if it is provisioned for a real channel */
                    p_chnl_ctxt[chnl_id].state = UTIL_SCHD_CHNL_OPERATIONAL;
                    p_chnl_ctxt[chnl_id].fo1_mpmo_dummy = TRUE;
                }
                /* the first calendar cycle of LO is not assigned to any LO channel */
                if (line_core_otn_q_result->p_recs[0]->lo_channel == 0xFFFFFFFF)
                {
                    dist_entries[line_core_otn_q_result->p_recs[0]->calendar_entry] = line_core_otn_q_result->p_recs[0]->calendar_entry;
                    p_chnl_ctxt[chnl_id].state = UTIL_SCHD_CHNL_OPERATIONAL;
                }                            
            }
            
             
        } /* only if two_stage muxing */    
    }
    else if (FALSE == is_schd_extra_channel &&
             (inst == UTIL_GEN_DB_ACCESSOR_MPMA_COREOTN 
              || inst == UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER))
    {     
        PMC_LOG_TRACE("instance = %u\n", inst);   
        
        line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
        line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
        line_core_otn_key.mo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.lo_channel = chnl_id;
        
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);
        
        /*PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);*/
        
        if (num_of_recs == 0)
        {
            line_core_otn_db_key one_stage_key;
            line_core_otn_db_query_res *one_stage_q_result = NULL;
                                 
            one_stage_key.calendar_entry = DO_NOT_USE_KEY;
            one_stage_key.ho_channel = DO_NOT_USE_KEY;
            one_stage_key.mo_channel = chnl_id;
            one_stage_key.lo_channel = DO_NOT_USE_KEY;
            one_stage_key.trib_slot = DO_NOT_USE_KEY;
            
            PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
            PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
            
            num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&one_stage_key,(void **)&one_stage_q_result);
            
            if (num_of_recs == 0)
            {     
                line_core_otn_db_key no_stage_key;
                line_core_otn_db_query_res *no_stage_q_result = NULL;
                                     
                no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                no_stage_key.ho_channel = chnl_id;
                no_stage_key.mo_channel = DO_NOT_USE_KEY;
                no_stage_key.lo_channel = DO_NOT_USE_KEY;
                no_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("2. No stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                PMC_LOG_TRACE("no_stage_key.ho_channel = %u", no_stage_key.ho_channel);
                
                num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key,(void **)&no_stage_q_result);
                
                PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
                
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = no_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (no_stage_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && no_stage_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }  
                    else 
                    { 
                        /* no stage muxing - index is HO */ 
                        if( no_stage_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                        }
                        else 
                        {
                            db_chnl_data_ptr[cal_entry] = no_stage_q_result->p_recs[i]->ho_channel;   
                            
                            if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                            {
                                dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                                PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                            } 
                            else 
                            {
                                dist_entries[cal_entry] = null_value;            
                            }       
                        }   
                    }
                    /*PMC_LOG_TRACE("No stage muxing: db_chnl_data = %u\n", db_chnl_data);*/
                }
                
            }
            else {
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = one_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (one_stage_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && one_stage_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data_ptr[cal_entry] = one_stage_q_result->p_recs[i]->mo_channel;
                        
                        if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                        {
                            dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                            PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                        } 
                        else 
                        {
                            dist_entries[cal_entry] = null_value;            
                        }  
                    }
                    else 
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }
                    PMC_LOG_TRACE("One stage muxing: cal_entyr = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                }
                
            }        
        }
        else {
            /* two stage muxing */
            /* two stage muxing - index is LO */
            for (i = 0; i < num_of_recs; i++)
            {
                cal_entry = line_core_otn_q_result->p_recs[i]->calendar_entry;
                
                db_chnl_data_ptr[cal_entry] = line_core_otn_q_result->p_recs[i]->lo_channel;
                PMC_LOG_TRACE("Two stage muxing: cal_entry = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                
                if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                    PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                } 
                else 
                {
                    dist_entries[cal_entry] = null_value;            
                }  
            }
            
        }           
    }
    else if (inst == UTIL_GEN_DB_ACCESSOR_COREOTN_ODUKSC2)
    {
        PMC_LOG_TRACE("instance = %u\n", inst);   
                                            
        line_core_otn_key.calendar_entry = DO_NOT_USE_KEY;
        line_core_otn_key.ho_channel = DO_NOT_USE_KEY;
        line_core_otn_key.mo_channel = DO_NOT_USE_KEY;
        line_core_otn_key.lo_channel = chnl_id;
        
        num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&line_core_otn_key,(void **)&line_core_otn_q_result);
         
        if (num_of_recs == 0)
        {
            line_core_otn_db_key one_stage_key;
            line_core_otn_db_query_res *one_stage_q_result = NULL;
                                 
            one_stage_key.calendar_entry = DO_NOT_USE_KEY;
            one_stage_key.ho_channel = DO_NOT_USE_KEY;
            one_stage_key.mo_channel = chnl_id;
            one_stage_key.lo_channel = DO_NOT_USE_KEY;
            one_stage_key.trib_slot = DO_NOT_USE_KEY;
            
            PMC_LOG_TRACE("2. One stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
            PMC_LOG_TRACE("one_stage_key.mo_channel = %u\n", one_stage_key.mo_channel);
            
            num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&one_stage_key,(void **)&one_stage_q_result);
            
            if (num_of_recs == 0)
            {     
                line_core_otn_db_key no_stage_key;
                line_core_otn_db_query_res *no_stage_q_result = NULL;
                                     
                no_stage_key.calendar_entry = DO_NOT_USE_KEY;
                no_stage_key.ho_channel = chnl_id;
                no_stage_key.mo_channel = DO_NOT_USE_KEY;
                no_stage_key.lo_channel = DO_NOT_USE_KEY;
                no_stage_key.trib_slot = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("2. No stage muxing: UTIL_GEN_DB_ACCESSOR_MPMO_COREOTN_MUX_FRAMER/UTIL_GEN_DB_ACCESSOR_MPMA\n");
                PMC_LOG_TRACE("no_stage_key.ho_channel = %u", no_stage_key.ho_channel);
                
                num_of_recs=gen_db_query(&(h->base), db_id, h->cfg.db_handle, (void *)&no_stage_key,(void **)&no_stage_q_result);
                
                PMC_ASSERT(num_of_recs != 0, UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, 0, 0);
                
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = no_stage_q_result->p_recs[i]->calendar_entry;
                    
                    if (no_stage_q_result->p_recs[i]->trib_slot == 0xFFFFFFFF && no_stage_q_result->p_recs[i]->ts_port_ho_mo != 0)
                    {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }  
                    else { 
                        /* no stage muxing - index is HO */ 
                        if(no_stage_q_result->p_recs[i]->cal_entry_status == 0xFFFFFFFF)
                        {
                            db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                        }
                        else {
                            db_chnl_data_ptr[cal_entry] = no_stage_q_result->p_recs[i]->ho_channel;  
                            
                            if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                            {
                                dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                                PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                            } else {
                                dist_entries[cal_entry] = null_value;  
                                PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);          
                            }      
                        }
                    }
                }
                
            }
            else {
                /* one stage muxing */
                for (i = 0; i < num_of_recs; i++)
                {
                    cal_entry = one_stage_q_result->p_recs[i]->calendar_entry;
                     
                    if (one_stage_q_result->p_recs[i]->ts_port_mo_lo == 0xFFFFFFFF && one_stage_q_result->p_recs[i]->ts_port_ho_mo != 2)
                    {
                        /* one stage muxing - index is MO */
                        db_chnl_data_ptr[cal_entry] = one_stage_q_result->p_recs[i]->mo_channel;
                        
                        if( p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                        {
                            dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                            PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                        } else {
                            dist_entries[cal_entry] = null_value;  
                            PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);          
                        } 
                    }
                    else {
                        db_chnl_data_ptr[cal_entry] = 0xFFFFFFFF;
                    }
                    PMC_LOG_TRACE("One stage muxing: cal_entry = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);  
                }
                
            }            
        }
        else {
            /* two stage muxing */
            for (i = 0; i < num_of_recs; i++)
            {
                cal_entry = line_core_otn_q_result->p_recs[i]->calendar_entry;               
                /* two stage muxing - index is LO */
                db_chnl_data_ptr[cal_entry] = line_core_otn_q_result->p_recs[i]->lo_channel;
                PMC_LOG_TRACE("Two stage muxing: cal_entry = %u, db_chnl_data = %u\n", cal_entry, db_chnl_data_ptr[cal_entry]);
                
                if(p_chnl_ctxt[chnl_id].state == UTIL_SCHD_CHNL_OPERATIONAL)
                {
                    dist_entries[cal_entry] = db_chnl_data_ptr[cal_entry];
                    PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);
                } else {
                    dist_entries[cal_entry] = null_value;  
                    PMC_LOG_TRACE("dist_entries[%u] = %u\n", cal_entry, dist_entries[cal_entry]);          
                }                
            } 
            
        }                                                         
    }
    else 
    {
        PMC_ASSERT(FALSE, UTIL_SCHD_ERR_INVALID_ARG, 0, 0);
    }
    
    
    PMC_FREE(&(db_chnl_data_ptr));
    
    PMC_LOG_TRACE("dist_entries generated\n");
  
    /*gen_db_detach(&(h->base),db_id,db_handle);*/
    PMC_RETURN(); 

}/* util_schd_chnl_dist_gen_db */









/*******************************************************************************
* util_schd_dcs_rd_queue_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
* function to retrieve rd_queue_id for either of specified states 
*  with rd_channel_id matching input value
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   rd_channel_id                   - rd_channel_id to match
*   state1                          - state to match
*
* OUTPUTS:
*   *rd_queue_id_ptr                - pointer to the rd_queue_id.
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void util_schd_dcs_rd_queue_id_get(util_schd_handle_t *h, 
                                           UINT32 rd_channel_id,
                                           util_schd_chnl_state_t state1, 
                                           UINT32 *rd_queue_id_ptr)
{
    util_schd_ctxt_t *schd_ctxt = h->var.schd_ctxt;
    util_schd_chnl_ctxt_t *chnl_ctxt;
    UINT32 rd_queue_id;

    chnl_ctxt = schd_ctxt->chnl_ctxt;

    rd_queue_id = schd_ctxt->rd_queue_ids[rd_channel_id];
    if (chnl_ctxt[ rd_queue_id ].state != state1 ||
        chnl_ctxt[ rd_queue_id ].dcs_rd_channel_id != rd_channel_id)
    {
        rd_queue_id = 0xFFFFFFFF;
    } 
    *rd_queue_id_ptr = rd_queue_id;
} /* util_schd_dcs_rd_queue_id_get */

/*******************************************************************************
* util_schd_var_chnl_default_set
* ______________________________________________________________________________
*
* DESCRIPTION:
* sets var data channel context to default values
*
* INPUTS:
*   *p_chnl_ctxt                    - pointer to channel context
*   chnl_id                         - chnl_id to set in context
*
* OUTPUTS:
*   None
*
* RETURNS:
*   Nothing.
*
* NOTES:
*   Modifies the input p_chnl_ctxt.
*
*******************************************************************************/
PUBLIC void util_schd_var_chnl_default_set(util_schd_chnl_ctxt_t *p_chnl_ctxt, UINT32 chnl_id)
{
    PMC_ENTRY();

    p_chnl_ctxt->chnl_id         = chnl_id;
    p_chnl_ctxt->grp_id          = (UINT8)UTIL_SCHD_INVALID_GROUP_ID;
    p_chnl_ctxt->rate            = 0;
    p_chnl_ctxt->rate_units      = UTIL_SCHD_1_BIT_SEC;
    p_chnl_ctxt->state           = UTIL_SCHD_CHNL_START;
    p_chnl_ctxt->scbs_user_bits  = 0;
    p_chnl_ctxt->min_entries     = 0;
    p_chnl_ctxt->max_entries     = 0;
    p_chnl_ctxt->assign_entries  = 0;
    p_chnl_ctxt->chnl_type       = 0;
    p_chnl_ctxt->inUseFlag       = 0;
    p_chnl_ctxt->allocated_ho_channel = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;

    PMC_RETURN();
}


/*******************************************************************************
* util_schd_var_grp_default_set
* ______________________________________________________________________________
*
* DESCRIPTION:
* sets var data group context to default values
*
* INPUTS:
*   *p_grp_ctxt                     - pointer to group context
*
* OUTPUTS:
*   None
*
* RETURNS:
*   Nothing.
*
* NOTES:
*   Modifies the input p_grp_ctxt.
*
*******************************************************************************/
PUBLIC void util_schd_var_grp_default_set(util_schd_grp_ctxt_t *p_grp_ctxt)
{
    PMC_ENTRY();
    p_grp_ctxt->grp_id      = 0;
    p_grp_ctxt->start_addr  = 0;
    p_grp_ctxt->end_addr    = 0;
    PMC_RETURN();
}

/*******************************************************************************
* util_schd_chnl_ctxt_copy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Copies source to dest.
*
* INPUTS:
*   *dest                           - Context to copy to
*   *source                         - Context to from
*
* OUTPUTS:
*   None
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void util_schd_chnl_ctxt_copy(util_schd_chnl_ctxt_t *dest,
                                     util_schd_chnl_ctxt_t *source)
{
    PMC_ENTRY();

    *dest = *source;

    PMC_RETURN();
}

/*******************************************************************************
* util_schd_math_division
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function to perform division
*
* INPUTS:
*   division_type                   - The type of division to do.
*   dividend_mantisa                - Dividend Mantissa
*   dividend_exp                    - Dividend Exponent
*   divisor_mantisa                 - Divisor Mantissa
*   divisor_exp                     - Divisor Exponent
*
* OUTPUTS:
*   *quotient_mantisa               - Quotient Mantissa
*   *quotient_exp                   - Quotient Exponent
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_math_division(util_schd_math_division_t division_type,
                                         UINT32 dividend_mantisa, INT32 dividend_exp,
                                         UINT32 divisor_mantisa, INT32 divisor_exp,
                                         UINT32 *quotient_mantisa, INT32 *quotient_exp)
{
    /* an alternate method can be generated to use floating point numbers. */

    UINT32 divisor_mantisa_reduce, divisor_mantisa_temp;
    INT32 divisor_exp_reduce;
    UINT32 quotient_mantisa_modulous;
    UINT32 quotient_mantisa_reduce;
    INT32 i;
    UINT32 adj_limit = 0;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("\ndivision_type=%u\ndividend_mantisa=%u\ndividend_exp=%u\ndivisor_mantisa=%u\ndivisor_exp=%u\n", division_type,dividend_mantisa,dividend_exp,divisor_mantisa,divisor_exp);

    /* division types: floor, ceiling, round */
    if(divisor_mantisa == 0) PMC_RETURN(UTIL_SCHD_ERR_DIVIDE_BY_ZERO);

    /* increase dividend value to maximize size storable in 32 bit value */
    while (dividend_mantisa <= ((UINT32)0xFFFFFFFF) / 10) /* floor rounding error */
    {
        /* increase dividend_mantisa by factor of 10 */
        dividend_mantisa = dividend_mantisa * 10;
        /* decrease dividend_exp by 1 */
        dividend_exp--;

        /*
         * Limit number of loops to 10. This already gives us a 10^10
         * increase in the dividend_mantisa.
         */
        adj_limit++;
        if (adj_limit >= 10) {
            break;
        }
    }

    /* reduce the divisor so that it is 1000 times less than the
       max possible value */
    divisor_mantisa_reduce = 1;
    divisor_exp_reduce = 0;
    divisor_mantisa_temp = divisor_mantisa;
    while (divisor_mantisa_temp > ((UINT32)0xFFFFFFFF) / (1000))
    {
        divisor_mantisa_reduce *= 10;
        divisor_mantisa_temp /= 10;
        divisor_exp_reduce++;
    }

    if (divisor_mantisa_reduce != 1)
    {
        divisor_mantisa = divisor_mantisa/divisor_mantisa_reduce + ((divisor_mantisa%divisor_mantisa_reduce >= divisor_mantisa_reduce/2)? 1:0);
    }

    divisor_exp += divisor_exp_reduce;


    switch(division_type)
    {
        case UTIL_SCHD_MATH_DIVISION_RAW:
            *quotient_mantisa = dividend_mantisa / divisor_mantisa;
            *quotient_exp = dividend_exp - divisor_exp;
            break;
        case UTIL_SCHD_MATH_DIVISION_ROUND:
        case UTIL_SCHD_MATH_DIVISION_CEILING:
        case UTIL_SCHD_MATH_DIVISION_FLOOR:
            *quotient_exp = dividend_exp - divisor_exp;
            *quotient_mantisa = dividend_mantisa / divisor_mantisa;
            quotient_mantisa_modulous = dividend_mantisa % divisor_mantisa;

            if(*quotient_exp == 0){
                if(UTIL_SCHD_MATH_DIVISION_ROUND == division_type)
                {
                    *quotient_mantisa += (quotient_mantisa_modulous >= divisor_mantisa/2)? 1:0;
                }
                if(UTIL_SCHD_MATH_DIVISION_CEILING == division_type)
                {
                    *quotient_mantisa += (quotient_mantisa_modulous != 0)? 1:0;
                }
                PMC_RETURN(PMC_SUCCESS);
            }

            quotient_mantisa_reduce = 1;
            /* want quotient_exp to represent a whole number */
            if(*quotient_exp < 0)
            {
                for(i=*quotient_exp; i < 0; i++)
                {
                    quotient_mantisa_reduce *= 10;
                }
                if(UTIL_SCHD_MATH_DIVISION_ROUND == division_type)
                {
                    *quotient_mantisa = *quotient_mantisa / quotient_mantisa_reduce +
                                        ((*quotient_mantisa % quotient_mantisa_reduce >=
                                          quotient_mantisa_reduce/2)? 1:0);
                }
                else if(UTIL_SCHD_MATH_DIVISION_CEILING == division_type)
                {
                    *quotient_mantisa = *quotient_mantisa / quotient_mantisa_reduce +
                                       ((*quotient_mantisa % quotient_mantisa_reduce != 0)? 1:0);
                }
                else
                {
                    *quotient_mantisa = *quotient_mantisa / quotient_mantisa_reduce;
                }
                *quotient_exp = 0;
            } else if(*quotient_exp > 0) {
                for(i=0; i < *quotient_exp; i++)
                {
                    quotient_mantisa_reduce *= 10;
                }
                if(*quotient_mantisa * quotient_mantisa_reduce >= 0xFFFFFFFF)
                {
                    PMC_RETURN(UTIL_SCHD_ERR_VARIABLE_OVERFLOW);
                }
                if(UTIL_SCHD_MATH_DIVISION_ROUND == division_type)
                {
                    *quotient_mantisa = *quotient_mantisa * quotient_mantisa_reduce +
                                       ((*quotient_mantisa % quotient_mantisa_reduce >=
                                        quotient_mantisa_reduce/2)? 1:0);
                }
                else if(UTIL_SCHD_MATH_DIVISION_CEILING == division_type)
                {
                    *quotient_mantisa = *quotient_mantisa * quotient_mantisa_reduce +
                                       ((*quotient_mantisa % quotient_mantisa_reduce != 0)? 1:0);
                }
                else
                {
                    *quotient_mantisa = *quotient_mantisa * quotient_mantisa_reduce;
                }
                *quotient_exp = 0;
            }
            break;
        default:
            PMC_RETURN(UTIL_SCHD_ERR_INVALID_ARG);

    }

    PMC_RETURN(PMC_SUCCESS);


} /* util_schd_math_division */

/*******************************************************************************
* util_schd_math_multiply
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function to perform multiplication.  Used to verify util_schd_math_division.
*
* INPUTS:
*   multiplicand_mantisa            - Multiplicand Mantissa
*   multiplicand_exp                - Multiplicand Exponent
*   multiplier_mantisa              - Multiplier Mantissa
*   multiplier_exp                  - Multiplier Exponent
*
* OUTPUTS:
*   *product_mantisa                - Product Mantissa
*   *product_exp                    - Product Exponent
*
* RETURNS:
*   Nothing.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_math_multiply(UINT32 multiplicand_mantisa, INT32 multiplicand_exp,
                                         UINT32 multiplier_mantisa, INT32 multiplier_exp,
                                         UINT32 *product_mantisa, INT32 *product_exp)
{

    UINT32 multiplicand_mantisa_temp;
    UINT32 multiplier_mantisa_temp;
    INT32 multiplicand_exp_temp;
    INT32 multiplier_exp_temp;
    UINT32 multiplier_mantisa_reduce;
    UINT32 multiplicand_mantisa_reduce;

    PMC_ENTRY();

    if (multiplicand_mantisa == 0 || multiplier_mantisa == 0)
    {
        product_mantisa = 0;
        product_exp = 0;
        PMC_RETURN(PMC_SUCCESS);
    }

    multiplicand_mantisa_temp = multiplicand_mantisa;
    multiplier_mantisa_temp = multiplier_mantisa;
    multiplicand_exp_temp = multiplicand_exp;
    multiplier_exp_temp = multiplier_exp;

    multiplier_mantisa_reduce = 1;
    multiplicand_mantisa_reduce = 1;

    while(0xFFFFFFFF/multiplicand_mantisa_temp < multiplier_mantisa_temp)
    {
        if(multiplicand_mantisa_temp % 10 == 0)
        {
            multiplicand_mantisa_reduce *= 10;
            multiplicand_mantisa_temp /= 10;
            multiplicand_exp_temp++;
        }
        else if(multiplier_mantisa_temp % 10 == 0)
        {
            multiplier_mantisa_reduce *= 10;
            multiplier_mantisa_temp /= 10;
            multiplier_exp_temp++;
        }
        else if( multiplicand_mantisa_temp > multiplier_mantisa_temp )
        {
            multiplicand_mantisa_reduce *= 10;
            multiplicand_mantisa_temp /= 10;
            multiplicand_exp_temp++;
        } else {
            multiplier_mantisa_reduce *= 10;
            multiplier_mantisa_temp /= 10;
            multiplier_exp_temp++;
        }
    }

    if(multiplier_mantisa_reduce != 1) {
        multiplier_mantisa = multiplier_mantisa/multiplier_mantisa_reduce + ((multiplier_mantisa%multiplier_mantisa_reduce >= multiplier_mantisa_reduce/2)? 1:0);
    }
    multiplier_exp = multiplier_exp_temp;

    if(multiplicand_mantisa_reduce != 1) {
        multiplicand_mantisa = multiplicand_mantisa/multiplicand_mantisa_reduce + ((multiplicand_mantisa%multiplicand_mantisa_reduce >= multiplicand_mantisa_reduce/2)? 1:0);
    }
    multiplicand_exp = multiplicand_exp_temp;

    *product_mantisa = multiplicand_mantisa * multiplier_mantisa;
    *product_exp = multiplicand_exp + multiplier_exp;

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_math_multiply */


/*******************************************************************************
* util_schd_mem_pool_assign
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function provides a method to set the in_use flag to not available and
*  returns a pointer to the mem_pool.
*
* INPUTS:
*   *h            - pointer to util_schd handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   A pointer to the mem_pool.
*
* NOTES:
*
*******************************************************************************/
PRIVATE util_schd_chnl_ctxt_t* util_schd_mem_pool_assign(util_schd_handle_t *h)
{
    UINT32 size = 0;
    
    PMC_ENTRY();

    PMC_ASSERT(h->var.util_schd_mem_pool_in_use==FALSE, UTIL_SCHD_MEM_POOL_NOT_AVAILABLE, 0, 0);
    
    size = sizeof(util_schd_chnl_ctxt_t);
    
    PMC_MEMSET(h->var.util_schd_mem_pool, 0, size);
    h->var.util_schd_mem_pool_in_use = TRUE;
    
    PMC_RETURN(h->var.util_schd_mem_pool);
} /* util_schd_mem_pool_assign */

/*******************************************************************************
* util_schd_mem_pool_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function provides a method to set the in_use flag to available for the 
*  pointer in use from the *util_schd_mem_pool in util_schd var_t. 
*  This functions also replaces PMC_FREE for the functions where we previously 
*  CALLOCed memory as mentioned in util_schd_mem_pool_assign().
*
* INPUTS:
*   *h            - pointer to util_schd handle instance
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
PRIVATE void util_schd_mem_pool_release(util_schd_handle_t *h)
{
    PMC_ENTRY();

    PMC_ASSERT(h->var.util_schd_mem_pool_in_use==TRUE, UTIL_SCHD_MEM_POOL_AVAILABLE, 0, 0);
    h->var.util_schd_mem_pool_in_use = FALSE;

    PMC_RETURN();
} /* util_schd_mem_pool_release */

/*******************************************************************************
* util_schd_mem_pool_alloc
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function allocates the mem_pool.  On cold start, util_schd_handle_init()
*  will call this function.  On restart, util_schd_handle_restart_init() will 
*  call this function.
*
* INPUTS:
*   *h            - pointer to util_schd handle instance
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
PRIVATE void util_schd_mem_pool_alloc(util_schd_handle_t *h)
{
    UINT32 mem_pool_size;

    PMC_ENTRY();

    mem_pool_size = sizeof(util_schd_chnl_ctxt_t)*(h->cfg.block_ctxt.num_schd_chnl) +
                    sizeof(UINT32)*(h->cfg.block_ctxt.max_entries) +
                    sizeof(UINT32)*(h->cfg.block_ctxt.max_entries);

    if (mem_pool_size > 0)
    {
        h->var.util_schd_mem_pool = (util_schd_chnl_ctxt_t*)PMC_CALLOC(mem_pool_size);

        PMC_ASSERT(h->var.util_schd_mem_pool != NULL, UTIL_SCHD_ERR_INVALID_ARG, 0 , 0);    
    }
    else
    {
        h->var.util_schd_mem_pool = NULL;
    }

    PMC_RETURN();
} /* util_schd_mem_pool_alloc */

/*******************************************************************************
* util_schd_chnl_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number to be updated.
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   *chnl_state_ptr                 - channel state of channel.
*                                     See util_schd_chnl_state_t for valid
*                                     values.
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
PUBLIC PMC_ERROR util_schd_chnl_state_get(util_schd_handle_t *h,
                                          UINT32 chnl_id,
                                          util_schd_chnl_state_t *chnl_state_ptr)
{
    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = &(h->cfg.block_ctxt);
    util_schd_chnl_ctxt_t *p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    PMC_ENTRY();

    /* Determine the channel index */
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    /* Retrieve the current state */
    *chnl_state_ptr = p_chnl_ctxt[chnl_index].state;

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_chnl_state_get */


/*******************************************************************************
* util_schd_custom_userbit_mask_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                  - pointer to scheduler handle instance
*   chnl                - Channel id to apply custom
*                         userbits to
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
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR util_schd_custom_userbit_mask_set(util_schd_handle_t *h,
                                                   UINT32 chnl,
                                                   BOOL enable,
                                                   UINT32 set_mask[3],
                                                   UINT32 userbit_mask[3])
{

    UINT32 i;

    PMC_ENTRY();

    

    /*set the context to use the customer user bit*/
    h->var.use_custom_userbit_mask[chnl] = enable;

    /*update the custom user bit mask */
    if(enable)
    {
        for(i=0;i<3;i++)
        {
            h->var.custom_userbit[i] = ((h->var.custom_userbit[i] & ~(set_mask[i])) | (userbit_mask[i] & set_mask[i]));
        }
    }
    
    
    
    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_custom_userbit_mask_set */



/*******************************************************************************
* util_schd_chnl_state_is_dummy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number to be updated.
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   *dummy_chnl_state               - dummy channel state of channel.
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
PUBLIC PMC_ERROR util_schd_chnl_state_is_dummy(util_schd_handle_t *h,
                                               UINT32 chnl_id,
                                               BOOL8 *dummy_chnl_state)
{
    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = &(h->cfg.block_ctxt);
    util_schd_chnl_ctxt_t *p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    PMC_ENTRY();

    /* Determine the channel index */
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    /* Retrieve the current state */
    *dummy_chnl_state = p_chnl_ctxt[chnl_index].fo1_mpmo_dummy;

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_chnl_state_is_dummy */

/*******************************************************************************
* util_schd_chnl_state_dummy_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the dummy channel state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                              - pointer to scheduler handle instance
*   chnl_id                         - scheduler channel number to be updated.
*                                     range blk_ctxt->base_chnl_num to
*                                     blk_ctxt->base_chnl_num +
*                                     blk_ctxt->num_schd_chnl
*   dummy_chnl_state                - dummy channel state of channel.
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
PUBLIC PMC_ERROR util_schd_chnl_state_dummy_set(util_schd_handle_t *h,
                                               UINT32 chnl_id,
                                               BOOL8 dummy_chnl_state)
{
    UINT32 chnl_index;
    util_schd_block_const_ctxt_t *p_block_const_ctxt = &(h->cfg.block_ctxt);
    util_schd_chnl_ctxt_t *p_chnl_ctxt = h->var.schd_ctxt->chnl_ctxt;

    PMC_ENTRY();

    /* Determine the channel index */
    chnl_index = chnl_id - p_block_const_ctxt->base_chnl_num;
    chnl_index = chnl_index/p_block_const_ctxt->chnl_num_step;

    /* Retrieve the current state */
    p_chnl_ctxt[chnl_index].fo1_mpmo_dummy = dummy_chnl_state;

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_chnl_state_dummy_set */

/*******************************************************************************
* util_schd_calendar_pinch_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the dummy channel state for a channel in scheduler channel context data
*   structure.
*
* INPUTS:
*   *h                  - pointer to scheduler handle instance
*   cal_pinch_type      - See util_global_calendar_pinch_t.  Used to set a custom
*                         calendar pinch scenario.
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
PUBLIC PMC_ERROR util_schd_calendar_pinch_mode_set(util_schd_handle_t *h,
                                                   util_global_calendar_pinch_t cal_pinch_type)
{
    PMC_ENTRY();

    h->var.cal_pinch_type = cal_pinch_type;

    PMC_RETURN(PMC_SUCCESS);

} /* util_schd_calendar_pinch_mode_set */



/*
** End of file
*/












