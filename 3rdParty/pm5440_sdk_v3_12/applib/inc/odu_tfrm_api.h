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
*   DESCRIPTION : This files contains definitions for usage in transmit ODU 
*   framer configuration APIs.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ODU_TFRM_API_H
#define _ODU_TFRM_API_H

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
/* Maximum number of TCM layers per channel */
#define ODU_TFRM_PIDS_PER_CHNL      8

/* Unassigned POOL identifier */
#define ODU_TFRM_UNASSIGNED_POOL_ID 0xFFFF
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct odu_tfrm_handle_struct_t odu_tfrm_handle_t; /*!< ODU_TFRM handle type*/

/*******************************************************************************
* ENUM: odu_tfrm_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying ODU_TFRM instances
*
* ELEMENTS:
*   ODU_TFRM_STG1      - Stage 1 ODU_TFRM
*   ODU_TFRM_STG2      - Stage 2 ODU_TFRM
*   ODU_TFRM_STG3A     - Stage 3a ODU_TFRM
*   ODU_TFRM_STG3B     - Stage 3b ODU_TFRM
*   ODU_TFRM_STG4      - Stage 4 ODU_TFRM
*   ODU_TFRM_STGN      - Stage N ODU_TFRM
*
*******************************************************************************/
typedef enum
{
    ODU_TFRM_STG1   = 0,
    ODU_TFRM_STG2   = 1,
    ODU_TFRM_STG3A  = 2,
    ODU_TFRM_STG3B  = 3,
    ODU_TFRM_STG4   = 4,
    ODU_TFRM_STGN   = 5,
} odu_tfrm_inst_t;

/*******************************************************************************
* ENUM: odu_tfrm_pm_tcm_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for tandem connection layer type.
*
*
*******************************************************************************/
typedef enum
{
    ODU_TFRM_MESSAGE_TYPE_PM            = 0,    /* PM Message */
    ODU_TFRM_MESSAGE_TYPE_TCM1          = 1,    /* TCM1 Message */
    ODU_TFRM_MESSAGE_TYPE_TCM2          = 2,    /* TCM2 Message */
    ODU_TFRM_MESSAGE_TYPE_TCM3          = 3,    /* TCM3 Message */
    ODU_TFRM_MESSAGE_TYPE_TCM4          = 4,    /* TCM4 Message */
    ODU_TFRM_MESSAGE_TYPE_TCM5          = 5,    /* TCM5 Message */
    ODU_TFRM_MESSAGE_TYPE_TCM6          = 6,    /* TCM6 Message */
    ODU_TFRM_MESSAGE_TYPE_DCI           = 9,    /* No parent - Parent is DCI */
    ODU_TFRM_MESSAGE_DONT_CARE          = 10,    /* No message type assigned */
    
    LAST_ODU_TFRM_MESSAGE_TYPE          = 11,    /* this is for out of range values error handling  */
} odu_tfrm_pm_tcm_id_t;

/*******************************************************************************
* ENUM: odu_tfrm_pm_tcm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store valid states for PM or a TCM layer in
*   ODU_TFRM.
*
* ELEMENTS:
*   ODU_TFRM_PM_TCM_START           - Default state
*   ODU_TFRM_PM_TCM_TT_OPERATIONAL  - PM or TCMi in operational
*   ODU_TFRM_PM_TCM_TT_TRANSPARENT  - PM or TCMi layer in transparent; unprovisioned
*******************************************************************************/
typedef enum
{
    ODU_TFRM_PM_TCM_START                = 0,
    ODU_TFRM_PM_TCM_TT_OPERATIONAL       = 1,
    ODU_TFRM_PM_TCM_TT_TRANSPARENT       = 2,
} odu_tfrm_pm_tcm_mode_t;

/*******************************************************************************
* ENUM: odu_tfrm_oh_ins_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying ODU_TFRM instances
*
* ELEMENTS:
*   ODU_TFRM_OH_INS_UPSTREAM    - Upstream value
*   ODU_TFRM_OH_INS_REGISTER    - Insertion from register
*    ODU_TFRM_OH_INS_RCP         - Insertion from Ring Control Port   
*   ODU_TFRM_OH_INS_EXT_OHP     - External Overhead Port
*   LAST_ODU_TFRM_OH_INS_SRC    - this is for error checking
*******************************************************************************/
typedef enum
{
    ODU_TFRM_OH_INS_UPSTREAM   = 0,
    ODU_TFRM_OH_INS_REGISTER,
    ODU_TFRM_OH_INS_RCP,
    ODU_TFRM_OH_INS_EXT_OHP,
    
    LAST_ODU_TFRM_OH_INS_SRC,
} odu_tfrm_oh_ins_src_t;

