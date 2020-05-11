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

#include "expand_ibuf_loc.h"



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
const char EXPAND_IBUF_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    EXPAND_IBUF_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char EXPAND_IBUF_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void expand_ibuf_var_default_init(expand_ibuf_handle_t *expand_ibuf_handle);

PRIVATE UINT32 expand_ibuf_srv_size_calc(expand_ibuf_handle_t *expand_ibuf_handle,
                                         UINT32 chnl,
                                         util_global_odukp_type_t server_type,
                                         UINT32 client_type,
                                         util_global_mapping_mode_t mapping_mode,
                                         util_global_odu_line_payload_t ts_type);

PRIVATE UINT32 expand_ibuf_m_calc(expand_ibuf_handle_t *expand_ibuf_handle,
                                  UINT32 chnl, UINT32 client_type,
                                  util_global_odukp_type_t server_type,
                                  UINT32 num_tribslot);

PRIVATE PMC_ERROR expand_ibuf_odtu_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 chnl,
                                             UINT32 num_tribslot,
                                             UINT32 server_size,
                                             util_global_mapping_mode_t mapping_mode);

PRIVATE PMC_ERROR expand_ibuf_num_trig_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 chnl, 
                                           util_global_odukp_type_t server_type,  
                                           UINT32 client_type,
                                           util_global_mapping_mode_t mapping_mode,
                                           UINT32 num_tribslot,
                                           util_global_odu_line_payload_t ts_type);

PRIVATE PMC_ERROR expand_ibuf_txjc_mgen_uncfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 chnl);

PRIVATE PMC_ERROR expand_ibuf_hao_cfg_uncfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                            UINT32 chnl);
PRIVATE BOOL8 expand_ibuf_find_rates(util_global_odukp_type_t server_types[],
                                     util_global_odukp_type_t client_types[],
                                     UINT32 num_server_rate,
                                     UINT32 num_client_rate,
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_tribslot,
                                     UINT32 golden_cn, UINT32 cn_n, UINT32 cn_d,
                                     util_global_odukp_type_t *server_type,
                                     util_global_odukp_type_t *client_type);

PRIVATE UINT32 expand_ibuf_calc_mf_tot(expand_ibuf_handle_t *expand_ibuf_handle,
                                       UINT32 cycle,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type);

PUBLIC UINT32 expand_ibuf_calc_ho_cal_cycle_tot(expand_ibuf_handle_t *expand_ibuf_handle,
                                                UINT32 cycle, UINT32 chnl,
                                                util_global_odukp_type_t ho_odu_type,
                                                util_global_odu_line_payload_t ts_type);

PRIVATE PMC_ERROR expand_ibuf_int_validate(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 chnl_id,
                                           void *non_null_ptr);

/*
** Public Functions
*/

/*******************************************************************************
* expand_ibuf_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an EXPAND_IBUF block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the EXPAND_IBUF subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   expand_ibuf_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC expand_ibuf_handle_t *expand_ibuf_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    expand_ibuf_handle_t *expand_ibuf_handle;

    PMC_ENTRY();
    
    expand_ibuf_handle = (expand_ibuf_handle_t*)PMC_CTXT_CALLOC(sizeof(expand_ibuf_handle_t), parent);

    pmc_handle_init(parent, expand_ibuf_handle, sys_handle, PMC_MID_DIGI_EXPAND_IBUF, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(EXPAND_IBUF_LOG_ERR_STRINGS[0], EXPAND_IBUF_LOG_ERR_TABLE_BASE, EXPAND_IBUF_LOG_ERR_COUNT);

    /* Create CFC handle to be accessed within EXPAND_IBUF */
    expand_ibuf_handle->cfc_handle = cfc_ctxt_create((pmc_handle_t *)expand_ibuf_handle, base_address+BASE_ADDR_EXP_EXP_CFC_CFC, sys_handle, "CFC");
    PMC_CTXT_REGISTER_PTR(&expand_ibuf_handle->cfc_handle, parent);

    PMC_RETURN(expand_ibuf_handle);

} /* expand_ibuf_ctxt_create */


/*******************************************************************************
* expand_ibuf_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a EXPAND_IBUF block instance.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
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
PUBLIC void expand_ibuf_ctxt_destroy(expand_ibuf_handle_t *expand_ibuf_handle)
{
    PMC_ENTRY();
    
    /* Destroy handle for CFC LWC in EXPAND_IBUF */
    cfc_ctxt_destroy(expand_ibuf_handle->cfc_handle);

    PMC_CTXT_FREE(&expand_ibuf_handle, expand_ibuf_handle);
    
    PMC_RETURN();
} /* expand_ibuf_ctxt_destroy */

/*******************************************************************************
* expand_ibuf_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an EXPAND_IBUF block handle instance.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   expand_ibuf_instance   - Enum. See expand_ibuf_instance_type_t.
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
PUBLIC void expand_ibuf_handle_init(expand_ibuf_handle_t *expand_ibuf_handle,
                                    expand_ibuf_instance_type_t expand_ibuf_instance)
{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    
    PMC_ENTRY();
     
    /* configure the parent that is using this block */
    expand_ibuf_handle->cfg.instance = expand_ibuf_instance;
    
    /* CFC parameters */
    expand_ibuf_handle->cfg.cfc_cfg.max_fifo_num  = EXPAND_IBUF_CFC_MAX_FIFO_NUM;
    expand_ibuf_handle->cfg.cfc_cfg.page_size     = EXPAND_IBUF_CFC_PAGE_SIZE;

    /* Initialize CFC */
    cfc_cfg.max_fifo_num = expand_ibuf_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = expand_ibuf_handle->cfg.cfc_cfg.page_size;
    cfc_handle_init(expand_ibuf_handle->cfc_handle, &cfc_cfg);
    
    /* Initialize var_t parameters */
    expand_ibuf_var_default_init(expand_ibuf_handle);

    PMC_RETURN();
} /* expand_ibuf_handle_init */

/*******************************************************************************
* expand_ibuf_alloc_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocates any memory freed in ctxt_destroy
*
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
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
PUBLIC void expand_ibuf_alloc_init(expand_ibuf_handle_t *expand_ibuf_handle)
{
    PMC_ENTRY();

    cfc_alloc_init(expand_ibuf_handle->cfc_handle);

    PMC_RETURN();
} /* expand_ibuf_alloc_init */


/*******************************************************************************
* expand_ibuf_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reinitializes an EXPAND_IBUF block handle instance.
*
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
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
PUBLIC void expand_ibuf_handle_restart_init(expand_ibuf_handle_t *expand_ibuf_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state)

{
    /* variable declaration */
    cfc_cfg_t   cfc_cfg;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        pmc_log_block_strings_register(EXPAND_IBUF_LOG_ERR_STRINGS[0], EXPAND_IBUF_LOG_ERR_TABLE_BASE, EXPAND_IBUF_LOG_ERR_COUNT);
    }
     
    /* Initialize CFC */
    cfc_cfg.max_fifo_num = expand_ibuf_handle->cfg.cfc_cfg.max_fifo_num;
    cfc_cfg.page_size    = expand_ibuf_handle->cfg.cfc_cfg.page_size;
    cfc_handle_restart_init(expand_ibuf_handle->cfc_handle, &cfc_cfg, restart_init_cfg_ptr, top_energy_state);

    PMC_RETURN();
} /* expand_ibuf_handle_restart_init */


/*******************************************************************************
*  expand_ibuf_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize EXPAND_IBUF with parameters specific to this instantiation.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   energy_state          - enum defining state to set the EXPAND_IBUF module to
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This function uses pmc_energy_state_t for defining its energy states as there 
*   are no optimization in energy state required for EXPAND_IBUF module 
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_init(expand_ibuf_handle_t *expand_ibuf_handle, 
                                  pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    PMC_ASSERT(expand_ibuf_handle!=NULL, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        ret_val = expand_ibuf_block_non_default_cfg(expand_ibuf_handle);
        if(PMC_SUCCESS == ret_val)
        {
            /* Initialize IPT in CFC */
            ret_val = cfc_init(expand_ibuf_handle->cfc_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {    
            ret_val = cfc_bypass(expand_ibuf_handle->cfc_handle, FALSE);
        }  
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        /* Initialize var_t parameters */
        expand_ibuf_var_default_init(expand_ibuf_handle);
        ret_val = cfc_init(expand_ibuf_handle->cfc_handle, energy_state);  
        
    } else {
        PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    }    
  
    PMC_RETURN(ret_val);

} /* expand_ibuf_init */


/*******************************************************************************
* expand_ibuf_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   EXPAND_IBUF instance.
*
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
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
PUBLIC BOOL expand_ibuf_start_state_test(expand_ibuf_handle_t *expand_ibuf_handle) 
{
    PMC_ENTRY();
    PMC_ASSERT(expand_ibuf_handle!=NULL, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    
    if(cfc_start_state_test(expand_ibuf_handle->cfc_handle) == FALSE)
        PMC_RETURN(FALSE);
  
    PMC_RETURN(TRUE);

} /* expand_ibuf_start_state_test */


/*******************************************************************************
*  expand_ibuf_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
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
PUBLIC PMC_ERROR expand_ibuf_ram_init(expand_ibuf_handle_t *expand_ibuf_handle)
{
    /* variable declaration */
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* Initialize CHANNEL_CFG_REG */
    /* Initialize HO_ID to 0x7F */
    for (i = 0; i < 96; i++)
    {
        expand_ibuf_field_MF_TOT_set(NULL, expand_ibuf_handle, i, 0);
        expand_ibuf_field_HO_CAL_CYCLE_TOT_set(NULL, expand_ibuf_handle, i, 0);
        expand_ibuf_field_HO_FIRST_CYCLE_set(NULL, expand_ibuf_handle, i, 0);
        expand_ibuf_field_HO_LAST_CYCLE_set(NULL, expand_ibuf_handle, i, 0);
        expand_ibuf_field_HO_ID_set(NULL, expand_ibuf_handle, i, 0x7F);
    }

    for (i = 0; i < 96; i++)
    {
        expand_ibuf_reg_TXJC_MGEN_CFGS_array_write(NULL, expand_ibuf_handle, i, 0);
        expand_ibuf_reg_HAO_CFG_REG_array_write(NULL, expand_ibuf_handle, i, 0);
    }

    
    /* initialize IPT_ID = 0x7F, IPT_VA = 0x7F */
    for (i = 0; i < 96; i++)
    {
        result = cfc_ipt_cfg(expand_ibuf_handle->cfc_handle, i, 0, 0x7F, 0x7F);
    }
    
    PMC_RETURN(result);

} /* expand_ibuf_ram_init */

