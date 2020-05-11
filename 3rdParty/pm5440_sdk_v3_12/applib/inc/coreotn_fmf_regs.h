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
 *     coreotn_fmf block
 *****************************************************************************/
#ifndef _COREOTN_FMF_REGS_H
#define _COREOTN_FMF_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_COREOTN_FMF_REG_SW_RESET_REG               0x000ac000
#define PMC_COREOTN_FMF_REG_CLK_GATING_REG             0x000ac004
#define PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG          0x000ac008
#define PMC_COREOTN_FMF_REG_DCI_CONFIG_REG             0x000ac014
#define PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG        0x000ac018
#define PMC_COREOTN_FMF_REG_TSB_CONFIG_REG             0x000ac01c
#define PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1    0x000ac020
#define PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2    0x000ac024
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1    0x000ac034
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2    0x000ac038
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3    0x000ac03c
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4    0x000ac040
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5    0x000ac044
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6    0x000ac048
#define PMC_COREOTN_FMF_REG_GPO_REG                    0x000ac094
#define PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG( N )  (0x000ac09c + (N) * 0x4)
#define PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG( N )   (0x000ac0cc + (N) * 0x4)
#define PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG( N )     (0x000ac0fc + (N) * 0x4)
#define PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG  0x000ac12c
#define PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG  0x000ac130
#define PMC_COREOTN_FMF_REG_PRBS_RESEED_REG            0x000ac134
#define PMC_COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG 0x000ac140
#define PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG       0x000ac144
#define PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG     0x000ac148
#define PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG      0x000ac14c

/*----------------------------------.
 | Register 0x000ac000 SW_RESET_REG |
 +----------------------------------+
 | bit  10 R/W  PRBS_RST            |
 | bit  9  R/W  ODUKSC2_RST         |
 | bit  8  R/W  ODUJAT_RST          |
 | bit  7  R/W  FRM_RCP96_RST       |
 | bit  6  R/W  FRM_RCP12_RST       |
 | bit  5  R/W  RFRM12_RST          |
 | bit  4  R/W  TFRM12_RST          |
 | bit  3  R/W  RFRM96_RST          |
 | bit  2  R/W  TFRM96_RST          |
 | bit  1  R/W  ODTU_DMX_RST        |
 | bit  0  R/W  ODTU_MUX_RST        |
 +---------------------------------*/
#define PMC_COREOTN_FMF_REG_SW_RESET_REG_UNUSED_MASK       0xfffff000
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_PRBS_RST_MSK      0x00000400
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_PRBS_RST_OFF      10
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUKSC2_RST_MSK   0x00000200
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUKSC2_RST_OFF   9
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUJAT_RST_MSK    0x00000100
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODUJAT_RST_OFF    8
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP96_RST_MSK 0x00000080
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP96_RST_OFF 7
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP12_RST_MSK 0x00000040
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_FRM_RCP12_RST_OFF 6
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM12_RST_MSK    0x00000020
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM12_RST_OFF    5
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM12_RST_MSK    0x00000010
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM12_RST_OFF    4
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM96_RST_MSK    0x00000008
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_RFRM96_RST_OFF    3
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM96_RST_MSK    0x00000004
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_TFRM96_RST_OFF    2
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_DMX_RST_MSK  0x00000002
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_DMX_RST_OFF  1
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_MUX_RST_MSK  0x00000001
#define COREOTN_FMF_REG_SW_RESET_REG_BIT_ODTU_MUX_RST_OFF  0

/*------------------------------------.
 | Register 0x000ac004 CLK_GATING_REG |
 +------------------------------------+
 | bit  11 R/W  FILTER_LOWPWR         |
 | bit  10 R/W  PRBS_LOWPWR           |
 | bit  9  R/W  ODUKSC2_LOWPWR        |
 | bit  8  R/W  ODUJAT_LOWPWR         |
 | bit  7  R/W  FRM_RCP96_LOWPWR      |
 | bit  6  R/W  FRM_RCP12_LOWPWR      |
 | bit  5  R/W  RFRM12_LOWPWR         |
 | bit  4  R/W  TFRM12_LOWPWR         |
 | bit  3  R/W  RFRM96_LOWPWR         |
 | bit  2  R/W  TFRM96_LOWPWR         |
 | bit  1  R/W  ODTU_DMX_LOWPWR       |
 | bit  0  R/W  ODTU_MUX_LOWPWR       |
 +-----------------------------------*/
