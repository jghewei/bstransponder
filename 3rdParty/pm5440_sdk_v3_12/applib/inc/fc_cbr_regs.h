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
 *     fc_cbr block
 *****************************************************************************/
#ifndef _FC_CBR_REGS_H
#define _FC_CBR_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC_CBR_REG_CFG                 0x00000000
#define PMC_FC_CBR_REG_RX_CFG              0x00000014
#define PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG    0x0000001c
#define PMC_FC_CBR_REG_RX_DLOLB_EN         0x00000020
#define PMC_FC_CBR_REG_RX_DEFECT_CFG       0x00000024
#define PMC_FC_CBR_REG_RX_GF_WRD_CNTR      0x00000028
#define PMC_FC_CBR_REG_RX_BF_WRD_CNTR      0x0000002c
#define PMC_FC_CBR_REG_RX_WORD_CNT         0x00000030
#define PMC_FC_CBR_REG_RX_FRAMES_BAD_CRC   0x00000034
#define PMC_FC_CBR_REG_RX_TOTAL_PACKETS    0x00000038
#define PMC_FC_CBR_REG_TX_USER_PATTERN_LSB 0x00000040
#define PMC_FC_CBR_REG_TX_CFG              0x00000044
#define PMC_FC_CBR_REG_LCV_CNT             0x00000090
#define PMC_FC_CBR_REG_CNT_UPDATE          0x00000094
#define PMC_FC_CBR_REG_INT_EN              0x000000b0
#define PMC_FC_CBR_REG_INT                 0x000000b4
#define PMC_FC_CBR_REG_INT_VAL             0x000000b8

/*----------------------------.
 | Register 0x00000000 CFG    |
 +----------------------------+
 | bit  31  R/W  RX_FRM_SCRAM |
 | bit  5:3 R/W  BUS_WIDTH    |
 | bit  2:1 R/W  MODE         |
 | bit  0   R/W  SOFT_RST     |
 +---------------------------*/
#define PMC_FC_CBR_REG_CFG_UNUSED_MASK      0x3ffffcc0
#define FC_CBR_REG_CFG_BIT_RX_FRM_SCRAM_MSK 0x80000000
#define FC_CBR_REG_CFG_BIT_RX_FRM_SCRAM_OFF 31
#define FC_CBR_REG_CFG_BIT_BUS_WIDTH_MSK    0x00000038
#define FC_CBR_REG_CFG_BIT_BUS_WIDTH_OFF    3
#define FC_CBR_REG_CFG_BIT_MODE_MSK         0x00000006
#define FC_CBR_REG_CFG_BIT_MODE_OFF         1
#define FC_CBR_REG_CFG_BIT_SOFT_RST_MSK     0x00000001
#define FC_CBR_REG_CFG_BIT_SOFT_RST_OFF     0

/*-----------------------------------.
 | Register 0x00000014 RX_CFG        |
 +-----------------------------------+
 | bit  8 R/W  RX_FC_CONT_ALIGN_MODE |
 | bit  2 R/W  RX_ENABLE_BYTE_ALIGN  |
 | bit  1 R/W  RX_FORCE_REALIGN      |
 | bit  0 R/W  RX_SOFT_RST           |
 +----------------------------------*/
#define PMC_FC_CBR_REG_RX_CFG_UNUSED_MASK               0x0000dc00
#define FC_CBR_REG_RX_CFG_BIT_RX_FC_CONT_ALIGN_MODE_MSK 0x00000100
#define FC_CBR_REG_RX_CFG_BIT_RX_FC_CONT_ALIGN_MODE_OFF 8
#define FC_CBR_REG_RX_CFG_BIT_RX_ENABLE_BYTE_ALIGN_MSK  0x00000004
#define FC_CBR_REG_RX_CFG_BIT_RX_ENABLE_BYTE_ALIGN_OFF  2
#define FC_CBR_REG_RX_CFG_BIT_RX_FORCE_REALIGN_MSK      0x00000002
#define FC_CBR_REG_RX_CFG_BIT_RX_FORCE_REALIGN_OFF      1
#define FC_CBR_REG_RX_CFG_BIT_RX_SOFT_RST_MSK           0x00000001
#define FC_CBR_REG_RX_CFG_BIT_RX_SOFT_RST_OFF           0

