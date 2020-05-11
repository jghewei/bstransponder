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
 *     mfec block
 *****************************************************************************/
#ifndef _MFEC_REGS_H
#define _MFEC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MFEC_REG_MASTER_CONFIG        0x00004000
#define PMC_MFEC_REG_SWIZZLE100G_CONFIG   0x0000400c
#define PMC_MFEC_REG_DECODER_CONFIG       0x00004010
#define PMC_MFEC_REG_DECODER_BYPASS       0x00004014
#define PMC_MFEC_REG_ENCODER_CONFIG       0x00004018
#define PMC_MFEC_REG_ENCODER_BYPASS       0x0000401c
#define PMC_MFEC_REG_FEC_STAT_UPDATE      0x00004020
#define PMC_MFEC_REG_TOTAL_FEC_STATS( N ) (0x00004034 + (N) * 0x4)

/*-----------------------------------.
 | Register 0x00004000 MASTER_CONFIG |
 +-----------------------------------+
 | bit  1 R/W  MFEC_GLUE_LOWPWR      |
 | bit  0 R/W  RESET                 |
 +----------------------------------*/
#define PMC_MFEC_REG_MASTER_CONFIG_UNUSED_MASK          0xfffffffc
#define MFEC_REG_MASTER_CONFIG_BIT_MFEC_GLUE_LOWPWR_MSK 0x00000002
#define MFEC_REG_MASTER_CONFIG_BIT_MFEC_GLUE_LOWPWR_OFF 1
#define MFEC_REG_MASTER_CONFIG_BIT_RESET_MSK            0x00000001
#define MFEC_REG_MASTER_CONFIG_BIT_RESET_OFF            0

/*----------------------------------------.
 | Register 0x0000400c SWIZZLE100G_CONFIG |
 +----------------------------------------+
 | bit  5   R/W  SWIZZLE_ENC_REINIT       |
 | bit  4:2 R/W  SWIZZLE_SEL_100G_40G     |
 | bit  0   R/W  SWIZZLE100G_RAM_LOWPWR   |
 +---------------------------------------*/
#define PMC_MFEC_REG_SWIZZLE100G_CONFIG_UNUSED_MASK                0xffffffc0
#define MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_ENC_REINIT_MSK     0x00000020
#define MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_ENC_REINIT_OFF     5
#define MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_MSK   0x0000001c
#define MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF   2
#define MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE100G_RAM_LOWPWR_MSK 0x00000001
#define MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE100G_RAM_LOWPWR_OFF 0

/*--------------------------------------------------.
 | Register 0x00004010 DECODER_CONFIG               |
 +--------------------------------------------------+
 | bit  30:27 R/W  DECODER_40G_SLICE2_RSFEC_10G_SEL |
 | bit  26:23 R/W  DECODER_40G_SLICE1_RSFEC_10G_SEL |
 | bit  22:19 R/W  DECODER_40G_SLICE0_RSFEC_10G_SEL |
 | bit  18:14 R/W  DECODER_40G_SLICE2_FEC_SEL       |
 | bit  13:9  R/W  DECODER_40G_SLICE1_FEC_SEL       |
 | bit  8:4   R/W  DECODER_40G_SLICE0_FEC_SEL       |
 | bit  3:1   R/W  DECODER_100G_FEC_SEL             |
 | bit  0     R/W  DECODER_40G_100G_SEL             |
 +-------------------------------------------------*/
#define PMC_MFEC_REG_DECODER_CONFIG_UNUSED_MASK                          0x80000000
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_MSK 0x78000000
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF 27
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_MSK 0x07800000
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF 23
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_MSK 0x00780000
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF 19
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_MSK       0x0007c000
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF       14
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_MSK       0x00003e00
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF       9
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_MSK       0x000001f0
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF       4
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_MSK             0x0000000e
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF             1
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_100G_SEL_MSK             0x00000001
#define MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_100G_SEL_OFF             0

/*------------------------------------.
 | Register 0x00004014 DECODER_BYPASS |
 +------------------------------------+
 | bit  11:0 R/W  DECODER_BYPASS      |
 +-----------------------------------*/
#define PMC_MFEC_REG_DECODER_BYPASS_UNUSED_MASK        0xfffff000
#define MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_MSK 0x00000fff
#define MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF 0

