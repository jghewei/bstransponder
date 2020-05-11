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
 *     emac_core block
 *****************************************************************************/
#ifndef _EMAC_CORE_REGS_H
#define _EMAC_CORE_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CORE_REG_SEG_ACTIVATION_CTRL               0x0000b000
#define PMC_CORE_REG_SEG_MODE_CTRL                     0x0000b004
#define PMC_CORE_REG_TX_CLK_SELECTION_CTRL             0x0000b008
#define PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE          0x0000b00c
#define PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL    0x0000b020
#define PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_    0x0000b024
#define PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_    0x0000b028
#define PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_    0x0000b02c
#define PMC_CORE_REG_TDM_CREDIT_STAT_1_3_              0x0000b030
#define PMC_CORE_REG_TDM_CREDIT_STAT_2_3_              0x0000b034
#define PMC_CORE_REG_TDM_CREDIT_STAT_3_3_              0x0000b038
#define PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL 0x0000b040

/*-----------------------------------------.
 | Register 0x0000b000 SEG_ACTIVATION_CTRL |
 +-----------------------------------------+
 | bit  11:0 R/W  SEG_CTL                  |
 +----------------------------------------*/
#define PMC_CORE_REG_SEG_ACTIVATION_CTRL_UNUSED_MASK 0xfffff000
#define CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_MSK 0x00000fff
#define CORE_REG_SEG_ACTIVATION_CTRL_BIT_SEG_CTL_OFF 0

/*-----------------------------------.
 | Register 0x0000b004 SEG_MODE_CTRL |
 +-----------------------------------+
 | bit  23:0 R/W  MODE_CTL           |
 +----------------------------------*/
#define PMC_CORE_REG_SEG_MODE_CTRL_UNUSED_MASK  0xff000000
#define CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_MSK 0x00ffffff
#define CORE_REG_SEG_MODE_CTRL_BIT_MODE_CTL_OFF 0

/*-------------------------------------------.
 | Register 0x0000b008 TX_CLK_SELECTION_CTRL |
 +-------------------------------------------+
 | bit  23:0 R  TXCLK_CTL                    |
 +------------------------------------------*/
#define PMC_CORE_REG_TX_CLK_SELECTION_CTRL_UNUSED_MASK   0xff000000
#define CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_MSK 0x00ffffff
#define CORE_REG_TX_CLK_SELECTION_CTRL_BIT_TXCLK_CTL_OFF 0

/*----------------------------------------------.
 | Register 0x0000b00c USER_DEFINED_TX_PREAMBLE |
 +----------------------------------------------+
 | bit  23:0 R/W  TX_PRMBL_CTL                  |
 +---------------------------------------------*/
#define PMC_CORE_REG_USER_DEFINED_TX_PREAMBLE_UNUSED_MASK      0xff000000
#define CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_MSK 0x00ffffff
#define CORE_REG_USER_DEFINED_TX_PREAMBLE_BIT_TX_PRMBL_CTL_OFF 0

/*----------------------------------------------------.
 | Register 0x0000b020 BASE_R_FAULT_STATEMACHINE_CTRL |
 +----------------------------------------------------+
 | bit  15:0 R/W  WAN_RS_COL_CNT                      |
 +---------------------------------------------------*/
#define PMC_CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_UNUSED_MASK        0xffff0000
#define CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_MSK 0x0000ffff
#define CORE_REG_BASE_R_FAULT_STATEMACHINE_CTRL_BIT_WAN_RS_COL_CNT_OFF 0

/*----------------------------------------------------.
 | Register 0x0000b024 TDM_CREDIT_INITIALIZATION_1_3_ |
 +----------------------------------------------------+
 | bit  31:24 R/W  SEG3_INIT_CREDIT                   |
 | bit  23:16 R/W  SEG2_INIT_CREDIT                   |
 | bit  15:8  R/W  SEG1_INIT_CREDIT                   |
 | bit  7:0   R/W  SEG0_INIT_CREDIT                   |
 +---------------------------------------------------*/
#define PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_UNUSED_MASK          0x00000000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_MSK 0xff000000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG3_INIT_CREDIT_OFF 24
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_MSK 0x00ff0000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG2_INIT_CREDIT_OFF 16
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_MSK 0x0000ff00
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG1_INIT_CREDIT_OFF 8
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_MSK 0x000000ff
#define CORE_REG_TDM_CREDIT_INITIALIZATION_1_3_BIT_SEG0_INIT_CREDIT_OFF 0

/*----------------------------------------------------.
 | Register 0x0000b028 TDM_CREDIT_INITIALIZATION_2_3_ |
 +----------------------------------------------------+
 | bit  31:24 R/W  SEG7_INIT_CREDIT                   |
 | bit  23:16 R/W  SEG6_INIT_CREDIT                   |
 | bit  15:8  R/W  SEG5_INIT_CREDIT                   |
 | bit  7:0   R/W  SEG4_INIT_CREDIT                   |
 +---------------------------------------------------*/
