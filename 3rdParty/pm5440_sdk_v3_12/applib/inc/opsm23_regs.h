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
 *     opsm23 block
 *****************************************************************************/
#ifndef _OPSM23_REGS_H
#define _OPSM23_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG                0x00011004
#define PMC_OPSM23_REG_TX_OTUK_AIS_CTRL_REG              0x00011008
#define PMC_OPSM23_REG_TX_INT_REG                        0x0001100c
#define PMC_OPSM23_REG_TX_INT_EN_REG                     0x00011010
#define PMC_OPSM23_REG_TX_INT_VAL_REG                    0x00011014
#define PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG           0x0001101c
#define PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH              0x00011020
#define PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG     0x0001102c
#define PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG                0x00011080
#define PMC_OPSM23_REG_RX_OTU3_CTRL_REG                  0x00011084
#define PMC_OPSM23_REG_RX_OTU3_INT_REG                   0x00011088
#define PMC_OPSM23_REG_RX_OTU3_INT_EN_REG                0x0001108c
#define PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG               0x00011090
#define PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG( N ) (0x00011094 + (N) * 0x4)
#define PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG( N )       (0x000110a4 + (N) * 0x4)
#define PMC_OPSM23_REG_RX_LANE_INT_REG( N )              (0x000110b4 + (N) * 0x4)
#define PMC_OPSM23_REG_RX_LANE_INT_EN_REG( N )           (0x000110c4 + (N) * 0x4)
#define PMC_OPSM23_REG_RX_LANE_INT_VAL_REG( N )          (0x000110d4 + (N) * 0x4)
#define PMC_OPSM23_REG_RX_FP_CONFIG_REG                  0x000110e4
#define PMC_OPSM23_REG_OPSM23_INT_SUMMARY                0x00011118

/*----------------------------------------.
 | Register 0x00011004 TX_GLOBAL_CTRL_REG |
 +----------------------------------------+
 | bit  20    R/W  TX_DATA_4X10G_MODE     |
 | bit  19    R/W  TX_OTL_3_4_ENABLE      |
 | bit  18:15 R/W  TX_SCRAMBLE_ENABLE     |
 | bit  14    R/W  TX_HARDWARE_RESET      |
 | bit  13:10 R/W  TX_SYS_LOOPBACK        |
 | bit  7:4   R/W  TX_DATA_IN_SCRAMBLED   |
 | bit  3:0   R/W  TX_LINE_SOFT_RESET     |
 +---------------------------------------*/
#define PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG_UNUSED_MASK              0xffc00200
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_4X10G_MODE_MSK   0x00100000
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_4X10G_MODE_OFF   20
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_OTL_3_4_ENABLE_MSK    0x00080000
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_OTL_3_4_ENABLE_OFF    19
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK   0x00078000
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF   15
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_MSK    0x00004000
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_OFF    14
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_MSK      0x00003c00
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF      10
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK 0x000000f0
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF 4
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK   0x0000000f
#define OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF   0

/*------------------------------------------.
 | Register 0x00011008 TX_OTUK_AIS_CTRL_REG |
 +------------------------------------------+
 +-----------------------------------------*/
#define PMC_OPSM23_REG_TX_OTUK_AIS_CTRL_REG_UNUSED_MASK 0xffff80ff

/*----------------------------------------------.
 | Register 0x0001100c TX_INT_REG               |
 +----------------------------------------------+
 | bit  25    R/W  TX_SFI_FIFO_OVERFLOW_INT_I   |
 | bit  24    R/W  TX_SFI_FIFO_UNDERFLOW_INT_I  |
 | bit  23:20 R/W  TX_SYS_FIFO_OVERFLOW_INT_I   |
 | bit  19:16 R/W  TX_SYS_FIFO_UNDERFLOW_INT_I  |
 | bit  7:4   R/W  TX_LANE_FIFO_OVERFLOW_INT_I  |
 | bit  3:0   R/W  TX_LANE_FIFO_UNDERFLOW_INT_I |
 +---------------------------------------------*/
#define PMC_OPSM23_REG_TX_INT_REG_UNUSED_MASK                      0xfc000000
#define OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK   0x02000000
#define OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF   25
#define OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK  0x01000000
#define OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF  24
#define OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK   0x00f00000
#define OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF   20
#define OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK  0x000f0000
#define OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF  16
#define OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK  0x000000f0
#define OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF  4
#define OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK 0x0000000f
#define OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF 0

