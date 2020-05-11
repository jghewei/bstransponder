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
 *     ge_gfpt block
 *****************************************************************************/
#ifndef _GE_GFPT_REGS_H
#define _GE_GFPT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_GE_GFPT_REG_GENERAL_CFG                                0x00002100
#define PMC_GE_GFPT_REG_RX_CFG                                     0x00002104
#define PMC_GE_GFPT_REG_RX_20_BIT_PATTERN                          0x00002108
#define PMC_GE_GFPT_REG_RX_LCV_CNT0                                0x00002110
#define PMC_GE_GFPT_REG_RX_LCV_CNT1                                0x00002114
#define PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT0                      0x00002118
#define PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT1                      0x0000211c
#define PMC_GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0             0x00002120
#define PMC_GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1                0x00002124
#define PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 0x00002128
#define PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 0x0000212c
#define PMC_GE_GFPT_REG_RX_PACKET_DELETED_CNT                      0x00002130
#define PMC_GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT             0x00002134
#define PMC_GE_GFPT_REG_RX_PMON_UPDATE                             0x00002138
#define PMC_GE_GFPT_REG_TX_CFG                                     0x0000213c
#define PMC_GE_GFPT_REG_TX_20_BIT_PATTERN                          0x00002140
#define PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL         0x00002144
#define PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL         0x00002148
#define PMC_GE_GFPT_REG_TX_LCV_CNT0                                0x0000214c
#define PMC_GE_GFPT_REG_TX_LCV_CNT1                                0x00002150
#define PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT0                      0x00002154
#define PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT1                      0x00002158
#define PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0             0x0000215c
#define PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1             0x00002160
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 0x00002164
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 0x00002168
#define PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT0                     0x0000216c
#define PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT1                     0x00002170
#define PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0   0x00002174
#define PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1   0x00002178
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0          0x0000217c
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1          0x00002180
#define PMC_GE_GFPT_REG_TX_PMON_UPDATE                             0x00002184
#define PMC_GE_GFPT_REG_DEFECT_STAT                                0x00002188
#define PMC_GE_GFPT_REG_INT_EN                                     0x0000218c
#define PMC_GE_GFPT_REG_INT                                        0x00002190

/*---------------------------------.
 | Register 0x00002100 GENERAL_CFG |
 +---------------------------------+
 | bit  0 R/W  SOFT_RESET          |
 +--------------------------------*/
#define PMC_GE_GFPT_REG_GENERAL_CFG_UNUSED_MASK    0xfffffffe
#define GE_GFPT_REG_GENERAL_CFG_BIT_SOFT_RESET_MSK 0x00000001
#define GE_GFPT_REG_GENERAL_CFG_BIT_SOFT_RESET_OFF 0

/*--------------------------------------.
 | Register 0x00002104 RX_CFG           |
 +--------------------------------------+
 | bit  24:21 R/W  RX_RESYNC_COUNT      |
 | bit  20:17 R/W  RX_LOS_COUNT         |
 | bit  16:13 R/W  RX_SYNC_COUNT        |
 | bit  12:3  R/W  RX_10BIT_PATTERN     |
 | bit  2     R/W  RX_10BIT_PATT_UPDATE |
 | bit  1     R/W  RX_10BIT_PATT_EN     |
 | bit  0     R/W  RX_SOFT_RESET        |
 +-------------------------------------*/
#define PMC_GE_GFPT_REG_RX_CFG_UNUSED_MASK              0xfe000000
#define GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_MSK      0x01e00000
#define GE_GFPT_REG_RX_CFG_BIT_RX_RESYNC_COUNT_OFF      21
#define GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_MSK         0x001e0000
#define GE_GFPT_REG_RX_CFG_BIT_RX_LOS_COUNT_OFF         17
#define GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_MSK        0x0001e000
#define GE_GFPT_REG_RX_CFG_BIT_RX_SYNC_COUNT_OFF        13
#define GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_MSK     0x00001ff8
#define GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATTERN_OFF     3
#define GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_UPDATE_MSK 0x00000004
#define GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_UPDATE_OFF 2
#define GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_EN_MSK     0x00000002
#define GE_GFPT_REG_RX_CFG_BIT_RX_10BIT_PATT_EN_OFF     1
#define GE_GFPT_REG_RX_CFG_BIT_RX_SOFT_RESET_MSK        0x00000001
#define GE_GFPT_REG_RX_CFG_BIT_RX_SOFT_RESET_OFF        0

