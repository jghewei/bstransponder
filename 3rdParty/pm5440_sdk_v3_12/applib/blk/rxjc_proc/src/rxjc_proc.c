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

#include "rxjc_proc_loc.h"
                                  

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
const char RXJC_PROC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    RXJC_PROC_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char RXJC_PROC_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References 
*/                           
                                       
/*
** Public Functions
*/

/*******************************************************************************
* rxjc_proc_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a RXJC_PROC block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the RXJC_PROC subsystem relative to 
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   rxjc_proc_handle      - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC rxjc_proc_handle_t *rxjc_proc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    rxjc_proc_handle_t *rxjc_proc_handle;

    PMC_ENTRY();

    rxjc_proc_handle = (rxjc_proc_handle_t*)PMC_CTXT_CALLOC(sizeof(rxjc_proc_handle_t), parent);
    
    pmc_handle_init(parent, rxjc_proc_handle, sys_handle, PMC_MID_DIGI_RXJC_PROC, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(RXJC_PROC_LOG_ERR_STRINGS[0], RXJC_PROC_LOG_ERR_TABLE_BASE, RXJC_PROC_LOG_ERR_COUNT);
    
    
    PMC_RETURN(rxjc_proc_handle);
    
} /* rxjc_proc_ctxt_create */

 
/*******************************************************************************
* rxjc_proc_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a RXJC_PROC block instance.
*
*
* INPUTS:
*   *rxjc_proc_handle         - pointer to RXJC_PROC handle instance
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
PUBLIC void rxjc_proc_ctxt_destroy(rxjc_proc_handle_t *rxjc_proc_handle)
{
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&rxjc_proc_handle, rxjc_proc_handle);   
    PMC_RETURN();
    
} /* rxjc_proc_ctxt_destroy */

/*******************************************************************************
* rxjc_proc_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an RXJC_PROC block handle instance.
*
* INPUTS:
*   *rxjc_proc_handle       - pointer to RXJC_PROC handle instance
*   rxjc_proc_instance      - Enum for RXJC_PROC instance. See rxjc_proc_instance_type_t. 
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
PUBLIC void rxjc_proc_handle_init(rxjc_proc_handle_t *rxjc_proc_handle,
                                  rxjc_proc_instance_type_t rxjc_proc_instance)
{
    /* variable declaration */
    
    PMC_ENTRY();
     
    /* configure the parent that is using this block */
    rxjc_proc_handle->cfg.instance = rxjc_proc_instance; 
    
    PMC_RETURN();
} /* rxjc_proc_handle_init */

/*******************************************************************************
* rxjc_proc_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on rxjc_proc.
*
*
* INPUTS:
*   *rxjc_proc_handle     - pointer to RXJC_PROC handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.\n
*     WARM : register/context are coherent\n
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR rxjc_proc_handle_restart_init(rxjc_proc_handle_t *rxjc_proc_handle,
                                               util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                               pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(RXJC_PROC_LOG_ERR_STRINGS[0], RXJC_PROC_LOG_ERR_TABLE_BASE, RXJC_PROC_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* rxjc_proc_handle_restart_init */


/*******************************************************************************
* rxjc_proc_ram_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
* INPUTS:
*   *rxjc_proc_handle         - pointer to RXJC_PROC handle instance
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
PUBLIC UINT32 rxjc_proc_ram_init(rxjc_proc_handle_t *rxjc_proc_handle)
{
    UINT32 chnl_itr;

    PMC_ENTRY();

    for (chnl_itr = 0;chnl_itr < UTIL_GLOBAL_MAX_NUM_ODU_CHNL; chnl_itr++) 
    {
        rxjc_proc_reg_COMMON_CFG_1_array_write(NULL, rxjc_proc_handle, chnl_itr, 0x00000000);  
        rxjc_proc_reg_COMMON_CFG_2_array_write(NULL, rxjc_proc_handle, chnl_itr, 0x00000000); 
        rxjc_proc_reg_COMMON_CFG_3_array_write(NULL, rxjc_proc_handle, chnl_itr, 0x00000000); 
    }

    PMC_RETURN(PMC_SUCCESS);

} /* rxjc_proc_ram_init */


