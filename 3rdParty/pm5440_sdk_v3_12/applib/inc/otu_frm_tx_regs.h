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
 *     otu_frm_tx block
 *****************************************************************************/
#ifndef _OTU_FRM_TX_REGS_H
#define _OTU_FRM_TX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI( N,M )    (0x00003000 + (N) * 0x100 + (M) * 0x04)
#define PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM( N )     (0x00003040 + (N) * 0x100)
#define PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES( N )    (0x00003044 + (N) * 0x100)
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL( N ) (0x00003048 + (N) * 0x100)
#define PMC_OTU_FRM_TX_REG_OTU_SO_CTRL( N )        (0x0000304c + (N) * 0x100)
#define PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL( N ) (0x00003050 + (N) * 0x100)
#define PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6           0x00003f04
#define PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28           0x00003f08
#define PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E          0x00003f40
#define PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I          0x00003f80
#define PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V          0x00003fc0

/* index definitions for PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI */
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_N_MIN       0
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_N_MAX       11
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_N_SIZE      12
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_N_OFFSET    0x100
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_M_MIN       0
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_M_MAX       15
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_M_SIZE      16
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_INDEX_M_OFFSET    0x04

/*----------------------------------------------------------------.
 | Register (0x00003000 + (N) * 0x100 + (M) * 0x04) OTU_SO_SM_TTI |
 +----------------------------------------------------------------+
 | bit  31:0 R/W  OTU_SO_SM_TTI                                   |
 +---------------------------------------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_TTI_UNUSED_MASK       0x00000000
#define OTU_FRM_TX_REG_OTU_SO_SM_TTI_BIT_OTU_SO_SM_TTI_MSK 0xffffffff
#define OTU_FRM_TX_REG_OTU_SO_SM_TTI_BIT_OTU_SO_SM_TTI_OFF 0

/* index definitions for PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM */
#define PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM_INDEX_N_MIN            0
#define PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM_INDEX_N_MAX            11
#define PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM_INDEX_N_SIZE           12
#define PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM_INDEX_N_OFFSET         0x100

/*----------------------------------------------------.
 | Register (0x00003040 + (N) * 0x100) OTU_SO_MFAS_SM |
 +----------------------------------------------------+
 | bit  23:16 R/W  OTU_SO_MFAS                        |
 | bit  15:8  R/W  OTU_SO_SM_BIP8                     |
 | bit  7:4   R/W  OTU_SO_SM_BEI_BIAE                 |
 | bit  3     R/W  OTU_SO_SM_BDI                      |
 | bit  2     R/W  OTU_SO_SM_IAE                      |
 | bit  1     R/W  OTU_SO_SM_RES7                     |
 | bit  0     R/W  OTU_SO_SM_RES8                     |
 +---------------------------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_MFAS_SM_UNUSED_MASK            0x00000000
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_MSK        0x00ff0000
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_MFAS_OFF        16
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_MSK     0x0000ff00
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BIP8_OFF     8
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_MSK 0x000000f0
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BEI_BIAE_OFF 4
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BDI_MSK      0x00000008
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_BDI_OFF      3
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_IAE_MSK      0x00000004
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_IAE_OFF      2
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES7_MSK     0x00000002
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES7_OFF     1
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES8_MSK     0x00000001
#define OTU_FRM_TX_REG_OTU_SO_MFAS_SM_BIT_OTU_SO_SM_RES8_OFF     0

/* index definitions for PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES */
#define PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES_INDEX_N_MIN      0
#define PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES_INDEX_N_MAX      11
#define PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES_INDEX_N_SIZE     12
#define PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES_INDEX_N_OFFSET   0x100

/*-----------------------------------------------------.
 | Register (0x00003044 + (N) * 0x100) OTU_SO_GCC0_RES |
 +-----------------------------------------------------+
 | bit  31:16 R/W  OTU_SO_GCC0                         |
 | bit  15:8  R/W  OTU_SO_RES13                        |
 | bit  7:0   R/W  OTU_SO_RES14                        |
 +----------------------------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_GCC0_RES_UNUSED_MASK      0x00000000
#define OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_MSK  0xffff0000
#define OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_GCC0_OFF  16
#define OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_MSK 0x0000ff00
#define OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES13_OFF 8
#define OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_MSK 0x000000ff
#define OTU_FRM_TX_REG_OTU_SO_GCC0_RES_BIT_OTU_SO_RES14_OFF 0

/* index definitions for PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL */
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_INDEX_N_MIN                     0
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_INDEX_N_MAX                     11
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_INDEX_N_SIZE                    12
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_INDEX_N_OFFSET                  0x100

