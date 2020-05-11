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
 *     l1tpp_fege block
 *****************************************************************************/
#ifndef _L1TPP_FEGE_REGS_H
#define _L1TPP_FEGE_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_L1TPP_FEGE_REG_CFG                       0x00002040
#define PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG            0x00002044
#define PMC_L1TPP_FEGE_REG_CSF_ERR_CODE              0x00002048
#define PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION 0x0000204c
#define PMC_L1TPP_FEGE_REG_CTRL                      0x00002054

/*----------------------------.
 | Register 0x00002040 CFG    |
 +----------------------------+
 | bit  15  R    L1TPP_STATUS |
 | bit  8   R/W  RD_ALIGN_EN  |
 | bit  3:1 R/W  MODE         |
 | bit  0   R/W  L1TPP_EN     |
 +---------------------------*/
#define PMC_L1TPP_FEGE_REG_CFG_UNUSED_MASK      0xffff7ef0
#define L1TPP_FEGE_REG_CFG_BIT_L1TPP_STATUS_MSK 0x00008000
#define L1TPP_FEGE_REG_CFG_BIT_L1TPP_STATUS_OFF 15
#define L1TPP_FEGE_REG_CFG_BIT_RD_ALIGN_EN_MSK  0x00000100
#define L1TPP_FEGE_REG_CFG_BIT_RD_ALIGN_EN_OFF  8
#define L1TPP_FEGE_REG_CFG_BIT_MODE_MSK         0x0000000e
#define L1TPP_FEGE_REG_CFG_BIT_MODE_OFF         1
#define L1TPP_FEGE_REG_CFG_BIT_L1TPP_EN_MSK     0x00000001
#define L1TPP_FEGE_REG_CFG_BIT_L1TPP_EN_OFF     0

/*------------------------------------.
 | Register 0x00002044 TENB_ERR_RDNEG |
 +------------------------------------+
 | bit  9:0 R/W  TENB_ERR_RDNEG       |
 +-----------------------------------*/
#define PMC_L1TPP_FEGE_REG_TENB_ERR_RDNEG_UNUSED_MASK        0xfffffc00
#define L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_MSK 0x000003ff
#define L1TPP_FEGE_REG_TENB_ERR_RDNEG_BIT_TENB_ERR_RDNEG_OFF 0

/*----------------------------------.
 | Register 0x00002048 CSF_ERR_CODE |
 +----------------------------------+
 | bit  8:0 R/W  CSF_ERR_CODE       |
 +---------------------------------*/
#define PMC_L1TPP_FEGE_REG_CSF_ERR_CODE_UNUSED_MASK      0xfffffe00
#define L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_MSK 0x000001ff
#define L1TPP_FEGE_REG_CSF_ERR_CODE_BIT_CSF_ERR_CODE_OFF 0

/*-----------------------------------------------.
 | Register 0x0000204c JITTER_PATTERN_GENERATION |
 +-----------------------------------------------+
 | bit  14:12 R/W  PTRN_SEL                      |
 | bit  8:0   R/W  CUS_PTRN                      |
 +----------------------------------------------*/
#define PMC_L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_UNUSED_MASK  0xffff8e00
#define L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_MSK 0x00007000
#define L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_PTRN_SEL_OFF 12
#define L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_MSK 0x000001ff
#define L1TPP_FEGE_REG_JITTER_PATTERN_GENERATION_BIT_CUS_PTRN_OFF 0

/*--------------------------.
 | Register 0x00002054 CTRL |
 +--------------------------+
 | bit  4 R/W  FEF_GEN_EN   |
 | bit  0 R    INVLD_FRAME  |
 +-------------------------*/
#define PMC_L1TPP_FEGE_REG_CTRL_UNUSED_MASK     0xffffffee
#define L1TPP_FEGE_REG_CTRL_BIT_FEF_GEN_EN_MSK  0x00000010
#define L1TPP_FEGE_REG_CTRL_BIT_FEF_GEN_EN_OFF  4
#define L1TPP_FEGE_REG_CTRL_BIT_INVLD_FRAME_MSK 0x00000001
#define L1TPP_FEGE_REG_CTRL_BIT_INVLD_FRAME_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _L1TPP_FEGE_REGS_H */
