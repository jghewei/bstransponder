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
#include "util_global_loc.h"

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
const char UTIL_GLOBAL_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    UTIL_GLOBAL_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char UTIL_GLOBAL_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE PMC_ERROR util_global_f_client_range_calc(util_global_odukp_type_t server_type,
                                                  UINT32 client_type, 
                                                  DOUBLE client_rate,
                                                  UINT32 num_tribslot,
                                                  BOOL   use_custom_ppm,
                                                  UINT32 ppm_offset,
                                                  DOUBLE *f_c_max, 
                                                  DOUBLE *f_c_min);
PRIVATE PMC_ERROR util_global_f_server_range_calc(util_global_odukp_type_t server_type,
                                                  UINT32 client_type, 
                                                  DOUBLE client_rate,
                                                  UINT32 num_tribslot, 
                                                  DOUBLE *f_serv_max, 
                                                  DOUBLE *f_serv_min,
                                                  DOUBLE *b_serv); 
PRIVATE PMC_ERROR util_global_f_server_calc(util_global_odukp_type_t server_type,
                                            UINT32 client_type, 
                                            DOUBLE client_rate,
                                            UINT32 num_tribslot,
                                            DOUBLE *f_serv);                                                  
                                                                                               
/*
** Public Functions
*/
/*******************************************************************************
* util_global_max_client_num_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives the number of tributary slots required for a LO ODU
*   container to fit into a HO OPU container. 
*   
*   The values are based on Table 7-9 of G.709.
*
*   This function is only used for ODTU_MUX and ODTU_DMX. 
*
* INPUTS:
*   server_type             - HO ODU rate \n
*                             Refer to util_global_odukp_type_t \n
*   client_type             - LO ODU rate for COREOTN \n
*                             Refer to util_global_odukp_type_t \n
*                             Adaptation_function for MAPOTN \n 
*                             Refer to util_global_map_adapt_func_t \n
*   ts_type                 - tributary slot size used to map LO ODU into HO OPU \n
*                             Refert to util_global_odu_line_payload_t \n
*   *num_tribslot           - number of tributary slots reguired by LO ODU into 
*                             the HO ODU                          
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
PUBLIC PMC_ERROR util_global_max_client_num_ts_get(util_global_odukp_type_t server_type,
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
    case UTIL_GLOBAL_ODU1:
        if (client_type == UTIL_GLOBAL_ODU0 && ts_type == UTIL_GLOBAL_ODU_TS_1G25)
        {
            num_ts = 1;
        }
        break;        
    case UTIL_GLOBAL_ODU2:
    case UTIL_GLOBAL_ODU2E:
    case UTIL_GLOBAL_ODU2F:
    case UTIL_GLOBAL_ODU1E:
    case UTIL_GLOBAL_ODU1F:
        if (client_type == UTIL_GLOBAL_ODU1 && ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            num_ts = 1;
        }
        else if (client_type == UTIL_GLOBAL_ODU1 && ts_type == UTIL_GLOBAL_ODU_TS_1G25)
        {
            num_ts = 2;
        }
        else if (client_type == UTIL_GLOBAL_ODU0 && ts_type == UTIL_GLOBAL_ODU_TS_1G25)
        {
            num_ts = 1;
        }
        break;
    case UTIL_GLOBAL_ODU3:
        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
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
        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            if (client_type == UTIL_GLOBAL_ODU2E
                || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
            {
                num_ts = 4;
            }
        }
        break;
    case UTIL_GLOBAL_ODU3E2:
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
            else if (client_type == UTIL_GLOBAL_ODU1E 
                     || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                     || client_type == UTIL_GLOBAL_ODU2 
                     || client_type == UTIL_GLOBAL_ODU2E
                     || client_type == UTIL_GLOBAL_ODU2E_ETRANS) {
                num_ts = 8;
            }
            else if (client_type == UTIL_GLOBAL_ODU1F
                     || client_type == UTIL_GLOBAL_ODU2F ) {
                num_ts = 9;
            }
            else if (client_type == UTIL_GLOBAL_ODU3)
            {
                num_ts = 32;
            }
        }
        break;
    case UTIL_GLOBAL_ODU4:
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
            else if (client_type == UTIL_GLOBAL_ODU1F
                     || client_type == UTIL_GLOBAL_ODU2F)
            {
                num_ts = 9;
            }
            else if (client_type == UTIL_GLOBAL_ODU1E
                     || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                     || client_type == UTIL_GLOBAL_ODU2 
                     || client_type == UTIL_GLOBAL_ODU2E
                     || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
            {
                num_ts = 8;
            }
            else if (client_type == UTIL_GLOBAL_ODU3)
            {
                num_ts = 31;
            }
            else if (client_type == UTIL_GLOBAL_ODU3E2 
                     || client_type == UTIL_GLOBAL_ODU3E1)
            {
                num_ts = 33;
            }
        }
        break;
    default:
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    
    *num_tribslot = num_ts;
    
    PMC_RETURN(PMC_SUCCESS);
} /* util_global_max_client_num_ts_get */  
                                                    
/*******************************************************************************
* util_global_max_server_num_ts_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives the number of maximum tributary slots allowed in a
*   server container. 
*
* INPUTS:
*   parent                  - parent block that needs to be configured. \n
*                             Refer to util_global_parent_t \n
*   server_type             - HO ODU rate \n
*                             Refer to util_global_odukp_type_t \n
*   ts_type                 - tributary slot size used to map LO ODU into HO OPU \n
*                             Refert to util_global_odu_line_payload_t \n
*   *num_tribslot           - number of tributary slots reguired by LO ODU into 
*                             the HO ODU                          
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
PUBLIC PMC_ERROR util_global_max_server_num_ts_get(util_global_parent_t parent,
                                                   util_global_odukp_type_t server_type,
                                                   util_global_odu_line_payload_t ts_type,
                                                   UINT32 *num_tribslot)
{
    /* variable declaration */
    UINT32 num_ts = 0;
    
    /* argument checking */
    PMC_ENTRY();  
    
    if (parent == UTIL_GLOBAL_TGMP_RGMP)
    {
        switch(server_type)
        {
        case UTIL_GLOBAL_ODU0:
            num_ts = 1;
            break;
        case UTIL_GLOBAL_ODU1:
            num_ts = 2;
            break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
            num_ts = 8;
            break;
        case UTIL_GLOBAL_ODU3:
        case UTIL_GLOBAL_ODU3E2:
            num_ts = 32;
            break;
        case UTIL_GLOBAL_ODU4:
            num_ts = 80;
            break;
        default:
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        /* parent is DEMUX or MUX */
        switch (server_type)
        {
        case UTIL_GLOBAL_ODU0:
            num_ts = 1;
            break;
        case UTIL_GLOBAL_ODU1:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                num_ts = 1;
            }  
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25 
                     || ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                num_ts = 2;
            }   
            break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
        case UTIL_GLOBAL_ODU1F:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                num_ts = 4;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25
                     || ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                num_ts = 8;
            }
            break;
        case UTIL_GLOBAL_ODU3:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                num_ts = 16;
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                num_ts = 32;
            } 
            else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                num_ts = 31;
            } 
            break;
        case UTIL_GLOBAL_ODU3E1:
            if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
            {
                num_ts = 32;
            }
            else {
                num_ts = 16;
            }
            break;
        case UTIL_GLOBAL_ODU3E2:
            num_ts = 32;              
            break;
        case UTIL_GLOBAL_ODU4:
            num_ts = 80;
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR:
        case UTIL_GLOBAL_ODUFLEX_GFP:
            num_ts = 0;
            break;
        default:
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        } 
    }
    
    *num_tribslot = num_ts;
    
    PMC_RETURN(PMC_SUCCESS);
} /* util_global_max_server_num_ts_get */  

/*******************************************************************************
* util_global_opu_pt_assign
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the payload type of the ODTU payload depending on 
*   the tributary slot size.
*
* INPUTS:
*   parent                - parent of the module calling this API
*   server_type           - for DEMUX, HO ODU rate \n
*                           for RGMP, ODUkp or ODUjp type \n
*                           Refer to util_global_odukp_type_t \n
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   client_type           - for DEMUX, LO ODU rate \n
*                           Refer to util_global_odukp_type_t \n
*                           for RGMP, adaptaion_function \n
*                           Refer to adaptaion_function_enum for valid types \n
*   mapping_mode          - mapping mode used in demuxing the server signal \n
*                           Refer to util_global_mapping_mode_t \n
*   ts_type               - tributary slot size \n
*                           Refer to util_global_odu_line_payload_t \n
*   *payload_type         - pointer to the pt value
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
PUBLIC PMC_ERROR util_global_opu_pt_assign(util_global_parent_t parent, 
                                           util_global_odukp_type_t server_type,
                                           UINT32 client_type,
                                           util_global_mapping_mode_t mapping_mode,
                                           util_global_odu_line_payload_t ts_type,
                                           util_global_opu_payload_type_t *payload_type)
{
    /* variable declaration */
    UINT32 pt = 0;
    util_global_map_adapt_func_t adaptation_func = UTIL_GLOBAL_MAP_NO_RATE;
     
    PMC_ENTRY();
    
    PMC_LOG_TRACE("server_type = %u, client_type = %u, mapping_mode = %u, \n",  server_type, client_type, mapping_mode);   
    
    if (client_type < UTIL_GLOBAL_ODU0)
    {
        adaptation_func = (util_global_map_adapt_func_t)client_type;  
    }
    
    /* For TGFPF */
    if (mapping_mode == UTIL_GLOBAL_GFP) 
    {
        if (UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP == adaptation_func || UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP == adaptation_func 
            || UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP == adaptation_func  || UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP == adaptation_func)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_5;
        }
        else if(UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP == adaptation_func)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_9; 
        }
        else if(UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP == adaptation_func)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_8;
        }
    }
    else if (parent == UTIL_GLOBAL_TGMP_RGMP)
    {
        
        if (mapping_mode ==  UTIL_GLOBAL_BMP)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_3;
            if (UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP  == adaptation_func  || UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP  == adaptation_func
                || UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP  == adaptation_func)  
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_1; 
            }
            else if (UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP == adaptation_func)
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_F;
            }
            else if (UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP == adaptation_func)
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_8;
            }
            else if (UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP == adaptation_func)
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_80;
            }
        }
        else if(mapping_mode ==  UTIL_GLOBAL_AMP)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_2;      
        }
        else if(mapping_mode ==  UTIL_GLOBAL_GMP)
        {
            if(adaptation_func == UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP || adaptation_func == UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP)
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_7;
            }
            else if ((adaptation_func >= UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP && adaptation_func <= UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP) 
                || (adaptation_func >= UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP && adaptation_func <= UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP))
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_21;
            }
            else if(adaptation_func >= UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP && adaptation_func <= UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP)
            {
                pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_1;
            }
        }
        else
        {
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else if (parent == UTIL_GLOBAL_MUX_DEMUX)
    {
        /* if AMP and ts size = 2.5G -> PT = 20 
           if ts size = 1.25G, -> PT = 21 
           
           ** special case when ODU0 muxed into ODU1 using 1.25G TS -> PT = 20 */
        if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_20;
        }
        else if (server_type == UTIL_GLOBAL_ODU1 && ts_type == UTIL_GLOBAL_ODU_TS_1G25)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_20;
        }   
        else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_21;
        }
        else if (server_type == UTIL_GLOBAL_ODU3E1 && 
                 (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS))
        {
            pt = UTIL_GLOBAL_OPU_PAYLOAD_PT_80;
        }
        else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED)
        {
            pt = 0;
        }
        else 
        {
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    
    *payload_type = (util_global_opu_payload_type_t)pt;
     
    PMC_RETURN(PMC_SUCCESS);
    
} /* util_global_opu_pt_assign */

