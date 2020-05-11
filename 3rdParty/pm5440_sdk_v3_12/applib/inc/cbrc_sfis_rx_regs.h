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
 *     cbrc_sfis_rx block
 *****************************************************************************/
#ifndef _CBRC_SFIS_RX_REGS_H
#define _CBRC_SFIS_RX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SFIS_RX_REG_RX_CFG                 0x00000100
#define PMC_SFIS_RX_REG_RX_INT_SUMMARY         0x00000104
#define PMC_SFIS_RX_REG_RX_INT_0               0x00000108
#define PMC_SFIS_RX_REG_RX_INT_1               0x0000010c
#define PMC_SFIS_RX_REG_RX_INT_2               0x00000110
#define PMC_SFIS_RX_REG_RX_INT_EN_0            0x00000118
#define PMC_SFIS_RX_REG_RX_INT_EN_1            0x0000011c
#define PMC_SFIS_RX_REG_RX_INT_EN_2            0x00000120
#define PMC_SFIS_RX_REG_RX_INT_VAL_0           0x00000128
#define PMC_SFIS_RX_REG_RX_BUFFER_CFG          0x0000012c
#define PMC_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG 0x00000130
#define PMC_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG  0x00000134
#define PMC_SFIS_RX_REG_RX_ALARM_INTEGRATION   0x0000013c

/*----------------------------.
 | Register 0x00000100 RX_CFG |
 +----------------------------+
 | bit  5   R/W  RX_ODD_INV   |
 | bit  4   R/W  RX_SW_RST    |
 | bit  3:0 R/W  RX_NUM_LANES |
 +---------------------------*/
#define PMC_SFIS_RX_REG_RX_CFG_UNUSED_MASK      0xffffff80
#define SFIS_RX_REG_RX_CFG_BIT_RX_ODD_INV_MSK   0x00000020
#define SFIS_RX_REG_RX_CFG_BIT_RX_ODD_INV_OFF   5
#define SFIS_RX_REG_RX_CFG_BIT_RX_SW_RST_MSK    0x00000010
#define SFIS_RX_REG_RX_CFG_BIT_RX_SW_RST_OFF    4
#define SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_MSK 0x0000000f
#define SFIS_RX_REG_RX_CFG_BIT_RX_NUM_LANES_OFF 0

