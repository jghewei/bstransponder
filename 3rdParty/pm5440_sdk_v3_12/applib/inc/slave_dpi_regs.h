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
 *     slave_dpi block
 *****************************************************************************/
#ifndef _SLAVE_DPI_REGS_H
#define _SLAVE_DPI_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SLAVE_DPI_REG_ZONE_CONFIG           0x00000000
#define PMC_SLAVE_DPI_REG_BURST_CONFIG          0x00000010
#define PMC_SLAVE_DPI_REG_HRD_CONFIG            0x00000020
#define PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG     0x00000084
#define PMC_SLAVE_DPI_REG_PMON_CNT_TRIG         0x00000100
#define PMC_SLAVE_DPI_REG_PMON_MODE_TRIG        0x00000104
#define PMC_SLAVE_DPI_REG_PMON_PKT_TRIG         0x00000108
#define PMC_SLAVE_DPI_REG_PMON_CNT0_REG         0x00000110
#define PMC_SLAVE_DPI_REG_PMON_CNT1_REG         0x00000114
#define PMC_SLAVE_DPI_REG_PMON_CNT2_REG         0x00000118
#define PMC_SLAVE_DPI_REG_PMON_CNT3_REG         0x0000011c
#define PMC_SLAVE_DPI_REG_CH_ENABLE( N )        (0x00000400 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_CH_CONFIG( N )        (0x00000800 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_CH_Q_MAP( N )         (0x00000c00 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_ZONE_0_1( N )         (0x00001000 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_ZONE_1_2( N )         (0x00001400 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_ZONE_2_3( N )         (0x00001800 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG( N ) (0x00001c00 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_CH_DROP_INT( N )      (0x00002000 + (N) * 0x4)
#define PMC_SLAVE_DPI_REG_CH_DROP_COUNT( N )    (0x00002800 + (N) * 0x4)

/*---------------------------------.
 | Register 0x00000000 ZONE_CONFIG |
 +---------------------------------+
 | bit  25:24 R/W  ZONE_OVRD_VAL   |
 | bit  17    R/W  ZONE_EN         |
 | bit  16    R/W  ZONE_CFG        |
 | bit  7:0   R/W  ZONE_MAX_CH     |
 +--------------------------------*/
#define PMC_SLAVE_DPI_REG_ZONE_CONFIG_UNUSED_MASK       0xfcfcff00
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_MSK 0x03000000
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_OVRD_VAL_OFF 24
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_MSK       0x00020000
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_OFF       17
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_CFG_MSK      0x00010000
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_CFG_OFF      16
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_MSK   0x000000ff
#define SLAVE_DPI_REG_ZONE_CONFIG_BIT_ZONE_MAX_CH_OFF   0

/*----------------------------------.
 | Register 0x00000010 BURST_CONFIG |
 +----------------------------------+
 | bit  0 R/W  BURST_EN             |
 +---------------------------------*/
#define PMC_SLAVE_DPI_REG_BURST_CONFIG_UNUSED_MASK  0xfffffffe
#define SLAVE_DPI_REG_BURST_CONFIG_BIT_BURST_EN_MSK 0x00000001
#define SLAVE_DPI_REG_BURST_CONFIG_BIT_BURST_EN_OFF 0

/*--------------------------------.
 | Register 0x00000020 HRD_CONFIG |
 +--------------------------------+
 | bit  5:0 R/W  HDR_SIZE         |
 +-------------------------------*/
#define PMC_SLAVE_DPI_REG_HRD_CONFIG_UNUSED_MASK  0xffffffc0
#define SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_MSK 0x0000003f
#define SLAVE_DPI_REG_HRD_CONFIG_BIT_HDR_SIZE_OFF 0

/*---------------------------------------.
 | Register 0x00000084 DROP_CNT_TRIG_REG |
 +---------------------------------------+
 | bit  0 R/W  CH_DROP_CNT_TRIG          |
 +--------------------------------------*/
#define PMC_SLAVE_DPI_REG_DROP_CNT_TRIG_REG_UNUSED_MASK          0xfffffffe
#define SLAVE_DPI_REG_DROP_CNT_TRIG_REG_BIT_CH_DROP_CNT_TRIG_MSK 0x00000001
#define SLAVE_DPI_REG_DROP_CNT_TRIG_REG_BIT_CH_DROP_CNT_TRIG_OFF 0

