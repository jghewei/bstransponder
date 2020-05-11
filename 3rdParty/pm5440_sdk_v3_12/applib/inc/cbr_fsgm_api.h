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

#ifndef _CBR_FSGM_API_H
#define _CBR_FSGM_API_H

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
* ENUM: cbr_fsgm_pattern_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Fault patterns for insertion.
*
* ELEMENTS:
*   CBR_FSGM_PATTERN_PN11       - PN-11 pattern
*   CBR_FSGM_PATTERN_MS_AIS     - MS-AIS pattern
*   CBR_FSGM_PATTERN_8B_10B     - 8b/10b pattern. Only valid for 10G
*   CBR_FSGM_PATTERN_64B_66B    - 64b/66b pattern. Only valid for 10G
*   LAST_CBR_FSGM_PATTERN       - For out of range error handling.
*
*******************************************************************************/
typedef enum {
    CBR_FSGM_PATTERN_PN11       = 0x0,
    CBR_FSGM_PATTERN_MS_AIS     = 0x1,
    CBR_FSGM_PATTERN_8B_10B     = 0x2,
    CBR_FSGM_PATTERN_64B_66B    = 0x3,
    
    LAST_CBR_FSGM_PATTERN,
} cbr_fsgm_pattern_t;


/*******************************************************************************
* ENUM: cbr_fsgm_user_fault_pattern_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   User fault pattern.
*
* ELEMENTS:
*   CBR_FSGM_USER_FAULT_PATTERN_NONE - No user fault pattern
*
*   CBR_FSGM_USER_FAULT_PATTERN_8B_10B_K30_7_CODE - Encoded GE /V/ fault pattern. 
*       The GE fault pattern, as recommended by clause 7.1.3.5 of ITU-T G.7041,
*       consists of repeating /V/ characters, where /V/ is /K30.7/.
*
*   CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ERR_CODE - 8b/10b error code pattern.
*       As per G7041 clause 8.1.1.1, it consists of repeating (RD+) or (RD-) characters,
*       where (RD-) is b001111 0001.
*
*   CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS - 8b/10b FC NOS pattern.
*       The 8B/10B encoded FC NOS primitive sequence is defined as:
*       /K28.5/D21.2/D31.5/D5.2/
*
*   CBR_FSMG_USER_FAULT_PATTERN_8B_10B - 
*
*   CBR_FSMG_USER_FAULT_PATTERN_64B_66B - Local fault.
*
*   CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE - Local fault for 10GE client
*   CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE - Remote fault for 10GE client
*
*   LAST_CBR_FSGM_USER_FAULT_PATTERN - For out of range error handling.
*
*******************************************************************************/
typedef enum {
    CBR_FSGM_USER_FAULT_PATTERN_NONE = 0,
    CBR_FSGM_USER_FAULT_PATTERN_8B_10B_K30_7_CODE,
    CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ERR_CODE,
    CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS,
    CBR_FSGM_USER_FAULT_PATTERN_8B_10B,
    CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ARB,
    CBR_FSGM_USER_FAULT_PATTERN_64B_66B,
    CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE,
    CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE,
    CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_IDLE,
    CBR_FSGM_USER_FAULT_PATTERN_64B_66B_FC_IDLE,
    
    LAST_CBR_FSGM_USER_FAULT_PATTERN,
} cbr_fsgm_user_fault_pattern_t;




/*******************************************************************************
* ENUM: cbr_fsgm_pattern_cfg_mask_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in cbr_fsgm_pattern_cfg_t
*
* ELEMENTS:
*   See enumeration.
*
*******************************************************************************/
typedef enum {
    CBR_FSGM_PATTERN_CFG_MASK_FAULT_PROP   = 0x1,
    CBR_FSGM_PATTERN_CFG_MASK_LOS_PROP     = 0x2,
    CBR_FSGM_PATTERN_CFG_MASK_FAULT_INSERT = 0x4,
    CBR_FSGM_PATTERN_CFG_MASK_LOS_INSERT   = 0x8,
    
} cbr_fsgm_pattern_cfg_mask_t;




/*******************************************************************************
* STRUCTURE: cbr_fsgm_pattern_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring fault pattern insertion and propagation.
*
* ELEMENTS:
*   mask                 - cbr_fsgm_pattern_cfg_mask_t bitmask to indicate config
*                          to set
*
*   fault_prop           - 1: Input faults are propagated to the output.
*                          0: Input faults are not propagated to the output.
*
*   los_prop             - 1: Input LOS is propagated to the output.
*                          0: Input LOS is not propagated to the output.
*                             los_insert logic determines the output.
*
*   fault_insert         - 1: Output is replaced with the fault pattern upon
*                             input faults.
*                          0: Inputs faults have no effect on the output.
*
*   los_insert           - 1: Output is replaced with the fault pattern upon
*                             input LOS.
*                          0: LOS has no effect on the output stream.
*
*
*******************************************************************************/
typedef struct {
    UINT32  mask;
    BOOL    fault_prop;
    BOOL    los_prop;
    BOOL    fault_insert;
    BOOL    los_insert;

} cbr_fsgm_pattern_cfg_t;



