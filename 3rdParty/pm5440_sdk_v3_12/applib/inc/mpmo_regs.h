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
 *     mpmo block
 *****************************************************************************/
#ifndef _MPMO_REGS_H
#define _MPMO_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CORE_REG_GEN_CFG                0x00000000
#define PMC_CORE_REG_CSI_CFG                0x00000004
#define PMC_CORE_REG_DATA_AVAIL_CTRL        0x00000008
#define PMC_CORE_REG_AGE_FILT_CTRL          0x0000000c
#define PMC_CORE_REG_DEBUG_CTRL             0x00000028
#define PMC_CORE_REG_DEBUG_DATA             0x0000002c
#define PMC_CORE_REG_DEBUG_DATA_1           0x00000030
#define PMC_CORE_REG_CKCTL_CTRL_REG( N )    (0x00000040 + (N) * 0x4)
#define PMC_CORE_REG_MPMO_DP_CH_CFG_0( N )  (0x00000100 + (N) * 0x10)
#define PMC_CORE_REG_MPMO_DP_CH_CFG_1( N )  (0x00000104 + (N) * 0x10)
#define PMC_CORE_REG_CFG_RAM_1( N )         (0x00001000 + (N) * 0x10)
#define PMC_CORE_REG_CFG_RAM_2( N )         (0x00001004 + (N) * 0x10)
#define PMC_CORE_REG_CFG_RAM_3( N )         (0x00001008 + (N) * 0x10)
#define PMC_CORE_REG_CSI_CHG_INT_REG_0      0x00001600
#define PMC_CORE_REG_CSI_CHG_INT_REG_1      0x00001604
#define PMC_CORE_REG_CSI_CHG_INT_REG_2      0x00001608
#define PMC_CORE_REG_SSF_GEN_INT_REG_0      0x0000160c
#define PMC_CORE_REG_SSF_GEN_INT_REG_1      0x00001610
#define PMC_CORE_REG_SSF_GEN_INT_REG_2      0x00001614
#define PMC_CORE_REG_PKT_DROP_INT_REG_0     0x00001618
#define PMC_CORE_REG_PKT_DROP_INT_REG_1     0x0000161c
#define PMC_CORE_REG_PKT_DROP_INT_REG_2     0x00001620
#define PMC_CORE_REG_PKT_DROP_INT_REG_3     0x00001624
#define PMC_CORE_REG_PKT_DROP_INT_REG_4     0x00001628
#define PMC_CORE_REG_PKT_DROP_INT_REG_5     0x0000162c
#define PMC_CORE_REG_EOP_ERR_INT_REG_0      0x00001630
#define PMC_CORE_REG_EOP_ERR_INT_REG_1      0x00001634
#define PMC_CORE_REG_EOP_ERR_INT_REG_2      0x00001638
#define PMC_CORE_REG_SOP_ERR_INT_REG_0      0x0000163c
#define PMC_CORE_REG_SOP_ERR_INT_REG_1      0x00001640
#define PMC_CORE_REG_SOP_ERR_INT_REG_2      0x00001644
#define PMC_CORE_REG_BWR_RAMP_INT_REG_0     0x00001650
#define PMC_CORE_REG_BWR_RAMP_INT_REG_1     0x00001654
#define PMC_CORE_REG_BWR_RAMP_INT_REG_2     0x00001658
#define PMC_CORE_REG_CSI_CHG_INT_EN_REG_0   0x00001664
#define PMC_CORE_REG_CSI_CHG_INT_EN_REG_1   0x00001680
#define PMC_CORE_REG_CSI_CHG_INT_EN_REG_2   0x00001684
#define PMC_CORE_REG_SSF_GEN_INT_EN_REG_0   0x00001688
#define PMC_CORE_REG_SSF_GEN_INT_EN_REG_1   0x0000168c
#define PMC_CORE_REG_SSF_GEN_INT_EN_REG_2   0x000016a0
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_0  0x000016a4
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_1  0x000016a8
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_2  0x000016ac
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_3  0x000016b0
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_4  0x000016b4
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_5  0x000016b8
#define PMC_CORE_REG_EOP_ERR_INT_EN_REG_0   0x000016bc
#define PMC_CORE_REG_EOP_ERR_INT_EN_REG_1   0x000016c0
#define PMC_CORE_REG_EOP_ERR_INT_EN_REG_2   0x000016c4
#define PMC_CORE_REG_SOP_ERR_INT_EN_REG_0   0x000016c8
#define PMC_CORE_REG_SOP_ERR_INT_EN_REG_1   0x000016cc
#define PMC_CORE_REG_SOP_ERR_INT_EN_REG_2   0x000016d0
#define PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0  0x00001708
#define PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1  0x0000170c
#define PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2  0x00001710
#define PMC_CORE_REG_BWR_RAMP_INT_V_REG_0   0x00001720
#define PMC_CORE_REG_BWR_RAMP_INT_V_REG_1   0x00001724
#define PMC_CORE_REG_BWR_RAMP_INT_V_REG_2   0x00001728
#define PMC_CORE_REG_SSF_STATE_INT_REG_0    0x00001730
#define PMC_CORE_REG_SSF_STATE_INT_REG_1    0x00001734
#define PMC_CORE_REG_SSF_STATE_INT_REG_2    0x00001738
#define PMC_CORE_REG_SSF_STATE_INT_EN_REG_0 0x0000173c
#define PMC_CORE_REG_SSF_STATE_INT_EN_REG_1 0x00001740
#define PMC_CORE_REG_SSF_STATE_INT_EN_REG_2 0x00001744
#define PMC_CORE_REG_SSF_STATE_INT_V_REG_0  0x00001748
#define PMC_CORE_REG_SSF_STATE_INT_V_REG_1  0x0000174c
#define PMC_CORE_REG_SSF_STATE_INT_V_REG_2  0x00001750