/*-----------------------------------.
 | Register 0x00000100 PMON_CNT_TRIG |
 +-----------------------------------+
 | bit  25 R/W  PMON_CNT3_RST        |
 | bit  24 R/W  PMON_CNT3_TRIG       |
 | bit  17 R/W  PMON_CNT2_RST        |
 | bit  16 R/W  PMON_CNT2_TRIG       |
 | bit  9  R/W  PMON_CNT1_RST        |
 | bit  8  R/W  PMON_CNT1_TRIG       |
 | bit  1  R/W  PMON_CNT0_RST        |
 | bit  0  R/W  PMON_CNT0_TRIG       |
 +----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_CNT_TRIG_UNUSED_MASK        0xfcfcfcfc
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_RST_MSK  0x02000000
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_RST_OFF  25
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_MSK 0x01000000
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT3_TRIG_OFF 24
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_RST_MSK  0x00020000
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_RST_OFF  17
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_MSK 0x00010000
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT2_TRIG_OFF 16
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_RST_MSK  0x00000200
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_RST_OFF  9
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_MSK 0x00000100
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT1_TRIG_OFF 8
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_RST_MSK  0x00000002
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_RST_OFF  1
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_MSK 0x00000001
#define SLAVE_DPI_REG_PMON_CNT_TRIG_BIT_PMON_CNT0_TRIG_OFF 0

/*------------------------------------.
 | Register 0x00000104 PMON_MODE_TRIG |
 +------------------------------------+
 | bit  24 R/W  PMON_CNT3_MODE        |
 | bit  16 R/W  PMON_CNT2_MODE        |
 | bit  8  R/W  PMON_CNT1_MODE        |
 | bit  0  R/W  PMON_CNT0_MODE        |
 +-----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_MODE_TRIG_UNUSED_MASK        0xfefefefe
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT3_MODE_MSK 0x01000000
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT3_MODE_OFF 24
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT2_MODE_MSK 0x00010000
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT2_MODE_OFF 16
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT1_MODE_MSK 0x00000100
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT1_MODE_OFF 8
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT0_MODE_MSK 0x00000001
#define SLAVE_DPI_REG_PMON_MODE_TRIG_BIT_PMON_CNT0_MODE_OFF 0

/*------------------------------------.
 | Register 0x00000108 PMON_PKT_TRIG  |
 +------------------------------------+
 | bit  28:24 R/W  PMON_CNT3_PKT_FILT |
 | bit  20:16 R/W  PMON_CNT2_PKT_FILT |
 | bit  12:8  R/W  PMON_CNT1_PKT_FILT |
 | bit  4:0   R/W  PMON_CNT0_PKT_FILT |
 +-----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_PKT_TRIG_UNUSED_MASK            0xe0e0e0e0
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_MSK 0x1f000000
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT3_PKT_FILT_OFF 24
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_MSK 0x001f0000
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT2_PKT_FILT_OFF 16
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_MSK 0x00001f00
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT1_PKT_FILT_OFF 8
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_MSK 0x0000001f
#define SLAVE_DPI_REG_PMON_PKT_TRIG_BIT_PMON_CNT0_PKT_FILT_OFF 0

/*-----------------------------------.
 | Register 0x00000110 PMON_CNT0_REG |
 +-----------------------------------+
 | bit  31:0 R  PMON_CNT0_VAL        |
 +----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_CNT0_REG_UNUSED_MASK       0x00000000
#define SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_MSK 0xffffffff
#define SLAVE_DPI_REG_PMON_CNT0_REG_BIT_PMON_CNT0_VAL_OFF 0

/*-----------------------------------.
 | Register 0x00000114 PMON_CNT1_REG |
 +-----------------------------------+
 | bit  31:0 R  PMON_CNT1_VAL        |
 +----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_CNT1_REG_UNUSED_MASK       0x00000000
#define SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_MSK 0xffffffff
#define SLAVE_DPI_REG_PMON_CNT1_REG_BIT_PMON_CNT1_VAL_OFF 0

/*-----------------------------------.
 | Register 0x00000118 PMON_CNT2_REG |
 +-----------------------------------+
 | bit  31:0 R  PMON_CNT2_VAL        |
 +----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_CNT2_REG_UNUSED_MASK       0x00000000
#define SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_MSK 0xffffffff
#define SLAVE_DPI_REG_PMON_CNT2_REG_BIT_PMON_CNT2_VAL_OFF 0

/*-----------------------------------.
 | Register 0x0000011c PMON_CNT3_REG |
 +-----------------------------------+
 | bit  31:0 R  PMON_CNT3_VAL        |
 +----------------------------------*/
