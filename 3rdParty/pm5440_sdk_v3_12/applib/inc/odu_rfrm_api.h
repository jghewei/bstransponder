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
*   DESCRIPTION : This files contains definitions for usage in receive ODU 
*   framer configuration APIs.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ODU_RFRM_API_H
#define _ODU_RFRM_API_H

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
* ENUM: odu_rfrm_int_defect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type listing all PM and PID defects in the RFRM. This is
*   used by the Interrupt to Channel mapping bit array.
*  
*   The bit positions correspond to the DEFECT_INT_STAT register in the ODU_RFRM
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum odu_rfrm_int_defect_t
{
    ODU_RFRM_INT_DEFECT_COMB_DLCK       = 0,
    ODU_RFRM_INT_DEFECT_COMB_DOCI       = 1,
    ODU_RFRM_INT_DEFECT_COMB_DAIS       = 2,
    ODU_RFRM_INT_DEFECT_COMB_DTIM       = 3,
    
    ODU_RFRM_INT_DEFECT_COMB_DDEG       = 4,
    ODU_RFRM_INT_DEFECT_COMB_DBDI       = 5,
    ODU_RFRM_INT_DEFECT_COMB_DLTC       = 6,
    ODU_RFRM_INT_DEFECT_COMB_DIAE       = 7,
    
    ODU_RFRM_INT_DEFECT_COMB_DBIAE      = 8,
    ODU_RFRM_INT_DEFECT_COMB_DLOFLOM    = 9,
    ODU_RFRM_INT_DEFECT_COMB_OOF        = 10,
                                       /* 11 - unused */

    ODU_RFRM_INT_DEFECT_COMB_ATSF       = 12,
    ODU_RFRM_INT_DEFECT_COMB_ATSD       = 13,
    ODU_RFRM_INT_DEFECT_COMB_AAIS       = 14,
    ODU_RFRM_INT_DEFECT_COMB_ABDI       = 15,
   
    LAST_ODU_RFRM_INT_DEFECT,   /* this is for out of range values error handling */
} odu_rfrm_int_defect_t;


/*******************************************************************************
* ENUM: odu_rfrm_pm_tcm_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for monitored connection layer type.
*
*
*******************************************************************************/
typedef enum
{
    ODU_RFRM_MESSAGE_TYPE_PM            = 0,    /* PM Message */
    ODU_RFRM_MESSAGE_TYPE_TCM1          = 1,    /* TCM1 Message */
    ODU_RFRM_MESSAGE_TYPE_TCM2          = 2,    /* TCM2 Message */
    ODU_RFRM_MESSAGE_TYPE_TCM3          = 3,    /* TCM3 Message */
    ODU_RFRM_MESSAGE_TYPE_TCM4          = 4,    /* TCM4 Message */
    ODU_RFRM_MESSAGE_TYPE_TCM5          = 5,    /* TCM5 Message */
    ODU_RFRM_MESSAGE_TYPE_TCM6          = 6,    /* TCM6 Message */
    ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM  = 7,    /* Floating TCM Message */
    ODU_RFRM_MESSAGE_TYPE_PM_NIM        = 8,    /* PM NIM Message */
    ODU_RFRM_MESSAGE_TYPE_DCI           = 9,    /* No parent - Parent is DCI */
    ODU_RFRM_MESSAGE_DONT_CARE          = 10,   /* No message type assigned */

    LAST_ODU_RFRM_MESSAGE_TYPE          = 11,   /* this is for out of range values error handling */
} odu_rfrm_pm_tcm_id_t;

/*******************************************************************************
* ENUM: odu_rfrm_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify ODU RFRM instances
*
* ELEMENTS:
*     ODU_RFRM_STG1      - Stage 1 ODU_RFRM
*     ODU_RFRM_STG2      - Stage 2 ODU_RFRM
*    ODU_RFRM_STG3A     - Stage 3a ODU_RFRM
*    ODU_RFRM_STG3B     - Stage 3b ODU_RFRM
*    ODU_RFRM_STG4      - Stage 4 ODU_RFRM
*    ODU_RFRM_STGN      - Stage N ODU_RFRM
*
*******************************************************************************/
typedef enum
{
    ODU_RFRM_STG1   = 0,
    ODU_RFRM_STG2   = 1,
    ODU_RFRM_STG3A  = 2,
    ODU_RFRM_STG3B  = 3,
    ODU_RFRM_STG4   = 4,
    ODU_RFRM_STGN   = 5,
} odu_rfrm_inst_t;

