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

#ifndef _TGMP_API_H
#define _TGMP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "ohfs_insert_api.h"
#include "txjc_proc_api.h"
#include "expand_ibuf_api.h"

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

typedef struct tgmp_handle_struct_t tgmp_handle_t;  /*!< TGMP handle type */

/*******************************************************************************
* STRUCTURE: tgmp_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   TGMP level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    ohfs_insert_int_chnl_t  ohfs_insert;
    txjc_proc_int_chnl_t    txjc_proc;
    expand_ibuf_int_chnl_t  expand_ibuf;

    /* interrupts */
    BOOL8          txjc_res_ovrflw_i;

    /* status */
    BOOL8          txjc_res_ovrflw_v;

} tgmp_int_chnl_t;


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
#define TGMP_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_LOG_CODE_DEBUG,                           "TGMP: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_ERR_CODE_ASSERT,                          "TGMP: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_LOG_CODE_LOG,                             "TGMP: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_CHNL_STATE_NOT_START,                     "TGMP: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_CHNL_STATE_NOT_EQUIPPED,                  "TGMP: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_CHNL_STATE_NOT_OPERATIONAL,               "TGMP: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_ERR_INVALID_PTR,                          "TGMP: Invalid Pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_ERR_INVALID_PARAMETERS,                   "TGMP: Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_ERR_DB_ENTRY_NOT_FOUND,                   "TGMP: DB Entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TGMP_ERR_CHNL_DEPROV,                          "TGMP: Error Channel deprovisioning" ) 
    
    

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum TGMP_LOG_ERR_INDEX
{
    TGMP_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_TGMP << 16),
    TGMP_LOG_ERR_TABLE
    TGMP_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char TGMP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char TGMP_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

/*
** Function Prototypes
*/


#ifdef __cplusplus
}
#endif

#endif /* _TGMP_API_H */

/*
** end of file
*/