/*******************************************************************************
* expand_ibuf_lo_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes and provisions a LO channel based on the channel's
*   client type and ODUk rate.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl                   - channel ID to be configured
*                            Valid range: 0 - 95
*   server_type            - for DEMUX, HO ODU rate. See odukp_type_enum. 
*                            for RGMP, ODUkp or ODUjp type see odukp_type_t,
*                            ODUkp all ODUk types valid, ODUjp only ODU2/3
*                            types valid
*   client_type            - for DEMUX, LO ODU rate. See odukp_type_enum
*                            for RGMP, adaptaion_function. See
*                            adaptaion_function_enum for valid types
*   client_rate            - client data rate in bits/sec for the case of: \n 
*                            ODUJP_CBRX_GMP \n
*                            ODUKP_CBRX_B_A \n
*                            ODUKP_CBRX_A
*   mapping_mode           - mapping mode. Refer to mapping_mode_enum.
*   num_tribslot           - number of tributary slots in the LO channel
*   ts_type                - tributary slot type. See odu_line_payload_t. \n
*                            NOTE: Only valid for DEMUX. This argument is
*                            ignored in RGMP.
*   num_cal_entries        - number of calendar enrties required for the channel
*   offline_update         - Used for MUX only. \n
*                            0 -- normal configuration \n
*                            1 -- offline page configuration 
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
PUBLIC PMC_ERROR expand_ibuf_lo_chnl_prov(expand_ibuf_handle_t *expand_ibuf_handle,
                                          UINT32 chnl,
                                          util_global_odukp_type_t server_type,
                                          UINT32 client_type,
                                          DOUBLE client_rate,
                                          util_global_mapping_mode_t mapping_mode,
                                          UINT32 num_tribslot,
                                          util_global_odu_line_payload_t ts_type, 
                                          UINT32 num_cal_entries,
                                          BOOL8 offline_update)
{
    /* variable declaration */
    UINT32 map_type;
    UINT32 server_size;
    UINT32 prog_server_size;
    UINT32 m_value;
    PMC_ERROR ret_val = PMC_SUCCESS;
    expand_ibuf_buffer_t expand_ibuf_buf[1];
    
    PMC_ENTRY();
    
    /* variable initialization */
    map_type = 0;
    server_size = 0;

    /* argument checking */
    if ((chnl > 95) || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || (mapping_mode > UTIL_GLOBAL_NO_MAP)
        || (mapping_mode == UTIL_GLOBAL_GFP) || (num_tribslot > 80) 
        || (ts_type > UTIL_GLOBAL_ODU_TS_2G5))
    {
        PMC_LOG_TRACE("chnl = %u, server_type = %u, client_type = %u, mapping_mode = %u, num_tribslot = %u, ts_type = %u...\n", chnl, server_type, client_type, mapping_mode, num_tribslot, ts_type);
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }

    switch (mapping_mode)
    {
        case UTIL_GLOBAL_BMP:
            map_type = 0;
            break;
        case UTIL_GLOBAL_AMP:
            if(server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1)
            {
                map_type = 2;
            }
            else if(server_type == UTIL_GLOBAL_ODU3E1 && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
            {
                map_type = 3;
            }
            else
            {
                map_type = 1;
            }
            break;
        case UTIL_GLOBAL_GMP:
            map_type = 4;
            break;
        case UTIL_GLOBAL_NO_MAP:
            PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
            break;
        default:
            PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
            break;
    }

    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);

    /* configure MAP_TYPE */
    expand_ibuf_field_CHAN_MAP_TYPE_set(expand_ibuf_buf, expand_ibuf_handle, chnl, map_type);

    if(UTIL_GLOBAL_GMP == mapping_mode)
    {
        /* configure M value */
        m_value = expand_ibuf_m_calc(expand_ibuf_handle, chnl, client_type, server_type, num_tribslot);
        expand_ibuf_field_M_set(expand_ibuf_buf, expand_ibuf_handle, chnl, m_value);
    }


    if (PMC_SUCCESS == ret_val)
    {
        /* configure server size */
        prog_server_size = expand_ibuf_srv_size_calc(expand_ibuf_handle, chnl, server_type, client_type, mapping_mode, ts_type);
        expand_ibuf_field_SERVER_SIZE_set(expand_ibuf_buf, expand_ibuf_handle, chnl, prog_server_size);
    }
    expand_ibuf_buffer_flush(expand_ibuf_buf);
   
    if (PMC_SUCCESS == ret_val)
    { 
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
        else {
            server_size = expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].server_size;
        }
    
        ret_val = expand_ibuf_odtu_cycle_cfg(expand_ibuf_handle, chnl, num_tribslot, server_size, mapping_mode);
        
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            num_tribslot = num_cal_entries;
        }
        ret_val = expand_ibuf_num_trig_cfg(expand_ibuf_handle, chnl, server_type, client_type, mapping_mode, num_tribslot, ts_type);
    }
    
    /* CFC configuration */
    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {
        /* configure CFC_FILL_LVL */
        ret_val = expand_ibuf_cfc_xoff_level_cfg(expand_ibuf_handle, chnl, num_cal_entries);
    }
    
    if (PMC_SUCCESS == ret_val && expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_TGMP)
    {
        /* build CFC FIFO */  
        ret_val = cfc_fifo_build(expand_ibuf_handle->cfc_handle, chnl, num_cal_entries);
    }
            
    PMC_RETURN(ret_val);

} /* expand_ibuf_lo_chnl_prov */


/*******************************************************************************
* expand_ibuf_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl                   - channel ID to be configured
*                            Valid range: 0 - 95
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
PUBLIC PMC_ERROR expand_ibuf_lo_chnl_deprov(expand_ibuf_handle_t *expand_ibuf_handle,
                                            UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val;

    PMC_ENTRY();

    /* Destroy CFC FIFO */
    ret_val = cfc_fifo_destroy(expand_ibuf_handle->cfc_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }


    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_lo_chnl_deprov */

/*******************************************************************************
* expand_ibuf_mux_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions the specified channel for MUX.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl                   - channel ID to be configured
*                            Valid range: 0 - 95
*   offline_update         - indicates if the function is used to perform
*                            the initial configuration or the offline update. \n
*                            0 -- initial config \n
*                            1 -- offline config
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
PUBLIC PMC_ERROR expand_ibuf_mux_lo_chnl_deprov(expand_ibuf_handle_t *expand_ibuf_handle,
                                                UINT32 chnl, UINT32 offline_update)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    ret_val = expand_ibuf_txjc_mgen_uncfg(expand_ibuf_handle, chnl);
    
    /* clear CFC FILL LEVEL */
    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {     
        ret_val = expand_ibuf_cfc_fill_lvl_set(expand_ibuf_handle, chnl, 0);
    }   
    
    /* clear SOMF_CYCLE config */
    if (PMC_SUCCESS == ret_val && offline_update == 0)
    {
    }
    
    /* clear HAO config */
    if (PMC_SUCCESS == ret_val)
    {
        ret_val = expand_ibuf_hao_cfg_uncfg(expand_ibuf_handle, chnl);
    }
                                                 
    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_mux_lo_chnl_deprov */


/*******************************************************************************
* expand_ibuf_ho_chnl_prov_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures each calendar cycle that belongs to a HO channel,
*   including the HO_CAL_CYCLE_TOT.
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   ho_cycle               - calendar cycle to be configured
*                            Valid range: 0 - 95
*   chnl                   - HO channel ID
*                            Valid range: 0 - 95
*   odu_type               - HO ODU rate. See odukp_type_enum.
*   ts_type                - tributary slot type. See odu_line_payload_t.
*   first_cycle            - indicates whether this cycle is the first calendar
*                            cycle for a given HO channel \n
*                            0 -- not first cycle \n
*                            1 -- first cycle
*   last_cycle             - indicates whether this cycle is the last calendar
*                            cycle for a given HO channel \n
*                            0 -- not last cycle \n
*                            1 -- last cycle
*   unset_ho_cal_cycle_tot - indicates whether the ho_cal_cycle_tot should be
*                            set to 0 or the calculated value. \n
*                            0 -- calculated value. \n
*                            1 -- 0.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_ho_chnl_prov_all(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 ho_cycle, UINT32 chnl,
                                              util_global_odukp_type_t odu_type,
                                              util_global_odu_line_payload_t ts_type,
                                              UINT32 first_cycle, UINT32 last_cycle,
                                              BOOL8 unset_ho_cal_cycle_tot)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 mf_tot;
    UINT32 ho_cal_cycle_tot;
    util_global_odu_line_payload_t loc_ts_type;
    expand_ibuf_buffer_t expand_ibuf_buf[1];

    PMC_ENTRY();


    loc_ts_type = ts_type;
    if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
    {
        loc_ts_type = UTIL_GLOBAL_ODU_TS_1G25;
    }
 
    
    mf_tot = expand_ibuf_calc_mf_tot(expand_ibuf_handle, ho_cycle, odu_type, loc_ts_type);
    if (mf_tot > 255)
    {
        PMC_RETURN(mf_tot);
    }

    if (TRUE == unset_ho_cal_cycle_tot)
    {
        ho_cal_cycle_tot = 0;
    }
    else
    {
        ho_cal_cycle_tot = expand_ibuf_calc_ho_cal_cycle_tot(expand_ibuf_handle, ho_cycle, chnl, odu_type, ts_type);
    }

    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);
    expand_ibuf_field_MF_TOT_set(expand_ibuf_buf, expand_ibuf_handle, ho_cycle, mf_tot);
    expand_ibuf_field_HO_FIRST_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, ho_cycle, first_cycle);
    expand_ibuf_field_HO_LAST_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, ho_cycle, last_cycle);
    expand_ibuf_field_HO_ID_set(expand_ibuf_buf, expand_ibuf_handle, ho_cycle, chnl);     
    expand_ibuf_field_HO_CAL_CYCLE_TOT_set(expand_ibuf_buf, expand_ibuf_handle, ho_cycle, ho_cal_cycle_tot);
    expand_ibuf_buffer_flush(expand_ibuf_buf);

    PMC_RETURN(result);

} /* expand_ibuf_ho_chnl_prov_all */

