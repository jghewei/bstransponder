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
*   DESCRIPTION : This file contains C functions for the creation and 
*                 destruction of MAPOTN subsystem and data path configuration.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "mapotn_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/*
 * Represents the number of register addresses packed into a batch read for
 * MAPOTN interrupt retrieval.
 */
#define NUM_ADDRESSES 92*3

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
const char MAPOTN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    MAPOTN_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char MAPOTN_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE PMC_ERROR mapotn_int_init(mapotn_handle_t *mapotn_handle,
                                  util_global_mapping_mode_t mapping_mode,
                                  BOOL8 enable);

PRIVATE PMC_ERROR mapotn_db_chnl_entry_get(mapotn_handle_t *mapotn_handle, 
                                           util_gen_db_id_t db_id,
                                           UINT32 chnl, 
                                           UINT32 num_cal_entries, 
                                           UINT32 *cal_entries);
PRIVATE PMC_ERROR mapotn_ctxt_audit(mapotn_handle_t *mapotn_handle,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                    mapotn_recover_var_t *mapotn_var_reg,                                         
                                    mapotn_energy_state_t *mapotn_energy_state_reg,
                                    mapotn_var_t *mapotn_var_clean);
PRIVATE PMC_ERROR mapotn_ctxt_cleanup(mapotn_handle_t *mapotn_handle,
                                      mapotn_recover_var_t *mapotn_var_reg,                                         
                                      mapotn_energy_state_t *mapotn_energy_state_reg,
                                      mapotn_var_t *mapotn_var_clean);
PRIVATE void mapotn_ctxt_mpmo_cleanup(mapotn_handle_t *mapotn_handle,
                                      util_global_restart_init_cfg_t *restart_init_cfg_ptr);
PRIVATE PMC_ERROR mapotn_ctxt_reg_recover(mapotn_handle_t *mapotn_handle,
                                          pmc_energy_state_t top_energy_state_reg,                                         
                                          mapotn_recover_var_t *mapotn_var_reg,                                         
                                          mapotn_energy_state_t *mapotn_energy_state_reg);

PRIVATE void mapotn_var_default_init(mapotn_var_t *mapotn_var);
PRIVATE void mapotn_var_volatile_ctxt_init(mapotn_handle_t *mapotn_handle,BOOL8 from_reg);
PRIVATE PMC_ERROR mapotn_recover_adaptation_func(mapotn_handle_t *mapotn_handle, 
                                                 mapotn_path_t path,
                                                 UINT32 chnl,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 num_cal_entries,
                                                 mapotn_src_dest_t *src_dest,
                                                 util_global_map_adapt_func_t *adaptation_function, 
                                                 BOOL8 *fuzzy_flag);
PRIVATE util_global_map_adapt_func_t mapotn_resolve_fuzzy_adaptation_func(mapotn_handle_t *mapotn_handle,                                                  
                                                                          util_global_map_adapt_func_t recov_adaptation_func, 
                                                                          util_global_map_adapt_func_t var_adaptation_func);
PRIVATE util_global_map_adapt_func_t mapotn_adaptation_func_per_rate(mapotn_handle_t *mapotn_handle, 
                                                                     DOUBLE client_rate,
                                                                     BOOL8 is_GMP);
PRIVATE PMC_ERROR mapotn_var_update(mapotn_handle_t *mapotn_handle, 
                                    mapotn_path_t path,
                                    UINT32 chnl,
                                    mapotn_chnl_state_t chnl_state,
                                    util_global_map_adapt_func_t adaptation_function,
                                    util_global_odukp_type_t odukp_type,  
                                    DOUBLE client_rate,
                                    UINT32 num_cal_entries,
                                    util_global_mapping_mode_t mapotn_mapping_mode, 
                                    mapotn_src_dest_t chnl_src_dest);  
                               
PRIVATE PMC_ERROR mapotn_client_rate_cal(mapotn_handle_t *mapotn_handle, 
                                         util_global_map_adapt_func_t adaptation_function,
                                         UINT32 oduflex_cbr_rate,
                                         UINT32 oduflex_gfp_rate,  
                                         UINT32 cbr_client_rate, 
                                         DOUBLE *client_rate);
                                         
PRIVATE PMC_ERROR mapotn_dci_2_dpi_fifo_enbl(mapotn_handle_t *mapotn_handle, 
                                             UINT32 chnl,
                                             UINT32 valid);                                                                                  

PRIVATE void mapotn_energy_state_reg_get(mapotn_handle_t *mapotn_handle, 
                                         mapotn_energy_state_t *mapotn_energy_state);

                                                      
PRIVATE PMC_ERROR mapotn_rgfpf_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                    pmc_energy_state_t energy_state);   
                                                    
PRIVATE PMC_ERROR mapotn_mpma_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state);                                                                                                       

PRIVATE PMC_ERROR mapotn_rgmp_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state); 
                                                                                                                                                                                                                                                        
PRIVATE PMC_ERROR mapotn_demapper_soft_reset_cfg(mapotn_handle_t *mapotn_handle,
                                                 pmc_energy_state_t energy_state);

 
 
/* MAPOTN TX */  
  
PRIVATE PMC_ERROR mapotn_tgfpf_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                    pmc_energy_state_t energy_state);   
                                                    
PRIVATE PMC_ERROR mapotn_mpmo_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state);                                                                                                       

PRIVATE PMC_ERROR mapotn_tgmp_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state); 
                                                   
PRIVATE PMC_ERROR mapotn_oduksc_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                    pmc_energy_state_t energy_state);                                                     
  

PRIVATE PMC_ERROR mapotn_mapper_soft_reset_cfg(mapotn_handle_t *mapotn_handle,
                                               pmc_energy_state_t energy_state);

PRIVATE PMC_ERROR mapotn_mapper_internal_tgmp_cfc_xoff_cfg(mapotn_handle_t *mapotn_handle,
                                                           UINT32 chnl,
                                                           BOOL8 procedural_apply,
                                                           BOOL8 super_prov);

/*
** Public Functions
*/

/*******************************************************************************
* mapotn_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a MAPOTN subsystem instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the MAPOTN subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   mapotn_handle_t      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC mapotn_handle_t *mapotn_ctxt_create(pmc_handle_t *parent, 
                                           UINT32 base_address, 
                                           pmc_sys_handle_t *sys_handle, 
                                           const char *tsb_name)
{
    mapotn_handle_t *mapotn_handle;
    
    PMC_ENTRY();

    mapotn_handle = (mapotn_handle_t*)PMC_CTXT_CALLOC(sizeof(mapotn_handle_t), parent);
    
    pmc_handle_init( parent, mapotn_handle, sys_handle, PMC_MID_DIGI_MAPOTN, tsb_name, base_address );
    
    /* Register log strings. */
    pmc_log_block_strings_register(MAPOTN_LOG_ERR_STRINGS[0], MAPOTN_LOG_ERR_TABLE_BASE, MAPOTN_LOG_ERR_COUNT);
    
    mapotn_handle->ohfs_remove_handle = ohfs_remove_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_RX_RGFPF96_OHFS_REMOVE, sys_handle, "OHFS_REMOVE");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->ohfs_remove_handle, parent);
    mapotn_handle->cfc_unpack_handle = cfc_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_RX_RGFPF96_UNPACK_CFC_CFC, sys_handle, "CFC_UNPACK");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->cfc_unpack_handle, parent);
    mapotn_handle->cfc_egress_handle = cfc_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_RX_RGFPF96_EGRESS_CFC_CFC, sys_handle, "CFC_EGRESS");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->cfc_egress_handle, parent);
    mapotn_handle->mpma_handle = mpma_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_RX_MPMA_MTSB, sys_handle, "MPMA");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->mpma_handle, parent);
    mapotn_handle->rgmp_handle = rgmp_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_RX_RGMP96_MTSB, sys_handle, "RGMP");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->rgmp_handle, parent);
    
    mapotn_handle->ohfs_insert_handle = ohfs_insert_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_TX_TGFPF96_OHFS_INSERT, sys_handle, "OHFS_INSERT");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->ohfs_insert_handle, parent);
    mapotn_handle->cfc_packing_handle = cfc_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_TX_TGFPF96_PACKING_FIFO_CFC_CFC, sys_handle, "CFC_PACK");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->cfc_packing_handle, parent);
    mapotn_handle->cfc_prefetch_handle = cfc_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_TX_TGFPF96_PREFETCH_FIFO_CFC_CFC, sys_handle, "CFC_PREFETCH");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->cfc_prefetch_handle, parent);
    mapotn_handle->mpmo_handle = mpmo_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_TX_MPMO_MTSB, sys_handle, "MPMO");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->mpmo_handle, parent);
    mapotn_handle->tgmp_handle = tgmp_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_TX_TGMP96_TGMP96_CORE, sys_handle, "TGMP");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->tgmp_handle, parent);
    
    
    mapotn_handle->oduksc_handle = oduksc_ctxt_create((pmc_handle_t *)mapotn_handle, base_address+BASE_ADDR_MAPOTN_MAPOTN_TX_ODUKSC_MTSB, sys_handle, "ODUKSC");
    PMC_CTXT_REGISTER_PTR(&mapotn_handle->oduksc_handle, parent);
    
    PMC_RETURN(mapotn_handle);
    
} /* mapotn_ctxt_create */  


/*******************************************************************************
* mapotn_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a MAPOTN subsystem instance.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
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
PUBLIC PMC_ERROR mapotn_ctxt_destroy(mapotn_handle_t *mapotn_handle)
{ 
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    ohfs_remove_ctxt_destroy(mapotn_handle->ohfs_remove_handle);
    cfc_ctxt_destroy(mapotn_handle->cfc_unpack_handle);
    cfc_ctxt_destroy(mapotn_handle->cfc_egress_handle);
    mpma_ctxt_destroy(mapotn_handle->mpma_handle); 
    rgmp_ctxt_destroy(mapotn_handle->rgmp_handle); 
      
    ohfs_insert_ctxt_destroy(mapotn_handle->ohfs_insert_handle);
    cfc_ctxt_destroy(mapotn_handle->cfc_packing_handle);
    cfc_ctxt_destroy(mapotn_handle->cfc_prefetch_handle);
    mpmo_ctxt_destroy(mapotn_handle->mpmo_handle); 
    tgmp_ctxt_destroy(mapotn_handle->tgmp_handle); 
    
    oduksc_ctxt_destroy(mapotn_handle->oduksc_handle);
    
    ret_val = gen_db_detach(&(mapotn_handle->base),UTIL_GEN_DB_ODUKSC_DB,mapotn_handle->oduksc_db_handle);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }  
    ret_val = gen_db_detach(&(mapotn_handle->base),UTIL_GEN_DB_MPMA_DB,mapotn_handle->mpma_db_handle);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = gen_db_detach(&(mapotn_handle->base),UTIL_GEN_DB_MPMO_DB,mapotn_handle->mpmo_db_handle);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    PMC_CTXT_FREE(&mapotn_handle, mapotn_handle);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_ctxt_destroy */



/*******************************************************************************
* mapotn_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes a MAPOTN subsystem handle instance.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   db_mode             - defining database write access locally or externally
*                         Refer to mapotn_db_method_t
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
PUBLIC void mapotn_handle_init(mapotn_handle_t *mapotn_handle, 
                               mapotn_db_method_t db_mode)
{
    /* Variable declaration */
    cfc_cfg_t   cfc_cfg;
    cfc_cfg_t   packing_cfc_cfg;
    cfc_cfg_t   unpack_cfc_cfg;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(db_mode < LAST_MAPOTN_DB_MODE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_handle->cfg.db_mode = db_mode;
    
    /* Update static configurations */
    mapotn_handle->cfg.num_mapotn_chanl    = MAPOTN_NUM_CHANL;    

    /* CFC parameters */
    mapotn_handle->cfg.cfc_cfg.max_fifo_num  = MAPOTN_NUM_CHANL; 
    mapotn_handle->cfg.cfc_cfg.page_size     = MAPOTN_RGFPF_CFC_PAGE_SIZE;
    
    /* Initialize var_t parameters */
    mapotn_var_default_init(&mapotn_handle->var);
    mapotn_var_volatile_ctxt_init(mapotn_handle,FALSE);

    /* Default Ordered set UPI values (0x14). These values are not configured
     * in mapotn_var_default_init because that function is called whenever there
     * are no active mapotn channels. The ordered set upi values are non
     * channel based and are only configured once (before any channels are 
     * provisioned). See mapotn_ordered_set_upi_cfg.
     */
    mapotn_handle->var.tgfpf_ordered_set_upi = MAPOTN_DEFAULT_ORDERED_SET_UPI; 
    mapotn_handle->var.rgfpf_ordered_set_upi = MAPOTN_DEFAULT_ORDERED_SET_UPI;

    /* Create and Initialize Database */
    mapotn_handle->oduksc_db_handle = gen_db_attach(&(mapotn_handle->base),UTIL_GEN_DB_ODUKSC_DB);
    mapotn_handle->mpmo_db_handle = gen_db_attach(&(mapotn_handle->base),UTIL_GEN_DB_MPMO_DB);
    mapotn_handle->mpma_db_handle = gen_db_attach(&(mapotn_handle->base),UTIL_GEN_DB_MPMA_DB);
    
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = mapotn_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = mapotn_handle->cfg.cfc_cfg.page_size;
    unpack_cfc_cfg.max_fifo_num = mapotn_handle->cfg.cfc_cfg.max_fifo_num;
    if (pmc_is_digi_rev_a_revision(&mapotn_handle->base) == TRUE)
    {
        unpack_cfc_cfg.page_size = mapotn_handle->cfg.cfc_cfg.page_size;
    }
    else
    {
        unpack_cfc_cfg.page_size = 2 * mapotn_handle->cfg.cfc_cfg.page_size;
    }
    cfc_handle_init(mapotn_handle->cfc_unpack_handle, &unpack_cfc_cfg);
    cfc_handle_init(mapotn_handle->cfc_egress_handle, &cfc_cfg);
     
    /* Initialize OHFS_REMOVE */
    ohfs_remove_handle_init(mapotn_handle->ohfs_remove_handle, OHFS_REMOVE_INSTANCE_RGMP);
    
    /* Initialize MPMA */
    mpma_handle_init(mapotn_handle->mpma_handle, MPMA_MAPOTN, SCBS3_TOC_SYNC_MAPOTN); 
    
    /* Initialize RGMP */
    rgmp_handle_init(mapotn_handle->rgmp_handle, INSTANCE_RGMP);
    
    /* Initialize OHFS_INSERT */
    ohfs_insert_handle_init(mapotn_handle->ohfs_insert_handle, OHFS_INSERT_INSTANCE_TGMP);
    
    /* Initialize CFC */
    packing_cfc_cfg.max_fifo_num = mapotn_handle->cfg.cfc_cfg.max_fifo_num;

    /* packing fifo needs twice the page size */
    packing_cfc_cfg.page_size    = 2*(mapotn_handle->cfg.cfc_cfg.page_size);
    cfc_handle_init(mapotn_handle->cfc_packing_handle, &packing_cfc_cfg);
    cfc_handle_init(mapotn_handle->cfc_prefetch_handle, &cfc_cfg);
    
    /* Initialize MPMO */
    mpmo_handle_init(mapotn_handle->mpmo_handle, MPMO_MAPOTN, 0); 
    
    /* Initialize TGMP */
    tgmp_handle_init(mapotn_handle->tgmp_handle, INSTANCE_TGMP);
    
    /* Initialize ODUKSC */
    oduksc_handle_init(mapotn_handle->oduksc_handle, ODUKSC_MAPOTN, MAPOTN_ODUKSC_USERBITS_DEL);       

    PMC_RETURN();

} /* mapotn_handle_init */

/*******************************************************************************
* mapotn_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles allocation of local memory
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
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
PUBLIC void mapotn_alloc_init(mapotn_handle_t *mapotn_handle)
{
    PMC_ENTRY();

    cfc_alloc_init(mapotn_handle->cfc_unpack_handle);
    cfc_alloc_init(mapotn_handle->cfc_egress_handle);
    cfc_alloc_init(mapotn_handle->cfc_packing_handle);
    cfc_alloc_init(mapotn_handle->cfc_prefetch_handle);
    ohfs_insert_alloc_init(mapotn_handle->ohfs_insert_handle);
    tgmp_alloc_init(mapotn_handle->tgmp_handle);
    mpmo_alloc_init(mapotn_handle->mpmo_handle);
    mpma_alloc_init(mapotn_handle->mpma_handle);
    oduksc_alloc_init(mapotn_handle->oduksc_handle);

    PMC_RETURN();
} /* mapotn_alloc_init */

/*******************************************************************************
* mapotn_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on mapton ss handle instance.
*
* INPUTS:
*   *mapotn_handle              - pointer to MAPOTN handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state_reg        - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.\n
*   PMC_SUCCESS : 
*    WARM : register/context are coherent\n
*    CRASH: everything is coherent (a cleanup may has been executed)\n
*   MAPOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:\n
*    incoherences have been detected on communal energy configuration
*    that is incoherent with top energy state\n
*    WARM : ERROR\n
*    CRASH: TOP shall put MAPOTN in RESET mode\n
*   
*    subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_handle_restart_init(mapotn_handle_t *mapotn_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t top_energy_state_reg)
{
    mapotn_recover_var_t *mapotn_var_reg   = NULL;
    mapotn_var_t *mapotn_var_clean = NULL;
    cfc_cfg_t cfc_cfg;
    cfc_cfg_t packing_cfc_cfg;
    BOOL8 mismatch_channels;
    PMC_ERROR rc = PMC_SUCCESS;
    mapotn_energy_state_t mapotn_energy_state_reg = { PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT,
                                                      PMC_ENERGY_STATE_INCOHERENT };
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(MAPOTN_LOG_ERR_STRINGS[0], MAPOTN_LOG_ERR_TABLE_BASE, MAPOTN_LOG_ERR_COUNT);
    }

    cfc_cfg.max_fifo_num = mapotn_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = mapotn_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(mapotn_handle->cfc_unpack_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state_reg);
    cfc_handle_restart_init(mapotn_handle->cfc_egress_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state_reg);
    packing_cfc_cfg.max_fifo_num = mapotn_handle->cfg.cfc_cfg.max_fifo_num;
    packing_cfc_cfg.page_size    = 2*(mapotn_handle->cfg.cfc_cfg.page_size);
    cfc_handle_restart_init(mapotn_handle->cfc_packing_handle, &packing_cfc_cfg, restart_init_cfg_ptr, top_energy_state_reg);
    cfc_handle_restart_init(mapotn_handle->cfc_prefetch_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state_reg);
    ohfs_insert_handle_restart_init(mapotn_handle->ohfs_insert_handle, restart_init_cfg_ptr, top_energy_state_reg);
    tgmp_handle_restart_init(mapotn_handle->tgmp_handle, restart_init_cfg_ptr, top_energy_state_reg);
    
    /* Reinitialize OHFS_REMOVE */
    rc = ohfs_remove_handle_restart_init(mapotn_handle->ohfs_remove_handle, restart_init_cfg_ptr, top_energy_state_reg);

    /* Initialize MPMO */
    if (PMC_SUCCESS == rc)
    {
        rc = mpmo_handle_restart_init(mapotn_handle->mpmo_handle, restart_init_cfg_ptr, top_energy_state_reg, TRUE, &mismatch_channels);
    }

    /* Initialize MPMA */
    if (PMC_SUCCESS == rc)
    {
        rc = mpma_handle_restart_init(mapotn_handle->mpma_handle, restart_init_cfg_ptr, top_energy_state_reg, TRUE, &mismatch_channels);
    }
    if (PMC_SUCCESS == rc)
    {
        rc = oduksc_handle_restart_init(mapotn_handle->oduksc_handle,restart_init_cfg_ptr);
    }
    
    mapotn_var_reg = (mapotn_recover_var_t*) PMC_CALLOC(sizeof(mapotn_recover_var_t));

    
    /* reinitialize memory */


    /* 
       Recover context from register space        
          - var context is partially recovered from register space (good enough to have a valid audit)
          - only OPERATIONAL channel configruation are recovered
              - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
          - detailed energy states are recovered
          - incoherency in register context means that the subsystem shall be shutdown
    */
    if (PMC_SUCCESS == rc)
    {
        rc = mapotn_ctxt_reg_recover(mapotn_handle, 
                                     top_energy_state_reg,
                                     mapotn_var_reg,
                                     &mapotn_energy_state_reg);
    }    
    if (PMC_SUCCESS == rc)
    {
        mapotn_var_clean = (mapotn_var_t*) PMC_CALLOC(sizeof(mapotn_var_t));
        
        /* 
         * Audit register context and memory context 
         *   - Audit power and decide if we should just shut down the subsystem 
         *   - Create a new context that represents the intersection between memory and register space (mapotn_var_clean)
         */
        rc = mapotn_ctxt_audit(mapotn_handle, 
                               restart_init_cfg_ptr,
                               mapotn_var_reg,
                               &mapotn_energy_state_reg,
                               mapotn_var_clean); 
        
        if (MAPOTN_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
        {
            /* 
             * Cleanup 
             *   - At the end, mapotn_var_clean is the new context
             *    - We keep OPERATIONAL channel defined in mapotn_var_clean
             *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
             *       - they may have be partially provisionned
             *    - We ajust power setting based on mapotn_var_clean
             */            
            rc = mapotn_ctxt_cleanup(mapotn_handle, 
                                     mapotn_var_reg,
                                     &mapotn_energy_state_reg,
                                     mapotn_var_clean); 
            
        }
        PMC_FREE(&mapotn_var_clean);

        /* 
         * in second phase, we clean MPMO channels associated to OPERATIONAL channels 
         * that are unconnected to the wwith shall be cleaned. MPMO are provisionned when we connect 
         * the segment the switch 
         *  
         */
        if (PMC_SUCCESS == rc && restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_SECOND)
        {
            mapotn_ctxt_mpmo_cleanup(mapotn_handle,restart_init_cfg_ptr);
        }

    }

    
    PMC_FREE(&mapotn_var_reg);
    PMC_RETURN(rc);
} /* mapotn_handle_restart_init */

/*******************************************************************************
* mapotn_db_entry_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets a specific calendar offset for a channel ID.
*
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   schd_addr           - calendar offset to be set for chnl data
                          Valid values 0 to 95
*   chnl                - channel ID 
*                         Valid range: 0 - 95 
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
PUBLIC PMC_ERROR mapotn_db_entry_set(mapotn_handle_t *mapotn_handle, 
                                     UINT32 schd_addr,
                                     UINT32 chnl)
{
    mapotn_db_rec rec;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    /* if data base does not exist return an error */
    if(mapotn_handle->cfg.db_mode != MAPOTN_DB_MODE_EXTERNAL)
    {
       PMC_RETURN(MAPOTN_ERR_DB_EXT_ACCESS);
    }

    
    /* write schd_addr and chnl to the MAPOTN data bases */
    PMC_MEMSET((void *)&rec, 0, sizeof(mapotn_db_rec));
    rec.channel_num = chnl;
    rec.calendar_entry = schd_addr;
    ret_val = gen_db_add_record(&(mapotn_handle->base), UTIL_GEN_DB_ODUKSC_DB, mapotn_handle->oduksc_db_handle, (void *)&rec);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    } 
    
    ret_val = gen_db_add_record(&(mapotn_handle->base), UTIL_GEN_DB_MPMA_DB, mapotn_handle->mpma_db_handle, (void *)&rec);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = gen_db_add_record(&(mapotn_handle->base), UTIL_GEN_DB_MPMO_DB, mapotn_handle->mpmo_db_handle, (void *)&rec);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }                           

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_db_entry_set */


/*******************************************************************************
* mapotn_db_entry_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Removes a channel ID from the calendar.
*
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID 
*                         Valid range: 0 - 95 
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
PUBLIC PMC_ERROR mapotn_db_entry_clear(mapotn_handle_t *mapotn_handle, 
                                       UINT32 chnl)
{
    mapotn_db_rec rec;
    UINT32 i;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    /* if data base does not exist return an error */
    if(mapotn_handle->cfg.db_mode != MAPOTN_DB_MODE_EXTERNAL)
    {
       PMC_RETURN(MAPOTN_ERR_DB_EXT_ACCESS);
    }

    
    /* remove the records from the MAPOTN databases */
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {  
        rec.calendar_entry = DO_NOT_USE_KEY;
        rec.channel_num = chnl;
        ret_val = gen_db_rem_record(&(mapotn_handle->base), UTIL_GEN_DB_ODUKSC_DB, mapotn_handle->oduksc_db_handle, (void *)&rec);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        ret_val = gen_db_rem_record(&(mapotn_handle->base), UTIL_GEN_DB_MPMA_DB, mapotn_handle->mpma_db_handle, (void *)&rec);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        ret_val = gen_db_rem_record(&(mapotn_handle->base), UTIL_GEN_DB_MPMO_DB, mapotn_handle->mpmo_db_handle, (void *)&rec);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        
    }                        

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_db_entry_clear */


/*******************************************************************************
* mapotn_db_entry_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel ID from the given calendar offset.
*
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   schd_addr           - calendar offset to get the channel data
*                         Valid values 0 to 95
*
* OUTPUTS:
*   *chnl               - pointer to the channel ID 
*                         Valid range: 0 - 95 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_db_entry_get(mapotn_handle_t *mapotn_handle,
                                     UINT32 schd_addr, 
                                     UINT32 *chnl)
{
    UINT32 num_of_recs = 0;
    mapotn_db_key   key;
    mapotn_db_query_res *q_result;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(schd_addr < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    /* if data base does not exist return an error */
    if(mapotn_handle->cfg.db_mode != MAPOTN_DB_MODE_EXTERNAL)
    {
       PMC_RETURN(MAPOTN_ERR_DB_EXT_ACCESS);
    }

    
    /* query chnl ID data for the schd_addr */
    key.calendar_entry = schd_addr;
    key.channel_num = DO_NOT_USE_KEY;
    num_of_recs = gen_db_query(&(mapotn_handle->base),UTIL_GEN_DB_ODUKSC_DB,mapotn_handle->oduksc_db_handle, (void *)&key,(void **)&q_result);

    if(num_of_recs == 0)
    {
        PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
    }

    PMC_LOG_TRACE("MAPOTN DB Element: schd_addr=0x%x,cal channel_id=0x%x,\n",
               q_result->p_recs[0]->calendar_entry, q_result->p_recs[0]->channel_num); 
        
    *chnl = q_result->p_recs[0]->channel_num;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_db_entry_get */

/*******************************************************************************
* mapotn_db_entry_all_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This function provides a method for the datapath layer to retrieve the
*    all the entries of databases that are in use by the schedulers in MAPOTN.
*
* INPUTS:
*   *mapotn_handle         - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   chnl                   - 96 element array that function will populate with
*                            the channel data
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_db_entry_all_get( mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl[MAPOTN_NUM_CHANL] )
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i, num_of_recs;
    mapotn_db_key    key;
    mapotn_db_query_res *q_result;
    
    PMC_ENTRY();

    /* if data base does not exist return an error */
    if(mapotn_handle->cfg.db_mode != MAPOTN_DB_MODE_EXTERNAL) {

        result = MAPOTN_ERR_DB_EXT_ACCESS;

    } else {

        key.calendar_entry = CALENDAR_ENTRIES_NUM;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs = gen_db_query(&(mapotn_handle->base),
                                   UTIL_GEN_DB_ODUKSC_DB, 
                                   mapotn_handle->oduksc_db_handle,
                                   (void *)&key,
                                   (void **)&q_result); 
        
        for(i=0; i < CALENDAR_ENTRIES_NUM; i++) { 

            chnl[i] = q_result->p_recs[i]->channel_num;
        }

    }
    PMC_RETURN(result);
    
} /* mapotn_db_entry_all_get */

/*******************************************************************************
* mapotn_db_link_num_entries_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the number of calendar entries for a channel and a 
*   database 
*
* INPUTS:
*   *mapotn_handle       - pointer to MAPOTN handle instance
*   chnl                 - the channel ID
*
* OUTPUTS:
*   *num_of_recs_ptr     - number of calendar entries required by the channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_db_link_num_entries_get(mapotn_handle_t *mapotn_handle,
                                                UINT32         chnl,              
                                                UINT32        *num_of_recs_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    mapotn_db_key    key;
    
    PMC_ENTRY();
    
  /* if data base does not exist return an error */
    if(mapotn_handle->cfg.db_mode != MAPOTN_DB_MODE_EXTERNAL) {

        result = MAPOTN_ERR_DB_EXT_ACCESS;

    } else {
        key.calendar_entry = DO_NOT_USE_KEY;
        key.channel_num = chnl;
        *num_of_recs_ptr = gen_db_query_count(&(mapotn_handle->base),
                                              UTIL_GEN_DB_ODUKSC_DB,
                                              mapotn_handle->oduksc_db_handle,
                                              (void *)&key);

        if (*num_of_recs_ptr == 0)
        {
            PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
        }

    }

    PMC_RETURN(result);
} /* mapotn_db_link_num_entries_get */


/*******************************************************************************
* mapotn_cm_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves the CM Status for the particular MAPOTN channel.
*
* INPUTS:
*  *mapotn_handle       - pointer to MAPOTN handle instance
*  chnl_id              - the channel ID
*
* OUTPUTS:
*  *value               - Contents of the register.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mapotn_cm_status_get(mapotn_handle_t *mapotn_handle,
                                 UINT32 chnl_id,              
                                 UINT32 *value)
{
    PMC_ENTRY();
    
    rxjc_proc_cm_status_get(mapotn_handle->rgmp_handle->rxjc_proc_handle,
                            chnl_id,
                            value);

    PMC_RETURN();

} /* mapotn_cm_status_get */


