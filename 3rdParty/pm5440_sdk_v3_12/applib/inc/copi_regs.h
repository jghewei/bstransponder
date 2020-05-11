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
 *     copi block
 *****************************************************************************/
#ifndef _COPI_REGS_H
#define _COPI_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_COPI_REG_SOFT_RESET              0x00004000
#define PMC_COPI_REG_CFG                     0x00004004
#define PMC_COPI_REG_TX_INTF_CFG             0x00004008
#define PMC_COPI_REG_RX_INTF_CFG             0x00004010
#define PMC_COPI_REG_TX_FIFO_TH( N )         (0x00004014 + (N) * 0x4)
#define PMC_COPI_REG_RX_FIFO_TH              0x0000401c
#define PMC_COPI_REG_TX_FLOW_CTRL_CFG        0x00004020
#define PMC_COPI_REG_RX_FLOW_CTRL_CFG        0x00004024
#define PMC_COPI_REG_DPI_LB                  0x00004038
#define PMC_COPI_REG_TOP_SUMMARY             0x00004130
#define PMC_COPI_REG_TX_FIFO_INT_E0          0x00004134
#define PMC_COPI_REG_TX_FIFO_INT0            0x0000413c
#define PMC_COPI_REG_RX_FIFO_INT_E0          0x00004140
#define PMC_COPI_REG_RX_FIFO_INT0            0x00004148
#define PMC_COPI_REG_SERDES_RX_LOS_INT_E0    0x00004164
#define PMC_COPI_REG_SERDES_RX_LOS_INT0      0x0000416c
#define PMC_COPI_REG_TX_INTF_ERR_INT_E0( N ) (0x00004170 + (N) * 0x4)
#define PMC_COPI_REG_TX_INTF_ERR_INT0( N )   (0x00004180 + (N) * 0x4)
#define PMC_COPI_REG_RX_INTF_ERR_INT_E0      0x00004188
#define PMC_COPI_REG_RX_INTF_ERR_INT0        0x00004190
#define PMC_COPI_REG_COM_INT_E0              0x000041ac
#define PMC_COPI_REG_TIP_INT0                0x000041b4
#define PMC_COPI_REG_TX_ERR_CNT( N )         (0x000041b8 + (N) * 0x4)
#define PMC_COPI_REG_RX_ERR_CNT              0x000041c0
#define PMC_COPI_REG_TX_DIAG_IN_CNT( N )     (0x000041c4 + (N) * 0x4)
#define PMC_COPI_REG_RX_DIAG_CNT( N )        (0x000041cc + (N) * 0x4)
#define PMC_COPI_REG_TX_CPB_ARB_CH_LK( N )   (0x00004200 + (N) * 0x4)
#define PMC_COPI_REG_TX_ODUK_ARB_CH_LK( N )  (0x00004500 + (N) * 0x4)
#define PMC_COPI_REG_TX_ODUK_4B_LK( N )      (0x00004700 + (N) * 0x4)
#define PMC_COPI_REG_TX_ODUK_2B_LK( N )      (0x00004900 + (N) * 0x4)
#define PMC_COPI_REG_RX_ARB_CH_LK( N )       (0x00004b00 + (N) * 0x4)
#define PMC_COPI_REG_RX_ODUK_4B_LK( N )      (0x00004f00 + (N) * 0x4)
#define PMC_COPI_REG_RX_ODUK_2B_LK( N )      (0x00005100 + (N) * 0x4)
#define PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR     0x000052a0
#define PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR    0x000052a4
#define PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0   0x000052a8
#define PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR1   0x000052ac

/*-----------------------------------.
 | Register 0x00004000 SOFT_RESET    |
 +-----------------------------------+
 | bit  18 R/W  OOB_LOWPWR_EN        |
 | bit  17 R/W  ODUK_LOWPWR_EN       |
 | bit  16 R/W  CPB_LOWPWR_EN        |
 | bit  6  R/W  ODUK_TX_SOFT         |
 | bit  5  R/W  CPB_TX_SOFT          |
 | bit  4  R/W  IL_SOFT_RST_RX_FC_X2 |
 | bit  3  R/W  IL_RX_SOFT           |
 | bit  2  R/W  IL_TX_SOFT           |
 | bit  1  R/W  ILAKEN_SOFT          |
 | bit  0  R/W  COPI_SOFT            |
 +----------------------------------*/
#define PMC_COPI_REG_SOFT_RESET_UNUSED_MASK              0xff00fe00
#define COPI_REG_SOFT_RESET_BIT_OOB_LOWPWR_EN_MSK        0x00040000
#define COPI_REG_SOFT_RESET_BIT_OOB_LOWPWR_EN_OFF        18
#define COPI_REG_SOFT_RESET_BIT_ODUK_LOWPWR_EN_MSK       0x00020000
#define COPI_REG_SOFT_RESET_BIT_ODUK_LOWPWR_EN_OFF       17
#define COPI_REG_SOFT_RESET_BIT_CPB_LOWPWR_EN_MSK        0x00010000
#define COPI_REG_SOFT_RESET_BIT_CPB_LOWPWR_EN_OFF        16
#define COPI_REG_SOFT_RESET_BIT_ODUK_TX_SOFT_MSK         0x00000040
#define COPI_REG_SOFT_RESET_BIT_ODUK_TX_SOFT_OFF         6
#define COPI_REG_SOFT_RESET_BIT_CPB_TX_SOFT_MSK          0x00000020
#define COPI_REG_SOFT_RESET_BIT_CPB_TX_SOFT_OFF          5
#define COPI_REG_SOFT_RESET_BIT_IL_SOFT_RST_RX_FC_X2_MSK 0x00000010
#define COPI_REG_SOFT_RESET_BIT_IL_SOFT_RST_RX_FC_X2_OFF 4
#define COPI_REG_SOFT_RESET_BIT_IL_RX_SOFT_MSK           0x00000008
#define COPI_REG_SOFT_RESET_BIT_IL_RX_SOFT_OFF           3
#define COPI_REG_SOFT_RESET_BIT_IL_TX_SOFT_MSK           0x00000004
#define COPI_REG_SOFT_RESET_BIT_IL_TX_SOFT_OFF           2
#define COPI_REG_SOFT_RESET_BIT_ILAKEN_SOFT_MSK          0x00000002
#define COPI_REG_SOFT_RESET_BIT_ILAKEN_SOFT_OFF          1
#define COPI_REG_SOFT_RESET_BIT_COPI_SOFT_MSK            0x00000001
#define COPI_REG_SOFT_RESET_BIT_COPI_SOFT_OFF            0

