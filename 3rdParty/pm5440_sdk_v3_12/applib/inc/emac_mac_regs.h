/******************************************************************************
 *   COPYRIGHT (C) 2013 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
 * -------------------------------------------------------------------------
 *   DESCRIPTION:
 *     Contains all register offset and register bit definitions for the
 *     emac_mac block
 *****************************************************************************/
#ifndef _EMAC_MAC_REGS_H
#define _EMAC_MAC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MAC_REG_REVISION                                        0x00008000
#define PMC_MAC_REG_SCRATCH                                         0x00008004
#define PMC_MAC_REG_CTRL_AND_CFG                                    0x00008008
#define PMC_MAC_REG_MAC_ADR_LSB_                                    0x0000800c
#define PMC_MAC_REG_MAC_ADR_MSB_                                    0x00008010
#define PMC_MAC_REG_FRAME_LENGTH                                    0x00008014
#define PMC_MAC_REG_STAT                                            0x00008040
#define PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_                        0x00008044
#define PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET                     0x00008048
#define PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION            0x0000804c
#define PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA                          0x00008054
#define PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA                          0x00008058
#define PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA                          0x0000805c
#define PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA                          0x00008060
#define PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH                       0x00008064
#define PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH                       0x00008068
#define PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH                       0x0000806c
#define PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH                       0x00008070
#define PMC_MAC_REG_RX_PAUSE_STAT                                   0x00008074
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_                  0x00008080
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_                  0x00008084
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_                  0x00008088
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_                  0x0000808c
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_        0x00008090
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_        0x00008094
#define PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_               0x00008098
#define PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_               0x0000809c
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_        0x000080a0
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_        0x000080a4
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_        0x000080a8
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_        0x000080ac
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_            0x000080b0
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_            0x000080b4
#define PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_           0x000080b8
#define PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_           0x000080bc
#define PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_                     0x000080c0
#define PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_                     0x000080c4
#define PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_                     0x000080c8
#define PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_                     0x000080cc
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_                    0x000080d0
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_                    0x000080d4
#define PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_                     0x000080d8
#define PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_                     0x000080dc
#define PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_                0x000080e0
#define PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_                0x000080e4
#define PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_            0x000080e8
#define PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_            0x000080ec
#define PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_            0x000080f0
#define PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_            0x000080f4
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_                    0x000080f8
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_                    0x000080fc
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_               0x00008108
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_               0x0000810c
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_           0x00008110
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_           0x00008114
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_           0x00008118
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_           0x0000811c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_         0x00008120
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_         0x00008124
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_               0x00008128
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_               0x0000812c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_               0x00008130
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_               0x00008134
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_      0x00008138
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_      0x0000813c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_         0x00008140
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_         0x00008144
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_    0x00008148
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_    0x0000814c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_   0x00008150
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_   0x00008154
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_   0x00008158
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_   0x0000815c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_  0x00008160
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_  0x00008164
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_ 0x00008168
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_ 0x0000816c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_  0x00008170
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_  0x00008174
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_       0x00008178
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_       0x0000817c
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_             0x00008180
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_             0x00008184
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_          0x00008188
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_          0x0000818c
#define PMC_MAC_REG_STATISTIC_CNT_IFINERRS_LSB_                     0x00008190
#define PMC_MAC_REG_STATISTIC_CNT_IFINERRS_MSB_                     0x00008194
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_         0x00008198
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_         0x0000819c
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_         0x000081a0
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_         0x000081a4
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_         0x000081a8
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_         0x000081ac
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_         0x000081b0
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_         0x000081b4
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_         0x000081b8
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_         0x000081bc
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_         0x000081c0
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_         0x000081c4
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_         0x000081c8
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_         0x000081cc
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_         0x000081d0
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_         0x000081d4
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_         0x000081d8
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_         0x000081dc
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_         0x000081e0
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_         0x000081e4
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_         0x000081e8
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_         0x000081ec
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_         0x000081f0
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_         0x000081f4
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_         0x000081f8
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_         0x000081fc
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_         0x00008200
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_         0x00008204
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_         0x00008208
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_         0x0000820c
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_         0x00008210
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_         0x00008214
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_          0x00008218
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_          0x0000821c
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_             0x00008220
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_             0x00008224
#define PMC_MAC_REG_TX_PREAMBLE_LSB_                                0x00008280
#define PMC_MAC_REG_TX_PREAMBLE_MSB_                                0x00008284

/*------------------------------.
 | Register 0x00008000 REVISION |
 +------------------------------+
 | bit  31:16 R  PRGM_REVISION  |
 | bit  15:8  R  CORE_VERSION   |
 | bit  7:0   R  CORE_REVISION  |
 +-----------------------------*/
#define PMC_MAC_REG_REVISION_UNUSED_MASK       0x00000000
#define MAC_REG_REVISION_BIT_PRGM_REVISION_MSK 0xffff0000
#define MAC_REG_REVISION_BIT_PRGM_REVISION_OFF 16
#define MAC_REG_REVISION_BIT_CORE_VERSION_MSK  0x0000ff00
#define MAC_REG_REVISION_BIT_CORE_VERSION_OFF  8
#define MAC_REG_REVISION_BIT_CORE_REVISION_MSK 0x000000ff
#define MAC_REG_REVISION_BIT_CORE_REVISION_OFF 0

/*-----------------------------.
 | Register 0x00008004 SCRATCH |
 +-----------------------------+
 | bit  31:0 R/W  SCRATCH      |
 +----------------------------*/
#define PMC_MAC_REG_SCRATCH_UNUSED_MASK 0x00000000
#define MAC_REG_SCRATCH_BIT_SCRATCH_MSK 0xffffffff
#define MAC_REG_SCRATCH_BIT_SCRATCH_OFF 0

