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
 *     mapotn_rx block
 *****************************************************************************/
#ifndef _MAPOTN_RX_REGS_H
#define _MAPOTN_RX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MAPOTN_RX_REG_REG_SOFT_RST                            0x00000000
#define PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE                         0x00000004
#define PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB             0x00000008
#define PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE 0x0000000c
#define PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR        0x00000010
#define PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE                     0x00000018
#define PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER( N )        (0x0000001c + (N) * 0x4)
#define PMC_MAPOTN_RX_REG_REG_INTR_EN_REG( N )                    (0x00000300 + (N) * 0x4)
#define PMC_MAPOTN_RX_REG_REG_INTR_REG                            0x00000310

/*--------------------------------.
 | Register 0x00000000 SOFT_RST   |
 +--------------------------------+
 | bit  3 R/W  MPMA_SOFT_RST      |
 | bit  2 R/W  RGMP96_SOFT_RST    |
 | bit  1 R/W  RGFPF96_SOFT_RST   |
 | bit  0 R/W  MAPOTN_RX_SOFT_RST |
 +-------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_SOFT_RST_UNUSED_MASK            0xfffffff0
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_MPMA_SOFT_RST_MSK      0x00000008
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_MPMA_SOFT_RST_OFF      3
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGMP96_SOFT_RST_MSK    0x00000004
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGMP96_SOFT_RST_OFF    2
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGFPF96_SOFT_RST_MSK   0x00000002
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_RGFPF96_SOFT_RST_OFF   1
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_MAPOTN_RX_SOFT_RST_MSK 0x00000001
#define MAPOTN_RX_REG_REG_SOFT_RST_BIT_MAPOTN_RX_SOFT_RST_OFF 0

/*---------------------------------.
 | Register 0x00000004 LOWPWR_MODE |
 +---------------------------------+
 | bit  2 R/W  MPMA_LOWPWR         |
 | bit  1 R/W  RGMP96_LOWPWR       |
 | bit  0 R/W  RGFPF96_LOWPWR      |
 +--------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_LOWPWR_MODE_UNUSED_MASK        0xfffffff8
#define MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_MPMA_LOWPWR_MSK    0x00000004
#define MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_MPMA_LOWPWR_OFF    2
#define MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGMP96_LOWPWR_MSK  0x00000002
#define MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGMP96_LOWPWR_OFF  1
#define MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGFPF96_LOWPWR_MSK 0x00000001
#define MAPOTN_RX_REG_REG_LOWPWR_MODE_BIT_RGFPF96_LOWPWR_OFF 0

/*---------------------------------------------.
 | Register 0x00000008 DCI_DPI_ASYNC_FIFO_RSTB |
 +---------------------------------------------+
 | bit  0 R/W  FIFO_RSTB                       |
 +--------------------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_UNUSED_MASK   0xfffffffe
#define MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_BIT_FIFO_RSTB_MSK 0x00000001
#define MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_RSTB_BIT_FIFO_RSTB_OFF 0

/*---------------------------------------------------------.
 | Register 0x0000000c DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE |
 +---------------------------------------------------------+
 | bit  0 R/W  FIFO_FULL_E                                 |
 +--------------------------------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_UNUSED_MASK     0xfffffffe
#define MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_BIT_FIFO_FULL_E_MSK 0x00000001
#define MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_ENABLE_BIT_FIFO_FULL_E_OFF 0

/*--------------------------------------------------.
 | Register 0x00000010 DCI_DPI_ASYNC_FIFO_FULL_INTR |
 +--------------------------------------------------+
 | bit  0 R/W  FIFO_FULL_I                          |
 +-------------------------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_UNUSED_MASK     0xfffffffe
#define MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_MSK 0x00000001
#define MAPOTN_RX_REG_REG_DCI_DPI_ASYNC_FIFO_FULL_INTR_BIT_FIFO_FULL_I_OFF 0

/*-------------------------------------.
 | Register 0x00000018 RAM_LOWPWR_MODE |
 +-------------------------------------+
 | bit  3 R/W  TOP_RAM_LOWPWR          |
 | bit  2 R/W  MPMA_RAM_LOWPWR         |
 | bit  1 R/W  RGMP96_RAM_LOWPWR       |
 | bit  0 R/W  RGFPF96_RAM_LOWPWR      |
 +------------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_UNUSED_MASK            0xffffff00
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_TOP_RAM_LOWPWR_MSK     0x00000008
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_TOP_RAM_LOWPWR_OFF     3
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_MPMA_RAM_LOWPWR_MSK    0x00000004
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_MPMA_RAM_LOWPWR_OFF    2
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGMP96_RAM_LOWPWR_MSK  0x00000002
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGMP96_RAM_LOWPWR_OFF  1
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGFPF96_RAM_LOWPWR_MSK 0x00000001
#define MAPOTN_RX_REG_REG_RAM_LOWPWR_MODE_BIT_RGFPF96_RAM_LOWPWR_OFF 0

/* index definitions for PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER */
#define PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_INDEX_N_MIN    0
#define PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_INDEX_N_MAX    2
#define PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_INDEX_N_SIZE   3
#define PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------.
 | Register (0x0000001c + (N) * 0x4) DATA_PATH_CTRL_REGISTER |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  DCI2DPI_EN                                 |
 +----------------------------------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_UNUSED_MASK    0x00000000
