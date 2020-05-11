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

#ifndef _MGMT_FEGE_API_H
#define _MGMT_FEGE_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "enet_fege_api.h"
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
* STRUCTURE: mgmt_fege_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct mgmt_fege_handle_struct_t mgmt_fege_handle_t;


/*******************************************************************************
* STRUCTURE: mgmt_fege_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MGMT FEGE table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    enet_fege_int_t enet_fege;
    BOOL8   tmvp_fifo_overflow_i;
    BOOL8   tmvp_invalid_dpi_id_i;
    BOOL8   tmvp_invalid_type_i;
    BOOL8   rmvp_fifo_overflow_i;
    BOOL8   rmvp_invalid_vlan_id_i;
    BOOL8   rmvp_invalid_type_i;
} mgmt_fege_int_t;


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
#define MGMT_FEGE_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_CODE_DEBUG,                   "MGMT_FEGE: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_ERR_CODE_ASSERT,                  "MGMT_FEGE: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_CODE_LOG,                     "MGMT_FEGE: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_NULL_HANDLE,              "MGMT_FEGE: NULL Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_INVALID_PARAMETERS,       "MGMT_FEGE: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_VLAN_ID_ALREADY_ASSIGNED, "MGMT_FEGE: VLAN ID already assigned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_VLAN_NOT_FOUND,           "MGMT_FEGE: VLAN ID not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_DPI_CHNL_ALREADY_PROV,    "MGMT_FEGE: DPI channel already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_DPI_CHNL_NOT_RPOV,        "MGMT_FEGE: DPI channel not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_LOG_ERR_UNKNOWN,                  "MGMT_FEGE: Unknown error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_STATE_NOT_START,                  "MGMT_FEGE: Instance is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_STATE_NOT_PROVISIONED,            "MGMT_FEGE: Instance is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_STATE_NOT_OPERATIONNAL,           "MGMT_FEGE: Instance is not in operational state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_STATE_ALREADY_START,              "MGMT_FEGE: Instance is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_STATE_ALREADY_PROVISIONED,        "MGMT_FEGE: Instance is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_STATE_UNKOWN,                     "MGMT_FEGE: Unkown state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_CHNL_STILL_CFG,                   "MGMT_FEGE: Instance is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_ERR_CONTEXT_INCOHERENT,           "MGMT_FEGE: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_ERR_CONTEXT_REG_INCOHERENT,       "MGMT_FEGE: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MGMT_FEGE_ERR_CONTEXT_CLEANUP,              "MGMT_FEGE: Context cleanup is required" )\


/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum MGMT_FEGE_LOG_ERR_INDEX
{
    MGMT_FEGE_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_MGMT_FEGE << 16),
    MGMT_FEGE_LOG_ERR_TABLE
    MGMT_FEGE_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char MGMT_FEGE_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char MGMT_FEGE_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _MGMT_FEGE_API_H */

/*
** end of file
*/
