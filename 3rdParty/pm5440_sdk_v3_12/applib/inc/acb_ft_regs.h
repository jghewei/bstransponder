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
 *     acb_ft block
 *****************************************************************************/
#ifndef _ACB_FT_REGS_H
#define _ACB_FT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ACB_FT_REG_SW_RESET_REG                      0x00000000
#define PMC_ACB_FT_REG_ACB_FT_EN_REG                     0x00000004
#define PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG               0x00000008
#define PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1                 0x0000000c
#define PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2                 0x00000010
#define PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG     0x00000014
#define PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG   0x00000018
#define PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG   0x0000001c
#define PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG 0x00000020
#define PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG 0x00000024
#define PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG        0x00000028
#define PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1      0x0000002c
#define PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2      0x00000030
#define PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3      0x00000034
#define PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1     0x00000038
#define PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2     0x0000003c
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG         0x00000044
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1      0x00000048
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2      0x0000004c
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3      0x00000050
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4      0x00000054
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5      0x00000058
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6      0x0000005c
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7      0x00000060
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8      0x00000064
#define PMC_ACB_FT_REG_ACB_FT_INT_REG                    0x00000068
#define PMC_ACB_FT_REG_ACB_FT_INT_EN_REG                 0x0000006c
#define PMC_ACB_FT_REG_ACB_FT_INT_STAT_REG               0x00000070

/*----------------------------------.
 | Register 0x00000000 SW_RESET_REG |
 +----------------------------------+
 | bit  0 R/W  SOFTWARE_RESET       |
 +---------------------------------*/
#define PMC_ACB_FT_REG_SW_RESET_REG_UNUSED_MASK        0xfffffffe
#define ACB_FT_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_MSK 0x00000001
#define ACB_FT_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_OFF 0

/*-----------------------------------.
 | Register 0x00000004 ACB_FT_EN_REG |
 +-----------------------------------+
 | bit  7 R/W  FIFO_CENT_ACCEL       |
 | bit  6 R/W  B_CKCTL_MODE          |
 | bit  5 R/W  C_CKCTL_MODE          |
 | bit  4 R/W  FIFO_CENT_TRIGGER     |
 | bit  3 R/W  FIFO_CENT_ENABLE      |
 | bit  2 R/W  B_INTERFACE_ENABLE    |
 | bit  1 R/W  FIFO_ADAPT_ENABLE     |
 | bit  0 R/W  PMM_ADAPT_ENABLE      |
 +----------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_EN_REG_UNUSED_MASK            0xffffff00
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ACCEL_MSK    0x00000080
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ACCEL_OFF    7
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_B_CKCTL_MODE_MSK       0x00000040
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_B_CKCTL_MODE_OFF       6
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_C_CKCTL_MODE_MSK       0x00000020
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_C_CKCTL_MODE_OFF       5
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_TRIGGER_MSK  0x00000010
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_TRIGGER_OFF  4
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ENABLE_MSK   0x00000008
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ENABLE_OFF   3
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_B_INTERFACE_ENABLE_MSK 0x00000004
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_B_INTERFACE_ENABLE_OFF 2
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_ADAPT_ENABLE_MSK  0x00000002
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_ADAPT_ENABLE_OFF  1
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_PMM_ADAPT_ENABLE_MSK   0x00000001
#define ACB_FT_REG_ACB_FT_EN_REG_BIT_PMM_ADAPT_ENABLE_OFF   0

/*-----------------------------------------.
 | Register 0x00000008 ACB_FT_HOLDOVER_REG |
 +-----------------------------------------+
 | bit  1 R/W  HOLDOVER                    |
 | bit  0 R/W  FORCE_NOMINAL               |
 +----------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG_UNUSED_MASK       0xfffffffc
#define ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_HOLDOVER_MSK      0x00000002
#define ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_HOLDOVER_OFF      1
#define ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_FORCE_NOMINAL_MSK 0x00000001
#define ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_FORCE_NOMINAL_OFF 0

/*---------------------------------------.
 | Register 0x0000000c ACB_FT_GAIN_REG_1 |
 +---------------------------------------+
 | bit  31:28 R/W  PLL_S_MIN             |
 | bit  27:24 R/W  DWELL_MAX             |
 | bit  23:20 R/W  FREQ_GAIN             |
 | bit  19:18 R/W  DITHER_SEL            |
 | bit  16    R/W  SEED                  |
 | bit  11:8  R/W  FINAL_SHIFT           |
 | bit  7:4   R/W  INT_GAIN              |
 | bit  3:0   R/W  FILT_GAIN             |
 +--------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1_UNUSED_MASK     0x0002f000
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_MSK   0xf0000000
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF   28
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_MSK   0x0f000000
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF   24
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_MSK   0x00f00000
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF   20
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_MSK  0x000c0000
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF  18
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_SEED_MSK        0x00010000
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_SEED_OFF        16
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_MSK 0x00000f00
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF 8
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_MSK    0x000000f0
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF    4
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_MSK   0x0000000f
#define ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF   0

/*---------------------------------------.
 | Register 0x00000010 ACB_FT_GAIN_REG_2 |
 +---------------------------------------+
 | bit  17   R/W  OUTPUT_DSM_ENABLE      |
 | bit  16   R/W  PHASE_FILT_ENABLE      |
 | bit  15:8 R/W  OUTPUT_UPDATE_RATE     |
 | bit  4:0  R/W  PLL_S_MAX              |
 +--------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2_UNUSED_MASK            0xfffc00e0
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_DSM_ENABLE_MSK  0x00020000
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_DSM_ENABLE_OFF  17
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PHASE_FILT_ENABLE_MSK  0x00010000
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PHASE_FILT_ENABLE_OFF  16
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_MSK 0x0000ff00
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF 8
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_MSK          0x0000001f
#define ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF          0

/*---------------------------------------------------.
 | Register 0x00000014 PACKET_PERIOD_INTEGER_CFG_REG |
 +---------------------------------------------------+
 | bit  15:0 R/W  PKT_PERIOD_INT                     |
 +--------------------------------------------------*/