/*----------------------------------.
 | Register 0x00008008 CTRL_AND_CFG |
 +----------------------------------+
 | bit  29 R/W  SEND_RF             |
 | bit  28 R/W  SEND_LF             |
 | bit  27 R/W  FLT_HDL_DIS         |
 | bit  22 R/W  TX_FLUSH            |
 | bit  21 R/W  RX_SFD_ANY          |
 | bit  20 R/W  PAUSE_PFC_COMP      |
 | bit  19 R/W  PFC_MODE            |
 | bit  18 R/W  RS_COL_CNT_EXT      |
 | bit  17 R/W  NO_LGTH_CHECK       |
 | bit  16 R/W  SEND_IDLE           |
 | bit  13 R/W  CMD_FRAME_EN        |
 | bit  12 R/W  SW_RESET            |
 | bit  11 R/W  TX_PAD_EN           |
 | bit  9  R/W  TX_ADDR_INS         |
 | bit  8  R/W  PAUSE_IGNORE        |
 | bit  7  R/W  PAUSE_FWD           |
 | bit  6  R/W  CRC_FWD             |
 | bit  4  R/W  PROMIS_EN           |
 | bit  3  R/W  WAN_MODE            |
 | bit  1  R/W  RX_ENA              |
 | bit  0  R/W  TX_ENA              |
 +---------------------------------*/
#define PMC_MAC_REG_CTRL_AND_CFG_UNUSED_MASK        0xc7800004
#define MAC_REG_CTRL_AND_CFG_BIT_SEND_RF_MSK        0x20000000
#define MAC_REG_CTRL_AND_CFG_BIT_SEND_RF_OFF        29
#define MAC_REG_CTRL_AND_CFG_BIT_SEND_LF_MSK        0x10000000
#define MAC_REG_CTRL_AND_CFG_BIT_SEND_LF_OFF        28
#define MAC_REG_CTRL_AND_CFG_BIT_FLT_HDL_DIS_MSK    0x08000000
#define MAC_REG_CTRL_AND_CFG_BIT_FLT_HDL_DIS_OFF    27
#define MAC_REG_CTRL_AND_CFG_BIT_TX_FLUSH_MSK       0x00400000
#define MAC_REG_CTRL_AND_CFG_BIT_TX_FLUSH_OFF       22
#define MAC_REG_CTRL_AND_CFG_BIT_RX_SFD_ANY_MSK     0x00200000
#define MAC_REG_CTRL_AND_CFG_BIT_RX_SFD_ANY_OFF     21
#define MAC_REG_CTRL_AND_CFG_BIT_PAUSE_PFC_COMP_MSK 0x00100000
#define MAC_REG_CTRL_AND_CFG_BIT_PAUSE_PFC_COMP_OFF 20
#define MAC_REG_CTRL_AND_CFG_BIT_PFC_MODE_MSK       0x00080000
#define MAC_REG_CTRL_AND_CFG_BIT_PFC_MODE_OFF       19
#define MAC_REG_CTRL_AND_CFG_BIT_RS_COL_CNT_EXT_MSK 0x00040000
#define MAC_REG_CTRL_AND_CFG_BIT_RS_COL_CNT_EXT_OFF 18
#define MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_MSK  0x00020000
#define MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_OFF  17
#define MAC_REG_CTRL_AND_CFG_BIT_SEND_IDLE_MSK      0x00010000
#define MAC_REG_CTRL_AND_CFG_BIT_SEND_IDLE_OFF      16
#define MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_MSK   0x00002000
#define MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_OFF   13
#define MAC_REG_CTRL_AND_CFG_BIT_SW_RESET_MSK       0x00001000
#define MAC_REG_CTRL_AND_CFG_BIT_SW_RESET_OFF       12
#define MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_MSK      0x00000800
#define MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_OFF      11
#define MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_MSK    0x00000200
#define MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_OFF    9
#define MAC_REG_CTRL_AND_CFG_BIT_PAUSE_IGNORE_MSK   0x00000100
#define MAC_REG_CTRL_AND_CFG_BIT_PAUSE_IGNORE_OFF   8
#define MAC_REG_CTRL_AND_CFG_BIT_PAUSE_FWD_MSK      0x00000080
#define MAC_REG_CTRL_AND_CFG_BIT_PAUSE_FWD_OFF      7
#define MAC_REG_CTRL_AND_CFG_BIT_CRC_FWD_MSK        0x00000040
#define MAC_REG_CTRL_AND_CFG_BIT_CRC_FWD_OFF        6
#define MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_MSK      0x00000010
#define MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_OFF      4
#define MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_MSK       0x00000008
#define MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_OFF       3
#define MAC_REG_CTRL_AND_CFG_BIT_RX_ENA_MSK         0x00000002
#define MAC_REG_CTRL_AND_CFG_BIT_RX_ENA_OFF         1
#define MAC_REG_CTRL_AND_CFG_BIT_TX_ENA_MSK         0x00000001
#define MAC_REG_CTRL_AND_CFG_BIT_TX_ENA_OFF         0

/*----------------------------------.
 | Register 0x0000800c MAC_ADR_LSB_ |
 +----------------------------------+
 | bit  31:0 R/W  MAC_ADDR_LSB      |
 +---------------------------------*/
#define PMC_MAC_REG_MAC_ADR_LSB_UNUSED_MASK      0x00000000
#define MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_MSK 0xffffffff
#define MAC_REG_MAC_ADR_LSB_BIT_MAC_ADDR_LSB_OFF 0

/*----------------------------------.
 | Register 0x00008010 MAC_ADR_MSB_ |
 +----------------------------------+
 | bit  15:0 R/W  MAC_ADDR_MSB      |
 +---------------------------------*/
#define PMC_MAC_REG_MAC_ADR_MSB_UNUSED_MASK      0xffff0000
#define MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_MSK 0x0000ffff
#define MAC_REG_MAC_ADR_MSB_BIT_MAC_ADDR_MSB_OFF 0

/*----------------------------------.
 | Register 0x00008014 FRAME_LENGTH |
 +----------------------------------+
 | bit  15:0 R/W  FRM_LENGTH        |
 +---------------------------------*/
