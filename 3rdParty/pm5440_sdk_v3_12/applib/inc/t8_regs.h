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
 *     t8 block
 *****************************************************************************/
#ifndef _T8_REGS_H
#define _T8_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PM20_82_24_REG_PCIE3_CFG_1                     0x00000000
#define PMC_PM20_82_24_REG_PCIE3_CFG_2                     0x00000004
#define PMC_PM20_82_24_REG_ANALOG_SIPO_BIST_STATUS_REG     0x00000008
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_1                   0x0000000c
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_2                   0x00000010
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_3                   0x00000014
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_4                   0x00000018
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_5                   0x0000001c
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_6                   0x00000020
#define PMC_PM20_82_24_REG_TX_CNTR_STA                     0x00000024
#define PMC_PM20_82_24_REG_DIG_LPBK_CFG                    0x00000028
#define PMC_PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG        0x0000002c
#define PMC_PM20_82_24_REG_OFFSET_RAM_CFG                  0x00000030
#define PMC_PM20_82_24_REG_PLL_CNFG_REG1                   0x00000034
#define PMC_PM20_82_24_REG_ADC_OC_CFG_1                    0x00000038
#define PMC_PM20_82_24_REG_ADC_OC_CFG_2                    0x0000003c
#define PMC_PM20_82_24_REG_ADC_OC_REGISTER3                0x00000040
#define PMC_PM20_82_24_REG_ADC_OC_STS                      0x00000044
#define PMC_PM20_82_24_REG_PMM_WRAP_CNFG_REG1              0x00000048
#define PMC_PM20_82_24_REG_PMM_WRAP_CNFG_REG2              0x0000004c
#define PMC_PM20_82_24_REG_SIPO_CNFG_REG                   0x00000050
#define PMC_PM20_82_24_REG_PISO_CNFG_REG                   0x00000054
#define PMC_PM20_82_24_REG_TX_CNTRL_OVERRIDE_1             0x00000058
#define PMC_PM20_82_24_REG_TX_CNTRL_OVERRIDE_REG_5         0x00000068
#define PMC_PM20_82_24_REG_CLK_DIV_FACT                    0x0000006c
#define PMC_PM20_82_24_REG_DEGLITCHER_EN                   0x00000070
#define PMC_PM20_82_24_REG_DIGITAL_RSTB_REG                0x00000074
#define PMC_PM20_82_24_REG_IE_REG                          0x00000078
#define PMC_PM20_82_24_REG_INT_REG                         0x0000007c
#define PMC_PM20_82_24_REG_IV_REG                          0x00000080
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_0_REG            0x00000084
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_1_REG            0x00000088
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_2_REG            0x0000008c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_3_REG            0x00000090
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_4_REG            0x00000094
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_5_REG            0x00000098
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_6_REG            0x0000009c
#define PMC_PM20_82_24_REG_TX_CTRL_STS_REG1                0x000000a0
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_7_REG            0x00000100
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_8_REG            0x00000104
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_9_REG            0x00000108
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_10_REG           0x0000010c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_11_REG           0x00000110
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_12_REG           0x00000114
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_13_REG           0x00000118
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_14_REG           0x0000011c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_15_REG           0x00000120
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_16_REG           0x00000124
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_17_REG           0x00000128
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_18_REG           0x0000012c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_19_REG           0x00000130
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_20_REG           0x00000134
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_21_REG           0x00000138
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_22_REG           0x0000013c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_23_REG           0x00000140
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_24_REG           0x00000144
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_25_REG           0x00000148
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_26_REG           0x0000014c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_27_REG           0x00000150
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_28_REG           0x00000154
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_29_REG           0x00000158
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_30_REG           0x0000015c
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_31_REG           0x00000160
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_0_REG             0x00000164
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_1_REG             0x00000168
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_2_REG             0x0000016c
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_3_REG             0x00000170
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_4_REG             0x00000174
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_5_REG             0x00000178
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_6_REG             0x0000017c
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_7_REG             0x00000180
#define PMC_PM20_82_24_REG_PGA_LUT_CTRL_REG                0x00000184
#define PMC_PM20_82_24_REG_PLL_CNFG_REG2                   0x00000188
#define PMC_PM20_82_24_REG_PLL_CNFG_REG4                   0x0000018c
#define PMC_PM20_82_24_REG_PLL_CNFG_REG6                   0x00000190
#define PMC_PM20_82_24_REG_PLL_STS_REG                     0x00000194
#define PMC_PM20_82_24_REG_MDSP_CFG_REG1                   0x00000198
#define PMC_PM20_82_24_REG_MDSP_CFG_REG2                   0x0000019c
#define PMC_PM20_82_24_REG_PGA_CFG_REG                     0x00000204
#define PMC_PM20_82_24_REG_DIAG_CNFG_REG1                  0x00000208
#define PMC_PM20_82_24_REG_DIAG_CNFG_REG2                  0x0000020c
#define PMC_PM20_82_24_REG_DIAG_STATUS_REG1                0x00000210
#define PMC_PM20_82_24_REG_DIAG_STATUS_REG2                0x00000214
#define PMC_PM20_82_24_REG_OFFSET_RAM_CNFG_REG             0x00000218
#define PMC_PM20_82_24_REG_DIG_LPBK_CTRL_REG               0x0000021c
#define PMC_PM20_82_24_REG_ADC_OC_REGISTER4                0x00000220
#define PMC_PM20_82_24_REG_ADC_OC_REGISTER5                0x00000224
#define PMC_PM20_82_24_REG_GCOC_MABC_CFG_REG               0x00000228
#define PMC_PM20_82_24_REG_DLOS_CFG                        0x00000230
#define PMC_PM20_82_24_REG_PATT_GEN_CNFG_REG1              0x00000234
#define PMC_PM20_82_24_REG_PATT_GEN_CNFG_REG2              0x00000238
#define PMC_PM20_82_24_REG_PATT_CHK_CNFG_REG1              0x0000023c
#define PMC_PM20_82_24_REG_PATT_CHK_CNFG_REG2              0x00000240
#define PMC_PM20_82_24_REG_PATT_CHK_STATUS_REG1            0x00000244
#define PMC_PM20_82_24_REG_TX_CNTRL_OVERRIDE_5             0x00000248
#define PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0( N ) (0x0000024c + (N) * 0x4)

/*---------------------------------.
 | Register 0x00000000 PCIE3_CFG_1 |
 +---------------------------------+
 | bit  26    R/W  TXRXCS_ATMSB    |
 | bit  25:22 R/W  TXRXCS_MODE     |
 | bit  21:18 R/W  TXRXCS_CTRL     |
 | bit  17    R/W  PISO_ATMSB      |
 | bit  10    R/W  PISO_ALTPAT_EN  |
 | bit  9     R/W  PISO_ENB        |
 | bit  0     R/W  PISO_DLB_EN     |
 +--------------------------------*/
#define PMC_PM20_82_24_REG_PCIE3_CFG_1_UNUSED_MASK        0xf801f9fe
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_TXRXCS_ATMSB_MSK   0x04000000
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_TXRXCS_ATMSB_OFF   26
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_TXRXCS_MODE_MSK    0x03c00000
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_TXRXCS_MODE_OFF    22
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_TXRXCS_CTRL_MSK    0x003c0000
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_TXRXCS_CTRL_OFF    18
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_ATMSB_MSK     0x00020000
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_ATMSB_OFF     17
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_ALTPAT_EN_MSK 0x00000400
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_ALTPAT_EN_OFF 10
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_ENB_MSK       0x00000200
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_ENB_OFF       9
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_DLB_EN_MSK    0x00000001
#define PM20_82_24_REG_PCIE3_CFG_1_BIT_PISO_DLB_EN_OFF    0

/*---------------------------------.
 | Register 0x00000004 PCIE3_CFG_2 |
 +---------------------------------+
 | bit  25    R/W  BADJ_ENB        |
 | bit  24    R/W  TX_TEST_SEL     |
 | bit  23    R/W  TX_TEST_EN      |
 | bit  22:20 R/W  ADC_ENB         |
 | bit  19:12 R/W  ADC_MODE        |
 | bit  11    R/W  ADC_BIST        |
 | bit  10    R/W  ADC_DLB_EN      |
 | bit  9     R/W  ADC_DAC_ENB     |
 | bit  8     R/W  RXCM_ENB        |
 | bit  5     R/W  ADC_ATMSB       |
 | bit  3     R/W  RX_ATMSB        |
 +--------------------------------*/
#define PMC_PM20_82_24_REG_PCIE3_CFG_2_UNUSED_MASK     0xfc0000d7
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_BADJ_ENB_MSK    0x02000000
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_BADJ_ENB_OFF    25
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_TX_TEST_SEL_MSK 0x01000000
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_TX_TEST_SEL_OFF 24
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_TX_TEST_EN_MSK  0x00800000
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_TX_TEST_EN_OFF  23
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_ENB_MSK     0x00700000
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_ENB_OFF     20
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_MODE_MSK    0x000ff000
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_MODE_OFF    12
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_BIST_MSK    0x00000800
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_BIST_OFF    11
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_DLB_EN_MSK  0x00000400
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_DLB_EN_OFF  10
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_DAC_ENB_MSK 0x00000200
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_DAC_ENB_OFF 9
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_RXCM_ENB_MSK    0x00000100
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_RXCM_ENB_OFF    8
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_ATMSB_MSK   0x00000020
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_ADC_ATMSB_OFF   5
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_RX_ATMSB_MSK    0x00000008
#define PM20_82_24_REG_PCIE3_CFG_2_BIT_RX_ATMSB_OFF    3

/*-------------------------------------------------.
 | Register 0x00000008 ANALOG_SIPO_BIST_STATUS_REG |
 +-------------------------------------------------+
 | bit  1 R  BIST_SYNC                             |
 | bit  0 R  BIST_ERR                              |
 +------------------------------------------------*/
