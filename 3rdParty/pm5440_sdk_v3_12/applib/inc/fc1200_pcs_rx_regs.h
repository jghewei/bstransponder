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
 *     fc1200_pcs_rx block
 *****************************************************************************/
#ifndef _FC1200_PCS_RX_REGS_H
#define _FC1200_PCS_RX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC1200_PCS_RX_REG_CFG_REG               0x0002f000
#define PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT         0x0002f004
#define PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG      0x0002f008
#define PMC_FC1200_PCS_RX_REG_STAT_REG              0x0002f014
#define PMC_FC1200_PCS_RX_REG_INT_EN_REG            0x0002f018
#define PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG    0x0002f01c
#define PMC_FC1200_PCS_RX_REG_INT_STAT_REG          0x0002f020
#define PMC_FC1200_PCS_RX_REG_ERR_CNTS              0x0002f024
#define PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS        0x0002f028
#define PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB 0x0002f02c
#define PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB 0x0002f030
#define PMC_FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT   0x0002f034

/*----------------------------------.
 | Register 0x0002f000 CFG_REG      |
 +----------------------------------+
 | bit  4 R/W  RESTORE_INVALID_IDLE |
 | bit  2 R/W  PRBS31_ENA           |
 | bit  1 R/W  DESCRAMBLER_ENA      |
 | bit  0 R/W  SW_RESET             |
 +---------------------------------*/
#define PMC_FC1200_PCS_RX_REG_CFG_REG_UNUSED_MASK              0xffffffe0
#define FC1200_PCS_RX_REG_CFG_REG_BIT_RESTORE_INVALID_IDLE_MSK 0x00000010
#define FC1200_PCS_RX_REG_CFG_REG_BIT_RESTORE_INVALID_IDLE_OFF 4
#define FC1200_PCS_RX_REG_CFG_REG_BIT_PRBS31_ENA_MSK           0x00000004
#define FC1200_PCS_RX_REG_CFG_REG_BIT_PRBS31_ENA_OFF           2
#define FC1200_PCS_RX_REG_CFG_REG_BIT_DESCRAMBLER_ENA_MSK      0x00000002
#define FC1200_PCS_RX_REG_CFG_REG_BIT_DESCRAMBLER_ENA_OFF      1
#define FC1200_PCS_RX_REG_CFG_REG_BIT_SW_RESET_MSK             0x00000001
#define FC1200_PCS_RX_REG_CFG_REG_BIT_SW_RESET_OFF             0

/*-----------------------------------.
 | Register 0x0002f004 PMON_CNT_STAT |
 +-----------------------------------+
 | bit  8 R    TIP                   |
 | bit  0 R/W  PMON_UPDATE           |
 +----------------------------------*/
#define PMC_FC1200_PCS_RX_REG_PMON_CNT_STAT_UNUSED_MASK     0xfffffefe
#define FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_TIP_MSK         0x00000100
#define FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_TIP_OFF         8
#define FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_MSK 0x00000001
#define FC1200_PCS_RX_REG_PMON_CNT_STAT_BIT_PMON_UPDATE_OFF 0

/*--------------------------------------.
 | Register 0x0002f008 HIGH_BER_CFG_REG |
 +--------------------------------------+
 | bit  15:0 R/W  COUNT_125US           |
 +-------------------------------------*/
#define PMC_FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_UNUSED_MASK     0xffff0000
#define FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_MSK 0x0000ffff
#define FC1200_PCS_RX_REG_HIGH_BER_CFG_REG_BIT_COUNT_125US_OFF 0

/*------------------------------.
 | Register 0x0002f014 STAT_REG |
 +------------------------------+
 | bit  1 R  REMOTE_FAULT       |
 | bit  0 R  LOCAL_FAULT        |
 +-----------------------------*/
#define PMC_FC1200_PCS_RX_REG_STAT_REG_UNUSED_MASK      0xfffffffc
#define FC1200_PCS_RX_REG_STAT_REG_BIT_REMOTE_FAULT_MSK 0x00000002
#define FC1200_PCS_RX_REG_STAT_REG_BIT_REMOTE_FAULT_OFF 1
#define FC1200_PCS_RX_REG_STAT_REG_BIT_LOCAL_FAULT_MSK  0x00000001
#define FC1200_PCS_RX_REG_STAT_REG_BIT_LOCAL_FAULT_OFF  0

/*--------------------------------.
 | Register 0x0002f018 INT_EN_REG |
 +--------------------------------+
 | bit  5 R/W  RF_E               |
 | bit  4 R/W  LF_E               |
 | bit  2 R/W  HIGH_BER_E         |
 | bit  1 R/W  LOSS_SYNC_E        |
 | bit  0 R/W  LOS_E              |
 +-------------------------------*/
#define PMC_FC1200_PCS_RX_REG_INT_EN_REG_UNUSED_MASK     0xffffffc8
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_RF_E_MSK        0x00000020
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_RF_E_OFF        5
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_LF_E_MSK        0x00000010
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_LF_E_OFF        4
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_HIGH_BER_E_MSK  0x00000004
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_HIGH_BER_E_OFF  2
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOSS_SYNC_E_MSK 0x00000002
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOSS_SYNC_E_OFF 1
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOS_E_MSK       0x00000001
#define FC1200_PCS_RX_REG_INT_EN_REG_BIT_LOS_E_OFF       0

