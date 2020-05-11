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
 *     coreotn_ctl block
 *****************************************************************************/
#ifndef _COREOTN_CTL_REGS_H
#define _COREOTN_CTL_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_COREOTN_CTL_REG_SW_RESET_REG                 0x002ac000
#define PMC_COREOTN_CTL_REG_CLK_GATING_REG               0x002ac004
#define PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG            0x002ac008
#define PMC_COREOTN_CTL_REG_INT_STAT_REG_1               0x002ac020
#define PMC_COREOTN_CTL_REG_INT_STAT_REG_2               0x002ac024
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1           0x002ac028
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1           0x002ac02c
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2           0x002ac030
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2           0x002ac034
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3           0x002ac038
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3           0x002ac03c
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4           0x002ac040
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4           0x002ac044
#define PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG           0x002ac048
#define PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG             0x002ac04c
#define PMC_COREOTN_CTL_REG_OHP_CONFIG_REG               0x002ac050
#define PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG 0x002ac054
#define PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG       0x002ac058
#define PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG       0x002ac05c
#define PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG     0x002ac060
#define PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG     0x002ac064

/*--------------------------------------.
 | Register 0x002ac000 SW_RESET_REG     |
 +--------------------------------------+
 |                                      |
 | bit  7 R/W  ECC_FRM_MICROPROC_RESETN |
 |                                      |
 |                                      |
 | bit  6 R/W  ECC_FRM_RSTB             |
 |                                      |
 |                                      |
 | bit  5 R/W  ECC_FRM_S_RSTB           |
 |                                      |
 | bit  4 R/W  MPMO_RST                 |
 | bit  3 R/W  MPMA_RST                 |
 | bit  2 R/W  OHP3_RST                 |
 | bit  1 R/W  OHP2_RST                 |
 | bit  0 R/W  OHP1_RST                 |
 +-------------------------------------*/
#define PMC_COREOTN_CTL_REG_SW_RESET_REG_UNUSED_MASK                  0xffffff00
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_MICROPROC_RESETN_MSK 0x00000080
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_MICROPROC_RESETN_OFF 7
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_RSTB_MSK             0x00000040
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_RSTB_OFF             6
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_S_RSTB_MSK           0x00000020
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_ECC_FRM_S_RSTB_OFF           5
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMO_RST_MSK                 0x00000010
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMO_RST_OFF                 4
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMA_RST_MSK                 0x00000008
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_MPMA_RST_OFF                 3
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP3_RST_MSK                 0x00000004
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP3_RST_OFF                 2
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP2_RST_MSK                 0x00000002
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP2_RST_OFF                 1
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP1_RST_MSK                 0x00000001
#define COREOTN_CTL_REG_SW_RESET_REG_BIT_OHP1_RST_OFF                 0

/*------------------------------------.
 | Register 0x002ac004 CLK_GATING_REG |
 +------------------------------------+
 |                                    |
 | bit  5 R/W  ECC_FRM_LOWPWR         |
 |                                    |
 | bit  4 R/W  MPMO_LOWPWR            |
 | bit  3 R/W  MPMA_LOWPWR            |
 | bit  2 R/W  OHP3_LOWPWR            |
 | bit  1 R/W  OHP2_LOWPWR            |
 | bit  0 R/W  OHP1_LOWPWR            |
 +-----------------------------------*/
#define PMC_COREOTN_CTL_REG_CLK_GATING_REG_UNUSED_MASK        0xffffffc0
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_ECC_FRM_LOWPWR_MSK 0x00000020
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_ECC_FRM_LOWPWR_OFF 5
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_MSK    0x00000010
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_OFF    4
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_MSK    0x00000008
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_OFF    3
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP3_LOWPWR_MSK    0x00000004
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP3_LOWPWR_OFF    2
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP2_LOWPWR_MSK    0x00000002
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP2_LOWPWR_OFF    1
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP1_LOWPWR_MSK    0x00000001
#define COREOTN_CTL_REG_CLK_GATING_REG_BIT_OHP1_LOWPWR_OFF    0

