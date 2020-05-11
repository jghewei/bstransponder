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
 *     emac block
 *****************************************************************************/
#ifndef _EMAC_REGS_H
#define _EMAC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_EMAC120_TOP_REG_SW_RESET                                    0x00000000
#define PMC_EMAC120_TOP_REG_LINK_RESET                                  0x00000004
#define PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL                        0x00000010
#define PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL              0x00000014
#define PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL             0x00000018
#define PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL          0x00000020
#define PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL      0x00000024
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_               0x00000030
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_               0x00000034
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_               0x00000038
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_               0x0000003c
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_              0x00000040
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_              0x00000044
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_              0x00000048
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_              0x0000004c
#define PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT                       0x00000050
#define PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT               0x00000058
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_                0x00000060
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_                0x00000064
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_                0x00000068
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_                0x0000006c
#define PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN                         0x00000070
#define PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN                 0x00000078
#define PMC_EMAC120_TOP_REG_INGRESS_DSI_IF                              0x00000080
#define PMC_EMAC120_TOP_REG_EGRESS_DSI_IF                               0x00000084
#define PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING               0x00000088
#define PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK                           0x0000008c
#define PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL                  0x00000090
#define PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE               0x00000094
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK                  0x00000098
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK 0x0000009c
#define PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG                       0x000000a0

/*----------------------------------.
 | Register 0x00000000 SW_RESET     |
 +----------------------------------+
 | bit  3 R/W  ETIME_NREGS_SW_RESET |
 | bit  2 R/W  ETIME_CORE_SW_RESET  |
 | bit  1 R/W  EMAC_NREGS_SW_RESET  |
 | bit  0 R/W  EMAC_CORE_SW_RESET   |
 +---------------------------------*/
#define PMC_EMAC120_TOP_REG_SW_RESET_UNUSED_MASK              0xfffffff0
#define EMAC120_TOP_REG_SW_RESET_BIT_ETIME_NREGS_SW_RESET_MSK 0x00000008
#define EMAC120_TOP_REG_SW_RESET_BIT_ETIME_NREGS_SW_RESET_OFF 3
#define EMAC120_TOP_REG_SW_RESET_BIT_ETIME_CORE_SW_RESET_MSK  0x00000004
#define EMAC120_TOP_REG_SW_RESET_BIT_ETIME_CORE_SW_RESET_OFF  2
#define EMAC120_TOP_REG_SW_RESET_BIT_EMAC_NREGS_SW_RESET_MSK  0x00000002
#define EMAC120_TOP_REG_SW_RESET_BIT_EMAC_NREGS_SW_RESET_OFF  1
#define EMAC120_TOP_REG_SW_RESET_BIT_EMAC_CORE_SW_RESET_MSK   0x00000001
#define EMAC120_TOP_REG_SW_RESET_BIT_EMAC_CORE_SW_RESET_OFF   0

/*--------------------------------.
 | Register 0x00000004 LINK_RESET |
 +--------------------------------+
 | bit  23:12 R/W  ING_LINK_RESET |
 | bit  11:0  R/W  EGR_LINK_RESET |
 +-------------------------------*/
#define PMC_EMAC120_TOP_REG_LINK_RESET_UNUSED_MASK        0xff000000
#define EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_MSK 0x00fff000
#define EMAC120_TOP_REG_LINK_RESET_BIT_ING_LINK_RESET_OFF 12
#define EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_MSK 0x00000fff
#define EMAC120_TOP_REG_LINK_RESET_BIT_EGR_LINK_RESET_OFF 0

/*------------------------------------------.
 | Register 0x00000010 ETIME_LOW_POWER_CTRL |
 +------------------------------------------+
 | bit  0 R/W  ETIME_TS_LOWPWR              |
 +-----------------------------------------*/
#define PMC_EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_UNUSED_MASK         0xfffffffe
#define EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_BIT_ETIME_TS_LOWPWR_MSK 0x00000001
#define EMAC120_TOP_REG_ETIME_LOW_POWER_CTRL_BIT_ETIME_TS_LOWPWR_OFF 0

