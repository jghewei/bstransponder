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
 *     enet_fege block
 *****************************************************************************/
#ifndef _ENET_FEGE_REGS_H
#define _ENET_FEGE_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ENET_FEGE_REG_MODE_CFG                                            0x00000000
#define PMC_ENET_FEGE_REG_SW_RESET                                            0x00000004
#define PMC_ENET_FEGE_REG_PTP_NTP_CFG                                         0x00000008
#define PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET      0x0000000c
#define PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET       0x00000010
#define PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET      0x00000014
#define PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET 0x00000018
#define PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT                            0x0000001c
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG                                   0x00000020
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS                                0x00000024
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP         0x00000028
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA                           0x0000002c
#define PMC_ENET_FEGE_REG_INT_EN                                              0x00000030
#define PMC_ENET_FEGE_REG_INT_VAL                                             0x00000034
#define PMC_ENET_FEGE_REG_INT                                                 0x00000038
#define PMC_ENET_FEGE_REG_INT_INTP_EN                                         0x0000003c
#define PMC_ENET_FEGE_REG_INT_INTM_EN                                         0x00000040
#define PMC_ENET_FEGE_REG_INT_INTD_EN                                         0x00000044
#define PMC_ENET_FEGE_REG_INT_SUMMARY                                         0x00000048
#define PMC_ENET_FEGE_REG_TIP_SUMMARY                                         0x0000004c
#define PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG                           0x00000058

/*-------------------------------------------.
 | Register 0x00000000 MODE_CFG              |
 +-------------------------------------------+
 | bit  31    R/W  TSTMP_CORR_UPD_BYPASS     |
 | bit  29:25 R/W  TSTMP_CORR_UPD_SKIP_BYTES |
 | bit  20    R/W  AUTO_ERR_CLEAR_EN         |
 | bit  19    R/W  FORCE_ERR_PATT            |
 | bit  18    R/W  CLEAR_ERR_PATT            |
 | bit  17:16 R/W  IPG_RED_ON_THRESH         |
 | bit  15:14 R/W  IPG_RED_OFF_THRESH        |
 | bit  13:12 R/W  PAUSE_ON_THRESH           |
 | bit  11:10 R/W  PAUSE_OFF_THRESH          |
 | bit  9     R/W  MAC_LOWPWR                |
 | bit  8     R/W  GE_GFPT_LOWPWR            |
 | bit  7     R    L1RPP_FEGE_CARRIER_SENSE  |
 | bit  4     R    L1RPP_FEGE_RXERROR_STATUS |
 | bit  3     R/W  UNIDIRECTIONAL_MODE       |
 | bit  2:0   R/W  FEGE_MODE                 |
 +------------------------------------------*/
