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

#ifndef _ODUKSC_API_H
#define _ODUKSC_API_H

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

typedef struct oduksc_handle_struct_t oduksc_handle_t;   /*!< Opaque reference to module handle see oduksc_handle_struct_t*/

/*******************************************************************************
* STRUCTURE: oduksc_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUKSC level table for unchannelized interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   acc_undf_i;
    BOOL8   acc_ovf_i;
} oduksc_int_t;

/*******************************************************************************
* STRUCTURE: oduksc_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUKSC level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   src_zone_i;
    BOOL8   ramp_done_i;

    BOOL8   src_zone_v;
} oduksc_int_chnl_t;


/*
** Global variables
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
#define ODUKSC_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_LOG_CODE_DEBUG,                           "ODUKSC: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_ERR_CODE_ASSERT,                          "ODUKSC: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_LOG_CODE_LOG,                             "ODUKSC: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_ERR_INVALID_PARAMETERS,                   "ODUKSC: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_ERR_DIV_BY_ZERO,                          "ODUKSC: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_CHNL_STATE_NOT_START,                     "ODUKSC: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_CHNL_STATE_NOT_PROVISIONED,               "ODUKSC: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_CHNL_STATE_NOT_ACTIVATED,                 "ODUKSC: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_RI_NOT_FOUND,                             "ODUKSC: Ramp Interval not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_RAMP_INTERVAL_ERR,                        "ODUKSC: Ramp Interval Error" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_PAGE_SWAP_REQ_TIMEOUT,                    "ODUKSC: Page Swap Request Timeout" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_INCOHERENT_BMP_CONFIG,                    "ODUKSC: Incoherent BMP configuration" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUKSC_ERR_DB_ENTRY_NOT_FOUND,                   "ODUKSC: DB entry not found" )    
   
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODUKSC_LOG_ERR_INDEX
{
    ODUKSC_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODUKSC << 16),
    ODUKSC_LOG_ERR_TABLE
    ODUKSC_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODUKSC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODUKSC_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _ODUKSC_API_H */

/*
** end of file
*/