/*******************************************************************************
* ENUM: odu_rfrm_chnl_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type to store valid operational modes for a channel processed in
*   ODU_RFRM
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
    ODU_RFRM_CHNL_NORMAL_WITH_FAU = 0,
    ODU_RFRM_CHNL_NORMAL_NO_FAU   = 1,
    ODU_RFRM_CHNL_PASSTHRU        = 2,
} odu_rfrm_chnl_mode_t;

/*******************************************************************************
* ENUM: odu_rfrm_pm_tcm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store valid states for PM or a TCM layer in
*   ODU_RFRM
*
* ELEMENTS:
*   ODU_RFRM_PM_TCM_START                   - Default state 
*   ODU_RFRM_PM_TCM_TT_OPERATIONAL          - PM or TCMi layer is operational
*   ODU_RFRM_PM_TCM_TT_TRANSPARENT          - PM or TCMi layer in transparent; unprovisioned
*   ODU_RFRM_PM_TCM_TT_MONITOR              - PM or TCMi layer performing monitor function
*   ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR   - PM or TCMi layer performing NIM function
*   ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED - operational, but TCM OH
*                                                     is not zeroed out (this
*                                                     mode is required for interop
*                                                     with some legacy equipment)
*   ODU_RFRM_PM_TCM_TT_ERASE                - Zeros out the TCM overhead (G.798
*                                             MI_TCM_Extension - erase).
*   ODU_RFRM_PM_TCM_TT_LAST                 - For error checking
*******************************************************************************/
typedef enum
{
    ODU_RFRM_PM_TCM_START                   = 0,
    ODU_RFRM_PM_TCM_TT_OPERATIONAL          = 1,
    ODU_RFRM_PM_TCM_TT_TRANSPARENT          = 2,
    ODU_RFRM_PM_TCM_TT_MONITOR              = 3,
    ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR   = 4,
    ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED = 5,
    ODU_RFRM_PM_TCM_TT_ERASE                = 6,

    ODU_RFRM_PM_TCM_LAST                 = 7
} odu_rfrm_pm_tcm_mode_t;



/*******************************************************************************
* STRUCTURE: odu_rfrm_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RFRM level channel table for interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{
    BOOL8   comb_dlck_i;
    BOOL8   comb_doci_i;
    BOOL8   comb_dais_i;
    BOOL8   comb_dtim_i;
    BOOL8   comb_ddeg_i;
    BOOL8   comb_dbdi_i;
    BOOL8   comb_dltc_i;
    BOOL8   comb_diae_i;
    BOOL8   comb_dbiae_i;
    BOOL8   comb_dloflom_i;
    BOOL8   comb_oof_i;
    BOOL8   comb_atsf_i;
    BOOL8   comb_atsd_i;
    BOOL8   comb_aais_i;
    BOOL8   comb_abdi_i;

} odu_rfrm_int_t;


/*******************************************************************************
* STRUCTURE: odu_rfrm_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RFRM level channel table for interrupts.
*
*   Note: Enabling of the following defects must be enabled per framer (HO/MO/LO)
*         using digi_otn_odu_int_defect_enable() and digi_otn_odu_int_pm_chnl_enable().
*         Individual channel control is not available.
*         Affected defects:
*           dlck_pm_i
*           doci_pm_i
*           dais_pm_i
*           dtim_pm_i
*           ddeg_pm_i
*           dbdi_pm_i
*           oof_i
*           dloflom_i
*           atsf_pm_i
*           atsd_pm_i
*           abdi_pm_i
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{
    BOOL8   ac_aps_i;

    /* PM */
    BOOL8   dlck_pm_i;
    BOOL8   doci_pm_i;
    BOOL8   dais_pm_i;
    BOOL8   dtim_pm_i;
    BOOL8   ddeg_pm_i;
    BOOL8   ddeg_enh_pm_i;
    BOOL8   ddeg_bursty_pm_i;
    BOOL8   dbdi_pm_i;
    BOOL8   oof_i;
    BOOL8   dloflom_i;
    BOOL8   atsf_pm_i;
    BOOL8   atsd_pm_i;
    BOOL8   abdi_pm_i;
    
    BOOL8   dlck_pm_v;
    BOOL8   doci_pm_v;
    BOOL8   dais_pm_v;
    BOOL8   dtim_pm_v;
    BOOL8   ddeg_pm_v;
    BOOL8   ddeg_enh_pm_v;
    BOOL8   ddeg_bursty_pm_v;
    BOOL8   dbdi_pm_v;
    BOOL8   oof_v;
    BOOL8   dloflom_v;
    BOOL8   ci_ssf_pm_v;
    BOOL8   atsf_pm_v;
    BOOL8   atsd_pm_v;
    BOOL8   abdi_pm_v;

} odu_rfrm_int_chnl_t;


