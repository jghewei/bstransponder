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
 *     enh_8g_csu block
 *****************************************************************************/
#ifndef _ENH_8G_CSU_REGS_H
#define _ENH_8G_CSU_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG           0x00000400
#define PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG             0x00000404
#define PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG            0x00000408
#define PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1                 0x00000410
#define PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG              0x00000414
#define PMC_ENH_8G_CSU_REG_PLL_CNTR_REG                  0x00000418
#define PMC_ENH_8G_CSU_REG_PLL_STA_REG                   0x0000041c
#define PMC_ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG  0x00000420
#define PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1            0x00000428
#define PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2            0x0000042c
#define PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG               0x00000430
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1           0x00000438
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2           0x00000440
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3           0x00000444
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG              0x00000448
#define PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG              0x00000454
#define PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG              0x0000045c
#define PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG             0x00000460
#define PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG              0x00000464
#define PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG       0x00000468
#define PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG        0x00000470
#define PMC_ENH_8G_CSU_REG_MABC_CNTL_REG                 0x00000474
#define PMC_ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG        0x00000478
#define PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1             0x00000480
#define PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2             0x00000484
#define PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3             0x00000488
#define PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG   0x0000048c
#define PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG    0x00000490
#define PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG         0x00000494
#define PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG    0x00000498
#define PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG 0x0000049c
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG           0x000004a0
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG       0x000004a4
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG       0x000004a8
#define PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1        0x000004ac
#define PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2        0x000004b0
#define PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1                0x000004b4

/*-----------------------------------------.
 | Register 0x00000400 CNI_UNI_CSU_RST_REG |
 +-----------------------------------------+
 | bit  7 R/W  ACB_RSTB                    |
 | bit  6 R/W  PLL_LOCK_DET_SW_RST         |
 | bit  5 R/W  CNI_UNI_CSU_GLB_RSTB        |
 | bit  4 R/W  GCOC_RSTB                   |
 | bit  3 R/W  EADSM_RSTB                  |
 | bit  2 R/W  OCC_TXRX_RSTB               |
 | bit  1 R/W  VCO_CAL_RSTB                |
 | bit  0 R/W  PLL_LOCK_DET_RSTB           |
 +----------------------------------------*/
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_UNUSED_MASK              0xffffff00
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_ACB_RSTB_MSK             0x00000080
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_ACB_RSTB_OFF             7
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_SW_RST_MSK  0x00000040
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_SW_RST_OFF  6
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_CNI_UNI_CSU_GLB_RSTB_MSK 0x00000020
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_CNI_UNI_CSU_GLB_RSTB_OFF 5
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_GCOC_RSTB_MSK            0x00000010
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_GCOC_RSTB_OFF            4
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_EADSM_RSTB_MSK           0x00000008
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_EADSM_RSTB_OFF           3
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_OCC_TXRX_RSTB_MSK        0x00000004
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_OCC_TXRX_RSTB_OFF        2
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_VCO_CAL_RSTB_MSK         0x00000002
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_VCO_CAL_RSTB_OFF         1
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_RSTB_MSK    0x00000001
#define ENH_8G_CSU_REG_CNI_UNI_CSU_RST_REG_BIT_PLL_LOCK_DET_RSTB_OFF    0

/*---------------------------------------.
 | Register 0x00000404 MABC_RST_CNFG_REG |
 +---------------------------------------+
 | bit  1 R/W  ARSTB                     |
 | bit  0 R/W  RST_SYNC                  |
 +--------------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_RST_CNFG_REG_UNUSED_MASK  0xfffffffc
#define ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_ARSTB_MSK    0x00000002
#define ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_ARSTB_OFF    1
#define ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_RST_SYNC_MSK 0x00000001
#define ENH_8G_CSU_REG_MABC_RST_CNFG_REG_BIT_RST_SYNC_OFF 0

/*----------------------------------------.
 | Register 0x00000408 CLK_DGLCH_CNFG_REG |
 +----------------------------------------+
 | bit  1 R/W  HRCLKDIV4OR5_EN            |
 | bit  0 R/W  HRCLKDIV4_EN               |
 +---------------------------------------*/
#define PMC_ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_UNUSED_MASK         0xfffffffc
#define ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4OR5_EN_MSK 0x00000002
#define ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4OR5_EN_OFF 1
#define ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4_EN_MSK    0x00000001
#define ENH_8G_CSU_REG_CLK_DGLCH_CNFG_REG_BIT_HRCLKDIV4_EN_OFF    0

/*-----------------------------------.
 | Register 0x00000410 PLL_CNFG_REG1 |
 +-----------------------------------+
 | bit  5   R/W  PPM_OFF_RD_REQ      |
 | bit  4   R/W  RESET_HISTORY       |
 | bit  3:0 R/W  LOCK_PRECISION      |
 +----------------------------------*/
