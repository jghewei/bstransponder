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

#ifndef _EXPAND_IBUF_API_H
#define _EXPAND_IBUF_API_H

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

typedef struct expand_ibuf_handle_struct_t expand_ibuf_handle_t; /*!< Opaque reference to module handle see expand_ibuf_handle_struct_t*/


/*******************************************************************************
* STRUCTURE: expand_ibuf_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EXPAND IBUF level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    cfc_int_chnl_t  cfc;
} expand_ibuf_int_chnl_t;

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
#define EXPAND_IBUF_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXPAND_IBUF_LOG_CODE_DEBUG,             "EXPAND_IBUF: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXPAND_IBUF_ERR_CODE_ASSERT,            "EXPAND_IBUF: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXPAND_IBUF_LOG_CODE_LOG,               "EXPAND_IBUF: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXPAND_IBUF_ERR_NO_SW_CONTENT,          "EXPAND_IBUF: Configuration Not Coded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXPAND_IBUF_LOG_ERR_INVALID_PARAMETERS, "EXPAND_IBUF: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EXPAND_IBUF_LOG_ERR_INVALID_PTR,        "EXPAND_IBUF: Invalid pointer" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum EXPAND_IBUF_LOG_ERR_INDEX
{
    EXPAND_IBUF_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_EXPAND_IBUF << 16),
    EXPAND_IBUF_LOG_ERR_TABLE
    EXPAND_IBUF_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char EXPAND_IBUF_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char EXPAND_IBUF_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _EXPAND_IBUF_API_H */

/*
** end of file
*/