/*--------------------------------------.
 | Register 0x0000001c RX_DLOLB_CLK_CFG |
 +--------------------------------------+
 | bit  31:0 R/W  RX_DLOLB_CLK_DIV      |
 +-------------------------------------*/
#define PMC_FC_CBR_REG_RX_DLOLB_CLK_CFG_UNUSED_MASK          0x00000000
#define FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_MSK 0xffffffff
#define FC_CBR_REG_RX_DLOLB_CLK_CFG_BIT_RX_DLOLB_CLK_DIV_OFF 0

/*-------------------------------------.
 | Register 0x00000020 RX_DLOLB_EN     |
 +-------------------------------------+
 | bit  7   R/W  RX_LCV_DLOLB_EN       |
 | bit  6   R/W  RX_XDET_DLOLB_EN      |
 | bit  5   R/W  ASD_DLOLB_EN          |
 | bit  4   R/W  RX_LOSS_SYNC_DLOLB_EN |
 | bit  3   R/W  RX_DRL_DLOLB_EN       |
 | bit  2:0 R/W  RX_DLOLB_INT_PER      |
 +------------------------------------*/
#define PMC_FC_CBR_REG_RX_DLOLB_EN_UNUSED_MASK               0xffffff00
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LCV_DLOLB_EN_MSK       0x00000080
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LCV_DLOLB_EN_OFF       7
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_XDET_DLOLB_EN_MSK      0x00000040
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_XDET_DLOLB_EN_OFF      6
#define FC_CBR_REG_RX_DLOLB_EN_BIT_ASD_DLOLB_EN_MSK          0x00000020
#define FC_CBR_REG_RX_DLOLB_EN_BIT_ASD_DLOLB_EN_OFF          5
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LOSS_SYNC_DLOLB_EN_MSK 0x00000010
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_LOSS_SYNC_DLOLB_EN_OFF 4
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DRL_DLOLB_EN_MSK       0x00000008
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DRL_DLOLB_EN_OFF       3
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_MSK      0x00000007
#define FC_CBR_REG_RX_DLOLB_EN_BIT_RX_DLOLB_INT_PER_OFF      0

/*----------------------------------------.
 | Register 0x00000024 RX_DEFECT_CFG      |
 +----------------------------------------+
 | bit  31:16 R/W  RX_LCV_DLOLB_THRSH     |
 | bit  15:8  R/W  RX_XDET_INTERVAL       |
 | bit  7:0   R/W  RX_XDET_DEASSERT_THRSH |
 +---------------------------------------*/
#define PMC_FC_CBR_REG_RX_DEFECT_CFG_UNUSED_MASK                0x00000000
#define FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_MSK     0xffff0000
#define FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_LCV_DLOLB_THRSH_OFF     16
#define FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_MSK       0x0000ff00
#define FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_INTERVAL_OFF       8
#define FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_MSK 0x000000ff
#define FC_CBR_REG_RX_DEFECT_CFG_BIT_RX_XDET_DEASSERT_THRSH_OFF 0

/*------------------------------------.
 | Register 0x00000028 RX_GF_WRD_CNTR |
 +------------------------------------+
 | bit  31:0 R  RX_GF_DWRD            |
 +-----------------------------------*/
#define PMC_FC_CBR_REG_RX_GF_WRD_CNTR_UNUSED_MASK    0x00000000
#define FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_MSK 0xffffffff
#define FC_CBR_REG_RX_GF_WRD_CNTR_BIT_RX_GF_DWRD_OFF 0

/*------------------------------------.
 | Register 0x0000002c RX_BF_WRD_CNTR |
 +------------------------------------+
 | bit  31:0 R  RX_BF_DWRD            |
 +-----------------------------------*/
