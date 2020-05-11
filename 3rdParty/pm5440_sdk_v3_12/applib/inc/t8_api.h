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
*
*   NOTES:
*
*******************************************************************************/

#ifndef _T8_API_H
#define _T8_API_H

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

/* T8 serdes user pattern size */
#define T8_USR_PATT_BIT_SIZE 40

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: t8_tx_swing_levels_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   T8 SERDES TX swing and de-emphasis levels
*
* ELEMENTS:
*   tx_dpeni1    - First pre-emphasis driver stages state.
*   tx_dpeni2    - Second pre-emphasis driver stages state.
*   deni         - output driver stages state.
*   tx_psel      - TX slave selector.
*******************************************************************************/
typedef struct {
    UINT8                   tx_dpeni1;
    UINT8                   tx_dpeni2;
    UINT32                  deni;     
    UINT32                  tx_psel;
} t8_tx_swing_levels_t;

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
#define T8_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOG_ERR_NULL_HANDLE,                "T8: NULL Handle") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOG_ERR_INVALID_PARAMETERS,         "T8: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOG_ERR_SERDES_ID_INVALID,          "T8: Serdes ID invalid: Serdes ID should be < 16" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOG_ERR_MODE_NOT_FOUND,             "T8: Cannot convert util pattern mode to t8 pattern mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOG_ERR_USR_PATT_SIZE_INVALID,      "T8: User-defined pattern size is invalid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_PATT_GEN_FACILITY_LOOPBACK_CONFLICT,"T8: Conflict between pattern generator and facility loopback" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOOPBACK_ALREADY_CONFIGURED,        "T8: Loopback already configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_LOOPBACK_ALREADY_NOT_CONFIGURED,    "T8: Loopback already not configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_ERR_CONTEXT_INCOHERENT,             "T8: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_ERR_CONTEXT_REG_INCOHERENT,         "T8: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_ERR_CONTEXT_CLEANUP,                "T8: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(T8_STATE_NOT_RESET,                    "T8: Port is not in reset state" ) \
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum T8_LOG_ERR_INDEX
{
    T8_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_T8_SERDES << 16),
    T8_LOG_ERR_TABLE
    T8_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char T8_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char T8_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

    
    
#ifdef __cplusplus
}
#endif

#endif /* _T8_API_H */

/*
** end of file
*/
