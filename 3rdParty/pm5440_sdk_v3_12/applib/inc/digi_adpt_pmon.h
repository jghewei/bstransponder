/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*     This head file describes the DIGI performance monitoring Run-Time function.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_PMON_H
#define _DIGI_ADPT_PMON_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_otn_api.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_fec_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DIGI FEC performance. The
*   element terms have a syntax:
*      DIGI_<application>
*
* ELEMENTS:
*   DIGI_FEC_TOTAL_CORR_ERRS        - TOTAL_CORR_ERRS[31:0]
*   DIGI_SWZ_LPA_STATS_FP           - LPA_STATS_FP[31:0]
*   DIGI_SWZ_LPA_STATS_UNCORR_CWDS  - LPA_STATS_UNCORR_CWDS [31:0]
*   DIGI_SWZ_LPA_STATS_CORR_1S      - LPA_STATS_CORR_1S [31:0]
*   DIGI_SWZ_LPA_STATS_CORR_0S      - LPA_STATS_CORR_0S [31:0]
*   DIGI_SWZ_LLSA_STATS_UNCORR_CWDS - LLSA_STATS_UNCORR_CWDS [31:0]
*   DIGI_SWZ_LLSA_STATS_CORR_1S     - LLSA_STATS_LANE_CORR_1S [31:0]
*   DIGI_SWZ_LLSA_STATS_CORR_0S     - LLSA_STATS_LANE_CORR_0S [31:0]
*   DIGI_GFEC_LPA_STATS_FP          - LPA_STATS_FP [31:0]
*   DIGI_GFEC_LPA_STATS_UNCORR_CWDS - LPA_STATS_UNCORR_CWDS [31:0]
*   DIGI_GFEC_LPA_STATS_CORR_1S     - LPA_STATS_CORR_1S [31:0]
*   DIGI_GFEC_LPA_STATS_CORR_0S     - LPA_STATS_CORR_0S [31:0]
*   DIGI_I4_FEC_CORR_0S_COUNT       - CORRECTED_ZEROES_X_COUNT [31:0]  (X=3..0)
*   DIGI_I4_FEC_CORR_1S_COUNT       - CORRECTED_ONES_X_COUNT [31:0]    (X=3..0)
*   DIGI_I4_FEC_UNCORR_ERROR_COUNT  - UNCORR_ERROR_X_COUNT [31:0]      (X=3..0)
*   DIGI_I4_FEC_RX_FRM_PULSE_COUNT  - RX_FRAME_PULSE_X_COUNT [31:0]    (X=3..0)
*   DIGI_I7_FEC_CORR_0S_COUNT       - CORRECTED_ZEROES_X_COUNT [31:0]  (X=3..0)
*   DIGI_I7_FEC_CORR_1S_COUNT       - CORRECTED_ONES_X_COUNT [31:0]    (X=3..0)
*   DIGI_I7_FEC_UNCORR_ERROR_COUNT  - UNCORR_ERROR_X_COUNT [31:0]      (X=3..0)
*   DIGI_I7_FEC_RX_FRM_PULSE_COUNT  - RX_FRAME_PULSE_X_COUNT [31:0]    (X=3..0)
*
*******************************************************************************/
typedef enum
{
    DIGI_FEC_TOTAL_CORR_ERRS = 0x0,

    /* 40G,100G SWIZZLE FEC (for 100G need to sum up all lanes) */
    DIGI_SWZ_LPA_STATS_FP,
    DIGI_SWZ_LPA_STATS_UNCORR_CWDS,
    DIGI_SWZ_LPA_STATS_CORR_1S,
    DIGI_SWZ_LPA_STATS_CORR_0S,
    DIGI_SWZ_LLSA_STATS_UNCORR_CWDS,
    DIGI_SWZ_LLSA_STATS_CORR_1S,
    DIGI_SWZ_LLSA_STATS_CORR_0S,

    /* GFEC 10G,40G,100G */
    DIGI_GFEC_LPA_STATS_FP,
    DIGI_GFEC_LPA_STATS_UNCORR_CWDS,
    DIGI_GFEC_LPA_STATS_CORR_1S,
    DIGI_GFEC_LPA_STATS_CORR_0S,

    /* I4 FEC 10G,40G */
    DIGI_I4_FEC_CORR_0S_COUNT,
    DIGI_I4_FEC_CORR_1S_COUNT,
    DIGI_I4_FEC_UNCORR_ERROR_COUNT,
    DIGI_I4_FEC_RX_FRM_PULSE_COUNT,

    /* I7 FEC 10G,40G  */
    DIGI_I7_FEC_CORR_0S_COUNT,
    DIGI_I7_FEC_CORR_1S_COUNT,
    DIGI_I7_FEC_UNCORR_ERROR_COUNT,
    DIGI_I7_FEC_RX_FRM_PULSE_COUNT,



    LAST_DIGI_FEC, /* this is for out of range values error handling */
} digi_fec_pm_t;

