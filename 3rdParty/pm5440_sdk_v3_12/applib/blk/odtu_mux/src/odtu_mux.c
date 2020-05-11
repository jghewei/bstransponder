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
#include "odtu_mux_loc.h"


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
** Local Variabls
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
const char ODTU_MUX_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODTU_MUX_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODTU_MUX_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/                           
PRIVATE PMC_ERROR odtu_mux_first_valid_dummy_find(odtu_mux_handle_t *odtu_mux_handle,
                                                  UINT32 first_cycle, 
                                                  UINT32 *valid_null_cycle,
                                                  UINT32 *dummy_idx);

PRIVATE odtu_mux_chnl_mode_t odtu_mux_chnl_mode_get(odtu_mux_handle_t *odtu_mux_handle,
                                                    UINT32 chnl_id);
PRIVATE PMC_ERROR odtu_mux_ctxt_audit(odtu_mux_handle_t *odtu_mux_handle,
                                      util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                      odtu_mux_recover_var_t *odtu_mux_var_reg,            
                                      odtu_mux_recover_var_t *prev_stg_mux_var_reg,                              
                                      odtu_mux_var_t *odtu_mux_var_clean);
PRIVATE PMC_ERROR odtu_mux_ctxt_cleanup(odtu_mux_handle_t *odtu_mux_handle,
                                        odtu_mux_recover_var_t *odtu_mux_var_reg,                                         
                                        odtu_mux_var_t *odtu_mux_var_clean);
PRIVATE PMC_ERROR odtu_mux_ctxt_reg_recover(odtu_mux_handle_t *odtu_mux_handle,
                                            pmc_energy_state_t top_energy_state_reg,                                         
                                            odtu_mux_recover_var_t *odtu_mux_var_reg);
PRIVATE void odtu_mux_var_default_init(odtu_mux_handle_t *odtu_mux_handle, odtu_mux_var_t *odtu_mux_var);


PRIVATE PMC_ERROR odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id, UINT32 chnl_state);
                                             
PRIVATE PMC_ERROR odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id, UINT32 chnl_state);

PRIVATE void odtu_mux_ho_chnl_var_update(odtu_mux_handle_t *odtu_mux_handle, 
                                         UINT32 ho_chnl_id, 
                                         odtu_mux_chnl_state_t chnl_state,
                                         util_global_odukp_type_t ho_odu_type,
                                         UINT32 num_tribslot, UINT8 *cal_array, 
                                         UINT32 *lo_chnl_ids,
                                         util_global_odu_line_payload_t ts_type,
                                         UINT32 num_active_ts,
                                         UINT32 num_cal_entries);
                                       
PRIVATE PMC_ERROR odtu_mux_ram_init(odtu_mux_handle_t *odtu_mux_handle);     

PRIVATE PMC_ERROR odtu_mux_dummy_schd_cfg(odtu_mux_handle_t *odtu_mux_handle, 
                                          UINT32 chnl, 
                                          util_global_odukp_type_t ho_odu_type, 
                                          UINT32 cal_entry,
                                          util_global_odu_line_payload_t ts_type);  
                                          
PRIVATE PMC_ERROR odtu_mux_dummy_schd_clean(odtu_mux_handle_t *odtu_mux_handle, 
                                            UINT32 chnl, 
                                            UINT32 cal_entry,
                                            util_global_odu_line_payload_t ts_type);                                                                          
                                         
/*
** Public Functions
*/

/*******************************************************************************
* odtu_mux_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ODTU_MUX block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ODTU_MUX subsystem relative to 
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   odtu_mux_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odtu_mux_handle_t *odtu_mux_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    /* variable declaration */
    odtu_mux_handle_t *odtu_mux_handle;

    PMC_ENTRY();
    
    /* allocate memory for ODTU_MUX handle */
    odtu_mux_handle = (odtu_mux_handle_t*)PMC_CTXT_CALLOC(sizeof(odtu_mux_handle_t), parent);
    
    /* initialize the handle for ODTU_MUX */
    pmc_handle_init(parent, odtu_mux_handle, sys_handle, PMC_MID_DIGI_ODTU_MUX, tsb_name, base_address);
    
    /* Register log strings */
    pmc_log_block_strings_register(ODTU_MUX_LOG_ERR_STRINGS[0], ODTU_MUX_LOG_ERR_TABLE_BASE, ODTU_MUX_LOG_ERR_COUNT);
    
    /* create handles for all children of ODTU_MUX */
    odtu_mux_handle->tgmp_handle = tgmp_ctxt_create((pmc_handle_t *)odtu_mux_handle, base_address+BASE_ADDR_TGMP96_MTSB, sys_handle, "TGMP");
    PMC_CTXT_REGISTER_PTR(&odtu_mux_handle->tgmp_handle, parent);
    odtu_mux_handle->sts_sw_handle = sts_sw_ctxt_create((pmc_handle_t *)odtu_mux_handle, base_address+BASE_ADDR_MUX_STS_STS_SW, sys_handle, "STS_SW");
    PMC_CTXT_REGISTER_PTR(&odtu_mux_handle->sts_sw_handle, parent);
    

    odtu_mux_handle->ghao_mux_state = (ghao_mux_var_t*) PMC_CTXT_CALLOC( ODTU_MUX_GHAO_NUM_CTXT * sizeof(ghao_mux_var_t), odtu_mux_handle);
    PMC_CTXT_REGISTER_PTR(&odtu_mux_handle->ghao_mux_state, odtu_mux_handle);


    PMC_RETURN(odtu_mux_handle);
} /* odtu_mux_ctxt_create */

 
/*******************************************************************************
* odtu_mux_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys all child handles of ODTU_MUX and a handle for a ODTU_MUX block 
*   instance.
*
* INPUTS:
*   *odtu_mux_handle         - pointer to ODTU_MUX handle instance
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
PUBLIC void odtu_mux_ctxt_destroy(odtu_mux_handle_t *odtu_mux_handle)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("odtu_mux_handle->cfg.db_id = %u\n", odtu_mux_handle->cfg.db_id);
    PMC_LOG_TRACE("odtu_mux_handle->cfg.db_handle = %u\n", odtu_mux_handle->cfg.db_handle);

    gen_db_detach(&(odtu_mux_handle->base), odtu_mux_handle->cfg.db_id,
                  odtu_mux_handle->cfg.db_handle);


    tgmp_ctxt_destroy(odtu_mux_handle->tgmp_handle);
    sts_sw_ctxt_destroy(odtu_mux_handle->sts_sw_handle);
    
    PMC_CTXT_FREE(&odtu_mux_handle->ghao_mux_state, odtu_mux_handle);

    PMC_CTXT_FREE(&odtu_mux_handle, odtu_mux_handle);
    
    PMC_RETURN();
} /* odtu_mux_ctxt_destroy */

/*******************************************************************************
* odtu_mux_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODTU_MUX block handle instance.
*
* INPUTS:
*   *odtu_mux_handle         - pointer to ODTU_MUX handle instance
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
PUBLIC void odtu_mux_handle_init(odtu_mux_handle_t *odtu_mux_handle)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("handle set to MUX\n");
    
    /* Update static configurations */
    odtu_mux_handle->cfg.max_chnl_num           = ODTU_MUX_NUM_CHANL;

    /* initialize var_t struct */
    odtu_mux_var_default_init(odtu_mux_handle,&odtu_mux_handle->var);
    
    PMC_LOG_TRACE("ODTU_MUX_MAX_CHNL_NUM set to %u\n", odtu_mux_handle->cfg.max_chnl_num);
   
    /* configure fixed variables for ODTU_MUX. */ 
    tgmp_handle_init(odtu_mux_handle->tgmp_handle, INSTANCE_MUX);
    sts_sw_handle_init(odtu_mux_handle->sts_sw_handle, INSTANCE_MUX_STS_SW);    
    

    /* attach to the databse */
    odtu_mux_handle->cfg.db_id = UTIL_GEN_DB_LINE_CORE_OTN_TX_DB;

    odtu_mux_handle->cfg.db_handle = gen_db_attach(&(odtu_mux_handle->base), odtu_mux_handle->cfg.db_id);

    PMC_RETURN();
} /* odtu_mux_handle_init */

/*******************************************************************************
* odtu_mux_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ODTU_MUX block instance.
*
*
* INPUTS:
*   *odtu_mux_handle        - pointer to ODTU_MUX handle instance.
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
PUBLIC UINT32 odtu_mux_base_address_get(odtu_mux_handle_t *odtu_mux_handle)
{
    PMC_ENTRY();
    PMC_RETURN(odtu_mux_handle->base.base_address);
} /* odtu_mux_base_address_get */

/*******************************************************************************
* odtu_mux_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on odtu_dmx handle instance.
*
* INPUTS:
*   *odtu_mux_handle            - pointer to mapotn handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   mux_stage                   - the top demux stage
*   top_energy_state_reg        - The expected energy state 
*  *prev_stg_mux_var_reg        - The recovery context from the previous stg\n
*                                 It may be NULL
*   *odtu_mux_var_reg          - The recovery context 
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   ODTU_MUX_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
*     incoherences have been detected on communal energy configuration
*     that is incoherent with top energy state
*      WARM : ERROR
*      CRASH: TOP shall put MAPOTN in RESET mode
*   
subsystem shall be put in reset mode 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_handle_restart_init(odtu_mux_handle_t *odtu_mux_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              odtu_mux_stage_instance_t mux_stage,
                                              pmc_energy_state_t top_energy_state_reg,
                                              odtu_mux_recover_var_t *prev_stg_mux_var_reg,
                                              odtu_mux_recover_var_t *odtu_mux_var_reg)
{
    odtu_mux_var_t          *odtu_mux_var_clean = NULL;
    
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
       
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ODTU_MUX_LOG_ERR_STRINGS[0], ODTU_MUX_LOG_ERR_TABLE_BASE, ODTU_MUX_LOG_ERR_COUNT);
    }
    
    PMC_MEMSET(odtu_mux_var_reg,0,sizeof(odtu_mux_recover_var_t));
    odtu_mux_var_reg->mux_stage = mux_stage;
    tgmp_handle_restart_init(odtu_mux_handle->tgmp_handle, restart_init_cfg_ptr, top_energy_state_reg);
    rc = sts_sw_handle_restart_init(odtu_mux_handle->sts_sw_handle, restart_init_cfg_ptr, top_energy_state_reg);    

    /* 
       Recover context from register space        
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only OPERATIONAL channel configruation are recovered
       - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
       - incoherency in register context means that the subsystem shall be shutdown
    */
    if (PMC_SUCCESS == rc)
    {
        rc = odtu_mux_ctxt_reg_recover(odtu_mux_handle,                                      
                                       top_energy_state_reg,
                                       odtu_mux_var_reg);
    }
    
    if (PMC_SUCCESS == rc)
    {
        odtu_mux_var_clean = (odtu_mux_var_t*) PMC_CALLOC(sizeof(odtu_mux_var_t));
        
        /* 
         * Audit register context and memory context 
         *   - Audit power and decide if we should just shut down the subsystem 
         *   - Create a new context that represents the intersection between memory and register space (odtu_mux_var_clean)
         */
        rc = odtu_mux_ctxt_audit(odtu_mux_handle, 
                                 restart_init_cfg_ptr, 
                                 odtu_mux_var_reg,
                                 prev_stg_mux_var_reg,
                                 odtu_mux_var_clean); 
        
        if (restart_init_cfg_ptr->is_warm_restart == FALSE &&  
            PMC_ENERGY_STATE_RUNNING == top_energy_state_reg)
        {
            /* 
             * Cleanup 
             *   - At the end, odtu_mux_var_clean is the new context
             *    - We keep OPERATIONAL channel defined in odtu_mux_var_clean
             *    - We clean (deactivate/deprov) all channels that are not OPERATIONAL
             *       - they may have be partially provisionned
             *    - We ajust power setting based on odtu_mux_var_clean
             */            
            rc = odtu_mux_ctxt_cleanup(odtu_mux_handle, 
                                           odtu_mux_var_reg,
                                           odtu_mux_var_clean); 
            
        }
        
        /* 
           resync odtu_mux_var_reg->var for the use at upper level 
         */
        PMC_MEMCPY(&odtu_mux_var_reg->var,odtu_mux_var_clean,sizeof(odtu_mux_var_t));
        PMC_FREE(&odtu_mux_var_clean);
    }
        
    PMC_RETURN(rc);
} /* odtu_mux_handle_restart_init */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
* odtu_mux_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the switch in ODTU_MUX.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance to be operated on             
*   enbl                - Enable or disable ODTU_MUX \n
*                         1 -- enable \n
*                         0 -- disable
* 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_init(odtu_mux_handle_t *odtu_mux_handle, UINT32 enbl)
{
    PMC_ENTRY();
    
    /* initializes CFC in LWC's */
    tgmp_init(odtu_mux_handle->tgmp_handle, PMC_ENERGY_STATE_RUNNING);
    
    /* initialize expand_ibuf ram based config */
    expand_ibuf_ram_init(odtu_mux_handle->tgmp_handle->expand_ibuf_handle);
    
    /* initialize mp_mgen ram based config */
    mp_mgen_ram_init(odtu_mux_handle->tgmp_handle->mp_mgen_handle);
    
    /* initialize ohfs_insert ram based config */
    ohfs_insert_ram_init(odtu_mux_handle->tgmp_handle->ohfs_insert_handle);
    
    /* initialize txjc_insert ram based config */
    txjc_proc_ram_init(odtu_mux_handle->tgmp_handle->txjc_proc_handle);
    
    /* initialize ram based configuration in STS_SW */
    sts_sw_ram_init(odtu_mux_handle->sts_sw_handle);
    
    /* initialize ODTU MUX top level ram based config */
    odtu_mux_ram_init(odtu_mux_handle);
    
    /* initialize B page for the same ram based configuration */   
    odtu_mux_field_FORCE_CONFIG_ADDR_set(NULL, odtu_mux_handle, 1);
    odtu_mux_field_FORCE_CONFIG_set(NULL, odtu_mux_handle, 1);
    
    /* initialize expand_ibuf ram based config */
    expand_ibuf_ram_init(odtu_mux_handle->tgmp_handle->expand_ibuf_handle);
    
    /* initialize mp_mgen ram based config */
    mp_mgen_ram_init(odtu_mux_handle->tgmp_handle->mp_mgen_handle);
    
    /* initialize ohfs_insert ram based config */
    ohfs_insert_ram_init(odtu_mux_handle->tgmp_handle->ohfs_insert_handle);
    
    /* initialize txjc_insert ram based config */
    txjc_proc_ram_init(odtu_mux_handle->tgmp_handle->txjc_proc_handle);
    
    /* initialize ram based configuration in STS_SW */
    sts_sw_ram_init(odtu_mux_handle->sts_sw_handle);
    
    /* initialize ODTU MUX top level ram based config */
    odtu_mux_ram_init(odtu_mux_handle);
    
    /* undo force config */
    odtu_mux_field_FORCE_CONFIG_set(NULL, odtu_mux_handle, 0);
    odtu_mux_field_FORCE_CONFIG_ADDR_set(NULL, odtu_mux_handle, 0);
   
    /* initializes switch to mux mode */ 
    sts_sw_init(odtu_mux_handle->sts_sw_handle, 0, enbl);
    
    PMC_RETURN(PMC_SUCCESS);  
} /* odtu_mux_init */

/*******************************************************************************
*  odtu_mux_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the initial make-up of a LO channel in ODTU_MUX.
*   The tributary slot type, number of tributary slots and operational mode of
*   the channel is defined in this API. 
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - channel ID to be configured (HO or MO)
*                         Valid range: 0 - 95
*   mode                - operational mode of the LO ODU channel \n
*                         0 -- unchannelized \n
*                         1 -- channelized operational
*                         2 -- null frame mode
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_chnl_init(odtu_mux_handle_t *odtu_mux_handle,
                                    UINT32 chnl_id,
                                    odtu_mux_chnl_mode_t mode)                            
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle!=NULL, ODTU_MUX_LOG_ERR_INVALID_ARG, 0, 0);
    
    PMC_LOG_TRACE("%s, %s, %d, mode = %u\n", __FILE__, __FUNCTION__, __LINE__, mode);
    if (mode == ODTU_MUX_CHNL_UNCHANNELIZED)
    {   /* passthru */
        PMC_LOG_TRACE("%s, %s, %d, ODTU_MUX HO chnl_id = %u in NORMAL MODE\n", __FILE__, __FUNCTION__, __LINE__, chnl_id);
        odtu_mux_field_PASSTHRU_set(NULL, odtu_mux_handle, chnl_id, TRUE);
        PMC_LOG_TRACE("%s, %s, %d, Set PASSTHRU\n", __FILE__, __FUNCTION__, __LINE__);
    }
    else if (mode == ODTU_MUX_CHNL_CHANNELIZED)
    {   /* channelized payload */
        odtu_mux_field_PASSTHRU_set(NULL, odtu_mux_handle, chnl_id, FALSE);
    }
    else {
        /* ODTU_MUX_NULL_FRAME_MODE */
        odtu_mux_field_PASSTHRU_set(NULL, odtu_mux_handle, chnl_id, FALSE);
        /*result = ohfs_insert_null_frame_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                            chnl_id, 1);*/
    }              
     
    PMC_RETURN(result);
} /* odtu_mux_chnl_init */

/*******************************************************************************
*  odtu_mux_null_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the null channel in ODTU_MUX. This function can
*   be called to configure NULL payload in HO or MO channel.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - channel ID to be configured (HO or MO)
*                         Valid range: 0 - 95
*   null_frame_en       - NULL frame mode \n
*                         0 -- disable NULL frame mode \n
*                         1 -- enable NULL frame mode
*   
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_null_chnl_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                        UINT32 chnl_id,
                                        UINT32 null_frame_en)                            
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle!=NULL, ODTU_MUX_LOG_ERR_INVALID_ARG, 0, 0);
    
    /* ODTU_MUX_NULL_FRAME_MODE */
    result = ohfs_insert_null_frame_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                        chnl_id, null_frame_en);       
     
    PMC_RETURN(result);
} /* odtu_mux_null_chnl_cfg */


/*******************************************************************************
*  odtu_mux_null_chan_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the null channel in ODTU_MUX. This function can
*   be called to configure NULL payload in HO or MO channel.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - channel ID to be configured (HO or MO)
*                         Valid range: 0 - 95
*   
*
* OUTPUTS:
*   *null_frame_en      - NULL frame mode \n
*                         0 -- disable NULL frame mode \n
*                         1 -- enable NULL frame mode
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_null_chan_mode_get(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id,
                                             BOOL8 *null_frame_en)                            
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle!=NULL, ODTU_MUX_LOG_ERR_INVALID_ARG, 0, 0);
    
    /* ODTU_MUX_NULL_FRAME_MODE */
    *null_frame_en = ohfs_insert_null_frame_mode_get(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);       
     
    PMC_RETURN(result);
} /* odtu_mux_null_chan_mode_get */

/*******************************************************************************
* odtu_mux_ho_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_mux_ho_chnl_prov() API must have been
*   called to provision a channel.                                                                               
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - HO ODU channel ID to be configured
*                         Valid range: 0 - 95                    
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
PUBLIC PMC_ERROR odtu_mux_ho_chnl_activate(odtu_mux_handle_t *odtu_mux_handle, 
                                           UINT32 chnl_id) 
{
    /* variable declaration */
    UINT32 i;
    UINT32 last_cycle;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (odtu_mux_handle->var.prov_mode == 0 
        && (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_EQUIPPED
        && (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state != (UINT32)ODTU_MUX_CHNL_PASSTHRU)
    {
        PMC_RETURN(ODTU_MUX_ERR_CHNL_INVALID_STATE); /* that channel is not in provisioned state*/
    } 
    else if (odtu_mux_handle->var.prov_mode == 1 
             && odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state ==  (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
    {
        /* bulk provisioniong mode and HO has been activated already */
        PMC_RETURN(ODTU_MUX_CHNL_OPERATIONAL);
    }
    
    PMC_LOG_TRACE("odtu_mux_handle->var.ho_chnl_cfg[%u].num_tribslot= %u\n", chnl_id, odtu_mux_handle->var.ho_chnl_cfg[chnl_id].num_tribslot);
    
    /* HO Switch Config */
    result = sts_sw_ho_chnl_cfg(odtu_mux_handle->sts_sw_handle, chnl_id);
    
    if (PMC_SUCCESS == result)
    {
        result = sts_sw_ho_chnl_ts_cfg(odtu_mux_handle->sts_sw_handle, chnl_id);
    }
    
    if (PMC_SUCCESS == result)
    {
        /* how am I going to know that this is two stage muxing and the MO is ODU3e1 or ODU3e2 */
        if ((odtu_mux_handle->var.ho_chnl_cfg[chnl_id].oduk_type == UTIL_GLOBAL_ODU3E1
            || odtu_mux_handle->var.ho_chnl_cfg[chnl_id].oduk_type == UTIL_GLOBAL_ODU3E2)
            && odtu_mux_handle->var.ho_chnl_cfg[chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)   
        {
            if (odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id == chnl_id)
            {
                last_cycle = odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry;
                result = sts_sw_ho_last_cycle_set(odtu_mux_handle->sts_sw_handle, 
                                                  chnl_id, last_cycle);
            }
            else if (odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id == chnl_id)
            {
                last_cycle = odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry;
                result = sts_sw_ho_last_cycle_set(odtu_mux_handle->sts_sw_handle, 
                                                  chnl_id, last_cycle);
            }           
        }
    }                                                
        
    if (PMC_SUCCESS == result)
    {
        
        /* configure CAL_EN */
        for (i = 0; i < odtu_mux_handle->var.ho_chnl_cfg[chnl_id].num_cal_entries; i++)
        {
            PMC_LOG_TRACE("odtu_mux_handle->var.ho_chnl_cfg[%u].cal_array[%u] = %u\n", chnl_id, i, odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array[i]);
            /* configure CAL_EN */
            odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array[i], 1);        
        }    
        /* Channel reset is released by configuring HO_CH_RSTB to '1' */
        result = odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle, chnl_id, TRUE);

    }
    
    if (PMC_SUCCESS == result)
    {
        /* Update var context for a provisioned channel */
        if ((UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state == (UINT32)ODTU_MUX_CHNL_EQUIPPED)
        {
            odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_OPERATIONAL;
        }          
    }
    
    PMC_RETURN(result);

} /* odtu_mux_ho_chnl_activate */

/*******************************************************************************
* odtu_mux_ho_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function is used to hold the channel in reset before provisioning the
*   channel or this function is also used to deactivate the HO channel that has 
*   been already provisioned.                                                                             
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - HO ODU channel ID to be configured
*                         Valid range: 0 - 95                    
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
PUBLIC PMC_ERROR odtu_mux_ho_chnl_deactivate(odtu_mux_handle_t *odtu_mux_handle, 
                                             UINT32 chnl_id) 
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    if ((UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_START
        && (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_OPERATIONAL
        && (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state != (UINT32)ODTU_MUX_CHNL_PASSTHRU)
    {
        PMC_RETURN(ODTU_MUX_ERR_CHNL_INVALID_STATE); /* that channel is not in provisioned state*/
    }
   
    /* Channel is put in reset by configuring HO_CH_RSTB to '0' */
    odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle, chnl_id, FALSE);
    
    /* Update var context for a provisioned channel */
    if ((UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state ==  (UINT32)ODTU_MUX_CHNL_OPERATIONAL
        || (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state == (UINT32)ODTU_MUX_CHNL_PASSTHRU)
    {
        odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_EQUIPPED;
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* odtu_mux_ho_chnl_deactivate */

/*******************************************************************************
* odtu_mux_lo_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function is used to hold the LO ODU channel in reset before 
*   provisioning. This function is also used to deactivate the LO ODU channel 
*   that has been already provisioned.
*                                          
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95 
*   ho_chnl_id          - HO ODU channel ID that the LO belongs to.
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deactivate(odtu_mux_handle_t *odtu_mux_handle, 
                                             UINT32 chnl_id, UINT32 ho_chnl_id) 
{
    /* variable declaration */
    PMC_ENTRY();

    if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_START
        && (UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
    {
        PMC_RETURN(ODTU_MUX_ERR_CHNL_INVALID_STATE); /* that channel is not in provisioned state*/
    }

    /* Channel is put in reset by configuring LO_CH_RSTB to '0' */
    odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle, chnl_id, FALSE);
    
    /* Update var context for a provisioned channel */
    if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
    {
        odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_EQUIPPED;
    }
   
    PMC_RETURN(PMC_SUCCESS);

} /* odtu_mux_lo_chnl_deactivate */


/*******************************************************************************
* odtu_mux_lo_chnl_deactivate_ch_state_chk_only
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function is used to hold the LO ODU channel in reset before 
*   provisioning. This function is also used to deactivate the LO ODU channel 
*   that has been already provisioned.
*                                          
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95 
*   ho_chnl_id          - HO ODU channel ID that the LO belongs to.
*                         Valid range: 0 - 95                   
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
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deactivate_ch_state_chk_only(odtu_mux_handle_t *odtu_mux_handle, 
                                                               UINT32 chnl_id, UINT32 ho_chnl_id) 
{
    /* variable declaration */
    UINT32 lo_rst_val;

    PMC_ENTRY();

    if (FALSE == pmc_coreotn_mux_empty_ho_order_revision(&odtu_mux_handle->base))
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    
    lo_rst_val = (odtu_mux_field_LO_CH_RSTB_get(NULL, odtu_mux_handle, chnl_id)); 
    
    PMC_LOG_TRACE("current LO CH RSTB = %u\n", lo_rst_val);
    
    if (lo_rst_val != 0)
    {
        if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_START
            && (UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
        {
            PMC_RETURN(ODTU_MUX_ERR_CHNL_INVALID_STATE); /* that channel is not in provisioned state*/
        }
    }
    
    
    /* Update var context for a provisioned channel */
    if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
    {
        odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_EQUIPPED;
    }
   
    PMC_RETURN(PMC_SUCCESS);

} /* odtu_mux_lo_chnl_deactivate_ch_state_chk_only */


/*******************************************************************************
* odtu_mux_lo_chnl_deactivate_only
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function is used to hold the LO ODU channel in reset before 
*   provisioning. This function is also used to deactivate the LO ODU channel 
*   that has been already provisioned.
*                                          
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95                   
*                                                                               
* OUTPUTS:                                                                      
*   *dummy_ch           - dummy channel status
*                         TRUE -- this LO channel is a dummy channel
*                         FALSE -- this LO channel is not a dummy channel                                                                     
*                                                                               
* RETURNS:                                                                      
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deactivate_only(odtu_mux_handle_t *odtu_mux_handle, 
                                                  UINT32 chnl_id, 
                                                  BOOL8 *dummy_ch) 
{
    /* variable declaration */
    UINT32 lo_rst_val;
    
    PMC_ENTRY();
         
    
    lo_rst_val = (odtu_mux_field_LO_CH_RSTB_get(NULL, odtu_mux_handle, chnl_id)); 

    
    /* Channel is put in reset by configuring LO_CH_RSTB to '0' */
    odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle, chnl_id, FALSE);
    
    if (lo_rst_val == 1)
    {
        *dummy_ch = TRUE;
    }
    else {
        *dummy_ch = FALSE;
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* odtu_mux_lo_chnl_deactivate_only */


/*******************************************************************************
* odtu_mux_lo_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function activates the LO ODU channel that has been already provisioned.
*   Prior to calling this API, odtu_mux_lo_chnl_prov() API must have been
*   called to provision a channel.                                                                               
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO ODU channel ID that the LO belongs to.
*                         Valid range: 0 - 95                    
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
PUBLIC PMC_ERROR odtu_mux_lo_chnl_activate(odtu_mux_handle_t *odtu_mux_handle, 
                                           UINT32 chnl_id, UINT32 ho_chnl_id) 
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state != (UINT32)ODTU_MUX_CHNL_EQUIPPED
        && (UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state != (UINT32)ODTU_MUX_CHNL_START)
    {
        PMC_RETURN(ODTU_MUX_ERR_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    } 
    
    if (PMC_SUCCESS == result)
    {                                    
        /* Channel reset is released by configuring HO_CH_RSTB to '1' */
        result = odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle, chnl_id, TRUE);
    }

    if (PMC_SUCCESS == result)
    {
        if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state == (UINT32)ODTU_MUX_CHNL_EQUIPPED)
        {
            odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_OPERATIONAL;
        }            
    }
   
    PMC_RETURN(result);

} /* odtu_mux_lo_chnl_activate */

/*******************************************************************************
* odtu_mux_lo_chnl_activate_only
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function activates the LO ODU channel that has not been already provisioned.
*   This is a dummy LO channel that needs to be activated in absence of a 
*   real LO channel that uses the same channel ID as the MO channel.                                                                             
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95                 
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
PUBLIC PMC_ERROR odtu_mux_lo_chnl_activate_only(odtu_mux_handle_t *odtu_mux_handle, 
                                                UINT32 chnl_id) 
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (PMC_SUCCESS == result)
    {                                    
        /* Channel reset is released by configuring HO_CH_RSTB to '1' */
        result = odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle, chnl_id, TRUE);
    }
   
    PMC_RETURN(result);

} /* odtu_mux_lo_chnl_activate_only */