/*--------------------------------------.
 | Register 0x00000000 GEN_CFG          |
 +--------------------------------------+
 | bit  30    R/W  RAMP_EXTEND_EN       |
 | bit  28    R/W  ZONE_SEL             |
 | bit  24    R/W  SEQ_CHK_EN           |
 | bit  21:20 R/W  PACER_MODE           |
 | bit  19    R/W  BWR_IND_MODE         |
 | bit  18    R/W  RAMP_STOP_MODE       |
 | bit  17    R/W  AGE_SQUELCH_EN       |
 | bit  16    R/W  CN_SQUELCH_EN        |
 | bit  14    R/W  AGE_FILTER_EN        |
 | bit  13    R/W  CN_BIT_MODE          |
 | bit  12    R/W  SSF_GEN_EN           |
 | bit  10    R/W  SSF_THRES_EN         |
 | bit  9:8   R/W  BUS_WIDTH            |
 | bit  5     R/W  DPI_DYNAMIC_LS_EN    |
 | bit  4     R/W  DCI_DYNAMIC_LS_EN    |
 | bit  0     R/W  MPMO_EN              |
 | bit  29    R/W  CN_TARGET_DECIMAL_EN |
 +-------------------------------------*/
#define PMC_CORE_REG_GEN_CFG_UNUSED_MASK              0x8ec088ce
#define CORE_REG_GEN_CFG_BIT_RAMP_EXTEND_EN_MSK       0x40000000
#define CORE_REG_GEN_CFG_BIT_RAMP_EXTEND_EN_OFF       30
#define CORE_REG_GEN_CFG_BIT_ZONE_SEL_MSK             0x10000000
#define CORE_REG_GEN_CFG_BIT_ZONE_SEL_OFF             28
#define CORE_REG_GEN_CFG_BIT_SEQ_CHK_EN_MSK           0x01000000
#define CORE_REG_GEN_CFG_BIT_SEQ_CHK_EN_OFF           24
#define CORE_REG_GEN_CFG_BIT_PACER_MODE_MSK           0x00300000
#define CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF           20
#define CORE_REG_GEN_CFG_BIT_BWR_IND_MODE_MSK         0x00080000
#define CORE_REG_GEN_CFG_BIT_BWR_IND_MODE_OFF         19
#define CORE_REG_GEN_CFG_BIT_RAMP_STOP_MODE_MSK       0x00040000
#define CORE_REG_GEN_CFG_BIT_RAMP_STOP_MODE_OFF       18
#define CORE_REG_GEN_CFG_BIT_AGE_SQUELCH_EN_MSK       0x00020000
#define CORE_REG_GEN_CFG_BIT_AGE_SQUELCH_EN_OFF       17
#define CORE_REG_GEN_CFG_BIT_CN_SQUELCH_EN_MSK        0x00010000
#define CORE_REG_GEN_CFG_BIT_CN_SQUELCH_EN_OFF        16
#define CORE_REG_GEN_CFG_BIT_AGE_FILTER_EN_MSK        0x00004000
#define CORE_REG_GEN_CFG_BIT_AGE_FILTER_EN_OFF        14
#define CORE_REG_GEN_CFG_BIT_CN_BIT_MODE_MSK          0x00002000
#define CORE_REG_GEN_CFG_BIT_CN_BIT_MODE_OFF          13
#define CORE_REG_GEN_CFG_BIT_SSF_GEN_EN_MSK           0x00001000
#define CORE_REG_GEN_CFG_BIT_SSF_GEN_EN_OFF           12
#define CORE_REG_GEN_CFG_BIT_SSF_THRES_EN_MSK         0x00000400
#define CORE_REG_GEN_CFG_BIT_SSF_THRES_EN_OFF         10
#define CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK            0x00000300
#define CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF            8
#define CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_MSK    0x00000020
#define CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_OFF    5
#define CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_MSK    0x00000010
#define CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_OFF    4
#define CORE_REG_GEN_CFG_BIT_MPMO_EN_MSK              0x00000001
#define CORE_REG_GEN_CFG_BIT_MPMO_EN_OFF              0
#define CORE_REG_GEN_CFG_BIT_CN_TARGET_DECIMAL_EN_MSK 0x20000000
#define CORE_REG_GEN_CFG_BIT_CN_TARGET_DECIMAL_EN_OFF 29

