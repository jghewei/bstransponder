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
 *     exco3m block
 *****************************************************************************/
#ifndef _EXCO3M_REGS_H
#define _EXCO3M_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_EXCO3M_REG_SW_RESET                     0x00000000
#define PMC_EXCO3M_REG_RX_PUSH_CFG_REG              0x00000010
#define PMC_EXCO3M_REG_TX_GLOBAL_CTRL               0x00000024
#define PMC_EXCO3M_REG_TX_FAULT_CTRL                0x00000028
#define PMC_EXCO3M_REG_TX_INT_VAL                   0x00000030
#define PMC_EXCO3M_REG_TX_INT_STAT                  0x00000034
#define PMC_EXCO3M_REG_TX_INT_EN                    0x00000038
#define PMC_EXCO3M_REG_TX_PCS_BER_CNT_LOWER         0x00000040
#define PMC_EXCO3M_REG_TX_PCS_BER_CNT_UPPER         0x00000044
#define PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER   0x00000048
#define PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER   0x0000004c
#define PMC_EXCO3M_REG_TX_LANE_INFO( N )            (0x00000080 + (N) * 4)
#define PMC_EXCO3M_REG_TX_LANE_INT_VAL( N )         (0x00000100 + (N) * 4)
#define PMC_EXCO3M_REG_TX_LANE_INT_STAT( N )        (0x00000180 + (N) * 4)
#define PMC_EXCO3M_REG_TX_LANE_INT_EN( N )          (0x00000200 + (N) * 4)
#define PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT( N ) (0x00000300 + (N) * 4)
#define PMC_EXCO3M_REG_RX_GLOBAL_CTRL               0x00000424
#define PMC_EXCO3M_REG_RX_FAULT_CTRL                0x00000428
#define PMC_EXCO3M_REG_RX_INT_VAL                   0x00000430
#define PMC_EXCO3M_REG_RX_INT_STAT                  0x00000434
#define PMC_EXCO3M_REG_RX_INT_EN                    0x00000438
#define PMC_EXCO3M_REG_RX_1027B_BER_CNT_LOWER       0x00000440
#define PMC_EXCO3M_REG_RX_1027B_BER_CNT_UPPER       0x00000444
#define PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER   0x00000448
#define PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER   0x0000044c
#define PMC_EXCO3M_REG_RX_LANE_INT_VAL( N )         (0x00000580 + (N) * 4)
#define PMC_EXCO3M_REG_RX_LANE_INT_STAT( N )        (0x00000600 + (N) * 4)
#define PMC_EXCO3M_REG_RX_LANE_INT_EN( N )          (0x00000680 + (N) * 4)
#define PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT( N ) (0x00000700 + (N) * 4)

/*--------------------------------.
 | Register 0x00000000 SW_RESET   |
 +--------------------------------+
 | bit  0 R/W  EXCO_CORE_SW_RESET |
 +-------------------------------*/
#define PMC_EXCO3M_REG_SW_RESET_UNUSED_MASK            0xfffffffe
#define EXCO3M_REG_SW_RESET_BIT_EXCO_CORE_SW_RESET_MSK 0x00000001
#define EXCO3M_REG_SW_RESET_BIT_EXCO_CORE_SW_RESET_OFF 0

/*-------------------------------------.
 | Register 0x00000010 RX_PUSH_CFG_REG |
 +-------------------------------------+
 | bit  0 R/W  RX_PCS_PUSH_ENA         |
 +------------------------------------*/
#define PMC_EXCO3M_REG_RX_PUSH_CFG_REG_UNUSED_MASK         0x7f00fffe
#define EXCO3M_REG_RX_PUSH_CFG_REG_BIT_RX_PCS_PUSH_ENA_MSK 0x00000001
#define EXCO3M_REG_RX_PUSH_CFG_REG_BIT_RX_PCS_PUSH_ENA_OFF 0

/*------------------------------------.
 | Register 0x00000024 TX_GLOBAL_CTRL |
 +------------------------------------+
 | bit  14 R/W  TX_XLGMII_MODE        |
 | bit  1  R/W  TX_PCS_ERR_INS_EN     |
 +-----------------------------------*/
#define PMC_EXCO3M_REG_TX_GLOBAL_CTRL_UNUSED_MASK           0xffff8ef9
#define EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_XLGMII_MODE_MSK    0x00004000
#define EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_XLGMII_MODE_OFF    14
#define EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_PCS_ERR_INS_EN_MSK 0x00000002
#define EXCO3M_REG_TX_GLOBAL_CTRL_BIT_TX_PCS_ERR_INS_EN_OFF 1

