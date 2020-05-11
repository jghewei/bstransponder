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
*   DESCRIPTION:
*     common types for Digi blocks
*
*   NOTES:
*
********************************************************************************/

#ifndef UTIL_GLOBAL_H
#define UTIL_GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "util_global_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: util_global_parent_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Parent TSB info.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_TGMP_RGMP = 0,
    UTIL_GLOBAL_MUX_DEMUX = 1,
    
    UTIL_GLOBAL_LAST_PARENT,
} util_global_parent_t;

/*******************************************************************************
* ENUM: util_global_map_adapt_func_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for otn payload mapping modes.  The element
*   terms have a syntax: \n
*      UTIL_GLOBAL_MAP_<odu container>_<client_data_type>_<mapping_mode> \n\n
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_MAP_NO_RATE                  = 0,
    UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP         = 1,
    UTIL_GLOBAL_MAP_ODU4P_100_GE_GFP         = 2,
    UTIL_GLOBAL_MAP_ODU3P_40_GE_GFP          = 3,
    UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP      = 4,
    UTIL_GLOBAL_MAP_ODU2P_10_GE_6_2_GFP      = 5,
    UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP       = 6,
    UTIL_GLOBAL_MAP_ODU2P_FC_800_GMP         = 7,
    UTIL_GLOBAL_MAP_ODU2EP_10G_GDPS_GMP      = 8,
    UTIL_GLOBAL_MAP_ODU2P_5G_GDPS_GMP        = 9,
    UTIL_GLOBAL_MAP_ODU2P_CPRI_5_GMP         = 10,
    UTIL_GLOBAL_MAP_ODU2P_CPRI_6_GMP         = 11,
    UTIL_GLOBAL_MAP_ODU2P_CPRI_7_GMP         = 12,
    UTIL_GLOBAL_MAP_ODU3P_40_GE_GMP          = 13,
    UTIL_GLOBAL_MAP_ODU4P_100_GE_GMP         = 14,
    UTIL_GLOBAL_MAP_ODU3E2P_40_G_CBR_GMP     = 15,
    UTIL_GLOBAL_MAP_ODUJP_CBRX_GMP           = 16,
    UTIL_GLOBAL_MAP_ODUFLEX_5G_GDPS_BMP      = 17,
    UTIL_GLOBAL_MAP_ODUFLEX_10G_GDPS_BMP     = 18,
    UTIL_GLOBAL_MAP_ODUFLEX_CPRI_5_BMP       = 19,
    UTIL_GLOBAL_MAP_ODUFLEX_CPRI_6_BMP       = 20,
    UTIL_GLOBAL_MAP_ODUFLEX_CPRI_7_BMP       = 21,
    UTIL_GLOBAL_MAP_ODUFLEX_FC_800_BMP       = 22,
    UTIL_GLOBAL_MAP_ODU2EP_FC_1200_BMP       = 23,
    UTIL_GLOBAL_MAP_ODU2FP_FC_1200_BMP       = 24,
    UTIL_GLOBAL_MAP_ODU1FP_FC_1200_BMP       = 25,
    UTIL_GLOBAL_MAP_ODU1EP_CBR_10G3_7_2_BMP  = 26,
    UTIL_GLOBAL_MAP_ODU2EP_CBR_10G3_7_1_BMP  = 27,
    UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_BMP     = 28,
    UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_BMP     = 29,
    UTIL_GLOBAL_MAP_ODUKP_CBRX_B_A_BMP       = 30,
    UTIL_GLOBAL_MAP_ODU3P_OC_768_CBR_AMP     = 31,
    UTIL_GLOBAL_MAP_ODU2P_OC_192_CBR_AMP     = 32,
    UTIL_GLOBAL_MAP_ODUKP_CBRX_A_AMP         = 33,
} util_global_map_adapt_func_t;

/*******************************************************************************
* ENUM: util_global_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store channel state types.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_CHNL_START        = 0,
    UTIL_GLOBAL_CHNL_EQUIPPED     = 1,
    UTIL_GLOBAL_CHNL_OPERATIONAL  = 2,
    UTIL_GLOBAL_CHNL_PASSTHRU     = 3,
    UTIL_GLOBAL_CHNL_BACKUP       = 4,
    LAST_UTIL_GLOBAL_CHNL_STATE
    
} util_global_chnl_state_t;

/*******************************************************************************
* ENUM: util_global_restart_init_phase_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store restart init phase
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST     = 0,
    UTIL_GLOBAL_RESTART_INIT_PHASE_SECOND    = 1,
    LAST_UTIL_GLOBAL_RESTART_INIT_PHASE
} util_global_restart_init_phase_t;

/*******************************************************************************
* ENUM: util_global_odtu_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store payload types processed in COREOTN.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_ODTU_TYPE_UNALLOC   = 0,
    UTIL_GLOBAL_ODTU_TYPE_ODTU01    = 1,
    UTIL_GLOBAL_ODTU_TYPE_ODTU12    = 2,
    UTIL_GLOBAL_ODTU_TYPE_ODTU13    = 3,
    UTIL_GLOBAL_ODTU_TYPE_ODTU23    = 4,
    UTIL_GLOBAL_ODTU_TYPE_ODTU2_TS  = 5,
    UTIL_GLOBAL_ODTU_TYPE_ODTU3_TS  = 6,
    UTIL_GLOBAL_ODTU_TYPE_ODTU4_TS  = 7,
} util_global_odtu_type_t;


/*******************************************************************************
* ENUM: util_global_cpb_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the client port using the CPB.
*
*   The element terms have a syntax:
*      UTIL_GLOBAL_CPB_PORT_<subsystem_block>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_CPB_PORT_ILKN1 = 0,
    UTIL_GLOBAL_CPB_PORT_ILKN2,
    UTIL_GLOBAL_CPB_PORT_CBRC,
    UTIL_GLOBAL_CPB_PORT_MAPOTN,
    UTIL_GLOBAL_CPB_PORT_ENET_LINE,
    UTIL_GLOBAL_CPB_PORT_ENET_SYS,
    UTIL_GLOBAL_CPB_PORT_NOT_USED,
    LAST_UTIL_GLOBAL_CPB_PORT  /* used for out of range values error handling */
} util_global_cpb_port_t;

/*******************************************************************************
* ENUM: util_global_mcpb_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the client port using the MCPB.
*
*   The element terms have a syntax:
*      UTIL_GLOBAL_CPB_PORT_<subsystem_block>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_MCPB_PORT_ENET_LINE = 0,
    UTIL_GLOBAL_MCPB_PORT_ENET_SYS, 
    UTIL_GLOBAL_MCPB_PORT_CPU,
    UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE,
    UTIL_GLOBAL_MCPB_PORT_NOT_USED,
    LAST_UTIL_GLOBAL_MCPB_PORT  /* used for out of range values error handling */
} util_global_mcpb_port_t;

/*******************************************************************************
* ENUM: util_global_oduk_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the client port using the ODUKSW.
*
*   The element terms have a syntax:
*      UTIL_GLOBAL_ODUK_PORT_<subsystem_block>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_ODUK_PORT_ILKN1 = 0,
    UTIL_GLOBAL_ODUK_PORT_ILKN2,
    UTIL_GLOBAL_ODUK_PORT_COREOTN,
    UTIL_GLOBAL_ODUK_PORT_MAPOTN,
    /*UTIL_GLOBAL_ODUK_PORT_COREOTN_LINESIDE,  used for SYSOTN application */
    UTIL_GLOBAL_ODUK_PORT_NOT_USED,
    LAST_UTIL_GLOBAL_ODUK_PORT /* used for out of range values error handling */
} util_global_oduk_port_t;

/*******************************************************************************
* ENUM: util_global_chnl_provisioned_state_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration defining the provisioned state of a handle. 
*   This keeps track of the current provisioning state of the handle.
*
*   The element terms have a syntax:
*      UTIL_GLOBAL_CHNL_PROV_STATE_<state>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG = 0,
    UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG,
    UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED,
    UTIL_GLOBAL_CHNL_PROV_STATE_UNUSED,  /* used for out of range values error handling */
    UTIL_GLOBAL_CHNL_PROV_STATE_RX_PROVISIONED,
    UTIL_GLOBAL_CHNL_PROV_STATE_TX_PROVISIONED,
} util_global_chnl_provisioned_state_t;

/*******************************************************************************
* ENUM: util_global_chnl_activated_state_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration defining the activation state of a handle. 
*   This keeps track of the current activation state of the handle.
*
*   The element terms have a syntax:
*      UTIL_GLOBAL_CHNL_ACTIVE_STATE_<state>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {

    UTIL_GLOBAL_CHNL_ACTIVE_STATE_INACTIVE = 0,
    UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE,
    LAST_UTIL_GLOBAL_CHNL_ACTIVE_STATE  /* used for out of range values error handling */
} util_global_chnl_activated_state_t;

