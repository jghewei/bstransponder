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
 *     sysotn_sfis_tx block
 *****************************************************************************/
#ifndef _SYSOTN_SFIS_TX_REGS_H
#define _SYSOTN_SFIS_TX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SYSOTN_SFIS_TX_REG_TX_CFG        0x00004080
#define PMC_SYSOTN_SFIS_TX_REG_TX_INT        0x00004084
#define PMC_SYSOTN_SFIS_TX_REG_TX_INT_EN     0x00004088
#define PMC_SYSOTN_SFIS_TX_REG_TX_BUFFER_CFG 0x0000408c

/*----------------------------.
 | Register 0x00004080 TX_CFG |
 +----------------------------+
 | bit  5   R/W  TX_ODD_INV   |
 | bit  4   R/W  TX_SW_RST    |
 | bit  3:0 R/W  TX_NUM_LANES |
 +---------------------------*/
#define PMC_SYSOTN_SFIS_TX_REG_TX_CFG_UNUSED_MASK      0xffffff80
#define SYSOTN_SFIS_TX_REG_TX_CFG_BIT_TX_ODD_INV_MSK   0x00000020
#define SYSOTN_SFIS_TX_REG_TX_CFG_BIT_TX_ODD_INV_OFF   5
#define SYSOTN_SFIS_TX_REG_TX_CFG_BIT_TX_SW_RST_MSK    0x00000010
#define SYSOTN_SFIS_TX_REG_TX_CFG_BIT_TX_SW_RST_OFF    4
#define SYSOTN_SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_MSK 0x0000000f
#define SYSOTN_SFIS_TX_REG_TX_CFG_BIT_TX_NUM_LANES_OFF 0

/*----------------------------.
 | Register 0x00004084 TX_INT |
 +----------------------------+
 | bit  1 R/W  TXOVR_I        |
 | bit  0 R/W  TXUDR_I        |
 +---------------------------*/
#define PMC_SYSOTN_SFIS_TX_REG_TX_INT_UNUSED_MASK 0xffffffc0
#define SYSOTN_SFIS_TX_REG_TX_INT_BIT_TXOVR_I_MSK 0x00000002
#define SYSOTN_SFIS_TX_REG_TX_INT_BIT_TXOVR_I_OFF 1
#define SYSOTN_SFIS_TX_REG_TX_INT_BIT_TXUDR_I_MSK 0x00000001
#define SYSOTN_SFIS_TX_REG_TX_INT_BIT_TXUDR_I_OFF 0

/*-------------------------------.
 | Register 0x00004088 TX_INT_EN |
 +-------------------------------+
 | bit  1 R/W  TXOVR_E           |
 | bit  0 R/W  TXUDR_E           |
 +------------------------------*/
#define PMC_SYSOTN_SFIS_TX_REG_TX_INT_EN_UNUSED_MASK 0xffffffc0
#define SYSOTN_SFIS_TX_REG_TX_INT_EN_BIT_TXOVR_E_MSK 0x00000002
#define SYSOTN_SFIS_TX_REG_TX_INT_EN_BIT_TXOVR_E_OFF 1
#define SYSOTN_SFIS_TX_REG_TX_INT_EN_BIT_TXUDR_E_MSK 0x00000001
#define SYSOTN_SFIS_TX_REG_TX_INT_EN_BIT_TXUDR_E_OFF 0

/*-----------------------------------.
 | Register 0x0000408c TX_BUFFER_CFG |
 +-----------------------------------+
 | bit  7 R/W  TX_FIFO_CENTER        |
 | bit  6 R/W  TX_FIFO_AUTO_CENTER   |
 +----------------------------------*/
#define PMC_SYSOTN_SFIS_TX_REG_TX_BUFFER_CFG_UNUSED_MASK             0xffc0803f
#define SYSOTN_SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_CENTER_MSK      0x00000080
#define SYSOTN_SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_CENTER_OFF      7
#define SYSOTN_SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_AUTO_CENTER_MSK 0x00000040
#define SYSOTN_SFIS_TX_REG_TX_BUFFER_CFG_BIT_TX_FIFO_AUTO_CENTER_OFF 6

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYSOTN_SFIS_TX_REGS_H */