#define PMC_COREOTN_FMF_REG_CLK_GATING_REG_UNUSED_MASK          0xfffff000
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_FILTER_LOWPWR_MSK    0x00000800
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_FILTER_LOWPWR_OFF    11
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_MSK      0x00000400
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_OFF      10
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUKSC2_LOWPWR_MSK   0x00000200
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUKSC2_LOWPWR_OFF   9
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUJAT_LOWPWR_MSK    0x00000100
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODUJAT_LOWPWR_OFF    8
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP96_LOWPWR_MSK 0x00000080
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP96_LOWPWR_OFF 7
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP12_LOWPWR_MSK 0x00000040
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_FRM_RCP12_LOWPWR_OFF 6
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM12_LOWPWR_MSK    0x00000020
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM12_LOWPWR_OFF    5
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM12_LOWPWR_MSK    0x00000010
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM12_LOWPWR_OFF    4
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM96_LOWPWR_MSK    0x00000008
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_RFRM96_LOWPWR_OFF    3
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM96_LOWPWR_MSK    0x00000004
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_TFRM96_LOWPWR_OFF    2
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_DMX_LOWPWR_MSK  0x00000002
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_DMX_LOWPWR_OFF  1
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_MUX_LOWPWR_MSK  0x00000001
#define COREOTN_FMF_REG_CLK_GATING_REG_BIT_ODTU_MUX_LOWPWR_OFF  0

/*---------------------------------------.
 | Register 0x000ac008 RAM_LOW_POWER_REG |
 +---------------------------------------+
 | bit  10 R/W  PRBS_RAM_LOWPWR          |
 | bit  9  R/W  ODUKSC2_RAM_LOWPWR       |
 | bit  8  R/W  ODUJAT_RAM_LOWPWR        |
 | bit  5  R/W  RFRM12_RAM_LOWPWR        |
 | bit  4  R/W  TFRM12_RAM_LOWPWR        |
 | bit  3  R/W  RFRM96_RAM_LOWPWR        |
 | bit  2  R/W  TFRM96_RAM_LOWPWR        |
 | bit  1  R/W  ODTU_DMX_RAM_LOWPWR      |
 | bit  0  R/W  ODTU_MUX_RAM_LOWPWR      |
 +--------------------------------------*/
#define PMC_COREOTN_FMF_REG_RAM_LOW_POWER_REG_UNUSED_MASK             0xfffff800
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_MSK     0x00000400
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_OFF     10
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUKSC2_RAM_LOWPWR_MSK  0x00000200
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUKSC2_RAM_LOWPWR_OFF  9
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUJAT_RAM_LOWPWR_MSK   0x00000100
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODUJAT_RAM_LOWPWR_OFF   8
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM12_RAM_LOWPWR_MSK   0x00000020
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM12_RAM_LOWPWR_OFF   5
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM12_RAM_LOWPWR_MSK   0x00000010
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM12_RAM_LOWPWR_OFF   4
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM96_RAM_LOWPWR_MSK   0x00000008
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_RFRM96_RAM_LOWPWR_OFF   3
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM96_RAM_LOWPWR_MSK   0x00000004
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_TFRM96_RAM_LOWPWR_OFF   2
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_DMX_RAM_LOWPWR_MSK 0x00000002
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_DMX_RAM_LOWPWR_OFF 1
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_MUX_RAM_LOWPWR_MSK 0x00000001
#define COREOTN_FMF_REG_RAM_LOW_POWER_REG_BIT_ODTU_MUX_RAM_LOWPWR_OFF 0

/*------------------------------------.
 | Register 0x000ac014 DCI_CONFIG_REG |
 +------------------------------------+
 | bit  16    R/W  DCI_MUX_CTRL_5     |
 | bit  13:12 R/W  DCI_MUX_CTRL_4     |
 | bit  8     R/W  DCI_MUX_CTRL_3     |
 | bit  4     R/W  DCI_MUX_CTRL_2     |
 | bit  0     R/W  DCI_MUX_CTRL_1     |
 +-----------------------------------*/
#define PMC_COREOTN_FMF_REG_DCI_CONFIG_REG_UNUSED_MASK        0xfffeceee
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_5_MSK 0x00010000
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_5_OFF 16
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_MSK 0x00003000
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_4_OFF 12
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_3_MSK 0x00000100
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_3_OFF 8
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_2_MSK 0x00000010
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_2_OFF 4
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_1_MSK 0x00000001
#define COREOTN_FMF_REG_DCI_CONFIG_REG_BIT_DCI_MUX_CTRL_1_OFF 0

/*-----------------------------------------.
 | Register 0x000ac018 DCI_XOFF_CONFIG_REG |
 +-----------------------------------------+
 | bit  8 R/W  DCI_XOFF_CTRL_3             |
 | bit  4 R/W  DCI_XOFF_CTRL_2             |
 | bit  0 R/W  DCI_XOFF_CTRL_1             |
 +----------------------------------------*/
