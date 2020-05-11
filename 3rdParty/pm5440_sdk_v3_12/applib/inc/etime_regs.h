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
 *     etime block
 *****************************************************************************/
#ifndef _ETIME_REGS_H
#define _ETIME_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ETIME120_REG_REFLLRR                     0x00000000
#define PMC_ETIME120_REG_INT_BEHAV_CTRL              0x00000004
#define PMC_ETIME120_REG_RAM_BIST_CTRL               0x00000008
#define PMC_ETIME120_REG_SB_ECC_ERR_INT_REG          0x0000000c
#define PMC_ETIME120_REG_MB_ECC_ERR_INT_REG          0x00000010
#define PMC_ETIME120_REG_REFL_OFLOW_ERR_INT_REG      0x00000014
#define PMC_ETIME120_REG_TS_DIS_INT_REG              0x00000018
#define PMC_ETIME120_REG_PACKET_DIS_INT_REG          0x0000001c
#define PMC_ETIME120_REG_PACKET_RAM_ERR_INT_REG      0x00000020
#define PMC_ETIME120_REG_EXT_DESYNC_ERR_INT_REG      0x00000024
#define PMC_ETIME120_REG_GP_INT_REG                  0x00000028
#define PMC_ETIME120_REG_SB_ECC_ERR_INT_EN_REG       0x0000002c
#define PMC_ETIME120_REG_MB_ECC_ERR_INT_EN_REG       0x00000030
#define PMC_ETIME120_REG_REFL_OFLOW_ERR_INT_EN_REG   0x00000034
#define PMC_ETIME120_REG_TS_DIS_INT_EN_REG           0x00000038
#define PMC_ETIME120_REG_PACKET_DIS_INT_EN_REG       0x0000003c
#define PMC_ETIME120_REG_PACKET_RAM_ERR_INT_EN_REG   0x00000040
#define PMC_ETIME120_REG_EXT_DESYNC_ERR_INT_EN_REG   0x00000044
#define PMC_ETIME120_REG_GP_INT_EN_REG               0x00000048
#define PMC_ETIME120_REG_SB_ECC_ERR_INT_VAL_REG      0x0000004c
#define PMC_ETIME120_REG_MB_ECC_ERR_INT_VAL_REG      0x00000050
#define PMC_ETIME120_REG_REFL_OFLOW_ERR_INT_VAL_REG  0x00000054
#define PMC_ETIME120_REG_TS_DIS_INT_VAL_REG          0x00000058
#define PMC_ETIME120_REG_PACKET_DIS_INT_VAL_REG      0x0000005c
#define PMC_ETIME120_REG_PACKET_RAM_ERR_INT_VAL_REG  0x00000060
#define PMC_ETIME120_REG_EXT_DESYNC_ERR_INT_VAL_REG  0x00000064
#define PMC_ETIME120_REG_GP_INT_VAL_REG              0x00000068
#define PMC_ETIME120_REG_ING_47B_M                   0x0000006c
#define PMC_ETIME120_REG_EGR_47B_M                   0x00000070
#define PMC_ETIME120_REG_ING_TS_ENA_REG              0x00000074
#define PMC_ETIME120_REG_REFL_DEST                   0x00000078
#define PMC_ETIME120_REG_EGR_TS_ENA_REG              0x0000007c
#define PMC_ETIME120_REG_TOD_OFFS_SEC_HI             0x00000080
#define PMC_ETIME120_REG_TOD_OFFS_SEC_LO             0x00000084
#define PMC_ETIME120_REG_TOD_OFFS_NSEC               0x00000088
#define PMC_ETIME120_REG_CFC_OFFS_NSEC_HI            0x0000008c
#define PMC_ETIME120_REG_CFC_OFFS_NSEC_LO            0x00000090
#define PMC_ETIME120_REG_TOD_ING_DLY_CH0             0x00000094
#define PMC_ETIME120_REG_TOD_ING_DLY_CH1             0x00000098
#define PMC_ETIME120_REG_TOD_ING_DLY_CH2             0x0000009c
#define PMC_ETIME120_REG_TOD_ING_DLY_CH3             0x000000a0
#define PMC_ETIME120_REG_TOD_ING_DLY_CH4             0x000000a4
#define PMC_ETIME120_REG_TOD_ING_DLY_CH5             0x000000a8
#define PMC_ETIME120_REG_TOD_ING_DLY_CH6             0x000000ac
#define PMC_ETIME120_REG_TOD_ING_DLY_CH7             0x000000b0
#define PMC_ETIME120_REG_TOD_ING_DLY_CH8             0x000000b4
#define PMC_ETIME120_REG_TOD_ING_DLY_CH9             0x000000b8
#define PMC_ETIME120_REG_TOD_ING_DLY_CH10            0x000000bc
#define PMC_ETIME120_REG_TOD_ING_DLY_CH11            0x000000c0
#define PMC_ETIME120_REG_CFC_ING_DLY_CH0             0x000000c4
#define PMC_ETIME120_REG_CFC_ING_DLY_CH1             0x000000c8
#define PMC_ETIME120_REG_CFC_ING_DLY_CH2             0x000000cc
#define PMC_ETIME120_REG_CFC_ING_DLY_CH3             0x000000d0
#define PMC_ETIME120_REG_CFC_ING_DLY_CH4             0x000000d4
#define PMC_ETIME120_REG_CFC_ING_DLY_CH5             0x000000d8
#define PMC_ETIME120_REG_CFC_ING_DLY_CH6             0x000000dc
#define PMC_ETIME120_REG_CFC_ING_DLY_CH7             0x000000e0
#define PMC_ETIME120_REG_CFC_ING_DLY_CH8             0x000000e4
#define PMC_ETIME120_REG_CFC_ING_DLY_CH9             0x000000e8
#define PMC_ETIME120_REG_CFC_ING_DLY_CH10            0x000000ec
#define PMC_ETIME120_REG_CFC_ING_DLY_CH11            0x000000f0
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH0             0x000000f4
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH1             0x000000f8
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH2             0x000000fc
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH3             0x00000100
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH4             0x00000104
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH5             0x00000108
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH6             0x0000010c
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH7             0x00000110
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH8             0x00000114
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH9             0x00000118
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH10            0x0000011c
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH11            0x00000120
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH0             0x00000124
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH1             0x00000128
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH2             0x0000012c
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH3             0x00000130
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH4             0x00000134
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH5             0x00000138
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH6             0x0000013c
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH7             0x00000140
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH8             0x00000144
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH9             0x00000148
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH10            0x0000014c
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH11            0x00000150
#define PMC_ETIME120_REG_TS_CNTR_CFG                 0x00000154
#define PMC_ETIME120_REG_OFFS_UPD_TIME_SEC_HI_REG    0x00000158
#define PMC_ETIME120_REG_OFFS_UPD_TIME_SEC_LO_REG    0x0000015c
#define PMC_ETIME120_REG_OFFS_UPD_TIME_NSEC_REG      0x00000160
#define PMC_ETIME120_REG_PTP_CLK_PER_INT_HI_REG      0x00000164
#define PMC_ETIME120_REG_PTP_CLK_PER_INT_LO_REG      0x00000168
#define PMC_ETIME120_REG_PTP_CLK_PER_NUM_REG         0x0000016c
#define PMC_ETIME120_REG_PTP_CLK_PER_DEN_REG         0x00000170
#define PMC_ETIME120_REG_OFFSET_UPD_QUANTA_REG       0x00000174
#define PMC_ETIME120_REG_LINE_CLK_PER_INT_HI_REG     0x00000178
#define PMC_ETIME120_REG_LINE_CLK_PER_INT_LO_REG     0x0000017c
#define PMC_ETIME120_REG_LINE_CLK_PER_NUM_REG        0x00000180
#define PMC_ETIME120_REG_LINE_CLK_PER_DEN_REG        0x00000184
#define PMC_ETIME120_REG_PTP_CNT                     0x00000188
#define PMC_ETIME120_REG_LINE_REF_CNT                0x0000018c
#define PMC_ETIME120_REG_TOD_TS_OUT_EXT_SEC_HI_REG   0x00000190
#define PMC_ETIME120_REG_TOD_TS_OUT_EXT_SEC_LO_REG   0x00000194
#define PMC_ETIME120_REG_TOD_TS_OUT_EXT_NSEC_REG     0x00000198
#define PMC_ETIME120_REG_CFC_TS_OUT_EXT_HI_REG       0x0000019c
#define PMC_ETIME120_REG_CFC_TS_OUT_EXT_LO_REG       0x000001a0
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_HI_REG 0x000001a4
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_LO_REG 0x000001a8
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGI_NSEC_REG   0x000001ac
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGI_HI_REG     0x000001b0
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGI_LO_REG     0x000001b4
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_HI_REG 0x000001b8
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_LO_REG 0x000001bc
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGO_NSEC_REG   0x000001c0
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGO_HI_REG     0x000001c4
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGO_LO_REG     0x000001c8
#define PMC_ETIME120_REG_TX_VEC_IRIGO_4_REG          0x000001cc
#define PMC_ETIME120_REG_TX_VEC_IRIGO_3_REG          0x000001d0
#define PMC_ETIME120_REG_TX_VEC_IRIGO_2_REG          0x000001d4
#define PMC_ETIME120_REG_TX_VEC_IRIGO_1_REG          0x000001d8
#define PMC_ETIME120_REG_RX_VEC_IRIGI_4_REG          0x000001dc
#define PMC_ETIME120_REG_RX_VEC_IRIGI_3_REG          0x000001e0
#define PMC_ETIME120_REG_RX_VEC_IRIGI_2_REG          0x000001e4
#define PMC_ETIME120_REG_RX_VEC_IRIGI_1_REG          0x000001e8
#define PMC_ETIME120_REG_IRIGI_CFG                   0x000001ec
#define PMC_ETIME120_REG_IRIGI_PWM_ZERO_REG          0x000001f0
#define PMC_ETIME120_REG_IRIGI_PWM_ONE_REG           0x000001f4
#define PMC_ETIME120_REG_IRIGI_PWM_WIDTH_PX_REG      0x000001f8
#define PMC_ETIME120_REG_IRIGI_PWM_MIN_DIFF_CFG_REG  0x000001fc
#define PMC_ETIME120_REG_IRIGI_PWM_MAX_DIFF_CFG_REG  0x00000200
#define PMC_ETIME120_REG_IRIGI_DATI_IDLE_DURATION    0x00000204
#define PMC_ETIME120_REG_IRIGO_CFG                   0x00000208
#define PMC_ETIME120_REG_IRIGO_PWM_NOM_PER_CFG_REG   0x0000020c
#define PMC_ETIME120_REG_IRIGO_PWM_ONE_REG           0x00000210
#define PMC_ETIME120_REG_IRIGO_PWM_ZERO_REG          0x00000214
#define PMC_ETIME120_REG_IRIGO_PWM_WIDTH_PX_REG      0x00000218
#define PMC_ETIME120_REG_IRIGO_PWM_1PPS_WIDTH_REG    0x0000021c
#define PMC_ETIME120_REG_INT_GEN_CFG                 0x00000220
#define PMC_ETIME120_REG_INT_PER_CFG                 0x00000224
#define PMC_ETIME120_REG_CC_INT_PER_HI_4_CFG         0x00000228
#define PMC_ETIME120_REG_CC_INT_PER_HI_3_CFG         0x0000022c
#define PMC_ETIME120_REG_CC_INT_PER_HI_2_CFG         0x00000230
#define PMC_ETIME120_REG_CC_INT_PER_HI_1_CFG         0x00000234
#define PMC_ETIME120_REG_CC_INT_PER_4_CFG            0x00000238
#define PMC_ETIME120_REG_CC_INT_PER_3_CFG            0x0000023c
#define PMC_ETIME120_REG_CC_INT_PER_2_CFG            0x00000240
#define PMC_ETIME120_REG_CC_INT_PER_1_CFG            0x00000244
#define PMC_ETIME120_REG_REFL_DLY_CFG                0x0000024c
#define PMC_ETIME120_REG_INTERPOL_CFG                0x00000250
#define PMC_ETIME120_REG_CAPT_OFFS                   0x00000254
#define PMC_ETIME120_REG_EXT_CPB_LSC_LOW             0x00000288
#define PMC_ETIME120_REG_EXT_CPB_LSC_HIGH            0x0000028c
#define PMC_ETIME120_REG_EXT_CPB_CFG                 0x00000290
#define PMC_ETIME120_REG_EXT_LNK_SCH_UPD             0x00000294
#define PMC_ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED      0x00000298
#define PMC_ETIME120_REG_ENA_REG                     0x0000029c
#define PMC_ETIME120_REG_REFL_DROP_ENA_CFG_REG       0x000002a0
#define PMC_ETIME120_REG_LINE_REF_CNT_DLY_REG        0x000002a8
#define PMC_ETIME120_REG_EXT_PMON_LINK0              0x000002ac
#define PMC_ETIME120_REG_EXT_PMON_LINK1              0x000002b0
#define PMC_ETIME120_REG_EXT_PMON_LINK2              0x000002b4
#define PMC_ETIME120_REG_EXT_PMON_LINK3              0x000002b8
#define PMC_ETIME120_REG_EXT_PMON_LINK4              0x000002bc
#define PMC_ETIME120_REG_EXT_PMON_LINK5              0x000002c0
#define PMC_ETIME120_REG_EXT_PMON_LINK6              0x000002c4
#define PMC_ETIME120_REG_EXT_PMON_LINK7              0x000002c8
#define PMC_ETIME120_REG_EXT_PMON_LINK8              0x000002cc
#define PMC_ETIME120_REG_EXT_PMON_LINK9              0x000002d0
#define PMC_ETIME120_REG_EXT_PMON_LINK10             0x000002d4
#define PMC_ETIME120_REG_EXT_PMON_LINK11             0x000002d8
#define PMC_ETIME120_REG_IRIGO_OFFS_S_REG            0x000002dc
#define PMC_ETIME120_REG_IRIGO_OFFS_NS_REG           0x000002e0

/*---------------------------------.
 | Register 0x00000000 REFLLRR     |
 +---------------------------------+
 | bit  11 R/W  REFL_LINK_RESET_11 |
 | bit  10 R/W  REFL_LINK_RESET_10 |
 | bit  9  R/W  REFL_LINK_RESET_9  |
 | bit  8  R/W  REFL_LINK_RESET_8  |
 | bit  7  R/W  REFL_LINK_RESET_7  |
 | bit  6  R/W  REFL_LINK_RESET_6  |
 | bit  5  R/W  REFL_LINK_RESET_5  |
 | bit  4  R/W  REFL_LINK_RESET_4  |
 | bit  3  R/W  REFL_LINK_RESET_3  |
 | bit  2  R/W  REFL_LINK_RESET_2  |
 | bit  1  R/W  REFL_LINK_RESET_1  |
 | bit  0  R/W  REFL_LINK_RESET_0  |
 +--------------------------------*/
#define PMC_ETIME120_REG_REFLLRR_UNUSED_MASK            0xfffff000
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_11_MSK 0x00000800
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_11_OFF 11
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_10_MSK 0x00000400
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_10_OFF 10
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_9_MSK  0x00000200
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_9_OFF  9
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_8_MSK  0x00000100
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_8_OFF  8
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_7_MSK  0x00000080
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_7_OFF  7
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_6_MSK  0x00000040
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_6_OFF  6
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_5_MSK  0x00000020
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_5_OFF  5
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_4_MSK  0x00000010
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_4_OFF  4
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_3_MSK  0x00000008
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_3_OFF  3
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_2_MSK  0x00000004
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_2_OFF  2
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_1_MSK  0x00000002
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_1_OFF  1
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_0_MSK  0x00000001
#define ETIME120_REG_REFLLRR_BIT_REFL_LINK_RESET_0_OFF  0

/*------------------------------------.
 | Register 0x00000004 INT_BEHAV_CTRL |
 +------------------------------------+
 | bit  1 R/W  WCI_MODE_CONTROL       |
 | bit  0 R/W  CLR_INT_CONTROL        |
 +-----------------------------------*/
#define PMC_ETIME120_REG_INT_BEHAV_CTRL_UNUSED_MASK          0xfffffffc
#define ETIME120_REG_INT_BEHAV_CTRL_BIT_WCI_MODE_CONTROL_MSK 0x00000002
#define ETIME120_REG_INT_BEHAV_CTRL_BIT_WCI_MODE_CONTROL_OFF 1
#define ETIME120_REG_INT_BEHAV_CTRL_BIT_CLR_INT_CONTROL_MSK  0x00000001
#define ETIME120_REG_INT_BEHAV_CTRL_BIT_CLR_INT_CONTROL_OFF  0

/*------------------------------------.
 | Register 0x00000008 RAM_BIST_CTRL  |
 +------------------------------------+
 | bit  4   R/W  RAM_LOWPOWER_REG_BIT |
 | bit  3   R/W  PG_OVERRIDE          |
 | bit  2   R/W  ECC_BYPASS           |
 | bit  1:0 R/W  RAM_ERR_INJECT       |
 +-----------------------------------*/
