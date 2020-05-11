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
 *     prgm block
 *****************************************************************************/
#ifndef _PRGM_REGS_H
#define _PRGM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PRGM_REG_IND_ADR                0x00001440
#define PMC_PRGM_REG_IND_DATA               0x00001444
#define PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG  0x00001448
#define PMC_PRGM_REG_MON_PAYLOAD_CFG        0x0000144c
#define PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT  0x00001450
#define PMC_PRGM_REG_MON_BYTE_ERR_INT_EN    0x00001454
#define PMC_PRGM_REG_MON_SYNC_INT_STAT      0x00001464
#define PMC_PRGM_REG_MON_SYNC_INT_EN        0x00001468
#define PMC_PRGM_REG_MON_SYNC_STAT          0x0000146c
#define PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG 0x00001470

/*-----------------------------.
 | Register 0x00001440 IND_ADR |
 +-----------------------------+
 | bit  15  R    BUSY          |
 | bit  14  R/W  RWB           |
 | bit  9:6 R/W  IADDR         |
 | bit  3:0 R/W  PATH          |
 +----------------------------*/
#define PMC_PRGM_REG_IND_ADR_UNUSED_MASK 0xffff3c30
#define PRGM_REG_IND_ADR_BIT_BUSY_MSK    0x00008000
#define PRGM_REG_IND_ADR_BIT_BUSY_OFF    15
#define PRGM_REG_IND_ADR_BIT_RWB_MSK     0x00004000
#define PRGM_REG_IND_ADR_BIT_RWB_OFF     14
#define PRGM_REG_IND_ADR_BIT_IADDR_MSK   0x000003c0
#define PRGM_REG_IND_ADR_BIT_IADDR_OFF   6
#define PRGM_REG_IND_ADR_BIT_PATH_MSK    0x0000000f
#define PRGM_REG_IND_ADR_BIT_PATH_OFF    0

/*------------------------------.
 | Register 0x00001444 IND_DATA |
 +------------------------------+
 | bit  15:0 R/W  DATA          |
 +-----------------------------*/
#define PMC_PRGM_REG_IND_DATA_UNUSED_MASK 0xffff0000
#define PRGM_REG_IND_DATA_BIT_DATA_MSK    0x0000ffff
#define PRGM_REG_IND_DATA_BIT_DATA_OFF    0

/*-------------------------------------------.
 | Register 0x00001448 GENERATOR_PAYLOAD_CFG |
 +-------------------------------------------+
 | bit  15   R/W  GEN_STS12CSL               |
 | bit  14   R/W  GEN_STS12C                 |
 | bit  10:8 R/W  GEN_MSSLEN                 |
 +------------------------------------------*/
#define PMC_PRGM_REG_GENERATOR_PAYLOAD_CFG_UNUSED_MASK      0xffff38f0
#define PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12CSL_MSK 0x00008000
#define PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12CSL_OFF 15
#define PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12C_MSK   0x00004000
#define PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_STS12C_OFF   14
#define PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_MSK   0x00000700
#define PRGM_REG_GENERATOR_PAYLOAD_CFG_BIT_GEN_MSSLEN_OFF   8

/*-------------------------------------.
 | Register 0x0000144c MON_PAYLOAD_CFG |
 +-------------------------------------+
 | bit  15   R/W  MON_STS12CSL         |
 | bit  14   R/W  MON_STS12C           |
 | bit  10:8 R/W  MON_MSSLEN           |
 +------------------------------------*/
#define PMC_PRGM_REG_MON_PAYLOAD_CFG_UNUSED_MASK      0xffff38b0
#define PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12CSL_MSK 0x00008000
#define PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12CSL_OFF 15
#define PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12C_MSK   0x00004000
#define PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_STS12C_OFF   14
#define PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_MSK   0x00000700
#define PRGM_REG_MON_PAYLOAD_CFG_BIT_MON_MSSLEN_OFF   8

