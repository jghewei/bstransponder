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
 *     tl_ic block
 *****************************************************************************/
#ifndef _TL_IC_REGS_H
#define _TL_IC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TL_IC_REG_LP_CTRL                 0x00000000
#define PMC_TL_IC_REG_RAM_LP_CTRL             0x00000004
#define PMC_TL_IC_REG_LINE_SERDES_IDDQ        0x00000010
#define PMC_TL_IC_REG_SYS_SERDES_IDDQ_1       0x00000014
#define PMC_TL_IC_REG_SYS_SERDES_IDDQ_2       0x00000018
#define PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ     0x0000001c
#define PMC_TL_IC_REG_DCSU_IDDQ               0x00000020
#define PMC_TL_IC_REG_RESET_CTRL0             0x00000030
#define PMC_TL_IC_REG_RESET_CTRL1             0x00000034
#define PMC_TL_IC_REG_RESET_CTRL2             0x00000038
#define PMC_TL_IC_REG_RESET_CTRL3             0x0000003c
#define PMC_TL_IC_REG_IRIG_CTRL               0x00000070
#define PMC_TL_IC_REG_LINE_SERDES_INTR_SUM    0x00000080
#define PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1   0x00000084
#define PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2   0x00000088
#define PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM 0x0000008c
#define PMC_TL_IC_REG_DCSU_INTR_SUM           0x00000090
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1   0x00000100
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B   0x00000108
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_1     0x0000010c
#define PMC_TL_IC_REG_DCSU_INTR_EN_1          0x00000110
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2   0x00000140
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A   0x00000144
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B   0x00000148
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_2     0x0000014c
#define PMC_TL_IC_REG_DCSU_INTR_EN_2          0x00000150
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3   0x00000180
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A   0x00000184
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B   0x00000188
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_3     0x0000018c
#define PMC_TL_IC_REG_DCSU_INTR_EN_3          0x00000190
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4   0x000001c0
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A   0x000001c4
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B   0x000001c8
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_4     0x000001cc
#define PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC   0x00000240
#define PMC_TL_IC_REG_APRAM_CFG               0x0000024c
#define PMC_TL_IC_REG_GPCFG( N )              (0x00000250 + (N) * 4)
#define PMC_TL_IC_REG_AVS_STATUS              0x00000284

/*-----------------------------------.
 | Register 0x00000000 LP_CTRL       |
 +-----------------------------------+
 | bit  13 R/W  PGMRCLK_LOW_PWR      |
 | bit  12 R/W  LINE_DSIS_LOW_PWR    |
 | bit  11 R/W  COREOTN_LOW_PWR      |
 | bit  10 R/W  SYSOTN_LOW_PWR       |
 | bit  9  R/W  SIFD_LOW_PWR         |
 | bit  8  R/W  MAPOTN_LOW_PWR       |
 | bit  7  R/W  ENET120_LINE_LOW_PWR |
 | bit  6  R/W  ENET120_SYS_LOW_PWR  |
 | bit  5  R/W  MCPB_LOW_PWR         |
 | bit  4  R/W  DCPB_LOW_PWR         |
 | bit  3  R/W  CBRC_LOW_PWR         |
 | bit  2  R/W  LIFD_MUX_LOW_PWR     |
 | bit  1  R/W  LINEOTN_LOW_PWR      |
 | bit  0  R/W  DIGI_M1_LOW_PWR      |
 +----------------------------------*/
#define PMC_TL_IC_REG_LP_CTRL_UNUSED_MASK              0xffffc000
#define TL_IC_REG_LP_CTRL_BIT_PGMRCLK_LOW_PWR_MSK      0x00002000
#define TL_IC_REG_LP_CTRL_BIT_PGMRCLK_LOW_PWR_OFF      13
#define TL_IC_REG_LP_CTRL_BIT_LINE_DSIS_LOW_PWR_MSK    0x00001000
#define TL_IC_REG_LP_CTRL_BIT_LINE_DSIS_LOW_PWR_OFF    12
#define TL_IC_REG_LP_CTRL_BIT_COREOTN_LOW_PWR_MSK      0x00000800
#define TL_IC_REG_LP_CTRL_BIT_COREOTN_LOW_PWR_OFF      11
#define TL_IC_REG_LP_CTRL_BIT_SYSOTN_LOW_PWR_MSK       0x00000400
#define TL_IC_REG_LP_CTRL_BIT_SYSOTN_LOW_PWR_OFF       10
#define TL_IC_REG_LP_CTRL_BIT_SIFD_LOW_PWR_MSK         0x00000200
#define TL_IC_REG_LP_CTRL_BIT_SIFD_LOW_PWR_OFF         9
#define TL_IC_REG_LP_CTRL_BIT_MAPOTN_LOW_PWR_MSK       0x00000100
#define TL_IC_REG_LP_CTRL_BIT_MAPOTN_LOW_PWR_OFF       8
#define TL_IC_REG_LP_CTRL_BIT_ENET120_LINE_LOW_PWR_MSK 0x00000080
#define TL_IC_REG_LP_CTRL_BIT_ENET120_LINE_LOW_PWR_OFF 7
#define TL_IC_REG_LP_CTRL_BIT_ENET120_SYS_LOW_PWR_MSK  0x00000040
#define TL_IC_REG_LP_CTRL_BIT_ENET120_SYS_LOW_PWR_OFF  6
#define TL_IC_REG_LP_CTRL_BIT_MCPB_LOW_PWR_MSK         0x00000020
#define TL_IC_REG_LP_CTRL_BIT_MCPB_LOW_PWR_OFF         5
#define TL_IC_REG_LP_CTRL_BIT_DCPB_LOW_PWR_MSK         0x00000010
#define TL_IC_REG_LP_CTRL_BIT_DCPB_LOW_PWR_OFF         4
#define TL_IC_REG_LP_CTRL_BIT_CBRC_LOW_PWR_MSK         0x00000008
#define TL_IC_REG_LP_CTRL_BIT_CBRC_LOW_PWR_OFF         3
#define TL_IC_REG_LP_CTRL_BIT_LIFD_MUX_LOW_PWR_MSK     0x00000004
#define TL_IC_REG_LP_CTRL_BIT_LIFD_MUX_LOW_PWR_OFF     2
#define TL_IC_REG_LP_CTRL_BIT_LINEOTN_LOW_PWR_MSK      0x00000002
#define TL_IC_REG_LP_CTRL_BIT_LINEOTN_LOW_PWR_OFF      1
#define TL_IC_REG_LP_CTRL_BIT_DIGI_M1_LOW_PWR_MSK      0x00000001
#define TL_IC_REG_LP_CTRL_BIT_DIGI_M1_LOW_PWR_OFF      0

/*---------------------------------------.
 | Register 0x00000004 RAM_LP_CTRL       |
 +---------------------------------------+
 | bit  11 R/W  COREOTN_RAM_LOW_PWR      |
 | bit  10 R/W  SYSOTN_RAM_LOW_PWR       |
 | bit  9  R/W  SIFD_RAM_LOW_PWR         |
 | bit  8  R/W  MAPOTN_RAM_LOW_PWR       |
 | bit  7  R/W  ENET120_LINE_RAM_LOW_PWR |
 | bit  6  R/W  ENET120_SYS_RAM_LOW_PWR  |
 | bit  5  R/W  MCPB_RAM_LOW_PWR         |
 | bit  4  R/W  DCPB_RAM_LOW_PWR         |
 | bit  3  R/W  CBRC_RAM_LOW_PWR         |
 | bit  1  R/W  LINEOTN_RAM_LOW_PWR      |
 | bit  0  R/W  DIGI_M1_RAM_LOW_PWR      |
 +--------------------------------------*/
