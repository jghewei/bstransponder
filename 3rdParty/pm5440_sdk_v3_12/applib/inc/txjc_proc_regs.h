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
 *     txjc_proc block
 *****************************************************************************/
#ifndef _TXJC_PROC_REGS_H
#define _TXJC_PROC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG( N )  (0x00000000 + (N) * 0x10)
#define PMC_TXJC_PROC_REG_GOLDEN_CN_REG( N )       (0x00000004 + (N) * 0x10)
#define PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR( N )   (0x00000008 + (N) * 0x10)
#define PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR( N ) (0x0000000c + (N) * 0x10)

/* index definitions for PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG */
#define PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG_INDEX_N_MIN            0
#define PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG_INDEX_N_MAX            95
#define PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG_INDEX_N_SIZE           96
#define PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG_INDEX_N_OFFSET         0x10

/*-------------------------------------------------------.
 | Register (0x00000000 + (N) * 0x10) CHANNEL_COMMON_CFG |
 +-------------------------------------------------------+
 | bit  5   R/W  SPECIAL_MODE_2_SEL                      |
 | bit  4   R/W  PREV_CN_SEL                             |
 | bit  3:2 R/W  PJO_CNT                                 |
 | bit  1:0 R/W  NJO_CNT                                 |
 +------------------------------------------------------*/
#define PMC_TXJC_PROC_REG_CHANNEL_COMMON_CFG_UNUSED_MASK            0xffffffc0
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_SPECIAL_MODE_2_SEL_MSK 0x00000020
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_SPECIAL_MODE_2_SEL_OFF 5
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PREV_CN_SEL_MSK        0x00000010
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PREV_CN_SEL_OFF        4
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_MSK            0x0000000c
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_PJO_CNT_OFF            2
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_MSK            0x00000003
#define TXJC_PROC_REG_CHANNEL_COMMON_CFG_BIT_NJO_CNT_OFF            0

/* index definitions for PMC_TXJC_PROC_REG_GOLDEN_CN_REG */
#define PMC_TXJC_PROC_REG_GOLDEN_CN_REG_INDEX_N_MIN    0
#define PMC_TXJC_PROC_REG_GOLDEN_CN_REG_INDEX_N_MAX    95
#define PMC_TXJC_PROC_REG_GOLDEN_CN_REG_INDEX_N_SIZE   96
#define PMC_TXJC_PROC_REG_GOLDEN_CN_REG_INDEX_N_OFFSET 0x10

/*--------------------------------------------------.
 | Register (0x00000004 + (N) * 0x10) GOLDEN_CN_REG |
 +--------------------------------------------------+
 | bit  23:0 R/W  CN_GOLDEN                         |
 +-------------------------------------------------*/
#define PMC_TXJC_PROC_REG_GOLDEN_CN_REG_UNUSED_MASK    0xff000000
#define TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_MSK  0x00ffffff
#define TXJC_PROC_REG_GOLDEN_CN_REG_BIT_CN_GOLDEN_OFF  0

/* index definitions for PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR */
#define PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR_INDEX_N_MIN             0
#define PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR_INDEX_N_MAX             95
#define PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR_INDEX_N_SIZE            96
#define PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR_INDEX_N_OFFSET          0x10

/*------------------------------------------------------.
 | Register (0x00000008 + (N) * 0x10) CN_RAMP_NUMERATOR |
 +------------------------------------------------------+
 | bit  23:0 R/W  CN_GOLDEN_NUMERATOR                   |
 +-----------------------------------------------------*/
#define PMC_TXJC_PROC_REG_CN_RAMP_NUMERATOR_UNUSED_MASK             0xff000000
#define TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_MSK 0x00ffffff
#define TXJC_PROC_REG_CN_RAMP_NUMERATOR_BIT_CN_GOLDEN_NUMERATOR_OFF 0

/* index definitions for PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR */
#define PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR_INDEX_N_MIN               0
#define PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR_INDEX_N_MAX               95
#define PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR_INDEX_N_SIZE              96
#define PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR_INDEX_N_OFFSET            0x10

/*--------------------------------------------------------.
 | Register (0x0000000c + (N) * 0x10) CN_RAMP_DENOMINATOR |
 +--------------------------------------------------------+
 | bit  23:0 R/W  CN_GOLDEN_DENOMINATOR                   |
 +-------------------------------------------------------*/
#define PMC_TXJC_PROC_REG_CN_RAMP_DENOMINATOR_UNUSED_MASK               0xff000000
#define TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_MSK 0x00ffffff
#define TXJC_PROC_REG_CN_RAMP_DENOMINATOR_BIT_CN_GOLDEN_DENOMINATOR_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TXJC_PROC_REGS_H */
