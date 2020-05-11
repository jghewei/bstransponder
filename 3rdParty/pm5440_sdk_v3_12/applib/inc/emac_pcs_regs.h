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
 *     emac_pcs block
 *****************************************************************************/
#ifndef _EMAC_PCS_REGS_H
#define _EMAC_PCS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PCS_REG_CTRL_AND_STAT                  0x0000c000
#define PMC_PCS_REG_STAT_1_2_                      0x0000c004
#define PMC_PCS_REG_DEVICE_ID_1_2_                 0x0000c008
#define PMC_PCS_REG_DEVICE_ID_2_2_                 0x0000c00c
#define PMC_PCS_REG_CTRL_2_2_                      0x0000c01c
#define PMC_PCS_REG_STAT_2_2_                      0x0000c020
#define PMC_PCS_REG_PACKAGE_ID_1_2_                0x0000c038
#define PMC_PCS_REG_PACKAGE_ID_2_2_                0x0000c03c
#define PMC_PCS_REG_BASE_R_STAT_1_2_               0x0000c080
#define PMC_PCS_REG_BASE_R_STAT_2_2_               0x0000c084
#define PMC_PCS_REG_BASE_R_SEED_A_1_4_             0x0000c088
#define PMC_PCS_REG_BASE_R_SEED_A_2_4_             0x0000c08c
#define PMC_PCS_REG_BASE_R_SEED_A_3_4_             0x0000c090
#define PMC_PCS_REG_BASE_R_SEED_A_4_4_             0x0000c094
#define PMC_PCS_REG_BASE_R_SEED_B_1_4_             0x0000c098
#define PMC_PCS_REG_BASE_R_SEED_B_2_4_             0x0000c09c
#define PMC_PCS_REG_BASE_R_SEED_B_3_4_             0x0000c0a0
#define PMC_PCS_REG_BASE_R_SEED_B_4_4_             0x0000c0a4
#define PMC_PCS_REG_BASE_R_TEST_CTRL               0x0000c0a8
#define PMC_PCS_REG_BASE_R_TEST_ERR_CNT            0x0000c0ac
#define PMC_PCS_REG_BER_CNT_MSB_                   0x0000c0b0
#define PMC_PCS_REG_BLOCK_ERR_CNT_MSB_             0x0000c0b4
#define PMC_PCS_REG_PMON_BLOCK_ERR_CNT_1_3_        0x0000c0b8
#define PMC_PCS_REG_PMON_BLOCK_ERR_CNT_2_3_        0x0000c0bc
#define PMC_PCS_REG_PMON_BLOCK_ERR_CNT_3_3_        0x0000c0c0
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_ 0x0000c0c8
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_ 0x0000c0cc
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_ 0x0000c0d0
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_ 0x0000c0d4
#define PMC_PCS_REG_LANE_BIP_ERR_CNT( N )          (0x0000c168 + (N) * 4)
#define PMC_PCS_REG_LANE_MAPPING( N )              (0x0000c1b8 + (N) * 4)

/*-----------------------------------.
 | Register 0x0000c000 CTRL_AND_STAT |
 +-----------------------------------+
 | bit  15  R/W  RESET               |
 | bit  14  R/W  LOOPBACK            |
 | bit  5:2 R    SPEED_SEL           |
 +----------------------------------*/
#define PMC_PCS_REG_CTRL_AND_STAT_UNUSED_MASK   0xffff1783
#define PCS_REG_CTRL_AND_STAT_BIT_RESET_MSK     0x00008000
#define PCS_REG_CTRL_AND_STAT_BIT_RESET_OFF     15
#define PCS_REG_CTRL_AND_STAT_BIT_LOOPBACK_MSK  0x00004000
#define PCS_REG_CTRL_AND_STAT_BIT_LOOPBACK_OFF  14
#define PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_MSK 0x0000003c
#define PCS_REG_CTRL_AND_STAT_BIT_SPEED_SEL_OFF 2

/*-------------------------------.
 | Register 0x0000c004 STAT_1_2_ |
 +-------------------------------+
 | bit  2 R  LINK_STATUS_L       |
 +------------------------------*/
#define PMC_PCS_REG_STAT_1_2_UNUSED_MASK       0xfffffffb
#define PCS_REG_STAT_1_2_BIT_LINK_STATUS_L_MSK 0x00000004
#define PCS_REG_STAT_1_2_BIT_LINK_STATUS_L_OFF 2

