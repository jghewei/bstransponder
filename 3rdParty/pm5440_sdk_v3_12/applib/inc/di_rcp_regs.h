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
 *     di_rcp block
 *****************************************************************************/
#ifndef _DI_RCP_REGS_H
#define _DI_RCP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_DI_ODU_RCP_REG_REG_SW_RESET        0x00008000
#define PMC_DI_ODU_RCP_REG_REG_LOWPWR          0x00008004
#define PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT    0x0000800c
#define PMC_DI_ODU_RCP_REG_IRQ_SUMMARY         0x00008010
#define PMC_DI_ODU_RCP_REG_REG_GOJF            0x00008014
#define PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN       0x00008020
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1     0x00008024
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2     0x00008028
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1     0x0000802c
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2     0x00008030
#define PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE  0x00008034
#define PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG    0x00008038
#define PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM 0x00008040
#define PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1  0x00008044
#define PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2  0x00008048
#define PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3  0x0000804c
#define PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I       0x00008060
#define PMC_DI_ODU_RCP_REG_GEN_IRQ_E           0x00008080
#define PMC_DI_ODU_RCP_REG_SDH_PMON_COPY( N )  (0x00008100 + (N) * 4)
#define PMC_DI_ODU_RCP_REG_OTN_PMON_COPY( N )  (0x00008200 + (N) * 4)
#define PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY( N ) (0x00009b00 + (N) * 4)

/*----------------------------------.
 | Register 0x00008000 REG_SW_RESET |
 +----------------------------------+
 | bit  1 R/W  REG_SOFT_SW_RESET    |
 | bit  0 R/W  REG_SW_RESET         |
 +---------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_SW_RESET_UNUSED_MASK           0xfffffffc
#define DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_MSK 0x00000002
#define DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SOFT_SW_RESET_OFF 1
#define DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_MSK      0x00000001
#define DI_ODU_RCP_REG_REG_SW_RESET_BIT_REG_SW_RESET_OFF      0

/*--------------------------------.
 | Register 0x00008004 REG_LOWPWR |
 +--------------------------------+
 | bit  0 R/W  REG_LOWPWR         |
 +-------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_LOWPWR_UNUSED_MASK    0xfffffffe
#define DI_ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_MSK 0x00000001
#define DI_ODU_RCP_REG_REG_LOWPWR_BIT_REG_LOWPWR_OFF 0

/*--------------------------------------.
 | Register 0x0000800c REG_RAM_PWR_MGMT |
 +--------------------------------------+
 | bit  0 R/W  REG_RAM_LOWPWR           |
 +-------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_UNUSED_MASK        0xfffffefe
#define DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_MSK 0x00000001
#define DI_ODU_RCP_REG_REG_RAM_PWR_MGMT_BIT_REG_RAM_LOWPWR_OFF 0

/*---------------------------------.
 | Register 0x00008010 IRQ_SUMMARY |
 +---------------------------------+
 | bit  2:0 R  INTERRUPT_SUMMARY   |
 +--------------------------------*/
#define PMC_DI_ODU_RCP_REG_IRQ_SUMMARY_UNUSED_MASK           0xfffffff8
#define DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK 0x00000007
#define DI_ODU_RCP_REG_IRQ_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF 0

