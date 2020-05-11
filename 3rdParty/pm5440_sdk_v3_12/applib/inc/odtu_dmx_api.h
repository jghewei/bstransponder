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

#ifndef _ODTU_DMX_API_H
#define _ODTU_DMX_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "util_global.h"
#include "rgmp_api.h"

/*
** Enumerated Types
*/


/*
** Constants
*/
#define ODTU_DMX_NUM_TS_IN_CHNL         80


/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
typedef struct odtu_dmx_handle_struct_t odtu_dmx_handle_t; /*!< ODTU_DMX handle type */


/*******************************************************************************
* STRUCTURE: odtu_dmx_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODTU_DMX level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8               dloomfi_i;

    BOOL8               dloomfi_v;
} odtu_dmx_int_t;


/*******************************************************************************
* STRUCTURE: odtu_dmx_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODTU_DMX level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8               dplm_i;
    BOOL8               dmsim_i;
    BOOL8               ssf_sf_rcvd_i;
    BOOL8               cm_out_limit_i;
    BOOL8               cnd_max_i;
    BOOL8               jc_nomtch_lossync_i;
    BOOL8               cm_cnd_crc_err_i;
    BOOL8               erly_pserv_expry_i;

    BOOL8               dplm_v;
    BOOL8               dmsim_v;
    BOOL8               ssf_sf_rcvd_v;
} odtu_dmx_int_chnl_t;




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
#define ODTU_DMX_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_LOG_CODE_DEBUG,              "ODTU_DMX: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_CODE_ASSERT,             "ODTU_DMX: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_LOG_CODE_LOG,                "ODTU_DMX: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_LOG_ERR_INVALID_PARAMETERS,  "ODTU_DMX: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_LOG_CODE_HO_TS_FULL,         "ODTU_DMX: Not enough TS available in the HO channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_CHNL_STATE_NOT_EQUIPPED,     "ODTU_DMX: Channel is not equipped" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_CHNL_STATE_NOT_START,        "ODTU_DMX: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_CHNL_STATE_INVALID,          "ODTU_DMX: Channel is in invalid state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_INVALID_PTR,             "ODTU_DMX: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_INVALID_HO_CYCLE,        "ODTU_DMX: Invalid cycle for HO channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_TS_CYCLE_MISMATCH,       "ODTU_DMX: Invalid tribslot to cycle mapping" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_INVALID_HO,              "ODTU_DMX: Invalid HO chanenl for switch Cfg" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_DB_ENTRY_NOT_FOUND,      "ODTU_DMX: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_ODUFLEX_NUM_TS_MISMATCH, "ODTU_DMX: Number of TS in ODUFLEX not matching" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_POLL_EXPIRY,                  "ODTU_DMX: Err Poll Expiry" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_CONTEXT_INCOHERENT,           "ODTU_DMX: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_CONTEXT_REG_INCOHERENT,       "ODTU_DMX: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_CONTEXT_CLEANUP,              "ODTU_DMX: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_CONTEXT_ENERGY_INCOHERENT, "ODTU_DMX: Register energy states incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_POLL_CONFIG_UPDATE_EXPIRY, "ODTU_DMX: CONFIG_UPDATE poll count expiry" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODTU_DMX_ERR_POLL_CONFIG_UPDATE_DONE_EXPIRY, "ODTU_DMX: CONFIG_UPDATE_DONE poll count expiry" )
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODTU_DMX_LOG_ERR_INDEX
{
    ODTU_DMX_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODTU_DMX << 16),
    ODTU_DMX_LOG_ERR_TABLE
    ODTU_DMX_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODTU_DMX_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODTU_DMX_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#ifdef __cplusplus
}
#endif

#endif /* _ODTU_DMX_API_H */

/*
** end of file
*/
