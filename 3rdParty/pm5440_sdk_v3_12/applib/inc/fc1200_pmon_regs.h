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
 *     fc1200_pmon block
 *****************************************************************************/
#ifndef _FC1200_PMON_REGS_H
#define _FC1200_PMON_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC1200_PMON_REG_CFG_REG                  0x00000000
#define PMC_FC1200_PMON_REG_PMON_CNT_STAT            0x00000004
#define PMC_FC1200_PMON_REG_DLOLB_EN_REG             0x00000014
#define PMC_FC1200_PMON_REG_PERF_MON_STAT_REG        0x00000018
#define PMC_FC1200_PMON_REG_VALID_FRAME_CNT_REG      0x0000001c
#define PMC_FC1200_PMON_REG_INVALID_FRAME_CNT_REG    0x00000020
#define PMC_FC1200_PMON_REG_VALID_FRAME_BYTE_CNT_REG 0x00000024
#define PMC_FC1200_PMON_REG_INT_EN_REG               0x00000028
#define PMC_FC1200_PMON_REG_INT_INDICATION_REG       0x0000002c
#define PMC_FC1200_PMON_REG_INT_STAT                 0x00000030

/*-------------------------------------.
 | Register 0x00000000 CFG_REG         |
 +-------------------------------------+
 | bit  2:1 R/W  INVALID_BLK_TYPE_MODE |
 | bit  0   R/W  SW_RESET              |
 +------------------------------------*/
#define PMC_FC1200_PMON_REG_CFG_REG_UNUSED_MASK               0xffffff88
#define FC1200_PMON_REG_CFG_REG_BIT_INVALID_BLK_TYPE_MODE_MSK 0x00000006
#define FC1200_PMON_REG_CFG_REG_BIT_INVALID_BLK_TYPE_MODE_OFF 1
#define FC1200_PMON_REG_CFG_REG_BIT_SW_RESET_MSK              0x00000001
#define FC1200_PMON_REG_CFG_REG_BIT_SW_RESET_OFF              0

/*-----------------------------------.
 | Register 0x00000004 PMON_CNT_STAT |
 +-----------------------------------+
 | bit  8 R    TIP                   |
 | bit  0 R/W  PMON_UPDATE           |
 +----------------------------------*/
#define PMC_FC1200_PMON_REG_PMON_CNT_STAT_UNUSED_MASK     0xfffffefe
#define FC1200_PMON_REG_PMON_CNT_STAT_BIT_TIP_MSK         0x00000100
#define FC1200_PMON_REG_PMON_CNT_STAT_BIT_TIP_OFF         8
#define FC1200_PMON_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK 0x00000001
#define FC1200_PMON_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF 0

/*----------------------------------.
 | Register 0x00000014 DLOLB_EN_REG |
 +----------------------------------+
 | bit  4 R/W  FORCE_DLOLB          |
 | bit  2 R/W  LOS_DLOLB_ENA        |
 | bit  1 R/W  LOSS_SYNC_DLOLB_ENA  |
 | bit  0 R/W  HIGH_BER_DLOLB_ENA   |
 +---------------------------------*/
#define PMC_FC1200_PMON_REG_DLOLB_EN_REG_UNUSED_MASK             0xffffffe0
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_FORCE_DLOLB_MSK         0x00000010
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_FORCE_DLOLB_OFF         4
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_LOS_DLOLB_ENA_MSK       0x00000004
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_LOS_DLOLB_ENA_OFF       2
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_LOSS_SYNC_DLOLB_ENA_MSK 0x00000002
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_LOSS_SYNC_DLOLB_ENA_OFF 1
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_HIGH_BER_DLOLB_ENA_MSK  0x00000001
#define FC1200_PMON_REG_DLOLB_EN_REG_BIT_HIGH_BER_DLOLB_ENA_OFF  0

/*---------------------------------------.
 | Register 0x00000018 PERF_MON_STAT_REG |
 +---------------------------------------+
 | bit  3 R  IN_FRAME                    |
 | bit  2 R  LOS                         |
 | bit  1 R  LOSS_SYNC                   |
 | bit  0 R  HIGH_BER                    |
 +--------------------------------------*/
#define PMC_FC1200_PMON_REG_PERF_MON_STAT_REG_UNUSED_MASK   0xfffffff0
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_IN_FRAME_MSK  0x00000008
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_IN_FRAME_OFF  3
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_LOS_MSK       0x00000004
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_LOS_OFF       2
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_LOSS_SYNC_MSK 0x00000002
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_LOSS_SYNC_OFF 1
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_HIGH_BER_MSK  0x00000001
#define FC1200_PMON_REG_PERF_MON_STAT_REG_BIT_HIGH_BER_OFF  0

/*-----------------------------------------.
 | Register 0x0000001c VALID_FRAME_CNT_REG |
 +-----------------------------------------+
 | bit  31:0 R  VALID_FRM_COUNT            |
 +----------------------------------------*/
