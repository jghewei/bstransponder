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

#include "tgmp_loc.h"


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
const char TGMP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    TGMP_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char TGMP_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/

PRIVATE void tgmp_var_default_init(tgmp_handle_t *tgmp_handle);

PRIVATE void tgmp_var_update(tgmp_handle_t *tgmp_handle, 
                             UINT32 chnl,
                             tgmp_chnl_state_t chnl_state, 
                             UINT32 num_cal_entries);
                                                                          
PRIVATE PMC_ERROR tgmp_chnl_reset_cfg(tgmp_handle_t *tgmp_handle,
                                      UINT32 chnl,
                                      UINT32 chnl_state);

PRIVATE PMC_ERROR tgmp_int_validate(tgmp_handle_t    *tgmp_handle,
                                    UINT32            chnl_id);
/*
** Public Functions
*/

/*******************************************************************************
* tgmp_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a TGMP block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the TGMP subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - TSB name
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   ohfsr_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC tgmp_handle_t *tgmp_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    tgmp_handle_t *tgmp_handle;

    PMC_ENTRY();
    
    tgmp_handle = (tgmp_handle_t*)PMC_CTXT_CALLOC(sizeof(tgmp_handle_t), parent);
    
    pmc_handle_init(parent, tgmp_handle, sys_handle, PMC_MID_DIGI_TGMP, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(TGMP_LOG_ERR_STRINGS[0], TGMP_LOG_ERR_TABLE_BASE, TGMP_LOG_ERR_COUNT);
    
    tgmp_handle->mp_mgen_handle = mp_mgen_ctxt_create((pmc_handle_t *)tgmp_handle, base_address+BASE_ADDR_TGMP96_MGEN_MP_MGEN, sys_handle, "MP_MGEN");
    PMC_CTXT_REGISTER_PTR(&tgmp_handle->mp_mgen_handle, parent);
    tgmp_handle->expand_ibuf_handle = expand_ibuf_ctxt_create((pmc_handle_t *)tgmp_handle, base_address+BASE_ADDR_TGMP96_EXP_EXPAND_IBUF, sys_handle, "EXPAND_IBUF");
    PMC_CTXT_REGISTER_PTR(&tgmp_handle->expand_ibuf_handle, parent);
    tgmp_handle->txjc_proc_handle = txjc_proc_ctxt_create((pmc_handle_t *)tgmp_handle, base_address+BASE_ADDR_TGMP96_TXJC_TXJC_PROC_REG_MTSB, sys_handle, "TXJC_PROC");
    PMC_CTXT_REGISTER_PTR(&tgmp_handle->txjc_proc_handle, parent);
    tgmp_handle->ohfs_insert_handle = ohfs_insert_ctxt_create((pmc_handle_t *)tgmp_handle, base_address+BASE_ADDR_TGMP96_OHFS_OHFS_INSERT, sys_handle, "OHFS_INSERT");
    PMC_CTXT_REGISTER_PTR(&tgmp_handle->ohfs_insert_handle, parent);
      
    PMC_RETURN(tgmp_handle);
    
} /* tgmp_ctxt_create */

 
/*******************************************************************************
* tgmp_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a TGMP block instance.
*
*
* INPUTS:
*   *tgmp_handle         - pointer to TGMP handle instance
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
PUBLIC void tgmp_ctxt_destroy(tgmp_handle_t *tgmp_handle)
{
    PMC_ENTRY();
    
    ohfs_insert_ctxt_destroy(tgmp_handle->ohfs_insert_handle);
    txjc_proc_ctxt_destroy(tgmp_handle->txjc_proc_handle);
    mp_mgen_ctxt_destroy(tgmp_handle->mp_mgen_handle);
    expand_ibuf_ctxt_destroy(tgmp_handle->expand_ibuf_handle);
    
    PMC_LOG_TRACE("tgmp_handle->cfg.db_id = %u\n", tgmp_handle->cfg.db_id);
    PMC_LOG_TRACE("tgmp_handle->cfg.db_handle = %u\n", tgmp_handle->cfg.db_handle);

    if (tgmp_handle->cfg.db_handle != 0)
    {
        gen_db_detach(&(tgmp_handle->base), tgmp_handle->cfg.db_id,
                      tgmp_handle->cfg.db_handle);
    }
    
    PMC_CTXT_FREE(&tgmp_handle, tgmp_handle);
    
    PMC_RETURN();
} /* tgmp_ctxt_destroy */

/*******************************************************************************
* tgmp_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an TGMP block handle instance.
*
*
* INPUTS:
*   *tgmp_handle         - pointer to TGMP handle instance
*   tgmp_instance        - Enum for TGMP instance. See tgmp_instance_type_t.
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
PUBLIC void tgmp_handle_init(tgmp_handle_t *tgmp_handle, 
                             tgmp_instance_type_t tgmp_instance)
{
    PMC_ENTRY();
    
    
    /* Update static configurations */
    if (tgmp_instance == INSTANCE_TGMP)
    {
        tgmp_handle->cfg.num_tgmp_chanl = TGMP_NUM_CHANL;
   
        tgmp_handle->cfg.max_fifo_num = TGMP_IPT_DEPTH;
        tgmp_handle->cfg.db_id = UTIL_GEN_DB_MPMO_DB;
        
        /* attach to the databse */
        tgmp_handle->cfg.db_handle = gen_db_attach(&(tgmp_handle->base),tgmp_handle->cfg.db_id);   
           
        /* Initialize var_t data */
        tgmp_var_default_init(tgmp_handle);
    }
    else if (tgmp_instance == INSTANCE_MUX)
    {
        tgmp_handle->cfg.num_tgmp_chanl = TGMP_NUM_CHANL;
   
        tgmp_handle->cfg.max_fifo_num = TGMP_IPT_DEPTH;
        tgmp_handle->cfg.db_id = UTIL_GEN_DB_LINE_CORE_OTN_TX_DB; /* fix to TX DB */
        
        /* attach to the databse */
        tgmp_handle->cfg.db_handle = gen_db_attach(&(tgmp_handle->base),tgmp_handle->cfg.db_id);   
           
        /* Initialize var_t data */
        tgmp_var_default_init(tgmp_handle);
    }    
    
    /* Initialize static configurations based on TGMP and its children block
    ** isntance
    */   
    tgmp_handle->cfg.instance = tgmp_instance;   
    
    /* Initialize OHFS_INSERT */
    ohfs_insert_handle_init(tgmp_handle->ohfs_insert_handle, (ohfs_insert_instance_type_t)tgmp_instance);
    
    /* Initialize TXJC_PROC */
    txjc_proc_handle_init(tgmp_handle->txjc_proc_handle, (txjc_proc_instance_type_t)tgmp_instance);
    
    /* Initialize MP_MGEN */
    mp_mgen_handle_init(tgmp_handle->mp_mgen_handle);
     
    /* Initialize EXPAND_IBUF */
    expand_ibuf_handle_init(tgmp_handle->expand_ibuf_handle, (expand_ibuf_instance_type_t)tgmp_instance);
    
    PMC_RETURN();   
} /* tgmp_handle_init */


