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
*     C8 serdes block API file.
*   NOTES:
*
*******************************************************************************/

#ifndef _C8_API_H
#define _C8_API_H

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

/* C8_USR_PATT_BIT_SIZE: C8 serdes user-defined pattern size */
#define C8_USR_PATT_BIT_SIZE 40

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/* This is the config struct for initial values. */

/*******************************************************************************
* STRUCTURE: c8_tx_swing_levels_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   C8 SERDES TX swing and de-emphasis levels
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
} c8_tx_swing_levels_t;

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
#define C8_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOG_ERR_NULL_HANDLE,                "C8: NULL Handle") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOG_ERR_INVALID_PARAMETERS,         "C8: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOG_ERR_SERDES_ID_INVALID,          "C8: Serdes ID invalid: Serdes ID should be < 16" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOG_ERR_MODE_NOT_FOUND,             "C8: Cannot convert util pattern mode to c8 pattern mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOG_ERR_USR_PATT_SIZE_INVALID,      "C8: User-defined pattern size is invlaid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOOPBACK_ALREADY_CONFIGURED,        "C8: Loopback already configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_LOOPBACK_ALREADY_NOT_CONFIGURED,    "C8: Loopback already not configured" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_ERR_CONTEXT_REG_INCOHERENT,         "C8: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_ERR_CONTEXT_CLEANUP,                "C8: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(C8_STATE_NOT_RESET,                    "C8: Port is not in reset state" ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum C8_LOG_ERR_INDEX
{
    C8_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_C8_SERDES << 16),
    C8_LOG_ERR_TABLE
    C8_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char C8_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char C8_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

    
    
#ifdef __cplusplus
}
#endif

#endif /* _C8_API_H */

/*
** end of file
*/