#define PMC_MAC_REG_FRAME_LENGTH_UNUSED_MASK    0xffff0000
#define MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_MSK 0x0000ffff
#define MAC_REG_FRAME_LENGTH_BIT_FRM_LENGTH_OFF 0

/*--------------------------.
 | Register 0x00008040 STAT |
 +--------------------------+
 | bit  2 R  PHY_LOS        |
 | bit  1 R  RX_REM_FAULT   |
 | bit  0 R  RX_LOC_FAULT   |
 +-------------------------*/
#define PMC_MAC_REG_STAT_UNUSED_MASK      0xfffffff0
#define MAC_REG_STAT_BIT_PHY_LOS_MSK      0x00000004
#define MAC_REG_STAT_BIT_PHY_LOS_OFF      2
#define MAC_REG_STAT_BIT_RX_REM_FAULT_MSK 0x00000002
#define MAC_REG_STAT_BIT_RX_REM_FAULT_OFF 1
#define MAC_REG_STAT_BIT_RX_LOC_FAULT_MSK 0x00000001
#define MAC_REG_STAT_BIT_RX_LOC_FAULT_OFF 0

/*----------------------------------------------.
 | Register 0x00008044 TX_INTER_PACKET_GAP_IPG_ |
 +----------------------------------------------+
 | bit  6:0 R/W  TX_IPG_LENGTH                  |
 +---------------------------------------------*/
#define PMC_MAC_REG_TX_INTER_PACKET_GAP_IPG_UNUSED_MASK       0xffffff80
#define MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_MSK 0x0000007f
#define MAC_REG_TX_INTER_PACKET_GAP_IPG_BIT_TX_IPG_LENGTH_OFF 0

/*-------------------------------------------------.
 | Register 0x00008048 RX_APPLICATION_CREDIT_RESET |
 +-------------------------------------------------+
 | bit  0 R/W  CREDIT_RESET                        |
 +------------------------------------------------*/
#define PMC_MAC_REG_RX_APPLICATION_CREDIT_RESET_UNUSED_MASK      0xfffffffe
#define MAC_REG_RX_APPLICATION_CREDIT_RESET_BIT_CREDIT_RESET_MSK 0x00000001
#define MAC_REG_RX_APPLICATION_CREDIT_RESET_BIT_CREDIT_RESET_OFF 0

/*----------------------------------------------------------.
 | Register 0x0000804c RX_APPLICATION_CREDIT_INITIALIZATION |
 +----------------------------------------------------------+
 | bit  7:0 R/W  CREDIT_INIT                                |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_UNUSED_MASK     0xffffff00
#define MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_MSK 0x000000ff
#define MAC_REG_RX_APPLICATION_CREDIT_INITIALIZATION_BIT_CREDIT_INIT_OFF 0

/*--------------------------------------------.
 | Register 0x00008054 CLASS_0_1_PAUSE_QUANTA |
 +--------------------------------------------+
 | bit  31:16 R/W  CL1_PAUSE_QUANTA           |
 | bit  15:0  R/W  CL0_PAUSE_QUANTA           |
 +-------------------------------------------*/
#define PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_UNUSED_MASK          0x00000000
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_MSK 0xffff0000
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL1_PAUSE_QUANTA_OFF 16
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_MSK 0x0000ffff
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_BIT_CL0_PAUSE_QUANTA_OFF 0

/*--------------------------------------------.
 | Register 0x00008058 CLASS_2_3_PAUSE_QUANTA |
 +--------------------------------------------+
 | bit  31:16 R/W  CL3_PAUSE_QUANTA           |
 | bit  15:0  R/W  CL2_PAUSE_QUANTA           |
 +-------------------------------------------*/
#define PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_UNUSED_MASK          0x00000000
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_MSK 0xffff0000
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL3_PAUSE_QUANTA_OFF 16
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_MSK 0x0000ffff
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_BIT_CL2_PAUSE_QUANTA_OFF 0

/*--------------------------------------------.
 | Register 0x0000805c CLASS_4_5_PAUSE_QUANTA |
 +--------------------------------------------+
 | bit  31:16 R/W  CL5_PAUSE_QUANTA           |
 | bit  15:0  R/W  CL4_PAUSE_QUANTA           |
 +-------------------------------------------*/
#define PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_UNUSED_MASK          0x00000000
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_MSK 0xffff0000
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL5_PAUSE_QUANTA_OFF 16
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_MSK 0x0000ffff
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_BIT_CL4_PAUSE_QUANTA_OFF 0

/*--------------------------------------------.
 | Register 0x00008060 CLASS_6_7_PAUSE_QUANTA |
 +--------------------------------------------+
 | bit  31:16 R/W  CL7_PAUSE_QUANTA           |
 | bit  15:0  R/W  CL6_PAUSE_QUANTA           |
 +-------------------------------------------*/
#define PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_UNUSED_MASK          0x00000000
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_MSK 0xffff0000
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL7_PAUSE_QUANTA_OFF 16
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_MSK 0x0000ffff
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_BIT_CL6_PAUSE_QUANTA_OFF 0

/*-----------------------------------------------.
 | Register 0x00008064 CLASS_0_1_PAUSE_QUANTA_TH |
 +-----------------------------------------------+
 | bit  31:16 R/W  CL1_PAUSE_QUANTA_THR          |
 | bit  15:0  R/W  CL0_PAUSE_QUANTA_THR          |
 +----------------------------------------------*/
#define PMC_MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_UNUSED_MASK              0x00000000
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_MSK 0xffff0000
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL1_PAUSE_QUANTA_THR_OFF 16
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_MSK 0x0000ffff
#define MAC_REG_CLASS_0_1_PAUSE_QUANTA_TH_BIT_CL0_PAUSE_QUANTA_THR_OFF 0

/*-----------------------------------------------.
 | Register 0x00008068 CLASS_2_3_PAUSE_QUANTA_TH |
 +-----------------------------------------------+
 | bit  31:16 R/W  CL3_PAUSE_QUANTA_THR          |
 | bit  15:0  R/W  CL2_PAUSE_QUANTA_THR          |
 +----------------------------------------------*/
