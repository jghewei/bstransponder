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
 *     tmac block
 *****************************************************************************/
#ifndef _TMAC_REGS_H
#define _TMAC_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TMAC_REG_CFG_1                       0x00002000
#define PMC_TMAC_REG_CFG_2                       0x00002004
#define PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2      0x00002008
#define PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG   0x0000200c
#define PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG 0x00002010
#define PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG    0x00002014
#define PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG        0x00002018
#define PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG        0x0000201c
#define PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG        0x00002020
#define PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG    0x00002024

/*-------------------------------.
 | Register 0x00002000 CFG_1     |
 +-------------------------------+
 | bit  15    R    TMAC_STATUS   |
 | bit  14    R/W  CRC_CK        |
 | bit  13:12 R/W  PAD_ADD_EN    |
 | bit  11    R/W  CRC_ADD_EN    |
 | bit  10    R/W  MIN_FRM_CK    |
 | bit  9     R/W  TRUNC_EN      |
 | bit  8     R/W  MAX_FRM_CK    |
 | bit  7     R/W  EBUF_PAUSE_EN |
 | bit  6     R/W  IBUF_HTHR_EN  |
 | bit  5     R/W  XON_EN        |
 | bit  4     R/W  RMAC_PAUSE_EN |
 | bit  3     R/W  EBUF_ERR_EN   |
 | bit  2     R/W  TRANSPARENT   |
 | bit  1     R/W  HW_STOP_EN    |
 | bit  0     R/W  TMAC_ENBL     |
 +------------------------------*/
#define PMC_TMAC_REG_CFG_1_UNUSED_MASK       0xffff0000
#define TMAC_REG_CFG_1_BIT_TMAC_STATUS_MSK   0x00008000
#define TMAC_REG_CFG_1_BIT_TMAC_STATUS_OFF   15
#define TMAC_REG_CFG_1_BIT_CRC_CK_MSK        0x00004000
#define TMAC_REG_CFG_1_BIT_CRC_CK_OFF        14
#define TMAC_REG_CFG_1_BIT_PAD_ADD_EN_MSK    0x00003000
#define TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF    12
#define TMAC_REG_CFG_1_BIT_CRC_ADD_EN_MSK    0x00000800
#define TMAC_REG_CFG_1_BIT_CRC_ADD_EN_OFF    11
#define TMAC_REG_CFG_1_BIT_MIN_FRM_CK_MSK    0x00000400
#define TMAC_REG_CFG_1_BIT_MIN_FRM_CK_OFF    10
#define TMAC_REG_CFG_1_BIT_TRUNC_EN_MSK      0x00000200
#define TMAC_REG_CFG_1_BIT_TRUNC_EN_OFF      9
#define TMAC_REG_CFG_1_BIT_MAX_FRM_CK_MSK    0x00000100
#define TMAC_REG_CFG_1_BIT_MAX_FRM_CK_OFF    8
#define TMAC_REG_CFG_1_BIT_EBUF_PAUSE_EN_MSK 0x00000080
#define TMAC_REG_CFG_1_BIT_EBUF_PAUSE_EN_OFF 7
#define TMAC_REG_CFG_1_BIT_IBUF_HTHR_EN_MSK  0x00000040
#define TMAC_REG_CFG_1_BIT_IBUF_HTHR_EN_OFF  6
#define TMAC_REG_CFG_1_BIT_XON_EN_MSK        0x00000020
#define TMAC_REG_CFG_1_BIT_XON_EN_OFF        5
#define TMAC_REG_CFG_1_BIT_RMAC_PAUSE_EN_MSK 0x00000010
#define TMAC_REG_CFG_1_BIT_RMAC_PAUSE_EN_OFF 4
#define TMAC_REG_CFG_1_BIT_EBUF_ERR_EN_MSK   0x00000008
#define TMAC_REG_CFG_1_BIT_EBUF_ERR_EN_OFF   3
#define TMAC_REG_CFG_1_BIT_TRANSPARENT_MSK   0x00000004
#define TMAC_REG_CFG_1_BIT_TRANSPARENT_OFF   2
#define TMAC_REG_CFG_1_BIT_HW_STOP_EN_MSK    0x00000002
#define TMAC_REG_CFG_1_BIT_HW_STOP_EN_OFF    1
#define TMAC_REG_CFG_1_BIT_TMAC_ENBL_MSK     0x00000001
#define TMAC_REG_CFG_1_BIT_TMAC_ENBL_OFF     0