#define PMC_PM20_82_24_REG_ANALOG_SIPO_BIST_STATUS_REG_UNUSED_MASK   0xfffffffc
#define PM20_82_24_REG_ANALOG_SIPO_BIST_STATUS_REG_BIT_BIST_SYNC_MSK 0x00000002
#define PM20_82_24_REG_ANALOG_SIPO_BIST_STATUS_REG_BIT_BIST_SYNC_OFF 1
#define PM20_82_24_REG_ANALOG_SIPO_BIST_STATUS_REG_BIT_BIST_ERR_MSK  0x00000001
#define PM20_82_24_REG_ANALOG_SIPO_BIST_STATUS_REG_BIT_BIST_ERR_OFF  0

/*-----------------------------------.
 | Register 0x0000000c TX_CNTR_CFG_1 |
 +-----------------------------------+
 | bit  26 R/W  CM_MODN_B            |
 | bit  25 R/W  CM_MODP_B            |
 | bit  24 R/W  TX_ATMSB             |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_1_UNUSED_MASK   0xf8ffffff
#define PM20_82_24_REG_TX_CNTR_CFG_1_BIT_CM_MODN_B_MSK 0x04000000
#define PM20_82_24_REG_TX_CNTR_CFG_1_BIT_CM_MODN_B_OFF 26
#define PM20_82_24_REG_TX_CNTR_CFG_1_BIT_CM_MODP_B_MSK 0x02000000
#define PM20_82_24_REG_TX_CNTR_CFG_1_BIT_CM_MODP_B_OFF 25
#define PM20_82_24_REG_TX_CNTR_CFG_1_BIT_TX_ATMSB_MSK  0x01000000
#define PM20_82_24_REG_TX_CNTR_CFG_1_BIT_TX_ATMSB_OFF  24

/*-----------------------------------.
 | Register 0x00000010 TX_CNTR_CFG_2 |
 +-----------------------------------+
 | bit  31:30 R/W  PISO_PRE2_MODE    |
 | bit  25    R/W  PISO_PRE2_ENABLE  |
 | bit  24    R/W  PISO_PRE1_ENABLE  |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_2_UNUSED_MASK          0x3cffffff
#define PM20_82_24_REG_TX_CNTR_CFG_2_BIT_PISO_PRE2_MODE_MSK   0xc0000000
#define PM20_82_24_REG_TX_CNTR_CFG_2_BIT_PISO_PRE2_MODE_OFF   30
#define PM20_82_24_REG_TX_CNTR_CFG_2_BIT_PISO_PRE2_ENABLE_MSK 0x02000000
#define PM20_82_24_REG_TX_CNTR_CFG_2_BIT_PISO_PRE2_ENABLE_OFF 25
#define PM20_82_24_REG_TX_CNTR_CFG_2_BIT_PISO_PRE1_ENABLE_MSK 0x01000000
#define PM20_82_24_REG_TX_CNTR_CFG_2_BIT_PISO_PRE1_ENABLE_OFF 24

/*-----------------------------------.
 | Register 0x00000014 TX_CNTR_CFG_3 |
 +-----------------------------------+
 | bit  21:17 R/W  TX_DER_CORE       |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_3_UNUSED_MASK     0xffc1ffff
#define PM20_82_24_REG_TX_CNTR_CFG_3_BIT_TX_DER_CORE_MSK 0x003e0000
#define PM20_82_24_REG_TX_CNTR_CFG_3_BIT_TX_DER_CORE_OFF 17

/*-----------------------------------.
 | Register 0x00000018 TX_CNTR_CFG_4 |
 +-----------------------------------+
 | bit  26 R/W  TX_AMP_EMPH_EN       |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_4_UNUSED_MASK        0xfbffffff
#define PM20_82_24_REG_TX_CNTR_CFG_4_BIT_TX_AMP_EMPH_EN_MSK 0x04000000
#define PM20_82_24_REG_TX_CNTR_CFG_4_BIT_TX_AMP_EMPH_EN_OFF 26

/*---------------------------------------.
 | Register 0x0000001c TX_CNTR_CFG_5     |
 +---------------------------------------+
 | bit  31   R/W  RXDET_CANCEL_OVERRIDE  |
 | bit  17   R/W  TX_CTRL_CMD_EN         |
 | bit  16   R/W  TX_EDGE_DELAY_SEL_CORE |
 | bit  15:0 R/W  TX_CTRL_EDGE_CORE      |
 +--------------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_5_UNUSED_MASK                0x7ffc0000
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_RXDET_CANCEL_OVERRIDE_MSK  0x80000000
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_RXDET_CANCEL_OVERRIDE_OFF  31
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_TX_CTRL_CMD_EN_MSK         0x00020000
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_TX_CTRL_CMD_EN_OFF         17
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_TX_EDGE_DELAY_SEL_CORE_MSK 0x00010000
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_TX_EDGE_DELAY_SEL_CORE_OFF 16
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_TX_CTRL_EDGE_CORE_MSK      0x0000ffff
#define PM20_82_24_REG_TX_CNTR_CFG_5_BIT_TX_CTRL_EDGE_CORE_OFF      0

/*-----------------------------------.
 | Register 0x00000020 TX_CNTR_CFG_6 |
 +-----------------------------------+
 | bit  15:0 R/W  TX_COUNTER         |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_CFG_6_UNUSED_MASK    0xffff0000
#define PM20_82_24_REG_TX_CNTR_CFG_6_BIT_TX_COUNTER_MSK 0x0000ffff
#define PM20_82_24_REG_TX_CNTR_CFG_6_BIT_TX_COUNTER_OFF 0

/*---------------------------------.
 | Register 0x00000024 TX_CNTR_STA |
 +---------------------------------+
 | bit  15:0 R  TX_CODE_COUNT      |
 +--------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTR_STA_UNUSED_MASK       0xffff0000
#define PM20_82_24_REG_TX_CNTR_STA_BIT_TX_CODE_COUNT_MSK 0x0000ffff
#define PM20_82_24_REG_TX_CNTR_STA_BIT_TX_CODE_COUNT_OFF 0

/*----------------------------------.
 | Register 0x00000028 DIG_LPBK_CFG |
 +----------------------------------+
 | bit  7 R/W  RX_TO_TX_LPBK_EN     |
 | bit  3 R/W  TX_TO_RX_LPBK_EN     |
 +---------------------------------*/
#define PMC_PM20_82_24_REG_DIG_LPBK_CFG_UNUSED_MASK          0xffffff77
#define PM20_82_24_REG_DIG_LPBK_CFG_BIT_RX_TO_TX_LPBK_EN_MSK 0x00000080
#define PM20_82_24_REG_DIG_LPBK_CFG_BIT_RX_TO_TX_LPBK_EN_OFF 7
#define PM20_82_24_REG_DIG_LPBK_CFG_BIT_TX_TO_RX_LPBK_EN_MSK 0x00000008
#define PM20_82_24_REG_DIG_LPBK_CFG_BIT_TX_TO_RX_LPBK_EN_OFF 3

/*----------------------------------------------.
 | Register 0x0000002c ANALOG_SIPO_BIST_CFG_REG |
 +----------------------------------------------+
 | bit  6   R/W  TIME_REVERSE                   |
 | bit  3:1 R/W  A_SIPO_BIST_SEL                |
 | bit  0   R/W  A_SIPO_BIST_EN                 |
 +---------------------------------------------*/
#define PMC_PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_UNUSED_MASK         0xffffffb0
#define PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_BIT_TIME_REVERSE_MSK    0x00000040
#define PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_BIT_TIME_REVERSE_OFF    6
#define PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_BIT_A_SIPO_BIST_SEL_MSK 0x0000000e
#define PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_BIT_A_SIPO_BIST_SEL_OFF 1
#define PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_BIT_A_SIPO_BIST_EN_MSK  0x00000001
#define PM20_82_24_REG_ANALOG_SIPO_BIST_CFG_REG_BIT_A_SIPO_BIST_EN_OFF  0

/*------------------------------------.
 | Register 0x00000030 OFFSET_RAM_CFG |
 +------------------------------------+
 | bit  6   R/W  GCOC_RAM_LS          |
 | bit  4   R/W  GCOC_RAM_SD          |
 | bit  3   R/W  GCOC_RAM_DS          |
 | bit  2:1 R/W  GCOC_RAM_ERR_INJ     |
 +-----------------------------------*/
#define PMC_PM20_82_24_REG_OFFSET_RAM_CFG_UNUSED_MASK          0xffffff01
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_LS_MSK      0x00000040
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_LS_OFF      6
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_SD_MSK      0x00000010
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_SD_OFF      4
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_DS_MSK      0x00000008
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_DS_OFF      3
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_ERR_INJ_MSK 0x00000006
#define PM20_82_24_REG_OFFSET_RAM_CFG_BIT_GCOC_RAM_ERR_INJ_OFF 1