#define PMC_SLAVE_DPI_REG_PMON_CNT3_REG_UNUSED_MASK       0x00000000
#define SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_MSK 0xffffffff
#define SLAVE_DPI_REG_PMON_CNT3_REG_BIT_PMON_CNT3_VAL_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_CH_ENABLE */
#define PMC_SLAVE_DPI_REG_CH_ENABLE_INDEX_N_MIN    0
#define PMC_SLAVE_DPI_REG_CH_ENABLE_INDEX_N_MAX    149
#define PMC_SLAVE_DPI_REG_CH_ENABLE_INDEX_N_SIZE   150
#define PMC_SLAVE_DPI_REG_CH_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------.
 | Register (0x00000400 + (N) * 0x4) CH_ENABLE |
 +---------------------------------------------+
 | bit  8 R/W  HDR_EN                          |
 | bit  3 R/W  QS3_CH_EN                       |
 | bit  2 R/W  QS2_CH_EN                       |
 | bit  1 R/W  QS1_CH_EN                       |
 | bit  0 R/W  QS0_CH_EN                       |
 +--------------------------------------------*/
#define PMC_SLAVE_DPI_REG_CH_ENABLE_UNUSED_MASK    0xfffffef0
#define SLAVE_DPI_REG_CH_ENABLE_BIT_HDR_EN_MSK     0x00000100
#define SLAVE_DPI_REG_CH_ENABLE_BIT_HDR_EN_OFF     8
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS3_CH_EN_MSK  0x00000008
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS3_CH_EN_OFF  3
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS2_CH_EN_MSK  0x00000004
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS2_CH_EN_OFF  2
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS1_CH_EN_MSK  0x00000002
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS1_CH_EN_OFF  1
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS0_CH_EN_MSK  0x00000001
#define SLAVE_DPI_REG_CH_ENABLE_BIT_QS0_CH_EN_OFF  0

/* index definitions for PMC_SLAVE_DPI_REG_CH_CONFIG */
#define PMC_SLAVE_DPI_REG_CH_CONFIG_INDEX_N_MIN       0
#define PMC_SLAVE_DPI_REG_CH_CONFIG_INDEX_N_MAX       255
#define PMC_SLAVE_DPI_REG_CH_CONFIG_INDEX_N_SIZE      256
#define PMC_SLAVE_DPI_REG_CH_CONFIG_INDEX_N_OFFSET    0x4

/*---------------------------------------------.
 | Register (0x00000800 + (N) * 0x4) CH_CONFIG |
 +---------------------------------------------+
 | bit  17:16 R/W  DCPB_IMUX_SEL               |
 | bit  11    R/W  PMON_CNT3_EN                |
 | bit  10    R/W  PMON_CNT2_EN                |
 | bit  9     R/W  PMON_CNT1_EN                |
 | bit  8     R/W  PMON_CNT0_EN                |
 | bit  3     R/W  QS3_CH_MST                  |
 | bit  2     R/W  QS2_CH_MST                  |
 | bit  1     R/W  QS1_CH_MST                  |
 | bit  0     R/W  QS0_CH_MST                  |
 +--------------------------------------------*/
