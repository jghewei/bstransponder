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
 *     ri_rcp block
 *****************************************************************************/
#ifndef _RI_RCP_REGS_H
#define _RI_RCP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ODU_RCP_REG_REG_SW_RESET               0x00000000
#define PMC_ODU_RCP_REG_REG_LOWPWR                 0x00000004
#define PMC_ODU_RCP_REG_REG_RAM_PWR_MGMT           0x0000000c
#define PMC_ODU_RCP_REG_IRQ_SUMMARY                0x00000010
#define PMC_ODU_RCP_REG_REG_GOJF                   0x0000001c
#define PMC_ODU_RCP_REG_GEN_IRQ_I                  0x00000020
#define PMC_ODU_RCP_REG_GEN_IRQ_E                  0x00000040
#define PMC_ODU_RCP_REG_REG_ETH_TX_EN              0x00000060
#define PMC_ODU_RCP_REG_REG_ETH_MAC_OUI            0x00000068
#define PMC_ODU_RCP_REG_REG_ETH_MAC_DIGI_ID        0x0000006c
#define PMC_ODU_RCP_REG_REG_ETH_ETYPE              0x00000070
#define PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG( N )     (0x00000080 + (N) * 4)
#define PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG( N )     (0x00000100 + (N) * 4)
#define PMC_ODU_RCP_REG_REG_TX_STG_1_RCP_INS_EN_0  0x00000b00
#define PMC_ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_0  0x00000b10
#define PMC_ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_1  0x00000b14
#define PMC_ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_2  0x00000b18
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_0 0x00000b20
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_1 0x00000b24
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_2 0x00000b28
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_0 0x00000b30
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_1 0x00000b34
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_2 0x00000b38
#define PMC_ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_0  0x00000b40
#define PMC_ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_1  0x00000b44
#define PMC_ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_2  0x00000b48
#define PMC_ODU_RCP_REG_REG_TX_STG_N_RCP_INS_EN_0  0x00000b50
#define PMC_ODU_RCP_REG_REG_TX_STG_1_APS_INS_EN_0  0x00000b80
#define PMC_ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_0  0x00000b90
#define PMC_ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_1  0x00000b94
#define PMC_ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_2  0x00000b98
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_0 0x00000ba0
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_1 0x00000ba4
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_2 0x00000ba8
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_0 0x00000bb0
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_1 0x00000bb4
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_2 0x00000bb8
#define PMC_ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_0  0x00000bc0
#define PMC_ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_1  0x00000bc4
#define PMC_ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_2  0x00000bc8
#define PMC_ODU_RCP_REG_REG_TX_STG_N_APS_INS_EN_0  0x00000bd0
#define PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL( N )    (0x00000c00 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL( N )    (0x00000c80 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL( N )   (0x00000e00 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL( N )   (0x00000f80 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL( N )    (0x00001100 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL( N )    (0x00001280 + (N) * 0x04)
#define PMC_ODU_RCP_REG_CFC_DBG_SEL                0x00001300
#define PMC_ODU_RCP_REG_DBG_CTRL                   0x00001304
#define PMC_ODU_RCP_REG_DBG_RDATA                  0x00001308
#define PMC_ODU_RCP_REG_CFC_IRQ_SEL                0x00001320
#define PMC_ODU_RCP_REG_CFC_IRQ_TYPE_SEL_0         0x00001324
#define PMC_ODU_RCP_REG_CFC_IRQ_TYPE_SEL_1         0x00001328
#define PMC_ODU_RCP_REG_CFC_IRQ_TYPE_SEL_2         0x0000132c
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_0             0x00001330
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_1             0x00001334
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_2             0x00001338
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_SEL            0x00001340
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_0              0x00001344
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_1              0x00001348
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_2              0x0000134c
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_EN_0          0x00001360
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_EN_1          0x00001364
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_EN_2          0x00001368
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_EN_0           0x00001380
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_EN_1           0x00001384
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_EN_2           0x00001388
#define PMC_ODU_RCP_REG_CFC_STATUS_SEL             0x000013c0
#define PMC_ODU_RCP_REG_CFC_EMPTY_0                0x000013c4
#define PMC_ODU_RCP_REG_CFC_EMPTY_1                0x000013c8
#define PMC_ODU_RCP_REG_CFC_EMPTY_2                0x000013cc
#define PMC_ODU_RCP_REG_CFC_FULL_0                 0x000013d0
#define PMC_ODU_RCP_REG_CFC_FULL_1                 0x000013d4
#define PMC_ODU_RCP_REG_CFC_FULL_2                 0x000013d8
#define PMC_ODU_RCP_REG_RX_STG_1_FIFO_CLRB_0       0x00001800
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_CFG           0x00001820
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_CFG_STAT      0x00001824
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_CFG_VAL       0x00001828
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT( N )      (0x00001840 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_2_FIFO_CLRB_0       0x00001a00
#define PMC_ODU_RCP_REG_RX_STG_2_FIFO_CLRB_1       0x00001a04
#define PMC_ODU_RCP_REG_RX_STG_2_FIFO_CLRB_2       0x00001a08
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_CFG           0x00001a20
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_CFG_STAT      0x00001a24
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_CFG_VAL       0x00001a28
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT( N )      (0x00001a40 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_0      0x00001c00
#define PMC_ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_1      0x00001c04
#define PMC_ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_2      0x00001c08
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_CFG          0x00001c20
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_CFG_STAT     0x00001c24
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_CFG_VAL      0x00001c28
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT( N )     (0x00001c40 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_0      0x00001e00
#define PMC_ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_1      0x00001e04
#define PMC_ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_2      0x00001e08
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_CFG          0x00001e20
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_CFG_STAT     0x00001e24
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_CFG_VAL      0x00001e28
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT( N )     (0x00001e40 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_4_FIFO_CLRB_0       0x00002000
#define PMC_ODU_RCP_REG_RX_STG_4_FIFO_CLRB_1       0x00002004
#define PMC_ODU_RCP_REG_RX_STG_4_FIFO_CLRB_2       0x00002008
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_CFG           0x00002020
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_CFG_STAT      0x00002024
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_CFG_VAL       0x00002028
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT( N )      (0x00002040 + (N) * 0x04)
#define PMC_ODU_RCP_REG_RX_STG_N_FIFO_CLRB_0       0x00002200
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_CFG           0x00002220
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_CFG_STAT      0x00002224
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_CFG_VAL       0x00002228
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT( N )      (0x00002240 + (N) * 0x04)
#define PMC_ODU_RCP_REG_TX_STG_1_FIFO_CLRB_0       0x00002400
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_CFG           0x00002420
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_CFG_STAT      0x00002424
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_CFG_VAL       0x00002428
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT( N )      (0x00002440 + (N) * 0x04)
#define PMC_ODU_RCP_REG_TX_STG_2_FIFO_CLRB_0       0x00002600
#define PMC_ODU_RCP_REG_TX_STG_2_FIFO_CLRB_1       0x00002604
#define PMC_ODU_RCP_REG_TX_STG_2_FIFO_CLRB_2       0x00002608
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_CFG           0x00002620
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_CFG_STAT      0x00002624
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_CFG_VAL       0x00002628
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT( N )      (0x00002640 + (N) * 0x04)
#define PMC_ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_0      0x00002800
#define PMC_ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_1      0x00002804
#define PMC_ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_2      0x00002808
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_CFG          0x00002820
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_CFG_STAT     0x00002824
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_CFG_VAL      0x00002828
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT( N )     (0x00002840 + (N) * 0x04)
#define PMC_ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_0      0x00002a00
#define PMC_ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_1      0x00002a04
#define PMC_ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_2      0x00002a08
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_CFG          0x00002a20
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_CFG_STAT     0x00002a24
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_CFG_VAL      0x00002a28
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT( N )     (0x00002a40 + (N) * 0x04)
#define PMC_ODU_RCP_REG_TX_STG_4_FIFO_CLRB_0       0x00002c00
#define PMC_ODU_RCP_REG_TX_STG_4_FIFO_CLRB_1       0x00002c04
#define PMC_ODU_RCP_REG_TX_STG_4_FIFO_CLRB_2       0x00002c08
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_CFG           0x00002c20
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_CFG_STAT      0x00002c24
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_CFG_VAL       0x00002c28
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT( N )      (0x00002c40 + (N) * 0x04)
#define PMC_ODU_RCP_REG_TX_STG_N_FIFO_CLRB_0       0x00002e00
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_CFG           0x00002e20
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_CFG_STAT      0x00002e24
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_CFG_VAL       0x00002e28
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT( N )      (0x00002e40 + (N) * 0x04)
#define PMC_ODU_RCP_REG_REG_SCH_CAL_ENABLE         0x00003000
#define PMC_ODU_RCP_REG_REG_SCH_CAL_SEL            0x00003004
#define PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG( N )     (0x00003080 + (N) * 0x04)
#define PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG( N )    (0x000030c0 + (N) * 0x04)
#define PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG( N )     (0x00003100 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG( N )    (0x00003200 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG( N )    (0x00003300 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG( N )   (0x00003400 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG( N )    (0x00003500 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG( N )   (0x00003600 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG( N )     (0x00003700 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG( N )    (0x00003800 + (N) * 4)
#define PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG( N )     (0x00003900 + (N) * 0x04)
#define PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG( N )    (0x00003940 + (N) * 0x04)

/*----------------------------------.
 | Register 0x00000000 REG_SW_RESET |
 +----------------------------------+
 | bit  1 R/W  REG_SOFT_SW_RESET    |
 | bit  0 R/W  REG_SW_RESET         |
 +---------------------------------*/
#define PMC_ODU_RCP_REG_REG_SW_RESET_UNUSED_MASK           0xfffffffc
#define ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_MSK 0x00000002
#define ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_OFF 1
#define ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_MSK      0x00000001
#define ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_OFF      0

/*--------------------------------.
 | Register 0x00000004 REG_LOWPWR |
 +--------------------------------+
 | bit  0 R/W  REG_LOWPWR         |
 +-------------------------------*/
#define PMC_ODU_RCP_REG_REG_LOWPWR_UNUSED_MASK    0xfffffffe
#define ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_MSK 0x00000001
#define ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_OFF 0

/*--------------------------------------.
 | Register 0x0000000c REG_RAM_PWR_MGMT |
 +--------------------------------------+
 | bit  0 R/W  REG_RAM_LOWPWR           |
 +-------------------------------------*/
#define PMC_ODU_RCP_REG_REG_RAM_PWR_MGMT_UNUSED_MASK        0xfffffefe
#define ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_MSK 0x00000001
#define ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_OFF 0

/*---------------------------------.
 | Register 0x00000010 IRQ_SUMMARY |
 +---------------------------------+
 | bit  12:0 R  INTERRUPT_SUMMARY  |
 +--------------------------------*/
#define PMC_ODU_RCP_REG_IRQ_SUMMARY_UNUSED_MASK           0xffffe000
#define ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK 0x00001fff
#define ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF 0

/*--------------------------------------------.
 | Register 0x0000001c REG_GOJF               |
 +--------------------------------------------+
 | bit  21:16 R/W  REG_ING_FIFO_MIN_SLOT_FREE |
 | bit  13:8  R/W  REG_ING_FIFO_MIN_LVL       |
 | bit  4     R/W  REG_DROP_PDU_WO_DFEC       |
 | bit  3     R/W  REG_ETH_RX_10B_SWIZZLE     |
 | bit  2     R/W  REG_ETH_TX_10B_SWIZZLE     |
 | bit  1     R/W  REG_ETH_RX_8B_SWIZZLE      |
 | bit  0     R/W  REG_ETH_TX_8B_SWIZZLE      |
 +-------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_GOJF_UNUSED_MASK                    0xffc0c0e0
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ING_FIFO_MIN_SLOT_FREE_MSK 0x003f0000
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ING_FIFO_MIN_SLOT_FREE_OFF 16
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ING_FIFO_MIN_LVL_MSK       0x00003f00
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ING_FIFO_MIN_LVL_OFF       8
#define ODU_RCP_REG_REG_GOJF_BIT_REG_DROP_PDU_WO_DFEC_MSK       0x00000010
#define ODU_RCP_REG_REG_GOJF_BIT_REG_DROP_PDU_WO_DFEC_OFF       4
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_MSK     0x00000008
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_OFF     3
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_MSK     0x00000004
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_OFF     2
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_8B_SWIZZLE_MSK      0x00000002
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_8B_SWIZZLE_OFF      1
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_MSK      0x00000001
#define ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_OFF      0

/*-------------------------------.
 | Register 0x00000020 GEN_IRQ_I |
 +-------------------------------+
 | bit  0 R/W  FEGE_TIP_I        |
 +------------------------------*/
#define PMC_ODU_RCP_REG_GEN_IRQ_I_UNUSED_MASK    0xfffffcce
#define ODU_RCP_REG_GEN_IRQ_I_BIT_FEGE_TIP_I_MSK 0x00000001
#define ODU_RCP_REG_GEN_IRQ_I_BIT_FEGE_TIP_I_OFF 0

/*-------------------------------.
 | Register 0x00000040 GEN_IRQ_E |
 +-------------------------------+
 | bit  0 R/W  FEGE_TIP_E        |
 +------------------------------*/
#define PMC_ODU_RCP_REG_GEN_IRQ_E_UNUSED_MASK    0xfffffcce
#define ODU_RCP_REG_GEN_IRQ_E_BIT_FEGE_TIP_E_MSK 0x00000001
#define ODU_RCP_REG_GEN_IRQ_E_BIT_FEGE_TIP_E_OFF 0

/*-----------------------------------.
 | Register 0x00000060 REG_ETH_TX_EN |
 +-----------------------------------+
 | bit  0 R/W  REG_ETH_TX_EN         |
 +----------------------------------*/
#define PMC_ODU_RCP_REG_REG_ETH_TX_EN_UNUSED_MASK       0xfffffffe
#define ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK 0x00000001
#define ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF 0

/*-------------------------------------.
 | Register 0x00000068 REG_ETH_MAC_OUI |
 +-------------------------------------+
 | bit  23:0 R/W  REG_ETH_MAC_OUI      |
 +------------------------------------*/
#define PMC_ODU_RCP_REG_REG_ETH_MAC_OUI_UNUSED_MASK         0xff000000
#define ODU_RCP_REG_REG_ETH_MAC_OUI_BIT_REG_ETH_MAC_OUI_MSK 0x00ffffff
#define ODU_RCP_REG_REG_ETH_MAC_OUI_BIT_REG_ETH_MAC_OUI_OFF 0

/*-----------------------------------------.
 | Register 0x0000006c REG_ETH_MAC_DIGI_ID |
 +-----------------------------------------+
 | bit  7:0 R/W  REG_ETH_MAC_DIGI_ID       |
 +----------------------------------------*/
#define PMC_ODU_RCP_REG_REG_ETH_MAC_DIGI_ID_UNUSED_MASK             0xffffff00
#define ODU_RCP_REG_REG_ETH_MAC_DIGI_ID_BIT_REG_ETH_MAC_DIGI_ID_MSK 0x000000ff
#define ODU_RCP_REG_REG_ETH_MAC_DIGI_ID_BIT_REG_ETH_MAC_DIGI_ID_OFF 0

/*-----------------------------------.
 | Register 0x00000070 REG_ETH_ETYPE |
 +-----------------------------------+
 | bit  15:0 R/W  REG_ETH_ETYPE      |
 +----------------------------------*/
#define PMC_ODU_RCP_REG_REG_ETH_ETYPE_UNUSED_MASK       0xffff0000
#define ODU_RCP_REG_REG_ETH_ETYPE_BIT_REG_ETH_ETYPE_MSK 0x0000ffff
#define ODU_RCP_REG_REG_ETH_ETYPE_BIT_REG_ETH_ETYPE_OFF 0

/* index definitions for PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG */
#define PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG_INDEX_N_MIN            0
#define PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG_INDEX_N_MAX            31
#define PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG_INDEX_N_SIZE           32
#define PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG_INDEX_N_OFFSET         4

/*---------------------------------------------------.
 | Register (0x00000080 + (N) * 4) HO_ODU_MAC_DA_CFG |
 +---------------------------------------------------+
 | bit  23:16 R/W  HO_ODU_DST_DIGI                   |
 | bit  10:8  R/W  HO_ODU_DST_FRM                    |
 | bit  6:0   R/W  HO_ODU_DST_CHANNEL                |
 +--------------------------------------------------*/
#define PMC_ODU_RCP_REG_HO_ODU_MAC_DA_CFG_UNUSED_MASK            0xff00f880
#define ODU_RCP_REG_HO_ODU_MAC_DA_CFG_BIT_HO_ODU_DST_DIGI_MSK    0x00ff0000
#define ODU_RCP_REG_HO_ODU_MAC_DA_CFG_BIT_HO_ODU_DST_DIGI_OFF    16
#define ODU_RCP_REG_HO_ODU_MAC_DA_CFG_BIT_HO_ODU_DST_FRM_MSK     0x00000700
#define ODU_RCP_REG_HO_ODU_MAC_DA_CFG_BIT_HO_ODU_DST_FRM_OFF     8
#define ODU_RCP_REG_HO_ODU_MAC_DA_CFG_BIT_HO_ODU_DST_CHANNEL_MSK 0x0000007f
#define ODU_RCP_REG_HO_ODU_MAC_DA_CFG_BIT_HO_ODU_DST_CHANNEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG */
#define PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG_INDEX_N_MIN            0
#define PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG_INDEX_N_MAX            511
#define PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG_INDEX_N_SIZE           512
#define PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG_INDEX_N_OFFSET         4

/*---------------------------------------------------.
 | Register (0x00000100 + (N) * 4) LO_ODU_MAC_DA_CFG |
 +---------------------------------------------------+
 | bit  23:16 R/W  LO_ODU_DST_DIGI                   |
 | bit  10:8  R/W  LO_ODU_DST_FRM                    |
 | bit  6:0   R/W  LO_ODU_DST_CHANNEL                |
 +--------------------------------------------------*/
#define PMC_ODU_RCP_REG_LO_ODU_MAC_DA_CFG_UNUSED_MASK            0xff00f880
#define ODU_RCP_REG_LO_ODU_MAC_DA_CFG_BIT_LO_ODU_DST_DIGI_MSK    0x00ff0000
#define ODU_RCP_REG_LO_ODU_MAC_DA_CFG_BIT_LO_ODU_DST_DIGI_OFF    16
#define ODU_RCP_REG_LO_ODU_MAC_DA_CFG_BIT_LO_ODU_DST_FRM_MSK     0x00000700
#define ODU_RCP_REG_LO_ODU_MAC_DA_CFG_BIT_LO_ODU_DST_FRM_OFF     8
#define ODU_RCP_REG_LO_ODU_MAC_DA_CFG_BIT_LO_ODU_DST_CHANNEL_MSK 0x0000007f
#define ODU_RCP_REG_LO_ODU_MAC_DA_CFG_BIT_LO_ODU_DST_CHANNEL_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b00 REG_TX_STG_1_RCP_INS_EN_0 |
 +-----------------------------------------------+
 | bit  11:0 R/W  REG_TX_STG_1_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_1_RCP_INS_EN_0_UNUSED_MASK                 0xfffff000
#define ODU_RCP_REG_REG_TX_STG_1_RCP_INS_EN_0_BIT_REG_TX_STG_1_RCP_INS_EN_MSK 0x00000fff
#define ODU_RCP_REG_REG_TX_STG_1_RCP_INS_EN_0_BIT_REG_TX_STG_1_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b10 REG_TX_STG_2_RCP_INS_EN_0 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_2_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_0_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_0_BIT_REG_TX_STG_2_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_0_BIT_REG_TX_STG_2_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b14 REG_TX_STG_2_RCP_INS_EN_1 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_2_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_1_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_1_BIT_REG_TX_STG_2_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_1_BIT_REG_TX_STG_2_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b18 REG_TX_STG_2_RCP_INS_EN_2 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_2_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_2_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_2_BIT_REG_TX_STG_2_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_2_RCP_INS_EN_2_BIT_REG_TX_STG_2_RCP_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000b20 REG_TX_STG_3A_RCP_INS_EN_0 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3A_RCP_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_0_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_0_BIT_REG_TX_STG_3A_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_0_BIT_REG_TX_STG_3A_RCP_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000b24 REG_TX_STG_3A_RCP_INS_EN_1 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3A_RCP_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_1_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_1_BIT_REG_TX_STG_3A_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_1_BIT_REG_TX_STG_3A_RCP_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000b28 REG_TX_STG_3A_RCP_INS_EN_2 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3A_RCP_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_2_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_2_BIT_REG_TX_STG_3A_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3A_RCP_INS_EN_2_BIT_REG_TX_STG_3A_RCP_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000b30 REG_TX_STG_3B_RCP_INS_EN_0 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3B_RCP_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_0_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_0_BIT_REG_TX_STG_3B_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_0_BIT_REG_TX_STG_3B_RCP_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000b34 REG_TX_STG_3B_RCP_INS_EN_1 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3B_RCP_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_1_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_1_BIT_REG_TX_STG_3B_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_1_BIT_REG_TX_STG_3B_RCP_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000b38 REG_TX_STG_3B_RCP_INS_EN_2 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3B_RCP_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_2_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_2_BIT_REG_TX_STG_3B_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3B_RCP_INS_EN_2_BIT_REG_TX_STG_3B_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b40 REG_TX_STG_4_RCP_INS_EN_0 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_4_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_0_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_0_BIT_REG_TX_STG_4_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_0_BIT_REG_TX_STG_4_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b44 REG_TX_STG_4_RCP_INS_EN_1 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_4_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_1_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_1_BIT_REG_TX_STG_4_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_1_BIT_REG_TX_STG_4_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b48 REG_TX_STG_4_RCP_INS_EN_2 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_4_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_2_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_2_BIT_REG_TX_STG_4_RCP_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_4_RCP_INS_EN_2_BIT_REG_TX_STG_4_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b50 REG_TX_STG_N_RCP_INS_EN_0 |
 +-----------------------------------------------+
 | bit  11:0 R/W  REG_TX_STG_N_RCP_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_N_RCP_INS_EN_0_UNUSED_MASK                 0xfffff000
#define ODU_RCP_REG_REG_TX_STG_N_RCP_INS_EN_0_BIT_REG_TX_STG_N_RCP_INS_EN_MSK 0x00000fff
#define ODU_RCP_REG_REG_TX_STG_N_RCP_INS_EN_0_BIT_REG_TX_STG_N_RCP_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b80 REG_TX_STG_1_APS_INS_EN_0 |
 +-----------------------------------------------+
 | bit  11:0 R/W  REG_TX_STG_1_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_1_APS_INS_EN_0_UNUSED_MASK                 0xfffff000
#define ODU_RCP_REG_REG_TX_STG_1_APS_INS_EN_0_BIT_REG_TX_STG_1_APS_INS_EN_MSK 0x00000fff
#define ODU_RCP_REG_REG_TX_STG_1_APS_INS_EN_0_BIT_REG_TX_STG_1_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b90 REG_TX_STG_2_APS_INS_EN_0 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_2_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_0_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_0_BIT_REG_TX_STG_2_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_0_BIT_REG_TX_STG_2_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b94 REG_TX_STG_2_APS_INS_EN_1 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_2_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_1_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_1_BIT_REG_TX_STG_2_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_1_BIT_REG_TX_STG_2_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000b98 REG_TX_STG_2_APS_INS_EN_2 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_2_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_2_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_2_BIT_REG_TX_STG_2_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_2_APS_INS_EN_2_BIT_REG_TX_STG_2_APS_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000ba0 REG_TX_STG_3A_APS_INS_EN_0 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3A_APS_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_0_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_0_BIT_REG_TX_STG_3A_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_0_BIT_REG_TX_STG_3A_APS_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000ba4 REG_TX_STG_3A_APS_INS_EN_1 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3A_APS_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_1_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_1_BIT_REG_TX_STG_3A_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_1_BIT_REG_TX_STG_3A_APS_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000ba8 REG_TX_STG_3A_APS_INS_EN_2 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3A_APS_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_2_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_2_BIT_REG_TX_STG_3A_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3A_APS_INS_EN_2_BIT_REG_TX_STG_3A_APS_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000bb0 REG_TX_STG_3B_APS_INS_EN_0 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3B_APS_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_0_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_0_BIT_REG_TX_STG_3B_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_0_BIT_REG_TX_STG_3B_APS_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000bb4 REG_TX_STG_3B_APS_INS_EN_1 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3B_APS_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_1_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_1_BIT_REG_TX_STG_3B_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_1_BIT_REG_TX_STG_3B_APS_INS_EN_OFF 0

/*------------------------------------------------.
 | Register 0x00000bb8 REG_TX_STG_3B_APS_INS_EN_2 |
 +------------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_3B_APS_INS_EN        |
 +-----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_2_UNUSED_MASK                  0x00000000
#define ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_2_BIT_REG_TX_STG_3B_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_3B_APS_INS_EN_2_BIT_REG_TX_STG_3B_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000bc0 REG_TX_STG_4_APS_INS_EN_0 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_4_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_0_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_0_BIT_REG_TX_STG_4_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_0_BIT_REG_TX_STG_4_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000bc4 REG_TX_STG_4_APS_INS_EN_1 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_4_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_1_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_1_BIT_REG_TX_STG_4_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_1_BIT_REG_TX_STG_4_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000bc8 REG_TX_STG_4_APS_INS_EN_2 |
 +-----------------------------------------------+
 | bit  31:0 R/W  REG_TX_STG_4_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_2_UNUSED_MASK                 0x00000000
#define ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_2_BIT_REG_TX_STG_4_APS_INS_EN_MSK 0xffffffff
#define ODU_RCP_REG_REG_TX_STG_4_APS_INS_EN_2_BIT_REG_TX_STG_4_APS_INS_EN_OFF 0

/*-----------------------------------------------.
 | Register 0x00000bd0 REG_TX_STG_N_APS_INS_EN_0 |
 +-----------------------------------------------+
 | bit  11:0 R/W  REG_TX_STG_N_APS_INS_EN        |
 +----------------------------------------------*/
#define PMC_ODU_RCP_REG_REG_TX_STG_N_APS_INS_EN_0_UNUSED_MASK                 0xfffff000
#define ODU_RCP_REG_REG_TX_STG_N_APS_INS_EN_0_BIT_REG_TX_STG_N_APS_INS_EN_MSK 0x00000fff
#define ODU_RCP_REG_REG_TX_STG_N_APS_INS_EN_0_BIT_REG_TX_STG_N_APS_INS_EN_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL */
#define PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL_INDEX_N_MAX                11
#define PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL_INDEX_N_SIZE               12
#define PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL_INDEX_N_OFFSET             0x04

/*-------------------------------------------------------.
 | Register (0x00000c00 + (N) * 0x04) RX_STG_1_APS_LEVEL |
 +-------------------------------------------------------+
 | bit  2:0 R/W  REG_RX_STG_1_APS_LEVEL                  |
 +------------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_1_APS_LEVEL_UNUSED_MASK                0xfffffff8
#define ODU_RCP_REG_RX_STG_1_APS_LEVEL_BIT_REG_RX_STG_1_APS_LEVEL_MSK 0x00000007
#define ODU_RCP_REG_RX_STG_1_APS_LEVEL_BIT_REG_RX_STG_1_APS_LEVEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL */
#define PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL_INDEX_N_MAX                95
#define PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL_INDEX_N_SIZE               96
#define PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL_INDEX_N_OFFSET             0x04

/*-------------------------------------------------------.
 | Register (0x00000c80 + (N) * 0x04) RX_STG_2_APS_LEVEL |
 +-------------------------------------------------------+
 | bit  2:0 R/W  REG_RX_STG_2_APS_LEVEL                  |
 +------------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_APS_LEVEL_UNUSED_MASK                0xfffffff8
#define ODU_RCP_REG_RX_STG_2_APS_LEVEL_BIT_REG_RX_STG_2_APS_LEVEL_MSK 0x00000007
#define ODU_RCP_REG_RX_STG_2_APS_LEVEL_BIT_REG_RX_STG_2_APS_LEVEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL */
#define PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL_INDEX_N_MIN                 0
#define PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL_INDEX_N_MAX                 95
#define PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL_INDEX_N_SIZE                96
#define PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL_INDEX_N_OFFSET              0x04

/*--------------------------------------------------------.
 | Register (0x00000e00 + (N) * 0x04) RX_STG_3A_APS_LEVEL |
 +--------------------------------------------------------+
 | bit  2:0 R/W  REG_RX_STG_3A_APS_LEVEL                  |
 +-------------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_APS_LEVEL_UNUSED_MASK                 0xfffffff8
#define ODU_RCP_REG_RX_STG_3A_APS_LEVEL_BIT_REG_RX_STG_3A_APS_LEVEL_MSK 0x00000007
#define ODU_RCP_REG_RX_STG_3A_APS_LEVEL_BIT_REG_RX_STG_3A_APS_LEVEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL */
#define PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL_INDEX_N_MIN                 0
#define PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL_INDEX_N_MAX                 95
#define PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL_INDEX_N_SIZE                96
#define PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL_INDEX_N_OFFSET              0x04

/*--------------------------------------------------------.
 | Register (0x00000f80 + (N) * 0x04) RX_STG_3B_APS_LEVEL |
 +--------------------------------------------------------+
 | bit  2:0 R/W  REG_RX_STG_3B_APS_LEVEL                  |
 +-------------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_APS_LEVEL_UNUSED_MASK                 0xfffffff8
#define ODU_RCP_REG_RX_STG_3B_APS_LEVEL_BIT_REG_RX_STG_3B_APS_LEVEL_MSK 0x00000007
#define ODU_RCP_REG_RX_STG_3B_APS_LEVEL_BIT_REG_RX_STG_3B_APS_LEVEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL */
#define PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL_INDEX_N_MAX                95
#define PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL_INDEX_N_SIZE               96
#define PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL_INDEX_N_OFFSET             0x04

/*-------------------------------------------------------.
 | Register (0x00001100 + (N) * 0x04) RX_STG_4_APS_LEVEL |
 +-------------------------------------------------------+
 | bit  2:0 R/W  REG_RX_STG_4_APS_LEVEL                  |
 +------------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_APS_LEVEL_UNUSED_MASK                0xfffffff8
#define ODU_RCP_REG_RX_STG_4_APS_LEVEL_BIT_REG_RX_STG_4_APS_LEVEL_MSK 0x00000007
#define ODU_RCP_REG_RX_STG_4_APS_LEVEL_BIT_REG_RX_STG_4_APS_LEVEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL */
#define PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL_INDEX_N_MAX                11
#define PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL_INDEX_N_SIZE               12
#define PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL_INDEX_N_OFFSET             0x04

/*-------------------------------------------------------.
 | Register (0x00001280 + (N) * 0x04) RX_STG_N_APS_LEVEL |
 +-------------------------------------------------------+
 | bit  2:0 R/W  REG_RX_STG_N_APS_LEVEL                  |
 +------------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_N_APS_LEVEL_UNUSED_MASK                0xfffffff8
#define ODU_RCP_REG_RX_STG_N_APS_LEVEL_BIT_REG_RX_STG_N_APS_LEVEL_MSK 0x00000007
#define ODU_RCP_REG_RX_STG_N_APS_LEVEL_BIT_REG_RX_STG_N_APS_LEVEL_OFF 0

/*---------------------------------.
 | Register 0x00001300 CFC_DBG_SEL |
 +---------------------------------+
 | bit  4   R/W  CFC_DBG_SRC_SEL   |
 | bit  2:0 R/W  CFC_DBG_STG_SEL   |
 +--------------------------------*/
#define PMC_ODU_RCP_REG_CFC_DBG_SEL_UNUSED_MASK         0xffffffe8
#define ODU_RCP_REG_CFC_DBG_SEL_BIT_CFC_DBG_SRC_SEL_MSK 0x00000010
#define ODU_RCP_REG_CFC_DBG_SEL_BIT_CFC_DBG_SRC_SEL_OFF 4
#define ODU_RCP_REG_CFC_DBG_SEL_BIT_CFC_DBG_STG_SEL_MSK 0x00000007
#define ODU_RCP_REG_CFC_DBG_SEL_BIT_CFC_DBG_STG_SEL_OFF 0

/*------------------------------.
 | Register 0x00001304 DBG_CTRL |
 +------------------------------+
 | bit  16   R/W  DBG_TRIG      |
 | bit  10:0 R/W  DBG_RADDR     |
 +-----------------------------*/
#define PMC_ODU_RCP_REG_DBG_CTRL_UNUSED_MASK   0xfffef800
#define ODU_RCP_REG_DBG_CTRL_BIT_DBG_TRIG_MSK  0x00010000
#define ODU_RCP_REG_DBG_CTRL_BIT_DBG_TRIG_OFF  16
#define ODU_RCP_REG_DBG_CTRL_BIT_DBG_RADDR_MSK 0x000007ff
#define ODU_RCP_REG_DBG_CTRL_BIT_DBG_RADDR_OFF 0

/*-------------------------------.
 | Register 0x00001308 DBG_RDATA |
 +-------------------------------+
 | bit  31:0 R  DBG_RDATA        |
 +------------------------------*/
#define PMC_ODU_RCP_REG_DBG_RDATA_UNUSED_MASK   0x00000000
#define ODU_RCP_REG_DBG_RDATA_BIT_DBG_RDATA_MSK 0xffffffff
#define ODU_RCP_REG_DBG_RDATA_BIT_DBG_RDATA_OFF 0

/*-----------------------------------.
 | Register 0x00001320 CFC_IRQ_SEL   |
 +-----------------------------------+
 | bit  4   R/W  REG_CFC_IRQ_SRC_SEL |
 | bit  2:0 R/W  REG_CFC_IRQ_STG_SEL |
 +----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IRQ_SEL_UNUSED_MASK             0xffffffe8
#define ODU_RCP_REG_CFC_IRQ_SEL_BIT_REG_CFC_IRQ_SRC_SEL_MSK 0x00000010
#define ODU_RCP_REG_CFC_IRQ_SEL_BIT_REG_CFC_IRQ_SRC_SEL_OFF 4
#define ODU_RCP_REG_CFC_IRQ_SEL_BIT_REG_CFC_IRQ_STG_SEL_MSK 0x00000007
#define ODU_RCP_REG_CFC_IRQ_SEL_BIT_REG_CFC_IRQ_STG_SEL_OFF 0

/*----------------------------------------.
 | Register 0x00001324 CFC_IRQ_TYPE_SEL_0 |
 +----------------------------------------+
 | bit  31:0 R/W  REG_CFC_IRQ_TYPE_SEL    |
 +---------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IRQ_TYPE_SEL_0_UNUSED_MASK              0x00000000
#define ODU_RCP_REG_CFC_IRQ_TYPE_SEL_0_BIT_REG_CFC_IRQ_TYPE_SEL_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IRQ_TYPE_SEL_0_BIT_REG_CFC_IRQ_TYPE_SEL_OFF 0

/*----------------------------------------.
 | Register 0x00001328 CFC_IRQ_TYPE_SEL_1 |
 +----------------------------------------+
 | bit  31:0 R/W  REG_CFC_IRQ_TYPE_SEL    |
 +---------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IRQ_TYPE_SEL_1_UNUSED_MASK              0x00000000
#define ODU_RCP_REG_CFC_IRQ_TYPE_SEL_1_BIT_REG_CFC_IRQ_TYPE_SEL_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IRQ_TYPE_SEL_1_BIT_REG_CFC_IRQ_TYPE_SEL_OFF 0

/*----------------------------------------.
 | Register 0x0000132c CFC_IRQ_TYPE_SEL_2 |
 +----------------------------------------+
 | bit  31:0 R/W  REG_CFC_IRQ_TYPE_SEL    |
 +---------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IRQ_TYPE_SEL_2_UNUSED_MASK              0x00000000
#define ODU_RCP_REG_CFC_IRQ_TYPE_SEL_2_BIT_REG_CFC_IRQ_TYPE_SEL_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IRQ_TYPE_SEL_2_BIT_REG_CFC_IRQ_TYPE_SEL_OFF 0

/*------------------------------------.
 | Register 0x00001330 CFC_FIFO_IRQ_0 |
 +------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_IRQ_I      |
 +-----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_0_UNUSED_MASK        0x00000000
#define ODU_RCP_REG_CFC_FIFO_IRQ_0_BIT_CFC_FIFO_IRQ_I_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FIFO_IRQ_0_BIT_CFC_FIFO_IRQ_I_OFF 0

/*------------------------------------.
 | Register 0x00001334 CFC_FIFO_IRQ_1 |
 +------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_IRQ_I      |
 +-----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_1_UNUSED_MASK        0x00000000
#define ODU_RCP_REG_CFC_FIFO_IRQ_1_BIT_CFC_FIFO_IRQ_I_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FIFO_IRQ_1_BIT_CFC_FIFO_IRQ_I_OFF 0

/*------------------------------------.
 | Register 0x00001338 CFC_FIFO_IRQ_2 |
 +------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_IRQ_I      |
 +-----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_2_UNUSED_MASK        0x00000000
#define ODU_RCP_REG_CFC_FIFO_IRQ_2_BIT_CFC_FIFO_IRQ_I_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FIFO_IRQ_2_BIT_CFC_FIFO_IRQ_I_OFF 0

/*---------------------------------------.
 | Register 0x00001340 CFC_IPT_ERR_SEL   |
 +---------------------------------------+
 | bit  4   R/W  REG_CFC_IPT_ERR_SRC_SEL |
 | bit  2:0 R/W  REG_CFC_IPT_ERR_STG_SEL |
 +--------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_SEL_UNUSED_MASK                 0xffffffe8
#define ODU_RCP_REG_CFC_IPT_ERR_SEL_BIT_REG_CFC_IPT_ERR_SRC_SEL_MSK 0x00000010
#define ODU_RCP_REG_CFC_IPT_ERR_SEL_BIT_REG_CFC_IPT_ERR_SRC_SEL_OFF 4
#define ODU_RCP_REG_CFC_IPT_ERR_SEL_BIT_REG_CFC_IPT_ERR_STG_SEL_MSK 0x00000007
#define ODU_RCP_REG_CFC_IPT_ERR_SEL_BIT_REG_CFC_IPT_ERR_STG_SEL_OFF 0

/*-----------------------------------.
 | Register 0x00001344 CFC_IPT_ERR_0 |
 +-----------------------------------+
 | bit  31:0 R/W  CFC_IPT_ERR_I      |
 +----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_0_UNUSED_MASK       0x00000000
#define ODU_RCP_REG_CFC_IPT_ERR_0_BIT_CFC_IPT_ERR_I_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IPT_ERR_0_BIT_CFC_IPT_ERR_I_OFF 0

/*-----------------------------------.
 | Register 0x00001348 CFC_IPT_ERR_1 |
 +-----------------------------------+
 | bit  31:0 R/W  CFC_IPT_ERR_I      |
 +----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_1_UNUSED_MASK       0x00000000
#define ODU_RCP_REG_CFC_IPT_ERR_1_BIT_CFC_IPT_ERR_I_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IPT_ERR_1_BIT_CFC_IPT_ERR_I_OFF 0

/*-----------------------------------.
 | Register 0x0000134c CFC_IPT_ERR_2 |
 +-----------------------------------+
 | bit  31:0 R/W  CFC_IPT_ERR_I      |
 +----------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_2_UNUSED_MASK       0x00000000
#define ODU_RCP_REG_CFC_IPT_ERR_2_BIT_CFC_IPT_ERR_I_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IPT_ERR_2_BIT_CFC_IPT_ERR_I_OFF 0

/*---------------------------------------.
 | Register 0x00001360 CFC_FIFO_IRQ_EN_0 |
 +---------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_IRQ_E         |
 +--------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_EN_0_UNUSED_MASK        0x00000000
#define ODU_RCP_REG_CFC_FIFO_IRQ_EN_0_BIT_CFC_FIFO_IRQ_E_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FIFO_IRQ_EN_0_BIT_CFC_FIFO_IRQ_E_OFF 0

/*---------------------------------------.
 | Register 0x00001364 CFC_FIFO_IRQ_EN_1 |
 +---------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_IRQ_E         |
 +--------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_EN_1_UNUSED_MASK        0x00000000
#define ODU_RCP_REG_CFC_FIFO_IRQ_EN_1_BIT_CFC_FIFO_IRQ_E_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FIFO_IRQ_EN_1_BIT_CFC_FIFO_IRQ_E_OFF 0

/*---------------------------------------.
 | Register 0x00001368 CFC_FIFO_IRQ_EN_2 |
 +---------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_IRQ_E         |
 +--------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FIFO_IRQ_EN_2_UNUSED_MASK        0x00000000
#define ODU_RCP_REG_CFC_FIFO_IRQ_EN_2_BIT_CFC_FIFO_IRQ_E_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FIFO_IRQ_EN_2_BIT_CFC_FIFO_IRQ_E_OFF 0

/*--------------------------------------.
 | Register 0x00001380 CFC_IPT_ERR_EN_0 |
 +--------------------------------------+
 | bit  31:0 R/W  CFC_IPT_ERR_E         |
 +-------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_EN_0_UNUSED_MASK       0x00000000
#define ODU_RCP_REG_CFC_IPT_ERR_EN_0_BIT_CFC_IPT_ERR_E_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IPT_ERR_EN_0_BIT_CFC_IPT_ERR_E_OFF 0

/*--------------------------------------.
 | Register 0x00001384 CFC_IPT_ERR_EN_1 |
 +--------------------------------------+
 | bit  31:0 R/W  CFC_IPT_ERR_E         |
 +-------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_EN_1_UNUSED_MASK       0x00000000
#define ODU_RCP_REG_CFC_IPT_ERR_EN_1_BIT_CFC_IPT_ERR_E_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IPT_ERR_EN_1_BIT_CFC_IPT_ERR_E_OFF 0

/*--------------------------------------.
 | Register 0x00001388 CFC_IPT_ERR_EN_2 |
 +--------------------------------------+
 | bit  31:0 R/W  CFC_IPT_ERR_E         |
 +-------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_IPT_ERR_EN_2_UNUSED_MASK       0x00000000
#define ODU_RCP_REG_CFC_IPT_ERR_EN_2_BIT_CFC_IPT_ERR_E_MSK 0xffffffff
#define ODU_RCP_REG_CFC_IPT_ERR_EN_2_BIT_CFC_IPT_ERR_E_OFF 0

/*--------------------------------------.
 | Register 0x000013c0 CFC_STATUS_SEL   |
 +--------------------------------------+
 | bit  4   R/W  REG_CFC_STATUS_SRC_SEL |
 | bit  2:0 R/W  REG_CFC_STATUS_STG_SEL |
 +-------------------------------------*/
#define PMC_ODU_RCP_REG_CFC_STATUS_SEL_UNUSED_MASK                0xffffffe8
#define ODU_RCP_REG_CFC_STATUS_SEL_BIT_REG_CFC_STATUS_SRC_SEL_MSK 0x00000010
#define ODU_RCP_REG_CFC_STATUS_SEL_BIT_REG_CFC_STATUS_SRC_SEL_OFF 4
#define ODU_RCP_REG_CFC_STATUS_SEL_BIT_REG_CFC_STATUS_STG_SEL_MSK 0x00000007
#define ODU_RCP_REG_CFC_STATUS_SEL_BIT_REG_CFC_STATUS_STG_SEL_OFF 0

/*---------------------------------.
 | Register 0x000013c4 CFC_EMPTY_0 |
 +---------------------------------+
 | bit  31:0 R  CFC_EMPTY          |
 +--------------------------------*/
#define PMC_ODU_RCP_REG_CFC_EMPTY_0_UNUSED_MASK   0x00000000
#define ODU_RCP_REG_CFC_EMPTY_0_BIT_CFC_EMPTY_MSK 0xffffffff
#define ODU_RCP_REG_CFC_EMPTY_0_BIT_CFC_EMPTY_OFF 0

/*---------------------------------.
 | Register 0x000013c8 CFC_EMPTY_1 |
 +---------------------------------+
 | bit  31:0 R  CFC_EMPTY          |
 +--------------------------------*/
#define PMC_ODU_RCP_REG_CFC_EMPTY_1_UNUSED_MASK   0x00000000
#define ODU_RCP_REG_CFC_EMPTY_1_BIT_CFC_EMPTY_MSK 0xffffffff
#define ODU_RCP_REG_CFC_EMPTY_1_BIT_CFC_EMPTY_OFF 0

/*---------------------------------.
 | Register 0x000013cc CFC_EMPTY_2 |
 +---------------------------------+
 | bit  31:0 R  CFC_EMPTY          |
 +--------------------------------*/
#define PMC_ODU_RCP_REG_CFC_EMPTY_2_UNUSED_MASK   0x00000000
#define ODU_RCP_REG_CFC_EMPTY_2_BIT_CFC_EMPTY_MSK 0xffffffff
#define ODU_RCP_REG_CFC_EMPTY_2_BIT_CFC_EMPTY_OFF 0

/*--------------------------------.
 | Register 0x000013d0 CFC_FULL_0 |
 +--------------------------------+
 | bit  31:0 R  CFC_FULL          |
 +-------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FULL_0_UNUSED_MASK  0x00000000
#define ODU_RCP_REG_CFC_FULL_0_BIT_CFC_FULL_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FULL_0_BIT_CFC_FULL_OFF 0

/*--------------------------------.
 | Register 0x000013d4 CFC_FULL_1 |
 +--------------------------------+
 | bit  31:0 R  CFC_FULL          |
 +-------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FULL_1_UNUSED_MASK  0x00000000
#define ODU_RCP_REG_CFC_FULL_1_BIT_CFC_FULL_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FULL_1_BIT_CFC_FULL_OFF 0

/*--------------------------------.
 | Register 0x000013d8 CFC_FULL_2 |
 +--------------------------------+
 | bit  31:0 R  CFC_FULL          |
 +-------------------------------*/
#define PMC_ODU_RCP_REG_CFC_FULL_2_UNUSED_MASK  0x00000000
#define ODU_RCP_REG_CFC_FULL_2_BIT_CFC_FULL_MSK 0xffffffff
#define ODU_RCP_REG_CFC_FULL_2_BIT_CFC_FULL_OFF 0

/*------------------------------------------.
 | Register 0x00001800 RX_STG_1_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  11:0 R/W  RX_STG_1_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_1_FIFO_CLRB_0_UNUSED_MASK            0xfffff000
#define ODU_RCP_REG_RX_STG_1_FIFO_CLRB_0_BIT_RX_STG_1_FIFO_CLRB_MSK 0x00000fff
#define ODU_RCP_REG_RX_STG_1_FIFO_CLRB_0_BIT_RX_STG_1_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00001820 RX_STG_1_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  RX_STG_1_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  RX_STG_1_CFC_END_ADDR_CHG_REQ   |
 | bit  19:16 R/W  RX_STG_1_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_CFG_UNUSED_MASK                         0xeef0ffff
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_BIT_RX_STG_1_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_BIT_RX_STG_1_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_BIT_RX_STG_1_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_BIT_RX_STG_1_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_BIT_RX_STG_1_CFC_END_ADDR_CHG_ID_MSK    0x000f0000
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_BIT_RX_STG_1_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00001824 RX_STG_1_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  RX_STG_1_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_STAT_BIT_RX_STG_1_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_STAT_BIT_RX_STG_1_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00001828 RX_STG_1_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  4:0 R/W  RX_STG_1_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_CFG_VAL_UNUSED_MASK               0xffffffe0
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_VAL_BIT_RX_STG_1_CFC_END_ADDR_MSK 0x0000001f
#define ODU_RCP_REG_RX_STG_1_CFC_CFG_VAL_BIT_RX_STG_1_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT */
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT_INDEX_N_MAX                11
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT_INDEX_N_SIZE               12
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00001840 + (N) * 0x04) RX_STG_1_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  RX_STG_1_CFC_IPT_VALID              |
 | bit  19:16 R/W  RX_STG_1_CFC_IPT_ID                 |
 | bit  3:0   R/W  RX_STG_1_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_1_CFC_IPT_UNUSED_MASK                0xfef0fff0
#define ODU_RCP_REG_RX_STG_1_CFC_IPT_BIT_RX_STG_1_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_RX_STG_1_CFC_IPT_BIT_RX_STG_1_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_RX_STG_1_CFC_IPT_BIT_RX_STG_1_CFC_IPT_ID_MSK    0x000f0000
#define ODU_RCP_REG_RX_STG_1_CFC_IPT_BIT_RX_STG_1_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_RX_STG_1_CFC_IPT_BIT_RX_STG_1_CFC_IPT_VA_MSK    0x0000000f
#define ODU_RCP_REG_RX_STG_1_CFC_IPT_BIT_RX_STG_1_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00001a00 RX_STG_2_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  31:0 R/W  RX_STG_2_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_FIFO_CLRB_0_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_RX_STG_2_FIFO_CLRB_0_BIT_RX_STG_2_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_2_FIFO_CLRB_0_BIT_RX_STG_2_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00001a04 RX_STG_2_FIFO_CLRB_1 |
 +------------------------------------------+
 | bit  31:0 R/W  RX_STG_2_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_FIFO_CLRB_1_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_RX_STG_2_FIFO_CLRB_1_BIT_RX_STG_2_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_2_FIFO_CLRB_1_BIT_RX_STG_2_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00001a08 RX_STG_2_FIFO_CLRB_2 |
 +------------------------------------------+
 | bit  31:0 R/W  RX_STG_2_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_FIFO_CLRB_2_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_RX_STG_2_FIFO_CLRB_2_BIT_RX_STG_2_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_2_FIFO_CLRB_2_BIT_RX_STG_2_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00001a20 RX_STG_2_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  RX_STG_2_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  RX_STG_2_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  RX_STG_2_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_CFG_UNUSED_MASK                         0xee80ffff
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_BIT_RX_STG_2_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_BIT_RX_STG_2_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_BIT_RX_STG_2_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_BIT_RX_STG_2_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_BIT_RX_STG_2_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_BIT_RX_STG_2_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00001a24 RX_STG_2_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  RX_STG_2_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_STAT_BIT_RX_STG_2_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_STAT_BIT_RX_STG_2_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00001a28 RX_STG_2_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  7:0 R/W  RX_STG_2_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_CFG_VAL_UNUSED_MASK               0xffffff00
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_VAL_BIT_RX_STG_2_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_RX_STG_2_CFC_CFG_VAL_BIT_RX_STG_2_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT */
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT_INDEX_N_MAX                95
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT_INDEX_N_SIZE               96
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00001a40 + (N) * 0x04) RX_STG_2_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  RX_STG_2_CFC_IPT_VALID              |
 | bit  22:16 R/W  RX_STG_2_CFC_IPT_ID                 |
 | bit  6:0   R/W  RX_STG_2_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_2_CFC_IPT_UNUSED_MASK                0xfe80ff80
#define ODU_RCP_REG_RX_STG_2_CFC_IPT_BIT_RX_STG_2_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_RX_STG_2_CFC_IPT_BIT_RX_STG_2_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_RX_STG_2_CFC_IPT_BIT_RX_STG_2_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_2_CFC_IPT_BIT_RX_STG_2_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_RX_STG_2_CFC_IPT_BIT_RX_STG_2_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_RX_STG_2_CFC_IPT_BIT_RX_STG_2_CFC_IPT_VA_OFF    0

/*-------------------------------------------.
 | Register 0x00001c00 RX_STG_3A_FIFO_CLRB_0 |
 +-------------------------------------------+
 | bit  31:0 R/W  RX_STG_3A_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_0_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_0_BIT_RX_STG_3A_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_0_BIT_RX_STG_3A_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00001c04 RX_STG_3A_FIFO_CLRB_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  RX_STG_3A_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_1_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_1_BIT_RX_STG_3A_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_1_BIT_RX_STG_3A_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00001c08 RX_STG_3A_FIFO_CLRB_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  RX_STG_3A_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_2_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_2_BIT_RX_STG_3A_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_3A_FIFO_CLRB_2_BIT_RX_STG_3A_FIFO_CLRB_OFF 0

/*--------------------------------------------------.
 | Register 0x00001c20 RX_STG_3A_CFC_CFG            |
 +--------------------------------------------------+
 | bit  28    R/W  RX_STG_3A_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  RX_STG_3A_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  RX_STG_3A_CFC_END_ADDR_CHG_ID    |
 +-------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_CFG_UNUSED_MASK                          0xee80ffff
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_BIT_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_BIT_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_BIT_RX_STG_3A_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_BIT_RX_STG_3A_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_BIT_RX_STG_3A_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_BIT_RX_STG_3A_CFC_END_ADDR_CHG_ID_OFF    16

/*----------------------------------------------.
 | Register 0x00001c24 RX_STG_3A_CFC_CFG_STAT   |
 +----------------------------------------------+
 | bit  0 R  RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS |
 +---------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_CFG_STAT_UNUSED_MASK                            0xfffffffe
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_STAT_BIT_RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_STAT_BIT_RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*-------------------------------------------.
 | Register 0x00001c28 RX_STG_3A_CFC_CFG_VAL |
 +-------------------------------------------+
 | bit  7:0 R/W  RX_STG_3A_CFC_END_ADDR      |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_CFG_VAL_UNUSED_MASK                0xffffff00
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_VAL_BIT_RX_STG_3A_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_RX_STG_3A_CFC_CFG_VAL_BIT_RX_STG_3A_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT */
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT_INDEX_N_MIN                 0
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT_INDEX_N_MAX                 95
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT_INDEX_N_SIZE                96
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT_INDEX_N_OFFSET              0x04

/*------------------------------------------------------.
 | Register (0x00001c40 + (N) * 0x04) RX_STG_3A_CFC_IPT |
 +------------------------------------------------------+
 | bit  24    R/W  RX_STG_3A_CFC_IPT_VALID              |
 | bit  22:16 R/W  RX_STG_3A_CFC_IPT_ID                 |
 | bit  6:0   R/W  RX_STG_3A_CFC_IPT_VA                 |
 +-----------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3A_CFC_IPT_UNUSED_MASK                 0xfe80ff80
#define ODU_RCP_REG_RX_STG_3A_CFC_IPT_BIT_RX_STG_3A_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_RX_STG_3A_CFC_IPT_BIT_RX_STG_3A_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_RX_STG_3A_CFC_IPT_BIT_RX_STG_3A_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_3A_CFC_IPT_BIT_RX_STG_3A_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_RX_STG_3A_CFC_IPT_BIT_RX_STG_3A_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_RX_STG_3A_CFC_IPT_BIT_RX_STG_3A_CFC_IPT_VA_OFF    0

/*-------------------------------------------.
 | Register 0x00001e00 RX_STG_3B_FIFO_CLRB_0 |
 +-------------------------------------------+
 | bit  31:0 R/W  RX_STG_3B_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_0_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_0_BIT_RX_STG_3B_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_0_BIT_RX_STG_3B_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00001e04 RX_STG_3B_FIFO_CLRB_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  RX_STG_3B_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_1_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_1_BIT_RX_STG_3B_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_1_BIT_RX_STG_3B_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00001e08 RX_STG_3B_FIFO_CLRB_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  RX_STG_3B_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_2_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_2_BIT_RX_STG_3B_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_3B_FIFO_CLRB_2_BIT_RX_STG_3B_FIFO_CLRB_OFF 0

/*--------------------------------------------------.
 | Register 0x00001e20 RX_STG_3B_CFC_CFG            |
 +--------------------------------------------------+
 | bit  28    R/W  RX_STG_3B_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  RX_STG_3B_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  RX_STG_3B_CFC_END_ADDR_CHG_ID    |
 +-------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_CFG_UNUSED_MASK                          0xee80ffff
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_BIT_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_BIT_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_BIT_RX_STG_3B_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_BIT_RX_STG_3B_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_BIT_RX_STG_3B_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_BIT_RX_STG_3B_CFC_END_ADDR_CHG_ID_OFF    16

/*----------------------------------------------.
 | Register 0x00001e24 RX_STG_3B_CFC_CFG_STAT   |
 +----------------------------------------------+
 | bit  0 R  RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS |
 +---------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_CFG_STAT_UNUSED_MASK                            0xfffffffe
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_STAT_BIT_RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_STAT_BIT_RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*-------------------------------------------.
 | Register 0x00001e28 RX_STG_3B_CFC_CFG_VAL |
 +-------------------------------------------+
 | bit  7:0 R/W  RX_STG_3B_CFC_END_ADDR      |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_CFG_VAL_UNUSED_MASK                0xffffff00
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_VAL_BIT_RX_STG_3B_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_RX_STG_3B_CFC_CFG_VAL_BIT_RX_STG_3B_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT */
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT_INDEX_N_MIN                 0
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT_INDEX_N_MAX                 95
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT_INDEX_N_SIZE                96
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT_INDEX_N_OFFSET              0x04

/*------------------------------------------------------.
 | Register (0x00001e40 + (N) * 0x04) RX_STG_3B_CFC_IPT |
 +------------------------------------------------------+
 | bit  24    R/W  RX_STG_3B_CFC_IPT_VALID              |
 | bit  22:16 R/W  RX_STG_3B_CFC_IPT_ID                 |
 | bit  6:0   R/W  RX_STG_3B_CFC_IPT_VA                 |
 +-----------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_3B_CFC_IPT_UNUSED_MASK                 0xfe80ff80
#define ODU_RCP_REG_RX_STG_3B_CFC_IPT_BIT_RX_STG_3B_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_RX_STG_3B_CFC_IPT_BIT_RX_STG_3B_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_RX_STG_3B_CFC_IPT_BIT_RX_STG_3B_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_3B_CFC_IPT_BIT_RX_STG_3B_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_RX_STG_3B_CFC_IPT_BIT_RX_STG_3B_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_RX_STG_3B_CFC_IPT_BIT_RX_STG_3B_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00002000 RX_STG_4_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  31:0 R/W  RX_STG_4_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_FIFO_CLRB_0_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_RX_STG_4_FIFO_CLRB_0_BIT_RX_STG_4_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_4_FIFO_CLRB_0_BIT_RX_STG_4_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00002004 RX_STG_4_FIFO_CLRB_1 |
 +------------------------------------------+
 | bit  31:0 R/W  RX_STG_4_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_FIFO_CLRB_1_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_RX_STG_4_FIFO_CLRB_1_BIT_RX_STG_4_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_4_FIFO_CLRB_1_BIT_RX_STG_4_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00002008 RX_STG_4_FIFO_CLRB_2 |
 +------------------------------------------+
 | bit  31:0 R/W  RX_STG_4_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_FIFO_CLRB_2_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_RX_STG_4_FIFO_CLRB_2_BIT_RX_STG_4_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_RX_STG_4_FIFO_CLRB_2_BIT_RX_STG_4_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00002020 RX_STG_4_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  RX_STG_4_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  RX_STG_4_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  RX_STG_4_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_CFG_UNUSED_MASK                         0xee80ffff
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_BIT_RX_STG_4_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_BIT_RX_STG_4_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_BIT_RX_STG_4_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_BIT_RX_STG_4_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_BIT_RX_STG_4_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_BIT_RX_STG_4_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00002024 RX_STG_4_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  RX_STG_4_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_STAT_BIT_RX_STG_4_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_STAT_BIT_RX_STG_4_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00002028 RX_STG_4_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  7:0 R/W  RX_STG_4_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_CFG_VAL_UNUSED_MASK               0xffffff00
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_VAL_BIT_RX_STG_4_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_RX_STG_4_CFC_CFG_VAL_BIT_RX_STG_4_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT */
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT_INDEX_N_MAX                95
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT_INDEX_N_SIZE               96
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00002040 + (N) * 0x04) RX_STG_4_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  RX_STG_4_CFC_IPT_VALID              |
 | bit  22:16 R/W  RX_STG_4_CFC_IPT_ID                 |
 | bit  6:0   R/W  RX_STG_4_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_4_CFC_IPT_UNUSED_MASK                0xfe80ff80
#define ODU_RCP_REG_RX_STG_4_CFC_IPT_BIT_RX_STG_4_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_RX_STG_4_CFC_IPT_BIT_RX_STG_4_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_RX_STG_4_CFC_IPT_BIT_RX_STG_4_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_RX_STG_4_CFC_IPT_BIT_RX_STG_4_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_RX_STG_4_CFC_IPT_BIT_RX_STG_4_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_RX_STG_4_CFC_IPT_BIT_RX_STG_4_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00002200 RX_STG_N_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  11:0 R/W  RX_STG_N_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_N_FIFO_CLRB_0_UNUSED_MASK            0xfffff000
#define ODU_RCP_REG_RX_STG_N_FIFO_CLRB_0_BIT_RX_STG_N_FIFO_CLRB_MSK 0x00000fff
#define ODU_RCP_REG_RX_STG_N_FIFO_CLRB_0_BIT_RX_STG_N_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00002220 RX_STG_N_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  RX_STG_N_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  RX_STG_N_CFC_END_ADDR_CHG_REQ   |
 | bit  19:16 R/W  RX_STG_N_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_CFG_UNUSED_MASK                         0xeef0ffff
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_BIT_RX_STG_N_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_BIT_RX_STG_N_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_BIT_RX_STG_N_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_BIT_RX_STG_N_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_BIT_RX_STG_N_CFC_END_ADDR_CHG_ID_MSK    0x000f0000
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_BIT_RX_STG_N_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00002224 RX_STG_N_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  RX_STG_N_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_STAT_BIT_RX_STG_N_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_STAT_BIT_RX_STG_N_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00002228 RX_STG_N_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  4:0 R/W  RX_STG_N_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_CFG_VAL_UNUSED_MASK               0xffffffe0
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_VAL_BIT_RX_STG_N_CFC_END_ADDR_MSK 0x0000001f
#define ODU_RCP_REG_RX_STG_N_CFC_CFG_VAL_BIT_RX_STG_N_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT */
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT_INDEX_N_MAX                11
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT_INDEX_N_SIZE               12
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00002240 + (N) * 0x04) RX_STG_N_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  RX_STG_N_CFC_IPT_VALID              |
 | bit  19:16 R/W  RX_STG_N_CFC_IPT_ID                 |
 | bit  3:0   R/W  RX_STG_N_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_RX_STG_N_CFC_IPT_UNUSED_MASK                0xfef0fff0
