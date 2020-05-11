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
 *     s16_csu_pcbi block
 *****************************************************************************/
#ifndef _S16_CSU_PCBI_REGS_H
#define _S16_CSU_PCBI_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PCBI_REG_CSU0_PLL_OOL_STAT_REG          0x00000500
#define PMC_PCBI_REG_CSU0_VCOC_STAT_REG             0x00000508
#define PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG   0x0000050c
#define PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1       0x00000510
#define PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2       0x00000514
#define PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3       0x00000518
#define PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG      0x0000051c
#define PMC_PCBI_REG_CSU0_CONFIG_REG2               0x00000520
#define PMC_PCBI_REG_CSU0_CONFIG_REG3               0x00000524
#define PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3     0x00000528
#define PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1          0x0000052c
#define PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2          0x00000530
#define PMC_PCBI_REG_CSU0_RESET_CONFIG_REG          0x00000534
#define PMC_PCBI_REG_CSU_TEST_CONFIG_REG            0x00000538
#define PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG      0x0000053c
#define PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG        0x00000540
#define PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1      0x00000548
#define PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2      0x0000054c
#define PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3      0x00000550
#define PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG 0x00000554
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1         0x00000558
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2         0x0000055c
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3         0x00000560
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4         0x00000564
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5         0x00000568
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6         0x0000056c
#define PMC_PCBI_REG_INT_EN_REG                     0x00000570
#define PMC_PCBI_REG_INT_REG                        0x00000574
#define PMC_PCBI_REG_INT_VAL_REG                    0x00000578
#define PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1           0x0000057c
#define PMC_PCBI_REG_ACB_FT_CONFIG_REG1             0x00000580
#define PMC_PCBI_REG_PMM_CONFIG_REG1                0x00000584
#define PMC_PCBI_REG_PMM_CONFIG_REG2                0x00000588
#define PMC_PCBI_REG_PMM_CONFIG_REG3                0x0000058c
#define PMC_PCBI_REG_PMM_STATUS_REG1                0x00000590
#define PMC_PCBI_REG_PMM_STATUS_REG2                0x00000594

/*-------------------------------------------.
 | Register 0x00000500 CSU0_PLL_OOL_STAT_REG |
 +-------------------------------------------+
 | bit  20:0 R  CSU0_PPM_OFF_RD_VAL          |
 +------------------------------------------*/
#define PMC_PCBI_REG_CSU0_PLL_OOL_STAT_REG_UNUSED_MASK             0xffe00000
#define PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_MSK 0x001fffff
#define PCBI_REG_CSU0_PLL_OOL_STAT_REG_BIT_CSU0_PPM_OFF_RD_VAL_OFF 0

/*----------------------------------------.
 | Register 0x00000508 CSU0_VCOC_STAT_REG |
 +----------------------------------------+
 | bit  15   R  VCOC_MODE_IN_RANGE        |
 | bit  14:7 R  CSU0_VCOC_BIST_MODE_COUNT |
 | bit  6:1  R  CSU0_VCOC_MODE_OUT        |
 +---------------------------------------*/
#define PMC_PCBI_REG_CSU0_VCOC_STAT_REG_UNUSED_MASK                   0xffff0001
#define PCBI_REG_CSU0_VCOC_STAT_REG_BIT_VCOC_MODE_IN_RANGE_MSK        0x00008000
#define PCBI_REG_CSU0_VCOC_STAT_REG_BIT_VCOC_MODE_IN_RANGE_OFF        15
#define PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_MSK 0x00007f80
#define PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_BIST_MODE_COUNT_OFF 7
#define PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_MSK        0x0000007e
#define PCBI_REG_CSU0_VCOC_STAT_REG_BIT_CSU0_VCOC_MODE_OUT_OFF        1

/*--------------------------------------------------.
 | Register 0x0000050c CSU0_EADSM_LOCK_DET_STAT_REG |
 +--------------------------------------------------+
 | bit  17:0 R  CSU0_EADSM_SYSCLK_CNT               |
 +-------------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_UNUSED_MASK               0xfffc0000
#define PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_MSK 0x0003ffff
#define PCBI_REG_CSU0_EADSM_LOCK_DET_STAT_REG_BIT_CSU0_EADSM_SYSCLK_CNT_OFF 0

/*----------------------------------------------.
 | Register 0x00000510 CSU0_PLL_OOL_CONFIG_REG1 |
 +----------------------------------------------+
 | bit  23   R/W  CSU0_SW_RESET                 |
 | bit  22   R/W  CSU0_PLL_RD                   |
 | bit  20   R/W  CSU0_RESET_HISTORY            |
 | bit  19:0 R/W  CSU0_CLK_CNT_MAX              |
 +---------------------------------------------*/
#define PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_UNUSED_MASK            0xff200000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_SW_RESET_MSK      0x00800000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_SW_RESET_OFF      23
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_PLL_RD_MSK        0x00400000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_PLL_RD_OFF        22
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_RESET_HISTORY_MSK 0x00100000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_RESET_HISTORY_OFF 20
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_MSK   0x000fffff
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG1_BIT_CSU0_CLK_CNT_MAX_OFF   0

