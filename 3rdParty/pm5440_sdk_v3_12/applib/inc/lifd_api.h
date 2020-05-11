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

#ifndef _LIFD_API_H
#define _LIFD_API_H

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
** Global variables
*/


/*
** Log Messaging
*/

/*******************************************************************************
* STRUCTURE: lifd_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct lifd_handle_struct_t lifd_handle_t; /*!< Opaque reference to module handle */

/* 
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums 
 */
#define LIFD_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_LOG_CODE_DEBUG,  "LIFD: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_ERR_CODE_ASSERT, "LIFD: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_LOG_ERR_UNKNOWN, "LIFD: Err unknown error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_LOG_ERR_INVALID_PARAMETERS, "LIFD: Err invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_LOG_CODE_LOG, "LIFD: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_LOG_ERR_NO_MUX, "LIFD: Couldn't find valid mux configuration" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(LIFD_ERR_NO_SW_CONTENT, "LIFD: Temporary return code during development" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum LIFD_LOG_ERR_INDEX
{
    LIFD_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_LIFD << 16),
    LIFD_LOG_ERR_TABLE
    LIFD_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char LIFD_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char LIFD_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _LIFD_API_H */

/*
** end of file
*/
