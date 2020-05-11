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
 *     odu_tfrm block
 *****************************************************************************/
#ifndef _ODU_TFRM_REGS_H
#define _ODU_TFRM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ODU_TFRM_REG_TTI_PID( N )                    (0x00000000 + (N) * 0x04)
#define PMC_ODU_TFRM_REG_TTI_PM( N )                     (0x00004000 + (N) * 0x04)
#define PMC_ODU_TFRM_REG_FTFL( N )                       (0x00006000 + (N) * 0x04)
#define PMC_ODU_TFRM_REG_APS_PCC( N )                    (0x00007000 + (N) * 0x04)
#define PMC_ODU_TFRM_REG_DM_PID( N )                     (0x00007c00 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM_CTRL1( N )                  (0x00007c04 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM_CTRL2( N )                  (0x00007c08 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM_CTRL3( N )                  (0x00007c0c + (N) * 0x40)
#define PMC_ODU_TFRM_REG_DM_PM( N )                      (0x0000bc00 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_PM_CTRL1( N )                   (0x0000bc04 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_ROW2_OH( N )                    (0x0000bc08 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_ROW3_4_OH( N )                  (0x0000bc0c + (N) * 0x40)
#define PMC_ODU_TFRM_REG_ROW4_OH0( N )                   (0x0000bc10 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_ROW4_OH1( N )                   (0x0000bc14 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM1_POOL_ID( N )               (0x0000bc18 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM2_POOL_ID( N )               (0x0000bc1c + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM3_POOL_ID( N )               (0x0000bc20 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM4_POOL_ID( N )               (0x0000bc24 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM5_POOL_ID( N )               (0x0000bc28 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_TCM6_POOL_ID( N )               (0x0000bc2c + (N) * 0x40)
#define PMC_ODU_TFRM_REG_OH_INSERT( N )                  (0x0000bc30 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_CHANNEL_CTRL( N )               (0x0000bc34 + (N) * 0x40)
#define PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0               0x0000d500
#define PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1               0x0000d504
#define PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2               0x0000d508
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I         0x0000d50c
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I         0x0000d510
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I         0x0000d514
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I       0x0000d518
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I       0x0000d51c
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I       0x0000d520
#define PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I         0x0000d524
#define PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I         0x0000d528
#define PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I         0x0000d52c
#define PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I         0x0000d530
#define PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I         0x0000d534
#define PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I         0x0000d538
#define PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I         0x0000d53c
#define PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I         0x0000d540
#define PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I 0x0000d54c
#define PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V          0x0000d64c
#define PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V          0x0000d650
#define PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V          0x0000d654
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E           0x0000d658
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E           0x0000d65c
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E           0x0000d660
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E         0x0000d664
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E         0x0000d668
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E         0x0000d66c
#define PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E           0x0000d670
#define PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E           0x0000d674
#define PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E           0x0000d678
#define PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E           0x0000d67c
#define PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E           0x0000d680
#define PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E           0x0000d684
#define PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E           0x0000d688
#define PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E           0x0000d68c
#define PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E   0x0000d698

/* index definitions for PMC_ODU_TFRM_REG_TTI_PID */
#define PMC_ODU_TFRM_REG_TTI_PID_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_TTI_PID_INDEX_N_MAX    4095
#define PMC_ODU_TFRM_REG_TTI_PID_INDEX_N_SIZE   4096
#define PMC_ODU_TFRM_REG_TTI_PID_INDEX_N_OFFSET 0x04

/*--------------------------------------------.
 | Register (0x00000000 + (N) * 0x04) TTI_PID |
 +--------------------------------------------+
 | bit  31:0 R/W  TTI                         |
 +-------------------------------------------*/
#define PMC_ODU_TFRM_REG_TTI_PID_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_TTI_PID_BIT_TTI_MSK        0xffffffff
#define ODU_TFRM_REG_TTI_PID_BIT_TTI_OFF        0