#define PMC_ENH_8G_CSU_REG_PLL_CNFG_REG1_UNUSED_MASK        0xffffffc0
#define ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_MSK 0x00000020
#define ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_OFF 5
#define ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_MSK  0x00000010
#define ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_OFF  4
#define ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_MSK 0x0000000f
#define ENH_8G_CSU_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF 0

/*--------------------------------------.
 | Register 0x00000414 PLL_CLK_CNTR_REG |
 +--------------------------------------+
 | bit  27:20 R/W  LOCK_CNT_MAX         |
 | bit  19:0  R/W  CLK_CNT_MAX          |
 +-------------------------------------*/
#define PMC_ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_UNUSED_MASK      0xf0000000
#define ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_MSK 0x0ff00000
#define ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_LOCK_CNT_MAX_OFF 20
#define ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_MSK  0x000fffff
#define ENH_8G_CSU_REG_PLL_CLK_CNTR_REG_BIT_CLK_CNT_MAX_OFF  0

/*----------------------------------.
 | Register 0x00000418 PLL_CNTR_REG |
 +----------------------------------+
 | bit  31:16 R/W  FAIL_CNT_MAX     |
 | bit  15:0  R/W  PASS_CNT_MAX     |
 +---------------------------------*/
#define PMC_ENH_8G_CSU_REG_PLL_CNTR_REG_UNUSED_MASK      0x00000000
#define ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_MSK 0xffff0000
#define ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_FAIL_CNT_MAX_OFF 16
#define ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_MSK 0x0000ffff
#define ENH_8G_CSU_REG_PLL_CNTR_REG_BIT_PASS_CNT_MAX_OFF 0

/*---------------------------------.
 | Register 0x0000041c PLL_STA_REG |
 +---------------------------------+
 | bit  1 R  LOCKB                 |
 +--------------------------------*/
#define PMC_ENH_8G_CSU_REG_PLL_STA_REG_UNUSED_MASK 0xfffffffd
#define ENH_8G_CSU_REG_PLL_STA_REG_BIT_LOCKB_MSK   0x00000002
#define ENH_8G_CSU_REG_PLL_STA_REG_BIT_LOCKB_OFF   1

/*--------------------------------------------------.
 | Register 0x00000420 PLL_LOCK_DET_PPM_OFF_STA_REG |
 +--------------------------------------------------+
 | bit  31   R  PPM_OFF_RD_SIGN                     |
 | bit  20:0 R  PPM_OFF_RD_VAL                      |
 +-------------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_UNUSED_MASK         0x7fe00000
#define ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_SIGN_MSK 0x80000000
#define ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_SIGN_OFF 31
#define ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_MSK  0x001fffff
#define ENH_8G_CSU_REG_PLL_LOCK_DET_PPM_OFF_STA_REG_BIT_PPM_OFF_RD_VAL_OFF  0

/*-----------------------------------------.
 | Register 0x00000428 VCO_CAL_CNFG_REG_1  |
 +-----------------------------------------+
 | bit  28:23 R/W  VCOC_MAX_THRESHOLD      |
 | bit  22:17 R/W  VCOC_MIN_THRESHOLD      |
 | bit  16    R/W  PPM_OFFSET_POLARITY_INV |
 | bit  15    R/W  LOAD_COUNT_OVR_EN       |
 | bit  14    R/W  LOAD_COUNT_OVR          |
 | bit  13:8  R/W  MIN_MODE                |
 | bit  7:2   R/W  MAX_MODE                |
 | bit  1     R/W  VCOC_START              |
 | bit  0     R/W  VCOC_EN                 |
 +----------------------------------------*/
#define PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_UNUSED_MASK                 0xe0000000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_MSK      0x1f800000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MAX_THRESHOLD_OFF      23
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_MSK      0x007e0000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_MIN_THRESHOLD_OFF      17
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_PPM_OFFSET_POLARITY_INV_MSK 0x00010000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_PPM_OFFSET_POLARITY_INV_OFF 16
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_EN_MSK       0x00008000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_EN_OFF       15
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_MSK          0x00004000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_LOAD_COUNT_OVR_OFF          14
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_MSK                0x00003f00
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MIN_MODE_OFF                8
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_MSK                0x000000fc
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_MAX_MODE_OFF                2
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_START_MSK              0x00000002
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_START_OFF              1
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_EN_MSK                 0x00000001
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_1_BIT_VCOC_EN_OFF                 0