/*******************************************************************************
* util_global_msi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected MSI for one tributary slot in a LO
*   ODU channel.
*
* INPUTS:
*   ts_loc              - tributary slot location
*                         Valid range: 0 - 79
*   server_type         - server ODUk type \n
*                         Refer to util_global_odukp_type_t \n
*   client_type         - client type \n
*                         Refer to util_global_odukp_type_t \n
*   ts_type             - tributary slot type \n 
*                         Refer to util_global_odu_line_payload_t \n
*   ts_port             - tributary port number where the tributary slot will be
*                         transported.
*   *msi_byte           - pointer to expected MSI byte
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
PUBLIC PMC_ERROR util_global_msi_cfg(UINT32 ts_loc, 
                                     util_global_odukp_type_t server_type, 
                                     UINT32 client_type, 
                                     util_global_odu_line_payload_t ts_type, 
                                     UINT32 ts_port, UINT32 *msi_byte)                                 
{
    /* variable declaration */
    util_global_odtu_type_t odtu_type   = (util_global_odtu_type_t)0;
    UINT32 msi              = 0;
    
    UINT32 psi_offset;
    UINT32 psi_off          = 6;
    UINT32 psi_odu4_off     = 7;
    
    UINT32 msi_val;
    UINT32 msi_mask;
    UINT32 msi_msk          = 0x3F;
    UINT32 msi_odu4_mask    = 0x7F;
  
    PMC_ENTRY();
    
    switch (server_type)
    {
        case UTIL_GLOBAL_ODU1:
            if (client_type == UTIL_GLOBAL_ODU0)
            {
                odtu_type = (util_global_odtu_type_t)0x3;
                /* fixed assignment */
                msi = ts_loc;
            }
            else {
                /* empty HO */
                odtu_type = (util_global_odtu_type_t)0x0; 
                msi = 0;
            }
            break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1F:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                if (client_type == UTIL_GLOBAL_ODU1)
                {
                    odtu_type = (util_global_odtu_type_t)0x0;
                    /* fixed assignment */
                    msi = ts_loc;
                }
                else {
                    /* empty HO; 0x0 for odtu_type indicates presence of ODTU12 */
                    odtu_type = (util_global_odtu_type_t)0x3;
                    /* fixed assignment */
                    msi = 0;
                }
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                if (client_type == UTIL_GLOBAL_ODU1)
                {
                    odtu_type = (util_global_odtu_type_t)0x0;
                    if (ts_port > 7)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        /* flexible assignment */
                        msi = ts_port;
                    } 
                }
                else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                         || client_type == UTIL_GLOBAL_ODUFLEX_GFP
                         || client_type == UTIL_GLOBAL_ODU0)
                {
                    odtu_type = (util_global_odtu_type_t)0x2;
                    if (ts_port > 7)
                    {    
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        /* flexible assignment */
                        msi = ts_port;
                    } 
                }
                else {
                    /* empty HO */
                    odtu_type = (util_global_odtu_type_t)0x3;
                    msi = 0;
                }
            } 
            else {
                /* empty HO */
                odtu_type = (util_global_odtu_type_t)0x3;
                msi = 0;
            }  
            break;
        case UTIL_GLOBAL_ODU3:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                if (client_type == UTIL_GLOBAL_ODU1)
                {
                    odtu_type = (util_global_odtu_type_t)0x0;
                    if (ts_port > 15)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        /* fixed assignment */
                        msi = ts_loc;
                    }  
                }
                else if (client_type == UTIL_GLOBAL_ODU2 
                        /* || client_type == UTIL_GLOBAL_ODU2E
                         || client_type == UTIL_GLOBAL_ODU2E_ETRANS*/)
                {
                    odtu_type = (util_global_odtu_type_t)0x1;
                    if (ts_port > 15)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        /* flexible assignment */
                        msi = ts_port;
                    }      
                }
                else {
                    /* empty HO. No recommended default for Unallocated. 
                       Set to RESERVED */
                    odtu_type = (util_global_odtu_type_t)0x0;
                    msi = 0;
                }
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                /* flexible assignment */
                if (client_type == UTIL_GLOBAL_ODU1
                    /*|| client_type == UTIL_GLOBAL_ODU1E
                    || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                    || client_type == UTIL_GLOBAL_ODU1F*/)
                {
                    odtu_type = (util_global_odtu_type_t)0x0;
                    if (ts_port > 31)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else if (client_type == UTIL_GLOBAL_ODU2 
                         /*|| client_type == UTIL_GLOBAL_ODU2E
                         || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                         || client_type == UTIL_GLOBAL_ODU2F*/)
                {
                    odtu_type = (util_global_odtu_type_t)0x1;
                    if (ts_port > 31)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else if (client_type == UTIL_GLOBAL_ODU1E 
                         || client_type == UTIL_GLOBAL_ODU2E
                         || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                         || client_type == UTIL_GLOBAL_ODU1F
                         || client_type == UTIL_GLOBAL_ODU2F)
                {
                    odtu_type = (util_global_odtu_type_t)0x2;
                    if (ts_port > 31)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                         || client_type == UTIL_GLOBAL_ODUFLEX_GFP
                         || client_type == UTIL_GLOBAL_ODU0)
                {
                    odtu_type = (util_global_odtu_type_t)0x2;
                    if (ts_port > 31)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else {
                    /* empty HO, set to Unallocated */
                    odtu_type = (util_global_odtu_type_t)0x3;
                    msi = 0;
                }
            }
            else {
                /* empty HO, set to Unallocated */
                    odtu_type = (util_global_odtu_type_t)0x3;
                    msi = 0;
            } 
            break;
        case UTIL_GLOBAL_ODU3E1:
            if (client_type != UTIL_GLOBAL_ODU2E && client_type != UTIL_GLOBAL_ODU2E_ETRANS && client_type != UTIL_GLOBAL_ODUK_DONT_CARE)
            {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
            }
            else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED 
                     || client_type == UTIL_GLOBAL_ODUK_DONT_CARE
                     || ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {  
                /* empty HO. Set to unallocated */
                /* allow ts_type == 1.25G as NULL payload comes in as 1.25G TS. */
                odtu_type = (util_global_odtu_type_t)0x0;
                msi = 0;
            }
            else {
                /*SE. assumption that 2E->3E1 is odtu_type = 0x1*/
                odtu_type = (util_global_odtu_type_t)0x1;
                /*if (ts_port > 32)*/
                if (ts_port > 16)
                {
                    PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                }
                else {
                    msi = ts_loc;
                }
            
            }
            break;
        case UTIL_GLOBAL_ODU3E2:
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                /* flexible assignment */
                if (client_type == UTIL_GLOBAL_ODU1
                    || client_type == UTIL_GLOBAL_ODU1E
                    || client_type == UTIL_GLOBAL_ODU1E_ETRANS
                    || client_type == UTIL_GLOBAL_ODU1F)
                {
                    /*odtu_type = (util_global_odtu_type_t)0x0;*/
                    odtu_type = (util_global_odtu_type_t)0x1;
                    if (ts_port > 32)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else if (client_type == UTIL_GLOBAL_ODU2 
                         || client_type == UTIL_GLOBAL_ODU2E
                         || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                         || client_type == UTIL_GLOBAL_ODU2F
                         || client_type == UTIL_GLOBAL_ODU3)
                {
                    /*SE. Assumption that ODU3->ODU3e2 is odtu_type == 0x1*/
                    odtu_type = (util_global_odtu_type_t)0x1;
                    if (ts_port > 32)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                         || client_type == UTIL_GLOBAL_ODUFLEX_GFP
                         || client_type == UTIL_GLOBAL_ODU0)
                {
                    odtu_type = (util_global_odtu_type_t)0x1;
                    if (ts_port > 32)
                    {
                        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                    }
                    else {
                        msi = ts_port;
                    }
                }
                else {
                    /* empty HO. Set to Unallocated */
                    odtu_type = (util_global_odtu_type_t)0x0;
                    msi = 0;
                } 
            }
            else {
                    /* empty HO. Called with UNCHANNELIZED. Set to Unallocated */
                    odtu_type = (util_global_odtu_type_t)0x0;
                    msi = 0;
            } 
            break;
        case UTIL_GLOBAL_ODU4:
            /* ODTU4.ts demuxed to ODU3 requires 31 tribslots; only one stage 
            ** of muxing is allowed. In case of two stage muxing of this payload,
            ** only upto 31 ODU0 can be generated
            */
            if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
            }
            else if (ts_type == UTIL_GLOBAL_ODU_UNCHANNELIZED 
                     || client_type == UTIL_GLOBAL_ODUK_DONT_CARE)
            {
                odtu_type = (util_global_odtu_type_t)0x0;
                msi = 0;
            }    
            else {
                odtu_type = (util_global_odtu_type_t)1;
                if (ts_port > 79)
                {
                    PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
                }
                else {
                    msi = ts_port;
                }
            }
            break;
        default:
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* write to MSI */
    psi_offset = ((server_type == UTIL_GLOBAL_ODU4) || (server_type == UTIL_GLOBAL_ODU3E2))? psi_odu4_off:psi_off;
    msi_mask = ((server_type == UTIL_GLOBAL_ODU4) || (server_type == UTIL_GLOBAL_ODU3E2))? msi_odu4_mask:msi_msk;
    
    odtu_type = (util_global_odtu_type_t)(odtu_type << psi_offset);
    PMC_LOG_TRACE("odtu_type = 0x%x\n", odtu_type);
    
    msi_val = msi & msi_mask;
    PMC_LOG_TRACE("msi_val = 0x%x\n", msi_val);
    
    *msi_byte = odtu_type + msi_val;
    
    PMC_LOG_TRACE("psi = 0x%x, msi_val = 0x%x, msi_byte = 0x%x\n", odtu_type, msi_val, *msi_byte);
    
    PMC_RETURN(PMC_SUCCESS);

} /* util_global_msi_cfg */