/*------------------------------------.
 | Register 0x0000c008 DEVICE_ID_1_2_ |
 +------------------------------------+
 | bit  15:0 R  DEVID_LSB             |
 +-----------------------------------*/
#define PMC_PCS_REG_DEVICE_ID_1_2_UNUSED_MASK   0xffff0000
#define PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_MSK 0x0000ffff
#define PCS_REG_DEVICE_ID_1_2_BIT_DEVID_LSB_OFF 0

/*------------------------------------.
 | Register 0x0000c00c DEVICE_ID_2_2_ |
 +------------------------------------+
 | bit  15:0 R  DEVID_MSB             |
 +-----------------------------------*/
#define PMC_PCS_REG_DEVICE_ID_2_2_UNUSED_MASK   0xffff0000
#define PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_MSK 0x0000ffff
#define PCS_REG_DEVICE_ID_2_2_BIT_DEVID_MSB_OFF 0

/*-------------------------------.
 | Register 0x0000c01c CTRL_2_2_ |
 +-------------------------------+
 | bit  2:0 R  PCS_SEL           |
 +------------------------------*/
#define PMC_PCS_REG_CTRL_2_2_UNUSED_MASK 0xfffffff8
#define PCS_REG_CTRL_2_2_BIT_PCS_SEL_MSK 0x00000007
#define PCS_REG_CTRL_2_2_BIT_PCS_SEL_OFF 0

/*-------------------------------.
 | Register 0x0000c020 STAT_2_2_ |
 +-------------------------------+
 | bit  5:0 R  PCS_CFG_MODE      |
 +------------------------------*/
#define PMC_PCS_REG_STAT_2_2_UNUSED_MASK      0xffff33c0
#define PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_MSK 0x0000003f
#define PCS_REG_STAT_2_2_BIT_PCS_CFG_MODE_OFF 0

/*-------------------------------------.
 | Register 0x0000c038 PACKAGE_ID_1_2_ |
 +-------------------------------------+
 | bit  15:0 R  PKGID_LSB              |
 +------------------------------------*/
#define PMC_PCS_REG_PACKAGE_ID_1_2_UNUSED_MASK   0xffff0000
#define PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_MSK 0x0000ffff
#define PCS_REG_PACKAGE_ID_1_2_BIT_PKGID_LSB_OFF 0

/*-------------------------------------.
 | Register 0x0000c03c PACKAGE_ID_2_2_ |
 +-------------------------------------+
 | bit  15:0 R  PKGID_MSB              |
 +------------------------------------*/
#define PMC_PCS_REG_PACKAGE_ID_2_2_UNUSED_MASK   0xffff0000
#define PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_MSK 0x0000ffff
#define PCS_REG_PACKAGE_ID_2_2_BIT_PKGID_MSB_OFF 0

/*--------------------------------------.
 | Register 0x0000c080 BASE_R_STAT_1_2_ |
 +--------------------------------------+
 | bit  12 R  LINK_STATUS               |
 | bit  1  R  HIGH_BER                  |
 | bit  0  R  BLOCK_LOCK                |
 +-------------------------------------*/
#define PMC_PCS_REG_BASE_R_STAT_1_2_UNUSED_MASK     0xffffeffc
#define PCS_REG_BASE_R_STAT_1_2_BIT_LINK_STATUS_MSK 0x00001000
#define PCS_REG_BASE_R_STAT_1_2_BIT_LINK_STATUS_OFF 12
#define PCS_REG_BASE_R_STAT_1_2_BIT_HIGH_BER_MSK    0x00000002
#define PCS_REG_BASE_R_STAT_1_2_BIT_HIGH_BER_OFF    1
#define PCS_REG_BASE_R_STAT_1_2_BIT_BLOCK_LOCK_MSK  0x00000001
#define PCS_REG_BASE_R_STAT_1_2_BIT_BLOCK_LOCK_OFF  0

/*--------------------------------------.
 | Register 0x0000c084 BASE_R_STAT_2_2_ |
 +--------------------------------------+
 | bit  15   R  BLOCK_LOCK_L            |
 | bit  14   R  HIGH_BER_L              |
 | bit  13:8 R  BER_CNT_LSB             |
 | bit  7:0  R  BLOCK_ERR_CNT_LSB       |
 +-------------------------------------*/
