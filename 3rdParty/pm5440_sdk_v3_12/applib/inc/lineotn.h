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
*     This file contains functions for the creation and destruction of
*     LINE_OTN subsystem datapaths.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _LINEOTN_H
#define _LINEOTN_H

#ifdef __cplusplus
extern "C" {
#endif


/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "lineotn_api.h"
#include "otu_frm.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: lineotn_cfg_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated lineOTN configuration modes.
*   LineOTN has 19 supported modes as specified below.
*
* ELEMENTS:
*    LINE_OTN_CFG_0  -     1x100GB                     1xOTL4.10                           
*    LINE_OTN_CFG_1  -     1x100GB                     1xSFI-S(n=10+1)                     
*    LINE_OTN_CFG_2  -     1x100GB                     2xSFI-S(n=5+1)                      
*    LINE_OTN_CFG_3  -     12x10GB                     12xOTU2                             
*    LINE_OTN_CFG_4  -     1x40GB + 8x10GB             1xOTL3.4 + 8xOTU2                   
*    LINE_OTN_CFG_5  -     1x40GB + 7x10GB             1xSFI-S(n=4+1) + 7xOTU2             
*    LINE_OTN_CFG_6  -     1x40GB + 1x40GB + 3x10GB    1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  
*    LINE_OTN_CFG_7  -     1x40GB + 8x10GB             1xSFI5.1 + 8xOTU2                   
*    LINE_OTN_CFG_8  -     1x40GB + 1x40GB + 4x10GB    1xOTL3.4 + 1xSFI5.1 + 4xOTU2        
*    LINE_OTN_CFG_9  -     1x40GB + 1x40GB + 4x10GB    1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  
*    LINE_OTN_CFG_10 -     1x40GB + 1x40GB + 1x40GB    1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1
*    LINE_OTN_CFG_11 -     2x40GB + 4x10GB             2xOTL3.4 + 4xOTU2                   
*    LINE_OTN_CFG_12 -     2x40GB + 1x40GB             2xOTL3.4 + 1xSFI5.1                 
*    LINE_OTN_CFG_13 -     2x40GB + 2x10GB             2xSFI-S(n=4+1) + 2xOTU2             
*    LINE_OTN_CFG_14 -     2x40GB + 1x40GB             2xSFI-S(n=4+1) + 1xSFI5.1           
*    LINE_OTN_CFG_15 -     2x40GB + 4x10GB             2xSFI5.1 + 4xOTU2                   
*    LINE_OTN_CFG_16 -     2x40GB + 1x40GB             2xSFI5.1 + 1xOTL3.4                 
*    LINE_OTN_CFG_17 -     2x40GB + 1x40GB             2xSFI5.1 + 1xSFI-S(n=4+1)           
*    LINE_OTN_CFG_18 -     3x40GB                      3xOTL3.4                            
*    LAST_LINE_OTN_CFG - this is for out of range values error handling                                  
*
*******************************************************************************/
typedef enum
{                       /* Max data rate    Rate breakdown              HW interfaces                       */
    LINE_OTN_CFG_0 = 0, /* 100GB            1x100GB                     1xOTL4.10                           */
    LINE_OTN_CFG_1,     /* 100GB            1x100GB                     1xSFI-S(n=10+1)                     */
    LINE_OTN_CFG_2,     /* 100GB            1x100GB                     2xSFI-S(n=5+1)                      */
    LINE_OTN_CFG_3,     /* 120GB            12x10GB                     12xOTU2                             */
    LINE_OTN_CFG_4,     /* 120GB            1x40GB + 8x10GB             1xOTL3.4 + 8xOTU2                   */
    LINE_OTN_CFG_5,     /* 110GB            1x40GB + 7x10GB             1xSFI-S(n=4+1) + 7xOTU2             */
    LINE_OTN_CFG_6,     /* 110GB            1x40GB + 1x40GB + 3x10GB    1xOTL3.4 + 1xSFI-S(n=4+1) + 3xOTU2  */
    LINE_OTN_CFG_7,     /* 120GB            1x40GB + 8x10GB             1xSFI5.1 + 8xOTU2                   */
    LINE_OTN_CFG_8,     /* 120GB            1x40GB + 1x40GB + 4x10GB    1xOTL3.4 + 1xSFI5.1 + 4xOTU2        */
    LINE_OTN_CFG_9,     /* 120GB            1x40GB + 1x40GB + 4x10GB    1xSFI-S(n=4+1) + 1xSFI5.1 + 4xOTU2  */
    LINE_OTN_CFG_10,    /* 120GB            1x40GB + 1x40GB + 1x40GB    1xOTL3.4 + 1xSFI-S(n=4+1) + 1xSFI5.1*/
    LINE_OTN_CFG_11,    /* 120GB            2x40GB + 4x10GB             2xOTL3.4 + 4xOTU2                   */
    LINE_OTN_CFG_12,    /* 120GB            2x40GB + 1x40GB             2xOTL3.4 + 1xSFI5.1                 */
    LINE_OTN_CFG_13,    /* 100GB            2x40GB + 2x10GB             2xSFI-S(n=4+1) + 2xOTU2             */
    LINE_OTN_CFG_14,    /* 120GB            2x40GB + 1x40GB             2xSFI-S(n=4+1) + 1xSFI5.1           */
    LINE_OTN_CFG_15,    /* 120GB            2x40GB + 4x10GB             2xSFI5.1 + 4xOTU2                   */
    LINE_OTN_CFG_16,    /* 120GB            2x40GB + 1x40GB             2xSFI5.1 + 1xOTL3.4                 */
    LINE_OTN_CFG_17,    /* 120GB            2x40GB + 1x40GB             2xSFI5.1 + 1xSFI-S(n=4+1)           */
    LINE_OTN_CFG_18,    /* 120GB            3x40GB                      3xOTL3.4                            */

    LAST_LINE_OTN_CFG   /* this is for out of range values error handling                                   */

}lineotn_cfg_type_t;


/*******************************************************************************
* ENUM: lineotn_power_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for lineOTN power mode.
*
* ELEMENTS:
*   LINE_OTN_PWR_OPERATIONAL    - high power, all lineOTN blocks are in operational mode
*   LINE_OTN_PWR_LOWPWR         - All lineOTN blocks are in low power mode
*   LAST_LINE_OTN_PWR           - place holder for out of range values
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_PWR_OPERATIONAL = 0,
    LINE_OTN_PWR_LOWPWR,
    LAST_LINE_OTN_PWR      
} lineotn_power_mode_t;

/*******************************************************************************
* ENUM: lineotn_reset_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for lineOTN reset mode.
*
* ELEMENTS:
*   LINE_OTN_OUT_OF_RESET   - All lineOTN blocks are not held in reset.
*   LINE_OTN_IN_RESET       - All lineOTN blocks are held in reset.
*   LAST_LINE_OTN_RESET     - place holder for out of range values
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_OUT_OF_RESET = 0,
    LINE_OTN_IN_RESET,
    LAST_LINE_OTN_RESET     
} lineotn_reset_mode_t;


/******************************************************************************
*
* ENUM: mld_hw_module_t
*
______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated MLD hardware modules.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    MLD_SFI_51_0 = 0,
    MLD_SFI_51_1,
    MLD_SFIS_N_10,
    MLD_SFIS_N_4_0,
    MLD_SFIS_N_4_1,
    MLD_OPSM23_0,
    MLD_OPSM23_1,
    MLD_OPSM23_2,
    MLD_OPSM4,
    MLD_BLOCK_HW,

    LAST_MLD_HW_MODULE
}mld_hw_module_t;


/******************************************************************************
*
* ENUM: mld_ifc_type_t
*
______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated MLD DSI stream interface type; used to indicate which 
*   line encoding mode is enabled.
*
* ELEMENTS:
*   MLD_SFI_51        - Indicates that the DSI stream uses the SFI-51 protocol
*   MLD_SFI_S         - Indicates that the DSI stream uses the SFI-S protocol 
*   MLD_OTL           - Indicates that the DSI stream uses the OTL protocol 
*   MLD_OTU2          - Indicates that the DSI stream is an OTU2 type 
*   MLD_NA            - Not active 
*   LAST_MLD_IFC_TYPE - Placeholder for error handling purposes 
*
*******************************************************************************/
typedef enum
{
    MLD_SFI_51 = 0,
    MLD_SFIS,
    MLD_OTL,
    MLD_OTU2,
    MLD_NA,  
    LAST_MLD_IFC_TYPE,
} mld_ifc_type_t;

/*******************************************************************************
* ENUM: lineotn_pwr_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying SS power state state.
*
* ELEMENTS:
*    LINE_OTN_ACTIVE         - Subsystem is enabled
*    LINE_OTN_LOW_PWR_RST    - Subsystem is in lower power and reset
*    LAST_LINE_OTN_PWR_STATE - Placeholder for error handling purposes
*******************************************************************************/
typedef enum
{
    LINE_OTN_ACTIVE = 0,
    LINE_OTN_LOW_PWR_RST,
    LAST_LINE_OTN_PWR_STATE      
}lineotn_pwr_state_t;


/*******************************************************************************
* ENUM: lineotn_sfi51_swap_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying sfi 51 lines swap state.
*
* ELEMENTS:
*    LINE_OTN_SFI51_LANES_SWAP_DISABLED - SFI-51 lines not swapped
*    LINE_OTN_SFI51_LANES_SWAP_ENABLED -  SFI-51 lines swapped
*    LAST_LINE_OTN_SFI51_LANES_SWAP_STATE - this is for out of range values error handling
* 
*******************************************************************************/
typedef enum
{
    LINE_OTN_SFI51_LANES_SWAP_DISABLED= 0,
    LINE_OTN_SFI51_LANES_SWAP_ENABLED,

    LAST_LINE_OTN_SFI51_LANES_SWAP_STATE      /* this is for out of range values error handling */
}lineotn_sfi51_swap_state_t;


/*******************************************************************************
* ENUM: sysotn_intr_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure enumerates the sysotn interrupt type.
*
* ELEMENTS:
*  SYSOTN_INTR_TYPE_CPUIF_INT_DISABLE     - cpuif interrupt disabled
*  SYSOTN_INTR_TYPE_CPUIF_INT_DEVICE_PIN  - cpuif interrupt device pin
*  SYSOTN_INTR_TYPE_CPUIF_INT_OTHER       - cpuif interrupt MSI, MSI-X or Legacy
*  SYSOTN_INTR_TYPE_SYSOTN_DISABLED       - application is not SYSOTN
*
*******************************************************************************/
typedef enum
{
    SYSOTN_INTR_TYPE_CPUIF_INT_DISABLE = 0,
    SYSOTN_INTR_TYPE_CPUIF_INT_DEVICE_PIN,
    SYSOTN_INTR_TYPE_CPUIF_INT_OTHER,
    SYSOTN_INTR_TYPE_SYSOTN_DISABLED,

    LAST_SYSOTN_INTR_TYPE
}sysotn_intr_type_t;

/*
** Constants
*/

#define LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE  4
#define LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE 20
#define LINEOTN_SFI51_1_DSI_STREAM_IDX      17

#define SYSOTN_INT_TYPE_STORAGE_MASK         0xC0000000
#define SYSOTN_INT_TYPE_STORAGE_MASK_REVERSE 0x3FFFFFFF
/*
** Macro Definitions
*/

#ifndef DOXYGEN_PUBLIC_ONLY

/* Interrupt Table Macro */
/* OTU_FRM_INT(handle, tsb_name, tsb_field, struct_field) */
#define LINEOTN_INT_TABLE_DEFINE(sysotn) \
    LINEOTN_INT(lineotn_handle, fecstat, AIR_ERR_INT                      , fecstat_air_err_int_i               ); \

/* OTU_FRM_INT(handle, tsb_name, tsb_field, struct_field) */
#define LINEOTN_OPSM23_INT_CHNL_TABLE_DEFINE(sysotn) \
    LINEOTN_INT(lineotn_handle, opsm23, TX_SFI_FIFO_OVERFLOW_INT          , opsm23_tx_sfi_fifo_overflow_int_i   ); \
    LINEOTN_INT(lineotn_handle, opsm23, TX_SFI_FIFO_UNDERFLOW_INT         , opsm23_tx_sfi_fifo_underflow_int_i  ); \
    LINEOTN_INT_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_SYS_FIFO_OVERFLOW_INT    , opsm23_tx_sys_fifo_overflow_int_i   ); \
    LINEOTN_INT_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_SYS_FIFO_UNDERFLOW_INT   , opsm23_tx_sys_fifo_underflow_int_i  ); \
    LINEOTN_INT_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_LANE_FIFO_OVERFLOW_INT   , opsm23_tx_lane_fifo_overflow_int_i  ); \
    LINEOTN_INT_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_LANE_FIFO_UNDERFLOW_INT  , opsm23_tx_lane_fifo_underflow_int_i ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SFI_LOS_INT                    , opsm23_rx_sfi_los_int_i             ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SFI_OOF_INT                    , opsm23_rx_sfi_oof_int_i             ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SFI_LOF_INT                    , opsm23_rx_sfi_lof_int_i             ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_OTU_OOF_INT                    , opsm23_rx_otu_oof_int_i             ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_OTU_LOF_INT                    , opsm23_rx_otu_lof_int_i             ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SFI_AIS_INT                    , opsm23_rx_sfi_ais_int_i             ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_LOL_INT                        , opsm23_rx_lol_int_i                 ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SKEW_RANGE_VIOLATION_INT       , opsm23_rx_skew_range_violation_int_i); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_ALIGN_CHANGE_INT               , opsm23_rx_align_change_int_i        ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_DESKEW_CHANGE_INT              , opsm23_rx_deskew_change_int_i       ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SFI_FIFO_OVERFLOW_INT          , opsm23_rx_sfi_fifo_overflow_int_i   ); \
    LINEOTN_INT(lineotn_handle, opsm23, RX_SFI_FIFO_UNDERFLOW_INT         , opsm23_rx_sfi_fifo_underflow_int_i  ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_LOS_INT           , opsm23_rx_lane_los_int_i            ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_OOF_INT           , opsm23_rx_lane_oof_int_i            ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_LOF_LOFLANE_INT   , opsm23_rx_lane_lof_loflane_int_i    ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_OTUK_AIS_INT      , opsm23_rx_lane_otuk_ais_int_i       ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_OTL_AIS_INT       , opsm23_rx_lane_otl_ais_int_i        ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_ALIGN_CHANGE_INT  , opsm23_rx_lane_align_change_int_i   ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_FIFO_ERROR_INT    , opsm23_rx_lane_fifo_error_int_i     ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_OOR_INT           , opsm23_rx_lane_oor_int_i            ); \
    LINEOTN_INT_LANE(lineotn_handle, opsm23, 4, RX_LANE_LOR_INT           , opsm23_rx_lane_lor_int_i            ); \