/*******************************************************************************
* util_global_cm_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the following GMP releated parameters: \n
*       Cm, max -- Maximum number of m-bit client data entities per HO ODUk 
*                  multiframe \n
*       Cm, min -- Minimum number of m-bit client data entities per HO ODUk 
*                  multiframe \n
*
*
* INPUTS:
*   server_type             - HO ODU rate \n
*                             Refer to util_global_odukp_type_t \n
*   client_type             - LO ODU rate for ODTU_DMX \n
*                             Refer to util_global_odukp_type_t \n
*                             Adaptation_function for RGMP \n 
*                             Refer to util_global_map_adapt_func_t\n
*   client_rate             - Client bit rate in bps. Only valid if any rate CBR
*                             is used in RGMP or client_type is set to ODUFLEX 
*                             in ODTU_DMX.
*   num_tribslot            - number of tributary slots occupied by LO ODU 
*   use_custom_ppm          - TRUE : use ppm_offset parameter to calculate
*                                    CM_MIN and CM_MAX
*                             FALSE : use ppm ofsets defined in G.709
*   ppm_offset              - If use_custom_ppm is TRUE, this is
*                             the user defined +/- ppm collar
*   *cm_max                 - pointer to Cm max
*   *cm_min                 - pointer to Cm min                     
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
PUBLIC PMC_ERROR util_global_cm_calc(util_global_odukp_type_t server_type,
                                     UINT32 client_type, 
                                     DOUBLE client_rate,
                                     UINT32 num_tribslot,
                                     BOOL   use_custom_ppm,
                                     UINT32 ppm_offset,
                                     UINT32 *cm_max, UINT32 *cm_min)
{
    /* variable declaration */
    DOUBLE f_server_max = 0;
    DOUBLE f_server_min = 0;
    DOUBLE f_client_max = 0;
    DOUBLE f_client_min = 0;
    
    DOUBLE b_server     = 0;
    DOUBLE b_serv_div_8 = 0;
    DOUBLE cm_denom     = 0;
    
    DOUBLE tmp_cm_max;
    DOUBLE tmp_cm_min;
     
    PMC_ENTRY();


        
    /* argument checking */
    
    /* get num_tribslot or M 
       util_global_max_client_num_ts_get(server_type, client_type, ts_type,
       &num_tribslot); */
    
    /* get f_server range */
    util_global_f_server_range_calc(server_type, client_type, client_rate, num_tribslot,
                                    &f_server_max, &f_server_min, &b_server);
    
    /* get f_client range */
    util_global_f_client_range_calc(server_type, client_type, client_rate, num_tribslot, use_custom_ppm, ppm_offset,
                                    &f_client_max, &f_client_min);
                                                                                                  

    b_serv_div_8 = b_server / 8;
    
    PMC_ASSERT(num_tribslot != 0, UTIL_GLOBAL_LOG_ERR_DIV_BY_ZERO, 0, 0);
    cm_denom = b_serv_div_8 / num_tribslot;
    
    PMC_LOG_TRACE("b_server = %f, b_serv_div_8 = %f, cm_denom = %f\n", b_server, b_serv_div_8, cm_denom);
    PMC_LOG_TRACE("f_client_max = %f, f_server_min = %f\n", f_client_max, f_server_min);
    
    /* find cm_max = (f_client_max / f_server_min) * [(b_server/8)/M] */    
    tmp_cm_max = (DOUBLE)(f_client_max / f_server_min) * cm_denom;
    PMC_LOG_TRACE("tmp cm max is %f\n", tmp_cm_max);
    *cm_max = (UINT32)ceil(tmp_cm_max);
    
    /* find cm_min = (f_client_min / f_server_max) * [(b_server/8)/M] */
    tmp_cm_min = (DOUBLE)(f_client_min / f_server_max) * cm_denom;
    PMC_LOG_TRACE("tmp_cm_min is %f\n", tmp_cm_min);
    *cm_min = (UINT32)PMC_FLOOR(tmp_cm_min);
        
    PMC_LOG_TRACE("Cm max is %u\n", *cm_max);
    PMC_LOG_TRACE("Cm min is %u\n", *cm_min);                                     

    PMC_RETURN(PMC_SUCCESS);
} /* util_global_cm_calc */

/*******************************************************************************
* util_global_cn_calc_double
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the Cn value, the Cn numerator and the Cn 
*   denominator.
*
* INPUTS:
*   parent                - parent block that uses golden cn
*   server_type           - for DEMUX, HO ODU rate \n 
*                           for RGMP, ODUkp or ODUjp type \n
*                           Refer to  util_global_odukp_type_t \n
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   client_type           - for DEMUX, LO ODU rate
*                           Refer to  util_global_odukp_type_t \n
*                           for RGMP, adaptaion_function \n
*                           Refer to adaptaion_function_enum for valid types \n
*   client_rate           - client data rate bits/sec for the case of ODUFLEX
*   ts_type               - tributary slot type. See util_global_odu_line_payload_t.
*   num_tribslot          - number of tributary slots occupied by LO ODU 
*   *cn                   - pointer to the cn value
*   *cn_n                 - pointer to the cn numerator
*   *cn_d                 - pointer to the cn denominator
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
PRIVATE PMC_ERROR util_global_cn_calc_double(util_global_parent_t parent,
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type, 
                                     DOUBLE client_rate, 
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_tribslot,
                                     UINT32 *cn, UINT32 *cn_n, UINT32 *cn_d)
{
    /* variable declaration */
    /*DOUBLE f_server_max = 0;
      DOUBLE f_server_min = 0;
      DOUBLE b_server     = 0;
    
      DOUBLE f_client_max     = 0;
      DOUBLE f_client_min     = 0;*/
    
    DOUBLE cn_golden = 0;
    
    UINT32 max_payload_bytes = 0;
    DOUBLE remainder;
    UINT32 n_temp;
    UINT32 d_temp;
    
    DOUBLE f_server     = 0;
    DOUBLE f_client     = 0;
    DOUBLE bit_rate     = 0;
    DOUBLE ppm_rate     = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* get num_tribslot or M 
       util_global_max_client_num_ts_get(server_type, client_type, ts_type,
       &num_tribslot);*/
    
    /* get f_server rate */
    result = util_global_f_server_calc(server_type, client_type, client_rate, num_tribslot, &f_server);
    if (result != PMC_SUCCESS) {
        PMC_RETURN(result);
    }
    /*util_global_f_server_range_calc(server_type, client_type, client_rate, num_tribslot, 
      &f_server_max, &f_server_min, &b_server);*/
    
    /* get f_client rate */
    if (client_type == UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP)
    {
        f_client = client_rate;
    }
    else if (client_type == UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP) {

        f_client = client_rate;
    }
    else if (client_type == UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP) {

        f_client = client_rate;
    }
    else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR)
    {
        f_client = client_rate;
    }
    else if (client_type == UTIL_GLOBAL_ODUFLEX_GFP)
    {
        if (num_tribslot < 9)
        {
            f_client = (DOUBLE)UTIL_GLOBAL_ODU2_TS_RATE * num_tribslot;
        }
        else if (num_tribslot < 33)
        {
            f_client = (DOUBLE)UTIL_GLOBAL_ODU3_TS_RATE * num_tribslot; 
        }
        else if (num_tribslot < 81)
        {
            f_client = (DOUBLE)UTIL_GLOBAL_ODU4_TS_RATE * num_tribslot; 
        }
    }
    else {
        /* get client rate */
        result = util_global_client_rate_get(client_type, &bit_rate, &ppm_rate);
        if (result != PMC_SUCCESS) {
            PMC_RETURN(result);
        }
        
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_client = bit_rate * 1000;
        }
        else {
            /* do not divide by 1000 for ODU rates used in MUX as it is the 
            ** actual rate to proper precision
            */
            f_client = bit_rate;
        }
    }
    /*util_global_f_client_range_calc(server_type, client_type, client_rate, num_tribslot, 
      &f_client_max, &f_client_min);*/
                                    
    /* Max payload bytes in OPU container - need this */
    if (UTIL_GLOBAL_MUX_DEMUX == parent)
    {
        switch (server_type)
        {
        case UTIL_GLOBAL_ODU1:
            /* ODTU01 1.25G TS */
            if (ts_type != UTIL_GLOBAL_ODU_TS_1G25)
            {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
            }
                
            max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 2;   
            break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1F:
            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 8; 
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 4; 
            }   
            break;
        case UTIL_GLOBAL_ODU3:
        case UTIL_GLOBAL_ODU3E1:
        case UTIL_GLOBAL_ODU3E2:
            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 32; 
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 16; 
            }                       
            break;
        case UTIL_GLOBAL_ODU4:
            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 80; 
            }
            else {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS); 
            }                    
            break;
        default:
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else {
        /* TGMP */
        max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES;
    }
    
    PMC_LOG_TRACE("f_client = %f \n", f_client);
    PMC_LOG_TRACE("f_server = %f \n", f_server); 

    /* Formulat to calculate Cn golden */
    /*cn_golden = (f_client_min / f_server_max) * (DOUBLE)max_payload_bytes;*/
    cn_golden = (f_client / f_server) * (DOUBLE)max_payload_bytes;
    PMC_LOG_TRACE("cn_golden = %f\n", cn_golden);  
     
    /* Remainer to calculate Cn ramp N and D */
    remainder = cn_golden - (DOUBLE)PMC_FLOOR(cn_golden);
    PMC_LOG_TRACE("remainder = %f\n", remainder);  

    /* Find fractional representation of the packet period */
    /*util_opsa_find_rational(remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);*/
    util_opsa_find_rational_less_than_one(remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);
    
    *cn = (UINT32)PMC_FLOOR(cn_golden);
    *cn_n = n_temp;
    *cn_d = d_temp;
           
    PMC_LOG_TRACE("n_temp = %u\n", n_temp); 
    PMC_LOG_TRACE("d_temp = %u\n", d_temp); 
    
    PMC_LOG_TRACE("cn_golden = %u\n", *cn);  
    
       
    PMC_RETURN(PMC_SUCCESS);
          
} /* util_global_cn_calc_double */

