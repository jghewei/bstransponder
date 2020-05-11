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
 *     dcsu block
 *****************************************************************************/
#ifndef _DCSU_REGS_H
#define _DCSU_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_DCSU_40N_REG_SW_RESET_REG         0x000a0000
#define PMC_DCSU_40N_REG_DCO_CTRL_REG         0x000a0004
#define PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG    0x000a0008
#define PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG  0x000a000c
#define PMC_DCSU_40N_REG_POST_DIV_CTRL        0x000a0010
#define PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG 0x000a0014
#define PMC_DCSU_40N_REG_FB_CONFIG            0x000a0018
#define PMC_DCSU_40N_REG_FB_NUMERATOR         0x000a001c
#define PMC_DCSU_40N_REG_FB_DENOMINATOR       0x000a0020
#define PMC_DCSU_40N_REG_MODULATION_CFG       0x000a0024
#define PMC_DCSU_40N_REG_M_NUMERATOR          0x000a0028
#define PMC_DCSU_40N_REG_M_STEPSIZE           0x000a002c
#define PMC_DCSU_40N_REG_DCSU_INT_VAL_REG     0x000a0030
#define PMC_DCSU_40N_REG_DCSU_INTS            0x000a0034
#define PMC_DCSU_40N_REG_INT_EN               0x000a0038

/*----------------------------------.
 | Register 0x000a0000 SW_RESET_REG |
 +----------------------------------+
 | bit  16 R    BOOT_MODE           |
 | bit  0  R/W  SOFTWARE_RESET      |
 +---------------------------------*/
#define PMC_DCSU_40N_REG_SW_RESET_REG_UNUSED_MASK        0xfffefffe
#define DCSU_40N_REG_SW_RESET_REG_BIT_BOOT_MODE_MSK      0x00010000
#define DCSU_40N_REG_SW_RESET_REG_BIT_BOOT_MODE_OFF      16
#define DCSU_40N_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_MSK 0x00000001
#define DCSU_40N_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_OFF 0

/*----------------------------------.
 | Register 0x000a0004 DCO_CTRL_REG |
 +----------------------------------+
 | bit  2 R/W  DIV_RSTB             |
 | bit  1 R/W  ARSTB                |
 | bit  0 R/W  ENB                  |
 +---------------------------------*/
#define PMC_DCSU_40N_REG_DCO_CTRL_REG_UNUSED_MASK  0x0000f0e0
#define DCSU_40N_REG_DCO_CTRL_REG_BIT_DIV_RSTB_MSK 0x00000004
#define DCSU_40N_REG_DCO_CTRL_REG_BIT_DIV_RSTB_OFF 2
#define DCSU_40N_REG_DCO_CTRL_REG_BIT_ARSTB_MSK    0x00000002
#define DCSU_40N_REG_DCO_CTRL_REG_BIT_ARSTB_OFF    1
#define DCSU_40N_REG_DCO_CTRL_REG_BIT_ENB_MSK      0x00000001
#define DCSU_40N_REG_DCO_CTRL_REG_BIT_ENB_OFF      0

/*---------------------------------------.
 | Register 0x000a0008 DCSU_CLK_CTRL_REG |
 +---------------------------------------+
 | bit  7:6 R/W  DIVIDER_CLKSEL          |
 | bit  5:4 R/W  DCO_CTL_CLKSEL          |
 | bit  3   R/W  DIV4_ENB                |
 | bit  2   R/W  DIV3_ENB                |
 | bit  1   R/W  DIV2_ENB                |
 | bit  0   R/W  DIV1_ENB                |
 +--------------------------------------*/
#define PMC_DCSU_40N_REG_DCSU_CLK_CTRL_REG_UNUSED_MASK        0xffffff00
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_MSK 0x000000c0
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIVIDER_CLKSEL_OFF 6
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_MSK 0x00000030
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DCO_CTL_CLKSEL_OFF 4
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV4_ENB_MSK       0x00000008
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV4_ENB_OFF       3
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV3_ENB_MSK       0x00000004
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV3_ENB_OFF       2
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV2_ENB_MSK       0x00000002
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV2_ENB_OFF       1
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV1_ENB_MSK       0x00000001
#define DCSU_40N_REG_DCSU_CLK_CTRL_REG_BIT_DIV1_ENB_OFF       0

