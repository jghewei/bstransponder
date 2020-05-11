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

#ifndef _MPMO_API_H
#define _MPMO_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "cfc_api.h"

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
/*******************************************************************************
* STRUCTURE: mpmo_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMO level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct mpmo_int_chnl_t
{
    cfc_int_chnl_t cfc;

    BOOL8   pkt_drop_replace_i;
    BOOL8   pkt_drop_no_replace_i;
} mpmo_int_chnl_t;

/*******************************************************************************
* STRUCTURE: mpmo_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct mpmo_handle_struct_t mpmo_handle_t;

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
#define MPMO_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_CODE_INVALID_CHANNEL_STATE,    "MPMO: Invalid channel state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_INVALID_PARAMETERS,        "MPMO: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_FRM_PERIOD_NOT_FOUND,      "MPMO: Specified frame period not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_DIVIDED_BY_ZERO,           "MPMO: Divsion by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_FRM_PERIOD_CNT_FULL,       "MPMO: Frame period count is full" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_ASSERT,                    "MPMO: Assertion, Illegal State" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_DB_ENTRY_NOT_FOUNDED,      "MPMO: DB entry not founded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_INVALID_CHNL,              "MPMO: Invalid channel number" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMO_LOG_ERR_DEBUG_ACCESS_FAIL,         "MPMO: Failure in Debug Status Retrieval" ) \


/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum MPMO_LOG_ERR_INDEX
{
    MPMO_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_MPMO << 16),
    MPMO_LOG_ERR_TABLE
    MPMO_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char MPMO_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char MPMO_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _MPMO_API_H */

/*
** end of file
*/
