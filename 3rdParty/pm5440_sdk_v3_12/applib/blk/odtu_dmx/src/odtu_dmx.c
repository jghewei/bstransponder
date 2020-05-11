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
#include "odtu_dmx_loc.h"

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
const char ODTU_DMX_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODTU_DMX_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODTU_DMX_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void odtu_dmx_dummy_schd_clean(odtu_dmx_handle_t *odtu_dmx_handle);
PRIVATE PMC_ERROR odtu_dmx_ctxt_audit(odtu_dmx_handle_t *odtu_dmx_handle,
                                      util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                      odtu_dmx_recover_var_t *odtu_dmx_var_reg,                                                  
                                      odtu_dmx_recover_var_t *prev_stg_dmx_var_reg,
                                      odtu_dmx_var_t *odtu_dmx_var_clean);
PRIVATE PMC_ERROR odtu_dmx_ctxt_reg_recover(odtu_dmx_handle_t *odtu_dmx_handle,
                                            pmc_energy_state_t top_energy_state_reg,                                         
                                            odtu_dmx_recover_var_t *odtu_dmx_var_reg);
PRIVATE void odtu_dmx_var_default_init(odtu_dmx_handle_t *odtu_dmx_handle, odtu_dmx_var_t *odtu_dmx_var);
PRIVATE void odtu_dmx_ho_chnl_var_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                         UINT32 ho_chnl_id,
                                         odtu_dmx_chnl_state_t chnl_state,
                                         util_global_odukp_type_t ho_odu_type,
                                         UINT32 num_tribslot, UINT8 *cal_array,
                                         UINT32 *lo_chnl_ids,
                                         util_global_odu_line_payload_t ts_type,
                                         UINT32 num_active_ts);
PRIVATE void odtu_dmx_lo_chnl_var_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                         UINT32 lo_chnl_id, UINT32 ho_chnl_id,
                                         odtu_dmx_chnl_state_t chnl_state,
                                         util_global_odukp_type_t lo_oduk_type,
                                         util_global_odukp_type_t ho_oduk_type,
                                         UINT32 num_ts, UINT8 *cal_array,
                                         util_global_odu_line_payload_t ts_type,
                                         util_global_mapping_mode_t mapping_mode,
                                         UINT8 ts_port);

PRIVATE UINT32 odtu_dmx_get_free_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                               UINT32 lo_chnl_id);
PRIVATE UINT32 odtu_dmx_get_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                           UINT32 lo_chnl_id);
PRIVATE void odtu_dmx_deprov_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                               UINT32 lo_chnl_id);
PRIVATE void odtu_dmx_deprov_first_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 lo_chnl_id);

PRIVATE UINT32 odtu_dmx_get_offline_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 lo_chnl_id, UINT32 frm_num);
PRIVATE PMC_ERROR odtu_dmx_dummy_schd_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                          UINT32 dummy_schd_chnl,
                                          UINT32 mo_chnl_id,
                                          UINT32 calendar_entry, BOOL8 enable);

PRIVATE void odtu_dmx_tribslot_var_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                          UINT32 ts_id, UINT32 ho_chnl_id,
                                          BOOL8 ts_occupied);

/*
** Public Functions
*/
/*******************************************************************************
* odtu_dmx_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ODTU_DMX block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the ODTU_DMX subsystem relative to
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier for the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   odtu_dmx_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odtu_dmx_handle_t *odtu_dmx_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    odtu_dmx_handle_t *odtu_dmx_handle;

    PMC_ENTRY();
    
    odtu_dmx_handle = (odtu_dmx_handle_t*)PMC_CTXT_CALLOC(sizeof(odtu_dmx_handle_t), parent);

    pmc_handle_init(parent, odtu_dmx_handle, sys_handle, PMC_MID_DIGI_ODTU_DMX, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(ODTU_DMX_LOG_ERR_STRINGS[0], ODTU_DMX_LOG_ERR_TABLE_BASE, ODTU_DMX_LOG_ERR_COUNT);

    /* create handles for all children of ODTU_MUX */
    odtu_dmx_handle->rgmp_handle = rgmp_ctxt_create((pmc_handle_t *)odtu_dmx_handle, base_address+BASE_ADDR_RGMP96_MTSB, sys_handle, "RGMP");
    PMC_CTXT_REGISTER_PTR(&odtu_dmx_handle->rgmp_handle, parent);
    odtu_dmx_handle->sts_sw_handle = sts_sw_ctxt_create((pmc_handle_t *)odtu_dmx_handle, base_address+BASE_ADDR_ODTU_DMX_STS_SW, sys_handle, "STS_SW");
    PMC_CTXT_REGISTER_PTR(&odtu_dmx_handle->sts_sw_handle, parent);


    odtu_dmx_handle->ghao_dmx_state = (ghao_dmx_var_t*) PMC_CTXT_CALLOC( ODTU_DMX_GHAO_NUM_CTXT * sizeof(ghao_dmx_var_t), odtu_dmx_handle);
    PMC_CTXT_REGISTER_PTR(&odtu_dmx_handle->ghao_dmx_state, odtu_dmx_handle);

    PMC_RETURN(odtu_dmx_handle);
} /* odtu_dmx_ctxt_create */

/*******************************************************************************
* odtu_dmx_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ODTU_DMX block instance.
*
*
* INPUTS:
*   *odtu_dmx_handle         - pointer to ODTU_DMX handle instance
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
PUBLIC void odtu_dmx_ctxt_destroy(odtu_dmx_handle_t *odtu_dmx_handle)
{
    PMC_ENTRY();

    PMC_LOG_TRACE("odtu_dmx_handle->cfg.db_id = %u\n", odtu_dmx_handle->cfg.db_id);
    PMC_LOG_TRACE("odtu_dmx_handle->cfg.db_handle = %u\n", odtu_dmx_handle->cfg.db_handle);

    gen_db_detach(&(odtu_dmx_handle->base), odtu_dmx_handle->cfg.db_id,
                  odtu_dmx_handle->cfg.db_handle);

    /* destory handles for all children in ODTU_DMX */
    rgmp_ctxt_destroy(odtu_dmx_handle->rgmp_handle);
    sts_sw_ctxt_destroy(odtu_dmx_handle->sts_sw_handle);

    PMC_CTXT_FREE(&odtu_dmx_handle->ghao_dmx_state, odtu_dmx_handle);

    PMC_CTXT_FREE(&odtu_dmx_handle, odtu_dmx_handle);
    
    PMC_RETURN();
} /* odtu_dmx_ctxt_destroy */


/*******************************************************************************
* odtu_dmx_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODTU_DMX block handle instance.
*
*
* INPUTS:
*   *odtu_dmx_handle         - pointer to ODTU_DMX handle instance
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
PUBLIC void odtu_dmx_handle_init(odtu_dmx_handle_t *odtu_dmx_handle)
{
    PMC_ENTRY();

    /* Update static configurations */
    odtu_dmx_handle->cfg.max_chnl_num     = ODTU_DMX_NUM_CHANL;
    odtu_dmx_handle->cfg.rgmp_instance    = INSTANCE_DEMUX;

    PMC_LOG_TRACE("ODTU_DMX_MAX_CHNL_NUM set to %u\n", odtu_dmx_handle->cfg.max_chnl_num);

     /* initialize var_t struct */
    odtu_dmx_var_default_init(odtu_dmx_handle,&odtu_dmx_handle->var);
    
    rgmp_handle_init(odtu_dmx_handle->rgmp_handle, INSTANCE_DEMUX);
    sts_sw_handle_init(odtu_dmx_handle->sts_sw_handle, INSTANCE_DEMUX_STS_SW);

    /* attach to the databse */
    odtu_dmx_handle->cfg.db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;

    odtu_dmx_handle->cfg.db_handle = gen_db_attach(&(odtu_dmx_handle->base), odtu_dmx_handle->cfg.db_id);

    PMC_RETURN();
} /* odtu_dmx_handle_init */

/*******************************************************************************
* odtu_dmx_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ODTU_DMX block instance.
*
*
* INPUTS:
*   *odtu_dmx_handle         - pointer to ODTU_DMX handle instance.
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
PUBLIC UINT32 odtu_dmx_base_address_get(odtu_dmx_handle_t *odtu_dmx_handle)
{
    PMC_ENTRY();
    PMC_RETURN(odtu_dmx_handle->rgmp_handle->base.base_address);
} /* odtu_dmx_base_address_get */

/*******************************************************************************
* odtu_dmx_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on odtu_dmx handle instance.
*
* INPUTS:
*   *odtu_dmx_handle            - pointer to mapotn handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   dmx_stage                   - the top demux stage
*   top_energy_state_reg        - The expected energy state ,
*  *prev_stg_dmx_var_reg        - The recovery context from the previous stg\n
*                                 It may be NULL
*   *odtu_dmx_var_clean         - A clean context which represents the intersection
*                                 between both context is also generated.
*   *odtu_dmx_var_reg           - The recovery context 
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*   ODTU_DMX_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT:
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
PUBLIC PMC_ERROR odtu_dmx_handle_restart_init(odtu_dmx_handle_t *odtu_dmx_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              odtu_dmx_stage_instance_t dmx_stage,
                                              pmc_energy_state_t top_energy_state_reg,
                                              odtu_dmx_recover_var_t *prev_stg_dmx_var_reg,
                                              odtu_dmx_var_t *odtu_dmx_var_clean,
                                              odtu_dmx_recover_var_t *odtu_dmx_var_reg)
{    
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
       
    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(ODTU_DMX_LOG_ERR_STRINGS[0], ODTU_DMX_LOG_ERR_TABLE_BASE, ODTU_DMX_LOG_ERR_COUNT);
    }

    PMC_MEMSET(odtu_dmx_var_reg,0,sizeof(odtu_dmx_recover_var_t));
    odtu_dmx_var_reg->dmx_stage = dmx_stage;

    rc = rgmp_handle_restart_init(odtu_dmx_handle->rgmp_handle, restart_init_cfg_ptr, top_energy_state_reg);
    
    if (PMC_SUCCESS == rc)
    {
        rc = sts_sw_handle_restart_init(odtu_dmx_handle->sts_sw_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    /* 
       Recover context from register space        
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only OPERATIONAL channel configruation are recovered
       - anyway a partial channel config is an error in warm restart and will be cleanup in crash-restarted
       - incoherency in register context means that the subsystem shall be shutdown
    */
    if (PMC_SUCCESS == rc)
    {
        rc = odtu_dmx_ctxt_reg_recover(odtu_dmx_handle,                                      
                                       top_energy_state_reg,
                                       odtu_dmx_var_reg);
    }
    
    if (PMC_SUCCESS == rc)
    {        
        /* 
         * Audit register context and memory context 
         *   - Audit power and decide if we should just shut down the subsystem 
         *   - Create a new context that represents the intersection between memory and register space (odtu_dmx_var_clean)
         */
        rc = odtu_dmx_ctxt_audit(odtu_dmx_handle, 
                                 restart_init_cfg_ptr,
                                 odtu_dmx_var_reg,
                                 prev_stg_dmx_var_reg,
                                 odtu_dmx_var_clean); 
                
    }

    PMC_RETURN(rc);
} /* odtu_dmx_handle_restart_init */

/*******************************************************************************
* odtu_dmx_lo_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provides a method to retrieve a set of LO channels for a
*    specied HO channel
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to mapotn handle instance
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
PUBLIC void odtu_dmx_lo_chnl_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                      UINT32 ho_chnl_id,
                                      UINT32 *ch_ptr,
                                      UINT32 *num_ch)
{    
    UINT32 lo_chnl_itr = 0;
    PMC_ENTRY();
    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    *num_ch = 0;
    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_DMX_NUM_CHANL; lo_chnl_itr ++)
    {
        if (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].state == UTIL_GLOBAL_CHNL_OPERATIONAL && 
            odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id == ho_chnl_id)
        {
            ch_ptr[*num_ch] = lo_chnl_itr;
            *num_ch += 1;
        }
    }

    PMC_RETURN();
} /* odtu_dmx_lo_chnl_get */


/*******************************************************************************
* Initialization Configuration Functions
*******************************************************************************/
/*******************************************************************************
* odtu_dmx_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the switch in ODTU_DMX.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance to be operated on
*   enbl                - Enable or disable ODTU_DMX \n
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
PUBLIC PMC_ERROR odtu_dmx_init(odtu_dmx_handle_t *odtu_dmx_handle, UINT32 enbl)
{
    PMC_ENTRY();    

     /* initialize B page */
    odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 1);
    odtu_dmx_field_FORCE_CONFIG_ADDR_set(NULL, odtu_dmx_handle, 1);
    
    /* initialize ram based configuration in OHFS_REMOVE */
    ohfs_remove_ram_init(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle);

    /* initialize ram based configuration in RXJC_PROC */
    rxjc_proc_ram_init(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle);
    
    /* initialize ram based configuration in CPACT_OBUF */
    cpact_obuf_ram_init(odtu_dmx_handle->rgmp_handle->cpact_obuf_handle);
    
    /* initialize ram based configuration in MP_MGEN */
    mp_mgen_ram_init(odtu_dmx_handle->rgmp_handle->mp_mgen_handle);
    
    /* initialize ram based configuration in STS_SW */
    sts_sw_ram_init(odtu_dmx_handle->sts_sw_handle);

    /* initialize B page */
    /*odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 1);*/
    odtu_dmx_field_FORCE_CONFIG_ADDR_set(NULL, odtu_dmx_handle, 0);
    
    /* initialize ram based configuration in OHFS_REMOVE */
    ohfs_remove_ram_init(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle);
    
    /* initialize ram based configuration in RXJC_PROC */
    rxjc_proc_ram_init(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle);
    
    /* initialize ram based configuration in CPACT_OBUF */
    cpact_obuf_ram_init(odtu_dmx_handle->rgmp_handle->cpact_obuf_handle);
    
    /* initialize ram based configuration in MP_MGEN */
    mp_mgen_ram_init(odtu_dmx_handle->rgmp_handle->mp_mgen_handle);
    
    /* initialize ram based configuration in STS_SW */
    sts_sw_ram_init(odtu_dmx_handle->sts_sw_handle);

    /* undo force config */
    odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 0);
    
    /* initializes switch to demux mode */
    
    /* MXDX_MODE_EN = 1 to propagate dMSIM to SSF */
    sts_sw_init(odtu_dmx_handle->sts_sw_handle, 1, enbl);
    
    /* MXDX_MODE_EN = 0 to disable SSF 
    sts_sw_init(odtu_dmx_handle->sts_sw_handle, 0, enbl);*/

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_init */

/*******************************************************************************
*  odtu_dmx_ho_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures reset mode for an ODU channel in ODTU Demux.
*   When a HO channel is put into reset, all LO channels associated with the HO
*   channel must be put into reset.
*
* INPUTS:
*   *odtu_dmx_handle        - pointer to ODTU_DMX handle instance
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
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 chnl_state)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("%s, %s, %d, chnl_state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_state);
    odtu_dmx_field_HO_CHNL_RSTB_set(NULL, odtu_dmx_handle, chnl_id, chnl_state);

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_ho_chnl_reset_cfg */

/*******************************************************************************
*  odtu_dmx_lo_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures reset mode for a LO ODU channel in ODTU Demux.
*
* INPUTS:
*   *odtu_dmx_handle        - pointer to ODTU_DMX handle instance
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
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_reset_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 chnl_state)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("%s, %s, %d, chnl_state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_state);
    odtu_dmx_field_LO_CHNL_RSTB_set (NULL, odtu_dmx_handle, chnl_id, chnl_state);

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_lo_chnl_reset_cfg */


/*******************************************************************************
*  odtu_dmx_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the operational mode of an HO channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - channel ID to be configured (HO or MO) \n
*                         Valid range: 0 - 95
*   mode                - operational mode of the LO ODU channel \n
*                         0 -- unchannelized \n
*                         1 -- channelized operational \n
*                         2 -- non-intrusive monitor
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_chnl_init(odtu_dmx_handle_t *odtu_dmx_handle,
                                    UINT32 chnl_id,
                                    odtu_dmx_chnl_mode_t mode)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE("%s, %s, %d, mode = %u\n", __FILE__, __FUNCTION__, __LINE__, mode);
    
    /* if unchannelized, set the channel in PASSTHRU */
    if (mode == ODTU_DMX_CHNL_UNCHANNELIZED)
    {
        odtu_dmx_field_PASSTHRU_set(NULL, odtu_dmx_handle, chnl_id, TRUE);
        odtu_dmx_field_NON_INTRUSVE_MODE_set(NULL, odtu_dmx_handle, chnl_id, FALSE);
        
        result = ohfs_remove_func_mode_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                           chnl_id, 0);
    }
    else if (mode == ODTU_DMX_CHNL_CHANNELIZED)
    {
        /* normal operation */
        odtu_dmx_field_PASSTHRU_set(NULL, odtu_dmx_handle, chnl_id, FALSE);
        odtu_dmx_field_NON_INTRUSVE_MODE_set(NULL, odtu_dmx_handle, chnl_id, FALSE);
        
        result = ohfs_remove_func_mode_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                           chnl_id, 0);
    }
    else if (mode == ODTU_DMX_CHNL_NON_INTRUSIVE)
    {
        /* non_intrusive */
        odtu_dmx_field_PASSTHRU_set(NULL, odtu_dmx_handle, chnl_id, FALSE);
        odtu_dmx_field_NON_INTRUSVE_MODE_set(NULL, odtu_dmx_handle, chnl_id, TRUE);
        
        result = ohfs_remove_func_mode_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                           chnl_id, 3);
    }
    else
    {
        /* null frame mode */
        odtu_dmx_field_PASSTHRU_set(NULL, odtu_dmx_handle, chnl_id, FALSE);
        odtu_dmx_field_NON_INTRUSVE_MODE_set(NULL, odtu_dmx_handle, chnl_id, FALSE);         
        
        result = ohfs_remove_func_mode_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                           chnl_id, 1);
    }
    
    PMC_RETURN(result);
} /* odtu_dmx_chnl_init */

/*******************************************************************************
* odtu_dmx_lo_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_dmx_dmp_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO ODU channel ID that the LO channel belongs to
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
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_deactivate(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id, UINT32 ho_chnl_id)
{
    /* variable declaration */
    PMC_ENTRY();

    /* check the current state of the channel */
    if (((UINT32)odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state != (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
        && (UINT32)(odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state != (UINT32)ODTU_DMX_CHNL_START))
    {
        PMC_RETURN(ODTU_DMX_CHNL_STATE_INVALID); /* that channel is is not in activated state or start state */
    }
    
    /* Channel reset is released by configuring HO_CH_RSTB to '0' */
    odtu_dmx_lo_chnl_reset_cfg(odtu_dmx_handle, chnl_id, FALSE);
     
    if ((UINT32)odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
    {
        /* Update var context for a provisioned channel */
        odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_EQUIPPED;
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* odtu_dmx_lo_chnl_deactivate */

/*******************************************************************************
* odtu_dmx_lo_chnl_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_dmx_dmp_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO ODU channel ID that the LO belongs to
*                         Valid range: 0 - 95
*   mux_stage           - enum for mux stage instance. See odtu_dmx_stage_instance_t.
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
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_activate(odtu_dmx_handle_t *odtu_dmx_handle,
                                           UINT32 chnl_id, 
                                           UINT32 ho_chnl_id,
                                           odtu_dmx_stage_instance_t mux_stage)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;     

    PMC_ENTRY();

    if ((UINT32)odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state != (UINT32)ODTU_DMX_CHNL_EQUIPPED
        && (UINT32)odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state != (UINT32)ODTU_DMX_CHNL_START)
    {
        PMC_RETURN(ODTU_DMX_CHNL_STATE_NOT_EQUIPPED);  /*that channel is not in provisioned state*/
    }

    /* Channel reset is released by configuring HO_CH_RSTB to '1' */
    odtu_dmx_lo_chnl_reset_cfg(odtu_dmx_handle, chnl_id, TRUE);

           
    /* Update var context for a provisioned channel */
    if ((UINT32)odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state == (UINT32)ODTU_DMX_CHNL_EQUIPPED)
    {  
        odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_OPERATIONAL;
    }
    PMC_RETURN(result);

} /* odtu_dmx_lo_chnl_activate */


/*******************************************************************************
*  odtu_dmx_reconfig_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the configuration mode for a HO ODU channel in
*   ODTU Demux. This function is used to specify whether the configuration
*   update is due to Hitless Adjustment of ODUFlex or a channel addition/removal.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - channel ID to be configured
*                         Valid range: 0 - 95
*   config_mode         - configuration mode of the HO ODU channel \n
*                         0 -- Configuration update is due to a channel addition
*                              or removal. \n
*                         1 -- Configuration update is due to a HAO event.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_reconfig_mode_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id,
                                            UINT32 config_mode)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("%s, %s, %d, config_mode = %u\n", __FILE__, __FUNCTION__, __LINE__, config_mode);
    odtu_dmx_field_CONFIG_MODE_set(NULL, odtu_dmx_handle, chnl_id, config_mode);

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_reconfig_mode_cfg */

/*******************************************************************************
*  odtu_dmx_reconfig_status_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates the status of SW completion on applying a new set of
*   configurations. Configuration update status needs to be updated after adding
*   or removing a HO/LO channel or HAO operation. Once new configurations are
*   accepted, configuration update status will be reset.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - channel ID to be configured
*                         Valid range: 0 - 95
*   update              - indicates if SW has completed a new set of
*                         configurations. \n
*                         0 -- SW has not completed new configurations. \n
*                         1 -- SW has completed new configurations.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_reconfig_status_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 chnl_id, UINT32 update)
{
    PMC_ENTRY();
    
    PMC_LOG_TRACE("%s, %s, %d, config_update = %u\n",   __FILE__, __FUNCTION__, __LINE__, update);
    odtu_dmx_field_CONFIG_UPDATE_set(NULL, odtu_dmx_handle, chnl_id, update);

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_reconfig_status_update */

/*******************************************************************************
*  odtu_dmx_page_config_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates the status of SW completion on applying a new set of
*   configurations. Configuration update status needs to be updated after adding
*   or removing a HO/LO channel or HAO operation. Once new configurations are
*   accepted, configuration update status will be reset.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - channel ID to be configured
*                         Valid range: 0 - 95
*   norm_cmd            - only used in G.HAO operation
*                         0 -- NORM_CMD is set
*                         1 -- NORM_CMD is cleared
*   config_mode         - configuration mode defining the action of the
*                         configuration update
*                         0 -- HAO configuration change
*                         1 -- HO/LO configuration change
*   update              - indicates if SW has completed a new set of
*                         configurations. \n
*                         0 -- SW has not completed new configurations. \n
*                         1 -- SW has completed new configurations.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_page_config_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id, 
                                             UINT32 norm_cmd,
                                             UINT32 config_mode,
                                             UINT32 update)
{
    PMC_ENTRY();
    
    odtu_dmx_buffer_t odtu_dmx_buf[1];

    odtu_dmx_buffer_init(odtu_dmx_buf, odtu_dmx_handle);
    /*odtu_dmx_field_NORM_CMD_set(odtu_dmx_buf, odtu_dmx_handle, chnl_id, norm_cmd);*/
    odtu_dmx_field_CONFIG_MODE_set(odtu_dmx_buf, odtu_dmx_handle, chnl_id, config_mode);
    odtu_dmx_field_CONFIG_UPDATE_set(odtu_dmx_buf, odtu_dmx_handle, chnl_id, update);

    odtu_dmx_buffer_flush(odtu_dmx_buf);

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_reconfig_status_update */


/*******************************************************************************
*  odtu_dmx_config_update_done_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function polls the CONFIG_UPDATE_DONE interrupt bit for the given
*   channel for a maximum of max_poll number of iterations.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - HO channel ID to be configured
*                         Valid range: 0 - 95
*   max_poll            - Maximum number of times to poll
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL If the poll times out.
*   PMC_SUCCESS If the poll succeeds.
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_config_update_done_poll(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 chnl_id, UINT32 max_poll)
{
/*    PMC_ENTRY(); */ /* Turned off tolower the logging output */
    
    PMC_ERROR result = PMC_SUCCESS;    

    result = odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll(NULL, odtu_dmx_handle, chnl_id, chnl_id, 1, PMC_POLL_EQUAL, max_poll, NULL, 1 );

    if (PMC_SUCCESS != result)
    {
#ifndef PMC_SW_SIMULATION
    
        result = ODTU_DMX_ERR_POLL_CONFIG_UPDATE_DONE_EXPIRY;
#else   
        result = PMC_SUCCESS;  
#endif /* PMC_SW_SIMULATION */              
    }  

    
    return(result);
/*    PMC_RETURN(PMC_SUCCESS);*/
} /* odtu_dmx_config_update_done_poll */

/*******************************************************************************
*  odtu_dmx_config_update_done_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clears the configuration update done interrupt status.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - HO channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_config_update_done_int_clear(odtu_dmx_handle_t *odtu_dmx_handle,
                                                       UINT32 chnl_id)
{
    PMC_ENTRY();
    
    odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear(NULL, odtu_dmx_handle, chnl_id, chnl_id, 1);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_config_update_done_int_clear */

/*******************************************************************************
*  odtu_dmx_config_update_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function polls the CONFIG_UPDATE register bit field, which indicates if
*   a page flip has been successful.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - HO channel ID to be configured
*                         Valid range: 0 - 95
*   max_poll            - Maximum number of times to poll
*
* OUTPUTS:
*
* RETURNS:
*   ODTU_DMX_ERR_POLL_CONFIG_UPDATE_EXPIRY If the polling timed out
*   PMC_SUCCESS If the poll was successful
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_config_update_poll(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 max_poll)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odtu_dmx_field_CONFIG_UPDATE_poll(NULL, odtu_dmx_handle, chnl_id, 0, PMC_POLL_EQUAL, max_poll, NULL, 1);

    if (PMC_SUCCESS != result)
    {
#ifndef PMC_SW_SIMULATION
    
        result = ODTU_DMX_ERR_POLL_CONFIG_UPDATE_EXPIRY;
#else   
        result = PMC_SUCCESS;
#endif /* PMC_SW_SIMULATION */                
    }  

    PMC_RETURN(result);
} /* odtu_dmx_config_update_poll */

/*******************************************************************************
* odtu_dmx_tribslot_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure a tributary slot occupied by a HO.
*   MSI coding for each tributary slot is done in this API.
*
*   This function is called by odtu_dmx_lo_chnl_prov() API to properly set up 
*   configurations for each tributary slot occupied by a LO channel.
*
*   As an example, when an ODU2 channel with 1.25G tributary slot is added,
*   this function is called 8 times to set up all 8 tributary slots.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   stage_inst          - multiplex stage instance
*   chnl_id             - LO ODU channel ID to be configured
*                         Valid range: 0 - 95
*   cal_slot            - calendar entry of the tributary slot.
*                         Valid range: 0 - 95
*   ts_port             - tributary port number that the tributary slot will be
*                         transported.
*   ho_chnl_id          - HO channel ID that this tributary slot belongs to.
*   client_type         - LO ODU channel rate. See util_global_odukp_type_t
*   server_type         - HO ODU channel rate. See util_global_odukp_type_t
*   ts_type             - tributary slot type. See odu_line_payload_t
*   mapping_mode        - mapping mode for the LO channel. See mapping_mode_enum
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
*   ghao_reprov         - For DMX when reprovisioning a ghao channel,
*                         no need to reprogram the mp_mgen ipt table
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
PUBLIC PMC_ERROR odtu_dmx_tribslot_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                       odtu_dmx_stage_instance_t stage_inst,
                                       UINT32 chnl_id, UINT32 cal_slot,
                                       UINT32 ts_port, UINT32 ho_chnl_id,
                                       UINT32 client_type,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 first_ts, 
                                       UINT32 last_ts,
                                       UINT32 frm_num, 
                                       BOOL8 offline_update, 
                                       BOOL8 ghao_reprov)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 num_tribslot;
    UINT32 num_active_ts;
    UINT32 valid = 1;
    UINT32 tribslot;
    UINT32  chan_map_type = 0;
    UINT32 mgen_ipt_entry;

    UINT32 i;
    line_core_otn_db_key    key;
    UINT32 num_of_recs;
    UINT32 num_of_lo_recs;
    line_core_otn_db_query_res *q_result;     
    line_core_otn_db_query_res *lo_q_result;
    BOOL8 found;
    UINT32 ipt_va = 0;
    
    PMC_ENTRY();


    /* argument checking */
    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);

    /* Retrieve the maximum number of tributary slot in the HO channel that
       the LO channel will be added */
    num_tribslot = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;
    num_active_ts = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts;

    PMC_LOG_TRACE("chnl_id = %u, cal_slot = %u\n", chnl_id, cal_slot);
    PMC_LOG_TRACE("num_tribslot in HO = %u, num_active_ts = %u\n", num_tribslot, num_active_ts);

    /* check if there is enough ts availble in the HO channel */
    if (offline_update == 0)
    {
        if (num_active_ts > num_tribslot)
        {
            PMC_LOG(PMC_LOG_SEV_HIGH, ODTU_DMX_LOG_CODE_HO_TS_FULL, 5, 6);
            PMC_RETURN(ODTU_DMX_LOG_CODE_HO_TS_FULL);
        }
    }

    if (stage_inst == ODTU_DMX_STG_ONE)
    {
        /* query the record to update */
        key.calendar_entry = cal_slot;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else if (stage_inst == ODTU_DMX_STG_TWO)
    {
        /* query the record to update */
        key.calendar_entry = cal_slot;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = chnl_id;
        key.trib_slot = DO_NOT_USE_KEY;
    }

    PMC_LOG_TRACE("db_id = %u\n", odtu_dmx_handle->cfg.db_id);
    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                               odtu_dmx_handle->cfg.db_id,
                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);
    PMC_LOG_TRACE("odtu_dmx_tribslot_cfg: num_of_recs = %u\n", num_of_recs);
    
    if (num_of_recs == 0)
    {                                  
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }
    
    found = FALSE;
    if (stage_inst == ODTU_DMX_STG_ONE)
    {
        /*tribslot = q_result->p_recs[0]->trib_slot;*/
        
        for (i = 0; i < num_of_recs; i++)
        {
            if (q_result->p_recs[i]->calendar_entry == cal_slot)
            {
                tribslot = q_result->p_recs[i]->trib_slot;
                found = TRUE;
                break;
            }
        }
    }
    else {
        /*tribslot = q_result->p_recs[0]->ts_port_mo_lo;*/
        
        for (i = 0; i < num_of_recs; i++)
        {
            if (q_result->p_recs[i]->calendar_entry == cal_slot)
            {
                tribslot = q_result->p_recs[i]->ts_port_mo_lo;
                found = TRUE;
                break;
            }  
        }
    }
    
    /* Ensure that the record was found */
    PMC_ASSERT(TRUE == found, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);
        
    /* convert mapping_mode to chan_map_type */
    if (mapping_mode == UTIL_GLOBAL_AMP)
    {
        if (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1)
        {
                chan_map_type = 2;
        }
        else if (server_type == UTIL_GLOBAL_ODU3E1 && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
        {
                chan_map_type = 3;
        }
        else {
            chan_map_type = mapping_mode;
        }
    }  
    else if (mapping_mode == UTIL_GLOBAL_GMP)
    {
        chan_map_type = 4;
    }
    
    /* OHFS_REMOVE TS based configuration */
    for (i = 0; i < num_of_recs; i++)
    {
       
        if (offline_update == FALSE && PMC_SUCCESS == ret_val)
        {
            /* MSI configuration */
            ret_val = ohfs_remove_msi_lo_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                                cal_slot, 
                                                server_type, client_type, ts_type, ts_port, chnl_id);
        }
        
        if (offline_update == TRUE && PMC_SUCCESS == ret_val)
        {
            ret_val = ohfs_remove_lo_chnl_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                 cal_slot, chnl_id);  
        }
        
        if (PMC_SUCCESS == ret_val)
        {
            /* Find if this calendar slot is the first tributary slot of the LO channel */
            ret_val = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                         cal_slot, 
                                         (util_global_mapping_mode_t)chan_map_type, 
                                         last_ts, first_ts, valid);                                         
        }

        if (PMC_SUCCESS == ret_val)
        {
            /* test */
            PMC_LOG_TRACE("before calling sts_sw_lo_chnl_cfg\n");
            PMC_LOG_TRACE("cal_slot = %u\n", cal_slot);
            ret_val = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, ho_chnl_id,
                                         cal_slot,  
                                         chnl_id, 0, FALSE);                                                                       
        }    

        
        /* get correct MP_MGEN IPT location */        
        if (PMC_SUCCESS == ret_val && offline_update == FALSE)
        {
            PMC_LOG_TRACE("after calling sts_sw_lo_chnl_cfg\n");
            PMC_LOG_TRACE("cal_slot = %u\n", cal_slot);
        
            if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
            {
                    if(ghao_reprov == 0)
                    {
                        mgen_ipt_entry = cal_slot;
                    }
                    else
                    {
                        mgen_ipt_entry = 0x7F;
                    }
            }
            else
            {
                    if( mapping_mode == UTIL_GLOBAL_GMP )
                    {
                        
                        if( ghao_reprov == 0 && first_ts )
                        {
                            mgen_ipt_entry = odtu_dmx_get_free_mgen_ipt_entry(odtu_dmx_handle,
                                                                              chnl_id);
                        }
                        else
                        {
                            mgen_ipt_entry = 0x7F;
                        }                    
                    }
                    else
                    {
                        mgen_ipt_entry = odtu_dmx_get_free_mgen_ipt_entry(odtu_dmx_handle,
                                                                          chnl_id);                                                                            
                    }
            }   
                
            if (mgen_ipt_entry != 0x7F)
            {
                PMC_LOG_TRACE("calling rgmp_chnl_tribslot_cfg with mgen_ipt_entry = %u\n",  mgen_ipt_entry);
                if (mapping_mode == UTIL_GLOBAL_GMP) 
                {
                    if (first_ts == 1)
                    {
                        ret_val = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, mgen_ipt_entry, chnl_id, frm_num, 1);
                    }                
                }
                else {
                    ret_val = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, mgen_ipt_entry, chnl_id, frm_num, 1);
                }                                                                                                                             
            }
        }   

        if (PMC_SUCCESS == ret_val)
        {
            /* cycle should be changed to tributary slot position */
            if (mapping_mode == UTIL_GLOBAL_AMP)
            {   
                if (server_type == UTIL_GLOBAL_ODU3E1 && client_type == UTIL_GLOBAL_ODU2E)
                {
                    if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
                    {
                        ret_val = mp_mgen_ipt_va_get(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                                     cal_slot, &ipt_va);
                    }                                                     
                                                
                    if (PMC_SUCCESS == ret_val)
                    {
                        if (stage_inst == ODTU_DMX_STG_ONE)
                        {
                            /* query the record to update */
                            key.calendar_entry = DO_NOT_USE_KEY;
                            key.ho_channel = DO_NOT_USE_KEY;
                            key.mo_channel = chnl_id;
                            key.lo_channel = DO_NOT_USE_KEY;
                            key.trib_slot = DO_NOT_USE_KEY;
                        }
                        else if (stage_inst == ODTU_DMX_STG_TWO)
                        {
                            /* query the record to update */
                            key.calendar_entry = DO_NOT_USE_KEY;
                            key.ho_channel = DO_NOT_USE_KEY;
                            key.mo_channel = DO_NOT_USE_KEY;
                            key.lo_channel = chnl_id;
                            key.trib_slot = DO_NOT_USE_KEY;
                        }
                        
                        PMC_LOG_TRACE("db_id = %u\n", odtu_dmx_handle->cfg.db_id);
                        /* find the total number of records that matches the HO channel ID */
                        num_of_lo_recs = gen_db_query(&(odtu_dmx_handle->base),
                                                      odtu_dmx_handle->cfg.db_id,
                                                      odtu_dmx_handle->cfg.db_handle, (void *)&key,
                                                      (void **)&lo_q_result);
                        
                        if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
                        {
                            if (stage_inst == ODTU_DMX_STG_ONE)
                            {                           
                                tribslot = lo_q_result->p_recs[(ipt_va/2)]->trib_slot; 
                            }
                            else if (stage_inst == ODTU_DMX_STG_TWO) 
                            {
                                tribslot = lo_q_result->p_recs[(ipt_va/2)]->ts_port_mo_lo; 
                            }   
                        }
                        else {
                            if (stage_inst == ODTU_DMX_STG_ONE)
                            {                           
                                tribslot = lo_q_result->p_recs[(frm_num/2)]->trib_slot; 
                            }
                            else if (stage_inst == ODTU_DMX_STG_TWO) 
                            {
                                tribslot = lo_q_result->p_recs[(frm_num/2)]->ts_port_mo_lo; 
                            }
                        }
                        
                    }                                                   
                }
                

                if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
                {
                    mgen_ipt_entry = cal_slot;
                }
                else {
                    /* retrieve the mgen IPT entry for offline page */
                    mgen_ipt_entry = odtu_dmx_get_offline_mgen_ipt_entry(odtu_dmx_handle,
                                                                             chnl_id, 
                                                                             frm_num);
                }  

                
                PMC_LOG_TRACE("calling mp_mgen_amp_cfg with mgen_ipt_entry = %u\n",  mgen_ipt_entry);
                ret_val = mp_mgen_amp_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                          mgen_ipt_entry, tribslot, server_type, 
                                          (util_global_odukp_type_t)client_type, ts_type);
            }  
        }                                                
    }
       
    if (offline_update == FALSE && PMC_SUCCESS == ret_val)
    {
        /* update number of active tributary slots in the HO channel 
        odtu_dmx_tribslot_var_update(odtu_dmx_handle, tribslot, ho_chnl_id, 1);*/
        
        odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[tribslot]++;
        
        if (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[tribslot] != 1)
        {
            odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[tribslot] = 1;                      
        }
              
        PMC_LOG_TRACE("%s, %s, %d, odtu_dmx_handle->var.ho_chnl_cfg[%u].num_active_ts = %u\n", __FILE__, __FUNCTION__, __LINE__, ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
        
        if (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[tribslot] == 1)
        {
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   
                if (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[tribslot] == 1)
                {
                    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = num_active_ts+1;
                }
            }
            else {
                odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = num_active_ts+1;
            }                
        }
        PMC_LOG_TRACE("%s, %s, %d, odtu_dmx_handle->var.ho_chnl_cfg[%u].num_active_ts = %u\n", __FILE__, __FUNCTION__, __LINE__, ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);
    }
    
    PMC_RETURN(ret_val);

} /* odtu_dmx_tribslot_cfg */