#define PMC_PCS_REG_BASE_R_STAT_2_2_UNUSED_MASK           0xffff0000
#define PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_LOCK_L_MSK      0x00008000
#define PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_LOCK_L_OFF      15
#define PCS_REG_BASE_R_STAT_2_2_BIT_HIGH_BER_L_MSK        0x00004000
#define PCS_REG_BASE_R_STAT_2_2_BIT_HIGH_BER_L_OFF        14
#define PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_MSK       0x00003f00
#define PCS_REG_BASE_R_STAT_2_2_BIT_BER_CNT_LSB_OFF       8
#define PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_MSK 0x000000ff
#define PCS_REG_BASE_R_STAT_2_2_BIT_BLOCK_ERR_CNT_LSB_OFF 0

/*----------------------------------------.
 | Register 0x0000c088 BASE_R_SEED_A_1_4_ |
 +----------------------------------------+
 | bit  15:0 R/W  TEST_SEED_A_15_0        |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_A_1_4_UNUSED_MASK          0xffff0000
#define PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_MSK 0x0000ffff
#define PCS_REG_BASE_R_SEED_A_1_4_BIT_TEST_SEED_A_15_0_OFF 0

/*----------------------------------------.
 | Register 0x0000c08c BASE_R_SEED_A_2_4_ |
 +----------------------------------------+
 | bit  15:0 R/W  TEST_SEED_A_31_16       |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_A_2_4_UNUSED_MASK           0xffff0000
#define PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_MSK 0x0000ffff
#define PCS_REG_BASE_R_SEED_A_2_4_BIT_TEST_SEED_A_31_16_OFF 0

/*----------------------------------------.
 | Register 0x0000c090 BASE_R_SEED_A_3_4_ |
 +----------------------------------------+
 | bit  15:0 R/W  TEST_SEED_A_47_32       |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_A_3_4_UNUSED_MASK           0xffff0000
#define PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_MSK 0x0000ffff
#define PCS_REG_BASE_R_SEED_A_3_4_BIT_TEST_SEED_A_47_32_OFF 0

/*----------------------------------------.
 | Register 0x0000c094 BASE_R_SEED_A_4_4_ |
 +----------------------------------------+
 | bit  9:0 R/W  TEST_SEED_A_57_48        |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_A_4_4_UNUSED_MASK           0xfffffc00
#define PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_MSK 0x000003ff
#define PCS_REG_BASE_R_SEED_A_4_4_BIT_TEST_SEED_A_57_48_OFF 0

/*----------------------------------------.
 | Register 0x0000c098 BASE_R_SEED_B_1_4_ |
 +----------------------------------------+
 | bit  15:0 R/W  TEST_SEED_B_15_0        |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_B_1_4_UNUSED_MASK          0xffff0000
#define PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_MSK 0x0000ffff
#define PCS_REG_BASE_R_SEED_B_1_4_BIT_TEST_SEED_B_15_0_OFF 0

/*----------------------------------------.
 | Register 0x0000c09c BASE_R_SEED_B_2_4_ |
 +----------------------------------------+
 | bit  15:0 R/W  TEST_SEED_B_31_16       |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_B_2_4_UNUSED_MASK           0xffff0000
#define PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_MSK 0x0000ffff
#define PCS_REG_BASE_R_SEED_B_2_4_BIT_TEST_SEED_B_31_16_OFF 0

/*----------------------------------------.
 | Register 0x0000c0a0 BASE_R_SEED_B_3_4_ |
 +----------------------------------------+
 | bit  15:0 R/W  TEST_SEED_B_47_32       |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_B_3_4_UNUSED_MASK           0xffff0000
#define PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_MSK 0x0000ffff
#define PCS_REG_BASE_R_SEED_B_3_4_BIT_TEST_SEED_B_47_32_OFF 0

/*----------------------------------------.
 | Register 0x0000c0a4 BASE_R_SEED_B_4_4_ |
 +----------------------------------------+
 | bit  9:0 R/W  TEST_SEED_B_57_48        |
 +---------------------------------------*/
#define PMC_PCS_REG_BASE_R_SEED_B_4_4_UNUSED_MASK           0xfffffc00
#define PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_MSK 0x000003ff
#define PCS_REG_BASE_R_SEED_B_4_4_BIT_TEST_SEED_B_57_48_OFF 0

