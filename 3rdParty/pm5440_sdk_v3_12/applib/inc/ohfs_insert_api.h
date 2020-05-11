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

#ifndef _OHFS_INSERT_API_H
#define _OHFS_INSERT_API_H

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

typedef struct ohfs_insert_handle_struct_t ohfs_insert_handle_t;  /*!< Opaque reference to module handle see ohfs_insert_handle_struct_t */


/*******************************************************************************
* STRUCTURE: ohfs_insert_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OHFS_INSERT level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    cfc_int_chnl_t  cfc;
} ohfs_insert_int_chnl_t;

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
#define OHFS_INSERT_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OHFS_INSERT_LOG_CODE_DEBUG,               "OHFS_INSERT: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OHFS_INSERT_ERR_CODE_ASSERT,              "OHFS_INSERT: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OHFS_INSERT_LOG_CODE_LOG,                 "OHFS_INSERT: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OHFS_INSERT_ERR_NO_SW_CONTENT,            "OHFS_INSERT: Configuration Not Coded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OHFS_INSERT_LOG_ERR_INVALID_PARAMETERS,   "OHFS_INSERT: Invalid parameters" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum OHFS_INSERT_LOG_ERR_INDEX
{
    OHFS_INSERT_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_OHFS_INSERT << 16),
    OHFS_INSERT_LOG_ERR_TABLE
    OHFS_INSERT_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char OHFS_INSERT_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char OHFS_INSERT_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

/*
** Function Prototypes
*/





#ifdef __cplusplus
}
#endif

#endif /* _OHFS_INSERT_API_H */

/*
** end of file
*/
