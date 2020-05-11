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
 *     cbrc_fc_dlvr block
 *****************************************************************************/
#ifndef _CBRC_FC_DLVR_REGS_H
#define _CBRC_FC_DLVR_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0     0x00053000
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1     0x00053004
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2     0x00053008
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3     0x0005300c
#define PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG( N ) (0x00053010 + (N) * 0x4)
#define PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG      0x00053040
#define PMC_CBRC_FC_DLVR_REG_GENERAL_PURPOSE_CTRL_REG  0x00053044
#define PMC_CBRC_FC_DLVR_REG_GENERAL_PURPOSE_STAT_REG  0x00053048

/*-------------------------------------------.
 | Register 0x00053000 DLVR_TSB_INT_EN_REG_0 |
 +-------------------------------------------+
 | bit  27:16 R/W  FC1200_PTD_IRQ_EN0        |
 | bit  15:4  R/W  FC800_PMG_IRQ_EN0         |
 | bit  2     R/W  PKTDINT_IRQ_EN0           |
 | bit  1     R/W  MPMO_IRQ_EN0              |
 | bit  0     R/W  CBRDINT_IRQ_EN0           |
 +------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_UNUSED_MASK            0xf0000008
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_MSK 0x0fff0000
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC1200_PTD_IRQ_EN0_OFF 16
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_MSK  0x0000fff0
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_FC800_PMG_IRQ_EN0_OFF  4
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_PKTDINT_IRQ_EN0_MSK    0x00000004
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_PKTDINT_IRQ_EN0_OFF    2
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_MPMO_IRQ_EN0_MSK       0x00000002
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_MPMO_IRQ_EN0_OFF       1
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_CBRDINT_IRQ_EN0_MSK    0x00000001
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_0_BIT_CBRDINT_IRQ_EN0_OFF    0

/*-------------------------------------------.
 | Register 0x00053004 DLVR_TSB_INT_EN_REG_1 |
 +-------------------------------------------+
 | bit  27:16 R/W  FC1200_PTD_IRQ_EN1        |
 | bit  15:4  R/W  FC800_PMG_IRQ_EN1         |
 | bit  2     R/W  PKTDINT_IRQ_EN1           |
 | bit  1     R/W  MPMO_IRQ_EN1              |
 | bit  0     R/W  CBRDINT_IRQ_EN1           |
 +------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_UNUSED_MASK            0xf0000008
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_MSK 0x0fff0000
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC1200_PTD_IRQ_EN1_OFF 16
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_MSK  0x0000fff0
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_FC800_PMG_IRQ_EN1_OFF  4
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_PKTDINT_IRQ_EN1_MSK    0x00000004
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_PKTDINT_IRQ_EN1_OFF    2
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_MPMO_IRQ_EN1_MSK       0x00000002
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_MPMO_IRQ_EN1_OFF       1
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_CBRDINT_IRQ_EN1_MSK    0x00000001
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_1_BIT_CBRDINT_IRQ_EN1_OFF    0

/*-------------------------------------------.
 | Register 0x00053008 DLVR_TSB_INT_EN_REG_2 |
 +-------------------------------------------+
 | bit  27:16 R/W  FC1200_PTD_IRQ_EN2        |
 | bit  15:4  R/W  FC800_PMG_IRQ_EN2         |
 | bit  2     R/W  PKTDINT_IRQ_EN2           |
 | bit  1     R/W  MPMO_IRQ_EN2              |
 | bit  0     R/W  CBRDINT_IRQ_EN2           |
 +------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_UNUSED_MASK            0xf0000008
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_MSK 0x0fff0000
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC1200_PTD_IRQ_EN2_OFF 16
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_MSK  0x0000fff0
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_FC800_PMG_IRQ_EN2_OFF  4
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_PKTDINT_IRQ_EN2_MSK    0x00000004
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_PKTDINT_IRQ_EN2_OFF    2
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_MPMO_IRQ_EN2_MSK       0x00000002
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_MPMO_IRQ_EN2_OFF       1
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_CBRDINT_IRQ_EN2_MSK    0x00000001
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_2_BIT_CBRDINT_IRQ_EN2_OFF    0

/*-------------------------------------------.
 | Register 0x0005300c DLVR_TSB_INT_EN_REG_3 |
 +-------------------------------------------+
 | bit  27:16 R/W  FC1200_PTD_IRQ_EN3        |
 | bit  15:4  R/W  FC800_PMG_IRQ_EN3         |
 | bit  2     R/W  PKTDINT_IRQ_EN3           |
 | bit  1     R/W  MPMO_IRQ_EN3              |
 | bit  0     R/W  CBRDINT_IRQ_EN3           |
 +------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_UNUSED_MASK            0xf0000008
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_MSK 0x0fff0000
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC1200_PTD_IRQ_EN3_OFF 16
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_MSK  0x0000fff0
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_FC800_PMG_IRQ_EN3_OFF  4
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_PKTDINT_IRQ_EN3_MSK    0x00000004
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_PKTDINT_IRQ_EN3_OFF    2
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_MPMO_IRQ_EN3_MSK       0x00000002
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_MPMO_IRQ_EN3_OFF       1
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_CBRDINT_IRQ_EN3_MSK    0x00000001
#define CBRC_FC_DLVR_REG_DLVR_TSB_INT_EN_REG_3_BIT_CBRDINT_IRQ_EN3_OFF    0

/* index definitions for PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG */
#define PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_INDEX_N_MIN            0
#define PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_INDEX_N_MAX            11
#define PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_INDEX_N_SIZE           12
#define PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00053010 + (N) * 0x4) DLVR_CHANNEL_CFG_REG |
 +--------------------------------------------------------+
 | bit  3 R/W  FC800PMG_CH_EN                             |
 | bit  2 R/W  FCPTD_CH_EN                                |
 | bit  1 R/W  LPBK_MUX_SEL                               |
 | bit  0 R/W  FCPTD_FC800PMG_SEL                         |
 +-------------------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_UNUSED_MASK            0xfffffff0
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FC800PMG_CH_EN_MSK     0x00000008
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FC800PMG_CH_EN_OFF     3
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_CH_EN_MSK        0x00000004
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_CH_EN_OFF        2
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_LPBK_MUX_SEL_MSK       0x00000002
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_LPBK_MUX_SEL_OFF       1
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_FC800PMG_SEL_MSK 0x00000001
#define CBRC_FC_DLVR_REG_DLVR_CHANNEL_CFG_REG_BIT_FCPTD_FC800PMG_SEL_OFF 0