#define PMC_FC1200_PMON_REG_VALID_FRAME_CNT_REG_UNUSED_MASK         0x00000000
#define FC1200_PMON_REG_VALID_FRAME_CNT_REG_BIT_VALID_FRM_COUNT_MSK 0xffffffff
#define FC1200_PMON_REG_VALID_FRAME_CNT_REG_BIT_VALID_FRM_COUNT_OFF 0

/*-------------------------------------------.
 | Register 0x00000020 INVALID_FRAME_CNT_REG |
 +-------------------------------------------+
 | bit  31:0 R  INVLD_FRM_COUNT              |
 +------------------------------------------*/
#define PMC_FC1200_PMON_REG_INVALID_FRAME_CNT_REG_UNUSED_MASK         0x00000000
#define FC1200_PMON_REG_INVALID_FRAME_CNT_REG_BIT_INVLD_FRM_COUNT_MSK 0xffffffff
#define FC1200_PMON_REG_INVALID_FRAME_CNT_REG_BIT_INVLD_FRM_COUNT_OFF 0

/*----------------------------------------------.
 | Register 0x00000024 VALID_FRAME_BYTE_CNT_REG |
 +----------------------------------------------+
 | bit  31:0 R  VALID_FRM_BYTE_COUNT            |
 +---------------------------------------------*/
#define PMC_FC1200_PMON_REG_VALID_FRAME_BYTE_CNT_REG_UNUSED_MASK              0x00000000
#define FC1200_PMON_REG_VALID_FRAME_BYTE_CNT_REG_BIT_VALID_FRM_BYTE_COUNT_MSK 0xffffffff
#define FC1200_PMON_REG_VALID_FRAME_BYTE_CNT_REG_BIT_VALID_FRM_BYTE_COUNT_OFF 0

/*--------------------------------.
 | Register 0x00000028 INT_EN_REG |
 +--------------------------------+
 | bit  2 R/W  LOS_INT_E          |
 | bit  1 R/W  LOSS_SYNC_INT_E    |
 | bit  0 R/W  HIGH_BER_INT_E     |
 +-------------------------------*/
#define PMC_FC1200_PMON_REG_INT_EN_REG_UNUSED_MASK         0xfffffff0
#define FC1200_PMON_REG_INT_EN_REG_BIT_LOS_INT_E_MSK       0x00000004
#define FC1200_PMON_REG_INT_EN_REG_BIT_LOS_INT_E_OFF       2
#define FC1200_PMON_REG_INT_EN_REG_BIT_LOSS_SYNC_INT_E_MSK 0x00000002
#define FC1200_PMON_REG_INT_EN_REG_BIT_LOSS_SYNC_INT_E_OFF 1
#define FC1200_PMON_REG_INT_EN_REG_BIT_HIGH_BER_INT_E_MSK  0x00000001
#define FC1200_PMON_REG_INT_EN_REG_BIT_HIGH_BER_INT_E_OFF  0

/*----------------------------------------.
 | Register 0x0000002c INT_INDICATION_REG |
 +----------------------------------------+
 | bit  2 R/W  LOS_INT_I                  |
 | bit  1 R/W  LOSS_SYNC_INT_I            |
 | bit  0 R/W  HIGH_BER_INT_I             |
 +---------------------------------------*/
#define PMC_FC1200_PMON_REG_INT_INDICATION_REG_UNUSED_MASK         0xfffffff0
#define FC1200_PMON_REG_INT_INDICATION_REG_BIT_LOS_INT_I_MSK       0x00000004
#define FC1200_PMON_REG_INT_INDICATION_REG_BIT_LOS_INT_I_OFF       2
#define FC1200_PMON_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_INT_I_MSK 0x00000002
#define FC1200_PMON_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_INT_I_OFF 1
#define FC1200_PMON_REG_INT_INDICATION_REG_BIT_HIGH_BER_INT_I_MSK  0x00000001
#define FC1200_PMON_REG_INT_INDICATION_REG_BIT_HIGH_BER_INT_I_OFF  0

/*------------------------------.
 | Register 0x00000030 INT_STAT |
 +------------------------------+
 | bit  2 R  LOS_INT_V          |
 | bit  1 R  LOSS_SYNC_INT_V    |
 | bit  0 R  HIGH_BER_INT_V     |
 +-----------------------------*/
#define PMC_FC1200_PMON_REG_INT_STAT_UNUSED_MASK         0xfffffff0
#define FC1200_PMON_REG_INT_STAT_BIT_LOS_INT_V_MSK       0x00000004
#define FC1200_PMON_REG_INT_STAT_BIT_LOS_INT_V_OFF       2
#define FC1200_PMON_REG_INT_STAT_BIT_LOSS_SYNC_INT_V_MSK 0x00000002
#define FC1200_PMON_REG_INT_STAT_BIT_LOSS_SYNC_INT_V_OFF 1
#define FC1200_PMON_REG_INT_STAT_BIT_HIGH_BER_INT_V_MSK  0x00000001
#define FC1200_PMON_REG_INT_STAT_BIT_HIGH_BER_INT_V_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC1200_PMON_REGS_H */
