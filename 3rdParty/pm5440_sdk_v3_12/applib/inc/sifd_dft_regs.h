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
 *     sifd_dft block
 *****************************************************************************/
#ifndef _SIFD_DFT_REGS_H
#define _SIFD_DFT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SIFD_DFT_REG_SOFT_RESET                       0x00000000
#define PMC_SIFD_DFT_REG_LOW_POWER_CFG                    0x00000004
#define PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG                0x0000000c
#define PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT          0x00000018
#define PMC_SIFD_DFT_REG_SIFD_INT_EN( N )                 (0x0000001c + (N) * 0x4)
#define PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT           0x0000002c
#define PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL                 0x00000030
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL     0x00000034
#define PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS( N )           (0x00000040 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS( N )           (0x000000a0 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_DSIS_CFGS( N )                   (0x00000100 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_LINE_OTN_CFGS( N )               (0x00000130 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_FIC_WORK_CFGS( N )               (0x00000150 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS( N )            (0x000001bc + (N) * 0x4)
#define PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS( N ) (0x00000228 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS( N )       (0x000002c8 + (N) * 0x4)
#define PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS( N ) (0x00000308 + (N) * 0x4)

/*---------------------------------.
 | Register 0x00000000 SOFT_RESET  |
 +---------------------------------+
 |                                 |
 | bit  4   R/W  FIC_SOFT_RST      |
 |                                 |
 | bit  3   R/W  DSIS_SOFT_RST     |
 | bit  2:1 R/W  COPI_SOFT_RST     |
 | bit  0   R/W  SIFD_DFT_SOFT_RST |
 +--------------------------------*/
#define PMC_SIFD_DFT_REG_SOFT_RESET_UNUSED_MASK           0xffffffe0
#define SIFD_DFT_REG_SOFT_RESET_BIT_FIC_SOFT_RST_MSK      0x00000010
#define SIFD_DFT_REG_SOFT_RESET_BIT_FIC_SOFT_RST_OFF      4
#define SIFD_DFT_REG_SOFT_RESET_BIT_DSIS_SOFT_RST_MSK     0x00000008
#define SIFD_DFT_REG_SOFT_RESET_BIT_DSIS_SOFT_RST_OFF     3
#define SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_MSK     0x00000006
#define SIFD_DFT_REG_SOFT_RESET_BIT_COPI_SOFT_RST_OFF     1
#define SIFD_DFT_REG_SOFT_RESET_BIT_SIFD_DFT_SOFT_RST_MSK 0x00000001
#define SIFD_DFT_REG_SOFT_RESET_BIT_SIFD_DFT_SOFT_RST_OFF 0

/*-----------------------------------.
 | Register 0x00000004 LOW_POWER_CFG |
 +-----------------------------------+
 |                                   |
 | bit  4   R/W  FIC_LOWPWR_CFG      |
 |                                   |
 | bit  3   R/W  DSIS_LOWPWR_CFG     |
 | bit  2:1 R/W  COPI_LOWPWR_CFG     |
 | bit  0   R/W  SIFD_DFT_LOWPWR_CFG |
 +----------------------------------*/
#define PMC_SIFD_DFT_REG_LOW_POWER_CFG_UNUSED_MASK             0xffffffe0
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_FIC_LOWPWR_CFG_MSK      0x00000010
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_FIC_LOWPWR_CFG_OFF      4
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_DSIS_LOWPWR_CFG_MSK     0x00000008
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_DSIS_LOWPWR_CFG_OFF     3
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_MSK     0x00000006
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_COPI_LOWPWR_CFG_OFF     1
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_SIFD_DFT_LOWPWR_CFG_MSK 0x00000001
#define SIFD_DFT_REG_LOW_POWER_CFG_BIT_SIFD_DFT_LOWPWR_CFG_OFF 0

/*---------------------------------------.
 | Register 0x0000000c RAM_LOW_POWER_CFG |
 +---------------------------------------+
 |                                       |
 | bit  2   R/W  FIC_RAM_LOWPWR_CFG      |
 |                                       |
 | bit  1:0 R/W  COPI_RAM_LOWPWR_CFG     |
 +--------------------------------------*/
