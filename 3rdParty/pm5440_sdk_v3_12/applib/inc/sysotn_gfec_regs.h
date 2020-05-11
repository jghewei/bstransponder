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
 *     sysotn_gfec block
 *****************************************************************************/
#ifndef _SYSOTN_GFEC_REGS_H
#define _SYSOTN_GFEC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SYSOTN_GFEC_REG_RS_FEC_MODE                                                              0x00005000
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_MODE_CFG                                                  0x00005008
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11( N )       (0x00005020 + (N) * 4)
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11( N ) (0x00005050 + (N) * 4)
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11( N )          (0x00005080 + (N) * 4)
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11( N )          (0x000050b0 + (N) * 4)

/*------------------------------------.
 | Register 0x00005000 RS_FEC_MODE    |
 +------------------------------------+
 | bit  31:28 R/W  REG_TX_ENCODE_TYPE |
 | bit  27:24 R/W  REG_RX_DECODE_TYPE |
 | bit  23:12 R/W  REG_DISABLE_DEC    |
 | bit  7     R/W  REG_SHUT_DOWN      |
 | bit  6     R/W  REG_DEEP_SLEEP     |
 | bit  5     R/W  REG_DISABLE_UNCORR |
 | bit  4     R/W  REG_DISABLE_SCRAM  |
 | bit  3     R/W  FEC_SW_RST         |
 | bit  0     R/W  GFEC_LOWPWR        |
 +-----------------------------------*/
#define PMC_SYSOTN_GFEC_REG_RS_FEC_MODE_UNUSED_MASK            0x00000b06
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_MSK 0xf0000000
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_TX_ENCODE_TYPE_OFF 28
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_MSK 0x0f000000
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_RX_DECODE_TYPE_OFF 24
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_MSK    0x00fff000
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_DEC_OFF    12
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_SHUT_DOWN_MSK      0x00000080
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_SHUT_DOWN_OFF      7
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DEEP_SLEEP_MSK     0x00000040
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DEEP_SLEEP_OFF     6
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_UNCORR_MSK 0x00000020
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_UNCORR_OFF 5
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_SCRAM_MSK  0x00000010
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_REG_DISABLE_SCRAM_OFF  4
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_FEC_SW_RST_MSK         0x00000008
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_FEC_SW_RST_OFF         3
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_GFEC_LOWPWR_MSK        0x00000001
#define SYSOTN_GFEC_REG_RS_FEC_MODE_BIT_GFEC_LOWPWR_OFF        0

/*---------------------------------------------.
 | Register 0x00005008 LPA_STATISTICS_MODE_CFG |
 +---------------------------------------------+
 | bit  1:0 R/W  LPA_STATS_COUNTER_MODE        |
 +--------------------------------------------*/
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_MODE_CFG_UNUSED_MASK                0xfffffffc
#define SYSOTN_GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK 0x00000003
#define SYSOTN_GFEC_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF 0

/* index definitions for PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11 */
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MIN      0
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MAX      11
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_SIZE     12
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_OFFSET   4

/*-----------------------------------------------------------------------------------------------.
 | Register (0x00005020 + (N) * 4) LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 +-----------------------------------------------------------------------------------------------+
 | bit  31:0 R  LPA_STATS_FP                                                                     |
 +----------------------------------------------------------------------------------------------*/
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_UNUSED_MASK      0x00000000
#define SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_FP_MSK 0xffffffff
#define SYSOTN_GFEC_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_FP_OFF 0

/* index definitions for PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11 */
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MIN               0
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MAX               11
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_SIZE              12
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_OFFSET            4

/*-----------------------------------------------------------------------------------------------------.
 | Register (0x00005050 + (N) * 4) LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 +-----------------------------------------------------------------------------------------------------+
 | bit  31:0 R  LPA_STATS_UNCORR_CWDS                                                                  |
 +----------------------------------------------------------------------------------------------------*/
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_UNUSED_MASK               0x00000000
#define SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_UNCORR_CWDS_MSK 0xffffffff
#define SYSOTN_GFEC_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_UNCORR_CWDS_OFF 0

/* index definitions for PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11 */
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MIN           0
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MAX           11
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_SIZE          12
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_OFFSET        4

/*--------------------------------------------------------------------------------------------.
 | Register (0x00005080 + (N) * 4) LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 +--------------------------------------------------------------------------------------------+
 | bit  31:0 R  LPA_STATS_CORR_1S                                                             |
 +-------------------------------------------------------------------------------------------*/
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_UNUSED_MASK           0x00000000
#define SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_CORR_1S_MSK 0xffffffff
#define SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_1S_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_CORR_1S_OFF 0

/* index definitions for PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11 */
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MIN           0
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_MAX           11
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_SIZE          12
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_INDEX_N_OFFSET        4

/*--------------------------------------------------------------------------------------------.
 | Register (0x000050b0 + (N) * 4) LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11 |
 +--------------------------------------------------------------------------------------------+
 | bit  31:0 R  LPA_STATS_CORR_0S                                                             |
 +-------------------------------------------------------------------------------------------*/
#define PMC_SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_UNUSED_MASK           0x00000000
#define SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_CORR_0S_MSK 0xffffffff
#define SYSOTN_GFEC_REG_LPA_STATISTICS_CORRECTED_0S_CNT_FOR_PHYSICAL_LANES_0_TO_11_BIT_LPA_STATS_CORR_0S_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYSOTN_GFEC_REGS_H */
