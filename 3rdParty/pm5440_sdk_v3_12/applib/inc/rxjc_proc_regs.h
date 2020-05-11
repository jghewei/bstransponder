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
 *     rxjc_proc block
 *****************************************************************************/
#ifndef _RXJC_PROC_REGS_H
#define _RXJC_PROC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE 0x00000000
#define PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID     0x00000004
#define PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CNT      0x00000008
#define PMC_RXJC_PROC_REG_COMMON_CFG_1( N )      (0x00000800 + (N) * 0x10)
#define PMC_RXJC_PROC_REG_COMMON_CFG_2( N )      (0x00000804 + (N) * 0x10)
#define PMC_RXJC_PROC_REG_COMMON_CFG_3( N )      (0x00000808 + (N) * 0x10)
#define PMC_RXJC_PROC_REG_CM_STAT_REG( N )       (0x00000e00 + (N) * 0x4)

/*--------------------------------------------.
 | Register 0x00000000 CM_STAT_UPDATE_DISABLE |
 +--------------------------------------------+
 | bit  0 R/W  CM_ST_UPDATE_DISBLE            |
 +-------------------------------------------*/
#define PMC_RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_UNUSED_MASK             0xfffffffe
#define RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_BIT_CM_ST_UPDATE_DISBLE_MSK 0x00000001
#define RXJC_PROC_REG_CM_STAT_UPDATE_DISABLE_BIT_CM_ST_UPDATE_DISBLE_OFF 0

/*----------------------------------------.
 | Register 0x00000004 PMON_SYNCLOSS_CHID |
 +----------------------------------------+
 | bit  6:0 R/W  PMON_SYNCLOSS_CHID       |
 +---------------------------------------*/
#define PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CHID_UNUSED_MASK            0xffffff80
#define RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_MSK 0x0000007f
#define RXJC_PROC_REG_PMON_SYNCLOSS_CHID_BIT_PMON_SYNCLOSS_CHID_OFF 0

/*---------------------------------------.
 | Register 0x00000008 PMON_SYNCLOSS_CNT |
 +---------------------------------------+
 | bit  31:0 R  PMON_SYNCLOSS_CNT        |
 +--------------------------------------*/
#define PMC_RXJC_PROC_REG_PMON_SYNCLOSS_CNT_UNUSED_MASK           0x00000000
#define RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_MSK 0xffffffff
#define RXJC_PROC_REG_PMON_SYNCLOSS_CNT_BIT_PMON_SYNCLOSS_CNT_OFF 0

/* index definitions for PMC_RXJC_PROC_REG_COMMON_CFG_1 */
#define PMC_RXJC_PROC_REG_COMMON_CFG_1_INDEX_N_MIN    0
#define PMC_RXJC_PROC_REG_COMMON_CFG_1_INDEX_N_MAX    95
#define PMC_RXJC_PROC_REG_COMMON_CFG_1_INDEX_N_SIZE   96
#define PMC_RXJC_PROC_REG_COMMON_CFG_1_INDEX_N_OFFSET 0x10

/*-------------------------------------------------.
 | Register (0x00000800 + (N) * 0x10) COMMON_CFG_1 |
 +-------------------------------------------------+
 | bit  29:16 R/W  CM_MAX                          |
 | bit  13:0  R/W  CM_MIN                          |
 +------------------------------------------------*/
#define PMC_RXJC_PROC_REG_COMMON_CFG_1_UNUSED_MASK    0xc000c000
#define RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_MSK     0x3fff0000
#define RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MAX_OFF     16
#define RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_MSK     0x00003fff
#define RXJC_PROC_REG_COMMON_CFG_1_BIT_CM_MIN_OFF     0

/* index definitions for PMC_RXJC_PROC_REG_COMMON_CFG_2 */
#define PMC_RXJC_PROC_REG_COMMON_CFG_2_INDEX_N_MIN    0
#define PMC_RXJC_PROC_REG_COMMON_CFG_2_INDEX_N_MAX    95
#define PMC_RXJC_PROC_REG_COMMON_CFG_2_INDEX_N_SIZE   96
#define PMC_RXJC_PROC_REG_COMMON_CFG_2_INDEX_N_OFFSET 0x10

/*-------------------------------------------------.
 | Register (0x00000804 + (N) * 0x10) COMMON_CFG_2 |
 +-------------------------------------------------+
 | bit  22:16 R/W  M                               |
 | bit  9:0   R/W  CND_MAX                         |
 +------------------------------------------------*/
#define PMC_RXJC_PROC_REG_COMMON_CFG_2_UNUSED_MASK    0x8080fc00
#define RXJC_PROC_REG_COMMON_CFG_2_BIT_M_MSK          0x007f0000
#define RXJC_PROC_REG_COMMON_CFG_2_BIT_M_OFF          16
#define RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_MSK    0x000003ff
#define RXJC_PROC_REG_COMMON_CFG_2_BIT_CND_MAX_OFF    0

/* index definitions for PMC_RXJC_PROC_REG_COMMON_CFG_3 */
#define PMC_RXJC_PROC_REG_COMMON_CFG_3_INDEX_N_MIN      0
#define PMC_RXJC_PROC_REG_COMMON_CFG_3_INDEX_N_MAX      95
#define PMC_RXJC_PROC_REG_COMMON_CFG_3_INDEX_N_SIZE     96
#define PMC_RXJC_PROC_REG_COMMON_CFG_3_INDEX_N_OFFSET   0x10

/*-------------------------------------------------.
 | Register (0x00000808 + (N) * 0x10) COMMON_CFG_3 |
 +-------------------------------------------------+
 | bit  25:24 R/W  NJO_PJO_MODE                    |
 | bit  20:0  R/W  SERVER_SIZE                     |
 +------------------------------------------------*/
#define PMC_RXJC_PROC_REG_COMMON_CFG_3_UNUSED_MASK      0xfce00000
#define RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_MSK 0x03000000
#define RXJC_PROC_REG_COMMON_CFG_3_BIT_NJO_PJO_MODE_OFF 24
#define RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_MSK  0x001fffff
#define RXJC_PROC_REG_COMMON_CFG_3_BIT_SERVER_SIZE_OFF  0

/* index definitions for PMC_RXJC_PROC_REG_CM_STAT_REG */
#define PMC_RXJC_PROC_REG_CM_STAT_REG_INDEX_N_MIN    0
#define PMC_RXJC_PROC_REG_CM_STAT_REG_INDEX_N_MAX    95
#define PMC_RXJC_PROC_REG_CM_STAT_REG_INDEX_N_SIZE   96
#define PMC_RXJC_PROC_REG_CM_STAT_REG_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x00000e00 + (N) * 0x4) CM_STAT_REG |
 +-----------------------------------------------+
 | bit  13:0 R  CM_STATUS                        |
 +----------------------------------------------*/
#define PMC_RXJC_PROC_REG_CM_STAT_REG_UNUSED_MASK    0xffffc000
#define RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_MSK  0x00003fff
#define RXJC_PROC_REG_CM_STAT_REG_BIT_CM_STATUS_OFF  0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RXJC_PROC_REGS_H */
