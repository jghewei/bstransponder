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
 *     gsup43_c73 block
 *****************************************************************************/
#ifndef _GSUP43_C73_REGS_H
#define _GSUP43_C73_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_GSUP43_C73_REG_CFG                                         0x00000000
#define PMC_GSUP43_C73_REG_RX_CFG                                      0x00000004
#define PMC_GSUP43_C73_REG_RX_GSUP43_CFG                               0x00000008
#define PMC_GSUP43_C73_REG_RX_125US_CNT                                0x0000000c
#define PMC_GSUP43_C73_REG_RX_INT_EN                                   0x00000010
#define PMC_GSUP43_C73_REG_RX_INT_INDICATION                           0x00000014
#define PMC_GSUP43_C73_REG_RX_STAT                                     0x00000018
#define PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT                            0x0000001c
#define PMC_GSUP43_C73_REG_RX_BLOCK_ERR_CNTS                           0x00000020
#define PMC_GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB            0x00000024
#define PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT 0x00000028
#define PMC_GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB                        0x0000002c
#define PMC_GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB                      0x00000030
#define PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB        0x00000034
#define PMC_GSUP43_C73_REG_RX_PACKET_CNT_LSB                           0x00000038
#define PMC_GSUP43_C73_REG_RX_PACKET_CNT_MSB                           0x0000003c
#define PMC_GSUP43_C73_REG_TX_CFG_1                                    0x00000040
#define PMC_GSUP43_C73_REG_TX_CFG_2                                    0x00000044
#define PMC_GSUP43_C73_REG_TX_PACKET_CNT_LSB                           0x00000048
#define PMC_GSUP43_C73_REG_TX_PACKET_CNT_MSB                           0x0000004c
#define PMC_GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB                        0x00000050
#define PMC_GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB                      0x00000054
#define PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB        0x00000058
#define PMC_GSUP43_C73_REG_TX_INT_EN                                   0x0000005c
#define PMC_GSUP43_C73_REG_TX_INT_INDICATION                           0x00000060
#define PMC_GSUP43_C73_REG_TX_STAT                                     0x00000064

/*-------------------------.
 | Register 0x00000000 CFG |
 +-------------------------+
 | bit  1 R/W  PMON_UPDATE |
 | bit  0 R/W  SW_RST      |
 +------------------------*/
#define PMC_GSUP43_C73_REG_CFG_UNUSED_MASK     0xfffffffc
#define GSUP43_C73_REG_CFG_BIT_PMON_UPDATE_MSK 0x00000002
#define GSUP43_C73_REG_CFG_BIT_PMON_UPDATE_OFF 1
#define GSUP43_C73_REG_CFG_BIT_SW_RST_MSK      0x00000001
#define GSUP43_C73_REG_CFG_BIT_SW_RST_OFF      0

/*----------------------------.
 | Register 0x00000004 RX_CFG |
 +----------------------------+
 | bit  31 R/W  RX_SW_RST     |
 | bit  0  R/W  LOS_INV       |
 +---------------------------*/
#define PMC_GSUP43_C73_REG_RX_CFG_UNUSED_MASK   0x7ffffff8
#define GSUP43_C73_REG_RX_CFG_BIT_RX_SW_RST_MSK 0x80000000
#define GSUP43_C73_REG_RX_CFG_BIT_RX_SW_RST_OFF 31
#define GSUP43_C73_REG_RX_CFG_BIT_LOS_INV_MSK   0x00000001
#define GSUP43_C73_REG_RX_CFG_BIT_LOS_INV_OFF   0

/*------------------------------------.
 | Register 0x00000008 RX_GSUP43_CFG  |
 +------------------------------------+
 | bit  31    R/W  RX_GSUP43_C73_EN   |
 | bit  25:24 R/W  DSCRD_ZONE         |
 | bit  23    R/W  RX_SIG_OS_DSCRD_EN |
 | bit  22    R/W  RX_SEQ_OS_DSCRD_EN |
 | bit  21    R/W  RX_OS_ALL_DSCRD    |
 | bit  20    R/W  RX_FULL_OS_RPT_DIS |
 | bit  19:16 R/W  RX_MIN_OS_RPT      |
 | bit  15:8  R/W  RX_PKT_TYPE        |
 | bit  7:0   R/W  RX_OS_TYPE         |
 +-----------------------------------*/
