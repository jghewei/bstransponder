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

#ifndef _OTU_FRM_API_H
#define _OTU_FRM_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "oduksc_api.h"

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
* STRUCTURE: otu_frm_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Opaque datatype
*******************************************************************************/
typedef struct otu_frm_handle_struct_t otu_frm_handle_t;

/*******************************************************************************
* STRUCTURE: otu_frm_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OTU_FRM level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    oduksc_int_t   oduksc;
    
} otu_frm_int_t;

/*******************************************************************************
* STRUCTURE: otu_frm_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OTU_FRM level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    oduksc_int_chnl_t   oduksc;

    BOOL8 otu_frm_d_lom_i;
    BOOL8 otu_frm_oom_i;
    BOOL8 otu_frm_intlv_rudr_i;
    BOOL8 otu_frm_intlv_wovr_i;
    BOOL8 otu_frm_dintlv_rudr_i;
    BOOL8 otu_frm_dintlv_wovr_i;
    BOOL8 v_fifo_ovr_i;
    BOOL8 v_fifo_udr_i;
    BOOL8 otu_sk_a_ais_i; 
    BOOL8 otu_sk_a_ssd_i;
    BOOL8 otu_sk_a_ssf_i; 
    BOOL8 otu_sk_a_tsf_i;
    BOOL8 otu_sk_a_tsd_i;
    BOOL8 otu_sk_a_biae_i;
    BOOL8 otu_sk_a_bei_i; 
    BOOL8 otu_sk_a_bdi_i; 
    BOOL8 otu_sk_d_iae_i; 
    BOOL8 otu_sk_d_tim_i; 
    BOOL8 otu_sk_d_deg_i; 
    BOOL8 otu_sk_d_deg_enh_i; 
    BOOL8 otu_sk_d_deg_bursty_i; 
    BOOL8 otu_sk_d_bdi_i; 
    BOOL8 otu_sk_d_biae_i;
    BOOL8 otu_sk_ci_ssf_i;
    BOOL8 otu_so_a_iae_i; 
    
    BOOL8 otu_frm_d_lom_v;
    BOOL8 otu_frm_oom_v;  
    BOOL8 otu_frm_intlv_rudr_v;
    BOOL8 otu_frm_intlv_wovr_v;
    BOOL8 otu_frm_dintlv_rudr_v;
    BOOL8 otu_frm_dintlv_wovr_v;
    BOOL8 otu_sk_a_ais_v; 
    BOOL8 otu_sk_a_ssd_v; 
    BOOL8 otu_sk_a_ssf_v; 
    BOOL8 otu_sk_a_tsf_v; 
    BOOL8 otu_sk_a_tsd_v; 
    BOOL8 otu_sk_a_biae_v;
    BOOL8 otu_sk_a_bei_v; 
    BOOL8 otu_sk_a_bdi_v; 
    BOOL8 otu_sk_d_iae_v; 
    BOOL8 otu_sk_d_tim_v; 
    BOOL8 otu_sk_d_deg_v; 
    BOOL8 otu_sk_d_deg_enh_v; 
    BOOL8 otu_sk_d_deg_bursty_v; 
    BOOL8 otu_sk_d_bdi_v; 
    BOOL8 otu_sk_d_biae_v;
    BOOL8 otu_sk_ci_ssf_v;
    
} otu_frm_int_chnl_t;

/*******************************************************************************
* STRUCTURE: otu_frm_counters_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining LINEOTN otu framer far/near end counters for a single channel.
*   The description of each counter is the register field name.
*
*
* ELEMENTS:
*    otu_frm_sk_sm_p_n_ds            - OTU_SK_SM_P_N_DS [24:24]
*    otu_frm_sk_sm_p_n_ebc           - OTU_SK_SM_P_N_EBC [23:0]
*    otu_frm_sk_sm_p_iae             - OTU_SK_SM_P_IAE [25:25]
*    otu_frm_sk_sm_p_f_ds            - OTU_SK_SM_P_F_DS [24:24]
*    otu_frm_sk_sm_p_f_ebc           - OTU_SK_SM_P_F_EBC [23:0]
*    otu_frm_sk_sm_p_biae            - OTU_SK_SM_P_BIAE [25:25]
*
*******************************************************************************/
typedef struct otu_frm_counters_t
 {
#ifdef DIGI_PMON_BITFIELD_MSB_FIRST
    UINT32 unused_align_32_bits0                        :6;     /*!< explicitily pad to 32 bit alignment */
    UINT32 otu_frm_sk_sm_p_iae                          :1;
    UINT32 otu_frm_sk_sm_p_n_ds                         :1;
    UINT32 otu_frm_sk_sm_p_n_ebc                        :24;

    UINT32 unused_align_32_bits1                        :6;     /*!< explicitily pad to 32 bit alignment */
    UINT32 otu_frm_sk_sm_p_biae                         :1;
    UINT32 otu_frm_sk_sm_p_f_ds                         :1;
    UINT32 otu_frm_sk_sm_p_f_ebc                        :24;
#else
    UINT32 otu_frm_sk_sm_p_n_ebc                        :24;
    UINT32 otu_frm_sk_sm_p_n_ds                         :1;
    UINT32 otu_frm_sk_sm_p_iae                          :1;
    UINT32 unused_align_32_bits0                        :6;     /*!< explicitily pad to 32 bit alignment */

    UINT32 otu_frm_sk_sm_p_f_ebc                        :24;
    UINT32 otu_frm_sk_sm_p_f_ds                         :1;
    UINT32 otu_frm_sk_sm_p_biae                         :1;
    UINT32 unused_align_32_bits1                        :6;     /*!< explicitily pad to 32 bit alignment */
#endif
 } otu_frm_counters_t;

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
#define OTU_FRM_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_LOG_CODE_DEBUG,    "OTU_FRM: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_CODE_ASSERT,   "OTU_FRM: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_INVALID_PARAMETERS,"OTU_FRM:Err Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_STREAM_PROV_FAILED,"OTU_FRM: Err stream provisioning failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_STREAM_DEPROV_FAILED,"OTU_FRM: Err stream deprovisioning failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_STREAM_ACT_FAILED,"OTU_FRM: Err stream activation failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_STREAM_DEACT_FAILED,"OTU_FRM: Err stream de-activation failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_FAIL,"OTU_FRM: Err Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_INVALID_PTR,"OTU_FRM: Err Invalid Pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_SO_PASSTHRU_REQUIRED,"OTU_FRM: Err Required SO pass through mode not set" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_SK_PASSTHRU_REQUIRED,"OTU_FRM: Err Required SK pass through mode not set" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_LOG_CODE_LOG,      "OTU_FRM: Err Log logging code" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_ERR_CONTEXT_CLEANUP,                      "OTU_FRM: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(OTU_FRM_DB_ENTRY_NOT_FOUND,      "OTU_FRM: Database entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_DINT_LOG_CODE_DEBUG,    "CBR_DINT: Err Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_DINT_ERR_CODE_ASSERT,   "CBR_DINT: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_DINT_ERR_INVALID_PARAMETERS,"CBR_DINT: Err Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_DINT_ERR_FAIL,"CBR_DINT: Err Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_DINT_ERR_INVALID_PTR,"CBR_DINT: Err Invalid Pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_DINT_ERR_CFG_COLLISION,"CBR_DINT: Err configuration collision" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_INT_LOG_CODE_DEBUG,    "CBR_INT: Err Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_INT_ERR_CODE_ASSERT,   "CBR_INT: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_INT_ERR_INVALID_PARAMETERS,"CBR_INT: Err Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_INT_ERR_FAIL,"CBR_INT: Err Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_INT_ERR_INVALID_PTR,"CBR_INT: Invalid Pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_INT_ERR_CFG_COLLISION,"CBR_INT: Err configuration collision" )



/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum OTU_FRM_LOG_ERR_INDEX
{
    OTU_FRM_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_OTU_FRM << 16),
    OTU_FRM_LOG_ERR_TABLE
    OTU_FRM_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char OTU_FRM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char OTU_FRM_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _OTU_FRM_API_H */

/*
** end of file
*/