/*----------------------------------------------.
 | Register 0x00000514 CSU0_PLL_OOL_CONFIG_REG2 |
 +----------------------------------------------+
 | bit  31:16 R/W  CSU0_PASS_CNT_MAX            |
 | bit  15:0  R/W  CSU0_FAIL_CNT_MAX            |
 +---------------------------------------------*/
#define PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_UNUSED_MASK           0x00000000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_MSK 0xffff0000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_PASS_CNT_MAX_OFF 16
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_MSK 0x0000ffff
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG2_BIT_CSU0_FAIL_CNT_MAX_OFF 0

/*----------------------------------------------.
 | Register 0x00000518 CSU0_PLL_OOL_CONFIG_REG3 |
 +----------------------------------------------+
 | bit  31:28 R/W  CSU0_LOCK_PRECISION          |
 | bit  27:20 R/W  CSU0_LOCK_CNT_MAX            |
 +---------------------------------------------*/
#define PMC_PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_UNUSED_MASK             0x000fffff
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_MSK 0xf0000000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_PRECISION_OFF 28
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_MSK   0x0ff00000
#define PCBI_REG_CSU0_PLL_OOL_CONFIG_REG3_BIT_CSU0_LOCK_CNT_MAX_OFF   20

/*-----------------------------------------------.
 | Register 0x0000051c CSU0_MABC_TEST_CONFIG_REG |
 +-----------------------------------------------+
 | bit  17    R/W  VBIAS_CSU_ATMSB               |
 | bit  16    R/W  CSU_ENB                       |
 | bit  15    R/W  CSU_CTRL_15                   |
 | bit  14:13 R/W  CSU_CTRL_14_13                |
 | bit  12    R/W  CSU_CTRL_12                   |
 | bit  11    R/W  CSU_CTRL_11                   |
 | bit  10    R/W  CSU_CTRL_10                   |
 | bit  9:6   R/W  CSU_CTRL_9_6                  |
 | bit  5     R/W  CSU_CTRL_5                    |
 | bit  4     R/W  CSU_CTRL_4                    |
 | bit  3     R/W  CSU_CTRL_3                    |
 | bit  2     R/W  CSU_CTRL_2                    |
 | bit  1:0   R/W  CSU_CTRL_1_0                  |
 +----------------------------------------------*/
#define PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_UNUSED_MASK         0xfffc0000
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_VBIAS_CSU_ATMSB_MSK 0x00020000
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_VBIAS_CSU_ATMSB_OFF 17
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_ENB_MSK         0x00010000
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_ENB_OFF         16
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_15_MSK     0x00008000
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_15_OFF     15
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_MSK  0x00006000
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_14_13_OFF  13
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_12_MSK     0x00001000
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_12_OFF     12
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_11_MSK     0x00000800
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_11_OFF     11
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_10_MSK     0x00000400
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_10_OFF     10
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_MSK    0x000003c0
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_9_6_OFF    6
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_5_MSK      0x00000020
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_5_OFF      5
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_4_MSK      0x00000010
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_4_OFF      4
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_3_MSK      0x00000008
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_3_OFF      3
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_2_MSK      0x00000004
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_2_OFF      2
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_MSK    0x00000003
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG_BIT_CSU_CTRL_1_0_OFF    0

/*--------------------------------------.
 | Register 0x00000520 CSU0_CONFIG_REG2 |
 +--------------------------------------+
 | bit  29    R/W  CSU0_CSU_MODE_29     |
 | bit  28    R/W  CSU0_CSU_MODE_28     |
 | bit  27    R/W  CSU0_CSU_MODE_27     |
 | bit  26:25 R/W  CSU0_CSU_MODE_26_25  |
 | bit  24:23 R/W  CSU0_CSU_MODE_24_23  |
 | bit  22    R/W  CSU0_CSU_MODE_22     |
 | bit  21    R/W  CSU0_CSU_MODE_21     |
 | bit  20    R/W  CSU0_CSU_MODE_20     |
 | bit  19    R/W  CSU0_CSU_MODE_19     |
 | bit  17:16 R/W  CSU0_CSU_MODE_17_16  |
 | bit  15:10 R/W  CSU0_CSU_MODE_15_10  |
 | bit  9:8   R/W  CSU0_CSU_MODE_9_8    |
 | bit  7:6   R/W  CSU0_CSU_MODE_7_6    |
 | bit  5:3   R/W  CSU0_CSU_MODE_5_3    |
 | bit  2:0   R/W  CSU0_CSU_MODE_2_0    |
 +-------------------------------------*/
#define PMC_PCBI_REG_CSU0_CONFIG_REG2_UNUSED_MASK             0xc0040000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_29_MSK    0x20000000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_29_OFF    29
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_28_MSK    0x10000000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_28_OFF    28
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_27_MSK    0x08000000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_27_OFF    27
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_MSK 0x06000000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_26_25_OFF 25
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_MSK 0x01800000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_24_23_OFF 23
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_22_MSK    0x00400000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_22_OFF    22
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_21_MSK    0x00200000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_21_OFF    21
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_20_MSK    0x00100000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_20_OFF    20
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_19_MSK    0x00080000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_19_OFF    19
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_MSK 0x00030000
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_17_16_OFF 16
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_MSK 0x0000fc00
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_15_10_OFF 10
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_MSK   0x00000300
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_9_8_OFF   8
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_MSK   0x000000c0
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_7_6_OFF   6
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_MSK   0x00000038
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_5_3_OFF   3
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_MSK   0x00000007
#define PCBI_REG_CSU0_CONFIG_REG2_BIT_CSU0_CSU_MODE_2_0_OFF   0

