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

#ifndef _GSUP43_C73_API_H
#define _GSUP43_C73_API_H

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

typedef struct gsup43_c73_handle_struct_t gsup43_c73_handle_t; /*!< Declare gsup43_c73 handle */


/*******************************************************************************
* STRUCTURE: gsup43_c73_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   GSUP43 C73 level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   los_i;
    BOOL8   los_sync_i;
    BOOL8   hi_ber_i;
    BOOL8   link_fail_i;
    BOOL8   rx_fault_i;
    BOOL8   blk_type_err_i;
    BOOL8   rx_prbs31_err_i;
    BOOL8   all_ones_or_zeros_i;
    BOOL8   rx_rf_i;
    BOOL8   rx_lf_i;
    BOOL8   fifo_undrun_i;
    BOOL8   fifo_ovrun_i;
    BOOL8   send_rf_i;
    BOOL8   send_lf_i;
    BOOL8   send_idle_i;
    BOOL8   type_err_i;

    BOOL8   los_v;
    BOOL8   los_sync_v;
    BOOL8   hi_ber_v;
    BOOL8   link_fail_v;
    BOOL8   rx_fault_v;
    BOOL8   all_ones_or_zeros_v;
    BOOL8   rx_rf_v;
    BOOL8   rx_lf_v;
    BOOL8   lf_fault_v;
    BOOL8   rf_fault_v;
    BOOL8   send_rf_v;
    BOOL8   send_lf_v;
    BOOL8   send_idle_v;
} gsup43_c73_int_chnl_t;

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
#define GSUP43_C73_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(GSUP43_C73_LOG_CODE_DEBUG,    "GSUP43_C73: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(GSUP43_C73_ERR_CODE_ASSERT,   "GSUP43_C73: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(GSUP43_C73_LOG_CODE_LOG,      "GSUP43_C73: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(GSUP43_C73_ERR_INVALID_ARGUMENTS,   "GSUP43_C73: Err invalid arguments" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(GSUP43_C73_ERR_CONTEXT_INCOHERENT,   "GSUP43_C73: Context and Register Space Incoherent")

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum GSUP43_C73_LOG_ERR_INDEX
{
    GSUP43_C73_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_GSUP43_C73 << 16),
    GSUP43_C73_LOG_ERR_TABLE
    GSUP43_C73_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char GSUP43_C73_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char GSUP43_C73_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _GSUP43_C73_API_H */

/*
** end of file
*/