/*******************************************************************************
* ENUM: util_global_odu_level_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the level of ODU channel 
*   connecting at the ODUK (HO/MO/LO)
*
* ELEMENTS:
*    UTIL_GLOBAL_LEVEL_HO_ODU - ODU channel level for HO ODU
*    UTIL_GLOBAL_LEVEL_MO_ODU - ODU channel level for MO ODU
*    UTIL_GLOBAL_LEVEL_LO_ODU - ODU channel level for LO ODU
*    UTIL_GLOBAL_LEVEL_3B_ODU - ODU channel level for ODU 3B stage 
*    UTIL_GLOBAL_LEVEL_4_ODU  - ODU channel level for ODU 4 stage
*    UTIL_GLOBAL_LEVEL_SYSOTN_LINE - ODU channel level for 
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_LEVEL_HO_ODU  = 0,
    UTIL_GLOBAL_LEVEL_MO_ODU,
    UTIL_GLOBAL_LEVEL_LO_ODU,
    UTIL_GLOBAL_LEVEL_3B_ODU,
    UTIL_GLOBAL_LEVEL_4_ODU,
    UTIL_GLOBAL_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC,
    
    LAST_UTIL_GLOBAL_LEVEL                    /* this is for out of range values error handling */
} util_global_odu_level_t;

/*******************************************************************************
* ENUM: util_global_datapath_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure enumerating datapath direction possible values.
*
* ELEMENTS:
*   UTIL_GLOBAL_DATAPATH_DIR_EGRESS     - egress direction
*   UTIL_GLOBAL_DATAPATH_DIR_INGRESS    - ingress direction
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_DATAPATH_DIR_EGRESS = 0x0,
    UTIL_GLOBAL_DATAPATH_DIR_INGRESS = 0x1,

    LAST_UTIL_GLOBAL_DATAPATH_DIR
}util_global_datapath_dir_t;

/*******************************************************************************
* ENUM: util_global_otn_msi_cfg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure enumerating MSI configuration mode for OTN payload.
*
* ELEMENTS:
*   UTIL_GLOBAL_OTN_MSI_CFG_AUTO    - MSI is configured by SW driver as part of 
*                                     datapath provisioning
*   UTIL_GLOBAL_OTN_MSI_CFG_MAN     - MSI is not configured as part of datapath
*                                     provisioning
*******************************************************************************/
typedef enum {
    UTIL_GLOBAL_OTN_MSI_CFG_AUTO   = 0,
    UTIL_GLOBAL_OTN_MSI_CFG_MAN    = 1,
} util_global_otn_msi_cfg_mode_t;


/*******************************************************************************
* ENUM: util_global_ghao_lcr_ctrl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for encoding the RCOH CTRL field
*
* ELEMENTS:
*    UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE   - RCOH CTRL field = IDLE
*    UTIL_GLOBAL_GHAO_LCR_CTRL_ADD    - RCOH CTRL field = ADD
*    UTIL_GLOBAL_GHAO_LCR_CTRL_REMOVE - RCOH CTRL field = REMOVE
*    UTIL_GLOBAL_GHAO_LCR_CTRL_NORM   - RCOH CTRL field = NORM
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_GHAO_LCR_CTRL_IDLE    = 0,
    UTIL_GLOBAL_GHAO_LCR_CTRL_ADD     = 1,
    UTIL_GLOBAL_GHAO_LCR_CTRL_REMOVE  = 2,
    UTIL_GLOBAL_GHAO_LCR_CTRL_NORM    = 3,
    LAST_UTIL_GLOBAL_GHAO_LCR_CTRL                    /* this is for out of range values error handling */
} util_global_ghao_lcr_ctrl_t;

/*******************************************************************************
* ENUM: util_global_ghao_lcr_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for encoding the G.HAO LCR states
*
* ELEMENTS:
*   UTIL_GLOBAL_GHAO_LCR_STATE_IDLE            - LCR IDLE state
*   UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK        = LCR ADD NACK state
*   UTIL_GLOBAL_GHAO_LCR_STATE_ADD_ACK         = LCR ADD ACK state
*   UTIL_GLOBAL_GHAO_LCR_STATE_INCR_NORM       = LCR Increase NORM state
*   UTIL_GLOBAL_GHAO_LCR_STATE_INCR_IDLE_NACK  = LCR Increase IDLE NACK state
*   UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY = LCR Increase TSCC relay state
*   UTIL_GLOBAL_GHAO_LCR_STATE_REM_NACK        = LCR REM NACK state
*   UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY = LCR Decrease TSCC relay state
*   UTIL_GLOBAL_GHAO_LCR_STATE_REM_ACK         = LCR REM ACK state
*   UTIL_GLOBAL_GHAO_LCR_STATE_DECR_NORM       = LCR Decrease NORM state
*   UTIL_GLOBAL_GHAO_LCR_STATE_DECR_IDLE_NACK  = LCR Decreate IDLE NACK state
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_GHAO_LCR_STATE_IDLE            = 0,
    UTIL_GLOBAL_GHAO_LCR_STATE_ADD_NACK        = 1,
    UTIL_GLOBAL_GHAO_LCR_STATE_ADD_ACK         = 2,
    UTIL_GLOBAL_GHAO_LCR_STATE_INCR_NORM       = 3,
    UTIL_GLOBAL_GHAO_LCR_STATE_INCR_IDLE_NACK  = 4,
    UTIL_GLOBAL_GHAO_LCR_STATE_INCR_TSCC_RELAY = 5,
    UTIL_GLOBAL_GHAO_LCR_STATE_REM_NACK        = 6,
    UTIL_GLOBAL_GHAO_LCR_STATE_DECR_TSCC_RELAY = 7,
    UTIL_GLOBAL_GHAO_LCR_STATE_REM_ACK         = 8,
    UTIL_GLOBAL_GHAO_LCR_STATE_DECR_NORM       = 9,
    UTIL_GLOBAL_GHAO_LCR_STATE_DECR_IDLE_NACK  = 10,
    LAST_UTIL_GLOBAL_GHAO_LCR_STATE                    /* this is for out of range values error handling */
} util_global_ghao_lcr_state_t;

/*******************************************************************************
* ENUM: util_global_ghao_bwr_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for encoding the G.HAO BWR states
*
* ELEMENTS:
*    UTIL_GLOBAL_GHAO_BWR_STATE_IDLE            - BWR IDLE state
*    UTIL_GLOBAL_GHAO_BWR_STATE_WAIT_TSCC_RELAY - BWR Wait TSCC relay state
*    UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_NACK       - BWR TSCC NACK state
*    UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_ACK        - BWR TSCC ACK state
*    UTIL_GLOBAL_GHAO_BWR_STATE_BWR_IND         - BWR BWR_IND state
*    UTIL_GLOBAL_GHAO_BWR_STATE_NO_TSCC_ACK     - BWR No TSCC ACK state
*    UTIL_GLOBAL_GHAO_BWR_STATE_NO_TSCC_NACK    - BWR No TSCC NACK state
*    UTIL_GLOBAL_GHAO_BWR_STATE_LCR_REACT_IND   - BWR LCR Reactive Indication state
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_GHAO_BWR_STATE_IDLE            = 0,
    UTIL_GLOBAL_GHAO_BWR_STATE_WAIT_TSCC_RELAY = 1,
    UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_NACK       = 2,
    UTIL_GLOBAL_GHAO_BWR_STATE_TSCC_ACK        = 3,
    UTIL_GLOBAL_GHAO_BWR_STATE_BWR_IND         = 4,
    UTIL_GLOBAL_GHAO_BWR_STATE_NO_TSCC_ACK     = 5,
    UTIL_GLOBAL_GHAO_BWR_STATE_NO_TSCC_NACK    = 6,
    UTIL_GLOBAL_GHAO_BWR_STATE_LCR_REACT_IND   = 7,
    LAST_UTIL_GLOBAL_GHAO_BWR_STATE                    /* this is for out of range values error handling */
} util_global_ghao_bwr_state_t;


/*******************************************************************************
* ENUM: util_global_calendar_pinch_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enum type for defining a custom calendar. 
*
* ELEMENTS:
*    UTIL_GLOBAL_NO_PINCH            - No custom calendar pinch (default)
*  UTIL_GLOBAL_ODU4_ODUKSW_TX_DPI  - Pinch for COREOTN ODU4 for ODUKSW DCS
*                                    port 2 (DPI) calendar
*  UTIL_GLOBAL_ODU4_ODUKSW_TX_DCI  - Pinch for COREOTN ODU4 for ODUKSW DCS
*                                    port 2 for the MPMO SCBS3 (DCI) calendar 
*  
*******************************************************************************/
typedef enum
{
    UTIL_GLOBAL_CAL_PINCH_NONE                    = 0,
    UTIL_GLOBAL_CAL_PINCH_ODU4_ODUKSW_TX_DPI = 1,
    UTIL_GLOBAL_CAL_PINCH_ODU4_ODUKSW_TX_DCI = 2,
  UTIL_GLOBAL_CAL_PINCH_LAST
} util_global_calendar_pinch_t;


/*
** Constants
*/

/* ILKN */
#define UTIL_GLOBAL_ILKN_FC_ENTRIES           300 
#define UTIL_GLOBAL_32BIT_ILKN_FC_ENTRIES     10 

/* Client bit rates (in Hz) */
/* Ethernet */
#define UTIL_GLOBAL_ENET_10GE_RATE     10312500E3
#define UTIL_GLOBAL_ENET_40GE_RATE     4 * UTIL_GLOBAL_ENET_10GE_RATE
#define UTIL_GLOBAL_ENET_40GE_XCD_RATE (DOUBLE)1027/1056 * UTIL_GLOBAL_ENET_40GE_RATE 
#define UTIL_GLOBAL_ENET_100GE_RATE    10 * UTIL_GLOBAL_ENET_10GE_RATE