/*---------------------------------------.
 | Register 0x00000524 CSU0_CONFIG_REG3  |
 +---------------------------------------+
 | bit  4   R/W  CSU0_CSU_FB_CTRL_OVR_EN |
 | bit  3:0 R/W  CSU0_CSU_FB_CTRL        |
 +--------------------------------------*/
#define PMC_PCBI_REG_CSU0_CONFIG_REG3_UNUSED_MASK                 0xffffffe0
#define PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OVR_EN_MSK 0x00000010
#define PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OVR_EN_OFF 4
#define PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_MSK        0x0000000f
#define PCBI_REG_CSU0_CONFIG_REG3_BIT_CSU0_CSU_FB_CTRL_OFF        0

/*------------------------------------------------.
 | Register 0x00000528 CSU0_MABC_TEST_CONFIG_REG3 |
 +------------------------------------------------+
 | bit  8:4 R/W  CSU0_VC_LPF_CODE                 |
 | bit  3:0 R/W  CSU0_VC_LPF_MODE                 |
 +-----------------------------------------------*/
#define PMC_PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_UNUSED_MASK          0xfffffe00
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_MSK 0x000001f0
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_CODE_OFF 4
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_MSK 0x0000000f
#define PCBI_REG_CSU0_MABC_TEST_CONFIG_REG3_BIT_CSU0_VC_LPF_MODE_OFF 0

/*-------------------------------------------.
 | Register 0x0000052c CSU0_VCOC_CONFIG_REG1 |
 +-------------------------------------------+
 | bit  17:12 R/W  CSU0_MAX_THRESHOLD        |
 | bit  11:6  R/W  CSU0_MIN_THRESHOLD        |
 | bit  5     R/W  CSU0_VCOC_START           |
 | bit  4:1   R/W  CSU0_VCOC_LOOP            |
 | bit  0     R/W  CSU0_VCOC_EN              |
 +------------------------------------------*/
#define PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG1_UNUSED_MASK            0xfffc0000
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_MSK 0x0003f000
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MAX_THRESHOLD_OFF 12
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_MSK 0x00000fc0
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_MIN_THRESHOLD_OFF 6
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_START_MSK    0x00000020
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_START_OFF    5
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_MSK     0x0000001e
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_LOOP_OFF     1
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_EN_MSK       0x00000001
#define PCBI_REG_CSU0_VCOC_CONFIG_REG1_BIT_CSU0_VCOC_EN_OFF       0

/*-------------------------------------------.
 | Register 0x00000530 CSU0_VCOC_CONFIG_REG2 |
 +-------------------------------------------+
 | bit  31:24 R/W  CSU0_VCOC_SETTLING_TIME   |
 | bit  23:18 R/W  CSU0_MIN_MODE             |
 | bit  17:12 R/W  CSU0_MAX_MODE             |
 | bit  11    R/W  CSU0_VCOC_BIST_EN         |
 | bit  10:5  R/W  CSU0_OFF_EQ_WINDOW        |
 | bit  4     R/W  CSU0_SWEEP_DIR            |
 +------------------------------------------*/
#define PMC_PCBI_REG_CSU0_VCOC_CONFIG_REG2_UNUSED_MASK                 0x0000000f
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_MSK 0xff000000
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_SETTLING_TIME_OFF 24
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_MSK           0x00fc0000
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MIN_MODE_OFF           18
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_MSK           0x0003f000
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_MAX_MODE_OFF           12
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_BIST_EN_MSK       0x00000800
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_VCOC_BIST_EN_OFF       11
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_MSK      0x000007e0
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_OFF_EQ_WINDOW_OFF      5
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_SWEEP_DIR_MSK          0x00000010
#define PCBI_REG_CSU0_VCOC_CONFIG_REG2_BIT_CSU0_SWEEP_DIR_OFF          4

/*-------------------------------------------.
 | Register 0x00000534 CSU0_RESET_CONFIG_REG |
 +-------------------------------------------+
 | bit  4 R/W  CSU0_ADSM_SFT_RSTB            |
 | bit  3 R/W  CSU0_ADSM_HRD_RSTB            |
 | bit  2 R/W  CSU0_PLL_RSTB                 |
 | bit  1 R/W  CSU_ARSTB                     |
 | bit  0 R/W  CSU0_VCOC_RSTB                |
 +------------------------------------------*/