/*--------------------------------------.
 | Register 0x0000c0a8 BASE_R_TEST_CTRL |
 +--------------------------------------+
 | bit  3 R/W  TX_TEST_EN               |
 | bit  2 R/W  RX_TEST_EN               |
 | bit  1 R/W  TEST_PATTERN             |
 | bit  0 R/W  DATA_PATTERN             |
 +-------------------------------------*/
#define PMC_PCS_REG_BASE_R_TEST_CTRL_UNUSED_MASK      0xfffffff0
#define PCS_REG_BASE_R_TEST_CTRL_BIT_TX_TEST_EN_MSK   0x00000008
#define PCS_REG_BASE_R_TEST_CTRL_BIT_TX_TEST_EN_OFF   3
#define PCS_REG_BASE_R_TEST_CTRL_BIT_RX_TEST_EN_MSK   0x00000004
#define PCS_REG_BASE_R_TEST_CTRL_BIT_RX_TEST_EN_OFF   2
#define PCS_REG_BASE_R_TEST_CTRL_BIT_TEST_PATTERN_MSK 0x00000002
#define PCS_REG_BASE_R_TEST_CTRL_BIT_TEST_PATTERN_OFF 1
#define PCS_REG_BASE_R_TEST_CTRL_BIT_DATA_PATTERN_MSK 0x00000001
#define PCS_REG_BASE_R_TEST_CTRL_BIT_DATA_PATTERN_OFF 0

/*-----------------------------------------.
 | Register 0x0000c0ac BASE_R_TEST_ERR_CNT |
 +-----------------------------------------+
 | bit  15:0 R  TEST_ERR_CNT               |
 +----------------------------------------*/
#define PMC_PCS_REG_BASE_R_TEST_ERR_CNT_UNUSED_MASK      0xffff0000
#define PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_MSK 0x0000ffff
#define PCS_REG_BASE_R_TEST_ERR_CNT_BIT_TEST_ERR_CNT_OFF 0

/*----------------------------------.
 | Register 0x0000c0b0 BER_CNT_MSB_ |
 +----------------------------------+
 | bit  15:0 R  BER_CNT_MSB         |
 +---------------------------------*/
#define PMC_PCS_REG_BER_CNT_MSB_UNUSED_MASK     0xffff0000
#define PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_MSK 0x0000ffff
#define PCS_REG_BER_CNT_MSB_BIT_BER_CNT_MSB_OFF 0

/*----------------------------------------.
 | Register 0x0000c0b4 BLOCK_ERR_CNT_MSB_ |
 +----------------------------------------+
 | bit  13:0 R  BLOCK_ERR_CNT_MSB         |
 +---------------------------------------*/
#define PMC_PCS_REG_BLOCK_ERR_CNT_MSB_UNUSED_MASK           0xffffc000
#define PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_MSK 0x00003fff
#define PCS_REG_BLOCK_ERR_CNT_MSB_BIT_BLOCK_ERR_CNT_MSB_OFF 0

/*---------------------------------------------.
 | Register 0x0000c0b8 PMON_BLOCK_ERR_CNT_1_3_ |
 +---------------------------------------------+
 | bit  15:0 R  PMON_BLOCK_ERR_CNT_LSB         |
 +--------------------------------------------*/
#define PMC_PCS_REG_PMON_BLOCK_ERR_CNT_1_3_UNUSED_MASK                0xffff0000
#define PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_MSK 0x0000ffff
#define PCS_REG_PMON_BLOCK_ERR_CNT_1_3_BIT_PMON_BLOCK_ERR_CNT_LSB_OFF 0

/*---------------------------------------------.
 | Register 0x0000c0bc PMON_BLOCK_ERR_CNT_2_3_ |
 +---------------------------------------------+
 | bit  15:0 R  PMON_BLOCK_ERR_CNT_MID         |
 +--------------------------------------------*/
#define PMC_PCS_REG_PMON_BLOCK_ERR_CNT_2_3_UNUSED_MASK                0xffff0000
#define PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_MSK 0x0000ffff
#define PCS_REG_PMON_BLOCK_ERR_CNT_2_3_BIT_PMON_BLOCK_ERR_CNT_MID_OFF 0

/*---------------------------------------------.
 | Register 0x0000c0c0 PMON_BLOCK_ERR_CNT_3_3_ |
 +---------------------------------------------+
 | bit  7:0 R  PMON_BLOCK_ERR_CNT_MSB          |
 +--------------------------------------------*/
