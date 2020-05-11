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
 *     fc1200_pcs_tx block
 *****************************************************************************/
#ifndef _FC1200_PCS_TX_REGS_H
#define _FC1200_PCS_TX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC1200_PCS_TX_REG_CFG                         0x00050000
#define PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT               0x00050004
#define PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG 0x00050008
#define PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB        0x0005000c
#define PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB        0x00050010
#define PMC_FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT          0x00050014
#define PMC_FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT          0x00050018
#define PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT 0x0005001c
#define PMC_FC1200_PCS_TX_REG_INT_EN                      0x00050020
#define PMC_FC1200_PCS_TX_REG_INT_INDICATION              0x00050024
#define PMC_FC1200_PCS_TX_REG_INT_STAT                    0x00050028

/*------------------------------.
 | Register 0x00050000 CFG      |
 +------------------------------+
 | bit  8 R/W  CORRUPT_BLK_MODE |
 | bit  5 R/W  LOS_LF_ENA       |
 | bit  4 R/W  EXT_LF_ENA       |
 | bit  3 R/W  FORCE_LF         |
 | bit  1 R/W  PRBS31_ENA       |
 | bit  0 R/W  SW_RESET         |
 +-----------------------------*/
#define PMC_FC1200_PCS_TX_REG_CFG_UNUSED_MASK          0xfff8f8c0
#define FC1200_PCS_TX_REG_CFG_BIT_CORRUPT_BLK_MODE_MSK 0x00000100
#define FC1200_PCS_TX_REG_CFG_BIT_CORRUPT_BLK_MODE_OFF 8
#define FC1200_PCS_TX_REG_CFG_BIT_LOS_LF_ENA_MSK       0x00000020
#define FC1200_PCS_TX_REG_CFG_BIT_LOS_LF_ENA_OFF       5
#define FC1200_PCS_TX_REG_CFG_BIT_EXT_LF_ENA_MSK       0x00000010
#define FC1200_PCS_TX_REG_CFG_BIT_EXT_LF_ENA_OFF       4
#define FC1200_PCS_TX_REG_CFG_BIT_FORCE_LF_MSK         0x00000008
#define FC1200_PCS_TX_REG_CFG_BIT_FORCE_LF_OFF         3
#define FC1200_PCS_TX_REG_CFG_BIT_PRBS31_ENA_MSK       0x00000002
#define FC1200_PCS_TX_REG_CFG_BIT_PRBS31_ENA_OFF       1
#define FC1200_PCS_TX_REG_CFG_BIT_SW_RESET_MSK         0x00000001
#define FC1200_PCS_TX_REG_CFG_BIT_SW_RESET_OFF         0

/*-----------------------------------.
 | Register 0x00050004 PMON_CNT_STAT |
 +-----------------------------------+
 | bit  8 R    TIP                   |
 | bit  0 R/W  PMON_UPDATE           |
 +----------------------------------*/
#define PMC_FC1200_PCS_TX_REG_PMON_CNT_STAT_UNUSED_MASK     0xfffffefe
#define FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_TIP_MSK         0x00000100
#define FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_TIP_OFF         8
#define FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK 0x00000001
#define FC1200_PCS_TX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF 0

/*-------------------------------------------------.
 | Register 0x00050008 LOCAL_FAULT_ORDERED_SET_CFG |
 +-------------------------------------------------+
 | bit  27:24 R/W  LF_O_CODE                       |
 | bit  23:0  R/W  LF_ORDERED_SET                  |
 +------------------------------------------------*/
#define PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_UNUSED_MASK        0xf0000000
#define FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_MSK      0x0f000000
#define FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_O_CODE_OFF      24
#define FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_MSK 0x00ffffff
#define FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CFG_BIT_LF_ORDERED_SET_OFF 0

/*------------------------------------------.
 | Register 0x0005000c TX_DATA_BYTE_CNT_LSB |
 +------------------------------------------+
 | bit  31:0 R  DATA_BYTE_CNT               |
 +-----------------------------------------*/
#define PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_UNUSED_MASK       0x00000000
#define FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK 0xffffffff
#define FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF 0