/*******************************************************************************
*  odtu_dmx_lo_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a new LO channel is added to an active HO channel
*   in ODTU_DMX. This function will configure all associated tributary slots
*   for the LO channel.
*
*   Prior to calling this API, call odtu_dmx_lo_chnl_deactivate() API to hold
*   the channel under reset. After this API is called, call
*   odtu_dmx_lo_chnl_activate() API to activate the channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   stage_inst          - Enum for demux stage instance. See odtu_dmx_stage_instance_t.
*   lo_odu_type         - LO ODU rate. See util_global_odukp_type_t.
*   odu_flex_data       - number of ODU0's in ODUFlex client if lo_odu_type is
*                         set to ODUFLEX_GFP. For all other lo_odu_types, set to 0.
*   cbr_client_rate     - client bitrate in kbps in ODUFlex CBR client. 
*                         Set to 0 for all other lo_odu_types.
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   mapping_mode        - mapping mode of the LO ODU channel.
*                         See util_global_mapping_mode_t.
*   trib_slot_mask      - 3 x 32 bit tributary slot location mask for the LO  
*                         channel relative to its HO channel. \n
*                         Also used to specify the tributary slot assigment for
*                         ODUFlex LO channel.
*   msi_data            - describes the mapping between tributary slots and
*                         tributary ports. \n
*                         NOTE: For any given lo_odu_type, tributary indexes
*                               beyond the maximum number of allowed tributary
*                               slots will not be processed.
*   lo_ts_type          - type of tributary slot. See util_global_odu_line_payload_t
*   ho_chnl_id          - channel ID of the HO ODU that the LO ODU channel
*                         belongs to.
*   offline_update      - indication for programming active/standby pages
*                         0 -- active page configuration
*                         1 -- offline page configuration
*   ghao_reprov         - For DMX when reprovisioning a ghao channel,
*                         no need to reprogram the mp_mgen ipt table
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_prov(odtu_dmx_handle_t *odtu_dmx_handle,
                                       UINT32 chnl_id,
                                       odtu_dmx_stage_instance_t stage_inst,
                                       util_global_odukp_type_t lo_odu_type,
                                       UINT32 odu_flex_data,
                                       UINT32 cbr_client_rate,
                                       util_global_odukp_type_t ho_odu_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 trib_slot_mask[3],
                                       UINT32 msi_data,
                                       util_global_odu_line_payload_t lo_ts_type,
                                       UINT32 ho_chnl_id, 
                                       BOOL8 offline_update,
                                       BOOL8 ghao_reprov)
{
    PMC_ERROR ret_code = PMC_SUCCESS;

    UINT32 num_tribslot;
    UINT32 total_allowed_ts = 0;
    /*UINT32 pt;*/
    UINT32 cnt = 0;
    UINT32 ts_id, first_ts, last_ts;
    UINT32 eomf_cycle;
    UINT32 is_eomf_cycle;
    UINT32 is_first_cycle;
    UINT32 opu_payload_size, val, mod_val;
    UINT64 lo_frame_size;
    
    UINT32 cal_entry;
    line_core_otn_db_key    key;
    UINT32            num_of_recs =0;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    
    line_core_otn_db_key    lo_key;
    line_core_otn_db_query_res *q2_result;

    line_core_otn_db_rec    update_rec;
    odtu_dmx_chnl_state_t chnl_state;
    UINT32 num_odu0;
    DOUBLE cbr_client_bitrate = 0;
    BOOL use_custom_ppm;
    util_global_odu_line_payload_t ts_type = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;

    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(trib_slot_mask!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    
    if (chnl_id > 95 || stage_inst > ODTU_DMX_STG_TWO || lo_odu_type > UTIL_GLOBAL_LAST_ODUK
        || ho_odu_type > UTIL_GLOBAL_LAST_ODUK || mapping_mode > UTIL_GLOBAL_NO_MAP
        || msi_data > 80 || lo_ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD
        || ho_chnl_id > 95 || offline_update > 1)
    {
        PMC_RETURN(ODTU_DMX_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (offline_update == FALSE)
    {  
        /* associate the LO channel to its parent HO channel */ 
        odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_chnl_id);   
    }        

    /* wait one full calendar cycle */
    PMC_Q_USLEEP(odtu_dmx_handle,1);
    
    PMC_LOG_TRACE("stage_inst = %u\n", stage_inst);

    ts_type = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type;
    
    num_tribslot = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;
    PMC_LOG_TRACE("num_tribslot = %u\n", num_tribslot);

    /* Get total number of allowed ts in the LO channel */
    if (lo_odu_type != UTIL_GLOBAL_ODUFLEX_CBR && lo_odu_type != UTIL_GLOBAL_ODUFLEX_GFP)
    {
        ret_code = util_global_max_client_num_ts_get(ho_odu_type, lo_odu_type, ts_type, 
                                                     &total_allowed_ts);
        PMC_LOG_TRACE("total_allowed_ts = %u\n", total_allowed_ts);                                    
    }  
    else {
        total_allowed_ts = odu_flex_data;
        if (!digi_use_float32_get()) {
            cbr_client_bitrate = (DOUBLE)cbr_client_rate * 1000; /* convert to bps */
        } else {
            cbr_client_bitrate = (DOUBLE)((FLOAT)cbr_client_rate * 1000); /* convert to bps */
        }
    }  


    if (PMC_SUCCESS == ret_code)
    {
        odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].num_ts = total_allowed_ts;
    
        /* configure the tribslot */
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
        num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                   odtu_dmx_handle->cfg.db_id,
                                   odtu_dmx_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result);
        temp_result = *q_result;
    }
    
    if (PMC_SUCCESS == ret_code && num_of_recs == 0)
    {
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }   
        
    if (PMC_SUCCESS == ret_code && offline_update == FALSE)
    {
        /* Update the database for the LO channel ID */
        /* ts could range from 0 - max num ts in HO */ /* upto ODU3 now */
        for (ts_id = 0; ts_id < num_of_recs; ts_id++)
        {
            if ((trib_slot_mask[ts_id/32] >> (ts_id%32)) & 1)
            {                   
                PMC_LOG_TRACE("ts_id = %u, chnl_id = %u\n", ts_id, chnl_id);  
                PMC_LOG_TRACE("1. calendar_entry = %u, q_result->p_recs[%u]->mo_channel = %u\n", q_result->p_recs[ts_id]->calendar_entry, ts_id, q_result->p_recs[ts_id]->mo_channel);  
                PMC_LOG_TRACE("stage_inst = %u\n", stage_inst); 
                        
                if (stage_inst == ODTU_DMX_STG_ONE)
                {
                    if ( ODTU_DMX_DB_ENTRY_UNKNOWN == (q_result->p_recs[ts_id]->mo_channel) )
                    {
                        PMC_LOG_TRACE("2.q_result->p_recs[%u]->mo_channel = %u\n", ts_id, q_result->p_recs[ts_id]->mo_channel); 
                        
                        update_rec = *temp_result.p_recs[ts_id];
                        /* calendar entry is not assigned to any tributary slot/LO channel */
                        /* update MO channel ID to 'calendar_entry' */
                        /*update_rec.calendar_entry = q_result->p_recs[ts_id]->calendar_entry;*/
                        /*update_rec.ho_channel = ho_chnl_id;*/
                        update_rec.mo_channel = chnl_id;
                        update_rec.trib_slot = ts_id;
                        update_rec.cal_entry_status = 1;

                        if (odtu_dmx_handle->var.prov_mode == 1 && lo_ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                        {
                            update_rec.ts_port_mo_lo = 0xFFFFFFFE; /* mark this is channelized MO */
                        }
                        else {
                            update_rec.ts_port_mo_lo = DO_NOT_USE_KEY;
                        }
                        
                        PMC_LOG_TRACE("update_rec.ho_channel = %u, update_rec.trib_slot = %u, update_rec.mo_channel = %u, update_rec.lo_channel = %u\n",
                                    update_rec.ho_channel,
                                    update_rec.trib_slot,
                                    update_rec.mo_channel,
                                    update_rec.lo_channel);

                        if (offline_update == FALSE)
                        {
                            /* send back the result to the DB */ 
                            gen_db_update_record(&(odtu_dmx_handle->base),
                                                 odtu_dmx_handle->cfg.db_id,
                                                 odtu_dmx_handle->cfg.db_handle,
                                                 &update_rec);
                        }
                        cnt++;                                        
                    } else {
                      /* GHAO Accounting for existing ts by lo channel */
                        cnt++;                                        
                    }
                }   
                else if (stage_inst == ODTU_DMX_STG_TWO)
                {
                    if ( ODTU_DMX_DB_ENTRY_UNKNOWN == (q_result->p_recs[ts_id]->lo_channel) )
                    {
                        update_rec = *temp_result.p_recs[ts_id];
                        /* calendar entry is not assigned to any tributary slot/LO channel */
                        /* update LO channel ID to 'chnl_id' */
                        /*update_rec.calendar_entry = q_result->p_recs[ts_id]->calendar_entry;*/
                        update_rec.lo_channel = chnl_id;
                        update_rec.ts_port_mo_lo = ts_id;
                        update_rec.cal_entry_status = 1;
                        /* update_rec.ts_port_mo_lo = 1;*/
                        
                        PMC_LOG_TRACE("update_rec.ho_channel = %u, update_rec.trib_slot = %u, update_rec.mo_channel = %u, update_rec.lo_channel = %u\n",
                                      update_rec.ho_channel,
                                      update_rec.trib_slot,
                                      update_rec.mo_channel,
                                      update_rec.lo_channel);
                 
                        if (offline_update == FALSE)
                        {
                            /* send back the result to the DB */
                            gen_db_update_record(&(odtu_dmx_handle->base),
                                                 odtu_dmx_handle->cfg.db_id,
                                                 odtu_dmx_handle->cfg.db_handle,
                                                 &update_rec);
                        }
                        cnt++;
                    } else {
                        /* GHAO Accounting for existing ts by lo channel */
                        cnt++;
                    }
                }    
        
                /* 2.5G TS */
                if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                {
                    if (stage_inst == ODTU_DMX_STG_ONE)
                    {
                        if ( ODTU_DMX_DB_ENTRY_UNKNOWN == (q_result->p_recs[ts_id + (num_of_recs/2)]->mo_channel) )
                        {
                            update_rec = *temp_result.p_recs[ts_id + (num_of_recs/2)];
                            /*update_rec.calendar_entry = q_result->p_recs[ts_id + (num_of_recs/2)]->calendar_entry;*/
                            /*update_rec.ho_channel = ho_chnl_id;*/
                            update_rec.mo_channel = chnl_id;
                            /*update_rec.lo_channel = ODTU_DMX_DB_ENTRY_UNKNOWN;*/
                            update_rec.trib_slot = ts_id;
                            update_rec.cal_entry_status = 1;
                            /*update_rec.ts_port_ho_mo = ODTU_DMX_DB_ENTRY_UNKNOWN;*/
                            /*update_rec.ts_port_mo_lo = ODTU_DMX_DB_ENTRY_UNKNOWN;*/

                            if (odtu_dmx_handle->var.prov_mode == 1 && lo_ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                            {
                                update_rec.ts_port_mo_lo = 0xFFFFFFFE; /* mark this is channelized MO */
                            }
                            else {
                                update_rec.ts_port_mo_lo = DO_NOT_USE_KEY;
                            }
                            
                            PMC_LOG_TRACE("update_rec.ho_channel = %u, update_rec.trib_slot = %u, update_rec.mo_channel = %u, update_rec.lo_channel = %u\n",
                                        update_rec.ho_channel,
                                        update_rec.trib_slot,
                                        update_rec.mo_channel,
                                        update_rec.lo_channel);
                            
                            /* send back the result to the DB */ 
                            gen_db_update_record(&(odtu_dmx_handle->base),
                                                 odtu_dmx_handle->cfg.db_id,
                                                 odtu_dmx_handle->cfg.db_handle,
                                                 &update_rec);
                        }
                    }
                    else if (stage_inst == ODTU_DMX_STG_TWO)
                    {
                        /*if ( ODTU_DMX_DB_ENTRY_UNKNOWN == (q_result->p_recs[ts_id]->lo_channel) */
                        if ( ODTU_DMX_DB_ENTRY_UNKNOWN == (q_result->p_recs[ts_id + (num_of_recs/2)]->lo_channel) )
                        {         
                            update_rec = *temp_result.p_recs[ts_id + (num_of_recs/2)];
                            /* can't write to any calendar entry if it is TS2G5. Calendar slot
                            ** must match the tributary slot location
                            */
                            /* if TS = 1 and 2G5, then start from the first calendar entry and repeat for the second half */
                            /* if TS = 2 and 2G5, then start from the second calendar entry */
                            /*update_rec.calendar_entry = q_result->p_recs[ts_id + (num_of_recs/2)]->calendar_entry;*/
                            
                            update_rec.lo_channel = chnl_id;   
                            update_rec.ts_port_mo_lo = ts_id;   
                            /*update_rec.trib_slot = ts_id;*/
                            update_rec.cal_entry_status = 1;
                            
                            /* send back the result to the Rx DB */
                            gen_db_update_record(&(odtu_dmx_handle->base),
                                                 odtu_dmx_handle->cfg.db_id,
                                                 odtu_dmx_handle->cfg.db_handle,
                                                 &update_rec); 
                            
                            
                            PMC_LOG_TRACE("update_rec.ho_channel = %u, update_rec.trib_slot = %u, update_rec.mo_channel = %u, update_rec.lo_channel = %u\n",
                                            update_rec.ho_channel,
                                            update_rec.trib_slot,
                                            update_rec.mo_channel,
                                            update_rec.lo_channel);    
                               
                        }
                    }                                                 
                    
                }
                
                /* if configured all tribslot for the LO channel, end for loop */
                if (lo_odu_type != UTIL_GLOBAL_ODUFLEX_CBR && lo_odu_type != UTIL_GLOBAL_ODUFLEX_GFP) 
                { 
                    if (cnt == total_allowed_ts)
                    {
                        break;
                    }
                    odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].num_ts = total_allowed_ts;
                    num_odu0 = 0;
                }  
                else {
                    odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].num_ts = cnt;
                    num_odu0 = cnt;
                    /*total_allowed_ts = num_odu0;*/
                } 
            }    /* matching trib slot mask with num of records*/
        }     /* for loop */
    }         /* if offline update == FALSE */


    /* OFFLINE AND ONLINE CONFIGURATION UPDATES*/
    /* MOST OF THESE CONFIG REGISTERS HAVE PAGE A/B RAMS, THAT SWITCH CONFIG WHEN CONFIG_UPDATE IS TRIGGERED*/

    if (PMC_SUCCESS == ret_code)
    {
        if (stage_inst == ODTU_DMX_STG_ONE)
        {
            /* query the record to update */
            lo_key.calendar_entry = DO_NOT_USE_KEY;
            lo_key.ho_channel = DO_NOT_USE_KEY;
            lo_key.mo_channel = chnl_id;
            lo_key.lo_channel = DO_NOT_USE_KEY;
            lo_key.trib_slot = DO_NOT_USE_KEY;
        }
        else if (stage_inst == ODTU_DMX_STG_TWO)
        {
            /* query the record to update */
            lo_key.calendar_entry = DO_NOT_USE_KEY;
            lo_key.ho_channel = DO_NOT_USE_KEY;
            lo_key.mo_channel = DO_NOT_USE_KEY;
            lo_key.lo_channel = chnl_id;
            lo_key.trib_slot = DO_NOT_USE_KEY;
        }
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                   odtu_dmx_handle->cfg.db_id,
                                   odtu_dmx_handle->cfg.db_handle, (void *)&lo_key,
                                   (void **)&q2_result);
        PMC_LOG_TRACE("num_of_recs = %u\n", num_of_recs);
    
        if (num_of_recs == 0)
        {
            PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
        }
    }

    /*
    ** Set EOMF_CYCLE and FIRST_CYCLE in STS_SW
    ** Call tribslot_cfg
    */
    if (PMC_SUCCESS == ret_code)
    {
        /* switch EOMF_CYCLE */
        /* calculcate LO_EOMF_CYCLE */
        opu_payload_size = ((ho_odu_type == UTIL_GLOBAL_ODU4)? 15200:15232);
        lo_frame_size = (opu_payload_size * total_allowed_ts);
        val = ((lo_frame_size % 48 != 0)? ((lo_frame_size/48)+1):(lo_frame_size/48));
        mod_val = (val % num_of_recs);
        eomf_cycle = ((mod_val == 0)? num_of_recs-1:mod_val-1);

        if (lo_odu_type == UTIL_GLOBAL_ODUFLEX_CBR || lo_odu_type == UTIL_GLOBAL_ODUFLEX_GFP)
        {   
            odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].num_ts = odu_flex_data;         
        }
   
        for (cal_entry = 0; cal_entry < num_of_recs; cal_entry++)
        {
            first_ts = (cal_entry == 0)? 1:0;
            last_ts = (cal_entry == total_allowed_ts-1)? 1:0;
            is_eomf_cycle = (cal_entry == eomf_cycle) ? 1 : 0;
            is_first_cycle = (cal_entry == 0) ? 1 : 0;
                    
            PMC_LOG_TRACE("first_ts = %u, last_ts = %u\n", first_ts, last_ts);
            
                

            if (PMC_SUCCESS == ret_code)
            {
                ret_code = odtu_dmx_tribslot_cfg(odtu_dmx_handle, stage_inst, chnl_id, 
                                                 q2_result->p_recs[cal_entry]->calendar_entry, 
                                                 msi_data, ho_chnl_id,
                                                 lo_odu_type, ho_odu_type, 
                                                 ts_type, mapping_mode,
                                                 first_ts, last_ts, cal_entry, offline_update, ghao_reprov);
            }
            
            if (PMC_SUCCESS == ret_code)
            {
                 ret_code = sts_sw_lo_chnl_cfg_all(odtu_dmx_handle->sts_sw_handle, ho_chnl_id,
                                                   q2_result->p_recs[cal_entry]->calendar_entry,  
                                                   chnl_id, is_eomf_cycle, is_first_cycle);
            }
            
        }
    }
    
    if(odtu_dmx_handle->var.ppm_offset[chnl_id] != 0)
    {
        use_custom_ppm = TRUE;
    }
    else
    {
        use_custom_ppm = FALSE;
    }
    
    /*
    ** Set RXJC_GMP_PARAM
    ** Set MP_MGEN DATA MASK AND LAST TXR CFG
    ** Set CPACT_OBUF CHNL CFG
    */              
    if (ret_code == PMC_SUCCESS)
    {                                              
        ret_code = odtu_dmx_rgmp_lo_chnl_prov(odtu_dmx_handle->rgmp_handle, chnl_id,
                                              lo_odu_type, ho_odu_type, mapping_mode,
                                              ts_type, total_allowed_ts, cbr_client_bitrate, 
                                              num_tribslot, offline_update, use_custom_ppm,
                                              odtu_dmx_handle->var.ppm_offset[chnl_id] ); /* added 0 for client_rate */
    }                                       


    /* every time a new LO is added to the same HO channel, update to keep track
    ** of occupied tributary slots within the HO
    */
    if (PMC_SUCCESS == ret_code && offline_update == FALSE)
    {
        chnl_state = ODTU_DMX_CHNL_EQUIPPED;
        
        odtu_dmx_lo_chnl_var_update(odtu_dmx_handle, chnl_id, ho_chnl_id, chnl_state,
                                    lo_odu_type, ho_odu_type, total_allowed_ts, 
                                    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array,
                                    ts_type, mapping_mode, msi_data);
        
        PMC_LOG_TRACE("num_tribslot = %u\n", num_tribslot);
    }
    
    PMC_LOG_TRACE("%s, %s, %d, num_active_ts = %u\n", __FILE__, __FUNCTION__, __LINE__, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts); 
    


    /*PMC_RETURN(PMC_SUCCESS);*/
    PMC_RETURN(ret_code);
} /* odtu_dmx_lo_chnl_prov */

/*******************************************************************************
*  odtu_dmx_lo_chnl_offline_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a new LO channel is added to an active HO channel
*   in ODTU_DMX. This function will configure all associated tributary slots
*   for the LO channel.
*
*   Prior to calling this API, call odtu_dmx_lo_chnl_deactivate() API to hold
*   the channel under reset. After this API is called, call
*   odtu_dmx_lo_chnl_activate() API to activate the channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   stage_inst          - Enum for demux stage instance. See odtu_dmx_stage_instance_t.
*   lo_odu_type         - LO ODU rate. See util_global_odukp_type_t.
*   odu_flex_data       - number of ODU0's in ODUFlex client if lo_odu_type is
*                         set to ODUFLEX_GFP. For all other lo_odu_types, 
*                         set to 0.
*   cbr_client_data     - client bitrate in kbps when lo_odu_type is ODUFLEX_CBR.
*                         Set to 0 for all other lo odu types.
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   mapping_mode        - mapping mode of the LO ODU channel.
*                         See util_global_mapping_mode_t.
*   trib_slot_mask      - 3 x 32 bit tributary slot location mask for the LO  
*                         channel relative to its HO channel. \n
*                         Also used to specify the tributary slot assigment for
*                         ODUFlex LO channel.
*   msi_data            - describes the mapping between tributary slots and
*                         tributary ports. \n
*                         NOTE: For any given lo_odu_type, tributary indexes
*                               beyond the maximum number of allowed tributary
*                               slots will not be processed.
*   ts_type             - type of tributary slot. See util_global_odu_line_payload_t
*   ho_chnl_id          - channel ID of the HO ODU that the LO ODU channel
*                         belongs to.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_offline_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT32 chnl_id,
                                                 odtu_dmx_stage_instance_t stage_inst,
                                                 util_global_odukp_type_t lo_odu_type,
                                                 UINT32 odu_flex_data,
                                                 UINT32 cbr_client_data,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 trib_slot_mask[3],
                                                 UINT32 msi_data,
                                                 util_global_odu_line_payload_t ts_type,
                                                 UINT32 ho_chnl_id)
{
    PMC_ERROR ret_code = PMC_SUCCESS;

    PMC_ENTRY();
    
    ret_code = odtu_dmx_lo_chnl_prov(odtu_dmx_handle,
                                     chnl_id,
                                     stage_inst,
                                     lo_odu_type,
                                     odu_flex_data,
                                     cbr_client_data,
                                     ho_odu_type,
                                     mapping_mode,
                                     trib_slot_mask,
                                     msi_data,
                                     ts_type,
                                     ho_chnl_id, 
                                     1,
                                     0);

    PMC_RETURN(ret_code);
} /* odtu_dmx_lo_chnl_offline_update */
 
/*******************************************************************************
*  odtu_dmx_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a LO channel needs to be removed from a HO
*   channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   mux_stage           - enum for demux instace. See odtu_dmx_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO channel ID that the LO channel belongs to
*                         Valid range: 0 - 95
*   offline_update      - indicates if the API is used for initial configuration
*                         or for updating the offline page.
*                         0 -- initial config
*                         1 -- offline config
*   mo_odu3e2_odu3e1    - boolean to indicate if MO is ODU3e1 or ODU3e2
*                         TRUE: MO is ODU3e1 or ODU3e2
*                         FALSE: MO is not ODU3e1 or ODU3e2
*   extra_cycle         - extra cycle for MO ODU3e1 or ODU3e2
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_deprov(odtu_dmx_handle_t  *odtu_dmx_handle,
                                         odtu_dmx_stage_instance_t mux_stage,
                                         UINT32 chnl_id, UINT32 ho_chnl_id,
                                         BOOL8 offline_update,
                                         BOOL8 mo_odu3e2_odu3e1,
                                         UINT8 extra_cycle)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 cycle = 0;
    UINT32 i;
    UINT32 ts_id;
    UINT32 num_tribslot;
    UINT32 tribslot;
    UINT32 mgen_ipt_entry = 0;
    
    /*UINT32 cal_entry;*/
    line_core_otn_db_key    key;
    UINT32            num_of_recs;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    
    odtu_dmx_chnl_state_t chnl_state;
    
    UINT32 mo_odu3e1_3e2_entry = 0x7F;

    PMC_ENTRY();


    PMC_LOG_TRACE("ODTU_DMX DEPROV : ORIG");
    if (odtu_dmx_handle->var.prov_mode == FALSE)
    {
        PMC_ATOMIC_YIELD(odtu_dmx_handle->base.parent_handle->parent_handle,0);
        odtu_dmx_dump_var_context( odtu_dmx_handle, ho_chnl_id, chnl_id, TRUE );
    }
    
    /* argument checking */
    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0 , 0);

    /* associate this LO to the HO channel */
    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_chnl_id);
    
    num_tribslot = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;

    /* deprov channel configurations */
    /* deprov rxjc channel */
    result = rxjc_proc_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle, 
                                      chnl_id);
    
    if (PMC_SUCCESS == result)
    {
        result = cpact_obuf_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->cpact_obuf_handle, 
                                           chnl_id);
    }

    if (PMC_SUCCESS == result)
    {
        if( offline_update == 0 )
        {
            result = cpact_obuf_chnl_mapping_cfg(odtu_dmx_handle->rgmp_handle->cpact_obuf_handle, 
                                                 chnl_id, 0);
        }
    }

    
    if (PMC_SUCCESS == result)
    {
        if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
        {
            result = mp_mgen_gmp_uncfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, chnl_id);
        }
        else
        {
            if(offline_update == 0) {
                PMC_LOG_TRACE("lo_chnl_deprov, mp_mgen_gmp_uncfg with chnl_id = %u\n",  chnl_id);
                result = mp_mgen_gmp_uncfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, chnl_id);
            }
        }

    }  
            

    if (mux_stage != ODTU_DMX_STG_TWO)
    {
        /* mo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else {
        /* lo is being deprovisioned */
        /* query the records to find the calendar entries for this LO ODU channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = chnl_id;
        key.trib_slot = DO_NOT_USE_KEY;
    }

    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                               odtu_dmx_handle->cfg.db_id,
                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);
    temp_result = *q_result;
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }
       
    /* determine if MO is ODU3e1 or ODU3e2 */
    if (mo_odu3e2_odu3e1)
    {
        mo_odu3e1_3e2_entry = extra_cycle;
    }     

    if (PMC_SUCCESS == result)
    {
        for (ts_id = 0; ts_id < num_of_recs; ts_id++)
        {
            cycle = temp_result.p_recs[ts_id]->calendar_entry;
            
            if (mux_stage == ODTU_DMX_STG_ONE)
            {
                tribslot = temp_result.p_recs[ts_id]->trib_slot;
            }
            else
            {
              tribslot = temp_result.p_recs[ts_id]->ts_port_mo_lo;
            }
         
            if (offline_update == 0)
            {
                if (PMC_SUCCESS == result)
                {
                    if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
                    {
                        mgen_ipt_entry = cycle;
                        result = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                                 mgen_ipt_entry, 0x7F, 0, 0);  
                    }
                    else
                    {
                        mgen_ipt_entry = odtu_dmx_get_mgen_ipt_entry( odtu_dmx_handle, chnl_id );
                        PMC_LOG_TRACE("lo_chnl_deprov, get mgen_ipt_entry = %u\n",  mgen_ipt_entry);

                        if( mgen_ipt_entry != 0x7F ) {
                            PMC_LOG_TRACE("clearing ipt cfg on mgen_ipt_entry = %u\n",  mgen_ipt_entry);

                            result = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                                   mgen_ipt_entry, 0x7F, 0, 0);  
                            PMC_LOG_TRACE("lo_chnl_deprov, free up mgen_ipt_entry = %u\n",  mgen_ipt_entry);
                            odtu_dmx_deprov_first_mgen_ipt_entry( odtu_dmx_handle, chnl_id );
                        }
                    }

                }                                                                         
            }
            
            if (PMC_SUCCESS == result)
            {
                result = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, 
                                            ho_chnl_id, cycle, chnl_id, 1, odtu_dmx_handle->var.prov_mode);  
            } 
            
            if (PMC_SUCCESS == result)
            {
                ohfs_remove_lo_chnl_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                           cycle, 0x7F);  
            }                                            
            
            if (PMC_SUCCESS == result)
            {                                    
                result = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, cycle,
                                            UTIL_GLOBAL_NO_MAP, 0, 0, 0);
            }
            
            /* MSI configuration; Set to Unallocated */
            if (PMC_SUCCESS == result)
            {
                if (odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODU0
                    && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_CBR
                    && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    /*result = ohfs_remove_exp_msi_set(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, cycle, 0);*/
                    result = ohfs_remove_msi_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                 cycle, 
                                                 odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type,
                                                 UTIL_GLOBAL_ODUK_DONT_CARE,
                                                 odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ts_type, 
                                                 0);
                }                                                 
            }

            if (PMC_SUCCESS == result)
            {
                result = sts_sw_lo_calendar_cfg_deprov(odtu_dmx_handle->sts_sw_handle, cycle);
            }  
            
            if (PMC_SUCCESS == result)
            {
                if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
                {
                    result = mp_mgen_amp_uncfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, cycle);
                }
                else
                {
                    if(offline_update == 0 && mgen_ipt_entry != 0x7F ) {
                        PMC_LOG_TRACE("lo_chnl_deprov, mp_mgen_amp_uncfg mgen_ipt_entry = %u\n",  mgen_ipt_entry);
                        result = mp_mgen_amp_uncfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, mgen_ipt_entry);
                    }
                }
            }

        PMC_ATOMIC_YIELD(odtu_dmx_handle->base.parent_handle->parent_handle,0);
        }
        
        /* this is to clean 33rd entry for MO ODU3e1 or MO ODu3e2 */
        if (PMC_SUCCESS == result && mo_odu3e1_3e2_entry != 0x7F)
        {
            cycle = mo_odu3e1_3e2_entry;
         
            if (offline_update == 0)
            {
                if (PMC_SUCCESS == result)
                {
                    result = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                             cycle, 0x7F, 0, 0);  
                }                                                                         
            }
            
            if (PMC_SUCCESS == result)
            {
                result = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, 
                                            ho_chnl_id, cycle, chnl_id, 1, odtu_dmx_handle->var.prov_mode);  
            } 
            
            if (PMC_SUCCESS == result)
            {
                ohfs_remove_lo_chnl_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                           cycle, 0x7F);  
            }                                            
            
            if (PMC_SUCCESS == result)
            {                                    
                result = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, cycle,
                                            UTIL_GLOBAL_NO_MAP, 0, 0, 0);
            }
            
            /* MSI configuration; Set to Unallocated */
            if (PMC_SUCCESS == result)
            {
                if (odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODU0
                    && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_CBR
                    && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type != UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    /*result = ohfs_remove_exp_msi_set(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, cycle, 0);*/
                    result = ohfs_remove_msi_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                 mo_odu3e1_3e2_entry, 
                                                 odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type,
                                                 UTIL_GLOBAL_ODUK_DONT_CARE,
                                                 odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ts_type, 
                                                 0);
                }                                                 
            }
            
            if (PMC_SUCCESS == result)
            {
                result = sts_sw_lo_calendar_cfg_deprov(odtu_dmx_handle->sts_sw_handle, cycle);
            }  
            
            if (PMC_SUCCESS == result)
            {
                result = mp_mgen_amp_uncfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, cycle);
            }
        }    
    }

    if (PMC_SUCCESS == result)
    {
        if (offline_update == 0)
        {
            /* if ODTU4.31 workaround is in place, then remove 31 from FMF1 MUX */
            if (odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type == UTIL_GLOBAL_ODU4
                && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].lo_oduk_type == UTIL_GLOBAL_ODU3
                && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED
                && num_of_recs == 32)
            {
                num_of_recs = 31;    
            }
            
            /* if ODTU4.31 workaround is in place, then remove 31 from FMF1 MUX */
            if (odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ho_oduk_type == UTIL_GLOBAL_ODU4
                && (odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].lo_oduk_type == UTIL_GLOBAL_ODU3E2
                    || odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].lo_oduk_type == UTIL_GLOBAL_ODU3E1)
                && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED
                && num_of_recs == 32)
            {
                num_of_recs = 33;    
                
            }   
            
            if (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts - (num_of_recs/2); 
                
                if (mux_stage == ODTU_DMX_STG_ONE)
                {
                    for (i = 0; i < num_of_recs/2; i++)
                    {
                        odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[temp_result.p_recs[i]->trib_slot] = 0;
                        odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[temp_result.p_recs[i]->trib_slot] = 0;
                    }
                }
                else 
                {
                    for (i = 0; i < num_of_recs/2; i++)
                    {
                        odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[temp_result.p_recs[i]->ts_port_mo_lo] = 0;
                        odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[temp_result.p_recs[i]->ts_port_mo_lo] = 0;
                    }
                }
            }
            else 
            {
                odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts - num_of_recs; 
                  if (mux_stage == ODTU_DMX_STG_ONE)
                  {
                      for (i = 0; i < num_of_recs; i++)
                      {

                          if (NULL != temp_result.p_recs[i] &&
                              temp_result.p_recs[i]->trib_slot != ODTU_DMX_DB_ENTRY_UNKNOWN)
                          {
                              odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[temp_result.p_recs[i]->trib_slot] = 0;
                              odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[temp_result.p_recs[i]->trib_slot] = 0;
                          }
                          else if (NULL != temp_result.p_recs[i] &&
                                   temp_result.p_recs[i]->trib_slot == ODTU_DMX_DB_ENTRY_UNKNOWN)
                          {
                              PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                                      "Hit ODTU_DMX1 trib_slot i=%d is 0xFFFFFFF. Number Of Records = %d\n", i, num_of_recs);
                          }
                      }
                  }
                  else 
                  {
                      for (i = 0; i < num_of_recs; i++)
                      {
                          if (NULL != temp_result.p_recs[i] &&
                              temp_result.p_recs[i]->ts_port_mo_lo != ODTU_DMX_DB_ENTRY_UNKNOWN)
                          {
                              odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[temp_result.p_recs[i]->ts_port_mo_lo] = 0;
                              odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[temp_result.p_recs[i]->ts_port_mo_lo] = 0;
                          }
                          else if (NULL != temp_result.p_recs[i] &&
                                   temp_result.p_recs[i]->ts_port_mo_lo == ODTU_DMX_DB_ENTRY_UNKNOWN)
                          {
                              PMC_LOG(PMC_LOG_SEV_HIGH, 0, 0, 0,
                                      "Hit ODTU_DMX2 ts_port_mo_lo i=%d is 0xFFFFFFF. Number Of Records = %d\n", i, num_of_recs);
                          }
                      }
                  }
            }
         
            if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == FALSE)
            {
                PMC_LOG_TRACE("lo_chnl_deprov, deprov chnl_id = %u\n",  chnl_id);
                odtu_dmx_deprov_mgen_ipt_entry(odtu_dmx_handle, chnl_id );
            }
        
        }
        else {
            /* we need this context to be present for offline_deprov to be used 
            ** in msi configuration
            */
            chnl_state = ODTU_DMX_CHNL_START;
            
            odtu_dmx_lo_chnl_var_update(odtu_dmx_handle, chnl_id, ho_chnl_id, chnl_state,
                                        UTIL_GLOBAL_ODUK_DONT_CARE, 
                                        UTIL_GLOBAL_ODUK_DONT_CARE, 0, NULL, LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 
                                        UTIL_GLOBAL_NO_MAP, 0x7F);

            odtu_dmx_handle->var.ppm_offset[chnl_id] = 0;
        }                                    
    
        PMC_LOG_TRACE("ODTU_DMX DEPROV : UPDATED");
        if (odtu_dmx_handle->var.prov_mode == FALSE)
        {
            odtu_dmx_dump_var_context( odtu_dmx_handle, ho_chnl_id, chnl_id, TRUE );
        }
    }
    
    PMC_RETURN(result);
    
} /* odtu_dmx_lo_chnl_deprov */

