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
 *     srli_768 block
 *****************************************************************************/
#ifndef _SRLI_768_REGS_H
#define _SRLI_768_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SRLI_768_REG_PGM_CLK_CFG 0x00000080

/*---------------------------------.
 | Register 0x00000080 PGM_CLK_CFG |
 +---------------------------------+
 | bit  15:12 R/W  INTERLEAVEEN    |
 +--------------------------------*/
#define PMC_SRLI_768_REG_PGM_CLK_CFG_UNUSED_MASK      0xffff0f00
#define SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_MSK 0x0000f000
#define SRLI_768_REG_PGM_CLK_CFG_BIT_INTERLEAVEEN_OFF 12

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SRLI_768_REGS_H */