#define PMC_TL_IC_REG_RAM_LP_CTRL_UNUSED_MASK                  0xfffff004
#define TL_IC_REG_RAM_LP_CTRL_BIT_COREOTN_RAM_LOW_PWR_MSK      0x00000800
#define TL_IC_REG_RAM_LP_CTRL_BIT_COREOTN_RAM_LOW_PWR_OFF      11
#define TL_IC_REG_RAM_LP_CTRL_BIT_SYSOTN_RAM_LOW_PWR_MSK       0x00000400
#define TL_IC_REG_RAM_LP_CTRL_BIT_SYSOTN_RAM_LOW_PWR_OFF       10
#define TL_IC_REG_RAM_LP_CTRL_BIT_SIFD_RAM_LOW_PWR_MSK         0x00000200
#define TL_IC_REG_RAM_LP_CTRL_BIT_SIFD_RAM_LOW_PWR_OFF         9
#define TL_IC_REG_RAM_LP_CTRL_BIT_MAPOTN_RAM_LOW_PWR_MSK       0x00000100
#define TL_IC_REG_RAM_LP_CTRL_BIT_MAPOTN_RAM_LOW_PWR_OFF       8
#define TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_LINE_RAM_LOW_PWR_MSK 0x00000080
#define TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_LINE_RAM_LOW_PWR_OFF 7
#define TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_SYS_RAM_LOW_PWR_MSK  0x00000040
#define TL_IC_REG_RAM_LP_CTRL_BIT_ENET120_SYS_RAM_LOW_PWR_OFF  6
#define TL_IC_REG_RAM_LP_CTRL_BIT_MCPB_RAM_LOW_PWR_MSK         0x00000020
#define TL_IC_REG_RAM_LP_CTRL_BIT_MCPB_RAM_LOW_PWR_OFF         5
#define TL_IC_REG_RAM_LP_CTRL_BIT_DCPB_RAM_LOW_PWR_MSK         0x00000010
#define TL_IC_REG_RAM_LP_CTRL_BIT_DCPB_RAM_LOW_PWR_OFF         4
#define TL_IC_REG_RAM_LP_CTRL_BIT_CBRC_RAM_LOW_PWR_MSK         0x00000008
#define TL_IC_REG_RAM_LP_CTRL_BIT_CBRC_RAM_LOW_PWR_OFF         3
#define TL_IC_REG_RAM_LP_CTRL_BIT_LINEOTN_RAM_LOW_PWR_MSK      0x00000002
#define TL_IC_REG_RAM_LP_CTRL_BIT_LINEOTN_RAM_LOW_PWR_OFF      1
#define TL_IC_REG_RAM_LP_CTRL_BIT_DIGI_M1_RAM_LOW_PWR_MSK      0x00000001
#define TL_IC_REG_RAM_LP_CTRL_BIT_DIGI_M1_RAM_LOW_PWR_OFF      0

/*--------------------------------------.
 | Register 0x00000010 LINE_SERDES_IDDQ |
 +--------------------------------------+
 | bit  13   R/W  SFI2_IDDQ             |
 | bit  12   R/W  SFI1_IDDQ             |
 | bit  11:0 R/W  LINE_SERDES_IDDQ      |
 +-------------------------------------*/
#define PMC_TL_IC_REG_LINE_SERDES_IDDQ_UNUSED_MASK          0xffffc000
#define TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI2_IDDQ_MSK        0x00002000
#define TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI2_IDDQ_OFF        13
#define TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI1_IDDQ_MSK        0x00001000
#define TL_IC_REG_LINE_SERDES_IDDQ_BIT_SFI1_IDDQ_OFF        12
#define TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_MSK 0x00000fff
#define TL_IC_REG_LINE_SERDES_IDDQ_BIT_LINE_SERDES_IDDQ_OFF 0

/*---------------------------------------.
 | Register 0x00000014 SYS_SERDES_IDDQ_1 |
 +---------------------------------------+
 | bit  21:20 R/W  LR_CSU_IDDQ           |
 | bit  19:0  R/W  LR_SERDES_IDDQ        |
 +--------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_IDDQ_1_UNUSED_MASK        0xffc00000
#define TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_MSK    0x00300000
#define TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_CSU_IDDQ_OFF    20
#define TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_MSK 0x000fffff
#define TL_IC_REG_SYS_SERDES_IDDQ_1_BIT_LR_SERDES_IDDQ_OFF 0

/*---------------------------------------.
 | Register 0x00000018 SYS_SERDES_IDDQ_2 |
 +---------------------------------------+
 | bit  24    R/W  SR_CSU_IDDQ           |
 | bit  23:16 R/W  SR_SERDES_IDDQ        |
 | bit  15:0  R/W  SYS_SERDES_IDDQ       |
 +--------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_IDDQ_2_UNUSED_MASK         0xfe000000
#define TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_CSU_IDDQ_MSK     0x01000000
#define TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_CSU_IDDQ_OFF     24
#define TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_MSK  0x00ff0000
#define TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SR_SERDES_IDDQ_OFF  16
#define TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_MSK 0x0000ffff
#define TL_IC_REG_SYS_SERDES_IDDQ_2_BIT_SYS_SERDES_IDDQ_OFF 0

/*-----------------------------------------.
 | Register 0x0000001c PCIE_OH_SERDES_IDDQ |
 +-----------------------------------------+
 | bit  15    R/W  OH_CSU_IDDQ             |
 | bit  14:12 R/W  OH_TX_SERDES_IDDQ       |
 | bit  10:8  R/W  OH_RX_SERDES_IDDQ       |
 | bit  4     R/W  PCIE_CSU_IDDQ           |
 | bit  3:0   R/W  PCIE_SERDES_IDDQ        |
 +----------------------------------------*/
#define PMC_TL_IC_REG_PCIE_OH_SERDES_IDDQ_UNUSED_MASK           0xfffe08e0
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_CSU_IDDQ_MSK       0x00008000
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_CSU_IDDQ_OFF       15
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_MSK 0x00007000
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_TX_SERDES_IDDQ_OFF 12
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_MSK 0x00000700
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_OH_RX_SERDES_IDDQ_OFF 8
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_CSU_IDDQ_MSK     0x00000010
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_CSU_IDDQ_OFF     4
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_MSK  0x0000000f
#define TL_IC_REG_PCIE_OH_SERDES_IDDQ_BIT_PCIE_SERDES_IDDQ_OFF  0

/*-------------------------------.
 | Register 0x00000020 DCSU_IDDQ |
 +-------------------------------+
 |                               |
 | bit  8 R/W  FIC2_DCSU_IDDQ    |
 |                               |
 |                               |
 | bit  7 R/W  ENET3_DCSU_IDDQ   |
 |                               |
 | bit  6 R/W  PTP_DCSU_IDDQ     |
 | bit  5 R/W  ENET2_DCSU_IDDQ   |
 | bit  4 R/W  ENET1_DCSU_IDDQ   |
 | bit  1 R/W  OTN2_DCSU_IDDQ    |
 | bit  0 R/W  OTN1_DCSU_IDDQ    |
 +------------------------------*/
