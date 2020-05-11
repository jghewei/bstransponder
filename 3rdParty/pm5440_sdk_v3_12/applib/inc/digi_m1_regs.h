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
 *     digi_m1 block
 *****************************************************************************/
#ifndef _DIGI_M1_REGS_H
#define _DIGI_M1_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS   0x00016000
#define PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG   0x00016004
#define PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0 0x00016008
#define PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1 0x0001600c
#define PMC_DIGI_M1_REG_IRIG_IF_MUX           0x00016010
#define PMC_DIGI_M1_REG_CAL_349_CFG_0         0x00016014
#define PMC_DIGI_M1_REG_CAL_349_CFG_1         0x00016018
#define PMC_DIGI_M1_REG_CAL_349_CFG_2         0x0001601c
#define PMC_DIGI_M1_REG_CAL_349_REQ           0x00016020
#define PMC_DIGI_M1_REG_CAL_500_CFG_0         0x0001603c
#define PMC_DIGI_M1_REG_CAL_500_CFG_1         0x00016040
#define PMC_DIGI_M1_REG_CAL_500_CFG_2         0x00016044
#define PMC_DIGI_M1_REG_CAL_500_REQ           0x00016048
#define PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS    0x00016064
#define PMC_DIGI_M1_REG_OTU_MFPO_SELECT       0x00016068
#define PMC_DIGI_M1_REG_EFUSE_STAT( N )       (0x000160a4 + (N) * 4)

/*-----------------------------------------.
 | Register 0x00016000 SUBSYSTEM_SW_RESETS |
 +-----------------------------------------+
 | bit  7 R/W  FEGE_SW_RST                 |
 | bit  6 R/W  PGMRCLK_SW_RST              |
 | bit  5 R/W  TOC_CAL_349_SW_RST          |
 | bit  4 R/W  TOC_CAL_500_SW_RST          |
 | bit  3 R/W  MISC_SW_RST                 |
 +----------------------------------------*/
#define PMC_DIGI_M1_REG_SUBSYSTEM_SW_RESETS_UNUSED_MASK            0xffffff07
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_FEGE_SW_RST_MSK        0x00000080
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_FEGE_SW_RST_OFF        7
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_PGMRCLK_SW_RST_MSK     0x00000040
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_PGMRCLK_SW_RST_OFF     6
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_349_SW_RST_MSK 0x00000020
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_349_SW_RST_OFF 5
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_500_SW_RST_MSK 0x00000010
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_TOC_CAL_500_SW_RST_OFF 4
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_MISC_SW_RST_MSK        0x00000008
#define DIGI_M1_REG_SUBSYSTEM_SW_RESETS_BIT_MISC_SW_RST_OFF        3

/*-----------------------------------------.
 | Register 0x00016004 PLL_SUPPORT_MUX_CFG |
 +-----------------------------------------+
 | bit  8   R/W  LINE_TIMING_REF_SEL       |
 | bit  6:4 R/W  LINE_REF_CLK_SEL          |
 | bit  2:0 R/W  PTP_DCSU_REF_SEL          |
 +----------------------------------------*/
#define PMC_DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_UNUSED_MASK             0xfffffe88
#define DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_TIMING_REF_SEL_MSK 0x00000100
#define DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_TIMING_REF_SEL_OFF 8
#define DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_MSK    0x00000070
#define DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_LINE_REF_CLK_SEL_OFF    4
#define DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_MSK    0x00000007
#define DIGI_M1_REG_PLL_SUPPORT_MUX_CFG_BIT_PTP_DCSU_REF_SEL_OFF    0

/*-------------------------------------------.
 | Register 0x00016008 PLL_SUPPORT_PMM_CFG_0 |
 +-------------------------------------------+
 | bit  29:16 R/W  PMM_DSM_DENOM             |
 | bit  15:0  R/W  PMM_PKT_PERIOD            |
 +------------------------------------------*/
#define PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_UNUSED_MASK        0xc0000000
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_MSK  0x3fff0000
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_DSM_DENOM_OFF  16
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_MSK 0x0000ffff
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_0_BIT_PMM_PKT_PERIOD_OFF 0

/*-------------------------------------------.
 | Register 0x0001600c PLL_SUPPORT_PMM_CFG_1 |
 +-------------------------------------------+
 | bit  24:16 R/W  PMM_PKT_SIZE              |
 | bit  2:1   R/W  PMM_DSM_TYPE              |
 | bit  0     R/W  PMM_ENABLE                |
 +------------------------------------------*/
