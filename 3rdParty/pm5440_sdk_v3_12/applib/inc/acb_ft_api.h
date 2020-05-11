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
*     ACB FT block structs, enumeration, constants, erro codes and functions 
*     visible to customer.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ACB_FT_API_H
#define _ACB_FT_API_H

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
#define ACB_FT_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_LOG_CODE_DEBUG,    "ACB_FT: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_ERR_CODE_ASSERT,   "ACB_FT: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_LOG_CODE_LOG,      "ACB_FT: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_ERR_INVALID_PARAMETERS, "ACB_FT: Err invalid parameters") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_CONTEXT_ALREADY_STORED, "ACB_FT: A context is already stored") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_CONTEXT_NOT_AVAILABLE, "ACB_FT: There is no context available to restore") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_STATE_NOT_START, "ACB_FT: It is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_STATE_NOT_EQUIPPED, "ACB_FT: It is not in equipped state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_STATE_NOT_OPERATIONAL, "ACB_FT: It is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ACB_FT_STATE_ALREADY_OPERATIONAL, "ACB_FT: In operational state" ) 

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ACB_FT_LOG_ERR_INDEX
{
    ACB_FT_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ACB_FT << 16),
    ACB_FT_LOG_ERR_TABLE
    ACB_FT_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ACB_FT_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ACB_FT_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/


#ifdef __cplusplus
}
#endif

#endif /* _ACB_FT_API_H */

/*
** end of file
*/