/*----------------------------------------.
 | Register 0x00008014 REG_GOJF           |
 +----------------------------------------+
 | bit  21:16 R/W  REG_FIFO_MIN_SLOT_FREE |
 | bit  13:8  R/W  REG_FIFO_MIN_LVL       |
 | bit  4     R/W  REG_ETH_TYPE_IN_PDU    |
 | bit  2     R/W  REG_ETH_RX_10B_SWIZZLE |
 | bit  1     R/W  REG_ETH_TX_10B_SWIZZLE |
 | bit  0     R/W  REG_ETH_TX_8B_SWIZZLE  |
 +---------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_GOJF_UNUSED_MASK                0xffc0c0e8
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_MSK 0x003f0000
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_SLOT_FREE_OFF 16
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_MSK       0x00003f00
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_FIFO_MIN_LVL_OFF       8
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TYPE_IN_PDU_MSK    0x00000010
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TYPE_IN_PDU_OFF    4
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_MSK 0x00000004
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_RX_10B_SWIZZLE_OFF 2
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_MSK 0x00000002
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_10B_SWIZZLE_OFF 1
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_MSK  0x00000001
#define DI_ODU_RCP_REG_REG_GOJF_BIT_REG_ETH_TX_8B_SWIZZLE_OFF  0

/*-----------------------------------.
 | Register 0x00008020 REG_ETH_TX_EN |
 +-----------------------------------+
 | bit  28:1 R/W  REG_ETH_TX_EN      |
 +----------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_ETH_TX_EN_UNUSED_MASK       0xe0000001
#define DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_MSK 0x1ffffffe
#define DI_ODU_RCP_REG_REG_ETH_TX_EN_BIT_REG_ETH_TX_EN_OFF 1

/*-------------------------------------.
 | Register 0x00008024 REG_ETH_MAC_DA1 |
 +-------------------------------------+
 | bit  31:0 R/W  REG_ETH_MAC_DA       |
 +------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA1_UNUSED_MASK        0x00000000
#define DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_MSK 0xffffffff
#define DI_ODU_RCP_REG_REG_ETH_MAC_DA1_BIT_REG_ETH_MAC_DA_OFF 0

/*-------------------------------------.
 | Register 0x00008028 REG_ETH_MAC_DA2 |
 +-------------------------------------+
 | bit  15:0 R/W  REG_ETH_MAC_DA       |
 +------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_DA2_UNUSED_MASK        0xffff0000
#define DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_MSK 0x0000ffff
#define DI_ODU_RCP_REG_REG_ETH_MAC_DA2_BIT_REG_ETH_MAC_DA_OFF 0

/*-------------------------------------.
 | Register 0x0000802c REG_ETH_MAC_SA1 |
 +-------------------------------------+
 | bit  31:0 R/W  REG_ETH_MAC_SA       |
 +------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA1_UNUSED_MASK        0x00000000
#define DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_MSK 0xffffffff
#define DI_ODU_RCP_REG_REG_ETH_MAC_SA1_BIT_REG_ETH_MAC_SA_OFF 0

/*-------------------------------------.
 | Register 0x00008030 REG_ETH_MAC_SA2 |
 +-------------------------------------+
 | bit  15:0 R/W  REG_ETH_MAC_SA       |
 +------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_ETH_MAC_SA2_UNUSED_MASK        0xffff0000
#define DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_MSK 0x0000ffff
#define DI_ODU_RCP_REG_REG_ETH_MAC_SA2_BIT_REG_ETH_MAC_SA_OFF 0

/*----------------------------------------.
 | Register 0x00008034 REG_ETH_ETYPE_BASE |
 +----------------------------------------+
 | bit  15:0 R/W  REG_ETH_ETYPE_BASE      |
 +---------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_UNUSED_MASK            0xffff0000
#define DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_MSK 0x0000ffff
#define DI_ODU_RCP_REG_REG_ETH_ETYPE_BASE_BIT_REG_ETH_ETYPE_BASE_OFF 0

/*--------------------------------------.
 | Register 0x00008038 REG_PMON_TX_TRIG |
 +--------------------------------------+
 | bit  0 R/W  REG_PMON_TX_TRIG         |
 +-------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_PMON_TX_TRIG_UNUSED_MASK          0xfffffffe
#define DI_ODU_RCP_REG_REG_PMON_TX_TRIG_BIT_REG_PMON_TX_TRIG_MSK 0x00000001
#define DI_ODU_RCP_REG_REG_PMON_TX_TRIG_BIT_REG_PMON_TX_TRIG_OFF 0

/*-----------------------------------------.
 | Register 0x00008040 REG_STG_1_N_D_RCOHM |
 +-----------------------------------------+
 | bit  11:0 R/W  REG_STG_1_N_D_RCOHM      |
 +----------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_UNUSED_MASK             0xfffff000
#define DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_MSK 0x00000fff
#define DI_ODU_RCP_REG_REG_STG_1_N_D_RCOHM_BIT_REG_STG_1_N_D_RCOHM_OFF 0

/*----------------------------------------.
 | Register 0x00008044 REG_STG_2_D_RCOHM1 |
 +----------------------------------------+
 | bit  31:0 R/W  REG_STG_2_D_RCOHM       |
 +---------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_UNUSED_MASK           0x00000000
#define DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_MSK 0xffffffff
#define DI_ODU_RCP_REG_REG_STG_2_D_RCOHM1_BIT_REG_STG_2_D_RCOHM_OFF 0

/*----------------------------------------.
 | Register 0x00008048 REG_STG_2_D_RCOHM2 |
 +----------------------------------------+
 | bit  31:0 R/W  REG_STG_2_D_RCOHM       |
 +---------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_UNUSED_MASK           0x00000000
#define DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_MSK 0xffffffff
#define DI_ODU_RCP_REG_REG_STG_2_D_RCOHM2_BIT_REG_STG_2_D_RCOHM_OFF 0

/*----------------------------------------.
 | Register 0x0000804c REG_STG_2_D_RCOHM3 |
 +----------------------------------------+
 | bit  31:0 R/W  REG_STG_2_D_RCOHM       |
 +---------------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_UNUSED_MASK           0x00000000
#define DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_MSK 0xffffffff
#define DI_ODU_RCP_REG_REG_STG_2_D_RCOHM3_BIT_REG_STG_2_D_RCOHM_OFF 0

/*-----------------------------------.
 | Register 0x00008060 REG_GEN_IRQ_I |
 +-----------------------------------+
 | bit  12 R/W  DFEC_TX_DONE_I       |
 | bit  4  R/W  DEFECTS_TIP_I        |
 | bit  0  R/W  PMON_TX_DONE_I       |
 +----------------------------------*/