#define PMC_SIFD_DFT_REG_RAM_LOW_POWER_CFG_UNUSED_MASK             0xfffffff8
#define SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_FIC_RAM_LOWPWR_CFG_MSK  0x00000004
#define SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_FIC_RAM_LOWPWR_CFG_OFF  2
#define SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_MSK 0x00000003
#define SIFD_DFT_REG_RAM_LOW_POWER_CFG_BIT_COPI_RAM_LOWPWR_CFG_OFF 0

/*---------------------------------------------.
 | Register 0x00000018 SIFD_SUBSYSTEM_INT_STAT |
 +---------------------------------------------+
 |                                             |
 | bit  14 R  MPMA_IRQ                         |
 |                                             |
 |                                             |
 | bit  13 R  MPMO_IRQ                         |
 |                                             |
 |                                             |
 | bit  12 R  FIC_WRAP_IRQ                     |
 |                                             |
 |                                             |
 | bit  11 R  FIC_IP_IRQ                       |
 |                                             |
 | bit  10 R  DSIS_IRQ                         |
 | bit  9  R  IL2_RX_IRQ_2ND                   |
 | bit  8  R  IL2_RX_IRQ                       |
 | bit  7  R  IL2_TX_IRQ_2ND                   |
 | bit  6  R  IL2_TX_IRQ                       |
 | bit  5  R  COPI2_IRQ                        |
 | bit  4  R  IL1_RX_IRQ_2ND                   |
 | bit  3  R  IL1_RX_IRQ                       |
 | bit  2  R  IL1_TX_IRQ_2ND                   |
 | bit  1  R  IL1_TX_IRQ                       |
 | bit  0  R  COPI1_IRQ                        |
 +--------------------------------------------*/
#define PMC_SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_UNUSED_MASK        0xffff8000
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMA_IRQ_MSK       0x00004000
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMA_IRQ_OFF       14
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMO_IRQ_MSK       0x00002000
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_MPMO_IRQ_OFF       13
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_WRAP_IRQ_MSK   0x00001000
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_WRAP_IRQ_OFF   12
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_IP_IRQ_MSK     0x00000800
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_FIC_IP_IRQ_OFF     11
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_DSIS_IRQ_MSK       0x00000400
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_DSIS_IRQ_OFF       10
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_2ND_MSK 0x00000200
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_2ND_OFF 9
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_MSK     0x00000100
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_RX_IRQ_OFF     8
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_2ND_MSK 0x00000080
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_2ND_OFF 7
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_MSK     0x00000040
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL2_TX_IRQ_OFF     6
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI2_IRQ_MSK      0x00000020
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI2_IRQ_OFF      5
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_2ND_MSK 0x00000010
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_2ND_OFF 4
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_MSK     0x00000008
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_RX_IRQ_OFF     3
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_2ND_MSK 0x00000004
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_2ND_OFF 2
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_MSK     0x00000002
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_IL1_TX_IRQ_OFF     1
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI1_IRQ_MSK      0x00000001
#define SIFD_DFT_REG_SIFD_SUBSYSTEM_INT_STAT_BIT_COPI1_IRQ_OFF      0

/* index definitions for PMC_SIFD_DFT_REG_SIFD_INT_EN */
#define PMC_SIFD_DFT_REG_SIFD_INT_EN_INDEX_N_MIN           0
#define PMC_SIFD_DFT_REG_SIFD_INT_EN_INDEX_N_MAX           3
#define PMC_SIFD_DFT_REG_SIFD_INT_EN_INDEX_N_SIZE          4
#define PMC_SIFD_DFT_REG_SIFD_INT_EN_INDEX_N_OFFSET        0x4