#define ODU_RCP_REG_RX_STG_N_CFC_IPT_BIT_RX_STG_N_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_RX_STG_N_CFC_IPT_BIT_RX_STG_N_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_RX_STG_N_CFC_IPT_BIT_RX_STG_N_CFC_IPT_ID_MSK    0x000f0000
#define ODU_RCP_REG_RX_STG_N_CFC_IPT_BIT_RX_STG_N_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_RX_STG_N_CFC_IPT_BIT_RX_STG_N_CFC_IPT_VA_MSK    0x0000000f
#define ODU_RCP_REG_RX_STG_N_CFC_IPT_BIT_RX_STG_N_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00002400 TX_STG_1_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  11:0 R/W  TX_STG_1_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_1_FIFO_CLRB_0_UNUSED_MASK            0xfffff000
#define ODU_RCP_REG_TX_STG_1_FIFO_CLRB_0_BIT_TX_STG_1_FIFO_CLRB_MSK 0x00000fff
#define ODU_RCP_REG_TX_STG_1_FIFO_CLRB_0_BIT_TX_STG_1_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00002420 TX_STG_1_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  TX_STG_1_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  TX_STG_1_CFC_END_ADDR_CHG_REQ   |
 | bit  19:16 R/W  TX_STG_1_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_CFG_UNUSED_MASK                         0xeef0ffff
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_BIT_TX_STG_1_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_BIT_TX_STG_1_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_BIT_TX_STG_1_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_BIT_TX_STG_1_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_BIT_TX_STG_1_CFC_END_ADDR_CHG_ID_MSK    0x000f0000
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_BIT_TX_STG_1_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00002424 TX_STG_1_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  TX_STG_1_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_STAT_BIT_TX_STG_1_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_STAT_BIT_TX_STG_1_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00002428 TX_STG_1_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  4:0 R/W  TX_STG_1_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_CFG_VAL_UNUSED_MASK               0xffffffe0
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_VAL_BIT_TX_STG_1_CFC_END_ADDR_MSK 0x0000001f
#define ODU_RCP_REG_TX_STG_1_CFC_CFG_VAL_BIT_TX_STG_1_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT */
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT_INDEX_N_MAX                11
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT_INDEX_N_SIZE               12
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00002440 + (N) * 0x04) TX_STG_1_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  TX_STG_1_CFC_IPT_VALID              |
 | bit  19:16 R/W  TX_STG_1_CFC_IPT_ID                 |
 | bit  3:0   R/W  TX_STG_1_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_1_CFC_IPT_UNUSED_MASK                0xfef0fff0