/*******************************************************************************
* ENUM: digi_otn_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DIGI OTUk/ODUk performance. The
*   element terms have a syntax:
*      DIGI_OTN_<application>
*
* ELEMENTS:
*   DIGI_OTN_BIP    - BIP(Bit Interleaved Parity) counter value
*   DIGI_OTN_NDS    - near-end defect second
*   DIGI_OTN_PIAE   - IAE(Incoming Alignment Error)
*   DIGI_OTN_BEI    - BEI(Backward Error Indicator) counter value
*   DIGI_OTN_FDS    - far-end defect second
*   DIGI_OTN_PBIAE  - BIAE(Backward Incoming Alignment Error)
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_BIP = 0x0,
    DIGI_OTN_NDS,
    DIGI_OTN_PIAE,
    DIGI_OTN_BEI,
    DIGI_OTN_FDS,
    DIGI_OTN_PBIAE,

    LAST_DIGI_OTN_PM, /* this is for out of range values error handling */
} digi_otn_pm_t;

/*******************************************************************************
* ENUM: digi_enet_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet performance monitoring. The
*   element terms have a syntax:
*      DIGI_<application>
*
* ELEMENTS:
*   DIGI_ENET_MIB_PM_ALL,                        - all ENET MIB PMON with the digi_pmon_enet_mib_t
*   DIGI_FRAMES_OK_LSB,                          - aFramesRxOK (LSB 32 bits)
*   DIGI_FRAMES_OK_MSB,                          - aFramesRxOK (MSB 8 bits)
*   DIGI_FRAME_CHECK_SEQUENCE_ERRORS_LSB,        - aFrameCheckSequenceErrors (LSB 32 bits)
*   DIGI_FRAME_CHECK_SEQUENCE_ERRORS_MSB,        - aFrameCheckSequenceErrors (MSB 8 bits)
*   DIGI_ALIGNMENT_ERRORS_LSB,                   - aAlignmentErrors (LSB 32 bits)
*   DIGI_ALIGNMENT_ERRORS_MSB,                   - aAlignmentErrors (MSB 8 bits)
*   DIGI_PAUSE_MAC_CTRL_FRAMES_LSB,              - aPAUSEMACCtrlFramesRx (LSB 32 bits)
*   DIGI_PAUSE_MAC_CTRL_FRAMES_MSB,              - aPAUSEMACCtrlFramesRx (MSB 8 bits)
*   DIGI_FRAME_TOO_LONG_ERRORS_LSB,              - aFrameTooLongErrors (LSB 32 bits)
*   DIGI_FRAME_TOO_LONG_ERRORS_MSB,              - aFrameTooLongErrors (MSB 8 bits)
*   DIGI_IN_RANGE_LENGTH_ERRORS_LSB,             - aInRangeLengthErrors (LSB 32 bits)
*   DIGI_IN_RANGE_LENGTH_ERRORS_MSB,             - aInRangeLengthErrors (MSB 8 bits)
*   DIGI_VLAN_OK_LSB,                            - VLANRxOK (LSB 32 bits)
*   DIGI_VLAN_OK_MSB,                            - VLANRxOK (MSB 8 bits)
*   DIGI_IF_OCTS_LSB,                            - ifInOcts (LSB 32 bits)
*   DIGI_IF_OCTS_MSB,                            - ifInOcts (MSB 8 bits)
*   DIGI_IF_ERRORS_LSB,                          - ifInErrors (LSB 32 bits)
*   DIGI_IF_ERRORS_MSB,                          - ifInErrors (MSB 8 bits)
*   DIGI_IF_UCAST_PKTS_LSB,                      - ifInUcastPkts (LSB 32 bits)
*   DIGI_IF_UCAST_PKTS_MSB,                      - ifInUcastPkts (MSB 8 bits)
*   DIGI_IF_MULTICAST_PKTS_LSB,                  - ifInMulticastPkts (LSB 32 bits)
*   DIGI_IF_MULTICAST_PKTS_MSB,                  - ifInMulticastPkts (MSB 8 bits)
*   DIGI_IF_BROADCAST_PKTS_LSB,                  - ifInBroadcastPkts (LSB 32 bits)
*   DIGI_IF_BROADCAST_PKTS_MSB,                  - ifInBroadcastPkts (MSB 8 bits)
*   DIGI_ETHER_STATS_DROP_EVENTS_LSB,            - etherStatsDropEvents (LSB 32 bits)
*   DIGI_ETHER_STATS_DROP_EVENTS_MSB,            - etherStatsDropEvents (MSB 8 bits)
*   DIGI_ETHER_STATS_OCTS_LSB,                   - etherStatsOcts (LSB 32 bits)
*   DIGI_ETHER_STATS_OCTS_MSB,                   - etherStatsOcts (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_LSB,                   - etherStatsPkts (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_MSB,                   - etherStatsPkts (MSB 8 bits)
*   DIGI_ETHER_STATS_UNDERSIZE_PKTS_LSB,         - etherStatsUndersizePkts (LSB 32 bits)
*   DIGI_ETHER_STATS_UNDERSIZE_PKTS_MSB,         - etherStatsUndersizePkts (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_64_OCTS_LSB,           - etherStatsPkts64Octs (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_64_OCTS_MSB,           - etherStatsPkts64Octs (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_LSB,    - etherStatsPkts65to127Octs (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_MSB,    - etherStatsPkts65to127Octs (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_LSB,   - etherStatsPkts128to255Octs (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_MSB,   - etherStatsPkts128to255Octs (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_LSB,   - etherStatsPkts256to511Octs (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_MSB,   - etherStatsPkts256to511Octs (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_LSB,  - etherStatsPkts512to1023Octs (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_MSB,  - etherStatsPkts512to1023Octs (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_LSB, - etherStatsPkts1024to1518Octs (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_MSB, - etherStatsPkts1024to1518Octs (MSB 8 bits)
*   DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_LSB,  - etherStatsPkts1519toMaxOcts (LSB 32 bits)
*   DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_MSB,  - etherStatsPkts1519toMaxOcts (MSB 8 bits)
*   DIGI_ETHER_STATS_OVERSIZE_PKTS_LSB,          - etherStatsOversizePkts (LSB 32 bits)
*   DIGI_ETHER_STATS_OVERSIZE_PKTS_MSB,          - etherStatsOversizePkts (MSB 8 bits)  
*   DIGI_ETHER_STATS_JABBER_LSB,                 - etherStatsJabber (LSB 32 bits)
*   DIGI_ETHER_STATS_JABBER_MSB,                 - etherStatsJabber (MSB 8 bits)
*   DIGI_ETHER_STATS_FRAGMENTS_LSB,              - etherStatsFragments (LSB 32 bits)
*   DIGI_ETHER_STATS_FRAGMENTS_MSB,              - etherStatsFragments (MSB 8 bits)
*   DIGI_MAC_CONTROL_FRAMES_LSB,                 - aMACControlFramesRx (LSB 32 bits)
*   DIGI_MAC_CONTROL_FRAMES_MSB,                 - aMACControlFramesRx (MSB 8 bits)
*   DIGI_LLDP_PKT_CNT                            - Packets filtered by the ECLASS LLDP filter.
*
*   RFC3635: 
*   ifInOctets = aOctetsReceivedOK + (18 * aFramesReceivedOK)
*   ifOutOctets = aOctetsTransmittedOK + (18 * aFramesTransmittedOK)
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_MIB_PM_ALL = 0,
    DIGI_FRAMES_OK_LSB,
    DIGI_FRAMES_OK_MSB,
    DIGI_FRAME_CHECK_SEQUENCE_ERRORS_LSB,
    DIGI_FRAME_CHECK_SEQUENCE_ERRORS_MSB,
    DIGI_ALIGNMENT_ERRORS_LSB,
    DIGI_ALIGNMENT_ERRORS_MSB,
    DIGI_PAUSE_MAC_CTRL_FRAMES_LSB,
    DIGI_PAUSE_MAC_CTRL_FRAMES_MSB,
    DIGI_FRAME_TOO_LONG_ERRORS_LSB,
    DIGI_FRAME_TOO_LONG_ERRORS_MSB,
    DIGI_IN_RANGE_LENGTH_ERRORS_LSB,
    DIGI_IN_RANGE_LENGTH_ERRORS_MSB,
    DIGI_VLAN_OK_LSB,
    DIGI_VLAN_OK_MSB,
    DIGI_IF_OCTS_LSB,
    DIGI_IF_OCTS_MSB,
    DIGI_IF_ERRORS_LSB,
    DIGI_IF_ERRORS_MSB,
    DIGI_IF_UCAST_PKTS_LSB,
    DIGI_IF_UCAST_PKTS_MSB,
    DIGI_IF_MULTICAST_PKTS_LSB,
    DIGI_IF_MULTICAST_PKTS_MSB,
    DIGI_IF_BROADCAST_PKTS_LSB,
    DIGI_IF_BROADCAST_PKTS_MSB,
    DIGI_ETHER_STATS_DROP_EVENTS_LSB,
    DIGI_ETHER_STATS_DROP_EVENTS_MSB,
    DIGI_ETHER_STATS_OCTS_LSB,
    DIGI_ETHER_STATS_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_LSB,
    DIGI_ETHER_STATS_PKTS_MSB,
    DIGI_ETHER_STATS_UNDERSIZE_PKTS_LSB,
    DIGI_ETHER_STATS_UNDERSIZE_PKTS_MSB,
    DIGI_ETHER_STATS_PKTS_64_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_64_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_65_TO_127_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_128_TO_255_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_256_TO_511_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_512_TO_1023_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_1024_TO_1518_OCTS_MSB,
    DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_LSB,
    DIGI_ETHER_STATS_PKTS_1519_TO_MAX_OCTS_MSB,
    DIGI_ETHER_STATS_OVERSIZE_PKTS_LSB,
    DIGI_ETHER_STATS_OVERSIZE_PKTS_MSB,
    DIGI_ETHER_STATS_JABBER_LSB,
    DIGI_ETHER_STATS_JABBER_MSB,
    DIGI_ETHER_STATS_FRAGMENTS_LSB,
    DIGI_ETHER_STATS_FRAGMENTS_MSB,
    DIGI_MAC_CONTROL_FRAMES_LSB,
    DIGI_MAC_CONTROL_FRAMES_MSB,
    DIGI_LLDP_PKT_CNT,

    LAST_DIGI_ENET_PM, /* this is for out of range values error handling */
} digi_enet_pm_t;