/*----------------------------------------------.
 | Register 0x00011010 TX_INT_EN_REG            |
 +----------------------------------------------+
 | bit  25    R/W  TX_SFI_FIFO_OVERFLOW_INT_E   |
 | bit  24    R/W  TX_SFI_FIFO_UNDERFLOW_INT_E  |
 | bit  23:20 R/W  TX_SYS_FIFO_OVERFLOW_INT_E   |
 | bit  19:16 R/W  TX_SYS_FIFO_UNDERFLOW_INT_E  |
 | bit  7:4   R/W  TX_LANE_FIFO_OVERFLOW_INT_E  |
 | bit  3:0   R/W  TX_LANE_FIFO_UNDERFLOW_INT_E |
 +---------------------------------------------*/
#define PMC_OPSM23_REG_TX_INT_EN_REG_UNUSED_MASK                      0xfc000000
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_MSK   0x02000000
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_OFF   25
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_MSK  0x01000000
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_OFF  24
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK   0x00f00000
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF   20
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK  0x000f0000
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF  16
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK  0x000000f0
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF  4
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK 0x0000000f
#define OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF 0

/*--------------------------------------------.
 | Register 0x00011014 TX_INT_VAL_REG         |
 +--------------------------------------------+
 | bit  25    R  TX_SFI_FIFO_OVERFLOW_INT_V   |
 | bit  24    R  TX_SFI_FIFO_UNDERFLOW_INT_V  |
 | bit  23:20 R  TX_SYS_FIFO_OVERFLOW_INT_V   |
 | bit  19:16 R  TX_SYS_FIFO_UNDERFLOW_INT_V  |
 | bit  7:4   R  TX_LANE_FIFO_OVERFLOW_INT_V  |
 | bit  3:0   R  TX_LANE_FIFO_UNDERFLOW_INT_V |
 +-------------------------------------------*/
#define PMC_OPSM23_REG_TX_INT_VAL_REG_UNUSED_MASK                      0xfc000000
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_MSK   0x02000000
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_OFF   25
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_MSK  0x01000000
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_OFF  24
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK   0x00f00000
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF   20
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK  0x000f0000
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF  16
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK  0x000000f0
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF  4
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK 0x0000000f
#define OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF 0

/*---------------------------------------------.
 | Register 0x0001101c TX_L2P_LANE_MAPPING_REG |
 +---------------------------------------------+
 | bit  7:6 R/W  TX_PHYSICAL_LANE_3_SELECT     |
 | bit  5:4 R/W  TX_PHYSICAL_LANE_2_SELECT     |
 | bit  3:2 R/W  TX_PHYSICAL_LANE_1_SELECT     |
 | bit  1:0 R/W  TX_PHYSICAL_LANE_0_SELECT     |
 +--------------------------------------------*/
#define PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG_UNUSED_MASK                   0xffffff00
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK 0x000000c0
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF 6
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK 0x00000030
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF 4
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK 0x0000000c
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF 2
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK 0x00000003
#define OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF 0

/*------------------------------------------------.
 | Register 0x00011020 TX_SYNC_FIFO_XOFF_TH       |
 +------------------------------------------------+
 | bit  15:8 R/W  TX_SYS_FIFO_XOFF_SET_FILL_LEVEL |
 +-----------------------------------------------*/
#define PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_UNUSED_MASK                         0xffff0000
#define OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_MSK 0x0000ff00
#define OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF 8

/*---------------------------------------------------.
 | Register 0x0001102c TX_FRAME_PULSE_GEN_CONFIG_REG |
 +---------------------------------------------------+
 | bit  19:16 R/W  TX_FP_MFAS_INVERT                 |
 +--------------------------------------------------*/
#define PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_UNUSED_MASK           0xfff00000
#define OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_MSK 0x000f0000
#define OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF 16

