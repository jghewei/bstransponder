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
 *     fc_tttm block
 *****************************************************************************/
#ifndef _FC_TTTM_REGS_H
#define _FC_TTTM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC_TTTM_REG_CFG                                    0x0002f100
#define PMC_FC_TTTM_REG_INT                                    0x0002f104
#define PMC_FC_TTTM_REG_INT_VAL                                0x0002f108
#define PMC_FC_TTTM_REG_INT_EN                                 0x0002f10c
#define PMC_FC_TTTM_REG_GFP_CORE_HEADER                        0x0002f110
#define PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER                     0x0002f114
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4                 0x0002f118
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8                 0x0002f11c
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12                0x0002f120
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16               0x0002f124
#define PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB            0x0002f128
#define PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB            0x0002f12c
#define PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB                     0x0002f130
#define PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB                     0x0002f134
#define PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR   0x0002f13c
#define PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR 0x0002f140
#define PMC_FC_TTTM_REG_GFP_FRAME_CNT                          0x0002f144
#define PMC_FC_TTTM_REG_GFP_ERRED_FRAMES_CNT                   0x0002f148
#define PMC_FC_TTTM_REG_CNT_UPDATE                             0x0002f14c

/*-----------------------------.
 | Register 0x0002f100 CFG     |
 +-----------------------------+
 | bit  26 R/W  DIS_SCRAM      |
 | bit  25 R/W  FORCE_LF       |
 | bit  24 R/W  FORCE_ERR      |
 | bit  14 R/W  LFOS_HIBER     |
 | bit  13 R/W  LFOS_SYNC_LOST |
 | bit  12 R/W  LFOS_LINK_FAIL |
 | bit  0  R/W  SW_RESET       |
 +----------------------------*/
#define PMC_FC_TTTM_REG_CFG_UNUSED_MASK        0xf00000fc
#define FC_TTTM_REG_CFG_BIT_DIS_SCRAM_MSK      0x04000000
#define FC_TTTM_REG_CFG_BIT_DIS_SCRAM_OFF      26
#define FC_TTTM_REG_CFG_BIT_FORCE_LF_MSK       0x02000000
#define FC_TTTM_REG_CFG_BIT_FORCE_LF_OFF       25
#define FC_TTTM_REG_CFG_BIT_FORCE_ERR_MSK      0x01000000
#define FC_TTTM_REG_CFG_BIT_FORCE_ERR_OFF      24
#define FC_TTTM_REG_CFG_BIT_LFOS_HIBER_MSK     0x00004000
#define FC_TTTM_REG_CFG_BIT_LFOS_HIBER_OFF     14
#define FC_TTTM_REG_CFG_BIT_LFOS_SYNC_LOST_MSK 0x00002000
#define FC_TTTM_REG_CFG_BIT_LFOS_SYNC_LOST_OFF 13
#define FC_TTTM_REG_CFG_BIT_LFOS_LINK_FAIL_MSK 0x00001000
#define FC_TTTM_REG_CFG_BIT_LFOS_LINK_FAIL_OFF 12
#define FC_TTTM_REG_CFG_BIT_SW_RESET_MSK       0x00000001
#define FC_TTTM_REG_CFG_BIT_SW_RESET_OFF       0

/*-------------------------.
 | Register 0x0002f104 INT |
 +-------------------------+
 | bit  5 R/W  DPI_I       |
 | bit  4 R/W  FOVR_I      |
 | bit  3 R/W  BADBLK_I    |
 | bit  2 R/W  LOSYNC_I    |
 | bit  1 R/W  HIBER_I     |
 | bit  0 R/W  LINK_FAIL_I |
 +------------------------*/
#define PMC_FC_TTTM_REG_INT_UNUSED_MASK     0xffffffc0
#define FC_TTTM_REG_INT_BIT_DPI_I_MSK       0x00000020
#define FC_TTTM_REG_INT_BIT_DPI_I_OFF       5
#define FC_TTTM_REG_INT_BIT_FOVR_I_MSK      0x00000010
#define FC_TTTM_REG_INT_BIT_FOVR_I_OFF      4
#define FC_TTTM_REG_INT_BIT_BADBLK_I_MSK    0x00000008
#define FC_TTTM_REG_INT_BIT_BADBLK_I_OFF    3
#define FC_TTTM_REG_INT_BIT_LOSYNC_I_MSK    0x00000004
#define FC_TTTM_REG_INT_BIT_LOSYNC_I_OFF    2
#define FC_TTTM_REG_INT_BIT_HIBER_I_MSK     0x00000002
#define FC_TTTM_REG_INT_BIT_HIBER_I_OFF     1
#define FC_TTTM_REG_INT_BIT_LINK_FAIL_I_MSK 0x00000001
#define FC_TTTM_REG_INT_BIT_LINK_FAIL_I_OFF 0