/* index definitions for PMC_ODU_TFRM_REG_TTI_PM */
#define PMC_ODU_TFRM_REG_TTI_PM_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_TTI_PM_INDEX_N_MAX    1535
#define PMC_ODU_TFRM_REG_TTI_PM_INDEX_N_SIZE   1536
#define PMC_ODU_TFRM_REG_TTI_PM_INDEX_N_OFFSET 0x04

/*-------------------------------------------.
 | Register (0x00004000 + (N) * 0x04) TTI_PM |
 +-------------------------------------------+
 | bit  31:0 R/W  TTI_PM                     |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_TTI_PM_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_TTI_PM_BIT_TTI_PM_MSK     0xffffffff
#define ODU_TFRM_REG_TTI_PM_BIT_TTI_PM_OFF     0

/* index definitions for PMC_ODU_TFRM_REG_FTFL */
#define PMC_ODU_TFRM_REG_FTFL_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_FTFL_INDEX_N_MAX    767
#define PMC_ODU_TFRM_REG_FTFL_INDEX_N_SIZE   768
#define PMC_ODU_TFRM_REG_FTFL_INDEX_N_OFFSET 0x04

/*-----------------------------------------.
 | Register (0x00006000 + (N) * 0x04) FTFL |
 +-----------------------------------------+
 | bit  31:0 R/W  FTFL                     |
 +----------------------------------------*/
#define PMC_ODU_TFRM_REG_FTFL_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_FTFL_BIT_FTFL_MSK       0xffffffff
#define ODU_TFRM_REG_FTFL_BIT_FTFL_OFF       0

/* index definitions for PMC_ODU_TFRM_REG_APS_PCC */
#define PMC_ODU_TFRM_REG_APS_PCC_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_APS_PCC_INDEX_N_MAX    767
#define PMC_ODU_TFRM_REG_APS_PCC_INDEX_N_SIZE   768
#define PMC_ODU_TFRM_REG_APS_PCC_INDEX_N_OFFSET 0x04

/*--------------------------------------------.
 | Register (0x00007000 + (N) * 0x04) APS_PCC |
 +--------------------------------------------+
 | bit  31:0 R/W  APS                         |
 +-------------------------------------------*/
#define PMC_ODU_TFRM_REG_APS_PCC_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_APS_PCC_BIT_APS_MSK        0xffffffff
#define ODU_TFRM_REG_APS_PCC_BIT_APS_OFF        0

/* index definitions for PMC_ODU_TFRM_REG_DM_PID */
#define PMC_ODU_TFRM_REG_DM_PID_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_DM_PID_INDEX_N_MAX    255
#define PMC_ODU_TFRM_REG_DM_PID_INDEX_N_SIZE   256
#define PMC_ODU_TFRM_REG_DM_PID_INDEX_N_OFFSET 0x40

/*-------------------------------------------.
 | Register (0x00007c00 + (N) * 0x40) DM_PID |
 +-------------------------------------------+
 | bit  26:24 R/W  DM_STATE                  |
 | bit  19:0  R/W  DM_COUNT                  |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_DM_PID_UNUSED_MASK    0xf8f00000
#define ODU_TFRM_REG_DM_PID_BIT_DM_STATE_MSK   0x07000000
#define ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF   24
#define ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_MSK   0x000fffff
#define ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF   0

/* index definitions for PMC_ODU_TFRM_REG_TCM_CTRL1 */
#define PMC_ODU_TFRM_REG_TCM_CTRL1_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_TCM_CTRL1_INDEX_N_MAX    255
#define PMC_ODU_TFRM_REG_TCM_CTRL1_INDEX_N_SIZE   256
#define PMC_ODU_TFRM_REG_TCM_CTRL1_INDEX_N_OFFSET 0x40

/*----------------------------------------------.
 | Register (0x00007c04 + (N) * 0x40) TCM_CTRL1 |
 +----------------------------------------------+
 | bit  2:0 R/W  STAT                           |
 +---------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM_CTRL1_UNUSED_MASK    0xfffffff8
#define ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_MSK       0x00000007
#define ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF       0

/* index definitions for PMC_ODU_TFRM_REG_TCM_CTRL2 */
#define PMC_ODU_TFRM_REG_TCM_CTRL2_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_TCM_CTRL2_INDEX_N_MAX    255
#define PMC_ODU_TFRM_REG_TCM_CTRL2_INDEX_N_SIZE   256
#define PMC_ODU_TFRM_REG_TCM_CTRL2_INDEX_N_OFFSET 0x40