/*-----------------------------------------------.
 | Register (0x0000001c + (N) * 0x4) SIFD_INT_EN |
 +-----------------------------------------------+
 |                                               |
 | bit  14 R/W  MPMA_IRQ_EN                      |
 |                                               |
 |                                               |
 | bit  13 R/W  MPMO_IRQ_EN                      |
 |                                               |
 |                                               |
 | bit  12 R/W  FIC_WRAP_IRQ_EN                  |
 |                                               |
 |                                               |
 | bit  11 R/W  FIC_IP_IRQ_EN                    |
 |                                               |
 | bit  10 R/W  DSIS_IRQ_EN                      |
 | bit  9  R/W  IL2_RX_IRQ_2ND_EN                |
 | bit  8  R/W  IL2_RX_IRQ_EN                    |
 | bit  7  R/W  IL2_TX_IRQ_2ND_EN                |
 | bit  6  R/W  IL2_TX_IRQ_EN                    |
 | bit  5  R/W  COPI2_IRQ_EN                     |
 | bit  4  R/W  IL1_RX_IRQ_2ND_EN                |
 | bit  3  R/W  IL1_RX_IRQ_EN                    |
 | bit  2  R/W  IL1_TX_IRQ_2ND_EN                |
 | bit  1  R/W  IL1_TX_IRQ_EN                    |
 | bit  0  R/W  COPI1_IRQ_EN                     |
 +----------------------------------------------*/
#define PMC_SIFD_DFT_REG_SIFD_INT_EN_UNUSED_MASK           0xffff8000
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMA_IRQ_EN_MSK       0x00004000
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMA_IRQ_EN_OFF       14
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMO_IRQ_EN_MSK       0x00002000
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_MPMO_IRQ_EN_OFF       13
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_WRAP_IRQ_EN_MSK   0x00001000
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_WRAP_IRQ_EN_OFF   12
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_IP_IRQ_EN_MSK     0x00000800
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_FIC_IP_IRQ_EN_OFF     11
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_DSIS_IRQ_EN_MSK       0x00000400
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_DSIS_IRQ_EN_OFF       10
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_2ND_EN_MSK 0x00000200
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_2ND_EN_OFF 9
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_EN_MSK     0x00000100
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_RX_IRQ_EN_OFF     8
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_2ND_EN_MSK 0x00000080
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_2ND_EN_OFF 7
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_EN_MSK     0x00000040
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL2_TX_IRQ_EN_OFF     6
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI2_IRQ_EN_MSK      0x00000020
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI2_IRQ_EN_OFF      5
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_2ND_EN_MSK 0x00000010
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_2ND_EN_OFF 4
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_EN_MSK     0x00000008
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_RX_IRQ_EN_OFF     3
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_2ND_EN_MSK 0x00000004
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_2ND_EN_OFF 2
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_EN_MSK     0x00000002
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_IL1_TX_IRQ_EN_OFF     1
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI1_IRQ_EN_MSK      0x00000001
#define SIFD_DFT_REG_SIFD_INT_EN_BIT_COPI1_IRQ_EN_OFF      0

/*--------------------------------------------.
 | Register 0x0000002c FIC_SERDES_LANE_SELECT |
 +--------------------------------------------+
 |                                            |
 | bit  7 R/W  FIC_P_TX_SERDES_SEL_19         |
 |                                            |
 |                                            |
 | bit  6 R/W  FIC_P_RX_SERDES_SEL_19         |
 |                                            |
 |                                            |
 | bit  5 R/W  FIC_P_TX_SERDES_SEL_18         |
 |                                            |
 |                                            |
 | bit  4 R/W  FIC_P_RX_SERDES_SEL_18         |
 |                                            |
 |                                            |
 | bit  3 R/W  FIC_W_TX_SERDES_SEL_19         |
 |                                            |
 |                                            |
 | bit  2 R/W  FIC_W_RX_SERDES_SEL_19         |
 |                                            |
 |                                            |
 | bit  1 R/W  FIC_W_TX_SERDES_SEL_18         |
 |                                            |
 |                                            |
 | bit  0 R/W  FIC_W_RX_SERDES_SEL_18         |
 |                                            |
 +-------------------------------------------*/