/* Interlaken */
#define UTIL_GLOBAL_ILKN_3G125_RATE   3125000E3
#define UTIL_GLOBAL_ILKN_6G25_RATE    6250000E3
#define UTIL_GLOBAL_ILKN_10G3125_RATE 10312500E3
#define UTIL_GLOBAL_ILKN_11G5_RATE    11500000E3
#define UTIL_GLOBAL_ILKN_12G5_RATE    12500000E3

/* SONET/SDH */
#define UTIL_GLOBAL_STS_192_STM_64_RATE     (DOUBLE) 8 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_STS_768_STM_256_RATE    (DOUBLE) 32 * UTIL_GLOBAL_ODU0_RATE

/* Fiber Channel */
#define UTIL_GLOBAL_FC_800_RATE            8500E6
#define UTIL_GLOBAL_FC_1200_RATE           10518750E3
#define UTIL_GLOBAL_FC_1200_TRANS_RATE     10312500E3

/* CBR */
#define UTIL_GLOBAL_GDPS_5G_RATE             5E9
#define UTIL_GLOBAL_GDPS_10G_RATE            10E9
#define UTIL_GLOBAL_CPRI_5_RATE              4915200E3
#define UTIL_GLOBAL_CPRI_6_RATE              6144000E3
#define UTIL_GLOBAL_CPRI_7_RATE              9830400E3
#define UTIL_GLOBAL_CBR_40_G_RATE            40E9 

/* CBR & FIBER CHANNEL odu flex Server */
#define UTIL_GLOBAL_ODU_FLEX_FC_800_RATE         (DOUBLE)239/238 * UTIL_GLOBAL_FC_800_RATE            
#define UTIL_GLOBAL_ODU_FLEX_FC_1200_RATE        (DOUBLE)239/238 * UTIL_GLOBAL_FC_1200_RATE           
#define UTIL_GLOBAL_ODU_FLEX_FC_1200_TRANS_RATE  (DOUBLE)239/238 * UTIL_GLOBAL_FC_1200_TRANS_RATE     
#define UTIL_GLOBAL_ODU_FLEX_GDPS_5G_RATE        (DOUBLE)239/238 * UTIL_GLOBAL_GDPS_5G_RATE           
#define UTIL_GLOBAL_ODU_FLEX_GDPS_10G_RATE       (DOUBLE)239/238 * UTIL_GLOBAL_GDPS_10G_RATE          
#define UTIL_GLOBAL_ODU_FLEX_CPRI_5_RATE         (DOUBLE)239/238 * UTIL_GLOBAL_CPRI_5_RATE            
#define UTIL_GLOBAL_ODU_FLEX_CPRI_6_RATE         (DOUBLE)239/238 * UTIL_GLOBAL_CPRI_6_RATE            
#define UTIL_GLOBAL_ODU_FLEX_CPRI_7_RATE         (DOUBLE)239/238 * UTIL_GLOBAL_CPRI_7_RATE            
#define UTIL_GLOBAL_ODU_FLEX_CBR_40_G_RATE       (DOUBLE)239/238 * UTIL_GLOBAL_CBR_40_G_RATE          


/* OTN */
#define UTIL_GLOBAL_ODU0_RATE     1244160E3

#define UTIL_GLOBAL_ODU1_RATE     (DOUBLE)239/238 * 2 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_ODU2_RATE     (DOUBLE)239/237 * 8 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_ODU3_RATE     (DOUBLE)239/236 * 32 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_ODU4_RATE     (DOUBLE)239/227 * 80 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_ODU1E_RATE    (DOUBLE)239/238 * UTIL_GLOBAL_ENET_10GE_RATE
#define UTIL_GLOBAL_ODU2E_RATE    (DOUBLE)239/237 * UTIL_GLOBAL_ENET_10GE_RATE
#define UTIL_GLOBAL_ODU3E1_RATE   (DOUBLE)239/236 * 4 * UTIL_GLOBAL_ENET_10GE_RATE
#define UTIL_GLOBAL_ODU3E2_RATE   (DOUBLE)239/255 * (DOUBLE)243/217 * 32 * UTIL_GLOBAL_ODU0_RATE

#define UTIL_GLOBAL_ODU1F_RATE    (DOUBLE)239/238 * UTIL_GLOBAL_FC_1200_RATE
#define UTIL_GLOBAL_ODU2F_RATE    (DOUBLE)239/237 * UTIL_GLOBAL_FC_1200_RATE

#define UTIL_GLOBAL_OPU0_RATE     (DOUBLE)238/239 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OPU1_RATE     (DOUBLE)2 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OPU2_RATE     (DOUBLE)238/237 * 8 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OPU3_RATE     (DOUBLE)238/236 * 32 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OPU4_RATE     (DOUBLE)238/227 * 80 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OPU2E_RATE    (DOUBLE)238/237 * UTIL_GLOBAL_ENET_10GE_RATE
#define UTIL_GLOBAL_OPU1E_RATE    (DOUBLE)238/237 * UTIL_GLOBAL_ENET_10GE_RATE /* VD: need correction*/
#define UTIL_GLOBAL_OPU1F_RATE    (DOUBLE)238/239 * UTIL_GLOBAL_ODU1F_RATE
#define UTIL_GLOBAL_OPU2F_RATE    (DOUBLE)238/239 * UTIL_GLOBAL_ODU2F_RATE
#define UTIL_GLOBAL_OPU3E1_RATE   (DOUBLE)238/239 * UTIL_GLOBAL_ODU3E1_RATE
#define UTIL_GLOBAL_OPU3E2_RATE   (DOUBLE)238/255 * (DOUBLE)243/217 * 32 * UTIL_GLOBAL_ODU0_RATE

#define UTIL_GLOBAL_ODUFLEX_CBR_BASE_RATE   (DOUBLE)239/238

/* ODUflex GFP bit rate */
#ifdef MORE_PRECISE_ODUFLEX_RATE
#define UTIL_GLOBAL_ODU2_TS_RATE    1249177230.064E0
#define UTIL_GLOBAL_ODU3_TS_RATE    1254470353.920E0
#define UTIL_GLOBAL_ODU4_TS_RATE    1301467133.181E0
#else
#define UTIL_GLOBAL_ODU2_TS_RATE    1249177230E0
#define UTIL_GLOBAL_ODU3_TS_RATE    1254470354E0
#define UTIL_GLOBAL_ODU4_TS_RATE    1301467133E0
#endif

#define UTIL_GLOBAL_OTU1_RATE    (DOUBLE)255/238 * 2 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OTU2_RATE    (DOUBLE)255/237 * 8 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OTU3_RATE    (DOUBLE)255/236 * 32 * UTIL_GLOBAL_ODU0_RATE
#define UTIL_GLOBAL_OTU4_RATE    (DOUBLE)255/227 * 80 * UTIL_GLOBAL_ODU0_RATE

#define UTIL_GLOBAL_OTU1E_RATE   (DOUBLE)255/238 * 10312500E3
#define UTIL_GLOBAL_OTU1F_RATE   (DOUBLE)255/238 * 10518750E3
#define UTIL_GLOBAL_OTU2E_RATE   (DOUBLE)255/237 * 10312500E3
#define UTIL_GLOBAL_OTU2F_RATE   (DOUBLE)255/237 * 10518750E3
#define UTIL_GLOBAL_OTU3E1_RATE  (DOUBLE)255/236 * 4 * 10312500E3
#define UTIL_GLOBAL_OTU3E2_RATE  (DOUBLE)243/217 * 16 * 2 * UTIL_GLOBAL_ODU0_RATE


/* ODTU payload bandwidth (bit/s) */
#define UTIL_GLOBAL_ODTU01_MIN_BW           1244216796E0
#define UTIL_GLOBAL_ODTU12_MIN_BW           2498933311E0
#define UTIL_GLOBAL_ODTU13_MIN_BW           2509522012E0
#define UTIL_GLOBAL_ODTU23_MIN_BW           10038088048E0
/* Below is the base rate. The actual bandwidth varies depending on the number
** of tributary slot
*/
#define UTIL_GLOBAL_ODTU2_TS_MIN_BASE_BW    1249384632E0
#define UTIL_GLOBAL_ODTU3_TS_MIN_BASE_BW    1254678635E0
#define UTIL_GLOBAL_ODTU4_TS_MIN_BASE_BW    1301683217E0
#define UTIL_GLOBAL_ODTU3E1_TS_MIN_BASE_BW  UTIL_GLOBAL_ODTU3E1_TS_BASE_BW * (1 - (DOUBLE)20/1000000)
#define UTIL_GLOBAL_ODTU3E2_TS_MIN_BASE_BW  UTIL_GLOBAL_ODTU3E2_TS_BASE_BW * (1 - (DOUBLE)20/1000000)

/* ODTU payload bandwidth (bit/s) */
#define UTIL_GLOBAL_ODTU01_BW           1244241681E0
#define UTIL_GLOBAL_ODTU12_BW           2498983291E0
#define UTIL_GLOBAL_ODTU13_BW           2509572203E0
#define UTIL_GLOBAL_ODTU23_BW           10038288814E0

/* Below is the base rate. The actual bandwidth varies depending on the number
** of tributary slot
*/
#define UTIL_GLOBAL_ODTU2_TS_BASE_BW    1249409620E0
#define UTIL_GLOBAL_ODTU3_TS_BASE_BW    1254703729E0
#define UTIL_GLOBAL_ODTU4_TS_BASE_BW    1301709251E0
#define UTIL_GLOBAL_ODTU3E1_TS_BASE_BW  (DOUBLE)119/3824 * UTIL_GLOBAL_ODU3E1_RATE 
#define UTIL_GLOBAL_ODTU3E2_TS_BASE_BW  (DOUBLE)119/3824 * UTIL_GLOBAL_ODU3E2_RATE 