#define PMC_PCS_REG_PMON_BLOCK_ERR_CNT_3_3_UNUSED_MASK                0xffffff00
#define PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_MSK 0x000000ff
#define PCS_REG_PMON_BLOCK_ERR_CNT_3_3_BIT_PMON_BLOCK_ERR_CNT_MSB_OFF 0

/*----------------------------------------------------.
 | Register 0x0000c0c8 MULTI_LANE_ALIGNMENT_STAT_1_4_ |
 +----------------------------------------------------+
 | bit  12  R  ALL_LANES_LOCK                         |
 | bit  7:0 R  LANE_7_0_BLOCK_LOCK                    |
 +---------------------------------------------------*/
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_UNUSED_MASK             0xffffef00
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_ALL_LANES_LOCK_MSK      0x00001000
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_ALL_LANES_LOCK_OFF      12
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_MSK 0x000000ff
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_1_4_BIT_LANE_7_0_BLOCK_LOCK_OFF 0

/*----------------------------------------------------.
 | Register 0x0000c0cc MULTI_LANE_ALIGNMENT_STAT_2_4_ |
 +----------------------------------------------------+
 | bit  11:0 R  LANE_19_12_BLOCK_LOCK                 |
 +---------------------------------------------------*/
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_UNUSED_MASK               0xfffff000
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_MSK 0x00000fff
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_2_4_BIT_LANE_19_12_BLOCK_LOCK_OFF 0

/*----------------------------------------------------.
 | Register 0x0000c0d0 MULTI_LANE_ALIGNMENT_STAT_3_4_ |
 +----------------------------------------------------+
 | bit  7:0 R  LANE_ALIGN_LOCK_7_0                    |
 +---------------------------------------------------*/
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_UNUSED_MASK             0xffffff00
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_MSK 0x000000ff
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_3_4_BIT_LANE_ALIGN_LOCK_7_0_OFF 0

/*----------------------------------------------------.
 | Register 0x0000c0d4 MULTI_LANE_ALIGNMENT_STAT_4_4_ |
 +----------------------------------------------------+
 | bit  11:0 R  LANE_ALIGN_LOCK_19_12                 |
 +---------------------------------------------------*/
#define PMC_PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_UNUSED_MASK               0xfffff000
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_MSK 0x00000fff
#define PCS_REG_MULTI_LANE_ALIGNMENT_STAT_4_4_BIT_LANE_ALIGN_LOCK_19_12_OFF 0

/* index definitions for PMC_PCS_REG_LANE_BIP_ERR_CNT */
#define PMC_PCS_REG_LANE_BIP_ERR_CNT_INDEX_N_MIN          0
#define PMC_PCS_REG_LANE_BIP_ERR_CNT_INDEX_N_MAX          19
#define PMC_PCS_REG_LANE_BIP_ERR_CNT_INDEX_N_SIZE         20
#define PMC_PCS_REG_LANE_BIP_ERR_CNT_INDEX_N_OFFSET       4

/*--------------------------------------------------.
 | Register (0x0000c168 + (N) * 4) LANE_BIP_ERR_CNT |
 +--------------------------------------------------+
 | bit  15:0 R  LANE_BIP_ERR_CNT                    |
 +-------------------------------------------------*/
#define PMC_PCS_REG_LANE_BIP_ERR_CNT_UNUSED_MASK          0xffff0000
#define PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_MSK 0x0000ffff
#define PCS_REG_LANE_BIP_ERR_CNT_BIT_LANE_BIP_ERR_CNT_OFF 0

/* index definitions for PMC_PCS_REG_LANE_MAPPING */
#define PMC_PCS_REG_LANE_MAPPING_INDEX_N_MIN    0
#define PMC_PCS_REG_LANE_MAPPING_INDEX_N_MAX    19
#define PMC_PCS_REG_LANE_MAPPING_INDEX_N_SIZE   20
#define PMC_PCS_REG_LANE_MAPPING_INDEX_N_OFFSET 4

/*----------------------------------------------.
 | Register (0x0000c1b8 + (N) * 4) LANE_MAPPING |
 +----------------------------------------------+
 | bit  4:0 R  LANE_MAP                         |
 +---------------------------------------------*/
#define PMC_PCS_REG_LANE_MAPPING_UNUSED_MASK    0xffffffe0
#define PCS_REG_LANE_MAPPING_BIT_LANE_MAP_MSK   0x0000001f
#define PCS_REG_LANE_MAPPING_BIT_LANE_MAP_OFF   0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_PCS_REGS_H */