/*******************************************************************************
* rxjc_proc_amp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures Negative justification (NJO) and Positive 
*   justification (PJO) and server size, the amount of payload bytes in ODTUk 
*   container excluding fixed stuff, for AMP processing.
*
*
* INPUTS:
*   *rxjc_proc_handle     - pointer to RXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   server_type           - OPUk rate if used by TGMP/RGMP or
*                           HO ODUk rate if ODTU_MUX/DMX.
*                           Refer to util_global_odukp_type_t                            
*   client_type           - LO ODUj rate when used by ODTU_MUX/DMX. 
*                           This argument is ignored if used by TGMP/RGMP.
*                           Refer to util_global_odukp_type_t
*   ts_type               - tributary slot type \n
*                           Only used if ODTU_MUX/DMX is being used. Otherwise, 
*                           this argument is ignored. \n
*                           0 -- 1G25 \n
*                           1 -- 2G5                            
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
PUBLIC PMC_ERROR rxjc_proc_amp_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                   UINT32 chnl,
                                   util_global_odukp_type_t server_type,
                                   util_global_odukp_type_t client_type,
                                   UINT32 ts_type)
{
    /* variable declaration */
    UINT32 server_size; 
    UINT32 njo_pjo_mode;
    UINT32 payload_size;
    UINT32 fixed_stuff;
    
    /* variable initialization */ 
    server_size = 0; 
    njo_pjo_mode = 0;
    payload_size = 0;
    fixed_stuff = 0; 
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl > 95 || server_type > UTIL_GLOBAL_LAST_ODUK || client_type > UTIL_GLOBAL_LAST_ODUK
        || ts_type > LAST_UTIL_GLOBAL_ODU_LINE_PAYLOAD)
    {
        PMC_RETURN(RXJC_PROC_ERR_INVALID_PARAMETERS);
    }
        
    PMC_LOG_TRACE("%s, %s, %d, Channel = %u, server_type = %u, client_type = %u, ts_type =%u\n", __FILE__, __FUNCTION__, __LINE__, chnl, server_type, client_type, ts_type);
    
    /* DEMUX */
    if (rxjc_proc_handle->cfg.instance == RXJC_PROC_INSTANCE_DEMUX)
    {
        if (server_type == UTIL_GLOBAL_ODU1)
        {
            njo_pjo_mode = 0;
            fixed_stuff = 0;
            payload_size = 15232;
        } 
        else if (server_type == UTIL_GLOBAL_ODU2)
        {
            njo_pjo_mode = 1;
            fixed_stuff = 0;
            if(ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                payload_size = 30464;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                payload_size = 15232;   
            } 
        }   
        else if (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU1)
        {
            njo_pjo_mode = 1;
            fixed_stuff = 64;
            if(ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                payload_size = 30464;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                payload_size = 15232;   
            } 
        }   
        else if (server_type == UTIL_GLOBAL_ODU3 && client_type == UTIL_GLOBAL_ODU2)
        {
            njo_pjo_mode = 1;
            fixed_stuff = 0;
            if(ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                payload_size = 121856;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5) 
            {
                payload_size = 60928;   
            } 
        }     
        else if (server_type == UTIL_GLOBAL_ODU3E1 
                 && (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
        {
            njo_pjo_mode = 2;
            fixed_stuff = 0;
            payload_size = 60928;
        }
        else 
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        } 
    }
    else 
    {
        /* Demapper */
        if((server_type == UTIL_GLOBAL_ODU1 || server_type == UTIL_GLOBAL_ODU1E 
                 || server_type == UTIL_GLOBAL_ODU1E_ETRANS
                 || server_type == UTIL_GLOBAL_ODU1F) 
                && client_type == UTIL_GLOBAL_ODUK_DONT_CARE)
        {
            njo_pjo_mode = 0;
            fixed_stuff = 0;
            payload_size = 15232;
        }
        else if((server_type == UTIL_GLOBAL_ODU2 || server_type == UTIL_GLOBAL_ODU2E
                  || server_type == UTIL_GLOBAL_ODU2E_ETRANS  
                  || server_type == UTIL_GLOBAL_ODU2F) 
                 && client_type == UTIL_GLOBAL_ODUK_DONT_CARE)
        {
            njo_pjo_mode = 0;
            fixed_stuff = 64;
            payload_size = 15232;
        }
        else if((server_type == UTIL_GLOBAL_ODU3 || server_type == UTIL_GLOBAL_ODU3E1  
                  || server_type == UTIL_GLOBAL_ODU3E2) 
                 && client_type == UTIL_GLOBAL_ODUK_DONT_CARE)  
        {
            njo_pjo_mode = 0;
            fixed_stuff = 128;
            payload_size = 15232;
        } 
        else 
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        }   
    }
    
    PMC_LOG_TRACE("%s, %s, %d, Channel = %u, payload_size = %u, fixed_stuff = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl, payload_size, fixed_stuff);
    PMC_LOG_TRACE("%s, %s, %d, Channel = %u, server_size = %u, njo_pjo_mode = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl, server_size, njo_pjo_mode); 
    
    /* Configure Server_size to ODTU container size  Fixed stuff bytes */
    server_size = payload_size - fixed_stuff;
    rxjc_proc_field_SERVER_SIZE_set(NULL, rxjc_proc_handle, chnl, server_size);
    /* Configure NJO_PJO_MODE */
    rxjc_proc_field_NJO_PJO_MODE_set(NULL, rxjc_proc_handle, chnl, njo_pjo_mode);
    
    /* Forcing the register value to 0 for AMP (avoiding propagating X in MP_MGEN) */
    rxjc_proc_field_CND_MAX_set(NULL, rxjc_proc_handle, chnl, 0);
    rxjc_proc_field_M_set(NULL, rxjc_proc_handle, chnl, 0);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* rxjc_proc_amp_cfg */

/*******************************************************************************
* rxjc_proc_bmp_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures Negative justification (NJO) and Positive 
*   justification (PJO) and server size, the amount of payload bytes in ODTUk 
*   container excluding fixed stuff, for AMP processing.
*
*
* INPUTS:
*   *rxjc_proc_handle     - pointer to RXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   server_type           - OPUk rate if used by TGMP/RGMP or
*                           HO ODUk rate if ODTU_MUX/DMX. 
*                           Refer to util_global_odukp_type_t.                            
*   ts_type               - tributary slot type. 
*                           Only used if ODTU_MUX/DMX is being used. Otherwise, 
*                           this argument is ignored. \n
*                           0 -- 1G25 \n
*                           1 -- 2G5                            
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
PUBLIC PMC_ERROR rxjc_proc_bmp_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                   UINT32 chnl,
                                   util_global_odukp_type_t server_type,
                                   UINT32 ts_type)
{
    /* variable declaration */
    UINT32 server_size; 
    UINT32 njo_pjo_mode;
    UINT32 payload_size;
    UINT32 fixed_stuff;
    
    /* variable initialization */ 
    server_size = 0; 
    njo_pjo_mode = 0;
    payload_size = 0;
    fixed_stuff = 0; 
    
    PMC_ENTRY();
    
    PMC_LOG_TRACE("%s, %s, %d, Channel = %u, server_type = %u, ts_type =%u\n", __FILE__, __FUNCTION__, __LINE__, chnl, server_type, ts_type);
    
    
    /* Demapper */
    if(server_type == UTIL_GLOBAL_ODU1 || server_type == UTIL_GLOBAL_ODU1E
        || server_type == UTIL_GLOBAL_ODU1E_ETRANS
        || server_type == UTIL_GLOBAL_ODU1F) 
    {
        njo_pjo_mode = 0;
        fixed_stuff = 0;
        payload_size = 15232;
    }
    else if(server_type == UTIL_GLOBAL_ODU2E || server_type == UTIL_GLOBAL_ODU2E_ETRANS || server_type == UTIL_GLOBAL_ODU2F 
            || server_type == UTIL_GLOBAL_ODU2)
    {
        njo_pjo_mode = 0;
        fixed_stuff = 64;
        payload_size = 15232;
    }
    else if(server_type == UTIL_GLOBAL_ODU3 || server_type == UTIL_GLOBAL_ODU3E1 
            || server_type == UTIL_GLOBAL_ODU3E2)  
    {
        njo_pjo_mode = 0;
        fixed_stuff = 128;
        payload_size = 15232;
    } 
    else if(server_type == UTIL_GLOBAL_ODUFLEX_CBR)  
    {
        njo_pjo_mode = 0;
        fixed_stuff = 0;
        payload_size = 15232;
    } 
    else 
    {
        PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
    }   
    
    
    PMC_LOG_TRACE("%s, %s, %d, Channel = %u, payload_size = %u, fixed_stuff = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl, payload_size, fixed_stuff);
    PMC_LOG_TRACE("%s, %s, %d, Channel = %u, server_size = %u, njo_pjo_mode = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl, server_size, njo_pjo_mode); 
    
    /* Configure Server_size to ODTU container size - Fixed stuff bytes */
    server_size = payload_size - fixed_stuff;
    rxjc_proc_field_SERVER_SIZE_set(NULL, rxjc_proc_handle, chnl, server_size);
    /* Configure NJO_PJO_MODE */
    rxjc_proc_field_NJO_PJO_MODE_set(NULL, rxjc_proc_handle, chnl, njo_pjo_mode);
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* rxjc_proc_bmp_cfg */

/*******************************************************************************
* rxjc_proc_cm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures minimum and maximum value for Cm. 
*   cm_max defines the maximum value for Special Mode JC and cm_min 
*   defines the minimum value for Special Mode JC.
*
*
* INPUTS:
*   *rxjc_proc_handle     - pointer to RXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*   cm_max                - For Special Mode channels the maximum value for
*                           Special Mode JC
*   cm_min                - For Special Mode channels the minimum value for
*                           Special Mode JC                           
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
PUBLIC PMC_ERROR rxjc_proc_cm_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                  UINT32 chnl,
                                  UINT32 cm_max,
                                  UINT32 cm_min)
{
    /* variable declaration */
    rxjc_proc_buffer_t rxjc_proc_buf[1];

    PMC_ENTRY();
    
    rxjc_proc_buffer_init(rxjc_proc_buf, rxjc_proc_handle);
    rxjc_proc_field_CM_MAX_set(rxjc_proc_buf, rxjc_proc_handle, chnl, cm_max);
    rxjc_proc_field_CM_MIN_set(rxjc_proc_buf, rxjc_proc_handle, chnl, cm_min);
    rxjc_proc_buffer_flush(rxjc_proc_buf);
     
    PMC_RETURN(PMC_SUCCESS);
    
} /* rxjc_proc_cm_cfg */

/*******************************************************************************
* rxjc_proc_lo_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function deprovisions a LO channel in ODTU_DMX. This function is used
*   to initialize LO channel based configurations to default values.
*
*
* INPUTS:
*   *rxjc_proc_handle     - pointer to RXJC_PROC handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95                          
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
PUBLIC PMC_ERROR rxjc_proc_lo_chnl_deprov(rxjc_proc_handle_t *rxjc_proc_handle, 
                                          UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    result = rxjc_proc_cm_cfg(rxjc_proc_handle, chnl, 0, 0);
    
    if (PMC_SUCCESS == result)
    {
        rxjc_proc_reg_COMMON_CFG_2_array_write(NULL, rxjc_proc_handle, chnl, 0x00000000 );
    }    
    
    PMC_RETURN(result);
} /* rxjc_proc_lo_chnl_deprov */    


/*******************************************************************************
* rxjc_proc_gmp_param_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the following GMP releated parameters: \n
*       Cm, max - Maximum number of m-bit client data entities per HO ODUk 
*                 multiframe \n
*       Cm, min - Minimum number of m-bit client data entities per HO ODUk 
*                 multiframe \n
*       CnD     - difference between Cn and (m/n x Cm); in this API, this value
*                 is set to equal 'M'. \n
*       M       - number of tributary slots occupied by the ODUj \n
*       Pserver - maximum number of data entities in the server payload area
*
*
* INPUTS:
*   *rxjc_proc_handle       - pointer to RXJC_PROC handle instance
*   chnl                    - channel ID to be configured
*                             Valid range: 0 - 95
*   server_type             - HO ODU rate. See util_global_odukp_type_t.
*   client_type             - LO ODU rate for ODTU_DMX. See util_global_odukp_type_t.
*                             Adaptation_function for RGMP. See adaptation_func_enum.
*   client_rate             - Client bit rate. Only valid if RGMP is used or
*                             if client_type is set to ODUFLEX_CBR or ODUFLEX_GFP.
*   num_tribslot            - number of tributary slots occupied by the LO ODU.
*                             Only valid if client_type is set to ODUFLEX_CBR or 
*                             ODUFLEX_GFP                          
*   use_custom_ppm          - TRUE : use ppm_offset parameter to calculate
*                                    CM_MIN and CM_MAX\n
*                             FALSE : use ppm ofsets defined in G.709
*   ppm_offset              - If use_custom_ppm is TRUE, this is
*                             the user defined +/- ppm collar
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
PUBLIC PMC_ERROR rxjc_proc_gmp_param_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                         UINT32 chnl, 
                                         util_global_odukp_type_t server_type,
                                         util_global_odukp_type_t client_type, 
                                         DOUBLE client_rate,
                                         UINT32 num_tribslot,
                                         BOOL   use_custom_ppm,
                                         UINT32 ppm_offset)
{

    /* variable declaration */
    UINT32 cm_max;
    UINT32 cm_min;
    UINT32 cnd_max;
    UINT32 m; 
    UINT32 p_serv;

    PMC_ERROR ret_val = PMC_SUCCESS;

    rxjc_proc_buffer_t rxjc_proc_buffer[1];
    
    PMC_ENTRY();
    
    /* variable initialization */
    cm_max = 0;
    cm_min = 0;
    cnd_max = 0;
    m = 0;
    p_serv = 0;
    
    /* get Cm based on server_type and client_type */
    
    if (RXJC_PROC_INSTANCE_RGMP == (rxjc_proc_handle->cfg.instance))
    {
    
        if(server_type == UTIL_GLOBAL_ODU0)
        {
            m = 1;
            p_serv = 15232;
        }
        else if(server_type == UTIL_GLOBAL_ODU1 || server_type == UTIL_GLOBAL_ODU1E 
                || server_type == UTIL_GLOBAL_ODU1E_ETRANS
                || server_type == UTIL_GLOBAL_ODU1F)
        {
            m = 2;
            p_serv = 7616;
        }
        else if(server_type == UTIL_GLOBAL_ODU2 
                || server_type == UTIL_GLOBAL_ODU2E 
                || server_type == UTIL_GLOBAL_ODU2E_ETRANS 
                || server_type == UTIL_GLOBAL_ODU2F)
        {
            m = 8;
            p_serv = 1904;
        }
        else if(server_type == UTIL_GLOBAL_ODU3 
                || server_type == UTIL_GLOBAL_ODU3E1 
                || server_type == UTIL_GLOBAL_ODU3E2)  
        {
            m = 32;
            p_serv = 476;
        } 
        else if(server_type == UTIL_GLOBAL_ODU4)  
        {
            m = 80;
            p_serv = 190;
        } 
        else 
        {
            PMC_RETURN(PMC_ERR_INVALID_PARAMETERS);
        } 
        
        /* Calculate Cm max and Cm min values */
        ret_val = util_global_cm_calc(server_type, client_type, client_rate, m, FALSE, 0,
                                      &cm_max, &cm_min);  
        PMC_LOG_TRACE("m = %u, p_serv = %u\n", m, p_serv);                                                                                                   
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }                                      
        
    }
    else
    {
        /* instace is DMX */
        switch (server_type)
        {
            case UTIL_GLOBAL_ODU2:
            case UTIL_GLOBAL_ODU2E:
            case UTIL_GLOBAL_ODU2E_ETRANS:
                p_serv = 15232;
                if (client_type == UTIL_GLOBAL_ODU0)
                {
                    cm_max = 15169;
                    cm_min = 15167;
                    m = 1;
                }
                else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                         || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
                {    
                    m = num_tribslot;
                    ret_val = util_global_cm_calc(server_type, client_type, 
                                                  client_rate, m, use_custom_ppm, ppm_offset, &cm_max, &cm_min);
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
                break;
            case UTIL_GLOBAL_ODU3:
                p_serv = 15232;
                if (client_type == UTIL_GLOBAL_ODU0)
                {
                    cm_max = 15105;
                    cm_min = 15103;
                    m = 1;
                }
                else if (client_type == UTIL_GLOBAL_ODU2E
                     || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                     || client_type == UTIL_GLOBAL_ODU2F
                     || client_type == UTIL_GLOBAL_ODU1E
                     || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                     || client_type == UTIL_GLOBAL_ODU1F)
                {
                    m = 9;
                    
                    ret_val = util_global_cm_calc(server_type, client_type, 
                                                  client_rate, m, use_custom_ppm, ppm_offset, &cm_max, &cm_min);

                }
                else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                         || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    m = num_tribslot;
                    ret_val = util_global_cm_calc(server_type, client_type, 
                                                  client_rate, m, use_custom_ppm, ppm_offset, &cm_max, &cm_min);
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
                break; 
            case UTIL_GLOBAL_ODU3E2:
                p_serv = 15232;
                
                /* SE a good enhancement for this API to to use this
                util_global_max_client_num_ts_get() to get all m values */
                
                /* For ODUFlex, we need to use user input for 'm'. 
                ** util_global_max_client_num_ts_get() returns '0' for Flex client 
                */
                if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                    || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    m = num_tribslot;
                }
                else {
                    ret_val = util_global_max_client_num_ts_get(server_type,
                                                                client_type,
                                                                UTIL_GLOBAL_ODU_TS_1G25,
                                                                &m);
                }
                
                if (ret_val == PMC_SUCCESS) {

                      ret_val = util_global_cm_calc(server_type, 
                                                    client_type, 
                                                    client_rate, 
                                                    m, 
                                                    use_custom_ppm,
                                                    ppm_offset,
                                                    &cm_max, 
                                                    &cm_min);

                }

                break;
            case UTIL_GLOBAL_ODU4:  
                p_serv = 15200;
                if (client_type == UTIL_GLOBAL_ODU0)
                {
                    cm_max = 14529;
                    cm_min = 14527;  
                    m = 1;
                }
                else if (client_type == UTIL_GLOBAL_ODU1)
                {
                    cm_max = 14590;
                    cm_min = 14588;
                    m = 2;
                }
                else if (client_type == UTIL_GLOBAL_ODU2)
                {
                    cm_max = 14652;
                    cm_min = 14650;
                    m = 8;
                }
                else if (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
                {
                    cm_max = 15182;
                    cm_min = 15177;
                    m = 8;
                }
                else if(client_type == UTIL_GLOBAL_ODU1F ||
                        client_type == UTIL_GLOBAL_ODU2F)
                {
                    m = 9;
                    
                    ret_val = util_global_cm_calc(server_type, client_type, 
                                                  client_rate, m, use_custom_ppm, ppm_offset, &cm_max, &cm_min);

                }
                else if (client_type == UTIL_GLOBAL_ODU3) 
                {
                    cm_max = 15188;
                    cm_min = 15186;
                    m = 31;
                }
                else if (client_type == UTIL_GLOBAL_ODU3E2
                         || client_type == UTIL_GLOBAL_ODU3E1) 
                {
                    m = 33;
                    ret_val = util_global_cm_calc(server_type, client_type, 
                                                  client_rate, m, use_custom_ppm, ppm_offset, &cm_max, &cm_min);
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
                else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                         || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
                {
                    m = num_tribslot;
                    ret_val = util_global_cm_calc(server_type, client_type, 
                                                  client_rate, m, use_custom_ppm, ppm_offset, &cm_max, &cm_min);
                    if (ret_val != PMC_SUCCESS)
                    {
                        PMC_RETURN(ret_val);
                    }
                }
                break;
            default:
                PMC_RETURN(RXJC_PROC_ERR_INVALID_PARAMETERS);
        }         
    }
    
    cnd_max = m - 1;
    
    PMC_LOG_TRACE("Cm max is %u\n", cm_max);
    PMC_LOG_TRACE("Cm min is %u\n", cm_min);
    
    rxjc_proc_buffer_init(rxjc_proc_buffer, rxjc_proc_handle);
    rxjc_proc_field_CM_MAX_set(rxjc_proc_buffer, rxjc_proc_handle, chnl, cm_max);
    rxjc_proc_field_CM_MIN_set(rxjc_proc_buffer, rxjc_proc_handle, chnl, cm_min); 
    rxjc_proc_buffer_flush(rxjc_proc_buffer);
    rxjc_proc_buffer_init(rxjc_proc_buffer, rxjc_proc_handle);
    rxjc_proc_field_CND_MAX_set(rxjc_proc_buffer, rxjc_proc_handle, chnl, cnd_max);
    rxjc_proc_field_M_set(rxjc_proc_buffer, rxjc_proc_handle, chnl, m);
    rxjc_proc_buffer_flush(rxjc_proc_buffer);
    rxjc_proc_field_SERVER_SIZE_set(NULL, rxjc_proc_handle, chnl, p_serv);
        
    PMC_RETURN(PMC_SUCCESS);
    
} /* rxjc_proc_gmp_param_cfg */

/*******************************************************************************
* rxjc_proc_param_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function recovers rxjc_proc parameters
*
* INPUTS:
*   *rxjc_proc_handle       - pointer to RXJC_PROC handle instance
*   chnl                    - channel ID to be configured
*                             Valid range: 0 - 95
*   ho_num_tribslot         - number of tributary slots occupied by the HO ODU.
*   num_tribslot            - number of tributary slots occupied by the LO ODU.
*   mapping_mode            - Enum for the mapping mode. See util_global_mapping_mode_t.                       
*   use_custom_ppm          - TRUE : use ppm_offset parameter to calculate
*                                    CM_MIN and CM_MAX\n
*                             FALSE: use ppm ofsets defined in G.709
*   ppm_offset              - If use_custom_ppm is TRUE, this is
*                             the user defined +/- ppm collar
*   
* OUTPUTS:
*   *ts_type                - tributary slot type.                
*   *server_type            - HO ODU rate. See util_global_odukp_type_t.
*   *client_type            - LO ODU rate for ODTU_DMX. See util_global_odukp_type_t.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void rxjc_proc_param_cfg_get(rxjc_proc_handle_t *rxjc_proc_handle, 
                                    UINT32 chnl, 
                                    UINT32 ho_num_tribslot,
                                    UINT32 num_tribslot,
                                    util_global_mapping_mode_t mapping_mode,
                                    BOOL use_custom_ppm,
                                    UINT32 ppm_offset,
                                    util_global_odu_line_payload_t *ts_type,
                                    util_global_odukp_type_t *server_type,
                                    util_global_odukp_type_t *client_type)
{
    BOOL8 hit = FALSE;
    UINT32 cm_max = 0, try_cm_max = 0;
    UINT32 cm_min = 0, try_cm_min = 0;
    UINT32 cnd_max = 0;
    UINT32 m = 0;
    UINT32 p_serv = 0;
    UINT32 njo_pjo_mode = 0;
    UINT32 client_itr = 0, server_itr = 0;
    util_global_odukp_type_t client_types[6],server_types[2];
    DOUBLE client_rates[6];

    PMC_ENTRY();
    
    /* get parameters */
    p_serv  = rxjc_proc_field_SERVER_SIZE_get(NULL, rxjc_proc_handle, chnl);
    
    if (UTIL_GLOBAL_AMP == mapping_mode)
    {
        njo_pjo_mode = rxjc_proc_field_NJO_PJO_MODE_get(NULL, rxjc_proc_handle, chnl);
        
        if (0 == njo_pjo_mode)
        {
            *server_type = UTIL_GLOBAL_ODU1;
            *client_type = UTIL_GLOBAL_ODU0;
            *ts_type = UTIL_GLOBAL_ODU_TS_1G25;
        } else if (2 == njo_pjo_mode)
        {
            *server_type = UTIL_GLOBAL_ODU3E1;
            *client_type = UTIL_GLOBAL_ODU2E;
            *ts_type = UTIL_GLOBAL_ODU_TS_2G5;
        } else if (1 == njo_pjo_mode)
        {
            if (p_serv >= 60928)
            {
                *server_type = UTIL_GLOBAL_ODU3;
                *client_type = UTIL_GLOBAL_ODU2;
                if (p_serv == 121856)
                {
                    *ts_type = UTIL_GLOBAL_ODU_TS_1G25; 
                } else 
                {
                    *ts_type = UTIL_GLOBAL_ODU_TS_2G5; 
                }
            } else if ( p_serv % 15232 == 0)
            {
                *server_type = UTIL_GLOBAL_ODU2;
                *client_type = UTIL_GLOBAL_ODU1;
                if (p_serv == 30464)
                {
                    *ts_type = UTIL_GLOBAL_ODU_TS_1G25; 
                } else 
                {
                    *ts_type = UTIL_GLOBAL_ODU_TS_2G5; 
                }
            } else 
            {
                *server_type = UTIL_GLOBAL_ODU3;
                *client_type = UTIL_GLOBAL_ODU1;
                if (p_serv == 30464-64)
                {
                    *ts_type = UTIL_GLOBAL_ODU_TS_1G25; 
                } else 
                {
                    *ts_type = UTIL_GLOBAL_ODU_TS_2G5; 
                }
            }
        }
    } 
    else if (UTIL_GLOBAL_GMP == mapping_mode)
    {
        cm_max  = rxjc_proc_field_CM_MAX_get(NULL, rxjc_proc_handle, chnl);
        cm_min  = rxjc_proc_field_CM_MIN_get(NULL, rxjc_proc_handle, chnl);
        cnd_max = rxjc_proc_field_CND_MAX_get(NULL, rxjc_proc_handle, chnl);
        m       = rxjc_proc_field_M_get(NULL, rxjc_proc_handle, chnl);

        /* ODU4 */
        if (15200 == p_serv)
        {
            *server_type = UTIL_GLOBAL_ODU4;
            *ts_type = UTIL_GLOBAL_ODU_TS_1G25; 
            
            if (cm_max == 14529)
            {            
                *client_type = UTIL_GLOBAL_ODU0;
            } else if (cm_max == 14590)
            {            
                *client_type = UTIL_GLOBAL_ODU1;
            } else if (cm_max == 14652)
            {
                *client_type = UTIL_GLOBAL_ODU2;
            } else if (cm_max == 15182)
            {
                *client_type = UTIL_GLOBAL_ODU2E;
            } else if (9 == m)
            {                             
                (void) util_global_cm_calc(UTIL_GLOBAL_ODU4, UTIL_GLOBAL_ODU1F, UTIL_GLOBAL_ODU1F_RATE, m, FALSE, 0, &try_cm_max, &try_cm_min); 
                if (try_cm_max == cm_max && try_cm_min == cm_min)
                {
                    *client_type = UTIL_GLOBAL_ODU1F;
                }  else 
                {
                    (void) util_global_cm_calc(UTIL_GLOBAL_ODU4, UTIL_GLOBAL_ODU2F, UTIL_GLOBAL_ODU2F_RATE, m, FALSE, 0, &try_cm_max, &try_cm_min); 
                    if (try_cm_max == cm_max && try_cm_min == cm_min)
                    {
                        *client_type = UTIL_GLOBAL_ODU2F;
                    }                    
                }                
            } else if (cm_max == 15188)
            {
                *client_type = UTIL_GLOBAL_ODU3;
            } else if (33 == m)
            {
                (void) util_global_cm_calc(UTIL_GLOBAL_ODU4, UTIL_GLOBAL_ODU3E2, UTIL_GLOBAL_ODU3E2_RATE, m, FALSE, 0, &try_cm_max, &try_cm_min); 
                if (try_cm_max == cm_max && try_cm_min == cm_min)
                {
                    *client_type = UTIL_GLOBAL_ODU3E2;
                }  else 
                {
                    (void) util_global_cm_calc(UTIL_GLOBAL_ODU4, UTIL_GLOBAL_ODU3E1, UTIL_GLOBAL_ODU3E1_RATE, m, FALSE, 0, &try_cm_max, &try_cm_min); 
                    if (try_cm_max == cm_max && try_cm_min == cm_min)
                    {
                        *client_type = UTIL_GLOBAL_ODU3E1;
                    }                    
                }                
            } else
            {
                if (m == 0)
                {
                   *client_type = UTIL_GLOBAL_ODUFLEX_CBR; 
                } else 
                {
                    (void) util_global_cm_calc(UTIL_GLOBAL_ODU4,UTIL_GLOBAL_ODUFLEX_GFP, 0, m, use_custom_ppm, ppm_offset, &try_cm_max, &try_cm_min); 
                    if (try_cm_max == cm_max && try_cm_min == cm_min)
                    {
                        *client_type = UTIL_GLOBAL_ODUFLEX_GFP;
                    }  else 
                    {
                        *client_type = UTIL_GLOBAL_ODUFLEX_CBR;
                    }
                }
            }            
        }  else if (1 == m && cm_max == 15169)
        {
            *server_type = UTIL_GLOBAL_ODU2; 
            *client_type = UTIL_GLOBAL_ODU0;
            *ts_type = UTIL_GLOBAL_ODU_TS_1G25;
        }  else if (1 == m && cm_max == 15105)
        {
            *server_type = UTIL_GLOBAL_ODU3; 
            *client_type = UTIL_GLOBAL_ODU0;
            *ts_type = UTIL_GLOBAL_ODU_TS_1G25;
        } else
        {
            if (ho_num_tribslot == 32 || ho_num_tribslot == 31 || ho_num_tribslot == 33)
            {
                *ts_type = UTIL_GLOBAL_ODU_TS_1G25;
                /* determine the server/client type */
                server_types[0] = UTIL_GLOBAL_ODU3;
                server_types[1] = UTIL_GLOBAL_ODU3E2;        
                client_types[0] = UTIL_GLOBAL_ODU2E;
                client_types[1] = UTIL_GLOBAL_ODU2F;
                client_types[2] = UTIL_GLOBAL_ODU1E;
                client_types[3] = UTIL_GLOBAL_ODU1F;
                client_types[4] = UTIL_GLOBAL_ODU3;
                client_types[5] = UTIL_GLOBAL_ODUFLEX_GFP;
                client_rates[0] = UTIL_GLOBAL_ODU2E_RATE;
                client_rates[1] = UTIL_GLOBAL_ODU2F_RATE;
                client_rates[2] = UTIL_GLOBAL_ODU1E_RATE;
                client_rates[3] = UTIL_GLOBAL_ODU1F_RATE;                
                client_rates[4] = UTIL_GLOBAL_ODU3_RATE;
                client_rates[5] = 0;
                *server_type = UTIL_GLOBAL_ODU3;
                *client_type = UTIL_GLOBAL_ODUFLEX_CBR;
                for (server_itr = 0; server_itr < 2 &&  FALSE == hit;server_itr++)
                {
                    for (client_itr = 0; client_itr < 6 &&  FALSE == hit; client_itr++)
                    {
                        if (m != 0)
                        {
                            (void) util_global_cm_calc(server_types[server_itr],client_types[client_itr], client_rates[client_itr], m, use_custom_ppm, ppm_offset, &try_cm_max, &try_cm_min); 
                        }
                        if (try_cm_max == cm_max && try_cm_min == cm_min)
                        {
                            *client_type = client_types[client_itr];
                            *server_type = server_types[server_itr];
                            hit = TRUE;
                        }            
                    }        
                }
            } else if (ho_num_tribslot == 8)
            {
                *server_type = UTIL_GLOBAL_ODU2;
                *ts_type = UTIL_GLOBAL_ODU_TS_1G25;

                server_types[0] = UTIL_GLOBAL_ODU2;
                server_types[1] = UTIL_GLOBAL_ODU2E;
                client_types[0] = UTIL_GLOBAL_ODUFLEX_GFP;
                client_rates[0] = 0;
                *client_type = UTIL_GLOBAL_ODUFLEX_CBR;
                for (server_itr = 0; server_itr < 2 &&  FALSE == hit;server_itr++)
                {
                    for (client_itr = 0; client_itr < 1 &&  FALSE == hit; client_itr++)
                    {
                        if (m != 0)
                        {
                            (void) util_global_cm_calc(server_types[server_itr],client_types[client_itr], client_rates[client_itr], m, use_custom_ppm, ppm_offset, &try_cm_max, &try_cm_min); 
                        }
                        if (try_cm_max == cm_max && try_cm_min == cm_min)
                        {
                            *client_type = client_types[client_itr];
                            *server_type = server_types[client_itr];
                            hit = TRUE;
                        }            
                    }        
                }                
            }
        }
    }
           
    PMC_RETURN();
    
} /* rxjc_proc_param_cfg_get */


/*******************************************************************************
* rxjc_proc_cm_status_get(
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function retrieves the rxjc cm stat reg field for the particular
*  handle and channel value.
*
* INPUTS:
*   *rxjc_proc_handle       - pointer to RXJC_PROC handle instance
*   chnl                    - channel ID to be configured
*                             Valid range: 0 - 95
*   
* OUTPUTS:
*   value                   - Value in the CM_STAT_REG register.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void rxjc_proc_cm_status_get(rxjc_proc_handle_t *rxjc_proc_handle, 
                                    UINT32 chnl, 
                                    UINT32 *value)
{
    PMC_ENTRY();
    
    *value = rxjc_proc_field_CM_STATUS_get(NULL, rxjc_proc_handle, chnl);

    PMC_RETURN();
}

/*
** Private Functions
*/

/*
** End of file
*/