/* OTU_FRM_INT(handle, tsb_name, tsb_field, struct_field) */
#define LINEOTN_OPSM4_INT_CHNL_TABLE_DEFINE(sysotn) \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_SFI_FIFO_OVERFLOW_INT     , opsm4_tx_sfi_fifo_overflow_int_i    ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_SFI_FIFO_UNDERFLOW_INT    , opsm4_tx_sfi_fifo_underflow_int_i   ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_SYS_FIFO_OVERFLOW_INT     , opsm4_tx_sys_fifo_overflow_int_i    ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_SYS_FIFO_UNDERFLOW_INT    , opsm4_tx_sys_fifo_underflow_int_i   ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_FAS_IAE_INT          , opsm4_tx_lane_fas_iae_int_i         ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_MFAS_IAE_INT         , opsm4_tx_lane_mfas_iae_int_i        ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_FIFO_OVERFLOW_INT    , opsm4_tx_lane_fifo_overflow_int_i   ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_FIFO_UNDERFLOW_INT   , opsm4_tx_lane_fifo_underflow_int_i  ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  TX_OTUK_AIS_INT              , opsm4_tx_otuk_ais_int_i             ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_LOS_INT               , opsm4_rx_sfi_los_int_i              ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_OOF_INT               , opsm4_rx_sfi_oof_int_i              ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_LOF_INT               , opsm4_rx_sfi_lof_int_i              ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_OTU_OOF_INT               , opsm4_rx_otu_oof_int_i              ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_OTU_LOF_INT               , opsm4_rx_otu_lof_int_i              ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_AIS_INT               , opsm4_rx_sfi_ais_int_i              ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_LOL_INT                   , opsm4_rx_lol_int_i                  ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SKEW_RANGE_VIOLATION_INT  , opsm4_rx_skew_range_violation_int_i ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_ALIGN_CHANGE_INT      , opsm4_rx_sfi_align_change_int_i     ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_DESKEW_CHANGE_INT         , opsm4_rx_deskew_change_int_i        ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_FIFO_OVERFLOW_INT     , opsm4_rx_sfi_fifo_overflow_int_i    ); \
    LINEOTN_INT_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_FIFO_UNDERFLOW_INT    , opsm4_rx_sfi_fifo_underflow_int_i   ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_LOS_INT          , opsm4_rx_lane_los_int_i         ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_OOF_INT          , opsm4_rx_lane_oof_int_i         ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_LOF_LOFLANE_INT  , opsm4_rx_lane_lof_loflane_int_i ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_OOM_INT          , opsm4_rx_lane_oom_int_i         ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_OTUK_AIS_INT     , opsm4_rx_lane_otuk_ais_int_i    ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_OTL_AIS_INT      , opsm4_rx_lane_otl_ais_int_i     ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_ALIGN_CHANGE_INT , opsm4_rx_lane_align_change_int_i); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_FIFO_ERROR_INT   , opsm4_rx_lane_fifo_error_int_i  ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_OOR_INT          , opsm4_rx_lane_oor_int_i         ); \
    LINEOTN_INT_100G_LANE(lineotn_handle, sysotn##opsm4, 20, RX_LANE_LOR_INT          , opsm4_rx_lane_lor_int_i         ); \



/* Status Table Macro */
#define LINEOTN_OPSM23_STATUS_CHNL_TABLE_DEFINE(sysotn)                                           \
    LINEOTN_STATUS(lineotn_handle, opsm23, TX_SFI_FIFO_OVERFLOW_INT_V           , opsm23_tx_sfi_fifo_overflow_int_v   ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, TX_SFI_FIFO_UNDERFLOW_INT_V          , opsm23_tx_sfi_fifo_underflow_int_v  ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_SYS_FIFO_OVERFLOW_INT_V    , opsm23_tx_sys_fifo_overflow_int_v   ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_SYS_FIFO_UNDERFLOW_INT_V   , opsm23_tx_sys_fifo_underflow_int_v  ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_LANE_FIFO_OVERFLOW_INT_V   , opsm23_tx_lane_fifo_overflow_int_v  ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, opsm23, LINEOTN_MLD_OTL34_NUM_LOGICAL_LANE, TX_LANE_FIFO_UNDERFLOW_INT_V  , opsm23_tx_lane_fifo_underflow_int_v ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SFI_LOS_INT_V                     , opsm23_rx_sfi_los_int_v             ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SFI_OOF_INT_V                     , opsm23_rx_sfi_oof_int_v             ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SFI_LOF_INT_V                     , opsm23_rx_sfi_lof_int_v             ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_OTU_OOF_INT_V                     , opsm23_rx_otu_oof_int_v             ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_OTU_LOF_INT_V                     , opsm23_rx_otu_lof_int_v             ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SFI_AIS_INT_V                     , opsm23_rx_sfi_ais_int_v             ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_LOL_INT_V                         , opsm23_rx_lol_int_v                 ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SKEW_RANGE_VIOLATION_INT_V        , opsm23_rx_skew_range_violation_int_v); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_ALIGN_CHANGE_INT_V                , opsm23_rx_align_change_int_v        ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_DESKEW_CHANGE_INT_V               , opsm23_rx_deskew_change_int_v       ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SFI_FIFO_OVERFLOW_INT_V           , opsm23_rx_sfi_fifo_overflow_int_v   ); \
    LINEOTN_STATUS(lineotn_handle, opsm23, RX_SFI_FIFO_UNDERFLOW_INT_V          , opsm23_rx_sfi_fifo_underflow_int_v  ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_LOS_INT_V            , opsm23_rx_lane_los_int_v           ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_OOF_INT_V            , opsm23_rx_lane_oof_int_v           ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_LOF_LOFLANE_INT_V    , opsm23_rx_lane_lof_loflane_int_v   ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_OTUK_AIS_INT_V       , opsm23_rx_lane_otuk_ais_int_v      ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_OTL_AIS_INT_V        , opsm23_rx_lane_otl_ais_int_v       ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_ALIGN_CHANGE_INT_V   , opsm23_rx_lane_align_change_int_v  ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_FIFO_ERROR_INT_V     , opsm23_rx_lane_fifo_error_int_v    ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_OOR_INT_V            , opsm23_rx_lane_oor_int_v           ); \
    LINEOTN_STATUS_LANE(lineotn_handle, opsm23, 4, RX_LANE_LOR_INT_V            , opsm23_rx_lane_lor_int_v           ); \

