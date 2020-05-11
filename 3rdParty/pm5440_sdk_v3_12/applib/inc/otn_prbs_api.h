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
*       OTN PRBS API
*
*   NOTES:
*
*******************************************************************************/

#ifndef _OTN_PRBS_API_H
#define _OTN_PRBS_API_H

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

/*******************************************************************************
* STRUCTURE: otn_prbs_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct otn_prbs_handle_struct_t otn_prbs_handle_t;

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
#define OTN_PRBS_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_LOG_CODE_DEBUG,                           "OTN_PRBS: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_ERR_CODE_ASSERT,                          "OTN_PRBS: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_LOG_CODE_LOG,                             "OTN_PRBS: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_ERR_INVALID_PTR,                          "OTN_PRBS: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_ERR_INVALID_ARGS,                         "OTN_PRBS: Invalid arguments" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_ERR_DIV_BY_ZERO,                          "OTN_PRBS: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_CHNL_STATE_NOT_START,                     "OTN_PRBS: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_CHNL_STATE_NOT_PROVISIONED,               "OTN_PRBS: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_CHNL_STATE_NOT_ACTIVATED,                 "OTN_PRBS: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTN_PRBS_CHNL_CALDENDAR_ENTRY_MISMATCH,            "OTN_PRBS: Calendar entries for HO channel do not match" ) \

   
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum OTN_PRBS_LOG_ERR_INDEX
{
    OTN_PRBS_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_OTN_PRBS << 16),
    OTN_PRBS_LOG_ERR_TABLE
    OTN_PRBS_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char OTN_PRBS_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char OTN_PRBS_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _OTN_PRBS_API_H */

/*
** end of file
*/
