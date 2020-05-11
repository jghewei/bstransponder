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
 *     cbrc_sfi51 block
 *****************************************************************************/
#ifndef _CBRC_SFI51_REGS_H
#define _CBRC_SFI51_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SFI51_REG_RX_GLOBAL_CTRL_REG                   0x00000000
#define PMC_SFI51_REG_TX_GLOBAL_CTRL_REG                   0x00000004
#define PMC_SFI51_REG_RX_INT_REG                           0x00000008
#define PMC_SFI51_REG_RX_INT_EN                            0x0000000c
#define PMC_SFI51_REG_RX_INT_STAT                          0x00000010
#define PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG                0x00000014
#define PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG             0x00000018
#define PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG           0x0000001c
#define PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG           0x00000020
#define PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG        0x00000024
#define PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG       0x00000028
#define PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG 0x0000002c
#define PMC_SFI51_REG_TX_INT_REG                           0x00000030
#define PMC_SFI51_REG_TX_INT_EN                            0x00000034
#define PMC_SFI51_REG_TX_INT_STAT                          0x00000038
#define PMC_SFI51_REG_EXPANSION_HEADER_REG                 0x0000003c

/*----------------------------------------.
 | Register 0x00000000 RX_GLOBAL_CTRL_REG |
 +----------------------------------------+
 | bit  2 R/W  RX_LANE_SWAP               |
 | bit  1 R/W  RX_DESKEW_DISABLE          |
 | bit  0 R/W  RX_SYNC_RESET              |
 +---------------------------------------*/
#define PMC_SFI51_REG_RX_GLOBAL_CTRL_REG_UNUSED_MASK           0xffffff88
#define SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LANE_SWAP_MSK      0x00000004
#define SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LANE_SWAP_OFF      2
#define SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DESKEW_DISABLE_MSK 0x00000002
#define SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DESKEW_DISABLE_OFF 1
#define SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYNC_RESET_MSK     0x00000001
#define SFI51_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYNC_RESET_OFF     0

/*----------------------------------------.
 | Register 0x00000004 TX_GLOBAL_CTRL_REG |
 +----------------------------------------+
 | bit  1 R/W  TX_LANE_SWAP               |
 | bit  0 R/W  TX_SYNC_RESET              |
 +---------------------------------------*/
#define PMC_SFI51_REG_TX_GLOBAL_CTRL_REG_UNUSED_MASK       0xfffffffc
#define SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LANE_SWAP_MSK  0x00000002
#define SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LANE_SWAP_OFF  1
#define SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYNC_RESET_MSK 0x00000001
#define SFI51_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYNC_RESET_OFF 0

/*-----------------------------------.
 | Register 0x00000008 RX_INT_REG    |
 +-----------------------------------+
 | bit  4 R/W  RX_DESKEW_FIFO_OVFL_I |
 | bit  3 R/W  RX_DESKEW_FIFO_UNFL_I |
 | bit  2 R/W  RX_DESKEW_LOF_I       |
 | bit  1 R/W  RX_DESKEW_OOF_I       |
 | bit  0 R/W  RX_DESKEW_LOS_I       |
 +----------------------------------*/
#define PMC_SFI51_REG_RX_INT_REG_UNUSED_MASK               0xffffffe0
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_MSK 0x00000010
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_OVFL_I_OFF 4
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_MSK 0x00000008
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_FIFO_UNFL_I_OFF 3
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_MSK       0x00000004
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOF_I_OFF       2
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_MSK       0x00000002
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_OOF_I_OFF       1
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_MSK       0x00000001
#define SFI51_REG_RX_INT_REG_BIT_RX_DESKEW_LOS_I_OFF       0

/*-----------------------------------.
 | Register 0x0000000c RX_INT_EN     |
 +-----------------------------------+
 | bit  4 R/W  RX_DESKEW_FIFO_OVFL_E |
 | bit  3 R/W  RX_DESKEW_FIFO_UNFL_E |
 | bit  2 R/W  RX_DESKEW_LOF_E       |
 | bit  1 R/W  RX_DESKEW_OOF_E       |
 | bit  0 R/W  RX_DESKEW_LOS_E       |
 +----------------------------------*/