#define PMC_GSUP43_C73_REG_RX_GSUP43_CFG_UNUSED_MASK            0x7c000000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_GSUP43_C73_EN_MSK   0x80000000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_GSUP43_C73_EN_OFF   31
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_MSK         0x03000000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_DSCRD_ZONE_OFF         24
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SIG_OS_DSCRD_EN_MSK 0x00800000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SIG_OS_DSCRD_EN_OFF 23
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SEQ_OS_DSCRD_EN_MSK 0x00400000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_SEQ_OS_DSCRD_EN_OFF 22
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_ALL_DSCRD_MSK    0x00200000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_ALL_DSCRD_OFF    21
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_FULL_OS_RPT_DIS_MSK 0x00100000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_FULL_OS_RPT_DIS_OFF 20
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_MSK      0x000f0000
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_MIN_OS_RPT_OFF      16
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_MSK        0x0000ff00
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_PKT_TYPE_OFF        8
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_MSK         0x000000ff
#define GSUP43_C73_REG_RX_GSUP43_CFG_BIT_RX_OS_TYPE_OFF         0

/*----------------------------------.
 | Register 0x0000000c RX_125US_CNT |
 +----------------------------------+
 | bit  15:0 R/W  RX_125US_CNT      |
 +---------------------------------*/
#define PMC_GSUP43_C73_REG_RX_125US_CNT_UNUSED_MASK      0xffff0000
#define GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_MSK 0x0000ffff
#define GSUP43_C73_REG_RX_125US_CNT_BIT_RX_125US_CNT_OFF 0

/*---------------------------------.
 | Register 0x00000010 RX_INT_EN   |
 +---------------------------------+
 | bit  9 R/W  RX_LF_E             |
 | bit  8 R/W  RX_RF_E             |
 | bit  7 R/W  ALL_ONES_OR_ZEROS_E |
 | bit  6 R/W  RX_PRBS31_ERR_E     |
 | bit  5 R/W  BLK_TYPE_ERR_E      |
 | bit  4 R/W  RX_FAULT_E          |
 | bit  3 R/W  LINK_FAIL_E         |
 | bit  2 R/W  HI_BER_E            |
 | bit  1 R/W  LOS_SYNC_E          |
 | bit  0 R/W  LOS_E               |
 +--------------------------------*/
#define PMC_GSUP43_C73_REG_RX_INT_EN_UNUSED_MASK             0xfffffc00
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_LF_E_MSK             0x00000200
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_LF_E_OFF             9
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_RF_E_MSK             0x00000100
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_RF_E_OFF             8
#define GSUP43_C73_REG_RX_INT_EN_BIT_ALL_ONES_OR_ZEROS_E_MSK 0x00000080
#define GSUP43_C73_REG_RX_INT_EN_BIT_ALL_ONES_OR_ZEROS_E_OFF 7
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_PRBS31_ERR_E_MSK     0x00000040
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_PRBS31_ERR_E_OFF     6
#define GSUP43_C73_REG_RX_INT_EN_BIT_BLK_TYPE_ERR_E_MSK      0x00000020
#define GSUP43_C73_REG_RX_INT_EN_BIT_BLK_TYPE_ERR_E_OFF      5
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_FAULT_E_MSK          0x00000010
#define GSUP43_C73_REG_RX_INT_EN_BIT_RX_FAULT_E_OFF          4
#define GSUP43_C73_REG_RX_INT_EN_BIT_LINK_FAIL_E_MSK         0x00000008
#define GSUP43_C73_REG_RX_INT_EN_BIT_LINK_FAIL_E_OFF         3
#define GSUP43_C73_REG_RX_INT_EN_BIT_HI_BER_E_MSK            0x00000004
#define GSUP43_C73_REG_RX_INT_EN_BIT_HI_BER_E_OFF            2
#define GSUP43_C73_REG_RX_INT_EN_BIT_LOS_SYNC_E_MSK          0x00000002
#define GSUP43_C73_REG_RX_INT_EN_BIT_LOS_SYNC_E_OFF          1
#define GSUP43_C73_REG_RX_INT_EN_BIT_LOS_E_MSK               0x00000001
#define GSUP43_C73_REG_RX_INT_EN_BIT_LOS_E_OFF               0