/*----------------------------------------------------.
 | Register 0x00000014 EMAC_PMA_IF_CLK_LOW_POWER_CTRL |
 +----------------------------------------------------+
 | bit  23:12 R/W  EMAC_TX_PMA_CLK_LOWPWR             |
 | bit  11:0  R/W  EMAC_RX_PMA_CLK_LOWPWR             |
 +---------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_UNUSED_MASK                0xff000000
#define EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_TX_PMA_CLK_LOWPWR_OFF 12
#define EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_BIT_EMAC_RX_PMA_CLK_LOWPWR_OFF 0

/*-----------------------------------------------------.
 | Register 0x00000018 EMAC_MAC_PCS_CLK_LOW_POWER_CTRL |
 +-----------------------------------------------------+
 | bit  18:16 R/W  EMAC_LINK_40_100G_CLK_LOWPWR        |
 | bit  11:0  R/W  EMAC_LINK_10G_CLK_LOWPWR            |
 +----------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_UNUSED_MASK                      0xfff8f000
#define EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_MSK 0x00070000
#define EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_40_100G_CLK_LOWPWR_OFF 16
#define EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_MSK     0x00000fff
#define EMAC120_TOP_REG_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_BIT_EMAC_LINK_10G_CLK_LOWPWR_OFF     0

/*--------------------------------------------------------.
 | Register 0x00000020 EMAC_STATISTICS_RAM_LOW_POWER_CTRL |
 +--------------------------------------------------------+
 | bit  11:0 R/W  EMAC_STAT_RAM_LOWPWR                    |
 +-------------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_UNUSED_MASK              0xfffff000
#define EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_BIT_EMAC_STAT_RAM_LOWPWR_OFF 0

/*------------------------------------------------------------.
 | Register 0x00000024 EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL |
 +------------------------------------------------------------+
 | bit  11:0 R/W  EGR_XOFF_OVERRIDE                           |
 +-----------------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_UNUSED_MASK           0xfffff000
#define EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_BIT_EGR_XOFF_OVERRIDE_OFF 0

/*---------------------------------------------------.
 | Register 0x00000030 EMAC_INGRESS_PCS_INT_VAL_1_4_ |
 +---------------------------------------------------+
 | bit  11:0 R  PHY_LOS_V                            |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_UNUSED_MASK   0xfffff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_1_4_BIT_PHY_LOS_V_OFF 0

/*---------------------------------------------------.
 | Register 0x00000034 EMAC_INGRESS_PCS_INT_VAL_2_4_ |
 +---------------------------------------------------+
 | bit  23:12 R  RX_REM_FAULT_V                      |
 | bit  11:0  R  RX_LOC_FAULT_V                      |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_UNUSED_MASK        0xff000000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_REM_FAULT_V_OFF 12
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_2_4_BIT_RX_LOC_FAULT_V_OFF 0

/*---------------------------------------------------.
 | Register 0x00000038 EMAC_INGRESS_PCS_INT_VAL_3_4_ |
 +---------------------------------------------------+
 | bit  11:0 R  HI_BER_V                             |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_UNUSED_MASK  0xfffff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_3_4_BIT_HI_BER_V_OFF 0

/*---------------------------------------------------.
 | Register 0x0000003c EMAC_INGRESS_PCS_INT_VAL_4_4_ |
 +---------------------------------------------------+
 | bit  18:16 R  ALIGN_STATUS_V                      |
 | bit  11:0  R  BLOCK_LOCK_V                        |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_UNUSED_MASK        0xfff8f000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_MSK 0x00070000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_ALIGN_STATUS_V_OFF 16
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_MSK   0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_VAL_4_4_BIT_BLOCK_LOCK_V_OFF   0

/*----------------------------------------------------.
 | Register 0x00000040 EMAC_INGRESS_PCS_INT_STAT_1_4_ |
 +----------------------------------------------------+
 | bit  11:0 R/W  PHY_LOS_I                           |
 +---------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_UNUSED_MASK   0xfffff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_1_4_BIT_PHY_LOS_I_OFF 0

/*----------------------------------------------------.
 | Register 0x00000044 EMAC_INGRESS_PCS_INT_STAT_2_4_ |
 +----------------------------------------------------+
 | bit  23:12 R/W  RX_REM_FAULT_I                     |
 | bit  11:0  R/W  RX_LOC_FAULT_I                     |
 +---------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_UNUSED_MASK        0xff000000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_REM_FAULT_I_OFF 12
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_2_4_BIT_RX_LOC_FAULT_I_OFF 0

/*----------------------------------------------------.
 | Register 0x00000048 EMAC_INGRESS_PCS_INT_STAT_3_4_ |
 +----------------------------------------------------+
 | bit  11:0 R/W  HI_BER_I                            |
 +---------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_UNUSED_MASK  0xfffff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_3_4_BIT_HI_BER_I_OFF 0

/*----------------------------------------------------.
 | Register 0x0000004c EMAC_INGRESS_PCS_INT_STAT_4_4_ |
 +----------------------------------------------------+
 | bit  18:16 R/W  ALIGN_STATUS_I                     |
 | bit  11:0  R/W  BLOCK_LOCK_I                       |
 +---------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_UNUSED_MASK        0xfff8f000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_MSK 0x00070000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_ALIGN_STATUS_I_OFF 16
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_MSK   0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_STAT_4_4_BIT_BLOCK_LOCK_I_OFF   0

/*-------------------------------------------.
 | Register 0x00000050 AGB_OVERFLOW_INT_STAT |
 +-------------------------------------------+
 | bit  23:12 R/W  ING_AGB_OVERFLOW_I        |
 | bit  11:0  R/W  EGR_AGB_OVERFLOW_I        |
 +------------------------------------------*/