#define PMC_ENET_FEGE_REG_MODE_CFG_UNUSED_MASK                   0x01c00000
#define ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_BYPASS_MSK     0x80000000
#define ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_BYPASS_OFF     31
#define ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_MSK 0x3e000000
#define ENET_FEGE_REG_MODE_CFG_BIT_TSTMP_CORR_UPD_SKIP_BYTES_OFF 25
#define ENET_FEGE_REG_MODE_CFG_BIT_AUTO_ERR_CLEAR_EN_MSK         0x00100000
#define ENET_FEGE_REG_MODE_CFG_BIT_AUTO_ERR_CLEAR_EN_OFF         20
#define ENET_FEGE_REG_MODE_CFG_BIT_FORCE_ERR_PATT_MSK            0x00080000
#define ENET_FEGE_REG_MODE_CFG_BIT_FORCE_ERR_PATT_OFF            19
#define ENET_FEGE_REG_MODE_CFG_BIT_CLEAR_ERR_PATT_MSK            0x00040000
#define ENET_FEGE_REG_MODE_CFG_BIT_CLEAR_ERR_PATT_OFF            18
#define ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_MSK         0x00030000
#define ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_ON_THRESH_OFF         16
#define ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_MSK        0x0000c000
#define ENET_FEGE_REG_MODE_CFG_BIT_IPG_RED_OFF_THRESH_OFF        14
#define ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_MSK           0x00003000
#define ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_ON_THRESH_OFF           12
#define ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_MSK          0x00000c00
#define ENET_FEGE_REG_MODE_CFG_BIT_PAUSE_OFF_THRESH_OFF          10
#define ENET_FEGE_REG_MODE_CFG_BIT_MAC_LOWPWR_MSK                0x00000200
#define ENET_FEGE_REG_MODE_CFG_BIT_MAC_LOWPWR_OFF                9
#define ENET_FEGE_REG_MODE_CFG_BIT_GE_GFPT_LOWPWR_MSK            0x00000100
#define ENET_FEGE_REG_MODE_CFG_BIT_GE_GFPT_LOWPWR_OFF            8
#define ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_CARRIER_SENSE_MSK  0x00000080
#define ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_CARRIER_SENSE_OFF  7
#define ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_RXERROR_STATUS_MSK 0x00000010
#define ENET_FEGE_REG_MODE_CFG_BIT_L1RPP_FEGE_RXERROR_STATUS_OFF 4
#define ENET_FEGE_REG_MODE_CFG_BIT_UNIDIRECTIONAL_MODE_MSK       0x00000008
#define ENET_FEGE_REG_MODE_CFG_BIT_UNIDIRECTIONAL_MODE_OFF       3
#define ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_MSK                 0x00000007
#define ENET_FEGE_REG_MODE_CFG_BIT_FEGE_MODE_OFF                 0

/*--------------------------------.
 | Register 0x00000004 SW_RESET   |
 +--------------------------------+
 | bit  10 R/W  TX_TSTMP_FIFO_RST |
 | bit  9  R/W  PTP_NTP_RST       |
 | bit  8  R/W  GE_GFPT_RST       |
 | bit  7  R/W  MSTATX_RST        |
 | bit  6  R/W  L1RPP_FEGE_RST    |
 | bit  5  R/W  RMAC_RST          |
 | bit  4  R/W  RX_ECLASS_RST     |
 | bit  3  R/W  L1TPP_FEGE_RST    |
 | bit  2  R/W  TMAC_RST          |
 | bit  1  R/W  TX_ECLASS_RST     |
 | bit  0  R/W  ENET_FEGE_RST     |
 +-------------------------------*/
#define PMC_ENET_FEGE_REG_SW_RESET_UNUSED_MASK           0x3ffff800
#define ENET_FEGE_REG_SW_RESET_BIT_TX_TSTMP_FIFO_RST_MSK 0x00000400
#define ENET_FEGE_REG_SW_RESET_BIT_TX_TSTMP_FIFO_RST_OFF 10
#define ENET_FEGE_REG_SW_RESET_BIT_PTP_NTP_RST_MSK       0x00000200
#define ENET_FEGE_REG_SW_RESET_BIT_PTP_NTP_RST_OFF       9
#define ENET_FEGE_REG_SW_RESET_BIT_GE_GFPT_RST_MSK       0x00000100
#define ENET_FEGE_REG_SW_RESET_BIT_GE_GFPT_RST_OFF       8
#define ENET_FEGE_REG_SW_RESET_BIT_MSTATX_RST_MSK        0x00000080
#define ENET_FEGE_REG_SW_RESET_BIT_MSTATX_RST_OFF        7
#define ENET_FEGE_REG_SW_RESET_BIT_L1RPP_FEGE_RST_MSK    0x00000040
#define ENET_FEGE_REG_SW_RESET_BIT_L1RPP_FEGE_RST_OFF    6
#define ENET_FEGE_REG_SW_RESET_BIT_RMAC_RST_MSK          0x00000020
#define ENET_FEGE_REG_SW_RESET_BIT_RMAC_RST_OFF          5
#define ENET_FEGE_REG_SW_RESET_BIT_RX_ECLASS_RST_MSK     0x00000010
#define ENET_FEGE_REG_SW_RESET_BIT_RX_ECLASS_RST_OFF     4
#define ENET_FEGE_REG_SW_RESET_BIT_L1TPP_FEGE_RST_MSK    0x00000008
#define ENET_FEGE_REG_SW_RESET_BIT_L1TPP_FEGE_RST_OFF    3
#define ENET_FEGE_REG_SW_RESET_BIT_TMAC_RST_MSK          0x00000004
#define ENET_FEGE_REG_SW_RESET_BIT_TMAC_RST_OFF          2
#define ENET_FEGE_REG_SW_RESET_BIT_TX_ECLASS_RST_MSK     0x00000002
#define ENET_FEGE_REG_SW_RESET_BIT_TX_ECLASS_RST_OFF     1
#define ENET_FEGE_REG_SW_RESET_BIT_ENET_FEGE_RST_MSK     0x00000001
#define ENET_FEGE_REG_SW_RESET_BIT_ENET_FEGE_RST_OFF     0