/*******************************************************************************
* ENUM: fc_cbr_patt_ins_cfg_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to config in fc_cbr_patt_ins_cfg_t
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    FC_CBR_PATT_INS_CFG_MASK_1 = 0x00000001,
    FC_CBR_PATT_INS_CFG_MASK_2 = 0x00000002,
    FC_CBR_PATT_INS_CFG_MASK_4 = 0x00000004,
    FC_CBR_PATT_INS_CFG_MASK_8 = 0x00000008,
    FC_CBR_PATT_INS_CFG_MASK_10 = 0x00000010,
    FC_CBR_PATT_INS_CFG_MASK_20 = 0x00000020,
    FC_CBR_PATT_INS_CFG_MASK_40 = 0x00000040,
} fc_cbr_patt_ins_cfg_mask_t;


/*******************************************************************************
* ENUM: fc_cbr_patt_ins_cfg_fault_size_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration options for fault size in fc_cbr_patt_ins_cfg_t.
*   Sets the number of words for Nx8-bit or Nx10-bit pattern insertion.
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    FC_CBR_PATT_INS_CFG_FAULT_SIZE_1    = 0x0,
    FC_CBR_PATT_INS_CFG_FAULT_SIZE_2    = 0x1,
    FC_CBR_PATT_INS_CFG_FAULT_SIZE_4    = 0x2,
    FC_CBR_PATT_INS_CFG_FAULT_SIZE_8    = 0x3,
    FC_CBR_PATT_INS_CFG_FAULT_SIZE_16   = 0x4,
    LAST_FC_CBR_PATT_INS_CFG_FAULT_SIZE,
} fc_cbr_patt_ins_cfg_fault_size_t;



/*******************************************************************************
* ENUM: fc_cbr_patt_ins_cfg_fault_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration options for fault mode in fc_cbr_patt_ins_cfg_t
*   Selects the pattern type for insertion.
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    FC_CBR_PATT_INS_CFG_FAULT_MODE_PN_11            = 0x0,
    FC_CBR_PATT_INS_CFG_FAULT_MODE_PRBS_31          = 0x1,
    FC_CBR_PATT_INS_CFG_FAULT_MODE_64B_66B          = 0x2,
    FC_CBR_PATT_INS_CFG_FAULT_MODE_Nx8BIT_Nx10BIT   = 0x3,

    LAST_FC_CBR_PATT_INS_CFG_FAULT_MODE,
} fc_cbr_patt_ins_cfg_fault_mode_t;


/*******************************************************************************
* ENUM: fc_cbr_patt_ins_cfg_bus_width_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration options for bus width in fc_cbr_patt_ins_cfg_t
*
* ELEMENTS:
*    See enumeration.
*
*******************************************************************************/
typedef enum { 
    FC_CBR_PATT_INS_CFG_BUS_WIDTH_8BIT          = 0x0,
    FC_CBR_PATT_INS_CFG_BUS_WIDTH_10BIT_8B_10B  = 0x1,
    FC_CBR_PATT_INS_CFG_BUS_WIDTH_16BIT         = 0x2,
    FC_CBR_PATT_INS_CFG_BUS_WIDTH_20BIT_8B_10B  = 0x3,
    FC_CBR_PATT_INS_CFG_BUS_WIDTH_32BIT         = 0x4,
    FC_CBR_PATT_INS_CFG_BUS_WIDTH_40BIT_8B_10B  = 0x5, 

    LAST_FC_CBR_PATT_INS_CFG_BUS_WIDTH,
} fc_cbr_patt_ins_cfg_bus_width_t;



/*******************************************************************************
* STRUCTURE: fc_cbr_patt_ins_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring pattern insertion
*   Default configuration is 0 for all fields.
*
* ELEMENTS:
*   mask              - fc_cbr_patt_ins_cfg_mask_t bitmask to indicate config to set
*   fault_size        - see fc_cbr_patt_ins_cfg_fault_size_t
*   fault_mode        - see fc_cbr_patt_ins_cfg_fault_mode_t    
*   bus_width         - see fc_cbr_patt_ins_cfg_bus_width_t     
*   scramble_enable   - Enables scrambling in case of 64B/66B pattern insertion
*                       1: Non-sync bits of the 64B/66B fault and recovery pattern
*                          are scrambled.
*                       0: Disables scrambling
*   pn11_invert       - 1: Enables the inversion of PN11 pattern 
*                       0: Disables inversion 
*
***************************************************************************/
typedef struct {
    UINT32                              mask;
    fc_cbr_patt_ins_cfg_fault_size_t    fault_size;
    fc_cbr_patt_ins_cfg_fault_mode_t    fault_mode;
    fc_cbr_patt_ins_cfg_bus_width_t     bus_width;
    BOOL                                scramble_enable;
    BOOL                                pn11_invert;
} fc_cbr_patt_ins_cfg_t;



