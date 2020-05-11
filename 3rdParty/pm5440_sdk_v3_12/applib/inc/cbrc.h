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

#ifndef _CBRC_H
#define _CBRC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "cbrc_api.h"
#include "scbs3.h"
#include "sdh_pmg.h"
#include "cbr_fsgm.h"
#include "fc1200_pmon.h"
#include "fc800_pmg.h"
#include "util_global_api.h"

/*
** Enumerated Types
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* ENUM: cbrc_port_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the client traffic modes that can be provisioned
*   through the CBRC. This is used with cbrc_port_provision().
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBRC_PORT_MODE_NONE = 0,

    CBRC_PORT_MODE_40G_STS768,
    CBRC_PORT_MODE_40G_STM256,
   
    CBRC_PORT_MODE_10G_STS192,
    CBRC_PORT_MODE_10G_STM64,
    CBRC_PORT_MODE_10G_FC1200_ODU1F,
    CBRC_PORT_MODE_10G_FC1200_ODU2F,
    CBRC_PORT_MODE_10G_FC1200_GFP_ODU2E,
    CBRC_PORT_MODE_10G_FC800,
    CBRC_PORT_MODE_10G_CPRI_5G,
    CBRC_PORT_MODE_10G_CPRI_6G,
    CBRC_PORT_MODE_10G_CPRI_10G,
    CBRC_PORT_MODE_10G_GDPS_5G,
    CBRC_PORT_MODE_10G_GDPS_10G,
    CBRC_PORT_MODE_10G_STS192_PRBS,
    CBRC_PORT_MODE_10G_STM64_PRBS,
    CBRC_PORT_MODE_10G_10GE,
    CBRC_PORT_MODE_10G_8B_10B_IDLE,
    CBRC_PORT_MODE_10G_64B_66B_IDLE,
    CBRC_PORT_MODE_10G_8B_10B_ARB,
    
    LAST_CBRC_PORT_MODE,
} cbrc_port_mode_t;

/*******************************************************************************
* ENUM: mld_prbs_rx_mux_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects either line side or system side traffic as the source to the
*   receive MLD. This is used with mld_prbs_rx_mux_cfg().
*
* ELEMENTS:
*    MLD_PRBS_RX_MUX_LINE   - Selects the line side interface for ingress
*                             traffic
*    MLD_PRBS_RX_MUX_SYSTEM - Selects the system side interface for ingress
*                             traffic
*    LAST_MLD_PRBS_RX_MUX   - For out of range error handling.
*
*******************************************************************************/
typedef enum {
    MLD_PRBS_RX_MUX_LINE   = 0,
    MLD_PRBS_RX_MUX_SYSTEM = 1,

    LAST_MLD_PRBS_RX_MUX,
} mld_prbs_rx_mux_t;

/*******************************************************************************
* ENUM: mld_prbs_tx_mux_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Selects either PRBS or FC_DLVR traffic as the source to the
*   transmit MLD.
*   
*
* ELEMENTS:
*    MLD_PRBS_TX_MUX_FC_DLRV - Selects the FC_DLVR for egress traffic
*    MLD_PRBS_TX_MUX_PRBS    - Selects the PRBS for egress traffic
*    LAST_MLD_PRBS_TX_MUX   - For out of range error handling.
*
*******************************************************************************/
typedef enum {
    MLD_PRBS_TX_MUX_FC_DLRV = 0,
    MLD_PRBS_TX_MUX_PRBS    = 1,

    LAST_MLD_PRBS_TX_MUX,
} mld_prbs_tx_mux_t;

/*******************************************************************************
* ENUM: cbrc_serdes_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the serdes used. This is used with
*   cbrc_port_provision().
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBRC_SERDES_TYPE_NONE = 0,
    CBRC_SERDES_TYPE_10G_XFI,
    CBRC_SERDES_TYPE_40G_STL,
    CBRC_SERDES_TYPE_40G_SFIS,
    CBRC_SERDES_TYPE_40G_SFI51,
    
    LAST_CBRC_SERDES_TYPE,
} cbrc_serdes_type_t;

/*******************************************************************************
* ENUM: cbrc_feature_mode_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration defining the distinct datapath modes that features can be applied.
*
* ELEMENTS:
*    CBRC_FEATURE_MODE_SDH          - 10G/40G SONET/SDH datapath
*    CBRC_FEATURE_MODE_CBR          - CBR datapath (FC800, CPRI, or GDPS)
*    CBRC_FEATURE_MODE_CPRI         - CPRI datapath
*    CBRC_FEATURE_MODE_GDPS         - GDPS datapath
*    CBRC_FEATURE_MODE_FC800        - FC800 datapath
*    CBRC_FEATURE_MODE_FC1200       - FC1200 ODU1F/2F datapath
*    CBRC_FEATURE_MODE_FC1200_GFP   - FC1200 ODU2e GFP datapath
*
*******************************************************************************/
typedef enum {
    CBRC_FEATURE_MODE_SDH = 0,
    CBRC_FEATURE_MODE_CBR,
    CBRC_FEATURE_MODE_CPRI,
    CBRC_FEATURE_MODE_GDPS,
    CBRC_FEATURE_MODE_FC800,
    CBRC_FEATURE_MODE_FC1200,
    CBRC_FEATURE_MODE_FC1200_GFP,

    LAST_CBRC_FEATURE_MODE, 
} cbrc_feature_mode_t;

/*******************************************************************************
* ENUM: cbrc_port_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unused.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBRC_PORT_STATE_START = 0,
    CBRC_PORT_STATE_EQUIPPED,
    CBRC_PORT_STATE_OPERATIONAL,

    LAST_CBRC_PORT_STATE,
} cbrc_port_state_t;
#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Constants
*/
#define CBRC_MAX_LINK                               CBRC_MAX_CHANNELS

#define CBRC_RESET                                  0x1
#define CBRC_ACTIVE                                 0x0

#define CBRC_ENABLE                                 0x1
#define CBRC_DISABLE                                0x0

#define CBRC_MLD_RX_STL                             0x0
#define CBRC_MLD_RX_SFIS                            0x1
#define CBRC_MLD_RX_SFI51                           0x2
#define CBRC_MLD_RX_XFI                             0x3
#define CBRC_MLD_TX_STL                             0x0
#define CBRC_MLD_TX_SFIS                            0x1
#define CBRC_MLD_TX_XFI                             0x3

#define CBRC_PKT_DINT_FIFO_THRESH_FC800             0x11
#define CBRC_PKT_DINT_FIFO_THRESH_FC800_NOS         0x18
#define CBRC_PKT_DINT_FIFO_THRESH_FC1200            0x16
#define CBRC_PKT_DINT_FIFO_THRESH_CPRI_5G           0xA
#define CBRC_PKT_DINT_FIFO_THRESH_CPRI_6G           0xD
#define CBRC_PKT_DINT_FIFO_THRESH_CPRI_10G          0x12
#define CBRC_PKT_DINT_FIFO_THRESH_GDPS_5G           0xA
#define CBRC_PKT_DINT_FIFO_THRESH_GDPS_10G          0x12
#define CBRC_PKT_DINT_FIFO_THRESH_STS192            0x16            
#define CBRC_PKT_DINT_FIFO_THRESH_STS768            0xF

