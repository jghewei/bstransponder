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
 *     dcs block
 *****************************************************************************/
#ifndef _DCS_REGS_H
#define _DCS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_DCS_REG_CONFIG                    0x00060000
#define PMC_DCS_REG_TRANSFER_MODE             0x00060004
#define PMC_DCS_REG_CREDIT_CONFIG             0x00060008
#define PMC_DCS_REG_QUEUE_OFFSET_CONFIG       0x0006000c
#define PMC_DCS_REG_XOFF_MODE                 0x00060010
#define PMC_DCS_REG_PAGE_SWAP_CTRL            0x00060014
#define PMC_DCS_REG_PAGE_SWAP_STAT            0x00060018
#define PMC_DCS_REG_TOP_OF_PAGE_CONFIG        0x0006001c
#define PMC_DCS_REG_STANDBY_PAGE_END_CONFIG   0x00060020
#define PMC_DCS_REG_QUEUE_RESET               0x00060024
#define PMC_DCS_REG_STANDBY_CALENDAR_RAM( N ) (0x00060400 + (N) * 0x4)

/*----------------------------.
 | Register 0x00060000 CONFIG |
 +----------------------------+
 | bit  0 R/W  DCS_ENABLE     |
 +---------------------------*/
#define PMC_DCS_REG_CONFIG_UNUSED_MASK    0xfffcfffe
#define DCS_REG_CONFIG_BIT_DCS_ENABLE_MSK 0x00000001
#define DCS_REG_CONFIG_BIT_DCS_ENABLE_OFF 0

/*---------------------------------------.
 | Register 0x00060004 TRANSFER_MODE     |
 +---------------------------------------+
 | bit  25 R/W  DCS_DISABLE_BURST_CREDIT |
 | bit  24 R/W  DCS_INFINITE_QUANTUM     |
 | bit  16 R/W  DCS_QUANTUM_MODE         |
 | bit  8  R/W  DCS_BURST_MODE           |
 | bit  0  R/W  DCS_XFER_MODE            |
 +--------------------------------------*/
#define PMC_DCS_REG_TRANSFER_MODE_UNUSED_MASK                  0xecfefefe
#define DCS_REG_TRANSFER_MODE_BIT_DCS_DISABLE_BURST_CREDIT_MSK 0x02000000
#define DCS_REG_TRANSFER_MODE_BIT_DCS_DISABLE_BURST_CREDIT_OFF 25
#define DCS_REG_TRANSFER_MODE_BIT_DCS_INFINITE_QUANTUM_MSK     0x01000000
#define DCS_REG_TRANSFER_MODE_BIT_DCS_INFINITE_QUANTUM_OFF     24
#define DCS_REG_TRANSFER_MODE_BIT_DCS_QUANTUM_MODE_MSK         0x00010000
#define DCS_REG_TRANSFER_MODE_BIT_DCS_QUANTUM_MODE_OFF         16
#define DCS_REG_TRANSFER_MODE_BIT_DCS_BURST_MODE_MSK           0x00000100
#define DCS_REG_TRANSFER_MODE_BIT_DCS_BURST_MODE_OFF           8
#define DCS_REG_TRANSFER_MODE_BIT_DCS_XFER_MODE_MSK            0x00000001
#define DCS_REG_TRANSFER_MODE_BIT_DCS_XFER_MODE_OFF            0

/*-----------------------------------.
 | Register 0x00060008 CREDIT_CONFIG |
 +-----------------------------------+
 | bit  24:16 R/W  DCS_BURST_SIZE    |
 | bit  15:0  R/W  DCS_QUANTUM_SIZE  |
 +----------------------------------*/
#define PMC_DCS_REG_CREDIT_CONFIG_UNUSED_MASK          0xfe000000
#define DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_MSK   0x01ff0000
#define DCS_REG_CREDIT_CONFIG_BIT_DCS_BURST_SIZE_OFF   16
#define DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_MSK 0x0000ffff
#define DCS_REG_CREDIT_CONFIG_BIT_DCS_QUANTUM_SIZE_OFF 0

/*-----------------------------------------.
 | Register 0x0006000c QUEUE_OFFSET_CONFIG |
 +-----------------------------------------+
 | bit  7:0 R/W  DCS_QUEUE_OFFSET          |
 +----------------------------------------*/
#define PMC_DCS_REG_QUEUE_OFFSET_CONFIG_UNUSED_MASK          0xffffff00
#define DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_MSK 0x000000ff
#define DCS_REG_QUEUE_OFFSET_CONFIG_BIT_DCS_QUEUE_OFFSET_OFF 0

/*------------------------------------.
 | Register 0x00060010 XOFF_MODE      |
 +------------------------------------+
 | bit  17:16 R/W  DCS_LINK_XOFF_MODE |
 | bit  1:0   R/W  DCS_CH_XOFF_MODE   |
 +-----------------------------------*/