/*------------------------------.
 | Register 0x00004004 CFG      |
 +------------------------------+
 | bit  9:8 R/W  DIAG_CNTR_MODE |
 | bit  0   R/W  PMON_UPDATE    |
 +-----------------------------*/
#define PMC_COPI_REG_CFG_UNUSED_MASK        0xffffe0f8
#define COPI_REG_CFG_BIT_DIAG_CNTR_MODE_MSK 0x00000300
#define COPI_REG_CFG_BIT_DIAG_CNTR_MODE_OFF 8
#define COPI_REG_CFG_BIT_PMON_UPDATE_MSK    0x00000001
#define COPI_REG_CFG_BIT_PMON_UPDATE_OFF    0

/*--------------------------------------.
 | Register 0x00004008 TX_INTF_CFG      |
 +--------------------------------------+
 | bit  11:10 R/W  TX_ODUK_6B_INS_EN    |
 | bit  9     R/W  TX_ODUK_CHEXT_EN     |
 | bit  8:7   R/W  TX_ODUK_BURSTMAX     |
 | bit  6     R/W  TX_ODUK_INTERFACE_EN |
 | bit  5     R/W  TX_ODUK_PKT_MODE     |
 | bit  4     R/W  TX_CPB_TYPE_EN       |
 | bit  3:2   R/W  TX_CPB_BURSTMAX      |
 | bit  1     R/W  TX_CPB_INTERFACE_EN  |
 | bit  0     R/W  TX_CPB_PKT_MODE      |
 +-------------------------------------*/
#define PMC_COPI_REG_TX_INTF_CFG_UNUSED_MASK              0xfffff000
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_MSK    0x00000c00
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_6B_INS_EN_OFF    10
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_CHEXT_EN_MSK     0x00000200
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_CHEXT_EN_OFF     9
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_MSK     0x00000180
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_BURSTMAX_OFF     7
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_INTERFACE_EN_MSK 0x00000040
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_INTERFACE_EN_OFF 6
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_PKT_MODE_MSK     0x00000020
#define COPI_REG_TX_INTF_CFG_BIT_TX_ODUK_PKT_MODE_OFF     5
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_TYPE_EN_MSK       0x00000010
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_TYPE_EN_OFF       4
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_MSK      0x0000000c
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_BURSTMAX_OFF      2
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_INTERFACE_EN_MSK  0x00000002
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_INTERFACE_EN_OFF  1
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_PKT_MODE_MSK      0x00000001
#define COPI_REG_TX_INTF_CFG_BIT_TX_CPB_PKT_MODE_OFF      0

/*----------------------------------.
 | Register 0x00004010 RX_INTF_CFG  |
 +----------------------------------+
 | bit  18    R/W  RX_ODUK_2B_MODE  |
 | bit  17:16 R/W  RX_ODUK_6B_CHK   |
 | bit  9     R/W  RX_ODUK_CHEXT_EN |
 | bit  4     R/W  RX_CPB_TYPE_EN   |
 | bit  3:2   R/W  RX_BURSTMAX      |
 | bit  1     R/W  RX_INTERFACE_EN  |
 | bit  0     R/W  RX_PKT_MODE      |
 +---------------------------------*/
#define PMC_COPI_REG_RX_INTF_CFG_UNUSED_MASK          0xfff8f9c0
#define COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_2B_MODE_MSK  0x00040000
#define COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_2B_MODE_OFF  18
#define COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_MSK   0x00030000
#define COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_6B_CHK_OFF   16
#define COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_CHEXT_EN_MSK 0x00000200
#define COPI_REG_RX_INTF_CFG_BIT_RX_ODUK_CHEXT_EN_OFF 9
#define COPI_REG_RX_INTF_CFG_BIT_RX_CPB_TYPE_EN_MSK   0x00000010
#define COPI_REG_RX_INTF_CFG_BIT_RX_CPB_TYPE_EN_OFF   4
#define COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_MSK      0x0000000c
#define COPI_REG_RX_INTF_CFG_BIT_RX_BURSTMAX_OFF      2
#define COPI_REG_RX_INTF_CFG_BIT_RX_INTERFACE_EN_MSK  0x00000002
#define COPI_REG_RX_INTF_CFG_BIT_RX_INTERFACE_EN_OFF  1
#define COPI_REG_RX_INTF_CFG_BIT_RX_PKT_MODE_MSK      0x00000001
#define COPI_REG_RX_INTF_CFG_BIT_RX_PKT_MODE_OFF      0

/* index definitions for PMC_COPI_REG_TX_FIFO_TH */
#define PMC_COPI_REG_TX_FIFO_TH_INDEX_N_MIN    0
#define PMC_COPI_REG_TX_FIFO_TH_INDEX_N_MAX    1
#define PMC_COPI_REG_TX_FIFO_TH_INDEX_N_SIZE   2
#define PMC_COPI_REG_TX_FIFO_TH_INDEX_N_OFFSET 0x4

/*----------------------------------------------.
 | Register (0x00004014 + (N) * 0x4) TX_FIFO_TH |
 +----------------------------------------------+
 | bit  22:16 R/W  TX_H_WM                      |
 | bit  6:0   R/W  TX_L_WM                      |
 +---------------------------------------------*/
#define PMC_COPI_REG_TX_FIFO_TH_UNUSED_MASK    0xff80ff80
#define COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_MSK    0x007f0000
#define COPI_REG_TX_FIFO_TH_BIT_TX_H_WM_OFF    16
#define COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_MSK    0x0000007f
#define COPI_REG_TX_FIFO_TH_BIT_TX_L_WM_OFF    0

/*--------------------------------.
 | Register 0x0000401c RX_FIFO_TH |
 +--------------------------------+
 | bit  24:16 R/W  RX_H_WM        |
 | bit  8:0   R/W  RX_L_WM        |
 +-------------------------------*/