/*******************************************************************************
*  odtu_mux_ho_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to provision a new HO channel in ODTU_MUX. 
*
*   Note that prior to calling this API, the HO channel must have been 
*   initialized with odtu_mux_chnl_init() API. 
*   
*   While provisioning the HO channel, the HO channel must be held in reset by  
*   calling odtu_mux_ho_chnl_deactivate() API. 
*
*   All corresponding tributary slots and the associated LO channels being added 
*   to this HO channel must be configured using odtu_mux_lo_chnl_prov().
*   Call odtu_mux_lo_chnl_prov() API for all tributary slots occupied by the HO
*   channel being provisioned.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   mux_stage_id        - mux stage id.
*                         Valid range: 0 - 2
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   ts_type             - size of tributary slot. See util_global_odu_line_payload_t
*                         UTIL_GLOBAL_ODU_UNCHANNELIZED  = 0,
*                         UTIL_GLOBAL_ODU_TS_1G25        = 1, 
*                         UTIL_GLOBAL_ODU_TS_2G5         = 2, 
*   offline_update      - indication for programming active/standby pages
*                         0 -- active page configuration
*                         1 -- offline page configuration          
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ho_chnl_prov(odtu_mux_handle_t *odtu_mux_handle,
                                       UINT32 mux_stage_id,
                                       UINT32 chnl_id, util_global_odukp_type_t ho_odu_type,
                                       util_global_odu_line_payload_t ts_type,
                                       BOOL8 offline_update)                             
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 max_ts_num; 
    UINT32 frm_num = 0;
    UINT32 i;
    UINT32 valid; 
    odtu_mux_chnl_state_t chnl_state;
    UINT32 first_cycle, last_cycle;
    
    line_core_otn_db_key key;
    UINT32 num_of_recs = 0;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;

    line_core_otn_db_key ho_key;
    UINT32 ho_num_of_recs = 0;
    line_core_otn_db_query_res *ho_q_result;
    UINT32 mo_num_of_recs;
    line_core_otn_db_rec new_rec;
    
    UINT32 dummy_idx;
    UINT32 valid_null_cycle;
    

    PMC_ENTRY();
    
    /* argument checking */
    if (chnl_id > 95 || (mux_stage_id != 2 && (ho_odu_type == UTIL_GLOBAL_ODUFLEX_CBR || ho_odu_type == UTIL_GLOBAL_ODUFLEX_GFP)) 
        || ho_odu_type > UTIL_GLOBAL_LAST_ODUK || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD)
    {
        PMC_RETURN(ODTU_MUX_ERR_CHNL_INVALID_STATE);
    }
    
    if (offline_update == FALSE)
    {
        /* check the current channel state */
        if ((UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_START
            && (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_PASSTHRU)
        {
            PMC_RETURN(ODTU_MUX_ERR_CHNL_STATE_NOT_START); /* that channel is not in start state*/
        }
    
        /* configure MEM_HO_ID first before configuration */
        (void) odtu_mux_ho_id_mem_cfg(odtu_mux_handle, chnl_id);           
    }
    
    if (PMC_SUCCESS == ret_code)
    {
        if (mux_stage_id == 2 && (ho_odu_type == UTIL_GLOBAL_ODUFLEX_CBR || ho_odu_type == UTIL_GLOBAL_ODUFLEX_GFP))
        {  
            max_ts_num = 0;
        }
        else 
        {
            ret_code = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX, 
                                                         ho_odu_type, ts_type,
                                                         &max_ts_num);                                                        
        }                                                         
    }
                                                         
    PMC_LOG_TRACE("max_ts_num = %u\n",  max_ts_num);
    
    if (PMC_SUCCESS == ret_code)
    {
        if (mux_stage_id != 2)
        {
            /* retrieve the calendar entries for the HO channel from the DB */
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = chnl_id;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
            
            /* find the total number of records that matches the HO channel ID */
            num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                       odtu_mux_handle->cfg.db_id,
                                       odtu_mux_handle->cfg.db_handle, (void *)&key,
                                       (void **)&q_result);
            temp_result = *q_result;
            
            if (num_of_recs == 0)
            {
                PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
            }
                                       
        }
        else 
        {
            /* retrieve the calendar entries for the HO channel from the DB */
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl_id;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
            
            /* find the total number of records that matches the HO channel ID */
            num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                       odtu_mux_handle->cfg.db_id,
                                       odtu_mux_handle->cfg.db_handle, (void *)&key,
                                       (void **)&q_result);
            
            temp_result = *q_result;
            if (num_of_recs == 0)
            {
                PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
            }
        }                                                                                 
    }

    /* temporary fix to get 31 ts in FMF2 */
    if (mux_stage_id == 2 && num_of_recs == 31 && ho_odu_type == UTIL_GLOBAL_ODU3
        && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {        
        /* get an idle calendar entry that is unused by HO ODU4 and 
        ** assign it to the MO ODU3 channel. Read from the HO calendar 
        ** entries 
        */  
        
        ret_code = odtu_mux_first_valid_dummy_find(odtu_mux_handle,
                                                   temp_result.p_recs[0]->calendar_entry, 
                                                   &valid_null_cycle,
                                                   &dummy_idx);

        if (offline_update == FALSE)
        {                            
            for (i = valid_null_cycle; i < 96; i++)
            {
                /* query the record to update */
                ho_key.calendar_entry = i;
                ho_key.ho_channel = DO_NOT_USE_KEY;
                ho_key.mo_channel = DO_NOT_USE_KEY;
                ho_key.lo_channel = DO_NOT_USE_KEY;
                ho_key.trib_slot = DO_NOT_USE_KEY;
                
                /* find the total number of records that matches the HO channel ID */
                ho_num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                              odtu_mux_handle->cfg.db_id,
                                              odtu_mux_handle->cfg.db_handle, 
                                              (void *)&ho_key,
                                              (void **)&ho_q_result);
            
                /* find an idle calendar entry */
                if (ho_q_result->p_recs[0]->ho_channel == ODTU_MUX_DB_ENTRY_UNKNOWN
                    && ho_q_result->p_recs[0]->cal_entry_status == ODTU_MUX_DB_ENTRY_UNKNOWN 
                    && i != UTIL_GLOBAL_DMX_DUMMY_SCHD_ID_1 && i != UTIL_GLOBAL_DMX_DUMMY_SCHD_ID_2)
                {
                                       
                   /* idle calendar entry */
                   new_rec.calendar_entry = i;
                   new_rec.ho_channel = ODTU_MUX_DB_ENTRY_UNKNOWN;
                   new_rec.mo_channel = chnl_id;
                   new_rec.lo_channel = DO_NOT_USE_KEY;
                   new_rec.trib_slot = 80+i; 
                   new_rec.cal_entry_status = 1;    
                   new_rec.ts_port_ho_mo = DO_NOT_USE_KEY;
                   new_rec.ts_port_mo_lo = DO_NOT_USE_KEY;
                   
                   gen_db_add_record(&(odtu_mux_handle->base),
                                     odtu_mux_handle->cfg.db_id,
                                     odtu_mux_handle->cfg.db_handle,
                                     (void *)&new_rec);
                                     
                    break;                    
                } 
            }
        }
        
       
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
        
        /* find the total number of records that matches the HO channel ID */
        mo_num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                      odtu_mux_handle->cfg.db_id,
                                      odtu_mux_handle->cfg.db_handle, (void *)&key,
                                      (void **)&q_result);
        
        num_of_recs = mo_num_of_recs;
        temp_result = *q_result;
        
        PMC_LOG_TRACE("num_of_recs for HO ODU3 special case = %u   mo_num_of_recs = %u \n", num_of_recs, mo_num_of_recs);                         
    } 
    else if (mux_stage_id == 2 && num_of_recs == 33 
             && (ho_odu_type == UTIL_GLOBAL_ODU3E1 || ho_odu_type == UTIL_GLOBAL_ODU3E2)
             && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        /* only want 32 calendar, so mute the last calendar entry that 
        ** belongs to this HO (MO ODU3e1/ODU3e2)
        */
        if (offline_update == FALSE)
        {
            if (temp_result.p_recs[32]->mo_channel == chnl_id)
            {
                /* 33rd calendar entry */
                if (odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry == 0x7F)
                {
                    odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry  = temp_result.p_recs[32]->calendar_entry;
                    odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id = chnl_id;
                }
                else if (odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry == 0x7F)
                {
                    odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry = temp_result.p_recs[32]->calendar_entry;
                    odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id = chnl_id; 
                }
                else {
                    /* cannot have more than 2 ODU3e1/3e2 in one HO */
                    PMC_RETURN(ODTU_MUX_LOG_CODE_HO_TS_FULL);
                }
                
                new_rec = *temp_result.p_recs[32];
                new_rec.mo_channel = DO_NOT_USE_KEY;
                new_rec.lo_channel = DO_NOT_USE_KEY; 
                new_rec.trib_slot = DO_NOT_USE_KEY; /* watch for this */
                new_rec.ts_port_ho_mo = 2;
                new_rec.cal_entry_status = chnl_id;
                
                /* send back the result to the DB */ 
                gen_db_update_record(&(odtu_mux_handle->base),
                                     odtu_mux_handle->cfg.db_id,
                                     odtu_mux_handle->cfg.db_handle,
                                     &new_rec);
                                     
                /* update the number of calendar entries that belong to this MO */
                num_of_recs = 32;
                odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array[32] = 0xFF;                                  
            }   
            else {
                PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
            }
        }
        
        if (PMC_SUCCESS == ret_code)
        {
            /* write a function to perform dummy scheduling... */
            ret_code = odtu_mux_dummy_schd_cfg(odtu_mux_handle, chnl_id, ho_odu_type, 
                                               temp_result.p_recs[32]->calendar_entry, ts_type);
        }                
    }
    
    /* Update the database for the ts_id */
    for (i = 0; i < num_of_recs; i++)
    {
        odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array[i] = temp_result.p_recs[i]->calendar_entry;
        PMC_LOG_TRACE("odtu_mux_handle->var.ho_chnl_cfg[%u].cal_array[%u] = %u\n", chnl_id, i, odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array[i]);
    }
    

    max_ts_num = num_of_recs;

    if (PMC_SUCCESS == ret_code)
    {
        /* if HO ODU is ODU4 */
        if (ho_odu_type == UTIL_GLOBAL_ODU4 && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
        {
            /* configure OMFI_INSERT */
            ret_code = ohfs_insert_omfi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                            chnl_id, 1);
        }
        else {
            /* for non-ODU4 rate set to 0 for this channel */
            ret_code = ohfs_insert_omfi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                            chnl_id, 0);
        }                                                        
    }
    PMC_ATOMIC_YIELD(odtu_mux_handle->base.parent_handle->parent_handle,0);
    if (PMC_SUCCESS == ret_code)
    {        
        /* Update the database for the LO channel ID */
        /* ts could range from 0 - max num ts in HO */ /* upto ODU3 now */
        for (i = 0; i < num_of_recs; i++)
        {
            /* calendar entry is not assigned to any tributary slot/LO channel */
            /* update MO channel ID to 'calendar_entry' */
            new_rec = *temp_result.p_recs[i];
            new_rec.calendar_entry = temp_result.p_recs[i]->calendar_entry;
            if (ts_type == 0)
            {
                new_rec.ts_port_ho_mo = 0;
            }
            else {
                if (mux_stage_id == 1)
                {
                    new_rec.ts_port_ho_mo = 1;
                }
                else if (mux_stage_id == 2)
                {
                    new_rec.ts_port_ho_mo = 2;
                }
            }
                
            new_rec.cal_entry_status = 0;
            
            PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u, new_rec.ts_port_ho_mo = %u\n",
                      new_rec.ho_channel,
                      new_rec.trib_slot,
                      new_rec.mo_channel,
                      new_rec.lo_channel,
                      new_rec.ts_port_ho_mo);
            
            /* send back the result to the DB */ 
            gen_db_update_record(&(odtu_mux_handle->base),
                                 odtu_mux_handle->cfg.db_id,
                                 odtu_mux_handle->cfg.db_handle,
                                 &new_rec);
        }

        ret_code = ohfs_insert_ho_chnl_prov(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                            chnl_id, ho_odu_type, ts_type, num_of_recs, offline_update);                                            
    }
    
    for  (i = 0; i < num_of_recs; i++)
    {
        if (PMC_SUCCESS == ret_code)
        {
            if (ho_odu_type != UTIL_GLOBAL_ODU0 && ho_odu_type != UTIL_GLOBAL_ODUFLEX_CBR && ho_odu_type != UTIL_GLOBAL_ODUFLEX_GFP)
            {
                /* MSI configuration - set to Unallocated */
                ret_code = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                               temp_result.p_recs[i]->calendar_entry, ho_odu_type, 
                                               UTIL_GLOBAL_ODUK_DONT_CARE, ts_type, 0);  
            } 
        }                
    }
                        
    if (offline_update == FALSE)
    {
        /* Configure CFC in OHFS_INSERT */
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = ohfs_insert_cfc_fifo_end_addr_only_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                              chnl_id, num_of_recs);
        }    
    
        /* Configure end addr in EXPAND_IBUF if unchannelized */
        if (PMC_SUCCESS == ret_code && ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
        {
            /* configure CFC_FILL_LVL */
            ret_code = expand_ibuf_cfc_xoff_level_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, chnl_id, num_of_recs);

        }                                                              
                                     
        if (PMC_SUCCESS == ret_code)
        {
            /* OHFS_INSERT: IPT configuration */
            for  (i = 0; i < num_of_recs; i++)
            {
                valid = 1;
                
                /* configure CFC_IPT table */
                ret_code = ohfs_insert_cfc_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                   temp_result.p_recs[i]->calendar_entry, valid, chnl_id, frm_num);
                                       
                if (PMC_SUCCESS == ret_code)
                {
                    /* configure IPT for the occupied tribslot */
                    ret_code = ohfs_insert_ho_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                      temp_result.p_recs[i]->calendar_entry, 
                                                      chnl_id, frm_num, valid); 
                }
                                   

                if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == TRUE)
                {
                    first_cycle = (i == 0)? 1:0;
                    last_cycle = (i == num_of_recs-1)? 1:0;
                }
                else
                {
                    if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED) {
                        first_cycle = 1;
                        last_cycle = 1;
                    } else {
                        first_cycle = (i == 0)? 1:0;
                        last_cycle = (i == num_of_recs-1)? 1:0;
                    }
                }
               
                if (PMC_SUCCESS == ret_code)
                {    
                    /* configure HO_MF_TOT */
                    ret_code = expand_ibuf_ho_chnl_prov_all(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                            temp_result.p_recs[i]->calendar_entry, chnl_id,
                                                            ho_odu_type,
                                                            ts_type,
                                                            first_cycle, last_cycle, 0);
                }
                
                if (PMC_SUCCESS == ret_code)
                {
                    ret_code = sts_sw_lo_chnl_cfg(odtu_mux_handle->sts_sw_handle, chnl_id,
                                                  temp_result.p_recs[i]->calendar_entry,
                                                  0x7F, 0, FALSE);
                }                                         
                    
                if (PMC_SUCCESS == ret_code && ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
                {
                    if (TRUE == pmc_coreotn_mux_empty_ho_keep_in_sync_revision(&odtu_mux_handle->base))
                    {
                        ret_code = expand_ibuf_lo_cycle_cfg_all(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                                                temp_result.p_recs[i]->calendar_entry, 1, 1);
                    }
                    else
                    {
                        ret_code =  expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                         temp_result.p_recs[i]->calendar_entry, 1);
   
                    }

                    if(i==0)
                    {

                        if (PMC_SUCCESS == ret_code)
                        {
                      
                            ret_code = cfc_fifo_build(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                                      chnl_id, num_of_recs);        
                        }
                    }
                }
                frm_num++;
            }


            /* need to overwrite EXPAND.HO_LAST_CYCLE = 0 for the real last cycle of ODU3e1/3e2 */

            if (PMC_SUCCESS == ret_code)
            {
                if (mux_stage_id == 2 
                    && (ho_odu_type == UTIL_GLOBAL_ODU3E1 || ho_odu_type == UTIL_GLOBAL_ODU3E2)
                    && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                { 
                    ret_code = expand_ibuf_ho_last_cycle_set(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                             temp_result.p_recs[31]->calendar_entry, 0);
                }
            }                                                           
        }                
    
    
        /* STS_SW HO channel init */
        if (PMC_SUCCESS == ret_code)
        {   
            ret_code = sts_sw_ho_chnl_init(odtu_mux_handle->sts_sw_handle, chnl_id, ho_odu_type, ts_type,
                                           num_of_recs, odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array);                                     
        }
            
        if (PMC_SUCCESS == ret_code)
        {
            if (ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                chnl_state = ODTU_MUX_CHNL_EQUIPPED;
            }
            else {
                chnl_state = ODTU_MUX_CHNL_PASSTHRU;
            }
                            
            /* update HO channel information */
            /* consider removing ho_chnl_cfg[].lo_chnl_ids from the channel context
            ** as there is another structure that holds the same information 
            */
            odtu_mux_ho_chnl_var_update(odtu_mux_handle, chnl_id, chnl_state, ho_odu_type, 
                                        max_ts_num, 
                                        odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array,
                                        0, ts_type, 0, num_of_recs);                                        
        } 

        
        if (PMC_SUCCESS == ret_code)
        {
            /* take CFC out of reset */
            ret_code = ohfs_insert_chnl_activate(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);  
        }
        
        if (PMC_SUCCESS == ret_code && ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
        {
            ret_code = expand_ibuf_chnl_activate(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, chnl_id); 
        }
    }  


    if (TRUE == pmc_coreotn_mux_empty_ho_keep_in_sync_revision(&odtu_mux_handle->base) && (ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED || offline_update != FALSE))
    {
        for (i = 0; i < num_of_recs; i++)
        {
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                                    temp_result.p_recs[i]->calendar_entry, 1);
            }     
        } 
    }                                                       

    if (PMC_SUCCESS == ret_code)
    {
        expand_ibuf_field_MEM_LO_ID_set(NULL, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, 0x7F);                                  
    }
  
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odtu_mux_dbg_mask_somf_sync_set(odtu_mux_handle, chnl_id, 1);
    }
    
    PMC_RETURN(ret_code);
} /* odtu_mux_ho_chnl_prov */

/*******************************************************************************
*  odtu_mux_ho_chnl_offline_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to provision a new HO channel in ODTU_MUX. 
*
*   Note that prior to calling this API, the HO channel must have been 
*   initialized with odtu_mux_chnl_init() API. 
*   
*   While provisioning the HO channel, the HO channel must be held in reset by  
*   calling odtu_mux_ho_chnl_deactivate() API. 
*
*   All corresponding tributary slots and the associated LO channels being added 
*   to this HO channel must be configured using odtu_mux_lo_chnl_prov().
*   Call odtu_mux_lo_chnl_prov() API for all tributary slots occupied by the HO
*   channel being provisioned.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   mux_stage_id        - mux stage id. Valid value: 1 or 2
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   ts_type             - size of tributary slot. See util_global_odu_line_payload_t
*                         UTIL_GLOBAL_ODU_UNCHANNELIZED  = 0,
*                         UTIL_GLOBAL_ODU_TS_1G25        = 1, 
*                         UTIL_GLOBAL_ODU_TS_2G5         = 2,           
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ho_chnl_offline_update(odtu_mux_handle_t *odtu_mux_handle,
                                                 UINT32 mux_stage_id,
                                                 UINT32 chnl_id, util_global_odukp_type_t ho_odu_type,
                                                 util_global_odu_line_payload_t ts_type)                             
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = odtu_mux_ho_chnl_prov(odtu_mux_handle, mux_stage_id, chnl_id, ho_odu_type,
                                   ts_type, TRUE);
    PMC_RETURN(result);                                     
} /* odtu_mux_ho_chnl_offline_update */

/*******************************************************************************
*  odtu_mux_ho_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to deprovision a HO channel in ODTU_MUX. 
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   mux_stage           - enum for mux stage. See odtu_mux_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   offline_update      - indicates if the deprovisioning operation is for
*                         updating offline page.
*                         0 -- initial config
*                         1 -- offline update       
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ho_chnl_deprov(odtu_mux_handle_t *odtu_mux_handle,
                                         odtu_mux_stage_instance_t mux_stage,
                                         UINT32 chnl_id, BOOL8 offline_update)                             
{
    /* variable declaration */
    PMC_ERROR ret_code;
    UINT32 i;
    odtu_mux_chnl_state_t chnl_state;
    
    line_core_otn_db_key key;
    UINT32 num_of_recs = 0;
    line_core_otn_db_query_res *q_result;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl_id > 95 || mux_stage > ODTU_MUX_STG_TWO || offline_update > 1)
    {
        PMC_RETURN(ODTU_MUX_LOG_ERR_INVALID_ARG);
    }
    
    if (offline_update == FALSE)
    {
        /* check the current channel state */
        if ((UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_EQUIPPED
            && (UINT32)odtu_mux_handle->var.ho_chnl_cfg[chnl_id].state != (UINT32)ODTU_MUX_CHNL_PASSTHRU)
        {
            PMC_RETURN(ODTU_MUX_ERR_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in start state*/
        }
    }
    PMC_ATOMIC_YIELD(odtu_mux_handle->base.parent_handle->parent_handle,0);
    /* put CFC fifo in reset */
    ret_code = ohfs_insert_chnl_deactivate(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);
    
    if (PMC_SUCCESS == ret_code && odtu_mux_handle->var.ho_chnl_cfg[chnl_id].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        ret_code = expand_ibuf_chnl_deactivate(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, chnl_id); 
    }
    
    if (PMC_SUCCESS == ret_code)
    {
        /* configure MEM_HO_ID first before configuration */
        ret_code = odtu_mux_ho_id_mem_cfg(odtu_mux_handle, chnl_id);
    }
     
    /* no LO exists, tear down HO */
    if (mux_stage != ODTU_MUX_STG_TWO)
    {
        /* ho is being deprovisioned */
        /* retrieve the calendar entries for the HO channel from the DB */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = chnl_id;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
        
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                   odtu_mux_handle->cfg.db_id,
                                   odtu_mux_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result); 
    }
    else {
        /* mo is being deprovisioned */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
        
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                   odtu_mux_handle->cfg.db_id,
                                   odtu_mux_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result); 
    }
    
    if (num_of_recs == 0) PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
    
    if (PMC_SUCCESS == ret_code)
    {        
        for (i = 0; i < num_of_recs; i++)
        {
            odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, q_result->p_recs[i]->calendar_entry, 0);    
        }                                       
    }
    
    /* deprov channel based cofigurations */
    if (PMC_SUCCESS == ret_code)
    {
        /* deprov ohfs insert channel */
        ret_code = ohfs_insert_ho_chnl_deprov(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);
    }        
    
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = sts_sw_ho_chnl_deprov(odtu_mux_handle->sts_sw_handle, chnl_id);
    }        
     
    
    /* deprov HO calendar entry based configurations */
    if (PMC_SUCCESS == ret_code)
    {
        for (i = 0; i < num_of_recs; i++)
        {
            ret_code = expand_ibuf_ho_cal_cycle_tot_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                        q_result->p_recs[i]->calendar_entry, chnl_id,
                                                        UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                        LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 1);
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = expand_ibuf_ho_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                    q_result->p_recs[i]->calendar_entry, 0, 0, 0x7F);
            } 
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = expand_ibuf_ho_mf_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                 q_result->p_recs[i]->calendar_entry,
                                                 UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                 LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 1);
            }
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = ohfs_insert_ho_calendar_cfg_deprov(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                              q_result->p_recs[i]->calendar_entry);
            }
         
            if (PMC_SUCCESS == ret_code)
            {                                             
                ret_code = ohfs_insert_cfc_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                   q_result->p_recs[i]->calendar_entry, 0, 0x7F, 0);
            }                                           
                                                  
            if (PMC_SUCCESS == ret_code && odtu_mux_handle->var.ho_chnl_cfg[chnl_id].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                ret_code =  expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                     q_result->p_recs[i]->calendar_entry, 0);

                if (PMC_SUCCESS == ret_code && i == 0)
                {
                    ret_code = cfc_fifo_destroy(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                                chnl_id);
                }


            }
 
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = sts_sw_ho_calendar_cfg_deprov(odtu_mux_handle->sts_sw_handle, 
                                                         q_result->p_recs[i]->calendar_entry);
            }                                                              
        }                                                                                                                                                                                                                                
    }
    
    /* clean up dummy scheduling context */
    if (offline_update == FALSE && mux_stage == ODTU_MUX_STG_TWO
        && (odtu_mux_handle->var.ho_chnl_cfg[chnl_id].oduk_type == UTIL_GLOBAL_ODU3E1
            || odtu_mux_handle->var.ho_chnl_cfg[chnl_id].oduk_type == UTIL_GLOBAL_ODU3E2)
        && odtu_mux_handle->var.ho_chnl_cfg[chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED) 
    {
        if (odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id == chnl_id)
        {
            /* unset cal_en for dummy cycle */
            /* configure CAL_EN */
            odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry, 0);
            
            ret_code = odtu_mux_dummy_schd_clean(odtu_mux_handle, 
                                                 chnl_id, 
                                                 odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry,
                                                 odtu_mux_handle->var.ho_chnl_cfg[chnl_id].ts_type);
            
            
            if (PMC_SUCCESS == ret_code)
            {    
                odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry = 0x7F;
                odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id = 0x7F;
            }
        }
        else if (odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id == chnl_id)
        {
            /* unset cal_en for dummy cycle */
            /* configure CAL_EN */
            odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry, 0);
            
            ret_code = odtu_mux_dummy_schd_clean(odtu_mux_handle, 
                                                 chnl_id, 
                                                 odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry,
                                                 odtu_mux_handle->var.ho_chnl_cfg[chnl_id].ts_type);
            
            if (PMC_SUCCESS == ret_code)
            {
                odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry = 0x7F;
                odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id = 0x7F; 
            }                
        }
    }
    
    if (ODTU_MUX_STG_TWO == 2 && odtu_mux_handle->var.ho_chnl_cfg[chnl_id].oduk_type == UTIL_GLOBAL_ODU3)
    {
        /* check if dummy_schd is enabled for this MO channel */
        for (i = 0; i < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; i++)
        {
            if (odtu_mux_handle->var.dummy_schd_ctxt[i].dummy_ch_id == chnl_id
                && odtu_mux_handle->var.dummy_schd_ctxt[i].dummy_state == TRUE)
            {
                if (PMC_SUCCESS == ret_code)
                {
                    /* unset the context to release the dummy cycle */
                    odtu_mux_handle->var.dummy_schd_ctxt[i].dummy_ch_id = 0x7F;
                    odtu_mux_handle->var.dummy_schd_ctxt[i].dummy_state = FALSE;
                    odtu_mux_handle->var.dummy_schd_ctxt[i].dummy_cycle = 0;
                }
            }
        }
    }      
                                                             
    if (PMC_SUCCESS == ret_code)
    {
        chnl_state = ODTU_MUX_CHNL_START;
                        
        /* update HO channel information */
        odtu_mux_ho_chnl_var_update(odtu_mux_handle, chnl_id, chnl_state, 
                                    UTIL_GLOBAL_ODUK_DONT_CARE, 
                                    0, 
                                    NULL,
                                    NULL, 
                                    LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 0, 0);
    }                                    
    
    PMC_RETURN(PMC_SUCCESS);
} /* odtu_mux_ho_chnl_deprov */