#define PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_UNUSED_MASK        0xffff0000
#define ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_MSK 0x0000ffff
#define ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF 0

/*-----------------------------------------------------.
 | Register 0x00000018 PACKET_PERIOD_NUMERATOR_MSB_REG |
 +-----------------------------------------------------+
 | bit  8:0 R/W  PKT_PERIOD_N_MSB                      |
 +----------------------------------------------------*/
#define PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_UNUSED_MASK          0xfffffe00
#define ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_MSK 0x000001ff
#define ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF 0

/*-----------------------------------------------------.
 | Register 0x0000001c PACKET_PERIOD_NUMERATOR_LSB_REG |
 +-----------------------------------------------------+
 | bit  31:0 R/W  PKT_PERIOD_N_LSB                     |
 +----------------------------------------------------*/
#define PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_UNUSED_MASK          0x00000000
#define ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_MSK 0xffffffff
#define ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00000020 PACKET_PERIOD_DENOMINATOR_MSB_REG |
 +-------------------------------------------------------+
 | bit  7:0 R/W  PKT_PERIOD_D_MSB                        |
 +------------------------------------------------------*/
#define PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_UNUSED_MASK          0xffffff00
#define ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_MSK 0x000000ff
#define ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF 0

/*-------------------------------------------------------.
 | Register 0x00000024 PACKET_PERIOD_DENOMINATOR_LSB_REG |
 +-------------------------------------------------------+
 | bit  31:0 R/W  PKT_PERIOD_D_LSB                       |
 +------------------------------------------------------*/
#define PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_UNUSED_MASK          0x00000000
#define ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_MSK 0xffffffff
#define ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF 0

