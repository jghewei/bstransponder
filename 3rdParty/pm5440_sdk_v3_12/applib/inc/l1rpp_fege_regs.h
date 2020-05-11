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
 *     l1rpp_fege block
 *****************************************************************************/
#ifndef _L1RPP_FEGE_REGS_H
#define _L1RPP_FEGE_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_L1RPP_FEGE_REG_INT                                                 0x00000080
#define PMC_L1RPP_FEGE_REG_INT_EN                                              0x00000084
#define PMC_L1RPP_FEGE_REG_INT_STAT                                            0x00000088
#define PMC_L1RPP_FEGE_REG_CFG_AND_STAT                                        0x0000008c
#define PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG                                 0x00000090
#define PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG                      0x00000094
#define PMC_L1RPP_FEGE_REG_MII_CTRL                                            0x00000098
#define PMC_L1RPP_FEGE_REG_MII_STAT                                            0x0000009c
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT                  0x000000a0
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY 0x000000a4
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION                      0x000000a8
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX                   0x000000ac
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE    0x000000b0
#define PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE                           0x000000bc
#define PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION                                  0x000000c0
#define PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE                                     0x000000c4
#define PMC_L1RPP_FEGE_REG_COMMA_DET                                           0x000000c8
#define PMC_L1RPP_FEGE_REG_PMON_CNT_0                                          0x000000cc
#define PMC_L1RPP_FEGE_REG_PMON_CNT_1                                          0x000000d0
#define PMC_L1RPP_FEGE_REG_PMON_CNT_2                                          0x000000d4
#define PMC_L1RPP_FEGE_REG_PMON_UPDATE                                         0x000000d8
#define PMC_L1RPP_FEGE_REG_BIT_ALIGNMENT                                       0x000000dc
#define PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION                              0x000000e0

/*--------------------------------------.
 | Register 0x00000080 INT              |
 +--------------------------------------+
 | bit  15 R/W  LOS_I                   |
 | bit  14 R/W  SDET_IN_I               |
 | bit  13 R/W  ALIGN_CHANGE_I          |
 | bit  12 R/W  LINK_MON_I              |
 | bit  11 R/W  FEC_STAT_I              |
 | bit  10 R/W  CARRIER_STAT_I          |
 | bit  9  R/W  RX_ERR_I                |
 | bit  8  R/W  LCV_I                   |
 | bit  7  R/W  AUTO_NEG_LP_ABLTY_UPD_I |
 | bit  4  R/W  LINK_OK_I               |
 | bit  0  R/W  SYNC_OK_I               |
 +-------------------------------------*/
#define PMC_L1RPP_FEGE_REG_INT_UNUSED_MASK                 0xffff006e
#define L1RPP_FEGE_REG_INT_BIT_LOS_I_MSK                   0x00008000
#define L1RPP_FEGE_REG_INT_BIT_LOS_I_OFF                   15
#define L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_MSK               0x00004000
#define L1RPP_FEGE_REG_INT_BIT_SDET_IN_I_OFF               14
#define L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_MSK          0x00002000
#define L1RPP_FEGE_REG_INT_BIT_ALIGN_CHANGE_I_OFF          13
#define L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_MSK              0x00001000
#define L1RPP_FEGE_REG_INT_BIT_LINK_MON_I_OFF              12
#define L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_MSK              0x00000800
#define L1RPP_FEGE_REG_INT_BIT_FEC_STAT_I_OFF              11
#define L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_MSK          0x00000400
#define L1RPP_FEGE_REG_INT_BIT_CARRIER_STAT_I_OFF          10
#define L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_MSK                0x00000200
#define L1RPP_FEGE_REG_INT_BIT_RX_ERR_I_OFF                9
#define L1RPP_FEGE_REG_INT_BIT_LCV_I_MSK                   0x00000100
#define L1RPP_FEGE_REG_INT_BIT_LCV_I_OFF                   8
#define L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_MSK 0x00000080
#define L1RPP_FEGE_REG_INT_BIT_AUTO_NEG_LP_ABLTY_UPD_I_OFF 7
#define L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_MSK               0x00000010
#define L1RPP_FEGE_REG_INT_BIT_LINK_OK_I_OFF               4
#define L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_MSK               0x00000001
#define L1RPP_FEGE_REG_INT_BIT_SYNC_OK_I_OFF               0