#define PMC_TL_IC_REG_DCSU_IDDQ_UNUSED_MASK         0xfffffe00
#define TL_IC_REG_DCSU_IDDQ_BIT_FIC2_DCSU_IDDQ_MSK  0x00000100
#define TL_IC_REG_DCSU_IDDQ_BIT_FIC2_DCSU_IDDQ_OFF  8
#define TL_IC_REG_DCSU_IDDQ_BIT_ENET3_DCSU_IDDQ_MSK 0x00000080
#define TL_IC_REG_DCSU_IDDQ_BIT_ENET3_DCSU_IDDQ_OFF 7
#define TL_IC_REG_DCSU_IDDQ_BIT_PTP_DCSU_IDDQ_MSK   0x00000040
#define TL_IC_REG_DCSU_IDDQ_BIT_PTP_DCSU_IDDQ_OFF   6
#define TL_IC_REG_DCSU_IDDQ_BIT_ENET2_DCSU_IDDQ_MSK 0x00000020
#define TL_IC_REG_DCSU_IDDQ_BIT_ENET2_DCSU_IDDQ_OFF 5
#define TL_IC_REG_DCSU_IDDQ_BIT_ENET1_DCSU_IDDQ_MSK 0x00000010
#define TL_IC_REG_DCSU_IDDQ_BIT_ENET1_DCSU_IDDQ_OFF 4
#define TL_IC_REG_DCSU_IDDQ_BIT_OTN2_DCSU_IDDQ_MSK  0x00000002
#define TL_IC_REG_DCSU_IDDQ_BIT_OTN2_DCSU_IDDQ_OFF  1
#define TL_IC_REG_DCSU_IDDQ_BIT_OTN1_DCSU_IDDQ_MSK  0x00000001
#define TL_IC_REG_DCSU_IDDQ_BIT_OTN1_DCSU_IDDQ_OFF  0

/*---------------------------------.
 | Register 0x00000030 RESET_CTRL0 |
 +---------------------------------+
 | bit  26 R/W  LINE_DSIS_RESET    |
 | bit  25 R/W  PGMRCLK_RESET      |
 | bit  24 R/W  PTP_DCSU_RESET     |
 | bit  23 R/W  ENET2_DCSU_RESET   |
 | bit  22 R/W  ENET1_DCSU_RESET   |
 |                                 |
 | bit  19 R/W  FIC2_DCSU_RESET    |
 |                                 |
 |                                 |
 | bit  18 R/W  ENET3_DCSU_RESET   |
 |                                 |
 | bit  17 R/W  OTN2_DCSU_RESET    |
 | bit  16 R/W  OTN1_DCSU_RESET    |
 | bit  11 R/W  COREOTN_RESET      |
 | bit  10 R/W  SYSOTN_RESET       |
 | bit  9  R/W  SIFD_RESET         |
 | bit  8  R/W  MAPOTN_RESET       |
 | bit  7  R/W  ENET120_LINE_RESET |
 | bit  6  R/W  ENET120_SYS_RESET  |
 | bit  5  R/W  MCPB_RESET         |
 | bit  4  R/W  DCPB_RESET         |
 | bit  3  R/W  CBRC_RESET         |
 | bit  2  R/W  LIFD_MUX_RESET     |
 | bit  1  R/W  LINEOTN_RESET      |
 | bit  0  R/W  DIGI_M1_RESET      |
 +--------------------------------*/
#define PMC_TL_IC_REG_RESET_CTRL0_UNUSED_MASK            0xf800f000
#define TL_IC_REG_RESET_CTRL0_BIT_LINE_DSIS_RESET_MSK    0x04000000
#define TL_IC_REG_RESET_CTRL0_BIT_LINE_DSIS_RESET_OFF    26
#define TL_IC_REG_RESET_CTRL0_BIT_PGMRCLK_RESET_MSK      0x02000000
#define TL_IC_REG_RESET_CTRL0_BIT_PGMRCLK_RESET_OFF      25
#define TL_IC_REG_RESET_CTRL0_BIT_PTP_DCSU_RESET_MSK     0x01000000
#define TL_IC_REG_RESET_CTRL0_BIT_PTP_DCSU_RESET_OFF     24
#define TL_IC_REG_RESET_CTRL0_BIT_ENET2_DCSU_RESET_MSK   0x00800000
#define TL_IC_REG_RESET_CTRL0_BIT_ENET2_DCSU_RESET_OFF   23
#define TL_IC_REG_RESET_CTRL0_BIT_ENET1_DCSU_RESET_MSK   0x00400000
#define TL_IC_REG_RESET_CTRL0_BIT_ENET1_DCSU_RESET_OFF   22
#define TL_IC_REG_RESET_CTRL0_BIT_FIC2_DCSU_RESET_MSK    0x00080000
#define TL_IC_REG_RESET_CTRL0_BIT_FIC2_DCSU_RESET_OFF    19
#define TL_IC_REG_RESET_CTRL0_BIT_ENET3_DCSU_RESET_MSK   0x00040000
#define TL_IC_REG_RESET_CTRL0_BIT_ENET3_DCSU_RESET_OFF   18
#define TL_IC_REG_RESET_CTRL0_BIT_OTN2_DCSU_RESET_MSK    0x00020000
#define TL_IC_REG_RESET_CTRL0_BIT_OTN2_DCSU_RESET_OFF    17
#define TL_IC_REG_RESET_CTRL0_BIT_OTN1_DCSU_RESET_MSK    0x00010000
#define TL_IC_REG_RESET_CTRL0_BIT_OTN1_DCSU_RESET_OFF    16
#define TL_IC_REG_RESET_CTRL0_BIT_COREOTN_RESET_MSK      0x00000800
#define TL_IC_REG_RESET_CTRL0_BIT_COREOTN_RESET_OFF      11
#define TL_IC_REG_RESET_CTRL0_BIT_SYSOTN_RESET_MSK       0x00000400
#define TL_IC_REG_RESET_CTRL0_BIT_SYSOTN_RESET_OFF       10
#define TL_IC_REG_RESET_CTRL0_BIT_SIFD_RESET_MSK         0x00000200
#define TL_IC_REG_RESET_CTRL0_BIT_SIFD_RESET_OFF         9
#define TL_IC_REG_RESET_CTRL0_BIT_MAPOTN_RESET_MSK       0x00000100
#define TL_IC_REG_RESET_CTRL0_BIT_MAPOTN_RESET_OFF       8
#define TL_IC_REG_RESET_CTRL0_BIT_ENET120_LINE_RESET_MSK 0x00000080
#define TL_IC_REG_RESET_CTRL0_BIT_ENET120_LINE_RESET_OFF 7
#define TL_IC_REG_RESET_CTRL0_BIT_ENET120_SYS_RESET_MSK  0x00000040
#define TL_IC_REG_RESET_CTRL0_BIT_ENET120_SYS_RESET_OFF  6
#define TL_IC_REG_RESET_CTRL0_BIT_MCPB_RESET_MSK         0x00000020
#define TL_IC_REG_RESET_CTRL0_BIT_MCPB_RESET_OFF         5
#define TL_IC_REG_RESET_CTRL0_BIT_DCPB_RESET_MSK         0x00000010
#define TL_IC_REG_RESET_CTRL0_BIT_DCPB_RESET_OFF         4
#define TL_IC_REG_RESET_CTRL0_BIT_CBRC_RESET_MSK         0x00000008
#define TL_IC_REG_RESET_CTRL0_BIT_CBRC_RESET_OFF         3
#define TL_IC_REG_RESET_CTRL0_BIT_LIFD_MUX_RESET_MSK     0x00000004
#define TL_IC_REG_RESET_CTRL0_BIT_LIFD_MUX_RESET_OFF     2
#define TL_IC_REG_RESET_CTRL0_BIT_LINEOTN_RESET_MSK      0x00000002
#define TL_IC_REG_RESET_CTRL0_BIT_LINEOTN_RESET_OFF      1
#define TL_IC_REG_RESET_CTRL0_BIT_DIGI_M1_RESET_MSK      0x00000001
#define TL_IC_REG_RESET_CTRL0_BIT_DIGI_M1_RESET_OFF      0