/*------------------------------------------.
 | Register 0x00011080 RX_GLOBAL_CTRL_REG   |
 +------------------------------------------+
 | bit  20    R/W  RX_DATA_4X10G_MODE       |
 | bit  19    R/W  RX_OTL_3_4_ENABLE        |
 | bit  18:15 R/W  RX_SCRAMBLE_ENABLE       |
 | bit  14    R/W  RX_HARDWARE_RESET        |
 | bit  13:10 R/W  RX_SYS_LOOPBACK          |
 | bit  9:6   R/W  RX_DATA_IN_NOT_SCRAMBLED |
 | bit  3:0   R/W  RX_LINE_SOFT_RESET       |
 +-----------------------------------------*/
#define PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG_UNUSED_MASK                  0xffc00020
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_4X10G_MODE_MSK       0x00100000
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_4X10G_MODE_OFF       20
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_3_4_ENABLE_MSK        0x00080000
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_3_4_ENABLE_OFF        19
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK       0x00078000
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF       15
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_MSK        0x00004000
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_OFF        14
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_MSK          0x00003c00
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF          10
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK 0x000003c0
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF 6
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_MSK       0x0000000f
#define OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF       0

/*-------------------------------------------.
 | Register 0x00011084 RX_OTU3_CTRL_REG      |
 +-------------------------------------------+
 | bit  7:0 R/W  RX_LOF_TRANSITION_OUT_COUNT |
 +------------------------------------------*/
#define PMC_OPSM23_REG_RX_OTU3_CTRL_REG_UNUSED_MASK                     0xffffff00
#define OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK 0x000000ff
#define OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF 0

/*--------------------------------------------.
 | Register 0x00011088 RX_OTU3_INT_REG        |
 +--------------------------------------------+
 | bit  15 R/W  RX_SFI_LOS_INT_I              |
 | bit  14 R/W  RX_SFI_OOF_INT_I              |
 | bit  13 R/W  RX_SFI_LOF_INT_I              |
 | bit  12 R/W  RX_OTU_OOF_INT_I              |
 | bit  11 R/W  RX_OTU_LOF_INT_I              |
 | bit  9  R/W  RX_SFI_AIS_INT_I              |
 | bit  8  R/W  RX_LOL_INT_I                  |
 | bit  7  R/W  RX_SKEW_RANGE_VIOLATION_INT_I |
 | bit  6  R/W  RX_ALIGN_CHANGE_INT_I         |
 | bit  5  R/W  RX_DESKEW_CHANGE_INT_I        |
 | bit  4  R/W  RX_SFI_FIFO_OVERFLOW_INT_I    |
 | bit  3  R/W  RX_SFI_FIFO_UNDERFLOW_INT_I   |
 +-------------------------------------------*/
#define PMC_OPSM23_REG_RX_OTU3_INT_REG_UNUSED_MASK                       0xffff0407
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK              0x00008000
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF              15
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK              0x00004000
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF              14
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK              0x00002000
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF              13
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK              0x00001000
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF              12
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK              0x00000800
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF              11
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK              0x00000200
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF              9
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_MSK                  0x00000100
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_OFF                  8
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK 0x00000080
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF 7
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_MSK         0x00000040
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_OFF         6
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK        0x00000020
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF        5
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK    0x00000010
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF    4
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK   0x00000008
#define OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF   3

/*--------------------------------------------.
 | Register 0x0001108c RX_OTU3_INT_EN_REG     |
 +--------------------------------------------+
 | bit  15 R/W  RX_SFI_LOS_INT_E              |
 | bit  14 R/W  RX_SFI_OOF_INT_E              |
 | bit  13 R/W  RX_SFI_LOF_INT_E              |
 | bit  12 R/W  RX_OTU_OOF_INT_E              |
 | bit  11 R/W  RX_OTU_LOF_INT_E              |
 | bit  9  R/W  RX_SFI_AIS_INT_E              |
 | bit  8  R/W  RX_LOL_INT_E                  |
 | bit  7  R/W  RX_SKEW_RANGE_VIOLATION_INT_E |
 | bit  6  R/W  RX_ALIGN_CHANGE_INT_E         |
 | bit  5  R/W  RX_DESKEW_CHANGE_INT_E        |
 | bit  4  R/W  RX_SFI_FIFO_OVERFLOW_INT_E    |
 | bit  3  R/W  RX_SFI_FIFO_UNDERFLOW_INT_E   |
 +-------------------------------------------*/
