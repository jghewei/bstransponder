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
 *     otu_frm_rx block
 *****************************************************************************/
#ifndef _OTU_FRM_RX_REGS_H
#define _OTU_FRM_RX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI( N,M )  (0x00002000 + (N) * 0x100 + (M) * 0x04)
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR( N )    (0x00002040 + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL( N )        (0x00002044 + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU( N )      (0x00002048 + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI( N,M )   (0x00002080 + (N) * 0x100 + (M) * 0x04)
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON( N )       (0x000020c0 + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON( N )    (0x000020c4 + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH( N )         (0x000020c8 + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_OH2( N )           (0x000020cc + (N) * 0x100)
#define PMC_OTU_FRM_RX_REG_OTU_SK_CFG                0x00002f00
#define PMC_OTU_FRM_RX_REG_OTU_SK_RAM_ECC_BYPASS     0x00002f04
#define PMC_OTU_FRM_RX_REG_OTU_SK_RAM_ECC_ERR_INJECT 0x00002f08
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E            0x00002f40
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E            0x00002f44
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E            0x00002f48
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E            0x00002f4c
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E            0x00002f50
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E           0x00002f54
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E            0x00002f58
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E            0x00002f5c
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E            0x00002f60
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E            0x00002f64
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E            0x00002f68
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E            0x00002f6c
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E           0x00002f70
#define PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E           0x00002f74
#define PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E      0x00002f78
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I            0x00002f80
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I            0x00002f84
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I            0x00002f88
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I            0x00002f8c
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I            0x00002f90
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I           0x00002f94
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I            0x00002f98
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I            0x00002f9c
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I            0x00002fa0
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I            0x00002fa4
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I            0x00002fa8
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I            0x00002fac
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I           0x00002fb0
#define PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I           0x00002fb4
#define PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I      0x00002fb8
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V            0x00002fc0
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V            0x00002fc4
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V            0x00002fc8
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V            0x00002fcc
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V            0x00002fd0
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V           0x00002fd4
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V            0x00002fd8
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V            0x00002fdc
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V            0x00002fe0
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V            0x00002fe4
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V            0x00002fe8
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V            0x00002fec
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V           0x00002ff0
#define PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V           0x00002ff4
#define PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V      0x00002ff8

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_N_MIN           0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_N_MAX           11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_N_SIZE          12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_N_OFFSET        0x100
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_M_MIN           0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_M_MAX           15
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_M_SIZE          16
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_INDEX_M_OFFSET        0x04

/*--------------------------------------------------------------------.
 | Register (0x00002000 + (N) * 0x100 + (M) * 0x04) OTU_SK_SM_EXP_TTI |
 +--------------------------------------------------------------------+
 | bit  31:0 R/W  OTU_SK_SM_EXP_TTI                                   |
 +-------------------------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_UNUSED_MASK           0x00000000
#define OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_BIT_OTU_SK_SM_EXP_TTI_MSK 0xffffffff
#define OTU_FRM_RX_REG_OTU_SK_SM_EXP_TTI_BIT_OTU_SK_SM_EXP_TTI_OFF 0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_INDEX_N_MIN           0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_INDEX_N_MAX           11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_INDEX_N_SIZE          12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_INDEX_N_OFFSET        0x100

/*-------------------------------------------------------.
 | Register (0x00002040 + (N) * 0x100) OTU_SK_SM_DEG_THR |
 +-------------------------------------------------------+
 | bit  23:20 R/W  OTU_SK_SM_DEG_M                       |
 | bit  19:0  R/W  OTU_SK_SM_DEG_THR                     |
 +------------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_UNUSED_MASK           0xff000000
#define OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_MSK   0x00f00000
#define OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_M_OFF   20
#define OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_MSK 0x000fffff
#define OTU_FRM_RX_REG_OTU_SK_SM_DEG_THR_BIT_OTU_SK_SM_DEG_THR_OFF 0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL_INDEX_N_MIN                 0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL_INDEX_N_MAX                 11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL_INDEX_N_SIZE                12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL_INDEX_N_OFFSET              0x100

/*---------------------------------------------------.
 | Register (0x00002044 + (N) * 0x100) OTU_SK_SM_CTL |
 +---------------------------------------------------+
 | bit  22  R/W  OTU_SK_BDI_DIS_FROM_TTI             |
 | bit  21  R/W  OTU_SK_BDI_DIS_FROM_SSF             |
 | bit  20  R/W  OTU_SK_BEI_DIS                      |
 | bit  19  R/W  OTU_SK_BIAE_DIS                     |
 | bit  18  R/W  OTU_SK_TSD_DIS                      |
 | bit  17  R/W  OTU_SK_TSF_DIS_FROM_SSF             |
 | bit  16  R/W  OTU_SK_TIM_ACT_DIS                  |
 | bit  14  R/W  OTU_SK_ADAPT_SSD_DIS                |
 | bit  13  R/W  OTU_SK_ADAPT_SSF_DIS                |
 | bit  12  R/W  OTU_SK_ADAPT_AIS_DIS                |
 | bit  8   R/W  OTU_SK_MI_LCK                       |
 | bit  7   R/W  OTU_SK_SSF_MI_DIS                   |
 | bit  6   R/W  OTU_SK_AIS_MI_DIS                   |
 | bit  5   R/W  OTU_SK_SSD_MI_DIS                   |
 | bit  3:2 R/W  OTU_SK_TIM_DET_MO                   |
 | bit  1:0 R/W  OTU_SK_MAINT_SIG                    |
 +--------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_CTL_UNUSED_MASK                 0xff808e00
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_TTI_MSK 0x00400000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_TTI_OFF 22
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_SSF_MSK 0x00200000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BDI_DIS_FROM_SSF_OFF 21
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BEI_DIS_MSK          0x00100000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BEI_DIS_OFF          20
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BIAE_DIS_MSK         0x00080000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_BIAE_DIS_OFF         19
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSD_DIS_MSK          0x00040000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSD_DIS_OFF          18
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSF_DIS_FROM_SSF_MSK 0x00020000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TSF_DIS_FROM_SSF_OFF 17
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_ACT_DIS_MSK      0x00010000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_ACT_DIS_OFF      16
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSD_DIS_MSK    0x00004000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSD_DIS_OFF    14
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSF_DIS_MSK    0x00002000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_SSF_DIS_OFF    13
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_AIS_DIS_MSK    0x00001000
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_ADAPT_AIS_DIS_OFF    12
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MI_LCK_MSK           0x00000100
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MI_LCK_OFF           8
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSF_MI_DIS_MSK       0x00000080
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSF_MI_DIS_OFF       7
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_AIS_MI_DIS_MSK       0x00000040
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_AIS_MI_DIS_OFF       6
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSD_MI_DIS_MSK       0x00000020
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_SSD_MI_DIS_OFF       5
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_MSK       0x0000000c
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_TIM_DET_MO_OFF       2
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_MSK        0x00000003
#define OTU_FRM_RX_REG_OTU_SK_SM_CTL_BIT_OTU_SK_MAINT_SIG_OFF        0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU */
#define PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU_INDEX_N_MIN         0
#define PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU_INDEX_N_MAX         11
#define PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU_INDEX_N_SIZE        12
#define PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU_INDEX_N_OFFSET      0x100

/*-----------------------------------------------------.
 | Register (0x00002048 + (N) * 0x100) OTU_SK_PASSTHRU |
 +-----------------------------------------------------+
 | bit  0 R/W  OTU_SK_PASSTHRU                         |
 +----------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_PASSTHRU_UNUSED_MASK         0xfffffffe
#define OTU_FRM_RX_REG_OTU_SK_PASSTHRU_BIT_OTU_SK_PASSTHRU_MSK 0x00000001
#define OTU_FRM_RX_REG_OTU_SK_PASSTHRU_BIT_OTU_SK_PASSTHRU_OFF 0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_N_MIN          0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_N_MAX          11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_N_SIZE         12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_N_OFFSET       0x100
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_M_MIN          0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_M_MAX          15
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_M_SIZE         16
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_INDEX_M_OFFSET       0x04

/*-------------------------------------------------------------------.
 | Register (0x00002080 + (N) * 0x100 + (M) * 0x04) OTU_SK_SM_AC_TTI |
 +-------------------------------------------------------------------+
 | bit  31:0 R  OTU_SK_SM_AC_TTI                                     |
 +------------------------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_UNUSED_MASK          0x00000000
#define OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_BIT_OTU_SK_SM_AC_TTI_MSK 0xffffffff
#define OTU_FRM_RX_REG_OTU_SK_SM_AC_TTI_BIT_OTU_SK_SM_AC_TTI_OFF 0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON_INDEX_N_MIN           0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON_INDEX_N_MAX           11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON_INDEX_N_SIZE          12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON_INDEX_N_OFFSET        0x100

/*----------------------------------------------------.
 | Register (0x000020c0 + (N) * 0x100) OTU_SK_SM_PMON |
 +----------------------------------------------------+
 | bit  25   R  OTU_SK_SM_P_IAE                       |
 | bit  24   R  OTU_SK_SM_P_N_DS                      |
 | bit  23:0 R  OTU_SK_SM_P_N_EBC                     |
 +---------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_PMON_UNUSED_MASK           0xfc000000
#define OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_IAE_MSK   0x02000000
#define OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_IAE_OFF   25
#define OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_DS_MSK  0x01000000
#define OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_DS_OFF  24
#define OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_MSK 0x00ffffff
#define OTU_FRM_RX_REG_OTU_SK_SM_PMON_BIT_OTU_SK_SM_P_N_EBC_OFF 0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_INDEX_N_MIN           0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_INDEX_N_MAX           11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_INDEX_N_SIZE          12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_INDEX_N_OFFSET        0x100

/*-------------------------------------------------------.
 | Register (0x000020c4 + (N) * 0x100) OTU_SK_SM_FE_PMON |
 +-------------------------------------------------------+
 | bit  25   R  OTU_SK_SM_P_BIAE                         |
 | bit  24   R  OTU_SK_SM_P_F_DS                         |
 | bit  23:0 R  OTU_SK_SM_P_F_EBC                        |
 +------------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_UNUSED_MASK           0xfc000000
#define OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_BIAE_MSK  0x02000000
#define OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_BIAE_OFF  25
#define OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_DS_MSK  0x01000000
#define OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_DS_OFF  24
#define OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_MSK 0x00ffffff
#define OTU_FRM_RX_REG_OTU_SK_SM_FE_PMON_BIT_OTU_SK_SM_P_F_EBC_OFF 0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH */
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH_INDEX_N_MIN            0
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH_INDEX_N_MAX            11
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH_INDEX_N_SIZE           12
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH_INDEX_N_OFFSET         0x100

/*--------------------------------------------------.
 | Register (0x000020c8 + (N) * 0x100) OTU_SK_SM_OH |
 +--------------------------------------------------+
 | bit  23:16 R  OTU_SK_SM_TTI                      |
 | bit  15:8  R  OTU_SK_SM_BIP8                     |
 | bit  7:4   R  OTU_SK_SM_BEI_BIAE                 |
 | bit  3     R  OTU_SK_SM_BDI                      |
 | bit  2     R  OTU_SK_SM_IAE                      |
 | bit  1:0   R  OTU_SK_SM_RES                      |
 +-------------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SM_OH_UNUSED_MASK            0xff000000
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_MSK      0x00ff0000
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_TTI_OFF      16
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_MSK     0x0000ff00
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BIP8_OFF     8
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_MSK 0x000000f0
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BEI_BIAE_OFF 4
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BDI_MSK      0x00000008
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_BDI_OFF      3
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_IAE_MSK      0x00000004
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_IAE_OFF      2
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_MSK      0x00000003
#define OTU_FRM_RX_REG_OTU_SK_SM_OH_BIT_OTU_SK_SM_RES_OFF      0

/* index definitions for PMC_OTU_FRM_RX_REG_OTU_SK_OH2 */
#define PMC_OTU_FRM_RX_REG_OTU_SK_OH2_INDEX_N_MIN     0
#define PMC_OTU_FRM_RX_REG_OTU_SK_OH2_INDEX_N_MAX     11
#define PMC_OTU_FRM_RX_REG_OTU_SK_OH2_INDEX_N_SIZE    12
#define PMC_OTU_FRM_RX_REG_OTU_SK_OH2_INDEX_N_OFFSET  0x100

/*------------------------------------------------.
 | Register (0x000020cc + (N) * 0x100) OTU_SK_OH2 |
 +------------------------------------------------+
 | bit  31:16 R  OTU_SK_GCC0                      |
 | bit  15:0  R  OTU_SK_RES                       |
 +-----------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_OH2_UNUSED_MASK     0x00000000
#define OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_MSK 0xffff0000
#define OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_GCC0_OFF 16
#define OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_MSK  0x0000ffff
#define OTU_FRM_RX_REG_OTU_SK_OH2_BIT_OTU_SK_RES_OFF  0

/*---------------------------------.
 | Register 0x00002f00 OTU_SK_CFG  |
 +---------------------------------+
 | bit  2 R/W  OTU_SK_AC_TTI_FIELD |
 | bit  1 R/W  OTU_SK_TTI_LOCK     |
 | bit  0 R/W  OTU_SK_LOCK         |
 +--------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_CFG_UNUSED_MASK             0x00000000
#define OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_AC_TTI_FIELD_MSK 0x00000004
#define OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_AC_TTI_FIELD_OFF 2
#define OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_TTI_LOCK_MSK     0x00000002
#define OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_TTI_LOCK_OFF     1
#define OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_LOCK_MSK         0x00000001
#define OTU_FRM_RX_REG_OTU_SK_CFG_BIT_OTU_SK_LOCK_OFF         0

/*-------------------------------------------.
 | Register 0x00002f04 OTU_SK_RAM_ECC_BYPASS |
 +-------------------------------------------+
 +------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_RAM_ECC_BYPASS_UNUSED_MASK 0xffffe000

/*-----------------------------------------------.
 | Register 0x00002f08 OTU_SK_RAM_ECC_ERR_INJECT |
 +-----------------------------------------------+
 +----------------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_RAM_ECC_ERR_INJECT_UNUSED_MASK 0xfc000000

/*------------------------------------.
 | Register 0x00002f40 OTU_SK_A_AIS_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_AIS_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_AIS_E_BIT_OTU_SK_A_AIS_E_OFF 0

/*------------------------------------.
 | Register 0x00002f44 OTU_SK_A_SSD_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_SSD_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_SSD_E_BIT_OTU_SK_A_SSD_E_OFF 0

/*------------------------------------.
 | Register 0x00002f48 OTU_SK_A_SSF_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_SSF_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_SSF_E_BIT_OTU_SK_A_SSF_E_OFF 0

/*------------------------------------.
 | Register 0x00002f4c OTU_SK_A_TSF_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_TSF_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_TSF_E_BIT_OTU_SK_A_TSF_E_OFF 0

/*------------------------------------.
 | Register 0x00002f50 OTU_SK_A_TSD_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_TSD_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_TSD_E_BIT_OTU_SK_A_TSD_E_OFF 0

/*-------------------------------------.
 | Register 0x00002f54 OTU_SK_A_BIAE_E |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_BIAE_E      |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BIAE_E_BIT_OTU_SK_A_BIAE_E_OFF 0

/*------------------------------------.
 | Register 0x00002f58 OTU_SK_A_BEI_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_BEI_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BEI_E_BIT_OTU_SK_A_BEI_E_OFF 0

/*------------------------------------.
 | Register 0x00002f5c OTU_SK_A_BDI_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_BDI_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BDI_E_BIT_OTU_SK_A_BDI_E_OFF 0

/*------------------------------------.
 | Register 0x00002f60 OTU_SK_D_IAE_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_IAE_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_IAE_E_BIT_OTU_SK_D_IAE_E_OFF 0

/*------------------------------------.
 | Register 0x00002f64 OTU_SK_D_TIM_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_TIM_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_TIM_E_BIT_OTU_SK_D_TIM_E_OFF 0

/*------------------------------------.
 | Register 0x00002f68 OTU_SK_D_DEG_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_DEG_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_DEG_E_BIT_OTU_SK_D_DEG_E_OFF 0

/*------------------------------------.
 | Register 0x00002f6c OTU_SK_D_BDI_E |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_BDI_E      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_E_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_BDI_E_BIT_OTU_SK_D_BDI_E_OFF 0

/*-------------------------------------.
 | Register 0x00002f70 OTU_SK_D_BIAE_E |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_BIAE_E      |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_BIAE_E_BIT_OTU_SK_D_BIAE_E_OFF 0

/*-------------------------------------.
 | Register 0x00002f74 OTU_SK_CI_SSF_E |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_SK_CI_SSF_E      |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_CI_SSF_E_BIT_OTU_SK_CI_SSF_E_OFF 0

/*------------------------------------------.
 | Register 0x00002f78 OTU_SK_SHADOW_XFER_E |
 +------------------------------------------+
 | bit  0 R/W  OTU_SK_COUNT_DONE_E          |
 +-----------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_UNUSED_MASK             0xfffffffe
#define OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_BIT_OTU_SK_COUNT_DONE_E_MSK 0x00000001
#define OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_E_BIT_OTU_SK_COUNT_DONE_E_OFF 0

/*------------------------------------.
 | Register 0x00002f80 OTU_SK_A_AIS_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_AIS_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_AIS_I_BIT_OTU_SK_A_AIS_I_OFF 0

/*------------------------------------.
 | Register 0x00002f84 OTU_SK_A_SSD_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_SSD_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_SSD_I_BIT_OTU_SK_A_SSD_I_OFF 0

/*------------------------------------.
 | Register 0x00002f88 OTU_SK_A_SSF_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_SSF_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_SSF_I_BIT_OTU_SK_A_SSF_I_OFF 0

/*------------------------------------.
 | Register 0x00002f8c OTU_SK_A_TSF_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_TSF_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_TSF_I_BIT_OTU_SK_A_TSF_I_OFF 0

/*------------------------------------.
 | Register 0x00002f90 OTU_SK_A_TSD_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_TSD_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_TSD_I_BIT_OTU_SK_A_TSD_I_OFF 0

/*-------------------------------------.
 | Register 0x00002f94 OTU_SK_A_BIAE_I |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_BIAE_I      |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BIAE_I_BIT_OTU_SK_A_BIAE_I_OFF 0

/*------------------------------------.
 | Register 0x00002f98 OTU_SK_A_BEI_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_BEI_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BEI_I_BIT_OTU_SK_A_BEI_I_OFF 0

/*------------------------------------.
 | Register 0x00002f9c OTU_SK_A_BDI_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_A_BDI_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BDI_I_BIT_OTU_SK_A_BDI_I_OFF 0

/*------------------------------------.
 | Register 0x00002fa0 OTU_SK_D_IAE_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_IAE_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_IAE_I_BIT_OTU_SK_D_IAE_I_OFF 0

/*------------------------------------.
 | Register 0x00002fa4 OTU_SK_D_TIM_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_TIM_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_TIM_I_BIT_OTU_SK_D_TIM_I_OFF 0

/*------------------------------------.
 | Register 0x00002fa8 OTU_SK_D_DEG_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_DEG_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_DEG_I_BIT_OTU_SK_D_DEG_I_OFF 0

/*------------------------------------.
 | Register 0x00002fac OTU_SK_D_BDI_I |
 +------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_BDI_I      |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_I_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_BDI_I_BIT_OTU_SK_D_BDI_I_OFF 0

/*-------------------------------------.
 | Register 0x00002fb0 OTU_SK_D_BIAE_I |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_SK_D_BIAE_I      |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_BIAE_I_BIT_OTU_SK_D_BIAE_I_OFF 0

/*-------------------------------------.
 | Register 0x00002fb4 OTU_SK_CI_SSF_I |
 +-------------------------------------+
 | bit  11:0 R/W  OTU_SK_CI_SSF_I      |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_CI_SSF_I_BIT_OTU_SK_CI_SSF_I_OFF 0

/*------------------------------------------.
 | Register 0x00002fb8 OTU_SK_SHADOW_XFER_I |
 +------------------------------------------+
 | bit  0 R/W  OTU_SK_COUNT_DONE_I          |
 +-----------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_UNUSED_MASK             0xfffffffe
#define OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_MSK 0x00000001
#define OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_I_BIT_OTU_SK_COUNT_DONE_I_OFF 0

/*------------------------------------.
 | Register 0x00002fc0 OTU_SK_A_AIS_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_AIS_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_AIS_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_AIS_V_BIT_OTU_SK_A_AIS_V_OFF 0

/*------------------------------------.
 | Register 0x00002fc4 OTU_SK_A_SSD_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_SSD_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSD_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_SSD_V_BIT_OTU_SK_A_SSD_V_OFF 0

/*------------------------------------.
 | Register 0x00002fc8 OTU_SK_A_SSF_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_SSF_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_SSF_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_SSF_V_BIT_OTU_SK_A_SSF_V_OFF 0

/*------------------------------------.
 | Register 0x00002fcc OTU_SK_A_TSF_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_TSF_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSF_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_TSF_V_BIT_OTU_SK_A_TSF_V_OFF 0

/*------------------------------------.
 | Register 0x00002fd0 OTU_SK_A_TSD_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_TSD_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_TSD_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_TSD_V_BIT_OTU_SK_A_TSD_V_OFF 0

/*-------------------------------------.
 | Register 0x00002fd4 OTU_SK_A_BIAE_V |
 +-------------------------------------+
 | bit  11:0 R  OTU_SK_A_BIAE_V        |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BIAE_V_BIT_OTU_SK_A_BIAE_V_OFF 0

/*------------------------------------.
 | Register 0x00002fd8 OTU_SK_A_BEI_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_BEI_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BEI_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BEI_V_BIT_OTU_SK_A_BEI_V_OFF 0

/*------------------------------------.
 | Register 0x00002fdc OTU_SK_A_BDI_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_A_BDI_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_A_BDI_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_A_BDI_V_BIT_OTU_SK_A_BDI_V_OFF 0

/*------------------------------------.
 | Register 0x00002fe0 OTU_SK_D_IAE_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_D_IAE_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_IAE_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_IAE_V_BIT_OTU_SK_D_IAE_V_OFF 0

/*------------------------------------.
 | Register 0x00002fe4 OTU_SK_D_TIM_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_D_TIM_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_TIM_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_TIM_V_BIT_OTU_SK_D_TIM_V_OFF 0

/*------------------------------------.
 | Register 0x00002fe8 OTU_SK_D_DEG_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_D_DEG_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_DEG_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_DEG_V_BIT_OTU_SK_D_DEG_V_OFF 0

/*------------------------------------.
 | Register 0x00002fec OTU_SK_D_BDI_V |
 +------------------------------------+
 | bit  11:0 R  OTU_SK_D_BDI_V        |
 +-----------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BDI_V_UNUSED_MASK        0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_BDI_V_BIT_OTU_SK_D_BDI_V_OFF 0

/*-------------------------------------.
 | Register 0x00002ff0 OTU_SK_D_BIAE_V |
 +-------------------------------------+
 | bit  11:0 R  OTU_SK_D_BIAE_V        |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_D_BIAE_V_BIT_OTU_SK_D_BIAE_V_OFF 0

/*-------------------------------------.
 | Register 0x00002ff4 OTU_SK_CI_SSF_V |
 +-------------------------------------+
 | bit  11:0 R  OTU_SK_CI_SSF_V        |
 +------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_UNUSED_MASK         0xfffff000
#define OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_MSK 0x00000fff
#define OTU_FRM_RX_REG_OTU_SK_CI_SSF_V_BIT_OTU_SK_CI_SSF_V_OFF 0

/*------------------------------------------.
 | Register 0x00002ff8 OTU_SK_SHADOW_XFER_V |
 +------------------------------------------+
 | bit  0 R  OTU_SK_COUNT_DONE_V            |
 +-----------------------------------------*/
#define PMC_OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_UNUSED_MASK             0xfffffffe
#define OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_BIT_OTU_SK_COUNT_DONE_V_MSK 0x00000001
#define OTU_FRM_RX_REG_OTU_SK_SHADOW_XFER_V_BIT_OTU_SK_COUNT_DONE_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTU_FRM_RX_REGS_H */