#define PMC_SFI51_REG_RX_INT_EN_UNUSED_MASK               0xffffffe0
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_OVFL_E_MSK 0x00000010
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_OVFL_E_OFF 4
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_UNFL_E_MSK 0x00000008
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_FIFO_UNFL_E_OFF 3
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOF_E_MSK       0x00000004
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOF_E_OFF       2
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_OOF_E_MSK       0x00000002
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_OOF_E_OFF       1
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOS_E_MSK       0x00000001
#define SFI51_REG_RX_INT_EN_BIT_RX_DESKEW_LOS_E_OFF       0

/*---------------------------------.
 | Register 0x00000010 RX_INT_STAT |
 +---------------------------------+
 | bit  4 R  RX_DESKEW_FIFO_OVFL_V |
 | bit  3 R  RX_DESKEW_FIFO_UNFL_V |
 | bit  2 R  RX_DESKEW_LOF_V       |
 | bit  1 R  RX_DESKEW_OOF_V       |
 | bit  0 R  RX_DESKEW_LOS_V       |
 +--------------------------------*/
#define PMC_SFI51_REG_RX_INT_STAT_UNUSED_MASK               0xffffffe0
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_OVFL_V_MSK 0x00000010
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_OVFL_V_OFF 4
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_UNFL_V_MSK 0x00000008
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_FIFO_UNFL_V_OFF 3
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOF_V_MSK       0x00000004
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOF_V_OFF       2
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_OOF_V_MSK       0x00000002
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_OOF_V_OFF       1
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOS_V_MSK       0x00000001
#define SFI51_REG_RX_INT_STAT_BIT_RX_DESKEW_LOS_V_OFF       0

/*-------------------------------------------.
 | Register 0x00000014 RX_DESKEW_OOA_INT_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  RX_DESKEW_OOA_I            |
 +------------------------------------------*/
#define PMC_SFI51_REG_RX_DESKEW_OOA_INT_REG_UNUSED_MASK         0xffff0000
#define SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_MSK 0x0000ffff
#define SFI51_REG_RX_DESKEW_OOA_INT_REG_BIT_RX_DESKEW_OOA_I_OFF 0

/*----------------------------------------------.
 | Register 0x00000018 RX_DESKEW_OOA_INT_EN_REG |
 +----------------------------------------------+
 | bit  15:0 R/W  RX_DESKEW_OOA_E               |
 +---------------------------------------------*/
#define PMC_SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_UNUSED_MASK         0xffff0000
#define SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_MSK 0x0000ffff
#define SFI51_REG_RX_DESKEW_OOA_INT_EN_REG_BIT_RX_DESKEW_OOA_E_OFF 0

/*------------------------------------------------.
 | Register 0x0000001c RX_DESKEW_OOA_INT_STAT_REG |
 +------------------------------------------------+
 | bit  15:0 R  RX_DESKEW_OOA_V                   |
 +-----------------------------------------------*/
#define PMC_SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_UNUSED_MASK         0xffff0000
#define SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_MSK 0x0000ffff
#define SFI51_REG_RX_DESKEW_OOA_INT_STAT_REG_BIT_RX_DESKEW_OOA_V_OFF 0

/*------------------------------------------------.
 | Register 0x00000020 RX_CHANNEL_BIT_ERR_INT_REG |
 +------------------------------------------------+
 | bit  15:0 R/W  RX_CHN_BIT_ERR_I                |
 +-----------------------------------------------*/
#define PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_UNUSED_MASK          0xffff0000
#define SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_MSK 0x0000ffff
#define SFI51_REG_RX_CHANNEL_BIT_ERR_INT_REG_BIT_RX_CHN_BIT_ERR_I_OFF 0

/*---------------------------------------------------.
 | Register 0x00000024 RX_CHANNEL_BIT_ERR_INT_EN_REG |
 +---------------------------------------------------+
 | bit  15:0 R/W  RX_CHN_BIT_ERR_E                   |
 +--------------------------------------------------*/