/*----------------------------------------------.
 | Register (0x00007c08 + (N) * 0x40) TCM_CTRL2 |
 +----------------------------------------------+
 | bit  1 R/W  DMT_SRC                          |
 | bit  0 R/W  DMT_VAL                          |
 +---------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM_CTRL2_UNUSED_MASK    0xfffffffc
#define ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_SRC_MSK    0x00000002
#define ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_SRC_OFF    1
#define ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_VAL_MSK    0x00000001
#define ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_VAL_OFF    0

/* index definitions for PMC_ODU_TFRM_REG_TCM_CTRL3 */
#define PMC_ODU_TFRM_REG_TCM_CTRL3_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM_CTRL3_INDEX_N_MAX      255
#define PMC_ODU_TFRM_REG_TCM_CTRL3_INDEX_N_SIZE     256
#define PMC_ODU_TFRM_REG_TCM_CTRL3_INDEX_N_OFFSET   0x40

/*----------------------------------------------.
 | Register (0x00007c0c + (N) * 0x40) TCM_CTRL3 |
 +----------------------------------------------+
 | bit  7:4 R/W  BEI_BIAE                       |
 | bit  3   R/W  BDI                            |
 | bit  2   R/W  TTI_OS_INS                     |
 | bit  1   R/W  TTI_DAPI_INS                   |
 | bit  0   R/W  TTI_SAPI_INS                   |
 +---------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM_CTRL3_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_MSK     0x000000f0
#define ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF     4
#define ODU_TFRM_REG_TCM_CTRL3_BIT_BDI_MSK          0x00000008
#define ODU_TFRM_REG_TCM_CTRL3_BIT_BDI_OFF          3
#define ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_OS_INS_MSK   0x00000004
#define ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_OS_INS_OFF   2
#define ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_DAPI_INS_MSK 0x00000002
#define ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_DAPI_INS_OFF 1
#define ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_SAPI_INS_MSK 0x00000001
#define ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_SAPI_INS_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_DM_PM */
#define PMC_ODU_TFRM_REG_DM_PM_INDEX_N_MIN     0
#define PMC_ODU_TFRM_REG_DM_PM_INDEX_N_MAX     95
#define PMC_ODU_TFRM_REG_DM_PM_INDEX_N_SIZE    96
#define PMC_ODU_TFRM_REG_DM_PM_INDEX_N_OFFSET  0x40

/*------------------------------------------.
 | Register (0x0000bc00 + (N) * 0x40) DM_PM |
 +------------------------------------------+
 | bit  26:24 R/W  DM_STATE_PM              |
 | bit  19:0  R/W  DM_COUNT_PM              |
 +-----------------------------------------*/
#define PMC_ODU_TFRM_REG_DM_PM_UNUSED_MASK     0xf8f00000
#define ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_MSK 0x07000000
#define ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF 24
#define ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_MSK 0x000fffff
#define ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_PM_CTRL1 */
#define PMC_ODU_TFRM_REG_PM_CTRL1_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_PM_CTRL1_INDEX_N_MAX    95
#define PMC_ODU_TFRM_REG_PM_CTRL1_INDEX_N_SIZE   96
#define PMC_ODU_TFRM_REG_PM_CTRL1_INDEX_N_OFFSET 0x40

/*---------------------------------------------.
 | Register (0x0000bc04 + (N) * 0x40) PM_CTRL1 |
 +---------------------------------------------+
 | bit  2:0 R/W  STAT_PM                       |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_PM_CTRL1_UNUSED_MASK    0xfffffff8
#define ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_MSK    0x00000007
#define ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF    0

/* index definitions for PMC_ODU_TFRM_REG_ROW2_OH */
#define PMC_ODU_TFRM_REG_ROW2_OH_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_ROW2_OH_INDEX_N_MAX    95
#define PMC_ODU_TFRM_REG_ROW2_OH_INDEX_N_SIZE   96
#define PMC_ODU_TFRM_REG_ROW2_OH_INDEX_N_OFFSET 0x40

