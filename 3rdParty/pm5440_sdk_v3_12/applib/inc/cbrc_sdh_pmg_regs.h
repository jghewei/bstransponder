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
 *     cbrc_sdh_pmg block
 *****************************************************************************/
#ifndef _CBRC_SDH_PMG_REGS_H
#define _CBRC_SDH_PMG_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CBRC_SDH_PMG_REG_CFG         0x00002000
#define PMC_CBRC_SDH_PMG_REG_INT_SUMMARY 0x00002004
#define PMC_CBRC_SDH_PMG_REG_INT_PIN_EN  0x00002008

/*-----------------------------------.
 | Register 0x00002000 CFG           |
 +-----------------------------------+
 | bit  10  R/W  SDH_PMG_2_LOWPWR    |
 | bit  9   R/W  SDH_PMG_1_LOWPWR    |
 | bit  8   R/W  SDH_PMG_0_LOWPWR    |
 | bit  7   R/W  SDH_PMG_2_SW_RST    |
 | bit  6   R/W  SDH_PMG_1_SW_RST    |
 | bit  5   R/W  SDH_PMG_0_SW_RST    |
 | bit  4   R/W  CBRC_SDH_PMG_SW_RST |
 | bit  3:0 R/W  PRBS_MON_CH_SEL     |
 +----------------------------------*/
#define PMC_CBRC_SDH_PMG_REG_CFG_UNUSED_MASK             0xfffff800
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_LOWPWR_MSK    0x00000400
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_LOWPWR_OFF    10
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_LOWPWR_MSK    0x00000200
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_LOWPWR_OFF    9
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_LOWPWR_MSK    0x00000100
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_LOWPWR_OFF    8
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_SW_RST_MSK    0x00000080
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_2_SW_RST_OFF    7
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_SW_RST_MSK    0x00000040
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_1_SW_RST_OFF    6
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_SW_RST_MSK    0x00000020
#define CBRC_SDH_PMG_REG_CFG_BIT_SDH_PMG_0_SW_RST_OFF    5
#define CBRC_SDH_PMG_REG_CFG_BIT_CBRC_SDH_PMG_SW_RST_MSK 0x00000010
#define CBRC_SDH_PMG_REG_CFG_BIT_CBRC_SDH_PMG_SW_RST_OFF 4
#define CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_MSK     0x0000000f
#define CBRC_SDH_PMG_REG_CFG_BIT_PRBS_MON_CH_SEL_OFF     0

/*---------------------------------.
 | Register 0x00002004 INT_SUMMARY |
 +---------------------------------+
 | bit  2 R  SDH_PMG_2_INT         |
 | bit  1 R  SDH_PMG_1_INT         |
 | bit  0 R  SDH_PMG_0_INT         |
 +--------------------------------*/
#define PMC_CBRC_SDH_PMG_REG_INT_SUMMARY_UNUSED_MASK       0xfffffff8
#define CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_2_INT_MSK 0x00000004
#define CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_2_INT_OFF 2
#define CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_1_INT_MSK 0x00000002
#define CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_1_INT_OFF 1
#define CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_0_INT_MSK 0x00000001
#define CBRC_SDH_PMG_REG_INT_SUMMARY_BIT_SDH_PMG_0_INT_OFF 0

/*--------------------------------.
 | Register 0x00002008 INT_PIN_EN |
 +--------------------------------+
 | bit  11 R/W  SDH_PMG_2_S_E     |
 | bit  10 R/W  SDH_PMG_1_S_E     |
 | bit  9  R/W  SDH_PMG_0_S_E     |
 | bit  8  R/W  SDH_PMG_2_D_E     |
 | bit  7  R/W  SDH_PMG_1_D_E     |
 | bit  6  R/W  SDH_PMG_0_D_E     |
 | bit  5  R/W  SDH_PMG_2_M_E     |
 | bit  4  R/W  SDH_PMG_1_M_E     |
 | bit  3  R/W  SDH_PMG_0_M_E     |
 | bit  2  R/W  SDH_PMG_2_P_E     |
 | bit  1  R/W  SDH_PMG_1_P_E     |
 | bit  0  R/W  SDH_PMG_0_P_E     |
 +-------------------------------*/
#define PMC_CBRC_SDH_PMG_REG_INT_PIN_EN_UNUSED_MASK       0xfffff000
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_S_E_MSK 0x00000800
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_S_E_OFF 11
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_S_E_MSK 0x00000400
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_S_E_OFF 10
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_S_E_MSK 0x00000200
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_S_E_OFF 9
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_D_E_MSK 0x00000100
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_D_E_OFF 8
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_D_E_MSK 0x00000080
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_D_E_OFF 7
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_D_E_MSK 0x00000040
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_D_E_OFF 6
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_M_E_MSK 0x00000020
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_M_E_OFF 5
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_M_E_MSK 0x00000010
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_M_E_OFF 4
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_M_E_MSK 0x00000008
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_M_E_OFF 3
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_P_E_MSK 0x00000004
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_2_P_E_OFF 2
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_P_E_MSK 0x00000002
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_1_P_E_OFF 1
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_P_E_MSK 0x00000001
#define CBRC_SDH_PMG_REG_INT_PIN_EN_BIT_SDH_PMG_0_P_E_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_SDH_PMG_REGS_H */