/*******************************************************************************
* STRUCTURE: odu_rfrm_int_pid_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RFRM level channel table for TCM PID interrupts.
* 
*   Note: Enabling of the following defects must be enabled per framer (HO/MO/LO)
*         using digi_otn_odu_int_defect_enable() and digi_otn_odu_int_pid_chnl_enable().
*         Individual channel control is not available.
*         Affected defects:
*           dlck_i
*           doci_i
*           dais_i
*           dtim_i
*           ddeg_i
*           dbdi_i
*           dbiae_i
*           dltc_i
*           diae_i
*           atsf_i
*           atsd_i
*           aais_i
*           abdi_i
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct
{

    /* PID[7] */
    BOOL8   dlck_i;
    BOOL8   doci_i;
    BOOL8   dais_i;
    BOOL8   dtim_i;
    BOOL8   ddeg_i;
    BOOL8   ddeg_enh_i;
    BOOL8   ddeg_bursty_i;
    BOOL8   dbdi_i;
    BOOL8   dbiae_i;
    BOOL8   dltc_i;
    BOOL8   diae_i;
    BOOL8   atsf_i;
    BOOL8   atsd_i;
    BOOL8   aais_i;
    BOOL8   abdi_i;
    
    BOOL8   dlck_v;
    BOOL8   doci_v;
    BOOL8   dais_v;
    BOOL8   dtim_v;
    BOOL8   ddeg_v;
    BOOL8   ddeg_enh_v;
    BOOL8   ddeg_bursty_v;
    BOOL8   dbdi_v;
    BOOL8   dbiae_v;
    BOOL8   dltc_v;
    BOOL8   diae_v;
    BOOL8   ci_ssf_v;
    BOOL8   atsf_v;
    BOOL8   atsd_v;
    BOOL8   aais_v;
    BOOL8   abdi_v;

} odu_rfrm_int_pid_chnl_t;

/*******************************************************************************
* STRUCTURE: odu_rfrm_odukp_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COREOTN ODUkP far/near end counters for a PM layer of
*   an ODUk channel.
*   The description of each counter is the register field name.
*
* ELEMENTS:
*    odukp_sk_pn_ds_pm           - PN_DS_PM [20:20]
*    odukp_sk_pn_ebc_pm          - PN_EBC_PM [19:0]
*    odukp_sk_pf_ds_pm           - PF_DS_PM [20:20]
*    odukp_sk_pf_ebc_pm          - PF_EBC_PM [19:0]
*******************************************************************************/
typedef struct odu_rfrm_odukp_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bits2                        :11;
    UINT32 odukp_sk_pn_ds_pm                            :1;
    UINT32 odukp_sk_pn_ebc_pm                           :20;

    UINT32 unused_align_32_bits0                        :11;
    UINT32 odukp_sk_pf_ds_pm                            :1;
    UINT32 odukp_sk_pf_ebc_pm                           :20;
#else
    UINT32 odukp_sk_pn_ebc_pm                           :20;
    UINT32 odukp_sk_pn_ds_pm                            :1;
    UINT32 unused_align_32_bits2                        :11;

    UINT32 odukp_sk_pf_ebc_pm                           :20;
    UINT32 odukp_sk_pf_ds_pm                            :1;
    UINT32 unused_align_32_bits0                        :11;
#endif
 } odu_rfrm_odukp_counters_t;

/*******************************************************************************
* STRUCTURE: odu_rfrm_odukt_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining COREOTN ODUkT far/near end counters for a TCM layer
*   of an ODUk channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*    odukt_sk_piae               - PIAE [21:21]
*    odukt_sk_pn_ds              - PN_DS [20:20]
*    odukt_sk_pn_ebc             - PN_EBC [19:0]
*    odukt_sk_pbiae              - PIAE [21:21]
*    odukt_sk_pf_ds              - PN_DS [20:20]
*    odukt_sk_pf_ebc             - PN_EBC [19:0]
*
*******************************************************************************/
typedef struct odu_rfrm_odukt_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bit1                       :10;
    UINT32 odukt_sk_piae                              :1;
    UINT32 odukt_sk_pn_ds                             :1;
    UINT32 odukt_sk_pn_ebc                            :20;

    UINT32 unused_align_32_bits0                      :10;
    UINT32 odukt_sk_pbiae                             :1;
    UINT32 odukt_sk_pf_ds                             :1;
    UINT32 odukt_sk_pf_ebc                            :20;