#define PMC_MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_UNUSED_MASK              0x00000000
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_MSK 0xffff0000
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL3_PAUSE_QUANTA_THR_OFF 16
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_MSK 0x0000ffff
#define MAC_REG_CLASS_2_3_PAUSE_QUANTA_TH_BIT_CL2_PAUSE_QUANTA_THR_OFF 0

/*-----------------------------------------------.
 | Register 0x0000806c CLASS_4_5_PAUSE_QUANTA_TH |
 +-----------------------------------------------+
 | bit  31:16 R/W  CL5_PAUSE_QUANTA_THR          |
 | bit  15:0  R/W  CL4_PAUSE_QUANTA_THR          |
 +----------------------------------------------*/
#define PMC_MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_UNUSED_MASK              0x00000000
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_MSK 0xffff0000
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL5_PAUSE_QUANTA_THR_OFF 16
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_MSK 0x0000ffff
#define MAC_REG_CLASS_4_5_PAUSE_QUANTA_TH_BIT_CL4_PAUSE_QUANTA_THR_OFF 0

/*-----------------------------------------------.
 | Register 0x00008070 CLASS_6_7_PAUSE_QUANTA_TH |
 +-----------------------------------------------+
 | bit  31:16 R/W  CL7_PAUSE_QUANTA_THR          |
 | bit  15:0  R/W  CL6_PAUSE_QUANTA_THR          |
 +----------------------------------------------*/
#define PMC_MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_UNUSED_MASK              0x00000000
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_MSK 0xffff0000
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL7_PAUSE_QUANTA_THR_OFF 16
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_MSK 0x0000ffff
#define MAC_REG_CLASS_6_7_PAUSE_QUANTA_TH_BIT_CL6_PAUSE_QUANTA_THR_OFF 0

/*-----------------------------------.
 | Register 0x00008074 RX_PAUSE_STAT |
 +-----------------------------------+
 | bit  7:0 R  PAUSE_STATUS          |
 +----------------------------------*/
#define PMC_MAC_REG_RX_PAUSE_STAT_UNUSED_MASK      0xffffff00
#define MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_MSK 0x000000ff
#define MAC_REG_RX_PAUSE_STAT_BIT_PAUSE_STATUS_OFF 0