#define CBRC_MLD_PRBS_TX_FIFO_THRESH_FC800          0x8
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_FC1200         0x8
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_CPRI_5G        0x8
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_CPRI_6G        0x7
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_CPRI_10G       0x7
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_GDPS_5G        0x8
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_GDPS_10G       0x8
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_STS192         0x8
#define CBRC_MLD_PRBS_TX_FIFO_THRESH_STS768         0x8

#define CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_STL           0x0
#define CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_SFIS          0x1
#define CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_SFI51         0x2
#define CBRC_MLD_PRBS_XOFF_TX_MUX_SEL_XFI           0x3

#define CBRC_SFI51_DSC_FIFO_HI_RD_THRSH             0xB
#define CBRC_FC_ILVR_48B_ENABLE_BIT                 0x7

#define CBRC_PKT_DINT_CFC_XOFF_THRESHOLD            0x8
   #define CBRC_PKT_DINT_UF_ZONE_MODE               0x1
   #define CBRC_PKT_DINT_ZONE_XOFF_SET_THRESHOLD    0x1
   #define CBRC_PKT_DINT_ZONE_XOFF_CLEAR_THRESHOLD  0x2

#define CBRC_MLD_STL256_AF_THRESH                   0x16

#define CBRC_FC_TTTM_OSIR_NUM                       5115619
#define CBRC_FC_TTTM_OSIR_DENOM                     7999993


#ifndef DOXYGEN_PUBLIC_ONLY 
#define     CBRC_NUM_SCHD           2 

/* CBRC_STS1_NUM_PATHS_MAX: maximum numner of STS-1 paths */
#define     CBRC_STS1_NUM_PATHS_MAX 16

#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Macro Definitions
*/

/* CBRC_INT(handle, tsb_name, tsb_field, struct_field) */
/* CBRC_INT_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) */

#define CBRC_INT_TABLE_DEFINE() \
    CBRC_INT_RANGE(cbrc_handle, cbr_dint, field_range, FIFO_UF_INT     , cbr_dint_fifo_uf_int_i     ); \
    CBRC_INT_RANGE(cbrc_handle, cbr_dint, field_range, FIFO_OVF_INT    , cbr_dint_fifo_ovf_int_i    ); \
    CBRC_INT_RANGE(cbrc_handle, cbr_dint, field_range, STAG_BUF_OVF_INT, cbr_dint_stag_buf_ovf_int_i); \
    CBRC_INT_RANGE(cbrc_handle, pkt_dint, field_range, CFC_FIFO_OVR    , pkt_dint_cfc_fifo_ovr_i    ); \
    CBRC_INT_RANGE(cbrc_handle, pkt_dint, field_range, CFC_FIFO_UDR    , pkt_dint_cfc_fifo_udr_i    ); \
    CBRC_INT_RANGE(cbrc_handle, pkt_int,  field_range, CFC_FIFO_OVR    , pkt_int_cfc_fifo_ovr_i     ); \
    CBRC_INT_RANGE(cbrc_handle, pkt_int,  field_range, CFC_FIFO_UDR    , pkt_int_cfc_fifo_udr_i     ); \
    CBRC_INT(cbrc_handle, fc1200_pcs_tx, LOCAL_FAULT , fc1200_pcs_tx_local_fault_i ); \
    CBRC_INT(cbrc_handle, fc1200_pcs_tx, BUF_UNDERRUN, fc1200_pcs_tx_buf_underrun_i); \
    CBRC_INT(cbrc_handle, fc_tttd,       CRC24ERR    , fc_tttd_crc24err_i          ); \
    CBRC_INT(cbrc_handle, fc_tttd,       UNDERRUN    , fc_tttd_underrun_i          ); \
    CBRC_INT(cbrc_handle, fc_tttd,       INVALBLKHDR , fc_tttd_invalblkhdr_i       ); \
    CBRC_INT(cbrc_handle, fc_tttm,       LINK_FAIL   , fc_tttm_link_fail_i         ); \
    CBRC_INT(cbrc_handle, fc_tttm,       HIBER       , fc_tttm_hiber_i             ); \
    CBRC_INT(cbrc_handle, fc_tttm,       LOSYNC      , fc_tttm_losync_i            ); \
    CBRC_INT(cbrc_handle, fc_tttm,       BADBLK      , fc_tttm_badblk_i            ); \
    CBRC_INT(cbrc_handle, fc_tttm,       FOVR        , fc_tttm_fovr_i              ); \
    CBRC_INT(cbrc_handle, fc_tttm,       DPI         , fc_tttm_dpi_i               ); \
    CBRC_INT_FIELD(cbrc_handle,   mldprbs_pcbi  , TX_XFI_FIFO_,    UNFL_INT , tx_xfi_fifo_unfl_int_i ); \
    CBRC_INT_FIELD(cbrc_handle,   mldprbs_pcbi  , TX_XFI_FIFO_,    OVFL_INT , tx_xfi_fifo_ovfl_int_i ); \
    CBRC_INT_FIELD_ARRAY(cbrc_handle,   prgm,  16  , MON,    ERR  , prgm_mon_err_i  ); \
    CBRC_INT_FIELD_ARRAY(cbrc_handle,   prgm,  16  , MON,    SYNC , prgm_mon_sync_i ); \
    CBRC_INT_IND(cbrc_handle, shpi_indirect,  PAISCI    , shpi_paisci  ); \
    CBRC_INT_IND(cbrc_handle, shpi_indirect,  PLOPCI    , shpi_plopci  ); \
    CBRC_INT_IND(cbrc_handle, shpi_indirect,  PAISI     , shpi_paisi   ); \
    CBRC_INT_IND(cbrc_handle, shpi_indirect,  PLOPI     , shpi_plopi   ); \