#define PMC_ETIME120_REG_RAM_BIST_CTRL_UNUSED_MASK              0xffffffe0
#define ETIME120_REG_RAM_BIST_CTRL_BIT_RAM_LOWPOWER_REG_BIT_MSK 0x00000010
#define ETIME120_REG_RAM_BIST_CTRL_BIT_RAM_LOWPOWER_REG_BIT_OFF 4
#define ETIME120_REG_RAM_BIST_CTRL_BIT_PG_OVERRIDE_MSK          0x00000008
#define ETIME120_REG_RAM_BIST_CTRL_BIT_PG_OVERRIDE_OFF          3
#define ETIME120_REG_RAM_BIST_CTRL_BIT_ECC_BYPASS_MSK           0x00000004
#define ETIME120_REG_RAM_BIST_CTRL_BIT_ECC_BYPASS_OFF           2
#define ETIME120_REG_RAM_BIST_CTRL_BIT_RAM_ERR_INJECT_MSK       0x00000003
#define ETIME120_REG_RAM_BIST_CTRL_BIT_RAM_ERR_INJECT_OFF       0

/*----------------------------------------.
 | Register 0x0000000c SB_ECC_ERR_INT_REG |
 +----------------------------------------+
 | bit  16:0 R/W  RAM_BIT_SB_ECC_INT_I    |
 +---------------------------------------*/
#define PMC_ETIME120_REG_SB_ECC_ERR_INT_REG_UNUSED_MASK              0xfffe0000
#define ETIME120_REG_SB_ECC_ERR_INT_REG_BIT_RAM_BIT_SB_ECC_INT_I_MSK 0x0001ffff
#define ETIME120_REG_SB_ECC_ERR_INT_REG_BIT_RAM_BIT_SB_ECC_INT_I_OFF 0

/*----------------------------------------.
 | Register 0x00000010 MB_ECC_ERR_INT_REG |
 +----------------------------------------+
 | bit  16:0 R/W  RAM_BIT_MB_ECC_INT_I    |
 +---------------------------------------*/
#define PMC_ETIME120_REG_MB_ECC_ERR_INT_REG_UNUSED_MASK              0xfffe0000
#define ETIME120_REG_MB_ECC_ERR_INT_REG_BIT_RAM_BIT_MB_ECC_INT_I_MSK 0x0001ffff
#define ETIME120_REG_MB_ECC_ERR_INT_REG_BIT_RAM_BIT_MB_ECC_INT_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000014 REFL_OFLOW_ERR_INT_REG |
 +--------------------------------------------+
 | bit  11:0 R/W  EXT_RAM_OFLOW_INT_I         |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_REFL_OFLOW_ERR_INT_REG_UNUSED_MASK             0xfffff000
#define ETIME120_REG_REFL_OFLOW_ERR_INT_REG_BIT_EXT_RAM_OFLOW_INT_I_MSK 0x00000fff
#define ETIME120_REG_REFL_OFLOW_ERR_INT_REG_BIT_EXT_RAM_OFLOW_INT_I_OFF 0

/*-------------------------------------.
 | Register 0x00000018 TS_DIS_INT_REG  |
 +-------------------------------------+
 | bit  23:12 R/W  TS_RAM_WR_DIS_INT_I |
 | bit  11:0  R/W  TS_RAM_RD_DIS_INT_I |
 +------------------------------------*/
#define PMC_ETIME120_REG_TS_DIS_INT_REG_UNUSED_MASK             0xff000000
#define ETIME120_REG_TS_DIS_INT_REG_BIT_TS_RAM_WR_DIS_INT_I_MSK 0x00fff000
#define ETIME120_REG_TS_DIS_INT_REG_BIT_TS_RAM_WR_DIS_INT_I_OFF 12
#define ETIME120_REG_TS_DIS_INT_REG_BIT_TS_RAM_RD_DIS_INT_I_MSK 0x00000fff
#define ETIME120_REG_TS_DIS_INT_REG_BIT_TS_RAM_RD_DIS_INT_I_OFF 0

/*-----------------------------------------.
 | Register 0x0000001c PACKET_DIS_INT_REG  |
 +-----------------------------------------+
 | bit  23:12 R/W  PACKET_RAM_WR_DIS_INT_I |
 | bit  11:0  R/W  PACKET_RAM_RD_DIS_INT_I |
 +----------------------------------------*/
#define PMC_ETIME120_REG_PACKET_DIS_INT_REG_UNUSED_MASK                 0xff000000
#define ETIME120_REG_PACKET_DIS_INT_REG_BIT_PACKET_RAM_WR_DIS_INT_I_MSK 0x00fff000
#define ETIME120_REG_PACKET_DIS_INT_REG_BIT_PACKET_RAM_WR_DIS_INT_I_OFF 12
#define ETIME120_REG_PACKET_DIS_INT_REG_BIT_PACKET_RAM_RD_DIS_INT_I_MSK 0x00000fff
#define ETIME120_REG_PACKET_DIS_INT_REG_BIT_PACKET_RAM_RD_DIS_INT_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000020 PACKET_RAM_ERR_INT_REG |
 +--------------------------------------------+
 | bit  11:0 R/W  PACKET_RAM_OSIZE_INT_I      |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_PACKET_RAM_ERR_INT_REG_UNUSED_MASK                0xfffff000
#define ETIME120_REG_PACKET_RAM_ERR_INT_REG_BIT_PACKET_RAM_OSIZE_INT_I_MSK 0x00000fff
#define ETIME120_REG_PACKET_RAM_ERR_INT_REG_BIT_PACKET_RAM_OSIZE_INT_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000024 EXT_DESYNC_ERR_INT_REG |
 +--------------------------------------------+
 | bit  11:0 R/W  EXT_DESYNC_INT_I            |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_EXT_DESYNC_ERR_INT_REG_UNUSED_MASK          0xfffff000
#define ETIME120_REG_EXT_DESYNC_ERR_INT_REG_BIT_EXT_DESYNC_INT_I_MSK 0x00000fff
#define ETIME120_REG_EXT_DESYNC_ERR_INT_REG_BIT_EXT_DESYNC_INT_I_OFF 0

/*---------------------------------------------.
 | Register 0x00000028 GP_INT_REG              |
 +---------------------------------------------+
 | bit  27 R/W  ILLEGAL_1STEP_INT_I            |
 | bit  26 R/W  LINE_REF_CPTR_EVNT_I           |
 | bit  25 R/W  IRIGO_TS_CPTR_EVNT_I           |
 | bit  24 R/W  IRIGO_ONE_PPSO_REALIGN_I       |
 | bit  23 R/W  IRIGO_TX_DAT_VAL_I             |
 | bit  22 R/W  PTP_DCSU_TSTMPR_CPTR_EVNT_I    |
 | bit  21 R/W  IRIGI_TS_CPTR_EVNT_I           |
 | bit  20 R/W  IRIGI_RX_DAT_VAL_I             |
 | bit  19 R/W  IRIGI_REALIGN_I                |
 | bit  18 R/W  IRIGI_P0PR_MISSING_I           |
 | bit  17 R/W  IRIGI_IDLE_I                   |
 | bit  16 R/W  IRIGI_ONE_PPSO_REALIGN_I       |
 | bit  15 R/W  IRIGI_PWM_DEC_ERR_I            |
 | bit  14 R/W  TOD_CFC_OFFS_TS_CNTR_UPDATED_I |
 | bit  13 R/W  TS_INTERPOLATE_CNTR_ERR_I      |
 | bit  12 R/W  TS_TOD_SIGN_OVRFL_I            |
 | bit  11 R/W  CFC_OVRFL_I                    |
 | bit  10 R/W  TOD_TS_OVRFL_I                 |
 | bit  9  R/W  FREE_RUN_CFC_OVRFL_I           |
 | bit  8  R/W  FREE_RUN_TS_OVRFL_I            |
 | bit  7  R/W  TOD_INT_1_I                    |
 | bit  6  R/W  TOD_INT_2_I                    |
 | bit  5  R/W  FREE_RUN_INT_1_I               |
 | bit  4  R/W  FREE_RUN_INT_2_I               |
 | bit  3  R/W  CC_INT_1_I                     |
 | bit  2  R/W  CC_INT_2_I                     |
 | bit  1  R/W  CC_INT_3_I                     |
 | bit  0  R/W  CC_INT_4_I                     |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_GP_INT_REG_UNUSED_MASK                        0xf0000000
#define ETIME120_REG_GP_INT_REG_BIT_ILLEGAL_1STEP_INT_I_MSK            0x08000000
#define ETIME120_REG_GP_INT_REG_BIT_ILLEGAL_1STEP_INT_I_OFF            27
#define ETIME120_REG_GP_INT_REG_BIT_LINE_REF_CPTR_EVNT_I_MSK           0x04000000
#define ETIME120_REG_GP_INT_REG_BIT_LINE_REF_CPTR_EVNT_I_OFF           26
#define ETIME120_REG_GP_INT_REG_BIT_IRIGO_TS_CPTR_EVNT_I_MSK           0x02000000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGO_TS_CPTR_EVNT_I_OFF           25
#define ETIME120_REG_GP_INT_REG_BIT_IRIGO_ONE_PPSO_REALIGN_I_MSK       0x01000000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGO_ONE_PPSO_REALIGN_I_OFF       24
#define ETIME120_REG_GP_INT_REG_BIT_IRIGO_TX_DAT_VAL_I_MSK             0x00800000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGO_TX_DAT_VAL_I_OFF             23
#define ETIME120_REG_GP_INT_REG_BIT_PTP_DCSU_TSTMPR_CPTR_EVNT_I_MSK    0x00400000
#define ETIME120_REG_GP_INT_REG_BIT_PTP_DCSU_TSTMPR_CPTR_EVNT_I_OFF    22
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_TS_CPTR_EVNT_I_MSK           0x00200000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_TS_CPTR_EVNT_I_OFF           21
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_RX_DAT_VAL_I_MSK             0x00100000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_RX_DAT_VAL_I_OFF             20
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_REALIGN_I_MSK                0x00080000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_REALIGN_I_OFF                19
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_P0PR_MISSING_I_MSK           0x00040000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_P0PR_MISSING_I_OFF           18
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_IDLE_I_MSK                   0x00020000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_IDLE_I_OFF                   17
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_ONE_PPSO_REALIGN_I_MSK       0x00010000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_ONE_PPSO_REALIGN_I_OFF       16
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_PWM_DEC_ERR_I_MSK            0x00008000
#define ETIME120_REG_GP_INT_REG_BIT_IRIGI_PWM_DEC_ERR_I_OFF            15
#define ETIME120_REG_GP_INT_REG_BIT_TOD_CFC_OFFS_TS_CNTR_UPDATED_I_MSK 0x00004000
#define ETIME120_REG_GP_INT_REG_BIT_TOD_CFC_OFFS_TS_CNTR_UPDATED_I_OFF 14
#define ETIME120_REG_GP_INT_REG_BIT_TS_INTERPOLATE_CNTR_ERR_I_MSK      0x00002000
#define ETIME120_REG_GP_INT_REG_BIT_TS_INTERPOLATE_CNTR_ERR_I_OFF      13
#define ETIME120_REG_GP_INT_REG_BIT_TS_TOD_SIGN_OVRFL_I_MSK            0x00001000
#define ETIME120_REG_GP_INT_REG_BIT_TS_TOD_SIGN_OVRFL_I_OFF            12
#define ETIME120_REG_GP_INT_REG_BIT_CFC_OVRFL_I_MSK                    0x00000800
#define ETIME120_REG_GP_INT_REG_BIT_CFC_OVRFL_I_OFF                    11
#define ETIME120_REG_GP_INT_REG_BIT_TOD_TS_OVRFL_I_MSK                 0x00000400
#define ETIME120_REG_GP_INT_REG_BIT_TOD_TS_OVRFL_I_OFF                 10
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_CFC_OVRFL_I_MSK           0x00000200
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_CFC_OVRFL_I_OFF           9
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_TS_OVRFL_I_MSK            0x00000100
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_TS_OVRFL_I_OFF            8
#define ETIME120_REG_GP_INT_REG_BIT_TOD_INT_1_I_MSK                    0x00000080
#define ETIME120_REG_GP_INT_REG_BIT_TOD_INT_1_I_OFF                    7
#define ETIME120_REG_GP_INT_REG_BIT_TOD_INT_2_I_MSK                    0x00000040
#define ETIME120_REG_GP_INT_REG_BIT_TOD_INT_2_I_OFF                    6
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_INT_1_I_MSK               0x00000020
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_INT_1_I_OFF               5
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_INT_2_I_MSK               0x00000010
#define ETIME120_REG_GP_INT_REG_BIT_FREE_RUN_INT_2_I_OFF               4
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_1_I_MSK                     0x00000008
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_1_I_OFF                     3
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_2_I_MSK                     0x00000004
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_2_I_OFF                     2
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_3_I_MSK                     0x00000002
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_3_I_OFF                     1
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_4_I_MSK                     0x00000001
#define ETIME120_REG_GP_INT_REG_BIT_CC_INT_4_I_OFF                     0

/*-------------------------------------------.
 | Register 0x0000002c SB_ECC_ERR_INT_EN_REG |
 +-------------------------------------------+
 | bit  16:0 R/W  RAM_BIT_SB_ECC_INT_E       |
 +------------------------------------------*/
#define PMC_ETIME120_REG_SB_ECC_ERR_INT_EN_REG_UNUSED_MASK              0xfffe0000
#define ETIME120_REG_SB_ECC_ERR_INT_EN_REG_BIT_RAM_BIT_SB_ECC_INT_E_MSK 0x0001ffff
#define ETIME120_REG_SB_ECC_ERR_INT_EN_REG_BIT_RAM_BIT_SB_ECC_INT_E_OFF 0

/*-------------------------------------------.
 | Register 0x00000030 MB_ECC_ERR_INT_EN_REG |
 +-------------------------------------------+
 | bit  16:0 R/W  RAM_BIT_MB_ECC_INT_E       |
 +------------------------------------------*/
#define PMC_ETIME120_REG_MB_ECC_ERR_INT_EN_REG_UNUSED_MASK              0xfffe0000
#define ETIME120_REG_MB_ECC_ERR_INT_EN_REG_BIT_RAM_BIT_MB_ECC_INT_E_MSK 0x0001ffff
#define ETIME120_REG_MB_ECC_ERR_INT_EN_REG_BIT_RAM_BIT_MB_ECC_INT_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000034 REFL_OFLOW_ERR_INT_EN_REG |
 +-----------------------------------------------+
 | bit  11:0 R/W  EXT_RAM_OFLOW_INT_E            |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_REFL_OFLOW_ERR_INT_EN_REG_UNUSED_MASK             0xfffff000
#define ETIME120_REG_REFL_OFLOW_ERR_INT_EN_REG_BIT_EXT_RAM_OFLOW_INT_E_MSK 0x00000fff
#define ETIME120_REG_REFL_OFLOW_ERR_INT_EN_REG_BIT_EXT_RAM_OFLOW_INT_E_OFF 0

/*---------------------------------------.
 | Register 0x00000038 TS_DIS_INT_EN_REG |
 +---------------------------------------+
 | bit  23:12 R/W  TS_RAM_WR_DIS_INT_E   |
 | bit  11:0  R/W  TS_RAM_RD_DIS_INT_E   |
 +--------------------------------------*/
#define PMC_ETIME120_REG_TS_DIS_INT_EN_REG_UNUSED_MASK             0xff000000
#define ETIME120_REG_TS_DIS_INT_EN_REG_BIT_TS_RAM_WR_DIS_INT_E_MSK 0x00fff000
#define ETIME120_REG_TS_DIS_INT_EN_REG_BIT_TS_RAM_WR_DIS_INT_E_OFF 12
#define ETIME120_REG_TS_DIS_INT_EN_REG_BIT_TS_RAM_RD_DIS_INT_E_MSK 0x00000fff
#define ETIME120_REG_TS_DIS_INT_EN_REG_BIT_TS_RAM_RD_DIS_INT_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000003c PACKET_DIS_INT_EN_REG |
 +-------------------------------------------+
 | bit  23:12 R/W  PACKET_RAM_WR_DIS_INT_E   |
 | bit  11:0  R/W  PACKET_RAM_RD_DIS_INT_E   |
 +------------------------------------------*/
#define PMC_ETIME120_REG_PACKET_DIS_INT_EN_REG_UNUSED_MASK                 0xff000000
#define ETIME120_REG_PACKET_DIS_INT_EN_REG_BIT_PACKET_RAM_WR_DIS_INT_E_MSK 0x00fff000
#define ETIME120_REG_PACKET_DIS_INT_EN_REG_BIT_PACKET_RAM_WR_DIS_INT_E_OFF 12
#define ETIME120_REG_PACKET_DIS_INT_EN_REG_BIT_PACKET_RAM_RD_DIS_INT_E_MSK 0x00000fff
#define ETIME120_REG_PACKET_DIS_INT_EN_REG_BIT_PACKET_RAM_RD_DIS_INT_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000040 PACKET_RAM_ERR_INT_EN_REG |
 +-----------------------------------------------+
 | bit  11:0 R/W  PACKET_RAM_OSIZE_INT_E         |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_PACKET_RAM_ERR_INT_EN_REG_UNUSED_MASK                0xfffff000
#define ETIME120_REG_PACKET_RAM_ERR_INT_EN_REG_BIT_PACKET_RAM_OSIZE_INT_E_MSK 0x00000fff
#define ETIME120_REG_PACKET_RAM_ERR_INT_EN_REG_BIT_PACKET_RAM_OSIZE_INT_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000044 EXT_DESYNC_ERR_INT_EN_REG |
 +-----------------------------------------------+
 | bit  11:0 R/W  EXT_DESYNC_INT_E               |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_EXT_DESYNC_ERR_INT_EN_REG_UNUSED_MASK          0xfffff000