/*******************************************************************************
*  mapotn_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures MAPOTN subsystem at a block level to provide or remove the ability
*   to perform channel level configuration.
*
*   In order to facilitate power savings, the block may be brought up to allow
*   processing GFP signals and at a later time to be further initialized to
*   perform AMP/BMP/GMP mapping.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state_request- enum, requested energy state
*   adaptation_function - major operational mode of resource\n 
*                         Refer to util_global_map_adapt_func_t for valid
*                         types\n
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
PUBLIC PMC_ERROR mapotn_init(mapotn_handle_t *mapotn_handle, 
                             pmc_energy_state_request_t energy_state_request,
                             util_global_map_adapt_func_t adaptation_function) 
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS; 

    PMC_ENTRY();
    
    if (PMC_ENERGY_STATE_REQUEST_RUNNING != energy_state_request)
    {
        /* the RESET/OPTIMAL case */
        /* we shutdown stuffs*/
        BOOL8 is_reset = (PMC_ENERGY_STATE_REQUEST_RESET == energy_state_request);
        UINT32 gmp_chnl_count, gfp_chnl_count;                
        mapotn_chnl_count_get(mapotn_handle, &gfp_chnl_count,&gmp_chnl_count);
        
        /* GFP */        
        if (TRUE == is_reset  || 0 == gfp_chnl_count) 
        {
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_tgfpf_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET);  
        }
        
        /* GMP */
        if (TRUE == is_reset  || 0 == gmp_chnl_count)
        {
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_rgmp_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET);
            mapotn_tgmp_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET); 
            mapotn_mpmo_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET);  
        }
        
        /* common resources */
        if (TRUE == is_reset  || (0 == gfp_chnl_count && 0 == gmp_chnl_count))
        {
            mapotn_var_default_init(&mapotn_handle->var);     
            mapotn_var_volatile_ctxt_init(mapotn_handle,FALSE);
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_mpma_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET);
            mapotn_oduksc_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET);          
            mapotn_rgfpf_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RESET);
            mapotn_demapper_soft_reset_cfg(mapotn_handle, PMC_ENERGY_STATE_RESET);    
            mapotn_mapper_soft_reset_cfg(mapotn_handle, PMC_ENERGY_STATE_RESET);    
            mapotn_int_init(mapotn_handle,UTIL_GLOBAL_NO_MAP,FALSE);  
        }
    } else 
    {
        util_global_mapping_mode_t mapotn_mapping_mode;

        /* get the mapping mode */
        (void) mapotn_mapping_mode_get(mapotn_handle, adaptation_function, &mapotn_mapping_mode);
        
        /* 
           common ressources
        */
        if(mapotn_start_state_test(mapotn_handle) == TRUE)
        {
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_demapper_soft_reset_cfg(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_mapper_soft_reset_cfg(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
            mapotn_handle->var.mapotn_start_state  = FALSE;
        }
        
        if(mpma_start_state_test(mapotn_handle->mpma_handle)== TRUE)
        {
            PMC_LOG_TRACE("configure MPMA energy state for first time\n");           
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_mpma_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
            mpma_init(mapotn_handle->mpma_handle, PMC_ENERGY_STATE_RUNNING);
            mpma_enable_cfg(mapotn_handle->mpma_handle, TRUE);
            PMC_LOG_TRACE("configure RGFPF energy state for first time\n");
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_rgfpf_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
            ret_val = mapotn_rgfpf_init(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
        }  
        
        if(oduksc_start_state_test(mapotn_handle->oduksc_handle)== TRUE)
        {
            PMC_LOG_TRACE("configure ODUKSC energy state for first time\n");           
            PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
            mapotn_oduksc_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
            oduksc_init(mapotn_handle->oduksc_handle, PMC_ENERGY_STATE_RUNNING);
            oduksc_reset_cfg(mapotn_handle->oduksc_handle, 0);
        }           
        /* 
           GFP blocks
        */
        if (UTIL_GLOBAL_GFP == mapotn_mapping_mode)
        {            
            if(mapotn_tgfpf_start_state_test(mapotn_handle)== TRUE) 
            { 
                PMC_LOG_TRACE("configure TGFPF energy state for first time\n");
                PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
                mapotn_tgfpf_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
                ret_val = mapotn_tgfpf_init(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
            } 
            
            /* RGMP is enabled to provide some interrupts in OHFS_REMOVE */
            if (TRUE ==  MAPOTN_DUAL_OHFS_REMOVE_MODE)
            {
                if(rgmp_start_state_test(mapotn_handle->rgmp_handle)== TRUE)
                {
                    PMC_LOG_TRACE("configure RGMP energy state for first time\n");
                    PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
                    mapotn_rgmp_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
                    ret_val = rgmp_init(mapotn_handle->rgmp_handle, PMC_ENERGY_STATE_RUNNING);
                }           
            }
        } else 
        {
            /* 
               GMP blocks
            */                               
            if(tgmp_start_state_test(mapotn_handle->tgmp_handle)== TRUE)
            {                
                PMC_LOG_TRACE("configure TGMP energy state for first time\n");
                PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
                mapotn_tgmp_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
                ret_val = tgmp_init(mapotn_handle->tgmp_handle, PMC_ENERGY_STATE_RUNNING);
            }
            
            if(mpmo_start_state_test(mapotn_handle->mpmo_handle)== TRUE)
            {
                PMC_LOG_TRACE("configure MPMO energy state for first time\n");
                PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
                mapotn_mpmo_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
                mpmo_init(mapotn_handle->mpmo_handle, PMC_ENERGY_STATE_RUNNING);
                mpmo_enable_cfg(mapotn_handle->mpmo_handle, TRUE);
            }       
            if(rgmp_start_state_test(mapotn_handle->rgmp_handle)== TRUE)
            {
                PMC_LOG_TRACE("configure RGMP energy state for first time\n");
                /* configure RGMP energy state */
                PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
                mapotn_rgmp_energy_state_reg_set(mapotn_handle, PMC_ENERGY_STATE_RUNNING);
                ret_val = rgmp_init(mapotn_handle->rgmp_handle, PMC_ENERGY_STATE_RUNNING);
            }    
        }
        mapotn_int_init(mapotn_handle,mapotn_mapping_mode,TRUE); 
    }
        
    PMC_RETURN(PMC_SUCCESS);
} /* mapotn_init */

/*******************************************************************************
* mapotn_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   MAPOTN instance.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8               - FALSE:  Resource(s) not in start state \n
*                         TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mapotn_start_state_test(mapotn_handle_t *mapotn_handle)
{
    /* Variable declaration */
    
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if( mapotn_handle->var.mapotn_start_state  == FALSE)
        PMC_RETURN(FALSE);
        
        
    PMC_RETURN(TRUE);
    
} /* mapotn_start_state_test */


/*******************************************************************************
* mapotn_num_cal_entry_req_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrives the number of calendar entries needed for the channel to be 
*   provisioned.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   odukp_type          - ODUkp or ODUjp type\n 
*                         Refer to util_global_odukp_type_t, ODUkp all ODUk
*                         types valid, ODUjp only ODU2/3 types valid
*   oduflex_cbr_rate    - ODUflex CBR container rate in kbits/sec.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A
*                         AND
*                         odukp_type set to ODUFLEX_CBR
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of ODU0 timeslots
*                         1 to 2 ODU1, 1 to 4 ODU2, 1 to 32 ODU3, 1 to 80 ODU4.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP 
*                         AND
*                         odukp_type set to ODUFLEX_GFP  
*     
*                                                                               
* OUTPUTS:
*   *num_cal_entry      - pointer to required number of calendar entires
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_num_cal_entry_req_get(mapotn_handle_t *mapotn_handle, 
                                              util_global_odukp_type_t odukp_type, 
                                              UINT32 oduflex_cbr_rate,
                                              UINT32 oduflex_gfp_rate,
                                              UINT32 *num_cal_entry)
{
    /* Variable declaration */
    UINT32 num_cal_entries;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(odukp_type < UTIL_GLOBAL_LAST_ODUK, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch (odukp_type)
    {
        case UTIL_GLOBAL_ODUFLEX_GFP:
            num_cal_entries = oduflex_gfp_rate;
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR: 
            PMC_ASSERT(oduflex_cbr_rate!=0, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
            num_cal_entries = (oduflex_cbr_rate/1000)/MAPOTN_PER_SLOT_BW;
            if((oduflex_cbr_rate/1000)%MAPOTN_PER_SLOT_BW > 0)
            {
                num_cal_entries  = num_cal_entries + 1;
            }
            break;
        case UTIL_GLOBAL_ODU0: 
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_ODU0;
            break;
        case UTIL_GLOBAL_ODU1:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_ODU1;
            break;
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_10G;
            break; 
        case UTIL_GLOBAL_ODU1F:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_10G;
            break;  
        case UTIL_GLOBAL_ODU2:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_10G;
            break;
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_10G;
            break;     
        case UTIL_GLOBAL_ODU2F:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_10G;
            break;
        case UTIL_GLOBAL_ODU3:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_40G;
            break;  
        case UTIL_GLOBAL_ODU3E1:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_40G;
            break;                                      
        case UTIL_GLOBAL_ODU3E2:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_40G;
            break; 
        case UTIL_GLOBAL_ODU4:
            num_cal_entries = MAPOTN_NUM_CAL_ENTRIES_IN_100G;
            break;     
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    
    *num_cal_entry = num_cal_entries;
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_num_cal_entry_req_get */

/*******************************************************************************
* mapotn_unused_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrives the unused MAPOTN channel ID.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
   
*                                                                               
* OUTPUTS:
*   *chnl               - pointer to unused channel ID
*                         Valid range: 0 - 95    
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                              
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_unused_chnl_get(mapotn_handle_t *mapotn_handle,
                                        UINT32 *chnl )
{
    /* Variable declaration */
    UINT32 i = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {
        if(mapotn_handle->var.tx_chnl_ctxt[i].state == MAPOTN_CHNL_START &&
           mapotn_handle->var.rx_chnl_ctxt[i].state == MAPOTN_CHNL_START )
        {
            /* found a unprovised/free chnl */
            PMC_LOG_TRACE("MAPOTN available channel ID is:%d\n", i);
            *chnl = i;
            PMC_RETURN(PMC_SUCCESS);
        }
    }
    
    PMC_RETURN(MAPOTN_CHNL_NOT_AVAILABLE);
} /* mapotn_unused_chnl_get */


/*******************************************************************************
* mapotn_chnl_avail_chk
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Checks the availability of the specified MAPOTN channel ID.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95       
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
PUBLIC PMC_ERROR mapotn_chnl_avail_chk(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl)
{
    PMC_ERROR result;

    /* Variable declaration */
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if(mapotn_handle->var.tx_chnl_ctxt[chnl].state == MAPOTN_CHNL_START &&
       mapotn_handle->var.rx_chnl_ctxt[chnl].state == MAPOTN_CHNL_START )
    {
        /* channel is available*/
        PMC_LOG_TRACE("MAPOTN channel ID is available\n");
        result = PMC_SUCCESS;
    }
    else
    {
        PMC_LOG_TRACE("MAPOTN channel ID is NOT available\n");
        result = MAPOTN_CHNL_NOT_AVAILABLE;
    }
    PMC_RETURN(result);
    
} /* mapotn_chnl_avail_chk */

/*******************************************************************************
* mapotn_head_tail_size_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrives the header byte lenght and end of frame length for GFP frames.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95         
*                                                                               
* OUTPUTS:
*   *head_byte_ptr      - pointer to the byte length of the GFP frame header            
*   *tail_ins_ptr       - pointer to enable inserting a 4 byte holecontrol if 
*                         payload FCS is inserted
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                            
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_head_tail_size_get(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl,
                                           UINT32 *head_byte_ptr, 
                                           BOOL8   *tail_ins_ptr)
{
    /* Variable declaration */
    UINT32 head_byte = 0;
    UINT32 ext_hdr_en = 0;
    UINT32 ext_hdr_len = 0;
    UINT32 fcs_ins = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    
    if(mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_GFP)
    {
        ext_hdr_en = mapotn_tgfpf_field_EXT_HDR_EN_get(NULL, mapotn_handle, chnl);
        if(ext_hdr_en)
        {
            ext_hdr_len = mapotn_tgfpf_field_EXT_HDR_LEN_get(NULL, mapotn_handle, chnl);
            head_byte = MAPOTN_CORE_TYPE_HDR_BYTE_LEN + ext_hdr_len;
        }
        else 
        {
            head_byte = MAPOTN_CORE_TYPE_HDR_BYTE_LEN;
        }
        
        fcs_ins = mapotn_tgfpf_field_FCS_INS_get(NULL, mapotn_handle, chnl);
        
    }
    else 
    {
        /* in case of GMP we don't need head/tail insertion its only for GFP frames*/
        head_byte = 0;
        fcs_ins = 0;
    }
    
    *head_byte_ptr = head_byte;

    if (0 == fcs_ins)
    {
        *tail_ins_ptr = FALSE;
    } else 
    {
        *tail_ins_ptr = TRUE;
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* mapotn_head_tail_size_get */


/*******************************************************************************
* mapotn_demapper_prov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Provisions MAPOTN RX channel according to its input parameters.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*   adaptation_function - major operational mode of resource\n 
*                         Refer to util_global_map_adapt_func_t for valid
*                         types\n
*   odukp_type          - ODUkp or ODUjp type\n 
*                         Refer to util_global_odukp_type_t, ODUkp all ODUk
*                         types valid, ODUjp only ODU2/3 types valid
*   oduflex_cbr_rate    - ODUflex CBR container rate in kbits/sec.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A 
*                         AND
*                         odukp_type set to ODUFLEX_CBR
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of ODU0 timeslots
*                         1 to 2 ODU1, 1 to 4 ODU2, 1 to 32 ODU3, 1 to 80 ODU4.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP 
*                         AND
*                         odukp_type set to ODUFLEX_GFP
*   cbr_client_rate     - Client data rate of signal on MAPOTN system interface
*                         in kbits/sec.  Argument processed mode set to one of 
*                         (else unused):
*                         UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP\n
*                         UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A\n
*                         UTIL_GLOBAL_MAP_ODUKP_CBRX_A\n
*   demapper_destination - Destination of the stream\n
*                          Refer to mapotn_src_dest_t\n
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
PUBLIC PMC_ERROR mapotn_demapper_prov(mapotn_handle_t *mapotn_handle,
                                      UINT32 chnl,
                                      util_global_map_adapt_func_t adaptation_function,
                                      util_global_odukp_type_t odukp_type, 
                                      UINT32 oduflex_cbr_rate,
                                      UINT32 oduflex_gfp_rate, 
                                      UINT32 cbr_client_rate, 
                                      mapotn_src_dest_t demapper_destination)
{
    /* Variable declaration */
    util_global_mapping_mode_t mapotn_mapping_mode;
    mapotn_chnl_state_t chnl_state;
    DOUBLE f_client_bit;
    UINT32 pkt_size;
    UINT32 pkt_size_range;
    UINT32 ps;
    UINT32 pkt_per_int;
    UINT32 pkt_per_n;
    UINT32 pkt_per_d;
    DOUBLE epsilon;
    util_opsa_cn_frame_period_t cn_frm_period;
    util_opsa_pkt_period_calc_t pkt_period_calc;
    UINT32 scbs3_user_bits_3_0;
    UINT32 num_ts;
    UINT32 ts_loc;
    UINT32 frm_num;
    UINT32 num_cal_entries;
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    
    /* variable initialization */
    num_ts = 0;
    cn_frm_period = UTIL_OPSA_T_FRAME_UNUSED;
    pkt_size = (adaptation_function != UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP) ? MAPOTN_MPMA_PKT_SIZE:(511-16);
    pkt_size_range = (adaptation_function != UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP) ? MAPOTN_MPMA_PKT_SIZE_RANGE:16;
    pkt_period_calc = UTIL_OPSA_EPS_MIN;
    ps = 0;
    pkt_per_int = 0; 
    pkt_per_n = 0;
    pkt_per_d = 0; 
    epsilon = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(adaptation_function <= UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(odukp_type < UTIL_GLOBAL_LAST_ODUK, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(demapper_destination < LAST_MAPOTN_DEST, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    PMC_LOG_TRACE( "Channel = %u, adaptation_function = %u\n", chnl, adaptation_function);  
    
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].state != MAPOTN_CHNL_START)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_START); /* it has either been already provisioned or activated*/
    }
    
    /* get mapping mode for a channel */
    ret_val = mapotn_mapping_mode_get(mapotn_handle, adaptation_function, &mapotn_mapping_mode);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }     
    
    ret_val = mapotn_cn_frame_period_get(mapotn_handle,odukp_type, &cn_frm_period);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    
    if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_EXTERNAL)
    {
        ret_val = mapotn_db_chnl_num_entries_get(mapotn_handle,chnl,UTIL_GEN_DB_MPMA_DB,&num_cal_entries);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    else 
    {
        ret_val = mapotn_num_cal_entry_req_get(mapotn_handle, odukp_type, oduflex_cbr_rate, oduflex_gfp_rate, &num_cal_entries);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        PMC_LOG_TRACE("num_cal_entries:%d\n", num_cal_entries);
    }
    
    ret_val = mapotn_client_rate_cal(mapotn_handle, adaptation_function, oduflex_cbr_rate, oduflex_gfp_rate, cbr_client_rate, &f_client_bit);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    PMC_LOG_TRACE("f_client_bit%f\n", f_client_bit);

    /* Update var context for a unused channel */
    chnl_state = mapotn_handle->var.rx_chnl_ctxt[chnl].state;
    mapotn_var_update(mapotn_handle, MAPOTN_RX, chnl, chnl_state, adaptation_function, odukp_type, f_client_bit, num_cal_entries, mapotn_mapping_mode, demapper_destination);
      
    /* SET scbs3_user_bits_3_0 */
    scbs3_user_bits_3_0 = 0x0;
    if (demapper_destination == MAPOTN_DEST_ENET || demapper_destination == MAPOTN_DEST_CPB_ENET) 
    {
        scbs3_user_bits_3_0 |= 0x1;
    } 
    if (demapper_destination == MAPOTN_DEST_CPB || demapper_destination == MAPOTN_DEST_CPB_ENET) 
    {
        scbs3_user_bits_3_0 |= 0x2;
    }
    if  (mapotn_mapping_mode == UTIL_GLOBAL_GFP) {
        scbs3_user_bits_3_0 |= 0x4;
    }
    
    if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
    {
        /* add channel data to the MPMA database */
        ret_val = mapotn_db_chnl_rec_add(mapotn_handle, chnl, num_cal_entries, UTIL_GEN_DB_MPMA_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {

        if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
        {
            UINT32 db_entries[MAPOTN_NUM_CHANL];
            UINT32 frm_num=0,slot;
            mapotn_handle->rgmp_handle->var.rgmp_chnl_cfg[chnl].state = RGMP_CHNL_EQUIPPED;
            
            ret_val = mapotn_db_entry_all_get(mapotn_handle,db_entries); 
            PMC_ASSERT(ret_val == PMC_SUCCESS, MAPOTN_ERR_DB_ENTRY_NOT_FOUND,0,0);
            for(slot=0; slot < MAPOTN_NUM_CHANL;slot++) 
            {
                if(db_entries[slot] == chnl) 
                {
                    /* note: the return code is always PMC_SUCCESS */
                    (void)ohfs_remove_ipt_cfg(mapotn_handle->rgmp_handle->ohfs_remove_handle,slot, chnl, frm_num, (frm_num == 0 ? 1 : 0)); 
                    frm_num++;
                }
            }
        }

        /* Configure RGFPF channel */
        ret_val = mapotn_rgfpf_chnl_prov(mapotn_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        
        /* Configure RGMP channel */
        if(odukp_type != UTIL_GLOBAL_ODUFLEX_CBR || odukp_type != UTIL_GLOBAL_ODUFLEX_GFP)
        {
            num_ts = 1;
        }
        ts_loc = 0;
        frm_num = 0;

        ret_val = odtu_dmx_rgmp_chnl_tribslot_cfg(mapotn_handle->rgmp_handle, chnl, ts_loc, adaptation_function, odukp_type, (util_global_odu_line_payload_t)0, mapotn_mapping_mode, 1, 1, 1, frm_num);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

        ret_val = odtu_dmx_rgmp_lo_chnl_prov(mapotn_handle->rgmp_handle, chnl, adaptation_function, odukp_type, mapotn_mapping_mode, (util_global_odu_line_payload_t)0, 0, f_client_bit, num_ts, FALSE, FALSE, 0);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }   
    }  
    
    
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP) 
    {
        /* Light version of MPMA*/                                                                                   
        ret_val = mpma_ch_prov(mapotn_handle->mpma_handle, chnl, f_client_bit, ps, 
                               cn_frm_period, pkt_period_calc, scbs3_user_bits_3_0, 
                               TRUE, pkt_per_int, pkt_per_n, pkt_per_d, epsilon);            
    } 
    else 
    {
        if (demapper_destination == MAPOTN_DEST_ENET || demapper_destination == MAPOTN_DEST_CPB_ENET)
        {
            /* enable DCI 2 DPI FIFO */
            mapotn_dci_2_dpi_fifo_enbl(mapotn_handle, chnl, 1);
        }
        if (demapper_destination == MAPOTN_DEST_ENET)
        {
            /* Light version of MPMA*/
            ret_val = mpma_ch_prov(mapotn_handle->mpma_handle, chnl, f_client_bit, ps,
                                   cn_frm_period, pkt_period_calc, scbs3_user_bits_3_0, 
                                   TRUE, pkt_per_int, pkt_per_n, pkt_per_d, epsilon);                                  
        }
        else
        {
           /* configure MPMA SCBS3 */
           ret_val = mpma_pkt_period_calc(mapotn_handle->mpma_handle, f_client_bit, pkt_size, 
                                          pkt_size_range, cn_frm_period,
                                          pkt_period_calc, &ps, &pkt_per_int, 
                                          &pkt_per_n, &pkt_per_d, &epsilon);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            } 
    
            PMC_LOG_TRACE("ps=%d, pkt_per_int=%d, pkt_per_n=%d, pkt_per_d=%d, epsilon=%f\n", ps, pkt_per_int, pkt_per_n, pkt_per_d, epsilon);
           
           /* Full version of MPMA*/
           ret_val = mpma_ch_prov(mapotn_handle->mpma_handle, chnl, f_client_bit, ps,
                                   cn_frm_period, pkt_period_calc, scbs3_user_bits_3_0, 
                                   FALSE, pkt_per_int, pkt_per_n, pkt_per_d, epsilon);                                 
        }
    }
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }  
    
    
    /* Update var context for a provisioned channel */
    chnl_state = MAPOTN_CHNL_EQUIPPED;
    mapotn_var_update(mapotn_handle, MAPOTN_RX, chnl, chnl_state, adaptation_function, odukp_type, f_client_bit, num_cal_entries, mapotn_mapping_mode, demapper_destination);
    
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_demapper_prov */


/*******************************************************************************
* mapotn_demapper_unprov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deprovisions the specific MAPOTN RX channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_demapper_unprov(mapotn_handle_t *mapotn_handle,
                                        UINT32 chnl)
{
    /* Variable declaration */
    util_global_map_adapt_func_t adaptation_function;
    util_global_mapping_mode_t mapotn_mapping_mode;
    mapotn_chnl_state_t chnl_state;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    adaptation_function = mapotn_handle->var.rx_chnl_ctxt[chnl].adaptation_func;
    mapotn_mapping_mode = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode;
    
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].state != MAPOTN_CHNL_EQUIPPED)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_EQUIPPED); /* it has either been already unprovisioned or deactivated*/
    }
    
    PMC_LOG_TRACE( "Channel = %u, adaptation_function = %u\n", chnl, adaptation_function);  
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        /* Deprov RGFPF channel */

        ret_val = mapotn_rgfpf_chnl_deprov(mapotn_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }  
        if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
        {
            UINT32 db_entries[MAPOTN_NUM_CHANL];
            UINT32 slot;
            
            ret_val = mapotn_db_entry_all_get(mapotn_handle,db_entries); 
            PMC_ASSERT(ret_val == PMC_SUCCESS, MAPOTN_ERR_INVALID_PARAMETERS,0,0);
            for(slot=0; slot < MAPOTN_NUM_CHANL;slot++) 
            {
                if(db_entries[slot] == chnl) 
                {
                    ret_val = ohfs_remove_chnl_unprov(mapotn_handle->rgmp_handle->ohfs_remove_handle,slot, chnl);
                    PMC_ASSERT(ret_val == PMC_SUCCESS, MAPOTN_ERR_INVALID_PARAMETERS,0,0);
                }
            }
        }         
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        /* Deprov RGMP channel */
        ret_val = odtu_dmx_rgmp_lo_chnl_unprov(mapotn_handle->rgmp_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    } 
    
    /* disable DCI 2 DPI FIFO */
    mapotn_dci_2_dpi_fifo_enbl(mapotn_handle, chnl, 0);  
    
    if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
    {
        /* remove channel data from the MPMA database */
        ret_val = mapotn_db_chnl_rec_rem(mapotn_handle, chnl, UTIL_GEN_DB_MPMA_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        } 
    }
    
    /* deprov MPMA */
    ret_val = mpma_ch_deprov(mapotn_handle->mpma_handle,chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* clear var context relative to channel */
    chnl_state = MAPOTN_CHNL_START;
    mapotn_var_update(mapotn_handle, MAPOTN_RX, chnl, chnl_state, UTIL_GLOBAL_MAP_NO_RATE, UTIL_GLOBAL_ODUK_DONT_CARE, 0, 0, UTIL_GLOBAL_NO_MAP, MAPOTN_DEST_CPB);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_demapper_unprov */

/*******************************************************************************
* mapotn_demapper_prov_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Crash-restart ASK operator for mapotn demapper provision function 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*   adaptation_function - major operational mode of resource\n 
*                         Refer to util_global_map_adapt_func_t for valid
*                         types\n
*               
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8               - FALSE:  Demapper is not in operational state\n
*                         TRUE:   Demapper is in operational state                                                              
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8  mapotn_demapper_prov_get( mapotn_handle_t                 *mapotn_handle,
                                        UINT32                           chnl,
                                        util_global_map_adapt_func_t     adaptation_function)
{
    BOOL8 result = FALSE;
    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if ((mapotn_handle->var.rx_chnl_ctxt[chnl].state == MAPOTN_CHNL_OPERATIONAL ||
         mapotn_handle->var.rx_chnl_ctxt[chnl].state == MAPOTN_CHNL_EQUIPPED)&&
        mapotn_handle->var.rx_chnl_ctxt[chnl].adaptation_func  == adaptation_function )
    {
        result = TRUE;
    }

    PMC_RETURN(result);
} /* mapotn_demapper_prov_get */

/*******************************************************************************
* mapotn_demapper_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean the memory and registr context of the specied MAPOTN RX channels. 
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   has_gfp             - GFP is not in low power 
*   has_gmp             - GMP is not in low power 
*   *chnl               - array of channel ID to be cleaned
*   num_chnl            - number of channels to be cleaned
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
PUBLIC void mapotn_demapper_clean(mapotn_handle_t *mapotn_handle,
                                  BOOL8 has_gfp,
                                  BOOL8 has_gmp,
                                  UINT32 *chnl,
                                  UINT32 num_chnl)
{   
    UINT32 chnl_itr;
    PMC_ENTRY();
    
    /* cleanup channels.. */
    for (chnl_itr = 0; chnl_itr < num_chnl; chnl_itr ++)
    {
        (void) mapotn_dci_2_dpi_fifo_enbl(mapotn_handle, chnl[chnl_itr], 0);  
    }
    if (TRUE == has_gmp ||
        (TRUE ==  MAPOTN_DUAL_OHFS_REMOVE_MODE && TRUE == has_gfp))
    {
        rgmp_chnl_clean(mapotn_handle->rgmp_handle,chnl,num_chnl);
    }
    if (TRUE == has_gfp)
    {
        mapotn_rgfpf_chnl_clean(mapotn_handle,chnl,num_chnl);
    }
    mpma_ch_clean(mapotn_handle->mpma_handle,chnl,num_chnl);
         
    PMC_RETURN();
} /* mapotn_demapper_clean */

