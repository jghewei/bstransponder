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
*   DESCRIPTION : This file contains definitions for C functions for
*   configuring OTN features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_OTN_API_H
#define _DIGI_OTN_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "util_global.h"
#include "odu_struct_api.h"
#include "odu_rcp_api.h"
#include "odu_ohp_api.h"
#include "coreotn_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_otn_dm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN function configuration operation mode.
*
* ELEMENTS:
*   DIGI_OTN_DM_SOURCE        - DIGI acts as a delay measurement source (the
*                               source flips the DMp/DMtx bit and waits
*                               for the value to be received by the sink).
*   DIGI_OTN_DM_FEEDTHROUGH   - DIGI acts as a delay measurement feedthough
*                               (DMp/DMtx value is passed transparently
*                                from source to sink).
*******************************************************************************/
typedef enum
{
    DIGI_OTN_DM_SOURCE         = 0,
    DIGI_OTN_DM_FEEDTHROUGH    = 1,
    
    LAST_DIGI_OTN_DM_MODE              /* this is for error checking */
} digi_otn_dm_mode_t;




/*******************************************************************************
* ENUM: digi_otn_ddeg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for enumerated type for dDEG monitoring mode.
*
* ELEMENTS:
*  DIGI_OTN_HARDWARE_DDEG   - Hardware dDEG will issue dDEG report using 1
*                             second monitoring interval as defined
*                             in G.806.
*
*  DIGI_OTN_ENHANCED_DDEG   - dDEG report will be issued by firmware. The
*                             enhanced dDEG report allows for a 
*                             configurable monitoring interval (10ms, or 100ms)
*
*  DIGI_OTN_ENH_BURSTY_DDEG  - dDEG report will be issued by firmware. The
*                              firmware will issue both the G.806 dDEG 
*                              report (1s interval) and the enhanced dDEG
*                              report configurable to 10/100 ms intervals.
*                              This mode represents FW ENHANCED + BURSTY
*                              dDEG collection.
*   
*******************************************************************************/
typedef enum
{
    DIGI_OTN_HARDWARE_DDEG = 0,
    DIGI_OTN_ENHANCED_DDEG, 
    DIGI_OTN_ENH_BURSTY_DDEG,
    
    LAST_DIGI_OTN_DDEG_MODE        /* this is for error checking */
} digi_otn_ddeg_mode_t;

/*******************************************************************************
* ENUM: digi_otn_ddeg_mon_seg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for dDEG monitoring segment.
*
* ELEMENTS:
*   DIGI_OTN_DDEG_MON_LINE      - dDEG monitoring is performed on COREOTN 
*                                 Stage 1/2/N/3A framers
*   DIGI_OTN_DDEG_MON_CLIENT    - dDEG monitoring is performed on COREOTN STG4
*                                 framer. 
*******************************************************************************/
typedef enum
{
    DIGI_OTN_DDEG_MON_LINE      = 0,
    DIGI_OTN_DDEG_MON_CLIENT    = 1,  
    
    LAST_DIGI_OTN_DDEG_MON_SEG        /* this is for error checking */
} digi_otn_ddeg_mon_seg_t;


/*******************************************************************************
* ENUM: digi_otn_enahnced_ddeg_mon_interval_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN function configuration operation mode.
*
* ELEMENTS:
*   DIGI_OTN_ENHANCED_DDEG_10MS_INTERVAL  - Enhanced dDEG uses 10 ms monitoring
*                                           interval
*   DIGI_OTN_ENHANCED_DDEG_100MS_INTERVAL - Enhanced dDEG uses 100 ms monitoring
*                                           interval
*******************************************************************************/
typedef enum
{
    DIGI_OTN_ENHANCED_DDEG_10MS_INTERVAL  = 0,
    DIGI_OTN_ENHANCED_DDEG_100MS_INTERVAL = 1,
    
    LAST_DIGI_OTN_ENHANCED_DDEG_INTERVAL       /* this is for error checking */
} digi_otn_enahnced_ddeg_mon_interval_t;

/*******************************************************************************
* ENUM: digi_otn_odu_oh_ins_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the source of ODU overhead
*    bytes insertion
*
* ELEMENTS:
*   DIGI_OTN_ODU_OH_INS_INCOMING          - data from incoming ODUk frame
*   DIGI_OTN_ODU_OH_INS_REGISTER_FORCE    - data from register forcing
*   DIGI_OTN_ODU_OH_INS_ODU_RCP           - data from FE/GE port
*   DIGI_OTN_ODU_OH_INS_ODU_OHP           - data from external overhead port
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_ODU_OH_INS_INCOMING         = 0,
    DIGI_OTN_ODU_OH_INS_REGISTER_FORCE,
    DIGI_OTN_ODU_OH_INS_ODU_RCP,
    DIGI_OTN_ODU_OH_INS_ODU_OHP,
    
    LAST_DIGI_OTN_ODU_OH_INS_SRC                            /* this is for out of range values error handling */
} digi_otn_odu_oh_ins_src_t;


/*******************************************************************************
* ENUM: digi_otn_prbs_pattern_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store PRBS pattern types.
*
* ELEMENTS:
*   DIGI_OTN_INVERTED_PRBS_11    - Inverted PRBS-11
*   DIGI_OTN_INVERTED_PRBS_31    - Inverted PRBS-31
*   DIGI_OTN_PRBS_11             - PRBS-11
*   DIGI_OTN_PRBS_31             - PRBS-31
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_INVERTED_PRBS_11    = 0,
    DIGI_OTN_INVERTED_PRBS_31,
    DIGI_OTN_PRBS_11,
    DIGI_OTN_PRBS_31
} digi_otn_prbs_pattern_t;

/*******************************************************************************
* ENUM: digi_otn_prbs_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the instance of PRBS
*
* ELEMENTS:
*   DIGI_OTN_FMF1_PRBS    - PRBS Generator and monitor block in FMF1
*   DIGI_OTN_FMF2_PRBS    - PRBS Generator and monitor block in FMF2
*   DIGI_OTN_FO1_PRBS     - PRBS Generator and monitor block in FO1
*   DIGI_OTN_FO2_PRBS     - PRBS Generator and monitor block in FO2
*
*******************************************************************************/
typedef enum
{
        DIGI_OTN_FMF1_PRBS   = 0,
        DIGI_OTN_FMF2_PRBS   = 1,
        DIGI_OTN_FO1_PRBS    = 2,
        DIGI_OTN_FO2_PRBS    = 3
} digi_otn_prbs_src_t;

/*******************************************************************************
* ENUM: digi_otn_server_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the source of the OTN server
*
* ELEMENTS:
*    See enum.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_SERVER_SRC_LINE = 0,
    DIGI_OTN_SERVER_SRC_SYS,

    LAST_DIGI_OTN_SERVER_SRC
} digi_otn_server_src_t;

/*******************************************************************************
* ENUM: digi_di_rcp_pdu_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet PDUs  supported by Defect 
*   Information Ring Control Port. Elements defined in this enum use an one-hot encoding in order
*    to allow enabling/disabling a set of PDUs in 1 call. 
*
* ELEMENTS:
*   DIGI_DI_RCP_CHNL_LINE_OTU_HO_ODUK                   - Line Side OTU + Channel based HO ODUk                     
*   DIGI_DI_RCP_CHNL_SYS_OTU_HO_ODUK                    - System Side OTU + Channel based HO ODUk                   
*   DIGI_DI_RCP_CHNL_PMON_HO_ODUK_RX_FRAMER_STG1        - Channel based HO ODUk Rx Framer Stage 1 (NEBC & FEBC)     
*   DIGI_DI_RCP_CHNL_PMON_HO_ODUK_RX_FRAMER_STGN        - Channel based HO ODUk Rx Framer Stage N (NEBC & FEBC)     
*   DIGI_DI_RCP_POOL_DEFECTS_HO_ODUK_RX_FRAMER_STG1     - Pool based HO ODUk Rx Framer Stage 1                      
*   DIGI_DI_RCP_POOL_DEFECTS_HO_ODUK_RX_FRAMER_STGN     - Pool based HO ODUk Rx Framer Stage N                      
*   DIGI_DI_RCP_POOL_PMON_HO_ODUK_RX_FRAMER_STG1        - Pool based HO ODUk Rx Framer Stage 1 (NEBC & FEBC)        
*   DIGI_DI_RCP_POOL_PMON_HO_ODUK_RX_FRAMER_STGN        - Pool based HO ODUk Rx Framer Stage N (NEBC & FEBC)        
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG2        - Channel based ODUk Rx Framer Stage 2                      
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG3A       - Channel based ODUk Rx Framer Stage 3a                     
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG3B       - Channel based ODUk Rx Framer Stage 3b                     
*   DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG4        - Channel based ODUk Rx Framer Stage 4                      
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG2           - Channel based ODUk Rx Framer Stage 2 (NEBC & FEBC)        
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG3A          - Channel based ODUk Rx Framer Stage 3a (NEBC & FEBC)       
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG3B          - Channel based ODUk Rx Framer Stage 3b (NEBC & FEBC)       
*   DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG4           - Channel based ODUk Rx Framer Stage 4 (NEBC & FEBC)        
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG2        - Pool based ODUk Rx Framer Stage 2                         
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG3A       - Pool based ODUk Rx Framer Stage 3a                        
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG3B       - Pool based ODUk Rx Framer Stage 3b                        
*   DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG4        - Pool based ODUk Rx Framer Stage 4                         
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG2           - Pool based ODUk Rx Framer Stage 2 (NEBC & FEBC)           
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG3A          - Pool based ODUk Rx Framer Stage 3a (NEBC & FEBC)          
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG3B          - Pool based ODUk Rx Framer Stage 3b (NEBC & FEBC)          
*   DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG4           - Pool based ODUk Rx Framer Stage 4 (NEBC & FEBC)           
*   DIGI_DI_RCP_GFPF                                    - GFP-F                                                     
*   DIGI_DI_RCP_CLIENT                                  - Clients (ENET, SDH, FC1200, FC800, PRBS)                  
*   DIGI_DI_RCP_SDH                                     - SDH B1 & B2 & M0/M1                                       
*   DIGI_DI_RCP_PRBS_NTSE                               - PRBS nTSE                                                 
*   DIGI_DI_RCP_ALL                                     - All PMON and all defects
*
*******************************************************************************/
typedef enum
{
    DIGI_DI_RCP_CHNL_LINE_OTU_HO_ODUK                      =  0x1,       
    DIGI_DI_RCP_CHNL_SYS_OTU_HO_ODUK                       =  0x2,       
    DIGI_DI_RCP_CHNL_PMON_HO_ODUK_RX_FRAMER_STG1           =  0x4,       
    DIGI_DI_RCP_CHNL_PMON_HO_ODUK_RX_FRAMER_STGN           =  0x8,      
    DIGI_DI_RCP_POOL_DEFECTS_HO_ODUK_RX_FRAMER_STG1        =  0x10,      
    DIGI_DI_RCP_POOL_DEFECTS_HO_ODUK_RX_FRAMER_STGN        =  0x20,      
    DIGI_DI_RCP_POOL_PMON_HO_ODUK_RX_FRAMER_STG1           =  0x40,      
    DIGI_DI_RCP_POOL_PMON_HO_ODUK_RX_FRAMER_STGN           =  0x80,     
    DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG2           =  0x100,     
    DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG3A          =  0x200,     
    DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG3B          =  0x400,     
    DIGI_DI_RCP_CHNL_DEFECTS_ODUK_RX_FRAMER_STG4           =  0x800,    
    DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG2              =  0x1000,    
    DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG3A             =  0x2000,    
    DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG3B             =  0x4000,    
    DIGI_DI_RCP_CHNL_PMON_ODUK_RX_FRAMER_STG4              =  0x8000,   
    DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG2           =  0x10000,   
    DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG3A          =  0x20000,   
    DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG3B          =  0x40000,   
    DIGI_DI_RCP_POOL_DEFECTS_ODUK_RX_FRAMER_STG4           =  0x80000,  
    DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG2              =  0x100000,  
    DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG3A             =  0x200000,  
    DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG3B             =  0x400000,  
    DIGI_DI_RCP_POOL_PMON_ODUK_RX_FRAMER_STG4              =  0x800000, 
    DIGI_DI_RCP_GFPF                                       =  0x1000000, 
    DIGI_DI_RCP_CLIENT                                     =  0x2000000, 
    DIGI_DI_RCP_SDH                                        =  0x4000000, 
    DIGI_DI_RCP_PRBS_NTSE                                  =  0x8000000,
    DIGI_DI_RCP_ALL                                        =  0xFFFFFFF,
    LAST_DIGI_DI_RCP_PDU
} digi_di_rcp_pdu_t;