#define PMC_COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_UNUSED_MASK         0xfffffeee
#define COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_3_MSK 0x00000100
#define COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_3_OFF 8
#define COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_2_MSK 0x00000010
#define COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_2_OFF 4
#define COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_1_MSK 0x00000001
#define COREOTN_FMF_REG_DCI_XOFF_CONFIG_REG_BIT_DCI_XOFF_CTRL_1_OFF 0

/*------------------------------------.
 | Register 0x000ac01c TSB_CONFIG_REG |
 +------------------------------------+
 | bit  8 R/W  NORM_RATE_SELECT       |
 | bit  0 R/W  ODTU_MUX_MPMX          |
 +-----------------------------------*/
#define PMC_COREOTN_FMF_REG_TSB_CONFIG_REG_UNUSED_MASK          0xfffffefe
#define COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_NORM_RATE_SELECT_MSK 0x00000100
#define COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_NORM_RATE_SELECT_OFF 8
#define COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_ODTU_MUX_MPMX_MSK    0x00000001
#define COREOTN_FMF_REG_TSB_CONFIG_REG_BIT_ODTU_MUX_MPMX_OFF    0

/*---------------------------------------------.
 | Register 0x000ac020 FRM_RCP_12_CONFIG_REG_1 |
 +---------------------------------------------+
 | bit  11:0 R/W  REMOTE_RCP12_CFG             |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_UNUSED_MASK          0xfffff000
#define COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_MSK 0x00000fff
#define COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_1_BIT_REMOTE_RCP12_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac024 FRM_RCP_12_CONFIG_REG_2 |
 +---------------------------------------------+
 | bit  11:0 R/W  CFC_FIFO1_CLRB_CFG           |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_UNUSED_MASK            0xfffff000
#define COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_MSK 0x00000fff
#define COREOTN_FMF_REG_FRM_RCP_12_CONFIG_REG_2_BIT_CFC_FIFO1_CLRB_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac034 FRM_RCP_96_CONFIG_REG_1 |
 +---------------------------------------------+
 | bit  31:0 R/W  REMOTE_RCP96_CFG             |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_UNUSED_MASK          0x00000000
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_MSK 0xffffffff
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_1_BIT_REMOTE_RCP96_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac038 FRM_RCP_96_CONFIG_REG_2 |
 +---------------------------------------------+
 | bit  31:0 R/W  REMOTE_RCP96_CFG             |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_UNUSED_MASK          0x00000000
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_MSK 0xffffffff
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_2_BIT_REMOTE_RCP96_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac03c FRM_RCP_96_CONFIG_REG_3 |
 +---------------------------------------------+
 | bit  31:0 R/W  REMOTE_RCP96_CFG             |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_UNUSED_MASK          0x00000000
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_MSK 0xffffffff
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_3_BIT_REMOTE_RCP96_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac040 FRM_RCP_96_CONFIG_REG_4 |
 +---------------------------------------------+
 | bit  31:0 R/W  CFC_FIFO2_CLRB_CFG           |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_UNUSED_MASK            0x00000000
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_MSK 0xffffffff
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_4_BIT_CFC_FIFO2_CLRB_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac044 FRM_RCP_96_CONFIG_REG_5 |
 +---------------------------------------------+
 | bit  31:0 R/W  CFC_FIFO2_CLRB_CFG           |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_UNUSED_MASK            0x00000000
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_MSK 0xffffffff
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_5_BIT_CFC_FIFO2_CLRB_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x000ac048 FRM_RCP_96_CONFIG_REG_6 |
 +---------------------------------------------+
 | bit  31:0 R/W  CFC_FIFO2_CLRB_CFG           |
 +--------------------------------------------*/
#define PMC_COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_UNUSED_MASK            0x00000000
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_MSK 0xffffffff
#define COREOTN_FMF_REG_FRM_RCP_96_CONFIG_REG_6_BIT_CFC_FIFO2_CLRB_CFG_OFF 0

/*-----------------------------.
 | Register 0x000ac094 GPO_REG |
 +-----------------------------+
 | bit  31:0 R/W  SPARE_GPO    |
 +----------------------------*/
#define PMC_COREOTN_FMF_REG_GPO_REG_UNUSED_MASK   0x00000000
#define COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_MSK 0xffffffff
#define COREOTN_FMF_REG_GPO_REG_BIT_SPARE_GPO_OFF 0

/* index definitions for PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG */
#define PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_MIN             0
#define PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_MAX             11
#define PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_SIZE            12
#define PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_OFFSET          0x4

