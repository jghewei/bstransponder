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
 *     master_dpi block
 *****************************************************************************/
#ifndef _MASTER_DPI_REGS_H
#define _MASTER_DPI_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MASTER_DPI_REG_ZONE_CONFIG           0x00020000
#define PMC_MASTER_DPI_REG_CH_CONFIG( N )        (0x00020400 + (N) * 0x4)
#define PMC_MASTER_DPI_REG_ZONE_0_1( N )         (0x00020800 + (N) * 0x4)
#define PMC_MASTER_DPI_REG_ZONE_1_2( N )         (0x00020c00 + (N) * 0x4)
#define PMC_MASTER_DPI_REG_ZONE_2_3( N )         (0x00021000 + (N) * 0x4)
#define PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG( N ) (0x00021c00 + (N) * 0x4)

/*---------------------------------.
 | Register 0x00020000 ZONE_CONFIG |
 +---------------------------------+
 | bit  17 R/W  ZONE_EN            |
 +--------------------------------*/
#define PMC_MASTER_DPI_REG_ZONE_CONFIG_UNUSED_MASK 0xfcfdffff
#define MASTER_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_MSK 0x00020000
#define MASTER_DPI_REG_ZONE_CONFIG_BIT_ZONE_EN_OFF 17

/* index definitions for PMC_MASTER_DPI_REG_CH_CONFIG */
#define PMC_MASTER_DPI_REG_CH_CONFIG_INDEX_N_MIN       0
#define PMC_MASTER_DPI_REG_CH_CONFIG_INDEX_N_MAX       255
#define PMC_MASTER_DPI_REG_CH_CONFIG_INDEX_N_SIZE      256
#define PMC_MASTER_DPI_REG_CH_CONFIG_INDEX_N_OFFSET    0x4

/*---------------------------------------------.
 | Register (0x00020400 + (N) * 0x4) CH_CONFIG |
 +---------------------------------------------+
 | bit  16  R/W  EOP_HOLE_SIZE                 |
 | bit  6:0 R/W  SOP_HOLE_SIZE                 |
 +--------------------------------------------*/
#define PMC_MASTER_DPI_REG_CH_CONFIG_UNUSED_MASK       0xfffeff80
#define MASTER_DPI_REG_CH_CONFIG_BIT_EOP_HOLE_SIZE_MSK 0x00010000
#define MASTER_DPI_REG_CH_CONFIG_BIT_EOP_HOLE_SIZE_OFF 16
#define MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_MSK 0x0000007f
#define MASTER_DPI_REG_CH_CONFIG_BIT_SOP_HOLE_SIZE_OFF 0

/* index definitions for PMC_MASTER_DPI_REG_ZONE_0_1 */
#define PMC_MASTER_DPI_REG_ZONE_0_1_INDEX_N_MIN      0
#define PMC_MASTER_DPI_REG_ZONE_0_1_INDEX_N_MAX      255
#define PMC_MASTER_DPI_REG_ZONE_0_1_INDEX_N_SIZE     256
#define PMC_MASTER_DPI_REG_ZONE_0_1_INDEX_N_OFFSET   0x4

/*--------------------------------------------.
 | Register (0x00020800 + (N) * 0x4) ZONE_0_1 |
 +--------------------------------------------+
 | bit  16:0 R/W  ZONE_THR_0_1                |
 +-------------------------------------------*/
#define PMC_MASTER_DPI_REG_ZONE_0_1_UNUSED_MASK      0xfffe0000
#define MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_MSK 0x0001ffff
#define MASTER_DPI_REG_ZONE_0_1_BIT_ZONE_THR_0_1_OFF 0

/* index definitions for PMC_MASTER_DPI_REG_ZONE_1_2 */
#define PMC_MASTER_DPI_REG_ZONE_1_2_INDEX_N_MIN      0
#define PMC_MASTER_DPI_REG_ZONE_1_2_INDEX_N_MAX      255
#define PMC_MASTER_DPI_REG_ZONE_1_2_INDEX_N_SIZE     256
#define PMC_MASTER_DPI_REG_ZONE_1_2_INDEX_N_OFFSET   0x4

/*--------------------------------------------.
 | Register (0x00020c00 + (N) * 0x4) ZONE_1_2 |
 +--------------------------------------------+
 | bit  16:0 R/W  ZONE_THR_1_2                |
 +-------------------------------------------*/
#define PMC_MASTER_DPI_REG_ZONE_1_2_UNUSED_MASK      0xfffe0000
#define MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_MSK 0x0001ffff
#define MASTER_DPI_REG_ZONE_1_2_BIT_ZONE_THR_1_2_OFF 0

/* index definitions for PMC_MASTER_DPI_REG_ZONE_2_3 */
#define PMC_MASTER_DPI_REG_ZONE_2_3_INDEX_N_MIN      0
#define PMC_MASTER_DPI_REG_ZONE_2_3_INDEX_N_MAX      255
#define PMC_MASTER_DPI_REG_ZONE_2_3_INDEX_N_SIZE     256
#define PMC_MASTER_DPI_REG_ZONE_2_3_INDEX_N_OFFSET   0x4

/*--------------------------------------------.
 | Register (0x00021000 + (N) * 0x4) ZONE_2_3 |
 +--------------------------------------------+
 | bit  16:0 R/W  ZONE_THR_2_3                |
 +-------------------------------------------*/
#define PMC_MASTER_DPI_REG_ZONE_2_3_UNUSED_MASK      0xfffe0000
#define MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_MSK 0x0001ffff
#define MASTER_DPI_REG_ZONE_2_3_BIT_ZONE_THR_2_3_OFF 0

/* index definitions for PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG */
#define PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_MIN       0
#define PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_MAX       255
#define PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_SIZE      256
#define PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG_INDEX_N_OFFSET    0x4

/*----------------------------------------------------.
 | Register (0x00021c00 + (N) * 0x4) ZONE_OVRD_CONFIG |
 +----------------------------------------------------+
 | bit  25 R/W  ZONE_OVRD_EN                          |
 | bit  24 R/W  ZONE_OVRD_CFG                         |
 +---------------------------------------------------*/
#define PMC_MASTER_DPI_REG_ZONE_OVRD_CONFIG_UNUSED_MASK       0xfcffffff
#define MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_MSK  0x02000000
#define MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_EN_OFF  25
#define MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_MSK 0x01000000
#define MASTER_DPI_REG_ZONE_OVRD_CONFIG_BIT_ZONE_OVRD_CFG_OFF 24

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MASTER_DPI_REGS_H */
