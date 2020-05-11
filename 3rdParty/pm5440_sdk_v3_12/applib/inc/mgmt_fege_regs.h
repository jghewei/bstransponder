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
 *     mgmt_fege block
 *****************************************************************************/
#ifndef _MGMT_FEGE_REGS_H
#define _MGMT_FEGE_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MGMT_FEGE_TOP_REG_SW_RESET                 0x00000000
#define PMC_MGMT_FEGE_TOP_REG_INT_EN                   0x00000010
#define PMC_MGMT_FEGE_TOP_REG_INT_STAT                 0x00000014
#define PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE       0x00000020
#define PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE       0x00000024
#define PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL 0x0000002c
#define PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT( N )         (0x00000100 + (N) * 0x4)
#define PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT( N )         (0x00000200 + (N) * 0x4)

/*--------------------------------.
 | Register 0x00000000 SW_RESET   |
 +--------------------------------+
 | bit  0 R/W  MGMT_FEGE_SW_RESET |
 +-------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_SW_RESET_UNUSED_MASK            0xfffffffe
#define MGMT_FEGE_TOP_REG_SW_RESET_BIT_MGMT_FEGE_SW_RESET_MSK 0x00000001
#define MGMT_FEGE_TOP_REG_SW_RESET_BIT_MGMT_FEGE_SW_RESET_OFF 0

/*------------------------------------.
 | Register 0x00000010 INT_EN         |
 +------------------------------------+
 | bit  5 R/W  RMVP_INVALID_TYPE_E    |
 | bit  4 R/W  RMVP_INVALID_VLAN_ID_E |
 | bit  3 R/W  RMVP_FIFO_OVERFLOW_E   |
 | bit  2 R/W  TMVP_INVALID_TYPE_E    |
 | bit  1 R/W  TMVP_INVALID_DPI_ID_E  |
 | bit  0 R/W  TMVP_FIFO_OVERFLOW_E   |
 +-----------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_INT_EN_UNUSED_MASK                0xffff00c0
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_TYPE_E_MSK    0x00000020
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_TYPE_E_OFF    5
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_VLAN_ID_E_MSK 0x00000010
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_INVALID_VLAN_ID_E_OFF 4
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_FIFO_OVERFLOW_E_MSK   0x00000008
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_RMVP_FIFO_OVERFLOW_E_OFF   3
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_TYPE_E_MSK    0x00000004
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_TYPE_E_OFF    2
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_DPI_ID_E_MSK  0x00000002
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_INVALID_DPI_ID_E_OFF  1
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_FIFO_OVERFLOW_E_MSK   0x00000001
#define MGMT_FEGE_TOP_REG_INT_EN_BIT_TMVP_FIFO_OVERFLOW_E_OFF   0

/*------------------------------------.
 | Register 0x00000014 INT_STAT       |
 +------------------------------------+
 | bit  5 R/W  RMVP_INVALID_TYPE_I    |
 | bit  4 R/W  RMVP_INVALID_VLAN_ID_I |
 | bit  3 R/W  RMVP_FIFO_OVERFLOW_I   |
 | bit  2 R/W  TMVP_INVALID_TYPE_I    |
 | bit  1 R/W  TMVP_INVALID_DPI_ID_I  |
 | bit  0 R/W  TMVP_FIFO_OVERFLOW_I   |
 +-----------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_INT_STAT_UNUSED_MASK                0xffff00c0
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_MSK    0x00000020
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_TYPE_I_OFF    5
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_MSK 0x00000010
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_INVALID_VLAN_ID_I_OFF 4
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_MSK   0x00000008
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_RMVP_FIFO_OVERFLOW_I_OFF   3
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_MSK    0x00000004
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_TYPE_I_OFF    2
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_MSK  0x00000002
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_INVALID_DPI_ID_I_OFF  1
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_MSK   0x00000001
#define MGMT_FEGE_TOP_REG_INT_STAT_BIT_TMVP_FIFO_OVERFLOW_I_OFF   0

/*----------------------------------------.
 | Register 0x00000020 TMVP_ETHERNET_TYPE |
 +----------------------------------------+
 | bit  15:0 R/W  TMVP_ETHERNET_TYPE      |
 +---------------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_UNUSED_MASK            0xffff0000
#define MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_MSK 0x0000ffff
#define MGMT_FEGE_TOP_REG_TMVP_ETHERNET_TYPE_BIT_TMVP_ETHERNET_TYPE_OFF 0

/*----------------------------------------.
 | Register 0x00000024 RMVP_ETHERNET_TYPE |
 +----------------------------------------+
 | bit  15:0 R/W  RMVP_ETHERNET_TYPE      |
 +---------------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_UNUSED_MASK            0xffff0000
#define MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_MSK 0x0000ffff
#define MGMT_FEGE_TOP_REG_RMVP_ETHERNET_TYPE_BIT_RMVP_ETHERNET_TYPE_OFF 0

/*----------------------------------------------.
 | Register 0x0000002c FEGE_SERDES_DATA_IF_CTRL |
 +----------------------------------------------+
 | bit  1 R/W  RDATA_BIT_SWAP                   |
 | bit  0 R/W  TDATA_BIT_SWAP                   |
 +---------------------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_UNUSED_MASK        0xfffffffc
#define MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_RDATA_BIT_SWAP_MSK 0x00000002
#define MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_RDATA_BIT_SWAP_OFF 1
#define MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_TDATA_BIT_SWAP_MSK 0x00000001
#define MGMT_FEGE_TOP_REG_FEGE_SERDES_DATA_IF_CTRL_BIT_TDATA_BIT_SWAP_OFF 0

/* index definitions for PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT */
#define PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT_INDEX_N_MIN               0
#define PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT_INDEX_N_MAX               63
#define PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT_INDEX_N_SIZE              64
#define PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT_INDEX_N_OFFSET            0x4