/*---------------------------------------.
 | Register 0x002ac008 RAM_LOW_POWER_REG |
 +---------------------------------------+
 |                                       |
 | bit  5 R/W  ECC_FRM_RAM_LOWPWR        |
 |                                       |
 | bit  4 R/W  MPMO_RAM_LOWPWR           |
 | bit  3 R/W  MPMA_RAM_LOWPWR           |
 | bit  2 R/W  OHP3_RAM_LOWPWR           |
 | bit  1 R/W  OHP2_RAM_LOWPWR           |
 | bit  0 R/W  OHP1_RAM_LOWPWR           |
 +--------------------------------------*/
#define PMC_COREOTN_CTL_REG_RAM_LOW_POWER_REG_UNUSED_MASK            0xffffffc0
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_ECC_FRM_RAM_LOWPWR_MSK 0x00000020
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_ECC_FRM_RAM_LOWPWR_OFF 5
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_MSK    0x00000010
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_OFF    4
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_MSK    0x00000008
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_OFF    3
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP3_RAM_LOWPWR_MSK    0x00000004
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP3_RAM_LOWPWR_OFF    2
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP2_RAM_LOWPWR_MSK    0x00000002
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP2_RAM_LOWPWR_OFF    1
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP1_RAM_LOWPWR_MSK    0x00000001
#define COREOTN_CTL_REG_RAM_LOW_POWER_REG_BIT_OHP1_RAM_LOWPWR_OFF    0

/*------------------------------------.
 | Register 0x002ac020 INT_STAT_REG_1 |
 +------------------------------------+
 | bit  31:0 R  INT_STAT              |
 +-----------------------------------*/
#define PMC_COREOTN_CTL_REG_INT_STAT_REG_1_UNUSED_MASK  0x00000000
#define COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_MSK 0xffffffff
#define COREOTN_CTL_REG_INT_STAT_REG_1_BIT_INT_STAT_OFF 0

/*------------------------------------.
 | Register 0x002ac024 INT_STAT_REG_2 |
 +------------------------------------+
 | bit  7:0 R  INT_STAT               |
 +-----------------------------------*/
#define PMC_COREOTN_CTL_REG_INT_STAT_REG_2_UNUSED_MASK  0xffffff00
#define COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_MSK 0x000000ff
#define COREOTN_CTL_REG_INT_STAT_REG_2_BIT_INT_STAT_OFF 0

/*----------------------------------------.
 | Register 0x002ac028 LOWER_INT_EN_REG_1 |
 +----------------------------------------+
 | bit  31:0 R/W  INT_EN_1                |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_1_UNUSED_MASK  0x00000000
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_MSK 0xffffffff
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_1_BIT_INT_EN_1_OFF 0

/*----------------------------------------.
 | Register 0x002ac02c UPPER_INT_EN_REG_1 |
 +----------------------------------------+
 | bit  7:0 R/W  INT_EN_1                 |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_1_UNUSED_MASK  0xffffff00
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_MSK 0x000000ff
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_1_BIT_INT_EN_1_OFF 0

/*----------------------------------------.
 | Register 0x002ac030 LOWER_INT_EN_REG_2 |
 +----------------------------------------+
 | bit  31:0 R/W  INT_EN_2                |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_2_UNUSED_MASK  0x00000000
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_MSK 0xffffffff
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_2_BIT_INT_EN_2_OFF 0

/*----------------------------------------.
 | Register 0x002ac034 UPPER_INT_EN_REG_2 |
 +----------------------------------------+
 | bit  7:0 R/W  INT_EN_2                 |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_2_UNUSED_MASK  0xffffff00
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_MSK 0x000000ff
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_2_BIT_INT_EN_2_OFF 0

/*----------------------------------------.
 | Register 0x002ac038 LOWER_INT_EN_REG_3 |
 +----------------------------------------+
 | bit  31:0 R/W  INT_EN_3                |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_3_UNUSED_MASK  0x00000000
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_MSK 0xffffffff
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_3_BIT_INT_EN_3_OFF 0

/*----------------------------------------.
 | Register 0x002ac03c UPPER_INT_EN_REG_3 |
 +----------------------------------------+
 | bit  7:0 R/W  INT_EN_3                 |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_3_UNUSED_MASK  0xffffff00
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_MSK 0x000000ff
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_3_BIT_INT_EN_3_OFF 0

/*----------------------------------------.
 | Register 0x002ac040 LOWER_INT_EN_REG_4 |
 +----------------------------------------+
 | bit  31:0 R/W  INT_EN_4                |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_LOWER_INT_EN_REG_4_UNUSED_MASK  0x00000000
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_MSK 0xffffffff
#define COREOTN_CTL_REG_LOWER_INT_EN_REG_4_BIT_INT_EN_4_OFF 0

/*----------------------------------------.
 | Register 0x002ac044 UPPER_INT_EN_REG_4 |
 +----------------------------------------+
 | bit  7:0 R/W  INT_EN_4                 |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_UPPER_INT_EN_REG_4_UNUSED_MASK  0xffffff00
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_MSK 0x000000ff
#define COREOTN_CTL_REG_UPPER_INT_EN_REG_4_BIT_INT_EN_4_OFF 0

/*----------------------------------------.
 | Register 0x002ac048 DPI_BUS_CONFIG_REG |
 +----------------------------------------+
 | bit  4 R/W  SIFD1_SEL_CTRL             |
 | bit  0 R/W  SIFD1_MUX_CTRL             |
 +---------------------------------------*/