/*---------------------------------------.
 | Register 0x00002108 RX_20_BIT_PATTERN |
 +---------------------------------------+
 | bit  21:2 R/W  RX_20BIT_PATTERN       |
 | bit  1    R/W  RX_20BIT_PATT_UPDATE   |
 | bit  0    R/W  RX_20BIT_PATT_EN       |
 +--------------------------------------*/
#define PMC_GE_GFPT_REG_RX_20_BIT_PATTERN_UNUSED_MASK              0xffc00000
#define GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_MSK     0x003ffffc
#define GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATTERN_OFF     2
#define GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_UPDATE_MSK 0x00000002
#define GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_UPDATE_OFF 1
#define GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_EN_MSK     0x00000001
#define GE_GFPT_REG_RX_20_BIT_PATTERN_BIT_RX_20BIT_PATT_EN_OFF     0

/*---------------------------------.
 | Register 0x00002110 RX_LCV_CNT0 |
 +---------------------------------+
 | bit  31:0 R  RXGE_GFPT_LCV_CNT  |
 +--------------------------------*/
#define PMC_GE_GFPT_REG_RX_LCV_CNT0_UNUSED_MASK           0x00000000
#define GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_MSK 0xffffffff
#define GE_GFPT_REG_RX_LCV_CNT0_BIT_RXGE_GFPT_LCV_CNT_OFF 0

/*---------------------------------.
 | Register 0x00002114 RX_LCV_CNT1 |
 +---------------------------------+
 | bit  7:0 R  RXGE_GFPT_LCV_CNT   |
 +--------------------------------*/
#define PMC_GE_GFPT_REG_RX_LCV_CNT1_UNUSED_MASK           0xffffff00
#define GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_MSK 0x000000ff
#define GE_GFPT_REG_RX_LCV_CNT1_BIT_RXGE_GFPT_LCV_CNT_OFF 0

/*-------------------------------------------.
 | Register 0x00002118 RX_IDLES_DELETED_CNT0 |
 +-------------------------------------------+
 | bit  31:0 R  RXGE_GFPT_IDLES_DEL_CNT      |
 +------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT0_UNUSED_MASK                 0x00000000
#define GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK 0xffffffff
#define GE_GFPT_REG_RX_IDLES_DELETED_CNT0_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF 0

/*-------------------------------------------.
 | Register 0x0000211c RX_IDLES_DELETED_CNT1 |
 +-------------------------------------------+
 | bit  7:0 R  RXGE_GFPT_IDLES_DEL_CNT       |
 +------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_IDLES_DELETED_CNT1_UNUSED_MASK                 0xffffff00
#define GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_MSK 0x000000ff
#define GE_GFPT_REG_RX_IDLES_DELETED_CNT1_BIT_RXGE_GFPT_IDLES_DEL_CNT_OFF 0

/*----------------------------------------------------.
 | Register 0x00002120 RX_CFG_ORDER_SETS_DELETED_CNT0 |
 +----------------------------------------------------+
 | bit  31:0 R  RXGE_GFPT_CONF_DEL_CNT                |
 +---------------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_UNUSED_MASK                0x00000000
#define GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK 0xffffffff
#define GE_GFPT_REG_RX_CFG_ORDER_SETS_DELETED_CNT0_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF 0

/*-------------------------------------------------.
 | Register 0x00002124 CFG_ORDER_SETS_DELETED_CNT1 |
 +-------------------------------------------------+
 | bit  7:0 R  RXGE_GFPT_CONF_DEL_CNT              |
 +------------------------------------------------*/