#define CBRC_STATUS_TABLE_DEFINE() \
    CBRC_STATUS_RANGE(cbrc_handle, cbr_dint, field_range, FIFO_UF_INT_V     , cbr_dint_fifo_uf_int_v     ); \
    CBRC_STATUS_RANGE(cbrc_handle, cbr_dint, field_range, FIFO_OVF_INT_V    , cbr_dint_fifo_ovf_int_v    ); \
    CBRC_STATUS_RANGE(cbrc_handle, cbr_dint, field_range, STAG_BUF_OVF_INT_V, cbr_dint_stag_buf_ovf_int_v); \
    CBRC_STATUS_RANGE(cbrc_handle, pkt_dint, field_range, CFC_FIFO_OVR_V    , pkt_dint_cfc_fifo_ovr_v    ); \
    CBRC_STATUS_RANGE(cbrc_handle, pkt_dint, field_range, CFC_FIFO_UDR_V    , pkt_dint_cfc_fifo_udr_v    ); \
    CBRC_STATUS_RANGE(cbrc_handle, pkt_int,  field_range, CFC_FIFO_OVR_V    , pkt_int_cfc_fifo_ovr_v     ); \
    CBRC_STATUS_RANGE(cbrc_handle, pkt_int,  field_range, CFC_FIFO_UDR_V    , pkt_int_cfc_fifo_udr_v     ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_rx, LOS_V         , fc1200_pcs_rx_los_v         ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_rx, LOSS_SYNC_V   , fc1200_pcs_rx_loss_sync_v   ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_rx, HIGH_BER_V    , fc1200_pcs_rx_high_ber_v    ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_rx, LF_V          , fc1200_pcs_rx_lf_v          ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_rx, RF_V          , fc1200_pcs_rx_rf_v          ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_tx, LOCAL_FAULT_V , fc1200_pcs_tx_local_fault_v ); \
    CBRC_STATUS(cbrc_handle, fc1200_pcs_tx, BUF_UNDERRUN_V, fc1200_pcs_tx_buf_underrun_v); \
    CBRC_STATUS(cbrc_handle, fc_tttm,       LINK_FAIL_V   , fc_tttm_link_fail_v         ); \
    CBRC_STATUS(cbrc_handle, fc_tttm,       HIBER_V       , fc_tttm_hiber_v             ); \
    CBRC_STATUS(cbrc_handle, fc_tttm,       LOSYNC_V      , fc_tttm_losync_v            ); \
    CBRC_STATUS_FIELD_ARRAY(cbrc_handle,   prgm,  12  , MON,    SYNC_V , prgm_mon_sync_v ); \
    CBRC_STATUS_IND(cbrc_handle, shpi_indirect,  PAISCV    , shpi_paiscv  ); \
    CBRC_STATUS_IND(cbrc_handle, shpi_indirect,  PLOPCV    , shpi_plopcv  ); \
    CBRC_STATUS_IND(cbrc_handle, shpi_indirect,  PAISV     , shpi_paisv   ); \
    CBRC_STATUS_IND(cbrc_handle, shpi_indirect,  PLOPV     , shpi_plopv   ); \

#define CBRC_INT_TABLE_DEFINE_FW() \
    CBRC_INT(cbrc_handle, fc1200_pcs_rx, LOS         , fc1200_pcs_rx_los_i         ); \
    CBRC_INT(cbrc_handle, fc1200_pcs_rx, LOSS_SYNC   , fc1200_pcs_rx_loss_sync_i   ); \
    CBRC_INT(cbrc_handle, fc1200_pcs_rx, HIGH_BER    , fc1200_pcs_rx_high_ber_i    ); \
    CBRC_INT(cbrc_handle, fc1200_pcs_rx, LF          , fc1200_pcs_rx_lf_i          ); \
    CBRC_INT(cbrc_handle, fc1200_pcs_rx, RF          , fc1200_pcs_rx_rf_i          ); \

/* CBRC_INT(handle, tsb_name, tsb_field, struct_field) */
/* CBRC_INT_RANGE(handle, tsb_name, field_type, tsb_field, struct_field) */

#define CBRC_STL256_INT_TABLE_DEFINE() \
    CBRC_INT(cbrc_handle, stl256,    DAIS                   , dais_i                   ); \
    CBRC_INT(cbrc_handle, stl256,    OOF                    , oof_i                    ); \
    CBRC_INT(cbrc_handle, stl256,    LANE_FIFO_OVR          , lane_fifo_ovr_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE0_OOF_STL          , lane0_oof_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE1_OOF_STL          , lane1_oof_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE2_OOF_STL          , lane2_oof_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE3_OOF_STL          , lane3_oof_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE0_DLOF_STL         , lane0_dlof_stl_i         ); \
    CBRC_INT(cbrc_handle, stl256,    LANE1_DLOF_STL         , lane1_dlof_stl_i         ); \
    CBRC_INT(cbrc_handle, stl256,    LANE2_DLOF_STL         , lane2_dlof_stl_i         ); \
    CBRC_INT(cbrc_handle, stl256,    LANE3_DLOF_STL         , lane3_dlof_stl_i         ); \
    CBRC_INT(cbrc_handle, stl256,    DSTL_AIS               , dstl_ais_i               ); \
    CBRC_INT(cbrc_handle, stl256,    DLOL                   , dlol_i                   ); \
    CBRC_INT(cbrc_handle, stl256,    ATSF                   , atsf_i                   ); \
    CBRC_INT(cbrc_handle, stl256,    LANE0_DLOS             , lane0_dlos_i             ); \
    CBRC_INT(cbrc_handle, stl256,    LANE1_DLOS             , lane1_dlos_i             ); \
    CBRC_INT(cbrc_handle, stl256,    LANE2_DLOS             , lane2_dlos_i             ); \
    CBRC_INT(cbrc_handle, stl256,    LANE3_DLOS             , lane3_dlos_i             ); \
    CBRC_INT(cbrc_handle, stl256,    LANE0_OOR_STL          , lane0_oor_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE1_OOR_STL          , lane1_oor_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE2_OOR_STL          , lane2_oor_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    LANE3_OOR_STL          , lane3_oor_stl_i          ); \
    CBRC_INT(cbrc_handle, stl256,    MARKERS_NOT_UNIQUE     , markers_not_unique_i     ); \
    CBRC_INT(cbrc_handle, stl256,    LANE_ID_NOT_CONSISTENT , lane_id_not_consistent_i ); \
    CBRC_INT(cbrc_handle, stl256,    EXCESSIVE_SKEW         , excessive_skew_i         ); \

#define CBRC_STL256_STATUS_TABLE_DEFINE() \
    CBRC_STATUS(cbrc_handle, stl256,    DAIS_V                   , dais_v                   ); \
    CBRC_STATUS(cbrc_handle, stl256,    OOF_V                    , oof_v                    ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE0_OOF_STL_V          , lane0_oof_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE1_OOF_STL_V          , lane1_oof_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE2_OOF_STL_V          , lane2_oof_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE3_OOF_STL_V          , lane3_oof_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE0_DLOF_STL_V         , lane0_dlof_stl_v         ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE1_DLOF_STL_V         , lane1_dlof_stl_v         ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE2_DLOF_STL_V         , lane2_dlof_stl_v         ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE3_DLOF_STL_V         , lane3_dlof_stl_v         ); \
    CBRC_STATUS(cbrc_handle, stl256,    DSTL_AIS_V               , dstl_ais_v               ); \
    CBRC_STATUS(cbrc_handle, stl256,    DLOL_V                   , dlol_v                   ); \
    CBRC_STATUS(cbrc_handle, stl256,    ATSF_V                   , atsf_v                   ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE0_DLOS_V             , lane0_dlos_v             ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE1_DLOS_V             , lane1_dlos_v             ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE2_DLOS_V             , lane2_dlos_v             ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE3_DLOS_V             , lane3_dlos_v             ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE0_OOR_STL_V          , lane0_oor_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE1_OOR_STL_V          , lane1_oor_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE2_OOR_STL_V          , lane2_oor_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE3_OOR_STL_V          , lane3_oor_stl_v          ); \
    CBRC_STATUS(cbrc_handle, stl256,    MARKERS_NOT_UNIQUE_V     , markers_not_unique_v     ); \
    CBRC_STATUS(cbrc_handle, stl256,    LANE_ID_NOT_CONSISTENT_V , lane_id_not_consistent_v ); \
    CBRC_STATUS(cbrc_handle, stl256,    EXCESSIVE_SKEW_V         , excessive_skew_v         ); \

