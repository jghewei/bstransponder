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
 *     i4_fec block
 *****************************************************************************/
#ifndef _I4_FEC_REGS_H
#define _I4_FEC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_I4_FEC_REG_FEC_ENCODER_CONFIG                    0x00028000
#define PMC_I4_FEC_REG_FEC_DECODER_CONFIG                    0x00028004
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_3_CNT                0x00028008
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_2_CNT                0x0002800c
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_1_CNT                0x00028010
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_0_CNT                0x00028014
#define PMC_I4_FEC_REG_CORRECTED_ONES_3_CNT                  0x00028018
#define PMC_I4_FEC_REG_CORRECTED_ONES_2_CNT                  0x0002801c
#define PMC_I4_FEC_REG_CORRECTED_ONES_1_CNT                  0x00028020
#define PMC_I4_FEC_REG_CORRECTED_ONES_0_CNT                  0x00028024
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_3_CNT                0x00028028
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_2_CNT                0x0002802c
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_1_CNT                0x00028030
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_0_CNT                0x00028034
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_3_CNT                  0x00028038
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_2_CNT                  0x0002803c
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_1_CNT                  0x00028040
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_0_CNT                  0x00028044
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT( N ) (0x00028048 + (N) * 4)
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT( N ) (0x00028088 + (N) * 4)
#define PMC_I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT           0x000280c8
#define PMC_I4_FEC_REG_LPA_CNT_MODE                          0x000280d8
#define PMC_I4_FEC_REG_LLSA_CNT_MODE                         0x000280e8
#define PMC_I4_FEC_REG_LLSA_LATCH_SOURCE                     0x000280ec
#define PMC_I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT          0x0002810c
#define PMC_I4_FEC_REG_FEC_CTRL                              0x00028114

/*----------------------------------------.
 | Register 0x00028000 FEC_ENCODER_CONFIG |
 +----------------------------------------+
 | bit  0 R/W  ENCODER_DATA_TYPE          |
 +---------------------------------------*/
#define PMC_I4_FEC_REG_FEC_ENCODER_CONFIG_UNUSED_MASK           0xfffffffe
#define I4_FEC_REG_FEC_ENCODER_CONFIG_BIT_ENCODER_DATA_TYPE_MSK 0x00000001
#define I4_FEC_REG_FEC_ENCODER_CONFIG_BIT_ENCODER_DATA_TYPE_OFF 0

/*----------------------------------------.
 | Register 0x00028004 FEC_DECODER_CONFIG |
 +----------------------------------------+
 | bit  11:8 R/W  DECODER_LN_DISABLE      |
 | bit  5:4  R/W  DECODER_STATS_ENABLE    |
 | bit  1:0  R/W  DECODER_OP_MODE         |
 +---------------------------------------*/
#define PMC_I4_FEC_REG_FEC_DECODER_CONFIG_UNUSED_MASK              0xfffff0cc
#define I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_MSK   0x00000f00
#define I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF   8
#define I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_MSK 0x00000030
#define I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF 4
#define I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_MSK      0x00000003
#define I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF      0

/*--------------------------------------------.
 | Register 0x00028008 CORRECTED_ZEROES_3_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_3_COUNT      |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_3_CNT_UNUSED_MASK                  0x00000000
#define I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002800c CORRECTED_ZEROES_2_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_2_COUNT      |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_2_CNT_UNUSED_MASK                  0x00000000
#define I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x00028010 CORRECTED_ZEROES_1_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_1_COUNT      |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_1_CNT_UNUSED_MASK                  0x00000000
#define I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x00028014 CORRECTED_ZEROES_0_CNT |
 +--------------------------------------------+
 | bit  31:0 R  CORRECTED_ZEROES_0_COUNT      |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ZEROES_0_CNT_UNUSED_MASK                  0x00000000
#define I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x00028018 CORRECTED_ONES_3_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_3_COUNT      |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ONES_3_CNT_UNUSED_MASK                0x00000000
#define I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002801c CORRECTED_ONES_2_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_2_COUNT      |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ONES_2_CNT_UNUSED_MASK                0x00000000
#define I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x00028020 CORRECTED_ONES_1_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_1_COUNT      |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ONES_1_CNT_UNUSED_MASK                0x00000000
#define I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x00028024 CORRECTED_ONES_0_CNT |
 +------------------------------------------+
 | bit  31:0 R  CORRECTED_ONES_0_COUNT      |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_CORRECTED_ONES_0_CNT_UNUSED_MASK                0x00000000
#define I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_MSK 0xffffffff
#define I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x00028028 UNCORRECTED_ERRS_3_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_3_COUNT          |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_MSK 0xffffffff
#define I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x0002802c UNCORRECTED_ERRS_2_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_2_COUNT          |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_MSK 0xffffffff
#define I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x00028030 UNCORRECTED_ERRS_1_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_1_COUNT          |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_MSK 0xffffffff
#define I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x00028034 UNCORRECTED_ERRS_0_CNT |
 +--------------------------------------------+
 | bit  31:0 R  UNCORR_ERROR_0_COUNT          |
 +-------------------------------------------*/
