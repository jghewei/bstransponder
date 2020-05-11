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
 *     sts_sw block
 *****************************************************************************/
#ifndef _STS_SW_REGS_H
#define _STS_SW_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_STS_SW_REG_CONFIG                   0x00000000
#define PMC_STS_SW_REG_HO_CFG( N )              (0x00001000 + (N) * 0x20)
#define PMC_STS_SW_REG_HO_CYCLE_CFG( N )        (0x00001010 + (N) * 0x20)
#define PMC_STS_SW_REG_IPT( N )                 (0x00002000 + (N) * 0x4)
#define PMC_STS_SW_REG_CALENDAR_CHANNEL_ID( N ) (0x00002200 + (N) * 0x4)

/*----------------------------.
 | Register 0x00000000 CONFIG |
 +----------------------------+
 | bit  8 R/W  MXDX_MODE_EN   |
 +---------------------------*/
#define PMC_STS_SW_REG_CONFIG_UNUSED_MASK      0xfffffefe
#define STS_SW_REG_CONFIG_BIT_MXDX_MODE_EN_MSK 0x00000100
#define STS_SW_REG_CONFIG_BIT_MXDX_MODE_EN_OFF 8

/* index definitions for PMC_STS_SW_REG_HO_CFG */
#define PMC_STS_SW_REG_HO_CFG_INDEX_N_MIN    0
#define PMC_STS_SW_REG_HO_CFG_INDEX_N_MAX    95
#define PMC_STS_SW_REG_HO_CFG_INDEX_N_SIZE   96
#define PMC_STS_SW_REG_HO_CFG_INDEX_N_OFFSET 0x20

/*-------------------------------------------.
 | Register (0x00001000 + (N) * 0x20) HO_CFG |
 +-------------------------------------------+
 | bit  22:16 R/W  EOMF_CNT                  |
 | bit  14:8  R/W  LAST_CNT                  |
 | bit  6:0   R/W  HO_CNT_TOT                |
 +------------------------------------------*/
#define PMC_STS_SW_REG_HO_CFG_UNUSED_MASK    0xff808080
#define STS_SW_REG_HO_CFG_BIT_EOMF_CNT_MSK   0x007f0000
#define STS_SW_REG_HO_CFG_BIT_EOMF_CNT_OFF   16
#define STS_SW_REG_HO_CFG_BIT_LAST_CNT_MSK   0x00007f00
#define STS_SW_REG_HO_CFG_BIT_LAST_CNT_OFF   8
#define STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_MSK 0x0000007f
#define STS_SW_REG_HO_CFG_BIT_HO_CNT_TOT_OFF 0

/* index definitions for PMC_STS_SW_REG_HO_CYCLE_CFG */
#define PMC_STS_SW_REG_HO_CYCLE_CFG_INDEX_N_MIN     0
#define PMC_STS_SW_REG_HO_CYCLE_CFG_INDEX_N_MAX     95
#define PMC_STS_SW_REG_HO_CYCLE_CFG_INDEX_N_SIZE    96
#define PMC_STS_SW_REG_HO_CYCLE_CFG_INDEX_N_OFFSET  0x20

/*-------------------------------------------------.
 | Register (0x00001010 + (N) * 0x20) HO_CYCLE_CFG |
 +-------------------------------------------------+
 | bit  22:16 R/W  EOMF_CYCLE                      |
 | bit  14:8  R/W  LAST_CYCLE                      |
 | bit  6:0   R/W  FIRST_CYCLE                     |
 +------------------------------------------------*/
#define PMC_STS_SW_REG_HO_CYCLE_CFG_UNUSED_MASK     0xff808080
#define STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_MSK  0x007f0000
#define STS_SW_REG_HO_CYCLE_CFG_BIT_EOMF_CYCLE_OFF  16
#define STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_MSK  0x00007f00
#define STS_SW_REG_HO_CYCLE_CFG_BIT_LAST_CYCLE_OFF  8
#define STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_MSK 0x0000007f
#define STS_SW_REG_HO_CYCLE_CFG_BIT_FIRST_CYCLE_OFF 0

/* index definitions for PMC_STS_SW_REG_IPT */
#define PMC_STS_SW_REG_IPT_INDEX_N_MIN    0
#define PMC_STS_SW_REG_IPT_INDEX_N_MAX    95
#define PMC_STS_SW_REG_IPT_INDEX_N_SIZE   96
#define PMC_STS_SW_REG_IPT_INDEX_N_OFFSET 0x4

/*---------------------------------------.
 | Register (0x00002000 + (N) * 0x4) IPT |
 +---------------------------------------+
 | bit  16   R/W  IPT_VALID              |
 | bit  14:8 R/W  IPT_ID                 |
 | bit  6:0  R/W  IPT_VA                 |
 +--------------------------------------*/
#define PMC_STS_SW_REG_IPT_UNUSED_MASK    0xfffe8080
#define STS_SW_REG_IPT_BIT_IPT_VALID_MSK  0x00010000
#define STS_SW_REG_IPT_BIT_IPT_VALID_OFF  16
#define STS_SW_REG_IPT_BIT_IPT_ID_MSK     0x00007f00
#define STS_SW_REG_IPT_BIT_IPT_ID_OFF     8
#define STS_SW_REG_IPT_BIT_IPT_VA_MSK     0x0000007f
#define STS_SW_REG_IPT_BIT_IPT_VA_OFF     0

/* index definitions for PMC_STS_SW_REG_CALENDAR_CHANNEL_ID */
#define PMC_STS_SW_REG_CALENDAR_CHANNEL_ID_INDEX_N_MIN          0
#define PMC_STS_SW_REG_CALENDAR_CHANNEL_ID_INDEX_N_MAX          95
#define PMC_STS_SW_REG_CALENDAR_CHANNEL_ID_INDEX_N_SIZE         96
#define PMC_STS_SW_REG_CALENDAR_CHANNEL_ID_INDEX_N_OFFSET       0x4

/*-------------------------------------------------------.
 | Register (0x00002200 + (N) * 0x4) CALENDAR_CHANNEL_ID |
 +-------------------------------------------------------+
 | bit  24  R/W  MAP_DMSIM_TO_SSF                        |
 | bit  16  R/W  LO_EOMF_CYCLE                           |
 | bit  8   R/W  LO_FIRST_CYCLE                          |
 | bit  6:0 R/W  CHAN_ID                                 |
 +------------------------------------------------------*/
#define PMC_STS_SW_REG_CALENDAR_CHANNEL_ID_UNUSED_MASK          0xfefefe80
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_MAP_DMSIM_TO_SSF_MSK 0x01000000
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_MAP_DMSIM_TO_SSF_OFF 24
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_EOMF_CYCLE_MSK    0x00010000
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_EOMF_CYCLE_OFF    16
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_FIRST_CYCLE_MSK   0x00000100
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_LO_FIRST_CYCLE_OFF   8
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_MSK          0x0000007f
#define STS_SW_REG_CALENDAR_CHANNEL_ID_BIT_CHAN_ID_OFF          0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _STS_SW_REGS_H */