/*******************************************************************************
*  odtu_mux_lo_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a new LO channel is added to an active HO channel 
*   in ODTU_MUX. This function will configure all associated tributary slots
*   for the LO channel.
*
*   Prior to calling this API, call odtu_mux_lo_chnl_deactivate() API to hold 
*   the channel under reset. After this API is called, call 
*   odtu_mux_lo_chnl_activate() API to activate the channel.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   stage_inst          - Enum for mux stage instance. See odtu_mux_stage_instance_t.
*   lo_odu_type         - LO ODU rate. See util_global_odukp_type_t.
*   odu_flex_rate       - If lo_odu_type is ODUFLEX_CBR or ODUFLEX_GFP, specify
*                         the client bit rate in bps. Otherwise, this argument 
*                         is ignored.
*   mapping_mode        - mapping mode of the LO ODU channel. 
*                         See util_global_mapping_mode_t.
*   trib_slot_mask      - 3 x 32 bit mask to specify occupied tributary slots 
*                         for the LO channel in the HO channel.
*   msi_data            - describes the mapping between tributary slots and
*                         tributary ports. 
*   lo_ts_type          - type of tributary slot. See odu_line_payload_t.
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   ho_chnl_id          - channel ID of the HO ODU that the LO ODU channel 
*                         belongs to.
*   offline_update      - indication for programming active/standby pages
*                         0 -- active page configuration
*                         1 -- offline page configuration
*   client_rate         - client rate in kb/sec (only used for CBR flex
*                         clients)
*   ghao_reprov         - TRUE : provisioning is part of g.hao operation
*                         FALSE : provisioning is not part of g.hao
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_lo_chnl_prov(odtu_mux_handle_t *odtu_mux_handle,
                                       UINT32 chnl_id,
                                       odtu_mux_stage_instance_t stage_inst,
                                       util_global_odukp_type_t lo_odu_type,
                                       UINT32 odu_flex_rate,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 trib_slot_mask[3],
                                       UINT32 msi_data,
                                       util_global_odu_line_payload_t lo_ts_type,
                                       util_global_odukp_type_t ho_odu_type,
                                       UINT32 ho_chnl_id,
                                       BOOL8 offline_update,
                                       UINT32 client_rate,
                                       BOOL ghao_reprov)                                       
{
    PMC_ERROR ret_code = PMC_SUCCESS;
    
    UINT32 i, cnt = 0;
    UINT32 num_ts = 0;
    UINT32 total_allowed_ts = 0;
    
    DOUBLE client_brate = 0;
    UINT32 ts_id, first_ts, last_ts;
    
    line_core_otn_db_key    key;
    line_core_otn_db_rec    new_rec;
    UINT32            num_of_recs = 0;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    
    odtu_mux_chnl_state_t chnl_state;
    util_global_odu_line_payload_t ts_type = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
     
    PMC_ENTRY();
     
    if (offline_update == 0)
    { 
        /* check the current channel state */
        if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_START)
        {
            PMC_RETURN(ODTU_MUX_ERR_CHNL_STATE_NOT_START); /* that channel is not in start state*/
        }
    
        /* configure MEM_HO_ID first before configuration */
        ret_code = odtu_mux_ho_id_mem_cfg(odtu_mux_handle, ho_chnl_id);
    }
    
    /* wait 2 calendar cycles after setting MEM_HO_ID */
    PMC_Q_USLEEP(odtu_mux_handle, 1);

    ts_type = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type;

    if (PMC_SUCCESS == ret_code)
    {
        if (lo_odu_type == UTIL_GLOBAL_ODUFLEX_GFP)  
        {
            client_brate = odu_flex_rate;
        }
        else if(lo_odu_type == UTIL_GLOBAL_ODUFLEX_CBR) {
            if (!digi_use_float32_get()) {
                client_brate = (DOUBLE)client_rate * 1000;
            } else {
                client_brate = (DOUBLE)((FLOAT)client_rate * 1000);
            }
        }
        else {
            client_brate = 0;
        } 
    }

    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odtu_mux_dbg_mask_somf_sync_set(odtu_mux_handle, ho_chnl_id, 0);
    }
   
    if (PMC_SUCCESS == ret_code)
    {
        num_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;
        PMC_LOG_TRACE("num_ts = %u\n", num_ts);
        PMC_LOG_TRACE("var.ho_chnl_cfg[%u].num_active_ts = %u\n", ho_chnl_id,
                   odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
    
        /* Get total number of allowed ts in the LO channel */
        if (lo_odu_type != UTIL_GLOBAL_ODUFLEX_CBR && lo_odu_type != UTIL_GLOBAL_ODUFLEX_GFP)
        {
            ret_code = util_global_max_client_num_ts_get(ho_odu_type, lo_odu_type, ts_type, 
                                                         &total_allowed_ts);
            PMC_LOG_TRACE("total_allowed_ts = %u\n", total_allowed_ts);
        }
        else {
            total_allowed_ts = odu_flex_rate;
        }
        
        if (PMC_SUCCESS == ret_code)
        {
            odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts = total_allowed_ts;
        
            if (stage_inst != 2)
            {
                /* query the record to update */
                key.calendar_entry = DO_NOT_USE_KEY;
                key.ho_channel = ho_chnl_id;
                key.mo_channel = DO_NOT_USE_KEY;
                key.lo_channel = DO_NOT_USE_KEY;
                key.trib_slot = DO_NOT_USE_KEY;
            }
            else {
                /* query the record to update */
                key.calendar_entry = DO_NOT_USE_KEY;
                key.ho_channel = DO_NOT_USE_KEY;
                key.mo_channel = ho_chnl_id;
                key.lo_channel = DO_NOT_USE_KEY;
                key.trib_slot = DO_NOT_USE_KEY;
            }
            
            /* find the total number of records that matches the HO channel ID */
            num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                       odtu_mux_handle->cfg.db_id,
                                       odtu_mux_handle->cfg.db_handle, (void *)&key,
                                       (void **)&q_result);
            if (num_of_recs == 0)
            {
                PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
            }                                   
            temp_result = *q_result;
        }
        
        if (TRUE == pmc_coreotn_mux_empty_ho_keep_in_sync_revision(&odtu_mux_handle->base) &&
            ghao_reprov == FALSE)
        {
            if ((offline_update == FALSE && odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts == 0)
                || (offline_update == TRUE 
                    && odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts == odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts))
            {
                for (i = 0; i < num_of_recs; i++)
                {
                    ret_code = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                                        temp_result.p_recs[i]->calendar_entry, 
                                                                        0);
                }
                
                if (PMC_SUCCESS == ret_code)
                {
                    /* first channel being added to the offline page */
                    if (offline_update == TRUE 
                        && odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts == odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts) 
                    {
                        odtu_mux_handle->var.offline_last_lo_chnl[ho_chnl_id] = FALSE;
                    }
                }  
            }  
        }                                                                 
            
        
        if (PMC_SUCCESS == ret_code && num_of_recs > 0 && offline_update == 0)
        {
            /* Update the database for the LO channel ID */
            /* ts could range from 0 - max num ts in HO */
            for (ts_id = 0; ts_id < num_ts; ts_id++)
            {
                if ((trib_slot_mask[ts_id/32] >> (ts_id%32)) & 1)
                {   
                    PMC_LOG_TRACE("ts_id = %u, chnl_id = %u\n", ts_id, chnl_id);  
                    PMC_LOG_TRACE("1.temp_result.p_recs[ts_id]->mo_channel = %u\n", temp_result.p_recs[ts_id]->mo_channel);  
                    PMC_LOG_TRACE("stage_inst = %u\n", stage_inst);          
                    if (stage_inst == ODTU_MUX_STG_ONE)
                    {
                        if ( ODTU_MUX_DB_ENTRY_UNKNOWN == (temp_result.p_recs[ts_id]->mo_channel) )
                        {
                            PMC_LOG_TRACE("2.temp_result.p_recs[ts_id]->mo_channel = %u\n", temp_result.p_recs[ts_id]->mo_channel);  
                            /* calendar entry is not assigned to any tributary slot/LO channel */
                            /* update MO channel ID to 'calendar_entry' */
                            new_rec = *temp_result.p_recs[ts_id];
                            new_rec.calendar_entry = temp_result.p_recs[ts_id]->calendar_entry;
                            new_rec.mo_channel = chnl_id;
                            new_rec.trib_slot = ts_id;
                            new_rec.cal_entry_status = 1;

                            if (odtu_mux_handle->var.prov_mode == 1 && lo_ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                            {
                                new_rec.ts_port_mo_lo = 0xFFFFFFFE; /* mark this is channelized MO */
                            }
                            else {
                                new_rec.ts_port_mo_lo = DO_NOT_USE_KEY;
                            }
                            
                            PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                        new_rec.ho_channel,
                                        new_rec.trib_slot,
                                        new_rec.mo_channel,
                                        new_rec.lo_channel);
                     
                            /* send back the result to the DB */ 
                            gen_db_update_record(&(odtu_mux_handle->base),
                                                 odtu_mux_handle->cfg.db_id,
                                                 odtu_mux_handle->cfg.db_handle,
                                                 &new_rec);
                            cnt++;                                        
                        }
                    }   
                    else if (stage_inst == ODTU_MUX_STG_TWO)
                    {
                        if ( ODTU_MUX_DB_ENTRY_UNKNOWN == (temp_result.p_recs[ts_id]->lo_channel) )
                        {
                            /* calendar entry is not assigned to any tributary slot/LO channel */
                            /* update LO channel ID to 'chnl_id' */
                            new_rec = *temp_result.p_recs[ts_id];
                            new_rec.lo_channel = chnl_id;
                            new_rec.ts_port_mo_lo = ts_id;
                            new_rec.cal_entry_status = 1;
                            
                            PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                        new_rec.ho_channel,
                                        new_rec.trib_slot,
                                        new_rec.mo_channel,
                                        new_rec.lo_channel);
                     
                            /* send back the result to the DB */
                            gen_db_update_record(&(odtu_mux_handle->base),
                                                 odtu_mux_handle->cfg.db_id,
                                                 odtu_mux_handle->cfg.db_handle,
                                                 &new_rec);
                            cnt++;
                        }
                    }
                    
                    if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                    {
                        /* can't write to any calendar entry if it is TS2G5. Calendar slot
                        ** must match the tributary slot location
                        */
                        /* if TS = 1 and 2G5, then start from the first calendar entry and repeat for the second half */
                        /* if TS = 2 and 2G5, then start from the second calendar entry */
                        if (stage_inst == ODTU_MUX_STG_ONE)
                        {
                            if ( ODTU_MUX_DB_ENTRY_UNKNOWN == (temp_result.p_recs[ts_id + (num_of_recs/2)]->mo_channel) )
                            {
                                new_rec = *temp_result.p_recs[ts_id + (num_of_recs/2)];
                                new_rec.mo_channel = chnl_id;
                                new_rec.trib_slot = ts_id;
                                new_rec.cal_entry_status = 1;

                                if (odtu_mux_handle->var.prov_mode == 1 && lo_ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                                {
                                    new_rec.ts_port_mo_lo = 0xFFFFFFFE; /* mark this is channelized MO */
                                }
                                else {
                                    new_rec.ts_port_mo_lo = DO_NOT_USE_KEY;
                                }
                                
                                PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                                new_rec.ho_channel,
                                                new_rec.trib_slot,
                                                new_rec.mo_channel,
                                                new_rec.lo_channel);
                                
                                /* send back the result to the Tx DB */
                                gen_db_update_record(&(odtu_mux_handle->base),
                                                     odtu_mux_handle->cfg.db_id,
                                                     odtu_mux_handle->cfg.db_handle,
                                                     &new_rec);
                            }
                        }
                        else if (stage_inst == ODTU_MUX_STG_TWO)
                        {
                            if ( ODTU_MUX_DB_ENTRY_UNKNOWN == (temp_result.p_recs[ts_id + (num_of_recs/2)]->lo_channel) )
                            {
                                /* calendar entry is not assigned to any tributary slot/LO channel */
                                /* update LO channel ID to 'chnl_id' */
                                new_rec = *temp_result.p_recs[ts_id + (num_of_recs/2)];
                                new_rec.lo_channel = chnl_id;
                                new_rec.ts_port_mo_lo = ts_id;
                                new_rec.cal_entry_status = 1;
                                
                                PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                              new_rec.ho_channel,
                                              new_rec.trib_slot,
                                              new_rec.mo_channel,
                                              new_rec.lo_channel);
                                
                                /* send back the result to the DB */
                                gen_db_update_record(&(odtu_mux_handle->base),
                                                     odtu_mux_handle->cfg.db_id,
                                                     odtu_mux_handle->cfg.db_handle,
                                                     &new_rec);
                            }
                        }                                                     
                    }  
                    
                    /* if configured all tribslot for the LO channel, end for loop */
                    if (lo_odu_type != UTIL_GLOBAL_ODUFLEX_CBR) 
                    { 
                        if (cnt == total_allowed_ts)
                        {
                            break;
                        }
                        odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts = total_allowed_ts;
                    }  
                    else {
                        odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts = cnt;
                    }    
                }                                       
            }                
        }
    }
     
    if (PMC_SUCCESS == ret_code)
    {
        /* store num of calendar entries in LO channel for TGMP */
        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        { 
            odtu_mux_handle->tgmp_handle->var.tgmp_chnl_cfg[chnl_id].num_cal_entries = total_allowed_ts * 2;
        }
        else {
            odtu_mux_handle->tgmp_handle->var.tgmp_chnl_cfg[chnl_id].num_cal_entries = total_allowed_ts;
        } 
    } 
    
    if (PMC_SUCCESS == ret_code)
    {                                             
         ret_code = odtu_mux_tgmp_lo_chnl_prov(odtu_mux_handle->tgmp_handle, chnl_id, 
                                               lo_odu_type, ho_odu_type, client_brate, 
                                               mapping_mode, ts_type, total_allowed_ts, 
                                               offline_update);                                            
    }                                            
                                         
    /* if GMP mapped, configure first_cycle in EXPAND */
    if (PMC_SUCCESS == ret_code && mapping_mode == UTIL_GLOBAL_GMP)
    {
        ret_code = expand_ibuf_first_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                               chnl_id, odtu_mux_handle->var.exp_first_cycle[chnl_id]);
    }

    if (PMC_SUCCESS == ret_code)
    {
        PMC_LOG_TRACE("before odtu_mux_lo_chnl_var_update\n");
        /* every time a new LO is added to the same HO channel, update to keep track
        ** of occupied tributary slots within the HO 
        */
        if (offline_update == 0)
        {
            chnl_state = ODTU_MUX_CHNL_EQUIPPED;
        
            odtu_mux_lo_chnl_var_update(odtu_mux_handle, chnl_id, ho_chnl_id, chnl_state, 
                                        lo_odu_type, ho_odu_type, total_allowed_ts, 
                                        ts_type, mapping_mode);                            
        }
        
        if (stage_inst == ODTU_MUX_STG_ONE)
        {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl_id;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
        }
        else if (stage_inst == ODTU_MUX_STG_TWO)
        {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = chnl_id;
            key.trib_slot = DO_NOT_USE_KEY;
        }
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                   odtu_mux_handle->cfg.db_id,
                                   odtu_mux_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result);
        
        if (num_of_recs == 0)
        {
            PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
        }
        
        PMC_LOG_TRACE("num_of_recs = %u\n", num_of_recs);
        
        if(num_of_recs > 0)
        {
            for (ts_id = 0; ts_id < num_of_recs; ts_id++) 
            {
               PMC_LOG_TRACE("q_result->p_recs[ts_id]->ho_channel = %u, q_result->p_recs[ts_id]->trib_slot = %u, q_result->p_recs[ts_id]->calendar_entry = %u, q_result->p_recs[ts_id]->mo_channel = %u, q_result.p_recs[ts_id]->lo_channel = %u\n",
                             q_result->p_recs[ts_id]->ho_channel,
                             q_result->p_recs[ts_id]->trib_slot,
                             q_result->p_recs[ts_id]->calendar_entry,
                             q_result->p_recs[ts_id]->mo_channel,
                             q_result->p_recs[ts_id]->lo_channel);
            }
        }

        if (offline_update == 0)
        {
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = cfc_fifo_build(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                          chnl_id, num_of_recs);        
            }

            if (PMC_SUCCESS == ret_code)
            {
                if (UTIL_GLOBAL_ODUFLEX_GFP == lo_odu_type) {
                  ret_code = cfc_fifo_build(odtu_mux_handle->tgmp_handle->txjc_proc_handle->cfc_handle, 
                                            chnl_id, 1);
                } else {
                  ret_code = cfc_fifo_build(odtu_mux_handle->tgmp_handle->txjc_proc_handle->cfc_handle, 
                                            chnl_id, num_of_recs);
                }
            }



        }
        
        if (PMC_SUCCESS == ret_code)
        {
            for (i = 0; i < num_of_recs; i++)
            {
                first_ts = (i == 0)? 1:0;
                last_ts = (i == total_allowed_ts-1)? 1:0;
                        
                PMC_LOG_TRACE("first_ts = %u, last_ts = %u\n", first_ts, last_ts);
                
                PMC_LOG_TRACE("num LO TS = %u\n", odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts);


                if (PMC_SUCCESS == ret_code)
                {
                    ret_code = odtu_mux_tribslot_cfg(odtu_mux_handle, stage_inst, chnl_id, 
                                                     q_result->p_recs[i]->calendar_entry,
                                                     msi_data, ho_chnl_id,
                                                     lo_odu_type, ho_odu_type, 
                                                     ts_type, mapping_mode,
                                                     first_ts, last_ts, i, offline_update);                                                     
                }                                                     
            }
        }  
    }  
          
    PMC_LOG_TRACE("var.ho_chnl_cfg[%u].num_active_ts = %u\n", ho_chnl_id,
                  odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
       
    if (offline_update == 0)
    {
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = expand_ibuf_chnl_activate(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, chnl_id);  
        }
    
    }

    PMC_RETURN(ret_code);
} /* odtu_mux_lo_chnl_prov */

/*******************************************************************************
*  odtu_mux_lo_chnl_offline_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a new LO channel is added to an active HO channel 
*   in ODTU_MUX. This function will configure all associated tributary slots
*   for the LO channel.
*
*   Prior to calling this API, call odtu_mux_lo_chnl_deactivate() API to hold 
*   the channel under reset. After this API is called, call 
*   odtu_mux_lo_chnl_activate() API to activate the channel.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   stage_inst          - Enum for mux stage instance. See odtu_mux_stage_instance_t.
*   lo_odu_type         - LO ODU rate. See util_global_odukp_type_t.
*   odu_flex_rate       - If lo_odu_type is ODUFLEX_CBR or ODUFLEX_GFP, specify
*                         the client bit rate in bps. Otherwise, this argument 
*                         is ignored.
*   mapping_mode        - mapping mode of the LO ODU channel. 
*                         See util_global_mapping_mode_t.
*   trib_slot_mask      - 3 x 32 bit mask to specify occupied tributary slots 
*                         for the LO channel in the HO channel.
*   msi_data            - describes the mapping between tributary slots and
*                         tributary ports. 
*   ts_type             - type of tributary slot. See odu_line_payload_t.LEX_GFP.
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   ho_chnl_id          - channel ID of the HO ODU that the LO ODU channel 
*                         belongs to.
*   client_rate         - client rate in kb/sec (only used for CBR flex)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_lo_chnl_offline_update(odtu_mux_handle_t *odtu_mux_handle,
                                                 UINT32 chnl_id,
                                                 odtu_mux_stage_instance_t stage_inst,
                                                 util_global_odukp_type_t lo_odu_type,
                                                 UINT32 odu_flex_rate,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 trib_slot_mask[3],
                                                 UINT32 msi_data,
                                                 util_global_odu_line_payload_t ts_type,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 UINT32 ho_chnl_id,
                                                 UINT32 client_rate)                                       
{
    PMC_ERROR ret_code = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    ret_code = odtu_mux_lo_chnl_prov(odtu_mux_handle, chnl_id, stage_inst, lo_odu_type,
                                     odu_flex_rate, mapping_mode, trib_slot_mask,
                                     msi_data, ts_type, ho_odu_type, ho_chnl_id, 1, client_rate, FALSE);
    
    PMC_RETURN(ret_code);
                                           
} /* odtu_mux_lo_chnl_offline_update */ 

/*******************************************************************************
*  odtu_mux_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a new LO channel is added to an active HO channel 
*   in ODTU_MUX. This function will configure all associated tributary slots
*   for the LO channel.
*
*   Prior to calling this API, call odtu_mux_lo_chnl_deactivate() API to hold 
*   the channel under reset. After this API is called, call 
*   odtu_mux_lo_chnl_activate() API to activate the channel.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   stage_inst          - enum for mux_stage. See odtu_mux_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - channel ID of the HO ODU that the LO ODU channel 
*                         belongs to.
*   offline_update      - indicates which page is being configured for 
*                         deprovisioning operation.
*                         0 -- normal configuration
*                         1 -- offline configuration
*   mo_odu3e2_odu3e1    - boolean to indicate if MO is ODU3e1 or ODU3e2
*                         TRUE: MO is ODU3e1 or ODU3e2
*                         FALSE: MO is not ODU3e1 or ODU3e2
*   extra_cycle         - calendar entry for the extra cycle used in
*                         MO ODU3e1 or ODU3e2
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deprov(odtu_mux_handle_t *odtu_mux_handle,
                                         odtu_mux_stage_instance_t stage_inst,
                                         UINT32 chnl_id, 
                                         UINT32 ho_chnl_id, BOOL8 offline_update,
                                         BOOL8 mo_odu3e2_odu3e1, 
                                         UINT8 extra_cycle)                                       
{
    PMC_ERROR ret_code = PMC_SUCCESS;
    
    UINT32 i;
    UINT32 num_ts;
    odtu_mux_chnl_state_t chnl_state;
    
    line_core_otn_db_key    key;
    UINT32            num_of_recs = 0;
    line_core_otn_db_query_res *q_result;
    
    UINT32 mo_odu3e1_3e2_entry = 0x7F;
    
    PMC_ENTRY();
   
    /* check the current channel state */
    if (offline_update == 0)
    { 
        if ((UINT32)odtu_mux_handle->var.lo_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_MUX_CHNL_EQUIPPED)
        {
            PMC_RETURN(ODTU_MUX_ERR_CHNL_STATE_NOT_EQUIPPED); /* the channel is not in deactivated state */
        }
    }
    
    /* hold cfc fifo channels */
    /* Deactivate EXP CFC.  Do not deactivate TXJC CFC (it's performed in firmware) */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = expand_ibuf_chnl_deactivate(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, chnl_id);  
    }
    
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odtu_mux_ho_id_mem_cfg(odtu_mux_handle, ho_chnl_id); 
    }
    
    /* wait for 2 multi-frame cycles */
    PMC_ATOMIC_YIELD(odtu_mux_handle->base.parent_handle->parent_handle,1);
    /*PMC_Q_USLEEP(odtu_mux_handle,1);*/
    
    /* deprov expand_ibuf LO channel */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = expand_ibuf_mux_lo_chnl_deprov(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                  chnl_id, offline_update);
    }
    odtu_mux_handle->var.exp_first_cycle[chnl_id] = 0;
    
    /* deprov txjc proc LO channel */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = txjc_proc_mux_lo_chnl_deprov(odtu_mux_handle->tgmp_handle->txjc_proc_handle, chnl_id);
    }                                                                                         
 
    if (PMC_SUCCESS == ret_code)
    {
        num_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;
  
        
        /* determine if this is LO or MO channel to be deprovisioned */
        if (stage_inst != ODTU_MUX_STG_TWO)
        {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl_id;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
        }
        else {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = chnl_id;
            key.trib_slot = DO_NOT_USE_KEY;
       }
            
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                   odtu_mux_handle->cfg.db_id,
                                   odtu_mux_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result);
    
        if (num_of_recs == 0)
        {
            PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
        }
    }
    
    if (mo_odu3e2_odu3e1)
    {
        mo_odu3e1_3e2_entry = extra_cycle;
    }    

    if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == FALSE)
    {
        if (offline_update == 0) {

            UINT32 num_current_pages;
            UINT32 *excess_page_array;
            excess_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle->cfg.max_fifo_num);
            PMC_ASSERT(excess_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);


            ret_code = cfc_fifo_current_num_pages_get(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                           chnl_id, &num_current_pages);

            if (ret_code != PMC_SUCCESS) {
                PMC_FREE(&(excess_page_array));                
                PMC_RETURN(ret_code);  /* this is to catch resources unavailable */
            }

            /* if FIFO doesn't exist return error */
            if(num_current_pages == 0)
            {
                PMC_FREE(&(excess_page_array));
                PMC_RETURN(CFC_ERR_FIFO_NOT_BUILT);
            }
            
            ret_code = cfc_fifo_excess_page_get(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle,
                                                chnl_id, 0, excess_page_array);
            if (ret_code != PMC_SUCCESS) {
                PMC_FREE(&(excess_page_array));
                PMC_RETURN(ret_code);  /* this is to catch resources unavailable */
            }

            for (i = 0; i < num_current_pages; i++)
            {
                if (PMC_SUCCESS == ret_code) {
                    ret_code = mp_mgen_ipt_cfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, 
                                               excess_page_array[i], 0x7F, 0, 0);
                }
                if (PMC_SUCCESS == ret_code) {
                    ret_code = mp_mgen_amp_uncfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, excess_page_array[i]);
                }
            }

            PMC_FREE(&(excess_page_array));
        }
    }

    if (PMC_SUCCESS == ret_code)
    {
        for (i = 0; i < num_of_recs; i++)
        {
            /* clear all IPT */
            ret_code = ohfs_insert_lo_calendar_cfg_deprov(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                          q_result->p_recs[i]->calendar_entry);
            PMC_ATOMIC_YIELD(odtu_mux_handle->base.parent_handle->parent_handle,0);                                              
            if (PMC_SUCCESS == ret_code)
            {
                /* set MSI to Unallocated instead of setting it to '0' */
                if (odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODU0
                    && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_CBR
                    && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    if (odtu_mux_handle->var.prov_mode == 1 && offline_update == 1 
                        && odtu_mux_handle->tgmp_handle->ohfs_insert_handle->cfg.msi_cfg_mode == UTIL_GLOBAL_OTN_MSI_CFG_MAN)
                    {
                        ohfs_insert_msi_mode_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                                 UTIL_GLOBAL_OTN_MSI_CFG_AUTO);
                                                 
                        /* MSI configuration */
                        ret_code = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                      q_result->p_recs[i]->calendar_entry, 
                                                      odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type, 
                                                      UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                      odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ts_type, 0); 

                        ohfs_insert_msi_mode_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                                 UTIL_GLOBAL_OTN_MSI_CFG_MAN);                                                                                                              
                    }   
                    else {   
                        /* MSI configuration */
                        ret_code = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                   q_result->p_recs[i]->calendar_entry, 
                                                   odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type, 
                                                   UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                   odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ts_type, 0); 
                    }
                }                                                   
            }                                                                                                         
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = expand_ibuf_lo_cycle_cfg_all(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                        q_result->p_recs[i]->calendar_entry,
                                                        0,
                                                        0);
            }
                                        
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = sts_sw_lo_chnl_cfg(odtu_mux_handle->sts_sw_handle, 
                                              ho_chnl_id, q_result->p_recs[i]->calendar_entry,
                                              chnl_id, 1, odtu_mux_handle->var.prov_mode);  
            }

            if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == TRUE)
            {
                if (PMC_SUCCESS == ret_code)
                {
                    ret_code = mp_mgen_ipt_cfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, 
                                               q_result->p_recs[i]->calendar_entry, 0x7F, 0, 0);
                } 
            }
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = mp_mgen_gmp_uncfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle,
                                             chnl_id);
            }  
            
            if (PMC_SUCCESS == ret_code)
            {
                if (offline_update == 0)
                {   
                    if (i == 0) {

                        if (PMC_SUCCESS == ret_code)
                        {
                            ret_code = cfc_fifo_destroy(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                                        chnl_id);
                        }

                        if (PMC_SUCCESS == ret_code)
                        {

                            ret_code = cfc_fifo_destroy(odtu_mux_handle->tgmp_handle->txjc_proc_handle->cfc_handle, 
                                                        chnl_id);
                        }
                    }
                  
                    if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == TRUE)
                    {
                        if (PMC_SUCCESS == ret_code)
                        {
                            ret_code = mp_mgen_amp_uncfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle,
                                                         q_result->p_recs[i]->calendar_entry);
                        } 
                    }
                }                                  
            } 
        } 
        
        if (PMC_SUCCESS == ret_code && mo_odu3e1_3e2_entry != 0x7F)
        {
            /* clear all IPT */
            ret_code = ohfs_insert_lo_calendar_cfg_deprov(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                          mo_odu3e1_3e2_entry);
                                                          
            if (PMC_SUCCESS == ret_code)
            {
                /* set MSI to Unallocated instead of setting it to '0' */
                if (odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODU0
                    && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_CBR
                    && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    ret_code = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                   mo_odu3e1_3e2_entry, 
                                                   odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type, 
                                                   UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                   odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ts_type, 0);     
                }                                                   
            }                                                                                                         
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = expand_ibuf_lo_cycle_cfg_all(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                        mo_odu3e1_3e2_entry,
                                                        0,
                                                        0);
            }
                                        
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = sts_sw_lo_chnl_cfg(odtu_mux_handle->sts_sw_handle, 
                                              ho_chnl_id, mo_odu3e1_3e2_entry,
                                              chnl_id, 1, odtu_mux_handle->var.prov_mode);  
            }
            
            if (PMC_SUCCESS == ret_code)
            {
                ret_code = mp_mgen_ipt_cfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, 
                                           mo_odu3e1_3e2_entry, 0x7F, 0, 0);
            }

            if (PMC_SUCCESS == ret_code)
            {
                if (offline_update == 0)
                {   
                
                    if (PMC_SUCCESS == ret_code)
                    {
                        ret_code = mp_mgen_amp_uncfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, 
                                                     mo_odu3e1_3e2_entry);                                        
                    }

                }                                  
            }  
        }  /*  if (PMC_SUCCESS == ret_code && mo_odu3e1_3e2_entry != 0x7F) */                                     
    }                                    

    if (PMC_SUCCESS == ret_code && offline_update == 0)
    {   
        /* if ODTU4.31 workaround is in place, then remove 31 from FMF1 MUX */
        if (odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type == UTIL_GLOBAL_ODU4
            && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].lo_oduk_type == UTIL_GLOBAL_ODU3
            && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED
            && num_of_recs == 32)
        {
            num_of_recs = 31;    
        }
        
        /* if ODTU4.31 workaround is in place, then remove 31 from FMF1 MUX */
        if (odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type == UTIL_GLOBAL_ODU4
            && (odtu_mux_handle->var.lo_chnl_cfg[chnl_id].lo_oduk_type == UTIL_GLOBAL_ODU3E2
                || odtu_mux_handle->var.lo_chnl_cfg[chnl_id].lo_oduk_type == UTIL_GLOBAL_ODU3E1)
            && odtu_mux_handle->var.lo_chnl_cfg[chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED
            && num_of_recs == 32)
        {
            num_of_recs = 33;    
        }
        
        if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts - (num_of_recs/2); 
            
            if (stage_inst == ODTU_MUX_STG_ONE)
            {
                for (i = 0; i < num_of_recs/2; i++)
                {
                    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[q_result->p_recs[i]->trib_slot] = 0;
                    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[q_result->p_recs[i]->trib_slot] = 0;
                }
            }
            else {
                for (i = 0; i < num_of_recs/2; i++)
                {
                    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[q_result->p_recs[i]->ts_port_mo_lo] = 0;
                    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[q_result->p_recs[i]->ts_port_mo_lo] = 0;
                }
            }
            
            
        }
        else {                         
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts - num_of_recs; 
            
            if (stage_inst == ODTU_MUX_STG_ONE)
            {
                for (i = 0; i < num_of_recs; i++)
                {
                    if (NULL != q_result->p_recs[i] &&
                        q_result->p_recs[i]->trib_slot != ODTU_MUX_DB_ENTRY_UNKNOWN)
                    {
                        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[q_result->p_recs[i]->trib_slot] = 0;
                        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[q_result->p_recs[i]->trib_slot] = 0;
                    }
                    else if (NULL != q_result->p_recs[i] &&
                             q_result->p_recs[i]->trib_slot == ODTU_MUX_DB_ENTRY_UNKNOWN)
                      {
                        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                                "Hit ODTU_MUX trib_slot i=%d is 0xFFFFFFF. Number Of Records = %d\n", i, num_of_recs);
                      }
                }
            }
            else {
                for (i = 0; i < num_of_recs; i++)
                {
                    if (NULL != q_result->p_recs[i] &&
                        q_result->p_recs[i]->ts_port_mo_lo != ODTU_MUX_DB_ENTRY_UNKNOWN)
                    {
                        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[q_result->p_recs[i]->ts_port_mo_lo] = 0;
                        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[q_result->p_recs[i]->ts_port_mo_lo] = 0;
                    }
                    else if (NULL != q_result->p_recs[i] &&
                             q_result->p_recs[i]->ts_port_mo_lo == ODTU_MUX_DB_ENTRY_UNKNOWN)
                    {
                        PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                                "Hit ODTU_MUX ts_port_mo_lo i=%d is 0xFFFFFFF. Number Of Records = %d\n", i, num_of_recs);
                    }
                }
            }
        }
        
        PMC_LOG_TRACE("%s, %s, %d, num_active_ts in HO = %u\n", __FILE__, __FUNCTION__, __LINE__, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
    }

    if (TRUE == pmc_coreotn_mux_empty_ho_keep_in_sync_revision(&odtu_mux_handle->base))
    {
        if ((offline_update == FALSE && odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts == 0)
            || (offline_update == TRUE && odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts == 0
                && odtu_mux_handle->var.offline_last_lo_chnl[ho_chnl_id] == FALSE))
        {
            for (i = 0; i < odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot; i++)
            {

                ret_code = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                              odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i], 
                                                              1);                                                                                                                     
            }
            
            if (offline_update == TRUE && odtu_mux_handle->var.offline_last_lo_chnl[ho_chnl_id] == FALSE)
            {
                odtu_mux_handle->var.offline_last_lo_chnl[ho_chnl_id] = TRUE;
            }
        }  
    }     
    
    if (offline_update == 1)
    {
        chnl_state = ODTU_MUX_CHNL_START;
        odtu_mux_lo_chnl_var_update(odtu_mux_handle, chnl_id, ho_chnl_id, 
                                    chnl_state, 
                                    UTIL_GLOBAL_ODUK_DONT_CARE, 
                                    UTIL_GLOBAL_ODUK_DONT_CARE, 
                                    0, 
                                    (util_global_odu_line_payload_t)LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 
                                    UTIL_GLOBAL_NO_MAP);
    }                                      
    
   
    PMC_RETURN(ret_code);
} /* odtu_mux_lo_chnl_deprov */