/*******************************************************************************
* ENUM: odu_tfrm_tti_oh_ins_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying ODU_TFRM instances
*
* ELEMENTS:
*   ODU_TFRM_TTI_SAPI_INS               - SAPI only
*   ODU_TFRM_TTI_DAPI_INS               - DAPI only
*   ODU_TFRM_TTI_OP_SPEC_INS            - Operator Specific only
*   ODU_TFRM_TTI_SAPI_DAPI_INS          - both SAPI and DAPI
*   ODU_TFRM_TTI_SAPI_OP_SPEC_INS       - both SAPI and Operator Specific
*   ODU_TFRM_TTI_DAPI_OP_SPEC_INS       - both DAPI and Operator Specific
*   ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS  - all SAPI, DAPI and Operator Specific
*******************************************************************************/
typedef enum
{
    ODU_TFRM_TTI_SAPI_INS   = 0,
    ODU_TFRM_TTI_DAPI_INS,
    ODU_TFRM_TTI_OP_SPEC_INS,
    ODU_TFRM_TTI_SAPI_DAPI_INS,
    ODU_TFRM_TTI_SAPI_OP_SPEC_INS,
    ODU_TFRM_TTI_DAPI_OP_SPEC_INS,
    ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS,
    
    LAST_ODU_TFRM_TTI_INS,
} odu_tfrm_tti_oh_ins_t;

/*******************************************************************************
* STRUCTURE: odu_tfrm_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_TFRM level table for unchannelized interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   dm_fifo_udr_i;
    BOOL8   dm_fifo_ovr_i;

} odu_tfrm_int_t;

/*******************************************************************************
* STRUCTURE: odu_tfrm_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_TFRM level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   iae_i;
    BOOL8   dm_pm_i;

    BOOL8   iae_v;
} odu_tfrm_int_chnl_t;


/*******************************************************************************
* STRUCTURE: odu_tfrm_int_pid_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU_TFRM TCM level channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   dm_pid_i;

} odu_tfrm_int_pid_chnl_t;


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
#define ODU_TFRM_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_DEBUG,                      "ODU_TFRM: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_CODE_ASSERT,                     "ODU_TFRM: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_LOG,                        "ODU_TFRM: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_INVALID_PTR,                     "ODU_TFRM: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS,          "ODU_TFRM: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_ENTRY_UNAVAIL,         "ODU_TFRM: Pool entry unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_FULL,                  "ODU_TFRM: Pool is full" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_ENTRY_OUT_OF_RANGE,    "ODU_TFRM: Pool entry out of range" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_ID_MISMATCH,           "ODU_TFRM: Pool id mismatch" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED,         "ODU_TFRM: Pool id unassigned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_ID_NOT_FOUND,          "ODU_TFRM: Pool id not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_LOG_CODE_POOL_ID_DUPLICATE,          "ODU_TFRM: Pool id duplicates" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_DM_POLL_FAILED,                  "ODU_TFRM: DM polling failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_NO_SW_CONTENT,                   "ODU_TFRM: No SW content" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_INCORRECT_OH_INS_SRC,            "ODU_TFRM: Incorrect overhead insertion source" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_PM_LAYER_NOT_PROVISIONED,        "ODU_TFRM: Can't connect to PM parent because it's not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_PM_LAYER_PROVISIONED,            "ODU_TFRM: Can't provision PM layer becase it's already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_DCI_PARENT,                      "ODU_TFRM: Can't connect to DCI parent because the PM layer is provision (provide PM for parent)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_TCM_PROVISIONED,                 "ODU_TFRM: Can't provision TCM layer because it's already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_PM_NOT_PROVISIONED,              "ODU_TFRM: Can't deprovision PM layer because it's not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_TCM_NOT_PROVISIONED,             "ODU_TFRM: Can't deprovision TCM layer because it's not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_ERR_DCI_CHILD,                       "ODU_TFRM: Child of DCI not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_TFRM_PARENT_LAYER_ERR,                    "ODU_TFRM: Parent layer is not provisioned, or connecting to this layer is not possible")




/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODU_TFRM_LOG_ERR_INDEX
{
    ODU_TFRM_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODU_TFRM << 16),
    ODU_TFRM_LOG_ERR_TABLE
    ODU_TFRM_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODU_TFRM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODU_TFRM_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


#ifdef __cplusplus
}
#endif

#endif /* _ODU_TFRM_API_H */

/*
** end of file
*/