/*---------------------------.
 | Register 0x00002004 CFG_2 |
 +---------------------------+
 | bit  7   R/W  LATE_COL_EN |
 | bit  4   R/W  SFD_EN      |
 | bit  3:0 R/W  PAMB_LEN    |
 +--------------------------*/
#define PMC_TMAC_REG_CFG_2_UNUSED_MASK     0xffff0000
#define TMAC_REG_CFG_2_BIT_LATE_COL_EN_MSK 0x00000080
#define TMAC_REG_CFG_2_BIT_LATE_COL_EN_OFF 7
#define TMAC_REG_CFG_2_BIT_SFD_EN_MSK      0x00000010
#define TMAC_REG_CFG_2_BIT_SFD_EN_OFF      4
#define TMAC_REG_CFG_2_BIT_PAMB_LEN_MSK    0x0000000f
#define TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF    0

/*--------------------------------------------.
 | Register 0x00002008 INTER_PACKET_GAP_CFG_2 |
 +--------------------------------------------+
 | bit  15:12 R/W  FIPG1_LEN                  |
 | bit  11:8  R/W  FIPG2_LEN                  |
 +-------------------------------------------*/
#define PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2_UNUSED_MASK   0xffff0000
#define TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_MSK 0x0000f000
#define TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF 12
#define TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_MSK 0x00000f00
#define TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF 8

/*-----------------------------------------------.
 | Register 0x0000200c STATION_ADR_HIGH_WORD_CFG |
 +-----------------------------------------------+
 | bit  15:0 R/W  STA                            |
 +----------------------------------------------*/
#define PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG_UNUSED_MASK 0xffff0000
#define TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK     0x0000ffff
#define TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF     0

/*-------------------------------------------------.
 | Register 0x00002010 STATION_ADR_MIDDLE_WORD_CFG |
 +-------------------------------------------------+
 | bit  15:0 R/W  STA                              |
 +------------------------------------------------*/
#define PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_UNUSED_MASK 0xffff0000
#define TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK     0x0000ffff
#define TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF     0

/*----------------------------------------------.
 | Register 0x00002014 STATION_ADR_LOW_WORD_CFG |
 +----------------------------------------------+
 | bit  15:0 R/W  STA                           |
 +---------------------------------------------*/
#define PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG_UNUSED_MASK 0xffff0000
#define TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK     0x0000ffff
#define TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF     0

/*------------------------------------------.
 | Register 0x00002018 MAX_FRAME_LENGTH_CFG |
 +------------------------------------------+
 | bit  15:0 R/W  MAX_FRM_LEN               |
 +-----------------------------------------*/
#define PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG_UNUSED_MASK     0xffff0000
#define TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK 0x0000ffff
#define TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF 0

/*------------------------------------------.
 | Register 0x0000201c MIN_FRAME_LENGTH_CFG |
 +------------------------------------------+
 | bit  6   R/W  MIN_FRM_TAG                |
 | bit  5:0 R/W  MIN_FRM_LEN                |
 +-----------------------------------------*/
#define PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG_UNUSED_MASK     0xffff0000
#define TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_TAG_MSK 0x00000040
#define TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_TAG_OFF 6
#define TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_MSK 0x0000003f
#define TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF 0

/*------------------------------------------.
 | Register 0x00002020 PAUSE_FRAME_TIME_CFG |
 +------------------------------------------+
 | bit  15:0 R/W  PAUSE_TIME                |
 +-----------------------------------------*/
#define PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG_UNUSED_MASK    0xffff0000
#define TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_MSK 0x0000ffff
#define TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF 0

/*----------------------------------------------.
 | Register 0x00002024 PAUSE_FRAME_INTERVAL_CFG |
 +----------------------------------------------+
 | bit  15:0 R/W  PAUSE_IVAL                    |
 +---------------------------------------------*/
#define PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_UNUSED_MASK    0xffff0000
#define TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_MSK 0x0000ffff
#define TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TMAC_REGS_H */