#define LINEOTN_OPSM4_STATUS_CHNL_TABLE_DEFINE(sysotn)                                           \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_SFI_FIFO_OVERFLOW_INT_V      , opsm4_tx_sfi_fifo_overflow_int_v   ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_SFI_FIFO_UNDERFLOW_INT_V     , opsm4_tx_sfi_fifo_underflow_int_v  ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_SYS_FIFO_OVERFLOW_INT_V      , opsm4_tx_sys_fifo_overflow_int_v   ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_SYS_FIFO_UNDERFLOW_INT_V     , opsm4_tx_sys_fifo_underflow_int_v  ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_FAS_IAE_INT_V           , opsm4_tx_lane_fas_iae_int_v        ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_MFAS_IAE_INT_V          , opsm4_tx_lane_mfas_iae_int_v       ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_FIFO_OVERFLOW_INT_V     , opsm4_tx_lane_fifo_overflow_int_v  ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_LANE_FIFO_UNDERFLOW_INT_V    , opsm4_tx_lane_fifo_underflow_int_v ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  TX_OTUK_AIS_INT_V               , opsm4_tx_otuk_ais_int_v            ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_LOS_INT_V                , opsm4_rx_sfi_los_int_v             ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_OOF_INT_V                , opsm4_rx_sfi_oof_int_v             ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_LOF_INT_V                , opsm4_rx_sfi_lof_int_v             ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_OTU_OOF_INT_V                , opsm4_rx_otu_oof_int_v             ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_OTU_LOF_INT_V                , opsm4_rx_otu_lof_int_v             ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_AIS_INT_V                , opsm4_rx_sfi_ais_int_v             ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_LOL_INT_V                    , opsm4_rx_lol_int_v                 ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SKEW_RANGE_VIOLATION_INT_V   , opsm4_rx_skew_range_violation_int_v); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_ALIGN_CHANGE_INT_V       , opsm4_rx_sfi_align_change_int_v    ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_DESKEW_CHANGE_INT_V          , opsm4_rx_deskew_change_int_v       ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_FIFO_OVERFLOW_INT_V      , opsm4_rx_sfi_fifo_overflow_int_v   ); \
    LINEOTN_STATUS_100G(lineotn_handle, sysotn##opsm4,  RX_SFI_FIFO_UNDERFLOW_INT_V     , opsm4_rx_sfi_fifo_underflow_int_v  ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_LOS_INT_V         , opsm4_rx_lane_los_int_v         ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_OOF_INT_V         , opsm4_rx_lane_oof_int_v         ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_LOF_LOFLANE_INT_V , opsm4_rx_lane_lof_loflane_int_v ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_OOM_INT_V         , opsm4_rx_lane_oom_int_v         ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_OTUK_AIS_INT_V    , opsm4_rx_lane_otuk_ais_int_v    ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_OTL_AIS_INT_V     , opsm4_rx_lane_otl_ais_int_v     ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_ALIGN_CHANGE_INT_V, opsm4_rx_lane_align_change_int_v); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_FIFO_ERROR_INT_V  , opsm4_rx_lane_fifo_error_int_v  ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_OOR_INT_V         , opsm4_rx_lane_oor_int_v         ); \
    LINEOTN_STATUS_100G_LANE(lineotn_handle, sysotn##opsm4,  20, RX_LANE_LOR_INT_V         , opsm4_rx_lane_lor_int_v         ); \


/* SFIS_RX_INT(handle, tsb_name, tsb_field, struct_field) */
#define LINEOTN_SFIS_INT_CHNL_TABLE_DEFINE(sysotn) \
    LINEOTN_INT_ARRAY(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE,  RXLOS          , rxlos_i          ); \
    LINEOTN_INT      (lineotn_handle, sfis_rx,                         RXLOS_DSC      , rxlos_dsc_i      ); \
    LINEOTN_INT      (lineotn_handle, sfis_rx,                         RXOOF          , rxoof_i          ); \
    LINEOTN_INT_ARRAY(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE,  RXOOA          , rxooa_i          ); \
    LINEOTN_INT_ARRAY(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE,  RXUDR          , rxudr_i          ); \
    LINEOTN_INT      (lineotn_handle, sfis_rx,                         RXUDR_DSC      , rxudr_dsc_i      ); \
    LINEOTN_INT_FIELD(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE,  RXOVR          , rxovr_i          ); \
    LINEOTN_INT      (lineotn_handle, sfis_rx,                         RXOVR_DSC      , rxovr_dsc_i      ); \
    LINEOTN_INT_ARRAY(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE,  RX_BITERR      , rx_biterr_i      ); \
    LINEOTN_INT      (lineotn_handle, sfis_rx,                         RX_BITERR_DSC  , rx_biterr_dsc_i  ); \
    LINEOTN_INT      (lineotn_handle, sfis_tx,                         TXOVR          , txovr_i          ); \
    LINEOTN_INT      (lineotn_handle, sfis_tx,                         TXUDR          , txudr_i          ); \

/* Status Table Macro */
#define LINEOTN_SFIS_STATUS_CHNL_TABLE_DEFINE(sysotn) \
    LINEOTN_STATUS_ARRAY(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE, RXLOS_V   , rxlos_v      ); \
    LINEOTN_STATUS      (lineotn_handle, sfis_rx, RXLOS_DSC_V                      , rxlos_dsc_v  ); \
    LINEOTN_STATUS      (lineotn_handle, sfis_rx, RXOOF_V                          , rxoof_v      ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, sfis_rx, LINEOTN_SFIS_NUM_LANE, RXOOA_V   , rxooa_v      ); \


/* SFI51_INT(handle, tsb_name, tsb_field, struct_field) */
#define LINEOTN_SFI51_INT_CHNL_TABLE_DEFINE(sysotn) \
    LINEOTN_INT      (lineotn_handle, sfi51, RX_DESKEW_LOS                            , rx_deskew_los_i        ); \
    LINEOTN_INT      (lineotn_handle, sfi51, RX_DESKEW_OOF                            , rx_deskew_oof_i        ); \
    LINEOTN_INT      (lineotn_handle, sfi51, RX_DESKEW_LOF                            , rx_deskew_lof_i        ); \
    LINEOTN_INT      (lineotn_handle, sfi51, RX_DESKEW_FIFO_UNFL                      , rx_deskew_fifo_unfl_i  ); \
    LINEOTN_INT      (lineotn_handle, sfi51, RX_DESKEW_FIFO_OVFL                      , rx_deskew_fifo_ovfl_i  ); \
    LINEOTN_INT_ARRAY(lineotn_handle, sfi51, LINEOTN_SFI51_NUM_LANE , RX_DESKEW_OOA   , rx_deskew_ooa_i        ); \
    LINEOTN_INT_ARRAY(lineotn_handle, sfi51, LINEOTN_SFI51_NUM_LANE , RX_CHN_BIT_ERR  , rx_chn_bit_err_i       ); \
    LINEOTN_INT      (lineotn_handle, sfi51, TX_FIFO_UNFL                             , tx_fifo_unfl_i         ); \
    LINEOTN_INT      (lineotn_handle, sfi51, TX_FIFO_OVFL                             , tx_fifo_ovfl_i         ); \

/* Status Table Macro */
#define LINEOTN_SFI51_STATUS_CHNL_TABLE_DEFINE(sysotn) \
    LINEOTN_STATUS      (lineotn_handle, sfi51, RX_DESKEW_LOS_V                           , rx_deskew_los_v        ); \
    LINEOTN_STATUS      (lineotn_handle, sfi51, RX_DESKEW_OOF_V                           , rx_deskew_oof_v        ); \
    LINEOTN_STATUS      (lineotn_handle, sfi51, RX_DESKEW_LOF_V                           , rx_deskew_lof_v        ); \
    LINEOTN_STATUS      (lineotn_handle, sfi51, RX_DESKEW_FIFO_UNFL_V                     , rx_deskew_fifo_unfl_v  ); \
    LINEOTN_STATUS      (lineotn_handle, sfi51, RX_DESKEW_FIFO_OVFL_V                     , rx_deskew_fifo_ovfl_v  ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, sfi51, LINEOTN_SFI51_NUM_LANE, RX_DESKEW_OOA_V   , rx_deskew_ooa_v        ); \
    LINEOTN_STATUS_ARRAY(lineotn_handle, sfi51, LINEOTN_SFI51_NUM_LANE, RX_CHN_BIT_ERR_V  , rx_chn_bit_err_v       ); \
    LINEOTN_STATUS      (lineotn_handle, sfi51, TX_FIFO_UNFL_V                            , tx_fifo_unfl_v         ); \
    LINEOTN_STATUS      (lineotn_handle, sfi51, TX_FIFO_OVFL_V                            , tx_fifo_ovfl_v         ); \

#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Structures and Unions
*/



/*
** Global variables
*/

/*
** Function Prototypes
*/
/*********************************** Exported helper Functions ***********************************************/

PUBLIC PMC_ERROR lineotn_find_dsi_by_dci(lineotn_handle_t           *h,
                                         UINT32                     dci_channel,
                                         UINT32                     *dsi_stream,
                                         lineotn_dsi_type_t         *dsi_type);

/*********************************** SS Init Functions ***********************************************/
PUBLIC lineotn_handle_t *lineotn_ctxt_create(pmc_handle_t *parent,
                                             UINT32 base_address,
                                             pmc_sys_handle_t *sys_handle,
                                             const char *tsb_name);

PUBLIC void lineotn_ctxt_destroy(lineotn_handle_t *lineotn_handle);

PUBLIC void lineotn_handle_init(lineotn_handle_t *lineotn_handle,
                                BOOL8 lineotn_mode);

PUBLIC BOOL lineotn_start_state_test( lineotn_handle_t *h );

PUBLIC void lineotn_init(lineotn_handle_t  *h, pmc_energy_state_t   energy_state);

PUBLIC void lineotn_set_ho_db_populate_ctxt(lineotn_handle_t *h,
                                            BOOL8 ho_db_pop);


/*********************** SS Provisioning and establishment functions ********************************/
PUBLIC PMC_ERROR lineotn_stream_prov        ( lineotn_handle_t      *h,
                                              lineotn_cfg_type_t    lineotn_cfg,
                                              UINT32                dsi_stream,
                                              lineotn_dsi_type_t    dsi_type,
                                              lineotn_deskew_type_t lineotn_deskew_type,
                                              lineotn_frame_rate_t  frame_rate,
                                              UINT32                *dci_channel_ptr );

PUBLIC PMC_ERROR lineotn_stream_deprov      ( lineotn_handle_t *h, UINT32 dsi_stream, lineotn_dsi_type_t dsi_type );
PUBLIC PMC_ERROR lineotn_stream_act         ( lineotn_handle_t *h, UINT32 dsi_stream, lineotn_dsi_type_t dsi_type,lineotn_dir_t  act_dir );
PUBLIC PMC_ERROR lineotn_stream_deact       ( lineotn_handle_t *h, UINT32 dsi_stream, lineotn_dsi_type_t dsi_type, lineotn_dir_t  deact_dir );

PUBLIC PMC_ERROR lineotn_fec_cfg                ( lineotn_handle_t        *h,
                                                  UINT32                   dsi_stream,
                                                  lineotn_dsi_type_t       dsi_type,
                                                  lineotn_fec_t            fec_type );

PUBLIC PMC_ERROR lineotn_fec_act                ( lineotn_handle_t        *h,
                                                  UINT32                   dsi_stream,
                                                  lineotn_dsi_type_t       dsi_type);


PUBLIC PMC_ERROR lineotn_fec_bypass_cfg         ( lineotn_handle_t        *h,
                                                  UINT32                   dsi_stream,
                                                  lineotn_dsi_type_t       dsi_type,
                                                  lineotn_fec_bypass_t     fec_bypass );

PUBLIC PMC_ERROR lineotn_loopback_cfg          ( lineotn_handle_t           *h,
                                                 UINT32                     dsi_stream,
                                                 lineotn_dir_t              loopback_dir,
                                                 lineotn_loopback_state_t   loopback_state );

PUBLIC PMC_ERROR lineotn_fec_type_get          ( lineotn_handle_t   *h,
                                                 UINT32             dsi_stream,
                                                 lineotn_dsi_type_t dsi_type,
                                                 lineotn_fec_t      *fec_type );

PUBLIC PMC_ERROR lineotn_fec_dec_passthru_set    ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_t      fec_type,
                                                   BOOL               dec_passthru );

PUBLIC PMC_ERROR lineotn_fec_dec_passthru_get    ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   BOOL               *dec_in_passthru_mode );
                                                   
