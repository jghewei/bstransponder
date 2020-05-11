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
 *     expand_ibuf block
 *****************************************************************************/
#ifndef _EXPAND_IBUF_REGS_H
#define _EXPAND_IBUF_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID       0x00000360
#define PMC_EXPAND_IBUF_REG_CFG_UPDATE( N )      (0x00000400 + (N) * 0x8)
#define PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG( N ) (0x00000404 + (N) * 0x8)
#define PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG( N )    (0x00000800 + (N) * 0x4)
#define PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS( N )  (0x00000c00 + (N) * 0x10)
#define PMC_EXPAND_IBUF_REG_HAO_CFG_REG( N )     (0x00000c04 + (N) * 0x10)
#define PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL( N )    (0x00000c08 + (N) * 0x10)
#define PMC_EXPAND_IBUF_REG_SOMF_CFG_REG( N )    (0x00001800 + (N) * 0x4)

/*------------------------------------.
 | Register 0x00000360 MEM_CFG_STA_ID |
 +------------------------------------+
 | bit  14:8 R/W  MEM_LO_ID           |
 | bit  6:0  R/W  MEM_HO_ID           |
 +-----------------------------------*/
#define PMC_EXPAND_IBUF_REG_MEM_CFG_STA_ID_UNUSED_MASK   0xffff8080
#define EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_MSK 0x00007f00
#define EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_LO_ID_OFF 8
#define EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_MSK 0x0000007f
#define EXPAND_IBUF_REG_MEM_CFG_STA_ID_BIT_MEM_HO_ID_OFF 0

/* index definitions for PMC_EXPAND_IBUF_REG_CFG_UPDATE */
#define PMC_EXPAND_IBUF_REG_CFG_UPDATE_INDEX_N_MIN       0
#define PMC_EXPAND_IBUF_REG_CFG_UPDATE_INDEX_N_MAX       95
#define PMC_EXPAND_IBUF_REG_CFG_UPDATE_INDEX_N_SIZE      96
#define PMC_EXPAND_IBUF_REG_CFG_UPDATE_INDEX_N_OFFSET    0x8

/*----------------------------------------------.
 | Register (0x00000400 + (N) * 0x8) CFG_UPDATE |
 +----------------------------------------------+
 | bit  16 R/W  NORM_CMD                        |
 | bit  8  R/W  CONFIG_MODE                     |
 | bit  0  R/W  CONFIG_UPDATE                   |
 +---------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_CFG_UPDATE_UNUSED_MASK       0xfffefefe
#define EXPAND_IBUF_REG_CFG_UPDATE_BIT_NORM_CMD_MSK      0x00010000
#define EXPAND_IBUF_REG_CFG_UPDATE_BIT_NORM_CMD_OFF      16
#define EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_MODE_MSK   0x00000100
#define EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_MODE_OFF   8
#define EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_MSK 0x00000001
#define EXPAND_IBUF_REG_CFG_UPDATE_BIT_CONFIG_UPDATE_OFF 0

/* index definitions for PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG */
#define PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG_INDEX_N_MIN          0
#define PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG_INDEX_N_MAX          95
#define PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG_INDEX_N_SIZE         96
#define PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG_INDEX_N_OFFSET       0x8

/*---------------------------------------------------.
 | Register (0x00000404 + (N) * 0x8) CHANNEL_CFG_REG |
 +---------------------------------------------------+
 | bit  31:17 R/W  HO_CAL_CYCLE_TOT                  |
 | bit  16    R/W  HO_LAST_CYCLE                     |
 | bit  15    R/W  HO_FIRST_CYCLE                    |
 | bit  14:8  R/W  HO_ID                             |
 | bit  7:0   R/W  MF_TOT                            |
 +--------------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_CHANNEL_CFG_REG_UNUSED_MASK          0x00000000
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_MSK 0xfffe0000
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_CAL_CYCLE_TOT_OFF 17
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_LAST_CYCLE_MSK    0x00010000
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_LAST_CYCLE_OFF    16
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_FIRST_CYCLE_MSK   0x00008000
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_FIRST_CYCLE_OFF   15
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_MSK            0x00007f00
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_HO_ID_OFF            8
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_MSK           0x000000ff
#define EXPAND_IBUF_REG_CHANNEL_CFG_REG_BIT_MF_TOT_OFF           0

/* index definitions for PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG */
#define PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG_INDEX_N_MIN         0
#define PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG_INDEX_N_MAX         95
#define PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG_INDEX_N_SIZE        96
#define PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG_INDEX_N_OFFSET      0x4