#define UTIL_GLOBAL_ODTU01_MAX_BW           1244266566E0
#define UTIL_GLOBAL_ODTU12_MAX_BW           2499033271E0
#define UTIL_GLOBAL_ODTU13_MAX_BW           2509622395E0
#define UTIL_GLOBAL_ODTU23_MAX_BW           10038489579E0
/* Below is the base rate. The actual bandwidth varies depending on the number
** of tributary slot
*/
#define UTIL_GLOBAL_ODTU2_TS_MAX_BASE_BW    1249434608E0
#define UTIL_GLOBAL_ODTU3_TS_MAX_BASE_BW    1254728823E0
#define UTIL_GLOBAL_ODTU4_TS_MAX_BASE_BW    1301735285E0
#define UTIL_GLOBAL_ODTU3E1_TS_MAX_BASE_BW  UTIL_GLOBAL_ODTU3E1_TS_BASE_BW * (1 + (DOUBLE)20/1000000)
#define UTIL_GLOBAL_ODTU3E2_TS_MAX_BASE_BW  UTIL_GLOBAL_ODTU3E2_TS_BASE_BW * (1 + (DOUBLE)20/1000000)

/* OPU container payload capacity */
#define UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES  15232
#define UTIL_GLOBAL_OPU4_MAX_PAYLOAD_BYTES  15200

/* Bits per server frame or multiframe */
#define UTIL_GLOBAL_OPU0_BRATE              8 * UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES
#define UTIL_GLOBAL_OPU1_BRATE              UTIL_GLOBAL_OPU0_BRATE
#define UTIL_GLOBAL_OPU2_BRATE              UTIL_GLOBAL_OPU0_BRATE
#define UTIL_GLOBAL_OPU3_BRATE              UTIL_GLOBAL_OPU0_BRATE
#define UTIL_GLOBAL_OPU4_BRATE              8 * UTIL_GLOBAL_OPU4_MAX_PAYLOAD_BYTES
/* 
** ODTUk.ts and ODTUjk payload bytes
*/
#define UTIL_GLOBAL_ODTU2_TS_BASE_BRATE     UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES
#define UTIL_GLOBAL_ODTU3_TS_BASE_BRATE     UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES
#define UTIL_GLOBAL_ODTU4_TS_BASE_BRATE     UTIL_GLOBAL_OPU4_MAX_PAYLOAD_BYTES

#define UTIL_GLOBAL_ODTU12_2G5_PAYLOAD_BYTES      UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES
#define UTIL_GLOBAL_ODTU13_2G5_PAYLOAD_BYTES      UTIL_GLOBAL_OPUK_MAX_PAYLOAD_BYTES
#define UTIL_GLOBAL_ODTU23_2G5_PAYLOAD_BYTES      UTIL_GLOBAL_ODTU13_2G5_PAYLOAD_BYTES * 4

#define UTIL_GLOBAL_ODTU01_1G25_PAYLOAD_BYTES     UTIL_GLOBAL_ODTU12_2G5_PAYLOAD_BYTES 
#define UTIL_GLOBAL_ODTU12_1G25_PAYLOAD_BYTES     UTIL_GLOBAL_ODTU12_2G5_PAYLOAD_BYTES * 2
#define UTIL_GLOBAL_ODTU13_1G25_PAYLOAD_BYTES     UTIL_GLOBAL_ODTU13_2G5_PAYLOAD_BYTES * 2
#define UTIL_GLOBAL_ODTU23_1G25_PAYLOAD_BYTES     UTIL_GLOBAL_ODTU23_2G5_PAYLOAD_BYTES * 2

/* ODU bit-rate tolerance PPM offsets */
#define UTIL_GLOBAL_OPU_100_PPM_OFF           100
#define UTIL_GLOBAL_OPU_20_PPM_OFF             20
#define UTIL_GLOBAL_CPRI_PPM_OFF    (DOUBLE)0.002

#define UTIL_GLOBAL_MAX_NUM_TS          80
#define UTIL_GLOBAL_MAX_NUM_ODU_CHNL    96
#define UTIL_GLOBAL_MAX_NUM_CAL_ENTRY   96 

/* COREOTN DMX dummy schedule id for channelized MO ODU3 */
#define UTIL_GLOBAL_MAX_NUM_DUMMY_SCHD      16
#define UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD      2    
#define UTIL_GLOBAL_DMX_DUMMY_SCHD_ID_1     5
#define UTIL_GLOBAL_DMX_DUMMY_SCHD_ID_2     11 
#define UTIL_GLOBAL_DMX_DUMMY_SCHD_ARRAY { \
    {5, 11, 17, 23, 29, 35, 41, 47, 53, 59, 65, 71, 77, 83, 89, 95} \
}

#define UTIL_GLOBAL_REF_FREQ                   (DOUBLE)311040000.0

#define UTIL_GLOBAL_CBRC_MAX_SLOTS        (12)

/* The following is used as a sanity check when de-referencing our
* opaque port handles to give us some faith that proceeding data is
* of the correct type. */
#define UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID 0xCAFECAFE

/* Number of bytes in MAC address */
#define UTIL_GLOBAL_MAC_NUM_BYTE  6

/* Maximum number of CPB port defined in switch connection table (util_global_cpb_connect_t)*/
#define UTIL_GLOBAL_CPB_CONNECT_MAX_NUM_PORTS             6

/* Maximum number of CPB channel per port defined in switch connection table (util_global_cpb_connect_t)*/
#define UTIL_GLOBAL_CPB_CONNECT_MAX_NUM_CHNLS_PER_PORT    150

/* Maximum number of ODUKSW port defined in switch connection table (util_global_cpb_connect_t)*/
#define UTIL_GLOBAL_ODUKSW_CONNECT_MAX_NUM_PORTS             4

/* Maximum number of ODUKSW channel per port defined in switch connection table (util_global_cpb_connect_t)*/
#define UTIL_GLOBAL_ODUKSW_CONNECT_MAX_NUM_CHNLS_PER_PORT    150

#define UTIL_GLOBAL_DDEG_HW_M_INTERVAL_MIN 1
#define UTIL_GLOBAL_DDEG_FW_M_INTERVAL_MIN 2
#define UTIL_GLOBAL_DDEG_FW_M_INTERVAL_MAX 10
#define UTIL_GLOBAL_DDEG_HW_M_INTERVAL_MAX 15

/* Number of serdes in BP interface */
#define UTIL_GLOBAL_MAX_T8_E1_IN_BP_LR     40

/*G.HAO*/

/*the maximum number of G.HAO segments that the software supports*/
#define MAX_NUM_GHAO_SEGMENTS 10
/* hardware only support 1 source node (2 firmware segments).
5440 intermediate nodes only require 1 firmware segment*/
#define MAX_NUM_FIRMWARE_SEGMENTS (MAX_NUM_GHAO_SEGMENTS+1)

#define UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX     150
#define UTIL_GLOBAL_ILKN_ODUK_DPI_CHNLS_MAX    104
#define UTIL_GLOBAL_ILKN_ARB_DPI_CHANS_MAX         (UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX + UTIL_GLOBAL_ILKN_ODUK_DPI_CHNLS_MAX)



/*
** Macro Definitions
*/

#define OFFSETOF(structure, field) (structure->field

/* Define to enable/disable/clear all interrupts  */
#define UTIL_GLOBAL_ALL_INTS            NULL
#define UTIL_GLOBAL_INT_OPTIMIZE        TRUE
#define UTIL_GLOBAL_INT_NO_OPTIMIZE     FALSE 

/* Read/write buffer */
#define UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(handle, tsb_name, optimize) \
    tsb_name##_buffer_t    *tsb_name##_buffer_ptr = NULL;                       \
    tsb_name##_buffer_t     tsb_name##_buffer[1];                               \
    tsb_name##_buffer_init(tsb_name##_buffer, handle);                          \
    if (optimize) {                                                             \
        tsb_name##_buffer_ptr = tsb_name##_buffer;                              \
    }                                                                           \

#define UTIL_GLOBAL_INT_BUFFER_INIT(handle, tsb_name, optimize) \
    tsb_name##_buffer_t    *tsb_name##_buffer_ptr = NULL;                       \
    tsb_name##_buffer_t     tsb_name##_buffer[1];                               \
    tsb_name##_buffer_init(tsb_name##_buffer, handle, link);                    \
    if (optimize) {                                                             \
        tsb_name##_buffer_ptr = tsb_name##_buffer;                              \
    }                                                                           \

#define UTIL_GLOBAL_INT_BUFFER_FLUSH(handle, tsb_name) \
    if (NULL != tsb_name##_buffer_ptr) {                                        \
        tsb_name##_buffer_flush(tsb_name##_buffer_ptr);                         \
    }                                                                           \


#define UTIL_GLOBAL_INT_SAFE_GET_OFFSET(table_ptr, field) \
    ((NULL == table_ptr) ? NULL : &table_ptr->field)


/* Interrupt Accessors */