PUBLIC PMC_ERROR lineotn_fec_dec_mode_set        ( lineotn_handle_t       *h,
                                                   UINT32                 dsi_stream,
                                                   lineotn_dsi_type_t     dsi_type,
                                                   lineotn_fec_dec_mode_t fec_dec_mode );
                                                   
PUBLIC PMC_ERROR lineotn_fec_dec_mode_get        ( lineotn_handle_t       *h,
                                                   UINT32                 dsi_stream,
                                                   lineotn_dsi_type_t     dsi_type,
                                                   lineotn_fec_dec_mode_t *fec_dec_mode );
                                                                                                                                                        

/*********************** SS FEC STAT functions ********************************/
PUBLIC PMC_ERROR lineotn_fec_stat_intf_init(lineotn_handle_t      *h,
                                            digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr);
PUBLIC PMC_ERROR lineotn_fec_stat_intf_uninit(lineotn_handle_t      *h);
PUBLIC PMC_ERROR lineotn_fec_stat_set(lineotn_handle_t *h,
                                      UINT32 dsi_stream,
                                      BOOL8 enable);
                                      
PUBLIC PMC_ERROR lineotn_fec_dec_bypass_set      ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_bypass_t fec_dec_bypass_mode );
                                                   
PUBLIC PMC_ERROR lineotn_fec_dec_bypass_get      ( lineotn_handle_t   *h,
                                                   UINT32             dsi_stream,
                                                   lineotn_dsi_type_t dsi_type,
                                                   lineotn_fec_bypass_t *fec_dec_bypass_mode );                                                   
                                                   
                                                                                         

