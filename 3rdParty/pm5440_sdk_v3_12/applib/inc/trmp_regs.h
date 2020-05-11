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
 *     trmp block
 *****************************************************************************/
#ifndef _TRMP_REGS_H
#define _TRMP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TRMP_REG_CFG               0x00001040
#define PMC_TRMP_REG_INSERTION         0x00001044
#define PMC_TRMP_REG_TX_J0_AND_Z0      0x0000104c
#define PMC_TRMP_REG_TX_E1_AND_F1      0x00001050
#define PMC_TRMP_REG_TX_D1D3_AND_D4D12 0x00001054
#define PMC_TRMP_REG_TX_K1_AND_K2      0x00001058
#define PMC_TRMP_REG_TX_S1_AND_Z1      0x0000105c
#define PMC_TRMP_REG_TX_Z2_AND_E2      0x00001060
#define PMC_TRMP_REG_TX_NATIONAL       0x0000106c

/*---------------------------.
 | Register 0x00001040 CFG   |
 +---------------------------+
 | bit  15 R/W  SCRAMA1A2SEL |
 | bit  14 R/W  SCRAMJ0SEL   |
 | bit  13 R/W  FRM768SEL    |
 | bit  12 R/W  M0EN         |
 | bit  3  R/W  STRACEEN     |
 | bit  2  R/W  J0Z0INCEN    |
 | bit  1  R/W  Z0DEF        |
 | bit  0  R/W  A1A2EN       |
 +--------------------------*/
#define PMC_TRMP_REG_CFG_UNUSED_MASK      0xffff00e0
#define TRMP_REG_CFG_BIT_SCRAMA1A2SEL_MSK 0x00008000
#define TRMP_REG_CFG_BIT_SCRAMA1A2SEL_OFF 15
#define TRMP_REG_CFG_BIT_SCRAMJ0SEL_MSK   0x00004000
#define TRMP_REG_CFG_BIT_SCRAMJ0SEL_OFF   14
#define TRMP_REG_CFG_BIT_FRM768SEL_MSK    0x00002000
#define TRMP_REG_CFG_BIT_FRM768SEL_OFF    13
#define TRMP_REG_CFG_BIT_M0EN_MSK         0x00001000
#define TRMP_REG_CFG_BIT_M0EN_OFF         12
#define TRMP_REG_CFG_BIT_STRACEEN_MSK     0x00000008
#define TRMP_REG_CFG_BIT_STRACEEN_OFF     3
#define TRMP_REG_CFG_BIT_J0Z0INCEN_MSK    0x00000004
#define TRMP_REG_CFG_BIT_J0Z0INCEN_OFF    2
#define TRMP_REG_CFG_BIT_Z0DEF_MSK        0x00000002
#define TRMP_REG_CFG_BIT_Z0DEF_OFF        1
#define TRMP_REG_CFG_BIT_A1A2EN_MSK       0x00000001
#define TRMP_REG_CFG_BIT_A1A2EN_OFF       0

/*-------------------------------.
 | Register 0x00001044 INSERTION |
 +-------------------------------+
 | bit  12 R/W  NATIONALEN       |
 | bit  10 R/W  E2REGEN          |
 | bit  9  R/W  Z2REGEN          |
 | bit  8  R/W  Z1REGEN          |
 | bit  7  R/W  S1REGEN          |
 | bit  6  R/W  D4D12REGEN       |
 | bit  5  R/W  K1K2REGEN        |
 | bit  4  R/W  D1D3REGEN        |
 | bit  3  R/W  F1REGEN          |
 | bit  2  R/W  E1REGEN          |
 | bit  1  R/W  Z0REGEN          |
 | bit  0  R/W  J0REGEN          |
 +------------------------------*/
#define PMC_TRMP_REG_INSERTION_UNUSED_MASK    0xffff2800
#define TRMP_REG_INSERTION_BIT_NATIONALEN_MSK 0x00001000
#define TRMP_REG_INSERTION_BIT_NATIONALEN_OFF 12
#define TRMP_REG_INSERTION_BIT_E2REGEN_MSK    0x00000400
#define TRMP_REG_INSERTION_BIT_E2REGEN_OFF    10
#define TRMP_REG_INSERTION_BIT_Z2REGEN_MSK    0x00000200
#define TRMP_REG_INSERTION_BIT_Z2REGEN_OFF    9
#define TRMP_REG_INSERTION_BIT_Z1REGEN_MSK    0x00000100
#define TRMP_REG_INSERTION_BIT_Z1REGEN_OFF    8
#define TRMP_REG_INSERTION_BIT_S1REGEN_MSK    0x00000080
#define TRMP_REG_INSERTION_BIT_S1REGEN_OFF    7
#define TRMP_REG_INSERTION_BIT_D4D12REGEN_MSK 0x00000040
#define TRMP_REG_INSERTION_BIT_D4D12REGEN_OFF 6
#define TRMP_REG_INSERTION_BIT_K1K2REGEN_MSK  0x00000020
#define TRMP_REG_INSERTION_BIT_K1K2REGEN_OFF  5
#define TRMP_REG_INSERTION_BIT_D1D3REGEN_MSK  0x00000010
#define TRMP_REG_INSERTION_BIT_D1D3REGEN_OFF  4
#define TRMP_REG_INSERTION_BIT_F1REGEN_MSK    0x00000008
#define TRMP_REG_INSERTION_BIT_F1REGEN_OFF    3
#define TRMP_REG_INSERTION_BIT_E1REGEN_MSK    0x00000004
#define TRMP_REG_INSERTION_BIT_E1REGEN_OFF    2
#define TRMP_REG_INSERTION_BIT_Z0REGEN_MSK    0x00000002
#define TRMP_REG_INSERTION_BIT_Z0REGEN_OFF    1
#define TRMP_REG_INSERTION_BIT_J0REGEN_MSK    0x00000001
#define TRMP_REG_INSERTION_BIT_J0REGEN_OFF    0