/*******************************************************************************
* ENUM: digi_otn_sdh_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of SONET datapath direction.
*
* ELEMENTS:
*  DIGI_OTN_SDH_DIR_INGRESS   - ingress datapath selected
*  DIGI_OTN_SDH_DIR_EGRESS    - egress datapath selected
*******************************************************************************/
typedef enum {
    DIGI_OTN_SDH_DIR_INGRESS,
    DIGI_OTN_SDH_DIR_EGRESS,
    DIGI_OTN_SDH_DIR_LAST
} digi_otn_sdh_dir_t;



/*******************************************************************************
* ENUM: digi_otn_int_tcm_num_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of OTN TCM numbers for interrupts and pmon
*
* ELEMENTS:
*   See enum.
*******************************************************************************/
typedef enum {
    DIGI_OTN_TCM_NUM_TCM1 = 0,
    DIGI_OTN_TCM_NUM_TCM2,
    DIGI_OTN_TCM_NUM_TCM3,
    DIGI_OTN_TCM_NUM_TCM4,
    DIGI_OTN_TCM_NUM_TCM5,
    DIGI_OTN_TCM_NUM_TCM6,
    DIGI_OTN_TCM_NUM_FLOATING_TCM,
    DIGI_OTN_TCM_NUM_PM_NIM,

    LAST_DIGI_OTN_TCM_NUM
} digi_otn_int_tcm_num_t;

/*******************************************************************************
* ENUM: digi_otn_asym_deprov_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of the flavour of deprov possible for asymmetric deprov
*
* ELEMENTS:
*   See enum.
*******************************************************************************/
typedef enum {
    DIGI_OTN_DEPROV_RX = 0,
    DIGI_OTN_DEPROV_TX,
    DIGI_OTN_DEPROV_RX_AND_TX,
    LAST_DIGI_OTN_DEPROV_NUM
} digi_otn_asym_deprov_t;



/*
** Constants
*/
#define DIGI_OTN_TCM_PID_INVALID          ODU_RFRM_UNASSIGNED_POOL_ID
#define DIGI_OTN_IS_TCM_PID_VALID(pid)    (DIGI_OTN_TCM_PID_INVALID != pid)     

#define DIGI_OTN_INT_ALL_PIDS             DIGI_OTN_TCM_PID_INVALID 
#define DIGI_OTN_INT_ALL_ODU_LEVELS       LAST_ODU_STRUCT_LEVEL

#define DIGI_OTN_INT_IS_ALL_PIDS(pid)               (DIGI_OTN_INT_ALL_PIDS == pid)     
#define DIGI_OTN_INT_IS_ALL_ODU_LEVELS(odu_level)   (DIGI_OTN_INT_ALL_ODU_LEVELS == odu_level)     

/* Code to specify ODU channel is not found */
#define DIGI_OTN_ODU_CHNL_UNKNOWN                   0xFF

/* Code to specify OTN server channel or stream is not found */
#define DIGI_OTN_ODU_CHNL_UNKNOWN                   0xFF

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: digi_otn_server_pmon_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for LINE/SYS OTN PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    
    digi_pmon_lineotn_otu_frm_counters_t     framer;
    digi_pmon_lineotn_fec_counters_t         fec;

} digi_otn_server_pmon_chnl_t;


/*******************************************************************************
* STRUCTURE: digi_otn_odu_pmon_chnl_data_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for CORE OTN PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_pmon_coreotn_odukp_counters_t       pm;
    
    UINT32                                   tcm_pool_ids[LAST_DIGI_OTN_TCM_NUM];
    digi_pmon_coreotn_odukt_counters_t       tcm[LAST_DIGI_OTN_TCM_NUM];

    BOOL8                                    prbs_valid;
    digi_pmon_coreotn_odukp_prbs_counter_t   prbs;

} digi_otn_odu_pmon_chnl_data_t;


/*******************************************************************************
* STRUCTURE: digi_otn_odu_pmon_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for CORE OTN PMON: contains HO/MO/LO channel's PMON as well
*   as the Stage 3b PMON (if applicable)
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_otn_odu_pmon_chnl_data_t            pmon;

    BOOL8                                    stg3b_valid;
    digi_otn_odu_pmon_chnl_data_t            stg3b;
} digi_otn_odu_pmon_chnl_t;


/*******************************************************************************
* STRUCTURE: digi_otn_pmon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for OTN PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    /* LINEOTN */
    digi_otn_server_chnl_t          *otn_server_handle[DIGI_OTN_SERVER_CHNL_MAX];
    digi_otn_server_pmon_chnl_t      otn_server_chnl[DIGI_OTN_SERVER_CHNL_MAX];
    
    /* SYSOTN */
    digi_otn_server_chnl_t          *sysotn_server_handle[DIGI_OTN_SERVER_SYS_CHNL_MAX];
    digi_otn_server_pmon_chnl_t      sysotn_server_chnl[DIGI_OTN_SERVER_SYS_CHNL_MAX];

    /* High Order */ 
    /* Line: Stg 1
     * Sys:  Stg N
     */
    digi_otn_odu_chnl_t             *otn_odu_ho_handle[DIGI_OTN_SERVER_HO_CHNL_MAX];
    digi_otn_odu_pmon_chnl_t         otn_odu_ho_chnl[DIGI_OTN_SERVER_HO_CHNL_MAX];

    /* Mid Order */
    /* Line: Stg 2
     * Sys:  Stg 3A
     */
    digi_otn_odu_chnl_t             *otn_odu_mo_handle[DIGI_OTN_SERVER_MO_CHNL_MAX];
    digi_otn_odu_pmon_chnl_t         otn_odu_mo_chnl[DIGI_OTN_SERVER_MO_CHNL_MAX];

    /* Low Order */
    /* Line: Stg 3A
     * Sys:  Stg 1 
     */
    digi_otn_odu_chnl_t             *otn_odu_lo_handle[DIGI_OTN_SERVER_LO_CHNL_MAX];
    digi_otn_odu_pmon_chnl_t         otn_odu_lo_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];
    
    /* SYSOTN Line TCM Monitoring */
    /* Line: Not valid
     * Sys:  Stg 2 
     */
    digi_otn_odu_chnl_t             *otn_odu_lo_mon_handle[DIGI_OTN_SERVER_LO_CHNL_MAX];
    digi_otn_odu_pmon_chnl_t         otn_odu_lo_mon_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];
    
} digi_otn_pmon_t;



/*******************************************************************************
* ENUM: digi_otn_int_filter_blocks_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration of blocks to retrieve for OTN interrupts
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    DIGI_OTN_INT_FILTER_BLOCKS_OTN_MLD     = COREOTN_INT_FILTER_BLOCKS_OTN_MLD   ,
    DIGI_OTN_INT_FILTER_BLOCKS_OTN_SERVER  = COREOTN_INT_FILTER_BLOCKS_OTN_SERVER,
    DIGI_OTN_INT_FILTER_BLOCKS_OTN_ODU     = COREOTN_INT_FILTER_BLOCKS_OTN_ODU   ,
    DIGI_OTN_INT_FILTER_BLOCKS_OTN_STG3B   = COREOTN_INT_FILTER_BLOCKS_OTN_STG3B ,
    DIGI_OTN_INT_FILTER_BLOCKS_TFRM        = COREOTN_INT_FILTER_BLOCKS_TFRM      ,
    DIGI_OTN_INT_FILTER_BLOCKS_MPMO        = COREOTN_INT_FILTER_BLOCKS_MPMO      ,
    DIGI_OTN_INT_FILTER_BLOCKS_MPMA        = COREOTN_INT_FILTER_BLOCKS_MPMA      ,
    DIGI_OTN_INT_FILTER_BLOCKS_ODTU_DMX    = COREOTN_INT_FILTER_BLOCKS_ODTU_DMX  ,
    DIGI_OTN_INT_FILTER_BLOCKS_ODTU_MUX    = COREOTN_INT_FILTER_BLOCKS_ODTU_MUX  ,
    DIGI_OTN_INT_FILTER_BLOCKS_ODUKSC      = COREOTN_INT_FILTER_BLOCKS_ODUKSC    ,
    DIGI_OTN_INT_FILTER_BLOCKS_ODUJAT      = COREOTN_INT_FILTER_BLOCKS_ODUJAT    ,
    DIGI_OTN_INT_FILTER_BLOCKS_OHP         = COREOTN_INT_FILTER_BLOCKS_OHP       ,
    DIGI_OTN_INT_FILTER_BLOCKS_RCP         = COREOTN_INT_FILTER_BLOCKS_RCP       ,
    DIGI_OTN_INT_FILTER_BLOCKS_PRBS        = COREOTN_INT_FILTER_BLOCKS_PRBS      ,
    
    LAST_DIGI_OTN_INT_FILTER_BLOCKS,
} digi_otn_int_filter_blocks_t;


/*******************************************************************************
* STRUCTURE: digi_otn_int_filter_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Filter to reduce retrieval scope of OTN interrupts
*
* ELEMENTS:
*   blocks - bitmask of digi_otn_int_filter_blocks_t enums to indicate
*            retrieval
*
*******************************************************************************/
typedef struct {
    UINT32      blocks;
} digi_otn_int_filter_t;