/*******************************************************************************
* tgmp_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Handles any allocation that is called in ctxt_destroy
*
*
* INPUTS:
*   *tgmp_handle         - pointer to TGMP handle instance
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
PUBLIC void tgmp_alloc_init(tgmp_handle_t *tgmp_handle)
{
    
    PMC_ENTRY();

    ohfs_insert_alloc_init(tgmp_handle->ohfs_insert_handle);
    txjc_proc_alloc_init(tgmp_handle->txjc_proc_handle);
    expand_ibuf_alloc_init(tgmp_handle->expand_ibuf_handle);
    
    PMC_RETURN();   
} /* tgmp_alloc_init */


/*******************************************************************************
* tgmp_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an TGMP block handle instance.
*
*
* INPUTS:
*   *tgmp_handle         - pointer to TGMP handle instance
*   *restart_init_cfg_ptr- pointer to restart init config
*   top_energy_state     - The expected energy state 
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
PUBLIC void tgmp_handle_restart_init(tgmp_handle_t *tgmp_handle, 
                                     util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                     pmc_energy_state_t top_energy_state
)
{
    PMC_ERROR rc;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(TGMP_LOG_ERR_STRINGS[0], TGMP_LOG_ERR_TABLE_BASE, TGMP_LOG_ERR_COUNT);
    }

    /* Reinitialize OHFS_INSERT */
    ohfs_insert_handle_restart_init(tgmp_handle->ohfs_insert_handle, restart_init_cfg_ptr, top_energy_state);
    
    /* Reinitialize TXJC_PROC */
    txjc_proc_handle_restart_init(tgmp_handle->txjc_proc_handle, restart_init_cfg_ptr, top_energy_state);
    
    /* Reinitialize EXPAND_IBUF */
    expand_ibuf_handle_restart_init(tgmp_handle->expand_ibuf_handle, restart_init_cfg_ptr, top_energy_state);
    
    rc = mp_mgen_handle_restart_init(tgmp_handle->mp_mgen_handle, restart_init_cfg_ptr, top_energy_state);
    PMC_ASSERT(PMC_SUCCESS == rc, TGMP_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_RETURN();   
} /* tgmp_handle_restart_init */


/*******************************************************************************
* tgmp_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an TGMP block instance.
*
*
* INPUTS:
*   *tgmp_handle         - pointer to TGMP handle instance.
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
PUBLIC UINT32 tgmp_base_address_get(tgmp_handle_t *tgmp_handle)
{
    PMC_ENTRY();
    PMC_RETURN(tgmp_handle->base.base_address);
    
} /* tgmp_base_address_get */

