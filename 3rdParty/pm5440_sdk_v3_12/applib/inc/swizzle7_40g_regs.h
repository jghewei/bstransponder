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
 *     swizzle7_40g block
 *****************************************************************************/
#ifndef _SWIZZLE7_40G_REGS_H
#define _SWIZZLE7_40G_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SWIZZLE7_40G_REG_DECODER_CFG                                             0x00020000
#define PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2                                    0x00020020
#define PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2                                    0x00020024
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_                        0x00020080
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_                        0x00020084
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_                        0x00020088
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_                        0x0002008c
#define PMC_SWIZZLE7_40G_REG_FEC_CTRL                                                0x000200f0
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG                                 0x00020180
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT                      0x00020190
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT                0x00020194
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT                         0x00020198
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT                         0x0002019c
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG                                0x000201a0
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT                            0x00020200
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT               0x0002021c
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19( N ) (0x00020240 + (N) * 4)
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19( N ) (0x00020300 + (N) * 4)

/*---------------------------------.
 | Register 0x00020000 DECODER_CFG |
 +---------------------------------+
 | bit  7:1 R/W  LIMIT_LATENCY     |
 | bit  0   R/W  DEC_DISABLE       |
 +--------------------------------*/
#define PMC_SWIZZLE7_40G_REG_DECODER_CFG_UNUSED_MASK       0xffffff00
#define SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_MSK 0x000000fe
#define SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF 1
#define SWIZZLE7_40G_REG_DECODER_CFG_BIT_DEC_DISABLE_MSK   0x00000001
#define SWIZZLE7_40G_REG_DECODER_CFG_BIT_DEC_DISABLE_OFF   0

/*------------------------------------------.
 | Register 0x00020020 DIAG_DECODER_CFG_1_2 |
 +------------------------------------------+
 | bit  16    R/W  FRAME_COMMIT_SEL         |
 | bit  15    R/W  LONG_TRIGGER_MODE        |
 | bit  14    R/W  TRAP_ENABLE              |
 | bit  13    R/W  TRAP_TYPE                |
 | bit  12    R/W  PDF_CAPTURE_ENABLE       |
 | bit  11:10 R/W  STAT_SEL                 |
 | bit  9     R/W  DISABLE_PRIORITY         |
 | bit  8:1   R/W  STATISTICS_FRAMES        |
 | bit  0     R/W  DEC_DISABLE_RESYNC       |
 +-----------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_UNUSED_MASK            0xfffe0000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_FRAME_COMMIT_SEL_MSK   0x00010000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_FRAME_COMMIT_SEL_OFF   16
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_LONG_TRIGGER_MODE_MSK  0x00008000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_LONG_TRIGGER_MODE_OFF  15
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_ENABLE_MSK        0x00004000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_ENABLE_OFF        14
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_TYPE_MSK          0x00002000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_TYPE_OFF          13
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_PDF_CAPTURE_ENABLE_MSK 0x00001000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_PDF_CAPTURE_ENABLE_OFF 12
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_MSK           0x00000c00
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF           10
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DISABLE_PRIORITY_MSK   0x00000200
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DISABLE_PRIORITY_OFF   9
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_MSK  0x000001fe
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF  1
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DEC_DISABLE_RESYNC_MSK 0x00000001
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DEC_DISABLE_RESYNC_OFF 0

/*------------------------------------------.
 | Register 0x00020024 DIAG_DECODER_CFG_2_2 |
 +------------------------------------------+
 | bit  15:0 R/W  REPROCESS_CONTROL         |
 +-----------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_UNUSED_MASK           0xffff0000
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_MSK 0x0000ffff
#define SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF 0

/*------------------------------------------------------.
 | Register 0x00020080 CONFIDENCE_FOR_T6_CODEWORDS_1_2_ |
 +------------------------------------------------------+
 | bit  30:24 R/W  CONF_T6_3                            |
 | bit  22:16 R/W  CONF_T6_2                            |
 | bit  14:8  R/W  CONF_T6_1                            |
 | bit  6:0   R/W  CONF_T6_0                            |
 +-----------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_UNUSED_MASK   0x80808080
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_MSK 0x7f000000
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF 24
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_MSK 0x007f0000
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF 16
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_MSK 0x00007f00
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF 8
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_MSK 0x0000007f
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF 0

/*------------------------------------------------------.
 | Register 0x00020084 CONFIDENCE_FOR_T6_CODEWORDS_2_2_ |
 +------------------------------------------------------+
 | bit  22:16 R/W  CONF_T6_6                            |
 | bit  14:8  R/W  CONF_T6_5                            |
 | bit  6:0   R/W  CONF_T6_4                            |
 +-----------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_UNUSED_MASK   0xff808080
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_MSK 0x007f0000
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF 16
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_MSK 0x00007f00
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF 8
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_MSK 0x0000007f
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF 0

/*------------------------------------------------------.
 | Register 0x00020088 CONFIDENCE_FOR_T5_CODEWORDS_1_2_ |
 +------------------------------------------------------+
 | bit  30:24 R/W  CONF_T5_3                            |
 | bit  22:16 R/W  CONF_T5_2                            |
 | bit  14:8  R/W  CONF_T5_1                            |
 | bit  6:0   R/W  CONF_T5_0                            |
 +-----------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_UNUSED_MASK   0x80808080
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_MSK 0x7f000000
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF 24
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_MSK 0x007f0000
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF 16
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_MSK 0x00007f00
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF 8
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_MSK 0x0000007f
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF 0

/*------------------------------------------------------.
 | Register 0x0002008c CONFIDENCE_FOR_T5_CODEWORDS_2_2_ |
 +------------------------------------------------------+
 | bit  14:8 R/W  CONF_T5_5                             |
 | bit  6:0  R/W  CONF_T5_4                             |
 +-----------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_UNUSED_MASK   0xffff8080
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_MSK 0x00007f00
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF 8
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_MSK 0x0000007f
#define SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF 0

/*------------------------------.
 | Register 0x000200f0 FEC_CTRL |
 +------------------------------+
 | bit  4 R/W  FEC_RAM_LOWPWR   |
 | bit  1 R/W  FEC_LOWPWR       |
 | bit  0 R/W  FEC_SW_RST       |
 +-----------------------------*/