/* SFIS_RX_INT(handle, tsb_name, tsb_field, struct_field) */
#define CBRC_SFIS_INT_TABLE_DEFINE() \
    CBRC_INT_ARRAY(cbrc_handle, cbrc_sfis_rx, RXLOS    , rxlos_i          ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXLOS_DSC      , rxlos_dsc_i      ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOOF          , rxoof_i          ); \
    CBRC_INT_ARRAY(cbrc_handle, cbrc_sfis_rx, RXOOA    , rxooa_i          ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXUDR          , rxudr_i          ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXUDR_DSC      , rxudr_dsc_i      ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR0         , rxovr0_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR1         , rxovr1_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR2         , rxovr2_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR3         , rxovr3_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR4         , rxovr4_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR5         , rxovr5_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR6         , rxovr6_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR7         , rxovr7_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR8         , rxovr8_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR9         , rxovr9_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RXOVR_DSC      , rxovr_dsc_i      ); \
    CBRC_INT_ARRAY(cbrc_handle, cbrc_sfis_rx, RX_BITERR, rx_biterr_i      ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_rx, RX_BITERR_DSC  , rx_biterr_dsc_i  ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_tx, TXOVR          , txovr_i          ); \
    CBRC_INT(cbrc_handle, cbrc_sfis_tx, TXUDR          , txudr_i          ); \

/* Status Table Macro */
#define CBRC_SFIS_STATUS_TABLE_DEFINE() \
    CBRC_STATUS_ARRAY(cbrc_handle, cbrc_sfis_rx, RXLOS_V   , rxlos_v      ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfis_rx, RXLOS_DSC_V     , rxlos_dsc_v  ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfis_rx, RXOOF_V         , rxoof_v      ); \
    CBRC_STATUS_ARRAY(cbrc_handle, cbrc_sfis_rx, RXOOA_V   , rxooa_v      ); \

/* SFI51_INT(handle, tsb_name, tsb_field, struct_field) */
#define CBRC_SFI51_INT_TABLE_DEFINE(sysotn) \
    CBRC_INT(cbrc_handle, cbrc_sfi51, RX_DESKEW_LOS         , rx_deskew_los_i        ); \
    CBRC_INT(cbrc_handle, cbrc_sfi51, RX_DESKEW_OOF         , rx_deskew_oof_i        ); \
    CBRC_INT(cbrc_handle, cbrc_sfi51, RX_DESKEW_LOF         , rx_deskew_lof_i        ); \
    CBRC_INT(cbrc_handle, cbrc_sfi51, RX_DESKEW_FIFO_UNFL   , rx_deskew_fifo_unfl_i  ); \
    CBRC_INT(cbrc_handle, cbrc_sfi51, RX_DESKEW_FIFO_OVFL   , rx_deskew_fifo_ovfl_i  ); \
    CBRC_INT_ARRAY(cbrc_handle, cbrc_sfi51, RX_DESKEW_OOA   , rx_deskew_ooa_i        ); \
    CBRC_INT_ARRAY(cbrc_handle, cbrc_sfi51, RX_CHN_BIT_ERR  , rx_chn_bit_err_i       ); \
    CBRC_INT(cbrc_handle, cbrc_sfi51, TX_FIFO_UNFL          , tx_fifo_unfl_i         ); \
    CBRC_INT(cbrc_handle, cbrc_sfi51, TX_FIFO_OVFL          , tx_fifo_ovfl_i         ); \

/* Status Table Macro */
#define CBRC_SFI51_STATUS_TABLE_DEFINE(sysotn) \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, RX_DESKEW_LOS_V         , rx_deskew_los_v        ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, RX_DESKEW_OOF_V         , rx_deskew_oof_v        ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, RX_DESKEW_LOF_V         , rx_deskew_lof_v        ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, RX_DESKEW_FIFO_UNFL_V   , rx_deskew_fifo_unfl_v  ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, RX_DESKEW_FIFO_OVFL_V   , rx_deskew_fifo_ovfl_v  ); \
    CBRC_STATUS_ARRAY(cbrc_handle, cbrc_sfi51, RX_DESKEW_OOA_V   , rx_deskew_ooa_v        ); \
    CBRC_STATUS_ARRAY(cbrc_handle, cbrc_sfi51, RX_CHN_BIT_ERR_V  , rx_chn_bit_err_v       ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, TX_FIFO_UNFL_V          , tx_fifo_unfl_v         ); \
    CBRC_STATUS(cbrc_handle, cbrc_sfi51, TX_FIFO_OVFL_V          , tx_fifo_ovfl_v         ); \




/*
** Structures and Unions
*/

/*******************************************************************************
* ENUM: cbrc_schd_populate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the CBRC scheduler 
*    content source generation mode.
*
* ELEMENTS:
*           CBRC_SCHD_POPULATE_LOCAL  - Use local scheduler content generation
*                                       method
*           CBRC_SCHD_POPULATE_DB     - Use database contents to generate
*                                       scheduler contents
*
*******************************************************************************/
typedef enum
{
    CBRC_SCHD_POPULATE_LOCAL      = 0,
    CBRC_SCHD_POPULATE_DB,

    LAST_CBRC_SCHD_POPULATE
} cbrc_schd_populate_t;



/*******************************************************************************
* ENUM: cbrc_db_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of database IDs to choose the SCBS3 database that needs to 
*    be programmed with cbrc_schd_populate_t.
*
* ELEMENTS:
*    CBRC_DB_ID_PKT_INT_MPMA - Program the SCBS3 in the ingress PKT_INT MPMA
*    CBRC_DB_ID_MPMO         - Program the SCBS3 in the egress MPMO
*
*******************************************************************************/
typedef enum
{
    CBRC_DB_ID_PKT_INT_MPMA = 0,
    CBRC_DB_ID_MPMO,

    LAST_CBRC_DB_ID,
} cbrc_db_id_t;


/*******************************************************************************
* ENUM: cbrc_loopback_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of loopback types
*
* ELEMENTS:
*    CBRC_FACILITY_LOOPBACK     - Facility loopback 
*    CBRC_DIAG_LOOPBACK         - Diagnostic loopback 
*    CBRC_DISABLE_LOOPBACK      - Disable loopback
*
*******************************************************************************/
typedef enum
{
    CBRC_DISABLE_LOOPBACK = 0, 
    CBRC_FACILITY_LOOPBACK,     
    CBRC_DIAG_LOOPBACK,   
} cbrc_loopback_t;