/*--------------------------------------------------------.
 | Register (0x000ac09c + (N) * 0x4) PRBS_CHANNEL_CFG_REG |
 +--------------------------------------------------------+
 | bit  14:8 R/W  PRBS_CHID                               |
 | bit  3    R/W  PRBS_SEL_INV_NONINV                     |
 | bit  2    R/W  PRBS_SEL_31_11                          |
 | bit  1    R/W  PRBS_MON_EN                             |
 | bit  0    R/W  PRBS_GEN_EN                             |
 +-------------------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_UNUSED_MASK             0xffff80f0
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_MSK           0x00007f00
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF           8
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_MSK 0x00000008
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_OFF 3
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_MSK      0x00000004
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_OFF      2
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_MSK         0x00000002
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_OFF         1
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_MSK         0x00000001
#define COREOTN_FMF_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_OFF         0

/* index definitions for PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG */
#define PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_INDEX_N_MIN         0
#define PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_INDEX_N_MAX         11
#define PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_INDEX_N_SIZE        12
#define PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_INDEX_N_OFFSET      0x4

/*-------------------------------------------------------.
 | Register (0x000ac0cc + (N) * 0x4) PRBS_DEFECT_CFG_REG |
 +-------------------------------------------------------+
 | bit  0 R/W  PRBS_INS_DEFECT                           |
 +------------------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_UNUSED_MASK         0xfffffffe
#define COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_MSK 0x00000001
#define COREOTN_FMF_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_OFF 0

/* index definitions for PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG */
#define PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG_INDEX_N_MIN           0
#define PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG_INDEX_N_MAX           11
#define PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG_INDEX_N_SIZE          12
#define PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------.
 | Register (0x000ac0fc + (N) * 0x4) PRBS_TSE_STAT_REG |
 +-----------------------------------------------------+
 | bit  31:0 R  PRBS_TSE_CHAN_REG                      |
 +----------------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_TSE_STAT_REG_UNUSED_MASK           0x00000000
#define COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_MSK 0xffffffff
#define COREOTN_FMF_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_OFF 0

/*-----------------------------------------------.
 | Register 0x000ac12c PRBS_BER_TH_LOWER_CFG_REG |
 +-----------------------------------------------+
 | bit  31:0 R/W  PRBS_BER_THRES                 |
 +----------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_UNUSED_MASK        0x00000000
#define COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_MSK 0xffffffff
#define COREOTN_FMF_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF 0

/*-----------------------------------------------.
 | Register 0x000ac130 PRBS_BER_TH_UPPER_CFG_REG |
 +-----------------------------------------------+
 | bit  2:0 R/W  PRBS_BER_THRES                  |
 +----------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_UNUSED_MASK        0xfffffff8
#define COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_MSK 0x00000007
#define COREOTN_FMF_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF 0

/*--------------------------------------.
 | Register 0x000ac134 PRBS_RESEED_REG  |
 +--------------------------------------+
 | bit  31:16 R/W  PRBS_RESEED_THR      |
 | bit  15:0  R/W  PRBS_RESEED_INTERVAL |
 +-------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_RESEED_REG_UNUSED_MASK              0x00000000
#define COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_MSK      0xffff0000
#define COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF      16
#define COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_MSK 0x0000ffff
#define COREOTN_FMF_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF 0

/*------------------------------------------------.
 | Register 0x000ac140 PRBS_TSE_ROLLOVER_STAT_REG |
 +------------------------------------------------+
 | bit  11:0 R  PRBS_TSE_ROVER                    |
 +-----------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_UNUSED_MASK        0xfffff000
#define COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_MSK 0x00000fff
#define COREOTN_FMF_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_OFF 0

/*------------------------------------------.
 | Register 0x000ac144 PRBS_DLSS_INT_EN_REG |
 +------------------------------------------+
 | bit  11:0 R/W  PRBS_DLSS_E               |
 +-----------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_UNUSED_MASK     0xfffff000
#define COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_MSK 0x00000fff
#define COREOTN_FMF_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF 0

/*--------------------------------------------.
 | Register 0x000ac148 PRBS_DLSS_INT_STAT_REG |
 +--------------------------------------------+
 | bit  11:0 R/W  PRBS_DLSS_I                 |
 +-------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_UNUSED_MASK     0xfffff000
#define COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK 0x00000fff
#define COREOTN_FMF_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF 0

/*-------------------------------------------.
 | Register 0x000ac14c PRBS_DLSS_INT_VAL_REG |
 +-------------------------------------------+
 | bit  11:0 R  PRBS_DLSS_V                  |
 +------------------------------------------*/
#define PMC_COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_UNUSED_MASK     0xfffff000
#define COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_MSK 0x00000fff
#define COREOTN_FMF_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COREOTN_FMF_REGS_H */