/*----------------------------------------------------.
 | Register 0x00008080 STATISTIC_CNT_AFRAMESTXOK_LSB_ |
 +----------------------------------------------------+
 | bit  31:0 R  AFRAMESTXOK_LSB                       |
 +---------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_UNUSED_MASK         0x00000000
#define MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AFRAMESTXOK_LSB_BIT_AFRAMESTXOK_LSB_OFF 0

/*----------------------------------------------------.
 | Register 0x00008084 STATISTIC_CNT_AFRAMESTXOK_MSB_ |
 +----------------------------------------------------+
 | bit  7:0 R  AFRAMESTXOK_MSB                        |
 +---------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_UNUSED_MASK         0xffffff00
#define MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AFRAMESTXOK_MSB_BIT_AFRAMESTXOK_MSB_OFF 0

/*----------------------------------------------------.
 | Register 0x00008088 STATISTIC_CNT_AFRAMESRXOK_LSB_ |
 +----------------------------------------------------+
 | bit  31:0 R  AFRAMESRXOK_LSB                       |
 +---------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_UNUSED_MASK         0x00000000
#define MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AFRAMESRXOK_LSB_BIT_AFRAMESRXOK_LSB_OFF 0

/*----------------------------------------------------.
 | Register 0x0000808c STATISTIC_CNT_AFRAMESRXOK_MSB_ |
 +----------------------------------------------------+
 | bit  7:0 R  AFRAMESRXOK_MSB                        |
 +---------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_UNUSED_MASK         0xffffff00
#define MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AFRAMESRXOK_MSB_BIT_AFRAMESRXOK_MSB_OFF 0

/*--------------------------------------------------------------.
 | Register 0x00008090 STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_ |
 +--------------------------------------------------------------+
 | bit  31:0 R  AFRAMECHECKSEQUENCEERRORS_LSB                   |
 +-------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_UNUSED_MASK                       0x00000000
#define MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_BIT_AFRAMECHECKSEQUENCEERRORS_LSB_OFF 0

/*--------------------------------------------------------------.
 | Register 0x00008094 STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_ |
 +--------------------------------------------------------------+
 | bit  7:0 R  AFRAMECHECKSEQUENCEERRORS_MSB                    |
 +-------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_UNUSED_MASK                       0xffffff00
#define MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_BIT_AFRAMECHECKSEQUENCEERRORS_MSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00008098 STATISTIC_CNT_AALIGNMENTERRS_LSB_ |
 +-------------------------------------------------------+
 | bit  31:0 R  AALIGNMENTERRORS_LSB                     |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_UNUSED_MASK              0x00000000
#define MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_LSB_BIT_AALIGNMENTERRORS_LSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x0000809c STATISTIC_CNT_AALIGNMENTERRS_MSB_ |
 +-------------------------------------------------------+
 | bit  7:0 R  AALIGNMENTERRORS_MSB                      |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_UNUSED_MASK              0xffffff00
#define MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AALIGNMENTERRS_MSB_BIT_AALIGNMENTERRORS_MSB_OFF 0

/*--------------------------------------------------------------.
 | Register 0x000080a0 STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_ |
 +--------------------------------------------------------------+
 | bit  31:0 R  APAUSEMACCTRLFRAMESTX_LSB                       |
 +-------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_UNUSED_MASK                   0x00000000
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_BIT_APAUSEMACCTRLFRAMESTX_LSB_OFF 0

/*--------------------------------------------------------------.
 | Register 0x000080a4 STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_ |
 +--------------------------------------------------------------+
 | bit  7:0 R  APAUSEMACCTRLFRAMESTX_MSB                        |
 +-------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_UNUSED_MASK                   0xffffff00
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_BIT_APAUSEMACCTRLFRAMESTX_MSB_OFF 0

/*--------------------------------------------------------------.
 | Register 0x000080a8 STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_ |
 +--------------------------------------------------------------+
 | bit  31:0 R  APAUSEMACCTRLFRAMESRX_LSB                       |
 +-------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_UNUSED_MASK                   0x00000000
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_BIT_APAUSEMACCTRLFRAMESRX_LSB_OFF 0

/*--------------------------------------------------------------.
 | Register 0x000080ac STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_ |
 +--------------------------------------------------------------+
 | bit  7:0 R  APAUSEMACCTRLFRAMESRX_MSB                        |
 +-------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_UNUSED_MASK                   0xffffff00
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_BIT_APAUSEMACCTRLFRAMESRX_MSB_OFF 0

/*----------------------------------------------------------.
 | Register 0x000080b0 STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_ |
 +----------------------------------------------------------+
 | bit  31:0 R  AFRAMETOOLONGERRORS_LSB                     |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_UNUSED_MASK                 0x00000000
#define MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_BIT_AFRAMETOOLONGERRORS_LSB_OFF 0

/*----------------------------------------------------------.
 | Register 0x000080b4 STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_ |
 +----------------------------------------------------------+
 | bit  7:0 R  AFRAMETOOLONGERRORS_MSB                      |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_UNUSED_MASK                 0xffffff00
#define MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_BIT_AFRAMETOOLONGERRORS_MSB_OFF 0

/*-----------------------------------------------------------.
 | Register 0x000080b8 STATISTIC_CNT_AINRANGELENGTHERRS_LSB_ |
 +-----------------------------------------------------------+
 | bit  31:0 R  AINRANGELENGTHERRORS_LSB                     |
 +----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_LSB_BIT_AINRANGELENGTHERRORS_LSB_OFF 0

/*-----------------------------------------------------------.
 | Register 0x000080bc STATISTIC_CNT_AINRANGELENGTHERRS_MSB_ |
 +-----------------------------------------------------------+
 | bit  7:0 R  AINRANGELENGTHERRORS_MSB                      |
 +----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AINRANGELENGTHERRS_MSB_BIT_AINRANGELENGTHERRORS_MSB_OFF 0

/*-------------------------------------------------.
 | Register 0x000080c0 STATISTIC_CNT_VLANTXOK_LSB_ |
 +-------------------------------------------------+
 | bit  31:0 R  VLANTXOK_LSB                       |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_UNUSED_MASK      0x00000000
#define MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_VLANTXOK_LSB_BIT_VLANTXOK_LSB_OFF 0

/*-------------------------------------------------.
 | Register 0x000080c4 STATISTIC_CNT_VLANTXOK_MSB_ |
 +-------------------------------------------------+
 | bit  7:0 R  VLANTXOK_MSB                        |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_UNUSED_MASK      0xffffff00
#define MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_VLANTXOK_MSB_BIT_VLANTXOK_MSB_OFF 0

/*-------------------------------------------------.
 | Register 0x000080c8 STATISTIC_CNT_VLANRXOK_LSB_ |
 +-------------------------------------------------+
 | bit  31:0 R  VLANRXOK_LSB                       |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_UNUSED_MASK      0x00000000
#define MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_VLANRXOK_LSB_BIT_VLANRXOK_LSB_OFF 0

/*-------------------------------------------------.
 | Register 0x000080cc STATISTIC_CNT_VLANRXOK_MSB_ |
 +-------------------------------------------------+
 | bit  7:0 R  VLANRXOK_MSB                        |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_UNUSED_MASK      0xffffff00
#define MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_VLANRXOK_MSB_BIT_VLANRXOK_MSB_OFF 0

/*--------------------------------------------------.
 | Register 0x000080d0 STATISTIC_CNT_IFOUTOCTS_LSB_ |
 +--------------------------------------------------+
 | bit  31:0 R  IFOUTOCTS_LSB                       |
 +-------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_UNUSED_MASK       0x00000000
#define MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFOUTOCTS_LSB_BIT_IFOUTOCTS_LSB_OFF 0

/*--------------------------------------------------.
 | Register 0x000080d4 STATISTIC_CNT_IFOUTOCTS_MSB_ |
 +--------------------------------------------------+
 | bit  7:0 R  IFOUTOCTS_MSB                        |
 +-------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_UNUSED_MASK       0xffffff00
#define MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFOUTOCTS_MSB_BIT_IFOUTOCTS_MSB_OFF 0

/*-------------------------------------------------.
 | Register 0x000080d8 STATISTIC_CNT_IFINOCTS_LSB_ |
 +-------------------------------------------------+
 | bit  31:0 R  IFINOCTS_LSB                       |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_UNUSED_MASK      0x00000000
#define MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFINOCTS_LSB_BIT_IFINOCTS_LSB_OFF 0

/*-------------------------------------------------.
 | Register 0x000080dc STATISTIC_CNT_IFINOCTS_MSB_ |
 +-------------------------------------------------+
 | bit  7:0 R  IFINOCTS_MSB                        |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_UNUSED_MASK      0xffffff00
#define MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFINOCTS_MSB_BIT_IFINOCTS_MSB_OFF 0

/*------------------------------------------------------.
 | Register 0x000080e0 STATISTIC_CNT_IFINUCASTPKTS_LSB_ |
 +------------------------------------------------------+
 | bit  31:0 R  IFINUCASTPKTS_LSB                       |
 +-----------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_UNUSED_MASK           0x00000000
#define MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_LSB_BIT_IFINUCASTPKTS_LSB_OFF 0

/*------------------------------------------------------.
 | Register 0x000080e4 STATISTIC_CNT_IFINUCASTPKTS_MSB_ |
 +------------------------------------------------------+
 | bit  7:0 R  IFINUCASTPKTS_MSB                        |
 +-----------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_UNUSED_MASK           0xffffff00
#define MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFINUCASTPKTS_MSB_BIT_IFINUCASTPKTS_MSB_OFF 0

/*----------------------------------------------------------.
 | Register 0x000080e8 STATISTIC_CNT_IFINMULTICASTPKTS_LSB_ |
 +----------------------------------------------------------+
 | bit  31:0 R  IFINMULTICASTPKTS_LSB                       |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_UNUSED_MASK               0x00000000
#define MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_LSB_BIT_IFINMULTICASTPKTS_LSB_OFF 0

/*----------------------------------------------------------.
 | Register 0x000080ec STATISTIC_CNT_IFINMULTICASTPKTS_MSB_ |
 +----------------------------------------------------------+
 | bit  7:0 R  IFINMULTICASTPKTS_MSB                        |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_UNUSED_MASK               0xffffff00
#define MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFINMULTICASTPKTS_MSB_BIT_IFINMULTICASTPKTS_MSB_OFF 0

/*----------------------------------------------------------.
 | Register 0x000080f0 STATISTIC_CNT_IFINBROADCASTPKTS_LSB_ |
 +----------------------------------------------------------+
 | bit  31:0 R  IFINBROADCASTPKTS_LSB                       |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_UNUSED_MASK               0x00000000
#define MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_LSB_BIT_IFINBROADCASTPKTS_LSB_OFF 0

/*----------------------------------------------------------.
 | Register 0x000080f4 STATISTIC_CNT_IFINBROADCASTPKTS_MSB_ |
 +----------------------------------------------------------+
 | bit  7:0 R  IFINBROADCASTPKTS_MSB                        |
 +---------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_UNUSED_MASK               0xffffff00
#define MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFINBROADCASTPKTS_MSB_BIT_IFINBROADCASTPKTS_MSB_OFF 0

/*--------------------------------------------------.
 | Register 0x000080f8 STATISTIC_CNT_IFOUTERRS_LSB_ |
 +--------------------------------------------------+
 | bit  31:0 R  IFOUTERRORS_LSB                     |
 +-------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_UNUSED_MASK         0x00000000
#define MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFOUTERRS_LSB_BIT_IFOUTERRORS_LSB_OFF 0

/*--------------------------------------------------.
 | Register 0x000080fc STATISTIC_CNT_IFOUTERRS_MSB_ |
 +--------------------------------------------------+
 | bit  7:0 R  IFOUTERRORS_MSB                      |
 +-------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_UNUSED_MASK         0xffffff00
#define MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFOUTERRS_MSB_BIT_IFOUTERRORS_MSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00008108 STATISTIC_CNT_IFOUTUCASTPKTS_LSB_ |
 +-------------------------------------------------------+
 | bit  31:0 R  IFOUTUCASTPKTS_LSB                       |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_UNUSED_MASK            0x00000000
#define MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_LSB_BIT_IFOUTUCASTPKTS_LSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x0000810c STATISTIC_CNT_IFOUTUCASTPKTS_MSB_ |
 +-------------------------------------------------------+
 | bit  7:0 R  IFOUTUCASTPKTS_MSB                        |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_UNUSED_MASK            0xffffff00
#define MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFOUTUCASTPKTS_MSB_BIT_IFOUTUCASTPKTS_MSB_OFF 0

/*-----------------------------------------------------------.
 | Register 0x00008110 STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_ |
 +-----------------------------------------------------------+
 | bit  31:0 R  IFOUTMULTICASTPKTS_LSB                       |
 +----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_UNUSED_MASK                0x00000000
#define MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_BIT_IFOUTMULTICASTPKTS_LSB_OFF 0

/*-----------------------------------------------------------.
 | Register 0x00008114 STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_ |
 +-----------------------------------------------------------+
 | bit  7:0 R  IFOUTMULTICASTPKTS_MSB                        |
 +----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_UNUSED_MASK                0xffffff00
#define MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_BIT_IFOUTMULTICASTPKTS_MSB_OFF 0

/*-----------------------------------------------------------.
 | Register 0x00008118 STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_ |
 +-----------------------------------------------------------+
 | bit  31:0 R  IFOUTBROADCASTPKTS_LSB                       |
 +----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_UNUSED_MASK                0x00000000
#define MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_BIT_IFOUTBROADCASTPKTS_LSB_OFF 0

/*-----------------------------------------------------------.
 | Register 0x0000811c STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_ |
 +-----------------------------------------------------------+
 | bit  7:0 R  IFOUTBROADCASTPKTS_MSB                        |
 +----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_UNUSED_MASK                0xffffff00
#define MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_BIT_IFOUTBROADCASTPKTS_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008120 STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSDROPEVENTS_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_BIT_ETHERSTATSDROPEVENTS_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008124 STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSDROPEVENTS_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_BIT_ETHERSTATSDROPEVENTS_MSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00008128 STATISTIC_CNT_ETHERSTATSOCTS_LSB_ |
 +-------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSOCTS_LSB                       |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_UNUSED_MASK            0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_LSB_BIT_ETHERSTATSOCTS_LSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x0000812c STATISTIC_CNT_ETHERSTATSOCTS_MSB_ |
 +-------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSOCTS_MSB                        |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_UNUSED_MASK            0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOCTS_MSB_BIT_ETHERSTATSOCTS_MSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00008130 STATISTIC_CNT_ETHERSTATSPKTS_LSB_ |
 +-------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS_LSB                       |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_UNUSED_MASK            0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_LSB_BIT_ETHERSTATSPKTS_LSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00008134 STATISTIC_CNT_ETHERSTATSPKTS_MSB_ |
 +-------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS_MSB                        |
 +------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_UNUSED_MASK            0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS_MSB_BIT_ETHERSTATSPKTS_MSB_OFF 0

/*----------------------------------------------------------------.
 | Register 0x00008138 STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_ |
 +----------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSUNDERSIZEPKTS_LSB                       |
 +---------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_UNUSED_MASK                     0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_BIT_ETHERSTATSUNDERSIZEPKTS_LSB_OFF 0

/*----------------------------------------------------------------.
 | Register 0x0000813c STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_ |
 +----------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSUNDERSIZEPKTS_MSB                        |
 +---------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_UNUSED_MASK                     0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_BIT_ETHERSTATSUNDERSIZEPKTS_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008140 STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS64OCTS_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_BIT_ETHERSTATSPKTS64OCTS_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008144 STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS64OCTS_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_BIT_ETHERSTATSPKTS64OCTS_MSB_OFF 0

/*------------------------------------------------------------------.
 | Register 0x00008148 STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_ |
 +------------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS65TO127OCTS_LSB                       |
 +-----------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_UNUSED_MASK                       0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_BIT_ETHERSTATSPKTS65TO127OCTS_LSB_OFF 0

/*------------------------------------------------------------------.
 | Register 0x0000814c STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_ |
 +------------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS65TO127OCTS_MSB                        |
 +-----------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_UNUSED_MASK                       0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_BIT_ETHERSTATSPKTS65TO127OCTS_MSB_OFF 0

/*-------------------------------------------------------------------.
 | Register 0x00008150 STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_ |
 +-------------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS128TO255OCTS_LSB                       |
 +------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_UNUSED_MASK                        0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_BIT_ETHERSTATSPKTS128TO255OCTS_LSB_OFF 0

/*-------------------------------------------------------------------.
 | Register 0x00008154 STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_ |
 +-------------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS128TO255OCTS_MSB                        |
 +------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_UNUSED_MASK                        0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_BIT_ETHERSTATSPKTS128TO255OCTS_MSB_OFF 0

/*-------------------------------------------------------------------.
 | Register 0x00008158 STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_ |
 +-------------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS256TO511OCTS_LSB                       |
 +------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_UNUSED_MASK                        0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_BIT_ETHERSTATSPKTS256TO511OCTS_LSB_OFF 0

/*-------------------------------------------------------------------.
 | Register 0x0000815c STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_ |
 +-------------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS256TO511OCTS_MSB                        |
 +------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_UNUSED_MASK                        0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_BIT_ETHERSTATSPKTS256TO511OCTS_MSB_OFF 0

/*--------------------------------------------------------------------.
 | Register 0x00008160 STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_ |
 +--------------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS512TO1023OCTS_LSB                       |
 +-------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_UNUSED_MASK                         0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_BIT_ETHERSTATSPKTS512TO1023OCTS_LSB_OFF 0

/*--------------------------------------------------------------------.
 | Register 0x00008164 STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_ |
 +--------------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS512TO1023OCTS_MSB                        |
 +-------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_UNUSED_MASK                         0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_BIT_ETHERSTATSPKTS512TO1023OCTS_MSB_OFF 0

/*---------------------------------------------------------------------.
 | Register 0x00008168 STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_ |
 +---------------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS1024TO1518OCTS_LSB                       |
 +--------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_UNUSED_MASK                          0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_LSB_OFF 0

/*---------------------------------------------------------------------.
 | Register 0x0000816c STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_ |
 +---------------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS1024TO1518OCTS_MSB                        |
 +--------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_UNUSED_MASK                          0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_BIT_ETHERSTATSPKTS1024TO1518OCTS_MSB_OFF 0

/*--------------------------------------------------------------------.
 | Register 0x00008170 STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_ |
 +--------------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSPKTS1519TOMAXOCTS_LSB                       |
 +-------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_UNUSED_MASK                         0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_OFF 0

/*--------------------------------------------------------------------.
 | Register 0x00008174 STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_ |
 +--------------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSPKTS1519TOMAXOCTS_MSB                        |
 +-------------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_UNUSED_MASK                         0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_BIT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_OFF 0

/*---------------------------------------------------------------.
 | Register 0x00008178 STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_ |
 +---------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSOVERSIZEPKTS_LSB                       |
 +--------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_UNUSED_MASK                    0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_BIT_ETHERSTATSOVERSIZEPKTS_LSB_OFF 0

/*---------------------------------------------------------------.
 | Register 0x0000817c STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_ |
 +---------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSOVERSIZEPKTS_MSB                        |
 +--------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_UNUSED_MASK                    0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_BIT_ETHERSTATSOVERSIZEPKTS_MSB_OFF 0

/*---------------------------------------------------------.
 | Register 0x00008180 STATISTIC_CNT_ETHERSTATSJABBER_LSB_ |
 +---------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSJABBER_LSB                       |
 +--------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_UNUSED_MASK              0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_LSB_BIT_ETHERSTATSJABBER_LSB_OFF 0

/*---------------------------------------------------------.
 | Register 0x00008184 STATISTIC_CNT_ETHERSTATSJABBER_MSB_ |
 +---------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSJABBER_MSB                        |
 +--------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_UNUSED_MASK              0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSJABBER_MSB_BIT_ETHERSTATSJABBER_MSB_OFF 0

/*------------------------------------------------------------.
 | Register 0x00008188 STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_ |
 +------------------------------------------------------------+
 | bit  31:0 R  ETHERSTATSFRAGMENTS_LSB                       |
 +-----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_UNUSED_MASK                 0x00000000
#define MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_BIT_ETHERSTATSFRAGMENTS_LSB_OFF 0

/*------------------------------------------------------------.
 | Register 0x0000818c STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_ |
 +------------------------------------------------------------+
 | bit  7:0 R  ETHERSTATSFRAGMENTS_MSB                        |
 +-----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_UNUSED_MASK                 0xffffff00
#define MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_BIT_ETHERSTATSFRAGMENTS_MSB_OFF 0

/*-------------------------------------------------.
 | Register 0x00008190 STATISTIC_CNT_IFINERRS_LSB_ |
 +-------------------------------------------------+
 | bit  31:0 R  IFINERRORS_LSB                     |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINERRS_LSB_UNUSED_MASK        0x00000000
#define MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_IFINERRS_LSB_BIT_IFINERRORS_LSB_OFF 0

/*-------------------------------------------------.
 | Register 0x00008194 STATISTIC_CNT_IFINERRS_MSB_ |
 +-------------------------------------------------+
 | bit  7:0 R  IFINERRORS_MSB                      |
 +------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_IFINERRS_MSB_UNUSED_MASK        0xffffff00
#define MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_IFINERRS_MSB_BIT_IFINERRORS_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008198 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_0_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_BIT_ACBFCPAUSEFRAMESTX_0_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x0000819c STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_0_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_BIT_ACBFCPAUSEFRAMESTX_0_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081a0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBCFPAUSEFRAMESTX_1_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_BIT_ACBCFPAUSEFRAMESTX_1_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081a4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBCFPAUSEFRAMESTX_1_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_BIT_ACBCFPAUSEFRAMESTX_1_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081a8 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_2_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_BIT_ACBFCPAUSEFRAMESTX_2_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081ac STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_2_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_BIT_ACBFCPAUSEFRAMESTX_2_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081b0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_3_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_BIT_ACBFCPAUSEFRAMESTX_3_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081b4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_3_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_BIT_ACBFCPAUSEFRAMESTX_3_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081b8 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_4_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_BIT_ACBFCPAUSEFRAMESTX_4_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081bc STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_4_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_BIT_ACBFCPAUSEFRAMESTX_4_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081c0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_5_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_BIT_ACBFCPAUSEFRAMESTX_5_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081c4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_5_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_BIT_ACBFCPAUSEFRAMESTX_5_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081c8 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_6_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_BIT_ACBFCPAUSEFRAMESTX_6_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081cc STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_6_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_BIT_ACBFCPAUSEFRAMESTX_6_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081d0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESTX_7_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_BIT_ACBFCPAUSEFRAMESTX_7_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081d4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESTX_7_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_BIT_ACBFCPAUSEFRAMESTX_7_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081d8 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_0_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_BIT_ACBFCPAUSEFRAMESRX_0_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081dc STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_0_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_BIT_ACBFCPAUSEFRAMESRX_0_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081e0 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_1_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_BIT_ACBFCPAUSEFRAMESRX_1_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081e4 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_1_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_BIT_ACBFCPAUSEFRAMESRX_1_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081e8 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_2_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_BIT_ACBFCPAUSEFRAMESRX_2_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081ec STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_2_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_BIT_ACBFCPAUSEFRAMESRX_2_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081f0 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_3_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_BIT_ACBFCPAUSEFRAMESRX_3_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081f4 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_3_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_BIT_ACBFCPAUSEFRAMESRX_3_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081f8 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_4_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_BIT_ACBFCPAUSEFRAMESRX_4_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x000081fc STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_4_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_BIT_ACBFCPAUSEFRAMESRX_4_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008200 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_5_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_BIT_ACBFCPAUSEFRAMESRX_5_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008204 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_5_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_BIT_ACBFCPAUSEFRAMESRX_5_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008208 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_6_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_BIT_ACBFCPAUSEFRAMESRX_6_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x0000820c STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_6_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_BIT_ACBFCPAUSEFRAMESRX_6_MSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008210 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_ |
 +-------------------------------------------------------------+
 | bit  31:0 R  ACBFCPAUSEFRAMESRX_7_LSB                       |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_UNUSED_MASK                  0x00000000
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_BIT_ACBFCPAUSEFRAMESRX_7_LSB_OFF 0

/*-------------------------------------------------------------.
 | Register 0x00008214 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_ |
 +-------------------------------------------------------------+
 | bit  7:0 R  ACBFCPAUSEFRAMESRX_7_MSB                        |
 +------------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_UNUSED_MASK                  0xffffff00
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_BIT_ACBFCPAUSEFRAMESRX_7_MSB_OFF 0

/*------------------------------------------------------------.
 | Register 0x00008218 STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_ |
 +------------------------------------------------------------+
 | bit  31:0 R  AMACCONTROLFRAMESTX_LSB                       |
 +-----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_UNUSED_MASK                 0x00000000
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_BIT_AMACCONTROLFRAMESTX_LSB_OFF 0

/*------------------------------------------------------------.
 | Register 0x0000821c STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_ |
 +------------------------------------------------------------+
 | bit  7:0 R  AMACCONTROLFRAMESTX_MSB                        |
 +-----------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_UNUSED_MASK                 0xffffff00
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_BIT_AMACCONTROLFRAMESTX_MSB_OFF 0

/*---------------------------------------------------------.
 | Register 0x00008220 STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_ |
 +---------------------------------------------------------+
 | bit  31:0 R  AMACCONTROLFRAMESRX_LSB                    |
 +--------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_UNUSED_MASK                 0x00000000
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_MSK 0xffffffff
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_BIT_AMACCONTROLFRAMESRX_LSB_OFF 0

/*---------------------------------------------------------.
 | Register 0x00008224 STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_ |
 +---------------------------------------------------------+
 | bit  7:0 R  AMACCONTROLFRAMESRX_MSB                     |
 +--------------------------------------------------------*/
#define PMC_MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_UNUSED_MASK                 0xffffff00
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_MSK 0x000000ff
#define MAC_REG_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_BIT_AMACCONTROLFRAMESRX_MSB_OFF 0

/*--------------------------------------.
 | Register 0x00008280 TX_PREAMBLE_LSB_ |
 +--------------------------------------+
 | bit  31:0 R/W  TX_PREAMBLE_LSB       |
 +-------------------------------------*/
#define PMC_MAC_REG_TX_PREAMBLE_LSB_UNUSED_MASK         0x00000000
#define MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_MSK 0xffffffff
#define MAC_REG_TX_PREAMBLE_LSB_BIT_TX_PREAMBLE_LSB_OFF 0

/*--------------------------------------.
 | Register 0x00008284 TX_PREAMBLE_MSB_ |
 +--------------------------------------+
 | bit  23:0 R/W  TX_PREAMBLE_MSB       |
 +-------------------------------------*/
#define PMC_MAC_REG_TX_PREAMBLE_MSB_UNUSED_MASK         0xff000000
#define MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_MSK 0x00ffffff
#define MAC_REG_TX_PREAMBLE_MSB_BIT_TX_PREAMBLE_MSB_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_MAC_REGS_H */