#define PMC_COPI_REG_RX_FIFO_TH_UNUSED_MASK 0xfe00fe00
#define COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_MSK 0x01ff0000
#define COPI_REG_RX_FIFO_TH_BIT_RX_H_WM_OFF 16
#define COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_MSK 0x000001ff
#define COPI_REG_RX_FIFO_TH_BIT_RX_L_WM_OFF 0

/*--------------------------------------.
 | Register 0x00004020 TX_FLOW_CTRL_CFG |
 +--------------------------------------+
 | bit  17 R/W  TX_ODUK_CH_FC_EN        |
 | bit  16 R/W  TX_ODUK_LINK_FC_EN      |
 | bit  5  R/W  TX_CPB_CH_FC_EN         |
 | bit  1  R/W  TX_OOB_EN               |
 | bit  0  R/W  TX_IB_EN                |
 +-------------------------------------*/
#define PMC_COPI_REG_TX_FLOW_CTRL_CFG_UNUSED_MASK            0xffe0fe0c
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_CH_FC_EN_MSK   0x00020000
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_CH_FC_EN_OFF   17
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_LINK_FC_EN_MSK 0x00010000
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_ODUK_LINK_FC_EN_OFF 16
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_CPB_CH_FC_EN_MSK    0x00000020
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_CPB_CH_FC_EN_OFF    5
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_OOB_EN_MSK          0x00000002
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_OOB_EN_OFF          1
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_IB_EN_MSK           0x00000001
#define COPI_REG_TX_FLOW_CTRL_CFG_BIT_TX_IB_EN_OFF           0

/*--------------------------------------.
 | Register 0x00004024 RX_FLOW_CTRL_CFG |
 +--------------------------------------+
 | bit  21:20 R/W  RX_ODUK_ZONE_HI_TH   |
 | bit  19:18 R/W  RX_ODUK_ZONE_LW_TH   |
 | bit  16    R/W  RX_ODUK_CH_FC_EN     |
 | bit  9:8   R/W  RX_CPB_ZONE_HI_TH    |
 | bit  7:6   R/W  RX_CPB_ZONE_LW_TH    |
 | bit  5     R/W  RX_LINK_FC_EN        |
 | bit  4     R/W  RX_CPB_CH_FC_EN      |
 | bit  1     R/W  RX_OOB_EN            |
 | bit  0     R/W  RX_IB_EN             |
 +-------------------------------------*/
#define PMC_COPI_REG_RX_FLOW_CTRL_CFG_UNUSED_MASK            0xffc2f00c
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_MSK 0x00300000
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_HI_TH_OFF 20
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_MSK 0x000c0000
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_ZONE_LW_TH_OFF 18
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_CH_FC_EN_MSK   0x00010000
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_ODUK_CH_FC_EN_OFF   16
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_MSK  0x00000300
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_HI_TH_OFF  8
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_MSK  0x000000c0
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_ZONE_LW_TH_OFF  6
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_LINK_FC_EN_MSK      0x00000020
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_LINK_FC_EN_OFF      5
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_CH_FC_EN_MSK    0x00000010
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_CPB_CH_FC_EN_OFF    4
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_OOB_EN_MSK          0x00000002
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_OOB_EN_OFF          1
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_IB_EN_MSK           0x00000001
#define COPI_REG_RX_FLOW_CTRL_CFG_BIT_RX_IB_EN_OFF           0

/*---------------------------------.
 | Register 0x00004038 DPI_LB      |
 +---------------------------------+
 | bit  11 R/W  DPI_FC_EXT_ODUK_LB |
 | bit  10 R/W  DPI_FC_EXT_CPB_LB  |
 | bit  9  R/W  DPI_FC_INT_ODUK_LB |
 | bit  8  R/W  DPI_FC_INT_CPB_LB  |
 | bit  3  R/W  DPI_EXT_ODUK_LB    |
 | bit  2  R/W  DPI_EXT_CPB_LB     |
 | bit  1  R/W  DPI_INT_ODUK_LB    |
 | bit  0  R/W  DPI_INT_CPB_LB     |
 +--------------------------------*/
#define PMC_COPI_REG_DPI_LB_UNUSED_MASK            0xfffff0f0
#define COPI_REG_DPI_LB_BIT_DPI_FC_EXT_ODUK_LB_MSK 0x00000800
#define COPI_REG_DPI_LB_BIT_DPI_FC_EXT_ODUK_LB_OFF 11
#define COPI_REG_DPI_LB_BIT_DPI_FC_EXT_CPB_LB_MSK  0x00000400
#define COPI_REG_DPI_LB_BIT_DPI_FC_EXT_CPB_LB_OFF  10
#define COPI_REG_DPI_LB_BIT_DPI_FC_INT_ODUK_LB_MSK 0x00000200
#define COPI_REG_DPI_LB_BIT_DPI_FC_INT_ODUK_LB_OFF 9
#define COPI_REG_DPI_LB_BIT_DPI_FC_INT_CPB_LB_MSK  0x00000100
#define COPI_REG_DPI_LB_BIT_DPI_FC_INT_CPB_LB_OFF  8
#define COPI_REG_DPI_LB_BIT_DPI_EXT_ODUK_LB_MSK    0x00000008
#define COPI_REG_DPI_LB_BIT_DPI_EXT_ODUK_LB_OFF    3
#define COPI_REG_DPI_LB_BIT_DPI_EXT_CPB_LB_MSK     0x00000004
#define COPI_REG_DPI_LB_BIT_DPI_EXT_CPB_LB_OFF     2
#define COPI_REG_DPI_LB_BIT_DPI_INT_ODUK_LB_MSK    0x00000002
#define COPI_REG_DPI_LB_BIT_DPI_INT_ODUK_LB_OFF    1
#define COPI_REG_DPI_LB_BIT_DPI_INT_CPB_LB_MSK     0x00000001
#define COPI_REG_DPI_LB_BIT_DPI_INT_CPB_LB_OFF     0

/*---------------------------------.
 | Register 0x00004130 TOP_SUMMARY |
 +---------------------------------+
 | bit  10  R  TIP_INT0            |
 | bit  7   R  RX_INTF_ERR_INT0    |
 | bit  6:5 R  TX_INTF_ERR_INT0    |
 | bit  4   R  SERDES_RX_LOS_INT0  |
 | bit  1   R  RX_FIFO_INT0        |
 | bit  0   R  TX_FIFO_INT0        |
 +--------------------------------*/
