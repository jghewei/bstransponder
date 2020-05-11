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
 *     rmac block
 *****************************************************************************/
#ifndef _RMAC_REGS_H
#define _RMAC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RMAC_REG_CFG_1                       0x00000100
#define PMC_RMAC_REG_CFG_2                       0x00000104
#define PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG        0x00000108
#define PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG   0x0000010c
#define PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG 0x00000110
#define PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG    0x00000114
#define PMC_RMAC_REG_LINK_AND_STATISTIC_CFG      0x00000118

/*-----------------------------.
 | Register 0x00000100 CFG_1   |
 +-----------------------------+
 | bit  15    R    RMAC_STATUS |
 | bit  14    R/W  PAMB_CK     |
 | bit  13    R/W  TAG_AWARE   |
 | bit  12    R/W  INRANGE_DIS |
 | bit  11:10 R/W  INRANGE_CK  |
 | bit  9     R/W  MAX_FRM_TRN |
 | bit  8     R/W  MAX_FRM_DIS |
 | bit  7     R/W  MAX_FRM_CK  |
 | bit  6     R/W  MIN_FRM_DIS |
 | bit  5     R/W  MIN_FRM_CK  |
 | bit  4     R/W  CRC_DIS     |
 | bit  3     R/W  CRC_CK      |
 | bit  2     R/W  TRANSPARENT |
 | bit  1     R/W  ETHER_MODE  |
 | bit  0     R/W  RMAC_ENBL   |
 +----------------------------*/
#define PMC_RMAC_REG_CFG_1_UNUSED_MASK     0xffff0000
#define RMAC_REG_CFG_1_BIT_RMAC_STATUS_MSK 0x00008000
#define RMAC_REG_CFG_1_BIT_RMAC_STATUS_OFF 15
#define RMAC_REG_CFG_1_BIT_PAMB_CK_MSK     0x00004000
#define RMAC_REG_CFG_1_BIT_PAMB_CK_OFF     14
#define RMAC_REG_CFG_1_BIT_TAG_AWARE_MSK   0x00002000
#define RMAC_REG_CFG_1_BIT_TAG_AWARE_OFF   13
#define RMAC_REG_CFG_1_BIT_INRANGE_DIS_MSK 0x00001000
#define RMAC_REG_CFG_1_BIT_INRANGE_DIS_OFF 12
#define RMAC_REG_CFG_1_BIT_INRANGE_CK_MSK  0x00000c00
#define RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF  10
#define RMAC_REG_CFG_1_BIT_MAX_FRM_TRN_MSK 0x00000200
#define RMAC_REG_CFG_1_BIT_MAX_FRM_TRN_OFF 9
#define RMAC_REG_CFG_1_BIT_MAX_FRM_DIS_MSK 0x00000100
#define RMAC_REG_CFG_1_BIT_MAX_FRM_DIS_OFF 8
#define RMAC_REG_CFG_1_BIT_MAX_FRM_CK_MSK  0x00000080
#define RMAC_REG_CFG_1_BIT_MAX_FRM_CK_OFF  7
#define RMAC_REG_CFG_1_BIT_MIN_FRM_DIS_MSK 0x00000040
#define RMAC_REG_CFG_1_BIT_MIN_FRM_DIS_OFF 6
#define RMAC_REG_CFG_1_BIT_MIN_FRM_CK_MSK  0x00000020
#define RMAC_REG_CFG_1_BIT_MIN_FRM_CK_OFF  5
#define RMAC_REG_CFG_1_BIT_CRC_DIS_MSK     0x00000010
#define RMAC_REG_CFG_1_BIT_CRC_DIS_OFF     4
#define RMAC_REG_CFG_1_BIT_CRC_CK_MSK      0x00000008
#define RMAC_REG_CFG_1_BIT_CRC_CK_OFF      3
#define RMAC_REG_CFG_1_BIT_TRANSPARENT_MSK 0x00000004
#define RMAC_REG_CFG_1_BIT_TRANSPARENT_OFF 2
#define RMAC_REG_CFG_1_BIT_ETHER_MODE_MSK  0x00000002
#define RMAC_REG_CFG_1_BIT_ETHER_MODE_OFF  1
#define RMAC_REG_CFG_1_BIT_RMAC_ENBL_MSK   0x00000001
#define RMAC_REG_CFG_1_BIT_RMAC_ENBL_OFF   0

