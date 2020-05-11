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
 *     mpma block
 *****************************************************************************/
#ifndef _MPMA_REGS_H
#define _MPMA_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CORE_REG_GEN_CFG                   0x00000000
#define PMC_CORE_REG_CSI_CFG_0                 0x00000004
#define PMC_CORE_REG_CSI_CFG_1                 0x00000008
#define PMC_CORE_REG_GEN_CFG_1                 0x0000000c
#define PMC_CORE_REG_CH_CFG( N )               (0x00000030 + (N) * 0x8)
#define PMC_CORE_REG_CFG_RAM_0( N )            (0x00000400 + (N) * 0x4)
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0    0x00000600
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1    0x00000604
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2    0x00000608
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0    0x0000060c
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1    0x00000610
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2    0x00000614
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0 0x00000640
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1 0x00000644
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2 0x00000648
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0 0x0000064c
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1 0x00000650
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2 0x00000654

/*----------------------------------------.
 | Register 0x00000000 GEN_CFG            |
 +----------------------------------------+
 | bit  30    R/W  BWR_IND_SEL            |
 | bit  28    R/W  FORCE_DEFAULT_PKT_SIZE |
 | bit  25:16 R/W  DEFAULT_PKT_SIZE       |
 | bit  13:12 R/W  BUS_WIDTH              |
 | bit  8     R/W  DPI_DYNAMIC_LS_EN      |
 | bit  4     R/W  DCI_DYNAMIC_LS_EN      |
 | bit  0     R/W  MPMA_EN                |
 +---------------------------------------*/
#define PMC_CORE_REG_GEN_CFG_UNUSED_MASK                0xac00ceee
#define CORE_REG_GEN_CFG_BIT_BWR_IND_SEL_MSK            0x40000000
#define CORE_REG_GEN_CFG_BIT_BWR_IND_SEL_OFF            30
#define CORE_REG_GEN_CFG_BIT_FORCE_DEFAULT_PKT_SIZE_MSK 0x10000000
#define CORE_REG_GEN_CFG_BIT_FORCE_DEFAULT_PKT_SIZE_OFF 28
#define CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_MSK       0x03ff0000
#define CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF       16
#define CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK              0x00003000
#define CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF              12
#define CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_MSK      0x00000100
#define CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_OFF      8
#define CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_MSK      0x00000010
#define CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_OFF      4
#define CORE_REG_GEN_CFG_BIT_MPMA_EN_MSK                0x00000001
#define CORE_REG_GEN_CFG_BIT_MPMA_EN_OFF                0

/*-------------------------------.
 | Register 0x00000004 CSI_CFG_0 |
 +-------------------------------+
 | bit  28:24 R/W  CSI_SF        |
 | bit  20:16 R/W  CSI_SD        |
 | bit  12:8  R/W  CSI_NORMAL    |
 | bit  4:0   R/W  CSI_FORCE_TO  |
 +------------------------------*/
#define PMC_CORE_REG_CSI_CFG_0_UNUSED_MASK      0xe0e0e0e0
#define CORE_REG_CSI_CFG_0_BIT_CSI_SF_MSK       0x1f000000
#define CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF       24
#define CORE_REG_CSI_CFG_0_BIT_CSI_SD_MSK       0x001f0000
#define CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF       16
#define CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_MSK   0x00001f00
#define CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF   8
#define CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_MSK 0x0000001f
#define CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF 0

/*--------------------------------.
 | Register 0x00000008 CSI_CFG_1  |
 +--------------------------------+
 | bit  20:16 R/W  CSI_FORCE_AWAY |
 | bit  4:0   R/W  CSI_SSF        |
 +-------------------------------*/
#define PMC_CORE_REG_CSI_CFG_1_UNUSED_MASK        0xffe0ffe0
#define CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_MSK 0x001f0000
#define CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF 16
#define CORE_REG_CSI_CFG_1_BIT_CSI_SSF_MSK        0x0000001f
#define CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF        0

/*-----------------------------------.
 | Register 0x0000000c GEN_CFG_1     |
 +-----------------------------------+
 | bit  5:0 R/W  PSD_FIFO_FULL_THRES |
 +----------------------------------*/
#define PMC_CORE_REG_GEN_CFG_1_UNUSED_MASK             0xffffffc0
#define CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_MSK 0x0000003f
#define CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF 0

/* index definitions for PMC_CORE_REG_CH_CFG */
#define PMC_CORE_REG_CH_CFG_INDEX_N_MIN             0
#define PMC_CORE_REG_CH_CFG_INDEX_N_MAX             95
#define PMC_CORE_REG_CH_CFG_INDEX_N_SIZE            96
#define PMC_CORE_REG_CH_CFG_INDEX_N_OFFSET          0x8