/*--------------------------------------------------.
 | Register 0x00004018 ENCODER_CONFIG               |
 +--------------------------------------------------+
 | bit  30:27 R/W  ENCODER_40G_SLICE2_RSFEC_10G_SEL |
 | bit  26:23 R/W  ENCODER_40G_SLICE1_RSFEC_10G_SEL |
 | bit  22:19 R/W  ENCODER_40G_SLICE0_RSFEC_10G_SEL |
 | bit  18:14 R/W  ENCODER_40G_SLICE2_FEC_SEL       |
 | bit  13:9  R/W  ENCODER_40G_SLICE1_FEC_SEL       |
 | bit  8:4   R/W  ENCODER_40G_SLICE0_FEC_SEL       |
 | bit  3:1   R/W  ENCODER_100G_FEC_SEL             |
 | bit  0     R/W  ENCODER_40G_100G_SEL             |
 +-------------------------------------------------*/
#define PMC_MFEC_REG_ENCODER_CONFIG_UNUSED_MASK                          0x80000000
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_MSK 0x78000000
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF 27
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_MSK 0x07800000
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF 23
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_MSK 0x00780000
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF 19
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_MSK       0x0007c000
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF       14
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_MSK       0x00003e00
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF       9
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_MSK       0x000001f0
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF       4
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_MSK             0x0000000e
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF             1
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_100G_SEL_MSK             0x00000001
#define MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_100G_SEL_OFF             0

/*------------------------------------.
 | Register 0x0000401c ENCODER_BYPASS |
 +------------------------------------+
 | bit  11:0 R/W  ENCODER_BYPASS      |
 +-----------------------------------*/
#define PMC_MFEC_REG_ENCODER_BYPASS_UNUSED_MASK        0xfffff000
#define MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_MSK 0x00000fff
#define MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF 0

/*--------------------------------------.
 | Register 0x00004020 FEC_STAT_UPDATE  |
 +--------------------------------------+
 |                                      |
*/
/*
 |                                      |
 | bit  12:10 R/W  SWZ_FEC_STATS_UPDATE |
 | bit  9:7   R/W  RS_FEC_STATS_UPDATE  |
 | bit  6:4   R/W  I7_FEC_STATS_UPDATE  |
 | bit  3:1   R/W  I4_FEC_STATS_UPDATE  |
 | bit  0     R/W  ALL_FEC_STATS_UPDATE |
 |                                      |
*/
/*
 |                                      |
 +-------------------------------------*/
#define PMC_MFEC_REG_FEC_STAT_UPDATE_UNUSED_MASK              0xffff0000

#define MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_MSK 0x00001c00
#define MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF 10
#define MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK  0x00000380
#define MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF  7
#define MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_MSK  0x00000070
#define MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF  4
#define MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_MSK  0x0000000e
#define MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF  1
#define MFEC_REG_FEC_STAT_UPDATE_BIT_ALL_FEC_STATS_UPDATE_MSK 0x00000001
#define MFEC_REG_FEC_STAT_UPDATE_BIT_ALL_FEC_STATS_UPDATE_OFF 0
/* index definitions for PMC_MFEC_REG_TOTAL_FEC_STATS */
#define PMC_MFEC_REG_TOTAL_FEC_STATS_INDEX_N_MIN         0
#define PMC_MFEC_REG_TOTAL_FEC_STATS_INDEX_N_MAX         11
#define PMC_MFEC_REG_TOTAL_FEC_STATS_INDEX_N_SIZE        12
#define PMC_MFEC_REG_TOTAL_FEC_STATS_INDEX_N_OFFSET      0x4

/*---------------------------------------------------.
 | Register (0x00004034 + (N) * 0x4) TOTAL_FEC_STATS |
 +---------------------------------------------------+
 | bit  31:0 R/W  TOTAL_CORR_ERRS                    |
 +--------------------------------------------------*/
#define PMC_MFEC_REG_TOTAL_FEC_STATS_UNUSED_MASK         0x00000000
#define MFEC_REG_TOTAL_FEC_STATS_BIT_TOTAL_CORR_ERRS_MSK 0xffffffff
#define MFEC_REG_TOTAL_FEC_STATS_BIT_TOTAL_CORR_ERRS_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MFEC_REGS_H */