/*------------------------------------------.
 | Register 0x00050010 TX_DATA_BYTE_CNT_MSB |
 +------------------------------------------+
 | bit  7:0 R  DATA_BYTE_CNT                |
 +-----------------------------------------*/
#define PMC_FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_UNUSED_MASK       0xffffff00
#define FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK 0x000000ff
#define FC1200_PCS_TX_REG_TX_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF 0

/*----------------------------------------.
 | Register 0x00050014 TX_ORDERED_SET_CNT |
 +----------------------------------------+
 | bit  31:0 R  ORDERED_SET_CNT           |
 +---------------------------------------*/
#define PMC_FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_UNUSED_MASK         0x00000000
#define FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK 0xffffffff
#define FC1200_PCS_TX_REG_TX_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF 0

/*----------------------------------------.
 | Register 0x00050018 ERR_CTRL_BLOCK_CNT |
 +----------------------------------------+
 | bit  31:0 R  ERROR_BLK_CNT             |
 +---------------------------------------*/
#define PMC_FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_UNUSED_MASK       0x00000000
#define FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_MSK 0xffffffff
#define FC1200_PCS_TX_REG_ERR_CTRL_BLOCK_CNT_BIT_ERROR_BLK_CNT_OFF 0

/*-------------------------------------------------.
 | Register 0x0005001c LOCAL_FAULT_ORDERED_SET_CNT |
 +-------------------------------------------------+
 | bit  31:0 R  LF_OS_CNT                          |
 +------------------------------------------------*/
#define PMC_FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_UNUSED_MASK   0x00000000
#define FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_MSK 0xffffffff
#define FC1200_PCS_TX_REG_LOCAL_FAULT_ORDERED_SET_CNT_BIT_LF_OS_CNT_OFF 0

/*----------------------------.
 | Register 0x00050020 INT_EN |
 +----------------------------+
 | bit  1 R/W  BUF_UNDERRUN_E |
 | bit  0 R/W  LOCAL_FAULT_E  |
 +---------------------------*/
#define PMC_FC1200_PCS_TX_REG_INT_EN_UNUSED_MASK        0xfffffffc
#define FC1200_PCS_TX_REG_INT_EN_BIT_BUF_UNDERRUN_E_MSK 0x00000002
#define FC1200_PCS_TX_REG_INT_EN_BIT_BUF_UNDERRUN_E_OFF 1
#define FC1200_PCS_TX_REG_INT_EN_BIT_LOCAL_FAULT_E_MSK  0x00000001
#define FC1200_PCS_TX_REG_INT_EN_BIT_LOCAL_FAULT_E_OFF  0

/*------------------------------------.
 | Register 0x00050024 INT_INDICATION |
 +------------------------------------+
 | bit  1 R/W  BUF_UNDERRUN_I         |
 | bit  0 R/W  LOCAL_FAULT_I          |
 +-----------------------------------*/
#define PMC_FC1200_PCS_TX_REG_INT_INDICATION_UNUSED_MASK        0xfffffffc
#define FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_MSK 0x00000002
#define FC1200_PCS_TX_REG_INT_INDICATION_BIT_BUF_UNDERRUN_I_OFF 1
#define FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_MSK  0x00000001
#define FC1200_PCS_TX_REG_INT_INDICATION_BIT_LOCAL_FAULT_I_OFF  0

/*------------------------------.
 | Register 0x00050028 INT_STAT |
 +------------------------------+
 | bit  1 R  BUF_UNDERRUN_V     |
 | bit  0 R  LOCAL_FAULT_V      |
 +-----------------------------*/
#define PMC_FC1200_PCS_TX_REG_INT_STAT_UNUSED_MASK        0xfffffffc
#define FC1200_PCS_TX_REG_INT_STAT_BIT_BUF_UNDERRUN_V_MSK 0x00000002
#define FC1200_PCS_TX_REG_INT_STAT_BIT_BUF_UNDERRUN_V_OFF 1
#define FC1200_PCS_TX_REG_INT_STAT_BIT_LOCAL_FAULT_V_MSK  0x00000001
#define FC1200_PCS_TX_REG_INT_STAT_BIT_LOCAL_FAULT_V_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC1200_PCS_TX_REGS_H */
