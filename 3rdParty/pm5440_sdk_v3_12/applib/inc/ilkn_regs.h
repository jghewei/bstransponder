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
 *     ilkn block
 *****************************************************************************/
#ifndef _ILKN_REGS_H
#define _ILKN_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ILAKEN_REG_LANE_DISABLE( N )                     (0x00005408 + (N) * 0x200)
#define PMC_ILAKEN_REG_CFG( N )                              (0x00005410 + (N) * 0x200)
#define PMC_ILAKEN_REG_TX_CTRL                               0x00005418
#define PMC_ILAKEN_REG_TX_FIFO_CFG                           0x00005420
#define PMC_ILAKEN_REG_BURST_CFG( N )                        (0x00005430 + (N) * 0x200)
#define PMC_ILAKEN_REG_METAFRAME_SYNC( N )                   (0x00005434 + (N) * 0x200)
#define PMC_ILAKEN_REG_TX_RATE_LIMITER_0                     0x00005438
#define PMC_ILAKEN_REG_TX_RATE_LIMITER_1                     0x0000543c
#define PMC_ILAKEN_REG_CNT_ACCESS( N )                       (0x00005474 + (N) * 0x200)
#define PMC_ILAKEN_REG_CNT_FILL( N )                         (0x00005478 + (N) * 0x200)
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH( N )              (0x00005480 + (N) * 0x200)
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW( N )               (0x00005484 + (N) * 0x200)
#define PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE( N )             (0x00005488 + (N) * 0x200)
#define PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE( N )             (0x0000548c + (N) * 0x200)
#define PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE( N )          (0x00005490 + (N) * 0x200)
#define PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE( N )                (0x00005494 + (N) * 0x200)
#define PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ( N )               (0x000054a0 + (N) * 0x200)
#define PMC_ILAKEN_REG_BYTE_CNT_LOW_READ( N )                (0x000054a4 + (N) * 0x200)
#define PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ( N )             (0x000054a8 + (N) * 0x200)
#define PMC_ILAKEN_REG_PACKET_CNT_LOW_READ( N )              (0x000054ac + (N) * 0x200)
#define PMC_ILAKEN_REG_ERR_CNT_HIGH_READ( N )                (0x000054b0 + (N) * 0x200)
#define PMC_ILAKEN_REG_ERR_CNT_LOW_READ( N )                 (0x000054b4 + (N) * 0x200)
#define PMC_ILAKEN_REG_CALENDAR_EN( N )                      (0x000054c0 + (N) * 0x200)
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS( N )     (0x000054c8 + (N) * 0x200)
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA( N ) (0x000054d0 + (N) * 0x200)
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA( N )  (0x000054d8 + (N) * 0x200)
#define PMC_ILAKEN_REG_SERDES_TEST_CTRL( N )                 (0x00005520 + (N) * 0x200)
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A( N )            (0x00005524 + (N) * 0x200)
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B( N )            (0x00005528 + (N) * 0x200)
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C( N )            (0x0000552c + (N) * 0x200)
#define PMC_ILAKEN_REG_TX_STAT_MESSAGE_RXD                   0x00005540
#define PMC_ILAKEN_REG_REMAP_LANE_5_0( N )                   (0x00005550 + (N) * 0x200)
#define PMC_ILAKEN_REG_REMAP_LANE_11_6( N )                  (0x00005554 + (N) * 0x200)
#define PMC_ILAKEN_REG_REMAP_LANE_17_12( N )                 (0x00005558 + (N) * 0x200)
#define PMC_ILAKEN_REG_REMAP_LANE_23_18( N )                 (0x0000555c + (N) * 0x200)
#define PMC_ILAKEN_REG_TX_INT( N )                           (0x00005580 + (N) * 0x8)
#define PMC_ILAKEN_REG_TX_INT_MASK( N )                      (0x00005584 + (N) * 0x8)
#define PMC_ILAKEN_REG_RX_CTRL                               0x00005618
#define PMC_ILAKEN_REG_RX_SERDES_FIFO_WATERMARK              0x00005628
#define PMC_ILAKEN_REG_RX_INT( N )                           (0x00005780 + (N) * 0x8)
#define PMC_ILAKEN_REG_RX_INT_MASK( N )                      (0x00005784 + (N) * 0x8)
#define PMC_ILAKEN_REG_RX_METAFRAME_ERR                      0x000057ac
#define PMC_ILAKEN_REG_RX_BAD_CTRL_WORD_ERR                  0x000057b0
#define PMC_ILAKEN_REG_RX_SCRAMBLER_STATE_ERR                0x000057b4
#define PMC_ILAKEN_REG_RX_CRC32_ERR                          0x000057b8
#define PMC_ILAKEN_REG_RX_CDR_ERR                            0x000057bc
#define PMC_ILAKEN_REG_RX_WORD_SYNC_CHANGED                  0x000057c0
#define PMC_ILAKEN_REG_RX_ALIGN_STAT                         0x000057c4
#define PMC_ILAKEN_REG_STAT_MESSAGE_RXD                      0x000057c8
#define PMC_ILAKEN_REG_RX_WORD_SYNC                          0x000057cc
#define PMC_ILAKEN_REG_CRC32_CNT_FILL                        0x000057d8
#define PMC_ILAKEN_REG_RX_CDR_LOST_CNT_FILL                  0x000057dc
#define PMC_ILAKEN_REG_RX_WORD_SYNC_CNT_FILL                 0x000057e0

/* index definitions for PMC_ILAKEN_REG_LANE_DISABLE */
#define PMC_ILAKEN_REG_LANE_DISABLE_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_LANE_DISABLE_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_LANE_DISABLE_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_LANE_DISABLE_INDEX_N_OFFSET 0x200

/*--------------------------------------------------.
 | Register (0x00005408 + (N) * 0x200) LANE_DISABLE |
 +--------------------------------------------------+
 | bit  23:0 R/W  DISABLE                           |
 +-------------------------------------------------*/
#define PMC_ILAKEN_REG_LANE_DISABLE_UNUSED_MASK    0xff000000
#define ILAKEN_REG_LANE_DISABLE_BIT_DISABLE_MSK    0x00ffffff
#define ILAKEN_REG_LANE_DISABLE_BIT_DISABLE_OFF    0

/* index definitions for PMC_ILAKEN_REG_CFG */
#define PMC_ILAKEN_REG_CFG_INDEX_N_MIN          0
#define PMC_ILAKEN_REG_CFG_INDEX_N_MAX          1
#define PMC_ILAKEN_REG_CFG_INDEX_N_SIZE         2
#define PMC_ILAKEN_REG_CFG_INDEX_N_OFFSET       0x200

/*-----------------------------------------.
 | Register (0x00005410 + (N) * 0x200) CFG |
 +-----------------------------------------+
 | bit  15 R/W  FC_RX_TO_TX_LB             |
 | bit  14 R/W  DATA_RX_TO_TX_LB           |
 | bit  13 R/W  FC_TX_TO_RX_LB             |
 | bit  12 R/W  DATA_TX_TO_RX_LB           |
 | bit  7  R/W  OUTBAND                    |
 | bit  6  R/W  INBAND                     |
 | bit  3  R/W  MULT_FC                    |
 +----------------------------------------*/
#define PMC_ILAKEN_REG_CFG_UNUSED_MASK          0xffff0c04
#define ILAKEN_REG_CFG_BIT_FC_RX_TO_TX_LB_MSK   0x00008000
#define ILAKEN_REG_CFG_BIT_FC_RX_TO_TX_LB_OFF   15
#define ILAKEN_REG_CFG_BIT_DATA_RX_TO_TX_LB_MSK 0x00004000
#define ILAKEN_REG_CFG_BIT_DATA_RX_TO_TX_LB_OFF 14
#define ILAKEN_REG_CFG_BIT_FC_TX_TO_RX_LB_MSK   0x00002000
#define ILAKEN_REG_CFG_BIT_FC_TX_TO_RX_LB_OFF   13
#define ILAKEN_REG_CFG_BIT_DATA_TX_TO_RX_LB_MSK 0x00001000
#define ILAKEN_REG_CFG_BIT_DATA_TX_TO_RX_LB_OFF 12
#define ILAKEN_REG_CFG_BIT_OUTBAND_MSK          0x00000080
#define ILAKEN_REG_CFG_BIT_OUTBAND_OFF          7
#define ILAKEN_REG_CFG_BIT_INBAND_MSK           0x00000040
#define ILAKEN_REG_CFG_BIT_INBAND_OFF           6
#define ILAKEN_REG_CFG_BIT_MULT_FC_MSK          0x00000008
#define ILAKEN_REG_CFG_BIT_MULT_FC_OFF          3

/*----------------------------------.
 | Register 0x00005418 TX_CTRL      |
 +----------------------------------+
 | bit  10 R/W  TX_STATUS_MSG_EN    |
 | bit  7  R/W  TX_DATA_OUT_KILL    |
 | bit  6  R/W  TX_FC_OUT_KILL      |
 | bit  1  R/W  IB_UNALIGN_XOFF_DIS |
 +---------------------------------*/