#define ODU_RCP_REG_TX_STG_1_CFC_IPT_BIT_TX_STG_1_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_TX_STG_1_CFC_IPT_BIT_TX_STG_1_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_TX_STG_1_CFC_IPT_BIT_TX_STG_1_CFC_IPT_ID_MSK    0x000f0000
#define ODU_RCP_REG_TX_STG_1_CFC_IPT_BIT_TX_STG_1_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_TX_STG_1_CFC_IPT_BIT_TX_STG_1_CFC_IPT_VA_MSK    0x0000000f
#define ODU_RCP_REG_TX_STG_1_CFC_IPT_BIT_TX_STG_1_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00002600 TX_STG_2_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  31:0 R/W  TX_STG_2_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_FIFO_CLRB_0_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_TX_STG_2_FIFO_CLRB_0_BIT_TX_STG_2_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_2_FIFO_CLRB_0_BIT_TX_STG_2_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00002604 TX_STG_2_FIFO_CLRB_1 |
 +------------------------------------------+
 | bit  31:0 R/W  TX_STG_2_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_FIFO_CLRB_1_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_TX_STG_2_FIFO_CLRB_1_BIT_TX_STG_2_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_2_FIFO_CLRB_1_BIT_TX_STG_2_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00002608 TX_STG_2_FIFO_CLRB_2 |
 +------------------------------------------+
 | bit  31:0 R/W  TX_STG_2_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_FIFO_CLRB_2_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_TX_STG_2_FIFO_CLRB_2_BIT_TX_STG_2_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_2_FIFO_CLRB_2_BIT_TX_STG_2_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00002620 TX_STG_2_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  TX_STG_2_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  TX_STG_2_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  TX_STG_2_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_CFG_UNUSED_MASK                         0xee80ffff
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_BIT_TX_STG_2_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_BIT_TX_STG_2_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_BIT_TX_STG_2_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_BIT_TX_STG_2_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_BIT_TX_STG_2_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_BIT_TX_STG_2_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00002624 TX_STG_2_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  TX_STG_2_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_STAT_BIT_TX_STG_2_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_STAT_BIT_TX_STG_2_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00002628 TX_STG_2_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  7:0 R/W  TX_STG_2_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_CFG_VAL_UNUSED_MASK               0xffffff00
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_VAL_BIT_TX_STG_2_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_TX_STG_2_CFC_CFG_VAL_BIT_TX_STG_2_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT */
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT_INDEX_N_MAX                95
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT_INDEX_N_SIZE               96
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00002640 + (N) * 0x04) TX_STG_2_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  TX_STG_2_CFC_IPT_VALID              |
 | bit  22:16 R/W  TX_STG_2_CFC_IPT_ID                 |
 | bit  6:0   R/W  TX_STG_2_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_2_CFC_IPT_UNUSED_MASK                0xfe80ff80
