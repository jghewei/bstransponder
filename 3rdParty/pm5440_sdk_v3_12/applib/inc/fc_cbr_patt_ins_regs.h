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
 *     fc_cbr_patt_ins block
 *****************************************************************************/
#ifndef _FC_CBR_PATT_INS_REGS_H
#define _FC_CBR_PATT_INS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC_CBR_PATT_INS_REG_CFG                      0x00000080
#define PMC_FC_CBR_PATT_INS_REG_UPDATE                   0x00000084
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0       0x00000088
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32      0x0000008c
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64      0x00000090
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96     0x00000094
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128 0x00000098
#define PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0     0x0000009c
#define PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32    0x000000a0
#define PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA            0x000000a4
#define PMC_FC_CBR_PATT_INS_REG_INT_INDICATION           0x000000a8
#define PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT           0x000000ac
#define PMC_FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT          0x000000b0
#define PMC_FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT          0x000000b4
#define PMC_FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT        0x000000b8

/*------------------------------------.
 | Register 0x00000080 CFG            |
 +------------------------------------+
 | bit  26:24 R/W  FAULT_SIZE         |
 | bit  21:20 R/W  FAULT_MODE         |
 | bit  18:16 R/W  BUS_WIDTH          |
 | bit  12    R/W  PN11_INV           |
 | bit  8     R/W  SCRAM_EN           |
 | bit  6:5   R/W  ZONE_LEVEL         |
 | bit  4     R/W  FORCE_FAULT        |
 | bit  1     R/W  FC_CBR_PATT_INS_EN |
 | bit  0     R/W  SOFT_RST           |
 +-----------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_CFG_UNUSED_MASK            0x08c8ee8c
#define FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_MSK         0x07000000
#define FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_SIZE_OFF         24
#define FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_MSK         0x00300000
#define FC_CBR_PATT_INS_REG_CFG_BIT_FAULT_MODE_OFF         20
#define FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_MSK          0x00070000
#define FC_CBR_PATT_INS_REG_CFG_BIT_BUS_WIDTH_OFF          16
#define FC_CBR_PATT_INS_REG_CFG_BIT_PN11_INV_MSK           0x00001000
#define FC_CBR_PATT_INS_REG_CFG_BIT_PN11_INV_OFF           12
#define FC_CBR_PATT_INS_REG_CFG_BIT_SCRAM_EN_MSK           0x00000100
#define FC_CBR_PATT_INS_REG_CFG_BIT_SCRAM_EN_OFF           8
#define FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_MSK         0x00000060
#define FC_CBR_PATT_INS_REG_CFG_BIT_ZONE_LEVEL_OFF         5
#define FC_CBR_PATT_INS_REG_CFG_BIT_FORCE_FAULT_MSK        0x00000010
#define FC_CBR_PATT_INS_REG_CFG_BIT_FORCE_FAULT_OFF        4
#define FC_CBR_PATT_INS_REG_CFG_BIT_FC_CBR_PATT_INS_EN_MSK 0x00000002
#define FC_CBR_PATT_INS_REG_CFG_BIT_FC_CBR_PATT_INS_EN_OFF 1
#define FC_CBR_PATT_INS_REG_CFG_BIT_SOFT_RST_MSK           0x00000001
#define FC_CBR_PATT_INS_REG_CFG_BIT_SOFT_RST_OFF           0

/*-------------------------------.
 | Register 0x00000084 UPDATE    |
 +-------------------------------+
 | bit  31 R    TIP              |
 | bit  4  R/W  UPDATE_PMON      |
 | bit  0  R/W  UPDATE_USER_PATT |
 +------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_UPDATE_UNUSED_MASK          0x7fffffee
#define FC_CBR_PATT_INS_REG_UPDATE_BIT_TIP_MSK              0x80000000
#define FC_CBR_PATT_INS_REG_UPDATE_BIT_TIP_OFF              31
#define FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_PMON_MSK      0x00000010
#define FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_PMON_OFF      4
#define FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_USER_PATT_MSK 0x00000001
#define FC_CBR_PATT_INS_REG_UPDATE_BIT_UPDATE_USER_PATT_OFF 0

/*----------------------------------------.
 | Register 0x00000088 USER_FLT_PATT_31_0 |
 +----------------------------------------+
 | bit  31:0 R/W  USER_FLT_PATT           |
 +---------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_UNUSED_MASK       0x00000000
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_MSK 0xffffffff
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_31_0_BIT_USER_FLT_PATT_OFF 0

/*-----------------------------------------.
 | Register 0x0000008c USER_FLT_PATT_63_32 |
 +-----------------------------------------+
 | bit  31:0 R/W  USER_FLT_PATT            |
 +----------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_UNUSED_MASK       0x00000000
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_MSK 0xffffffff
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_63_32_BIT_USER_FLT_PATT_OFF 0

/*-----------------------------------------.
 | Register 0x00000090 USER_FLT_PATT_95_64 |
 +-----------------------------------------+
 | bit  31:0 R/W  USER_FLT_PATT            |
 +----------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_UNUSED_MASK       0x00000000
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_MSK 0xffffffff
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_95_64_BIT_USER_FLT_PATT_OFF 0

/*------------------------------------------.
 | Register 0x00000094 USER_FLT_PATT_127_96 |
 +------------------------------------------+
 | bit  31:0 R/W  USER_FLT_PATT             |
 +-----------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_UNUSED_MASK       0x00000000
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_MSK 0xffffffff
#define FC_CBR_PATT_INS_REG_USER_FLT_PATT_127_96_BIT_USER_FLT_PATT_OFF 0

/*----------------------------------------------.
 | Register 0x00000098 USER_FLT_PATTERN_159_128 |
 +----------------------------------------------+
 | bit  31:0 R/W  USER_FLT_PATT                 |
 +---------------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_UNUSED_MASK       0x00000000
#define FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_MSK 0xffffffff
#define FC_CBR_PATT_INS_REG_USER_FLT_PATTERN_159_128_BIT_USER_FLT_PATT_OFF 0

/*------------------------------------------.
 | Register 0x0000009c USER_RCVRY_PATT_31_0 |
 +------------------------------------------+
 | bit  31:0 R/W  USER_RCVRY_PATT           |
 +-----------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_UNUSED_MASK         0x00000000
#define FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_MSK 0xffffffff
#define FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_31_0_BIT_USER_RCVRY_PATT_OFF 0

/*-------------------------------------------.
 | Register 0x000000a0 USER_RCVRY_PATT_39_32 |
 +-------------------------------------------+
 | bit  7:0 R/W  USER_RCVRY_PATT             |
 +------------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_UNUSED_MASK         0xffffff00
#define FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_MSK 0x000000ff
#define FC_CBR_PATT_INS_REG_USER_RCVRY_PATT_39_32_BIT_USER_RCVRY_PATT_OFF 0

/*--------------------------------------------.
 | Register 0x000000a4 INTERRUPT_ENA          |
 +--------------------------------------------+
 | bit  24 R/W  RX_PRBS31_ALL_ONES_OR_ZEROS_E |
 | bit  20 R/W  RX_PRBS31_ERR_E               |
 | bit  16 R/W  RX_PN11_ERR_E                 |
 | bit  12 R/W  TX_PN11_ERR_E                 |
 | bit  8  R/W  RX_PN11_SYNC_E                |
 | bit  4  R/W  TX_PN11_SYNC_E                |
 | bit  0  R/W  UNDERRUN_E                    |
 +-------------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_INTERRUPT_ENA_UNUSED_MASK                       0xfeeeeeee
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_E_MSK 0x01000000
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_E_OFF 24
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ERR_E_MSK               0x00100000
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PRBS31_ERR_E_OFF               20
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_ERR_E_MSK                 0x00010000
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_ERR_E_OFF                 16
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_ERR_E_MSK                 0x00001000
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_ERR_E_OFF                 12
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_SYNC_E_MSK                0x00000100
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_RX_PN11_SYNC_E_OFF                8
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_SYNC_E_MSK                0x00000010
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_TX_PN11_SYNC_E_OFF                4
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_UNDERRUN_E_MSK                    0x00000001
#define FC_CBR_PATT_INS_REG_INTERRUPT_ENA_BIT_UNDERRUN_E_OFF                    0

/*--------------------------------------------.
 | Register 0x000000a8 INT_INDICATION         |
 +--------------------------------------------+
 | bit  24 R/W  RX_PRBS31_ALL_ONES_OR_ZEROS_I |
 | bit  20 R/W  RX_PRBS31_ERR_I               |
 | bit  16 R/W  RX_PN11_ERR_I                 |
 | bit  12 R/W  TX_PN11_ERR_I                 |
 | bit  8  R/W  RX_PN11_SYNC_I                |
 | bit  4  R/W  TX_PN11_SYNC_I                |
 | bit  0  R/W  UNDERRUN_I                    |
 +-------------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_INT_INDICATION_UNUSED_MASK                       0xfeeeeeee
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_MSK 0x01000000
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_I_OFF 24
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_MSK               0x00100000
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PRBS31_ERR_I_OFF               20
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_MSK                 0x00010000
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_ERR_I_OFF                 16
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_MSK                 0x00001000
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_ERR_I_OFF                 12
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_MSK                0x00000100
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_RX_PN11_SYNC_I_OFF                8
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_MSK                0x00000010
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_TX_PN11_SYNC_I_OFF                4
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_MSK                    0x00000001
#define FC_CBR_PATT_INS_REG_INT_INDICATION_BIT_UNDERRUN_I_OFF                    0

/*------------------------------------------.
 | Register 0x000000ac PN11_SYNC_STAT       |
 +------------------------------------------+
 | bit  24 R  RX_PRBS31_ALL_ONES_OR_ZEROS_V |
 | bit  8  R  RX_PN11_SYNC_V                |
 | bit  4  R  TX_PN11_SYNC_V                |
 | bit  0  R  UNDERRUN_V                    |
 +-----------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_UNUSED_MASK                       0xfefffeee
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_V_MSK 0x01000000
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PRBS31_ALL_ONES_OR_ZEROS_V_OFF 24
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PN11_SYNC_V_MSK                0x00000100
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_RX_PN11_SYNC_V_OFF                8
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_TX_PN11_SYNC_V_MSK                0x00000010
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_TX_PN11_SYNC_V_OFF                4
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_UNDERRUN_V_MSK                    0x00000001
#define FC_CBR_PATT_INS_REG_PN11_SYNC_STAT_BIT_UNDERRUN_V_OFF                    0

/*-------------------------------------.
 | Register 0x000000b0 TX_PN11_ERR_CNT |
 +-------------------------------------+
 | bit  15:0 R  TX_PN11_ERR_CNT        |
 +------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_UNUSED_MASK         0xffff0000
#define FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_MSK 0x0000ffff
#define FC_CBR_PATT_INS_REG_TX_PN11_ERR_CNT_BIT_TX_PN11_ERR_CNT_OFF 0

/*-------------------------------------.
 | Register 0x000000b4 RX_PN11_ERR_CNT |
 +-------------------------------------+
 | bit  15:0 R  RX_PN11_ERR_CNT        |
 +------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_UNUSED_MASK         0xffff0000
#define FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_MSK 0x0000ffff
#define FC_CBR_PATT_INS_REG_RX_PN11_ERR_CNT_BIT_RX_PN11_ERR_CNT_OFF 0

/*---------------------------------------.
 | Register 0x000000b8 RX_PRBS31_ERR_CNT |
 +---------------------------------------+
 | bit  15:0 R  RX_PRBS31_ERR_CNT        |
 +--------------------------------------*/
#define PMC_FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_UNUSED_MASK           0xffff0000
#define FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_MSK 0x0000ffff
#define FC_CBR_PATT_INS_REG_RX_PRBS31_ERR_CNT_BIT_RX_PRBS31_ERR_CNT_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_CBR_PATT_INS_REGS_H */