/*--------------------------------------.
 | Register 0x00000028 TX_FAULT_CTRL    |
 +--------------------------------------+
 | bit  15 R/W  TX_FORCE_LF_OS          |
 | bit  14 R/W  TX_FORCE_RF_OS          |
 | bit  13 R/W  TX_FORCE_PCS_ERR        |
 | bit  12 R/W  TX_FORCE_IDLE_TEST      |
 | bit  11 R/W  TX_FORCE_LF_OS_LOS      |
 | bit  10 R/W  TX_FORCE_LF_OS_PCS_LOBL |
 | bit  9  R/W  TX_FORCE_LF_OS_LOAM     |
 | bit  8  R/W  TX_FORCE_LF_OS_SKEW     |
 +-------------------------------------*/
#define PMC_EXCO3M_REG_TX_FAULT_CTRL_UNUSED_MASK                 0xfffe00ff
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_MSK          0x00008000
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_OFF          15
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_RF_OS_MSK          0x00004000
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_RF_OS_OFF          14
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_PCS_ERR_MSK        0x00002000
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_PCS_ERR_OFF        13
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_IDLE_TEST_MSK      0x00001000
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_IDLE_TEST_OFF      12
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOS_MSK      0x00000800
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOS_OFF      11
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_PCS_LOBL_MSK 0x00000400
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_PCS_LOBL_OFF 10
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOAM_MSK     0x00000200
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_LOAM_OFF     9
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_SKEW_MSK     0x00000100
#define EXCO3M_REG_TX_FAULT_CTRL_BIT_TX_FORCE_LF_OS_SKEW_OFF     8

/*--------------------------------.
 | Register 0x00000030 TX_INT_VAL |
 +--------------------------------+
 | bit  15 R  TX_LOS_V            |
 | bit  8  R  TX_PCS_HBER_V       |
 +-------------------------------*/
#define PMC_EXCO3M_REG_TX_INT_VAL_UNUSED_MASK       0xffff7eff
#define EXCO3M_REG_TX_INT_VAL_BIT_TX_LOS_V_MSK      0x00008000
#define EXCO3M_REG_TX_INT_VAL_BIT_TX_LOS_V_OFF      15
#define EXCO3M_REG_TX_INT_VAL_BIT_TX_PCS_HBER_V_MSK 0x00000100
#define EXCO3M_REG_TX_INT_VAL_BIT_TX_PCS_HBER_V_OFF 8

/*-------------------------------------.
 | Register 0x00000034 TX_INT_STAT     |
 +-------------------------------------+
 | bit  15 R/W  TX_LOS_I               |
 | bit  14 R/W  TX_LF_OS_DET_I         |
 | bit  13 R/W  TX_RF_OS_DET_I         |
 | bit  12 R/W  TX_IDLE_TEST_ERR_DET_I |
 | bit  11 R/W  TX_SKEW_RANGE_VIOL_I   |
 | bit  10 R/W  TX_PCS_INV_BLK_DET_I   |
 | bit  9  R/W  TX_PCS_ERR_BLK_DET_I   |
 | bit  8  R/W  TX_PCS_HBER_I          |
 +------------------------------------*/
#define PMC_EXCO3M_REG_TX_INT_STAT_UNUSED_MASK                0x07ff00ff
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_MSK               0x00008000
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_LOS_I_OFF               15
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_MSK         0x00004000
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_LF_OS_DET_I_OFF         14
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_MSK         0x00002000
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_RF_OS_DET_I_OFF         13
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_MSK 0x00001000
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_IDLE_TEST_ERR_DET_I_OFF 12
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_MSK   0x00000800
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_SKEW_RANGE_VIOL_I_OFF   11
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_MSK   0x00000400
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_INV_BLK_DET_I_OFF   10
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_MSK   0x00000200
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_ERR_BLK_DET_I_OFF   9
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_MSK          0x00000100
#define EXCO3M_REG_TX_INT_STAT_BIT_TX_PCS_HBER_I_OFF          8

/*-------------------------------------.
 | Register 0x00000038 TX_INT_EN       |
 +-------------------------------------+
 | bit  15 R/W  TX_LOS_E               |
 | bit  14 R/W  TX_LF_OS_DET_E         |
 | bit  13 R/W  TX_RF_OS_DET_E         |
 | bit  12 R/W  TX_IDLE_TEST_ERR_DET_E |
 | bit  11 R/W  TX_SKEW_RANGE_VIOL_E   |
 | bit  10 R/W  TX_PCS_INV_BLK_DET_E   |
 | bit  9  R/W  TX_PCS_ERR_BLK_DET_E   |
 | bit  8  R/W  TX_PCS_HBER_E          |
 +------------------------------------*/