/*------------------------------------------.
 | Register (0x00000030 + (N) * 0x8) CH_CFG |
 +------------------------------------------+
 | bit  30 R/W  BWR_IND                     |
 | bit  28 R/W  FORCE_AWAY                  |
 | bit  24 R/W  FORCE_TO                    |
 | bit  20 R/W  DEFAULT_PKT_SIZE_EN         |
 | bit  16 R/W  CH_EN                       |
 +-----------------------------------------*/
#define PMC_CORE_REG_CH_CFG_UNUSED_MASK             0xaeeeffff
#define CORE_REG_CH_CFG_BIT_BWR_IND_MSK             0x40000000
#define CORE_REG_CH_CFG_BIT_BWR_IND_OFF             30
#define CORE_REG_CH_CFG_BIT_FORCE_AWAY_MSK          0x10000000
#define CORE_REG_CH_CFG_BIT_FORCE_AWAY_OFF          28
#define CORE_REG_CH_CFG_BIT_FORCE_TO_MSK            0x01000000
#define CORE_REG_CH_CFG_BIT_FORCE_TO_OFF            24
#define CORE_REG_CH_CFG_BIT_DEFAULT_PKT_SIZE_EN_MSK 0x00100000
#define CORE_REG_CH_CFG_BIT_DEFAULT_PKT_SIZE_EN_OFF 20
#define CORE_REG_CH_CFG_BIT_CH_EN_MSK               0x00010000
#define CORE_REG_CH_CFG_BIT_CH_EN_OFF               16

/* index definitions for PMC_CORE_REG_CFG_RAM_0 */
#define PMC_CORE_REG_CFG_RAM_0_INDEX_N_MIN               0
#define PMC_CORE_REG_CFG_RAM_0_INDEX_N_MAX               95
#define PMC_CORE_REG_CFG_RAM_0_INDEX_N_SIZE              96
#define PMC_CORE_REG_CFG_RAM_0_INDEX_N_OFFSET            0x4

/*---------------------------------------------.
 | Register (0x00000400 + (N) * 0x4) CFG_RAM_0 |
 +---------------------------------------------+
 | bit  25:16 R/W  PKT_SIZE                    |
 | bit  12:8  R/W  PSD_FIFO_REFILL_THRES       |
 | bit  4     R/W  TS_ENABLE                   |
 | bit  1:0   R/W  MKR_MODE                    |
 +--------------------------------------------*/
#define PMC_CORE_REG_CFG_RAM_0_UNUSED_MASK               0xfc00e0ec
#define CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_MSK              0x03ff0000
#define CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF              16
#define CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK 0x00001f00
#define CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF 8
#define CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_MSK             0x00000010
#define CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_OFF             4
#define CORE_REG_CFG_RAM_0_BIT_MKR_MODE_MSK              0x00000003
#define CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF              0

/*--------------------------------------------.
 | Register 0x00000600 PSD_FIFO_OVR_INT_REG_0 |
 +--------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_OVR_I              |
 +-------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000604 PSD_FIFO_OVR_INT_REG_1 |
 +--------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_OVR_I              |
 +-------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000608 PSD_FIFO_OVR_INT_REG_2 |
 +--------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_OVR_I              |
 +-------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF 0

/*--------------------------------------------.
 | Register 0x0000060c PSD_FIFO_UDR_INT_REG_0 |
 +--------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_UDR_I              |
 +-------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000610 PSD_FIFO_UDR_INT_REG_1 |
 +--------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_UDR_I              |
 +-------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF 0

/*--------------------------------------------.
 | Register 0x00000614 PSD_FIFO_UDR_INT_REG_2 |
 +--------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_UDR_I              |
 +-------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF 0

/*-----------------------------------------------.
 | Register 0x00000640 PSD_FIFO_OVR_INT_EN_REG_0 |
 +-----------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_OVR_E                 |
 +----------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000644 PSD_FIFO_OVR_INT_EN_REG_1 |
 +-----------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_OVR_E                 |
 +----------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000648 PSD_FIFO_OVR_INT_EN_REG_2 |
 +-----------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_OVR_E                 |
 +----------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF 0

/*-----------------------------------------------.
 | Register 0x0000064c PSD_FIFO_UDR_INT_EN_REG_0 |
 +-----------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_UDR_E                 |
 +----------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000650 PSD_FIFO_UDR_INT_EN_REG_1 |
 +-----------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_UDR_E                 |
 +----------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000654 PSD_FIFO_UDR_INT_EN_REG_2 |
 +-----------------------------------------------+
 | bit  31:0 R/W  PSD_FIFO_UDR_E                 |
 +----------------------------------------------*/
#define PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_UNUSED_MASK        0x00000000
#define CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_MSK 0xffffffff
#define CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMA_REGS_H */