#define PMC_OPSM23_REG_RX_OTU3_INT_EN_REG_UNUSED_MASK                       0xffff0407
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_MSK              0x00008000
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_OFF              15
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_MSK              0x00004000
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_OFF              14
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_MSK              0x00002000
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_OFF              13
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_MSK              0x00001000
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_OFF              12
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_MSK              0x00000800
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_OFF              11
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_MSK              0x00000200
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_OFF              9
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_LOL_INT_E_MSK                  0x00000100
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_LOL_INT_E_OFF                  8
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_MSK 0x00000080
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_OFF 7
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_ALIGN_CHANGE_INT_E_MSK         0x00000040
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_ALIGN_CHANGE_INT_E_OFF         6
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_MSK        0x00000020
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_OFF        5
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_MSK    0x00000010
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_OFF    4
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_MSK   0x00000008
#define OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_OFF   3

/*------------------------------------------.
 | Register 0x00011090 RX_OTU3_INT_VAL_REG  |
 +------------------------------------------+
 | bit  15 R  RX_SFI_LOS_INT_V              |
 | bit  14 R  RX_SFI_OOF_INT_V              |
 | bit  13 R  RX_SFI_LOF_INT_V              |
 | bit  12 R  RX_OTU_OOF_INT_V              |
 | bit  11 R  RX_OTU_LOF_INT_V              |
 | bit  9  R  RX_SFI_AIS_INT_V              |
 | bit  8  R  RX_LOL_INT_V                  |
 | bit  7  R  RX_SKEW_RANGE_VIOLATION_INT_V |
 | bit  6  R  RX_ALIGN_CHANGE_INT_V         |
 | bit  5  R  RX_DESKEW_CHANGE_INT_V        |
 | bit  4  R  RX_SFI_FIFO_OVERFLOW_INT_V    |
 | bit  3  R  RX_SFI_FIFO_UNDERFLOW_INT_V   |
 +-----------------------------------------*/
#define PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG_UNUSED_MASK                       0xffff0407
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_MSK              0x00008000
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_OFF              15
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_MSK              0x00004000
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_OFF              14
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_MSK              0x00002000
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_OFF              13
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_MSK              0x00001000
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_OFF              12
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_MSK              0x00000800
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_OFF              11
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_MSK              0x00000200
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_OFF              9
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_LOL_INT_V_MSK                  0x00000100
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_LOL_INT_V_OFF                  8
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_MSK 0x00000080
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_OFF 7
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_ALIGN_CHANGE_INT_V_MSK         0x00000040
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_ALIGN_CHANGE_INT_V_OFF         6
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_MSK        0x00000020
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_OFF        5
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_MSK    0x00000010
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_OFF    4
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_MSK   0x00000008
#define OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_OFF   3

/* index definitions for PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG */
#define PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_INDEX_N_MIN                  0
#define PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_INDEX_N_MAX                  3
#define PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_INDEX_N_SIZE                 4
#define PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_INDEX_N_OFFSET               0x4

/*----------------------------------------------------------------.
 | Register (0x00011094 + (N) * 0x4) RX_LANE_SSF_FAIL_OR_CTRL_REG |
 +----------------------------------------------------------------+
 | bit  15 R/W  RX_SSF_LANE_MI_ACTIVE                             |
 | bit  14 R/W  RX_SSF_LANE_LOS                                   |
 | bit  13 R/W  RX_SSF_LANE_LOF_LOFLANE                           |
 | bit  11 R/W  RX_SSF_LANE_OTUK_AIS                              |
 | bit  10 R/W  RX_SSF_LANE_OTL_AIS                               |
 | bit  9  R/W  RX_SSF_LOL                                        |
 | bit  8  R/W  RX_SSF_OTU3_LOF                                   |
 | bit  7  R/W  RX_FEC_LANE_MI_ACTIVE                             |
 | bit  6  R/W  RX_FAIL_LANE_LOS                                  |
 | bit  5  R/W  RX_FAIL_LANE_LOF_LOFLANE                          |
 | bit  3  R/W  RX_FAIL_LANE_OTUK_AIS                             |
 | bit  2  R/W  RX_FAIL_LANE_OTL_AIS                              |
 | bit  1  R/W  RX_FAIL_LOL                                       |
 | bit  0  R/W  RX_FAIL_OTU3_LOF                                  |
 +---------------------------------------------------------------*/