#define PMC_DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_UNUSED_MASK      0xfe00fff8
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_MSK 0x01ff0000
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_PKT_SIZE_OFF 16
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_MSK 0x00000006
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_DSM_TYPE_OFF 1
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_ENABLE_MSK   0x00000001
#define DIGI_M1_REG_PLL_SUPPORT_PMM_CFG_1_BIT_PMM_ENABLE_OFF   0

/*---------------------------------.
 | Register 0x00016010 IRIG_IF_MUX |
 +---------------------------------+
 | bit  2 R/W  IRIG_CLKO_SEL       |
 | bit  1 R/W  IRIG_DATO_SEL       |
 | bit  0 R/W  ONE_PPSO_SEL        |
 +--------------------------------*/
#define PMC_DIGI_M1_REG_IRIG_IF_MUX_UNUSED_MASK       0xfffffff8
#define DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_CLKO_SEL_MSK 0x00000004
#define DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_CLKO_SEL_OFF 2
#define DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_DATO_SEL_MSK 0x00000002
#define DIGI_M1_REG_IRIG_IF_MUX_BIT_IRIG_DATO_SEL_OFF 1
#define DIGI_M1_REG_IRIG_IF_MUX_BIT_ONE_PPSO_SEL_MSK  0x00000001
#define DIGI_M1_REG_IRIG_IF_MUX_BIT_ONE_PPSO_SEL_OFF  0

/*-----------------------------------.
 | Register 0x00016014 CAL_349_CFG_0 |
 +-----------------------------------+
 | bit  21    R/W  CNT_349_EN        |
 | bit  20:14 R/W  CNT_349_ROLLOVER  |
 | bit  13:7  R/W  SS_CAL_349_DLY    |
 | bit  6:0   R/W  SS_TOC_349_DLY    |
 +----------------------------------*/
#define PMC_DIGI_M1_REG_CAL_349_CFG_0_UNUSED_MASK          0xffc00000
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_EN_MSK       0x00200000
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_EN_OFF       21
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_MSK 0x001fc000
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_CNT_349_ROLLOVER_OFF 14
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_MSK   0x00003f80
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_CAL_349_DLY_OFF   7
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_MSK   0x0000007f
#define DIGI_M1_REG_CAL_349_CFG_0_BIT_SS_TOC_349_DLY_OFF   0

/*-----------------------------------.
 | Register 0x00016018 CAL_349_CFG_1 |
 +-----------------------------------+
 | bit  9:0 R/W  SS_TOC_349_EN       |
 +----------------------------------*/
#define PMC_DIGI_M1_REG_CAL_349_CFG_1_UNUSED_MASK       0xfffffc00
#define DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_MSK 0x000003ff
#define DIGI_M1_REG_CAL_349_CFG_1_BIT_SS_TOC_349_EN_OFF 0

/*-----------------------------------.
 | Register 0x0001601c CAL_349_CFG_2 |
 +-----------------------------------+
 | bit  24:0 R/W  SS_CAL_349_EN      |
 +----------------------------------*/
#define PMC_DIGI_M1_REG_CAL_349_CFG_2_UNUSED_MASK       0xfe000000
#define DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_MSK 0x01ffffff
#define DIGI_M1_REG_CAL_349_CFG_2_BIT_SS_CAL_349_EN_OFF 0

/*---------------------------------.
 | Register 0x00016020 CAL_349_REQ |
 +---------------------------------+
 | bit  0 R/W  REQ_CAL_PG_349      |
 +--------------------------------*/
#define PMC_DIGI_M1_REG_CAL_349_REQ_UNUSED_MASK        0xfffffffe
#define DIGI_M1_REG_CAL_349_REQ_BIT_REQ_CAL_PG_349_MSK 0x00000001
#define DIGI_M1_REG_CAL_349_REQ_BIT_REQ_CAL_PG_349_OFF 0

/*-----------------------------------.
 | Register 0x0001603c CAL_500_CFG_0 |
 +-----------------------------------+
 | bit  21    R/W  CNT_500_EN        |
 | bit  20:14 R/W  CNT_500_ROLLOVER  |
 | bit  13:7  R/W  SS_CAL_500_DLY    |
 | bit  6:0   R/W  SS_TOC_500_DLY    |
 +----------------------------------*/