/*******************************************************************************
*  odtu_dmx_lo_chnl_offline_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a LO channel needs to be removed from a HO
*   channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   mux_stage           - enum for demux instance. See odtu_dmx_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO channel ID that the LO channel belongs to
*                         Valid range: 0 - 95
*   mo_odu3e2_odu3e1    - boolean to indicate if MO is ODU3e1 or ODU3e2
*                         TRUE: MO is ODU3e1 or ODU3e2
*                         FALSE: MO is not ODU3e1 or ODU3e2
*   extra_cycle         - extra cycle for MO ODU3e1 or ODU3e2
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_offline_deprov(odtu_dmx_handle_t  *odtu_dmx_handle,
                                                 odtu_dmx_stage_instance_t mux_stage,
                                                 UINT32 chnl_id, UINT32 ho_chnl_id,
                                                 BOOL8 mo_odu3e2_odu3e1,
                                                 UINT8 extra_cycle)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = odtu_dmx_lo_chnl_deprov(odtu_dmx_handle, mux_stage, chnl_id, 
                                     ho_chnl_id, 1,
                                     mo_odu3e2_odu3e1,
                                     extra_cycle);
    
    PMC_RETURN(result);
} /* odtu_dmx_lo_chnl_offline_deprov */    

/*******************************************************************************
*  odtu_dmx_ho_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to provision a new HO channel in ODTU_DMX.
*
*   Note that prior to calling this API, the HO channel must be held in reset
*   by calling odtu_dmx_ho_chnl_deactivate() API. All corresponding
*   tributary slots and the associated LO channels being added to this HO
*   channel must be configured using odtu_dmx_lo_chnl_prov()
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   stage_inst          - enum for demux stage instance. See odtu_dmx_stage_instance_t
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   ts_type             - size of tributary slot. See odu_line_payload_t
*                         ODU_UNCHANNELIZED  = 0,
*                         ODU_TS_1G25        = 1,
*                         ODU_TS_2G5         = 2,
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_prov(odtu_dmx_handle_t *odtu_dmx_handle,
                                       odtu_dmx_stage_instance_t stage_inst,
                                       UINT32 chnl_id,
                                       util_global_odukp_type_t ho_odu_type,
                                       util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 max_ts_num;
    odtu_dmx_chnl_state_t chnl_state;
    UINT32 cal_entry;
    line_core_otn_db_key key;
    UINT32 num_of_recs;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;

    line_core_otn_db_key mo_key;
    
    line_core_otn_db_key ho_key;
    UINT32 ho_num_of_recs;
    line_core_otn_db_query_res *ho_q_result;
    UINT32 mo_num_of_recs;
    UINT32 i;
    line_core_otn_db_rec new_rec;
    
    UINT32 dummy_idx;
    UINT32 valid_null_cycle;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl_id > 95 || (stage_inst != 2 && (ho_odu_type == UTIL_GLOBAL_ODUFLEX_CBR 
        || ho_odu_type == UTIL_GLOBAL_ODUFLEX_GFP))
        || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD)
    {
        PMC_RETURN(ODTU_DMX_LOG_ERR_INVALID_PARAMETERS);
    }
    
    
   
    if (ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        /* set NULL frame mode */
        if (TRUE == pmc_coreotn_dmx_empty_ho_order_revision(&odtu_dmx_handle->base))
        {
            ret_code = ohfs_remove_func_mode_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                 chnl_id, 1);
        }                                                 
    }  
   
    if (PMC_SUCCESS == ret_code)
    {  
        /* associate the LO channel to its parent HO channel */
        odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, 0x7F);
    }
    
    /* wait 1 full calendar cycle */
    PMC_Q_USLEEP(odtu_dmx_handle,1);
    
    if (stage_inst == 2 && (ho_odu_type == UTIL_GLOBAL_ODUFLEX_CBR || ho_odu_type == UTIL_GLOBAL_ODUFLEX_GFP))
    {  
        max_ts_num = 0;
    }
    else {  
        ret_code = util_global_max_server_num_ts_get(UTIL_GLOBAL_MUX_DEMUX, 
                                                     ho_odu_type, ts_type,
                                                     &max_ts_num);
    }
    if (ret_code != PMC_SUCCESS)
    {
        PMC_RETURN(ret_code);
    }
    PMC_LOG_TRACE("max_ts_num = %u\n",  max_ts_num);

    if (stage_inst != ODTU_DMX_STG_TWO)
    {
        /* get calendar entries for this HO channel */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = chnl_id;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else {
        /* get calendar entries for this HO channel */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    
    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                               odtu_dmx_handle->cfg.db_id,
                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);
    temp_result = *q_result;

    PMC_LOG_TRACE("num_of_recs for HO chnl_prov  = %u  \n", num_of_recs);
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }

    /* need to add an extra entry here if HO = ODU3 and num_of_recs = 31 
    ** need to read through ho calendar entry and grab an empty calendar slot
    ** 
    */
    if (stage_inst == 2 && ho_odu_type == UTIL_GLOBAL_ODU3 && num_of_recs == 31
        && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {        
        
        ret_code = odtu_dmx_first_valid_dummy_find(odtu_dmx_handle,
                                                   q_result->p_recs[0]->calendar_entry, 
                                                   &valid_null_cycle,
                                                   &dummy_idx);
                                                   
        for (i = valid_null_cycle; i < 96; i++)
        {
            ho_key.calendar_entry = i;
            ho_key.ho_channel = DO_NOT_USE_KEY;
            ho_key.mo_channel = DO_NOT_USE_KEY;
            ho_key.lo_channel = DO_NOT_USE_KEY;
            ho_key.trib_slot = DO_NOT_USE_KEY;
            
            /* find the total number of records that matches the HO channel ID */
            ho_num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                          odtu_dmx_handle->cfg.db_id,
                                          odtu_dmx_handle->cfg.db_handle, (void *)&ho_key,
                                          (void **)&ho_q_result);
            
            PMC_LOG_TRACE("ho_num_of_recs = %u, ho_rec[%u].calendar_entry = %u, ho_rec[%u].ho_channel = %u, how_rec[%u].trib_slot = %u, ho_rec[%u].mo_channel = %u, ho_rec[%u].lo_channel = %u\n",
                          ho_num_of_recs,
                          i,
                          ho_q_result->p_recs[0]->calendar_entry,
                          i,
                          ho_q_result->p_recs[0]->ho_channel,
                          i,
                          ho_q_result->p_recs[0]->trib_slot,
                          i,
                          ho_q_result->p_recs[0]->mo_channel,
                          i,
                          ho_q_result->p_recs[0]->lo_channel);

            if (ho_q_result->p_recs[0]->ho_channel == ODTU_DMX_DB_ENTRY_UNKNOWN
                && ho_q_result->p_recs[0]->cal_entry_status == ODTU_DMX_DB_ENTRY_UNKNOWN 
                && i != UTIL_GLOBAL_DMX_DUMMY_SCHD_ID_1 && i != UTIL_GLOBAL_DMX_DUMMY_SCHD_ID_2)
            {
                /* add 32nd entry */
                new_rec.calendar_entry = i;
                new_rec.ho_channel = ODTU_DMX_DB_ENTRY_UNKNOWN;
                new_rec.mo_channel = chnl_id;
                new_rec.lo_channel = DO_NOT_USE_KEY;
                new_rec.trib_slot = 80+i; 
                new_rec.cal_entry_status = 1;
                new_rec.ts_port_ho_mo = DO_NOT_USE_KEY;
                new_rec.ts_port_mo_lo = DO_NOT_USE_KEY;
                
                PMC_LOG_TRACE("new_rec[%u].calendar_entry = %u, new_rec[%u].ho_channel = %u, new_rec[%u].trib_slot = %u, new_rec[%u].mo_channel = %u, new_rec[%u].lo_channel = %u\n",
                              i,
                              new_rec.calendar_entry,
                              i,
                              new_rec.ho_channel,
                              i,
                              new_rec.trib_slot,
                              i,
                              new_rec.mo_channel,
                              i,
                              new_rec.lo_channel);
                
                ret_code = gen_db_add_record(&(odtu_dmx_handle->base),
                                             odtu_dmx_handle->cfg.db_id,
                                             odtu_dmx_handle->cfg.db_handle,
                                             (void *)&new_rec);
                
                if (PMC_SUCCESS == ret_code)
                {
                    ret_code = odtu_dmx_dummy_schd_cfg(odtu_dmx_handle, dummy_idx, chnl_id, i, 1); 
                } 

                break;
               
            }
        }

        /* query the record to update */
        mo_key.calendar_entry = DO_NOT_USE_KEY;
        mo_key.ho_channel = DO_NOT_USE_KEY;
        mo_key.mo_channel = chnl_id;
        mo_key.lo_channel = DO_NOT_USE_KEY;
        mo_key.trib_slot = DO_NOT_USE_KEY;
    
        /* find the total number of records that matches the HO channel ID */
        mo_num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                      odtu_dmx_handle->cfg.db_id,
                                      odtu_dmx_handle->cfg.db_handle, (void *)&mo_key,
                                      (void **)&q_result);
        
        num_of_recs = mo_num_of_recs;
        temp_result = *q_result;
        
        PMC_LOG_TRACE("num_of_recs for HO ODU3 special case = %u   mo_num_of_recs = %u \n", num_of_recs, mo_num_of_recs);
        PMC_LOG_TRACE("dummy calendar_entry = %u\n", i);
    } /* end of if */
    else if (stage_inst == 2 && num_of_recs == 33 
             && (ho_odu_type == UTIL_GLOBAL_ODU3E1 || ho_odu_type == UTIL_GLOBAL_ODU3E2)
             && ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
    { 
        /* use only the first 32 entries */
        new_rec = *temp_result.p_recs[32];
        new_rec.mo_channel = DO_NOT_USE_KEY;
        new_rec.lo_channel = DO_NOT_USE_KEY; 
        new_rec.trib_slot = DO_NOT_USE_KEY; /* watch for this */
        new_rec.ts_port_ho_mo = 2;
        new_rec.cal_entry_status = chnl_id;
        
        /* send back the result to the DB */ 
        gen_db_update_record(&(odtu_dmx_handle->base),
                             odtu_dmx_handle->cfg.db_id,
                             odtu_dmx_handle->cfg.db_handle,
                             &new_rec);
                             
        /* update the number of calendar entries that belong to this MO */
        num_of_recs = 32;
        odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array[32] = 0xFF;     
    }             
          
    max_ts_num = num_of_recs;
    
    /* Update the database for the ts_id */
    for (cal_entry = 0; cal_entry < num_of_recs; cal_entry++)
    {
        odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array[cal_entry] = temp_result.p_recs[cal_entry]->calendar_entry;
        PMC_LOG_TRACE("odtu_dmx_handle->var.ho_chnl_cfg[%u].cal_array[%u] = %u\n", chnl_id, cal_entry, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array[cal_entry]);
    }
    PMC_ATOMIC_YIELD(odtu_dmx_handle->base.parent_handle->parent_handle,0);       
    /* Update the database for the LO channel ID */
    /* ts could range from 0 - max num ts in HO */ /* upto ODU3 now */
    for (cal_entry = 0; cal_entry < num_of_recs; cal_entry++)
    {
        /* calendar entry is not assigned to any tributary slot/LO channel */
        /* update MO channel ID to 'calendar_entry' */
        new_rec = *temp_result.p_recs[cal_entry];
        new_rec.calendar_entry = temp_result.p_recs[cal_entry]->calendar_entry;
        if (ts_type == 0)
        {
            new_rec.ts_port_ho_mo = 0;
        }
        else {
            if (stage_inst == 1) 
            {
                new_rec.ts_port_ho_mo = 1;
            }
            else if (stage_inst == 2) {
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
        gen_db_update_record(&(odtu_dmx_handle->base),
                             odtu_dmx_handle->cfg.db_id,
                             odtu_dmx_handle->cfg.db_handle,
                             &new_rec);                                  
    }
    
    ret_code = ohfs_remove_chnl_prov(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                     chnl_id, ho_odu_type, 0, 
                                     UTIL_GLOBAL_NO_MAP, ts_type);
    if (ret_code != PMC_SUCCESS)
    {
        PMC_RETURN(ret_code);
    }
    
    /* OHFS_REMOVE: IPT configuration */
    for (cal_entry = 0; cal_entry < num_of_recs; cal_entry++)
    {   
        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            if (cal_entry < (num_of_recs/2))
            {
                /* configure IPT for the occupied tribslot */
                ret_code = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               temp_result.p_recs[cal_entry]->calendar_entry, 
                                               chnl_id, cal_entry, 1);
            }
            else {
                /* configure IPT for the occupied tribslot */
                ret_code = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               temp_result.p_recs[cal_entry]->calendar_entry, 
                                               chnl_id, cal_entry, 0);
            }
        }
        else {  
            /* configure IPT for the occupied tribslot */
            ret_code = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                           temp_result.p_recs[cal_entry]->calendar_entry, 
                                           chnl_id, cal_entry, 1);
        }
        
        /* Needs to be initialized on HO addition to ensure clean removal */
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = ohfs_remove_lo_chnl_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                  temp_result.p_recs[cal_entry]->calendar_entry, 0x7F);  
        }


        if (PMC_SUCCESS == ret_code)
        {
            ret_code = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                          temp_result.p_recs[cal_entry]->calendar_entry,
                                          UTIL_GLOBAL_NO_MAP, 0, 0, 0);
        } 
        
        /* MSI configuration; Set to Unallocated. */
        if (PMC_SUCCESS == ret_code)
        {
            if (ho_odu_type != UTIL_GLOBAL_ODU0 && ho_odu_type != UTIL_GLOBAL_ODUFLEX_CBR 
                && ho_odu_type != UTIL_GLOBAL_ODUFLEX_GFP)
            {
                ret_code = ohfs_remove_msi_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               temp_result.p_recs[cal_entry]->calendar_entry, 
                                               ho_odu_type, UTIL_GLOBAL_ODUK_DONT_CARE, 
                                               ts_type, 0);
            }
        }      


        if (PMC_SUCCESS == ret_code)
        {
            if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
            {
                ret_code = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                           temp_result.p_recs[cal_entry]->calendar_entry, 0, 0, 0); 
            }
        }                                                                                                   
    }  /* for loop */   


    if (PMC_SUCCESS == ret_code)
    {
        /* STS_SW HO channel init */
        ret_code = sts_sw_ho_chnl_init(odtu_dmx_handle->sts_sw_handle, chnl_id, ho_odu_type,
                                       ts_type, num_of_recs, 
                                       odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array);                                        
    }                                       
    
    if ((UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state == (UINT32)ODTU_DMX_CHNL_START)
    {                   
        /* update the state of the channel */
        chnl_state = ODTU_DMX_CHNL_EQUIPPED;
    }
    else {
        chnl_state = (odtu_dmx_chnl_state_t)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state;
    }        
    
    /* LO channel ID */
    PMC_LOG_TRACE("max_ts_num = %u\n",  max_ts_num);
    odtu_dmx_ho_chnl_var_update(odtu_dmx_handle, chnl_id, chnl_state, ho_odu_type,
                                max_ts_num, 
                                odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array,
                                0,
                                ts_type, 0);

    PMC_LOG_TRACE("odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].num_tribslot = %u\n", odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].num_tribslot);
    
    
    PMC_RETURN(PMC_SUCCESS);

} /* odtu_dmx_ho_chnl_prov */

/*******************************************************************************
*  odtu_dmx_ho_chnl_offline_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to provision a new HO channel in ODTU_DMX.
*
*   Note that prior to calling this API, the HO channel must be held in reset
*   by calling odtu_dmx_ho_chnl_deactivate() API. All corresponding
*   tributary slots and the associated LO channels being added to this HO
*   channel must be configured using odtu_dmx_lo_chnl_prov()
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   stage_inst          - Enum for ODTU_DMX instance. See odtu_dmx_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   ts_type             - size of tributary slot. See odu_line_payload_t
*                         ODU_UNCHANNELIZED  = 0,
*                         ODU_TS_1G25        = 1,
*                         ODU_TS_2G5         = 2,
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_offline_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 odtu_dmx_stage_instance_t stage_inst,
                                                 UINT32 chnl_id,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 cal_entry;
    line_core_otn_db_key key;
    UINT32 num_of_recs;
    line_core_otn_db_query_res *q_result;
    
    PMC_ENTRY();
    
    if (stage_inst != ODTU_DMX_STG_TWO)
    {
        /* get calendar entries for this HO channel */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = chnl_id;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else {
        /* get calendar entries for this HO channel */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    
    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                               odtu_dmx_handle->cfg.db_id,
                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }

    /* OHFS_REMOVE: IPT configuration */
    for (cal_entry = 0; cal_entry < num_of_recs; cal_entry++)
    {
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = ohfs_remove_lo_chnl_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                  q_result->p_recs[cal_entry]->calendar_entry, 0x7F);  
        }
        
        if (PMC_SUCCESS == ret_code)
        {                    
            ret_code = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                         q_result->p_recs[cal_entry]->calendar_entry,
                                         UTIL_GLOBAL_NO_MAP, 0, 0, 0);
        } 
        
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, chnl_id,
                                         q_result->p_recs[cal_entry]->calendar_entry,
                                         0x7F, 0, FALSE);   
        }  
    }        
   
                
    PMC_RETURN(ret_code);      
} /* odtu_dmx_ho_chnl_offline_update */  

/*******************************************************************************
* odtu_dmx_ho_chnl_switch_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_dmx_dmp_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
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
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_switch_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id,
                                             BOOL8 cfg_all)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if (chnl_id >= 96)
    {
        PMC_RETURN(ODTU_DMX_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG_TRACE("odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].ts_type = %u\n", odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].ts_type);

    PMC_ASSERT(odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array != NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
        
    PMC_LOG_TRACE("STS_SW configuration in odtu_dmx_ho_chnl_switch_cfg\n");
    PMC_ATOMIC_YIELD(odtu_dmx_handle->base.parent_handle->parent_handle,0);

    result = sts_sw_cfg(odtu_dmx_handle->sts_sw_handle, chnl_id, cfg_all);
    
    PMC_RETURN(result);
} /* odtu_dmx_ho_chnl_switch_cfg */

/*******************************************************************************
* odtu_dmx_ho_chnl_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_dmx_dmp_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
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
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_activate(odtu_dmx_handle_t *odtu_dmx_handle,
                                           UINT32 chnl_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (odtu_dmx_handle->var.prov_mode == 0
        && (UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_DMX_CHNL_EQUIPPED)
    {
        PMC_RETURN(ODTU_DMX_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    }
    else if (odtu_dmx_handle->var.prov_mode == 1
             && (UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state ==  (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
    {
        /* bulk provisioning. If HO is already activated then do nothing and exit */
        PMC_RETURN(ODTU_DMX_CHNL_OPERATIONAL);
    }
    
    if (PMC_SUCCESS == result)
    {
        /* HO Switch Config */
        result = sts_sw_ho_chnl_cfg(odtu_dmx_handle->sts_sw_handle, chnl_id);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = sts_sw_ho_chnl_ts_cfg(odtu_dmx_handle->sts_sw_handle, chnl_id);
    } 

    if (PMC_SUCCESS == result)
    {                                
        /* Channel reset is released by configuring HO_CH_RSTB to '1' */ 
        odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle, chnl_id, TRUE);
    }
    
    /* Update var context for a provisioned channel */
    odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_OPERATIONAL;
    
    PMC_LOG_TRACE("current dmx HO channel state = %u\n", odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state);
      
    PMC_RETURN(result);

} /* odtu_dmx_ho_chnl_activate */

/*******************************************************************************
* odtu_dmx_ho_chnl_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_dmx_dmp_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
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
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_deactivate(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (((UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_DMX_CHNL_START)
         && ((UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
         && ((UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state !=  (UINT32)ODTU_DMX_CHNL_PASSTHRU) )
    {
        PMC_RETURN(ODTU_DMX_CHNL_STATE_INVALID); /* that channel is not in provisioned state*/
    }
    
    if (PMC_SUCCESS == ret_val )
    {
        /* wait one full calendar cycle */
    PMC_Q_USLEEP(odtu_dmx_handle,1);
                                            
        /* Channel reset is released by configuring HO_CH_RSTB to '0' */
        odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle, chnl_id, FALSE);
    }
    
    if ((UINT32)odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
    {    
        /* Update var context for a provisioned channel */
        odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_EQUIPPED;
    }
    
    PMC_LOG_TRACE("current dmx chnl state is %u\n", odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state);
    PMC_RETURN(ret_val);

} /* odtu_dmx_ho_chnl_deactivate */


/*******************************************************************************
*  odtu_dmx_ho_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when removing a HO channel provisioned in ODTU_DMX.
*
*   All LO channels that belong to the HO being removed must be in reset.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   mux_stage           - enum for demux instance. See odtu_dmx_stage_instance_t.
*   chnl_id             - HO channel ID to be configured
*                         Valid range: 0 - 95
*   offline_update      - boolean to indidate if this is to clean the offline page
*                         TRUE  -- offline page update
*                         FALSE -- online page update
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_deprov(odtu_dmx_handle_t *odtu_dmx_handle,
                                         odtu_dmx_stage_instance_t mux_stage,
                                         UINT32 chnl_id,
                                         BOOL8 offline_update)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 cycle = 0;
    UINT32 num_tribslot;
    UINT32 i;

    odtu_dmx_chnl_state_t chnl_state;
    
    line_core_otn_db_key    key;
    UINT32            num_of_recs = 0;
    line_core_otn_db_query_res *q_result;

    PMC_ENTRY();

    PMC_ASSERT(odtu_dmx_handle != NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);

    num_tribslot = odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].num_tribslot;

    /* channel based configuration */
    /* put the HO channel in passthru */

    if (offline_update == FALSE
        && (odtu_dmx_handle->var.prov_mode == 0
            || (odtu_dmx_handle->var.prov_mode == 1 
                && odtu_dmx_handle->var.lo_chnl_cfg[chnl_id].state != UTIL_GLOBAL_CHNL_OPERATIONAL)))
    {
        result = odtu_dmx_chnl_init(odtu_dmx_handle, chnl_id, (odtu_dmx_chnl_mode_t)0);
    }
                                   
    if (PMC_SUCCESS == result)
    {
        result = ohfs_remove_ho_chnl_deprov(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, chnl_id);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = sts_sw_ho_chnl_deprov(odtu_dmx_handle->sts_sw_handle, chnl_id);
    }
            
    if (PMC_SUCCESS == result)
    {
        if (mux_stage != ODTU_DMX_STG_TWO)
        {
            /* read from DB to get calendar entries for the HO channel */
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = chnl_id;
            key.mo_channel = DO_NOT_USE_KEY;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
        }
        else {
            /* query the record to update */
            key.calendar_entry = DO_NOT_USE_KEY;
            key.ho_channel = DO_NOT_USE_KEY;
            key.mo_channel = chnl_id;
            key.lo_channel = DO_NOT_USE_KEY;
            key.trib_slot = DO_NOT_USE_KEY;
        }
                    
        /* find the total number of records that matches the HO channel ID */
        num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                   odtu_dmx_handle->cfg.db_id,
                                   odtu_dmx_handle->cfg.db_handle, (void *)&key,
                                   (void **)&q_result);
    }
    
    if (num_of_recs == 0) PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);      
        
    if (PMC_SUCCESS == result)
    {    
        for(i = 0; i < num_of_recs; i++)
        {
            cycle = q_result->p_recs[i]->calendar_entry;
            
            PMC_LOG_TRACE("cycle to unprov = %u\n", cycle);

            result = ohfs_remove_lo_chnl_id_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                 cycle, 0x7F);  
        
            if (PMC_SUCCESS == result)
            {                              
                result = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                            cycle, UTIL_GLOBAL_NO_MAP, 0, 0, 0);                                              
            }
            
            if (PMC_SUCCESS == result && offline_update == FALSE)
            {                                              
                result = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                             cycle, 0x7F, 0, 0);
            }
            
            if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
            {
                if (PMC_SUCCESS == result && offline_update == FALSE)
                {
                    result = mp_mgen_ipt_cfg(odtu_dmx_handle->rgmp_handle->mp_mgen_handle, 
                                             cycle, 0x7F, 0, 0);
                }
            }
            
            if (PMC_SUCCESS == result)
            {
                result = sts_sw_ho_calendar_cfg_deprov(odtu_dmx_handle->sts_sw_handle, cycle);
            }   
            
            if (PMC_SUCCESS == result && offline_update ==  TRUE)
            {
                result = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, chnl_id,
                                            cycle, 0x7F, 0, FALSE);   
            } 
        }
    }
    
    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, 0x7F);
                                                      
    /* update ho_chnl_var */
    chnl_state = ODTU_DMX_CHNL_START;

    odtu_dmx_ho_chnl_var_update(odtu_dmx_handle, chnl_id, chnl_state, 
                                UTIL_GLOBAL_ODUK_DONT_CARE, 0, NULL, NULL, LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, 0);
    PMC_MEMSET(&odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array[0], 0xFF, sizeof(odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array));

    PMC_RETURN(PMC_SUCCESS);
} /* odtu_dmx_ho_chnl_deprov */

/*******************************************************************************
* odtu_dmx_ho_null_frame_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates the HO channel that has been already provisioned.
*   Prior to calling this API, odtu_dmx_dmp_ho_chnl_prov() API must have been
*   called to provision a channel.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - HO ODU channel ID to be configured
*                         Valid range: 0 - 95
*   null_frame_mode     - set null frame mode 
*                         Valid range: 0 - 1
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
PUBLIC PMC_ERROR odtu_dmx_ho_null_frame_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 null_frame_mode)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = ohfs_remove_func_mode_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                       chnl_id, null_frame_mode);
    
    PMC_RETURN(result);
} /* odtu_dmx_ho_null_frame_cfg */

/*******************************************************************************
* odtu_dmx_activated_lo_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started LO channels
*                                                                               
* INPUTS:                                                                       
*   *odtu_dmx_handle      - pointer to ODTU_DMX handle instance
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
PUBLIC void odtu_dmx_activated_chnl_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                        BOOL8 *actv_ho_chnl,
                                        BOOL8 *actv_lo_chnl)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_dmx_handle != NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    
    for (itr = 0; itr < ODTU_DMX_NUM_CHANL; itr++)
    {
        UINT32 ho_value = odtu_dmx_field_HO_CHNL_RSTB_get(NULL, odtu_dmx_handle, itr);
        actv_ho_chnl[itr] = FALSE;

        if (0 != ho_value)
        {
            /*
             * Assign the result of the HO_CHNL_RSTB register to the 
             * actv_ho_chnl array. However if the SW ho_chnl_cfg state
             * is start but the lo level state is OPERATIONAL, indicating
             * that there was previously a mid order and lo order that 
             * were using the same channel value but not in the same MO
             * container, then ensure that the HO is not set to TRUE.
             */
            if (!(UTIL_GLOBAL_CHNL_START == odtu_dmx_handle->var.ho_chnl_cfg[itr].state &&
                  UTIL_GLOBAL_CHNL_OPERATIONAL == odtu_dmx_handle->var.lo_chnl_cfg[itr].state))
            {
                actv_ho_chnl[itr] = TRUE;
            }
        }
        actv_lo_chnl[itr] = odtu_dmx_field_LO_CHNL_RSTB_get(NULL, odtu_dmx_handle, itr)==0?FALSE:TRUE;
    }

    PMC_RETURN();
}  /* odtu_dmx_activated_chnl_get */

/*******************************************************************************
*  odtu_dmx_chnl_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the operational mode of an HO channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - channel ID to be configured (HO or MO) \n
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   odtu_dmx_chnl_mode_t - operational mode of the HO ODU channel 
*
*******************************************************************************/
PUBLIC odtu_dmx_chnl_mode_t odtu_dmx_chnl_mode_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 chnl_id)
{
    BOOL8 passthru, non_intrusive_mode, null_frame_mode;    
    odtu_dmx_chnl_mode_t mode;
    
    PMC_ENTRY();

    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    passthru           = odtu_dmx_field_PASSTHRU_get(NULL, odtu_dmx_handle, chnl_id);;
    non_intrusive_mode = odtu_dmx_field_NON_INTRUSVE_MODE_get(NULL, odtu_dmx_handle, chnl_id);

    if (TRUE ==  passthru)
    {
        mode = ODTU_DMX_CHNL_UNCHANNELIZED;            
    } else 
    {
        if (TRUE ==  non_intrusive_mode)
        {
            mode = ODTU_DMX_CHNL_NON_INTRUSIVE;
        } else 
        {
            null_frame_mode    = ohfs_remove_null_frame_mode_get(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, chnl_id);
            if (FALSE == null_frame_mode)
            {
                mode = ODTU_DMX_CHNL_CHANNELIZED;
            } else 
            {
                mode = ODTU_DMX_CHNL_NULL_FRAME_MODE;
            }
        }
    }
        
    PMC_RETURN(mode);
} /* odtu_dmx_chnl_mode_get */

/*******************************************************************************
*  odtu_dmx_dummy_schd_prov_num_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the number of dummy calendar that has been scheduled
*   for MO ODU3 channels in the DMX. 
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*
* OUTPUTS:
*   *num_dummy_schd     - number of scheduled dummy calendar for MO ODU3
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_dummy_schd_prov_num_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 *num_dummy_schd)
{    
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    UINT32 i;
    UINT32 dummy_schd_prov_num = 0;
    
    for (i = 0; i < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; i++)
    {
        if (odtu_dmx_handle->var.dummy_schd_ctxt[i].dummy_ch_id != 0x7F
            && odtu_dmx_handle->var.dummy_schd_ctxt[i].dummy_ch_id < 96)
        {
            /* dummy calendar has been scheduled */
            dummy_schd_prov_num++;
        }
    }
   
    *num_dummy_schd = dummy_schd_prov_num;     

    PMC_RETURN(result);
} /* odtu_dmx_dummy_schd_prov_num_get */

/*******************************************************************************
* odtu_dmx_ho_ch_id_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started LO channels
*                                                                               
* INPUTS:                                                                       
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
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
PUBLIC void odtu_dmx_ho_ch_id_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                  UINT32 lo_chnl_id,
                                  UINT32 *ho_chnl_id)
{
    PMC_ENTRY();
    
    PMC_ASSERT(odtu_dmx_handle != NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    
    *ho_chnl_id = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ho_chnl_id;
    

    PMC_RETURN();
}  /* odtu_dmx_activated_chnl_get */


/*******************************************************************************
*  odtu_dmx_ho_config_addr_st_id_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets HO_CONFIG_ADDR_ST_ID to the current HO ID that needs to 
*   have its page swapped.. 
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_ch_id             - HO channel ID.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ho_config_addr_st_id_set(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 ho_ch_id)
{    
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_ch_id);
    
    PMC_RETURN(result);
} /* odtu_dmx_ho_config_addr_st_id_set */                                                