/*---------------------------------.
 | Register 0x00000034 RESET_CTRL1 |
 +---------------------------------+
 | bit  24    R/W  C8_CSU_RESET    |
 | bit  23:16 R/W  C8_RESET        |
 | bit  15:0  R/W  S16_SYS_RESET   |
 +--------------------------------*/
#define PMC_TL_IC_REG_RESET_CTRL1_UNUSED_MASK       0xfe000000
#define TL_IC_REG_RESET_CTRL1_BIT_C8_CSU_RESET_MSK  0x01000000
#define TL_IC_REG_RESET_CTRL1_BIT_C8_CSU_RESET_OFF  24
#define TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_MSK      0x00ff0000
#define TL_IC_REG_RESET_CTRL1_BIT_C8_RESET_OFF      16
#define TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_MSK 0x0000ffff
#define TL_IC_REG_RESET_CTRL1_BIT_S16_SYS_RESET_OFF 0

/*---------------------------------.
 | Register 0x00000038 RESET_CTRL2 |
 +---------------------------------+
 | bit  21:20 R/W  TITAN_CSU_RESET |
 | bit  19:0  R/W  TITAN_RESET     |
 +--------------------------------*/
#define PMC_TL_IC_REG_RESET_CTRL2_UNUSED_MASK         0xffc00000
#define TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_MSK 0x00300000
#define TL_IC_REG_RESET_CTRL2_BIT_TITAN_CSU_RESET_OFF 20
#define TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_MSK     0x000fffff
#define TL_IC_REG_RESET_CTRL2_BIT_TITAN_RESET_OFF     0

/*------------------------------------.
 | Register 0x0000003c RESET_CTRL3    |
 +------------------------------------+
 | bit  31    R/W  D8_CSU_RESET       |
 | bit  30:28 R/W  OH_SERDES_TX_RESET |
 | bit  27:25 R/W  OH_SERDES_RX_RESET |
 | bit  24    R/W  C8_PCIE_CSU_RESET  |
 | bit  23:20 R/W  C8_PCIE_RESET      |
 | bit  19:18 R/W  SFI51_TX_RESET     |
 | bit  17:16 R/W  SFI51_RX_RESET     |
 | bit  11:0  R/W  S16_LINE_RESET     |
 +-----------------------------------*/
#define PMC_TL_IC_REG_RESET_CTRL3_UNUSED_MASK            0x00000000
#define TL_IC_REG_RESET_CTRL3_BIT_D8_CSU_RESET_MSK       0x80000000
#define TL_IC_REG_RESET_CTRL3_BIT_D8_CSU_RESET_OFF       31
#define TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_MSK 0x70000000
#define TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_TX_RESET_OFF 28
#define TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_MSK 0x0e000000
#define TL_IC_REG_RESET_CTRL3_BIT_OH_SERDES_RX_RESET_OFF 25
#define TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_CSU_RESET_MSK  0x01000000
#define TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_CSU_RESET_OFF  24
#define TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_MSK      0x00f00000
#define TL_IC_REG_RESET_CTRL3_BIT_C8_PCIE_RESET_OFF      20
#define TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_MSK     0x000c0000
#define TL_IC_REG_RESET_CTRL3_BIT_SFI51_TX_RESET_OFF     18
#define TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_MSK     0x00030000
#define TL_IC_REG_RESET_CTRL3_BIT_SFI51_RX_RESET_OFF     16
#define TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_MSK     0x00000fff
#define TL_IC_REG_RESET_CTRL3_BIT_S16_LINE_RESET_OFF     0

/*-------------------------------.
 | Register 0x00000070 IRIG_CTRL |
 +-------------------------------+
 | bit  3 R/W  IRIG_ONE_PPS_O_EN |
 | bit  2 R/W  IRIG_DATO_EN      |
 | bit  1 R/W  IRIG_CLK0_EN      |
 | bit  0 R/W  IRIG_ENET_SOURCE  |
 +------------------------------*/
#define PMC_TL_IC_REG_IRIG_CTRL_UNUSED_MASK           0xfffffff0
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_ONE_PPS_O_EN_MSK 0x00000008
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_ONE_PPS_O_EN_OFF 3
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_DATO_EN_MSK      0x00000004
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_DATO_EN_OFF      2
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_CLK0_EN_MSK      0x00000002
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_CLK0_EN_OFF      1
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_ENET_SOURCE_MSK  0x00000001
#define TL_IC_REG_IRIG_CTRL_BIT_IRIG_ENET_SOURCE_OFF  0

/*------------------------------------------.
 | Register 0x00000080 LINE_SERDES_INTR_SUM |
 +------------------------------------------+
 | bit  15   R  SFI2_TX_INTR                |
 | bit  14   R  SFI2_RX_INTR                |
 | bit  13   R  SFI1_TX_INTR                |
 | bit  12   R  SFI1_RX_INTR                |
 | bit  11:0 R  LINE_SERDES_INTR            |
 +-----------------------------------------*/
#define PMC_TL_IC_REG_LINE_SERDES_INTR_SUM_UNUSED_MASK          0xffff0000
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_TX_INTR_MSK     0x00008000
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_TX_INTR_OFF     15
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_RX_INTR_MSK     0x00004000
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI2_RX_INTR_OFF     14
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_TX_INTR_MSK     0x00002000
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_TX_INTR_OFF     13
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_RX_INTR_MSK     0x00001000
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_SFI1_RX_INTR_OFF     12
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_MSK 0x00000fff
#define TL_IC_REG_LINE_SERDES_INTR_SUM_BIT_LINE_SERDES_INTR_OFF 0

/*-------------------------------------------.
 | Register 0x00000084 SYS_SERDES_INTR_SUM_1 |
 +-------------------------------------------+
 | bit  21:20 R  LR_CSU_INTR                 |
 | bit  19:0  R  LR_SERDES_INTR              |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_1_UNUSED_MASK        0xffc00000
#define TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_MSK    0x00300000
#define TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_CSU_INTR_OFF    20
#define TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_MSK 0x000fffff
#define TL_IC_REG_SYS_SERDES_INTR_SUM_1_BIT_LR_SERDES_INTR_OFF 0

/*-------------------------------------------.
 | Register 0x00000088 SYS_SERDES_INTR_SUM_2 |
 +-------------------------------------------+
 | bit  24    R  SR_CSU_INTR                 |
 | bit  23:16 R  SR_SERDES_INTR              |
 | bit  15:0  R  SYS_SERDES_INTR             |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_SUM_2_UNUSED_MASK         0xfe000000
#define TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_CSU_INTR_MSK     0x01000000
#define TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_CSU_INTR_OFF     24
#define TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_MSK  0x00ff0000
#define TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SR_SERDES_INTR_OFF  16
#define TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_MSK 0x0000ffff
#define TL_IC_REG_SYS_SERDES_INTR_SUM_2_BIT_SYS_SERDES_INTR_OFF 0

/*---------------------------------------------.
 | Register 0x0000008c PCIE_OH_SERDES_INTR_SUM |
 +---------------------------------------------+
 | bit  16    R  MGMT_FEGE_INTR                |
 | bit  15    R  OH_CSU_INTR                   |
 | bit  14:12 R  OH_TX_SERDES_INTR             |
 | bit  10:8  R  OH_RX_SERDES_INTR             |
 | bit  4     R  PCIE_CSU_INTR                 |
 | bit  3:0   R  PCIE_SERDES_INTR              |
 +--------------------------------------------*/