/*******************************************************************************
* ENUM: digi_sdh_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SONET/SDH performance information. The
*   element terms have a syntax:
*      DIGI_SDH_<application>
*
* ELEMENTS:
*   DIGI_SDH_B1        - RS B1 error count
*   DIGI_SDH_B2        - MS B2 error count
*   DIGI_SDH_REI       - MS REI error count
*
*******************************************************************************/
typedef enum
{
    DIGI_SDH_B1 = 0,
    DIGI_SDH_B2,
    DIGI_SDH_REI,
    
    LAST_DIGI_SDH_PM, /* this is for out of range values error handling */
} digi_sdh_pm_t;

/*******************************************************************************
* ENUM: digi_fc1200_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for FC-1200 performance information. The
*   element terms have a syntax:
*      DIGI_<application>
*
* ELEMENTS:
*   DIGI_RX_HIGH_ERR_CNT           - ERR_CNTS -> HIGH_BER_ERR_CNT
*   DIGI_RX_BLK_TYPE_ERR_CNT       - BLOCK_ERR_CNTS -> BLK_TYPE_ERR_CNT
*   DIGI_RX_INVLD_BLK_CNT          - BLOCK_ERR_CNTS -> INVLD_BLK_CNT
*   DIGI_RX_DATA_BYTE_CNT_LSB      - RXD_DATA_BYTE_CNT_LSB (LSB 32 bits)
*   DIGI_RX_DATA_BYTE_CNT_MSB      - RXD_DATA_BYTE_CNT_MSB (MSB 8 bits)
*   DIGI_RX_OS_CNT                 - RXD_ORDERED_SET_CNT
*   DIGI_RX_VALID_FRM_CNT          - VALID_FRM_COUNT
*   DIGI_RX_INVALID_FRM_CNT        - INVLD_FRM_COUNT
*   DIGI_RX_VALID_FRM_BYTE_CNT     - VALID_FRM_BYTE_COUNT
*   DIGI_RX_CRC_24_ERR_CNT         - CRCERRCNT
*   DIGI_RX_INVALID_BLK_HDR_CNT    - INVALBLKHDRCNT
*   DIGI_TX_BYTE_CNT_LSB           - TX_DATA_BYTE_CNT_LSB (LSB 32 bits)
*   DIGI_TX_BYTE_CNT_MSB           - TX_DATA_BYTE_CNT_MSB (MSB 8 bits)
*   DIGI_TX_OS_CNT                 - TX_ORDERED_SET_CNT
*   DIGI_TX_ERR_CTRL_BLK_CNT       - ERROR_BLK_CNT
*   DIGI_TX_LF_OS_CNT              - LF_OS_CNT
*   DIGI_TX_VALID_FRM_CNT          - VALID_FRM_COUNT
*   DIGI_TX_INVALID_FRM_CNT        - INVLD_FRM_COUNT
*   DIGI_TX_VALID_FRM_BYTE_CNT     - VALID_FRM_BYTE_COUNT
*   DIGI_TX_GFP_FRM_CNT            - GFPCNT [23:0]
*   DIGI_TX_GFP_ERR_FRM_CNT        - GFPERRCNT [23:0]
*
*******************************************************************************/
typedef enum
{
    /* FC1200_PCS_RX */
    DIGI_RX_HIGH_ERR_CNT = 0,
    DIGI_RX_BLK_TYPE_ERR_CNT,
    DIGI_RX_INVLD_BLK_CNT,
    DIGI_RX_DATA_BYTE_CNT_LSB,
    DIGI_RX_DATA_BYTE_CNT_MSB,
    DIGI_RX_OS_CNT,

    /* FC1200_PMON RX */
    DIGI_RX_VALID_FRM_CNT,
    DIGI_RX_INVALID_FRM_CNT,
    DIGI_RX_VALID_FRM_BYTE_CNT,

    /* FC_TTTM */
    DIGI_RX_GFP_FRM_CNT,
    DIGI_RX_GFP_ERR_FRM_CNT,

    /* FC1200_PCS_TX */
    DIGI_TX_BYTE_CNT_LSB,
    DIGI_TX_BYTE_CNT_MSB,
    DIGI_TX_OS_CNT,
    DIGI_TX_ERR_CTRL_BLK_CNT,
    DIGI_TX_LF_OS_CNT,

    /* FC1200_PMON TX */
    DIGI_TX_VALID_FRM_CNT,
    DIGI_TX_INVALID_FRM_CNT,
    DIGI_TX_VALID_FRM_BYTE_CNT,

    /* FC_TTTD */
    DIGI_TX_CRC_24_ERR_CNT,
    DIGI_TX_INVALID_BLK_HDR_CNT,

    LAST_DIGI_FC1200, /* this is for out of range values error handling */
} digi_fc1200_pm_t;