/*--------------------------------------------------------.
 | Register (0x00003048 + (N) * 0x100) OTU_SO_SM_INS_CTRL |
 +--------------------------------------------------------+
 | bit  19    R/W  OTU_SO_FAS_INS_CTRL                    |
 | bit  18:17 R/W  OTU_SO_MFAS_INS_CTRL                   |
 | bit  16    R/W  OTU_SO_SM_TTI_SAPI_INS_CTRL            |
 | bit  15    R/W  OTU_SO_SM_TTI_DAPI_INS_CTRL            |
 | bit  14    R/W  OTU_SO_SM_TTI_OP_INS_CTRL              |
 | bit  13:11 R/W  OTU_SO_SM_BIP8_INS_CTRL                |
 | bit  10:9  R/W  OTU_SO_SM_BEI_BIAE_INS_CTRL            |
 | bit  8:7   R/W  OTU_SO_SM_BDI_INS_CTRL                 |
 | bit  6:5   R/W  OTU_SO_SM_IAE_INS_CTRL                 |
 | bit  4     R/W  OTU_SO_SM_RES7_INS_CTRL                |
 | bit  3     R/W  OTU_SO_SM_RES8_INS_CTRL                |
 | bit  2     R/W  OTU_SO_GCC0_INS_CTRL                   |
 | bit  1     R/W  OTU_SO_RES13_INS_CTRL                  |
 | bit  0     R/W  OTU_SO_RES14_INS_CTRL                  |
 +-------------------------------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_UNUSED_MASK                     0xff000000
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_FAS_INS_CTRL_MSK         0x00080000
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_FAS_INS_CTRL_OFF         19
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_MSK        0x00060000
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_MFAS_INS_CTRL_OFF        17
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_SAPI_INS_CTRL_MSK 0x00010000
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_SAPI_INS_CTRL_OFF 16
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_DAPI_INS_CTRL_MSK 0x00008000
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_DAPI_INS_CTRL_OFF 15
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_OP_INS_CTRL_MSK   0x00004000
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_TTI_OP_INS_CTRL_OFF   14
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_MSK     0x00003800
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BIP8_INS_CTRL_OFF     11
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_MSK 0x00000600
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BEI_BIAE_INS_CTRL_OFF 9
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_MSK      0x00000180
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_BDI_INS_CTRL_OFF      7
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_MSK      0x00000060
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_IAE_INS_CTRL_OFF      5
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES7_INS_CTRL_MSK     0x00000010
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES7_INS_CTRL_OFF     4
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES8_INS_CTRL_MSK     0x00000008
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_SM_RES8_INS_CTRL_OFF     3
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_GCC0_INS_CTRL_MSK        0x00000004
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_GCC0_INS_CTRL_OFF        2
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES13_INS_CTRL_MSK       0x00000002
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES13_INS_CTRL_OFF       1
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES14_INS_CTRL_MSK       0x00000001
#define OTU_FRM_TX_REG_OTU_SO_SM_INS_CTRL_BIT_OTU_SO_RES14_INS_CTRL_OFF       0

/* index definitions for PMC_OTU_FRM_TX_REG_OTU_SO_CTRL */
#define PMC_OTU_FRM_TX_REG_OTU_SO_CTRL_INDEX_N_MIN              0
#define PMC_OTU_FRM_TX_REG_OTU_SO_CTRL_INDEX_N_MAX              11
#define PMC_OTU_FRM_TX_REG_OTU_SO_CTRL_INDEX_N_SIZE             12
#define PMC_OTU_FRM_TX_REG_OTU_SO_CTRL_INDEX_N_OFFSET           0x100

/*-------------------------------------------------.
 | Register (0x0000304c + (N) * 0x100) OTU_SO_CTRL |
 +-------------------------------------------------+
 | bit  2:1 R/W  OTU_SO_MAINT_SIG                  |
 | bit  0   R/W  OTU_SO_MI_ADMINSTATE              |
 +------------------------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_CTRL_UNUSED_MASK              0xfffffff8
#define OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_MSK     0x00000006
#define OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MAINT_SIG_OFF     1
#define OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MI_ADMINSTATE_MSK 0x00000001
#define OTU_FRM_TX_REG_OTU_SO_CTRL_BIT_OTU_SO_MI_ADMINSTATE_OFF 0

/* index definitions for PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL */
#define PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_INDEX_N_MIN         0
#define PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_INDEX_N_MAX         11
#define PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_INDEX_N_SIZE        12
#define PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_INDEX_N_OFFSET      0x100

/*--------------------------------------------------------.
 | Register (0x00003050 + (N) * 0x100) OTU_SO_MASTER_CTRL |
 +--------------------------------------------------------+
 | bit  0 R/W  OTU_SO_PASSTHRU                            |
 +-------------------------------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_UNUSED_MASK         0xfffffff0
#define OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_BIT_OTU_SO_PASSTHRU_MSK 0x00000001
#define OTU_FRM_TX_REG_OTU_SO_MASTER_CTRL_BIT_OTU_SO_PASSTHRU_OFF 0

/*-----------------------------------.
 | Register 0x00003f04 OTU_SO_FAS_F6 |
 +-----------------------------------+
 | bit  23:0 R/W  OTU_SO_FAS_F6      |
 +----------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_FAS_F6_UNUSED_MASK       0xff000000
#define OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_MSK 0x00ffffff
#define OTU_FRM_TX_REG_OTU_SO_FAS_F6_BIT_OTU_SO_FAS_F6_OFF 0

/*-----------------------------------.
 | Register 0x00003f08 OTU_SO_FAS_28 |
 +-----------------------------------+
 | bit  23:0 R/W  OTU_SO_FAS_28      |
 +----------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_FAS_28_UNUSED_MASK       0xff000000
#define OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_MSK 0x00ffffff
#define OTU_FRM_TX_REG_OTU_SO_FAS_28_BIT_OTU_SO_FAS_28_OFF 0

/*------------------------------------.
 | Register 0x00003f40 OTU_SO_A_IAE_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SO_A_IAE_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_MSK 0x00000fff
#define OTU_FRM_TX_REG_OTU_SO_A_IAE_E_BIT_OTU_SO_A_IAE_E_OFF 0

/*------------------------------------.
 | Register 0x00003f80 OTU_SO_A_IAE_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SO_A_IAE_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_MSK 0x00000fff
#define OTU_FRM_TX_REG_OTU_SO_A_IAE_I_BIT_OTU_SO_A_IAE_I_OFF 0

/*------------------------------------.
 | Register 0x00003fc0 OTU_SO_A_IAE_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SO_A_IAE_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_TX_REG_OTU_SO_A_IAE_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_MSK 0x00000fff
#define OTU_FRM_TX_REG_OTU_SO_A_IAE_V_BIT_OTU_SO_A_IAE_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTU_FRM_TX_REGS_H */