/*----------------------------------------.
 | Register 0x0002f01c INT_INDICATION_REG |
 +----------------------------------------+
 | bit  5 R/W  RF_I                       |
 | bit  4 R/W  LF_I                       |
 | bit  2 R/W  HIGH_BER_I                 |
 | bit  1 R/W  LOSS_SYNC_I                |
 | bit  0 R/W  LOS_I                      |
 +---------------------------------------*/
#define PMC_FC1200_PCS_RX_REG_INT_INDICATION_REG_UNUSED_MASK     0xffffffc8
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_MSK        0x00000020
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_RF_I_OFF        5
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_MSK        0x00000010
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LF_I_OFF        4
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_MSK  0x00000004
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_HIGH_BER_I_OFF  2
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_MSK 0x00000002
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOSS_SYNC_I_OFF 1
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_MSK       0x00000001
#define FC1200_PCS_RX_REG_INT_INDICATION_REG_BIT_LOS_I_OFF       0

/*----------------------------------.
 | Register 0x0002f020 INT_STAT_REG |
 +----------------------------------+
 | bit  5 R  RF_V                   |
 | bit  4 R  LF_V                   |
 | bit  2 R  HIGH_BER_V             |
 | bit  1 R  LOSS_SYNC_V            |
 | bit  0 R  LOS_V                  |
 +---------------------------------*/
#define PMC_FC1200_PCS_RX_REG_INT_STAT_REG_UNUSED_MASK     0xffffffc8
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_RF_V_MSK        0x00000020
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_RF_V_OFF        5
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LF_V_MSK        0x00000010
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LF_V_OFF        4
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_HIGH_BER_V_MSK  0x00000004
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_HIGH_BER_V_OFF  2
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOSS_SYNC_V_MSK 0x00000002
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOSS_SYNC_V_OFF 1
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOS_V_MSK       0x00000001
#define FC1200_PCS_RX_REG_INT_STAT_REG_BIT_LOS_V_OFF       0

/*--------------------------------.
 | Register 0x0002f024 ERR_CNTS   |
 +--------------------------------+
 | bit  23:16 R  HIGH_BER_ERR_CNT |
 | bit  15:0  R  PRBS31_ERR_CNT   |
 +-------------------------------*/
#define PMC_FC1200_PCS_RX_REG_ERR_CNTS_UNUSED_MASK          0xff000000
#define FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_MSK 0x00ff0000
#define FC1200_PCS_RX_REG_ERR_CNTS_BIT_HIGH_BER_ERR_CNT_OFF 16
#define FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_MSK   0x0000ffff
#define FC1200_PCS_RX_REG_ERR_CNTS_BIT_PRBS31_ERR_CNT_OFF   0

/*------------------------------------.
 | Register 0x0002f028 BLOCK_ERR_CNTS |
 +------------------------------------+
 | bit  31:16 R  BLK_TYPE_ERR_CNT     |
 | bit  15:0  R  INVLD_BLK_CNT        |
 +-----------------------------------*/
#define PMC_FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_UNUSED_MASK          0x00000000
#define FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_MSK 0xffff0000
#define FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_BLK_TYPE_ERR_CNT_OFF 16
#define FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_MSK    0x0000ffff
#define FC1200_PCS_RX_REG_BLOCK_ERR_CNTS_BIT_INVLD_BLK_CNT_OFF    0

/*-------------------------------------------.
 | Register 0x0002f02c RXD_DATA_BYTE_CNT_LSB |
 +-------------------------------------------+
 | bit  31:0 R  DATA_BYTE_CNT                |
 +------------------------------------------*/
#define PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_UNUSED_MASK       0x00000000
#define FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_MSK 0xffffffff
#define FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_LSB_BIT_DATA_BYTE_CNT_OFF 0

/*-------------------------------------------.
 | Register 0x0002f030 RXD_DATA_BYTE_CNT_MSB |
 +-------------------------------------------+
 | bit  7:0 R  DATA_BYTE_CNT                 |
 +------------------------------------------*/
#define PMC_FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_UNUSED_MASK       0xffffff00
#define FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_MSK 0x000000ff
#define FC1200_PCS_RX_REG_RXD_DATA_BYTE_CNT_MSB_BIT_DATA_BYTE_CNT_OFF 0

/*-----------------------------------------.
 | Register 0x0002f034 RXD_ORDERED_SET_CNT |
 +-----------------------------------------+
 | bit  31:0 R  ORDERED_SET_CNT            |
 +----------------------------------------*/
#define PMC_FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_UNUSED_MASK         0x00000000
#define FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_MSK 0xffffffff
#define FC1200_PCS_RX_REG_RXD_ORDERED_SET_CNT_BIT_ORDERED_SET_CNT_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC1200_PCS_RX_REGS_H */