#define PMC_PCBI_REG_CSU0_RESET_CONFIG_REG_UNUSED_MASK            0xffffffe0
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_SFT_RSTB_MSK 0x00000010
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_SFT_RSTB_OFF 4
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_HRD_RSTB_MSK 0x00000008
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_ADSM_HRD_RSTB_OFF 3
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_PLL_RSTB_MSK      0x00000004
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_PLL_RSTB_OFF      2
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU_ARSTB_MSK          0x00000002
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU_ARSTB_OFF          1
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_VCOC_RSTB_MSK     0x00000001
#define PCBI_REG_CSU0_RESET_CONFIG_REG_BIT_CSU0_VCOC_RSTB_OFF     0

/*-----------------------------------------.
 | Register 0x00000538 CSU_TEST_CONFIG_REG |
 +-----------------------------------------+
 | bit  0 R/W  CSU_ATMSB                   |
 +----------------------------------------*/
#define PMC_PCBI_REG_CSU_TEST_CONFIG_REG_UNUSED_MASK   0xfffffffe
#define PCBI_REG_CSU_TEST_CONFIG_REG_BIT_CSU_ATMSB_MSK 0x00000001
#define PCBI_REG_CSU_TEST_CONFIG_REG_BIT_CSU_ATMSB_OFF 0

/*-----------------------------------------------.
 | Register 0x0000053c CLK_DEGLITCHER_CONFIG_REG |
 +-----------------------------------------------+
 | bit  1 R/W  SEL_DIVCLK_16_OR_64               |
 | bit  0 R/W  CLK_DEGLITCHER_EN                 |
 +----------------------------------------------*/
#define PMC_PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_UNUSED_MASK             0xfffffffc
#define PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_SEL_DIVCLK_16_OR_64_MSK 0x00000002
#define PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_SEL_DIVCLK_16_OR_64_OFF 1
#define PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_CLK_DEGLITCHER_EN_MSK   0x00000001
#define PCBI_REG_CLK_DEGLITCHER_CONFIG_REG_BIT_CLK_DEGLITCHER_EN_OFF   0

/*---------------------------------------------.
 | Register 0x00000540 GLOBAL_RESET_CONFIG_REG |
 +---------------------------------------------+
 | bit  0 R/W  GLBL_RSTB                       |
 +--------------------------------------------*/
#define PMC_PCBI_REG_GLOBAL_RESET_CONFIG_REG_UNUSED_MASK   0xfffffffe
#define PCBI_REG_GLOBAL_RESET_CONFIG_REG_BIT_GLBL_RSTB_MSK 0x00000001
#define PCBI_REG_GLOBAL_RESET_CONFIG_REG_BIT_GLBL_RSTB_OFF 0

/*-----------------------------------------------.
 | Register 0x00000548 CSU0_FRAC_DIV_CONFIG_REG1 |
 +-----------------------------------------------+
 | bit  26 R/W  CSU0_FRAC_DIV_MOD_EXT_EN         |
 | bit  25 R/W  CSU0_FRAC_DIV_MOD_OVR_EN         |
 +----------------------------------------------*/
#define PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_UNUSED_MASK                  0xf9ffffff
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_EXT_EN_MSK 0x04000000
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_EXT_EN_OFF 26
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_OVR_EN_MSK 0x02000000
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG1_BIT_CSU0_FRAC_DIV_MOD_OVR_EN_OFF 25

/*-----------------------------------------------.
 | Register 0x0000054c CSU0_FRAC_DIV_CONFIG_REG2 |
 +-----------------------------------------------+
 | bit  15:0 R/W  CSU0_FRAC_DIV_MOD_OVR_31_16    |
 +----------------------------------------------*/
#define PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_UNUSED_MASK                     0xffff0000
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_MSK 0x0000ffff
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG2_BIT_CSU0_FRAC_DIV_MOD_OVR_31_16_OFF 0

/*-----------------------------------------------.
 | Register 0x00000550 CSU0_FRAC_DIV_CONFIG_REG3 |
 +-----------------------------------------------+
 | bit  8:0 R/W  CSU0_FRAC_DIV_MOD_OVR_40_32     |
 +----------------------------------------------*/
#define PMC_PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_UNUSED_MASK                     0xfffffe00
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_MSK 0x000001ff
#define PCBI_REG_CSU0_FRAC_DIV_CONFIG_REG3_BIT_CSU0_FRAC_DIV_MOD_OVR_40_32_OFF 0