/*-----------------------------------.
 | Register 0x00000034 PLL_CNFG_REG1 |
 +-----------------------------------+
 | bit  15:12 R/W  LOCK_PRECISION    |
 | bit  4     R/W  LOAD_CLK_CNTRS    |
 | bit  2     R/W  RESET_HISTORY     |
 | bit  0     R/W  PPM_OFF_RD_REQ    |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_PLL_CNFG_REG1_UNUSED_MASK        0xffff0fea
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_MSK 0x0000f000
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_LOCK_PRECISION_OFF 12
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_LOAD_CLK_CNTRS_MSK 0x00000010
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_LOAD_CLK_CNTRS_OFF 4
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_MSK  0x00000004
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_RESET_HISTORY_OFF  2
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_MSK 0x00000001
#define PM20_82_24_REG_PLL_CNFG_REG1_BIT_PPM_OFF_RD_REQ_OFF 0

/*--------------------------------------.
 | Register 0x00000038 ADC_OC_CFG_1     |
 +--------------------------------------+
 | bit  31    R/W  ADCOC_BIST_EN        |
 | bit  30    R/W  ADCOC_RD_OVR         |
 | bit  29    R/W  ADCOC_OFFSET_EN      |
 | bit  28    R/W  ADCOC_CALIB_PROG_EN  |
 | bit  27:24 R/W  ADCOC_ACC_DELAY      |
 | bit  23:14 R/W  ADCOC_DAC_DELAY      |
 | bit  8:0   R/W  ADCOC_N_ACCUM_CYCLES |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_ADC_OC_CFG_1_UNUSED_MASK              0x00003e00
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_BIST_EN_MSK        0x80000000
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_BIST_EN_OFF        31
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_RD_OVR_MSK         0x40000000
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_RD_OVR_OFF         30
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_OFFSET_EN_MSK      0x20000000
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_OFFSET_EN_OFF      29
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_CALIB_PROG_EN_MSK  0x10000000
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_CALIB_PROG_EN_OFF  28
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_ACC_DELAY_MSK      0x0f000000
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_ACC_DELAY_OFF      24
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_DAC_DELAY_MSK      0x00ffc000
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_DAC_DELAY_OFF      14
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_N_ACCUM_CYCLES_MSK 0x000001ff
#define PM20_82_24_REG_ADC_OC_CFG_1_BIT_ADCOC_N_ACCUM_CYCLES_OFF 0

/*---------------------------------------.
 | Register 0x0000003c ADC_OC_CFG_2      |
 +---------------------------------------+
 | bit  31:28 R/W  ADCOC_LINEAR_STEP     |
 | bit  27:18 R/W  ADCOC_CAL_DELAY       |
 | bit  17    R/W  ADCOC_MABC_INT_OVR_EN |
 | bit  16    R/W  ADCOC_ADC_WR_OVR      |
 | bit  15:8  R/W  ADCOC_ADC_A_OVR       |
 | bit  7:0   R/W  ADCOC_ADC_D_OVR       |
 +--------------------------------------*/
#define PMC_PM20_82_24_REG_ADC_OC_CFG_2_UNUSED_MASK               0x00000000
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_LINEAR_STEP_MSK     0xf0000000
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_LINEAR_STEP_OFF     28
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_CAL_DELAY_MSK       0x0ffc0000
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_CAL_DELAY_OFF       18
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_MABC_INT_OVR_EN_MSK 0x00020000
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_MABC_INT_OVR_EN_OFF 17
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_ADC_WR_OVR_MSK      0x00010000
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_ADC_WR_OVR_OFF      16
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_ADC_A_OVR_MSK       0x0000ff00
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_ADC_A_OVR_OFF       8
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_ADC_D_OVR_MSK       0x000000ff
#define PM20_82_24_REG_ADC_OC_CFG_2_BIT_ADCOC_ADC_D_OVR_OFF       0

/*--------------------------------------.
 | Register 0x00000040 ADC_OC_REGISTER3 |
 +--------------------------------------+
 | bit  31:0 R/W  THR_MAP31_0           |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_ADC_OC_REGISTER3_UNUSED_MASK     0x00000000
#define PM20_82_24_REG_ADC_OC_REGISTER3_BIT_THR_MAP31_0_MSK 0xffffffff
#define PM20_82_24_REG_ADC_OC_REGISTER3_BIT_THR_MAP31_0_OFF 0

/*---------------------------------.
 | Register 0x00000044 ADC_OC_STS  |
 +---------------------------------+
 | bit  9:2 R  ADC_Q               |
 | bit  1   R  ADCOC_PROG_REF_DONE |
 | bit  0   R  ADCOC_CALIB_DONE    |
 +--------------------------------*/
#define PMC_PM20_82_24_REG_ADC_OC_STS_UNUSED_MASK             0xfffffc00
#define PM20_82_24_REG_ADC_OC_STS_BIT_ADC_Q_MSK               0x000003fc
#define PM20_82_24_REG_ADC_OC_STS_BIT_ADC_Q_OFF               2
#define PM20_82_24_REG_ADC_OC_STS_BIT_ADCOC_PROG_REF_DONE_MSK 0x00000002
#define PM20_82_24_REG_ADC_OC_STS_BIT_ADCOC_PROG_REF_DONE_OFF 1
#define PM20_82_24_REG_ADC_OC_STS_BIT_ADCOC_CALIB_DONE_MSK    0x00000001
#define PM20_82_24_REG_ADC_OC_STS_BIT_ADCOC_CALIB_DONE_OFF    0

/*----------------------------------------.
 | Register 0x00000048 PMM_WRAP_CNFG_REG1 |
 +----------------------------------------+
 | bit  29:16 R/W  DSM_DENOM              |
 | bit  1:0   R/W  DSM_TYPE               |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_PMM_WRAP_CNFG_REG1_UNUSED_MASK   0xc000fffc
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG1_BIT_DSM_DENOM_MSK 0x3fff0000
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG1_BIT_DSM_DENOM_OFF 16
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG1_BIT_DSM_TYPE_MSK  0x00000003
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG1_BIT_DSM_TYPE_OFF  0

/*----------------------------------------.
 | Register 0x0000004c PMM_WRAP_CNFG_REG2 |
 +----------------------------------------+
 | bit  31    R/W  PMM_WRAP_EN            |
 | bit  24:16 R/W  PKT_SIZE               |
 | bit  15:0  R/W  PKT_PERIOD             |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_PMM_WRAP_CNFG_REG2_UNUSED_MASK     0x7e000000
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG2_BIT_PMM_WRAP_EN_MSK 0x80000000
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG2_BIT_PMM_WRAP_EN_OFF 31
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG2_BIT_PKT_SIZE_MSK    0x01ff0000
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG2_BIT_PKT_SIZE_OFF    16
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG2_BIT_PKT_PERIOD_MSK  0x0000ffff
#define PM20_82_24_REG_PMM_WRAP_CNFG_REG2_BIT_PKT_PERIOD_OFF  0

/*-----------------------------------.
 | Register 0x00000050 SIPO_CNFG_REG |
 +-----------------------------------+
 | bit  2:1 R/W  SIPO_DIV_RATIO      |
 | bit  0   R/W  SIPO_BUS_WIDTH      |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_SIPO_CNFG_REG_UNUSED_MASK        0xfffffff8
#define PM20_82_24_REG_SIPO_CNFG_REG_BIT_SIPO_DIV_RATIO_MSK 0x00000006
#define PM20_82_24_REG_SIPO_CNFG_REG_BIT_SIPO_DIV_RATIO_OFF 1
#define PM20_82_24_REG_SIPO_CNFG_REG_BIT_SIPO_BUS_WIDTH_MSK 0x00000001
#define PM20_82_24_REG_SIPO_CNFG_REG_BIT_SIPO_BUS_WIDTH_OFF 0

/*-----------------------------------.
 | Register 0x00000054 PISO_CNFG_REG |
 +-----------------------------------+
 | bit  2:1 R/W  PISO_DIV_RATIO      |
 | bit  0   R/W  PISO_BUS_WIDTH      |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_PISO_CNFG_REG_UNUSED_MASK        0xfffffff8
#define PM20_82_24_REG_PISO_CNFG_REG_BIT_PISO_DIV_RATIO_MSK 0x00000006
#define PM20_82_24_REG_PISO_CNFG_REG_BIT_PISO_DIV_RATIO_OFF 1
#define PM20_82_24_REG_PISO_CNFG_REG_BIT_PISO_BUS_WIDTH_MSK 0x00000001
#define PM20_82_24_REG_PISO_CNFG_REG_BIT_PISO_BUS_WIDTH_OFF 0

/*-----------------------------------------.
 | Register 0x00000058 TX_CNTRL_OVERRIDE_1 |
 +-----------------------------------------+
 | bit  31:26 R/W  PCBI_TX_DPENI1          |
 | bit  25:20 R/W  PCBI_TX_DPENI2          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTRL_OVERRIDE_1_UNUSED_MASK        0x000fffff
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_1_BIT_PCBI_TX_DPENI1_MSK 0xfc000000
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_1_BIT_PCBI_TX_DPENI1_OFF 26
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_1_BIT_PCBI_TX_DPENI2_MSK 0x03f00000
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_1_BIT_PCBI_TX_DPENI2_OFF 20

/*---------------------------------------------.
 | Register 0x00000068 TX_CNTRL_OVERRIDE_REG_5 |
 +---------------------------------------------+
 | bit  19 R/W  TX_WIZARD_EN                   |
 +--------------------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTRL_OVERRIDE_REG_5_UNUSED_MASK      0xfff7ffff
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_REG_5_BIT_TX_WIZARD_EN_MSK 0x00080000
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_REG_5_BIT_TX_WIZARD_EN_OFF 19

/*----------------------------------.
 | Register 0x0000006c CLK_DIV_FACT |
 +----------------------------------+
 | bit  31:25 R/W  CM_CLK_DIV_FACT  |
 | bit  23:20 R/W  ADC_CLK_DIV_FACT |
 | bit  19:16 R/W  CLK800_DIV_FACT  |
 +---------------------------------*/
#define PMC_PM20_82_24_REG_CLK_DIV_FACT_UNUSED_MASK          0x0100ffff
#define PM20_82_24_REG_CLK_DIV_FACT_BIT_CM_CLK_DIV_FACT_MSK  0xfe000000
#define PM20_82_24_REG_CLK_DIV_FACT_BIT_CM_CLK_DIV_FACT_OFF  25
#define PM20_82_24_REG_CLK_DIV_FACT_BIT_ADC_CLK_DIV_FACT_MSK 0x00f00000
#define PM20_82_24_REG_CLK_DIV_FACT_BIT_ADC_CLK_DIV_FACT_OFF 20
#define PM20_82_24_REG_CLK_DIV_FACT_BIT_CLK800_DIV_FACT_MSK  0x000f0000
#define PM20_82_24_REG_CLK_DIV_FACT_BIT_CLK800_DIV_FACT_OFF  16