#define PMC_SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_UNUSED_MASK                0xffffff00
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_19_MSK 0x00000080
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_19_OFF 7
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_19_MSK 0x00000040
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_19_OFF 6
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_18_MSK 0x00000020
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_TX_SERDES_SEL_18_OFF 5
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_18_MSK 0x00000010
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_P_RX_SERDES_SEL_18_OFF 4
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_19_MSK 0x00000008
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_19_OFF 3
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_19_MSK 0x00000004
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_19_OFF 2
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_18_MSK 0x00000002
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_TX_SERDES_SEL_18_OFF 1
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_18_MSK 0x00000001
#define SIFD_DFT_REG_FIC_SERDES_LANE_SELECT_BIT_FIC_W_RX_SERDES_SEL_18_OFF 0

/*--------------------------------------.
 | Register 0x00000030 COPI_FIC_DPI_SEL |
 +--------------------------------------+
 |                                      |
 | bit  0 R/W  COPI_FIC_DPI_SEL         |
 |                                      |
 +-------------------------------------*/
#define PMC_SIFD_DFT_REG_COPI_FIC_DPI_SEL_UNUSED_MASK          0xfffffffe
#define SIFD_DFT_REG_COPI_FIC_DPI_SEL_BIT_COPI_FIC_DPI_SEL_MSK 0x00000001
#define SIFD_DFT_REG_COPI_FIC_DPI_SEL_BIT_COPI_FIC_DPI_SEL_OFF 0

/*--------------------------------------------------.
 | Register 0x00000034 SYSTEM_N1_SERDES_ACB_MUX_SEL |
 +--------------------------------------------------+
 | bit  31:24 R/W  N1_SERD_ACB_SEL                  |
 +-------------------------------------------------*/
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_UNUSED_MASK         0x00ffffff
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_MSK 0xff000000
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_ACB_MUX_SEL_BIT_N1_SERD_ACB_SEL_OFF 24

/* index definitions for PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS */
#define PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS_INDEX_N_MIN           0
#define PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS_INDEX_N_MAX           23
#define PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS_INDEX_N_SIZE          24
#define PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------.
 | Register (0x00000040 + (N) * 0x4) INTERLAKEN_1_CFGS |
 +-----------------------------------------------------+
 | bit  5 R/W  IL1_TX_SERDES_SEL                       |
 | bit  4 R/W  IL1_RX_SERDES_SEL                       |
 | bit  3 R/W  IL1_DLOS_INV                            |
 | bit  2 R/W  IL1_DLOS_EN                             |
 | bit  1 R/W  IL1_TX_LANE_DIS                         |
 | bit  0 R/W  IL1_RX_LANE_DIS                         |
 +----------------------------------------------------*/
#define PMC_SIFD_DFT_REG_INTERLAKEN_1_CFGS_UNUSED_MASK           0xffffffc0
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_SERDES_SEL_MSK 0x00000020
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_SERDES_SEL_OFF 5
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_SERDES_SEL_MSK 0x00000010
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_SERDES_SEL_OFF 4
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_INV_MSK      0x00000008
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_INV_OFF      3
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_EN_MSK       0x00000004
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_DLOS_EN_OFF       2
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_LANE_DIS_MSK   0x00000002
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_TX_LANE_DIS_OFF   1
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_LANE_DIS_MSK   0x00000001
#define SIFD_DFT_REG_INTERLAKEN_1_CFGS_BIT_IL1_RX_LANE_DIS_OFF   0

/* index definitions for PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS */
#define PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS_INDEX_N_MIN           0
#define PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS_INDEX_N_MAX           23
#define PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS_INDEX_N_SIZE          24
#define PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------.
 | Register (0x000000a0 + (N) * 0x4) INTERLAKEN_2_CFGS |
 +-----------------------------------------------------+
 | bit  5 R/W  IL2_TX_SERDES_SEL                       |
 | bit  4 R/W  IL2_RX_SERDES_SEL                       |
 | bit  3 R/W  IL2_DLOS_INV                            |
 | bit  2 R/W  IL2_DLOS_EN                             |
 | bit  1 R/W  IL2_TX_LANE_DIS                         |
 | bit  0 R/W  IL2_RX_LANE_DIS                         |
 +----------------------------------------------------*/
