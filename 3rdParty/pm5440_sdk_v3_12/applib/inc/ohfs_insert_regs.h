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
 *     ohfs_insert block
 *****************************************************************************/
#ifndef _OHFS_INSERT_REGS_H
#define _OHFS_INSERT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG( N )     (0x00000020 + (N) * 0x04)
#define PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG( N )     (0x00000200 + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG( N )        (0x00000204 + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG( N )       (0x00000208 + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_XOFF_WM_CFG( N )               (0x0000020c + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY( N )           (0x00000210 + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY( N )           (0x00000214 + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG( N ) (0x00000218 + (N) * 0x20)
#define PMC_OHFS_INSERT_REG_LO_CFG_REG( N )                (0x0000021c + (N) * 0x20)

/* index definitions for PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG */
#define PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_INDEX_N_MIN    0
#define PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_INDEX_N_MAX    95
#define PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_INDEX_N_SIZE   96
#define PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_INDEX_N_OFFSET 0x04

/*----------------------------------------------------------.
 | Register (0x00000020 + (N) * 0x04) CHANNEL_RP_NCS_CONFIG |
 +----------------------------------------------------------+
 | bit  1 R/W  NCS_EN                                       |
 | bit  0 R/W  RP                                           |
 +---------------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_UNUSED_MASK    0xfffffffc
#define OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_NCS_EN_MSK     0x00000002
#define OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_NCS_EN_OFF     1
#define OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_RP_MSK         0x00000001
#define OHFS_INSERT_REG_CHANNEL_RP_NCS_CONFIG_BIT_RP_OFF         0

/* index definitions for PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG */
#define PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_INDEX_N_MIN              0
#define PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_INDEX_N_MAX              95
#define PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_INDEX_N_SIZE             96
#define PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_INDEX_N_OFFSET           0x20

/*----------------------------------------------------------.
 | Register (0x00000200 + (N) * 0x20) CHANNEL_COMMON_CONFIG |
 +----------------------------------------------------------+
 | bit  26    R/W  DBG_UNDERRUN_INT_SRC                     |
 | bit  25    R/W  DBG_MASK_SOMF_SYNC                       |
 | bit  24    R/W  EXT_PAYLOAD_TYPE                         |
 | bit  23:16 R/W  PT                                       |
 | bit  15:10 R/W  PAD_CNT                                  |
 | bit  9:3   R/W  MAX_SLOT_NUM                             |
 | bit  2     R/W  OMFI_INSERT                              |
 | bit  0     R/W  NULL_CHAN_EN                             |
 +---------------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_UNUSED_MASK              0xf8000002
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_UNDERRUN_INT_SRC_MSK 0x04000000
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_UNDERRUN_INT_SRC_OFF 26
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_MASK_SOMF_SYNC_MSK   0x02000000
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_DBG_MASK_SOMF_SYNC_OFF   25
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_EXT_PAYLOAD_TYPE_MSK     0x01000000
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_EXT_PAYLOAD_TYPE_OFF     24
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_MSK                   0x00ff0000
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PT_OFF                   16
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_MSK              0x0000fc00
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_PAD_CNT_OFF              10
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_MSK         0x000003f8
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_MAX_SLOT_NUM_OFF         3
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_OMFI_INSERT_MSK          0x00000004
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_OMFI_INSERT_OFF          2
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_NULL_CHAN_EN_MSK         0x00000001
#define OHFS_INSERT_REG_CHANNEL_COMMON_CONFIG_BIT_NULL_CHAN_EN_OFF         0

/* index definitions for PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG */
#define PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG_INDEX_N_MIN         0
#define PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG_INDEX_N_MAX         95
#define PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG_INDEX_N_SIZE        96
#define PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG_INDEX_N_OFFSET      0x20

/*-------------------------------------------------------.
 | Register (0x00000204 + (N) * 0x20) FIXED_STUFF_CONFIG |
 +-------------------------------------------------------+
 | bit  26:15 R/W  STUFF_COL_POS_2                       |
 | bit  14:3  R/W  STUFF_COL_POS_1                       |
 | bit  2     R/W  STUFF_POS_SEL                         |
 | bit  1     R/W  STUFF_MODE                            |
 | bit  0     R/W  STUFF_EN                              |
 +------------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_FIXED_STUFF_CONFIG_UNUSED_MASK         0xf8000000
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_MSK 0x07ff8000
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_2_OFF 15
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_MSK 0x00007ff8
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_COL_POS_1_OFF 3
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_POS_SEL_MSK   0x00000004
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_POS_SEL_OFF   2
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_MODE_MSK      0x00000002
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_MODE_OFF      1
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_EN_MSK        0x00000001
#define OHFS_INSERT_REG_FIXED_STUFF_CONFIG_BIT_STUFF_EN_OFF        0

/* index definitions for PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG */
#define PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_INDEX_N_MIN    0
#define PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_INDEX_N_MAX    95
#define PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_INDEX_N_SIZE   96
#define PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_INDEX_N_OFFSET 0x20