/*******************************************************************************
* expand_ibuf_ho_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the total number of calendar cycles required for
*   the multi-frame, the first and last calendar cycle allocated to the
*   corresponding HO channel and the HO channel ID that corresponds to the
*   incoming LO channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   first_cycle            - indicates whether this cycle is the first calendar
*                            cycle for a given HO channel \n
*                            0 -- not first cycle \n
*                            1 -- first cycle
*   last_cycle             - indicates whether this cycle is the last calendar
*                            cycle for a given HO channel \n
*                            0 -- not last cycle \n
*                            1 -- last cycle
*   chnl                   - HO channel ID associated with this calendar cycle
*                            Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_ho_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                          UINT32 cycle,
                                          UINT32 first_cycle,
                                          UINT32 last_cycle, UINT32 chnl)
{
    /* variable declaration */
    expand_ibuf_buffer_t expand_ibuf_buf[1];

    PMC_ENTRY();
    
    /* argument checking */
    if ((cycle > 95) || (first_cycle > 95) || (last_cycle > 95))
    {
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }

    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);
    expand_ibuf_field_HO_FIRST_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, cycle, first_cycle);
    expand_ibuf_field_HO_LAST_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, cycle, last_cycle);
    expand_ibuf_field_HO_ID_set(expand_ibuf_buf, expand_ibuf_handle, cycle, chnl);     
    expand_ibuf_buffer_flush(expand_ibuf_buf);


    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_ho_cycle_cfg */

/*******************************************************************************
* expand_ibuf_ho_last_cycle_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the calendar cycle of the last cycle of a HO channel.
*   This is only used for MO ODU3e1 and MO ODU3e1 case.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   last_cycle             - indicates whether this cycle is the last calendar
*                            cycle for a given HO channel \n
*                            0 -- not last cycle \n
*                            1 -- last cycle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_ho_last_cycle_set(expand_ibuf_handle_t *expand_ibuf_handle,
                                               UINT32 cycle, UINT32 last_cycle)
{
    /* variable declaration */
    PMC_ENTRY();
    
    /* argument checking */
    if (cycle > 95 || last_cycle > 95)
    {
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }

    expand_ibuf_field_HO_LAST_CYCLE_set(NULL, expand_ibuf_handle, cycle, last_cycle);
    
    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_ho_last_cycle_set */

/*******************************************************************************
* expand_ibuf_calc_ho_cal_cycle_tot
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the total number of calendar cycles required for
*   the multi-frame, the first and last calendar cycle allocated to the
*   corresponding HO channel and the HO channel ID that corresponds to the
*   incoming LO channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   chnl                   - HO channel ID which this calendar cycle belongs to
*                            if no HO channel exists it must be set to 127
*                            Valid range: 0 - 95
*   ho_odu_type            - HO ODU rate. See util_global_odukp_type_t.
*   ts_type                - enum for tributary size. See util_global_odu_line_payload_t.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                 - The total nuymber of calendar cycles required for the multiframe.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC UINT32 expand_ibuf_calc_ho_cal_cycle_tot(expand_ibuf_handle_t *expand_ibuf_handle,
                                                UINT32 cycle, UINT32 chnl,
                                                util_global_odukp_type_t ho_odu_type,
                                                util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    UINT32 ho_cal_cycle_tot;
    DOUBLE tmp_ho_cal_cycle_tot;
    UINT32 server_size;
    BOOL8 use_float32_mode = digi_use_float32_get();

    PMC_ENTRY();
        
    /* variable initialization */
    ho_cal_cycle_tot = 0;
    server_size = 0 ;

    server_size = expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].server_size;

    if (ho_odu_type == UTIL_GLOBAL_ODU4)
    {
        server_size = 15200;
    } else {
        server_size = 15232;
    }
    
    if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
    {
        server_size = server_size / 2;
    }

    if (!use_float32_mode) {
        tmp_ho_cal_cycle_tot = ((DOUBLE)server_size / 48);
    } else {
        tmp_ho_cal_cycle_tot = (DOUBLE)((FLOAT)server_size / 48);
    }
    PMC_LOG_TRACE("%s, %s, %d, tmp_ho_cal_cycle_tot = %f\n", __FILE__, __FUNCTION__, __LINE__, tmp_ho_cal_cycle_tot);
        
    if (!use_float32_mode) {
        ho_cal_cycle_tot = (UINT32)PMC_CEIL(tmp_ho_cal_cycle_tot);
    } else {
        ho_cal_cycle_tot = (UINT32)PMC_CEILF(tmp_ho_cal_cycle_tot);
    }
    PMC_LOG_TRACE("%s, %s, %d, ho_cal_cycle_tot = %u\n", __FILE__, __FUNCTION__, __LINE__, ho_cal_cycle_tot);
    
    
    PMC_RETURN(ho_cal_cycle_tot);

} /* expand_ibuf_calc_ho_cal_cycle_tot */

/*******************************************************************************
* expand_ibuf_ho_cal_cycle_tot_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the total number of calendar cycles required for
*   the multi-frame, the first and last calendar cycle allocated to the
*   corresponding HO channel and the HO channel ID that corresponds to the
*   incoming LO channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   chnl                   - HO channel ID which this calendar cycle belongs to
*                            if no HO channel exists it must be set to 127
*                            Valid range: 0 - 95
*   ho_odu_type            - HO ODU rate. See util_global_odukp_type_t.
*   ts_type                - enum for tributary size. See util_global_odu_line_payload_t.
*   unset                  - indicates if the API is used to set the value or
*                            to initialize to default value. \n
*                            0 -- set the value \n
*                            1 -- initialize to default value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_ho_cal_cycle_tot_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                                  UINT32 cycle, UINT32 chnl,
                                                  util_global_odukp_type_t ho_odu_type,
                                                  util_global_odu_line_payload_t ts_type,
                                                  UINT32 unset)
{
    /* variable declaration */
    UINT32 ho_cal_cycle_tot;

    PMC_ENTRY();
        
    /* variable initialization */
    ho_cal_cycle_tot = 0;

    /* argument checking */
    if (chnl > 95 || cycle > 95 || ho_odu_type > UTIL_GLOBAL_LAST_ODUK
        || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD || unset > 1)
    {
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (unset)
    {
        ho_cal_cycle_tot = 0;
    }
    else
    {
        ho_cal_cycle_tot = expand_ibuf_calc_ho_cal_cycle_tot(expand_ibuf_handle, cycle, chnl, ho_odu_type, ts_type);
    }
    
    expand_ibuf_field_HO_CAL_CYCLE_TOT_set(NULL, expand_ibuf_handle, cycle, ho_cal_cycle_tot);
    
    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_ho_cal_cycle_tot_cfg */

/*******************************************************************************
* expand_ibuf_calc_mf_tot
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the total multi-frames for a given HO channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   server_type            - HO ODU rate. See util_global_odukp_type_t.
*   ts_type                - tributary slot type. See odu_line_payload_t
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                 - The calculated value for MF_TOT.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 expand_ibuf_calc_mf_tot(expand_ibuf_handle_t *expand_ibuf_handle,
                                       UINT32 cycle,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    UINT32 mf_tot;

    PMC_ENTRY();

    /* variable initialization */
    mf_tot = 0;

    /* argument checking */
    if (cycle > 95 || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD)
    {
        PMC_LOG_TRACE(" server_type = %u, ts_type = %u...\n", server_type,  ts_type);
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }
    
    switch (ts_type)
    {
        case UTIL_GLOBAL_ODU_TS_1G25:
            if(UTIL_GLOBAL_ODU1 == server_type)
            {
                mf_tot = 128;
            }
            else if(UTIL_GLOBAL_ODU2 == server_type || 
                    UTIL_GLOBAL_ODU2E == server_type || 
                    UTIL_GLOBAL_ODU2E_ETRANS == server_type ||
                    UTIL_GLOBAL_ODU1F == server_type ||
                    UTIL_GLOBAL_ODU1E == server_type ||
                    UTIL_GLOBAL_ODU2F == server_type)
            {
                mf_tot = 32;
            }
            else if(UTIL_GLOBAL_ODU3 == server_type ||
                    server_type == UTIL_GLOBAL_ODU3E2 ||
                    server_type == UTIL_GLOBAL_ODU3E1)
            {
                mf_tot = 8;
            }
            else if(UTIL_GLOBAL_ODU4 == server_type)
            {
                mf_tot = 16;
            }
            else if (server_type == UTIL_GLOBAL_ODUFLEX_GFP || server_type == UTIL_GLOBAL_ODUFLEX_CBR
                     || server_type == UTIL_GLOBAL_ODU0)
            {
                mf_tot = 0;
            }
            else
            {
                PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
            }
            break;
        case UTIL_GLOBAL_ODU_TS_2G5:
            if(UTIL_GLOBAL_ODU2 == server_type || UTIL_GLOBAL_ODU2E == server_type || UTIL_GLOBAL_ODU2E_ETRANS == server_type)
            {
                mf_tot = 64;
            }
            else if(server_type == UTIL_GLOBAL_ODU3 
                    || server_type == UTIL_GLOBAL_ODU3E1)
            {
                mf_tot = 16;
            }
            else if (server_type == UTIL_GLOBAL_ODUFLEX_GFP || server_type == UTIL_GLOBAL_ODUFLEX_CBR)
            {
                mf_tot = 0;
            }           
            else {
                PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
            }
            break;
        case UTIL_GLOBAL_ODU_UNCHANNELIZED:
            PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
            break;
        default:
            PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
            break;
    }


    PMC_RETURN(mf_tot);

} /* expand_ibuf_calc_mf_tot */