#define PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_UNUSED_MASK                  0xffff0000
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_MSK    0x00008000
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_OFF    15
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_MSK          0x00004000
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_OFF          14
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_MSK  0x00002000
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_OFF  13
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_MSK     0x00000800
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_OFF     11
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_MSK      0x00000400
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_OFF      10
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_MSK               0x00000200
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_OFF               9
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU3_LOF_MSK          0x00000100
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU3_LOF_OFF          8
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_MSK    0x00000080
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_OFF    7
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_MSK         0x00000040
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_OFF         6
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_MSK 0x00000020
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_OFF 5
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_MSK    0x00000008
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_OFF    3
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_MSK     0x00000004
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_OFF     2
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_MSK              0x00000002
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_OFF              1
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU3_LOF_MSK         0x00000001
#define OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU3_LOF_OFF         0

/* index definitions for PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG */
#define PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_INDEX_N_MIN                    0
#define PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_INDEX_N_MAX                    3
#define PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_INDEX_N_SIZE                   4
#define PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_INDEX_N_OFFSET                 0x4

/*----------------------------------------------------------.
 | Register (0x000110a4 + (N) * 0x4) RX_LANE_ID_OR_SKEW_REG |
 +----------------------------------------------------------+
 | bit  1:0 R  RX_LOGICAL_LANE_IDENTIFIER                   |
 +---------------------------------------------------------*/
#define PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_UNUSED_MASK                    0xfffffc00
#define OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK 0x00000003
#define OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF 0

/* index definitions for PMC_OPSM23_REG_RX_LANE_INT_REG */
#define PMC_OPSM23_REG_RX_LANE_INT_REG_INDEX_N_MIN                    0
#define PMC_OPSM23_REG_RX_LANE_INT_REG_INDEX_N_MAX                    3
#define PMC_OPSM23_REG_RX_LANE_INT_REG_INDEX_N_SIZE                   4
#define PMC_OPSM23_REG_RX_LANE_INT_REG_INDEX_N_OFFSET                 0x4

/*---------------------------------------------------.
 | Register (0x000110b4 + (N) * 0x4) RX_LANE_INT_REG |
 +---------------------------------------------------+
 | bit  15 R/W  RX_LANE_LOS_INT_I                    |
 | bit  14 R/W  RX_LANE_OOF_INT_I                    |
 | bit  13 R/W  RX_LANE_LOF_LOFLANE_INT_I            |
 | bit  10 R/W  RX_LANE_OTUK_AIS_INT_I               |
 | bit  9  R/W  RX_LANE_OTL_AIS_INT_I                |
 | bit  7  R/W  RX_LANE_ALIGN_CHANGE_INT_I           |
 | bit  5  R/W  RX_LANE_FIFO_ERROR_INT_I             |
 | bit  2  R/W  RX_LANE_OOR_INT_I                    |
 | bit  1  R/W  RX_LANE_LOR_INT_I                    |
 +--------------------------------------------------*/
#define PMC_OPSM23_REG_RX_LANE_INT_REG_UNUSED_MASK                    0xffff0159
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK          0x00008000
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF          15
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK          0x00004000
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF          14
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK  0x00002000
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF  13
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK     0x00000400
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF     10
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK      0x00000200
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF      9
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK 0x00000080
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF 7
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK   0x00000020
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF   5
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK          0x00000004
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF          2
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK          0x00000002
#define OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF          1

/* index definitions for PMC_OPSM23_REG_RX_LANE_INT_EN_REG */
#define PMC_OPSM23_REG_RX_LANE_INT_EN_REG_INDEX_N_MIN                    0
#define PMC_OPSM23_REG_RX_LANE_INT_EN_REG_INDEX_N_MAX                    3
#define PMC_OPSM23_REG_RX_LANE_INT_EN_REG_INDEX_N_SIZE                   4
#define PMC_OPSM23_REG_RX_LANE_INT_EN_REG_INDEX_N_OFFSET                 0x4

