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

#ifndef _ODUJAT_API_H
#define _ODUJAT_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "cfc_api.h"
#include "oduksc_api.h"


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

typedef struct odujat_handle_struct_t odujat_handle_t;  /*!< Opaque reference to module handle see odujat_handle_struct_t*/


/*******************************************************************************
* STRUCTURE: odujat_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUJAT level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    cfc_int_chnl_t      cfc;
} odujat_int_chnl_t;

/*******************************************************************************
* STRUCTURE: odujat_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUJAT level table for unchannelized interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8          gain_ovf_i;
    BOOL8          gain_undf_i;
    BOOL8          dq_limit_i;
} odujat_int_t;

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
#define ODUJAT_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_LOG_CODE_DEBUG,                           "ODUJAT: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_ERR_CODE_ASSERT,                          "ODUJAT: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_LOG_CODE_LOG,                             "ODUJAT: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_ERR_INVALID_PTR,                          "ODUJAT: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_ERR_INVALID_ARGS,                         "ODUJAT: Invalid arguments" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_ERR_DIV_BY_ZERO,                          "ODUJAT: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_CHNL_STATE_NOT_START,                     "ODUJAT: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_CHNL_STATE_NOT_PROVISIONED,               "ODUJAT: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_CHNL_STATE_NOT_ACTIVATED,                 "ODUJAT: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_CHNL_CALDENDAR_ENTRY_MISMATCH,            "ODUJAT: Calendar entries for HO channel do not match" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODUJAT_ERR_DB_ENTRY_NOT_FOUND,                   "ODUJAT: DB entry not found" )

   
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODUJAT_LOG_ERR_INDEX
{
    ODUJAT_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODUJAT << 16),
    ODUJAT_LOG_ERR_TABLE
    ODUJAT_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODUJAT_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODUJAT_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _ODUJAT_API_H */

/*
** end of file
*/