#define PMC_GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_UNUSED_MASK                0xffffff00
#define GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_MSK 0x000000ff
#define GE_GFPT_REG_CFG_ORDER_SETS_DELETED_CNT1_BIT_RXGE_GFPT_CONF_DEL_CNT_OFF 0

/*----------------------------------------------------------------.
 | Register 0x00002128 RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 |
 +----------------------------------------------------------------+
 | bit  31:0 R  RXGE_GFPT_PROPGERR_DEL_CNT                        |
 +---------------------------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_UNUSED_MASK                    0x00000000
#define GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK 0xffffffff
#define GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF 0

/*----------------------------------------------------------------.
 | Register 0x0000212c RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 |
 +----------------------------------------------------------------+
 | bit  7:0 R  RXGE_GFPT_PROPGERR_DEL_CNT                         |
 +---------------------------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_UNUSED_MASK                    0xffffff00
#define GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_MSK 0x000000ff
#define GE_GFPT_REG_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_RXGE_GFPT_PROPGERR_DEL_CNT_OFF 0

/*-------------------------------------------.
 | Register 0x00002130 RX_PACKET_DELETED_CNT |
 +-------------------------------------------+
 | bit  23:0 R  RXGE_GFPT_PKT_DEL_CNT        |
 +------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_PACKET_DELETED_CNT_UNUSED_MASK               0xff000000
#define GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_MSK 0x00ffffff
#define GE_GFPT_REG_RX_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PKT_DEL_CNT_OFF 0

/*----------------------------------------------------.
 | Register 0x00002134 RX_PREAMBLE_PACKET_DELETED_CNT |
 +----------------------------------------------------+
 | bit  23:0 R  RXGE_GFPT_PREPKT_DEL_CNT              |
 +---------------------------------------------------*/
#define PMC_GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_UNUSED_MASK                  0xff000000
#define GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_MSK 0x00ffffff
#define GE_GFPT_REG_RX_PREAMBLE_PACKET_DELETED_CNT_BIT_RXGE_GFPT_PREPKT_DEL_CNT_OFF 0

/*------------------------------------.
 | Register 0x00002138 RX_PMON_UPDATE |
 +------------------------------------+
 | bit  0 R/W  RX_PMON_UPDATE         |
 +-----------------------------------*/
#define PMC_GE_GFPT_REG_RX_PMON_UPDATE_UNUSED_MASK        0xfffffffe
#define GE_GFPT_REG_RX_PMON_UPDATE_BIT_RX_PMON_UPDATE_MSK 0x00000001
#define GE_GFPT_REG_RX_PMON_UPDATE_BIT_RX_PMON_UPDATE_OFF 0

/*--------------------------------------.
 | Register 0x0000213c TX_CFG           |
 +--------------------------------------+
 | bit  26:23 R/W  TX_RESYNC_COUNT      |
 | bit  22:19 R/W  TX_LOS_COUNT         |
 | bit  18:15 R/W  TX_SYNC_COUNT        |
 | bit  14    R/W  DISP_INVERSION_EN    |
 | bit  13    R/W  CLK_ADAPT_FIFO_RESET |
 | bit  12:3  R/W  TX_10BIT_PATTERN     |
 | bit  2     R/W  TX_10BIT_PATT_UPDATE |
 | bit  1     R/W  TX_10BIT_PATT_EN     |
 | bit  0     R/W  TX_SOFT_RESET        |
 +-------------------------------------*/
