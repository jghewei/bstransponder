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
 *     s16_adapt_dsp_16 block
 *****************************************************************************/
#ifndef _S16_ADAPT_DSP_16_REGS_H
#define _S16_ADAPT_DSP_16_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL             0x00000300
#define PMC_ADSP_REG_ADAPT_CTRL_1                   0x00000304
#define PMC_ADSP_REG_ADAPT_CTRL_2                   0x00000308
#define PMC_ADSP_REG_ADAPT_CTRL_3                   0x0000030c
#define PMC_ADSP_REG_ADAPT_STAT                     0x00000310
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST  0x00000314
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1        0x00000318
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2        0x0000031c
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3        0x00000320
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4        0x00000324
#define PMC_ADSP_REG_STAT_INT_EN                    0x00000328
#define PMC_ADSP_REG_STAT_INT_EVENT                 0x0000032c
#define PMC_ADSP_REG_STAT_INT                       0x00000330
#define PMC_ADSP_REG_DSS_CTRL                       0x00000334
#define PMC_ADSP_REG_DSS_TAPS_1                     0x00000338
#define PMC_ADSP_REG_DSS_TAPS_2                     0x0000033c
#define PMC_ADSP_REG_DSS_TAPS_3                     0x00000340
#define PMC_ADSP_REG_DSS_TAPS_4                     0x00000344
#define PMC_ADSP_REG_LMS_MU_FACTORS                 0x00000348
#define PMC_ADSP_REG_ROUND_ROBIN_PERIODS            0x0000034c
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1           0x00000350
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2           0x00000354
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3           0x00000358
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4           0x0000035c
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5           0x00000360
#define PMC_ADSP_REG_CLIP_RATE_MON_PARAMS           0x00000364
#define PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS      0x00000368
#define PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS 0x0000036c
#define PMC_ADSP_REG_ADAPT_FORCE_VAL                0x00000370
#define PMC_ADSP_REG_FETX_TTIU_CTRL                 0x00000374
#define PMC_ADSP_REG_FETX_CTRL                      0x00000378
#define PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN      0x0000037c
#define PMC_ADSP_REG_FETX_ADJ_STATUS                0x00000380

/*----------------------------------------.
 | Register 0x00000300 ADAPT_DSP_CLK_CTRL |
 +----------------------------------------+
 | bit  0 R/W  ADAPT_CLK_ENB              |
 +---------------------------------------*/
#define PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL_UNUSED_MASK       0xfffffffe
#define ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK 0x00000001
#define ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF 0

/*----------------------------------.
 | Register 0x00000304 ADAPT_CTRL_1 |
 +----------------------------------+
 | bit  18   R/W  ADAPT_START       |
 | bit  17   R/W  EQ_ADAPT_CTRL_ENB |
 | bit  15   R/W  INV_DFE_OUT       |
 | bit  14   R/W  ADAPT_INITB       |
 | bit  11   R/W  PRELOAD_ENB       |
 | bit  10:8 R/W  ADAPT_DELAY       |
 | bit  7:4  R/W  ADAPT_LOOPS       |
 | bit  3    R/W  ADAPT_ENB         |
 +---------------------------------*/
#define PMC_ADSP_REG_ADAPT_CTRL_1_UNUSED_MASK           0xfff83003
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_START_MSK       0x00040000
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_START_OFF       18
#define ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK 0x00020000
#define ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF 17
#define ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK       0x00008000
#define ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF       15
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK       0x00004000
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF       14
#define ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK       0x00000800
#define ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF       11
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK       0x00000700
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF       8
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK       0x000000f0
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF       4
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK         0x00000008
#define ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF         3

/*----------------------------------.
 | Register 0x00000308 ADAPT_CTRL_2 |
 +----------------------------------+
 | bit  17:0 R/W  LMS_DATA_SEL      |
 +---------------------------------*/
#define PMC_ADSP_REG_ADAPT_CTRL_2_UNUSED_MASK      0xfffc0000
#define ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK 0x0003ffff
#define ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF 0

/*----------------------------------.
 | Register 0x0000030c ADAPT_CTRL_3 |
 +----------------------------------+
 | bit  17:0 R/W  OBJECTS_EN        |
 +---------------------------------*/
