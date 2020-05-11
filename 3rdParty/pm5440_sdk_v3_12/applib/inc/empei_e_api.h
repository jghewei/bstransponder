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

#ifndef _EMPEI_E_API_H
#define _EMPEI_E_API_H

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

typedef struct empei_e_handle_struct_t empei_e_handle_t; /*!< Declare empei_e handle */


/*******************************************************************************
* STRUCTURE: empei_e_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EMPEI_E level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   sys_q_overflow_int_i;
    BOOL8   intl_q_overflow_int_i;
    BOOL8   sys_q_underrun_int_i;
    BOOL8   ins_q_underrun_int_i;
    BOOL8   sys_q_resync_int_i;
    BOOL8   ins_q_resync_int_i;
    BOOL8   ext_q_resync_int_i;
    BOOL8   intl_q_resync_int_i;
    
    BOOL8   sys_q_overflow_int_v;
    BOOL8   intl_q_overflow_int_v;
    BOOL8   sys_q_underrun_int_v;
    BOOL8   ins_q_underrun_int_v;
} empei_e_int_chnl_t;



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
#define EMPEI_E_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_LOG_CODE_DEBUG,              "EMPEI_E: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_ERR_CODE_ASSERT,             "EMPEI_E: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_LOG_CODE_LOG,                "EMPEI_E: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_ERR_POLL_TIMEOUT,            "EMPEI_E: Error Polling Timeout" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_LOG_ERR_INVALID_PARAMETERS,  "EMPEI_E: Err invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_LOG_ERR_NO_AVAIL_EXT_PMON,   "EMPEI_E: No available EXT PMON" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_LOG_ERR_EXT_PMON_NOT_FOUND,  "EMPEI_E: EXT PMON not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMPEI_E_LOG_ERR_PKT_GEN_BUSY,        "EMPEI_E: Packet Generator busy" ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum EMPEI_E_LOG_ERR_INDEX
{
    EMPEI_E_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_EMPEI_E << 16),
    EMPEI_E_LOG_ERR_TABLE
    EMPEI_E_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char EMPEI_E_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char EMPEI_E_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _EMPEI_E_API_H */

/*
** end of file
*/
