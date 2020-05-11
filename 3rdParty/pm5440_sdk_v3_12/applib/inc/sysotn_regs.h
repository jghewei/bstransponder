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
 *     sysotn block
 *****************************************************************************/
#ifndef _SYSOTN_REGS_H
#define _SYSOTN_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SYSOTN_REG_MASTER_CONFIG   0x00000000
#define PMC_SYSOTN_REG_TOP_INT_EN      0x00000004
#define PMC_SYSOTN_REG_FEC_STAT_UPDATE 0x00000010

/*-----------------------------------.
 | Register 0x00000000 MASTER_CONFIG |
 +-----------------------------------+
 | bit  10 R/W  FEC_FAIL_DLOM_ENABLE |
 | bit  9  R/W  BYPASS_GFEC_TX       |
 | bit  8  R/W  BYPASS_GFEC_RX       |
 | bit  7  R/W  OTL_MODE             |
 | bit  6  R/W  OTN_OTU_LOWPWR       |
 | bit  5  R/W  SFIS_LOWPWR          |
 | bit  4  R/W  OPSM4_LOWPWR         |
 | bit  3  R/W  GFEC_234_LOWPWR      |
 | bit  2  R/W  PCBI_LCLK_MASK       |
 +----------------------------------*/
#define PMC_SYSOTN_REG_MASTER_CONFIG_UNUSED_MASK              0xfffff800
#define SYSOTN_REG_MASTER_CONFIG_BIT_FEC_FAIL_DLOM_ENABLE_MSK 0x00000400
#define SYSOTN_REG_MASTER_CONFIG_BIT_FEC_FAIL_DLOM_ENABLE_OFF 10
#define SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_TX_MSK       0x00000200
#define SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_TX_OFF       9
#define SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_RX_MSK       0x00000100
#define SYSOTN_REG_MASTER_CONFIG_BIT_BYPASS_GFEC_RX_OFF       8
#define SYSOTN_REG_MASTER_CONFIG_BIT_OTL_MODE_MSK             0x00000080
#define SYSOTN_REG_MASTER_CONFIG_BIT_OTL_MODE_OFF             7
#define SYSOTN_REG_MASTER_CONFIG_BIT_OTN_OTU_LOWPWR_MSK       0x00000040
#define SYSOTN_REG_MASTER_CONFIG_BIT_OTN_OTU_LOWPWR_OFF       6
#define SYSOTN_REG_MASTER_CONFIG_BIT_SFIS_LOWPWR_MSK          0x00000020
#define SYSOTN_REG_MASTER_CONFIG_BIT_SFIS_LOWPWR_OFF          5
#define SYSOTN_REG_MASTER_CONFIG_BIT_OPSM4_LOWPWR_MSK         0x00000010
#define SYSOTN_REG_MASTER_CONFIG_BIT_OPSM4_LOWPWR_OFF         4
#define SYSOTN_REG_MASTER_CONFIG_BIT_GFEC_234_LOWPWR_MSK      0x00000008
#define SYSOTN_REG_MASTER_CONFIG_BIT_GFEC_234_LOWPWR_OFF      3
#define SYSOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_MSK       0x00000004
#define SYSOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_OFF       2

/*--------------------------------.
 | Register 0x00000004 TOP_INT_EN |
 +--------------------------------+
 | bit  31:0 R/W  TOP_INT_E       |
 +-------------------------------*/
#define PMC_SYSOTN_REG_TOP_INT_EN_UNUSED_MASK   0x00000000
#define SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_MSK 0xffffffff
#define SYSOTN_REG_TOP_INT_EN_BIT_TOP_INT_E_OFF 0

/*-------------------------------------.
 | Register 0x00000010 FEC_STAT_UPDATE |
 +-------------------------------------+
 | bit  1 R/W  RS_FEC_STATS_UPDATE     |
 +------------------------------------*/
#define PMC_SYSOTN_REG_FEC_STAT_UPDATE_UNUSED_MASK             0xfffffffd
#define SYSOTN_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK 0x00000002
#define SYSOTN_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF 1

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYSOTN_REGS_H */