#define PMC_ADSP_REG_ADAPT_CTRL_3_UNUSED_MASK    0xfffc0000
#define ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_MSK 0x0003ffff
#define ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF 0

/*---------------------------------------------.
 | Register 0x00000310 ADAPT_STAT              |
 +---------------------------------------------+
 | bit  17:1 R    ADAPT_STATUS                 |
 | bit  0    R/W  CAPTURE_ADAPT_STATUS_REQUEST |
 +--------------------------------------------*/
#define PMC_ADSP_REG_ADAPT_STAT_UNUSED_MASK                      0xfffc0000
#define ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK                 0x0003fffe
#define ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF                 1
#define ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK 0x00000001
#define ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF 0

/*---------------------------------------------------.
 | Register 0x00000314 ADAPT_RESULTS_CAPTURE_REQUEST |
 +---------------------------------------------------+
 | bit  0 R/W  CAPTURE_RESULTS_REQUEST               |
 +--------------------------------------------------*/
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_UNUSED_MASK                 0xfffffffe
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK 0x00000001
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF 0

/*---------------------------------------------.
 | Register 0x00000318 ADAPT_RESULTS_CAPTURE_1 |
 +---------------------------------------------+
 | bit  30:26 R  TR_FFE_B                      |
 | bit  25:21 R  TR_FFE_A                      |
 | bit  20:14 R  DP_FFE_C                      |
 | bit  13:7  R  DP_FFE_B                      |
 | bit  6:0   R  DP_FFE_A                      |
 +--------------------------------------------*/
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1_UNUSED_MASK  0x80000000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK 0x7c000000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF 26
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK 0x03e00000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF 21
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK 0x001fc000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF 14
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK 0x00003f80
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF 7
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK 0x0000007f
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF 0

/*---------------------------------------------.
 | Register 0x0000031c ADAPT_RESULTS_CAPTURE_2 |
 +---------------------------------------------+
 | bit  26:18 R  DFE_COEF2                     |
 | bit  17:9  R  DFE_COEF1                     |
 | bit  8:0   R  DFE_COEF0                     |
 +--------------------------------------------*/
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2_UNUSED_MASK   0xf8000000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK 0x07fc0000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF 18
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK 0x0003fe00
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF 9
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK 0x000001ff
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF 0

/*---------------------------------------------.
 | Register 0x00000320 ADAPT_RESULTS_CAPTURE_3 |
 +---------------------------------------------+
 | bit  26:18 R  DFE_COEF5                     |
 | bit  17:9  R  DFE_COEF4                     |
 | bit  8:0   R  DFE_COEF3                     |
 +--------------------------------------------*/
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3_UNUSED_MASK   0xf8000000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK 0x07fc0000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF 18
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK 0x0003fe00
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF 9
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_MSK 0x000001ff
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_OFF 0

/*---------------------------------------------.
 | Register 0x00000324 ADAPT_RESULTS_CAPTURE_4 |
 +---------------------------------------------+
 | bit  22:19 R  ADC_MAP_SCALE                 |
 | bit  18:13 R  TARGET_AMP                    |
 | bit  12:5  R  PGA_OFFSET                    |
 | bit  4:0   R  PGA_GAIN                      |
 +--------------------------------------------*/
#define PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4_UNUSED_MASK       0xff800000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK 0x00780000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF 19
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK    0x0007e000
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF    13
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK    0x00001fe0
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF    5
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK      0x0000001f
#define ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF      0

/*---------------------------------.
 | Register 0x00000328 STAT_INT_EN |
 +---------------------------------+
 | bit  9 R/W  PROG_REF_DONE_E     |
 | bit  8 R/W  PROG_REF_TIMEOUT_E  |
 | bit  0 R/W  STOP_ADAPT_E        |
 +--------------------------------*/
#define PMC_ADSP_REG_STAT_INT_EN_UNUSED_MASK            0xfffffcfe
#define ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK    0x00000200
#define ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF    9
#define ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK 0x00000100
#define ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF 8
#define ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK       0x00000001
#define ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF       0

