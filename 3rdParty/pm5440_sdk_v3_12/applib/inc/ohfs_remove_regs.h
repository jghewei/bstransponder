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
 *     ohfs_remove block
 *****************************************************************************/
#ifndef _OHFS_REMOVE_REGS_H
#define _OHFS_REMOVE_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG              0x00000000
#define PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG             0x00000004
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG                     0x00000008
#define PMC_OHFS_REMOVE_REG_COMMON_CFG( N )                         (0x00000400 + (N) * 0x8)
#define PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION( N ) (0x00000404 + (N) * 0x8)
#define PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT( N )                     (0x00000800 + (N) * 0x10)
#define PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG( N )                (0x00000804 + (N) * 0x10)
#define PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG( N )                      (0x00000808 + (N) * 0x10)
#define PMC_OHFS_REMOVE_REG_PT_STAT_REG( N )                        (0x00001000 + (N) * 0x4)
#define PMC_OHFS_REMOVE_REG_MSI_STAT_REG( N )                       (0x00001200 + (N) * 0x4)
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_REG( N )                      (0x00001400 + (N) * 0x4)

/*------------------------------------------------.
 | Register 0x00000000 PT_STAT_UPDATE_DISABLE_CFG |
 +------------------------------------------------+
 | bit  0 R/W  PT_STAT_UPDATE_DISABLE             |
 +-----------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_UNUSED_MASK                0xfffffffe
#define OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_BIT_PT_STAT_UPDATE_DISABLE_MSK 0x00000001
#define OHFS_REMOVE_REG_PT_STAT_UPDATE_DISABLE_CFG_BIT_PT_STAT_UPDATE_DISABLE_OFF 0

/*-------------------------------------------------.
 | Register 0x00000004 MSI_STAT_UPDATE_DISABLE_CFG |
 +-------------------------------------------------+
 | bit  0 R/W  MSI_STAT_UPDATE_DISABLE             |
 +------------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_UNUSED_MASK                 0xfffffffe
#define OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_BIT_MSI_STAT_UPDATE_DISABLE_MSK 0x00000001
#define OHFS_REMOVE_REG_MSI_STAT_UPDATE_DISABLE_CFG_BIT_MSI_STAT_UPDATE_DISABLE_OFF 0

/*-----------------------------------------.
 | Register 0x00000008 RCOH_STAT_LATCH_CFG |
 +-----------------------------------------+
 | bit  0 R/W  TRNSFR_RCOH_TO_SHADOW_EN    |
 +----------------------------------------*/
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_UNUSED_MASK                  0xfffffffe
#define OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_BIT_TRNSFR_RCOH_TO_SHADOW_EN_MSK 0x00000001
#define OHFS_REMOVE_REG_RCOH_STAT_LATCH_CFG_BIT_TRNSFR_RCOH_TO_SHADOW_EN_OFF 0

/* index definitions for PMC_OHFS_REMOVE_REG_COMMON_CFG */
#define PMC_OHFS_REMOVE_REG_COMMON_CFG_INDEX_N_MIN            0
#define PMC_OHFS_REMOVE_REG_COMMON_CFG_INDEX_N_MAX            95
#define PMC_OHFS_REMOVE_REG_COMMON_CFG_INDEX_N_SIZE           96
#define PMC_OHFS_REMOVE_REG_COMMON_CFG_INDEX_N_OFFSET         0x8