/*--------------------------------------.
 | Register 0x00000070 DEGLITCHER_EN    |
 +--------------------------------------+
 | bit  31:15 R/W  PCBI_TX_DENI         |
 | bit  2     R/W  TPCOUT_DEGLITCH_EN   |
 | bit  1     R/W  BADJ_CLK_DEGLITCH_EN |
 | bit  0     R/W  ADC_CLK_DEGLITCH_EN  |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_DEGLITCHER_EN_UNUSED_MASK              0x00007ff8
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_PCBI_TX_DENI_MSK         0xffff8000
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_PCBI_TX_DENI_OFF         15
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_TPCOUT_DEGLITCH_EN_MSK   0x00000004
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_TPCOUT_DEGLITCH_EN_OFF   2
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_BADJ_CLK_DEGLITCH_EN_MSK 0x00000002
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_BADJ_CLK_DEGLITCH_EN_OFF 1
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_ADC_CLK_DEGLITCH_EN_MSK  0x00000001
#define PM20_82_24_REG_DEGLITCHER_EN_BIT_ADC_CLK_DEGLITCH_EN_OFF  0

/*--------------------------------------.
 | Register 0x00000074 DIGITAL_RSTB_REG |
 +--------------------------------------+
 | bit  31 R/W  GLBL_DIGITAL_RSTB       |
 | bit  15 R/W  PCBI_FIFO_HW_RESET      |
 | bit  12 R/W  MON_RSTB                |
 | bit  11 R/W  GEN_RSTB                |
 | bit  10 R/W  PISO_RSTB               |
 | bit  9  R/W  SIPO_RSTB               |
 | bit  8  R/W  DLOS_RSTB               |
 | bit  7  R/W  PLL_LOCK_DET_RSTB       |
 | bit  6  R/W  MDSP_RSTB               |
 | bit  5  R/W  DIAGNOSTICS_RSTB        |
 | bit  4  R/W  SIPO_BIST_RSTB          |
 | bit  3  R/W  GCOC_RSTB               |
 | bit  2  R/W  LPBK_DESKEW_FIFO_RSTB   |
 | bit  1  R/W  DESKEW_FIFO_RSTB        |
 | bit  0  R/W  PMM_WRAP_RSTB           |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_DIGITAL_RSTB_REG_UNUSED_MASK               0x7fff6000
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_GLBL_DIGITAL_RSTB_MSK     0x80000000
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_GLBL_DIGITAL_RSTB_OFF     31
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PCBI_FIFO_HW_RESET_MSK    0x00008000
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PCBI_FIFO_HW_RESET_OFF    15
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_MON_RSTB_MSK              0x00001000
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_MON_RSTB_OFF              12
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_GEN_RSTB_MSK              0x00000800
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_GEN_RSTB_OFF              11
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PISO_RSTB_MSK             0x00000400
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PISO_RSTB_OFF             10
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_SIPO_RSTB_MSK             0x00000200
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_SIPO_RSTB_OFF             9
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_DLOS_RSTB_MSK             0x00000100
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_DLOS_RSTB_OFF             8
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PLL_LOCK_DET_RSTB_MSK     0x00000080
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PLL_LOCK_DET_RSTB_OFF     7
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_MDSP_RSTB_MSK             0x00000040
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_MDSP_RSTB_OFF             6
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_DIAGNOSTICS_RSTB_MSK      0x00000020
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_DIAGNOSTICS_RSTB_OFF      5
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_SIPO_BIST_RSTB_MSK        0x00000010
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_SIPO_BIST_RSTB_OFF        4
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_GCOC_RSTB_MSK             0x00000008
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_GCOC_RSTB_OFF             3
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_LPBK_DESKEW_FIFO_RSTB_MSK 0x00000004
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_LPBK_DESKEW_FIFO_RSTB_OFF 2
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_DESKEW_FIFO_RSTB_MSK      0x00000002
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_DESKEW_FIFO_RSTB_OFF      1
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PMM_WRAP_RSTB_MSK         0x00000001
#define PM20_82_24_REG_DIGITAL_RSTB_REG_BIT_PMM_WRAP_RSTB_OFF         0

/*----------------------------------.
 | Register 0x00000078 IE_REG       |
 +----------------------------------+
 | bit  19 R/W  CALIB_PASS_E19      |
 | bit  18 R/W  PATT_MON_ERR_E18    |
 | bit  17 R/W  ADC_BIST_ERR_E17    |
 | bit  16 R/W  ADC_BIST_DONE_E16   |
 | bit  15 R/W  ADCOC_FSM_ERR_E15   |
 | bit  14 R/W  DLOS_E14            |
 | bit  9  R/W  DIV_CLK_DEAD_E9     |
 | bit  7  R/W  IN_RANGEN_FAILS_E7  |
 | bit  6  R/W  IN_RANGEN_PASSES_E6 |
 | bit  5  R/W  PLL_LOCK_GAIN_E5    |
 | bit  4  R/W  PROG_REF_DONE_E4    |
 | bit  3  R/W  CALIB_DONE_E3       |
 | bit  0  R/W  PLL_LOCK_LOSS_E0    |
 +---------------------------------*/
#define PMC_PM20_82_24_REG_IE_REG_UNUSED_MASK             0xfff00106
#define PM20_82_24_REG_IE_REG_BIT_CALIB_PASS_E19_MSK      0x00080000
#define PM20_82_24_REG_IE_REG_BIT_CALIB_PASS_E19_OFF      19
#define PM20_82_24_REG_IE_REG_BIT_PATT_MON_ERR_E18_MSK    0x00040000
#define PM20_82_24_REG_IE_REG_BIT_PATT_MON_ERR_E18_OFF    18
#define PM20_82_24_REG_IE_REG_BIT_ADC_BIST_ERR_E17_MSK    0x00020000
#define PM20_82_24_REG_IE_REG_BIT_ADC_BIST_ERR_E17_OFF    17
#define PM20_82_24_REG_IE_REG_BIT_ADC_BIST_DONE_E16_MSK   0x00010000
#define PM20_82_24_REG_IE_REG_BIT_ADC_BIST_DONE_E16_OFF   16
#define PM20_82_24_REG_IE_REG_BIT_ADCOC_FSM_ERR_E15_MSK   0x00008000
#define PM20_82_24_REG_IE_REG_BIT_ADCOC_FSM_ERR_E15_OFF   15
#define PM20_82_24_REG_IE_REG_BIT_DLOS_E14_MSK            0x00004000
#define PM20_82_24_REG_IE_REG_BIT_DLOS_E14_OFF            14
#define PM20_82_24_REG_IE_REG_BIT_DIV_CLK_DEAD_E9_MSK     0x00000200
#define PM20_82_24_REG_IE_REG_BIT_DIV_CLK_DEAD_E9_OFF     9
#define PM20_82_24_REG_IE_REG_BIT_IN_RANGEN_FAILS_E7_MSK  0x00000080
#define PM20_82_24_REG_IE_REG_BIT_IN_RANGEN_FAILS_E7_OFF  7
#define PM20_82_24_REG_IE_REG_BIT_IN_RANGEN_PASSES_E6_MSK 0x00000040
#define PM20_82_24_REG_IE_REG_BIT_IN_RANGEN_PASSES_E6_OFF 6
#define PM20_82_24_REG_IE_REG_BIT_PLL_LOCK_GAIN_E5_MSK    0x00000020
#define PM20_82_24_REG_IE_REG_BIT_PLL_LOCK_GAIN_E5_OFF    5
#define PM20_82_24_REG_IE_REG_BIT_PROG_REF_DONE_E4_MSK    0x00000010
#define PM20_82_24_REG_IE_REG_BIT_PROG_REF_DONE_E4_OFF    4
#define PM20_82_24_REG_IE_REG_BIT_CALIB_DONE_E3_MSK       0x00000008
#define PM20_82_24_REG_IE_REG_BIT_CALIB_DONE_E3_OFF       3
#define PM20_82_24_REG_IE_REG_BIT_PLL_LOCK_LOSS_E0_MSK    0x00000001
#define PM20_82_24_REG_IE_REG_BIT_PLL_LOCK_LOSS_E0_OFF    0

/*----------------------------------.
 | Register 0x0000007c INT_REG      |
 +----------------------------------+
 | bit  19 R/W  CALIB_PASS_I19      |
 | bit  18 R/W  PATT_MON_ERR_I18    |
 | bit  17 R/W  ADC_BIST_ERR_I17    |
 | bit  16 R/W  ADC_BIST_DONE_I16   |
 | bit  15 R/W  ADCOC_FSM_ERR_I15   |
 | bit  14 R/W  DLOS_I14            |
 | bit  9  R/W  DIV_CLK_DEAD_I9     |
 | bit  7  R/W  IN_RANGEN_FAILS_I7  |
 | bit  6  R/W  IN_RANGEN_PASSES_I6 |
 | bit  5  R/W  PLL_LOCK_GAIN_I5    |
 | bit  4  R/W  PROG_REF_DONE_I4    |
 | bit  3  R/W  CALIB_DONE_I3       |
 | bit  0  R/W  PLL_LOCK_LOSS_I0    |
 +---------------------------------*/