/*------------------------------------.
 | Register 0x0000032c STAT_INT_EVENT |
 +------------------------------------+
 | bit  9 R/W  PROG_REF_DONE_I        |
 | bit  8 R/W  PROG_REF_TIMEOUT_I     |
 | bit  0 R/W  STOP_ADAPT_I           |
 +-----------------------------------*/
#define PMC_ADSP_REG_STAT_INT_EVENT_UNUSED_MASK            0xfffffcfe
#define ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK    0x00000200
#define ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF    9
#define ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK 0x00000100
#define ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF 8
#define ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK       0x00000001
#define ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF       0

/*------------------------------.
 | Register 0x00000330 STAT_INT |
 +------------------------------+
 | bit  9 R  PROG_REF_DONE_V    |
 | bit  8 R  PROG_REF_TIMEOUT_V |
 | bit  0 R  STOP_ADAPT_V       |
 +-----------------------------*/
#define PMC_ADSP_REG_STAT_INT_UNUSED_MASK            0xfffffcfe
#define ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK    0x00000200
#define ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF    9
#define ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK 0x00000100
#define ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF 8
#define ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_MSK       0x00000001
#define ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_OFF       0

/*--------------------------------------.
 | Register 0x00000334 DSS_CTRL         |
 +--------------------------------------+
 | bit  31:28 R/W  EXPECTED_EQ2         |
 | bit  27:24 R/W  EXPECTED_EQ1         |
 | bit  23:16 R/W  H_LIMIT              |
 | bit  15:8  R/W  S_LIMIT              |
 | bit  7:2   R/W  OFL_DECAY            |
 | bit  1     R/W  PATH_PATTERN_CHK_ENB |
 | bit  0     R/W  DSS_ENB              |
 +-------------------------------------*/
#define PMC_ADSP_REG_DSS_CTRL_UNUSED_MASK              0x00000000
#define ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK         0xf0000000
#define ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF         28
#define ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK         0x0f000000
#define ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF         24
#define ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK              0x00ff0000
#define ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF              16
#define ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK              0x0000ff00
#define ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF              8
#define ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK            0x000000fc
#define ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF            2
#define ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK 0x00000002
#define ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF 1
#define ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK              0x00000001
#define ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF              0

/*----------------------------------------.
 | Register 0x00000338 DSS_TAPS_1         |
 +----------------------------------------+
 | bit  30    R/W  DFE_COEF5_TAP_OFL1_EN  |
 | bit  29    R/W  DFE_COEF4_TAP_OFL1_EN  |
 | bit  28    R/W  DFE_COEF3_TAP_OFL1_EN  |
 | bit  27    R/W  DFE_COEF2_TAP_OFL1_EN  |
 | bit  26    R/W  DFE_COEF1_TAP_OFL1_EN  |
 | bit  25    R/W  DFE_COEF0_TAP_OFL1_EN  |
 | bit  24    R/W  TARGET_AMP_TAP_OFL1_EN |
 | bit  23:20 R/W  DFE_COEF5_TAP          |
 | bit  19:16 R/W  DFE_COEF4_TAP          |
 | bit  15:12 R/W  DFE_COEF3_TAP          |
 | bit  11:8  R/W  DFE_COEF2_TAP          |
 | bit  7:4   R/W  DFE_COEF1_TAP          |
 | bit  3:0   R/W  DFE_COEF0_TAP          |
 +---------------------------------------*/
#define PMC_ADSP_REG_DSS_TAPS_1_UNUSED_MASK                0x80000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_MSK  0x40000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_OFF  30
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_MSK  0x20000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_OFF  29
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_MSK  0x10000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_OFF  28
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_MSK  0x08000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_OFF  27
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_MSK  0x04000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_OFF  26
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_MSK  0x02000000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_OFF  25
#define ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_MSK 0x01000000
#define ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_OFF 24
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK          0x00f00000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF          20
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK          0x000f0000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF          16
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK          0x0000f000
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF          12
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK          0x00000f00
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF          8
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK          0x000000f0
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF          4
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK          0x0000000f
#define ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF          0

/*--------------------------------.
 | Register 0x0000033c DSS_TAPS_2 |
 +--------------------------------+
 | bit  3:0 R/W  GAIN_TAP         |
 +-------------------------------*/