/*--------------------------------------------.
 | Register (0x0000bc08 + (N) * 0x40) ROW2_OH |
 +--------------------------------------------+
 | bit  24    R/W  PM_TCM_RES                 |
 | bit  23:16 R/W  TCM_ACT                    |
 | bit  15:0  R/W  RES1                       |
 +-------------------------------------------*/
#define PMC_ODU_TFRM_REG_ROW2_OH_UNUSED_MASK    0xfe000000
#define ODU_TFRM_REG_ROW2_OH_BIT_PM_TCM_RES_MSK 0x01000000
#define ODU_TFRM_REG_ROW2_OH_BIT_PM_TCM_RES_OFF 24
#define ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK    0x00ff0000
#define ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF    16
#define ODU_TFRM_REG_ROW2_OH_BIT_RES1_MSK       0x0000ffff
#define ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF       0

/* index definitions for PMC_ODU_TFRM_REG_ROW3_4_OH */
#define PMC_ODU_TFRM_REG_ROW3_4_OH_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_ROW3_4_OH_INDEX_N_MAX    95
#define PMC_ODU_TFRM_REG_ROW3_4_OH_INDEX_N_SIZE   96
#define PMC_ODU_TFRM_REG_ROW3_4_OH_INDEX_N_OFFSET 0x40

/*----------------------------------------------.
 | Register (0x0000bc0c + (N) * 0x40) ROW3_4_OH |
 +----------------------------------------------+
 | bit  31:16 R/W  RES2                         |
 | bit  15:0  R/W  EXP                          |
 +---------------------------------------------*/
#define PMC_ODU_TFRM_REG_ROW3_4_OH_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_MSK       0xffff0000
#define ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF       16
#define ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_MSK        0x0000ffff
#define ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF        0

/* index definitions for PMC_ODU_TFRM_REG_ROW4_OH0 */
#define PMC_ODU_TFRM_REG_ROW4_OH0_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_ROW4_OH0_INDEX_N_MAX    95
#define PMC_ODU_TFRM_REG_ROW4_OH0_INDEX_N_SIZE   96
#define PMC_ODU_TFRM_REG_ROW4_OH0_INDEX_N_OFFSET 0x40

/*---------------------------------------------.
 | Register (0x0000bc10 + (N) * 0x40) ROW4_OH0 |
 +---------------------------------------------+
 | bit  31:0 R/W  RES3                         |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_ROW4_OH0_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_ROW4_OH0_BIT_RES3_MSK       0xffffffff
#define ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF       0

/* index definitions for PMC_ODU_TFRM_REG_ROW4_OH1 */
#define PMC_ODU_TFRM_REG_ROW4_OH1_INDEX_N_MIN    0
#define PMC_ODU_TFRM_REG_ROW4_OH1_INDEX_N_MAX    95
#define PMC_ODU_TFRM_REG_ROW4_OH1_INDEX_N_SIZE   96
#define PMC_ODU_TFRM_REG_ROW4_OH1_INDEX_N_OFFSET 0x40

/*---------------------------------------------.
 | Register (0x0000bc14 + (N) * 0x40) ROW4_OH1 |
 +---------------------------------------------+
 | bit  31:16 R/W  GCC2                        |
 | bit  15:0  R/W  GCC1                        |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_ROW4_OH1_UNUSED_MASK    0x00000000
#define ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_MSK       0xffff0000
#define ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF       16
#define ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_MSK       0x0000ffff
#define ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF       0

/* index definitions for PMC_ODU_TFRM_REG_TCM1_POOL_ID */
#define PMC_ODU_TFRM_REG_TCM1_POOL_ID_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM1_POOL_ID_INDEX_N_MAX      95
#define PMC_ODU_TFRM_REG_TCM1_POOL_ID_INDEX_N_SIZE     96
#define PMC_ODU_TFRM_REG_TCM1_POOL_ID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x0000bc18 + (N) * 0x40) TCM1_POOL_ID |
 +-------------------------------------------------+
 | bit  7:0 R/W  TCM1_POOL_ID                      |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM1_POOL_ID_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_MSK 0x000000ff