/*-----------------------------.
 | Register 0x0002f108 INT_VAL |
 +-----------------------------+
 | bit  2 R  LOSYNC_V          |
 | bit  1 R  HIBER_V           |
 | bit  0 R  LINK_FAIL_V       |
 +----------------------------*/
#define PMC_FC_TTTM_REG_INT_VAL_UNUSED_MASK     0xfffffff8
#define FC_TTTM_REG_INT_VAL_BIT_LOSYNC_V_MSK    0x00000004
#define FC_TTTM_REG_INT_VAL_BIT_LOSYNC_V_OFF    2
#define FC_TTTM_REG_INT_VAL_BIT_HIBER_V_MSK     0x00000002
#define FC_TTTM_REG_INT_VAL_BIT_HIBER_V_OFF     1
#define FC_TTTM_REG_INT_VAL_BIT_LINK_FAIL_V_MSK 0x00000001
#define FC_TTTM_REG_INT_VAL_BIT_LINK_FAIL_V_OFF 0

/*----------------------------.
 | Register 0x0002f10c INT_EN |
 +----------------------------+
 | bit  5 R/W  DPI_E          |
 | bit  4 R/W  FOVR_E         |
 | bit  3 R/W  BADBLK_E       |
 | bit  2 R/W  LOSYNC_E       |
 | bit  1 R/W  HIBER_E        |
 | bit  0 R/W  LINK_FAIL_E    |
 +---------------------------*/
#define PMC_FC_TTTM_REG_INT_EN_UNUSED_MASK     0xffffffc0
#define FC_TTTM_REG_INT_EN_BIT_DPI_E_MSK       0x00000020
#define FC_TTTM_REG_INT_EN_BIT_DPI_E_OFF       5
#define FC_TTTM_REG_INT_EN_BIT_FOVR_E_MSK      0x00000010
#define FC_TTTM_REG_INT_EN_BIT_FOVR_E_OFF      4
#define FC_TTTM_REG_INT_EN_BIT_BADBLK_E_MSK    0x00000008
#define FC_TTTM_REG_INT_EN_BIT_BADBLK_E_OFF    3
#define FC_TTTM_REG_INT_EN_BIT_LOSYNC_E_MSK    0x00000004
#define FC_TTTM_REG_INT_EN_BIT_LOSYNC_E_OFF    2
#define FC_TTTM_REG_INT_EN_BIT_HIBER_E_MSK     0x00000002
#define FC_TTTM_REG_INT_EN_BIT_HIBER_E_OFF     1
#define FC_TTTM_REG_INT_EN_BIT_LINK_FAIL_E_MSK 0x00000001
#define FC_TTTM_REG_INT_EN_BIT_LINK_FAIL_E_OFF 0

/*-------------------------------------.
 | Register 0x0002f110 GFP_CORE_HEADER |
 +-------------------------------------+
 | bit  31:16 R/W  PLI                 |
 | bit  15:0  R/W  CHEC                |
 +------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_CORE_HEADER_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_MSK     0xffff0000
#define FC_TTTM_REG_GFP_CORE_HEADER_BIT_PLI_OFF     16
#define FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_MSK    0x0000ffff
#define FC_TTTM_REG_GFP_CORE_HEADER_BIT_CHEC_OFF    0

/*----------------------------------------.
 | Register 0x0002f114 GFP_PAYLOAD_HEADER |
 +----------------------------------------+
 | bit  31:29 R/W  PTI                    |
 | bit  28    R/W  PFI                    |
 | bit  27:24 R/W  EXI                    |
 | bit  23:16 R/W  UPI                    |
 | bit  15:0  R/W  THEC                   |
 +---------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_PAYLOAD_HEADER_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_MSK     0xe0000000
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PTI_OFF     29
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PFI_MSK     0x10000000
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_PFI_OFF     28
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_MSK     0x0f000000
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_EXI_OFF     24
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_MSK     0x00ff0000
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_UPI_OFF     16
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_MSK    0x0000ffff
#define FC_TTTM_REG_GFP_PAYLOAD_HEADER_BIT_THEC_OFF    0

/*--------------------------------------------.
 | Register 0x0002f118 GFP_RESERVED_BYTES_1_4 |
 +--------------------------------------------+
 | bit  31:0 R/W  GFP_RES1                    |
 +-------------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_UNUSED_MASK  0x00000000
#define FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_MSK 0xffffffff
#define FC_TTTM_REG_GFP_RESERVED_BYTES_1_4_BIT_GFP_RES1_OFF 0

/*--------------------------------------------.
 | Register 0x0002f11c GFP_RESERVED_BYTES_5_8 |
 +--------------------------------------------+
 | bit  31:0 R/W  GFP_RES2                    |
 +-------------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_UNUSED_MASK  0x00000000
#define FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_MSK 0xffffffff
#define FC_TTTM_REG_GFP_RESERVED_BYTES_5_8_BIT_GFP_RES2_OFF 0

/*---------------------------------------------.
 | Register 0x0002f120 GFP_RESERVED_BYTES_9_12 |
 +---------------------------------------------+
 | bit  31:0 R/W  GFP_RES3                     |
 +--------------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_UNUSED_MASK  0x00000000
#define FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_MSK 0xffffffff
#define FC_TTTM_REG_GFP_RESERVED_BYTES_9_12_BIT_GFP_RES3_OFF 0

/*----------------------------------------------.
 | Register 0x0002f124 GFP_RESERVED_BYTES_13_16 |
 +----------------------------------------------+
 | bit  31:0 R/W  GFP_RES4                      |
 +---------------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_UNUSED_MASK  0x00000000
#define FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_MSK 0xffffffff
#define FC_TTTM_REG_GFP_RESERVED_BYTES_13_16_BIT_GFP_RES4_OFF 0

/*-------------------------------------------------.
 | Register 0x0002f128 LOCAL_FAULT_ORDERED_SET_LSB |
 +-------------------------------------------------+
 | bit  31:0 R/W  LFOS                             |
 +------------------------------------------------*/