#define PMC_FC_CBR_REG_RX_BF_WRD_CNTR_UNUSED_MASK    0x00000000
#define FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_MSK 0xffffffff
#define FC_CBR_REG_RX_BF_WRD_CNTR_BIT_RX_BF_DWRD_OFF 0

/*---------------------------------.
 | Register 0x00000030 RX_WORD_CNT |
 +---------------------------------+
 | bit  31:0 R  RX_WRD_CNT         |
 +--------------------------------*/
#define PMC_FC_CBR_REG_RX_WORD_CNT_UNUSED_MASK    0x00000000
#define FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_MSK 0xffffffff
#define FC_CBR_REG_RX_WORD_CNT_BIT_RX_WRD_CNT_OFF 0

/*---------------------------------------.
 | Register 0x00000034 RX_FRAMES_BAD_CRC |
 +---------------------------------------+
 | bit  31:0 R  RX_BAD_CRC               |
 +--------------------------------------*/
#define PMC_FC_CBR_REG_RX_FRAMES_BAD_CRC_UNUSED_MASK    0x00000000
#define FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_MSK 0xffffffff
#define FC_CBR_REG_RX_FRAMES_BAD_CRC_BIT_RX_BAD_CRC_OFF 0

/*--------------------------------------.
 | Register 0x00000038 RX_TOTAL_PACKETS |
 +--------------------------------------+
 | bit  31:0 R  RX_TOT_PKTS             |
 +-------------------------------------*/
#define PMC_FC_CBR_REG_RX_TOTAL_PACKETS_UNUSED_MASK     0x00000000
#define FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_MSK 0xffffffff
#define FC_CBR_REG_RX_TOTAL_PACKETS_BIT_RX_TOT_PKTS_OFF 0

/*-----------------------------------------.
 | Register 0x00000040 TX_USER_PATTERN_LSB |
 +-----------------------------------------+
 | bit  31:0 R/W  TX_USR_PATT              |
 +----------------------------------------*/
#define PMC_FC_CBR_REG_TX_USER_PATTERN_LSB_UNUSED_MASK     0x00000000
#define FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_MSK 0xffffffff
#define FC_CBR_REG_TX_USER_PATTERN_LSB_BIT_TX_USR_PATT_OFF 0

/*------------------------------------.
 | Register 0x00000044 TX_CFG         |
 +------------------------------------+
 | bit  31:24 R/W  TX_USR_PATT        |
 | bit  19:16 R/W  TX_USR_PATT_DISP   |
 | bit  15    R/W  GE_RD_INV_EN       |
 | bit  14    R/W  FC_RD_INV_EN       |
 | bit  13    R/W  TX_RD_INV_EN       |
 | bit  9     R/W  TX_USR_PATT_UPDATE |
 | bit  7     R/W  TX_LCV_ERR_INS_EN  |
 | bit  6     R/W  TX_PN11_INV        |
 | bit  5     R/W  TX_NOS_PN11_SEL    |
 | bit  4     R/W  TX_FORCE_NOS_PN11  |
 | bit  0     R/W  TX_SOFT_RST        |
 +-----------------------------------*/