#define ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_TCM2_POOL_ID */
#define PMC_ODU_TFRM_REG_TCM2_POOL_ID_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM2_POOL_ID_INDEX_N_MAX      95
#define PMC_ODU_TFRM_REG_TCM2_POOL_ID_INDEX_N_SIZE     96
#define PMC_ODU_TFRM_REG_TCM2_POOL_ID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x0000bc1c + (N) * 0x40) TCM2_POOL_ID |
 +-------------------------------------------------+
 | bit  7:0 R/W  TCM2_POOL_ID                      |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM2_POOL_ID_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_MSK 0x000000ff
#define ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_TCM3_POOL_ID */
#define PMC_ODU_TFRM_REG_TCM3_POOL_ID_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM3_POOL_ID_INDEX_N_MAX      95
#define PMC_ODU_TFRM_REG_TCM3_POOL_ID_INDEX_N_SIZE     96
#define PMC_ODU_TFRM_REG_TCM3_POOL_ID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x0000bc20 + (N) * 0x40) TCM3_POOL_ID |
 +-------------------------------------------------+
 | bit  7:0 R/W  TCM3_POOL_ID                      |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM3_POOL_ID_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_MSK 0x000000ff
#define ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_TCM4_POOL_ID */
#define PMC_ODU_TFRM_REG_TCM4_POOL_ID_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM4_POOL_ID_INDEX_N_MAX      95
#define PMC_ODU_TFRM_REG_TCM4_POOL_ID_INDEX_N_SIZE     96
#define PMC_ODU_TFRM_REG_TCM4_POOL_ID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x0000bc24 + (N) * 0x40) TCM4_POOL_ID |
 +-------------------------------------------------+
 | bit  7:0 R/W  TCM4_POOL_ID                      |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM4_POOL_ID_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_MSK 0x000000ff
#define ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_TCM5_POOL_ID */
#define PMC_ODU_TFRM_REG_TCM5_POOL_ID_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM5_POOL_ID_INDEX_N_MAX      95
#define PMC_ODU_TFRM_REG_TCM5_POOL_ID_INDEX_N_SIZE     96
#define PMC_ODU_TFRM_REG_TCM5_POOL_ID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x0000bc28 + (N) * 0x40) TCM5_POOL_ID |
 +-------------------------------------------------+
 | bit  7:0 R/W  TCM5_POOL_ID                      |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM5_POOL_ID_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_MSK 0x000000ff
#define ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_TCM6_POOL_ID */
#define PMC_ODU_TFRM_REG_TCM6_POOL_ID_INDEX_N_MIN      0
#define PMC_ODU_TFRM_REG_TCM6_POOL_ID_INDEX_N_MAX      95
#define PMC_ODU_TFRM_REG_TCM6_POOL_ID_INDEX_N_SIZE     96
#define PMC_ODU_TFRM_REG_TCM6_POOL_ID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x0000bc2c + (N) * 0x40) TCM6_POOL_ID |
 +-------------------------------------------------+
 | bit  7:0 R/W  TCM6_POOL_ID                      |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_TCM6_POOL_ID_UNUSED_MASK      0xffffff00
#define ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_MSK 0x000000ff
#define ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF 0

/* index definitions for PMC_ODU_TFRM_REG_OH_INSERT */
#define PMC_ODU_TFRM_REG_OH_INSERT_INDEX_N_MIN        0
#define PMC_ODU_TFRM_REG_OH_INSERT_INDEX_N_MAX        95
#define PMC_ODU_TFRM_REG_OH_INSERT_INDEX_N_SIZE       96
#define PMC_ODU_TFRM_REG_OH_INSERT_INDEX_N_OFFSET     0x40

