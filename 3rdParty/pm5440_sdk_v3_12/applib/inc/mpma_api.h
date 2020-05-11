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

#ifndef _MPMA_API_H
#define _MPMA_API_H

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
* STRUCTURE: mpma_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMA level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct mpma_int_chnl_t
{
    cfc_int_chnl_t cfc;

    BOOL8   psd_fifo_ovr_i;
    BOOL8   psd_fifo_udr_i;
} mpma_int_chnl_t;


/*******************************************************************************
* STRUCTURE: mpma_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct mpma_handle_struct_t mpma_handle_t;


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
#define MPMA_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMA_LOG_CODE_INVALID_CHANNEL_STATE,      "MPMA: Invalid channel state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMA_LOG_ERR_INVALID_PARAMETERS,          "MPMA: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMA_LOG_ERR_DIVIDED_BY_ZERO,             "MPMA: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMA_LOG_ERR_DB_ENTRY_NOT_FOUND,          "MPMA: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MPMA_ERR_CODE_ASSERT,                     "MPMA: Assertion" ) \


/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum MPMA_LOG_ERR_INDEX
{
    MPMA_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_MPMA << 16),
    MPMA_LOG_ERR_TABLE
    MPMA_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char MPMA_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char MPMA_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#ifdef __cplusplus
}
#endif

#endif /* _MPMA_API_H */

/*
** end of file
*/