/*******************************************************************************
* util_global_cn_calc_float
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the Cn value, the Cn numerator and the Cn 
*   denominator.
*
* INPUTS:
*   parent                - parent block that uses golden cn
*   server_type           - for DEMUX, HO ODU rate \n 
*                           for RGMP, ODUkp or ODUjp type \n
*                           Refer to  util_global_odukp_type_t \n
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   client_type           - for DEMUX, LO ODU rate
*                           Refer to  util_global_odukp_type_t \n
*                           for RGMP, adaptaion_function \n
*                           Refer to adaptaion_function_enum for valid types \n
*   client_rate           - client data rate bits/sec for the case of ODUFLEX
*   ts_type               - tributary slot type. See util_global_odu_line_payload_t.
*   num_tribslot          - number of tributary slots occupied by LO ODU 
*   *cn                   - pointer to the cn value
*   *cn_n                 - pointer to the cn numerator
*   *cn_d                 - pointer to the cn denominator
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
PRIVATE PMC_ERROR util_global_cn_calc_float(util_global_parent_t parent,
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type, 
                                     DOUBLE client_rate, 
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_tribslot,
                                     UINT32 *cn, UINT32 *cn_n, UINT32 *cn_d)
{
    /* variable declaration */
    /*DOUBLE f_server_max = 0;
      DOUBLE f_server_min = 0;
      DOUBLE b_server     = 0;
    
      DOUBLE f_client_max     = 0;
      DOUBLE f_client_min     = 0;*/
    
    FLOAT cn_golden = 0;
    
    UINT32 max_payload_bytes = 0;
    FLOAT remainder;
    UINT32 n_temp;
    UINT32 d_temp;
    
    FLOAT f_server     = 0;
    FLOAT f_client     = 0;
    FLOAT bit_rate     = 0;
    FLOAT ppm_rate     = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* get num_tribslot or M 
       util_global_max_client_num_ts_get(server_type, client_type, ts_type,
       &num_tribslot);*/
    
    /* get f_server rate */
    result = util_global_f_server_calc(server_type, client_type, client_rate, num_tribslot, (DOUBLE *)&f_server);
    if (result != PMC_SUCCESS) {
        PMC_RETURN(result);
    }
    /*util_global_f_server_range_calc(server_type, client_type, client_rate, num_tribslot, 
      &f_server_max, &f_server_min, &b_server);*/
    
    /* get f_client rate */
    if (client_type == UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP)
    {
        f_client = (FLOAT)client_rate;
    }
    else if (client_type == UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP) {

        f_client = (FLOAT)client_rate;
    }
    else if (client_type == UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP) {

        f_client = (FLOAT)client_rate;
    }
    else if (client_type == UTIL_GLOBAL_ODUFLEX_CBR)
    {
        f_client = (FLOAT)client_rate;
    }
    else if (client_type == UTIL_GLOBAL_ODUFLEX_GFP)
    {
        if (num_tribslot < 9)
        {
            f_client = (FLOAT)UTIL_GLOBAL_ODU2_TS_RATE * num_tribslot;
        }
        else if (num_tribslot < 33)
        {
            f_client = (FLOAT)UTIL_GLOBAL_ODU3_TS_RATE * num_tribslot; 
        }
        else if (num_tribslot < 81)
        {
            f_client = (FLOAT)UTIL_GLOBAL_ODU4_TS_RATE * num_tribslot; 
        }
    }
    else {
        /* get client rate */
        result = util_global_client_rate_get(client_type, (DOUBLE *)&bit_rate, (DOUBLE *)&ppm_rate);
        if (result != PMC_SUCCESS) {
            PMC_RETURN(result);
        }
        
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_client = bit_rate * 1000;
        }
        else {
            /* do not divide by 1000 for ODU rates used in MUX as it is the 
            ** actual rate to proper precision
            */
            f_client = bit_rate;
        }
    }
    /*util_global_f_client_range_calc(server_type, client_type, client_rate, num_tribslot, 
      &f_client_max, &f_client_min);*/
                                    
    /* Max payload bytes in OPU container - need this */
    if (UTIL_GLOBAL_MUX_DEMUX == parent)
    {
        switch (server_type)
        {
        case UTIL_GLOBAL_ODU1:
            /* ODTU01 1.25G TS */
            if (ts_type != UTIL_GLOBAL_ODU_TS_1G25)
            {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
            }
                
            max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 2;   
            break;
        case UTIL_GLOBAL_ODU2:
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2F:
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1F:
            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 8; 
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 4; 
            }   
            break;
        case UTIL_GLOBAL_ODU3:
        case UTIL_GLOBAL_ODU3E1:
        case UTIL_GLOBAL_ODU3E2:
            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 32; 
            }
            else if (ts_type == UTIL_GLOBAL_ODU_TS_2G5)
            {   
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 16; 
            }                       
            break;
        case UTIL_GLOBAL_ODU4:
            if (ts_type == UTIL_GLOBAL_ODU_TS_1G25)
            {
                max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES * 80; 
            }
            else {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS); 
            }                    
            break;
        default:
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else {
        /* TGMP */
        max_payload_bytes = UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES;
    }
    
    PMC_LOG_TRACE("f_client = %f \n", f_client);
    PMC_LOG_TRACE("f_server = %f \n", f_server); 

    /* Formulat to calculate Cn golden */
    /*cn_golden = (f_client_min / f_server_max) * (DOUBLE)max_payload_bytes;*/
    cn_golden = (f_client / f_server) * (FLOAT)max_payload_bytes;
    PMC_LOG_TRACE("cn_golden = %f\n", cn_golden);  
     
    /* Remainer to calculate Cn ramp N and D */
    remainder = cn_golden - (FLOAT)PMC_FLOORF(cn_golden);
    PMC_LOG_TRACE("remainder = %f\n", remainder);  

    /* Find fractional representation of the packet period */
    /*util_opsa_find_rational(remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);*/
    util_opsa_find_rational_less_than_one((DOUBLE)remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);
    
    *cn = (UINT32)PMC_FLOORF(cn_golden);
    *cn_n = n_temp;
    *cn_d = d_temp;
           
    PMC_LOG_TRACE("n_temp = %u\n", n_temp); 
    PMC_LOG_TRACE("d_temp = %u\n", d_temp); 
    
    PMC_LOG_TRACE("cn_golden = %u\n", *cn);  
    
       
    PMC_RETURN(PMC_SUCCESS);
          
} /* util_global_cn_calc_float */

/*******************************************************************************
* util_global_cn_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the Cn value, the Cn numerator and the Cn 
*   denominator.
*
* INPUTS:
*   parent                - parent block that uses golden cn
*   server_type           - for DEMUX, HO ODU rate \n 
*                           for RGMP, ODUkp or ODUjp type \n
*                           Refer to  util_global_odukp_type_t \n
*                           ODUkp all ODUk types valid, ODUjp only ODU2/3 
*                           types valid
*   client_type           - for DEMUX, LO ODU rate
*                           Refer to  util_global_odukp_type_t \n
*                           for RGMP, adaptaion_function \n
*                           Refer to adaptaion_function_enum for valid types \n
*   client_rate           - client data rate bits/sec for the case of ODUFLEX
*   ts_type               - tributary slot type. See util_global_odu_line_payload_t.
*   num_tribslot          - number of tributary slots occupied by LO ODU 
*   *cn                   - pointer to the cn value
*   *cn_n                 - pointer to the cn numerator
*   *cn_d                 - pointer to the cn denominator
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
PUBLIC PMC_ERROR util_global_cn_calc(util_global_parent_t parent,
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type, 
                                     DOUBLE client_rate, 
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_tribslot,
                                     UINT32 *cn, UINT32 *cn_n, UINT32 *cn_d)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (!digi_use_float32_get())
    {
        result = util_global_cn_calc_double(parent,
                                     server_type,
                                     client_type, 
                                     client_rate, 
                                     ts_type,
                                     num_tribslot,
                                     cn, cn_n, cn_d);
    }
    else
    {
        result = util_global_cn_calc_float(parent,
                                     server_type,
                                     client_type, 
                                     client_rate, 
                                     ts_type,
                                     num_tribslot,
                                     cn, cn_n, cn_d);
    }

    PMC_RETURN(result);
} /* util_global_cn_calc */

/*******************************************************************************
* util_global_golden_cn_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines the Golden Cn value based on frame period 
*   numerator, denominator and client bit rate.
*
* INPUTS:
*   frm_period_n     - Cn frame period numerator [31:0] \n
*                      (in UTIL_OPSA_REF_FREQ clock cycles)
*   frm_period_d     - Cn frame period denominator [23:0]
*   client_bitrate   - client bit rate (in Hz)
*   *cn_base_int_ptr - pointer to Cn,base Integer [23:0] \n
*                      (Cn,base = cn_base_int + cn_base_n/cn_base_d)
*   *cn_base_n_ptr   - pointer to Cn,base Numerator [31:0]
*   *cn_base_d_ptr   - pointer to Cn,base Denominator [31:0]
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
PUBLIC PMC_ERROR util_global_golden_cn_calc(UINT32 frm_period_n, UINT32 frm_period_d,
                                            DOUBLE client_bitrate, UINT32 *cn_base_int_ptr, 
                                            UINT32 *cn_base_n_ptr, UINT32 *cn_base_d_ptr)
{
    /* variable declaration */
    DOUBLE cn_golden = 0;
    DOUBLE frame_period =0.0;
    FLOAT f_cn_golden = 0;
    FLOAT f_frame_period =0.0F;
    
    DOUBLE remainder;
    FLOAT f_remainder;
    UINT32 n_temp;
    UINT32 d_temp;
    
    PMC_ENTRY();

    if (!digi_use_float32_get())
    {
        frame_period = ((DOUBLE)frm_period_n / (DOUBLE)frm_period_d) / UTIL_GLOBAL_REF_FREQ;                                   
        cn_golden =  (client_bitrate *  frame_period)/(DOUBLE)8.0; 
        
        PMC_LOG_TRACE("frm_period_n = %d\n", frm_period_n);
        PMC_LOG_TRACE("frm_period_d = %d\n", frm_period_d);
        PMC_LOG_TRACE("frame_period = %f\n", frame_period);
        PMC_LOG_TRACE("client_bitrate = %f\n", client_bitrate);
        PMC_LOG_TRACE("cn_golden = %f\n", cn_golden);  
         
        /* Remainer to calculate Cn ramp N and D */
        remainder = cn_golden - (DOUBLE)PMC_FLOOR(cn_golden);
        PMC_LOG_TRACE("remainder = %f\n", remainder);  
        
        /* Find fractional representation of the packet period */
        /*util_opsa_find_rational(remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);*/
        util_opsa_find_rational_less_than_one(remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);
        
        *cn_base_int_ptr = (UINT32)PMC_FLOOR(cn_golden);
        *cn_base_n_ptr = n_temp;
        *cn_base_d_ptr = d_temp;
    }
    else
    {
        f_frame_period = ((FLOAT)frm_period_n / (FLOAT)frm_period_d) / (FLOAT)UTIL_GLOBAL_REF_FREQ;                                   
        f_cn_golden =  (client_bitrate *  f_frame_period)/8.0F; 
        
        PMC_LOG_TRACE("frm_period_n = %d\n", frm_period_n);
        PMC_LOG_TRACE("frm_period_d = %d\n", frm_period_d);
        PMC_LOG_TRACE("frame_period = %f\n", f_frame_period);
        PMC_LOG_TRACE("client_bitrate = %f\n", client_bitrate);
        PMC_LOG_TRACE("cn_golden = %f\n", f_cn_golden);  
         
        /* Remainer to calculate Cn ramp N and D */
        f_remainder = f_cn_golden - (FLOAT)PMC_FLOORF(f_cn_golden);
        PMC_LOG_TRACE("remainder = %f\n", f_remainder);  
        
        /* Find fractional representation of the packet period */
        /*util_opsa_find_rational(remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);*/
        util_opsa_find_rational_less_than_one((DOUBLE)f_remainder, UTIL_GLOBAL_CN_RAMP_D_MAX, &n_temp, &d_temp);
        
        *cn_base_int_ptr = (UINT32)PMC_FLOORF(f_cn_golden);
        *cn_base_n_ptr = n_temp;
        *cn_base_d_ptr = d_temp;
    }
    
    PMC_LOG_TRACE("n_temp = %u\n", n_temp); 
    PMC_LOG_TRACE("d_temp = %u\n", d_temp); 
    
    PMC_LOG_TRACE("cn_golden = %u\n", *cn_base_int_ptr);  
       
    PMC_RETURN(PMC_SUCCESS);
          
} /* util_global_golden_cn_calc */