/*---------------------------------------.
 | Register 0x00000014 RX_INT_INDICATION |
 +---------------------------------------+
 | bit  9 R/W  RX_LF_IC                  |
 | bit  8 R/W  RX_RF_IC                  |
 | bit  7 R/W  ALL_ONES_OR_ZEROS_IC      |
 | bit  6 R/W  RX_PRBS31_ERR_I           |
 | bit  5 R/W  BLK_TYPE_ERR_I            |
 | bit  4 R/W  RX_FAULT_IC               |
 | bit  3 R/W  LINK_FAIL_IC              |
 | bit  2 R/W  HI_BER_IC                 |
 | bit  1 R/W  LOS_SYNC_IC               |
 | bit  0 R/W  LOS_IC                    |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_INT_INDICATION_UNUSED_MASK              0xfffffc00
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_MSK             0x00000200
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_LF_IC_OFF             9
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_MSK             0x00000100
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_RF_IC_OFF             8
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_MSK 0x00000080
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_ALL_ONES_OR_ZEROS_IC_OFF 7
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK      0x00000040
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF      6
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_MSK       0x00000020
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_BLK_TYPE_ERR_I_OFF       5
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_MSK          0x00000010
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_RX_FAULT_IC_OFF          4
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_MSK         0x00000008
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_LINK_FAIL_IC_OFF         3
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_MSK            0x00000004
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_HI_BER_IC_OFF            2
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_MSK          0x00000002
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_SYNC_IC_OFF          1
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_MSK               0x00000001
#define GSUP43_C73_REG_RX_INT_INDICATION_BIT_LOS_IC_OFF               0

/*--------------------------------.
 | Register 0x00000018 RX_STAT    |
 +--------------------------------+
 | bit  31 R  RF_FAULT            |
 | bit  30 R  LF_FAULT            |
 | bit  9  R  RX_LF_V             |
 | bit  8  R  RX_RF_V             |
 | bit  7  R  ALL_ONES_OR_ZEROS_V |
 | bit  4  R  RX_FAULT_V          |
 | bit  3  R  LINK_FAIL_V         |
 | bit  2  R  HI_BER_V            |
 | bit  1  R  LOS_SYNC_V          |
 | bit  0  R  LOS_V               |
 +-------------------------------*/
#define PMC_GSUP43_C73_REG_RX_STAT_UNUSED_MASK             0x3ffffc60
#define GSUP43_C73_REG_RX_STAT_BIT_RF_FAULT_MSK            0x80000000
#define GSUP43_C73_REG_RX_STAT_BIT_RF_FAULT_OFF            31
#define GSUP43_C73_REG_RX_STAT_BIT_LF_FAULT_MSK            0x40000000
#define GSUP43_C73_REG_RX_STAT_BIT_LF_FAULT_OFF            30
#define GSUP43_C73_REG_RX_STAT_BIT_RX_LF_V_MSK             0x00000200
#define GSUP43_C73_REG_RX_STAT_BIT_RX_LF_V_OFF             9
#define GSUP43_C73_REG_RX_STAT_BIT_RX_RF_V_MSK             0x00000100
#define GSUP43_C73_REG_RX_STAT_BIT_RX_RF_V_OFF             8
#define GSUP43_C73_REG_RX_STAT_BIT_ALL_ONES_OR_ZEROS_V_MSK 0x00000080
#define GSUP43_C73_REG_RX_STAT_BIT_ALL_ONES_OR_ZEROS_V_OFF 7
#define GSUP43_C73_REG_RX_STAT_BIT_RX_FAULT_V_MSK          0x00000010
#define GSUP43_C73_REG_RX_STAT_BIT_RX_FAULT_V_OFF          4
#define GSUP43_C73_REG_RX_STAT_BIT_LINK_FAIL_V_MSK         0x00000008
#define GSUP43_C73_REG_RX_STAT_BIT_LINK_FAIL_V_OFF         3
#define GSUP43_C73_REG_RX_STAT_BIT_HI_BER_V_MSK            0x00000004
#define GSUP43_C73_REG_RX_STAT_BIT_HI_BER_V_OFF            2
#define GSUP43_C73_REG_RX_STAT_BIT_LOS_SYNC_V_MSK          0x00000002
#define GSUP43_C73_REG_RX_STAT_BIT_LOS_SYNC_V_OFF          1
#define GSUP43_C73_REG_RX_STAT_BIT_LOS_V_MSK               0x00000001
#define GSUP43_C73_REG_RX_STAT_BIT_LOS_V_OFF               0