/*-----------------------------------------------.
 | Register (0x00000100 + (N) * 0x4) TX_VLAN_LUT |
 +-----------------------------------------------+
 | bit  13   R/W  TXLUT_VALID                    |
 | bit  12   R/W  TXLUT_VLAN_ID_REPLACE          |
 | bit  11:0 R/W  TXLUT_VLAN_ID                  |
 +----------------------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_TX_VLAN_LUT_UNUSED_MASK               0xffffc000
#define MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VALID_MSK           0x00002000
#define MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VALID_OFF           13
#define MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_REPLACE_MSK 0x00001000
#define MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_REPLACE_OFF 12
#define MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_MSK         0x00000fff
#define MGMT_FEGE_TOP_REG_TX_VLAN_LUT_BIT_TXLUT_VLAN_ID_OFF         0

/* index definitions for PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT */
#define PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT_INDEX_N_MIN                0
#define PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT_INDEX_N_MAX                63
#define PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT_INDEX_N_SIZE               64
#define PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT_INDEX_N_OFFSET             0x4

/*-----------------------------------------------.
 | Register (0x00000200 + (N) * 0x4) RX_VLAN_LUT |
 +-----------------------------------------------+
 | bit  15   R/W  RXLUT_VALID                    |
 | bit  14   R/W  RXLUT_PRIORITY_ENABLE          |
 | bit  13   R/W  RXLUT_FIFO_ZONE_ENABLE         |
 | bit  12   R/W  RXLUT_FCS_ENABLE               |
 | bit  11:0 R/W  RXLUT_VLAN_ID                  |
 +----------------------------------------------*/
#define PMC_MGMT_FEGE_TOP_REG_RX_VLAN_LUT_UNUSED_MASK                0xffff0000
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VALID_MSK            0x00008000
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VALID_OFF            15
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_PRIORITY_ENABLE_MSK  0x00004000
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_PRIORITY_ENABLE_OFF  14
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FIFO_ZONE_ENABLE_MSK 0x00002000
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FIFO_ZONE_ENABLE_OFF 13
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FCS_ENABLE_MSK       0x00001000
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_FCS_ENABLE_OFF       12
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_MSK          0x00000fff
#define MGMT_FEGE_TOP_REG_RX_VLAN_LUT_BIT_RXLUT_VLAN_ID_OFF          0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MGMT_FEGE_REGS_H */