/*******************************************************************************
* STRUCTURE: digi_otn_odu_int_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for CORE OTN interrupts: contains HO/MO/LO channel interrupt
*   as well as the Stage 3b (if applicable)
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    coreotn_int_chnl_t      status;

    BOOL8                   stg3b_valid;
    coreotn_int_chnl_t      stg3b;
} digi_otn_odu_int_chnl_t;


/*******************************************************************************
* STRUCTURE: digi_otn_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for OTN interrupts
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    /* LINEOTN */
    /* LineOtn unchannelized interrupts */
    lineotn_int_t             lineotn;

    digi_otn_server_chnl_t   *otn_server_handle[DIGI_OTN_SERVER_CHNL_MAX];
    lineotn_int_chnl_t        otn_server_chnl[DIGI_OTN_SERVER_CHNL_MAX];
    
    /* SYSOTN */
    /* SysOtn unchannelized interrupts */
    lineotn_int_t             sysotn;

    digi_otn_server_chnl_t   *sysotn_server_handle[DIGI_OTN_SERVER_SYS_CHNL_MAX];
    lineotn_int_chnl_t        sysotn_server_chnl[DIGI_OTN_SERVER_SYS_CHNL_MAX];

    /* COREOTN */
    /* Coreotn unchannelized interrupts */
    coreotn_int_t             coreotn;

    /* High Order */ 
    /* non-DIGI_PROD_APP_SYSOTN_CARD prod app: Stg 1
     * DIGI_PROD_APP_SYSOTN_CARD prod app:     Stg N
     */
    digi_otn_odu_chnl_t      *otn_odu_ho_handle[DIGI_OTN_SERVER_HO_CHNL_MAX];
    digi_otn_odu_int_chnl_t   otn_odu_ho_chnl[DIGI_OTN_SERVER_HO_CHNL_MAX];

    /* Mid Order */
    /* non-DIGI_PROD_APP_SYSOTN_CARD prod app: Stg 2
     * DIGI_PROD_APP_SYSOTN_CARD prod app:     Stg 3A
     */
    digi_otn_odu_chnl_t      *otn_odu_mo_handle[DIGI_OTN_SERVER_MO_CHNL_MAX];
    digi_otn_odu_int_chnl_t   otn_odu_mo_chnl[DIGI_OTN_SERVER_MO_CHNL_MAX];

    /* Low Order */
    /* non-DIGI_PROD_APP_SYSOTN_CARD prod app: Stg 3A
     * DIGI_PROD_APP_SYSOTN_CARD prod app:     Stg 1
     */
    digi_otn_odu_chnl_t      *otn_odu_lo_handle[DIGI_OTN_SERVER_LO_CHNL_MAX];
    digi_otn_odu_int_chnl_t   otn_odu_lo_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];
    
    /* SYSOTN Line TCM Monitoring */
    /* non-DIGI_PROD_APP_SYSOTN_CARD prod app: Not valid
     * DIGI_PROD_APP_SYSOTN_CARD prod app:     Stg 2
     */
    digi_otn_odu_chnl_t      *otn_odu_lo_mon_handle[DIGI_OTN_SERVER_LO_CHNL_MAX];
    digi_otn_odu_int_chnl_t   otn_odu_lo_mon_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];
    
    /* ODU OHP */
    odu_ohp_int_t             ohp[COREOTN_NUM_ODU_OHP_INST];

    /* DI RCP */
    odu_rcp_int_t             di_rcp;   

    /* RI RCP */
    odu_rcp_int_t             ri_rcp;
} digi_otn_int_t;


/*******************************************************************************
* ENUM: digi_otn_server_int_summary_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to retrieve in digi_otn_server_int_summary_t.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_OTN_SERVER_INT_SUMMARY_MASK_LINE_SYS           = 0x01,
    DIGI_OTN_SERVER_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM    = 0x02,
    DIGI_OTN_SERVER_INT_SUMMARY_MASK_ACTIVE_CHNL        = 0x04,
} digi_otn_server_int_summary_mask_t;


/*******************************************************************************
* STRUCTURE: digi_otn_server_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for OTN server channels
*
* ELEMENTS:
*   mask                    - digi_otn_server_int_summary_mask_t bitmask to
*                             indicate which fields have been returned
*
*   active_chnl_num         - Number of channels in OTN server that have an active
*                             enabled interrupts
*
*   active_chnl             - Array of OTN server channel handles that have an
*                             have an active enabled interrupt. This allows the user
*                             to iteratively call digi_otn_server_int_chnl_filt_retrieve()
*                             with the provided channel handles.
*
*******************************************************************************/
typedef struct {
    UINT32                  mask;

    UINT32                  active_chnl_num;
    digi_otn_server_chnl_t *active_chnl[DIGI_OTN_SERVER_HO_CHNL_MAX];
} digi_otn_server_int_summary_t;



/*******************************************************************************
* ENUM: digi_otn_odu_int_summary_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to retrieve in digi_otn_odu_int_summary_t.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_OTN_ODU_INT_SUMMARY_MASK_ODU_LEVEL             = 0x001,

    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM       = 0x002,
    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_CHNL           = 0x004,

    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_CHNL_BITMASK   = 0x008,
    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_CHNL_POPCOUNT  = 0x010,

    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_PID_BITMASK    = 0x020,
    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_PID_POPCOUNT   = 0x040,

    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_INT_BITMASK    = 0x080,
    DIGI_OTN_ODU_INT_SUMMARY_MASK_ACTIVE_INT_POPCOUNT   = 0x100,
} digi_otn_odu_int_summary_mask_t;


/*******************************************************************************
* STRUCTURE: digi_otn_odu_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for OTN ODU channels
*
* ELEMENTS:
*   mask                    - digi_otn_odu_int_summary_mask_t bitmask to
*                             indicate which fields have been returned
*
*   odu_level               - ODU level that is being summarized
* 
*   active_chnl_num         - Number of channels in active_chnl field that have an active
*                             interrupt (a channel will be active if there assigned PIDs
*                             to the chnl also have active interrupts)
*
*   active_chnl             - Array of ODU channel handles in ODU framer that
*                             have an active interrupt. This allows the user
*                             to iteratively call digi_otn_odu_int_chnl_filt_retrieve()
*                             with the provided channel handles.
*
*
*   active_chnl_popcount    - Population count of bits in active_chnl_bitmask
*
*   active_chnl_bitmask     - Bitmask of channels in ODU framer that have an
*                             active interrupt
*
*   active_pid_popcount     - Population count of bits in active_pid_bitmask
*
*   active_pid_bitmask      - Bitmask of PID TCM pool IDs in ODU framer that have an
*                             active interrupt
*
*   active_int_popcount     - Population count of bits in active_int_bitmask
*
*   active_int_bitmask      - Bitmask of active interrupts in ODU framer.
*                             The user can iteratively determine active interrupts
*                             by accessing the bit numbers defined in 
*                             odu_rfrm_int_defect_t
*                             
*
*******************************************************************************/
typedef struct {
    UINT32                  mask;

    odu_struct_odu_level_t  odu_level;

    UINT32                  active_chnl_num;
    digi_otn_odu_chnl_t    *active_chnl[DIGI_OTN_SERVER_LO_CHNL_MAX];

    UINT32                  active_chnl_popcount;
    UINT32                  active_chnl_bitmask[PMC_BITARRAY_SIZE(DIGI_ODU_CHNLS_MAX)];
    
    UINT32                  active_pid_popcount;
    UINT32                  active_pid_bitmask[PMC_BITARRAY_SIZE(DIGI_ODU_PIDS_MAX)];    

    UINT32                  active_int_popcount;
    UINT32                  active_int_bitmask[PMC_BITARRAY_SIZE(LAST_ODU_RFRM_INT_DEFECT)];
} digi_otn_odu_int_summary_t;



/*******************************************************************************
* STRUCTURE:  digi_bergen_lane_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining BERGEN lane configuration. A lane configuration is 
*   applied to 1 or multiple lanes based on the logical_lanes_msk field
*
* ELEMENTS:
*   error_rate - The bit error density. You can specify a value between 0 and\n
*                1.0. This field has a precision of 24 bits.
*   ringosc_enable   - Enables or disables the bergen Ring Oscillator. The 32\n
*                      bit Bit Error Rate Generator (BERGEN32) blocks have the\n
*                      ability to use clocks created by free running ring \n
*                      oscillators to create asynchronous events that better \n
*                      randomize the distribution of bit errors inserted in \n
*                      the transmit line data.
*   err_mode     - The error insertion mode (see lineotn_bergen_err_mode type\n
*                  definition)
*
*******************************************************************************/
typedef struct digi_bergen_lane_cfg_t   {    
    FLOAT error_rate;
    BOOL8 ringosc_enable;
    lineotn_bergen_err_mode bergen_mode;
} digi_bergen_lane_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_di_rcp_pmon_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Config options for di rcp pmon behaviour
*
* ELEMENTS:
*   no_otn_count  - If TRUE, RCP will not accumulate and collect OTN PMON counts. Instead, they
*                   will be available via the PMON interrupt. 
*                   If FALSE, RCP accumulates and collects the OTN PMON counts.
*   no_cbrc_count - If TRUE, RCP will not accumulate and collect the SDH (eg. B1 and B2 counts)
*                   counts. Instead they will be available via the 1 sec PMON interrupt.
*                   If FALSE, RCP accumlates and collects the SDH counts.
*   sdh_cnt_sel   - Select mask for channels 0 to 11. 
*                   When channel bit is 0 -> ingress count is selected
*                   When channel bit is 1 -> egress count is selected 
*   enable        - TRUE to enable transfer, FALSE to disable
*******************************************************************************/
typedef struct {
    BOOL8 enable;
    UINT16 sdh_cnt_sel;
    BOOL8 no_cbrc_count;
    BOOL8 no_otn_count;
} digi_di_rcp_pmon_cfg_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

/*
** ODU related functions 
*/

PUBLIC PMC_ERROR digi_otn_odu_defect_actions_ctrl_set(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT8 defect_action_wr_mask,
                                                      UINT8 defect_action_values);
PUBLIC PMC_ERROR digi_otn_odu_tcmi_ltc_to_ais_tsf_set(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                      BOOL8 dLTC_dis);
PUBLIC PMC_ERROR digi_otn_odu_tim_act_dis_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              BOOL8 tim_act_dis);                                                
PUBLIC PMC_ERROR digi_otn_odu_maint_sig_set(digi_handle_t *digi_handle,
                                            digi_otn_odu_chnl_t *chnl_ptr,
                                            UINT8 direction,
                                            coreotn_maint_sig_type_t force_alarm_sig);

PUBLIC PMC_ERROR digi_otn_odu_odukp_pck_maint_sig_set(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      coreotn_maint_sig_type_t force_alarm_sig);
PUBLIC PMC_ERROR digi_otn_odu_odukp_pck_maint_sig_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      coreotn_maint_sig_type_t *force_alarm_sig);
PUBLIC PMC_ERROR digi_otn_odu_chnl_sd_sf_ssf_prop_set(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT8 chnl_defect_action_msk);
PUBLIC PMC_ERROR digi_otn_odu_chnl_lof_lom_act_dis_set(digi_handle_t *digi_handle,
                                                       digi_otn_odu_chnl_t *chnl_ptr,
                                                       BOOL8 lof_lom_act_dis);
                                         
PUBLIC PMC_ERROR digi_otn_odu_rx_pm_tcmi_set(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 pool_assign_mode,
                                             UINT8 tcm_pool_id, 
                                             UINT8 float_tcmi_src,
                                             odu_struct_odu_frm_pm_tcm_id_t parent_tcm_num,
                                             odu_struct_pm_tcm_mode_t sink_mode,
                                             UINT16 *assigned_pool_id);
PUBLIC PMC_ERROR digi_otn_odu_rx_pm_tcmi_activate(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr);
PUBLIC PMC_ERROR digi_otn_odu_rx_tcmi_mode_update(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  odu_struct_pm_tcm_mode_t sink_mode);
PUBLIC PMC_ERROR digi_otn_odu_tcmi_resource_transition(digi_handle_t *digi_handle,
                                                       digi_otn_odu_chnl_t *chnl_ptr,
                                                       UINT32 pool_id);