#define PMC_SIFD_DFT_REG_INTERLAKEN_2_CFGS_UNUSED_MASK           0xffffffc0
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_SERDES_SEL_MSK 0x00000020
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_SERDES_SEL_OFF 5
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_SERDES_SEL_MSK 0x00000010
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_SERDES_SEL_OFF 4
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_INV_MSK      0x00000008
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_INV_OFF      3
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_EN_MSK       0x00000004
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_DLOS_EN_OFF       2
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_LANE_DIS_MSK   0x00000002
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_TX_LANE_DIS_OFF   1
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_LANE_DIS_MSK   0x00000001
#define SIFD_DFT_REG_INTERLAKEN_2_CFGS_BIT_IL2_RX_LANE_DIS_OFF   0

/* index definitions for PMC_SIFD_DFT_REG_DSIS_CFGS */
#define PMC_SIFD_DFT_REG_DSIS_CFGS_INDEX_N_MIN          0
#define PMC_SIFD_DFT_REG_DSIS_CFGS_INDEX_N_MAX          11
#define PMC_SIFD_DFT_REG_DSIS_CFGS_INDEX_N_SIZE         12
#define PMC_SIFD_DFT_REG_DSIS_CFGS_INDEX_N_OFFSET       0x4

/*---------------------------------------------.
 | Register (0x00000100 + (N) * 0x4) DSIS_CFGS |
 +---------------------------------------------+
 | bit  6 R/W  DSIS_LOS_POL_INV                |
 | bit  5 R/W  DSIS_DLOS_INV                   |
 | bit  4 R/W  DSIS_DLOS_EN                    |
 | bit  3 R/W  DSIS_PIN_LOS_INV                |
 | bit  2 R/W  DSIS_PIN_LOS_EN                 |
 | bit  1 R/W  DSIS_TX_LANE_DIS                |
 | bit  0 R/W  DSIS_RX_LANE_DIS                |
 +--------------------------------------------*/
#define PMC_SIFD_DFT_REG_DSIS_CFGS_UNUSED_MASK          0xffffff80
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_LOS_POL_INV_MSK 0x00000040
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_LOS_POL_INV_OFF 6
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_INV_MSK    0x00000020
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_INV_OFF    5
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_EN_MSK     0x00000010
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_DLOS_EN_OFF     4
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_INV_MSK 0x00000008
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_INV_OFF 3
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_EN_MSK  0x00000004
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_PIN_LOS_EN_OFF  2
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_TX_LANE_DIS_MSK 0x00000002
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_TX_LANE_DIS_OFF 1
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_RX_LANE_DIS_MSK 0x00000001
#define SIFD_DFT_REG_DSIS_CFGS_BIT_DSIS_RX_LANE_DIS_OFF 0

/* index definitions for PMC_SIFD_DFT_REG_LINE_OTN_CFGS */
#define PMC_SIFD_DFT_REG_LINE_OTN_CFGS_INDEX_N_MIN          0
#define PMC_SIFD_DFT_REG_LINE_OTN_CFGS_INDEX_N_MAX          7
#define PMC_SIFD_DFT_REG_LINE_OTN_CFGS_INDEX_N_SIZE         8
#define PMC_SIFD_DFT_REG_LINE_OTN_CFGS_INDEX_N_OFFSET       0x4

/*-------------------------------------------------.
 | Register (0x00000130 + (N) * 0x4) LINE_OTN_CFGS |
 +-------------------------------------------------+
 | bit  1 R/W  LOTN_TX_LANE_DIS                    |
 | bit  0 R/W  LOTN_RX_LANE_DIS                    |
 +------------------------------------------------*/
#define PMC_SIFD_DFT_REG_LINE_OTN_CFGS_UNUSED_MASK          0xfffffffc
#define SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_TX_LANE_DIS_MSK 0x00000002
#define SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_TX_LANE_DIS_OFF 1
#define SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_RX_LANE_DIS_MSK 0x00000001
#define SIFD_DFT_REG_LINE_OTN_CFGS_BIT_LOTN_RX_LANE_DIS_OFF 0