#define ETIME120_REG_EXT_DESYNC_ERR_INT_EN_REG_BIT_EXT_DESYNC_INT_E_MSK 0x00000fff
#define ETIME120_REG_EXT_DESYNC_ERR_INT_EN_REG_BIT_EXT_DESYNC_INT_E_OFF 0

/*---------------------------------------------.
 | Register 0x00000048 GP_INT_EN_REG           |
 +---------------------------------------------+
 | bit  27 R/W  ILLEGAL_1STEP_INT_E            |
 | bit  26 R/W  LINE_REF_CPTR_EVNT_E           |
 | bit  25 R/W  IRIGO_TS_CPTR_EVNT_E           |
 | bit  24 R/W  IRIGO_ONE_PPSO_REALIGN_E       |
 | bit  23 R/W  IRIGO_TX_DAT_VAL_E             |
 | bit  22 R/W  PTP_DCSU_TSTMPR_CPTR_EVNT_E    |
 | bit  21 R/W  IRIGI_TS_CPTR_EVNT_E           |
 | bit  20 R/W  IRIGI_RX_DAT_VAL_E             |
 | bit  19 R/W  IRIGI_REALIGN_E                |
 | bit  18 R/W  IRIGI_P0PR_MISSING_E           |
 | bit  17 R/W  IRIGI_IDLE_E                   |
 | bit  16 R/W  IRIGI_ONE_PPSO_REALIGN_E       |
 | bit  15 R/W  IRIGI_PWM_DEC_ERR_E            |
 | bit  14 R/W  TOD_CFC_OFFS_TS_CNTR_UPDATED_E |
 | bit  13 R/W  TS_INTERPOLATE_CNTR_ERR_E      |
 | bit  12 R/W  TS_TOD_SIGN_OVRFL_E            |
 | bit  11 R/W  CFC_OVRFL_E                    |
 | bit  10 R/W  TOD_TS_OVRFL_E                 |
 | bit  9  R/W  FREE_RUN_CFC_OVRFL_E           |
 | bit  8  R/W  FREE_RUN_TS_OVRFL_E            |
 | bit  7  R/W  TOD_INT_1_E                    |
 | bit  6  R/W  TOD_INT_2_E                    |
 | bit  5  R/W  FREE_RUN_INT_1_E               |
 | bit  4  R/W  FREE_RUN_INT_2_E               |
 | bit  3  R/W  CC_INT_1_E                     |
 | bit  2  R/W  CC_INT_2_E                     |
 | bit  1  R/W  CC_INT_3_E                     |
 | bit  0  R/W  CC_INT_4_E                     |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_GP_INT_EN_REG_UNUSED_MASK                        0xf0000000
#define ETIME120_REG_GP_INT_EN_REG_BIT_ILLEGAL_1STEP_INT_E_MSK            0x08000000
#define ETIME120_REG_GP_INT_EN_REG_BIT_ILLEGAL_1STEP_INT_E_OFF            27
#define ETIME120_REG_GP_INT_EN_REG_BIT_LINE_REF_CPTR_EVNT_E_MSK           0x04000000
#define ETIME120_REG_GP_INT_EN_REG_BIT_LINE_REF_CPTR_EVNT_E_OFF           26
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGO_TS_CPTR_EVNT_E_MSK           0x02000000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGO_TS_CPTR_EVNT_E_OFF           25
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGO_ONE_PPSO_REALIGN_E_MSK       0x01000000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGO_ONE_PPSO_REALIGN_E_OFF       24
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGO_TX_DAT_VAL_E_MSK             0x00800000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGO_TX_DAT_VAL_E_OFF             23
#define ETIME120_REG_GP_INT_EN_REG_BIT_PTP_DCSU_TSTMPR_CPTR_EVNT_E_MSK    0x00400000
#define ETIME120_REG_GP_INT_EN_REG_BIT_PTP_DCSU_TSTMPR_CPTR_EVNT_E_OFF    22
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_TS_CPTR_EVNT_E_MSK           0x00200000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_TS_CPTR_EVNT_E_OFF           21
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_RX_DAT_VAL_E_MSK             0x00100000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_RX_DAT_VAL_E_OFF             20
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_REALIGN_E_MSK                0x00080000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_REALIGN_E_OFF                19
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_P0PR_MISSING_E_MSK           0x00040000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_P0PR_MISSING_E_OFF           18
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_IDLE_E_MSK                   0x00020000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_IDLE_E_OFF                   17
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_ONE_PPSO_REALIGN_E_MSK       0x00010000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_ONE_PPSO_REALIGN_E_OFF       16
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_PWM_DEC_ERR_E_MSK            0x00008000
#define ETIME120_REG_GP_INT_EN_REG_BIT_IRIGI_PWM_DEC_ERR_E_OFF            15
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_CFC_OFFS_TS_CNTR_UPDATED_E_MSK 0x00004000
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_CFC_OFFS_TS_CNTR_UPDATED_E_OFF 14
#define ETIME120_REG_GP_INT_EN_REG_BIT_TS_INTERPOLATE_CNTR_ERR_E_MSK      0x00002000
#define ETIME120_REG_GP_INT_EN_REG_BIT_TS_INTERPOLATE_CNTR_ERR_E_OFF      13
#define ETIME120_REG_GP_INT_EN_REG_BIT_TS_TOD_SIGN_OVRFL_E_MSK            0x00001000
#define ETIME120_REG_GP_INT_EN_REG_BIT_TS_TOD_SIGN_OVRFL_E_OFF            12
#define ETIME120_REG_GP_INT_EN_REG_BIT_CFC_OVRFL_E_MSK                    0x00000800
#define ETIME120_REG_GP_INT_EN_REG_BIT_CFC_OVRFL_E_OFF                    11
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_TS_OVRFL_E_MSK                 0x00000400
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_TS_OVRFL_E_OFF                 10
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_CFC_OVRFL_E_MSK           0x00000200
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_CFC_OVRFL_E_OFF           9
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_TS_OVRFL_E_MSK            0x00000100
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_TS_OVRFL_E_OFF            8
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_INT_1_E_MSK                    0x00000080
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_INT_1_E_OFF                    7
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_INT_2_E_MSK                    0x00000040
#define ETIME120_REG_GP_INT_EN_REG_BIT_TOD_INT_2_E_OFF                    6
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_INT_1_E_MSK               0x00000020
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_INT_1_E_OFF               5
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_INT_2_E_MSK               0x00000010
#define ETIME120_REG_GP_INT_EN_REG_BIT_FREE_RUN_INT_2_E_OFF               4
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_1_E_MSK                     0x00000008
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_1_E_OFF                     3
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_2_E_MSK                     0x00000004
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_2_E_OFF                     2
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_3_E_MSK                     0x00000002
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_3_E_OFF                     1
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_4_E_MSK                     0x00000001
#define ETIME120_REG_GP_INT_EN_REG_BIT_CC_INT_4_E_OFF                     0

/*--------------------------------------------.
 | Register 0x0000004c SB_ECC_ERR_INT_VAL_REG |
 +--------------------------------------------+
 | bit  16:0 R  RAM_BIT_SB_ECC_INT_V          |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_SB_ECC_ERR_INT_VAL_REG_UNUSED_MASK              0xfffe0000
#define ETIME120_REG_SB_ECC_ERR_INT_VAL_REG_BIT_RAM_BIT_SB_ECC_INT_V_MSK 0x0001ffff
#define ETIME120_REG_SB_ECC_ERR_INT_VAL_REG_BIT_RAM_BIT_SB_ECC_INT_V_OFF 0

/*--------------------------------------------.
 | Register 0x00000050 MB_ECC_ERR_INT_VAL_REG |
 +--------------------------------------------+
 | bit  16:0 R  RAM_BIT_MB_ECC_INT_V          |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_MB_ECC_ERR_INT_VAL_REG_UNUSED_MASK              0xfffe0000
#define ETIME120_REG_MB_ECC_ERR_INT_VAL_REG_BIT_RAM_BIT_MB_ECC_INT_V_MSK 0x0001ffff
#define ETIME120_REG_MB_ECC_ERR_INT_VAL_REG_BIT_RAM_BIT_MB_ECC_INT_V_OFF 0

/*------------------------------------------------.
 | Register 0x00000054 REFL_OFLOW_ERR_INT_VAL_REG |
 +------------------------------------------------+
 | bit  11:0 R  EXT_RAM_OFLOW_INT_V               |
 +-----------------------------------------------*/
#define PMC_ETIME120_REG_REFL_OFLOW_ERR_INT_VAL_REG_UNUSED_MASK             0xfffff000
#define ETIME120_REG_REFL_OFLOW_ERR_INT_VAL_REG_BIT_EXT_RAM_OFLOW_INT_V_MSK 0x00000fff
#define ETIME120_REG_REFL_OFLOW_ERR_INT_VAL_REG_BIT_EXT_RAM_OFLOW_INT_V_OFF 0

/*----------------------------------------.
 | Register 0x00000058 TS_DIS_INT_VAL_REG |
 +----------------------------------------+
 | bit  23:12 R  TS_RAM_WR_DIS_INT_V      |
 | bit  11:0  R  TS_RAM_RD_DIS_INT_V      |
 +---------------------------------------*/
#define PMC_ETIME120_REG_TS_DIS_INT_VAL_REG_UNUSED_MASK             0xff000000
#define ETIME120_REG_TS_DIS_INT_VAL_REG_BIT_TS_RAM_WR_DIS_INT_V_MSK 0x00fff000
#define ETIME120_REG_TS_DIS_INT_VAL_REG_BIT_TS_RAM_WR_DIS_INT_V_OFF 12
#define ETIME120_REG_TS_DIS_INT_VAL_REG_BIT_TS_RAM_RD_DIS_INT_V_MSK 0x00000fff
#define ETIME120_REG_TS_DIS_INT_VAL_REG_BIT_TS_RAM_RD_DIS_INT_V_OFF 0

/*--------------------------------------------.
 | Register 0x0000005c PACKET_DIS_INT_VAL_REG |
 +--------------------------------------------+
 | bit  23:12 R  PACKET_RAM_WR_DIS_INT_V      |
 | bit  11:0  R  PACKET_RAM_RD_DIS_INT_V      |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_PACKET_DIS_INT_VAL_REG_UNUSED_MASK                 0xff000000
#define ETIME120_REG_PACKET_DIS_INT_VAL_REG_BIT_PACKET_RAM_WR_DIS_INT_V_MSK 0x00fff000
#define ETIME120_REG_PACKET_DIS_INT_VAL_REG_BIT_PACKET_RAM_WR_DIS_INT_V_OFF 12
#define ETIME120_REG_PACKET_DIS_INT_VAL_REG_BIT_PACKET_RAM_RD_DIS_INT_V_MSK 0x00000fff
#define ETIME120_REG_PACKET_DIS_INT_VAL_REG_BIT_PACKET_RAM_RD_DIS_INT_V_OFF 0

/*------------------------------------------------.
 | Register 0x00000060 PACKET_RAM_ERR_INT_VAL_REG |
 +------------------------------------------------+
 | bit  11:0 R  PACKET_RAM_OSIZE_INT_V            |
 +-----------------------------------------------*/
#define PMC_ETIME120_REG_PACKET_RAM_ERR_INT_VAL_REG_UNUSED_MASK                0xfffff000
#define ETIME120_REG_PACKET_RAM_ERR_INT_VAL_REG_BIT_PACKET_RAM_OSIZE_INT_V_MSK 0x00000fff
#define ETIME120_REG_PACKET_RAM_ERR_INT_VAL_REG_BIT_PACKET_RAM_OSIZE_INT_V_OFF 0

/*------------------------------------------------.
 | Register 0x00000064 EXT_DESYNC_ERR_INT_VAL_REG |
 +------------------------------------------------+
 | bit  11:0 R  EXT_DESYNC_INT_V                  |
 +-----------------------------------------------*/
#define PMC_ETIME120_REG_EXT_DESYNC_ERR_INT_VAL_REG_UNUSED_MASK          0xfffff000
#define ETIME120_REG_EXT_DESYNC_ERR_INT_VAL_REG_BIT_EXT_DESYNC_INT_V_MSK 0x00000fff
#define ETIME120_REG_EXT_DESYNC_ERR_INT_VAL_REG_BIT_EXT_DESYNC_INT_V_OFF 0

/*-------------------------------------------.
 | Register 0x00000068 GP_INT_VAL_REG        |
 +-------------------------------------------+
 | bit  27 R  ILLEGAL_1STEP_INT_V            |
 | bit  26 R  LINE_REF_CPTR_EVNT_V           |
 | bit  25 R  IRIGO_TS_CPTR_EVNT_V           |
 | bit  24 R  IRIGO_ONE_PPSO_REALIGN_V       |
 | bit  23 R  IRIGO_TX_DAT_VAL_V             |
 | bit  22 R  PTP_DCSU_TSTMPR_CPTR_EVNT_V    |
 | bit  21 R  IRIGI_TS_CPTR_EVNT_V           |
 | bit  20 R  IRIGI_RX_DAT_VAL_V             |
 | bit  19 R  IRIGI_REALIGN_V                |
 | bit  18 R  IRIGI_P0PR_MISSING_V           |
 | bit  17 R  IRIGI_IDLE_V                   |
 | bit  16 R  IRIGI_ONE_PPSO_REALIGN_V       |
 | bit  15 R  IRIGI_PWM_DEC_ERR_V            |
 | bit  14 R  TOD_CFC_OFFS_TS_CNTR_UPDATED_V |
 | bit  13 R  TS_INTERPOLATE_CNTR_ERR_V      |
 | bit  12 R  TS_TOD_SIGN_OVRFL_V            |
 | bit  11 R  CFC_OVRFL_V                    |
 | bit  10 R  TOD_TS_OVRFL_V                 |
 | bit  9  R  FREE_RUN_CFC_OVRFL_V           |
 | bit  8  R  FREE_RUN_TS_OVRFL_V            |
 | bit  7  R  TOD_INT_1_V                    |
 | bit  6  R  TOD_INT_2_V                    |
 | bit  5  R  FREE_RUN_INT_1_V               |
 | bit  4  R  FREE_RUN_INT_2_V               |
 | bit  3  R  CC_INT_1_V                     |
 | bit  2  R  CC_INT_2_V                     |
 | bit  1  R  CC_INT_3_V                     |
 | bit  0  R  CC_INT_4_V                     |
 +------------------------------------------*/
#define PMC_ETIME120_REG_GP_INT_VAL_REG_UNUSED_MASK                        0xf0000000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_ILLEGAL_1STEP_INT_V_MSK            0x08000000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_ILLEGAL_1STEP_INT_V_OFF            27
#define ETIME120_REG_GP_INT_VAL_REG_BIT_LINE_REF_CPTR_EVNT_V_MSK           0x04000000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_LINE_REF_CPTR_EVNT_V_OFF           26
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGO_TS_CPTR_EVNT_V_MSK           0x02000000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGO_TS_CPTR_EVNT_V_OFF           25
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGO_ONE_PPSO_REALIGN_V_MSK       0x01000000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGO_ONE_PPSO_REALIGN_V_OFF       24
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGO_TX_DAT_VAL_V_MSK             0x00800000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGO_TX_DAT_VAL_V_OFF             23
#define ETIME120_REG_GP_INT_VAL_REG_BIT_PTP_DCSU_TSTMPR_CPTR_EVNT_V_MSK    0x00400000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_PTP_DCSU_TSTMPR_CPTR_EVNT_V_OFF    22
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_TS_CPTR_EVNT_V_MSK           0x00200000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_TS_CPTR_EVNT_V_OFF           21
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_RX_DAT_VAL_V_MSK             0x00100000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_RX_DAT_VAL_V_OFF             20
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_REALIGN_V_MSK                0x00080000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_REALIGN_V_OFF                19
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_P0PR_MISSING_V_MSK           0x00040000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_P0PR_MISSING_V_OFF           18
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_IDLE_V_MSK                   0x00020000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_IDLE_V_OFF                   17
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_ONE_PPSO_REALIGN_V_MSK       0x00010000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_ONE_PPSO_REALIGN_V_OFF       16
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_PWM_DEC_ERR_V_MSK            0x00008000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_IRIGI_PWM_DEC_ERR_V_OFF            15
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_CFC_OFFS_TS_CNTR_UPDATED_V_MSK 0x00004000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_CFC_OFFS_TS_CNTR_UPDATED_V_OFF 14
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TS_INTERPOLATE_CNTR_ERR_V_MSK      0x00002000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TS_INTERPOLATE_CNTR_ERR_V_OFF      13
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TS_TOD_SIGN_OVRFL_V_MSK            0x00001000
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TS_TOD_SIGN_OVRFL_V_OFF            12
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CFC_OVRFL_V_MSK                    0x00000800
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CFC_OVRFL_V_OFF                    11
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_TS_OVRFL_V_MSK                 0x00000400
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_TS_OVRFL_V_OFF                 10
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_CFC_OVRFL_V_MSK           0x00000200
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_CFC_OVRFL_V_OFF           9
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_TS_OVRFL_V_MSK            0x00000100
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_TS_OVRFL_V_OFF            8
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_INT_1_V_MSK                    0x00000080
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_INT_1_V_OFF                    7
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_INT_2_V_MSK                    0x00000040
#define ETIME120_REG_GP_INT_VAL_REG_BIT_TOD_INT_2_V_OFF                    6
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_INT_1_V_MSK               0x00000020
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_INT_1_V_OFF               5
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_INT_2_V_MSK               0x00000010
#define ETIME120_REG_GP_INT_VAL_REG_BIT_FREE_RUN_INT_2_V_OFF               4
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_1_V_MSK                     0x00000008
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_1_V_OFF                     3
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_2_V_MSK                     0x00000004
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_2_V_OFF                     2
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_3_V_MSK                     0x00000002
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_3_V_OFF                     1
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_4_V_MSK                     0x00000001
#define ETIME120_REG_GP_INT_VAL_REG_BIT_CC_INT_4_V_OFF                     0