/*-----------------------------------------.
 | Register 0x00000008 PTP_NTP_CFG         |
 +-----------------------------------------+
 | bit  31    R/W  RX_TSTMP_OFFSET_UPD     |
 | bit  30    R/W  TX_TSTMP_OFFSET_UPD     |
 | bit  29    R/W  RX_TSTMP_FRAC_STAT_SIGN |
 | bit  28    R/W  RX_TSTMP_FRAC_DYN_SIGN  |
 | bit  27    R/W  TX_TSTMP_FRAC_STAT_SIGN |
 | bit  26    R/W  TX_TSTMP_FRAC_DYN_SIGN  |
 | bit  23:16 R/W  CORR_POSITION           |
 | bit  15:8  R/W  TSTMP_POSITION          |
 | bit  7:5   R/W  TAG_FOR_UPD             |
 | bit  4     R/W  ONE_STEP_CLK_FCS        |
 | bit  2:1   R/W  TSTMP_MODE              |
 | bit  0     R/W  PTP_NTP_EN              |
 +----------------------------------------*/
#define PMC_ENET_FEGE_REG_PTP_NTP_CFG_UNUSED_MASK                 0x03000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_OFFSET_UPD_MSK     0x80000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_OFFSET_UPD_OFF     31
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_OFFSET_UPD_MSK     0x40000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_OFFSET_UPD_OFF     30
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_STAT_SIGN_MSK 0x20000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_STAT_SIGN_OFF 29
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_DYN_SIGN_MSK  0x10000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_RX_TSTMP_FRAC_DYN_SIGN_OFF  28
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_STAT_SIGN_MSK 0x08000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_STAT_SIGN_OFF 27
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_DYN_SIGN_MSK  0x04000000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TX_TSTMP_FRAC_DYN_SIGN_OFF  26
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_MSK           0x00ff0000
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_CORR_POSITION_OFF           16
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_MSK          0x0000ff00
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_POSITION_OFF          8
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_MSK             0x000000e0
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TAG_FOR_UPD_OFF             5
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_ONE_STEP_CLK_FCS_MSK        0x00000010
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_ONE_STEP_CLK_FCS_OFF        4
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_MSK              0x00000006
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_TSTMP_MODE_OFF              1
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_PTP_NTP_EN_MSK              0x00000001
#define ENET_FEGE_REG_PTP_NTP_CFG_BIT_PTP_NTP_EN_OFF              0

/*--------------------------------------------------------------------.
 | Register 0x0000000c PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET |
 +--------------------------------------------------------------------+
 | bit  31:0 R/W  RX_TSTMP_FRAC_DYN_OFFSET                            |
 +-------------------------------------------------------------------*/
#define PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_UNUSED_MASK                  0x00000000
#define ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_MSK 0xffffffff
#define ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_RX_TSTMP_FRAC_DYN_OFFSET_OFF 0

/*-------------------------------------------------------------------.
 | Register 0x00000010 PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET |
 +-------------------------------------------------------------------+
 | bit  31:0 R/W  RX_TSTMP_FRAC_STAT_OFFSET                          |
 +------------------------------------------------------------------*/
