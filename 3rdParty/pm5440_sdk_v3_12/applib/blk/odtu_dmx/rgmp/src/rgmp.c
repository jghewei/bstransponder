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

#include "rgmp_loc.h"


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
const char RGMP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    RGMP_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char RGMP_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/
PRIVATE PMC_ERROR rgmp_var_default_init(rgmp_handle_t *rgmp_handle);

PUBLIC void rgmp_var_update(rgmp_handle_t *rgmp_handle, 
                            UINT32 chnl,
                            util_global_chnl_state_t chnl_state);                                
                                        
PRIVATE PMC_ERROR rgmp_chnl_reset_cfg(rgmp_handle_t *rgmp_handle,
                                      UINT32 chnl,
                                      util_global_chnl_state_t chnl_state);
                                      
PRIVATE PMC_ERROR rgmp_server_num_ts_cfg(rgmp_handle_t *rgmp_handle, 
                                         util_global_odukp_type_t server_type,
                                         util_global_odukp_type_t client_type, 
                                         util_global_odu_line_payload_t ts_type,
                                         UINT32 *num_tribslot);                                      

/*
** Public Functions
*/

/*******************************************************************************
* rgmp_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a RGMP block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the RGMP subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier for the block
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
PUBLIC rgmp_handle_t *rgmp_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    rgmp_handle_t *rgmp_handle;

    PMC_ENTRY();
    
    rgmp_handle = (rgmp_handle_t*)PMC_CTXT_CALLOC(sizeof(rgmp_handle_t), parent);
    
    pmc_handle_init(parent, rgmp_handle, sys_handle, PMC_MID_DIGI_RGMP, tsb_name, base_address); 
    
    /* Register log strings. */
    pmc_log_block_strings_register(RGMP_LOG_ERR_STRINGS[0], RGMP_LOG_ERR_TABLE_BASE, RGMP_LOG_ERR_COUNT);
    
    rgmp_handle->ohfs_remove_handle = ohfs_remove_ctxt_create((pmc_handle_t *)rgmp_handle, base_address+BASE_ADDR_RGMP96_OHFS_REMOVE, sys_handle, "OHFS_REMOVE");
    PMC_CTXT_REGISTER_PTR(&rgmp_handle->ohfs_remove_handle, parent);
    rgmp_handle->rxjc_proc_handle = rxjc_proc_ctxt_create((pmc_handle_t *)rgmp_handle, base_address+BASE_ADDR_RGMP96_RXJC_PROC, sys_handle, "RXJC_PROC");
    PMC_CTXT_REGISTER_PTR(&rgmp_handle->rxjc_proc_handle, parent);
    rgmp_handle->cpact_obuf_handle = cpact_obuf_ctxt_create((pmc_handle_t *)rgmp_handle, base_address+BASE_ADDR_RGMP96_CPACT_OBUF, sys_handle, "CPACT_OBUF");
    PMC_CTXT_REGISTER_PTR(&rgmp_handle->cpact_obuf_handle, parent);
    rgmp_handle->mp_mgen_handle = mp_mgen_ctxt_create((pmc_handle_t *)rgmp_handle, base_address+BASE_ADDR_RGMP96_MP_MGEN, sys_handle, "MP_MGEN");
    PMC_CTXT_REGISTER_PTR(&rgmp_handle->mp_mgen_handle, parent);
    
    PMC_RETURN(rgmp_handle);
    
} /* rgmp_ctxt_create */

 
/*******************************************************************************
* rgmp_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a RGMP block instance.
*
*
* INPUTS:
*   *rgmp_handle         - pointer to RGMP handle instance
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
PUBLIC void rgmp_ctxt_destroy(rgmp_handle_t *rgmp_handle)
{
    PMC_ENTRY();
    
    ohfs_remove_ctxt_destroy(rgmp_handle->ohfs_remove_handle);
    rxjc_proc_ctxt_destroy(rgmp_handle->rxjc_proc_handle);
    mp_mgen_ctxt_destroy(rgmp_handle->mp_mgen_handle);
    cpact_obuf_ctxt_destroy(rgmp_handle->cpact_obuf_handle);
    
    PMC_LOG_TRACE("rgmp_handle->cfg.db_id = %u\n", rgmp_handle->cfg.db_id);
    PMC_LOG_TRACE("rgmp_handle->cfg.db_handle = %u\n", rgmp_handle->cfg.db_handle);

    if (rgmp_handle->cfg.db_handle != 0)
    {
        gen_db_detach(&(rgmp_handle->base), rgmp_handle->cfg.db_id,
                      rgmp_handle->cfg.db_handle);
    }
    
    PMC_CTXT_FREE(&rgmp_handle, rgmp_handle);
    
    PMC_RETURN();
} /* rgmp_ctxt_destroy */

