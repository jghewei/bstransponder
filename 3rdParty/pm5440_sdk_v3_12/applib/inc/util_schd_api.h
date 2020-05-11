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

#ifndef _UTIL_SCHD_API_H
#define _UTIL_SCHD_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/



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

/** Opaque reference to module handle */
typedef struct util_schd_handle_struct_t util_schd_handle_t;


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
#define UTIL_SCHD_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_LOG_CODE_DEBUG,     "util_schd: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_CODE_ASSERT,    "util_schd: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_LOG_CODE_LOG,       "util_schd: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_INVALID_ARG,    "util_schd: Err: Invalid Input Argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_DIVIDE_BY_ZERO, "util_schd: Err: Division with 0 divisor" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_VARIABLE_OVERFLOW, "util_schd: Err: Variable Size Exceeded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_UINT_SUBTRACTION_UNDERFLOW, "util_schd: Err: UINT32 subtraction with negative result") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_CALCULATION_FAIL, "util_schd: Err: Calculation Failure") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_INSUFFICIENT_ENTRIES, "util_schd: Err: Not enough calendar entries available") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_NULL_PTR, "util_schd: Err: NULL pointer intended to be processed") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_MEM_POOL_NOT_AVAILABLE,   "util_schd: Memory Pool is already in use. Cannot assign memory" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_MEM_POOL_AVAILABLE,   "util_schd: Memory Pool not in use. No need to release memory" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_INVALID_CH_STATE, "util_schd: Err: Invalid channel state") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_WARM_RESTART_MISMATCH, "util_schd: Warm restart mismatch") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_SCHD_ERR_DB_ENTRY_NOT_FOUND, "util_schd: Err: DB entry not found")



/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum UTIL_SCHD_LOG_ERR_INDEX
{
    UTIL_SCHD_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_UTIL_SCHD << 16),
    UTIL_SCHD_LOG_ERR_TABLE
    UTIL_SCHD_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char UTIL_SCHD_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char UTIL_SCHD_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _UTIL_SCHD_API_H */

/*
** end of file
*/