/*-----------------------------------------.
 | Register 0x000a000c PRE_DIVIDER_CFG_REG |
 +-----------------------------------------+
 | bit  4:0 R/W  PRE_DIV_BY                |
 +----------------------------------------*/
#define PMC_DCSU_40N_REG_PRE_DIVIDER_CFG_REG_UNUSED_MASK    0xffffffe0
#define DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_MSK 0x0000001f
#define DCSU_40N_REG_PRE_DIVIDER_CFG_REG_BIT_PRE_DIV_BY_OFF 0

/*-----------------------------------.
 | Register 0x000a0010 POST_DIV_CTRL |
 +-----------------------------------+
 | bit  31    R/W  POST_UPDATE_EN    |
 | bit  15:12 R/W  SHIFT_CLK_SEL     |
 | bit  11:8  R/W  AC_PULSE_REQ      |
 | bit  7:4   R/W  CLK_STOP          |
 | bit  3:0   R/W  CLK_BYPASS_EN     |
 +----------------------------------*/
#define PMC_DCSU_40N_REG_POST_DIV_CTRL_UNUSED_MASK        0x7fff0000
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_POST_UPDATE_EN_MSK 0x80000000
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_POST_UPDATE_EN_OFF 31
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_MSK  0x0000f000
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_SHIFT_CLK_SEL_OFF  12
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_MSK   0x00000f00
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_AC_PULSE_REQ_OFF   8
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_MSK       0x000000f0
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_STOP_OFF       4
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_MSK  0x0000000f
#define DCSU_40N_REG_POST_DIV_CTRL_BIT_CLK_BYPASS_EN_OFF  0

/*------------------------------------------.
 | Register 0x000a0014 POST_DIVIDER_CFG_REG |
 +------------------------------------------+
 | bit  31:28 R/W  POST_SYNC_EN             |
 | bit  24:20 R/W  POST_SYNC_CNT            |
 | bit  19:0  R/W  POST_DIV_BY              |
 +-----------------------------------------*/
#define PMC_DCSU_40N_REG_POST_DIVIDER_CFG_REG_UNUSED_MASK       0x0e000000
#define DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_MSK  0xf0000000
#define DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_EN_OFF  28
#define DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_MSK 0x01f00000
#define DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_SYNC_CNT_OFF 20
#define DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_MSK   0x000fffff
#define DCSU_40N_REG_POST_DIVIDER_CFG_REG_BIT_POST_DIV_BY_OFF   0

/*-------------------------------.
 | Register 0x000a0018 FB_CONFIG |
 +-------------------------------+
 | bit  31  R/W  FB_UPDATE_EN    |
 | bit  7:0 R/W  FB_INT_DIV      |
 +------------------------------*/
#define PMC_DCSU_40N_REG_FB_CONFIG_UNUSED_MASK      0x7fffff00
#define DCSU_40N_REG_FB_CONFIG_BIT_FB_UPDATE_EN_MSK 0x80000000
#define DCSU_40N_REG_FB_CONFIG_BIT_FB_UPDATE_EN_OFF 31
#define DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_MSK   0x000000ff
#define DCSU_40N_REG_FB_CONFIG_BIT_FB_INT_DIV_OFF   0

/*----------------------------------.
 | Register 0x000a001c FB_NUMERATOR |
 +----------------------------------+
 | bit  31:0 R/W  FB_NUMERATOR      |
 +---------------------------------*/
#define PMC_DCSU_40N_REG_FB_NUMERATOR_UNUSED_MASK      0x00000000
#define DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_MSK 0xffffffff
#define DCSU_40N_REG_FB_NUMERATOR_BIT_FB_NUMERATOR_OFF 0

/*------------------------------------.
 | Register 0x000a0020 FB_DENOMINATOR |
 +------------------------------------+
 | bit  31:0 R/W  FB_DENOMINATOR      |
 +-----------------------------------*/
#define PMC_DCSU_40N_REG_FB_DENOMINATOR_UNUSED_MASK        0x00000000
#define DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_MSK 0xffffffff
#define DCSU_40N_REG_FB_DENOMINATOR_BIT_FB_DENOMINATOR_OFF 0

