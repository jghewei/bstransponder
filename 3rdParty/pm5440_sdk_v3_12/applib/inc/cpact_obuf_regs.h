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
 *     cpact_obuf block
 *****************************************************************************/
#ifndef _CPACT_OBUF_REGS_H
#define _CPACT_OBUF_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG( N ) (0x00000000 + (N) * 0x10)
#define PMC_CPACT_OBUF_REG_SOMF_CFG( N )       (0x00000004 + (N) * 0x10)
#define PMC_CPACT_OBUF_REG_EOMF_CFG( N )       (0x00000008 + (N) * 0x10)

/* index definitions for PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG */
#define PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG_INDEX_N_MIN     0
#define PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG_INDEX_N_MAX     95
#define PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG_INDEX_N_SIZE    96
#define PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG_INDEX_N_OFFSET  0x10

/*---------------------------------------------------.
 | Register (0x00000000 + (N) * 0x10) CPACT_CHNL_CFG |
 +---------------------------------------------------+
 | bit  2:0 R/W  CH_MAP_TYPE                         |
 +--------------------------------------------------*/
#define PMC_CPACT_OBUF_REG_CPACT_CHNL_CFG_UNUSED_MASK     0xfffffff8
#define CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_MSK 0x00000007
#define CPACT_OBUF_REG_CPACT_CHNL_CFG_BIT_CH_MAP_TYPE_OFF 0

/* index definitions for PMC_CPACT_OBUF_REG_SOMF_CFG */
#define PMC_CPACT_OBUF_REG_SOMF_CFG_INDEX_N_MIN       0
#define PMC_CPACT_OBUF_REG_SOMF_CFG_INDEX_N_MAX       95
#define PMC_CPACT_OBUF_REG_SOMF_CFG_INDEX_N_SIZE      96
#define PMC_CPACT_OBUF_REG_SOMF_CFG_INDEX_N_OFFSET    0x10

/*---------------------------------------------.
 | Register (0x00000004 + (N) * 0x10) SOMF_CFG |
 +---------------------------------------------+
 | bit  21:16 R/W  SOMF_FIRST_DT               |
 | bit  14:0  R/W  SOMF_CYCLE                  |
 +--------------------------------------------*/
#define PMC_CPACT_OBUF_REG_SOMF_CFG_UNUSED_MASK       0xffc08000
#define CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_MSK 0x003f0000
#define CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_FIRST_DT_OFF 16
#define CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_MSK    0x00007fff
#define CPACT_OBUF_REG_SOMF_CFG_BIT_SOMF_CYCLE_OFF    0

/* index definitions for PMC_CPACT_OBUF_REG_EOMF_CFG */
#define PMC_CPACT_OBUF_REG_EOMF_CFG_INDEX_N_MIN      0
#define PMC_CPACT_OBUF_REG_EOMF_CFG_INDEX_N_MAX      95
#define PMC_CPACT_OBUF_REG_EOMF_CFG_INDEX_N_SIZE     96
#define PMC_CPACT_OBUF_REG_EOMF_CFG_INDEX_N_OFFSET   0x10

/*---------------------------------------------.
 | Register (0x00000008 + (N) * 0x10) EOMF_CFG |
 +---------------------------------------------+
 | bit  21:16 R/W  EOMF_LAST_DT                |
 | bit  14:0  R/W  EOMF_CYCLE                  |
 +--------------------------------------------*/
#define PMC_CPACT_OBUF_REG_EOMF_CFG_UNUSED_MASK      0xffc08000
#define CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_MSK 0x003f0000
#define CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_LAST_DT_OFF 16
#define CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_MSK   0x00007fff
#define CPACT_OBUF_REG_EOMF_CFG_BIT_EOMF_CYCLE_OFF   0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CPACT_OBUF_REGS_H */