/*******************************************************************************
* expand_ibuf_ho_mf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the total multi-frames for a given HO channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   server_type            - HO ODU rate. See util_global_odukp_type_t.
*   ts_type                - tributary slot type. See odu_line_payload_t
*   unset                  - indicates if the API is used for deprovisioning 
*                            or not. \n
*                            0 -- prov \n
*                            1 -- deprov
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_ho_mf_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                       UINT32 cycle,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type,
                                       UINT32 unset)
{
    /* variable declaration */
    UINT32 mf_tot;

    PMC_ENTRY();

    /* argument checking */
    if (cycle > 95 || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD
        || unset > 1)
    {
        PMC_LOG_TRACE(" server_type = %u, ts_type = %u...\n", server_type,  ts_type);
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (unset == 0)
    {
        mf_tot = expand_ibuf_calc_mf_tot(expand_ibuf_handle, cycle, server_type, ts_type);
        /* If this isn't a valid multi-frame total, it's an error code */
        if (mf_tot > 255)
        {
            PMC_RETURN(mf_tot);
        }
    }
    else
    {
        mf_tot = 0;
    }
    
    expand_ibuf_field_MF_TOT_set(NULL, expand_ibuf_handle, cycle, mf_tot);


    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_ho_mf_cfg */

/*******************************************************************************
* expand_ibuf_lo_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the LO channel's first calendar cycle and if during
*   the calendar when EOMF if the cycle is a Data cycle or an unused Stuff cycle
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   first_cycle            - indicates whether this cycle is the first calendar
*                            cycle for the LO channel that occupies it \n
*                            0 -- not first cycle \n
*                            1 -- first cycle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_lo_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                          UINT32 cycle,
                                          UINT32 first_cycle)
{
    /* variable declaration */
  
    /* variable initialization */
    PMC_ENTRY();
    
    /* argument checking */
    if ((cycle > 95) || (first_cycle > 1))
    {
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }

    expand_ibuf_field_LO_FIRST_CYCLE_set(NULL, expand_ibuf_handle, cycle, first_cycle);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_lo_cycle_cfg */

/*******************************************************************************
* expand_ibuf_lo_cycle_cfg_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the LO channel's first calendar cycle and if during
*   the calendar when EOMF if the cycle is a Data cycle or an unused Stuff cycle
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   first_cycle            - indicates whether this cycle is the first calendar
*                            cycle for the LO channel that occupies it \n
*                            0 -- not first cycle \n
*                            1 -- first cycle
*   eomf_cycle             - value to be configured to EOMF_DATA_CYCLE
*                            Valid value: 0 - 1
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_lo_cycle_cfg_all(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 cycle,
                                              UINT32 first_cycle,
                                              UINT32 eomf_cycle)
{
    /* variable declaration */
    expand_ibuf_buffer_t expand_ibuf_buf[1];
  
    /* variable initialization */
    PMC_ENTRY();
    
    /* argument checking */
    if (cycle > 95 || first_cycle > 1)
    {
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }

    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);
    expand_ibuf_field_LO_FIRST_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, cycle, first_cycle);
    expand_ibuf_field_EOMF_DATA_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, cycle, eomf_cycle);
    expand_ibuf_buffer_flush(expand_ibuf_buf);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_lo_cycle_cfg */

/*******************************************************************************
* expand_ibuf_first_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the first cycle of the LO channel.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl                   - calendar cycle to be configured
*                            Valid range: 0 - 95
*   first_cycle            - indicates whether this cycle is the first calendar
*                            cycle for a given HO channel \n
*                            0 -- not first cycle \n
*                            1 -- first cycle
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_first_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 chnl,
                                             UINT32 first_cycle)
{
    /* variable declaration */
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl > 95 || first_cycle > 1)
    {
        PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
    }

    expand_ibuf_field_FIRST_CYCLE_set(NULL, expand_ibuf_handle, chnl, first_cycle);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_first_cycle_cfg */

/*******************************************************************************
* expand_ibuf_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize common configuration register for all channels. That is required
*   for proper operation of this block.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
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
PUBLIC UINT32 expand_ibuf_block_non_default_cfg(expand_ibuf_handle_t *expand_ibuf_handle)
{
    /* Variable declaration */
    UINT32 chnl;

    PMC_ENTRY();

    /* Initialize CFC_FILL_LVL for all channels */
    for (chnl = 0; chnl < EXPAND_IBUF_NUM_CHANL; ++chnl)
    {
        expand_ibuf_field_CFC_FILL_LVL_set(NULL, expand_ibuf_handle, chnl, 0);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_block_non_default_cfg */


/*******************************************************************************
* expand_ibuf_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel.
*                                                                               
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance                                                                             
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
PUBLIC PMC_ERROR expand_ibuf_chnl_activate(expand_ibuf_handle_t *expand_ibuf_handle, 
                                           UINT32 chnl) 

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
     
    /* take the specified CFC FIFO out of reset */
    ret_val = cfc_fifo_activate(expand_ibuf_handle->cfc_handle,chnl);
    
    PMC_RETURN(ret_val);

} /* expand_ibuf_chnl_activate */


/*******************************************************************************
* expand_ibuf_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance                                                                             
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
PUBLIC PMC_ERROR expand_ibuf_chnl_deactivate(expand_ibuf_handle_t *expand_ibuf_handle, 
                                             UINT32 chnl) 
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
     
    /* put the specified CFC FIFO into reset */
    ret_val = cfc_fifo_deactivate(expand_ibuf_handle->cfc_handle,chnl);
   
    PMC_RETURN(ret_val);
   
} /* expand_ibuf_chnl_deactivate */


/*******************************************************************************
* expand_ibuf_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of EXPAND IBUF channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *expand_ibuf_handle  - pointer to EXPAND_IBUF handle instance
*   *chnl                - set of channel ID to be cleaned
*   num_chnl             - number of channel ID to be cleaned
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
PUBLIC void expand_ibuf_chnl_clean(expand_ibuf_handle_t *expand_ibuf_handle, 
                                   UINT32 *chnl,
                                   UINT32 num_chnl) 
{
    PMC_ENTRY();
    
    /* clean sub-blocks */
    cfc_fifo_clean(expand_ibuf_handle->cfc_handle,chnl,num_chnl);


       
    PMC_RETURN();

} /* expand_ibuf_chnl_clean */

/*******************************************************************************
* expand_ibuf_chnl_cycle_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of EXPAND_IBUF channels and cycles (demux mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *expand_ibuf_handle - pointer to EXPAND_IBUF handle instance
*   *ho_chnl            - set of HO channel ID to be cleaned
*   num_ho_chnl         - number of channel ID to be cleaned
*   *lo_chnl            - set of channel ID to be cleaned
*   num_lo_chnl         - number of channel ID to be cleaned
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
PUBLIC void expand_ibuf_chnl_cycle_clean(expand_ibuf_handle_t *expand_ibuf_handle, 
                                         UINT32 *ho_chnl,
                                         UINT32 num_ho_chnl,
                                         UINT32 *lo_chnl,
                                         UINT32 num_lo_chnl) 
{
    UINT32 ho_chnl_itr, lo_chnl_itr,cycle_itr, ho_id;
    BOOL8   hit;

    PMC_ENTRY();

    /* clean sub-blocks */
    cfc_fifo_clean(expand_ibuf_handle->cfc_handle,lo_chnl,num_lo_chnl);
    
    /* Disable channel stuffs */
    for(lo_chnl_itr = 0; lo_chnl_itr < num_lo_chnl;lo_chnl_itr++)
    {
        (void) expand_ibuf_cfc_fill_lvl_set(expand_ibuf_handle,lo_chnl[lo_chnl_itr],0);
    }
                            
    /* clean IPT */
    for(cycle_itr=0; cycle_itr<EXPAND_IBUF_NUM_CHANL; cycle_itr++)
    {
        ho_id = expand_ibuf_field_HO_ID_get(NULL, expand_ibuf_handle, cycle_itr); 
        
        if (0x7F != ho_id)
        {
            hit = FALSE;
            for(ho_chnl_itr = 0; ho_chnl_itr < num_ho_chnl && hit == FALSE;ho_chnl_itr++)
            {  
                if (ho_id == ho_chnl[ho_chnl_itr])
                {
                    hit = TRUE;
                    expand_ibuf_field_HO_CAL_CYCLE_TOT_set(NULL, expand_ibuf_handle, cycle_itr, 0);
                    expand_ibuf_field_HO_FIRST_CYCLE_set(NULL, expand_ibuf_handle, cycle_itr,0);
                    expand_ibuf_field_HO_LAST_CYCLE_set(NULL, expand_ibuf_handle, cycle_itr, 0);
                    expand_ibuf_field_HO_ID_set(NULL, expand_ibuf_handle, cycle_itr, 0x7F);     
                    expand_ibuf_field_MF_TOT_set(NULL, expand_ibuf_handle, cycle_itr, 0);     
                }
            }
        }
    }
    
    PMC_RETURN();

} /* expand_ibuf_chnl_cycle_clean */