/*------------------------------------------------------.
 | Register (0x000110c4 + (N) * 0x4) RX_LANE_INT_EN_REG |
 +------------------------------------------------------+
 | bit  15 R/W  RX_LANE_LOS_INT_E                       |
 | bit  14 R/W  RX_LANE_OOF_INT_E                       |
 | bit  13 R/W  RX_LANE_LOF_LOFLANE_INT_E               |
 | bit  10 R/W  RX_LANE_OTUK_AIS_INT_E                  |
 | bit  9  R/W  RX_LANE_OTL_AIS_INT_E                   |
 | bit  7  R/W  RX_LANE_ALIGN_CHANGE_INT_E              |
 | bit  5  R/W  RX_LANE_FIFO_ERROR_INT_E                |
 | bit  2  R/W  RX_LANE_OOR_INT_E                       |
 | bit  1  R/W  RX_LANE_LOR_INT_E                       |
 +-----------------------------------------------------*/
#define PMC_OPSM23_REG_RX_LANE_INT_EN_REG_UNUSED_MASK                    0xffff0159
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_MSK          0x00008000
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_OFF          15
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_MSK          0x00004000
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_OFF          14
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_MSK  0x00002000
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_OFF  13
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_MSK     0x00000400
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_OFF     10
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_MSK      0x00000200
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_OFF      9
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_MSK 0x00000080
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_OFF 7
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_MSK   0x00000020
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_OFF   5
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_MSK          0x00000004
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_OFF          2
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_MSK          0x00000002
#define OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_OFF          1

/* index definitions for PMC_OPSM23_REG_RX_LANE_INT_VAL_REG */
#define PMC_OPSM23_REG_RX_LANE_INT_VAL_REG_INDEX_N_MIN                    0
#define PMC_OPSM23_REG_RX_LANE_INT_VAL_REG_INDEX_N_MAX                    3
#define PMC_OPSM23_REG_RX_LANE_INT_VAL_REG_INDEX_N_SIZE                   4
#define PMC_OPSM23_REG_RX_LANE_INT_VAL_REG_INDEX_N_OFFSET                 0x4

/*-------------------------------------------------------.
 | Register (0x000110d4 + (N) * 0x4) RX_LANE_INT_VAL_REG |
 +-------------------------------------------------------+
 | bit  15 R  RX_LANE_LOS_INT_V                          |
 | bit  14 R  RX_LANE_OOF_INT_V                          |
 | bit  13 R  RX_LANE_LOF_LOFLANE_INT_V                  |
 | bit  10 R  RX_LANE_OTUK_AIS_INT_V                     |
 | bit  9  R  RX_LANE_OTL_AIS_INT_V                      |
 | bit  7  R  RX_LANE_ALIGN_CHANGE_INT_V                 |
 | bit  5  R  RX_LANE_FIFO_ERROR_INT_V                   |
 | bit  2  R  RX_LANE_OOR_INT_V                          |
 | bit  1  R  RX_LANE_LOR_INT_V                          |
 +------------------------------------------------------*/
#define PMC_OPSM23_REG_RX_LANE_INT_VAL_REG_UNUSED_MASK                    0xffff0159
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_MSK          0x00008000
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_OFF          15
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_MSK          0x00004000
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_OFF          14
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_MSK  0x00002000
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_OFF  13
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_MSK     0x00000400
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_OFF     10
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_MSK      0x00000200
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_OFF      9
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_MSK 0x00000080
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_OFF 7
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_MSK   0x00000020
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_OFF   5
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_MSK          0x00000004
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_OFF          2
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_MSK          0x00000002
#define OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_OFF          1

/*--------------------------------------.
 | Register 0x000110e4 RX_FP_CONFIG_REG |
 +--------------------------------------+
 | bit  12:9 R/W  RX_FP_MFAS_GEN_INVERT |
 | bit  8:5  R/W  RX_FP_MFAS_INVERT     |
 +-------------------------------------*/
#define PMC_OPSM23_REG_RX_FP_CONFIG_REG_UNUSED_MASK               0xffffe000
#define OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_MSK 0x00001e00
#define OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF 9
#define OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_MSK     0x000001e0
#define OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF     5

/*----------------------------------------.
 | Register 0x00011118 OPSM23_INT_SUMMARY |
 +----------------------------------------+
 | bit  7:0 R  INTERRUPT_SUMMARY          |
 +---------------------------------------*/
#define PMC_OPSM23_REG_OPSM23_INT_SUMMARY_UNUSED_MASK           0xffffff00
#define OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK 0x000000ff
#define OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OPSM23_REGS_H */