/*******************************************************************************
* util_global_direction_value_compute
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function computes flow direction new value.
*
* INPUTS:                     
*   old_dir         - The current direction value.
*   dir             - The direction to be enabled or disabled
*   enable          - If true, dir is enabled, if false, dir is disabled
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_global_direction_t - The new direction value.
*
* NOTES:
*
*******************************************************************************/
PUBLIC util_global_direction_t util_global_direction_value_compute(util_global_direction_t old_dir,
                                                                   util_global_direction_t dir,
                                                                   BOOL8                   enable)
{
    BOOL8 cfg_rx = FALSE;
    BOOL8 cfg_tx = FALSE;
    
    PMC_ENTRY();

    if (TRUE == enable)
    {
        old_dir = (util_global_direction_t)(((UINT32) old_dir) | ((UINT32)dir));
    }
    else
    {
        /*determine which direction to disable*/
        switch (dir)
        {
        case UTIL_GLOBAL_DIR_RX:
            cfg_rx = TRUE;
            break;
        case UTIL_GLOBAL_DIR_TX:
            cfg_tx = TRUE;
            break;
        case UTIL_GLOBAL_DIR_BOTH:
            cfg_rx = TRUE;
            cfg_tx = TRUE;
        break;
        default:
            break;
        }
        
        if (TRUE == cfg_rx)
        {
            old_dir = (util_global_direction_t)(((UINT32) old_dir) & ((UINT32)UTIL_GLOBAL_DIR_TX));
        }

        if (TRUE == cfg_tx)
        {
            old_dir = (util_global_direction_t)(((UINT32) old_dir) & ((UINT32)UTIL_GLOBAL_DIR_RX));
        }
    }

    PMC_RETURN(old_dir);
}/*util_global_direction_value_compute*/

