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
 *     shpi block
 *****************************************************************************/
#ifndef _SHPI_REGS_H
#define _SHPI_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SHPI_REG_IND_ADR  0x00001840
#define PMC_SHPI_REG_IND_DATA 0x00001844

/*-----------------------------.
 | Register 0x00001840 IND_ADR |
 +-----------------------------+
 | bit  15  R    BUSY          |
 | bit  14  R/W  RWB           |
 | bit  8:6 R/W  IADDR         |
 +----------------------------*/
#define PMC_SHPI_REG_IND_ADR_UNUSED_MASK 0xffff3e00
#define SHPI_REG_IND_ADR_BIT_BUSY_MSK    0x00008000
#define SHPI_REG_IND_ADR_BIT_BUSY_OFF    15
#define SHPI_REG_IND_ADR_BIT_RWB_MSK     0x00004000
#define SHPI_REG_IND_ADR_BIT_RWB_OFF     14
#define SHPI_REG_IND_ADR_BIT_IADDR_MSK   0x000001c0
#define SHPI_REG_IND_ADR_BIT_IADDR_OFF   6

/*------------------------------.
 | Register 0x00001844 IND_DATA |
 +------------------------------+
 | bit  15:0 R/W  DATA          |
 +-----------------------------*/
#define PMC_SHPI_REG_IND_DATA_UNUSED_MASK 0xffff0000
#define SHPI_REG_IND_DATA_BIT_DATA_MSK    0x0000ffff
#define SHPI_REG_IND_DATA_BIT_DATA_OFF    0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SHPI_REGS_H */