/*----------------------------------------.
 | Register 0x0000042c VCO_CAL_CNFG_REG_2 |
 +----------------------------------------+
 | bit  23:16 R/W  VCO_SETTLING_TIME      |
 | bit  11:8  R/W  VCOC_LOOP              |
 | bit  6:1   R/W  OFF_EQ_WINDOW          |
 | bit  0     R/W  VCOC_BIST_EN           |
 +---------------------------------------*/
#define PMC_ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_UNUSED_MASK           0xff00f080
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_MSK 0x00ff0000
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCO_SETTLING_TIME_OFF 16
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_MSK         0x00000f00
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_LOOP_OFF         8
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_MSK     0x0000007e
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_OFF_EQ_WINDOW_OFF     1
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_BIST_EN_MSK      0x00000001
#define ENH_8G_CSU_REG_VCO_CAL_CNFG_REG_2_BIT_VCOC_BIST_EN_OFF      0

/*-------------------------------------.
 | Register 0x00000430 VCO_CAL_STA_REG |
 +-------------------------------------+
 | bit  16   R  VCOC_MODE_IN_RANGE_S   |
 | bit  15:8 R  VCOC_BIST_MODE_COUNT   |
 | bit  5:0  R  VCOC_MODE_OUT          |
 +------------------------------------*/
#define PMC_ENH_8G_CSU_REG_VCO_CAL_STA_REG_UNUSED_MASK              0xfffe00c0
#define ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_IN_RANGE_S_MSK 0x00010000
#define ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_IN_RANGE_S_OFF 16
#define ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_MSK 0x0000ff00
#define ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_BIST_MODE_COUNT_OFF 8
#define ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_MSK        0x0000003f
#define ENH_8G_CSU_REG_VCO_CAL_STA_REG_BIT_VCOC_MODE_OUT_OFF        0

/*-----------------------------------------.
 | Register 0x00000438 OCC_TXRX_CNFG_REG_1 |
 +-----------------------------------------+
 | bit  21    R/W  OCC_START               |
 | bit  20    R/W  OCC_CONT                |
 | bit  19    R/W  CLR_OCDAC               |
 | bit  18    R/W  OCC_ENABLE              |
 | bit  17    R/W  OC_OVERRIDE_ENB         |
 | bit  16    R/W  OC_INVERT               |
 | bit  15:14 R/W  OCC_DONE_GEN_SEL        |
 +----------------------------------------*/
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_UNUSED_MASK          0xffc03fff
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_START_MSK        0x00200000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_START_OFF        21
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_CONT_MSK         0x00100000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_CONT_OFF         20
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_CLR_OCDAC_MSK        0x00080000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_CLR_OCDAC_OFF        19
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_ENABLE_MSK       0x00040000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_ENABLE_OFF       18
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_OVERRIDE_ENB_MSK  0x00020000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_OVERRIDE_ENB_OFF  17
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_INVERT_MSK        0x00010000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OC_INVERT_OFF        16
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_MSK 0x0000c000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_1_BIT_OCC_DONE_GEN_SEL_OFF 14

/*-----------------------------------------.
 | Register 0x00000440 OCC_TXRX_CNFG_REG_2 |
 +-----------------------------------------+
 | bit  24:20 R/W  OC_OVERRIDE             |
 | bit  14:0  R/W  OC_TOLERANCE            |
 +----------------------------------------*/
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_UNUSED_MASK      0xfe0f8000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_MSK  0x01f00000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_OVERRIDE_OFF  20
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_MSK 0x00007fff
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_2_BIT_OC_TOLERANCE_OFF 0

/*-----------------------------------------.
 | Register 0x00000444 OCC_TXRX_CNFG_REG_3 |
 +-----------------------------------------+
 | bit  19:16 R/W  OC_THRESHOLD            |
 | bit  15:0  R/W  OC_INTERVAL             |
 +----------------------------------------*/
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_UNUSED_MASK      0xfff00000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_MSK 0x000f0000
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_THRESHOLD_OFF 16
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_MSK  0x0000ffff
#define ENH_8G_CSU_REG_OCC_TXRX_CNFG_REG_3_BIT_OC_INTERVAL_OFF  0

/*--------------------------------------.
 | Register 0x00000448 OCC_TXRX_STA_REG |
 +--------------------------------------+
 | bit  6   R  OC_THRESHOLD_CHECK_FAILB |
 | bit  5:1 R  OCDAC_UNENC              |
 | bit  0   R  OC_SENSE                 |
 +-------------------------------------*/
#define PMC_ENH_8G_CSU_REG_OCC_TXRX_STA_REG_UNUSED_MASK                  0xffffff80
#define ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_THRESHOLD_CHECK_FAILB_MSK 0x00000040
#define ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_THRESHOLD_CHECK_FAILB_OFF 6
#define ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_MSK              0x0000003e
#define ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OCDAC_UNENC_OFF              1
#define ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_SENSE_MSK                 0x00000001
#define ENH_8G_CSU_REG_OCC_TXRX_STA_REG_BIT_OC_SENSE_OFF                 0

