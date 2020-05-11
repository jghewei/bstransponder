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

#ifndef _CFC_API_H
#define _CFC_API_H

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
* STRUCTURE: cfc_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CFC level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   fifo_ovr_i;
    BOOL8   fifo_udr_i;
    BOOL8   ipt_err_i;
} cfc_int_chnl_t;


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
#define CFC_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_LOG_CODE_DEBUG,    "CFC: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_CODE_ASSERT,   "CFC: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_LOG_CODE_LOG,      "CFC: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_INVALID_PARAMETERS, "CFC: Err invalid parameters") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_END_ADDR_UPDATE_INCOMPLETE, "CFC: End address update incomplete") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_FIFO_PAGES_UNAVAILABLE, "CFC: FIFO pages unavailable") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_DECREASE_GREATER_FIFO_SIZE, "CFC: FIFO decrease Greater than FIFO size") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_FIFO_ALREADY_EXSITS, "CFC: FIFO already exists") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_ERR_FIFO_NOT_BUILT, "CFC: FIFO not built") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CFC_POLL_TIMEOUT, "CFC: poll time out")

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum CFC_LOG_ERR_INDEX
{
    CFC_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_CFC << 16),
    CFC_LOG_ERR_TABLE
    CFC_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char CFC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char CFC_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/


#ifdef __cplusplus
}
#endif

#endif /* _CFC_API_H */

/*
** end of file
*/