/*******************************************************************************
* rgmp_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an RGMP block handle instance.
*
* INPUTS:
*   *rgmp_handle         - pointer to RGMP handle instance
*   rgmp_instance        - Enum. See rgmp_instance_type_t.
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
PUBLIC void rgmp_handle_init(rgmp_handle_t *rgmp_handle, 
                             rgmp_instance_type_t rgmp_instance)
{
    /* variable declaration */
    ohfs_remove_instance_type_t ohfs_remove_instance;
    rxjc_proc_instance_type_t rxjc_proc_instance;
    cpact_obuf_instance_type_t cpact_obuf_instance;
    
    PMC_ENTRY();
    
    /* get the ultimate parent for DB usage */
    rgmp_handle->cfg.instance = rgmp_instance;
    
    
    /* Update static configurations */
    if (rgmp_instance == INSTANCE_RGMP)
    {    
        ohfs_remove_instance = OHFS_REMOVE_INSTANCE_RGMP;
        rxjc_proc_instance = RXJC_PROC_INSTANCE_RGMP;
        cpact_obuf_instance = CPACT_OBUF_INSTANCE_RGMP;
        rgmp_handle->cfg.num_rgmp_chanl = RGMP_NUM_CHANL;
        rgmp_handle->cfg.max_fifo_num = RGMP_IPT_DEPTH;
        rgmp_handle->cfg.db_id = UTIL_GEN_DB_MPMA_DB;
        
        /* attach to the databse */
        rgmp_handle->cfg.db_handle = gen_db_attach(&(rgmp_handle->base),rgmp_handle->cfg.db_id);
    
        /* Initialize var_t parameters */
        rgmp_var_default_init(rgmp_handle);
    }
    else {
        ohfs_remove_instance = OHFS_REMOVE_INSTANCE_DEMUX;
        rxjc_proc_instance = RXJC_PROC_INSTANCE_DEMUX;
        cpact_obuf_instance = CPACT_OBUF_INSTANCE_DEMUX;
        rgmp_handle->cfg.db_id = UTIL_GEN_DB_LINE_CORE_OTN_RX_DB;
        
        rgmp_handle->cfg.db_handle = gen_db_attach(&(rgmp_handle->base), rgmp_handle->cfg.db_id);
    }
        
    /* Initialize OHFS_REMOVE */
    ohfs_remove_handle_init(rgmp_handle->ohfs_remove_handle, ohfs_remove_instance);
    /*ohfs_remove_handle_init(rgmp_handle->ohfs_remove_handle);*/
    
    /* Initialize RXJC_PROC */
    rxjc_proc_handle_init(rgmp_handle->rxjc_proc_handle, rxjc_proc_instance);
    
    /* Initialize MP_MGEN */
    mp_mgen_handle_init(rgmp_handle->mp_mgen_handle);
    
    /* Initialize CPACT_OBUF */
    cpact_obuf_handle_init(rgmp_handle->cpact_obuf_handle, cpact_obuf_instance);
     
    PMC_RETURN();
} /* rgmp_handle_init */

/*******************************************************************************
* rgmp_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on rgmp.
*
*
* INPUTS:
*   *rgmp_handle        - pointer to RGMP handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR rgmp_handle_restart_init(rgmp_handle_t *rgmp_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(RGMP_LOG_ERR_STRINGS[0], RGMP_LOG_ERR_TABLE_BASE, RGMP_LOG_ERR_COUNT);
    }

    ret_val = ohfs_remove_handle_restart_init(rgmp_handle->ohfs_remove_handle, restart_init_cfg_ptr, top_energy_state_reg);

    if (PMC_SUCCESS == ret_val)
    {
        rxjc_proc_handle_restart_init(rgmp_handle->rxjc_proc_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    if (PMC_SUCCESS == ret_val)
    {
        mp_mgen_handle_restart_init(rgmp_handle->mp_mgen_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    if (PMC_SUCCESS == ret_val)
    {
        cpact_obuf_handle_restart_init(rgmp_handle->cpact_obuf_handle, restart_init_cfg_ptr, top_energy_state_reg);
    }

    PMC_RETURN(ret_val);
} /* rgmp_handle_restart_init */


/*******************************************************************************
* rgmp_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize RGMP with parameters specific to this instantiation.
*
*
* INPUTS:
*   *rgmp_handle         - pointer to RGMP handle instance.
*   energy_state          - enum defining state to set the RGMP module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for RGMP module
*
*******************************************************************************/
PUBLIC PMC_ERROR rgmp_init(rgmp_handle_t *rgmp_handle, 
                           pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(rgmp_handle!=NULL, RGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        ret_val = ohfs_remove_init(rgmp_handle->ohfs_remove_handle, energy_state);
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        rgmp_var_default_init(rgmp_handle);
        ret_val = ohfs_remove_init(rgmp_handle->ohfs_remove_handle, energy_state);
    } else {
        PMC_ASSERT(FALSE, RGMP_ERR_INVALID_PARAMETERS, 0, 0);
    }
    
    PMC_RETURN(ret_val);
    
    
} /* rgmp_init */


/*******************************************************************************
* rgmp_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   RGMP instance.  It will test the channel state for all resources within
*   the RGMP instance if any are not in RGMP_CHNL_START state then
*   FALSE is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the RGMP and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *rgmp_handle          - pointer to RGMP handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                  - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 rgmp_start_state_test(rgmp_handle_t *rgmp_handle) 
{
    UINT32 i;
    PMC_ENTRY();
    PMC_ASSERT(rgmp_handle!=NULL, RGMP_ERR_INVALID_PARAMETERS, 0, 0);

    for (i = 0; i < rgmp_handle->cfg.num_rgmp_chanl; i++)
    {
        if(rgmp_handle->var.rgmp_chnl_cfg[i].state != RGMP_CHNL_START) 
            PMC_RETURN(FALSE);  
    }

    PMC_RETURN(TRUE);

} /* rgmp_start_state_test */


/*******************************************************************************
* rgmp_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an RGMP block instance.
*
*
* INPUTS:
*   *rgmp_handle         - pointer to RGMP handle instance.
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
PUBLIC UINT32 rgmp_base_address_get(rgmp_handle_t *rgmp_handle)
{
    PMC_ENTRY();
    PMC_RETURN(rgmp_handle->base.base_address);
    
} /* rgmp_base_address_get */