#else
    UINT32 odukt_sk_pn_ebc                            :20;
    UINT32 odukt_sk_pn_ds                             :1;
    UINT32 odukt_sk_piae                              :1;
    UINT32 unused_align_32_bit1                       :10;

    UINT32 odukt_sk_pf_ebc                            :20;
    UINT32 odukt_sk_pf_ds                             :1;
    UINT32 odukt_sk_pbiae                             :1;
    UINT32 unused_align_32_bits0                      :10;
#endif
 } odu_rfrm_odukt_counters_t;


/*
** Constants
*/
#define ODU_RFRM_PIDS_PER_CHNL      8
/*#define ODU_RFRM_UNASSIGNED_POOL_ID 0xFFFFFFFF*/
#define ODU_RFRM_UNASSIGNED_POOL_ID 0xFFFF

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct odu_rfrm_handle_struct_t odu_rfrm_handle_t; /*!< ODU_RFRM handle type*/



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
#define ODU_RFRM_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_DEBUG,                      "ODU_RFRM: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_CODE_ASSERT,                     "ODU_RFRM: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_LOG,                        "ODU_RFRM: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_INVALID_PTR,                     "ODU_RFRM: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS,          "ODU_RFRM: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_ENTRY_UNAVAIL,         "ODU_RFRM: Pool entry unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_FULL,                  "ODU_RFRM: Pool is full" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_ENTRY_OUT_OF_RANGE,    "ODU_RFRM: Pool entry out of range" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_ID_MISMATCH,           "ODU_RFRM: Pool id mismatch" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_ID_UNASSIGNED,         "ODU_RFRM: Pool id unassigned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_ID_NOT_FOUND,          "ODU_RFRM: Pool id not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_POOL_ID_DUPLICATE,          "ODU_RFRM: Pool id duplicates" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_LOG_CODE_CHNL_NOT_SCHEDULED,         "ODU_RFRM: Channel is not scheduled" )  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_NO_SW_CONTENT,                   "ODU_RFRM: No SW content" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_LAYER_NOT_FOUND,                 "ODU_RFRM: No layer has been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_LAYER_HIERARCHY_FULL,            "ODU_RFRM: Too many layers have been provisioned on an ODU channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_SW_POOL_CTXT_OUT_OF_SYNC,        "ODU_RFRM: SW context for pool ID is out of sync" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_DCI_CHILD_GET,                   "ODU_RFRM: DCI has more than two children") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_PARENT_LAYER_ERR,                    "ODU_RFRM: Parent layer is not provisioned, or connecting to this layer is not possible") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_INT_INCORRECT_PM_API,            "ODU_RFRM: PM interrupts must be enabled with digi_otn_odu_int_defect_enable and digi_otn_odu_int_pm_chnl_enable.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_INT_INCORRECT_PID_API,           "ODU_RFRM: PID interrupts API must be enabled with digi_otn_odu_int_defect_enable and digi_otn_odu_int_pid_enable.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_ERR_INT_COMB_BITS_NOT_VALID,         "ODU_RFRM: Interrupt summary information not valid. Must call digi_otn_odu_int_start() first.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TCM_PM_PM_NIM_FULL,                           "ODU_RFRM: All TCMs, PMs and PM-NIM are already provisioned") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TCM_PM_PM_NIM_EMPTY,                          "ODU_RFRM: No TCMs, PMs and PM-NIM are provisioned") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TCM_PM_PM_NIM_ALREADY_PROVISIONED,            "ODU_RFRM: Cannot provision this TCM/PM/floating TCM resource because it's already provisioned") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(TCM_PM_PM_NIM_NOT_PROVISIONED,                "ODU_RFRM: Cannot deprovision this TCM/PM/floating TCM resource because it not provisioned") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RFRM_TCM_PM_TIM_DET_INVALID_SET,  "ODU_RFRM: Cannot set TIM_DET_MO bit in not OPERATIONAL") \


/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODU_RFRM_LOG_ERR_INDEX
{
    ODU_RFRM_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODU_RFRM << 16),
    ODU_RFRM_LOG_ERR_TABLE
    ODU_RFRM_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODU_RFRM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODU_RFRM_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _ODU_RFRM_API_H */

/*
** end of file
*/