/*******************************************************************************
* STRUCTURE: cbrc_handle_init_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for configuring configuring the SCBS3's calendar mode within the CBRC
*
* ELEMENTS:
*   schd_populate_mode - array that stores the access mode
*                        0: Ingress path PKT INT SCBS3
*                        1: Egress path MPMO SCBS3
*
*
*
*******************************************************************************/
typedef struct
{
    cbrc_schd_populate_t schd_populate_mode[CBRC_NUM_SCHD];
} cbrc_handle_init_cfg_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC PMC_ERROR cbrc_init(cbrc_handle_t       *cbrc_handle_ptr,
                           pmc_energy_state_t   energy_state);

PUBLIC BOOL8 cbrc_start_state_test(cbrc_handle_t *cbrc_handle);

PUBLIC cbrc_handle_t *cbrc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);

PUBLIC void cbrc_ctxt_destroy(cbrc_handle_t *cbrc_handle_ptr);

PUBLIC void cbrc_handle_init(cbrc_handle_t          *cbrc_handle_ptr,
                             cbrc_handle_init_cfg_t *cbrc_handle_init_cfg_ptr);

PUBLIC void cbrc_alloc_init(cbrc_handle_t *cbrc_handle_ptr);

PUBLIC PMC_ERROR cbrc_handle_restart_init(cbrc_handle_t     *cbrc_handle_ptr,
                                          util_global_restart_init_cfg_t              *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state);



PUBLIC PMC_ERROR cbrc_db_entry_set(cbrc_handle_t *cbrc_handle_ptr,
                                   cbrc_db_id_t dcs_instance,
                                   UINT32 schd_addr,
                                   UINT32 dcs_schd_channel_id );

PUBLIC PMC_ERROR cbrc_db_entry_clear(cbrc_handle_t *cbrc_handle_ptr,
                                     cbrc_db_id_t dcs_instance,
                                     UINT32 dcs_schd_channel_id );

PUBLIC PMC_ERROR cbrc_db_entry_get(cbrc_handle_t *cbrc_handle_ptr,
                                   cbrc_db_id_t dcs_instance,
                                   UINT32 schd_addr,
                                   UINT32 *schd_channel_id_ptr) ;

PUBLIC PMC_ERROR cbrc_db_entry_all_get(cbrc_handle_t *cbrc_handle_ptr,
                                       cbrc_db_id_t   cbrc_db_id,
                                       UINT32         schd_channel_id_ptr[CALENDAR_ENTRIES_NUM]);


PUBLIC BOOL8 cbrc_serdes_type_is_40g(cbrc_serdes_type_t   serdes_type);
PUBLIC BOOL8 cbrc_port_mode_is_fc1200(cbrc_port_mode_t  port_mode);
PUBLIC BOOL8 cbrc_port_mode_is_fc1200_gfp(cbrc_port_mode_t  port_mode);



PUBLIC PMC_ERROR cbrc_port_provision(cbrc_handle_t      *cbrc_handle_ptr,
                                     UINT32              link,
                                     cbrc_serdes_type_t  serdes_type,
                                     cbrc_port_mode_t    mode);



PUBLIC PMC_ERROR cbrc_port_unprovision(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link);




PUBLIC PMC_ERROR cbrc_port_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                    cbrc_port_dir_t   dir,
                                    UINT32            link);



PUBLIC PMC_ERROR cbrc_port_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                      cbrc_port_dir_t   dir,
                                      UINT32            link);


PUBLIC DOUBLE cbrc_client_rate_get(cbrc_handle_t    *cbrc_handle_ptr,
                                   cbrc_port_mode_t  mode);

PUBLIC PMC_ERROR cbrc_mpmo_cfg_get(cbrc_handle_t    *cbrc_hndl,
                                   UINT32           link,
                                   BOOL8             *mpmo_cfg_req);

PUBLIC PMC_ERROR cbrc_mld_prbs_prov(cbrc_handle_t    *cbrc_handle_ptr,
                                    UINT32            link);
PUBLIC PMC_ERROR cbrc_mpma_data_get(cbrc_handle_t   *cbrc_hndl,
                                    UINT32          link,
                                    UINT32          *pkt_size_ptr,
                                    UINT32          *pkt_per_int_ptr,
                                    UINT32          *pkt_per_n_ptr,
                                    UINT32          *pkt_per_d_ptr,
                                    DOUBLE          *epsilon_ptr);

PUBLIC PMC_ERROR cbrc_mpmo_data_get(cbrc_handle_t  *cbrc_hndl,
                                    UINT32          link,
                                    UINT32         *pkt_size_ptr,
                                    UINT32         *pkt_per_int_ptr,
                                    UINT32         *pkt_per_n_ptr,
                                    UINT32         *pkt_per_d_ptr);                                    

PUBLIC PMC_ERROR cbrc_mpmo_prov(cbrc_handle_t    *cbrc_hndl,
                                UINT32           link,
                                UINT32           pkt_size,
                                UINT32           pkt_per_int,
                                UINT32           pkt_per_n,
                                UINT32           pkt_per_d);

PUBLIC PMC_ERROR cbrc_mpmo_deprov(cbrc_handle_t    *cbrc_hndl,
                                  UINT32           link);

PUBLIC PMC_ERROR cbrc_mpmo_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link);
PUBLIC PMC_ERROR cbrc_mpmo_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link);
                                      
PUBLIC PMC_ERROR cbrc_sdh_pmg_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                       cbrc_port_dir_t   dir,
                                       UINT32            link);
PUBLIC PMC_ERROR cbrc_sonet_los_v_workaround(cbrc_handle_t *cbrc_handle_ptr, UINT32 link);
PUBLIC PMC_ERROR cbrc_sdh_pmg_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                         cbrc_port_dir_t   dir,
                                         UINT32            link);                                      

PUBLIC PMC_ERROR cbrc_sdh_pmg_pn11_ins(cbrc_handle_t            *cbrc_handle_ptr,
                                       cbrc_port_dir_t           dir,
                                       UINT32                    link,
                                       sdh_pmg_pn11_insert_t     lof_los,
                                       BOOL8                      ins_enable);