/*******************************************************************************
* tgmp_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an TGMP block instance.
*
*
* INPUTS:
*   *tgmp_handle           - pointer to TGMP handle instance.
*   energy_state           - enum defining state to set the TGMP module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for TGMP module
*
*******************************************************************************/
PUBLIC PMC_ERROR tgmp_init(tgmp_handle_t *tgmp_handle, 
                           pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(tgmp_handle!=NULL, TGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        ret_val = expand_ibuf_init(tgmp_handle->expand_ibuf_handle, energy_state);
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = ohfs_insert_init(tgmp_handle->ohfs_insert_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = txjc_proc_init(tgmp_handle->txjc_proc_handle, energy_state);
        }   
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        tgmp_var_default_init(tgmp_handle);
        ret_val = expand_ibuf_init(tgmp_handle->expand_ibuf_handle, energy_state);
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = ohfs_insert_init(tgmp_handle->ohfs_insert_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = txjc_proc_init(tgmp_handle->txjc_proc_handle, energy_state);
        } 
    } else {
        PMC_ASSERT(FALSE, TGMP_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN(ret_val);
} /* tgmp_init */


/*******************************************************************************
* tgmp_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   TGMP instance.  It will test the channel state for all resources within
*   the TGMP instance if any are not in TGMP_CHNL_START state then
*   FALSE is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the TGMP and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *tgmp_handle           - pointer to TGMP handle instance.
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
PUBLIC BOOL tgmp_start_state_test(tgmp_handle_t *tgmp_handle) 
{
    UINT32 i;
    PMC_ENTRY();
    PMC_ASSERT(tgmp_handle!=NULL, TGMP_ERR_INVALID_PARAMETERS, 0, 0);

    for (i = 0; i < tgmp_handle->cfg.num_tgmp_chanl; i++)
    {
        if(tgmp_handle->var.tgmp_chnl_cfg[i].state!= TGMP_CHNL_START)
            PMC_RETURN(FALSE);  
    }
    
    if((ohfs_insert_start_state_test(tgmp_handle->ohfs_insert_handle) == FALSE) ||
       (txjc_proc_start_state_test(tgmp_handle->txjc_proc_handle) == FALSE) ||
       (expand_ibuf_start_state_test(tgmp_handle->expand_ibuf_handle) == FALSE))
        PMC_RETURN(FALSE);
        
    PMC_RETURN(TRUE);

} /* tgmp_start_state_test */


/*******************************************************************************
* odtu_mux_tgmp_lo_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions a channl in TGMP or LO ODU channel in ODTU_MUX.
*
*
* INPUTS:
*   *tgmp_handle        - pointer to TGMP handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   client_type         - For TGMP, client_type. 
*                         See util_global_map_adapt_func_t. \n
*                         For MUX, LO ODU type. See util_global_odukp_type_t.
*   server_type         - For TGMP, LO OPUk type. \n
*                         For MUX, LO ODUj type. See util_global_odukp_type_t
*   client_rate         - client data rate bits/sec. Only valid if client_type 
*                         is set to: \n
*                         ODUJP_CBRX_GMP \n
*                         ODUKP_CBRX_B_A \n
*                         ODUKP_CBRX_A
*   mapping_mode        - mapping mode. See util_global_mapping_mode_t.
*   ts_type             - tributary slot type. See odu_line_payload_t.
*   num_ts              - number of tributary slots in the LO channel. \n
*                         For TGMP, set to 1 if mapping_mode is set to AMP or BMP.
*   offline_update      - Used for MUX only. Indication for programming 
*                         active/standby pages
*                         0 -- active page configuration
*                         1 -- offline page configuration
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_tgmp_lo_chnl_prov(tgmp_handle_t *tgmp_handle,
                                            UINT32 chnl, UINT32 client_type,
                                            util_global_odukp_type_t server_type,
                                            DOUBLE client_rate,
                                            util_global_mapping_mode_t mapping_mode,
                                            util_global_odu_line_payload_t ts_type, 
                                            UINT32 num_ts, BOOL8 offline_update)
{
    /* variable declaration */
    UINT32 m;
    UINT32 num_lo_ts;
    tgmp_chnl_state_t chnl_state;
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl > 95 || server_type > UTIL_GLOBAL_LAST_ODUK 
        || mapping_mode > UTIL_GLOBAL_NO_MAP 
        || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD || num_ts > 80
        || offline_update > 1)
    {
        PMC_RETURN(TGMP_ERR_INVALID_PARAMETERS);
    }
       
    if (tgmp_handle->cfg.instance == INSTANCE_TGMP) 
    {
        PMC_LOG_TRACE("instance = %u, client = %u\n", tgmp_handle->cfg.instance, client_type);
        
        if (client_type > UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP)
        {
            PMC_RETURN(TGMP_ERR_INVALID_PARAMETERS);
        }
    }
    else 
    {
        if (client_type > UTIL_GLOBAL_ODUK_DONT_CARE)
        {
            PMC_RETURN(TGMP_ERR_INVALID_PARAMETERS);   
        }
    }
        
    num_lo_ts = tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries;
    
    PMC_LOG_TRACE("%s, %s, %d, tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries = %u\n", __FILE__, __FUNCTION__, __LINE__, tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries);
               
    /* mapping mode specific configurations done in each chnl_prov() */
    /* EXPAND_IBUF - LO TS*/
    ret_val = expand_ibuf_lo_chnl_prov(tgmp_handle->expand_ibuf_handle, chnl, 
                                       server_type, client_type, client_rate, 
                                       mapping_mode, num_ts, ts_type, 
                                       num_lo_ts, offline_update);

    if (PMC_SUCCESS == ret_val)
    {
        /* OHFS_INSERT */
        ret_val = ohfs_insert_chnl_prov(tgmp_handle->ohfs_insert_handle, chnl,
                                        server_type, client_type, client_rate,
                                        mapping_mode, ts_type, 
                                        num_lo_ts);
    }
                                           
    if (PMC_SUCCESS == ret_val)
    {
        /* GMP specific MP_MGEN configuration - LO TS */
        if (mapping_mode == UTIL_GLOBAL_GMP)
        {
            if (tgmp_handle->cfg.instance == INSTANCE_MUX)
            {
                /* get M value */
                m = tgmp_handle->expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].M;
            }
            else {    
                /* Get the m value */                         
                ret_val = util_global_max_server_num_ts_get((util_global_parent_t)tgmp_handle->cfg.instance, 
                                                            server_type, 
                                                            (util_global_odu_line_payload_t)0, 
                                                            &m);
            }

            if (ret_val == PMC_SUCCESS)
            {
                ret_val = mp_mgen_data_mask_gen_cfg(tgmp_handle->mp_mgen_handle, 
                                                    chnl, m);
            }  
        } 
        else {
            m = 0;
        }
        
        /* Perform mapping mode specific configurations - LO TS */
        /* MP_MGEN common configuration for AMP, BMP and GMP mapping mode */
        ret_val = mp_mgen_last_txr_cfg(tgmp_handle->mp_mgen_handle, chnl,  
                                       server_type, 
                                       (util_global_odukp_type_t)client_type, 
                                       mapping_mode, 
                                       ts_type, num_ts, m);   
        
    }
 
    if (PMC_SUCCESS == ret_val)
    {
        /* TXJC_PROC */    
        ret_val = txjc_proc_chnl_prov(tgmp_handle->txjc_proc_handle, 
                                      chnl, server_type, client_type,
                                      client_rate, mapping_mode, ts_type, num_ts);
    }                                      
    
    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {
        
        chnl_state = TGMP_CHNL_EQUIPPED;
    
        PMC_LOG_TRACE("1. tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries = %u", tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries);
        
        tgmp_var_update(tgmp_handle, chnl, chnl_state, tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries);  
    } 
       
    PMC_RETURN(ret_val);    
} /* odtu_mux_tgmp_lo_chnl_prov */

/*******************************************************************************
* odtu_mux_tgmp_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unprovisions the specific channel.
*
*
* INPUTS:
*   *tgmp_handle      - pointer to TGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_tgmp_lo_chnl_deprov(tgmp_handle_t *tgmp_handle, UINT32 chnl)
{
    /* variable declaration */
    UINT32 ts;
    UINT32 num_of_recs = 0;
    UINT32 tot_ts = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    line_core_otn_db_key    core_key;
    
    mapotn_db_key        key;
    mapotn_db_query_res *q_result;
    
    PMC_ENTRY();
    
    /* 1. OHFS_REMOVE:  Set IPT_TS_VALID to 0 
    ** 2. RXJC_PROC:    Set CHNL_RST to 0 
    ** 3. MP_MGEN:      Set CHNL_RST to 0 
    ** 4. CPACT_OBUF:   Set CHNL_RST to 0 */
    
    tot_ts = tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries;
    PMC_LOG_TRACE("tot_ts = %u\n", tot_ts);
    
    /*  exp ibuf deprov  */
    ret_val = expand_ibuf_lo_chnl_deprov(tgmp_handle->expand_ibuf_handle, chnl);
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = txjc_proc_chnl_deprov(tgmp_handle->txjc_proc_handle, chnl);
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        if (tgmp_handle->cfg.instance == INSTANCE_MUX)
        {
            /* access DB to remove occupied tribslots */
            core_key.calendar_entry = DO_NOT_USE_KEY;
            core_key.ho_channel = DO_NOT_USE_KEY;
            core_key.mo_channel = chnl;
            core_key.lo_channel = DO_NOT_USE_KEY;
            core_key.trib_slot = DO_NOT_USE_KEY;
            
            num_of_recs = gen_db_query_count(&(tgmp_handle->base),
                                            tgmp_handle->cfg.db_id, 
                                            tgmp_handle->cfg.db_handle, 
                                            (void *)&core_key);
            
            PMC_LOG_TRACE("core_key.mo_channel = %u, num_of_recs = %u\n", core_key.mo_channel, num_of_recs);
            if (num_of_recs == 0)
            {
                
                core_key.calendar_entry = DO_NOT_USE_KEY;
                core_key.ho_channel = DO_NOT_USE_KEY;
                core_key.mo_channel = DO_NOT_USE_KEY;
                core_key.lo_channel = chnl;
                core_key.trib_slot = DO_NOT_USE_KEY;
                
                num_of_recs = gen_db_query_count(&(tgmp_handle->base),
                                                 tgmp_handle->cfg.db_id, 
                                                 tgmp_handle->cfg.db_handle, 
                                                 (void *)&core_key);
                if(num_of_recs== 0)
                {
                    PMC_RETURN(TGMP_ERR_DB_ENTRY_NOT_FOUND);
                }
            }
            else {
                PMC_LOG_TRACE("core_key.lo_channel = %u, num_of_recs = %u\n", core_key.lo_channel, num_of_recs);
            }              
            
            for (ts = 0; ts < num_of_recs; ts++)
            {
                ret_val = ohfs_insert_chnl_unprov(tgmp_handle->ohfs_insert_handle, chnl);
            }
        
        } else {
            /* MAPOTN DB */
            /* read from DB to get calendar entries for the channel */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.channel_num = chnl;
            num_of_recs = gen_db_query(&(tgmp_handle->base),
                                       tgmp_handle->cfg.db_id,
                                       tgmp_handle->cfg.db_handle, 
                                       (void *)&key,(void **)&q_result);
            PMC_LOG_TRACE("num_of_recs %d\n", num_of_recs);
            
            if(num_of_recs== 0){
                PMC_RETURN(TGMP_ERR_DB_ENTRY_NOT_FOUND);
            }
            
            for (ts= 0; ts < num_of_recs; ts++)
            {
                ret_val = ohfs_insert_ho_ipt_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[ts]->calendar_entry, chnl, 0, 0);                                               
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                ret_val = ohfs_insert_lo_ipt_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[ts]->calendar_entry, chnl, 0, 0);                                               
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                if(ts == 0)
                {
                    ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, q_result->p_recs[ts]->calendar_entry, chnl, 0, 0);
                    if (ret_val != PMC_SUCCESS) {
                        PMC_RETURN(ret_val);
                    }
                }
            }

            ret_val = ohfs_insert_chnl_unprov(tgmp_handle->ohfs_insert_handle, chnl);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
        }   
    }
  
    /* check if the unprovisioned ts matches the total number of provisioned ts */
    if (num_of_recs != tot_ts)
    {
        PMC_LOG_TRACE("Total number tribslots removed does not match the number of provisioned tribslots... removed cnt = %u, num_ts = %u\n", num_of_recs, tot_ts);
        PMC_RETURN(TGMP_ERR_CHNL_DEPROV);
    }
    
    /* update the channel stat and total number of ts */
    tgmp_handle->var.tgmp_chnl_cfg[chnl].state              = TGMP_CHNL_START;
    tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries    = 0;
                                                                                           
    PMC_RETURN(PMC_SUCCESS);   
} /* odtu_mux_tgmp_lo_chnl_deprov */