#define PMC_TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_UNUSED_MASK           0xfffe08e0
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_MGMT_FEGE_INTR_MSK    0x00010000
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_MGMT_FEGE_INTR_OFF    16
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_CSU_INTR_MSK       0x00008000
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_CSU_INTR_OFF       15
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_MSK 0x00007000
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_TX_SERDES_INTR_OFF 12
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_MSK 0x00000700
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_OH_RX_SERDES_INTR_OFF 8
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_CSU_INTR_MSK     0x00000010
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_CSU_INTR_OFF     4
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_MSK  0x0000000f
#define TL_IC_REG_PCIE_OH_SERDES_INTR_SUM_BIT_PCIE_SERDES_INTR_OFF  0

/*-----------------------------------.
 | Register 0x00000090 DCSU_INTR_SUM |
 +-----------------------------------+
 |                                   |
 | bit  8 R  FIC2_DCSU_INTR          |
 |                                   |
 |                                   |
 | bit  7 R  FIC1_DCSU_INTR          |
 |                                   |
 | bit  6 R  PTP_DCSU_INTR           |
 | bit  5 R  ENET2_DCSU_INTR         |
 | bit  4 R  ENET1_DCSU_INTR         |
 | bit  3 R  SYS2_DCSU_INTR          |
 | bit  2 R  SYS_DCSU_INTR           |
 | bit  1 R  OTN2_DCSU_INTR          |
 | bit  0 R  OTN1_DCSU_INTR          |
 +----------------------------------*/
#define PMC_TL_IC_REG_DCSU_INTR_SUM_UNUSED_MASK         0xfffffe00
#define TL_IC_REG_DCSU_INTR_SUM_BIT_FIC2_DCSU_INTR_MSK  0x00000100
#define TL_IC_REG_DCSU_INTR_SUM_BIT_FIC2_DCSU_INTR_OFF  8
#define TL_IC_REG_DCSU_INTR_SUM_BIT_FIC1_DCSU_INTR_MSK  0x00000080
#define TL_IC_REG_DCSU_INTR_SUM_BIT_FIC1_DCSU_INTR_OFF  7
#define TL_IC_REG_DCSU_INTR_SUM_BIT_PTP_DCSU_INTR_MSK   0x00000040
#define TL_IC_REG_DCSU_INTR_SUM_BIT_PTP_DCSU_INTR_OFF   6
#define TL_IC_REG_DCSU_INTR_SUM_BIT_ENET2_DCSU_INTR_MSK 0x00000020
#define TL_IC_REG_DCSU_INTR_SUM_BIT_ENET2_DCSU_INTR_OFF 5
#define TL_IC_REG_DCSU_INTR_SUM_BIT_ENET1_DCSU_INTR_MSK 0x00000010
#define TL_IC_REG_DCSU_INTR_SUM_BIT_ENET1_DCSU_INTR_OFF 4
#define TL_IC_REG_DCSU_INTR_SUM_BIT_SYS2_DCSU_INTR_MSK  0x00000008
#define TL_IC_REG_DCSU_INTR_SUM_BIT_SYS2_DCSU_INTR_OFF  3
#define TL_IC_REG_DCSU_INTR_SUM_BIT_SYS_DCSU_INTR_MSK   0x00000004
#define TL_IC_REG_DCSU_INTR_SUM_BIT_SYS_DCSU_INTR_OFF   2
#define TL_IC_REG_DCSU_INTR_SUM_BIT_OTN2_DCSU_INTR_MSK  0x00000002
#define TL_IC_REG_DCSU_INTR_SUM_BIT_OTN2_DCSU_INTR_OFF  1
#define TL_IC_REG_DCSU_INTR_SUM_BIT_OTN1_DCSU_INTR_MSK  0x00000001
#define TL_IC_REG_DCSU_INTR_SUM_BIT_OTN1_DCSU_INTR_OFF  0

/*-------------------------------------------.
 | Register 0x00000100 LINE_SERDES_INTR_EN_1 |
 +-------------------------------------------+
 | bit  15 R/W  SFI2_TX_INTR_EN_MIPS         |
 | bit  14 R/W  SFI2_RX_INTR_EN_MIPS         |
 | bit  13 R/W  SFI1_TX_INTR_EN_MIPS         |
 | bit  12 R/W  SFI1_RX_INTR_EN_MIPS         |
 +------------------------------------------*/
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_1_UNUSED_MASK              0xffff0000
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_TX_INTR_EN_MIPS_MSK 0x00008000
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_TX_INTR_EN_MIPS_OFF 15
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_RX_INTR_EN_MIPS_MSK 0x00004000
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI2_RX_INTR_EN_MIPS_OFF 14
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_TX_INTR_EN_MIPS_MSK 0x00002000
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_TX_INTR_EN_MIPS_OFF 13
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_RX_INTR_EN_MIPS_MSK 0x00001000
#define TL_IC_REG_LINE_SERDES_INTR_EN_1_BIT_SFI1_RX_INTR_EN_MIPS_OFF 12

/*-------------------------------------------.
 | Register 0x00000108 SYS_SERDES_INTR_EN_1B |
 +-------------------------------------------+
 | bit  24 R/W  SR_CSU_INTR_EN_MIPS          |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_1B_UNUSED_MASK             0xfe000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_1B_BIT_SR_CSU_INTR_EN_MIPS_MSK 0x01000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_1B_BIT_SR_CSU_INTR_EN_MIPS_OFF 24

/*-----------------------------------------.
 | Register 0x0000010c OH_SERDES_INTR_EN_1 |
 +-----------------------------------------+
 | bit  16 R/W  MGMT_FEGE_INTR_EN_MIPS     |
 | bit  15 R/W  OH_CSU_INTR_EN_MIPS        |
 | bit  4  R/W  PCIE_CSU_INTR_EN_MIPS      |
 +----------------------------------------*/
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_1_UNUSED_MASK                0xfffe08e0
#define TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_MGMT_FEGE_INTR_EN_MIPS_MSK 0x00010000
#define TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_MGMT_FEGE_INTR_EN_MIPS_OFF 16
#define TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_OH_CSU_INTR_EN_MIPS_MSK    0x00008000
#define TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_OH_CSU_INTR_EN_MIPS_OFF    15
#define TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_PCIE_CSU_INTR_EN_MIPS_MSK  0x00000010
#define TL_IC_REG_OH_SERDES_INTR_EN_1_BIT_PCIE_CSU_INTR_EN_MIPS_OFF  4

