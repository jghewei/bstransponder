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

#ifndef _EPMM_API_H
#define _EPMM_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "exco3m_api.h"
#include "exco4m_api.h"
#include "gsup43_c73_api.h"
#include "mpmo_api.h"
#include "mpma_api.h"

/*
** Enumerated Types
*/

/* Data rate */
/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
typedef struct epmm_handle_struct_t epmm_handle_t;  /*!< Declare epmm handle */

/*******************************************************************************
* STRUCTURE: epmm_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EPMM level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8                   exco3m_valid;
    exco3m_int_chnl_t       exco3m;
    BOOL8                   exco4m_valid;
    exco4m_int_chnl_t       exco4m;
    BOOL8                   gsup43_c73_valid;
    gsup43_c73_int_chnl_t   gsup43_c73;
    mpma_int_chnl_t         mpma;
    mpmo_int_chnl_t         mpmo; 
} epmm_int_chnl_t;


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
#define EPMM_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EPMM_LOG_CODE_DEBUG,    "EPMM: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EPMM_ERR_CODE_ASSERT,   "EPMM: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EPMM_ERR_INVALID_MODE,  "EPMM: Err invalid mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EPMM_LOG_CODE_LOG,      "EPMM: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EPMM_ERR_INVALID_PARAMETERS,  "EPMM: Err invalid parameters" )

/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum EPMM_LOG_ERR_INDEX
{
    EPMM_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_EPMM << 16),
    EPMM_LOG_ERR_TABLE
    EPMM_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char EPMM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char EPMM_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _EPMM_API_H */

/*
** end of file
*/