/*******************************************************************************
*  odtu_mux_lo_chnl_offline_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when removing an active LO channel from an active HO  
*   channel in ODTU_MUX. 
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   stage_inst          - enum for mux_stage. See odtu_mux_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - channel ID of the HO ODU that the LO ODU channel 
*                         belongs to
*   mo_odu3e2_odu3e1    - boolean to indicate if MO is ODU3e1 or ODU3e2
*                         TRUE: MO is ODU3e1 or ODU3e2
*                         FALSE: MO is not ODU3e1 or ODU3e2
*   extra_cycle         - calendar entry for the extra cycle used in
*                         MO ODU3e1 or ODU3e2
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_lo_chnl_offline_deprov(odtu_mux_handle_t *odtu_mux_handle,
                                                 odtu_mux_stage_instance_t stage_inst,
                                                 UINT32 chnl_id, 
                                                 UINT32 ho_chnl_id,
                                                 BOOL8 mo_odu3e2_odu3e1, 
                                                 UINT8 extra_cycle)                                       
{
    PMC_ERROR ret_code = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_code = odtu_mux_lo_chnl_deprov(odtu_mux_handle, stage_inst, chnl_id, 
                                       ho_chnl_id, 1, mo_odu3e2_odu3e1, extra_cycle);
   
    PMC_RETURN(ret_code);
} /* odtu_mux_lo_chnl_offline_deprov */

/*******************************************************************************
* odtu_mux_tribslot_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function is used to configure a tributary slot occupied by a HO. 
*   MSI coding for each tributary slot is done in this API. 
*   
*   This function must be called prior to calling odtu_mux_lo_chnl_prov() or
*   odtu_mux_ho_chnl_prov() API to properly set up configurations for each 
*   tributary slot occupied by the HO/LO channel. 
*
*   As an example, when 8 ODU0 LO channels are added to an active ODU2 HO channel 
*   with 1.25G tributary slot, this function must be called 8 times to set up 
*   all 8 tributary slots.                       
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance 
*   stage_inst          - Enum for mux stage instance. See odtu_mux_stage_instance_t.
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95  
*   cycle               - calendar cycle associated with this tribslot
*                         Valid range: 0 - 95
*   ts_port             - tributary port number that the tributary slot will be
*                         transported.
*   ho_chnl_id          - HO ODU channel ID that the current ts belongs to.
*                         Valid range: 0 - 95 
*   client_type         - LO ODU channel rate. See util_global_odukp_type_t
*   server_type         - HO ODU channel rate. See util_global_odukp_type_t
*   ts_type             - tributary slot type. See odu_line_payload_t
*   mapping_mode        - mapping mode for the LO channel. See util_global_mapping_mode_t
*   first_ts            - indicates if this tributary slot is the first tributary
*                         slot for the LO channel \n
*                         1 -- the tributary slot is the first tributary slot \n
*                         0 -- the tributary slot is not the first tributary slot
*   last_ts             - indicates if this tributary slot is the last tributary
*                         slot for the LO channel. \n
*                         1 -- the tributary slot is the last tributary slot \n
*                         0 -- the tributary slot is not the last tributary slot
*   frm_num             - occurence number of the LO channel 
*   offline_update      - indication for programming active/standby pages
*                         0 -- active page configuration
*                         1 -- offline page configuration                                        
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
PUBLIC PMC_ERROR odtu_mux_tribslot_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                       odtu_mux_stage_instance_t stage_inst,
                                       UINT32 chnl_id, UINT32 cycle,
                                       UINT32 ts_port, UINT32 ho_chnl_id,
                                       UINT32 client_type,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 first_ts, UINT32 last_ts,
                                       UINT32 frm_num, BOOL8 offline_update)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 num_tribslot;
    UINT32 num_active_ts;
    UINT32 num_lo_ts;
    UINT32 last_data_cycle;
    UINT32 server_size;
    UINT32 valid = 1;
    UINT32 div;
    UINT32 i;
    UINT32 tribslot = 0xFF;
    UINT32 lo_eomf_data_cycle;
    BOOL8 found;
    UINT32 ipt_va = 0;
    
    /*UINT32 cal_entry;*/
    line_core_otn_db_key    key;
    UINT32            num_of_recs = 0;
    line_core_otn_db_query_res *q_result;
    UINT32 mgen_amp_entry;
    
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(odtu_mux_handle!=NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
    
    /* Retrieve the maximum number of tributary slot in the HO channel that
       the LO channel will be added */
    num_tribslot = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;
    num_active_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts; 
    num_lo_ts = odtu_mux_handle->var.lo_chnl_cfg[chnl_id].num_ts;
      
    PMC_LOG_TRACE("num_tribslot in HO = %u, num_active_ts = %u\n", num_tribslot, num_active_ts); 
      
    /* check if there is enough ts availble in the HO channel */
    if (offline_update == 0 && num_active_ts > num_tribslot)
    {
        PMC_LOG(PMC_LOG_SEV_HIGH, ODTU_MUX_LOG_CODE_HO_TS_FULL, 5, 6);
        PMC_RETURN(ODTU_MUX_LOG_CODE_HO_TS_FULL);
    }
    ret_val = odtu_mux_tgmp_tribslot_cfg(odtu_mux_handle->tgmp_handle, chnl_id, 
                                         cycle, client_type, server_type, ts_type, 
                                         mapping_mode, 1, last_ts, frm_num, offline_update);
                                                                                   
    if (PMC_SUCCESS == ret_val)
    {
        if (stage_inst == ODTU_MUX_STG_ONE)
        {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl_id;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
        }
        else if (stage_inst == ODTU_MUX_STG_TWO)
        {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = chnl_id;
            key.trib_slot = DO_NOT_USE_KEY;
        }
        
        PMC_LOG_TRACE("db_id = %u\n", odtu_mux_handle->cfg.db_id);
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                   odtu_mux_handle->cfg.db_id,
                                   odtu_mux_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result);

        if (num_of_recs == 0)
        {
            PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
        }  
    
        found = FALSE;
        if (stage_inst == ODTU_MUX_STG_ONE)
        {
            for (i = 0; i < num_of_recs; i++)
            {
                if (q_result->p_recs[i]->calendar_entry == cycle)
                {
                    tribslot = q_result->p_recs[i]->trib_slot;
                    found = TRUE;
                    break;
                }
            }
        }
        else if (stage_inst == ODTU_MUX_STG_TWO)
        {
            for (i = 0; i < num_of_recs; i++)
            {
                if (q_result->p_recs[i]->calendar_entry == cycle)
                {
                    tribslot = q_result->p_recs[i]->ts_port_mo_lo;
                    found = TRUE;
                    break;
                }  
            }
        }

        /* Ensure that the record was found */
        PMC_ASSERT(TRUE == found, ODTU_MUX_LOG_ERR_INVALID_ARG, 0, 0);
    }

    if (PMC_SUCCESS == ret_val)
    {         
        if (odtu_mux_handle->var.prov_mode == 1 && offline_update == 1 
            && odtu_mux_handle->tgmp_handle->ohfs_insert_handle->cfg.msi_cfg_mode == UTIL_GLOBAL_OTN_MSI_CFG_MAN)
        {
            ohfs_insert_msi_mode_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                     UTIL_GLOBAL_OTN_MSI_CFG_AUTO);
                                     
            /* MSI configuration */
            ret_val = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                          cycle, server_type, client_type, ts_type, ts_port); 

            ohfs_insert_msi_mode_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                     UTIL_GLOBAL_OTN_MSI_CFG_MAN);                                                                                                              
        }   
        else {   
            /* MSI configuration */
            ret_val = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                          cycle, server_type, client_type, ts_type, ts_port); 
        }                                                                                                                        
    }
    
    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {
        if (mapping_mode == UTIL_GLOBAL_GMP)
        {        
   
            /* LO CH IPT */
            if( cycle == q_result->p_recs[num_of_recs-1]->calendar_entry)
            {
                ret_val = ohfs_insert_lo_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                                 cycle, chnl_id, odtu_mux_handle->var.exp_first_cycle[chnl_id], valid);
            } else {
                ret_val = ohfs_insert_lo_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                                 cycle, 0x7f, 0, 0);
            }

        }                                                
        else {
            /* LO CH IPT for non GMP mapping */
            ret_val = ohfs_insert_lo_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                             cycle, chnl_id, frm_num, valid);
        }
    }                  
    
    if (PMC_SUCCESS == ret_val)
    {
        /* EXPAND_IBUF configuration */
        ret_val = expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                           cycle, 1);
    }                                               
    
    /* EOMF_DATA_CYCLE configuration */ 
    if (PMC_SUCCESS == ret_val)
    {
        /* configure EOMF_DATA_CYCLE */
        server_size = odtu_mux_handle->tgmp_handle->expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl_id].server_size; 
        
        if (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1)
        {
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                server_size = 15232;
            }
            else {
                server_size = 30464;
            }
        }
        else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
        {
            if (server_type == UTIL_GLOBAL_ODU4)
            {
                server_size = 15200 * num_lo_ts;
            }
            else {
                server_size = 15232 * num_lo_ts;
            }
        }
        else if (server_type == UTIL_GLOBAL_ODU4)
        {
            server_size = 15200 * num_lo_ts;
        }
        else if (mapping_mode == UTIL_GLOBAL_GMP)
        {
            server_size = server_size * num_lo_ts;
        }
        
        PMC_LOG_TRACE("num_lo_ts = %u\n", num_lo_ts);
        PMC_LOG_TRACE("server_size = %u\n", server_size);
        if ((server_size % 48) == 0)
        {
            div = (server_size / 48);
        }
        else {
            div = (server_size / 48) + 1;
        }
              
        last_data_cycle = div % num_of_recs;
        if (last_data_cycle == 0) 
        {
            last_data_cycle = num_of_recs - 1;
        }
        else {
            last_data_cycle--;
        }
        
        lo_eomf_data_cycle = (last_data_cycle == frm_num)? 1:0;
 
        PMC_LOG_TRACE("lo_eomf_data_cycle = %u", lo_eomf_data_cycle);


        ret_val = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, 
                                                     q_result->p_recs[frm_num]->calendar_entry, 
                                                     lo_eomf_data_cycle);

    } 

    
    if (PMC_SUCCESS == ret_val)
    {
        if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == TRUE)
        {
            mgen_amp_entry = cycle;
        }
        else
        {

            if (offline_update == 0) 
            {
                UINT32 num_current_pages;
                UINT32 *excess_page_array;
                excess_page_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle->cfg.max_fifo_num);
                PMC_ASSERT(excess_page_array != NULL, CFC_ERR_CODE_ASSERT, 0, 0);

                ret_val = cfc_fifo_current_num_pages_get(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                                         chnl_id, &num_current_pages);

                if (ret_val != PMC_SUCCESS) {
                    PMC_FREE(&(excess_page_array));
                    PMC_RETURN(ret_val);  /* this is to catch resources unavailable */
                }

                /* if FIFO doesn't exist return error */
                if(num_current_pages == 0)
                {
                    PMC_FREE(&(excess_page_array));
                    PMC_RETURN(CFC_ERR_FIFO_NOT_BUILT);
                }
                
                ret_val = cfc_fifo_excess_page_get(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle,
                                                    chnl_id, 0, excess_page_array);
                if (ret_val != PMC_SUCCESS) {
                    PMC_FREE(&(excess_page_array));
                    PMC_RETURN(ret_val);  /* this is to catch resources unavailable */
                }

                mgen_amp_entry = excess_page_array[frm_num];

                PMC_FREE(&(excess_page_array));
            }
        }
    }

    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {
        /* HO_CAL_CYCLE_TOT */
        ret_val = expand_ibuf_ho_cal_cycle_tot_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                   cycle, chnl_id, server_type, ts_type, 0);
        
        if (PMC_SUCCESS == ret_val)
        {                                        
            if (mapping_mode == UTIL_GLOBAL_AMP)
            {   
                if (server_type == UTIL_GLOBAL_ODU3E1 && client_type == UTIL_GLOBAL_ODU2E)
                {
                    ret_val = mp_mgen_ipt_va_get(odtu_mux_handle->tgmp_handle->mp_mgen_handle, 
                                                 mgen_amp_entry, &ipt_va);
                                                
                    if (PMC_SUCCESS == ret_val)
                    {
                        PMC_ASSERT(q_result->p_recs != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
                        if (stage_inst == ODTU_MUX_STG_ONE)
                        {
                            tribslot = q_result->p_recs[(ipt_va/2)]->trib_slot; 
                        }
                        else if (stage_inst == ODTU_MUX_STG_TWO)
                        {  
                            tribslot = q_result->p_recs[(ipt_va/2)]->ts_port_mo_lo;
                        }                           
                    }                                                     
                }
                
                if (PMC_SUCCESS == ret_val)
                {
                    ret_val = mp_mgen_amp_cfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, 
                                              mgen_amp_entry, tribslot, server_type, 
                                              (util_global_odukp_type_t)client_type, ts_type); 
                }                                                                                                                   
            } 
            else {
                ret_val = mp_mgen_amp_uncfg(odtu_mux_handle->tgmp_handle->mp_mgen_handle, mgen_amp_entry);
            }     
        }                                                           
    }                                                             
    
    if (PMC_SUCCESS == ret_val)
    {
        /* STS_SW configuration */
        ret_val = sts_sw_lo_chnl_cfg(odtu_mux_handle->sts_sw_handle, ho_chnl_id,
                                     cycle, chnl_id, 0, FALSE);  
    }
    
    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {
        /* update ts var */
        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[tribslot]++;
        
        if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[tribslot] != 1)
        {
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[tribslot] = 1;                      
        }
                     
        PMC_LOG_TRACE("%s, %s, %d, odtu_mux_handle->var.ho_chnl_cfg[%u].num_active_ts = %u\n", __FILE__, __FUNCTION__, __LINE__, ho_chnl_id, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
        if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[tribslot] == 1)
        {
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   
                if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[tribslot] == 1)
                {
                    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = num_active_ts+1;
                }
            }
            else {
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = num_active_ts+1;
            }                
        }

        PMC_LOG_TRACE("%s, %s, %d, odtu_mux_handle->var.ho_chnl_cfg[%u].num_active_ts = %u\n", __FILE__, __FUNCTION__, __LINE__, ho_chnl_id, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
    }
    
                                                           
    PMC_RETURN(ret_val);    
} /* odtu_mux_tribslot_cfg */ 

/*******************************************************************************
* odtu_mux_ho_chnl_switch_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_mux_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl_id             - HO ODU channel ID to be configured
*                         Valid range: 0 - 95
*   cfg_all             - boolean to indicate if both DMX and MUX STS_SW block
*                         will be configured with one function call 
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
PUBLIC PMC_ERROR odtu_mux_ho_chnl_switch_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id,
                                             BOOL8 cfg_all)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);
    if (chnl_id >= 96)
    {
        PMC_RETURN(ODTU_MUX_LOG_ERR_INVALID_ARG);
    }

    /* check if the calendar array has been populated */
    PMC_ASSERT(odtu_mux_handle->var.ho_chnl_cfg[chnl_id].cal_array != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
        
    PMC_LOG_TRACE("STS_SW configuration in odtu_mux_ho_chnl_prov\n");

    result = sts_sw_cfg(odtu_mux_handle->sts_sw_handle, chnl_id, cfg_all);
   
    PMC_RETURN(result);
} /* odtu_mux_ho_chnl_switch_cfg */

/*******************************************************************************
* odtu_mux_expand_ibuf_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates CFC FIFO in EXPAND_IBUF 
*                                                                               
*                                                                               
* INPUTS:
*   *odtu_mux_handle      - pointer to ODTU_MUX handle instance                                                                             
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
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
PUBLIC PMC_ERROR odtu_mux_expand_ibuf_chnl_deactivate(odtu_mux_handle_t *odtu_mux_handle, 
                                                      UINT32 chnl) 
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
     
    /* put the specified CFC FIFO into reset */
    ret_val = expand_ibuf_chnl_deactivate(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, 
                                          chnl);
   
    PMC_RETURN(ret_val);
   
} /* expand_ibuf_chnl_deactivate */
                              

/*******************************************************************************
* odtu_mux_lo_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides a method to retrieve a set of LO channels for a
*    specied HO channel
*
* INPUTS:
*   *odtu_mux_handle    - pointer to mapotn handle instance
*   ho_chnl_id          - HO ODU channel ID that the LO channel belongs to
*                         Valid range: 0 - 95
* OUTPUTS:
*   *ch_ptr             - storage for an array of channel provisionned under 
*                         specied HO channel (shall be at least sizeof(UINT3232)*80
*                         the HO channel.
*   *num_ch             - number of channel in the current HO 
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_mux_lo_chnl_get(odtu_mux_handle_t *odtu_mux_handle,
                                      UINT32 ho_chnl_id,
                                      UINT32 *ch_ptr,
                                      UINT32 *num_ch)
{    
    UINT32 lo_chnl_itr = 0;
    PMC_ENTRY();
    
    *num_ch = 0;
    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_MUX_NUM_CHANL; lo_chnl_itr ++)
    {
        if (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].state == UTIL_GLOBAL_CHNL_OPERATIONAL && 
            odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id == ho_chnl_id)
        {
            ch_ptr[*num_ch] = lo_chnl_itr;
            *num_ch += 1;
        }
    }

    PMC_RETURN();
} /* odtu_mux_lo_chnl_get */

/*******************************************************************************
* odtu_mux_activated_lo_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started HO/LO channels
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle      - pointer to ODTU_MUX handle instance
*                                                                             
* OUTPUTS:
*   *actv_ho_chnl         - all activated HO channels
*   *actv_lo_chnl         - all activated LO channels
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void odtu_mux_activated_chnl_get(odtu_mux_handle_t *odtu_mux_handle,
                                        BOOL8 *actv_ho_chnl,
                                        BOOL8 *actv_lo_chnl)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
    
    for (itr = 0; itr < ODTU_MUX_NUM_CHANL; itr++)
    {
        actv_ho_chnl[itr] = odtu_mux_field_HO_CH_RSTB_get(NULL, odtu_mux_handle, itr)==0?FALSE:TRUE;
        actv_lo_chnl[itr] = odtu_mux_field_LO_CH_RSTB_get(NULL, odtu_mux_handle, itr)==0?FALSE:TRUE;
    }

    PMC_RETURN();
}  /* odtu_mux_activated_chnl_get */


/*******************************************************************************
* odtu_mux_ohfs_insert_cfc_fifo_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function activates OHFS_INSERT CFC fifo.
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle      - pointer to ODTU_MUX handle instance
*   chnl_id               - HO channel ID associated with OHFS_INSERT FIFO
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
PUBLIC PMC_ERROR odtu_mux_ohfs_insert_cfc_fifo_activate(odtu_mux_handle_t *odtu_mux_handle,
                                                        UINT32 chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
    
    /* take CFC out of reset */
    result = ohfs_insert_chnl_activate(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);  

    PMC_RETURN(result);
    
} /* odtu_mux_ohfs_insert_cfc_fifo_activate */

/*******************************************************************************
* odtu_mux_ohfs_insert_cfc_fifo_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function deactivates OHFS_INSERT CFC fifo.
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle      - pointer to ODTU_MUX handle instance
*   chnl_id               - HO channel ID associated with OHFS_INSERT FIFO
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
PUBLIC PMC_ERROR odtu_mux_ohfs_insert_cfc_fifo_deactivate(odtu_mux_handle_t *odtu_mux_handle,
                                                          UINT32 chnl_id)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
    
    /* take CFC out of reset */
    result = ohfs_insert_chnl_deactivate(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);  

    PMC_RETURN(result);
    
} /* odtu_mux_ohfs_insert_cfc_fifo_deactivate */

/*******************************************************************************
*  odtu_mux_ho_chnl_reset_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures reset mode for an ODU channel in ODTU_MUX.
*
* INPUTS:
*   *odtu_mux_handle        - pointer to ODTU_MUX handle instance
*   chnl_id                 - channel ID to be configured
*                             Valid range: 0 - 95
*   chnl_state              - register access state of ODU channel \n
*                             0 -- The ODU channel is in reset mode. It clears all  
*                                  the context information for that channel \n
*                             1 -- The ODU channel is out of reset.
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
PUBLIC PMC_ERROR odtu_mux_ho_chnl_reset_set(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT32 chnl_id, UINT32 chnl_state)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);

    result = odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle, chnl_id, chnl_state);
    
    PMC_RETURN(result);
} /* odtu_mux_ho_chnl_reset_set */


/*******************************************************************************
* odtu_mux_ho_ch_id_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started LO channels
*                                                                               
* INPUTS:                                                                       
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   lo_chnl_id          - LO channel ID.
*                           Valid range: 0 - 95    
*                                                                             
* OUTPUTS:
*   *ho_chnl_id         - ho channel ID that lo_chnl_id belongs to
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void odtu_mux_ho_ch_id_get(odtu_mux_handle_t *odtu_mux_handle,
                                  UINT32 lo_chnl_id,
                                  UINT32 *ho_chnl_id)
{
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);
    
    *ho_chnl_id = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].ho_chnl_id;
    

    PMC_RETURN();
}  /* odtu_mux_ho_ch_id_get */

/*******************************************************************************
*  odtu_mux_prov_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the provision mode of adding LO channels. 
*
* INPUTS:
*   *odtu_mux_handle    - handle to the ODTU_MUX instance to be operated on
*   prov_mode           - boolean to indicate if we are operating in normal 
*                         prov vs. super prov
*                         0 -- normal prov
*                         1 -- super prov
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_mux_prov_mode_set(odtu_mux_handle_t *odtu_mux_handle,
                                   BOOL8 prov_mode)
{
    PMC_ENTRY();
    
    /* prov_mode == 0: normal prov
    ** prov_mode == 1: super prov
    */
    odtu_mux_handle->var.prov_mode = prov_mode;
      
    PMC_RETURN();

} /* odtu_mux_prov_mode_set */                                               

/*******************************************************************************
* FUNCTION: odtu_mux_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odtu_mux_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odtu_mux_int_chnl_enable(odtu_mux_handle_t     *odtu_mux_handle,
                                          UINT32                 chnl_id,
                                          odtu_mux_int_chnl_t   *int_table_ptr,
                                          odtu_mux_int_chnl_t   *int_en_table_ptr,
                                          BOOL8                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    /* TGMP */
    if (PMC_SUCCESS == result)
    {
        result = tgmp_int_chnl_enable(odtu_mux_handle->tgmp_handle,
                                      chnl_id,
                                      UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, tgmp),
                                      &int_en_table_ptr->tgmp,
                                      enable);
    }


    PMC_RETURN(result);
} /* odtu_mux_int_chnl_enable */

/*******************************************************************************
* FUNCTION: odtu_mux_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odtu_mux_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odtu_mux_int_chnl_clear(odtu_mux_handle_t    *odtu_mux_handle,
                                       UINT32                  chnl_id,
                                       odtu_mux_int_chnl_t    *int_table_ptr,
                                       odtu_mux_int_chnl_t    *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    

    /* TGMP */
    if (PMC_SUCCESS == result)
    {
        result = tgmp_int_chnl_clear(odtu_mux_handle->tgmp_handle, 
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, tgmp),
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, tgmp));
    }


    PMC_RETURN(result);
} /* odtu_mux_int_chnl_clear */

/*******************************************************************************
* FUNCTION: odtu_mux_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   odtu_mux_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odtu_mux_int_chnl_retrieve(odtu_mux_handle_t      *odtu_mux_handle,
                                            UINT32                  chnl_id,
                                            odtu_mux_int_chnl_t    *filt_table_ptr,
                                            odtu_mux_int_chnl_t    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odtu_mux_handle, odtu_mux, UTIL_GLOBAL_INT_OPTIMIZE);
    
    #ifdef ODTU_MUX_STATUS_CHNL_RANGE
    #undef ODTU_MUX_STATUS_CHNL_RANGE
    #endif
    #define ODTU_MUX_STATUS_CHNL_RANGE UTIL_GLOBAL_STATUS_LFIELD_RETRIEVE_SINGLE

    ODTU_MUX_STATUS_CHNL_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odtu_mux_handle, odtu_mux);     

    /* TGMP */
    if (PMC_SUCCESS == result)
    {
        result = tgmp_int_chnl_retrieve(odtu_mux_handle->tgmp_handle,
                                        chnl_id,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, tgmp),
                                        &int_table_ptr->tgmp);
    }

    PMC_RETURN(result);
} /* odtu_mux_int_chnl_retrieve */

/*******************************************************************************
* FUNCTION: odtu_mux_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odtu_mux_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odtu_mux_int_chnl_enabled_check(odtu_mux_handle_t    *odtu_mux_handle,
                                                 UINT32                chnl_id,
                                                 odtu_mux_int_chnl_t  *int_en_table_ptr,
                                                 BOOL8                *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* TGMP */
    if (PMC_SUCCESS == result)
    {
        result = tgmp_int_chnl_enabled_check(odtu_mux_handle->tgmp_handle, 
                                             chnl_id,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, tgmp),
                                             int_found_ptr);
    }    
    
    PMC_RETURN(result);
} /* odtu_mux_int_chnl_enabled_check */

/*******************************************************************************
*  odtu_mux_ho_id_mem_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the configuration memory for the HO channel ID.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   ho_chnl_id          - HO channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ho_id_mem_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                        UINT32 ho_chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);
    
    /* argument checking */
    if(ho_chnl_id > 95)
    {
        PMC_RETURN(ODTU_MUX_LOG_ERR_INVALID_ARG);
    }
        
    result = expand_ibuf_mem_ho_id_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_id);   
  
    PMC_RETURN(result);
} /* odtu_mux_ho_id_mem_cfg */

/*******************************************************************************
*  odtu_mux_config_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the configuration memory space to be accessed and
*   whether configuration will be based on internal configuration address or
*   forced.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   ho_chnl_id          - HO channel ID \n
*                         valid range: 0 - 95
*   config_mode         - configuration mode \n
*                         0 -- HO/LO reconfigration change \n
*                         1 -- HAO configuration
*   norm_cmd            - NORM command. Only if config_mode is set to '1'.\n
*                         0 -- NORM_CMD will clear. \n
*                         1 -- NORM_CMD will be set.
*   config_update       - configuration mode
*                         0 -- No change. \n
*                         1 -- HO channel is scheduled for configuration change.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_config_update(odtu_mux_handle_t *odtu_mux_handle,
                                        UINT32 ho_chnl_id,
                                        UINT32 config_mode, UINT32 norm_cmd,
                                        UINT32 config_update)
{
    expand_ibuf_buffer_t expand_ibuf_buf[1];

    PMC_ENTRY();
    expand_ibuf_buffer_init(expand_ibuf_buf, odtu_mux_handle->tgmp_handle->expand_ibuf_handle);
    expand_ibuf_field_CONFIG_MODE_set(expand_ibuf_buf, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_id, config_mode);
    expand_ibuf_field_NORM_CMD_set(expand_ibuf_buf, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_id, norm_cmd);
    expand_ibuf_field_CONFIG_UPDATE_set(expand_ibuf_buf, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_id, config_update);  
    expand_ibuf_buffer_flush(expand_ibuf_buf);
  
    PMC_RETURN(PMC_SUCCESS); 
} /* odtu_mux_config_update */