/*******************************************************************************
* ENUM: digi_fc800_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for FC-800 performance information. The
*   element terms have a syntax:
*      DIGI_<application>
*
* ELEMENTS:
*   DIGI_GOOD_FRM_CNT          - GF_DWRD
*   DIGI_BAD_FRM_CNT           - BF_DWRD
*   DIGI_BYTE_CNT              - WRD_CNT
*   DIGI_BAD_CRC_CNT           - BAD_CRC
*   DIGI_TOTAL_PKT_CNT         - TOT_PKTS
*   DIGI_LCV_CNT               - LCV_CNT
*
*******************************************************************************/
typedef enum
{
    DIGI_GOOD_FRM_CNT = 0,
    DIGI_BAD_FRM_CNT,
    DIGI_BYTE_CNT,
    DIGI_BAD_CRC_CNT,
    DIGI_TOTAL_PKT_CNT,
    DIGI_LCV_CNT,

    LAST_DIGI_FC800, /* this is for out of range values error handling */
} digi_fc800_pm_t;

/*******************************************************************************
* ENUM: digi_gfp_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for GFP defects. The
*   element terms have a syntax:
*      DIGI_GFP_<application>
*
* ELEMENTS:
*   DIGI_GFP_SK_RX_BYTE_COUNT_LSB                     - RX_BYTE_CNT_LSB [31:0]
*   DIGI_GFP_SK_RX_BYTE_COUNT_MSB                     - RX_BYTE_CNT_MSB [8:0]    
*   DIGI_GFP_SK_RX_CLIENT_DATA_FRM_COUNT              - RX_FRM_CNT [31:0]             
*   DIGI_GFP_SK_RX_IDLE_FRM_COUNT                     - IDLE_FRM_CNT [31:0]           
*   DIGI_GFP_SK_RX_CTRL_FRM_COUNT                     - CTRL_FRM_CNT [31:0]           
*   DIGI_GFP_SK_RX_CHEC_ERR_DROP_FRM_COUNT            - ERR_CHEC_CNT [31:0]           
*   DIGI_GFP_SK_RX_CHEC_SINGLE_CORR_ERR_FRM_COUNT     - SINGL_ERR_CORR_CHEC_CNT [31:0]
*   DIGI_GFP_SK_RX_UNPACK_DROP_FRM_COUNT              - UNPACK_DROP_FRM_CNT [31:0]    
*   DIGI_GFP_SK_N_FDIS_THEC                           - ERR_THEC_CNT [31:0]           
*   DIGI_GFP_SK_RX_THEC_SINGLE_CORR_ERR_FRM_COUNT     - SINGL_ERR_CORR_TE_CNT [31:0]  
*   DIGI_GFP_SK_RX_FILTER_DROP_FRM_COUNT              - FILTER_DROP_FRM_CNT [31:0]    
*   DIGI_GFP_SK_N_FDIS_EHEC_EXI                       - DEXM_EHEC_CNT [31:0]          
*   DIGI_GFP_SK_P_FDIS                                - DUPM_FRM_CNT [31:0]           
*   DIGI_GFP_SK_RX_GENERIC_FILTER_DROP_FRM_COUNT      - DGEM_FRM_CNT [31:0]           
*   DIGI_GFP_SK_RX_CSF_FRM_COUNT                      - CSF_FRM_CNT [31:0]            
*   DIGI_GFP_SK_P_FCS_ERROR                           - ERR_FCS_CNT [31:0]            
*   DIGI_GFP_SK_RX_PAUSE_FRM_DROP_COUNT               - PAUSE_FRM_CNT [31:0]          
*   DIGI_GFP_SK_RX_PAUSE_FRM_FROWARDED_COUNT          - PAUSE_FRM_TR_CNT [31:0]       
*   DIGI_GFP_SK_RX_CPU_FIFO_OVR_FLW_DROP_FRM_COUNT    - CPU_OVF_CNT [31:0]            
*   DIGI_GFP_SK_TX_FRM_COUNT                          - EGRESS_FRM_CNT [31:0]         
*   DIGI_GFP_SK_RX_UNPACKED_FRM_COUNT                 - UNPACK_FRM_CNT [31:0]         
*   DIGI_GFP_SK_RX_MAX_FRM_LENGTH_FRM_DROP_COUNT      - MINFL_CHK_CNT [31:0]          
*   DIGI_GFP_SK_RX_MIN_FRM_LENGTH_FRM_DROP_COUNT      - MAXFL_CHK_CNT [31:0]          
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_0    - FL_CHK_STAT_CNT [31:0]        
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_1    - FL_CHK_STAT_CNT [31:0]         
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_2    - FL_CHK_STAT_CNT [31:0]            
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_3    - FL_CHK_STAT_CNT [31:0]            
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_4    - FL_CHK_STAT_CNT [31:0]         
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_5    - FL_CHK_STAT_CNT [31:0]         
*   DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_6    - FL_CHK_STAT_CNT [31:0]            
*   DIGI_GFP_SO_TX_0_COUNT                            - TX_BYTE_CNT0 [39:0]
*   DIGI_GFP_SO_TX_1_COUNT                            - TX_BYTE_CNT1 [39:0]
*   DIGI_GFP_SO_TX_MGMT_FRMS                          - MGMT_FRM_CNT [31:0]                         
*   DIGI_GFP_SO_TX_CLIENT_DATA_FRAME_COUNT            - TX_FRM_CNT [31:0] 
*   DIGI_GFP_SO_TX_PAUSE_FRM_COUNT                    - PAUSE_FRM_CNT [31:0] 
*   DIGI_GFP_SO_TX_IDLE_FRM_COUNT                     - IDLE_FRM_CNT [31:0]  
*   DIGI_GFP_SO_TX_ERR_FRM_COUNT                      - ERR_FRM_CNT [31:0]
*   DIGI_GFP_SO_TX_MIN_FRM_LENGTH_FRM_COUNT           - TX_FRM_CNT_UNDERSIZE [31:0] 
*   DIGI_GFP_SO_TX_MAX_FRM_LENGTH_FRM_COUNT           - TX_FRM_CNT_OVERSIZE [31:0]
*   DIGI_GFP_DCPB_SLV_DPI_PKT_DROP_COUNT              - CH_DROP_CNT_VAL (for DCPB)
*   DIGI_GFP_OCPB_SLV_DPI_PKT_DROP_COUNT              - CH_DROP_CNT_VAL (for ODUK_SW)                       
*
*******************************************************************************/
typedef enum
{
    DIGI_GFP_SK_RX_BYTE_COUNT_LSB = 0,
    DIGI_GFP_SK_RX_BYTE_COUNT_MSB,
    DIGI_GFP_SK_RX_CLIENT_DATA_FRM_COUNT,
    DIGI_GFP_SK_RX_IDLE_FRM_COUNT,
    DIGI_GFP_SK_RX_CTRL_FRM_COUNT,
    DIGI_GFP_SK_RX_CHEC_ERR_DROP_FRM_COUNT,
    DIGI_GFP_SK_RX_CHEC_SINGLE_CORR_ERR_FRM_COUNT,
    DIGI_GFP_SK_RX_UNPACK_DROP_FRM_COUNT,
    DIGI_GFP_SK_N_FDIS_THEC,
    DIGI_GFP_SK_RX_THEC_SINGLE_CORR_ERR_FRM_COUNT,
    DIGI_GFP_SK_RX_FILTER_DROP_FRM_COUNT,
    DIGI_GFP_SK_N_FDIS_EHEC_EXI,
    DIGI_GFP_SK_P_FDIS,
    DIGI_GFP_SK_RX_GENERIC_FILTER_DROP_FRM_COUNT,
    DIGI_GFP_SK_RX_CSF_FRM_COUNT,
    DIGI_GFP_SK_P_FCS_ERROR,
    DIGI_GFP_SK_RX_PAUSE_FRM_DROP_COUNT,
    DIGI_GFP_SK_RX_PAUSE_FRM_FROWARDED_COUNT,
    DIGI_GFP_SK_RX_CPU_FIFO_OVR_FLW_DROP_FRM_COUNT,
    DIGI_GFP_SK_TX_FRM_COUNT,
    DIGI_GFP_SK_RX_UNPACKED_FRM_COUNT,
    DIGI_GFP_SK_RX_MAX_FRM_LENGTH_FRM_DROP_COUNT,
    DIGI_GFP_SK_RX_MIN_FRM_LENGTH_FRM_DROP_COUNT,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_0,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_1,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_2,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_3,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_4,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_5,
    DIGI_GFP_SK_RX_MIN_LEN_CHK_FAIL_FW_FRM_COUNT_6,
    DIGI_GFP_SO_TX_0_COUNT,
    DIGI_GFP_SO_TX_1_COUNT,
    DIGI_GFP_SO_TX_MGMT_FRMS,
    DIGI_GFP_SO_TX_CLIENT_DATA_FRAME_COUNT,
    DIGI_GFP_SO_TX_PAUSE_FRM_COUNT,
    DIGI_GFP_SO_TX_IDLE_FRM_COUNT,
    DIGI_GFP_SO_TX_ERR_FRM_COUNT,
    DIGI_GFP_SO_TX_MIN_FRM_LENGTH_FRM_COUNT,
    DIGI_GFP_SO_TX_MAX_FRM_LENGTH_FRM_COUNT,

    /* CPB counts*/
    DIGI_GFP_DCPB_SLV_DPI_PKT_DROP_COUNT,
    DIGI_GFP_OCPB_SLV_DPI_PKT_DROP_COUNT,
    
    LAST_DIGI_GFP_PM, /* this is for out of range values error handling */
} digi_gfp_pm_t;

