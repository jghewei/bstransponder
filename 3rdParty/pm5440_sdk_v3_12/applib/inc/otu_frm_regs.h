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
 *     otu_frm block
 *****************************************************************************/
#ifndef _OTU_FRM_REGS_H
#define _OTU_FRM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG           0x00000000
#define PMC_OTU_FRM_REG_OTU_FRM_SW_PMON              0x00000004
#define PMC_OTU_FRM_REG_OTU_FRM_SW_RESET             0x0000000c
#define PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT 0x00000018
#define PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG   0x0000001c
#define PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE( N )      (0x00000040 + (N) * 0x04)
#define PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE( N )      (0x00000070 + (N) * 0x04)
#define PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE          0x0000007c
#define PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG( N )      (0x00000080 + (N) * 0x04)
#define PMC_OTU_FRM_REG_OTU_FRM_SPARE                0x000000e8
#define PMC_OTU_FRM_REG_OTU_FRM_INT_SUMMARY          0x000000fc
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E         0x00000100
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E         0x00000104
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E        0x00000108
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E        0x0000010c
#define PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E              0x00000110
#define PMC_OTU_FRM_REG_OTU_FRM_OOM_E                0x00000114
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I         0x00000120
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I         0x00000124
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I        0x00000128
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I        0x0000012c
#define PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I              0x00000130
#define PMC_OTU_FRM_REG_OTU_FRM_OOM_I                0x00000134
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V         0x00000160
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V         0x00000164
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V        0x00000168
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V        0x0000016c
#define PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V              0x00000170
#define PMC_OTU_FRM_REG_OTU_FRM_OOM_V                0x00000174

/*----------------------------------------.
 | Register 0x00000000 OTU_FRM_MASTER_CFG |
 +----------------------------------------+
 | bit  31 R/W  OTU_FRM_SW_RESET          |
 | bit  1  R/W  OTU_FRM_SW_RAM_LOWPWR     |
 | bit  0  R/W  OTU_FRM_SW_LOWPWR         |
 +---------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_MASTER_CFG_UNUSED_MASK               0x7ffffe00
#define OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RESET_MSK      0x80000000
#define OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RESET_OFF      31
#define OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RAM_LOWPWR_MSK 0x00000002
#define OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_RAM_LOWPWR_OFF 1
#define OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_LOWPWR_MSK     0x00000001
#define OTU_FRM_REG_OTU_FRM_MASTER_CFG_BIT_OTU_FRM_SW_LOWPWR_OFF     0

/*-------------------------------------.
 | Register 0x00000004 OTU_FRM_SW_PMON |
 +-------------------------------------+
 | bit  1 R/W  OTU_FRM_LCLK_EN         |
 | bit  0 R/W  OTU_FRM_PMON_UPDATE     |
 +------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_SW_PMON_UNUSED_MASK             0xfffffff8
#define OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_LCLK_EN_MSK     0x00000002
#define OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_LCLK_EN_OFF     1
#define OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_PMON_UPDATE_MSK 0x00000001
#define OTU_FRM_REG_OTU_FRM_SW_PMON_BIT_OTU_FRM_PMON_UPDATE_OFF 0

/*--------------------------------------------.
 | Register 0x0000000c OTU_FRM_SW_RESET       |
 +--------------------------------------------+
 | bit  13   R/W  OTU_FRM_SW_SYSCLK_RESET     |
 | bit  12   R/W  OTU_FRM_SW_CLK355M_RESET    |
 | bit  11:0 R/W  OTU_FRM_SW_RX_LINECLK_RESET |
 +-------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_SW_RESET_UNUSED_MASK                     0xffffc000
#define OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_SYSCLK_RESET_MSK     0x00002000
#define OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_SYSCLK_RESET_OFF     13
#define OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_CLK355M_RESET_MSK    0x00001000
#define OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_CLK355M_RESET_OFF    12
#define OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_SW_RESET_BIT_OTU_FRM_SW_RX_LINECLK_RESET_OFF 0

/*--------------------------------------------------.
 | Register 0x00000018 OTU_FRM_SF_SQUELCH_CNT_LIMIT |
 +--------------------------------------------------+
 | bit  31:0 R/W  OTU_FRM_SF_CNT_LIMIT              |
 +-------------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_UNUSED_MASK              0x00000000
#define OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_MSK 0xffffffff
#define OTU_FRM_REG_OTU_FRM_SF_SQUELCH_CNT_LIMIT_BIT_OTU_FRM_SF_CNT_LIMIT_OFF 0

/*------------------------------------------------.
 | Register 0x0000001c OTU_FRM_RX_LINECLK_MUX_CFG |
 +------------------------------------------------+
 | bit  22  R/W  OTU_FRM_RX_LINECLK_11_SRC2_EN    |
 | bit  21  R/W  OTU_FRM_RX_LINECLK_11_SRC1_EN    |
 | bit  20  R/W  OTU_FRM_RX_LINECLK_11_SRC0_EN    |
 | bit  19  R/W  OTU_FRM_RX_LINECLK_7_SRC1_EN     |
 | bit  18  R/W  OTU_FRM_RX_LINECLK_7_SRC0_EN     |
 | bit  17  R/W  OTU_FRM_RX_LINECLK_3_SRC1_EN     |
 | bit  16  R/W  OTU_FRM_RX_LINECLK_3_SRC0_EN     |
 | bit  3:2 R/W  OTU_FRM_RX_LINECLK_11_SEL        |
 | bit  1   R/W  OTU_FRM_RX_LINECLK_7_SEL         |
 | bit  0   R/W  OTU_FRM_RX_LINECLK_3_SEL         |
 +-----------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_UNUSED_MASK                       0xff80fff0
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC2_EN_MSK 0x00400000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC2_EN_OFF 22
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC1_EN_MSK 0x00200000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC1_EN_OFF 21
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC0_EN_MSK 0x00100000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SRC0_EN_OFF 20
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC1_EN_MSK  0x00080000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC1_EN_OFF  19
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC0_EN_MSK  0x00040000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SRC0_EN_OFF  18
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC1_EN_MSK  0x00020000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC1_EN_OFF  17
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC0_EN_MSK  0x00010000
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SRC0_EN_OFF  16
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_MSK     0x0000000c
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_11_SEL_OFF     2
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SEL_MSK      0x00000002
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_7_SEL_OFF      1
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SEL_MSK      0x00000001
#define OTU_FRM_REG_OTU_FRM_RX_LINECLK_MUX_CFG_BIT_OTU_FRM_RX_LINECLK_3_SEL_OFF      0

/* index definitions for PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE */
#define PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE_INDEX_N_MIN                     0
#define PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE_INDEX_N_MAX                     11
#define PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE_INDEX_N_SIZE                    12
#define PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE_INDEX_N_OFFSET                  0x04