#define PMC_GE_GFPT_REG_TX_CFG_UNUSED_MASK              0xf8000000
#define GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_MSK      0x07800000
#define GE_GFPT_REG_TX_CFG_BIT_TX_RESYNC_COUNT_OFF      23
#define GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_MSK         0x00780000
#define GE_GFPT_REG_TX_CFG_BIT_TX_LOS_COUNT_OFF         19
#define GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_MSK        0x00078000
#define GE_GFPT_REG_TX_CFG_BIT_TX_SYNC_COUNT_OFF        15
#define GE_GFPT_REG_TX_CFG_BIT_DISP_INVERSION_EN_MSK    0x00004000
#define GE_GFPT_REG_TX_CFG_BIT_DISP_INVERSION_EN_OFF    14
#define GE_GFPT_REG_TX_CFG_BIT_CLK_ADAPT_FIFO_RESET_MSK 0x00002000
#define GE_GFPT_REG_TX_CFG_BIT_CLK_ADAPT_FIFO_RESET_OFF 13
#define GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_MSK     0x00001ff8
#define GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATTERN_OFF     3
#define GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_UPDATE_MSK 0x00000004
#define GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_UPDATE_OFF 2
#define GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_EN_MSK     0x00000002
#define GE_GFPT_REG_TX_CFG_BIT_TX_10BIT_PATT_EN_OFF     1
#define GE_GFPT_REG_TX_CFG_BIT_TX_SOFT_RESET_MSK        0x00000001
#define GE_GFPT_REG_TX_CFG_BIT_TX_SOFT_RESET_OFF        0

/*---------------------------------------.
 | Register 0x00002140 TX_20_BIT_PATTERN |
 +---------------------------------------+
 | bit  21:2 R/W  TX_20BIT_PATTERN       |
 | bit  1    R/W  TX_20BIT_PATT_UPDATE   |
 | bit  0    R/W  TX_20BIT_PATT_EN       |
 +--------------------------------------*/
#define PMC_GE_GFPT_REG_TX_20_BIT_PATTERN_UNUSED_MASK              0xffc00000
#define GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_MSK     0x003ffffc
#define GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATTERN_OFF     2
#define GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_UPDATE_MSK 0x00000002
#define GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_UPDATE_OFF 1
#define GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_EN_MSK     0x00000001
#define GE_GFPT_REG_TX_20_BIT_PATTERN_BIT_TX_20BIT_PATT_EN_OFF     0

/*--------------------------------------------------------.
 | Register 0x00002144 TX_RATE_ADAPTATION_DELETE_INTERVAL |
 +--------------------------------------------------------+
 | bit  15:0 R/W  TXGE_GFPT_DELETE_INTERVAL               |
 +-------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_UNUSED_MASK                   0xffff0000
#define GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_MSK 0x0000ffff
#define GE_GFPT_REG_TX_RATE_ADAPTATION_DELETE_INTERVAL_BIT_TXGE_GFPT_DELETE_INTERVAL_OFF 0

/*--------------------------------------------------------.
 | Register 0x00002148 TX_RATE_ADAPTATION_INSERT_INTERVAL |
 +--------------------------------------------------------+
 | bit  15:0 R/W  TXGE_GFPT_INSERT_INTERVAL               |
 +-------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_UNUSED_MASK                   0xffff0000
#define GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_MSK 0x0000ffff
#define GE_GFPT_REG_TX_RATE_ADAPTATION_INSERT_INTERVAL_BIT_TXGE_GFPT_INSERT_INTERVAL_OFF 0

/*---------------------------------.
 | Register 0x0000214c TX_LCV_CNT0 |
 +---------------------------------+
 | bit  31:0 R  TXGE_GFPT_LCV_CNT  |
 +--------------------------------*/
#define PMC_GE_GFPT_REG_TX_LCV_CNT0_UNUSED_MASK           0x00000000
#define GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_LCV_CNT0_BIT_TXGE_GFPT_LCV_CNT_OFF 0

/*---------------------------------.
 | Register 0x00002150 TX_LCV_CNT1 |
 +---------------------------------+
 | bit  7:0 R  TXGE_GFPT_LCV_CNT   |
 +--------------------------------*/
#define PMC_GE_GFPT_REG_TX_LCV_CNT1_UNUSED_MASK           0xffffff00
#define GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_LCV_CNT1_BIT_TXGE_GFPT_LCV_CNT_OFF 0