#define PMC_ILAKEN_REG_TX_CTRL_UNUSED_MASK             0xfffffb21
#define ILAKEN_REG_TX_CTRL_BIT_TX_STATUS_MSG_EN_MSK    0x00000400
#define ILAKEN_REG_TX_CTRL_BIT_TX_STATUS_MSG_EN_OFF    10
#define ILAKEN_REG_TX_CTRL_BIT_TX_DATA_OUT_KILL_MSK    0x00000080
#define ILAKEN_REG_TX_CTRL_BIT_TX_DATA_OUT_KILL_OFF    7
#define ILAKEN_REG_TX_CTRL_BIT_TX_FC_OUT_KILL_MSK      0x00000040
#define ILAKEN_REG_TX_CTRL_BIT_TX_FC_OUT_KILL_OFF      6
#define ILAKEN_REG_TX_CTRL_BIT_IB_UNALIGN_XOFF_DIS_MSK 0x00000002
#define ILAKEN_REG_TX_CTRL_BIT_IB_UNALIGN_XOFF_DIS_OFF 1

/*---------------------------------.
 | Register 0x00005420 TX_FIFO_CFG |
 +---------------------------------+
 | bit  11:8 R/W  WM_HIGH          |
 | bit  3:0  R/W  WM_LOW           |
 +--------------------------------*/
#define PMC_ILAKEN_REG_TX_FIFO_CFG_UNUSED_MASK 0xfffcf0f0
#define ILAKEN_REG_TX_FIFO_CFG_BIT_WM_HIGH_MSK 0x00000f00
#define ILAKEN_REG_TX_FIFO_CFG_BIT_WM_HIGH_OFF 8
#define ILAKEN_REG_TX_FIFO_CFG_BIT_WM_LOW_MSK  0x0000000f
#define ILAKEN_REG_TX_FIFO_CFG_BIT_WM_LOW_OFF  0

/* index definitions for PMC_ILAKEN_REG_BURST_CFG */
#define PMC_ILAKEN_REG_BURST_CFG_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_BURST_CFG_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_BURST_CFG_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_BURST_CFG_INDEX_N_OFFSET 0x200

/*-----------------------------------------------.
 | Register (0x00005430 + (N) * 0x200) BURST_CFG |
 +-----------------------------------------------+
 | bit  26:24 R/W  BURSTMAX                      |
 | bit  18:16 R/W  BURSTMIN                      |
 | bit  10:8  R/W  BURSTSHORT                    |
 | bit  2     R/W  MAX_DIS                       |
 | bit  1     R/W  MIN_DIS                       |
 +----------------------------------------------*/
#define PMC_ILAKEN_REG_BURST_CFG_UNUSED_MASK    0xf8f8f8f9
#define ILAKEN_REG_BURST_CFG_BIT_BURSTMAX_MSK   0x07000000
#define ILAKEN_REG_BURST_CFG_BIT_BURSTMAX_OFF   24
#define ILAKEN_REG_BURST_CFG_BIT_BURSTMIN_MSK   0x00070000
#define ILAKEN_REG_BURST_CFG_BIT_BURSTMIN_OFF   16
#define ILAKEN_REG_BURST_CFG_BIT_BURSTSHORT_MSK 0x00000700
#define ILAKEN_REG_BURST_CFG_BIT_BURSTSHORT_OFF 8
#define ILAKEN_REG_BURST_CFG_BIT_MAX_DIS_MSK    0x00000004
#define ILAKEN_REG_BURST_CFG_BIT_MAX_DIS_OFF    2
#define ILAKEN_REG_BURST_CFG_BIT_MIN_DIS_MSK    0x00000002
#define ILAKEN_REG_BURST_CFG_BIT_MIN_DIS_OFF    1

/* index definitions for PMC_ILAKEN_REG_METAFRAME_SYNC */
#define PMC_ILAKEN_REG_METAFRAME_SYNC_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_METAFRAME_SYNC_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_METAFRAME_SYNC_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_METAFRAME_SYNC_INDEX_N_OFFSET 0x200

/*----------------------------------------------------.
 | Register (0x00005434 + (N) * 0x200) METAFRAME_SYNC |
 +----------------------------------------------------+
 | bit  15:0 R/W  PERIOD                              |
 +---------------------------------------------------*/
#define PMC_ILAKEN_REG_METAFRAME_SYNC_UNUSED_MASK    0xffff0000
#define ILAKEN_REG_METAFRAME_SYNC_BIT_PERIOD_MSK     0x0000ffff
#define ILAKEN_REG_METAFRAME_SYNC_BIT_PERIOD_OFF     0

/*---------------------------------------.
 | Register 0x00005438 TX_RATE_LIMITER_0 |
 +---------------------------------------+
 | bit  31:16 R/W  CREDIT_MAX            |
 | bit  10:8  R/W  BYTE_GRAN             |
 | bit  0     R/W  RL_ENABLE             |
 +--------------------------------------*/
#define PMC_ILAKEN_REG_TX_RATE_LIMITER_0_UNUSED_MASK    0x0000f8fe
#define ILAKEN_REG_TX_RATE_LIMITER_0_BIT_CREDIT_MAX_MSK 0xffff0000
#define ILAKEN_REG_TX_RATE_LIMITER_0_BIT_CREDIT_MAX_OFF 16
#define ILAKEN_REG_TX_RATE_LIMITER_0_BIT_BYTE_GRAN_MSK  0x00000700
#define ILAKEN_REG_TX_RATE_LIMITER_0_BIT_BYTE_GRAN_OFF  8
#define ILAKEN_REG_TX_RATE_LIMITER_0_BIT_RL_ENABLE_MSK  0x00000001
#define ILAKEN_REG_TX_RATE_LIMITER_0_BIT_RL_ENABLE_OFF  0

/*---------------------------------------.
 | Register 0x0000543c TX_RATE_LIMITER_1 |
 +---------------------------------------+
 | bit  31:16 R/W  RL_TIME               |
 | bit  15:0  R/W  RL_DELTA              |
 +--------------------------------------*/
#define PMC_ILAKEN_REG_TX_RATE_LIMITER_1_UNUSED_MASK  0x00000000
#define ILAKEN_REG_TX_RATE_LIMITER_1_BIT_RL_TIME_MSK  0xffff0000
#define ILAKEN_REG_TX_RATE_LIMITER_1_BIT_RL_TIME_OFF  16
#define ILAKEN_REG_TX_RATE_LIMITER_1_BIT_RL_DELTA_MSK 0x0000ffff
#define ILAKEN_REG_TX_RATE_LIMITER_1_BIT_RL_DELTA_OFF 0

/* index definitions for PMC_ILAKEN_REG_CNT_ACCESS */
#define PMC_ILAKEN_REG_CNT_ACCESS_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_CNT_ACCESS_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_CNT_ACCESS_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_CNT_ACCESS_INDEX_N_OFFSET 0x200

/*------------------------------------------------.
 | Register (0x00005474 + (N) * 0x200) CNT_ACCESS |
 +------------------------------------------------+
 | bit  26:24 R/W  CMD                            |
 | bit  19:16 R/W  ACC_TYPE                       |
 | bit  7:0   R/W  ACC_ADDR                       |
 +-----------------------------------------------*/
#define PMC_ILAKEN_REG_CNT_ACCESS_UNUSED_MASK    0xf8f0ff00
#define ILAKEN_REG_CNT_ACCESS_BIT_CMD_MSK        0x07000000
#define ILAKEN_REG_CNT_ACCESS_BIT_CMD_OFF        24
#define ILAKEN_REG_CNT_ACCESS_BIT_ACC_TYPE_MSK   0x000f0000
#define ILAKEN_REG_CNT_ACCESS_BIT_ACC_TYPE_OFF   16
#define ILAKEN_REG_CNT_ACCESS_BIT_ACC_ADDR_MSK   0x000000ff
#define ILAKEN_REG_CNT_ACCESS_BIT_ACC_ADDR_OFF   0

/* index definitions for PMC_ILAKEN_REG_CNT_FILL */
#define PMC_ILAKEN_REG_CNT_FILL_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_CNT_FILL_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_CNT_FILL_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_CNT_FILL_INDEX_N_OFFSET 0x200

/*----------------------------------------------.
 | Register (0x00005478 + (N) * 0x200) CNT_FILL |
 +----------------------------------------------+
 | bit  31 R  BOVER                             |
 | bit  30 R  POVER                             |
 | bit  29 R  EOVER                             |
 | bit  28 R  BHALF                             |
 | bit  27 R  PHALF                             |
 | bit  26 R  EHALF                             |
 +---------------------------------------------*/
#define PMC_ILAKEN_REG_CNT_FILL_UNUSED_MASK    0x03f0ff00
#define ILAKEN_REG_CNT_FILL_BIT_BOVER_MSK      0x80000000
#define ILAKEN_REG_CNT_FILL_BIT_BOVER_OFF      31
#define ILAKEN_REG_CNT_FILL_BIT_POVER_MSK      0x40000000
#define ILAKEN_REG_CNT_FILL_BIT_POVER_OFF      30
#define ILAKEN_REG_CNT_FILL_BIT_EOVER_MSK      0x20000000
#define ILAKEN_REG_CNT_FILL_BIT_EOVER_OFF      29
#define ILAKEN_REG_CNT_FILL_BIT_BHALF_MSK      0x10000000
#define ILAKEN_REG_CNT_FILL_BIT_BHALF_OFF      28
#define ILAKEN_REG_CNT_FILL_BIT_PHALF_MSK      0x08000000
#define ILAKEN_REG_CNT_FILL_BIT_PHALF_OFF      27
#define ILAKEN_REG_CNT_FILL_BIT_EHALF_MSK      0x04000000
#define ILAKEN_REG_CNT_FILL_BIT_EHALF_OFF      26

/* index definitions for PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH */
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH_INDEX_N_OFFSET   0x200

