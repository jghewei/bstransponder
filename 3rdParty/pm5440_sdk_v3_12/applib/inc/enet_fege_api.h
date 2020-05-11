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

#ifndef _ENET_FEGE_API_H
#define _ENET_FEGE_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "util_global.h"

/*
** Enumerated Types
*/


/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: enet_fege_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enet FEGE interrupt structure 
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef struct enet_fege_int_t 
{         
    /* interrupts */
    BOOL8 tx_tstmpr_tod_sign_ovfl_i;
    BOOL8 rx_tstmpr_tod_sign_ovfl_i;
    BOOL8 overflow_i;          
    BOOL8 rx_comma_del_chg_i;    
    BOOL8 tx_tstmp_fifo_i;
    BOOL8 tx_sof_i;              
    BOOL8 rx_sof_i;  
    BOOL8 rx_long_length_error_i;
    BOOL8 rx_inrange_length_error_i;
    BOOL8 rx_layer1_error_i;        
    BOOL8 rx_symbol_error_i;        
    BOOL8 rx_fcs_error_i;           
    BOOL8 rx_mac_control_frames_ok_i;
    BOOL8 rx_mac_pause_control_frames_ok_i;
    BOOL8 rx_tagged_frames_ok_i;           
    BOOL8 rx_multicast_frames_ok_i;        
    BOOL8 rx_broadcast_frames_ok_i;        
    BOOL8 rx_unicast_frames_ok_i;          
    BOOL8 rx_octets_i;                     
    BOOL8 rx_frames_i;                     
    BOOL8 rx_octets_ok_i;                  
    BOOL8 rx_frames_ok_i;                  
    BOOL8 rx_variable_i;                   
    BOOL8 rx_station_address_filtered_i;   
    BOOL8 rx_frames_1519_to_max_size_i;    
    BOOL8 rx_frames_1024_to_1518_octets_i; 
    BOOL8 rx_frames_512_to_1023_octets_i;  
    BOOL8 rx_frames_256_to_511_octets_i;   
    BOOL8 rx_frames_128_to_255_octets_i;   
    BOOL8 rx_frames_65_to_127_octets_i;    
    BOOL8 rx_frames_64_octets_i;           
    BOOL8 rx_short_length_crc_error_i;     
    BOOL8 rx_short_length_error_i;         
    BOOL8 rx_long_length_crc_error_i;      
    BOOL8 rx_frames_2501_to_max_size_i;    
    BOOL8 rx_frames_2001_to_2500_octets_i; 
    BOOL8 rx_frames_1549_to_2000_octets_i; 
    BOOL8 rx_frames_1523_to_1548_octets_i; 
    BOOL8 rx_frames_1519_to_1522_octets_i; 
    BOOL8 tx_unicast_frames_attempted_i;   
    BOOL8 tx_multicast_frames_ok_i;        
    BOOL8 tx_broadcast_frames_ok_i;        
    BOOL8 tx_unicast_frames_ok_i;          
    BOOL8 tx_mac_control_frames_ok_i;      
    BOOL8 tx_frames_1523_to_max_size_i;    
    BOOL8 tx_mac_error_i;                  
    BOOL8 tx_system_error_i;               
    BOOL8 tx_long_length_error_i;          
    BOOL8 tx_short_length_error_i;         
    BOOL8 tx_fcs_error_i;                  
    BOOL8 tx_mac_pause_control_frames_ok_i;
    BOOL8 tx_tagged_frames_ok_i;           
    BOOL8 tx_octets_i;                     
    BOOL8 tx_octets_ok_i;                  
    BOOL8 tx_frames_ok_i;                  
    BOOL8 tx_frames_2501_to_max_size_i;    
    BOOL8 tx_frames_2001_to_2500_octets_i; 
    BOOL8 tx_frames_1549_to_2000_octets_i; 
    BOOL8 tx_frames_1523_to_1548_octets_i; 
    BOOL8 tx_frames_1519_to_1522_octets_i; 
    BOOL8 rx_frames_1523_to_max_size_i;    
    BOOL8 tx_variable_i;                   
    BOOL8 tx_frames_1519_to_max_size_i;    
    BOOL8 tx_frames_1024_to_1518_octets_i; 
    BOOL8 tx_frames_512_to_1023_octets_i;  
    BOOL8 tx_frames_256_to_511_octets_i;   
    BOOL8 tx_frames_128_to_255_octets_i;   
    BOOL8 tx_frames_65_to_127_octets_i;    
    BOOL8 tx_frames_64_octets_i;           
    BOOL8 tx_multicast_frames_attempted_i; 
    BOOL8 tx_broadcast_frames_attempted_i;   
    BOOL8 los_i;                           
    BOOL8 sdet_in_i;                       
    BOOL8 align_change_i;
    BOOL8 link_mon_i;
    BOOL8 fec_stat_i;
    BOOL8 carrier_stat_i;
    BOOL8 rx_err_i;                        
    BOOL8 lcv_i;                           
    BOOL8 auto_neg_lp_ablty_upd_i;         
    BOOL8 link_ok_i;                      
    BOOL8 sync_ok_i;

    /* status */
    BOOL8 tx_tstmpr_tod_sign_ovfl_v;
    BOOL8 rx_tstmpr_tod_sign_ovfl_v;
    BOOL8 los_v;                           
    BOOL8 sdet_in_v;                       
    BOOL8 align_change_v;
    BOOL8 link_mon_v;
    BOOL8 fec_stat_v;
    BOOL8 carrier_stat_v;                  
    BOOL8 auto_neg_lp_ablty_upd_v;         
    BOOL8 link_ok_v;
    BOOL8 sync_ok_v;

} enet_fege_int_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
   
                                                                       


/*
** Log Messaging
*/
/* 
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums 
 */
#define ENET_FEGE_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_LOG_CODE_DEBUG,              "ENET_FEGE: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_ERR_CODE_ASSERT,             "ENET_FEGE: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_LOG_CODE_LOG,                "ENET_FEGE: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_ERR_INVALID_ARG,         "ENET_FEGE: Invalid argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_STATE_NOT_EQUIPPED,     "ENET_FEGE: Channel is not equipped" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_STATE_NOT_START,        "ENET_FEGE: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_ERR_INVALID_PTR,             "ENET_FEGE: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_STATE_ALREADY_START,               "ENET_FEGE: Instance is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_STATE_ALREADY_PROVISIONED,                 "ENET_FEGE: Instance is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_FEGE_STATE_UNKOWN,                 "ENET_FEGE: Unkown state" ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ENET_FEGE_LOG_ERR_INDEX
{
    ENET_FEGE_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ENET_FEGE << 16),
    ENET_FEGE_LOG_ERR_TABLE
    ENET_FEGE_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ENET_FEGE_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ENET_FEGE_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#ifdef __cplusplus
}
#endif

#endif /* _ENET_FEGE_API_H */

/*
** end of file
*/
