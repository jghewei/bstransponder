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
 *     i7_fec block
 *****************************************************************************/
#ifndef _I7_FEC_REGS_H
#define _I7_FEC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_I7_FEC_REG_FEC_CONFIG             0x0002e000
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_3_CNT 0x0002e004
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_2_CNT 0x0002e008
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_1_CNT 0x0002e00c
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_0_CNT 0x0002e010
#define PMC_I7_FEC_REG_CORRECTED_ONES_3_CNT   0x0002e014
#define PMC_I7_FEC_REG_CORRECTED_ONES_2_CNT   0x0002e018
#define PMC_I7_FEC_REG_CORRECTED_ONES_1_CNT   0x0002e01c
#define PMC_I7_FEC_REG_CORRECTED_ONES_0_CNT   0x0002e020
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_3_CNT 0x0002e024
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_2_CNT 0x0002e028
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_1_CNT 0x0002e02c
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_0_CNT 0x0002e030
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_3_CNT   0x0002e034
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_2_CNT   0x0002e038
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_1_CNT   0x0002e03c
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_0_CNT   0x0002e040
#define PMC_I7_FEC_REG_LLSA_LATCH_SOURCE      0x0002e0a8
#define PMC_I7_FEC_REG_FEC_CTRL               0x0002e0cc

/*--------------------------------.
 | Register 0x0002e000 FEC_CONFIG |
 +--------------------------------+
 | bit  7:6 R/W  STATS_ENABLE     |
 | bit  5:2 R/W  DECODER_DISABLE  |
 | bit  1   R/W  DECODE_TYPE      |
 | bit  0   R/W  ENCODE_TYPE      |
 +-------------------------------*/
#define PMC_I7_FEC_REG_FEC_CONFIG_UNUSED_MASK         0x7ffff000
#define I7_FEC_REG_FEC_CONFIG_BIT_STATS_ENABLE_MSK    0x000000c0
#define I7_FEC_REG_FEC_CONFIG_BIT_STATS_ENABLE_OFF    6
#define I7_FEC_REG_FEC_CONFIG_BIT_DECODER_DISABLE_MSK 0x0000003c
#define I7_FEC_REG_FEC_CONFIG_BIT_DECODER_DISABLE_OFF 2
#define I7_FEC_REG_FEC_CONFIG_BIT_DECODE_TYPE_MSK     0x00000002
#define I7_FEC_REG_FEC_CONFIG_BIT_DECODE_TYPE_OFF     1
#define I7_FEC_REG_FEC_CONFIG_BIT_ENCODE_TYPE_MSK     0x00000001
#define I7_FEC_REG_FEC_CONFIG_BIT_ENCODE_TYPE_OFF     0

/*--------------------------------------------.
 | Register 0x0002e004 CORRECTED_ZEROES_3_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_3_COUNT      |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_3_CNT_UNUSED_MASK                  0x00000000
#define I7_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e008 CORRECTED_ZEROES_2_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_2_COUNT      |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_2_CNT_UNUSED_MASK                  0x00000000
#define I7_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e00c CORRECTED_ZEROES_1_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_1_COUNT      |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_1_CNT_UNUSED_MASK                  0x00000000
#define I7_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e010 CORRECTED_ZEROES_0_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_0_COUNT      |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ZEROES_0_CNT_UNUSED_MASK                  0x00000000
#define I7_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e014 CORRECTED_ONES_3_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_3_COUNT      |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ONES_3_CNT_UNUSED_MASK                0x00000000
#define I7_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e018 CORRECTED_ONES_2_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_2_COUNT      |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ONES_2_CNT_UNUSED_MASK                0x00000000
#define I7_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e01c CORRECTED_ONES_1_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_1_COUNT      |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ONES_1_CNT_UNUSED_MASK                0x00000000
#define I7_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e020 CORRECTED_ONES_0_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_0_COUNT      |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_CORRECTED_ONES_0_CNT_UNUSED_MASK                0x00000000
#define I7_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_MSK 0xffffffff
#define I7_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e024 UNCORRECTED_ERRS_3_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_3_COUNT          |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_3_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_MSK 0xffffffff
#define I7_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e028 UNCORRECTED_ERRS_2_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_2_COUNT          |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_2_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_MSK 0xffffffff
#define I7_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e02c UNCORRECTED_ERRS_1_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_1_COUNT          |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_1_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_MSK 0xffffffff
#define I7_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002e030 UNCORRECTED_ERRS_0_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_0_COUNT          |
 +-------------------------------------------*/
#define PMC_I7_FEC_REG_UNCORRECTED_ERRS_0_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_MSK 0xffffffff
#define I7_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e034 RX_FRAME_PULSE_3_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_3_COUNT        |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_3_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_MSK 0xffffffff
#define I7_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e038 RX_FRAME_PULSE_2_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_2_COUNT        |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_2_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_MSK 0xffffffff
#define I7_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e03c RX_FRAME_PULSE_1_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_1_COUNT        |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_1_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_MSK 0xffffffff
#define I7_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002e040 RX_FRAME_PULSE_0_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_0_COUNT        |
 +-----------------------------------------*/
#define PMC_I7_FEC_REG_RX_FRAME_PULSE_0_CNT_UNUSED_MASK              0x00000000
#define I7_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_MSK 0xffffffff
#define I7_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_OFF 0

/*---------------------------------------.
 | Register 0x0002e0a8 LLSA_LATCH_SOURCE |
 +---------------------------------------+
 | bit  0 R/W  LLSA_LATCH_SOURCE         |
 +--------------------------------------*/
#define PMC_I7_FEC_REG_LLSA_LATCH_SOURCE_UNUSED_MASK           0xfffffffe
#define I7_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_MSK 0x00000001
#define I7_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_OFF 0

/*-------------------------------.
 | Register 0x0002e0cc FEC_CTRL  |
 +-------------------------------+
 | bit  2 R/W  FEC_SW_RAM_LOWPWR |
 | bit  1 R/W  FEC_SW_LOWPWR     |
 | bit  0 R/W  FEC_SW_RST        |
 +------------------------------*/
#define PMC_I7_FEC_REG_FEC_CTRL_UNUSED_MASK           0xfffffff0
#define I7_FEC_REG_FEC_CTRL_BIT_FEC_SW_RAM_LOWPWR_MSK 0x00000004
#define I7_FEC_REG_FEC_CTRL_BIT_FEC_SW_RAM_LOWPWR_OFF 2
#define I7_FEC_REG_FEC_CTRL_BIT_FEC_SW_LOWPWR_MSK     0x00000002
#define I7_FEC_REG_FEC_CTRL_BIT_FEC_SW_LOWPWR_OFF     1
#define I7_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK        0x00000001
#define I7_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF        0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _I7_FEC_REGS_H */