/*----------------------------------------------------.
 | Register 0x00000554 CSU0_EADSM_LOCK_DET_CONFIG_REG |
 +----------------------------------------------------+
 | bit  23:6 R/W  CSU0_EADSM_CLK_CNT_MAX              |
 | bit  5:0  R/W  CSU0_EADSM_LOCK_CNT                 |
 +---------------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_UNUSED_MASK                0xff000000
#define PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_MSK 0x00ffffc0
#define PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_CLK_CNT_MAX_OFF 6
#define PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_MSK    0x0000003f
#define PCBI_REG_CSU0_EADSM_LOCK_DET_CONFIG_REG_BIT_CSU0_EADSM_LOCK_CNT_OFF    0

/*--------------------------------------------.
 | Register 0x00000558 CSU0_EADSM_CONFIG_REG1 |
 +--------------------------------------------+
 | bit  4:3 R/W  CSU0_DITHER_SEL              |
 | bit  2   R/W  CSU0_MULT2                   |
 | bit  1   R/W  CSU0_SEED                    |
 | bit  0   R/W  CSU0_CLKO_POLARITY           |
 +-------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG1_UNUSED_MASK            0xffffffe0
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_MSK    0x00000018
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_DITHER_SEL_OFF    3
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_MULT2_MSK         0x00000004
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_MULT2_OFF         2
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_SEED_MSK          0x00000002
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_SEED_OFF          1
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_CLKO_POLARITY_MSK 0x00000001
#define PCBI_REG_CSU0_EADSM_CONFIG_REG1_BIT_CSU0_CLKO_POLARITY_OFF 0

/*--------------------------------------------.
 | Register 0x0000055c CSU0_EADSM_CONFIG_REG2 |
 +--------------------------------------------+
 | bit  9:0 R/W  CSU0_INT_DIV                 |
 +-------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG2_UNUSED_MASK      0xfffffc00
#define PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_MSK 0x000003ff
#define PCBI_REG_CSU0_EADSM_CONFIG_REG2_BIT_CSU0_INT_DIV_OFF 0

/*--------------------------------------------.
 | Register 0x00000560 CSU0_EADSM_CONFIG_REG3 |
 +--------------------------------------------+
 | bit  31:0 R/W  CSU0_DENOMIN_31_0           |
 +-------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG3_UNUSED_MASK           0x00000000
#define PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_MSK 0xffffffff
#define PCBI_REG_CSU0_EADSM_CONFIG_REG3_BIT_CSU0_DENOMIN_31_0_OFF 0

/*--------------------------------------------.
 | Register 0x00000564 CSU0_EADSM_CONFIG_REG4 |
 +--------------------------------------------+
 | bit  7:0 R/W  CSU0_DENOMIN_39_32           |
 +-------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG4_UNUSED_MASK            0xffffff00
#define PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_MSK 0x000000ff
#define PCBI_REG_CSU0_EADSM_CONFIG_REG4_BIT_CSU0_DENOMIN_39_32_OFF 0

/*--------------------------------------------.
 | Register 0x00000568 CSU0_EADSM_CONFIG_REG5 |
 +--------------------------------------------+
 | bit  31:0 R/W  CSU0_FRAC_DIV_31_0          |
 +-------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG5_UNUSED_MASK            0x00000000
#define PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_MSK 0xffffffff
#define PCBI_REG_CSU0_EADSM_CONFIG_REG5_BIT_CSU0_FRAC_DIV_31_0_OFF 0

/*--------------------------------------------.
 | Register 0x0000056c CSU0_EADSM_CONFIG_REG6 |
 +--------------------------------------------+
 | bit  7:0 R/W  CSU0_FRAC_DIV_39_32          |
 +-------------------------------------------*/
#define PMC_PCBI_REG_CSU0_EADSM_CONFIG_REG6_UNUSED_MASK             0xffffff00
#define PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_MSK 0x000000ff
#define PCBI_REG_CSU0_EADSM_CONFIG_REG6_BIT_CSU0_FRAC_DIV_39_32_OFF 0

/*-----------------------------------------------------.
 | Register 0x00000570 INT_EN_REG                      |
 +-----------------------------------------------------+
 | bit  14 R/W  UPDATE_RATE_OVERFLOW_E                 |
 | bit  13 R/W  ACB_DESKEW_OVERFLOW_E                  |
 | bit  12 R/W  ACB_DESKEW_UNDERFLOW_E                 |
 | bit  11 R/W  VC_LPF_OUT1_E                          |
 | bit  10 R/W  VC_LPF_OUT0_E                          |
 | bit  9  R/W  CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E |
 | bit  8  R/W  CSU0_EADSM_LOCKDET_LOCK_E              |
 | bit  7  R/W  CSU0_PLL_LOCKDET_PPM_OFF_UPD_E         |
 | bit  6  R/W  CSU0_VCOC_BIST_ERR_E                   |
 | bit  5  R/W  CSU0_DIVCLK_DEAD_E                     |
 | bit  4  R/W  CSU0_IN_RANGE_N_FAILS_E                |
 | bit  3  R/W  CSU0_IN_RANGE_N_PASSES_E               |
 | bit  2  R/W  CSU0_LOCK_LOSS_E                       |
 | bit  1  R/W  CSU0_LOCK_GAIN_E                       |
 | bit  0  R/W  CSU0_VCOC_DONE_E                       |
 +----------------------------------------------------*/