/*******************************************************************************
* mapotn_demapper_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned MAPTON RX channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_demapper_activate(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl) 

{
    /* Variable declaration */
    util_global_mapping_mode_t mapotn_mapping_mode;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].state!= MAPOTN_CHNL_EQUIPPED)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    } 
    
     mapotn_mapping_mode = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode;
    /* activate MPMA */
    if (mapotn_mapping_mode == UTIL_GLOBAL_BMP) {
        ret_val = mpma_ch_activate(mapotn_handle->mpma_handle,chnl,FALSE);
    } else {
        ret_val = mpma_ch_activate(mapotn_handle->mpma_handle,chnl,TRUE);
    }
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    mapotn_mapping_mode = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode;
    
    PMC_LOG_TRACE( "Channel = %u, mapotn_mapping_mode = %u\n", chnl, mapotn_mapping_mode);
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        ret_val = mapotn_rgfpf_chnl_activate(mapotn_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        ret_val = rgmp_chnl_activate(mapotn_handle->rgmp_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }    
    
    /* Update var context for a activated channel */
    mapotn_handle->var.rx_chnl_ctxt[chnl].state = MAPOTN_CHNL_OPERATIONAL; 
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_demapper_activate */


/*******************************************************************************
* mapotn_demapper_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned MAPOTN RX channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_demapper_deactivate(mapotn_handle_t *mapotn_handle,
                                            UINT32 chnl)                                        

{
    /* Variable declaration */
    util_global_mapping_mode_t mapotn_mapping_mode;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL); /* that channel is is not in activated state*/
    }
    
    mapotn_mapping_mode = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode; 
    
    PMC_LOG_TRACE( "Channel = %u, mapotn_mapping_mode = %u\n", chnl, mapotn_mapping_mode);
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        ret_val = mapotn_rgfpf_chnl_deactivate(mapotn_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        } 
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        ret_val = rgmp_chnl_deactivate(mapotn_handle->rgmp_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    
    /* deactivate MPMA */
    ret_val = mpma_ch_deactivate(mapotn_handle->mpma_handle,chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* Update var context for a deactivated channel */
    mapotn_handle->var.rx_chnl_ctxt[chnl].state = MAPOTN_CHNL_EQUIPPED;
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_demapper_deactivate */


/*******************************************************************************
* mapotn_mapper_prov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Provisions MAPOTN TX channel according to its input parameters.   
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*   adaptation_function - major operational mode of resource\n
*                         Refer to util_global_map_adapt_func_t for valid
*                         types\n
*   odukp_type          - ODUkp or ODUjp type\n 
*                         Refer to util_global_odukp_type_t, ODUkp all ODUk
*                         types valid, ODUjp only ODU2/3 types valid
*   oduflex_cbr_rate    - ODUflex CBR container rate in kbits/sec.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A 
*                         AND
*                         odukp_type set to ODUFLEX_CBR
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of ODU0 timeslots
*                         1 to 2 ODU1, 1 to 4 ODU2, 1 to 32 ODU3, 1 to 80 ODU4.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP 
*                         AND
*                         odukp_type set to ODUFLEX_GFP
*   cbr_client_rate     - Client data rate of signal on MAPOTN system interface
*                         in kbits/sec.  Argument processed mode set to one of 
*                         (else unused):\n
*                         UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP\n
*                         UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A\n
*                         UTIL_GLOBAL_MAP_ODUKP_CBRX_A\n  
*   enet_mapper_source  - Source of ENET data stream.  Valid for mapping_mode
*                         set to one of:\n               
*                         UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP\n   
*                         UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP\n    
*                         UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP\n   
*                         UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP\n    
*                         UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP\n                   
*                         UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP\n 
*                         UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP\n 
*                         UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP\n  
*                         0 -- CPB Subsystem\n
*                         1 -- ENET_LINE Subsystem\n               
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
PUBLIC PMC_ERROR mapotn_mapper_prov(mapotn_handle_t *mapotn_handle,
                                    UINT32 chnl,
                                    util_global_map_adapt_func_t adaptation_function,
                                    util_global_odukp_type_t odukp_type, 
                                    UINT32 oduflex_cbr_rate,
                                    UINT32 oduflex_gfp_rate,  
                                    UINT32 cbr_client_rate, 
                                    mapotn_src_dest_t enet_mapper_source)
{
    /* Variable declaration */
    util_global_mapping_mode_t mapotn_mapping_mode = UTIL_GLOBAL_NO_MAP;
    util_global_odukp_type_t odukp_type_int = UTIL_GLOBAL_LAST_ODUK;
    mapotn_chnl_state_t chnl_state;
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 num_cal_entries;
    DOUBLE f_client_bit;
    UINT32 rate;
    
    /* Variable initialization */
    num_cal_entries = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(adaptation_function <= UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(odukp_type < UTIL_GLOBAL_LAST_ODUK, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(enet_mapper_source < LAST_MAPOTN_DEST, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    PMC_LOG_TRACE("Channel = %u, adaptation_function = %u\n", chnl, adaptation_function);
    
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_START)
    {  
        ret_val = MAPOTN_CHNL_STATE_NOT_START; /* it has either been already provisioned or activated*/
    }
    
    if(PMC_SUCCESS == ret_val)
    {
        ret_val = mapotn_mapping_mode_get(mapotn_handle, adaptation_function, &mapotn_mapping_mode);
    }

    /* convert util_global_odukp_type_t odukp_type to uniquely identify data source - required
       for ODUKSC divider configuration */
    if(odukp_type == UTIL_GLOBAL_ODU1E && enet_mapper_source == MAPOTN_DEST_ENET)
    {
        odukp_type_int = UTIL_GLOBAL_ODU1E_ETRANS;
    }
    else if (odukp_type == UTIL_GLOBAL_ODU2E && enet_mapper_source == MAPOTN_DEST_ENET)
    {
        odukp_type_int = UTIL_GLOBAL_ODU2E_ETRANS;
    }
    else
    {
        odukp_type_int = odukp_type;
    }
            
    if(PMC_SUCCESS == ret_val)
    {
        if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_EXTERNAL)
        {
            ret_val = mapotn_db_chnl_num_entries_get(mapotn_handle,chnl,UTIL_GEN_DB_ODUKSC_DB,&num_cal_entries);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
        } 
        else 
        {
            ret_val = mapotn_num_cal_entry_req_get(mapotn_handle, odukp_type_int, oduflex_cbr_rate, 
                                                   oduflex_gfp_rate, &num_cal_entries);
            PMC_LOG_TRACE("num_cal_entries:%d\n", num_cal_entries);
        }
    }
                
    if(PMC_SUCCESS == ret_val)
    {
        ret_val = mapotn_client_rate_cal(mapotn_handle, adaptation_function, oduflex_cbr_rate, 
                                         oduflex_gfp_rate, cbr_client_rate, &f_client_bit);
        PMC_LOG_TRACE("f_client_bit%f\n", f_client_bit);
    }
                    
    if(PMC_SUCCESS == ret_val)
    {
        ret_val = mapotn_server_rate_cal(mapotn_handle, odukp_type_int, oduflex_cbr_rate, oduflex_gfp_rate, 
                                         &rate);
        PMC_LOG_TRACE("server_rate:%d\n", rate);
    }
                        
    if(PMC_SUCCESS == ret_val)
    {
        /* Update var context for a unused channel */
        chnl_state = mapotn_handle->var.tx_chnl_ctxt[chnl].state;
        ret_val = mapotn_var_update(mapotn_handle, MAPOTN_TX, chnl, chnl_state, adaptation_function, 
                                    odukp_type_int, f_client_bit, num_cal_entries, mapotn_mapping_mode,
                                    enet_mapper_source);
    }
    
    if(PMC_SUCCESS == ret_val)
    {            
        if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
        {
            /* Configure TGFPF channel */
            ret_val = mapotn_tgfpf_chnl_prov(mapotn_handle, chnl); 
        }  
        else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
                 mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
                 mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
        {
            if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL) 
            {
                /* add channel data to the MPMO database */
                ret_val = mapotn_db_chnl_rec_add(mapotn_handle, chnl, num_cal_entries, UTIL_GEN_DB_MPMO_DB);
            } 
            else if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_EXTERNAL)
            {
                ret_val = PMC_SUCCESS;  /*db is already updated from external layer*/
            }
            else
            {
               PMC_ASSERT(mapotn_handle->cfg.db_mode < LAST_MAPOTN_DB_MODE, MAPOTN_ERR_UNSUPPORTED_DB_MODE, 0, 0);
            }

            if(PMC_SUCCESS == ret_val)
            {
                /* Configure TGMP channel */ 
                ret_val = odtu_mux_tgmp_tribslot_cfg(mapotn_handle->tgmp_handle, chnl, 0, adaptation_function,
                                                     odukp_type_int, (util_global_odu_line_payload_t)0, mapotn_mapping_mode, 1, 1, 0, 0); 
            }
            
            if(PMC_SUCCESS == ret_val)
            {
                ret_val = odtu_mux_tgmp_lo_chnl_prov(mapotn_handle->tgmp_handle, chnl, adaptation_function, 
                                                     odukp_type_int, f_client_bit, mapotn_mapping_mode, (util_global_odu_line_payload_t)0, 1, 0); 
            }
        }
        else
        {
            PMC_ASSERT(mapotn_mapping_mode < UTIL_GLOBAL_NO_MAP, MAPOTN_ERR_INVALID_MAPPING_MODE, 0, 0);
        }
    } 
    
    if(PMC_SUCCESS == ret_val)
    { 
                        
        if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
        {
            /* add channel data to the ODUKSC database */
            ret_val = mapotn_db_chnl_rec_add(mapotn_handle, chnl, num_cal_entries, UTIL_GEN_DB_ODUKSC_DB);
        }
        else if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_EXTERNAL)
        {
            ret_val = PMC_SUCCESS;  /*db is already updated from external layer*/
        }
        else
        {
            PMC_ASSERT(mapotn_handle->cfg.db_mode < LAST_MAPOTN_DB_MODE, MAPOTN_ERR_UNSUPPORTED_DB_MODE, 0, 0);
        }
    }
        
    if(PMC_SUCCESS == ret_val)
    {
        /* provision oduksc */
        /* server_type, I need to understand */
        ret_val = oduksc_chnl_prov(mapotn_handle->oduksc_handle, chnl, UTIL_GLOBAL_ODUK_DONT_CARE, odukp_type_int,
                                   rate, mapotn_mapping_mode, oduflex_gfp_rate);
    }
                        
    if(PMC_SUCCESS == ret_val)
    {
        if(UTIL_GLOBAL_BMP == mapotn_mapping_mode && MAPOTN_DEST_CPB == enet_mapper_source)
        {
           /*Enable zone pushback */
           ret_val = oduksc_zone_pushback_cfg(mapotn_handle->oduksc_handle, chnl, 1);
        }
    }
                            
    if(PMC_SUCCESS == ret_val)
    {
        ret_val = oduksc_chnl_update(mapotn_handle->oduksc_handle, chnl, TRUE);
    }
                            
    if(PMC_SUCCESS == ret_val)
    {
        /* Update var context for a provisioned channel */
        chnl_state = MAPOTN_CHNL_EQUIPPED;
        ret_val = mapotn_var_update(mapotn_handle, MAPOTN_TX, chnl, chnl_state, adaptation_function,  
                                    odukp_type_int,f_client_bit, num_cal_entries, mapotn_mapping_mode, 
                                    enet_mapper_source);
    }
                       
    
    PMC_RETURN(ret_val); 
    
} /* mapotn_mapper_prov */

    
/*******************************************************************************
* mapotn_mapper_unprov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deprovisions the specific MAPOTN TX channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_mapper_unprov(mapotn_handle_t *mapotn_handle,
                                      UINT32 chnl)
{
    /* Variable declaration */
    util_global_map_adapt_func_t adaptation_function;
    util_global_mapping_mode_t mapotn_mapping_mode;
    mapotn_chnl_state_t chnl_state;
    mapotn_src_dest_t mapper_src_dest;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    adaptation_function = mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func;
    mapotn_mapping_mode = mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode;
    mapper_src_dest = mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest;
    
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_EQUIPPED)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_EQUIPPED); /* it has either been already unprovisioned or deactivated*/
    }
    
    PMC_LOG_TRACE( "Channel = %u, adaptation_function = %u\n", chnl, adaptation_function);  
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    { 
        /* Deprov TGFPF channel */
        ret_val = mapotn_tgfpf_chnl_deprov(mapotn_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        /* Add GMP stuff*/
        ret_val = odtu_mux_tgmp_lo_chnl_deprov(mapotn_handle->tgmp_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        
        if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
        {
            /* remove channel data from the MPMO database */
            ret_val = mapotn_db_chnl_rec_rem(mapotn_handle, chnl, UTIL_GEN_DB_MPMO_DB);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
        }
        
    }   
    
    if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
    {
        /* remove channel data from the ODUKSC database */
        ret_val = mapotn_db_chnl_rec_rem(mapotn_handle, chnl, UTIL_GEN_DB_ODUKSC_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        } 
    }
    
    /* Disable zone pushback - mapper_prov enables it for UTIL_GLOBAL_BMP and mapper dest = enet*/
    if(UTIL_GLOBAL_BMP == mapotn_mapping_mode && MAPOTN_DEST_CPB == mapper_src_dest)
    {
        ret_val = oduksc_zone_pushback_cfg(mapotn_handle->oduksc_handle, chnl, 0);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    
    /* deprov ODUKSC */
    ret_val = oduksc_chnl_deprov(mapotn_handle->oduksc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val =  oduksc_chnl_update(mapotn_handle->oduksc_handle, chnl, TRUE);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    } 
    
    
    /* clear var context relative to channel */
    chnl_state = MAPOTN_CHNL_START;
    mapotn_var_update(mapotn_handle, MAPOTN_TX, chnl, chnl_state, UTIL_GLOBAL_MAP_NO_RATE, UTIL_GLOBAL_ODUK_DONT_CARE, 0, 0, UTIL_GLOBAL_NO_MAP, MAPOTN_DEST_CPB);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_mapper_unprov */


/*******************************************************************************
* mapotn_mapper_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned MAPTON TX channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                   
*                                                                               
* NOTES:  Following the channel activation for GMP mapped datapath coming from 
*         ENET we need to call mapotn_mapper_tgmp_cfc_xoff_cfg() API as
*         MPMO CFC fifo will overflow and we need to reprogram the EXPAND IBUF 
*         threshold. (refer to 235554)                                                                       
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR mapotn_mapper_activate(mapotn_handle_t *mapotn_handle,
                                        UINT32 chnl) 

{
    /* Variable declaration */
    util_global_mapping_mode_t mapotn_mapping_mode;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state!= MAPOTN_CHNL_EQUIPPED)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    } 
    /* activate ODUKSc channel */
    /* it is important that the scheduler be activated before the GMP TSB */
    ret_val = oduksc_chnl_activate(mapotn_handle->oduksc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }   
    
    mapotn_mapping_mode = mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode;
    
    PMC_LOG_TRACE( "Channel = %u, mapotn_mapping_mode = %u\n", chnl, mapotn_mapping_mode);
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        ret_val = mapotn_tgfpf_chnl_activate(mapotn_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        ret_val = tgmp_chnl_activate(mapotn_handle->tgmp_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        
    }    

    /* Update var context for a activated channel */
    mapotn_handle->var.tx_chnl_ctxt[chnl].state = MAPOTN_CHNL_OPERATIONAL; 
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_mapper_activate */


/*******************************************************************************
* mapotn_mapper_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned MAPOTN TX channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_mapper_deactivate(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl)                                        

{
    /* Variable declaration */
    util_global_mapping_mode_t mapotn_mapping_mode;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL); /* that channel is is not in activated state*/
    }
    
    mapotn_mapping_mode = mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode; 
    
    PMC_LOG_TRACE( "Channel = %u, mapotn_mapping_mode = %u\n", chnl, mapotn_mapping_mode);
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        ret_val = mapotn_tgfpf_chnl_deactivate(mapotn_handle, chnl); 
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        ret_val = tgmp_chnl_deactivate(mapotn_handle->tgmp_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }    
    
    /* deactivate ODUKSC */
    ret_val = oduksc_chnl_deactivate(mapotn_handle->oduksc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* Update var context for a deactivated channel */
    mapotn_handle->var.tx_chnl_ctxt[chnl].state = MAPOTN_CHNL_EQUIPPED;
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_mapper_deactivate */

/*******************************************************************************
* mapotn_mapper_prov_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Crash-restart ASK operator for mapotn mapper provision function 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*   adaptation_function - major operational mode of resource\n
*                         Refer to util_global_map_adapt_func_t for valid
*                         types        
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8               - FALSE:  Mapper is not in operational state\n
*                         TRUE:   Mapper is in operational state                                                          
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8 mapotn_mapper_prov_get(mapotn_handle_t *mapotn_handle,
                                    UINT32 chnl,
                                    util_global_map_adapt_func_t adaptation_function)
{
    BOOL8 result = FALSE;
    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if ((mapotn_handle->var.tx_chnl_ctxt[chnl].state == MAPOTN_CHNL_OPERATIONAL ||
         mapotn_handle->var.tx_chnl_ctxt[chnl].state == MAPOTN_CHNL_EQUIPPED) &&
        mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func  == adaptation_function)
    {
        result = TRUE;     
    }

    PMC_RETURN(result);
} /* mapotn_mapper_prov_get */

/*******************************************************************************
* mapotn_mapper_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean meomry and register contexts for specied MAPOTN TX channels. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   has_gfp             - GFP is not in low power 
*   has_gmp             - GMP is not in low power 
*   *chnl               - array of channel ID to be cleaned
*   num_chnl            - number of channels to be cleaned
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
PUBLIC void mapotn_mapper_clean(mapotn_handle_t *mapotn_handle,
                                BOOL8 has_gfp,
                                BOOL8 has_gmp,
                                UINT32 *chnl,
                                UINT32 num_chnl)
{   
    PMC_ENTRY();
    
    if (TRUE == has_gmp)
    {
        tgmp_chnl_clean(mapotn_handle->tgmp_handle,chnl,num_chnl);
        mpmo_ch_clean(mapotn_handle->mpmo_handle,chnl,num_chnl);
    }
    if (TRUE == has_gfp)
    {
        mapotn_tgfpf_chnl_clean(mapotn_handle,chnl,num_chnl);
    }
    oduksc_chnl_clean(mapotn_handle->oduksc_handle,chnl,num_chnl);
     
    PMC_RETURN();
} /* mapotn_mapper_clean */

/*******************************************************************************
* mapotn_mpmo_ch_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates MPMO channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
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
PUBLIC PMC_ERROR mapotn_mpmo_ch_activate(mapotn_handle_t *mapotn_handle,
                                         UINT32 chnl)
{   
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* test to determine if AMP/GMP mapping from DCPB, if so do
       special AMP/GMP from DCPB startup procedure */
    if((mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_AMP || 
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_GMP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_BMP) && 
        mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest     == MAPOTN_DEST_CPB)
    {
        if(result == PMC_SUCCESS)
        {
            result = oduksc_chnl_deactivate(mapotn_handle->oduksc_handle, chnl);
        }

        if(result == PMC_SUCCESS)
        {

            result = oduksc_chnl_update(mapotn_handle->oduksc_handle, chnl, FALSE);
        }

        if(result == PMC_SUCCESS)
        {
            result = tgmp_chnl_deactivate(mapotn_handle->tgmp_handle, chnl);
        }

        if(result == PMC_SUCCESS)
        {
            result = mpmo_ch_activate(mapotn_handle->mpmo_handle, chnl);
        }

        if(result == PMC_SUCCESS)
        {
            result = tgmp_chnl_amp_gmp_startup_activate(mapotn_handle->tgmp_handle, chnl);
        }

        if(result == PMC_SUCCESS)
        {
            result = oduksc_chnl_activate(mapotn_handle->oduksc_handle, chnl);
        }
    }
    else
    {
        result = mpmo_ch_activate(mapotn_handle->mpmo_handle, chnl);
    }
   
    PMC_RETURN(result);
} /* mapotn_mpmo_ch_activate */

/*******************************************************************************
* mapotn_mpmo_ch_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates MPMO channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
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
PUBLIC PMC_ERROR mapotn_mpmo_ch_deactivate(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl)
{   
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = mpmo_ch_deactivate(mapotn_handle->mpmo_handle, chnl);
     
    PMC_RETURN(result);
} /* mapotn_mpmo_ch_deactivate */

/*******************************************************************************
* mapotn_mapper_tgmp_cfc_xoff_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function monitors the MPMO FIFO fill level 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_mapper_tgmp_cfc_xoff_cfg(mapotn_handle_t *mapotn_handle,
                                                 UINT32 chnl)                                        

{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    ret_val = mapotn_mapper_internal_tgmp_cfc_xoff_cfg(mapotn_handle, chnl, TRUE, FALSE);
    
    PMC_RETURN(ret_val);

} /* mapotn_mapper_tgmp_cfc_xoff_cfg */

/*******************************************************************************
* mapotn_mapper_tgmp_cfc_xoff_cfg_super_prov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function monitors the MPMO FIFO fill level 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR mapotn_mapper_tgmp_cfc_xoff_cfg_super_prov(mapotn_handle_t *mapotn_handle,
                                                            UINT32 chnl)                                        

{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    ret_val = mapotn_mapper_internal_tgmp_cfc_xoff_cfg(mapotn_handle, chnl, TRUE, TRUE);
    
    PMC_RETURN(ret_val);

} /* mapotn_mapper_tgmp_cfc_xoff_cfg_super_prov */

/*******************************************************************************
* mapotn_mapper_internal_tgmp_cfc_xoff_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function monitors the MPMO FIFO fill level 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*   procedural_apply    - method to apply the configuration: \n
*                         TRUE:  Fill Level 0'd, FIFO status tested then
*                                value applied \n
*                         FALSE: Only value applied
*   super_prov          - COREOTN ODU channel provisioning mode
*                         TRUE:  COREOTN ODU channels are provisioned in bulk
*                         FALSE: COREOTN ODU channels are provisioned in increment
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
PRIVATE PMC_ERROR mapotn_mapper_internal_tgmp_cfc_xoff_cfg(mapotn_handle_t *mapotn_handle,
                                                           UINT32 chnl,
                                                           BOOL8 procedural_apply,
                                                           BOOL8 super_prov)                                        

{
    /* Variable declaration */
    UINT32 cfc_fill_lvl;
    UINT32 num_cal_entries = 0;
    util_global_mapping_mode_t mapotn_mapping_mode;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_mapping_mode = mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode;

    if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
        mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
        mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {
        if(procedural_apply == TRUE)
        {

            if(ret_val == PMC_SUCCESS)
            {
                ret_val = oduksc_chnl_deactivate(mapotn_handle->oduksc_handle, chnl);
            }
    
            if(ret_val == PMC_SUCCESS)
            {
    
                ret_val = oduksc_chnl_update(mapotn_handle->oduksc_handle, chnl, FALSE);
            }
            if(ret_val == PMC_SUCCESS)
            {
                ret_val = tgmp_chnl_deactivate(mapotn_handle->tgmp_handle, chnl);
            }
            if(ret_val == PMC_SUCCESS)
            {
                ret_val = tgmp_chnl_amp_gmp_startup_activate(mapotn_handle->tgmp_handle, chnl);
            }
            if(ret_val == PMC_SUCCESS)
            {
                ret_val = oduksc_chnl_activate(mapotn_handle->oduksc_handle, chnl);
                if (ret_val != PMC_SUCCESS) 
                {
                    PMC_RETURN(ret_val);
                }
            }
            if (ret_val != PMC_SUCCESS) 
            {
                PMC_RETURN(ret_val);
            }

            cfc_fill_lvl = 0;
            ret_val = tgmp_expd_ibuf_cfc_xoff_level_cfg(mapotn_handle->tgmp_handle, chnl, cfc_fill_lvl);
            if (ret_val != PMC_SUCCESS) 
            {
                PMC_RETURN(ret_val);
            }
            
            /* monitor the interrupt FIFO_OVR_I */
            ret_val = mpmo_cfc_fifo_overflow_mon(mapotn_handle->mpmo_handle, chnl);
            if(ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
 
            /* wait 20us */
            PMC_Q_USLEEP(mapotn_handle,20);

            if (super_prov == TRUE && mapotn_mapping_mode == UTIL_GLOBAL_GMP)
            {
                /* toggle CH_EN in MAPOTN MPMO to get out of FIFO_UDR_I state */
                ret_val = mpmo_ch_enable_cfg(mapotn_handle->mpmo_handle, chnl, FALSE);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }

                ret_val = mpmo_ch_enable_cfg(mapotn_handle->mpmo_handle, chnl, TRUE);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }
            }
        }
        
        num_cal_entries = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries;
        cfc_fill_lvl = ((num_cal_entries * 3) + 1);
        ret_val = tgmp_expd_ibuf_cfc_xoff_level_cfg(mapotn_handle->tgmp_handle, chnl, cfc_fill_lvl);
        if (ret_val != PMC_SUCCESS) 
        {  
            PMC_RETURN(ret_val);
        }   
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_mapper_internal_tgmp_cfc_xoff_cfg */


/*******************************************************************************
* mapotn_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *mapotn_handle     - pointer to MAPOTN handle instance
*   chnl_id            - Channel ID: Valid range: 0 - 95
*   int_table_ptr      - pointer to aggregation or channel interrupt table with
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
PUBLIC PMC_ERROR mapotn_int_validate(mapotn_handle_t *mapotn_handle,
                                     UINT32 chnl_id,
                                     void *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != mapotn_handle, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= MAPOTN_NUM_CHANL)
    {
        result = MAPOTN_ERR_INVALID_PARAMETERS;
    }

    if (NULL == int_table_ptr)
    {
        result = MAPOTN_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* mapotn_int_validate */



/*******************************************************************************
* mapotn_int_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
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
PUBLIC PMC_ERROR mapotn_int_enable(mapotn_handle_t *mapotn_handle,
                                   mapotn_int_t *int_table_ptr,
                                   mapotn_int_t *int_en_table_ptr,
                                   BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    #ifdef MAPOTN_INT
    #undef MAPOTN_INT
    #endif
    #define MAPOTN_INT UTIL_GLOBAL_INT_ENABLE_SINGLE

    MAPOTN_INT_TABLE_DEFINE();
    
    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_enable(mapotn_handle->oduksc_handle,
                                   UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                   &int_en_table_ptr->oduksc,
                                   enable);
    }

    PMC_RETURN(result);
} /* mapotn_int_enable */

/*******************************************************************************
* mapotn_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
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
PUBLIC PMC_ERROR mapotn_int_clear(mapotn_handle_t *mapotn_handle,
                                  mapotn_int_t *int_table_ptr,
                                  mapotn_int_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    
    #ifdef MAPOTN_INT
    #undef MAPOTN_INT
    #endif
    #define MAPOTN_INT UTIL_GLOBAL_INT_CLEAR_SINGLE

    MAPOTN_INT_TABLE_DEFINE();
    
    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_clear(mapotn_handle->oduksc_handle,
                                  UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                  UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, oduksc));
    }


    PMC_RETURN(result);
} /* mapotn_int_clear */



/*******************************************************************************
* mapotn_int_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
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
PUBLIC PMC_ERROR mapotn_int_retrieve(mapotn_handle_t *mapotn_handle,
                                     mapotn_int_t *filt_table_ptr,
                                     mapotn_int_t *int_table_ptr)
{
    PMC_ERROR  result = PMC_SUCCESS;

    PMC_ENTRY();

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mapotn_handle, mapotn_rx, UTIL_GLOBAL_INT_NO_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mapotn_handle, mapotn_tx, UTIL_GLOBAL_INT_NO_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mapotn_handle, mapotn_rgfpf, UTIL_GLOBAL_INT_NO_OPTIMIZE);

    #ifdef MAPOTN_INT
    #undef MAPOTN_INT
    #endif
    #define MAPOTN_INT UTIL_GLOBAL_INT_RETRIEVE_SINGLE

    MAPOTN_INT_TABLE_DEFINE();
   

    /* ODUKSC */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_retrieve(mapotn_handle->oduksc_handle,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, oduksc),
                                     &int_table_ptr->oduksc);
    }

    UTIL_GLOBAL_INT_BUFFER_FLUSH(tgmp_handle, mapotn_rx);        
    UTIL_GLOBAL_INT_BUFFER_FLUSH(tgmp_handle, mapotn_tx);        
    UTIL_GLOBAL_INT_BUFFER_FLUSH(tgmp_handle, mapotn_rgfpf);        

    PMC_RETURN(result);
} /* mapotn_int_retrieve */




/*******************************************************************************
* mapotn_int_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
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
PUBLIC PMC_ERROR mapotn_int_enabled_check(mapotn_handle_t *mapotn_handle,
                                          mapotn_int_t *int_en_table_ptr,
                                          BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();


    #ifdef MAPOTN_INT
    #undef MAPOTN_INT
    #endif
    #define MAPOTN_INT UTIL_GLOBAL_INT_CHECK_SINGLE

    MAPOTN_INT_TABLE_DEFINE();
    
    /* ODUKSC */
    if (PMC_SUCCESS == result &&
        (FALSE == *int_found_ptr))
    {
        result = oduksc_int_enabled_check(mapotn_handle->oduksc_handle,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, oduksc),
                                          int_found_ptr);
    }

    PMC_RETURN(result);
} /* mapotn_int_enabled_check */