/*-------------------------------------------------------.
 | Register (0x00000040 + (N) * 0x04) OTU_FRM_10G_ENABLE |
 +-------------------------------------------------------+
 | bit  10 R/W  OTU_FRM_10G_SCRAMBLING_EN                |
 | bit  9  R/W  OTU_FRM_10G_FEC_COL_INS_EN               |
 | bit  8  R/W  OTU_FRM_10G_DINTLV_EN                    |
 | bit  6  R/W  OTU_FRM_10G_MI_ACTIVE                    |
 | bit  5  R/W  OTU_FRM_10G_DLOM_TO_SF_EN                |
 | bit  4  R/W  OTU_FRM_10G_DLOM_EN                      |
 | bit  3  R/W  OTU_FRM_10G_SF_SQUELCH_EN                |
 | bit  2  R/W  OTU_FRM_10G_DESCRAMBLING_EN              |
 | bit  1  R/W  OTU_FRM_10G_FEC_COL_RM_EN                |
 | bit  0  R/W  OTU_FRM_10G_INTLV_EN                     |
 +------------------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_10G_ENABLE_UNUSED_MASK                     0xfffef880
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SCRAMBLING_EN_MSK   0x00000400
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SCRAMBLING_EN_OFF   10
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_INS_EN_MSK  0x00000200
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_INS_EN_OFF  9
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DINTLV_EN_MSK       0x00000100
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DINTLV_EN_OFF       8
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_MI_ACTIVE_MSK       0x00000040
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_MI_ACTIVE_OFF       6
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_TO_SF_EN_MSK   0x00000020
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_TO_SF_EN_OFF   5
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_EN_MSK         0x00000010
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DLOM_EN_OFF         4
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SF_SQUELCH_EN_MSK   0x00000008
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_SF_SQUELCH_EN_OFF   3
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DESCRAMBLING_EN_MSK 0x00000004
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_DESCRAMBLING_EN_OFF 2
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_RM_EN_MSK   0x00000002
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_FEC_COL_RM_EN_OFF   1
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_INTLV_EN_MSK        0x00000001
#define OTU_FRM_REG_OTU_FRM_10G_ENABLE_BIT_OTU_FRM_10G_INTLV_EN_OFF        0

/* index definitions for PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE */
#define PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE_INDEX_N_MIN                     0
#define PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE_INDEX_N_MAX                     2
#define PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE_INDEX_N_SIZE                    3
#define PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE_INDEX_N_OFFSET                  0x04