/*------------------------------------------------.
 | Register 0x00000028 ACB_FT_PACKET_SIZE_CFG_REG |
 +------------------------------------------------+
 | bit  29:24 R/W  FB_WORDSIZE                    |
 | bit  16    R/W  PKT_SIZE_VAR                   |
 | bit  8:0   R/W  PKT_SIZE                       |
 +-----------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_UNUSED_MASK      0xc0fefe00
#define ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_MSK  0x3f000000
#define ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF  24
#define ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_MSK 0x00010000
#define ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_OFF 16
#define ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK     0x000001ff
#define ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF     0

/*--------------------------------------------------.
 | Register 0x0000002c ACB_FT_THERMOMETER_CFG_REG_1 |
 +--------------------------------------------------+
 | bit  3:0 R/W  THERMOMETER_INT                    |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_UNUSED_MASK         0xfffffff0
#define ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_MSK 0x0000000f
#define ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF 0

/*--------------------------------------------------.
 | Register 0x00000030 ACB_FT_THERMOMETER_CFG_REG_2 |
 +--------------------------------------------------+
 | bit  31:0 R/W  THERMOMETER_N                     |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_UNUSED_MASK       0x00000000
#define ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_MSK 0xffffffff
#define ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF 0

/*--------------------------------------------------.
 | Register 0x00000034 ACB_FT_THERMOMETER_CFG_REG_3 |
 +--------------------------------------------------+
 | bit  31:0 R/W  THERMOMETER_D                     |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_UNUSED_MASK       0x00000000
#define ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_MSK 0xffffffff
#define ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF 0

/*---------------------------------------------------.
 | Register 0x00000038 ACB_FT_FEEDBACK_CNT_CFG_REG_1 |
 +---------------------------------------------------+
 | bit  31:0 R/W  CNT_N                              |
 +--------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_UNUSED_MASK 0x00000000
#define ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_MSK   0xffffffff
#define ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF   0

/*---------------------------------------------------.
 | Register 0x0000003c ACB_FT_FEEDBACK_CNT_CFG_REG_2 |
 +---------------------------------------------------+
 | bit  31:0 R/W  CNT_D                              |
 +--------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_UNUSED_MASK 0x00000000
#define ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_MSK   0xffffffff
#define ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF   0

/*-----------------------------------------------.
 | Register 0x00000044 ACB_FT_OUTPUT_MON_CFG_REG |
 +-----------------------------------------------+
 | bit  3 R/W  OVERRIDE_FILT                     |
 | bit  2 R/W  UPDATE_FILT                       |
 | bit  1 R/W  UPDATE_INT                        |
 | bit  0 R/W  RESET_MINMAX                      |
 +----------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_UNUSED_MASK       0xfffffff0
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_OVERRIDE_FILT_MSK 0x00000008
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_OVERRIDE_FILT_OFF 3
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_FILT_MSK   0x00000004
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_FILT_OFF   2
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_INT_MSK    0x00000002
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_INT_OFF    1
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_RESET_MINMAX_MSK  0x00000001
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_RESET_MINMAX_OFF  0

/*--------------------------------------------------.
 | Register 0x00000048 ACB_FT_OUTPUT_MON_STAT_REG_1 |
 +--------------------------------------------------+
 | bit  24:0 R  PLL_S_OUT_MIN                       |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_UNUSED_MASK       0xfe000000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_MSK 0x01ffffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_OFF 0

/*--------------------------------------------------.
 | Register 0x0000004c ACB_FT_OUTPUT_MON_STAT_REG_2 |
 +--------------------------------------------------+
 | bit  24:0 R  PLL_S_OUT_MAX                       |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_UNUSED_MASK       0xfe000000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_MSK 0x01ffffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_OFF 0

/*--------------------------------------------------.
 | Register 0x00000050 ACB_FT_OUTPUT_MON_STAT_REG_3 |
 +--------------------------------------------------+
 | bit  24:0 R  INTEGRATOR_VALUE                    |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_UNUSED_MASK          0xfe000000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_MSK 0x01ffffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_OFF 0

/*--------------------------------------------------.
 | Register 0x00000054 ACB_FT_OUTPUT_MON_STAT_REG_4 |
 +--------------------------------------------------+
 | bit  24:0 R  FILTER_INT_VALUE                    |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_UNUSED_MASK          0xfe000000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_MSK 0x01ffffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_OFF 0

/*--------------------------------------------------.
 | Register 0x00000058 ACB_FT_OUTPUT_MON_STAT_REG_5 |
 +--------------------------------------------------+
 | bit  15:0 R  FILTER_FRAC_VALUE                   |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_UNUSED_MASK           0xffff0000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_MSK 0x0000ffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_OFF 0

/*--------------------------------------------------.
 | Register 0x0000005c ACB_FT_OUTPUT_MON_STAT_REG_6 |
 +--------------------------------------------------+
 | bit  24:0 R/W  FILTER_INT_OVERRIDE               |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_UNUSED_MASK             0xfe000000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_MSK 0x01ffffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF 0

/*--------------------------------------------------.
 | Register 0x00000060 ACB_FT_OUTPUT_MON_STAT_REG_7 |
 +--------------------------------------------------+
 | bit  15:0 R/W  FILTER_FRAC_OVERRIDE              |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_UNUSED_MASK              0xffff0000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_MSK 0x0000ffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF 0

/*--------------------------------------------------.
 | Register 0x00000064 ACB_FT_OUTPUT_MON_STAT_REG_8 |
 +--------------------------------------------------+
 | bit  31:0 R  B_CNT                               |
 +-------------------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_UNUSED_MASK 0x00000000
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_MSK   0xffffffff
#define ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_OFF   0

/*------------------------------------.
 | Register 0x00000068 ACB_FT_INT_REG |
 +------------------------------------+
 | bit  0 R/W  ACB_FAIL_I             |
 +-----------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_INT_REG_UNUSED_MASK    0xfffffffe
#define ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_MSK 0x00000001
#define ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_OFF 0

/*---------------------------------------.
 | Register 0x0000006c ACB_FT_INT_EN_REG |
 +---------------------------------------+
 | bit  0 R/W  ACB_FAIL_E                |
 +--------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_INT_EN_REG_UNUSED_MASK    0xfffffffe
#define ACB_FT_REG_ACB_FT_INT_EN_REG_BIT_ACB_FAIL_E_MSK 0x00000001
#define ACB_FT_REG_ACB_FT_INT_EN_REG_BIT_ACB_FAIL_E_OFF 0

/*-----------------------------------------.
 | Register 0x00000070 ACB_FT_INT_STAT_REG |
 +-----------------------------------------+
 | bit  0 R  ACB_FAIL_V                    |
 +----------------------------------------*/
#define PMC_ACB_FT_REG_ACB_FT_INT_STAT_REG_UNUSED_MASK    0xfffffffe
#define ACB_FT_REG_ACB_FT_INT_STAT_REG_BIT_ACB_FAIL_V_MSK 0x00000001
#define ACB_FT_REG_ACB_FT_INT_STAT_REG_BIT_ACB_FAIL_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ACB_FT_REGS_H */