#define PMC_PM20_82_24_REG_INT_REG_UNUSED_MASK             0xfff00106
#define PM20_82_24_REG_INT_REG_BIT_CALIB_PASS_I19_MSK      0x00080000
#define PM20_82_24_REG_INT_REG_BIT_CALIB_PASS_I19_OFF      19
#define PM20_82_24_REG_INT_REG_BIT_PATT_MON_ERR_I18_MSK    0x00040000
#define PM20_82_24_REG_INT_REG_BIT_PATT_MON_ERR_I18_OFF    18
#define PM20_82_24_REG_INT_REG_BIT_ADC_BIST_ERR_I17_MSK    0x00020000
#define PM20_82_24_REG_INT_REG_BIT_ADC_BIST_ERR_I17_OFF    17
#define PM20_82_24_REG_INT_REG_BIT_ADC_BIST_DONE_I16_MSK   0x00010000
#define PM20_82_24_REG_INT_REG_BIT_ADC_BIST_DONE_I16_OFF   16
#define PM20_82_24_REG_INT_REG_BIT_ADCOC_FSM_ERR_I15_MSK   0x00008000
#define PM20_82_24_REG_INT_REG_BIT_ADCOC_FSM_ERR_I15_OFF   15
#define PM20_82_24_REG_INT_REG_BIT_DLOS_I14_MSK            0x00004000
#define PM20_82_24_REG_INT_REG_BIT_DLOS_I14_OFF            14
#define PM20_82_24_REG_INT_REG_BIT_DIV_CLK_DEAD_I9_MSK     0x00000200
#define PM20_82_24_REG_INT_REG_BIT_DIV_CLK_DEAD_I9_OFF     9
#define PM20_82_24_REG_INT_REG_BIT_IN_RANGEN_FAILS_I7_MSK  0x00000080
#define PM20_82_24_REG_INT_REG_BIT_IN_RANGEN_FAILS_I7_OFF  7
#define PM20_82_24_REG_INT_REG_BIT_IN_RANGEN_PASSES_I6_MSK 0x00000040
#define PM20_82_24_REG_INT_REG_BIT_IN_RANGEN_PASSES_I6_OFF 6
#define PM20_82_24_REG_INT_REG_BIT_PLL_LOCK_GAIN_I5_MSK    0x00000020
#define PM20_82_24_REG_INT_REG_BIT_PLL_LOCK_GAIN_I5_OFF    5
#define PM20_82_24_REG_INT_REG_BIT_PROG_REF_DONE_I4_MSK    0x00000010
#define PM20_82_24_REG_INT_REG_BIT_PROG_REF_DONE_I4_OFF    4
#define PM20_82_24_REG_INT_REG_BIT_CALIB_DONE_I3_MSK       0x00000008
#define PM20_82_24_REG_INT_REG_BIT_CALIB_DONE_I3_OFF       3
#define PM20_82_24_REG_INT_REG_BIT_PLL_LOCK_LOSS_I0_MSK    0x00000001
#define PM20_82_24_REG_INT_REG_BIT_PLL_LOCK_LOSS_I0_OFF    0

/*--------------------------------.
 | Register 0x00000080 IV_REG     |
 +--------------------------------+
 | bit  19 R  CALIB_PASS_V19      |
 | bit  18 R  PATT_MON_ERR_V18    |
 | bit  17 R  ADC_BIST_ERR_V17    |
 | bit  16 R  ADC_BIST_DONE_V16   |
 | bit  15 R  ADCOC_FSM_ERR_V15   |
 | bit  14 R  DLOS_V14            |
 | bit  9  R  DIV_CLK_DEAD_V9     |
 | bit  7  R  IN_RANGEN_FAILS_V7  |
 | bit  6  R  IN_RANGEN_PASSES_V6 |
 | bit  5  R  PLL_LOCK_GAIN_V5    |
 | bit  4  R  PROG_REF_DONE_V4    |
 | bit  3  R  CALIB_DONE_V3       |
 | bit  0  R  PLL_LOCK_LOSS_V0    |
 +-------------------------------*/
#define PMC_PM20_82_24_REG_IV_REG_UNUSED_MASK             0xfff00106
#define PM20_82_24_REG_IV_REG_BIT_CALIB_PASS_V19_MSK      0x00080000
#define PM20_82_24_REG_IV_REG_BIT_CALIB_PASS_V19_OFF      19
#define PM20_82_24_REG_IV_REG_BIT_PATT_MON_ERR_V18_MSK    0x00040000
#define PM20_82_24_REG_IV_REG_BIT_PATT_MON_ERR_V18_OFF    18
#define PM20_82_24_REG_IV_REG_BIT_ADC_BIST_ERR_V17_MSK    0x00020000
#define PM20_82_24_REG_IV_REG_BIT_ADC_BIST_ERR_V17_OFF    17
#define PM20_82_24_REG_IV_REG_BIT_ADC_BIST_DONE_V16_MSK   0x00010000
#define PM20_82_24_REG_IV_REG_BIT_ADC_BIST_DONE_V16_OFF   16
#define PM20_82_24_REG_IV_REG_BIT_ADCOC_FSM_ERR_V15_MSK   0x00008000
#define PM20_82_24_REG_IV_REG_BIT_ADCOC_FSM_ERR_V15_OFF   15
#define PM20_82_24_REG_IV_REG_BIT_DLOS_V14_MSK            0x00004000
#define PM20_82_24_REG_IV_REG_BIT_DLOS_V14_OFF            14
#define PM20_82_24_REG_IV_REG_BIT_DIV_CLK_DEAD_V9_MSK     0x00000200
#define PM20_82_24_REG_IV_REG_BIT_DIV_CLK_DEAD_V9_OFF     9
#define PM20_82_24_REG_IV_REG_BIT_IN_RANGEN_FAILS_V7_MSK  0x00000080
#define PM20_82_24_REG_IV_REG_BIT_IN_RANGEN_FAILS_V7_OFF  7
#define PM20_82_24_REG_IV_REG_BIT_IN_RANGEN_PASSES_V6_MSK 0x00000040
#define PM20_82_24_REG_IV_REG_BIT_IN_RANGEN_PASSES_V6_OFF 6
#define PM20_82_24_REG_IV_REG_BIT_PLL_LOCK_GAIN_V5_MSK    0x00000020
#define PM20_82_24_REG_IV_REG_BIT_PLL_LOCK_GAIN_V5_OFF    5
#define PM20_82_24_REG_IV_REG_BIT_PROG_REF_DONE_V4_MSK    0x00000010
#define PM20_82_24_REG_IV_REG_BIT_PROG_REF_DONE_V4_OFF    4
#define PM20_82_24_REG_IV_REG_BIT_CALIB_DONE_V3_MSK       0x00000008
#define PM20_82_24_REG_IV_REG_BIT_CALIB_DONE_V3_OFF       3
#define PM20_82_24_REG_IV_REG_BIT_PLL_LOCK_LOSS_V0_MSK    0x00000001
#define PM20_82_24_REG_IV_REG_BIT_PLL_LOCK_LOSS_V0_OFF    0

