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
 *     mpmo_dsp block
 *****************************************************************************/
#ifndef _MPMO_DSP_REGS_H
#define _MPMO_DSP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MPMO_DSP_REG_COMMON_CFG_REG                        0x00006000
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG( N )   (0x00006004 + (N) * 0x8)
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG( N ) (0x00006008 + (N) * 0x8)
#define PMC_MPMO_DSP_REG_GENERAL_CFG_REG( N )                  (0x00006044 + (N) * 0x14)
#define PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG( N )          (0x00006048 + (N) * 0x14)
#define PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG( N )        (0x0000604c + (N) * 0x14)
#define PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG( N )      (0x00006050 + (N) * 0x14)
#define PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG( N )         (0x00006054 + (N) * 0x14)
#define PMC_MPMO_DSP_REG_DEBUG684_REG( N )                     (0x00006684 + (N) * 0x8)

/*------------------------------------.
 | Register 0x00006000 COMMON_CFG_REG |
 +------------------------------------+
 | bit  0 R/W  CN_REQ_MODE            |
 +-----------------------------------*/
#define PMC_MPMO_DSP_REG_COMMON_CFG_REG_UNUSED_MASK     0xfffffffe
#define MPMO_DSP_REG_COMMON_CFG_REG_BIT_CN_REQ_MODE_MSK 0x00000001
#define MPMO_DSP_REG_COMMON_CFG_REG_BIT_CN_REQ_MODE_OFF 0

/* index definitions for PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG */
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_INDEX_N_MIN      0
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_INDEX_N_MAX      7
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_INDEX_N_SIZE     8
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_INDEX_N_OFFSET   0x8

/*------------------------------------------------------------------.
 | Register (0x00006004 + (N) * 0x8) FRAME_PERIOD_NUMERATOR_CFG_REG |
 +------------------------------------------------------------------+
 | bit  31:0 R/W  FRM_PERIOD_N                                      |
 +-----------------------------------------------------------------*/
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_UNUSED_MASK      0x00000000
#define MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_MSK 0xffffffff
#define MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF 0

/* index definitions for PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG */
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_INDEX_N_MIN      0
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_INDEX_N_MAX      7
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_INDEX_N_SIZE     8
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_INDEX_N_OFFSET   0x8

/*--------------------------------------------------------------------.
 | Register (0x00006008 + (N) * 0x8) FRAME_PERIOD_DENOMINATOR_CFG_REG |
 +--------------------------------------------------------------------+
 | bit  31   R/W  FP_ENABLE                                           |
 | bit  23:0 R/W  FRM_PERIOD_D                                        |
 +-------------------------------------------------------------------*/
#define PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_UNUSED_MASK      0x7f000000
#define MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FP_ENABLE_MSK    0x80000000
#define MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FP_ENABLE_OFF    31
#define MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_MSK 0x00ffffff
#define MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF 0

/* index definitions for PMC_MPMO_DSP_REG_GENERAL_CFG_REG */
#define PMC_MPMO_DSP_REG_GENERAL_CFG_REG_INDEX_N_MIN              0
#define PMC_MPMO_DSP_REG_GENERAL_CFG_REG_INDEX_N_MAX              95
#define PMC_MPMO_DSP_REG_GENERAL_CFG_REG_INDEX_N_SIZE             96
#define PMC_MPMO_DSP_REG_GENERAL_CFG_REG_INDEX_N_OFFSET           0x14

/*----------------------------------------------------.
 | Register (0x00006044 + (N) * 0x14) GENERAL_CFG_REG |
 +----------------------------------------------------+
 | bit  29:27 R/W  FRM_CNT_SEL                        |
 | bit  26:22 R/W  AGE_INT_GAIN                       |
 | bit  21:17 R/W  AGE_PROP_GAIN                      |
 | bit  16:12 R/W  ZONE_CENT_GAIN                     |
 | bit  11:8  R/W  LPF_GAIN                           |
 | bit  7     R/W  AGE_GAIN_UPDATE                    |
 | bit  6     R/W  HOLDOVER                           |
 | bit  5     R/W  ZONE_CENT_CONTINUOUS               |
 | bit  4     R/W  ZONE_CENT_TRIGGER                  |
 | bit  3     R/W  ZONE_CENT_ENABLE                   |
 | bit  2     R/W  CN_BIT_MODE                        |
 | bit  1     R/W  CN_GENERATOR_ENABLE                |
 | bit  0     R/W  SLICE_ENABLE                       |
 +---------------------------------------------------*/