/*******************************************************************************
* expand_ibuf_lo_cycle_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of EXPAND_IBUF cycles (demux mode)
*                                                                                                                                      
* INPUTS:                                                                       
*   *expand_ibuf_handle  - pointer to EXPAND_IBUF handle instance
*   *lo_chnl             - set of channel ID to be cleaned
*   num_lo_chnl          - number of channel ID to be cleaned
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
PUBLIC void expand_ibuf_lo_cycle_clean(expand_ibuf_handle_t *expand_ibuf_handle, 
                                       UINT32 *lo_chnl,
                                       UINT32 num_lo_chnl,
                                       UINT32 *cycle,
                                       UINT32 num_cycle) 
{
    UINT32 cycle_itr, lo_chnl_itr;

    PMC_ENTRY();
   
    for(cycle_itr=0; cycle_itr<num_cycle; cycle_itr++)
    {
        expand_ibuf_field_LO_FIRST_CYCLE_set(NULL, expand_ibuf_handle, cycle[cycle_itr], 0);
        expand_ibuf_field_EOMF_DATA_CYCLE_set(NULL, expand_ibuf_handle, cycle[cycle_itr], 0);
    }

    for(lo_chnl_itr=0; lo_chnl_itr<num_lo_chnl;lo_chnl_itr++)
    {
        (void) expand_ibuf_txjc_mgen_uncfg(expand_ibuf_handle, lo_chnl[lo_chnl_itr]);
        (void) expand_ibuf_first_cycle_cfg(expand_ibuf_handle, lo_chnl[lo_chnl_itr], 0);
        (void) expand_ibuf_hao_cfg_uncfg(expand_ibuf_handle,lo_chnl[lo_chnl_itr]);
        expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[lo_chnl[lo_chnl_itr]].server_size  = 0;
        expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[lo_chnl[lo_chnl_itr]].eomf_cycle = 0;
        expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[lo_chnl[lo_chnl_itr]].M = 0;
    }    
    
    PMC_RETURN();

} /* expand_ibuf_lo_cycle_clean */

/*******************************************************************************
* expand_ibuf_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *expand_ibuf_handle  - pointer to EXPAND_IBUF handle instance
*   chnl_id              - Channel ID: Valid range: 0 - 95
*   int_table_ptr        - pointer to channel interrupt table with
*                          bits set to enable interrupts. If NULL, enables/disables
*                          all interrupts
*   int_en_table_ptr     - Pointer to table tracking enabled interrupts
*   enable               - TRUE to enable interrupts, FALSE to disable interrupts
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
PUBLIC PMC_ERROR expand_ibuf_int_chnl_enable(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 chnl_id,
                                             expand_ibuf_int_chnl_t *int_table_ptr,
                                             expand_ibuf_int_chnl_t *int_en_table_ptr,
                                             BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = expand_ibuf_int_validate(expand_ibuf_handle, chnl_id, int_en_table_ptr);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_enable(expand_ibuf_handle->cfc_handle,
                                     chnl_id,
                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc),
                                     &int_en_table_ptr->cfc,
                                     enable);
                                     
    }

    PMC_RETURN(result);
} /* expand_ibuf_int_chnl_enable */

/*******************************************************************************
* expand_ibuf_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
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
PUBLIC PMC_ERROR expand_ibuf_int_chnl_clear(expand_ibuf_handle_t *expand_ibuf_handle,
                                            UINT32 chnl_id,
                                            expand_ibuf_int_chnl_t *int_table_ptr,
                                            expand_ibuf_int_chnl_t *int_en_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = expand_ibuf_int_validate(expand_ibuf_handle, chnl_id, (void *)1);
    
    /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_clear(expand_ibuf_handle->cfc_handle,
                                    chnl_id,
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc),
                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cfc));
                                     
    }
    
    PMC_RETURN(result);
} /* expand_ibuf_int_chnl_clear */

/*******************************************************************************
* expand_ibuf_int_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   *expand_ibuf_handle  - pointer to EXPAND_IBUF handle instance
*   chnl_id              - Channel ID: Valid range: 0 - 95
*   filt_table_ptr       - pointer to filter table. Mark fields as TRUE within this
*                          table to indicate that the interrupt/status will be retrieved
*                          to the output table. NULL to retrieve all 
*                          interrupt/status.
*
* OUTPUTS:
*   int_table_ptr        - pointer to channel interrupt table to fill with 
*                          retrieved data
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_int_chnl_retrieve(expand_ibuf_handle_t *expand_ibuf_handle,
                                               UINT32 chnl_id,
                                               expand_ibuf_int_chnl_t *filt_table_ptr,
                                               expand_ibuf_int_chnl_t *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = expand_ibuf_int_validate(expand_ibuf_handle, chnl_id, int_table_ptr);

     /* CFC */
    if (PMC_SUCCESS == result)
    {
        result = cfc_int_chnl_retrieve(expand_ibuf_handle->cfc_handle,
                                       chnl_id,
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cfc),
                                       &int_table_ptr->cfc);
                                     
    }


    PMC_RETURN(result);
} /* expand_ibuf_int_chnl_retrieve */

/*******************************************************************************
* expand_ibuf_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   *expand_ibuf_handle  - pointer to EXPAND_IBUF handle instance
*   chnl_id              - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr     - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr        - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_int_chnl_enabled_check(expand_ibuf_handle_t *expand_ibuf_handle,
                                                    UINT32 chnl_id,
                                                    expand_ibuf_int_chnl_t *int_en_table_ptr,
                                                    BOOL8 *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    result = expand_ibuf_int_validate(expand_ibuf_handle, chnl_id, int_found_ptr);

     /* CFC */
    if (PMC_SUCCESS == result && 
        *int_found_ptr == FALSE)
    {
        result = cfc_int_chnl_enabled_check(expand_ibuf_handle->cfc_handle,
                                            chnl_id,
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_en_table_ptr, cfc),
                                            int_found_ptr);
                                     
    }

    PMC_RETURN(result);
} /* expand_ibuf_int_chnl_enabled_check */


/*******************************************************************************
* expxand_cfc_fifo_ipt_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This API retrieves the channel ID associated with the page number within
*    the FIFO.
*
*
* INPUTS:
*   *expand_ibuf_handle  - pointer to EXPAND_IBUF handle instance
*   fifo_id              - fifo id
*   frm_num              - frame number within a fifo 
*   
*
* OUTPUTS:
*  ipt_id                - channel ID associated with this frame number of the fifo 
* 
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
*******************************************************************************/
PUBLIC PMC_ERROR expxand_cfc_fifo_ipt_id_get(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 fifo_id,
                                             UINT32 frm_num,
                                             UINT32 *ipt_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = cfc_fifo_ipt_id_get(expand_ibuf_handle->cfc_handle,
                                 fifo_id, frm_num, ipt_id);

    PMC_RETURN(result);
} /* expxand_cfc_fifo_ipt_id_get */
                        

/*
** Private Functions
*/

/*******************************************************************************
*  expand_ibuf_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes EXPAND_IBUF var_t variables.
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
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
PRIVATE void expand_ibuf_var_default_init(expand_ibuf_handle_t *expand_ibuf_handle)
{
    /* Variable declaration */
    UINT32 chnl;

    PMC_ENTRY();

    /* Configure default EXPAND_IBUF variable parameters for each channel */
    for (chnl = 0; chnl < EXPAND_IBUF_NUM_CHANL; chnl++)
    {
        expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].server_size  = 0;
        expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].eomf_cycle = 0;
        expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].M = 0;
    }

    PMC_RETURN();
} /* expand_ibuf_var_default_init */