/*-----------------------------.
 | Register 0x00000004 CSI_CFG |
 +-----------------------------+
 | bit  31  R/W  CSI_MASK_EN   |
 | bit  4:0 R/W  CSI_NORMAL    |
 +----------------------------*/
#define PMC_CORE_REG_CSI_CFG_UNUSED_MASK     0x7fffffe0
#define CORE_REG_CSI_CFG_BIT_CSI_MASK_EN_MSK 0x80000000
#define CORE_REG_CSI_CFG_BIT_CSI_MASK_EN_OFF 31
#define CORE_REG_CSI_CFG_BIT_CSI_NORMAL_MSK  0x0000001f
#define CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF  0

/*-------------------------------------.
 | Register 0x00000008 DATA_AVAIL_CTRL |
 +-------------------------------------+
 | bit  28  R/W  DATA_AVAIL_CONT_HO_EN |
 | bit  24  R/W  DATA_AVAIL_SQUELCH_EN |
 | bit  20  R/W  HOLDOFF_ENB           |
 | bit  16  R/W  FORCE_DATA_AVAIL_EN   |
 | bit  9:0 R/W  BURST_SIZE            |
 +------------------------------------*/
#define PMC_CORE_REG_DATA_AVAIL_CTRL_UNUSED_MASK               0xeeeefc00
#define CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_CONT_HO_EN_MSK 0x10000000
#define CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_CONT_HO_EN_OFF 28
#define CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_SQUELCH_EN_MSK 0x01000000
#define CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_SQUELCH_EN_OFF 24
#define CORE_REG_DATA_AVAIL_CTRL_BIT_HOLDOFF_ENB_MSK           0x00100000
#define CORE_REG_DATA_AVAIL_CTRL_BIT_HOLDOFF_ENB_OFF           20
#define CORE_REG_DATA_AVAIL_CTRL_BIT_FORCE_DATA_AVAIL_EN_MSK   0x00010000
#define CORE_REG_DATA_AVAIL_CTRL_BIT_FORCE_DATA_AVAIL_EN_OFF   16
#define CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_MSK            0x000003ff
#define CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF            0

/*-----------------------------------.
 | Register 0x0000000c AGE_FILT_CTRL |
 +-----------------------------------+
 | bit  23:0 R/W  AGE_FILT_PERIOD    |
 +----------------------------------*/
#define PMC_CORE_REG_AGE_FILT_CTRL_UNUSED_MASK         0xff000000
#define CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_MSK 0x00ffffff
#define CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF 0

/*--------------------------------------.
 | Register 0x00000028 DEBUG_CTRL       |
 +--------------------------------------+
 | bit  28    R/W  DEBUG_DATA_REQ_ABORT |
 | bit  24    R/W  DEBUG_DATA_REQ       |
 | bit  16:22 R/W  DEBUG_DATA_REQ_ID    |
 +-------------------------------------*/
#define PMC_CORE_REG_DEBUG_CTRL_UNUSED_MASK              0xee80ffff
#define CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_MSK 0x10000000
#define CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_OFF 28
#define CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_MSK       0x01000000
#define CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_OFF       24
#define CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_MSK    0x007f0000
#define CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF    16

/*--------------------------------.
 | Register 0x0000002c DEBUG_DATA |
 +--------------------------------+
 | bit  16:15 R  RAMP_STATE       |
 | bit  4:0   R  CURR_CSI         |
 +-------------------------------*/
