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
 *     mp_mgen block
 *****************************************************************************/
#ifndef _MP_MGEN_REGS_H
#define _MP_MGEN_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MP_MGEN_REG_IPT_ENTRY( N )         (0x00000000 + (N) * 0x8)
#define PMC_MP_MGEN_REG_AMP_CFGS( N )          (0x00000004 + (N) * 0x8)
#define PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR( N ) (0x00000400 + (N) * 0x4)

/* index definitions for PMC_MP_MGEN_REG_IPT_ENTRY */
#define PMC_MP_MGEN_REG_IPT_ENTRY_INDEX_N_MIN    0
#define PMC_MP_MGEN_REG_IPT_ENTRY_INDEX_N_MAX    95
#define PMC_MP_MGEN_REG_IPT_ENTRY_INDEX_N_SIZE   96
#define PMC_MP_MGEN_REG_IPT_ENTRY_INDEX_N_OFFSET 0x8

/*---------------------------------------------.
 | Register (0x00000000 + (N) * 0x8) IPT_ENTRY |
 +---------------------------------------------+
 | bit  16   R/W  IPT_VALID                    |
 | bit  14:8 R/W  IPT_ID                       |
 | bit  6:0  R/W  IPT_VA                       |
 +--------------------------------------------*/
#define PMC_MP_MGEN_REG_IPT_ENTRY_UNUSED_MASK    0xfffe8080
#define MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VALID_MSK  0x00010000
#define MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VALID_OFF  16
#define MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_MSK     0x00007f00
#define MP_MGEN_REG_IPT_ENTRY_BIT_IPT_ID_OFF     8
#define MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_MSK     0x0000007f
#define MP_MGEN_REG_IPT_ENTRY_BIT_IPT_VA_OFF     0

/* index definitions for PMC_MP_MGEN_REG_AMP_CFGS */
#define PMC_MP_MGEN_REG_AMP_CFGS_INDEX_N_MIN        0
#define PMC_MP_MGEN_REG_AMP_CFGS_INDEX_N_MAX        95
#define PMC_MP_MGEN_REG_AMP_CFGS_INDEX_N_SIZE       96
#define PMC_MP_MGEN_REG_AMP_CFGS_INDEX_N_OFFSET     0x8

/*--------------------------------------------.
 | Register (0x00000004 + (N) * 0x8) AMP_CFGS |
 +--------------------------------------------+
 | bit  25:24 R/W  NJO_TYPE                   |
 | bit  21:16 R/W  NJO_POS                    |
 | bit  11:0  R/W  NJO_ODTU_CYCLE             |
 +-------------------------------------------*/
#define PMC_MP_MGEN_REG_AMP_CFGS_UNUSED_MASK        0xfcc0f000
#define MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_MSK       0x03000000
#define MP_MGEN_REG_AMP_CFGS_BIT_NJO_TYPE_OFF       24
#define MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_MSK        0x003f0000
#define MP_MGEN_REG_AMP_CFGS_BIT_NJO_POS_OFF        16
#define MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_MSK 0x00000fff
#define MP_MGEN_REG_AMP_CFGS_BIT_NJO_ODTU_CYCLE_OFF 0

/* index definitions for PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR */
#define PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR_INDEX_N_MIN        0
#define PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR_INDEX_N_MAX        95
#define PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR_INDEX_N_SIZE       96
#define PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR_INDEX_N_OFFSET     0x4

/*-----------------------------------------------------.
 | Register (0x00000400 + (N) * 0x4) DIV_48_M_LAST_TXR |
 +-----------------------------------------------------+
 | bit  21:16 R/W  LAST_TXR_BYTES                      |
 | bit  13:8  R/W  MOD_48_M                            |
 | bit  5:0   R/W  DIV_48_M                            |
 +----------------------------------------------------*/
#define PMC_MP_MGEN_REG_DIV_48_M_LAST_TXR_UNUSED_MASK        0xffc0c0c0
#define MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_MSK 0x003f0000
#define MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_LAST_TXR_BYTES_OFF 16
#define MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_MSK       0x00003f00
#define MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_MOD_48_M_OFF       8
#define MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_MSK       0x0000003f
#define MP_MGEN_REG_DIV_48_M_LAST_TXR_BIT_DIV_48_M_OFF       0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MP_MGEN_REGS_H */
