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
 *     shared_ram block
 *****************************************************************************/
#ifndef _SHARED_RAM_REGS_H
#define _SHARED_RAM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SHARED_RAM_REG_MASTER_CONFIG 0x00001000
#define PMC_SHARED_RAM_REG_LOWPWR_CONFIG 0x00001028

/*-----------------------------------.
 | Register 0x00001000 MASTER_CONFIG |
 +-----------------------------------+
 | bit  4:1 R/W  FEC_SELECT          |
 | bit  0   R/W  RESET               |
 +----------------------------------*/
#define PMC_SHARED_RAM_REG_MASTER_CONFIG_UNUSED_MASK    0xffffffe0
#define SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_MSK 0x0000001e
#define SHARED_RAM_REG_MASTER_CONFIG_BIT_FEC_SELECT_OFF 1
#define SHARED_RAM_REG_MASTER_CONFIG_BIT_RESET_MSK      0x00000001
#define SHARED_RAM_REG_MASTER_CONFIG_BIT_RESET_OFF      0

/*-----------------------------------.
 | Register 0x00001028 LOWPWR_CONFIG |
 +-----------------------------------+
 | bit  31:28 R/W  REGION_F3_LOWPWR  |
 | bit  27:24 R/W  REGION_F2_LOWPWR  |
 | bit  23:20 R/W  REGION_F1_LOWPWR  |
 | bit  19:16 R/W  REGION_E_LOWPWR   |
 | bit  15:13 R/W  REGION_D2_LOWPWR  |
 | bit  12:9  R/W  REGION_D1_LOWPWR  |
 | bit  8:6   R/W  REGION_C_LOWPWR   |
 | bit  5:4   R/W  REGION_B_LOWPWR   |
 | bit  3:0   R/W  REGION_A_LOWPWR   |
 +----------------------------------*/
#define PMC_SHARED_RAM_REG_LOWPWR_CONFIG_UNUSED_MASK          0x00000000
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_MSK 0xf0000000
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F3_LOWPWR_OFF 28
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_MSK 0x0f000000
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F2_LOWPWR_OFF 24
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_MSK 0x00f00000
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_F1_LOWPWR_OFF 20
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_MSK  0x000f0000
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_E_LOWPWR_OFF  16
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_MSK 0x0000e000
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D2_LOWPWR_OFF 13
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_MSK 0x00001e00
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_D1_LOWPWR_OFF 9
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_MSK  0x000001c0
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_C_LOWPWR_OFF  6
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_MSK  0x00000030
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_B_LOWPWR_OFF  4
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_MSK  0x0000000f
#define SHARED_RAM_REG_LOWPWR_CONFIG_BIT_REGION_A_LOWPWR_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SHARED_RAM_REGS_H */