#define PMC_ADSP_REG_DSS_TAPS_2_UNUSED_MASK  0xfffffff0
#define ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK 0x0000000f
#define ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF 0

/*--------------------------------------.
 | Register 0x00000340 DSS_TAPS_3       |
 +--------------------------------------+
 | bit  30    R/W  TAP_OFL2_EN          |
 | bit  29:27 R/W  TAP2                 |
 | bit  26    R/W  DP_FFE_C_TAP_OFL1_EN |
 | bit  25    R/W  DP_FFE_B_TAP_OFL1_EN |
 | bit  24    R/W  DP_FFE_A_TAP_OFL1_EN |
 | bit  23:20 R/W  DP_FFE_C_TAP         |
 | bit  19:16 R/W  DP_FFE_B_TAP         |
 | bit  15:12 R/W  DP_FFE_A_TAP         |
 | bit  11:8  R/W  TARGET_AMP_TAP       |
 | bit  7:4   R/W  ADC_TAP              |
 | bit  3:0   R/W  OFFSET_TAP           |
 +-------------------------------------*/
#define PMC_ADSP_REG_DSS_TAPS_3_UNUSED_MASK              0x80000000
#define ADSP_REG_DSS_TAPS_3_BIT_TAP_OFL2_EN_MSK          0x40000000
#define ADSP_REG_DSS_TAPS_3_BIT_TAP_OFL2_EN_OFF          30
#define ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK                 0x38000000
#define ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF                 27
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_MSK 0x04000000
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_OFF 26
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_MSK 0x02000000
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_OFF 25
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_MSK 0x01000000
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_OFF 24
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK         0x00f00000
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF         20
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK         0x000f0000
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF         16
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK         0x0000f000
#define ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF         12
#define ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK       0x00000f00
#define ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF       8
#define ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK              0x000000f0
#define ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF              4
#define ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK           0x0000000f
#define ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF           0

/*--------------------------------------.
 | Register 0x00000344 DSS_TAPS_4       |
 +--------------------------------------+
 | bit  14   R/W  FETX_POST_TAP_OFL1_EN |
 | bit  13   R/W  FETX_CTR_TAP_OFL1_EN  |
 | bit  12   R/W  FETX_PRE_TAP_OFL1_EN  |
 | bit  11:8 R/W  FETX_POST_TAP         |
 | bit  7:4  R/W  FETX_CTR_TAP          |
 | bit  3:0  R/W  FETX_PRE_TAP          |
 +-------------------------------------*/
#define PMC_ADSP_REG_DSS_TAPS_4_UNUSED_MASK               0xffff8000
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFL1_EN_MSK 0x00004000
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFL1_EN_OFF 14
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFL1_EN_MSK  0x00002000
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFL1_EN_OFF  13
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFL1_EN_MSK  0x00001000
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFL1_EN_OFF  12
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_MSK         0x00000f00
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF         8
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_MSK          0x000000f0
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF          4
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_MSK          0x0000000f
#define ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF          0

/*------------------------------------.
 | Register 0x00000348 LMS_MU_FACTORS |
 +------------------------------------+
 | bit  13   R/W  LSB_HYSTERESIS_EN   |
 | bit  12   R/W  DELTA_H_RND_EN      |
 | bit  11:9 R/W  MU_STEP_ADC         |
 | bit  8:6  R/W  MU_STEP_COEF        |
 | bit  5:3  R/W  MU_STEP_OFFSET      |
 | bit  2:0  R/W  MU_STEP_GAIN        |
 +-----------------------------------*/
#define PMC_ADSP_REG_LMS_MU_FACTORS_UNUSED_MASK           0xffffc000
#define ADSP_REG_LMS_MU_FACTORS_BIT_LSB_HYSTERESIS_EN_MSK 0x00002000
#define ADSP_REG_LMS_MU_FACTORS_BIT_LSB_HYSTERESIS_EN_OFF 13
#define ADSP_REG_LMS_MU_FACTORS_BIT_DELTA_H_RND_EN_MSK    0x00001000
#define ADSP_REG_LMS_MU_FACTORS_BIT_DELTA_H_RND_EN_OFF    12
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK       0x00000e00
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF       9
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK      0x000001c0
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF      6
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK    0x00000038
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF    3
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK      0x00000007
#define ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF      0