#define PMC_COPI_REG_TOP_SUMMARY_UNUSED_MASK            0xfffff800
#define COPI_REG_TOP_SUMMARY_BIT_TIP_INT0_MSK           0x00000400
#define COPI_REG_TOP_SUMMARY_BIT_TIP_INT0_OFF           10
#define COPI_REG_TOP_SUMMARY_BIT_RX_INTF_ERR_INT0_MSK   0x00000080
#define COPI_REG_TOP_SUMMARY_BIT_RX_INTF_ERR_INT0_OFF   7
#define COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_MSK   0x00000060
#define COPI_REG_TOP_SUMMARY_BIT_TX_INTF_ERR_INT0_OFF   5
#define COPI_REG_TOP_SUMMARY_BIT_SERDES_RX_LOS_INT0_MSK 0x00000010
#define COPI_REG_TOP_SUMMARY_BIT_SERDES_RX_LOS_INT0_OFF 4
#define COPI_REG_TOP_SUMMARY_BIT_RX_FIFO_INT0_MSK       0x00000002
#define COPI_REG_TOP_SUMMARY_BIT_RX_FIFO_INT0_OFF       1
#define COPI_REG_TOP_SUMMARY_BIT_TX_FIFO_INT0_MSK       0x00000001
#define COPI_REG_TOP_SUMMARY_BIT_TX_FIFO_INT0_OFF       0

/*------------------------------------------.
 | Register 0x00004134 TX_FIFO_INT_E0       |
 +------------------------------------------+
 | bit  29:28 R/W  ODUK_TX_BUF_ALIGN_INT_E0 |
 | bit  23:20 R/W  ODUK_TX_BUF_UN_INT_E0    |
 | bit  13:12 R/W  CPB_TX_BUF_ALIGN_INT_E0  |
 | bit  7:4   R/W  CPB_TX_BUF_UN_INT_E0     |
 +-----------------------------------------*/
#define PMC_COPI_REG_TX_FIFO_INT_E0_UNUSED_MASK                  0xc000c000
#define COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_MSK 0x30000000
#define COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_ALIGN_INT_E0_OFF 28
#define COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_MSK    0x00f00000
#define COPI_REG_TX_FIFO_INT_E0_BIT_ODUK_TX_BUF_UN_INT_E0_OFF    20
#define COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_MSK  0x00003000
#define COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_ALIGN_INT_E0_OFF  12
#define COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_MSK     0x000000f0
#define COPI_REG_TX_FIFO_INT_E0_BIT_CPB_TX_BUF_UN_INT_E0_OFF     4

/*------------------------------------------.
 | Register 0x0000413c TX_FIFO_INT0         |
 +------------------------------------------+
 | bit  29:28 R/W  ODUK_TX_BUF_ALIGN_INT_I0 |
 | bit  23:20 R/W  ODUK_TX_BUF_UN_INT_I0    |
 | bit  13:12 R/W  CPB_TX_BUF_ALIGN_INT_I0  |
 | bit  7:4   R/W  CPB_TX_BUF_UN_INT_I0     |
 +-----------------------------------------*/
#define PMC_COPI_REG_TX_FIFO_INT0_UNUSED_MASK                  0xc000c000
#define COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_MSK 0x30000000
#define COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_ALIGN_INT_I0_OFF 28
#define COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_MSK    0x00f00000
#define COPI_REG_TX_FIFO_INT0_BIT_ODUK_TX_BUF_UN_INT_I0_OFF    20
#define COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_MSK  0x00003000
#define COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_ALIGN_INT_I0_OFF  12
#define COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_MSK     0x000000f0
#define COPI_REG_TX_FIFO_INT0_BIT_CPB_TX_BUF_UN_INT_I0_OFF     4

/*------------------------------------.
 | Register 0x00004140 RX_FIFO_INT_E0 |
 +------------------------------------+
 | bit  10:9 R/W  RX_BUF_ALIGN_INT_E0 |
 | bit  5:3  R/W  RX_BUF_UN_INT_E0    |
 +-----------------------------------*/
#define PMC_COPI_REG_RX_FIFO_INT_E0_UNUSED_MASK             0xfffff800
#define COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_MSK 0x00000600
#define COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_ALIGN_INT_E0_OFF 9
#define COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_MSK    0x00000038
#define COPI_REG_RX_FIFO_INT_E0_BIT_RX_BUF_UN_INT_E0_OFF    3

/*------------------------------------.
 | Register 0x00004148 RX_FIFO_INT0   |
 +------------------------------------+
 | bit  10:9 R/W  RX_BUF_ALIGN_INT_I0 |
 | bit  5:3  R/W  RX_BUF_UN_INT_I0    |
 +-----------------------------------*/
#define PMC_COPI_REG_RX_FIFO_INT0_UNUSED_MASK             0xfffff800
#define COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_MSK 0x00000600
#define COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_ALIGN_INT_I0_OFF 9
#define COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_MSK    0x00000038
#define COPI_REG_RX_FIFO_INT0_BIT_RX_BUF_UN_INT_I0_OFF    3

/*------------------------------------------.
 | Register 0x00004164 SERDES_RX_LOS_INT_E0 |
 +------------------------------------------+
 | bit  23:0 R/W  SER_RX_LOS_INT_E0         |
 +-----------------------------------------*/
#define PMC_COPI_REG_SERDES_RX_LOS_INT_E0_UNUSED_MASK           0xff000000
#define COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_MSK 0x00ffffff
#define COPI_REG_SERDES_RX_LOS_INT_E0_BIT_SER_RX_LOS_INT_E0_OFF 0

/*----------------------------------------.
 | Register 0x0000416c SERDES_RX_LOS_INT0 |
 +----------------------------------------+
 | bit  23:0 R/W  SER_RX_LOS_INT_I0       |
 +---------------------------------------*/
#define PMC_COPI_REG_SERDES_RX_LOS_INT0_UNUSED_MASK           0xff000000
#define COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_MSK 0x00ffffff
#define COPI_REG_SERDES_RX_LOS_INT0_BIT_SER_RX_LOS_INT_I0_OFF 0