#define UTIL_GLOBAL_INT_ENABLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_E_set(NULL, handle, link, enable);       \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_CLEAR(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_I_set_to_clear(NULL, handle, link, TRUE);\
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_RETRIEVE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (0 != filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_get(tsb_name##_buffer_ptr, handle, link); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        *int_found_ptr = tsb_name##_field_##tsb_field##_I_get(NULL, handle, link); \
    }                                                                           \
} while(0)




#define UTIL_GLOBAL_INT_ENABLE_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_##field_type##_##tsb_field##_E_set(NULL, handle, link1, link2, enable); \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CLEAR_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_##field_type##_##tsb_field##_I_set_to_clear(NULL, handle, link1, link2, TRUE); \
    }                                                                       \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_##field_type##_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link1, link2); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_RETRIEVE_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_##field_type##_##tsb_field##_get(tsb_name##_buffer_ptr, handle, link1, link2); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        *int_found_ptr = tsb_name##_##field_type##_##tsb_field##_I_get(NULL, handle, link1, link2); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ENABLE_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_E_set(NULL, handle, enable);             \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CLEAR_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_I_set_to_clear(NULL, handle, TRUE);      \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle); \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_STATUS_RETRIEVE_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_get(tsb_name##_buffer_ptr, handle); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        *int_found_ptr = tsb_name##_field_##tsb_field##_I_get(NULL, handle);    \
    }                                                                           \
} while(0)