/*----------------------------------.
 | Register 0x0000104c TX_J0_AND_Z0 |
 +----------------------------------+
 | bit  15:8 R/W  J0V               |
 | bit  7:0  R/W  Z0V               |
 +---------------------------------*/
#define PMC_TRMP_REG_TX_J0_AND_Z0_UNUSED_MASK 0xffff0000
#define TRMP_REG_TX_J0_AND_Z0_BIT_J0V_MSK     0x0000ff00
#define TRMP_REG_TX_J0_AND_Z0_BIT_J0V_OFF     8
#define TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_MSK     0x000000ff
#define TRMP_REG_TX_J0_AND_Z0_BIT_Z0V_OFF     0

/*----------------------------------.
 | Register 0x00001050 TX_E1_AND_F1 |
 +----------------------------------+
 | bit  15:8 R/W  E1V               |
 | bit  7:0  R/W  F1V               |
 +---------------------------------*/
#define PMC_TRMP_REG_TX_E1_AND_F1_UNUSED_MASK 0xffff0000
#define TRMP_REG_TX_E1_AND_F1_BIT_E1V_MSK     0x0000ff00
#define TRMP_REG_TX_E1_AND_F1_BIT_E1V_OFF     8
#define TRMP_REG_TX_E1_AND_F1_BIT_F1V_MSK     0x000000ff
#define TRMP_REG_TX_E1_AND_F1_BIT_F1V_OFF     0

/*---------------------------------------.
 | Register 0x00001054 TX_D1D3_AND_D4D12 |
 +---------------------------------------+
 | bit  15:8 R/W  D1D3V                  |
 | bit  7:0  R/W  D4D12V                 |
 +--------------------------------------*/
#define PMC_TRMP_REG_TX_D1D3_AND_D4D12_UNUSED_MASK 0xffff0000
#define TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_MSK   0x0000ff00
#define TRMP_REG_TX_D1D3_AND_D4D12_BIT_D1D3V_OFF   8
#define TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_MSK  0x000000ff
#define TRMP_REG_TX_D1D3_AND_D4D12_BIT_D4D12V_OFF  0

/*----------------------------------.
 | Register 0x00001058 TX_K1_AND_K2 |
 +----------------------------------+
 | bit  15:8 R/W  K1V               |
 | bit  7:0  R/W  K2V               |
 +---------------------------------*/
#define PMC_TRMP_REG_TX_K1_AND_K2_UNUSED_MASK 0xffff0000
#define TRMP_REG_TX_K1_AND_K2_BIT_K1V_MSK     0x0000ff00
#define TRMP_REG_TX_K1_AND_K2_BIT_K1V_OFF     8
#define TRMP_REG_TX_K1_AND_K2_BIT_K2V_MSK     0x000000ff
#define TRMP_REG_TX_K1_AND_K2_BIT_K2V_OFF     0

/*----------------------------------.
 | Register 0x0000105c TX_S1_AND_Z1 |
 +----------------------------------+
 | bit  15:8 R/W  S1V               |
 | bit  7:0  R/W  Z1V               |
 +---------------------------------*/
#define PMC_TRMP_REG_TX_S1_AND_Z1_UNUSED_MASK 0xffff0000
#define TRMP_REG_TX_S1_AND_Z1_BIT_S1V_MSK     0x0000ff00
#define TRMP_REG_TX_S1_AND_Z1_BIT_S1V_OFF     8
#define TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_MSK     0x000000ff
#define TRMP_REG_TX_S1_AND_Z1_BIT_Z1V_OFF     0

/*----------------------------------.
 | Register 0x00001060 TX_Z2_AND_E2 |
 +----------------------------------+
 | bit  15:8 R/W  Z2V               |
 | bit  7:0  R/W  E2V               |
 +---------------------------------*/
#define PMC_TRMP_REG_TX_Z2_AND_E2_UNUSED_MASK 0xffff0000
#define TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_MSK     0x0000ff00
#define TRMP_REG_TX_Z2_AND_E2_BIT_Z2V_OFF     8
#define TRMP_REG_TX_Z2_AND_E2_BIT_E2V_MSK     0x000000ff
#define TRMP_REG_TX_Z2_AND_E2_BIT_E2V_OFF     0

/*---------------------------------.
 | Register 0x0000106c TX_NATIONAL |
 +---------------------------------+
 | bit  7:0 R/W  NATIONALV         |
 +--------------------------------*/
#define PMC_TRMP_REG_TX_NATIONAL_UNUSED_MASK   0xffffff00
#define TRMP_REG_TX_NATIONAL_BIT_NATIONALV_MSK 0x000000ff
#define TRMP_REG_TX_NATIONAL_BIT_NATIONALV_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TRMP_REGS_H */