/*----------------------------------------------.
 | Register (0x0000bc30 + (N) * 0x40) OH_INSERT |
 +----------------------------------------------+
 | bit  18   R/W  RCP_EN                        |
 | bit  17   R/W  GCC1_INS                      |
 | bit  16   R/W  GCC2_INS                      |
 | bit  15:8 R/W  APS_PCC_INS                   |
 | bit  6    R/W  RES2_INS                      |
 | bit  5    R/W  EXP_INS                       |
 | bit  4    R/W  RES1_INS                      |
 | bit  3    R/W  PM_TCM_RES_INS                |
 | bit  2    R/W  TCM_ACT_INS                   |
 | bit  1:0  R/W  FTFL_INS                      |
 +---------------------------------------------*/
#define PMC_ODU_TFRM_REG_OH_INSERT_UNUSED_MASK        0xfff80080
#define ODU_TFRM_REG_OH_INSERT_BIT_RCP_EN_MSK         0x00040000
#define ODU_TFRM_REG_OH_INSERT_BIT_RCP_EN_OFF         18
#define ODU_TFRM_REG_OH_INSERT_BIT_GCC1_INS_MSK       0x00020000
#define ODU_TFRM_REG_OH_INSERT_BIT_GCC1_INS_OFF       17
#define ODU_TFRM_REG_OH_INSERT_BIT_GCC2_INS_MSK       0x00010000
#define ODU_TFRM_REG_OH_INSERT_BIT_GCC2_INS_OFF       16
#define ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_MSK    0x0000ff00
#define ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF    8
#define ODU_TFRM_REG_OH_INSERT_BIT_RES2_INS_MSK       0x00000040
#define ODU_TFRM_REG_OH_INSERT_BIT_RES2_INS_OFF       6
#define ODU_TFRM_REG_OH_INSERT_BIT_EXP_INS_MSK        0x00000020
#define ODU_TFRM_REG_OH_INSERT_BIT_EXP_INS_OFF        5
#define ODU_TFRM_REG_OH_INSERT_BIT_RES1_INS_MSK       0x00000010
#define ODU_TFRM_REG_OH_INSERT_BIT_RES1_INS_OFF       4
#define ODU_TFRM_REG_OH_INSERT_BIT_PM_TCM_RES_INS_MSK 0x00000008
#define ODU_TFRM_REG_OH_INSERT_BIT_PM_TCM_RES_INS_OFF 3
#define ODU_TFRM_REG_OH_INSERT_BIT_TCM_ACT_INS_MSK    0x00000004
#define ODU_TFRM_REG_OH_INSERT_BIT_TCM_ACT_INS_OFF    2
#define ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_MSK       0x00000003
#define ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF       0

/* index definitions for PMC_ODU_TFRM_REG_CHANNEL_CTRL */
#define PMC_ODU_TFRM_REG_CHANNEL_CTRL_INDEX_N_MIN          0
#define PMC_ODU_TFRM_REG_CHANNEL_CTRL_INDEX_N_MAX          95
#define PMC_ODU_TFRM_REG_CHANNEL_CTRL_INDEX_N_SIZE         96
#define PMC_ODU_TFRM_REG_CHANNEL_CTRL_INDEX_N_OFFSET       0x40