/* index definitions for PMC_SIFD_DFT_REG_FIC_WORK_CFGS */
#define PMC_SIFD_DFT_REG_FIC_WORK_CFGS_INDEX_N_MIN           0
#define PMC_SIFD_DFT_REG_FIC_WORK_CFGS_INDEX_N_MAX           26
#define PMC_SIFD_DFT_REG_FIC_WORK_CFGS_INDEX_N_SIZE          27
#define PMC_SIFD_DFT_REG_FIC_WORK_CFGS_INDEX_N_OFFSET        0x4

/*-------------------------------------------------.
 | Register (0x00000150 + (N) * 0x4) FIC_WORK_CFGS |
 +-------------------------------------------------+
 |                                                 |
 | bit  3 R/W  FIC_W_DLOS_INV                      |
 |                                                 |
 |                                                 |
 | bit  2 R/W  FIC_W_DLOS_EN                       |
 |                                                 |
 |                                                 |
 | bit  1 R/W  FIC_W_TX_LANE_DIS                   |
 |                                                 |
 |                                                 |
 | bit  0 R/W  FIC_W_RX_LANE_DIS                   |
 |                                                 |
 +------------------------------------------------*/
#define PMC_SIFD_DFT_REG_FIC_WORK_CFGS_UNUSED_MASK           0xfffffff0
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_INV_MSK    0x00000008
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_INV_OFF    3
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_EN_MSK     0x00000004
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_DLOS_EN_OFF     2
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_TX_LANE_DIS_MSK 0x00000002
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_TX_LANE_DIS_OFF 1
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_RX_LANE_DIS_MSK 0x00000001
#define SIFD_DFT_REG_FIC_WORK_CFGS_BIT_FIC_W_RX_LANE_DIS_OFF 0

/* index definitions for PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS */
#define PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS_INDEX_N_MIN           0
#define PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS_INDEX_N_MAX           26
#define PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS_INDEX_N_SIZE          27
#define PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS_INDEX_N_OFFSET        0x4

/*----------------------------------------------------.
 | Register (0x000001bc + (N) * 0x4) FIC_PROTECT_CFGS |
 +----------------------------------------------------+
 |                                                    |
 | bit  3 R/W  FIC_P_DLOS_INV                         |
 |                                                    |
 |                                                    |
 | bit  2 R/W  FIC_P_DLOS_EN                          |
 |                                                    |
 |                                                    |
 | bit  1 R/W  FIC_P_TX_LANE_DIS                      |
 |                                                    |
 |                                                    |
 | bit  0 R/W  FIC_P_RX_LANE_DIS                      |
 |                                                    |
 +---------------------------------------------------*/
#define PMC_SIFD_DFT_REG_FIC_PROTECT_CFGS_UNUSED_MASK           0xfffffff0
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_INV_MSK    0x00000008
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_INV_OFF    3
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_EN_MSK     0x00000004
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_DLOS_EN_OFF     2
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_TX_LANE_DIS_MSK 0x00000002
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_TX_LANE_DIS_OFF 1
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_RX_LANE_DIS_MSK 0x00000001
#define SIFD_DFT_REG_FIC_PROTECT_CFGS_BIT_FIC_P_RX_LANE_DIS_OFF 0

/* index definitions for PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS */
#define PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_INDEX_N_MIN                     0
#define PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_INDEX_N_MAX                     39
#define PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_INDEX_N_SIZE                    40
#define PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_INDEX_N_OFFSET                  0x4