/*******************************************************************************
* odtu_dmx_rgmp_lo_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions a channel for the specified adaptaion function and 
*   ODU rate in RGMP. This function can also be used to add a LO ODU channel 
*   into an active HO channel.
*   
*   Prior to provisioning a channel, the channel must be put into reset using
*   rgmp_chnl_deactivate() API. Then for each tributary slot in the channel,
*   odtu_dmx_rgmp_chnl_tribslot_cfg() API must be called to set per tribslot 
*   configurations. After the channel configuration is complete, call
*   rgmp_chnl_activate() API to enable the channel.
*
* INPUTS:
*   *rgmp_handle        - pointer to RGMP handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   client_type         - For DEMUX, LO ODU rate. See util_global_odukp_type_t. \n
*                         For RGMP, adaptaion_function. See
*                         adaptaion_function_enum for valid types
*   server_type         - For DEMUX, HO ODU rate. See util_global_odukp_type_t.\n
*                         For RGMP, ODUkp or ODUjp type see odukp_type_t, ODUkp 
*                         all ODUk types valid, ODUjp only ODU2/3 types valid
*   mapping_mode        - mapping mode used in mapping the LO channel to HO ODU. \n
*                         For RGMP, this argument is not used since mapping mode 
*                         is set as a function of client_type inside this API.  
*   ts_type             - tributary slot type. See odu_line_payload_t. \n
*                         NOTE: Only valid for DEMUX. This argument is ignored
*                               in RGMP.
*   num_odu0            - number of ODU0's in ODUFLEX_GFP client. Set to 0 for
*                         all other client types.
*   client_rate         - client bit rate in kbps for ODUFLEX_CBR.
*   num_ts              - if client_type is set to ODUFLEX_CBR or ODUFLEX_GFP,  
*                         this argument is used to specify the total number of  
*                         tributary slots required by the channel into the HO  
*                         ODU channel. For non-FLEX client types, this argument 
*                         is ignored.
*   offline_update      - Used for DMX only. 
*                         0 -- active page configuration
*                         1 -- offline page configuration
*   use_custom_ppm          - TRUE : use ppm_offset parameter to calculate
*                                    CM_MIN and CM_MAX
*                             FALSE : use ppm ofsets defined in G.709
*   ppm_offset              - If use_custom_ppm is TRUE, this is
*                             the user defined +/- ppm collar
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
PUBLIC PMC_ERROR odtu_dmx_rgmp_lo_chnl_prov(rgmp_handle_t *rgmp_handle,
                                            UINT32 chnl, 
                                            UINT32 client_type, 
                                            util_global_odukp_type_t server_type,
                                            util_global_mapping_mode_t mapping_mode,
                                            util_global_odu_line_payload_t ts_type,
                                            UINT32 num_odu0,
                                            DOUBLE client_rate,
                                            UINT32 num_ts,
                                            BOOL8 offline_update,
                                            BOOL use_custom_ppm,
                                            UINT32 ppm_offset)             
{
    /* variable declaration */
    rgmp_chnl_state_t chnl_state;
    util_global_parent_t parent = UTIL_GLOBAL_LAST_PARENT;
   
    UINT32 num_tribslot;
    UINT32 client = 0;
    UINT32 m;
    
    PMC_ERROR ret_val;
   
    PMC_ENTRY();
     
    /* argument checking */
    if (rgmp_handle->cfg.instance == INSTANCE_RGMP) {
        /* check if (client_type > adaptation_function) */
        if (client_type > UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP)
        {
            PMC_RETURN(RGMP_ERR_INVALID_PARAMETERS);
        }
        
        /* check if the channel is put in reset */
        if (rgmp_handle->var.rgmp_chnl_cfg[chnl].state != RGMP_CHNL_START)
        {
            PMC_RETURN(RGMP_CHNL_STATE_NOT_START);
        }
    }
    else 
    {
        if (client_type > UTIL_GLOBAL_ODUK_DONT_CARE)
        {
            PMC_RETURN(RGMP_ERR_INVALID_PARAMETERS);   
        }
    }
    
    if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
    {
        num_ts = 1;
        ts_type = (util_global_odu_line_payload_t)0;
        
        /* OHFS Config */
        /* setting up the functional mode */
        ret_val = ohfs_remove_func_mode_cfg(rgmp_handle->ohfs_remove_handle, 
                                            chnl, 0);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* configure common configurations and FS stripping */
        ret_val = ohfs_remove_chnl_prov(rgmp_handle->ohfs_remove_handle, chnl, 
                                        server_type, client_type, mapping_mode, (util_global_odu_line_payload_t)0);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    else {
        if (client_type != UTIL_GLOBAL_ODUFLEX_CBR 
            && client_type != UTIL_GLOBAL_ODUFLEX_GFP 
            &&  server_type != UTIL_GLOBAL_ODUFLEX_CBR 
            && server_type != UTIL_GLOBAL_ODUFLEX_GFP)
        { 
            ret_val = rgmp_server_num_ts_cfg(rgmp_handle, server_type, (util_global_odukp_type_t)client_type, 
                                             ts_type, &num_tribslot);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }                                       
            num_ts = num_tribslot;
        } 
        else {
            num_ts = num_odu0; /* number of ODU0's */
        }  
    } 
    
    /* cpact_obuf configuration */
    ret_val = cpact_obuf_chnl_cfg(rgmp_handle->cpact_obuf_handle, chnl, server_type, client_type,
                                  mapping_mode, num_ts, ts_type, offline_update);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }
         
    /* RXJC PROC Config */
    /* AMP mode configuration */
    if(mapping_mode == UTIL_GLOBAL_AMP)
    {
        if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
        {
            client = UTIL_GLOBAL_ODUK_DONT_CARE;
        }
        else if (rgmp_handle->cfg.instance == INSTANCE_DEMUX)
        {
            client = client_type;
        }
       
        ret_val = rxjc_proc_amp_cfg(rgmp_handle->rxjc_proc_handle, chnl, 
                                    server_type, (util_global_odukp_type_t)client, ts_type);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
      
        /* MP_MGEN configuration */
        /* LAST_TXR_BYTES */
        PMC_LOG_TRACE("mp_mgen update: offline_update = %u\n", offline_update);
        
        ret_val = mp_mgen_last_txr_cfg(rgmp_handle->mp_mgen_handle, chnl, server_type, 
                                       (util_global_odukp_type_t)client_type, mapping_mode, ts_type, 
                                       UTIL_GLOBAL_ODUK_DONT_CARE, 0);
        if (ret_val != PMC_SUCCESS)
        {           
            PMC_RETURN(ret_val);
        }        
                                                                              
    } 
    /* BMP mode configuration */
    else if(mapping_mode == UTIL_GLOBAL_BMP)
    {
        ret_val = rxjc_proc_bmp_cfg(rgmp_handle->rxjc_proc_handle, chnl, server_type, ts_type);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        
        /* MP_MGEN configuration */
        /* LAST_TXR_BYTES */
        ret_val = mp_mgen_last_txr_cfg(rgmp_handle->mp_mgen_handle, chnl, server_type, 
                                       (util_global_odukp_type_t)client_type, mapping_mode, ts_type, 
                                       UTIL_GLOBAL_ODUK_DONT_CARE, 0);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }   
                                                                                    
    }
    else if (mapping_mode == UTIL_GLOBAL_GMP)
    { 
        /* GMP mode configuration */
        /* RXJC_PROC */
        ret_val = rxjc_proc_gmp_param_cfg(rgmp_handle->rxjc_proc_handle, chnl, 
                                          server_type, (util_global_odukp_type_t)client_type, client_rate, num_ts,
                                          use_custom_ppm, ppm_offset); 
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        
        if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
        {
            parent = UTIL_GLOBAL_TGMP_RGMP;
        }
        else if (rgmp_handle->cfg.instance == INSTANCE_DEMUX)
        {
            parent = UTIL_GLOBAL_MUX_DEMUX;
        }
        
        if (parent == UTIL_GLOBAL_TGMP_RGMP)
        {
            /* Get the m value */
            ret_val = util_global_max_server_num_ts_get(parent, server_type, (util_global_odu_line_payload_t)0, &m);
        }
        else {
            if (client_type != UTIL_GLOBAL_ODUFLEX_CBR && client_type != UTIL_GLOBAL_ODUFLEX_GFP)
            {
                ret_val = util_global_max_client_num_ts_get(server_type, (util_global_odukp_type_t)client_type, 
                                                            ts_type, &m);
            }
            else {
                m = num_ts; /* for ODUFLEX_GFP */
            }                                                          
        }
                                                           
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        
        /* MP_MGEN configuration */
        /* Div M 48 Last Txr */                                                     
        ret_val = mp_mgen_last_txr_cfg(rgmp_handle->mp_mgen_handle, chnl, server_type, 
                                       (util_global_odukp_type_t)client_type, 
                                       mapping_mode, ts_type, num_ts, m);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        } 
    }
    
    /* Update var context for a provisioned channel */  
    if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
    {    
        chnl_state = RGMP_CHNL_EQUIPPED;
    
        /*PMC_LOG_TRACE("1. rgmp_handle->var.odu_lo_chnl_cfg[chnl].num_ts = %u", rgmp_handle->var.rgmp_chnl_cfg[chnl].num_ts);
        num_lo_ts = (rgmp_handle->var.rgmp_chnl_cfg[chnl].num_ts) + 1;*/
        rgmp_var_update(rgmp_handle, chnl, (util_global_chnl_state_t)chnl_state);
    
        /*PMC_LOG_TRACE("2. rgmp_handle->var.odu_lo_chnl_cfg[chnl].num_ts = %u", rgmp_handle->var.rgmp_chnl_cfg[chnl].num_ts);*/
    } 
                                
    PMC_RETURN(PMC_SUCCESS);
    
} /* odtu_dmx_rgmp_lo_chnl_prov */