/*-------------------------------------------------.
 | Register (0x0000bc34 + (N) * 0x40) CHANNEL_CTRL |
 +-------------------------------------------------+
 | bit  28    R/W  IAE_INS                         |
 | bit  26    R/W  TTI_OS_INS_PM                   |
 | bit  25    R/W  TTI_DAPI_INS_PM                 |
 | bit  24    R/W  TTI_SAPI_INS_PM                 |
 | bit  23:20 R/W  BEI_PM                          |
 | bit  19    R/W  BDI_PM                          |
 | bit  17    R/W  SF_SD_PROP                      |
 | bit  16    R/W  SSF_PROP                        |
 | bit  13    R/W  TCM6_TRANSPARENT                |
 | bit  12    R/W  TCM5_TRANSPARENT                |
 | bit  11    R/W  TCM4_TRANSPARENT                |
 | bit  10    R/W  TCM3_TRANSPARENT                |
 | bit  9     R/W  TCM2_TRANSPARENT                |
 | bit  8     R/W  TCM1_TRANSPARENT                |
 | bit  7     R/W  PM_TRANSPARENT                  |
 | bit  5:4   R/W  MAINT_SIG                       |
 | bit  1     R/W  DMP_SRC                         |
 | bit  0     R/W  DMP_VAL                         |
 +------------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_CTRL_UNUSED_MASK          0xe804c04c
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_IAE_INS_MSK          0x10000000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_IAE_INS_OFF          28
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_OS_INS_PM_MSK    0x04000000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_OS_INS_PM_OFF    26
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_DAPI_INS_PM_MSK  0x02000000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_DAPI_INS_PM_OFF  25
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_SAPI_INS_PM_MSK  0x01000000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_SAPI_INS_PM_OFF  24
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_MSK           0x00f00000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF           20
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_BDI_PM_MSK           0x00080000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_BDI_PM_OFF           19
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_SF_SD_PROP_MSK       0x00020000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_SF_SD_PROP_OFF       17
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_SSF_PROP_MSK         0x00010000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_SSF_PROP_OFF         16
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM6_TRANSPARENT_MSK 0x00002000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM6_TRANSPARENT_OFF 13
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM5_TRANSPARENT_MSK 0x00001000
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM5_TRANSPARENT_OFF 12
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM4_TRANSPARENT_MSK 0x00000800
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM4_TRANSPARENT_OFF 11
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM3_TRANSPARENT_MSK 0x00000400
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM3_TRANSPARENT_OFF 10
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM2_TRANSPARENT_MSK 0x00000200
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM2_TRANSPARENT_OFF 9
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM1_TRANSPARENT_MSK 0x00000100
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM1_TRANSPARENT_OFF 8
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_PM_TRANSPARENT_MSK   0x00000080
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_PM_TRANSPARENT_OFF   7
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_MSK        0x00000030
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF        4
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_SRC_MSK          0x00000002
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_SRC_OFF          1
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_VAL_MSK          0x00000001
#define ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_VAL_OFF          0

/*---------------------------------------.
 | Register 0x0000d500 CHANNEL_PASSTHRU0 |
 +---------------------------------------+
 | bit  31:0 R/W  PASSTHRU               |
 +--------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF 0

/*---------------------------------------.
 | Register 0x0000d504 CHANNEL_PASSTHRU1 |
 +---------------------------------------+
 | bit  31:0 R/W  PASSTHRU               |
 +--------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF 0

/*---------------------------------------.
 | Register 0x0000d508 CHANNEL_PASSTHRU2 |
 +---------------------------------------+
 | bit  31:0 R/W  PASSTHRU               |
 +--------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF 0

/*---------------------------------------------.
 | Register 0x0000d50c CHANNEL_INT_STAT0_IAE_I |
 +---------------------------------------------+
 | bit  31:0 R/W  IAE_I                        |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF   0

/*---------------------------------------------.
 | Register 0x0000d510 CHANNEL_INT_STAT1_IAE_I |
 +---------------------------------------------+
 | bit  31:0 R/W  IAE_I                        |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF   0

/*---------------------------------------------.
 | Register 0x0000d514 CHANNEL_INT_STAT2_IAE_I |
 +---------------------------------------------+
 | bit  31:0 R/W  IAE_I                        |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF   0

/*-----------------------------------------------.
 | Register 0x0000d518 CHANNEL_INT_STAT3_DM_PM_I |
 +-----------------------------------------------+
 | bit  31:0 R/W  DM_PM_I                        |
 +----------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF 0

/*-----------------------------------------------.
 | Register 0x0000d51c CHANNEL_INT_STAT4_DM_PM_I |
 +-----------------------------------------------+
 | bit  31:0 R/W  DM_PM_I                        |
 +----------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF 0

/*-----------------------------------------------.
 | Register 0x0000d520 CHANNEL_INT_STAT5_DM_PM_I |
 +-----------------------------------------------+
 | bit  31:0 R/W  DM_PM_I                        |
 +----------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d524 POOL_INT_STAT0_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d528 POOL_INT_STAT1_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d52c POOL_INT_STAT2_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d530 POOL_INT_STAT3_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d534 POOL_INT_STAT4_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d538 POOL_INT_STAT5_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d53c POOL_INT_STAT6_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF 0

/*---------------------------------------------.
 | Register 0x0000d540 POOL_INT_STAT7_DM_PID_I |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PID_I                     |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF 0

/*-----------------------------------------------------.
 | Register 0x0000d54c FIFO_INT_STAT_DM_FIFO_OVR_UDR_I |
 +-----------------------------------------------------+
 | bit  1 R/W  DM_FIFO_UDR_I                           |
 | bit  0 R/W  DM_FIFO_OVR_I                           |
 +----------------------------------------------------*/