#define PMC_DCS_REG_XOFF_MODE_UNUSED_MASK            0xfffcfffc
#define DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_MSK 0x00030000
#define DCS_REG_XOFF_MODE_BIT_DCS_LINK_XOFF_MODE_OFF 16
#define DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_MSK   0x00000003
#define DCS_REG_XOFF_MODE_BIT_DCS_CH_XOFF_MODE_OFF   0

/*------------------------------------.
 | Register 0x00060014 PAGE_SWAP_CTRL |
 +------------------------------------+
 | bit  0 R/W  DCS_PAGE_SWAP          |
 +-----------------------------------*/
#define PMC_DCS_REG_PAGE_SWAP_CTRL_UNUSED_MASK       0xfffffefe
#define DCS_REG_PAGE_SWAP_CTRL_BIT_DCS_PAGE_SWAP_MSK 0x00000001
#define DCS_REG_PAGE_SWAP_CTRL_BIT_DCS_PAGE_SWAP_OFF 0

/*------------------------------------.
 | Register 0x00060018 PAGE_SWAP_STAT |
 +------------------------------------+
 | bit  0 R  DCS_PAGE_SWAP_STATUS     |
 +-----------------------------------*/
#define PMC_DCS_REG_PAGE_SWAP_STAT_UNUSED_MASK              0xfffffefe
#define DCS_REG_PAGE_SWAP_STAT_BIT_DCS_PAGE_SWAP_STATUS_MSK 0x00000001
#define DCS_REG_PAGE_SWAP_STAT_BIT_DCS_PAGE_SWAP_STATUS_OFF 0

/*----------------------------------------.
 | Register 0x0006001c TOP_OF_PAGE_CONFIG |
 +----------------------------------------+
 | bit  23:16 R/W  DCS_TOP_OF_PAGE_PTR    |
 | bit  9:8   R/W  DCS_TOP_OF_PAGE_MODE   |
 | bit  0     R/W  DCS_TOP_OF_PAGE        |
 +---------------------------------------*/
#define PMC_DCS_REG_TOP_OF_PAGE_CONFIG_UNUSED_MASK              0xfe00fcfe
#define DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_MSK  0x00ff0000
#define DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_PTR_OFF  16
#define DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_MSK 0x00000300
#define DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MODE_OFF 8
#define DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_MSK      0x00000001
#define DCS_REG_TOP_OF_PAGE_CONFIG_BIT_DCS_TOP_OF_PAGE_OFF      0

/*---------------------------------------------.
 | Register 0x00060020 STANDBY_PAGE_END_CONFIG |
 +---------------------------------------------+
 | bit  7:0 R/W  DCS_PAGE_END_PTR              |
 +--------------------------------------------*/
#define PMC_DCS_REG_STANDBY_PAGE_END_CONFIG_UNUSED_MASK          0xffffff00
#define DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_MSK 0x000000ff
#define DCS_REG_STANDBY_PAGE_END_CONFIG_BIT_DCS_PAGE_END_PTR_OFF 0

/*---------------------------------.
 | Register 0x00060024 QUEUE_RESET |
 +---------------------------------+
 | bit  7:0 R/W  DCS_Q_RESET       |
 +--------------------------------*/
#define PMC_DCS_REG_QUEUE_RESET_UNUSED_MASK     0xffffff00
#define DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_MSK 0x000000ff
#define DCS_REG_QUEUE_RESET_BIT_DCS_Q_RESET_OFF 0

/* index definitions for PMC_DCS_REG_STANDBY_CALENDAR_RAM */
#define PMC_DCS_REG_STANDBY_CALENDAR_RAM_INDEX_N_MIN          0
#define PMC_DCS_REG_STANDBY_CALENDAR_RAM_INDEX_N_MAX          95
#define PMC_DCS_REG_STANDBY_CALENDAR_RAM_INDEX_N_SIZE         96
#define PMC_DCS_REG_STANDBY_CALENDAR_RAM_INDEX_N_OFFSET       0x4

/*--------------------------------------------------------.
 | Register (0x00060400 + (N) * 0x4) STANDBY_CALENDAR_RAM |
 +--------------------------------------------------------+
 | bit  25:24 R/W  C_RAM_SF1                              |
 | bit  23:16 R/W  C_RAM_SF2                              |
 | bit  15:8  R/W  C_RAM_CHANNEL                          |
 | bit  6:0   R/W  C_RAM_IDENTIFIER                       |
 +-------------------------------------------------------*/
#define PMC_DCS_REG_STANDBY_CALENDAR_RAM_UNUSED_MASK          0xfc000080
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_MSK        0x03000000
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF1_OFF        24
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_MSK        0x00ff0000
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_SF2_OFF        16
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_MSK    0x0000ff00
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_CHANNEL_OFF    8
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_MSK 0x0000007f
#define DCS_REG_STANDBY_CALENDAR_RAM_BIT_C_RAM_IDENTIFIER_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DCS_REGS_H */
