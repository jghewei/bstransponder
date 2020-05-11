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
 *     fc_tttd block
 *****************************************************************************/
#ifndef _FC_TTTD_REGS_H
#define _FC_TTTD_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FC_TTTD_REG_CFG                         0x00050100
#define PMC_FC_TTTD_REG_INT                         0x00050104
#define PMC_FC_TTTD_REG_INT_EN                      0x00050108
#define PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB 0x0005010c
#define PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB 0x00050110
#define PMC_FC_TTTD_REG_CRC_24_ERR_CNT              0x00050118
#define PMC_FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT    0x0005011c
#define PMC_FC_TTTD_REG_CNT_UPDATE                  0x00050120

/*--------------------------.
 | Register 0x00050100 CFG  |
 +--------------------------+
 | bit  25  R/W  FORCE_LF   |
 | bit  9:8 R/W  ZONE_THRES |
 | bit  0   R/W  SW_RESET   |
 +-------------------------*/
#define PMC_FC_TTTD_REG_CFG_UNUSED_MASK    0xf0fffcf8
#define FC_TTTD_REG_CFG_BIT_FORCE_LF_MSK   0x02000000
#define FC_TTTD_REG_CFG_BIT_FORCE_LF_OFF   25
#define FC_TTTD_REG_CFG_BIT_ZONE_THRES_MSK 0x00000300
#define FC_TTTD_REG_CFG_BIT_ZONE_THRES_OFF 8
#define FC_TTTD_REG_CFG_BIT_SW_RESET_MSK   0x00000001
#define FC_TTTD_REG_CFG_BIT_SW_RESET_OFF   0

/*---------------------------.
 | Register 0x00050104 INT   |
 +---------------------------+
 | bit  2 R/W  INVALBLKHDR_I |
 | bit  1 R/W  UNDERRUN_I    |
 | bit  0 R/W  CRC24ERR_I    |
 +--------------------------*/
#define PMC_FC_TTTD_REG_INT_UNUSED_MASK       0xfffffff0
#define FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_MSK 0x00000004
#define FC_TTTD_REG_INT_BIT_INVALBLKHDR_I_OFF 2
#define FC_TTTD_REG_INT_BIT_UNDERRUN_I_MSK    0x00000002
#define FC_TTTD_REG_INT_BIT_UNDERRUN_I_OFF    1
#define FC_TTTD_REG_INT_BIT_CRC24ERR_I_MSK    0x00000001
#define FC_TTTD_REG_INT_BIT_CRC24ERR_I_OFF    0

/*----------------------------.
 | Register 0x00050108 INT_EN |
 +----------------------------+
 | bit  2 R/W  INVALBLKHDR_E  |
 | bit  1 R/W  UNDERRUN_E     |
 | bit  0 R/W  CRC24ERR_E     |
 +---------------------------*/
#define PMC_FC_TTTD_REG_INT_EN_UNUSED_MASK       0xfffffff0
#define FC_TTTD_REG_INT_EN_BIT_INVALBLKHDR_E_MSK 0x00000004
#define FC_TTTD_REG_INT_EN_BIT_INVALBLKHDR_E_OFF 2
#define FC_TTTD_REG_INT_EN_BIT_UNDERRUN_E_MSK    0x00000002
#define FC_TTTD_REG_INT_EN_BIT_UNDERRUN_E_OFF    1
#define FC_TTTD_REG_INT_EN_BIT_CRC24ERR_E_MSK    0x00000001
#define FC_TTTD_REG_INT_EN_BIT_CRC24ERR_E_OFF    0

/*-------------------------------------------------.
 | Register 0x0005010c LOCAL_FAULT_ORDERED_SET_LSB |
 +-------------------------------------------------+
 | bit  31:0 R/W  LFOS                             |
 +------------------------------------------------*/
#define PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_UNUSED_MASK 0x00000000
#define FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_MSK    0xffffffff
#define FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_LSB_BIT_LFOS_OFF    0

/*-------------------------------------------------.
 | Register 0x00050110 LOCAL_FAULT_ORDERED_SET_MSB |
 +-------------------------------------------------+
 | bit  31:0 R/W  LFOS                             |
 +------------------------------------------------*/
#define PMC_FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_UNUSED_MASK 0x00000000
#define FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_MSK    0xffffffff
#define FC_TTTD_REG_LOCAL_FAULT_ORDERED_SET_MSB_BIT_LFOS_OFF    0

/*------------------------------------.
 | Register 0x00050118 CRC_24_ERR_CNT |
 +------------------------------------+
 | bit  15:0 R  CRCERRCNT             |
 +-----------------------------------*/
#define PMC_FC_TTTD_REG_CRC_24_ERR_CNT_UNUSED_MASK   0x00000000
#define FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_MSK 0x0000ffff
#define FC_TTTD_REG_CRC_24_ERR_CNT_BIT_CRCERRCNT_OFF 0

/*----------------------------------------------.
 | Register 0x0005011c INVALID_BLOCK_HEADER_CNT |
 +----------------------------------------------+
 | bit  15:0 R  INVALBLKHDRCNT                  |
 +---------------------------------------------*/
#define PMC_FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_UNUSED_MASK        0x00000000
#define FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_MSK 0x0000ffff
#define FC_TTTD_REG_INVALID_BLOCK_HEADER_CNT_BIT_INVALBLKHDRCNT_OFF 0

/*--------------------------------.
 | Register 0x00050120 CNT_UPDATE |
 +--------------------------------+
 | bit  31 R    TIP               |
 | bit  0  R/W  CNTR_UPDATE       |
 +-------------------------------*/
#define PMC_FC_TTTD_REG_CNT_UPDATE_UNUSED_MASK     0x7ffffffe
#define FC_TTTD_REG_CNT_UPDATE_BIT_TIP_MSK         0x80000000
#define FC_TTTD_REG_CNT_UPDATE_BIT_TIP_OFF         31
#define FC_TTTD_REG_CNT_UPDATE_BIT_CNTR_UPDATE_MSK 0x00000001
#define FC_TTTD_REG_CNT_UPDATE_BIT_CNTR_UPDATE_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FC_TTTD_REGS_H */
