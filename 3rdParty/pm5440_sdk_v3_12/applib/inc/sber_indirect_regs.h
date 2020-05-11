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
 *     sber_indirect block
 *****************************************************************************/
#ifndef _SBER_INDIRECT_REGS_H
#define _SBER_INDIRECT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT                  0x00000000
#define PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_EN                0x00000001
#define PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT              0x00000002
#define PMC_SBER_INDIRECT_REG_SD_BERM_BER_TH                   0x00000003
#define PMC_SBER_INDIRECT_REG_SD_BERM_SET_TH                   0x00000004
#define PMC_SBER_INDIRECT_REG_SD_BERM_CLEAR_TH                 0x00000005
#define PMC_SBER_INDIRECT_REG_SD_PATH_MONING_EN                0x00000006
#define PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2 0x00000007
#define PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1 0x00000008
#define PMC_SBER_INDIRECT_REG_SF_BERM_BER_TH                   0x00000013
#define PMC_SBER_INDIRECT_REG_SF_BERM_SET_TH                   0x00000014
#define PMC_SBER_INDIRECT_REG_SF_BERM_CLEAR_TH                 0x00000015
#define PMC_SBER_INDIRECT_REG_SF_PATH_MONING_EN                0x00000016
#define PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2 0x00000017
#define PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1 0x00000018

/*-------------------------------------.
 | Register 0x00000000 SD_SF_BERM_STAT |
 +-------------------------------------+
 | bit  5 R  SFPBS                     |
 | bit  4 R  SDPBS                     |
 | bit  3 R  SFCO                      |
 | bit  2 R  SDCO                      |
 | bit  1 R  SF_V                      |
 | bit  0 R  SD_V                      |
 +------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_SF_BERM_STAT_UNUSED_MASK 0xffffffc0
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFPBS_MSK   0x00000020
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFPBS_OFF   5
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDPBS_MSK   0x00000010
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDPBS_OFF   4
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFCO_MSK    0x00000008
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SFCO_OFF    3
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDCO_MSK    0x00000004
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SDCO_OFF    2
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SF_V_MSK    0x00000002
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SF_V_OFF    1
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SD_V_MSK    0x00000001
#define SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SD_V_OFF    0

/*---------------------------------------.
 | Register 0x00000001 SD_SF_BERM_INT_EN |
 +---------------------------------------+
 | bit  1 R/W  SF_E                      |
 | bit  0 R/W  SD_E                      |
 +--------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_UNUSED_MASK 0xfffffffc
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_MSK    0x00000002
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_OFF    1
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_MSK    0x00000001
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_OFF    0

/*-----------------------------------------.
 | Register 0x00000002 SD_SF_BERM_INT_STAT |
 +-----------------------------------------+
 | bit  1 R/W  SF_I                        |
 | bit  0 R/W  SD_I                        |
 +----------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_UNUSED_MASK 0xfffffffc
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK    0x00000002
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_OFF    1
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK    0x00000001
#define SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_OFF    0

/*------------------------------------.
 | Register 0x00000003 SD_BERM_BER_TH |
 +------------------------------------+
 | bit  3:0 R/W  SDTR                 |
 +-----------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_BERM_BER_TH_UNUSED_MASK 0xfffffff0
#define SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_MSK    0x0000000f
#define SBER_INDIRECT_REG_SD_BERM_BER_TH_BIT_SDTR_OFF    0

/*------------------------------------.
 | Register 0x00000004 SD_BERM_SET_TH |
 +------------------------------------+
 | bit  11:0 R/W  SDST                |
 +-----------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_BERM_SET_TH_UNUSED_MASK 0xfffff000
#define SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_MSK    0x00000fff
#define SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_OFF    0

/*--------------------------------------.
 | Register 0x00000005 SD_BERM_CLEAR_TH |
 +--------------------------------------+
 | bit  11:0 R/W  SDCT                  |
 +-------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_UNUSED_MASK 0xfffff000
#define SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_MSK    0x00000fff
#define SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_OFF    0

/*---------------------------------------.
 | Register 0x00000006 SD_PATH_MONING_EN |
 +---------------------------------------+
 | bit  0 R/W  SD_EN                     |
 +--------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_PATH_MONING_EN_UNUSED_MASK 0xfffffffe
#define SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK   0x00000001
#define SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_OFF   0

/*------------------------------------------------------.
 | Register 0x00000007 SD_BERM_SET_INTEGRATION_PERIOD_2 |
 +------------------------------------------------------+
 | bit  15:0 R/W  SDIP                                  |
 +-----------------------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_UNUSED_MASK 0xffff0000
#define SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_MSK    0x0000ffff
#define SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_OFF    0

/*------------------------------------------------------.
 | Register 0x00000008 SD_BERM_SET_INTEGRATION_PERIOD_1 |
 +------------------------------------------------------+
 | bit  15:0 R/W  SDIP                                  |
 +-----------------------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_UNUSED_MASK 0xffff0000
#define SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_MSK    0x0000ffff
#define SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_OFF    0

/*------------------------------------.
 | Register 0x00000013 SF_BERM_BER_TH |
 +------------------------------------+
 | bit  3:0 R/W  SFTR                 |
 +-----------------------------------*/
#define PMC_SBER_INDIRECT_REG_SF_BERM_BER_TH_UNUSED_MASK 0xfffffff0
#define SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_MSK    0x0000000f
#define SBER_INDIRECT_REG_SF_BERM_BER_TH_BIT_SFTR_OFF    0

/*------------------------------------.
 | Register 0x00000014 SF_BERM_SET_TH |
 +------------------------------------+
 | bit  11:0 R/W  SFST                |
 +-----------------------------------*/
#define PMC_SBER_INDIRECT_REG_SF_BERM_SET_TH_UNUSED_MASK 0xfffff000
#define SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_MSK    0x00000fff
#define SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_OFF    0

/*--------------------------------------.
 | Register 0x00000015 SF_BERM_CLEAR_TH |
 +--------------------------------------+
 | bit  11:0 R/W  SFCT                  |
 +-------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_UNUSED_MASK 0xfffff000
#define SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_MSK    0x00000fff
#define SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_OFF    0

/*---------------------------------------.
 | Register 0x00000016 SF_PATH_MONING_EN |
 +---------------------------------------+
 | bit  0 R/W  SF_EN                     |
 +--------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SF_PATH_MONING_EN_UNUSED_MASK 0xfffffffe
#define SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK   0x00000001
#define SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_OFF   0

/*------------------------------------------------------.
 | Register 0x00000017 SF_BERM_SET_INTEGRATION_PERIOD_2 |
 +------------------------------------------------------+
 | bit  15:0 R/W  SFIP                                  |
 +-----------------------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_UNUSED_MASK 0xffff0000
#define SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_MSK    0x0000ffff
#define SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_OFF    0

/*------------------------------------------------------.
 | Register 0x00000018 SF_BERM_SET_INTEGRATION_PERIOD_1 |
 +------------------------------------------------------+
 | bit  15:0 R/W  SFIP                                  |
 +-----------------------------------------------------*/
#define PMC_SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_UNUSED_MASK 0xffff0000
#define SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_MSK    0x0000ffff
#define SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_OFF    0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SBER_INDIRECT_REGS_H */