/*******************************************************************************
*  odtu_mux_config_update_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function polls the CONFIG_UPDATE register bit field, which indicates if
*   a page flip has been successful.
*
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   ho_chnl_id          - HO channel ID
*                         Valid range: 0 - 95
*   max_poll            - Maximum number of times to poll
*
* OUTPUTS:
*
* RETURNS:
*   ODTU_MUX_ERR_POLL_CONFIG_UPDATE_EXPIRY If the polling timed out
*   PMC_SUCCESS If the poll was successful
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_config_update_poll(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 ho_chnl_id, UINT32 max_poll)
{
    PMC_ERROR result = PMC_SUCCESS;

    /* This is intentionally commented out to reduce logging */
    /* PMC_ENTRY(); */
    result = expand_ibuf_field_CONFIG_UPDATE_poll(NULL, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_id, 0, PMC_POLL_EQUAL, max_poll, NULL, 1);

    if (PMC_SUCCESS != result)
    {
#ifndef PMC_SW_SIMULATION
    
        result = ODTU_MUX_ERR_POLL_CONFIG_UPDATE_EXPIRY;
#else   
        result = PMC_SUCCESS; 
#endif /* PMC_SW_SIMULATION */               
    }  
    
    return(result);
    /*PMC_RETURN(PMC_SUCCESS); */
   
} /* odtu_mux_config_update_poll */

/*******************************************************************************
*  odtu_mux_config_update_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to flip the dual pages in ODTU_MUX.
*   This function should only be used when flipping the page is absolutely
*   necessary. Calling this function may result in changing the datapath 
*   configuration!!!
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   ho_chnl_id          - HO channel ID \n
*                         valid range: 0 - 95
*   config_update       - configuration mode
*                         0 -- No change. \n
*                         1 -- HO channel is scheduled for configuration change.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_config_update_set(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT32 ho_chnl_id,
                                            UINT32 config_update)
{
    PMC_ENTRY();
        
    expand_ibuf_field_CONFIG_UPDATE_set(NULL, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_id, config_update);  

  
    PMC_RETURN(PMC_SUCCESS);
} /* odtu_mux_config_update_set */

/*******************************************************************************
*  odtu_mux_lo_chnl_var_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODTU_MUX var_t variables. 
*
* INPUTS:
*   odtu_mux_handle - handle to the ODTU_MUX instance to be operated on
*   lo_chnl_id      - LO ODU channel ID.
*                     Valid range: 0 - 95
*   ho_chnl_id      - HO ODU channel ID. Valid range: 0 - 95
*   chnl_state      - Enum for mux channel state. See odtu_mux_chnl_state_t.
*   lo_oduk_type    - Enum for LO ODU type. See util_global_odukp_type_t.
*   ho_oduk_type    - Enum for HO ODU type. See util_global_odukp_type_t.
*   num_ts          - number of tributary slots in the current LO channel.
*   ts_type         - Enum for the payload format. See util_global_odu_line_payload_t.
*   mapping_mode    - Enum for the mapping mode. See util_global_mapping_mode_t.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_mux_lo_chnl_var_update(odtu_mux_handle_t *odtu_mux_handle, 
                                        UINT32 lo_chnl_id, UINT32 ho_chnl_id,
                                        odtu_mux_chnl_state_t chnl_state,
                                        util_global_odukp_type_t lo_oduk_type,
                                        util_global_odukp_type_t ho_oduk_type,
                                        UINT32 num_ts, 
                                        util_global_odu_line_payload_t ts_type,
                                        util_global_mapping_mode_t mapping_mode)
{
    PMC_ENTRY();
    
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].state = (util_global_chnl_state_t)chnl_state;
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].ho_chnl_id = ho_chnl_id;
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].lo_oduk_type = lo_oduk_type;
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].ho_oduk_type = ho_oduk_type;
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].num_ts = num_ts;
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].ts_type = ts_type; 
    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_id].mapping_mode = mapping_mode;

    PMC_RETURN();
} /* odtu_mux_lo_chnl_var_update */

/*******************************************************************************
* odtu_mux_dbg_mask_somf_sync_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures DBG_MASK_SOMF_SYNC to control frame alignment in
*   the empty HO channel in ODTU_MUX .
*
* INPUTS:
*   *odtu_mux_handle     - pointer to OHFS_INSERT handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95
*   somf_sync            - value of somf_sync 
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
PUBLIC PMC_ERROR odtu_mux_dbg_mask_somf_sync_set(odtu_mux_handle_t *odtu_mux_handle, 
                                                 UINT32 chnl, UINT32 somf_sync)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if (TRUE == pmc_coreotn_mux_empty_ho_order_revision(&odtu_mux_handle->base))
    {
        result = ohfs_insert_dbg_mask_somf_sync_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                    chnl, somf_sync);
    }
    
    PMC_RETURN(result);
} /* odtu_mux_dbg_mask_somf_sync_set */
        
/*
** Private Functions
*/

/*******************************************************************************
*  odtu_mux_first_valid_dummy_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODU_RFRM var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   odtu_mux_handle     - handle to the ODU_RFRM instance to be operated on
*   first_cycle         - tributary slot number
*
* OUTPUTS:
*   *valid_null_cycle   - first valid null cycle 
*   *dummy_idx          - index for dummy schedule
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_mux_first_valid_dummy_find(odtu_mux_handle_t *odtu_mux_handle,
                                                  UINT32 first_cycle, 
                                                  UINT32 *valid_null_cycle,
                                                  UINT32 *dummy_idx)
{
    UINT32 i = 0;
    UINT32 j;
    UINT32 start_idx;
    UINT32 dummy_schd_array[1][UTIL_GLOBAL_MAX_NUM_DUMMY_SCHD] = UTIL_GLOBAL_DMX_DUMMY_SCHD_ARRAY;
    
    PMC_ENTRY();
    
    if (TRUE == pmc_coreotn_schd_extra_channel_revision(&odtu_mux_handle->base))
    {
        start_idx = 2;
    }
    else
    {
        start_idx = 0;
    }

    for (i = start_idx; i < UTIL_GLOBAL_MAX_NUM_DUMMY_SCHD; i++)
    {
        if (dummy_schd_array[0][i] > first_cycle)
        {
            *valid_null_cycle = dummy_schd_array[0][i];
            
            for (j = 0; j < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; j++)
            {
                if (odtu_mux_handle->var.dummy_schd_ctxt[j].dummy_cycle == 0
                    && odtu_mux_handle->var.dummy_schd_ctxt[j].dummy_state == FALSE)
                {
                    /* empty index */
                    *dummy_idx = j;
                }
                else if (odtu_mux_handle->var.dummy_schd_ctxt[j].dummy_cycle == *valid_null_cycle
                         && odtu_mux_handle->var.dummy_schd_ctxt[j].dummy_state == TRUE)
                {
                    *valid_null_cycle = 0;
                    break;
                }
            }
                    
            if (*valid_null_cycle != 0)
            {
                break;
            }
            
        }
    }
        
    PMC_RETURN(PMC_SUCCESS);
} /* odtu_mux_first_valid_dummy_find */

/*******************************************************************************
*  odtu_mux_chnl_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the operational mode of an HO channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - channel ID to be configured (HO or MO) \n
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   odtu_mux_chnl_mode_t - operational mode of the HO ODU channel 
*
*******************************************************************************/
PRIVATE odtu_mux_chnl_mode_t odtu_mux_chnl_mode_get(odtu_mux_handle_t *odtu_mux_handle,
                                                    UINT32 chnl_id)
{
    BOOL8 passthru, null_frame_mode;    
    odtu_mux_chnl_mode_t mode;
    
    PMC_ENTRY();

    PMC_ASSERT(odtu_mux_handle!=NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);
            
    passthru           = odtu_mux_field_PASSTHRU_get(NULL, odtu_mux_handle, chnl_id);;

    if (TRUE ==  passthru)
    {
        mode = ODTU_MUX_CHNL_UNCHANNELIZED;            
    } else 
    {
        null_frame_mode    = ohfs_insert_null_frame_mode_get(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, chnl_id);
        if (FALSE == null_frame_mode)
        {
            mode = ODTU_MUX_CHNL_CHANNELIZED;
        } else 
        {
            mode = ODTU_MUX_NULL_FRAME_MODE;
        }
    }
        
    PMC_RETURN(mode);
} /* odtu_mux_chnl_mode_get */

/*******************************************************************************
* odtu_mux_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover a odtu_mux_recover_var_t context register space.
*
*  This function is used by odtu_mux_handle_restart_init.
*  
*  This function return an error when something is incoherent in the register 
*  space. In such case, the only solution for crash restart is to reset the 
*  subsystem
* 
* INPUTS:
*   *odtu_mux_handle        - pointer to MAPOTN handle instance to be operated on
*   top_energy_state_reg  - energy state from top-level
*   
*
* OUTPUTS:
*   *odtu_mux_var_reg          - A partial odtu_mux_var_t context + additional
*                              information to facilitate audit phase
*
* RETURNS:
*  PMC_SUCCESS: 
*   recovery is a success 
*  ODTU_MUX_ERR_CONTEXT_REG_INCOHERENT:
*   Register conetxt is incoherent. top will have
*    to shutdown the subsystem
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_mux_ctxt_reg_recover(odtu_mux_handle_t *odtu_mux_handle,
                                            pmc_energy_state_t top_energy_state_reg,                                         
                                            odtu_mux_recover_var_t *odtu_mux_var_reg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32  max_num_ho_chnl;
    
    PMC_ENTRY();
  
    /* 
       create a register context 
    */

    /* start from a clean context */

    odtu_mux_var_default_init(odtu_mux_handle,&odtu_mux_var_reg->var); 


    if(odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO)
    {
        max_num_ho_chnl = ODTU_MUX_NUM_CHANL;
    }
    else
    {
        max_num_ho_chnl = ODTU_MUX_STG1_MAX_NUM_HO_CHNL;
    }

    /* 
       In the case where top_energy state is RESET, we just have an initialized context       
    */    
    if (PMC_SUCCESS == rc && PMC_ENERGY_STATE_RUNNING == top_energy_state_reg)
    {
        UINT8 map_mode;
        UINT32 ho_chnl_itr, lo_chnl_itr, ts_itr UNUSED;
        BOOL8 mux_ho_actv_chnl[ODTU_MUX_NUM_CHANL];
        BOOL8 mux_lo_actv_chnl[ODTU_MUX_NUM_CHANL];
        UINT32 cn_n, cn_d;
        UINT32 golden_cn, pjo_cnt, njo_cnt;
        UINT32 bit_index;

        odtu_mux_activated_chnl_get(odtu_mux_handle,
                                    &mux_ho_actv_chnl[0],
                                    &mux_lo_actv_chnl[0]);
        ohfs_insert_reg_ctxt_recover(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                     &odtu_mux_var_reg->ohfs_insert_recover_var);
        
        /* iterate over all HO channel */
        for (ho_chnl_itr = 0; ho_chnl_itr < max_num_ho_chnl && PMC_SUCCESS== rc;ho_chnl_itr++)
        {
            if (mux_ho_actv_chnl[ho_chnl_itr] == TRUE)
            {
                /* retrieve information from OHFS_INSERT */
                ohfs_insert_reg_ctxt_ho_chnl_cfg_get(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                                     &odtu_mux_var_reg->ohfs_insert_recover_var,
                                                     ho_chnl_itr,
                                                     &odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot,
                                                     odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].cal_array);
                /* recover ts_type */
                if (ODTU_MUX_CHNL_UNCHANNELIZED == odtu_mux_chnl_mode_get(odtu_mux_handle,ho_chnl_itr))
                {
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type = UTIL_GLOBAL_ODU_UNCHANNELIZED;
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_PASSTHRU;
                } else if (ODTU_MUX_NULL_FRAME_MODE == odtu_mux_chnl_mode_get(odtu_mux_handle,ho_chnl_itr))
                {
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type = UTIL_GLOBAL_ODU_TS_1G25;
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_OPERATIONAL;
                } else 
                {                    
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_OPERATIONAL;
                    /* UTIL_GLOBAL_ODU_TS_1G25 or UTIL_GLOBAL_ODU_TS_2G5 */
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type   = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
                    /* recover ODUKP type  */
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type = UTIL_GLOBAL_LAST_ODUK;
                                      
                    
                    /* UNUSED */
                    /* odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_cal_entries */

                    /*
                      The following fields will be resolved in the LO channels recovery 
                        ts_type (UTIL_GLOBAL_ODU_TS_1G25 or UTIL_GLOBAL_ODU_TS_2G5)
                        oduk_type
                        num_active_ts
                        tribslot_added[UTIL_GLOBAL_MAX_NUM_TS];
                        tribslot_occupied[UTIL_GLOBAL_MAX_NUM_TS];
                    */
                    
                }
            }   
        }         

        /* recover information for LO channles */
        for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_MUX_NUM_CHANL && PMC_SUCCESS== rc;lo_chnl_itr++)
        {
 
            
            if (mux_lo_actv_chnl[lo_chnl_itr] == TRUE)
            {
                if (odtu_mux_var_reg->var.ho_chnl_cfg[lo_chnl_itr].state == (util_global_chnl_state_t)ODTU_MUX_CHNL_PASSTHRU)
                {
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id = lo_chnl_itr;
                } else 
                {

                    ohfs_insert_reg_ctxt_lo_chnl_cfg_get(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                                         &odtu_mux_var_reg->ohfs_insert_recover_var,
                                                         lo_chnl_itr,
                                                         &odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts,
                                                         &map_mode,
                                                         &odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id,
                                                         odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].cal_array,
                                                         odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array);
                                   
                    if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id != 0x7F && 
                        mux_ho_actv_chnl[odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id ] == TRUE && 
                        odtu_mux_var_reg->var.ho_chnl_cfg[odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                    {                  
                        if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts > 0)
                        {
                            odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_OPERATIONAL;
                        
                        
                            /* just to simplify the code */
                            ho_chnl_itr = odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id;
                            odtu_mux_ho_id_mem_cfg(odtu_mux_handle, ho_chnl_itr); 
                        
                            /* resolve mapping mode */
                            if ( (map_mode >= 1) && (map_mode <= 3) )
                            {
                                odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode = UTIL_GLOBAL_AMP;
                            }
                            else if (map_mode == 4)
                            {
                                odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode = UTIL_GLOBAL_GMP;
                            } 
                            else 
                            {
                                rc = ODTU_MUX_ERR_CONTEXT_REG_INCOHERENT;
                            }
                        
                            /* get ts_type */
                            expand_ibuf_ts_type_get(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, 
                                                    lo_chnl_itr,
                                                    odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].cal_array[0],
                                                    &odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type);
                        
                        
                            txjc_proc_cfg_get(odtu_mux_handle->tgmp_handle->txjc_proc_handle, 
                                              lo_chnl_itr, &cn_n, &cn_d, &golden_cn, &pjo_cnt, &njo_cnt);
                        
                            /* lo/ho_odukp_type*/
                            expand_ibuf_param_cfg_get(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                      lo_chnl_itr,
                                                      odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot,
                                                      odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts,
                                                      odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode,
                                                      odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type,
                                                      cn_n, cn_d, golden_cn, pjo_cnt, njo_cnt,
                                                      &odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type,
                                                      &odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type);
                            /* ajust the number of trib slots...*/
                            if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                            {
                                odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts /= 2;
                            }
                            /* update HO variables */
                            odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type = 
                                odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type;
                            odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type = 
                                odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                        
                        
                            /* regenerate trib_slot_mask */                
                            for(ts_itr = 0;ts_itr < odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts;ts_itr++)
                            {
                                bit_index = odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr];
                                odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].tx_tribslot_mask[bit_index/32] |= 0x1 << (bit_index%32);
                            }
                        
                            for(ts_itr = 0;ts_itr < odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts; ts_itr++)
                            {
                                if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_added[odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] == 1 ||
                                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_occupied[odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] == 1)
                                {
                                    rc = ODTU_MUX_ERR_CONTEXT_REG_INCOHERENT;
                                } else 
                                {
                                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_added[odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_occupied[odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                                }
                                odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts += 1;
                            }
                        }
                    }
                }
            }
        }

        
    }
    PMC_RETURN(rc);
} /* odtu_mux_ctxt_reg_recover */

/*******************************************************************************
* odtu_mux_ctxt_audit
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
*   *odtu_mux_handle        - pointer to MAPOTN handle instance to be operated on
*   *restart_init_cfg_ptr       - pointer to restart init config
*   *odtu_mux_var_reg       - A partial odtu_mux_var_t context + additional
*                              information to facilite audit phase
*  *prev_stg_mux_var_reg   - The recovery context from the previous stg\n
*                            It may be NULL
*
* OUTPUTS:
*   *odtu_mux_var_clean     - A clean context which represents the intersection
*                             between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS:
*    Memory & register context matches
*   ODTU_MUX_ERR_CONTEXT_CLEANUP:
*    A cleanup is required.
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_mux_ctxt_audit(odtu_mux_handle_t *odtu_mux_handle,
                                      util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                      odtu_mux_recover_var_t *odtu_mux_var_reg,            
                                      odtu_mux_recover_var_t *prev_stg_mux_var_reg,                              
                                      odtu_mux_var_t *odtu_mux_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 ho_chnl_itr, lo_chnl_itr, ts_itr;
    line_core_otn_db_key key;
    UINT32 num_of_recs;
    UINT32 actual_num_of_recs;
    line_core_otn_db_query_res *q_result;
    BOOL8 chnl_in_sync;
    BOOL8 odu3_3Xslot_case = FALSE;
    BOOL8 has_prev_stage = FALSE;
    UINT32 max_num_ho_chnl;

    PMC_ENTRY();
    
    
    /* in the SYSOTN configuration, stage 2 is driven by stage 1
       in such case, we are not considering anything the previous stage */
    if (odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO && 
        prev_stg_mux_var_reg != NULL)
    {
        has_prev_stage = TRUE;
    }

    /* init the new context  */
    odtu_mux_var_default_init(odtu_mux_handle,odtu_mux_var_clean);


    /* 
       first, we resolve some ambiguities 
    */    

    if(odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO)
    {
        max_num_ho_chnl = ODTU_MUX_NUM_CHANL;
    }
    else
    {
        max_num_ho_chnl = ODTU_MUX_STG1_MAX_NUM_HO_CHNL;
    }

    for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_MUX_NUM_CHANL;ho_chnl_itr++)
    {
        if ( odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU4 && 
             odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED &&
             (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_PASSTHRU ||
              odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL) && 
             odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state  == (UINT32)ODTU_MUX_CHNL_START)
        {
            PMC_MEMCPY(&odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr], &odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
        }

        if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL &&
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
        {
            if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3 &&
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3E2)
            {
                odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type;
            }
        }
    }


    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_MUX_NUM_CHANL;lo_chnl_itr++)
    {
        /* in warm restart mode, additional checks are performed at upper-level */
        if (restart_init_cfg_ptr->is_warm_restart == TRUE)
        {
            if (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_EQUIPPED) 
            {
                PMC_MEMCPY(&odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr], &odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr], sizeof(util_global_odu_lo_chnl_cfg_t)); 
            }
        }

        if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL && 
            odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL  &&
            (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode))
        {
            if ( UTIL_GLOBAL_GMP == odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode )
            {
                /*  GMP:all client_type may be converted to UTIL_GLOBAL_ODUFLEX_CBR or UTIL_GLOBAL_ODUFLEX_GFP  fter coherency check */
                if ((UTIL_GLOBAL_ODUFLEX_GFP == odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR))
                {                 
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODUFLEX_CBR;
                }
                if ((UTIL_GLOBAL_ODUFLEX_CBR == odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) && 
                    (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP))
                {                 
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODUFLEX_GFP;
                }

                /*GMP: for client_type UTIL_GLOBAL_ODU2E -> we accept UTIL_GLOBAL_ODU2E_ETRANS */
                if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU2E &&
                    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU2E_ETRANS)
                {
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODU2E_ETRANS;
                }
                /* GMP:client_type UTIL_GLOBAL_ODU1E -> accept  UTIL_GLOBAL_ODU1E_ETRANS */
                if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU1E &&
                    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU1E_ETRANS)
                {
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODU1E_ETRANS;
                }
                if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3 &&
                    odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3E2)
                {
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /* GMP:server_type = UTIL_GLOBAL_ODU2 and client_type = UTIL_GLOBAL_ODU0 -> we accept also server_type = UTIL_GLOBAL_ODU2E (_ETRANS_ */
                if ((odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU0 ) &&
                    (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2 ) &&                    
                    (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2E ||  odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2E_ETRANS))
                {                 
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    odtu_mux_var_reg->var.ho_chnl_cfg[odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /* GMP: *server_type = UTIL_GLOBAL_ODU3 && *client_type = UTIL_GLOBAL_ODUFLEX_CBR, we also accept *server_type = UTIL_GLOBAL_ODU3E2 && *client_type = UTIL_GLOBAL_ODUFLEX_CBR */
                if ((odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP) &&
                    (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3 ) &&                    
                    (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3E2))
                {                 
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    odtu_mux_var_reg->var.ho_chnl_cfg[odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /* GMP: *server_type = UTIL_GLOBAL_ODU2 && *client_type = UTIL_GLOBAL_ODUFLEX_CBR, we also accept *server_type = UTIL_GLOBAL_ODU2E && *client_type = UTIL_GLOBAL_ODUFLEX_CBR  */
                if ((odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP ) &&
                    (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2 ) &&                    
                    (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2E))
                {                 
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    odtu_mux_var_reg->var.ho_chnl_cfg[odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /*  GMP:all client_type may be converted to UTIL_GLOBAL_ODUFLEX_CBR or UTIL_GLOBAL_ODUFLEX_GFP  fter coherency check */
                if ((odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type) &&                   
                    (odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP ||
                     odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP))
                {                 
                    odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type;
                }               
            }
        }
    }            
        
    /* Channel context audit */
    /* iterate over all HO channel */
    for (ho_chnl_itr = 0; ho_chnl_itr < max_num_ho_chnl;ho_chnl_itr++)
    {
        if (restart_init_cfg_ptr->is_warm_restart == TRUE && odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_PASSTHRU)
        {
            PMC_MEMCPY(&odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr], &odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
        } else if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state != odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state)
        {
            rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
        } else if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL ||
                   odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_PASSTHRU) 
        {
            if (!( (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].ts_type ||
                    odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD )&&
                  odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot == odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot))
            {
                rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
            } else 
            {
                chnl_in_sync = TRUE;
                if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED &&
                    odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO && 
                    has_prev_stage == TRUE)
                {
                    /* in this case, we check the status of the previous stage */
                    if (!(prev_stg_mux_var_reg->var.lo_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL ||
                          (prev_stg_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state == UTIL_GLOBAL_CHNL_PASSTHRU &&
                           prev_stg_mux_var_reg->var.lo_chnl_cfg[ho_chnl_itr].state == UTIL_GLOBAL_CHNL_START)))
                    {
                        rc = ODTU_MUX_ERR_CONTEXT_CLEANUP; 
                        chnl_in_sync = FALSE;
                    }
                } else 
                {
                    /* check if the DB is in sync with the register space*/                
                    key.calendar_entry = DO_NOT_USE_KEY;
                    if (odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO)
                    {
                        key.ho_channel = DO_NOT_USE_KEY;
                        key.mo_channel = ho_chnl_itr;
                    } else 
                    {
                        key.ho_channel = ho_chnl_itr;
                        key.mo_channel = DO_NOT_USE_KEY;
                    }
                    key.lo_channel = DO_NOT_USE_KEY;
                    key.trib_slot = DO_NOT_USE_KEY;
                    num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                               odtu_mux_handle->cfg.db_id,
                                               odtu_mux_handle->cfg.db_handle, (void *)&key,
                                               (void **)&q_result);
                    if (num_of_recs == 0 && num_of_recs != odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot) 
                    {
                        rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
                        chnl_in_sync = FALSE;
                    } else 
                    {                    
                        for (ts_itr = 0; ts_itr < num_of_recs && chnl_in_sync == TRUE; ts_itr++)
                        {
                            if (q_result->p_recs[ts_itr]->calendar_entry !=  odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].cal_array[ts_itr])
                            {
                                rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
                                chnl_in_sync = FALSE;
                            }                         
                        }                   
                    }
                }
                if (TRUE == chnl_in_sync)
                {                
                    if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_NULL ||  
                        odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
                    {
                        PMC_MEMCPY(&odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr], &odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
                    } else
                    {
                        /* we have a channelized HO container, continue the audit  */
                        /* the first condition handles the case where the container is empty */
                        /*
                         * Added comparison of the num_active_ts for the
                         * HO and removed the low order comparison.  The
                         * former represents all active lo order channels
                         * for this HO where as the LO order channel in
                         * the channel handle represents the possible 
                         * number, 8 for ODU2 for example, whereas the
                         * recovered value is the actual number.
                         */
                        if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type != UTIL_GLOBAL_LAST_ODUK &&
                            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type !=  odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type &&
                            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts != odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts)
                        {
                            rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
                        } else 
                        {
                            if (TRUE == chnl_in_sync)
                            {
                                /* in this case, we check the status of the previous stage */
                                if (has_prev_stage == TRUE && 
                                    prev_stg_mux_var_reg->var.lo_chnl_cfg[ho_chnl_itr].state != (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
                                {
                                    rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;                                   
                                } else 
                                {                                                                    
                                    /* at this point we can accept the HO channel                                                   
                                     */                    
                                    PMC_MEMCPY(&odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr], &odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
                                
                                    /* tribslot_added, tribslot_occupied, num_active_ts may not be in sync but it is only an indication that
                                       they are LO channel not in sync. That will be identified in the next LO channel inspection */
                                    PMC_MEMSET(odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_added,0,sizeof(odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_added));
                                    PMC_MEMSET(odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_occupied,0,sizeof(odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_occupied));
                                    odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].num_active_ts = 0;                                                
                                } 
                            }   
                        }
                    }
                }
            }
        }
    }
    
    /* iterate over LO channel */
    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_MUX_NUM_CHANL;lo_chnl_itr++)
    {             
        odu3_3Xslot_case = FALSE;
        if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state != odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].state)
        {
            rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
        } else if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state ==  (UINT32)ODTU_MUX_CHNL_OPERATIONAL)
        {
            if(!( odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ts_type &&
                  odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode &&
                  odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id &&
                  odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type &&
                  odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type))
            {
                rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
            } else 
            {
                ho_chnl_itr = odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id;
                /* first check if the associated HO channel is still alive ...*/
                if (odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].state ==  (UINT32)ODTU_MUX_CHNL_OPERATIONAL &&
                    odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].oduk_type ==  odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type)
                {
                    chnl_in_sync = TRUE;
                    /* check if the DB is in sync with the register space*/                
                    key.calendar_entry = DO_NOT_USE_KEY;
                    if (odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO)
                    {
                        key.ho_channel = DO_NOT_USE_KEY;
                        key.mo_channel = DO_NOT_USE_KEY;
                        key.lo_channel = lo_chnl_itr;
                    } else 
                    {
                        key.ho_channel = DO_NOT_USE_KEY;
                        key.mo_channel = lo_chnl_itr;
                        key.lo_channel = DO_NOT_USE_KEY;
                    }                      
                    key.trib_slot = DO_NOT_USE_KEY;                      
                    num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                               odtu_mux_handle->cfg.db_id,
                                               odtu_mux_handle->cfg.db_handle, (void *)&key,
                                               (void **)&q_result);
                    actual_num_of_recs = odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts;
                    if (odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                    {
                        actual_num_of_recs *= 2;
                    } else if (actual_num_of_recs == 31 && num_of_recs == 32 )
                    {
                        num_of_recs = 31;
                        odu3_3Xslot_case = TRUE;
                    } else if (actual_num_of_recs == 33 && num_of_recs == 32 )
                    {
                        num_of_recs = 33;
                        odu3_3Xslot_case = TRUE;
                    }

                    if (num_of_recs == 0 || num_of_recs != actual_num_of_recs)
                    {
                        rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
                        chnl_in_sync = FALSE;
                    } else 
                    {             
                        if (odu3_3Xslot_case == FALSE)
                        {
                            for (ts_itr = 0; ts_itr < num_of_recs && chnl_in_sync == TRUE; ts_itr++)
                            {
                                if (q_result->p_recs[ts_itr]->calendar_entry !=  odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].cal_array[ts_itr] )
                                {
                                    rc = ODTU_MUX_ERR_CONTEXT_CLEANUP;
                                    chnl_in_sync = FALSE;
                                }                         
                            }                   
                        }
                    }
                    if (TRUE == chnl_in_sync)
                    {
                        PMC_MEMCPY(&odtu_mux_var_clean->lo_chnl_cfg[lo_chnl_itr], &odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr], sizeof(util_global_odu_lo_chnl_cfg_t)); 
                        
                        /* RESYNC HO fields */
                        for(ts_itr = 0;ts_itr < odtu_mux_var_clean->lo_chnl_cfg[lo_chnl_itr].num_ts; ts_itr++)
                        {
                            odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_added[odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                            odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_occupied[odtu_mux_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                            odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr].num_active_ts += 1;
                        }                    
                    }
                }                                
            }
        }   
    }
    if (has_prev_stage  == TRUE)
    {
        UINT32 entry_itr = 0;
        UINT8 chnl_0 = 0xFF;
        for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_MUX_NUM_CHANL ;ho_chnl_itr++)
        {
            if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_MUX_CHNL_OPERATIONAL && 
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED && 
                (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3E1 ||
                 odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3E2))
            {
                for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_MUX_NUM_CHANL;lo_chnl_itr++)
                {
                    if (entry_itr < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD)
                    {
                        if (prev_stg_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type && 
                            chnl_0 != lo_chnl_itr)
                        {
                            odtu_mux_var_clean->dummy_schd_entry[entry_itr].dummy_mo_ch_id  = lo_chnl_itr;
                            odtu_mux_var_clean->dummy_schd_entry[entry_itr].dummy_cal_entry = prev_stg_mux_var_reg->lo_chnl_extra[lo_chnl_itr].cal_array[prev_stg_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts-1];
                            if (entry_itr == 0)
                            {
                                chnl_0 = lo_chnl_itr;
                            }
                            entry_itr += 1;
                        }
                    }
                }                 
            }
        }                
    }

    if(odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_ONE)
    {
        for (ho_chnl_itr = ODTU_MUX_STG1_MAX_NUM_HO_CHNL; ho_chnl_itr < ODTU_MUX_NUM_CHANL ;ho_chnl_itr++)
        {
            PMC_MEMCPY(&odtu_mux_var_clean->ho_chnl_cfg[ho_chnl_itr], &odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
        }
    }
    

    PMC_RETURN(rc);
} /* odtu_mux_ctxt_audit */

