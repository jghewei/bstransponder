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
 *     lineotn block
 *****************************************************************************/
#ifndef _LINEOTN_REGS_H
#define _LINEOTN_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_LINEOTN_REG_MASTER_CONFIG 0x00000000
#define PMC_LINEOTN_REG_INT0_EN       0x00000008
#define PMC_LINEOTN_REG_INT0_SUMMARY  0x0000000c
#define PMC_LINEOTN_REG_INT1_EN       0x00000010
#define PMC_LINEOTN_REG_INT1_SUMMARY  0x00000014
#define PMC_LINEOTN_REG_INT2_EN       0x00000018
#define PMC_LINEOTN_REG_INT2_SUMMARY  0x0000001c
#define PMC_LINEOTN_REG_INT3_EN       0x00000020
#define PMC_LINEOTN_REG_INT3_SUMMARY  0x00000024
#define PMC_LINEOTN_REG_TIP_STAT      0x00000028

/*-----------------------------------.
 | Register 0x00000000 MASTER_CONFIG |
 +-----------------------------------+
 | bit  2 R/W  PCBI_LCLK_MASK        |
 +----------------------------------*/
#define PMC_LINEOTN_REG_MASTER_CONFIG_UNUSED_MASK        0xfffffffb
#define LINEOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_MSK 0x00000004
#define LINEOTN_REG_MASTER_CONFIG_BIT_PCBI_LCLK_MASK_OFF 2

/*-----------------------------.
 | Register 0x00000008 INT0_EN |
 +-----------------------------+
 | bit  16:0 R/W  INT0_E       |
 +----------------------------*/
#define PMC_LINEOTN_REG_INT0_EN_UNUSED_MASK 0xfffe0000
#define LINEOTN_REG_INT0_EN_BIT_INT0_E_MSK  0x0001ffff
#define LINEOTN_REG_INT0_EN_BIT_INT0_E_OFF  0

/*----------------------------------.
 | Register 0x0000000c INT0_SUMMARY |
 +----------------------------------+
 | bit  16:0 R  INT0_SUMMARY        |
 +---------------------------------*/
#define PMC_LINEOTN_REG_INT0_SUMMARY_UNUSED_MASK      0xfffe0000
#define LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_MSK 0x0001ffff
#define LINEOTN_REG_INT0_SUMMARY_BIT_INT0_SUMMARY_OFF 0

/*-----------------------------.
 | Register 0x00000010 INT1_EN |
 +-----------------------------+
 | bit  16:0 R/W  INT1_E       |
 +----------------------------*/
#define PMC_LINEOTN_REG_INT1_EN_UNUSED_MASK 0xfffe0000
#define LINEOTN_REG_INT1_EN_BIT_INT1_E_MSK  0x0001ffff
#define LINEOTN_REG_INT1_EN_BIT_INT1_E_OFF  0

/*----------------------------------.
 | Register 0x00000014 INT1_SUMMARY |
 +----------------------------------+
 | bit  16:0 R  INT1_SUMMARY        |
 +---------------------------------*/
#define PMC_LINEOTN_REG_INT1_SUMMARY_UNUSED_MASK      0xfffe0000
#define LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_MSK 0x0001ffff
#define LINEOTN_REG_INT1_SUMMARY_BIT_INT1_SUMMARY_OFF 0

/*-----------------------------.
 | Register 0x00000018 INT2_EN |
 +-----------------------------+
 | bit  16:0 R/W  INT2_E       |
 +----------------------------*/
#define PMC_LINEOTN_REG_INT2_EN_UNUSED_MASK 0xfffe0000
#define LINEOTN_REG_INT2_EN_BIT_INT2_E_MSK  0x0001ffff
#define LINEOTN_REG_INT2_EN_BIT_INT2_E_OFF  0

/*----------------------------------.
 | Register 0x0000001c INT2_SUMMARY |
 +----------------------------------+
 | bit  16:0 R  INT2_SUMMARY        |
 +---------------------------------*/
#define PMC_LINEOTN_REG_INT2_SUMMARY_UNUSED_MASK      0xfffe0000
#define LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_MSK 0x0001ffff
#define LINEOTN_REG_INT2_SUMMARY_BIT_INT2_SUMMARY_OFF 0

/*-----------------------------.
 | Register 0x00000020 INT3_EN |
 +-----------------------------+
 | bit  16:0 R/W  INT3_E       |
 +----------------------------*/
#define PMC_LINEOTN_REG_INT3_EN_UNUSED_MASK 0xfffe0000
#define LINEOTN_REG_INT3_EN_BIT_INT3_E_MSK  0x0001ffff
#define LINEOTN_REG_INT3_EN_BIT_INT3_E_OFF  0

/*----------------------------------.
 | Register 0x00000024 INT3_SUMMARY |
 +----------------------------------+
 | bit  16:0 R  INT3_SUMMARY        |
 +---------------------------------*/
#define PMC_LINEOTN_REG_INT3_SUMMARY_UNUSED_MASK      0xfffe0000
#define LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_MSK 0x0001ffff
#define LINEOTN_REG_INT3_SUMMARY_BIT_INT3_SUMMARY_OFF 0

/*------------------------------.
 | Register 0x00000028 TIP_STAT |
 +------------------------------+
 | bit  2 R  OTN_OTU_TIP        |
 | bit  1 R  MFEC_TIP           |
 +-----------------------------*/
#define PMC_LINEOTN_REG_TIP_STAT_UNUSED_MASK     0xfffffff8
#define LINEOTN_REG_TIP_STAT_BIT_OTN_OTU_TIP_MSK 0x00000004
#define LINEOTN_REG_TIP_STAT_BIT_OTN_OTU_TIP_OFF 2
#define LINEOTN_REG_TIP_STAT_BIT_MFEC_TIP_MSK    0x00000002
#define LINEOTN_REG_TIP_STAT_BIT_MFEC_TIP_OFF    1

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINEOTN_REGS_H */