PUBLIC PMC_ERROR cbrc_sdh_pmg_pn11_ins_get(cbrc_handle_t            *cbrc_handle_ptr,
                                           cbrc_port_dir_t           dir,
                                           UINT32                    link,
                                           sdh_pmg_pn11_insert_t     lof_los,
                                           BOOL                     *ins_enable_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_oof_force(cbrc_handle_t    *cbrc_handle_ptr,
                                        cbrc_port_dir_t   dir,
                                        UINT32            link);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rrmp_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                       cbrc_port_dir_t           dir,
                                       UINT32                    link,
                                       sdh_pmg_rrmp_cfg_type_t   type,
                                       void                     *rrmp_cfg_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rrmp_cfg_get(cbrc_handle_t            *cbrc_handle_ptr,
                                           cbrc_port_dir_t           dir,
                                           UINT32                    link,
                                           sdh_pmg_rrmp_cfg_type_t   type,
                                           void                     *rrmp_cfg_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rrmp_byte_get(cbrc_handle_t            *cbrc_handle_ptr,
                                            cbrc_port_dir_t           dir,
                                            UINT32                    link,
                                            sdh_pmg_rrmp_byte_type_t  type,
                                            UINT8                    *byte_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_trmp_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                       UINT32                    link,
                                       sdh_pmg_trmp_cfg_type_t   type,
                                       sdh_pmg_trmp_alarm_cfg_t  *trmp_cfg_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_trmp_cfg_get(cbrc_handle_t            *cbrc_handle_ptr,
                                           UINT32                    link,
                                           sdh_pmg_trmp_cfg_type_t   type,
                                           sdh_pmg_trmp_alarm_cfg_t  *trmp_cfg_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_byte_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                            sdh_pmg_sber_byte_t       byte);

PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_byte_get(cbrc_handle_t            *cbrc_handle_ptr,
                                            sdh_pmg_sber_byte_t      *byte_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_enable(cbrc_handle_t                *cbrc_handle_ptr,
                                          cbrc_port_dir_t               dir,
                                          UINT32                        link,
                                          sdh_pmg_sber_alarm_t          alarm,
                                          sdh_pmg_sber_ber_t            ber);

PUBLIC PMC_ERROR cbrc_sdh_pmg_sber_ber_get(cbrc_handle_t                *cbrc_handle_ptr,
                                           cbrc_port_dir_t               dir,
                                           UINT32                        link,
                                           sdh_pmg_sber_alarm_t          alarm,
                                           sdh_pmg_sber_ber_t           *ber_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_len_cfg(cbrc_handle_t           *cbrc_handle_ptr,
                                           cbrc_port_dir_t          dir,
                                           UINT32                   link,
                                           sdh_pmg_rttp_length_t    length,
                                           sdh_pmg_rttp_algo_t      algo);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_cfg_get(cbrc_handle_t           *cbrc_handle_ptr,
                                           cbrc_port_dir_t          dir,
                                           UINT32                   link,
                                           sdh_pmg_rttp_cfg_t      *cfg_ptr);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_expect_trace_set(cbrc_handle_t            *cbrc_handle_ptr,
                                                    cbrc_port_dir_t           dir,
                                                    UINT32                    link,
                                                    sdh_pmg_rttp_trace_t     *expect_trace);

PUBLIC PMC_ERROR cbrc_sdh_pmg_rttp_trace_get(cbrc_handle_t              *cbrc_handle_ptr,
                                             cbrc_port_dir_t             dir,
                                             UINT32                      link,
                                             sdh_pmg_rttp_trace_type_t   trace_type,
                                             sdh_pmg_rttp_trace_t       *trace);


PUBLIC PMC_ERROR cbrc_mld_prbs_rx_mux_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                          UINT32               link,
                                          mld_prbs_rx_mux_t    rx_sel);

PUBLIC PMC_ERROR cbrc_cbr_fsgm_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                   cbrc_port_dir_t           dir,
                                   UINT32                    link,
                                   UINT32                    fault_ins_en);






PUBLIC PMC_ERROR cbrc_mpmo_ckctl_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                     UINT32               ckctl_port, 
                                     UINT32               chnl,
                                     BOOL8                 enable);

PUBLIC PMC_ERROR cbrc_loopback_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                   UINT32                   link,
                                   cbrc_loopback_t           cbrc_loopback);

PUBLIC PMC_ERROR cbrc_loopback_status_get(cbrc_handle_t            *cbrc_handle,
                                          UINT32                   link,
                                          cbrc_loopback_t          *loopback_ptr);

/* Test Functions */
PUBLIC PMC_ERROR cbrc_test_write(cbrc_handle_t *cbrc_handle, 
                                 UINT32 pmc_error_value,
                                 UINT32 write_value);

PUBLIC PMC_ERROR cbrc_test_read(cbrc_handle_t *cbrc_handle, 
                                UINT32 pmc_error_value,
                                UINT32 *read_value);

PUBLIC PMC_ERROR cbrc_sync_reset(cbrc_handle_t       *cbrc_handle_ptr,
                                 BOOL8                 reset);
/* PRBS functions */

PUBLIC  PMC_ERROR cbrc_sdh_prbs_set(cbrc_handle_t         *handle,
                                    cbrc_port_dir_t       dir,
                                    UINT8                 link,
                                    sdh_pmg_prbs_cfg_t    *cfg_ptr);

PUBLIC  PMC_ERROR cbrc_sdh_prbs_get(cbrc_handle_t         *handle,
                                    cbrc_port_dir_t       dir,
                                    UINT8                 link,
                                    BOOL8                 force_reg_rd,  
                                    sdh_pmg_prbs_cfg_t    *cfg_ptr);

PUBLIC PMC_ERROR cbrc_sdh_prbs_resync(cbrc_handle_t         *handle,
                                      UINT8                 link);

PUBLIC PMC_ERROR cbrc_sdh_prbs_mon_lfsr_state_get(cbrc_handle_t            *handle,
                                                  UINT8                    link,
                                                  UINT32                  *status);

PUBLIC PMC_ERROR cbrc_sdh_prbs_gen_lfsr_state_get(cbrc_handle_t            *handle,
                                                  UINT8                    link,
                                                  UINT32                  *status);

PUBLIC PMC_ERROR cbrc_sdh_prbs_oh_ins_set(cbrc_handle_t         *handle,
                                          UINT8                 link,
                                          sdh_pmg_prbs_oh_ins_t    ins_type,
                                          char                    *ins_value_ptr,
                                          BOOL8                     ins_enable);

PUBLIC PMC_ERROR cbrc_sdh_prbs_oh_ins_get(cbrc_handle_t         *handle,
                                          UINT8                 link,
                                          sdh_pmg_prbs_oh_ins_t    ins_type,
                                          char                    *ins_value_ptr,
                                          BOOL                    *ins_enable_ptr);

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR cbrc_sdh_prbs_status_get(cbrc_handle_t           *handle,
                                          UINT8                    link,
                                          util_patt_status_t      *status,
                                          UINT32                  *err_cnt);
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

PUBLIC PMC_ERROR cbrc_sdh_prbs_error_ins(cbrc_handle_t        *handle,
                                         UINT8                 link);


PUBLIC PMC_ERROR cbrc_fsgm_pattern_cfg(cbrc_handle_t       *cbrc_handle_ptr,
                                       UINT32               link,
                                       cbrc_port_dir_t      dir,
                                       cbr_fsgm_pattern_t   pattern);


PUBLIC PMC_ERROR cbrc_fsgm_pattern_cfg_get(cbrc_handle_t       *cbrc_handle_ptr,
                                           UINT32               link,
                                           cbrc_port_dir_t      dir,
                                           cbr_fsgm_pattern_t  *pattern_ptr);

PUBLIC PMC_ERROR cbrc_fsgm_user_fault_pattern_set(cbrc_handle_t                 *cbrc_handle_ptr,
                                                  UINT32                         link,
                                                  cbrc_port_dir_t                dir,
                                                  cbr_fsgm_user_fault_pattern_t  user_pattern);

PUBLIC PMC_ERROR cbrc_fsgm_user_fault_pattern_get(cbrc_handle_t                  *cbrc_handle_ptr,
                                                  UINT32                          link,
                                                  cbrc_port_dir_t                 dir,
                                                  cbr_fsgm_user_fault_pattern_t  *user_pattern_ptr);

PUBLIC PMC_ERROR cbrc_fsgm_fault_force(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link,
                                       cbrc_port_dir_t   dir,
                                       BOOL8              enable);

PUBLIC PMC_ERROR cbrc_fsgm_fault_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32            link,
                                           cbrc_port_dir_t   dir,
                                           BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fsgm_pn11_mon_enable(cbrc_handle_t       *cbrc_handle_ptr,
                                           UINT32               link,
                                           cbrc_port_dir_t      dir,
                                           BOOL8                 enable);

PUBLIC PMC_ERROR cbrc_fsgm_pn11_mon_enable_get(cbrc_handle_t   *cbrc_handle_ptr,
                                           UINT32               link,
                                           cbrc_port_dir_t      dir,
                                           BOOL                *enable_ptr);

PUBLIC PMC_ERROR cbrc_fsgm_insert_cfg(cbrc_handle_t            *cbrc_handle_ptr,
                                      UINT32                    link,
                                      cbrc_port_dir_t           dir,
                                      cbr_fsgm_pattern_cfg_t   *cfg_ptr);

PUBLIC PMC_ERROR cbrc_fsgm_insert_cfg_get(cbrc_handle_t            *cbrc_handle_ptr,
                                          UINT32                    link,
                                          cbrc_port_dir_t           dir,
                                          cbr_fsgm_pattern_cfg_t   *cfg_ptr);

PUBLIC PMC_ERROR cbrc_patt_ins_fault_force(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32            link,
                                           cbrc_port_dir_t   dir,
                                           BOOL8              force);

PUBLIC PMC_ERROR cbrc_patt_ins_fault_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32                link,
                                           cbrc_port_dir_t       dir,
                                           BOOL                 *force_ptr);