/*-------------------------------.
 | Register 0x0000006c ING_47B_M |
 +-------------------------------+
 | bit  11 R/W  ING_47B_M_CH11   |
 | bit  10 R/W  ING_47B_M_CH10   |
 | bit  9  R/W  ING_47B_M_CH9    |
 | bit  8  R/W  ING_47B_M_CH8    |
 | bit  7  R/W  ING_47B_M_CH7    |
 | bit  6  R/W  ING_47B_M_CH6    |
 | bit  5  R/W  ING_47B_M_CH5    |
 | bit  4  R/W  ING_47B_M_CH4    |
 | bit  3  R/W  ING_47B_M_CH3    |
 | bit  2  R/W  ING_47B_M_CH2    |
 | bit  1  R/W  ING_47B_M_CH1    |
 | bit  0  R/W  ING_47B_M_CH0    |
 +------------------------------*/
#define PMC_ETIME120_REG_ING_47B_M_UNUSED_MASK        0xfffff000
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH11_MSK 0x00000800
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH11_OFF 11
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH10_MSK 0x00000400
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH10_OFF 10
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH9_MSK  0x00000200
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH9_OFF  9
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH8_MSK  0x00000100
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH8_OFF  8
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH7_MSK  0x00000080
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH7_OFF  7
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH6_MSK  0x00000040
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH6_OFF  6
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH5_MSK  0x00000020
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH5_OFF  5
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH4_MSK  0x00000010
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH4_OFF  4
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH3_MSK  0x00000008
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH3_OFF  3
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH2_MSK  0x00000004
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH2_OFF  2
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH1_MSK  0x00000002
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH1_OFF  1
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH0_MSK  0x00000001
#define ETIME120_REG_ING_47B_M_BIT_ING_47B_M_CH0_OFF  0

/*-------------------------------.
 | Register 0x00000070 EGR_47B_M |
 +-------------------------------+
 | bit  11 R/W  EGR_47B_M_CH11   |
 | bit  10 R/W  EGR_47B_M_CH10   |
 | bit  9  R/W  EGR_47B_M_CH9    |
 | bit  8  R/W  EGR_47B_M_CH8    |
 | bit  7  R/W  EGR_47B_M_CH7    |
 | bit  6  R/W  EGR_47B_M_CH6    |
 | bit  5  R/W  EGR_47B_M_CH5    |
 | bit  4  R/W  EGR_47B_M_CH4    |
 | bit  3  R/W  EGR_47B_M_CH3    |
 | bit  2  R/W  EGR_47B_M_CH2    |
 | bit  1  R/W  EGR_47B_M_CH1    |
 | bit  0  R/W  EGR_47B_M_CH0    |
 +------------------------------*/
#define PMC_ETIME120_REG_EGR_47B_M_UNUSED_MASK        0xfffff000
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH11_MSK 0x00000800
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH11_OFF 11
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH10_MSK 0x00000400
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH10_OFF 10
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH9_MSK  0x00000200
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH9_OFF  9
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH8_MSK  0x00000100
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH8_OFF  8
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH7_MSK  0x00000080
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH7_OFF  7
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH6_MSK  0x00000040
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH6_OFF  6
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH5_MSK  0x00000020
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH5_OFF  5
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH4_MSK  0x00000010
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH4_OFF  4
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH3_MSK  0x00000008
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH3_OFF  3
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH2_MSK  0x00000004
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH2_OFF  2
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH1_MSK  0x00000002
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH1_OFF  1
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH0_MSK  0x00000001
#define ETIME120_REG_EGR_47B_M_BIT_EGR_47B_M_CH0_OFF  0

/*------------------------------------.
 | Register 0x00000074 ING_TS_ENA_REG |
 +------------------------------------+
 | bit  0 R/W  RX_TS_ENA              |
 +-----------------------------------*/
#define PMC_ETIME120_REG_ING_TS_ENA_REG_UNUSED_MASK   0xfffffffe
#define ETIME120_REG_ING_TS_ENA_REG_BIT_RX_TS_ENA_MSK 0x00000001
#define ETIME120_REG_ING_TS_ENA_REG_BIT_RX_TS_ENA_OFF 0

/*-------------------------------.
 | Register 0x00000078 REFL_DEST |
 +-------------------------------+
 | bit  11 R/W  REFL_DEST_CH11   |
 | bit  10 R/W  REFL_DEST_CH10   |
 | bit  9  R/W  REFL_DEST_CH9    |
 | bit  8  R/W  REFL_DEST_CH8    |
 | bit  7  R/W  REFL_DEST_CH7    |
 | bit  6  R/W  REFL_DEST_CH6    |
 | bit  5  R/W  REFL_DEST_CH5    |
 | bit  4  R/W  REFL_DEST_CH4    |
 | bit  3  R/W  REFL_DEST_CH3    |
 | bit  2  R/W  REFL_DEST_CH2    |
 | bit  1  R/W  REFL_DEST_CH1    |
 | bit  0  R/W  REFL_DEST_CH0    |
 +------------------------------*/
#define PMC_ETIME120_REG_REFL_DEST_UNUSED_MASK        0xfffff000
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH11_MSK 0x00000800
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH11_OFF 11
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH10_MSK 0x00000400
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH10_OFF 10
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH9_MSK  0x00000200
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH9_OFF  9
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH8_MSK  0x00000100
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH8_OFF  8
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH7_MSK  0x00000080
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH7_OFF  7
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH6_MSK  0x00000040
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH6_OFF  6
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH5_MSK  0x00000020
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH5_OFF  5
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH4_MSK  0x00000010
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH4_OFF  4
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH3_MSK  0x00000008
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH3_OFF  3
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH2_MSK  0x00000004
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH2_OFF  2
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH1_MSK  0x00000002
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH1_OFF  1
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH0_MSK  0x00000001
#define ETIME120_REG_REFL_DEST_BIT_REFL_DEST_CH0_OFF  0

/*------------------------------------.
 | Register 0x0000007c EGR_TS_ENA_REG |
 +------------------------------------+
 | bit  11 R/W  TX_TS_ENA_LINK11      |
 | bit  10 R/W  TX_TS_ENA_LINK10      |
 | bit  9  R/W  TX_TS_ENA_LINK9       |
 | bit  8  R/W  TX_TS_ENA_LINK8       |
 | bit  7  R/W  TX_TS_ENA_LINK7       |
 | bit  6  R/W  TX_TS_ENA_LINK6       |
 | bit  5  R/W  TX_TS_ENA_LINK5       |
 | bit  4  R/W  TX_TS_ENA_LINK4       |
 | bit  3  R/W  TX_TS_ENA_LINK3       |
 | bit  2  R/W  TX_TS_ENA_LINK2       |
 | bit  1  R/W  TX_TS_ENA_LINK1       |
 | bit  0  R/W  TX_TS_ENA_LINK0       |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EGR_TS_ENA_REG_UNUSED_MASK          0xfffff000
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK11_MSK 0x00000800
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK11_OFF 11
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK10_MSK 0x00000400
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK10_OFF 10
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK9_MSK  0x00000200
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK9_OFF  9
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK8_MSK  0x00000100
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK8_OFF  8
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK7_MSK  0x00000080
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK7_OFF  7
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK6_MSK  0x00000040
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK6_OFF  6
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK5_MSK  0x00000020
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK5_OFF  5
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK4_MSK  0x00000010
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK4_OFF  4
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK3_MSK  0x00000008
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK3_OFF  3
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK2_MSK  0x00000004
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK2_OFF  2
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK1_MSK  0x00000002
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK1_OFF  1
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK0_MSK  0x00000001
#define ETIME120_REG_EGR_TS_ENA_REG_BIT_TX_TS_ENA_LINK0_OFF  0

/*-------------------------------------.
 | Register 0x00000080 TOD_OFFS_SEC_HI |
 +-------------------------------------+
 | bit  18:16 R/W  OFFS_TS_PAGE        |
 | bit  15:0  R/W  TOD_OFFS_SEC_HI     |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_OFFS_SEC_HI_UNUSED_MASK         0xfff80000
#define ETIME120_REG_TOD_OFFS_SEC_HI_BIT_OFFS_TS_PAGE_MSK    0x00070000
#define ETIME120_REG_TOD_OFFS_SEC_HI_BIT_OFFS_TS_PAGE_OFF    16
#define ETIME120_REG_TOD_OFFS_SEC_HI_BIT_TOD_OFFS_SEC_HI_MSK 0x0000ffff
#define ETIME120_REG_TOD_OFFS_SEC_HI_BIT_TOD_OFFS_SEC_HI_OFF 0

/*-------------------------------------.
 | Register 0x00000084 TOD_OFFS_SEC_LO |
 +-------------------------------------+
 | bit  31:0 R/W  TOD_OFFS_SEC_LO      |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_OFFS_SEC_LO_UNUSED_MASK         0x00000000
#define ETIME120_REG_TOD_OFFS_SEC_LO_BIT_TOD_OFFS_SEC_LO_MSK 0xffffffff
#define ETIME120_REG_TOD_OFFS_SEC_LO_BIT_TOD_OFFS_SEC_LO_OFF 0

/*-----------------------------------.
 | Register 0x00000088 TOD_OFFS_NSEC |
 +-----------------------------------+
 | bit  31:0 R/W  TOD_OFFS_NSEC      |
 +----------------------------------*/
#define PMC_ETIME120_REG_TOD_OFFS_NSEC_UNUSED_MASK       0x00000000
#define ETIME120_REG_TOD_OFFS_NSEC_BIT_TOD_OFFS_NSEC_MSK 0xffffffff
#define ETIME120_REG_TOD_OFFS_NSEC_BIT_TOD_OFFS_NSEC_OFF 0

/*--------------------------------------.
 | Register 0x0000008c CFC_OFFS_NSEC_HI |
 +--------------------------------------+
 | bit  15:0 R/W  CFC_OFFS_NSEC_HI      |
 +-------------------------------------*/
#define PMC_ETIME120_REG_CFC_OFFS_NSEC_HI_UNUSED_MASK          0xffff0000
#define ETIME120_REG_CFC_OFFS_NSEC_HI_BIT_CFC_OFFS_NSEC_HI_MSK 0x0000ffff
#define ETIME120_REG_CFC_OFFS_NSEC_HI_BIT_CFC_OFFS_NSEC_HI_OFF 0

/*--------------------------------------.
 | Register 0x00000090 CFC_OFFS_NSEC_LO |
 +--------------------------------------+
 | bit  31:0 R/W  CFC_OFFS_NSEC_LO      |
 +-------------------------------------*/
#define PMC_ETIME120_REG_CFC_OFFS_NSEC_LO_UNUSED_MASK          0x00000000
#define ETIME120_REG_CFC_OFFS_NSEC_LO_BIT_CFC_OFFS_NSEC_LO_MSK 0xffffffff
#define ETIME120_REG_CFC_OFFS_NSEC_LO_BIT_CFC_OFFS_NSEC_LO_OFF 0