#define PMC_EXCO3M_REG_TX_INT_EN_UNUSED_MASK                0x07ff00ff
#define EXCO3M_REG_TX_INT_EN_BIT_TX_LOS_E_MSK               0x00008000
#define EXCO3M_REG_TX_INT_EN_BIT_TX_LOS_E_OFF               15
#define EXCO3M_REG_TX_INT_EN_BIT_TX_LF_OS_DET_E_MSK         0x00004000
#define EXCO3M_REG_TX_INT_EN_BIT_TX_LF_OS_DET_E_OFF         14
#define EXCO3M_REG_TX_INT_EN_BIT_TX_RF_OS_DET_E_MSK         0x00002000
#define EXCO3M_REG_TX_INT_EN_BIT_TX_RF_OS_DET_E_OFF         13
#define EXCO3M_REG_TX_INT_EN_BIT_TX_IDLE_TEST_ERR_DET_E_MSK 0x00001000
#define EXCO3M_REG_TX_INT_EN_BIT_TX_IDLE_TEST_ERR_DET_E_OFF 12
#define EXCO3M_REG_TX_INT_EN_BIT_TX_SKEW_RANGE_VIOL_E_MSK   0x00000800
#define EXCO3M_REG_TX_INT_EN_BIT_TX_SKEW_RANGE_VIOL_E_OFF   11
#define EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_INV_BLK_DET_E_MSK   0x00000400
#define EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_INV_BLK_DET_E_OFF   10
#define EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_ERR_BLK_DET_E_MSK   0x00000200
#define EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_ERR_BLK_DET_E_OFF   9
#define EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_HBER_E_MSK          0x00000100
#define EXCO3M_REG_TX_INT_EN_BIT_TX_PCS_HBER_E_OFF          8

/*------------------------------------------.
 | Register 0x00000040 TX_PCS_BER_CNT_LOWER |
 +------------------------------------------+
 | bit  15:0 R  TX_PCS_BER_CTR              |
 +-----------------------------------------*/
#define PMC_EXCO3M_REG_TX_PCS_BER_CNT_LOWER_UNUSED_MASK        0xffff0000
#define EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_MSK 0x0000ffff
#define EXCO3M_REG_TX_PCS_BER_CNT_LOWER_BIT_TX_PCS_BER_CTR_OFF 0

/*------------------------------------------.
 | Register 0x00000044 TX_PCS_BER_CNT_UPPER |
 +------------------------------------------+
 | bit  5:0 R  TX_PCS_BER_CTR               |
 +-----------------------------------------*/
#define PMC_EXCO3M_REG_TX_PCS_BER_CNT_UPPER_UNUSED_MASK        0xffffffc0
#define EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_MSK 0x0000003f
#define EXCO3M_REG_TX_PCS_BER_CNT_UPPER_BIT_TX_PCS_BER_CTR_OFF 0

/*------------------------------------------------.
 | Register 0x00000048 TX_PCS_BLOCK_ERR_CNT_LOWER |
 +------------------------------------------------+
 | bit  15:0 R  TX_PCS_BLK_ERR_CTR                |
 +-----------------------------------------------*/
#define PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_UNUSED_MASK            0xffff0000
#define EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_MSK 0x0000ffff
#define EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_LOWER_BIT_TX_PCS_BLK_ERR_CTR_OFF 0

/*------------------------------------------------.
 | Register 0x0000004c TX_PCS_BLOCK_ERR_CNT_UPPER |
 +------------------------------------------------+
 | bit  5:0 R  TX_PCS_BLK_ERR_CTR                 |
 +-----------------------------------------------*/
#define PMC_EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_UNUSED_MASK            0xffffffc0
#define EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_MSK 0x0000003f
#define EXCO3M_REG_TX_PCS_BLOCK_ERR_CNT_UPPER_BIT_TX_PCS_BLK_ERR_CTR_OFF 0

/* index definitions for PMC_EXCO3M_REG_TX_LANE_INFO */
#define PMC_EXCO3M_REG_TX_LANE_INFO_INDEX_N_MIN                0
#define PMC_EXCO3M_REG_TX_LANE_INFO_INDEX_N_MAX                3
#define PMC_EXCO3M_REG_TX_LANE_INFO_INDEX_N_SIZE               4
#define PMC_EXCO3M_REG_TX_LANE_INFO_INDEX_N_OFFSET             4

