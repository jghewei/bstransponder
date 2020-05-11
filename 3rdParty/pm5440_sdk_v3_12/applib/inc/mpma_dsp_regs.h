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
 *     mpma_dsp block
 *****************************************************************************/
#ifndef _MPMA_DSP_REGS_H
#define _MPMA_DSP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_MPMA_DSP_REG_COMMON_CFG_REG             0x00003800
#define PMC_MPMA_DSP_REG_GENERAL_CFG_REG( N )       (0x00003804 + (N) * 0x14)
#define PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG( N )   (0x00003808 + (N) * 0x14)
#define PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG( N ) (0x0000380c + (N) * 0x14)
#define PMC_MPMA_DSP_REG_DSM_CFG_REG( N )           (0x00003810 + (N) * 0x14)
#define PMC_MPMA_DSP_REG_CN_BASE_REG( N )           (0x00003814 + (N) * 0x14)

/*------------------------------------.
 | Register 0x00003800 COMMON_CFG_REG |
 +------------------------------------+
 | bit  16  R/W  SCHEDULING_PACER_EN  |
 | bit  8   R/W  INTERRUPT_CLR        |
 | bit  5:0 R/W  CLIENT_WIDTH         |
 +-----------------------------------*/
#define PMC_MPMA_DSP_REG_COMMON_CFG_REG_UNUSED_MASK             0xfffefec0
#define MPMA_DSP_REG_COMMON_CFG_REG_BIT_SCHEDULING_PACER_EN_MSK 0x00010000
#define MPMA_DSP_REG_COMMON_CFG_REG_BIT_SCHEDULING_PACER_EN_OFF 16
#define MPMA_DSP_REG_COMMON_CFG_REG_BIT_INTERRUPT_CLR_MSK       0x00000100
#define MPMA_DSP_REG_COMMON_CFG_REG_BIT_INTERRUPT_CLR_OFF       8
#define MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_MSK        0x0000003f
#define MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF        0

/* index definitions for PMC_MPMA_DSP_REG_GENERAL_CFG_REG */
#define PMC_MPMA_DSP_REG_GENERAL_CFG_REG_INDEX_N_MIN                   0
#define PMC_MPMA_DSP_REG_GENERAL_CFG_REG_INDEX_N_MAX                   95
#define PMC_MPMA_DSP_REG_GENERAL_CFG_REG_INDEX_N_SIZE                  96
#define PMC_MPMA_DSP_REG_GENERAL_CFG_REG_INDEX_N_OFFSET                0x14

/*----------------------------------------------------.
 | Register (0x00003804 + (N) * 0x14) GENERAL_CFG_REG |
 +----------------------------------------------------+
 | bit  31:16 R/W  PKTS_PER_CN                        |
 | bit  4     R/W  CN_MODE                            |
 | bit  3     R/W  CN_BIT_MODE                        |
 | bit  2     R/W  CN_ENABLE                          |
 | bit  1     R/W  CND_ENABLE                         |
 | bit  0     R/W  SLICE_ENABLE                       |
 | bit  15:12 R/W  INTEGRATOR_CLAMP                   |
 | bit  5     R/W  SCHEDULING_PACER_EN_LOCAL          |
 +---------------------------------------------------*/
#define PMC_MPMA_DSP_REG_GENERAL_CFG_REG_UNUSED_MASK                   0x00000fc0
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_MSK               0xffff0000
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF               16
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_MODE_MSK                   0x00000010
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_MODE_OFF                   4
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_MSK               0x00000008
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_OFF               3
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_ENABLE_MSK                 0x00000004
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_ENABLE_OFF                 2
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CND_ENABLE_MSK                0x00000002
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CND_ENABLE_OFF                1
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_MSK              0x00000001
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_OFF              0
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_MSK          0x0000f000
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF          12
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SCHEDULING_PACER_EN_LOCAL_MSK 0x00000020
#define MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SCHEDULING_PACER_EN_LOCAL_OFF 5

/* index definitions for PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG */
#define PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG_INDEX_N_MIN      0
#define PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG_INDEX_N_MAX      95
#define PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG_INDEX_N_SIZE     96
#define PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG_INDEX_N_OFFSET   0x14