#define PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_UNUSED_MASK          0x00000000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_MSK 0xff000000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG7_INIT_CREDIT_OFF 24
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_MSK 0x00ff0000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG6_INIT_CREDIT_OFF 16
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_MSK 0x0000ff00
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG5_INIT_CREDIT_OFF 8
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_MSK 0x000000ff
#define CORE_REG_TDM_CREDIT_INITIALIZATION_2_3_BIT_SEG4_INIT_CREDIT_OFF 0

/*----------------------------------------------------.
 | Register 0x0000b02c TDM_CREDIT_INITIALIZATION_3_3_ |
 +----------------------------------------------------+
 | bit  31:24 R/W  SEG11_INIT_CREDIT                  |
 | bit  23:16 R/W  SEG10_INIT_CREDIT                  |
 | bit  15:8  R/W  SEG9_INIT_CREDIT                   |
 | bit  7:0   R/W  SEG8_INIT_CREDIT                   |
 +---------------------------------------------------*/
#define PMC_CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_UNUSED_MASK           0x00000000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_MSK 0xff000000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG11_INIT_CREDIT_OFF 24
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_MSK 0x00ff0000
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG10_INIT_CREDIT_OFF 16
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_MSK  0x0000ff00
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG9_INIT_CREDIT_OFF  8
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_MSK  0x000000ff
#define CORE_REG_TDM_CREDIT_INITIALIZATION_3_3_BIT_SEG8_INIT_CREDIT_OFF  0

/*------------------------------------------.
 | Register 0x0000b030 TDM_CREDIT_STAT_1_3_ |
 +------------------------------------------+
 | bit  31:24 R  SEG3_REG_CREDIT            |
 | bit  23:16 R  SEG2_REG_CREDIT            |
 | bit  15:8  R  SEG1_REG_CREDIT            |
 | bit  7:0   R  SEG0_REG_CREDIT            |
 +-----------------------------------------*/
#define PMC_CORE_REG_TDM_CREDIT_STAT_1_3_UNUSED_MASK         0x00000000
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_MSK 0xff000000
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG3_REG_CREDIT_OFF 24
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_MSK 0x00ff0000
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG2_REG_CREDIT_OFF 16
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_MSK 0x0000ff00
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG1_REG_CREDIT_OFF 8
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_MSK 0x000000ff
#define CORE_REG_TDM_CREDIT_STAT_1_3_BIT_SEG0_REG_CREDIT_OFF 0

/*------------------------------------------.
 | Register 0x0000b034 TDM_CREDIT_STAT_2_3_ |
 +------------------------------------------+
 | bit  31:24 R  SEG7_REG_CREDIT            |
 | bit  23:16 R  SEG6_REG_CREDIT            |
 | bit  15:8  R  SEG5_REG_CREDIT            |
 | bit  7:0   R  SEG4_REG_CREDIT            |
 +-----------------------------------------*/
#define PMC_CORE_REG_TDM_CREDIT_STAT_2_3_UNUSED_MASK         0x00000000
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_MSK 0xff000000
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG7_REG_CREDIT_OFF 24
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_MSK 0x00ff0000
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG6_REG_CREDIT_OFF 16
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_MSK 0x0000ff00
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG5_REG_CREDIT_OFF 8
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_MSK 0x000000ff
#define CORE_REG_TDM_CREDIT_STAT_2_3_BIT_SEG4_REG_CREDIT_OFF 0

/*------------------------------------------.
 | Register 0x0000b038 TDM_CREDIT_STAT_3_3_ |
 +------------------------------------------+
 | bit  31:24 R  SEG11_REG_CREDIT           |
 | bit  23:16 R  SEG10_REG_CREDIT           |
 | bit  15:8  R  SEG9_REG_CREDIT            |
 | bit  7:0   R  SEG8_REG_CREDIT            |
 +-----------------------------------------*/
#define PMC_CORE_REG_TDM_CREDIT_STAT_3_3_UNUSED_MASK          0x00000000
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_MSK 0xff000000
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG11_REG_CREDIT_OFF 24
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_MSK 0x00ff0000
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG10_REG_CREDIT_OFF 16
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_MSK  0x0000ff00
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG9_REG_CREDIT_OFF  8
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_MSK  0x000000ff
#define CORE_REG_TDM_CREDIT_STAT_3_3_BIT_SEG8_REG_CREDIT_OFF  0

/*-------------------------------------------------------.
 | Register 0x0000b040 VIRTUAL_LANE_MARKER_INTERVAL_CTRL |
 +-------------------------------------------------------+
 | bit  0 R/W  VL_INTVL                                  |
 +------------------------------------------------------*/
#define PMC_CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_UNUSED_MASK  0xfffffffe
#define CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_BIT_VL_INTVL_MSK 0x00000001
#define CORE_REG_VIRTUAL_LANE_MARKER_INTERVAL_CTRL_BIT_VL_INTVL_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_CORE_REGS_H */