/*********************** Block level Cfg/Get functions ********************************/


PUBLIC PMC_ERROR lineotn_consq_action_cfg       ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_action_type_t     cons_action_type,
                                                  UINT32                    defect_bm,
                                                  BOOL                      set_defects );

PUBLIC PMC_ERROR lineotn_consq_action_cfg_get   ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_action_type_t     cons_action_type,
                                                  UINT32                    defect_bm,
                                                  UINT32                    *defects_set_bm);

/* OTU_FRM functions*/

PUBLIC PMC_ERROR lineotn_mfas_oh_ins_cfg        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_mfas_ins_type_t   mfas_ins_type,
                                                  UINT32                    mfas_value );   
 
PUBLIC PMC_ERROR lineotn_mfas_oh_ins_get        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_mfas_ins_type_t   *mfas_ins_type,
                                                  UINT32                    *mfas_value );   


PUBLIC PMC_ERROR lineotn_bip8_oh_ins_cfg        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_bip8_ins_type_t   bip8_ins_type,
                                                  UINT32                    bip8_value );   
 
PUBLIC PMC_ERROR lineotn_bip8_oh_ins_get        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_bip8_ins_type_t   *bip8_ins_type,
                                                  UINT32                    *bip8_value );   
 
 
PUBLIC PMC_ERROR lineotn_iae_oh_ins_cfg         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_iae_ins_type_t    iae_ins_type,
                                                  UINT32                    iae_value );   
 