/*----------------------------------------------.
 | Register (0x00000080 + (N) * 4) TX_LANE_INFO |
 +----------------------------------------------+
 | bit  15:11 R  TX_LANE_ID                     |
 | bit  6:0   R  TX_LANE_SKEW_FIFO_ADDR         |
 +---------------------------------------------*/
#define PMC_EXCO3M_REG_TX_LANE_INFO_UNUSED_MASK                0xffff0780
#define EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_MSK             0x0000f800
#define EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_ID_OFF             11
#define EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_MSK 0x0000007f
#define EXCO3M_REG_TX_LANE_INFO_BIT_TX_LANE_SKEW_FIFO_ADDR_OFF 0

/* index definitions for PMC_EXCO3M_REG_TX_LANE_INT_VAL */
#define PMC_EXCO3M_REG_TX_LANE_INT_VAL_INDEX_N_MIN       0
#define PMC_EXCO3M_REG_TX_LANE_INT_VAL_INDEX_N_MAX       3
#define PMC_EXCO3M_REG_TX_LANE_INT_VAL_INDEX_N_SIZE      4
#define PMC_EXCO3M_REG_TX_LANE_INT_VAL_INDEX_N_OFFSET    4

/*-------------------------------------------------.
 | Register (0x00000100 + (N) * 4) TX_LANE_INT_VAL |
 +-------------------------------------------------+
 | bit  15 R  TX_PCS_LOBL_V                        |
 | bit  12 R  TX_LOAM_V                            |
 +------------------------------------------------*/
#define PMC_EXCO3M_REG_TX_LANE_INT_VAL_UNUSED_MASK       0xffff6fff
#define EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_PCS_LOBL_V_MSK 0x00008000
#define EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_PCS_LOBL_V_OFF 15
#define EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_LOAM_V_MSK     0x00001000
#define EXCO3M_REG_TX_LANE_INT_VAL_BIT_TX_LOAM_V_OFF     12

/* index definitions for PMC_EXCO3M_REG_TX_LANE_INT_STAT */
#define PMC_EXCO3M_REG_TX_LANE_INT_STAT_INDEX_N_MIN              0
#define PMC_EXCO3M_REG_TX_LANE_INT_STAT_INDEX_N_MAX              3
#define PMC_EXCO3M_REG_TX_LANE_INT_STAT_INDEX_N_SIZE             4
#define PMC_EXCO3M_REG_TX_LANE_INT_STAT_INDEX_N_OFFSET           4

/*--------------------------------------------------.
 | Register (0x00000180 + (N) * 4) TX_LANE_INT_STAT |
 +--------------------------------------------------+
 | bit  15 R/W  TX_PCS_LOBL_I                       |
 | bit  14 R/W  TX_PCS_BER_DET_I                    |
 | bit  13 R/W  TX_PCS_BIP_ERR_DET_I                |
 | bit  12 R/W  TX_LOAM_I                           |
 | bit  11 R/W  TX_MRK_INV_DET_I                    |
 +-------------------------------------------------*/
#define PMC_EXCO3M_REG_TX_LANE_INT_STAT_UNUSED_MASK              0x3fff01ff
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_MSK        0x00008000
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_LOBL_I_OFF        15
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_MSK     0x00004000
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BER_DET_I_OFF     14
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_MSK 0x00002000
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_PCS_BIP_ERR_DET_I_OFF 13
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_MSK            0x00001000
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_LOAM_I_OFF            12
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_MSK     0x00000800
#define EXCO3M_REG_TX_LANE_INT_STAT_BIT_TX_MRK_INV_DET_I_OFF     11

/* index definitions for PMC_EXCO3M_REG_TX_LANE_INT_EN */
#define PMC_EXCO3M_REG_TX_LANE_INT_EN_INDEX_N_MIN              0
#define PMC_EXCO3M_REG_TX_LANE_INT_EN_INDEX_N_MAX              3
#define PMC_EXCO3M_REG_TX_LANE_INT_EN_INDEX_N_SIZE             4
#define PMC_EXCO3M_REG_TX_LANE_INT_EN_INDEX_N_OFFSET           4

/*------------------------------------------------.
 | Register (0x00000200 + (N) * 4) TX_LANE_INT_EN |
 +------------------------------------------------+
 | bit  15 R/W  TX_PCS_LOBL_E                     |
 | bit  14 R/W  TX_PCS_BER_DET_E                  |
 | bit  13 R/W  TX_PCS_BIP_ERR_DET_E              |
 | bit  12 R/W  TX_LOAM_E                         |
 | bit  11 R/W  TX_MRK_INV_DET_E                  |
 +-----------------------------------------------*/