/*-------------------------------------.
 | Register 0x00000110 DCSU_INTR_EN_1  |
 +-------------------------------------+
 |                                     |
 | bit  8 R/W  FIC2_DCSU_INTR_EN_MIPS  |
 |                                     |
 |                                     |
 | bit  7 R/W  FIC1_DCSU_INTR_EN_MIPS  |
 |                                     |
 | bit  6 R/W  PTP_DCSU_INTR_EN_MIPS   |
 | bit  5 R/W  ENET2_DCSU_INTR_EN_MIPS |
 | bit  4 R/W  ENET1_DCSU_INTR_EN_MIPS |
 | bit  3 R/W  SYS2_DCSU_INTR_EN_MIPS  |
 | bit  2 R/W  SYS_DCSU_INTR_EN_MIPS   |
 | bit  1 R/W  OTN2_DCSU_INTR_EN_MIPS  |
 | bit  0 R/W  OTN1_DCSU_INTR_EN_MIPS  |
 +------------------------------------*/
#define PMC_TL_IC_REG_DCSU_INTR_EN_1_UNUSED_MASK                 0xfffffe00
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC2_DCSU_INTR_EN_MIPS_MSK  0x00000100
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC2_DCSU_INTR_EN_MIPS_OFF  8
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC1_DCSU_INTR_EN_MIPS_MSK  0x00000080
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_FIC1_DCSU_INTR_EN_MIPS_OFF  7
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_PTP_DCSU_INTR_EN_MIPS_MSK   0x00000040
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_PTP_DCSU_INTR_EN_MIPS_OFF   6
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET2_DCSU_INTR_EN_MIPS_MSK 0x00000020
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET2_DCSU_INTR_EN_MIPS_OFF 5
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET1_DCSU_INTR_EN_MIPS_MSK 0x00000010
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_ENET1_DCSU_INTR_EN_MIPS_OFF 4
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS2_DCSU_INTR_EN_MIPS_MSK  0x00000008
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS2_DCSU_INTR_EN_MIPS_OFF  3
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS_DCSU_INTR_EN_MIPS_MSK   0x00000004
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_SYS_DCSU_INTR_EN_MIPS_OFF   2
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN2_DCSU_INTR_EN_MIPS_MSK  0x00000002
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN2_DCSU_INTR_EN_MIPS_OFF  1
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN1_DCSU_INTR_EN_MIPS_MSK  0x00000001
#define TL_IC_REG_DCSU_INTR_EN_1_BIT_OTN1_DCSU_INTR_EN_MIPS_OFF  0

/*-------------------------------------------.
 | Register 0x00000140 LINE_SERDES_INTR_EN_2 |
 +-------------------------------------------+
 | bit  15   R/W  SFI2_TX_INTR_EN_PCIE       |
 | bit  14   R/W  SFI2_RX_INTR_EN_PCIE       |
 | bit  13   R/W  SFI1_TX_INTR_EN_PCIE       |
 | bit  12   R/W  SFI1_RX_INTR_EN_PCIE       |
 | bit  11:0 R/W  LINE_SERDES_INTR_EN_DEVICE |
 +------------------------------------------*/
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_2_UNUSED_MASK                    0xffff0000
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_TX_INTR_EN_PCIE_MSK       0x00008000
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_TX_INTR_EN_PCIE_OFF       15
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_RX_INTR_EN_PCIE_MSK       0x00004000
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI2_RX_INTR_EN_PCIE_OFF       14
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_TX_INTR_EN_PCIE_MSK       0x00002000
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_TX_INTR_EN_PCIE_OFF       13
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_RX_INTR_EN_PCIE_MSK       0x00001000
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_SFI1_RX_INTR_EN_PCIE_OFF       12
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_MSK 0x00000fff
#define TL_IC_REG_LINE_SERDES_INTR_EN_2_BIT_LINE_SERDES_INTR_EN_DEVICE_OFF 0

/*-------------------------------------------.
 | Register 0x00000144 SYS_SERDES_INTR_EN_2A |
 +-------------------------------------------+
 | bit  21:20 R/W  LR_CSU_INTR_EN_DEVICE     |
 | bit  19:0  R/W  LR_SERDES_INTR_EN_DEVICE  |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2A_UNUSED_MASK                  0xffc00000
#define TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_MSK    0x00300000
#define TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_CSU_INTR_EN_DEVICE_OFF    20
#define TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_MSK 0x000fffff
#define TL_IC_REG_SYS_SERDES_INTR_EN_2A_BIT_LR_SERDES_INTR_EN_DEVICE_OFF 0

/*-------------------------------------------.
 | Register 0x00000148 SYS_SERDES_INTR_EN_2B |
 +-------------------------------------------+
 | bit  24    R/W  SR_CSU_INTR_EN_PCIE       |
 | bit  23:16 R/W  SR_SERDES_INTR_EN_DEVICE  |
 | bit  15:0  R/W  SYS_SERDES_INTR_EN_DEVICE |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_2B_UNUSED_MASK                   0xfe000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_CSU_INTR_EN_PCIE_MSK       0x01000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_CSU_INTR_EN_PCIE_OFF       24
#define TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_MSK  0x00ff0000
#define TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SR_SERDES_INTR_EN_DEVICE_OFF  16
#define TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_MSK 0x0000ffff
#define TL_IC_REG_SYS_SERDES_INTR_EN_2B_BIT_SYS_SERDES_INTR_EN_DEVICE_OFF 0

/*---------------------------------------------.
 | Register 0x0000014c OH_SERDES_INTR_EN_2     |
 +---------------------------------------------+
 | bit  16    R/W  MGMT_FEGE_INTR_EN_PCIE      |
 | bit  15    R/W  OH_CSU_INTR_EN_PCIE         |
 | bit  14:12 R/W  OH_TX_SERDES_INTR_EN_DEVICE |
 | bit  10:8  R/W  OH_RX_SERDES_INTR_EN_DEVICE |
 | bit  4     R/W  PCIE_CSU_INTR_EN_PCIE       |
 | bit  3:0   R/W  PCIE_SERDES_INTR_EN_DEVICE  |
 +--------------------------------------------*/
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_2_UNUSED_MASK                     0xfffe08e0
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_MGMT_FEGE_INTR_EN_PCIE_MSK      0x00010000
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_MGMT_FEGE_INTR_EN_PCIE_OFF      16
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_CSU_INTR_EN_PCIE_MSK         0x00008000
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_CSU_INTR_EN_PCIE_OFF         15
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_MSK 0x00007000
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_TX_SERDES_INTR_EN_DEVICE_OFF 12
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_MSK 0x00000700
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_OH_RX_SERDES_INTR_EN_DEVICE_OFF 8
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_CSU_INTR_EN_PCIE_MSK       0x00000010
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_CSU_INTR_EN_PCIE_OFF       4
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_MSK  0x0000000f
#define TL_IC_REG_OH_SERDES_INTR_EN_2_BIT_PCIE_SERDES_INTR_EN_DEVICE_OFF  0

/*-------------------------------------.
 | Register 0x00000150 DCSU_INTR_EN_2  |
 +-------------------------------------+
 |                                     |
 | bit  8 R/W  FIC2_DCSU_INTR_EN_PCIE  |
 |                                     |
 |                                     |
 | bit  7 R/W  FIC1_DCSU_INTR_EN_PCIE  |
 |                                     |
 | bit  6 R/W  PTP_DCSU_INTR_EN_PCIE   |
 | bit  5 R/W  ENET2_DCSU_INTR_EN_PCIE |
 | bit  4 R/W  ENET1_DCSU_INTR_EN_PCIE |
 | bit  3 R/W  SYS2_DCSU_INTR_EN_PCIE  |
 | bit  2 R/W  SYS_DCSU_INTR_EN_PCIE   |
 | bit  1 R/W  OTN2_DCSU_INTR_EN_PCIE  |
 | bit  0 R/W  OTN1_DCSU_INTR_EN_PCIE  |
 +------------------------------------*/
