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

#include "cpact_obuf_loc.h"
#include "cpact_obuf_io.h"
#include "cpact_obuf_io_inline.h"



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
const char CPACT_OBUF_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    CPACT_OBUF_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char CPACT_OBUF_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward references
*/                                                    
                                                      
/*
** Public Functions
*/

/*******************************************************************************
* cpact_obuf_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an CPACT_OBUF block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the CPACT_OBUF subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique identifier string for the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   cpact_obuf_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC cpact_obuf_handle_t *cpact_obuf_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    cpact_obuf_handle_t *cpact_obuf_handle;

    PMC_ENTRY();
    
    cpact_obuf_handle = (cpact_obuf_handle_t*)PMC_CTXT_CALLOC(sizeof(cpact_obuf_handle_t), parent);
    
    pmc_handle_init(parent, cpact_obuf_handle, sys_handle, PMC_MID_DIGI_CPACT_OBUF, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(CPACT_OBUF_LOG_ERR_STRINGS[0], CPACT_OBUF_LOG_ERR_TABLE_BASE, CPACT_OBUF_LOG_ERR_COUNT);
    
    PMC_RETURN(cpact_obuf_handle);
    
} /* cpact_obuf_ctxt_create */

 
/*******************************************************************************
* cpact_obuf_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a CPACT_OBUF block instance.
*
*
* INPUTS:
*   *cpact_obuf_handle         - pointer to CPACT_OBUF handle instance
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
PUBLIC void cpact_obuf_ctxt_destroy(cpact_obuf_handle_t *cpact_obuf_handle)
{
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&cpact_obuf_handle, cpact_obuf_handle);
    
    PMC_RETURN();
} /* cpact_obuf_ctxt_destroy */


/*******************************************************************************
* cpact_obuf_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an CPACT_OBUF block handle instance.
*
*
* INPUTS:
*   *cpact_obuf_handle          - pointer to CPACT_OBUF handle instance
*   instance                    - Enum. See cpact_obuf_instance_type_t.
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
PUBLIC void cpact_obuf_handle_init(cpact_obuf_handle_t *cpact_obuf_handle,
                                   cpact_obuf_instance_type_t instance)
{
    /* variable declaration */
    PMC_ENTRY();
    
    cpact_obuf_handle->cfg.instance = instance;
    
    PMC_RETURN(); 
} /* cpact_obuf_handle_init */


/*******************************************************************************
* cpact_obuf_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on CPACT_OBUF.
*
* INPUTS:
*   *cpact_obuf_handle          - pointer to CPACT_OBUF handle instance
*   *restart_init_cfg_ptr       - pointer to restart init config
*   top_energy_state_reg        - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.\n
*     WARM : register/context are coherent\n
*     CRASH: everything is coherent (a cleanup may has been executed)
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cpact_obuf_handle_restart_init(cpact_obuf_handle_t *cpact_obuf_handle,
                                                util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                                pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(CPACT_OBUF_LOG_ERR_STRINGS[0], CPACT_OBUF_LOG_ERR_TABLE_BASE, CPACT_OBUF_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);

} /* cpact_obuf_handle_restart_init */


/*******************************************************************************
* cpact_obuf_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
* INPUTS:
*   *cpact_obuf_handle         - pointer to CPACT_OBUF handle instance
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
PUBLIC UINT32 cpact_obuf_ram_init(cpact_obuf_handle_t *cpact_obuf_handle)
{
    UINT32 chnl_itr;

    PMC_ENTRY();

    for (chnl_itr = 0;chnl_itr < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; chnl_itr++) 
    {
        cpact_obuf_reg_CPACT_CHNL_CFG_array_write(NULL, cpact_obuf_handle, chnl_itr, 0x00000000); 
        cpact_obuf_reg_SOMF_CFG_array_write(NULL, cpact_obuf_handle, chnl_itr, 0x00000000);       
        cpact_obuf_reg_EOMF_CFG_array_write(NULL, cpact_obuf_handle, chnl_itr, 0x00000000);      
    }
        
    PMC_RETURN(PMC_SUCCESS);

} /* cpact_obuf_ram_init */