#define PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK    0xffffffff
#define FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF    0

/*-------------------------------------------------.
 | Register 0x0002f12c LOCAL_FAULT_ORDERED_SET_MSB |
 +-------------------------------------------------+
 | bit  31:0 R/W  LFOS                             |
 +------------------------------------------------*/
#define PMC_FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK    0xffffffff
#define FC_TTTM_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF    0

/*----------------------------------------.
 | Register 0x0002f130 ERR_CTRL_BLOCK_LSB |
 +----------------------------------------+
 | bit  31:0 R/W  ECB                     |
 +---------------------------------------*/
#define PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_MSK     0xffffffff
#define FC_TTTM_REG_ERR_CTRL_BLOCK_LSB_BIT_ECB_OFF     0

/*----------------------------------------.
 | Register 0x0002f134 ERR_CTRL_BLOCK_MSB |
 +----------------------------------------+
 | bit  31:0 R/W  ECB                     |
 +---------------------------------------*/
#define PMC_FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_MSK     0xffffffff
#define FC_TTTM_REG_ERR_CTRL_BLOCK_MSB_BIT_ECB_OFF     0

/*----------------------------------------------------------.
 | Register 0x0002f13c ORDERED_SET_INSERTION_RATE_NUMERATOR |
 +----------------------------------------------------------+
 | bit  31:0 R/W  OSIR_NUM                                  |
 +---------------------------------------------------------*/
#define PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_UNUSED_MASK  0x00000000
#define FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_MSK 0xffffffff
#define FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_NUMERATOR_BIT_OSIR_NUM_OFF 0

/*------------------------------------------------------------.
 | Register 0x0002f140 ORDERED_SET_INSERTION_RATE_DENOMINATOR |
 +------------------------------------------------------------+
 | bit  31:0 R/W  OSIR_DENOM                                  |
 +-----------------------------------------------------------*/
#define PMC_FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_UNUSED_MASK    0x00000000
#define FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_MSK 0xffffffff
#define FC_TTTM_REG_ORDERED_SET_INSERTION_RATE_DENOMINATOR_BIT_OSIR_DENOM_OFF 0

/*-----------------------------------.
 | Register 0x0002f144 GFP_FRAME_CNT |
 +-----------------------------------+
 | bit  23:0 R  GFPCNT               |
 +----------------------------------*/
#define PMC_FC_TTTM_REG_GFP_FRAME_CNT_UNUSED_MASK 0x00000000
#define FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_MSK  0x00ffffff
#define FC_TTTM_REG_GFP_FRAME_CNT_BIT_GFPCNT_OFF  0

/*------------------------------------------.
 | Register 0x0002f148 GFP_ERRED_FRAMES_CNT |
 +------------------------------------------+
 | bit  23:0 R  GFPERRCNT                   |
 +-----------------------------------------*/
#define PMC_FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_UNUSED_MASK   0x00000000
#define FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_MSK 0x00ffffff
#define FC_TTTM_REG_GFP_ERRED_FRAMES_CNT_BIT_GFPERRCNT_OFF 0

/*--------------------------------.
 | Register 0x0002f14c CNT_UPDATE |
 +--------------------------------+
 | bit  31 R    TIP               |
 | bit  0  R/W  CNTR_UPDATE       |
 +-------------------------------*/
#define PMC_FC_TTTM_REG_CNT_UPDATE_UNUSED_MASK     0x7ffffffe
#define FC_TTTM_REG_CNT_UPDATE_BIT_TIP_MSK         0x80000000
#define FC_TTTM_REG_CNT_UPDATE_BIT_TIP_OFF         31
#define FC_TTTM_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK 0x00000001
#define FC_TTTM_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_TTTM_REGS_H */