/*-------------------------------------------.
 | Register 0x00001450 MON_BYTE_ERR_INT_STAT |
 +-------------------------------------------+
 | bit  11 R/W  MON12_ERR_I                  |
 | bit  10 R/W  MON11_ERR_I                  |
 | bit  9  R/W  MON10_ERR_I                  |
 | bit  8  R/W  MON9_ERR_I                   |
 | bit  7  R/W  MON8_ERR_I                   |
 | bit  6  R/W  MON7_ERR_I                   |
 | bit  5  R/W  MON6_ERR_I                   |
 | bit  4  R/W  MON5_ERR_I                   |
 | bit  3  R/W  MON4_ERR_I                   |
 | bit  2  R/W  MON3_ERR_I                   |
 | bit  1  R/W  MON2_ERR_I                   |
 | bit  0  R/W  MON1_ERR_I                   |
 +------------------------------------------*/
#define PMC_PRGM_REG_MON_BYTE_ERR_INT_STAT_UNUSED_MASK     0xfffff000
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_MSK 0x00000800
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON12_ERR_I_OFF 11
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_MSK 0x00000400
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON11_ERR_I_OFF 10
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_MSK 0x00000200
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON10_ERR_I_OFF 9
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_MSK  0x00000100
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON9_ERR_I_OFF  8
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_MSK  0x00000080
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON8_ERR_I_OFF  7
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_MSK  0x00000040
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON7_ERR_I_OFF  6
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_MSK  0x00000020
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON6_ERR_I_OFF  5
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_MSK  0x00000010
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON5_ERR_I_OFF  4
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_MSK  0x00000008
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON4_ERR_I_OFF  3
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_MSK  0x00000004
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON3_ERR_I_OFF  2
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_MSK  0x00000002
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON2_ERR_I_OFF  1
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_MSK  0x00000001
#define PRGM_REG_MON_BYTE_ERR_INT_STAT_BIT_MON1_ERR_I_OFF  0

/*-----------------------------------------.
 | Register 0x00001454 MON_BYTE_ERR_INT_EN |
 +-----------------------------------------+
 | bit  11 R/W  MON12_ERR_E                |
 | bit  10 R/W  MON11_ERR_E                |
 | bit  9  R/W  MON10_ERR_E                |
 | bit  8  R/W  MON9_ERR_E                 |
 | bit  7  R/W  MON8_ERR_E                 |
 | bit  6  R/W  MON7_ERR_E                 |
 | bit  5  R/W  MON6_ERR_E                 |
 | bit  4  R/W  MON5_ERR_E                 |
 | bit  3  R/W  MON4_ERR_E                 |
 | bit  2  R/W  MON3_ERR_E                 |
 | bit  1  R/W  MON2_ERR_E                 |
 | bit  0  R/W  MON1_ERR_E                 |
 +----------------------------------------*/
#define PMC_PRGM_REG_MON_BYTE_ERR_INT_EN_UNUSED_MASK     0xfffff000
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON12_ERR_E_MSK 0x00000800
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON12_ERR_E_OFF 11
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON11_ERR_E_MSK 0x00000400
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON11_ERR_E_OFF 10
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON10_ERR_E_MSK 0x00000200
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON10_ERR_E_OFF 9
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON9_ERR_E_MSK  0x00000100
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON9_ERR_E_OFF  8
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON8_ERR_E_MSK  0x00000080
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON8_ERR_E_OFF  7
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON7_ERR_E_MSK  0x00000040
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON7_ERR_E_OFF  6
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON6_ERR_E_MSK  0x00000020
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON6_ERR_E_OFF  5
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON5_ERR_E_MSK  0x00000010
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON5_ERR_E_OFF  4
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON4_ERR_E_MSK  0x00000008
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON4_ERR_E_OFF  3
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON3_ERR_E_MSK  0x00000004
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON3_ERR_E_OFF  2
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON2_ERR_E_MSK  0x00000002
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON2_ERR_E_OFF  1
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON1_ERR_E_MSK  0x00000001
#define PRGM_REG_MON_BYTE_ERR_INT_EN_BIT_MON1_ERR_E_OFF  0