#define PMC_DIGI_M1_REG_CAL_500_CFG_0_UNUSED_MASK          0xffc00000
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_EN_MSK       0x00200000
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_EN_OFF       21
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_MSK 0x001fc000
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_CNT_500_ROLLOVER_OFF 14
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_MSK   0x00003f80
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_CAL_500_DLY_OFF   7
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_MSK   0x0000007f
#define DIGI_M1_REG_CAL_500_CFG_0_BIT_SS_TOC_500_DLY_OFF   0

/*-----------------------------------.
 | Register 0x00016040 CAL_500_CFG_1 |
 +-----------------------------------+
 | bit  9:0 R/W  SS_TOC_500_EN       |
 +----------------------------------*/
#define PMC_DIGI_M1_REG_CAL_500_CFG_1_UNUSED_MASK       0xfffffc00
#define DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_MSK 0x000003ff
#define DIGI_M1_REG_CAL_500_CFG_1_BIT_SS_TOC_500_EN_OFF 0

/*-----------------------------------.
 | Register 0x00016044 CAL_500_CFG_2 |
 +-----------------------------------+
 | bit  25:0 R/W  SS_CAL_500_EN      |
 +----------------------------------*/
#define PMC_DIGI_M1_REG_CAL_500_CFG_2_UNUSED_MASK       0xfc000000
#define DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_MSK 0x03ffffff
#define DIGI_M1_REG_CAL_500_CFG_2_BIT_SS_CAL_500_EN_OFF 0

/*---------------------------------.
 | Register 0x00016048 CAL_500_REQ |
 +---------------------------------+
 | bit  0 R/W  REQ_CAL_PG_500      |
 +--------------------------------*/
#define PMC_DIGI_M1_REG_CAL_500_REQ_UNUSED_MASK        0xfffffffe
#define DIGI_M1_REG_CAL_500_REQ_BIT_REQ_CAL_PG_500_MSK 0x00000001
#define DIGI_M1_REG_CAL_500_REQ_BIT_REQ_CAL_PG_500_OFF 0

/*----------------------------------------.
 | Register 0x00016064 IRIG_IF_OUTPUT_ENS |
 +----------------------------------------+
 | bit  2 R/W  IRIG_CLKO_OEB              |
 | bit  1 R/W  IRIG_DATO_OEB              |
 | bit  0 R/W  ONE_PPSO_OEB               |
 +---------------------------------------*/
#define PMC_DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_UNUSED_MASK       0xfffffff8
#define DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_CLKO_OEB_MSK 0x00000004
#define DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_CLKO_OEB_OFF 2
#define DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_DATO_OEB_MSK 0x00000002
#define DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_IRIG_DATO_OEB_OFF 1
#define DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_ONE_PPSO_OEB_MSK  0x00000001
#define DIGI_M1_REG_IRIG_IF_OUTPUT_ENS_BIT_ONE_PPSO_OEB_OFF  0

/*-------------------------------------.
 | Register 0x00016068 OTU_MFPO_SELECT |
 +-------------------------------------+
 | bit  27:16 R/W  OTU_RX_MFPO_SEL     |
 | bit  11:0  R/W  OTU_TX_MFPO_SEL     |
 +------------------------------------*/
#define PMC_DIGI_M1_REG_OTU_MFPO_SELECT_UNUSED_MASK         0xf000f000
#define DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_MSK 0x0fff0000
#define DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_RX_MFPO_SEL_OFF 16
#define DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_MSK 0x00000fff
#define DIGI_M1_REG_OTU_MFPO_SELECT_BIT_OTU_TX_MFPO_SEL_OFF 0

/* index definitions for PMC_DIGI_M1_REG_EFUSE_STAT */
#define PMC_DIGI_M1_REG_EFUSE_STAT_INDEX_N_MIN    0
#define PMC_DIGI_M1_REG_EFUSE_STAT_INDEX_N_MAX    9
#define PMC_DIGI_M1_REG_EFUSE_STAT_INDEX_N_SIZE   10
#define PMC_DIGI_M1_REG_EFUSE_STAT_INDEX_N_OFFSET 4

/*--------------------------------------------.
 | Register (0x000160a4 + (N) * 4) EFUSE_STAT |
 +--------------------------------------------+
 | bit  31:0 R  EFUSE_DATA                    |
 +-------------------------------------------*/
#define PMC_DIGI_M1_REG_EFUSE_STAT_UNUSED_MASK    0x00000000
#define DIGI_M1_REG_EFUSE_STAT_BIT_EFUSE_DATA_MSK 0xffffffff
#define DIGI_M1_REG_EFUSE_STAT_BIT_EFUSE_DATA_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DIGI_M1_REGS_H */