/*-----------------------------------------.
 | Register 0x0000034c ROUND_ROBIN_PERIODS |
 +-----------------------------------------+
 | bit  23:20 R/W  RR_PERIOD_FETX          |
 | bit  19:16 R/W  RR_PERIOD_DSS           |
 | bit  15:12 R/W  RR_PERIOD_ADC_MAP       |
 | bit  11:8  R/W  RR_PERIOD_OFFSET        |
 | bit  7:4   R/W  RR_PERIOD_GAIN          |
 | bit  3:0   R/W  RR_PERIOD_COEF          |
 +----------------------------------------*/
#define PMC_ADSP_REG_ROUND_ROBIN_PERIODS_UNUSED_MASK           0xff000000
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_MSK    0x00f00000
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF    20
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK     0x000f0000
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF     16
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK 0x0000f000
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF 12
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK  0x00000f00
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF  8
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK    0x000000f0
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF    4
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK    0x0000000f
#define ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF    0

/*------------------------------------------.
 | Register 0x00000350 OBJECT_PRELOAD_VAL_1 |
 +------------------------------------------+
 | bit  30:26 R/W  TR_FFE_B_PRELOAD         |
 | bit  25:21 R/W  TR_FFE_A_PRELOAD         |
 | bit  20:14 R/W  DP_FFE_C_PRELOAD         |
 | bit  13:7  R/W  DP_FFE_B_PRELOAD         |
 | bit  6:0   R/W  DP_FFE_A_PRELOAD         |
 +-----------------------------------------*/
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1_UNUSED_MASK          0x80000000
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK 0x7c000000
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF 26
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK 0x03e00000
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF 21
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK 0x001fc000
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF 14
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK 0x00003f80
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF 7
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK 0x0000007f
#define ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF 0

/*------------------------------------------.
 | Register 0x00000354 OBJECT_PRELOAD_VAL_2 |
 +------------------------------------------+
 | bit  26:18 R/W  DFE_COEF2_PRELOAD        |
 | bit  17:9  R/W  DFE_COEF1_PRELOAD        |
 | bit  8:0   R/W  DFE_COEF0_PRELOAD        |
 +-----------------------------------------*/
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2_UNUSED_MASK           0xf8000000
#define ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK 0x07fc0000
#define ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF 18
#define ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK 0x0003fe00
#define ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF 9
#define ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK 0x000001ff
#define ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF 0

/*------------------------------------------.
 | Register 0x00000358 OBJECT_PRELOAD_VAL_3 |
 +------------------------------------------+
 | bit  26:18 R/W  DFE_COEF5_PRELOAD        |
 | bit  17:9  R/W  DFE_COEF4_PRELOAD        |
 | bit  8:0   R/W  DFE_COEF3_PRELOAD        |
 +-----------------------------------------*/
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3_UNUSED_MASK           0xf8000000
#define ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK 0x07fc0000
#define ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF 18
#define ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK 0x0003fe00
#define ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF 9
#define ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_MSK 0x000001ff
#define ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF 0

/*------------------------------------------.
 | Register 0x0000035c OBJECT_PRELOAD_VAL_4 |
 +------------------------------------------+
 | bit  22:19 R/W  ADC_MAP_PRELOAD          |
 | bit  18:13 R/W  TARGET_AMP_PRELOAD       |
 | bit  12:5  R/W  PGA_OFFSET_PRELOAD       |
 | bit  4:0   R/W  PGA_GAIN_PRELOAD         |
 +-----------------------------------------*/
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4_UNUSED_MASK            0xff800000
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK    0x00780000
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF    19
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK 0x0007e000
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF 13
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK 0x00001fe0
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF 5
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK   0x0000001f
#define ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF   0

/*------------------------------------------.
 | Register 0x00000360 OBJECT_PRELOAD_VAL_5 |
 +------------------------------------------+
 | bit  20:14 R/W  FETX_POST_PRELOAD        |
 | bit  13:7  R/W  FETX_CTR_PRELOAD         |
 | bit  6:0   R/W  FETX_PRE_PRELOAD         |
 +-----------------------------------------*/
