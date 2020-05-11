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
*   DESCRIPTION : This file contains the function prototypes for the COREOTN
*   subsystem.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _COREOTN_API_H
#define _COREOTN_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "odu_rfrm_api.h"
#include "odu_tfrm_api.h"
#include "odtu_dmx_api.h"
#include "odtu_mux_api.h"
#include "oduksc_api.h"
#include "odujat_api.h"
#include "mpmo_api.h"
#include "mpma_api.h"
#include "odu_ohp_api.h"
#include "odu_rcp_api.h"

/*
** Constants
*/
#define COREOTN_PIDS_PER_CHNL      ODU_RFRM_PIDS_PER_CHNL
#define COREOTN_NUM_ODU_OHP_INST   ODU_OHP_LAST 

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: coreotn_prbs_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of PRBS
*
* ELEMENTS:
*   COREOTN_FMF1_PRBS    - PRBS Generator and monitor block in FMF1
*   COREOTN_FMF2_PRBS    - PRBS Generator and monitor block in FMF2
*   COREOTN_FO1_PRBS     - PRBS Generator and monitor block in FO1
*   COREOTN_FO2_PRBS     - PRBS Generator and monitor block in FO2
*
*******************************************************************************/
typedef enum
{
    COREOTN_FMF1_PRBS   = 0,
    COREOTN_FMF2_PRBS   = 1,
    COREOTN_FO1_PRBS    = 2,
    COREOTN_FO2_PRBS    = 3,
    LAST_COREOTN_PRBS_INST,
} coreotn_prbs_inst_t;




/*******************************************************************************
* ENUM: coreotn_latch_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated the latch defect mode in COREOTN 
*
* ELEMENTS:
*   COREOTN_LATCH_DEFECT_STG4   - latch defects from stg4
*   COREOTN_LATCH_DEFECT_ODUKP  - latch defects from all RFRMS except stg4 
*   COREOTN_LATCH_DEFECT_ALL    - latch defects form all RFRMs
*
*******************************************************************************/
typedef enum
{
    COREOTN_LATCH_DEFECT_STG4 = 0,
    COREOTN_LATCH_DEFECT_ODUKP,
    COREOTN_LATCH_DEFECT_ALL,    
    LAST_COREOTN_LATCH_DEFECT,
} coreotn_latch_defect_t;


/*******************************************************************************
* ENUM: coreotn_int_filter_blocks_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of blocks to retrieve for COREOTN interrupts
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    COREOTN_INT_FILTER_BLOCKS_OTN_MLD     = 0x1 <<  0,
    COREOTN_INT_FILTER_BLOCKS_OTN_SERVER  = 0x1 <<  1,
    COREOTN_INT_FILTER_BLOCKS_OTN_ODU     = 0x1 <<  2,
    COREOTN_INT_FILTER_BLOCKS_OTN_STG3B   = 0x1 <<  3,
    COREOTN_INT_FILTER_BLOCKS_TFRM        = 0x1 <<  4,
    COREOTN_INT_FILTER_BLOCKS_MPMO        = 0x1 <<  5,
    COREOTN_INT_FILTER_BLOCKS_MPMA        = 0x1 <<  6,
    COREOTN_INT_FILTER_BLOCKS_ODTU_DMX    = 0x1 <<  7,
    COREOTN_INT_FILTER_BLOCKS_ODTU_MUX    = 0x1 <<  8,
    COREOTN_INT_FILTER_BLOCKS_ODUKSC      = 0x1 <<  9,
    COREOTN_INT_FILTER_BLOCKS_ODUJAT      = 0x1 << 10,
    COREOTN_INT_FILTER_BLOCKS_OHP         = 0x1 << 11,
    COREOTN_INT_FILTER_BLOCKS_RCP         = 0x1 << 12,
    COREOTN_INT_FILTER_BLOCKS_PRBS        = 0x1 << 13,
    
    LAST_COREOTN_INT_FILTER_BLOCKS,
} coreotn_int_filter_blocks_t;


/*******************************************************************************
* ENUM: coreotn_maint_sig_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated maintenance signal types.
*
* ELEMENTS:
*  CORE_OTN_NO_MAINT_SIG    - no maintenace signal
*  CORE_OTN_ODUK_AIS        - oduk AIS maintenace signal
*  CORE_OTN_ODUK_LCK        - oduk lock maintenace signal
*  CORE_OTN_ODUK_OCI        - oduk OCI maintenace signal (always performed
*                             at nominal rate)
*  CORE_OTN_ODUK_AIS_NOMINAL - oduk AIS maintenace signal at nominal rate
*  CORE_OTN_ODUK_LCK_NOMINAL - oduk LCK maintenance signal at nominal rate
*  LAST_CORE_OTN_MAINT_SIG - this is for out of range values error handling 
*******************************************************************************/
typedef enum
{
    CORE_OTN_NO_MAINT_SIG = 0,
    CORE_OTN_ODUK_AIS,
    CORE_OTN_ODUK_LCK,
    CORE_OTN_ODUK_OCI,
    CORE_OTN_ODUK_AIS_NOMINAL,
    CORE_OTN_ODUK_LCK_NOMINAL,

    LAST_CORE_OTN_MAINT_SIG    /* this is for out of range values error handling */

}coreotn_maint_sig_type_t;