PUBLIC PMC_ERROR digi_otn_odu_rx_pm_tcmi_nim_parent_update(digi_handle_t *digi_handle,
                                                           digi_otn_odu_chnl_t *chnl_ptr,
                                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                           odu_struct_odu_frm_pm_tcm_id_t new_parent_tcm_num);
PUBLIC PMC_ERROR digi_otn_odu_tx_pm_tcmi_set(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 pool_assign_mode,
                                             UINT8 tcm_pool_id,
                                             odu_struct_odu_frm_pm_tcm_id_t parent_tcm_num,
                                             odu_struct_pm_tcm_mode_t source_mode,
                                             UINT16 *assigned_pool_id);  
PUBLIC PMC_ERROR digi_otn_odu_rx_pm_tcmi_layer_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   odu_struct_odu_frm_layer_struct_t (*tcm_layers)[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                   UINT32 *num_layers);
PUBLIC PMC_ERROR digi_otn_odu_tx_pm_tcmi_layer_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   odu_struct_odu_frm_layer_struct_t (*tcm_layers)[ODU_SOURCE_MAX_PM_TCM_LAYERS],
                                                   UINT32 *num_layers);                                                   
                                             
PUBLIC PMC_ERROR digi_otn_odu_pm_tcm_res_src_set(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 digi_otn_odu_oh_ins_src_t pm_tcm_res_src,
                                                 UINT8 pm_tcm_res_force_val);
PUBLIC PMC_ERROR digi_otn_odu_ftfl_src_set(digi_handle_t *digi_handle,
                                           digi_otn_odu_chnl_t *chnl_ptr,
                                           digi_otn_odu_oh_ins_src_t ftfl_source,
                                           UINT8 ftfl_force_field);
PUBLIC PMC_ERROR digi_otn_odu_tx_ftfl_set(digi_handle_t *digi_handle,
                                       digi_otn_odu_chnl_t *chnl_ptr,
                                       UINT8 fwd_id,
                                       UINT8 fwd_op_id[9],
                                       UINT8 bwd_id,
                                       UINT8 bwd_op_id[9]);
PUBLIC PMC_ERROR digi_otn_odu_tx_tcm_act_src_set(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 digi_otn_odu_oh_ins_src_t tcm_act_source,
                                                 UINT8 tcm_act_force_val);                                             
PUBLIC PMC_ERROR digi_otn_odu_aps_pcc_src_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              UINT8 mon_level,
                                              digi_otn_odu_oh_ins_src_t aps_pcc_source,
                                              UINT32 aps_pcc_force_val);                                         
PUBLIC PMC_ERROR digi_otn_odu_tx_res1_src_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              digi_otn_odu_oh_ins_src_t res1_source,
                                              UINT16 res1_force_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_res2_src_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              digi_otn_odu_oh_ins_src_t res2_source,
                                              UINT8 res2_force_val[6]);
PUBLIC PMC_ERROR digi_otn_odu_tx_exp_src_set(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             digi_otn_odu_oh_ins_src_t exp_source,
                                             UINT16 exp_force_val);                                          
PUBLIC PMC_ERROR digi_otn_odu_tx_gcc1_src_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              digi_otn_odu_oh_ins_src_t gcc1_source,
                                              UINT16 gcc1_force_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_gcc2_src_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              digi_otn_odu_oh_ins_src_t gcc2_source,
                                              UINT16 gcc2_force_val);

PUBLIC PMC_ERROR digi_otn_odu_deg_m_set(digi_handle_t                 *digi_handle,
                                        digi_otn_odu_chnl_t           *chnl_ptr,                                      
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        UINT8                          deg_m);

PUBLIC PMC_ERROR digi_otn_odu_deg_th_set(digi_handle_t                 *digi_handle,
                                         digi_otn_odu_chnl_t           *chnl_ptr,
                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                         UINT32                         deg_th);

PUBLIC PMC_ERROR digi_otn_odu_deg_state_get(digi_handle_t                  *digi_handle,
                                            digi_otn_odu_chnl_t            *chnl_ptr,
                                            odu_struct_odu_frm_pm_tcm_id_t  pm_tcm_num,
                                            digi_pmon_ddeg_state_t         *digi_pmon_ddeg_state_ptr);
PUBLIC PMC_ERROR digi_otn_otu_deg_state_get(digi_handle_t *digi_handle,
                                            digi_otn_server_chnl_t *otn_server_ptr,
                                            digi_pmon_ddeg_state_t         *digi_pmon_ddeg_state_ptr);
PUBLIC PMC_ERROR digi_otn_odu_tim_det_mon_set(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              UINT8 tim_det_mo);
PUBLIC PMC_ERROR digi_otn_odu_bei_biae_set(digi_handle_t *digi_handle,
                                           digi_otn_odu_chnl_t *chnl_ptr,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 force_val);
PUBLIC PMC_ERROR digi_otn_odu_bdi_set(digi_handle_t *digi_handle,
                                      digi_otn_odu_chnl_t *chnl_ptr,
                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                      UINT8 force_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_stat_set(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 force_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_all_tcm_iae_force(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr);
PUBLIC PMC_ERROR digi_otn_odu_tcmi_maint_sig_set(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 UINT8 direction,
                                                 odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                 coreotn_maint_sig_type_t maint_sig);      


PUBLIC PMC_ERROR digi_otn_odu_get_delay_measure_count(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                      UINT32 *dm_count);
PUBLIC PMC_ERROR digi_otn_odu_get_delay_measure_count_no_block(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                      UINT32 *dm_count);
PUBLIC PMC_ERROR digi_otn_odu_delay_measure_go(digi_handle_t *digi_handle,
                                               digi_otn_odu_chnl_t *chnl_ptr,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num);
PUBLIC PMC_ERROR digi_otn_odu_delay_measure_init(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                 digi_otn_dm_mode_t dm_mode,
                                                 UINT32 dm_val);
PUBLIC PMC_ERROR digi_otn_odu_delay_measure_mode_get(digi_handle_t *digi_handle,
                                                     digi_otn_odu_chnl_t *chnl_ptr,
                                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                     digi_otn_dm_mode_t *dm_mode,
                                                     UINT32 *dm_val);
PUBLIC PMC_ERROR digi_otn_msi_mode_set( digi_handle_t *digi_handle,
                                        digi_otn_msi_cfg_mode_t msi_cfg_mode); 

PUBLIC PMC_ERROR digi_otn_msi_mode_get( digi_handle_t *digi_handle,
                                        digi_otn_msi_cfg_mode_t *msi_cfg_mode);

PUBLIC PMC_ERROR digi_otn_opu_rx_expected_msi_set(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  UINT8 ts_port,
                                                  UINT8 exp_msi);
PUBLIC PMC_ERROR digi_otn_opu_tx_msi_set(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         UINT8 ts_port,
                                         UINT8 msi_byte);                                                                                      
PUBLIC PMC_ERROR digi_otn_odu_tx_tti_set(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                         odu_struct_tti_oh_ins_t tti_oh_ins,
                                         UINT8 sapi[16],
                                         UINT8 dapi[16],
                                         UINT8 op_spec[32]); 
PUBLIC PMC_ERROR digi_otn_odu_rx_expected_tti_set(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 sapi[16],
                                                  UINT8 dapi[16]);   
PUBLIC PMC_ERROR digi_otn_opu_tx_pt_set(digi_handle_t *digi_handle,
                                        digi_otn_odu_chnl_t *chnl_ptr,
                                        UINT8 pt_val);
PUBLIC PMC_ERROR digi_otn_opu_rx_expected_pt_set(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 UINT8 pt_val);                                                                                                                                            
PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_bw_act_set(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                 UINT8 bw_dis);


PUBLIC PMC_ERROR digi_otn_odu_dplm_to_ssf_set(digi_handle_t *digi_handle, 
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              BOOL8 enable);

PUBLIC PMC_ERROR digi_otn_odu_dloomfi_to_ssf_set(digi_handle_t *digi_handle, 
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 BOOL8 enable);

PUBLIC PMC_ERROR digi_otn_odu_dmsim_to_ssf_set(digi_handle_t *digi_handle, 
                                               digi_otn_odu_chnl_t *chnl_ptr,
                                               BOOL8 enable);

PUBLIC PMC_ERROR digi_otn_odu_ddeg_mode_set(digi_handle_t          *digi_handle,
                                            digi_otn_ddeg_mode_t    ddeg_mode,
                                            digi_otn_ddeg_mon_seg_t odu_segment,
                                            BOOL8                   enable);

PUBLIC PMC_ERROR digi_otn_odu_enhanced_ddeg_interval_set(digi_handle_t                  *digi_handle,
                                                         digi_otn_odu_chnl_t            *chnl_ptr,
                                                         odu_struct_odu_frm_pm_tcm_id_t  pm_tcm_num,
                                                         UINT8                           mon_interval) ;

PUBLIC PMC_ERROR digi_otn_odu_ddeg_mon_enable(digi_handle_t                   *digi_handle,
                                              digi_otn_odu_chnl_t             *chnl_ptr,
                                              digi_otn_ddeg_mode_t             ddeg_mode,
                                              odu_struct_odu_frm_pm_tcm_id_t   pm_tcm_num,
                                              BOOL                             enable);                                                         

PUBLIC PMC_ERROR digi_otn_otu_hw_ddeg_mon_enable(digi_handle_t           *digi_handle,
                                                 BOOL                     enable);

PUBLIC PMC_ERROR digi_otn_odu_hw_ddeg_mon_enable(digi_handle_t *digi_handle,
                                                 BOOL           enable);

PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_tsd_tsf_dis_set(digi_handle_t                  *digi_handle,
                                                      digi_otn_odu_chnl_t            *chnl_ptr,
                                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                      UINT8 defect_ctrl,
                                                      UINT8 tsd_dis,
                                                      UINT8 tsf_dis);    
PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_ais_dis_set(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 ais_dis);  

PUBLIC PMC_ERROR digi_otn_odu_cm_min_max_reprov(digi_handle_t * digi_handle,
                                                digi_otn_odu_chnl_t *chnl_ptr,
                                                BOOL use_custom_ppm,
                                                UINT32 ppm_offset);                                    
                                           
/*
** ODU Struct Getter methods
*/
PUBLIC PMC_ERROR digi_otn_odu_rx_pm_tcm_get(digi_handle_t *digi_handle,
                                            digi_otn_odu_chnl_t *chnl_ptr,
                                            UINT8 *pm_tcm_val);
PUBLIC PMC_ERROR digi_otn_odu_tcm_pool_id_get(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              UINT8 direction,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              UINT16 *tcm_pool_id);
                                              
PUBLIC PMC_ERROR digi_otn_odu_ddeg_mode_get(digi_handle_t           *digi_handle,
                                            digi_otn_ddeg_mode_t     ddeg_mode,
                                            BOOL                    *ddeg_enabled,
                                            digi_otn_ddeg_mon_seg_t *mon_seg);
                                            
PUBLIC PMC_ERROR digi_otn_opu_null_payload_status_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      BOOL8 *null_status);
PUBLIC PMC_ERROR digi_otn_odu_tx_aps_pcc_src_get(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 UINT8 mon_level,
                                                 UINT8 *aps_pcc_source);
PUBLIC PMC_ERROR digi_otn_odu_tx_aps_pcc_get(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             UINT8 mon_level,
                                             UINT32 *aps_pcc_force_val);
PUBLIC PMC_ERROR digi_otn_odu_rx_aps_pcc_get(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             UINT8 mon_level,
                                             UINT32 *aps_pcc_force_val);
PUBLIC PMC_ERROR digi_otn_odu_chnl_lof_lom_act_dis_get(digi_handle_t *digi_handle,
                                                       digi_otn_odu_chnl_t *chnl_ptr,
                                                       BOOL8 *lof_lom_act_dis);
PUBLIC PMC_ERROR digi_otn_odu_defect_actions_ctrl_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT8 *defect_action_values);

PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_ais_dis_get(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 *ais_dis);
PUBLIC PMC_ERROR digi_otn_odu_ebc_get(digi_handle_t *digi_handle,
                                      digi_otn_odu_chnl_t *chnl_ptr,
                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                      digi_pmon_coreotn_odukp_counters_t *pm_ebc,
                                      digi_pmon_coreotn_odukt_counters_t *tcm_ebc);
PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_tsd_tsf_dis_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                      UINT8 *tsd_dis,
                                                      UINT8 *tsf_dis);
PUBLIC PMC_ERROR digi_otn_odu_pm_tcm_res_get(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             UINT8 *pm_tcm_res_src,
                                             UINT8 *pm_tcm_res_val);
PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_bw_act_get(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                 UINT8 *bw_dis);
PUBLIC PMC_ERROR digi_otn_odu_tcmi_ltc_to_ais_tsf_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                      BOOL8 *dLTC_dis);
PUBLIC PMC_ERROR digi_otn_odu_tx_res1_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT8 *res1_src,
                                          UINT16 *res1_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_res2_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT8 *res2_src,
                                          UINT8 *res2_val);                                          
PUBLIC PMC_ERROR digi_otn_odu_tx_stat_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 *stat_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_gcc1_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT8 *gcc1_src,
                                          UINT16 *gcc1_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_gcc2_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT8 *gcc2_src,
                                          UINT16 *gcc2_val);                                          
PUBLIC PMC_ERROR digi_otn_opu_tx_msi_get(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         UINT8 *msi_val);
PUBLIC PMC_ERROR digi_otn_opu_tx_msi_per_ts_get(digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_t *chnl_ptr,
                                                UINT8 ts_port,
                                                UINT8 *msi_val);                              
PUBLIC PMC_ERROR digi_otn_opu_rx_partial_ho_accept_msi_init(digi_handle_t *digi_handle,
                                                            digi_otn_odu_chnl_t *chnl_ptr,
                                                            BOOL init_msi);           
PUBLIC PMC_ERROR digi_otn_opu_rx_accepted_msi_get(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  UINT8 *msi_val); 
PUBLIC PMC_ERROR digi_otn_opu_rx_accepted_msi_per_ts_get(digi_handle_t *digi_handle,
                                                         digi_otn_odu_chnl_t *chnl_ptr,
                                                         UINT8 ts_port,
                                                         UINT8 *msi_val);                                                  
PUBLIC PMC_ERROR digi_otn_opu_rx_expected_msi_get(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  UINT8 msi_val[UTIL_GLOBAL_MAX_NUM_TS]);     
PUBLIC PMC_ERROR digi_otn_opu_rx_expected_msi_per_ts_get(digi_handle_t *digi_handle,
                                                         digi_otn_odu_chnl_t *chnl_ptr,
                                                         UINT8 ts_port,
                                                         UINT8* msi_val);                                                                                             
PUBLIC PMC_ERROR digi_otn_odu_pm_tcmi_mode_get(digi_handle_t *digi_handle,
                                               digi_otn_odu_chnl_t *chnl_ptr,
                                               UINT8 direction,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                               odu_struct_pm_tcm_mode_t *mode);

PUBLIC PMC_ERROR digi_otn_opu_rx_expected_pt_get(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 UINT8 *exp_pt_val);
PUBLIC PMC_ERROR digi_otn_opu_rx_accepted_pt_get(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 UINT8 *accpt_pt_val);                                        
PUBLIC PMC_ERROR digi_otn_opu_tx_pt_get(digi_handle_t *digi_handle,
                                        digi_otn_odu_chnl_t *chnl_ptr,
                                        UINT8 *pt_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_tti_get(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                         UINT8 *sapi_ptr,
                                         UINT8 *dapi_ptr,
                                         UINT8 *op_spec_ptr);
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_tti_lock(digi_handle_t *digi_handle,
                                                  BOOL8 set);
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_tti_get(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 *sapi_ptr,
                                                  UINT8 *dapi_ptr,
                                                  UINT8 *op_spec_ptr);
PUBLIC PMC_ERROR digi_otn_odu_rx_expected_tti_get(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 *sapi_ptr,
                                                  UINT8 *dapi_ptr);                                                                                       
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_ftfl_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   UINT8 *ftfl);
PUBLIC PMC_ERROR digi_otn_odu_tx_ftfl_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT8 *ftfl_src,
                                          UINT8 *ftfl); 
PUBLIC PMC_ERROR digi_otn_odu_tx_tcm_act_get(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             UINT8 *tcm_act_src,
                                             UINT8 *tcm_act_val);                                                                                
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_tcm_act_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT8 *tcm_act_val);
PUBLIC PMC_ERROR digi_otn_odu_rx_res1_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT16 *res1_val);
PUBLIC PMC_ERROR digi_otn_odu_rx_res2_get(digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *chnl_ptr,
                                          UINT8 *res2_val);
PUBLIC PMC_ERROR digi_otn_odu_rx_exp_get(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         UINT16 *exp_val);
PUBLIC PMC_ERROR digi_otn_odu_tx_exp_get(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         UINT8 *exp_src,
                                         UINT16 *exp_val);                                                  
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_gcc1_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   UINT16 *gcc1_val);
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_gcc2_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   UINT16 *gcc2_val);
PUBLIC PMC_ERROR digi_otn_odu_bei_biae_get(digi_handle_t *digi_handle,
                                           digi_otn_odu_chnl_t *chnl_ptr,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 *bei_biae_val);
PUBLIC PMC_ERROR digi_otn_odu_bdi_get(digi_handle_t *digi_handle,
                                      digi_otn_odu_chnl_t *chnl_ptr,
                                      odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                      UINT8 *bdi_val);
PUBLIC PMC_ERROR digi_otn_odu_rcp_en_get(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         UINT32 *rcp_en);
PUBLIC PMC_ERROR digi_otn_odu_rx_accepted_stat_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                   UINT8 *stat_val);

PUBLIC PMC_ERROR digi_otn_odu_deg_m_get(digi_handle_t                 *digi_handle, 
                                        digi_otn_odu_chnl_t           *chnl_ptr,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        UINT8                         *deg_m);

PUBLIC PMC_ERROR digi_otn_odu_deg_th_get(digi_handle_t                 *digi_handle,
                                         digi_otn_odu_chnl_t           *chnl_ptr,
                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                         UINT32                        *deg_th);

PUBLIC PMC_ERROR digi_otn_odu_tim_det_mon_get(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              UINT8 *tim_det_mo);

PUBLIC PMC_ERROR digi_otn_odu_tim_act_dis_get(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              BOOL8 *tim_act_dis);

PUBLIC PMC_ERROR digi_otn_odu_dmsim_to_ssf_get(digi_handle_t *digi_handle, 
                                               digi_otn_odu_chnl_t *chnl_ptr,
                                               BOOL8 *enable);


PUBLIC PMC_ERROR digi_otn_odu_dloomfi_to_ssf_get(digi_handle_t *digi_handle, 
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 BOOL8 *enable);

PUBLIC PMC_ERROR digi_otn_odu_dplm_to_ssf_get(digi_handle_t *digi_handle, 
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              BOOL8 *enable);

PUBLIC PMC_ERROR digi_otn_odu_enhanced_ddeg_interval_get(digi_handle_t *digi_handle,
                                                         digi_otn_odu_chnl_t *chnl_ptr,
                                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                         UINT8 *mon_interval);

/* 
** OTN PRBS functions 
*/
PUBLIC PMC_ERROR digi_otn_prbs_chnl_init(digi_handle_t *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,  
                                         UINT32 prbs_chid,
                                         UINT32 prbs_operation,
                                         UINT32 mode);
PUBLIC PMC_ERROR digi_otn_prbs_pattern_cfg(digi_handle_t *digi_handle,
                                           digi_otn_odu_chnl_t *chnl_ptr, 
                                           UINT32 prbs_chid,
                                           digi_otn_prbs_pattern_t prbs_pattern); 
PUBLIC PMC_ERROR digi_otn_prbs_ber_thres_cfg(digi_handle_t *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr, 
                                             UINT32 prbs_ber_lo_thres, 
                                             UINT32 prbs_ber_hi_thres,
                                             UINT16 prbs_reseed_thres,
                                             UINT16 prbs_reseed_interval);
PUBLIC PMC_ERROR digi_otn_prbs_chnl_bit_err_inject(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr,
                                                   UINT32 prbs_chid, 
                                                   UINT32 insert_err);
PUBLIC PMC_ERROR digi_otn_prbs_chnl_defect_cnt_get(digi_handle_t *digi_handle,
                                                   digi_otn_odu_chnl_t *chnl_ptr, 
                                                   UINT32 prbs_chid, 
                                                   UINT32 *defect_cnt);   
PUBLIC PMC_ERROR digi_otn_chnl_prbs_pattern_get(digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_t *chnl_ptr, 
                                                UINT32 prbs_chid,
                                                digi_otn_prbs_pattern_t *prbs_pattern);

PUBLIC PMC_ERROR digi_otn_odu_prbs_chid_get(digi_handle_t          *digi_handle,
                                            digi_otn_odu_chnl_t    *chnl_handle,
                                            UINT32                 *prbs_chid);

PUBLIC odu_struct_odu_level_t digi_otn_odu_level_get(digi_handle_t              *digi_handle,
                                                     digi_otn_odu_chnl_t        *chnl_handle);

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR digi_otn_prbs_dlss_status_get(digi_handle_t* digi_handle,
                                               digi_otn_odu_chnl_t *chnl_ptr,
                                               UINT32 prbs_chnl, 
                                               UINT8 *dlss_stat);                                                                                                                                                                                                                                                                                                               
/* LCOV_EXCL_STOP */



#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

PUBLIC PMC_ERROR digi_otn_get_chnl_ptr_for_tcm_apps(digi_handle_t *digi_handle,
                                                    digi_otn_odu_chnl_t *chnl_in_ptr,
                                                    digi_otn_odu_chnl_t **chnl_out_ptr);

PUBLIC PMC_ERROR digi_otn_get_chnl_ptr_for_stg3b(digi_handle_t          *digi_handle,
                                                 digi_otn_odu_chnl_t    *chnl_in_ptr,
                                                 digi_otn_odu_chnl_t   **chnl_out_ptr);

PUBLIC PMC_ERROR digi_otn_odu_maint_sig_get(digi_handle_t *digi_handle,
                                                    digi_otn_odu_chnl_t *chnl_ptr,
                                                    UINT8 direction,
                                                    coreotn_maint_sig_type_t *maint_sig);

PUBLIC PMC_ERROR digi_otn_odu_tcmi_maint_sig_get(digi_handle_t *digi_handle,
                                                    digi_otn_odu_chnl_t *chnl_ptr,
                                                    UINT8 direction,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    coreotn_maint_sig_type_t *maint_sig);