#define PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_UNUSED_MASK          0xffff0000
#define SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_MSK 0x0000ffff
#define SFI51_REG_RX_CHANNEL_BIT_ERR_INT_EN_REG_BIT_RX_CHN_BIT_ERR_E_OFF 0

/*----------------------------------------------------.
 | Register 0x00000028 RX_CHANNEL_BIT_ERR_INT_VAL_REG |
 +----------------------------------------------------+
 | bit  15:0 R  RX_CHN_BIT_ERR_V                      |
 +---------------------------------------------------*/
#define PMC_SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_UNUSED_MASK          0xffff0000
#define SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_MSK 0x0000ffff
#define SFI51_REG_RX_CHANNEL_BIT_ERR_INT_VAL_REG_BIT_RX_CHN_BIT_ERR_V_OFF 0

/*----------------------------------------------------------.
 | Register 0x0000002c DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG |
 +----------------------------------------------------------+
 | bit  4:0 R/W  DSC_FIFO_HI_RD_THRSH                       |
 +---------------------------------------------------------*/
#define PMC_SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_UNUSED_MASK              0xffffffe0
#define SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_MSK 0x0000001f
#define SFI51_REG_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_BIT_DSC_FIFO_HI_RD_THRSH_OFF 0

/*--------------------------------.
 | Register 0x00000030 TX_INT_REG |
 +--------------------------------+
 | bit  1 R/W  TX_FIFO_UNFL_I     |
 | bit  0 R/W  TX_FIFO_OVFL_I     |
 +-------------------------------*/
#define PMC_SFI51_REG_TX_INT_REG_UNUSED_MASK        0xfffffffc
#define SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_MSK 0x00000002
#define SFI51_REG_TX_INT_REG_BIT_TX_FIFO_UNFL_I_OFF 1
#define SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_MSK 0x00000001
#define SFI51_REG_TX_INT_REG_BIT_TX_FIFO_OVFL_I_OFF 0

/*-------------------------------.
 | Register 0x00000034 TX_INT_EN |
 +-------------------------------+
 | bit  1 R/W  TX_FIFO_UNFL_E    |
 | bit  0 R/W  TX_FIFO_OVFL_E    |
 +------------------------------*/
#define PMC_SFI51_REG_TX_INT_EN_UNUSED_MASK        0xfffffffc
#define SFI51_REG_TX_INT_EN_BIT_TX_FIFO_UNFL_E_MSK 0x00000002
#define SFI51_REG_TX_INT_EN_BIT_TX_FIFO_UNFL_E_OFF 1
#define SFI51_REG_TX_INT_EN_BIT_TX_FIFO_OVFL_E_MSK 0x00000001
#define SFI51_REG_TX_INT_EN_BIT_TX_FIFO_OVFL_E_OFF 0

/*---------------------------------.
 | Register 0x00000038 TX_INT_STAT |
 +---------------------------------+
 | bit  1 R  TX_FIFO_UNFL_V        |
 | bit  0 R  TX_FIFO_OVFL_V        |
 +--------------------------------*/
#define PMC_SFI51_REG_TX_INT_STAT_UNUSED_MASK        0xfffffffc
#define SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_UNFL_V_MSK 0x00000002
#define SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_UNFL_V_OFF 1
#define SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_OVFL_V_MSK 0x00000001
#define SFI51_REG_TX_INT_STAT_BIT_TX_FIFO_OVFL_V_OFF 0

/*------------------------------------------.
 | Register 0x0000003c EXPANSION_HEADER_REG |
 +------------------------------------------+
 | bit  31:0 R/W  EXPN_HEADER               |
 +-----------------------------------------*/
#define PMC_SFI51_REG_EXPANSION_HEADER_REG_UNUSED_MASK     0x00000000
#define SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_MSK 0xffffffff
#define SFI51_REG_EXPANSION_HEADER_REG_BIT_EXPN_HEADER_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_SFI51_REGS_H */