/*******************************************************************************
* expand_ibuf_srv_size_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the server size of the incoming
*   channels container for the specific mapping mode.  It also updates the
*   context.  The result is expected to be used immediately to set the
*   SERVER_SIZE register bit field.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   server_type           - for DEMUX, HO ODU rate. See odukp_type_enum.
*                           for RGMP, ODUkp or ODUjp type see odukp_type_t,
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3
*                           types valid
*   client_type           - for DEMUX, LO ODU rate. See odukp_type_enum
*                           for RGMP, adaptaion_function. See
*                           adaptaion_function_enum for valid types
*   mapping_mode          - mapping mode used in demuxing the server signal
*   ts_type               - payload format of ODU channel. 
*                           See util_global_odu_line_payload_t.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32               - The server size to be written into the SERVER_SIZE 
*                          register bit field.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 expand_ibuf_srv_size_calc(expand_ibuf_handle_t *expand_ibuf_handle,
                                         UINT32 chnl,
                                         util_global_odukp_type_t server_type,
                                         UINT32 client_type,
                                         util_global_mapping_mode_t mapping_mode,
                                         util_global_odu_line_payload_t ts_type)
{
   
    /* variable declaration */
    UINT32 server_size;
    UINT32 prog_server_size;
    UINT32 M;
    UINT32 fixed_stuff;
    UINT32 payload_size;

    PMC_ENTRY();
    
    /* variable initialization */
    server_size = 0;
    prog_server_size = 0;
    M = 1;

    /* set size of fixed stuff */
    if (expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_TGMP)
    {
        switch (mapping_mode)
        {
             case UTIL_GLOBAL_AMP:
             case UTIL_GLOBAL_BMP:
                if (server_type == UTIL_GLOBAL_ODU2 
                    || server_type == UTIL_GLOBAL_ODU2E 
                    || server_type == UTIL_GLOBAL_ODU2E_ETRANS 
                    || server_type == UTIL_GLOBAL_ODU2F)
                {
                    fixed_stuff = 64;
                }
                else if (server_type == UTIL_GLOBAL_ODU3)
                {
                    fixed_stuff = 128;
                }
                else
                {
                    fixed_stuff = 0;
                }
                break;
             case UTIL_GLOBAL_GMP:
                if (server_type == UTIL_GLOBAL_ODU4)
                {
                    fixed_stuff = 32;
                }
                else
                {
                    fixed_stuff = 0;
                }
          
                M = expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].M;
                break;
             default:
                 PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                 break;
        }

        server_size = 15232 - fixed_stuff;
        /*prog_server_size is only for GMP cases other wise M = 1 always*/
        prog_server_size = server_size/M;

    }
    else if(expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_MUX)
    {
        switch (mapping_mode)
        {
            case UTIL_GLOBAL_AMP:
                switch (server_type)
                {
                    case UTIL_GLOBAL_ODU1:
                        server_size = 15232;
                    break;
                    case UTIL_GLOBAL_ODU2:
                    case UTIL_GLOBAL_ODU2E:
                    case UTIL_GLOBAL_ODU2E_ETRANS:
                        if (ts_type == UTIL_GLOBAL_ODU_TS_1G25 && client_type == UTIL_GLOBAL_ODU1)
                        {
                            server_size = 30464;
                        }
                        else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5 && client_type == UTIL_GLOBAL_ODU1)
                        {
                            server_size = 15232;
                        }
                    break;
                    case UTIL_GLOBAL_ODU3:
                        if (mapping_mode == UTIL_GLOBAL_AMP && client_type == UTIL_GLOBAL_ODU1) 
                        {
                            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
                            {
                                payload_size = 30464;
                                fixed_stuff = 128;
                                server_size = payload_size - fixed_stuff;
                            }
                            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                            {
                                payload_size = 15232;
                                fixed_stuff = 64;
                                server_size = payload_size - fixed_stuff;
                            }
                        }
                        else if (client_type == UTIL_GLOBAL_ODU2) {
                            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
                            {
                                server_size = 121856;
                            }
                            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
                            {
                                server_size = 60928;
                            }   
                        }
                    break;
                    case UTIL_GLOBAL_ODU3E1:
                        if (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS) 
                        {
                            server_size = 60928;
                        }
                    break;
                    default:
                        PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                        break;
                } 
            break;
            case UTIL_GLOBAL_GMP:  
                switch (server_type)
                {
                    case UTIL_GLOBAL_ODU2:
                    case UTIL_GLOBAL_ODU2E:
                    case UTIL_GLOBAL_ODU2E_ETRANS:
                    case UTIL_GLOBAL_ODU3:
                    case UTIL_GLOBAL_ODU3E1:
                    case UTIL_GLOBAL_ODU3E2:
                        payload_size = 15232;
                    break;
                    case UTIL_GLOBAL_ODU4:
                        payload_size = 15200;
                    break;
                    default:
                        PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                        break;
                }
                
                server_size = payload_size;
            break;
            default:
                PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                break;
        }
        
        prog_server_size = server_size;
    }
                            
    
    /* Update var context for a channel */
    expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].server_size = server_size;

    PMC_RETURN(prog_server_size);

} /* expand_ibuf_srv_size_calc */

/*******************************************************************************
* expand_ibuf_lo_eomf_data_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the LO channel's first calendar cycle and if during
*   the calendar when EOMF if the cycle is a Data cycle or an unused Stuff cycle
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   eomf_cycle             - calendar cycle to be configured
*                            Valid range: 0 - 95
*   value                  - value to be configured to EOMF_DATA_CYCLE
*                            Valid value: 0 - 1
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   This is only used in the ODTU_MUX.
*
*******************************************************************************/
PUBLIC PMC_ERROR expand_ibuf_lo_eomf_data_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                                    UINT32 eomf_cycle, UINT32 value)
{
    PMC_ENTRY();

    PMC_ASSERT(expand_ibuf_handle != NULL, EXPAND_IBUF_LOG_ERR_INVALID_PTR, 0, 0);

    /* EOMF_DATA_CYCLE this is the last data cycle */
    expand_ibuf_field_EOMF_DATA_CYCLE_set(NULL, expand_ibuf_handle, eomf_cycle, value);
    
    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_lo_eomf_data_cycle_cfg */

/*******************************************************************************
* expand_ibuf_txjc_mgen_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the TXJC_MGEN related configurations to default values.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95.
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
PRIVATE PMC_ERROR expand_ibuf_txjc_mgen_uncfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 chnl)
{
    PMC_ERROR result = PMC_SUCCESS;
    expand_ibuf_buffer_t expand_ibuf_buf[1];

    PMC_ENTRY();
    
    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);
    expand_ibuf_field_CHAN_MAP_TYPE_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0);
    expand_ibuf_field_M_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0);
    expand_ibuf_field_SERVER_SIZE_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0);
    expand_ibuf_buffer_flush(expand_ibuf_buf);      
    
    PMC_RETURN(result);
} /* expand_ibuf_txjc_mgen_uncfg */


/*******************************************************************************
* expand_ibuf_m_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the M value of the server container.
*   This function is only valid if mapping mode is GMP.  The function updates
*   the context.  The return value is expected to be immediately used to set the
*   M register bit field.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   client_type           - For MUX, LO ODU rate. See odukp_type_enum.
*                           Only valid for MUX.
*   server_type           - for MUX, HO ODU rate. See odukp_type_enum.
*                           for TGMP, ODUkp or ODUjp type see odukp_type_t,
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3
*                           types valid
*   num_tribslot          - number of tributary slots in a HO channel. Only valid
*                           for ODTU_MUX if client_type is set to ODUFLEX_CBR or 
*                           ODUFLEX_GFP.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32                - The value that is to be written into the M register 
*                           bit field.
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 expand_ibuf_m_calc(expand_ibuf_handle_t *expand_ibuf_handle,
                                    UINT32 chnl, UINT32 client_type,
                                    util_global_odukp_type_t server_type,
                                    UINT32 num_tribslot)
{
    
    
    /* variable declaration */
    UINT32 m_value;
    /* variable initialization */
    m_value = 0;


    PMC_ENTRY();

    if (expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_MUX)
    {

        switch (client_type)
        {
            case UTIL_GLOBAL_ODU0:
                m_value = 1;
                break;
            case UTIL_GLOBAL_ODU1:
                m_value = 2;  
                break;
            case UTIL_GLOBAL_ODU1E:
            case UTIL_GLOBAL_ODU1E_ETRANS:
            case UTIL_GLOBAL_ODU1F:
                m_value = 9;

                break;
            case UTIL_GLOBAL_ODU2F:
                m_value = 9;
                break;
            case UTIL_GLOBAL_ODU2:
                m_value = 8;
                break;
            case UTIL_GLOBAL_ODU2E:               
            case UTIL_GLOBAL_ODU2E_ETRANS:               
                if (server_type == UTIL_GLOBAL_ODU3)
                {
                    m_value = 9;
                }
                else {
                    m_value = 8;
                }
                break;
            case UTIL_GLOBAL_ODU3:
                if (server_type == UTIL_GLOBAL_ODU4)
                {
                    m_value = 31;
                }
                else {
                    m_value = 32;
                }
                break;
            case UTIL_GLOBAL_ODU3E2:
                if (server_type == UTIL_GLOBAL_ODU4)
                {
                    m_value = 33;
                }
                break;
            case UTIL_GLOBAL_ODU3E1:
                if (server_type == UTIL_GLOBAL_ODU4)
                {
                    m_value = 33;
                }
                break;                
            case UTIL_GLOBAL_ODUFLEX_CBR:
            case UTIL_GLOBAL_ODUFLEX_GFP:
                m_value = num_tribslot;
                break;
            default:
                PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                break;
        }
    }
    else if (expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_TGMP)
    {
        if(UTIL_GLOBAL_ODU0 == server_type)
        {
            m_value = 1;
        }
        else if(UTIL_GLOBAL_ODU1 == server_type)
        {
            m_value = 2;
        }
        else if(UTIL_GLOBAL_ODU2 == server_type 
                || UTIL_GLOBAL_ODU2E == server_type 
                || UTIL_GLOBAL_ODU2E_ETRANS == server_type 
                || UTIL_GLOBAL_ODU2F == server_type
                || UTIL_GLOBAL_ODU1E == server_type 
                || UTIL_GLOBAL_ODU1E_ETRANS == server_type 
                || UTIL_GLOBAL_ODU1F == server_type)
        {
            m_value = 8;
        }
        else if(UTIL_GLOBAL_ODU3 == server_type 
                || UTIL_GLOBAL_ODU3E1 == server_type
                || UTIL_GLOBAL_ODU3E2 == server_type)
        {
            m_value = 32;
        }
        else if(UTIL_GLOBAL_ODU4 == server_type)
        {
            m_value = 80;
        }
        else
        {
            /* This should never happen. */
            PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
        }
    }

    /* Update var context for a channel */
    expand_ibuf_handle->var.expand_ibuf_chnl_ctxt[chnl].M = m_value;

    PMC_RETURN(m_value);

} /* expand_ibuf_m_calc */


/*******************************************************************************
* expand_ibuf_odtu_cycle_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the number of 48 byte blocks needed for the LO
*   container and determines the number of data bytes in the last 48 byte cycle.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   num_tribslot          - number of tributary slots
*   server_size           - the size of the incoming channels container
*   mapping_mode          - mapping mode. See util_global_mapping_mode_t.
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
PRIVATE PMC_ERROR expand_ibuf_odtu_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 chnl,
                                             UINT32 num_tribslot,
                                             UINT32 server_size,
                                             util_global_mapping_mode_t mapping_mode)
{
    /* variable declaration */
    UINT32 odtu_cnt;
    UINT32 eomf_last_cycle_data;

    /* variable initialization */
    odtu_cnt = 0;
    eomf_last_cycle_data = 0;

    PMC_ENTRY();
    
    if(expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_TGMP)
    {
        PMC_LOG_TRACE("%s, %s, %d, server_size = %u\n", __FILE__, __FUNCTION__, __LINE__, server_size);
        PMC_LOG_TRACE("%s, %s, %d, num_tribslot = %u\n", __FILE__, __FUNCTION__, __LINE__, num_tribslot);

        num_tribslot = 1;
        
        odtu_cnt = ((server_size * num_tribslot) / 48);
        if((server_size * num_tribslot)% 48 > 0)
        {
            odtu_cnt = odtu_cnt + 1;  
        }
    
        PMC_LOG_TRACE("%s, %s, %d, odtu_cnt = %u\n", __FILE__, __FUNCTION__, __LINE__, odtu_cnt);
   
        expand_ibuf_field_ODTU_CNT_TOT_set(NULL, expand_ibuf_handle, chnl, odtu_cnt);
    }
    else {
        if (mapping_mode == UTIL_GLOBAL_AMP)
        {
            num_tribslot = 1;
        }
    }
    
    eomf_last_cycle_data = (server_size * num_tribslot) % 48;
    expand_ibuf_field_EOMF_LASTCYCLE_DATA_set(NULL, expand_ibuf_handle, chnl, eomf_last_cycle_data);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_odtu_cycle_cfg */