/*******************************************************************************
* odtu_dmx_rgmp_lo_chnl_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unprovisions the specific channel.
*
*
* INPUTS:
*   *rgmp_handle      - pointer to RGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
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
PUBLIC PMC_ERROR odtu_dmx_rgmp_lo_chnl_unprov(rgmp_handle_t *rgmp_handle, UINT32 chnl)
{
    /* variable declaration */
    mapotn_db_key        key;
    mapotn_db_query_res *q_result;
    UINT32     num_cal_entries =0;
    UINT32 i;
    PMC_ERROR ret_val;
    
    /*UINT32 ts = 0;
    UINT32 cnt = 0;
    UINT32 tot_ts = 0;*/

    
    PMC_ENTRY();
    
    /* 1. OHFS_REMOVE:  Set IPT_TS_VALID to 0 
    ** 2. RXJC_PROC:    Set CHNL_RST to 0 
    ** 3. MP_MGEN:      Set CHNL_RST to 0 
    ** 4. CPACT_OBUF:   Set CHNL_RST to 0 */
    
    /*tot_ts = rgmp_handle->var.rgmp_chnl_cfg[chnl].num_ts;
    PMC_LOG_TRACE("1. tot_ts = %u\n", tot_ts);*/
    
    /* check the channel state */
    if (rgmp_handle->var.rgmp_chnl_cfg[chnl].state != RGMP_CHNL_EQUIPPED)
    {
        PMC_RETURN(RGMP_CHNL_STATE_NOT_EQUIPPED); /* it has either been already unprovisioned or deactivated*/
    }
    
    /* read from DB to get calendar entries for the channel */
    key.calendar_entry = DO_NOT_USE_KEY;
    key.channel_num = chnl;
    num_cal_entries=gen_db_query(&(rgmp_handle->base),rgmp_handle->cfg.db_id,rgmp_handle->cfg.db_handle, (void *)&key,(void **)&q_result);
    PMC_LOG_TRACE("num_cal_entries %d\n", num_cal_entries);

    if(num_cal_entries == 0){
        PMC_RETURN(RGMP_ERR_DB_ENTRY_NOT_FOUND);
    }
          
    for (i = 0; i < num_cal_entries; i++)
    {
        if(i == 0)
        {
            ret_val = ohfs_remove_fs_enbl(rgmp_handle->ohfs_remove_handle, chnl, 0);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
        }
        
        ret_val = ohfs_remove_chnl_unprov(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, chnl);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        
        ret_val = mp_mgen_ipt_cfg(rgmp_handle->mp_mgen_handle,q_result->p_recs[i]->calendar_entry , chnl, i, 0);
        if (ret_val != PMC_SUCCESS) 
        {
            PMC_RETURN(ret_val);
        }
        
    }
    
    rgmp_var_update(rgmp_handle, chnl, (util_global_chnl_state_t)RGMP_CHNL_START);   
    
                                                                                                 
    PMC_RETURN(PMC_SUCCESS);   
} /* odtu_dmx_rgmp_lo_chnl_unprov */