/*******************************************************************************
* odtu_mux_tgmp_tribslot_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the tributary slot used for LO mapping. 
*   This function sets per tributary slot related configuration in OHFS_REMOVE,
*   MP_MGEN and CPACT_OBUF LWC's. 
*
*   This function must be called for all tributary slots occupied by the LO 
*   channel prior to calling odtu_dmx_rgmp_lo_chnl_prov API.
*
* INPUTS:
*   *tgmp_handle        - pointer to TGMP handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ts_loc              - tributary location that the LO channel takes up.
*   client_type         - For MUX, LO ODU rate. See util_global_odukp_type_t. \n
*                         For TGMP, adaptaion_function. See
*                         adaptaion_function_enum for valid types
*   server_type         - For MUX, HO ODU rate. See util_global_odukp_type_t. \n
*                         For TGMP, ODUkp or ODUjp type see odukp_type_t, ODUkp 
*                         all ODUk types valid, ODUjp only ODU2/3 types valid
*   ts_type             - tributary slot type. See odu_line_payload_t. \n
*                         NOTE: Only valid for MUX. This argument is ignored
*                               in TGMP.
*   mapping_mode        - mapping mode used in mapping the LO channel to HO ODU. \n
*                         For TGMP, this argument is ignored.
*   ts_occupied         - determines if the tribslot is occupied. \n
*                         1 -- tribslot is used for the channel. \n
*                         0 -- tribslot is not used for the channel.
*   last_ts             - determines whether the tribslot occupied by this LO
*                         channel is the last tributary slot. Only valid when
*                         'mapping_mode' is set to GMP. \n
*                         NOTE: For TGMP, always set to 1. \n
*                         1 -- the tribslot is the last tributary slot. \n
*                         0 -- the tribslot is not the last tributary slot.
*   frm_num             - For MUX, occurent number of tributary slots in the HO channel. 
*   offline_update      - Used for MUX only. Indication for programming 
*                         active/standby pages
*                         0 -- active page configuration
*                         1 -- offline page configuration
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_tgmp_tribslot_cfg(tgmp_handle_t *tgmp_handle,
                                            UINT32 chnl, UINT32 ts_loc, 
                                            UINT32 client_type, 
                                            UINT32 server_type,
                                            util_global_odu_line_payload_t ts_type,
                                            util_global_mapping_mode_t mapping_mode,
                                            UINT32 ts_occupied,
                                            UINT32 last_ts,
                                            UINT32 frm_num, BOOL8 offline_update) 
{
    /* variable declaration */
    tgmp_chnl_state_t chnl_state;
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32     num_cal_entries =0;
    UINT32 i;
    UINT32 map_mode;
    UINT32 ipt_id;
    UINT32 ipt_entry;
    UINT32 null_id;
    
    mapotn_db_key        key;
    mapotn_db_query_res *q_result;
    
    line_core_otn_db_key    core_key;
    line_core_otn_db_query_res *core_q_result;
    
    PMC_ENTRY();
    
    
    switch (mapping_mode)
    {
        case UTIL_GLOBAL_AMP:
             map_mode = 1;
             break;
        case UTIL_GLOBAL_BMP:
             map_mode = 0;
             break;
        case UTIL_GLOBAL_GMP:
            map_mode = 4;
            break;
        default:
            PMC_RETURN(TGMP_ERR_INVALID_PARAMETERS);
    }    
    
    /* argument checking */  
    if (tgmp_handle->cfg.instance == INSTANCE_TGMP)
    {
         
        /* go throught the database and get the number of calendar entries for 
           the channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.channel_num = chnl;
        num_cal_entries=gen_db_query(&(tgmp_handle->base),tgmp_handle->cfg.db_id, tgmp_handle->cfg.db_handle, (void *)&key,(void **)&q_result);
        
        if(num_cal_entries == 0){
            PMC_RETURN(TGMP_ERR_DB_ENTRY_NOT_FOUND);
        }
        
        for (i = 0; i < num_cal_entries; i++)
        {
            ret_val = ohfs_insert_ho_ipt_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, chnl, i, 1);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
            ret_val = ohfs_insert_lo_ipt_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, chnl, i, 1);                                               
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
            
            if(i == (num_cal_entries-1))
            {
                 ret_val = ohfs_insert_ts_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, 1, 1);
                 if (ret_val != PMC_SUCCESS) {
                     PMC_RETURN(ret_val);
                 }
            }
            else
            {
                ret_val = ohfs_insert_ts_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, 0, 1);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
            }
            
            ret_val = ohfs_insert_lo_chnl_id_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, chnl);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
                 
            ret_val = ohfs_insert_chnl_mapping_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, map_mode);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
            
            if (i == 0) 
            {
                ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, q_result->p_recs[i]->calendar_entry, chnl, i, 1);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                
                if (mapping_mode == UTIL_GLOBAL_AMP)
                {
                    ret_val = mp_mgen_amp_cfg(tgmp_handle->mp_mgen_handle, q_result->p_recs[i]->calendar_entry, 0, 
                                              (util_global_odukp_type_t)server_type, (util_global_odukp_type_t)client_type, ts_type);
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                } 
                else if (mapping_mode == UTIL_GLOBAL_BMP) 
                {
                    ret_val = ohfs_insert_lo_chnl_id_cfg(tgmp_handle->ohfs_insert_handle, q_result->p_recs[i]->calendar_entry, chnl); 
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
            } 
            else 
            {
                ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, q_result->p_recs[i]->calendar_entry, chnl, i, 0);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
            }
             
        }
        
    }
    else 
    {
        switch (mapping_mode)
        {
            case UTIL_GLOBAL_AMP:
                if(server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1)
                {
                    map_mode = 2;
                }
                else if(server_type == UTIL_GLOBAL_ODU3E1 && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
                {
                    map_mode = 3;
                }
                else
                {
                    map_mode = 1;
                }
                break;
            case UTIL_GLOBAL_GMP:
                map_mode = 4;
                break;
            default:
                PMC_RETURN(TGMP_ERR_INVALID_PARAMETERS);
        }
        
        /* query the record to update */
        core_key.calendar_entry = ts_loc;
        core_key.ho_channel = DO_NOT_USE_KEY;
        core_key.mo_channel = DO_NOT_USE_KEY;
        core_key.lo_channel = DO_NOT_USE_KEY;
        core_key.trib_slot = DO_NOT_USE_KEY;
    
        num_cal_entries = gen_db_query(&(tgmp_handle->base),
                                       tgmp_handle->cfg.db_id, 
                                       tgmp_handle->cfg.db_handle, 
                                       (void *)&core_key,(void **)&core_q_result);
       
        if (num_cal_entries == 0) PMC_RETURN(TGMP_ERR_DB_ENTRY_NOT_FOUND);
            
        PMC_LOG_TRACE("core_q_result.trib_slot = %u, num_cal_entries = %u\n", core_q_result->p_recs[0]->trib_slot, num_cal_entries);
        ret_val = tgmp_expxand_cfc_fifo_ipt_id_get(tgmp_handle,
                                                   chnl,
                                                   frm_num,
                                                   &ipt_id);

        if (PMC_SUCCESS != ret_val) {
          PMC_RETURN(ret_val);
        }

        for (i = 0; i < num_cal_entries; i++)
        {  
            /* Correct IPT Config for MP_MGEN & OHFS_INSERT */
            /* MP_MGEN */
            if (offline_update == 0)
            {

                if (FALSE == pmc_is_digi_rev_a_revision(&tgmp_handle->base))
                {
                    ipt_entry = ipt_id;
                    null_id = 0x7F;
                }
                else {
                    ipt_entry = core_q_result->p_recs[i]->calendar_entry;
                    null_id = 0;
                }
                    
                if (mapping_mode == UTIL_GLOBAL_GMP)
                {   
                    if (frm_num == 0)
                    {
                        ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                  ipt_entry,
                                                  chnl, 
                                                  frm_num, 
                                                  ts_occupied); 
                    }
                    else {
                        ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                  ipt_entry,
                                                  null_id, 
                                                  0, 
                                                  0);
                    }                                                                                                  
                }                
                else {
                    if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                    {
                        if (client_type == UTIL_GLOBAL_ODU2)
                        {
                               if (frm_num < 4)
                               {
                                   ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                             ipt_entry,
                                                             chnl, 
                                                             frm_num, 
                                                             ts_occupied);
                            }  
                            else {
                                ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                          ipt_entry, 0, 0, 0);
                            }                                                                                                                                            
                        }
                        else if (client_type == UTIL_GLOBAL_ODU1)
                        {
                            if (frm_num == 0)
                            {
                                ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                          ipt_entry,
                                                          chnl, 
                                                          frm_num, 
                                                          ts_occupied);     
                            }
                            else {
                                ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                          ipt_entry, 0, 0, 0);    
                            }
                        }
                        else if (client_type == UTIL_GLOBAL_ODU2E)
                        {
                            ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                      ipt_entry, 
                                                      chnl, frm_num, ts_occupied);
                        }   
                    }
                    else { 
                           ret_val = mp_mgen_ipt_cfg(tgmp_handle->mp_mgen_handle, 
                                                     ipt_entry,
                                                     chnl, 
                                                     frm_num, 
                                                     ts_occupied);
                    }                                                  
                }                                              
            }
                                                      
            if ((PMC_SUCCESS == ret_val) && (mapping_mode != UTIL_GLOBAL_BMP))
            {
                ret_val = ohfs_insert_lo_chnl_id_cfg(tgmp_handle->ohfs_insert_handle, 
                                                     core_q_result->p_recs[i]->calendar_entry, 
                                                     chnl); 
            }
                                                                
            if (PMC_SUCCESS == ret_val)
            {
                ret_val = ohfs_insert_chnl_mapping_cfg(tgmp_handle->ohfs_insert_handle, 
                                                       core_q_result->p_recs[i]->calendar_entry, 
                                                       map_mode);  
            }
            
            /* OHFS_INSERT common configuration */
            if ((PMC_SUCCESS == ret_val) && (mapping_mode == UTIL_GLOBAL_GMP))
            {
                ret_val = ohfs_insert_ts_cfg(tgmp_handle->ohfs_insert_handle, 
                                             core_q_result->p_recs[i]->calendar_entry, 
                                             last_ts, ts_occupied);                               
            }
            else if ((PMC_SUCCESS == ret_val) && (mapping_mode == UTIL_GLOBAL_AMP))
            {    
                ret_val = ohfs_insert_ts_cfg(tgmp_handle->ohfs_insert_handle, 
                                             core_q_result->p_recs[i]->calendar_entry, 
                                             0, ts_occupied);
            }  
        }    
        
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        chnl_state = TGMP_CHNL_START;
        tgmp_var_update(tgmp_handle, chnl, chnl_state, num_cal_entries);
    }   
    
    PMC_RETURN(PMC_SUCCESS);                       
} /* odtu_mux_tgmp_tribslot_cfg */