/*---------------------------------------.
 | Register 0x00001464 MON_SYNC_INT_STAT |
 +---------------------------------------+
 | bit  11 R/W  MON12_SYNC_I             |
 | bit  10 R/W  MON11_SYNC_I             |
 | bit  9  R/W  MON10_SYNC_I             |
 | bit  8  R/W  MON9_SYNC_I              |
 | bit  7  R/W  MON8_SYNC_I              |
 | bit  6  R/W  MON7_SYNC_I              |
 | bit  5  R/W  MON6_SYNC_I              |
 | bit  4  R/W  MON5_SYNC_I              |
 | bit  3  R/W  MON4_SYNC_I              |
 | bit  2  R/W  MON3_SYNC_I              |
 | bit  1  R/W  MON2_SYNC_I              |
 | bit  0  R/W  MON1_SYNC_I              |
 +--------------------------------------*/
#define PMC_PRGM_REG_MON_SYNC_INT_STAT_UNUSED_MASK      0xfffff000
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_MSK 0x00000800
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON12_SYNC_I_OFF 11
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_MSK 0x00000400
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON11_SYNC_I_OFF 10
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_MSK 0x00000200
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON10_SYNC_I_OFF 9
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_MSK  0x00000100
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON9_SYNC_I_OFF  8
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_MSK  0x00000080
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON8_SYNC_I_OFF  7
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_MSK  0x00000040
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON7_SYNC_I_OFF  6
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_MSK  0x00000020
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON6_SYNC_I_OFF  5
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_MSK  0x00000010
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON5_SYNC_I_OFF  4
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_MSK  0x00000008
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON4_SYNC_I_OFF  3
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_MSK  0x00000004
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON3_SYNC_I_OFF  2
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_MSK  0x00000002
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON2_SYNC_I_OFF  1
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_MSK  0x00000001
#define PRGM_REG_MON_SYNC_INT_STAT_BIT_MON1_SYNC_I_OFF  0

/*-------------------------------------.
 | Register 0x00001468 MON_SYNC_INT_EN |
 +-------------------------------------+
 | bit  11 R/W  MON12_SYNC_E           |
 | bit  10 R/W  MON11_SYNC_E           |
 | bit  9  R/W  MON10_SYNC_E           |
 | bit  8  R/W  MON9_SYNC_E            |
 | bit  7  R/W  MON8_SYNC_E            |
 | bit  6  R/W  MON7_SYNC_E            |
 | bit  5  R/W  MON6_SYNC_E            |
 | bit  4  R/W  MON5_SYNC_E            |
 | bit  3  R/W  MON4_SYNC_E            |
 | bit  2  R/W  MON3_SYNC_E            |
 | bit  1  R/W  MON2_SYNC_E            |
 | bit  0  R/W  MON1_SYNC_E            |
 +------------------------------------*/
#define PMC_PRGM_REG_MON_SYNC_INT_EN_UNUSED_MASK      0xfffff000
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON12_SYNC_E_MSK 0x00000800
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON12_SYNC_E_OFF 11
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON11_SYNC_E_MSK 0x00000400
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON11_SYNC_E_OFF 10
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON10_SYNC_E_MSK 0x00000200
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON10_SYNC_E_OFF 9
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON9_SYNC_E_MSK  0x00000100
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON9_SYNC_E_OFF  8
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON8_SYNC_E_MSK  0x00000080
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON8_SYNC_E_OFF  7
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON7_SYNC_E_MSK  0x00000040
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON7_SYNC_E_OFF  6
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON6_SYNC_E_MSK  0x00000020
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON6_SYNC_E_OFF  5
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON5_SYNC_E_MSK  0x00000010
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON5_SYNC_E_OFF  4
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON4_SYNC_E_MSK  0x00000008
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON4_SYNC_E_OFF  3
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON3_SYNC_E_MSK  0x00000004
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON3_SYNC_E_OFF  2
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON2_SYNC_E_MSK  0x00000002
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON2_SYNC_E_OFF  1
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON1_SYNC_E_MSK  0x00000001
#define PRGM_REG_MON_SYNC_INT_EN_BIT_MON1_SYNC_E_OFF  0