#define PMC_SWIZZLE7_40G_REG_FEC_CTRL_UNUSED_MASK        0xffffefec
#define SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_MSK 0x00000010
#define SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_OFF 4
#define SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_LOWPWR_MSK     0x00000002
#define SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_LOWPWR_OFF     1
#define SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK     0x00000001
#define SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF     0

/*---------------------------------------------.
 | Register 0x00020180 LPA_STATISTICS_MODE_CFG |
 +---------------------------------------------+
 | bit  1:0 R/W  LPA_STATS_COUNTER_MODE        |
 +--------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_UNUSED_MASK                0xfffffffc
#define SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK 0x00000003
#define SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF 0

/*--------------------------------------------------------.
 | Register 0x00020190 LPA_STATISTICS_OTU_FRAME_PULSE_CNT |
 +--------------------------------------------------------+
 | bit  31:0 R  LPA_STATS_FP                              |
 +-------------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_UNUSED_MASK      0x00000000
#define SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_OFF 0

/*--------------------------------------------------------------.
 | Register 0x00020194 LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT |
 +--------------------------------------------------------------+
 | bit  31:0 R  LPA_STATS_UNCORR_CWDS                           |
 +-------------------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_UNUSED_MASK               0x00000000
#define SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_OFF 0

/*-----------------------------------------------------.
 | Register 0x00020198 LPA_STATISTICS_CORRECTED_1S_CNT |
 +-----------------------------------------------------+
 | bit  31:0 R  LPA_STATS_CORR_1S                      |
 +----------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_UNUSED_MASK           0x00000000
#define SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_OFF 0

/*-----------------------------------------------------.
 | Register 0x0002019c LPA_STATISTICS_CORRECTED_0S_CNT |
 +-----------------------------------------------------+
 | bit  31:0 R  LPA_STATS_CORR_0S                      |
 +----------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_UNUSED_MASK           0x00000000
#define SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_OFF 0

/*----------------------------------------------.
 | Register 0x000201a0 LLSA_STATISTICS_MODE_CFG |
 +----------------------------------------------+
 | bit  2   R/W  LLSA_STATS_LATCH_SOURCE        |
 | bit  1:0 R/W  LLSA_STATS_COUNTER_MODE        |
 +---------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_UNUSED_MASK                 0xfffffff8
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_LATCH_SOURCE_MSK 0x00000004
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_LATCH_SOURCE_OFF 2
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_MSK 0x00000003
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF 0

/*--------------------------------------------------.
 | Register 0x00020200 LLSA_STATISTICS_FAILURE_STAT |
 +--------------------------------------------------+
 | bit  0 R  LLSA_STATS_OTU_FAIL                    |
 +-------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT_UNUSED_MASK             0xfffffffe
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_MSK 0x00000001
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_OFF 0

/*---------------------------------------------------------------.
 | Register 0x0002021c LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT |
 +---------------------------------------------------------------+
 | bit  31:0 R  LLSA_STATS_UNCORR_CWDS                           |
 +--------------------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_UNUSED_MASK                0x00000000
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_OFF 0

/* index definitions for PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19 */
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_INDEX_N_MIN                 0
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_INDEX_N_MAX                 19
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_INDEX_N_SIZE                20
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_INDEX_N_OFFSET              4

/*------------------------------------------------------------------------------------.
 | Register (0x00020240 + (N) * 4) LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19 |
 +------------------------------------------------------------------------------------+
 | bit  31:0 R  LLSA_STATS_LANE_CORR_1S                                               |
 +-----------------------------------------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_UNUSED_MASK                 0x00000000
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_BIT_LLSA_STATS_LANE_CORR_1S_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_BIT_LLSA_STATS_LANE_CORR_1S_OFF 0

/* index definitions for PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19 */
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_INDEX_N_MIN                 0
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_INDEX_N_MAX                 19
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_INDEX_N_SIZE                20
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_INDEX_N_OFFSET              4

/*------------------------------------------------------------------------------------.
 | Register (0x00020300 + (N) * 4) LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19 |
 +------------------------------------------------------------------------------------+
 | bit  31:0 R  LLSA_STATS_LANE_CORR_0S                                               |
 +-----------------------------------------------------------------------------------*/
#define PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_UNUSED_MASK                 0x00000000
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_BIT_LLSA_STATS_LANE_CORR_0S_MSK 0xffffffff
#define SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_BIT_LLSA_STATS_LANE_CORR_0S_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SWIZZLE7_40G_REGS_H */
