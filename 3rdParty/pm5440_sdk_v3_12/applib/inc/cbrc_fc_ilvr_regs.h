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
 *     cbrc_fc_ilvr block
 *****************************************************************************/
#ifndef _CBRC_FC_ILVR_REGS_H
#define _CBRC_FC_ILVR_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0 0x00031600
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1 0x00031604
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2 0x00031608
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3 0x0003160c
#define PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG( N ) (0x00031610 + (N) * 0x4)
#define PMC_CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG     0x00031640
#define PMC_CBRC_FC_ILVR_REG_GENERAL_PURPOSE_CTRL_REG 0x00031644
#define PMC_CBRC_FC_ILVR_REG_GENERAL_PURPOSE_STAT_REG 0x00031648

/*----------------------------------------------.
 | Register 0x00031600 ILVR_TSB_INT_ENABLE_REG0 |
 +----------------------------------------------+
 | bit  31:20 R/W  FC1200_PTE_IRQ_EN0           |
 | bit  19:8  R/W  FC800_PMG_IRQ_EN0            |
 | bit  6:4   R/W  CBRFSGM_IRQ_EN0              |
 | bit  2     R/W  PKTINT_IRQ_EN0               |
 | bit  1     R/W  MPMA_IRQ_EN0                 |
 | bit  0     R/W  CBRINT_IRQ_EN0               |
 +---------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_UNUSED_MASK            0x00000088
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_FC1200_PTE_IRQ_EN0_MSK 0xfff00000
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_FC1200_PTE_IRQ_EN0_OFF 20
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_FC800_PMG_IRQ_EN0_MSK  0x000fff00
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_FC800_PMG_IRQ_EN0_OFF  8
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_MSK    0x00000070
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_CBRFSGM_IRQ_EN0_OFF    4
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_PKTINT_IRQ_EN0_MSK     0x00000004
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_PKTINT_IRQ_EN0_OFF     2
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_MPMA_IRQ_EN0_MSK       0x00000002
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_MPMA_IRQ_EN0_OFF       1
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_CBRINT_IRQ_EN0_MSK     0x00000001
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG0_BIT_CBRINT_IRQ_EN0_OFF     0

/*----------------------------------------------.
 | Register 0x00031604 ILVR_TSB_INT_ENABLE_REG1 |
 +----------------------------------------------+
 | bit  31:20 R/W  FC1200_PTE_IRQ_EN1           |
 | bit  19:8  R/W  FC800_PMG_IRQ_EN1            |
 | bit  6:4   R/W  CBRFSGM_IRQ_EN1              |
 | bit  2     R/W  PKTINT_IRQ_EN1               |
 | bit  1     R/W  MPMA_IRQ_EN1                 |
 | bit  0     R/W  CBRINT_IRQ_EN1               |
 +---------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_UNUSED_MASK            0x00000088
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_FC1200_PTE_IRQ_EN1_MSK 0xfff00000
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_FC1200_PTE_IRQ_EN1_OFF 20
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_FC800_PMG_IRQ_EN1_MSK  0x000fff00
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_FC800_PMG_IRQ_EN1_OFF  8
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_MSK    0x00000070
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_CBRFSGM_IRQ_EN1_OFF    4
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_PKTINT_IRQ_EN1_MSK     0x00000004
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_PKTINT_IRQ_EN1_OFF     2
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_MPMA_IRQ_EN1_MSK       0x00000002
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_MPMA_IRQ_EN1_OFF       1
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_CBRINT_IRQ_EN1_MSK     0x00000001
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG1_BIT_CBRINT_IRQ_EN1_OFF     0

/*----------------------------------------------.
 | Register 0x00031608 ILVR_TSB_INT_ENABLE_REG2 |
 +----------------------------------------------+
 | bit  31:20 R/W  FC1200_PTE_IRQ_EN2           |
 | bit  19:8  R/W  FC800_PMG_IRQ_EN2            |
 | bit  6:4   R/W  CBRFSGM_IRQ_EN2              |
 | bit  2     R/W  PKTINT_IRQ_EN2               |
 | bit  1     R/W  MPMA_IRQ_EN2                 |
 | bit  0     R/W  CBRINT_IRQ_EN2               |
 +---------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_UNUSED_MASK            0x00000088
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_FC1200_PTE_IRQ_EN2_MSK 0xfff00000
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_FC1200_PTE_IRQ_EN2_OFF 20
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_FC800_PMG_IRQ_EN2_MSK  0x000fff00
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_FC800_PMG_IRQ_EN2_OFF  8
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_MSK    0x00000070
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_CBRFSGM_IRQ_EN2_OFF    4
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_PKTINT_IRQ_EN2_MSK     0x00000004
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_PKTINT_IRQ_EN2_OFF     2
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_MPMA_IRQ_EN2_MSK       0x00000002
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_MPMA_IRQ_EN2_OFF       1
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_CBRINT_IRQ_EN2_MSK     0x00000001
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG2_BIT_CBRINT_IRQ_EN2_OFF     0

/*----------------------------------------------.
 | Register 0x0003160c ILVR_TSB_INT_ENABLE_REG3 |
 +----------------------------------------------+
 | bit  31:20 R/W  FC1200_PTE_IRQ_EN3           |
 | bit  19:8  R/W  FC800_PMG_IRQ_EN3            |
 | bit  6:4   R/W  CBRFSGM_IRQ_EN3              |
 | bit  2     R/W  PKTINT_IRQ_EN3               |
 | bit  1     R/W  MPMA_IRQ_EN3                 |
 | bit  0     R/W  CBRINT_IRQ_EN3               |
 +---------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_UNUSED_MASK            0x00000088
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_FC1200_PTE_IRQ_EN3_MSK 0xfff00000
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_FC1200_PTE_IRQ_EN3_OFF 20
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_FC800_PMG_IRQ_EN3_MSK  0x000fff00
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_FC800_PMG_IRQ_EN3_OFF  8
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_MSK    0x00000070
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_CBRFSGM_IRQ_EN3_OFF    4
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_PKTINT_IRQ_EN3_MSK     0x00000004
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_PKTINT_IRQ_EN3_OFF     2
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_MPMA_IRQ_EN3_MSK       0x00000002
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_MPMA_IRQ_EN3_OFF       1
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_CBRINT_IRQ_EN3_MSK     0x00000001
#define CBRC_FC_ILVR_REG_ILVR_TSB_INT_ENABLE_REG3_BIT_CBRINT_IRQ_EN3_OFF     0

/* index definitions for PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG */
#define PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_INDEX_N_MIN        0
#define PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_INDEX_N_MAX        11
#define PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_INDEX_N_SIZE       12
#define PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_INDEX_N_OFFSET     0x4