/*-------------------------------------------------------.
 | Register (0x00000070 + (N) * 0x04) OTU_FRM_40G_ENABLE |
 +-------------------------------------------------------+
 | bit  10 R/W  OTU_FRM_40G_SCRAMBLING_EN                |
 | bit  9  R/W  OTU_FRM_40G_FEC_COL_INS_EN               |
 | bit  8  R/W  OTU_FRM_40G_DINTLV_EN                    |
 | bit  6  R/W  OTU_FRM_40G_MI_ACTIVE                    |
 | bit  5  R/W  OTU_FRM_40G_DLOM_TO_SF_EN                |
 | bit  4  R/W  OTU_FRM_40G_DLOM_EN                      |
 | bit  3  R/W  OTU_FRM_40G_SF_SQUELCH_EN                |
 | bit  2  R/W  OTU_FRM_40G_DESCRAMBLING_EN              |
 | bit  1  R/W  OTU_FRM_40G_FEC_COL_RM_EN                |
 | bit  0  R/W  OTU_FRM_40G_INTLV_EN                     |
 +------------------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_40G_ENABLE_UNUSED_MASK                     0xfffef880
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SCRAMBLING_EN_MSK   0x00000400
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SCRAMBLING_EN_OFF   10
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_INS_EN_MSK  0x00000200
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_INS_EN_OFF  9
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DINTLV_EN_MSK       0x00000100
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DINTLV_EN_OFF       8
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_MI_ACTIVE_MSK       0x00000040
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_MI_ACTIVE_OFF       6
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_TO_SF_EN_MSK   0x00000020
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_TO_SF_EN_OFF   5
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_EN_MSK         0x00000010
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DLOM_EN_OFF         4
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SF_SQUELCH_EN_MSK   0x00000008
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_SF_SQUELCH_EN_OFF   3
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DESCRAMBLING_EN_MSK 0x00000004
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_DESCRAMBLING_EN_OFF 2
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_RM_EN_MSK   0x00000002
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_FEC_COL_RM_EN_OFF   1
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_INTLV_EN_MSK        0x00000001
#define OTU_FRM_REG_OTU_FRM_40G_ENABLE_BIT_OTU_FRM_40G_INTLV_EN_OFF        0

/*-------------------------------------------.
 | Register 0x0000007c OTU_FRM_100G_ENABLE   |
 +-------------------------------------------+
 | bit  10 R/W  OTU_FRM_100G_SCRAMBLING_EN   |
 | bit  9  R/W  OTU_FRM_100G_FEC_COL_INS_EN  |
 | bit  8  R/W  OTU_FRM_100G_DINTLV_EN       |
 | bit  6  R/W  OTU_FRM_100G_MI_ACTIVE       |
 | bit  5  R/W  OTU_FRM_100G_DLOM_TO_SF_EN   |
 | bit  4  R/W  OTU_FRM_100G_DLOM_EN         |
 | bit  3  R/W  OTU_FRM_100G_SF_SQUELCH_EN   |
 | bit  2  R/W  OTU_FRM_100G_DESCRAMBLING_EN |
 | bit  1  R/W  OTU_FRM_100G_FEC_COL_RM_EN   |
 | bit  0  R/W  OTU_FRM_100G_INTLV_EN        |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_100G_ENABLE_UNUSED_MASK                      0xfffef880
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SCRAMBLING_EN_MSK   0x00000400
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SCRAMBLING_EN_OFF   10
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_INS_EN_MSK  0x00000200
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_INS_EN_OFF  9
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DINTLV_EN_MSK       0x00000100
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DINTLV_EN_OFF       8
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_MI_ACTIVE_MSK       0x00000040
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_MI_ACTIVE_OFF       6
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_TO_SF_EN_MSK   0x00000020
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_TO_SF_EN_OFF   5
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_EN_MSK         0x00000010
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DLOM_EN_OFF         4
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SF_SQUELCH_EN_MSK   0x00000008
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_SF_SQUELCH_EN_OFF   3
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DESCRAMBLING_EN_MSK 0x00000004
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_DESCRAMBLING_EN_OFF 2
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_RM_EN_MSK   0x00000002
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_FEC_COL_RM_EN_OFF   1
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_INTLV_EN_MSK        0x00000001
#define OTU_FRM_REG_OTU_FRM_100G_ENABLE_BIT_OTU_FRM_100G_INTLV_EN_OFF        0

/* index definitions for PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG */
#define PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG_INDEX_N_MIN                   0
#define PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG_INDEX_N_MAX                   11
#define PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG_INDEX_N_SIZE                  12
#define PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG_INDEX_N_OFFSET                0x04