/*-------------------------------------.
 | Register 0x00000094 TOD_ING_DLY_CH0 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_0        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH0_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH0_BIT_TOD_ING_DLY_0_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH0_BIT_TOD_ING_DLY_0_OFF 0

/*-------------------------------------.
 | Register 0x00000098 TOD_ING_DLY_CH1 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_1        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH1_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH1_BIT_TOD_ING_DLY_1_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH1_BIT_TOD_ING_DLY_1_OFF 0

/*-------------------------------------.
 | Register 0x0000009c TOD_ING_DLY_CH2 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_2        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH2_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH2_BIT_TOD_ING_DLY_2_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH2_BIT_TOD_ING_DLY_2_OFF 0

/*-------------------------------------.
 | Register 0x000000a0 TOD_ING_DLY_CH3 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_3        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH3_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH3_BIT_TOD_ING_DLY_3_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH3_BIT_TOD_ING_DLY_3_OFF 0

/*-------------------------------------.
 | Register 0x000000a4 TOD_ING_DLY_CH4 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_4        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH4_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH4_BIT_TOD_ING_DLY_4_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH4_BIT_TOD_ING_DLY_4_OFF 0

/*-------------------------------------.
 | Register 0x000000a8 TOD_ING_DLY_CH5 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_5        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH5_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH5_BIT_TOD_ING_DLY_5_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH5_BIT_TOD_ING_DLY_5_OFF 0

/*-------------------------------------.
 | Register 0x000000ac TOD_ING_DLY_CH6 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_6        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH6_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH6_BIT_TOD_ING_DLY_6_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH6_BIT_TOD_ING_DLY_6_OFF 0

/*-------------------------------------.
 | Register 0x000000b0 TOD_ING_DLY_CH7 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_7        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH7_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH7_BIT_TOD_ING_DLY_7_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH7_BIT_TOD_ING_DLY_7_OFF 0

/*-------------------------------------.
 | Register 0x000000b4 TOD_ING_DLY_CH8 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_8        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH8_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH8_BIT_TOD_ING_DLY_8_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH8_BIT_TOD_ING_DLY_8_OFF 0

/*-------------------------------------.
 | Register 0x000000b8 TOD_ING_DLY_CH9 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_9        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH9_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH9_BIT_TOD_ING_DLY_9_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH9_BIT_TOD_ING_DLY_9_OFF 0

/*--------------------------------------.
 | Register 0x000000bc TOD_ING_DLY_CH10 |
 +--------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_10        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH10_UNUSED_MASK        0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH10_BIT_TOD_ING_DLY_10_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH10_BIT_TOD_ING_DLY_10_OFF 0

/*--------------------------------------.
 | Register 0x000000c0 TOD_ING_DLY_CH11 |
 +--------------------------------------+
 | bit  15:0 R/W  TOD_ING_DLY_11        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_TOD_ING_DLY_CH11_UNUSED_MASK        0xffff0000
#define ETIME120_REG_TOD_ING_DLY_CH11_BIT_TOD_ING_DLY_11_MSK 0x0000ffff
#define ETIME120_REG_TOD_ING_DLY_CH11_BIT_TOD_ING_DLY_11_OFF 0

/*-------------------------------------.
 | Register 0x000000c4 CFC_ING_DLY_CH0 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_0        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH0_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH0_BIT_CFC_ING_DLY_0_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH0_BIT_CFC_ING_DLY_0_OFF 0

/*-------------------------------------.
 | Register 0x000000c8 CFC_ING_DLY_CH1 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_1        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH1_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH1_BIT_CFC_ING_DLY_1_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH1_BIT_CFC_ING_DLY_1_OFF 0

/*-------------------------------------.
 | Register 0x000000cc CFC_ING_DLY_CH2 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_2        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH2_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH2_BIT_CFC_ING_DLY_2_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH2_BIT_CFC_ING_DLY_2_OFF 0

/*-------------------------------------.
 | Register 0x000000d0 CFC_ING_DLY_CH3 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_3        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH3_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH3_BIT_CFC_ING_DLY_3_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH3_BIT_CFC_ING_DLY_3_OFF 0

/*-------------------------------------.
 | Register 0x000000d4 CFC_ING_DLY_CH4 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_4        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH4_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH4_BIT_CFC_ING_DLY_4_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH4_BIT_CFC_ING_DLY_4_OFF 0

/*-------------------------------------.
 | Register 0x000000d8 CFC_ING_DLY_CH5 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_5        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH5_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH5_BIT_CFC_ING_DLY_5_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH5_BIT_CFC_ING_DLY_5_OFF 0

/*-------------------------------------.
 | Register 0x000000dc CFC_ING_DLY_CH6 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_6        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH6_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH6_BIT_CFC_ING_DLY_6_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH6_BIT_CFC_ING_DLY_6_OFF 0

/*-------------------------------------.
 | Register 0x000000e0 CFC_ING_DLY_CH7 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_7        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH7_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH7_BIT_CFC_ING_DLY_7_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH7_BIT_CFC_ING_DLY_7_OFF 0

/*-------------------------------------.
 | Register 0x000000e4 CFC_ING_DLY_CH8 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_8        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH8_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH8_BIT_CFC_ING_DLY_8_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH8_BIT_CFC_ING_DLY_8_OFF 0

/*-------------------------------------.
 | Register 0x000000e8 CFC_ING_DLY_CH9 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_9        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH9_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH9_BIT_CFC_ING_DLY_9_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH9_BIT_CFC_ING_DLY_9_OFF 0

/*--------------------------------------.
 | Register 0x000000ec CFC_ING_DLY_CH10 |
 +--------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_10        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH10_UNUSED_MASK        0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH10_BIT_CFC_ING_DLY_10_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH10_BIT_CFC_ING_DLY_10_OFF 0

/*--------------------------------------.
 | Register 0x000000f0 CFC_ING_DLY_CH11 |
 +--------------------------------------+
 | bit  15:0 R/W  CFC_ING_DLY_11        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_CFC_ING_DLY_CH11_UNUSED_MASK        0xffff0000
#define ETIME120_REG_CFC_ING_DLY_CH11_BIT_CFC_ING_DLY_11_MSK 0x0000ffff
#define ETIME120_REG_CFC_ING_DLY_CH11_BIT_CFC_ING_DLY_11_OFF 0

/*-------------------------------------.
 | Register 0x000000f4 TOD_EGR_DLY_CH0 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_0        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH0_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH0_BIT_TOD_EGR_DLY_0_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH0_BIT_TOD_EGR_DLY_0_OFF 0

/*-------------------------------------.
 | Register 0x000000f8 TOD_EGR_DLY_CH1 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_1        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH1_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH1_BIT_TOD_EGR_DLY_1_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH1_BIT_TOD_EGR_DLY_1_OFF 0

/*-------------------------------------.
 | Register 0x000000fc TOD_EGR_DLY_CH2 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_2        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH2_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH2_BIT_TOD_EGR_DLY_2_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH2_BIT_TOD_EGR_DLY_2_OFF 0

/*-------------------------------------.
 | Register 0x00000100 TOD_EGR_DLY_CH3 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_3        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH3_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH3_BIT_TOD_EGR_DLY_3_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH3_BIT_TOD_EGR_DLY_3_OFF 0

/*-------------------------------------.
 | Register 0x00000104 TOD_EGR_DLY_CH4 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_4        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH4_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH4_BIT_TOD_EGR_DLY_4_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH4_BIT_TOD_EGR_DLY_4_OFF 0

/*-------------------------------------.
 | Register 0x00000108 TOD_EGR_DLY_CH5 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_5        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH5_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH5_BIT_TOD_EGR_DLY_5_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH5_BIT_TOD_EGR_DLY_5_OFF 0

/*-------------------------------------.
 | Register 0x0000010c TOD_EGR_DLY_CH6 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_6        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH6_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH6_BIT_TOD_EGR_DLY_6_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH6_BIT_TOD_EGR_DLY_6_OFF 0

/*-------------------------------------.
 | Register 0x00000110 TOD_EGR_DLY_CH7 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_7        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH7_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH7_BIT_TOD_EGR_DLY_7_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH7_BIT_TOD_EGR_DLY_7_OFF 0

/*-------------------------------------.
 | Register 0x00000114 TOD_EGR_DLY_CH8 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_8        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH8_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH8_BIT_TOD_EGR_DLY_8_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH8_BIT_TOD_EGR_DLY_8_OFF 0

/*-------------------------------------.
 | Register 0x00000118 TOD_EGR_DLY_CH9 |
 +-------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_9        |
 +------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH9_UNUSED_MASK       0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH9_BIT_TOD_EGR_DLY_9_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH9_BIT_TOD_EGR_DLY_9_OFF 0

/*--------------------------------------.
 | Register 0x0000011c TOD_EGR_DLY_CH10 |
 +--------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_10        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH10_UNUSED_MASK        0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH10_BIT_TOD_EGR_DLY_10_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH10_BIT_TOD_EGR_DLY_10_OFF 0

/*--------------------------------------.
 | Register 0x00000120 TOD_EGR_DLY_CH11 |
 +--------------------------------------+
 | bit  15:0 R/W  TOD_EGR_DLY_11        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_TOD_EGR_DLY_CH11_UNUSED_MASK        0xffff0000
#define ETIME120_REG_TOD_EGR_DLY_CH11_BIT_TOD_EGR_DLY_11_MSK 0x0000ffff
#define ETIME120_REG_TOD_EGR_DLY_CH11_BIT_TOD_EGR_DLY_11_OFF 0

/*-------------------------------------.
 | Register 0x00000124 CFC_EGR_DLY_CH0 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_0        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH0_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH0_BIT_CFC_EGR_DLY_0_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH0_BIT_CFC_EGR_DLY_0_OFF 0

/*-------------------------------------.
 | Register 0x00000128 CFC_EGR_DLY_CH1 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_1        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH1_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH1_BIT_CFC_EGR_DLY_1_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH1_BIT_CFC_EGR_DLY_1_OFF 0

/*-------------------------------------.
 | Register 0x0000012c CFC_EGR_DLY_CH2 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_2        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH2_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH2_BIT_CFC_EGR_DLY_2_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH2_BIT_CFC_EGR_DLY_2_OFF 0

/*-------------------------------------.
 | Register 0x00000130 CFC_EGR_DLY_CH3 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_3        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH3_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH3_BIT_CFC_EGR_DLY_3_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH3_BIT_CFC_EGR_DLY_3_OFF 0

/*-------------------------------------.
 | Register 0x00000134 CFC_EGR_DLY_CH4 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_4        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH4_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH4_BIT_CFC_EGR_DLY_4_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH4_BIT_CFC_EGR_DLY_4_OFF 0

/*-------------------------------------.
 | Register 0x00000138 CFC_EGR_DLY_CH5 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_5        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH5_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH5_BIT_CFC_EGR_DLY_5_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH5_BIT_CFC_EGR_DLY_5_OFF 0

/*-------------------------------------.
 | Register 0x0000013c CFC_EGR_DLY_CH6 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_6        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH6_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH6_BIT_CFC_EGR_DLY_6_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH6_BIT_CFC_EGR_DLY_6_OFF 0

/*-------------------------------------.
 | Register 0x00000140 CFC_EGR_DLY_CH7 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_7        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH7_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH7_BIT_CFC_EGR_DLY_7_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH7_BIT_CFC_EGR_DLY_7_OFF 0

/*-------------------------------------.
 | Register 0x00000144 CFC_EGR_DLY_CH8 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_8        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH8_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH8_BIT_CFC_EGR_DLY_8_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH8_BIT_CFC_EGR_DLY_8_OFF 0

/*-------------------------------------.
 | Register 0x00000148 CFC_EGR_DLY_CH9 |
 +-------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_9        |
 +------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH9_UNUSED_MASK       0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH9_BIT_CFC_EGR_DLY_9_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH9_BIT_CFC_EGR_DLY_9_OFF 0

/*--------------------------------------.
 | Register 0x0000014c CFC_EGR_DLY_CH10 |
 +--------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_10        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH10_UNUSED_MASK        0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH10_BIT_CFC_EGR_DLY_10_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH10_BIT_CFC_EGR_DLY_10_OFF 0

/*--------------------------------------.
 | Register 0x00000150 CFC_EGR_DLY_CH11 |
 +--------------------------------------+
 | bit  15:0 R/W  CFC_EGR_DLY_11        |
 +-------------------------------------*/
#define PMC_ETIME120_REG_CFC_EGR_DLY_CH11_UNUSED_MASK        0xffff0000
#define ETIME120_REG_CFC_EGR_DLY_CH11_BIT_CFC_EGR_DLY_11_MSK 0x0000ffff
#define ETIME120_REG_CFC_EGR_DLY_CH11_BIT_CFC_EGR_DLY_11_OFF 0

/*-------------------------------------.
 | Register 0x00000154 TS_CNTR_CFG     |
 +-------------------------------------+
 | bit  10  R/W  TS_ZERO_STRAP         |
 | bit  9   R/W  B2B_SUPPRESS_ENA      |
 | bit  8:7 R/W  MAC_SYNC_DLY          |
 | bit  6   R/W  IRIGO_CAPTURED_TS_SEL |
 | bit  5   R/W  IRIGI_CAPTURED_TS_SEL |
 | bit  4   R/W  CAPTURED_TS_SEL       |
 | bit  3:1 R/W  TS_ADV                |
 | bit  0   R/W  TS_CNTR_EN            |
 +------------------------------------*/
#define PMC_ETIME120_REG_TS_CNTR_CFG_UNUSED_MASK               0xfffff800
#define ETIME120_REG_TS_CNTR_CFG_BIT_TS_ZERO_STRAP_MSK         0x00000400
#define ETIME120_REG_TS_CNTR_CFG_BIT_TS_ZERO_STRAP_OFF         10
#define ETIME120_REG_TS_CNTR_CFG_BIT_B2B_SUPPRESS_ENA_MSK      0x00000200
#define ETIME120_REG_TS_CNTR_CFG_BIT_B2B_SUPPRESS_ENA_OFF      9
#define ETIME120_REG_TS_CNTR_CFG_BIT_MAC_SYNC_DLY_MSK          0x00000180
#define ETIME120_REG_TS_CNTR_CFG_BIT_MAC_SYNC_DLY_OFF          7
#define ETIME120_REG_TS_CNTR_CFG_BIT_IRIGO_CAPTURED_TS_SEL_MSK 0x00000040
#define ETIME120_REG_TS_CNTR_CFG_BIT_IRIGO_CAPTURED_TS_SEL_OFF 6
#define ETIME120_REG_TS_CNTR_CFG_BIT_IRIGI_CAPTURED_TS_SEL_MSK 0x00000020
#define ETIME120_REG_TS_CNTR_CFG_BIT_IRIGI_CAPTURED_TS_SEL_OFF 5
#define ETIME120_REG_TS_CNTR_CFG_BIT_CAPTURED_TS_SEL_MSK       0x00000010
#define ETIME120_REG_TS_CNTR_CFG_BIT_CAPTURED_TS_SEL_OFF       4
#define ETIME120_REG_TS_CNTR_CFG_BIT_TS_ADV_MSK                0x0000000e
#define ETIME120_REG_TS_CNTR_CFG_BIT_TS_ADV_OFF                1
#define ETIME120_REG_TS_CNTR_CFG_BIT_TS_CNTR_EN_MSK            0x00000001
#define ETIME120_REG_TS_CNTR_CFG_BIT_TS_CNTR_EN_OFF            0

/*----------------------------------------------.
 | Register 0x00000158 OFFS_UPD_TIME_SEC_HI_REG |
 +----------------------------------------------+
 | bit  15:0 R/W  OFFS_UPD_TIME_SEC_HI          |
 +---------------------------------------------*/
#define PMC_ETIME120_REG_OFFS_UPD_TIME_SEC_HI_REG_UNUSED_MASK              0xffff0000
#define ETIME120_REG_OFFS_UPD_TIME_SEC_HI_REG_BIT_OFFS_UPD_TIME_SEC_HI_MSK 0x0000ffff
#define ETIME120_REG_OFFS_UPD_TIME_SEC_HI_REG_BIT_OFFS_UPD_TIME_SEC_HI_OFF 0

/*----------------------------------------------.
 | Register 0x0000015c OFFS_UPD_TIME_SEC_LO_REG |
 +----------------------------------------------+
 | bit  31:0 R/W  OFFS_UPD_TIME_SEC_LO          |
 +---------------------------------------------*/
#define PMC_ETIME120_REG_OFFS_UPD_TIME_SEC_LO_REG_UNUSED_MASK              0x00000000
#define ETIME120_REG_OFFS_UPD_TIME_SEC_LO_REG_BIT_OFFS_UPD_TIME_SEC_LO_MSK 0xffffffff
#define ETIME120_REG_OFFS_UPD_TIME_SEC_LO_REG_BIT_OFFS_UPD_TIME_SEC_LO_OFF 0

/*--------------------------------------------.
 | Register 0x00000160 OFFS_UPD_TIME_NSEC_REG |
 +--------------------------------------------+
 | bit  31:0 R/W  OFFS_UPD_TIME_NSEC          |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_OFFS_UPD_TIME_NSEC_REG_UNUSED_MASK            0x00000000
#define ETIME120_REG_OFFS_UPD_TIME_NSEC_REG_BIT_OFFS_UPD_TIME_NSEC_MSK 0xffffffff
#define ETIME120_REG_OFFS_UPD_TIME_NSEC_REG_BIT_OFFS_UPD_TIME_NSEC_OFF 0

/*--------------------------------------------.
 | Register 0x00000164 PTP_CLK_PER_INT_HI_REG |
 +--------------------------------------------+
 | bit  31:0 R/W  PTP_CLK_PER_INT_HI          |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_PTP_CLK_PER_INT_HI_REG_UNUSED_MASK            0x00000000
#define ETIME120_REG_PTP_CLK_PER_INT_HI_REG_BIT_PTP_CLK_PER_INT_HI_MSK 0xffffffff
#define ETIME120_REG_PTP_CLK_PER_INT_HI_REG_BIT_PTP_CLK_PER_INT_HI_OFF 0

/*--------------------------------------------.
 | Register 0x00000168 PTP_CLK_PER_INT_LO_REG |
 +--------------------------------------------+
 | bit  31:0 R/W  PTP_CLK_PER_INT_LO          |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_PTP_CLK_PER_INT_LO_REG_UNUSED_MASK            0x00000000
#define ETIME120_REG_PTP_CLK_PER_INT_LO_REG_BIT_PTP_CLK_PER_INT_LO_MSK 0xffffffff
#define ETIME120_REG_PTP_CLK_PER_INT_LO_REG_BIT_PTP_CLK_PER_INT_LO_OFF 0

/*-----------------------------------------.
 | Register 0x0000016c PTP_CLK_PER_NUM_REG |
 +-----------------------------------------+
 | bit  31:0 R/W  PTP_CLK_PER_NUM          |
 +----------------------------------------*/
#define PMC_ETIME120_REG_PTP_CLK_PER_NUM_REG_UNUSED_MASK         0x00000000
#define ETIME120_REG_PTP_CLK_PER_NUM_REG_BIT_PTP_CLK_PER_NUM_MSK 0xffffffff
#define ETIME120_REG_PTP_CLK_PER_NUM_REG_BIT_PTP_CLK_PER_NUM_OFF 0

/*-----------------------------------------.
 | Register 0x00000170 PTP_CLK_PER_DEN_REG |
 +-----------------------------------------+
 | bit  31:0 R/W  PTP_CLK_PER_DEN          |
 +----------------------------------------*/
#define PMC_ETIME120_REG_PTP_CLK_PER_DEN_REG_UNUSED_MASK         0x00000000
#define ETIME120_REG_PTP_CLK_PER_DEN_REG_BIT_PTP_CLK_PER_DEN_MSK 0xffffffff
#define ETIME120_REG_PTP_CLK_PER_DEN_REG_BIT_PTP_CLK_PER_DEN_OFF 0

/*-------------------------------------------.
 | Register 0x00000174 OFFSET_UPD_QUANTA_REG |
 +-------------------------------------------+
 | bit  29:0 R/W  OFFSET_UPD_QUANTA          |
 +------------------------------------------*/
#define PMC_ETIME120_REG_OFFSET_UPD_QUANTA_REG_UNUSED_MASK           0xc0000000
#define ETIME120_REG_OFFSET_UPD_QUANTA_REG_BIT_OFFSET_UPD_QUANTA_MSK 0x3fffffff
#define ETIME120_REG_OFFSET_UPD_QUANTA_REG_BIT_OFFSET_UPD_QUANTA_OFF 0

/*---------------------------------------------.
 | Register 0x00000178 LINE_CLK_PER_INT_HI_REG |
 +---------------------------------------------+
 | bit  31:0 R/W  LINE_CLK_PER_INT_HI          |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_LINE_CLK_PER_INT_HI_REG_UNUSED_MASK             0x00000000
#define ETIME120_REG_LINE_CLK_PER_INT_HI_REG_BIT_LINE_CLK_PER_INT_HI_MSK 0xffffffff
#define ETIME120_REG_LINE_CLK_PER_INT_HI_REG_BIT_LINE_CLK_PER_INT_HI_OFF 0

/*---------------------------------------------.
 | Register 0x0000017c LINE_CLK_PER_INT_LO_REG |
 +---------------------------------------------+
 | bit  31:0 R/W  LINE_CLK_PER_INT_LO          |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_LINE_CLK_PER_INT_LO_REG_UNUSED_MASK             0x00000000
#define ETIME120_REG_LINE_CLK_PER_INT_LO_REG_BIT_LINE_CLK_PER_INT_LO_MSK 0xffffffff
#define ETIME120_REG_LINE_CLK_PER_INT_LO_REG_BIT_LINE_CLK_PER_INT_LO_OFF 0

/*------------------------------------------.
 | Register 0x00000180 LINE_CLK_PER_NUM_REG |
 +------------------------------------------+
 | bit  31:0 R/W  LINE_CLK_PER_NUM          |
 +-----------------------------------------*/
#define PMC_ETIME120_REG_LINE_CLK_PER_NUM_REG_UNUSED_MASK          0x00000000
#define ETIME120_REG_LINE_CLK_PER_NUM_REG_BIT_LINE_CLK_PER_NUM_MSK 0xffffffff
#define ETIME120_REG_LINE_CLK_PER_NUM_REG_BIT_LINE_CLK_PER_NUM_OFF 0

/*------------------------------------------.
 | Register 0x00000184 LINE_CLK_PER_DEN_REG |
 +------------------------------------------+
 | bit  31:0 R/W  LINE_CLK_PER_DEN          |
 +-----------------------------------------*/
#define PMC_ETIME120_REG_LINE_CLK_PER_DEN_REG_UNUSED_MASK          0x00000000
#define ETIME120_REG_LINE_CLK_PER_DEN_REG_BIT_LINE_CLK_PER_DEN_MSK 0xffffffff
#define ETIME120_REG_LINE_CLK_PER_DEN_REG_BIT_LINE_CLK_PER_DEN_OFF 0

/*------------------------------------------.
 | Register 0x00000188 PTP_CNT              |
 +------------------------------------------+
 | bit  31:30 R  LINE_REF_PTP_SEC_CNTR_FRC  |
 | bit  29:0  R  LINE_REF_PTP_NSEC_CNTR_FRC |
 +-----------------------------------------*/