#define PMC_CORE_REG_DEBUG_DATA_UNUSED_MASK    0xfffe7fe0
#define CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_MSK 0x00018000
#define CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_OFF 15
#define CORE_REG_DEBUG_DATA_BIT_CURR_CSI_MSK   0x0000001f
#define CORE_REG_DEBUG_DATA_BIT_CURR_CSI_OFF   0

/*----------------------------------.
 | Register 0x00000030 DEBUG_DATA_1 |
 +----------------------------------+
 | bit  15:0 R  PKT_AGE             |
 +---------------------------------*/
#define PMC_CORE_REG_DEBUG_DATA_1_UNUSED_MASK 0xffff0000
#define CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_MSK 0x0000ffff
#define CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_OFF 0

/* index definitions for PMC_CORE_REG_CKCTL_CTRL_REG */
#define PMC_CORE_REG_CKCTL_CTRL_REG_INDEX_N_MIN       0
#define PMC_CORE_REG_CKCTL_CTRL_REG_INDEX_N_MAX       25
#define PMC_CORE_REG_CKCTL_CTRL_REG_INDEX_N_SIZE      26
#define PMC_CORE_REG_CKCTL_CTRL_REG_INDEX_N_OFFSET    0x4

/*--------------------------------------------------.
 | Register (0x00000040 + (N) * 0x4) CKCTL_CTRL_REG |
 +--------------------------------------------------+
 | bit  16  R/W  CKCTL_EN                           |
 | bit  0:6 R/W  CKCTL_PORT_ID                      |
 +-------------------------------------------------*/
#define PMC_CORE_REG_CKCTL_CTRL_REG_UNUSED_MASK       0xfffeff80
#define CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_EN_MSK      0x00010000
#define CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_EN_OFF      16
#define CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_MSK 0x0000007f
#define CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF 0

/* index definitions for PMC_CORE_REG_MPMO_DP_CH_CFG_0 */
#define PMC_CORE_REG_MPMO_DP_CH_CFG_0_INDEX_N_MIN            0
#define PMC_CORE_REG_MPMO_DP_CH_CFG_0_INDEX_N_MAX            95
#define PMC_CORE_REG_MPMO_DP_CH_CFG_0_INDEX_N_SIZE           96
#define PMC_CORE_REG_MPMO_DP_CH_CFG_0_INDEX_N_OFFSET         0x10

/*-----------------------------------------------------.
 | Register (0x00000100 + (N) * 0x10) MPMO_DP_CH_CFG_0 |
 +-----------------------------------------------------+
 | bit  0:11  R/W  XOFF_THRES                          |
 | bit  28    R/W  DATA_AVAIL_CTRL_EN                  |
 | bit  23:16 R/W  DATA_AVAIL_THRES                    |
 +----------------------------------------------------*/
#define PMC_CORE_REG_MPMO_DP_CH_CFG_0_UNUSED_MASK            0xef00f000
#define CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_MSK         0x00000fff
#define CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF         0
#define CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_CTRL_EN_MSK 0x10000000
#define CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_CTRL_EN_OFF 28
#define CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_MSK   0x00ff0000
#define CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF   16

/* index definitions for PMC_CORE_REG_MPMO_DP_CH_CFG_1 */
#define PMC_CORE_REG_MPMO_DP_CH_CFG_1_INDEX_N_MIN          0
#define PMC_CORE_REG_MPMO_DP_CH_CFG_1_INDEX_N_MAX          95
#define PMC_CORE_REG_MPMO_DP_CH_CFG_1_INDEX_N_SIZE         96
#define PMC_CORE_REG_MPMO_DP_CH_CFG_1_INDEX_N_OFFSET       0x10

/*-----------------------------------------------------.
 | Register (0x00000104 + (N) * 0x10) MPMO_DP_CH_CFG_1 |
 +-----------------------------------------------------+
 | bit  28    R/W  HOLDOFF_TRIG                        |
 | bit  25:16 R/W  PKT_SIZE                            |
 | bit  12    R/W  SSF_CN_CENTER_EN                    |
 | bit  8     R/W  TS_ENABLE                           |
 | bit  4     R/W  RAMP_EN                             |
 | bit  0     R/W  CH_EN                               |
 | bit  2     R/W  RAMP_UP_DWN2                        |
 +----------------------------------------------------*/