/*--------------------------------------.
 | Register 0x0000001c RX_FRAME_ERR_CNT |
 +--------------------------------------+
 | bit  15:8 R  BLK_TYPE_ERR_CNT        |
 | bit  7:0  R  HI_BER_CNT              |
 +-------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_FRAME_ERR_CNT_UNUSED_MASK          0xffff0000
#define GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_MSK 0x0000ff00
#define GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_BLK_TYPE_ERR_CNT_OFF 8
#define GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_MSK       0x000000ff
#define GSUP43_C73_REG_RX_FRAME_ERR_CNT_BIT_HI_BER_CNT_OFF       0

/*---------------------------------------.
 | Register 0x00000020 RX_BLOCK_ERR_CNTS |
 +---------------------------------------+
 | bit  31:0 R  RX_BLK_ERR_CNT           |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_UNUSED_MASK        0x00000000
#define GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_RX_BLOCK_ERR_CNTS_BIT_RX_BLK_ERR_CNT_OFF 0

/*------------------------------------------------------.
 | Register 0x00000024 RX_DISCARDED_ORDERED_SET_CNT_LSB |
 +------------------------------------------------------+
 | bit  31:0 R  RX_OS_DSCRCD_CNT                        |
 +-----------------------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_UNUSED_MASK          0x00000000
#define GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_RX_DISCARDED_ORDERED_SET_CNT_LSB_BIT_RX_OS_DSCRCD_CNT_OFF 0

/*-----------------------------------------------------------------.
 | Register 0x00000028 RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT |
 +-----------------------------------------------------------------+
 | bit  31:16 R  PRBS31_ERR_CNT                                    |
 | bit  7:0   R  RX_OS_DSCRCD_CNT                                  |
 +----------------------------------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_UNUSED_MASK          0x0000ff00
#define GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_MSK   0xffff0000
#define GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_PRBS31_ERR_CNT_OFF   16
#define GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_MSK 0x000000ff
#define GSUP43_C73_REG_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_BIT_RX_OS_DSCRCD_CNT_OFF 0

/*------------------------------------------.
 | Register 0x0000002c RX_DATA_BYTE_CNT_LSB |
 +------------------------------------------+
 | bit  31:0 R  RX_DATA_BYTE_CNT            |
 +-----------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_UNUSED_MASK          0x00000000
#define GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_RX_DATA_BYTE_CNT_LSB_BIT_RX_DATA_BYTE_CNT_OFF 0

/*--------------------------------------------.
 | Register 0x00000030 RX_ORDERED_SET_CNT_LSB |
 +--------------------------------------------+
 | bit  31:0 R  RX_OS_CNT                     |
 +-------------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_UNUSED_MASK   0x00000000
#define GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_RX_ORDERED_SET_CNT_LSB_BIT_RX_OS_CNT_OFF 0

/*----------------------------------------------------------.
 | Register 0x00000034 RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB |
 +----------------------------------------------------------+
 | bit  31:24 R  RX_DATA_BYTE_CNT                           |
 | bit  7:0   R  RX_OS_CNT                                  |
 +---------------------------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_UNUSED_MASK          0x00ffff00
#define GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_MSK 0xff000000
#define GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_DATA_BYTE_CNT_OFF 24
#define GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_MSK        0x000000ff
#define GSUP43_C73_REG_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_RX_OS_CNT_OFF        0

/*---------------------------------------.
 | Register 0x00000038 RX_PACKET_CNT_LSB |
 +---------------------------------------+
 | bit  31:0 R  RX_PKT_CNT               |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_PACKET_CNT_LSB_UNUSED_MASK    0x00000000
#define GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_RX_PACKET_CNT_LSB_BIT_RX_PKT_CNT_OFF 0

/*---------------------------------------.
 | Register 0x0000003c RX_PACKET_CNT_MSB |
 +---------------------------------------+
 | bit  7:0 R  RX_PKT_CNT                |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_RX_PACKET_CNT_MSB_UNUSED_MASK    0xffffff00
#define GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_MSK 0x000000ff
#define GSUP43_C73_REG_RX_PACKET_CNT_MSB_BIT_RX_PKT_CNT_OFF 0

/*------------------------------.
 | Register 0x00000040 TX_CFG_1 |
 +------------------------------+
 | bit  31   R/W  TX_SW_RST     |
 | bit  16   R/W  TX_ERR_DROP   |
 | bit  15:8 R/W  TX_PKT_TYPE   |
 | bit  7:0  R/W  TX_OS_TYPE    |
 +-----------------------------*/
#define PMC_GSUP43_C73_REG_TX_CFG_1_UNUSED_MASK     0x7ffe0000
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_SW_RST_MSK   0x80000000
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_SW_RST_OFF   31
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_ERR_DROP_MSK 0x00010000
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_ERR_DROP_OFF 16
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_MSK 0x0000ff00
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_PKT_TYPE_OFF 8
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_MSK  0x000000ff
#define GSUP43_C73_REG_TX_CFG_1_BIT_TX_OS_TYPE_OFF  0