#define PMC_ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_UNUSED_MASK                   0x00000000
#define ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_MSK 0xffffffff
#define ENET_FEGE_REG_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_RX_TSTMP_FRAC_STAT_OFFSET_OFF 0

/*--------------------------------------------------------------------.
 | Register 0x00000014 PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET |
 +--------------------------------------------------------------------+
 | bit  31:0 R/W  TX_TSTMP_FRAC_DYN_OFFSET                            |
 +-------------------------------------------------------------------*/
#define PMC_ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_UNUSED_MASK                  0x00000000
#define ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_MSK 0xffffffff
#define ENET_FEGE_REG_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_BIT_TX_TSTMP_FRAC_DYN_OFFSET_OFF 0

/*-------------------------------------------------------------------------.
 | Register 0x00000018 PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET |
 +-------------------------------------------------------------------------+
 | bit  31:0 R/W  TX_TSTMP_FRAC_STAT_OFFSET                                |
 +------------------------------------------------------------------------*/
#define PMC_ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_UNUSED_MASK                   0x00000000
#define ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_MSK 0xffffffff
#define ENET_FEGE_REG_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_BIT_TX_TSTMP_FRAC_STAT_OFFSET_OFF 0

/*----------------------------------------------.
 | Register 0x0000001c L1RPP_FEGE_BIT_ALIGNMENT |
 +----------------------------------------------+
 | bit  4   R/W  AUTO_BIT_ALIGN_OFFSET          |
 | bit  3:0 R    L1RPP_FEGE_BIT_ALIGN           |
 +---------------------------------------------*/
#define PMC_ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_UNUSED_MASK               0xffffffe0
#define ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_AUTO_BIT_ALIGN_OFFSET_MSK 0x00000010
#define ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_AUTO_BIT_ALIGN_OFFSET_OFF 4
#define ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_MSK  0x0000000f
#define ENET_FEGE_REG_L1RPP_FEGE_BIT_ALIGNMENT_BIT_L1RPP_FEGE_BIT_ALIGN_OFF  0

/*---------------------------------------.
 | Register 0x00000020 TX_TSTMP_FIFO_CFG |
 +---------------------------------------+
 | bit  8   R/W  ERR_PKT_DROP            |
 | bit  7:0 R/W  EXTRACT_TAG             |
 +--------------------------------------*/
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_UNUSED_MASK      0x1ffffe00
#define ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_ERR_PKT_DROP_MSK 0x00000100
#define ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_ERR_PKT_DROP_OFF 8
#define ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_MSK  0x000000ff
#define ENET_FEGE_REG_TX_TSTMP_FIFO_CFG_BIT_EXTRACT_TAG_OFF  0

/*------------------------------------------.
 | Register 0x00000024 TX_TSTMP_FIFO_ACCESS |
 +------------------------------------------+
 | bit  1 R    TX_TSTMP_PKT_AVAIL           |
 | bit  0 R/W  Q_BUSY_RD                    |
 +-----------------------------------------*/
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_UNUSED_MASK            0xfffffffc
#define ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_TX_TSTMP_PKT_AVAIL_MSK 0x00000002
#define ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_TX_TSTMP_PKT_AVAIL_OFF 1
#define ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_MSK          0x00000001
#define ENET_FEGE_REG_TX_TSTMP_FIFO_ACCESS_BIT_Q_BUSY_RD_OFF          0

/*-----------------------------------------------------------------.
 | Register 0x00000028 TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP |
 +-----------------------------------------------------------------+
 | bit  31:24 R  TOD_TSTMP                                         |
 | bit  5     R  Q_SOP                                             |
 | bit  4:1   R  Q_EOP                                             |
 | bit  0     R  Q_ERR                                             |
 +----------------------------------------------------------------*/
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_UNUSED_MASK   0x00ffffc0
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_MSK 0xff000000
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_TOD_TSTMP_OFF 24
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_SOP_MSK     0x00000020
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_SOP_OFF     5
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_MSK     0x0000001e
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_EOP_OFF     1
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_ERR_MSK     0x00000001
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_BIT_Q_ERR_OFF     0

