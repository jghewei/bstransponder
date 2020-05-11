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
 *     tgfpf96_isum block
 *****************************************************************************/
#ifndef _TGFPF96_ISUM_REGS_H
#define _TGFPF96_ISUM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TGFPF96_ISUM_REG_INT_SUMMARY_REG_0 0x00000000
#define PMC_TGFPF96_ISUM_REG_INT_SUMMARY_REG_1 0x00000004

/*---------------------------------------.
 | Register 0x00000000 INT_SUMMARY_REG_0 |
 +---------------------------------------+
 | bit  31:0 R  INT_SUMMARY_0            |
 +--------------------------------------*/
#define PMC_TGFPF96_ISUM_REG_INT_SUMMARY_REG_0_UNUSED_MASK       0x00000000
#define TGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_MSK 0xffffffff
#define TGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF 0

/*---------------------------------------.
 | Register 0x00000004 INT_SUMMARY_REG_1 |
 +---------------------------------------+
 | bit  26:0 R  INT_SUMMARY_1            |
 +--------------------------------------*/
#define PMC_TGFPF96_ISUM_REG_INT_SUMMARY_REG_1_UNUSED_MASK       0xf8000000
#define TGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_MSK 0x07ffffff
#define TGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TGFPF96_ISUM_REGS_H */