#define PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_UNUSED_MASK       0xfffffffc
#define ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_MSK 0x00000002
#define ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_OFF 1
#define ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_MSK 0x00000001
#define ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_OFF 0

/*--------------------------------------------.
 | Register 0x0000d64c CHANNEL_INT_VAL0_IAE_V |
 +--------------------------------------------+
 | bit  31:0 R  IAE_V                         |
 +-------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF   0

/*--------------------------------------------.
 | Register 0x0000d650 CHANNEL_INT_VAL1_IAE_V |
 +--------------------------------------------+
 | bit  31:0 R  IAE_V                         |
 +-------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF   0

/*--------------------------------------------.
 | Register 0x0000d654 CHANNEL_INT_VAL2_IAE_V |
 +--------------------------------------------+
 | bit  31:0 R  IAE_V                         |
 +-------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF   0

/*-------------------------------------------.
 | Register 0x0000d658 CHANNEL_INT_EN0_IAE_E |
 +-------------------------------------------+
 | bit  31:0 R/W  IAE_E                      |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF   0

/*-------------------------------------------.
 | Register 0x0000d65c CHANNEL_INT_EN1_IAE_E |
 +-------------------------------------------+
 | bit  31:0 R/W  IAE_E                      |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF   0

/*-------------------------------------------.
 | Register 0x0000d660 CHANNEL_INT_EN2_IAE_E |
 +-------------------------------------------+
 | bit  31:0 R/W  IAE_E                      |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_MSK   0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF   0

/*---------------------------------------------.
 | Register 0x0000d664 CHANNEL_INT_EN3_DM_PM_E |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PM_E                      |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF 0

/*---------------------------------------------.
 | Register 0x0000d668 CHANNEL_INT_EN4_DM_PM_E |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PM_E                      |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF 0

/*---------------------------------------------.
 | Register 0x0000d66c CHANNEL_INT_EN5_DM_PM_E |
 +---------------------------------------------+
 | bit  31:0 R/W  DM_PM_E                      |
 +--------------------------------------------*/
#define PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_UNUSED_MASK 0x00000000
#define ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_MSK 0xffffffff
#define ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d670 POOL_INT_EN0_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d674 POOL_INT_EN1_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d678 POOL_INT_EN2_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d67c POOL_INT_EN3_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d680 POOL_INT_EN4_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d684 POOL_INT_EN5_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d688 POOL_INT_EN6_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000d68c POOL_INT_EN7_DM_PID_E |
 +-------------------------------------------+
 | bit  31:0 R/W  DM_PID_E                   |
 +------------------------------------------*/
#define PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_UNUSED_MASK  0x00000000
#define ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_MSK 0xffffffff
#define ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF 0

/*---------------------------------------------------.
 | Register 0x0000d698 FIFO_INT_EN_DM_FIFO_OVR_UDR_E |
 +---------------------------------------------------+
 | bit  1 R/W  DM_FIFO_UDR_E                         |
 | bit  0 R/W  DM_FIFO_OVR_E                         |
 +--------------------------------------------------*/
#define PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_UNUSED_MASK       0xfffffffc
#define ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_UDR_E_MSK 0x00000002
#define ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_UDR_E_OFF 1
#define ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_OVR_E_MSK 0x00000001
#define ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_OVR_E_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODU_TFRM_REGS_H */