#define PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_UNUSED_MASK            0xff000000
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_MSK 0x00fff000
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_ING_AGB_OVERFLOW_I_OFF 12
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_MSK 0x00000fff
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_STAT_BIT_EGR_AGB_OVERFLOW_I_OFF 0

/*---------------------------------------------------.
 | Register 0x00000058 EMAC_EGRESS_OVERFLOW_INT_STAT |
 +---------------------------------------------------+
 | bit  23:12 R/W  EMAC_TX_UNDERFLOW_I               |
 | bit  11:0  R/W  EMAC_TX_OVERFLOW_I                |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_UNUSED_MASK             0xff000000
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_UNDERFLOW_I_OFF 12
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_MSK  0x00000fff
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_STAT_BIT_EMAC_TX_OVERFLOW_I_OFF  0

/*--------------------------------------------------.
 | Register 0x00000060 EMAC_INGRESS_PCS_INT_EN_1_4_ |
 +--------------------------------------------------+
 | bit  23:12 R/W  RX_REM_FAULT_E                   |
 | bit  11:0  R/W  RX_LOC_FAULT_E                   |
 +-------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_UNUSED_MASK        0xff000000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_REM_FAULT_E_OFF 12
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_1_4_BIT_RX_LOC_FAULT_E_OFF 0

/*--------------------------------------------------.
 | Register 0x00000064 EMAC_INGRESS_PCS_INT_EN_2_4_ |
 +--------------------------------------------------+
 | bit  11:0 R/W  PHY_LOS_E                         |
 +-------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_UNUSED_MASK   0xfffff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_2_4_BIT_PHY_LOS_E_OFF 0

/*--------------------------------------------------.
 | Register 0x00000068 EMAC_INGRESS_PCS_INT_EN_3_4_ |
 +--------------------------------------------------+
 | bit  11:0 R/W  HI_BER_E                          |
 +-------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_UNUSED_MASK  0xfffff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_3_4_BIT_HI_BER_E_OFF 0

/*--------------------------------------------------.
 | Register 0x0000006c EMAC_INGRESS_PCS_INT_EN_4_4_ |
 +--------------------------------------------------+
 | bit  18:16 R/W  ALIGN_STATUS_E                   |
 | bit  11:0  R/W  BLOCK_LOCK_E                     |
 +-------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_UNUSED_MASK        0xfff8f000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_MSK 0x00070000
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_ALIGN_STATUS_E_OFF 16
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_MSK   0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PCS_INT_EN_4_4_BIT_BLOCK_LOCK_E_OFF   0

/*-----------------------------------------.
 | Register 0x00000070 AGB_OVERFLOW_INT_EN |
 +-----------------------------------------+
 | bit  23:12 R/W  ING_AGB_OVERFLOW_E      |
 | bit  11:0  R/W  EGR_AGB_OVERFLOW_E      |
 +----------------------------------------*/
#define PMC_EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_UNUSED_MASK            0xff000000
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_MSK 0x00fff000
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_ING_AGB_OVERFLOW_E_OFF 12
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_MSK 0x00000fff
#define EMAC120_TOP_REG_AGB_OVERFLOW_INT_EN_BIT_EGR_AGB_OVERFLOW_E_OFF 0