PUBLIC PMC_ERROR lineotn_iae_oh_ins_get         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_iae_ins_type_t    *iae_ins_type,
                                                  UINT32                    *iae_value );   

PUBLIC PMC_ERROR lineotn_bkwd_oh_ins_cfg        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_bkwd_oh_field_t   bkwd_ctrl_ins_field,
                                                  lineotn_bkwd_ins_type_t   bkwd_ins_type,
                                                  UINT32                    bkwd_value );   

PUBLIC PMC_ERROR lineotn_bkwd_oh_ins_get        ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_bkwd_oh_field_t   bkwd_ctrl_ins_field,
                                                  lineotn_bkwd_ins_type_t   *bkwd_ins_type,
                                                  UINT32                    *bkwd_value );   
 
PUBLIC PMC_ERROR lineotn_oh_ins_cfg             ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_field_t        ctrl_ins_field,
                                                  lineotn_oh_ins_type_t     ctrl_ins_type,
                                                  UINT32                    ctrl_field_value );   
 
PUBLIC PMC_ERROR lineotn_oh_ins_get             ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_field_t        ctrl_ins_field,
                                                  lineotn_oh_ins_type_t     *ctrl_ins_type,
                                                  UINT32                    *ctrl_field_value );

PUBLIC PMC_ERROR lineotn_oh_extract_get         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_extract_t      extract_field,
                                                  UINT32                    *extracted_val_ptr );

PUBLIC PMC_ERROR lineotn_force_maintenance_sig_cfg( lineotn_handle_t          *h,
                                                    UINT32                    dsi_stream,
                                                    lineotn_dsi_type_t        dsi_type,
                                                    lineotn_maint_sig_type_t  maint_sig_type,
                                                    lineotn_dir_t             maint_sig_dir );

PUBLIC PMC_ERROR lineotn_mi_admin_state_cfg     ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_dir_t             admin_state_dir, 
                                                  lineotn_mi_admin_state_t  admin_state );

PUBLIC PMC_ERROR lineotn_force_mi_active_cfg    ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_mi_active_type_t  mi_active_type);

PUBLIC PMC_ERROR lineotn_sm_deg_set             ( lineotn_handle_t    *h,
                                                  UINT32              dsi_stream,
                                                  lineotn_dsi_type_t  dsi_type,
                                                  lineotn_sk_sm_deg_t deg_field,
                                                  UINT32              value );

PUBLIC PMC_ERROR lineotn_sm_deg_get             ( lineotn_handle_t    *h,
                                                  UINT32              dsi_stream,
                                                  lineotn_dsi_type_t  dsi_type,
                                                  lineotn_sk_sm_deg_t deg_field,
                                                  UINT32              *deg_value );

PUBLIC PMC_ERROR lineotn_sm_so_tti_set          ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  lineotn_oh_ins_type_t     ctrl_ins_type,
                                                  UINT32                    tti_data[16] );   

PUBLIC PMC_ERROR lineotn_sm_so_tti_get          ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  UINT32                    tti_data[16] );   
 
PUBLIC PMC_ERROR lineotn_sm_exp_tti_set         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  UINT32                    tti_data[16] );    

PUBLIC PMC_ERROR lineotn_sm_exp_tti_get         ( lineotn_handle_t          *h,
                                                  UINT32                    dsi_stream,
                                                  lineotn_dsi_type_t        dsi_type,
                                                  UINT32                    tti_data[16] );    

PUBLIC PMC_ERROR lineotn_sm_accepted_tti_get    ( lineotn_handle_t   *h,
                                                  UINT32             dsi_stream,
                                                  lineotn_dsi_type_t dsi_type,
                                                  UINT32             tti_data_buf[16] );

PUBLIC PMC_ERROR lineotn_exp_tti_tim_det_mode_set( lineotn_handle_t          *h,
                                                   UINT32                    dsi_stream,
                                                   lineotn_dsi_type_t        dsi_type,
                                                   lineotn_sk_tim_det_mode_t exp_tti_tim_det_mode );

PUBLIC PMC_ERROR lineotn_exp_tti_tim_det_mode_get( lineotn_handle_t          *h,
                                                   UINT32                    dsi_stream,
                                                   lineotn_dsi_type_t        dsi_type,
                                                   lineotn_sk_tim_det_mode_t *exp_tti_tim_det_mode );
                                                   
PUBLIC PMC_ERROR lineotn_ebc_get( lineotn_handle_t          *h,
                                  UINT32                    dsi_stream,
                                  lineotn_dsi_type_t        dsi_type,
                                  lineotn_otu_frm_counters_t *ebc );
                                  
PUBLIC PMC_ERROR lineotn_maintenance_sig_state_get( lineotn_handle_t          *h,
                                                    UINT32                    dsi_stream,
                                                    lineotn_dsi_type_t        dsi_type,
                                                    lineotn_dir_t             maint_sig_dir,
                                                    lineotn_maint_sig_type_t  *maint_sig_type_ptr );