/*--------------------------------.
 | Register 0x00000104 CFG_2      |
 +--------------------------------+
 | bit  15    R/W  INTERPKT_L1_EN |
 | bit  14:13 R/W  BLIND          |
 | bit  12:7  R/W  MIN_FRM_LEN    |
 | bit  6     R/W  PAD_STRIP_EN   |
 | bit  5     R/W  CRC_STRIP_EN   |
 | bit  4     R/W  CARRIER_EN     |
 | bit  3     R/W  INPAUSE_EN     |
 | bit  2     R/W  PAUSEFRM_DROP  |
 | bit  1     R/W  CTRLFRM_DROP   |
 | bit  0     R/W  DA_DROP        |
 +-------------------------------*/
#define PMC_RMAC_REG_CFG_2_UNUSED_MASK        0xffff0000
#define RMAC_REG_CFG_2_BIT_INTERPKT_L1_EN_MSK 0x00008000
#define RMAC_REG_CFG_2_BIT_INTERPKT_L1_EN_OFF 15
#define RMAC_REG_CFG_2_BIT_BLIND_MSK          0x00006000
#define RMAC_REG_CFG_2_BIT_BLIND_OFF          13
#define RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_MSK    0x00001f80
#define RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF    7
#define RMAC_REG_CFG_2_BIT_PAD_STRIP_EN_MSK   0x00000040
#define RMAC_REG_CFG_2_BIT_PAD_STRIP_EN_OFF   6
#define RMAC_REG_CFG_2_BIT_CRC_STRIP_EN_MSK   0x00000020
#define RMAC_REG_CFG_2_BIT_CRC_STRIP_EN_OFF   5
#define RMAC_REG_CFG_2_BIT_CARRIER_EN_MSK     0x00000010
#define RMAC_REG_CFG_2_BIT_CARRIER_EN_OFF     4
#define RMAC_REG_CFG_2_BIT_INPAUSE_EN_MSK     0x00000008
#define RMAC_REG_CFG_2_BIT_INPAUSE_EN_OFF     3
#define RMAC_REG_CFG_2_BIT_PAUSEFRM_DROP_MSK  0x00000004
#define RMAC_REG_CFG_2_BIT_PAUSEFRM_DROP_OFF  2
#define RMAC_REG_CFG_2_BIT_CTRLFRM_DROP_MSK   0x00000002
#define RMAC_REG_CFG_2_BIT_CTRLFRM_DROP_OFF   1
#define RMAC_REG_CFG_2_BIT_DA_DROP_MSK        0x00000001
#define RMAC_REG_CFG_2_BIT_DA_DROP_OFF        0

/*------------------------------------------.
 | Register 0x00000108 MAX_FRAME_LENGTH_CFG |
 +------------------------------------------+
 | bit  15:0 R/W  MAX_FRM_LEN               |
 +-----------------------------------------*/
#define PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG_UNUSED_MASK     0xffff0000
#define RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK 0x0000ffff
#define RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF 0

/*-----------------------------------------------.
 | Register 0x0000010c STATION_ADR_HIGH_WORD_CFG |
 +-----------------------------------------------+
 | bit  15:0 R/W  STA                            |
 +----------------------------------------------*/
#define PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG_UNUSED_MASK 0xffff0000
#define RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK     0x0000ffff
#define RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF     0

/*-------------------------------------------------.
 | Register 0x00000110 STATION_ADR_MIDDLE_WORD_CFG |
 +-------------------------------------------------+
 | bit  15:0 R/W  STA                              |
 +------------------------------------------------*/
#define PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_UNUSED_MASK 0xffff0000
#define RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK     0x0000ffff
#define RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF     0

/*----------------------------------------------.
 | Register 0x00000114 STATION_ADR_LOW_WORD_CFG |
 +----------------------------------------------+
 | bit  15:0 R/W  STA                           |
 +---------------------------------------------*/
#define PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG_UNUSED_MASK 0xffff0000
#define RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK     0x0000ffff
#define RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF     0

/*--------------------------------------------.
 | Register 0x00000118 LINK_AND_STATISTIC_CFG |
 +--------------------------------------------+
 | bit  15  R    GIGA_STATUS                  |
 | bit  9   R/W  CTRLFRM_STAT_EN              |
 | bit  8   R/W  PAUSEFRM_STAT_EN             |
 | bit  7:4 R/W  GPO                          |
 +-------------------------------------------*/
#define PMC_RMAC_REG_LINK_AND_STATISTIC_CFG_UNUSED_MASK          0xffff7c0c
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GIGA_STATUS_MSK      0x00008000
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GIGA_STATUS_OFF      15
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_CTRLFRM_STAT_EN_MSK  0x00000200
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_CTRLFRM_STAT_EN_OFF  9
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_PAUSEFRM_STAT_EN_MSK 0x00000100
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_PAUSEFRM_STAT_EN_OFF 8
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_MSK              0x000000f0
#define RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF              4

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RMAC_REGS_H */