/*******************************************************************************
* mapotn_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
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
PUBLIC PMC_ERROR mapotn_int_chnl_enable(mapotn_handle_t *mapotn_handle,
                                        UINT32 chnl_id,
                                        mapotn_int_chnl_t *int_table_ptr,
                                        mapotn_int_chnl_t *int_en_table_ptr,
                                        BOOL8 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = mapotn_int_validate(mapotn_handle, chnl_id, int_en_table_ptr);

    #ifdef MAPOTN_INT_CHNL
    #undef MAPOTN_INT_CHNL
    #endif
    #define MAPOTN_INT_CHNL UTIL_GLOBAL_INT_ENABLE_RANGE

    #ifdef MAPOTN_INT_CHNL_ARRAY
    #undef MAPOTN_INT_CHNL_ARRAY
    #endif
    #define MAPOTN_INT_CHNL_ARRAY UTIL_GLOBAL_INT_ENABLE_ARRAY_RANGE


    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        MAPOTN_INT_CHNL_TABLE_DEFINE();

        if (FALSE == pmc_is_digi_rev_a_revision(&mapotn_handle->base))
        {
            MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE();
        }
        int_en_table_ptr->tgfpf_valid = TRUE;
        int_en_table_ptr->rgfpf_valid = TRUE;
    }
    else
    {
        int_en_table_ptr->tgfpf_valid = FALSE;
        int_en_table_ptr->rgfpf_valid = FALSE;
    }


    if ((PMC_SUCCESS == result)  &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)) &&
         (NULL != int_table_ptr) &&
        (TRUE == int_table_ptr->rgmp_valid))
    {
        int_en_table_ptr->rgmp_valid = TRUE;

        result = rgmp_int_chnl_enable(mapotn_handle->rgmp_handle,
                                      chnl_id,
                                      &int_table_ptr->rgmp,
                                      &int_en_table_ptr->rgmp,
                                      enable);
    }


    /* TGMP channelized interrupts */
    if ((PMC_SUCCESS == result) &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)))
    {
        result = tgmp_int_chnl_enable(mapotn_handle->tgmp_handle,
                                      chnl_id,
                                      &int_table_ptr->tgmp,
                                      &int_en_table_ptr->tgmp,
                                      enable);
    } 

    /* ODUKSC channelized interrupts */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_enable(mapotn_handle->oduksc_handle,
                                        chnl_id,
                                        &int_table_ptr->oduksc,
                                        &int_en_table_ptr->oduksc,
                                        enable);
    }

    /* OHFS_INSERT channelized interrupts */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        result = ohfs_insert_int_chnl_enable(mapotn_handle->ohfs_insert_handle,
                                             chnl_id,
                                             &int_table_ptr->ohfs_insert,
                                             &int_en_table_ptr->ohfs_insert,
                                             enable);
    }

    if (PMC_SUCCESS == result &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)))
    {
        int_en_table_ptr->mpmo_valid = TRUE;
        result = mpmo_int_chnl_enable(mapotn_handle->mpmo_handle,
                                      chnl_id,
                                      &int_table_ptr->mpmo,
                                      &int_en_table_ptr->mpmo,
                                      enable);
    }
    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_enable(mapotn_handle->mpma_handle,
                                      chnl_id,
                                      &int_table_ptr->mpma,
                                      &int_en_table_ptr->mpma,
                                      enable);
    }

    /* CFC_UNPACK */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        result = cfc_int_chnl_enable(mapotn_handle->cfc_unpack_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc_unpack,
                                     &int_en_table_ptr->cfc_unpack,
                                     enable);
    }
    
    /* CFC_EGRESS */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        result = cfc_int_chnl_enable(mapotn_handle->cfc_egress_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc_egress,
                                     &int_en_table_ptr->cfc_egress,
                                     enable);
    }
    
    /* CFC_PACKING */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        result = cfc_int_chnl_enable(mapotn_handle->cfc_packing_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc_packing,
                                     &int_en_table_ptr->cfc_packing,
                                     enable);
    }
    
    /* CFC_PREFETCH */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        result = cfc_int_chnl_enable(mapotn_handle->cfc_prefetch_handle,
                                     chnl_id,
                                     &int_table_ptr->cfc_prefetch,
                                     &int_en_table_ptr->cfc_prefetch,
                                     enable);
    }

    PMC_RETURN(result);
} /* mapotn_int_chnl_enable */

/*******************************************************************************
* mapotn_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
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
PUBLIC PMC_ERROR mapotn_int_chnl_clear(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl_id,
                                       mapotn_int_chnl_t *int_table_ptr,
                                       mapotn_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = mapotn_int_validate(mapotn_handle, chnl_id, int_en_table_ptr);

    #ifdef MAPOTN_INT_CHNL
    #undef MAPOTN_INT_CHNL
    #endif
    #define MAPOTN_INT_CHNL UTIL_GLOBAL_INT_CLEAR_RANGE

    #ifdef MAPOTN_INT_CHNL_ARRAY
    #undef MAPOTN_INT_CHNL_ARRAY
    #endif
    #define MAPOTN_INT_CHNL_ARRAY UTIL_GLOBAL_INT_CLEAR_ARRAY_RANGE


    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        MAPOTN_INT_CHNL_TABLE_DEFINE();

        if (FALSE == pmc_is_digi_rev_a_revision(&mapotn_handle->base))
        {
            MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE();
        }
    }
    
    
    if ((PMC_SUCCESS == result)  &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)) &&
        (NULL != int_table_ptr) &&
        (TRUE == int_table_ptr->rgmp_valid))
    {
        result = rgmp_int_chnl_clear(mapotn_handle->rgmp_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, rgmp));
    }

    /* TGMP channelized interrupts */
    if ((PMC_SUCCESS == result)  &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)) &&
        (NULL != int_table_ptr) &&
        (TRUE == int_table_ptr->tgmp_valid))
    {
        result = tgmp_int_chnl_clear(mapotn_handle->tgmp_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, tgmp),
                                     &int_en_table_ptr->tgmp);
    }

    /* ODUKSC channelized interrupts */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_clear(mapotn_handle->oduksc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, oduksc),
                                       &int_en_table_ptr->oduksc);
    }

    /* OHFS_INSERT channelized interrupts */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        result = ohfs_insert_int_chnl_clear(mapotn_handle->ohfs_insert_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, ohfs_insert),
                                            &int_en_table_ptr->ohfs_insert);
    }

    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_clear(mapotn_handle->mpma_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mpma),
                                     &int_en_table_ptr->mpma);
    }
    if (PMC_SUCCESS == result &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)))
    {
        result = mpmo_int_chnl_clear(mapotn_handle->mpmo_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mpmo),
                                     &int_en_table_ptr->mpmo);
    }

    /* CFC_UNPACK */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(mapotn_handle->cfc_unpack_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc_unpack),
                                     &int_en_table_ptr->cfc_unpack);
    }
    
    /* CFC_EGRESS */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(mapotn_handle->cfc_egress_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc_egress),
                                     &int_en_table_ptr->cfc_egress);
    }
    
    /* CFC_PACKING */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(mapotn_handle->cfc_packing_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc_packing),
                                     &int_en_table_ptr->cfc_packing);
    }
    
    /* CFC_PREFETCH */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(mapotn_handle->cfc_prefetch_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc_prefetch),
                                     &int_en_table_ptr->cfc_prefetch);
    }


    PMC_RETURN(result);
} /* mapotn_int_chnl_clear */



/*******************************************************************************
* mapotn_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
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
PUBLIC PMC_ERROR mapotn_int_chnl_retrieve(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl_id,
                                          mapotn_int_chnl_t *filt_table_ptr,
                                          mapotn_int_chnl_t *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = mapotn_int_validate(mapotn_handle, chnl_id, int_table_ptr);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mapotn_handle, mapotn_tgfpf, UTIL_GLOBAL_INT_NO_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(mapotn_handle, mapotn_rgfpf, UTIL_GLOBAL_INT_NO_OPTIMIZE);
    
    #ifdef MAPOTN_INT_CHNL
    #undef MAPOTN_INT_CHNL
    #endif

    #define MAPOTN_INT_CHNL UTIL_GLOBAL_INT_RETRIEVE_RANGE

    #ifdef MAPOTN_INT_CHNL_ARRAY
    #undef MAPOTN_INT_CHNL_ARRAY
    #endif
    #define MAPOTN_INT_CHNL_ARRAY UTIL_GLOBAL_INT_RETRIEVE_ARRAY_RANGE

    #ifdef MAPOTN_STATUS_CHNL
    #undef MAPOTN_STATUS_CHNL
    #endif
    #define MAPOTN_STATUS_CHNL UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    
    int_table_ptr->tgfpf_valid = FALSE;
    int_table_ptr->rgfpf_valid = FALSE;

    if (PMC_SUCCESS == result &&
        UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode)
    {
        MAPOTN_INT_CHNL_TABLE_DEFINE();

        if (FALSE == pmc_is_digi_rev_a_revision(&mapotn_handle->base))
        {
            MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE();
        }
        int_table_ptr->tgfpf_valid = TRUE;
        int_table_ptr->rgfpf_valid = TRUE;

        MAPOTN_STATUS_CHNL_TABLE_DEFINE();

        if (FALSE == pmc_is_digi_rev_a_revision(&mapotn_handle->base))
        {
            MAPOTN_STATUS_CHNL_TABLE_REV_B_DEFINE();
        }
        result = ohfs_insert_int_chnl_retrieve(mapotn_handle->ohfs_insert_handle,
                                               chnl_id,
                                               UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, ohfs_insert),
                                               &int_table_ptr->ohfs_insert);
    }
    UTIL_GLOBAL_INT_BUFFER_FLUSH(mapotn_handle, mapotn_tgfpf);        
    UTIL_GLOBAL_INT_BUFFER_FLUSH(mapotn_handle, mapotn_rgfpf);        

    int_table_ptr->rgmp_valid = FALSE;
    int_table_ptr->tgmp_valid = FALSE;
    int_table_ptr->mpmo_valid = FALSE;

    if (PMC_SUCCESS == result &&
        MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id))
    {
        result = rgmp_int_chnl_retrieve(mapotn_handle->rgmp_handle,
                                        chnl_id,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, rgmp),
                                        &int_table_ptr->rgmp);

        if (PMC_SUCCESS == result)
        {
            int_table_ptr->rgmp_valid = TRUE;
            result = tgmp_int_chnl_retrieve(mapotn_handle->tgmp_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, tgmp),
                                            &int_table_ptr->tgmp);
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->tgmp_valid = TRUE;
            }
        }

        if (PMC_SUCCESS == result)
        {
            int_table_ptr->mpmo_valid = TRUE;
            result = mpmo_int_chnl_retrieve(mapotn_handle->mpmo_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, mpmo),
                                            &int_table_ptr->mpmo);
        }
    }

    /* ODUKSC channelized interrupts */
    if (PMC_SUCCESS == result)
    {
        result = oduksc_int_chnl_retrieve(mapotn_handle->oduksc_handle,
                                          chnl_id,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, oduksc),
                                          &int_table_ptr->oduksc);
    }

    if (PMC_SUCCESS == result)
    {
        result = mpma_int_chnl_retrieve(mapotn_handle->mpma_handle,
                                        chnl_id,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, mpma),
                                        &int_table_ptr->mpma);
    }

    /* CFC_UNPACK */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(mapotn_handle->cfc_unpack_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc_unpack),
                                       &int_table_ptr->cfc_unpack);
    }
    
    /* CFC_EGRESS */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(mapotn_handle->cfc_egress_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc_egress),
                                       &int_table_ptr->cfc_egress);
    }
    
    /* CFC_PACKING */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(mapotn_handle->cfc_packing_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc_packing),
                                       &int_table_ptr->cfc_packing);
    }
    
    /* CFC_PREFETCH */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(mapotn_handle->cfc_prefetch_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc_prefetch),
                                       &int_table_ptr->cfc_prefetch);
    }

    PMC_RETURN(result);
} /* mapotn_int_chnl_retrieve */




/*******************************************************************************
* mapotn_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl_id           - Channel ID: Valid range: 0 - 95
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
PUBLIC PMC_ERROR mapotn_int_chnl_enabled_check(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl_id,
                                               mapotn_int_chnl_t *int_en_table_ptr,
                                               BOOL8 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = mapotn_int_validate(mapotn_handle, chnl_id, int_found_ptr);


    #ifdef MAPOTN_INT_CHNL
    #undef MAPOTN_INT_CHNL
    #endif
    #define MAPOTN_INT_CHNL UTIL_GLOBAL_INT_CHECK_RANGE

    #ifdef MAPOTN_INT_CHNL_ARRAY
    #undef MAPOTN_INT_CHNL_ARRAY
    #endif
    #define MAPOTN_INT_CHNL_ARRAY UTIL_GLOBAL_INT_CHECK_ARRAY_RANGE


    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode))
    {
        MAPOTN_INT_CHNL_TABLE_DEFINE();

        if (FALSE == pmc_is_digi_rev_a_revision(&mapotn_handle->base))
        {
            MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE();
        }
    }


    if ((PMC_SUCCESS == result)  &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)) &&
        (FALSE == *int_found_ptr) &&
        (TRUE == int_en_table_ptr->rgmp_valid))
    {
        result = rgmp_int_chnl_enabled_check(mapotn_handle->rgmp_handle,
                                             chnl_id,
                                             &int_en_table_ptr->rgmp,
                                             int_found_ptr);
                                       
    }

    /* TGMP channelized interrupts */
    if (PMC_SUCCESS == result  &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)) &&
        (TRUE == int_en_table_ptr->tgmp_valid) &&
        (FALSE == *int_found_ptr))
    {
        result = tgmp_int_chnl_enabled_check(mapotn_handle->tgmp_handle,
                                             chnl_id,
                                             &int_en_table_ptr->tgmp,
                                             int_found_ptr);
    }

    /* ODUKSC channelized interrupts */
    if (PMC_SUCCESS == result &&
        (FALSE == *int_found_ptr))
    {
        result = oduksc_int_chnl_enabled_check(mapotn_handle->oduksc_handle,
                                               chnl_id,
                                               &int_en_table_ptr->oduksc,
                                               int_found_ptr);
    }

    /* OHFS_INSERT channelized interrupts */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) &&
        (TRUE == int_en_table_ptr->tgfpf_valid) &&
        (FALSE == *int_found_ptr))
    {
        result = ohfs_insert_int_chnl_enabled_check(mapotn_handle->ohfs_insert_handle,
                                                    chnl_id,
                                                    &int_en_table_ptr->ohfs_insert,
                                                    int_found_ptr);
    }

    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr))
    {
        result = mpma_int_chnl_enabled_check(mapotn_handle->mpma_handle,
                                             chnl_id,
                                             &int_en_table_ptr->mpma,
                                             int_found_ptr);
    }
    if ((PMC_SUCCESS == result) &&
        (FALSE == *int_found_ptr)  &&
        (TRUE == int_en_table_ptr->mpmo_valid) &&
        (MAPOTN_MAPPING_MODE_IS_GMP(mapotn_handle, chnl_id)))
    {
        result = mpmo_int_chnl_enabled_check(mapotn_handle->mpmo_handle,
                                             chnl_id,
                                             &int_en_table_ptr->mpmo,
                                             int_found_ptr);
    }

    /* CFC_UNPACK */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) &&
        (TRUE == int_en_table_ptr->rgfpf_valid) &&
        (FALSE == *int_found_ptr))
    {
        result = cfc_int_chnl_enabled_check(mapotn_handle->cfc_unpack_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc_unpack,
                                            int_found_ptr);
    }
    
    /* CFC_EGRESS */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) &&
        (TRUE == int_en_table_ptr->rgfpf_valid) &&
        (FALSE == *int_found_ptr))
    {
        result = cfc_int_chnl_enabled_check(mapotn_handle->cfc_egress_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc_egress,
                                            int_found_ptr);
    }
    
    /* CFC_PACKING */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) &&
        (TRUE == int_en_table_ptr->tgfpf_valid) &&
        (FALSE == *int_found_ptr))
    {
        result = cfc_int_chnl_enabled_check(mapotn_handle->cfc_packing_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc_egress,
                                             int_found_ptr);
    }
    
    /* CFC_PREFETCH */
    if (PMC_SUCCESS == result &&
        (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl_id].mapping_mode) &&
        (TRUE == int_en_table_ptr->tgfpf_valid) &&
        (FALSE == *int_found_ptr))
    {
        result = cfc_int_chnl_enabled_check(mapotn_handle->cfc_prefetch_handle,
                                            chnl_id,
                                            &int_en_table_ptr->cfc_prefetch,
                                            int_found_ptr);
    }

    PMC_RETURN(result);
} /* mapotn_int_chnl_enabled_check */


/*
** Private Functions
*/

/*******************************************************************************
* mapotn_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover a mapotn_recover_var_t context and energy state from register space.
*
*  This function is used by mapotn_handle_restart_init. It also recovers 
*  volatile information from mapotn_var_t context.
*  
*  This function return an error when something is incoherent in the register 
*  space. In such case, the only solution for crash restart is to reset the 
*  subsystem
* 
* INPUTS:
*   *mapotn_handle            - pointer to MAPOTN handle instance
*   top_energy_state_reg      - energy state from top-level
*   
*
* OUTPUTS:
*   *mapotn_var_reg          - A partial mapotn_var_t context + additional
*                              information to facilite audit phase
*   *mapotn_energy_state_reg - MAPOTN energy states 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.\n
*    PMC_SUCCESS: recovery is a success \n
*    MAPOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT: 
*    Communal ressource energy states are incoherent. top will have
*    to shutdown the subsystem\n
*    MAPOTN_ERR_CONTEXT_REG_INCOHERENT:
*    Register conetxt is incoherent. top will have to shutdown the subsystem\n
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_ctxt_reg_recover(mapotn_handle_t *mapotn_handle,
                                          pmc_energy_state_t top_energy_state_reg,                                         
                                          mapotn_recover_var_t *mapotn_var_reg,                                         
                                          mapotn_energy_state_t *mapotn_energy_state_reg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 dirty_chnl;
    UINT32 chnl_itr;
    scbs3_handle_t *mpma_scbs3_handle, *oduksc_scbs3_handle;    
    BOOL8 rx_gfp_actv_chan[MAPOTN_NUM_CHANL];
    BOOL8 rx_gmp_actv_chan[MAPOTN_NUM_CHANL];
    BOOL8 tx_gfp_actv_chan[MAPOTN_NUM_CHANL];
    BOOL8 tx_gmp_actv_chan[MAPOTN_NUM_CHANL];

    PMC_ENTRY();


    /* GET SCBS3 handle */
    mpma_scbs3_handle   = mpma_scbs3_handle_get(mapotn_handle->mpma_handle);
    oduksc_scbs3_handle = oduksc_scbs3_handle_get(mapotn_handle->oduksc_handle);
    
    /* 
       create a register context 
    */
    /* start from a clean context */
    mapotn_var_default_init(&mapotn_var_reg->var); 
    
    /* this information is coming from top-level register */
    mapotn_var_reg->var.mapotn_start_state = (top_energy_state_reg == PMC_ENERGY_STATE_RESET);
    
    /* 
       IF start_state is TRUE, the susbystem is completly in power down
       mapotn_register_var is completed (anyway, digi-layer should not call use with PMC_ENERGY_STATE_RESET)
    */
    if (FALSE == mapotn_var_reg->var.mapotn_start_state)
    {
        /* REGISTER:: MAPOTN SS is not in power down */
        
        /* get energy state from mapotn-top register */
        mapotn_energy_state_reg_get(mapotn_handle, 
                                    mapotn_energy_state_reg);
                
        /* 
           check power state from communal blocks
           TOP/RX, TOP/TX, MPMA and ODUKSC               
           they should all be in RUNNING state when digi-level soft reset has been
           removed
        */
        if (!(PMC_ENERGY_STATE_RUNNING == mapotn_energy_state_reg->rx_top &&
              PMC_ENERGY_STATE_RUNNING == mapotn_energy_state_reg->mpma   && 
              PMC_ENERGY_STATE_RUNNING == mapotn_energy_state_reg->tx_top &&
              PMC_ENERGY_STATE_RUNNING == mapotn_energy_state_reg->oduksc))
        {
            rc = MAPOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;
        }
        
        if (PMC_SUCCESS == rc)
        {
            UINT32 mpma_scbs3_user_bits;
            /* 
               At this point, we can recover channel-based information
               
               a COHERENT CHANNEL is in OPERATIONAL state
               a [possibly] INCOHERENT CHANNEL could be anything from START to OPERATIONAL 
               
            */
            
            /* determine which data paths are enabled on which side */            
            if (FALSE ==  MAPOTN_DUAL_OHFS_REMOVE_MODE)
            {
                mapotn_var_reg->has_rx_gfp_chnl = (mapotn_energy_state_reg->rgfpf96 == PMC_ENERGY_STATE_RUNNING);
            } else
            {
                mapotn_var_reg->has_rx_gfp_chnl = (mapotn_energy_state_reg->rgfpf96 == PMC_ENERGY_STATE_RUNNING) && 
                    (mapotn_energy_state_reg->rgmp96 == PMC_ENERGY_STATE_RUNNING);
            }
            mapotn_var_reg->has_rx_gmp_chnl = (mapotn_energy_state_reg->rgmp96 == PMC_ENERGY_STATE_RUNNING);
            mapotn_var_reg->has_tx_gfp_chnl = (mapotn_energy_state_reg->tgfpf96 == PMC_ENERGY_STATE_RUNNING);
            mapotn_var_reg->has_tx_gmp_chnl = (mapotn_energy_state_reg->tgmp96 == PMC_ENERGY_STATE_RUNNING) &&
                (mapotn_energy_state_reg->mpmo == PMC_ENERGY_STATE_RUNNING);

            
            /* when communal ressources are enabled, at least 1 channel based ressources shall be enabled */
            if (FALSE == mapotn_var_reg->has_rx_gfp_chnl && FALSE == mapotn_var_reg->has_tx_gfp_chnl &&
                FALSE == mapotn_var_reg->has_rx_gmp_chnl && FALSE == mapotn_var_reg->has_tx_gmp_chnl)
            {
                rc = MAPOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;
            } else 
            {
                /* 
                   get channel information 
                   - channel reset
                   - calendar entries
                   - user bits
                */
                PMC_MEMSET(&rx_gfp_actv_chan[0],0,sizeof(BOOL8)*MAPOTN_NUM_CHANL);
                PMC_MEMSET(&tx_gfp_actv_chan[0],0,sizeof(BOOL8)*MAPOTN_NUM_CHANL);
                PMC_MEMSET(&rx_gmp_actv_chan[0],0,sizeof(BOOL8)*MAPOTN_NUM_CHANL);
                PMC_MEMSET(&tx_gmp_actv_chan[0],0,sizeof(BOOL8)*MAPOTN_NUM_CHANL);
                
                if (TRUE == mapotn_var_reg->has_rx_gfp_chnl)
                {
                    mapotn_rgfpf_activated_chnl_get(mapotn_handle,&rx_gfp_actv_chan[0]);
                }
                if (TRUE == mapotn_var_reg->has_rx_gmp_chnl)
                {
                    rgmp_activated_chnl_get(mapotn_handle->rgmp_handle,&rx_gmp_actv_chan[0]);
                }
                if (TRUE == mapotn_var_reg->has_rx_gfp_chnl || TRUE == mapotn_var_reg->has_rx_gmp_chnl)
                {
                    scbs3_page_get(mpma_scbs3_handle,SCBS3_ACTIVE_MEM, &mapotn_var_reg->mpma_scbs3_page);
                }
                if (TRUE == mapotn_var_reg->has_tx_gfp_chnl)
                {                        
                    mapotn_tgfpf_activated_chnl_get(mapotn_handle,&tx_gfp_actv_chan[0]);  
                }
                if (TRUE == mapotn_var_reg->has_tx_gmp_chnl)
                {                        
                    tgmp_activated_chnl_get(mapotn_handle->tgmp_handle,&tx_gmp_actv_chan[0]);
                }
                if (TRUE == mapotn_var_reg->has_tx_gfp_chnl || TRUE == mapotn_var_reg->has_tx_gmp_chnl)
                {
                    scbs3_page_get(oduksc_scbs3_handle,SCBS3_ACTIVE_MEM, &mapotn_var_reg->oduksc_scbs3_page);
                }                                        
            }
            
            /* populate channel context */
            for (chnl_itr = 0; chnl_itr  < MAPOTN_NUM_CHANL && PMC_SUCCESS == rc; chnl_itr++)
            {                    
                /* check if you have incoherent register context */
                if ( ((TRUE == rx_gfp_actv_chan[chnl_itr]) &&  (mapotn_var_reg->has_rx_gfp_chnl == FALSE)) ||
                     ((TRUE == tx_gfp_actv_chan[chnl_itr]) &&  (mapotn_var_reg->has_tx_gfp_chnl == FALSE)) ||
                     ((TRUE == rx_gmp_actv_chan[chnl_itr]) &&  (mapotn_var_reg->has_rx_gmp_chnl == FALSE)) ||
                     ((TRUE == tx_gmp_actv_chan[chnl_itr]) &&  (mapotn_var_reg->has_tx_gmp_chnl == FALSE)))
                {                     
                    rc = MAPOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT;
                } else 
                {
                    
                    dirty_chnl = FALSE;
                    if (TRUE == rx_gfp_actv_chan[chnl_itr] || TRUE == rx_gmp_actv_chan[chnl_itr])
                    {
                        mpma_scbs3_user_bits = scbs3_userbits_per_chnl_get(mpma_scbs3_handle,
                                                                           &mapotn_var_reg->mpma_scbs3_page,
                                                                           chnl_itr);
                        mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].state            = MAPOTN_CHNL_OPERATIONAL;
                        mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].num_cal_entries = scbs3_num_entries_per_chnl_get(mpma_scbs3_handle,
                                                                                                                    &mapotn_var_reg->mpma_scbs3_page,
                                                                                                                    chnl_itr); 
                        if (0 == mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].num_cal_entries)
                        {
                            dirty_chnl = TRUE;
                        }
                        /* restore mapping mode */
                        if ((mpma_scbs3_user_bits & 0x4) == 0x4)
                        {
                            mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].mapping_mode     = UTIL_GLOBAL_GFP;
                        } else 
                        {
                            /* OHFS_REMOVE "trib slot" cfg (in this case, it is not really trib slot since we in an OPU container, 
                               but it is the same concept */                                
                            UINT32 first_entry = scbs3_first_calendar_entry_per_chnl_get(mpma_scbs3_handle, &mapotn_var_reg->mpma_scbs3_page,chnl_itr);                                
                            
                            if (first_entry < 96)
                            {
                                /* get mapping from RGMP */
                                mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].mapping_mode     = rgmp_mapping_mode_get(mapotn_handle->rgmp_handle,                                                                                                                
                                                                                                                    first_entry);
                            } else 
                            {
                                dirty_chnl = TRUE;
                            }
                        }
                        /* the ODUKP type may not be completed correct if we havea flex rate. For example, for a flex rate that requires  8 cal entries, this function return ODU2.
                           however, that can be easily resolved. */                               
                        mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].odukp_type = 
                            util_global_odukp_type_per_num_entries_get(mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].num_cal_entries,
                                                                       mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].mapping_mode);
                        
                        /* restore RX datapath destination */
                        if ((mpma_scbs3_user_bits & 0x1) == 0x1 && (mpma_scbs3_user_bits & 0x2) == 0x2) 
                        {
                            mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].src_dest         = MAPOTN_DEST_CPB_ENET;
                        } else if ((mpma_scbs3_user_bits & 0x1) == 0x1)
                        {
                            mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].src_dest         = MAPOTN_DEST_ENET;
                        } else if ((mpma_scbs3_user_bits & 0x2) == 0x2)
                        {
                            mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].src_dest         = MAPOTN_DEST_CPB;
                        } else 
                        {
                            /* bad channel */
                            mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].src_dest = LAST_MAPOTN_DEST;
                            dirty_chnl = TRUE;
                        }  
                        

                        if (PMC_SUCCESS == rc && dirty_chnl == FALSE)
                        {
                            rc = mapotn_recover_adaptation_func(mapotn_handle,
                                                                MAPOTN_RX,
                                                                chnl_itr,
                                                                mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].mapping_mode,
                                                                mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].num_cal_entries,
                                                                &mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].src_dest,
                                                                &mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].adaptation_func,
                                                                &mapotn_var_reg->rx_fuzzy_flag[chnl_itr]);
                        }
                        /* the client is not recover per channel*/
                        mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].client_rate      = 0;                        
                    }
                    dirty_chnl = FALSE;
                    /* do TX side ... */
                    if (PMC_SUCCESS == rc && (TRUE == tx_gfp_actv_chan[chnl_itr] || TRUE == tx_gmp_actv_chan[chnl_itr]))
                    {
                        mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].state            = MAPOTN_CHNL_OPERATIONAL;
                        mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].num_cal_entries  = scbs3_num_entries_per_chnl_get(oduksc_scbs3_handle,
                                                                                                                     &mapotn_var_reg->oduksc_scbs3_page,
                                                                                                                     chnl_itr);  
                        if (0 == mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].num_cal_entries)
                        {
                            dirty_chnl = TRUE;
                        }
                        if (TRUE == tx_gfp_actv_chan[chnl_itr])
                        {
                            mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].mapping_mode     = UTIL_GLOBAL_GFP;
                        } else 
                        {
                            UINT32 first_entry = scbs3_first_calendar_entry_per_chnl_get(oduksc_scbs3_handle, &mapotn_var_reg->oduksc_scbs3_page,chnl_itr);      
                            if (first_entry < 96)
                            {
                                mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].mapping_mode     = tgmp_mapping_mode_get(mapotn_handle->tgmp_handle,                                                                                                                
                                                                                                                    first_entry);
                            } else 
                            {                         
                                dirty_chnl = TRUE;
                            }
                        }
                        
                        /* the ODUKP type may not be completed correct if we have flex rate. For example, 
                           for a flex rate that requires  8 cal entries, this function return ODU2.
                           however, that can be easily resolved. */                               
                        mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].odukp_type = 
                            util_global_odukp_type_per_num_entries_get(mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].num_cal_entries,
                                                                       mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].mapping_mode);
                        
                        if (UTIL_GLOBAL_GFP == mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].mapping_mode)
                        {
                            mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].src_dest = mapotn_tgfpf_chnl_mapping_get(mapotn_handle,chnl_itr);                                                                
                        } else 
                        {
                            if (oduksc_zone_pushback_cfg_get(mapotn_handle->oduksc_handle, chnl_itr) == 1)
                            {
                                /* it is BMP/CPB */
                                mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].src_dest  =  MAPOTN_DEST_CPB;  
                            } else 
                            {
                                mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].src_dest  = 
                                    mpmo_ch_ssf_cn_center_get(mapotn_handle->mpmo_handle, chnl_itr)==1?MAPOTN_DEST_ENET:MAPOTN_DEST_CPB;
                            }
                        }


                        if (PMC_SUCCESS == rc && dirty_chnl == FALSE)
                        {
                            rc = mapotn_recover_adaptation_func(mapotn_handle,
                                                                MAPOTN_TX,
                                                                chnl_itr,
                                                                mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].mapping_mode,
                                                                mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].num_cal_entries,
                                                                &mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].src_dest,
                                                                &mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].adaptation_func,
                                                                &mapotn_var_reg->tx_fuzzy_flag[chnl_itr]);
                        }
                       /* the client is not recover per channel*/
                        mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].client_rate = 0;
                    }
                }                
            }                            
        }                
    }
        
    PMC_RETURN(rc);
} /* mapotn_ctxt_reg_recover */