/*******************************************************************************
* tgmp_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to TGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95                    
*                                                                               
* OUTPUTS:                                                                      
*   None.                                                                       
*                                                                               
* RETURNS:                                                                      
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR tgmp_chnl_activate(tgmp_handle_t *tgmp_handle, UINT32 chnl) 

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (tgmp_handle->var.tgmp_chnl_cfg[chnl].state != TGMP_CHNL_EQUIPPED)
    {
        PMC_RETURN(TGMP_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    } 
    
    /* Channel reset is released by configuring CH_RSTB to '1' */
    tgmp_chnl_reset_cfg(tgmp_handle, chnl, TRUE);
    
    ret_val = expand_ibuf_chnl_activate(tgmp_handle->expand_ibuf_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    ret_val = txjc_proc_chnl_activate(tgmp_handle->txjc_proc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    ret_val = ohfs_insert_chnl_activate(tgmp_handle->ohfs_insert_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* Update var context for a provisioned channel */
    tgmp_handle->var.tgmp_chnl_cfg[chnl].state = TGMP_CHNL_OPERATIONAL;
   
    PMC_RETURN(ret_val);

} /* tgmp_chnl_activate */

/*******************************************************************************
* tgmp_chnl_amp_gmp_startup_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel with startup sequence for AMP and GMP
*   mappings for MAPOTN when data sourced from DCPB vs ETRANS path
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to TGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95                    
*                                                                               
* OUTPUTS:                                                                      
*   None.                                                                       
*                                                                               
* RETURNS:                                                                      
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR tgmp_chnl_amp_gmp_startup_activate(tgmp_handle_t *tgmp_handle, UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (tgmp_handle->var.tgmp_chnl_cfg[chnl].state != TGMP_CHNL_EQUIPPED)
    {
        PMC_RETURN(TGMP_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    } 



    ret_val = ohfs_insert_chnl_activate(tgmp_handle->ohfs_insert_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* Channel reset is released by configuring CH_RSTB to '1' */
    tgmp_chnl_reset_cfg(tgmp_handle, chnl, TRUE);
    
    ret_val = expand_ibuf_chnl_activate(tgmp_handle->expand_ibuf_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    ret_val = txjc_proc_chnl_activate(tgmp_handle->txjc_proc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
   
    /* Update var context for a provisioned channel */
    tgmp_handle->var.tgmp_chnl_cfg[chnl].state = TGMP_CHNL_OPERATIONAL;
   
    PMC_RETURN(ret_val);

} /* tgmp_chnl_amp_gmp_startup_activate */

/*******************************************************************************
* tgmp_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to TGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95                  
*                                                                               
* OUTPUTS:                                                                      
*   None.                                                                       
*                                                                               
* RETURNS:                                                                      
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC PMC_ERROR tgmp_chnl_deactivate(tgmp_handle_t *tgmp_handle, UINT32 chnl)                                        

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* put channel in reset by configuring CH_RSTB to '0' */
    tgmp_chnl_reset_cfg(tgmp_handle, chnl, FALSE);
    
    
    ret_val = expand_ibuf_chnl_deactivate(tgmp_handle->expand_ibuf_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    ret_val = txjc_proc_chnl_deactivate(tgmp_handle->txjc_proc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    ret_val = ohfs_insert_chnl_deactivate(tgmp_handle->ohfs_insert_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* Update var context for a provisioned channel */
    tgmp_handle->var.tgmp_chnl_cfg[chnl].state = TGMP_CHNL_EQUIPPED;
   
    PMC_RETURN(ret_val);
   
} /* tgmp_chnl_deactivate */


/*******************************************************************************
* tgmp_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of TGMP  channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *tgmp_handle  - pointer to ohfs_remove handle instance
*   *chnl         - set of channel ID to be cleaned
*   num_chnl      - number of channel ID to be cleaned
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void tgmp_chnl_clean( tgmp_handle_t *tgmp_handle, 
                             UINT32        *chnl,
                             UINT32        num_chnl ) 
{
    UINT32 chnl_itr;

    PMC_ENTRY();
    
    /* clean sub-blocks */
    ohfs_insert_chnl_clean(tgmp_handle->ohfs_insert_handle,chnl,num_chnl);
    mp_mgen_chnl_clean(tgmp_handle->mp_mgen_handle,chnl,num_chnl);
    expand_ibuf_chnl_clean(tgmp_handle->expand_ibuf_handle, chnl,num_chnl); 
    txjc_proc_chnl_clean(tgmp_handle->txjc_proc_handle, chnl,num_chnl); 

    /* reset channel and put it in START state */
    for(chnl_itr = 0; chnl_itr < num_chnl;chnl_itr++)
    {
        tgmp_chnl_reset_cfg(tgmp_handle, chnl[chnl_itr], FALSE);
        tgmp_handle->var.tgmp_chnl_cfg[chnl[chnl_itr]].state              = TGMP_CHNL_START;
        tgmp_handle->var.tgmp_chnl_cfg[chnl[chnl_itr]].num_cal_entries    = 0;
    }
    
    PMC_RETURN();

} /* tgmp_chnl_clean */

/*******************************************************************************
* tgmp_expd_ibuf_cfc_xoff_level_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Sets the expand ibuf CFC fill level where DATA_XOFF is gnerated for the
*   corresponding LO channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to TGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   cfc_fill_lvl      - desired CFC Fill level                  
*                                                                               
* OUTPUTS:                                                                      
*   None.                                                                       
*                                                                               
* RETURNS:                                                                      
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC PMC_ERROR tgmp_expd_ibuf_cfc_xoff_level_cfg(tgmp_handle_t *tgmp_handle, 
                                                   UINT32 chnl,  
                                                   UINT32 cfc_fill_lvl)                                        

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_val = expand_ibuf_cfc_fill_lvl_set(tgmp_handle->expand_ibuf_handle, chnl, cfc_fill_lvl);
    if (PMC_SUCCESS != ret_val)
    {
        PMC_RETURN(ret_val);
    }
    
    PMC_RETURN(PMC_SUCCESS);
   
} /* tgmp_expd_ibuf_cfc_xoff_level_cfg */


/*******************************************************************************
* tgmp_activated_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to TGMP handle instance
*                                                                             
* OUTPUTS:
*   *actv_chnl         - all activated channels
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void tgmp_activated_chnl_get(tgmp_handle_t *tgmp_handle,
                                    BOOL8 *actv_chnl)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(tgmp_handle != NULL, TGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < TGMP_NUM_CHANL; itr++)
    {
        actv_chnl[itr] = tgmp_field_CHAN_RSTB_get(NULL, tgmp_handle, itr)==0?FALSE:TRUE; 
    }

    PMC_RETURN();
}  /* tgmp_activated_chnl_get */

/*******************************************************************************
* FUNCTION: tgmp_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   tgmp_handle       - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR tgmp_int_chnl_enable(tgmp_handle_t     *tgmp_handle,
                                      UINT32             chnl_id,
                                      tgmp_int_chnl_t   *int_table_ptr,
                                      tgmp_int_chnl_t   *int_en_table_ptr,
                                      BOOL8              enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    result = tgmp_int_validate(tgmp_handle, chnl_id);

    if (PMC_SUCCESS == result && NULL == int_en_table_ptr)
    {
        result = TGMP_ERR_INVALID_PARAMETERS;
    }

    
    if (tgmp_handle->cfg.instance == INSTANCE_TGMP && PMC_SUCCESS == result)
    {
        #ifdef TGMP_INT_CHNL_RANGE
        #undef TGMP_INT_CHNL_RANGE
        #endif
        #define TGMP_INT_CHNL_RANGE    UTIL_GLOBAL_INT_LFIELD_ENABLE_SINGLE
        
        TGMP_INT_CHNL_TABLE_DEFINE();   
    }

    /* OHFS_INSERT */
    if (PMC_SUCCESS == result)
    {
        result = ohfs_insert_int_chnl_enable(tgmp_handle->ohfs_insert_handle,
                                             chnl_id,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, ohfs_insert),
                                             &int_en_table_ptr->ohfs_insert,
                                             enable);
    }

    /* TXJC_PROC */
    if (PMC_SUCCESS == result)
    {
        result = txjc_proc_int_chnl_enable(tgmp_handle->txjc_proc_handle,
                                           chnl_id,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, txjc_proc),
                                           &int_en_table_ptr->txjc_proc,
                                           enable);
    }

    /* EXPAND_IBUF */
    if (PMC_SUCCESS == result)
    {
        result = expand_ibuf_int_chnl_enable(tgmp_handle->expand_ibuf_handle,
                                             chnl_id,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, expand_ibuf),
                                             &int_en_table_ptr->expand_ibuf,
                                             enable);
    }

    PMC_RETURN(result);
} /* tgmp_int_chnl_enable */