#define MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_BIT_DCI2DPI_EN_MSK 0xffffffff
#define MAPOTN_RX_REG_REG_DATA_PATH_CTRL_REGISTER_BIT_DCI2DPI_EN_OFF 0

/* index definitions for PMC_MAPOTN_RX_REG_REG_INTR_EN_REG */
#define PMC_MAPOTN_RX_REG_REG_INTR_EN_REG_INDEX_N_MIN               0
#define PMC_MAPOTN_RX_REG_REG_INTR_EN_REG_INDEX_N_MAX               3
#define PMC_MAPOTN_RX_REG_REG_INTR_EN_REG_INDEX_N_SIZE              4
#define PMC_MAPOTN_RX_REG_REG_INTR_EN_REG_INDEX_N_OFFSET            0x4

/*-----------------------------------------------.
 | Register (0x00000300 + (N) * 0x4) INTR_EN_REG |
 +-----------------------------------------------+
 | bit  8 R/W  MPMO_INTR_EN                      |
 | bit  7 R/W  ODUKSC_INTR_EN                    |
 | bit  6 R/W  TGMP96_INTR_EN                    |
 | bit  5 R/W  TGFPF96_INTR_EN                   |
 | bit  4 R/W  MAPOTN_TX_TOP_INTR_EN             |
 | bit  3 R/W  MPMA_INTR_EN                      |
 | bit  2 R/W  RGMP96_INTR_EN                    |
 | bit  1 R/W  RGFPF96_INTR_EN                   |
 | bit  0 R/W  MAPOTN_RX_TOP_INTR_EN             |
 +----------------------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_INTR_EN_REG_UNUSED_MASK               0xfffffe00
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMO_INTR_EN_MSK          0x00000100
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMO_INTR_EN_OFF          8
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_ODUKSC_INTR_EN_MSK        0x00000080
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_ODUKSC_INTR_EN_OFF        7
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGMP96_INTR_EN_MSK        0x00000040
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGMP96_INTR_EN_OFF        6
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGFPF96_INTR_EN_MSK       0x00000020
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_TGFPF96_INTR_EN_OFF       5
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_TX_TOP_INTR_EN_MSK 0x00000010
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_TX_TOP_INTR_EN_OFF 4
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMA_INTR_EN_MSK          0x00000008
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MPMA_INTR_EN_OFF          3
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGMP96_INTR_EN_MSK        0x00000004
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGMP96_INTR_EN_OFF        2
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGFPF96_INTR_EN_MSK       0x00000002
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_RGFPF96_INTR_EN_OFF       1
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_RX_TOP_INTR_EN_MSK 0x00000001
#define MAPOTN_RX_REG_REG_INTR_EN_REG_BIT_MAPOTN_RX_TOP_INTR_EN_OFF 0

/*----------------------------------.
 | Register 0x00000310 INTR_REG     |
 +----------------------------------+
 | bit  19:16 R  MAPOTN_INTR        |
 | bit  8     R  MPMO_INTR          |
 | bit  7     R  ODUKSC_INTR        |
 | bit  6     R  TGMP96_INTR        |
 | bit  5     R  TGFPF96_INTR       |
 | bit  4     R  MAPOTN_TX_TOP_INTR |
 | bit  3     R  MPMA_INTR          |
 | bit  2     R  RGMP96_INTR        |
 | bit  1     R  RGFPF96_INTR       |
 | bit  0     R  MAPOTN_RX_TOP_INTR |
 +---------------------------------*/
#define PMC_MAPOTN_RX_REG_REG_INTR_REG_UNUSED_MASK            0xfff0fe00
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_MSK        0x000f0000
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_INTR_OFF        16
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMO_INTR_MSK          0x00000100
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMO_INTR_OFF          8
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_ODUKSC_INTR_MSK        0x00000080
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_ODUKSC_INTR_OFF        7
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_TGMP96_INTR_MSK        0x00000040
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_TGMP96_INTR_OFF        6
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_TGFPF96_INTR_MSK       0x00000020
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_TGFPF96_INTR_OFF       5
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_TX_TOP_INTR_MSK 0x00000010
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_TX_TOP_INTR_OFF 4
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMA_INTR_MSK          0x00000008
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MPMA_INTR_OFF          3
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_RGMP96_INTR_MSK        0x00000004
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_RGMP96_INTR_OFF        2
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_RGFPF96_INTR_MSK       0x00000002
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_RGFPF96_INTR_OFF       1
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_RX_TOP_INTR_MSK 0x00000001
#define MAPOTN_RX_REG_REG_INTR_REG_BIT_MAPOTN_RX_TOP_INTR_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_RX_REGS_H */