/*******************************************************************************
* mapotn_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Audit memory context with recovered register context. This function 
*   determines if a cleanup is required
*   
*   A clean context which represents the intersection between both context is also 
*   generated.
*
* INPUTS:
*   *mapotn_handle           - pointer to MAPOTN handle instance
*   *restart_init_cfg_ptr    - pointer to restart init config
*   *mapotn_var_reg          - A partial mapotn_var_t context + additional
*                              information to facilite audit phase
*   *mapotn_energy_state_reg - MAPOTN energy states 
*
* OUTPUTS:
*   *mapotn_var_clean        - A clean context which represents the intersection
*                              between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*    PMC_SUCCESS: Memory & register context matches\n
*    MAPOTN_ERR_CONTEXT_CLEANUP: A cleanup is required.\n
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_ctxt_audit(mapotn_handle_t *mapotn_handle,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                    mapotn_recover_var_t *mapotn_var_reg,                                         
                                    mapotn_energy_state_t *mapotn_energy_state_reg,
                                    mapotn_var_t *mapotn_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 num_cal_entries;
    UINT32 chnl_itr, cal_itr;
    UINT32 cal_entries[96];
    BOOL8 need_cleanup = FALSE;
    BOOL8 accept_chnl = FALSE;
    BOOL8 rx_check_ctxt_sync = FALSE;
    BOOL8 tx_check_ctxt_sync = FALSE;
    BOOL8 tx_equipped, rx_equipped;
    PMC_ENTRY();
    
    /* init the new context  */
    mapotn_var_default_init(mapotn_var_clean);
    PMC_MEMSET(&cal_entries[0],0,sizeof(UINT32) * 96);
    /* AUDIT POWER */
    
    /* 
       first, if we are here, it means that top-level register says that we are out-of-reset 
    */
    if (mapotn_var_reg->var.mapotn_start_state != mapotn_start_state_test(mapotn_handle))
    {       
        mapotn_var_clean->mapotn_start_state = TRUE;
        need_cleanup = TRUE;
    } else if (mapotn_var_reg->var.mapotn_start_state == FALSE) 
    {
        /* we are out-of-reset, now we check communal ressources */
        if (mapotn_energy_state_reg->mpma    == PMC_ENERGY_STATE_RUNNING && mpma_start_state_test(mapotn_handle->mpma_handle) == FALSE &&
            mapotn_energy_state_reg->oduksc  == PMC_ENERGY_STATE_RUNNING && oduksc_start_state_test(mapotn_handle->oduksc_handle) == FALSE &&
            mapotn_energy_state_reg->rx_top  == PMC_ENERGY_STATE_RUNNING && mapotn_energy_state_reg->tx_top  == PMC_ENERGY_STATE_RUNNING )
        {
            mapotn_var_clean->mapotn_start_state = mapotn_var_reg->var.mapotn_start_state;
        } else 
        {
            mapotn_var_clean->mapotn_start_state = TRUE;
            need_cleanup = TRUE;
        }
        
        /* Channel based energy audit
           
           the following functions can be used to have an overview of the power configuration                              
            - mapotn_rgfpf_start_state_test(mapotn_handle)
            - mapotn_tgfpf_start_state_test(mapotn_handle)
            - rgmp_start_state_test(mapotn_handle->rgmp_handle)
            - tgmp_start_state_test(mapotn_handle->tgmp_handle)
            - mpmo_start_state_test(mapotn_handle->mpmo_handle)
           These functions are actually based on a subset of the memoery context. We are doing something equivalent in the next step
        */

        /* Channel context audit */
        for (chnl_itr = 0; chnl_itr  < MAPOTN_NUM_CHANL; chnl_itr++)
        {
            accept_chnl = TRUE;
            rx_equipped = FALSE;
            tx_equipped = FALSE;
            rx_check_ctxt_sync = FALSE;
            tx_check_ctxt_sync = FALSE;

            /* 
               We only consider channel that are both in OPERATIONAL mode
               then we accept or reject the memory context based on information collected from 
               register space
            */
            
            /* AUDIT RX channels */
            if (mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].state != mapotn_handle->var.rx_chnl_ctxt[chnl_itr].state)
            {
                if (MAPOTN_CHNL_EQUIPPED == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].state)
                {
                    rx_equipped = TRUE;
                } else 
                {
                    rx_check_ctxt_sync = TRUE;
                }
            }  else  if (mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].state == MAPOTN_CHNL_OPERATIONAL && 
                         mapotn_handle->var.rx_chnl_ctxt[chnl_itr].state  == MAPOTN_CHNL_OPERATIONAL) 
            {       
                /* resolve channels that have been flagged as fuzzy */
                if (TRUE == mapotn_var_reg->rx_fuzzy_flag[chnl_itr])
                {
                    mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].adaptation_func = 
                        mapotn_resolve_fuzzy_adaptation_func(mapotn_handle,   
                                                             mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].adaptation_func,
                                                             mapotn_handle->var.rx_chnl_ctxt[chnl_itr].adaptation_func);
                }
                
                /* Check num entries, destination,  mapping mode and adaptation layer */
                if (mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].num_cal_entries  == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].num_cal_entries &&
                    mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].src_dest         == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].src_dest &&
                    mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].mapping_mode     == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].mapping_mode && 
                    (mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].adaptation_func  == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].adaptation_func))                
                {                 
                    /* 
                       check that this channel config and the db config are in sync 
                       ENTRIES shall be in sync
                     */
                    (void) mapotn_db_chnl_num_entries_get(mapotn_handle,chnl_itr,UTIL_GEN_DB_MPMA_DB,&num_cal_entries);

                    if ( num_cal_entries == mapotn_var_reg->var.rx_chnl_ctxt[chnl_itr].num_cal_entries) 
                    {                     
                        (void) mapotn_db_chnl_entry_get(mapotn_handle, UTIL_GEN_DB_MPMA_DB,chnl_itr,num_cal_entries,&cal_entries[0]);
                        for (cal_itr = 0;cal_itr < num_cal_entries;cal_itr++)
                        {                        
                            if (chnl_itr != mapotn_var_reg->mpma_scbs3_page.calendar[cal_entries[cal_itr]])
                            {
                                need_cleanup = TRUE;
                                accept_chnl = FALSE;
                            }
                        }
                        if (accept_chnl == TRUE)
                        {
                            /* this channel is accepted */               
                            PMC_MEMCPY(&mapotn_var_clean->rx_chnl_ctxt[chnl_itr], &mapotn_handle->var.rx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t));                
                        }
                    } else 
                    {
                        need_cleanup = TRUE;
                    }
                } else 
                {
                    need_cleanup = TRUE;
                }
            } 

            accept_chnl = TRUE;
            /* AUDIT TX Channels */
            if (mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].state != mapotn_handle->var.tx_chnl_ctxt[chnl_itr].state)
            {
                if (MAPOTN_CHNL_EQUIPPED == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].state)
                {
                    tx_equipped = TRUE;
                } else 
                {
                    tx_check_ctxt_sync = TRUE;
                }
            } else if (mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].state == MAPOTN_CHNL_OPERATIONAL && 
                       mapotn_handle->var.tx_chnl_ctxt[chnl_itr].state  == MAPOTN_CHNL_OPERATIONAL) 
            {       
                /* resolve channels that have been flagged as fuzzy */
                if (TRUE == mapotn_var_reg->tx_fuzzy_flag[chnl_itr])
                {
                    mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].adaptation_func = 
                        mapotn_resolve_fuzzy_adaptation_func(mapotn_handle,   
                                                             mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].adaptation_func,
                                                             mapotn_handle->var.tx_chnl_ctxt[chnl_itr].adaptation_func);
                }
                /* Check num entries, destination and mapping mode */
                if (mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].num_cal_entries  == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].num_cal_entries &&
                    mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].src_dest     == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].src_dest &&
                    mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].mapping_mode == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].mapping_mode  && 
                    mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].adaptation_func  == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].adaptation_func)                
                {
                    (void) mapotn_db_chnl_num_entries_get(mapotn_handle,chnl_itr,UTIL_GEN_DB_ODUKSC_DB,&num_cal_entries);
                    if ( PMC_SUCCESS == rc && num_cal_entries == mapotn_var_reg->var.tx_chnl_ctxt[chnl_itr].num_cal_entries) 
                    {                   
                        (void) mapotn_db_chnl_entry_get(mapotn_handle, UTIL_GEN_DB_ODUKSC_DB,chnl_itr,num_cal_entries,&cal_entries[0]);
                        for (cal_itr = 0;cal_itr < num_cal_entries;cal_itr++)
                        {                        
                            if (chnl_itr != mapotn_var_reg->oduksc_scbs3_page.calendar[cal_entries[cal_itr]])
                            {
                                accept_chnl = FALSE;
                                need_cleanup = TRUE;
                            }
                        }
                        if (accept_chnl == TRUE)
                        {
                            PMC_MEMCPY(&mapotn_var_clean->tx_chnl_ctxt[chnl_itr], &mapotn_handle->var.tx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t));                
                        }
                    } else 
                    {
                        need_cleanup = TRUE;
                    }
                } else 
                {
                    need_cleanup = TRUE;
                }
            }    
            /* resolve equipped case, we accept 1 side in EQUIPPED state if the other one is in OPERATIONAL state
               that is done to support working-protect scheme */
            if (restart_init_cfg_ptr->is_warm_restart == TRUE)            
            {
                if (rx_equipped == TRUE)
                {
                    PMC_MEMCPY(&mapotn_var_clean->rx_chnl_ctxt[chnl_itr], &mapotn_handle->var.rx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t));                
                }
                if (tx_equipped == TRUE)
                {
                    PMC_MEMCPY(&mapotn_var_clean->tx_chnl_ctxt[chnl_itr], &mapotn_handle->var.tx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t));                
                }
                if (rx_check_ctxt_sync == TRUE ||
                    tx_check_ctxt_sync == TRUE)
                {
                    need_cleanup = TRUE;
                }
            } else 
            {
                if (rx_equipped == TRUE && tx_equipped == TRUE)
                {
                    need_cleanup = TRUE;
                } else if (rx_equipped == TRUE)
                {
                    if (MAPOTN_CHNL_OPERATIONAL == mapotn_var_clean->tx_chnl_ctxt[chnl_itr].state)
                    {
                        PMC_MEMCPY(&mapotn_var_clean->rx_chnl_ctxt[chnl_itr], &mapotn_handle->var.rx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t));                
                    } else 
                    {
                        need_cleanup = TRUE;
                    }
                } else if (tx_equipped == TRUE)
                {
                    if (MAPOTN_CHNL_OPERATIONAL == mapotn_var_clean->rx_chnl_ctxt[chnl_itr].state)
                    {
                        PMC_MEMCPY(&mapotn_var_clean->tx_chnl_ctxt[chnl_itr], &mapotn_handle->var.tx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t));                
                    } else 
                    {
                        need_cleanup = TRUE;
                    }
                }
                
                if (tx_check_ctxt_sync == TRUE)
                {
                    if (MAPOTN_CHNL_OPERATIONAL == mapotn_var_clean->rx_chnl_ctxt[chnl_itr].state && 
                        MAPOTN_CHNL_START  == mapotn_var_clean->tx_chnl_ctxt[chnl_itr].state && 
                        MAPOTN_CHNL_OPERATIONAL == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].state)
                    {
                         PMC_MEMCPY(&mapotn_var_clean->tx_chnl_ctxt[chnl_itr], &mapotn_handle->var.tx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t)); 
                         mapotn_var_clean->tx_chnl_ctxt[chnl_itr].state = MAPOTN_CHNL_EQUIPPED;
                    } else 
                    {
                        need_cleanup = TRUE;
                    }
                }
                if (rx_check_ctxt_sync == TRUE)
                {
                    if (MAPOTN_CHNL_OPERATIONAL == mapotn_var_clean->tx_chnl_ctxt[chnl_itr].state && 
                        MAPOTN_CHNL_START  == mapotn_var_clean->rx_chnl_ctxt[chnl_itr].state && 
                        MAPOTN_CHNL_OPERATIONAL == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].state)
                    {
                         PMC_MEMCPY(&mapotn_var_clean->rx_chnl_ctxt[chnl_itr], &mapotn_handle->var.rx_chnl_ctxt[chnl_itr], sizeof(mapotn_chnl_ctxt_t)); 
                         mapotn_var_clean->rx_chnl_ctxt[chnl_itr].state = MAPOTN_CHNL_EQUIPPED;
                    } else 
                    {
                        need_cleanup = TRUE;
                    }
                }

            }
        }
    }

    if (TRUE == need_cleanup)
    {
        rc = MAPOTN_ERR_CONTEXT_CLEANUP;
    }
    PMC_RETURN(rc);
} /* mapotn_ctxt_audit */

/*******************************************************************************
* mapotn_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function put in place the cleaned var_t context. The register and 
*   memory context of channels that are not in OPERATIONAL state are cleaned 
*
* INPUTS:
*   *mapotn_handle           - pointer to MAPOTN handle instance
*   *mapotn_var_reg          - A partial mapotn_var_t context + additional
*                              information to facilite audit phase
*   *mapotn_energy_state_reg - MAPOTN energy states 
*   *mapotn_var_clean        - A clean context which represents the intersection
*                              between both context is also generated.
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
PRIVATE PMC_ERROR mapotn_ctxt_cleanup(mapotn_handle_t *mapotn_handle,
                                      mapotn_recover_var_t *mapotn_var_reg,                                         
                                      mapotn_energy_state_t *mapotn_energy_state_reg,
                                      mapotn_var_t *mapotn_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 chnl_to_clean[MAPOTN_NUM_CHANL];
    UINT32 num_chnl_to_clean;    
    UINT32 chnl_itr;
    
    PMC_ENTRY();

    /* the old mapotn var context is incoherent, we use the new one */
    PMC_MEMCPY(&mapotn_handle->var,mapotn_var_clean,sizeof(mapotn_var_t));
    
    /* resync energy state */           
    mapotn_init(mapotn_handle, PMC_ENERGY_STATE_REQUEST_OPTIMAL,UTIL_GLOBAL_MAP_NO_RATE);

    /* Perform regiter cleanup and sub-blocks memory context cleanup */
    if (mapotn_start_state_test(mapotn_handle) == FALSE)
    {
        /* demapper cleanup */
        num_chnl_to_clean = 0;
        for (chnl_itr = 0; chnl_itr  < MAPOTN_NUM_CHANL && PMC_SUCCESS == rc; chnl_itr++)
        {
            if (MAPOTN_CHNL_START == mapotn_handle->var.rx_chnl_ctxt[chnl_itr].state)
            {
                chnl_to_clean[num_chnl_to_clean] = chnl_itr;
                num_chnl_to_clean += 1;
            }
        }
        mapotn_demapper_clean(mapotn_handle,
                              mapotn_var_reg->has_rx_gfp_chnl,
                              mapotn_var_reg->has_rx_gmp_chnl,
                              chnl_to_clean,
                              num_chnl_to_clean);
        
        /* mapper cleanup */
        num_chnl_to_clean = 0;
        for (chnl_itr = 0; chnl_itr  < MAPOTN_NUM_CHANL && PMC_SUCCESS == rc; chnl_itr++)
        {
            if (MAPOTN_CHNL_START == mapotn_handle->var.tx_chnl_ctxt[chnl_itr].state)
            {
                chnl_to_clean[num_chnl_to_clean] = chnl_itr;
                num_chnl_to_clean += 1;
            }
        }
        mapotn_mapper_clean(mapotn_handle,
                            mapotn_var_reg->has_tx_gfp_chnl,
                            mapotn_var_reg->has_tx_gmp_chnl,                            
                            chnl_to_clean,
                            num_chnl_to_clean);               
    } 
    PMC_RETURN(rc);
} /* mapotn_ctxt_cleanup */

/*******************************************************************************
*  mapotn_ctxt_mpmo_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reset and unconfigure MAPOTN and its sub-block specified link.
*
* INPUTS:
*   *hndl                  - pointer to MAPOTN handle instance   
*   *restart_init_cfg_ptr  - pointer to restart init config
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
PRIVATE void mapotn_ctxt_mpmo_cleanup(mapotn_handle_t             *hndl,
                                      util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    UINT32 chnl_to_clean[MAPOTN_NUM_CHANL];
    UINT32 num_chnl_to_clean = 0;    
    UINT32 chnl_itr =0;
    UINT8*  map2enet_chnl_ptr;
    PMC_ENTRY();

    PMC_ASSERT(hndl!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(restart_init_cfg_ptr->dcpb_connect!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
        
    map2enet_chnl_ptr = (UINT8*)restart_init_cfg_ptr->opaque;

    for(chnl_itr = 0;chnl_itr < MAPOTN_NUM_CHANL;chnl_itr++)
    {
        if (hndl->var.tx_chnl_ctxt[chnl_itr].state == MAPOTN_CHNL_OPERATIONAL && 
            hndl->var.tx_chnl_ctxt[chnl_itr].mapping_mode != UTIL_GLOBAL_GFP && 
            hndl->var.tx_chnl_ctxt[chnl_itr].src_dest     == MAPOTN_DEST_CPB)
        {                                
            if (UTIL_GLOBAL_CHNL_OPERATIONAL != restart_init_cfg_ptr->dcpb_connect->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_MAPOTN][chnl_itr].mst.state)
            {
                chnl_to_clean[num_chnl_to_clean] = chnl_itr;
                num_chnl_to_clean += 1;
            }
        }
        if (hndl->var.tx_chnl_ctxt[chnl_itr].state == MAPOTN_CHNL_OPERATIONAL && 
            hndl->var.tx_chnl_ctxt[chnl_itr].mapping_mode == UTIL_GLOBAL_GFP && 
            hndl->var.tx_chnl_ctxt[chnl_itr].src_dest     == MAPOTN_DEST_CPB)
        {                                
            if (UTIL_GLOBAL_CHNL_OPERATIONAL != restart_init_cfg_ptr->ocpb_connect->chnl[(UINT32)UTIL_GLOBAL_ODUK_PORT_MAPOTN][chnl_itr].slv.state)
            {
                (void) mapotn_mapper_deactivate(hndl,chnl_itr);
            }
        }
        if (hndl->var.rx_chnl_ctxt[chnl_itr].state == MAPOTN_CHNL_OPERATIONAL && 
            (hndl->var.rx_chnl_ctxt[chnl_itr].mapping_mode == UTIL_GLOBAL_GFP && hndl->var.rx_chnl_ctxt[chnl_itr].adaptation_func != UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP))
        {                                
            if ((hndl->var.rx_chnl_ctxt[chnl_itr].src_dest     == MAPOTN_DEST_CPB && 
                 UTIL_GLOBAL_CHNL_OPERATIONAL != restart_init_cfg_ptr->dcpb_connect->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_MAPOTN][chnl_itr].slv.state) ||
                (hndl->var.rx_chnl_ctxt[chnl_itr].src_dest     == MAPOTN_DEST_ENET && 
                 UTIL_GLOBAL_CHNL_OPERATIONAL != restart_init_cfg_ptr->dcpb_connect->chnl[(UINT32)UTIL_GLOBAL_CPB_PORT_ENET_LINE][map2enet_chnl_ptr[chnl_itr]].slv.state))                                
            {
                (void) mapotn_demapper_deactivate(hndl,chnl_itr);
            }
        }

    }
    if (num_chnl_to_clean != 0)
    {
        mpmo_ch_clean(hndl->mpmo_handle,&chnl_to_clean[0], num_chnl_to_clean);
    }
    
    PMC_RETURN();
    
} /* mapotn_ctxt_mpmo_cleanup */

/*******************************************************************************
* mapotn_recover_adaptation_func
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Recover Adaptation function from register space.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   path                - RX or TX
*   chnl                - channel
*   mapping_mode        - the mapping mode
*   num_cal_entries     - number of calendar entries for this channel
*
* OUTPUTS:
*   *src_dest            - source or destination
*   *adaptation_function - major operational mode of resource
*                          Refer to util_global_map_adapt_func_t for valid types
*   *fuzzy_flag          - This flag is set when a valid decision has been made
*                          but other values may be good too. The final
*                          decision is made when we will be compare context
*
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_recover_adaptation_func(mapotn_handle_t              *mapotn_handle, 
                                                 mapotn_path_t                path,
                                                 UINT32                       chnl,
                                                 util_global_mapping_mode_t   mapping_mode,
                                                 UINT32                       num_cal_entries,
                                                 mapotn_src_dest_t            *src_dest,
                                                 util_global_map_adapt_func_t *adaptation_function, 
                                                 BOOL8                        *fuzzy_flag)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 has_fixed_stuff;
    DOUBLE candidate_rates[11];
    util_opsa_cn_frame_period_t candidate_cn_frame[16];
    UINT32 num_candidate_rates;
    UINT32 best_rate_idx;
    DOUBLE rate_error;
    UINT32 pkt_size = 0;
    UINT32 cn_base;
    UINT32 pkts_per_cn;
    UINT32 cn_base_int; 
    UINT32 cn_base_n; 
    UINT32 cn_base_d;
    UINT32 cn_frm_per_n; 
    UINT32 cn_frm_per_d;

    PMC_ENTRY();
    
    *adaptation_function = UTIL_GLOBAL_MAP_NO_RATE;
    *fuzzy_flag = FALSE;

    if (UTIL_GLOBAL_GFP == mapping_mode)
    {
        UINT32 fc1200_traffic, extended_payload;
        
        if (MAPOTN_RX == path)
        {
            fc1200_traffic   = mapotn_rgfpf_field_FC1200_TRAFFIC_get(NULL, mapotn_handle, chnl);
            extended_payload = ohfs_remove_ext_payload_get(mapotn_handle->ohfs_remove_handle,chnl);
        } 
        else 
        {
            extended_payload = ohfs_insert_ext_payload_get(mapotn_handle->ohfs_insert_handle,chnl);
            fc1200_traffic = 0;
        }
        
        /* resolve it */
        if (1 == fc1200_traffic)
        {
            *adaptation_function = UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP;
        } 
        else if (1 == extended_payload)
        {
            *adaptation_function = UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP;

        }
        else if (*src_dest == MAPOTN_DEST_ENET || *src_dest == MAPOTN_DEST_CPB_ENET)
        {
            if (*src_dest == MAPOTN_DEST_ENET && MAPOTN_TX == path &&
                UTIL_GLOBAL_ODUFLEX_GFP ==
                mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type &&
                num_cal_entries == mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries &&
                MAPOTN_DEST_CPB == mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest &&
                mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest ==
                mapotn_handle->var.rx_chnl_ctxt[chnl].src_dest)
            {
                *adaptation_function = UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP;
                *src_dest = MAPOTN_DEST_CPB;
            }
            else if (8 == num_cal_entries)
            {
                 *adaptation_function = UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP;
            } 
            else if (32 == num_cal_entries)
            {
                *adaptation_function = UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP;
            } 
            else if (80 == num_cal_entries)
            {
                *adaptation_function = UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP;
            }
        } 
        else 
        {
            /* it is going DCPB...*/
            *adaptation_function = UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP;
            /* When we have 8, 32 , 80 entries, the ENET mapping may be also valid too
               We could look at the FCS default config to take a better decision, but this setting may be changed using a running API. 
               Therefore, we says UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP, but ENET mapping will be accepted when we will compare the 
               register and memory context
             */
            if(8  == num_cal_entries || 
               32 == num_cal_entries ||
               80 == num_cal_entries)
            {
                *fuzzy_flag = TRUE;                
            }
            *adaptation_function = UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP;
        }
    } 
    else 
    {
        /* HERE we are with BMP/AMP/GMP mapping */  

        /* Determine if fixed is enabled, it is specific per mapping */
        if (MAPOTN_RX == path)
        {
            has_fixed_stuff = rgmp_fixed_stuff_enable_get(mapotn_handle->rgmp_handle,chnl);
        } else 
        {
            has_fixed_stuff = tgmp_fixed_stuff_enable_get(mapotn_handle->tgmp_handle,chnl);
        }

        /* resolve ENET mapping */
        if (*src_dest == MAPOTN_DEST_ENET || *src_dest == MAPOTN_DEST_CPB_ENET)
        {
            if (8 == num_cal_entries)
            {
                if (TRUE == has_fixed_stuff)
                {
                    *adaptation_function = UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP ;
                } else 
                {
                    *adaptation_function = UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP;
                }
            } else if (32 == num_cal_entries)
            {
                *adaptation_function = UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP;
            } else if (80 == num_cal_entries)
            {
                *adaptation_function = UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP;
            }   
        } else if (UTIL_GLOBAL_AMP == mapping_mode)
        {            
            /* all these mapping are for DCPB */            
            if (FALSE == has_fixed_stuff)
            {
                *adaptation_function = UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP;
            } else 
            {
                if (8 == num_cal_entries)
                {
                    *adaptation_function = UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP;
                } else if (32 == num_cal_entries)
                {
                    *adaptation_function = UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP;
                }   
            }
        }  else if (UTIL_GLOBAL_BMP == mapping_mode)    
        {
            /* all these mapping are for DCPB */ 
            if (TRUE == has_fixed_stuff && 32 == num_cal_entries)
            {              
                *adaptation_function = UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_BMP;
            } else {   
                DOUBLE actual_server_rate;
                UINT32 server_rate;
                
                /* for BMP, we use the server rate to recover the adaptation function*/
                mapotn_server_rate_cal(mapotn_handle, UTIL_GLOBAL_ODU2,0,0,&server_rate);
                candidate_rates[0] = (DOUBLE) 238/239* server_rate;
                mapotn_server_rate_cal(mapotn_handle, UTIL_GLOBAL_ODU2E,0,0,&server_rate);
                candidate_rates[1] = (DOUBLE) 238/239* server_rate;
                mapotn_server_rate_cal(mapotn_handle, UTIL_GLOBAL_ODU2F,0,0,&server_rate);
                candidate_rates[2] = (DOUBLE) 238/239* server_rate;
                mapotn_server_rate_cal(mapotn_handle, UTIL_GLOBAL_ODU1F,0,0,&server_rate);
                candidate_rates[3] = (DOUBLE) 238/239* server_rate;
                mapotn_server_rate_cal(mapotn_handle, UTIL_GLOBAL_ODU1E,0,0,&server_rate);
                candidate_rates[4] = (DOUBLE) 238/239* server_rate;
                candidate_rates[5] = UTIL_GLOBAL_FC_800_RATE/1000;
                candidate_rates[6] = UTIL_GLOBAL_GDPS_5G_RATE/1000;
                candidate_rates[7] = UTIL_GLOBAL_GDPS_10G_RATE/1000;
                candidate_rates[8] = UTIL_GLOBAL_CPRI_5_RATE/1000;
                candidate_rates[9] = UTIL_GLOBAL_CPRI_6_RATE/1000;
                candidate_rates[10] = UTIL_GLOBAL_CPRI_7_RATE/1000;                    
                num_candidate_rates = 11;
                
                rc = oduksc_bmp_mode_cgen_chnl_cfg_get(mapotn_handle->oduksc_handle,chnl,&actual_server_rate);

                if (PMC_SUCCESS == rc)
                {
                    if (actual_server_rate != 1)
                    {
                        actual_server_rate = (DOUBLE) 238/239* actual_server_rate;
                        util_opsa_find_best_rate(actual_server_rate, &candidate_rates[0],num_candidate_rates,&best_rate_idx,&rate_error);
                        /* if the error is correct, we accept it */
                        if (rate_error < 0.01)
                        {
                            server_rate = candidate_rates[best_rate_idx];
                            if (3 == best_rate_idx)                         /* ODU1F */
                            {
                                *adaptation_function = UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP;
                            } else if (4 == best_rate_idx)                         /* ODU1E */
                            {
                                *adaptation_function = UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP;
                            } else if (0 == best_rate_idx)                         /* ODU2 */
                            {
                                *adaptation_function = UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP;
                            } else if (1 == best_rate_idx)                         /* ODU2E */
                            {
                                *adaptation_function = UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP;
                                *fuzzy_flag = TRUE;
                                /* for this mapping, we also accept ODU2EP_CBR_10G3_7_1_BMP */
                            } else if (2 == best_rate_idx)                         /* ODU2F */
                            {
                                *adaptation_function = UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP;                           
                            } else 
                            {
                                *adaptation_function = mapotn_adaptation_func_per_rate(mapotn_handle,candidate_rates[best_rate_idx]*1000,FALSE);
                            }
                            /* 
                           however, we could have a CBRX rate with the rate of a predefined mapping 
                           we set the fuzzy flag and the selected mapping and UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP will
                           be accepted
                            */
                            *fuzzy_flag = TRUE;
                        
                        } else 
                        {
                            *adaptation_function = UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP; 
                        }       
                    }
                    else
                    {
                        *adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP;
                        *fuzzy_flag = TRUE;
                    } 
                }          
            }
        } else if (UTIL_GLOBAL_GMP == mapping_mode)    
        {                    
            if (TRUE == has_fixed_stuff)
            {
                /* fixed stuff is only for ODU4 */
                *adaptation_function = UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP;
            } else if ( !(8 == num_cal_entries || 9 == num_cal_entries || 32 == num_cal_entries))
            {
                
                /* a flex number of entries can only use the CBRX mapping */
                *adaptation_function = UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP;
               
            } else 
            {                               
                /* Get the information required to deduce the client rate */
                if (MAPOTN_RX == path)
                {
                    /* get information from our MPMA */                    
                    mpma_ch_pkt_cfg_get(mapotn_handle->mpma_handle,chnl,&pkt_size,&pkts_per_cn,&cn_base);
                }  else {
                    /* get information from our MPMO */
                    rc = mpmo_ch_pkt_cfg_get(mapotn_handle->mpmo_handle,chnl,&pkt_size,&cn_base_int, 
                                             &cn_base_n,&cn_base_d,&cn_frm_per_n, &cn_frm_per_d);                    
                    if (0 == pkt_size || (0 == cn_base_int && 0 == cn_base_n))
                    {
                        *fuzzy_flag = TRUE;
                        mpma_ch_pkt_cfg_get(mapotn_handle->mpma_handle,chnl,&pkt_size,&pkts_per_cn,&cn_base);
                    }
                }
                if (PMC_SUCCESS == rc)
                {
                    /* define what are the candidates */
                    if (32 == num_cal_entries)
                    {                        
                        candidate_cn_frame[0] = UTIL_OPSA_MAPOTN_ODU3E2_T_FRAME;
                        candidate_cn_frame[1] = UTIL_OPSA_MAPOTN_ODU3_T_FRAME;
                    
                        candidate_rates[0] = UTIL_GLOBAL_CBR_40_G_RATE;
                        candidate_rates[1] = UTIL_GLOBAL_ENET_40GE_XCD_RATE;
                        num_candidate_rates = 2;
                    } else
                    {
                        candidate_cn_frame[0] = UTIL_OPSA_MAPOTN_ODU2_T_FRAME;
                        candidate_cn_frame[1] = UTIL_OPSA_MAPOTN_ODU2E_T_FRAME;
                        candidate_cn_frame[2] = UTIL_OPSA_MAPOTN_ODU2_T_FRAME;
                        candidate_cn_frame[3] = UTIL_OPSA_MAPOTN_ODU2_T_FRAME;
                        candidate_cn_frame[4] = UTIL_OPSA_MAPOTN_ODU2_T_FRAME;
                        candidate_cn_frame[5] = UTIL_OPSA_MAPOTN_ODU2_T_FRAME;
                        
                        candidate_rates[0] = UTIL_GLOBAL_FC_800_RATE;
                        candidate_rates[1] = UTIL_GLOBAL_GDPS_10G_RATE;
                        candidate_rates[2] = UTIL_GLOBAL_GDPS_5G_RATE;
                        candidate_rates[3] = UTIL_GLOBAL_CPRI_5_RATE;
                        candidate_rates[4] = UTIL_GLOBAL_CPRI_6_RATE;
                        candidate_rates[5] = UTIL_GLOBAL_CPRI_7_RATE;
                        num_candidate_rates = 6;
                    }
                    /* find the best fit */
                    if (MAPOTN_RX == path ||
                        (MAPOTN_TX == path && *fuzzy_flag == TRUE))
                    {
                        util_opsa_find_best_rate_per_pkt_size_period(pkt_size,pkts_per_cn, cn_base,
                                                                     &candidate_rates[0],&candidate_cn_frame[0],
                                                                     num_candidate_rates,&best_rate_idx,&rate_error);
                    } else 
                    {                        
                        util_opsa_find_best_rate_per_cn_base(pkt_size,cn_base_int,cn_base_n,cn_base_d,cn_frm_per_n,cn_frm_per_d,
                                                             &candidate_rates[0],num_candidate_rates,&best_rate_idx,&rate_error);
                    }

                    /* when the error is low, we accept the result, otherwise it is CBR rate */
                    if (rate_error < 0.01)
                    {
                        *adaptation_function = mapotn_adaptation_func_per_rate(mapotn_handle,candidate_rates[best_rate_idx],TRUE);
                        /* 
                           however, we could have a CBRX rate with the rate of a predefined mapping 
                           we set the fuzzy flag and the selected mapping and UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP will
                           be accepted
                         */
                        *fuzzy_flag = TRUE;
                    } else 
                    {
                        *adaptation_function = UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP; 
                    }                     
                }
            }
        }
    }


    if (*adaptation_function == UTIL_GLOBAL_MAP_NO_RATE)
    {
        rc = MAPOTN_ERR_CONTEXT_INCOHERENT;
    }

    PMC_RETURN(rc);
} /*mapotn_recover_adaptation_func*/
    

