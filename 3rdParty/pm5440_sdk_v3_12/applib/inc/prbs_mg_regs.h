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
 *     prbs_mg block
 *****************************************************************************/
#ifndef _PRBS_MG_REGS_H
#define _PRBS_MG_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PRBS_MG_REG_TOP_CFG_REG     0x00001000
#define PMC_PRBS_MG_REG_SW_RESET_REG_   0x00001004
#define PMC_PRBS_MG_REG_ASCLK_PULSE_REG 0x00001008
#define PMC_PRBS_MG_REG_LOW_POWER_CFG_  0x0000100c
#define PMC_PRBS_MG_REG_MTSB_INT_EN     0x00001010
#define PMC_PRBS_MG_REG_INT_SUMMARY     0x00001014
#define PMC_PRBS_MG_REG_TIP_SUMMARY     0x00001018

/*---------------------------------.
 | Register 0x00001000 TOP_CFG_REG |
 +---------------------------------+
 | bit  2 R/W  PRBS_MG_RX_CHNL_EN  |
 | bit  1 R/W  PRBS_MG_TX_CHNL_EN  |
 | bit  0 R/W  OC_768_MODE         |
 +--------------------------------*/
#define PMC_PRBS_MG_REG_TOP_CFG_REG_UNUSED_MASK            0xffff7f08
#define PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_RX_CHNL_EN_MSK 0x00000004
#define PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_RX_CHNL_EN_OFF 2
#define PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_TX_CHNL_EN_MSK 0x00000002
#define PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_TX_CHNL_EN_OFF 1
#define PRBS_MG_REG_TOP_CFG_REG_BIT_OC_768_MODE_MSK        0x00000001
#define PRBS_MG_REG_TOP_CFG_REG_BIT_OC_768_MODE_OFF        0

/*-----------------------------------.
 | Register 0x00001004 SW_RESET_REG_ |
 +-----------------------------------+
 | bit  1 R/W  PRBS_RX_SW_RST        |
 | bit  0 R/W  PRBS_TX_SW_RST        |
 +----------------------------------*/
#define PMC_PRBS_MG_REG_SW_RESET_REG_UNUSED_MASK        0xfffffffc
#define PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_RX_SW_RST_MSK 0x00000002
#define PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_RX_SW_RST_OFF 1
#define PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_TX_SW_RST_MSK 0x00000001
#define PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_TX_SW_RST_OFF 0

/*-------------------------------------.
 | Register 0x00001008 ASCLK_PULSE_REG |
 +-------------------------------------+
 | bit  0 R/W  ASCLK_PULSE             |
 +------------------------------------*/
#define PMC_PRBS_MG_REG_ASCLK_PULSE_REG_UNUSED_MASK     0xfffffffe
#define PRBS_MG_REG_ASCLK_PULSE_REG_BIT_ASCLK_PULSE_MSK 0x00000001
#define PRBS_MG_REG_ASCLK_PULSE_REG_BIT_ASCLK_PULSE_OFF 0

/*------------------------------------.
 | Register 0x0000100c LOW_POWER_CFG_ |
 +------------------------------------+
 | bit  1 R/W  PRBS_RX_LOW_PWR        |
 | bit  0 R/W  PRBS_TX_LOW_PWR        |
 +-----------------------------------*/
#define PMC_PRBS_MG_REG_LOW_POWER_CFG_UNUSED_MASK         0xfffffffc
#define PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_RX_LOW_PWR_MSK 0x00000002
#define PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_RX_LOW_PWR_OFF 1
#define PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_TX_LOW_PWR_MSK 0x00000001
#define PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_TX_LOW_PWR_OFF 0

/*---------------------------------.
 | Register 0x00001010 MTSB_INT_EN |
 +---------------------------------+
 | bit  7:4 R/W  SHPI_U_INT_EN     |
 | bit  3:0 R/W  PRGM_INT_EN       |
 +--------------------------------*/
#define PMC_PRBS_MG_REG_MTSB_INT_EN_UNUSED_MASK       0xffffff00
#define PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_MSK 0x000000f0
#define PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF 4
#define PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_MSK   0x0000000f
#define PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF   0

/*---------------------------------.
 | Register 0x00001014 INT_SUMMARY |
 +---------------------------------+
 | bit  7:4 R  SHPI_U_INT          |
 | bit  3:0 R  PRGM_INT            |
 +--------------------------------*/
#define PMC_PRBS_MG_REG_INT_SUMMARY_UNUSED_MASK    0xffffff00
#define PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_MSK 0x000000f0
#define PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_OFF 4
#define PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_MSK   0x0000000f
#define PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_OFF   0

/*---------------------------------.
 | Register 0x00001018 TIP_SUMMARY |
 +---------------------------------+
 | bit  7:4 R  SHPI_U_TIP          |
 | bit  3:0 R  PRGM_TIP            |
 +--------------------------------*/
#define PMC_PRBS_MG_REG_TIP_SUMMARY_UNUSED_MASK    0xffffff00
#define PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_MSK 0x000000f0
#define PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF 4
#define PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_MSK   0x0000000f
#define PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF   0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRBS_MG_REGS_H */