#define PMC_ETIME120_REG_PTP_CNT_UNUSED_MASK                    0x00000000
#define ETIME120_REG_PTP_CNT_BIT_LINE_REF_PTP_SEC_CNTR_FRC_MSK  0xc0000000
#define ETIME120_REG_PTP_CNT_BIT_LINE_REF_PTP_SEC_CNTR_FRC_OFF  30
#define ETIME120_REG_PTP_CNT_BIT_LINE_REF_PTP_NSEC_CNTR_FRC_MSK 0x3fffffff
#define ETIME120_REG_PTP_CNT_BIT_LINE_REF_PTP_NSEC_CNTR_FRC_OFF 0

/*--------------------------------------.
 | Register 0x0000018c LINE_REF_CNT     |
 +--------------------------------------+
 | bit  31:30 R  LINE_REF_SEC_CNTR_FRC  |
 | bit  29:0  R  LINE_REF_NSEC_CNTR_FRC |
 +-------------------------------------*/
#define PMC_ETIME120_REG_LINE_REF_CNT_UNUSED_MASK                0x00000000
#define ETIME120_REG_LINE_REF_CNT_BIT_LINE_REF_SEC_CNTR_FRC_MSK  0xc0000000
#define ETIME120_REG_LINE_REF_CNT_BIT_LINE_REF_SEC_CNTR_FRC_OFF  30
#define ETIME120_REG_LINE_REF_CNT_BIT_LINE_REF_NSEC_CNTR_FRC_MSK 0x3fffffff
#define ETIME120_REG_LINE_REF_CNT_BIT_LINE_REF_NSEC_CNTR_FRC_OFF 0

/*-----------------------------------------------.
 | Register 0x00000190 TOD_TS_OUT_EXT_SEC_HI_REG |
 +-----------------------------------------------+
 | bit  18:16 R  TOD_TS_OUT_EXT_TS_PAGE          |
 | bit  15:0  R  TOD_TS_OUT_EXT_SEC_HI           |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_EXT_SEC_HI_REG_UNUSED_MASK                0xfff80000
#define ETIME120_REG_TOD_TS_OUT_EXT_SEC_HI_REG_BIT_TOD_TS_OUT_EXT_TS_PAGE_MSK 0x00070000
#define ETIME120_REG_TOD_TS_OUT_EXT_SEC_HI_REG_BIT_TOD_TS_OUT_EXT_TS_PAGE_OFF 16
#define ETIME120_REG_TOD_TS_OUT_EXT_SEC_HI_REG_BIT_TOD_TS_OUT_EXT_SEC_HI_MSK  0x0000ffff
#define ETIME120_REG_TOD_TS_OUT_EXT_SEC_HI_REG_BIT_TOD_TS_OUT_EXT_SEC_HI_OFF  0

/*-----------------------------------------------.
 | Register 0x00000194 TOD_TS_OUT_EXT_SEC_LO_REG |
 +-----------------------------------------------+
 | bit  31:0 R  TOD_TS_OUT_EXT_SEC_LO            |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_EXT_SEC_LO_REG_UNUSED_MASK               0x00000000
#define ETIME120_REG_TOD_TS_OUT_EXT_SEC_LO_REG_BIT_TOD_TS_OUT_EXT_SEC_LO_MSK 0xffffffff
#define ETIME120_REG_TOD_TS_OUT_EXT_SEC_LO_REG_BIT_TOD_TS_OUT_EXT_SEC_LO_OFF 0

/*---------------------------------------------.
 | Register 0x00000198 TOD_TS_OUT_EXT_NSEC_REG |
 +---------------------------------------------+
 | bit  31:0 R  TOD_TS_OUT_EXT_NSEC            |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_EXT_NSEC_REG_UNUSED_MASK             0x00000000
#define ETIME120_REG_TOD_TS_OUT_EXT_NSEC_REG_BIT_TOD_TS_OUT_EXT_NSEC_MSK 0xffffffff
#define ETIME120_REG_TOD_TS_OUT_EXT_NSEC_REG_BIT_TOD_TS_OUT_EXT_NSEC_OFF 0

/*-------------------------------------------.
 | Register 0x0000019c CFC_TS_OUT_EXT_HI_REG |
 +-------------------------------------------+
 | bit  15:0 R  CFC_TS_OUT_EXT_HI            |
 +------------------------------------------*/
#define PMC_ETIME120_REG_CFC_TS_OUT_EXT_HI_REG_UNUSED_MASK           0xffff0000
#define ETIME120_REG_CFC_TS_OUT_EXT_HI_REG_BIT_CFC_TS_OUT_EXT_HI_MSK 0x0000ffff
#define ETIME120_REG_CFC_TS_OUT_EXT_HI_REG_BIT_CFC_TS_OUT_EXT_HI_OFF 0

/*-------------------------------------------.
 | Register 0x000001a0 CFC_TS_OUT_EXT_LO_REG |
 +-------------------------------------------+
 | bit  31:0 R  CFC_TS_OUT_EXT_LO            |
 +------------------------------------------*/
#define PMC_ETIME120_REG_CFC_TS_OUT_EXT_LO_REG_UNUSED_MASK           0x00000000
#define ETIME120_REG_CFC_TS_OUT_EXT_LO_REG_BIT_CFC_TS_OUT_EXT_LO_MSK 0xffffffff
#define ETIME120_REG_CFC_TS_OUT_EXT_LO_REG_BIT_CFC_TS_OUT_EXT_LO_OFF 0

/*-------------------------------------------------.
 | Register 0x000001a4 TOD_TS_OUT_IRIGI_SEC_HI_REG |
 +-------------------------------------------------+
 | bit  18:16 R  TS_OUT_IRIGI_TS_PAGE              |
 | bit  15:0  R  TOD_TS_OUT_IRIGI_SEC_HI           |
 +------------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_HI_REG_UNUSED_MASK                 0xfff80000
#define ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_HI_REG_BIT_TS_OUT_IRIGI_TS_PAGE_MSK    0x00070000
#define ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_HI_REG_BIT_TS_OUT_IRIGI_TS_PAGE_OFF    16
#define ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_HI_REG_BIT_TOD_TS_OUT_IRIGI_SEC_HI_MSK 0x0000ffff
#define ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_HI_REG_BIT_TOD_TS_OUT_IRIGI_SEC_HI_OFF 0

/*-------------------------------------------------.
 | Register 0x000001a8 TOD_TS_OUT_IRIGI_SEC_LO_REG |
 +-------------------------------------------------+
 | bit  31:0 R  TOD_TS_OUT_IRIGI_SEC_LO            |
 +------------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_LO_REG_UNUSED_MASK                 0x00000000
#define ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_LO_REG_BIT_TOD_TS_OUT_IRIGI_SEC_LO_MSK 0xffffffff
#define ETIME120_REG_TOD_TS_OUT_IRIGI_SEC_LO_REG_BIT_TOD_TS_OUT_IRIGI_SEC_LO_OFF 0

/*-----------------------------------------------.
 | Register 0x000001ac TOD_TS_OUT_IRIGI_NSEC_REG |
 +-----------------------------------------------+
 | bit  31:0 R  TOD_TS_OUT_IRIGI_NSEC            |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGI_NSEC_REG_UNUSED_MASK               0x00000000
#define ETIME120_REG_TOD_TS_OUT_IRIGI_NSEC_REG_BIT_TOD_TS_OUT_IRIGI_NSEC_MSK 0xffffffff
#define ETIME120_REG_TOD_TS_OUT_IRIGI_NSEC_REG_BIT_TOD_TS_OUT_IRIGI_NSEC_OFF 0

/*---------------------------------------------.
 | Register 0x000001b0 CFC_TS_OUT_IRIGI_HI_REG |
 +---------------------------------------------+
 | bit  15:0 R  CFC_TS_OUT_IRIGI_HI            |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGI_HI_REG_UNUSED_MASK             0xffff0000
#define ETIME120_REG_CFC_TS_OUT_IRIGI_HI_REG_BIT_CFC_TS_OUT_IRIGI_HI_MSK 0x0000ffff
#define ETIME120_REG_CFC_TS_OUT_IRIGI_HI_REG_BIT_CFC_TS_OUT_IRIGI_HI_OFF 0

/*---------------------------------------------.
 | Register 0x000001b4 CFC_TS_OUT_IRIGI_LO_REG |
 +---------------------------------------------+
 | bit  31:0 R  CFC_TS_OUT_IRIGI_LO            |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGI_LO_REG_UNUSED_MASK             0x00000000
#define ETIME120_REG_CFC_TS_OUT_IRIGI_LO_REG_BIT_CFC_TS_OUT_IRIGI_LO_MSK 0xffffffff
#define ETIME120_REG_CFC_TS_OUT_IRIGI_LO_REG_BIT_CFC_TS_OUT_IRIGI_LO_OFF 0

/*-------------------------------------------------.
 | Register 0x000001b8 TOD_TS_OUT_IRIGO_SEC_HI_REG |
 +-------------------------------------------------+
 | bit  18:16 R  TS_OUT_IRIGO_TS_PAGE              |
 | bit  15:0  R  TOD_TS_OUT_IRIGO_SEC_HI           |
 +------------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_HI_REG_UNUSED_MASK                 0xfff80000
#define ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_HI_REG_BIT_TS_OUT_IRIGO_TS_PAGE_MSK    0x00070000
#define ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_HI_REG_BIT_TS_OUT_IRIGO_TS_PAGE_OFF    16
#define ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_HI_REG_BIT_TOD_TS_OUT_IRIGO_SEC_HI_MSK 0x0000ffff
#define ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_HI_REG_BIT_TOD_TS_OUT_IRIGO_SEC_HI_OFF 0

/*-------------------------------------------------.
 | Register 0x000001bc TOD_TS_OUT_IRIGO_SEC_LO_REG |
 +-------------------------------------------------+
 | bit  31:0 R  TOD_TS_OUT_IRIGO_SEC_LO            |
 +------------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_LO_REG_UNUSED_MASK                 0x00000000
#define ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_LO_REG_BIT_TOD_TS_OUT_IRIGO_SEC_LO_MSK 0xffffffff
#define ETIME120_REG_TOD_TS_OUT_IRIGO_SEC_LO_REG_BIT_TOD_TS_OUT_IRIGO_SEC_LO_OFF 0

/*-----------------------------------------------.
 | Register 0x000001c0 TOD_TS_OUT_IRIGO_NSEC_REG |
 +-----------------------------------------------+
 | bit  31:0 R  TOD_TS_OUT_IRIGO_NSEC            |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_TOD_TS_OUT_IRIGO_NSEC_REG_UNUSED_MASK               0x00000000
#define ETIME120_REG_TOD_TS_OUT_IRIGO_NSEC_REG_BIT_TOD_TS_OUT_IRIGO_NSEC_MSK 0xffffffff
#define ETIME120_REG_TOD_TS_OUT_IRIGO_NSEC_REG_BIT_TOD_TS_OUT_IRIGO_NSEC_OFF 0

/*---------------------------------------------.
 | Register 0x000001c4 CFC_TS_OUT_IRIGO_HI_REG |
 +---------------------------------------------+
 | bit  15:0 R  CFC_TS_OUT_IRIGO_HI            |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGO_HI_REG_UNUSED_MASK             0xffff0000
#define ETIME120_REG_CFC_TS_OUT_IRIGO_HI_REG_BIT_CFC_TS_OUT_IRIGO_HI_MSK 0x0000ffff
#define ETIME120_REG_CFC_TS_OUT_IRIGO_HI_REG_BIT_CFC_TS_OUT_IRIGO_HI_OFF 0

/*---------------------------------------------.
 | Register 0x000001c8 CFC_TS_OUT_IRIGO_LO_REG |
 +---------------------------------------------+
 | bit  31:0 R  CFC_TS_OUT_IRIGO_LO            |
 +--------------------------------------------*/
#define PMC_ETIME120_REG_CFC_TS_OUT_IRIGO_LO_REG_UNUSED_MASK             0x00000000
#define ETIME120_REG_CFC_TS_OUT_IRIGO_LO_REG_BIT_CFC_TS_OUT_IRIGO_LO_MSK 0xffffffff
#define ETIME120_REG_CFC_TS_OUT_IRIGO_LO_REG_BIT_CFC_TS_OUT_IRIGO_LO_OFF 0

/*----------------------------------------.
 | Register 0x000001cc TX_VEC_IRIGO_4_REG |
 +----------------------------------------+
 | bit  31:0 R/W  TX_VEC_IRIGO_4          |
 +---------------------------------------*/
#define PMC_ETIME120_REG_TX_VEC_IRIGO_4_REG_UNUSED_MASK        0x00000000
#define ETIME120_REG_TX_VEC_IRIGO_4_REG_BIT_TX_VEC_IRIGO_4_MSK 0xffffffff
#define ETIME120_REG_TX_VEC_IRIGO_4_REG_BIT_TX_VEC_IRIGO_4_OFF 0

/*----------------------------------------.
 | Register 0x000001d0 TX_VEC_IRIGO_3_REG |
 +----------------------------------------+
 | bit  31:0 R/W  TX_VEC_IRIGO_3          |
 +---------------------------------------*/
#define PMC_ETIME120_REG_TX_VEC_IRIGO_3_REG_UNUSED_MASK        0x00000000
#define ETIME120_REG_TX_VEC_IRIGO_3_REG_BIT_TX_VEC_IRIGO_3_MSK 0xffffffff
#define ETIME120_REG_TX_VEC_IRIGO_3_REG_BIT_TX_VEC_IRIGO_3_OFF 0

/*----------------------------------------.
 | Register 0x000001d4 TX_VEC_IRIGO_2_REG |
 +----------------------------------------+
 | bit  31:0 R/W  TX_VEC_IRIGO_2          |
 +---------------------------------------*/
#define PMC_ETIME120_REG_TX_VEC_IRIGO_2_REG_UNUSED_MASK        0x00000000
#define ETIME120_REG_TX_VEC_IRIGO_2_REG_BIT_TX_VEC_IRIGO_2_MSK 0xffffffff
#define ETIME120_REG_TX_VEC_IRIGO_2_REG_BIT_TX_VEC_IRIGO_2_OFF 0

/*----------------------------------------.
 | Register 0x000001d8 TX_VEC_IRIGO_1_REG |
 +----------------------------------------+
 | bit  3:0 R/W  TX_VEC_IRIGO_1           |
 +---------------------------------------*/
#define PMC_ETIME120_REG_TX_VEC_IRIGO_1_REG_UNUSED_MASK        0xfffffff0
#define ETIME120_REG_TX_VEC_IRIGO_1_REG_BIT_TX_VEC_IRIGO_1_MSK 0x0000000f
#define ETIME120_REG_TX_VEC_IRIGO_1_REG_BIT_TX_VEC_IRIGO_1_OFF 0

/*----------------------------------------.
 | Register 0x000001dc RX_VEC_IRIGI_4_REG |
 +----------------------------------------+
 | bit  31:0 R  RX_VEC_IRIGI_4            |
 +---------------------------------------*/
#define PMC_ETIME120_REG_RX_VEC_IRIGI_4_REG_UNUSED_MASK        0x00000000
#define ETIME120_REG_RX_VEC_IRIGI_4_REG_BIT_RX_VEC_IRIGI_4_MSK 0xffffffff
#define ETIME120_REG_RX_VEC_IRIGI_4_REG_BIT_RX_VEC_IRIGI_4_OFF 0

/*----------------------------------------.
 | Register 0x000001e0 RX_VEC_IRIGI_3_REG |
 +----------------------------------------+
 | bit  31:0 R  RX_VEC_IRIGI_3            |
 +---------------------------------------*/
#define PMC_ETIME120_REG_RX_VEC_IRIGI_3_REG_UNUSED_MASK        0x00000000
#define ETIME120_REG_RX_VEC_IRIGI_3_REG_BIT_RX_VEC_IRIGI_3_MSK 0xffffffff
#define ETIME120_REG_RX_VEC_IRIGI_3_REG_BIT_RX_VEC_IRIGI_3_OFF 0

/*----------------------------------------.
 | Register 0x000001e4 RX_VEC_IRIGI_2_REG |
 +----------------------------------------+
 | bit  31:0 R  RX_VEC_IRIGI_2            |
 +---------------------------------------*/
#define PMC_ETIME120_REG_RX_VEC_IRIGI_2_REG_UNUSED_MASK        0x00000000
#define ETIME120_REG_RX_VEC_IRIGI_2_REG_BIT_RX_VEC_IRIGI_2_MSK 0xffffffff
#define ETIME120_REG_RX_VEC_IRIGI_2_REG_BIT_RX_VEC_IRIGI_2_OFF 0

/*----------------------------------------.
 | Register 0x000001e8 RX_VEC_IRIGI_1_REG |
 +----------------------------------------+
 | bit  3:0 R  RX_VEC_IRIGI_1             |
 +---------------------------------------*/
#define PMC_ETIME120_REG_RX_VEC_IRIGI_1_REG_UNUSED_MASK        0xfffffff0
#define ETIME120_REG_RX_VEC_IRIGI_1_REG_BIT_RX_VEC_IRIGI_1_MSK 0x0000000f
#define ETIME120_REG_RX_VEC_IRIGI_1_REG_BIT_RX_VEC_IRIGI_1_OFF 0