/*******************************************************************************
* STRUCTURE: coreotn_int_framer_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   COREOTN level framer channel table for interrupts.
*   odtu_dmx field is not valid for Stg3b and FO2.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8                    rfrm_valid; 
    odu_rfrm_int_chnl_t      rfrm;

    BOOL8                    odtu_dmx_valid; 
    odtu_dmx_int_chnl_t      odtu_dmx;

    BOOL8                    tfrm_valid; 
    odu_tfrm_int_chnl_t      tfrm;

    BOOL8                    oduksc_valid; 
    oduksc_int_chnl_t        oduksc;

    BOOL8                    odujat_valid;
    odujat_int_chnl_t        odujat;

    BOOL8                    odtu_mux_valid;
    odtu_mux_int_chnl_t      odtu_mux;

    BOOL8                    mpmo_valid;
    mpmo_int_chnl_t          mpmo;

    BOOL8                    mpma_valid;
    mpma_int_chnl_t          mpma;
} coreotn_int_framer_chnl_t;

/*******************************************************************************
* STRUCTURE: coreotn_int_prbs_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   COREOTN level PRBS channel table for interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   prbs_dlss_i;
    BOOL8   prbs_dlss_v;
} coreotn_int_prbs_chnl_t;


/*******************************************************************************
* STRUCTURE: coreotn_int_pid_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   COREOTN TCM level channel table for interrupts.
*
*   The TCMs, FLOATING_NIM, and PM_NIM elements in rx_tcm_pool_ids and rx_tcm
*   can be indexed using the enumerations from digi_otn_int_tcm_num_t.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    /* RFRM pools */
    UINT32                      rx_tcm_pool_ids[COREOTN_PIDS_PER_CHNL];
    odu_rfrm_int_pid_chnl_t     rx_tcm[COREOTN_PIDS_PER_CHNL];
        
} coreotn_int_pid_chnl_t;

/*******************************************************************************
* STRUCTURE: coreotn_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   COREOTN level channel table for interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    coreotn_int_framer_chnl_t   framer_chnl;

    coreotn_int_pid_chnl_t      pid;

    BOOL8                       prbs_valid;
    coreotn_int_prbs_chnl_t     prbs;

    odu_ohp_int_chnl_t          ohp_chnl[COREOTN_NUM_ODU_OHP_INST];

    odu_ri_rcp_int_chnl_t       ri_rcp_chnl;
} coreotn_int_chnl_t;

/*******************************************************************************
* STRUCTURE: coreotn_int_framer_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   COREOTN level framer table for unchannelized interrupts.
*   odtu_dmx field is not valid for Stg3b and FO2.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    /* TFRM */
    BOOL8                       tfrm_valid; 
    odu_tfrm_int_t              tfrm;

    /* RFRM */
    BOOL8                       rfrm_valid; 
    odu_rfrm_int_t              rfrm;
} coreotn_int_framer_t;


/*******************************************************************************
* STRUCTURE: coreotn_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   COREOTN level table for unchannelized interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    /* DEMUX */
    BOOL8                       fmf1_dmx_valid; 
    odtu_dmx_int_t              fmf1_dmx;

    BOOL8                       fmf2_dmx_valid; 
    odtu_dmx_int_t              fmf2_dmx;

    /* ODUKSC */
    BOOL8                       fmf1_oduksc_valid; 
    oduksc_int_t                fmf1_oduksc;

    BOOL8                       fmf2_oduksc_valid; 
    oduksc_int_t                fmf2_oduksc;

    /* ODUJAT */
    BOOL8                       fmf1_odujat_valid; 
    odujat_int_t                fmf1_odujat;

    BOOL8                       fmf2_odujat_valid; 
    odujat_int_t                fmf2_odujat;

    /* TFRM/RFRM */
    coreotn_int_framer_t        framer_stg1;

    coreotn_int_framer_t        framer_stg2;

    coreotn_int_framer_t        framer_stg3a;

    coreotn_int_framer_t        framer_stg3b;

    coreotn_int_framer_t        framer_stgN;
} coreotn_int_t;

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct coreotn_handle_struct_t coreotn_handle_t; /*!< Opaque reference to module handle */