/*--------------------------------.
 | Register 0x00000044 TX_CFG_2   |
 +--------------------------------+
 | bit  31  R/W  TX_GSUP43_C73_EN |
 | bit  10  R/W  FORCE_MIN_IPG    |
 | bit  9   R/W  UNIDIR_EN        |
 | bit  8   R/W  SEND_IDLE_EN     |
 | bit  7   R/W  SEND_LF_EN       |
 | bit  6   R/W  SEND_RF_EN       |
 | bit  5   R/W  PRBS31_EN        |
 | bit  4:0 R/W  FIFO_HMARK       |
 +-------------------------------*/
#define PMC_GSUP43_C73_REG_TX_CFG_2_UNUSED_MASK          0x7fffe000
#define GSUP43_C73_REG_TX_CFG_2_BIT_TX_GSUP43_C73_EN_MSK 0x80000000
#define GSUP43_C73_REG_TX_CFG_2_BIT_TX_GSUP43_C73_EN_OFF 31
#define GSUP43_C73_REG_TX_CFG_2_BIT_FORCE_MIN_IPG_MSK    0x00000400
#define GSUP43_C73_REG_TX_CFG_2_BIT_FORCE_MIN_IPG_OFF    10
#define GSUP43_C73_REG_TX_CFG_2_BIT_UNIDIR_EN_MSK        0x00000200
#define GSUP43_C73_REG_TX_CFG_2_BIT_UNIDIR_EN_OFF        9
#define GSUP43_C73_REG_TX_CFG_2_BIT_SEND_IDLE_EN_MSK     0x00000100
#define GSUP43_C73_REG_TX_CFG_2_BIT_SEND_IDLE_EN_OFF     8
#define GSUP43_C73_REG_TX_CFG_2_BIT_SEND_LF_EN_MSK       0x00000080
#define GSUP43_C73_REG_TX_CFG_2_BIT_SEND_LF_EN_OFF       7
#define GSUP43_C73_REG_TX_CFG_2_BIT_SEND_RF_EN_MSK       0x00000040
#define GSUP43_C73_REG_TX_CFG_2_BIT_SEND_RF_EN_OFF       6
#define GSUP43_C73_REG_TX_CFG_2_BIT_PRBS31_EN_MSK        0x00000020
#define GSUP43_C73_REG_TX_CFG_2_BIT_PRBS31_EN_OFF        5
#define GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_MSK       0x0000001f
#define GSUP43_C73_REG_TX_CFG_2_BIT_FIFO_HMARK_OFF       0

/*---------------------------------------.
 | Register 0x00000048 TX_PACKET_CNT_LSB |
 +---------------------------------------+
 | bit  31:0 R  TX_PKT_CNT               |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_TX_PACKET_CNT_LSB_UNUSED_MASK    0x00000000
#define GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_TX_PACKET_CNT_LSB_BIT_TX_PKT_CNT_OFF 0

/*---------------------------------------.
 | Register 0x0000004c TX_PACKET_CNT_MSB |
 +---------------------------------------+
 | bit  7:0 R  TX_PKT_CNT                |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_TX_PACKET_CNT_MSB_UNUSED_MASK    0xffffff00
#define GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_MSK 0x000000ff
#define GSUP43_C73_REG_TX_PACKET_CNT_MSB_BIT_TX_PKT_CNT_OFF 0

/*------------------------------------------.
 | Register 0x00000050 TX_DATA_BYTE_CNT_LSB |
 +------------------------------------------+
 | bit  31:0 R  TX_DATA_BYTE_CNT            |
 +-----------------------------------------*/
#define PMC_GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_UNUSED_MASK          0x00000000
#define GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_TX_DATA_BYTE_CNT_LSB_BIT_TX_DATA_BYTE_CNT_OFF 0

/*--------------------------------------------.
 | Register 0x00000054 TX_ORDERED_SET_CNT_LSB |
 +--------------------------------------------+
 | bit  31:0 R  TX_OS_CNT                     |
 +-------------------------------------------*/
#define PMC_GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_UNUSED_MASK   0x00000000
#define GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_MSK 0xffffffff
#define GSUP43_C73_REG_TX_ORDERED_SET_CNT_LSB_BIT_TX_OS_CNT_OFF 0