/*-------------------------------------------.
 | Register 0x00002154 TX_IDLES_DELETED_CNT0 |
 +-------------------------------------------+
 | bit  31:0 R  TXGE_GFPT_IDLES_DEL_CNT      |
 +------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT0_UNUSED_MASK                 0x00000000
#define GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_IDLES_DELETED_CNT0_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF 0

/*-------------------------------------------.
 | Register 0x00002158 TX_IDLES_DELETED_CNT1 |
 +-------------------------------------------+
 | bit  7:0 R  TXGE_GFPT_IDLES_DEL_CNT       |
 +------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_IDLES_DELETED_CNT1_UNUSED_MASK                 0xffffff00
#define GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_IDLES_DELETED_CNT1_BIT_TXGE_GFPT_IDLES_DEL_CNT_OFF 0

/*----------------------------------------------------.
 | Register 0x0000215c TX_CFG_ORDER_SETS_DELETED_CNT0 |
 +----------------------------------------------------+
 | bit  31:0 R  TXGE_GFPT_CONF_DEL_CNT                |
 +---------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_UNUSED_MASK                0x00000000
#define GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT0_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF 0

/*----------------------------------------------------.
 | Register 0x00002160 TX_CFG_ORDER_SETS_DELETED_CNT1 |
 +----------------------------------------------------+
 | bit  7:0 R  TXGE_GFPT_CONF_DEL_CNT                 |
 +---------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_UNUSED_MASK                0xffffff00
#define GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_CFG_ORDER_SETS_DELETED_CNT1_BIT_TXGE_GFPT_CONF_DEL_CNT_OFF 0

/*----------------------------------------------------------------.
 | Register 0x00002164 TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 |
 +----------------------------------------------------------------+
 | bit  31:0 R  TXGE_GFPT_PROPAGERR_DEL_CNT                       |
 +---------------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_UNUSED_MASK                     0x00000000
#define GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF 0

/*----------------------------------------------------------------.
 | Register 0x00002168 TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1 |
 +----------------------------------------------------------------+
 | bit  7:0 R  TXGE_GFPT_PROPAGERR_DEL_CNT                        |
 +---------------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_UNUSED_MASK                     0xffffff00
#define GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_BIT_TXGE_GFPT_PROPAGERR_DEL_CNT_OFF 0

/*--------------------------------------------.
 | Register 0x0000216c TX_IDLES_INSERTED_CNT0 |
 +--------------------------------------------+
 | bit  31:0 R  TXGE_GFPT_IDLES_INSRT_CNT     |
 +-------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_UNUSED_MASK                   0x00000000
#define GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_IDLES_INSERTED_CNT0_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF 0

/*--------------------------------------------.
 | Register 0x00002170 TX_IDLES_INSERTED_CNT1 |
 +--------------------------------------------+
 | bit  7:0 R  TXGE_GFPT_IDLES_INSRT_CNT      |
 +-------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_UNUSED_MASK                   0xffffff00
#define GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_IDLES_INSERTED_CNT1_BIT_TXGE_GFPT_IDLES_INSRT_CNT_OFF 0

/*--------------------------------------------------------------.
 | Register 0x00002174 TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0 |
 +--------------------------------------------------------------+
 | bit  31:0 R  TXGE_GFPT_CONF_INSRT_CNT                        |
 +-------------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_UNUSED_MASK                  0x00000000
#define GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF 0

/*--------------------------------------------------------------.
 | Register 0x00002178 TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1 |
 +--------------------------------------------------------------+
 | bit  7:0 R  TXGE_GFPT_CONF_INSRT_CNT                         |
 +-------------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_UNUSED_MASK                  0xffffff00
#define GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_BIT_TXGE_GFPT_CONF_INSRT_CNT_OFF 0

/*-------------------------------------------------------.
 | Register 0x0000217c TX_PROPAGATION_ERRS_INSERTED_CNT0 |
 +-------------------------------------------------------+
 | bit  31:0 R  TXGE_GFPT_PROPG_INSRT_CNT                |
 +------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_UNUSED_MASK                   0x00000000
#define GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK 0xffffffff
#define GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT0_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF 0

/*-------------------------------------------------------.
 | Register 0x00002180 TX_PROPAGATION_ERRS_INSERTED_CNT1 |
 +-------------------------------------------------------+
 | bit  7:0 R  TXGE_GFPT_PROPG_INSRT_CNT                 |
 +------------------------------------------------------*/