#define ODU_RCP_REG_TX_STG_2_CFC_IPT_BIT_TX_STG_2_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_TX_STG_2_CFC_IPT_BIT_TX_STG_2_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_TX_STG_2_CFC_IPT_BIT_TX_STG_2_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_2_CFC_IPT_BIT_TX_STG_2_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_TX_STG_2_CFC_IPT_BIT_TX_STG_2_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_TX_STG_2_CFC_IPT_BIT_TX_STG_2_CFC_IPT_VA_OFF    0

/*-------------------------------------------.
 | Register 0x00002800 TX_STG_3A_FIFO_CLRB_0 |
 +-------------------------------------------+
 | bit  31:0 R/W  TX_STG_3A_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_0_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_0_BIT_TX_STG_3A_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_0_BIT_TX_STG_3A_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00002804 TX_STG_3A_FIFO_CLRB_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  TX_STG_3A_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_1_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_1_BIT_TX_STG_3A_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_1_BIT_TX_STG_3A_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00002808 TX_STG_3A_FIFO_CLRB_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  TX_STG_3A_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_2_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_2_BIT_TX_STG_3A_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_3A_FIFO_CLRB_2_BIT_TX_STG_3A_FIFO_CLRB_OFF 0

/*--------------------------------------------------.
 | Register 0x00002820 TX_STG_3A_CFC_CFG            |
 +--------------------------------------------------+
 | bit  28    R/W  TX_STG_3A_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  TX_STG_3A_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  TX_STG_3A_CFC_END_ADDR_CHG_ID    |
 +-------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_CFG_UNUSED_MASK                          0xee80ffff
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_BIT_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_BIT_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_BIT_TX_STG_3A_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_BIT_TX_STG_3A_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_BIT_TX_STG_3A_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_BIT_TX_STG_3A_CFC_END_ADDR_CHG_ID_OFF    16

/*----------------------------------------------.
 | Register 0x00002824 TX_STG_3A_CFC_CFG_STAT   |
 +----------------------------------------------+
 | bit  0 R  TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS |
 +---------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_CFG_STAT_UNUSED_MASK                            0xfffffffe
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_STAT_BIT_TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_STAT_BIT_TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*-------------------------------------------.
 | Register 0x00002828 TX_STG_3A_CFC_CFG_VAL |
 +-------------------------------------------+
 | bit  7:0 R/W  TX_STG_3A_CFC_END_ADDR      |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_CFG_VAL_UNUSED_MASK                0xffffff00
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_VAL_BIT_TX_STG_3A_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_TX_STG_3A_CFC_CFG_VAL_BIT_TX_STG_3A_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT */
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT_INDEX_N_MIN                 0
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT_INDEX_N_MAX                 95
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT_INDEX_N_SIZE                96
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT_INDEX_N_OFFSET              0x04