/* index definitions for PMC_COPI_REG_TX_INTF_ERR_INT_E0 */
#define PMC_COPI_REG_TX_INTF_ERR_INT_E0_INDEX_N_MIN                     0
#define PMC_COPI_REG_TX_INTF_ERR_INT_E0_INDEX_N_MAX                     1
#define PMC_COPI_REG_TX_INTF_ERR_INT_E0_INDEX_N_SIZE                    2
#define PMC_COPI_REG_TX_INTF_ERR_INT_E0_INDEX_N_OFFSET                  0x4

/*------------------------------------------------------.
 | Register (0x00004170 + (N) * 0x4) TX_INTF_ERR_INT_E0 |
 +------------------------------------------------------+
 | bit  7 R/W  TX_EOP_ERR_INT_E0                        |
 | bit  6 R/W  TX_SOP_ERR_INT_E0                        |
 | bit  5 R/W  TX_EOB_ERR_INT_E0                        |
 | bit  3 R/W  TX_PKT_CH_CHANGE_ERR_INT_E0              |
 | bit  2 R/W  TX_BURST_DROP_ERR_INT_E0                 |
 | bit  1 R/W  TX_BURST_LEN_ERR_INT_E0                  |
 | bit  0 R/W  TX_SOB_ERR_INT_E0                        |
 +-----------------------------------------------------*/
#define PMC_COPI_REG_TX_INTF_ERR_INT_E0_UNUSED_MASK                     0xfffefe10
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOP_ERR_INT_E0_MSK           0x00000080
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOP_ERR_INT_E0_OFF           7
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOP_ERR_INT_E0_MSK           0x00000040
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOP_ERR_INT_E0_OFF           6
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOB_ERR_INT_E0_MSK           0x00000020
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_EOB_ERR_INT_E0_OFF           5
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_PKT_CH_CHANGE_ERR_INT_E0_MSK 0x00000008
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_PKT_CH_CHANGE_ERR_INT_E0_OFF 3
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_DROP_ERR_INT_E0_MSK    0x00000004
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_DROP_ERR_INT_E0_OFF    2
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_LEN_ERR_INT_E0_MSK     0x00000002
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_BURST_LEN_ERR_INT_E0_OFF     1
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOB_ERR_INT_E0_MSK           0x00000001
#define COPI_REG_TX_INTF_ERR_INT_E0_BIT_TX_SOB_ERR_INT_E0_OFF           0

/* index definitions for PMC_COPI_REG_TX_INTF_ERR_INT0 */
#define PMC_COPI_REG_TX_INTF_ERR_INT0_INDEX_N_MIN                     0
#define PMC_COPI_REG_TX_INTF_ERR_INT0_INDEX_N_MAX                     1
#define PMC_COPI_REG_TX_INTF_ERR_INT0_INDEX_N_SIZE                    2
#define PMC_COPI_REG_TX_INTF_ERR_INT0_INDEX_N_OFFSET                  0x4

/*----------------------------------------------------.
 | Register (0x00004180 + (N) * 0x4) TX_INTF_ERR_INT0 |
 +----------------------------------------------------+
 | bit  7 R/W  TX_EOP_ERR_INT_I0                      |
 | bit  6 R/W  TX_SOP_ERR_INT_I0                      |
 | bit  5 R/W  TX_EOB_ERR_INT_I0                      |
 | bit  3 R/W  TX_PKT_CH_CHANGE_ERR_INT_I0            |
 | bit  2 R/W  TX_BURST_DROP_ERR_INT_I0               |
 | bit  1 R/W  TX_BURST_LEN_ERR_INT_I0                |
 | bit  0 R/W  TX_SOB_ERR_INT_I0                      |
 +---------------------------------------------------*/
#define PMC_COPI_REG_TX_INTF_ERR_INT0_UNUSED_MASK                     0xfffefe10
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_MSK           0x00000080
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOP_ERR_INT_I0_OFF           7
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_MSK           0x00000040
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOP_ERR_INT_I0_OFF           6
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_MSK           0x00000020
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_EOB_ERR_INT_I0_OFF           5
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_MSK 0x00000008
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_PKT_CH_CHANGE_ERR_INT_I0_OFF 3
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_MSK    0x00000004
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_DROP_ERR_INT_I0_OFF    2
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_MSK     0x00000002
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_BURST_LEN_ERR_INT_I0_OFF     1
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_MSK           0x00000001
#define COPI_REG_TX_INTF_ERR_INT0_BIT_TX_SOB_ERR_INT_I0_OFF           0

/*---------------------------------------------------.
 | Register 0x00004188 RX_INTF_ERR_INT_E0            |
 +---------------------------------------------------+
 | bit  19    R/W  RX_ODUK_H4B_CMPR_ERR_INT_E0       |
 | bit  18    R/W  RX_ODUK_H2B_CMPR_ERR_INT_E0       |
 | bit  17    R/W  RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0 |
 | bit  16    R/W  RX_ODUK_H2B_CAM_ERR_INT_E0        |
 | bit  11:10 R/W  RX_IL_DAT_ERR_INT_E0              |
 | bit  9     R/W  RX_IL_ALIGN_INT_E0                |
 | bit  8     R/W  RX_IL_ERR_ALL_INT_E0              |
 | bit  7     R/W  RX_EOP_ERR_INT_E0                 |
 | bit  6     R/W  RX_SOP_ERR_INT_E0                 |
 | bit  5     R/W  RX_EOB_ERR_INT_E0                 |
 | bit  3     R/W  RX_PKT_CH_CHANGE_ERR_INT_E0       |
 | bit  2     R/W  RX_BURST_DROP_ERR_INT_E0          |
 | bit  1     R/W  RX_BURST_LEN_ERR_INT_E0           |
 | bit  0     R/W  RX_SOB_ERR_INT_E0                 |
 +--------------------------------------------------*/