/*******************************************************************************
* odtu_dmx_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function put in place the cleaned var_t context. The register and 
*   memory context of channels that are not in OPERATIONAL state are cleaned 
*
* INPUTS:
*   *odtu_dmx_handle        - pointer to MAPOTN handle instance to be operated on
*   *odtu_dmx_var_reg       - A partial odtu_dmx_var_t context + additional
*                           information to facilite audit phase
*   *odtu_dmx_var_clean     - A clean context which represents the intersection
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
PUBLIC PMC_ERROR odtu_dmx_ctxt_cleanup(odtu_dmx_handle_t *odtu_dmx_handle,
                                       odtu_dmx_recover_var_t *odtu_dmx_var_reg,
                                       odtu_dmx_var_t *odtu_dmx_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32  ho_chnl_to_clean[ODTU_DMX_NUM_CHANL],lo_chnl_to_clean[ODTU_DMX_NUM_CHANL];
    UINT32 num_ho_chnl_to_clean,num_lo_chnl_to_clean, ho_chnl_id, num_ho_unchannelized_cycle;      
    UINT32  cleaned_cycle[ODTU_DMX_NUM_CHANL], mem_unchannelized_cycle[ODTU_DMX_NUM_CHANL];
    UINT32  num_cleaned_cycle;
    UINT32 config_update_itr UNUSED, cycle_itr UNUSED;
    UINT32 ho_chnl_itr,lo_chnl_itr, ho_unchannelized_cycle_itr UNUSED;
    BOOL8 config_update_done UNUSED;
    BOOL8 set_start_state = FALSE;

    PMC_ENTRY();

    /* keep list of unchannlized cycle */
    num_ho_unchannelized_cycle = 0;
    for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_DMX_NUM_CHANL;ho_chnl_itr++)
    {
        if (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (util_global_chnl_state_t)ODTU_DMX_CHNL_OPERATIONAL &&
            odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
        {
            mem_unchannelized_cycle[num_ho_unchannelized_cycle] = ho_chnl_itr;
            num_ho_unchannelized_cycle += 1;
        }
    }

    /* 
       iterate over HO channel 
         - build the list of HO channels to clean 
         - PUT ALL HO Channel in NULL_FRAME mode
    */
    num_ho_chnl_to_clean = 0;
    for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_DMX_NUM_CHANL;ho_chnl_itr++)
    {
        if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO || ho_chnl_itr <12) 
        { 
            if ((UINT32) ODTU_DMX_CHNL_START == odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].state && 
                (UINT32) ODTU_DMX_CHNL_OPERATIONAL != odtu_dmx_var_clean->lo_chnl_cfg[ho_chnl_itr].state)
            {
                ho_chnl_to_clean[num_ho_chnl_to_clean] = ho_chnl_itr;
                num_ho_chnl_to_clean += 1;                
            } 
        } 
        
    }
    
    num_lo_chnl_to_clean = 0;
    PMC_MEMSET(lo_chnl_to_clean,0,sizeof(lo_chnl_to_clean));
    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_DMX_NUM_CHANL;lo_chnl_itr++)
    {        

        if ((UINT32) ODTU_DMX_CHNL_START == odtu_dmx_var_clean->lo_chnl_cfg[lo_chnl_itr].state)
        {
            set_start_state = FALSE;
            ho_chnl_id = odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id;
            if (ho_chnl_id >= ODTU_DMX_NUM_CHANL)
            {
                set_start_state = FALSE;
            } else 
            {
                if (!(UTIL_GLOBAL_ODU_UNCHANNELIZED == odtu_dmx_var_reg->var.ho_chnl_cfg[lo_chnl_itr].ts_type &&
                      (UINT32) ODTU_DMX_CHNL_OPERATIONAL == odtu_dmx_var_clean->ho_chnl_cfg[lo_chnl_itr].state))
                {
                    set_start_state = TRUE;
                }
            }
            if (set_start_state == TRUE)
            {
                lo_chnl_to_clean[num_lo_chnl_to_clean] = lo_chnl_itr;
                num_lo_chnl_to_clean += 1;
            }
        }
    }
    if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == FALSE)
    {
        /* copy mp_mgen_entries */        
        PMC_MEMCPY(&odtu_dmx_var_clean->mp_mpmgen_avl_ipt[0],
                   &odtu_dmx_handle->var.mp_mpmgen_avl_ipt[0],sizeof(odtu_dmx_handle->var.mp_mpmgen_avl_ipt));
    }
        
    /* the old mapotn var context is incoherent, we use the new one */
    PMC_MEMCPY(&odtu_dmx_handle->var,odtu_dmx_var_clean,sizeof(odtu_dmx_var_t));
  
    /* IMPORTANT
       to perform a channel clean, we need to bring the HO channel out of reset 
     */
      
    /* get cycle to clean */
    ohfs_remove_chnl_cycle_clean_get(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                     ho_chnl_to_clean,num_ho_chnl_to_clean,                                 
                                     lo_chnl_to_clean,num_lo_chnl_to_clean,                                 
                                     cleaned_cycle,&num_cleaned_cycle);

    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean;ho_chnl_itr++)
    {       
        /* be sure that the HO channel is out of reset */
        odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr], TRUE);
        
        for (lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl_to_clean;lo_chnl_itr++)
        {
            {                  
                /* deactivate the LO channel */
                odtu_dmx_lo_chnl_reset_cfg(odtu_dmx_handle, lo_chnl_to_clean[lo_chnl_itr], FALSE); 
                if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == FALSE)
                {
                    odtu_dmx_deprov_mgen_ipt_entry(odtu_dmx_handle,lo_chnl_to_clean[lo_chnl_itr]);                    
                }
#ifdef PMC_INTERNAL_DEBUG
                /* try this one */
                rc = odtu_dmx_lo_chnl_deprov(odtu_dmx_handle,odtu_dmx_var_reg->dmx_stage, lo_chnl_to_clean[lo_chnl_itr], ho_chnl_to_clean[ho_chnl_itr], 0, FALSE, 0x7F);
                if (PMC_SUCCESS != rc)
                {
                    rc = PMC_SUCCESS;
                    continue;
                }
                
#else
                /* associate this LO to the HO channel */
                odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr]);
                (void) rxjc_proc_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle, lo_chnl_to_clean[lo_chnl_itr]);
                (void) cpact_obuf_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->cpact_obuf_handle, lo_chnl_to_clean[lo_chnl_itr]);

                if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == FALSE)
                {  
                    (void) mp_mgen_chnl_clean(odtu_dmx_handle->rgmp_handle->mp_mgen_handle,&lo_chnl_to_clean[lo_chnl_itr],1);
                }
                odtu_dmx_lo_chnl_var_update(odtu_dmx_handle,lo_chnl_to_clean[lo_chnl_itr],
                                            ho_chnl_to_clean[ho_chnl_itr],ODTU_DMX_CHNL_START,
                                            UTIL_GLOBAL_ODUK_DONT_CARE, UTIL_GLOBAL_ODUK_DONT_CARE, 0, 
                                            NULL, LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, UTIL_GLOBAL_NO_MAP, 0);

                /* cycle cleanup */
                if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE) 
                {                     
                    mp_mgen_cycle_clean(odtu_dmx_handle->rgmp_handle->mp_mgen_handle,cleaned_cycle,num_cleaned_cycle);
                }
              
                sts_sw_lo_calendar_cfg_clean(odtu_dmx_handle->sts_sw_handle,cleaned_cycle,num_cleaned_cycle);
                ohfs_remove_lo_cycle_clean(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,cleaned_cycle,num_cleaned_cycle);
                    
#endif
                   
                if (PMC_SUCCESS == rc)
                    rc = odtu_dmx_ho_chnl_switch_cfg(odtu_dmx_handle,  ho_chnl_to_clean[ho_chnl_itr], FALSE);
                if (PMC_SUCCESS == rc)
                {
                    /* perform page swap */
                    rc = odtu_dmx_reconfig_mode_cfg(odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr], 0);
                }
                
                if (PMC_SUCCESS == rc)
                {
                    rc = odtu_dmx_reconfig_status_update(odtu_dmx_handle,ho_chnl_to_clean[ho_chnl_itr], 1);
                }
                /* poll config_update */
                if (PMC_SUCCESS == rc)
                {
                    rc = odtu_dmx_config_update_poll(odtu_dmx_handle,ho_chnl_to_clean[ho_chnl_itr], 1000);

                    if (PMC_SUCCESS != rc)
                    {
                        rc = PMC_SUCCESS;
                        /* skip this one */
                        continue;
                    }
                }
                if (PMC_SUCCESS == rc)
                {
                    /* read once to see what the value is  */
                    rc = odtu_dmx_config_update_done_poll(odtu_dmx_handle,
                                                          ho_chnl_to_clean[ho_chnl_itr], 1000);
                }
                                
                PMC_Q_USLEEP(odtu_dmx_handle,120);
                /*
                ** update offline page 
                */
                if (PMC_SUCCESS == rc)
                {
#ifdef PMC_INTERNAL_DEBUG
                    /* try this one */
                    rc = odtu_dmx_lo_chnl_deprov(odtu_dmx_handle,odtu_dmx_var_reg->dmx_stage, lo_chnl_to_clean[lo_chnl_itr], ho_chnl_to_clean[ho_chnl_itr], 1, FALSE, 0x7F);
#else
                    /* associate this LO to the HO channel */
                    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr]);
                    (void) rxjc_proc_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle, lo_chnl_to_clean[lo_chnl_itr]);
                    (void) cpact_obuf_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->cpact_obuf_handle, lo_chnl_to_clean[lo_chnl_itr]);
                    if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == FALSE)
                    {  
                        (void) mp_mgen_chnl_clean(odtu_dmx_handle->rgmp_handle->mp_mgen_handle,&lo_chnl_to_clean[lo_chnl_itr],1);
                    }
                    odtu_dmx_lo_chnl_var_update(odtu_dmx_handle,lo_chnl_to_clean[lo_chnl_itr],
                                                ho_chnl_to_clean[ho_chnl_itr],ODTU_DMX_CHNL_START,
                                                UTIL_GLOBAL_ODUK_DONT_CARE, UTIL_GLOBAL_ODUK_DONT_CARE, 0, 
                                                NULL, LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD, UTIL_GLOBAL_NO_MAP, 0);
                        
                    /* cycle cleanup */
                    if (pmc_is_digi_rev_a_revision(&odtu_dmx_handle->base) == TRUE)
                    {
                        mp_mgen_cycle_clean(odtu_dmx_handle->rgmp_handle->mp_mgen_handle,cleaned_cycle,num_cleaned_cycle);
                    }
                    sts_sw_lo_calendar_cfg_clean(odtu_dmx_handle->sts_sw_handle,cleaned_cycle,num_cleaned_cycle);
                    ohfs_remove_lo_cycle_clean(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,cleaned_cycle,num_cleaned_cycle);
#endif

                }

            }
        }
    }



    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, 0x7F);

    /* clean HO */
     sts_sw_ho_clean(odtu_dmx_handle->sts_sw_handle,ho_chnl_to_clean,num_ho_chnl_to_clean,   
                     lo_chnl_to_clean,num_lo_chnl_to_clean);  
    if (odtu_dmx_var_reg->dmx_stage ==  ODTU_DMX_STG_TWO)
    {
        odtu_dmx_dummy_schd_clean(odtu_dmx_handle);
            
    }    
    ohfs_remove_chnl_cycle_clean(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                 ho_chnl_to_clean,num_ho_chnl_to_clean,                                 
                                 lo_chnl_to_clean,num_lo_chnl_to_clean);


    /* put HO channel in reset */
    for (ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl_to_clean;ho_chnl_itr++)
    {
        /* rerun switch config */
        /* (void) odtu_dmx_ho_chnl_switch_cfg(odtu_dmx_handle,ho_chnl_to_clean[ho_chnl_itr]); */
        (void) odtu_dmx_chnl_init(odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr], (odtu_dmx_chnl_mode_t)0);
        rgmp_var_update(odtu_dmx_handle->rgmp_handle, ho_chnl_to_clean[ho_chnl_itr], (util_global_chnl_state_t)RGMP_CHNL_START);
        odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr], FALSE); 

        /* NULL FRAME mode */    
        (void) odtu_dmx_ho_null_frame_cfg(odtu_dmx_handle, ho_chnl_to_clean[ho_chnl_itr], 0);      
        PMC_MEMSET(&odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_to_clean[ho_chnl_itr]].cal_array[0], 0xFF, sizeof(odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_to_clean[ho_chnl_itr]].cal_array));    
    }
        
    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, 0x7F);
 
    /* the old mapotn var context is incoherent, we use the new one */
    PMC_MEMCPY(&odtu_dmx_var_reg->var,odtu_dmx_var_clean,sizeof(odtu_dmx_var_t));

    PMC_RETURN(rc);
} /* odtu_dmx_ctxt_cleanup */

/*******************************************************************************
*  odtu_dmx_first_valid_dummy_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODU_RFRM var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODU_RFRM instance to be operated on
*   first_cycle         - tributary slot number
*
* OUTPUTS:
*   *valid_null_cycle   - first valid null cycle 
*   *dummy_idx          - index for the dummy schedule
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_first_valid_dummy_find(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 first_cycle, 
                                                  UINT32 *valid_null_cycle,
                                                  UINT32 *dummy_idx)
{
    UINT32 i = 0;
    UINT32 j;
    UINT32 start_idx;
    UINT32 dmx_dummy_schd_array[1][UTIL_GLOBAL_MAX_NUM_DUMMY_SCHD] = UTIL_GLOBAL_DMX_DUMMY_SCHD_ARRAY;
    
    PMC_ENTRY();
    
    *valid_null_cycle = 0;

    if (TRUE == pmc_coreotn_schd_extra_channel_revision(&odtu_dmx_handle->base))
    {
        start_idx = 2;
    }
    else
    {
        start_idx = 0;
    }
        
    for (i = start_idx; i < UTIL_GLOBAL_MAX_NUM_DUMMY_SCHD; i++)
    {
        if (dmx_dummy_schd_array[0][i] > first_cycle && *valid_null_cycle == 0)
        {
            *valid_null_cycle = dmx_dummy_schd_array[0][i];
            
            for (j = 0; j < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; j++)
            {
                if (odtu_dmx_handle->var.dummy_schd_ctxt[j].dummy_cycle == 0
                    && odtu_dmx_handle->var.dummy_schd_ctxt[j].dummy_state == FALSE)
                {
                    /* empty index */
                    *dummy_idx = j;
                }
                else if (odtu_dmx_handle->var.dummy_schd_ctxt[j].dummy_cycle == *valid_null_cycle
                         && odtu_dmx_handle->var.dummy_schd_ctxt[j].dummy_state == TRUE)
                {
                    /* already used for another channel, pick the next null */
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
} /* odtu_dmx_first_valid_dummy_find */

/*******************************************************************************
*  odtu_dmx_prov_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the provision mode of adding LO channels. 
*
* INPUTS:
*   *odtu_dmx_handle    - handle to the ODTU_DMX instance to be operated on
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
PUBLIC void odtu_dmx_prov_mode_set(odtu_dmx_handle_t *odtu_dmx_handle,
                                   BOOL8 prov_mode)
{
    PMC_ENTRY();
    
    /* prov_mode == 0: normal prov
    ** prov_mode == 1: super prov
    */
    odtu_dmx_handle->var.prov_mode = prov_mode;
   
    PMC_RETURN();
       
} /* odtu_dmx_prov_mode_set */ 


/*******************************************************************************
* FUNCTION: odtu_dmx_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID
*   non_null_ptr      - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_int_validate(odtu_dmx_handle_t    *odtu_dmx_handle,
                                       UINT32                chnl_id,
                                       void                 *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odtu_dmx_handle, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
   
    if (chnl_id >= ODTU_DMX_NUM_CHANL)
    {
        result = ODTU_DMX_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = ODTU_DMX_LOG_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* odtu_dmx_int_validate */



/*******************************************************************************
* FUNCTION: odtu_dmx_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   int_table_ptr     - pointer to interrupt table with bits set to enable
*                       interrupts. If NULL, enables/disables all interrupts
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
PUBLIC PMC_ERROR odtu_dmx_int_enable(odtu_dmx_handle_t    *odtu_dmx_handle,
                                     odtu_dmx_int_t       *int_table_ptr,
                                     odtu_dmx_int_t       *int_en_table_ptr,
                                     BOOL8                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != odtu_dmx_handle, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != int_en_table_ptr, ODTU_DMX_ERR_INVALID_PTR, 0, 0);


    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_ENABLE_SINGLE
    
    ODTU_DMX_INT_TABLE_DEFINE();
    

    PMC_RETURN(result);
} /* odtu_dmx_int_enable */



/*******************************************************************************
* FUNCTION: odtu_dmx_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_int_clear(odtu_dmx_handle_t     *odtu_dmx_handle,
                                    odtu_dmx_int_t        *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != odtu_dmx_handle, ODTU_DMX_ERR_INVALID_PTR, 0, 0);


    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_CLEAR_SINGLE

    ODTU_DMX_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* odtu_dmx_int_clear */




/*******************************************************************************
* FUNCTION: odtu_dmx_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odtu_dmx_int_retrieve(odtu_dmx_handle_t      *odtu_dmx_handle,
                                       odtu_dmx_int_t         *filt_table_ptr,
                                       odtu_dmx_int_t         *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    odtu_dmx_buffer_t   *odtu_dmx_buffer_ptr = NULL;

    PMC_ENTRY();

    PMC_ASSERT(NULL != odtu_dmx_handle, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != int_table_ptr, ODTU_DMX_ERR_INVALID_PTR, 0, 0);

    
    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    ODTU_DMX_INT_TABLE_DEFINE();

    #ifdef ODTU_DMX_STATUS_SINGLE
    #undef ODTU_DMX_STATUS_SINGLE
    #endif
    #define ODTU_DMX_STATUS_SINGLE UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
    
    ODTU_DMX_STATUS_TABLE_DEFINE();
    

    PMC_RETURN(result);
} /* odtu_dmx_int_retrieve */



/*******************************************************************************
* FUNCTION: odtu_dmx_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odtu_dmx_int_enabled_check(odtu_dmx_handle_t     *odtu_dmx_handle,
                                            odtu_dmx_int_t        *int_en_table_ptr,
                                            BOOL8                 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != odtu_dmx_handle, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(NULL != int_found_ptr, ODTU_DMX_ERR_INVALID_PTR, 0, 0);


    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_CHECK_SINGLE
    
    ODTU_DMX_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* odtu_dmx_int_enabled_check */



/*******************************************************************************
* FUNCTION: odtu_dmx_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   chnl_info         - structure descrbing the channel: chnl id, calendar entries
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
PUBLIC PMC_ERROR odtu_dmx_int_chnl_enable(odtu_dmx_handle_t         *odtu_dmx_handle,
                                          odtu_dmx_int_chnl_info_t  *chnl_info,
                                          odtu_dmx_int_chnl_t       *int_table_ptr,
                                          odtu_dmx_int_chnl_t       *int_en_table_ptr,
                                          BOOL8                      enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1  = chnl_info->chnl_id;
    UINT32          link2  = chnl_info->chnl_id;

    PMC_ENTRY();

    result = odtu_dmx_int_validate(odtu_dmx_handle, chnl_info->chnl_id, int_en_table_ptr);

    #ifdef ODTU_DMX_INT
    #undef ODTU_DMX_INT
    #endif
    #define ODTU_DMX_INT UTIL_GLOBAL_INT_ENABLE_RANGE
    
    #ifdef ODTU_DMX_INT_TRIB
    #undef ODTU_DMX_INT_TRIB
    #endif
    #define ODTU_DMX_INT_TRIB UTIL_GLOBAL_INT_ODTU_DMX_ENABLE
    
    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_ENABLE_SINGLE
    
    if (TRUE == chnl_info->higher_order)
    {
        ODTU_DMX_INT_TABLE_HO_DEFINE();
    }
    else
    {
        ODTU_DMX_INT_TABLE_LO_DEFINE();
    }
    
    

    PMC_RETURN(result);
} /* odtu_dmx_int_chnl_enable */




/*******************************************************************************
* FUNCTION: odtu_dmx_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   chnl_info         - structure descrbing the channel: chnl id, calendar entries
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_int_chnl_clear(odtu_dmx_handle_t          *odtu_dmx_handle,
                                         odtu_dmx_int_chnl_info_t   *chnl_info,
                                         odtu_dmx_int_chnl_t        *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1  = chnl_info->chnl_id;
    UINT32          link2  = chnl_info->chnl_id;

    PMC_ENTRY();

    result = odtu_dmx_int_validate(odtu_dmx_handle, chnl_info->chnl_id, chnl_info);

    #ifdef ODTU_DMX_INT
    #undef ODTU_DMX_INT
    #endif
    #define ODTU_DMX_INT UTIL_GLOBAL_INT_CLEAR_RANGE 
    
    #ifdef ODTU_DMX_INT_TRIB
    #undef ODTU_DMX_INT_TRIB
    #endif
    #define ODTU_DMX_INT_TRIB UTIL_GLOBAL_INT_ODTU_DMX_CLEAR
    
    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_CLEAR_SINGLE

    
    if (TRUE == chnl_info->higher_order)
    {
        ODTU_DMX_INT_TABLE_HO_DEFINE();
    }
    else
    {
        ODTU_DMX_INT_TABLE_LO_DEFINE();
    }
   


    PMC_RETURN(result);
} /* odtu_dmx_int_chnl_clear */




/*******************************************************************************
* FUNCTION: odtu_dmx_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   chnl_info         - structure descrbing the channel: chnl id, calendar entries
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
PUBLIC PMC_ERROR odtu_dmx_int_chnl_retrieve(odtu_dmx_handle_t           *odtu_dmx_handle,
                                            odtu_dmx_int_chnl_info_t    *chnl_info,
                                            odtu_dmx_int_chnl_t         *filt_table_ptr,
                                            odtu_dmx_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_info->chnl_id;
    UINT32          link2 = chnl_info->chnl_id;
    odtu_dmx_buffer_t   *odtu_dmx_buffer_ptr = NULL;

    PMC_ENTRY();

    result = odtu_dmx_int_validate(odtu_dmx_handle, chnl_info->chnl_id, int_table_ptr);

    
    #ifdef ODTU_DMX_INT
    #undef ODTU_DMX_INT
    #endif
    #define ODTU_DMX_INT UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    #ifdef ODTU_DMX_INT_TRIB
    #undef ODTU_DMX_INT_TRIB
    #endif
    #define ODTU_DMX_INT_TRIB UTIL_GLOBAL_INT_ODTU_DMX_RETRIEVE
    
   

    #ifdef ODTU_DMX_STATUS
    #undef ODTU_DMX_STATUS
    #endif
    #define ODTU_DMX_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    
    #ifdef ODTU_DMX_STATUS_TRIB
    #undef ODTU_DMX_STATUS_TRIB
    #endif
    #define ODTU_DMX_STATUS_TRIB UTIL_GLOBAL_STATUS_ODTU_DMX_RETRIEVE


    #ifdef ODTU_DMX_STATUS_SINGLE
    #undef ODTU_DMX_STATUS_SINGLE
    #endif
    #define ODTU_DMX_STATUS_SINGLE UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE
    
    
    if (TRUE == chnl_info->higher_order)
    {
        ODTU_DMX_INT_TABLE_HO_DEFINE();
        ODTU_DMX_STATUS_TABLE_HO_DEFINE();
    }
    else
    {
        ODTU_DMX_INT_TABLE_LO_DEFINE();
        ODTU_DMX_STATUS_TABLE_LO_DEFINE();
    }
    
    


    PMC_RETURN(result);
} /* odtu_dmx_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: odtu_dmx_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odtu_dmx_handle   - Pointer to subsystem handle
*   chnl_info         - structure descrbing the channel: chnl id, calendar entries
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
PUBLIC PMC_ERROR odtu_dmx_int_chnl_enabled_check(odtu_dmx_handle_t          *odtu_dmx_handle,
                                                 odtu_dmx_int_chnl_info_t   *chnl_info,
                                                 odtu_dmx_int_chnl_t        *int_en_table_ptr,
                                                 BOOL8                      *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1  = chnl_info->chnl_id;
    UINT32          link2  = chnl_info->chnl_id;

    PMC_ENTRY();

    result = odtu_dmx_int_validate(odtu_dmx_handle, chnl_info->chnl_id, int_en_table_ptr);


    #ifdef ODTU_DMX_INT
    #undef ODTU_DMX_INT
    #endif
    #define ODTU_DMX_INT UTIL_GLOBAL_INT_CHECK_RANGE
    
    #ifdef ODTU_DMX_INT_TRIB
    #undef ODTU_DMX_INT_TRIB
    #endif
    #define ODTU_DMX_INT_TRIB UTIL_GLOBAL_INT_ODTU_DMX_CHECK
    
    #ifdef ODTU_DMX_INT_SINGLE
    #undef ODTU_DMX_INT_SINGLE
    #endif
    #define ODTU_DMX_INT_SINGLE UTIL_GLOBAL_INT_CHECK_SINGLE

    if (TRUE == chnl_info->higher_order)
    {
        ODTU_DMX_INT_TABLE_HO_DEFINE();
    }
    else
    {
        ODTU_DMX_INT_TABLE_LO_DEFINE();
    }




    PMC_RETURN(result);
} /* odtu_dmx_int_chnl_enabled_check */


/*******************************************************************************
*  odtu_dmx_dummy_schd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function schedules a calendar entry to be assigned for a MO channel. 
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   dmx_schd_idx         - dummy schedule channel instance. Valid range: 0 - 3
*
* OUTPUTS:
*   *mo_chnl_id          - MO channel ID that requires an additional calendar entry.
*                          Valid range: 0 - 95
*   *calendar_entry      - Calendar entry to be assigned for the additional scheduling
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_dummy_schd_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                         UINT32 dmx_schd_idx,
                                         UINT8 *mo_chnl_id,
                                         UINT8 *calendar_entry)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
             
    if (odtu_dmx_handle->var.dummy_schd_ctxt[dmx_schd_idx].dummy_state == TRUE)
    {
        *mo_chnl_id = odtu_dmx_handle->var.dummy_schd_ctxt[dmx_schd_idx].dummy_ch_id; 
        *calendar_entry = odtu_dmx_handle->var.dummy_schd_ctxt[dmx_schd_idx].dummy_cycle;
    }
    
    PMC_RETURN(result);
} /* odtu_dmx_dummy_schd_get */                                     
                                   
                                   
                                                                    
/*******************************************************************************
*  odtu_dmx_dummy_schd_entry_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function resets the dummy calendar entries used for MO ODU3 in the
*   second stage demux. 
*
* INPUTS:
*   *odtu_dmx_handle    - handle to the ODTU_DMX instance to be operated on
*   chnl_id             - MO channel ID that requires an additional calendar entry.
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_dummy_schd_entry_reset(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 chnl_id)
{
    PMC_ENTRY();
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;
        
    for (i = 0; i < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; i++)
    {
        if (odtu_dmx_handle->var.dummy_schd_ctxt[i].dummy_ch_id == chnl_id  
            && odtu_dmx_handle->var.dummy_schd_ctxt[i].dummy_state == TRUE)
        {
            result = odtu_dmx_dummy_schd_cfg(odtu_dmx_handle, i, 0x7F, 0, FALSE);
        }
    } 
    
    PMC_RETURN(result);
} /* odtu_dmx_dummy_schd_state_test */

/*******************************************************************************
*  odtu_dmx_temp_mo_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to provision a temporary MO channel in FMF2::ODTU_DMX
*   prior to making the MO in FMF1::ODTU_DMX operational.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   stage_inst          - enum for demux stage instance. See odtu_dmx_stage_instance_t.
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_odu_type         - Enum, HO ODU rate. See util_global_odukp_type_t.
*   ts_type             - size of tributary slot. See odu_line_payload_t.
*                         ODU_UNCHANNELIZED  = 0,
*                         ODU_TS_1G25        = 1,
*                         ODU_TS_2G5         = 2,
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_temp_mo_chnl_prov(odtu_dmx_handle_t *odtu_dmx_handle,
                                            odtu_dmx_stage_instance_t stage_inst,
                                            UINT32 chnl_id,
                                            util_global_odukp_type_t ho_odu_type,
                                            util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    
    UINT32 i;
    line_core_otn_db_key key;
    UINT32 num_of_recs;
    line_core_otn_db_query_res *q_result;
    UINT32 cal_entry[96];
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl_id > 95 || stage_inst > 2 || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD)
    {
        PMC_RETURN(ODTU_DMX_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* initialize calendar array */
    for (i = 0; i < 96; i++)
    {   
        cal_entry[i] = 0x7F;
    }

    if (stage_inst != ODTU_DMX_STG_TWO)
    {
        /* get calendar entries for this HO channel */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = chnl_id;
        key.mo_channel = DO_NOT_USE_KEY;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    else {
        /* get calendar entries for this HO channel */
        /* query the record to update */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.ho_channel = DO_NOT_USE_KEY;
        key.mo_channel = chnl_id;
        key.lo_channel = DO_NOT_USE_KEY;
        key.trib_slot = DO_NOT_USE_KEY;
    }
    
    /* find the total number of records that matches the HO channel ID */
    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                               odtu_dmx_handle->cfg.db_id,
                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);

    PMC_LOG_TRACE("num_of_recs for HO chnl_prov  = %u  \n", num_of_recs);
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }

    
    if ((ho_odu_type == UTIL_GLOBAL_ODU3E1 || ho_odu_type == UTIL_GLOBAL_ODU3E2) 
        && stage_inst == ODTU_DMX_STG_TWO && num_of_recs == 33)
    {
        num_of_recs = 32;
    }
        
    for (i = 0; i < num_of_recs; i++)
    {   
        cal_entry[i] = q_result->p_recs[i]->calendar_entry;

        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            if (i < (num_of_recs/2))
            {
                /* configure IPT for the occupied tribslot */
                ret_code = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               q_result->p_recs[i]->calendar_entry, 
                                               chnl_id, i, 1);
            }
            else {
                /* configure IPT for the occupied tribslot */
                ret_code = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               q_result->p_recs[i]->calendar_entry, 
                                               chnl_id, i, 0);
            }
        }
        else {  
            /* configure IPT for the occupied tribslot */
            ret_code = ohfs_remove_ipt_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                           q_result->p_recs[i]->calendar_entry, 
                                           chnl_id, i, 1);
        }   
    }

    if (PMC_SUCCESS == ret_code)
    {
        ret_code = sts_sw_ho_calendar_null_cfg(odtu_dmx_handle->sts_sw_handle, 
                                               chnl_id, num_of_recs,
                                               cal_entry,
                                               0x1);
    }
        
    if (PMC_SUCCESS == ret_code)
    {
        odtu_dmx_field_PASSTHRU_set(NULL, odtu_dmx_handle, chnl_id, FALSE);
        
        ret_code = odtu_dmx_ho_null_frame_cfg(odtu_dmx_handle, chnl_id, 1);

        if (PMC_SUCCESS == ret_code)
        {                                            
            ret_code = odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle,
                                                  chnl_id, 1);
        }            
    }
    
    PMC_RETURN(ret_code);
                                            
} /* odtu_dmx_temp_mo_chnl_prov */


/*
** Private Functions
*/
/*******************************************************************************
* odtu_dmx_ctxt_reg_recover
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Recover a odtu_dmx_recover_var_t context register space.
*
*  This function is used by odtu_dmx_handle_restart_init.
*  
*  This function return an error when something is incoherent in the register 
*  space. In such case, the only solution for crash restart is to reset the 
*  subsystem
* 
* INPUTS:
*   *odtu_dmx_handle        - pointer to MAPOTN handle instance to be operated on
*   top_energy_state_reg  - energy state from top-level
*   
*
* OUTPUTS:
*   *odtu_dmx_var_reg          - A partial odtu_dmx_var_t context + additional
*                              information to facilitate audit phase
*
* RETURNS:
*  PMC_SUCCESS: 
*   recovery is a success 
*  ODTU_DMX_ERR_CONTEXT_REG_INCOHERENT:
*   Register conetxt is incoherent. top will have
*    to shutdown the subsystem
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_dmx_ctxt_reg_recover(odtu_dmx_handle_t *odtu_dmx_handle,
                                            pmc_energy_state_t top_energy_state_reg,                                         
                                            odtu_dmx_recover_var_t *odtu_dmx_var_reg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL use_custom_ppm; 
    
    PMC_ENTRY();
  
    /* 
       create a register context 
    */

    /* start from a clean context */

    odtu_dmx_var_default_init(odtu_dmx_handle,&odtu_dmx_var_reg->var); 
          
    /* 
       In the case where top_energy state is RESET, we just have an initialized context
       
    */    
    if (PMC_SUCCESS == rc && PMC_ENERGY_STATE_RUNNING == top_energy_state_reg)
    {
        UINT8 map_mode;
        UINT32 ho_chnl_itr, lo_chnl_itr, ts_itr;
        BOOL8 dmx_ho_actv_chnl[ODTU_DMX_NUM_CHANL];
        BOOL8 dmx_lo_actv_chnl[ODTU_DMX_NUM_CHANL];
        UINT32 bit_index;

        odtu_dmx_activated_chnl_get(odtu_dmx_handle,
                                    &dmx_ho_actv_chnl[0],
                                    &dmx_lo_actv_chnl[0]);
        ohfs_remove_reg_ctxt_recover(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                     &odtu_dmx_var_reg->ohfs_remove_recover_var);
        
        /* iterate over all HO channel */
        for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_DMX_NUM_CHANL && PMC_SUCCESS== rc;ho_chnl_itr++)
        {
            if (dmx_ho_actv_chnl[ho_chnl_itr] == TRUE)
            {
                odtu_dmx_chnl_mode_t chnl_mode;
                odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_OPERATIONAL;

                /* retrieve information from OHFS_REMOVE */
                ohfs_remove_reg_ctxt_ho_chnl_cfg_get(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                                     &odtu_dmx_var_reg->ohfs_remove_recover_var,
                                                     ho_chnl_itr,
                                                     &odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot,
                                                     odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].cal_array);
                /* recover ts_type */
                chnl_mode = odtu_dmx_chnl_mode_get(odtu_dmx_handle,ho_chnl_itr);
                if (ODTU_DMX_CHNL_UNCHANNELIZED == chnl_mode)
                {
                    odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type = UTIL_GLOBAL_ODU_UNCHANNELIZED;
                } else if (ODTU_DMX_CHNL_NULL_FRAME_MODE == chnl_mode)
                {
                    odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type = UTIL_GLOBAL_ODU_NULL;
                } else 
                {                    
                    /* UTIL_GLOBAL_ODU_TS_1G25 or UTIL_GLOBAL_ODU_TS_2G5 */
                    odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type   = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
                    /* recover ODUKP type  */
                    odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type = UTIL_GLOBAL_LAST_ODUK;
                                                                              
                    
                    /* UNUSED */
                    /* odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_cal_entries */

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
        for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_DMX_NUM_CHANL && PMC_SUCCESS== rc;lo_chnl_itr++)
        {
            if (dmx_lo_actv_chnl[lo_chnl_itr] == TRUE)
            {
                ohfs_remove_reg_ctxt_lo_chnl_cfg_get(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                                     &odtu_dmx_var_reg->ohfs_remove_recover_var,
                                                     lo_chnl_itr,
                                                     &odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts,
                                                     &map_mode,
                                                     &odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id,
                                                     odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].cal_array,
                                                     odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array);
                
                if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id < ODTU_DMX_NUM_CHANL)
                {
                    if (dmx_ho_actv_chnl[odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id] == TRUE &&
                    odtu_dmx_var_reg->var.ho_chnl_cfg[odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].ts_type != UTIL_GLOBAL_ODU_UNCHANNELIZED)
                {                   
                    
                    /* check if it is a channel in passthru */
                    if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id < ODTU_DMX_NUM_CHANL && 
                        odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts > 0)
                    {
                        odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_OPERATIONAL;
                        
                        /* just to simplify the code */
                        ho_chnl_itr = odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id;
                        
                        odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_chnl_itr);
                    
                        /* resolve mapping mode */
                        if ( (map_mode >= 1) && (map_mode <= 3) )
                        {
                            odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode = UTIL_GLOBAL_AMP;
                        }
                        else if (map_mode == 4)
                        {
                            odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode = UTIL_GLOBAL_GMP;
                        } 
                        else 
                        {
                            rc = ODTU_DMX_ERR_CONTEXT_REG_INCOHERENT;
                        }
                        
                        if(odtu_dmx_var_reg->var.ppm_offset[lo_chnl_itr] != 0)
                        {
                            use_custom_ppm = TRUE;
                        }
                        else
                        {
                            use_custom_ppm = FALSE;
                        }
                        
                        /* resolve ts_type, lo/ho_odukp_type*/
                        rxjc_proc_param_cfg_get(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle, 
                                                lo_chnl_itr,
                                                odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot,
                                                odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts,
                                                odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode,
                                                use_custom_ppm,
                                                odtu_dmx_var_reg->var.ppm_offset[lo_chnl_itr],
                                                &odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type,
                                                &odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type,
                                                &odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type);
                        /* ajust the number of trib slots...*/
                        if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                        {
                            odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts /= 2;
                        }

                        /* update HO variables */
                        odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type = 
                            odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type;
                        odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type = 
                            odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    
                    
                        /* regenerate trib_slot_mask */                
                        for(ts_itr = 0;ts_itr < odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts;ts_itr++)
                        {
                            bit_index = odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr];
                            odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].rx_tribslot_mask[bit_index/32] |= 0x1 << (bit_index%32);
                        }
                    
                        for(ts_itr = 0;ts_itr < odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts; ts_itr++)
                        {
                            if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_added[odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] == 1 ||
                                odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_occupied[odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] == 1)
                            {
                                rc = ODTU_DMX_ERR_CONTEXT_REG_INCOHERENT;
                            } else 
                            {
                                odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_added[odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                                odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].tribslot_occupied[odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                            }
                            odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts += 1;
                            }
                        }
                    }
                }
            }
        }

        /* recover dummy_sched */
        if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO)
        {
            for (ts_itr = 0; ts_itr < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; ts_itr++)
            {
                if (odtu_dmx_field_DUMMY_SCHDL_EN_get(NULL, odtu_dmx_handle, ts_itr) == TRUE)
                {
                    odtu_dmx_var_reg->var.dummy_schd_ctxt[ts_itr].dummy_ch_id = odtu_dmx_field_DUMMY_SCHDL_ID_get(NULL, odtu_dmx_handle, ts_itr);
                    odtu_dmx_var_reg->var.dummy_schd_ctxt[ts_itr].dummy_cycle = odtu_dmx_field_DUMMY_SCHDL_CYCLE_get(NULL, odtu_dmx_handle, ts_itr);
                    odtu_dmx_var_reg->var.dummy_schd_ctxt[ts_itr].dummy_state = TRUE;
                }
            }
        }
    }
    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, 0x7F);
    PMC_RETURN(rc);
} /* odtu_dmx_ctxt_reg_recover */