/*------------------------------------------------------.
 | Register (0x00002840 + (N) * 0x04) TX_STG_3A_CFC_IPT |
 +------------------------------------------------------+
 | bit  24    R/W  TX_STG_3A_CFC_IPT_VALID              |
 | bit  22:16 R/W  TX_STG_3A_CFC_IPT_ID                 |
 | bit  6:0   R/W  TX_STG_3A_CFC_IPT_VA                 |
 +-----------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3A_CFC_IPT_UNUSED_MASK                 0xfe80ff80
#define ODU_RCP_REG_TX_STG_3A_CFC_IPT_BIT_TX_STG_3A_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_TX_STG_3A_CFC_IPT_BIT_TX_STG_3A_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_TX_STG_3A_CFC_IPT_BIT_TX_STG_3A_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_3A_CFC_IPT_BIT_TX_STG_3A_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_TX_STG_3A_CFC_IPT_BIT_TX_STG_3A_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_TX_STG_3A_CFC_IPT_BIT_TX_STG_3A_CFC_IPT_VA_OFF    0

/*-------------------------------------------.
 | Register 0x00002a00 TX_STG_3B_FIFO_CLRB_0 |
 +-------------------------------------------+
 | bit  31:0 R/W  TX_STG_3B_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_0_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_0_BIT_TX_STG_3B_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_0_BIT_TX_STG_3B_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00002a04 TX_STG_3B_FIFO_CLRB_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  TX_STG_3B_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_1_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_1_BIT_TX_STG_3B_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_1_BIT_TX_STG_3B_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00002a08 TX_STG_3B_FIFO_CLRB_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  TX_STG_3B_FIFO_CLRB        |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_2_UNUSED_MASK             0x00000000
#define ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_2_BIT_TX_STG_3B_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_3B_FIFO_CLRB_2_BIT_TX_STG_3B_FIFO_CLRB_OFF 0

/*--------------------------------------------------.
 | Register 0x00002a20 TX_STG_3B_CFC_CFG            |
 +--------------------------------------------------+
 | bit  28    R/W  TX_STG_3B_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  TX_STG_3B_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  TX_STG_3B_CFC_END_ADDR_CHG_ID    |
 +-------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_CFG_UNUSED_MASK                          0xee80ffff
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_BIT_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_BIT_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_BIT_TX_STG_3B_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_BIT_TX_STG_3B_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_BIT_TX_STG_3B_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_BIT_TX_STG_3B_CFC_END_ADDR_CHG_ID_OFF    16

/*----------------------------------------------.
 | Register 0x00002a24 TX_STG_3B_CFC_CFG_STAT   |
 +----------------------------------------------+
 | bit  0 R  TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS |
 +---------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_CFG_STAT_UNUSED_MASK                            0xfffffffe
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_STAT_BIT_TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_STAT_BIT_TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*-------------------------------------------.
 | Register 0x00002a28 TX_STG_3B_CFC_CFG_VAL |
 +-------------------------------------------+
 | bit  7:0 R/W  TX_STG_3B_CFC_END_ADDR      |
 +------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_CFG_VAL_UNUSED_MASK                0xffffff00
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_VAL_BIT_TX_STG_3B_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_TX_STG_3B_CFC_CFG_VAL_BIT_TX_STG_3B_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT */
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT_INDEX_N_MIN                 0
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT_INDEX_N_MAX                 95
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT_INDEX_N_SIZE                96
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT_INDEX_N_OFFSET              0x04