/*------------------------------------------.
 | Register 0x00000084 PRIM_LUT_INDEX_0_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_0          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_0_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_0_REG_BIT_PRIM_LUT_INDEX_0_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_0_REG_BIT_PRIM_LUT_INDEX_0_OFF 0

/*------------------------------------------.
 | Register 0x00000088 PRIM_LUT_INDEX_1_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_1          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_1_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_1_REG_BIT_PRIM_LUT_INDEX_1_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_1_REG_BIT_PRIM_LUT_INDEX_1_OFF 0

/*------------------------------------------.
 | Register 0x0000008c PRIM_LUT_INDEX_2_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_2          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_2_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_2_REG_BIT_PRIM_LUT_INDEX_2_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_2_REG_BIT_PRIM_LUT_INDEX_2_OFF 0

/*------------------------------------------.
 | Register 0x00000090 PRIM_LUT_INDEX_3_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_3          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_3_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_3_REG_BIT_PRIM_LUT_INDEX_3_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_3_REG_BIT_PRIM_LUT_INDEX_3_OFF 0

/*------------------------------------------.
 | Register 0x00000094 PRIM_LUT_INDEX_4_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_4          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_4_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_4_REG_BIT_PRIM_LUT_INDEX_4_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_4_REG_BIT_PRIM_LUT_INDEX_4_OFF 0

/*------------------------------------------.
 | Register 0x00000098 PRIM_LUT_INDEX_5_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_5          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_5_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_5_REG_BIT_PRIM_LUT_INDEX_5_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_5_REG_BIT_PRIM_LUT_INDEX_5_OFF 0

/*------------------------------------------.
 | Register 0x0000009c PRIM_LUT_INDEX_6_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_6          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_6_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_6_REG_BIT_PRIM_LUT_INDEX_6_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_6_REG_BIT_PRIM_LUT_INDEX_6_OFF 0

/*--------------------------------------.
 | Register 0x000000a0 TX_CTRL_STS_REG1 |
 +--------------------------------------+
 | bit  15:0 R  TX_SUM_OUT              |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_TX_CTRL_STS_REG1_UNUSED_MASK    0xffff0000
#define PM20_82_24_REG_TX_CTRL_STS_REG1_BIT_TX_SUM_OUT_MSK 0x0000ffff
#define PM20_82_24_REG_TX_CTRL_STS_REG1_BIT_TX_SUM_OUT_OFF 0

/*------------------------------------------.
 | Register 0x00000100 PRIM_LUT_INDEX_7_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_7          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_7_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_7_REG_BIT_PRIM_LUT_INDEX_7_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_7_REG_BIT_PRIM_LUT_INDEX_7_OFF 0

/*------------------------------------------.
 | Register 0x00000104 PRIM_LUT_INDEX_8_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_8          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_8_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_8_REG_BIT_PRIM_LUT_INDEX_8_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_8_REG_BIT_PRIM_LUT_INDEX_8_OFF 0

/*------------------------------------------.
 | Register 0x00000108 PRIM_LUT_INDEX_9_REG |
 +------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_9          |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_9_REG_UNUSED_MASK          0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_9_REG_BIT_PRIM_LUT_INDEX_9_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_9_REG_BIT_PRIM_LUT_INDEX_9_OFF 0

/*-------------------------------------------.
 | Register 0x0000010c PRIM_LUT_INDEX_10_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_10          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_10_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_10_REG_BIT_PRIM_LUT_INDEX_10_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_10_REG_BIT_PRIM_LUT_INDEX_10_OFF 0

/*-------------------------------------------.
 | Register 0x00000110 PRIM_LUT_INDEX_11_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_11          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_11_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_11_REG_BIT_PRIM_LUT_INDEX_11_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_11_REG_BIT_PRIM_LUT_INDEX_11_OFF 0

/*-------------------------------------------.
 | Register 0x00000114 PRIM_LUT_INDEX_12_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_12          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_12_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_12_REG_BIT_PRIM_LUT_INDEX_12_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_12_REG_BIT_PRIM_LUT_INDEX_12_OFF 0

/*-------------------------------------------.
 | Register 0x00000118 PRIM_LUT_INDEX_13_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_13          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_13_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_13_REG_BIT_PRIM_LUT_INDEX_13_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_13_REG_BIT_PRIM_LUT_INDEX_13_OFF 0

/*-------------------------------------------.
 | Register 0x0000011c PRIM_LUT_INDEX_14_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_14          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_14_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_14_REG_BIT_PRIM_LUT_INDEX_14_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_14_REG_BIT_PRIM_LUT_INDEX_14_OFF 0

/*-------------------------------------------.
 | Register 0x00000120 PRIM_LUT_INDEX_15_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_15          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_15_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_15_REG_BIT_PRIM_LUT_INDEX_15_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_15_REG_BIT_PRIM_LUT_INDEX_15_OFF 0

/*-------------------------------------------.
 | Register 0x00000124 PRIM_LUT_INDEX_16_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_16          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_16_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_16_REG_BIT_PRIM_LUT_INDEX_16_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_16_REG_BIT_PRIM_LUT_INDEX_16_OFF 0

/*-------------------------------------------.
 | Register 0x00000128 PRIM_LUT_INDEX_17_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_17          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_17_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_17_REG_BIT_PRIM_LUT_INDEX_17_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_17_REG_BIT_PRIM_LUT_INDEX_17_OFF 0

/*-------------------------------------------.
 | Register 0x0000012c PRIM_LUT_INDEX_18_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_18          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_18_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_18_REG_BIT_PRIM_LUT_INDEX_18_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_18_REG_BIT_PRIM_LUT_INDEX_18_OFF 0

/*-------------------------------------------.
 | Register 0x00000130 PRIM_LUT_INDEX_19_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_19          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_19_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_19_REG_BIT_PRIM_LUT_INDEX_19_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_19_REG_BIT_PRIM_LUT_INDEX_19_OFF 0

/*-------------------------------------------.
 | Register 0x00000134 PRIM_LUT_INDEX_20_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_20          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_20_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_20_REG_BIT_PRIM_LUT_INDEX_20_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_20_REG_BIT_PRIM_LUT_INDEX_20_OFF 0

/*-------------------------------------------.
 | Register 0x00000138 PRIM_LUT_INDEX_21_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_21          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_21_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_21_REG_BIT_PRIM_LUT_INDEX_21_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_21_REG_BIT_PRIM_LUT_INDEX_21_OFF 0

/*-------------------------------------------.
 | Register 0x0000013c PRIM_LUT_INDEX_22_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_22          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_22_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_22_REG_BIT_PRIM_LUT_INDEX_22_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_22_REG_BIT_PRIM_LUT_INDEX_22_OFF 0

/*-------------------------------------------.
 | Register 0x00000140 PRIM_LUT_INDEX_23_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_23          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_23_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_23_REG_BIT_PRIM_LUT_INDEX_23_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_23_REG_BIT_PRIM_LUT_INDEX_23_OFF 0

/*-------------------------------------------.
 | Register 0x00000144 PRIM_LUT_INDEX_24_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_24          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_24_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_24_REG_BIT_PRIM_LUT_INDEX_24_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_24_REG_BIT_PRIM_LUT_INDEX_24_OFF 0

/*-------------------------------------------.
 | Register 0x00000148 PRIM_LUT_INDEX_25_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_25          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_25_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_25_REG_BIT_PRIM_LUT_INDEX_25_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_25_REG_BIT_PRIM_LUT_INDEX_25_OFF 0

/*-------------------------------------------.
 | Register 0x0000014c PRIM_LUT_INDEX_26_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_26          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_26_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_26_REG_BIT_PRIM_LUT_INDEX_26_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_26_REG_BIT_PRIM_LUT_INDEX_26_OFF 0

/*-------------------------------------------.
 | Register 0x00000150 PRIM_LUT_INDEX_27_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_27          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_27_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_27_REG_BIT_PRIM_LUT_INDEX_27_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_27_REG_BIT_PRIM_LUT_INDEX_27_OFF 0

/*-------------------------------------------.
 | Register 0x00000154 PRIM_LUT_INDEX_28_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_28          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_28_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_28_REG_BIT_PRIM_LUT_INDEX_28_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_28_REG_BIT_PRIM_LUT_INDEX_28_OFF 0

/*-------------------------------------------.
 | Register 0x00000158 PRIM_LUT_INDEX_29_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_29          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_29_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_29_REG_BIT_PRIM_LUT_INDEX_29_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_29_REG_BIT_PRIM_LUT_INDEX_29_OFF 0

/*-------------------------------------------.
 | Register 0x0000015c PRIM_LUT_INDEX_30_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_30          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_30_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_30_REG_BIT_PRIM_LUT_INDEX_30_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_30_REG_BIT_PRIM_LUT_INDEX_30_OFF 0

/*-------------------------------------------.
 | Register 0x00000160 PRIM_LUT_INDEX_31_REG |
 +-------------------------------------------+
 | bit  15:0 R/W  PRIM_LUT_INDEX_31          |
 +------------------------------------------*/
#define PMC_PM20_82_24_REG_PRIM_LUT_INDEX_31_REG_UNUSED_MASK           0xffff0000
#define PM20_82_24_REG_PRIM_LUT_INDEX_31_REG_BIT_PRIM_LUT_INDEX_31_MSK 0x0000ffff
#define PM20_82_24_REG_PRIM_LUT_INDEX_31_REG_BIT_PRIM_LUT_INDEX_31_OFF 0

/*-----------------------------------------.
 | Register 0x00000164 ALT_LUT_INDEX_0_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_0          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_0_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_0_REG_BIT_ALT_LUT_INDEX_0_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_0_REG_BIT_ALT_LUT_INDEX_0_OFF 0

/*-----------------------------------------.
 | Register 0x00000168 ALT_LUT_INDEX_1_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_1          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_1_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_1_REG_BIT_ALT_LUT_INDEX_1_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_1_REG_BIT_ALT_LUT_INDEX_1_OFF 0

/*-----------------------------------------.
 | Register 0x0000016c ALT_LUT_INDEX_2_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_2          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_2_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_2_REG_BIT_ALT_LUT_INDEX_2_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_2_REG_BIT_ALT_LUT_INDEX_2_OFF 0

/*-----------------------------------------.
 | Register 0x00000170 ALT_LUT_INDEX_3_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_3          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_3_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_3_REG_BIT_ALT_LUT_INDEX_3_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_3_REG_BIT_ALT_LUT_INDEX_3_OFF 0

/*-----------------------------------------.
 | Register 0x00000174 ALT_LUT_INDEX_4_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_4          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_4_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_4_REG_BIT_ALT_LUT_INDEX_4_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_4_REG_BIT_ALT_LUT_INDEX_4_OFF 0

/*-----------------------------------------.
 | Register 0x00000178 ALT_LUT_INDEX_5_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_5          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_5_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_5_REG_BIT_ALT_LUT_INDEX_5_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_5_REG_BIT_ALT_LUT_INDEX_5_OFF 0

/*-----------------------------------------.
 | Register 0x0000017c ALT_LUT_INDEX_6_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_6          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_6_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_6_REG_BIT_ALT_LUT_INDEX_6_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_6_REG_BIT_ALT_LUT_INDEX_6_OFF 0

/*-----------------------------------------.
 | Register 0x00000180 ALT_LUT_INDEX_7_REG |
 +-----------------------------------------+
 | bit  15:0 R/W  ALT_LUT_INDEX_7          |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_ALT_LUT_INDEX_7_REG_UNUSED_MASK         0xffff0000
#define PM20_82_24_REG_ALT_LUT_INDEX_7_REG_BIT_ALT_LUT_INDEX_7_MSK 0x0000ffff
#define PM20_82_24_REG_ALT_LUT_INDEX_7_REG_BIT_ALT_LUT_INDEX_7_OFF 0

/*--------------------------------------.
 | Register 0x00000184 PGA_LUT_CTRL_REG |
 +--------------------------------------+
 | bit  24    R/W  PLEN                 |
 | bit  22:20 R/W  CG_S2_IN             |
 | bit  18:16 R/W  CG_S1_IN             |
 | bit  15:12 R/W  RG_S2_IN             |
 | bit  11:8  R/W  RG_S1_IN             |
 | bit  1:0   R/W  ATTENUATE_EN_IN      |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_PGA_LUT_CTRL_REG_UNUSED_MASK         0xfe8800fc
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_PLEN_MSK            0x01000000
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_PLEN_OFF            24
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_CG_S2_IN_MSK        0x00700000
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_CG_S2_IN_OFF        20
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_CG_S1_IN_MSK        0x00070000
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_CG_S1_IN_OFF        16
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_RG_S2_IN_MSK        0x0000f000
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_RG_S2_IN_OFF        12
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_RG_S1_IN_MSK        0x00000f00
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_RG_S1_IN_OFF        8
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_ATTENUATE_EN_IN_MSK 0x00000003
#define PM20_82_24_REG_PGA_LUT_CTRL_REG_BIT_ATTENUATE_EN_IN_OFF 0

/*-----------------------------------.
 | Register 0x00000188 PLL_CNFG_REG2 |
 +-----------------------------------+
 | bit  19:0 R/W  CLK_CNT_MAX        |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_PLL_CNFG_REG2_UNUSED_MASK     0xfff00000
#define PM20_82_24_REG_PLL_CNFG_REG2_BIT_CLK_CNT_MAX_MSK 0x000fffff
#define PM20_82_24_REG_PLL_CNFG_REG2_BIT_CLK_CNT_MAX_OFF 0

/*-----------------------------------.
 | Register 0x0000018c PLL_CNFG_REG4 |
 +-----------------------------------+
 | bit  31:16 R/W  FAIL_CNT_MAX      |
 | bit  15:0  R/W  PASS_CNT_MAX      |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_PLL_CNFG_REG4_UNUSED_MASK      0x00000000
#define PM20_82_24_REG_PLL_CNFG_REG4_BIT_FAIL_CNT_MAX_MSK 0xffff0000
#define PM20_82_24_REG_PLL_CNFG_REG4_BIT_FAIL_CNT_MAX_OFF 16
#define PM20_82_24_REG_PLL_CNFG_REG4_BIT_PASS_CNT_MAX_MSK 0x0000ffff
#define PM20_82_24_REG_PLL_CNFG_REG4_BIT_PASS_CNT_MAX_OFF 0

/*-----------------------------------.
 | Register 0x00000190 PLL_CNFG_REG6 |
 +-----------------------------------+
 | bit  7:0 R/W  LOCK_CNT_MAX        |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_PLL_CNFG_REG6_UNUSED_MASK      0xffffff00
#define PM20_82_24_REG_PLL_CNFG_REG6_BIT_LOCK_CNT_MAX_MSK 0x000000ff
#define PM20_82_24_REG_PLL_CNFG_REG6_BIT_LOCK_CNT_MAX_OFF 0

/*---------------------------------.
 | Register 0x00000194 PLL_STS_REG |
 +---------------------------------+
 | bit  31   R  PPM_OFF_RD_SIGN    |
 | bit  20:0 R  PPM_OFF_RD_VAL     |
 +--------------------------------*/