/*******************************************************************************
* FUNCTION: tgmp_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   tgmp_handle       - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR tgmp_int_chnl_clear(tgmp_handle_t      *tgmp_handle,
                                     UINT32              chnl_id,
                                     tgmp_int_chnl_t    *int_table_ptr,
                                     tgmp_int_chnl_t    *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();

    result = tgmp_int_validate(tgmp_handle, chnl_id);
        
    if (tgmp_handle->cfg.instance == INSTANCE_TGMP && PMC_SUCCESS == result)
    {
        #ifdef TGMP_INT_CHNL_RANGE
        #undef TGMP_INT_CHNL_RANGE
        #endif
        #define TGMP_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CLEAR_SINGLE
    
        TGMP_INT_CHNL_TABLE_DEFINE();
    }
    
    /* OHFS_INSERT */
    if (PMC_SUCCESS == result)
    {
        result = ohfs_insert_int_chnl_clear(tgmp_handle->ohfs_insert_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, ohfs_insert),
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, ohfs_insert));
    }

    /* TXJC_PROC */
    if (PMC_SUCCESS == result)
    {
        result = txjc_proc_int_chnl_clear(tgmp_handle->txjc_proc_handle,
                                          chnl_id,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, txjc_proc),
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, txjc_proc));
    }

    /* EXPAND_IBUF */
    if (PMC_SUCCESS == result)
    {
        result = expand_ibuf_int_chnl_clear(tgmp_handle->expand_ibuf_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, expand_ibuf),
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, expand_ibuf));
    }

    PMC_RETURN(result);
} /* tgmp_int_chnl_clear */