/*-----------------------------------------------.
 | Register 0x0000002c TX_TSTMP_FIFO_PACKET_DATA |
 +-----------------------------------------------+
 | bit  31:0 R  Q_DATA                           |
 +----------------------------------------------*/
#define PMC_ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_UNUSED_MASK 0x00000000
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_MSK  0xffffffff
#define ENET_FEGE_REG_TX_TSTMP_FIFO_PACKET_DATA_BIT_Q_DATA_OFF  0

/*----------------------------------------.
 | Register 0x00000030 INT_EN             |
 +----------------------------------------+
 | bit  6 R/W  TX_TSTMPR_TOD_SIGN_OVRFL_E |
 | bit  5 R/W  RX_TSTMPR_TOD_SIGN_OVRFL_E |
 | bit  4 R/W  OVERFLOW_E                 |
 | bit  3 R/W  RX_COMMA_DEL_CHG_E         |
 | bit  2 R/W  TX_TSTMP_FIFO_E            |
 | bit  1 R/W  TX_SOF_E                   |
 | bit  0 R/W  RX_SOF_E                   |
 +---------------------------------------*/
#define PMC_ENET_FEGE_REG_INT_EN_UNUSED_MASK                    0x0fffff80
#define ENET_FEGE_REG_INT_EN_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_E_MSK 0x00000040
#define ENET_FEGE_REG_INT_EN_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_E_OFF 6
#define ENET_FEGE_REG_INT_EN_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_E_MSK 0x00000020
#define ENET_FEGE_REG_INT_EN_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_E_OFF 5
#define ENET_FEGE_REG_INT_EN_BIT_OVERFLOW_E_MSK                 0x00000010
#define ENET_FEGE_REG_INT_EN_BIT_OVERFLOW_E_OFF                 4
#define ENET_FEGE_REG_INT_EN_BIT_RX_COMMA_DEL_CHG_E_MSK         0x00000008
#define ENET_FEGE_REG_INT_EN_BIT_RX_COMMA_DEL_CHG_E_OFF         3
#define ENET_FEGE_REG_INT_EN_BIT_TX_TSTMP_FIFO_E_MSK            0x00000004
#define ENET_FEGE_REG_INT_EN_BIT_TX_TSTMP_FIFO_E_OFF            2
#define ENET_FEGE_REG_INT_EN_BIT_TX_SOF_E_MSK                   0x00000002
#define ENET_FEGE_REG_INT_EN_BIT_TX_SOF_E_OFF                   1
#define ENET_FEGE_REG_INT_EN_BIT_RX_SOF_E_MSK                   0x00000001
#define ENET_FEGE_REG_INT_EN_BIT_RX_SOF_E_OFF                   0

/*--------------------------------------.
 | Register 0x00000034 INT_VAL          |
 +--------------------------------------+
 | bit  6 R  TX_TSTMPR_TOD_SIGN_OVRFL_V |
 | bit  5 R  RX_TSTMPR_TOD_SIGN_OVRFL_V |
 +-------------------------------------*/
#define PMC_ENET_FEGE_REG_INT_VAL_UNUSED_MASK                    0xffffff9f
#define ENET_FEGE_REG_INT_VAL_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_V_MSK 0x00000040
#define ENET_FEGE_REG_INT_VAL_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_V_OFF 6
#define ENET_FEGE_REG_INT_VAL_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_V_MSK 0x00000020
#define ENET_FEGE_REG_INT_VAL_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_V_OFF 5

/*----------------------------------------.
 | Register 0x00000038 INT                |
 +----------------------------------------+
 | bit  6 R/W  TX_TSTMPR_TOD_SIGN_OVRFL_I |
 | bit  5 R/W  RX_TSTMPR_TOD_SIGN_OVRFL_I |
 | bit  4 R/W  OVERFLOW_I                 |
 | bit  3 R/W  RX_COMMA_DEL_CHG_I         |
 | bit  2 R/W  TX_TSTMP_FIFO_I            |
 | bit  1 R/W  TX_SOF_I                   |
 | bit  0 R/W  RX_SOF_I                   |
 +---------------------------------------*/