#define PMC_CORE_REG_MPMO_DP_CH_CFG_1_UNUSED_MASK          0xec00eeea
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_HOLDOFF_TRIG_MSK     0x10000000
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_HOLDOFF_TRIG_OFF     28
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_MSK         0x03ff0000
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF         16
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_SSF_CN_CENTER_EN_MSK 0x00001000
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_SSF_CN_CENTER_EN_OFF 12
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_TS_ENABLE_MSK        0x00000100
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_TS_ENABLE_OFF        8
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_EN_MSK          0x00000010
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_EN_OFF          4
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_CH_EN_MSK            0x00000001
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_CH_EN_OFF            0
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_UP_DWN2_MSK     0x00000004
#define CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_UP_DWN2_OFF     2

/* index definitions for PMC_CORE_REG_CFG_RAM_1 */
#define PMC_CORE_REG_CFG_RAM_1_INDEX_N_MIN    0
#define PMC_CORE_REG_CFG_RAM_1_INDEX_N_MAX    95
#define PMC_CORE_REG_CFG_RAM_1_INDEX_N_SIZE   96
#define PMC_CORE_REG_CFG_RAM_1_INDEX_N_OFFSET 0x10

/*----------------------------------------------.
 | Register (0x00001000 + (N) * 0x10) CFG_RAM_1 |
 +----------------------------------------------+
 | bit  31:16 R/W  AGE_IDEAL                    |
 | bit  15:0  R/W  AGE_OLD                      |
 +---------------------------------------------*/
#define PMC_CORE_REG_CFG_RAM_1_UNUSED_MASK    0x00000000
#define CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_MSK  0xffff0000
#define CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF  16
#define CORE_REG_CFG_RAM_1_BIT_AGE_OLD_MSK    0x0000ffff
#define CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF    0

/* index definitions for PMC_CORE_REG_CFG_RAM_2 */
#define PMC_CORE_REG_CFG_RAM_2_INDEX_N_MIN       0
#define PMC_CORE_REG_CFG_RAM_2_INDEX_N_MAX       95
#define PMC_CORE_REG_CFG_RAM_2_INDEX_N_SIZE      96
#define PMC_CORE_REG_CFG_RAM_2_INDEX_N_OFFSET    0x10

/*----------------------------------------------.
 | Register (0x00001004 + (N) * 0x10) CFG_RAM_2 |
 +----------------------------------------------+
 | bit  31:16 R/W  AGE_THRES_3_2                |
 | bit  15:0  R/W  AGE_THRES_1_0                |
 +---------------------------------------------*/
#define PMC_CORE_REG_CFG_RAM_2_UNUSED_MASK       0x00000000
#define CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_MSK 0xffff0000
#define CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF 16
#define CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_MSK 0x0000ffff
#define CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF 0

/* index definitions for PMC_CORE_REG_CFG_RAM_3 */
#define PMC_CORE_REG_CFG_RAM_3_INDEX_N_MIN           0
#define PMC_CORE_REG_CFG_RAM_3_INDEX_N_MAX           95
#define PMC_CORE_REG_CFG_RAM_3_INDEX_N_SIZE          96
#define PMC_CORE_REG_CFG_RAM_3_INDEX_N_OFFSET        0x10

/*----------------------------------------------.
 | Register (0x00001008 + (N) * 0x10) CFG_RAM_3 |
 +----------------------------------------------+
 | bit  24:24 R/W  RAMP_UP_DWN                  |
 | bit  0:23  R/W  CN_TARGET                    |
 | bit  31:8  R/W  CN_TARGET2                   |
 | bit  7:0   R/W  CN_TARGET_DECIMAL            |
 +---------------------------------------------*/
#define PMC_CORE_REG_CFG_RAM_3_UNUSED_MASK           0x00000000
#define CORE_REG_CFG_RAM_3_BIT_RAMP_UP_DWN_MSK       0x01000000
#define CORE_REG_CFG_RAM_3_BIT_RAMP_UP_DWN_OFF       24
#define CORE_REG_CFG_RAM_3_BIT_CN_TARGET_MSK         0x00ffffff
#define CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF         0
#define CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_MSK        0xffffff00
#define CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF        8
#define CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_MSK 0x000000ff
#define CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF 0