PUBLIC PMC_ERROR cbrc_patt_ins_enable(cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link,
                                      cbrc_port_dir_t   dir,
                                      BOOL8              enable);

PUBLIC PMC_ERROR cbrc_patt_ins_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                          UINT32            link,
                                          cbrc_port_dir_t   dir,
                                          BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc800_user_pattern_set(cbrc_handle_t    *cbrc_handle_ptr,
                                             UINT32            link,
                                             cbrc_port_dir_t   dir,
                                             UINT32            user_pattern);

PUBLIC PMC_ERROR cbrc_fc800_user_pattern_get(cbrc_handle_t    *cbrc_handle_ptr,
                                             UINT32            link,
                                             cbrc_port_dir_t   dir,
                                             UINT32           *user_pattern_ptr);

PUBLIC PMC_ERROR cbrc_fc800_pattern_ins_set(cbrc_handle_t                *cbrc_handle_ptr,
                                            UINT32                        link,
                                            cbrc_port_dir_t               dir,
                                            fc800_pmg_pattern_ins_cfg_t   pattern_ins_cfg);

PUBLIC PMC_ERROR cbrc_fc800_pattern_ins_get(cbrc_handle_t                *cbrc_handle_ptr,
                                            UINT32                        link,
                                            cbrc_port_dir_t               dir,
                                            fc800_pmg_pattern_ins_cfg_t  *pattern_ins_cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc800_pattern_force(cbrc_handle_t          *cbrc_handle_ptr,
                                          UINT32                  link,
                                          cbrc_port_dir_t         dir,
                                          BOOL8                    force);

PUBLIC PMC_ERROR cbrc_fc800_pattern_force_get(cbrc_handle_t         *cbrc_handle_ptr,
                                              UINT32                 link,
                                              cbrc_port_dir_t        dir,
                                              BOOL                  *force_ptr);

PUBLIC PMC_ERROR cbrc_fc800_dlolb_gen_set(cbrc_handle_t          *cbrc_handle_ptr,
                                          UINT32                  link,
                                          cbrc_port_dir_t         dir,
                                          fc800_pmg_dlolb_gen_t  *dlolb_gen_cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc800_dlolb_gen_get(cbrc_handle_t         *cbrc_handle_ptr,
                                          UINT32                 link,
                                          cbrc_port_dir_t        dir,
                                          fc800_pmg_dlolb_gen_t *dlolb_gen_cfg_ptr);
                                          
PUBLIC PMC_ERROR cbrc_fc800_dlvr_mode_cfg(cbrc_handle_t    *cbrc_handle_ptr,
                                          UINT32            link, 
                                          UINT32            fifo_thresh);

PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_cfg(cbrc_handle_t                *cbrc_handle_ptr,
                                            UINT32                        link,
                                            cbrc_port_dir_t               dir,
                                            fc1200_pmon_dlolb_cfg_t      *cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_cfg_get(cbrc_handle_t                 *cbrc_handle_ptr,
                                                UINT32                         link,
                                                cbrc_port_dir_t                dir,
                                                fc1200_pmon_dlolb_cfg_t       *cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_force(cbrc_handle_t    *cbrc_handle_ptr,
                                              UINT32            link,
                                              cbrc_port_dir_t   dir,
                                              BOOL8              enable);

PUBLIC PMC_ERROR cbrc_fc1200_pmon_dlolb_force_get(cbrc_handle_t      *cbrc_handle_ptr,
                                                  UINT32              link,
                                                  cbrc_port_dir_t     dir,
                                                  BOOL               *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_pmon_invalid_block_mode_set(
                                       cbrc_handle_t                    *cbrc_handle_ptr,
                                       UINT32                            link,
                                       cbrc_port_dir_t                   dir,
                                       fc1200_pmon_invalid_block_mode_t  mode); 

PUBLIC PMC_ERROR cbrc_fc1200_pmon_invalid_block_mode_get(
                                    cbrc_handle_t                    *cbrc_handle_ptr,
                                    UINT32                            link,
                                    cbrc_port_dir_t                   dir,
                                    fc1200_pmon_invalid_block_mode_t *mode_ptr); 

PUBLIC PMC_ERROR cbrc_fc1200_prbs31_enable(cbrc_handle_t    *cbrc_handle_ptr,
                                           UINT32            link,
                                           cbrc_port_dir_t   dir,
                                           BOOL8              enable);

PUBLIC PMC_ERROR cbrc_fc1200_prbs31_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                               UINT32            link,
                                               cbrc_port_dir_t   dir,
                                               BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_lfos_force(cbrc_handle_t    *cbrc_handle_ptr,
                                        UINT32            link,
                                        cbrc_port_dir_t   dir,
                                        BOOL8             enable);

PUBLIC PMC_ERROR cbrc_fc1200_lfos_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                            UINT32            link,
                                            cbrc_port_dir_t   dir,
                                            BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_lfos_set(cbrc_handle_t       *cbrc_handle_ptr,
                                      UINT32               link,
                                      cbrc_port_dir_t      dir,
                                      cbrc_fc1200_lfos_t  *lfos_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_lfos_get(cbrc_handle_t         *cbrc_handle_ptr,
                                      UINT32                 link,
                                      cbrc_port_dir_t        dir,
                                      cbrc_fc1200_lfos_t    *lfos_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_lfos_cfg(cbrc_handle_t             *cbrc_handle_ptr,
                                         UINT32                     link,
                                         cbrc_fc1200_rx_lfos_cfg_t *lfos_cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_lfos_cfg_get(cbrc_handle_t                 *cbrc_handle_ptr,
                                             UINT32                         link,
                                             cbrc_fc1200_rx_lfos_cfg_t     *lfos_cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_tx_lfos_cfg(cbrc_handle_t              *cbrc_handle_ptr,
                                         UINT32                      link,
                                         cbrc_fc1200_tx_lfos_cfg_t  *lfos_cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_tx_lfos_cfg_get(cbrc_handle_t              *cbrc_handle_ptr,
                                             UINT32                      link,
                                             cbrc_fc1200_tx_lfos_cfg_t  *lfos_cfg_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_restore_invalid_idle_enable(
                                      cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link,
                                      BOOL8             enable);

PUBLIC PMC_ERROR cbrc_fc1200_rx_descrambler_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                                       UINT32            link,
                                                       BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_restore_invalid_idle_enable_get(
                                      cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link,
                                      BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_descrambler_enable(cbrc_handle_t    *cbrc_handle_ptr,
                                                   UINT32            link,
                                                   BOOL8              enable);

PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_force(cbrc_handle_t    *cbrc_handle_ptr,
                                          UINT32            link,
                                          BOOL8              enable);

PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_force_get(cbrc_handle_t    *cbrc_handle_ptr,
                                              UINT32            link,
                                              BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_set(cbrc_handle_t      *cbrc_handle_ptr,
                                        UINT32              link,
                                        cbrc_fc1200_ecb_t  *ecb_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_ecb_get(cbrc_handle_t      *cbrc_handle_ptr,
                                        UINT32              link,
                                        cbrc_fc1200_ecb_t  *ecb_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_payload_scramble_enable(cbrc_handle_t  *cbrc_handle_ptr,
                                                            UINT32          link,
                                                            BOOL8            enable);

PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_payload_scramble_enable_get(cbrc_handle_t    *cbrc_handle_ptr,
                                                                UINT32            link,
                                                                BOOL             *enable_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_set(cbrc_handle_t         *cbrc_handle_ptr,
                                        UINT32                 link,
                                        cbrc_fc1200_rx_gfp_t  *gfp_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_rx_gfp_get(cbrc_handle_t         *cbrc_handle_ptr,
                                        UINT32                 link,
                                        cbrc_fc1200_rx_gfp_t  *gfp_ptr);

PUBLIC PMC_ERROR cbrc_fc1200_tx_corrupt_blk_conv_enable(
                                      cbrc_handle_t    *cbrc_handle_ptr,
                                      UINT32            link,
                                      BOOL8              enable);

PUBLIC PMC_ERROR cbrc_fc1200_tx_corrupt_blk_conv_enable_get(
                                               cbrc_handle_t    *cbrc_handle_ptr,
                                               UINT32            link,
                                               BOOL             *enable_ptr);


PUBLIC PMC_ERROR cbrc_int_chnl_enable(cbrc_handle_t    *cbrc_handle,
                                      UINT32            link,
                                      cbrc_int_chnl_t  *int_table_ptr,
                                      cbrc_int_chnl_t  *int_en_table_ptr,
                                      BOOL8              enable);

PUBLIC PMC_ERROR cbrc_int_chnl_clear(cbrc_handle_t    *cbrc_handle,
                                     UINT32            link,
                                     cbrc_int_chnl_t  *int_table_ptr,
                                     cbrc_int_chnl_t  *int_en_table_ptr);

PUBLIC PMC_ERROR cbrc_int_chnl_retrieve(cbrc_handle_t    *cbrc_handle,
                                        UINT32            link,
                                        cbrc_int_chnl_t  *filt_table_ptr,
                                        cbrc_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR cbrc_int_chnl_enabled_check(cbrc_handle_t     *cbrc_handle,
                                             UINT32             link,
                                             cbrc_int_chnl_t   *int_en_table_ptr,
                                             BOOL              *int_found_ptr);

PUBLIC PMC_ERROR cbrc_feature_validate(cbrc_handle_t       *cbrc_handle,
                                       UINT32               link,
                                       cbrc_feature_mode_t  mode);

PUBLIC cbrc_handle_t *cbrc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);

PUBLIC void cbrc_ctxt_destroy(cbrc_handle_t *cbrc_handle_ptr);


PUBLIC PMC_ERROR cbrc_port_provision(cbrc_handle_t      *cbrc_handle_ptr,
                                     UINT32              link,
                                     cbrc_serdes_type_t  serdes_type,
                                     cbrc_port_mode_t    mode);

PUBLIC PMC_ERROR cbrc_port_unprovision(cbrc_handle_t    *cbrc_handle_ptr,
                                       UINT32            link);

PUBLIC PMC_ERROR cbrc_port_activate(cbrc_handle_t    *cbrc_handle_ptr,
                                    cbrc_port_dir_t   dir,
                                    UINT32            link);

PUBLIC PMC_ERROR cbrc_port_deactivate(cbrc_handle_t    *cbrc_handle_ptr,
                                      cbrc_port_dir_t   dir,
                                      UINT32            link);

PUBLIC BOOL8 cbrc_prov_get(cbrc_handle_t *cbrc_handle_ptr,
                           UINT32         link,
                           BOOL8          rx_dir);

PUBLIC PMC_ERROR cbrc_sfis_odd_parity_set(cbrc_handle_t            *cbrc_handle,
                                          UINT8                     link,
                                          util_global_direction_t   dir,
                                          BOOL8                     inv);

PUBLIC PMC_ERROR cbrc_sfis_odd_parity_get(cbrc_handle_t           *cbrc_handle,
                                          UINT8                    link,
                                          util_global_direction_t  dir,
                                          BOOL8                   *inv_ptr);

PUBLIC BOOL8 cbrc_port_is_sonet_get(cbrc_handle_t *cbrc_hndl,
                                    UINT32 link);

#ifdef __cplusplus
}
#endif

#endif /* _CBRC_H */

/*
** end of file
*/