#define PMC_COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_UNUSED_MASK        0xffffffee
#define COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_SEL_CTRL_MSK 0x00000010
#define COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_SEL_CTRL_OFF 4
#define COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_MUX_CTRL_MSK 0x00000001
#define COREOTN_CTL_REG_DPI_BUS_CONFIG_REG_BIT_SIFD1_MUX_CTRL_OFF 0

/*--------------------------------------.
 | Register 0x002ac04c CKCTL_CONFIG_REG |
 +--------------------------------------+
 | bit  0 R/W  CKCTL_CTRL               |
 +-------------------------------------*/
#define PMC_COREOTN_CTL_REG_CKCTL_CONFIG_REG_UNUSED_MASK    0xfffffffe
#define COREOTN_CTL_REG_CKCTL_CONFIG_REG_BIT_CKCTL_CTRL_MSK 0x00000001
#define COREOTN_CTL_REG_CKCTL_CONFIG_REG_BIT_CKCTL_CTRL_OFF 0

/*------------------------------------.
 | Register 0x002ac050 OHP_CONFIG_REG |
 +------------------------------------+
 | bit  31:24 R/W  OHP3_SEL_B         |
 | bit  23:16 R/W  OHP3_SEL_A         |
 | bit  15:8  R/W  OHP2_SEL_B         |
 | bit  7:0   R/W  OHP2_SEL_A         |
 +-----------------------------------*/
#define PMC_COREOTN_CTL_REG_OHP_CONFIG_REG_UNUSED_MASK    0x00000000
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_MSK 0xff000000
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_B_OFF 24
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_MSK 0x00ff0000
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP3_SEL_A_OFF 16
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_MSK 0x0000ff00
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_B_OFF 8
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_MSK 0x000000ff
#define COREOTN_CTL_REG_OHP_CONFIG_REG_BIT_OHP2_SEL_A_OFF 0

/*--------------------------------------------------.
 | Register 0x002ac054 ECC_FRM_PROCESSOR_CONFIG_REG |
 +--------------------------------------------------+
 |                                                  |
 | bit  22:16 R/W  SLV_OP_MAX_CNT                   |
 |                                                  |
 |                                                  |
 | bit  14:0  R/W  MP_BLOCK_MAX_CNT                 |
 |                                                  |
 +-------------------------------------------------*/
#define PMC_COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_UNUSED_MASK          0xff808000
#define COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_MSK   0x007f0000
#define COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_SLV_OP_MAX_CNT_OFF   16
#define COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_MSK 0x00007fff
#define COREOTN_CTL_REG_ECC_FRM_PROCESSOR_CONFIG_REG_BIT_MP_BLOCK_MAX_CNT_OFF 0