PUBLIC lineotn_deskew_type_t lineotn_deskew_get(lineotn_handle_t *lineotn_handle,
                                                UINT32 dsi_stream);

PUBLIC PMC_ERROR lineotn_ddeg_mode_set (lineotn_handle_t *h,
                                        BOOL8 fw_ddeg);

PUBLIC UINT32 lineotn_frame_rate_otuk_num_get(lineotn_frame_rate_t frame_rate);

/* MLD Block functions */
PUBLIC void mld_reset_config_set ( lineotn_handle_t *lineotn_handle,
                                   BOOL             reset_mld );

PUBLIC PMC_ERROR mld_pwr_mode_config_set( lineotn_handle_t  *lineotn_handle,
                                          mld_hw_module_t    hw_type,
                                          BOOL               low_power );

PUBLIC void mld_blk_init( lineotn_handle_t *lineotn_handle );

PUBLIC PMC_ERROR mld_blk_deprov_stream( lineotn_handle_t   *lineotn_handle,
                                        lineotn_cfg_type_t lineotn_cfg,
                                        lineotn_deskew_type_t deskew_type,
                                        mld_ifc_type_t     interface_type,
                                        UINT8              dsi_stream );

PUBLIC PMC_ERROR mld_blk_prov_stream ( lineotn_handle_t   *lineotn_handle,
                                       lineotn_cfg_type_t lineotn_cfg,
                                       lineotn_deskew_type_t deskew_type,
                                       mld_ifc_type_t     interface_type,
                                       UINT8              dsi_stream );


PUBLIC void mld_opsm23_scramble_set (lineotn_handle_t   *lineotn_handle,
                                     UINT8              opsm_idx,
                                     UINT8              dsi_stream,
                                     BOOL               scramble_in_opsm);

PUBLIC void mld_opsm4_scramble_set (lineotn_handle_t   *lineotn_handle,
                                    BOOL               scramble_in_opsm);

PUBLIC PMC_ERROR mld_sfi_51_dsi_idx_get(lineotn_cfg_type_t lineotn_cfg,
                                        lineotn_dsi_type_t dsi_type,
                                        UINT32             *dsi_start,
                                        UINT32             *dsi_end );
PUBLIC UINT32 mld_opsm_defect_value_get(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream,
                                        UINT32             defect_bit);
PUBLIC PMC_ERROR mld_opsm_ffail_defect_set(lineotn_handle_t   *lineotn_handle,
                                           UINT8              dsi_stream,
                                           UINT32             defect_bit,
                                           BOOL               set_defect_bit);

PUBLIC UINT32 mld_opsm_ffail_defect_get(lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream,
                                        UINT32             defect_bit);


PUBLIC PMC_ERROR mld_opsm_ssf_defect_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         UINT32             defect_bit,
                                         BOOL               set_defect_bit);

PUBLIC UINT32 mld_opsm_ssf_defect_get(lineotn_handle_t   *lineotn_handle,
                                         UINT8           dsi_stream,
                                         UINT32          defect_bit);


PUBLIC PMC_ERROR mld_opsm_loopback_set(lineotn_handle_t   *lineotn_handle,
                                       mld_hw_module_t    opsm,
                                       UINT8              dsi_stream,
                                       lineotn_frame_rate_t frame_rate,
                                       lineotn_dir_t      rx_tx_dir, 
                                       BOOL               loopback_enable);

PUBLIC PMC_ERROR mld_opsm_loopback_get(lineotn_handle_t   *lineotn_handle,
                                       mld_hw_module_t    opsm,
                                       UINT8              dsi_stream,
                                       lineotn_frame_rate_t frame_rate,
                                       lineotn_dir_t      rx_tx_dir, 
                                       BOOL               *loopback_enable);


PUBLIC PMC_ERROR lineotn_sfi51_lane_swap_cfg ( lineotn_handle_t           *h,
                                               lineotn_dsi_type_t         sfi51_ifc, 
                                               lineotn_dir_t              swap_dir,
                                               lineotn_sfi51_swap_state_t lanes_swap_state );

PUBLIC BOOL mld_start_state_test             ( lineotn_handle_t *lineotn_handle );

PUBLIC void mld_init                         ( lineotn_handle_t     *lineotn_handle,
                                               pmc_energy_state_t   energy_state );
 
PUBLIC PMC_ERROR lineotn_bergen_cfg_set          ( lineotn_handle_t            *h,
                                                   UINT8                       dsi_stream,
                                                   UINT32                      logical_lanes_msk,
                                                   FLOAT                       error_rate,
                                                   BOOL8                       ringosc_enable,
                                                   lineotn_bergen_err_mode     bergen_mode,
                                                   BOOL8                        bergen_enable );

PUBLIC PMC_ERROR lineotn_bergen_cfg_get          ( lineotn_handle_t            *h,
                                                   UINT8                       dsi_stream,
                                                   UINT32                      logical_lanes_idx,
                                                   FLOAT                       *error_rate,
                                                   BOOL8                       *ringosc_enable,
                                                   lineotn_bergen_err_mode     *bergen_mode,
                                                   BOOL8                        *bergen_enable );

PUBLIC PMC_ERROR mld_bergen_cfg_set         ( lineotn_handle_t            *lineotn_handle,
                                              UINT8                       dsi_stream,
                                              FLOAT                       error_rate,
                                              BOOL8                       ringosc_enable,
                                              lineotn_bergen_err_mode     bergen_mode,
                                              BOOL8                        bergen_enable );
PUBLIC void mld_bergen_cfg_get              ( lineotn_handle_t            *lineotn_handle,
                                               UINT8                       dsi_stream,
                                               FLOAT                       *error_rate,
                                               BOOL8                       *ringosc_enable,
                                               lineotn_bergen_err_mode     *bergen_mode,
                                               BOOL8                        *bergen_enable );

PUBLIC void mld_otl_logical_lane_id_get(lineotn_handle_t  *lineotn_handle,
                                        UINT8             dsi_stream,
                                        UINT32            logical_lane_id[LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE],
                                        BOOL              get_phy_lane);

PUBLIC PMC_ERROR lineotn_sysotn_mode_db_entry_get(lineotn_handle_t *lineotn_handle,
                                                  UINT32 schd_addr, 
                                                  UINT32 *chnl);

/* fec Block functions */

PUBLIC PMC_ERROR fec_pwr_mode_config_set( lineotn_handle_t  *lineotn_handle,
                                          UINT8             dsi_stream,
                                          lineotn_fec_t     fec_type,
                                          BOOL              low_power );

PUBLIC void fec_blk_init( lineotn_handle_t *lineotn_handle );

PUBLIC PMC_ERROR fec_blk_deprov_stream( lineotn_handle_t   *lineotn_handle,
                                        UINT8              dsi_stream );

PUBLIC PMC_ERROR fec_blk_prov_stream ( lineotn_handle_t   *lineotn_handle,
                                       UINT8              dsi_stream,
                                       lineotn_fec_t      fec_type );