/*******************************************************************************
* cpact_obuf_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures cpact_obuf block when adding a client channel to
*   RGMP or when adding a LO ODU channel into an active HO channel in case of 
*   multiplexing. 
*
* INPUTS:
*   *cpact_obuf_handle      - pointer to CPACT_OBUF handle instance
*   chnl_id                 - channel ID to be configured
*                             Valid range: 0 - 95
*   server_type             - OPUk rate if used by RGMP or
*                             HO ODUk rate if ODTU_DMX\n
*                             Refer to util_global_odukp_type_t. 
*   client_type             - LO ODU rate if ODTU_DMX. See odukp_type enum for 
*                             valid rates.\n 
*                             NOTE: This argument is only used for DMX. Set 
*                             to 0 if used in RGMP.                    
*   mapping_mode            - mapping procedure
*                             See util_global_mapping_mode_t.\n
*                             BMP     = 0,\n
*                             AMP     = 1,\n
*                             GMP     = 2,\n  
*   num_tribslot            - number of tributary slots occupied by client LO 
*                             channel. This argument is only used for DMX. Set 
*                             to 1 if used in RGMP. 
*   ts_type                 - tributary slot size. See odu_line_payload_t. 
*                             This argument is only used for DMX. Set 
*                             to 0 if used in RGMP.
*   offline_update          - Used for MUX only.\n
*                             0 -- normal configuration\n
*                             1 -- offline page configuration\n 
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
PUBLIC PMC_ERROR cpact_obuf_chnl_cfg(cpact_obuf_handle_t *cpact_obuf_handle, 
                                     UINT32 chnl_id, util_global_odukp_type_t server_type,
                                     UINT32 client_type,
                                     util_global_mapping_mode_t mapping_mode,
                                     UINT32 num_tribslot, 
                                     util_global_odu_line_payload_t ts_type, 
                                     BOOL8 offline_update)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 mapping = 0;
    UINT32 map_type = 0;
    
    PMC_ENTRY();
    
    /* argument checking */
    if ((chnl_id > 95) || (server_type > UTIL_GLOBAL_ODUK_DONT_CARE) 
        || (client_type > UTIL_GLOBAL_ODUK_DONT_CARE)
        || (mapping_mode > UTIL_GLOBAL_NO_MAP) || (num_tribslot > 80) 
        || (ts_type > UTIL_GLOBAL_ODU_TS_2G5))
    {
        PMC_LOG_TRACE("chnl_id = %u, server_type = %u, client_type = %u, mapping_mode = %u, num_tribslot = %u, ts_type = %u...\n", chnl_id, server_type, client_type, mapping_mode, num_tribslot, ts_type);
         
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
 
    if (mapping_mode == UTIL_GLOBAL_BMP)                             
    {                                                                    
        /* BMP mapping */
        ret_val = cpact_obuf_chnl_mapping_cfg(cpact_obuf_handle, chnl_id, 0);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        
        ret_val = cpact_obuf_end_of_multi_frame_cfg(cpact_obuf_handle, chnl_id, server_type, (util_global_odukp_type_t)client_type, 0, num_tribslot);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    }
    else if (mapping_mode == UTIL_GLOBAL_AMP)           
    {
        if (cpact_obuf_handle->cfg.instance == CPACT_OBUF_INSTANCE_DEMUX)
        {
            switch (ts_type)
            {
                case UTIL_GLOBAL_ODU_UNCHANNELIZED:
                case UTIL_GLOBAL_ODU_TS_1G25:
                    mapping = 3;
                break;
                case UTIL_GLOBAL_ODU_TS_2G5:
                    mapping = 4;
                break;
                default:
                    PMC_RETURN(CPACT_OBUF_ERR_INVALID_ARG);
            }
            
            if (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1)
            {
                    map_type = 2;
            }
            else if (server_type == UTIL_GLOBAL_ODU3E1 && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
            {
                    map_type = 3;
            }     
            else {
                    map_type = 1;
            }
            
            if (offline_update == FALSE)
            {    
                ret_val = cpact_obuf_chnl_mapping_cfg(cpact_obuf_handle, chnl_id, map_type); 
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
        }
        else {
            ret_val = cpact_obuf_chnl_mapping_cfg(cpact_obuf_handle, chnl_id, 1); 
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
                
            mapping = 1;
        }       
                
        ret_val = cpact_obuf_end_of_multi_frame_cfg(cpact_obuf_handle, chnl_id, 
                                                    server_type, (util_global_odukp_type_t)client_type, 
                                                    mapping, num_tribslot);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        } 
    }          
    else if (mapping_mode == UTIL_GLOBAL_GMP)  
    {
        /* GMP mapping */     
        if (offline_update == FALSE)
        {
            ret_val = cpact_obuf_chnl_mapping_cfg(cpact_obuf_handle, chnl_id, 4);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            } 
        }
        
        if (cpact_obuf_handle->cfg.instance == CPACT_OBUF_INSTANCE_DEMUX)
        {
            switch (ts_type)
            {
                case UTIL_GLOBAL_ODU_UNCHANNELIZED:
                case UTIL_GLOBAL_ODU_TS_1G25:
                    mapping = 5;
                break;
                case UTIL_GLOBAL_ODU_TS_2G5:
                    mapping = 6;
                break;
                default:
                    PMC_RETURN(CPACT_OBUF_ERR_INVALID_ARG);
            }
        }
        else {
            mapping = 2;
        }       
        
        ret_val = cpact_obuf_end_of_multi_frame_cfg(cpact_obuf_handle, chnl_id, server_type, (util_global_odukp_type_t)client_type, mapping, num_tribslot);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    } 
    else                     
    {                      
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);           
    }   

    PMC_RETURN(PMC_SUCCESS);
    
} /* cpact_obuf_chnl_cfg */                                          