/*------------------------------------------------------.
 | Register (0x00002a40 + (N) * 0x04) TX_STG_3B_CFC_IPT |
 +------------------------------------------------------+
 | bit  24    R/W  TX_STG_3B_CFC_IPT_VALID              |
 | bit  22:16 R/W  TX_STG_3B_CFC_IPT_ID                 |
 | bit  6:0   R/W  TX_STG_3B_CFC_IPT_VA                 |
 +-----------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_3B_CFC_IPT_UNUSED_MASK                 0xfe80ff80
#define ODU_RCP_REG_TX_STG_3B_CFC_IPT_BIT_TX_STG_3B_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_TX_STG_3B_CFC_IPT_BIT_TX_STG_3B_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_TX_STG_3B_CFC_IPT_BIT_TX_STG_3B_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_3B_CFC_IPT_BIT_TX_STG_3B_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_TX_STG_3B_CFC_IPT_BIT_TX_STG_3B_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_TX_STG_3B_CFC_IPT_BIT_TX_STG_3B_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00002c00 TX_STG_4_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  31:0 R/W  TX_STG_4_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_FIFO_CLRB_0_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_TX_STG_4_FIFO_CLRB_0_BIT_TX_STG_4_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_4_FIFO_CLRB_0_BIT_TX_STG_4_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00002c04 TX_STG_4_FIFO_CLRB_1 |
 +------------------------------------------+
 | bit  31:0 R/W  TX_STG_4_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_FIFO_CLRB_1_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_TX_STG_4_FIFO_CLRB_1_BIT_TX_STG_4_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_4_FIFO_CLRB_1_BIT_TX_STG_4_FIFO_CLRB_OFF 0

/*------------------------------------------.
 | Register 0x00002c08 TX_STG_4_FIFO_CLRB_2 |
 +------------------------------------------+
 | bit  31:0 R/W  TX_STG_4_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_FIFO_CLRB_2_UNUSED_MASK            0x00000000
#define ODU_RCP_REG_TX_STG_4_FIFO_CLRB_2_BIT_TX_STG_4_FIFO_CLRB_MSK 0xffffffff
#define ODU_RCP_REG_TX_STG_4_FIFO_CLRB_2_BIT_TX_STG_4_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00002c20 TX_STG_4_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  TX_STG_4_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  TX_STG_4_CFC_END_ADDR_CHG_REQ   |
 | bit  22:16 R/W  TX_STG_4_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_CFG_UNUSED_MASK                         0xee80ffff
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_BIT_TX_STG_4_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_BIT_TX_STG_4_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_BIT_TX_STG_4_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_BIT_TX_STG_4_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_BIT_TX_STG_4_CFC_END_ADDR_CHG_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_BIT_TX_STG_4_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00002c24 TX_STG_4_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  TX_STG_4_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_STAT_BIT_TX_STG_4_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_STAT_BIT_TX_STG_4_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00002c28 TX_STG_4_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  7:0 R/W  TX_STG_4_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_CFG_VAL_UNUSED_MASK               0xffffff00
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_VAL_BIT_TX_STG_4_CFC_END_ADDR_MSK 0x000000ff
#define ODU_RCP_REG_TX_STG_4_CFC_CFG_VAL_BIT_TX_STG_4_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT */
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT_INDEX_N_MAX                95
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT_INDEX_N_SIZE               96
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00002c40 + (N) * 0x04) TX_STG_4_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  TX_STG_4_CFC_IPT_VALID              |
 | bit  22:16 R/W  TX_STG_4_CFC_IPT_ID                 |
 | bit  6:0   R/W  TX_STG_4_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_4_CFC_IPT_UNUSED_MASK                0xfe80ff80