#define PMC_PCBI_REG_INT_EN_REG_UNUSED_MASK                                0xffff8000
#define PCBI_REG_INT_EN_REG_BIT_UPDATE_RATE_OVERFLOW_E_MSK                 0x00004000
#define PCBI_REG_INT_EN_REG_BIT_UPDATE_RATE_OVERFLOW_E_OFF                 14
#define PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_OVERFLOW_E_MSK                  0x00002000
#define PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_OVERFLOW_E_OFF                  13
#define PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_UNDERFLOW_E_MSK                 0x00001000
#define PCBI_REG_INT_EN_REG_BIT_ACB_DESKEW_UNDERFLOW_E_OFF                 12
#define PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT1_E_MSK                          0x00000800
#define PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT1_E_OFF                          11
#define PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT0_E_MSK                          0x00000400
#define PCBI_REG_INT_EN_REG_BIT_VC_LPF_OUT0_E_OFF                          10
#define PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_MSK 0x00000200
#define PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_OFF 9
#define PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_E_MSK              0x00000100
#define PCBI_REG_INT_EN_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_E_OFF              8
#define PCBI_REG_INT_EN_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_MSK         0x00000080
#define PCBI_REG_INT_EN_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_OFF         7
#define PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_BIST_ERR_E_MSK                   0x00000040
#define PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_BIST_ERR_E_OFF                   6
#define PCBI_REG_INT_EN_REG_BIT_CSU0_DIVCLK_DEAD_E_MSK                     0x00000020
#define PCBI_REG_INT_EN_REG_BIT_CSU0_DIVCLK_DEAD_E_OFF                     5
#define PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_FAILS_E_MSK                0x00000010
#define PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_FAILS_E_OFF                4
#define PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_PASSES_E_MSK               0x00000008
#define PCBI_REG_INT_EN_REG_BIT_CSU0_IN_RANGE_N_PASSES_E_OFF               3
#define PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_LOSS_E_MSK                       0x00000004
#define PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_LOSS_E_OFF                       2
#define PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_GAIN_E_MSK                       0x00000002
#define PCBI_REG_INT_EN_REG_BIT_CSU0_LOCK_GAIN_E_OFF                       1
#define PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_DONE_E_MSK                       0x00000001
#define PCBI_REG_INT_EN_REG_BIT_CSU0_VCOC_DONE_E_OFF                       0

/*-----------------------------------------------------.
 | Register 0x00000574 INT_REG                         |
 +-----------------------------------------------------+
 | bit  14 R/W  UPDATE_RATE_OVERFLOW_I                 |
 | bit  13 R/W  ACB_DESKEW_OVERFLOW_I                  |
 | bit  12 R/W  ACB_DESKEW_UNDERFLOW_I                 |
 | bit  11 R/W  VC_LPF_OUT1_I                          |
 | bit  10 R/W  VC_LPF_OUT0_I                          |
 | bit  9  R/W  CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I |
 | bit  8  R/W  CSU0_EADSM_LOCKDET_LOCK_I              |
 | bit  7  R/W  CSU0_PLL_LOCKDET_PPM_OFF_UPD_I         |
 | bit  6  R/W  CSU0_VCOC_BIST_ERR_I                   |
 | bit  5  R/W  CSU0_DIVCLK_DEAD_I                     |
 | bit  4  R/W  CSU0_IN_RANGE_N_FAILS_I                |
 | bit  3  R/W  CSU0_IN_RANGE_N_PASSES_I               |
 | bit  2  R/W  CSU0_LOCK_LOSS_I                       |
 | bit  1  R/W  CSU0_LOCK_GAIN_I                       |
 | bit  0  R/W  CSU0_VCOC_DONE_I                       |
 +----------------------------------------------------*/
#define PMC_PCBI_REG_INT_REG_UNUSED_MASK                                0xffff8000
#define PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_MSK                 0x00004000
#define PCBI_REG_INT_REG_BIT_UPDATE_RATE_OVERFLOW_I_OFF                 14
#define PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_MSK                  0x00002000
#define PCBI_REG_INT_REG_BIT_ACB_DESKEW_OVERFLOW_I_OFF                  13
#define PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_MSK                 0x00001000
#define PCBI_REG_INT_REG_BIT_ACB_DESKEW_UNDERFLOW_I_OFF                 12
#define PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_MSK                          0x00000800
#define PCBI_REG_INT_REG_BIT_VC_LPF_OUT1_I_OFF                          11
#define PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_MSK                          0x00000400
#define PCBI_REG_INT_REG_BIT_VC_LPF_OUT0_I_OFF                          10
#define PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_MSK 0x00000200
#define PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_OFF 9
#define PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_MSK              0x00000100
#define PCBI_REG_INT_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_I_OFF              8
#define PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_MSK         0x00000080
#define PCBI_REG_INT_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_OFF         7
#define PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_MSK                   0x00000040
#define PCBI_REG_INT_REG_BIT_CSU0_VCOC_BIST_ERR_I_OFF                   6
#define PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_MSK                     0x00000020
#define PCBI_REG_INT_REG_BIT_CSU0_DIVCLK_DEAD_I_OFF                     5
#define PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_MSK                0x00000010
#define PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_FAILS_I_OFF                4
#define PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_MSK               0x00000008
#define PCBI_REG_INT_REG_BIT_CSU0_IN_RANGE_N_PASSES_I_OFF               3
#define PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_MSK                       0x00000004
#define PCBI_REG_INT_REG_BIT_CSU0_LOCK_LOSS_I_OFF                       2
#define PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_MSK                       0x00000002
#define PCBI_REG_INT_REG_BIT_CSU0_LOCK_GAIN_I_OFF                       1
#define PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_MSK                       0x00000001
#define PCBI_REG_INT_REG_BIT_CSU0_VCOC_DONE_I_OFF                       0