/*--------------------------------------------.
 | Register 0x002ac058 ECC_FRM_GCC_CONFIG_REG |
 +--------------------------------------------+
 |                                            |
 | bit  12 R/W  TX_GCC_BYTE_SWAP              |
 |                                            |
 |                                            |
 | bit  8  R/W  TX_GCC_BIT_SWAP               |
 |                                            |
 |                                            |
 | bit  4  R/W  RX_GCC_BYTE_SWAP              |
 |                                            |
 |                                            |
 | bit  0  R/W  RX_GCC_BIT_SWAP               |
 |                                            |
 +-------------------------------------------*/
#define PMC_COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_UNUSED_MASK          0xffffeeee
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BYTE_SWAP_MSK 0x00001000
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BYTE_SWAP_OFF 12
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BIT_SWAP_MSK  0x00000100
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_TX_GCC_BIT_SWAP_OFF  8
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BYTE_SWAP_MSK 0x00000010
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BYTE_SWAP_OFF 4
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BIT_SWAP_MSK  0x00000001
#define COREOTN_CTL_REG_ECC_FRM_GCC_CONFIG_REG_BIT_RX_GCC_BIT_SWAP_OFF  0

/*--------------------------------------------.
 | Register 0x002ac05c ECC_FRM_FIFO_RESET_REG |
 +--------------------------------------------+
 |                                            |
 | bit  4 R/W  ECC_FRM_FIFO_TX_RSTB           |
 |                                            |
 |                                            |
 | bit  0 R/W  ECC_FRM_FIFO_RX_RSTB           |
 |                                            |
 +-------------------------------------------*/
#define PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_UNUSED_MASK              0xffffffee
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_TX_RSTB_MSK 0x00000010
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_TX_RSTB_OFF 4
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_RX_RSTB_MSK 0x00000001
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RESET_REG_BIT_ECC_FRM_FIFO_RX_RSTB_OFF 0

/*----------------------------------------------.
 | Register 0x002ac060 ECC_FRM_FIFO_RX_STAT_REG |
 +----------------------------------------------+
 |                                              |
 | bit  29:24 R  ECC_FRM_FIFO_RX_EMPTY          |
 |                                              |
 |                                              |
 | bit  21:16 R  ECC_FRM_FIFO_RX_FULL           |
 |                                              |
 |                                              |
 | bit  13:8  R  ECC_FRM_FIFO_RX_UNDERFLOW      |
 |                                              |
 |                                              |
 | bit  5:0   R  ECC_FRM_FIFO_RX_OVERFLOW       |
 |                                              |
 +---------------------------------------------*/
#define PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_UNUSED_MASK                   0xc0c0c0c0
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_MSK     0x3f000000
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_EMPTY_OFF     24
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_MSK      0x003f0000
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_FULL_OFF      16
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_MSK 0x00003f00
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_UNDERFLOW_OFF 8
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_MSK  0x0000003f
#define COREOTN_CTL_REG_ECC_FRM_FIFO_RX_STAT_REG_BIT_ECC_FRM_FIFO_RX_OVERFLOW_OFF  0

/*----------------------------------------------.
 | Register 0x002ac064 ECC_FRM_FIFO_TX_STAT_REG |
 +----------------------------------------------+
 |                                              |
 | bit  29:24 R  ECC_FRM_FIFO_TX_EMPTY          |
 |                                              |
 |                                              |
 | bit  21:16 R  ECC_FRM_FIFO_TX_FULL           |
 |                                              |
 |                                              |
 | bit  13:8  R  ECC_FRM_FIFO_TX_UNDERFLOW      |
 |                                              |
 |                                              |
 | bit  5:0   R  ECC_FRM_FIFO_TX_OVERFLOW       |
 |                                              |
 +---------------------------------------------*/
#define PMC_COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_UNUSED_MASK                   0xc0c0c0c0
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_MSK     0x3f000000
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_EMPTY_OFF     24
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_MSK      0x003f0000
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_FULL_OFF      16
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_MSK 0x00003f00
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_UNDERFLOW_OFF 8
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_MSK  0x0000003f
#define COREOTN_CTL_REG_ECC_FRM_FIFO_TX_STAT_REG_BIT_ECC_FRM_FIFO_TX_OVERFLOW_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COREOTN_CTL_REGS_H */