#define PMC_SLAVE_DPI_REG_CH_CONFIG_UNUSED_MASK       0xfffcf0f0
#define SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_MSK 0x00030000
#define SLAVE_DPI_REG_CH_CONFIG_BIT_DCPB_IMUX_SEL_OFF 16
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT3_EN_MSK  0x00000800
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT3_EN_OFF  11
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT2_EN_MSK  0x00000400
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT2_EN_OFF  10
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT1_EN_MSK  0x00000200
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT1_EN_OFF  9
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT0_EN_MSK  0x00000100
#define SLAVE_DPI_REG_CH_CONFIG_BIT_PMON_CNT0_EN_OFF  8
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS3_CH_MST_MSK    0x00000008
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS3_CH_MST_OFF    3
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS2_CH_MST_MSK    0x00000004
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS2_CH_MST_OFF    2
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS1_CH_MST_MSK    0x00000002
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS1_CH_MST_OFF    1
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS0_CH_MST_MSK    0x00000001
#define SLAVE_DPI_REG_CH_CONFIG_BIT_QS0_CH_MST_OFF    0

/* index definitions for PMC_SLAVE_DPI_REG_CH_Q_MAP */
#define PMC_SLAVE_DPI_REG_CH_Q_MAP_INDEX_N_MIN     0
#define PMC_SLAVE_DPI_REG_CH_Q_MAP_INDEX_N_MAX     255
#define PMC_SLAVE_DPI_REG_CH_Q_MAP_INDEX_N_SIZE    256
#define PMC_SLAVE_DPI_REG_CH_Q_MAP_INDEX_N_OFFSET  0x4

/*--------------------------------------------.
 | Register (0x00000c00 + (N) * 0x4) CH_Q_MAP |
 +--------------------------------------------+
 | bit  30:24 R/W  QS3_POP_QID                |
 | bit  22:16 R/W  QS2_POP_QID                |
 | bit  14:8  R/W  QS1_POP_QID                |
 | bit  6:0   R/W  QS0_POP_QID                |
 +-------------------------------------------*/
#define PMC_SLAVE_DPI_REG_CH_Q_MAP_UNUSED_MASK     0x80808080
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_MSK 0x7f000000
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS3_POP_QID_OFF 24
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_MSK 0x007f0000
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS2_POP_QID_OFF 16
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_MSK 0x00007f00
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS1_POP_QID_OFF 8
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_MSK 0x0000007f
#define SLAVE_DPI_REG_CH_Q_MAP_BIT_QS0_POP_QID_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_ZONE_0_1 */
#define PMC_SLAVE_DPI_REG_ZONE_0_1_INDEX_N_MIN      0
#define PMC_SLAVE_DPI_REG_ZONE_0_1_INDEX_N_MAX      255
#define PMC_SLAVE_DPI_REG_ZONE_0_1_INDEX_N_SIZE     256
#define PMC_SLAVE_DPI_REG_ZONE_0_1_INDEX_N_OFFSET   0x4

/*--------------------------------------------.
 | Register (0x00001000 + (N) * 0x4) ZONE_0_1 |
 +--------------------------------------------+
 | bit  16:0 R/W  ZONE_THR_0_1                |
 +-------------------------------------------*/
#define PMC_SLAVE_DPI_REG_ZONE_0_1_UNUSED_MASK      0xfffe0000
#define SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK 0x0001ffff
#define SLAVE_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_ZONE_1_2 */
#define PMC_SLAVE_DPI_REG_ZONE_1_2_INDEX_N_MIN      0
#define PMC_SLAVE_DPI_REG_ZONE_1_2_INDEX_N_MAX      255
#define PMC_SLAVE_DPI_REG_ZONE_1_2_INDEX_N_SIZE     256
#define PMC_SLAVE_DPI_REG_ZONE_1_2_INDEX_N_OFFSET   0x4

/*--------------------------------------------.
 | Register (0x00001400 + (N) * 0x4) ZONE_1_2 |
 +--------------------------------------------+
 | bit  16:0 R/W  ZONE_THR_1_2                |
 +-------------------------------------------*/
#define PMC_SLAVE_DPI_REG_ZONE_1_2_UNUSED_MASK      0xfffe0000
#define SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK 0x0001ffff
#define SLAVE_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_ZONE_2_3 */
#define PMC_SLAVE_DPI_REG_ZONE_2_3_INDEX_N_MIN      0
#define PMC_SLAVE_DPI_REG_ZONE_2_3_INDEX_N_MAX      255
#define PMC_SLAVE_DPI_REG_ZONE_2_3_INDEX_N_SIZE     256
#define PMC_SLAVE_DPI_REG_ZONE_2_3_INDEX_N_OFFSET   0x4