/*
** Private Functions
*/

/*******************************************************************************
* cpact_obuf_chnl_mapping_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the mapping mode of a channel in the content 
*   memory. 
*
*
* INPUTS:
*   *cpact_obuf_handle    - pointer to CPACT_OBUF handle instance
*   chnl_id               - channel ID to be configured
*                           Valid range: 0 - 95
*   mapping_mode          - the mapping mode of the channel \n
*                           0 -- BMP \n
*                           1 -- AMP \n
*                           2 -- ODTU13 \n
*                           3 -- ODTU2e3e1 \n
*                           4 -- GMP \n
*                           5 -- RESERVED \n
*                           6 -- Special Mode \n
*                           7 -- Special Mode 
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
PUBLIC PMC_ERROR cpact_obuf_chnl_mapping_cfg(cpact_obuf_handle_t *cpact_obuf_handle, 
                                             UINT32 chnl_id,
                                             UINT32 mapping_mode)
{
    /* variable declaration */
    
    PMC_ENTRY();
       
    cpact_obuf_field_CH_MAP_TYPE_set(NULL, cpact_obuf_handle, chnl_id, mapping_mode);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* cpact_obuf_chnl_mapping_cfg */

/*******************************************************************************
* cpact_obuf_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function deprovisions a LO channel in CPACT_OBUF.
*
* INPUTS:
*   *cpact_obuf_handle    - pointer to CPACT_OBUF handle instance   
*   chnl_id               - channel ID to be configured
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
PUBLIC PMC_ERROR cpact_obuf_lo_chnl_deprov(cpact_obuf_handle_t *cpact_obuf_handle, 
                                           UINT32 chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();  
    
    cpact_obuf_field_EOMF_LAST_DT_set(NULL, cpact_obuf_handle, chnl_id, 0);
    cpact_obuf_field_EOMF_CYCLE_set(NULL, cpact_obuf_handle, chnl_id, 0);
    
    cpact_obuf_field_CH_MAP_TYPE_set(NULL, cpact_obuf_handle, chnl_id, 0);

    
    PMC_RETURN(result);
} /* cpact_obuf_lo_chnl_deprov */