/*******************************************************************************
* odtu_dmx_ctxt_audit
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
*   *odtu_dmx_handle        - pointer to MAPOTN handle instance to be operated on
*   *restart_init_cfg_ptr       - pointer to restart init config
*   *odtu_dmx_var_reg       - A partial odtu_dmx_var_t context + additional
*                              information to facilite audit phase
*  *prev_stg_dmx_var_reg        - The recovery context from the previous stg\n
*                                 It may be NULL
* OUTPUTS:
*   *odtu_dmx_var_clean     - A clean context which represents the intersection
*                             between both context is also generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS:
*    Memory & register context matches
*   ODTU_DMX_ERR_CONTEXT_CLEANUP:
*    A cleanup is required.
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_dmx_ctxt_audit(odtu_dmx_handle_t *odtu_dmx_handle,
                                      util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                      odtu_dmx_recover_var_t *odtu_dmx_var_reg,      
                                      odtu_dmx_recover_var_t *prev_stg_dmx_var_reg,
                                      odtu_dmx_var_t *odtu_dmx_var_clean)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 ho_chnl_itr, lo_chnl_itr, ts_itr;
    line_core_otn_db_key key;
    UINT32 num_of_recs = 0;
    UINT32 actual_num_of_recs;
    BOOL8 odu3_3Xslot_case = FALSE;
    line_core_otn_db_query_res *q_result;
    BOOL8 chnl_in_sync;
    BOOL8 has_prev_stage = FALSE;

    PMC_ENTRY();
    
    /* in the SYSOTN configuration, stage 2 is driven by stage 1
       in such case, we are not considering anything the previous stage */
    if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO && 
        prev_stg_dmx_var_reg != NULL)
    {
        has_prev_stage = TRUE;
    }


    /* init the new context  */
    odtu_dmx_var_default_init(odtu_dmx_handle,odtu_dmx_var_clean);
    
    /* 
       first, we resolve some ambiguities 
    */    
    for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_DMX_NUM_CHANL;ho_chnl_itr++)
    {
        if ( odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU4 && 
             odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED &&
             odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL && 
             odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state  == (UINT32)ODTU_DMX_CHNL_START)
        {
            PMC_MEMCPY(&odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr], &odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
        }
        
        if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state  == (UINT32)ODTU_DMX_CHNL_OPERATIONAL && 
            odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state  == odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state && 
            odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot  == odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot &&             
            odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts ==0)
        {
            /* we have not detected any active trib slots, but the container matches, we accept it */
            PMC_MEMCPY(&odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr], &odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
        }
            
        /* resolve ODU3E2 workaround */
        if ( odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state  == (UINT32)ODTU_DMX_CHNL_OPERATIONAL && 
             odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state  == odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state && 
             odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot  == 33 && 
             odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot == 32)
        {
            odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot;
        }

        if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL &&
            odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
        {
            if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3 &&
                odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3E2)
            {
                odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type;
            }
        }
    }

    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_DMX_NUM_CHANL;lo_chnl_itr++)
    {
        /* in warm restart mode, additional checks are performed at upper-level */
        if (restart_init_cfg_ptr->is_warm_restart == TRUE)
        {
            if (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_EQUIPPED) 
            {
                PMC_MEMCPY(&odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr], &odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr], sizeof(util_global_odu_lo_chnl_cfg_t)); 
            }
        }

        if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL && 
            odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL  &&
            (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode))
        {
            if ( UTIL_GLOBAL_GMP == odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode)
            {
                if ((odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP) &&                   
                    (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR))
                {                 
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODUFLEX_CBR;
                }
                if ((odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR) &&                   
                    (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP))
                {                 
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODUFLEX_GFP;
                }
                
                /*GMP: for client_type UTIL_GLOBAL_ODU2E -> we accept UTIL_GLOBAL_ODU2E_ETRANS */
                if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU2E &&
                    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU2E_ETRANS)
                {
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODU2E_ETRANS;
                }
                /* GMP:client_type UTIL_GLOBAL_ODU1E -> accept  UTIL_GLOBAL_ODU1E_ETRANS */
                if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU1E &&
                    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU1E_ETRANS)
                {
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = UTIL_GLOBAL_ODU1E_ETRANS;
                }
                if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3 &&
                    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3E2)
                {
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                    
                /* GMP:server_type = UTIL_GLOBAL_ODU2 and client_type = UTIL_GLOBAL_ODU0 -> we accept also server_type = UTIL_GLOBAL_ODU2E (_ETRANS_ */
                if ((odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODU0 ) &&
                    (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2 ) &&                    
                    (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2E ||  odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2E_ETRANS))
                {                 
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    odtu_dmx_var_reg->var.ho_chnl_cfg[odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /* GMP: *server_type = UTIL_GLOBAL_ODU3 && *client_type = UTIL_GLOBAL_ODUFLEX_CBR, we also accept *server_type = UTIL_GLOBAL_ODU3E2 && *client_type = UTIL_GLOBAL_ODUFLEX_CBR */
                if ((odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP ) &&
                    (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3 ) &&                    
                    (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU3E2))
                {                 
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    odtu_dmx_var_reg->var.ho_chnl_cfg[odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /* GMP: *server_type = UTIL_GLOBAL_ODU2 && *client_type = UTIL_GLOBAL_ODUFLEX_CBR, we also accept *server_type = UTIL_GLOBAL_ODU2E && *client_type = UTIL_GLOBAL_ODUFLEX_CBR  */
                if ((odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type) &&
                    (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP ) &&
                    (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2 ) &&                    
                    (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == UTIL_GLOBAL_ODU2E))
                {                 
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                    odtu_dmx_var_reg->var.ho_chnl_cfg[odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id].oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type;
                }
                /*  GMP:all client_type may be converted to UTIL_GLOBAL_ODUFLEX_CBR after coherency check */
                if ((odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type) &&                   
                    (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP ||
                     odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_CBR ||
                     odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP))
                {                 
                    odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type = odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type;
                }
            }
        }
    }
    
    
    /* Channel context audit */
    /* iterate over all HO channel */
    for (ho_chnl_itr = 0; ho_chnl_itr < ODTU_DMX_NUM_CHANL;ho_chnl_itr++)
    {
        
        if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state != odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state)
        {
            rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
        } else if (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
        {
            if (!(  (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].ts_type ||
                     odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_NULL)    &&
                    odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot == odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot))
            {
                rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
            } else 
            {
                chnl_in_sync = TRUE;
                if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO && 
                    TRUE == has_prev_stage)
                {
                    if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED )
                    {
                        /* in this case, we check the status of the previous stage */
                        if (!(prev_stg_dmx_var_reg->var.lo_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL ||
                              (prev_stg_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL &&
                               prev_stg_dmx_var_reg->var.lo_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_START)))
                        {
                            chnl_in_sync = FALSE;
                            rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                        }
                    } else                         
                    {
                        if (!(prev_stg_dmx_var_reg->var.lo_chnl_cfg[ho_chnl_itr].state == (UINT32)ODTU_DMX_CHNL_OPERATIONAL))
                        {
                            chnl_in_sync = FALSE;
                            rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                        }
                    }
                } 
                if (chnl_in_sync == TRUE)
                {
                    if (!(odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED && 
                          odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO))
                    {
                        /* check if the DB is in sync with the register space*/                
                        key.calendar_entry = DO_NOT_USE_KEY;
                        if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO)
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
                        num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                                   odtu_dmx_handle->cfg.db_id,
                                                   odtu_dmx_handle->cfg.db_handle, (void *)&key,
                                                   (void **)&q_result);
                        if (num_of_recs == 0 && num_of_recs != odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_tribslot) 
                        {
                            rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                            chnl_in_sync = FALSE;
                        } else 
                        {                    
                            for (ts_itr = 0; ts_itr < num_of_recs && chnl_in_sync == TRUE; ts_itr++)
                            {
                                if (q_result->p_recs[ts_itr]->calendar_entry !=  odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].cal_array[ts_itr])
                                {
                                    rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                                    chnl_in_sync = FALSE;
                                }                         
                            }                   
                        }
                    }
                }
                
                if (TRUE == chnl_in_sync)
                {                
                    if (odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_NULL ||  
                        odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
                    {
                        PMC_MEMCPY(&odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr], &odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
                    } else
                    {
                        /* we have a channelized HO container, continue the audit  */
                        /*
                         * Added comparison of the num_active_ts for the
                         * HO and removed the low order comparison.  The
                         * former represents all active lo order channels
                         * for this HO where as the LO order channel in
                         * the channel handle represents the possible 
                         * number, 8 for ODU2 for example, whereas the
                         * recovered value is the actual number.
                         */
                        if (!(odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type ==  odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].oduk_type && 
                              util_global_array_compare(odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].cal_array,
                                                        odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].cal_array, 
                                                        sizeof(odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].cal_array))== TRUE) &&
                            (odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts != odtu_dmx_var_reg->var.ho_chnl_cfg[ho_chnl_itr].num_active_ts))
                        {
                            rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                        } else 
                        {
                            /* we are almost there */          
                            chnl_in_sync = TRUE;                            
                            if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO && odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr].oduk_type == UTIL_GLOBAL_ODU3)
                            {
                                for (ts_itr = 0; ts_itr < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; ts_itr++)
                                {
                                    if (odtu_dmx_var_reg->var.dummy_schd_ctxt[ts_itr].dummy_ch_id == ho_chnl_itr && 
                                        odtu_dmx_handle->var.dummy_schd_ctxt[ts_itr].dummy_ch_id == ho_chnl_itr)
                                    {
                                        PMC_MEMCPY(&odtu_dmx_var_clean->dummy_schd_ctxt[ts_itr], &odtu_dmx_handle->var.dummy_schd_ctxt[ts_itr],sizeof(odtu_dmx_dummy_schd_ctxt_t));
                                    }
                                }
                            }
                            if (TRUE == chnl_in_sync)
                            {                              
                                /* at this point we can accept the HO channel                                                   
                                 */                    
                                PMC_MEMCPY(&odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr], &odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_itr], sizeof(util_global_odu_ho_chnl_cfg_t));  
                                
                                /* tribslot_added, tribslot_occupied, num_active_ts may not be in sync but it is only an indication that
                                   they are LO channel not in sync. That will be identified in the next LO channel inspection */
                                PMC_MEMSET(odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_added,0,sizeof(odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_added));
                                PMC_MEMSET(odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_occupied,0,sizeof(odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_occupied));
                                odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].num_active_ts = 0;                                                
                            }   
                        }
                    }
                }
            }
        }
    }
    

    /* iterate over LO channel */
    for (lo_chnl_itr = 0; lo_chnl_itr < ODTU_DMX_NUM_CHANL;lo_chnl_itr++)
    {             
        odu3_3Xslot_case = FALSE;
        if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state != odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].state)
        {
            rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
        } else if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].state ==  (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
        {
            if (!( odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ts_type &&
                   odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].mapping_mode &&
                   odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id &&
                   odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type &&
                   odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type == odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr].lo_oduk_type))
            {
                rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
            } else 
            {
                ho_chnl_itr = odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_chnl_id;
                /* first check if the associated HO channel is still alive ...*/
                if (odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].state ==  (UINT32)ODTU_DMX_CHNL_OPERATIONAL &&
                    odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].oduk_type ==  odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ho_oduk_type)
                {
                    chnl_in_sync = TRUE;
                    /* check if the DB is in sync with the register space*/                
                    key.calendar_entry = DO_NOT_USE_KEY;
                    if (odtu_dmx_var_reg->dmx_stage == ODTU_DMX_STG_TWO)
                    {
                        key.ho_channel = DO_NOT_USE_KEY;
                        key.mo_channel = DO_NOT_USE_KEY;
                        key.lo_channel = lo_chnl_itr;
                    } else 
                    {
                        key.ho_channel = DO_NOT_USE_KEY;
                        key.mo_channel = lo_chnl_itr;;
                        key.lo_channel = DO_NOT_USE_KEY;
                    }                      
                    key.trib_slot = DO_NOT_USE_KEY;                      
                    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                                               odtu_dmx_handle->cfg.db_id,
                                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                                               (void **)&q_result);
                    actual_num_of_recs = odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].num_ts;
                    if (odtu_dmx_var_reg->var.lo_chnl_cfg[lo_chnl_itr].ts_type == UTIL_GLOBAL_ODU_TS_2G5)
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
                        rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                        chnl_in_sync = FALSE;
                    } else 
                    {                 
                        if (odu3_3Xslot_case == FALSE)
                        {
                            for (ts_itr = 0; ts_itr < num_of_recs && chnl_in_sync == TRUE; ts_itr++)
                            {
                                if (q_result->p_recs[ts_itr]->calendar_entry !=  odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].cal_array[ts_itr] )
                                {
                                    rc = ODTU_DMX_ERR_CONTEXT_CLEANUP;
                                    chnl_in_sync = FALSE;
                                }                         
                            }                   
                        }
                    }
                    if (TRUE == chnl_in_sync)
                    {
                        PMC_MEMCPY(&odtu_dmx_var_clean->lo_chnl_cfg[lo_chnl_itr], &odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_itr], sizeof(util_global_odu_lo_chnl_cfg_t)); 
                        
                        /* RESYNC HO fields */
                        for(ts_itr = 0;ts_itr < odtu_dmx_var_clean->lo_chnl_cfg[lo_chnl_itr].num_ts; ts_itr++)
                        {
                            odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_added[odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                            odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].tribslot_occupied[odtu_dmx_var_reg->lo_chnl_extra[lo_chnl_itr].ts_array[ts_itr]] = 1;
                            odtu_dmx_var_clean->ho_chnl_cfg[ho_chnl_itr].num_active_ts += 1;
                        }                    
                    }
                }                                
            }
        }   
    }
    
    PMC_RETURN(rc);
} /* odtu_dmx_ctxt_audit */


/*******************************************************************************
*  odtu_dmx_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODTU_DMX var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   *odtu_dmx_handle  - pointer to the  ODTU_DMX handle
*   *odtu_dmx_var     - pointer to the  ODTU_DMX var instance 
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_var_default_init(odtu_dmx_handle_t *odtu_dmx_handle, 
                                       odtu_dmx_var_t *odtu_dmx_var)
{
    /* variable delcaration */
    UINT32 chnl_id;
    UINT32 ts, lo_ch;
    UINT32 i;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("initialize var data \n");

    /* argument checking */
    PMC_ASSERT(odtu_dmx_handle != NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(odtu_dmx_var != NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);

    odtu_dmx_var->prov_mode = odtu_dmx_handle->var.prov_mode;
    /* initialize all HO chnl cfg */
    for (chnl_id = 0; chnl_id < odtu_dmx_handle->cfg.max_chnl_num; chnl_id++)
    {
        odtu_dmx_var->ho_chnl_cfg[chnl_id].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_START;
        odtu_dmx_var->ho_chnl_cfg[chnl_id].oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
        odtu_dmx_var->ho_chnl_cfg[chnl_id].num_tribslot = 0;
        odtu_dmx_var->ho_chnl_cfg[chnl_id].num_cal_entries = 0;
        odtu_dmx_var->ho_chnl_cfg[chnl_id].ts_type = UTIL_GLOBAL_ODU_UNCHANNELIZED;
        odtu_dmx_var->ho_chnl_cfg[chnl_id].num_active_ts = 0;

        /* initialize all ts config in one HO channel */
        for (ts = 0; ts < ODTU_DMX_NUM_TS_IN_CHNL; ts++)
        {
            odtu_dmx_var->ho_chnl_cfg[chnl_id].tribslot_added[ts] = 0;
            odtu_dmx_var->ho_chnl_cfg[chnl_id].tribslot_occupied[ts] = 0;
        }
        for (ts = 0; ts < UTIL_GLOBAL_MAX_NUM_TS; ts++)
        {
            odtu_dmx_var->ho_chnl_cfg[chnl_id].cal_array[ts] = 0xFF;
        }

        for (lo_ch = 0; lo_ch < 96; lo_ch++)
        {
            odtu_dmx_var->lo_chnl_cfg[lo_ch].state = (util_global_chnl_state_t)ODTU_DMX_CHNL_START;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].lo_oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].ho_oduk_type = UTIL_GLOBAL_ODUK_DONT_CARE;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].num_ts = 0;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].ts_type = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].ho_chnl_id = 0xFF;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].mapping_mode = UTIL_GLOBAL_NO_MAP;
            odtu_dmx_var->lo_chnl_cfg[lo_ch].ts_port = 0x7F;
            odtu_dmx_var->ppm_offset[lo_ch] = 0;
        }

        odtu_dmx_var->ho_chnl_cfg[chnl_id].ts_type = LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD;
        odtu_dmx_var->ho_chnl_cfg[chnl_id].num_active_ts = 0;
    }
    
    for (i = 0; i < UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD; i++)
    {
        odtu_dmx_var->dummy_schd_ctxt[i].dummy_ch_id = 0x7F;
        odtu_dmx_var->dummy_schd_ctxt[i].dummy_cycle = 0;
        odtu_dmx_var->dummy_schd_ctxt[i].dummy_state = FALSE;
    }        
    
    /* initialize all avl ipt */
    for (lo_ch = 0; lo_ch < ODTU_DMX_NUM_CHANL; lo_ch++)
    {
        odtu_dmx_var->mp_mpmgen_avl_ipt[lo_ch] = 0x7F;
    }
    
    UINT32 ghao_ctxt;
    /* initialize G.HAO contexts */
    for (ghao_ctxt = 0; ghao_ctxt < ODTU_DMX_GHAO_NUM_CTXT; ghao_ctxt++)
    {
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].lcr_state = GHAO_LCR_STATE_IDLE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].bwr_state = GHAO_BWR_STATE_IDLE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].discovery_mode = FALSE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].channel_id = 0;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].rp = FALSE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].tscc = FALSE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].tsgs = FALSE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].ctrl = GHAO_LCR_CTRL_IDLE;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].tpid = 0;
        odtu_dmx_handle->ghao_dmx_state[ghao_ctxt].same_rcoh = FALSE;
    }

    PMC_RETURN();
} /* odtu_dmx_var_default_init */

/*******************************************************************************
*  odtu_dmx_tribslot_var_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODU_RFRM var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODU_RFRM instance to be operated on
*   ts_id               - tributary slot number
*   ho_chnl_id          - HO Channel ID that the tributary slot belongs to.
*   ts_occupied         - status of the tributary slot to show whether it is in use. \n
*                         0 -- unoccupied \n
*                         1 -- occupied
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_tribslot_var_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                          UINT32 ts_id, UINT32 ho_chnl_id,
                                          BOOL8 ts_occupied)
{
    PMC_ENTRY();
    
    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[ts_id] = ts_occupied;
    
    PMC_RETURN();
} /* odtu_dmx_tribslot_var_update */

/*******************************************************************************
*  odtu_dmx_dump_var_context
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function dump the var context of ODTU_DMX var_t variables for debug 
*   purposes
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id           - HO channel ID
*   lo_chnl_id           - LO channel ID
*   all_chnl             - dump all channel
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_dump_var_context(odtu_dmx_handle_t *odtu_dmx_handle, UINT8 ho_chnl_id, UINT8 lo_chnl_id, BOOL8 all_chnl )
{
    /* variable delcaration */
    UINT32 chnl_id;
    UINT32 ts, lo_ch;
    UINT32 i;

    PMC_ENTRY();
    
    if(all_chnl) {

    PMC_LOG_TRACE("DUMP var data \n");

    PMC_LOG_TRACE("odtu_dmx_handle->cfg.max_chnl_num = %u \n", odtu_dmx_handle->cfg.max_chnl_num);
    /* initialize all HO chnl cfg 96 */
    for (chnl_id = 0; chnl_id < odtu_dmx_handle->cfg.max_chnl_num; chnl_id++)
    {
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].state : %d", chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].state);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].oduk_type : %d",chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].oduk_type);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].num_tribslot = %u", chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].num_tribslot);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].num_cal_entries = %u", chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].num_cal_entries);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].ts_type : %d ", chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].ts_type);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].num_active_ts : %u ", chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].num_active_ts);
      
      /* 96 */
      /* However, cal_array have only UTIL_GLOBAL_MAX_NUM_TS members, so
         we are going to print only first UTIL_GLOBAL_MAX_NUM_TS of them. */
      for (ts = 0; ts < UTIL_GLOBAL_MAX_NUM_TS; ts++)
        {
          PMC_LOG_TRACE("ho_chnl_cfg[%d].cal_array[%u] = %u",chnl_id, ts, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].cal_array[ts]);
        }
      /* 80*/
      for (ts = 0; ts < ODTU_DMX_NUM_TS_IN_CHNL; ts++) {
        PMC_LOG_TRACE("ho_chnl_cfg[%d].tribslot_added[%u] = %u", chnl_id, ts, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].tribslot_added[ts]);
      }
      for (ts = 0; ts < ODTU_DMX_NUM_TS_IN_CHNL; ts++) {
        PMC_LOG_TRACE("ho_chnl_cfg[%d].tribslot_occupied[%u] = %u", chnl_id, ts, odtu_dmx_handle->var.ho_chnl_cfg[chnl_id].tribslot_occupied[ts]);
      }
    }

    /* initialize all LO chnl cfg 96 */
    for (lo_ch = 0; lo_ch < 96; lo_ch++)
      {
        PMC_LOG_TRACE("lo_chnl_cfg[%d].state : %d ", lo_ch, odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].state);
        PMC_LOG_TRACE("lo_chnl_cfg[%d].ho_chnl_id : %u ",lo_ch,odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].ho_chnl_id);
        PMC_LOG_TRACE("lo_chnl_cfg[%d].lo_oduk_type : %d ",lo_ch,odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].lo_oduk_type);
        PMC_LOG_TRACE("lo_chnl_cfg[%d].ho_oduk_type : %d ",lo_ch,odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].ho_oduk_type);
        PMC_LOG_TRACE("lo_chnl_cfg[%d].num_ts : %u ",lo_ch,odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].num_ts);
        PMC_LOG_TRACE("lo_chnl_cfg[%d].mapping_mode : %d ",lo_ch,odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].mapping_mode);
        PMC_LOG_TRACE("lo_chnl_cfg[%d].ts_port : %d ",lo_ch,odtu_dmx_handle->var.lo_chnl_cfg[lo_ch].ts_port);
      }

    for (lo_ch = 0; lo_ch < 96; lo_ch++)
      {
        PMC_LOG_TRACE("mp_mgen_avl_ipt[%d] : %d ", lo_ch, odtu_dmx_handle->var.mp_mpmgen_avl_ipt[lo_ch]);
      }

    } else {

      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].state : %d", ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].state);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].oduk_type : %d",ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].num_tribslot = %u", ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].num_cal_entries = %u", ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_cal_entries);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].ts_type : %d ", ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type);
      PMC_LOG_TRACE("dmx - ho_chnl_cfg[%d].num_active_ts : %u ", ho_chnl_id, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts);

      /* cal_array is only UTIL_GLOBAL_MAX_NUM_TS deep. */
      for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++) {
        PMC_LOG_TRACE("ho_chnl_cfg[%d].cal_array[%u] = %u",ho_chnl_id, i, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i]);
      }
      
      for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++) {
        PMC_LOG_TRACE("ho_chnl_cfg[%d].tribslot_added[%u] = %u", ho_chnl_id, i, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_added[i]);
      }
      for (i = 0; i < UTIL_GLOBAL_MAX_NUM_TS; i++) {
        PMC_LOG_TRACE("ho_chnl_cfg[%d].tribslot_occupied[%u] = %u", ho_chnl_id, i, odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].tribslot_occupied[i]);
      }
      
      /* ------------------------------------------------------------------------- */
      
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : STS_SW HO_CHAN_ID STATE ORIG");

      PMC_LOG_TRACE("sts - ho_chnl_cfg[%d].oduk_type : %d", ho_chnl_id, odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type);
      PMC_LOG_TRACE("sts - ho_chnl_cfg[%d].ts_type : %d ", ho_chnl_id, odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type);
      PMC_LOG_TRACE("sts - ho_chnl_cfg[%d].chnl_id = %u", ho_chnl_id, odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].chnl_id);
      PMC_LOG_TRACE("sts - ho_chnl_cfg[%d].num_tribslot = %u", ho_chnl_id, odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot);
      
      for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {
        PMC_LOG_TRACE("sts -ho_chnl_cfg[%d].cal_entry[%u] = %u", ho_chnl_id, i, odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].cal_entry[i]);
      }
      for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {
        PMC_LOG_TRACE("sts -ho_chnl_cfg[%d].lo_chnl_ids_ptr[%u] = %u",ho_chnl_id, i, odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[ho_chnl_id].lo_chnl_ids_ptr[i]);
      }
      

      /* ------------------------------------------------------------------------- */
      
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : ODTU_DMX LO_CHAN_ID STATE ORIG");
      
      PMC_LOG_TRACE("lo_chnl_cfg[%d].state : %d ", lo_chnl_id, odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].state);
      PMC_LOG_TRACE("lo_chnl_cfg[%d].ho_chnl_id : %u ",lo_chnl_id,odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ho_chnl_id);
      PMC_LOG_TRACE("lo_chnl_cfg[%d].lo_oduk_type : %d ",lo_chnl_id,odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].lo_oduk_type);
      PMC_LOG_TRACE("lo_chnl_cfg[%d].ho_oduk_type : %d ",lo_chnl_id,odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ho_oduk_type);
      PMC_LOG_TRACE("lo_chnl_cfg[%d].num_ts : %u ",lo_chnl_id,odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].num_ts);
      PMC_LOG_TRACE("lo_chnl_cfg[%d].mapping_mode : %d ",lo_chnl_id,odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].mapping_mode);
      PMC_LOG_TRACE("lo_chnl_cfg[%d].ts_port : %d ",lo_chnl_id,odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ts_port);
      
      /* ------------------------------------------------------------------------- */
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : GHAO DMX STATE ORIG");
      
      for (i = 0; i < ODTU_DMX_GHAO_NUM_CTXT; i++) {
        PMC_LOG_TRACE("ghao_dmx_state.lcr_state : %d ",odtu_dmx_handle->ghao_dmx_state[i].lcr_state);
        PMC_LOG_TRACE("ghao_dmx_state.bwr_state : %d ",odtu_dmx_handle->ghao_dmx_state[i].bwr_state);
        PMC_LOG_TRACE("ghao_dmx_state.discovery_mode : : %d ",odtu_dmx_handle->ghao_dmx_state[i].discovery_mode);
        PMC_LOG_TRACE("ghao_dmx_state.channel_id : : %u ",odtu_dmx_handle->ghao_dmx_state[i].channel_id);
        PMC_LOG_TRACE("ghao_dmx_state.tpid : : %X ",odtu_dmx_handle->ghao_dmx_state[i].tpid);
      }

      for (lo_ch = 0; lo_ch < 96; lo_ch++)
        {
          PMC_LOG_TRACE("mp_mgen_avl_ipt[%d] : %d ", lo_ch, odtu_dmx_handle->var.mp_mpmgen_avl_ipt[lo_ch]);
        }


    }


    PMC_RETURN();   
} /* odtu_dmx_dump_var_context */

/*******************************************************************************
*  odtu_dmx_lo_chnl_var_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODU_RFRM var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   odtu_dmx_handle - handle to the ODTU_DMX instance to be operated on
*   lo_chnl_id      - LO ODU channel ID.
*                     Valid range: 0 - 95
*   ho_chnl_id      - HO ODU channel ID. Valid range: 0 - 95
*   chnl_state      - Enum for dmx channel state. See odtu_dmx_chnl_state_t.
*   lo_oduk_type    - Enum for LO ODU type. See util_global_odukp_type_t.
*   ho_oduk_type    - Enum for HO ODU type. See util_global_odukp_type_t.
*   num_ts          - number of tributary slots in the current LO channel.
*   *cal_array      - pointer to the calendar slot offsets of the LO channel.
*   ts_type         - Enum for the payload format. See util_global_odu_line_payload_t.
*   mapping_mode    - Enum for the mapping mode. See util_global_mapping_mode_t.
*   ts_port         - tributary port number associated with the lo_chnl_id.
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_lo_chnl_var_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                         UINT32 lo_chnl_id, UINT32 ho_chnl_id,
                                         odtu_dmx_chnl_state_t chnl_state,
                                         util_global_odukp_type_t lo_oduk_type,
                                         util_global_odukp_type_t ho_oduk_type,
                                         UINT32 num_ts, UINT8 *cal_array,
                                         util_global_odu_line_payload_t ts_type,
                                         util_global_mapping_mode_t mapping_mode,
                                         UINT8 ts_port)
{
    PMC_ENTRY();

    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].state = (util_global_chnl_state_t)chnl_state;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ho_chnl_id = ho_chnl_id;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].lo_oduk_type = lo_oduk_type;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ho_oduk_type = ho_oduk_type;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].num_ts = num_ts;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ts_type = ts_type;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].mapping_mode = mapping_mode;
    odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ts_port = ts_port;

    PMC_RETURN();

} /* odtu_dmx_lo_chnl_var_update */

/*******************************************************************************
*  odtu_dmx_ho_chnl_var_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODTU_DMX var_t variables. 
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - HO channel ID. Valid range: 0 - 95
*   chnl_state          - Enum for dmx channel state. See odtu_dmx_chnl_state_t.
*   ho_odu_type         - Enum for HO ODU type. See util_global_odukp_type_t.
*   num_tribslot        - number of tributary slots in the HO channel.
*                         Valid range: 0 - 80
*   *cal_array          - pointer to the calendar slot offsets of the HO channel.
*   *lo_chnl_ids        - pointer to the LO channel IDs that are provisioned in
*                         the current HO channel.
*   ts_type             - Enum for payload format. See util_global_odu_line_payload_t.
*   num_active_ts       - number of active tributary slots in the current HO channel.
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_ho_chnl_var_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                         UINT32 ho_chnl_id,
                                         odtu_dmx_chnl_state_t chnl_state,
                                         util_global_odukp_type_t ho_odu_type,
                                         UINT32 num_tribslot, UINT8 *cal_array,
                                         UINT32 *lo_chnl_ids,
                                         util_global_odu_line_payload_t ts_type,
                                         UINT32 num_active_ts)
{
    /* variable declaration */
    UINT32 i;
    
    PMC_ENTRY();

    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].state = (util_global_chnl_state_t)chnl_state;
    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type = ho_odu_type;
    
    PMC_LOG_TRACE("num_tribslot = %u\n", num_tribslot);
    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot = num_tribslot;
    PMC_LOG_TRACE("odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot = %u\n", odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot);

    for (i = 0; i < num_tribslot; i++)
    {
        if (NULL != cal_array)
        {
            odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i] = cal_array[i];
        } else 
        {
            odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].cal_array[i] = 0xFF;
        }
    }
    
    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type = ts_type;
    odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_active_ts = num_active_ts;

    PMC_RETURN();
} /* odtu_dmx_ho_chnl_var_update */



/*******************************************************************************
*  odtu_dmx_dummy_schd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function schedules a calendar entry to be assigned for a MO channel. 
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   dummy_schd_chnl     - dummy schedule channel instance. Valid range: 0 - 3
*   mo_chnl_id          - MO channel ID that requires an additional calendar entry.
*                         Valid range: 0 - 95
*   calendar_entry      - Calendar entry to be assigned for the additional scheduling
*   enable              - enable or disable the dummy calendar scheduling
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERROR
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odtu_dmx_dummy_schd_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                          UINT32 dummy_schd_chnl,
                                          UINT32 mo_chnl_id,
                                          UINT32 calendar_entry,
                                          BOOL8 enable)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    
    if (enable)
    {                                         
        odtu_dmx_field_DUMMY_SCHDL_ID_set(NULL, odtu_dmx_handle, dummy_schd_chnl, mo_chnl_id);
        odtu_dmx_field_DUMMY_SCHDL_CYCLE_set(NULL, odtu_dmx_handle, dummy_schd_chnl, calendar_entry);
        odtu_dmx_field_DUMMY_SCHDL_EN_set(NULL, odtu_dmx_handle, dummy_schd_chnl, enable);
    }
    else {
        odtu_dmx_field_DUMMY_SCHDL_EN_set(NULL, odtu_dmx_handle, dummy_schd_chnl, enable);
        odtu_dmx_field_DUMMY_SCHDL_ID_set(NULL, odtu_dmx_handle, dummy_schd_chnl, mo_chnl_id);
        odtu_dmx_field_DUMMY_SCHDL_CYCLE_set(NULL, odtu_dmx_handle, dummy_schd_chnl, calendar_entry);        
    }         
    
    /* update the context */
    odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_chnl].dummy_ch_id = mo_chnl_id;
    odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_chnl].dummy_cycle = calendar_entry;
    odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_chnl].dummy_state = enable;
    
    PMC_RETURN(result);
} /* odtu_dmx_dummy_schd_cfg */  