/*******************************************************************************
* ENUM: digi_sifd_pm_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for SIFD performance information. The
*   element terms have a syntax:
*      DIGI_SIFD_<application>
*
* ELEMENTS:
*   DIGI_SIFD_TX_ERR_CNT_CPB            - TX_ERR_CNT(N=0) [31:0]
*   DIGI_SIFD_TX_ERR_CNT_ODUK           - TX_ERR_CNT(N=1) [31:0]
*   DIGI_SIFD_RX_ERR_CNT                - RX_ERR_CNT [31:0] 
*   DIGI_SIFD_TX_DIAG_CNT_CPB           - TX_DIAG_CNT(N=0)[31:0]
*   DIGI_SIFD_TX_DIAG_CNT_ODUK          - TX_DIAG_CNT(N=1)[31:0]
*   DIGI_SIFD_RX_DIAG_CNT_CPB           - RX_DIAG_CNT(N=0)[31:0] 
*   DIGI_SIFD_RX_DIAG_CNT_ODUK          - RX_DIAG_CNT(N=1)[31:0]  
*   DIGI_SIFD_CPB_DCPB_PACKET_DROP_CNT  - CH_DROP_CNT_VAL (for DCPB)
*   DIGI_SIFD_CPB_OCPB_PACKET_DROP_CNT  - CH_DROP_CNT_VAL (for ODUK_SW)   
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_TX_ERR_CNT_CPB = 0,
    DIGI_SIFD_TX_ERR_CNT_ODUK,
    DIGI_SIFD_RX_ERR_CNT,
    DIGI_SIFD_TX_DIAG_CNT_CPB,
    DIGI_SIFD_TX_DIAG_CNT_ODUK,
    DIGI_SIFD_RX_DIAG_CNT_CPB,
    DIGI_SIFD_RX_DIAG_CNT_ODUK,
    DIGI_SIFD_CPB_DCPB_PACKET_DROP_CNT,
    DIGI_SIFD_CPB_OCPB_PACKET_DROP_CNT,

    LAST_DIGI_SIFD_PM, /* this is for out of range values error handling */
} digi_sifd_pm_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_pmon_int_set(UINT32 dev_id, 
                                 digi_pmon_int_t *int_table);