#define PMC_PM20_82_24_REG_PLL_STS_REG_UNUSED_MASK         0x7fe00000
#define PM20_82_24_REG_PLL_STS_REG_BIT_PPM_OFF_RD_SIGN_MSK 0x80000000
#define PM20_82_24_REG_PLL_STS_REG_BIT_PPM_OFF_RD_SIGN_OFF 31
#define PM20_82_24_REG_PLL_STS_REG_BIT_PPM_OFF_RD_VAL_MSK  0x001fffff
#define PM20_82_24_REG_PLL_STS_REG_BIT_PPM_OFF_RD_VAL_OFF  0

/*-----------------------------------.
 | Register 0x00000198 MDSP_CFG_REG1 |
 +-----------------------------------+
 | bit  31  R/W  ADD_DROP_HOLD       |
 | bit  4   R/W  TED_HOLD_OUTPUT_EN  |
 | bit  3   R/W  DFE_BYPASS          |
 | bit  2:1 R/W  TED_MODE            |
 | bit  0   R/W  DLB_ENB             |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_MDSP_CFG_REG1_UNUSED_MASK            0x7fffffe0
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_ADD_DROP_HOLD_MSK      0x80000000
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_ADD_DROP_HOLD_OFF      31
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_TED_HOLD_OUTPUT_EN_MSK 0x00000010
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_TED_HOLD_OUTPUT_EN_OFF 4
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_DFE_BYPASS_MSK         0x00000008
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_DFE_BYPASS_OFF         3
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_TED_MODE_MSK           0x00000006
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_TED_MODE_OFF           1
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_DLB_ENB_MSK            0x00000001
#define PM20_82_24_REG_MDSP_CFG_REG1_BIT_DLB_ENB_OFF            0

/*-----------------------------------.
 | Register 0x0000019c MDSP_CFG_REG2 |
 +-----------------------------------+
 | bit  15:8 R/W  LF_CLAMP_MIN       |
 | bit  7:0  R/W  LF_CLAMP_MAX       |
 +----------------------------------*/
#define PMC_PM20_82_24_REG_MDSP_CFG_REG2_UNUSED_MASK      0xffff0000
#define PM20_82_24_REG_MDSP_CFG_REG2_BIT_LF_CLAMP_MIN_MSK 0x0000ff00
#define PM20_82_24_REG_MDSP_CFG_REG2_BIT_LF_CLAMP_MIN_OFF 8
#define PM20_82_24_REG_MDSP_CFG_REG2_BIT_LF_CLAMP_MAX_MSK 0x000000ff
#define PM20_82_24_REG_MDSP_CFG_REG2_BIT_LF_CLAMP_MAX_OFF 0

/*-------------------------------------.
 | Register 0x00000204 PGA_CFG_REG     |
 +-------------------------------------+
 | bit  7 R/W  PGA_ATMSB               |
 | bit  6 R/W  PGA_CMFILT_ENB          |
 | bit  1 R/W  PGA_ATTENUATE_FORCE_OFF |
 +------------------------------------*/
#define PMC_PM20_82_24_REG_PGA_CFG_REG_UNUSED_MASK                 0xffffff3d
#define PM20_82_24_REG_PGA_CFG_REG_BIT_PGA_ATMSB_MSK               0x00000080
#define PM20_82_24_REG_PGA_CFG_REG_BIT_PGA_ATMSB_OFF               7
#define PM20_82_24_REG_PGA_CFG_REG_BIT_PGA_CMFILT_ENB_MSK          0x00000040
#define PM20_82_24_REG_PGA_CFG_REG_BIT_PGA_CMFILT_ENB_OFF          6
#define PM20_82_24_REG_PGA_CFG_REG_BIT_PGA_ATTENUATE_FORCE_OFF_MSK 0x00000002
#define PM20_82_24_REG_PGA_CFG_REG_BIT_PGA_ATTENUATE_FORCE_OFF_OFF 1

/*----------------------------------------------.
 | Register 0x00000208 DIAG_CNFG_REG1           |
 +----------------------------------------------+
 | bit  31:24 R/W  VHIST_CNT_TIM_LIMIT_39_32    |
 | bit  3     R/W  VHIST_TITAN_TARGET_AMP_WR_EN |
 | bit  2     R/W  VHIST_PERBIN_VALUE_RD_ENB    |
 | bit  1     R/W  VHIST_CAPT_START             |
 | bit  0     R/W  VHIST_CAPT_ENB               |
 +---------------------------------------------*/
#define PMC_PM20_82_24_REG_DIAG_CNFG_REG1_UNUSED_MASK                      0x00fffff0
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_CNT_TIM_LIMIT_39_32_MSK    0xff000000
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_CNT_TIM_LIMIT_39_32_OFF    24
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_TITAN_TARGET_AMP_WR_EN_MSK 0x00000008
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_TITAN_TARGET_AMP_WR_EN_OFF 3
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_PERBIN_VALUE_RD_ENB_MSK    0x00000004
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_PERBIN_VALUE_RD_ENB_OFF    2
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_CAPT_START_MSK             0x00000002
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_CAPT_START_OFF             1
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_CAPT_ENB_MSK               0x00000001
#define PM20_82_24_REG_DIAG_CNFG_REG1_BIT_VHIST_CAPT_ENB_OFF               0

/*-----------------------------------------.
 | Register 0x0000020c DIAG_CNFG_REG2      |
 +-----------------------------------------+
 | bit  31:0 R/W  VHIST_CNT_TIM_LIMIT_31_0 |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_DIAG_CNFG_REG2_UNUSED_MASK                  0x00000000
#define PM20_82_24_REG_DIAG_CNFG_REG2_BIT_VHIST_CNT_TIM_LIMIT_31_0_MSK 0xffffffff
#define PM20_82_24_REG_DIAG_CNFG_REG2_BIT_VHIST_CNT_TIM_LIMIT_31_0_OFF 0

/*----------------------------------------.
 | Register 0x00000210 DIAG_STATUS_REG1   |
 +----------------------------------------+
 | bit  31:24 R  VHIST_PERBIN_VALUE_39_32 |
 | bit  15:10 R  VHIST_READBACK_ADDR      |
 | bit  7:5   R  VHIST_FSM_STATE          |
 | bit  1     R  VHIST_PERBIN_VALUE_OVFL  |
 | bit  0     R  VHIST_CAPT_RUNNING       |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_DIAG_STATUS_REG1_UNUSED_MASK                  0x00ff031c
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_PERBIN_VALUE_39_32_MSK 0xff000000
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_PERBIN_VALUE_39_32_OFF 24
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_READBACK_ADDR_MSK      0x0000fc00
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_READBACK_ADDR_OFF      10
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_FSM_STATE_MSK          0x000000e0
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_FSM_STATE_OFF          5
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_PERBIN_VALUE_OVFL_MSK  0x00000002
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_PERBIN_VALUE_OVFL_OFF  1
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_CAPT_RUNNING_MSK       0x00000001
#define PM20_82_24_REG_DIAG_STATUS_REG1_BIT_VHIST_CAPT_RUNNING_OFF       0

/*--------------------------------------.
 | Register 0x00000214 DIAG_STATUS_REG2 |
 +--------------------------------------+
 | bit  31:0 R  VHIST_PERBIN_VALUE_31_0 |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_DIAG_STATUS_REG2_UNUSED_MASK                 0x00000000
#define PM20_82_24_REG_DIAG_STATUS_REG2_BIT_VHIST_PERBIN_VALUE_31_0_MSK 0xffffffff
#define PM20_82_24_REG_DIAG_STATUS_REG2_BIT_VHIST_PERBIN_VALUE_31_0_OFF 0

/*-----------------------------------------.
 | Register 0x00000218 OFFSET_RAM_CNFG_REG |
 +-----------------------------------------+
 | bit  1:0 R/W  OFFSET_RAM_QUADRANT_SEL   |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_OFFSET_RAM_CNFG_REG_UNUSED_MASK                 0xfffffffc
#define PM20_82_24_REG_OFFSET_RAM_CNFG_REG_BIT_OFFSET_RAM_QUADRANT_SEL_MSK 0x00000003
#define PM20_82_24_REG_OFFSET_RAM_CNFG_REG_BIT_OFFSET_RAM_QUADRANT_SEL_OFF 0

/*----------------------------------------.
 | Register 0x0000021c DIG_LPBK_CTRL_REG  |
 +----------------------------------------+
 | bit  31:29 R/W  RD_PTR_SYNC_VAL        |
 | bit  28:26 R/W  DESKEW_RD_PTR_SYNC_VAL |
 | bit  2     R/W  DESKEW_FIFO_SYNC       |
 | bit  1     R/W  PLL_LOCK_DET_SW_RST    |
 | bit  0     R/W  DIG_LPBK_SYNC          |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_DIG_LPBK_CTRL_REG_UNUSED_MASK                0x03fffff8
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_RD_PTR_SYNC_VAL_MSK        0xe0000000
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_RD_PTR_SYNC_VAL_OFF        29
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_DESKEW_RD_PTR_SYNC_VAL_MSK 0x1c000000
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_DESKEW_RD_PTR_SYNC_VAL_OFF 26
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_DESKEW_FIFO_SYNC_MSK       0x00000004
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_DESKEW_FIFO_SYNC_OFF       2
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_PLL_LOCK_DET_SW_RST_MSK    0x00000002
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_PLL_LOCK_DET_SW_RST_OFF    1
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_DIG_LPBK_SYNC_MSK          0x00000001
#define PM20_82_24_REG_DIG_LPBK_CTRL_REG_BIT_DIG_LPBK_SYNC_OFF          0

/*--------------------------------------.
 | Register 0x00000220 ADC_OC_REGISTER4 |
 +--------------------------------------+
 | bit  31:0 R/W  THR_MAP63_32          |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_ADC_OC_REGISTER4_UNUSED_MASK      0x00000000
#define PM20_82_24_REG_ADC_OC_REGISTER4_BIT_THR_MAP63_32_MSK 0xffffffff
#define PM20_82_24_REG_ADC_OC_REGISTER4_BIT_THR_MAP63_32_OFF 0

/*--------------------------------------.
 | Register 0x00000224 ADC_OC_REGISTER5 |
 +--------------------------------------+
 | bit  31:24 R/W  MAX_THRESHOLD        |
 | bit  23:16 R/W  MIN_THRESHOLD        |
 | bit  15:0  R/W  THR_MAP79_64         |
 +-------------------------------------*/