/*---------------------------------------------------------.
 | Register (0x00005480 + (N) * 0x200) BYTE_CNT_WRITE_HIGH |
 +---------------------------------------------------------+
 | bit  23:0 R/W  MSB_BC_WDATA                             |
 +--------------------------------------------------------*/
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_HIGH_UNUSED_MASK      0xff000000
#define ILAKEN_REG_BYTE_CNT_WRITE_HIGH_BIT_MSB_BC_WDATA_MSK 0x00ffffff
#define ILAKEN_REG_BYTE_CNT_WRITE_HIGH_BIT_MSB_BC_WDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW */
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW_INDEX_N_OFFSET   0x200

/*--------------------------------------------------------.
 | Register (0x00005484 + (N) * 0x200) BYTE_CNT_WRITE_LOW |
 +--------------------------------------------------------+
 | bit  31:0 R/W  LSB_BC_WDATA                            |
 +-------------------------------------------------------*/
#define PMC_ILAKEN_REG_BYTE_CNT_WRITE_LOW_UNUSED_MASK      0x00000000
#define ILAKEN_REG_BYTE_CNT_WRITE_LOW_BIT_LSB_BC_WDATA_MSK 0xffffffff
#define ILAKEN_REG_BYTE_CNT_WRITE_LOW_BIT_LSB_BC_WDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE */
#define PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE_INDEX_N_OFFSET   0x200

/*----------------------------------------------------------.
 | Register (0x00005488 + (N) * 0x200) MSB_PACKET_CNT_WRITE |
 +----------------------------------------------------------+
 | bit  15:0 R/W  MSB_PC_WDATA                              |
 +---------------------------------------------------------*/
#define PMC_ILAKEN_REG_MSB_PACKET_CNT_WRITE_UNUSED_MASK      0xffff0000
#define ILAKEN_REG_MSB_PACKET_CNT_WRITE_BIT_MSB_PC_WDATA_MSK 0x0000ffff
#define ILAKEN_REG_MSB_PACKET_CNT_WRITE_BIT_MSB_PC_WDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE */
#define PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE_INDEX_N_OFFSET   0x200

/*----------------------------------------------------------.
 | Register (0x0000548c + (N) * 0x200) LSB_PACKET_CNT_WRITE |
 +----------------------------------------------------------+
 | bit  31:0 R/W  LSB_PC_WDATA                              |
 +---------------------------------------------------------*/
#define PMC_ILAKEN_REG_LSB_PACKET_CNT_WRITE_UNUSED_MASK      0x00000000
#define ILAKEN_REG_LSB_PACKET_CNT_WRITE_BIT_LSB_PC_WDATA_MSK 0xffffffff
#define ILAKEN_REG_LSB_PACKET_CNT_WRITE_BIT_LSB_PC_WDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE */
#define PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_INDEX_N_OFFSET   0x200

/*-------------------------------------------------------------.
 | Register (0x00005490 + (N) * 0x200) MSB_ERROR_COUNTER_WRITE |
 +-------------------------------------------------------------+
 | bit  15:0 R/W  MSB_EC_WDATA                                 |
 +------------------------------------------------------------*/
#define PMC_ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_UNUSED_MASK      0xffff0000
#define ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_BIT_MSB_EC_WDATA_MSK 0x0000ffff
#define ILAKEN_REG_MSB_ERROR_COUNTER_WRITE_BIT_MSB_EC_WDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE */
#define PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE_INDEX_N_OFFSET   0x200

/*-------------------------------------------------------.
 | Register (0x00005494 + (N) * 0x200) LSB_ERR_CNT_WRITE |
 +-------------------------------------------------------+
 | bit  31:0 R/W  LSB_EC_WDATA                           |
 +------------------------------------------------------*/
#define PMC_ILAKEN_REG_LSB_ERR_CNT_WRITE_UNUSED_MASK      0x00000000
#define ILAKEN_REG_LSB_ERR_CNT_WRITE_BIT_LSB_EC_WDATA_MSK 0xffffffff
#define ILAKEN_REG_LSB_ERR_CNT_WRITE_BIT_LSB_EC_WDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ */
#define PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ_INDEX_N_OFFSET   0x200

/*--------------------------------------------------------.
 | Register (0x000054a0 + (N) * 0x200) BYTE_CNT_HIGH_READ |
 +--------------------------------------------------------+
 | bit  31   R  RD_BOVER                                  |
 | bit  30   R  RD_BHALF                                  |
 | bit  29   R  RD_BUNCORR                                |
 | bit  28   R  RD_BCORR                                  |
 | bit  15:0 R  MSB_BC_RDATA                              |
 +-------------------------------------------------------*/
#define PMC_ILAKEN_REG_BYTE_CNT_HIGH_READ_UNUSED_MASK      0x0fff0000
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BOVER_MSK     0x80000000
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BOVER_OFF     31
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BHALF_MSK     0x40000000
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BHALF_OFF     30
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BUNCORR_MSK   0x20000000
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BUNCORR_OFF   29
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BCORR_MSK     0x10000000
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_RD_BCORR_OFF     28
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_MSB_BC_RDATA_MSK 0x0000ffff
#define ILAKEN_REG_BYTE_CNT_HIGH_READ_BIT_MSB_BC_RDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_BYTE_CNT_LOW_READ */
#define PMC_ILAKEN_REG_BYTE_CNT_LOW_READ_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_BYTE_CNT_LOW_READ_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_BYTE_CNT_LOW_READ_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_BYTE_CNT_LOW_READ_INDEX_N_OFFSET   0x200

/*-------------------------------------------------------.
 | Register (0x000054a4 + (N) * 0x200) BYTE_CNT_LOW_READ |
 +-------------------------------------------------------+
 | bit  31:0 R  LSB_BC_RDATA                             |
 +------------------------------------------------------*/
#define PMC_ILAKEN_REG_BYTE_CNT_LOW_READ_UNUSED_MASK      0x00000000
#define ILAKEN_REG_BYTE_CNT_LOW_READ_BIT_LSB_BC_RDATA_MSK 0xffffffff
#define ILAKEN_REG_BYTE_CNT_LOW_READ_BIT_LSB_BC_RDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ */
#define PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ_INDEX_N_OFFSET   0x200

/*----------------------------------------------------------.
 | Register (0x000054a8 + (N) * 0x200) PACKET_CNT_HIGH_READ |
 +----------------------------------------------------------+
 | bit  31   R  RD_POVER                                    |
 | bit  30   R  RD_PHALF                                    |
 | bit  29   R  RD_PUNCORR                                  |
 | bit  28   R  RD_PCORR                                    |
 | bit  15:0 R  MSB_PC_RDATA                                |
 +---------------------------------------------------------*/
#define PMC_ILAKEN_REG_PACKET_CNT_HIGH_READ_UNUSED_MASK      0x0fff0000
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_POVER_MSK     0x80000000
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_POVER_OFF     31
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_PHALF_MSK     0x40000000
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_PHALF_OFF     30
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_PUNCORR_MSK   0x20000000
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_PUNCORR_OFF   29
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_PCORR_MSK     0x10000000
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_RD_PCORR_OFF     28
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_MSB_PC_RDATA_MSK 0x0000ffff
#define ILAKEN_REG_PACKET_CNT_HIGH_READ_BIT_MSB_PC_RDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_PACKET_CNT_LOW_READ */
#define PMC_ILAKEN_REG_PACKET_CNT_LOW_READ_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_PACKET_CNT_LOW_READ_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_PACKET_CNT_LOW_READ_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_PACKET_CNT_LOW_READ_INDEX_N_OFFSET   0x200

/*---------------------------------------------------------.
 | Register (0x000054ac + (N) * 0x200) PACKET_CNT_LOW_READ |
 +---------------------------------------------------------+
 | bit  31:0 R  LSB_PC_RDATA                               |
 +--------------------------------------------------------*/
#define PMC_ILAKEN_REG_PACKET_CNT_LOW_READ_UNUSED_MASK      0x00000000
#define ILAKEN_REG_PACKET_CNT_LOW_READ_BIT_LSB_PC_RDATA_MSK 0xffffffff
#define ILAKEN_REG_PACKET_CNT_LOW_READ_BIT_LSB_PC_RDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_ERR_CNT_HIGH_READ */
#define PMC_ILAKEN_REG_ERR_CNT_HIGH_READ_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_ERR_CNT_HIGH_READ_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_ERR_CNT_HIGH_READ_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_ERR_CNT_HIGH_READ_INDEX_N_OFFSET   0x200

/*-------------------------------------------------------.
 | Register (0x000054b0 + (N) * 0x200) ERR_CNT_HIGH_READ |
 +-------------------------------------------------------+
 | bit  31   R  RD_EOVER                                 |
 | bit  30   R  RD_EHALF                                 |
 | bit  29   R  RD_EUNCORR                               |
 | bit  28   R  RD_ECORR                                 |
 | bit  15:0 R  MSB_EC_RDATA                             |
 +------------------------------------------------------*/
#define PMC_ILAKEN_REG_ERR_CNT_HIGH_READ_UNUSED_MASK      0x0fff0000
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_EOVER_MSK     0x80000000
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_EOVER_OFF     31
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_EHALF_MSK     0x40000000
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_EHALF_OFF     30
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_EUNCORR_MSK   0x20000000
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_EUNCORR_OFF   29
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_ECORR_MSK     0x10000000
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_RD_ECORR_OFF     28
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_MSB_EC_RDATA_MSK 0x0000ffff
#define ILAKEN_REG_ERR_CNT_HIGH_READ_BIT_MSB_EC_RDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_ERR_CNT_LOW_READ */
#define PMC_ILAKEN_REG_ERR_CNT_LOW_READ_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_ERR_CNT_LOW_READ_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_ERR_CNT_LOW_READ_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_ERR_CNT_LOW_READ_INDEX_N_OFFSET   0x200