/*--------------------------------------------.
 | Register (0x00001800 + (N) * 0x4) ZONE_2_3 |
 +--------------------------------------------+
 | bit  16:0 R/W  ZONE_THR_2_3                |
 +-------------------------------------------*/
#define PMC_SLAVE_DPI_REG_ZONE_2_3_UNUSED_MASK      0xfffe0000
#define SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK 0x0001ffff
#define SLAVE_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG */
#define PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_MIN           0
#define PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_MAX           255
#define PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_SIZE          256
#define PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_OFFSET        0x4

/*----------------------------------------------------.
 | Register (0x00001c00 + (N) * 0x4) ZONE_OVRD_CONFIG |
 +----------------------------------------------------+
 | bit  25   R/W  ZONE_OVRD_EN                        |
 | bit  24   R/W  ZONE_OVRD_CFG                       |
 | bit  10:8 R/W  ZONE_OVRD_DPI_MST                   |
 | bit  7:0  R/W  ZONE_OVRD_XOFF_CH                   |
 +---------------------------------------------------*/
#define PMC_SLAVE_DPI_REG_ZONE_OVRD_CONFIG_UNUSED_MASK           0xfcfff800
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_MSK      0x02000000
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_OFF      25
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_MSK     0x01000000
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_OFF     24
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_MSK 0x00000700
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_DPI_MST_OFF 8
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_MSK 0x000000ff
#define SLAVE_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_XOFF_CH_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_CH_DROP_INT */
#define PMC_SLAVE_DPI_REG_CH_DROP_INT_INDEX_N_MIN           0
#define PMC_SLAVE_DPI_REG_CH_DROP_INT_INDEX_N_MAX           149
#define PMC_SLAVE_DPI_REG_CH_DROP_INT_INDEX_N_SIZE          150
#define PMC_SLAVE_DPI_REG_CH_DROP_INT_INDEX_N_OFFSET        0x4

/*-----------------------------------------------.
 | Register (0x00002000 + (N) * 0x4) CH_DROP_INT |
 +-----------------------------------------------+
 | bit  16 R/W  CH_DROP_FULL_I                   |
 | bit  8  R/W  CH_DROP_CLIENT_I                 |
 | bit  0  R/W  CH_DROP_OVRSIZE_I                |
 +----------------------------------------------*/
#define PMC_SLAVE_DPI_REG_CH_DROP_INT_UNUSED_MASK           0xfffefefe
#define SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_MSK    0x00010000
#define SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_FULL_I_OFF    16
#define SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_MSK  0x00000100
#define SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_CLIENT_I_OFF  8
#define SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_MSK 0x00000001
#define SLAVE_DPI_REG_CH_DROP_INT_BIT_CH_DROP_OVRSIZE_I_OFF 0

/* index definitions for PMC_SLAVE_DPI_REG_CH_DROP_COUNT */
#define PMC_SLAVE_DPI_REG_CH_DROP_COUNT_INDEX_N_MIN         0
#define PMC_SLAVE_DPI_REG_CH_DROP_COUNT_INDEX_N_MAX         149
#define PMC_SLAVE_DPI_REG_CH_DROP_COUNT_INDEX_N_SIZE        150
#define PMC_SLAVE_DPI_REG_CH_DROP_COUNT_INDEX_N_OFFSET      0x4

/*-------------------------------------------------.
 | Register (0x00002800 + (N) * 0x4) CH_DROP_COUNT |
 +-------------------------------------------------+
 | bit  31:0 R/W  CH_DROP_CNT_VAL                  |
 +------------------------------------------------*/
#define PMC_SLAVE_DPI_REG_CH_DROP_COUNT_UNUSED_MASK         0x00000000
#define SLAVE_DPI_REG_CH_DROP_COUNT_BIT_CH_DROP_CNT_VAL_MSK 0xffffffff
#define SLAVE_DPI_REG_CH_DROP_COUNT_BIT_CH_DROP_CNT_VAL_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SLAVE_DPI_REGS_H */