/*******************************************************************************
* mapotn_resolve_fuzzy_adaptation_func
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function resolves adaptation functions that have been flsaged as fuzzy 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle         - pointer to MAPOTN handle instance                  
*   recov_adaptation_func  - The recovered adaptation function 
*   var_adaptation_func    - The adaptation function stored in memory context 
*                                                                               
* OUTPUTS:
*   None.                   
*
* RETURNS:
*   util_global_map_adapt_func_t - The resolved adaptation function
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PRIVATE util_global_map_adapt_func_t mapotn_resolve_fuzzy_adaptation_func(mapotn_handle_t              *mapotn_handle,                                                  
                                                                          util_global_map_adapt_func_t recov_adaptation_func, 
                                                                          util_global_map_adapt_func_t var_adaptation_func)
{
    util_global_map_adapt_func_t resolved_af;
    
    PMC_ENTRY();

    resolved_af = recov_adaptation_func;


    /* the ENET case */
    if (recov_adaptation_func == UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP)
    {
        /* we also accept these mapping */
        if (UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP == var_adaptation_func ||
            UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP     == var_adaptation_func ||
            UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP    == var_adaptation_func)
        {
            resolved_af = var_adaptation_func;
        }                        
    }
    /* the BMP cases  */
    if ( (UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP   == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP   == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP == recov_adaptation_func ) &&
         UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP == var_adaptation_func)
    {
        resolved_af = var_adaptation_func;
    }  
    if ( (UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP   == recov_adaptation_func ) &&
         UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP == var_adaptation_func)
    {
        resolved_af = var_adaptation_func;
    }  
    if ( (UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP  == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP  == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP  == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP  == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP  == recov_adaptation_func) &&         
         UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP == var_adaptation_func)
    {
        resolved_af = var_adaptation_func;
    }
    if ( (UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP  == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP  == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP  == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP  == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP  == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP  == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP == var_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP == var_adaptation_func) &&         
         UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP == recov_adaptation_func)
    {
        resolved_af = var_adaptation_func;
    }
    /* the GMP cases */
    if ( (UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP == recov_adaptation_func ) &&         
         UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP == var_adaptation_func)
    {
        resolved_af = var_adaptation_func;
    }
    if ( (UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2EP_10G_GDPS_GMP == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP == recov_adaptation_func ||
          UTIL_GLOBAL_MAP_ODU2P_CPRI_6_GMP == recov_adaptation_func) &&         
         UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP == var_adaptation_func)
    {
        resolved_af = var_adaptation_func;
    }                


    PMC_RETURN(resolved_af);
} /*  mapotn_resolve_fuzzy_adaptation_func */

/*******************************************************************************
* mapotn_chnl_count_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the number of GFP channel provisioned againt MAPOTN.
*
*
* INPUTS:
*   *mapotn_handle            - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   *gfp_chnl_count           - a pointer to the number of GFP channel provisioned 
*   *gmp_chnl_count           - a pointer to the number of GMP channel provisioned 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.    
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_chnl_count_get(mapotn_handle_t *mapotn_handle, 
                                       UINT32 *gfp_chnl_count, 
                                       UINT32 *gmp_chnl_count)
{
    /* Variable declaration */
    UINT32 i;
    UINT32 gfp_count = 0;
    UINT32 gmp_count = 0;
    
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {    
        if(mapotn_handle->var.tx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_GFP ||
           mapotn_handle->var.rx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_GFP) 
        {
            gfp_count++;        
            if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
            {
                gmp_count++;  
            }
        }
        if(mapotn_handle->var.tx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_GMP ||
           mapotn_handle->var.tx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_AMP ||
           mapotn_handle->var.tx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_BMP ||
           mapotn_handle->var.rx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_GMP ||
           mapotn_handle->var.rx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_AMP ||
           mapotn_handle->var.rx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_BMP)
        {
            gmp_count++;        
        }
    }
    
    *gfp_chnl_count = gfp_count;
    *gmp_chnl_count = gmp_count;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_chnl_count_get */

/*******************************************************************************
*  mapotn_var_volatile_ctxt_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes MAPOTN volatile var_t fiels
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   from_reg          - From register space 
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
PRIVATE void mapotn_var_volatile_ctxt_init(mapotn_handle_t *mapotn_handle,
                                           BOOL8 from_reg)
{
    /* Variable declaration */
    UINT32 chnl;
    
    PMC_ENTRY();
        
    /* Configure default MAPOTN variable parameters for each channel */
    for (chnl = 0; chnl < MAPOTN_NUM_CHANL; chnl++)
    {             
        if (mapotn_handle->var.rx_chnl_ctxt[chnl].state == MAPOTN_CHNL_OPERATIONAL && 
            TRUE == from_reg)
        {
            mapotn_rgfpf_volatile_ctxt_init(mapotn_handle,chnl);
        } else 
        {
            mapotn_handle->var.mapotn_rgfpf_chnl_fltr_mgmt[chnl].next_filter = 0;
            
        }        
    }

    PMC_RETURN();

} /* mapotn_var_volatile_ctxt_init */


/*******************************************************************************
*  mapotn_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes MAPOTN var_t variables.
*
* INPUTS:
*   *mapotn_var      - pointer to MAPOTN var context 
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
PRIVATE void mapotn_var_default_init(mapotn_var_t *mapotn_var)
{
    /* Variable declaration */
    UINT32 chnl;
    
    PMC_ENTRY();
        
    mapotn_var->mapotn_start_state  = TRUE;

    /* Configure default MAPOTN variable parameters for each channel */
    for (chnl = 0; chnl < MAPOTN_NUM_CHANL; chnl++)
    {
        
        mapotn_var->rx_chnl_ctxt[chnl].state            = MAPOTN_CHNL_START;
        mapotn_var->rx_chnl_ctxt[chnl].adaptation_func  = UTIL_GLOBAL_MAP_NO_RATE;
        mapotn_var->rx_chnl_ctxt[chnl].odukp_type       = UTIL_GLOBAL_ODUK_DONT_CARE;
        mapotn_var->rx_chnl_ctxt[chnl].client_rate      = 0;
        mapotn_var->rx_chnl_ctxt[chnl].num_cal_entries  = 0;
        mapotn_var->rx_chnl_ctxt[chnl].mapping_mode     = UTIL_GLOBAL_NO_MAP;
        mapotn_var->rx_chnl_ctxt[chnl].src_dest         = MAPOTN_DEST_CPB;
        
        mapotn_var->tx_chnl_ctxt[chnl].state            = MAPOTN_CHNL_START;
        mapotn_var->tx_chnl_ctxt[chnl].adaptation_func  = UTIL_GLOBAL_MAP_NO_RATE;
        mapotn_var->tx_chnl_ctxt[chnl].odukp_type       = UTIL_GLOBAL_ODUK_DONT_CARE;
        mapotn_var->tx_chnl_ctxt[chnl].client_rate      = 0;
        mapotn_var->tx_chnl_ctxt[chnl].num_cal_entries  = 0;
        mapotn_var->tx_chnl_ctxt[chnl].mapping_mode     = UTIL_GLOBAL_NO_MAP;
        mapotn_var->tx_chnl_ctxt[chnl].src_dest         = MAPOTN_DEST_CPB;              
    }

    PMC_RETURN();

} /* mapotn_var_default_init */


/*******************************************************************************
* mapotn_var_update
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Updates the channel based dynamically changing context in the memory.                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle       - pointer to MAPOTN handle instance
*   path                 - mapotn path to be configured \n
*                          Refer to mapotn_path_t for valid types
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95 
*   chnl_state           - the state of the channel \n
*                          Refer to mapotn_chnl_state_t forvalid types                                          
*   adaptation_function  - major operational mode of resource \n
*                          Refer to util_global_map_adapt_func_t for valid types
*   odukp_type           - ODUkp or ODUjp type \n
*                          Refer to util_global_odukp_type_t, ODUkp all ODUk
*                          types valid, ODUjp only ODU2/3 types valid
*   client_rate          - Client data rate of signal on MAPOTN system interface
*                          in kbits/sec.  
*   num_cal_entries      - number of calendar entries depending on the client 
*                          rate   
*   mapotn_mapping_mode  - mapping mode of the channel \n
*                          Refer to util_global_mapping_mode_t for valid types
*   chnl_src_dest        - source/destination of the channel \n
*                          Refer to mapotn_src_dest_t for valid types               
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
PRIVATE PMC_ERROR mapotn_var_update(mapotn_handle_t *mapotn_handle, 
                                    mapotn_path_t path,
                                    UINT32 chnl,
                                    mapotn_chnl_state_t chnl_state,
                                    util_global_map_adapt_func_t adaptation_function,
                                    util_global_odukp_type_t odukp_type, 
                                    DOUBLE client_rate,
                                    UINT32 num_cal_entries,
                                    util_global_mapping_mode_t mapotn_mapping_mode, 
                                    mapotn_src_dest_t chnl_src_dest)
{
    /* Variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(path <= MAPOTN_TX, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl_state <= MAPOTN_CHNL_OPERATIONAL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(adaptation_function <= UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(odukp_type < UTIL_GLOBAL_LAST_ODUK, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(mapotn_mapping_mode <= UTIL_GLOBAL_NO_MAP, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl_src_dest < LAST_MAPOTN_DEST, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(path == MAPOTN_RX) /* update demapper var context for a channel */
    {
        mapotn_handle->var.rx_chnl_ctxt[chnl].state            = chnl_state;
        mapotn_handle->var.rx_chnl_ctxt[chnl].adaptation_func  = adaptation_function;
        mapotn_handle->var.rx_chnl_ctxt[chnl].odukp_type       = odukp_type;
        mapotn_handle->var.rx_chnl_ctxt[chnl].client_rate      = client_rate;
        mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries  = num_cal_entries;
        mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode     = mapotn_mapping_mode;
        mapotn_handle->var.rx_chnl_ctxt[chnl].src_dest         = chnl_src_dest;
        mapotn_handle->var.rx_chnl_ctxt[chnl].ghao_start_oduflex_gfp_rate = num_cal_entries;
        mapotn_handle->var.rx_chnl_ctxt[chnl].doing_ghao_ramp_until_bwr_ind = FALSE;
    } 
    else if (path == MAPOTN_TX) /* update mapper var context for a channel */
    {
        mapotn_handle->var.tx_chnl_ctxt[chnl].state            = chnl_state;
        mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func  = adaptation_function;
        mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type       = odukp_type;
        mapotn_handle->var.tx_chnl_ctxt[chnl].client_rate      = client_rate;
        mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries  = num_cal_entries;
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode     = mapotn_mapping_mode;
        mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest         = chnl_src_dest;
        mapotn_handle->var.tx_chnl_ctxt[chnl].ghao_start_oduflex_gfp_rate = num_cal_entries;
        mapotn_handle->var.tx_chnl_ctxt[chnl].doing_ghao_ramp_until_bwr_ind = FALSE;
    }
    else 
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    } 
    
    PMC_RETURN(PMC_SUCCESS);
      
}  /* mapotn_var_update */ 


/*******************************************************************************
* mapotn_mapping_mode_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieves mapping mode for a specific adaptation function.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle       - pointer to MAPOTN handle instance                  
*   adaptation_function  - major operational mode of resource 
*                          Refer to util_global_map_adapt_func_t for valid types          
*                                                                               
* OUTPUTS:
*   *mapotn_mapping_mode - a pointer to the mapping mode  
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                      
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_mapping_mode_get(mapotn_handle_t *mapotn_handle, 
                                         util_global_map_adapt_func_t adaptation_function, 
                                         util_global_mapping_mode_t *mapotn_mapping_mode)
{
    /* Variable declaration */
    util_global_mapping_mode_t mapping_mode;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(adaptation_function <= UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP     == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU2EP_10G_GDPS_GMP  == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP    == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP     == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP     == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU2P_CPRI_6_GMP     == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP      == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP     == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP == adaptation_function  || \
        UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP       == adaptation_function)                         
    {                                                                    
             mapping_mode = UTIL_GLOBAL_GMP;
    }
    else if (UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP    == adaptation_function || \
             UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP    == adaptation_function || \
             UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP     == adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP == adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP == adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP  == adaptation_function)          
    {
             mapping_mode = UTIL_GLOBAL_GFP;
    }         
    else if (UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP     ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP    ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP      ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_BMP    ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP    ==  adaptation_function || \
             UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP      ==  adaptation_function)
    {
             mapping_mode = UTIL_GLOBAL_BMP;
    } 
    else if (UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP  == adaptation_function || \
             UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP  == adaptation_function || \
             UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP      == adaptation_function)
    {
             mapping_mode = UTIL_GLOBAL_AMP;
    } 
    else                     
    {                      
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);           
    }  
    
    PMC_LOG_TRACE( "adaptation_function = %u, mapping_mode = %u\n", adaptation_function, mapping_mode);
    
    *mapotn_mapping_mode = mapping_mode;
    
    
    PMC_RETURN(PMC_SUCCESS);
      
}  /* mapotn_mapping_mode_get */ 


/*******************************************************************************
* mapotn_client_rate_cal
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieves client rate for a specific adaptation function.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance                  
*   adaptation_function - major operational mode of resource 
*                         Refer to util_global_map_adapt_func_t for valid types
*   oduflex_cbr_rate    - ODUflex CBR container rate in kbits/sec.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of ODU0 timeslots
*                         1 to 2 ODU1, 1 to 4 ODU2, 1 to 32 ODU3, 1 to 80 ODU4.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP 
*   cbr_client_rate     - Client data rate of signal on MAPOTN system interface
*                         in kbits/sec.  Argument processed mode set to one of 
*                         (else unused):\n
*                         UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP\n
*                         UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A\n
*                         UTIL_GLOBAL_MAP_ODUKP_CBRX_A\n                 
*                                                                               
* OUTPUTS:
*   *client_rate        - a pointer to the client rate 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                       
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_client_rate_cal(mapotn_handle_t *mapotn_handle, 
                                         util_global_map_adapt_func_t adaptation_function,
                                         UINT32 oduflex_cbr_rate,
                                         UINT32 oduflex_gfp_rate,  
                                         UINT32 cbr_client_rate, 
                                         DOUBLE *client_rate)
{
    /* Variable declaration */
    DOUBLE rate = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(adaptation_function <= UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch (adaptation_function)
    {
        case UTIL_GLOBAL_MAP_NO_RATE: 
            rate = 0;
            break;
        case UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP: 
            rate = oduflex_gfp_rate * UTIL_GLOBAL_ODU0_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP: 
            rate = UTIL_GLOBAL_ENET_100GE_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP:
            rate = UTIL_GLOBAL_ENET_40GE_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP:
            rate = UTIL_GLOBAL_ENET_10GE_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP:
            rate = UTIL_GLOBAL_ENET_10GE_RATE;
            break; 
        case UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP:
            rate = UTIL_GLOBAL_FC_1200_TRANS_RATE;
            break;  
        case UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP:
            rate = UTIL_GLOBAL_FC_800_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2EP_10G_GDPS_GMP:
            rate = UTIL_GLOBAL_GDPS_10G_RATE;
            break;     
        case UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP:
            rate = UTIL_GLOBAL_GDPS_5G_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP:
            rate = UTIL_GLOBAL_CPRI_5_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2P_CPRI_6_GMP:
            rate = UTIL_GLOBAL_CPRI_6_RATE;
            break; 
        case UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP:
            rate = UTIL_GLOBAL_CPRI_7_RATE;
            break;       
        case UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP:
            rate = UTIL_GLOBAL_ENET_40GE_XCD_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP:
            rate = UTIL_GLOBAL_ENET_100GE_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP:
            rate = UTIL_GLOBAL_CBR_40_G_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP:
            rate = (DOUBLE)cbr_client_rate*1000;
            break; 
        case UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP:
            rate = UTIL_GLOBAL_GDPS_5G_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP:
            rate = UTIL_GLOBAL_GDPS_10G_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP:
            rate = UTIL_GLOBAL_CPRI_5_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP:
            rate = UTIL_GLOBAL_CPRI_6_RATE;
            break; 
        case UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP:
            rate = UTIL_GLOBAL_CPRI_7_RATE;
            break;  
        case UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP:
            rate = UTIL_GLOBAL_FC_800_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP:
            rate = UTIL_GLOBAL_FC_1200_TRANS_RATE;
            break; 
        case UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP:
            rate = UTIL_GLOBAL_FC_1200_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP:
            rate = UTIL_GLOBAL_FC_1200_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP:
            rate = UTIL_GLOBAL_ENET_10GE_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP:
            rate = UTIL_GLOBAL_ENET_10GE_RATE;
            break;    
        case UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_BMP:
            rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP:
            rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP:
            rate = (DOUBLE)(((DOUBLE)oduflex_cbr_rate*(DOUBLE)1000*(DOUBLE)238)/(DOUBLE)239);
            break;    
        case UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP:
            rate = UTIL_GLOBAL_STS_768_STM_256_RATE;
            break;
        case UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP:
            rate = UTIL_GLOBAL_STS_192_STM_64_RATE;
            break; 
        case UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP:
            rate = (DOUBLE)cbr_client_rate*1000;
            break;     
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    
    
    PMC_LOG_TRACE( "adaptation_function = %u, rate = %f\n", adaptation_function, rate);
    
    *client_rate = rate; 
    
    PMC_RETURN(PMC_SUCCESS);
      
}  /* mapotn_client_rate_cal */

/*******************************************************************************
* mapotn_adaptation_func_per_rate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieves adaptation function for a specific rate 
*   This function is intended to be used by mapotn_recover_adaptation_func and 
*   it is not supporting all rate/adaptation combination 
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance                  
*   client_rate         - the client rate 
*   is_GMP              - TRUE: GMP\n
*                         FALSE: BMP     
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_global_map_adapt_func_t - The adaptation function (major operational 
*                                  mode of resource)
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PRIVATE util_global_map_adapt_func_t mapotn_adaptation_func_per_rate(mapotn_handle_t *mapotn_handle, 
                                                                     DOUBLE client_rate,
                                                                     BOOL8 is_GMP)
{
    util_global_map_adapt_func_t adaptation_function;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(client_rate != 0, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    

    if (UTIL_GLOBAL_FC_800_RATE == client_rate) 
    {
        if (FALSE == is_GMP)
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP;
        } else 
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP;
        }
    } else if (UTIL_GLOBAL_GDPS_10G_RATE == client_rate)
    {
        if (FALSE == is_GMP)
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP;
        } else 
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODU2EP_10G_GDPS_GMP;
        }
    } else if (UTIL_GLOBAL_GDPS_5G_RATE == client_rate)
    {
        if (FALSE == is_GMP)
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP;
        } else 
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP;
        }
    } else if (UTIL_GLOBAL_CPRI_5_RATE == client_rate)
    {
        if (TRUE == is_GMP)
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP;
        } else 
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP;
        }
    } else if (UTIL_GLOBAL_CPRI_6_RATE == client_rate)
    {
        if (TRUE == is_GMP)
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODU2P_CPRI_6_GMP;
        } else 
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP;
        }
    } else if (UTIL_GLOBAL_CPRI_7_RATE == client_rate)
    {
        if (TRUE == is_GMP)
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP;
        } else 
        {
            adaptation_function = UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP;
        }
    } else if (UTIL_GLOBAL_ENET_40GE_XCD_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP;
    } else if (UTIL_GLOBAL_ENET_100GE_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP;
    } else if (UTIL_GLOBAL_CBR_40_G_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP;
    } else if (UTIL_GLOBAL_FC_1200_TRANS_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP;
    } else if (UTIL_GLOBAL_FC_1200_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP;
    } else if (UTIL_GLOBAL_FC_1200_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP;
    } else if (UTIL_GLOBAL_ENET_10GE_RATE == client_rate)
    {
        PMC_ASSERT(0,MAPOTN_ERR_INVALID_PARAMETERS,0,0);
        adaptation_function = UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP;
        adaptation_function = UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP;
    } else if (UTIL_GLOBAL_STS_768_STM_256_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_BMP;
    } else if (UTIL_GLOBAL_STS_192_STM_64_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP;
    } else if (UTIL_GLOBAL_STS_768_STM_256_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP;
    } else if (UTIL_GLOBAL_STS_192_STM_64_RATE == client_rate)
    {
        adaptation_function = UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP;
    } else 
    {
        PMC_ASSERT(0,MAPOTN_ERR_INVALID_PARAMETERS,0,0);
    }
        
    PMC_RETURN(adaptation_function);
      
}  /* mapotn_adaptation_func_per_rate */

/*******************************************************************************
* mapotn_server_rate_cal
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieves server rate for a specific oduk type.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance                  
*   odukp_type          - ODUkp or ODUjp type\n 
*                         Refer to util_global_odukp_type_t
*   oduflex_cbr_rate    - ODUflex CBR container rate in kbits/sec.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A 
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of ODU0 timeslots
*                         1 to 2 ODU1, 1 to 4 ODU2, 1 to 32 ODU3, 1 to 80 ODU4.
*                         Argument processed when (else unused):
*                         mode set to UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP                 
*                                                                               
* OUTPUTS:
*   *server_rate        - a pointer to the server rate  
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                               
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_server_rate_cal(mapotn_handle_t *mapotn_handle, 
                                        util_global_odukp_type_t odukp_type, 
                                        UINT32 oduflex_cbr_rate,
                                        UINT32 oduflex_gfp_rate,  
                                        UINT32 *server_rate)
{
    /* Variable declaration */
    UINT32 rate = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(odukp_type < UTIL_GLOBAL_LAST_ODUK, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch (odukp_type)
    {
        case UTIL_GLOBAL_ODUFLEX_GFP: 
            if(oduflex_gfp_rate <= 8){
                 rate = (UINT32)((oduflex_gfp_rate * UTIL_GLOBAL_ODU2_TS_RATE)/1000);
            } else if(oduflex_gfp_rate <= 32){
                 rate = (UINT32)((oduflex_gfp_rate * UTIL_GLOBAL_ODU3_TS_RATE)/1000);
            } else {
                rate = (UINT32)((oduflex_gfp_rate * UTIL_GLOBAL_ODU4_TS_RATE)/1000);
            }
            /* rate = (oduflex_gfp_rate * UTIL_GLOBAL_ODU0_RATE)/1000; */
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR: 
            rate = oduflex_cbr_rate;
            break;
        case UTIL_GLOBAL_ODU0: 
            rate = (UINT32)(UTIL_GLOBAL_ODU0_RATE/1000);
            break;
        case UTIL_GLOBAL_ODU1:
            rate = (UINT32)(UTIL_GLOBAL_ODU1_RATE/1000);
            break;
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
            rate = (UINT32)(UTIL_GLOBAL_ODU1E_RATE/1000);
            break; 
        case UTIL_GLOBAL_ODU1F:
            rate = (UINT32)(UTIL_GLOBAL_ODU1F_RATE/1000);
            break;  
        case UTIL_GLOBAL_ODU2:
            rate = (UINT32)(UTIL_GLOBAL_ODU2_RATE/1000);
            break;
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
            rate = (UINT32)(UTIL_GLOBAL_ODU2E_RATE/1000);
            break;     
        case UTIL_GLOBAL_ODU2F:
            rate = (UINT32)(UTIL_GLOBAL_ODU2F_RATE/1000);
            break;
        case UTIL_GLOBAL_ODU3:
            rate = (UINT32)(UTIL_GLOBAL_ODU3_RATE/1000);
            break;  
        case UTIL_GLOBAL_ODU3E1:
            rate = (UINT32)(UTIL_GLOBAL_ODU3E1_RATE/1000);
            break;                                      
        case UTIL_GLOBAL_ODU3E2:
            rate = (UINT32)(UTIL_GLOBAL_ODU3E2_RATE/1000);
            break; 
        case UTIL_GLOBAL_ODU4:
            rate = (UINT32)(UTIL_GLOBAL_ODU4_RATE/1000);
            break;     
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    
    
    PMC_LOG_TRACE( "server rate = %d\n", rate);
    
    *server_rate = rate; 
    
    PMC_RETURN(PMC_SUCCESS);
      
}  /* mapotn_server_rate_cal */


/*******************************************************************************
* mapotn_dci_2_dpi_fifo_enbl
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Enables DCI 2 DPI FIFO for a given channel.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance 
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                  
   valid                - Indicates whether a channel belong to GFP maped 
*                         traffic or GMP mapped traffic in demapping direction \n
*                         0 -- channel does not belong to GMP mapped traffic 
*                              destined to Ethernet \n 
*                         1 -- channel belongs to GMP mapped traffic destined to
*                              Ethernet           
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
PRIVATE PMC_ERROR mapotn_dci_2_dpi_fifo_enbl(mapotn_handle_t *mapotn_handle, 
                                             UINT32 chnl, 
                                             UINT32 valid)
{
    /* Variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rx_lfield_range_DCI2DPI_EN_set(NULL,mapotn_handle,chnl,chnl,valid);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_dci_2_dpi_fifo_enbl */


/*******************************************************************************
* mapotn_map_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Controls propagation of consequential actions to SSF
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance 
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                  
*   map_ssf_type        - Which type of defect to map to ssf 
*   enable              - defines whether the specified defect should be 
*                         passed to egress SSF in operation mode \n
*                         0 -- disable propagation \n
*                         1 -- enable propagation \n
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
PUBLIC PMC_ERROR mapotn_map_to_ssf_set(mapotn_handle_t *mapotn_handle, 
                                       UINT32 chnl, 
                                       ohfs_remove_map_ssf_type_t map_ssf_type,
                                       UINT32 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)
    {        
        result = ohfs_remove_map_ssf_set(mapotn_handle->ohfs_remove_handle,chnl,map_ssf_type,enable);        
    } else 
    {
        result = ohfs_remove_map_ssf_set(mapotn_handle->rgmp_handle->ohfs_remove_handle,chnl,map_ssf_type,enable);
    }   
    
    PMC_RETURN(result);
} /* mapotn_map_to_ssf_set */

/*******************************************************************************
* mapotn_map_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Controls propagation of consequential actions to SSF
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance 
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                  
*   map_ssf_type        - Which type of defect to map to ssf 
*
* OUTPUTS:
*   *enable             - defines whether the specified defect should be 
*                         passed to egress SSF in operation mode \n
*                         0 -- disable propagation \n
*                         1 -- enable propagation \n
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_map_to_ssf_get(mapotn_handle_t *mapotn_handle, 
                                       UINT32 chnl, 
                                       ohfs_remove_map_ssf_type_t map_ssf_type,
                                       UINT32 *enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)
    {        
        result = ohfs_remove_map_ssf_get(mapotn_handle->ohfs_remove_handle,chnl,map_ssf_type,enable);        
    } else 
    {
        result = ohfs_remove_map_ssf_get(mapotn_handle->rgmp_handle->ohfs_remove_handle,chnl,map_ssf_type,enable);
    }   
    
    PMC_RETURN(result);
} /* mapotn_map_to_ssf_get */


/* ENERGY functions  */  

/*******************************************************************************
*  mapotn_energy_state_reg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets all energy states from mapotn
*   
* INPUTS:
*   *mapotn_handle       - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   *mapotn_energy_state - MAPOTN energy states
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE void mapotn_energy_state_reg_get(mapotn_handle_t    *mapotn_handle, 
                                         mapotn_energy_state_t *mapotn_energy_state)
{    
    UINT32 low_pwr, ram_low_pwr, soft_reset;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* RGFFP96 */
    low_pwr        = mapotn_rx_field_RGFPF96_LOWPWR_get( NULL, mapotn_handle);
    ram_low_pwr    = mapotn_rx_field_RGFPF96_RAM_LOWPWR_get( NULL, mapotn_handle);
    soft_reset     = mapotn_rx_field_RGFPF96_SOFT_RST_get( NULL, mapotn_handle);
    mapotn_energy_state->rgfpf96 = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* MPMA */
    low_pwr        = mapotn_rx_field_MPMA_LOWPWR_get(NULL, mapotn_handle);
    ram_low_pwr    = mapotn_rx_field_MPMA_RAM_LOWPWR_get(NULL, mapotn_handle);
    soft_reset     = mapotn_rx_field_MPMA_SOFT_RST_get(NULL, mapotn_handle);
    mapotn_energy_state->mpma = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);
    
    /* RGMP */    
    low_pwr        = mapotn_rx_field_RGMP96_LOWPWR_get( NULL, mapotn_handle);
    ram_low_pwr    = mapotn_rx_field_RGMP96_RAM_LOWPWR_get( NULL, mapotn_handle);
    soft_reset     = mapotn_rx_field_RGMP96_SOFT_RST_get( NULL, mapotn_handle);
    mapotn_energy_state->rgmp96 = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* TOP RX */
    ram_low_pwr    = mapotn_rx_field_TOP_RAM_LOWPWR_get(NULL, mapotn_handle);
    soft_reset     = mapotn_rx_field_MAPOTN_RX_SOFT_RST_get(NULL, mapotn_handle);
    /* we invert FIFO_RSTB to use it in util_global_recover_energy_state */
    low_pwr        = mapotn_rx_field_FIFO_RSTB_get(NULL, mapotn_handle) == 0x1?0x0:0x1;
    mapotn_energy_state->rx_top = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* TGFPF96 */
    low_pwr        = mapotn_tx_field_TGFPF96_LOWPWR_get( NULL, mapotn_handle);
    ram_low_pwr    = mapotn_tx_field_TGFPF96_RAM_LOWPWR_get( NULL, mapotn_handle);
    soft_reset     = mapotn_tx_field_TGFPF96_SOFT_RST_get( NULL, mapotn_handle);
    mapotn_energy_state->tgfpf96 = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);
    
    /* TGMP96 */
    low_pwr        = mapotn_tx_field_TGMP96_LOWPWR_get( NULL, mapotn_handle);
    ram_low_pwr    = mapotn_tx_field_TGMP96_RAM_LOWPWR_get( NULL, mapotn_handle);
    soft_reset     = mapotn_tx_field_TGMP96_SOFT_RST_get( NULL, mapotn_handle);
    mapotn_energy_state->tgmp96 = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* MPMO */
    low_pwr        = mapotn_tx_field_MPMO_LOWPWR_get(NULL, mapotn_handle);
    ram_low_pwr    = mapotn_tx_field_MPMO_RAM_LOWPWR_get(NULL, mapotn_handle);
    soft_reset     = mapotn_tx_field_MPMO_SOFT_RST_get(NULL, mapotn_handle);
    mapotn_energy_state->mpmo = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);
    
    /* ODUKSC */            
    low_pwr        = mapotn_tx_field_ODUKSC_LOWPWR_get(NULL, mapotn_handle);
    ram_low_pwr    = mapotn_tx_field_ODUKSC_RAM_LOWPWR_get(NULL, mapotn_handle);
    soft_reset     = mapotn_tx_field_ODUKSC_SOFT_RST_get(NULL, mapotn_handle);
    mapotn_energy_state->oduksc = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    /* TX TOP */
    soft_reset     = mapotn_tx_field_MAPOTN_TX_SOFT_RST_get(NULL, mapotn_handle);
    /* we invert FIFO_RSTB to use it in util_global_recover_energy_state */
    low_pwr        = mapotn_tx_field_FIFO_CLRB_get(NULL,mapotn_handle) == 0x1?0x0:0x1;
    /* just add a third dummy parameter to use util_global_recover_energy_state */
    ram_low_pwr    = low_pwr;
    mapotn_energy_state->tx_top = util_global_recover_energy_state(low_pwr,ram_low_pwr,soft_reset);

    PMC_RETURN();
} /* mapotn_energy_state_reg_get */