/*-----------------------------------.
 | Register 0x000001ec IRIGI_CFG     |
 +-----------------------------------+
 | bit  3   R/W  IRIGI_1PPS_DET_MODE |
 | bit  2:1 R/W  IRIGI_MODE          |
 | bit  0   R/W  IRIGI_ENA           |
 +----------------------------------*/
#define PMC_ETIME120_REG_IRIGI_CFG_UNUSED_MASK             0xfffffff0
#define ETIME120_REG_IRIGI_CFG_BIT_IRIGI_1PPS_DET_MODE_MSK 0x00000008
#define ETIME120_REG_IRIGI_CFG_BIT_IRIGI_1PPS_DET_MODE_OFF 3
#define ETIME120_REG_IRIGI_CFG_BIT_IRIGI_MODE_MSK          0x00000006
#define ETIME120_REG_IRIGI_CFG_BIT_IRIGI_MODE_OFF          1
#define ETIME120_REG_IRIGI_CFG_BIT_IRIGI_ENA_MSK           0x00000001
#define ETIME120_REG_IRIGI_CFG_BIT_IRIGI_ENA_OFF           0

/*-----------------------------------------.
 | Register 0x000001f0 IRIGI_PWM_ZERO_REG  |
 +-----------------------------------------+
 | bit  21:0 R/W  IRIGI_PWM_WIDTH_ZERO_CFG |
 +----------------------------------------*/
#define PMC_ETIME120_REG_IRIGI_PWM_ZERO_REG_UNUSED_MASK                  0xffc00000
#define ETIME120_REG_IRIGI_PWM_ZERO_REG_BIT_IRIGI_PWM_WIDTH_ZERO_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGI_PWM_ZERO_REG_BIT_IRIGI_PWM_WIDTH_ZERO_CFG_OFF 0

/*----------------------------------------.
 | Register 0x000001f4 IRIGI_PWM_ONE_REG  |
 +----------------------------------------+
 | bit  21:0 R/W  IRIGI_PWM_WIDTH_ONE_CFG |
 +---------------------------------------*/
#define PMC_ETIME120_REG_IRIGI_PWM_ONE_REG_UNUSED_MASK                 0xffc00000
#define ETIME120_REG_IRIGI_PWM_ONE_REG_BIT_IRIGI_PWM_WIDTH_ONE_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGI_PWM_ONE_REG_BIT_IRIGI_PWM_WIDTH_ONE_CFG_OFF 0

/*--------------------------------------------.
 | Register 0x000001f8 IRIGI_PWM_WIDTH_PX_REG |
 +--------------------------------------------+
 | bit  21:0 R/W  IRIGI_PWM_WIDTH_PX_CFG      |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_IRIGI_PWM_WIDTH_PX_REG_UNUSED_MASK                0xffc00000
#define ETIME120_REG_IRIGI_PWM_WIDTH_PX_REG_BIT_IRIGI_PWM_WIDTH_PX_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGI_PWM_WIDTH_PX_REG_BIT_IRIGI_PWM_WIDTH_PX_CFG_OFF 0

/*------------------------------------------------.
 | Register 0x000001fc IRIGI_PWM_MIN_DIFF_CFG_REG |
 +------------------------------------------------+
 | bit  20:0 R/W  IRIGI_PWM_MIN_DIFF_CFG          |
 +-----------------------------------------------*/
#define PMC_ETIME120_REG_IRIGI_PWM_MIN_DIFF_CFG_REG_UNUSED_MASK                0xffe00000
#define ETIME120_REG_IRIGI_PWM_MIN_DIFF_CFG_REG_BIT_IRIGI_PWM_MIN_DIFF_CFG_MSK 0x001fffff
#define ETIME120_REG_IRIGI_PWM_MIN_DIFF_CFG_REG_BIT_IRIGI_PWM_MIN_DIFF_CFG_OFF 0

/*------------------------------------------------.
 | Register 0x00000200 IRIGI_PWM_MAX_DIFF_CFG_REG |
 +------------------------------------------------+
 | bit  20:0 R/W  IRIGI_PWM_MAX_DIFF_CFG          |
 +-----------------------------------------------*/
#define PMC_ETIME120_REG_IRIGI_PWM_MAX_DIFF_CFG_REG_UNUSED_MASK                0xffe00000
#define ETIME120_REG_IRIGI_PWM_MAX_DIFF_CFG_REG_BIT_IRIGI_PWM_MAX_DIFF_CFG_MSK 0x001fffff
#define ETIME120_REG_IRIGI_PWM_MAX_DIFF_CFG_REG_BIT_IRIGI_PWM_MAX_DIFF_CFG_OFF 0

/*----------------------------------------------.
 | Register 0x00000204 IRIGI_DATI_IDLE_DURATION |
 +----------------------------------------------+
 | bit  27:0 R/W  IRIGI_DATI_IDLE_DURATION      |
 +---------------------------------------------*/
#define PMC_ETIME120_REG_IRIGI_DATI_IDLE_DURATION_UNUSED_MASK                  0xf0000000
#define ETIME120_REG_IRIGI_DATI_IDLE_DURATION_BIT_IRIGI_DATI_IDLE_DURATION_MSK 0x0fffffff
#define ETIME120_REG_IRIGI_DATI_IDLE_DURATION_BIT_IRIGI_DATI_IDLE_DURATION_OFF 0

/*-------------------------------.
 | Register 0x00000208 IRIGO_CFG |
 +-------------------------------+
 | bit  3   R/W  IRIGO_P0PR_ONLY |
 | bit  2:1 R/W  IRIGO_MODE      |
 | bit  0   R/W  IRIGO_ENA       |
 +------------------------------*/
#define PMC_ETIME120_REG_IRIGO_CFG_UNUSED_MASK         0xfffffff0
#define ETIME120_REG_IRIGO_CFG_BIT_IRIGO_P0PR_ONLY_MSK 0x00000008
#define ETIME120_REG_IRIGO_CFG_BIT_IRIGO_P0PR_ONLY_OFF 3
#define ETIME120_REG_IRIGO_CFG_BIT_IRIGO_MODE_MSK      0x00000006
#define ETIME120_REG_IRIGO_CFG_BIT_IRIGO_MODE_OFF      1
#define ETIME120_REG_IRIGO_CFG_BIT_IRIGO_ENA_MSK       0x00000001
#define ETIME120_REG_IRIGO_CFG_BIT_IRIGO_ENA_OFF       0

/*-----------------------------------------------.
 | Register 0x0000020c IRIGO_PWM_NOM_PER_CFG_REG |
 +-----------------------------------------------+
 | bit  21:0 R/W  IRIGO_PWM_NOM_PER_CFG          |
 +----------------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_PWM_NOM_PER_CFG_REG_UNUSED_MASK               0xffc00000
#define ETIME120_REG_IRIGO_PWM_NOM_PER_CFG_REG_BIT_IRIGO_PWM_NOM_PER_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGO_PWM_NOM_PER_CFG_REG_BIT_IRIGO_PWM_NOM_PER_CFG_OFF 0

/*----------------------------------------.
 | Register 0x00000210 IRIGO_PWM_ONE_REG  |
 +----------------------------------------+
 | bit  21:0 R/W  IRIGO_PWM_WIDTH_ONE_CFG |
 +---------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_PWM_ONE_REG_UNUSED_MASK                 0xffc00000
#define ETIME120_REG_IRIGO_PWM_ONE_REG_BIT_IRIGO_PWM_WIDTH_ONE_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGO_PWM_ONE_REG_BIT_IRIGO_PWM_WIDTH_ONE_CFG_OFF 0

/*-----------------------------------------.
 | Register 0x00000214 IRIGO_PWM_ZERO_REG  |
 +-----------------------------------------+
 | bit  21:0 R/W  IRIGO_PWM_WIDTH_ZERO_CFG |
 +----------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_PWM_ZERO_REG_UNUSED_MASK                  0xffc00000
#define ETIME120_REG_IRIGO_PWM_ZERO_REG_BIT_IRIGO_PWM_WIDTH_ZERO_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGO_PWM_ZERO_REG_BIT_IRIGO_PWM_WIDTH_ZERO_CFG_OFF 0

/*--------------------------------------------.
 | Register 0x00000218 IRIGO_PWM_WIDTH_PX_REG |
 +--------------------------------------------+
 | bit  21:0 R/W  IRIGO_PWM_WIDTH_PX_CFG      |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_PWM_WIDTH_PX_REG_UNUSED_MASK                0xffc00000
#define ETIME120_REG_IRIGO_PWM_WIDTH_PX_REG_BIT_IRIGO_PWM_WIDTH_PX_CFG_MSK 0x003fffff
#define ETIME120_REG_IRIGO_PWM_WIDTH_PX_REG_BIT_IRIGO_PWM_WIDTH_PX_CFG_OFF 0

/*----------------------------------------------.
 | Register 0x0000021c IRIGO_PWM_1PPS_WIDTH_REG |
 +----------------------------------------------+
 | bit  24:0 R/W  IRIGO_PWM_1PPS_WIDTH_CFG      |
 +---------------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_PWM_1PPS_WIDTH_REG_UNUSED_MASK                  0xfe000000
#define ETIME120_REG_IRIGO_PWM_1PPS_WIDTH_REG_BIT_IRIGO_PWM_1PPS_WIDTH_CFG_MSK 0x01ffffff
#define ETIME120_REG_IRIGO_PWM_1PPS_WIDTH_REG_BIT_IRIGO_PWM_1PPS_WIDTH_CFG_OFF 0

/*---------------------------------.
 | Register 0x00000220 INT_GEN_CFG |
 +---------------------------------+
 | bit  11 R/W  CC_INT_4_RAND_EN   |
 | bit  10 R/W  CC_INT_3_RAND_EN   |
 | bit  9  R/W  CC_INT_2_RAND_EN   |
 | bit  8  R/W  CC_INT_1_RAND_EN   |
 | bit  7  R/W  CC_INT_4_EN        |
 | bit  6  R/W  CC_INT_3_EN        |
 | bit  5  R/W  CC_INT_2_EN        |
 | bit  4  R/W  CC_INT_1_EN        |
 | bit  3  R/W  TOD_INT_2_EN       |
 | bit  2  R/W  TOD_INT_1_EN       |
 | bit  1  R/W  FREE_RUN_INT_2_EN  |
 | bit  0  R/W  FREE_RUN_INT_1_EN  |
 +--------------------------------*/
#define PMC_ETIME120_REG_INT_GEN_CFG_UNUSED_MASK           0xfffff000
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_4_RAND_EN_MSK  0x00000800
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_4_RAND_EN_OFF  11
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_3_RAND_EN_MSK  0x00000400
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_3_RAND_EN_OFF  10
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_2_RAND_EN_MSK  0x00000200
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_2_RAND_EN_OFF  9
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_1_RAND_EN_MSK  0x00000100
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_1_RAND_EN_OFF  8
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_4_EN_MSK       0x00000080
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_4_EN_OFF       7
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_3_EN_MSK       0x00000040
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_3_EN_OFF       6
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_2_EN_MSK       0x00000020
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_2_EN_OFF       5
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_1_EN_MSK       0x00000010
#define ETIME120_REG_INT_GEN_CFG_BIT_CC_INT_1_EN_OFF       4
#define ETIME120_REG_INT_GEN_CFG_BIT_TOD_INT_2_EN_MSK      0x00000008
#define ETIME120_REG_INT_GEN_CFG_BIT_TOD_INT_2_EN_OFF      3
#define ETIME120_REG_INT_GEN_CFG_BIT_TOD_INT_1_EN_MSK      0x00000004
#define ETIME120_REG_INT_GEN_CFG_BIT_TOD_INT_1_EN_OFF      2
#define ETIME120_REG_INT_GEN_CFG_BIT_FREE_RUN_INT_2_EN_MSK 0x00000002
#define ETIME120_REG_INT_GEN_CFG_BIT_FREE_RUN_INT_2_EN_OFF 1
#define ETIME120_REG_INT_GEN_CFG_BIT_FREE_RUN_INT_1_EN_MSK 0x00000001
#define ETIME120_REG_INT_GEN_CFG_BIT_FREE_RUN_INT_1_EN_OFF 0

/*------------------------------------.
 | Register 0x00000224 INT_PER_CFG    |
 +------------------------------------+
 | bit  31:24 R/W  TOD_INT_PER_2      |
 | bit  23:16 R/W  TOD_INT_PER_1      |
 | bit  15:8  R/W  FREE_RUN_INT_PER_2 |
 | bit  7:0   R/W  FREE_RUN_INT_PER_1 |
 +-----------------------------------*/
#define PMC_ETIME120_REG_INT_PER_CFG_UNUSED_MASK            0x00000000
#define ETIME120_REG_INT_PER_CFG_BIT_TOD_INT_PER_2_MSK      0xff000000
#define ETIME120_REG_INT_PER_CFG_BIT_TOD_INT_PER_2_OFF      24
#define ETIME120_REG_INT_PER_CFG_BIT_TOD_INT_PER_1_MSK      0x00ff0000
#define ETIME120_REG_INT_PER_CFG_BIT_TOD_INT_PER_1_OFF      16
#define ETIME120_REG_INT_PER_CFG_BIT_FREE_RUN_INT_PER_2_MSK 0x0000ff00
#define ETIME120_REG_INT_PER_CFG_BIT_FREE_RUN_INT_PER_2_OFF 8
#define ETIME120_REG_INT_PER_CFG_BIT_FREE_RUN_INT_PER_1_MSK 0x000000ff
#define ETIME120_REG_INT_PER_CFG_BIT_FREE_RUN_INT_PER_1_OFF 0

/*-----------------------------------------.
 | Register 0x00000228 CC_INT_PER_HI_4_CFG |
 +-----------------------------------------+
 | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_4  |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_HI_4_CFG_UNUSED_MASK                  0xfffffff0
#define ETIME120_REG_CC_INT_PER_HI_4_CFG_BIT_CC_MAX_RAND_INT_PER_HI_4_MSK 0x0000000f
#define ETIME120_REG_CC_INT_PER_HI_4_CFG_BIT_CC_MAX_RAND_INT_PER_HI_4_OFF 0

/*-----------------------------------------.
 | Register 0x0000022c CC_INT_PER_HI_3_CFG |
 +-----------------------------------------+
 | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_3  |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_HI_3_CFG_UNUSED_MASK                  0xfffffff0
#define ETIME120_REG_CC_INT_PER_HI_3_CFG_BIT_CC_MAX_RAND_INT_PER_HI_3_MSK 0x0000000f
#define ETIME120_REG_CC_INT_PER_HI_3_CFG_BIT_CC_MAX_RAND_INT_PER_HI_3_OFF 0

/*-----------------------------------------.
 | Register 0x00000230 CC_INT_PER_HI_2_CFG |
 +-----------------------------------------+
 | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_2  |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_HI_2_CFG_UNUSED_MASK                  0xfffffff0
#define ETIME120_REG_CC_INT_PER_HI_2_CFG_BIT_CC_MAX_RAND_INT_PER_HI_2_MSK 0x0000000f
#define ETIME120_REG_CC_INT_PER_HI_2_CFG_BIT_CC_MAX_RAND_INT_PER_HI_2_OFF 0

/*-----------------------------------------.
 | Register 0x00000234 CC_INT_PER_HI_1_CFG |
 +-----------------------------------------+
 | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_1  |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_HI_1_CFG_UNUSED_MASK                  0xfffffff0
#define ETIME120_REG_CC_INT_PER_HI_1_CFG_BIT_CC_MAX_RAND_INT_PER_HI_1_MSK 0x0000000f
#define ETIME120_REG_CC_INT_PER_HI_1_CFG_BIT_CC_MAX_RAND_INT_PER_HI_1_OFF 0

/*-----------------------------------------.
 | Register 0x00000238 CC_INT_PER_4_CFG    |
 +-----------------------------------------+
 | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_4 |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_4_CFG_UNUSED_MASK                  0x00000000
#define ETIME120_REG_CC_INT_PER_4_CFG_BIT_CC_MAX_RAND_INT_PER_LO_4_MSK 0xffffffff
#define ETIME120_REG_CC_INT_PER_4_CFG_BIT_CC_MAX_RAND_INT_PER_LO_4_OFF 0

/*-----------------------------------------.
 | Register 0x0000023c CC_INT_PER_3_CFG    |
 +-----------------------------------------+
 | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_3 |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_3_CFG_UNUSED_MASK                  0x00000000
#define ETIME120_REG_CC_INT_PER_3_CFG_BIT_CC_MAX_RAND_INT_PER_LO_3_MSK 0xffffffff
#define ETIME120_REG_CC_INT_PER_3_CFG_BIT_CC_MAX_RAND_INT_PER_LO_3_OFF 0

/*-----------------------------------------.
 | Register 0x00000240 CC_INT_PER_2_CFG    |
 +-----------------------------------------+
 | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_2 |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_2_CFG_UNUSED_MASK                  0x00000000
#define ETIME120_REG_CC_INT_PER_2_CFG_BIT_CC_MAX_RAND_INT_PER_LO_2_MSK 0xffffffff
#define ETIME120_REG_CC_INT_PER_2_CFG_BIT_CC_MAX_RAND_INT_PER_LO_2_OFF 0

/*-----------------------------------------.
 | Register 0x00000244 CC_INT_PER_1_CFG    |
 +-----------------------------------------+
 | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_1 |
 +----------------------------------------*/
