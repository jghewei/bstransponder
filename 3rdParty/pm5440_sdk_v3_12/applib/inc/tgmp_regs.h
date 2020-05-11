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
 *     tgmp block
 *****************************************************************************/
#ifndef _TGMP_REGS_H
#define _TGMP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG( N )   (0x00000004 + (N) * 0x4)
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN( N )  (0x00000200 + (N) * 0x4)
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT( N ) (0x00000220 + (N) * 0x4)

/* index definitions for PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG */
#define PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG_INDEX_N_MIN    0
#define PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG_INDEX_N_MAX    95
#define PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG_INDEX_N_SIZE   96
#define PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------.
 | Register (0x00000004 + (N) * 0x4) CHANNEL_RESET_REG |
 +-----------------------------------------------------+
 | bit  0 R/W  CHAN_RSTB                               |
 +----------------------------------------------------*/
#define PMC_TGMP96_CORE_REG_CHANNEL_RESET_REG_UNUSED_MASK    0xfffffffe
#define TGMP96_CORE_REG_CHANNEL_RESET_REG_BIT_CHAN_RSTB_MSK  0x00000001
#define TGMP96_CORE_REG_CHANNEL_RESET_REG_BIT_CHAN_RSTB_OFF  0

/* index definitions for PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN */
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_INDEX_N_MIN           0
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_INDEX_N_MAX           2
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_INDEX_N_SIZE          3
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_INDEX_N_OFFSET        0x4

/*------------------------------------------------------.
 | Register (0x00000200 + (N) * 0x4) TXJC_RES_OVRFLW_EN |
 +------------------------------------------------------+
 | bit  31:0 R/W  TXJC_RES_OVRFLW_E                     |
 +-----------------------------------------------------*/
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_UNUSED_MASK           0x00000000
#define TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_BIT_TXJC_RES_OVRFLW_E_MSK 0xffffffff
#define TGMP96_CORE_REG_TXJC_RES_OVRFLW_EN_BIT_TXJC_RES_OVRFLW_E_OFF 0

/* index definitions for PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT */
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_INDEX_N_MIN           0
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_INDEX_N_MAX           2
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_INDEX_N_SIZE          3
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_INDEX_N_OFFSET        0x4

/*-------------------------------------------------------.
 | Register (0x00000220 + (N) * 0x4) TXJC_RES_OVRFLW_INT |
 +-------------------------------------------------------+
 | bit  31:0 R/W  TXJC_RES_OVRFLW_I                      |
 +------------------------------------------------------*/
#define PMC_TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_UNUSED_MASK           0x00000000
#define TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_BIT_TXJC_RES_OVRFLW_I_MSK 0xffffffff
#define TGMP96_CORE_REG_TXJC_RES_OVRFLW_INT_BIT_TXJC_RES_OVRFLW_I_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TGMP_REGS_H */