#define PMC_ENET_FEGE_REG_INT_UNUSED_MASK                    0x0fffff80
#define ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_MSK 0x00000040
#define ENET_FEGE_REG_INT_BIT_TX_TSTMPR_TOD_SIGN_OVRFL_I_OFF 6
#define ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_MSK 0x00000020
#define ENET_FEGE_REG_INT_BIT_RX_TSTMPR_TOD_SIGN_OVRFL_I_OFF 5
#define ENET_FEGE_REG_INT_BIT_OVERFLOW_I_MSK                 0x00000010
#define ENET_FEGE_REG_INT_BIT_OVERFLOW_I_OFF                 4
#define ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_MSK         0x00000008
#define ENET_FEGE_REG_INT_BIT_RX_COMMA_DEL_CHG_I_OFF         3
#define ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_MSK            0x00000004
#define ENET_FEGE_REG_INT_BIT_TX_TSTMP_FIFO_I_OFF            2
#define ENET_FEGE_REG_INT_BIT_TX_SOF_I_MSK                   0x00000002
#define ENET_FEGE_REG_INT_BIT_TX_SOF_I_OFF                   1
#define ENET_FEGE_REG_INT_BIT_RX_SOF_I_MSK                   0x00000001
#define ENET_FEGE_REG_INT_BIT_RX_SOF_I_OFF                   0

/*---------------------------------.
 | Register 0x0000003c INT_INTP_EN |
 +---------------------------------+
 | bit  3 R/W  FEGE_P_E            |
 | bit  2 R/W  MSTATX_P_E          |
 | bit  0 R/W  L1RPP_FEGE_P_E      |
 +--------------------------------*/
#define PMC_ENET_FEGE_REG_INT_INTP_EN_UNUSED_MASK        0x7ffffff0
#define ENET_FEGE_REG_INT_INTP_EN_BIT_FEGE_P_E_MSK       0x00000008
#define ENET_FEGE_REG_INT_INTP_EN_BIT_FEGE_P_E_OFF       3
#define ENET_FEGE_REG_INT_INTP_EN_BIT_MSTATX_P_E_MSK     0x00000004
#define ENET_FEGE_REG_INT_INTP_EN_BIT_MSTATX_P_E_OFF     2
#define ENET_FEGE_REG_INT_INTP_EN_BIT_L1RPP_FEGE_P_E_MSK 0x00000001
#define ENET_FEGE_REG_INT_INTP_EN_BIT_L1RPP_FEGE_P_E_OFF 0

/*---------------------------------.
 | Register 0x00000040 INT_INTM_EN |
 +---------------------------------+
 | bit  3 R/W  FEGE_M_E            |
 | bit  2 R/W  MSTATX_M_E          |
 | bit  0 R/W  L1RPP_FEGE_M_E      |
 +--------------------------------*/
#define PMC_ENET_FEGE_REG_INT_INTM_EN_UNUSED_MASK        0xfffffff0
#define ENET_FEGE_REG_INT_INTM_EN_BIT_FEGE_M_E_MSK       0x00000008
#define ENET_FEGE_REG_INT_INTM_EN_BIT_FEGE_M_E_OFF       3
#define ENET_FEGE_REG_INT_INTM_EN_BIT_MSTATX_M_E_MSK     0x00000004
#define ENET_FEGE_REG_INT_INTM_EN_BIT_MSTATX_M_E_OFF     2
#define ENET_FEGE_REG_INT_INTM_EN_BIT_L1RPP_FEGE_M_E_MSK 0x00000001
#define ENET_FEGE_REG_INT_INTM_EN_BIT_L1RPP_FEGE_M_E_OFF 0