#define PMC_FC_CBR_REG_TX_CFG_UNUSED_MASK            0x00000002
#define FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_MSK        0xff000000
#define FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_OFF        24
#define FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_MSK   0x000f0000
#define FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_DISP_OFF   16
#define FC_CBR_REG_TX_CFG_BIT_GE_RD_INV_EN_MSK       0x00008000
#define FC_CBR_REG_TX_CFG_BIT_GE_RD_INV_EN_OFF       15
#define FC_CBR_REG_TX_CFG_BIT_FC_RD_INV_EN_MSK       0x00004000
#define FC_CBR_REG_TX_CFG_BIT_FC_RD_INV_EN_OFF       14
#define FC_CBR_REG_TX_CFG_BIT_TX_RD_INV_EN_MSK       0x00002000
#define FC_CBR_REG_TX_CFG_BIT_TX_RD_INV_EN_OFF       13
#define FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_UPDATE_MSK 0x00000200
#define FC_CBR_REG_TX_CFG_BIT_TX_USR_PATT_UPDATE_OFF 9
#define FC_CBR_REG_TX_CFG_BIT_TX_LCV_ERR_INS_EN_MSK  0x00000080
#define FC_CBR_REG_TX_CFG_BIT_TX_LCV_ERR_INS_EN_OFF  7
#define FC_CBR_REG_TX_CFG_BIT_TX_PN11_INV_MSK        0x00000040
#define FC_CBR_REG_TX_CFG_BIT_TX_PN11_INV_OFF        6
#define FC_CBR_REG_TX_CFG_BIT_TX_NOS_PN11_SEL_MSK    0x00000020
#define FC_CBR_REG_TX_CFG_BIT_TX_NOS_PN11_SEL_OFF    5
#define FC_CBR_REG_TX_CFG_BIT_TX_FORCE_NOS_PN11_MSK  0x00000010
#define FC_CBR_REG_TX_CFG_BIT_TX_FORCE_NOS_PN11_OFF  4
#define FC_CBR_REG_TX_CFG_BIT_TX_SOFT_RST_MSK        0x00000001
#define FC_CBR_REG_TX_CFG_BIT_TX_SOFT_RST_OFF        0

/*-----------------------------.
 | Register 0x00000090 LCV_CNT |
 +-----------------------------+
 | bit  15:0 R  RX_LCV_CNT     |
 +----------------------------*/
#define PMC_FC_CBR_REG_LCV_CNT_UNUSED_MASK    0x00000000
#define FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_MSK 0x0000ffff
#define FC_CBR_REG_LCV_CNT_BIT_RX_LCV_CNT_OFF 0

/*--------------------------------.
 | Register 0x00000094 CNT_UPDATE |
 +--------------------------------+
 | bit  0 R/W  CNTR_UPDATE        |
 +-------------------------------*/
#define PMC_FC_CBR_REG_CNT_UPDATE_UNUSED_MASK     0x7ffffffe
#define FC_CBR_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK 0x00000001
#define FC_CBR_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF 0

/*------------------------------.
 | Register 0x000000b0 INT_EN   |
 +------------------------------+
 | bit  6 R/W  RX_DISP_INVERT_E |
 | bit  5 R/W  RX_XDET_E        |
 | bit  4 R/W  RX_SOF_DET_E     |
 | bit  3 R/W  RX_DLOLB_E       |
 | bit  2 R/W  RX_ASD_E         |
 | bit  1 R/W  RX_SYNC_E        |
 | bit  0 R/W  RX_LCV_E         |
 +-----------------------------*/
#define PMC_FC_CBR_REG_INT_EN_UNUSED_MASK          0x00002000
#define FC_CBR_REG_INT_EN_BIT_RX_DISP_INVERT_E_MSK 0x00000040
#define FC_CBR_REG_INT_EN_BIT_RX_DISP_INVERT_E_OFF 6
#define FC_CBR_REG_INT_EN_BIT_RX_XDET_E_MSK        0x00000020
#define FC_CBR_REG_INT_EN_BIT_RX_XDET_E_OFF        5
#define FC_CBR_REG_INT_EN_BIT_RX_SOF_DET_E_MSK     0x00000010
#define FC_CBR_REG_INT_EN_BIT_RX_SOF_DET_E_OFF     4
#define FC_CBR_REG_INT_EN_BIT_RX_DLOLB_E_MSK       0x00000008
#define FC_CBR_REG_INT_EN_BIT_RX_DLOLB_E_OFF       3
#define FC_CBR_REG_INT_EN_BIT_RX_ASD_E_MSK         0x00000004
#define FC_CBR_REG_INT_EN_BIT_RX_ASD_E_OFF         2
#define FC_CBR_REG_INT_EN_BIT_RX_SYNC_E_MSK        0x00000002
#define FC_CBR_REG_INT_EN_BIT_RX_SYNC_E_OFF        1
#define FC_CBR_REG_INT_EN_BIT_RX_LCV_E_MSK         0x00000001
#define FC_CBR_REG_INT_EN_BIT_RX_LCV_E_OFF         0

