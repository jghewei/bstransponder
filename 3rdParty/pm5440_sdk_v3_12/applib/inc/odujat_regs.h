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
 *     odujat block
 *****************************************************************************/
#ifndef _ODUJAT_REGS_H
#define _ODUJAT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ODUJAT_CORE_REG_CFG             0x00000000
#define PMC_ODUJAT_CORE_REG_ERR_INT_EN      0x00000004
#define PMC_ODUJAT_CORE_REG_ERR_INTS        0x00000008
#define PMC_ODUJAT_CORE_REG_ERR_CHAN        0x0000000c
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_0( N ) (0x00000100 + (N) * 0x10)
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_1( N ) (0x00000104 + (N) * 0x10)
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_2( N ) (0x00000108 + (N) * 0x10)

/*-----------------------------.
 | Register 0x00000000 CFG     |
 +-----------------------------+
 | bit  17:16 R/W  FIFO_LIMIT  |
 | bit  9     R/W  RAM_LS      |
 | bit  8     R/W  PG_OVERRIDE |
 +----------------------------*/
#define PMC_ODUJAT_CORE_REG_CFG_UNUSED_MASK     0xfffcfcfe
#define ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_MSK  0x00030000
#define ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF  16
#define ODUJAT_CORE_REG_CFG_BIT_RAM_LS_MSK      0x00000200
#define ODUJAT_CORE_REG_CFG_BIT_RAM_LS_OFF      9
#define ODUJAT_CORE_REG_CFG_BIT_PG_OVERRIDE_MSK 0x00000100
#define ODUJAT_CORE_REG_CFG_BIT_PG_OVERRIDE_OFF 8

/*--------------------------------.
 | Register 0x00000004 ERR_INT_EN |
 +--------------------------------+
 | bit  2 R/W  DQ_LIMIT_E         |
 | bit  1 R/W  GAIN_UNDF_E        |
 | bit  0 R/W  GAIN_OVF_E         |
 +-------------------------------*/
#define PMC_ODUJAT_CORE_REG_ERR_INT_EN_UNUSED_MASK     0xfffffff8
#define ODUJAT_CORE_REG_ERR_INT_EN_BIT_DQ_LIMIT_E_MSK  0x00000004
#define ODUJAT_CORE_REG_ERR_INT_EN_BIT_DQ_LIMIT_E_OFF  2
#define ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_UNDF_E_MSK 0x00000002
#define ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_UNDF_E_OFF 1
#define ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_OVF_E_MSK  0x00000001
#define ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_OVF_E_OFF  0

/*------------------------------.
 | Register 0x00000008 ERR_INTS |
 +------------------------------+
 | bit  2 R/W  DQ_LIMIT_I       |
 | bit  1 R/W  GAIN_UNDF_I      |
 | bit  0 R/W  GAIN_OVF_I       |
 +-----------------------------*/
#define PMC_ODUJAT_CORE_REG_ERR_INTS_UNUSED_MASK     0xfffffff8
#define ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_MSK  0x00000004
#define ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_OFF  2
#define ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_MSK 0x00000002
#define ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_OFF 1
#define ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_MSK  0x00000001
#define ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_OFF  0

/*------------------------------.
 | Register 0x0000000c ERR_CHAN |
 +------------------------------+
 | bit  14:8 R  LIMIT_ERR_CHAN  |
 | bit  6:0  R  GAIN_ERR_CHAN   |
 +-----------------------------*/
#define PMC_ODUJAT_CORE_REG_ERR_CHAN_UNUSED_MASK        0xffff8080
#define ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_MSK 0x00007f00
#define ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_OFF 8
#define ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_MSK  0x0000007f
#define ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_OFF  0

/* index definitions for PMC_ODUJAT_CORE_REG_CHAN_CFG_0 */
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_0_INDEX_N_MIN           0
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_0_INDEX_N_MAX           95
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_0_INDEX_N_SIZE          96
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_0_INDEX_N_OFFSET        0x10

/*-----------------------------------------------.
 | Register (0x00000100 + (N) * 0x10) CHAN_CFG_0 |
 +-----------------------------------------------+
 | bit  31    R/W  JAT_ENABLE                    |
 | bit  24    R/W  DQ_AT_LIMIT                   |
 | bit  20:18 R/W  CND_SELECT                    |
 | bit  17    R/W  ADD_RESIDUE                   |
 | bit  16    R/W  ADD_CND                       |
 | bit  11:8  R/W  LOOP_ALPHA_SELECT             |
 | bit  4:0   R/W  CONV_GAIN_SELECT              |
 +----------------------------------------------*/
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_0_UNUSED_MASK           0x7ee0f0e0
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_JAT_ENABLE_MSK        0x80000000
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_JAT_ENABLE_OFF        31
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_DQ_AT_LIMIT_MSK       0x01000000
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_DQ_AT_LIMIT_OFF       24
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_MSK        0x001c0000
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF        18
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_RESIDUE_MSK       0x00020000
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_RESIDUE_OFF       17
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_CND_MSK           0x00010000
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_CND_OFF           16
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_MSK 0x00000f00
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF 8
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_MSK  0x0000001f
#define ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF  0

/* index definitions for PMC_ODUJAT_CORE_REG_CHAN_CFG_1 */
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_1_INDEX_N_MIN      0
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_1_INDEX_N_MAX      95
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_1_INDEX_N_SIZE     96
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_1_INDEX_N_OFFSET   0x10

/*-----------------------------------------------.
 | Register (0x00000104 + (N) * 0x10) CHAN_CFG_1 |
 +-----------------------------------------------+
 | bit  25:0 R/W  MC_NUMERATOR                   |
 +----------------------------------------------*/
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_1_UNUSED_MASK      0xfc000000
#define ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_MSK 0x03ffffff
#define ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF 0

/* index definitions for PMC_ODUJAT_CORE_REG_CHAN_CFG_2 */
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_2_INDEX_N_MIN          0
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_2_INDEX_N_MAX          95
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_2_INDEX_N_SIZE         96
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_2_INDEX_N_OFFSET       0x10

/*-----------------------------------------------.
 | Register (0x00000108 + (N) * 0x10) CHAN_CFG_2 |
 +-----------------------------------------------+
 | bit  24:16 R/W  FIFO_LEVEL_LIMIT              |
 | bit  9:0   R/W  CND_DELAY                     |
 +----------------------------------------------*/
#define PMC_ODUJAT_CORE_REG_CHAN_CFG_2_UNUSED_MASK          0xfe00fc00
#define ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_MSK 0x01ff0000
#define ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF 16
#define ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_MSK        0x000003ff
#define ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF        0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODUJAT_REGS_H */
