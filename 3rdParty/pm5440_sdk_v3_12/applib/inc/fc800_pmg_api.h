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

#ifndef _FC800_PMG_API_H
#define _FC800_PMG_API_H

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

/*******************************************************************************
* ENUM: fc800_pmg_dlolb_gen_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in fc800_pmg_dlolb_gen_t 
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    FC800_PMG_DLOLB_GEN_MASK_LCV        = 0x00000001,
    FC800_PMG_DLOLB_GEN_MASK_XDET       = 0x00000002,
    FC800_PMG_DLOLB_GEN_MASK_ASD        = 0x00000004,
    FC800_PMG_DLOLB_GEN_MASK_LOSS_SYNC  = 0x00000008,
    FC800_PMG_DLOLB_GEN_MASK_DENSITY    = 0x00000010,
} fc800_pmg_dlolb_gen_mask_t;

/*******************************************************************************
* STRUCTURE: fc800_pmg_dlolb_gen_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring defects that affect DLOLB generation
*
* ELEMENTS:
*   mask        - fc800_pmg_dlolb_gen_mask_t bitmask to indicate config to set
*
*   lcv         - Generate DLOLB when line code violation defect detected.
*
*   xdet        - Generate DLOLB when transition detect defect detected.
*
*   asd         - Generate DLOLB when analog loss of signal defect detected.
*
*   loss_sync   - Generate DLOLB when RX loss sync detected.
*
*   density     - Generate DLOLB when comma density defect detected.

*
*******************************************************************************/
typedef struct {
    UINT32  mask;
    BOOL    lcv;
    BOOL    xdet;
    BOOL    asd;
    BOOL    loss_sync;
    BOOL    density;
} fc800_pmg_dlolb_gen_t;


/*******************************************************************************
* ENUM: fc800_pmg_pattern_ins_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   FC800_PMG pattern insertion configuration
*
* ELEMENTS:
*   FC800_PMG_PATTERN_INS_CFG_USER_PATT  - 32-bit configured user pattern with
*                                          digi_cbr_fc800_user_pattern_set()
*   FC800_PMG_PATTERN_INS_CFG_NOS_PN11   - NOS for FC800 traffic or
*                                          PN11 for CBR traffic
*
*******************************************************************************/
typedef enum {
    FC800_PMG_PATTERN_INS_CFG_USER_PATT = 0,
    FC800_PMG_PATTERN_INS_CFG_NOS_PN11  = 1,

    LAST_FC800_PMG_PATTERN_INS_CFG,
} fc800_pmg_pattern_ins_cfg_t;


/*******************************************************************************
* STRUCTURE: fc800_pmg_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   FC800_PMG level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   rx_disp_invert_i;
    BOOL8   rx_xdet_i;
    BOOL8   rx_sof_det_i;
    BOOL8   rx_dlolb_i;
    BOOL8   rx_asd_i;
    BOOL8   rx_sync_i;
    BOOL8   rx_lcv_i;
    
    BOOL8   rx_disp_invert_v;
    BOOL8   rx_xdet_v;
    BOOL8   rx_dlolb_v;
    BOOL8   rx_asd_v;
    BOOL8   rx_sync_v;
    BOOL8   rx_lcv_v;
} fc800_pmg_int_chnl_t;

typedef struct fc800_pmg_handle_struct_t fc800_pmg_handle_t; /*!< Opaque reference to module handle */ 


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
#define FC800_PMG_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC800_PMG_LOG_CODE_DEBUG,    "FC800_PMG: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC800_PMG_ERR_CODE_ASSERT,   "FC800_PMG: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC800_PMG_ERR_NULL_HANDLE,   "FC800_PMG: Null handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC800_PMG_ERR_INVALID_ARG,   "FC800_PMG: Invalid argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC800_PMG_LOG_CODE_LOG,      "FC800_PMG: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(FC800_PMG_ERR_FEATURE_REQ_FC800, "FC800_PMG: Feature requires link to be prov for FC800" )

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum FC800_PMG_LOG_ERR_INDEX
{
    FC800_PMG_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_FC800_PMG << 16),
    FC800_PMG_LOG_ERR_TABLE
    FC800_PMG_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char FC800_PMG_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char FC800_PMG_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _FC800_PMG_API_H */

/*
** end of file
*/
