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
 *     s16_titan_br2_mdsp block
 *****************************************************************************/
#ifndef _S16_TITAN_BR2_MDSP_REGS_H
#define _S16_TITAN_BR2_MDSP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP                                       0x00000200
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP                                       0x00000204
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP                                       0x00000208
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP                                       0x0000020c
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP                                       0x00000210
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP                                       0x00000214
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP                                       0x00000218
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP                                       0x0000021c
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP                                       0x00000220
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP                                      0x00000224
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP                                      0x00000228
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 0x0000022c
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2                                   0x00000230
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2                                   0x00000234
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2                                   0x00000238
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2                                   0x0000023c
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16                                   0x00000240
#define PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0                                  0x00000244
#define PMC_TITAN_BR2_DIGI_REG_DFE_STATE                                                    0x00000248
#define PMC_TITAN_BR2_DIGI_REG_TCB_STATE_0                                                  0x0000024c
#define PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL                                                 0x00000250

/*--------------------------------------------.
 | Register 0x00000200 CONFIG_1_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_D_MAP_4                |
 | bit  23:18 R/W  ADC_D_MAP_3                |
 | bit  17:12 R/W  ADC_D_MAP_2                |
 | bit  11:6  R/W  ADC_D_MAP_1                |
 | bit  5:0   R/W  ADC_D_MAP_0                |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_UNUSED_MASK     0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_4_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_3_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_2_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_1_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_1_PM20_80_48_PP_BIT_ADC_D_MAP_0_OFF 0

/*--------------------------------------------.
 | Register 0x00000204 CONFIG_2_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_D_MAP_9                |
 | bit  23:18 R/W  ADC_D_MAP_8                |
 | bit  17:12 R/W  ADC_D_MAP_7                |
 | bit  11:6  R/W  ADC_D_MAP_6                |
 | bit  5:0   R/W  ADC_D_MAP_5                |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_UNUSED_MASK     0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_9_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_8_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_7_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_6_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_2_PM20_80_48_PP_BIT_ADC_D_MAP_5_OFF 0

/*--------------------------------------------.
 | Register 0x00000208 CONFIG_3_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_D_MAP_14               |
 | bit  23:18 R/W  ADC_D_MAP_13               |
 | bit  17:12 R/W  ADC_D_MAP_12               |
 | bit  11:6  R/W  ADC_D_MAP_11               |
 | bit  5:0   R/W  ADC_D_MAP_10               |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_UNUSED_MASK      0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_14_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_13_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_12_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_11_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_3_PM20_80_48_PP_BIT_ADC_D_MAP_10_OFF 0

/*--------------------------------------------.
 | Register 0x0000020c CONFIG_4_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_D_MAP_19               |
 | bit  23:18 R/W  ADC_D_MAP_18               |
 | bit  17:12 R/W  ADC_D_MAP_17               |
 | bit  11:6  R/W  ADC_D_MAP_16               |
 | bit  5:0   R/W  ADC_D_MAP_15               |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_UNUSED_MASK      0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_19_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_18_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_17_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_16_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_4_PM20_80_48_PP_BIT_ADC_D_MAP_15_OFF 0

/*--------------------------------------------.
 | Register 0x00000210 CONFIG_5_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_D_MAP_24               |
 | bit  23:18 R/W  ADC_D_MAP_23               |
 | bit  17:12 R/W  ADC_D_MAP_22               |
 | bit  11:6  R/W  ADC_D_MAP_21               |
 | bit  5:0   R/W  ADC_D_MAP_20               |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_UNUSED_MASK      0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_24_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_23_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_22_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_21_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_5_PM20_80_48_PP_BIT_ADC_D_MAP_20_OFF 0

/*--------------------------------------------.
 | Register 0x00000214 CONFIG_6_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_D_MAP_29               |
 | bit  23:18 R/W  ADC_D_MAP_28               |
 | bit  17:12 R/W  ADC_D_MAP_27               |
 | bit  11:6  R/W  ADC_D_MAP_26               |
 | bit  5:0   R/W  ADC_D_MAP_25               |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_UNUSED_MASK      0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_29_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_28_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_27_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_26_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_6_PM20_80_48_PP_BIT_ADC_D_MAP_25_OFF 0

/*--------------------------------------------.
 | Register 0x00000218 CONFIG_7_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  11:6 R/W  ADC_D_MAP_31                |
 | bit  5:0  R/W  ADC_D_MAP_30                |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_UNUSED_MASK      0xfffff000
#define TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_31_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_7_PM20_80_48_PP_BIT_ADC_D_MAP_30_OFF 0

/*--------------------------------------------.
 | Register 0x0000021c CONFIG_8_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_T_MAP_4                |
 | bit  23:18 R/W  ADC_T_MAP_3                |
 | bit  17:12 R/W  ADC_T_MAP_2                |
 | bit  11:6  R/W  ADC_T_MAP_1                |
 | bit  5:0   R/W  ADC_T_MAP_0                |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_UNUSED_MASK     0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_4_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_3_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_2_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_1_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_8_PM20_80_48_PP_BIT_ADC_T_MAP_0_OFF 0

/*--------------------------------------------.
 | Register 0x00000220 CONFIG_9_PM20_80_48_PP |
 +--------------------------------------------+
 | bit  29:24 R/W  ADC_T_MAP_9                |
 | bit  23:18 R/W  ADC_T_MAP_8                |
 | bit  17:12 R/W  ADC_T_MAP_7                |
 | bit  11:6  R/W  ADC_T_MAP_6                |
 | bit  5:0   R/W  ADC_T_MAP_5                |
 +-------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_UNUSED_MASK     0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_9_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_8_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_7_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_6_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_9_PM20_80_48_PP_BIT_ADC_T_MAP_5_OFF 0

/*---------------------------------------------.
 | Register 0x00000224 CONFIG_10_PM20_80_48_PP |
 +---------------------------------------------+
 | bit  29:24 R/W  ADC_T_MAP_14                |
 | bit  23:18 R/W  ADC_T_MAP_13                |
 | bit  17:12 R/W  ADC_T_MAP_12                |
 | bit  11:6  R/W  ADC_T_MAP_11                |
 | bit  5:0   R/W  ADC_T_MAP_10                |
 +--------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_UNUSED_MASK      0xc0000000
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_MSK 0x3f000000
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_14_OFF 24
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_MSK 0x00fc0000
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_13_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_12_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_MSK 0x00000fc0
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_11_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_10_PM20_80_48_PP_BIT_ADC_T_MAP_10_OFF 0

/*---------------------------------------------.
 | Register 0x00000228 CONFIG_11_PM20_80_48_PP |
 +---------------------------------------------+
 | bit  5:0 R/W  ADC_T_MAP_15                  |
 +--------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_UNUSED_MASK      0xffffffc0
#define TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_MSK 0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_11_PM20_80_48_PP_BIT_ADC_T_MAP_15_OFF 0

/*----------------------------------------------------------------------------------.
 | Register 0x0000022c CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 |
 +----------------------------------------------------------------------------------+
 | bit  9:8 R/W  BLOCK_SIZE                                                         |
 | bit  6   R/W  CFG_FROM_PCBI_EN                                                   |
 | bit  5   R/W  PCBI_PARAM_EN                                                      |
 | bit  2   R/W  INV_DFE_OUT                                                        |
 | bit  1   R/W  INV_DFE_ERR                                                        |
 | bit  0   R/W  INV_DP_FFE_IN                                                      |
 +---------------------------------------------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_UNUSED_MASK          0xfffffc98
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_MSK       0x00000300
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_BLOCK_SIZE_OFF       8
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_CFG_FROM_PCBI_EN_MSK 0x00000040
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_CFG_FROM_PCBI_EN_OFF 6
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_PCBI_PARAM_EN_MSK    0x00000020
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_PCBI_PARAM_EN_OFF    5
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_OUT_MSK      0x00000004
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_OUT_OFF      2
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_ERR_MSK      0x00000002
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DFE_ERR_OFF      1
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DP_FFE_IN_MSK    0x00000001
#define TITAN_BR2_DIGI_REG_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_BIT_INV_DP_FFE_IN_OFF    0

/*------------------------------------------------.
 | Register 0x00000230 CONFIG_1_LW20_80_49_TR_BR2 |
 +------------------------------------------------+
 | bit  26    R/W  ENCODER_FLOP_BYPASS_EN         |
 | bit  25    R/W  IMUXCODE_BRIDGE_EN             |
 | bit  24    R/W  UNLOCK_EYE_EDGE_EN             |
 | bit  23:21 R/W  LF_INTEG_LEFT_SHIFT            |
 | bit  20:16 R/W  LF_INTEG_CTRL_GAIN             |
 | bit  15:14 R/W  LF_PROP_LEFT_SHIFT             |
 | bit  13:9  R/W  LF_PROP_CTRL_GAIN              |
 | bit  5:0   R/W  LF_BIAS                        |
 +-----------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_UNUSED_MASK                0xf80001c0
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_ENCODER_FLOP_BYPASS_EN_MSK 0x04000000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_ENCODER_FLOP_BYPASS_EN_OFF 26
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_IMUXCODE_BRIDGE_EN_MSK     0x02000000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_IMUXCODE_BRIDGE_EN_OFF     25
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_UNLOCK_EYE_EDGE_EN_MSK     0x01000000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_UNLOCK_EYE_EDGE_EN_OFF     24
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_MSK    0x00e00000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_LEFT_SHIFT_OFF    21
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_MSK     0x001f0000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_INTEG_CTRL_GAIN_OFF     16
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_MSK     0x0000c000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_LEFT_SHIFT_OFF     14
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_MSK      0x00003e00
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_PROP_CTRL_GAIN_OFF      9
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_MSK                0x0000003f
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_49_TR_BR2_BIT_LF_BIAS_OFF                0

/*------------------------------------------------.
 | Register 0x00000234 CONFIG_2_LW20_80_49_TR_BR2 |
 +------------------------------------------------+
 | bit  26:21 R/W  INTERP_IDX_OVR                 |
 | bit  20    R/W  INTERP_IDX_OVR_EN              |
 | bit  19:18 R/W  LF_INTEG_OUT_MODE              |
 | bit  11:0  R/W  LF_INTEG_OUT_OVR               |
 +-----------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_UNUSED_MASK           0xf803f000
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_MSK    0x07e00000
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_OFF    21
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_EN_MSK 0x00100000
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_INTERP_IDX_OVR_EN_OFF 20
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_MSK 0x000c0000
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_MODE_OFF 18
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_MSK  0x00000fff
#define TITAN_BR2_DIGI_REG_CONFIG_2_LW20_80_49_TR_BR2_BIT_LF_INTEG_OUT_OVR_OFF  0

/*------------------------------------------------.
 | Register 0x00000238 CONFIG_3_LW20_80_49_TR_BR2 |
 +------------------------------------------------+
 | bit  11:0 R/W  LF_CLAMP_MAX                    |
 +-----------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_UNUSED_MASK      0xfffff000
#define TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_MSK 0x00000fff
#define TITAN_BR2_DIGI_REG_CONFIG_3_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MAX_OFF 0

/*------------------------------------------------.
 | Register 0x0000023c CONFIG_4_LW20_80_49_TR_BR2 |
 +------------------------------------------------+
 | bit  17:12 R/W  EQ_TARGET_AMP                  |
 | bit  11:0  R/W  LF_CLAMP_MIN                   |
 +-----------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_UNUSED_MASK       0xfffc0000
#define TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_EQ_TARGET_AMP_OFF 12
#define TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_MSK  0x00000fff
#define TITAN_BR2_DIGI_REG_CONFIG_4_LW20_80_49_TR_BR2_BIT_LF_CLAMP_MIN_OFF  0

/*------------------------------------------------.
 | Register 0x00000240 CONFIG_1_LW20_80_46_DP_T16 |
 +------------------------------------------------+
 | bit  20    R/W  PTW_SUM_OVR_EN                 |
 | bit  17:12 R/W  PTW_SUM_SEL                    |
 | bit  11:0  R/W  PTW_SUM_OVR                    |
 +-----------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_UNUSED_MASK        0xffec0000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_EN_MSK 0x00100000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_EN_OFF 20
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_MSK    0x0003f000
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_SEL_OFF    12
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_MSK    0x00000fff
#define TITAN_BR2_DIGI_REG_CONFIG_1_LW20_80_46_DP_T16_BIT_PTW_SUM_OVR_OFF    0

/*-------------------------------------------------.
 | Register 0x00000244 PRE_PROCESS_DATA_CAPTURED_0 |
 +-------------------------------------------------+
 | bit  17:12 R  INTERP_IDX_CAPT                   |
 | bit  11:6  R  PP_T_DATA_CAPT                    |
 | bit  5:0   R  PP_D_DATA_CAPT                    |
 +------------------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_UNUSED_MASK         0xfffc0000
#define TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_MSK 0x0003f000
#define TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_INTERP_IDX_CAPT_OFF 12
#define TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_MSK  0x00000fc0
#define TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_T_DATA_CAPT_OFF  6
#define TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_MSK  0x0000003f
#define TITAN_BR2_DIGI_REG_PRE_PROCESS_DATA_CAPTURED_0_BIT_PP_D_DATA_CAPT_OFF  0

/*-------------------------------.
 | Register 0x00000248 DFE_STATE |
 +-------------------------------+
 | bit  15:0 R  DFE_STATE_CAPT   |
 +------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_DFE_STATE_UNUSED_MASK        0xffff0000
#define TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_MSK 0x0000ffff
#define TITAN_BR2_DIGI_REG_DFE_STATE_BIT_DFE_STATE_CAPT_OFF 0

/*---------------------------------.
 | Register 0x0000024c TCB_STATE_0 |
 +---------------------------------+
 | bit  17:0 R  INTEGRAL_ACC       |
 +--------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_TCB_STATE_0_UNUSED_MASK      0xfffc0000
#define TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_MSK 0x0003ffff
#define TITAN_BR2_DIGI_REG_TCB_STATE_0_BIT_INTEGRAL_ACC_OFF 0

/*----------------------------------------.
 | Register 0x00000250 DIAG_CONTROL       |
 +----------------------------------------+
 | bit  29:25 R/W  CAPT_RDBK_SEL          |
 | bit  24:19 R/W  INTERP_IDX_CAPT_TIME   |
 | bit  18:13 R/W  INTEGRAL_ACC_CAPT_TIME |
 | bit  12:7  R/W  DFE_STATE_CAPT_TIME    |
 | bit  6:1   R/W  SAMP_CAPT_TIME         |
 | bit  0     R/W  CAPTURE_REQ            |
 +---------------------------------------*/
#define PMC_TITAN_BR2_DIGI_REG_DIAG_CONTROL_UNUSED_MASK                0xc0000000
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_MSK          0x3e000000
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPT_RDBK_SEL_OFF          25
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_MSK   0x01f80000
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTERP_IDX_CAPT_TIME_OFF   19
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_MSK 0x0007e000
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_INTEGRAL_ACC_CAPT_TIME_OFF 13
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_MSK    0x00001f80
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_DFE_STATE_CAPT_TIME_OFF    7
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_MSK         0x0000007e
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_SAMP_CAPT_TIME_OFF         1
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPTURE_REQ_MSK            0x00000001
#define TITAN_BR2_DIGI_REG_DIAG_CONTROL_BIT_CAPTURE_REQ_OFF            0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _S16_TITAN_BR2_MDSP_REGS_H */