/*---------------------------------------------------.
 | Register 0x00000578 INT_VAL_REG                   |
 +---------------------------------------------------+
 | bit  14 R  UPDATE_RATE_OVERFLOW_V                 |
 | bit  13 R  ACB_DESKEW_OVERFLOW_V                  |
 | bit  12 R  ACB_DESKEW_UNDERFLOW_V                 |
 | bit  11 R  VC_LPF_OUT1_V                          |
 | bit  10 R  VC_LPF_OUT0_V                          |
 | bit  9  R  CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V |
 | bit  8  R  CSU0_EADSM_LOCKDET_LOCK_V              |
 | bit  7  R  CSU0_PLL_LOCKDET_PPM_OFF_UPD_V         |
 | bit  6  R  CSU0_VCOC_BIST_ERR_V                   |
 | bit  5  R  CSU0_DIVCLK_DEAD_V                     |
 | bit  4  R  CSU0_IN_RANGE_N_FAILS_V                |
 | bit  3  R  CSU0_IN_RANGE_N_PASSES_V               |
 | bit  2  R  CSU0_LOCK_LOSS_V                       |
 | bit  1  R  CSU0_LOCK_GAIN_V                       |
 | bit  0  R  CSU0_VCOC_DONE_V                       |
 +--------------------------------------------------*/
#define PMC_PCBI_REG_INT_VAL_REG_UNUSED_MASK                                0xffff8000
#define PCBI_REG_INT_VAL_REG_BIT_UPDATE_RATE_OVERFLOW_V_MSK                 0x00004000
#define PCBI_REG_INT_VAL_REG_BIT_UPDATE_RATE_OVERFLOW_V_OFF                 14
#define PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_OVERFLOW_V_MSK                  0x00002000
#define PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_OVERFLOW_V_OFF                  13
#define PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_UNDERFLOW_V_MSK                 0x00001000
#define PCBI_REG_INT_VAL_REG_BIT_ACB_DESKEW_UNDERFLOW_V_OFF                 12
#define PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT1_V_MSK                          0x00000800
#define PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT1_V_OFF                          11
#define PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT0_V_MSK                          0x00000400
#define PCBI_REG_INT_VAL_REG_BIT_VC_LPF_OUT0_V_OFF                          10
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_MSK 0x00000200
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_OFF 9
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_V_MSK              0x00000100
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_EADSM_LOCKDET_LOCK_V_OFF              8
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_MSK         0x00000080
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_OFF         7
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_BIST_ERR_V_MSK                   0x00000040
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_BIST_ERR_V_OFF                   6
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_DIVCLK_DEAD_V_MSK                     0x00000020
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_DIVCLK_DEAD_V_OFF                     5
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_FAILS_V_MSK                0x00000010
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_FAILS_V_OFF                4
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_PASSES_V_MSK               0x00000008
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_IN_RANGE_N_PASSES_V_OFF               3
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_LOSS_V_MSK                       0x00000004
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_LOSS_V_OFF                       2
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_GAIN_V_MSK                       0x00000002
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_LOCK_GAIN_V_OFF                       1
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_DONE_V_MSK                       0x00000001
#define PCBI_REG_INT_VAL_REG_BIT_CSU0_VCOC_DONE_V_OFF                       0

/*------------------------------------------.
 | Register 0x0000057c CSU0_GLOBAL_CFG_REG1 |
 +------------------------------------------+
 | bit  23:22 R/W  TRCS_MODE_23_22          |
 | bit  14    R/W  TRCS_MODE_14             |
 | bit  10    R/W  TRCS_MODE_10             |
 +-----------------------------------------*/
#define PMC_PCBI_REG_CSU0_GLOBAL_CFG_REG1_UNUSED_MASK         0xff3fbbff
#define PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_MSK 0x00c00000
#define PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_23_22_OFF 22
#define PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_14_MSK    0x00004000
#define PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_14_OFF    14
#define PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_10_MSK    0x00000400
#define PCBI_REG_CSU0_GLOBAL_CFG_REG1_BIT_TRCS_MODE_10_OFF    10

/*----------------------------------------.
 | Register 0x00000580 ACB_FT_CONFIG_REG1 |
 +----------------------------------------+
 | bit  31    R/W  ACB_RSTB               |
 | bit  27    R/W  PDM_PHS_DET_DEL_SEL    |
 | bit  26    R/W  PDM_ACCUM_BYPASS       |
 | bit  25:21 R/W  PDM_TCLK2_DLY_SEL      |
 | bit  20:16 R/W  PDM_TCLK1_DLY_SEL      |
 | bit  15    R/W  PDM_EN                 |
 | bit  13:10 R/W  PDM_TCLK_SEL           |
 | bit  9:6   R/W  FRAC_DIV_MOD_EXT_SEL   |
 | bit  1     R/W  ACB_HOLDOVER           |
 | bit  0     R/W  ACB_FORCE_NOMINAL      |
 +---------------------------------------*/