/*******************************************************************************
* odtu_mux_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function put in place the cleaned var_t context. The register and 
*   memory context of channels that are not in OPERATIONAL state are cleaned 
*
* INPUTS:
*   *odtu_mux_handle        - pointer to MAPOTN handle instance to be operated on
*   *odtu_mux_var_reg       - A partial odtu_mux_var_t context + additional
*                           information to facilite audit phase
*   *odtu_mux_var_clean     - A clean context which represents the intersection
*                           between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS: There is no error case in this function (not yet)
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_mux_ctxt_cleanup(odtu_mux_handle_t *odtu_mux_handle,
                                        odtu_mux_recover_var_t *odtu_mux_var_reg,
                                        odtu_mux_var_t *odtu_mux_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;

    UINT32 ho_chnl_to_clean[ODTU_MUX_NUM_CHANL],lo_chnl_to_clean[ODTU_MUX_NUM_CHANL];
    UINT32 num_ho_chnl_to_clean,num_lo_chnl_to_clean, ho_chnl_id, num_ho_unchannelized_cycle;        
    BOOL8 set_start_state = FALSE;
    UINT32 lo_cleaned_cycle[ODTU_MUX_NUM_CHANL], ho_cycle[ODTU_MUX_NUM_CHANL], ho_cleaned_cycle[ODTU_MUX_NUM_CHANL],mem_unchannelized_cycle[ODTU_MUX_NUM_CHANL];
    UINT32 num_lo_cleaned_cycle, num_ho_cycle,num_ho_cleaned_cycle;
    UINT32 config_update_itr; 
    BOOL8 config_update_done, found;
    UINT32 ho_chnl_itr,lo_chnl_itr, cycle_itr, ho_cycle_itr, ho_unchannelized_cycle_itr;
    UINT32 max_num_ho_chnl;

    PMC_ENTRY();

    /* keep list of unchannlized cycle */

    if(odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO)
    {
        max_num_ho_chnl = ODTU_MUX_NUM_CHANL;
    }
    else
    {
        max_num_ho_chnl = ODTU_MUX_STG1_MAX_NUM_HO_CHNL;
    }


    num_ho_unchannelized_cycle = 0;
    for (ho_chnl_itr = 0; ho_chnl_itr < max_num_ho_chnl;ho_chnl_itr++)
    {
        mem_unchannelized_cycle[ho_chnl_itr] = 0x7f;
        if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state == (util_global_chnl_state_t)ODTU_MUX_CHNL_PASSTHRU ||
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (util_global_chnl_state_t)ODTU_MUX_CHNL_PASSTHRU)
        {
            mem_unchannelized_cycle[num_ho_unchannelized_cycle] = ho_chnl_itr;
            num_ho_unchannelized_cycle += 1;
        }
    }

    /* the old mapotn var context is incoherent, we use the new one */
    PMC_MEMCPY(&odtu_mux_handle->var,odtu_mux_var_clean,sizeof(odtu_mux_var_t));
    
    /* 
       iterate over HO channel 
         - build the list of HO channels to clean 
         - PUT ALL HO Channel in NULL_FRAME mode
    */
    num_lo_chnl_to_clean = 0;
    ho_unchannelized_cycle_itr = 0;
    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_MUX_NUM_CHANL;lo_chnl_itr++)
    {

        if ((UINT32) ODTU_MUX_CHNL_OPERATIONAL != odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].state)
        {
            set_start_state = FALSE;
            ho_chnl_id = odtu_mux_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id;
            if (ho_chnl_id >= ODTU_MUX_NUM_CHANL)
            {
                set_start_state = TRUE;
            } else {
                if (odtu_mux_var_reg->var.ho_chnl_cfg[ho_chnl_id].state != (util_global_chnl_state_t)ODTU_MUX_CHNL_PASSTHRU)
                {
                   set_start_state = TRUE; 
                }
            }
            if (TRUE == set_start_state)
            {                
                odtu_mux_handle->var.lo_chnl_cfg[lo_chnl_itr].state = UTIL_GLOBAL_CHNL_START;
                lo_chnl_to_clean[num_lo_chnl_to_clean] = lo_chnl_itr;
                num_lo_chnl_to_clean += 1;

                if (mem_unchannelized_cycle[ho_unchannelized_cycle_itr] == lo_chnl_itr)
                {
                    /* we have this one */
                    ho_unchannelized_cycle_itr += 1;
                }
            } else 
            {
                if (mem_unchannelized_cycle[ho_unchannelized_cycle_itr] == lo_chnl_itr)
                {
                    if (odtu_mux_handle->var.ho_chnl_cfg[lo_chnl_itr].state == UTIL_GLOBAL_CHNL_START)
                    {
                        lo_chnl_to_clean[num_lo_chnl_to_clean] = lo_chnl_itr;
                        num_lo_chnl_to_clean += 1;
                    }
                    /* we add this one */
                    ho_unchannelized_cycle_itr += 1;
                }
            }
        }
    }

    num_ho_chnl_to_clean = 0;
    num_ho_cycle = 0;
    for (ho_chnl_itr = 0; ho_chnl_itr < max_num_ho_chnl;ho_chnl_itr++)
    {
        if ( odtu_mux_var_reg->mux_stage == ODTU_MUX_STG_TWO || ho_chnl_itr < ODTU_MUX_STG1_MAX_NUM_HO_CHNL)
        {
            if ((UINT32) ODTU_MUX_CHNL_START == odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state)
            {
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].state = UTIL_GLOBAL_CHNL_START;
                ho_chnl_to_clean[num_ho_chnl_to_clean] = ho_chnl_itr;
                num_ho_chnl_to_clean += 1;
            } else 
            {
                for (cycle_itr = 0; cycle_itr < odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].num_cal_entries; cycle_itr++)
                {
                    ho_cycle[num_ho_cycle] =  odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_itr].cal_array[cycle_itr];
                    num_ho_cycle += 1;
                }


                if (odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id == ho_chnl_itr)
                {
                    ho_cycle[num_ho_cycle] =  odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry;
                    num_ho_cycle += 1;
                }
                if (odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id == ho_chnl_itr)
                {
                    ho_cycle[num_ho_cycle] =  odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry;
                    num_ho_cycle += 1;
                }                

            }
        }
    }    
    num_ho_cleaned_cycle = 0;
    for(cycle_itr = 0 ; cycle_itr < max_num_ho_chnl;cycle_itr++)
    {           
        found = FALSE;
        for(ho_cycle_itr = 0; ho_cycle_itr < num_ho_cycle && FALSE == found; ho_cycle_itr+=1)
        {
            if (cycle_itr == ho_cycle[ho_cycle_itr])
            {
                found = TRUE;
            }
        }
        if (FALSE == found)
        {
            ho_cleaned_cycle[num_ho_cleaned_cycle] = cycle_itr;
            num_ho_cleaned_cycle += 1;
        }
    }

    /* PUT LO  channel in RESET */
    for (lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl_to_clean;lo_chnl_itr++)
    {
        odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle, lo_chnl_to_clean[lo_chnl_itr], TRUE);
    }        
    /* PUT HO channel out of reset for cleanup */
    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean;ho_chnl_itr++)
    {
        odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle, ho_chnl_to_clean[ho_chnl_itr], TRUE);
      
    }
    PMC_Q_USLEEP(odtu_mux_handle,200);

    /* get cycle to clean */
    ohfs_insert_chnl_cycle_clean_get(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                     ho_chnl_to_clean,num_ho_chnl_to_clean,                                 
                                     lo_chnl_to_clean,num_lo_chnl_to_clean,                                 
                                     lo_cleaned_cycle,&num_lo_cleaned_cycle);
  
    /* cleanup lo order stuff (for all HO channels) */
    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean;ho_chnl_itr++)
    {

        /* configure MEM_HO_ID first before configuration */
        (void) odtu_mux_ho_id_mem_cfg(odtu_mux_handle, ho_chnl_to_clean[ho_chnl_itr]);
        PMC_Q_USLEEP(odtu_mux_handle,1);

        /* clean OHFS_REMOVE */
        ohfs_insert_lo_cycle_clean(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                   ho_chnl_to_clean,num_ho_chnl_to_clean,
                                   lo_cleaned_cycle,num_lo_cleaned_cycle);
        /* clean LO config */
        if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == TRUE)
        {
            mp_mgen_cycle_clean(odtu_mux_handle->tgmp_handle->mp_mgen_handle,lo_cleaned_cycle,num_lo_cleaned_cycle);
        } else 
        {  
            mp_mgen_chnl_clean(odtu_mux_handle->tgmp_handle->mp_mgen_handle,lo_chnl_to_clean,num_lo_chnl_to_clean);    
        }
        expand_ibuf_lo_cycle_clean( odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                    lo_chnl_to_clean,num_lo_chnl_to_clean,
                                    lo_cleaned_cycle,num_lo_cleaned_cycle);     
        txjc_proc_lo_chnl_clean(odtu_mux_handle->tgmp_handle->txjc_proc_handle,lo_chnl_to_clean,num_lo_chnl_to_clean);  

        /* update the switch */
        (void) odtu_mux_ho_chnl_switch_cfg(odtu_mux_handle,ho_chnl_to_clean[ho_chnl_itr], FALSE);

        /* perform page swap */
        (void) odtu_mux_config_update(odtu_mux_handle,ho_chnl_to_clean[ho_chnl_itr], 0, 0, 1);        
    }
    
    /* wait for page swap completion on all HO channels */
    for(config_update_itr = 0;config_update_itr < 200000; config_update_itr++)
    {
        config_update_done = TRUE;
        for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean && config_update_done == TRUE;ho_chnl_itr++)
        {
            if (1 == expand_ibuf_field_CONFIG_UPDATE_get(NULL, odtu_mux_handle->tgmp_handle->expand_ibuf_handle, ho_chnl_to_clean[ho_chnl_itr]))
            {
                config_update_done = FALSE;
            }                 
         }
        if (config_update_done == TRUE)
        {
            break;
        } else 
        {
            PMC_Q_USLEEP(odtu_mux_handle,1);
        }
    }    
    /* wait for 120 us */
    PMC_Q_USLEEP(odtu_mux_handle,120);
    /* cleanup standby page */
    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean;ho_chnl_itr++)
    {
        /* configure MEM_HO_ID first before configuration */
        (void) odtu_mux_ho_id_mem_cfg(odtu_mux_handle, ho_chnl_to_clean[ho_chnl_itr]);

        /* clean OHFS_REMOVE */
        ohfs_insert_lo_cycle_clean(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                   ho_chnl_to_clean,num_ho_chnl_to_clean,
                                   lo_cleaned_cycle,num_lo_cleaned_cycle);
        /* clean LO config */
        if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == TRUE)
        {
            mp_mgen_cycle_clean(odtu_mux_handle->tgmp_handle->mp_mgen_handle,lo_cleaned_cycle,num_lo_cleaned_cycle);
        } else 
        {  
            mp_mgen_chnl_clean(odtu_mux_handle->tgmp_handle->mp_mgen_handle,lo_chnl_to_clean,num_lo_chnl_to_clean);    
        }
        expand_ibuf_lo_cycle_clean( odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                    lo_chnl_to_clean,num_lo_chnl_to_clean,
                                    lo_cleaned_cycle,num_lo_cleaned_cycle);
        txjc_proc_lo_chnl_clean(odtu_mux_handle->tgmp_handle->txjc_proc_handle,lo_chnl_to_clean,num_lo_chnl_to_clean);
        (void) odtu_mux_ho_chnl_switch_cfg(odtu_mux_handle,ho_chnl_to_clean[ho_chnl_itr], FALSE);

    }
    /* CLEAN ALL HO CHANNELs */
    ohfs_insert_chnl_cycle_clean(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                 ho_chnl_to_clean,num_ho_chnl_to_clean,                                 
                                 lo_chnl_to_clean,num_lo_chnl_to_clean);

    sts_sw_ho_clean(odtu_mux_handle->sts_sw_handle,ho_chnl_to_clean,num_ho_chnl_to_clean,
                    lo_chnl_to_clean,num_lo_chnl_to_clean);      

    expand_ibuf_chnl_cycle_clean(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                 ho_chnl_to_clean,num_ho_chnl_to_clean,                                 
                                 lo_chnl_to_clean,num_lo_chnl_to_clean);
    cfc_fifo_clean(odtu_mux_handle->tgmp_handle->txjc_proc_handle->cfc_handle,lo_chnl_to_clean,num_lo_chnl_to_clean); 
    cfc_fifo_clean(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle,lo_chnl_to_clean,num_lo_chnl_to_clean);  
    cfc_fifo_clean(odtu_mux_handle->tgmp_handle->ohfs_insert_handle->cfc_handle,ho_chnl_to_clean,num_ho_chnl_to_clean);  

    /* clean cycle */
    
    for (lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl_to_clean;lo_chnl_itr++)
    {
        odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle, lo_chnl_to_clean[lo_chnl_itr], FALSE);
    }  
    for(cycle_itr = 0 ; cycle_itr < num_ho_cleaned_cycle;cycle_itr++)
    {           
        odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle,ho_cleaned_cycle[cycle_itr], 0);               
    }
    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean;ho_chnl_itr++)
    {
        odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle, ho_chnl_to_clean[ho_chnl_itr], FALSE);
        if (odtu_mux_var_reg->mux_stage ==  ODTU_MUX_STG_TWO)
        {
            if (odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id == ho_chnl_to_clean[ho_chnl_itr])
            {
                odtu_mux_handle->var.dummy_schd_entry[0].dummy_mo_ch_id = 0x7F;
                odtu_mux_handle->var.dummy_schd_entry[0].dummy_cal_entry = 0x7F;
            }
            if (odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id == ho_chnl_to_clean[ho_chnl_itr])
            {
                odtu_mux_handle->var.dummy_schd_entry[1].dummy_mo_ch_id = 0x7F;
                odtu_mux_handle->var.dummy_schd_entry[1].dummy_cal_entry = 0x7F;
            }
        }
    }  
    PMC_RETURN(rc);
} /* odtu_mux_ctxt_cleanup */


/*******************************************************************************
* odtu_mux_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes ram based registers in ODTU_MUX.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance to be operated on
* 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_mux_ram_init(odtu_mux_handle_t *odtu_mux_handle)
{
    /* variable declaration */
    UINT32 i;

    PMC_ENTRY();    

    /* initializes PASSTHRU */
    for (i = 0; i < 96; i++)
    {
        odtu_mux_field_PASSTHRU_set(NULL, odtu_mux_handle, i, 1);
        odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, i, 0);
    }
    
    PMC_RETURN(PMC_SUCCESS);  
} /* odtu_mux_ram_init */
 
/*******************************************************************************
*  odtu_mux_ho_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures reset mode for an ODU channel in ODTU_MUX.
*   When a HO channel is put into reset, all LO channels associated with the HO
*   channel must be put into reset.
*
* INPUTS:
*   *odtu_mux_handle        - pointer to ODTU_MUX handle instance
*   chnl_id                 - channel ID to be configured
*                             Valid range: 0 - 95
*   chnl_state              - register access state of ODU channel \n
*                             0 -- The ODU channel is in reset mode. It clears all  
*                                  the context information for that channel \n
*                             1 -- The ODU channel is out of reset.
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
PRIVATE PMC_ERROR odtu_mux_ho_chnl_reset_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id, UINT32 chnl_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);
    
    /* argument checking */
    if(chnl_id > 95 || chnl_state > LAST_ODTU_MUX_CHNL_STATE)
    {
        PMC_RETURN(ODTU_MUX_LOG_ERR_INVALID_ARG);
    }
     
    PMC_LOG_TRACE("%s, %s, %d, chnl_state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_state);
    odtu_mux_field_HO_CH_RSTB_set(NULL, odtu_mux_handle, chnl_id, chnl_state);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odtu_mux_ho_chnl_reset_cfg */

/*******************************************************************************
*  odtu_mux_lo_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures reset mode for a LO ODU channel in ODTU Demux.
*
* INPUTS:
*   *odtu_mux_handle        - pointer to ODTU_MUX handle instance
*   chnl_id                 - channel ID to be configured
*                             Valid range: 0 - 95
*   chnl_state              - register access state of ODU channel \n
*                             0 -- The LO ODU channel is in reset mode. \n
*                             1 -- The LO ODU channel is out of reset.
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
PRIVATE PMC_ERROR odtu_mux_lo_chnl_reset_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id, UINT32 chnl_state)
{
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_NULL_HANDLE, 0, 0);
    
    /* argument checking */
    if(chnl_id > 95 || chnl_state > LAST_ODTU_MUX_CHNL_STATE)
    {
        PMC_RETURN(ODTU_MUX_LOG_ERR_INVALID_ARG);
    }
    
    PMC_LOG_TRACE("%s, %s, %d, chnl_state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_state);
    odtu_mux_field_LO_CH_RSTB_set(NULL, odtu_mux_handle, chnl_id, chnl_state);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odtu_mux_lo_chnl_reset_cfg */

/*******************************************************************************
*  odtu_mux_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODTU_MUX var_t variables. 
*
* INPUTS:
*   *odtu_mux_handle  - pointer to the  ODTU_DMX handle
*   *odtu_mux_var     - pointer to the  ODTU_DMX var instance 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_mux_var_default_init(odtu_mux_handle_t *odtu_mux_handle, 
                                       odtu_mux_var_t *odtu_mux_var)
{
    /* variable delcaration */
    UINT32 chnl_id;
    UINT32 ts, lo_ch;
    UINT32 i;
    
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(odtu_mux_handle != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(odtu_mux_var != NULL, ODTU_MUX_ERR_INVALID_PTR, 0, 0);
    
    odtu_mux_var->prov_mode = odtu_mux_handle->var.prov_mode;
    
    for (i = 0; i < UTIL_GLOBAL_MAX_NUM_CAL_ENTRY; i++)
    {
        odtu_mux_var->exp_first_cycle[i] = 0;
        odtu_mux_var->offline_last_lo_chnl[i] = FALSE;
    }
    
    /* initialize all HO chnl cfg */
    for (chnl_id = 0; chnl_id < odtu_mux_handle->cfg.max_chnl_num; chnl_id++)
    {
        odtu_mux_var->ho_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_START;
        odtu_mux_var->ho_chnl_cfg[chnl_id].oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE; 
        odtu_mux_var->ho_chnl_cfg[chnl_id].num_tribslot = 0;
        
        /* initialize all ts config in one HO channel */
        for (ts = 0; ts < ODTU_MUX_NUM_TS_IN_CHNL; ts++)
        {
            odtu_mux_var->ho_chnl_cfg[chnl_id].cal_array[ts] = 0xFF;
            odtu_mux_var->ho_chnl_cfg[chnl_id].tribslot_occupied[ts] = FALSE;
            odtu_mux_var->ho_chnl_cfg[chnl_id].tribslot_added[ts] = 0;
        }
        
        for (lo_ch = 0; lo_ch < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; lo_ch++)
        {
            odtu_mux_var->lo_chnl_cfg[lo_ch].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_START;
            odtu_mux_var->lo_chnl_cfg[lo_ch].lo_oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE; 
            odtu_mux_var->lo_chnl_cfg[lo_ch].ho_oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
            odtu_mux_var->lo_chnl_cfg[lo_ch].num_ts = 0;
            odtu_mux_var->lo_chnl_cfg[lo_ch].ts_type = (util_global_odu_line_payload_t)LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
            odtu_mux_var->lo_chnl_cfg[lo_ch].ho_chnl_id = 0xFF;
            odtu_mux_var->lo_chnl_cfg[lo_ch].mapping_mode = UTIL_GLOBAL_NO_MAP;
        }
        
        /* this is for MO ODU3e1 or MO ODU3e2 */
        for (i = 0; i < 2; i++)
        {
            odtu_mux_var->dummy_schd_entry[i].dummy_mo_ch_id = 0x7F;
            odtu_mux_var->dummy_schd_entry[i].dummy_cal_entry = 0x7F;
        }
       
        /* this is for MO ODU3 */
        for (i = 0; i < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; i++)
        {
            odtu_mux_var->dummy_schd_ctxt[i].dummy_ch_id = 0x7F;
            odtu_mux_var->dummy_schd_ctxt[i].dummy_state = FALSE;
            odtu_mux_var->dummy_schd_ctxt[i].dummy_cycle = 0;
        }
        
        odtu_mux_var->ho_chnl_cfg[chnl_id].ts_type = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
        odtu_mux_var->ho_chnl_cfg[chnl_id].num_active_ts = 0;  
    } 


    for (i = 0; i < ODTU_MUX_GHAO_NUM_CTXT; i++)
    {
        odtu_mux_handle->ghao_mux_state[i].lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_IDLE;
        odtu_mux_handle->ghao_mux_state[i].bwr_state = UTIL_GLOBAL_GHAO_BWR_STATE_IDLE;
        odtu_mux_handle->ghao_mux_state[i].ho_id = 0;
        odtu_mux_handle->ghao_mux_state[i].lo_id = 0;
        odtu_mux_handle->ghao_mux_state[i].tpid = 0;
        PMC_MEMSET(odtu_mux_handle->ghao_mux_state->old_ts,0,sizeof(odtu_mux_handle->ghao_mux_state->old_ts));
        PMC_MEMSET(odtu_mux_handle->ghao_mux_state->new_ts,0,sizeof(odtu_mux_handle->ghao_mux_state->new_ts));
        odtu_mux_handle->ghao_mux_state[i].mux_stage = ODTU_MUX_STG_NONE;
        odtu_mux_handle->ghao_mux_state[i].old_last_cs = FALSE;
    }


    PMC_RETURN();  
} /* odtu_mux_var_default_init */

/*******************************************************************************
*  odtu_mux_ho_chnl_var_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODTU_MUX var_t variables. n.
*
* INPUTS:
*   odtu_mux_handle     - handle to the ODTU_MUX instance to be operated on
*   ho_chnl_id          - HO channel ID. Valid range: 0 - 95
*   chnl_state          - Enum for mux channel state. See odtu_mux_chnl_state_t.
*   ho_odu_type         - Enum for HO ODU type. See util_global_odukp_type_t.
*   num_tribslot        - number of tributary slots in the HO channel.
*                         Valid range: 0 - 80
*   *cal_array          - pointer to the calendar slot offsets of the HO channel.
*   *lo_chnl_ids        - pointer to the LO channel IDs that are provisioned in
*                         the current HO channel.
*   ts_type             - Enum for payload format. See util_global_odu_line_payload_t.
*   num_active_ts       - number of active tributary slots in the current HO channel.
*   num_cal_entries     - number of calendar entries in the HO ODU channel
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_mux_ho_chnl_var_update(odtu_mux_handle_t *odtu_mux_handle, 
                                         UINT32 ho_chnl_id, 
                                         odtu_mux_chnl_state_t chnl_state,
                                         util_global_odukp_type_t ho_odu_type,
                                         UINT32 num_tribslot, UINT8 *cal_array, 
                                         UINT32 *lo_chnl_ids,
                                         util_global_odu_line_payload_t ts_type,
                                         UINT32 num_active_ts,
                                         UINT32 num_cal_entries)                                    
{
    /* variable declaration */
    UINT32 i;
    
    PMC_ENTRY();
    
    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].state = (util_global_chnl_state_t)chnl_state;
    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type = ho_odu_type; 
    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot = num_tribslot;
    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_cal_entries = num_cal_entries;
    
    for (i = 0; i < num_tribslot; i++)
    {
        if (NULL != cal_array)
        {
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i] = cal_array[i];
        } else 
        {
            odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i] = 0xFF;
        }
    }
    
    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type = ts_type;
    odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = num_active_ts;  
    
    PMC_RETURN();
   
} /* odtu_mux_ho_chnl_var_update */


/*******************************************************************************
* odtu_mux_dummy_schd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures a dummy calendar entry to be processed by MUX.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ho_odu_type         - HO ODU type. See util_global_odukp_type_t.
*   cal_entry           - calendar entry for the dummy cycle
*                         Valid range: 0 - 95 
*   ts_type             - payload type of HO. See util_global_odu_line_payload_t 
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
PRIVATE PMC_ERROR odtu_mux_dummy_schd_cfg(odtu_mux_handle_t *odtu_mux_handle, 
                                          UINT32 chnl, 
                                          util_global_odukp_type_t ho_odu_type, 
                                          UINT32 cal_entry,
                                          util_global_odu_line_payload_t ts_type)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    result = expand_ibuf_ho_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                      cal_entry, 0, 1, chnl);   
                                      
    /* configure CAL_EN */
    odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, cal_entry, 1);                                                                         
                                       
    if (PMC_SUCCESS == result && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        /* configure MF_TOT, HO_CAL_CYCLE_TOT */
        result = expand_ibuf_ho_mf_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                       cal_entry, ho_odu_type, ts_type, 0);
                                       
        if (PMC_SUCCESS == result)
        {
            result = expand_ibuf_ho_cal_cycle_tot_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                      cal_entry, chnl, ho_odu_type, ts_type, 0);
        }
    }  
  
    if (PMC_SUCCESS == result)
    {
        result = sts_sw_calendar_null_cfg(odtu_mux_handle->sts_sw_handle, cal_entry);
    }                                     

    PMC_RETURN(result);
} /* odtu_mux_dummy_schd_cfg */

/*******************************************************************************
* odtu_mux_dummy_schd_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures a dummy calendar entry to be processed by MUX.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_MUX handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   cal_entry           - calendar entry for the dummy cycle
*                         Valid range: 0 - 95 
*   ts_type             - payload type of HO. See util_global_odu_line_payload_t 
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
PRIVATE PMC_ERROR odtu_mux_dummy_schd_clean(odtu_mux_handle_t *odtu_mux_handle, 
                                            UINT32 chnl, 
                                            UINT32 cal_entry,
                                            util_global_odu_line_payload_t ts_type)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    /* configure CAL_EN */
    odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, cal_entry, 0);    
    
    result = expand_ibuf_ho_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                      cal_entry, 0, 0, 0x7F);   
                                                                            
    if (PMC_SUCCESS == result && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        /* configure MF_TOT, HO_CAL_CYCLE_TOT */
        result = expand_ibuf_ho_mf_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                       cal_entry, UTIL_GLOBAL_ODUK_DONT_CARE, 
                                       LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 1);
                                       
        if (PMC_SUCCESS == result)
        {
            result = expand_ibuf_ho_cal_cycle_tot_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                      cal_entry, chnl, 
                                                      UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                      LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 1);
        }
    }  
    
    PMC_RETURN(result);
    
} /* odtu_mux_dummy_schd_clean */



/*
** GHAO APIS
*/