/*******************************************************************************
*  mapotn_rgfpf_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the RGFPF module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of RGFPF
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
PRIVATE PMC_ERROR mapotn_rgfpf_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                    pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_rx_field_RGFPF96_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_rx_field_RGFPF96_RAM_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_rx_field_RGFPF96_SOFT_RST_set( NULL, mapotn_handle, 0 );
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_rx_field_RGFPF96_SOFT_RST_set( NULL, mapotn_handle, 1 );
            mapotn_rx_field_RGFPF96_LOWPWR_set( NULL, mapotn_handle, 1 );
            mapotn_rx_field_RGFPF96_RAM_LOWPWR_set( NULL, mapotn_handle, 1 );
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = mapotn_rgfpf_init(mapotn_handle, PMC_ENERGY_STATE_RESET);
    }

    PMC_RETURN(ret_val);

} /* mapotn_rgfpf_energy_state_reg_set */


/*******************************************************************************
*  mapotn_mpma_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the MPMA module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of MPMA
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
PRIVATE PMC_ERROR mapotn_mpma_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_rx_field_MPMA_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_rx_field_MPMA_RAM_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_rx_field_MPMA_SOFT_RST_set(NULL, mapotn_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_rx_field_MPMA_SOFT_RST_set(NULL, mapotn_handle, 1);
            mapotn_rx_field_MPMA_LOWPWR_set(NULL, mapotn_handle, 1);
            mapotn_rx_field_MPMA_RAM_LOWPWR_set(NULL, mapotn_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = mpma_init(mapotn_handle->mpma_handle, PMC_ENERGY_STATE_RESET);
    }

    PMC_RETURN(ret_val);

} /* mapotn_mpma_energy_state_reg_set */ 


/*******************************************************************************
*  mapotn_rgmp_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the RGMP module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of RGMP
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
PRIVATE PMC_ERROR mapotn_rgmp_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_rx_field_RGMP96_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_rx_field_RGMP96_RAM_LOWPWR_set( NULL, mapotn_handle, 0);
            mapotn_rx_field_RGMP96_SOFT_RST_set( NULL, mapotn_handle, 0 );
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_rx_field_RGMP96_SOFT_RST_set( NULL, mapotn_handle, 1 );
            mapotn_rx_field_RGMP96_LOWPWR_set( NULL, mapotn_handle, 1 );
            mapotn_rx_field_RGMP96_RAM_LOWPWR_set( NULL, mapotn_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = rgmp_init(mapotn_handle->rgmp_handle, PMC_ENERGY_STATE_RESET); 
    }

    PMC_RETURN(ret_val);

} /* mapotn_rgmp_energy_state_reg_set */ 


/*******************************************************************************
*  mapotn_demapper_soft_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures reset mode for MAPOTN RX. 
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - enerty state of MAPOTN RX\n
*                         0 -- MAPOTN RX macro is not in reset mode\n
*                         1 -- MAPOTN RX macro is in reset mode\n
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
PRIVATE PMC_ERROR mapotn_demapper_soft_reset_cfg(mapotn_handle_t *mapotn_handle,
                                                 pmc_energy_state_t energy_state)
{   
    /* Variable declaration */

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_rx_field_TOP_RAM_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_rx_field_MAPOTN_RX_SOFT_RST_set(NULL, mapotn_handle, 0);
            mapotn_rx_field_FIFO_RSTB_set(NULL, mapotn_handle, 0x1);
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_rx_field_MAPOTN_RX_SOFT_RST_set(NULL, mapotn_handle, 1);
            mapotn_rx_field_FIFO_RSTB_set(NULL, mapotn_handle, 0x0);
            mapotn_rx_field_TOP_RAM_LOWPWR_set(NULL, mapotn_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_demapper_soft_reset_cfg */

/*******************************************************************************
*  mapotn_tgfpf_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the TGFPF module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of TGFPF
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
PRIVATE PMC_ERROR mapotn_tgfpf_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                    pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_tx_field_TGFPF96_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_tx_field_TGFPF96_RAM_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_tx_field_TGFPF96_SOFT_RST_set( NULL, mapotn_handle, 0 );
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_tx_field_TGFPF96_SOFT_RST_set( NULL, mapotn_handle, 1 );
            mapotn_tx_field_TGFPF96_LOWPWR_set( NULL, mapotn_handle, 1 );
            mapotn_tx_field_TGFPF96_RAM_LOWPWR_set( NULL, mapotn_handle, 1 );
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = mapotn_tgfpf_init(mapotn_handle, PMC_ENERGY_STATE_RESET); 
    }

    PMC_RETURN(ret_val);

} /* mapotn_tgfpf_energy_state_reg_set */
   
/*******************************************************************************
*  mapotn_tgmp_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the TGMP module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of TGMP
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
PRIVATE PMC_ERROR mapotn_tgmp_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_tx_field_TGMP96_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_tx_field_TGMP96_RAM_LOWPWR_set( NULL, mapotn_handle, 0 );
            mapotn_tx_field_TGMP96_SOFT_RST_set( NULL, mapotn_handle, 0 );
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_tx_field_TGMP96_SOFT_RST_set( NULL, mapotn_handle, 1 );
            mapotn_tx_field_TGMP96_LOWPWR_set( NULL, mapotn_handle, 1 );
            mapotn_tx_field_TGMP96_RAM_LOWPWR_set( NULL, mapotn_handle, 1 );
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = tgmp_init(mapotn_handle->tgmp_handle, PMC_ENERGY_STATE_RESET); 
    }

    PMC_RETURN(ret_val);

} /* mapotn_tgmp_energy_state_reg_set */   


/*******************************************************************************
*  mapotn_mpmo_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the MPMO module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of MPMO
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
PRIVATE PMC_ERROR mapotn_mpmo_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                   pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_tx_field_MPMO_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_tx_field_MPMO_RAM_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_tx_field_MPMO_SOFT_RST_set(NULL, mapotn_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_tx_field_MPMO_SOFT_RST_set(NULL, mapotn_handle, 1);
            mapotn_tx_field_MPMO_LOWPWR_set(NULL, mapotn_handle, 1);
            mapotn_tx_field_MPMO_RAM_LOWPWR_set(NULL, mapotn_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = mpmo_init(mapotn_handle->mpmo_handle, PMC_ENERGY_STATE_RESET);
    }

    PMC_RETURN(ret_val);

} /* mapotn_mpmo_energy_state_reg_set */  


/*******************************************************************************
*  mapotn_oduksc_energy_state_reg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the energy state for the ODUKSC module.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - Energy state of ODUKSC
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
PRIVATE PMC_ERROR mapotn_oduksc_energy_state_reg_set(mapotn_handle_t *mapotn_handle, 
                                                     pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();    
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_tx_field_ODUKSC_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_tx_field_ODUKSC_RAM_LOWPWR_set(NULL, mapotn_handle, 0);
            mapotn_tx_field_ODUKSC_SOFT_RST_set(NULL, mapotn_handle, 0);
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_tx_field_ODUKSC_SOFT_RST_set(NULL, mapotn_handle, 1);
            mapotn_tx_field_ODUKSC_LOWPWR_set(NULL, mapotn_handle, 1);
            mapotn_tx_field_ODUKSC_RAM_LOWPWR_set(NULL, mapotn_handle, 1);
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }

    if(energy_state == PMC_ENERGY_STATE_RESET)
    {
        /* protect us from any perspective reset all is made */
        ret_val = oduksc_init(mapotn_handle->oduksc_handle, PMC_ENERGY_STATE_RESET);
    }

    PMC_RETURN(ret_val);

} /* mapotn_mpmo_energy_state_reg_set */


/*******************************************************************************
*  mapotn_mapper_soft_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures reset mode for MAPOTN TX. 
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   energy_state        - energy state of MAPOTN TX\n
*                         0 -- MAPOTN RX macro is not in reset mode\n
*                         1 -- MAPOTN RX macro is in reset mode 
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
PRIVATE PMC_ERROR mapotn_mapper_soft_reset_cfg(mapotn_handle_t *mapotn_handle,
                                               pmc_energy_state_t energy_state)
{   
    /* Variable declaration */

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch(energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            mapotn_tx_field_MAPOTN_TX_SOFT_RST_set(NULL, mapotn_handle, 0);
            mapotn_tx_field_FIFO_CLRB_set(NULL,mapotn_handle,0x1);
            break;
        case PMC_ENERGY_STATE_RESET:
            mapotn_tx_field_MAPOTN_TX_SOFT_RST_set(NULL, mapotn_handle, 1);
            mapotn_tx_field_FIFO_CLRB_set(NULL,mapotn_handle,0x0);
            break;
        default:
            PMC_ASSERT(FALSE, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_mapper_soft_reset_cfg */  

/* LCOV_EXCL_START */
/*******************************************************************************
*  mapotn_db_chnl_rec_add
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Adds the records of a channel to the database.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*   num_cal_entries     - number of calendar enrties required by the channel
*   db_id               - database ID to access
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
PUBLIC PMC_ERROR mapotn_db_chnl_rec_add(mapotn_handle_t *mapotn_handle, 
                                        UINT32 chnl, 
                                        UINT32 num_cal_entries, 
                                        util_gen_db_id_t db_id)
{   
    /* Variable declaration */
    UINT32 j;
    UINT32 db_handle;
    UINT32 *available_rec_array;
    mapotn_db_rec rec;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* argument checking */
    switch (db_id)
    {
        case UTIL_GEN_DB_ODUKSC_DB: 
            db_handle = mapotn_handle->oduksc_db_handle;
            break;
        case UTIL_GEN_DB_MPMA_DB: 
            db_handle = mapotn_handle->mpma_db_handle;
            break;
        case UTIL_GEN_DB_MPMO_DB: 
            db_handle = mapotn_handle->mpmo_db_handle;
            break;
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    
    
    available_rec_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*mapotn_handle->cfg.num_mapotn_chanl);
    PMC_ASSERT(available_rec_array != NULL, MAPOTN_ERR_CODE_ASSERT, 0, 0);
          
    ret_val = mapotn_db_empty_rec_get(mapotn_handle, num_cal_entries, db_id, db_handle, available_rec_array);
    if (ret_val != PMC_SUCCESS) {
        PMC_FREE(&available_rec_array);
        PMC_RETURN(ret_val);  /* if not enough available records return an error */
    }
    
    /* add the record to the database */
    
    for(j=0; j<num_cal_entries; j++)
    {
        rec.channel_num = chnl;
        rec.calendar_entry = available_rec_array[j];
        ret_val = gen_db_add_record(&(mapotn_handle->base), db_id, db_handle, (void *)&rec);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&available_rec_array);
            PMC_RETURN(ret_val);
        } 
    }                          
    
    PMC_FREE(&available_rec_array);
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_db_chnl_rec_add */


/*******************************************************************************
*  mapotn_db_chnl_rec_rem
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Removes the records of a channel from the database.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95  
*   db_id               - database ID to access
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
PUBLIC PMC_ERROR mapotn_db_chnl_rec_rem(mapotn_handle_t *mapotn_handle, 
                                        UINT32 chnl,
                                        util_gen_db_id_t db_id)
{   
    /* Variable declaration */
    UINT32 i;
    UINT32 db_handle;
    mapotn_db_key   key;
    UINT32          num_of_recs;
    mapotn_db_query_res *q_result;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* argument checking */
    switch (db_id)
    {
        case UTIL_GEN_DB_ODUKSC_DB: 
            db_handle = mapotn_handle->oduksc_db_handle;
            break;
        case UTIL_GEN_DB_MPMA_DB: 
            db_handle = mapotn_handle->mpma_db_handle;
            break;
        case UTIL_GEN_DB_MPMO_DB: 
            db_handle = mapotn_handle->mpmo_db_handle;
            break;
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    
    
    /* remove the records from the database */
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {  
        key.calendar_entry = DO_NOT_USE_KEY;
        key.channel_num = chnl;
        ret_val = gen_db_rem_record(&(mapotn_handle->base), db_id, db_handle, (void *)&key);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }    
    
    /*print db*/
    PMC_LOG_TRACE("printDB after removing records for channel:%d\n", chnl);
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {  
        key.calendar_entry = i;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs=gen_db_query(&(mapotn_handle->base),db_id,db_handle, (void *)&key,(void **)&q_result);
        
        if(num_of_recs == 0)
        {
            PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
        }
        
        PMC_LOG_TRACE("1:n query result: i= %d, channel=0x%x,cal entry=0x%x,  %s\n",
            i,q_result->p_recs[0]->channel_num,q_result->p_recs[0]->calendar_entry, __FILE__);   
                                     
    }              
   
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_db_chnl_rec_rem */

/* LCOV_EXCL_STOP */

/*******************************************************************************
*  mapotn_db_empty_rec_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns an array of indices of empty records and checks if
*   there is enough empty records left for the num calendar entries.
*   
*
* INPUTS:
*   *mapotn_handle        - pointer to MAPOTN handle instance
*   num_cal_entries       - number of calendar enrties required by the channel
*   db_id                 - database ID to access
*   db_handle             - handle to the database                   
*
* OUTPUTS:
*   *available_rec_array  - a pointer to array of empty indices in the DB 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_db_empty_rec_get(mapotn_handle_t *mapotn_handle, 
                                         UINT32 num_cal_entries,
                                         util_gen_db_id_t db_id,
                                         UINT32 db_handle, 
                                         UINT32 *available_rec_array)
{   
    /* Variable declaration */
    UINT32 i;
    UINT32 available_rec_count;
    mapotn_db_key   key;
    UINT32          num_of_recs;
    mapotn_db_query_res *q_result;
    
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* initialize the array*/
    for(i = 0; i < MAPOTN_NUM_CHANL; i++)  
    { 
        available_rec_array[i]= MAPOTN_NULL_DB_REC;
    }                                         
    
    /* get the list of all empty indices and store them in the array */
    available_rec_count = 0;
    for(i = 0; i < MAPOTN_NUM_CHANL; i++)
    { 
        key.calendar_entry = i;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs=gen_db_query(&(mapotn_handle->base),db_id,db_handle, (void *)&key,(void **)&q_result);
        
        if(num_of_recs == 0)
        {
            PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
        }

        PMC_LOG_TRACE("1:n query result: i= %d, channel=0x%x,cal entry=0x%x, %s\n",
            i,q_result->p_recs[0]->channel_num,q_result->p_recs[0]->calendar_entry,  __FILE__);
        
        if(q_result->p_recs[0]->channel_num == 0xffffffff)
        {
            available_rec_array[available_rec_count] = i;
            available_rec_count++;
        }
    }
    
    PMC_LOG_TRACE( "available_rec_count = %u \n", available_rec_count);
    
    /* check if enough number of pages are available */
    if(num_cal_entries > available_rec_count)
    {
        /* return error the not enough pages available */
        PMC_RETURN(MAPOTN_ERR_DB_RECS_UNAVAILABLE);    
    }
    
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_db_empty_rec_get */


/*******************************************************************************
*  mapotn_db_chnl_num_entries_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the number of calendar entries for a channel and a 
*   database 
*   
*
* INPUTS:
*   *mapotn_handle        - pointer to MAPOTN handle instance
*   chnl                  - the channel ID
*   db_id                 - database ID to access
*
* OUTPUTS:
*   *num_cal_entries      - number of calendar enrties required by the channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_db_chnl_num_entries_get(mapotn_handle_t *mapotn_handle, 
                                                UINT32 chnl,                                                  
                                                util_gen_db_id_t db_id,
                                                UINT32 *num_cal_entries)
{
    mapotn_db_key key;
    UINT32 db_handle;

    PMC_ENTRY();

    /* argument checking */
    switch (db_id)
    {
        case UTIL_GEN_DB_ODUKSC_DB: 
            db_handle = mapotn_handle->oduksc_db_handle;
            break;
        case UTIL_GEN_DB_MPMA_DB: 
            db_handle = mapotn_handle->mpma_db_handle;
            break;
        case UTIL_GEN_DB_MPMO_DB: 
            db_handle = mapotn_handle->mpmo_db_handle;
            break;
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
      
    key.calendar_entry = DO_NOT_USE_KEY;
    key.channel_num = chnl;
    *num_cal_entries = gen_db_query_count(&(mapotn_handle->base),db_id, db_handle, (void *)&key);
    
    if(*num_cal_entries == 0)
    {
        PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
    }
    
           
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_db_chnl_num_entries_get */


/*******************************************************************************
*  mapotn_db_chnl_entry_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Get calendar entries for a channel
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   db_id               - database ID to access
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*   num_cal_entries     - number of calendar enrties required by the channel
*
* OUTPUTS:
*   *cal_entries        - Calendar entries
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_db_chnl_entry_get(mapotn_handle_t *mapotn_handle, 
                                          util_gen_db_id_t db_id,
                                          UINT32 chnl, 
                                          UINT32 num_cal_entries, 
                                          UINT32 *cal_entries)                                          
{   
    /* Variable declaration */
    UINT32 i,cal_itr;
    UINT32 db_handle;
    mapotn_db_key   key;
    UINT32          num_of_recs;
    mapotn_db_query_res *q_result;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(mapotn_handle->cfg.db_mode != MAPOTN_DB_MODE_EXTERNAL)
    {
        PMC_RETURN(MAPOTN_ERR_DB_EXT_ACCESS);
    }
    
    /* argument checking */
    switch (db_id)
    {
        case UTIL_GEN_DB_ODUKSC_DB: 
            db_handle = mapotn_handle->oduksc_db_handle;
            break;
        case UTIL_GEN_DB_MPMA_DB: 
            db_handle = mapotn_handle->mpma_db_handle;
            break;
        case UTIL_GEN_DB_MPMO_DB: 
            db_handle = mapotn_handle->mpmo_db_handle;
            break;
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    cal_itr = 0;
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {  
        key.calendar_entry = i;
        key.channel_num = DO_NOT_USE_KEY;
        num_of_recs=gen_db_query(&(mapotn_handle->base),db_id,db_handle, (void *)&key,(void **)&q_result);
        
        if(num_of_recs == 0)
        {
            PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
        }
        if (q_result->p_recs[0]->channel_num == chnl)
        {
            cal_entries[cal_itr] = i;
            cal_itr++;
        }
    }
        
    PMC_RETURN(PMC_SUCCESS);  

} /* mapotn_db_chnl_entry_get */




/*******************************************************************************
*  mapotn_mapper_mpmo_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Determines if the MPMO configuration is required for a specific channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   *mpmo_cfg_req       - MPMO configuration required or not\n
*                         TRUE  -- MPMO configuration required\n
*                         FALSE -- MPMO configuration not required\n
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_mapper_mpmo_cfg_get(mapotn_handle_t *mapotn_handle,
                                            UINT32 chnl,
                                            BOOL8 *mpmo_cfg_req)
{   
    /* Variable declaration */
    
    PMC_ENTRY();
    
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_BMP || 
       mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_GMP ||
       mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_AMP)
    {
        *mpmo_cfg_req = TRUE;    
    }
    else {
        *mpmo_cfg_req = FALSE; 
    }
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_mapper_mpmo_cfg_get */


/*******************************************************************************
*  mapotn_mpma_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Gets MPMA packet size and packet period information for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   *pkt_size_ptr       - pointer to configured packet size
*   *pkt_per_int_ptr    - pointer to configured integer portion of packet period
*   *pkt_per_n_ptr      - pointer to configured numerator of packet period 
*                         fraction
*   *pkt_per_d_ptr      - pointer to configured denominator of packet period 
*                         fraction
*   *epsilon_ptr        - pointer to configured minimum epsilon
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_mpma_data_get(mapotn_handle_t *mapotn_handle,
                                      UINT32 chnl,
                                      UINT32 *pkt_size_ptr,
                                      UINT32 *pkt_per_int_ptr,
                                      UINT32 *pkt_per_n_ptr,
                                      UINT32 *pkt_per_d_ptr,
                                      DOUBLE *epsilon_ptr)
{   
    /* Variable declaration */
    UINT32 ps_ptr; 
    UINT32 pp_int_ptr;  
    UINT32 pp_n_ptr;    
    UINT32 pp_d_ptr;  
    DOUBLE eps_ptr;  
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    ret_val = mpma_pkt_period_var_get(mapotn_handle->mpma_handle, chnl, 
                                      &ps_ptr,&pp_int_ptr,&pp_n_ptr,&pp_d_ptr,&eps_ptr);                                       
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    } 
    
    *pkt_size_ptr = ps_ptr; 
    *pkt_per_int_ptr = pp_int_ptr;                         
    *pkt_per_n_ptr = pp_n_ptr;
    *pkt_per_d_ptr = pp_d_ptr;
    *epsilon_ptr = eps_ptr;
    
    PMC_LOG_TRACE("ps_ptr=%d  pp_int_ptr=%d  pp_n_ptr=%d  pp_d_ptr=%d  eps_ptr=%f\n", ps_ptr,pp_int_ptr,pp_n_ptr,pp_d_ptr,eps_ptr);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_mpma_data_get */


/*******************************************************************************
*  mapotn_mapper_mpmo_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Configures MPMO channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*   pkt_size            - packet size configured at the packet maker
*   pkt_per_int         - integer portion for packet period configured at the 
*                         packet maker
*   pkt_per_n           - numerator for packet period fraction configured at the 
*                         packet maker
*   pkt_per_d           - denominator for packet period fraction configured at  
*                         the packet maker
*   is_sonet            - TRUE : Is CBRC SONET
*                         FALSE : Is not CBRC SONET 
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
PUBLIC PMC_ERROR mapotn_mapper_mpmo_prov(mapotn_handle_t *mapotn_handle,
                                         UINT32 chnl,
                                         UINT32 pkt_size,
                                         UINT32 pkt_per_int,
                                         UINT32 pkt_per_n,
                                         UINT32 pkt_per_d,
                                         BOOL8  is_sonet)
{   
    /* Variable declaration */
    util_opsa_cn_frame_period_t cn_frame_period;
    BOOL8 golden_cn_en;
    BOOL8 bmp_map_mode = FALSE;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    ret_val = mapotn_cn_frame_period_get(mapotn_handle,mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type, &cn_frame_period);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    if(mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest == MAPOTN_DEST_ENET)
    {
        pkt_size = 48;
        golden_cn_en = 1;
    }
    else
    {
        golden_cn_en = 0;
    }
    
    if(mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_BMP)
    {
        /* for BMP mapped disable Cn Generation*/
        cn_frame_period = UTIL_OPSA_T_FRAME_UNUSED;
    }
    
    if(mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode == UTIL_GLOBAL_BMP && mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest == MAPOTN_DEST_CPB)
    {
        /* for BMP mapped from CPB use MPMO BMP mapping settings. */
        bmp_map_mode = TRUE;
        /* configure the CFC_FILL_LVL */
        ret_val = mapotn_mapper_internal_tgmp_cfc_xoff_cfg(mapotn_handle, chnl, FALSE, FALSE);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

    }
    
    ret_val = mpmo_mapotn_ch_prov(mapotn_handle->mpmo_handle, chnl, mapotn_handle->var.tx_chnl_ctxt[chnl].client_rate, 
                                  pkt_size, pkt_per_int, pkt_per_n, pkt_per_d, cn_frame_period, golden_cn_en, bmp_map_mode,  is_sonet);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_mapper_mpmo_prov */


/*******************************************************************************
*  mapotn_mapper_mpmo_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Deprovisiones MPMO channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
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
PUBLIC PMC_ERROR mapotn_mapper_mpmo_deprov(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl)
{   
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    
    ret_val = mpmo_ch_deprov(mapotn_handle->mpmo_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_mapper_mpmo_deprov */