/*------------------------------------------------------.
 | Register (0x000054b4 + (N) * 0x200) ERR_CNT_LOW_READ |
 +------------------------------------------------------+
 | bit  31:0 R  LSB_EC_RDATA                            |
 +-----------------------------------------------------*/
#define PMC_ILAKEN_REG_ERR_CNT_LOW_READ_UNUSED_MASK      0x00000000
#define ILAKEN_REG_ERR_CNT_LOW_READ_BIT_LSB_EC_RDATA_MSK 0xffffffff
#define ILAKEN_REG_ERR_CNT_LOW_READ_BIT_LSB_EC_RDATA_OFF 0

/* index definitions for PMC_ILAKEN_REG_CALENDAR_EN */
#define PMC_ILAKEN_REG_CALENDAR_EN_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_CALENDAR_EN_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_CALENDAR_EN_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_CALENDAR_EN_INDEX_N_OFFSET 0x200

/*-------------------------------------------------.
 | Register (0x000054c0 + (N) * 0x200) CALENDAR_EN |
 +-------------------------------------------------+
 | bit  18:8 R/W  LAST                             |
 | bit  0    R/W  PROG                             |
 +------------------------------------------------*/
#define PMC_ILAKEN_REG_CALENDAR_EN_UNUSED_MASK    0xfff800fe
#define ILAKEN_REG_CALENDAR_EN_BIT_LAST_MSK       0x0007ff00
#define ILAKEN_REG_CALENDAR_EN_BIT_LAST_OFF       8
#define ILAKEN_REG_CALENDAR_EN_BIT_PROG_MSK       0x00000001
#define ILAKEN_REG_CALENDAR_EN_BIT_PROG_OFF       0

/* index definitions for PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS */
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_INDEX_N_OFFSET   0x200

/*------------------------------------------------------------------.
 | Register (0x000054c8 + (N) * 0x200) PROGRAMMABLE_CALENDAR_ACCESS |
 +------------------------------------------------------------------+
 | bit  25:24 R/W  CAL_ACC_CMD                                      |
 | bit  10:0  R/W  CAL_ACC_ADDR                                     |
 +-----------------------------------------------------------------*/
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_UNUSED_MASK      0xfcfff800
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_BIT_CAL_ACC_CMD_MSK  0x03000000
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_BIT_CAL_ACC_CMD_OFF  24
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_BIT_CAL_ACC_ADDR_MSK 0x000007ff
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_ACCESS_BIT_CAL_ACC_ADDR_OFF 0

/* index definitions for PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA */
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_INDEX_N_OFFSET 0x200

/*----------------------------------------------------------------------.
 | Register (0x000054d0 + (N) * 0x200) PROGRAMMABLE_CALENDAR_WRITE_DATA |
 +----------------------------------------------------------------------+
 | bit  8:0 R/W  CAL_WDATA                                              |
 +---------------------------------------------------------------------*/
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_UNUSED_MASK    0xfffffe00
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_BIT_CAL_WDATA_MSK  0x000001ff
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_WRITE_DATA_BIT_CAL_WDATA_OFF  0

/* index definitions for PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA */
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_INDEX_N_OFFSET 0x200

/*---------------------------------------------------------------------.
 | Register (0x000054d8 + (N) * 0x200) PROGRAMMABLE_CALENDAR_READ_DATA |
 +---------------------------------------------------------------------+
 | bit  7:0 R  CAL_RDATA                                               |
 +--------------------------------------------------------------------*/
#define PMC_ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_UNUSED_MASK    0xffffff00
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_BIT_CAL_RDATA_MSK  0x000000ff
#define ILAKEN_REG_PROGRAMMABLE_CALENDAR_READ_DATA_BIT_CAL_RDATA_OFF  0

/* index definitions for PMC_ILAKEN_REG_SERDES_TEST_CTRL */
#define PMC_ILAKEN_REG_SERDES_TEST_CTRL_INDEX_N_MIN    0
#define PMC_ILAKEN_REG_SERDES_TEST_CTRL_INDEX_N_MAX    1
#define PMC_ILAKEN_REG_SERDES_TEST_CTRL_INDEX_N_SIZE   2
#define PMC_ILAKEN_REG_SERDES_TEST_CTRL_INDEX_N_OFFSET 0x200

/*------------------------------------------------------.
 | Register (0x00005520 + (N) * 0x200) SERDES_TEST_CTRL |
 +------------------------------------------------------+
 | bit  31:24 R/W  REPC                                 |
 | bit  23:16 R/W  REPB                                 |
 | bit  15:8  R/W  REPA                                 |
 | bit  1     R/W  PROG_EN                              |
 | bit  0     R/W  PRBS_EN                              |
 +-----------------------------------------------------*/
#define PMC_ILAKEN_REG_SERDES_TEST_CTRL_UNUSED_MASK    0x000000fc
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_REPC_MSK       0xff000000
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_REPC_OFF       24
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_REPB_MSK       0x00ff0000
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_REPB_OFF       16
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_REPA_MSK       0x0000ff00
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_REPA_OFF       8
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_PROG_EN_MSK    0x00000002
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_PROG_EN_OFF    1
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_PRBS_EN_MSK    0x00000001
#define ILAKEN_REG_SERDES_TEST_CTRL_BIT_PRBS_EN_OFF    0

/* index definitions for PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A */
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A_INDEX_N_MIN          0
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A_INDEX_N_MAX          1
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A_INDEX_N_SIZE         2
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A_INDEX_N_OFFSET       0x200

/*-----------------------------------------------------------.
 | Register (0x00005524 + (N) * 0x200) SERDES_TEST_PATTERN_A |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  TST_PTRN_A_VALUE                           |
 +----------------------------------------------------------*/
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_A_UNUSED_MASK          0x00000000
#define ILAKEN_REG_SERDES_TEST_PATTERN_A_BIT_TST_PTRN_A_VALUE_MSK 0xffffffff
#define ILAKEN_REG_SERDES_TEST_PATTERN_A_BIT_TST_PTRN_A_VALUE_OFF 0

/* index definitions for PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B */
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B_INDEX_N_MIN          0
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B_INDEX_N_MAX          1
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B_INDEX_N_SIZE         2
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B_INDEX_N_OFFSET       0x200

/*-----------------------------------------------------------.
 | Register (0x00005528 + (N) * 0x200) SERDES_TEST_PATTERN_B |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  TST_PTRN_B_VALUE                           |
 +----------------------------------------------------------*/
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_B_UNUSED_MASK          0x00000000
#define ILAKEN_REG_SERDES_TEST_PATTERN_B_BIT_TST_PTRN_B_VALUE_MSK 0xffffffff
#define ILAKEN_REG_SERDES_TEST_PATTERN_B_BIT_TST_PTRN_B_VALUE_OFF 0

/* index definitions for PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C */
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C_INDEX_N_MIN          0
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C_INDEX_N_MAX          1
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C_INDEX_N_SIZE         2
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C_INDEX_N_OFFSET       0x200

/*-----------------------------------------------------------.
 | Register (0x0000552c + (N) * 0x200) SERDES_TEST_PATTERN_C |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  TST_PTRN_C_VALUE                           |
 +----------------------------------------------------------*/
#define PMC_ILAKEN_REG_SERDES_TEST_PATTERN_C_UNUSED_MASK          0x00000000
#define ILAKEN_REG_SERDES_TEST_PATTERN_C_BIT_TST_PTRN_C_VALUE_MSK 0xffffffff
#define ILAKEN_REG_SERDES_TEST_PATTERN_C_BIT_TST_PTRN_C_VALUE_OFF 0

/*-----------------------------------------.
 | Register 0x00005540 TX_STAT_MESSAGE_RXD |
 +-----------------------------------------+
 | bit  31   R  CURRENT                    |
 | bit  30   R  VALID                      |
 | bit  26   R  SYNC                       |
 | bit  24   R  SIF                        |
 | bit  23:0 R  TX_SL                      |
 +----------------------------------------*/
#define PMC_ILAKEN_REG_TX_STAT_MESSAGE_RXD_UNUSED_MASK 0x3a000000
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_CURRENT_MSK 0x80000000
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_CURRENT_OFF 31
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_VALID_MSK   0x40000000
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_VALID_OFF   30
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_SYNC_MSK    0x04000000
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_SYNC_OFF    26
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_SIF_MSK     0x01000000
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_SIF_OFF     24
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_TX_SL_MSK   0x00ffffff
#define ILAKEN_REG_TX_STAT_MESSAGE_RXD_BIT_TX_SL_OFF   0

/* index definitions for PMC_ILAKEN_REG_REMAP_LANE_5_0 */
#define PMC_ILAKEN_REG_REMAP_LANE_5_0_INDEX_N_MIN     0
#define PMC_ILAKEN_REG_REMAP_LANE_5_0_INDEX_N_MAX     1
#define PMC_ILAKEN_REG_REMAP_LANE_5_0_INDEX_N_SIZE    2
#define PMC_ILAKEN_REG_REMAP_LANE_5_0_INDEX_N_OFFSET  0x200

/*----------------------------------------------------.
 | Register (0x00005550 + (N) * 0x200) REMAP_LANE_5_0 |
 +----------------------------------------------------+
 | bit  29:25 R/W  REMAP_LANE5                        |
 | bit  24:20 R/W  REMAP_LANE4                        |
 | bit  19:15 R/W  REMAP_LANE3                        |
 | bit  14:10 R/W  REMAP_LANE2                        |
 | bit  9:5   R/W  REMAP_LANE1                        |
 | bit  4:0   R/W  REMAP_LANE0                        |
 +---------------------------------------------------*/