/*------------------------------------------.
 | Register 0x00053040 DLVR_GLOBAL_CTRL_REG |
 +------------------------------------------+
 | bit  4 R/W  CBRDINT_RAM_SD_REG           |
 | bit  3 R/W  PKTDINT_RAM_SD_REG           |
 | bit  2 R/W  MPMO_RAM_SD_REG              |
 | bit  1 R/W  CBR_CH_EN                    |
 | bit  0 R/W  DLVR_SYNC_RST                |
 +-----------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_UNUSED_MASK            0xffffffc0
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBRDINT_RAM_SD_REG_MSK 0x00000010
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBRDINT_RAM_SD_REG_OFF 4
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_PKTDINT_RAM_SD_REG_MSK 0x00000008
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_PKTDINT_RAM_SD_REG_OFF 3
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_MPMO_RAM_SD_REG_MSK    0x00000004
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_MPMO_RAM_SD_REG_OFF    2
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_MSK          0x00000002
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_CBR_CH_EN_OFF          1
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_DLVR_SYNC_RST_MSK      0x00000001
#define CBRC_FC_DLVR_REG_DLVR_GLOBAL_CTRL_REG_BIT_DLVR_SYNC_RST_OFF      0

/*----------------------------------------------.
 | Register 0x00053044 GENERAL_PURPOSE_CTRL_REG |
 +----------------------------------------------+
 +---------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_GENERAL_PURPOSE_CTRL_REG_UNUSED_MASK 0xffffff00

/*----------------------------------------------.
 | Register 0x00053048 GENERAL_PURPOSE_STAT_REG |
 +----------------------------------------------+
 +---------------------------------------------*/
#define PMC_CBRC_FC_DLVR_REG_GENERAL_PURPOSE_STAT_REG_UNUSED_MASK 0xffffff00

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_FC_DLVR_REGS_H */