#define PMC_EXCO3M_REG_TX_LANE_INT_EN_UNUSED_MASK              0x3fff01ff
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_LOBL_E_MSK        0x00008000
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_LOBL_E_OFF        15
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BER_DET_E_MSK     0x00004000
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BER_DET_E_OFF     14
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BIP_ERR_DET_E_MSK 0x00002000
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_PCS_BIP_ERR_DET_E_OFF 13
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_LOAM_E_MSK            0x00001000
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_LOAM_E_OFF            12
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_MRK_INV_DET_E_MSK     0x00000800
#define EXCO3M_REG_TX_LANE_INT_EN_BIT_TX_MRK_INV_DET_E_OFF     11

/* index definitions for PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT */
#define PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_INDEX_N_MIN            0
#define PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_INDEX_N_MAX            3
#define PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_INDEX_N_SIZE           4
#define PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_INDEX_N_OFFSET         4

/*---------------------------------------------------------.
 | Register (0x00000300 + (N) * 4) TX_LANE_PCS_BIP_ERR_CNT |
 +---------------------------------------------------------+
 | bit  15:0 R  TX_PCS_BIP_ERR_CTR                         |
 +--------------------------------------------------------*/
#define PMC_EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_UNUSED_MASK            0xffff0000
#define EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_MSK 0x0000ffff
#define EXCO3M_REG_TX_LANE_PCS_BIP_ERR_CNT_BIT_TX_PCS_BIP_ERR_CTR_OFF 0

/*------------------------------------.
 | Register 0x00000424 RX_GLOBAL_CTRL |
 +------------------------------------+
 | bit  14 R/W  RX_XLGMII_MODE        |
 | bit  1  R/W  RX_PCS_ERR_INS_EN     |
 +-----------------------------------*/
#define PMC_EXCO3M_REG_RX_GLOBAL_CTRL_UNUSED_MASK           0xffff8e09
#define EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_XLGMII_MODE_MSK    0x00004000
#define EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_XLGMII_MODE_OFF    14
#define EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_PCS_ERR_INS_EN_MSK 0x00000002
#define EXCO3M_REG_RX_GLOBAL_CTRL_BIT_RX_PCS_ERR_INS_EN_OFF 1

/*-----------------------------------.
 | Register 0x00000428 RX_FAULT_CTRL |
 +-----------------------------------+
 | bit  15 R/W  RX_FORCE_LF_OS       |
 | bit  14 R/W  RX_FORCE_RF_OS       |
 | bit  13 R/W  RX_FORCE_PCS_ERR     |
 | bit  12 R/W  RX_FORCE_IDLE_TEST   |
 | bit  11 R/W  RX_FORCE_LF_OS_TSF   |
 | bit  10 R/W  RX_FORCE_LF_OS_LOBL  |
 | bit  9  R/W  RX_FORCE_LF_OS_LOAM  |
 | bit  8  R/W  RX_FORCE_LF_OS_SKEW  |
 +----------------------------------*/
#define PMC_EXCO3M_REG_RX_FAULT_CTRL_UNUSED_MASK             0xfffe00ff
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_MSK      0x00008000
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_OFF      15
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_RF_OS_MSK      0x00004000
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_RF_OS_OFF      14
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_PCS_ERR_MSK    0x00002000
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_PCS_ERR_OFF    13
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_IDLE_TEST_MSK  0x00001000
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_IDLE_TEST_OFF  12
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_TSF_MSK  0x00000800
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_TSF_OFF  11
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOBL_MSK 0x00000400
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOBL_OFF 10
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOAM_MSK 0x00000200
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_LOAM_OFF 9
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_SKEW_MSK 0x00000100
#define EXCO3M_REG_RX_FAULT_CTRL_BIT_RX_FORCE_LF_OS_SKEW_OFF 8

/*--------------------------------.
 | Register 0x00000430 RX_INT_VAL |
 +--------------------------------+
 | bit  15 R  RX_TSF_V            |
 | bit  10 R  RX_1027B_LOBL_V     |
 | bit  9  R  RX_1027B_HBER_V     |
 +-------------------------------*/
