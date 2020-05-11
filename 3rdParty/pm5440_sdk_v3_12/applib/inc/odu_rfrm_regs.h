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
 *     odu_rfrm block
 *****************************************************************************/
#ifndef _ODU_RFRM_REGS_H
#define _ODU_RFRM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ODU_RFRM_REG_AC_TTI_PID( N )             (0x00000000 + (N) * 0x04)
#define PMC_ODU_RFRM_REG_AC_TTI_PM( N )              (0x00004000 + (N) * 0x04)
#define PMC_ODU_RFRM_REG_EXP_TTI_PID( N )            (0x00008000 + (N) * 0x04)
#define PMC_ODU_RFRM_REG_EXP_TTI_PM( N )             (0x0000a000 + (N) * 0x04)
#define PMC_ODU_RFRM_REG_FTFL( N )                   (0x0000b000 + (N) * 0x04)
#define PMC_ODU_RFRM_REG_AC_APS_PCC( N )             (0x0000c000 + (N) * 0x04)
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PID( N )        (0x0000cc00 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PID( N )        (0x0000cc04 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PID( N )      (0x0000cc20 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PID( N )       (0x0000cc24 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID( N )    (0x0000cc28 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PID( N )          (0x0000cc2c + (N) * 0x40)
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PM( N )         (0x00010c00 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PM( N )         (0x00010c04 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_TCM1_2_3_PID( N )           (0x00010c08 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_TCM4_5_6_PID( N )           (0x00010c0c + (N) * 0x40)
#define PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID( N )    (0x00010c10 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL0( N )          (0x00010c14 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL1( N )          (0x00010c18 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_ROW4_OH1( N )               (0x00010c1c + (N) * 0x40)
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PM( N )       (0x00010c20 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PM( N )        (0x00010c24 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM( N )     (0x00010c28 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PM( N )           (0x00010c2c + (N) * 0x40)
#define PMC_ODU_RFRM_REG_AC_STAT( N )                (0x00010c30 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_ROW2_OH( N )                (0x00010c34 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_ROW3_4_OH( N )              (0x00010c38 + (N) * 0x40)
#define PMC_ODU_RFRM_REG_ROW4_OH0( N )               (0x00010c3c + (N) * 0x40)
#define PMC_ODU_RFRM_REG_DEFECT_INT_STAT             0x00012400
#define PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0 0x00012404
#define PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1 0x00012408
#define PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2 0x0001240c
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0     0x00012410
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1     0x00012414
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2     0x00012418
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3     0x0001241c
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4     0x00012420
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5     0x00012424
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6     0x00012428
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7     0x0001242c
#define PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT    0x00012430
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0  0x0001243c
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1  0x00012440
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2  0x00012444
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0      0x00012448
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1      0x0001244c
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2      0x00012450
#define PMC_ODU_RFRM_REG_DEFECT_INT_EN               0x00012454
#define PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN      0x00012458
#define PMC_ODU_RFRM_REG_GLOBAL_CONFIG               0x00012464

/* index definitions for PMC_ODU_RFRM_REG_AC_TTI_PID */
#define PMC_ODU_RFRM_REG_AC_TTI_PID_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_AC_TTI_PID_INDEX_N_MAX    4095
#define PMC_ODU_RFRM_REG_AC_TTI_PID_INDEX_N_SIZE   4096
#define PMC_ODU_RFRM_REG_AC_TTI_PID_INDEX_N_OFFSET 0x04

/*-----------------------------------------------.
 | Register (0x00000000 + (N) * 0x04) AC_TTI_PID |
 +-----------------------------------------------+
 | bit  31:0 R  AC_TTI                           |
 +----------------------------------------------*/
#define PMC_ODU_RFRM_REG_AC_TTI_PID_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_AC_TTI_PID_BIT_AC_TTI_MSK     0xffffffff
#define ODU_RFRM_REG_AC_TTI_PID_BIT_AC_TTI_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_AC_TTI_PM */
#define PMC_ODU_RFRM_REG_AC_TTI_PM_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_AC_TTI_PM_INDEX_N_MAX    1535
#define PMC_ODU_RFRM_REG_AC_TTI_PM_INDEX_N_SIZE   1536
#define PMC_ODU_RFRM_REG_AC_TTI_PM_INDEX_N_OFFSET 0x04

/*----------------------------------------------.
 | Register (0x00004000 + (N) * 0x04) AC_TTI_PM |
 +----------------------------------------------+
 | bit  31:0 R  AC_TTI_PM                       |
 +---------------------------------------------*/
#define PMC_ODU_RFRM_REG_AC_TTI_PM_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_AC_TTI_PM_BIT_AC_TTI_PM_MSK  0xffffffff
#define ODU_RFRM_REG_AC_TTI_PM_BIT_AC_TTI_PM_OFF  0

/* index definitions for PMC_ODU_RFRM_REG_EXP_TTI_PID */
#define PMC_ODU_RFRM_REG_EXP_TTI_PID_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_EXP_TTI_PID_INDEX_N_MAX    2047
#define PMC_ODU_RFRM_REG_EXP_TTI_PID_INDEX_N_SIZE   2048
#define PMC_ODU_RFRM_REG_EXP_TTI_PID_INDEX_N_OFFSET 0x04

/*------------------------------------------------.
 | Register (0x00008000 + (N) * 0x04) EXP_TTI_PID |
 +------------------------------------------------+
 | bit  31:0 R/W  EX_TTI                          |
 +-----------------------------------------------*/
#define PMC_ODU_RFRM_REG_EXP_TTI_PID_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_EXP_TTI_PID_BIT_EX_TTI_MSK     0xffffffff
#define ODU_RFRM_REG_EXP_TTI_PID_BIT_EX_TTI_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_EXP_TTI_PM */
#define PMC_ODU_RFRM_REG_EXP_TTI_PM_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_EXP_TTI_PM_INDEX_N_MAX    767
#define PMC_ODU_RFRM_REG_EXP_TTI_PM_INDEX_N_SIZE   768
#define PMC_ODU_RFRM_REG_EXP_TTI_PM_INDEX_N_OFFSET 0x04

/*-----------------------------------------------.
 | Register (0x0000a000 + (N) * 0x04) EXP_TTI_PM |
 +-----------------------------------------------+
 | bit  31:0 R/W  EX_TTI_PM                      |
 +----------------------------------------------*/
#define PMC_ODU_RFRM_REG_EXP_TTI_PM_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_EXP_TTI_PM_BIT_EX_TTI_PM_MSK  0xffffffff
#define ODU_RFRM_REG_EXP_TTI_PM_BIT_EX_TTI_PM_OFF  0

/* index definitions for PMC_ODU_RFRM_REG_FTFL */
#define PMC_ODU_RFRM_REG_FTFL_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_FTFL_INDEX_N_MAX    767
#define PMC_ODU_RFRM_REG_FTFL_INDEX_N_SIZE   768
#define PMC_ODU_RFRM_REG_FTFL_INDEX_N_OFFSET 0x04

/*-----------------------------------------.
 | Register (0x0000b000 + (N) * 0x04) FTFL |
 +-----------------------------------------+
 | bit  31:0 R  FTFL                       |
 +----------------------------------------*/
#define PMC_ODU_RFRM_REG_FTFL_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_FTFL_BIT_FTFL_MSK       0xffffffff
#define ODU_RFRM_REG_FTFL_BIT_FTFL_OFF       0

/* index definitions for PMC_ODU_RFRM_REG_AC_APS_PCC */
#define PMC_ODU_RFRM_REG_AC_APS_PCC_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_AC_APS_PCC_INDEX_N_MAX    767
#define PMC_ODU_RFRM_REG_AC_APS_PCC_INDEX_N_SIZE   768
#define PMC_ODU_RFRM_REG_AC_APS_PCC_INDEX_N_OFFSET 0x04

/*-----------------------------------------------.
 | Register (0x0000c000 + (N) * 0x04) AC_APS_PCC |
 +-----------------------------------------------+
 | bit  31:0 R  AC_APS                           |
 +----------------------------------------------*/
#define PMC_ODU_RFRM_REG_AC_APS_PCC_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_AC_APS_PCC_BIT_AC_APS_MSK     0xffffffff
#define ODU_RFRM_REG_AC_APS_PCC_BIT_AC_APS_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_LAYER_CTRL0_PID */
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PID_INDEX_N_MIN     0
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PID_INDEX_N_MAX     255
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PID_INDEX_N_SIZE    256
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PID_INDEX_N_OFFSET  0x40

/*----------------------------------------------------.
 | Register (0x0000cc00 + (N) * 0x40) LAYER_CTRL0_PID |
 +----------------------------------------------------+
 | bit  27    R/W  LTC_ACT_DIS                        |
 | bit  26    R/W  TIM_ACT_DIS                        |
 | bit  25:24 R/W  TIM_DET_MO                         |
 | bit  23:20 R/W  DEG_M                              |
 | bit  19:0  R/W  DEG_THR                            |
 +---------------------------------------------------*/
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PID_UNUSED_MASK     0xf0000000
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_LTC_ACT_DIS_MSK 0x08000000
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_LTC_ACT_DIS_OFF 27
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_ACT_DIS_MSK 0x04000000
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_ACT_DIS_OFF 26
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_MSK  0x03000000
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF  24
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_MSK       0x00f00000
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF       20
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_MSK     0x000fffff
#define ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_LAYER_CTRL1_PID */
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PID_INDEX_N_MIN        0
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PID_INDEX_N_MAX        255
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PID_INDEX_N_SIZE       256
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PID_INDEX_N_OFFSET     0x40

/*----------------------------------------------------.
 | Register (0x0000cc04 + (N) * 0x40) LAYER_CTRL1_PID |
 +----------------------------------------------------+
 | bit  20    R/W  INT_EN                             |
 | bit  19    R/W  AIS_DIS                            |
 | bit  18    R/W  BW_DIS                             |
 | bit  17    R/W  TSD_DIS                            |
 | bit  16    R/W  TSF_DIS                            |
 | bit  15:14 R/W  MODE                               |
 | bit  13:12 R/W  TCM_MAINT_SIG                      |
 | bit  8     R/W  PARENT_IS_DCI                      |
 | bit  7:0   R/W  PARENT_POOL_ID                     |
 +---------------------------------------------------*/
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PID_UNUSED_MASK        0xffe00e00
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_INT_EN_MSK         0x00100000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_INT_EN_OFF         20
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_AIS_DIS_MSK        0x00080000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_AIS_DIS_OFF        19
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_BW_DIS_MSK         0x00040000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_BW_DIS_OFF         18
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSD_DIS_MSK        0x00020000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSD_DIS_OFF        17
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSF_DIS_MSK        0x00010000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSF_DIS_OFF        16
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_MSK           0x0000c000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF           14
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_MSK  0x00003000
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF  12
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_IS_DCI_MSK  0x00000100
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_IS_DCI_OFF  8
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_MSK 0x000000ff
#define ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF 0

/* index definitions for PMC_ODU_RFRM_REG_NEAR_END_PMON_PID */
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PID_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PID_INDEX_N_MAX    255
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PID_INDEX_N_SIZE   256
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PID_INDEX_N_OFFSET 0x40

/*------------------------------------------------------.
 | Register (0x0000cc20 + (N) * 0x40) NEAR_END_PMON_PID |
 +------------------------------------------------------+
 | bit  21   R  PIAE                                    |
 | bit  20   R  PN_DS                                   |
 | bit  19:0 R  PN_EBC                                  |
 +-----------------------------------------------------*/
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PID_UNUSED_MASK    0xffc00000
#define ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PIAE_MSK       0x00200000
#define ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PIAE_OFF       21
#define ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_DS_MSK      0x00100000
#define ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_DS_OFF      20
#define ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_MSK     0x000fffff
#define ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_FAR_END_PMON_PID */
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PID_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PID_INDEX_N_MAX    255
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PID_INDEX_N_SIZE   256
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PID_INDEX_N_OFFSET 0x40

/*-----------------------------------------------------.
 | Register (0x0000cc24 + (N) * 0x40) FAR_END_PMON_PID |
 +-----------------------------------------------------+
 | bit  21   R  PBIAE                                  |
 | bit  20   R  PF_DS                                  |
 | bit  19:0 R  PF_EBC                                 |
 +----------------------------------------------------*/
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PID_UNUSED_MASK    0xffc00000
#define ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PBIAE_MSK      0x00200000
#define ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PBIAE_OFF      21
#define ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_DS_MSK      0x00100000
#define ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_DS_OFF      20
#define ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_MSK     0x000fffff
#define ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID */
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID_INDEX_N_MAX    255
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID_INDEX_N_SIZE   256
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID_INDEX_N_OFFSET 0x40

/*--------------------------------------------------------.
 | Register (0x0000cc28 + (N) * 0x40) DEFECTS_ACTIONS_PID |
 +--------------------------------------------------------+
 | bit  31 R  ABDI_V                                      |
 | bit  30 R  AAIS_V                                      |
 | bit  29 R  ATSD_V                                      |
 | bit  28 R  ATSF_V                                      |
 | bit  25 R  CI_SSF_V                                    |
 | bit  24 R  DIAE_V                                      |
 | bit  23 R  DLTC_V                                      |
 | bit  22 R  DBIAE_V                                     |
 | bit  21 R  DBDI_V                                      |
 | bit  20 R  DDEG_V                                      |
 | bit  19 R  DTIM_V                                      |
 | bit  18 R  DAIS_V                                      |
 | bit  17 R  DOCI_V                                      |
 | bit  16 R  DLCK_V                                      |
 | bit  15 R  ABDI_I                                      |
 | bit  14 R  AAIS_I                                      |
 | bit  13 R  ATSD_I                                      |
 | bit  12 R  ATSF_I                                      |
 | bit  8  R  DIAE_I                                      |
 | bit  7  R  DLTC_I                                      |
 | bit  6  R  DBIAE_I                                     |
 | bit  5  R  DBDI_I                                      |
 | bit  4  R  DDEG_I                                      |
 | bit  3  R  DTIM_I                                      |
 | bit  2  R  DAIS_I                                      |
 | bit  1  R  DOCI_I                                      |
 | bit  0  R  DLCK_I                                      |
 +-------------------------------------------------------*/
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID_UNUSED_MASK    0x0c000e00
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_V_MSK     0x80000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_V_OFF     31
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_V_MSK     0x40000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_V_OFF     30
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_V_MSK     0x20000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_V_OFF     29
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_V_MSK     0x10000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_V_OFF     28
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_CI_SSF_V_MSK   0x02000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_CI_SSF_V_OFF   25
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_V_MSK     0x01000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_V_OFF     24
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_V_MSK     0x00800000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_V_OFF     23
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_V_MSK    0x00400000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_V_OFF    22
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_V_MSK     0x00200000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_V_OFF     21
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_V_MSK     0x00100000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_V_OFF     20
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_V_MSK     0x00080000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_V_OFF     19
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_V_MSK     0x00040000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_V_OFF     18
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_V_MSK     0x00020000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_V_OFF     17
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_V_MSK     0x00010000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_V_OFF     16
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_I_MSK     0x00008000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_I_OFF     15
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_I_MSK     0x00004000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_I_OFF     14
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_I_MSK     0x00002000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_I_OFF     13
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_I_MSK     0x00001000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_I_OFF     12
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_I_MSK     0x00000100
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_I_OFF     8
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_I_MSK     0x00000080
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_I_OFF     7
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_I_MSK    0x00000040
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_I_OFF    6
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_I_MSK     0x00000020
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_I_OFF     5
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_I_MSK     0x00000010
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_I_OFF     4
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_I_MSK     0x00000008
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_I_OFF     3
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_I_MSK     0x00000004
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_I_OFF     2
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_I_MSK     0x00000002
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_I_OFF     1
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_I_MSK     0x00000001
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_I_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_NE_FE_EBC_PID */
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PID_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PID_INDEX_N_MAX    255
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PID_INDEX_N_SIZE   256
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PID_INDEX_N_OFFSET 0x40

/*--------------------------------------------------.
 | Register (0x0000cc2c + (N) * 0x40) NE_FE_EBC_PID |
 +--------------------------------------------------+
 | bit  28:16 R  FE_EBC                             |
 | bit  12:0  R  NE_EBC                             |
 +-------------------------------------------------*/
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PID_UNUSED_MASK    0xe000e000
#define ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_MSK     0x1fff0000
#define ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_OFF     16
#define ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_MSK     0x00001fff
#define ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_LAYER_CTRL0_PM */
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PM_INDEX_N_MIN        0
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PM_INDEX_N_MAX        95
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PM_INDEX_N_SIZE       96
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PM_INDEX_N_OFFSET     0x40

/*---------------------------------------------------.
 | Register (0x00010c00 + (N) * 0x40) LAYER_CTRL0_PM |
 +---------------------------------------------------+
 | bit  26    R/W  TIM_ACT_DIS_PM                    |
 | bit  25:24 R/W  TIM_DET_MO_PM                     |
 | bit  23:20 R/W  DEG_M_PM                          |
 | bit  19:0  R/W  DEG_THR_PM                        |
 +--------------------------------------------------*/
#define PMC_ODU_RFRM_REG_LAYER_CTRL0_PM_UNUSED_MASK        0xf8000000
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_ACT_DIS_PM_MSK 0x04000000
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_ACT_DIS_PM_OFF 26
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_MSK  0x03000000
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF  24
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_MSK       0x00f00000
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF       20
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_MSK     0x000fffff
#define ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF     0

/* index definitions for PMC_ODU_RFRM_REG_LAYER_CTRL1_PM */
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PM_INDEX_N_MIN           0
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PM_INDEX_N_MAX           95
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PM_INDEX_N_SIZE          96
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PM_INDEX_N_OFFSET        0x40

/*---------------------------------------------------.
 | Register (0x00010c04 + (N) * 0x40) LAYER_CTRL1_PM |
 +---------------------------------------------------+
 | bit  20    R/W  INT_EN_PM                         |
 | bit  18    R/W  BW_DIS_PM                         |
 | bit  17    R/W  TSD_DIS_PM                        |
 | bit  16    R/W  TSF_DIS_PM                        |
 | bit  15:14 R/W  MODE_PM                           |
 | bit  8     R/W  PARENT_IS_DCI_PM                  |
 | bit  7:0   R/W  PARENT_POOL_ID_PM                 |
 +--------------------------------------------------*/
#define PMC_ODU_RFRM_REG_LAYER_CTRL1_PM_UNUSED_MASK           0xffe83e00
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_INT_EN_PM_MSK         0x00100000
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_INT_EN_PM_OFF         20
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_BW_DIS_PM_MSK         0x00040000
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_BW_DIS_PM_OFF         18
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSD_DIS_PM_MSK        0x00020000
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSD_DIS_PM_OFF        17
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSF_DIS_PM_MSK        0x00010000
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSF_DIS_PM_OFF        16
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_MSK           0x0000c000
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF           14
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_IS_DCI_PM_MSK  0x00000100
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_IS_DCI_PM_OFF  8
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_MSK 0x000000ff
#define ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF 0

/* index definitions for PMC_ODU_RFRM_REG_TCM1_2_3_PID */
#define PMC_ODU_RFRM_REG_TCM1_2_3_PID_INDEX_N_MIN      0
#define PMC_ODU_RFRM_REG_TCM1_2_3_PID_INDEX_N_MAX      95
#define PMC_ODU_RFRM_REG_TCM1_2_3_PID_INDEX_N_SIZE     96
#define PMC_ODU_RFRM_REG_TCM1_2_3_PID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x00010c08 + (N) * 0x40) TCM1_2_3_PID |
 +-------------------------------------------------+
 | bit  30    R/W  TCM3_WR_EN                      |
 | bit  29    R/W  TCM2_WR_EN                      |
 | bit  28    R/W  TCM1_WR_EN                      |
 | bit  23:16 R/W  TCM3_POOL_ID                    |
 | bit  15:8  R/W  TCM2_POOL_ID                    |
 | bit  7:0   R/W  TCM1_POOL_ID                    |
 +------------------------------------------------*/
#define PMC_ODU_RFRM_REG_TCM1_2_3_PID_UNUSED_MASK      0x8f000000
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_WR_EN_MSK   0x40000000
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_WR_EN_OFF   30
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_WR_EN_MSK   0x20000000
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_WR_EN_OFF   29
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_WR_EN_MSK   0x10000000
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_WR_EN_OFF   28
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_MSK 0x00ff0000
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF 16
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_MSK 0x0000ff00
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF 8
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_MSK 0x000000ff
#define ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF 0

/* index definitions for PMC_ODU_RFRM_REG_TCM4_5_6_PID */
#define PMC_ODU_RFRM_REG_TCM4_5_6_PID_INDEX_N_MIN      0
#define PMC_ODU_RFRM_REG_TCM4_5_6_PID_INDEX_N_MAX      95
#define PMC_ODU_RFRM_REG_TCM4_5_6_PID_INDEX_N_SIZE     96
#define PMC_ODU_RFRM_REG_TCM4_5_6_PID_INDEX_N_OFFSET   0x40

/*-------------------------------------------------.
 | Register (0x00010c0c + (N) * 0x40) TCM4_5_6_PID |
 +-------------------------------------------------+
 | bit  30    R/W  TCM6_WR_EN                      |
 | bit  29    R/W  TCM5_WR_EN                      |
 | bit  28    R/W  TCM4_WR_EN                      |
 | bit  23:16 R/W  TCM6_POOL_ID                    |
 | bit  15:8  R/W  TCM5_POOL_ID                    |
 | bit  7:0   R/W  TCM4_POOL_ID                    |
 +------------------------------------------------*/
#define PMC_ODU_RFRM_REG_TCM4_5_6_PID_UNUSED_MASK      0x8f000000
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_WR_EN_MSK   0x40000000
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_WR_EN_OFF   30
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_WR_EN_MSK   0x20000000
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_WR_EN_OFF   29
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_WR_EN_MSK   0x10000000
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_WR_EN_OFF   28
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_MSK 0x00ff0000
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF 16
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_MSK 0x0000ff00
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF 8
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_MSK 0x000000ff
#define ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF 0

/* index definitions for PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID */
#define PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID_INDEX_N_MIN              0
#define PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID_INDEX_N_MAX              95
#define PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID_INDEX_N_SIZE             96
#define PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID_INDEX_N_OFFSET           0x40

/*--------------------------------------------------------.
 | Register (0x00010c10 + (N) * 0x40) FLOATING_PM_NIM_PID |
 +--------------------------------------------------------+
 | bit  29   R/W  FLOATING_TCM_WR_EN                      |
 | bit  28   R/W  PM_NIM_WR_EN                            |
 | bit  15:8 R/W  FLOATING_TCM_POOL_ID                    |
 | bit  7:0  R/W  PM_NIM_POOL_ID                          |
 +-------------------------------------------------------*/
#define PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID_UNUSED_MASK              0xcfff0000
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_WR_EN_MSK   0x20000000
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_WR_EN_OFF   29
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_WR_EN_MSK         0x10000000
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_WR_EN_OFF         28
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_MSK 0x0000ff00
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF 8
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_MSK       0x000000ff
#define ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF       0

/* index definitions for PMC_ODU_RFRM_REG_CHANNEL_CTRL0 */
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL0_INDEX_N_MIN            0
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL0_INDEX_N_MAX            95
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL0_INDEX_N_SIZE           96
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL0_INDEX_N_OFFSET         0x40

/*--------------------------------------------------.
 | Register (0x00010c14 + (N) * 0x40) CHANNEL_CTRL0 |
 +--------------------------------------------------+
 | bit  25   R/W  TCM6_REMOVE_OH                    |
 | bit  24   R/W  TCM5_REMOVE_OH                    |
 | bit  23   R/W  TCM4_REMOVE_OH                    |
 | bit  22   R/W  TCM3_REMOVE_OH                    |
 | bit  21   R/W  TCM2_REMOVE_OH                    |
 | bit  20   R/W  TCM1_REMOVE_OH                    |
 | bit  18   R/W  PM_NIM_TRANSPARENT                |
 | bit  17   R/W  TCM6_TRANSPARENT                  |
 | bit  16   R/W  TCM5_TRANSPARENT                  |
 | bit  15   R/W  TCM4_TRANSPARENT                  |
 | bit  14   R/W  TCM3_TRANSPARENT                  |
 | bit  13   R/W  TCM2_TRANSPARENT                  |
 | bit  12   R/W  TCM1_TRANSPARENT                  |
 | bit  10:8 R/W  FLOATING_TCM_ID                   |
 | bit  3    R/W  LAST_IS_DCI                       |
 | bit  2:0  R/W  LAST_LAYER                        |
 +-------------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL0_UNUSED_MASK            0xfc0808f0
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_REMOVE_OH_MSK     0x02000000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_REMOVE_OH_OFF     25
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_REMOVE_OH_MSK     0x01000000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_REMOVE_OH_OFF     24
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_REMOVE_OH_MSK     0x00800000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_REMOVE_OH_OFF     23
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_REMOVE_OH_MSK     0x00400000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_REMOVE_OH_OFF     22
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_REMOVE_OH_MSK     0x00200000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_REMOVE_OH_OFF     21
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_REMOVE_OH_MSK     0x00100000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_REMOVE_OH_OFF     20
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_PM_NIM_TRANSPARENT_MSK 0x00040000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_PM_NIM_TRANSPARENT_OFF 18
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_TRANSPARENT_MSK   0x00020000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_TRANSPARENT_OFF   17
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_TRANSPARENT_MSK   0x00010000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_TRANSPARENT_OFF   16
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_TRANSPARENT_MSK   0x00008000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_TRANSPARENT_OFF   15
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_TRANSPARENT_MSK   0x00004000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_TRANSPARENT_OFF   14
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_TRANSPARENT_MSK   0x00002000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_TRANSPARENT_OFF   13
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_TRANSPARENT_MSK   0x00001000
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_TRANSPARENT_OFF   12
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_MSK    0x00000700
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF    8
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_IS_DCI_MSK        0x00000008
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_IS_DCI_OFF        3
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_MSK         0x00000007
#define ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF         0

/* index definitions for PMC_ODU_RFRM_REG_CHANNEL_CTRL1 */
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL1_INDEX_N_MIN        0
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL1_INDEX_N_MAX        95
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL1_INDEX_N_SIZE       96
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL1_INDEX_N_OFFSET     0x40

/*--------------------------------------------------.
 | Register (0x00010c18 + (N) * 0x40) CHANNEL_CTRL1 |
 +--------------------------------------------------+
 | bit  4   R/W  LOFLOM_ACT_DIS                     |
 | bit  3   R/W  FAU_PASSTHRU                       |
 | bit  2:1 R/W  MAINT_SIG                          |
 | bit  0   R/W  PASSTHRU                           |
 +-------------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_CTRL1_UNUSED_MASK        0xffffffe0
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_LOFLOM_ACT_DIS_MSK 0x00000010
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_LOFLOM_ACT_DIS_OFF 4
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_FAU_PASSTHRU_MSK   0x00000008
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_FAU_PASSTHRU_OFF   3
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_MSK      0x00000006
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF      1
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_PASSTHRU_MSK       0x00000001
#define ODU_RFRM_REG_CHANNEL_CTRL1_BIT_PASSTHRU_OFF       0

/* index definitions for PMC_ODU_RFRM_REG_ROW4_OH1 */
#define PMC_ODU_RFRM_REG_ROW4_OH1_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_ROW4_OH1_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_ROW4_OH1_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_ROW4_OH1_INDEX_N_OFFSET 0x40

/*---------------------------------------------.
 | Register (0x00010c1c + (N) * 0x40) ROW4_OH1 |
 +---------------------------------------------+
 | bit  31:16 R  GCC2                          |
 | bit  15:0  R  GCC1                          |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_ROW4_OH1_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_MSK       0xffff0000
#define ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_OFF       16
#define ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_MSK       0x0000ffff
#define ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_OFF       0

/* index definitions for PMC_ODU_RFRM_REG_NEAR_END_PMON_PM */
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PM_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PM_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PM_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PM_INDEX_N_OFFSET 0x40

/*-----------------------------------------------------.
 | Register (0x00010c20 + (N) * 0x40) NEAR_END_PMON_PM |
 +-----------------------------------------------------+
 | bit  20   R  PN_DS_PM                               |
 | bit  19:0 R  PN_EBC_PM                              |
 +----------------------------------------------------*/
#define PMC_ODU_RFRM_REG_NEAR_END_PMON_PM_UNUSED_MASK    0xffe00000
#define ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_DS_PM_MSK   0x00100000
#define ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_DS_PM_OFF   20
#define ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_MSK  0x000fffff
#define ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_OFF  0

/* index definitions for PMC_ODU_RFRM_REG_FAR_END_PMON_PM */
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PM_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PM_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PM_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PM_INDEX_N_OFFSET 0x40

/*----------------------------------------------------.
 | Register (0x00010c24 + (N) * 0x40) FAR_END_PMON_PM |
 +----------------------------------------------------+
 | bit  20   R  PF_DS_PM                              |
 | bit  19:0 R  PF_EBC_PM                             |
 +---------------------------------------------------*/
#define PMC_ODU_RFRM_REG_FAR_END_PMON_PM_UNUSED_MASK    0xffe00000
#define ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_DS_PM_MSK   0x00100000
#define ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_DS_PM_OFF   20
#define ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_MSK  0x000fffff
#define ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_OFF  0

/* index definitions for PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM */
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM_INDEX_N_MIN     0
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM_INDEX_N_MAX     95
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM_INDEX_N_SIZE    96
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM_INDEX_N_OFFSET  0x40

/*-------------------------------------------------------.
 | Register (0x00010c28 + (N) * 0x40) DEFECTS_ACTIONS_PM |
 +-------------------------------------------------------+
 | bit  31 R  ABDI_PM_V                                  |
 | bit  29 R  ATSD_PM_V                                  |
 | bit  28 R  ATSF_PM_V                                  |
 | bit  25 R  CI_SSF_PM_V                                |
 | bit  24 R  DLOFLOM_V                                  |
 | bit  23 R  OOF_V                                      |
 | bit  21 R  DBDI_PM_V                                  |
 | bit  20 R  DDEG_PM_V                                  |
 | bit  19 R  DTIM_PM_V                                  |
 | bit  18 R  DAIS_PM_V                                  |
 | bit  17 R  DOCI_PM_V                                  |
 | bit  16 R  DLCK_PM_V                                  |
 | bit  15 R  ABDI_PM_I                                  |
 | bit  13 R  ATSD_PM_I                                  |
 | bit  12 R  ATSF_PM_I                                  |
 | bit  8  R  DLOFLOM_I                                  |
 | bit  7  R  OOF_I                                      |
 | bit  5  R  DBDI_PM_I                                  |
 | bit  4  R  DDEG_PM_I                                  |
 | bit  3  R  DTIM_PM_I                                  |
 | bit  2  R  DAIS_PM_I                                  |
 | bit  1  R  DOCI_PM_I                                  |
 | bit  0  R  DLCK_PM_I                                  |
 +------------------------------------------------------*/
#define PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM_UNUSED_MASK     0x4c404e40
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_V_MSK   0x80000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_V_OFF   31
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_V_MSK   0x20000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_V_OFF   29
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_V_MSK   0x10000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_V_OFF   28
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_CI_SSF_PM_V_MSK 0x02000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_CI_SSF_PM_V_OFF 25
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_V_MSK   0x01000000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_V_OFF   24
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_V_MSK       0x00800000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_V_OFF       23
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_V_MSK   0x00200000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_V_OFF   21
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_V_MSK   0x00100000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_V_OFF   20
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_V_MSK   0x00080000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_V_OFF   19
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_V_MSK   0x00040000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_V_OFF   18
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_V_MSK   0x00020000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_V_OFF   17
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_V_MSK   0x00010000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_V_OFF   16
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_I_MSK   0x00008000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_I_OFF   15
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_I_MSK   0x00002000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_I_OFF   13
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_I_MSK   0x00001000
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_I_OFF   12
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_I_MSK   0x00000100
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_I_OFF   8
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_I_MSK       0x00000080
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_I_OFF       7
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_I_MSK   0x00000020
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_I_OFF   5
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_I_MSK   0x00000010
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_I_OFF   4
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_I_MSK   0x00000008
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_I_OFF   3
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_I_MSK   0x00000004
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_I_OFF   2
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_I_MSK   0x00000002
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_I_OFF   1
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_I_MSK   0x00000001
#define ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_I_OFF   0

/* index definitions for PMC_ODU_RFRM_REG_NE_FE_EBC_PM */
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PM_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PM_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PM_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PM_INDEX_N_OFFSET 0x40

/*-------------------------------------------------.
 | Register (0x00010c2c + (N) * 0x40) NE_FE_EBC_PM |
 +-------------------------------------------------+
 | bit  28:16 R  FE_EBC_PM                         |
 | bit  12:0  R  NE_EBC_PM                         |
 +------------------------------------------------*/
#define PMC_ODU_RFRM_REG_NE_FE_EBC_PM_UNUSED_MASK    0xe000e000
#define ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_MSK  0x1fff0000
#define ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_OFF  16
#define ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_MSK  0x00001fff
#define ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_OFF  0

/* index definitions for PMC_ODU_RFRM_REG_AC_STAT */
#define PMC_ODU_RFRM_REG_AC_STAT_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_AC_STAT_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_AC_STAT_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_AC_STAT_INDEX_N_OFFSET 0x40

/*--------------------------------------------.
 | Register (0x00010c30 + (N) * 0x40) AC_STAT |
 +--------------------------------------------+
 | bit  26:24 R  AC_TCM6                      |
 | bit  22:20 R  AC_TCM5                      |
 | bit  18:16 R  AC_TCM4                      |
 | bit  14:12 R  AC_TCM3                      |
 | bit  10:8  R  AC_TCM2                      |
 | bit  6:4   R  AC_TCM1                      |
 | bit  2:0   R  AC_PM                        |
 +-------------------------------------------*/
#define PMC_ODU_RFRM_REG_AC_STAT_UNUSED_MASK    0xf8888888
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_MSK    0x07000000
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_OFF    24
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_MSK    0x00700000
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_OFF    20
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_MSK    0x00070000
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_OFF    16
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_MSK    0x00007000
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_OFF    12
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_MSK    0x00000700
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_OFF    8
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_MSK    0x00000070
#define ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_OFF    4
#define ODU_RFRM_REG_AC_STAT_BIT_AC_PM_MSK      0x00000007
#define ODU_RFRM_REG_AC_STAT_BIT_AC_PM_OFF      0

/* index definitions for PMC_ODU_RFRM_REG_ROW2_OH */
#define PMC_ODU_RFRM_REG_ROW2_OH_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_ROW2_OH_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_ROW2_OH_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_ROW2_OH_INDEX_N_OFFSET 0x40

/*--------------------------------------------.
 | Register (0x00010c34 + (N) * 0x40) ROW2_OH |
 +--------------------------------------------+
 | bit  31:24 R  PM_TCM                       |
 | bit  23:16 R  TCM_ACT                      |
 | bit  15:0  R  RES1                         |
 +-------------------------------------------*/
#define PMC_ODU_RFRM_REG_ROW2_OH_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_MSK     0xff000000
#define ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_OFF     24
#define ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK    0x00ff0000
#define ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF    16
#define ODU_RFRM_REG_ROW2_OH_BIT_RES1_MSK       0x0000ffff
#define ODU_RFRM_REG_ROW2_OH_BIT_RES1_OFF       0

/* index definitions for PMC_ODU_RFRM_REG_ROW3_4_OH */
#define PMC_ODU_RFRM_REG_ROW3_4_OH_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_ROW3_4_OH_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_ROW3_4_OH_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_ROW3_4_OH_INDEX_N_OFFSET 0x40

/*----------------------------------------------.
 | Register (0x00010c38 + (N) * 0x40) ROW3_4_OH |
 +----------------------------------------------+
 | bit  31:16 R  RES2                           |
 | bit  15:0  R  EXP                            |
 +---------------------------------------------*/
#define PMC_ODU_RFRM_REG_ROW3_4_OH_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_MSK       0xffff0000
#define ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_OFF       16
#define ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_MSK        0x0000ffff
#define ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_OFF        0

/* index definitions for PMC_ODU_RFRM_REG_ROW4_OH0 */
#define PMC_ODU_RFRM_REG_ROW4_OH0_INDEX_N_MIN    0
#define PMC_ODU_RFRM_REG_ROW4_OH0_INDEX_N_MAX    95
#define PMC_ODU_RFRM_REG_ROW4_OH0_INDEX_N_SIZE   96
#define PMC_ODU_RFRM_REG_ROW4_OH0_INDEX_N_OFFSET 0x40

/*---------------------------------------------.
 | Register (0x00010c3c + (N) * 0x40) ROW4_OH0 |
 +---------------------------------------------+
 | bit  31:0 R  RES3                           |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_ROW4_OH0_UNUSED_MASK    0x00000000
#define ODU_RFRM_REG_ROW4_OH0_BIT_RES3_MSK       0xffffffff
#define ODU_RFRM_REG_ROW4_OH0_BIT_RES3_OFF       0

/*-------------------------------------.
 | Register 0x00012400 DEFECT_INT_STAT |
 +-------------------------------------+
 | bit  15 R/W  COMB_ABDI_I            |
 | bit  14 R/W  COMB_AAIS_I            |
 | bit  13 R/W  COMB_ATSD_I            |
 | bit  12 R/W  COMB_ATSF_I            |
 | bit  10 R/W  COMB_OOF_I             |
 | bit  9  R/W  COMB_DLOFLOM_I         |
 | bit  8  R/W  COMB_DBIAE_I           |
 | bit  7  R/W  COMB_DIAE_I            |
 | bit  6  R/W  COMB_DLTC_I            |
 | bit  5  R/W  COMB_DBDI_I            |
 | bit  4  R/W  COMB_DDEG_I            |
 | bit  3  R/W  COMB_DTIM_I            |
 | bit  2  R/W  COMB_DAIS_I            |
 | bit  1  R/W  COMB_DOCI_I            |
 | bit  0  R/W  COMB_DLCK_I            |
 +------------------------------------*/
#define PMC_ODU_RFRM_REG_DEFECT_INT_STAT_UNUSED_MASK        0xffff0800
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_MSK    0x00008000
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_OFF    15
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_MSK    0x00004000
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_OFF    14
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_MSK    0x00002000
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_OFF    13
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_MSK    0x00001000
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_OFF    12
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_MSK     0x00000400
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_OFF     10
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_MSK 0x00000200
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_OFF 9
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_MSK   0x00000100
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_OFF   8
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_MSK    0x00000080
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_OFF    7
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_MSK    0x00000040
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_OFF    6
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_MSK    0x00000020
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_OFF    5
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_MSK    0x00000010
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_OFF    4
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_MSK    0x00000008
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_OFF    3
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_MSK    0x00000004
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_OFF    2
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_MSK    0x00000002
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_OFF    1
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_MSK    0x00000001
#define ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_OFF    0

/*-------------------------------------------------.
 | Register 0x00012404 CHANNEL_DEFECT_INT_SUMMARY0 |
 +-------------------------------------------------+
 | bit  31:0 R  PM_DEF_I                           |
 +------------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_OFF 0

/*-------------------------------------------------.
 | Register 0x00012408 CHANNEL_DEFECT_INT_SUMMARY1 |
 +-------------------------------------------------+
 | bit  31:0 R  PM_DEF_I                           |
 +------------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_OFF 0

/*-------------------------------------------------.
 | Register 0x0001240c CHANNEL_DEFECT_INT_SUMMARY2 |
 +-------------------------------------------------+
 | bit  31:0 R  PM_DEF_I                           |
 +------------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x00012410 PID_DEFECT_INT_SUMMARY0 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x00012414 PID_DEFECT_INT_SUMMARY1 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x00012418 PID_DEFECT_INT_SUMMARY2 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x0001241c PID_DEFECT_INT_SUMMARY3 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x00012420 PID_DEFECT_INT_SUMMARY4 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x00012424 PID_DEFECT_INT_SUMMARY5 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x00012428 PID_DEFECT_INT_SUMMARY6 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_OFF 0

/*---------------------------------------------.
 | Register 0x0001242c PID_DEFECT_INT_SUMMARY7 |
 +---------------------------------------------+
 | bit  31:0 R  PID_DEF_I                      |
 +--------------------------------------------*/
#define PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_UNUSED_MASK   0x00000000
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_MSK 0xffffffff
#define ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_OFF 0

/*----------------------------------------------.
 | Register 0x00012430 SHADOW_TRANSFER_INT_STAT |
 +----------------------------------------------+
 | bit  5 R/W  PMON_DONE_I                      |
 | bit  4 R/W  DEF_ACT_DONE_I                   |
 | bit  1 R/W  PMON_START_I                     |
 | bit  0 R/W  DEF_ACT_START_I                  |
 +---------------------------------------------*/
#define PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_UNUSED_MASK         0xffffffcc
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_MSK     0x00000020
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_OFF     5
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_MSK  0x00000010
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_OFF  4
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_MSK    0x00000002
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_OFF    1
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_MSK 0x00000001
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_OFF 0

/*------------------------------------------------.
 | Register 0x0001243c CHANNEL_AC_APS_INT_STATUS0 |
 +------------------------------------------------+
 | bit  31:0 R/W  AC_APS_I                        |
 +-----------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF 0

/*------------------------------------------------.
 | Register 0x00012440 CHANNEL_AC_APS_INT_STATUS1 |
 +------------------------------------------------+
 | bit  31:0 R/W  AC_APS_I                        |
 +-----------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF 0

/*------------------------------------------------.
 | Register 0x00012444 CHANNEL_AC_APS_INT_STATUS2 |
 +------------------------------------------------+
 | bit  31:0 R/W  AC_APS_I                        |
 +-----------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF 0

/*--------------------------------------------.
 | Register 0x00012448 CHANNEL_AC_APS_INT_EN0 |
 +--------------------------------------------+
 | bit  31:0 R/W  AC_APS_E                    |
 +-------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF 0

/*--------------------------------------------.
 | Register 0x0001244c CHANNEL_AC_APS_INT_EN1 |
 +--------------------------------------------+
 | bit  31:0 R/W  AC_APS_E                    |
 +-------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF 0

/*--------------------------------------------.
 | Register 0x00012450 CHANNEL_AC_APS_INT_EN2 |
 +--------------------------------------------+
 | bit  31:0 R/W  AC_APS_E                    |
 +-------------------------------------------*/
#define PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_UNUSED_MASK  0x00000000
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_MSK 0xffffffff
#define ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF 0

/*-----------------------------------.
 | Register 0x00012454 DEFECT_INT_EN |
 +-----------------------------------+
 | bit  15 R/W  COMB_ABDI_E          |
 | bit  14 R/W  COMB_AAIS_E          |
 | bit  13 R/W  COMB_ATSD_E          |
 | bit  12 R/W  COMB_ATSF_E          |
 | bit  10 R/W  COMB_OOF_E           |
 | bit  9  R/W  COMB_DLOFLOM_E       |
 | bit  8  R/W  COMB_DBIAE_E         |
 | bit  7  R/W  COMB_DIAE_E          |
 | bit  6  R/W  COMB_DLTC_E          |
 | bit  5  R/W  COMB_DBDI_E          |
 | bit  4  R/W  COMB_DDEG_E          |
 | bit  3  R/W  COMB_DTIM_E          |
 | bit  2  R/W  COMB_DAIS_E          |
 | bit  1  R/W  COMB_DOCI_E          |
 | bit  0  R/W  COMB_DLCK_E          |
 +----------------------------------*/
#define PMC_ODU_RFRM_REG_DEFECT_INT_EN_UNUSED_MASK        0xffff0800
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ABDI_E_MSK    0x00008000
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ABDI_E_OFF    15
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_AAIS_E_MSK    0x00004000
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_AAIS_E_OFF    14
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSD_E_MSK    0x00002000
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSD_E_OFF    13
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSF_E_MSK    0x00001000
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSF_E_OFF    12
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_OOF_E_MSK     0x00000400
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_OOF_E_OFF     10
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLOFLOM_E_MSK 0x00000200
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLOFLOM_E_OFF 9
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBIAE_E_MSK   0x00000100
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBIAE_E_OFF   8
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DIAE_E_MSK    0x00000080
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DIAE_E_OFF    7
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLTC_E_MSK    0x00000040
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLTC_E_OFF    6
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBDI_E_MSK    0x00000020
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBDI_E_OFF    5
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DDEG_E_MSK    0x00000010
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DDEG_E_OFF    4
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DTIM_E_MSK    0x00000008
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DTIM_E_OFF    3
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DAIS_E_MSK    0x00000004
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DAIS_E_OFF    2
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DOCI_E_MSK    0x00000002
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DOCI_E_OFF    1
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLCK_E_MSK    0x00000001
#define ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLCK_E_OFF    0

/*--------------------------------------------.
 | Register 0x00012458 SHADOW_TRANSFER_INT_EN |
 +--------------------------------------------+
 | bit  5 R/W  PMON_DONE_E                    |
 | bit  4 R/W  DEF_ACT_DONE_E                 |
 | bit  1 R/W  PMON_START_E                   |
 | bit  0 R/W  DEF_ACT_START_E                |
 +-------------------------------------------*/
#define PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_UNUSED_MASK         0xffffffcc
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_DONE_E_MSK     0x00000020
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_DONE_E_OFF     5
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_DONE_E_MSK  0x00000010
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_DONE_E_OFF  4
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_START_E_MSK    0x00000002
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_START_E_OFF    1
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_START_E_MSK 0x00000001
#define ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_START_E_OFF 0

/*-----------------------------------.
 | Register 0x00012464 GLOBAL_CONFIG |
 +-----------------------------------+
 | bit  15 R/W  DLTC_ACT_DIS         |
 | bit  14 R/W  DAIS_ACT_DIS         |
 | bit  13 R/W  DOCI_ACT_DIS         |
 | bit  12 R/W  DLCK_ACT_DIS         |
 | bit  10 R/W  AC_TTI_FIELD         |
 | bit  9  R/W  LOCK_TTI             |
 | bit  8  R/W  LOCK                 |
 +----------------------------------*/
#define PMC_ODU_RFRM_REG_GLOBAL_CONFIG_UNUSED_MASK      0xfe000800
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLTC_ACT_DIS_MSK 0x00008000
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLTC_ACT_DIS_OFF 15
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DAIS_ACT_DIS_MSK 0x00004000
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DAIS_ACT_DIS_OFF 14
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DOCI_ACT_DIS_MSK 0x00002000
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DOCI_ACT_DIS_OFF 13
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLCK_ACT_DIS_MSK 0x00001000
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLCK_ACT_DIS_OFF 12
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_AC_TTI_FIELD_MSK 0x00000400
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_AC_TTI_FIELD_OFF 10
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_TTI_MSK     0x00000200
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_TTI_OFF     9
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_MSK         0x00000100
#define ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_OFF         8

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODU_RFRM_REGS_H */
