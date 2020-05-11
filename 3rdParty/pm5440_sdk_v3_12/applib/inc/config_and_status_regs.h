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
 *     config_and_status block
 *****************************************************************************/
#ifndef _CONFIG_AND_STATUS_REGS_H
#define _CONFIG_AND_STATUS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG              0x00000000
#define PMC_CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR       0x00000001
#define PMC_CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR        0x00000002
#define PMC_CONFIG_AND_STATUS_REG_MON_ERR_CNT                     0x00000004
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG        0x00000008
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR 0x00000009
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR  0x0000000a
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL             0x0000000b

/*----------------------------------------.
 | Register 0x00000000 MON_STS_1_PATH_CFG |
 +----------------------------------------+
 | bit  3 W    RESYNC                     |
 | bit  2 R/W  MON_INV_PRBS               |
 | bit  0 R/W  MON_ENA                    |
 +---------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_UNUSED_MASK      0xfffff990
#define CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_RESYNC_MSK       0x00000008
#define CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_RESYNC_OFF       3
#define CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_MSK 0x00000004
#define CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_INV_PRBS_OFF 2
#define CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_MSK      0x00000001
#define CONFIG_AND_STATUS_REG_MON_STS_1_PATH_CFG_BIT_MON_ENA_OFF      0

/*-----------------------------------------------.
 | Register 0x00000001 MON_PRBS_22_7_ACCUMULATOR |
 +-----------------------------------------------+
 | bit  15:0 R/W  MON_PRBS                       |
 +----------------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_UNUSED_MASK  0xffff0000
#define CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_MSK 0x0000ffff
#define CONFIG_AND_STATUS_REG_MON_PRBS_22_7_ACCUMULATOR_BIT_MON_PRBS_OFF 0

/*----------------------------------------------.
 | Register 0x00000002 MON_PRBS_6_0_ACCUMULATOR |
 +----------------------------------------------+
 | bit  6:0 R/W  MON_PRBS                       |
 +---------------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_UNUSED_MASK  0xffffff80
#define CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_MSK 0x0000007f
#define CONFIG_AND_STATUS_REG_MON_PRBS_6_0_ACCUMULATOR_BIT_MON_PRBS_OFF 0

/*---------------------------------.
 | Register 0x00000004 MON_ERR_CNT |
 +---------------------------------+
 | bit  15:0 R  ERR_CNT            |
 +--------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_MON_ERR_CNT_UNUSED_MASK 0xffff0000
#define CONFIG_AND_STATUS_REG_MON_ERR_CNT_BIT_ERR_CNT_MSK 0x0000ffff
#define CONFIG_AND_STATUS_REG_MON_ERR_CNT_BIT_ERR_CNT_OFF 0

/*----------------------------------------------.
 | Register 0x00000008 GENERATOR_STS_1_PATH_CFG |
 +----------------------------------------------+
 | bit  12  R/W  LINKENA0                       |
 | bit  9:8 R/W  GEN_GPO                        |
 | bit  7:6 R/W  S                              |
 | bit  4   R/W  GEN_B1E1_ENA                   |
 | bit  3   W    FORCE_ERR                      |
 | bit  1   R/W  GEN_INV_PRBS                   |
 | bit  0   R/W  GEN_AMODE                      |
 +---------------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_UNUSED_MASK      0xffffcc04
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_MSK     0x00001000
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_LINKENA0_OFF     12
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_MSK      0x00000300
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_GPO_OFF      8
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_MSK            0x000000c0
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_S_OFF            6
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_MSK 0x00000010
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_B1E1_ENA_OFF 4
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_FORCE_ERR_MSK    0x00000008
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_FORCE_ERR_OFF    3
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_MSK 0x00000002
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_INV_PRBS_OFF 1
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_MSK    0x00000001
#define CONFIG_AND_STATUS_REG_GENERATOR_STS_1_PATH_CFG_BIT_GEN_AMODE_OFF    0

/*-----------------------------------------------------.
 | Register 0x00000009 GENERATOR_PRBS_22_7_ACCUMULATOR |
 +-----------------------------------------------------+
 | bit  15:0 R/W  GEN_PRBS                             |
 +----------------------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_UNUSED_MASK  0xffff0000
#define CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_MSK 0x0000ffff
#define CONFIG_AND_STATUS_REG_GENERATOR_PRBS_22_7_ACCUMULATOR_BIT_GEN_PRBS_OFF 0

/*----------------------------------------------------.
 | Register 0x0000000a GENERATOR_PRBS_6_0_ACCUMULATOR |
 +----------------------------------------------------+
 | bit  6:0 R/W  GEN_PRBS                             |
 +---------------------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_UNUSED_MASK  0xffffff80
#define CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_MSK 0x0000007f
#define CONFIG_AND_STATUS_REG_GENERATOR_PRBS_6_0_ACCUMULATOR_BIT_GEN_PRBS_OFF 0

/*-----------------------------------------.
 | Register 0x0000000b GENERATOR_B1_E1_VAL |
 +-----------------------------------------+
 | bit  7:0 R/W  GEN_B1                    |
 +----------------------------------------*/
#define PMC_CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_UNUSED_MASK 0xffffff00
#define CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_MSK  0x000000ff
#define CONFIG_AND_STATUS_REG_GENERATOR_B1_E1_VAL_BIT_GEN_B1_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CONFIG_AND_STATUS_REGS_H */