/*---------------------------------------------------------------.
 | Register (0x00000228 + (N) * 0x4) BACKPLANE_LR_E1_SERDES_CFGS |
 +---------------------------------------------------------------+
 | bit  25:24 R/W  E1_TX_LANE_SEL                                |
 |                                                               |
 | bit  20    R/W  E1_TX_DESKEW_CONST_DELAY_EN                   |
 |                                                               |
 | bit  19:16 R/W  E1_TX_NUM_DESKEW_STAGES                       |
 |                                                               |
 | bit  12    R/W  E1_RX_DESKEW_CONST_DELAY_EN                   |
 |                                                               |
 | bit  11:8  R/W  E1_RX_NUM_DESKEW_STAGES                       |
 +--------------------------------------------------------------*/
#define PMC_SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_UNUSED_MASK                     0xfce0e0ff
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_MSK              0x03000000
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_LANE_SEL_OFF              24
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_DESKEW_CONST_DELAY_EN_MSK 0x00100000
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_DESKEW_CONST_DELAY_EN_OFF 20
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_MSK     0x000f0000
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_TX_NUM_DESKEW_STAGES_OFF     16
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_DESKEW_CONST_DELAY_EN_MSK 0x00001000
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_DESKEW_CONST_DELAY_EN_OFF 12
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_MSK     0x00000f00
#define SIFD_DFT_REG_BACKPLANE_LR_E1_SERDES_CFGS_BIT_E1_RX_NUM_DESKEW_STAGES_OFF     8

/* index definitions for PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS */
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_INDEX_N_MIN                     0
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_INDEX_N_MAX                     15
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_INDEX_N_SIZE                    16
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_INDEX_N_OFFSET                  0x4

/*---------------------------------------------------------.
 | Register (0x000002c8 + (N) * 0x4) SYSTEM_N1_SERDES_CFGS |
 +---------------------------------------------------------+
 | bit  25:24 R/W  N1_TX_LANE_SEL                          |
 |                                                         |
 | bit  18    R/W  N1_TX_DESKEW_CONST_DELAY_EN             |
 |                                                         |
 | bit  17:16 R/W  N1_TX_NUM_DESKEW_STAGES                 |
 | bit  9:8   R/W  N1_RX_NUM_DESKEW_STAGES                 |
 +--------------------------------------------------------*/
#define PMC_SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_UNUSED_MASK                     0xfcf8fcff
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_MSK              0x03000000
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_LANE_SEL_OFF              24
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_DESKEW_CONST_DELAY_EN_MSK 0x00040000
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_DESKEW_CONST_DELAY_EN_OFF 18
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_MSK     0x00030000
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_TX_NUM_DESKEW_STAGES_OFF     16
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_MSK     0x00000300
#define SIFD_DFT_REG_SYSTEM_N1_SERDES_CFGS_BIT_N1_RX_NUM_DESKEW_STAGES_OFF     8

/* index definitions for PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS */
#define PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_INDEX_N_MIN                     0
#define PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_INDEX_N_MAX                     7
#define PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_INDEX_N_SIZE                    8
#define PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_INDEX_N_OFFSET                  0x4

/*---------------------------------------------------------------.
 | Register (0x00000308 + (N) * 0x4) BACKPLANE_SR_N2_SERDES_CFGS |
 +---------------------------------------------------------------+
 | bit  25:24 R/W  N2_TX_LANE_SEL                                |
 |                                                               |
 | bit  19    R/W  N2_TX_DESKEW_CONST_DELAY_EN                   |
 |                                                               |
 | bit  18:16 R/W  N2_TX_NUM_DESKEW_STAGES                       |
 | bit  11:8  R/W  N2_RX_NUM_DESKEW_STAGES                       |
 +--------------------------------------------------------------*/
#define PMC_SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_UNUSED_MASK                     0xfcf0f0ff
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_MSK              0x03000000
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_LANE_SEL_OFF              24
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_DESKEW_CONST_DELAY_EN_MSK 0x00080000
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_DESKEW_CONST_DELAY_EN_OFF 19
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_MSK     0x00070000
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_TX_NUM_DESKEW_STAGES_OFF     16
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_MSK     0x00000f00
#define SIFD_DFT_REG_BACKPLANE_SR_N2_SERDES_CFGS_BIT_N2_RX_NUM_DESKEW_STAGES_OFF     8

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SIFD_DFT_REGS_H */