/*-----------------------------------.
 | Register 0x0000146c MON_SYNC_STAT |
 +-----------------------------------+
 | bit  11 R  MON12_SYNC_V           |
 | bit  10 R  MON11_SYNC_V           |
 | bit  9  R  MON10_SYNC_V           |
 | bit  8  R  MON9_SYNC_V            |
 | bit  7  R  MON8_SYNC_V            |
 | bit  6  R  MON7_SYNC_V            |
 | bit  5  R  MON6_SYNC_V            |
 | bit  4  R  MON5_SYNC_V            |
 | bit  3  R  MON4_SYNC_V            |
 | bit  2  R  MON3_SYNC_V            |
 | bit  1  R  MON2_SYNC_V            |
 | bit  0  R  MON1_SYNC_V            |
 +----------------------------------*/
#define PMC_PRGM_REG_MON_SYNC_STAT_UNUSED_MASK      0xfffff000
#define PRGM_REG_MON_SYNC_STAT_BIT_MON12_SYNC_V_MSK 0x00000800
#define PRGM_REG_MON_SYNC_STAT_BIT_MON12_SYNC_V_OFF 11
#define PRGM_REG_MON_SYNC_STAT_BIT_MON11_SYNC_V_MSK 0x00000400
#define PRGM_REG_MON_SYNC_STAT_BIT_MON11_SYNC_V_OFF 10
#define PRGM_REG_MON_SYNC_STAT_BIT_MON10_SYNC_V_MSK 0x00000200
#define PRGM_REG_MON_SYNC_STAT_BIT_MON10_SYNC_V_OFF 9
#define PRGM_REG_MON_SYNC_STAT_BIT_MON9_SYNC_V_MSK  0x00000100
#define PRGM_REG_MON_SYNC_STAT_BIT_MON9_SYNC_V_OFF  8
#define PRGM_REG_MON_SYNC_STAT_BIT_MON8_SYNC_V_MSK  0x00000080
#define PRGM_REG_MON_SYNC_STAT_BIT_MON8_SYNC_V_OFF  7
#define PRGM_REG_MON_SYNC_STAT_BIT_MON7_SYNC_V_MSK  0x00000040
#define PRGM_REG_MON_SYNC_STAT_BIT_MON7_SYNC_V_OFF  6
#define PRGM_REG_MON_SYNC_STAT_BIT_MON6_SYNC_V_MSK  0x00000020
#define PRGM_REG_MON_SYNC_STAT_BIT_MON6_SYNC_V_OFF  5
#define PRGM_REG_MON_SYNC_STAT_BIT_MON5_SYNC_V_MSK  0x00000010
#define PRGM_REG_MON_SYNC_STAT_BIT_MON5_SYNC_V_OFF  4
#define PRGM_REG_MON_SYNC_STAT_BIT_MON4_SYNC_V_MSK  0x00000008
#define PRGM_REG_MON_SYNC_STAT_BIT_MON4_SYNC_V_OFF  3
#define PRGM_REG_MON_SYNC_STAT_BIT_MON3_SYNC_V_MSK  0x00000004
#define PRGM_REG_MON_SYNC_STAT_BIT_MON3_SYNC_V_OFF  2
#define PRGM_REG_MON_SYNC_STAT_BIT_MON2_SYNC_V_MSK  0x00000002
#define PRGM_REG_MON_SYNC_STAT_BIT_MON2_SYNC_V_OFF  1
#define PRGM_REG_MON_SYNC_STAT_BIT_MON1_SYNC_V_MSK  0x00000001
#define PRGM_REG_MON_SYNC_STAT_BIT_MON1_SYNC_V_OFF  0

/*--------------------------------------------.
 | Register 0x00001470 PERF_CNT_TRANSFER_TRIG |
 +--------------------------------------------+
 | bit  0 R/W  TIP                            |
 +-------------------------------------------*/
#define PMC_PRGM_REG_PERF_CNT_TRANSFER_TRIG_UNUSED_MASK 0xfffffffe
#define PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_MSK     0x00000001
#define PRGM_REG_PERF_CNT_TRANSFER_TRIG_BIT_TIP_OFF     0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRGM_REGS_H */