#define PMC_EXCO3M_REG_RX_INT_VAL_UNUSED_MASK         0xffff79ff
#define EXCO3M_REG_RX_INT_VAL_BIT_RX_TSF_V_MSK        0x00008000
#define EXCO3M_REG_RX_INT_VAL_BIT_RX_TSF_V_OFF        15
#define EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_LOBL_V_MSK 0x00000400
#define EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_LOBL_V_OFF 10
#define EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_HBER_V_MSK 0x00000200
#define EXCO3M_REG_RX_INT_VAL_BIT_RX_1027B_HBER_V_OFF 9

/*-------------------------------------.
 | Register 0x00000434 RX_INT_STAT     |
 +-------------------------------------+
 | bit  15 R/W  RX_TSF_I               |
 | bit  14 R/W  RX_LF_OS_DET_I         |
 | bit  13 R/W  RX_RF_OS_DET_I         |
 | bit  12 R/W  RX_IDLE_TEST_ERR_DET_I |
 | bit  11 R/W  RX_PCS_ERR_BLK_DET_I   |
 | bit  10 R/W  RX_1027B_LOBL_I        |
 | bit  9  R/W  RX_1027B_HBER_I        |
 | bit  8  R/W  RX_1027B_BER_I         |
 | bit  7  R/W  RX_SKEW_RANGE_VIOL_I   |
 | bit  5  R/W  RX_513B_POS_ERR_DET_I  |
 | bit  4  R/W  RX_WELL_PKT_ERR_I      |
 | bit  2  R/W  RX_LANE_ALGN_MRK_ERR_I |
 | bit  1  R/W  RX_1027B_PAR_ERR_I     |
 +------------------------------------*/
#define PMC_EXCO3M_REG_RX_INT_STAT_UNUSED_MASK                0x00ff0001
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_MSK               0x00008000
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_TSF_I_OFF               15
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_MSK         0x00004000
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_LF_OS_DET_I_OFF         14
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_MSK         0x00002000
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_RF_OS_DET_I_OFF         13
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_MSK 0x00001000
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_IDLE_TEST_ERR_DET_I_OFF 12
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_MSK   0x00000800
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_PCS_ERR_BLK_DET_I_OFF   11
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_MSK        0x00000400
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_LOBL_I_OFF        10
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_MSK        0x00000200
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_HBER_I_OFF        9
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_MSK         0x00000100
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_BER_I_OFF         8
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_MSK   0x00000080
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_SKEW_RANGE_VIOL_I_OFF   7
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_MSK  0x00000020
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_513B_POS_ERR_DET_I_OFF  5
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_MSK      0x00000010
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_WELL_PKT_ERR_I_OFF      4
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_MSK 0x00000004
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_LANE_ALGN_MRK_ERR_I_OFF 2
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_MSK     0x00000002
#define EXCO3M_REG_RX_INT_STAT_BIT_RX_1027B_PAR_ERR_I_OFF     1

/*-------------------------------------.
 | Register 0x00000438 RX_INT_EN       |
 +-------------------------------------+
 | bit  15 R/W  RX_TSF_E               |
 | bit  14 R/W  RX_LF_OS_DET_E         |
 | bit  13 R/W  RX_RF_OS_DET_E         |
 | bit  12 R/W  RX_IDLE_TEST_ERR_DET_E |
 | bit  11 R/W  RX_PCS_ERR_BLK_DET_E   |
 | bit  10 R/W  RX_1027B_LOBL_E        |
 | bit  9  R/W  RX_1027B_HBER_E        |
 | bit  8  R/W  RX_1027B_BER_E         |
 | bit  7  R/W  RX_SKEW_RANGE_VIOL_E   |
 | bit  5  R/W  RX_513B_POS_ERR_DET_E  |
 | bit  4  R/W  RX_WELL_PKT_ERR_E      |
 | bit  2  R/W  RX_LANE_ALGN_MRK_ERR_E |
 | bit  1  R/W  RX_1027B_PAR_ERR_E     |
 +------------------------------------*/