/*-------------------------------------------------------.
 | Register (0x00031610 + (N) * 0x4) ILVR_CHN_CONFIG_REG |
 +-------------------------------------------------------+
 | bit  1 R/W  FC800PMG_CH_EN                            |
 | bit  0 R/W  FCPTE_CH_EN                               |
 +------------------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_UNUSED_MASK        0xfffffffc
#define CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_BIT_FC800PMG_CH_EN_MSK 0x00000002
#define CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_BIT_FC800PMG_CH_EN_OFF 1
#define CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_BIT_FCPTE_CH_EN_MSK    0x00000001
#define CBRC_FC_ILVR_REG_ILVR_CHN_CONFIG_REG_BIT_FCPTE_CH_EN_OFF    0

/*------------------------------------------.
 | Register 0x00031640 ILVR_GLOBAL_CTRL_REG |
 +------------------------------------------+
 | bit  4 R/W  PKTINT_RAM_SD_REG            |
 | bit  2 R/W  MPMA_RAM_SD_REG              |
 | bit  1 R/W  CBR_CH_EN                    |
 | bit  0 R/W  ILVR_SYNC_RST                |
 +-----------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_UNUSED_MASK           0xffffffe0
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_PKTINT_RAM_SD_REG_MSK 0x00000010
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_PKTINT_RAM_SD_REG_OFF 4
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_MPMA_RAM_SD_REG_MSK   0x00000004
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_MPMA_RAM_SD_REG_OFF   2
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_MSK         0x00000002
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_OFF         1
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_ILVR_SYNC_RST_MSK     0x00000001
#define CBRC_FC_ILVR_REG_ILVR_GLOBAL_CTRL_REG_BIT_ILVR_SYNC_RST_OFF     0

/*----------------------------------------------.
 | Register 0x00031644 GENERAL_PURPOSE_CTRL_REG |
 +----------------------------------------------+
 | bit  7:0 R/W  GPO_REG                        |
 +---------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_GENERAL_PURPOSE_CTRL_REG_UNUSED_MASK 0xffffff00
#define CBRC_FC_ILVR_REG_GENERAL_PURPOSE_CTRL_REG_BIT_GPO_REG_MSK 0x000000ff
#define CBRC_FC_ILVR_REG_GENERAL_PURPOSE_CTRL_REG_BIT_GPO_REG_OFF 0

/*----------------------------------------------.
 | Register 0x00031648 GENERAL_PURPOSE_STAT_REG |
 +----------------------------------------------+
 +---------------------------------------------*/
#define PMC_CBRC_FC_ILVR_REG_GENERAL_PURPOSE_STAT_REG_UNUSED_MASK 0xffffff00

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_FC_ILVR_REGS_H */