/*******************************************************************************
* expand_ibuf_hao_cfg_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes the hao config register to its default value.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
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
PRIVATE PMC_ERROR expand_ibuf_hao_cfg_uncfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                            UINT32 chnl)
{
    PMC_ERROR result = PMC_SUCCESS;
    expand_ibuf_buffer_t expand_ibuf_buf[1];

    PMC_ENTRY();    

    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);
    expand_ibuf_field_ODTU_CNT_TOT_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0);
    expand_ibuf_field_EOMF_LASTCYCLE_DATA_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0);
    expand_ibuf_field_NUM_TRIG_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0); 
    expand_ibuf_field_FIRST_CYCLE_set(expand_ibuf_buf, expand_ibuf_handle, chnl, 0);
    expand_ibuf_buffer_flush(expand_ibuf_buf);
   
    PMC_RETURN(result); 
    
} /* expand_ibuf_hao_cfg_uncfg */

/*******************************************************************************
* expand_ibuf_num_trig_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the number of 48 byte blocks are needed for the LO
*   container and determines how many bytes in the 48 byte transfer are data
*   bytes. It also configures the number of triggers.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   server_type           - for DEMUX, HO ODU rate. See odukp_type_enum. 
*                           for RGMP, ODUkp or ODUjp type see odukp_type_t,
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3
*                           types valid
*   client_type           - for DEMUX, LO ODU rate. See odukp_type_enum
*                           for RGMP, adaptaion_function. See
*                           adaptaion_function_enum for valid types  
*   mapping_mode          - mapping mode used in demuxing the server signal
*   num_tribslot          - number of tributary slots
*   ts_type               - tributary slot type. See odu_line_payload_t. \n
*                           NOTE: Only valid for DEMUX. This argument is
*                           ignored in RGMP.
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
PRIVATE PMC_ERROR expand_ibuf_num_trig_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 chnl,
                                           util_global_odukp_type_t server_type,   
                                           UINT32 client_type,
                                           util_global_mapping_mode_t mapping_mode,
                                           UINT32 num_tribslot,
                                           util_global_odu_line_payload_t ts_type)
{
    /* variable declaration */
    UINT32 num_trig;

    /* variable initialization */
    num_trig = 0;

    PMC_ENTRY();

    if(expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_TGMP)
    {
        if (mapping_mode == UTIL_GLOBAL_GMP)
        {
            num_trig = 0;
        }
        else if (mapping_mode == UTIL_GLOBAL_BMP 
                 || mapping_mode == UTIL_GLOBAL_AMP)
        {
            num_trig = 1;
        }
        else {
            PMC_RETURN(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else if (expand_ibuf_handle->cfg.instance == EXPAND_IBUF_INSTANCE_MUX)
    {
        if (mapping_mode == UTIL_GLOBAL_GMP)
        {
            num_trig = 0;
        }
        else if(mapping_mode == UTIL_GLOBAL_AMP)
        {
            if(ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                num_trig = num_tribslot - 1;
            }
            else if(UTIL_GLOBAL_ODU_TS_2G5 == ts_type)
            {
                num_trig = (num_tribslot/2) - 1;
            }
        }
    }

    expand_ibuf_field_NUM_TRIG_set(NULL, expand_ibuf_handle, chnl, num_trig);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_num_trig_cfg */

/*******************************************************************************
* expand_ibuf_cfc_xoff_level_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the CFC fill level where DATA_XOFF is gnerated for
*   the corresponding LO channel.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   trib_slot             - number of tributary slots
*                           Valid range: 0 - 80
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
PUBLIC PMC_ERROR expand_ibuf_cfc_xoff_level_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                                UINT32 chnl,
                                                UINT32 trib_slot)
{
    /* variable declaration */
    UINT32 cfc_xoff_level;

    /* variable initialization */
    cfc_xoff_level = 0;

    PMC_ENTRY();

    cfc_xoff_level = ((trib_slot * 2) + 2);
    
    expand_ibuf_cfc_fill_lvl_set(expand_ibuf_handle, chnl, cfc_xoff_level);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_cfc_xoff_level_cfg */

/*******************************************************************************
* expand_ibuf_mem_ho_id_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures channel ID of the current HO channel that is being
*   configured or reconfigured.
*
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl                   - channel ID to be configured
*                            Valid range: 0 - 95
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
PUBLIC PMC_ERROR expand_ibuf_mem_ho_id_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 chnl)
{
    expand_ibuf_buffer_t expand_ibuf_buf[1];
    PMC_ENTRY();

    expand_ibuf_buffer_init(expand_ibuf_buf, expand_ibuf_handle);
    expand_ibuf_field_MEM_LO_ID_set(expand_ibuf_buf, expand_ibuf_handle, 0x7F);
    expand_ibuf_field_MEM_HO_ID_set(expand_ibuf_buf, expand_ibuf_handle, chnl);
    expand_ibuf_buffer_flush(expand_ibuf_buf);

    PMC_RETURN(PMC_SUCCESS);
} /* expand_ibuf_mem_ho_id_cfg */


/*******************************************************************************
* expand_ibuf_cfc_fill_lvl_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set the CFC fill level where DATA_XOFF is gnerated for
*   the corresponding LO channel.
*
* INPUTS:
*   *expand_ibuf_handle   - pointer to EXPAND_IBUF handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   cfc_fill_lvl          - desired CFC Fill level
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
PUBLIC PMC_ERROR expand_ibuf_cfc_fill_lvl_set(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 chnl,
                                              UINT32 cfc_fill_lvl)
{


    PMC_ENTRY();


    expand_ibuf_field_CFC_FILL_LVL_set(NULL, expand_ibuf_handle, chnl, cfc_fill_lvl);

    PMC_RETURN(PMC_SUCCESS);

} /* expand_ibuf_cfc_fill_lvl_set */

/*******************************************************************************
* expand_ibuf_ts_type_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function recovers time slot type 
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   cycle                  - calendar cycle to be configured
*                            Valid range: 0 - 95
*   chnl                   - HO channel ID which this calendar cycle belongs to
*                            if no HO channel exists it must be set to 127
*                            Valid range: 0 - 95
*   
* OUTPUTS:
*   *ts_type               - tributary slot type.                
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void expand_ibuf_ts_type_get(expand_ibuf_handle_t *expand_ibuf_handle, 
                                    UINT32 chnl,
                                    UINT32 cycle, 
                                    util_global_odu_line_payload_t *ts_type)
{
    UINT32 ho_cal_cycle_tot, p_serv;
    PMC_ENTRY();
    
    ho_cal_cycle_tot  = expand_ibuf_field_HO_CAL_CYCLE_TOT_get(NULL, expand_ibuf_handle, cycle);
    p_serv            = expand_ibuf_field_SERVER_SIZE_get(NULL, expand_ibuf_handle, chnl);
        
    ho_cal_cycle_tot = ho_cal_cycle_tot * 48;
    if (ho_cal_cycle_tot  > 15000)
    {
        *ts_type = UTIL_GLOBAL_ODU_TS_1G25;
    } else
    {
        *ts_type = UTIL_GLOBAL_ODU_TS_2G5;
    }

    
    PMC_RETURN();
} /* expand_ibuf_ts_type_get */

/*******************************************************************************
* expand_ibuf_param_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function recovers expand_ibuf parameters
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl                   - channel ID to be configured
*                            Valid range: 0 - 95
*   ho_num_tribslot        - number of tributary slots occupied by the HO ODU.
*   num_tribslot           - number of tributary slots occupied by the LO ODU.
*   mapping_mode           - Enum for the mapping mode. See util_global_mapping_mode_t.                       
*   ts_type                - tributary slot type.                
*   cn_n                   - golden Cn numerator
*   cn_d                   - golden Cn denominator
*   golden_cn              - golden Cn value
*   pjo_cnt                - maximum possible PJOs
*   njo_cnt                - maximum possible NJOs
*
* OUTPUTS:
*   *server_type           - HO ODU rate. See util_global_odukp_type_t.
*   *client_type           - LO ODU rate for ODTU_DMX. See util_global_odukp_type_t.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void expand_ibuf_param_cfg_get(expand_ibuf_handle_t *expand_ibuf_handle, 
                                      UINT32 chnl, 
                                      UINT32 ho_num_tribslot,
                                      UINT32 num_tribslot,
                                      util_global_mapping_mode_t mapping_mode,
                                      util_global_odu_line_payload_t ts_type,
                                      UINT32 cn_n, 
                                      UINT32 cn_d,
                                      UINT32 golden_cn, 
                                      UINT32 pjo_cnt, 
                                      UINT32 njo_cnt,
                                      util_global_odukp_type_t *server_type,
                                      util_global_odukp_type_t *client_type)
{
    UINT32 m; 
    UINT32 p_serv;
    util_global_odukp_type_t client_types[5] = {UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK};
    util_global_odukp_type_t server_types[5] = {UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK,UTIL_GLOBAL_LAST_ODUK};
    UINT32 num_server_rates = 0, num_client_rates = 0;
    PMC_ENTRY();
    
    /* get parameters */
    p_serv  = expand_ibuf_field_SERVER_SIZE_get(NULL, expand_ibuf_handle, chnl);
    
    if (UTIL_GLOBAL_AMP == mapping_mode)
    {                
        if (1 == pjo_cnt && 1 == njo_cnt)
        {
            *server_type = UTIL_GLOBAL_ODU1;
            *client_type = UTIL_GLOBAL_ODU0;
        } else if (3 == pjo_cnt && 2 == njo_cnt)
        {
            *server_type = UTIL_GLOBAL_ODU3E1;
            *client_type = UTIL_GLOBAL_ODU2E;
        } else if (2 == pjo_cnt && 1 == njo_cnt)
        {
            if (p_serv >= 60928)
            {
                *server_type = UTIL_GLOBAL_ODU3;
                *client_type = UTIL_GLOBAL_ODU2;
            } else if ( p_serv % 15232 == 0)
            {
                *server_type = UTIL_GLOBAL_ODU2;
                *client_type = UTIL_GLOBAL_ODU1;
            } else 
            {
                *server_type = UTIL_GLOBAL_ODU3;
                *client_type = UTIL_GLOBAL_ODU1;
            }
        }
        else
        {
            PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
        }

    } 
    else if (UTIL_GLOBAL_GMP == mapping_mode)
    {        
        m  = expand_ibuf_field_M_get(NULL, expand_ibuf_handle, chnl);
        
        /* ODU4 */
        if (15200 == p_serv)
        {
            *server_type = UTIL_GLOBAL_ODU4;
            num_server_rates = 1;
            server_types[0] = UTIL_GLOBAL_ODU4;
            client_types[0] = UTIL_GLOBAL_ODUFLEX_GFP;            
            if (1 == m)
            {
                client_types[1] = UTIL_GLOBAL_ODU0;
                num_client_rates = 2;
            } else if (2 == m)
            {
                client_types[1] = UTIL_GLOBAL_ODU1;
                num_client_rates = 2;
            } else if (8 == m)
            {
                client_types[1] = UTIL_GLOBAL_ODU2;
                client_types[2] = UTIL_GLOBAL_ODU2E;
                num_client_rates = 3;
            } else if (9 == m)
            {    
                client_types[1] = UTIL_GLOBAL_ODU1F;
                client_types[2] = UTIL_GLOBAL_ODU1E;             
                client_types[3] = UTIL_GLOBAL_ODU2F;
                num_client_rates = 4;
            } else if (31 == m)
            {
                client_types[1] = UTIL_GLOBAL_ODU3;
                num_client_rates = 5;
            } else if (33 == m)
            { 
                client_types[1] = UTIL_GLOBAL_ODU3E2;
                client_types[2] = UTIL_GLOBAL_ODU3E1;
                num_client_rates = 3;
            }         
                         
            if (FALSE == expand_ibuf_find_rates(server_types,client_types,num_server_rates,num_client_rates,ts_type,num_tribslot,
                                               golden_cn,cn_n,cn_d,server_type,client_type))
            {
                *client_type = UTIL_GLOBAL_ODUFLEX_CBR;
            }
        } else
        {
            /* set possible client rates */
            client_types[0] = UTIL_GLOBAL_ODUFLEX_GFP;  
            if (m == 1)
            {
                client_types[1] = UTIL_GLOBAL_ODU0;    
                num_client_rates = 2;        
            } else if (m == 2)
            {
                client_types[1] = UTIL_GLOBAL_ODU1;    
                num_client_rates = 2;        
            } else if (m == 8)
            {
                client_types[1] = UTIL_GLOBAL_ODU2;    
                client_types[2] = UTIL_GLOBAL_ODU2E;    
                num_client_rates = 3;        
            } else if (m == 9)
            {
                client_types[1] = UTIL_GLOBAL_ODU1E;    
                client_types[2] = UTIL_GLOBAL_ODU1F;    
                client_types[3] = UTIL_GLOBAL_ODU2F;    
                client_types[4] = UTIL_GLOBAL_ODU2E;    
                num_client_rates = 5;        
            } else if (m==32)
            {
                client_types[1] = UTIL_GLOBAL_ODU3; 
                num_client_rates = 2;   
            }          

            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                switch(ho_num_tribslot)
                {
                case 2:
                    server_types[0] = UTIL_GLOBAL_ODU1; 
                    num_server_rates = 1;
                    break;
                case 8:
                    server_types[0] = UTIL_GLOBAL_ODU2;
                    server_types[1] = UTIL_GLOBAL_ODU2E;
                    server_types[2] = UTIL_GLOBAL_ODU2F;
                    server_types[3] = UTIL_GLOBAL_ODU1E;
                    server_types[4] = UTIL_GLOBAL_ODU1F;
                    num_server_rates = 5;
                    break;
                case 32:
                    server_types[0] = UTIL_GLOBAL_ODU3; 
                    server_types[1] = UTIL_GLOBAL_ODU3E2;
                    server_types[2] = UTIL_GLOBAL_ODU3E1;
                    num_server_rates = 3;
                    break;
                default:
                    PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                    break;
                }
            } else 
            {
                switch(ho_num_tribslot)
                {
                case 1:
                    server_types[0] = UTIL_GLOBAL_ODU1; 
                    num_server_rates = 1;
                    break;
                case 4:
                    server_types[0] = UTIL_GLOBAL_ODU2;
                    server_types[1] = UTIL_GLOBAL_ODU2E;
                    server_types[2] = UTIL_GLOBAL_ODU2F; 
                    server_types[3] = UTIL_GLOBAL_ODU1E; 
                    server_types[4] = UTIL_GLOBAL_ODU1F;
                    num_server_rates = 5;
                    break;
                case 16:
                    server_types[0] = UTIL_GLOBAL_ODU3; 
                    server_types[1] = UTIL_GLOBAL_ODU3E2;
                    server_types[2] = UTIL_GLOBAL_ODU3E1;
                    num_server_rates = 3;                    

                    break;
                default:
                    PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
                    break;
                }
            }
            /* set default values */
            *server_type = server_types[0];
            *client_type = client_types[1];
            if (FALSE == expand_ibuf_find_rates(server_types,client_types,num_server_rates,num_client_rates,ts_type,num_tribslot,
                                               golden_cn,cn_n,cn_d,server_type,client_type))
            {
                *client_type = UTIL_GLOBAL_ODUFLEX_CBR;
            }
        }
    }
    else
    {
        PMC_ASSERT(FALSE, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0); 
    }
           
    PMC_RETURN();
    
} /* expand_ibuf_param_cfg_get */


/*
** Private functions
*/ 

/*******************************************************************************
* expand_ibuf_find_rates
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function find server and client rate pair based on the Cn value, 
*   the Cn numerator and the Cn denominator.
*
* INPUTS:
*   server_types          - array of HO ODU rate 
*   client_types          - array of LO ODU rate
*   num_server_rate       - number of candidates rates
*   num_client_rate       - number of candidates rates
*   ts_type               - tributary slot type. See util_global_odu_line_payload_t.
*   num_tribslot          - number of tributary slots occupied by LO ODU 
*   golden_cn             - the cn value
*   cn_n                  - the cn numerator
*   cn_d                  - the cn denominator
*
* OUTPUTS:
*   *server_type          - selected server type
*   *client_type          - selected client type
*
* RETURNS:
*   BOOL8                 - TRUE:  when there is a valid selection \n
*                           FALSE: when there is no valid selection
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 expand_ibuf_find_rates(util_global_odukp_type_t server_types[],
                                     util_global_odukp_type_t client_types[],                                     
                                     UINT32 num_server_rate,
                                     UINT32 num_client_rate,
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_tribslot,
                                     UINT32 golden_cn, 
                                     UINT32 cn_n, 
                                     UINT32 cn_d,
                                     util_global_odukp_type_t *server_type,
                                     util_global_odukp_type_t *client_type)
{
    BOOL8 match = FALSE;
    UINT32 client_rate_itr, server_rate_itr;
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 try_golden_cn, try_cn_n, try_cn_d;

    PMC_ENTRY();

    for(server_rate_itr = 0;server_rate_itr < num_server_rate && match == FALSE && rc == PMC_SUCCESS;server_rate_itr ++)
    {
        for(client_rate_itr = 0;client_rate_itr < num_client_rate && match == FALSE && rc == PMC_SUCCESS;client_rate_itr ++)
        {
            rc = util_global_cn_calc(UTIL_GLOBAL_MUX_DEMUX,
                                     server_types[server_rate_itr], (UINT32) client_types[client_rate_itr], 0, 
                                     ts_type, num_tribslot, &try_golden_cn, &try_cn_n, &try_cn_d); 
            
            if (PMC_SUCCESS == rc && 
                try_golden_cn == golden_cn && 
                try_cn_n == cn_n &&
                try_cn_d == cn_d)
            {
                match = TRUE;
                *client_type = client_types[client_rate_itr];
                *server_type = server_types[server_rate_itr];
            }        
        }
    }
    PMC_RETURN(match);
} /*expand_ibuf_find_rates*/

/*******************************************************************************
* expand_ibuf_int_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   *expand_ibuf_handle    - pointer to EXPAND_IBUF handle instance
*   chnl_id                - channel ID to be configured
*                            Valid range: 0 - 95
*   non_null_ptr           - Pointer to check that shouldn't be NULL
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
PRIVATE PMC_ERROR expand_ibuf_int_validate(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 chnl_id,
                                           void  *non_null_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != expand_ibuf_handle, EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (chnl_id >= expand_ibuf_handle->cfg.cfc_cfg.max_fifo_num)
    {
        result = EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS;
    }

    if (PMC_SUCCESS == result && NULL == non_null_ptr)
    {
        result = EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* expand_ibuf_int_validate */

/*
** End of file
*/