/*******************************************************************************
* rgmp_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *rgmp_handle      - pointer to RGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95                    
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
PUBLIC PMC_ERROR rgmp_chnl_activate(rgmp_handle_t *rgmp_handle, UINT32 chnl) 

{
    /* variable declaration */
    
    PMC_ENTRY();
    
    if (rgmp_handle->var.rgmp_chnl_cfg[chnl].state != RGMP_CHNL_EQUIPPED)
    {
        PMC_RETURN(RGMP_CHNL_STATE_NOT_EQUIPPED); /* that channel is not in provisioned state*/
    } 
    
    /* Channel reset is released by configuring CH_RSTB to '1' */
    rgmp_chnl_reset_cfg(rgmp_handle, chnl, (util_global_chnl_state_t)1);
    
    /* Update var context for a provisioned channel */
    rgmp_handle->var.rgmp_chnl_cfg[chnl].state = RGMP_CHNL_OPERATIONAL;
   
    PMC_RETURN(PMC_SUCCESS);

} /* rgmp_chnl_activate */

/*******************************************************************************
* rgmp_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *rgmp_handle      - pointer to RGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95                  
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
PUBLIC PMC_ERROR rgmp_chnl_deactivate(rgmp_handle_t *rgmp_handle, UINT32 chnl)                                        

{
    /* variable declaration */
    
    PMC_ENTRY();
    
    if ((UINT32)rgmp_handle->var.rgmp_chnl_cfg[chnl].state != (UINT32)UTIL_GLOBAL_CHNL_OPERATIONAL)
    {
        PMC_RETURN(RGMP_CHNL_STATE_NOT_OPERATIONAL); 
    }
    
    /* put channel in reset by configuring CH_RSTB to '0' */
    rgmp_chnl_reset_cfg(rgmp_handle, chnl, (util_global_chnl_state_t)0);
    
    /* Update var context for the unprovisioned channel */
    rgmp_handle->var.rgmp_chnl_cfg[chnl].state = RGMP_CHNL_EQUIPPED;
   
    PMC_RETURN(PMC_SUCCESS);
   
} /* rgmp_chnl_deactivate */

/*******************************************************************************
* rgmp_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of RGMP  channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *rgmp_handle  - pointer to ohfs_remove handle instance
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
PUBLIC void rgmp_chnl_clean( rgmp_handle_t *rgmp_handle, 
                             UINT32        *chnl,
                             UINT32        num_chnl ) 
{
    UINT32 chnl_itr;

    PMC_ENTRY();
    
    /* clean sub-blocks */
    ohfs_remove_chnl_clean(rgmp_handle->ohfs_remove_handle,chnl,num_chnl);
    mp_mgen_chnl_clean(rgmp_handle->mp_mgen_handle,chnl,num_chnl);

    /* reset channel and put it in START state */
    for(chnl_itr = 0; chnl_itr < num_chnl;chnl_itr++)
    {
        rgmp_chnl_reset_cfg(rgmp_handle, chnl[chnl_itr], (util_global_chnl_state_t)0);
        rgmp_var_update(rgmp_handle, chnl[chnl_itr], (util_global_chnl_state_t)RGMP_CHNL_START);  
    }
    
    PMC_RETURN();

} /* rgmp_chnl_clean */

