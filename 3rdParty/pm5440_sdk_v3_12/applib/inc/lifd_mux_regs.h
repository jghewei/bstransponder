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
 *     lifd_mux block
 *****************************************************************************/
#ifndef _LIFD_MUX_REGS_H
#define _LIFD_MUX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_LIFD_MUX_REG_SFI_IF                                0x00150148
#define PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL                    0x0015014c
#define PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT                   0x00150150
#define PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN                       0x00150154
#define PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT               0x00150158
#define PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN                   0x0015015c
#define PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 0x00150160

/*-----------------------------.
 | Register 0x00150148 SFI_IF  |
 +-----------------------------+
 | bit  12 R/W  SFI2_RXS_INV   |
 | bit  11 R/W  SFI2_RXS_EN    |
 | bit  10 R/W  SFI1_RXS_INV   |
 | bit  9  R/W  SFI1_RXS_EN    |
 | bit  8  R/W  SFI_IF_SWAP_EN |
 | bit  7  R/W  SFI2_TX_IFSEL  |
 | bit  6  R/W  SFI1_TX_IFSEL  |
 | bit  5  R/W  SFI2_EN        |
 | bit  4  R/W  SFI1_EN        |
 | bit  3  R/W  SFI2_OTN_EN    |
 | bit  2  R/W  SFI1_OTN_EN    |
 | bit  1  R/W  SFI2_CBRC_EN   |
 | bit  0  R/W  SFI1_CBRC_EN   |
 +----------------------------*/
#define PMC_LIFD_MUX_REG_SFI_IF_UNUSED_MASK        0xffffe000
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_INV_MSK   0x00001000
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_INV_OFF   12
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_EN_MSK    0x00000800
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_RXS_EN_OFF    11
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_INV_MSK   0x00000400
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_INV_OFF   10
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_EN_MSK    0x00000200
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_RXS_EN_OFF    9
#define LIFD_MUX_REG_SFI_IF_BIT_SFI_IF_SWAP_EN_MSK 0x00000100
#define LIFD_MUX_REG_SFI_IF_BIT_SFI_IF_SWAP_EN_OFF 8
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_TX_IFSEL_MSK  0x00000080
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_TX_IFSEL_OFF  7
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_TX_IFSEL_MSK  0x00000040
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_TX_IFSEL_OFF  6
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_EN_MSK        0x00000020
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_EN_OFF        5
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_EN_MSK        0x00000010
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_EN_OFF        4
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_OTN_EN_MSK    0x00000008
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_OTN_EN_OFF    3
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_OTN_EN_MSK    0x00000004
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_OTN_EN_OFF    2
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_CBRC_EN_MSK   0x00000002
#define LIFD_MUX_REG_SFI_IF_BIT_SFI2_CBRC_EN_OFF   1
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_CBRC_EN_MSK   0x00000001
#define LIFD_MUX_REG_SFI_IF_BIT_SFI1_CBRC_EN_OFF   0

/*----------------------------------------.
 | Register 0x0015014c SFI_C_CKCTL_SS_SEL |
 +----------------------------------------+
 | bit  3:2 R/W  SFI2_ACB_IF_SEL          |
 | bit  1:0 R/W  SFI1_ACB_IF_SEL          |
 +---------------------------------------*/
#define PMC_LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_UNUSED_MASK         0xfffffff0
#define LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_MSK 0x0000000c
#define LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI2_ACB_IF_SEL_OFF 2
#define LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_MSK 0x00000003
#define LIFD_MUX_REG_SFI_C_CKCTL_SS_SEL_BIT_SFI1_ACB_IF_SEL_OFF 0

/*-----------------------------------------.
 | Register 0x00150150 PIN_LOS_0_19_INVERT |
 +-----------------------------------------+
 | bit  19:0 R/W  PIN_LOS_INV              |
 +----------------------------------------*/
#define PMC_LIFD_MUX_REG_PIN_LOS_0_19_INVERT_UNUSED_MASK     0xfff00000
#define LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_MSK 0x000fffff
#define LIFD_MUX_REG_PIN_LOS_0_19_INVERT_BIT_PIN_LOS_INV_OFF 0

/*-------------------------------------.
 | Register 0x00150154 PIN_LOS_0_19_EN |
 +-------------------------------------+
 | bit  19:0 R/W  PIN_LOS_EN           |
 +------------------------------------*/
#define PMC_LIFD_MUX_REG_PIN_LOS_0_19_EN_UNUSED_MASK    0xfff00000
#define LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_MSK 0x000fffff
#define LIFD_MUX_REG_PIN_LOS_0_19_EN_BIT_PIN_LOS_EN_OFF 0

/*---------------------------------------------.
 | Register 0x00150158 SERDES_0_19_DLOS_INVERT |
 +---------------------------------------------+
 | bit  19:0 R/W  DLOS_INV                     |
 +--------------------------------------------*/
#define PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_UNUSED_MASK  0xfff00000
#define LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_MSK 0x000fffff
#define LIFD_MUX_REG_SERDES_0_19_DLOS_INVERT_BIT_DLOS_INV_OFF 0

/*-----------------------------------------.
 | Register 0x0015015c SERDES_0_19_DLOS_EN |
 +-----------------------------------------+
 | bit  19:0 R/W  DLOS_EN                  |
 +----------------------------------------*/
#define PMC_LIFD_MUX_REG_SERDES_0_19_DLOS_EN_UNUSED_MASK 0xfff00000
#define LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_MSK 0x000fffff
#define LIFD_MUX_REG_SERDES_0_19_DLOS_EN_BIT_DLOS_EN_OFF 0

/*-----------------------------------------------------------.
 | Register 0x00150160 LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 |
 +-----------------------------------------------------------+
 | bit  19:0 R/W  LOS_POL_INV                                |
 +----------------------------------------------------------*/
#define PMC_LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_UNUSED_MASK     0xfff00000
#define LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_MSK 0x000fffff
#define LIFD_MUX_REG_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_BIT_LOS_POL_INV_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LIFD_MUX_REGS_H */