#define PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5_UNUSED_MASK           0xffe00000
#define ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_MSK 0x001fc000
#define ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF 14
#define ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_MSK  0x00003f80
#define ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF  7
#define ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_MSK  0x0000007f
#define ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF  0

/*------------------------------------------.
 | Register 0x00000364 CLIP_RATE_MON_PARAMS |
 +------------------------------------------+
 | bit  19    R/W  PROG_REF_OVR_EN          |
 | bit  18    R/W  PROG_REF_OVR             |
 | bit  17:14 R/W  PROG_REF_TIMEOUT_PERIOD  |
 | bit  13:11 R/W  CLIP_RATE_THRESH         |
 | bit  10:5  R/W  CLIP_RATE_AMP            |
 | bit  4:2   R/W  GAIN_OFFSET              |
 | bit  1:0   R/W  ADC_OFFSET               |
 +-----------------------------------------*/
#define PMC_ADSP_REG_CLIP_RATE_MON_PARAMS_UNUSED_MASK                 0xfff00000
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK         0x00080000
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF         19
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK            0x00040000
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF            18
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK 0x0003c000
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF 14
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK        0x00003800
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF        11
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK           0x000007e0
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF           5
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK             0x0000001c
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF             2
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK              0x00000003
#define ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF              0

/*-----------------------------------------------.
 | Register 0x00000368 CLIP_RATE_MON_ADC_RESULTS |
 +-----------------------------------------------+
 | bit  9   R/W  CAPTURE_GAIN_CLIP_FLAG_REQUEST  |
 | bit  7:2 R    CLIP_FLAG_ADC                   |
 | bit  0   R/W  CAPTURE_ADC_CLIP_FLAGS_REQUEST  |
 +----------------------------------------------*/
#define PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_UNUSED_MASK                        0xfffffd02
#define ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK 0x00000200
#define ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF 9
#define ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK                  0x000000fc
#define ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF                  2
#define ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK 0x00000001
#define ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF 0

/*----------------------------------------------------.
 | Register 0x0000036c CLIP_RATE_MON_PGA_GAIN_RESULTS |
 +----------------------------------------------------+
 | bit  31:0 R  CLIP_FLAG_GAIN                        |
 +---------------------------------------------------*/
#define PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_UNUSED_MASK        0x00000000
#define ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK 0xffffffff
#define ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF 0

/*-------------------------------------.
 | Register 0x00000370 ADAPT_FORCE_VAL |
 +-------------------------------------+
 | bit  17:0 R/W  FORCE_ADAPT_VALUE_EN |
 +------------------------------------*/
#define PMC_ADSP_REG_ADAPT_FORCE_VAL_UNUSED_MASK              0xfffc0000
#define ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_MSK 0x0003ffff
#define ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF 0

/*------------------------------------.
 | Register 0x00000374 FETX_TTIU_CTRL |
 +------------------------------------+
 | bit  17:0 R/W  TTIU_REJECT_EN      |
 +-----------------------------------*/
#define PMC_ADSP_REG_FETX_TTIU_CTRL_UNUSED_MASK        0xfffc0000
#define ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_MSK 0x0003ffff
#define ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF 0

/*-----------------------------------------.
 | Register 0x00000378 FETX_CTRL           |
 +-----------------------------------------+
 | bit  30:25 R/W  TTIU_COUNT_MAX          |
 | bit  24    R/W  FETX_UPDATE_REQ_CLR_EN  |
 | bit  23    R/W  FETX_READY_CHECK_ENB    |
 | bit  22    R/W  SWAP_DEC_INC_EN         |
 | bit  21    R/W  HANDLE_MIN_FETX_PRE_EN  |
 | bit  20    R/W  HANDLE_MAX_FETX_PRE_EN  |
 | bit  19    R/W  HANDLE_MIN_FETX_CTR_EN  |
 | bit  18    R/W  HANDLE_MAX_FETX_CTR_EN  |
 | bit  17    R/W  HANDLE_MIN_FETX_POST_EN |
 | bit  16    R/W  HANDLE_MAX_FETX_POST_EN |
 | bit  15    R/W  FETX_COEF_SET_SEL       |
 | bit  14:8  R/W  FETX_ADJ_NEG_THRESH     |
 | bit  7:1   R/W  FETX_ADJ_POS_THRESH     |
 | bit  0     R/W  FETX_ADAPT_MODE         |
 +----------------------------------------*/