/*------------------------------.
 | Register 0x000000b4 INT      |
 +------------------------------+
 | bit  6 R/W  RX_DISP_INVERT_I |
 | bit  5 R/W  RX_XDET_I        |
 | bit  4 R/W  RX_SOF_DET_I     |
 | bit  3 R/W  RX_DLOLB_I       |
 | bit  2 R/W  RX_ASD_I         |
 | bit  1 R/W  RX_SYNC_I        |
 | bit  0 R/W  RX_LCV_I         |
 +-----------------------------*/
#define PMC_FC_CBR_REG_INT_UNUSED_MASK          0x00002000
#define FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_MSK 0x00000040
#define FC_CBR_REG_INT_BIT_RX_DISP_INVERT_I_OFF 6
#define FC_CBR_REG_INT_BIT_RX_XDET_I_MSK        0x00000020
#define FC_CBR_REG_INT_BIT_RX_XDET_I_OFF        5
#define FC_CBR_REG_INT_BIT_RX_SOF_DET_I_MSK     0x00000010
#define FC_CBR_REG_INT_BIT_RX_SOF_DET_I_OFF     4
#define FC_CBR_REG_INT_BIT_RX_DLOLB_I_MSK       0x00000008
#define FC_CBR_REG_INT_BIT_RX_DLOLB_I_OFF       3
#define FC_CBR_REG_INT_BIT_RX_ASD_I_MSK         0x00000004
#define FC_CBR_REG_INT_BIT_RX_ASD_I_OFF         2
#define FC_CBR_REG_INT_BIT_RX_SYNC_I_MSK        0x00000002
#define FC_CBR_REG_INT_BIT_RX_SYNC_I_OFF        1
#define FC_CBR_REG_INT_BIT_RX_LCV_I_MSK         0x00000001
#define FC_CBR_REG_INT_BIT_RX_LCV_I_OFF         0

/*-----------------------------.
 | Register 0x000000b8 INT_VAL |
 +-----------------------------+
 | bit  6 R  RX_DISP_INVERT_V  |
 | bit  5 R  RX_XDET_V         |
 | bit  3 R  RX_DLOLB_V        |
 | bit  2 R  RX_ASD_V          |
 | bit  1 R  RX_SYNC_V         |
 | bit  0 R  RX_LCV_V          |
 +----------------------------*/
#define PMC_FC_CBR_REG_INT_VAL_UNUSED_MASK          0x00002000
#define FC_CBR_REG_INT_VAL_BIT_RX_DISP_INVERT_V_MSK 0x00000040
#define FC_CBR_REG_INT_VAL_BIT_RX_DISP_INVERT_V_OFF 6
#define FC_CBR_REG_INT_VAL_BIT_RX_XDET_V_MSK        0x00000020
#define FC_CBR_REG_INT_VAL_BIT_RX_XDET_V_OFF        5
#define FC_CBR_REG_INT_VAL_BIT_RX_DLOLB_V_MSK       0x00000008
#define FC_CBR_REG_INT_VAL_BIT_RX_DLOLB_V_OFF       3
#define FC_CBR_REG_INT_VAL_BIT_RX_ASD_V_MSK         0x00000004
#define FC_CBR_REG_INT_VAL_BIT_RX_ASD_V_OFF         2
#define FC_CBR_REG_INT_VAL_BIT_RX_SYNC_V_MSK        0x00000002
#define FC_CBR_REG_INT_VAL_BIT_RX_SYNC_V_OFF        1
#define FC_CBR_REG_INT_VAL_BIT_RX_LCV_V_MSK         0x00000001
#define FC_CBR_REG_INT_VAL_BIT_RX_LCV_V_OFF         0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_CBR_REGS_H */
