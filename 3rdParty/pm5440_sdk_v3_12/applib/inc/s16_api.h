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

#ifndef _S16_API_H
#define _S16_API_H

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
/* Maximum number of serdes in block */
#define S16_MAX_NUM_SERDES 16

/* S16 serdes user pattern size */
#define S16_USR_PATT_BIT_SIZE 320

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: s16_tx_swing_levels_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 SERDES TX swing and de-emphasis levels
*
* ELEMENTS:
*   amplitude    - TX launch amplitude for calibration.
*   post_cursor  - TX de-emphasis for calibration.
*   pre_cursor   - TX edge rate for calibration.
*******************************************************************************/
typedef struct
{
    UINT8                   amplitude;
    UINT8                   post_cursor;
    UINT8                   pre_cursor;
} s16_tx_swing_levels_t;


/*******************************************************************************
* STRUCTURE: s16_int_pin_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{
    BOOL8   acb_fail_i;
    BOOL8   acb_fail_v;
} s16_int_pin_t;

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
#define S16_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_NULL_HANDLE,                     "S16: NULL handle") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_INVALID_PARAMETERS,              "S16: Invalid parameters") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_SERDES_ID_INVALID,               "S16: Serdes ID invalid: Serdes ID should be < 16" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_MODE_NOT_FOUND,                  "S16: Cannot convert util pattern mode to s16 pattern mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_CSU_DENOMINATOR_ZERO,                    "S16: CSU Denominator is 0" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_PATT_MON_DISABLED,               "S16: Pattern monitor is disabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_PATT_GEN_FACILITY_LOOPBACK_CONFLICT,     "S16: Conflict between pattern generator and facility loopback" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_FACILITY_LOOPBACK_ACB_NULL_CFG_CONFLICT, "S16: ACB NULL configuration in Facility loopback" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_STATE_NOT_RESET,                          "S16: Port is not in reset state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_STATE_NOT_START,                          "S16: Port is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_STATE_NOT_EQUIPPED,                       "S16: Port is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_STATE_NOT_OPERATIONAL,                    "S16: Port is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_STATE_NOT_OPERATIONAL_LOOPBACK,           "S16: Port is not in activated state (loopback)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_USR_PATT_SIZE_INVALID,            "S16: User-defined pattern size is invlaid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_LOG_ERR_NULL_CTXT,                        "S16: NULL Context") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_CONTEXT_ALREADY_STORED,                   "S16: Context already stored, Cannot push a new context.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_ACB_FT_FORCE_NOMINAL_NOT_SET,             "S16: ACB FT nominal mode not set" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_FACILITY_LOOPBACK_ALREADY_PROV,           "S16: ACB FT nominal mode cannot be set when serdes is already in facilty loopback" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_ACB_FT_CTXT_FIFO_FULL,                    "S16: ACB FT context FIFO is full. Saved configuration will be lost if operation proceed.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_ERR_CONTEXT_INCOHERENT,                   "S16: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_ERR_CONTEXT_REG_INCOHERENT,               "S16: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_ERR_CONTEXT_CLEANUP,                      "S16: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(S16_STATE_IN_RESET,                           "S16: Port is in reset state" ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum S16_LOG_ERR_INDEX
{
    S16_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_S16_SERDES << 16),
    S16_LOG_ERR_TABLE
    S16_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char S16_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char S16_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _S16_API_H */

/*
** end of file
*/