#define PMC_PM20_82_24_REG_ADC_OC_REGISTER5_UNUSED_MASK       0x00000000
#define PM20_82_24_REG_ADC_OC_REGISTER5_BIT_MAX_THRESHOLD_MSK 0xff000000
#define PM20_82_24_REG_ADC_OC_REGISTER5_BIT_MAX_THRESHOLD_OFF 24
#define PM20_82_24_REG_ADC_OC_REGISTER5_BIT_MIN_THRESHOLD_MSK 0x00ff0000
#define PM20_82_24_REG_ADC_OC_REGISTER5_BIT_MIN_THRESHOLD_OFF 16
#define PM20_82_24_REG_ADC_OC_REGISTER5_BIT_THR_MAP79_64_MSK  0x0000ffff
#define PM20_82_24_REG_ADC_OC_REGISTER5_BIT_THR_MAP79_64_OFF  0

/*---------------------------------------.
 | Register 0x00000228 GCOC_MABC_CFG_REG |
 +---------------------------------------+
 | bit  7:4 R/W  OFFSET                  |
 +--------------------------------------*/
#define PMC_PM20_82_24_REG_GCOC_MABC_CFG_REG_UNUSED_MASK 0xffffff0f
#define PM20_82_24_REG_GCOC_MABC_CFG_REG_BIT_OFFSET_MSK  0x000000f0
#define PM20_82_24_REG_GCOC_MABC_CFG_REG_BIT_OFFSET_OFF  4

/*------------------------------.
 | Register 0x00000230 DLOS_CFG |
 +------------------------------+
 | bit  18    R/W  DLOS_OVR_EN  |
 | bit  17    R/W  DLOS_OVR     |
 | bit  16:13 R/W  LOS_ACC_CNT  |
 | bit  12:5  R/W  LOS_LIMIT    |
 | bit  0     R/W  ONE_COMP_EN  |
 +-----------------------------*/
#define PMC_PM20_82_24_REG_DLOS_CFG_UNUSED_MASK     0xfff8001e
#define PM20_82_24_REG_DLOS_CFG_BIT_DLOS_OVR_EN_MSK 0x00040000
#define PM20_82_24_REG_DLOS_CFG_BIT_DLOS_OVR_EN_OFF 18
#define PM20_82_24_REG_DLOS_CFG_BIT_DLOS_OVR_MSK    0x00020000
#define PM20_82_24_REG_DLOS_CFG_BIT_DLOS_OVR_OFF    17
#define PM20_82_24_REG_DLOS_CFG_BIT_LOS_ACC_CNT_MSK 0x0001e000
#define PM20_82_24_REG_DLOS_CFG_BIT_LOS_ACC_CNT_OFF 13
#define PM20_82_24_REG_DLOS_CFG_BIT_LOS_LIMIT_MSK   0x00001fe0
#define PM20_82_24_REG_DLOS_CFG_BIT_LOS_LIMIT_OFF   5
#define PM20_82_24_REG_DLOS_CFG_BIT_ONE_COMP_EN_MSK 0x00000001
#define PM20_82_24_REG_DLOS_CFG_BIT_ONE_COMP_EN_OFF 0

/*----------------------------------------.
 | Register 0x00000234 PATT_GEN_CNFG_REG1 |
 +----------------------------------------+
 | bit  31:24 R/W  GEN_USR_PAT_39_32      |
 | bit  14:9  R/W  PATT_BUS_WIDTH         |
 | bit  8     R/W  GEN_ERR_INJ            |
 | bit  7     R/W  GEN_INV                |
 | bit  6:3   R/W  GEN_MODE               |
 | bit  2     R/W  INV_TX_DATA            |
 | bit  1     R/W  PATT_GEN_SEL           |
 | bit  0     R/W  GEN_EN                 |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_PATT_GEN_CNFG_REG1_UNUSED_MASK           0x00ff8000
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_USR_PAT_39_32_MSK 0xff000000
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_USR_PAT_39_32_OFF 24
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_PATT_BUS_WIDTH_MSK    0x00007e00
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_PATT_BUS_WIDTH_OFF    9
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_ERR_INJ_MSK       0x00000100
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_ERR_INJ_OFF       8
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_INV_MSK           0x00000080
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_INV_OFF           7
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_MODE_MSK          0x00000078
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_MODE_OFF          3
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_INV_TX_DATA_MSK       0x00000004
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_INV_TX_DATA_OFF       2
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_PATT_GEN_SEL_MSK      0x00000002
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_PATT_GEN_SEL_OFF      1
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_EN_MSK            0x00000001
#define PM20_82_24_REG_PATT_GEN_CNFG_REG1_BIT_GEN_EN_OFF            0

/*----------------------------------------.
 | Register 0x00000238 PATT_GEN_CNFG_REG2 |
 +----------------------------------------+
 | bit  31:0 R/W  GEN_USR_PAT_31_0        |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_PATT_GEN_CNFG_REG2_UNUSED_MASK          0x00000000
#define PM20_82_24_REG_PATT_GEN_CNFG_REG2_BIT_GEN_USR_PAT_31_0_MSK 0xffffffff
#define PM20_82_24_REG_PATT_GEN_CNFG_REG2_BIT_GEN_USR_PAT_31_0_OFF 0

/*----------------------------------------.
 | Register 0x0000023c PATT_CHK_CNFG_REG1 |
 +----------------------------------------+
 | bit  31:24 R/W  CHK_USR_PAT_39_32      |
 | bit  8     R/W  CHK_READ               |
 | bit  7     R/W  CHK_INV                |
 | bit  6:3   R/W  CHK_MODE               |
 | bit  0     R/W  CHK_EN                 |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_PATT_CHK_CNFG_REG1_UNUSED_MASK           0x00fffe06
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_USR_PAT_39_32_MSK 0xff000000
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_USR_PAT_39_32_OFF 24
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_READ_MSK          0x00000100
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_READ_OFF          8
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_INV_MSK           0x00000080
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_INV_OFF           7
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_MODE_MSK          0x00000078
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_MODE_OFF          3
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_EN_MSK            0x00000001
#define PM20_82_24_REG_PATT_CHK_CNFG_REG1_BIT_CHK_EN_OFF            0

/*----------------------------------------.
 | Register 0x00000240 PATT_CHK_CNFG_REG2 |
 +----------------------------------------+
 | bit  31:0 R/W  CHK_USR_PAT_31_0        |
 +---------------------------------------*/
#define PMC_PM20_82_24_REG_PATT_CHK_CNFG_REG2_UNUSED_MASK          0x00000000
#define PM20_82_24_REG_PATT_CHK_CNFG_REG2_BIT_CHK_USR_PAT_31_0_MSK 0xffffffff
#define PM20_82_24_REG_PATT_CHK_CNFG_REG2_BIT_CHK_USR_PAT_31_0_OFF 0

/*------------------------------------------.
 | Register 0x00000244 PATT_CHK_STATUS_REG1 |
 +------------------------------------------+
 | bit  31:0 R  CHK_ERR_CNT                 |
 +-----------------------------------------*/
#define PMC_PM20_82_24_REG_PATT_CHK_STATUS_REG1_UNUSED_MASK     0x00000000
#define PM20_82_24_REG_PATT_CHK_STATUS_REG1_BIT_CHK_ERR_CNT_MSK 0xffffffff
#define PM20_82_24_REG_PATT_CHK_STATUS_REG1_BIT_CHK_ERR_CNT_OFF 0

/*-----------------------------------------.
 | Register 0x00000248 TX_CNTRL_OVERRIDE_5 |
 +-----------------------------------------+
 | bit  16:0 R/W  PCBI_TX_PSELI            |
 +----------------------------------------*/
#define PMC_PM20_82_24_REG_TX_CNTRL_OVERRIDE_5_UNUSED_MASK       0xfffe0000
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_5_BIT_PCBI_TX_PSELI_MSK 0x0001ffff
#define PM20_82_24_REG_TX_CNTRL_OVERRIDE_5_BIT_PCBI_TX_PSELI_OFF 0

/* index definitions for PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0 */
#define PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_INDEX_N_MIN                         0
#define PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_INDEX_N_MAX                         95
#define PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_INDEX_N_SIZE                        96
#define PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_INDEX_N_OFFSET                      0x4

/*--------------------------------------------------------------.
 | Register (0x0000024c + (N) * 0x4) PM20_82_24_OFFSET_RAM_REG0 |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  PM20_82_24_MTSB0_OFFSET_RAM_REG               |
 +-------------------------------------------------------------*/
#define PMC_PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_UNUSED_MASK                         0x00000000
#define PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_BIT_PM20_82_24_MTSB0_OFFSET_RAM_REG_MSK 0xffffffff
#define PM20_82_24_REG_PM20_82_24_OFFSET_RAM_REG0_BIT_PM20_82_24_MTSB0_OFFSET_RAM_REG_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _T8_REGS_H */