/*--------------------------------------------------------.
 | Register (0x00000208 + (N) * 0x20) RESIZE_OVERHEAD_CFG |
 +--------------------------------------------------------+
 | bit  23:0 R/W  RCOH                                    |
 +-------------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_UNUSED_MASK    0xff000000
#define OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_MSK       0x00ffffff
#define OHFS_INSERT_REG_RESIZE_OVERHEAD_CFG_BIT_RCOH_OFF       0

/* index definitions for PMC_OHFS_INSERT_REG_XOFF_WM_CFG */
#define PMC_OHFS_INSERT_REG_XOFF_WM_CFG_INDEX_N_MIN       0
#define PMC_OHFS_INSERT_REG_XOFF_WM_CFG_INDEX_N_MAX       95
#define PMC_OHFS_INSERT_REG_XOFF_WM_CFG_INDEX_N_SIZE      96
#define PMC_OHFS_INSERT_REG_XOFF_WM_CFG_INDEX_N_OFFSET    0x20

/*------------------------------------------------.
 | Register (0x0000020c + (N) * 0x20) XOFF_WM_CFG |
 +------------------------------------------------+
 | bit  7:0 R/W  XOFF_WM_LEVEL                    |
 +-----------------------------------------------*/
#define PMC_OHFS_INSERT_REG_XOFF_WM_CFG_UNUSED_MASK       0xffffff00
#define OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_MSK 0x000000ff
#define OHFS_INSERT_REG_XOFF_WM_CFG_BIT_XOFF_WM_LEVEL_OFF 0

/* index definitions for PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY */
#define PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY_INDEX_N_MIN         0
#define PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY_INDEX_N_MAX         95
#define PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY_INDEX_N_SIZE        96
#define PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY_INDEX_N_OFFSET      0x20

/*----------------------------------------------------.
 | Register (0x00000210 + (N) * 0x20) JC_RD_IPT_ENTRY |
 +----------------------------------------------------+
 | bit  16   R/W  JC_RD_IPT_VALID                     |
 | bit  14:8 R/W  JC_RD_IPT_ID                        |
 | bit  6:0  R/W  JC_RD_IPT_VA                        |
 +---------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_JC_RD_IPT_ENTRY_UNUSED_MASK         0xfffe8080
#define OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VALID_MSK 0x00010000
#define OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VALID_OFF 16
#define OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_MSK    0x00007f00
#define OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_ID_OFF    8
#define OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_MSK    0x0000007f
#define OHFS_INSERT_REG_JC_RD_IPT_ENTRY_BIT_JC_RD_IPT_VA_OFF    0

/* index definitions for PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY */
#define PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY_INDEX_N_MIN         0
#define PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY_INDEX_N_MAX         95
#define PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY_INDEX_N_SIZE        96
#define PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY_INDEX_N_OFFSET      0x20

/*----------------------------------------------------.
 | Register (0x00000214 + (N) * 0x20) JC_WR_IPT_ENTRY |
 +----------------------------------------------------+
 | bit  16   R/W  JC_WR_IPT_VALID                     |
 | bit  14:8 R/W  JC_WR_IPT_ID                        |
 | bit  6:0  R/W  JC_WR_IPT_VA                        |
 +---------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_JC_WR_IPT_ENTRY_UNUSED_MASK         0xfffe8080
#define OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VALID_MSK 0x00010000
#define OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VALID_OFF 16
#define OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_MSK    0x00007f00
#define OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_ID_OFF    8
#define OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_MSK    0x0000007f
#define OHFS_INSERT_REG_JC_WR_IPT_ENTRY_BIT_JC_WR_IPT_VA_OFF    0

/* index definitions for PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG */
#define PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_INDEX_N_MIN    0
#define PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_INDEX_N_MAX    95
#define PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_INDEX_N_SIZE   96
#define PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_INDEX_N_OFFSET 0x20

/*--------------------------------------------------------------.
 | Register (0x00000218 + (N) * 0x20) CHANNEL_CSF_INSERT_CONFIG |
 +--------------------------------------------------------------+
 | bit  0 R/W  CSF_INSERT                                       |
 +-------------------------------------------------------------*/
#define PMC_OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_UNUSED_MASK    0xfffffffe
#define OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_BIT_CSF_INSERT_MSK 0x00000001
#define OHFS_INSERT_REG_CHANNEL_CSF_INSERT_CONFIG_BIT_CSF_INSERT_OFF 0

/* index definitions for PMC_OHFS_INSERT_REG_LO_CFG_REG */
#define PMC_OHFS_INSERT_REG_LO_CFG_REG_INDEX_N_MIN    0
#define PMC_OHFS_INSERT_REG_LO_CFG_REG_INDEX_N_MAX    95
#define PMC_OHFS_INSERT_REG_LO_CFG_REG_INDEX_N_SIZE   96
#define PMC_OHFS_INSERT_REG_LO_CFG_REG_INDEX_N_OFFSET 0x20

/*-----------------------------------------------.
 | Register (0x0000021c + (N) * 0x20) LO_CFG_REG |
 +-----------------------------------------------+
 | bit  23:16 R/W  MSI                           |
 | bit  12    R/W  TS_LAST                       |
 | bit  11    R/W  TS_VALID                      |
 | bit  10:8  R/W  MAP_TYPE                      |
 | bit  6:0   R/W  LO_CHID                       |
 +----------------------------------------------*/
#define PMC_OHFS_INSERT_REG_LO_CFG_REG_UNUSED_MASK    0xff00e080
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_MSK        0x00ff0000
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_MSI_OFF        16
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_LAST_MSK    0x00001000
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_LAST_OFF    12
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_VALID_MSK   0x00000800
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_TS_VALID_OFF   11
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_MSK   0x00000700
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_MAP_TYPE_OFF   8
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_MSK    0x0000007f
#define OHFS_INSERT_REG_LO_CFG_REG_BIT_LO_CHID_OFF    0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OHFS_INSERT_REGS_H */