#define PMC_ILAKEN_REG_REMAP_LANE_5_0_UNUSED_MASK     0xc0000000
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE5_MSK 0x3e000000
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE5_OFF 25
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE4_MSK 0x01f00000
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE4_OFF 20
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE3_MSK 0x000f8000
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE3_OFF 15
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE2_MSK 0x00007c00
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE2_OFF 10
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE1_MSK 0x000003e0
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE1_OFF 5
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE0_MSK 0x0000001f
#define ILAKEN_REG_REMAP_LANE_5_0_BIT_REMAP_LANE0_OFF 0

/* index definitions for PMC_ILAKEN_REG_REMAP_LANE_11_6 */
#define PMC_ILAKEN_REG_REMAP_LANE_11_6_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_REMAP_LANE_11_6_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_REMAP_LANE_11_6_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_REMAP_LANE_11_6_INDEX_N_OFFSET   0x200

/*-----------------------------------------------------.
 | Register (0x00005554 + (N) * 0x200) REMAP_LANE_11_6 |
 +-----------------------------------------------------+
 | bit  29:25 R/W  REMAP_LANE11                        |
 | bit  24:20 R/W  REMAP_LANE10                        |
 | bit  19:15 R/W  REMAP_LANE9                         |
 | bit  14:10 R/W  REMAP_LANE8                         |
 | bit  9:5   R/W  REMAP_LANE7                         |
 | bit  4:0   R/W  REMAP_LANE6                         |
 +----------------------------------------------------*/
#define PMC_ILAKEN_REG_REMAP_LANE_11_6_UNUSED_MASK      0xc0000000
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE11_MSK 0x3e000000
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE11_OFF 25
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE10_MSK 0x01f00000
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE10_OFF 20
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE9_MSK  0x000f8000
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE9_OFF  15
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE8_MSK  0x00007c00
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE8_OFF  10
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE7_MSK  0x000003e0
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE7_OFF  5
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE6_MSK  0x0000001f
#define ILAKEN_REG_REMAP_LANE_11_6_BIT_REMAP_LANE6_OFF  0

/* index definitions for PMC_ILAKEN_REG_REMAP_LANE_17_12 */
#define PMC_ILAKEN_REG_REMAP_LANE_17_12_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_REMAP_LANE_17_12_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_REMAP_LANE_17_12_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_REMAP_LANE_17_12_INDEX_N_OFFSET   0x200

/*------------------------------------------------------.
 | Register (0x00005558 + (N) * 0x200) REMAP_LANE_17_12 |
 +------------------------------------------------------+
 | bit  29:25 R/W  REMAP_LANE17                         |
 | bit  24:20 R/W  REMAP_LANE16                         |
 | bit  19:15 R/W  REMAP_LANE15                         |
 | bit  14:10 R/W  REMAP_LANE14                         |
 | bit  9:5   R/W  REMAP_LANE13                         |
 | bit  4:0   R/W  REMAP_LANE12                         |
 +-----------------------------------------------------*/
#define PMC_ILAKEN_REG_REMAP_LANE_17_12_UNUSED_MASK      0xc0000000
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE17_MSK 0x3e000000
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE17_OFF 25
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE16_MSK 0x01f00000
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE16_OFF 20
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE15_MSK 0x000f8000
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE15_OFF 15
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE14_MSK 0x00007c00
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE14_OFF 10
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE13_MSK 0x000003e0
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE13_OFF 5
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE12_MSK 0x0000001f
#define ILAKEN_REG_REMAP_LANE_17_12_BIT_REMAP_LANE12_OFF 0

/* index definitions for PMC_ILAKEN_REG_REMAP_LANE_23_18 */
#define PMC_ILAKEN_REG_REMAP_LANE_23_18_INDEX_N_MIN      0
#define PMC_ILAKEN_REG_REMAP_LANE_23_18_INDEX_N_MAX      1
#define PMC_ILAKEN_REG_REMAP_LANE_23_18_INDEX_N_SIZE     2
#define PMC_ILAKEN_REG_REMAP_LANE_23_18_INDEX_N_OFFSET   0x200

/*------------------------------------------------------.
 | Register (0x0000555c + (N) * 0x200) REMAP_LANE_23_18 |
 +------------------------------------------------------+
 | bit  29:25 R/W  REMAP_LANE23                         |
 | bit  24:20 R/W  REMAP_LANE22                         |
 | bit  19:15 R/W  REMAP_LANE21                         |
 | bit  14:10 R/W  REMAP_LANE20                         |
 | bit  9:5   R/W  REMAP_LANE19                         |
 | bit  4:0   R/W  REMAP_LANE18                         |
 +-----------------------------------------------------*/
#define PMC_ILAKEN_REG_REMAP_LANE_23_18_UNUSED_MASK      0xc0000000
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE23_MSK 0x3e000000
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE23_OFF 25
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE22_MSK 0x01f00000
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE22_OFF 20
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE21_MSK 0x000f8000
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE21_OFF 15
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE20_MSK 0x00007c00
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE20_OFF 10
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE19_MSK 0x000003e0
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE19_OFF 5
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE18_MSK 0x0000001f
#define ILAKEN_REG_REMAP_LANE_23_18_BIT_REMAP_LANE18_OFF 0

/* index definitions for PMC_ILAKEN_REG_TX_INT */
#define PMC_ILAKEN_REG_TX_INT_INDEX_N_MIN                0
#define PMC_ILAKEN_REG_TX_INT_INDEX_N_MAX                1
#define PMC_ILAKEN_REG_TX_INT_INDEX_N_SIZE               2
#define PMC_ILAKEN_REG_TX_INT_INDEX_N_OFFSET             0x8

/*------------------------------------------.
 | Register (0x00005580 + (N) * 0x8) TX_INT |
 +------------------------------------------+
 | bit  29 R/W  FIFO_OVER_I                 |
 | bit  28 R/W  FIFO_UNDER_I                |
 | bit  17 R/W  OOB_FC_ERR_I                |
 | bit  16 R/W  TX_STATUS_MSG_CHANGE_I      |
 | bit  1  R/W  TX_FC_CNT_FILL_I            |
 | bit  0  R/W  TX_CHAN_CNT_FILL_I          |
 +-----------------------------------------*/
#define PMC_ILAKEN_REG_TX_INT_UNUSED_MASK                0x0ffc3ffc
#define ILAKEN_REG_TX_INT_BIT_FIFO_OVER_I_MSK            0x20000000
#define ILAKEN_REG_TX_INT_BIT_FIFO_OVER_I_OFF            29
#define ILAKEN_REG_TX_INT_BIT_FIFO_UNDER_I_MSK           0x10000000
#define ILAKEN_REG_TX_INT_BIT_FIFO_UNDER_I_OFF           28
#define ILAKEN_REG_TX_INT_BIT_OOB_FC_ERR_I_MSK           0x00020000
#define ILAKEN_REG_TX_INT_BIT_OOB_FC_ERR_I_OFF           17
#define ILAKEN_REG_TX_INT_BIT_TX_STATUS_MSG_CHANGE_I_MSK 0x00010000
#define ILAKEN_REG_TX_INT_BIT_TX_STATUS_MSG_CHANGE_I_OFF 16
#define ILAKEN_REG_TX_INT_BIT_TX_FC_CNT_FILL_I_MSK       0x00000002
#define ILAKEN_REG_TX_INT_BIT_TX_FC_CNT_FILL_I_OFF       1
#define ILAKEN_REG_TX_INT_BIT_TX_CHAN_CNT_FILL_I_MSK     0x00000001
#define ILAKEN_REG_TX_INT_BIT_TX_CHAN_CNT_FILL_I_OFF     0

/* index definitions for PMC_ILAKEN_REG_TX_INT_MASK */
#define PMC_ILAKEN_REG_TX_INT_MASK_INDEX_N_MIN                0
#define PMC_ILAKEN_REG_TX_INT_MASK_INDEX_N_MAX                1
#define PMC_ILAKEN_REG_TX_INT_MASK_INDEX_N_SIZE               2
#define PMC_ILAKEN_REG_TX_INT_MASK_INDEX_N_OFFSET             0x8

/*-----------------------------------------------.
 | Register (0x00005584 + (N) * 0x8) TX_INT_MASK |
 +-----------------------------------------------+
 | bit  29 R/W  FIFO_OVER_M                      |
 | bit  28 R/W  FIFO_UNDER_M                     |
 | bit  17 R/W  OOB_FC_ERR_M                     |
 | bit  16 R/W  TX_STATUS_MSG_CHANGE_M           |
 | bit  1  R/W  TX_FC_CNT_FILL_M                 |
 | bit  0  R/W  TX_CHAN_CNT_FILL_M               |
 +----------------------------------------------*/