#define PMC_GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_UNUSED_MASK                   0xffffff00
#define GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_MSK 0x000000ff
#define GE_GFPT_REG_TX_PROPAGATION_ERRS_INSERTED_CNT1_BIT_TXGE_GFPT_PROPG_INSRT_CNT_OFF 0

/*------------------------------------.
 | Register 0x00002184 TX_PMON_UPDATE |
 +------------------------------------+
 | bit  0 R/W  TX_PMON_UPDATE         |
 +-----------------------------------*/
#define PMC_GE_GFPT_REG_TX_PMON_UPDATE_UNUSED_MASK        0xfffffffe
#define GE_GFPT_REG_TX_PMON_UPDATE_BIT_TX_PMON_UPDATE_MSK 0x00000001
#define GE_GFPT_REG_TX_PMON_UPDATE_BIT_TX_PMON_UPDATE_OFF 0

/*---------------------------------.
 | Register 0x00002188 DEFECT_STAT |
 +---------------------------------+
 | bit  1 R  TX_LOS_V              |
 | bit  0 R  RX_LOS_V              |
 +--------------------------------*/
#define PMC_GE_GFPT_REG_DEFECT_STAT_UNUSED_MASK  0xfffffff8
#define GE_GFPT_REG_DEFECT_STAT_BIT_TX_LOS_V_MSK 0x00000002
#define GE_GFPT_REG_DEFECT_STAT_BIT_TX_LOS_V_OFF 1
#define GE_GFPT_REG_DEFECT_STAT_BIT_RX_LOS_V_MSK 0x00000001
#define GE_GFPT_REG_DEFECT_STAT_BIT_RX_LOS_V_OFF 0

/*------------------------------.
 | Register 0x0000218c INT_EN   |
 +------------------------------+
 | bit  2 R/W  TX_DISP_INVERT_E |
 | bit  1 R/W  TX_LOS_E         |
 | bit  0 R/W  RX_LOS_E         |
 +-----------------------------*/
#define PMC_GE_GFPT_REG_INT_EN_UNUSED_MASK          0xfffffff0
#define GE_GFPT_REG_INT_EN_BIT_TX_DISP_INVERT_E_MSK 0x00000004
#define GE_GFPT_REG_INT_EN_BIT_TX_DISP_INVERT_E_OFF 2
#define GE_GFPT_REG_INT_EN_BIT_TX_LOS_E_MSK         0x00000002
#define GE_GFPT_REG_INT_EN_BIT_TX_LOS_E_OFF         1
#define GE_GFPT_REG_INT_EN_BIT_RX_LOS_E_MSK         0x00000001
#define GE_GFPT_REG_INT_EN_BIT_RX_LOS_E_OFF         0

/*-------------------------------.
 | Register 0x00002190 INT       |
 +-------------------------------+
 | bit  2 R/W  TX_DISP_INVERT_IC |
 | bit  1 R/W  TX_LOS_I          |
 | bit  0 R/W  RX_LOS_I          |
 +------------------------------*/
#define PMC_GE_GFPT_REG_INT_UNUSED_MASK           0xfffffff0
#define GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_MSK 0x00000004
#define GE_GFPT_REG_INT_BIT_TX_DISP_INVERT_IC_OFF 2
#define GE_GFPT_REG_INT_BIT_TX_LOS_I_MSK          0x00000002
#define GE_GFPT_REG_INT_BIT_TX_LOS_I_OFF          1
#define GE_GFPT_REG_INT_BIT_RX_LOS_I_MSK          0x00000001
#define GE_GFPT_REG_INT_BIT_RX_LOS_I_OFF          0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GE_GFPT_REGS_H */