/*******************************************************************************
*  odtu_dmx_dummy_schd_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function schedules a calendar entry to be assigned for a MO channel. 
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_dummy_schd_clean(odtu_dmx_handle_t *odtu_dmx_handle)
{
    UINT32 dummy_schd_itr, id, enable;
    PMC_ENTRY();
    
    for(dummy_schd_itr = 0;dummy_schd_itr<UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD;dummy_schd_itr++)
    {                
        enable = odtu_dmx_field_DUMMY_SCHDL_EN_get(NULL, odtu_dmx_handle, dummy_schd_itr);
        if (TRUE == enable)
        {
            id = odtu_dmx_field_DUMMY_SCHDL_ID_get(NULL, odtu_dmx_handle, dummy_schd_itr);
            
            PMC_ASSERT(id < ODTU_DMX_NUM_CHANL, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);
            if (odtu_dmx_handle->var.ho_chnl_cfg[id].state !=  (UINT32)ODTU_DMX_CHNL_OPERATIONAL)
            {            
                odtu_dmx_field_DUMMY_SCHDL_ID_set(NULL, odtu_dmx_handle, dummy_schd_itr, 0x7F);
                odtu_dmx_field_DUMMY_SCHDL_CYCLE_set(NULL, odtu_dmx_handle, dummy_schd_itr, 0);
                odtu_dmx_field_DUMMY_SCHDL_EN_set(NULL, odtu_dmx_handle, dummy_schd_itr, 0);            
                
                odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_ch_id = 0x7F;
                odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_cycle = 0;
                odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_state = FALSE;
            }            
        } else if (odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_state == TRUE)
        {
            odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_ch_id = 0x7F;
            odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_cycle = 0;
            odtu_dmx_handle->var.dummy_schd_ctxt[dummy_schd_itr].dummy_state = FALSE;
        }
    }
    PMC_RETURN();
} /* odtu_dmx_dummy_schd_clean */  


/*******************************************************************************
*  odtu_dmx_get_free_mgen_ipt_entry
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the first available MP_MGEN IPT entry
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   *lo_chnl_id          - LO channel ID
*
* OUTPUTS:
*
* RETURNS:
*   UINT32 - the first available MP_MGEN IPT entry
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odtu_dmx_get_free_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                                UINT32 lo_chnl_id)
{
    /* variable delcaration */
    UINT32 i;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("Searching next available ipt entry");


    /* initialize all avl ipt */
    for (i = 0; i < 96; i++)
    {
        if( odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] == 0x7F) {
            odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] = lo_chnl_id;
            break;
        }
    }
    
    PMC_ASSERT(i < 96, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    /*odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] = lo_chnl_id;*/

    PMC_LOG_TRACE("available ipt entry : %d\n", i);

    PMC_RETURN(i);   
} /* odtu_dmx_get_free_mgen_ipt_entry */

/*******************************************************************************
*  odtu_dmx_get_offline_mgen_ipt_entry
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns IPT entry associated with "frm_num" 
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   lo_chnl_id           - LO channel ID
*   frm_num              - frame number of the tribslot in a LO channel
*
* OUTPUTS:
*
* RETURNS:
*   UINT32 - IPT entry associated with "frm_num" 
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odtu_dmx_get_offline_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 lo_chnl_id, UINT32 frm_num)
{
    /* variable delcaration */
    UINT32 i;
    UINT32 entry;
    UINT32 cnt = 0;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("Searching next available ipt entry");


    /* initialize all avl ipt */
    for (i = 0; i < 96; i++)
    {
        if(odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] == lo_chnl_id) 
        {
            if (cnt == frm_num)
            { 
                entry = i;
                break;
            }
            cnt++; 
        }
    }
    
    PMC_ASSERT(i < 96, ODTU_DMX_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    /*odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] = lo_chnl_id;*/

    PMC_LOG_TRACE("available ipt entry : %d\n", i);

    PMC_RETURN(entry);   
} /* odtu_dmx_get_offline_mgen_ipt_entry */


/*******************************************************************************
*  odtu_dmx_get_mgen_ipt_entry
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns the first MP MGEN IPT entry for the LO channel provided
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   *lo_chnl_id          - LO channel ID
*
* OUTPUTS:
*
* RETURNS:
*   UINT32 - the first MP MGEN IPT entry for the LO channel provided
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odtu_dmx_get_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                           UINT32 lo_chnl_id)
{
    /* variable delcaration */
    UINT32 i;
    BOOL8 match;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("Searching matching  ipt entry");

    match = FALSE;
    /* initialize all avl ipt */
    for (i = 0; i < 96; i++)
    {
        if( odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] == lo_chnl_id) 
        {
            PMC_LOG_TRACE("found matching  ipt entry : %d\n", i);
            match = TRUE;
            break;
        }
    }

    if( !match ) {
      i = 0x7F;
    }

    PMC_RETURN(i);   
} /* odtu_dmx_get_mgen_ipt_entry */


/*******************************************************************************
*  odtu_dmx_deprov_mgen_ipt_entry
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions all MP MGEN IPT entries
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   *lo_chnl_id          - LO channel ID
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_deprov_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 lo_chnl_id )
{
    /* variable delcaration */
    UINT8 i;
    UINT8 num_match = 0;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("Deprov  ipt entry matching lo_chnl_id : %d\n", lo_chnl_id);


    /* initialize all avl ipt */
    for (i = 0; i < 96; i++)
      {
        if( odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] == lo_chnl_id) {
          odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] = 0x7F;
          num_match += 1;
        }
      }

    PMC_LOG_TRACE("Deprov : %d entrries \n", num_match);

    PMC_RETURN();   
} /* odtu_dmx_deprov_mgen_ipt_entry */

/*******************************************************************************
*  odtu_dmx_deprov_first_mgen_ipt_entry
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisioned the first MP MGEN AIPT entries.
*
* INPUTS:
*   *odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   *lo_chnl_id          - LO channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odtu_dmx_deprov_first_mgen_ipt_entry(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 lo_chnl_id )
{
    /* variable delcaration */
    UINT8 i;
    UINT8 num_match = 0;

    PMC_ENTRY();
    
    PMC_LOG_TRACE("Deprov  ipt entry matching lo_chnl_id : %d\n", lo_chnl_id);


    /* initialize all avl ipt */
    for (i = 0; i < 96; i++)
    {
        if( odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] == lo_chnl_id) {
          odtu_dmx_handle->var.mp_mpmgen_avl_ipt[i] = 0x7F;
          num_match += 1;
          break;
        }
     }

    PMC_LOG_TRACE("Deprov : %d entrries \n", num_match);

    PMC_RETURN();   
} /* odtu_dmx_deprov_first_mgen_ipt_entry */


/*
** GHAO APIs
*/
/*******************************************************************************
*  odtu_dmx_ghao_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable ghao mecanism which indicate the OHFS_REMOVE to capture rcoh and process it
*   - enable OHFS_REMOVE HO_RCOH_EN
*   - enable OHFS_REMOVE FLEX_RCOH_EN
*   For demux instance, only need to enable ho_rcoh_enable
*
* INPUTS:
*   odtu_dmx_handle        - handle to the ODTU_DMX instance to be operated on
*   chnl_id                - channel id to enable
*   ho_rcoh_enable         - enable for ho rcoh
*   flex_rcoh_enable       - enable for flex rcoh
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_ghao_enable(odtu_dmx_handle_t *odtu_dmx_handle,
                                 UINT8 chnl_id,
                                 BOOL8 ho_rcoh_enable,
                                 BOOL8 flex_rcoh_enable)
{
    PMC_ENTRY();

    odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 1);

    ohfs_remove_ho_rcoh_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                            chnl_id,
                            ho_rcoh_enable);

    ohfs_remove_flex_rcoh_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                              chnl_id,
                              flex_rcoh_enable);

    odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 0);

    
    PMC_RETURN();
} /* odtu_dmx_ghao_enable */  

/*******************************************************************************
*  odtu_dmx_ghao_isr_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable ghao related events to be propagated to the interrupt pin 
*   - enable RCOH_TRNSFR_SHDW_DNE_I event to interrupt pin
*   - enable HAO_STATUS_AVL_I(96) event to propagate to interrupt pin
*   - enable CONFIG_UPDATE_DNE(96) event to propagate to interrupt pin
*   - enable var context to process procedure based on isr
*
* INPUTS:
*   odtu_dmx_handle        - handle to the ODTU_DMX instance to be operated on
*   enable                 - enable/disable
*
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_ghao_isr_enable(odtu_dmx_handle_t *odtu_dmx_handle,
                                          BOOL8 enable)
{
    PMC_ENTRY();

    UINT32 enable_all[3];    


    /* clear interrupt, before enabling it */            
    enable_all[0] = odtu_dmx_reg_HAO_STAT_INT_array_read( NULL, odtu_dmx_handle, 0);
    enable_all[1] = odtu_dmx_reg_HAO_STAT_INT_array_read( NULL, odtu_dmx_handle, 1);
    enable_all[2] = odtu_dmx_reg_HAO_STAT_INT_array_read( NULL, odtu_dmx_handle, 2);
    odtu_dmx_reg_HAO_STAT_INT_array_burst_write( NULL, odtu_dmx_handle, 0, 3, enable_all);

    enable_all[0] = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( NULL, odtu_dmx_handle, 0);
    enable_all[1] = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( NULL, odtu_dmx_handle, 1);
    enable_all[2] = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( NULL, odtu_dmx_handle, 2);
    odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write( NULL, odtu_dmx_handle, 0, 3, enable_all);

    enable_all[0] = 0;
    enable_all[1] = 0;
    enable_all[2] = 0;
    odtu_dmx_reg_HAO_STAT_INT_array_burst_write( NULL, odtu_dmx_handle, 0, 3, enable_all);
    odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write( NULL, odtu_dmx_handle, 0, 3, enable_all);


    /* enable interrupt */
    odtu_dmx_handle->var.isr_mode = enable;

    odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_set( NULL, odtu_dmx_handle, enable);
    if( enable ) {
      enable_all[0] = 0xFFFFFFFF;
      enable_all[1] = 0xFFFFFFFF;
      enable_all[2] = 0xFFFFFFFF;
    } else {
      enable_all[0] = 0;
      enable_all[0] = 0;
      enable_all[0] = 0;
    }
    odtu_dmx_lfield_HAO_STATUS_AVL_E_set( NULL, odtu_dmx_handle, enable_all);
    odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_set( NULL, odtu_dmx_handle, enable_all);
    
    PMC_RETURN();
} /* odtu_dmx_ghao_ghao_isr_enable */  

/*******************************************************************************
*  odtu_dmx_ghao_wait_event
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - wait for interrupt or poll for interrupt summary based on isr context
*   - get list of affected hao trib_slot 
*   - compare list with input trib_slot if not in discovery mode context
*   - initiate transfer_rcoh_status to register
*   - read concerned rcoh status registers
*   - compare extracted chnl_id with input if not in discovery mode context
*   - check for consistency of RCOH among the captured RCOH on different TS
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   trib_slot           - list of tributary slot expected if not in discovery mode
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first Tributary slot
*   lo_chnl_id          - LO channel id associated with first Tributary slot 
*   trib_slot           - list of tributary slot affected if in discovery mode
*   ref_rp              - rp value of extracted RCOH
*   ref_ctrl            - ctrl command extracted RCOH
*   ref_tpid            - tpid value of extracted RCOH
*   ref_tsgs            - tsgs value of extracted RCOH
*   ref_tscc            - tscc value of extracted RCOH
*   first_ts            - index of first tributary slot
*   same_rcoh           - all capture RCOH on the different ts have the same info
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * contradicting TPID in list of extracted RCOH
*       * contradicting ctrl command in list of extracted RCOH
*       * contradicting rp in list of extracted RCOH
*       * contradicting tsgs in list of extracted RCOH
*       * contradicting tscc in list of extracted RCOH
*       * contradicting trib_slot if not in discovery mode
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_event(odtu_dmx_handle_t *odtu_dmx_handle,
                                          UINT8 *ho_chnl_id,
                                          UINT8 *lo_chnl_id,
                                          BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                          BOOL8 *ref_rp,
                                          ghao_lcr_ctrl_t *ref_ctrl,
                                          UINT8 *ref_tpid,
                                          BOOL8 *ref_tsgs,
                                          BOOL8 *ref_tscc,
                                          UINT8 *first_ts,
                                          BOOL8 *same_rcoh)
{
    PMC_ENTRY();
    PMC_ERROR  result = PMC_SUCCESS;
    
    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            ho_chnl_id, 
                                            lo_chnl_id, 
                                            trib_slot, 
                                            ref_rp, 
                                            ref_ctrl, 
                                            ref_tpid, 
                                            ref_tsgs, 
                                            ref_tscc, 
                                            first_ts, 
                                            same_rcoh,
                                            0);
    
    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_event */

/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_event
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - wait for interrupt or poll for interrupt summary based on isr context
*   - get list of affected hao trib_slot 
*   - compare list with input trib_slot if not in discovery mode context
*   - initiate transfer_rcoh_status to register
*   - read concerned rcoh status registers
*   - compare extracted chnl_id with input if not in discovery mode context
*   - check for consistency of RCOH among the captured RCOH on different TS
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   trib_slot           - list of tributary slot expected if not in discovery mode
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first Tributary slot
*   lo_chnl_id          - LO channel id associated with first Tributary slot 
*   trib_slot           - list of tributary slot affected if in discovery mode
*   ref_rp              - rp value of extracted RCOH
*   ref_ctrl            - ctrl command extracted RCOH
*   ref_tpid            - tpid value of extracted RCOH
*   ref_tsgs            - tsgs value of extracted RCOH
*   ref_tscc            - tscc value of extracted RCOH
*   first_ts            - index of first tributary slot
*   same_rcoh           - all capture RCOH on the different ts have the same info
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * contradicting TPID in list of extracted RCOH
*       * contradicting ctrl command in list of extracted RCOH
*       * contradicting rp in list of extracted RCOH
*       * contradicting tsgs in list of extracted RCOH
*       * contradicting tscc in list of extracted RCOH
*       * contradicting trib_slot if not in discovery mode
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle_t *odtu_dmx_handle,
                                                UINT8 *ho_chnl_id,
                                                UINT8 *lo_chnl_id,
                                                BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                BOOL8 *ref_rp,
                                                ghao_lcr_ctrl_t *ref_ctrl,
                                                UINT8 *ref_tpid,
                                                BOOL8 *ref_tsgs,
                                                BOOL8 *ref_tscc,
                                                UINT8 *first_ts,
                                                BOOL8 *same_rcoh,
                                                UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    BOOL8 cfg_dne_0_31;
    BOOL8 cfg_dne_32_63;
    BOOL8 cfg_dne_64_95;
    BOOL8 hao_st_avl_0_31;
    BOOL8 hao_st_avl_32_63;
    BOOL8 hao_st_avl_64_95;
    BOOL8 rcoh_st_trsnfr_dne;
    BOOL8 l_hao_cs[ODTU_DMX_NUM_CHANL];
    BOOL8 l_hao_ts[ODTU_DMX_NUM_CHANL];
    UINT32 i, j;
    BOOL8 first_rcoh_read;
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    UINT8 tpid;
    BOOL8 tsgs;
    BOOL8 tscc;

    /* Check which ghao interrup has been triggered */              
    result = odtu_dmx_chk_summary_ghao_interrupt (odtu_dmx_handle,
                                                  &cfg_dne_0_31,
                                                  &cfg_dne_32_63,
                                                  &cfg_dne_64_95,
                                                  &hao_st_avl_0_31,
                                                  &hao_st_avl_32_63,
                                                  &hao_st_avl_64_95,
                                                  &rcoh_st_trsnfr_dne);
    if( result != PMC_SUCCESS ) { 
      PMC_LOG_TRACE("ODTU_DMX no hao interrupt ");
      PMC_RETURN( result ); 
    }

    /* Get list of affected hao trib slot */
    result = odtu_dmx_get_hao_ts (odtu_dmx_handle,
                                  hao_st_avl_0_31,
                                  hao_st_avl_32_63,
                                  hao_st_avl_64_95,
                                  ho_chnl_id,
                                  l_hao_cs,
                                  l_hao_ts);
    
    if( result != PMC_SUCCESS ) { 
      PMC_LOG_TRACE("ODTU_DMX can't get hao ts ");
      PMC_RETURN( result ); 
    }

    /* compare list with input trib_slot if not in discovery mode context */
    if( odtu_dmx_handle->ghao_dmx_state[ghao_index].discovery_mode )
    {
      for (i = 0; i < 96; i++) 
      {
        trib_slot[i] = l_hao_ts[i];
      }
    } else {
#ifndef PMC_SW_SIMULATION
      for (i = 0; i < 96; i++) 
      {
        if(trib_slot[i] != l_hao_ts[i])
        {
          result = PMC_ERR_FAIL;
          break;
        }
      }
#endif
    }
    if( result != PMC_SUCCESS ) { 
      PMC_LOG_TRACE("ODTU_DMX trib_slot and l_hao_ts mismatch ");
      PMC_RETURN( result ); 
    }
  
    /* initiate transfer_rcoh_status to register */
    result = odtu_dmx_transfer_rcoh_status(odtu_dmx_handle);
    if( result != PMC_SUCCESS ) { 
      PMC_LOG_TRACE("ODTU_DMX can't initiate transfer ");
      PMC_RETURN( result ); 
    }


    /* read concerned rcoh status registers */
    first_rcoh_read = TRUE;
    *same_rcoh = TRUE;

    for (i = 0; i < 96; i++) 
    {   
      if( l_hao_cs[i] ) 
      {
        result = odtu_dmx_get_rcoh (odtu_dmx_handle,
                                    i,
                                    &rp,
                                    &ctrl,
                                    &tpid,
                                    &tsgs,
                                    &tscc);
        if( result != PMC_SUCCESS ) { 
          PMC_LOG_TRACE("ODTU_DMX can't get rcoh status ");
          PMC_RETURN( result ); 
        }

        PMC_LOG_TRACE("ODTU_DMX get rcoh status RP   : %d on cs %d\n", rp, i);
        PMC_LOG_TRACE("ODTU_DMX get rcoh status CTRL : %d on cs %d\n", ctrl, i);
        PMC_LOG_TRACE("ODTU_DMX get rcoh status TPID : %d on cs %d\n", tpid, i);
        PMC_LOG_TRACE("ODTU_DMX get rcoh status TSGS : %d on cs %d\n", tsgs, i);
        PMC_LOG_TRACE("ODTU_DMX get rcoh status TSCC : %d on cs %d\n", tscc, i);
       
        if (first_rcoh_read)
        {
          *ref_rp = rp;
          *ref_ctrl = ctrl;
          *ref_tpid = tpid;
          *ref_tsgs = tsgs;
          *ref_tscc = tscc;
          *first_ts = i;
          first_rcoh_read = FALSE;

          /* search for lo_chnl_id */
          for (j = 0; j < 96; j++) {
            if( odtu_dmx_handle->var.lo_chnl_cfg[j].ts_port == tpid && 
                odtu_dmx_handle->var.lo_chnl_cfg[j].ho_chnl_id == *ho_chnl_id ) {
              *lo_chnl_id = j;
              break;
            }
          }
          if( j > 95 ) {
            if( rp == 0 ) {
              /* information of RCOH may become invalid when rp goes to 0*/
              *lo_chnl_id = 0x7f;
            } else {
              result = PMC_ERR_FAIL;
              *lo_chnl_id = 0x7f;
              odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE);
              PMC_LOG_TRACE("WARNINGS: ODTU_DMX unable to find matching tpid to lo_chnl_id");
              PMC_RETURN( result ); 
            }
          }

        } else {
          /* check that rcoh status are consistent */
          result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, *ref_rp, rp, i);
          if( result != PMC_SUCCESS ) { 
            *same_rcoh = FALSE;
            PMC_RETURN( PMC_SUCCESS );  /* FAKE SUCCESS */
          }

          result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, *ref_ctrl, ctrl, i);
          if( result != PMC_SUCCESS ) {  
            *same_rcoh = FALSE;
            PMC_RETURN( PMC_SUCCESS );  /* FAKE SUCCESS */
          }

          result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, *ref_tsgs, tsgs, i);
          if( result != PMC_SUCCESS ) {  
            *same_rcoh = FALSE;
            PMC_RETURN( PMC_SUCCESS );  /* FAKE SUCCESS */
          }

          result = odtu_dmx_ghao_chk_tscc(odtu_dmx_handle, *ref_tscc, tscc, i);
          if( result != PMC_SUCCESS ) {  
            *same_rcoh = FALSE;
            PMC_RETURN( PMC_SUCCESS );  /* FAKE SUCCESS */
          }

          result = odtu_dmx_ghao_chk_tpid(odtu_dmx_handle, *ref_tpid, tpid, i);
          if( result != PMC_SUCCESS ) {  
            *same_rcoh = FALSE;
            PMC_RETURN( PMC_SUCCESS );  /* FAKE SUCCESS */
          }
        }
      }
    }


    PMC_RETURN(result);
} /* odtu_dmx_multi_ghao_wait_event */  

/*******************************************************************************
*  odtu_dmx_ghao_wait_for_add_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an ADD NACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an ADD NACK message (rp =1, ctrl=ADD, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_add_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 *ho_chnl_id,
                                                 UINT8 *lo_chnl_id,
                                                 UINT8 *tpid,
                                                 BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{  
    PMC_ENTRY();
    
    PMC_ERROR  result = PMC_SUCCESS;
    BOOL8 same_rcoh;
    
    result = odtu_dmx_multi_ghao_wait_for_add_nack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);
    
    if( same_rcoh == FALSE ) {
      PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_ADD_NACK : different RCOH information found on multiple TS");
      result = PMC_ERR_FAIL;
    }
    
    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_add_nack */


/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_add_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an ADD NACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an ADD NACK message (rp =1, ctrl=ADD, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_add_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                       UINT8 *ho_chnl_id,
                                                       UINT8 *lo_chnl_id,
                                                       UINT8 *tpid,
                                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                       BOOL8 *same_rcoh,
                                                       UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
  
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    /* UINT8 tpid; */
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;


    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;
              
    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid, /* actually tpid */
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK :wait_event returns  first_ts : %d\n", first_ts);

    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT ADD NACK Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_ADD_NACK;
      bwr_state = GHAO_BWR_STATE_LCR;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;
       
      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      /* Checking RCOH */

      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, TRUE, rp, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : unexpected RP");
          PMC_RETURN( result ); 
        } 
      else 
        {
          var_rp = rp;
        }
      
      result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, GHAO_LCR_CTRL_ADD, ctrl, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : unexpected CTRL");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_ctrl = ctrl;
        }
      
      result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, FALSE, tsgs, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : unexpected TSGS");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_tsgs = tsgs;
        }

      /* update state */

      if( lcr_state == GHAO_LCR_STATE_IDLE ) {
        lcr_state = GHAO_LCR_STATE_ADD_NACK;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : unexpected lcr_state from var\n");
        lcr_state = GHAO_LCR_STATE_ADD_NACK;
      }

      if( bwr_state == GHAO_BWR_STATE_IDLE ) {
        bwr_state = GHAO_BWR_STATE_LCR;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : unexpected lcr_state from var\n");
        bwr_state = GHAO_BWR_STATE_LCR;
      }

      if( *ho_chnl_id != extract_ho_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : ERROR expected ho_chnl_id %d, but extracted %d\n", *ho_chnl_id, extract_ho_chnl_id);
        result = PMC_ERR_FAIL;
      }

      if( *lo_chnl_id != extract_lo_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : ERROR expected lo_chnl_id %d, but extracted %d\n", *lo_chnl_id, extract_lo_chnl_id);
        result = PMC_ERR_FAIL;
      } else {
        var_chnl_id = extract_lo_chnl_id;
      }

      if( *tpid != extract_tpid ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD NACK : WARNING expected tpid %d, but extracted %d\n", *tpid, extract_tpid);
      } else {
        var_tpid = extract_tpid;
      }
    
      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }
    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);
                            

    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_add_nack */  


/*******************************************************************************
*  odtu_dmx_ghao_update_stdby_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Assumptions:
*   - Assumes that caller is aware of configuration context and reprovision the other channels
*     into the stdby pages with the following functions before calling this function:
*   - calls all odtu_dmx_ho_chnl_offline_update that writes to HO config info into stdby pages
*     is previously called
*   - calls that odtu_dmx_lo_chnl_offline_update that writes to LO config info into stdby pages 
*     is previously called
*   Configure the stanby configuration RAM as follow:
*   - calls odtu_dmx_ho_chnl_offline_update to write HO config info into stdby pages
*   - adjust memory context for reprovisioning
*   - calls odtu_dmx_lo_offline_deprov to remove old config from the database
*   - calls odtu_dmx_lo_chnl_prov to provision new lo channel configuration, 
*     with ghao_reprov=1 which do not reprogram the mp_mgen IPT table
*   - calls sts_sw_cfg to write configuration into standby RAM
*   Trigger odtu_dmx_reconfig_status_update with GHAO mode (swich on knife edge)
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - High order channel id
*   lo_chnl_id          - Low order channel id
*   ghao_trib_slot      - postion of tributary slots within the HO container on which ghao protocol 
*                         is adding or removing, using a list of bool format
*   stage_inst          - stage instance
*   add                 - when 1, adding tributary slot, when 0, removing ts
*   old_trib_slot_mask  - current position of the tributary slot within ho container
*                         in a list of 3x32 bit format
*   msi_data            - Tributary port ID (TPID) used in the added/removed TS
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR   - if updating standby page configuration was unsuccessful
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_update_stdby_config(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT8 *ho_chnl_id,   
                                                   UINT8 *lo_chnl_id,   
                                                   BOOL8 ghao_trib_slot[ODTU_DMX_NUM_CHANL],
                                                   odtu_dmx_stage_instance_t stage_inst,
                                                   BOOL8 add,
                                                   UINT32 old_trib_slot_mask[3],
                                                   UINT8 msi_data)
{ 
    PMC_ENTRY();
    
    PMC_ERROR  result = PMC_SUCCESS;
    UINT32 ho_config_addr_st;


    /* force offline page update to not use the firmware Q */
    l5sys_firmware_queue_disable(odtu_dmx_handle);

    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, *ho_chnl_id);

    ho_config_addr_st = odtu_dmx_field_HO_CONFIG_ADDR_ST_get(NULL, odtu_dmx_handle);

    
    odtu_dmx_field_FORCE_CONFIG_ADDR_set(NULL, odtu_dmx_handle, (~ho_config_addr_st & 0x1));

    odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 1);


    result = odtu_dmx_multi_ghao_update_stdby_config(odtu_dmx_handle,
                                                     ho_chnl_id,
                                                     lo_chnl_id,
                                                     ghao_trib_slot,
                                                     stage_inst,
                                                     add,
                                                     old_trib_slot_mask,
                                                     msi_data,
                                                     0);
    
    odtu_dmx_field_FORCE_CONFIG_set(NULL, odtu_dmx_handle, 0);

    /*enable the firmware queue*/
    l5sys_firmware_queue_enable(odtu_dmx_handle);


    PMC_RETURN(result);
} /* odtu_dmx_ghao_update_stdby_config */


/*******************************************************************************
*  odtu_dmx_multi_ghao_update_stdby_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Configure the stanby configuration RAM as follow:
*   - calls odtu_dmx_ho_chnl_offline_update to write HO config info into stdby pages
*   - adjust memory context for reprovisioning
*   - calls odtu_dmx_lo_offline_deprov to remove old config from the database
*   - calls odtu_dmx_lo_chnl_prov to provision new lo channel configuration, 
*     with ghao_reprov=1 which do not reprogram the mp_mgen IPT table
*   - calls sts_sw_cfg to write configuration into standby RAM
*   Trigger odtu_dmx_reconfig_status_update with GHAO mode (swich on knife edge)
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - High order channel id
*   lo_chnl_id          - Low order channel id
*   ghao_trib_slot      - postion of tributary slots within the HO container on which ghao protocol 
*                         is adding or removing, using a list of bool format
*   stage_inst          - stage instance
*   add                 - when 1, adding tributary slot, when 0, removing ts
*   old_trib_slot_mask  - current position of the tributary slot within ho container
*                         in a list of 3x32 bit format
*   msi_data            - Tributary port ID (TPID) used in the added/removed TS
*   ghao_index          - G.HAO context variable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_update_stdby_config(odtu_dmx_handle_t *odtu_dmx_handle,
                                                         UINT8 *ho_chnl_id,   
                                                         UINT8 *lo_chnl_id,   
                                                         BOOL8 ghao_trib_slot[ODTU_DMX_NUM_CHANL],
                                                         odtu_dmx_stage_instance_t stage_inst,
                                                         BOOL8 add,
                                                         UINT32 old_trib_slot_mask[3],
                                                         UINT8 msi_data,
                                                         UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 lo_chnl;

    UINT32 new_tribslot_mask[3];
    UINT8 new_num_ts;
    UINT8 total_num_ts;
    UINT32 client_bitrate;
    UINT8 i;

    /* argument checking */
    PMC_ASSERT(old_trib_slot_mask!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);


    /* ------------------------------------------------------------------------- */
    /* DEBUG PRINT */
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : PARAM");
    PMC_LOG_TRACE("param : lo_chnl_id : %u\n", *lo_chnl_id);
    PMC_LOG_TRACE("param : ho_chnl_id : %u\n", *ho_chnl_id);
    for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {
      PMC_LOG_TRACE("param : ghao_trib_slot[%u] = %d\n",i, ghao_trib_slot[i]);
    }
    PMC_LOG_TRACE("param : stage_inst : %d\n", stage_inst);
    PMC_LOG_TRACE("param : add : %d\n", add);
    PMC_LOG_TRACE("param : old_trib_slot_mask[0] : %X\n", old_trib_slot_mask[0]);
    PMC_LOG_TRACE("param : old_trib_slot_mask[1] : %X\n", old_trib_slot_mask[1]);
    PMC_LOG_TRACE("param : old_trib_slot_mask[2] : %X\n", old_trib_slot_mask[2]);
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : ODTU_DMX HO_CHAN_ID STATE ORIG");
    odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, FALSE );
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : STATE \n");
    /* DEBUG PRINT */
    /* ------------------------------------------------------------------------- */

    /* count number of trib slot to add/remove */
    new_num_ts = 0;
    
    if((odtu_dmx_handle->var.ho_chnl_cfg[*ho_chnl_id].state == UTIL_GLOBAL_CHNL_OPERATIONAL) && 
       (odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].state == UTIL_GLOBAL_CHNL_OPERATIONAL) ) {

      /* reconfig mode to 1 for HAO so it reconfigures after receiving NORM and next RMF */
      odtu_dmx_reconfig_mode_cfg( odtu_dmx_handle, *ho_chnl_id, 1 ); 

      /*
      odtu_dmx_ho_chnl_offline_update( odtu_dmx_handle,
                                       stage_inst,
                                       *ho_chnl_id,
                                       odtu_dmx_handle->var.ho_chnl_cfg[*ho_chnl_id].oduk_type,
                                       odtu_dmx_handle->var.ho_chnl_cfg[*ho_chnl_id].ts_type);
      */

      new_tribslot_mask[0] = old_trib_slot_mask[0];  
      new_tribslot_mask[1] = old_trib_slot_mask[1];
      new_tribslot_mask[2] = old_trib_slot_mask[2];
      PMC_LOG_TRACE("new_trib_slot[0] = %X\n", new_tribslot_mask[0]);
      PMC_LOG_TRACE("new_trib_slot[1] = %X\n", new_tribslot_mask[1]);
      PMC_LOG_TRACE("new_trib_slot[2] = %X\n", new_tribslot_mask[2]);
                
      /* adjust trib slot position for lo channels */
      for (lo_chnl = 0; lo_chnl < ODTU_DMX_NUM_CHANL; lo_chnl++) {
        if( ghao_trib_slot[lo_chnl] == TRUE ) { 
          new_num_ts += 1;
          if( add ) {
            new_tribslot_mask[lo_chnl/32] |= (1<< (lo_chnl%32));
            PMC_LOG_TRACE("new_trib_slot[%d] changed to = %X on index %d with offset %d\n", (lo_chnl/32),new_tribslot_mask[lo_chnl/32], lo_chnl, (lo_chnl%32));
          } else {
            new_tribslot_mask[lo_chnl/32] &= (0xFFFFFFFF & ~(1<< (lo_chnl%32)));
            PMC_LOG_TRACE("new_trib_slot[%d] changed to = %X on index %d with offset %d\n", (lo_chnl/32),new_tribslot_mask[lo_chnl/32], lo_chnl, (lo_chnl%32));
          }
        }
      }
      PMC_LOG_TRACE("new_trib_slot[0] = %X\n", new_tribslot_mask[0]);
      PMC_LOG_TRACE("new_trib_slot[1] = %X\n", new_tribslot_mask[1]);
      PMC_LOG_TRACE("new_trib_slot[2] = %X\n", new_tribslot_mask[2]);

      /* tribslot_cfg will re-add them properly */
      odtu_dmx_handle->var.ho_chnl_cfg[*ho_chnl_id].num_active_ts -= odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].num_ts;

      /* GHAO add/remove */
      if( add ) {
        odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].num_ts += new_num_ts;
      } else {
        odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].num_ts -= new_num_ts;
      }

      /* Get target active num_ts*/
      total_num_ts = odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].num_ts;

      /* Use special Deprov that will no interfere with current datastream and corrupt var context*/
      odtu_dmx_ghao_lo_chnl_deprov( odtu_dmx_handle,
                                    stage_inst,
                                    *lo_chnl_id,
                                    *ho_chnl_id);

      if (total_num_ts <= 8) {
        client_bitrate = (UINT32)((total_num_ts * UTIL_GLOBAL_ODU2_TS_RATE)/1000);
      } else if (total_num_ts <= 32) {
        client_bitrate = (UINT32)((total_num_ts * UTIL_GLOBAL_ODU3_TS_RATE)/1000);
      } else {
        client_bitrate = (UINT32)((total_num_ts * UTIL_GLOBAL_ODU4_TS_RATE)/1000);
      }         

      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : lo_chnl_prov param : lo_chnl_id     : %d\n", *lo_chnl_id);
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : lo_chnl_prov param : total_num_ts   : %d\n", total_num_ts);
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : lo_chnl_prov param : client_bitrate : %d\n", client_bitrate);
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : lo_chnl_prov param : msi_data       : %d\n", msi_data);
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : lo_chnl_prov param : ho_chnl_id     : %d\n", *ho_chnl_id);

      odtu_dmx_lo_chnl_prov( odtu_dmx_handle,
                             *lo_chnl_id,
                             stage_inst,
                             UTIL_GLOBAL_ODUFLEX_GFP,
                             total_num_ts,
                             client_bitrate ,
                             odtu_dmx_handle->var.ho_chnl_cfg[*ho_chnl_id].oduk_type ,
                             odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].mapping_mode,
                             new_tribslot_mask,
                             msi_data,                                           
                             odtu_dmx_handle->var.ho_chnl_cfg[*ho_chnl_id].ts_type,
                             *ho_chnl_id,
                             0,
                             1);
      
      odtu_dmx_handle->var.lo_chnl_cfg[*lo_chnl_id].state = UTIL_GLOBAL_CHNL_OPERATIONAL;

    } else {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : Trying to configure a lo channel that is not operational in ghao ");
      odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
      PMC_RETURN( result );               
    }
       
    sts_sw_cfg(odtu_dmx_handle->sts_sw_handle, *ho_chnl_id, FALSE);
    odtu_dmx_reconfig_status_update( odtu_dmx_handle, *ho_chnl_id, 1);


    /* ------------------------------------------------------------------------- */
    /* DEBUG PRINT */
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : PARAM");
    PMC_LOG_TRACE("param : lo_chnl_id : %u\n", *lo_chnl_id);
    PMC_LOG_TRACE("param : ho_chnl_id : %u\n", *ho_chnl_id);
    for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {
      PMC_LOG_TRACE("param : ghao_trib_slot[%u] = %d\n",i, ghao_trib_slot[i]);
    }
    PMC_LOG_TRACE("param : stage_inst : %d\n", stage_inst);
    PMC_LOG_TRACE("param : add : %d\n", add);
    PMC_LOG_TRACE("param : old_trib_slot_mask[0] : %X\n", old_trib_slot_mask[0]);
    PMC_LOG_TRACE("param : old_trib_slot_mask[1] : %X\n", old_trib_slot_mask[1]);
    PMC_LOG_TRACE("param : old_trib_slot_mask[2] : %X\n", old_trib_slot_mask[2]);
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : ODTU_DMX HO_CHAN_ID STATE UPDATED\n");
    odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, FALSE );
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : STATE UPDATED\n");
    /* DEBUG PRINT */
    /* ------------------------------------------------------------------------- */

    PMC_RETURN(result);

} /* odtu_dmx_update_stdby_config */  