/*---------------------------------------.
 | Register 0x00001600 CSI_CHG_INT_REG_0 |
 +---------------------------------------+
 | bit  0:31 R/W  CSI_CHG_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_CSI_CHG_INT_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_MSK 0xffffffff
#define CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF 0

/*---------------------------------------.
 | Register 0x00001604 CSI_CHG_INT_REG_1 |
 +---------------------------------------+
 | bit  31:0 R/W  CSI_CHG_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_CSI_CHG_INT_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_MSK 0xffffffff
#define CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF 0

/*---------------------------------------.
 | Register 0x00001608 CSI_CHG_INT_REG_2 |
 +---------------------------------------+
 | bit  31:0 R/W  CSI_CHG_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_CSI_CHG_INT_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_MSK 0xffffffff
#define CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF 0

/*---------------------------------------.
 | Register 0x0000160c SSF_GEN_INT_REG_0 |
 +---------------------------------------+
 | bit  0:31 R/W  SSF_GEN_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_SSF_GEN_INT_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_MSK 0xffffffff
#define CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF 0

/*---------------------------------------.
 | Register 0x00001610 SSF_GEN_INT_REG_1 |
 +---------------------------------------+
 | bit  31:0 R/W  SSF_GEN_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_SSF_GEN_INT_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_MSK 0xffffffff
#define CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF 0

/*---------------------------------------.
 | Register 0x00001614 SSF_GEN_INT_REG_2 |
 +---------------------------------------+
 | bit  31:0 R/W  SSF_GEN_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_SSF_GEN_INT_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_MSK 0xffffffff
#define CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF 0

/*----------------------------------------.
 | Register 0x00001618 PKT_DROP_INT_REG_0 |
 +----------------------------------------+
 | bit  0:31 R/W  PKT_DROP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_REG_0_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF 0

/*----------------------------------------.
 | Register 0x0000161c PKT_DROP_INT_REG_1 |
 +----------------------------------------+
 | bit  31:0 R/W  PKT_DROP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_REG_1_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF 0

/*----------------------------------------.
 | Register 0x00001620 PKT_DROP_INT_REG_2 |
 +----------------------------------------+
 | bit  31:0 R/W  PKT_DROP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_REG_2_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF 0

/*----------------------------------------.
 | Register 0x00001624 PKT_DROP_INT_REG_3 |
 +----------------------------------------+
 | bit  31:0 R/W  PKT_DROP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_REG_3_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF 0

/*----------------------------------------.
 | Register 0x00001628 PKT_DROP_INT_REG_4 |
 +----------------------------------------+
 | bit  31:0 R/W  PKT_DROP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_REG_4_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF 0

/*----------------------------------------.
 | Register 0x0000162c PKT_DROP_INT_REG_5 |
 +----------------------------------------+
 | bit  31:0 R/W  PKT_DROP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_REG_5_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF 0

/*---------------------------------------.
 | Register 0x00001630 EOP_ERR_INT_REG_0 |
 +---------------------------------------+
 | bit  0:31 R/W  EOP_ERR_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_EOP_ERR_INT_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_MSK 0xffffffff
#define CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF 0

/*---------------------------------------.
 | Register 0x00001634 EOP_ERR_INT_REG_1 |
 +---------------------------------------+
 | bit  31:0 R/W  EOP_ERR_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_EOP_ERR_INT_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_MSK 0xffffffff
#define CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF 0

/*---------------------------------------.
 | Register 0x00001638 EOP_ERR_INT_REG_2 |
 +---------------------------------------+
 | bit  31:0 R/W  EOP_ERR_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_EOP_ERR_INT_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_MSK 0xffffffff
#define CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF 0

/*---------------------------------------.
 | Register 0x0000163c SOP_ERR_INT_REG_0 |
 +---------------------------------------+
 | bit  0:31 R/W  SOP_ERR_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_SOP_ERR_INT_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_MSK 0xffffffff
#define CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF 0

/*---------------------------------------.
 | Register 0x00001640 SOP_ERR_INT_REG_1 |
 +---------------------------------------+
 | bit  31:0 R/W  SOP_ERR_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_SOP_ERR_INT_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_MSK 0xffffffff
#define CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF 0

/*---------------------------------------.
 | Register 0x00001644 SOP_ERR_INT_REG_2 |
 +---------------------------------------+
 | bit  31:0 R/W  SOP_ERR_I              |
 +--------------------------------------*/
#define PMC_CORE_REG_SOP_ERR_INT_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_MSK 0xffffffff
#define CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF 0