/* LCOV_EXCL_START */
/*******************************************************************************
*  mapotn_mpma_pkt_period_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Calculates MPMA packet size and packet period information for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *pkt_size_ptr       - pointer to packet size
*   *pkt_per_int_ptr    - pointer to integer portion of packet period
*   *pkt_per_n_ptr      - pointer to numerator of packet period fraction
*   *pkt_per_d_ptr      - pointer to denominator of packet period fraction
*   *epsilon_ptr        - pointer to minimum epsilon 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_mpma_pkt_period_calc(mapotn_handle_t *mapotn_handle,
                                             UINT32 chnl, 
                                             UINT32 *pkt_size_ptr,
                                             UINT32 *pkt_per_int_ptr,
                                             UINT32 *pkt_per_n_ptr,
                                             UINT32 *pkt_per_d_ptr,
                                             DOUBLE *epsilon_ptr)
{   
    /* Variable declaration */
    util_opsa_cn_frame_period_t cn_frame_period; 
    UINT32 pkt_size;
    UINT32 pkt_size_range;
    util_opsa_pkt_period_calc_t pkt_period_calc;
    
    UINT32 pkt_size_loc;
    UINT32 pkt_per_int_loc;
    UINT32 pkt_per_n_loc;
    UINT32 pkt_per_d_loc;
    DOUBLE eps_loc;
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    pkt_size = (mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP) ? MAPOTN_MPMA_PKT_SIZE:(511-16);
    pkt_size_range = (mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP) ? MAPOTN_MPMA_PKT_SIZE_RANGE:16;
    pkt_period_calc = UTIL_OPSA_EPS_MIN;

    ret_val = mapotn_cn_frame_period_get(mapotn_handle,mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type, &cn_frame_period);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    ret_val = mpma_pkt_period_calc(mapotn_handle->mpma_handle, mapotn_handle->var.tx_chnl_ctxt[chnl].client_rate, pkt_size, 
                                   pkt_size_range, cn_frame_period,
                                   pkt_period_calc, &pkt_size_loc, &pkt_per_int_loc, 
                                   &pkt_per_n_loc, &pkt_per_d_loc, &eps_loc);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }    
    
    *pkt_size_ptr = pkt_size_loc;  
    *pkt_per_int_ptr = pkt_per_int_loc; 
    *pkt_per_n_ptr = pkt_per_n_loc;
    *pkt_per_d_ptr = pkt_per_d_loc;  
    *epsilon_ptr = eps_loc;     
    
    PMC_LOG_TRACE("pkt_size_loc=%d, pkt_per_int_loc=%d, pkt_per_n_loc=%d, pkt_per_d_loc=%d, eps_loc=%f\n", pkt_size_loc, pkt_per_int_loc, pkt_per_n_loc, pkt_per_d_loc, eps_loc);
    
      
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_mpma_pkt_period_calc */
/* LCOV_EXCL_STOP */

/*******************************************************************************
*  mapotn_cn_frame_period_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Retrieves Cn frame period for a specific ODUk type.           
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   odukp_type          - ODUkp or ODUjp type\n 
*                         Refer to util_global_odukp_type_t, ODUkp all ODUk
*                         types valid, ODUjp only ODU2/3 types valid
*
* OUTPUTS:
*   *cn_frame_period    - pointer to Cn frame period
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_cn_frame_period_get(mapotn_handle_t *mapotn_handle,
                                            util_global_odukp_type_t odukp_type, 
                                            util_opsa_cn_frame_period_t *cn_frame_period)
{   
    /* Variable declaration */
    util_opsa_cn_frame_period_t cn_frame_pr; 
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(odukp_type < UTIL_GLOBAL_LAST_ODUK, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    switch (odukp_type)
    {
        case UTIL_GLOBAL_ODUFLEX_GFP: 
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME;
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR: 
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODUFLEX_T_FRAME;
            break;
        case UTIL_GLOBAL_ODU0: 
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU0_T_FRAME;
            break;
        case UTIL_GLOBAL_ODU1:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU1_T_FRAME;
            break;
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU1E_T_FRAME;
            break; 
        case UTIL_GLOBAL_ODU1F:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU1F_T_FRAME;
            break;  
        case UTIL_GLOBAL_ODU2:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU2_T_FRAME;
            break;
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU2E_T_FRAME;
            break;     
        case UTIL_GLOBAL_ODU2F:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU2F_T_FRAME;
            break;
        case UTIL_GLOBAL_ODU3:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU3_T_FRAME;
            break;  
        case UTIL_GLOBAL_ODU3E1:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU3E1_T_FRAME;
            break;                                      
        case UTIL_GLOBAL_ODU3E2:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU3E2_T_FRAME;
            break; 
        case UTIL_GLOBAL_ODU4:
            cn_frame_pr = UTIL_OPSA_MAPOTN_ODU4_T_FRAME;
            break;     
        default:
            PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
            break;
    }
    
      
    *cn_frame_period = cn_frame_pr;     
      
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_cn_frame_period_get */


/*******************************************************************************
*  mapotn_ohfs_insert_chnl_payload_type_set
* ______________________________________________________________________________
*
* DESCRIPTION:
* Sets OPU payload type field for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   pt                  - payload type of the ODU payload
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
PUBLIC PMC_ERROR mapotn_ohfs_insert_chnl_payload_type_set(mapotn_handle_t *mapotn_handle,
                                                          UINT32 chnl,
                                                          UINT8 pt)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    if (UTIL_GLOBAL_GFP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_insert_pt_cfg(mapotn_handle->ohfs_insert_handle, chnl, pt);
    }
    else if (UTIL_GLOBAL_GMP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_AMP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_BMP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_insert_pt_cfg(mapotn_handle->tgmp_handle->ohfs_insert_handle, chnl, pt);
    }
    else
    {
        ret_val = MAPOTN_ERR_INVALID_MAPPING_MODE;
    }

    PMC_RETURN(ret_val);
} /* mapotn_ohfs_insert_chnl_payload_type_set */


/*******************************************************************************
*  mapotn_ohfs_insert_chnl_payload_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
* Retrieves OPU payload type field for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *pt                 - pointer to storage for payload type of the OPU payload
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_ohfs_insert_chnl_payload_type_get(mapotn_handle_t *mapotn_handle,
                                                          UINT32 chnl,
                                                          UINT8 *pt)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (UTIL_GLOBAL_GFP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_insert_pt_get(mapotn_handle->ohfs_insert_handle, chnl, pt);
    }
    else if (UTIL_GLOBAL_GMP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_AMP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_BMP == mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_insert_pt_get(mapotn_handle->tgmp_handle->ohfs_insert_handle, chnl, pt);
    }
    else
    {
        ret_val = MAPOTN_ERR_INVALID_MAPPING_MODE;
    }

    PMC_RETURN(ret_val);
} /* mapotn_ohfs_insert_chnl_payload_type_get */


/*******************************************************************************
*  mapotn_ohfs_remove_chnl_payload_type_set
* ______________________________________________________________________________
*
* DESCRIPTION:
* Sets expected OPU payload type field for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   pt                  - payload type of the ODU payload
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
PUBLIC PMC_ERROR mapotn_ohfs_remove_chnl_payload_type_set(mapotn_handle_t *mapotn_handle,
                                                          UINT32 chnl,
                                                          UINT8 pt)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)
    {  
        ret_val = ohfs_remove_pt_cfg(mapotn_handle->ohfs_remove_handle, chnl, pt);
    }
    else if (UTIL_GLOBAL_GMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_AMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_BMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)    
    {
        ret_val = ohfs_remove_pt_cfg(mapotn_handle->rgmp_handle->ohfs_remove_handle, chnl, pt);
    }
    else
    {
        ret_val = MAPOTN_ERR_INVALID_MAPPING_MODE;
    }

    PMC_RETURN(ret_val);
} /* mapotn_ohfs_remove_chnl_payload_type_set */


/*******************************************************************************
*  mapotn_ohfs_remove_chnl_accpt_payload_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
* Retrieves accepted OPU payload type field for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *accpt_pt           - pointer to storage for payload type of the OPU payload
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_ohfs_remove_chnl_accpt_payload_type_get(mapotn_handle_t *mapotn_handle,
                                                                UINT32 chnl,
                                                                UINT8 *accpt_pt)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_remove_accpt_pt_get(mapotn_handle->ohfs_remove_handle, chnl, accpt_pt);
        if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE && ret_val == PMC_SUCCESS)
        {
            ret_val = ohfs_remove_accpt_pt_get(mapotn_handle->rgmp_handle->ohfs_remove_handle, chnl, accpt_pt);
        }
    }
    else if (UTIL_GLOBAL_GMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_AMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_BMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode) 
    {
        ret_val = ohfs_remove_accpt_pt_get(mapotn_handle->rgmp_handle->ohfs_remove_handle, chnl, accpt_pt);
    }
    else
    {
        ret_val = MAPOTN_ERR_INVALID_MAPPING_MODE;
    }

    PMC_RETURN(ret_val);
} /* mapotn_ohfs_remove_chnl_accpt_payload_type_get */


/*******************************************************************************
*  mapotn_ohfs_remove_chnl_exp_payload_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
* Retrieves expected OPU payload type field for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *exp_pt             - pointer to storage for payload type of the OPU payload
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_ohfs_remove_chnl_exp_payload_type_get(mapotn_handle_t *mapotn_handle,
                                                              UINT32 chnl,
                                                              UINT8 *exp_pt)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (UTIL_GLOBAL_GFP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_remove_exp_pt_get(mapotn_handle->ohfs_remove_handle, chnl, exp_pt);
    }
    else if (UTIL_GLOBAL_GMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_AMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode ||
             UTIL_GLOBAL_BMP == mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode)
    {
        ret_val = ohfs_remove_exp_pt_get(mapotn_handle->rgmp_handle->ohfs_remove_handle, chnl, exp_pt);
    }
    else
    {
        ret_val = MAPOTN_ERR_INVALID_MAPPING_MODE;
    }
    
    PMC_RETURN(ret_val);
} /* mapotn_ohfs_remove_chnl_exp_payload_type_get */

/*******************************************************************************
* mapotn_adjust_dci_clock
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Performs MPMO/ODUKSC adjectments given an integer numerator and
*   denominator multiplier on the DCI clock frequency.
*                                                                              
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   num_mult            - numerator to be applied to the DCI clock frequency
*   denom_mult          - denominator to be applied to the DCI clock frequency   
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
PUBLIC PMC_ERROR mapotn_adjust_dci_clock(mapotn_handle_t *mapotn_handle,
                                          UINT32 num_mult,
                                          UINT32 denom_mult)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    

    result = mpmo_adjust_dci_clk(mapotn_handle->mpmo_handle,
                                  num_mult,
                                  denom_mult);

    if(result == PMC_SUCCESS)
    {

        result = oduksc_adjust_dci_clk(mapotn_handle->oduksc_handle,
                                       denom_mult,
                                       num_mult);
    }

    
    PMC_RETURN(result);

} /* mapotn_adjust_dci_clock  */


/*******************************************************************************
* mapotn_null_frame_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Configures NULL frame mode in TGFPF/RGFPF or TGMP/RGMP
*                                                                              
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95  
*   dir                 - UTIL_GLOBAL_DIR_RX or
*                         UTIL_GLOBAL_DIR_TX or
*                         UTIL_GLOBAL_DIR_BOTH
*   null_frame_en_ptr      - pointer to null frame enable
*                            context (bitmask of size UINT32[3])
*   prbs_tx_null_frame_en_ptr  - pointer NULL frame enable for
*                            PRBS is enabled at FO2 context
*                            (bitmask of size UINT32[3])
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
PUBLIC PMC_ERROR mapotn_null_frame_cfg(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl,
                                       util_global_direction_t dir,
                                       UINT32 *null_frame_en_ptr,
                                       UINT32 *prbs_tx_null_frame_en_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    util_global_mapping_mode_t mapotn_mapping_mode;
    BOOL null_frame_en;
    BOOL prbs_tx_null_frame_en;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_mapping_mode = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode;
    
    null_frame_en = (null_frame_en_ptr[chnl/32] >> (chnl%32)) & 0x1;
    prbs_tx_null_frame_en = (prbs_tx_null_frame_en_ptr[chnl/32] >> (chnl%32)) & 0x1;

    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        if(dir == UTIL_GLOBAL_DIR_TX ||
           dir == UTIL_GLOBAL_DIR_BOTH)
        {
            /* Configure TGFPF channel */
            result = ohfs_insert_null_frame_cfg(mapotn_handle->ohfs_insert_handle,
                                                chnl,
                                                (null_frame_en || prbs_tx_null_frame_en));


        }

        if(dir == UTIL_GLOBAL_DIR_RX ||
           dir == UTIL_GLOBAL_DIR_BOTH)
        {
            /* Configure RGFPF channel */
            if(result == PMC_SUCCESS)
            {
                result = ohfs_remove_func_mode_cfg(mapotn_handle->ohfs_remove_handle,
                                                   chnl,
                                                   null_frame_en);
            }
        }

    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
             mapotn_mapping_mode == UTIL_GLOBAL_AMP ) 
    {

        if(dir == UTIL_GLOBAL_DIR_TX ||
           dir == UTIL_GLOBAL_DIR_BOTH)
        {
            /* Configure TGMP channel */
            result = ohfs_insert_null_frame_cfg(mapotn_handle->tgmp_handle->ohfs_insert_handle,
                                                chnl,
                                                (null_frame_en || prbs_tx_null_frame_en));
        }

        if(dir == UTIL_GLOBAL_DIR_RX ||
           dir == UTIL_GLOBAL_DIR_BOTH)
        {
            /* Configure RGMP channel */
            if(result == PMC_SUCCESS)
            {
                result = ohfs_remove_func_mode_cfg(mapotn_handle->rgmp_handle->ohfs_remove_handle,
                                                   chnl,
                                                   null_frame_en);
            }
        }

    }

    PMC_RETURN(result);

} /* mapotn_null_frame_cfg */


/*******************************************************************************
* mapotn_null_frame_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Gets NULL frame mode in TGFPF/RGFPF or TGMP/RGMP
*                                                                              
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                                                     
* OUTPUTS:
*   *tx_null_frame_en   - TRUE: OPU NULL payload is enabled in TX dir\n
*                         FALSE: OPU NULL payload is disabled  in TX DIR
*   *rx_null_frame_en   - TRUE: OPU NULL payload is enabled in RX dir\n
*                         FALSE: OPU NULL payload is disabled  in RX DIR
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                  
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_null_frame_get(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl,
                                       BOOL8 *tx_null_frame_en,
                                       BOOL8 *rx_null_frame_en)
{
    PMC_ERROR result = PMC_SUCCESS;
    util_global_mapping_mode_t mapotn_mapping_mode;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_mapping_mode = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode;
    
    if (mapotn_mapping_mode == UTIL_GLOBAL_GFP)
    {
        /* get NULL frame mode from TGFPF channel */
        if (NULL != tx_null_frame_en)
        {
            *tx_null_frame_en = ohfs_insert_null_frame_mode_get(mapotn_handle->ohfs_insert_handle,
                                                                chnl);
        }

        if (NULL != rx_null_frame_en)
        {
            *rx_null_frame_en = ohfs_remove_null_frame_mode_get(mapotn_handle->ohfs_remove_handle,
                                                                chnl);
        }
    }  
    else if (mapotn_mapping_mode == UTIL_GLOBAL_GMP || \
            mapotn_mapping_mode == UTIL_GLOBAL_BMP || \
            mapotn_mapping_mode == UTIL_GLOBAL_AMP )
    {

        /* Get NULL frame mode from TGMP channel */
        if (NULL != tx_null_frame_en)
        {
            *tx_null_frame_en = ohfs_insert_null_frame_mode_get(mapotn_handle->tgmp_handle->ohfs_insert_handle,
                                                                chnl);
        }

        if (NULL != rx_null_frame_en)
        {
            *rx_null_frame_en = ohfs_remove_null_frame_mode_get(mapotn_handle->rgmp_handle->ohfs_remove_handle,
                                                                chnl);
        }
    }
    else
    {
        if (NULL != tx_null_frame_en)
        {
            *tx_null_frame_en = FALSE;
        }

        if (NULL != rx_null_frame_en)
        {
            *rx_null_frame_en = FALSE;
        }
    }

    PMC_RETURN(result);

} /* mapotn_null_frame_get */

/*******************************************************************************
* FUNCTION: mapotn_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   mapping_mode        - mapping mode
*   enable              - Enables/disables interrupt and status support.
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
PRIVATE PMC_ERROR mapotn_int_init(mapotn_handle_t    *mapotn_handle,
                                  util_global_mapping_mode_t   mapping_mode,
                                  BOOL8             enable)

{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          i;
    mapotn_rx_buffer_t mapotn_rx_buffer[1];
    UINT32 top_int_en = (enable ? 1 : 0);
    UINT32 rgmp_int_en = 0, tgmp_int_en = 0, rgfpf_int_en = 0, tgfpf_int_en = 0;

    PMC_ENTRY();

    mapotn_rx_buffer_init(mapotn_rx_buffer,mapotn_handle); 

    /* 
     *  Only enable interrupts for a block if it is already enabled (based on the start state) or if
     *  it is in the process of being enabled (based on the mapping mode). 
     *
     *  If enable == FALSE -> disable all interrupts.
     */
    if (enable)
    {
        if (mapping_mode == UTIL_GLOBAL_GFP)
        {
            rgfpf_int_en = 1;
            tgfpf_int_en = 1;
        }
        else
        {
            rgfpf_int_en = (mapotn_rgfpf_start_state_test(mapotn_handle) == FALSE ? 1 : 0);
            tgfpf_int_en = (mapotn_tgfpf_start_state_test(mapotn_handle) == FALSE ? 1 : 0);
        }
    
        if (mapping_mode == UTIL_GLOBAL_GMP || mapping_mode == UTIL_GLOBAL_BMP || mapping_mode == UTIL_GLOBAL_AMP)
        {
            rgmp_int_en =  1;
            tgmp_int_en =  1;
        }
        else
        {
            rgmp_int_en =  (rgmp_start_state_test(mapotn_handle->rgmp_handle) == FALSE ? 1 : 0);
            tgmp_int_en =  (tgmp_start_state_test(mapotn_handle->tgmp_handle) == FALSE ? 1 : 0);
        }
    }
    
    
    /* Enable interrupts */
    for (i = 1; i < 3; i++)
    {
        mapotn_rx_field_MAPOTN_RX_TOP_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, top_int_en); 
        mapotn_rx_field_MAPOTN_TX_TOP_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, top_int_en); 
        mapotn_rx_field_RGFPF96_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, rgfpf_int_en); 
        mapotn_rx_field_RGMP96_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, rgmp_int_en);
        mapotn_rx_field_MPMA_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, (rgfpf_int_en | rgmp_int_en)); 
        mapotn_rx_field_TGFPF96_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, tgfpf_int_en);
        mapotn_rx_field_TGMP96_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, tgmp_int_en);
        mapotn_rx_field_ODUKSC_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, (tgfpf_int_en | tgmp_int_en)); 
        mapotn_rx_field_MPMO_INTR_EN_set(mapotn_rx_buffer,mapotn_handle,i, tgmp_int_en);
    }
    mapotn_rx_buffer_flush(mapotn_rx_buffer);

    PMC_RETURN(result);
} /* mapotn_int_init */

/* LCOV_EXCL_START */
/*******************************************************************************
* FUNCTION: mapotn_ordered_set_upi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MAPOTN ordered set UPI value for rgfpf/tgfpf.
*   
*
* INPUTS:
*   *mapotn_handle          - pointer to MAPOTN handle instance
*   tgfpf_ordered_set_upi   - TGFPF ordered set UPI value
*   rgfpf_ordered_set_upi   - RGFPF ordered set UPI value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES: This function may be called when MAPOTN is in reset - in which case
*        the function will store the UPI values in the mapotn context. When 
*        MAPOTN is brought out of reset, the UPI values will be configured.
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_ordered_set_upi_cfg(mapotn_handle_t *mapotn_handle,
                                             UINT8 tgfpf_ordered_set_upi,
                                             UINT8 rgfpf_ordered_set_upi)

{
    PMC_ERROR       result = PMC_SUCCESS;
    BOOL rgfpf_en, tgfpf_en;

    rgfpf_en = (mapotn_rgfpf_start_state_test(mapotn_handle) == FALSE);
    tgfpf_en = (mapotn_tgfpf_start_state_test(mapotn_handle) == FALSE);

    PMC_ENTRY();

    if (tgfpf_en)
    {
        (void)mapotn_tgfpf_order_set_upi_cfg(mapotn_handle, tgfpf_ordered_set_upi);
    }
    if (rgfpf_en)
    {
        (void)mapotn_rgfpf_ordered_set_upi_cfg(mapotn_handle, rgfpf_ordered_set_upi);
    }

    /* Save the UPI values */
    mapotn_handle->var.rgfpf_ordered_set_upi = rgfpf_ordered_set_upi;
    mapotn_handle->var.tgfpf_ordered_set_upi = tgfpf_ordered_set_upi;
    PMC_RETURN(result);
} /* mapotn_ordered_set_upi_cfg */
/* LCOV_EXCL_STOP */




/*******************************************************************************
* mapotn_is_etrans_in_path
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function determines if ETRANS in the path for any of the MAPOTN 
*   channels.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance   
*                                                                               
* OUTPUTS:
*   None.   
*
* RETURNS:
*   BOOL - TRUE if ETRANS is in the path for any of the MAPOTN channels, 
*   FALSE otherwise.                                                            
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL mapotn_is_etrans_in_path(mapotn_handle_t *mapotn_handle)
{
    /* Variable declaration */
    BOOL result = FALSE;
    UINT32 i;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {
        if(mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP ||
           mapotn_handle->var.tx_chnl_ctxt[i].adaptation_func == UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP)
        {
            result = TRUE;
            break;
        }
    }
    
    PMC_RETURN(result);
    
} /* mapotn_is_etrans_in_path */


/*******************************************************************************
* mapotn_retrieve_chnl_registers
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*  Determines the register addresses to be accessed as part of MAPOTN
*  interrupt retrieval and uses l5sys_burst_read to read the registers in a
*  burst. Then populates the internal cache with the results.
*                                                                               
* INPUTS:
*   mapotn_handle      - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   None.   
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void mapotn_retrieve_chnl_registers(mapotn_handle_t *mapotn_handle)
{
    /* Variable declaration */
    UINT32 reg_addrs[NUM_ADDRESSES];
    UINT32 data[NUM_ADDRESSES];
    UINT32 start_addr;
    UINT32 i;
    UINT32 index = 0;
    pmc_sys_handle_t *sys_handle = NULL;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    sys_handle = ((pmc_handle_t *)mapotn_handle)->sys_handle;
    PMC_MEMSET(reg_addrs, 0, NUM_ADDRESSES*sizeof(UINT32));
    PMC_MEMSET(data, 0, NUM_ADDRESSES*sizeof(UINT32));

    /*
     * For the interrupts defined in MAPOTN_INT_CHNL_TABLE_DEFINE which are
     * in the MAPOTN RGFPF class of registers the starting reg_addrs is the 
     * LOFD_I interrupt at 0x8303C8. There are 96 sets of these registers
     * bits corresponding to the number of MAPOTN channels.  The 3 32-bit
     * register reg_addrs are contiguous and are calculated based on the outer
     * loop. The inner loop maps to the number of register types being
     * access which are 12 bytes off of each other.  
     */
    for (i = 0; i < 3; i++)
    {
        UINT32 j = 0;

        /*start_addr = 0x8303C8;*/
        start_addr = ((pmc_handle_t *)mapotn_handle)->base_address + PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR(i);

        /*
         * There are 20 elements to be added here however there are 2 blanks
         * so need to skip them. there is nothing at the 11th entry,
         * 0x83044C. there is nothing at the 15th element that applies to
         * this table, CSF_ST_AV_I. there is an 8 byte shift before 
         * continuing along the 12 bbyte offset until element 20.
         */
        for (j = 0; j < 22; j++)
        {
            if (j == 11)
            {
                continue;
            }
            else if (j == 15)
            {
                start_addr += 8;
                continue;
            }
            reg_addrs[index++] = start_addr + j*12;  /* + i*4;*/
        }
        /*
         * Added in 3 explicit address values that do not form a specific
         * pattern
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR(i);
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR(i);
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_HAO_STAT_INT(i);
        /*
         * At address 0x8A0290 there are 6 registers to be added starting
         * with len_err_int_i @ 0x820290. These related to mapotn_tgfpf.
         * Start addess shou;d be 0x8A0290
         */
        start_addr = ((pmc_handle_t *)mapotn_handle)->base_address + PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT(i);
       
        for (j = 0; j < 6; j++)
        {
            reg_addrs[index++] = start_addr + j*16;
        }
        /*
         * This tacks on the 4 registers defined in 
         * MAPOTN_INT_CHNL_TABLE_REV_B_DEFINE table.
         *  Addresses are: 
         *    0x830558 + i*4
         *    0x830570 + i*4
         *    0x83057C + i*4
         *    0x8303BC + i*4
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT(i);
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_DPLM_INT(i);
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_OPU_CSF_INT(i);
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle)->base_address +
          PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR(i);

        /*
         * Now add status level _V related bits.
         * Starting address is 0x830590
         */
        start_addr = ((pmc_handle_t *)mapotn_handle)->base_address + PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE(i);

        for (j = 0; j < 9; j++)
        {
            reg_addrs[index++] = start_addr + j*12;
        }
        /*
         * Now retrieve the RGMP registers which also may be required.
         */
        for (j = 0; j < 12; j++)
        {
            if (0 == j)
            {
              /*start_addr = 0x810400;*/
                start_addr = ((pmc_handle_t *)mapotn_handle->rgmp_handle)->base_address + PMC_RGMP96_CORE_REG_DPLM_INT(i);
            }
            else if (9 == j)
            {
              /*start_addr = 0x810500;*/
                start_addr = ((pmc_handle_t *)mapotn_handle->rgmp_handle)->base_address + PMC_RGMP96_CORE_REG_DPLM_INT_VAL(i);
            }
            reg_addrs[index++] = start_addr + j*12;
        }
        /*
         * Added in TGMP txjc_res_ovrflw_i register
         */
        /*reg_addrs[index++] = 0x8C0220 + j*12 + i*4;*/
        
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle)->base_address + PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT(i);
        /*
         * Added in CFC registers, FIFO_OVR_I, FIFO_UDR_I, and
         * PTP_ERR_I.  
         * Addresses are: 0x8CB200, 0x8CB220, 0x8CB240, tgmp
         *                0x8C8200, 0x8C8220, 0x8C8240, txjc_proc
         *                0x8C4200, 0x8C4220, 0x8C4240
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->ohfs_insert_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->ohfs_insert_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->ohfs_insert_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;

        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->txjc_proc_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->txjc_proc_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->txjc_proc_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->expand_ibuf_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->expand_ibuf_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->tgmp_handle->expand_ibuf_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;
        /*
         * Retrieve ODUKSC registers:
         *   lfield_range_SRC_ZONE_I: 0x8900A0
         *   lfield_range_RAMP_DONE_I: 0x8900B0
         *   lfield_range_SRC_ZONE_V: 0x8900C0
         */ 
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->oduksc_handle)->base_address + PMC_ODUKSC_CORE_REG_SRC_ZONE_INTS(i);
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->oduksc_handle)->base_address + PMC_ODUKSC_CORE_REG_RAMP_DONE_INTS(i);

        if (0 == i)
        {
            reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->oduksc_handle)->base_address + PMC_ODUKSC_CORE_REG_SRC_ZONE_INT_STAT(0);
        }
        /*
         * Update OHFS_INSERT registers using the handle from MAPOTN vs
         * the one used for TGMP handled above
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->ohfs_insert_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->ohfs_insert_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->ohfs_insert_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;


        /*
         * Retrieve MPMA and MPMA CFC registers
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpma_handle)->base_address + 0x00000600 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpma_handle)->base_address + 0x0000060c + i*4;


        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpma_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpma_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpma_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;

       /*
         * Retrieve MPMO and MPMO CFC registers
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpmo_handle)->base_address + 0x00001618 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpmo_handle)->base_address + 0x00001624 + i*4;

        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpmo_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpmo_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->mpmo_handle->cfc_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;

        /*
         * CFC unpack handle registers
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_unpack_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_unpack_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_unpack_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;

        /*
         * CFC egress handle registers
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_egress_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_egress_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_egress_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;

        /*
         * CFC packing handle registers
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_packing_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_packing_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_packing_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;

        /*
         * CFC prefetch handle registers
         */
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_prefetch_handle)->base_address + PMC_CFC_REG_FIFO_OVR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_prefetch_handle)->base_address + PMC_CFC_REG_FIFO_UDR_INT_REG_0 + i*4;
        reg_addrs[index++] = ((pmc_handle_t *)mapotn_handle->cfc_prefetch_handle)->base_address + PMC_CFC_REG_FIFO_IPT_ERR_INT_REG_0 + i*4;
        PMC_ASSERT(index < NUM_ADDRESSES, MAPOTN_ERR_CODE_ASSERT, 0, 0);
    }
    l5sys_batch_read(sys_handle, index, reg_addrs, data);

#ifndef NO_CONFIG_CACHE
    for (i = 0; i < index; i++)
    {
        l3pmc_sys_handle_t *h = *(l3pmc_sys_handle_t**)sys_handle;
        int_cache_reg_add(&h->int_cache, reg_addrs[i], data[i]);
    }
#endif
    PMC_RETURN();
} /* mapotn_retrieve_chnl_registers */

/*#endif */ /*DOXYGEN_PUBLIC_ONLY*/
/*
** End of file
*/