#define PMC_PCBI_REG_ACB_FT_CONFIG_REG1_UNUSED_MASK              0x7000403c
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_RSTB_MSK             0x80000000
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_RSTB_OFF             31
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_PHS_DET_DEL_SEL_MSK  0x08000000
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_PHS_DET_DEL_SEL_OFF  27
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_ACCUM_BYPASS_MSK     0x04000000
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_ACCUM_BYPASS_OFF     26
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_MSK    0x03e00000
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK2_DLY_SEL_OFF    21
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_MSK    0x001f0000
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK1_DLY_SEL_OFF    16
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_EN_MSK               0x00008000
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_EN_OFF               15
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_MSK         0x00003c00
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_PDM_TCLK_SEL_OFF         10
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_MSK 0x000003c0
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_FRAC_DIV_MOD_EXT_SEL_OFF 6
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_HOLDOVER_MSK         0x00000002
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_HOLDOVER_OFF         1
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_FORCE_NOMINAL_MSK    0x00000001
#define PCBI_REG_ACB_FT_CONFIG_REG1_BIT_ACB_FORCE_NOMINAL_OFF    0

/*-------------------------------------.
 | Register 0x00000584 PMM_CONFIG_REG1 |
 +-------------------------------------+
 | bit  29:16 R/W  PMM_DSM_DENOM       |
 | bit  15:0  R/W  PMM_PKT_PERIOD      |
 +------------------------------------*/
#define PMC_PCBI_REG_PMM_CONFIG_REG1_UNUSED_MASK        0xc0000000
#define PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_MSK  0x3fff0000
#define PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_DSM_DENOM_OFF  16
#define PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_MSK 0x0000ffff
#define PCBI_REG_PMM_CONFIG_REG1_BIT_PMM_PKT_PERIOD_OFF 0

/*-------------------------------------.
 | Register 0x00000588 PMM_CONFIG_REG2 |
 +-------------------------------------+
 | bit  19    R/W  ACB_DESKEW_CLRB     |
 | bit  18:12 R/W  PDM_UPDATE_RATE     |
 | bit  11    R/W  PMM_ENABLE          |
 | bit  10:2  R/W  PMM_PKT_SIZE        |
 | bit  1:0   R/W  PMM_DSM_TYPE        |
 +------------------------------------*/
#define PMC_PCBI_REG_PMM_CONFIG_REG2_UNUSED_MASK         0xfff00000
#define PCBI_REG_PMM_CONFIG_REG2_BIT_ACB_DESKEW_CLRB_MSK 0x00080000
#define PCBI_REG_PMM_CONFIG_REG2_BIT_ACB_DESKEW_CLRB_OFF 19
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_MSK 0x0007f000
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PDM_UPDATE_RATE_OFF 12
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_ENABLE_MSK      0x00000800
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_ENABLE_OFF      11
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_MSK    0x000007fc
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_PKT_SIZE_OFF    2
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_MSK    0x00000003
#define PCBI_REG_PMM_CONFIG_REG2_BIT_PMM_DSM_TYPE_OFF    0

/*-------------------------------------.
 | Register 0x0000058c PMM_CONFIG_REG3 |
 +-------------------------------------+
 | bit  31    R/W  PDM_OVERRIDE_EN     |
 | bit  28:17 R/W  PDM_OVERRIDE_VAL    |
 | bit  16:6  R/W  PDM_ACCUM_CYCLES    |
 | bit  5:3   R/W  PDM_GAIN            |
 | bit  2:0   R/W  PDM_ATTEN           |
 +------------------------------------*/
#define PMC_PCBI_REG_PMM_CONFIG_REG3_UNUSED_MASK          0x60000000
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_EN_MSK  0x80000000
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_EN_OFF  31
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_MSK 0x1ffe0000
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_OVERRIDE_VAL_OFF 17
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_MSK 0x0001ffc0
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ACCUM_CYCLES_OFF 6
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_MSK         0x00000038
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_GAIN_OFF         3
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_MSK        0x00000007
#define PCBI_REG_PMM_CONFIG_REG3_BIT_PDM_ATTEN_OFF        0

/*-------------------------------------.
 | Register 0x00000590 PMM_STATUS_REG1 |
 +-------------------------------------+
 | bit  28:17 R  PDM_ACCUM_ATTEN       |
 | bit  11:0  R  PDM_ACCUM             |
 +------------------------------------*/
#define PMC_PCBI_REG_PMM_STATUS_REG1_UNUSED_MASK         0xe001f000
#define PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_MSK 0x1ffe0000
#define PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_ATTEN_OFF 17
#define PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_MSK       0x00000fff
#define PCBI_REG_PMM_STATUS_REG1_BIT_PDM_ACCUM_OFF       0

/*-------------------------------------.
 | Register 0x00000594 PMM_STATUS_REG2 |
 +-------------------------------------+
 | bit  28:17 R  PDM_ALIGN_OUT         |
 | bit  11:0  R  PDM_ACCUM_GAIN        |
 +------------------------------------*/
#define PMC_PCBI_REG_PMM_STATUS_REG2_UNUSED_MASK        0xe001f000
#define PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_MSK  0x1ffe0000
#define PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ALIGN_OUT_OFF  17
#define PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_MSK 0x00000fff
#define PCBI_REG_PMM_STATUS_REG2_BIT_PDM_ACCUM_GAIN_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _S16_CSU_PCBI_REGS_H */