/*--------------------------------------.
 | Register 0x00000454 CLK_DIV_CNFG_REG |
 +--------------------------------------+
 | bit  3:0 R/W  DIV_CLK_FACT           |
 +-------------------------------------*/
#define PMC_ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_UNUSED_MASK      0xfffffff0
#define ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_MSK 0x0000000f
#define ENH_8G_CSU_REG_CLK_DIV_CNFG_REG_BIT_DIV_CLK_FACT_OFF 0

/*--------------------------------------.
 | Register 0x0000045c MABC_TC_CNFG_REG |
 +--------------------------------------+
 | bit  16   R/W  TERM_OVR              |
 | bit  11:8 R/W  TERMB0_ENB_DIG        |
 | bit  3:0  R/W  TERMA0_ENB_DIG        |
 +-------------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_TC_CNFG_REG_UNUSED_MASK        0xfffef0f0
#define ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERM_OVR_MSK       0x00010000
#define ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERM_OVR_OFF       16
#define ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_MSK 0x00000f00
#define ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMB0_ENB_DIG_OFF 8
#define ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_MSK 0x0000000f
#define ENH_8G_CSU_REG_MABC_TC_CNFG_REG_BIT_TERMA0_ENB_DIG_OFF 0

/*---------------------------------------.
 | Register 0x00000460 MABC_TEST_CON_REG |
 +---------------------------------------+
 | bit  6   R/W  CSU_ATMSB               |
 | bit  5   R/W  CS_ATMSB                |
 | bit  4:0 R/W  ATIN                    |
 +--------------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_TEST_CON_REG_UNUSED_MASK   0xffffff80
#define ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CSU_ATMSB_MSK 0x00000040
#define ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CSU_ATMSB_OFF 6
#define ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CS_ATMSB_MSK  0x00000020
#define ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_CS_ATMSB_OFF  5
#define ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_MSK      0x0000001f
#define ENH_8G_CSU_REG_MABC_TEST_CON_REG_BIT_ATIN_OFF      0

/*--------------------------------------.
 | Register 0x00000464 MABC_ENB_CON_REG |
 +--------------------------------------+
 | bit  0 R/W  ENB                      |
 +-------------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_ENB_CON_REG_UNUSED_MASK 0xfffffffe
#define ENH_8G_CSU_REG_MABC_ENB_CON_REG_BIT_ENB_MSK     0x00000001
#define ENH_8G_CSU_REG_MABC_ENB_CON_REG_BIT_ENB_OFF     0

/*---------------------------------------------.
 | Register 0x00000468 MABC_CS_MODE_CONFIG_REG |
 +---------------------------------------------+
 | bit  21:20 R/W  CS_DIVMODE                  |
 | bit  17:13 R/W  CS_CTRL_7_3                 |
 | bit  12    R/W  CS_CTRL_2                   |
 | bit  11    R/W  CS_CTRL_1                   |
 | bit  10    R/W  CS_CTRL_0                   |
 | bit  7:6   R/W  CS_MODE_7_6                 |
 | bit  5     R/W  CS_MODE_5                   |
 | bit  4     R/W  CS_MODE_4                   |
 | bit  3     R/W  CS_MODE_3                   |
 | bit  2     R/W  CS_MODE_2                   |
 | bit  1     R/W  CS_MODE_1                   |
 | bit  0     R/W  CS_MODE_0                   |
 +--------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_UNUSED_MASK     0xffcc0300
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_MSK  0x00300000
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_DIVMODE_OFF  20
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_MSK 0x0003e000
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_7_3_OFF 13
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_2_MSK   0x00001000
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_2_OFF   12
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_1_MSK   0x00000800
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_1_OFF   11
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_0_MSK   0x00000400
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_CTRL_0_OFF   10
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_MSK 0x000000c0
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_7_6_OFF 6
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_5_MSK   0x00000020
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_5_OFF   5
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_4_MSK   0x00000010
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_4_OFF   4
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_3_MSK   0x00000008
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_3_OFF   3
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_2_MSK   0x00000004
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_2_OFF   2
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_1_MSK   0x00000002
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_1_OFF   1
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_0_MSK   0x00000001
#define ENH_8G_CSU_REG_MABC_CS_MODE_CONFIG_REG_BIT_CS_MODE_0_OFF   0

/*--------------------------------------------.
 | Register 0x00000470 MABC_CSU_MODE_CNFG_REG |
 +--------------------------------------------+
 | bit  31:28 R/W  CSU_MODE_31_28             |
 | bit  27    R/W  CSU_MODE_27                |
 | bit  26:25 R/W  CSU_MODE_26_25             |
 | bit  24:22 R/W  CSU_MODE_24_22             |
 | bit  21    R/W  CSU_MODE_21                |
 | bit  20    R/W  CSU_MODE_20                |
 | bit  19    R/W  CSU_MODE_19                |
 | bit  18:16 R/W  CSU_MODE_18_16             |
 | bit  15:10 R/W  CSU_MODE_15_10             |
 | bit  9:8   R/W  CSU_MODE_9_8               |
 | bit  7:6   R/W  CSU_MODE_7_6               |
 | bit  5:3   R/W  CSU_MODE_5_3               |
 | bit  2:0   R/W  CSU_MODE_2_0               |
 +-------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_UNUSED_MASK        0x00000000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_MSK 0xf0000000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_31_28_OFF 28
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_27_MSK    0x08000000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_27_OFF    27
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_MSK 0x06000000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_26_25_OFF 25
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_MSK 0x01c00000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_24_22_OFF 22
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_21_MSK    0x00200000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_21_OFF    21
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_20_MSK    0x00100000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_20_OFF    20
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_19_MSK    0x00080000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_19_OFF    19
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_MSK 0x00070000
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_18_16_OFF 16
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_MSK 0x0000fc00
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_15_10_OFF 10
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_MSK   0x00000300
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_9_8_OFF   8
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_MSK   0x000000c0
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_7_6_OFF   6
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_MSK   0x00000038
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_5_3_OFF   3
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_MSK   0x00000007
#define ENH_8G_CSU_REG_MABC_CSU_MODE_CNFG_REG_BIT_CSU_MODE_2_0_OFF   0

/*-----------------------------------.
 | Register 0x00000474 MABC_CNTL_REG |
 +-----------------------------------+
 | bit  19:17 R/W  CSU_CTRL_19_17    |
 | bit  16    R/W  CSU_CTRL_16       |
 | bit  15    R/W  CSU_CTRL_15       |
 | bit  14:13 R/W  CSU_CTRL_14_13    |
 | bit  12    R/W  CSU_CTRL_12       |
 | bit  11    R/W  CSU_CTRL_11       |
 | bit  10    R/W  CSU_CTRL_10       |
 | bit  9:6   R/W  CSU_CTRL_9_6      |
 | bit  5     R/W  CSU_CTRL_5        |
 | bit  4     R/W  CSU_CTRL_4        |
 | bit  3     R/W  CSU_CTRL_3        |
 | bit  2     R/W  CSU_CTRL_2        |
 | bit  1:0   R/W  CSU_CTRL_1_0      |
 +----------------------------------*/