#define PMC_TL_IC_REG_DCSU_INTR_EN_2_UNUSED_MASK                 0xfffffe00
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC2_DCSU_INTR_EN_PCIE_MSK  0x00000100
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC2_DCSU_INTR_EN_PCIE_OFF  8
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC1_DCSU_INTR_EN_PCIE_MSK  0x00000080
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_FIC1_DCSU_INTR_EN_PCIE_OFF  7
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_PTP_DCSU_INTR_EN_PCIE_MSK   0x00000040
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_PTP_DCSU_INTR_EN_PCIE_OFF   6
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET2_DCSU_INTR_EN_PCIE_MSK 0x00000020
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET2_DCSU_INTR_EN_PCIE_OFF 5
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET1_DCSU_INTR_EN_PCIE_MSK 0x00000010
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_ENET1_DCSU_INTR_EN_PCIE_OFF 4
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS2_DCSU_INTR_EN_PCIE_MSK  0x00000008
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS2_DCSU_INTR_EN_PCIE_OFF  3
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS_DCSU_INTR_EN_PCIE_MSK   0x00000004
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_SYS_DCSU_INTR_EN_PCIE_OFF   2
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN2_DCSU_INTR_EN_PCIE_MSK  0x00000002
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN2_DCSU_INTR_EN_PCIE_OFF  1
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN1_DCSU_INTR_EN_PCIE_MSK  0x00000001
#define TL_IC_REG_DCSU_INTR_EN_2_BIT_OTN1_DCSU_INTR_EN_PCIE_OFF  0

/*-------------------------------------------.
 | Register 0x00000180 LINE_SERDES_INTR_EN_3 |
 +-------------------------------------------+
 | bit  15   R/W  SFI2_TX_INTR_EN_DEVICE     |
 | bit  14   R/W  SFI2_RX_INTR_EN_DEVICE     |
 | bit  13   R/W  SFI1_TX_INTR_EN_DEVICE     |
 | bit  12   R/W  SFI1_RX_INTR_EN_DEVICE     |
 | bit  11:0 R/W  LINE_SERDES_INTR_EN_PCIE   |
 +------------------------------------------*/
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_3_UNUSED_MASK                  0xffff0000
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_TX_INTR_EN_DEVICE_MSK   0x00008000
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_TX_INTR_EN_DEVICE_OFF   15
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_RX_INTR_EN_DEVICE_MSK   0x00004000
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI2_RX_INTR_EN_DEVICE_OFF   14
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_TX_INTR_EN_DEVICE_MSK   0x00002000
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_TX_INTR_EN_DEVICE_OFF   13
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_RX_INTR_EN_DEVICE_MSK   0x00001000
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_SFI1_RX_INTR_EN_DEVICE_OFF   12
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_MSK 0x00000fff
#define TL_IC_REG_LINE_SERDES_INTR_EN_3_BIT_LINE_SERDES_INTR_EN_PCIE_OFF 0

/*-------------------------------------------.
 | Register 0x00000184 SYS_SERDES_INTR_EN_3A |
 +-------------------------------------------+
 | bit  21:20 R/W  LR_CSU_INTR_EN_PCIE       |
 | bit  19:0  R/W  LR_SERDES_INTR_EN_PCIE    |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3A_UNUSED_MASK                0xffc00000
#define TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_MSK    0x00300000
#define TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_CSU_INTR_EN_PCIE_OFF    20
#define TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_MSK 0x000fffff
#define TL_IC_REG_SYS_SERDES_INTR_EN_3A_BIT_LR_SERDES_INTR_EN_PCIE_OFF 0

/*-------------------------------------------.
 | Register 0x00000188 SYS_SERDES_INTR_EN_3B |
 +-------------------------------------------+
 | bit  24    R/W  SR_CSU_INTR_EN_DEVICE     |
 | bit  23:16 R/W  SR_SERDES_INTR_EN_PCIE    |
 | bit  15:0  R/W  SYS_SERDES_INTR_EN_PCIE   |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_3B_UNUSED_MASK                 0xfe000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_CSU_INTR_EN_DEVICE_MSK   0x01000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_CSU_INTR_EN_DEVICE_OFF   24
#define TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_MSK  0x00ff0000
#define TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SR_SERDES_INTR_EN_PCIE_OFF  16
#define TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_MSK 0x0000ffff
#define TL_IC_REG_SYS_SERDES_INTR_EN_3B_BIT_SYS_SERDES_INTR_EN_PCIE_OFF 0

/*-------------------------------------------.
 | Register 0x0000018c OH_SERDES_INTR_EN_3   |
 +-------------------------------------------+
 | bit  16    R/W  MGMT_FEGE_INTR_EN_DEVICE  |
 | bit  15    R/W  OH_CSU_INTR_EN_DEVICE     |
 | bit  14:12 R/W  OH_TX_SERDES_INTR_EN_PCIE |
 | bit  10:8  R/W  OH_RX_SERDES_INTR_EN_PCIE |
 | bit  4     R/W  PCIE_CSU_INTR_EN_DEVICE   |
 | bit  3:0   R/W  PCIE_SERDES_INTR_EN_PCIE  |
 +------------------------------------------*/
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_3_UNUSED_MASK                   0xfffe08e0
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_MGMT_FEGE_INTR_EN_DEVICE_MSK  0x00010000
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_MGMT_FEGE_INTR_EN_DEVICE_OFF  16
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_CSU_INTR_EN_DEVICE_MSK     0x00008000
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_CSU_INTR_EN_DEVICE_OFF     15
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_MSK 0x00007000
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_TX_SERDES_INTR_EN_PCIE_OFF 12
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_MSK 0x00000700
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_OH_RX_SERDES_INTR_EN_PCIE_OFF 8
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_CSU_INTR_EN_DEVICE_MSK   0x00000010
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_CSU_INTR_EN_DEVICE_OFF   4
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_MSK  0x0000000f
#define TL_IC_REG_OH_SERDES_INTR_EN_3_BIT_PCIE_SERDES_INTR_EN_PCIE_OFF  0

/*---------------------------------------.
 | Register 0x00000190 DCSU_INTR_EN_3    |
 +---------------------------------------+
 |                                       |
 | bit  8 R/W  FIC2_DCSU_INTR_EN_DEVICE  |
 |                                       |
 |                                       |
 | bit  7 R/W  FIC1_DCSU_INTR_EN_DEVICE  |
 |                                       |
 | bit  6 R/W  PTP_DCSU_INTR_EN_DEVICE   |
 | bit  5 R/W  ENET2_DCSU_INTR_EN_DEVICE |
 | bit  4 R/W  ENET1_DCSU_INTR_EN_DEVICE |
 | bit  3 R/W  SYS2_DCSU_INTR_EN_DEVICE  |
 | bit  2 R/W  SYS_DCSU_INTR_EN_DEVICE   |
 | bit  1 R/W  OTN2_DCSU_INTR_EN_DEVICE  |
 | bit  0 R/W  OTN1_DCSU_INTR_EN_DEVICE  |
 +--------------------------------------*/
