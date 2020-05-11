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
 *     cbr_fsgm block
 *****************************************************************************/
#ifndef _CBR_FSGM_REGS_H
#define _CBR_FSGM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CBR_FSGM_REG_GLOBAL_CFG_REG                                  0x00000000
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG                               0x00000004
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL( channel )         (0x00000004 + (channel) * 0x0000000c)
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG                       0x00000008
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL( channel ) (0x00000008 + (channel) * 0x0000000c)
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG                       0x0000000c
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL( channel ) (0x0000000c + (channel) * 0x0000000c)
#define PMC_CBR_FSGM_REG_CHANNEL_1_CFG_REG                               0x00000010
#define PMC_CBR_FSGM_REG_CHANNEL_1_GAP_RATIO_N_REG                       0x00000014
#define PMC_CBR_FSGM_REG_CHANNEL_1_GAP_RATIO_D_REG                       0x00000018
#define PMC_CBR_FSGM_REG_CHANNEL_2_CFG_REG                               0x0000001c
#define PMC_CBR_FSGM_REG_CHANNEL_2_GAP_RATIO_N_REG                       0x00000020
#define PMC_CBR_FSGM_REG_CHANNEL_2_GAP_RATIO_D_REG                       0x00000024
#define PMC_CBR_FSGM_REG_CHANNEL_3_CFG_REG                               0x00000028
#define PMC_CBR_FSGM_REG_CHANNEL_3_GAP_RATIO_N_REG                       0x0000002c
#define PMC_CBR_FSGM_REG_CHANNEL_3_GAP_RATIO_D_REG                       0x00000030
#define PMC_CBR_FSGM_REG_INT_EN_REG                                      0x00000034
#define PMC_CBR_FSGM_REG_INT_INDICATION_REG                              0x00000038
#define PMC_CBR_FSGM_REG_INT_STAT_REG                                    0x0000003c
#define PMC_CBR_FSGM_REG_GENERAL_PURPOSE_I_O_REG                         0x00000040

/*------------------------------------.
 | Register 0x00000000 GLOBAL_CFG_REG |
 +------------------------------------+
 | bit  5 R/W  MS_AIS_RSOH_FILL       |
 | bit  4 R/W  MODE_40G               |
 | bit  0 R/W  SW_RESET               |
 +-----------------------------------*/
#define PMC_CBR_FSGM_REG_GLOBAL_CFG_REG_UNUSED_MASK          0xfffffece
#define CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MS_AIS_RSOH_FILL_MSK 0x00000020
#define CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MS_AIS_RSOH_FILL_OFF 5
#define CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MODE_40G_MSK         0x00000010
#define CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MODE_40G_OFF         4
#define CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_SW_RESET_MSK         0x00000001
#define CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_SW_RESET_OFF         0

/*---------------------------------------.
 | Register 0x00000004 CHANNEL_0_CFG_REG |
 +---------------------------------------+
 +--------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_UNUSED_MASK 0xfffffe00

/* index definitions for PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL */
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_INDEX_channel_MIN       0
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_INDEX_channel_MAX       3
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_INDEX_channel_SIZE      4
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_INDEX_channel_OFFSET    0x0000000c

/*-----------------------------------------------------------------------------.
 | Register (0x00000004 + (channel) * 0x0000000c) CHANNEL_0_CFG_REG_by_channel |
 +-----------------------------------------------------------------------------+
 | bit  6   R/W  CH_n_FAULT_INS_ENA                                            |
 | bit  1   R/W  CH_n_PN11_MON_ENA                                             |
 | bit  8   R/W  CH_n_FAULT_PROP_ENA                                           |
 | bit  4   R/W  CH_n_FORCE_FAULT                                              |
 | bit  0   R/W  CH_n_ENA                                                      |
 | bit  3:2 R/W  CH_n_PATT_MODE                                                |
 | bit  7   R/W  CH_n_LOS_PROP_ENA                                             |
 | bit  5   R/W  CH_n_LOS_INS_ENA                                              |
 +----------------------------------------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_UNUSED_MASK             0xfffffe00
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_INS_ENA_MSK  0x00000040
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_INS_ENA_OFF  6
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PN11_MON_ENA_MSK   0x00000002
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PN11_MON_ENA_OFF   1
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_PROP_ENA_MSK 0x00000100
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_PROP_ENA_OFF 8
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FORCE_FAULT_MSK    0x00000010
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FORCE_FAULT_OFF    4
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_ENA_MSK            0x00000001
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_ENA_OFF            0
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_MSK      0x0000000c
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF      2
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_PROP_ENA_MSK   0x00000080
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_PROP_ENA_OFF   7
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_INS_ENA_MSK    0x00000020
#define CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_INS_ENA_OFF    5

/*-----------------------------------------------.
 | Register 0x00000008 CHANNEL_0_GAP_RATIO_N_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_UNUSED_MASK 0x00000000

/* index definitions for PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL */
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_INDEX_channel_MIN    0
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_INDEX_channel_MAX    3
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_INDEX_channel_SIZE   4
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_INDEX_channel_OFFSET 0x0000000c

/*-------------------------------------------------------------------------------------.
 | Register (0x00000008 + (channel) * 0x0000000c) CHANNEL_0_GAP_RATIO_N_REG_by_channel |
 +-------------------------------------------------------------------------------------+
 | bit  31:0 R/W  CH_n_GAP_NUM                                                         |
 +------------------------------------------------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_UNUSED_MASK          0x00000000
#define CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_MSK     0xffffffff
#define CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF     0

/*-----------------------------------------------.
 | Register 0x0000000c CHANNEL_0_GAP_RATIO_D_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_UNUSED_MASK 0x00000000

/* index definitions for PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL */
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_INDEX_channel_MIN    0
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_INDEX_channel_MAX    3
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_INDEX_channel_SIZE   4
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_INDEX_channel_OFFSET 0x0000000c

