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
 *     shpi_indirect block
 *****************************************************************************/
#ifndef _SHPI_INDIRECT_REGS_H
#define _SHPI_INDIRECT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SHPI_INDIRECT_REG_IND_INT_VAL                           0x00000000
#define PMC_SHPI_INDIRECT_REG_IND_INT_STAT                          0x00000002
#define PMC_SHPI_INDIRECT_REG_CFG                                   0x00000003
#define PMC_SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT 0x00000004
#define PMC_SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT 0x00000005
#define PMC_SHPI_INDIRECT_REG_POINTER_VAL                           0x00000006

/*---------------------------------.
 | Register 0x00000000 IND_INT_VAL |
 +---------------------------------+
 | bit  5 R  PAISCV                |
 | bit  4 R  PLOPCV                |
 | bit  3 R  PAISV                 |
 | bit  2 R  PLOPV                 |
 +--------------------------------*/
#define PMC_SHPI_INDIRECT_REG_IND_INT_VAL_UNUSED_MASK 0xffff8143
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISCV_MSK  0x00000020
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISCV_OFF  5
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPCV_MSK  0x00000010
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPCV_OFF  4
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISV_MSK   0x00000008
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PAISV_OFF   3
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPV_MSK   0x00000004
#define SHPI_INDIRECT_REG_IND_INT_VAL_BIT_PLOPV_OFF   2

/*----------------------------------.
 | Register 0x00000002 IND_INT_STAT |
 +----------------------------------+
 | bit  5 R/W  PAISCI               |
 | bit  4 R/W  PLOPCI               |
 | bit  3 R/W  PAISI                |
 | bit  2 R/W  PLOPI                |
 +---------------------------------*/
#define PMC_SHPI_INDIRECT_REG_IND_INT_STAT_UNUSED_MASK 0xffff7e40
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_MSK  0x00000020
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISCI_OFF  5
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_MSK  0x00000010
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPCI_OFF  4
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_MSK   0x00000008
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PAISI_OFF   3
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_MSK   0x00000004
#define SHPI_INDIRECT_REG_IND_INT_STAT_BIT_PLOPI_OFF   2

/*-------------------------.
 | Register 0x00000003 CFG |
 +-------------------------+
 | bit  4 R/W  NDFCNT      |
 | bit  0 R/W  SSEN        |
 +------------------------*/
#define PMC_SHPI_INDIRECT_REG_CFG_UNUSED_MASK 0xffff4180
#define SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_MSK  0x00000010
#define SHPI_INDIRECT_REG_CFG_BIT_NDFCNT_OFF  4
#define SHPI_INDIRECT_REG_CFG_BIT_SSEN_MSK    0x00000001
#define SHPI_INDIRECT_REG_CFG_BIT_SSEN_OFF    0

/*-----------------------------------------------------------.
 | Register 0x00000004 PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT |
 +-----------------------------------------------------------+
 | bit  12:0 R  PNJE                                         |
 +----------------------------------------------------------*/
#define PMC_SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_UNUSED_MASK 0xffffe000
#define SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_BIT_PNJE_MSK    0x00001fff
#define SHPI_INDIRECT_REG_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_BIT_PNJE_OFF    0

/*-----------------------------------------------------------.
 | Register 0x00000005 PATH_POSITIVE_JUSTIFICATION_EVENT_CNT |
 +-----------------------------------------------------------+
 | bit  12:0 R  PPJE                                         |
 +----------------------------------------------------------*/
#define PMC_SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_UNUSED_MASK 0xffffe000
#define SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_BIT_PPJE_MSK    0x00001fff
#define SHPI_INDIRECT_REG_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_BIT_PPJE_OFF    0

/*---------------------------------.
 | Register 0x00000006 POINTER_VAL |
 +---------------------------------+
 | bit  15:12 R  NDF               |
 | bit  11:10 R  SSV               |
 | bit  9:0   R  PTRV              |
 +--------------------------------*/
#define PMC_SHPI_INDIRECT_REG_POINTER_VAL_UNUSED_MASK 0xffff0000
#define SHPI_INDIRECT_REG_POINTER_VAL_BIT_NDF_MSK     0x0000f000
#define SHPI_INDIRECT_REG_POINTER_VAL_BIT_NDF_OFF     12
#define SHPI_INDIRECT_REG_POINTER_VAL_BIT_SSV_MSK     0x00000c00
#define SHPI_INDIRECT_REG_POINTER_VAL_BIT_SSV_OFF     10
#define SHPI_INDIRECT_REG_POINTER_VAL_BIT_PTRV_MSK    0x000003ff
#define SHPI_INDIRECT_REG_POINTER_VAL_BIT_PTRV_OFF    0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SHPI_INDIRECT_REGS_H */