/*******************************************************************************
* FUNCTION: tgmp_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   tgmp_handle       - Pointer to subsystem handle
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
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR tgmp_int_chnl_retrieve(tgmp_handle_t      *tgmp_handle,
                                        UINT32              chnl_id,
                                        tgmp_int_chnl_t    *filt_table_ptr,
                                        tgmp_int_chnl_t    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = tgmp_int_validate(tgmp_handle, chnl_id);

    if (PMC_SUCCESS == result && NULL == int_table_ptr)
    {
        result = TGMP_ERR_INVALID_PARAMETERS;
    }

    if (tgmp_handle->cfg.instance == INSTANCE_TGMP && PMC_SUCCESS == result)
    {
        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(tgmp_handle, tgmp, UTIL_GLOBAL_INT_OPTIMIZE);
    
        #ifdef TGMP_INT_CHNL_RANGE
        #undef TGMP_INT_CHNL_RANGE
        #endif
        #define TGMP_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_RETRIEVE_SINGLE
        
        TGMP_INT_CHNL_TABLE_DEFINE();
        
        UTIL_GLOBAL_INT_BUFFER_FLUSH(tgmp_handle, tgmp);        
    }
    
    /* OHFS_INSERT */
    if (PMC_SUCCESS == result)
    {
        result = ohfs_insert_int_chnl_retrieve(tgmp_handle->ohfs_insert_handle,
                                               chnl_id,
                                               UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, ohfs_insert),
                                               &int_table_ptr->ohfs_insert);
    }

    /* TXJC_PROC */
    if (PMC_SUCCESS == result)
    {
        result = txjc_proc_int_chnl_retrieve(tgmp_handle->txjc_proc_handle,
                                             chnl_id,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, txjc_proc),
                                             &int_table_ptr->txjc_proc);
    }

    /* EXPAND_IBUF */
    if (PMC_SUCCESS == result)
    {
        result = expand_ibuf_int_chnl_retrieve(tgmp_handle->expand_ibuf_handle,
                                               chnl_id,
                                               UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, expand_ibuf),
                                               &int_table_ptr->expand_ibuf);
    } 
    PMC_RETURN(result);
} /* tgmp_int_chnl_retrieve */

/*******************************************************************************
* FUNCTION: tgmp_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   tgmp_handle       - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR tgmp_int_chnl_enabled_check(tgmp_handle_t    *tgmp_handle,
                                             UINT32            chnl_id,
                                             tgmp_int_chnl_t  *int_en_table_ptr,
                                             BOOL8            *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = tgmp_int_validate(tgmp_handle, chnl_id);
    
    if (PMC_SUCCESS == result && NULL == int_found_ptr)
    {
        result = TGMP_ERR_INVALID_PARAMETERS;
    }

    if (tgmp_handle->cfg.instance == INSTANCE_TGMP && PMC_SUCCESS == result)
    {
#ifdef TGMP_INT_CHNL_RANGE
#undef TGMP_INT_CHNL_RANGE
#endif
#define TGMP_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CHECK_SINGLE
    
        TGMP_INT_CHNL_TABLE_DEFINE();
    }

    /* OHFS_INSERT */
    if (PMC_SUCCESS == result && FALSE == *int_found_ptr)
    {
        result = ohfs_insert_int_chnl_enabled_check(tgmp_handle->ohfs_insert_handle,
                                                    chnl_id,
                                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, ohfs_insert),
                                                    int_found_ptr);
    }
    
    /* TXJC_PROC */
    if (PMC_SUCCESS == result && FALSE == *int_found_ptr)
    {
        result = txjc_proc_int_chnl_enabled_check(tgmp_handle->txjc_proc_handle,
                                                  chnl_id,
                                                  UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, txjc_proc),
                                                  int_found_ptr);
    }

    /* EXPAND_IBUF */
    if (PMC_SUCCESS == result)
    {
        result = expand_ibuf_int_chnl_enabled_check(tgmp_handle->expand_ibuf_handle,
                                                    chnl_id,
                                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, expand_ibuf),
                                                    int_found_ptr);
    }

    PMC_RETURN(result);
} /* tgmp_int_chnl_enabled_check */