/*---------------------------------.
 | Register 0x00000044 INT_INTD_EN |
 +---------------------------------+
 | bit  3 R/W  FEGE_D_E            |
 | bit  2 R/W  MSTATX_D_E          |
 | bit  0 R/W  L1RPP_FEGE_D_E      |
 +--------------------------------*/
#define PMC_ENET_FEGE_REG_INT_INTD_EN_UNUSED_MASK        0xfffffff0
#define ENET_FEGE_REG_INT_INTD_EN_BIT_FEGE_D_E_MSK       0x00000008
#define ENET_FEGE_REG_INT_INTD_EN_BIT_FEGE_D_E_OFF       3
#define ENET_FEGE_REG_INT_INTD_EN_BIT_MSTATX_D_E_MSK     0x00000004
#define ENET_FEGE_REG_INT_INTD_EN_BIT_MSTATX_D_E_OFF     2
#define ENET_FEGE_REG_INT_INTD_EN_BIT_L1RPP_FEGE_D_E_MSK 0x00000001
#define ENET_FEGE_REG_INT_INTD_EN_BIT_L1RPP_FEGE_D_E_OFF 0

/*---------------------------------.
 | Register 0x00000048 INT_SUMMARY |
 +---------------------------------+
 | bit  3 R  FEGE_INT              |
 | bit  2 R  MSTATX_INT            |
 | bit  0 R  L1RPP_FEGE_INT        |
 +--------------------------------*/
#define PMC_ENET_FEGE_REG_INT_SUMMARY_UNUSED_MASK        0xfffffff0
#define ENET_FEGE_REG_INT_SUMMARY_BIT_FEGE_INT_MSK       0x00000008
#define ENET_FEGE_REG_INT_SUMMARY_BIT_FEGE_INT_OFF       3
#define ENET_FEGE_REG_INT_SUMMARY_BIT_MSTATX_INT_MSK     0x00000004
#define ENET_FEGE_REG_INT_SUMMARY_BIT_MSTATX_INT_OFF     2
#define ENET_FEGE_REG_INT_SUMMARY_BIT_L1RPP_FEGE_INT_MSK 0x00000001
#define ENET_FEGE_REG_INT_SUMMARY_BIT_L1RPP_FEGE_INT_OFF 0

/*---------------------------------.
 | Register 0x0000004c TIP_SUMMARY |
 +---------------------------------+
 | bit  2 R  MSTATX_TIP            |
 | bit  0 R  L1RPP_FEGE_TIP        |
 +--------------------------------*/
#define PMC_ENET_FEGE_REG_TIP_SUMMARY_UNUSED_MASK        0xfffffff8
#define ENET_FEGE_REG_TIP_SUMMARY_BIT_MSTATX_TIP_MSK     0x00000004
#define ENET_FEGE_REG_TIP_SUMMARY_BIT_MSTATX_TIP_OFF     2
#define ENET_FEGE_REG_TIP_SUMMARY_BIT_L1RPP_FEGE_TIP_MSK 0x00000001
#define ENET_FEGE_REG_TIP_SUMMARY_BIT_L1RPP_FEGE_TIP_OFF 0

/*-----------------------------------------------.
 | Register 0x00000058 UDP_CHKSUM_CORRECTION_CFG |
 +-----------------------------------------------+
 | bit  11  R/W  UDP_EN                          |
 | bit  10  R/W  UDP_CORR_POS_EN                 |
 | bit  9   R/W  UDP_CORR_POS_UPD                |
 | bit  8:0 R/W  UDP_CORR_POS                    |
 +----------------------------------------------*/
#define PMC_ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_UNUSED_MASK          0xfffff000
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_EN_MSK           0x00000800
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_EN_OFF           11
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_EN_MSK  0x00000400
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_EN_OFF  10
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_UPD_MSK 0x00000200
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_UPD_OFF 9
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_MSK     0x000001ff
#define ENET_FEGE_REG_UDP_CHKSUM_CORRECTION_CFG_BIT_UDP_CORR_POS_OFF     0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ENET_FEGE_REGS_H */