/*------------------------------------------------.
 | Register (0x00000800 + (N) * 0x4) LO_CYCLE_CFG |
 +------------------------------------------------+
 | bit  8 R/W  EOMF_DATA_CYCLE                    |
 | bit  0 R/W  LO_FIRST_CYCLE                     |
 +-----------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_LO_CYCLE_CFG_UNUSED_MASK         0xfffffefe
#define EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_EOMF_DATA_CYCLE_MSK 0x00000100
#define EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_EOMF_DATA_CYCLE_OFF 8
#define EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_LO_FIRST_CYCLE_MSK  0x00000001
#define EXPAND_IBUF_REG_LO_CYCLE_CFG_BIT_LO_FIRST_CYCLE_OFF  0

/* index definitions for PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS */
#define PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS_INDEX_N_MIN       0
#define PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS_INDEX_N_MAX       95
#define PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS_INDEX_N_SIZE      96
#define PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS_INDEX_N_OFFSET    0x10

/*---------------------------------------------------.
 | Register (0x00000c00 + (N) * 0x10) TXJC_MGEN_CFGS |
 +---------------------------------------------------+
 | bit  30:28 R/W  CHAN_MAP_TYPE                     |
 | bit  27:21 R/W  M                                 |
 | bit  20:0  R/W  SERVER_SIZE                       |
 +--------------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_TXJC_MGEN_CFGS_UNUSED_MASK       0x80000000
#define EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_MSK 0x70000000
#define EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_CHAN_MAP_TYPE_OFF 28
#define EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_MSK             0x0fe00000
#define EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_M_OFF             21
#define EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_MSK   0x001fffff
#define EXPAND_IBUF_REG_TXJC_MGEN_CFGS_BIT_SERVER_SIZE_OFF   0

/* index definitions for PMC_EXPAND_IBUF_REG_HAO_CFG_REG */
#define PMC_EXPAND_IBUF_REG_HAO_CFG_REG_INDEX_N_MIN             0
#define PMC_EXPAND_IBUF_REG_HAO_CFG_REG_INDEX_N_MAX             95
#define PMC_EXPAND_IBUF_REG_HAO_CFG_REG_INDEX_N_SIZE            96
#define PMC_EXPAND_IBUF_REG_HAO_CFG_REG_INDEX_N_OFFSET          0x10

/*------------------------------------------------.
 | Register (0x00000c04 + (N) * 0x10) HAO_CFG_REG |
 +------------------------------------------------+
 | bit  31:17 R/W  ODTU_CNT_TOT                   |
 | bit  16:10 R/W  EOMF_LASTCYCLE_DATA            |
 | bit  9:7   R/W  NUM_TRIG                       |
 | bit  6:0   R/W  FIRST_CYCLE                    |
 +-----------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_HAO_CFG_REG_UNUSED_MASK             0x00000000
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_MSK        0xfffe0000
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_ODTU_CNT_TOT_OFF        17
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_MSK 0x0001fc00
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_EOMF_LASTCYCLE_DATA_OFF 10
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_MSK            0x00000380
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_NUM_TRIG_OFF            7
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_MSK         0x0000007f
#define EXPAND_IBUF_REG_HAO_CFG_REG_BIT_FIRST_CYCLE_OFF         0

/* index definitions for PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL */
#define PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL_INDEX_N_MIN      0
#define PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL_INDEX_N_MAX      95
#define PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL_INDEX_N_SIZE     96
#define PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL_INDEX_N_OFFSET   0x10

/*-------------------------------------------------.
 | Register (0x00000c08 + (N) * 0x10) CFC_XOFF_LVL |
 +-------------------------------------------------+
 | bit  9:0 R/W  CFC_FILL_LVL                      |
 +------------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_CFC_XOFF_LVL_UNUSED_MASK      0xfffffc00
#define EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_MSK 0x000003ff
#define EXPAND_IBUF_REG_CFC_XOFF_LVL_BIT_CFC_FILL_LVL_OFF 0

/* index definitions for PMC_EXPAND_IBUF_REG_SOMF_CFG_REG */
#define PMC_EXPAND_IBUF_REG_SOMF_CFG_REG_INDEX_N_MIN              0
#define PMC_EXPAND_IBUF_REG_SOMF_CFG_REG_INDEX_N_MAX              95
#define PMC_EXPAND_IBUF_REG_SOMF_CFG_REG_INDEX_N_SIZE             96
#define PMC_EXPAND_IBUF_REG_SOMF_CFG_REG_INDEX_N_OFFSET           0x4

/*------------------------------------------------.
 | Register (0x00001800 + (N) * 0x4) SOMF_CFG_REG |
 +------------------------------------------------+
 | bit  22:16 R/W  SOMF_FIRSTCYCLE_DATA           |
 | bit  14:0  R/W  SOMF_CYCLE_CNT                 |
 +-----------------------------------------------*/
#define PMC_EXPAND_IBUF_REG_SOMF_CFG_REG_UNUSED_MASK              0xff808000
#define EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_MSK 0x007f0000
#define EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_FIRSTCYCLE_DATA_OFF 16
#define EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_MSK       0x00007fff
#define EXPAND_IBUF_REG_SOMF_CFG_REG_BIT_SOMF_CYCLE_CNT_OFF       0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXPAND_IBUF_REGS_H */