PUBLIC PMC_ERROR fec_mfec_cfg_set(lineotn_handle_t   *lineotn_handle,
                                   UINT8              dsi_stream,
                                   lineotn_fec_t      fec_type);


PUBLIC PMC_ERROR fec_mfec_bypass_set( lineotn_handle_t   *lineotn_handle,
                                      UINT8              dsi_stream,
                                      BOOL               bypass_enc_dec );
                                      
PUBLIC PMC_ERROR fec_mfec_dec_bypass_set(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         BOOL               bypass_dec); 

PUBLIC PMC_ERROR fec_mfec_dec_bypass_get(lineotn_handle_t   *lineotn_handle,
                                         UINT8              dsi_stream,
                                         BOOL               *bypass_dec);                                                                              

PUBLIC PMC_ERROR fec_dec_passthru_set(lineotn_handle_t     *lineotn_handle,
                                      UINT8                dsi_stream,
                                      lineotn_fec_t        fec_type,
                                      BOOL                 dec_passthru );


PUBLIC BOOL fec_start_state_test     ( lineotn_handle_t *lineotn_handle ); 

PUBLIC void fec_init ( lineotn_handle_t     *lineotn_handle,
                       pmc_energy_state_t   energy_state);

PUBLIC PMC_ERROR fec_stat_intf_init(lineotn_handle_t      *h,
                                    digi_fec_stat_intf_cfg_t *fec_stat_intf_cfg_ptr);

PUBLIC PMC_ERROR fec_stat_intf_uninit(lineotn_handle_t *h);

PUBLIC BOOL8 fec_stat_enabled(lineotn_handle_t *h);

PUBLIC PMC_ERROR fec_stat_validate_stream(lineotn_handle_t *h,
                                          UINT32 dsi_stream);

PUBLIC PMC_ERROR fec_stat_set(lineotn_handle_t *h,
                              UINT32 dsi_stream, 
                              UINT32 logical_lane_identifier[LINEOTN_MLD_OTL410_NUM_LOGICAL_LANE],
                              BOOL8 enable);


PUBLIC PMC_ERROR lineotn_dsi_rate_get (lineotn_handle_t *lineotn_handle,
                                       UINT32 dsi_stream,
                                       lineotn_frame_rate_t *dsi_rate);

PUBLIC PMC_ERROR lineotn_int_enable(lineotn_handle_t   *lineotn_handle,
                                    lineotn_int_t *int_table_ptr,
                                    lineotn_int_t *int_en_table_ptr,
                                    BOOL                enable);

PUBLIC PMC_ERROR lineotn_int_clear(lineotn_handle_t  *lineotn_handle,
                                   lineotn_int_t *int_table_ptr,
                                   lineotn_int_t *int_en_table_ptr);

PUBLIC PMC_ERROR lineotn_int_retrieve(lineotn_handle_t    *lineotn_handle,
                                      lineotn_int_t  *filt_table_ptr,
                                      lineotn_int_t  *int_table_ptr);

PUBLIC PMC_ERROR lineotn_int_enabled_check(lineotn_handle_t    *lineotn_handle,
                                           lineotn_int_t *int_en_table_ptr,
                                           BOOL               *int_found_ptr);

PUBLIC PMC_ERROR lineotn_int_chnl_enable(lineotn_handle_t   *lineotn_handle,
                                         UINT32              chnl_id,
                                         lineotn_int_chnl_t *int_table_ptr,
                                         lineotn_int_chnl_t *int_en_table_ptr,
                                         BOOL                enable);

PUBLIC PMC_ERROR lineotn_int_chnl_clear(lineotn_handle_t  *lineotn_handle,
                                        UINT32              chnl_id,
                                        lineotn_int_chnl_t *int_en_table_ptr,
                                        lineotn_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR lineotn_int_chnl_retrieve(lineotn_handle_t    *lineotn_handle,
                                           UINT32               chnl_id,
                                           lineotn_int_chnl_t  *filt_table_ptr,
                                           lineotn_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR lineotn_int_chnl_enabled_check(lineotn_handle_t    *lineotn_handle,
                                                UINT32              chnl_id,
                                                lineotn_int_chnl_t *int_en_table_ptr,
                                                BOOL               *int_found_ptr);
PUBLIC PMC_ERROR lineotn_lof_mon_fw_cfg       ( lineotn_handle_t          *h,
                                                UINT32                    dsi_stream,
                                                lineotn_dsi_type_t        dsi_type,
                                                BOOL                      prov);

PUBLIC void lineotn_alloc_init(lineotn_handle_t    *lineotn_handle);

PUBLIC PMC_ERROR lineotn_handle_restart_init(lineotn_handle_t    *lineotn_handle,
                                             BOOL8                is_lineotn_ss,
                                             util_global_restart_init_cfg_t                *restart_init_cfg_ptr, 
                                             pmc_energy_state_t   top_energy_state_reg);

PUBLIC BOOL8 lineotn_prov_get(lineotn_handle_t    *lineotn_handle,
                              UINT32               dsi_stream,
                              BOOL8                rx_chnl);


PUBLIC PMC_ERROR lineotn_adjust_dci_clock(lineotn_handle_t *lineotn_handle,
                                          UINT32 num_mult,
                                          UINT32 denom_mult);


PUBLIC PMC_ERROR lineotn_otu_frm_dintlv_xoff_thres_set(lineotn_handle_t *h,
                                                       UINT32           dci_channel,
                                                       UINT32           dintlv_xoff);

PUBLIC void fec_gfec_lowpwr_set( lineotn_handle_t  *lineotn_handle,
                                 BOOL8              low_power);

PUBLIC void fec_all_swizzle_fec_lowpwr_set( lineotn_handle_t  *lineotn_handle,
                                            BOOL8              low_power);

PUBLIC PMC_ERROR lineotn_mld_sfis_odd_parity_set(lineotn_handle_t            *lineotn_handle,
                                                 UINT8                       dsi_stream,
                                                 util_global_direction_t     dir,
                                                 BOOL8                       inv);

PUBLIC PMC_ERROR lineotn_mld_sfis_odd_parity_get(lineotn_handle_t            *lineotn_handle,
                                                 UINT8                       dsi_stream,
                                                 util_global_direction_t     dir,
                                                 BOOL8                       *inv_ptr);
               

PUBLIC void lineotn_top_level_interrupt_set(lineotn_handle_t *lineotn_handle,
                                            BOOL8 enable);

PUBLIC void lineotn_interrupt_type_save(lineotn_handle_t    *handle, 
                                        sysotn_intr_type_t  type);
                                        
PUBLIC void lineotn_top_level_interrupt_recover(lineotn_handle_t *handle);

PUBLIC PMC_ERROR lineotn_mld_dlom_to_ffail_mapping_set(lineotn_handle_t    *lineotn_handle,
                                              UINT32 dsi_stream,
                                               BOOL8 enable);

#ifdef __cplusplus
}
#endif

#endif /* _LINEOTN_H */

/*
** end of file
*/