/*******************************************************************************
*  odtu_mux_ghao_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize G.HAO state machines and HO_ID memory in ODTU_MUX
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ho_chnl_id             - High Order channel ID
*   lo_chnl_id             - Low Order channel ID
*   tpid                   - Tributary slot ID
*   ts                     - Calendar slot mask of the added / removed TS
*   mux_stage              - Muxing stage
*   ghao_index             - Index of the G.HAO context array
*
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_init(odtu_mux_handle_t *odtu_mux_handle,
                                    UINT8 ho_chnl_id,
                                    UINT8 lo_chnl_id,
                                    UINT8 tpid,
                                    BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                    odtu_mux_stage_instance_t mux_stage,
                                    UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    line_core_otn_db_key       key;
    UINT32             num_of_recs = 0;
    line_core_otn_db_query_res *q_result;

    UINT8 i;
    BOOL8 old_ts[ODTU_MUX_NUM_CHANL];
    BOOL8 new_ts[ODTU_MUX_NUM_CHANL];

    UINT8 old_ts_num;
    UINT8 new_ts_num;
    UINT8 old_last_cs;

    
    if (mux_stage == ODTU_MUX_STG_ONE)
    {
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = lo_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else if (mux_stage == ODTU_MUX_STG_TWO &&
             odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = lo_chnl_id;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else if (mux_stage == ODTU_MUX_STG_TWO &&
             odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = lo_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    
    PMC_LOG_TRACE("db_id = %u\n", odtu_mux_handle->cfg.db_id);
    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                               odtu_mux_handle->cfg.db_id,
                               odtu_mux_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);

    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
    }

    old_last_cs = q_result->p_recs[num_of_recs-1]->calendar_entry;
    odtu_mux_handle->ghao_mux_state[ghao_index].old_last_cs = old_last_cs;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {
        old_ts[i] = FALSE;      
        new_ts[i] = FALSE;      
    }

    for (i = 0; i < num_of_recs; i++) {
        old_ts[q_result->p_recs[i]->calendar_entry] = TRUE;
    }

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {
        new_ts[i] = old_ts[i] ^ ts[i];
    }

    result = odtu_mux_ho_id_mem_cfg(odtu_mux_handle, ho_chnl_id);           
    
    if (PMC_SUCCESS == result) {
        result = odtu_mux_ghao_set_state(odtu_mux_handle,
                                         UTIL_GLOBAL_GHAO_LCR_STATE_IDLE,
                                         UTIL_GLOBAL_GHAO_BWR_STATE_IDLE,
                                         ho_chnl_id,
                                         lo_chnl_id,
                                         tpid,
                                         ts,
                                         old_ts,
                                         new_ts,
                                         mux_stage,
                                         ghao_index);
    }

    /* Initial LCR_STATE is different in passthru mode */
    if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED) {

       odtu_mux_ghao_get_old_ts_num(odtu_mux_handle, &old_ts_num, ghao_index);
       odtu_mux_ghao_get_new_ts_num(odtu_mux_handle, &new_ts_num, ghao_index);

       if (new_ts_num >= old_ts_num) {
         /* G.HAO increase */
            result = odtu_mux_ghao_set_state(odtu_mux_handle,
                                             UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK,
                                             UTIL_GLOBAL_GHAO_BWR_STATE_IDLE,
                                             ho_chnl_id,
                                             lo_chnl_id,
                                             tpid,
                                             ts,
                                             old_ts,
                                             new_ts,
                                             mux_stage,
                                             ghao_index);
       } else {
            /* G.HAO decrease */
            result = odtu_mux_ghao_set_state(odtu_mux_handle,
                                             UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY,
                                             UTIL_GLOBAL_GHAO_BWR_STATE_IDLE,
                                             ho_chnl_id,
                                             lo_chnl_id,
                                             tpid,
                                             ts,
                                             old_ts,
                                             new_ts,
                                             mux_stage,
                                             ghao_index);
        }
    }
                    
    PMC_RETURN(result);
} /* odtu_mux_ghao_init */  

/*******************************************************************************
*  odtu_mux_ghao_send_add_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send ADD NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_add_nack(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    TRUE,          /* rp_flag */
                                    TRUE,          /* rp = 1 */
                                    UTIL_GLOBAL_GHAO_LCR_CTRL_ADD,
                                    var_tpid,      /* TPID */
                                    FALSE,         /* TSGS = NACK */
                                    FALSE);        /* TSCC = 0 */

    /* update state */

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_IDLE ) {
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK;
    } else {
        PMC_LOG_TRACE("ODTU_MUX SEND ADD NACK : unexpected lcr_state from var\n");
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK;
    }

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_add_nack */  


/*******************************************************************************
*  odtu_mux_ghao_send_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send ADD ACK on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context arraydtu_mux_ghao
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_add_ack(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT8 ghao_index)

{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    FALSE,         /* rp_flag */
                                    TRUE,          /* rp = 1 */
                                    UTIL_GLOBAL_GHAO_LCR_CTRL_ADD,
                                    var_tpid,      /* TPID */
                                    TRUE,          /* TSGS = ACK */
                                    FALSE);        /* TSCC = 0 */

    /* update state */

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK ) {
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_ACK;
    } else {
        PMC_LOG_TRACE("ODTU_MUX SEND ADD ACK : unexpected lcr_state from var\n");
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_ACK;
    }

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_add_ack */  


/*******************************************************************************
*  odtu_mux_ghao_send_remove_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send REM NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_remove_nack(odtu_mux_handle_t *odtu_mux_handle,
                                                UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    TRUE,          /* rp_flag */
                                    TRUE,          /* rp = 1 */
                                    UTIL_GLOBAL_GHAO_LCR_CTRL_REMOVE,
                                    var_tpid,      /* TPID */
                                    FALSE,         /* TSGS = NACK */
                                    FALSE);        /* TSCC = 0 */

    /* update state */

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_IDLE ) {
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_REM_NACK;
    } else {
        PMC_LOG_TRACE("ODTU_MUX SEND REMOVE NACK : unexpected lcr_state from var\n");
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_REM_NACK;
    }

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_remove_nack */  

/*******************************************************************************
*  odtu_mux_ghao_send_remove_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send REM ACK on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_remove_ack(odtu_mux_handle_t *odtu_mux_handle,
                                               UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    TRUE,          /* rp_flag */
                                    TRUE,          /* rp = 1 */
                                    UTIL_GLOBAL_GHAO_LCR_CTRL_REMOVE,
                                    var_tpid,      /* TPID */
                                    TRUE,          /* TSGS = ACK */
                                    FALSE);        /* TSCC = 0 */

    /* update state */

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY ) {
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_REM_ACK;
    } else {
        PMC_LOG_TRACE("ODTU_MUX SEND REMOVE ACK : unexpected lcr_state from var\n");
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_REM_ACK;
    }

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_remove_ack */  

/*******************************************************************************
*  odtu_mux_ghao_send_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send TSCC on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   tscc                   - TSCC value to be sent
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_tscc(odtu_mux_handle_t *odtu_mux_handle,
                                             BOOL8 tscc,
                                             UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;

    util_global_ghao_lcr_ctrl_t ctrl;
    UINT8 tpid;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    if (TRUE == tscc) {

        if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_IDLE_NACK ) {
            ctrl = UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE;
            tpid = 0;
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY;
        } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_REM_NACK ) {
            ctrl = UTIL_GLOBAL_GHAO_LCR_CTRL_REMOVE;
            tpid = var_tpid;
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY;
        } else {
            PMC_LOG_TRACE("ODTU_MUX SEND TSCC %u : unexpected lcr_state from var\n",tscc);
            ctrl = UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE;
            tpid = 0;
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY;
        }
        
        if( bwr_state == UTIL_GLOBAL_GHAO_BWR_STATE_IDLE ) {
          bwr_state = UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_NACK;
        } else {
          PMC_LOG_TRACE("ODTU_MUX SEND TSCC %u : unexpected bwr_state from var\n",tscc);
          bwr_state = UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_NACK;
        }

    } else {

        if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY ) {
            ctrl = UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE;
            tpid = 0;
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY;
        } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY ) {
            ctrl = UTIL_GLOBAL_GHAO_LCR_CTRL_REMOVE;
            tpid = var_tpid;
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY;
        } else {
            PMC_LOG_TRACE("ODTU_MUX SEND TSCC %u : unexpected lcr_state from var\n",tscc);
            ctrl = UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE;
            tpid = 0;
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY;
        }
        
        if( bwr_state == UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_NACK ) {
            bwr_state = UTIL_GLOBAL_GHAO_BWR_STATE_NO_TSCC_NACK;
        } else {
            PMC_LOG_TRACE("ODTU_MUX SEND TSCC %u : unexpected bwr_state from var\n",tscc);
            bwr_state = UTIL_GLOBAL_GHAO_BWR_STATE_NO_TSCC_NACK;
        }

    }

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    TRUE,          /* rp_flag */
                                    TRUE,          /* rp = 1 */
                                    ctrl,          /* CTRL = IDLE or REM */
                                    0,             /* TPID=0 during BWR */
                                    FALSE,         /* TSGS = NACK */
                                    tscc);         /* TSCC */

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_tscc */  

/*******************************************************************************
*  odtu_mux_ghao_send_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set RP bit in RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   rp                     - RP value to be sent
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * The HO / LO ID combination is not provisioned
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_rp(odtu_mux_handle_t *odtu_mux_handle,
                                           BOOL8 rp,
                                           UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    if (TRUE == rp) {

      PMC_LOG_TRACE("ODTU_MUX SEND RP %u\n",rp);

    } else {

        if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY ) {
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_IDLE;
        } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_IDLE_NACK ) {
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_IDLE;
        } else {
            PMC_LOG_TRACE("ODTU_MUX SEND RP %u : unexpected lcr_state from var\n",rp);
            lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_IDLE;
        }

    }

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    TRUE,          /* rp_flag */
                                    rp,            /* rp */
                                    UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE,
                                    0,             /* TPID = 0 */
                                    FALSE,         /* TSGS = NACK */
                                    FALSE);        /* TSCC = 0 */

    /* update state */

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_rp */  

/*******************************************************************************
*  odtu_mux_ghao_send_norm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send NORM on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_norm(odtu_mux_handle_t *odtu_mux_handle,
                                         UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_ADD_ACK ) {
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_NORM;
    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_REM_ACK ) {
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_DECR_NORM;
    } else {
        PMC_LOG_TRACE("ODTU_MUX SEND NORM : unexpected lcr_state from var\n");
        lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_NORM;
    }



    if (pmc_is_digi_rev_a_revision(&odtu_mux_handle->base) == FALSE)
    {
        UINT8 i;
        for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
        {
            if (TRUE == var_trib_slot[i]) {
                PMC_LOG_TRACE("ohfs_insert_field_ncs_en_set(%u, 1) ", i);
                ohfs_insert_ncs_en_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                       i,
                                       1);
            }
        }
    }


    PMC_Q_USLEEP(odtu_mux_handle,110);

    odtu_mux_config_update(odtu_mux_handle,
                           var_ho_chnl_id,
                           1,  /* config_mode */
                           1,  /* norm_cmd */
                           1); /* config_update */

    /* update state */

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_send_norm */  

/*******************************************************************************
*  odtu_mux_ghao_disable_norm
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send IDLE NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_disable_norm(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
    UINT8 i;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
    {
        if (TRUE == var_trib_slot[i]) {
            PMC_LOG_TRACE("DISABLE_NORM: ohfs_insert_field_ncs_en_set(%u, 0) ", i);
            ohfs_insert_ncs_en_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                   i,
                                   0);
        }
    }

    PMC_RETURN(result);
} /* odtu_mux_ghao_disable_norm */  

/*******************************************************************************
*  odtu_mux_ghao_send_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Send IDLE NACK on RCOH of currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_send_idle_nack(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
    UINT8 i;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_NORM ) {
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_IDLE_NACK;
    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_NORM ) {
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_DECR_IDLE_NACK;
    } else {
      PMC_LOG_TRACE("ODTU_MUX SEND IDLE NACK : unexpected lcr_state from var\n");
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_IDLE;
    }

    result = odtu_mux_ghao_set_rcoh(odtu_mux_handle,
                                    var_trib_slot, /* Trib_slot list */
                                    FALSE,         /* rp_flag */
                                    TRUE,          /* rp */
                                    UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE,
                                    0,             /* TPID=0 for IDLE_NACK */
                                    FALSE,         /* TSGS = NACK */
                                    FALSE);        /* TSCC = 0 */


    odtu_mux_config_update(odtu_mux_handle,
                           var_ho_chnl_id,
                           0,  /* config_mode */
                           0,  /* norm_cmd */
                           0); /* config_update */

    /* update state */

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);



    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
    {
        if (TRUE == var_trib_slot[i]) {
            PMC_LOG_TRACE("DISABLE_NORM: ohfs_insert_field_ncs_en_set(%u, 0) ", i);
            ohfs_insert_ncs_en_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                   i,
                                   0);
        }
    }


    PMC_RETURN(result);
} /* odtu_mux_ghao_send_idle_nack */  


/*******************************************************************************
*  odtu_mux_ghao_update_stdby_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update standby configuration registers in currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_update_stdby_config(odtu_mux_handle_t *odtu_mux_handle,
                                                   UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    UINT8 lo_chnl;

    UINT32 new_tribslot_mask[3];
    UINT8 new_num_ts;
    UINT8 old_num_ts;
    BOOL8 add = TRUE;
    UINT32 prev_cn_sel = 0;
    BOOL8 config_update = FALSE;
    BOOL8 offline_update = FALSE;

    UINT32 cnt = 0;
    UINT32 num_ts = 0;
    UINT32 total_allowed_ts = 0;
    UINT32 ts_id;
    UINT32 db_chnl_id = ODTU_MUX_DB_ENTRY_UNKNOWN;
    UINT8 old_highest_cs = 0;
    UINT8 new_highest_cs = 0;
    line_core_otn_db_key    key;
    line_core_otn_db_rec    new_rec;
    UINT32          num_of_recs;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    util_global_mapping_mode_t   mapping_mode;
    UINT8 dummy_num_cal;

    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    old_highest_cs = odtu_mux_handle->ghao_mux_state[ghao_index].old_last_cs;


    /*PMC_LOG_TRACE("ODTU_MUX GHAO UPDATE STDBY CONFIG: lcr_state from var is %s", lcr_state); */

    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK ) {
      add = TRUE;
      prev_cn_sel = 1;

    } else if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_IDLE_NACK ) {
      add = TRUE;
      prev_cn_sel = 1;
      offline_update = TRUE;

    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY ) {
      add = TRUE;
      prev_cn_sel = 0;
      config_update = TRUE;

    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_REM_NACK ) {
      add = FALSE;
      prev_cn_sel = 1;
      config_update = TRUE;

    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY ) {
      add = FALSE;
      prev_cn_sel = 0;

    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_IDLE_NACK ) {
      add = FALSE;
      prev_cn_sel = 0;
      offline_update = TRUE;

    } else {
      PMC_LOG_TRACE("ODTU_MUX GHAO UPDATE STDBY CONFIG: unexpected lcr_state from var\n");
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK;
    }

    if(add == TRUE)
    {
        new_highest_cs = 0;
    }
    else
    {
        new_highest_cs = 96;
    }

    PMC_LOG_TRACE("ODTU_MUX GHAO UPDATE STDBY CONFIG: prev_cn_sel=%u, config_update=%u, offline_update=%u, add=%u\n",
                                                     prev_cn_sel, config_update,    offline_update,    add);

    result = odtu_mux_ho_id_mem_cfg(odtu_mux_handle, var_ho_chnl_id);

    /* count number of trib slot to add/remove */
    new_num_ts = 0;
    old_num_ts = 0;

    mapping_mode = odtu_mux_handle->var.lo_chnl_cfg[var_lo_chnl_id].mapping_mode;
    
    if ( config_update == FALSE ) {

        new_tribslot_mask[0] = 0;
        new_tribslot_mask[1] = 0;
        new_tribslot_mask[2] = 0;
                  
        /* adjust trib slot position for lo channels */
        for (lo_chnl = 0; lo_chnl < ODTU_MUX_NUM_CHANL; lo_chnl++) {
          if( var_new_trib_slot[lo_chnl] == TRUE ) { 
            new_num_ts += 1;
          }

          if(var_old_trib_slot[lo_chnl] == TRUE) {
            old_num_ts += 1;
          }
        }

        if (offline_update == FALSE &&
            lcr_state != UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY) {
          /* GHAO add/remove */
          result = odtu_mux_ghao_lo_chnl_deprov( odtu_mux_handle,
                                                 var_mux_stage,
                                                 var_lo_chnl_id,
                                                 var_ho_chnl_id,
                                                 TRUE,
                                                 old_num_ts,
                                                 &dummy_num_cal);
        }

        if (PMC_SUCCESS == result) {
        
            num_ts = odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].num_tribslot;
            PMC_LOG_TRACE("num_ts = %u\n", num_ts);
        
            /* Get total number of allowed ts in the LO channel */
            total_allowed_ts = new_num_ts;
            PMC_LOG_TRACE("total_allowed_ts = %u\n", total_allowed_ts);
            odtu_mux_handle->var.lo_chnl_cfg[var_lo_chnl_id].num_ts = total_allowed_ts;
            
            /* query the record to update (only supports stage one) */
            if (var_mux_stage != ODTU_MUX_STG_TWO)
            {
                /* mo is being deprovisioned */
                /* query the records to find the calendar entries for this LO ODU channel */
                key.calendar_entry = DO_NOT_USE_KEY;
                key.ho_channel = var_ho_chnl_id;
                key.mo_channel = DO_NOT_USE_KEY;
                key.lo_channel = DO_NOT_USE_KEY;
                key.trib_slot = DO_NOT_USE_KEY;
            }
            else {
                /* lo is being deprovisioned */
                /* query the records to find the calendar entries for this LO ODU channel */
                key.calendar_entry = DO_NOT_USE_KEY;
                key.ho_channel = DO_NOT_USE_KEY;
                key.mo_channel = var_ho_chnl_id;
                key.lo_channel = DO_NOT_USE_KEY;
                key.trib_slot = DO_NOT_USE_KEY;
            }

            /* find the total number of records that matches the HO channel ID */
            num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                                       odtu_mux_handle->cfg.db_id,
                                       odtu_mux_handle->cfg.db_handle, (void *)&key,
                                       (void **)&q_result);
            if (num_of_recs == 0)
            {
                PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
            }                                   
            temp_result = *q_result;
        
            if (num_of_recs > 0)
            {

                /* Update the database for the LO channel ID */
                /* ts could range from 0 - max num ts in HO */
                for (ts_id = 0; ts_id < num_ts; ts_id++)
                {
                    if (TRUE == var_new_trib_slot[temp_result.p_recs[ts_id]->calendar_entry])
                    {   
                        new_highest_cs = 0;
                        if ( new_highest_cs < temp_result.p_recs[ts_id]->calendar_entry )
                        {
                            new_highest_cs =  temp_result.p_recs[ts_id]->calendar_entry;
                        }
                        new_tribslot_mask[ts_id/32] |= (1<< (ts_id%32));
                        PMC_LOG_TRACE("new_trib_slot[%d] changed to = %X on index %d with offset %d\n", (ts_id/32),new_tribslot_mask[ts_id/32], ts_id, (ts_id%32));
        
                        if (cnt == 0) {
                            db_chnl_id = var_lo_chnl_id;
                          }
                          PMC_LOG_TRACE("ts_id = %u, db_chnl_id = %u\n", ts_id, db_chnl_id);  
                          PMC_LOG_TRACE("1.temp_result.p_recs[ts_id]->mo_channel = %u\n", temp_result.p_recs[ts_id]->mo_channel);  

                        if (offline_update == FALSE) {
                          if (var_mux_stage == ODTU_MUX_STG_ONE)
                          {
                              if ( ODTU_MUX_DB_ENTRY_UNKNOWN == (temp_result.p_recs[ts_id]->mo_channel) )
                            {
                                PMC_LOG_TRACE("2.temp_result.p_recs[ts_id]->mo_channel = %u\n", temp_result.p_recs[ts_id]->mo_channel);  
                                /* calendar entry is not assigned to any tributary slot/LO channel */
                                /* update MO channel ID to 'calendar_entry' */
                                  new_rec = *temp_result.p_recs[ts_id];
                                  new_rec.mo_channel = db_chnl_id;
                                  new_rec.trib_slot = ts_id;
                                  new_rec.cal_entry_status = 1;
                                  /*new_rec.lo_channel = DO_NOT_USE_KEY;*/
                                  
                                  PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                              new_rec.ho_channel,
                                              new_rec.trib_slot,
                                              new_rec.mo_channel,
                                              new_rec.lo_channel);
   
                                  cnt++;                                        
                              }
                          }   
                          else if (var_mux_stage == ODTU_MUX_STG_TWO)
                          {
                        if ( ODTU_MUX_DB_ENTRY_UNKNOWN == (temp_result.p_recs[ts_id]->lo_channel) )
                        {
                            /* calendar entry is not assigned to any tributary slot/LO channel */
                            /* update LO channel ID to 'chnl_id' */
                              new_rec = *temp_result.p_recs[ts_id];
                              new_rec.lo_channel = db_chnl_id;
                              new_rec.ts_port_mo_lo = ts_id;
                              new_rec.cal_entry_status = 1;
                              /*new_rec.mo_channel = DO_NOT_USE_KEY;*/
                              
                              PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                          new_rec.ho_channel,
                                          new_rec.trib_slot,
                                          new_rec.mo_channel,
                                          new_rec.lo_channel);
                       
                              cnt++;
                              }
                          }
                        }
                        
                        odtu_mux_handle->var.lo_chnl_cfg[var_lo_chnl_id].num_ts = total_allowed_ts;
                        odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].tribslot_occupied[ts_id] = 1;
                    }                                       
                } 

                if (offline_update == FALSE) {
                  odtu_mux_lo_chnl_var_update(odtu_mux_handle, var_lo_chnl_id, var_ho_chnl_id, 
                                              ODTU_MUX_CHNL_EQUIPPED,
                                              UTIL_GLOBAL_ODUFLEX_GFP,
                                              odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].oduk_type,
                                              total_allowed_ts, 
                                              UTIL_GLOBAL_ODU_TS_1G25,
                                              UTIL_GLOBAL_GMP);
                }
            }       
        }

        PMC_LOG_TRACE("new_trib_slot[0] = %X\n", new_tribslot_mask[0]);
        PMC_LOG_TRACE("new_trib_slot[1] = %X\n", new_tribslot_mask[1]);
        PMC_LOG_TRACE("new_trib_slot[2] = %X\n", new_tribslot_mask[2]);

    }
    if (PMC_SUCCESS == result) {
      result = txjc_proc_cn_select(odtu_mux_handle->tgmp_handle->txjc_proc_handle,
                                   var_lo_chnl_id, prev_cn_sel);
    }

    if (offline_update == FALSE ) {

          if ( (new_highest_cs > old_highest_cs &&  add == TRUE) ||
               (old_highest_cs > new_highest_cs &&  add == FALSE) )
          {
              if (odtu_mux_handle->var.exp_first_cycle[var_lo_chnl_id] == 1) {
                  odtu_mux_handle->var.exp_first_cycle[var_lo_chnl_id] = 0;
              } else if (odtu_mux_handle->var.exp_first_cycle[var_lo_chnl_id] == 0) {
                  odtu_mux_handle->var.exp_first_cycle[var_lo_chnl_id] = 1;
              } else {
                  PMC_LOG_TRACE("odtu_mux_handle->var.exp_first_cycle has legal value %u on channel %u\n",
                            odtu_mux_handle->var.exp_first_cycle[var_lo_chnl_id], var_lo_chnl_id);
                  PMC_RETURN(ODTU_MUX_ERR_CHNL_INVALID_STATE);
              }

              /*set jc_wr_ipt_va[new last cycle] = first_cycle */
              /*set jc_wr_ipt_valid[new last cycle] = 1 */
              result = ohfs_insert_lo_ipt_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                              new_highest_cs,
                                              var_lo_chnl_id,
                                              odtu_mux_handle->var.exp_first_cycle[var_lo_chnl_id],
                                              1);
              
              /*update old_last_cs in context so that FIRST_CYCLE, and JC_WR_IPT_VA/VALID
              only get updated once suring G.HAO */
              odtu_mux_handle->ghao_mux_state[ghao_index].old_last_cs = new_highest_cs;

          }
    }



    if ( config_update == FALSE ) {
        if (PMC_SUCCESS == result) {
            result = odtu_mux_lo_chnl_prov( odtu_mux_handle,
                                            var_lo_chnl_id,
                                            var_mux_stage,
                                            UTIL_GLOBAL_ODUFLEX_GFP,
                                            new_num_ts,
                                            UTIL_GLOBAL_GMP,
                                            new_tribslot_mask,
                                            var_tpid,
                                            odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].ts_type,
                                            odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].oduk_type,
                                            var_ho_chnl_id,
                                            1,
                                            0,
                                            TRUE);
        }
        
        if (PMC_SUCCESS == result) {
            result = sts_sw_cfg(odtu_mux_handle->sts_sw_handle, var_ho_chnl_id, FALSE);
        }

        if (offline_update == FALSE) {
          PMC_LOG_TRACE("odtu_mux_ghao_update_stdby_config: old var.ho_chnl_cfg[%u].num_active_ts = %u\n",
                     var_ho_chnl_id, odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].num_active_ts);
  
          odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].num_active_ts = odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].num_active_ts
                                                                       + total_allowed_ts; 
          PMC_LOG_TRACE("odtu_mux_ghao_update_stdby_config: new var.ho_chnl_cfg[%u].num_active_ts = %u\n",
                     var_ho_chnl_id, odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].num_active_ts);
        }

        /* If decreasing, we need to set the removed TS to Unallocated */
        if (add == FALSE) {
          for (ts_id = 0; ts_id < ODTU_MUX_NUM_CHANL; ts_id++) {
            if (PMC_SUCCESS == result) {
              if (var_trib_slot[ts_id]) {
                if (result == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("odtu_mux_ghao_update_stdby_config: Set MSI byte to Unallocated in cal slot %u",ts_id);
                    result = ohfs_insert_msi_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                 ts_id, 
                                                 odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].oduk_type,
                                                 UTIL_GLOBAL_ODUK_DONT_CARE, 
                                                 odtu_mux_handle->var.ho_chnl_cfg[var_ho_chnl_id].ts_type, 
                                                 0);  
                } 

                PMC_LOG_TRACE("odtu_mux_ghao_update_stdby_config(): Disable cal_slot %d\n", ts_id);
                if (result == PMC_SUCCESS)
                {
                    result = expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                      ts_id, 0);
                } 
                if (result == PMC_SUCCESS)
                {
                    result = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                                ts_id, 0);
                }                                                                                                                                                                                
                if (result == PMC_SUCCESS)
                {
                    result = ohfs_insert_lo_chnl_id_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                        ts_id, 
                                                        0); 
                }
                if (result == PMC_SUCCESS)
                {
                    result = ohfs_insert_ts_cfg(odtu_mux_handle->tgmp_handle->ohfs_insert_handle, 
                                                ts_id, 
                                                0, 0);                               
                }
              }
            }
          }
        }
    }


    if (PMC_SUCCESS == result && TRUE == config_update) {
        result = odtu_mux_config_update(odtu_mux_handle,
                                        var_ho_chnl_id,
                                        0,  /* config_mode */
                                        0,  /* norm_cmd */
                                        1); /* config_update */

        PMC_Q_USLEEP(odtu_mux_handle,110);
        /* update the PREV_CN_SEL register on the offline page */
        if (PMC_SUCCESS == result) {
            result = txjc_proc_cn_select(odtu_mux_handle->tgmp_handle->txjc_proc_handle,
                                   var_lo_chnl_id, prev_cn_sel);
        }
    }



    /*mux state should not change through G.HAO opertation */
    odtu_mux_handle->var.lo_chnl_cfg[var_lo_chnl_id].state = (util_global_chnl_state_t)ODTU_MUX_CHNL_OPERATIONAL;

    PMC_RETURN(result);

} /* odtu_mux_ghao_update_stdby_config */  



/*******************************************************************************
*  odtu_mux_ghao_resize_fifo
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resize EXPAND_IBUF and TXJC_PROC CFC FIFOs in currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_resize_fifo(odtu_mux_handle_t *odtu_mux_handle,
                                           UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    UINT8 old_ts_num;
    UINT8 new_ts_num;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    odtu_mux_ghao_get_new_ts_num(odtu_mux_handle, &new_ts_num, ghao_index);

    result = odtu_mux_ho_id_mem_cfg(odtu_mux_handle, var_ho_chnl_id);

    odtu_mux_ghao_get_old_ts_num(odtu_mux_handle, &old_ts_num, ghao_index);
    
    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK ) {

       if (old_ts_num == 1) {

         result = cfc_fifo_resize_one_ts_step1(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle,
                                               var_lo_chnl_id, new_ts_num);
         
         if (PMC_SUCCESS == result) {
           result = odtu_mux_ghao_adjust_xoff_lvl(odtu_mux_handle,3,ghao_index);
         }

         if (PMC_SUCCESS == result) {
            result = cfc_fifo_resize_one_ts_step2(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle,
                                                  var_lo_chnl_id, new_ts_num, old_ts_num);
         }

         if (PMC_SUCCESS == result) {
           result = odtu_mux_ghao_adjust_xoff_lvl(odtu_mux_handle,4,ghao_index);
         }

       } else {

         result = cfc_fifo_resize(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle,
                                  var_lo_chnl_id, new_ts_num);

       }

    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY ) {

       if( (old_ts_num - new_ts_num) != 1)
       {
          result = cfc_fifo_resize(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                    var_lo_chnl_id, new_ts_num+1);

       }

    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_NORM ) {

      
       if(new_ts_num == 1)
       {
           /* So the resize operation can complete, change xoff threshold to 3*/
           result = odtu_mux_ghao_adjust_xoff_lvl(odtu_mux_handle,
                                                  3,
                                                  ghao_index);
       }


       result = cfc_fifo_resize(odtu_mux_handle->tgmp_handle->expand_ibuf_handle->cfc_handle, 
                                var_lo_chnl_id, new_ts_num);

       if(new_ts_num == 1)
       {
           /* So the resize operation can complete, change xoff threshold to 3*/
           result = odtu_mux_ghao_adjust_xoff_lvl(odtu_mux_handle,
                                                  4,
                                                  ghao_index);
       }


    } else {
      PMC_LOG_TRACE("ODTU_MUX GHAO RESIZE FIFO : unexpected lcr_state from var\n");
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK;
    }
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_resize_fifo */  

/*******************************************************************************
*  odtu_mux_ghao_adjust_xoff_lvl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Adjust EXPAND_IBUF and TXJC_PROC CFC FIFO XOFF level in 
*   currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   xoff_lvl               - New CFC FIFOs XOFF level
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_adjust_xoff_lvl(odtu_mux_handle_t *odtu_mux_handle,
                                               UINT8 xoff_lvl,
                                               UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;
              
    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    result = expand_ibuf_cfc_fill_lvl_set(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                          var_lo_chnl_id,
                                          xoff_lvl);

    PMC_RETURN(result);
} /* odtu_mux_ghao_adjust_xoff_lvl */  