#define PMC_ETIME120_REG_CC_INT_PER_1_CFG_UNUSED_MASK                  0x00000000
#define ETIME120_REG_CC_INT_PER_1_CFG_BIT_CC_MAX_RAND_INT_PER_LO_1_MSK 0xffffffff
#define ETIME120_REG_CC_INT_PER_1_CFG_BIT_CC_MAX_RAND_INT_PER_LO_1_OFF 0

/*---------------------------------------.
 | Register 0x0000024c REFL_DLY_CFG      |
 +---------------------------------------+
 | bit  29:20 R/W  REFL_TIMEOUT_100G_DLY |
 | bit  19:10 R/W  REFL_TIMEOUT_40G_DLY  |
 | bit  9:0   R/W  REFL_TIMEOUT_10G_DLY  |
 +--------------------------------------*/
#define PMC_ETIME120_REG_REFL_DLY_CFG_UNUSED_MASK               0xc0000000
#define ETIME120_REG_REFL_DLY_CFG_BIT_REFL_TIMEOUT_100G_DLY_MSK 0x3ff00000
#define ETIME120_REG_REFL_DLY_CFG_BIT_REFL_TIMEOUT_100G_DLY_OFF 20
#define ETIME120_REG_REFL_DLY_CFG_BIT_REFL_TIMEOUT_40G_DLY_MSK  0x000ffc00
#define ETIME120_REG_REFL_DLY_CFG_BIT_REFL_TIMEOUT_40G_DLY_OFF  10
#define ETIME120_REG_REFL_DLY_CFG_BIT_REFL_TIMEOUT_10G_DLY_MSK  0x000003ff
#define ETIME120_REG_REFL_DLY_CFG_BIT_REFL_TIMEOUT_10G_DLY_OFF  0

/*----------------------------------.
 | Register 0x00000250 INTERPOL_CFG |
 +----------------------------------+
 | bit  31:28 R/W  INTERPOL_VAL7    |
 | bit  27:24 R/W  INTERPOL_VAL6    |
 | bit  23:20 R/W  INTERPOL_VAL5    |
 | bit  19:16 R/W  INTERPOL_VAL4    |
 | bit  15:12 R/W  INTERPOL_VAL3    |
 | bit  11:8  R/W  INTERPOL_VAL2    |
 | bit  7:4   R/W  INTERPOL_VAL1    |
 | bit  3:0   R/W  INTERPOL_VAL0    |
 +---------------------------------*/
#define PMC_ETIME120_REG_INTERPOL_CFG_UNUSED_MASK       0x00000000
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL7_MSK 0xf0000000
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL7_OFF 28
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL6_MSK 0x0f000000
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL6_OFF 24
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL5_MSK 0x00f00000
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL5_OFF 20
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL4_MSK 0x000f0000
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL4_OFF 16
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL3_MSK 0x0000f000
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL3_OFF 12
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL2_MSK 0x00000f00
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL2_OFF 8
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL1_MSK 0x000000f0
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL1_OFF 4
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL0_MSK 0x0000000f
#define ETIME120_REG_INTERPOL_CFG_BIT_INTERPOL_VAL0_OFF 0

/*---------------------------------.
 | Register 0x00000254 CAPT_OFFS   |
 +---------------------------------+
 | bit  29:20 R/W  CAPT_OFFS_EXT   |
 | bit  19:10 R/W  CAPT_OFFS_IRIGI |
 | bit  9:0   R/W  CAPT_OFFS_IRIGO |
 +--------------------------------*/
#define PMC_ETIME120_REG_CAPT_OFFS_UNUSED_MASK         0xc0000000
#define ETIME120_REG_CAPT_OFFS_BIT_CAPT_OFFS_EXT_MSK   0x3ff00000
#define ETIME120_REG_CAPT_OFFS_BIT_CAPT_OFFS_EXT_OFF   20
#define ETIME120_REG_CAPT_OFFS_BIT_CAPT_OFFS_IRIGI_MSK 0x000ffc00
#define ETIME120_REG_CAPT_OFFS_BIT_CAPT_OFFS_IRIGI_OFF 10
#define ETIME120_REG_CAPT_OFFS_BIT_CAPT_OFFS_IRIGO_MSK 0x000003ff
#define ETIME120_REG_CAPT_OFFS_BIT_CAPT_OFFS_IRIGO_OFF 0

/*--------------------------------------.
 | Register 0x00000288 EXT_CPB_LSC_LOW  |
 +--------------------------------------+
 | bit  31:28 R/W  EXT_CPB_LINK_SCH_TS7 |
 | bit  27:24 R/W  EXT_CPB_LINK_SCH_TS6 |
 | bit  23:20 R/W  EXT_CPB_LINK_SCH_TS5 |
 | bit  19:16 R/W  EXT_CPB_LINK_SCH_TS4 |
 | bit  15:12 R/W  EXT_CPB_LINK_SCH_TS3 |
 | bit  11:8  R/W  EXT_CPB_LINK_SCH_TS2 |
 | bit  7:4   R/W  EXT_CPB_LINK_SCH_TS1 |
 | bit  3:0   R/W  EXT_CPB_LINK_SCH_TS0 |
 +-------------------------------------*/
#define PMC_ETIME120_REG_EXT_CPB_LSC_LOW_UNUSED_MASK              0x00000000
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS7_MSK 0xf0000000
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS7_OFF 28
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS6_MSK 0x0f000000
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS6_OFF 24
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS5_MSK 0x00f00000
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS5_OFF 20
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS4_MSK 0x000f0000
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS4_OFF 16
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS3_MSK 0x0000f000
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS3_OFF 12
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS2_MSK 0x00000f00
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS2_OFF 8
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS1_MSK 0x000000f0
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS1_OFF 4
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS0_MSK 0x0000000f
#define ETIME120_REG_EXT_CPB_LSC_LOW_BIT_EXT_CPB_LINK_SCH_TS0_OFF 0

/*---------------------------------------.
 | Register 0x0000028c EXT_CPB_LSC_HIGH  |
 +---------------------------------------+
 | bit  15:12 R/W  EXT_CPB_LINK_SCH_TS11 |
 | bit  11:8  R/W  EXT_CPB_LINK_SCH_TS10 |
 | bit  7:4   R/W  EXT_CPB_LINK_SCH_TS9  |
 | bit  3:0   R/W  EXT_CPB_LINK_SCH_TS8  |
 +--------------------------------------*/
#define PMC_ETIME120_REG_EXT_CPB_LSC_HIGH_UNUSED_MASK               0xffff0000
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS11_MSK 0x0000f000
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS11_OFF 12
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS10_MSK 0x00000f00
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS10_OFF 8
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS9_MSK  0x000000f0
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS9_OFF  4
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS8_MSK  0x0000000f
#define ETIME120_REG_EXT_CPB_LSC_HIGH_BIT_EXT_CPB_LINK_SCH_TS8_OFF  0

/*----------------------------------------------.
 | Register 0x00000290 EXT_CPB_CFG              |
 +----------------------------------------------+
 | bit  8   R/W  PMON_CNT_CLR                   |
 | bit  7:4 R/W  EXT_CPB_LINK_SCH_LAST_TIMESLOT |
 | bit  3:0 R/W  EXT_CPB_LINK_SCH_OFFSET        |
 +---------------------------------------------*/
#define PMC_ETIME120_REG_EXT_CPB_CFG_UNUSED_MASK                        0xfffffe00
#define ETIME120_REG_EXT_CPB_CFG_BIT_PMON_CNT_CLR_MSK                   0x00000100
#define ETIME120_REG_EXT_CPB_CFG_BIT_PMON_CNT_CLR_OFF                   8
#define ETIME120_REG_EXT_CPB_CFG_BIT_EXT_CPB_LINK_SCH_LAST_TIMESLOT_MSK 0x000000f0
#define ETIME120_REG_EXT_CPB_CFG_BIT_EXT_CPB_LINK_SCH_LAST_TIMESLOT_OFF 4
#define ETIME120_REG_EXT_CPB_CFG_BIT_EXT_CPB_LINK_SCH_OFFSET_MSK        0x0000000f
#define ETIME120_REG_EXT_CPB_CFG_BIT_EXT_CPB_LINK_SCH_OFFSET_OFF        0

/*-------------------------------------.
 | Register 0x00000294 EXT_LNK_SCH_UPD |
 +-------------------------------------+
 | bit  0 R/W  EXT_CPB_LINK_SCH_UPD    |
 +------------------------------------*/
#define PMC_ETIME120_REG_EXT_LNK_SCH_UPD_UNUSED_MASK              0xfffffffe
#define ETIME120_REG_EXT_LNK_SCH_UPD_BIT_EXT_CPB_LINK_SCH_UPD_MSK 0x00000001
#define ETIME120_REG_EXT_LNK_SCH_UPD_BIT_EXT_CPB_LINK_SCH_UPD_OFF 0

/*--------------------------------------------.
 | Register 0x00000298 EXT_NUM_SLOTS_OCCUPIED |
 +--------------------------------------------+
 | bit  20:14 R/W  EXT_SLOTS_OCCUPIED_100G    |
 | bit  13:7  R/W  EXT_SLOTS_OCCUPIED_40G     |
 | bit  6:0   R/W  EXT_SLOTS_OCCUPIED_10G     |
 +-------------------------------------------*/
#define PMC_ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_UNUSED_MASK                 0xffe00000
#define ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_BIT_EXT_SLOTS_OCCUPIED_100G_MSK 0x001fc000
#define ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_BIT_EXT_SLOTS_OCCUPIED_100G_OFF 14
#define ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_BIT_EXT_SLOTS_OCCUPIED_40G_MSK  0x00003f80
#define ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_BIT_EXT_SLOTS_OCCUPIED_40G_OFF  7
#define ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_BIT_EXT_SLOTS_OCCUPIED_10G_MSK  0x0000007f
#define ETIME120_REG_EXT_NUM_SLOTS_OCCUPIED_BIT_EXT_SLOTS_OCCUPIED_10G_OFF  0

/*-----------------------------.
 | Register 0x0000029c ENA_REG |
 +-----------------------------+
 | bit  1 R/W  LINE_REF_ENA    |
 | bit  0 R/W  REFL_PATH_ENA   |
 +----------------------------*/
#define PMC_ETIME120_REG_ENA_REG_UNUSED_MASK       0xfffffffc
#define ETIME120_REG_ENA_REG_BIT_LINE_REF_ENA_MSK  0x00000002
#define ETIME120_REG_ENA_REG_BIT_LINE_REF_ENA_OFF  1
#define ETIME120_REG_ENA_REG_BIT_REFL_PATH_ENA_MSK 0x00000001
#define ETIME120_REG_ENA_REG_BIT_REFL_PATH_ENA_OFF 0

/*-------------------------------------------.
 | Register 0x000002a0 REFL_DROP_ENA_CFG_REG |
 +-------------------------------------------+
 | bit  11 R/W  REFL_DROP_ENA_CH11           |
 | bit  10 R/W  REFL_DROP_ENA_CH10           |
 | bit  9  R/W  REFL_DROP_ENA_CH9            |
 | bit  8  R/W  REFL_DROP_ENA_CH8            |
 | bit  7  R/W  REFL_DROP_ENA_CH7            |
 | bit  6  R/W  REFL_DROP_ENA_CH6            |
 | bit  5  R/W  REFL_DROP_ENA_CH5            |
 | bit  4  R/W  REFL_DROP_ENA_CH4            |
 | bit  3  R/W  REFL_DROP_ENA_CH3            |
 | bit  2  R/W  REFL_DROP_ENA_CH2            |
 | bit  1  R/W  REFL_DROP_ENA_CH1            |
 | bit  0  R/W  REFL_DROP_ENA_CH0            |
 +------------------------------------------*/
#define PMC_ETIME120_REG_REFL_DROP_ENA_CFG_REG_UNUSED_MASK            0xfffff000
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH11_MSK 0x00000800
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH11_OFF 11
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH10_MSK 0x00000400
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH10_OFF 10
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH9_MSK  0x00000200
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH9_OFF  9
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH8_MSK  0x00000100
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH8_OFF  8
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH7_MSK  0x00000080
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH7_OFF  7
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH6_MSK  0x00000040
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH6_OFF  6
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH5_MSK  0x00000020
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH5_OFF  5
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH4_MSK  0x00000010
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH4_OFF  4
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH3_MSK  0x00000008
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH3_OFF  3
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH2_MSK  0x00000004
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH2_OFF  2
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH1_MSK  0x00000002
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH1_OFF  1
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH0_MSK  0x00000001
#define ETIME120_REG_REFL_DROP_ENA_CFG_REG_BIT_REFL_DROP_ENA_CH0_OFF  0

/*------------------------------------------.
 | Register 0x000002a8 LINE_REF_CNT_DLY_REG |
 +------------------------------------------+
 | bit  31:0 R/W  LINE_REF_CNT_DLY          |
 +-----------------------------------------*/
#define PMC_ETIME120_REG_LINE_REF_CNT_DLY_REG_UNUSED_MASK          0x00000000
#define ETIME120_REG_LINE_REF_CNT_DLY_REG_BIT_LINE_REF_CNT_DLY_MSK 0xffffffff
#define ETIME120_REG_LINE_REF_CNT_DLY_REG_BIT_LINE_REF_CNT_DLY_OFF 0

/*------------------------------------.
 | Register 0x000002ac EXT_PMON_LINK0 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_0           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK0_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK0_BIT_REFL_PMON_0_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK0_BIT_REFL_PMON_0_OFF 0

/*------------------------------------.
 | Register 0x000002b0 EXT_PMON_LINK1 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_1           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK1_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK1_BIT_REFL_PMON_1_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK1_BIT_REFL_PMON_1_OFF 0

/*------------------------------------.
 | Register 0x000002b4 EXT_PMON_LINK2 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_2           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK2_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK2_BIT_REFL_PMON_2_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK2_BIT_REFL_PMON_2_OFF 0

/*------------------------------------.
 | Register 0x000002b8 EXT_PMON_LINK3 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_3           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK3_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK3_BIT_REFL_PMON_3_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK3_BIT_REFL_PMON_3_OFF 0

/*------------------------------------.
 | Register 0x000002bc EXT_PMON_LINK4 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_4           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK4_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK4_BIT_REFL_PMON_4_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK4_BIT_REFL_PMON_4_OFF 0

/*------------------------------------.
 | Register 0x000002c0 EXT_PMON_LINK5 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_5           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK5_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK5_BIT_REFL_PMON_5_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK5_BIT_REFL_PMON_5_OFF 0

/*------------------------------------.
 | Register 0x000002c4 EXT_PMON_LINK6 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_6           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK6_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK6_BIT_REFL_PMON_6_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK6_BIT_REFL_PMON_6_OFF 0

/*------------------------------------.
 | Register 0x000002c8 EXT_PMON_LINK7 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_7           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK7_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK7_BIT_REFL_PMON_7_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK7_BIT_REFL_PMON_7_OFF 0

/*------------------------------------.
 | Register 0x000002cc EXT_PMON_LINK8 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_8           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK8_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK8_BIT_REFL_PMON_8_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK8_BIT_REFL_PMON_8_OFF 0

/*------------------------------------.
 | Register 0x000002d0 EXT_PMON_LINK9 |
 +------------------------------------+
 | bit  11:0 R  REFL_PMON_9           |
 +-----------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK9_UNUSED_MASK     0xfffff000
#define ETIME120_REG_EXT_PMON_LINK9_BIT_REFL_PMON_9_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK9_BIT_REFL_PMON_9_OFF 0

/*-------------------------------------.
 | Register 0x000002d4 EXT_PMON_LINK10 |
 +-------------------------------------+
 | bit  11:0 R  REFL_PMON_10           |
 +------------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK10_UNUSED_MASK      0xfffff000
#define ETIME120_REG_EXT_PMON_LINK10_BIT_REFL_PMON_10_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK10_BIT_REFL_PMON_10_OFF 0

/*-------------------------------------.
 | Register 0x000002d8 EXT_PMON_LINK11 |
 +-------------------------------------+
 | bit  11:0 R  REFL_PMON_11           |
 +------------------------------------*/
#define PMC_ETIME120_REG_EXT_PMON_LINK11_UNUSED_MASK      0xfffff000
#define ETIME120_REG_EXT_PMON_LINK11_BIT_REFL_PMON_11_MSK 0x00000fff
#define ETIME120_REG_EXT_PMON_LINK11_BIT_REFL_PMON_11_OFF 0

/*--------------------------------------.
 | Register 0x000002dc IRIGO_OFFS_S_REG |
 +--------------------------------------+
 | bit  15:0 R/W  IRIGO_OFFS_S          |
 +-------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_OFFS_S_REG_UNUSED_MASK      0xffff0000
#define ETIME120_REG_IRIGO_OFFS_S_REG_BIT_IRIGO_OFFS_S_MSK 0x0000ffff
#define ETIME120_REG_IRIGO_OFFS_S_REG_BIT_IRIGO_OFFS_S_OFF 0

/*---------------------------------------.
 | Register 0x000002e0 IRIGO_OFFS_NS_REG |
 +---------------------------------------+
 | bit  30:0 R/W  IRIGO_OFFS_NS          |
 +--------------------------------------*/
#define PMC_ETIME120_REG_IRIGO_OFFS_NS_REG_UNUSED_MASK       0x80000000
#define ETIME120_REG_IRIGO_OFFS_NS_REG_BIT_IRIGO_OFFS_NS_MSK 0x7fffffff
#define ETIME120_REG_IRIGO_OFFS_NS_REG_BIT_IRIGO_OFFS_NS_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ETIME_REGS_H */