#define PMC_COPI_REG_RX_INTF_ERR_INT_E0_UNUSED_MASK                           0xfff0f010
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_E0_MSK       0x00080000
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_E0_OFF       19
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_E0_MSK       0x00040000
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_E0_OFF       18
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_MSK 0x00020000
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_OFF 17
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_ERR_INT_E0_MSK        0x00010000
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_ODUK_H2B_CAM_ERR_INT_E0_OFF        16
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_MSK              0x00000c00
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_DAT_ERR_INT_E0_OFF              10
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ALIGN_INT_E0_MSK                0x00000200
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ALIGN_INT_E0_OFF                9
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ERR_ALL_INT_E0_MSK              0x00000100
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_IL_ERR_ALL_INT_E0_OFF              8
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOP_ERR_INT_E0_MSK                 0x00000080
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOP_ERR_INT_E0_OFF                 7
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOP_ERR_INT_E0_MSK                 0x00000040
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOP_ERR_INT_E0_OFF                 6
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOB_ERR_INT_E0_MSK                 0x00000020
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_EOB_ERR_INT_E0_OFF                 5
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_PKT_CH_CHANGE_ERR_INT_E0_MSK       0x00000008
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_PKT_CH_CHANGE_ERR_INT_E0_OFF       3
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_DROP_ERR_INT_E0_MSK          0x00000004
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_DROP_ERR_INT_E0_OFF          2
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_LEN_ERR_INT_E0_MSK           0x00000002
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_BURST_LEN_ERR_INT_E0_OFF           1
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOB_ERR_INT_E0_MSK                 0x00000001
#define COPI_REG_RX_INTF_ERR_INT_E0_BIT_RX_SOB_ERR_INT_E0_OFF                 0

/*---------------------------------------------------.
 | Register 0x00004190 RX_INTF_ERR_INT0              |
 +---------------------------------------------------+
 | bit  19    R/W  RX_ODUK_H4B_CMPR_ERR_INT_I0       |
 | bit  18    R/W  RX_ODUK_H2B_CMPR_ERR_INT_I0       |
 | bit  17    R/W  RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0 |
 | bit  16    R/W  RX_ODUK_H2B_CAM_ERR_INT_I0        |
 | bit  11:10 R/W  RX_IL_DAT_ERR_INT_I0              |
 | bit  9     R/W  RX_IL_ALIGN_INT_I0                |
 | bit  8     R/W  RX_IL_ERR_ALL_INT_I0              |
 | bit  7     R/W  RX_EOP_ERR_INT_I0                 |
 | bit  6     R/W  RX_SOP_ERR_INT_I0                 |
 | bit  5     R/W  RX_EOB_ERR_INT_I0                 |
 | bit  3     R/W  RX_PKT_CH_CHANGE_ERR_INT_I0       |
 | bit  2     R/W  RX_BURST_DROP_ERR_INT_I0          |
 | bit  1     R/W  RX_BURST_LEN_ERR_INT_I0           |
 | bit  0     R/W  RX_SOB_ERR_INT_I0                 |
 +--------------------------------------------------*/
#define PMC_COPI_REG_RX_INTF_ERR_INT0_UNUSED_MASK                           0xfff0f010
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_MSK       0x00080000
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H4B_CMPR_ERR_INT_I0_OFF       19
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_MSK       0x00040000
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CMPR_ERR_INT_I0_OFF       18
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_MSK 0x00020000
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_OFF 17
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_MSK        0x00010000
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_ODUK_H2B_CAM_ERR_INT_I0_OFF        16
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_MSK              0x00000c00
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_DAT_ERR_INT_I0_OFF              10
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_MSK                0x00000200
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ALIGN_INT_I0_OFF                9
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_MSK              0x00000100
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_IL_ERR_ALL_INT_I0_OFF              8
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_MSK                 0x00000080
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOP_ERR_INT_I0_OFF                 7
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_MSK                 0x00000040
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOP_ERR_INT_I0_OFF                 6
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_MSK                 0x00000020
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_EOB_ERR_INT_I0_OFF                 5
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_MSK       0x00000008
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_PKT_CH_CHANGE_ERR_INT_I0_OFF       3
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_MSK          0x00000004
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_DROP_ERR_INT_I0_OFF          2
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_MSK           0x00000002
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_BURST_LEN_ERR_INT_I0_OFF           1
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_MSK                 0x00000001
#define COPI_REG_RX_INTF_ERR_INT0_BIT_RX_SOB_ERR_INT_I0_OFF                 0

/*--------------------------------.
 | Register 0x000041ac COM_INT_E0 |
 +--------------------------------+
 | bit  0 R/W  TIP_INT_E0         |
 +-------------------------------*/
#define PMC_COPI_REG_COM_INT_E0_UNUSED_MASK    0xfffffffe
#define COPI_REG_COM_INT_E0_BIT_TIP_INT_E0_MSK 0x00000001
#define COPI_REG_COM_INT_E0_BIT_TIP_INT_E0_OFF 0

/*------------------------------.
 | Register 0x000041b4 TIP_INT0 |
 +------------------------------+
 | bit  0 R/W  TIP_INT_I0       |
 +-----------------------------*/
#define PMC_COPI_REG_TIP_INT0_UNUSED_MASK    0xfffffffe
#define COPI_REG_TIP_INT0_BIT_TIP_INT_I0_MSK 0x00000001
#define COPI_REG_TIP_INT0_BIT_TIP_INT_I0_OFF 0

/* index definitions for PMC_COPI_REG_TX_ERR_CNT */
#define PMC_COPI_REG_TX_ERR_CNT_INDEX_N_MIN    0
#define PMC_COPI_REG_TX_ERR_CNT_INDEX_N_MAX    1
#define PMC_COPI_REG_TX_ERR_CNT_INDEX_N_SIZE   2
#define PMC_COPI_REG_TX_ERR_CNT_INDEX_N_OFFSET 0x4

/*----------------------------------------------.
 | Register (0x000041b8 + (N) * 0x4) TX_ERR_CNT |
 +----------------------------------------------+
 | bit  31:0 R  TX_ERR_CNT                      |
 +---------------------------------------------*/
#define PMC_COPI_REG_TX_ERR_CNT_UNUSED_MASK    0x00000000
#define COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_MSK 0xffffffff
#define COPI_REG_TX_ERR_CNT_BIT_TX_ERR_CNT_OFF 0

/*--------------------------------.
 | Register 0x000041c0 RX_ERR_CNT |
 +--------------------------------+
 | bit  31:0 R  RX_ERR_CNT        |
 +-------------------------------*/
#define PMC_COPI_REG_RX_ERR_CNT_UNUSED_MASK    0x00000000
#define COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_MSK 0xffffffff
#define COPI_REG_RX_ERR_CNT_BIT_RX_ERR_CNT_OFF 0