/*--------------------------------------.
 | Register 0x00000084 INT_EN           |
 +--------------------------------------+
 | bit  15 R/W  LOS_E                   |
 | bit  14 R/W  SDET_IN_E               |
 | bit  13 R/W  ALIGN_CHANGE_E          |
 | bit  12 R/W  LINK_MON_E              |
 | bit  11 R/W  FEC_STAT_E              |
 | bit  10 R/W  CARRIER_STAT_E          |
 | bit  9  R/W  RX_ERR_E                |
 | bit  8  R/W  LCV_E                   |
 | bit  7  R/W  AUTO_NEG_LP_ABLTY_UPD_E |
 | bit  4  R/W  LINK_OK_E               |
 | bit  0  R/W  SYNC_OK_E               |
 +-------------------------------------*/
#define PMC_L1RPP_FEGE_REG_INT_EN_UNUSED_MASK                 0xffff006e
#define L1RPP_FEGE_REG_INT_EN_BIT_LOS_E_MSK                   0x00008000
#define L1RPP_FEGE_REG_INT_EN_BIT_LOS_E_OFF                   15
#define L1RPP_FEGE_REG_INT_EN_BIT_SDET_IN_E_MSK               0x00004000
#define L1RPP_FEGE_REG_INT_EN_BIT_SDET_IN_E_OFF               14
#define L1RPP_FEGE_REG_INT_EN_BIT_ALIGN_CHANGE_E_MSK          0x00002000
#define L1RPP_FEGE_REG_INT_EN_BIT_ALIGN_CHANGE_E_OFF          13
#define L1RPP_FEGE_REG_INT_EN_BIT_LINK_MON_E_MSK              0x00001000
#define L1RPP_FEGE_REG_INT_EN_BIT_LINK_MON_E_OFF              12
#define L1RPP_FEGE_REG_INT_EN_BIT_FEC_STAT_E_MSK              0x00000800
#define L1RPP_FEGE_REG_INT_EN_BIT_FEC_STAT_E_OFF              11
#define L1RPP_FEGE_REG_INT_EN_BIT_CARRIER_STAT_E_MSK          0x00000400
#define L1RPP_FEGE_REG_INT_EN_BIT_CARRIER_STAT_E_OFF          10
#define L1RPP_FEGE_REG_INT_EN_BIT_RX_ERR_E_MSK                0x00000200
#define L1RPP_FEGE_REG_INT_EN_BIT_RX_ERR_E_OFF                9
#define L1RPP_FEGE_REG_INT_EN_BIT_LCV_E_MSK                   0x00000100
#define L1RPP_FEGE_REG_INT_EN_BIT_LCV_E_OFF                   8
#define L1RPP_FEGE_REG_INT_EN_BIT_AUTO_NEG_LP_ABLTY_UPD_E_MSK 0x00000080
#define L1RPP_FEGE_REG_INT_EN_BIT_AUTO_NEG_LP_ABLTY_UPD_E_OFF 7
#define L1RPP_FEGE_REG_INT_EN_BIT_LINK_OK_E_MSK               0x00000010
#define L1RPP_FEGE_REG_INT_EN_BIT_LINK_OK_E_OFF               4
#define L1RPP_FEGE_REG_INT_EN_BIT_SYNC_OK_E_MSK               0x00000001
#define L1RPP_FEGE_REG_INT_EN_BIT_SYNC_OK_E_OFF               0

/*------------------------------------.
 | Register 0x00000088 INT_STAT       |
 +------------------------------------+
 | bit  15 R  LOS_V                   |
 | bit  14 R  SDET_IN_V               |
 | bit  13 R  ALIGN_CHANGE_V          |
 | bit  12 R  LINK_MON_V              |
 | bit  11 R  FEC_STAT_V              |
 | bit  10 R  CARRIER_STAT_V          |
 | bit  7  R  AUTO_NEG_LP_ABLTY_UPD_V |
 | bit  4  R  LINK_OK_V               |
 | bit  0  R  SYNC_OK_V               |
 +-----------------------------------*/