/*******************************************************************************
* cpact_obuf_end_of_multi_frame_cfg
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function configures the size of an ODTU/OPUk container excluding the 
*   Fixed Stuff (FS) columns in the content memory.  
*
*   When Hitless Adjustment of ODUFlex (HAO) is enabled or LO reconfiguration
*   is required, call this function twice for dual context handling.
*   Normal context value is programmed using channel ID 0 to 95 and the second
*   context value is programmed using channel ID 96 to 191.
*
*
* INPUTS:
*   *cpact_obuf_handle    - pointer to CPACT_OBUF handle instance   
*   chnl_id               - channel ID to be configured
*                           Valid range: 0 - 95
*   server_type           - OPUk rate if used by TGMP/RGMP or
*                           HO ODUk rate if ODTU_MUX/DMX. \n
*                           See util_global_odukp_type_t                                                      
*   client_type           - LO ODUj rate.
*                           Only used if ODTU_MUX/DMX is being used. \n
*                           See util_global_odukp_type_t.                          
*   mapping_type          - mapping procedure \n
*                           0 -- BMP \n
*                           1 -- AMP \n
*                           2 -- GMP \n
*                           3 -- AMP 1G25 \n
*                           4 -- AMP 2G5 \n
*                           5 -- GMP 1G25 \n
*                           6 -- GMP 2G5 
*   trib_slot             - number of tributary slots in the HO channel if the 
*                           client_type is ODUFlex. Otherwise, it is a DON't 
*                           CARE. 
*                           Valid range: 1 - 80
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
PUBLIC PMC_ERROR cpact_obuf_end_of_multi_frame_cfg(cpact_obuf_handle_t *cpact_obuf_handle, 
                                                   UINT32 chnl_id,
                                                   util_global_odukp_type_t server_type,
                                                   util_global_odukp_type_t client_type,
                                                   UINT32 mapping_type, 
                                                   UINT32 trib_slot)
{
    /* variable declaration */
    UINT32 last_data_unit;
    UINT32 cycle;
    UINT32 payload_size;
    cpact_obuf_buffer_t cpact_buf[1];
    
    /* variable initialization */
    last_data_unit = 0;
    cycle = 0;
    payload_size = 0;
    
    PMC_ENTRY();
    
    switch (mapping_type)
    {
        case 0: /* BMP */
            if (server_type == UTIL_GLOBAL_ODU2 || server_type == UTIL_GLOBAL_ODU2E 
                || server_type == UTIL_GLOBAL_ODU2E_ETRANS || server_type == UTIL_GLOBAL_ODU2F) 
            {
                last_data_unit = 48;
                cycle = 315; 
            }
            else if (server_type == UTIL_GLOBAL_ODU3) 
            {
                last_data_unit = 32;
                cycle = 314; 
            }
            else 
            {
                last_data_unit = 16;
                cycle = 317;
            }
            break;
        case 1: /* AMP */ 
            if (server_type == UTIL_GLOBAL_ODU2) 
            {
                last_data_unit = 48;
                cycle = 315; 
            }
            else if (server_type == UTIL_GLOBAL_ODU3) 
            {
                last_data_unit = 32;
                cycle = 314; 
            }
            break;
         case 2: /* GMP */ 

            if (server_type == UTIL_GLOBAL_ODU4) 
            {
                last_data_unit = 32;
                cycle = 316; 
            }
            else 
            {
                last_data_unit = 16;
                cycle = 317;
            }   
            break;
         case 3: /* AMP 1.25G */    
            if (server_type == UTIL_GLOBAL_ODU1 && client_type == UTIL_GLOBAL_ODU0) 
            {
                last_data_unit = 16;
                cycle = 317; 
            }
            else if( (server_type == UTIL_GLOBAL_ODU2 
                      || server_type == UTIL_GLOBAL_ODU3)  
                     && client_type == UTIL_GLOBAL_ODU1)
            {
                last_data_unit = 32;
                cycle = 634;
            } 
            else if(server_type == UTIL_GLOBAL_ODU3 
                    && client_type == UTIL_GLOBAL_ODU2)
            {
                last_data_unit = 32;
                cycle = 2538;
            }    
            break;  
         case 4: /* AMP 2.5G */         
            if ((server_type == UTIL_GLOBAL_ODU2 || server_type == UTIL_GLOBAL_ODU3) 
                && client_type == UTIL_GLOBAL_ODU1)
            {
                last_data_unit = 16;
                cycle = 317; 
            }
            else if ((server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU2) 
                      ||(server_type == UTIL_GLOBAL_ODU3E1 && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)))
            { 
                last_data_unit = 16;
                cycle = 1269;
            }
            break;
        case 5: /* GMP 1.25G */
            if (server_type == UTIL_GLOBAL_ODU4 
                && (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                    || client_type == UTIL_GLOBAL_ODUFLEX_GFP)) 
            {
                payload_size = 15200 * trib_slot;
                last_data_unit = payload_size % 48;
                /*cycle = (CEILING(payload_size / 48)) -1;*/
                
                if (last_data_unit == 0)
                {
                    cycle = (payload_size / 48) - 1;
                }
                else {
                    cycle = (payload_size / 48);
                } 
            }
            else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                     || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
            {
                payload_size = 15232 * trib_slot;
                last_data_unit = payload_size % 48;
                
                if (last_data_unit == 0)
                {
                    cycle = (payload_size / 48) - 1;
                }
                else {
                    cycle = (payload_size / 48);
                } 
            }
            else if ((server_type == UTIL_GLOBAL_ODU2 
                      || server_type == UTIL_GLOBAL_ODU3 
                      || server_type == UTIL_GLOBAL_ODU3E2) 
                     && client_type == UTIL_GLOBAL_ODU0)
            {
                last_data_unit = 16;
                cycle = 317; 
            }  
            else if ((server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU2) || 
                     (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU2E) || 
                     (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU2E_ETRANS) )
            {
                last_data_unit = 32;
                cycle = 2538; 
            }  
            else if ((server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1E) || 
                     (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1E_ETRANS) || 
                     (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1F) ||
                     (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU2E) ||
                     (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU2E_ETRANS) ||
                     (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU2F) ||
                     (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU2F) ||
                     (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU1F) ||
                     (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU1E))
            {
                last_data_unit = 48;
                cycle = 2855; 
            }  
            else if (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU1)
            {
                last_data_unit = 32;
                cycle = 634; 
            }           
            else if (server_type == UTIL_GLOBAL_ODU3E2 && client_type == UTIL_GLOBAL_ODU3)
            {
                last_data_unit = 32;
                cycle = 10154; 
            } 
            else if (server_type == UTIL_GLOBAL_ODU4)
            {
                if (client_type == UTIL_GLOBAL_ODU0)
                {
                    last_data_unit = 32;
                    cycle = 316;
                }
                else if (client_type == UTIL_GLOBAL_ODU1)
                {
                    last_data_unit = 16;
                    cycle = 633;
                }
                else if (client_type == UTIL_GLOBAL_ODU2  
                        || client_type == UTIL_GLOBAL_ODU2E 
                        || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
                {
                    last_data_unit = 16;
                    cycle = 2533;
                }
                else if(client_type == UTIL_GLOBAL_ODU1F 
                        || client_type == UTIL_GLOBAL_ODU2F)
                {
                    last_data_unit = 48;
                    cycle = 2849;
                }
                else if (client_type == UTIL_GLOBAL_ODU3) 
                {
                    last_data_unit = 32;
                    cycle = 9816;
                }
                else if (client_type == UTIL_GLOBAL_ODU3E1 
                         || client_type == UTIL_GLOBAL_ODU3E2) 
                {
                    last_data_unit = 48;
                    cycle = 10449;
                }
            }     
            break;
        
        case 6: /* GMP 2.5G */
            last_data_unit = 16;
            cycle = 1269;
            break;     
        default:
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }
     
    cpact_obuf_buffer_init(cpact_buf, cpact_obuf_handle);
    cpact_obuf_field_EOMF_LAST_DT_set(cpact_buf, cpact_obuf_handle, chnl_id, last_data_unit);
    cpact_obuf_field_EOMF_CYCLE_set(cpact_buf, cpact_obuf_handle, chnl_id, cycle);
    cpact_obuf_buffer_flush(cpact_buf);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* cpact_obuf_end_of_multi_frame_cfg */



/*
** End of file
*/