/* index definitions for PMC_COPI_REG_TX_DIAG_IN_CNT */
#define PMC_COPI_REG_TX_DIAG_IN_CNT_INDEX_N_MIN     0
#define PMC_COPI_REG_TX_DIAG_IN_CNT_INDEX_N_MAX     1
#define PMC_COPI_REG_TX_DIAG_IN_CNT_INDEX_N_SIZE    2
#define PMC_COPI_REG_TX_DIAG_IN_CNT_INDEX_N_OFFSET  0x4

/*--------------------------------------------------.
 | Register (0x000041c4 + (N) * 0x4) TX_DIAG_IN_CNT |
 +--------------------------------------------------+
 | bit  31:0 R  TX_DIAG_CNT                         |
 +-------------------------------------------------*/
#define PMC_COPI_REG_TX_DIAG_IN_CNT_UNUSED_MASK     0x00000000
#define COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_MSK 0xffffffff
#define COPI_REG_TX_DIAG_IN_CNT_BIT_TX_DIAG_CNT_OFF 0

/* index definitions for PMC_COPI_REG_RX_DIAG_CNT */
#define PMC_COPI_REG_RX_DIAG_CNT_INDEX_N_MIN        0
#define PMC_COPI_REG_RX_DIAG_CNT_INDEX_N_MAX        1
#define PMC_COPI_REG_RX_DIAG_CNT_INDEX_N_SIZE       2
#define PMC_COPI_REG_RX_DIAG_CNT_INDEX_N_OFFSET     0x4

/*-----------------------------------------------.
 | Register (0x000041cc + (N) * 0x4) RX_DIAG_CNT |
 +-----------------------------------------------+
 | bit  31:0 R  RX_DIAG_IN_CNT                   |
 +----------------------------------------------*/
#define PMC_COPI_REG_RX_DIAG_CNT_UNUSED_MASK        0x00000000
#define COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_MSK 0xffffffff
#define COPI_REG_RX_DIAG_CNT_BIT_RX_DIAG_IN_CNT_OFF 0

/* index definitions for PMC_COPI_REG_TX_CPB_ARB_CH_LK */
#define PMC_COPI_REG_TX_CPB_ARB_CH_LK_INDEX_N_MIN       0
#define PMC_COPI_REG_TX_CPB_ARB_CH_LK_INDEX_N_MAX       149
#define PMC_COPI_REG_TX_CPB_ARB_CH_LK_INDEX_N_SIZE      150
#define PMC_COPI_REG_TX_CPB_ARB_CH_LK_INDEX_N_OFFSET    0x4

/*----------------------------------------------------.
 | Register (0x00004200 + (N) * 0x4) TX_CPB_ARB_CH_LK |
 +----------------------------------------------------+
 | bit  7:0 R/W  TX_CPB_ARB_CH                        |
 +---------------------------------------------------*/
#define PMC_COPI_REG_TX_CPB_ARB_CH_LK_UNUSED_MASK       0xffffff00
#define COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_MSK 0x000000ff
#define COPI_REG_TX_CPB_ARB_CH_LK_BIT_TX_CPB_ARB_CH_OFF 0

/* index definitions for PMC_COPI_REG_TX_ODUK_ARB_CH_LK */
#define PMC_COPI_REG_TX_ODUK_ARB_CH_LK_INDEX_N_MIN        0
#define PMC_COPI_REG_TX_ODUK_ARB_CH_LK_INDEX_N_MAX        103
#define PMC_COPI_REG_TX_ODUK_ARB_CH_LK_INDEX_N_SIZE       104
#define PMC_COPI_REG_TX_ODUK_ARB_CH_LK_INDEX_N_OFFSET     0x4

/*-----------------------------------------------------.
 | Register (0x00004500 + (N) * 0x4) TX_ODUK_ARB_CH_LK |
 +-----------------------------------------------------+
 | bit  7:0 R/W  TX_ODUK_ARB_CH                        |
 +----------------------------------------------------*/
#define PMC_COPI_REG_TX_ODUK_ARB_CH_LK_UNUSED_MASK        0xffffff00
#define COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_MSK 0x000000ff
#define COPI_REG_TX_ODUK_ARB_CH_LK_BIT_TX_ODUK_ARB_CH_OFF 0

/* index definitions for PMC_COPI_REG_TX_ODUK_4B_LK */
#define PMC_COPI_REG_TX_ODUK_4B_LK_INDEX_N_MIN       0
#define PMC_COPI_REG_TX_ODUK_4B_LK_INDEX_N_MAX       103
#define PMC_COPI_REG_TX_ODUK_4B_LK_INDEX_N_SIZE      104
#define PMC_COPI_REG_TX_ODUK_4B_LK_INDEX_N_OFFSET    0x4

/*-------------------------------------------------.
 | Register (0x00004700 + (N) * 0x4) TX_ODUK_4B_LK |
 +-------------------------------------------------+
 | bit  31:0 R/W  TX_ODUK_4B_LK                    |
 +------------------------------------------------*/
#define PMC_COPI_REG_TX_ODUK_4B_LK_UNUSED_MASK       0x00000000
#define COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_MSK 0xffffffff
#define COPI_REG_TX_ODUK_4B_LK_BIT_TX_ODUK_4B_LK_OFF 0

/* index definitions for PMC_COPI_REG_TX_ODUK_2B_LK */
#define PMC_COPI_REG_TX_ODUK_2B_LK_INDEX_N_MIN       0
#define PMC_COPI_REG_TX_ODUK_2B_LK_INDEX_N_MAX       103
#define PMC_COPI_REG_TX_ODUK_2B_LK_INDEX_N_SIZE      104
#define PMC_COPI_REG_TX_ODUK_2B_LK_INDEX_N_OFFSET    0x4

/*-------------------------------------------------.
 | Register (0x00004900 + (N) * 0x4) TX_ODUK_2B_LK |
 +-------------------------------------------------+
 | bit  15:0 R/W  TX_ODUK_2B_LK                    |
 +------------------------------------------------*/
#define PMC_COPI_REG_TX_ODUK_2B_LK_UNUSED_MASK       0xffff0000
#define COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_MSK 0x0000ffff
#define COPI_REG_TX_ODUK_2B_LK_BIT_TX_ODUK_2B_LK_OFF 0