#define PMC_L1RPP_FEGE_REG_INT_STAT_UNUSED_MASK                 0xffff036e
#define L1RPP_FEGE_REG_INT_STAT_BIT_LOS_V_MSK                   0x00008000
#define L1RPP_FEGE_REG_INT_STAT_BIT_LOS_V_OFF                   15
#define L1RPP_FEGE_REG_INT_STAT_BIT_SDET_IN_V_MSK               0x00004000
#define L1RPP_FEGE_REG_INT_STAT_BIT_SDET_IN_V_OFF               14
#define L1RPP_FEGE_REG_INT_STAT_BIT_ALIGN_CHANGE_V_MSK          0x00002000
#define L1RPP_FEGE_REG_INT_STAT_BIT_ALIGN_CHANGE_V_OFF          13
#define L1RPP_FEGE_REG_INT_STAT_BIT_LINK_MON_V_MSK              0x00001000
#define L1RPP_FEGE_REG_INT_STAT_BIT_LINK_MON_V_OFF              12
#define L1RPP_FEGE_REG_INT_STAT_BIT_FEC_STAT_V_MSK              0x00000800
#define L1RPP_FEGE_REG_INT_STAT_BIT_FEC_STAT_V_OFF              11
#define L1RPP_FEGE_REG_INT_STAT_BIT_CARRIER_STAT_V_MSK          0x00000400
#define L1RPP_FEGE_REG_INT_STAT_BIT_CARRIER_STAT_V_OFF          10
#define L1RPP_FEGE_REG_INT_STAT_BIT_AUTO_NEG_LP_ABLTY_UPD_V_MSK 0x00000080
#define L1RPP_FEGE_REG_INT_STAT_BIT_AUTO_NEG_LP_ABLTY_UPD_V_OFF 7
#define L1RPP_FEGE_REG_INT_STAT_BIT_LINK_OK_V_MSK               0x00000010
#define L1RPP_FEGE_REG_INT_STAT_BIT_LINK_OK_V_OFF               4
#define L1RPP_FEGE_REG_INT_STAT_BIT_SYNC_OK_V_MSK               0x00000001
#define L1RPP_FEGE_REG_INT_STAT_BIT_SYNC_OK_V_OFF               0

/*----------------------------------.
 | Register 0x0000008c CFG_AND_STAT |
 +----------------------------------+
 | bit  15  R    L1RPP_STATUS       |
 | bit  13  R    SDET               |
 | bit  12  R    ENCDET             |
 | bit  11  R/W  AUTO_SENSE         |
 | bit  10  R/W  MANUAL_ACK_EN      |
 | bit  9   R/W  AN_ABLTY_ADV_UPD   |
 | bit  6:4 R/W  MODE               |
 | bit  0   R/W  L1RPP_EN           |
 +---------------------------------*/
#define PMC_L1RPP_FEGE_REG_CFG_AND_STAT_UNUSED_MASK          0xffff418e
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_STATUS_MSK     0x00008000
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_STATUS_OFF     15
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_SDET_MSK             0x00002000
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_SDET_OFF             13
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_ENCDET_MSK           0x00001000
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_ENCDET_OFF           12
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AUTO_SENSE_MSK       0x00000800
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AUTO_SENSE_OFF       11
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MANUAL_ACK_EN_MSK    0x00000400
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MANUAL_ACK_EN_OFF    10
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AN_ABLTY_ADV_UPD_MSK 0x00000200
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_AN_ABLTY_ADV_UPD_OFF 9
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_MSK             0x00000070
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_MODE_OFF             4
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_EN_MSK         0x00000001
#define L1RPP_FEGE_REG_CFG_AND_STAT_BIT_L1RPP_EN_OFF         0