/*******************************************************************************
*  tgmp_expxand_cfc_fifo_ipt_id_get
* ______________________________________________________________________________
*
*   DESCRIPTION:
*    This API retrieves the channel ID associated with the page number within
*    the FIFO.
*
*
* INPUTS:
*   *tgmp_handle        - Pointer to TGMP handle
*   fifo_id             - fifo id
*   frm_num             - frame number within a fifo 
*   
*
* OUTPUTS:
*  *ipt_id          - channel ID associated with this frame number of the fifo 
* 
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR - if no valid IPT ID is found
*
*******************************************************************************/
PUBLIC PMC_ERROR tgmp_expxand_cfc_fifo_ipt_id_get(tgmp_handle_t *tgmp_handle,
                                                  UINT32 fifo_id,
                                                  UINT32 frm_num,
                                                  UINT32 *ipt_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = expxand_cfc_fifo_ipt_id_get(tgmp_handle->expand_ibuf_handle,
                                         fifo_id, frm_num, ipt_id);

    PMC_RETURN(result);
} /* tgmp_expxand_cfc_fifo_ipt_id_get */

/*******************************************************************************
* tgmp_mapping_mode_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the mapping mode at a specified calendar cycle
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to MAPOTN handle instance
*   calendar_cycle    - The calendar cycle
*                                                                 
* OUTPUTS:
*   NOne
*
* RETURNS:
*    The mapping mode
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC util_global_mapping_mode_t tgmp_mapping_mode_get(tgmp_handle_t *tgmp_handle,
                                                        UINT32 calendar_cycle)
{
    util_global_mapping_mode_t mode;
    PMC_ENTRY();
    
    PMC_ASSERT(tgmp_handle != NULL, TGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    mode = ohfs_insert_mapping_mode_get(tgmp_handle->ohfs_insert_handle,calendar_cycle);
    
    PMC_RETURN(mode);
}  /* tgmp_mapping_mode_get*/

/*******************************************************************************
* tgmp_fixed_stuff_enable_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns TRUE when fixed stuff is enabled
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle      - pointer to MAPOTN handle instance
*   chnl                     - the channel 
*                                                                 
* OUTPUTS:
*   None
*
* RETURNS:
*   TRUE when fixed is enabled, FALSE when it is not 
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8 tgmp_fixed_stuff_enable_get(tgmp_handle_t *tgmp_handle,
                                         UINT32 chnl)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();
    result = ohfs_insert_fixed_stuff_enable_get(tgmp_handle->ohfs_insert_handle,chnl);
    
    PMC_RETURN(result);
} /* tgmp_fixed_stuff_enable_get */



/*
** Private Functions
*/

/*******************************************************************************
*  tgmp_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize TGMP var_t variables.
*
* INPUTS:
*   *tgmp_handle      - pointer to TGMP handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void tgmp_var_default_init(tgmp_handle_t *tgmp_handle)
{
    /* variable declaration */
    UINT32 chnl;
    
    PMC_ENTRY();
    
    /* Configure default TGMP variable parameters for each channel */
    for (chnl = 0; chnl < tgmp_handle->cfg.num_tgmp_chanl; chnl++)
    {
        tgmp_handle->var.tgmp_chnl_cfg[chnl].state          = TGMP_CHNL_START;
        tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries = 0;
    }

    PMC_RETURN();
} /* tgmp_var_default_init */

/*******************************************************************************
* tgmp_var_update
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Updates the channel based dynamically changing context in the memory.                                                                               
*                                                                               
* INPUTS:                                                                       
*   *tgmp_handle        - pointer to TGMP handle instance                  
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*   chnl_state          - the state of the channel see tgmp_chnl_state_t for
*                         valid types                                          
*   num_cal_entries     - number of calendar enrties for the channel
*              
*                                                                               
* OUTPUTS:                                                                      
*   None.                                                                       
*                                                                               
* RETURNS:                                                                      
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PRIVATE void tgmp_var_update(tgmp_handle_t *tgmp_handle, 
                             UINT32 chnl,
                             tgmp_chnl_state_t chnl_state, 
                             UINT32 num_cal_entries)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    /* Update var context for a channel */
    tgmp_handle->var.tgmp_chnl_cfg[chnl].state              = chnl_state;
    tgmp_handle->var.tgmp_chnl_cfg[chnl].num_cal_entries    = num_cal_entries;
    
    PMC_RETURN(); 
}  /* tgmp_var_update */ 

/*******************************************************************************
*  tgmp_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures reset mode for TGMP channel.
*
*
* INPUTS:
*   *tgmp_handle      - pointer to TGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   chnl_state        - register access state of TGMP channel \n
*                       0 -- TGMP channel is in reset mode. It clears all the 
*                            context information for that channel \n
*                       1 -- TGMP channel is out of reset.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES: For reconfiguring a channel to a different mode, SW should first assert
*        the reset, change the configuration and then release the reset.
*
*******************************************************************************/
PRIVATE PMC_ERROR tgmp_chnl_reset_cfg(tgmp_handle_t *tgmp_handle,
                                      UINT32 chnl,
                                      UINT32 chnl_state)
{
    /* variable declaration */
    
    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, chnl_state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_state);
    
    tgmp_field_CHAN_RSTB_set(NULL, tgmp_handle, chnl, chnl_state);
    
    if(chnl_state == 0)
    {
       PMC_Q_USLEEP(tgmp_handle,1);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* tgmp_chnl_reset_cfg */

/*******************************************************************************
* FUNCTION: tgmp_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   tgmp_handle    - Pointer to TGMP handle
*   chnl_id          - chnl id
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR tgmp_int_validate(tgmp_handle_t    *tgmp_handle,
                                    UINT32            chnl_id)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != tgmp_handle, TGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= tgmp_handle->cfg.num_tgmp_chanl)
    {
        result = TGMP_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* tgmp_int_validate */

/*
** End of file
*/