#define PMC_ADSP_REG_FETX_CTRL_UNUSED_MASK                 0x80000000
#define ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_MSK          0x7e000000
#define ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF          25
#define ADSP_REG_FETX_CTRL_BIT_FETX_UPDATE_REQ_CLR_EN_MSK  0x01000000
#define ADSP_REG_FETX_CTRL_BIT_FETX_UPDATE_REQ_CLR_EN_OFF  24
#define ADSP_REG_FETX_CTRL_BIT_FETX_READY_CHECK_ENB_MSK    0x00800000
#define ADSP_REG_FETX_CTRL_BIT_FETX_READY_CHECK_ENB_OFF    23
#define ADSP_REG_FETX_CTRL_BIT_SWAP_DEC_INC_EN_MSK         0x00400000
#define ADSP_REG_FETX_CTRL_BIT_SWAP_DEC_INC_EN_OFF         22
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_PRE_EN_MSK  0x00200000
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_PRE_EN_OFF  21
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_PRE_EN_MSK  0x00100000
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_PRE_EN_OFF  20
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_CTR_EN_MSK  0x00080000
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_CTR_EN_OFF  19
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_CTR_EN_MSK  0x00040000
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_CTR_EN_OFF  18
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_POST_EN_MSK 0x00020000
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_POST_EN_OFF 17
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_POST_EN_MSK 0x00010000
#define ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_POST_EN_OFF 16
#define ADSP_REG_FETX_CTRL_BIT_FETX_COEF_SET_SEL_MSK       0x00008000
#define ADSP_REG_FETX_CTRL_BIT_FETX_COEF_SET_SEL_OFF       15
#define ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_MSK     0x00007f00
#define ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF     8
#define ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_MSK     0x000000fe
#define ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF     1
#define ADSP_REG_FETX_CTRL_BIT_FETX_ADAPT_MODE_MSK         0x00000001
#define ADSP_REG_FETX_CTRL_BIT_FETX_ADAPT_MODE_OFF         0

/*-----------------------------------------------.
 | Register 0x0000037c TTIU_FRAME_MARKER_PATTERN |
 +-----------------------------------------------+
 | bit  13:0 R/W  TTIU_FM_PATTERN                |
 +----------------------------------------------*/
#define PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN_UNUSED_MASK         0xffffc000
#define ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_MSK 0x00003fff
#define ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF 0

/*------------------------------------------.
 | Register 0x00000380 FETX_ADJ_STATUS      |
 +------------------------------------------+
 | bit  30    R    TTIU_FM_CST_STAT         |
 | bit  29    R    TTIU_FM_STAT             |
 | bit  28:20 R    FETX_POST_ADJ            |
 | bit  19:11 R    FETX_CTR_ADJ             |
 | bit  10:2  R    FETX_PRE_ADJ             |
 | bit  1     R/W  FETX_ADJ_CLR             |
 | bit  0     R/W  CAPTURE_FETX_ADJ_REQUEST |
 +-----------------------------------------*/
#define PMC_ADSP_REG_FETX_ADJ_STATUS_UNUSED_MASK                  0x80000000
#define ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_CST_STAT_MSK         0x40000000
#define ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_CST_STAT_OFF         30
#define ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_STAT_MSK             0x20000000
#define ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_STAT_OFF             29
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_MSK            0x1ff00000
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_OFF            20
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_MSK             0x000ff800
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_OFF             11
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_MSK             0x000007fc
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_OFF             2
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_ADJ_CLR_MSK             0x00000002
#define ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_ADJ_CLR_OFF             1
#define ADSP_REG_FETX_ADJ_STATUS_BIT_CAPTURE_FETX_ADJ_REQUEST_MSK 0x00000001
#define ADSP_REG_FETX_ADJ_STATUS_BIT_CAPTURE_FETX_ADJ_REQUEST_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _S16_ADAPT_DSP_16_REGS_H */