#define PMC_ILAKEN_REG_TX_INT_MASK_UNUSED_MASK                0x0ffc3ffc
#define ILAKEN_REG_TX_INT_MASK_BIT_FIFO_OVER_M_MSK            0x20000000
#define ILAKEN_REG_TX_INT_MASK_BIT_FIFO_OVER_M_OFF            29
#define ILAKEN_REG_TX_INT_MASK_BIT_FIFO_UNDER_M_MSK           0x10000000
#define ILAKEN_REG_TX_INT_MASK_BIT_FIFO_UNDER_M_OFF           28
#define ILAKEN_REG_TX_INT_MASK_BIT_OOB_FC_ERR_M_MSK           0x00020000
#define ILAKEN_REG_TX_INT_MASK_BIT_OOB_FC_ERR_M_OFF           17
#define ILAKEN_REG_TX_INT_MASK_BIT_TX_STATUS_MSG_CHANGE_M_MSK 0x00010000
#define ILAKEN_REG_TX_INT_MASK_BIT_TX_STATUS_MSG_CHANGE_M_OFF 16
#define ILAKEN_REG_TX_INT_MASK_BIT_TX_FC_CNT_FILL_M_MSK       0x00000002
#define ILAKEN_REG_TX_INT_MASK_BIT_TX_FC_CNT_FILL_M_OFF       1
#define ILAKEN_REG_TX_INT_MASK_BIT_TX_CHAN_CNT_FILL_M_MSK     0x00000001
#define ILAKEN_REG_TX_INT_MASK_BIT_TX_CHAN_CNT_FILL_M_OFF     0

/*----------------------------------.
 | Register 0x00005618 RX_CTRL      |
 +----------------------------------+
 | bit  11 R/W  RX_STATUS_MSG_CONT  |
 | bit  10 R/W  RX_STATUS_MSG_EN    |
 | bit  7  R/W  RX_DATA_OUT_KILL    |
 | bit  6  R/W  RX_FC_OUT_KILL      |
 | bit  1  R/W  OB_UNALIGN_XOFF_DIS |
 +---------------------------------*/
#define PMC_ILAKEN_REG_RX_CTRL_UNUSED_MASK             0xfffff321
#define ILAKEN_REG_RX_CTRL_BIT_RX_STATUS_MSG_CONT_MSK  0x00000800
#define ILAKEN_REG_RX_CTRL_BIT_RX_STATUS_MSG_CONT_OFF  11
#define ILAKEN_REG_RX_CTRL_BIT_RX_STATUS_MSG_EN_MSK    0x00000400
#define ILAKEN_REG_RX_CTRL_BIT_RX_STATUS_MSG_EN_OFF    10
#define ILAKEN_REG_RX_CTRL_BIT_RX_DATA_OUT_KILL_MSK    0x00000080
#define ILAKEN_REG_RX_CTRL_BIT_RX_DATA_OUT_KILL_OFF    7
#define ILAKEN_REG_RX_CTRL_BIT_RX_FC_OUT_KILL_MSK      0x00000040
#define ILAKEN_REG_RX_CTRL_BIT_RX_FC_OUT_KILL_OFF      6
#define ILAKEN_REG_RX_CTRL_BIT_OB_UNALIGN_XOFF_DIS_MSK 0x00000002
#define ILAKEN_REG_RX_CTRL_BIT_OB_UNALIGN_XOFF_DIS_OFF 1

/*----------------------------------------------.
 | Register 0x00005628 RX_SERDES_FIFO_WATERMARK |
 +----------------------------------------------+
 | bit  27:24 R/W  FC_HIGH                      |
 | bit  19:16 R/W  FC_LOW                       |
 | bit  11:8  R/W  RX_DATA_HIGH                 |
 | bit  3:0   R/W  RX_DATA_LOW                  |
 +---------------------------------------------*/
#define PMC_ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_UNUSED_MASK      0xf0f0f0f0
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_FC_HIGH_MSK      0x0f000000
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_FC_HIGH_OFF      24
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_FC_LOW_MSK       0x000f0000
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_FC_LOW_OFF       16
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_RX_DATA_HIGH_MSK 0x00000f00
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_RX_DATA_HIGH_OFF 8
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_RX_DATA_LOW_MSK  0x0000000f
#define ILAKEN_REG_RX_SERDES_FIFO_WATERMARK_BIT_RX_DATA_LOW_OFF  0

/* index definitions for PMC_ILAKEN_REG_RX_INT */
#define PMC_ILAKEN_REG_RX_INT_INDEX_N_MIN                0
#define PMC_ILAKEN_REG_RX_INT_INDEX_N_MAX                1
#define PMC_ILAKEN_REG_RX_INT_INDEX_N_SIZE               2
#define PMC_ILAKEN_REG_RX_INT_INDEX_N_OFFSET             0x8

/*------------------------------------------.
 | Register (0x00005780 + (N) * 0x8) RX_INT |
 +------------------------------------------+
 | bit  31 R/W  CTLBIT_ERR_I                |
 | bit  29 R/W  MF_ERR_I                    |
 | bit  28 R/W  BADCTL_ERR_I                |
 | bit  27 R/W  SCRAM_ERR_I                 |
 | bit  26 R/W  CRC32_ERR_I                 |
 | bit  25 R/W  CDR_ERR_I                   |
 | bit  24 R/W  WRD_SYNC_CHANGE_I           |
 | bit  23 R/W  ALIGN_CHANGE_I              |
 | bit  22 R/W  ALIGN_ERR_I                 |
 | bit  21 R/W  BURST_ERR_I                 |
 | bit  20 R/W  BMIN_ERR_I                  |
 | bit  19 R/W  BMAX_ERR_I                  |
 | bit  18 R/W  CRC24_ERR_I                 |
 | bit  17 R/W  IB_FC_ERR_I                 |
 | bit  16 R/W  RX_STATUS_MSG_CHANGE_I      |
 | bit  13 R/W  BADCTL_CNT_FILL_I           |
 | bit  10 R/W  CRC32_CNT_FILL_I            |
 | bit  9  R/W  CDR_CNT_FILL_I              |
 | bit  8  R/W  WRD_SYNC_CNT_FILL_I         |
 | bit  7  R/W  ALIGN_FAIL_CNT_FILL_I       |
 | bit  6  R/W  ALIGN_CNT_FILL_I            |
 | bit  5  R/W  BURST_CNT_FILL_I            |
 | bit  4  R/W  BMIN_CNT_FILL_I             |
 | bit  3  R/W  BMAX_CNT_FILL_I             |
 | bit  2  R/W  CRC24_CNT_FILL_I            |
 | bit  1  R/W  RX_FC_CNT_FILL_I            |
 | bit  0  R/W  RX_CHAN_CNT_FILL_I          |
 +-----------------------------------------*/
#define PMC_ILAKEN_REG_RX_INT_UNUSED_MASK                0x40001800
#define ILAKEN_REG_RX_INT_BIT_CTLBIT_ERR_I_MSK           0x80000000
#define ILAKEN_REG_RX_INT_BIT_CTLBIT_ERR_I_OFF           31
#define ILAKEN_REG_RX_INT_BIT_MF_ERR_I_MSK               0x20000000
#define ILAKEN_REG_RX_INT_BIT_MF_ERR_I_OFF               29
#define ILAKEN_REG_RX_INT_BIT_BADCTL_ERR_I_MSK           0x10000000
#define ILAKEN_REG_RX_INT_BIT_BADCTL_ERR_I_OFF           28
#define ILAKEN_REG_RX_INT_BIT_SCRAM_ERR_I_MSK            0x08000000
#define ILAKEN_REG_RX_INT_BIT_SCRAM_ERR_I_OFF            27
#define ILAKEN_REG_RX_INT_BIT_CRC32_ERR_I_MSK            0x04000000
#define ILAKEN_REG_RX_INT_BIT_CRC32_ERR_I_OFF            26
#define ILAKEN_REG_RX_INT_BIT_CDR_ERR_I_MSK              0x02000000
#define ILAKEN_REG_RX_INT_BIT_CDR_ERR_I_OFF              25
#define ILAKEN_REG_RX_INT_BIT_WRD_SYNC_CHANGE_I_MSK      0x01000000
#define ILAKEN_REG_RX_INT_BIT_WRD_SYNC_CHANGE_I_OFF      24
#define ILAKEN_REG_RX_INT_BIT_ALIGN_CHANGE_I_MSK         0x00800000
#define ILAKEN_REG_RX_INT_BIT_ALIGN_CHANGE_I_OFF         23
#define ILAKEN_REG_RX_INT_BIT_ALIGN_ERR_I_MSK            0x00400000
#define ILAKEN_REG_RX_INT_BIT_ALIGN_ERR_I_OFF            22
#define ILAKEN_REG_RX_INT_BIT_BURST_ERR_I_MSK            0x00200000
#define ILAKEN_REG_RX_INT_BIT_BURST_ERR_I_OFF            21
#define ILAKEN_REG_RX_INT_BIT_BMIN_ERR_I_MSK             0x00100000
#define ILAKEN_REG_RX_INT_BIT_BMIN_ERR_I_OFF             20
#define ILAKEN_REG_RX_INT_BIT_BMAX_ERR_I_MSK             0x00080000
#define ILAKEN_REG_RX_INT_BIT_BMAX_ERR_I_OFF             19
#define ILAKEN_REG_RX_INT_BIT_CRC24_ERR_I_MSK            0x00040000
#define ILAKEN_REG_RX_INT_BIT_CRC24_ERR_I_OFF            18
#define ILAKEN_REG_RX_INT_BIT_IB_FC_ERR_I_MSK            0x00020000
#define ILAKEN_REG_RX_INT_BIT_IB_FC_ERR_I_OFF            17
#define ILAKEN_REG_RX_INT_BIT_RX_STATUS_MSG_CHANGE_I_MSK 0x00010000
#define ILAKEN_REG_RX_INT_BIT_RX_STATUS_MSG_CHANGE_I_OFF 16
#define ILAKEN_REG_RX_INT_BIT_BADCTL_CNT_FILL_I_MSK      0x00002000
#define ILAKEN_REG_RX_INT_BIT_BADCTL_CNT_FILL_I_OFF      13
#define ILAKEN_REG_RX_INT_BIT_CRC32_CNT_FILL_I_MSK       0x00000400
#define ILAKEN_REG_RX_INT_BIT_CRC32_CNT_FILL_I_OFF       10
#define ILAKEN_REG_RX_INT_BIT_CDR_CNT_FILL_I_MSK         0x00000200
#define ILAKEN_REG_RX_INT_BIT_CDR_CNT_FILL_I_OFF         9
#define ILAKEN_REG_RX_INT_BIT_WRD_SYNC_CNT_FILL_I_MSK    0x00000100
#define ILAKEN_REG_RX_INT_BIT_WRD_SYNC_CNT_FILL_I_OFF    8
#define ILAKEN_REG_RX_INT_BIT_ALIGN_FAIL_CNT_FILL_I_MSK  0x00000080
#define ILAKEN_REG_RX_INT_BIT_ALIGN_FAIL_CNT_FILL_I_OFF  7
#define ILAKEN_REG_RX_INT_BIT_ALIGN_CNT_FILL_I_MSK       0x00000040
#define ILAKEN_REG_RX_INT_BIT_ALIGN_CNT_FILL_I_OFF       6
#define ILAKEN_REG_RX_INT_BIT_BURST_CNT_FILL_I_MSK       0x00000020
#define ILAKEN_REG_RX_INT_BIT_BURST_CNT_FILL_I_OFF       5
#define ILAKEN_REG_RX_INT_BIT_BMIN_CNT_FILL_I_MSK        0x00000010
#define ILAKEN_REG_RX_INT_BIT_BMIN_CNT_FILL_I_OFF        4
#define ILAKEN_REG_RX_INT_BIT_BMAX_CNT_FILL_I_MSK        0x00000008
#define ILAKEN_REG_RX_INT_BIT_BMAX_CNT_FILL_I_OFF        3
#define ILAKEN_REG_RX_INT_BIT_CRC24_CNT_FILL_I_MSK       0x00000004
#define ILAKEN_REG_RX_INT_BIT_CRC24_CNT_FILL_I_OFF       2
#define ILAKEN_REG_RX_INT_BIT_RX_FC_CNT_FILL_I_MSK       0x00000002
#define ILAKEN_REG_RX_INT_BIT_RX_FC_CNT_FILL_I_OFF       1
#define ILAKEN_REG_RX_INT_BIT_RX_CHAN_CNT_FILL_I_MSK     0x00000001
#define ILAKEN_REG_RX_INT_BIT_RX_CHAN_CNT_FILL_I_OFF     0