#define PMC_EXCO3M_REG_RX_INT_EN_UNUSED_MASK                0x00ff0001
#define EXCO3M_REG_RX_INT_EN_BIT_RX_TSF_E_MSK               0x00008000
#define EXCO3M_REG_RX_INT_EN_BIT_RX_TSF_E_OFF               15
#define EXCO3M_REG_RX_INT_EN_BIT_RX_LF_OS_DET_E_MSK         0x00004000
#define EXCO3M_REG_RX_INT_EN_BIT_RX_LF_OS_DET_E_OFF         14
#define EXCO3M_REG_RX_INT_EN_BIT_RX_RF_OS_DET_E_MSK         0x00002000
#define EXCO3M_REG_RX_INT_EN_BIT_RX_RF_OS_DET_E_OFF         13
#define EXCO3M_REG_RX_INT_EN_BIT_RX_IDLE_TEST_ERR_DET_E_MSK 0x00001000
#define EXCO3M_REG_RX_INT_EN_BIT_RX_IDLE_TEST_ERR_DET_E_OFF 12
#define EXCO3M_REG_RX_INT_EN_BIT_RX_PCS_ERR_BLK_DET_E_MSK   0x00000800
#define EXCO3M_REG_RX_INT_EN_BIT_RX_PCS_ERR_BLK_DET_E_OFF   11
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_LOBL_E_MSK        0x00000400
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_LOBL_E_OFF        10
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_HBER_E_MSK        0x00000200
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_HBER_E_OFF        9
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_BER_E_MSK         0x00000100
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_BER_E_OFF         8
#define EXCO3M_REG_RX_INT_EN_BIT_RX_SKEW_RANGE_VIOL_E_MSK   0x00000080
#define EXCO3M_REG_RX_INT_EN_BIT_RX_SKEW_RANGE_VIOL_E_OFF   7
#define EXCO3M_REG_RX_INT_EN_BIT_RX_513B_POS_ERR_DET_E_MSK  0x00000020
#define EXCO3M_REG_RX_INT_EN_BIT_RX_513B_POS_ERR_DET_E_OFF  5
#define EXCO3M_REG_RX_INT_EN_BIT_RX_WELL_PKT_ERR_E_MSK      0x00000010
#define EXCO3M_REG_RX_INT_EN_BIT_RX_WELL_PKT_ERR_E_OFF      4
#define EXCO3M_REG_RX_INT_EN_BIT_RX_LANE_ALGN_MRK_ERR_E_MSK 0x00000004
#define EXCO3M_REG_RX_INT_EN_BIT_RX_LANE_ALGN_MRK_ERR_E_OFF 2
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_PAR_ERR_E_MSK     0x00000002
#define EXCO3M_REG_RX_INT_EN_BIT_RX_1027B_PAR_ERR_E_OFF     1

/*--------------------------------------------.
 | Register 0x00000440 RX_1027B_BER_CNT_LOWER |
 +--------------------------------------------+
 | bit  15:0 R  RX_1027B_BER_CTR              |
 +-------------------------------------------*/
#define PMC_EXCO3M_REG_RX_1027B_BER_CNT_LOWER_UNUSED_MASK          0xffff0000
#define EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_MSK 0x0000ffff
#define EXCO3M_REG_RX_1027B_BER_CNT_LOWER_BIT_RX_1027B_BER_CTR_OFF 0

/*--------------------------------------------.
 | Register 0x00000444 RX_1027B_BER_CNT_UPPER |
 +--------------------------------------------+
 | bit  5:0 R  RX_1027B_BER_CTR               |
 +-------------------------------------------*/
#define PMC_EXCO3M_REG_RX_1027B_BER_CNT_UPPER_UNUSED_MASK          0xffffffc0
#define EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_MSK 0x0000003f
#define EXCO3M_REG_RX_1027B_BER_CNT_UPPER_BIT_RX_1027B_BER_CTR_OFF 0

/*------------------------------------------------.
 | Register 0x00000448 RX_PCS_BLOCK_ERR_CNT_LOWER |
 +------------------------------------------------+
 | bit  15:0 R  RX_PCS_BLK_ERR_CTR                |
 +-----------------------------------------------*/
#define PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_UNUSED_MASK            0xffff0000
#define EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_MSK 0x0000ffff
#define EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_LOWER_BIT_RX_PCS_BLK_ERR_CTR_OFF 0

/*------------------------------------------------.
 | Register 0x0000044c RX_PCS_BLOCK_ERR_CNT_UPPER |
 +------------------------------------------------+
 | bit  5:0 R  RX_PCS_BLK_ERR_CTR                 |
 +-----------------------------------------------*/
#define PMC_EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_UNUSED_MASK            0xffffffc0
#define EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_MSK 0x0000003f
#define EXCO3M_REG_RX_PCS_BLOCK_ERR_CNT_UPPER_BIT_RX_PCS_BLK_ERR_CTR_OFF 0

/* index definitions for PMC_EXCO3M_REG_RX_LANE_INT_VAL */
#define PMC_EXCO3M_REG_RX_LANE_INT_VAL_INDEX_N_MIN    0
#define PMC_EXCO3M_REG_RX_LANE_INT_VAL_INDEX_N_MAX    3
#define PMC_EXCO3M_REG_RX_LANE_INT_VAL_INDEX_N_SIZE   4
#define PMC_EXCO3M_REG_RX_LANE_INT_VAL_INDEX_N_OFFSET 4