/*-------------------------------------------------------.
 | Register (0x00000080 + (N) * 0x04) OTU_FRM_STREAM_CFG |
 +-------------------------------------------------------+
 | bit  24:16 R/W  OTU_FRM_DINTLV_XOFF_THRES             |
 | bit  6:0   R/W  OTU_FRM_CH_ID                         |
 +------------------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_STREAM_CFG_UNUSED_MASK                   0xfe00ff80
#define OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_MSK 0x01ff0000
#define OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_DINTLV_XOFF_THRES_OFF 16
#define OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_MSK             0x0000007f
#define OTU_FRM_REG_OTU_FRM_STREAM_CFG_BIT_OTU_FRM_CH_ID_OFF             0

/*-----------------------------------.
 | Register 0x000000e8 OTU_FRM_SPARE |
 +-----------------------------------+
 | bit  31:0 R/W  OTU_FRM_SPARE      |
 +----------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_SPARE_UNUSED_MASK       0x00000000
#define OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_MSK 0xffffffff
#define OTU_FRM_REG_OTU_FRM_SPARE_BIT_OTU_FRM_SPARE_OFF 0

/*-----------------------------------------.
 | Register 0x000000fc OTU_FRM_INT_SUMMARY |
 +-----------------------------------------+
 | bit  29:0 R  OTU_FRM_INT_SUMMARY        |
 +----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INT_SUMMARY_UNUSED_MASK             0xc0000000
#define OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_MSK 0x3fffffff
#define OTU_FRM_REG_OTU_FRM_INT_SUMMARY_BIT_OTU_FRM_INT_SUMMARY_OFF 0

/*------------------------------------------.
 | Register 0x00000100 OTU_FRM_INTLV_WOVR_E |
 +------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_INTLV_WOVR_E      |
 +-----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_UNUSED_MASK              0xfffff000
#define OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_INTLV_WOVR_E_BIT_OTU_FRM_INTLV_WOVR_E_OFF 0

/*------------------------------------------.
 | Register 0x00000104 OTU_FRM_INTLV_RUDR_E |
 +------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_INTLV_RUDR_E      |
 +-----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_UNUSED_MASK              0xfffff000
#define OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_INTLV_RUDR_E_BIT_OTU_FRM_INTLV_RUDR_E_OFF 0

/*-------------------------------------------.
 | Register 0x00000108 OTU_FRM_DINTLV_WOVR_E |
 +-------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_DINTLV_WOVR_E      |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_UNUSED_MASK               0xfffff000
#define OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_E_BIT_OTU_FRM_DINTLV_WOVR_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000010c OTU_FRM_DINTLV_RUDR_E |
 +-------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_DINTLV_RUDR_E      |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_UNUSED_MASK               0xfffff000
#define OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_E_BIT_OTU_FRM_DINTLV_RUDR_E_OFF 0

/*-------------------------------------.
 | Register 0x00000110 OTU_FRM_D_LOM_E |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_FRM_D_LOM_E      |
 +------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_D_LOM_E_UNUSED_MASK         0xfffff000
#define OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_D_LOM_E_BIT_OTU_FRM_D_LOM_E_OFF 0

/*-----------------------------------.
 | Register 0x00000114 OTU_FRM_OOM_E |
 +-----------------------------------+
 | bit  11:0 R/W  OTU_FRM_OOM_E      |
 +----------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_OOM_E_UNUSED_MASK       0xfffff000
#define OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_OOM_E_BIT_OTU_FRM_OOM_E_OFF 0

/*------------------------------------------.
 | Register 0x00000120 OTU_FRM_INTLV_WOVR_I |
 +------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_INTLV_WOVR_I      |
 +-----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_UNUSED_MASK              0xfffff000
#define OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_INTLV_WOVR_I_BIT_OTU_FRM_INTLV_WOVR_I_OFF 0

/*------------------------------------------.
 | Register 0x00000124 OTU_FRM_INTLV_RUDR_I |
 +------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_INTLV_RUDR_I      |
 +-----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_UNUSED_MASK              0xfffff000
#define OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_INTLV_RUDR_I_BIT_OTU_FRM_INTLV_RUDR_I_OFF 0

/*-------------------------------------------.
 | Register 0x00000128 OTU_FRM_DINTLV_WOVR_I |
 +-------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_DINTLV_WOVR_I      |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_UNUSED_MASK               0xfffff000
#define OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_I_BIT_OTU_FRM_DINTLV_WOVR_I_OFF 0

/*-------------------------------------------.
 | Register 0x0000012c OTU_FRM_DINTLV_RUDR_I |
 +-------------------------------------------+
 | bit  11:0 R/W  OTU_FRM_DINTLV_RUDR_I      |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_UNUSED_MASK               0xfffff000
#define OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_I_BIT_OTU_FRM_DINTLV_RUDR_I_OFF 0

/*-------------------------------------.
 | Register 0x00000130 OTU_FRM_D_LOM_I |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_FRM_D_LOM_I      |
 +------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_D_LOM_I_UNUSED_MASK         0xfffff000
#define OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_D_LOM_I_BIT_OTU_FRM_D_LOM_I_OFF 0

/*-----------------------------------.
 | Register 0x00000134 OTU_FRM_OOM_I |
 +-----------------------------------+
 | bit  11:0 R/W  OTU_FRM_OOM_I      |
 +----------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_OOM_I_UNUSED_MASK       0xfffff000
#define OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_OOM_I_BIT_OTU_FRM_OOM_I_OFF 0

/*------------------------------------------.
 | Register 0x00000160 OTU_FRM_INTLV_WOVR_V |
 +------------------------------------------+
 | bit  11:0 R  OTU_FRM_INTLV_WOVR_V        |
 +-----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_UNUSED_MASK              0xfffff000
#define OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_INTLV_WOVR_V_BIT_OTU_FRM_INTLV_WOVR_V_OFF 0

/*------------------------------------------.
 | Register 0x00000164 OTU_FRM_INTLV_RUDR_V |
 +------------------------------------------+
 | bit  11:0 R  OTU_FRM_INTLV_RUDR_V        |
 +-----------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_UNUSED_MASK              0xfffff000
#define OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_INTLV_RUDR_V_BIT_OTU_FRM_INTLV_RUDR_V_OFF 0

/*-------------------------------------------.
 | Register 0x00000168 OTU_FRM_DINTLV_WOVR_V |
 +-------------------------------------------+
 | bit  11:0 R  OTU_FRM_DINTLV_WOVR_V        |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_UNUSED_MASK               0xfffff000
#define OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_DINTLV_WOVR_V_BIT_OTU_FRM_DINTLV_WOVR_V_OFF 0

/*-------------------------------------------.
 | Register 0x0000016c OTU_FRM_DINTLV_RUDR_V |
 +-------------------------------------------+
 | bit  11:0 R  OTU_FRM_DINTLV_RUDR_V        |
 +------------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_UNUSED_MASK               0xfffff000
#define OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_DINTLV_RUDR_V_BIT_OTU_FRM_DINTLV_RUDR_V_OFF 0

/*-------------------------------------.
 | Register 0x00000170 OTU_FRM_D_LOM_V |
 +-------------------------------------+
 | bit  11:0 R  OTU_FRM_D_LOM_V        |
 +------------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_D_LOM_V_UNUSED_MASK         0xfffff000
#define OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_D_LOM_V_BIT_OTU_FRM_D_LOM_V_OFF 0

/*-----------------------------------.
 | Register 0x00000174 OTU_FRM_OOM_V |
 +-----------------------------------+
 | bit  11:0 R  OTU_FRM_OOM_V        |
 +----------------------------------*/
#define PMC_OTU_FRM_REG_OTU_FRM_OOM_V_UNUSED_MASK       0xfffff000
#define OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_MSK 0x00000fff
#define OTU_FRM_REG_OTU_FRM_OOM_V_BIT_OTU_FRM_OOM_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTU_FRM_REGS_H */
