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
 *     pmpm block
 *****************************************************************************/
#ifndef _PMPM_REGS_H
#define _PMPM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PMPM_REG_RAM_POWER_CONFIG( N ) (0x00068100 + (N) * 0x04)

/* index definitions for PMC_PMPM_REG_RAM_POWER_CONFIG */
#define PMC_PMPM_REG_RAM_POWER_CONFIG_INDEX_N_MIN            0
#define PMC_PMPM_REG_RAM_POWER_CONFIG_INDEX_N_MAX            31
#define PMC_PMPM_REG_RAM_POWER_CONFIG_INDEX_N_SIZE           32
#define PMC_PMPM_REG_RAM_POWER_CONFIG_INDEX_N_OFFSET         0x04

/*-----------------------------------------------------.
 | Register (0x00068100 + (N) * 0x04) RAM_POWER_CONFIG |
 +-----------------------------------------------------+
 | bit  17:16 R/W  PWR_MODE                            |
 | bit  9     R/W  DYNAMIC_ADDR_LS_EN                  |
 | bit  8     R/W  DYNAMIC_LS_EN                       |
 +----------------------------------------------------*/
#define PMC_PMPM_REG_RAM_POWER_CONFIG_UNUSED_MASK            0xfffcfcfe
#define PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_MSK           0x00030000
#define PMPM_REG_RAM_POWER_CONFIG_BIT_PWR_MODE_OFF           16
#define PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_ADDR_LS_EN_MSK 0x00000200
#define PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_ADDR_LS_EN_OFF 9
#define PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_LS_EN_MSK      0x00000100
#define PMPM_REG_RAM_POWER_CONFIG_BIT_DYNAMIC_LS_EN_OFF      8

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PMPM_REGS_H */