/*-----------------------------------------.
 | Register 0x00000090 LINK_SYNC_SM_CONFIG |
 +-----------------------------------------+
 | bit  14   R/W  LS_RESET                 |
 | bit  11:8 R/W  RESYNC_COUNT             |
 | bit  7:4  R/W  LOS_COUNT                |
 | bit  3:0  R/W  SYNC_COUNT               |
 +----------------------------------------*/
#define PMC_L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_UNUSED_MASK      0xffffb000
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LS_RESET_MSK     0x00004000
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LS_RESET_OFF     14
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_MSK 0x00000f00
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_RESYNC_COUNT_OFF 8
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_MSK    0x000000f0
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_LOS_COUNT_OFF    4
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_MSK   0x0000000f
#define L1RPP_FEGE_REG_LINK_SYNC_SM_CONFIG_BIT_SYNC_COUNT_OFF   0

/*----------------------------------------------------.
 | Register 0x00000094 JITTER_PATTERN_STAT_AND_CONFIG |
 +----------------------------------------------------+
 | bit  15    R    R_PRBS_IND                         |
 | bit  14    R    PRBS_IND                           |
 | bit  13    R    R_PTRN_IND                         |
 | bit  12    R    PTRN_IND                           |
 | bit  11:10 R/W  PTRN_SEL                           |
 | bit  9:0   R/W  CUS_PTRN                           |
 +---------------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_UNUSED_MASK    0xffff0000
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PRBS_IND_MSK 0x00008000
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PRBS_IND_OFF 15
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PRBS_IND_MSK   0x00004000
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PRBS_IND_OFF   14
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PTRN_IND_MSK 0x00002000
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_R_PTRN_IND_OFF 13
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_IND_MSK   0x00001000
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_IND_OFF   12
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_MSK   0x00000c00
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_PTRN_SEL_OFF   10
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_MSK   0x000003ff
#define L1RPP_FEGE_REG_JITTER_PATTERN_STAT_AND_CONFIG_BIT_CUS_PTRN_OFF   0

/*------------------------------.
 | Register 0x00000098 MII_CTRL |
 +------------------------------+
 | bit  12 R/W  AN_ENABLE       |
 | bit  9  R/W  AN_RESTART      |
 +-----------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_CTRL_UNUSED_MASK    0xffffedff
#define L1RPP_FEGE_REG_MII_CTRL_BIT_AN_ENABLE_MSK  0x00001000
#define L1RPP_FEGE_REG_MII_CTRL_BIT_AN_ENABLE_OFF  12
#define L1RPP_FEGE_REG_MII_CTRL_BIT_AN_RESTART_MSK 0x00000200
#define L1RPP_FEGE_REG_MII_CTRL_BIT_AN_RESTART_OFF 9

/*------------------------------.
 | Register 0x0000009c MII_STAT |
 +------------------------------+
 | bit  5 R  AN_COMPLETE        |
 | bit  4 R  REM_FLT_IND        |
 | bit  2 R  LNK_STATE          |
 +-----------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_STAT_UNUSED_MASK     0xffffffcb
#define L1RPP_FEGE_REG_MII_STAT_BIT_AN_COMPLETE_MSK 0x00000020
#define L1RPP_FEGE_REG_MII_STAT_BIT_AN_COMPLETE_OFF 5
#define L1RPP_FEGE_REG_MII_STAT_BIT_REM_FLT_IND_MSK 0x00000010
#define L1RPP_FEGE_REG_MII_STAT_BIT_REM_FLT_IND_OFF 4
#define L1RPP_FEGE_REG_MII_STAT_BIT_LNK_STATE_MSK   0x00000004
#define L1RPP_FEGE_REG_MII_STAT_BIT_LNK_STATE_OFF   2

/*--------------------------------------------------------.
 | Register 0x000000a0 MII_AUTO_NEGOTIATION_ADVERTISEMENT |
 +--------------------------------------------------------+
 | bit  15:0 R/W  AN_ABLTY_ADV                            |
 +-------------------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_UNUSED_MASK      0xffff0000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_MSK 0x0000ffff
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_ADVERTISEMENT_BIT_AN_ABLTY_ADV_OFF 0

/*-------------------------------------------------------------------------.
 | Register 0x000000a4 MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY |
 +-------------------------------------------------------------------------+
 | bit  15:0 R  AN_LP_ABLTY                                                |
 +------------------------------------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_UNUSED_MASK     0xffff0000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_MSK 0x0000ffff
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_BIT_AN_LP_ABLTY_OFF 0

/*----------------------------------------------------.
 | Register 0x000000a8 MII_AUTO_NEGOTIATION_EXPANSION |
 +----------------------------------------------------+
 | bit  1 R  RX_PAGE                                  |
 | bit  0 R  RESOLVE_PRIORITY                         |
 +---------------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_UNUSED_MASK          0xfffffffc
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RX_PAGE_MSK          0x00000002
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RX_PAGE_OFF          1
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RESOLVE_PRIORITY_MSK 0x00000001
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_EXPANSION_BIT_RESOLVE_PRIORITY_OFF 0

/*-------------------------------------------------------.
 | Register 0x000000ac MII_AUTO_NEGOTIATION_NEXT_PAGE_TX |
 +-------------------------------------------------------+
 | bit  15   R/W  LOC_NP_NEXT_PAGE                       |
 | bit  13   R/W  LOC_NP_MSG_PAGE                        |
 | bit  12   R/W  LOC_NP_ACK2                            |
 | bit  11   R    LOC_NP_TOGGLE                          |
 | bit  10:0 R/W  LOC_NP_CODE_FIELD                      |
 +------------------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_UNUSED_MASK           0xffff4000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_NEXT_PAGE_MSK  0x00008000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_NEXT_PAGE_OFF  15
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_MSG_PAGE_MSK   0x00002000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_MSG_PAGE_OFF   13
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_ACK2_MSK       0x00001000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_ACK2_OFF       12
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_TOGGLE_MSK     0x00000800
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_TOGGLE_OFF     11
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_MSK 0x000007ff
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_BIT_LOC_NP_CODE_FIELD_OFF 0

/*----------------------------------------------------------------------.
 | Register 0x000000b0 MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE |
 +----------------------------------------------------------------------+
 | bit  15   R  LNK_NP_NEXT_PAGE                                        |
 | bit  14   R  LNK_NP_ACK                                              |
 | bit  13   R  LNK_NP_MSG_PAGE                                         |
 | bit  12   R  LNK_NP_ACK2                                             |
 | bit  11   R  LNK_NP_TOGGLE                                           |
 | bit  10:0 R  LNK_NP_CODE_FIELD                                       |
 +---------------------------------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_UNUSED_MASK           0xffff0000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_NEXT_PAGE_MSK  0x00008000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_NEXT_PAGE_OFF  15
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK_MSK        0x00004000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK_OFF        14
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_MSG_PAGE_MSK   0x00002000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_MSG_PAGE_OFF   13
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK2_MSK       0x00001000
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_ACK2_OFF       12
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_TOGGLE_MSK     0x00000800
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_TOGGLE_OFF     11
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_MSK 0x000007ff
#define L1RPP_FEGE_REG_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_BIT_LNK_NP_CODE_FIELD_OFF 0

/*-----------------------------------------------.
 | Register 0x000000bc DEBUG_LINK_TIMER_OVERRIDE |
 +-----------------------------------------------+
 | bit  15:0 R/W  LINK_TIMER                     |
 +----------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_UNUSED_MASK    0xffff0000
#define L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_MSK 0x0000ffff
#define L1RPP_FEGE_REG_DEBUG_LINK_TIMER_OVERRIDE_BIT_LINK_TIMER_OFF 0

/*----------------------------------------.
 | Register 0x000000c0 LINK_TIME_DURATION |
 +----------------------------------------+
 | bit  13:0 R/W  LINK_TIME_DUR           |
 +---------------------------------------*/