/* index definitions for PMC_ILAKEN_REG_RX_INT_MASK */
#define PMC_ILAKEN_REG_RX_INT_MASK_INDEX_N_MIN                0
#define PMC_ILAKEN_REG_RX_INT_MASK_INDEX_N_MAX                1
#define PMC_ILAKEN_REG_RX_INT_MASK_INDEX_N_SIZE               2
#define PMC_ILAKEN_REG_RX_INT_MASK_INDEX_N_OFFSET             0x8

/*-----------------------------------------------.
 | Register (0x00005784 + (N) * 0x8) RX_INT_MASK |
 +-----------------------------------------------+
 | bit  31 R/W  CTLBIT_ERR_M                     |
 | bit  29 R/W  MF_ERR_M                         |
 | bit  28 R/W  BADCTL_ERR_M                     |
 | bit  27 R/W  SCRAM_ERR_M                      |
 | bit  26 R/W  CRC32_ERR_M                      |
 | bit  25 R/W  CDR_ERR_M                        |
 | bit  24 R/W  WRD_SYNC_CHANGE_M                |
 | bit  23 R/W  ALIGN_CHANGE_M                   |
 | bit  22 R/W  ALIGN_ERR_M                      |
 | bit  21 R/W  BURST_ERR_M                      |
 | bit  20 R/W  BMIN_ERR_M                       |
 | bit  19 R/W  BMAX_ERR_M                       |
 | bit  18 R/W  CRC24_ERR_M                      |
 | bit  17 R/W  IB_FC_ERR_M                      |
 | bit  16 R/W  RX_STATUS_MSG_CHANGE_M           |
 | bit  13 R/W  BADCTL_CNT_FILL_M                |
 | bit  10 R/W  CRC32_CNT_FILL_M                 |
 | bit  9  R/W  CDR_CNT_FILL_M                   |
 | bit  8  R/W  WRD_SYNC_CNT_FILL_M              |
 | bit  7  R/W  ALIGN_FAIL_CNT_FILL_M            |
 | bit  6  R/W  ALIGN_CNT_FILL_M                 |
 | bit  5  R/W  BURST_CNT_FILL_M                 |
 | bit  4  R/W  BMIN_CNT_FILL_M                  |
 | bit  3  R/W  BMAX_CNT_FILL_M                  |
 | bit  2  R/W  CRC24_CNT_FILL_M                 |
 | bit  1  R/W  RX_FC_CNT_FILL_M                 |
 | bit  0  R/W  RX_CHAN_CNT_FILL_M               |
 +----------------------------------------------*/
#define PMC_ILAKEN_REG_RX_INT_MASK_UNUSED_MASK                0x40001800
#define ILAKEN_REG_RX_INT_MASK_BIT_CTLBIT_ERR_M_MSK           0x80000000
#define ILAKEN_REG_RX_INT_MASK_BIT_CTLBIT_ERR_M_OFF           31
#define ILAKEN_REG_RX_INT_MASK_BIT_MF_ERR_M_MSK               0x20000000
#define ILAKEN_REG_RX_INT_MASK_BIT_MF_ERR_M_OFF               29
#define ILAKEN_REG_RX_INT_MASK_BIT_BADCTL_ERR_M_MSK           0x10000000
#define ILAKEN_REG_RX_INT_MASK_BIT_BADCTL_ERR_M_OFF           28
#define ILAKEN_REG_RX_INT_MASK_BIT_SCRAM_ERR_M_MSK            0x08000000
#define ILAKEN_REG_RX_INT_MASK_BIT_SCRAM_ERR_M_OFF            27
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC32_ERR_M_MSK            0x04000000
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC32_ERR_M_OFF            26
#define ILAKEN_REG_RX_INT_MASK_BIT_CDR_ERR_M_MSK              0x02000000
#define ILAKEN_REG_RX_INT_MASK_BIT_CDR_ERR_M_OFF              25
#define ILAKEN_REG_RX_INT_MASK_BIT_WRD_SYNC_CHANGE_M_MSK      0x01000000
#define ILAKEN_REG_RX_INT_MASK_BIT_WRD_SYNC_CHANGE_M_OFF      24
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_CHANGE_M_MSK         0x00800000
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_CHANGE_M_OFF         23
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_ERR_M_MSK            0x00400000
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_ERR_M_OFF            22
#define ILAKEN_REG_RX_INT_MASK_BIT_BURST_ERR_M_MSK            0x00200000
#define ILAKEN_REG_RX_INT_MASK_BIT_BURST_ERR_M_OFF            21
#define ILAKEN_REG_RX_INT_MASK_BIT_BMIN_ERR_M_MSK             0x00100000
#define ILAKEN_REG_RX_INT_MASK_BIT_BMIN_ERR_M_OFF             20
#define ILAKEN_REG_RX_INT_MASK_BIT_BMAX_ERR_M_MSK             0x00080000
#define ILAKEN_REG_RX_INT_MASK_BIT_BMAX_ERR_M_OFF             19
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC24_ERR_M_MSK            0x00040000
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC24_ERR_M_OFF            18
#define ILAKEN_REG_RX_INT_MASK_BIT_IB_FC_ERR_M_MSK            0x00020000
#define ILAKEN_REG_RX_INT_MASK_BIT_IB_FC_ERR_M_OFF            17
#define ILAKEN_REG_RX_INT_MASK_BIT_RX_STATUS_MSG_CHANGE_M_MSK 0x00010000
#define ILAKEN_REG_RX_INT_MASK_BIT_RX_STATUS_MSG_CHANGE_M_OFF 16
#define ILAKEN_REG_RX_INT_MASK_BIT_BADCTL_CNT_FILL_M_MSK      0x00002000
#define ILAKEN_REG_RX_INT_MASK_BIT_BADCTL_CNT_FILL_M_OFF      13
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC32_CNT_FILL_M_MSK       0x00000400
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC32_CNT_FILL_M_OFF       10
#define ILAKEN_REG_RX_INT_MASK_BIT_CDR_CNT_FILL_M_MSK         0x00000200
#define ILAKEN_REG_RX_INT_MASK_BIT_CDR_CNT_FILL_M_OFF         9
#define ILAKEN_REG_RX_INT_MASK_BIT_WRD_SYNC_CNT_FILL_M_MSK    0x00000100
#define ILAKEN_REG_RX_INT_MASK_BIT_WRD_SYNC_CNT_FILL_M_OFF    8
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_FAIL_CNT_FILL_M_MSK  0x00000080
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_FAIL_CNT_FILL_M_OFF  7
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_CNT_FILL_M_MSK       0x00000040
#define ILAKEN_REG_RX_INT_MASK_BIT_ALIGN_CNT_FILL_M_OFF       6
#define ILAKEN_REG_RX_INT_MASK_BIT_BURST_CNT_FILL_M_MSK       0x00000020
#define ILAKEN_REG_RX_INT_MASK_BIT_BURST_CNT_FILL_M_OFF       5
#define ILAKEN_REG_RX_INT_MASK_BIT_BMIN_CNT_FILL_M_MSK        0x00000010
#define ILAKEN_REG_RX_INT_MASK_BIT_BMIN_CNT_FILL_M_OFF        4
#define ILAKEN_REG_RX_INT_MASK_BIT_BMAX_CNT_FILL_M_MSK        0x00000008
#define ILAKEN_REG_RX_INT_MASK_BIT_BMAX_CNT_FILL_M_OFF        3
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC24_CNT_FILL_M_MSK       0x00000004
#define ILAKEN_REG_RX_INT_MASK_BIT_CRC24_CNT_FILL_M_OFF       2
#define ILAKEN_REG_RX_INT_MASK_BIT_RX_FC_CNT_FILL_M_MSK       0x00000002
#define ILAKEN_REG_RX_INT_MASK_BIT_RX_FC_CNT_FILL_M_OFF       1
#define ILAKEN_REG_RX_INT_MASK_BIT_RX_CHAN_CNT_FILL_M_MSK     0x00000001
#define ILAKEN_REG_RX_INT_MASK_BIT_RX_CHAN_CNT_FILL_M_OFF     0

