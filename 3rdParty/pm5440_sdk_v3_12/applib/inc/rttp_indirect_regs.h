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
 *     rttp_indirect block
 *****************************************************************************/
#ifndef _RTTP_INDIRECT_REGS_H
#define _RTTP_INDIRECT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT     0x00000000
#define PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN   0x00000001
#define PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT 0x00000002
#define PMC_RTTP_INDIRECT_REG_TRACE_CFG                      0x00000003
#define PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE( N )            (0x00000040 + (N) * 0x1)
#define PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE( N )            (0x00000080 + (N) * 0x1)
#define PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE( N )            (0x000000c0 + (N) * 0x1)

/*------------------------------------------------.
 | Register 0x00000000 TRACE_MISMATCH_UNSTBL_STAT |
 +------------------------------------------------+
 | bit  1 R  TIM_V                                |
 | bit  0 R  TIU_V                                |
 +-----------------------------------------------*/
#define PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_UNUSED_MASK 0xfffffffc
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIM_V_MSK   0x00000002
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIM_V_OFF   1
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIU_V_MSK   0x00000001
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIU_V_OFF   0

/*--------------------------------------------------.
 | Register 0x00000001 TRACE_MISMATCH_UNSTBL_INT_EN |
 +--------------------------------------------------+
 | bit  1 R/W  TIM_E                                |
 | bit  0 R/W  TIU_E                                |
 +-------------------------------------------------*/
#define PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_UNUSED_MASK 0xfffffffc
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_MSK   0x00000002
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_OFF   1
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_MSK   0x00000001
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_OFF   0

/*----------------------------------------------------.
 | Register 0x00000002 TRACE_MISMATCH_UNSTBL_INT_STAT |
 +----------------------------------------------------+
 | bit  1 R/W  TIM_I                                  |
 | bit  0 R/W  TIU_I                                  |
 +---------------------------------------------------*/
#define PMC_RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_UNUSED_MASK 0xfffffffc
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK   0x00000002
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_OFF   1
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK   0x00000001
#define RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_OFF   0

/*-------------------------------.
 | Register 0x00000003 TRACE_CFG |
 +-------------------------------+
 | bit  6   R/W  TTMMONEN        |
 | bit  5   R/W  SYNC_CRLF       |
 | bit  4   R/W  ZEROEN          |
 | bit  3   R/W  PER5            |
 | bit  2   R/W  LENGTH16        |
 | bit  1:0 R/W  ALGO            |
 +------------------------------*/
#define PMC_RTTP_INDIRECT_REG_TRACE_CFG_UNUSED_MASK   0xffffff80
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_MSK  0x00000040
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_OFF  6
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_MSK 0x00000020
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_OFF 5
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_MSK    0x00000010
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_OFF    4
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_MSK      0x00000008
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_PER5_OFF      3
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_MSK  0x00000004
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_OFF  2
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_MSK      0x00000003
#define RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_OFF      0

/* index definitions for PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE */
#define PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE_INDEX_N_MIN    0
#define PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE_INDEX_N_MAX    63
#define PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE_INDEX_N_SIZE   64
#define PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE_INDEX_N_OFFSET 0x1

/*--------------------------------------------------.
 | Register (0x00000040 + (N) * 0x1) CAPTURED_TRACE |
 +--------------------------------------------------+
 | bit  7:0 R  CTRACE                               |
 +-------------------------------------------------*/
#define PMC_RTTP_INDIRECT_REG_CAPTURED_TRACE_UNUSED_MASK    0xffffff00
#define RTTP_INDIRECT_REG_CAPTURED_TRACE_BIT_CTRACE_MSK     0x000000ff
#define RTTP_INDIRECT_REG_CAPTURED_TRACE_BIT_CTRACE_OFF     0

/* index definitions for PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE */
#define PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE_INDEX_N_MIN    0
#define PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE_INDEX_N_MAX    63
#define PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE_INDEX_N_SIZE   64
#define PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE_INDEX_N_OFFSET 0x1

/*--------------------------------------------------.
 | Register (0x00000080 + (N) * 0x1) ACCEPTED_TRACE |
 +--------------------------------------------------+
 | bit  7:0 R  ATRACE                               |
 +-------------------------------------------------*/
#define PMC_RTTP_INDIRECT_REG_ACCEPTED_TRACE_UNUSED_MASK    0xffffff00
#define RTTP_INDIRECT_REG_ACCEPTED_TRACE_BIT_ATRACE_MSK     0x000000ff
#define RTTP_INDIRECT_REG_ACCEPTED_TRACE_BIT_ATRACE_OFF     0

/* index definitions for PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE */
#define PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE_INDEX_N_MIN    0
#define PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE_INDEX_N_MAX    63
#define PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE_INDEX_N_SIZE   64
#define PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE_INDEX_N_OFFSET 0x1

/*--------------------------------------------------.
 | Register (0x000000c0 + (N) * 0x1) EXPECTED_TRACE |
 +--------------------------------------------------+
 | bit  7:0 R/W  ETRACE                             |
 +-------------------------------------------------*/
#define PMC_RTTP_INDIRECT_REG_EXPECTED_TRACE_UNUSED_MASK    0xffffff00
#define RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_MSK     0x000000ff
#define RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_OFF     0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTTP_INDIRECT_REGS_H */
