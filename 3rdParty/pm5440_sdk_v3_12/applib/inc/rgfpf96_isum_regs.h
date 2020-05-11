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
 *     rgfpf96_isum block
 *****************************************************************************/
#ifndef _RGFPF96_ISUM_REGS_H
#define _RGFPF96_ISUM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0 0x00000000
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1 0x00000004
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2 0x00000008
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3 0x0000000c
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4 0x00000010

/*---------------------------------------.
 | Register 0x00000000 INT_SUMMARY_REG_0 |
 +---------------------------------------+
 | bit  31:0 R  INT_SUMMARY_0            |
 +--------------------------------------*/
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_UNUSED_MASK       0x00000000
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_MSK 0xffffffff
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_0_BIT_INT_SUMMARY_0_OFF 0

/*---------------------------------------.
 | Register 0x00000004 INT_SUMMARY_REG_1 |
 +---------------------------------------+
 | bit  31:0 R  INT_SUMMARY_1            |
 +--------------------------------------*/
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_UNUSED_MASK       0x00000000
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_MSK 0xffffffff
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_1_BIT_INT_SUMMARY_1_OFF 0

/*---------------------------------------.
 | Register 0x00000008 INT_SUMMARY_REG_2 |
 +---------------------------------------+
 | bit  31:0 R  INT_SUMMARY_2            |
 +--------------------------------------*/
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_UNUSED_MASK       0x00000000
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_MSK 0xffffffff
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_2_BIT_INT_SUMMARY_2_OFF 0

/*---------------------------------------.
 | Register 0x0000000c INT_SUMMARY_REG_3 |
 +---------------------------------------+
 | bit  31:0 R  INT_SUMMARY_3            |
 +--------------------------------------*/
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_UNUSED_MASK       0x00000000
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_MSK 0xffffffff
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_3_BIT_INT_SUMMARY_3_OFF 0

/*---------------------------------------.
 | Register 0x00000010 INT_SUMMARY_REG_4 |
 +---------------------------------------+
 | bit  14:0 R  INT_SUMMARY_4            |
 +--------------------------------------*/
#define PMC_RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_UNUSED_MASK       0xffff8000
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_MSK 0x00007fff
#define RGFPF96_ISUM_REG_INT_SUMMARY_REG_4_BIT_INT_SUMMARY_4_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RGFPF96_ISUM_REGS_H */