/*-------------------------------------------------.
 | Register (0x00000580 + (N) * 4) RX_LANE_INT_VAL |
 +-------------------------------------------------+
 | bit  14 R  RX_LOAM_V                            |
 +------------------------------------------------*/
#define PMC_EXCO3M_REG_RX_LANE_INT_VAL_UNUSED_MASK    0xffffbfff
#define EXCO3M_REG_RX_LANE_INT_VAL_BIT_RX_LOAM_V_MSK  0x00004000
#define EXCO3M_REG_RX_LANE_INT_VAL_BIT_RX_LOAM_V_OFF  14

/* index definitions for PMC_EXCO3M_REG_RX_LANE_INT_STAT */
#define PMC_EXCO3M_REG_RX_LANE_INT_STAT_INDEX_N_MIN              0
#define PMC_EXCO3M_REG_RX_LANE_INT_STAT_INDEX_N_MAX              3
#define PMC_EXCO3M_REG_RX_LANE_INT_STAT_INDEX_N_SIZE             4
#define PMC_EXCO3M_REG_RX_LANE_INT_STAT_INDEX_N_OFFSET           4

/*--------------------------------------------------.
 | Register (0x00000600 + (N) * 4) RX_LANE_INT_STAT |
 +--------------------------------------------------+
 | bit  15 R/W  RX_OTN_BIP_ERR_DET_I                |
 | bit  14 R/W  RX_LOAM_I                           |
 | bit  13 R/W  RX_PCS_MRK_INV_DET_I                |
 +-------------------------------------------------*/
#define PMC_EXCO3M_REG_RX_LANE_INT_STAT_UNUSED_MASK              0x0fff1fff
#define EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_MSK 0x00008000
#define EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_OTN_BIP_ERR_DET_I_OFF 15
#define EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_MSK            0x00004000
#define EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_LOAM_I_OFF            14
#define EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_MSK 0x00002000
#define EXCO3M_REG_RX_LANE_INT_STAT_BIT_RX_PCS_MRK_INV_DET_I_OFF 13

/* index definitions for PMC_EXCO3M_REG_RX_LANE_INT_EN */
#define PMC_EXCO3M_REG_RX_LANE_INT_EN_INDEX_N_MIN              0
#define PMC_EXCO3M_REG_RX_LANE_INT_EN_INDEX_N_MAX              3
#define PMC_EXCO3M_REG_RX_LANE_INT_EN_INDEX_N_SIZE             4
#define PMC_EXCO3M_REG_RX_LANE_INT_EN_INDEX_N_OFFSET           4

/*------------------------------------------------.
 | Register (0x00000680 + (N) * 4) RX_LANE_INT_EN |
 +------------------------------------------------+
 | bit  15 R/W  RX_OTN_BIP_ERR_DET_E              |
 | bit  14 R/W  RX_LOAM_E                         |
 | bit  13 R/W  RX_PCS_MRK_INV_DET_E              |
 +-----------------------------------------------*/
#define PMC_EXCO3M_REG_RX_LANE_INT_EN_UNUSED_MASK              0x0fff1fff
#define EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_OTN_BIP_ERR_DET_E_MSK 0x00008000
#define EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_OTN_BIP_ERR_DET_E_OFF 15
#define EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_LOAM_E_MSK            0x00004000
#define EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_LOAM_E_OFF            14
#define EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_PCS_MRK_INV_DET_E_MSK 0x00002000
#define EXCO3M_REG_RX_LANE_INT_EN_BIT_RX_PCS_MRK_INV_DET_E_OFF 13

/* index definitions for PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT */
#define PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_INDEX_N_MIN            0
#define PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_INDEX_N_MAX            3
#define PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_INDEX_N_SIZE           4
#define PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_INDEX_N_OFFSET         4

/*---------------------------------------------------------.
 | Register (0x00000700 + (N) * 4) RX_LANE_OTN_BIP_ERR_CNT |
 +---------------------------------------------------------+
 | bit  15:0 R  RX_OTN_BIP_ERR_CTR                         |
 +--------------------------------------------------------*/
#define PMC_EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_UNUSED_MASK            0xffff0000
#define EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_MSK 0x0000ffff
#define EXCO3M_REG_RX_LANE_OTN_BIP_ERR_CNT_BIT_RX_OTN_BIP_ERR_CTR_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXCO3M_REGS_H */
