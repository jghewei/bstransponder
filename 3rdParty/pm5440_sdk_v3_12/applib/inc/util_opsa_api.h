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

#ifndef _UTIL_OPSA_API_H
#define _UTIL_OPSA_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: util_opsa_pkt_period_calc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated methods for calculating packet period and size.
*
* ELEMENTS:
*   UTIL_OPSA_EPS_MIN           - Minimum epsilon.
*   UTIL_OPSA_INT_PKT_PER       - Integer packet period.
*   UTIL_OPSA_USE_OIF_128_BYTE  - OIF defined 128-byte fabric class
*   UTIL_OPSA_USE_OIF_256_BYTE  - OIF defined 256-byte fabric class.
*   UTIL_OPSA_USE_OIF_512_BYTE  - OIF defined 512-byte fabric class.
*
*******************************************************************************/
/*  */
typedef enum {
    UTIL_OPSA_EPS_MIN     = 0,
    UTIL_OPSA_INT_PKT_PER = 1,  
    /* OIF 2011.052.05 recommendations for packet period and sizes */
    UTIL_OPSA_USE_OIF_128_BYTE = 2, 
    UTIL_OPSA_USE_OIF_256_BYTE = 3,
    UTIL_OPSA_USE_OIF_512_BYTE = 4   
} util_opsa_pkt_period_calc_t;

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/** Opaque reference to module handle */
typedef struct util_opsa_handle_struct_t util_opsa_handle_t;


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
#define UTIL_OPSA_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_OPSA_LOG_ERR_INVALID_PARAMETERS,          "UTIL_OPSA: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_OPSA_LOG_ERR_PKT_SIZE_RANGE_TOO_LARGE,    "UTIL_OPSA: Packet size range too large" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_OPSA_LOG_ERR_NO_VALID_RESULTS,            "UTIL_OPSA: No valid results calculated" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_OPSA_LOG_ERR_DIVIDED_BY_ZERO,             "UTIL_OPSA: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(UTIL_OPSA_ERR_CODE_ASSERT,                     "UTIL_OPSA: Assertion" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum UTIL_OPSA_LOG_ERR_INDEX
{
    UTIL_OPSA_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_UTIL_OPSA << 16),
    UTIL_OPSA_LOG_ERR_TABLE
    UTIL_OPSA_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char UTIL_OPSA_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char UTIL_OPSA_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#ifdef __cplusplus
}
#endif

#endif /* _UTIL_OPSA_API_H */

/*
** end of file
*/