/*----------------------------------------.
 | Register 0x00001650 BWR_RAMP_INT_REG_0 |
 +----------------------------------------+
 | bit  0:31 R/W  BWR_RAMP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_REG_0_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF 0

/*----------------------------------------.
 | Register 0x00001654 BWR_RAMP_INT_REG_1 |
 +----------------------------------------+
 | bit  31:0 R/W  BWR_RAMP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_REG_1_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF 0

/*----------------------------------------.
 | Register 0x00001658 BWR_RAMP_INT_REG_2 |
 +----------------------------------------+
 | bit  31:0 R/W  BWR_RAMP_I              |
 +---------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_REG_2_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF 0

/*------------------------------------------.
 | Register 0x00001664 CSI_CHG_INT_EN_REG_0 |
 +------------------------------------------+
 | bit  0:31 R/W  CSI_CHG_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_CSI_CHG_INT_EN_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_MSK 0xffffffff
#define CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF 0

/*------------------------------------------.
 | Register 0x00001680 CSI_CHG_INT_EN_REG_1 |
 +------------------------------------------+
 | bit  31:0 R/W  CSI_CHG_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_CSI_CHG_INT_EN_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_MSK 0xffffffff
#define CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF 0

/*------------------------------------------.
 | Register 0x00001684 CSI_CHG_INT_EN_REG_2 |
 +------------------------------------------+
 | bit  31:0 R/W  CSI_CHG_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_CSI_CHG_INT_EN_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_MSK 0xffffffff
#define CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF 0

/*------------------------------------------.
 | Register 0x00001688 SSF_GEN_INT_EN_REG_0 |
 +------------------------------------------+
 | bit  0:31 R/W  SSF_GEN_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_SSF_GEN_INT_EN_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_MSK 0xffffffff
#define CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF 0

/*------------------------------------------.
 | Register 0x0000168c SSF_GEN_INT_EN_REG_1 |
 +------------------------------------------+
 | bit  31:0 R/W  SSF_GEN_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_SSF_GEN_INT_EN_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_MSK 0xffffffff
#define CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF 0

/*------------------------------------------.
 | Register 0x000016a0 SSF_GEN_INT_EN_REG_2 |
 +------------------------------------------+
 | bit  31:0 R/W  SSF_GEN_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_SSF_GEN_INT_EN_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_MSK 0xffffffff
#define CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF 0

/*-------------------------------------------.
 | Register 0x000016a4 PKT_DROP_INT_EN_REG_0 |
 +-------------------------------------------+
 | bit  0:31 R/W  PKT_DROP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_0_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF 0

/*-------------------------------------------.
 | Register 0x000016a8 PKT_DROP_INT_EN_REG_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  PKT_DROP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_1_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF 0

/*-------------------------------------------.
 | Register 0x000016ac PKT_DROP_INT_EN_REG_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  PKT_DROP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_2_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF 0

/*-------------------------------------------.
 | Register 0x000016b0 PKT_DROP_INT_EN_REG_3 |
 +-------------------------------------------+
 | bit  31:0 R/W  PKT_DROP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_3_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF 0

/*-------------------------------------------.
 | Register 0x000016b4 PKT_DROP_INT_EN_REG_4 |
 +-------------------------------------------+
 | bit  31:0 R/W  PKT_DROP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_4_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF 0

/*-------------------------------------------.
 | Register 0x000016b8 PKT_DROP_INT_EN_REG_5 |
 +-------------------------------------------+
 | bit  31:0 R/W  PKT_DROP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_PKT_DROP_INT_EN_REG_5_UNUSED_MASK    0x00000000
#define CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_MSK 0xffffffff
#define CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF 0

/*------------------------------------------.
 | Register 0x000016bc EOP_ERR_INT_EN_REG_0 |
 +------------------------------------------+
 | bit  0:31 R/W  EOP_ERR_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_EOP_ERR_INT_EN_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_MSK 0xffffffff
#define CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF 0

/*------------------------------------------.
 | Register 0x000016c0 EOP_ERR_INT_EN_REG_1 |
 +------------------------------------------+
 | bit  31:0 R/W  EOP_ERR_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_EOP_ERR_INT_EN_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_MSK 0xffffffff
#define CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF 0

/*------------------------------------------.
 | Register 0x000016c4 EOP_ERR_INT_EN_REG_2 |
 +------------------------------------------+
 | bit  31:0 R/W  EOP_ERR_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_EOP_ERR_INT_EN_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_MSK 0xffffffff
#define CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF 0

/*------------------------------------------.
 | Register 0x000016c8 SOP_ERR_INT_EN_REG_0 |
 +------------------------------------------+
 | bit  0:31 R/W  SOP_ERR_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_SOP_ERR_INT_EN_REG_0_UNUSED_MASK   0x00000000
#define CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_MSK 0xffffffff
#define CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF 0

/*------------------------------------------.
 | Register 0x000016cc SOP_ERR_INT_EN_REG_1 |
 +------------------------------------------+
 | bit  31:0 R/W  SOP_ERR_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_SOP_ERR_INT_EN_REG_1_UNUSED_MASK   0x00000000
#define CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_MSK 0xffffffff
#define CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF 0

/*------------------------------------------.
 | Register 0x000016d0 SOP_ERR_INT_EN_REG_2 |
 +------------------------------------------+
 | bit  31:0 R/W  SOP_ERR_E                 |
 +-----------------------------------------*/