/*******************************************************************************
* odtu_dmx_rgmp_chnl_tribslot_cfg
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
*   *rgmp_handle        - pointer to RGMP handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ts_loc              - tributary location that the LO channel takes up.
*   client_type         - For DEMUX, LO ODU rate. See util_global_odukp_type_t. \n
*                         For RGMP, adaptaion_function. See
*                         adaptaion_function_enum for valid types
*   server_type         - For DEMUX, HO ODU rate. See util_global_odukp_type_t. \n
*                         For RGMP, ODUkp or ODUjp type see odukp_type_t, ODUkp all ODUk
*                         types valid, ODUjp only ODU2/3 types valid
*   ts_type             - tributary slot type. See odu_line_payload_t. \n
*                         NOTE: Only valid for DEMUX. This argument is ignored
*                               in RGMP.
*   mapping_mode        - mapping mode used in mapping the LO channel to HO ODU. \n
*                         For RGMP, this argument is ignored.
*   ts_occupied         - determines if the tribslot is occupied. \n
*                         1 -- tribslot is used for the channel. \n
*                         0 -- tribslot is not used for the channel.
*   first_ts            - determines whether the tribslot occupied by this LO
*                         channel is the first tributary slot. \n
*                         NOTE: For RGMP, always set to 1. \n
*                         1 -- the tribslot is the first tributary slot. \n
*                         0 -- the tribslot is not the first tributary slot.
*   last_ts             - determines whether the tribslot occupied by this LO
*                         channel is the last tributary slot. \n
*                         NOTE: For RGMP, always set to 1. \n
*                         1 -- the tribslot is the last tributary slot. \n
*                         0 -- the tribslot is not the last tributary slot.
*   frm_num             - For DMX, occurent number of the channel in a calendar  
*                         cycle. When the first tributary slot is added, frm_num
*                         should be set to 0. frm_num should be incremented each
*                         time a new tributary slot is added for the channel.
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
PUBLIC PMC_ERROR odtu_dmx_rgmp_chnl_tribslot_cfg(rgmp_handle_t *rgmp_handle,
                                                 UINT32 chnl, UINT32 ts_loc, 
                                                 UINT32 client_type, 
                                                 UINT32 server_type,
                                                 util_global_odu_line_payload_t ts_type,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 ts_occupied,
                                                 UINT32 first_ts, UINT32 last_ts,
                                                 UINT32 frm_num) 
{
    /* variable declaration */
    rgmp_chnl_state_t chnl_state;
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32     num_cal_entries =0;
    UINT32 i;
    
    mapotn_db_key        key;
    mapotn_db_query_res *q_result;
    
    line_core_otn_db_key core_key;
    
    
    PMC_ENTRY();
    
    /* argument checking */  
    if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
    {
                
        first_ts = 1;
        last_ts = 1; 

        /* go throught the database and get the number of calendar entries for 
           the channel */
        key.calendar_entry = DO_NOT_USE_KEY;
        key.channel_num = chnl;
        num_cal_entries=gen_db_query(&(rgmp_handle->base),rgmp_handle->cfg.db_id,rgmp_handle->cfg.db_handle, (void *)&key,(void **)&q_result);
        PMC_LOG_TRACE("num_cal_entries %d\n", num_cal_entries);
        
        if(num_cal_entries == 0){
            PMC_RETURN(RGMP_ERR_DB_ENTRY_NOT_FOUND);
        }
          
        for (i = 0; i < num_cal_entries; i++)
        {
            if( i==0 )
            {
                ret_val = mp_mgen_ipt_cfg(rgmp_handle->mp_mgen_handle,q_result->p_recs[i]->calendar_entry , chnl, i, 1);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                
                ret_val = ohfs_remove_ipt_cfg(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, chnl, i, 1);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                if (mapping_mode == UTIL_GLOBAL_AMP)
                {
                    ret_val = mp_mgen_amp_cfg(rgmp_handle->mp_mgen_handle, q_result->p_recs[i]->calendar_entry, 0, (util_global_odukp_type_t)server_type,
                                              (util_global_odukp_type_t)client_type, ts_type);
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
                
                ret_val = ohfs_remove_lo_chnl_id_cfg(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, chnl);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }
                
                ret_val = ohfs_remove_ts_cfg(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, mapping_mode, 1, 1, 1);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }
            }
            else
            {
                ret_val = mp_mgen_ipt_cfg(rgmp_handle->mp_mgen_handle,q_result->p_recs[i]->calendar_entry , chnl, i, 0);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                
                ret_val = ohfs_remove_ipt_cfg(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, chnl, i, 0);
                if (ret_val != PMC_SUCCESS) {
                    PMC_RETURN(ret_val);
                }
                
                ret_val = ohfs_remove_lo_chnl_id_cfg(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, chnl);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }
                
                ret_val = ohfs_remove_ts_cfg(rgmp_handle->ohfs_remove_handle, q_result->p_recs[i]->calendar_entry, mapping_mode, 1, 1, 0);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }
            }
        }
        
        PMC_LOG_TRACE("PRINT mp_mgen and ohfs_remove IPT\n");
        mp_mgen_ipt_print(rgmp_handle->mp_mgen_handle); 
        ohfs_remove_ipt_print(rgmp_handle->ohfs_remove_handle);

    } 
    else {
        /* retrieve calendar entries from the DB */
        PMC_LOG_TRACE("%s, %s, %d, I am DMX\n", __FILE__, __FUNCTION__, __LINE__);

        core_key.calendar_entry = ts_loc;
        core_key.ho_channel = DO_NOT_USE_KEY;
        core_key.mo_channel = DO_NOT_USE_KEY;
        core_key.lo_channel = DO_NOT_USE_KEY;
        core_key.trib_slot = DO_NOT_USE_KEY;

        num_cal_entries = gen_db_query_count(&(rgmp_handle->base), rgmp_handle->cfg.db_id, rgmp_handle->cfg.db_handle, (void *)&core_key);
        
        PMC_LOG_TRACE("num_cal_entries = %u\n", num_cal_entries);
        
        if(num_cal_entries == 0){
            PMC_RETURN(RGMP_ERR_DB_ENTRY_NOT_FOUND);
        }
        
        /* if called by ODTU_DMX */
        /* for GMP set only the first entry of each LO channel */
        
        if (mapping_mode == UTIL_GLOBAL_GMP) 
        {
            if (first_ts == 1)
            {
                ret_val = mp_mgen_ipt_cfg(rgmp_handle->mp_mgen_handle, ts_loc, chnl, frm_num, 1);
            }                
        }
        else {
            ret_val = mp_mgen_ipt_cfg(rgmp_handle->mp_mgen_handle, ts_loc, chnl, frm_num, 1);
        }
      
    }
    
    /* rgmp_var_update on ts configuration */
    if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
    {
        chnl_state = RGMP_CHNL_START;
        rgmp_var_update(rgmp_handle, chnl, (util_global_chnl_state_t)chnl_state);
        
    }
    
    PMC_RETURN(ret_val);                       
} /* odtu_dmx_rgmp_chnl_tribslot_cfg */

                                             
/*
** Private Functions
*/

/*******************************************************************************
*  rgmp_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize RGMP var_t variables.
*
* INPUTS:
*   *rgmp_handle      - pointer to RGMP handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR rgmp_var_default_init(rgmp_handle_t *rgmp_handle)
{
    /* variable declaration */
    UINT32 chnl;
    
    PMC_ENTRY();
    
    /* Configure default RGMP variable parameters for each channel */
    for (chnl = 0; chnl < rgmp_handle->cfg.num_rgmp_chanl; chnl++)
    {
        rgmp_handle->var.rgmp_chnl_cfg[chnl].state        = RGMP_CHNL_START; 
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* rgmp_var_default_init */


/*******************************************************************************
* rgmp_var_update
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Updates the channel based dynamically changing context in the memory.                                                                               
*                                                                               
* INPUTS:                                                                       
*   *rgmp_handle        - pointer to RGMP handle instance                  
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*   chnl_state          - the state of the channel see util_global_chnl_state_t 
*                         for valid types                                          
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
PUBLIC void rgmp_var_update(rgmp_handle_t *rgmp_handle, UINT32 chnl,
                            util_global_chnl_state_t chnl_state)
{
    /* variable declaration */
    PMC_ENTRY();
    
    /* Update var context for a channel */
    rgmp_handle->var.rgmp_chnl_cfg[chnl].state        = (rgmp_chnl_state_t)chnl_state;
   
    PMC_RETURN();
}  /* rgmp_var_update */ 