PUBLIC PMC_ERROR adpt_pmon_trigger(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_pmon_clear(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_otn_fec_pm_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_fec_pm_t pm_id,
                                   UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_otn_pm_get(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_otn_seg_t seg_id,
                               digi_otn_loc_t location,
                               digi_otn_pm_t pm_id,
                               UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_otn_dm_trigger(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_seg_t seg_id,
                                  digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_otn_dm_pm_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   UINT32 *dm_count);
PUBLIC PMC_ERROR adpt_otn_prbs_pm_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_enet_pm_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_enet_pm_t pm_id,
                                UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_enet_errors_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_pmon_enet_errors_t *enet_errors);
PUBLIC PMC_ERROR adpt_enet_prbs_pm_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_sdh_pm_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_sdh_pm_t pm_id,
                                UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_sdh_prbs_pm_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_gfp_pm_get(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_gfp_pm_t pm_id,
                               UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_fc1200_pm_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_fc1200_pm_t pm_id,
                                  UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_fc800_pm_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_port_dir_t dir,
                                 digi_fc800_pm_t pm_id,
                                 UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_fc_prbs_pm_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   UINT32 *pm_count);
PUBLIC PMC_ERROR adpt_sifd_pm_get(UINT32 dev_id,
                                ilkn_sifd_type_t sifd_id,
                                digi_sifd_pm_t pm_id,
                                UINT32 *pm_count);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_PMON_H */

/* 
** end of file 
*/