#define PMC_CORE_REG_SOP_ERR_INT_EN_REG_2_UNUSED_MASK   0x00000000
#define CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_MSK 0xffffffff
#define CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF 0

/*-------------------------------------------.
 | Register 0x00001708 BWR_RAMP_INT_EN_REG_0 |
 +-------------------------------------------+
 | bit  0:31 R/W  BWR_RAMP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF 0

/*-------------------------------------------.
 | Register 0x0000170c BWR_RAMP_INT_EN_REG_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  BWR_RAMP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF 0

/*-------------------------------------------.
 | Register 0x00001710 BWR_RAMP_INT_EN_REG_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  BWR_RAMP_E                 |
 +------------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF 0

/*------------------------------------------.
 | Register 0x00001720 BWR_RAMP_INT_V_REG_0 |
 +------------------------------------------+
 | bit  0:31 R  BWR_RAMP_V                  |
 +-----------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_V_REG_0_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF 0

/*------------------------------------------.
 | Register 0x00001724 BWR_RAMP_INT_V_REG_1 |
 +------------------------------------------+
 | bit  31:0 R  BWR_RAMP_V                  |
 +-----------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_V_REG_1_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF 0

/*------------------------------------------.
 | Register 0x00001728 BWR_RAMP_INT_V_REG_2 |
 +------------------------------------------+
 | bit  31:0 R  BWR_RAMP_V                  |
 +-----------------------------------------*/
#define PMC_CORE_REG_BWR_RAMP_INT_V_REG_2_UNUSED_MASK    0x00000000
#define CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_MSK 0xffffffff
#define CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF 0

/*-----------------------------------------.
 | Register 0x00001730 SSF_STATE_INT_REG_0 |
 +-----------------------------------------+
 | bit  31:0 R/W  SSF_STATE_I              |
 +----------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_REG_0_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF 0

/*-----------------------------------------.
 | Register 0x00001734 SSF_STATE_INT_REG_1 |
 +-----------------------------------------+
 | bit  31:0 R/W  SSF_STATE_I              |
 +----------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_REG_1_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF 0

/*-----------------------------------------.
 | Register 0x00001738 SSF_STATE_INT_REG_2 |
 +-----------------------------------------+
 | bit  0:31 R/W  SSF_STATE_I              |
 +----------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_REG_2_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF 0

/*--------------------------------------------.
 | Register 0x0000173c SSF_STATE_INT_EN_REG_0 |
 +--------------------------------------------+
 | bit  31:0 R/W  SSF_STATE_E                 |
 +-------------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_EN_REG_0_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF 0

/*--------------------------------------------.
 | Register 0x00001740 SSF_STATE_INT_EN_REG_1 |
 +--------------------------------------------+
 | bit  31:0 R/W  SSF_STATE_E                 |
 +-------------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_EN_REG_1_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF 0

/*--------------------------------------------.
 | Register 0x00001744 SSF_STATE_INT_EN_REG_2 |
 +--------------------------------------------+
 | bit  0:31 R/W  SSF_STATE_E                 |
 +-------------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_EN_REG_2_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF 0

/*-------------------------------------------.
 | Register 0x00001748 SSF_STATE_INT_V_REG_0 |
 +-------------------------------------------+
 | bit  31:0 R  SSF_STATE_V                  |
 +------------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_V_REG_0_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF 0

/*-------------------------------------------.
 | Register 0x0000174c SSF_STATE_INT_V_REG_1 |
 +-------------------------------------------+
 | bit  31:0 R  SSF_STATE_V                  |
 +------------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_V_REG_1_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF 0

/*-------------------------------------------.
 | Register 0x00001750 SSF_STATE_INT_V_REG_2 |
 +-------------------------------------------+
 | bit  0:31 R  SSF_STATE_V                  |
 +------------------------------------------*/
#define PMC_CORE_REG_SSF_STATE_INT_V_REG_2_UNUSED_MASK     0x00000000
#define CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_MSK 0xffffffff
#define CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMO_REGS_H */