/*
** Private Functions
*/
/*******************************************************************************
* util_global_client_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function finds the client bit rate for a non ODUFLEX client type.
*
* INPUTS:
*   client_type             - Adaptation_function for RGMP \n
*                             Refer to util_global_map_adapt_func_t \n
*   *client_rate            - pointer to the client bit rate  
*   *ppm_rate               - pointer to the ppm offset                      
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
PUBLIC PMC_ERROR util_global_client_rate_get(UINT32 client_type, 
                                             DOUBLE *client_rate,
                                             DOUBLE *ppm_rate)
{
    /* variable declaration */
    PMC_ENTRY();
    
    /* temporary */
    if (client_type >= UTIL_GLOBAL_ODU0 && client_type <= UTIL_GLOBAL_ODUK_DONT_CARE)
    {
        *ppm_rate = 0;
    }
    
    switch (client_type)
    {
    case UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP:
    case UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_FC_800_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP:
    case UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_FC_1200_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP:
    case UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_FC_1200_TRANS_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_100_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP:
    case UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP:
    case UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP:
    case UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_ENET_10GE_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_100_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2EP_10G_GDPS_GMP:
    case UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_GDPS_10G_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP:
    case UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_GDPS_5G_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP:
    case UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP:
        *client_rate =(DOUBLE)UTIL_GLOBAL_CPRI_5_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_CPRI_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2P_CPRI_6_GMP:
    case UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_CPRI_6_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_CPRI_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP:
    case UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_CPRI_7_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_CPRI_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_ENET_40GE_XCD_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_100_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP: 
        *client_rate = (DOUBLE)UTIL_GLOBAL_ENET_40GE_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_100_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP:
    case UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_ENET_100GE_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_CBR_40_G_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP:
    case UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_BMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_STS_768_STM_256_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP:
    case UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP:
        *client_rate = (DOUBLE)UTIL_GLOBAL_STS_192_STM_64_RATE / 1000;
        *ppm_rate = UTIL_GLOBAL_OPU_20_PPM_OFF;
        break;
    case UTIL_GLOBAL_ODU0:
        *client_rate = UTIL_GLOBAL_ODU0_RATE;
        break;
    case UTIL_GLOBAL_ODU1: 
        *client_rate = UTIL_GLOBAL_ODU1_RATE;
        break;
    case UTIL_GLOBAL_ODU1E:
    case UTIL_GLOBAL_ODU1E_ETRANS:
        *client_rate = UTIL_GLOBAL_ODU1E_RATE;
        break;
    case UTIL_GLOBAL_ODU1F:
        *client_rate = UTIL_GLOBAL_ODU1F_RATE;
        break;
    case UTIL_GLOBAL_ODU2:
        *client_rate = UTIL_GLOBAL_ODU2_RATE;
        break;
    case UTIL_GLOBAL_ODU2E:
    case UTIL_GLOBAL_ODU2E_ETRANS:
        *client_rate = UTIL_GLOBAL_ODU2E_RATE;
        break;
    case UTIL_GLOBAL_ODU2F:
        *client_rate = UTIL_GLOBAL_ODU2F_RATE;
        break;
    case UTIL_GLOBAL_ODU3:
        *client_rate = UTIL_GLOBAL_ODU3_RATE;
        break;
    case UTIL_GLOBAL_ODU3E1:
        *client_rate = UTIL_GLOBAL_ODU3E1_RATE;
        break;
    case UTIL_GLOBAL_ODU3E2:
        *client_rate = UTIL_GLOBAL_ODU3E2_RATE;
        break;
    case UTIL_GLOBAL_ODU4:
        *client_rate = UTIL_GLOBAL_ODU4_RATE;
        break;
    default:
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* util_global_client_rate_get */

/*******************************************************************************
* util_global_f_client_range_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the clinet range with the PPM offsets.
*
*
* INPUTS:
*   server_type             - HO ODU rate \n
*                             Refer to See util_global_odukp_type_t \n
*   client_type             - LO ODU rate for ODTU_DMX \n
*                             Refer to See util_global_odukp_type_t \n
*                             Adaptation_function for RGMP \n
*                             Refer to util_global_map_adapt_func_t \n
*   client_rate             - Client bit rate in bps. Only valid if any rate CBR
*                             is used in RGMP or client_type is set to ODUFLEX 
*                             in ODTU_DMX.
*   num_tribslot            - number of tributary slots occupied by LO ODU. Only
*                             valid if client_type is ODUFLEX_CBR or ODUFLEX_GFP.
*   use_custom_ppm          - TRUE : use ppm_offset parameter to calculate
*                                    CM_MIN and CM_MAX
*                             FALSE : use ppm ofsets defined in G.709
*   ppm_offset              - If use_custom_ppm is TRUE, this is
*                             the user defined +/- ppm collar
*   *f_c_max                - pointer to maximum range of client rate
*   *f_c_min                - pointer to minimum range of client rate                        
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
PRIVATE PMC_ERROR util_global_f_client_range_calc(util_global_odukp_type_t server_type,
                                                  UINT32 client_type, 
                                                  DOUBLE client_rate,
                                                  UINT32 num_tribslot,
                                                  BOOL   use_custom_ppm,
                                                  UINT32 ppm_offset,
                                                  DOUBLE *f_c_max, 
                                                  DOUBLE *f_c_min)
{
    
    

    /* variable declaration */
    DOUBLE f_client_max = 0;
    DOUBLE f_client_min = 0;
    DOUBLE odu_flex_rate = 0;
    
    DOUBLE ppm_100_off;
    DOUBLE ppm_20_off;
    DOUBLE ppm_custom_off;  
    
    DOUBLE bit_rate;
    DOUBLE ppm_rate;

    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (server_type > UTIL_GLOBAL_ODUK_DONT_CARE
        || client_type > UTIL_GLOBAL_ODUK_DONT_CARE || num_tribslot > 80)
    {
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* PPM offsets */
    ppm_100_off = (DOUBLE)(UTIL_GLOBAL_OPU_100_PPM_OFF/1000000.0);
    ppm_20_off = (DOUBLE)(UTIL_GLOBAL_OPU_20_PPM_OFF/1000000.0);
    ppm_custom_off = (DOUBLE)(ppm_offset/1000000.0);

    
    /* find M */
    if (client_type < UTIL_GLOBAL_ODU0)
    { 
        if (client_type == UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP)
        {
            ppm_rate = ppm_100_off;
            bit_rate = client_rate / 1000.0;
        }
        else if (client_type == UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP) {
            ppm_rate = ppm_20_off;
            bit_rate = client_rate / 1000.0;
        }
        else {
            /* get client rate */
            result = util_global_client_rate_get(client_type, &bit_rate, &ppm_rate);
            if (PMC_SUCCESS == result) {
                ppm_rate = (DOUBLE)(ppm_rate/1000000.0); 
            } else {
                PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
            }
        }
        
        f_client_max = (DOUBLE)(bit_rate * 1000) * (DOUBLE)(1 + ppm_rate);
        f_client_min = (DOUBLE)(bit_rate * 1000) * (DOUBLE)(1 - ppm_rate);
        PMC_LOG_TRACE("bit_rate = %f, f_client_max = %f and f_client_min = %f\n", bit_rate, f_client_max, f_client_min);
    }
    else {
        switch (client_type)
        {
        case UTIL_GLOBAL_ODU0:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU0_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU0_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU0_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU0_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            PMC_LOG_TRACE("f_client_max = %f and f_client_min = %f\n", f_client_max, f_client_min);
            break;
        case UTIL_GLOBAL_ODU1:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU1_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU1_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU1_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU1_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU1E_RATE * (DOUBLE)(1 + ppm_100_off);
                f_client_min = UTIL_GLOBAL_ODU1E_RATE * (DOUBLE)(1 - ppm_100_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU1E_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU1E_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU2:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU2_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU2_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU2_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU2_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU2E_RATE * (DOUBLE)(1 + ppm_100_off);
                f_client_min = UTIL_GLOBAL_ODU2E_RATE * (DOUBLE)(1 - ppm_100_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU2E_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU2E_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break; 
        case UTIL_GLOBAL_ODU1F:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU1F_RATE * (DOUBLE)(1 + ppm_100_off);
                f_client_min = UTIL_GLOBAL_ODU1F_RATE * (DOUBLE)(1 - ppm_100_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU1F_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU1F_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU2F:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU2F_RATE * (DOUBLE)(1 + ppm_100_off);
                f_client_min = UTIL_GLOBAL_ODU2F_RATE * (DOUBLE)(1 - ppm_100_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU2F_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU2F_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU3:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU3_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU3_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU3_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU3_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU3E1:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU3E1_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU3E1_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU3E1_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU3E1_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU3E2:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU3E2_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU3E2_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU3E2_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU3E2_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODU4:
            if(use_custom_ppm == FALSE)
            {
                f_client_max = UTIL_GLOBAL_ODU4_RATE * (DOUBLE)(1 + ppm_20_off);
                f_client_min = UTIL_GLOBAL_ODU4_RATE * (DOUBLE)(1 - ppm_20_off);
            }
            else
            {
                f_client_max = UTIL_GLOBAL_ODU4_RATE * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = UTIL_GLOBAL_ODU4_RATE * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR:
            if(use_custom_ppm == FALSE)
            {                        
                f_client_max = (client_rate) * (DOUBLE)(1 + ppm_100_off);
                f_client_min = (client_rate) * (DOUBLE)(1 - ppm_100_off);
            }
            else
            {
                f_client_max = (client_rate) * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = (client_rate) * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        case UTIL_GLOBAL_ODUFLEX_GFP:
            if (num_tribslot < 9)
            {
                odu_flex_rate = (DOUBLE)UTIL_GLOBAL_ODU2_TS_RATE * num_tribslot;
            }
            else if (num_tribslot < 33)
            {
                odu_flex_rate = (DOUBLE)UTIL_GLOBAL_ODU3_TS_RATE * num_tribslot; 
            }
            else if (num_tribslot < 81)
            {
                odu_flex_rate = (DOUBLE)UTIL_GLOBAL_ODU4_TS_RATE * num_tribslot; 
            }

            if(use_custom_ppm == FALSE)
            {  
                f_client_max = odu_flex_rate * (DOUBLE)(1 + ppm_100_off);
                f_client_min = odu_flex_rate * (DOUBLE)(1 - ppm_100_off);
            }
            else
            {
                f_client_max = odu_flex_rate * (DOUBLE)(1 + ppm_custom_off);
                f_client_min = odu_flex_rate * (DOUBLE)(1 - ppm_custom_off);
            }
            break;
        default:
            PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    
    PMC_LOG_TRACE("f_client_max = %f, f_client_min = %f\n", f_client_max, f_client_min);
    
    *f_c_max = f_client_max;
    *f_c_min = f_client_min;
    
    PMC_LOG_TRACE("f_c_max is %f\n", *f_c_max);
    PMC_LOG_TRACE("f_c_min is %f\n", *f_c_min);
    
    PMC_RETURN(PMC_SUCCESS);
} /* util_global_f_client_range_calc */ 


/*******************************************************************************
* util_global_f_server_range_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the server range with the PPM offsets.
*
*
* INPUTS:
*   server_type             - HO ODU rate \n
*                             Refer to util_global_odukp_type_t \n
*   client_type             - LO ODU rate for ODTU_DMX or ODTU_MUX \n 
*                             Refer to util_global_odukp_type_t \n
*                             Adaptation_function for RGMP or TGMP \n 
*                             Refer to util_global_map_adapt_func_t \n
*   client_rate             - Client bit rate in bps. Only valid if any rate CBR
*                             is used in RGMP/TGMP or client_type is set to 
*                             ODUFLEX_CBR/ODUFLEX_GFP in ODTU_DMX or ODTU_MUX.
*   num_tribslot            - number of tributary slots occupied by LO ODU. Only
*                             valid if ODTU_DMX or ODTU_MUX is used.   
*   *f_serv_max             - pointer to maximum range of server rate
*   *f_serv_min             - pointer to minimum range of server rate  
*   *b_serv                 - pointet to server payload size                      
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
PRIVATE PMC_ERROR util_global_f_server_range_calc(util_global_odukp_type_t server_type,
                                                  UINT32 client_type, 
                                                  DOUBLE client_rate,
                                                  UINT32 num_tribslot, 
                                                  DOUBLE *f_serv_max, 
                                                  DOUBLE *f_serv_min,
                                                  DOUBLE *b_serv)
{
    /* variable declaration */
    DOUBLE f_server_max = 0;
    DOUBLE f_server_min = 0;
    
    DOUBLE b_server = 0;
    DOUBLE b_serv_div_8;
    
    DOUBLE ppm_100_off;
    DOUBLE ppm_20_off;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (server_type > UTIL_GLOBAL_ODUK_DONT_CARE
        || client_type > UTIL_GLOBAL_ODUK_DONT_CARE || num_tribslot > 80)
    {
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* PPM offsets */
    ppm_100_off = (DOUBLE)(UTIL_GLOBAL_OPU_100_PPM_OFF/1000000.0);
    ppm_20_off = (DOUBLE)(UTIL_GLOBAL_OPU_20_PPM_OFF/1000000.0);
    
    switch (server_type)
    {
    case UTIL_GLOBAL_ODU0: 
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU0_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU0_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
        } 
        b_server = num_tribslot * UTIL_GLOBAL_OPU0_BRATE;
        break;
    case UTIL_GLOBAL_ODU1:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU1_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU1_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
        }
        else if (client_type == UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_ODTU01_MAX_BW;
            f_server_min = UTIL_GLOBAL_ODTU01_MIN_BW;
        }   
        b_server = num_tribslot * UTIL_GLOBAL_OPU1_BRATE ;
        break;
    case UTIL_GLOBAL_ODU1E:
    case UTIL_GLOBAL_ODU1E_ETRANS:
    case UTIL_GLOBAL_ODU1F:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU1E_RATE * (DOUBLE)(1 + ppm_100_off);
            f_server_min = UTIL_GLOBAL_OPU1E_RATE * (DOUBLE)(1 - ppm_100_off);
            PMC_LOG_TRACE("f_server_max = %f and f_server_min = %f\n", f_server_max, f_server_min);
            
            num_tribslot = 1;
        }
        b_server = num_tribslot * UTIL_GLOBAL_OPU2_BRATE;
        break;
    case UTIL_GLOBAL_ODU2:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            PMC_LOG_TRACE("server_type = %u, client_type = %u\n", server_type, client_type);
                
            f_server_max = UTIL_GLOBAL_OPU2_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU2_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
        }
        else if (client_type == UTIL_GLOBAL_ODU1)
        {   
            f_server_max = UTIL_GLOBAL_ODTU12_MAX_BW;
            f_server_min = UTIL_GLOBAL_ODTU12_MIN_BW;
        }
        else if (client_type == UTIL_GLOBAL_ODU0 
                 || client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                 || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
        {
            f_server_max = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU2_TS_MAX_BASE_BW;
            f_server_min = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU2_TS_MIN_BASE_BW;
            PMC_LOG_TRACE("num_tribslot = %u\n", num_tribslot);
            PMC_LOG_TRACE("f_server_max = %f and f_server_min = %f\n", f_server_max, f_server_min);
                
        }
        b_server = num_tribslot * UTIL_GLOBAL_OPU2_BRATE;
        break;
    case UTIL_GLOBAL_ODU2E:
    case UTIL_GLOBAL_ODU2E_ETRANS:
    case UTIL_GLOBAL_ODU2F:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU2E_RATE * (DOUBLE)(1 + ppm_100_off);
            f_server_min = UTIL_GLOBAL_OPU2E_RATE * (DOUBLE)(1 - ppm_100_off);
            PMC_LOG_TRACE("f_server_max = %f and f_server_min = %f\n", f_server_max, f_server_min);
            
            num_tribslot = 1;
        }
        b_server = num_tribslot * UTIL_GLOBAL_OPU2_BRATE;
        break;
    case UTIL_GLOBAL_ODU3:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU3_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU3_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
        }
        else if (client_type == UTIL_GLOBAL_ODU1)
        {
            f_server_max = UTIL_GLOBAL_ODTU13_MAX_BW;
            f_server_min = UTIL_GLOBAL_ODTU13_MIN_BW;
        }
        else if (client_type == UTIL_GLOBAL_ODU2)
        {
            f_server_max = UTIL_GLOBAL_ODTU23_MAX_BW;
            f_server_min = UTIL_GLOBAL_ODTU23_MIN_BW;
        }
        else if (client_type == UTIL_GLOBAL_ODU0 
                 || client_type == UTIL_GLOBAL_ODUFLEX_CBR 
                 || client_type == UTIL_GLOBAL_ODUFLEX_GFP
                 || client_type == UTIL_GLOBAL_ODU1E
                 || client_type == UTIL_GLOBAL_ODU1F
                 || client_type == UTIL_GLOBAL_ODU2E
                 || client_type == UTIL_GLOBAL_ODU2F)
        {
            f_server_max = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3_TS_MAX_BASE_BW;
            f_server_min = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3_TS_MIN_BASE_BW;
        }    
        b_server = num_tribslot * UTIL_GLOBAL_OPU3_BRATE;
        break;
    case UTIL_GLOBAL_ODU3E2:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU3E2_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU3E2_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
        }
        else if (client_type == UTIL_GLOBAL_ODU0
                 || client_type == UTIL_GLOBAL_ODU1
                 || client_type == UTIL_GLOBAL_ODU2
                 || client_type == UTIL_GLOBAL_ODU2E
                 || client_type == UTIL_GLOBAL_ODU2E_ETRANS
                 || client_type == UTIL_GLOBAL_ODU1F
                 || client_type == UTIL_GLOBAL_ODU2F
                 || client_type == UTIL_GLOBAL_ODU3
                 || client_type == UTIL_GLOBAL_ODUFLEX_GFP
                 || client_type == UTIL_GLOBAL_ODUFLEX_CBR)
        {
            f_server_max = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3E2_TS_MAX_BASE_BW;
            f_server_min = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3E2_TS_MIN_BASE_BW;    
        }   
        b_server = num_tribslot * UTIL_GLOBAL_OPU3_BRATE;
        break;
    case UTIL_GLOBAL_ODU3E1:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU3E1_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU3E1_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
        }
        else if (client_type == UTIL_GLOBAL_ODU2E || client_type == UTIL_GLOBAL_ODU2E_ETRANS)
        {
            f_server_max = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3E1_TS_MAX_BASE_BW;
            f_server_min = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3E1_TS_MIN_BASE_BW; 
        }   
        b_server = num_tribslot * UTIL_GLOBAL_OPU3_BRATE;
        break;
    case UTIL_GLOBAL_ODU4:
        if (client_type < UTIL_GLOBAL_ODU0)
        {
            f_server_max = UTIL_GLOBAL_OPU4_RATE * (DOUBLE)(1 + ppm_20_off);
            f_server_min = UTIL_GLOBAL_OPU4_RATE * (DOUBLE)(1 - ppm_20_off);
                
            num_tribslot = 1;
            b_server = num_tribslot * UTIL_GLOBAL_OPU0_BRATE;
        }
        /*else if (client_type == UTIL_GLOBAL_ODU0 
          || client_type == UTIL_GLOBAL_ODUFLEX_CBR 
          || client_type == UTIL_GLOBAL_ODUFLEX_GFP)*/
        else if (client_type >= UTIL_GLOBAL_ODU0 
                 && client_type <= UTIL_GLOBAL_ODUFLEX_GFP)                     
        {
            f_server_max = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU4_TS_MAX_BASE_BW;
            f_server_min = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU4_TS_MIN_BASE_BW;
            b_server = num_tribslot * UTIL_GLOBAL_OPU4_BRATE;
        }
            
        break;
    case UTIL_GLOBAL_ODUFLEX_CBR:
        f_server_max = client_rate * (DOUBLE)(1 + ppm_20_off);
        f_server_min = client_rate * (DOUBLE)(1 - ppm_20_off);
            
        num_tribslot = 1;
        b_server = num_tribslot * client_rate;
        break;
    default:
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    
    b_serv_div_8 = b_server / 8;
    
    PMC_LOG_TRACE("b_server = %f, b_serv_div_8 = %f\n", b_server, b_serv_div_8);
    PMC_LOG_TRACE("f_server_max = %f, f_server_min = %f\n", f_server_max, f_server_min);
     
    *f_serv_max = f_server_max;
    *f_serv_min = f_server_min;
    *b_serv     = b_server;
       
    PMC_LOG_TRACE("f_serv_max is %f\n", *f_serv_max);
    PMC_LOG_TRACE("f_serv_min is %f\n", *f_serv_min);
    PMC_LOG_TRACE("b_serv is %f\n", *b_serv);
    
    PMC_RETURN(PMC_SUCCESS);
} /* util_global_f_server_range_calc */ 

/*******************************************************************************
* util_global_f_server_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function finds the server bit rate.
*
*
* INPUTS:
*   server_type             - HO ODU rate \n
*                             Refer to util_global_odukp_type_t \n
*   client_type             - LO ODU rate for ODTU_DMX or ODTU_MUX \n
*                             Refer to util_global_odukp_type_t \n
*                             Adaptation_function for RGMP or TGMP \n
*                             Refer to util_global_map_adapt_func_t \n
*   client_rate             - Client bit rate in bps. Only valid if any rate CBR
*                             is used in RGMP/TGMP or client_type is set to 
*                             ODUFLEX_CBR/ODUFLEX_GFP in ODTU_DMX or ODTU_MUX.
*   num_tribslot            - number of tributary slots occupied by LO ODU. Only
*                             valid if ODTU_DMX or ODTU_MUX is used. 
*   *f_serv                 - pointer to the server bit rate.                         
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
PRIVATE PMC_ERROR util_global_f_server_calc(util_global_odukp_type_t server_type,
                                            UINT32 client_type, 
                                            DOUBLE client_rate,
                                            UINT32 num_tribslot,
                                            DOUBLE *f_serv)
{
    /* variable declaration */
    DOUBLE f_server = 0;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (server_type > UTIL_GLOBAL_ODUK_DONT_CARE)
    {
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
     
    switch (server_type)
    {
    case UTIL_GLOBAL_ODU0: 
        f_server = UTIL_GLOBAL_OPU0_RATE;
        break;
    case UTIL_GLOBAL_ODU1:
        f_server = UTIL_GLOBAL_OPU1_RATE;
        break;
    case UTIL_GLOBAL_ODU1E:
    case UTIL_GLOBAL_ODU1E_ETRANS:
        f_server = UTIL_GLOBAL_OPU1E_RATE;
        break;
    case UTIL_GLOBAL_ODU1F:
        f_server = UTIL_GLOBAL_OPU1F_RATE;
        break;
    case UTIL_GLOBAL_ODU2:
        /*if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
          || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
          {
          f_server = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU2_TS_BASE_BW;
                
          }
          else {
          f_server = UTIL_GLOBAL_OPU2_RATE;
          }  */
        f_server = UTIL_GLOBAL_OPU2_RATE;              
        break;
    case UTIL_GLOBAL_ODU2E:
    case UTIL_GLOBAL_ODU2E_ETRANS:
        f_server = UTIL_GLOBAL_OPU2E_RATE;           
        break;
    case UTIL_GLOBAL_ODU2F:
        /*if (client_type < UTIL_GLOBAL_ODU0)
          {
          f_server = UTIL_GLOBAL_OPU2E_RATE;
          }*/
        f_server = UTIL_GLOBAL_OPU2F_RATE;           
        break;
    case UTIL_GLOBAL_ODU3:
        f_server = UTIL_GLOBAL_OPU3_RATE;
        /*if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
          || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
          {
          f_server = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU3_TS_BASE_BW;
          }
          else {
          f_server = UTIL_GLOBAL_OPU3_RATE;
          } */              
        break;
    case UTIL_GLOBAL_ODU3E2:
        f_server = UTIL_GLOBAL_OPU3E2_RATE;
               
        break;
    case UTIL_GLOBAL_ODU3E1:
        /*if (client_type < UTIL_GLOBAL_ODU0)
          {
          f_server = UTIL_GLOBAL_OPU3E2_RATE;
          }*/
        f_server = UTIL_GLOBAL_OPU3E1_RATE;
               
        break;
    case UTIL_GLOBAL_ODU4:
        /*
          if (client_type < UTIL_GLOBAL_ODU0)
          {
          f_server = UTIL_GLOBAL_OPU4_RATE;
          }
          else if (client_type == UTIL_GLOBAL_ODU0 
          || client_type == UTIL_GLOBAL_ODUFLEX_CBR 
          || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
          {
          f_server = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU4_TS_BASE_BW;
          }*/
        /*if (client_type == UTIL_GLOBAL_ODU0 
          || client_type == UTIL_GLOBAL_ODUFLEX_CBR 
          || client_type == UTIL_GLOBAL_ODUFLEX_GFP)*/
        /*if (client_type == UTIL_GLOBAL_ODUFLEX_CBR 
          || client_type == UTIL_GLOBAL_ODUFLEX_GFP)
          {
          f_server = (DOUBLE)num_tribslot * UTIL_GLOBAL_ODTU4_TS_BASE_BW;
          }
          else {
          f_server = UTIL_GLOBAL_OPU4_RATE;
          }*/
        f_server = UTIL_GLOBAL_OPU4_RATE;
        break;
    case UTIL_GLOBAL_ODUFLEX_CBR:
        f_server = client_rate;
        break;
    default:
        PMC_RETURN(UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS);
    }
    

    PMC_LOG_TRACE("f_server = %f\n", f_server);
     
    *f_serv = f_server;
       
    PMC_LOG_TRACE("f_serv is %f\n", *f_serv);
    
    PMC_RETURN(PMC_SUCCESS);
} /* util_global_f_server_calc */


/*******************************************************************************
* util_global_conv_fract_to_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function converts fraction to decimal. It calculates the numerator 
*    and denominator that correspond to a fractional number. 
*
*
* INPUTS:
*  fract      - Fractional number          
*   
* OUTPUTS:
*   num       - Numerator
*   den       - Denominator
*
* RETURNS:
*   None. 
*
* NOTES:
*  The following code is inspired from example provided by David Eppstein
*  (http://www.ics.uci.edu/~eppstein/numth/frap.c)
*******************************************************************************/
PUBLIC void util_global_fract_to_dec_conv(DOUBLE fract,
                                          UINT32 *num,
                                          UINT32 *den)
{
   
    long long m[2][2];
    DOUBLE x, startx;
    long long maxden;
    long long ai;

    PMC_ENTRY();

    PMC_ASSERT(num != NULL, UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(den != NULL, UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    x = fract;
    startx = fract;

    /* initialize matrix */
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    /* loop finding terms until denom gets too big */
    for ( maxden = 4294967295ULL; maxden >= 0; maxden-- ) 
    {
        while (m[1][0] *  ( ai = (long long)x ) + m[1][1] <= maxden) {
            long long t;
            t = m[0][0] * ai + m[0][1];
            m[0][1] = m[0][0];
            m[0][0] = t;
            t = m[1][0] * ai + m[1][1];
            m[1][1] = m[1][0];
            m[1][0] = t;
            if(x == (DOUBLE)ai) {
                break;     /* AF: division by zero */
            }
            if (!digi_use_float32_get())
            {
                x = 1 / (x - (DOUBLE) ai);
            }
            else
            {
                x = (DOUBLE)(1 / ((FLOAT)x - ai));
            }
            if(x > (DOUBLE)0x7FFFFFFF) {
                break;  /* AF: representation failure */        
            }
        }
    
        if (m[1][0] != 0) 
        {
            /* we have something ..*/
            break;        
        }
    }
    /* now remaining x is between 0 and 1/ai */
    /* approx as either 0 or 1/m where m is max that will fit in maxden */
    /* first try zero */
    if (m[1][0] != 0) 
    {
        PMC_LOG_TRACE("in  util_global_fract_to_dec_conv: fract = %lg: %lld/%lld, error = %e\n", (DOUBLE)fract, m[0][0], m[1][0],
                  (DOUBLE)(startx - ((DOUBLE) m[0][0] / (DOUBLE) m[1][0])));
    }    

    *num = m[0][0];
    *den = m[1][0];

    PMC_RETURN();    
}/* util_global_fract_to_dec_conv */

/*******************************************************************************
* util_global_fract_to_dec_conv_max_num
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function converts fraction to decimal. It calculates the numerator
*    and denominator that correspond to a fractional number.
*   Make sure output numerator will not greater than max_num.
*
* INPUTS:
*  fract      - Fractional number
*  max_num    - max numerator value
*
* OUTPUTS:
*   num       - Numerator
*   den       - Denominator
*
* RETURNS:
*   None.
*
* NOTES:
*  The following code is inspired from example provided by David Eppstein
*  (http://www.ics.uci.edu/~eppstein/numth/frap.c)
*******************************************************************************/
PUBLIC void util_global_fract_to_dec_conv_max_num(DOUBLE fract,
                                                  UINT32 max_num,
                                                  UINT32 *num,
                                                  UINT32 *den)
{

    long long m[2][2];
    DOUBLE x, startx;
    long long maxden;
    long long ai;
    BOOL8 quit = FALSE;

    PMC_ENTRY();

    PMC_ASSERT(num != NULL, UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(den != NULL, UTIL_GLOBAL_LOG_ERR_INVALID_PARAMETERS, 0, 0);

    x = fract;
    startx = fract;

    /* initialize matrix */
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    /* loop finding terms until denom gets too big */
    for ( maxden = 4294967295ULL; maxden >= 0; maxden-- )
    {
        while (m[1][0] *  ( ai = (long long)x ) + m[1][1] <= maxden) {
            long long t;
            t = m[0][0] * ai + m[0][1];
            if ( t > max_num )
            {
                quit = TRUE;
                break;
            }
            m[0][1] = m[0][0];
            m[0][0] = t;
            t = m[1][0] * ai + m[1][1];
            m[1][1] = m[1][0];
            m[1][0] = t;
            if(x == (DOUBLE)ai) {
                break;     /* AF: division by zero */
            }
            if (!digi_use_float32_get())
            {
                x = 1 / (x - (DOUBLE) ai);
            }
            else
            {
                x = (DOUBLE)(1 / ((FLOAT)x - ai));
            }
            if(x > (DOUBLE)0x7FFFFFFF) {
                break;  /* AF: representation failure */
            }
        }

        if ( (m[1][0] != 0) || (TRUE == quit) )
        {
            /* we have something ..*/
            break;
        }
    }
    /* now remaining x is between 0 and 1/ai */
    /* approx as either 0 or 1/m where m is max that will fit in maxden */
    /* first try zero */
    if (m[1][0] != 0)
    {
        PMC_LOG_TRACE("in  util_global_fract_to_dec_conv_max_num: fract = %lg: %lld/%lld, error = %e\n", (DOUBLE)fract, m[0][0], m[1][0],
                  (DOUBLE)(startx - ((DOUBLE) m[0][0] / (DOUBLE) m[1][0])));
    }

    *num = m[0][0];
    *den = m[1][0];

    PMC_RETURN();
}/* util_global_fract_to_dec_conv_max_num */

/*******************************************************************************
* FUNCTION: util_global_is_chnl_prov()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Determines if a channel is provisioned
*
* INPUTS:
*   chnl_handle     - Channel handle
*
* OUTPUTS:
*
* RETURNS:
*   TRUE            - Channel is provisioned
*   FALSE           - Channel is not provisioned
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL util_global_is_chnl_prov(void*  chnl_handle)
{

    util_global_chnl_provisioned_state_t     prov_state;
    BOOL result = FALSE;
    prov_state = ((util_global_chnl_header_t*)chnl_handle)->prov_state;

    PMC_ENTRY();

    if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED    == prov_state ||
        UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED == prov_state ||
        UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED == prov_state)
    {
        result = TRUE;
    }
    PMC_RETURN(result);
} /* util_global_is_chnl_prov */

/*******************************************************************************
* util_global_isnumber
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns TRUE when given string could be converted as
*   a decimal number. 
*
*
* INPUTS:
*  str      - given stiring
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when the given string can be converted as a decimal number. 
*
* NOTES:
*  
*******************************************************************************/
PUBLIC BOOL8 util_global_isnumber(char *str)
{
    BOOL8 rc = TRUE;
    UINT32 i = 0;
    BOOL8 is_hex = FALSE;

    PMC_ENTRY();
    
    while ('\0' != str[i])
    {
        if (1 == i && ('x' == str[i] || 'X' == str[i]) && '0' == str[0])
        {
            is_hex = TRUE;
            i++;
            continue; 
        }

        if (('0' > str[i] || '9' < str[i]) &&
            (TRUE == is_hex && (('a' > str[i] || 'f' < str[i]) &&
                                ('A' < str[i] || 'F' < str[i]))))
        {
            rc = FALSE;
        }

        i++;
    }

    if (0 == i || (TRUE == is_hex && 2 <= i))
    {
        rc = FALSE;
    }

    PMC_RETURN(rc);    
} /*util_global_isnumber*/

/*******************************************************************************
* util_global_recover_energy_state
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function recovers energy state from 3 power flags.
*    all flags set to FALSE -> RUNNING
*    all flags set to TRUE  -> RESET
*    else  -> INCOHERENT
*
* INPUTS:
*  power_flag0   - Power flag 0 
*  power_flag1   - Power flag 1 
*  power_flag2   - Power flag 2 
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   pmc_energy_state_t - the resolved energy state
*
* NOTES:
*  
*******************************************************************************/
PUBLIC pmc_energy_state_t util_global_recover_energy_state(BOOL8 power_flag0,
                                                           BOOL8 power_flag1,
                                                           BOOL8 power_flag2)
{   
    pmc_energy_state_t energy_state;

    PMC_ENTRY();

    if (0 == (power_flag0 | power_flag1 | power_flag2))
    {
        energy_state = PMC_ENERGY_STATE_RUNNING;
    } else  if (1 == (power_flag0 & power_flag1 & power_flag2))
    {
        energy_state = PMC_ENERGY_STATE_RESET;
    } else {
        energy_state = PMC_ENERGY_STATE_INCOHERENT;
    }

    PMC_RETURN(energy_state);    
} /*util_global_recover_energy_state*/

/*******************************************************************************
* util_global_odukp_type_per_num_entries_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function the ODUKP_TYPE per number of calendar entries
*
*
* INPUTS:
*   num_entries           - number of calendar entries
*   mapping_mode          - mapping mode used in demuxing the server signal \n
*                           Refer to util_global_mapping_mode_t \n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   util_global_odukp_type_t - the ODUKP type
*
* NOTES:
*  
*******************************************************************************/
PUBLIC util_global_odukp_type_t  util_global_odukp_type_per_num_entries_get(UINT32 num_entries,
                                                                            util_global_mapping_mode_t mapping_mode)
{   
    util_global_odukp_type_t odukp_type = UTIL_GLOBAL_LAST_ODUK;

    PMC_ENTRY();
    
    if ( 1 == num_entries) 
    {
        odukp_type = UTIL_GLOBAL_ODU0;
    } else if ( 2 == num_entries) 
    {
        odukp_type = UTIL_GLOBAL_ODU1;
    } else if ( 8 == num_entries) 
    {
        odukp_type = UTIL_GLOBAL_ODU2;
    } else if ( 32 == num_entries) 
    {
        odukp_type = UTIL_GLOBAL_ODU3;
    }  else if ( 80 == num_entries) 
    {
        odukp_type = UTIL_GLOBAL_ODU4;
    } else if (80 < num_entries)
    {
        if (UTIL_GLOBAL_GFP == mapping_mode)
        {
            odukp_type = UTIL_GLOBAL_ODUFLEX_GFP;
        } else 
        {
            odukp_type = UTIL_GLOBAL_ODUFLEX_CBR;
        }
    }

    PMC_RETURN(odukp_type);    
} /*util_global_odukp_type_per_num_entries_get */


/*******************************************************************************
* util_global_array_compare
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Compare 2 array
*
*
* INPUTS:
*   ptr_a  - A pointer to an array 
*   ptr_b  - A pointer to another array 
*   array_size - The size of these arrays
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when they are equal, FALSE when they are not 
*
* NOTES:
*  
*******************************************************************************/
PUBLIC BOOL8 util_global_array_compare(UINT8 ptr_a[], UINT8 ptr_b[], UINT32 array_size)
{
    BOOL8 result = TRUE;
    UINT32 itr;
    PMC_ENTRY();
    
    for (itr = 0;itr < array_size && result == TRUE;itr++)
    {
        if (ptr_a[itr] != ptr_b[itr])
        {
            result = FALSE;
        }
    }
    PMC_RETURN(result);
}

/*******************************************************************************
* util_global_double_compare
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Compare 2 double values
*
*
* INPUTS:
*   a  - value a
*   b  - value b 
*   epsilon - max error value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when they are equal, otherwise FALSE.
*
* NOTES:
*  
*******************************************************************************/
PUBLIC BOOL8 util_global_double_compare(DOUBLE a, DOUBLE b, DOUBLE epsilon)
{
    BOOL8 result = TRUE;

    PMC_ENTRY();

    result = (PMC_FABS(a - b) < epsilon);

    PMC_RETURN(result);
}

/*******************************************************************************
* util_global_vert_eye_opening_calc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function computes the vertical eye opening in number of bins.
*
*
* INPUTS:
*   bin_buffer  - Bin buffer array 
*                 It consists of three parts: n-values[0:31],0, and p-values[0:30].
*
* OUTPUTS:
*   *port_verteye_opening  - The vertical eye opening value.
*
* RETURNS:
*   None.
*
* NOTES:
*  
*******************************************************************************/
PUBLIC void util_global_vert_eye_opening_calc(UINT64 bin_buffer[64], 
                                              UINT32 *port_verteye_opening)
{
    UINT64 bin_value;
    UINT32 bin_number;
    UINT32 bin_count_31_to_0=0;
    UINT32 bin_count_33_to_63=0;
    UINT32 eye_opening;

    PMC_ENTRY();

    /* find the first non-zero bin from 31 to 0 */
    bin_count_31_to_0 = 0;
    bin_value = 0;
    bin_number = 31;
    /* bin_number is unsigned int, should always be < 64 */
    while ((bin_value == 0) && (bin_number < 64))
    {
        bin_value = bin_buffer[bin_number];
        if (bin_value == 0)
        {
            bin_count_31_to_0++;
            bin_number--;
        }
    }

    /* find the first non-zero bin from 33 to 63 */
    bin_count_33_to_63 = 0;
    bin_value = 0;
    bin_number = 33;
    while ((bin_value == 0) && (bin_number < 64))
    {
        bin_value = bin_buffer[bin_number];
        if (bin_value == 0)
        {
            bin_count_33_to_63++;
            bin_number++;
        }
    }

    /* Use the lower count to calculate the opening */
    if (bin_count_31_to_0 > bin_count_33_to_63) {
        eye_opening = (bin_count_33_to_63 * 2) + 1;
    } else {
        eye_opening = (bin_count_31_to_0 * 2) + 1;
    }

    if (bin_count_31_to_0 == 32 || bin_count_33_to_63 == 31 || bin_buffer[32] != 0)
    {
        eye_opening = 0;
    }
    *port_verteye_opening = eye_opening;

    PMC_RETURN();

} /* util_global_vert_eye_opening_calc */

/*******************************************************************************
* util_global_chnl_prov_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns channel provisioning state in a given direction.
*
*
* INPUTS:
*   chnl_state  - Channel state
*   dir         - Channel provioning direction direction: Rx, TX, or both
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when channel is provisones in the given direction.
*   FALSE otherwise.
*
* NOTES:
*  
*******************************************************************************/
PUBLIC BOOL8 util_global_chnl_prov_status_get(util_global_chnl_provisioned_state_t  chnl_state,
                                              util_global_direction_t               dir)
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();

    /* determine direction provisining state */
    switch (dir)
    {
    case UTIL_GLOBAL_DIR_RX:
        switch(chnl_state)
        {
        case UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED:
        case UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED:
            rc = TRUE;
            break;
        default:
            break;
        }
        break;
    case UTIL_GLOBAL_DIR_TX:        
        switch(chnl_state)
        {
        case UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED:
        case UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED:
            rc = TRUE;
            break;
        default:
            break;
        }
        break;
    case UTIL_GLOBAL_DIR_BOTH:
        switch(chnl_state)
        {
        case UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED:
            rc = TRUE;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    
    PMC_RETURN(rc);
} /* util_global_chnl_prov_status_get */

/*******************************************************************************
* util_global_rx_or_tx_chnl_prov_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns channel provisioning state in RX or TX direction.
*
*
* INPUTS:
*   chnl_state  - Channel state
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when channel is provisoned in  RX or TX direction.
*   FALSE otherwise.
*
* NOTES:
*  
*******************************************************************************/
PUBLIC BOOL8 util_global_rx_or_tx_chnl_prov_status_get(util_global_chnl_provisioned_state_t  chnl_state)
{
    BOOL8 rc = FALSE;

    PMC_ENTRY();

    /* returns TRUE when one of the direction is provisioned */
    switch(chnl_state)
    {
    case UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED:
    case UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED:
    case UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED:
        rc = TRUE;
        break;
    default:
        break;
    }
    
    PMC_RETURN(rc);
} /* util_global_rx_or_tx_chnl_prov_status_get */

/*
** End of file
*/