/*----------------------------------------------.
 | Register (0x00000400 + (N) * 0x8) COMMON_CFG |
 +----------------------------------------------+
 | bit  31:24 R/W  PT                           |
 | bit  21:20 R/W  MSI_ACCEPTANCE_CNT           |
 | bit  17:16 R/W  PT_ACCEPTANCE_CNT            |
 | bit  14:12 R/W  TS_NUM_TYPE                  |
 | bit  10    R/W  MAP_DLOOMFI_TO_SSF           |
 | bit  9     R/W  HO_RCOH_EN                   |
 | bit  8     R/W  FLEX_RCOH_EN                 |
 | bit  7     R/W  MAP_DCSF_TO_SSF              |
 | bit  6     R/W  MAP_DPLM_TO_SSF              |
 | bit  5     R/W  MAP_SF_TO_SSF                |
 | bit  4     R/W  CSF_CHK_EN                   |
 | bit  3     R/W  MSI_CHK_EN                   |
 | bit  2     R/W  PT_CHK_EN                    |
 | bit  1     R/W  OPUKHDR_DATA                 |
 | bit  0     R/W  NULL_FRAME_MODE              |
 +---------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_COMMON_CFG_UNUSED_MASK            0x00cc8000
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_MSK                 0xff000000
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_OFF                 24
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_MSK 0x00300000
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_ACCEPTANCE_CNT_OFF 20
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_MSK  0x00030000
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_ACCEPTANCE_CNT_OFF  16
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_MSK        0x00007000
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_TS_NUM_TYPE_OFF        12
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DLOOMFI_TO_SSF_MSK 0x00000400
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DLOOMFI_TO_SSF_OFF 10
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_HO_RCOH_EN_MSK         0x00000200
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_HO_RCOH_EN_OFF         9
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_FLEX_RCOH_EN_MSK       0x00000100
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_FLEX_RCOH_EN_OFF       8
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DCSF_TO_SSF_MSK    0x00000080
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DCSF_TO_SSF_OFF    7
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DPLM_TO_SSF_MSK    0x00000040
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_DPLM_TO_SSF_OFF    6
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_SF_TO_SSF_MSK      0x00000020
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MAP_SF_TO_SSF_OFF      5
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_CSF_CHK_EN_MSK         0x00000010
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_CSF_CHK_EN_OFF         4
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_CHK_EN_MSK         0x00000008
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_MSI_CHK_EN_OFF         3
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_CHK_EN_MSK          0x00000004
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_PT_CHK_EN_OFF          2
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_OPUKHDR_DATA_MSK       0x00000002
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_OPUKHDR_DATA_OFF       1
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_NULL_FRAME_MODE_MSK    0x00000001
#define OHFS_REMOVE_REG_COMMON_CFG_BIT_NULL_FRAME_MODE_OFF    0

/* index definitions for PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION */
#define PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_INDEX_N_MIN    0
#define PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_INDEX_N_MAX    95
#define PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_INDEX_N_SIZE   96
#define PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_INDEX_N_OFFSET 0x8

/*----------------------------------------------------------------------.
 | Register (0x00000404 + (N) * 0x8) FIXED_STUFF_REMOVAL_CONFIGUTRATION |
 +----------------------------------------------------------------------+
 | bit  31:20 R/W  FS2_ADDR                                             |
 | bit  17    R/W  FS2_16_8                                             |
 | bit  16    R/W  FS2_EN                                               |
 | bit  15:4  R/W  FS1_ADDR                                             |
 | bit  1     R/W  FS1_16_8                                             |
 | bit  0     R/W  FS1_EN                                               |
 +---------------------------------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_UNUSED_MASK    0x000c000c
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_MSK   0xfff00000
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_ADDR_OFF   20
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_16_8_MSK   0x00020000
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_16_8_OFF   17
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_EN_MSK     0x00010000
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS2_EN_OFF     16
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_MSK   0x0000fff0
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_ADDR_OFF   4
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_16_8_MSK   0x00000002
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_16_8_OFF   1
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_EN_MSK     0x00000001
#define OHFS_REMOVE_REG_FIXED_STUFF_REMOVAL_CONFIGUTRATION_BIT_FS1_EN_OFF     0

/* index definitions for PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT */
#define PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT_INDEX_N_MIN        0
#define PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT_INDEX_N_MAX        95
#define PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT_INDEX_N_SIZE       96
#define PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT_INDEX_N_OFFSET     0x10

/*---------------------------------------------------.
 | Register (0x00000800 + (N) * 0x10) TS_ADDRESS_IPT |
 +---------------------------------------------------+
 | bit  22:16 R/W  TS_IPT_FRM_NUM                    |
 | bit  14:8  R/W  TS_IPT_ID                         |
 | bit  0     R/W  TS_IPT_VALID                      |
 +--------------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_TS_ADDRESS_IPT_UNUSED_MASK        0xff8080fe
#define OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_MSK 0x007f0000
#define OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_FRM_NUM_OFF 16
#define OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_MSK      0x00007f00
#define OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_ID_OFF      8
#define OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_VALID_MSK   0x00000001
#define OHFS_REMOVE_REG_TS_ADDRESS_IPT_BIT_TS_IPT_VALID_OFF   0

/* index definitions for PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG */
#define PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_INDEX_N_MIN       0
#define PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_INDEX_N_MAX       95
#define PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_INDEX_N_SIZE      96
#define PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_INDEX_N_OFFSET    0x10