/*******************************************************************************
* STRUCTURE: cbr_fsgm_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CBR_FSGM level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    /* CBR_FSGM */
    BOOL8   pn11_det_i;
    BOOL8   pn11_det_v;
    
    
    /* FC_CBR_PATT_INS */
    BOOL8   underrun_i;
    BOOL8   tx_pn11_sync_i;
    BOOL8   rx_pn11_sync_i;
    BOOL8   tx_pn11_err_i;
    BOOL8   rx_pn11_err_i;
    BOOL8   rx_prbs31_err_i;
    BOOL8   rx_prbs31_all_ones_or_zeros_i;
    
    BOOL8   underrun_v;
    BOOL8   tx_pn11_sync_v;
    BOOL8   rx_pn11_sync_v;
    BOOL8   rx_prbs31_all_ones_or_zeros_v;

} cbr_fsgm_int_chnl_t;



/*********************************************************************
* Defines for the USER_FLT_PATT values 
*
#ifdef PMC_INTERNAL
* FC_CBR_PATT_INS Eng Doc (PMC-2110653)
*   K30_7_CODE: 7.2.4   8B/10B Encoded GE /V/ Fault Pattern
*   FC_NOS:     7.2.6   8B/10B Encoded FC NOS Fault Pattern
#endif
***************************************************************************/
#define FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_159_128            0x3eaaa62d
#define FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_127_96             0x8bc16d56
#define FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_95_64              0x2d8bc16a
#define FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_63_32              0xa9d2743e
#define FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_31_0               0x9259d274

#define FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_159_128           0x000002aa
#define FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_127_96            0x00008000
#define FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_95_64             0x00008002
#define FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_63_32             0x78000000
#define FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_31_0              0x00000000

#define FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_159_128   0x000002aa 
#define FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_127_96    0x00008000 
#define FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_95_64     0x00008000 
#define FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_63_32     0x00000000 
#define FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_31_0      0x00000000 

#define FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_159_128   0x000002aa
#define FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_127_96    0x00004000
#define FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_95_64     0x00004000
#define FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_63_32     0x00000000
#define FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_31_0      0x00000000

#define FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_159_128 0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_127_96  0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_95_64   0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_63_32   0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_31_0    0x000001E8

#define FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_159_128   0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_127_96    0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_95_64     0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_63_32     0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_31_0      0x000000F1

#define FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_159_128     0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_127_96      0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_95_64       0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_63_32       0x0000003e
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_31_0        0xaa552a95


#define FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_159_128     0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_127_96      0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_95_64       0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_63_32       0x0000003e
#define FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_31_0        0x8b2ac6b1

#define FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_31_0      0x00000000
#define FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_63_32     0x78000000
#define FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_95_64     0x00000002
#define FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_127_96    0x00000000
#define FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_159_128   0x00000278

#define FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_31_0       0xAA2AAAAA
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_63_32      0x0000003E
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_95_64      0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_127_96     0x00000000
#define FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_159_128    0x00000000

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct cbr_fsgm_handle_struct_t cbr_fsgm_handle_t; /*!< Opaque reference to module handle */ 


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
#define CBR_FSGM_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_LOG_CODE_DEBUG,    "CBR_FSGM: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_ERR_CODE_ASSERT,   "CBR_FSGM: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_ERR_NULL_HANDLE,   "CBR_FSGM: Null handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_ERR_INVALID_INDEX, "CBR_FSGM: Invalid index" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_ERR_INVALID_ARG,   "CBR_FSGM: Invalid arg" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_LOG_CODE_LOG,      "CBR_FSGM: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CBR_FSGM_ERR_INVALID_PATTERN_FOR_40G,   "CBR_FSGM: Pattern not valid for 40G" ) \

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum CBR_FSGM_LOG_ERR_INDEX
{
    CBR_FSGM_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_CBR_FSGM << 16),
    CBR_FSGM_LOG_ERR_TABLE
    CBR_FSGM_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char CBR_FSGM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char CBR_FSGM_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _CBR_FSGM_API_H */

/*
** end of file
*/