/*------------------------------------.
 | Register 0x00000104 RX_INT_SUMMARY |
 +------------------------------------+
 | bit  3:0 R  RX_I                   |
 +-----------------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_SUMMARY_UNUSED_MASK 0xfffffff0
#define SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_MSK    0x0000000f
#define SFIS_RX_REG_RX_INT_SUMMARY_BIT_RX_I_OFF    0

/*------------------------------.
 | Register 0x00000108 RX_INT_0 |
 +------------------------------+
 | bit  25:16 R/W  RXOOA_I      |
 | bit  11    R/W  RXOOF_I      |
 | bit  10    R/W  RXLOS_DSC_I  |
 | bit  9:0   R/W  RXLOS_I      |
 +-----------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_0_UNUSED_MASK     0xfc00f000
#define SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_MSK     0x03ff0000
#define SFIS_RX_REG_RX_INT_0_BIT_RXOOA_I_OFF     16
#define SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_MSK     0x00000800
#define SFIS_RX_REG_RX_INT_0_BIT_RXOOF_I_OFF     11
#define SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_0_BIT_RXLOS_DSC_I_OFF 10
#define SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_0_BIT_RXLOS_I_OFF     0

/*------------------------------.
 | Register 0x0000010c RX_INT_1 |
 +------------------------------+
 | bit  26  R/W  RXOVR_DSC_I    |
 | bit  25  R/W  RXOVR9_I       |
 | bit  24  R/W  RXOVR8_I       |
 | bit  23  R/W  RXOVR7_I       |
 | bit  22  R/W  RXOVR6_I       |
 | bit  21  R/W  RXOVR5_I       |
 | bit  20  R/W  RXOVR4_I       |
 | bit  19  R/W  RXOVR3_I       |
 | bit  18  R/W  RXOVR2_I       |
 | bit  17  R/W  RXOVR1_I       |
 | bit  16  R/W  RXOVR0_I       |
 | bit  10  R/W  RXUDR_DSC_I    |
 | bit  9:0 R/W  RXUDR_I        |
 +-----------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_1_UNUSED_MASK     0xf800f800
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_MSK 0x04000000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR_DSC_I_OFF 26
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_MSK    0x02000000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR9_I_OFF    25
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_MSK    0x01000000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR8_I_OFF    24
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_MSK    0x00800000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR7_I_OFF    23
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_MSK    0x00400000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR6_I_OFF    22
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_MSK    0x00200000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR5_I_OFF    21
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_MSK    0x00100000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR4_I_OFF    20
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_MSK    0x00080000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR3_I_OFF    19
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_MSK    0x00040000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR2_I_OFF    18
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_MSK    0x00020000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR1_I_OFF    17
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_MSK    0x00010000
#define SFIS_RX_REG_RX_INT_1_BIT_RXOVR0_I_OFF    16
#define SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_1_BIT_RXUDR_DSC_I_OFF 10
#define SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_1_BIT_RXUDR_I_OFF     0

/*-------------------------------.
 | Register 0x00000110 RX_INT_2  |
 +-------------------------------+
 | bit  10  R/W  RX_BITERR_DSC_I |
 | bit  9:0 R/W  RX_BITERR_I     |
 +------------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_2_UNUSED_MASK         0xfffff800
#define SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_DSC_I_OFF 10
#define SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_2_BIT_RX_BITERR_I_OFF     0

/*---------------------------------.
 | Register 0x00000118 RX_INT_EN_0 |
 +---------------------------------+
 | bit  25:16 R/W  RXOOA_E         |
 | bit  11    R/W  RXOOF_E         |
 | bit  10    R/W  RXLOS_DSC_E     |
 | bit  9:0   R/W  RXLOS_E         |
 +--------------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_EN_0_UNUSED_MASK     0xfc00f000
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_MSK     0x03ff0000
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOA_E_OFF     16
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOF_E_MSK     0x00000800
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXOOF_E_OFF     11
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_DSC_E_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_DSC_E_OFF 10
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_EN_0_BIT_RXLOS_E_OFF     0

/*---------------------------------.
 | Register 0x0000011c RX_INT_EN_1 |
 +---------------------------------+
 | bit  26  R/W  RXOVR_DSC_E       |
 | bit  25  R/W  RXOVR9_E          |
 | bit  24  R/W  RXOVR8_E          |
 | bit  23  R/W  RXOVR7_E          |
 | bit  22  R/W  RXOVR6_E          |
 | bit  21  R/W  RXOVR5_E          |
 | bit  20  R/W  RXOVR4_E          |
 | bit  19  R/W  RXOVR3_E          |
 | bit  18  R/W  RXOVR2_E          |
 | bit  17  R/W  RXOVR1_E          |
 | bit  16  R/W  RXOVR0_E          |
 | bit  10  R/W  RXUDR_DSC_E       |
 | bit  9:0 R/W  RXUDR_E           |
 +--------------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_EN_1_UNUSED_MASK     0xf800f800
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR_DSC_E_MSK 0x04000000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR_DSC_E_OFF 26
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR9_E_MSK    0x02000000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR9_E_OFF    25
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR8_E_MSK    0x01000000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR8_E_OFF    24
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR7_E_MSK    0x00800000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR7_E_OFF    23
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR6_E_MSK    0x00400000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR6_E_OFF    22
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR5_E_MSK    0x00200000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR5_E_OFF    21
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR4_E_MSK    0x00100000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR4_E_OFF    20
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR3_E_MSK    0x00080000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR3_E_OFF    19
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR2_E_MSK    0x00040000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR2_E_OFF    18
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR1_E_MSK    0x00020000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR1_E_OFF    17
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR0_E_MSK    0x00010000
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXOVR0_E_OFF    16
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_DSC_E_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_DSC_E_OFF 10
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_EN_1_BIT_RXUDR_E_OFF     0

/*---------------------------------.
 | Register 0x00000120 RX_INT_EN_2 |
 +---------------------------------+
 | bit  10  R/W  RX_BITERR_DSC_E   |
 | bit  9:0 R/W  RX_BITERR_E       |
 +--------------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_EN_2_UNUSED_MASK         0xfffff800
#define SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_DSC_E_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_DSC_E_OFF 10
#define SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_EN_2_BIT_RX_BITERR_E_OFF     0

/*----------------------------------.
 | Register 0x00000128 RX_INT_VAL_0 |
 +----------------------------------+
 | bit  25:16 R  RXOOA_V            |
 | bit  11    R  RXOOF_V            |
 | bit  10    R  RXLOS_DSC_V        |
 | bit  9:0   R  RXLOS_V            |
 +---------------------------------*/