#define PMC_L1RPP_FEGE_REG_LINK_TIME_DURATION_UNUSED_MASK       0xffffc000
#define L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_MSK 0x00003fff
#define L1RPP_FEGE_REG_LINK_TIME_DURATION_BIT_LINK_TIME_DUR_OFF 0

/*-------------------------------------.
 | Register 0x000000c4 SGMII_GMII_MODE |
 +-------------------------------------+
 | bit  15 R/W  MODE_SEL               |
 +------------------------------------*/
#define PMC_L1RPP_FEGE_REG_SGMII_GMII_MODE_UNUSED_MASK  0xffff7fff
#define L1RPP_FEGE_REG_SGMII_GMII_MODE_BIT_MODE_SEL_MSK 0x00008000
#define L1RPP_FEGE_REG_SGMII_GMII_MODE_BIT_MODE_SEL_OFF 15

/*-------------------------------.
 | Register 0x000000c8 COMMA_DET |
 +-------------------------------+
 | bit  0 R/W  EN_CDET_SEL       |
 +------------------------------*/
#define PMC_L1RPP_FEGE_REG_COMMA_DET_UNUSED_MASK     0xfffffeee
#define L1RPP_FEGE_REG_COMMA_DET_BIT_EN_CDET_SEL_MSK 0x00000001
#define L1RPP_FEGE_REG_COMMA_DET_BIT_EN_CDET_SEL_OFF 0