/*------------------------------------.
 | Register 0x000a0024 MODULATION_CFG |
 +------------------------------------+
 | bit  31  R/W  M_EN                 |
 | bit  7:0 R/W  M_INT_DIV            |
 +-----------------------------------*/
#define PMC_DCSU_40N_REG_MODULATION_CFG_UNUSED_MASK   0x7fffff00
#define DCSU_40N_REG_MODULATION_CFG_BIT_M_EN_MSK      0x80000000
#define DCSU_40N_REG_MODULATION_CFG_BIT_M_EN_OFF      31
#define DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_MSK 0x000000ff
#define DCSU_40N_REG_MODULATION_CFG_BIT_M_INT_DIV_OFF 0

/*---------------------------------.
 | Register 0x000a0028 M_NUMERATOR |
 +---------------------------------+
 | bit  31:0 R/W  M_NUMERATOR      |
 +--------------------------------*/
#define PMC_DCSU_40N_REG_M_NUMERATOR_UNUSED_MASK     0x00000000
#define DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_MSK 0xffffffff
#define DCSU_40N_REG_M_NUMERATOR_BIT_M_NUMERATOR_OFF 0

/*--------------------------------.
 | Register 0x000a002c M_STEPSIZE |
 +--------------------------------+
 | bit  31:0 R/W  M_STEPSIZE      |
 +-------------------------------*/
#define PMC_DCSU_40N_REG_M_STEPSIZE_UNUSED_MASK    0x00000000
#define DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_MSK 0xffffffff
#define DCSU_40N_REG_M_STEPSIZE_BIT_M_STEPSIZE_OFF 0

/*--------------------------------------.
 | Register 0x000a0030 DCSU_INT_VAL_REG |
 +--------------------------------------+
 | bit  2 R  ERR_FAST_V                 |
 | bit  1 R  ERR_SLOW_V                 |
 | bit  0 R  LOCK_V                     |
 +-------------------------------------*/
#define PMC_DCSU_40N_REG_DCSU_INT_VAL_REG_UNUSED_MASK    0xffffffe0
#define DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_FAST_V_MSK 0x00000004
#define DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_FAST_V_OFF 2
#define DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_SLOW_V_MSK 0x00000002
#define DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_ERR_SLOW_V_OFF 1
#define DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_LOCK_V_MSK     0x00000001
#define DCSU_40N_REG_DCSU_INT_VAL_REG_BIT_LOCK_V_OFF     0

/*-------------------------------.
 | Register 0x000a0034 DCSU_INTS |
 +-------------------------------+
 | bit  2 R/W  ERR_FAST_I        |
 | bit  1 R/W  ERR_SLOW_I        |
 | bit  0 R/W  LOCK_I            |
 +------------------------------*/
#define PMC_DCSU_40N_REG_DCSU_INTS_UNUSED_MASK    0xffffffe0
#define DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_MSK 0x00000004
#define DCSU_40N_REG_DCSU_INTS_BIT_ERR_FAST_I_OFF 2
#define DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_MSK 0x00000002
#define DCSU_40N_REG_DCSU_INTS_BIT_ERR_SLOW_I_OFF 1
#define DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_MSK     0x00000001
#define DCSU_40N_REG_DCSU_INTS_BIT_LOCK_I_OFF     0

/*----------------------------.
 | Register 0x000a0038 INT_EN |
 +----------------------------+
 | bit  2 R/W  ERR_FAST_E     |
 | bit  1 R/W  ERR_SLOW_E     |
 | bit  0 R/W  LOCK_E         |
 +---------------------------*/
#define PMC_DCSU_40N_REG_INT_EN_UNUSED_MASK    0xffffffe0
#define DCSU_40N_REG_INT_EN_BIT_ERR_FAST_E_MSK 0x00000004
#define DCSU_40N_REG_INT_EN_BIT_ERR_FAST_E_OFF 2
#define DCSU_40N_REG_INT_EN_BIT_ERR_SLOW_E_MSK 0x00000002
#define DCSU_40N_REG_INT_EN_BIT_ERR_SLOW_E_OFF 1
#define DCSU_40N_REG_INT_EN_BIT_LOCK_E_MSK     0x00000001
#define DCSU_40N_REG_INT_EN_BIT_LOCK_E_OFF     0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DCSU_REGS_H */