/*
** OTU related functions 
*/
PUBLIC PMC_ERROR digi_otn_otu_consq_action_cfg       ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_action_type_t     cons_action_type,
                                                       UINT32                    defect_bm,
                                                       BOOL                      set_defects);

PUBLIC PMC_ERROR digi_otn_otu_consq_action_cfg_get  ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      lineotn_action_type_t     cons_action_type,
                                                      UINT32                    defect_bm,
                                                      UINT32                    *defects_set_bm );

PUBLIC PMC_ERROR digi_otn_otu_mfas_oh_ins_cfg        ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_mfas_ins_type_t   mfas_ins_type,
                                                       UINT32                    mfas_value );

PUBLIC PMC_ERROR digi_otn_otu_mfas_oh_ins_get        ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_mfas_ins_type_t   *mfas_ins_type,
                                                       UINT32                    *mfas_value );

PUBLIC PMC_ERROR digi_otn_otu_bip8_oh_ins_cfg        ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_bip8_ins_type_t   bip8_ins_type,
                                                       UINT32                    bip8_value );

PUBLIC PMC_ERROR digi_otn_otu_bip8_oh_ins_get        ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_bip8_ins_type_t   *bip8_ins_type,
                                                       UINT32                    *bip8_value );

PUBLIC PMC_ERROR digi_otn_otu_iae_oh_ins_cfg         ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_iae_ins_type_t    iae_ins_type,
                                                       UINT32                    iae_value );

PUBLIC PMC_ERROR digi_otn_otu_iae_oh_ins_get         ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_iae_ins_type_t    *iae_ins_type,
                                                       UINT32                    *iae_value );

PUBLIC PMC_ERROR digi_otn_otu_bkwd_oh_ins_cfg        ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_bkwd_oh_field_t   bkwd_ctrl_ins_field,
                                                       lineotn_bkwd_ins_type_t   bkwd_ins_type,
                                                       UINT32                    bkwd_value );

PUBLIC PMC_ERROR digi_otn_otu_bkwd_oh_ins_get        ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_bkwd_oh_field_t   bkwd_ctrl_ins_field,
                                                       lineotn_bkwd_ins_type_t   *bkwd_ins_type,
                                                       UINT32                    *bkwd_value );

PUBLIC PMC_ERROR digi_otn_otu_oh_ins_cfg             ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_oh_field_t        ctrl_ins_field,
                                                       lineotn_oh_ins_type_t     ctrl_ins_type,
                                                       UINT32                    ctrl_field_value );

PUBLIC PMC_ERROR digi_otn_otu_oh_ins_get             ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_oh_field_t        ctrl_ins_field,
                                                       lineotn_oh_ins_type_t     *ctrl_ins_type,
                                                       UINT32                    *ctrl_field_value );

PUBLIC PMC_ERROR digi_otn_otu_oh_extract_get         ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_oh_extract_t      extract_field,
                                                       UINT32                    *extracted_val_ptr );

PUBLIC PMC_ERROR digi_otn_otu_force_maintenance_sig_cfg( digi_handle_t             *digi_handle,
                                                         digi_otn_server_chnl_t    *otn_server_ptr,
                                                         lineotn_maint_sig_type_t  maint_sig_type,
                                                         lineotn_dir_t             maint_sig_dir );

PUBLIC PMC_ERROR digi_otn_otu_force_mi_active_cfg   ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      lineotn_mi_active_type_t  mi_active_type );


PUBLIC PMC_ERROR digi_otn_otu_mi_admin_state_cfg    ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      lineotn_dir_t             admin_state_dir,
                                                      lineotn_mi_admin_state_t  admin_state );
#ifdef PMC_DEPRECATED
PUBLIC PMC_ERROR digi_otn_otu_sm_deg_set             ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_sk_sm_deg_t       deg_field,
                                                       UINT32                    deg_value );

PUBLIC PMC_ERROR digi_otn_otu_sm_deg_get             ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_sk_sm_deg_t       deg_field,
                                                       UINT32                    *deg_value );
#endif /* PMC_DEPRECATED */

PUBLIC PMC_ERROR digi_otn_otu_deg_m_set(digi_handle_t *digi_handle,
                                        digi_otn_server_chnl_t *otn_server_ptr,
                                        UINT8 deg_m);
PUBLIC PMC_ERROR digi_otn_otu_deg_m_get(digi_handle_t *digi_handle,
                                        digi_otn_server_chnl_t *otn_server_ptr,
                                        UINT8 *deg_m);
PUBLIC PMC_ERROR digi_otn_otu_deg_th_set(digi_handle_t *digi_handle,
                                         digi_otn_server_chnl_t *otn_server_ptr,
                                         UINT32 deg_th);
PUBLIC PMC_ERROR digi_otn_otu_deg_th_get(digi_handle_t *digi_handle,
                                         digi_otn_server_chnl_t *otn_server_ptr,
                                         UINT32 *deg_th);
PUBLIC PMC_ERROR digi_otn_otu_ddeg_mode_set(digi_handle_t *digi_handle, 
                                            digi_otn_ddeg_mode_t ddeg_mode);
PUBLIC PMC_ERROR digi_otn_otu_ddeg_mode_get(digi_handle_t *digi_handle,
                                            digi_otn_ddeg_mode_t *ddeg_mode);
PUBLIC PMC_ERROR digi_otn_otu_ddeg_mon_enable(digi_handle_t *digi_handle,
                                              digi_otn_server_chnl_t *otn_server_ptr,
                                              BOOL enable);
PUBLIC PMC_ERROR digi_otn_otu_enhanced_ddeg_interval_set(digi_handle_t *digi_handle,
                                                         digi_otn_server_chnl_t *otn_server_ptr,
                                                         UINT8 mon_interval);
PUBLIC PMC_ERROR digi_otn_otu_enhanced_ddeg_interval_get(digi_handle_t *digi_handle,
                                                         digi_otn_server_chnl_t *otn_server_ptr,
                                                         UINT8 *mon_interval);
PUBLIC PMC_ERROR digi_otn_odu_enhanced_ddeg_th_set(digi_handle_t                 *digi_handle,
                                                   digi_otn_odu_chnl_t           *chnl_ptr,
                                                   odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 set_threshold,
                                                   UINT32 clear_threshold);
PUBLIC PMC_ERROR digi_otn_odu_enhanced_ddeg_th_get(digi_handle_t                 *digi_handle,
                                                   digi_otn_odu_chnl_t           *chnl_ptr,
                                                   odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 *set_threshold,
                                                   UINT32 *clear_threshold);
PUBLIC PMC_ERROR digi_otn_otu_enhanced_ddeg_th_set(digi_handle_t *digi_handle,
                                                   digi_otn_server_chnl_t *otn_server_ptr,
                                                   UINT32 set_threshold,
                                                   UINT32 clear_threshold);
PUBLIC PMC_ERROR digi_otn_otu_enhanced_ddeg_th_get(digi_handle_t *digi_handle,
                                                   digi_otn_server_chnl_t *otn_server_ptr,
                                                   UINT32 *set_threshold,
                                                   UINT32 *clear_threshold);
    

PUBLIC PMC_ERROR digi_otn_otu_sm_so_tti_set          ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       lineotn_oh_ins_type_t     ctrl_ins_type,
                                                       UINT8                     tti_data[64] );

PUBLIC PMC_ERROR digi_otn_otu_sm_so_tti_get          ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       UINT8                     tti_data[64] );


PUBLIC PMC_ERROR digi_otn_otu_sm_exp_tti_set         ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       UINT8                     tti_data[64] );

PUBLIC PMC_ERROR digi_otn_otu_sm_exp_tti_get         ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       UINT8                     tti_data[64] );

PUBLIC PMC_ERROR digi_otn_exp_tti_tim_det_mode_set  ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      lineotn_sk_tim_det_mode_t exp_tti_tim_det_mode );

PUBLIC PMC_ERROR digi_otn_exp_tti_tim_det_mode_get  ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      lineotn_sk_tim_det_mode_t *exp_tti_tim_det_mode );
                                                      
PUBLIC PMC_ERROR digi_otn_otu_ebc_get( digi_handle_t             *digi_handle,
                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                       digi_pmon_lineotn_otu_frm_counters_t *ebc );

PUBLIC PMC_ERROR digi_otn_otu_sm_accepted_tti_get   ( digi_handle_t             *digi_handle,
                                                       digi_otn_server_chnl_t    *otn_server_ptr,
                                                       UINT8                     tti_data_buf[64] );

PUBLIC PMC_ERROR digi_otn_fec_type_get              ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      digi_otn_server_fec_t     *fec_type );

PUBLIC PMC_ERROR digi_otn_fec_type_set              ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      digi_otn_server_fec_t     fec_type );

PUBLIC PMC_ERROR digi_otn_fec_dec_passthru_set      ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      digi_otn_server_fec_t     fec_type,
                                                      BOOL                      dec_passthru );

PUBLIC PMC_ERROR digi_otn_fec_dec_passthru_get      ( digi_handle_t             *digi_handle,
                                                      digi_otn_server_chnl_t    *otn_server_ptr,
                                                      BOOL                      *fec_dec_in_passthru_mode );

PUBLIC PMC_ERROR digi_otn_fec_rx_mode_set( digi_handle_t             *digi_handle,
                                           digi_otn_server_chnl_t    *otn_server_ptr,
                                           digi_fec_rx_mode_t         fec_rx_mode );
                                           
PUBLIC PMC_ERROR digi_otn_fec_rx_mode_get( digi_handle_t             *digi_handle,
                                           digi_otn_server_chnl_t    *otn_server_ptr,
                                           digi_fec_rx_mode_t        *fec_rx_mode );                                           

PUBLIC PMC_ERROR digi_lineotn_bergen_set( digi_handle_t             *digi_handle,
                                          digi_otn_server_chnl_t    *otn_server_ptr,   
                                          UINT32                   logical_lanes_msk,                                         
                                          digi_bergen_lane_cfg_t   *bergen_lane_cfg_ptr,
                                          BOOL8                     enable);
PUBLIC PMC_ERROR digi_lineotn_bergen_get(   digi_handle_t           *digi_handle,
                                            digi_otn_server_chnl_t  *otn_server_ptr,
                                            UINT32                   logical_lanes_idx,
                                            digi_bergen_lane_cfg_t  *bergen_lane_cfg_ptr,
                                            BOOL8                    *enable);

PUBLIC PMC_ERROR digi_otn_maintenance_sig_state_get( digi_handle_t             *digi_handle,
                                                     digi_otn_server_chnl_t    *otn_server_ptr,
                                                     lineotn_dir_t             maint_sig_dir,
                                                     lineotn_maint_sig_type_t  *maint_sig_type_ptr );

/*
** Defect Information (DI) Ring Control Port (RCP) functions 
*/


PUBLIC PMC_ERROR digi_di_rcp_init(digi_handle_t    *digi_handle,
                                  odu_di_rcp_cfg_t *digi_di_rcp_cfg_ptr);
    
