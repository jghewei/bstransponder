/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*      SFI51_D8_TX_18X APIs file.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _SFI51_D8_TX_18X_API_H
#define _SFI51_D8_TX_18X_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "util_global_api.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
/* Maximum number of serdes in block */
#define SFI51_D8_TX_MAX_NUM_SERDES 18
 
/*
** Macro Definitions
*/

/* 
** Structures and Unions
*/

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
#define SFI51_D8_TX_18X_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_LOG_ERR_NULL_HANDLE,             "SFI51_D8_TX_18X: NULL Handle") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_LOG_ERR_INVALID_PARAMETERS,      "SFI51_D8_TX_18X: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_LOG_ERR_SERDES_ID_INVALID,       "SFI51_D8_TX_18X: Serdes ID invalid: Serdes ID should be < 16" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_LOG_ERR_MODE_NOT_FOUND,          "SFI51_D8_TX_18X: Cannot convert util pattern mode to sfi51_d8_tx_18x pattern mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_STATE_IN_RESET,                 "SFI51_D8_TX_18X: Port is in reset state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_STATE_NOT_RESET,                 "SFI51_D8_TX_18X: Port is not in reset state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_STATE_NOT_START,                 "SFI51_D8_TX_18X: Port is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_STATE_NOT_EQUIPPED,              "SFI51_D8_TX_18X: Port is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_STATE_NOT_OPERATIONAL,           "SFI51_D8_TX_18X: Port is not in activated state" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_LOOPBACK_ALREADY_CONFIGURED,     "SFI51_D8_TX_18X: Loopback already configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_LOOPBACK_ALREADY_NOT_CONFIGURED, "SFI51_D8_TX_18X: Loopback already not configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_ACB_FT_FORCE_NOMINAL_NOT_SET,    "SFI51_D8_TX_18X: ACB FT nominal mode not set" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_ACB_FT_CTXT_FIFO_FULL,           "SFI51_D8_TX_18X: CB FT context FIFO is full. Saved configuration will be lost if operation proceed." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_ERR_CONTEXT_INCOHERENT,          "SFI51_D8_TX_18X: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_ERR_CONTEXT_REG_INCOHERENT,      "SFI51_D8_TX_18X: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(SFI51_D8_TX_18X_ERR_CONTEXT_CLEANUP,             "SFI51_D8_TX_18X: Context cleanup is required" )\

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */ 
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum SFI51_D8_TX_18X_LOG_ERR_INDEX
{
    SFI51_D8_TX_18X_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_SFI51_18X_TX_SERDES_GLUE << 16),
    SFI51_D8_TX_18X_LOG_ERR_TABLE
    SFI51_D8_TX_18X_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char SFI51_D8_TX_18X_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char SFI51_D8_TX_18X_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

    

#ifdef __cplusplus
}
#endif

#endif /* _SFI51_D8_TX_18X_API_H */

/*
** end of file
*/