/*******************************************************************************
*  odtu_dmx_ghao_update_stdby_config_after_knife_edge
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Configure the stanby configuration RAM as follow:
*   - calls odtu_dmx_ho_chnl_offline_update to write HO config info into stdby pages
*   - adjust memory context for reprovisioning
*   - calls odtu_dmx_lo_offline_deprov to remove old config from the database
*   - calls odtu_dmx_lo_chnl_prov to provision new lo channel configuration, 
*     with ghao_reprov=1 which do not reprogram the mp_mgen IPT table
*   - calls sts_sw_cfg to write configuration into standby RAM
*   Trigger odtu_dmx_reconfig_status_update with GHAO mode (swich on knife edge)
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   stage_inst          - stage instance
*   ho_chnl_id          - High order channel id
*   lo_chnl_id          - Low order channel id
*   num_ts              - Number of TS after knife edge
*   new_tribslot_mask   - current position of the tributary slot within ho container
*                         in a list of 3x32 bit format
*   msi_data            - Tributary port ID (TPID) used in the added/removed TS
*   add                 - When 1, adding tributary slots, when 0, removing ts
*   cal_to_add_remove_num  -Number of calendar slots to be added/removed
*   cal_slots_add_rem   - Calendar slots that have been added/removed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_update_stdby_config_after_knife_edge(odtu_dmx_handle_t *odtu_dmx_handle,
                                                                    odtu_dmx_stage_instance_t stage_inst,
                                                                    UINT8 ho_chnl_id,   
                                                                    UINT8 lo_chnl_id,   
                                                                    UINT32 num_ts,
                                                                    UINT32 new_tribslot_mask[3],
                                                                    UINT8 msi_data,
                                                                    BOOL8 add,
                                                                    UINT32 cal_to_add_remove_num,
                                                                    UINT32 cal_slots_add_rem[79])
{
    PMC_ERROR result = PMC_SUCCESS;

    /* variable declaration */
    UINT32 client_bitrate;
    UINT32 cal_pos;

    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(new_tribslot_mask!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0, 0);


    if (num_ts <= 8) {
      client_bitrate = (UINT32)((num_ts * UTIL_GLOBAL_ODU2_TS_RATE)/1000);
    } else if (num_ts <= 32) {
      client_bitrate = (UINT32)((num_ts * UTIL_GLOBAL_ODU3_TS_RATE)/1000);
    } else {
      client_bitrate = (UINT32)((num_ts * UTIL_GLOBAL_ODU4_TS_RATE)/1000);
    }         

    /* ------------------------------------------------------------------------- */
    /* DEBUG PRINT */
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG AFTER KNIFE EDGE : PARAM\n");
    PMC_LOG_TRACE("param : ho_chnl_id : %u\n", ho_chnl_id);
    PMC_LOG_TRACE("param : lo_chnl_id : %u\n", lo_chnl_id);
    PMC_LOG_TRACE("param : num_ts : %u\n", num_ts);
    PMC_LOG_TRACE("param : stage_inst : %d\n", stage_inst);
    PMC_LOG_TRACE("param : num_ts : %d\n", num_ts);
    PMC_LOG_TRACE("param : msi_data : %d\n", msi_data);
    PMC_LOG_TRACE("param : add : %d\n", add);
    PMC_LOG_TRACE("param : cal_to_add_remove_num : %d\n", cal_to_add_remove_num);
    PMC_LOG_TRACE("param : new_tribslot_mask[0] : %X\n", new_tribslot_mask[0]);
    PMC_LOG_TRACE("param : new_tribslot_mask[1] : %X\n", new_tribslot_mask[1]);
    PMC_LOG_TRACE("param : new_tribslot_mask[2] : %X\n", new_tribslot_mask[2]);
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG : lo_chnl_prov param : client_bitrate : %d\n", client_bitrate);
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG AFTER KNIFE EDGE : ODTU_DMX HO_CHAN_ID STATE ORIG\n");
    odtu_dmx_dump_var_context( odtu_dmx_handle, ho_chnl_id, lo_chnl_id, FALSE );
    PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG AFTER KNIFE EDGE : STATE \n");
    /* DEBUG PRINT */
    /* ------------------------------------------------------------------------- */


    if((odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].state == UTIL_GLOBAL_CHNL_OPERATIONAL) && 
       (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].state == UTIL_GLOBAL_CHNL_OPERATIONAL) )
    {
        if (add == FALSE)
        {
            /* Deprovision IPT for removed calendar slots */
            for (cal_pos = 0; cal_pos < cal_to_add_remove_num; cal_pos ++)
            {
                PMC_LOG_TRACE("odtu_dmx_ghao_update_stdby_config_after_knife_edge(): Disable cal_slots[%d]=%d\n", cal_pos, cal_slots_add_rem[cal_pos]);
                if (result == PMC_SUCCESS)
                {
                   result = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                               cal_slots_add_rem[cal_pos],
                                               UTIL_GLOBAL_NO_MAP,
                                               0, 0, 0);
                }

                if (result == PMC_SUCCESS)
                {
                    result = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, ho_chnl_id,
                                                cal_slots_add_rem[cal_pos],
                                                0x7F, 0, FALSE);   
                }  
            }
        }


        /* Reprovision standby page for LO channel */
        if (result == PMC_SUCCESS)
        {
            result = odtu_dmx_lo_chnl_offline_update(odtu_dmx_handle,
                                                     lo_chnl_id,
                                                     stage_inst,
                                                     UTIL_GLOBAL_ODUFLEX_GFP,
                                                     num_ts,
                                                     client_bitrate ,
                                                     odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type ,
                                                     odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].mapping_mode,
                                                     new_tribslot_mask,
                                                     msi_data,                                           
                                                     odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].ts_type,
                                                     ho_chnl_id);
        }

        odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].state = UTIL_GLOBAL_CHNL_OPERATIONAL;

    }
    else
    {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX GHAO UPDATE STDBY CONFIG AFTER KNIFE EDGE : Trying to configure a lo channel that is not operational in ghao ");
      odtu_dmx_dump_var_context( odtu_dmx_handle, ho_chnl_id, lo_chnl_id, TRUE );
      PMC_RETURN( result );               
    }
       

    PMC_RETURN(result);

} /* odtu_dmx_ghao_update_stdby_config_after_knife_edge */  


/*******************************************************************************
*  odtu_dmx_ghao_wait_for_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an ADD ACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an ADD ACK message (rp =1, ctrl=ADD, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_add_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                UINT8 *ho_chnl_id,
                                                UINT8 *lo_chnl_id,
                                                UINT8 *tpid,
                                                BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
  
    PMC_ENTRY();
    
    PMC_ERROR  result = PMC_SUCCESS;
    BOOL8 same_rcoh;
    
    result =  odtu_dmx_multi_ghao_wait_for_add_ack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);
    
    if( same_rcoh == FALSE ) {
      PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_ADD_ACK : different RCOH information found on multiple TS");
      result = PMC_ERR_FAIL;
    }
    
    PMC_RETURN(result);
}     

/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an ADD ACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an ADD ACK message (rp =1, ctrl=ADD, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_add_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                      UINT8 *ho_chnl_id,
                                                      UINT8 *lo_chnl_id,
                                                      UINT8 *tpid,
                                                      BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                      BOOL8 *same_rcoh,
                                                      UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;
              
    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;

    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid, /* actually tpid */
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK :wait_event returns  first_ts : %d\n", first_ts);

    /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    /* update state */

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT ADD ACK Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_ADD_ACK;
      bwr_state = GHAO_BWR_STATE_LCR;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      /* Checking RCOH */
      /* DO SOME CHECKING */

      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, TRUE, rp, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : unexpected RP\n");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_rp = rp;
        }

      result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, GHAO_LCR_CTRL_ADD, ctrl, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : unexpected CTRL\n");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_ctrl = ctrl;
        }

      result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, TRUE, tsgs, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : unexpected TSGS\n");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_tsgs = tsgs;
        }


      if( lcr_state == GHAO_LCR_STATE_ADD_NACK ) {
        lcr_state = GHAO_LCR_STATE_ADD_ACK;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : WARNING unexpected lcr_state from var\n");
        lcr_state = GHAO_LCR_STATE_ADD_ACK;
      }

      if( bwr_state == GHAO_BWR_STATE_LCR ) {
        bwr_state = GHAO_BWR_STATE_LCR;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : WARNING unexpected bwr_state from var\n");
        bwr_state = GHAO_BWR_STATE_LCR;
      }
    
      if( *ho_chnl_id != extract_ho_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : ERROR expected ho_chnl_id %d, but extracted %d\n", *ho_chnl_id, extract_ho_chnl_id);
        result = PMC_ERR_FAIL;
      }

      if( *lo_chnl_id != extract_lo_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : ERROR expected lo_chnl_id %d, but extracted %d\n", *lo_chnl_id, extract_lo_chnl_id);
        result = PMC_ERR_FAIL;
      } else {
        var_chnl_id = extract_lo_chnl_id;
      }

      if( *tpid != extract_tpid ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT ADD ACK : WARNING expected tpid %d, but extracted %d\n", *tpid, extract_tpid);
      } else {
        var_tpid = extract_tpid;
      }
   
      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }
    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);

    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_add_ack */  


/*******************************************************************************
*  odtu_dmx_ghao_wait_for_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an NORM ACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not a NORM ACK message (rp =1, ctrl=NORM, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_norm_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 *ho_chnl_id,
                                                 UINT8 *lo_chnl_id,
                                                 UINT8 *tpid,
                                                 BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
  
  PMC_ENTRY();
  
  PMC_ERROR  result = PMC_SUCCESS;
  BOOL8 same_rcoh;
  
  result = odtu_dmx_multi_ghao_wait_for_norm_ack(odtu_dmx_handle, ho_chnl_id,lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);

  if( same_rcoh == FALSE ) {
    PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_NORM_ACK : different RCOH information found on multiple TS");
    result = PMC_ERR_FAIL;
  }

  
  PMC_RETURN(result);
}     


/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an NORM ACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not a NORM ACK message (rp =1, ctrl=NORM, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_norm_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                       UINT8 *ho_chnl_id,
                                                       UINT8 *lo_chnl_id,
                                                       UINT8 *tpid,
                                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                       BOOL8 *same_rcoh,
                                                       UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;

    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;
              
    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid,  /* actually tpid */
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK :wait_event returns  first_ts : %d\n", first_ts);

    /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    /* update state */

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT NORM ACK Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_NORM;
      bwr_state = GHAO_BWR_STATE_LCR;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      /* Checking RCOH */
      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, TRUE, rp, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : unexpected RP\n");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_rp = rp;
        }

      result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, GHAO_LCR_CTRL_NORM, ctrl, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : unexpected CTRL\n");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_ctrl = ctrl;
        }

      result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, TRUE, tsgs, first_ts);
      if( result != PMC_SUCCESS ) 
        { 
          PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : unexpected TSGS\n");
          PMC_RETURN( result ); 
        }
      else 
        {
          var_tsgs = tsgs;
        }

      if( lcr_state == GHAO_LCR_STATE_ADD_ACK || lcr_state == GHAO_LCR_STATE_REMOVE_ACK ) {
        lcr_state = GHAO_LCR_STATE_NORM;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : WARNING unexpected lcr_state from var\n");
        lcr_state = GHAO_LCR_STATE_NORM;
      }

      if( bwr_state == GHAO_BWR_STATE_LCR ) {
        bwr_state = GHAO_BWR_STATE_LCR;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : WARNING unexpected bwr_state from var\n");
        bwr_state = GHAO_BWR_STATE_LCR;
      }
      
      if( *ho_chnl_id != extract_ho_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : ERROR expected ho_chnl_id %d, but extracted %d\n", *ho_chnl_id, extract_ho_chnl_id);
        result = PMC_ERR_FAIL;
      }

      if( *lo_chnl_id != extract_lo_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : ERROR expected lo_chnl_id %d, but extracted %d\n", *lo_chnl_id, extract_lo_chnl_id);
        result = PMC_ERR_FAIL;
      } else {
        var_chnl_id = extract_lo_chnl_id;
      }


      if( *tpid != extract_tpid ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT NORM ACK : WARNING expected tpid %d, but extracted %d\n", *tpid, extract_tpid);
      } else {
        var_tpid = extract_tpid;
      }
    
      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }
    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);

    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_norm_ack */  

/*******************************************************************************
*  odtu_dmx_ghao_wait_for_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an IDLE NACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an IDLE NACK message (rp =1, ctrl=IDLE, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_idle_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT8 *ho_chnl_id,
                                                  UINT8 *lo_chnl_id,
                                                  UINT8 *tpid,
                                                  BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])

{
  
  PMC_ENTRY();
  
  PMC_ERROR  result = PMC_SUCCESS;
  BOOL8 same_rcoh;
  
  result = odtu_dmx_multi_ghao_wait_for_idle_nack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);
  
  if( same_rcoh == FALSE ) {
    PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_IDLE_NACK : different RCOH information found on multiple TS");
    result = PMC_ERR_FAIL;
  }

  PMC_RETURN(result);
}     

/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an IDLE NACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an IDLE NACK message (rp =1, ctrl=IDLE, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_idle_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                        UINT8 *ho_chnl_id,
                                                        UINT8 *lo_chnl_id,
                                                        UINT8 *tpid,
                                                        BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                        BOOL8 *same_rcoh,
                                                        UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;

    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;
              
    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid,  /* actually tpid */
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK :wait_event returns  first_ts : %d\n", first_ts);
    
    /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    /* update state */

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT IDLE NACK Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_IDLE;
      bwr_state = GHAO_BWR_STATE_LCR;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, TRUE, rp, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : unexpected RP");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_rp = rp;
        }
      
      result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, GHAO_LCR_CTRL_IDLE, ctrl, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : unexpected CTRL");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_ctrl = ctrl;
        }
      
      result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, FALSE, tsgs, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : unexpected TSGS");
          PMC_RETURN( result ); 
      }
      else 
        {
          var_tsgs = tsgs;
        }
      

      if( lcr_state == GHAO_LCR_STATE_NORM ) {
        lcr_state = GHAO_LCR_STATE_IDLE;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : WARNING unexpected lcr_state from var");
        lcr_state = GHAO_LCR_STATE_IDLE;
      }

      if( bwr_state == GHAO_BWR_STATE_LCR ) {
        bwr_state = GHAO_BWR_STATE_LCR;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT IDLE NACK : WARNING unexpected bwr_state from var");
        bwr_state = GHAO_BWR_STATE_LCR;
      }

    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);


    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_idle_nack */  

/*******************************************************************************
*  odtu_dmx_ghao_wait_for_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information, compare with expected TSCC message
*   - update internal context
*   - compare and update TSCC value in context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*   tscc                - expected value of tscc when discovery mode is FALSE
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   tscc                - tscc extracted when discovery mode is true
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH does not contain expected TSCC  message
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_tscc(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT8 *ho_chnl_id,
                                             UINT8 *lo_chnl_id,
                                             UINT8 *tpid,
                                             BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                             BOOL8 *tscc)
{
  
  PMC_ENTRY();
  
  PMC_ERROR  result = PMC_SUCCESS;
  BOOL8 same_rcoh;
  
  result = odtu_dmx_multi_ghao_wait_for_tscc(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, tscc, &same_rcoh, 0);
  
  if( same_rcoh == FALSE ) {
    PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_TSCC : different RCOH information found on multiple TS");
    result = PMC_ERR_FAIL;
  };

  PMC_RETURN(result);
}     


/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information, compare with expected TSCC message
*   - update internal context
*   - compare and update TSCC value in context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*   tscc                - expected value of tscc when discovery mode is FALSE
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   tscc                - tscc extracted when discovery mode is true
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH does not contain expected TSCC  message
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_tscc(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   UINT8 *tpid,
                                                   BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                   BOOL8 *tscc,
                                                   BOOL8 *same_rcoh,
                                                   UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 extract_tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;

    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;
              
    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid,
                                            &tsgs,
                                            &extract_tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
    extract_tscc = *tscc;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT TSCC : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC :wait_event returns  tscc : %d\n", extract_tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT TSCC :wait_event returns  first_ts : %d\n", first_ts);
   
    
    /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    /* update state */

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT TSCC Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;
      *tscc = extract_tscc;

      lcr_state = GHAO_LCR_STATE_IDLE;
      if( extract_tscc == 1 ) {
        bwr_state = GHAO_BWR_STATE_RAMP;
      } else {
        bwr_state = GHAO_BWR_STATE_LCR;
      }
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = extract_tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      result = odtu_dmx_ghao_chk_tscc(odtu_dmx_handle, *tscc, extract_tscc, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT TSCC : unexpected TSCC");
      } else {
        var_tscc = extract_tscc;
      }

      if( lcr_state == GHAO_LCR_STATE_IDLE ) {
        lcr_state = GHAO_LCR_STATE_IDLE;
      } else if (lcr_state == GHAO_LCR_STATE_REMOVE_NACK) {
        lcr_state = GHAO_LCR_STATE_REMOVE_NACK;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT TSCC : WARNING unexpected lcr_state from var");
        lcr_state = GHAO_LCR_STATE_IDLE;
      }

      if( *tscc != extract_tscc ) {
        PMC_LOG_TRACE("ODTU_DMX WAIT WAIT TSCC : ERROR expected tscc %d, but extracted %d\n", *tscc, extract_tscc);
        result = PMC_ERR_FAIL;
      }

      if( *tscc == 1 ) {
        if( bwr_state == GHAO_BWR_STATE_LCR ) {
          bwr_state = GHAO_BWR_STATE_RAMP;
        } else {
          PMC_LOG_TRACE("ODTU_DMX WAIT TSCC : WARNING unexpected bwr_state from var");
          bwr_state = GHAO_BWR_STATE_RAMP;
        }
      } else {
        if( bwr_state == GHAO_BWR_STATE_RAMP ) {
          bwr_state = GHAO_BWR_STATE_LCR;
        } else {
          PMC_LOG_TRACE("ODTU_DMX WAIT TSCC : WARNING unexpected bwr_state from var");
          bwr_state = GHAO_BWR_STATE_LCR;
        }
      }

    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);

    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_tscc */  

/*******************************************************************************
*  odtu_dmx_ghao_wait_for_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare end of RP message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an end of RP message (rp =0)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_rp(odtu_dmx_handle_t *odtu_dmx_handle,
                                           UINT8 *ho_chnl_id,
                                           UINT8 *lo_chnl_id,
                                           UINT8 *tpid,
                                           BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
  
  PMC_ENTRY();
  
  PMC_ERROR  result = PMC_SUCCESS;
  BOOL8 same_rcoh;
  
  result = odtu_dmx_multi_ghao_wait_for_rp(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);
  
  /*no need to check same_rcoh.  On transition from 1->0, CND will be transmitted
  in the RCOH of the last TS, so this check will fail. */

  PMC_RETURN(result);
}   


/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare end of RP message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR if
*       * extracted RCOH is not an end of RP message (rp =0)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_rp(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 *ho_chnl_id,
                                                 UINT8 *lo_chnl_id,
                                                 UINT8 *tpid,
                                                 BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                 BOOL8 *same_rcoh,
                                                 UINT8 ghao_index)
{

    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;
              
    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;

    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid,
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT RP : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT RP : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT RP :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT RP :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT RP :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT RP :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT RP :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT RP :wait_event returns  first_ts : %d\n", first_ts);

    /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    /* update state */

    /* update state */

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT RP Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_IDLE;
      bwr_state = GHAO_BWR_STATE_IDLE;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, FALSE, rp, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT RP : unexpected RP\n");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_rp = rp;
        }

      if( lcr_state == GHAO_LCR_STATE_IDLE ) {
        lcr_state = GHAO_LCR_STATE_IDLE;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT RP : WARNING unexpected lcr_state from var");
        lcr_state = GHAO_LCR_STATE_IDLE;
      }
      
      if( bwr_state == GHAO_BWR_STATE_LCR ) {
        bwr_state = GHAO_BWR_STATE_IDLE;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT RP : WARNING unexpected bwr_state from var");
        bwr_state = GHAO_BWR_STATE_IDLE;
      }

    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);
    
    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_rp */  


/*******************************************************************************
*  odtu_dmx_ghao_wait_for_remove_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an REMOVE NACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an REMOVE NACK message (rp =1, ctrl=REMOVE, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_remove_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                    UINT8 *ho_chnl_id,
                                                    UINT8 *lo_chnl_id,
                                                    UINT8 *tpid,
                                                    BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
  
  PMC_ENTRY();
  
  PMC_ERROR  result = PMC_SUCCESS;
  BOOL8 same_rcoh;
  
  result = odtu_dmx_multi_ghao_wait_for_remove_nack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);
  
  if( same_rcoh == FALSE ) {
    PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_ADD_NACK : different RCOH information found on multiple TS");
    result = PMC_ERR_FAIL;
  };

  PMC_RETURN(result);
}   


/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_remove_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an REMOVE NACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an REMOVE NACK message (rp =1, ctrl=REMOVE, tsgs=NACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_remove_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                          UINT8 *ho_chnl_id,
                                                          UINT8 *lo_chnl_id,
                                                          UINT8 *tpid,
                                                          BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                          BOOL8 *same_rcoh,
                                                          UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;

    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;
              
    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid,
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK :wait_event returns  first_ts : %d\n", first_ts);

   /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);
    
    /* update state */
    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT REMOVE NACK Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_REMOVE_NACK;
      bwr_state = GHAO_BWR_STATE_LCR;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, TRUE, rp, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : unexpected RP");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_rp = rp;
        }

      result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, GHAO_LCR_CTRL_REMOVE, ctrl, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : unexpected CTRL");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_ctrl = ctrl;
        }

      result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, FALSE, tsgs, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : unexpected TSGS");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_tsgs = tsgs;
        }

      if( lcr_state == GHAO_LCR_STATE_IDLE ) {
        lcr_state = GHAO_LCR_STATE_REMOVE_NACK;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : WARNING unexpected lcr_state from var");
        lcr_state = GHAO_LCR_STATE_REMOVE_NACK;
      }

      if( bwr_state == GHAO_BWR_STATE_LCR ) {
        bwr_state = GHAO_BWR_STATE_LCR;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : WARNING unexpected bwr_state from var");
        bwr_state = GHAO_BWR_STATE_LCR;
      }

      if( *ho_chnl_id != extract_ho_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : ERROR expected ho_chnl_id %d, but extracted %d\n", *ho_chnl_id, extract_ho_chnl_id);
        result = PMC_ERR_FAIL;
      }

      if( *lo_chnl_id != extract_lo_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : ERROR expected lo_chnl_id %d, but extracted %d\n", *lo_chnl_id, extract_lo_chnl_id);
        result = PMC_ERR_FAIL;
      } else {
        var_chnl_id = extract_lo_chnl_id;
      }


      if( *tpid != extract_tpid ) {
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE NACK : WARNING expected tpid %d, but extracted %d\n", *tpid, extract_tpid);
      } else {
        var_tpid = extract_tpid;
      }

    
      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }
    }
    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);
    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_remove_nack */  

/*******************************************************************************
*  odtu_dmx_ghao_wait_for_remove_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an REMOVE ACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an REMOVE ACK message (rp =1, ctrl=REMOVE, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_remove_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   UINT8 *tpid,
                                                   BOOL8 trib_slot[ODTU_DMX_NUM_CHANL])
{
  
  PMC_ENTRY();
  
  PMC_ERROR  result = PMC_SUCCESS;
  BOOL8 same_rcoh;
  
  result = odtu_dmx_multi_ghao_wait_for_remove_ack(odtu_dmx_handle, ho_chnl_id, lo_chnl_id, tpid, trib_slot, &same_rcoh, 0);
  
  if( same_rcoh == FALSE ) {
    PMC_LOG_TRACE("ODTU_DMX_GHAO_WAIT_FOR_ADD_NACK : different RCOH information found on multiple TS");
    result = PMC_ERR_FAIL;
  };

  PMC_RETURN(result);
} 


/*******************************************************************************
*  odtu_dmx_multi_ghao_wait_for_remove_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   - calls odtu_dmx_multi_ghao_wait_event to fetch RCOH information
*   - extract RCOH information and compare with an REMOVE ACK message
*   - update internal context
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ho_chnl_id          - expected HO channel id when discovery mode is false
*   lo_chnl_id          - expected LO channel id when discovery mode is false
*   tpid                - expected TPID when discovery mode is false
*
* OUTPUTS:
*   ho_chnl_id          - HO channel id associated with first TS of RCOH extracted when discovery mode is true
*   lo_chnl_id          - LO channel id associated with first TS of RCOH extracted when discovery mode is true
*   tpid                - TPID information extracted from RCOH when discovery mode is true
*   trib_slot           - list of tributary slot which received RCOH
*                         this list provides the position of TS within the HO container
*                         based on HO cal slot vs LO cal slot 
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - G.HAO context variable
*
* RETURNS:
*   PMC_SUCCESS 
*   PMC_ERROR if
*       * extracted RCOH is not an REMOVE ACK message (rp =1, ctrl=REMOVE, tsgs=ACK)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_remove_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                         UINT8 *ho_chnl_id,
                                                         UINT8 *lo_chnl_id,
                                                         UINT8 *tpid,
                                                         BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                         BOOL8 *same_rcoh,
                                                         UINT8 ghao_index)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
                                             
    BOOL8 rp;
    ghao_lcr_ctrl_t ctrl;
    BOOL8 tsgs;
    BOOL8 tscc;
    UINT8 first_ts;
    UINT8 extract_ho_chnl_id;
    UINT8 extract_lo_chnl_id;
    UINT8 extract_tpid;
              
    ghao_lcr_state_t lcr_state;
    ghao_bwr_state_t bwr_state;
    BOOL8 discovery;
    UINT8 var_chnl_id;
    UINT8 var_tpid;
    BOOL8 var_rp;
    BOOL8 var_tscc;
    BOOL8 var_tsgs;
    BOOL8 var_same_rcoh;
    ghao_lcr_ctrl_t var_ctrl;
    BOOL8 var_trib_slot[ODTU_DMX_NUM_CHANL];
    UINT8 i;

    result = odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle,
                                            &extract_ho_chnl_id,
                                            &extract_lo_chnl_id,
                                            trib_slot,
                                            &rp,
                                            &ctrl,
                                            &extract_tpid,
                                            &tsgs,
                                            &tscc,
                                            &first_ts,
                                            same_rcoh,
                                            ghao_index); 

#ifdef PMC_SW_SIMULATION
    extract_ho_chnl_id = *ho_chnl_id;
    extract_lo_chnl_id = *lo_chnl_id;
    extract_tpid = *tpid;
#endif

    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : wait_event returns lo_chnl_id : %d\n", extract_lo_chnl_id);
    for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
      PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : wait_event returns trib_slot[ %d ] : %d\n", i, trib_slot[i]);
    }
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK :wait_event returns  rp : %d\n", rp);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK :wait_event returns  ctrl : %d\n", ctrl);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK :wait_event returns  tpid : %d\n", extract_tpid);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK :wait_event returns  tsgs : %d\n", tsgs);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK :wait_event returns  tscc : %d\n", tscc);
    PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK :wait_event returns  first_ts : %d\n", first_ts);

   /* UPDATE CONTEXT MEMORY */
    odtu_dmx_multi_ghao_get_state(odtu_dmx_handle, 
                                  &lcr_state,
                                  &bwr_state,
                                  &discovery,
                                  &var_chnl_id,
                                  &var_tpid,
                                  var_trib_slot,
                                  &var_rp,
                                  &var_tscc,
                                  &var_tsgs,
                                  &var_ctrl,
                                  &var_same_rcoh,
                                  ghao_index);

    /* update state */

    if( discovery ) {

      if( extract_lo_chnl_id != 0x7F ) {
        *lo_chnl_id = extract_lo_chnl_id;
      } else {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("WARNING : ODTU_DMX WAIT REMOVE ACK Can't find lo_chnl_id in var context \n");
      }

      *ho_chnl_id = extract_ho_chnl_id;
      *tpid = extract_tpid;

      lcr_state = GHAO_LCR_STATE_REMOVE_ACK;
      bwr_state = GHAO_BWR_STATE_LCR;
      var_chnl_id = extract_lo_chnl_id;
      var_tpid = extract_tpid;   
      var_rp = rp;
      var_ctrl = ctrl;
      var_tsgs = tsgs;
      var_tscc = tscc;
      var_same_rcoh = *same_rcoh;

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }

    } else {

      result = odtu_dmx_ghao_chk_rp(odtu_dmx_handle, TRUE, rp, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : unexpected RP");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_rp = rp;
        }

      result = odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle, GHAO_LCR_CTRL_REMOVE, ctrl, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : unexpected CTRL");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_ctrl = ctrl;
        }

      result = odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle, TRUE, tsgs, first_ts);
      if( result != PMC_SUCCESS ) { 
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : unexpected TSGS");
        PMC_RETURN( result ); 
      }
      else 
        {
          var_tsgs = tsgs;
        }

      if( lcr_state == GHAO_LCR_STATE_REMOVE_NACK ) {
        lcr_state = GHAO_LCR_STATE_REMOVE_ACK;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : WARNING unexpected lcr_state from var");
        lcr_state = GHAO_LCR_STATE_REMOVE_ACK;
      }

      if( bwr_state == GHAO_BWR_STATE_LCR ) {
        bwr_state = GHAO_BWR_STATE_LCR;
      } else {
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : WARNING unexpected bwr_state from var");
        bwr_state = GHAO_BWR_STATE_LCR;
      }

      if( *ho_chnl_id != extract_ho_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : ERROR expected ho_chnl_id %d, but extracted %d\n", *ho_chnl_id, extract_ho_chnl_id);
        result = PMC_ERR_FAIL;
      }

      if( *lo_chnl_id != extract_lo_chnl_id ) {
        odtu_dmx_dump_var_context( odtu_dmx_handle, *ho_chnl_id, *lo_chnl_id, TRUE );
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : ERROR expected lo_chnl_id %d, but extracted %d\n", *lo_chnl_id, extract_lo_chnl_id);
        result = PMC_ERR_FAIL;
      } else {
        var_chnl_id = extract_lo_chnl_id;
      }

      if( *tpid != extract_tpid ) {
        PMC_LOG_TRACE("ODTU_DMX WAIT REMOVE ACK : WARNING expected tpid %d, but extracted %d\n", *tpid, extract_tpid);
      } else {
        var_tpid = extract_tpid;
      }

      for(i = 0; i < ODTU_DMX_NUM_CHANL; i++) {    
        var_trib_slot[i] = trib_slot[i];
      }
    }

    odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                     lcr_state,
                                     bwr_state,
                                     discovery,
                                     var_chnl_id,
                                     var_tpid,
                                     var_trib_slot,
                                     var_rp,
                                     var_tscc,
                                     var_tsgs,
                                     var_ctrl,
                                     var_same_rcoh,
                                     ghao_index);

    PMC_RETURN(result);
} /* odtu_dmx_ghao_wait_for_remove_ack */  


/*******************************************************************************
*  odtu_dmx_transfer_rcoh_status
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Transfer captured RCOH to the RCOH Status Register (0x3400 + 0x4*N)
*   OHFS_REMOVE will reset TRNSFR_RCOH_TO_SHADOW_EN once transfer is done
*   OHFS_REMOVE will raise RCOH_TRNSFR_SHDW_DNE_INTR interrupt
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if transfer is complete
*   PMC_ERROR if timeout
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_transfer_rcoh_status(odtu_dmx_handle_t *odtu_dmx_handle)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 failed_attempt;
    UINT32 reg_read;

    /* Initiate transfer */
    ohfs_remove_trnsfr_rcoh_to_shadow_en(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle);
    
    /* force next poll to not use the firmware Q */
    l5sys_firmware_queue_disable(odtu_dmx_handle);

    /* wait for RCOH_TRNSFR_SHDW_DNE interrupt */
    odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll(NULL,
                                          odtu_dmx_handle,
                                          (1<<20),           /* mask */
                                          (1<<20),           /* value */
                                          PMC_POLL_EQUAL,
                                          100,               /* max attempts */
                                          &failed_attempt,   /* number of attempts*/
                                          1);                /* time in us between poll */
    l5sys_firmware_queue_enable(odtu_dmx_handle);

    /* Clear interrupt */
    reg_read = odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_get( NULL, odtu_dmx_handle );
    odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear( NULL, odtu_dmx_handle, 1 );

    PMC_RETURN(result);
} /* odtu_dmx_transfer_rcoh_status */  