/*--------------------------------------.
 | Register 0x000057ac RX_METAFRAME_ERR |
 +--------------------------------------+
 | bit  23:0 R  RX_MF_ERR               |
 +-------------------------------------*/
#define PMC_ILAKEN_REG_RX_METAFRAME_ERR_UNUSED_MASK   0xff000000
#define ILAKEN_REG_RX_METAFRAME_ERR_BIT_RX_MF_ERR_MSK 0x00ffffff
#define ILAKEN_REG_RX_METAFRAME_ERR_BIT_RX_MF_ERR_OFF 0

/*------------------------------------------.
 | Register 0x000057b0 RX_BAD_CTRL_WORD_ERR |
 +------------------------------------------+
 | bit  23:0 R  RX_BADCTL_ERR               |
 +-----------------------------------------*/
#define PMC_ILAKEN_REG_RX_BAD_CTRL_WORD_ERR_UNUSED_MASK       0xff000000
#define ILAKEN_REG_RX_BAD_CTRL_WORD_ERR_BIT_RX_BADCTL_ERR_MSK 0x00ffffff
#define ILAKEN_REG_RX_BAD_CTRL_WORD_ERR_BIT_RX_BADCTL_ERR_OFF 0

/*--------------------------------------------.
 | Register 0x000057b4 RX_SCRAMBLER_STATE_ERR |
 +--------------------------------------------+
 | bit  23:0 R  RX_SCRAM_ERR                  |
 +-------------------------------------------*/
#define PMC_ILAKEN_REG_RX_SCRAMBLER_STATE_ERR_UNUSED_MASK      0xff000000
#define ILAKEN_REG_RX_SCRAMBLER_STATE_ERR_BIT_RX_SCRAM_ERR_MSK 0x00ffffff
#define ILAKEN_REG_RX_SCRAMBLER_STATE_ERR_BIT_RX_SCRAM_ERR_OFF 0

/*----------------------------------.
 | Register 0x000057b8 RX_CRC32_ERR |
 +----------------------------------+
 | bit  23:0 R  RX_CRC32_ERR        |
 +---------------------------------*/
#define PMC_ILAKEN_REG_RX_CRC32_ERR_UNUSED_MASK      0xff000000
#define ILAKEN_REG_RX_CRC32_ERR_BIT_RX_CRC32_ERR_MSK 0x00ffffff
#define ILAKEN_REG_RX_CRC32_ERR_BIT_RX_CRC32_ERR_OFF 0

/*--------------------------------.
 | Register 0x000057bc RX_CDR_ERR |
 +--------------------------------+
 | bit  23:0 R  RX_CDR_ERR        |
 +-------------------------------*/
#define PMC_ILAKEN_REG_RX_CDR_ERR_UNUSED_MASK    0xff000000
#define ILAKEN_REG_RX_CDR_ERR_BIT_RX_CDR_ERR_MSK 0x00ffffff
#define ILAKEN_REG_RX_CDR_ERR_BIT_RX_CDR_ERR_OFF 0

/*------------------------------------------.
 | Register 0x000057c0 RX_WORD_SYNC_CHANGED |
 +------------------------------------------+
 | bit  23:0 R  RX_WRD_SYNC_CHG             |
 +-----------------------------------------*/
#define PMC_ILAKEN_REG_RX_WORD_SYNC_CHANGED_UNUSED_MASK         0xff000000
#define ILAKEN_REG_RX_WORD_SYNC_CHANGED_BIT_RX_WRD_SYNC_CHG_MSK 0x00ffffff
#define ILAKEN_REG_RX_WORD_SYNC_CHANGED_BIT_RX_WRD_SYNC_CHG_OFF 0

/*-----------------------------------.
 | Register 0x000057c4 RX_ALIGN_STAT |
 +-----------------------------------+
 | bit  23 R  RX_ALIGN_GOOD          |
 | bit  22 R  RX_ALIGN_ERR           |
 +----------------------------------*/
#define PMC_ILAKEN_REG_RX_ALIGN_STAT_UNUSED_MASK       0xff3fffff
#define ILAKEN_REG_RX_ALIGN_STAT_BIT_RX_ALIGN_GOOD_MSK 0x00800000
#define ILAKEN_REG_RX_ALIGN_STAT_BIT_RX_ALIGN_GOOD_OFF 23
#define ILAKEN_REG_RX_ALIGN_STAT_BIT_RX_ALIGN_ERR_MSK  0x00400000
#define ILAKEN_REG_RX_ALIGN_STAT_BIT_RX_ALIGN_ERR_OFF  22

/*--------------------------------------.
 | Register 0x000057c8 STAT_MESSAGE_RXD |
 +--------------------------------------+
 | bit  31   R  RX_STATUS_MSG_VALID_ALL |
 | bit  30   R  RX_STATUS_MSG_VALID_ANY |
 | bit  26   R  RX_STATUS_MSG_SYNC      |
 | bit  25   R  SIF_ALL                 |
 | bit  24   R  SIF_ANY                 |
 | bit  23:0 R  RX_SL                   |
 +-------------------------------------*/
#define PMC_ILAKEN_REG_STAT_MESSAGE_RXD_UNUSED_MASK                 0x38000000
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_STATUS_MSG_VALID_ALL_MSK 0x80000000
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_STATUS_MSG_VALID_ALL_OFF 31
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_STATUS_MSG_VALID_ANY_MSK 0x40000000
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_STATUS_MSG_VALID_ANY_OFF 30
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_STATUS_MSG_SYNC_MSK      0x04000000
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_STATUS_MSG_SYNC_OFF      26
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_SIF_ALL_MSK                 0x02000000
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_SIF_ALL_OFF                 25
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_SIF_ANY_MSK                 0x01000000
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_SIF_ANY_OFF                 24
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_SL_MSK                   0x00ffffff
#define ILAKEN_REG_STAT_MESSAGE_RXD_BIT_RX_SL_OFF                   0

/*----------------------------------.
 | Register 0x000057cc RX_WORD_SYNC |
 +----------------------------------+
 | bit  23:0 R  RX_WRD_SYNC_LANE    |
 +---------------------------------*/
#define PMC_ILAKEN_REG_RX_WORD_SYNC_UNUSED_MASK          0xff000000
#define ILAKEN_REG_RX_WORD_SYNC_BIT_RX_WRD_SYNC_LANE_MSK 0x00ffffff
#define ILAKEN_REG_RX_WORD_SYNC_BIT_RX_WRD_SYNC_LANE_OFF 0

/*-------------------------------------.
 | Register 0x000057d8 CRC32_CNT_FILL  |
 +-------------------------------------+
 | bit  23:0 R  RX_CRC32_CNT_FILL_LANE |
 +------------------------------------*/
#define PMC_ILAKEN_REG_CRC32_CNT_FILL_UNUSED_MASK                0xff000000
#define ILAKEN_REG_CRC32_CNT_FILL_BIT_RX_CRC32_CNT_FILL_LANE_MSK 0x00ffffff
#define ILAKEN_REG_CRC32_CNT_FILL_BIT_RX_CRC32_CNT_FILL_LANE_OFF 0

/*------------------------------------------.
 | Register 0x000057dc RX_CDR_LOST_CNT_FILL |
 +------------------------------------------+
 | bit  23:0 R  RX_CRD_CNT_FILL_LANE        |
 +-----------------------------------------*/
#define PMC_ILAKEN_REG_RX_CDR_LOST_CNT_FILL_UNUSED_MASK              0xff000000
#define ILAKEN_REG_RX_CDR_LOST_CNT_FILL_BIT_RX_CRD_CNT_FILL_LANE_MSK 0x00ffffff
#define ILAKEN_REG_RX_CDR_LOST_CNT_FILL_BIT_RX_CRD_CNT_FILL_LANE_OFF 0

/*-------------------------------------------.
 | Register 0x000057e0 RX_WORD_SYNC_CNT_FILL |
 +-------------------------------------------+
 | bit  23:0 R  RX_WRD_SYNC_CNT_FILL_LANE    |
 +------------------------------------------*/
#define PMC_ILAKEN_REG_RX_WORD_SYNC_CNT_FILL_UNUSED_MASK                   0xff000000
#define ILAKEN_REG_RX_WORD_SYNC_CNT_FILL_BIT_RX_WRD_SYNC_CNT_FILL_LANE_MSK 0x00ffffff
#define ILAKEN_REG_RX_WORD_SYNC_CNT_FILL_BIT_RX_WRD_SYNC_CNT_FILL_LANE_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ILKN_REGS_H */