#define ODU_RCP_REG_TX_STG_4_CFC_IPT_BIT_TX_STG_4_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_TX_STG_4_CFC_IPT_BIT_TX_STG_4_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_TX_STG_4_CFC_IPT_BIT_TX_STG_4_CFC_IPT_ID_MSK    0x007f0000
#define ODU_RCP_REG_TX_STG_4_CFC_IPT_BIT_TX_STG_4_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_TX_STG_4_CFC_IPT_BIT_TX_STG_4_CFC_IPT_VA_MSK    0x0000007f
#define ODU_RCP_REG_TX_STG_4_CFC_IPT_BIT_TX_STG_4_CFC_IPT_VA_OFF    0

/*------------------------------------------.
 | Register 0x00002e00 TX_STG_N_FIFO_CLRB_0 |
 +------------------------------------------+
 | bit  11:0 R/W  TX_STG_N_FIFO_CLRB        |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_N_FIFO_CLRB_0_UNUSED_MASK            0xfffff000
#define ODU_RCP_REG_TX_STG_N_FIFO_CLRB_0_BIT_TX_STG_N_FIFO_CLRB_MSK 0x00000fff
#define ODU_RCP_REG_TX_STG_N_FIFO_CLRB_0_BIT_TX_STG_N_FIFO_CLRB_OFF 0

/*-------------------------------------------------.
 | Register 0x00002e20 TX_STG_N_CFC_CFG            |
 +-------------------------------------------------+
 | bit  28    R/W  TX_STG_N_CFC_END_ADDR_CHG_ABORT |
 | bit  24    R/W  TX_STG_N_CFC_END_ADDR_CHG_REQ   |
 | bit  19:16 R/W  TX_STG_N_CFC_END_ADDR_CHG_ID    |
 +------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_CFG_UNUSED_MASK                         0xeef0ffff
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_BIT_TX_STG_N_CFC_END_ADDR_CHG_ABORT_MSK 0x10000000
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_BIT_TX_STG_N_CFC_END_ADDR_CHG_ABORT_OFF 28
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_BIT_TX_STG_N_CFC_END_ADDR_CHG_REQ_MSK   0x01000000
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_BIT_TX_STG_N_CFC_END_ADDR_CHG_REQ_OFF   24
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_BIT_TX_STG_N_CFC_END_ADDR_CHG_ID_MSK    0x000f0000
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_BIT_TX_STG_N_CFC_END_ADDR_CHG_ID_OFF    16

/*---------------------------------------------.
 | Register 0x00002e24 TX_STG_N_CFC_CFG_STAT   |
 +---------------------------------------------+
 | bit  0 R  TX_STG_N_CFC_END_ADDR_CHG_SUCCESS |
 +--------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_CFG_STAT_UNUSED_MASK                           0xfffffffe
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_STAT_BIT_TX_STG_N_CFC_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_STAT_BIT_TX_STG_N_CFC_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------------------.
 | Register 0x00002e28 TX_STG_N_CFC_CFG_VAL |
 +------------------------------------------+
 | bit  4:0 R/W  TX_STG_N_CFC_END_ADDR      |
 +-----------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_CFG_VAL_UNUSED_MASK               0xffffffe0
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_VAL_BIT_TX_STG_N_CFC_END_ADDR_MSK 0x0000001f
#define ODU_RCP_REG_TX_STG_N_CFC_CFG_VAL_BIT_TX_STG_N_CFC_END_ADDR_OFF 0

/* index definitions for PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT */
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT_INDEX_N_MAX                11
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT_INDEX_N_SIZE               12
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------.
 | Register (0x00002e40 + (N) * 0x04) TX_STG_N_CFC_IPT |
 +-----------------------------------------------------+
 | bit  24    R/W  TX_STG_N_CFC_IPT_VALID              |
 | bit  19:16 R/W  TX_STG_N_CFC_IPT_ID                 |
 | bit  3:0   R/W  TX_STG_N_CFC_IPT_VA                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_TX_STG_N_CFC_IPT_UNUSED_MASK                0xfef0fff0
#define ODU_RCP_REG_TX_STG_N_CFC_IPT_BIT_TX_STG_N_CFC_IPT_VALID_MSK 0x01000000
#define ODU_RCP_REG_TX_STG_N_CFC_IPT_BIT_TX_STG_N_CFC_IPT_VALID_OFF 24
#define ODU_RCP_REG_TX_STG_N_CFC_IPT_BIT_TX_STG_N_CFC_IPT_ID_MSK    0x000f0000
#define ODU_RCP_REG_TX_STG_N_CFC_IPT_BIT_TX_STG_N_CFC_IPT_ID_OFF    16
#define ODU_RCP_REG_TX_STG_N_CFC_IPT_BIT_TX_STG_N_CFC_IPT_VA_MSK    0x0000000f
#define ODU_RCP_REG_TX_STG_N_CFC_IPT_BIT_TX_STG_N_CFC_IPT_VA_OFF    0

