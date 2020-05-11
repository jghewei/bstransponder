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
 *     mapotn_tx block
 *****************************************************************************/
#ifndef _MAPOTN_TX_REGS_H
#define _MAPOTN_TX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MAPOTN_TX_CORE_REG_SOFT_RST              0x00080000
#define PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE           0x00080004
#define PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE       0x0008000c
#define PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR         0x00080010
#define PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN 0x00080020
#define PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR   0x00080024

/*--------------------------------.
 | Register 0x00080000 SOFT_RST   |
 +--------------------------------+
 | bit  4 R/W  ODUKSC_SOFT_RST    |
 | bit  3 R/W  MPMO_SOFT_RST      |
 | bit  2 R/W  TGMP96_SOFT_RST    |
 | bit  1 R/W  TGFPF96_SOFT_RST   |
 | bit  0 R/W  MAPOTN_TX_SOFT_RST |
 +-------------------------------*/
#define PMC_MAPOTN_TX_CORE_REG_SOFT_RST_UNUSED_MASK            0xffffffe0
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_ODUKSC_SOFT_RST_MSK    0x00000010
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_ODUKSC_SOFT_RST_OFF    4
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MPMO_SOFT_RST_MSK      0x00000008
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MPMO_SOFT_RST_OFF      3
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGMP96_SOFT_RST_MSK    0x00000004
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGMP96_SOFT_RST_OFF    2
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGFPF96_SOFT_RST_MSK   0x00000002
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_TGFPF96_SOFT_RST_OFF   1
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MAPOTN_TX_SOFT_RST_MSK 0x00000001
#define MAPOTN_TX_CORE_REG_SOFT_RST_BIT_MAPOTN_TX_SOFT_RST_OFF 0

/*---------------------------------.
 | Register 0x00080004 LOWPWR_MODE |
 +---------------------------------+
 | bit  3 R/W  ODUKSC_LOWPWR       |
 | bit  2 R/W  MPMO_LOWPWR         |
 | bit  1 R/W  TGMP96_LOWPWR       |
 | bit  0 R/W  TGFPF96_LOWPWR      |
 +--------------------------------*/
#define PMC_MAPOTN_TX_CORE_REG_LOWPWR_MODE_UNUSED_MASK        0xfffffff0
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_ODUKSC_LOWPWR_MSK  0x00000008
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_ODUKSC_LOWPWR_OFF  3
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_MPMO_LOWPWR_MSK    0x00000004
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_MPMO_LOWPWR_OFF    2
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGMP96_LOWPWR_MSK  0x00000002
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGMP96_LOWPWR_OFF  1
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGFPF96_LOWPWR_MSK 0x00000001
#define MAPOTN_TX_CORE_REG_LOWPWR_MODE_BIT_TGFPF96_LOWPWR_OFF 0

/*-------------------------------------.
 | Register 0x0008000c RAM_LOWPWR_MODE |
 +-------------------------------------+
 | bit  3 R/W  ODUKSC_RAM_LOWPWR       |
 | bit  2 R/W  MPMO_RAM_LOWPWR         |
 | bit  1 R/W  TGMP96_RAM_LOWPWR       |
 | bit  0 R/W  TGFPF96_RAM_LOWPWR      |
 +------------------------------------*/
#define PMC_MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_UNUSED_MASK            0xffffff00
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_ODUKSC_RAM_LOWPWR_MSK  0x00000008
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_ODUKSC_RAM_LOWPWR_OFF  3
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_MPMO_RAM_LOWPWR_MSK    0x00000004
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_MPMO_RAM_LOWPWR_OFF    2
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGMP96_RAM_LOWPWR_MSK  0x00000002
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGMP96_RAM_LOWPWR_OFF  1
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGFPF96_RAM_LOWPWR_MSK 0x00000001
#define MAPOTN_TX_CORE_REG_RAM_LOWPWR_MODE_BIT_TGFPF96_RAM_LOWPWR_OFF 0

/*-----------------------------------.
 | Register 0x00080010 ENET_FIFO_CLR |
 +-----------------------------------+
 | bit  0 R/W  FIFO_CLRB             |
 +----------------------------------*/
#define PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_UNUSED_MASK   0xfffffffe
#define MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_BIT_FIFO_CLRB_MSK 0x00000001
#define MAPOTN_TX_CORE_REG_ENET_FIFO_CLR_BIT_FIFO_CLRB_OFF 0

/*-------------------------------------------.
 | Register 0x00080020 ENET_FIFO_OVFL_INT_EN |
 +-------------------------------------------+
 | bit  0 R/W  ENET_FIFO_OVFL_E              |
 +------------------------------------------*/
#define PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_UNUSED_MASK          0xfffffffe
#define MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_BIT_ENET_FIFO_OVFL_E_MSK 0x00000001
#define MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INT_EN_BIT_ENET_FIFO_OVFL_E_OFF 0

/*-----------------------------------------.
 | Register 0x00080024 ENET_FIFO_OVFL_INTR |
 +-----------------------------------------+
 | bit  0 R/W  ENET_FIFO_OVFL_I            |
 +----------------------------------------*/
#define PMC_MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_UNUSED_MASK          0xfffffffe
#define MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_MSK 0x00000001
#define MAPOTN_TX_CORE_REG_ENET_FIFO_OVFL_INTR_BIT_ENET_FIFO_OVFL_I_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_TX_REGS_H */
