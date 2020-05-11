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
 *     cbr_int block
 *****************************************************************************/
#ifndef _CBR_INT_REGS_H
#define _CBR_INT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG( N ) (0x00020000 + (N) * 4)
#define PMC_CBR_INT_REG_CHANNEL_UPDATE_REG                   0x0002000c
#define PMC_CBR_INT_REG_CHANNELN_CONFIG_REG( N )             (0x00020010 + (N) * 4)
#define PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG   0x00020040

/* index definitions for PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG */
#define PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_MIN            0
#define PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_MAX            2
#define PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_SIZE           3
#define PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_OFFSET         4

/*-----------------------------------------------------------------.
 | Register (0x00020000 + (N) * 4) MULTI_LANE_40G_GROUP_CONFIG_REG |
 +-----------------------------------------------------------------+
 | bit  0 R/W  MULTI_LANE_40G_SEL                                  |
 +----------------------------------------------------------------*/
#define PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_UNUSED_MASK            0xfffffffe
#define CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_MSK 0x00000001
#define CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_OFF 0

/*----------------------------------------.
 | Register 0x0002000c CHANNEL_UPDATE_REG |
 +----------------------------------------+
 | bit  0 R/W  CH_UPDATE                  |
 +---------------------------------------*/
#define PMC_CBR_INT_REG_CHANNEL_UPDATE_REG_UNUSED_MASK   0xfffffffe
#define CBR_INT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_MSK 0x00000001
#define CBR_INT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_OFF 0

/* index definitions for PMC_CBR_INT_REG_CHANNELN_CONFIG_REG */
#define PMC_CBR_INT_REG_CHANNELN_CONFIG_REG_INDEX_N_MIN    0
#define PMC_CBR_INT_REG_CHANNELN_CONFIG_REG_INDEX_N_MAX    11
#define PMC_CBR_INT_REG_CHANNELN_CONFIG_REG_INDEX_N_SIZE   12
#define PMC_CBR_INT_REG_CHANNELN_CONFIG_REG_INDEX_N_OFFSET 4

/*-----------------------------------------------------.
 | Register (0x00020010 + (N) * 4) CHANNELN_CONFIG_REG |
 +-----------------------------------------------------+
 | bit  7:4 R/W  CHN_ID                                |
 | bit  0   R/W  CHN_EN                                |
 +----------------------------------------------------*/
#define PMC_CBR_INT_REG_CHANNELN_CONFIG_REG_UNUSED_MASK    0xffffff0e
#define CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_MSK     0x000000f0
#define CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF     4
#define CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_MSK     0x00000001
#define CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_OFF     0

/*--------------------------------------------------------.
 | Register 0x00020040 CBR_INTERLEAVER_GENERAL_CONFIG_REG |
 +--------------------------------------------------------+
 | bit  0 R/W  CBR_INT_RESET                              |
 +-------------------------------------------------------*/
#define PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_UNUSED_MASK       0xfffffffe
#define CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_BIT_CBR_INT_RESET_MSK 0x00000001
#define CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_BIT_CBR_INT_RESET_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBR_INT_REGS_H */