/*******************************************************************************
*  odtu_mux_ghao_recfg_passthru
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reconfigures the EXPAND_IBUF HO channels and ODTU_MUX calendar when in
*   passthru mode during G.HAO resizing
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_recfg_passthru(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;

    UINT8 i;
    UINT8 last_cycle_idx;
    UINT8 num_lo_ts;
              
    util_global_odu_line_payload_t ts_type;
    UINT32 first_cycle, last_cycle, unset;
    UINT32 frm_num = 0;
    UINT32 last_data_cycle;
    UINT32 server_size;
    UINT32 div;
    UINT32 lo_eomf_data_cycle;

    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    /* update state */
    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK ) {
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY;
    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY ) {
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_DECR_NORM;
    } else {
      PMC_LOG_TRACE("ODTU_MUX RECFG PASSTHRU : unexpected lcr_state from var");
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK;
    }

    odtu_mux_ghao_get_new_ts_last_cycle(odtu_mux_handle,
                                        &last_cycle_idx,
                                        ghao_index);

    ts_type = UTIL_GLOBAL_ODU_UNCHANNELIZED;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {

      if (var_old_trib_slot[i] || var_new_trib_slot[i]) {
        first_cycle = (var_new_trib_slot[i] == 1)? 1:0;
        last_cycle = (i == last_cycle_idx)? 1:0;
        unset = (var_new_trib_slot[i] == 0)? 1:0;

        PMC_LOG_TRACE("i = %u ; last_cycle_idx = %u ; first_cycle = %u ; last_cycle = %u ; unset = %u", 
                   i,       last_cycle_idx,       first_cycle,       last_cycle,       unset);

        /* reconfigure HO_MF_TOT */
        if (PMC_SUCCESS == result) {
          result = expand_ibuf_ho_chnl_prov_all(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                i, var_ho_chnl_id,
                                                UTIL_GLOBAL_ODUFLEX_GFP,
                                                ts_type,
                                                first_cycle, last_cycle, unset);
        }
                
        odtu_mux_ghao_get_new_ts_num(odtu_mux_handle,
                                     &num_lo_ts,
                                     ghao_index);

        server_size = 15232 * num_lo_ts;
        if ((server_size % 48) == 0)
        {
          div = (server_size / 48);
        }
        else {
          div = (server_size / 48) + 1;
        }
        
        last_data_cycle = div % num_lo_ts;
        if (last_data_cycle == 0) 
        {
          last_data_cycle = num_lo_ts - 1;
        }
        else {
          last_data_cycle--;
        }
        
        lo_eomf_data_cycle = (last_data_cycle == frm_num)? 1:0;

        /* Clearing EOMF_DATA_CYCLE if the TS is removed */
        if (var_new_trib_slot[i] == 0) {
          lo_eomf_data_cycle = 0;
        }
        
        PMC_LOG_TRACE("frm_num = %u\n", frm_num);
        PMC_LOG_TRACE("num_lo_ts = %u\n", num_lo_ts);
        PMC_LOG_TRACE("server_size = %u\n", server_size);
        PMC_LOG_TRACE("lo_eomf_data_cycle = %u", lo_eomf_data_cycle);

        if (PMC_SUCCESS == result) {
          result = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle, 
                                                      i, lo_eomf_data_cycle);
        }

        if (PMC_SUCCESS == result) {
          odtu_mux_field_CAL_EN_set(NULL, odtu_mux_handle, i, first_cycle);
        }

        if (var_new_trib_slot[i]) {
          frm_num++;
        }
      }
    }

    odtu_mux_ghao_set_state(odtu_mux_handle, 
                            lcr_state,
                            bwr_state,
                            var_ho_chnl_id,
                            var_lo_chnl_id,
                            var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            var_mux_stage,
                            ghao_index);

    PMC_RETURN(result);
} /* odtu_mux_ghao_recfg_passthru */  


/*******************************************************************************
*  odtu_mux_ghao_recfg_exp_lo_calen
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reconfigures the EXPAND_IBUF LO calendar in passthru mode during G.HAO
*   resizing
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   cycle                  - LO calendar cycle that will be enabled / disabled
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_recfg_exp_lo_calen(odtu_mux_handle_t *odtu_mux_handle,
                                                  UINT32 cycle,
                                                  UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    util_global_ghao_lcr_state_t lcr_state;
    util_global_ghao_bwr_state_t bwr_state;
    UINT8 var_ho_chnl_id;
    UINT8 var_lo_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_old_trib_slot[ODTU_MUX_NUM_CHANL];
    BOOL8 var_new_trib_slot[ODTU_MUX_NUM_CHANL];
    odtu_mux_stage_instance_t var_mux_stage;

    UINT32 first_cycle = 0;

    odtu_mux_ghao_get_state(odtu_mux_handle, 
                            &lcr_state,
                            &bwr_state,
                            &var_ho_chnl_id,
                            &var_lo_chnl_id,
                            &var_tpid,
                            var_trib_slot,
                            var_old_trib_slot,
                            var_new_trib_slot,
                            &var_mux_stage,
                            ghao_index);

    /* update state */
    if( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY ) {
      first_cycle = 1;
    } else if ( lcr_state == UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY ) {
      first_cycle = 0;
    } else {
      PMC_LOG_TRACE("ODTU_MUX RECFG EXP LO CALEN : unexpected lcr_state from var");
      lcr_state = UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY;
    }

    result =  expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                       cycle, first_cycle);

    PMC_RETURN(result);
} /* odtu_mux_ghao_recfg_exp_lo_calen */  


/*******************************************************************************
*  odtu_mux_ghao_end
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear LCR and BWR context variables for G.HAO
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_end(odtu_mux_handle_t *odtu_mux_handle,
                                   UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    UINT8  i;
    BOOL8  ts[ODTU_MUX_NUM_CHANL];

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
    {
      ts[i] = FALSE;
    }

    if (PMC_SUCCESS == result) {
      result = odtu_mux_ghao_set_state(odtu_mux_handle,
                                       LAST_UTIL_GLOBAL_GHAO_LCR_STATE,
                                       LAST_UTIL_GLOBAL_GHAO_BWR_STATE,
                                       0xFF,
                                       0xFF,
                                       0xFF,
                                       ts,
                                       ts,
                                       ts,
                                       ODTU_MUX_STG_NONE,
                                       ghao_index);
    }
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_end */  

/*******************************************************************************
*  odtu_mux_ghao_set_rcoh
* ______________________________________________________________________________
*
* DESCRIPTION:
*      Set RCOH in resize LO channel
*  
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ts                     - Calendar slot mask on which the RCOH is updated
*   rp_flag                - Indicates that RP bit must be set in OHFS_INSERT
*   rp                     - Resizing Protocol indicator
*                              0 : RCOH carries ovherhead associated with the mapping 
*                              1 : RCOH is carrying resizing protocol
*   ctrl                  - LCR control field
*                              00 : idle
*                              01 : add
*                              10 : remove
*                              11 : norm
*   tpid                   - Identifier for tributary port ID
*   tsgs                   - Link connection acknowledgment indication
*                              0 : nack
*                              1 : ack
*   tscc                   - Tributary slot connectivity check
*                              0 : Transition to 0 when it exited the GMP special 
*                                  mode and indicate bw resize completion
*                              1 : confirm GMP special at intermediate nodes and 
*                                  signal sink that all NE so to sk are ready to
*                                  support the bw resizing
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_set_rcoh(odtu_mux_handle_t *odtu_mux_handle,
                                        BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                        BOOL8 rp_flag,
                                        BOOL8 rp,
                                        util_global_ghao_lcr_ctrl_t ctrl,
                                        UINT8 tpid,
                                        BOOL8 tsgs,
                                        BOOL8 tscc)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    UINT8  i;
    UINT32 rcoh = 0;
    UINT32 rcoh_row1 = 0;
    UINT32 rcoh_row2 = 0;

    rcoh_row1  = rp << 7;
    rcoh_row1 |= tpid >> 2;

    rcoh_row2  = tscc << 7;
    rcoh_row2 |= tsgs << 4;
    rcoh_row2 |= ctrl << 2;
    rcoh_row2 |= tpid & 0x3;

    rcoh  = rcoh_row1 << 16;
    rcoh |= rcoh_row2 << 8;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
    {
      if (TRUE == ts[i]) {

        if(rp == 1)
        {
            if (PMC_SUCCESS == result) {
              PMC_LOG_TRACE("ohfs_insert_rcoh_set(%u,%08X) \n", i, rcoh);
              result = ohfs_insert_rcoh_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                            i,
                                            rcoh);
            }

            if (PMC_SUCCESS == result) {
              if (TRUE == rp_flag) {
                PMC_LOG_TRACE("ohfs_insert_field_RP_set(%u,%u) \n", i, rp);
                ohfs_insert_rp_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                   i,
                                   rp);
              }
            }
        }
        else
        {

            if (PMC_SUCCESS == result) {
              if (TRUE == rp_flag) {
                PMC_LOG_TRACE("ohfs_insert_field_RP_set(%u,%u) \n", i, rp);
                ohfs_insert_rp_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                   i,
                                   rp);
              }
            }

            if (PMC_SUCCESS == result) {
              PMC_LOG_TRACE("ohfs_insert_rcoh_set(%u,%08X) \n", i, rcoh);
              result = ohfs_insert_rcoh_set(odtu_mux_handle->tgmp_handle->ohfs_insert_handle,
                                            i,
                                            rcoh);
            }


        }
      };
    }

    
    PMC_RETURN(result);
} /* odtu_mux_ghao_set_rcoh */  

/*******************************************************************************
*  odtu_mux_ghao_get_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get G.HAO LCR and BWR states, channel ID and TS information for currently
*   resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*
* OUTPUTS:
*   lcr_state              - LCR state
*   bwr_state              - BWR state
*   ho_chnl_id             - High Order channel ID
*   lo_chnl_id             - Low Order channel ID
*   tpid                   - Tributary slot ID
*   ts                     - Calendar slot mask of the added / removed TS
*   old_ts                 - Calendar slot mask of TS before resizing
*   new_ts                 - Calendar slot mask of TS after resizing
*   mux_stage              - Muxing stage
*   ghao_index             - Index of the G.HAO context array
*
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_get_state(odtu_mux_handle_t *odtu_mux_handle,
                                         util_global_ghao_lcr_state_t *lcr_state,
                                         util_global_ghao_bwr_state_t *bwr_state,
                                         UINT8 *ho_chnl_id,
                                         UINT8 *lo_chnl_id,
                                         UINT8 *tpid,
                                         BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 old_ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 new_ts[ODTU_MUX_NUM_CHANL],
                                         odtu_mux_stage_instance_t *mux_stage,
                                         UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 i;

    *lcr_state = odtu_mux_handle->ghao_mux_state[ghao_index].lcr_state;
    *bwr_state = odtu_mux_handle->ghao_mux_state[ghao_index].bwr_state;
    *ho_chnl_id = odtu_mux_handle->ghao_mux_state[ghao_index].ho_id;
    *lo_chnl_id = odtu_mux_handle->ghao_mux_state[ghao_index].lo_id;
    *tpid = odtu_mux_handle->ghao_mux_state[ghao_index].tpid;

    PMC_LOG_TRACE("odtu_mux_ghao_get_state: ghao_index = %u\n", ghao_index);
    PMC_LOG_TRACE("odtu_mux_ghao_get_state: lcr_state = %u\n", *lcr_state);
    PMC_LOG_TRACE("odtu_mux_ghao_get_state: bwr_state = %u\n", *bwr_state);
    PMC_LOG_TRACE("odtu_mux_ghao_get_state: ho_chnl_id = %u\n", *ho_chnl_id);
    PMC_LOG_TRACE("odtu_mux_ghao_get_state: lo_chnl_id = %u\n", *lo_chnl_id);
    PMC_LOG_TRACE("odtu_mux_ghao_get_state: tpid = %u\n", *tpid);
    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
    {
      ts[i]     = odtu_mux_handle->ghao_mux_state[ghao_index].ts[i];
      old_ts[i] = odtu_mux_handle->ghao_mux_state[ghao_index].old_ts[i];
      new_ts[i] = odtu_mux_handle->ghao_mux_state[ghao_index].new_ts[i];
      if ((i % 32) == 31) {
        PMC_LOG_TRACE("odtu_mux_ghao_get_state: ts[%u] = %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                                            i/32, ts[i-31],ts[i-30],ts[i-29],ts[i-28],ts[i-27],ts[i-26],ts[i-25],ts[i-24],
                                                  ts[i-23],ts[i-22],ts[i-21],ts[i-20],ts[i-19],ts[i-18],ts[i-17],ts[i-16],
                                                  ts[i-15],ts[i-14],ts[i-13],ts[i-12],ts[i-11],ts[i-10],ts[i-9], ts[i-8],
                                                  ts[i-7], ts[i-6], ts[i-5], ts[i-4], ts[i-3], ts[i-2], ts[i-1], ts[i]);
        PMC_LOG_TRACE("odtu_mux_ghao_get_state: old_ts[%u] = %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                                            i/32, old_ts[i-31],old_ts[i-30],old_ts[i-29],old_ts[i-28],old_ts[i-27],old_ts[i-26],old_ts[i-25],old_ts[i-24],
                                                  old_ts[i-23],old_ts[i-22],old_ts[i-21],old_ts[i-20],old_ts[i-19],old_ts[i-18],old_ts[i-17],old_ts[i-16],
                                                  old_ts[i-15],old_ts[i-14],old_ts[i-13],old_ts[i-12],old_ts[i-11],old_ts[i-10],old_ts[i-9], old_ts[i-8],
                                                  old_ts[i-7], old_ts[i-6], old_ts[i-5], old_ts[i-4], old_ts[i-3], old_ts[i-2], old_ts[i-1], old_ts[i]);
        PMC_LOG_TRACE("odtu_mux_ghao_get_state: new_ts[%u] = %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                                            i/32, new_ts[i-31],new_ts[i-30],new_ts[i-29],new_ts[i-28],new_ts[i-27],new_ts[i-26],new_ts[i-25],new_ts[i-24],
                                                  new_ts[i-23],new_ts[i-22],new_ts[i-21],new_ts[i-20],new_ts[i-19],new_ts[i-18],new_ts[i-17],new_ts[i-16],
                                                  new_ts[i-15],new_ts[i-14],new_ts[i-13],new_ts[i-12],new_ts[i-11],new_ts[i-10],new_ts[i-9], new_ts[i-8],
                                                  new_ts[i-7], new_ts[i-6], new_ts[i-5], new_ts[i-4], new_ts[i-3], new_ts[i-2], new_ts[i-1], new_ts[i]);
      };
    }

    *mux_stage = odtu_mux_handle->ghao_mux_state[ghao_index].mux_stage;
    PMC_LOG_TRACE("odtu_mux_ghao_get_state: mux_stage = %u\n", *mux_stage);

    
    PMC_RETURN(result);
} /* odtu_mux_ghao_get_state */  

/*******************************************************************************
*  odtu_mux_ghao_set_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set G.HAO LCR and BWR states, channel ID and TS information for currently
*   resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   lcr_state              - LCR state
*   bwr_state              - BWR state
*   ho_chnl_id             - High Order channel ID
*   lo_chnl_id             - Low Order channel ID
*   tpid                   - Tributary slot ID
*   ts                     - Calendar slot mask of the added / removed TS
*   old_ts                 - Calendar slot mask of TS before resizing
*   new_ts                 - Calendar slot mask of TS after resizing
*   mux_stage              - Muxing stage
*   ghao_index             - Index of the G.HAO context array
*
*
* OUTPUTS:
*
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_set_state(odtu_mux_handle_t *odtu_mux_handle,
                                         util_global_ghao_lcr_state_t lcr_state,
                                         util_global_ghao_bwr_state_t bwr_state,
                                         UINT8 ho_chnl_id,
                                         UINT8 lo_chnl_id,
                                         UINT8 tpid,
                                         BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 old_ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 new_ts[ODTU_MUX_NUM_CHANL],
                                         odtu_mux_stage_instance_t mux_stage,
                                         UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 i;
                                             
    PMC_LOG_TRACE("odtu_mux_ghao_set_state: ghao_index = %u\n", ghao_index);
    PMC_LOG_TRACE("odtu_mux_ghao_set_state: lcr_state = %u\n", lcr_state);
    PMC_LOG_TRACE("odtu_mux_ghao_set_state: bwr_state = %u\n", bwr_state);
    PMC_LOG_TRACE("odtu_mux_ghao_set_state: ho_chnl_id = %u\n", ho_chnl_id);
    PMC_LOG_TRACE("odtu_mux_ghao_set_state: lo_chnl_id = %u\n", lo_chnl_id);
    PMC_LOG_TRACE("odtu_mux_ghao_set_state: tpid = %u\n", tpid);
    odtu_mux_handle->ghao_mux_state[ghao_index].lcr_state = lcr_state;
    odtu_mux_handle->ghao_mux_state[ghao_index].bwr_state = bwr_state;
    odtu_mux_handle->ghao_mux_state[ghao_index].ho_id = ho_chnl_id;
    odtu_mux_handle->ghao_mux_state[ghao_index].lo_id = lo_chnl_id;
    odtu_mux_handle->ghao_mux_state[ghao_index].tpid = tpid;
    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++)
    {
      if ((i % 32) == 31) {
        PMC_LOG_TRACE("odtu_mux_ghao_set_state: ts[%u] = %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                                            i/32, ts[i-31],ts[i-30],ts[i-29],ts[i-28],ts[i-27],ts[i-26],ts[i-25],ts[i-24],
                                                  ts[i-23],ts[i-22],ts[i-21],ts[i-20],ts[i-19],ts[i-18],ts[i-17],ts[i-16],
                                                  ts[i-15],ts[i-14],ts[i-13],ts[i-12],ts[i-11],ts[i-10],ts[i-9], ts[i-8],
                                                  ts[i-7], ts[i-6], ts[i-5], ts[i-4], ts[i-3], ts[i-2], ts[i-1], ts[i]);
        PMC_LOG_TRACE("odtu_mux_ghao_set_state: old_ts[%u] = %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                                            i/32, old_ts[i-31],old_ts[i-30],old_ts[i-29],old_ts[i-28],old_ts[i-27],old_ts[i-26],old_ts[i-25],old_ts[i-24],
                                                  old_ts[i-23],old_ts[i-22],old_ts[i-21],old_ts[i-20],old_ts[i-19],old_ts[i-18],old_ts[i-17],old_ts[i-16],
                                                  old_ts[i-15],old_ts[i-14],old_ts[i-13],old_ts[i-12],old_ts[i-11],old_ts[i-10],old_ts[i-9], old_ts[i-8],
                                                  old_ts[i-7], old_ts[i-6], old_ts[i-5], old_ts[i-4], old_ts[i-3], old_ts[i-2], old_ts[i-1], old_ts[i]);
        PMC_LOG_TRACE("odtu_mux_ghao_set_state: new_ts[%u] = %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
                                            i/32, new_ts[i-31],new_ts[i-30],new_ts[i-29],new_ts[i-28],new_ts[i-27],new_ts[i-26],new_ts[i-25],new_ts[i-24],
                                                  new_ts[i-23],new_ts[i-22],new_ts[i-21],new_ts[i-20],new_ts[i-19],new_ts[i-18],new_ts[i-17],new_ts[i-16],
                                                  new_ts[i-15],new_ts[i-14],new_ts[i-13],new_ts[i-12],new_ts[i-11],new_ts[i-10],new_ts[i-9], new_ts[i-8],
                                                  new_ts[i-7], new_ts[i-6], new_ts[i-5], new_ts[i-4], new_ts[i-3], new_ts[i-2], new_ts[i-1], new_ts[i]);
      };
      odtu_mux_handle->ghao_mux_state[ghao_index].ts[i]     = ts[i];
      odtu_mux_handle->ghao_mux_state[ghao_index].old_ts[i] = old_ts[i];
      odtu_mux_handle->ghao_mux_state[ghao_index].new_ts[i] = new_ts[i];
    }

    PMC_LOG_TRACE("odtu_mux_ghao_set_state: mux_stage = %u\n", mux_stage);
    odtu_mux_handle->ghao_mux_state[ghao_index].mux_stage = mux_stage;
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_set_state */  


/*******************************************************************************
*  odtu_mux_ghao_get_ts_num
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the number of added / removed TS in currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*   ts_num                 - Number of added / removed TS 
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_get_ts_num(odtu_mux_handle_t *odtu_mux_handle,
                                          UINT8 *ts_num,
                                          UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 i;
                                             
    *ts_num = 0;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {
      *ts_num += odtu_mux_handle->ghao_mux_state[ghao_index].ts[i];
    }
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_get_ts_num */  


/*******************************************************************************
*  odtu_mux_ghao_get_old_ts_num
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the number of TS before resizing in currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*   old_ts_num             - Number of TS before resizing
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_get_old_ts_num(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 *old_ts_num,
                                              UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 i;
                                             
    *old_ts_num = 0;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {
      *old_ts_num += odtu_mux_handle->ghao_mux_state[ghao_index].old_ts[i];
    }
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_get_old_ts_num */  


/*******************************************************************************
*  odtu_mux_ghao_get_new_ts_num
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the number TS after resizing in currently resized LO channel
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*   new_ts_num             - Number TS after resizing
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_get_new_ts_num(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 *new_ts_num,
                                              UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 i;
                                             
    *new_ts_num = 0;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {
      *new_ts_num += odtu_mux_handle->ghao_mux_state[ghao_index].new_ts[i];
    }
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_get_new_ts_num */  


/*******************************************************************************
*  odtu_mux_ghao_get_new_ts_last_cycle
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the index of the last TS after LO channel resize
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   ghao_index             - Index of the G.HAO context array
*
* OUTPUTS:
*   new_ts_last_idx        - Calendar slot of last TS after resize
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_get_new_ts_last_cycle(odtu_mux_handle_t *odtu_mux_handle,
                                                     UINT8 *new_ts_last_idx,
                                                     UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 i;
                                             
    *new_ts_last_idx = 255;

    for (i = 0; i < ODTU_MUX_NUM_CHANL; i++) {
      if (odtu_mux_handle->ghao_mux_state[ghao_index].new_ts[i]) {
        *new_ts_last_idx = i;
      };
    }
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_get_new_ts_last_cycle */  


/*******************************************************************************
*  odtu_mux_ghao_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a LO channel needs to be removed from a HO
*   channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_mux_handle    - pointer to ODTU_DMX handle instance
*   mux_stage           - enum for demux instace. See odtu_mux_stage_instance_t.
*   lo_chnl_id          - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO channel ID that the LO channel belongs to
*                         Valid range: 0 - 95
*   ghao_inc            - TRUE : ghao increase
*                         FALSE : ghao decrease
*   num_old_ts          - if ghao_inc == TRUE, the old number of tribslots
*                         must be provided so that it can decrement 
*                         num_active_ts properly.
*
* OUTPUTS:
*   old_highest_cs      - the highest cal slots of LO channel before HAO resizing
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_lo_chnl_deprov(odtu_mux_handle_t  *odtu_mux_handle,
                                              odtu_mux_stage_instance_t mux_stage,
                                              UINT32 lo_chnl_id, UINT32 ho_chnl_id,
                                              BOOL ghao_inc,
                                              UINT32 num_old_ts,
                                              UINT8 *old_highest_cs)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 cycle = 0;
    UINT32 ts_id;
    UINT32 num_tribslot;

    UINT32 i;
    UINT32 loc = 0;

    UINT32 old_h_cs = 0;
    
    /*UINT32 cal_entry;*/
    line_core_otn_db_key    key;
    UINT32          num_of_recs;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(odtu_mux_handle!=NULL, ODTU_MUX_ERR_INVALID_PTR, 0 , 0);

    num_tribslot = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;
   
    PMC_LOG_TRACE("%s, %s, %d, num_active_ts in HO = %u\n", __FILE__, __FUNCTION__, __LINE__, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);

    if (mux_stage != ODTU_MUX_STG_TWO)
    {
        /* mo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = lo_chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
    }
    else {
        /* lo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = lo_chnl_id;
            key.trib_slot = DO_NOT_USE_KEY;
        
    }

    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_mux_handle->base),
                               odtu_mux_handle->cfg.db_id,
                               odtu_mux_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);
    
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND);
    }
    temp_result = *q_result;

    for (ts_id = 0; ts_id < num_of_recs; ts_id++)
    {
        cycle = temp_result.p_recs[ts_id]->calendar_entry;
        if ( cycle > old_h_cs )
        {
            old_h_cs = cycle;
        }
                
        if (PMC_SUCCESS == result)
        {
            result = expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                cycle, 0);
        } 
        if (PMC_SUCCESS == result)
        {
            result = expand_ibuf_lo_eomf_data_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                                          cycle, 0);
        }                                                                                                                                                                                
        for (i = 0; i < odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot; i++)
        {
           if (odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i] == cycle)
           {
               loc = i;
               break;
           }
        }
    
        /* Update DB */
        if (mux_stage == ODTU_MUX_STG_ONE)
          {
            if ( ODTU_MUX_DB_ENTRY_UNKNOWN != (temp_result.p_recs[ts_id]->mo_channel) )
              {
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[ts_id] = 0;
                
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[ts_id] = 0;
                
                odtu_mux_lo_chnl_var_update(odtu_mux_handle, lo_chnl_id, ho_chnl_id, 
                                            ODTU_MUX_CHNL_START, 
                                            UTIL_GLOBAL_ODUK_DONT_CARE, 
                                            UTIL_GLOBAL_ODUK_DONT_CARE, 
                                            0, 
                                            LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 
                                            UTIL_GLOBAL_NO_MAP);   
                
                PMC_LOG_TRACE("%s, %s, %d, num_active_ts in HO = %u\n", __FILE__, __FUNCTION__, __LINE__, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
                odtu_mux_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] = 0x7F;

                for (i = 0; i < 96; i++)
                {
                    PMC_LOG_TRACE("sts_sw_handle->var.ho_chnl_cfg[%u].lo_chnl_ids_ptr[%u] = %u\n", ho_chnl_id, i, odtu_mux_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i]);
                }


              }
          }   
        else if (mux_stage == ODTU_MUX_STG_TWO)
          {
            if ( ODTU_MUX_DB_ENTRY_UNKNOWN != (temp_result.p_recs[ts_id]->lo_channel) )
              {
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[ts_id] = 0;
                
                odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[ts_id] = 0;
                
                odtu_mux_lo_chnl_var_update(odtu_mux_handle, lo_chnl_id, ho_chnl_id, 
                                            ODTU_MUX_CHNL_START, 
                                            UTIL_GLOBAL_ODUK_DONT_CARE, 
                                            UTIL_GLOBAL_ODUK_DONT_CARE, 
                                            0, 
                                            LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 
                                            UTIL_GLOBAL_NO_MAP);   
                
                PMC_LOG_TRACE("%s, %s, %d, num_active_ts in HO = %u\n", __FILE__, __FUNCTION__, __LINE__, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
                odtu_mux_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[loc] = 0x7F;

                for (i = 0; i < 96; i++)
                {
                    PMC_LOG_TRACE("sts_sw_handle->var.ho_chnl_cfg[%u].lo_chnl_ids_ptr[%u] = %u\n", ho_chnl_id, i, odtu_mux_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i]);
                }


              }
          }    
    }
  
    PMC_LOG_TRACE("odtu_mux_ghao_lo_chnl_deprov: old var.ho_chnl_cfg[%u].num_active_ts = %u\n",
               ho_chnl_id, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);

     if(ghao_inc)
     {
        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts - num_old_ts;
     }
     else
     {
        odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts - num_of_recs;
     }


    PMC_LOG_TRACE("odtu_mux_ghao_lo_chnl_deprov: new var.ho_chnl_cfg[%u].num_active_ts = %u\n",
               ho_chnl_id, odtu_mux_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);


    PMC_LOG_TRACE("odtu_mux_ghao_lo_chnl_deprov: old_h_cs = %u\n", old_h_cs);
    *old_highest_cs = old_h_cs;
    
    PMC_RETURN(result);
} /* odtu_mux_ghao_lo_chnl_deprov */


/*******************************************************************************
*  odtu_mux_ghao_adjust_exp_xoff_lvl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Adjust EXPAND_IBUF currently resized LO channel
*  
*   API is similar to odtu_mux_ghao_adjust_xoff_lvl but doesn't
*   perform a lookup in the MUX's g.hao context
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   lo_chnl_id             - lo channel id
*   xoff_lvl               - New CFC FIFOs XOFF level
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_adjust_exp_xoff_lvl(odtu_mux_handle_t *odtu_mux_handle,
                                                   UINT32 lo_chnl_id,
                                                   UINT8 xoff_lvl)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

  
    result = expand_ibuf_cfc_fill_lvl_set(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                          lo_chnl_id,
                                          xoff_lvl);

    PMC_RETURN(result);
} /* odtu_mux_ghao_adjust_exp_xoff_lvl */  

/*******************************************************************************
*  odtu_mux_ghao_passthru_cal_enable_diable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is used for enabling/disabling the calendar entries in the
*   passthru MUX.
*
* INPUTS:
*   odtu_mux_handle        - handle to the ODTU_MUX instance to be operated on
*   cycle                  - calendar entry to enable/disable
*   enable                 - TRUE : enable the calendar entry
*                            FALSE : disable the calendar entry
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   - PMC_ERROR if
*       * Any function called don't return PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_mux_ghao_passthru_cal_enable_diable(odtu_mux_handle_t *odtu_mux_handle,
                                                          UINT32 cycle,
                                                          BOOL8 enable)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

  
    result = expand_ibuf_lo_cycle_cfg(odtu_mux_handle->tgmp_handle->expand_ibuf_handle,
                                      cycle,
                                      enable);                                          

    PMC_RETURN(result);
} /* odtu_mux_ghao_passthru_cal_enable_diable */  







/*
** End of file
*/