/*--------------------------------------------------------.
 | Register (0x00003808 + (N) * 0x14) PACKET_SIZE_CFG_REG |
 +--------------------------------------------------------+
 | bit  31:24 R/W  PKT_PERIOD_D                           |
 | bit  16    R/W  PKT_SIZE_VAR                           |
 | bit  8:0   R/W  PKT_SIZE                               |
 +-------------------------------------------------------*/
#define PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG_UNUSED_MASK      0x00fefe00
#define MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_MSK 0xff000000
#define MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF 24
#define MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_MSK 0x00010000
#define MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_OFF 16
#define MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK     0x000001ff
#define MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF     0

/* index definitions for PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG */
#define PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_INDEX_N_MIN        0
#define PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_INDEX_N_MAX        95
#define PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_INDEX_N_SIZE       96
#define PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_INDEX_N_OFFSET     0x14

/*----------------------------------------------------------.
 | Register (0x0000380c + (N) * 0x14) PACKET_PERIOD_CFG_REG |
 +----------------------------------------------------------+
 | bit  24:16 R/W  PKT_PERIOD_N                             |
 | bit  15:0  R/W  PKT_PERIOD_INT                           |
 +---------------------------------------------------------*/
#define PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_UNUSED_MASK        0xfe000000
#define MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_MSK   0x01ff0000
#define MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF   16
#define MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_MSK 0x0000ffff
#define MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF 0

/* index definitions for PMC_MPMA_DSP_REG_DSM_CFG_REG */
#define PMC_MPMA_DSP_REG_DSM_CFG_REG_INDEX_N_MIN              0
#define PMC_MPMA_DSP_REG_DSM_CFG_REG_INDEX_N_MAX              95
#define PMC_MPMA_DSP_REG_DSM_CFG_REG_INDEX_N_SIZE             96
#define PMC_MPMA_DSP_REG_DSM_CFG_REG_INDEX_N_OFFSET           0x14

/*------------------------------------------------.
 | Register (0x00003810 + (N) * 0x14) DSM_CFG_REG |
 +------------------------------------------------+
 | bit  6   R    INTEGRATOR_UNDERFLOW             |
 | bit  5   R    INTEGRATOR_OVERFLOW              |
 | bit  4   R/W  DSM_TYPE                         |
 | bit  3:0 R/W  MPMA_GAIN                        |
 | bit  10  R    CLAMP_UNDERFLOW                  |
 | bit  9   R    CLAMP_OVERFLOW                   |
 +-----------------------------------------------*/
#define PMC_MPMA_DSP_REG_DSM_CFG_REG_UNUSED_MASK              0x00000000
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_UNDERFLOW_MSK 0x00000040
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_UNDERFLOW_OFF 6
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_OVERFLOW_MSK  0x00000020
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_OVERFLOW_OFF  5
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_DSM_TYPE_MSK             0x00000010
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_DSM_TYPE_OFF             4
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_MSK            0x0000000f
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF            0
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_UNDERFLOW_MSK      0x00000400
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_UNDERFLOW_OFF      10
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_OVERFLOW_MSK       0x00000200
#define MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_OVERFLOW_OFF       9

/* index definitions for PMC_MPMA_DSP_REG_CN_BASE_REG */
#define PMC_MPMA_DSP_REG_CN_BASE_REG_INDEX_N_MIN        0
#define PMC_MPMA_DSP_REG_CN_BASE_REG_INDEX_N_MAX        95
#define PMC_MPMA_DSP_REG_CN_BASE_REG_INDEX_N_SIZE       96
#define PMC_MPMA_DSP_REG_CN_BASE_REG_INDEX_N_OFFSET     0x14

/*------------------------------------------------.
 | Register (0x00003814 + (N) * 0x14) CN_BASE_REG |
 +------------------------------------------------+
 | bit  28:24 R/W  CN_BASE_OFFSET                 |
 | bit  23:0  R/W  CN_BASE                        |
 +-----------------------------------------------*/
#define PMC_MPMA_DSP_REG_CN_BASE_REG_UNUSED_MASK        0xe0000000
#define MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_MSK 0x1f000000
#define MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF 24
#define MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_MSK        0x00ffffff
#define MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF        0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMA_DSP_REGS_H */