/*******************************************************************************
*  rgmp_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures reset mode for RGMP channel.
*
*
* INPUTS:
*   *rgmp_handle      - pointer to RGMP handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   chnl_state        - register access state of RGMP channel \n
*                       0 -- RGMP channel is in reset mode. It clears all the 
*                            context information for that channel \n
*                       1 -- RGMP channel is out of reset.
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES: For reconfiguring a channel to a different mode, SW should first assert
*        the reset, change the configuration and then release the reset.
*
*******************************************************************************/
PRIVATE PMC_ERROR rgmp_chnl_reset_cfg(rgmp_handle_t *rgmp_handle,
                                      UINT32 chnl,
                                      util_global_chnl_state_t chnl_state)
{
    /* variable declaration */
    
    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, chnl_state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_state);
    
    rgmp_field_CHAN_RSTB_set(NULL, rgmp_handle, chnl, chnl_state);
    
    if(chnl_state == 0)
    {
       PMC_Q_USLEEP(rgmp_handle,1); 
    }

    PMC_RETURN(PMC_SUCCESS);

} /* rgmp_chnl_reset_cfg */

/*******************************************************************************
* rgmp_server_num_ts_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the number of tributary slots required for LO ODU
*   into HO OPUk. This function is used for ODTU_DMX only.
*
*
* INPUTS:
*   *rgmp_handle        - pointer to RGMP handle instance
*   server_type         - HO ODU rate. See util_global_odukp_type_t.
*   client_type         - LO ODU rate for ODTU_DMX. See util_global_odukp_type_t.
*                         Adaptation_function for RGMP. See adaptation_func_enum.
*   ts_type             - tributary slot size used to map LO ODU into HO OPU
*   *num_tribslot       - number of tributary slots reguired by LO ODU into 
*                         the HO ODU                          
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
PRIVATE PMC_ERROR rgmp_server_num_ts_cfg(rgmp_handle_t *rgmp_handle, 
                                         util_global_odukp_type_t server_type,
                                         util_global_odukp_type_t client_type, 
                                         util_global_odu_line_payload_t ts_type,
                                         UINT32 *num_tribslot)
{
    /* variable declaration */
    UINT32 num_ts = 0;
    
    PMC_ENTRY();
    /* argument checking */
    
    switch (server_type)
    {
        case UTIL_GLOBAL_ODU0:
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 1;
            }
            break;
        case UTIL_GLOBAL_ODU1:
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 2;
            }
            else if (client_type == UTIL_GLOBAL_ODU0)
            {
                num_ts = 1;
            } 
            break;       
        case UTIL_GLOBAL_ODU2:
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 8;
            }
            else if (client_type == UTIL_GLOBAL_ODU1 && ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                num_ts = 1;
            }
            else if (client_type == UTIL_GLOBAL_ODU1 && ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                num_ts = 2;
            }
            break;
        case UTIL_GLOBAL_ODU3:          
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 32;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                if (client_type == UTIL_GLOBAL_ODU1)
                {
                    num_ts = 1;
                }
                else if (client_type == UTIL_GLOBAL_ODU2)
                {
                    num_ts = 4;
                } 
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                if (client_type == UTIL_GLOBAL_ODU0)
                {
                    num_ts = 1;
                }
                else if (client_type == UTIL_GLOBAL_ODU1)
                {
                    num_ts = 2;
                }
                else if (client_type == UTIL_GLOBAL_ODU2)
                {
                    num_ts = 8;
                }
                else if (client_type == UTIL_GLOBAL_ODU2E
                         || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                         || client_type == UTIL_GLOBAL_ODU2F
                         || client_type == UTIL_GLOBAL_ODU1E
                         || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                         || client_type == UTIL_GLOBAL_ODU1F)
                {
                    num_ts = 9;
                }   
            }
            break;
        case UTIL_GLOBAL_ODU3E1:
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 32;
            }
            else {
                if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                {
                    if (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
                    {
                        num_ts = 4;
                    }
                }
                else {
                    PMC_RETURN(RGMP_ERR_INVALID_PARAMETERS);
                } 
            }    
            break;
        case UTIL_GLOBAL_ODU3E2:
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 32;
            }
            else {
                if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
                {
                    if (client_type == UTIL_GLOBAL_ODU0)
                    {
                        num_ts = 1;
                    }
                    else if (client_type == UTIL_GLOBAL_ODU1)
                    {
                        num_ts = 2;
                    }
                    else if (client_type == UTIL_GLOBAL_ODU2
                             || client_type == UTIL_GLOBAL_ODU2E
                             || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
                    {
                        num_ts = 8;
                    }
                    else if (client_type == UTIL_GLOBAL_ODU2F
                             || client_type == UTIL_GLOBAL_ODU1E
                             || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                             || client_type == UTIL_GLOBAL_ODU1F)
                    {
                        num_ts = 9;
                    } 
                    else if (client_type == UTIL_GLOBAL_ODU3)
                    {
                        num_ts = 32;
                    }  
                }
                else {
                    PMC_RETURN(RGMP_ERR_INVALID_PARAMETERS);
                } 
            }    
            break;
        case UTIL_GLOBAL_ODU4:
            if (rgmp_handle->cfg.instance == INSTANCE_RGMP)
            {
                num_ts = 80;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                if (client_type == UTIL_GLOBAL_ODU0)
                {
                    num_ts = 1;
                }
                else if (client_type == UTIL_GLOBAL_ODU1)
                {
                    num_ts = 2;
                }
                else if (client_type == UTIL_GLOBAL_ODU2 
                         || client_type == UTIL_GLOBAL_ODU2E
                         || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
                {
                    num_ts = 8;
                }
                else if (client_type == UTIL_GLOBAL_ODU1E
                         || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                         || client_type == UTIL_GLOBAL_ODU1F
                         || client_type == UTIL_GLOBAL_ODU2F)
                {
                    num_ts = 9;    
                }
                else if (client_type == UTIL_GLOBAL_ODU3)
                {
                    num_ts = 31;
                }
                else if (client_type == UTIL_GLOBAL_ODU3E1
                         || client_type == UTIL_GLOBAL_ODU3E2)
                {
                    num_ts = 33;
                }
            }
            break;
        default:
            PMC_RETURN(RGMP_ERR_INVALID_PARAMETERS);
    }
    
    *num_tribslot = num_ts;
    
    PMC_RETURN(PMC_SUCCESS);
} /* rgmp_server_num_ts_cfg */  