#define PMC_DI_ODU_RCP_REG_REG_GEN_IRQ_I_UNUSED_MASK        0xffffecee
#define DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_MSK 0x00001000
#define DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DFEC_TX_DONE_I_OFF 12
#define DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_MSK  0x00000010
#define DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_DEFECTS_TIP_I_OFF  4
#define DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_MSK 0x00000001
#define DI_ODU_RCP_REG_REG_GEN_IRQ_I_BIT_PMON_TX_DONE_I_OFF 0

/*-------------------------------.
 | Register 0x00008080 GEN_IRQ_E |
 +-------------------------------+
 | bit  12 R/W  DFEC_TX_DONE_E   |
 | bit  4  R/W  DEFECTS_TIP_E    |
 | bit  0  R/W  PMON_TX_DONE_E   |
 +------------------------------*/
#define PMC_DI_ODU_RCP_REG_GEN_IRQ_E_UNUSED_MASK        0xffffecee
#define DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DFEC_TX_DONE_E_MSK 0x00001000
#define DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DFEC_TX_DONE_E_OFF 12
#define DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DEFECTS_TIP_E_MSK  0x00000010
#define DI_ODU_RCP_REG_GEN_IRQ_E_BIT_DEFECTS_TIP_E_OFF  4
#define DI_ODU_RCP_REG_GEN_IRQ_E_BIT_PMON_TX_DONE_E_MSK 0x00000001
#define DI_ODU_RCP_REG_GEN_IRQ_E_BIT_PMON_TX_DONE_E_OFF 0

/* index definitions for PMC_DI_ODU_RCP_REG_SDH_PMON_COPY */
#define PMC_DI_ODU_RCP_REG_SDH_PMON_COPY_INDEX_N_MIN       0
#define PMC_DI_ODU_RCP_REG_SDH_PMON_COPY_INDEX_N_MAX       63
#define PMC_DI_ODU_RCP_REG_SDH_PMON_COPY_INDEX_N_SIZE      64
#define PMC_DI_ODU_RCP_REG_SDH_PMON_COPY_INDEX_N_OFFSET    4

/*-----------------------------------------------.
 | Register (0x00008100 + (N) * 4) SDH_PMON_COPY |
 +-----------------------------------------------+
 | bit  15:0 R/W  SDH_PMON_COPY                  |
 +----------------------------------------------*/
#define PMC_DI_ODU_RCP_REG_SDH_PMON_COPY_UNUSED_MASK       0xffff0000
#define DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_MSK 0x0000ffff
#define DI_ODU_RCP_REG_SDH_PMON_COPY_BIT_SDH_PMON_COPY_OFF 0

/* index definitions for PMC_DI_ODU_RCP_REG_OTN_PMON_COPY */
#define PMC_DI_ODU_RCP_REG_OTN_PMON_COPY_INDEX_N_MIN    0
#define PMC_DI_ODU_RCP_REG_OTN_PMON_COPY_INDEX_N_MAX    1599
#define PMC_DI_ODU_RCP_REG_OTN_PMON_COPY_INDEX_N_SIZE   1600
#define PMC_DI_ODU_RCP_REG_OTN_PMON_COPY_INDEX_N_OFFSET 4

/*-----------------------------------------------.
 | Register (0x00008200 + (N) * 4) OTN_PMON_COPY |
 +-----------------------------------------------+
 | bit  28:16 R/W  F_EBC                         |
 | bit  12:0  R/W  N_EBC                         |
 +----------------------------------------------*/
#define PMC_DI_ODU_RCP_REG_OTN_PMON_COPY_UNUSED_MASK    0xe000e000
#define DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_MSK      0x1fff0000
#define DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_F_EBC_OFF      16
#define DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_MSK      0x00001fff
#define DI_ODU_RCP_REG_OTN_PMON_COPY_BIT_N_EBC_OFF      0

/* index definitions for PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY */
#define PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY_INDEX_N_MIN    0
#define PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY_INDEX_N_MAX    47
#define PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY_INDEX_N_SIZE   48
#define PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY_INDEX_N_OFFSET 4

/*------------------------------------------------.
 | Register (0x00009b00 + (N) * 4) PRBS_PMON_COPY |
 +------------------------------------------------+
 | bit  15:0 R/W  TSE                             |
 +-----------------------------------------------*/
#define PMC_DI_ODU_RCP_REG_PRBS_PMON_COPY_UNUSED_MASK    0xffff0000
#define DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_MSK        0x0000ffff
#define DI_ODU_RCP_REG_PRBS_PMON_COPY_BIT_TSE_OFF        0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DI_RCP_REGS_H */