/*--------------------------------------------------------.
 | Register (0x00000804 + (N) * 0x10) TRIB_SLOT_BASED_CFG |
 +--------------------------------------------------------+
 | bit  10:8 R/W  CHAN_MAP_TYPE                           |
 | bit  2    R/W  LAST_TS                                 |
 | bit  1    R/W  FIRST_TS                                |
 | bit  0    R/W  TS_VALID                                |
 +-------------------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_UNUSED_MASK       0xfffff8f8
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_MSK 0x00000700
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_CHAN_MAP_TYPE_OFF 8
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_LAST_TS_MSK       0x00000004
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_LAST_TS_OFF       2
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_FIRST_TS_MSK      0x00000002
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_FIRST_TS_OFF      1
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_TS_VALID_MSK      0x00000001
#define OHFS_REMOVE_REG_TRIB_SLOT_BASED_CFG_BIT_TS_VALID_OFF      0

/* index definitions for PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG */
#define PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG_INDEX_N_MIN    0
#define PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG_INDEX_N_MAX    95
#define PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG_INDEX_N_SIZE   96
#define PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG_INDEX_N_OFFSET 0x10

/*--------------------------------------------------.
 | Register (0x00000808 + (N) * 0x10) LO_ID_MSI_CFG |
 +--------------------------------------------------+
 | bit  14:8 R/W  LO_ID                             |
 | bit  7:0  R/W  MSI                               |
 +-------------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_LO_ID_MSI_CFG_UNUSED_MASK    0xffff8000
#define OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_MSK      0x00007f00
#define OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_LO_ID_OFF      8
#define OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_MSK        0x000000ff
#define OHFS_REMOVE_REG_LO_ID_MSI_CFG_BIT_MSI_OFF        0

/* index definitions for PMC_OHFS_REMOVE_REG_PT_STAT_REG */
#define PMC_OHFS_REMOVE_REG_PT_STAT_REG_INDEX_N_MIN    0
#define PMC_OHFS_REMOVE_REG_PT_STAT_REG_INDEX_N_MAX    95
#define PMC_OHFS_REMOVE_REG_PT_STAT_REG_INDEX_N_SIZE   96
#define PMC_OHFS_REMOVE_REG_PT_STAT_REG_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x00001000 + (N) * 0x4) PT_STAT_REG |
 +-----------------------------------------------+
 | bit  7:0 R  PT_STATUS                         |
 +----------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_PT_STAT_REG_UNUSED_MASK    0xffffff00
#define OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_MSK  0x000000ff
#define OHFS_REMOVE_REG_PT_STAT_REG_BIT_PT_STATUS_OFF  0

/* index definitions for PMC_OHFS_REMOVE_REG_MSI_STAT_REG */
#define PMC_OHFS_REMOVE_REG_MSI_STAT_REG_INDEX_N_MIN    0
#define PMC_OHFS_REMOVE_REG_MSI_STAT_REG_INDEX_N_MAX    95
#define PMC_OHFS_REMOVE_REG_MSI_STAT_REG_INDEX_N_SIZE   96
#define PMC_OHFS_REMOVE_REG_MSI_STAT_REG_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x00001200 + (N) * 0x4) MSI_STAT_REG |
 +------------------------------------------------+
 | bit  7:0 R  MSI_STATUS                         |
 +-----------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_MSI_STAT_REG_UNUSED_MASK    0xffffff00
#define OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_MSK 0x000000ff
#define OHFS_REMOVE_REG_MSI_STAT_REG_BIT_MSI_STATUS_OFF 0

/* index definitions for PMC_OHFS_REMOVE_REG_RCOH_STAT_REG */
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_REG_INDEX_N_MIN     0
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_REG_INDEX_N_MAX     95
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_REG_INDEX_N_SIZE    96
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_REG_INDEX_N_OFFSET  0x4

/*-------------------------------------------------.
 | Register (0x00001400 + (N) * 0x4) RCOH_STAT_REG |
 +-------------------------------------------------+
 | bit  15:0 R  RCOH_STATUS                        |
 +------------------------------------------------*/
#define PMC_OHFS_REMOVE_REG_RCOH_STAT_REG_UNUSED_MASK     0xffff0000
#define OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_MSK 0x0000ffff
#define OHFS_REMOVE_REG_RCOH_STAT_REG_BIT_RCOH_STATUS_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OHFS_REMOVE_REGS_H */