#define PMC_I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_MSK 0xffffffff
#define I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x00028038 RX_FRAME_PULSE_3_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_3_COUNT        |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_3_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_MSK 0xffffffff
#define I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x0002803c RX_FRAME_PULSE_2_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_2_COUNT        |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_2_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_MSK 0xffffffff
#define I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x00028040 RX_FRAME_PULSE_1_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_1_COUNT        |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_1_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_MSK 0xffffffff
#define I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_OFF 0

/*------------------------------------------.
 | Register 0x00028044 RX_FRAME_PULSE_0_CNT |
 +------------------------------------------+
 | bit  31:0 R  RX_FRM_PULSE_0_COUNT        |
 +-----------------------------------------*/
#define PMC_I4_FEC_REG_RX_FRAME_PULSE_0_CNT_UNUSED_MASK              0x00000000
#define I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_MSK 0xffffffff
#define I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_OFF 0

/* index definitions for PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT */
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_INDEX_N_MIN                 0
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_INDEX_N_MAX                 15
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_INDEX_N_SIZE                16
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_INDEX_N_OFFSET              4

/*------------------------------------------------------------------.
 | Register (0x00028048 + (N) * 4) LLSA_STATISTICS_CORRECTED_0S_CNT |
 +------------------------------------------------------------------+
 | bit  31:0 R  LLSA_STATS_LANE_CORR_0S                             |
 +-----------------------------------------------------------------*/
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_UNUSED_MASK                 0x00000000
#define I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_BIT_LLSA_STATS_LANE_CORR_0S_MSK 0xffffffff
#define I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_BIT_LLSA_STATS_LANE_CORR_0S_OFF 0

/* index definitions for PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT */
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_INDEX_N_MIN                 0
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_INDEX_N_MAX                 15
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_INDEX_N_SIZE                16
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_INDEX_N_OFFSET              4

/*------------------------------------------------------------------.
 | Register (0x00028088 + (N) * 4) LLSA_STATISTICS_CORRECTED_1S_CNT |
 +------------------------------------------------------------------+
 | bit  31:0 R  LLSA_STATS_LANE_CORR_1S                             |
 +-----------------------------------------------------------------*/
#define PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_UNUSED_MASK                 0x00000000
#define I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_BIT_LLSA_STATS_LANE_CORR_1S_MSK 0xffffffff
#define I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_BIT_LLSA_STATS_LANE_CORR_1S_OFF 0

/*-------------------------------------------------.
 | Register 0x000280c8 LLSA_UNCORRECTED_ERRS_3_CNT |
 +-------------------------------------------------+
 | bit  31:0 R  LLSA_UNCORR_ERROR_3_COUNT          |
 +------------------------------------------------*/
#define PMC_I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_UNUSED_MASK                   0x00000000
#define I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_MSK 0xffffffff
#define I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_OFF 0

/*----------------------------------.
 | Register 0x000280d8 LPA_CNT_MODE |
 +----------------------------------+
 | bit  1:0 R/W  LPA_COUNTER_MODE   |
 +---------------------------------*/
#define PMC_I4_FEC_REG_LPA_CNT_MODE_UNUSED_MASK          0xfffffffc
#define I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_MSK 0x00000003
#define I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF 0

/*-----------------------------------.
 | Register 0x000280e8 LLSA_CNT_MODE |
 +-----------------------------------+
 | bit  1:0 R/W  LLSA_COUNTER_MODE   |
 +----------------------------------*/
#define PMC_I4_FEC_REG_LLSA_CNT_MODE_UNUSED_MASK           0xfffffffc
#define I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_MSK 0x00000003
#define I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF 0

/*---------------------------------------.
 | Register 0x000280ec LLSA_LATCH_SOURCE |
 +---------------------------------------+
 | bit  0 R/W  LLSA_LATCH_SOURCE         |
 +--------------------------------------*/
#define PMC_I4_FEC_REG_LLSA_LATCH_SOURCE_UNUSED_MASK           0xfffffffe
#define I4_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_MSK 0x00000001
#define I4_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_OFF 0

/*--------------------------------------------------.
 | Register 0x0002810c LLSA_STATISTICS_FAILURE_STAT |
 +--------------------------------------------------+
 | bit  3:0 R  LLSA_STATS_OTU_FAIL                  |
 +-------------------------------------------------*/
#define PMC_I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_UNUSED_MASK             0xfffffff0
#define I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_MSK 0x0000000f
#define I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_OFF 0

/*------------------------------.
 | Register 0x00028114 FEC_CTRL |
 +------------------------------+
 | bit  2 R/W  FEC_RAM_LOWPWR   |
 | bit  1 R/W  FEC_LOWPWR       |
 | bit  0 R/W  FEC_SW_RST       |
 +-----------------------------*/
#define PMC_I4_FEC_REG_FEC_CTRL_UNUSED_MASK        0xfffffff0
#define I4_FEC_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_MSK 0x00000004
#define I4_FEC_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_OFF 2
#define I4_FEC_REG_FEC_CTRL_BIT_FEC_LOWPWR_MSK     0x00000002
#define I4_FEC_REG_FEC_CTRL_BIT_FEC_LOWPWR_OFF     1
#define I4_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK     0x00000001
#define I4_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF     0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _I4_FEC_REGS_H */