/*-------------------------------------------------------------------------------------.
 | Register (0x0000000c + (channel) * 0x0000000c) CHANNEL_0_GAP_RATIO_D_REG_by_channel |
 +-------------------------------------------------------------------------------------+
 | bit  31:0 R/W  CH_n_GAP_DENOM                                                       |
 +------------------------------------------------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_UNUSED_MASK          0x00000000
#define CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_MSK   0xffffffff
#define CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF   0

/*---------------------------------------.
 | Register 0x00000010 CHANNEL_1_CFG_REG |
 +---------------------------------------+
 +--------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_1_CFG_REG_UNUSED_MASK 0xfffffe00

/*-----------------------------------------------.
 | Register 0x00000014 CHANNEL_1_GAP_RATIO_N_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_1_GAP_RATIO_N_REG_UNUSED_MASK 0x00000000

/*-----------------------------------------------.
 | Register 0x00000018 CHANNEL_1_GAP_RATIO_D_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_1_GAP_RATIO_D_REG_UNUSED_MASK 0x00000000

/*---------------------------------------.
 | Register 0x0000001c CHANNEL_2_CFG_REG |
 +---------------------------------------+
 +--------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_2_CFG_REG_UNUSED_MASK 0xfffffe00

/*-----------------------------------------------.
 | Register 0x00000020 CHANNEL_2_GAP_RATIO_N_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_2_GAP_RATIO_N_REG_UNUSED_MASK 0x00000000

/*-----------------------------------------------.
 | Register 0x00000024 CHANNEL_2_GAP_RATIO_D_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_2_GAP_RATIO_D_REG_UNUSED_MASK 0x00000000

/*---------------------------------------.
 | Register 0x00000028 CHANNEL_3_CFG_REG |
 +---------------------------------------+
 +--------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_3_CFG_REG_UNUSED_MASK 0xfffffe00

/*-----------------------------------------------.
 | Register 0x0000002c CHANNEL_3_GAP_RATIO_N_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_3_GAP_RATIO_N_REG_UNUSED_MASK 0x00000000

/*-----------------------------------------------.
 | Register 0x00000030 CHANNEL_3_GAP_RATIO_D_REG |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_CBR_FSGM_REG_CHANNEL_3_GAP_RATIO_D_REG_UNUSED_MASK 0x00000000

/*--------------------------------.
 | Register 0x00000034 INT_EN_REG |
 +--------------------------------+
 | bit  3 R/W  CH3_PN11_DET_E     |
 | bit  2 R/W  CH2_PN11_DET_E     |
 | bit  1 R/W  CH1_PN11_DET_E     |
 | bit  0 R/W  CH0_PN11_DET_E     |
 +-------------------------------*/
#define PMC_CBR_FSGM_REG_INT_EN_REG_UNUSED_MASK        0xfffffff0
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH3_PN11_DET_E_MSK 0x00000008
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH3_PN11_DET_E_OFF 3
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH2_PN11_DET_E_MSK 0x00000004
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH2_PN11_DET_E_OFF 2
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH1_PN11_DET_E_MSK 0x00000002
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH1_PN11_DET_E_OFF 1
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH0_PN11_DET_E_MSK 0x00000001
#define CBR_FSGM_REG_INT_EN_REG_BIT_CH0_PN11_DET_E_OFF 0

/*----------------------------------------.
 | Register 0x00000038 INT_INDICATION_REG |
 +----------------------------------------+
 | bit  3 R/W  CH3_PN11_DET_I             |
 | bit  2 R/W  CH2_PN11_DET_I             |
 | bit  1 R/W  CH1_PN11_DET_I             |
 | bit  0 R/W  CH0_PN11_DET_I             |
 +---------------------------------------*/
#define PMC_CBR_FSGM_REG_INT_INDICATION_REG_UNUSED_MASK        0xfffffff0
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_MSK 0x00000008
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_OFF 3
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_MSK 0x00000004
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_OFF 2
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_MSK 0x00000002
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_OFF 1
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_MSK 0x00000001
#define CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_OFF 0

/*----------------------------------.
 | Register 0x0000003c INT_STAT_REG |
 +----------------------------------+
 | bit  3 R  CH3_PN11_DET_V         |
 | bit  2 R  CH2_PN11_DET_V         |
 | bit  1 R  CH1_PN11_DET_V         |
 | bit  0 R  CH0_PN11_DET_V         |
 +---------------------------------*/
#define PMC_CBR_FSGM_REG_INT_STAT_REG_UNUSED_MASK        0xfffffff0
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH3_PN11_DET_V_MSK 0x00000008
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH3_PN11_DET_V_OFF 3
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH2_PN11_DET_V_MSK 0x00000004
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH2_PN11_DET_V_OFF 2
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH1_PN11_DET_V_MSK 0x00000002
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH1_PN11_DET_V_OFF 1
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH0_PN11_DET_V_MSK 0x00000001
#define CBR_FSGM_REG_INT_STAT_REG_BIT_CH0_PN11_DET_V_OFF 0

/*---------------------------------------------.
 | Register 0x00000040 GENERAL_PURPOSE_I_O_REG |
 +---------------------------------------------+
 +--------------------------------------------*/
#define PMC_CBR_FSGM_REG_GENERAL_PURPOSE_I_O_REG_UNUSED_MASK 0xffffff00

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBR_FSGM_REGS_H */
