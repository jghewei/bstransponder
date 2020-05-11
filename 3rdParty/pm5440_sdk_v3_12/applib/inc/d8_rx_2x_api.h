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
*     D8_RX_2X serdes block include file.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _D8_RX_2X_API_H
#define _D8_RX_2X_API_H

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

/* D8 RX 2X slice serdes user pattern size */
#define D8_RX_2X_USR_PATT_BIT_SIZE 40

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: d8_rx_2x_int_pin_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   D8 level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   patt_mon_err_a_i;
    BOOL8   patt_mon_err_b_i;
    BOOL8   calib_done_int_a_i;
    BOOL8   calib_done_int_b_i;
    BOOL8   prog_ref_done_int_a_i;
    BOOL8   prog_ref_done_int_b_i;
    BOOL8   adcoc_fsm_err_int_a_i;
    BOOL8   adcoc_fsm_err_int_b_i;
    BOOL8   pll_lock_gain_int_a_i;
    BOOL8   pll_lock_gain_int_b_i;
    BOOL8   pll_lock_loss_int_a_i;
    BOOL8   pll_lock_loss_int_b_i;
    BOOL8   in_rangen_passes_int_a_i;
    BOOL8   in_rangen_passes_int_b_i;
    BOOL8   in_rangen_fails_int_a_i;
    BOOL8   in_rangen_fails_int_b_i;
    BOOL8   divclk_dead_a_i;
    BOOL8   divclk_dead_b_i;
    BOOL8   sipo_bist_err_a_i;
    BOOL8   sipo_bist_err_b_i;
    BOOL8   sipo_bist_sync_a_i;
    BOOL8   sipo_bist_sync_b_i;
    BOOL8   ram_parity_err_a_i;
    BOOL8   ram_parity_err_b_i;
    BOOL8   adc_bist_done_a_i;
    BOOL8   adc_bist_done_b_i;
    BOOL8   adc_bist_err_a_i;
    BOOL8   adc_bist_err_b_i;
    BOOL8   calib_pass_int_a_i;
    BOOL8   calib_pass_int_b_i;
    BOOL8   los_oc_pass_a_i;
    BOOL8   los_oc_pass_b_i;
    BOOL8   los_oc_fail_a_i;
    BOOL8   los_oc_fail_b_i;
       
    BOOL8   patt_mon_err_a_v;
    BOOL8   patt_mon_err_b_v;
    BOOL8   calib_done_int_a_v;
    BOOL8   calib_done_int_b_v;
    BOOL8   prog_ref_done_int_a_v;
    BOOL8   prog_ref_done_int_b_v;
    BOOL8   adcoc_fsm_err_int_a_v;
    BOOL8   adcoc_fsm_err_int_b_v;
    BOOL8   pll_lock_gain_int_a_v;
    BOOL8   pll_lock_gain_int_b_v;
    BOOL8   pll_lock_loss_int_a_v;
    BOOL8   pll_lock_loss_int_b_v;
    BOOL8   in_rangen_passes_int_a_v;
    BOOL8   in_rangen_passes_int_b_v;
    BOOL8   in_rangen_fails_int_a_v;
    BOOL8   in_rangen_fails_int_b_v;
    BOOL8   divclk_dead_a_v;
    BOOL8   divclk_dead_b_v;
    BOOL8   sipo_bist_err_a_v;
    BOOL8   sipo_bist_err_b_v;
    BOOL8   sipo_bist_sync_a_v;
    BOOL8   sipo_bist_sync_b_v;
    BOOL8   ram_parity_err_a_v;
    BOOL8   ram_parity_err_b_v;
    BOOL8   adc_bist_done_a_v;
    BOOL8   adc_bist_done_b_v;
    BOOL8   adc_bist_err_a_v;
    BOOL8   adc_bist_err_b_v;
    BOOL8   calib_pass_int_a_v;
    BOOL8   calib_pass_int_b_v;
    BOOL8   los_oc_pass_a_v;
    BOOL8   los_oc_pass_b_v;
    BOOL8   los_oc_fail_a_v;
    BOOL8   los_oc_fail_b_v;
} d8_rx_2x_int_pin_t;

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
#define D8_RX_2X_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOG_ERR_NULL_HANDLE,             "D8_2X_TTX: NULL handle") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOG_ERR_INVALID_PARAMETERS,      "D8_RX_2X: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOG_ERR_SERDES_ID_INVALID,       "D8_RX_2X: Serdes ID invalid: Serdes ID should be < 2" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOG_ERR_MODE_NOT_FOUND,          "D8_RX_2X: Cannot convert util pattern mode to d8_rx_2x pattern mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOG_ERR_USR_PATT_SIZE_INVALID,   "D8_RX_2X: User-defined pattern size is invalid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOOPBACK_ALREADY_CONFIGURED,     "D8_RX_2X: Loopback already configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_LOOPBACK_ALREADY_NOT_CONFIGURED, "D8_RX_2X: Loopback already not configured" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_ERR_CONTEXT_INCOHERENT,          "D8_RX_2X: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_ERR_CONTEXT_REG_INCOHERENT,      "D8_RX_2X: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_ERR_CONTEXT_CLEANUP,             "D8_RX_2X: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(D8_RX_2X_ERR_STATE_NOT_RESET,             "D8_TX_2X: D8 serdes not in reset state" )


/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum D8_RX_2X_LOG_ERR_INDEX
{
    D8_RX_2X_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_D8_RX_2X_SERDES << 16),
    D8_RX_2X_LOG_ERR_TABLE
    D8_RX_2X_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char D8_RX_2X_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char D8_RX_2X_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */
    
#ifdef __cplusplus
}
#endif

#endif /* _D8_RX_2X_API_H */

/*
** end of file
*/