#define PMC_ENH_8G_CSU_REG_MABC_CNTL_REG_UNUSED_MASK        0xfff00000
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_MSK 0x000e0000
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_19_17_OFF 17
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_16_MSK    0x00010000
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_16_OFF    16
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_15_MSK    0x00008000
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_15_OFF    15
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_MSK 0x00006000
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_14_13_OFF 13
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_12_MSK    0x00001000
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_12_OFF    12
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_11_MSK    0x00000800
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_11_OFF    11
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_10_MSK    0x00000400
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_10_OFF    10
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_MSK   0x000003c0
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_9_6_OFF   6
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_5_MSK     0x00000020
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_5_OFF     5
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_4_MSK     0x00000010
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_4_OFF     4
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_3_MSK     0x00000008
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_3_OFF     3
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_2_MSK     0x00000004
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_2_OFF     2
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_MSK   0x00000003
#define ENH_8G_CSU_REG_MABC_CNTL_REG_BIT_CSU_CTRL_1_0_OFF   0

/*--------------------------------------------.
 | Register 0x00000478 REF_DBLR_CALIB_STA_REG |
 +--------------------------------------------+
 | bit  6   R  REF_DCD_VAL                    |
 | bit  5:0 R  DBL_CTRL_RD                    |
 +-------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_UNUSED_MASK     0xffffff80
#define ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_REF_DCD_VAL_MSK 0x00000040
#define ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_REF_DCD_VAL_OFF 6
#define ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_MSK 0x0000003f
#define ENH_8G_CSU_REG_REF_DBLR_CALIB_STA_REG_BIT_DBL_CTRL_RD_OFF 0

/*---------------------------------------.
 | Register 0x00000480 E_ADSM_CNFG_REG_1 |
 +---------------------------------------+
 | bit  3 R/W  CLKO_POLARITY             |
 | bit  2 R/W  DSM_RSTB                  |
 | bit  1 R/W  MULT2                     |
 | bit  0 R/W  SEED                      |
 +--------------------------------------*/