/*-------------------------------------------------.
 | Register 0x00000078 EMAC_EGRESS_OVERFLOW_INT_EN |
 +-------------------------------------------------+
 | bit  23:12 R/W  EMAC_TX_UNDERFLOW_E             |
 | bit  11:0  R/W  EMAC_TX_OVERFLOW_E              |
 +------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_UNUSED_MASK             0xff000000
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_UNDERFLOW_E_OFF 12
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_MSK  0x00000fff
#define EMAC120_TOP_REG_EMAC_EGRESS_OVERFLOW_INT_EN_BIT_EMAC_TX_OVERFLOW_E_OFF  0

/*------------------------------------.
 | Register 0x00000080 INGRESS_DSI_IF |
 +------------------------------------+
 | bit  23:22 R/W  ING_DSI_CTRL_11    |
 | bit  21:20 R/W  ING_DSI_CTRL_10    |
 | bit  19:18 R/W  ING_DSI_CTRL_9     |
 | bit  17:16 R/W  ING_DSI_CTRL_8     |
 | bit  15:14 R/W  ING_DSI_CTRL_7     |
 | bit  13:12 R/W  ING_DSI_CTRL_6     |
 | bit  11:10 R/W  ING_DSI_CTRL_5     |
 | bit  9:8   R/W  ING_DSI_CTRL_4     |
 | bit  7:6   R/W  ING_DSI_CTRL_3     |
 | bit  5:4   R/W  ING_DSI_CTRL_2     |
 | bit  3:2   R/W  ING_DSI_CTRL_1     |
 | bit  1:0   R/W  ING_DSI_CTRL_0     |
 +-----------------------------------*/
#define PMC_EMAC120_TOP_REG_INGRESS_DSI_IF_UNUSED_MASK         0xff000000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_MSK 0x00c00000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_11_OFF 22
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_MSK 0x00300000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_10_OFF 20
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_MSK  0x000c0000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_9_OFF  18
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_MSK  0x00030000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_8_OFF  16
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_MSK  0x0000c000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_7_OFF  14
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_MSK  0x00003000
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_6_OFF  12
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_MSK  0x00000c00
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_5_OFF  10
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_MSK  0x00000300
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_4_OFF  8
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_MSK  0x000000c0
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_3_OFF  6
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_MSK  0x00000030
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_2_OFF  4
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_MSK  0x0000000c
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_1_OFF  2
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_MSK  0x00000003
#define EMAC120_TOP_REG_INGRESS_DSI_IF_BIT_ING_DSI_CTRL_0_OFF  0

/*-----------------------------------.
 | Register 0x00000084 EGRESS_DSI_IF |
 +-----------------------------------+
 | bit  23:22 R/W  EGR_DSI_CTRL_11   |
 | bit  21:20 R/W  EGR_DSI_CTRL_10   |
 | bit  19:18 R/W  EGR_DSI_CTRL_9    |
 | bit  17:16 R/W  EGR_DSI_CTRL_8    |
 | bit  15:14 R/W  EGR_DSI_CTRL_7    |
 | bit  13:12 R/W  EGR_DSI_CTRL_6    |
 | bit  11:10 R/W  EGR_DSI_CTRL_5    |
 | bit  9:8   R/W  EGR_DSI_CTRL_4    |
 | bit  7:6   R/W  EGR_DSI_CTRL_3    |
 | bit  5:4   R/W  EGR_DSI_CTRL_2    |
 | bit  3:2   R/W  EGR_DSI_CTRL_1    |
 | bit  1:0   R/W  EGR_DSI_CTRL_0    |
 +----------------------------------*/
#define PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_UNUSED_MASK         0xff000000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_MSK 0x00c00000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_11_OFF 22
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_MSK 0x00300000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_10_OFF 20
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_MSK  0x000c0000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_9_OFF  18
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_MSK  0x00030000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_8_OFF  16
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_MSK  0x0000c000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_7_OFF  14
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_MSK  0x00003000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_6_OFF  12
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_MSK  0x00000c00
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_5_OFF  10
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_MSK  0x00000300
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_4_OFF  8
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_MSK  0x000000c0
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_3_OFF  6
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_MSK  0x00000030
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_2_OFF  4
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_MSK  0x0000000c
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_1_OFF  2
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_MSK  0x00000003
#define EMAC120_TOP_REG_EGRESS_DSI_IF_BIT_EGR_DSI_CTRL_0_OFF  0

