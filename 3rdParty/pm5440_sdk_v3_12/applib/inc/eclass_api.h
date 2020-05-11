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

#ifndef _ECLASS_API_H
#define _ECLASS_API_H

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

#define ECLASS_MAX_NUM_VLAN_ID_PER_LINK 6
#define ECLASS_SUPPORTED_PKT_TYPE 22 
#define ECLASS_NUM_SUPPORTED_PKT_PER_LINK ECLASS_MAX_NUM_VLAN_ID_PER_LINK+ECLASS_SUPPORTED_PKT_TYPE

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct eclass_handle_struct_t eclass_handle_t;  /*!< Declare eclass handle */

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
#define ECLASS_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_LOG_CODE_DEBUG,    "ECLASS: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_CODE_ASSERT,   "ECLASS: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_LOG_CODE_LOG,      "ECLASS: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_NULL_HANDLE,      "ECLASS: NULL handle" )     \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_INVALID_PARAMETERS,      "ECLASS: Err invalid parameters" )     \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_EXTRACT_ALREADY_ENABLED,      "ECLASS: Extract config already enabled" )    \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_EXTRACT_ALREADY_DISABLED,      "ECLASS: Extract config already disabled" )     \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_EXTRACT_VLAN_TABLE_FULL,      "ECLASS: Extract VLAN table full" )    \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ECLASS_ERR_EXTRACT_VLAN_NOT_FOUND,      "ECLASS: Extract VLAN not found" )   

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ECLASS_LOG_ERR_INDEX
{
    ECLASS_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ECLASS << 16),
    ECLASS_LOG_ERR_TABLE
    ECLASS_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ECLASS_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ECLASS_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _ECLASS_API_H */

/*
** end of file
*/