PUBLIC PMC_ERROR digi_di_rcp_uninit(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_di_rcp_init_get(digi_handle_t *digi_handle,
                                      odu_di_rcp_cfg_t *digi_di_rcp_cfg_ptr,
                                      BOOL *enable);

PUBLIC PMC_ERROR digi_di_rcp_cfg(digi_handle_t *digi_handle, 
                                 UINT32         di_rcp_pdu,
                                 BOOL8          enable);

PUBLIC PMC_ERROR digi_di_rcp_sdh_counts_cfg(digi_handle_t        *digi_handle, 
                                            BOOL8                 enable,
                                            UINT16                sdh_dir_msk,
                                            BOOL8                 no_sdh_counts);

PUBLIC PMC_ERROR digi_di_rcp_otn_counts_cfg(digi_handle_t *digi_handle,
                                            BOOL           no_otn_counts);
    
/*
** Remote Information (RI) Ring Control Port (RCP) functions 
*/

PUBLIC PMC_ERROR digi_ri_rcp_init(digi_handle_t *digi_handle,
                                  digi_otn_odu_ri_rcp_cfg_t *digi_ri_rcp_cfg_ptr);
    
PUBLIC PMC_ERROR digi_ri_rcp_uninit(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_ri_rcp_init_get(digi_handle_t *digi_handle,
                                      digi_otn_odu_ri_rcp_cfg_t *digi_ri_rcp_cfg_ptr,
                                      BOOL *enable);

PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_extract_cfg( digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *chnl_ptr,
                                                  digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                  UINT8 aps_pcc_mon_level,
                                                  BOOL8 enable);

PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_extract_cfg_get(digi_handle_t *digi_handle,
                                                     digi_otn_odu_chnl_t *chnl_ptr,
                                                     digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                     UINT8 *aps_pcc_mon_level,
                                                     BOOL8 *enable);

PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_aps_pcc_mon_level_cfg(digi_handle_t *digi_handle,
                                                           digi_otn_odu_chnl_t *chnl_ptr,
                                                           UINT8 aps_pcc_mon_level);

PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_dest_get(digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *chnl_ptr,
                                              digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr);

PUBLIC PMC_ERROR digi_otn_odu_bdi_bei_biae_insert_cfg(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT8 source);

PUBLIC PMC_ERROR digi_otn_odu_bdi_bei_biae_status_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT8 *source);
PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_aps_insert_cfg(digi_handle_t *digi_handle,
                                                    digi_otn_odu_chnl_t *chnl_ptr,                                                    
                                                    BOOL8 enable); 
PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_aps_insert_cfg_get( digi_handle_t *digi_handle,
                                                        digi_otn_odu_chnl_t *chnl_ptr,                                                     
                                                        BOOL8 *source);

PUBLIC PMC_ERROR digi_otn_ohp_init(digi_handle_t *digi_handle,
                                   odu_ohp_cfg_t *cfg_ptr);

PUBLIC PMC_ERROR digi_otn_ohp_uninit(digi_handle_t *digi_handle,
                                     odu_ohp_inst_t ohp);

PUBLIC PMC_ERROR digi_otn_ohp_init_get(digi_handle_t *digi_handle,
                                       odu_ohp_inst_t ohp,
                                       odu_ohp_cfg_t *cfg_ptr,
                                       BOOL *enable);

PUBLIC PMC_ERROR digi_otn_ohp_ch_cfg(digi_handle_t *digi_handle,
                                     digi_otn_odu_chnl_t *chnl_ptr,
                                     UINT16 extract_mask,
                                     UINT16 insert_mask,
                                     BOOL enable);
PUBLIC PMC_ERROR digi_otn_ohp_ch_cfg_get(digi_handle_t       *digi_handle,
                                         digi_otn_odu_chnl_t *chnl_ptr,
                                         UINT16              *extract_mask,
                                         UINT16              *insert_mask,
                                         BOOL                *enable);

PUBLIC PMC_ERROR digi_otn_ohp_ch_extract_cfg(digi_handle_t       *digi_handle,
                                             digi_otn_odu_chnl_t *chnl_ptr,
                                             UINT16               extract_mask,
                                             BOOL                 enable);

PUBLIC PMC_ERROR digi_otn_ohp_ch_extract_cfg_get(digi_handle_t       *digi_handle,
                                                 digi_otn_odu_chnl_t *chnl_ptr,
                                                 UINT16              *extract_mask,
                                                 BOOL                *enable);

PUBLIC PMC_ERROR digi_otn_ohp_ch_insert_cfg(digi_handle_t       *digi_handle,
                                            digi_otn_odu_chnl_t *chnl_ptr,
                                            UINT16               insert_mask,
                                            BOOL                 enable);

PUBLIC PMC_ERROR digi_otn_ohp_ch_insert_cfg_get(digi_handle_t       *digi_handle,
                                                digi_otn_odu_chnl_t *chnl_ptr,
                                                UINT16              *insert_mask,
                                                BOOL                *enable);

PUBLIC PMC_ERROR digi_otn_ohp_ch_resize(digi_handle_t *digi_handle,
                                        digi_otn_odu_chnl_t *chnl_ptr,
                                        INT32 resize);

PUBLIC PMC_ERROR digi_otn_odu_ri_rcp_resize(digi_handle_t *digi_handle,
                                            digi_otn_odu_chnl_t *chnl_ptr,
                                            INT32 resize);

PUBLIC PMC_ERROR digi_otn_ohp_channel_framer_num_get(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *chnl_ptr,
                                                      UINT32 *channel_num_ptr,
                                                      odu_ohp_port_framer_t *framer_num_ptr);

PUBLIC PMC_ERROR digi_otn_otu_fec_stat_intf_init(digi_handle_t *digi_handle,
                                                 digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr);
PUBLIC PMC_ERROR digi_otn_otu_fec_stat_intf_uninit(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_otn_otu_fec_stat_set(digi_handle_t           *digi_handle,
                                           digi_otn_server_chnl_t  *otn_server_ptr,
                                           BOOL8 enable);

PMC_ERROR digi_otn_pmon_retrieve(digi_handle_t      *digi_handle,
                                 digi_otn_pmon_t    *pmon_table_ptr);

PUBLIC PMC_ERROR digi_otn_server_pmon_chnl_get(digi_handle_t                *digi_handle,
                                               digi_otn_server_chnl_t       *chnl_handle,
                                               digi_otn_pmon_t              *pmon_table_ptr,
                                               digi_otn_server_pmon_chnl_t **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_sysotn_server_pmon_chnl_get(digi_handle_t                *digi_handle,
                                                  digi_otn_server_chnl_t       *chnl_handle,
                                                  digi_otn_pmon_t              *pmon_table_ptr,
                                                  digi_otn_server_pmon_chnl_t **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_otn_odu_pmon_chnl_get(digi_handle_t                *digi_handle,
                                            digi_otn_odu_chnl_t          *chnl_handle,
                                            digi_otn_pmon_t              *pmon_table_ptr,
                                            digi_otn_odu_pmon_chnl_t    **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_otn_int_register_cb(digi_handle_t      *digi_handle,
                                          void               *token,
                                          UINT32              token_size,
                                          digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_register_cb(digi_handle_t           *digi_handle,
                                                      void                    *token,
                                                      UINT32                   token_size,
                                                      digi_int_cb_fcn          user_cb);

PUBLIC PMC_ERROR digi_sysotn_server_int_chnl_register_cb(digi_handle_t           *digi_handle,
                                                         void                    *token,
                                                         UINT32                   token_size,
                                                         digi_int_cb_fcn          user_cb);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_register_cb(digi_handle_t           *digi_handle,
                                                   void                    *token,
                                                   UINT32                   token_size,
                                                   digi_int_cb_fcn          user_cb);

PUBLIC PMC_ERROR digi_otn_server_int_enable(digi_handle_t        *digi_handle,
                                            lineotn_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_server_int_disable(digi_handle_t        *digi_handle,
                                             lineotn_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_enable(digi_handle_t        *digi_handle,
                                               lineotn_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_disable(digi_handle_t        *digi_handle,
                                                lineotn_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_enable(digi_handle_t             *digi_handle,
                                                 digi_otn_server_chnl_t    *chnl_handle,   
                                                 lineotn_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_disable(digi_handle_t             *digi_handle,
                                                  digi_otn_server_chnl_t    *chnl_handle,   
                                                  lineotn_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_chnl_enable(digi_handle_t             *digi_handle,
                                                    digi_otn_server_chnl_t    *chnl_handle,   
                                                    lineotn_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_chnl_disable(digi_handle_t             *digi_handle,
                                                     digi_otn_server_chnl_t    *chnl_handle,   
                                                     lineotn_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_coreotn_enable(digi_handle_t             *digi_handle,
                                             coreotn_int_t             *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_coreotn_disable(digi_handle_t             *digi_handle,
                                              coreotn_int_t             *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_coreotn_filt_retrieve(digi_handle_t              *digi_handle,
                                                    coreotn_int_t              *filt_table_ptr,
                                                    coreotn_int_t              *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_coreotn_clear(digi_handle_t           *digi_handle,
                                            coreotn_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_framer_chnl_enable(digi_handle_t             *digi_handle,
                                                     digi_otn_odu_chnl_t       *chnl_handle,
                                                     coreotn_int_framer_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_framer_chnl_disable(digi_handle_t             *digi_handle,
                                                      digi_otn_odu_chnl_t       *chnl_handle,
                                                      coreotn_int_framer_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_pm_chnl_enable(digi_handle_t          *digi_handle,
                                                 digi_otn_odu_chnl_t    *chnl_handle);

PUBLIC PMC_ERROR digi_otn_odu_int_pm_chnl_disable(digi_handle_t          *digi_handle,
                                                  digi_otn_odu_chnl_t    *chnl_handle);


PUBLIC PMC_ERROR digi_otn_odu_int_pid_chnl_enable(digi_handle_t                 *digi_handle,
                                                  digi_otn_odu_chnl_t           *chnl_handle,
                                                  digi_otn_int_tcm_num_t         tcm_num,
                                                  odu_rfrm_int_pid_chnl_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_prbs_chnl_enable(digi_handle_t             *digi_handle,
                                                   digi_otn_odu_chnl_t       *chnl_handle,
                                                   coreotn_int_prbs_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_pid_enable(digi_handle_t             *digi_handle,
                                             odu_struct_odu_level_t     odu_level,
                                             UINT32                     pool_id);

PUBLIC PMC_ERROR digi_otn_odu_int_pid_disable(digi_handle_t             *digi_handle,
                                              odu_struct_odu_level_t     odu_level,
                                              UINT32                     pool_id);

PUBLIC PMC_ERROR digi_otn_odu_int_defect_enable(digi_handle_t           *digi_handle,
                                                odu_struct_odu_level_t   odu_level,
                                                odu_rfrm_int_defect_t    defect);

PUBLIC PMC_ERROR digi_otn_odu_int_defect_disable(digi_handle_t           *digi_handle,
                                                 odu_struct_odu_level_t   odu_level,
                                                 odu_rfrm_int_defect_t    defect);

PUBLIC PMC_ERROR digi_otn_odu_int_prbs_chnl_disable(digi_handle_t             *digi_handle,
                                                    digi_otn_odu_chnl_t       *chnl_handle,
                                                    coreotn_int_prbs_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_pid_chnl_disable(digi_handle_t                 *digi_handle,
                                                   digi_otn_odu_chnl_t           *chnl_handle,
                                                   digi_otn_int_tcm_num_t         tcm_num,
                                                   odu_rfrm_int_pid_chnl_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_stg3b_chnl_enable(digi_handle_t             *digi_handle,
                                                    digi_otn_odu_chnl_t       *chnl_handle,
                                                    coreotn_int_framer_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_stg3b_chnl_disable(digi_handle_t             *digi_handle,
                                                     digi_otn_odu_chnl_t       *chnl_handle,
                                                     coreotn_int_framer_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_chnl_enable(digi_handle_t             *digi_handle,
                                                  odu_ohp_inst_t              ohp,
                                                  digi_otn_odu_chnl_t       *chnl_handle,
                                                  odu_ohp_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_chnl_disable(digi_handle_t             *digi_handle,
                                                   odu_ohp_inst_t              ohp,
                                                   digi_otn_odu_chnl_t       *chnl_handle,
                                                   odu_ohp_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_chnl_enable(digi_handle_t             *digi_handle,
                                                     digi_otn_odu_chnl_t       *chnl_handle,
                                                     odu_ri_rcp_int_chnl_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_chnl_disable(digi_handle_t             *digi_handle,
                                                      digi_otn_odu_chnl_t       *chnl_handle,
                                                      odu_ri_rcp_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_ddeg_int_start(digi_handle_t           *digi_handle,
                                         UINT32                   unused);

PUBLIC PMC_ERROR digi_otn_ddeg_int_end(digi_handle_t            *digi_handle,
                                       UINT32                    unused);

PUBLIC PMC_ERROR digi_otn_int_retrieve(digi_handle_t      *digi_handle,
                                       digi_otn_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_filt_retrieve(digi_handle_t           *digi_handle,
                                            digi_otn_int_filter_t   *int_filter_ptr,
                                            digi_otn_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_clear(digi_handle_t       *digi_handle,
                                    digi_otn_int_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_lineotn_int_clear(digi_handle_t        *digi_handle,
                                            lineotn_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_sysotn_int_clear(digi_handle_t        *digi_handle,
                                           lineotn_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_lineotn_int_filt_retrieve(digi_handle_t       *digi_handle,
                                                    lineotn_int_t       *filt_table_ptr,
                                                    lineotn_int_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_sysotn_int_filt_retrieve(digi_handle_t       *digi_handle,
                                                   lineotn_int_t       *filt_table_ptr,
                                                   lineotn_int_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_clear(digi_handle_t             *digi_handle,
                                                digi_otn_server_chnl_t    *chnl_handle,   
                                                lineotn_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_filt_retrieve(digi_handle_t            *digi_handle,
                                                        digi_otn_server_chnl_t   *chnl_handle,
                                                        lineotn_int_chnl_t       *filt_table_ptr,
                                                        lineotn_int_chnl_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_filt_retrieve(digi_handle_t              *digi_handle,
                                                     digi_otn_odu_chnl_t        *chnl_handle,
                                                     coreotn_int_framer_chnl_t  *status_filt_table_ptr,
                                                     odu_rfrm_int_pid_chnl_t    *pid_filt_table_ptr,
                                                     digi_otn_odu_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_clear(digi_handle_t              *digi_handle,
                                             digi_otn_odu_chnl_t        *chnl_handle,
                                             digi_otn_odu_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_rfrm_chnl_clear(digi_handle_t              *digi_handle,
                                              digi_otn_odu_chnl_t        *chnl_handle);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_chnl_filt_retrieve(digi_handle_t            *digi_handle,
                                                         odu_ohp_inst_t            ohp,
                                                         digi_otn_odu_chnl_t      *chnl_handle,
                                                         odu_ohp_int_chnl_t       *ohp_filt_table_ptr,
                                                         odu_ohp_int_chnl_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_chnl_filt_retrieve(digi_handle_t          *digi_handle,
                                                            digi_otn_odu_chnl_t    *chnl_handle,
                                                            odu_ri_rcp_int_chnl_t  *ohp_filt_table_ptr,
                                                            odu_ri_rcp_int_chnl_t  *int_table_ptr);


PUBLIC PMC_ERROR digi_otn_odu_int_start(digi_handle_t           *digi_handle, 
                                        coreotn_latch_defect_t   latch_defect);

PUBLIC PMC_ERROR digi_otn_odu_int_end(digi_handle_t           *digi_handle,
                                      coreotn_latch_defect_t   latch_defect);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_get(digi_handle_t                *digi_handle,
                                              digi_otn_server_chnl_t       *chnl_handle,
                                              digi_otn_int_t               *int_table_ptr,
                                              lineotn_int_chnl_t          **int_chnl_table_pptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_chnl_get(digi_handle_t                *digi_handle,
                                                 digi_otn_server_chnl_t       *chnl_handle,
                                                 digi_otn_int_t               *int_table_ptr,
                                                 lineotn_int_chnl_t          **int_chnl_table_pptr);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_get(digi_handle_t                *digi_handle,
                                           digi_otn_odu_chnl_t          *chnl_handle,
                                           digi_otn_int_t               *int_table_ptr,
                                           digi_otn_odu_int_chnl_t     **int_chnl_table_pptr);

PUBLIC PMC_ERROR digi_otn_int_enabled_get(digi_handle_t      *digi_handle,
                                          digi_otn_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_int_enabled_check(digi_handle_t   *digi_handle);

PUBLIC PMC_ERROR digi_otn_server_int_summary_get(digi_handle_t                 *digi_handle,
                                                 UINT32  otn_server_type,
                                                 digi_otn_server_int_summary_t *int_summary_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_summary_get(digi_handle_t                 *digi_handle,
                                              odu_struct_odu_level_t         odu_level,
                                              UINT32                         summary_mask,
                                              digi_otn_odu_int_summary_t    *int_summary_ptr);

PUBLIC PMC_ERROR digi_otn_server_int_chnl_enabled_check(digi_handle_t                 *digi_handle,
                                                        digi_otn_server_int_summary_t *int_summary_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_chnl_enabled_check(digi_handle_t                 *digi_handle,
                                                           digi_otn_server_int_summary_t *int_summary_ptr);

PUBLIC PMC_ERROR digi_sysotn_server_int_chnl_clear(digi_handle_t             *digi_handle,
                                                   digi_otn_server_chnl_t    *chnl_handle,   
                                                   lineotn_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_enabled_check(digi_handle_t                  *digi_handle,
                                                     odu_struct_odu_level_t          odu_level,
                                                     digi_otn_odu_int_summary_t     *int_summary_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_enable(digi_handle_t             *digi_handle,
                                             odu_ohp_inst_t             ohp,
                                             odu_ohp_int_t             *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_disable(digi_handle_t             *digi_handle,
                                              odu_ohp_inst_t             ohp,
                                              odu_ohp_int_t             *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_filt_retrieve(digi_handle_t              *digi_handle,
                                                    odu_ohp_inst_t              ohp,
                                                    odu_ohp_int_t              *filt_table_ptr,
                                                    odu_ohp_int_t              *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ohp_clear(digi_handle_t     *digi_handle,
                                            odu_ohp_inst_t     ohp,
                                            odu_ohp_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_di_rcp_enable(digi_handle_t          *digi_handle,
                                                odu_rcp_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_di_rcp_disable(digi_handle_t          *digi_handle,
                                                 odu_rcp_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_di_rcp_filt_retrieve(digi_handle_t           *digi_handle,
                                                       odu_rcp_int_t           *filt_table_ptr,
                                                       odu_rcp_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_di_rcp_clear(digi_handle_t        *digi_handle,
                                               odu_rcp_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_enable(digi_handle_t          *digi_handle,
                                                odu_rcp_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_disable(digi_handle_t          *digi_handle,
                                                 odu_rcp_int_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_filt_retrieve(digi_handle_t           *digi_handle,
                                                       odu_rcp_int_t           *filt_table_ptr,
                                                       odu_rcp_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ri_rcp_clear(digi_handle_t        *digi_handle,
                                               odu_rcp_int_t        *int_table_ptr);

PUBLIC void digi_otn_fege_mstat_counter_access(digi_handle_t *digi_handle,
                                               util_global_coreotn_sgmii_inst_t inst,
                                               util_global_counter_action_t action,
                                               digi_pmon_coreotn_fege_mstat_t *counts_ptr);



PUBLIC UINT32 digi_otn_server_chnl_num_get(digi_handle_t              *digi_handle,
                                           digi_otn_server_chnl_t     *chnl_handle);
PUBLIC UINT32 digi_otn_server_stream_num_get(digi_handle_t              *digi_handle,
                                             digi_otn_server_chnl_t     *chnl_handle);
PUBLIC UINT32 digi_otn_odu_chnl_num_get(digi_handle_t              *digi_handle,
                                        digi_otn_odu_chnl_t        *chnl_handle);

/*
** ODUKSW Latency Measurement Tools
*/

PUBLIC PMC_ERROR digi_oduksw_mpmo_debug_latency_get(digi_handle_t *digi_handle,
                                                    util_global_switch_data_t *output_pc_ptr,
                                                    UINT32 *debug_latency_ptr);

PUBLIC PMC_ERROR digi_oduksw_mpmo_raw_latency_get(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *output_pc_ptr,
                                                  UINT32 *minimum_raw_latency,
                                                  UINT32 *maximum_raw_latency);

PUBLIC void digi_ri_rcp_cfc_int_sel_get(digi_handle_t          *digi_handle,
                                        odu_rcp_frm_id_t        *framer_id_ptr,
                                        util_global_direction_t *src_dir_ptr);

PUBLIC PMC_ERROR digi_ri_rcp_cfc_int_sel_set(digi_handle_t           *digi_handle,
                                             odu_rcp_frm_id_t         framer_id,
                                             util_global_direction_t  src_dir);

PUBLIC PMC_ERROR digi_ri_rcp_cfc_int_type_sel_set(digi_handle_t             *digi_handle,
                                                  digi_otn_odu_chnl_t       *chnl_ptr,
                                                  odu_ri_rcp_cfc_int_type_t  int_type);

PUBLIC PMC_ERROR digi_ri_rcp_cfc_int_type_sel_get(digi_handle_t              *digi_handle,
                                                  digi_otn_odu_chnl_t        *chnl_ptr,
                                                  odu_ri_rcp_cfc_int_type_t  *int_type_ptr);

PUBLIC PMC_ERROR digi_otn_server_sfis_odd_parity_set(digi_handle_t             *digi_handle,
                                                     digi_otn_server_chnl_t    *otn_server_ptr,
                                                     util_global_direction_t     dir,
                                                     BOOL8                       inv);

PUBLIC PMC_ERROR digi_otn_server_sfis_odd_parity_get(digi_handle_t             *digi_handle,
                                                     digi_otn_server_chnl_t    *otn_server_ptr,
                                                     util_global_direction_t     dir,
                                                     BOOL8                      *inv_ptr);

PUBLIC PMC_ERROR digi_otn_server_line_tx_rx_dsi_lane_align(digi_handle_t          *digi_handle,
                                                           digi_otn_server_chnl_t *otn_server_ptr,
                                                           BOOL8                   enable);


PUBLIC PMC_ERROR digi_otn_pm_status_get(digi_handle_t *digi_handle,
                                        digi_otn_odu_chnl_t *chnl_handle,
                                        odu_struct_odu_level_t odu_level,
                                        util_global_otn_pm_status_t *pm_table_ptr);


PUBLIC PMC_ERROR digi_lineotn_mld_dlom_to_ffail_mapping_set(digi_handle_t   *digi_handle,
                                                            digi_otn_server_chnl_t *otn_server_ptr,
                                                            BOOL8 enable);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_OTN_API_H */

/*
** end of file
*/