/*---------------------------------------------------.
 | Register 0x00000088 EGRESS_DSI_IF_DATA_SQUELCHING |
 +---------------------------------------------------+
 | bit  27:16 R/W  EGR_DSI_OVR_EN                    |
 | bit  15:0  R/W  EGR_DSI_OVR_PAT                   |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_UNUSED_MASK         0xf0000000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_MSK  0x0fff0000
#define EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_EN_OFF  16
#define EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_MSK 0x0000ffff
#define EMAC120_TOP_REG_EGRESS_DSI_IF_DATA_SQUELCHING_BIT_EGR_DSI_OVR_PAT_OFF 0

/*---------------------------------------.
 | Register 0x0000008c INGRESS_DATA_SINK |
 +---------------------------------------+
 | bit  11:0 R/W  ING_DATA_SINK          |
 +--------------------------------------*/
#define PMC_EMAC120_TOP_REG_INGRESS_DATA_SINK_UNUSED_MASK       0xfffff000
#define EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_MSK 0x00000fff
#define EMAC120_TOP_REG_INGRESS_DATA_SINK_BIT_ING_DATA_SINK_OFF 0

/*------------------------------------------------.
 | Register 0x00000090 EGRESS_TIMESTAMP_MASK_CTRL |
 +------------------------------------------------+
 | bit  30:16 R/W  TS_MASK_END                    |
 | bit  14:0  R/W  TS_MASK_START                  |
 +-----------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_UNUSED_MASK       0x80008000
#define EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_MSK   0x7fff0000
#define EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_END_OFF   16
#define EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_MSK 0x00007fff
#define EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_CTRL_BIT_TS_MASK_START_OFF 0

/*---------------------------------------------------.
 | Register 0x00000094 EGRESS_TIMESTAMP_MASK_DISABLE |
 +---------------------------------------------------+
 | bit  2:0 R/W  TS_MASK_DISABLE                     |
 +--------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_UNUSED_MASK         0xfffffff8
#define EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_MSK 0x00000007
#define EMAC120_TOP_REG_EGRESS_TIMESTAMP_MASK_DISABLE_BIT_TS_MASK_DISABLE_OFF 0

/*------------------------------------------------.
 | Register 0x00000098 EMAC_INGRESS_DROP_ERR_MASK |
 +------------------------------------------------+
 | bit  15:8 R/W  EMAC_ING_DPI_DROP_MASK          |
 | bit  7:0  R/W  EMAC_ING_DPI_ERR_MASK           |
 +-----------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_UNUSED_MASK                0xffff0000
#define EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_MSK 0x0000ff00
#define EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_DROP_MASK_OFF 8
#define EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_MSK  0x000000ff
#define EMAC120_TOP_REG_EMAC_INGRESS_DROP_ERR_MASK_BIT_EMAC_ING_DPI_ERR_MASK_OFF  0

/*-----------------------------------------------------------------.
 | Register 0x0000009c EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK |
 +-----------------------------------------------------------------+
 | bit  23:12 R/W  EMAC_ING_DPI_FCS_MASK                           |
 | bit  11:0  R/W  EMAC_ING_DPI_LEN_MASK                           |
 +----------------------------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_UNUSED_MASK               0xff000000
#define EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_MSK 0x00fff000
#define EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_FCS_MASK_OFF 12
#define EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_MSK 0x00000fff
#define EMAC120_TOP_REG_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_BIT_EMAC_ING_DPI_LEN_MASK_OFF 0

/*------------------------------------------------.
 | Register 0x000000a0 EGRESS_CREDIT_CNT_CFG      |
 +------------------------------------------------+
 | bit  29:24 R/W  EMAC_TX_CREDIT_COUNT_MAX_100G  |
 | bit  23:20 R/W  EMAC_TX_CREDIT_COUNT_MAX_40G   |
 | bit  19:16 R/W  EMAC_TX_CREDIT_COUNT_MAX_10G   |
 | bit  13:8  R/W  EMAC_TX_CREDIT_COUNT_INIT_100G |
 | bit  7:4   R/W  EMAC_TX_CREDIT_COUNT_INIT_40G  |
 | bit  3:0   R/W  EMAC_TX_CREDIT_COUNT_INIT_10G  |
 +-----------------------------------------------*/
#define PMC_EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_UNUSED_MASK                        0xc000c000
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_MSK  0x3f000000
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_100G_OFF  24
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_MSK   0x00f00000
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_40G_OFF   20
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_MSK   0x000f0000
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_MAX_10G_OFF   16
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_MSK 0x00003f00
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_100G_OFF 8
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_MSK  0x000000f0
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_40G_OFF  4
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_MSK  0x0000000f
#define EMAC120_TOP_REG_EGRESS_CREDIT_CNT_CFG_BIT_EMAC_TX_CREDIT_COUNT_INIT_10G_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_REGS_H */