/*******************************************************************************
*  odtu_dmx_chk_summary_ghao_interrupt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Detect ghao related events in interrupt summary
*   - read bit 0 Config update done Interrupt on TS (0 to 31)
*   - read bit 1 Config update done Interrupt on TS (32 to 63)
*   - read bit 2 Config update done Interrupt on TS (64 to 95)
*   - read bit 3 HAO Status Available Interrupt on TS (0 to 31)
*   - read bit 4 HAO Status Available Interrupt on TS (32 to 63)
*   - read bit 5 HAO Status Available Interrupt on TS (64 to 95)
*   - read bit 20 RCOH Status Transfer Done Interrupt Register 
*  
* INPUTS:
*   odtu_dmx_handle        - handle to the ODTU_DMX instance to be operated on
*
* OUTPUTS:
*   cfg_dne_0_31           - config update done on channel 0 to 31 detected
*   cfg_dne_32_63          - config update done on channel 32 to 63 detected
*   cfg_dne_64_95          - config update done on channel 64 to 95 detected
*   hao_st_avl_0_31        - HAO status available Interrupt on TS 0 to 31 detected
*   hao_st_avl_32_63       - HAO status available Interrupt on TS 32 to 63 detected
*   hao_st_avl_64_95       - HAO status available Interrupt on TS 64 to 95 detected
*   rcoh_st_trsnfr_dne     - RCOH status transfer done Interrupt detected
*
* RETURNS:
*   - PMC_SUCCESS
*   - PMC_ERROR 
*
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_chk_summary_ghao_interrupt(odtu_dmx_handle_t *odtu_dmx_handle,
                                                     BOOL8 *cfg_dne_0_31,
                                                     BOOL8 *cfg_dne_32_63,
                                                     BOOL8 *cfg_dne_64_95,
                                                     BOOL8 *hao_st_avl_0_31,
                                                     BOOL8 *hao_st_avl_32_63,
                                                     BOOL8 *hao_st_avl_64_95,
                                                     BOOL8 *rcoh_st_trsnfr_dne)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 summary_1;
    UINT32 failed_attempt;


    /* wait for interrupt or poll for interrupt summary based on isr context */ 
    if( odtu_dmx_handle->var.isr_mode ) 
    {
      /* Interrupt service ON, assume interrupt pin triggered by ghao events*/

    } else {

      /* force next poll to not use the firmware Q */
      l5sys_firmware_queue_disable(odtu_dmx_handle);

      /* wait for GHAO interrupt */
      odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll(NULL,
                                            odtu_dmx_handle,
                                            0x0010003F,           /* mask */
                                            0,           /* value */
                                            PMC_POLL_NOT_EQUAL,
                                            1000,               /* max attempts */
                                            &failed_attempt,   /* number of attempts*/
                                            1000);                /* time in us between poll */

      l5sys_firmware_queue_enable(odtu_dmx_handle);

    }

    /* Wait 125us until the multiframe is received */
    PMC_Q_USLEEP(odtu_dmx_handle,125);

    summary_1 = odtu_dmx_reg_INTERRUPT_SUMMARY_1_read(NULL, odtu_dmx_handle);

    if( summary_1 & (1<<0) ) {
      *cfg_dne_0_31 = TRUE;
    } else {
      *cfg_dne_0_31 = FALSE;
    }

    if( summary_1 & (1<<1) ) {
      *cfg_dne_32_63 = TRUE;
    } else {
      *cfg_dne_32_63 = FALSE;
    }

    if( summary_1 & (1<<2) ) {
      *cfg_dne_64_95 = TRUE;
    } else {
      *cfg_dne_64_95 = FALSE;
    }
    
    if( summary_1 & (1<<3) ) {
      *hao_st_avl_0_31 = TRUE;
    } else {
      *hao_st_avl_0_31 = FALSE;
    }

    if( summary_1 & (1<<4) ) {
      *hao_st_avl_32_63 = TRUE;
    } else {
      *hao_st_avl_32_63 = FALSE;
    }

    if( summary_1 & (1<<5) ) {
      *hao_st_avl_64_95 = TRUE;
    } else {
      *hao_st_avl_64_95 = FALSE;
    }

    if( summary_1 & (1<<20) ) {
      *rcoh_st_trsnfr_dne = TRUE;
    } else {
      *rcoh_st_trsnfr_dne = FALSE;
    }

    if( *cfg_dne_0_31 || *cfg_dne_32_63 || *cfg_dne_64_95 || *hao_st_avl_0_31 || *hao_st_avl_32_63 || *hao_st_avl_64_95 ||*rcoh_st_trsnfr_dne ) {
      result = PMC_SUCCESS;
    } else {
      result = PMC_ERR_FAIL;
    }
             
    
    PMC_RETURN(result);
} /* odtu_dmx_chk_summary_ghao_interrupt */  


/*******************************************************************************
*  odtu_dmx_get_hao_ts
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Read hao status interrupt registers
*   use inputs to minimize register reads
*   construct list of CS on which HAO status is available from register read
*   Look through the sts memory context which HO chan ID is associated with first CS in list
*   Look through the sts memory context which LO chan ID is associated with first CS in list
*   construct list of TS on which HAO status is available from sts memory context
*  
* INPUTS:
*   odtu_dmx_handle        - handle to the ODTU_DMX instance to be operated on
*   hao_st_avl_0_31        - HAO status available Interrupt on TS 0 to 31 detected
*   hao_st_avl_32_63       - HAO status available Interrupt on TS 32 to 63 detected
*   hao_st_avl_64_95       - HAO status available Interrupt on TS 64 to 95 detected
*
* OUTPUTS:
*   ho_chnl_id             - HO channel id on which new HAO status is available
*   l_hao_cs               - list of CS on which new HAO status is available
*   l_hao_ts               - list of TS on which new HAO status is available
*
* RETURNS:
*   - PMC_SUCCESS if lcr_ctrl from list of ts are all ADD and TSGS are NACK
*   - PMC_ERROR if
*       * contradicting TPID
*       * contradicting lcr_ctrl
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_get_hao_ts(odtu_dmx_handle_t *odtu_dmx_handle,
                                     BOOL8 hao_st_avl_0_31,
                                     BOOL8 hao_st_avl_32_63,
                                     BOOL8 hao_st_avl_64_95,
                                     UINT8 *ho_chnl_id,
                                     BOOL8 l_hao_cs[ODTU_DMX_NUM_CHANL],
                                     BOOL8 l_hao_ts[ODTU_DMX_NUM_CHANL])
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;
     
    UINT32 available_hao_status;
    UINT8 i, j;

    UINT8 cal_entry_idx = 0;
    UINT8 first_cal_slot = 0;
             
    if( hao_st_avl_0_31 == TRUE ) {
      available_hao_status = odtu_dmx_reg_HAO_STAT_INT_array_read( NULL, odtu_dmx_handle, 0);
      odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set(NULL, odtu_dmx_handle, 0, 0xFFFFFFFF, 0,  available_hao_status);
      for (i = 0; i < 32; i++) {
        if( available_hao_status & (1<<i)) {
          l_hao_cs[i] = TRUE;
        } else {
          l_hao_cs[i] = FALSE;
        }
      }
    } else {
      for (i = 0; i < 32; i++) {
        l_hao_cs[i] = FALSE;
      }
    }

    if( hao_st_avl_32_63 == TRUE ) {
      available_hao_status = odtu_dmx_reg_HAO_STAT_INT_array_read( NULL, odtu_dmx_handle, 1);
      odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set(NULL, odtu_dmx_handle, 1, 0xFFFFFFFF, 0,  available_hao_status);
      for (i = 0; i < 32; i++) {
        if( available_hao_status & (1<<i)) {
          l_hao_cs[i+32] = TRUE;
        } else {
          l_hao_cs[i+32] = FALSE;
        }
      }

    } else {
      for (i = 0; i < 32; i++) {
        l_hao_cs[i+32] = FALSE;
      }
    }

    if( hao_st_avl_64_95 == TRUE ) {
      available_hao_status = odtu_dmx_reg_HAO_STAT_INT_array_read( NULL, odtu_dmx_handle, 2);
      odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set(NULL, odtu_dmx_handle, 2, 0xFFFFFFFF, 0,  available_hao_status);
      for (i = 0; i < 32; i++) {
        if( available_hao_status & (1<<i)) {
          l_hao_cs[i+64] = TRUE;
        } else {
          l_hao_cs[i+64] = FALSE;
        }
      }
    } else {
      for (i = 0; i < 32; i++) {
        l_hao_cs[i+64] = FALSE;
      }
    }

    /* Search for which channel the first cs is associated with*/
    for (i = 0; i < 96; i++) {
      if(l_hao_cs[i] == TRUE ) {
        first_cal_slot = i;
        break;
      }
    }

    for (i = 0; i < 96; i++) {
      for( j = 0; j < 96; j++) {
          if( odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[i].cal_entry[j] == first_cal_slot ) {
            *ho_chnl_id = i;
            break;
          }
        }
    }
    
    /* Initialize to all false */     
    for (i = 0; i < 96; i++) {
      l_hao_ts[i] = FALSE;
    }

    for (i = 0; i < 96; i++) {
      /*  RCOH detected on that calendar slot, and retain calendar index (i) */
      if( l_hao_cs[i] == TRUE ) {

        /* Assign TS for matching HO calendar slot */
        for (cal_entry_idx = 0; cal_entry_idx < 96; cal_entry_idx++) {
          /* Found matching calendar entry with detected RCOH with cal_entry_idx */
          if( odtu_dmx_handle->sts_sw_handle->var.ho_chnl_cfg[*ho_chnl_id].cal_entry[cal_entry_idx] == i ) {
            l_hao_ts[cal_entry_idx] = TRUE;
          }
        }
      }
    }
    

    PMC_LOG_TRACE("get_hao_ts : HO CHAN ID : %d\n", *ho_chnl_id);
    for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {
      PMC_LOG_TRACE("get_hao_ts : l_hao_cs[%d] = %d\n", i, l_hao_cs[i]);
    }
    for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {
      PMC_LOG_TRACE("get_hao_ts : l_hao_ts[%d] = %d\n", i, l_hao_ts[i]);
    }

    PMC_RETURN(result);
} /* odtu_dmx_get_hao_ts */  


/*******************************************************************************
*  odtu_dmx_get_rcoh
* ______________________________________________________________________________
*
* DESCRIPTION:
*      Read RCOH status and determine which action has been detected
*  
* INPUTS:
*   odtu_dmx_handle        - handle to the ODTU_DMX instance to be operated on
*   hao_ts                 - calendar slot
*
* OUTPUTS:
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
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_get_rcoh(odtu_dmx_handle_t *odtu_dmx_handle,
                                   UINT8 hao_ts,
                                   BOOL8 *rp,
                                   ghao_lcr_ctrl_t *ctrl,
                                   UINT8 *tpid,
                                   BOOL8 *tsgs,
                                   BOOL8 *tscc)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 rcoh_status;
    UINT32 tmp;

    result = ohfs_remove_rcoh_get(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                  hao_ts,
                                  &rcoh_status);
              
    if (rcoh_status & (1<<15)) 
    {                               
      *rp = TRUE;
    } else {
      *rp = FALSE;
    };

    if (rcoh_status & (1<<7)) 
    {  
      *tscc = TRUE;
    } else {
      *tscc = FALSE;
    };

    if (rcoh_status & (1<<4)) 
    {  
      *tsgs = TRUE;
    } else {
      *tsgs = FALSE;
    };

    tmp = (rcoh_status & 0x00001F00);
    *tpid = (UINT8) (tmp >> 6);
    *tpid = *tpid | ((UINT8) (rcoh_status & 0x00000003));

    switch ((rcoh_status & 0x0000000C)>>2 ) 
    {
    case 0 :
      *ctrl = GHAO_LCR_CTRL_IDLE;
      break;
    case 1 :
      *ctrl = GHAO_LCR_CTRL_ADD;
      break;
    case 2 :
      *ctrl = GHAO_LCR_CTRL_REMOVE;
      break;
    case 3 :
      *ctrl = GHAO_LCR_CTRL_NORM;
      break;
    default :
      *ctrl = GHAO_LCR_CTRL_IDLE;
      break;      
    }
    
    PMC_RETURN(result);
} /* odtu_dmx_get_rcoh */  


/*******************************************************************************
*  odtu_dmx_ghao_get_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   get the demux ghao context variable
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*
* OUTPUTS:
*   lcr_state           - link control resize state
*   bwr_state           - bandwidth resize state
*   discovery_mode      - when true don't check value
*   channel_id          - ghao channel id 
*   tpid                - ghao tributary port id
*   ts                  - list of tributary slot affected by ghao protocol
*   rp                  - resize protocol
*   tscc                - tributary slot connectivity check
*   tsgs                - tributary slot group status
*   ctrl                - control field
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_ghao_get_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                         ghao_lcr_state_t *lcr_state,
                                         ghao_bwr_state_t *bwr_state,
                                         BOOL8 *discovery_mode,
                                         UINT8 *channel_id,
                                         UINT8 *tpid,
                                         BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                         BOOL8 *rp,
                                         BOOL8 *tscc,
                                         BOOL8 *tsgs,
                                         ghao_lcr_ctrl_t *ctrl,
                                         BOOL8 *same_rcoh)
{
  
  PMC_ENTRY();
   
  odtu_dmx_multi_ghao_get_state(odtu_dmx_handle,
                                         lcr_state, 
                                         bwr_state, 
                                         discovery_mode, 
                                         channel_id, 
                                         tpid, 
                                         ts, 
                                         rp,
                                         tscc,
                                         tsgs,
                                         ctrl,
                                         same_rcoh,
                                         0);
  
  PMC_RETURN();
} /* odtu_dmx_ghao_get_state */  

/*******************************************************************************
*  odtu_dmx_multi_ghao_get_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   get the demux ghao context variable
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   ghao_index          - Index of the G.HAO context array
*
* OUTPUTS:
*   lcr_state           - link control resize state
*   bwr_state           - bandwidth resize state
*   discovery_mode      - when true don't check value
*   channel_id          - ghao channel id 
*   tpid                - ghao tributary port id
*   ts                  - list of tributary slot affected by ghao protocol
*   rp                  - resize protocol
*   tscc                - tributary slot connectivity check
*   tsgs                - tributary slot group status
*   ctrl                - control field
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_multi_ghao_get_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                               ghao_lcr_state_t *lcr_state,
                                               ghao_bwr_state_t *bwr_state,
                                               BOOL8 *discovery_mode,
                                               UINT8 *channel_id,
                                               UINT8 *tpid,
                                               BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                               BOOL8 *rp,
                                               BOOL8 *tscc,
                                               BOOL8 *tsgs,
                                               ghao_lcr_ctrl_t *ctrl,
                                               BOOL8 *same_rcoh,
                                               UINT8 ghao_index)
{
    PMC_ENTRY();

    *lcr_state = odtu_dmx_handle->ghao_dmx_state[ghao_index].lcr_state;
    *bwr_state = odtu_dmx_handle->ghao_dmx_state[ghao_index].bwr_state;
    *discovery_mode = odtu_dmx_handle->ghao_dmx_state[ghao_index].discovery_mode;
    *channel_id = odtu_dmx_handle->ghao_dmx_state[ghao_index].channel_id;
    *tpid = odtu_dmx_handle->ghao_dmx_state[ghao_index].tpid;
    *rp = odtu_dmx_handle->ghao_dmx_state[ghao_index].rp;
    *tscc = odtu_dmx_handle->ghao_dmx_state[ghao_index].tscc;
    *tsgs = odtu_dmx_handle->ghao_dmx_state[ghao_index].tsgs;
    *ctrl = odtu_dmx_handle->ghao_dmx_state[ghao_index].ctrl;
    *same_rcoh = odtu_dmx_handle->ghao_dmx_state[ghao_index].same_rcoh;
    
    PMC_RETURN();
} /* odtu_dmx_multi_ghao_get_state */  

/*******************************************************************************
*  odtu_dmx_ghao_update_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Update the demux ghao context variable
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   lcr_state           - link control resize state
*   bwr_state           - bandwidth resize state
*   discovery_mode      - when true don't check value
*   channel_id          - ghao channel id 
*   tpid                - ghao tributary port id
*   ts                  - list of tributary slot affected by ghao protocol
*   rp                  - resize protocol
*   tscc                - tributary slot connectivity check
*   tsgs                - tributary slot group status
*   ctrl                - control field
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_ghao_update_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                            ghao_lcr_state_t lcr_state,
                                            ghao_bwr_state_t bwr_state,
                                            BOOL8 discovery_mode,
                                            UINT8 channel_id,
                                            UINT8 tpid,
                                            BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                            BOOL8 rp,
                                            BOOL8 tscc,
                                            BOOL8 tsgs,
                                            ghao_lcr_ctrl_t ctrl,
                                            BOOL8 same_rcoh)

{
  
  PMC_ENTRY();
   
  odtu_dmx_multi_ghao_update_state(odtu_dmx_handle, 
                                            lcr_state, 
                                            bwr_state, 
                                            discovery_mode, 
                                            channel_id, 
                                            tpid, 
                                            ts, 
                                            rp,
                                            tscc,
                                            tsgs,
                                            ctrl,
                                            same_rcoh,
                                            0);
  
  PMC_RETURN();
} /* odtu_dmx_ghao_update_state */ 


/*******************************************************************************
*  odtu_dmx_multi_ghao_update_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Update the demux ghao context variable
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   lcr_state           - link control resize state
*   bwr_state           - bandwidth resize state
*   discovery_mode      - when true don't check value
*   channel_id          - ghao channel id 
*   tpid                - ghao tributary port id
*   ts                  - list of tributary slot affected by ghao protocol
*   rp                  - resize protocol
*   tscc                - tributary slot connectivity check
*   tsgs                - tributary slot group status
*   ctrl                - control field
*   same_rcoh           - the extracted list of tributary slot contains all the same rcoh information
*   ghao_index          - Index of the G.HAO context array
*
* OUTPUTS:
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odtu_dmx_multi_ghao_update_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  ghao_lcr_state_t lcr_state,
                                                  ghao_bwr_state_t bwr_state,
                                                  BOOL8 discovery_mode,
                                                  UINT8 channel_id,
                                                  UINT8 tpid,
                                                  BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                                  BOOL8 rp,
                                                  BOOL8 tscc,
                                                  BOOL8 tsgs,
                                                  ghao_lcr_ctrl_t ctrl,
                                                  BOOL8 same_rcoh,
                                                  UINT8 ghao_index)
{
    PMC_ENTRY();
                                            
    odtu_dmx_handle->ghao_dmx_state[ghao_index].lcr_state = lcr_state;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].bwr_state = bwr_state;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].discovery_mode = discovery_mode;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].channel_id = channel_id;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].tpid = tpid;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].rp = rp;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].tscc = tscc;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].tsgs = tsgs;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].ctrl = ctrl;
    odtu_dmx_handle->ghao_dmx_state[ghao_index].same_rcoh = same_rcoh;
    
    PMC_RETURN();
} /* odtu_dmx_multi_ghao_update_state */  



/*******************************************************************************
*  odtu_dmx_ghao_chk_tpid
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Check the value in affected tributary slot have all the same TPID
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   exp_tpid            - expected TPID value
*   new_tpid            - new TPID value read from rcoh status
*   ts                  - trib_slot on which the check is executed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if TPID are equal
*   PMC_ERROR if contradicting TPID
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_chk_tpid(odtu_dmx_handle_t *odtu_dmx_handle,
                                        UINT8 exp_tpid,
                                        UINT8 new_tpid,
                                        UINT8 ts)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_SW_SIMULATION
    new_tpid = exp_tpid;
    result = PMC_SUCCESS;
#else
    if (exp_tpid == new_tpid) 
    {
      result = PMC_SUCCESS;
    } else {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX EXP TPID: %d AND NEW TPID: %d MISMATCH for TS : %d\n", exp_tpid, new_tpid, ts );
    }
#endif

    PMC_RETURN(result);
} /* odtu_dmx_ghao_chk_tpid */  

/*******************************************************************************
*  odtu_dmx_ghao_chk_ctrl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Check the value in affected tributary slot have all the same CTRL
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   exp_ctrl            - expected value of CTRL
*   new_ctrl            - new CTRL value read from rcoh status
*   ts                  - trib_slot on which the check is executed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if CTRL are equal
*   PMC_ERROR if contradicting CTRL
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle_t *odtu_dmx_handle,
                                        ghao_lcr_ctrl_t exp_ctrl,
                                        ghao_lcr_ctrl_t new_ctrl,
                                        UINT8 ts)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_SW_SIMULATION
    new_ctrl = exp_ctrl;
    result = PMC_SUCCESS;
#else
    if (exp_ctrl == new_ctrl) 
    {
      result = PMC_SUCCESS;
    } else {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX EXP CTRL: %d AND NEW CTRL: %d MISMATCH for TS : %d\n", exp_ctrl, new_ctrl, ts );
    }
#endif

    PMC_RETURN(result);
} /* odtu_dmx_ghao_chk_ctrl */  


/*******************************************************************************
*  odtu_dmx_ghao_chk_tsgs
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Check the value TSGS
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   exp_tsgs            - expected value of TSGS
*   new_tsgs            - new TSGS value read from rcoh status
*   ts                  - trib_slot on which the check is executed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if TSGS are equal
*   PMC_ERROR if contradicting TSGS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle_t *odtu_dmx_handle,
                                        BOOL8 exp_tsgs,
                                        BOOL8 new_tsgs,
                                        UINT8 ts)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_SW_SIMULATION
    new_tsgs = exp_tsgs;
    result = PMC_SUCCESS;
#else
    if (exp_tsgs == new_tsgs) 
    {
      result = PMC_SUCCESS;
    } else {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX EXP TSGS: %d AND NEW TSGS: %d MISMATCH for TS : %d\n", exp_tsgs, new_tsgs, ts );
    }
#endif

    PMC_RETURN(result);
} /* odtu_dmx_ghao_chk_tsgs */  


/*******************************************************************************
*  odtu_dmx_ghao_chk_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Check the value of TSCC
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   exp_tscc            - expected tscc
*   new_tscc            - new tscc value read from rcoh status
*   ts                  - trib_slot on which the check is executed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if TSCC are equal
*   PMC_ERROR if contradicting TSCC
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_chk_tscc(odtu_dmx_handle_t *odtu_dmx_handle,
                                        BOOL8 exp_tscc,
                                        BOOL8 new_tscc,
                                        UINT8 ts)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_SW_SIMULATION
    new_tscc = exp_tscc;
    result = PMC_SUCCESS;
#else
    if (exp_tscc == new_tscc) 
    {
      result = PMC_SUCCESS;
    } else {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX EXP TSCC: %d AND NEW TSCC: %d MISMATCH for TS : %d\n", exp_tscc, new_tscc, ts );
    }
#endif

    PMC_RETURN(result);
} /* odtu_dmx_ghao_chk_tscc */  


/*******************************************************************************
*  odtu_dmx_ghao_chk_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*   Check the RP value in all affected TS
*
* INPUTS:
*   odtu_dmx_handle     - handle to the ODTU_DMX instance to be operated on
*   exp_rp              - expected rp value
*   new_rp              - new rp value read from RCOH status
*   ts                  - trib_slot on which the check is executed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS if rp is expected
*   PMC_ERROR if rp value is contradicting
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_chk_rp(odtu_dmx_handle_t *odtu_dmx_handle,
                                      BOOL8 exp_rp,
                                      BOOL8 new_rp,
                                      UINT8 ts)
{
    PMC_ENTRY();
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_SW_SIMULATION
    new_rp = exp_rp;
    result = PMC_SUCCESS;
#else
    if (exp_rp == new_rp) 
    {
      result = PMC_SUCCESS;
    } else {
      result = PMC_ERR_FAIL;
      PMC_LOG_TRACE("ODTU_DMX EXP RP: %d AND NEW RP: %d MISMATCH for TS : %d\n", exp_rp, new_rp, ts );
    }
#endif

    PMC_RETURN(result);
} /* odtu_dmx_ghao_chk_rp */  


/*******************************************************************************
*  odtu_dmx_ghao_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used when a LO channel needs to be removed from a HO
*   channel in ODTU_DMX.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   mux_stage           - enum for demux instace. See odtu_dmx_stage_instance_t.
*   lo_chnl_id          - LO channel ID to be configured
*                         Valid range: 0 - 95
*   ho_chnl_id          - HO channel ID that the LO channel belongs to
*                         Valid range: 0 - 95
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_ghao_lo_chnl_deprov(odtu_dmx_handle_t  *odtu_dmx_handle,
                                              odtu_dmx_stage_instance_t mux_stage,
                                              UINT32 lo_chnl_id, UINT32 ho_chnl_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 cycle = 0;
    UINT32 ts_id;
    UINT32 num_tribslot;
    UINT32 tribslot;
    /*UINT8 msi_val;*/
    /*UINT32 mgen_ipt_entry;*/
    
    /*UINT32 cal_entry;*/
    line_core_otn_db_key    key;
    line_core_otn_db_rec    new_rec;
    UINT32          num_of_recs;
    line_core_otn_db_query_res *q_result;
    line_core_otn_db_query_res temp_result;
    
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(odtu_dmx_handle!=NULL, ODTU_DMX_ERR_INVALID_PTR, 0 , 0);

    /* associate this LO to the HO channel */
    odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set(NULL, odtu_dmx_handle, ho_chnl_id);
    
    num_tribslot = odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].num_tribslot;

    /* deprov channel configurations */
    /* deprov rxjc channel */
    result = rxjc_proc_lo_chnl_deprov(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle, 
                                      lo_chnl_id);
    
    if (PMC_SUCCESS == result)
    {

        cpact_obuf_field_EOMF_LAST_DT_set(NULL, odtu_dmx_handle->rgmp_handle->cpact_obuf_handle, lo_chnl_id, 0);
        cpact_obuf_field_EOMF_CYCLE_set(NULL, odtu_dmx_handle->rgmp_handle->cpact_obuf_handle, lo_chnl_id, 0);


    }

    
    if (mux_stage != ODTU_DMX_STG_TWO)
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
    num_of_recs = gen_db_query(&(odtu_dmx_handle->base),
                               odtu_dmx_handle->cfg.db_id,
                               odtu_dmx_handle->cfg.db_handle, (void *)&key,
                               (void **)&q_result);
    temp_result = *q_result;
    if (num_of_recs == 0)
    {
        PMC_RETURN(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND);
    }

    if (PMC_SUCCESS == result)
    {
        for (ts_id = 0; ts_id < num_of_recs; ts_id++)
        {
            cycle = temp_result.p_recs[ts_id]->calendar_entry;
            /*tribslot = temp_result.p_recs[ts_id]->trib_slot;*/
            if (mux_stage == ODTU_DMX_STG_ONE)
              {
                tribslot = temp_result.p_recs[ts_id]->trib_slot;
              }
            else {
              tribslot = temp_result.p_recs[ts_id]->ts_port_mo_lo;
            }
                     
            if (PMC_SUCCESS == result)
            {
                result = sts_sw_lo_chnl_cfg(odtu_dmx_handle->sts_sw_handle, 
                                            ho_chnl_id, cycle, lo_chnl_id, 1, FALSE);  
            } 
            
            if (PMC_SUCCESS == result)
            {                                    
                result = ohfs_remove_ts_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, cycle,
                                            UTIL_GLOBAL_NO_MAP, 0, 0, 0);
            }
            
            if (PMC_SUCCESS == result)
            {
                result = ohfs_remove_dmx_chan_mapping_cfg(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle, 
                                                          cycle, 0);
            }
            
           /* MSI configuration; Set to Unallocated 
              -> No need to change expected msi in GHAO, when provisioning the new TS, it will correctly
              reassign the correct expected msi. When removing TS, the TS_valid will be set to FALSE, therefore
              expected msi will be ignored.
            */
            /*
            if (PMC_SUCCESS == result)
            {*/
              /*if (odtu_dmx_handle->var.lo_chnl_cfg[lo_chnl_id].ho_oduk_type == UTIL_GLOBAL_ODUFLEX_GFP)
                {
              */  
              /*              
              result = ohfs_remove_exp_msi_get(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               cycle, 
                                               &msi_val);
              
              if( odtu_dmx_handle->var.ho_chnl_cfg[ho_chnl_id].oduk_type == UTIL_GLOBAL_ODU4 ) {
                msi_val = msi_val & 0x7F;
              } else {
                msi_val = msi_val | 0xC0;
              }
              result = ohfs_remove_exp_msi_set(odtu_dmx_handle->rgmp_handle->ohfs_remove_handle,
                                               cycle, 
                                               msi_val);
              */    
              /*} 
            }*/
                        
            if (PMC_SUCCESS == result)
            {
                result = sts_sw_lo_calendar_cfg_deprov(odtu_dmx_handle->sts_sw_handle, cycle);
            }  
            

            /* Update DB */
            if (PMC_SUCCESS == result)
            {

                if (mux_stage == ODTU_DMX_STG_ONE)
                {
                    if ( ODTU_DMX_DB_ENTRY_UNKNOWN != (temp_result.p_recs[ts_id]->mo_channel) )
                    {
                      new_rec = *temp_result.p_recs[ts_id];
                      /*new_rec.ho_channel = ho_chnl_id;*/
                      new_rec.mo_channel = ODTU_DMX_DB_ENTRY_UNKNOWN;
                      /*new_rec.lo_channel = ODTU_DMX_DB_ENTRY_UNKNOWN;*/
                      /*new_rec.trib_slot = tribslot;*/
                      new_rec.cal_entry_status = 0;
                        
                      PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                    new_rec.ho_channel,
                                    new_rec.trib_slot,
                                    new_rec.mo_channel,
                                    new_rec.lo_channel);
                      odtu_dmx_tribslot_var_update(odtu_dmx_handle, tribslot, ho_chnl_id, 0);
                    }
                }   
                else if (mux_stage == ODTU_DMX_STG_TWO)
                {
                    if ( ODTU_DMX_DB_ENTRY_UNKNOWN != (temp_result.p_recs[ts_id]->lo_channel) )
                    {
                      new_rec = *temp_result.p_recs[ts_id];
                      new_rec.calendar_entry = temp_result.p_recs[ts_id]->calendar_entry;
                      new_rec.lo_channel = ODTU_DMX_DB_ENTRY_UNKNOWN;
                      /*new_rec.ts_port_mo_lo = tribslot;*/
                      new_rec.cal_entry_status = 0;
                        
                      PMC_LOG_TRACE("new_rec.ho_channel = %u, new_rec.trib_slot = %u, new_rec.mo_channel = %u, new_rec.lo_channel = %u\n",
                                    new_rec.ho_channel,
                                    new_rec.trib_slot,
                                    new_rec.mo_channel,
                                    new_rec.lo_channel);
                      odtu_dmx_tribslot_var_update(odtu_dmx_handle, tribslot, ho_chnl_id, 0);
                    }
                }                  
            }             
        }
    }


    
    PMC_RETURN(result);
} /* odtu_dmx_ghao_lo_chnl_deprov */


/*******************************************************************************
*  odtu_dmx_chk_cfg_dne
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used check if cfg update dne is triggered
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   ho_chnl_id          - HO channel ID that the LO channel belongs to
*                         Valid range: 0 - 12
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_chk_cfg_dne( odtu_dmx_handle_t  *odtu_dmx_handle,
                                       UINT8 ho_chnl_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS; 
    PMC_ENTRY();

  
    BOOL8 cfg_dne_0_31;
    BOOL8 cfg_dne_32_63;
    BOOL8 cfg_dne_64_95;
    BOOL8 hao_st_avl_0_31;
    BOOL8 hao_st_avl_32_63;
    BOOL8 hao_st_avl_64_95;
    BOOL8 rcoh_st_trsnfr_dne;

    UINT32 cfg_dne_int;
    UINT8 i;

    /* Check which ghao interrup has been triggered */              
    result = odtu_dmx_chk_summary_ghao_interrupt (odtu_dmx_handle,
                                                  &cfg_dne_0_31,
                                                  &cfg_dne_32_63,
                                                  &cfg_dne_64_95,
                                                  &hao_st_avl_0_31,
                                                  &hao_st_avl_32_63,
                                                  &hao_st_avl_64_95,
                                                  &rcoh_st_trsnfr_dne);
    
    if( result != PMC_SUCCESS ) { 
      PMC_LOG_TRACE("ODTU_DMX no interrupt ");
      PMC_RETURN( result ); 
    }


    if( cfg_dne_0_31 == TRUE ) {
      cfg_dne_int = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( NULL, odtu_dmx_handle, 0);
      odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set(NULL, odtu_dmx_handle, 0, 0xFFFFFFFF, 0,  cfg_dne_int);
      for (i = 0; i < 32; i++) {
        if( cfg_dne_int & (1<<i)) {
          if( i == ho_chnl_id ) {
            result = PMC_SUCCESS;
          } else {
            PMC_LOG_TRACE("ODTU_DMX mismatch from expected cfg update dne channel %d and dut detected chnl %d \n", ho_chnl_id, i);
            PMC_RETURN( result ); 
          }
        }
      }
    } 

    if( cfg_dne_32_63 == TRUE ) {
      cfg_dne_int = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( NULL, odtu_dmx_handle, 1);
      odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set(NULL, odtu_dmx_handle, 1, 0xFFFFFFFF, 0,  cfg_dne_int);
      for (i = 0; i < 32; i++) {
        if( cfg_dne_int & (1<<i)) {
          if( i == ho_chnl_id ) {
            result = PMC_SUCCESS;
          } else {
            PMC_LOG_TRACE("ODTU_DMX mismatch from expected cfg update dne channel %d and dut detected chnl %d \n", ho_chnl_id, i);
            PMC_RETURN( result ); 
          }
        }
      } 
    }

    if( cfg_dne_64_95 == TRUE ) {
      cfg_dne_int = odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read( NULL, odtu_dmx_handle, 2);
      odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set(NULL, odtu_dmx_handle, 2, 0xFFFFFFFF, 0,  cfg_dne_int);
     for (i = 0; i < 32; i++) {
        if( cfg_dne_int & (1<<i)) {
          if( i == ho_chnl_id ) {
            result = PMC_SUCCESS;
          } else {
            PMC_LOG_TRACE("ODTU_DMX mismatch from expected cfg update dne channel %d and dut detected chnl %d \n", ho_chnl_id, i);
            PMC_RETURN( result ); 
          }
        }
      }
    } 

    PMC_RETURN(result);
} /* odtu_dmx_chk_cfg_dne */



/*******************************************************************************
*  odtu_dmx_cm_min_max_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to reprovision CM_MIN and CM_MAX if a different
*   +/- ppm max collar is desired.
*
* INPUTS:
*   *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*   chnl_id             - LO channel ID to be configured
*                         Valid range: 0 - 95
*   lo_odu_type         - LO ODU rate. See util_global_odukp_type_t.
*   cbr_client_rate     - client bitrate in kbps in ODUFlex CBR client. 
*                         Set to 0 for all other lo_odu_types.
*   ho_odu_type         - HO ODU rate. See util_global_odukp_type_t.
*   mapping_mode        - mapping mode of the LO ODU channel.
*                         See util_global_mapping_mode_t.
*   num_ts              - The number of tribslots
*   use_custom_ppm      - TRUE : use ppm_offset parameter to calculate
*                                CM_MIN and CM_MAX
*                         FALSE : use ppm ofsets defined in G.709
*   ppm_offset          - If use_custom_ppm is TRUE, this is
*                         the user defined +/- ppm collar
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odtu_dmx_cm_min_max_reprov(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id,
                                            util_global_odukp_type_t lo_odu_type,
                                            UINT32 cbr_client_rate,
                                            util_global_odukp_type_t ho_odu_type,
                                            util_global_mapping_mode_t mapping_mode,
                                            UINT32 num_ts,
                                            BOOL use_custom_ppm,
                                            UINT32 ppm_offset)
{ 

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    
    if(use_custom_ppm)
    {
        odtu_dmx_handle->var.ppm_offset[chnl_id] = ppm_offset;
    }
    else
    {
        odtu_dmx_handle->var.ppm_offset[chnl_id] = 0;
    }
    
    if(result == PMC_SUCCESS)
    {
        result = rxjc_proc_gmp_param_cfg(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle,
                                         chnl_id, 
                                         ho_odu_type, (util_global_odukp_type_t)lo_odu_type,
                                         cbr_client_rate,
                                         num_ts,
                                         use_custom_ppm,
                                         odtu_dmx_handle->var.ppm_offset[chnl_id]); 
    }


    PMC_RETURN(result);

}

/*******************************************************************************
*  odtu_dmx_cm_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function retrieves the CM status value in the CM_STAT_REG register for
*  the particular odtu_dmx handle.
*
* INPUTS:
*  *odtu_dmx_handle    - pointer to ODTU_DMX handle instance
*  chnl_id             - LO channel ID to be configured
*                        Valid range: 0 - 95
*
* OUTPUTS:
*  value               - Value in the register.
*
* RETURNS:
*   None
*
*******************************************************************************/
PUBLIC void odtu_dmx_cm_status_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                   UINT32 chnl_id,
                                   UINT32 *value)
{ 
    PMC_ENTRY();

    rxjc_proc_cm_status_get(odtu_dmx_handle->rgmp_handle->rxjc_proc_handle,
                            chnl_id,
                            value);

    PMC_RETURN();
} /* odtu_dmx_cm_status_get */




/*
** End of file
*/
