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

#ifndef _ODTU_MUX_API_H
#define _ODTU_MUX_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "tgmp_api.h"

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

typedef struct odtu_mux_handle_struct_t odtu_mux_handle_t; /*!< ODTU MUX handle type*/

/*******************************************************************************
* STRUCTURE: odtu_mux_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODTU_MUX level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    tgmp_int_chnl_t     tgmp;

    BOOL8               txjc_res_ovrflw_v;
} odtu_mux_int_chnl_t;

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
#define ODTU_MUX_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_LOG_CODE_DEBUG,              "ODTU_MUX: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CODE_ASSERT,             "ODTU_MUX: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_LOG_CODE_LOG,                "ODTU_MUX: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_NULL_HANDLE,             "ODTU_MUX: NULL handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_LOG_ERR_INVALID_ARG,         "ODTU_MUX: Invalid arguments" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_LOG_CODE_HO_TS_FULL,         "ODTU_MUX: Not enough TS available in the HO channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_NUM_ACTIVE_TS_ROLLOVER,  "ODTU_MUX: Number of active tribslots rolled over" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_INVALID_NUM_TS,          "ODTU_MUX: Invalid tributary slot" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CHNL_STATE_NOT_EQUIPPED, "ODTU_MUX: Channel is not equipped" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CHNL_STATE_NOT_START,    "ODTU_MUX: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CHNL_INVALID_STATE,      "ODTU_MUX: Channel is in invalid state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_INVALID_PTR,             "ODTU_MUX: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_INVALID_HO_CYCLE,        "ODTU_MUX: Invalid HO Calendar Cycle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_TS_CYCLE_MISMATCH,       "ODTU_MUX: Invalid cycle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_INVALID_DEPROV,          "ODTU_MUX: Channel cannot be deprovisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_DB_ENTRY_NOT_FOUND,      "ODTU_MUX: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CONTEXT_INCOHERENT,      "ODTU_MUX: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CONTEXT_REG_INCOHERENT,  "ODTU_MUX: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CONTEXT_CLEANUP,         "ODTU_MUX: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_CONTEXT_ENERGY_INCOHERENT, "ODTU_MUX: Register energy states incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_MUX_ERR_POLL_CONFIG_UPDATE_EXPIRY, "ODTU_MUX: CONFIG_UPDATE poll count expiry" )
 
/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODTU_MUX_LOG_ERR_INDEX
{
    ODTU_MUX_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODTU_MUX << 16),
    ODTU_MUX_LOG_ERR_TABLE
    ODTU_MUX_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODTU_MUX_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODTU_MUX_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _ODTU_MUX_API_H */

/*
** end of file
*/