/*----------------------------------------------------------.
 | Register 0x00000058 TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB |
 +----------------------------------------------------------+
 | bit  15:8 R  TX_DATA_BYTE_CNT                            |
 | bit  7:0  R  TX_OS_CNT                                   |
 +---------------------------------------------------------*/
#define PMC_GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_UNUSED_MASK          0xffff0000
#define GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_MSK 0x0000ff00
#define GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_DATA_BYTE_CNT_OFF 8
#define GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_MSK        0x000000ff
#define GSUP43_C73_REG_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_BIT_TX_OS_CNT_OFF        0

/*-------------------------------.
 | Register 0x0000005c TX_INT_EN |
 +-------------------------------+
 | bit  5 R/W  TYPE_ERR_E        |
 | bit  4 R/W  SEND_IDLE_E       |
 | bit  3 R/W  SEND_LF_E         |
 | bit  2 R/W  SEND_RF_E         |
 | bit  1 R/W  FIFO_OVRUN_E      |
 | bit  0 R/W  FIFO_UNDRUN_E     |
 +------------------------------*/
#define PMC_GSUP43_C73_REG_TX_INT_EN_UNUSED_MASK       0xffffffc0
#define GSUP43_C73_REG_TX_INT_EN_BIT_TYPE_ERR_E_MSK    0x00000020
#define GSUP43_C73_REG_TX_INT_EN_BIT_TYPE_ERR_E_OFF    5
#define GSUP43_C73_REG_TX_INT_EN_BIT_SEND_IDLE_E_MSK   0x00000010
#define GSUP43_C73_REG_TX_INT_EN_BIT_SEND_IDLE_E_OFF   4
#define GSUP43_C73_REG_TX_INT_EN_BIT_SEND_LF_E_MSK     0x00000008
#define GSUP43_C73_REG_TX_INT_EN_BIT_SEND_LF_E_OFF     3
#define GSUP43_C73_REG_TX_INT_EN_BIT_SEND_RF_E_MSK     0x00000004
#define GSUP43_C73_REG_TX_INT_EN_BIT_SEND_RF_E_OFF     2
#define GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_OVRUN_E_MSK  0x00000002
#define GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_OVRUN_E_OFF  1
#define GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_UNDRUN_E_MSK 0x00000001
#define GSUP43_C73_REG_TX_INT_EN_BIT_FIFO_UNDRUN_E_OFF 0

/*---------------------------------------.
 | Register 0x00000060 TX_INT_INDICATION |
 +---------------------------------------+
 | bit  5 R/W  TYPE_ERR_I                |
 | bit  4 R/W  SEND_IDLE_I               |
 | bit  3 R/W  SEND_LF_I                 |
 | bit  2 R/W  SEND_RF_I                 |
 | bit  1 R/W  FIFO_OVRUN_I              |
 | bit  0 R/W  FIFO_UNDRUN_I             |
 +--------------------------------------*/
#define PMC_GSUP43_C73_REG_TX_INT_INDICATION_UNUSED_MASK       0xffffffc0
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_MSK    0x00000020
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_TYPE_ERR_I_OFF    5
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_MSK   0x00000010
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_IDLE_I_OFF   4
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_MSK     0x00000008
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_LF_I_OFF     3
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_MSK     0x00000004
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_SEND_RF_I_OFF     2
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_MSK  0x00000002
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_OVRUN_I_OFF  1
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_MSK 0x00000001
#define GSUP43_C73_REG_TX_INT_INDICATION_BIT_FIFO_UNDRUN_I_OFF 0

/*-----------------------------.
 | Register 0x00000064 TX_STAT |
 +-----------------------------+
 | bit  4 R  SEND_IDLE_V       |
 | bit  3 R  SEND_LF_V         |
 | bit  2 R  SEND_RF_V         |
 +----------------------------*/
#define PMC_GSUP43_C73_REG_TX_STAT_UNUSED_MASK     0xffffffe3
#define GSUP43_C73_REG_TX_STAT_BIT_SEND_IDLE_V_MSK 0x00000010
#define GSUP43_C73_REG_TX_STAT_BIT_SEND_IDLE_V_OFF 4
#define GSUP43_C73_REG_TX_STAT_BIT_SEND_LF_V_MSK   0x00000008
#define GSUP43_C73_REG_TX_STAT_BIT_SEND_LF_V_OFF   3
#define GSUP43_C73_REG_TX_STAT_BIT_SEND_RF_V_MSK   0x00000004
#define GSUP43_C73_REG_TX_STAT_BIT_SEND_RF_V_OFF   2

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GSUP43_C73_REGS_H */