#define PMC_SFIS_RX_REG_RX_INT_VAL_0_UNUSED_MASK     0xfc00f000
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_MSK     0x03ff0000
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOA_V_OFF     16
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOF_V_MSK     0x00000800
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXOOF_V_OFF     11
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_DSC_V_MSK 0x00000400
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_DSC_V_OFF 10
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_MSK     0x000003ff
#define SFIS_RX_REG_RX_INT_VAL_0_BIT_RXLOS_V_OFF     0

/*-----------------------------------.
 | Register 0x0000012c RX_BUFFER_CFG |
 +-----------------------------------+
 | bit  7 R/W  RX_FIFO_CENTER        |
 | bit  6 R/W  RX_FIFO_AUTO_CENTER   |
 +----------------------------------*/
#define PMC_SFIS_RX_REG_RX_BUFFER_CFG_UNUSED_MASK             0xffc08000
#define SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_CENTER_MSK      0x00000080
#define SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_CENTER_OFF      7
#define SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_AUTO_CENTER_MSK 0x00000040
#define SFIS_RX_REG_RX_BUFFER_CFG_BIT_RX_FIFO_AUTO_CENTER_OFF 6

/*--------------------------------------------.
 | Register 0x00000130 RX_FRAMING_CTRLLER_CFG |
 +--------------------------------------------+
 | bit  15:8 R/W  RX_MATCH_TIMEOUT_THRESH     |
 | bit  7:4  R/W  RX_MATCH_THRESH             |
 | bit  3:0  R/W  RX_BAD_THRESH               |
 +-------------------------------------------*/
#define PMC_SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_UNUSED_MASK                 0xffff0000
#define SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_MSK 0x0000ff00
#define SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_TIMEOUT_THRESH_OFF 8
#define SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_MSK         0x000000f0
#define SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_MATCH_THRESH_OFF         4
#define SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_MSK           0x0000000f
#define SFIS_RX_REG_RX_FRAMING_CTRLLER_CFG_BIT_RX_BAD_THRESH_OFF           0

/*----------------------------------------------.
 | Register 0x00000134 RX_DESKEW_CTRLLER_CFG    |
 +----------------------------------------------+
 | bit  15:8 R/W  RX_ALIGN_MATCH_TIMEOUT_THRESH |
 | bit  7:4  R/W  RX_ALIGN_MATCH_THRESH         |
 | bit  3:0  R/W  RX_ALIGN_BAD_THRESH           |
 +---------------------------------------------*/
#define PMC_SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_UNUSED_MASK                       0xffff0000
#define SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_MSK 0x0000ff00
#define SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_TIMEOUT_THRESH_OFF 8
#define SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_MSK         0x000000f0
#define SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_MATCH_THRESH_OFF         4
#define SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_MSK           0x0000000f
#define SFIS_RX_REG_RX_DESKEW_CTRLLER_CFG_BIT_RX_ALIGN_BAD_THRESH_OFF           0

/*------------------------------------------.
 | Register 0x0000013c RX_ALARM_INTEGRATION |
 +------------------------------------------+
 | bit  3 R/W  RXOOA_ALARM_EN               |
 | bit  2 R/W  RXOOF_ALARM_EN               |
 | bit  1 R/W  RXLOS_ALARM_EN               |
 | bit  0 R/W  RX_FORCE_LOS                 |
 +-----------------------------------------*/
#define PMC_SFIS_RX_REG_RX_ALARM_INTEGRATION_UNUSED_MASK        0xfffffff0
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOA_ALARM_EN_MSK 0x00000008
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOA_ALARM_EN_OFF 3
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOF_ALARM_EN_MSK 0x00000004
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXOOF_ALARM_EN_OFF 2
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXLOS_ALARM_EN_MSK 0x00000002
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RXLOS_ALARM_EN_OFF 1
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RX_FORCE_LOS_MSK   0x00000001
#define SFIS_RX_REG_RX_ALARM_INTEGRATION_BIT_RX_FORCE_LOS_OFF   0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBRC_SFIS_RX_REGS_H */