#define PMC_MPMO_DSP_REG_GENERAL_CFG_REG_UNUSED_MASK              0xc0000000
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_MSK          0x38000000
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF          27
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_MSK         0x07c00000
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF         22
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_MSK        0x003e0000
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF        17
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_MSK       0x0001f000
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF       12
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_MSK             0x00000f00
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF             8
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_GAIN_UPDATE_MSK      0x00000080
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_GAIN_UPDATE_OFF      7
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_HOLDOVER_MSK             0x00000040
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_HOLDOVER_OFF             6
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_CONTINUOUS_MSK 0x00000020
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_CONTINUOUS_OFF 5
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_TRIGGER_MSK    0x00000010
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_TRIGGER_OFF    4
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_ENABLE_MSK     0x00000008
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_ENABLE_OFF     3
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_MSK          0x00000004
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_OFF          2
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_GENERATOR_ENABLE_MSK  0x00000002
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_GENERATOR_ENABLE_OFF  1
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_MSK         0x00000001
#define MPMO_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_OFF         0

/* index definitions for PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG */
#define PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_INDEX_N_MIN          0
#define PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_INDEX_N_MAX          95
#define PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_INDEX_N_SIZE         96
#define PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_INDEX_N_OFFSET       0x14

/*------------------------------------------------------------.
 | Register (0x00006048 + (N) * 0x14) CN_BASE_INTEGER_CFG_REG |
 +------------------------------------------------------------+
 | bit  23:0  R/W  CN_BASE_INT                                |
 | bit  31    R/W  FAST_LOCK_ENABLE                           |
 | bit  27:24 R/W  FAST_LOCK_THRESH                           |
 +-----------------------------------------------------------*/
#define PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_UNUSED_MASK          0x40000000
#define MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_MSK      0x00ffffff
#define MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF      0
#define MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_ENABLE_MSK 0x80000000
#define MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_ENABLE_OFF 31
#define MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_MSK 0x0f000000
#define MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF 24

/* index definitions for PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG */
#define PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_INDEX_N_MIN    0
#define PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_INDEX_N_MAX    95
#define PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_INDEX_N_SIZE   96
#define PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_INDEX_N_OFFSET 0x14

/*--------------------------------------------------------------.
 | Register (0x0000604c + (N) * 0x14) CN_BASE_NUMERATOR_CFG_REG |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  CN_BASE_N                                     |
 +-------------------------------------------------------------*/
#define PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_UNUSED_MASK    0x00000000
#define MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_MSK  0xffffffff
#define MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF  0

/* index definitions for PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG */
#define PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_INDEX_N_MIN    0
#define PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_INDEX_N_MAX    95
#define PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_INDEX_N_SIZE   96
#define PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_INDEX_N_OFFSET 0x14

/*----------------------------------------------------------------.
 | Register (0x00006050 + (N) * 0x14) CN_BASE_DENOMINATOR_CFG_REG |
 +----------------------------------------------------------------+
 | bit  31:0 R/W  CN_BASE_D                                       |
 +---------------------------------------------------------------*/
#define PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_UNUSED_MASK    0x00000000
#define MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_MSK  0xffffffff
#define MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF  0

/* index definitions for PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG */
#define PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_INDEX_N_MIN        0
#define PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_INDEX_N_MAX        95
#define PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_INDEX_N_SIZE       96
#define PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_INDEX_N_OFFSET     0x14

/*-------------------------------------------------------------.
 | Register (0x00006054 + (N) * 0x14) ODUFLEX_RESIZING_CFG_REG |
 +-------------------------------------------------------------+
 | bit  29:28 R/W  BWR_RAMP_DELAY                              |
 | bit  19:0  R/W  BWR_STEP                                    |
 +------------------------------------------------------------*/
#define PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_UNUSED_MASK        0xcff00000
#define MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_MSK 0x30000000
#define MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF 28
#define MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_MSK       0x000fffff
#define MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF       0

/* index definitions for PMC_MPMO_DSP_REG_DEBUG684_REG */
#define PMC_MPMO_DSP_REG_DEBUG684_REG_INDEX_N_MIN       0
#define PMC_MPMO_DSP_REG_DEBUG684_REG_INDEX_N_MAX       79
#define PMC_MPMO_DSP_REG_DEBUG684_REG_INDEX_N_SIZE      80
#define PMC_MPMO_DSP_REG_DEBUG684_REG_INDEX_N_OFFSET    0x8

/*------------------------------------------------.
 | Register (0x00006684 + (N) * 0x8) DEBUG684_REG |
 +------------------------------------------------+
 | bit  31:0 R  INTEGRATOR684                     |
 +-----------------------------------------------*/
#define PMC_MPMO_DSP_REG_DEBUG684_REG_UNUSED_MASK       0x00000000
#define MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_MSK 0xffffffff
#define MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMO_DSP_REGS_H */