/*--------------------------------.
 | Register 0x000000cc PMON_CNT_0 |
 +--------------------------------+
 | bit  15:0 R  PMON_CNTR         |
 +-------------------------------*/
#define PMC_L1RPP_FEGE_REG_PMON_CNT_0_UNUSED_MASK   0xffff0000
#define L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_MSK 0x0000ffff
#define L1RPP_FEGE_REG_PMON_CNT_0_BIT_PMON_CNTR_OFF 0

/*--------------------------------.
 | Register 0x000000d0 PMON_CNT_1 |
 +--------------------------------+
 | bit  15:0 R  PMON_CNTR         |
 +-------------------------------*/
#define PMC_L1RPP_FEGE_REG_PMON_CNT_1_UNUSED_MASK   0xffff0000
#define L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_MSK 0x0000ffff
#define L1RPP_FEGE_REG_PMON_CNT_1_BIT_PMON_CNTR_OFF 0

/*--------------------------------.
 | Register 0x000000d4 PMON_CNT_2 |
 +--------------------------------+
 | bit  7:0 R  PMON_CNTR          |
 +-------------------------------*/
#define PMC_L1RPP_FEGE_REG_PMON_CNT_2_UNUSED_MASK   0xffffff00
#define L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_MSK 0x000000ff
#define L1RPP_FEGE_REG_PMON_CNT_2_BIT_PMON_CNTR_OFF 0

/*---------------------------------.
 | Register 0x000000d8 PMON_UPDATE |
 +---------------------------------+
 | bit  0 R/W  PMON_UPDATE         |
 +--------------------------------*/
#define PMC_L1RPP_FEGE_REG_PMON_UPDATE_UNUSED_MASK     0xfffffffe
#define L1RPP_FEGE_REG_PMON_UPDATE_BIT_PMON_UPDATE_MSK 0x00000001
#define L1RPP_FEGE_REG_PMON_UPDATE_BIT_PMON_UPDATE_OFF 0

/*-----------------------------------.
 | Register 0x000000dc BIT_ALIGNMENT |
 +-----------------------------------+
 | bit  3:0 R  BIT_ALIGN             |
 +----------------------------------*/
#define PMC_L1RPP_FEGE_REG_BIT_ALIGNMENT_UNUSED_MASK   0xfffffff0
#define L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_MSK 0x0000000f
#define L1RPP_FEGE_REG_BIT_ALIGNMENT_BIT_BIT_ALIGN_OFF 0

/*--------------------------------------------.
 | Register 0x000000e0 MIN_LINK_TIME_DURATION |
 +--------------------------------------------+
 | bit  13:0 R/W  MIN_LINK_TIME_DUR           |
 +-------------------------------------------*/
#define PMC_L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_UNUSED_MASK           0xffffc000
#define L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_MSK 0x00003fff
#define L1RPP_FEGE_REG_MIN_LINK_TIME_DURATION_BIT_MIN_LINK_TIME_DUR_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _L1RPP_FEGE_REGS_H */