/*******************************************************************************
* FUNCTION: rgmp_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   rgmp_handle      - Pointer to subsystem handle
*   chnl_id          - Channel ID
*   non_null_ptr     - Pointer to check that shouldn't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR rgmp_int_validate(rgmp_handle_t    *rgmp_handle,
                                   UINT32            chnl_id,
                                   void             *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != rgmp_handle, RGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= RGMP_NUM_CHANL)
    {
        result = RGMP_ERR_INVALID_PARAMETERS;
    }

    if (NULL == non_null_ptr)
    {
        result = RGMP_ERR_INVALID_PARAMETERS;
    }


    PMC_RETURN(result);
} /* rgmp_int_validate */



/*******************************************************************************
* FUNCTION: rgmp_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   rgmp_handle       - Pointer to subsystem handle
*   chnl_id           - Channel ID
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
PUBLIC PMC_ERROR rgmp_int_chnl_enable(rgmp_handle_t    *rgmp_handle,
                                      UINT32            chnl_id,
                                      rgmp_int_chnl_t  *int_table_ptr,
                                      rgmp_int_chnl_t  *int_en_table_ptr,
                                      BOOL8              enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = rgmp_int_validate(rgmp_handle, chnl_id, int_en_table_ptr);

    #ifdef RGMP_INT
    #undef RGMP_INT
    #endif
    #define RGMP_INT UTIL_GLOBAL_INT_ENABLE_RANGE

    RGMP_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* rgmp_int_chnl_enable */

/*******************************************************************************
* FUNCTION: rgmp_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   rgmp_handle   - Pointer to subsystem handle
*   int_table_ptr - pointer to channel interrupt table with
*                   bits set to clear interrupts. If NULL, clears
*                   all interrupts
*   chnl_id       - Channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR rgmp_int_chnl_clear(rgmp_handle_t    *rgmp_handle,
                                     UINT32            chnl_id,
                                     rgmp_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = rgmp_int_validate(rgmp_handle, chnl_id, &chnl_id);

    #ifdef RGMP_INT
    #undef RGMP_INT
    #endif
    #define RGMP_INT UTIL_GLOBAL_INT_CLEAR_RANGE
    

    RGMP_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* rgmp_int_chnl_clear */




/*******************************************************************************
* FUNCTION: rgmp_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*
* INPUTS:
*   rgmp_handle       - Pointer to subsystem handle
*   chnl_id           - Channel ID
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
PUBLIC PMC_ERROR rgmp_int_chnl_retrieve(rgmp_handle_t    *rgmp_handle,
                                        UINT32            chnl_id,
                                        rgmp_int_chnl_t  *filt_table_ptr,
                                        rgmp_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;
    rgmp_buffer_t  *rgmp_buffer_ptr = NULL;

    PMC_ENTRY();

    result = rgmp_int_validate(rgmp_handle, chnl_id, int_table_ptr);

    
    #ifdef RGMP_INT
    #undef RGMP_INT
    #endif
    #define RGMP_INT UTIL_GLOBAL_INT_RETRIEVE_RANGE
    
    RGMP_INT_TABLE_DEFINE();
   

    #ifdef RGMP_STATUS
    #undef RGMP_STATUS
    #endif
    #define RGMP_STATUS UTIL_GLOBAL_STATUS_RETRIEVE_RANGE
    
    RGMP_STATUS_TABLE_DEFINE();
    
    


    PMC_RETURN(result);
} /* rgmp_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: rgmp_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   rgmp_handle       - Pointer to subsystem handle
*   chnl_id           - Channel ID
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
PUBLIC PMC_ERROR rgmp_int_chnl_enabled_check(rgmp_handle_t     *rgmp_handle,
                                             UINT32             chnl_id,
                                             rgmp_int_chnl_t   *int_en_table_ptr,
                                             BOOL8              *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = rgmp_int_validate(rgmp_handle, chnl_id, int_found_ptr);


    #ifdef RGMP_INT
    #undef RGMP_INT
    #endif
    #define RGMP_INT UTIL_GLOBAL_INT_CHECK_RANGE

    RGMP_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* rgmp_int_chnl_enabled_check */

/*******************************************************************************
* rgmp_activated_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *rgmp_handle      - pointer to MAPOTN handle instance
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
PUBLIC void rgmp_activated_chnl_get(rgmp_handle_t *rgmp_handle,
                                    BOOL8 *actv_chnl)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(rgmp_handle != NULL, RGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < RGMP_NUM_CHANL; itr++)
    {
        actv_chnl[itr] = rgmp_field_CHAN_RSTB_get(NULL, rgmp_handle, itr)==0?FALSE:TRUE;
    }

    PMC_RETURN();
}  /* rgmp_activated_chnl_get */

/*******************************************************************************
* rgmp_mapping_mode_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns the mapping mode at a specified calendar cycle
*                                                                               
* INPUTS:                                                                       
*   *rgmp_handle      - pointer to MAPOTN handle instance
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
PUBLIC util_global_mapping_mode_t rgmp_mapping_mode_get(rgmp_handle_t *rgmp_handle,
                                                        UINT32 calendar_cycle)
{
    util_global_mapping_mode_t mode;
    PMC_ENTRY();
    
    PMC_ASSERT(rgmp_handle != NULL, RGMP_ERR_INVALID_PARAMETERS, 0, 0);
    
    mode = ohfs_remove_mapping_mode_get(rgmp_handle->ohfs_remove_handle,calendar_cycle);
    
    PMC_RETURN(mode);
}  /* rgmp_mapping_mode_get*/

/*******************************************************************************
* rgmp_fixed_stuff_enable_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns TRUE when fixed stuff is enabled
*                                                                               
* INPUTS:                                                                       
*   *rgmp_handle      - pointer to MAPOTN handle instance
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
PUBLIC BOOL8 rgmp_fixed_stuff_enable_get(rgmp_handle_t *rgmp_handle,
                                         UINT32 chnl)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();
    result = ohfs_remove_fixed_stuff_enable_get(rgmp_handle->ohfs_remove_handle,chnl);
    
    PMC_RETURN(result);
} /* rgmp_fixed_stuff_enable_get */

/*
** End of file
*/