/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
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
#define COREOTN_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_LOG_CODE_DEBUG,                   "COREOTN: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CODE_ASSERT,                  "COREOTN: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_LOG_CODE_LOG,                     "COREOTN: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_NO_SW_CONTENT,                "COREOTN: Function Not Coded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INVALID_DEVICE_STATE,         "COREOTN: Invalid device state read" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INVALID_ARG,                  "COREOTN: Err Invalid Argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_OOM,                          "COREOTN: Err out of memory" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_NULL_HANDLE,                  "COREOTN: Null handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_DIV_BY_ZERO,                  "COREOTN: Division by 0 attempted" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CHANNEL_NOT_FOUND,            "COREOTN: Channel data not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ODU_STRUCT_INVALID_PTR,       "COREOTN: Invalid ODU STRUCT pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INVALID_PTR,                  "COREOTN: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_NO_SW_CONTEXT,                "COREOTN: No SW context" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_POOL_ID_UNAVAILABLE,          "COREOTN: POOL ID unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_POOL_ID_NOT_FOUND,            "COREOTN: POOL ID not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_POOL_ID_OUT_OF_RANGE,         "COREOTN: POOL ID Out of range" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CHNL_STATE_NOT_START,         "COREOTN: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CHNL_STATE_NOT_EQUIPPED,      "COREOTN: Channel is not equipped" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CHNL_STATE_NOT_OPERATIONAL,   "COREOTN: Channel is not equipped" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CHNL_INVALID_STATE,           "COREOTN: Channel is in invalid state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_DB_ENTRY_NOT_FOUND,           "COREOTN: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ODUKSW_UNSUPPORTED_SCHD_MODE,     "COREOTN: ODUKSW unspported scheduling mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CHNL_DEPROV,                  "COREOTN: Err deprovisioning" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_POLL_EXPIRY,                  "COREOTN: Err Poll Expiry" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_INVALID_TRIBSLOT_MASK,            "COREOTN: Invalid tribslot mask." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_BEI_BDI_SRC_CFG,              "COREOTN: BDI/BEI insertion source not configured properly" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ODU_LEVEL_NOT_VALID,          "COREOTN: Incorrect ODU level. Only HO/MO/LO supported." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ODU_STRUCT_TCM_RSC_FULL,      "COREOTN: Attempting to provision too many layers." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ODU_STRUCT_TCM_CTXT_UPDATE,   "COREOTN: Error updating TCM context in odu struct" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ENH_DDEG_NOT_ENABLED,         "COREOTN: Enhanced dDEG monitoring is not enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CONFLICT_WITH_ENABLED_DDEG,   "COREOTN: Cannot configre hardware ddeg when enhanced or bursty ddeg or both are already configured (vice versa)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_HW_DDEG_NOT_ENABLED,          "COREOTN: Hardware dDEG monitoring is not enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CONTEXT_INCOHERENT,           "COREOTN: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CONTEXT_REG_INCOHERENT,       "COREOTN: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CONTEXT_CLEANUP,              "COREOTN: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_CONTEXT_ENERGY_INCOHERENT,    "COREOTN: Register energy states incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_PRIMARY_UNABLE_TO_DEACTIVATE, "COREOTN: ODUKSW Primary unable to deactivate" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_OHP_PORT_NOT_DEFINED,         "COREOTN: OHP Port not Configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_TRIB_SLOT_NOT_IN_USE,         "COREOTN: Tributary slot is not in use" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ODU_STRUCT_OPU,               "COREOTN: Lowest order OPU OH is not terminated in COREOTN, use digi_mapper_opu API " ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INCORRECT_NUM_LO_CHNL,        "COREOTN: Incorrect number of LO channels " ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INCORRECT_MSI_CONFIG_MODE,    "COREOTN: Incorrect MSI configuration mode " ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_NULL_PAYLOAD_INCOHERENT_CTXT, "COREOTN: Incoherent null payload register setting and SW context" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INVALID_PARAMETERS,           "COREOTN: Err Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_EXCEEDS_DEFAULT_INTERNAL_LATENCY, "COREOTN: The programmable hysteresis buffer latency exceeds the default hysteresis buffer" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INTERNAL_LATENCY_LESS_ONE_PKT_LATENCY, "COREOTN: The programmable hysteresis buffer latency is less than the latency of one packet" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_INVALID_PRBS_CFG_STG3A3B, "COREOTN: No support for PRBS MON/GEN at stage 3a/3b " ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_LOFLOM_ACT_DIS, "COREOTN: LOFLOM_ACT_DIS = 0 is an illegal configuration for RFRMs at the egress of the ODUKSW" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(COREOTN_ERR_ERASE_FUNC_CANNOT_BE_APPLIED_ON_OPERATIONAL_TCMS, "COREOTN: Erase function cannot be applied on operational TCMs" ) \


/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum COREOTN_LOG_ERR_INDEX
{
    COREOTN_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_COREOTN << 16),
    COREOTN_LOG_ERR_TABLE
    COREOTN_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char COREOTN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char COREOTN_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _COREOTN_API_H */

/*
** end of file
*/