#define PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_UNUSED_MASK       0xfffffff0
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_CLKO_POLARITY_MSK 0x00000008
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_CLKO_POLARITY_OFF 3
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_DSM_RSTB_MSK      0x00000004
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_DSM_RSTB_OFF      2
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_MULT2_MSK         0x00000002
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_MULT2_OFF         1
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_SEED_MSK          0x00000001
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_1_BIT_SEED_OFF          0

/*---------------------------------------.
 | Register 0x00000484 E_ADSM_CNFG_REG_2 |
 +---------------------------------------+
 | bit  31:0 R/W  FRAC_DIV_LOWER         |
 +--------------------------------------*/
#define PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_UNUSED_MASK        0x00000000
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_MSK 0xffffffff
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_2_BIT_FRAC_DIV_LOWER_OFF 0

/*---------------------------------------.
 | Register 0x00000488 E_ADSM_CNFG_REG_3 |
 +---------------------------------------+
 | bit  7:0 R/W  FRAC_DIV_UPPER          |
 +--------------------------------------*/
#define PMC_ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_UNUSED_MASK        0xffffff00
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_MSK 0x000000ff
#define ENH_8G_CSU_REG_E_ADSM_CNFG_REG_3_BIT_FRAC_DIV_UPPER_OFF 0

/*-------------------------------------------------.
 | Register 0x0000048c E_ADSM_DENOMIN_LOW_CNFG_REG |
 +-------------------------------------------------+
 | bit  31:0 R/W  DENOMIN_LOWER                    |
 +------------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_UNUSED_MASK       0x00000000
#define ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_MSK 0xffffffff
#define ENH_8G_CSU_REG_E_ADSM_DENOMIN_LOW_CNFG_REG_BIT_DENOMIN_LOWER_OFF 0

/*------------------------------------------------.
 | Register 0x00000490 E_ADSM_DENOMIN_UP_CNFG_REG |
 +------------------------------------------------+
 | bit  7:0 R/W  DENOMIN_UPPER                    |
 +-----------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_UNUSED_MASK       0xffffff00
#define ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_MSK 0x000000ff
#define ENH_8G_CSU_REG_E_ADSM_DENOMIN_UP_CNFG_REG_BIT_DENOMIN_UPPER_OFF 0

/*-------------------------------------------.
 | Register 0x00000494 E_ADSM_ID_DS_CNFG_REG |
 +-------------------------------------------+
 | bit  11:10 R/W  DITHER_SEL                |
 | bit  9:0   R/W  INT_DIV                   |
 +------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_UNUSED_MASK    0xfffff000
#define ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_MSK 0x00000c00
#define ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_DITHER_SEL_OFF 10
#define ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_MSK    0x000003ff
#define ENH_8G_CSU_REG_E_ADSM_ID_DS_CNFG_REG_BIT_INT_DIV_OFF    0

/*------------------------------------------------.
 | Register 0x00000498 EADSM_FRAC_DIV_MOD_OVR_REG |
 +------------------------------------------------+
 | bit  29   R/W  FRAC_DIV_MOD_OVR_ENB            |
 | bit  24:0 R/W  FRAC_DIV_MOD_LOW_OVR_VAL        |
 +-----------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_UNUSED_MASK                  0xde000000
#define ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_OVR_ENB_MSK     0x20000000
#define ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_OVR_ENB_OFF     29
#define ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_MSK 0x01ffffff
#define ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_OVR_REG_BIT_FRAC_DIV_MOD_LOW_OVR_VAL_OFF 0

/*---------------------------------------------------.
 | Register 0x0000049c EADSM_FRAC_DIV_MOD_UP_OVR_REG |
 +---------------------------------------------------+
 | bit  15:0 R/W  FRAC_DIV_MOD_UP_OVR_VAL            |
 +--------------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_UNUSED_MASK                 0xffff0000
#define ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_MSK 0x0000ffff
#define ENH_8G_CSU_REG_EADSM_FRAC_DIV_MOD_UP_OVR_REG_BIT_FRAC_DIV_MOD_UP_OVR_VAL_OFF 0

/*-------------------------------------------.
 | Register 0x000004a0 CNI_UNI_CSU_INT_REG   |
 +-------------------------------------------+
 | bit  12 R/W  OC_THRESHOLD_CHECK_FAILB_I12 |
 | bit  11 R/W  VCOC_MODE_IN_RANGE_I11       |
 | bit  10 R/W  VCOC_BIST_ERR_I10            |
 | bit  8  R/W  PPM_OFFSET_UPD_I8            |
 | bit  7  R/W  DIVCLK_DEAD_I7               |
 | bit  6  R/W  CALIB_DONE_I6                |
 | bit  5  R/W  OCC_DONE_I5                  |
 | bit  4  R/W  LOCK_LOSS_I4                 |
 | bit  3  R/W  LOCK_GAIN_I3                 |
 | bit  2  R/W  VCOC_DONE_I2                 |
 | bit  1  R/W  IN_RANGE_N_FAILS_I1          |
 | bit  0  R/W  IN_RANGE_N_PASSES_I0         |
 +------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_UNUSED_MASK                      0xffffe200
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_MSK 0x00001000
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OC_THRESHOLD_CHECK_FAILB_I12_OFF 12
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_MSK       0x00000800
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_MODE_IN_RANGE_I11_OFF       11
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_MSK            0x00000400
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_BIST_ERR_I10_OFF            10
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_MSK            0x00000100
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_PPM_OFFSET_UPD_I8_OFF            8
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_MSK               0x00000080
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_DIVCLK_DEAD_I7_OFF               7
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_MSK                0x00000040
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_CALIB_DONE_I6_OFF                6
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_MSK                  0x00000020
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_OCC_DONE_I5_OFF                  5
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_MSK                 0x00000010
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_LOSS_I4_OFF                 4
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_MSK                 0x00000008
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_LOCK_GAIN_I3_OFF                 3
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_MSK                 0x00000004
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_VCOC_DONE_I2_OFF                 2
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_MSK          0x00000002
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_FAILS_I1_OFF          1
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_MSK         0x00000001
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_REG_BIT_IN_RANGE_N_PASSES_I0_OFF         0

/*---------------------------------------------.
 | Register 0x000004a4 CNI_UNI_CSU_INT_ENB_REG |
 +---------------------------------------------+
 | bit  12 R/W  OC_THRESHOLD_CHECK_FAILB_E12   |
 | bit  11 R/W  VCOC_MODE_IN_RANGE_E11         |
 | bit  10 R/W  VCOC_BIST_ERR_E10              |
 | bit  8  R/W  PPM_OFFSET_UPD_E8              |
 | bit  7  R/W  DIVCLK_DEAD_E7                 |
 | bit  6  R/W  CALIB_DONE_E6                  |
 | bit  5  R/W  OCC_DONE_E5                    |
 | bit  4  R/W  LOCK_LOSS_E4                   |
 | bit  3  R/W  LOCK_GAIN_E3                   |
 | bit  2  R/W  VCOC_DONE_E2                   |
 | bit  1  R/W  IN_RANGE_N_FAILS_E1            |
 | bit  0  R/W  IN_RANGE_N_PASSES_E0           |
 +--------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_UNUSED_MASK                      0xffffe200
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OC_THRESHOLD_CHECK_FAILB_E12_MSK 0x00001000
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OC_THRESHOLD_CHECK_FAILB_E12_OFF 12
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_MODE_IN_RANGE_E11_MSK       0x00000800
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_MODE_IN_RANGE_E11_OFF       11
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_BIST_ERR_E10_MSK            0x00000400
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_BIST_ERR_E10_OFF            10
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_PPM_OFFSET_UPD_E8_MSK            0x00000100
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_PPM_OFFSET_UPD_E8_OFF            8
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_DIVCLK_DEAD_E7_MSK               0x00000080
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_DIVCLK_DEAD_E7_OFF               7
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_CALIB_DONE_E6_MSK                0x00000040
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_CALIB_DONE_E6_OFF                6
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OCC_DONE_E5_MSK                  0x00000020
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_OCC_DONE_E5_OFF                  5
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_LOSS_E4_MSK                 0x00000010
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_LOSS_E4_OFF                 4
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_GAIN_E3_MSK                 0x00000008
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_LOCK_GAIN_E3_OFF                 3
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_DONE_E2_MSK                 0x00000004
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_VCOC_DONE_E2_OFF                 2
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_FAILS_E1_MSK          0x00000002
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_FAILS_E1_OFF          1
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_PASSES_E0_MSK         0x00000001
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_ENB_REG_BIT_IN_RANGE_N_PASSES_E0_OFF         0

/*---------------------------------------------.
 | Register 0x000004a8 CNI_UNI_CSU_INT_VAL_REG |
 +---------------------------------------------+
 | bit  12 R  OC_THRESHOLD_CHECK_FAILB_V12     |
 | bit  11 R  VCOC_MODE_IN_RANGE_V11           |
 | bit  10 R  VCOC_BIST_ERR_V10                |
 | bit  8  R  PPM_OFFSET_UPD_V8                |
 | bit  7  R  DIVCLK_DEAD_V7                   |
 | bit  6  R  CALIB_DONE_V6                    |
 | bit  5  R  OCC_DONE_V5                      |
 | bit  4  R  LOCK_LOSS_V4                     |
 | bit  3  R  LOCK_GAIN_V3                     |
 | bit  2  R  VCOC_DONE_V2                     |
 | bit  1  R  IN_RANGE_N_FAILS_V1              |
 | bit  0  R  IN_RANGE_N_PASSES_V0             |
 +--------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_UNUSED_MASK                      0xffffe200
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OC_THRESHOLD_CHECK_FAILB_V12_MSK 0x00001000
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OC_THRESHOLD_CHECK_FAILB_V12_OFF 12
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_MODE_IN_RANGE_V11_MSK       0x00000800
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_MODE_IN_RANGE_V11_OFF       11
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_BIST_ERR_V10_MSK            0x00000400
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_BIST_ERR_V10_OFF            10
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_PPM_OFFSET_UPD_V8_MSK            0x00000100
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_PPM_OFFSET_UPD_V8_OFF            8
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_DIVCLK_DEAD_V7_MSK               0x00000080
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_DIVCLK_DEAD_V7_OFF               7
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_CALIB_DONE_V6_MSK                0x00000040
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_CALIB_DONE_V6_OFF                6
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OCC_DONE_V5_MSK                  0x00000020
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_OCC_DONE_V5_OFF                  5
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_LOSS_V4_MSK                 0x00000010
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_LOSS_V4_OFF                 4
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_GAIN_V3_MSK                 0x00000008
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_LOCK_GAIN_V3_OFF                 3
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_DONE_V2_MSK                 0x00000004
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_VCOC_DONE_V2_OFF                 2
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_FAILS_V1_MSK          0x00000002
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_FAILS_V1_OFF          1
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_PASSES_V0_MSK         0x00000001
#define ENH_8G_CSU_REG_CNI_UNI_CSU_INT_VAL_REG_BIT_IN_RANGE_N_PASSES_V0_OFF         0

/*--------------------------------------------.
 | Register 0x000004ac REFCLK_DBLR_CNFG_REG_1 |
 +--------------------------------------------+
 | bit  29:18 R/W  DAC_DELAY                  |
 | bit  17:10 R/W  ACC_DELAY                  |
 | bit  9:2   R/W  N_ACCUM_CYCLES             |
 | bit  1     R/W  CALIB_PROG_EN              |
 | bit  0     R/W  CALIB                      |
 +-------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_UNUSED_MASK        0xc0000000
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_MSK      0x3ffc0000
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_DAC_DELAY_OFF      18
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_MSK      0x0003fc00
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_ACC_DELAY_OFF      10
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_MSK 0x000003fc
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_N_ACCUM_CYCLES_OFF 2
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_PROG_EN_MSK  0x00000002
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_PROG_EN_OFF  1
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_MSK          0x00000001
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_1_BIT_CALIB_OFF          0

/*--------------------------------------------.
 | Register 0x000004b0 REFCLK_DBLR_CNFG_REG_2 |
 +--------------------------------------------+
 | bit  8   R/W  REF_DCD_INV                  |
 | bit  7   R/W  DBL_CTRL_BIT5_INV            |
 | bit  6:1 R/W  DBL_CTRL_OVR                 |
 | bit  0   R/W  DBL_CTRL_OVR_EN              |
 +-------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_UNUSED_MASK           0xfffffe00
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_REF_DCD_INV_MSK       0x00000100
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_REF_DCD_INV_OFF       8
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_BIT5_INV_MSK 0x00000080
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_BIT5_INV_OFF 7
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_MSK      0x0000007e
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_OFF      1
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_EN_MSK   0x00000001
#define ENH_8G_CSU_REG_REFCLK_DBLR_CNFG_REG_2_BIT_DBL_CTRL_OVR_EN_OFF   0

/*--------------------------------------------.
 | Register 0x000004b4 ACB_CNFG_REG_1         |
 +--------------------------------------------+
 | bit  23    R/W  PLL_S_OUT_FRAC_DIV_MOD_SEL |
 | bit  22:19 R/W  ACB_CLK_FACT               |
 +-------------------------------------------*/
#define PMC_ENH_8G_CSU_REG_ACB_CNFG_REG_1_UNUSED_MASK                    0xff07ffff
#define ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_PLL_S_OUT_FRAC_DIV_MOD_SEL_MSK 0x00800000
#define ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_PLL_S_OUT_FRAC_DIV_MOD_SEL_OFF 23
#define ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_MSK               0x00780000
#define ENH_8G_CSU_REG_ACB_CNFG_REG_1_BIT_ACB_CLK_FACT_OFF               19

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ENH_8G_CSU_REGS_H */