/* Array interrupt accessors */
#define UTIL_GLOBAL_INT_ENABLE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_E_set(NULL, handle, link, i, enable); \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CLEAR_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_I_set_to_clear(NULL, handle, link, i, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_RETRIEVE_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##_get(tsb_name##_buffer_ptr, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK_ARRAY(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; (i < array_len) && (FALSE == *int_found_ptr); i++) {            \
        if (TRUE == int_en_table_ptr->struct_field[i]) {                        \
            *int_found_ptr = tsb_name##_field_##tsb_field##_I_get(NULL, handle, link, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ENABLE_ARRAY_RANGE(handle, tsb_name, field_type, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_##field_type##_##tsb_field##_E_set(NULL, handle, i, link1, link2, enable); \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CLEAR_ARRAY_RANGE(handle, tsb_name, field_type, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_##field_type##_##tsb_field##_I_set_to_clear(NULL, handle, i, link1, link2, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE_ARRAY_RANGE(handle, tsb_name, field_type, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_##field_type##_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, i, link1, link2); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_RETRIEVE_ARRAY_RANGE(handle, tsb_name, field_type, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_##field_type##_##tsb_field##_get(tsb_name##_buffer_ptr, handle, i, link1, link2); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK_ARRAY_RANGE(handle, tsb_name, field_type, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; (i < array_len) && (FALSE == *int_found_ptr); i++) {            \
        if (TRUE == int_en_table_ptr->struct_field[i]) {                        \
            *int_found_ptr = tsb_name##_##field_type##_##tsb_field##_I_get(NULL, handle, i, link1, link2); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ENABLE_ARRAY_SINGLE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_E_set(NULL, handle, i, enable);      \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_LFIELD_RETRIEVE_SINGLE(handle, tsb_name, tsb_field, struct_field) \
    do {                                                                \
        if ((NULL == filt_table_ptr) ||                                 \
            (TRUE == filt_table_ptr->struct_field)) {                   \
            int_table_ptr->struct_field = tsb_name##_lfield_range_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, chnl_id, chnl_id); \
        }                                                               \
    } while(0)

#define UTIL_GLOBAL_STATUS_LFIELD_RETRIEVE_SINGLE(handle, tsb_name, tsb_field, struct_field) \
    do {                                                                \
        if ((NULL == filt_table_ptr) ||                                 \
            (TRUE == filt_table_ptr->struct_field)) {                   \
            int_table_ptr->struct_field = tsb_name##_lfield_range_##tsb_field##_get(tsb_name##_buffer_ptr, handle, chnl_id, chnl_id); \
        }                                                               \
    } while(0)


#define UTIL_GLOBAL_INT_LFIELD_ENABLE_SINGLE(handle, tsb_name, tsb_field, struct_field) \
    do {                                                                \
        if ((NULL == int_table_ptr) ||                                              \
            (TRUE == int_table_ptr->struct_field)) {                                \
            tsb_name##_lfield_range_##tsb_field##_E_set(NULL, handle, chnl_id, chnl_id, enable); \
            int_en_table_ptr->struct_field = enable;                             \
        }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_LFIELD_CLEAR_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_lfield_range_##tsb_field##_I_set_to_clear(NULL, handle, chnl_id, chnl_id, TRUE); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_LFIELD_CHECK_SINGLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if (NULL != int_en_table_ptr && TRUE == int_en_table_ptr->struct_field) {                    \
        *int_found_ptr = tsb_name##_lfield_range_##tsb_field##_I_get(NULL, handle, chnl_id, chnl_id); \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_CLEAR_ARRAY_SINGLE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            tsb_name##_field_##tsb_field##_I_set_to_clear(NULL, handle, i, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE_ARRAY_SINGLE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_RETRIEVE_ARRAY_SINGLE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field##_get(tsb_name##_buffer_ptr, handle, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK_ARRAY_SINGLE(handle, tsb_name, array_len, tsb_field, struct_field) \
do {                                                                            \
    for (i = 0; (i < array_len) && (FALSE == *int_found_ptr); i++) {            \
        if (TRUE == int_en_table_ptr->struct_field[i]) {                        \
            *int_found_ptr = tsb_name##_field_##tsb_field##_I_get(NULL, handle, i); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ENABLE_FIELD12(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        switch(link)                                                            \
        {                                                                       \
            case 0:  tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 1:  tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 2:  tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 3:  tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 4:  tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 5:  tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 6:  tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 7:  tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 8:  tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 9:  tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_E_set(NULL, handle, enable); break;  \
            case 10: tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_E_set(NULL, handle, enable); break; \
            case 11: tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_E_set(NULL, handle, enable); break; \
            default: break;                                                     \
        }                                                                       \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_CLEAR_FIELD12(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        switch(link)                                                            \
        {                                                                       \
            case 0:  tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 1:  tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 2:  tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 3:  tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 4:  tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 5:  tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 6:  tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 7:  tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 8:  tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 9:  tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break;  \
            case 10: tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break; \
            case 11: tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_I_set_to_clear(NULL, handle, TRUE); break; \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_RETRIEVE_FIELD12(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        switch(link)                                                            \
        {                                                                       \
            case 0:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 1:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 2:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 3:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 4:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 5:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 6:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 7:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 8:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 9:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break;  \
            case 10: int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break; \
            case 11: int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle); break; \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_RETRIEVE_FIELD12(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        switch(link)                                                            \
        {                                                                       \
            case 0:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 1:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 2:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 3:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 4:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 5:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 6:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 7:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 8:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 9:  int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break;  \
            case 10: int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break; \
            case 11: int_table_ptr->struct_field = tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle); break; \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_CHECK_FIELD12(handle, tsb_name, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        switch(link)                                                            \
        {                                                                       \
            case 0:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##0_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 1:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 2:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 3:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 4:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 5:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 6:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 7:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 8:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 9:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_I_get(NULL, handle); break;  \
            case 10: *int_found_ptr = tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_I_get(NULL, handle); break; \
            case 11: *int_found_ptr = tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_I_get(NULL, handle); break; \
            default: break;                                                     \
        }                                                                       \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_FIELD12_ARRAY_ENABLE(handle, tsb_name, array_len, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            switch(link)                                                        \
            {                                                                   \
                case 0:  tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 1:  tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 2:  tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 3:  tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 4:  tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 5:  tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 6:  tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 7:  tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 8:  tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 9:  tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_E_set(NULL, handle, i, enable); break;  \
                case 10: tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_E_set(NULL, handle, i, enable); break; \
                case 11: tsb_name##_field_##tsb_field_pre##12_##tsb_field_post##_E_set(NULL, handle, i, enable); break; \
                default: break;                                                 \
            }                                                                   \
            int_en_table_ptr->struct_field[i] = enable;                         \
        }                                                                       \
    }                                                                           \
} while(0)


#define UTIL_GLOBAL_INT_FIELD12_ARRAY_CLEAR(handle, tsb_name, array_len, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == int_table_ptr) ||                                          \
            (TRUE == int_table_ptr->struct_field[i])) {                         \
            switch(link)                                                        \
            {                                                                   \
                case 0:  tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 1:  tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 2:  tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 3:  tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 4:  tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 5:  tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 6:  tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 7:  tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 8:  tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 9:  tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break;  \
                case 10: tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break; \
                case 11: tsb_name##_field_##tsb_field_pre##12_##tsb_field_post##_I_set_to_clear(NULL, handle, i, TRUE); break; \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_FIELD12_ARRAY_RETRIEVE(handle, tsb_name, array_len, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            switch(link)                                                        \
            {                                                                   \
                case 0:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 1:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 2:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 3:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 4:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 5:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 6:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 7:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 8:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 9:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 10: int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break; \
                case 11: int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##12_##tsb_field_post##_I_get(tsb_name##_buffer_ptr, handle, i); break; \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_FIELD12_ARRAY_RETRIEVE(handle, tsb_name, array_len, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((NULL == filt_table_ptr) ||                                         \
            (TRUE == filt_table_ptr->struct_field[i])) {                        \
            switch(link)                                                        \
            {                                                                   \
                case 0:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 1:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 2:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 3:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 4:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 5:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 6:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 7:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 8:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 9:  int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break;  \
                case 10: int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break; \
                case 11: int_table_ptr->struct_field[i] = tsb_name##_field_##tsb_field_pre##12_##tsb_field_post##_get(tsb_name##_buffer_ptr, handle, i); break; \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_FIELD12_ARRAY_CHECK(handle, tsb_name, array_len, tsb_field_pre, tsb_field_post, struct_field) \
do {                                                                            \
    for (i = 0; i < array_len; i++) {                                           \
        if ((FALSE == *int_found_ptr) &&                                        \
            (TRUE == int_en_table_ptr->struct_field[i])) {                      \
            switch(link)                                                        \
            {                                                                   \
                case 0:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##1_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 1:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##2_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 2:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##3_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 3:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##4_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 4:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##5_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 5:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##6_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 6:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##7_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 7:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##8_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 8:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##9_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 9:  *int_found_ptr = tsb_name##_field_##tsb_field_pre##10_##tsb_field_post##_I_get(NULL, handle, i); break;  \
                case 10: *int_found_ptr = tsb_name##_field_##tsb_field_pre##11_##tsb_field_post##_I_get(NULL, handle, i); break; \
                case 11: *int_found_ptr = tsb_name##_field_##tsb_field_pre##12_##tsb_field_post##_I_get(NULL, handle, i); break; \
                default: break;                                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

/* SDH interrupt accessors */
#define UTIL_GLOBAL_INT_SDH_PMG_CLEAR(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_I_set(NULL, handle, link, TRUE);         \
    }                                                                           \
} while(0)




    
#define UTIL_GLOBAL_INT_ODU_RFRM_RETRIEVE(handle, tsb_name, tsb_field, tsb_comb_field, struct_field)       \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, chnl_id); \
    }                                                                           \
} while(0)
    
#define UTIL_GLOBAL_STATUS_ODU_RFRM_RETRIEVE(handle, tsb_name, tsb_field, tsb_comb_field, struct_field)       \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_V_get(tsb_name##_buffer_ptr, handle, chnl_id); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ODU_RFRM_PID_RETRIEVE(handle, tsb_name, tsb_field, tsb_comb_field, struct_field)                      \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr[i].struct_field = tsb_name##_field_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, pool_id); \
    }                                                                           \
} while(0)
            
#define UTIL_GLOBAL_STATUS_ODU_RFRM_PID_RETRIEVE(handle, tsb_name, tsb_field, tsb_comb_field, struct_field)                   \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr[i].struct_field = tsb_name##_field_##tsb_field##_V_get(tsb_name##_buffer_ptr, handle, pool_id); \
    }                                                                           \
} while(0)



/* PMON Interrupt Accessors */
#define UTIL_GLOBAL_INT_PMON_ENABLE(pmon_type, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        /* Store enabled interrupt internally */                                \
        int_en_table_ptr->struct_field = enable;                                \
                                                                                \
        /* Enable PMON interrupt */                                             \
        digi_pmon_ipi_int_enable(digi_handle, DIGI_PMON_IPI_TYPE_PMON_##pmon_type, enable); \
    }                                                                           \
} while(0)
    
    
    
#define UTIL_GLOBAL_INT_PMON_CLEAR(pmon_type, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        /* Clear PMON interrupt */                                              \
        digi_pmon_ipi_int_clear(digi_handle, DIGI_PMON_IPI_TYPE_PMON_##pmon_type); \
        /* Mark IPI bit as SW DONE so FW can have access */                     \
        digi_pmon_ipi_bit_set(digi_handle, DIGI_PMON_IPI_TYPE_PMON_##pmon_type, DIGI_PMON_IPI_BITS_SW_DONE); \
    }                                                                           \
} while(0)
    
    
#define UTIL_GLOBAL_INT_PMON_RETRIEVE(pmon_type, struct_field) \
do {                                                                            \
    /* Retrieve PMON interrupt via IPI */                                       \
    int_table_ptr->struct_field = digi_pmon_ipi_int_retrieve(digi_handle, DIGI_PMON_IPI_TYPE_PMON_##pmon_type); \
} while(0)
        
        
#define UTIL_GLOBAL_INT_PMON_CHECK(pmon_type, struct_field) \
do {                                                                            \
    if (TRUE == int_en_table_ptr->struct_field) {                               \
        /* Retrieve status of enabled interrupt */                              \
        struct_field##_int_found = digi_pmon_ipi_int_retrieve(digi_handle, DIGI_PMON_IPI_TYPE_PMON_##pmon_type); \
        int_found |= struct_field##_int_found;                                  \
    }                                                                           \
} while(0)

/* SIFD interrupt accessors */
#define UTIL_GLOBAL_INT_SIFD_ENABLE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_E0_set(NULL, handle, link1, enable);     \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)

/*
 * COPI TSB registers are 1 by default meaning the interrupt can only be
 * processed by polling.  If 0 then will be processed via CORE_TX_INT.
 * As a result the value set in the register is negated so 1 is 0 and
 * vice versa.  That is enabled value is 0, and disable is 1.
 */    
#define UTIL_GLOBAL_INT_SIFD_RANGE_ENABLE(handle, tsb_name, field_type, tsb_field, struct_field)   \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_##field_type##_##tsb_field##_M_set(NULL, handle, link1, link2, !enable); \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_SIFD_CLEAR(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_field_##tsb_field##_I0_set_to_clear(NULL, handle, link1, TRUE); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_SIFD_RANGE_CLEAR(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                        \
    if ((NULL == int_table_ptr) ||                                              \
        (TRUE == int_table_ptr->struct_field)) {                                \
        tsb_name##_##field_type##_##tsb_field##_I_set(NULL, handle, link1, link2, TRUE); \
    }                                                                       \
} while(0)
    
#define UTIL_GLOBAL_INT_SIFD_RETRIEVE(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        int_table_ptr->struct_field = tsb_name##_field_##tsb_field##_I0_get(tsb_name##_buffer_ptr, handle, link1); \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_SIFD_CHECK(handle, tsb_name, tsb_field, struct_field) \
do {                                                                            \
    if ((FALSE == *int_found_ptr) &&                                            \
        (TRUE == int_en_table_ptr->struct_field)) {                             \
        *int_found_ptr = tsb_name##_field_##tsb_field##_I0_get(NULL, handle, link1); \
    }                                                                           \
} while(0)

/* ODTU DMX interrupt accessors */
#define UTIL_GLOBAL_INT_ODTU_DMX_ENABLE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    UINT32  calendar_id, i;                                                     \
    if (NULL == int_table_ptr) {                                                \
        for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {                              \
            tsb_name##_##field_type##_##tsb_field##_E_set(tsb_name##_buffer_ptr, handle, i, i, enable); \
        }                                                                       \
        int_en_table_ptr->struct_field = enable;                                \
    } else if (int_table_ptr->struct_field) {                                   \
        for (i = 0; i < chnl_info->num_cal_entries; i++) {                      \
            calendar_id = chnl_info->calendar_id[i];                            \
            tsb_name##_##field_type##_##tsb_field##_E_set(tsb_name##_buffer_ptr, handle, calendar_id, calendar_id, enable); \
        }                                                                       \
        int_en_table_ptr->struct_field = enable;                                \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ODTU_DMX_CLEAR(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    UINT32  calendar_id, i;                                                     \
    if (NULL == int_table_ptr) {                                                \
        for (i = 0; i < ODTU_DMX_NUM_CHANL; i++) {                              \
            tsb_name##_##field_type##_##tsb_field##_I_set_to_clear(tsb_name##_buffer_ptr, handle, i, i, TRUE); \
        }                                                                       \
    } else if (int_table_ptr->struct_field) {                                   \
        for (i = 0; i < chnl_info->num_cal_entries; i++) {                      \
            calendar_id = chnl_info->calendar_id[i];                            \
            tsb_name##_##field_type##_##tsb_field##_I_set_to_clear(tsb_name##_buffer_ptr, handle, calendar_id, calendar_id, TRUE); \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ODTU_DMX_RETRIEVE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    UINT32  calendar_id, reg_val, i;                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        for (i = 0; i < chnl_info->num_cal_entries; i++) {                      \
            calendar_id = chnl_info->calendar_id[i];                            \
            reg_val = tsb_name##_##field_type##_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, calendar_id, calendar_id); \
            if (TRUE == reg_val) {                                              \
                int_table_ptr->struct_field = reg_val;                          \
                break;                                                          \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_STATUS_ODTU_DMX_RETRIEVE(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    UINT32  calendar_id, reg_val, i;                                            \
    if ((NULL == filt_table_ptr) ||                                             \
        (TRUE == filt_table_ptr->struct_field)) {                               \
        for (i = 0; i < chnl_info->num_cal_entries; i++) {                      \
            calendar_id = chnl_info->calendar_id[i];                            \
            reg_val = tsb_name##_##field_type##_##tsb_field##_get(tsb_name##_buffer_ptr, handle, calendar_id, calendar_id); \
            if (TRUE == reg_val) {                                              \
                int_table_ptr->struct_field = reg_val;                          \
                break;                                                          \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

#define UTIL_GLOBAL_INT_ODTU_DMX_CHECK(handle, tsb_name, field_type, tsb_field, struct_field) \
do {                                                                            \
    UINT32  calendar_id, reg_val, i;                                            \
    if (int_en_table_ptr->struct_field) {                                       \
        for (i = 0; (i < chnl_info->num_cal_entries) && (FALSE == *int_found_ptr); i++) { \
            calendar_id = chnl_info->calendar_id[i];                            \
            reg_val = tsb_name##_##field_type##_##tsb_field##_I_get(tsb_name##_buffer_ptr, handle, calendar_id, calendar_id); \
            if (TRUE == reg_val) {                                              \
                *int_found_ptr = TRUE;                                          \
            }                                                                   \
        }                                                                       \
    }                                                                           \
} while(0)

/* Interrupt No-Ops */
#define UTIL_GLOBAL_INT_NOOP(handle, tsb_name, unused, struct_field)
#define UTIL_GLOBAL_INT_NOOP_RANGE(handle, tsb_name, unused1, unused2, struct_field)

/* BOOL type conversion */
#define UTIL_GLOBAL_BOOL8_TO_BOOL_CONVERT(bool8_val) \
    (TRUE == bool8_val ? TRUE : FALSE)
#define UTIL_GLOBAL_BOOL_TO_BOOL8_CONVERT(bool_val) \
    (TRUE == bool_val ? TRUE : FALSE)

/* ACB FT contexts indices */
#define UTIL_GLOBAL_ACB_FT_LPBK_CTXT_IDX 0
#define UTIL_GLOBAL_ACB_FT_FORCE_NOMINAL_CTXT_IDX 1
#define UTIL_GLOBAL_ACB_FT_DEFAULT_CTXT_IDX UTIL_GLOBAL_ACB_FT_LPBK_CTXT_IDX


/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: util_global_odu_lo_chnl_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for LO ODUk channel configuration.
*
* ELEMENTS:
*   state           - state of the LO channel
*                     1 - enabled
*                     0 - disabled
*   lo_oduk_type    - ODUk type of LO channel
*   ho_oduk_type    - ODUk type of HO channel that this LO channel is associated
*                     with.
*   num_ts          - number of tributary slots used in this LO channel
*   payload_type    - Enum for payload format of the ODUk
*   ho_chnl_id      - HO channel ID that this LO ODU channel belongs to
*   mapping_mode    - Mapping mode of the LO channel
*   ts_port         - tributary port number that the tributary slot will be
*                     transported.
*
*******************************************************************************/
typedef struct util_global_odu_lo_chnl_cfg_t
{
    util_global_chnl_state_t state;
    util_global_odukp_type_t lo_oduk_type;
    util_global_odukp_type_t ho_oduk_type;
    UINT8 num_ts;
    util_global_odu_line_payload_t ts_type;
    /*util_global_opu_payload_type_t payload_type;*/
    UINT8 ho_chnl_id;
    util_global_mapping_mode_t mapping_mode;
    UINT8 ts_port;
} util_global_odu_lo_chnl_cfg_t;

/*******************************************************************************
* STRUCTURE: util_global_odu_ho_chnl_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for HO ODUk channel configuration.
*
* ELEMENTS:
*   state                   - state of HO ODUk channel
*   oduk_type               - ODUk type of HO channel
*   ts_type                 - Tribslot type. See odu_line_payload_t.
*   cal_array               - an array of calendar entries consumed by this HO
*   num_tribslot            - Number of tribslots this HO Channel contains
*   num_cal_entries         - Number of calendar entries consumed by this HO
*   tribslot_added          - an array to describe if tribslot has been added
*   tribslot_occupied       - an array of boolena to indicate location of
*                             tributary slots relative to the HO channel.
*   num_active_ts           - number of active tributary slots in the HO channel
*******************************************************************************/
typedef struct util_global_odu_ho_chnl_cfg_t
{
    util_global_chnl_state_t state;
    util_global_odukp_type_t oduk_type;
    util_global_odu_line_payload_t ts_type;
    UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS];
    UINT8 num_tribslot;
    UINT8 num_cal_entries;
    UINT8 tribslot_added[UTIL_GLOBAL_MAX_NUM_TS];
    BOOL8 tribslot_occupied[UTIL_GLOBAL_MAX_NUM_TS];
    UINT8 num_active_ts;
} util_global_odu_ho_chnl_cfg_t;

/*******************************************************************************
* STRUCTURE: util_global_chnl_map_state_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the status of a channel. This keeps track of the current
*   state of the channel including its mapping state, activation state and the
*   number of times this channel has been used as the source in a mapping.
*
* ELEMENTS:
*       input_map_count   - The number of times this channel has been used
*                           as an input in a mapping operation. This is used 
*                           to keep track for deprovisioning multicasted
*                           mappings.
*       dest_active_count - The number of active channels going to this output
*                           port and channel
*       is_input_mapped   - TRUE if this channel has been used as an input for
*                           a mapping.
*       is_output_mapped  - TRUE if this channel has been used as an output for
*                           a mapping.
*       is_active         - TRUE if this channel has been activated.
*
*******************************************************************************/
typedef struct util_global_chnl_map_state_t
{
    UINT32 input_map_count;
    UINT32 dest_active_count;
    BOOL is_input_mapped;
    BOOL is_output_mapped;
} util_global_chnl_map_state_t;

/*******************************************************************************
* STRUCTURE: util_global_chnl_operational_state_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the operational status of a handle. 
*   This keeps track of both the current mapping, and activation
*   state of the channel.
*
* ELEMENTS:
*   map_state            - The mapping state of the channel. See 
*                          util_global_chnl_map_state_t for more information.
*   active_state         - The activation state of the channel. See 
*                          util_global_chnl_map_state_t for more information.
*
*
*******************************************************************************/
typedef struct util_global_chnl_operational_state_t
{
    util_global_chnl_map_state_t         map_state;
    util_global_chnl_activated_state_t   active_state;
} util_global_chnl_operational_state_t;

/*******************************************************************************
* STRUCTURE: util_global_cpb_port_data_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common cpb input / output parameters
*
* ELEMENTS:
*       op_state           - The operational status of a handle.
*       port_type          - See enum, util_global_cpb_port_t
*       channel            - UINT32 CPB DPI channel
*       data_source        - indicates the data source.
*                            Only for an input port set for CBRC, ENET_LINE 
*                            or MAPOTN sources. 
*                            Used by API in order to select which queue 
*                            system to transfer the signal on.\n
*                            0 -- line SerDes \n
*                            1 -- ODUKSW \n
*
*******************************************************************************/
typedef struct util_global_cpb_port_data_t
{
    util_global_chnl_operational_state_t op_state;
    util_global_cpb_port_t port_type;
    UINT32 channel;
    BOOL8 data_source;
} util_global_cpb_port_data_t;

/*******************************************************************************
* STRUCTURE: util_global_oduk_port_data_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common oduksw input / output parameters
*
* ELEMENTS:
*       op_state        - See enum, util_global_chnl_operational_state_t
*       port_type       - See enum, util_global_oduk_port_t
*       channel         - UINT32 ODUKSW DPI channel 
*       odu_level       - See enum, util_global_odu_level_t
*       lineotn_src     - ODU Framer Data Source
*                         
*
*******************************************************************************/
typedef struct util_global_oduk_port_data_t
{
    util_global_chnl_operational_state_t op_state;
    util_global_oduk_port_t port_type;
    UINT32 channel;   
    util_global_odu_level_t odu_level;
    BOOL8 lineotn_src;
} util_global_oduk_port_data_t;


/*******************************************************************************
* STRUCTURE: util_global_chnl_header_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common channel header parameters
*
* ELEMENTS:
*       data_key        - always set to UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID
*       prov_state      - the current operational state of this channel. See
*                         util_global_chnl_operational_state_t for more info.
*                         more info.
*
*******************************************************************************/
typedef struct util_global_chnl_header_t
{
    UINT32                               data_key;
    util_global_chnl_provisioned_state_t prov_state;
} util_global_chnl_header_t;


/*******************************************************************************
* STRUCTURE: util_global_switch_data_def_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the common cpb digi input / output parameters
*
* ELEMENTS:
*       header          - First element of all channel handles
*       cpb_data        - the cpb data. See util_global_cpb_port_data_t for 
*                         more info.
*       oduk_data       - the oduk data. See util_global_oduk_port_data_t for 
*                         more info.
*
*******************************************************************************/
typedef struct util_global_switch_data_def_t
{
    util_global_chnl_header_t       header;
    util_global_cpb_port_data_t     cpb_data;
    util_global_oduk_port_data_t    oduk_data;
} util_global_switch_data_def_t;

#ifdef CPB_BLOCK_LIST
/*******************************************************************************
* STRUCTURE: queue_block_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CPB write queue block list
*   All blocks in pages are numbered globally, and the number is called universal 
*   block id, so as to distinguish from block index inside a page.
*
* ELEMENTS:
*       queue_length         - Number of blocks in queue
*       head_block           - Head block of queue. Universal block id.
*       tail_block           - Tail block of queue. Universal block id.
*       
*******************************************************************************/
typedef struct queue_block_struct_t {
    UINT32 queue_length;
    UINT32 head_block;
    UINT32 tail_block;
} queue_block_t;
#endif

/*******************************************************************************
* STRUCTURE: util_global_cpb_slv_chnl_connect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct defines the interconnection of a DPI slave channel.\n\n
*
*   It is a sub element of the super structure cpb_connect_t.\n\n
*
*   Navigation to the next configured slave port channel is available through
*   the next_slv_port and next_slv_chnl pointers, the value CPB_INDEX_LAST
*   terminates the set of configured port channels.
*
* ELEMENTS:
*   state           - functional state of the data source
*   is_broadcast    - if TRUE this source is broadcast to multiple queue
*                     systems
*   if_multicast    - if TRUE this source is multicast from a FIFO
*   next_slv_port   - cpb_conntect_t array index of the next used DPI slave
*                     port.
*   next_slv_chnl   - cpb_conntect_t array index of the next used DPI slave
*                     channel.
*   dest_mst_port   - cpb_connect_t array index to the primary data destination
*                     of this DPI slave port
*   dest_mst_chnl   - cpb_connect_t array index to the primary data destination
*                     of this DPI slave port
*       
*******************************************************************************/
typedef struct {
    util_global_chnl_state_t state;
    BOOL8   is_broadcast;
    BOOL8   is_multicast;
    UINT8   next_slv_port;
    UINT8   next_slv_chnl;
    UINT8   dest_mst_port;
    UINT8   dest_mst_chnl;
} util_global_cpb_slv_chnl_connect_t;

/*******************************************************************************
* STRUCTURE: util_global_cpb_mst_chnl_connect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct defines the interconnection of a DPI master channel.\n\n
*
*   It is a sub element of the super structure cpb_connect_t.\n\n
*
*   The data identifies if the destination channel has a companion destination.
*   A companion destination would be where the data source for the destination,
*   is also broadcasting or multicasting to another destination channel.\n\n
*
*   The value CPB_INDEX_LAST terminates the set of companion destinations.
*
*
* ELEMENTS:
*   state - functional state of the destination port
*   is_reader_primary - if TRUE this destination is the primary reader in
*                       multicast scenario
*   is_slave_zone_primary - if TRUE this destination is the primary reader in
*                           broadcast scenario
*   dcs_inst           - DCS instance associated with Master DPI Port and Channel
*   rd_queue_id        - DCS read queue id associated with Master DPI Port and Channel
*   next_dest_mst_port - Companion destination port
*   next_dest_mst_chnl - Companion destination channel
*   source_slv_port    - Source Slave Port
*   source_slv_chnl    - Source Slave Chnl
*
*       
*******************************************************************************/
typedef struct {
    util_global_chnl_state_t state;
    BOOL8  is_reader_primary; 
    BOOL8  is_slave_zone_primary;
    UINT8 dcs_inst;
    UINT8 rd_queue_id;
    UINT8 next_dest_mst_port;
    UINT8 next_dest_mst_chnl;
    UINT8 source_slv_port;
    UINT8 source_slv_chnl;
} util_global_cpb_mst_chnl_connect_t;

/*******************************************************************************
* STRUCTURE: util_global_cpb_chnl_connect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct defines the interconnection of a bidirectional DPI channel.\n\n
*
*   It is an element of the super structure cpb_connect_t, which provides the
*   addressing of resource associated with the DPI slave and master port channel
*   data.
*
* ELEMENTS:
*
*       
*******************************************************************************/
typedef struct {
    util_global_cpb_slv_chnl_connect_t slv;
    util_global_cpb_mst_chnl_connect_t mst; 
} util_global_cpb_chnl_connect_t;

/*******************************************************************************
* STRUCTURE: util_global_cpb_connect_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This struct defines the interconnection and activation state of port 
*   channels for the entire CPB subsystem. \n\n
*
* ELEMENTS:
*   chnl           - 2D array of port channel data
*   queue_block    - See queue_block_t. 4 is for four write/push ports.
*   port_bulk_update_required   - 2D array of the ODUKSW ports and their direction that
*                                 indicate if a bulk reprov needs to be performed on it.
*       
*******************************************************************************/
typedef struct {
    util_global_cpb_chnl_connect_t chnl[UTIL_GLOBAL_CPB_CONNECT_MAX_NUM_PORTS][UTIL_GLOBAL_CPB_CONNECT_MAX_NUM_CHNLS_PER_PORT];
#ifdef CPB_BLOCK_LIST
    queue_block_t                queue_block[4][UTIL_GLOBAL_CPB_CONNECT_MAX_NUM_CHNLS_PER_PORT];
#endif
    BOOL8                        port_bulk_update_required[UTIL_GLOBAL_ODUKSW_CONNECT_MAX_NUM_PORTS][LAST_UTIL_GLOBAL_DATAPATH_DIR];
} util_global_cpb_connect_t;

/*******************************************************************************
* STRUCTURE: util_global_restart_init_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for subsystem/block handle_restart_init
*
* ELEMENTS:
*   is_warm_restart             - TRUE : perform a warm restart init. \n
*                                 FALSE: perform a crash restart init.
*   alloc_mem                   - TRUE : allocate memory pools\n
*                               - FALSE: do not allocate memory pools\n
*   phase                       - restart_init phase
*   dcpb_connect                - pointer to DCPB connect
*   dcpb_connect                - pointer to OCPB connect
*   opaque                      - pointer to a void pointer  
*******************************************************************************/
typedef struct util_global_restart_init_cfg_t
{
    BOOL8 is_warm_restart;
    BOOL8 alloc_mem;
    util_global_restart_init_phase_t phase;
    util_global_cpb_connect_t       *dcpb_connect;
    util_global_cpb_connect_t       *ocpb_connect;
    void                            *opaque;
} util_global_restart_init_cfg_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR util_global_max_client_num_ts_get(util_global_odukp_type_t server_type,
                                                   util_global_odukp_type_t client_type,
                                                   util_global_odu_line_payload_t ts_type,
                                                   UINT32 *num_tribslot);

PUBLIC PMC_ERROR util_global_max_server_num_ts_get(util_global_parent_t parent,
                                                   util_global_odukp_type_t server_type,
                                                   util_global_odu_line_payload_t ts_type,
                                                   UINT32 *num_tribslot);

PUBLIC PMC_ERROR util_global_opu_pt_assign(util_global_parent_t parent,
                                           util_global_odukp_type_t server_type,
                                           UINT32 client_type,
                                           util_global_mapping_mode_t mapping_mode,
                                           util_global_odu_line_payload_t ts_type,
                                           util_global_opu_payload_type_t *payload_type);

PUBLIC PMC_ERROR util_global_msi_cfg(UINT32 ts_loc, util_global_odukp_type_t server_type,
                                     UINT32 client_type, util_global_odu_line_payload_t ts_type,
                                     UINT32 ts_port, UINT32 *msi_byte);

PUBLIC PMC_ERROR util_global_cm_calc(util_global_odukp_type_t server_type,
                                     UINT32 client_type, 
                                     DOUBLE client_rate,
                                     UINT32 num_tribslot,
                                     BOOL   use_custom_ppm,
                                     UINT32 ppm_offset,
                                     UINT32 *cm_max, UINT32 *cm_min);

/*PUBLIC PMC_ERROR util_global_cn_calc(util_global_odukp_type_t server_type,
                                     UINT32 client_type,
                                     DOUBLE client_rate, UINT32 num_tribslot,
                                     UINT32 *cn, UINT32 *cn_n, UINT32 *cn_d);*/

PUBLIC PMC_ERROR util_global_cn_calc(util_global_parent_t parent,
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type, 
                                     DOUBLE client_rate, 
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 num_tribslot,
                                     UINT32 *cn, UINT32 *cn_n, UINT32 *cn_d);

PUBLIC PMC_ERROR util_global_golden_cn_calc(UINT32 frm_period_n, UINT32 frm_period_d,
                                            DOUBLE client_bitrate, UINT32 *cn_base_int_ptr, 
                                            UINT32 *cn_base_n_ptr, UINT32 *cn_base_d_ptr);

PUBLIC PMC_ERROR util_global_client_rate_get(UINT32 client_type, 
                                             DOUBLE *client_rate,
                                             DOUBLE *ppm_rate);  

PUBLIC void util_global_fract_to_dec_conv(DOUBLE fract,
                                          UINT32 *num,
                                          UINT32 *den);

PUBLIC void util_global_fract_to_dec_conv_max_num(DOUBLE fract,
                                                  UINT32 max_num,
                                                  UINT32 *num,
                                                  UINT32 *den);

PUBLIC BOOL util_global_is_chnl_prov(void*  chnl_handle);

PUBLIC BOOL8 util_global_isnumber(char *str);
PUBLIC pmc_energy_state_t util_global_recover_energy_state(BOOL8 power_flags0,
                                                           BOOL8 power_flags1,
                                                           BOOL8 power_flags2);

PUBLIC util_global_odukp_type_t util_global_odukp_type_per_num_entries_get(UINT32 num_entries,
                                                                           util_global_mapping_mode_t mapping_mode);
PUBLIC BOOL8 util_global_array_compare(UINT8 ptr_a[], UINT8 ptr_b[], UINT32 array_size);

PUBLIC BOOL8 util_global_double_compare(DOUBLE a, DOUBLE b, DOUBLE epsilon);

PUBLIC util_global_direction_t util_global_direction_value_compute(util_global_direction_t old_dir,
                                                                   util_global_direction_t dir,
                                                                   BOOL8                   enable);
PUBLIC void util_global_vert_eye_opening_calc(UINT64 bin_buffer[64], 
                                              UINT32 *port_verteye_opening);

PUBLIC BOOL8 util_global_chnl_prov_status_get(util_global_chnl_provisioned_state_t  chnl_state,
                                              util_global_direction_t               dir);
PUBLIC BOOL8 util_global_rx_or_tx_chnl_prov_status_get(util_global_chnl_provisioned_state_t  chnl_state);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_GLOBAL_H */

/*
** end of file
*/

