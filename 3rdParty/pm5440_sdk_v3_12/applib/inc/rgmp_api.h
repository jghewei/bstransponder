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

#ifndef _RGMP_API_H
#define _RGMP_API_H

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

typedef struct rgmp_handle_struct_t rgmp_handle_t; /*!< RGMP handle type */

/*******************************************************************************
* STRUCTURE: rgmp_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RGMP level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8 dplm_i;
    BOOL8 csf_i;
    BOOL8 cm_out_limit_i;
    BOOL8 cnd_max_i;
    BOOL8 jc_err_syncloss_i;
    BOOL8 cm_cnd_crc_err_i;
    BOOL8 pserv_early_i;
    BOOL8 pserv_late_i;
    BOOL8 ssf_sf_rcvd_i;
    BOOL8 dplm_v;
    BOOL8 csf_v;
    BOOL8 ssf_sf_rcvd_v;
} rgmp_int_chnl_t;


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
#define RGMP_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_LOG_CODE_DEBUG,                           "RGMP: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_ERR_CODE_ASSERT,                          "RGMP: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_LOG_CODE_LOG,                             "RGMP: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_CHNL_STATE_NOT_START,                     "RGMP: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_CHNL_STATE_NOT_EQUIPPED,                  "RGMP: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_CHNL_STATE_NOT_OPERATIONAL,               "RGMP: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_ERR_INVALID_PARAMETERS,                   "RGMP: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_ERR_INVALID_PTR,                          "RGMP: RGMP handle is not valid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_ERR_CHNL_DEPROV,                          "RGMP: RGMP channel deprovisioning error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(RGMP_ERR_DB_ENTRY_NOT_FOUND,                   "RGMP: DB entry not found" )     

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum RGMP_LOG_ERR_INDEX
{
    RGMP_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_RGMP << 16),
    RGMP_LOG_ERR_TABLE
    RGMP_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char RGMP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char RGMP_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _RGMP_API_H */

/*
** end of file
*/