#define PMC_TL_IC_REG_DCSU_INTR_EN_3_UNUSED_MASK                   0xfffffe00
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC2_DCSU_INTR_EN_DEVICE_MSK  0x00000100
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC2_DCSU_INTR_EN_DEVICE_OFF  8
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC1_DCSU_INTR_EN_DEVICE_MSK  0x00000080
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_FIC1_DCSU_INTR_EN_DEVICE_OFF  7
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_PTP_DCSU_INTR_EN_DEVICE_MSK   0x00000040
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_PTP_DCSU_INTR_EN_DEVICE_OFF   6
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET2_DCSU_INTR_EN_DEVICE_MSK 0x00000020
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET2_DCSU_INTR_EN_DEVICE_OFF 5
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET1_DCSU_INTR_EN_DEVICE_MSK 0x00000010
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_ENET1_DCSU_INTR_EN_DEVICE_OFF 4
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS2_DCSU_INTR_EN_DEVICE_MSK  0x00000008
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS2_DCSU_INTR_EN_DEVICE_OFF  3
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS_DCSU_INTR_EN_DEVICE_MSK   0x00000004
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_SYS_DCSU_INTR_EN_DEVICE_OFF   2
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN2_DCSU_INTR_EN_DEVICE_MSK  0x00000002
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN2_DCSU_INTR_EN_DEVICE_OFF  1
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN1_DCSU_INTR_EN_DEVICE_MSK  0x00000001
#define TL_IC_REG_DCSU_INTR_EN_3_BIT_OTN1_DCSU_INTR_EN_DEVICE_OFF  0

/*-------------------------------------------.
 | Register 0x000001c0 LINE_SERDES_INTR_EN_4 |
 +-------------------------------------------+
 | bit  11:0 R/W  LINE_SERDES_INTR_EN_MIPS   |
 +------------------------------------------*/
#define PMC_TL_IC_REG_LINE_SERDES_INTR_EN_4_UNUSED_MASK                  0xffff0000
#define TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_MSK 0x00000fff
#define TL_IC_REG_LINE_SERDES_INTR_EN_4_BIT_LINE_SERDES_INTR_EN_MIPS_OFF 0

/*-------------------------------------------.
 | Register 0x000001c4 SYS_SERDES_INTR_EN_4A |
 +-------------------------------------------+
 | bit  21:20 R/W  LR_CSU_INTR_EN_MIPS       |
 | bit  19:0  R/W  LR_SERDES_INTR_EN_MIPS    |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4A_UNUSED_MASK                0xffc00000
#define TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_MSK    0x00300000
#define TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_CSU_INTR_EN_MIPS_OFF    20
#define TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_MSK 0x000fffff
#define TL_IC_REG_SYS_SERDES_INTR_EN_4A_BIT_LR_SERDES_INTR_EN_MIPS_OFF 0

/*-------------------------------------------.
 | Register 0x000001c8 SYS_SERDES_INTR_EN_4B |
 +-------------------------------------------+
 | bit  23:16 R/W  SR_SERDES_INTR_EN_MIPS    |
 | bit  15:0  R/W  SYS_SERDES_INTR_EN_MIPS   |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SYS_SERDES_INTR_EN_4B_UNUSED_MASK                 0xfe000000
#define TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_MSK  0x00ff0000
#define TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SR_SERDES_INTR_EN_MIPS_OFF  16
#define TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_MSK 0x0000ffff
#define TL_IC_REG_SYS_SERDES_INTR_EN_4B_BIT_SYS_SERDES_INTR_EN_MIPS_OFF 0

/*-------------------------------------------.
 | Register 0x000001cc OH_SERDES_INTR_EN_4   |
 +-------------------------------------------+
 | bit  14:12 R/W  OH_TX_SERDES_INTR_EN_MIPS |
 | bit  10:8  R/W  OH_RX_SERDES_INTR_EN_MIPS |
 | bit  3:0   R/W  PCIE_SERDES_INTR_EN_MIPS  |
 +------------------------------------------*/
#define PMC_TL_IC_REG_OH_SERDES_INTR_EN_4_UNUSED_MASK                   0xfffe08e0
#define TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_MSK 0x00007000
#define TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_TX_SERDES_INTR_EN_MIPS_OFF 12
#define TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_MSK 0x00000700
#define TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_OH_RX_SERDES_INTR_EN_MIPS_OFF 8
#define TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_MSK  0x0000000f
#define TL_IC_REG_OH_SERDES_INTR_EN_4_BIT_PCIE_SERDES_INTR_EN_MIPS_OFF  0

/*-------------------------------------------.
 | Register 0x00000240 SERDES_CSU_RESET_SYNC |
 +-------------------------------------------+
 | bit  31:16 R/W  SYS_RST_SYNC              |
 | bit  11:0  R/W  LINE_RST_SYNC             |
 +------------------------------------------*/
#define PMC_TL_IC_REG_SERDES_CSU_RESET_SYNC_UNUSED_MASK       0x0000f000
#define TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_MSK  0xffff0000
#define TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_SYS_RST_SYNC_OFF  16
#define TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_MSK 0x00000fff
#define TL_IC_REG_SERDES_CSU_RESET_SYNC_BIT_LINE_RST_SYNC_OFF 0

/*-------------------------------.
 | Register 0x0000024c APRAM_CFG |
 +-------------------------------+
 | bit  2 R/W  APRAM_RAM_LOW_PWR |
 | bit  1 R/W  APRAM_LOW_PWR     |
 | bit  0 R/W  APRAM_RESET       |
 +------------------------------*/
#define PMC_TL_IC_REG_APRAM_CFG_UNUSED_MASK           0xffffffe0
#define TL_IC_REG_APRAM_CFG_BIT_APRAM_RAM_LOW_PWR_MSK 0x00000004
#define TL_IC_REG_APRAM_CFG_BIT_APRAM_RAM_LOW_PWR_OFF 2
#define TL_IC_REG_APRAM_CFG_BIT_APRAM_LOW_PWR_MSK     0x00000002
#define TL_IC_REG_APRAM_CFG_BIT_APRAM_LOW_PWR_OFF     1
#define TL_IC_REG_APRAM_CFG_BIT_APRAM_RESET_MSK       0x00000001
#define TL_IC_REG_APRAM_CFG_BIT_APRAM_RESET_OFF       0

/* index definitions for PMC_TL_IC_REG_GPCFG */
#define PMC_TL_IC_REG_GPCFG_INDEX_N_MIN    0
#define PMC_TL_IC_REG_GPCFG_INDEX_N_MAX    11
#define PMC_TL_IC_REG_GPCFG_INDEX_N_SIZE   12
#define PMC_TL_IC_REG_GPCFG_INDEX_N_OFFSET 4

/*---------------------------------------.
 | Register (0x00000250 + (N) * 4) GPCFG |
 +---------------------------------------+
 | bit  31:0 R  GP_CONFIG                |
 +--------------------------------------*/
#define PMC_TL_IC_REG_GPCFG_UNUSED_MASK    0x00000000
#define TL_IC_REG_GPCFG_BIT_GP_CONFIG_MSK  0xffffffff
#define TL_IC_REG_GPCFG_BIT_GP_CONFIG_OFF  0

/*--------------------------------.
 | Register 0x00000284 AVS_STATUS |
 +--------------------------------+
 | bit  0 R  AVS_ENB              |
 +-------------------------------*/
#define PMC_TL_IC_REG_AVS_STATUS_UNUSED_MASK 0xfffffffc
#define TL_IC_REG_AVS_STATUS_BIT_AVS_ENB_MSK 0x00000001
#define TL_IC_REG_AVS_STATUS_BIT_AVS_ENB_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TL_IC_REGS_H */