/*----------------------------------------.
 | Register 0x00003000 REG_SCH_CAL_ENABLE |
 +----------------------------------------+
 | bit  5:0 R/W  REG_SCH_CAL_EN           |
 +---------------------------------------*/
#define PMC_ODU_RCP_REG_REG_SCH_CAL_ENABLE_UNUSED_MASK        0xffffffc0
#define ODU_RCP_REG_REG_SCH_CAL_ENABLE_BIT_REG_SCH_CAL_EN_MSK 0x0000003f
#define ODU_RCP_REG_REG_SCH_CAL_ENABLE_BIT_REG_SCH_CAL_EN_OFF 0

/*-------------------------------------.
 | Register 0x00003004 REG_SCH_CAL_SEL |
 +-------------------------------------+
 | bit  5:0 R/W  REG_SCH_CAL_SEL       |
 +------------------------------------*/
#define PMC_ODU_RCP_REG_REG_SCH_CAL_SEL_UNUSED_MASK         0xffffffc0
#define ODU_RCP_REG_REG_SCH_CAL_SEL_BIT_REG_SCH_CAL_SEL_MSK 0x0000003f
#define ODU_RCP_REG_REG_SCH_CAL_SEL_BIT_REG_SCH_CAL_SEL_OFF 0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG_INDEX_N_MIN           0
#define PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG_INDEX_N_MAX           11
#define PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG_INDEX_N_SIZE          12
#define PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG_INDEX_N_OFFSET        0x04

/*------------------------------------------------------.
 | Register (0x00003080 + (N) * 0x04) SCH_STG_1_ACT_CFG |
 +------------------------------------------------------+
 | bit  3:0 R/W  SCH_STG_1_ACT_CFG                      |
 +-----------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_1_ACT_CFG_UNUSED_MASK           0xfffffff0
#define ODU_RCP_REG_SCH_STG_1_ACT_CFG_BIT_SCH_STG_1_ACT_CFG_MSK 0x0000000f
#define ODU_RCP_REG_SCH_STG_1_ACT_CFG_BIT_SCH_STG_1_ACT_CFG_OFF 0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG_INDEX_N_MIN            0
#define PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG_INDEX_N_MAX            11
#define PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG_INDEX_N_SIZE           12
#define PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG_INDEX_N_OFFSET         0x04

/*-------------------------------------------------------.
 | Register (0x000030c0 + (N) * 0x04) SCH_STG_1_STBY_CFG |
 +-------------------------------------------------------+
 | bit  3:0 R/W  SCH_STG_1_STBY_CFG                      |
 +------------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_1_STBY_CFG_UNUSED_MASK            0xfffffff0
#define ODU_RCP_REG_SCH_STG_1_STBY_CFG_BIT_SCH_STG_1_STBY_CFG_MSK 0x0000000f
#define ODU_RCP_REG_SCH_STG_1_STBY_CFG_BIT_SCH_STG_1_STBY_CFG_OFF 0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG_INDEX_N_MIN              0
#define PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG_INDEX_N_MAX              48
#define PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG_INDEX_N_SIZE             49
#define PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG_INDEX_N_OFFSET           4

/*---------------------------------------------------.
 | Register (0x00003100 + (N) * 4) SCH_STG_2_ACT_CFG |
 +---------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_2_ACT_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_2_ACT_CH_N                 |
 +--------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_2_ACT_CFG_UNUSED_MASK              0xffff8080
#define ODU_RCP_REG_SCH_STG_2_ACT_CFG_BIT_SCH_STG_2_ACT_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_2_ACT_CFG_BIT_SCH_STG_2_ACT_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_2_ACT_CFG_BIT_SCH_STG_2_ACT_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_2_ACT_CFG_BIT_SCH_STG_2_ACT_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG_INDEX_N_MIN               0
#define PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG_INDEX_N_MAX               48
#define PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG_INDEX_N_SIZE              49
#define PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG_INDEX_N_OFFSET            4

/*----------------------------------------------------.
 | Register (0x00003200 + (N) * 4) SCH_STG_2_STBY_CFG |
 +----------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_2_STBY_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_2_STBY_CH_N                 |
 +---------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_2_STBY_CFG_UNUSED_MASK               0xffff8080
#define ODU_RCP_REG_SCH_STG_2_STBY_CFG_BIT_SCH_STG_2_STBY_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_2_STBY_CFG_BIT_SCH_STG_2_STBY_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_2_STBY_CFG_BIT_SCH_STG_2_STBY_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_2_STBY_CFG_BIT_SCH_STG_2_STBY_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG_INDEX_N_MIN               0
#define PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG_INDEX_N_MAX               48
#define PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG_INDEX_N_SIZE              49
#define PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG_INDEX_N_OFFSET            4

/*----------------------------------------------------.
 | Register (0x00003300 + (N) * 4) SCH_STG_3A_ACT_CFG |
 +----------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_3A_ACT_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_3A_ACT_CH_N                 |
 +---------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_3A_ACT_CFG_UNUSED_MASK               0xffff8080
#define ODU_RCP_REG_SCH_STG_3A_ACT_CFG_BIT_SCH_STG_3A_ACT_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_3A_ACT_CFG_BIT_SCH_STG_3A_ACT_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_3A_ACT_CFG_BIT_SCH_STG_3A_ACT_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_3A_ACT_CFG_BIT_SCH_STG_3A_ACT_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG_INDEX_N_MAX                48
#define PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG_INDEX_N_SIZE               49
#define PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG_INDEX_N_OFFSET             4

/*-----------------------------------------------------.
 | Register (0x00003400 + (N) * 4) SCH_STG_3A_STBY_CFG |
 +-----------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_3A_STBY_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_3A_STBY_CH_N                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_3A_STBY_CFG_UNUSED_MASK                0xffff8080
#define ODU_RCP_REG_SCH_STG_3A_STBY_CFG_BIT_SCH_STG_3A_STBY_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_3A_STBY_CFG_BIT_SCH_STG_3A_STBY_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_3A_STBY_CFG_BIT_SCH_STG_3A_STBY_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_3A_STBY_CFG_BIT_SCH_STG_3A_STBY_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG_INDEX_N_MIN               0
#define PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG_INDEX_N_MAX               48
#define PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG_INDEX_N_SIZE              49
#define PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG_INDEX_N_OFFSET            4

/*----------------------------------------------------.
 | Register (0x00003500 + (N) * 4) SCH_STG_3B_ACT_CFG |
 +----------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_3B_ACT_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_3B_ACT_CH_N                 |
 +---------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_3B_ACT_CFG_UNUSED_MASK               0xffff8080
#define ODU_RCP_REG_SCH_STG_3B_ACT_CFG_BIT_SCH_STG_3B_ACT_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_3B_ACT_CFG_BIT_SCH_STG_3B_ACT_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_3B_ACT_CFG_BIT_SCH_STG_3B_ACT_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_3B_ACT_CFG_BIT_SCH_STG_3B_ACT_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG_INDEX_N_MIN                0
#define PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG_INDEX_N_MAX                48
#define PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG_INDEX_N_SIZE               49
#define PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG_INDEX_N_OFFSET             4

/*-----------------------------------------------------.
 | Register (0x00003600 + (N) * 4) SCH_STG_3B_STBY_CFG |
 +-----------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_3B_STBY_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_3B_STBY_CH_N                 |
 +----------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_3B_STBY_CFG_UNUSED_MASK                0xffff8080
#define ODU_RCP_REG_SCH_STG_3B_STBY_CFG_BIT_SCH_STG_3B_STBY_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_3B_STBY_CFG_BIT_SCH_STG_3B_STBY_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_3B_STBY_CFG_BIT_SCH_STG_3B_STBY_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_3B_STBY_CFG_BIT_SCH_STG_3B_STBY_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG_INDEX_N_MIN              0
#define PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG_INDEX_N_MAX              48
#define PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG_INDEX_N_SIZE             49
#define PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG_INDEX_N_OFFSET           4

/*---------------------------------------------------.
 | Register (0x00003700 + (N) * 4) SCH_STG_4_ACT_CFG |
 +---------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_4_ACT_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_4_ACT_CH_N                 |
 +--------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_4_ACT_CFG_UNUSED_MASK              0xffff8080
#define ODU_RCP_REG_SCH_STG_4_ACT_CFG_BIT_SCH_STG_4_ACT_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_4_ACT_CFG_BIT_SCH_STG_4_ACT_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_4_ACT_CFG_BIT_SCH_STG_4_ACT_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_4_ACT_CFG_BIT_SCH_STG_4_ACT_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG_INDEX_N_MIN               0
#define PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG_INDEX_N_MAX               48
#define PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG_INDEX_N_SIZE              49
#define PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG_INDEX_N_OFFSET            4

/*----------------------------------------------------.
 | Register (0x00003800 + (N) * 4) SCH_STG_4_STBY_CFG |
 +----------------------------------------------------+
 | bit  14:8 R/W  SCH_STG_4_STBY_CH_NP1               |
 | bit  6:0  R/W  SCH_STG_4_STBY_CH_N                 |
 +---------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_4_STBY_CFG_UNUSED_MASK               0xffff8080
#define ODU_RCP_REG_SCH_STG_4_STBY_CFG_BIT_SCH_STG_4_STBY_CH_NP1_MSK 0x00007f00
#define ODU_RCP_REG_SCH_STG_4_STBY_CFG_BIT_SCH_STG_4_STBY_CH_NP1_OFF 8
#define ODU_RCP_REG_SCH_STG_4_STBY_CFG_BIT_SCH_STG_4_STBY_CH_N_MSK   0x0000007f
#define ODU_RCP_REG_SCH_STG_4_STBY_CFG_BIT_SCH_STG_4_STBY_CH_N_OFF   0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG_INDEX_N_MIN           0
#define PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG_INDEX_N_MAX           11
#define PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG_INDEX_N_SIZE          12
#define PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG_INDEX_N_OFFSET        0x04

/*------------------------------------------------------.
 | Register (0x00003900 + (N) * 0x04) SCH_STG_N_ACT_CFG |
 +------------------------------------------------------+
 | bit  3:0 R/W  SCH_STG_N_ACT_CFG                      |
 +-----------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_N_ACT_CFG_UNUSED_MASK           0xfffffff0
#define ODU_RCP_REG_SCH_STG_N_ACT_CFG_BIT_SCH_STG_N_ACT_CFG_MSK 0x0000000f
#define ODU_RCP_REG_SCH_STG_N_ACT_CFG_BIT_SCH_STG_N_ACT_CFG_OFF 0

/* index definitions for PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG */
#define PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG_INDEX_N_MIN            0
#define PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG_INDEX_N_MAX            11
#define PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG_INDEX_N_SIZE           12
#define PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG_INDEX_N_OFFSET         0x04

/*-------------------------------------------------------.
 | Register (0x00003940 + (N) * 0x04) SCH_STG_N_STBY_CFG |
 +-------------------------------------------------------+
 | bit  3:0 R/W  SCH_STG_N_STBY_CFG                      |
 +------------------------------------------------------*/
#define PMC_ODU_RCP_REG_SCH_STG_N_STBY_CFG_UNUSED_MASK            0xfffffff0
#define ODU_RCP_REG_SCH_STG_N_STBY_CFG_BIT_SCH_STG_N_STBY_CFG_MSK 0x0000000f
#define ODU_RCP_REG_SCH_STG_N_STBY_CFG_BIT_SCH_STG_N_STBY_CFG_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RI_RCP_REGS_H */