/* index definitions for PMC_COPI_REG_RX_ARB_CH_LK */
#define PMC_COPI_REG_RX_ARB_CH_LK_INDEX_N_MIN              0
#define PMC_COPI_REG_RX_ARB_CH_LK_INDEX_N_MAX              253
#define PMC_COPI_REG_RX_ARB_CH_LK_INDEX_N_SIZE             254
#define PMC_COPI_REG_RX_ARB_CH_LK_INDEX_N_OFFSET           0x4

/*------------------------------------------------.
 | Register (0x00004b00 + (N) * 0x4) RX_ARB_CH_LK |
 +------------------------------------------------+
 | bit  8   R/W  RX_CPB_ODUK_PATH_SEL             |
 | bit  7:0 R/W  RX_ARB_CH                        |
 +-----------------------------------------------*/
#define PMC_COPI_REG_RX_ARB_CH_LK_UNUSED_MASK              0xfffffe00
#define COPI_REG_RX_ARB_CH_LK_BIT_RX_CPB_ODUK_PATH_SEL_MSK 0x00000100
#define COPI_REG_RX_ARB_CH_LK_BIT_RX_CPB_ODUK_PATH_SEL_OFF 8
#define COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_MSK            0x000000ff
#define COPI_REG_RX_ARB_CH_LK_BIT_RX_ARB_CH_OFF            0

/* index definitions for PMC_COPI_REG_RX_ODUK_4B_LK */
#define PMC_COPI_REG_RX_ODUK_4B_LK_INDEX_N_MIN       0
#define PMC_COPI_REG_RX_ODUK_4B_LK_INDEX_N_MAX       103
#define PMC_COPI_REG_RX_ODUK_4B_LK_INDEX_N_SIZE      104
#define PMC_COPI_REG_RX_ODUK_4B_LK_INDEX_N_OFFSET    0x4

/*-------------------------------------------------.
 | Register (0x00004f00 + (N) * 0x4) RX_ODUK_4B_LK |
 +-------------------------------------------------+
 | bit  31:0 R/W  RX_ODUK_4B_LK                    |
 +------------------------------------------------*/
#define PMC_COPI_REG_RX_ODUK_4B_LK_UNUSED_MASK       0x00000000
#define COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_MSK 0xffffffff
#define COPI_REG_RX_ODUK_4B_LK_BIT_RX_ODUK_4B_LK_OFF 0

/* index definitions for PMC_COPI_REG_RX_ODUK_2B_LK */
#define PMC_COPI_REG_RX_ODUK_2B_LK_INDEX_N_MIN       0
#define PMC_COPI_REG_RX_ODUK_2B_LK_INDEX_N_MAX       103
#define PMC_COPI_REG_RX_ODUK_2B_LK_INDEX_N_SIZE      104
#define PMC_COPI_REG_RX_ODUK_2B_LK_INDEX_N_OFFSET    0x4

/*-------------------------------------------------.
 | Register (0x00005100 + (N) * 0x4) RX_ODUK_2B_LK |
 +-------------------------------------------------+
 | bit  15:0 R/W  RX_ODUK_2B_LK                    |
 +------------------------------------------------*/
#define PMC_COPI_REG_RX_ODUK_2B_LK_UNUSED_MASK       0xffff0000
#define COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_MSK 0x0000ffff
#define COPI_REG_RX_ODUK_2B_LK_BIT_RX_ODUK_2B_LK_OFF 0

/*-----------------------------------------.
 | Register 0x000052a0 RX_ODUK_H2B_CAM_ERR |
 +-----------------------------------------+
 | bit  31   R/W  RX_ODUK_H2B_CAM_ERR_SET  |
 | bit  23:0 R    RX_ODUK_H2B_CAM_ERR      |
 +----------------------------------------*/
#define PMC_COPI_REG_RX_ODUK_H2B_CAM_ERR_UNUSED_MASK                 0x7f000000
#define COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_SET_MSK 0x80000000
#define COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_SET_OFF 31
#define COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_MSK     0x00ffffff
#define COPI_REG_RX_ODUK_H2B_CAM_ERR_BIT_RX_ODUK_H2B_CAM_ERR_OFF     0

/*------------------------------------------.
 | Register 0x000052a4 RX_ODUK_H2B_CMPR_ERR |
 +------------------------------------------+
 | bit  31   R/W  RX_ODUK_H2B_CMPR_ERR_SET  |
 | bit  23:0 R    RX_ODUK_H2B_CMPR_ERR      |
 +-----------------------------------------*/
#define PMC_COPI_REG_RX_ODUK_H2B_CMPR_ERR_UNUSED_MASK                  0x7f000000
#define COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_SET_MSK 0x80000000
#define COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_SET_OFF 31
#define COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_MSK     0x00ffffff
#define COPI_REG_RX_ODUK_H2B_CMPR_ERR_BIT_RX_ODUK_H2B_CMPR_ERR_OFF     0

/*-------------------------------------------.
 | Register 0x000052a8 RX_ODUK_H4B_CMPR_ERR0 |
 +-------------------------------------------+
 | bit  31    R/W  RX_ODUK_H4B_CMPR_ERR_SET  |
 | bit  23:16 R    RX_ODUK_H4B_CMPR_ERR0     |
 +------------------------------------------*/
#define PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR0_UNUSED_MASK                  0x7f00ffff
#define COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR_SET_MSK 0x80000000
#define COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR_SET_OFF 31
#define COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_MSK    0x00ff0000
#define COPI_REG_RX_ODUK_H4B_CMPR_ERR0_BIT_RX_ODUK_H4B_CMPR_ERR0_OFF    16

/*-------------------------------------------.
 | Register 0x000052ac RX_ODUK_H4B_CMPR_ERR1 |
 +-------------------------------------------+
 | bit  31:0 R  RX_ODUK_H4B_CMPR_ERR1        |
 +------------------------------------------*/
#define PMC_COPI_REG_RX_ODUK_H4B_CMPR_ERR1_UNUSED_MASK               0x00000000
#define COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_MSK 0xffffffff
#define COPI_REG_RX_ODUK_H4B_CMPR_ERR1_BIT_RX_ODUK_H4B_CMPR_ERR1_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COPI_REGS_H */
