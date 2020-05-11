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
 *     d8_rx_2x_cdru block
 *****************************************************************************/
#ifndef _D8_RX_2X_CDRU_REGS_H
#define _D8_RX_2X_CDRU_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1 0x00000200
#define PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4 0x0000020c

/*---------------------------------------.
 | Register 0x00000200 DELTA8_CDRU_CFG_1 |
 +---------------------------------------+
 | bit  18 R/W  FILT_EN                  |
 | bit  17 R/W  IPD_EN                   |
 | bit  16 R/W  INV_REC_DATA             |
 | bit  1  R/W  TED_SEL                  |
 +--------------------------------------*/
#define PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_UNUSED_MASK      0x03f80001
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_FILT_EN_MSK      0x00040000
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_FILT_EN_OFF      18
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_IPD_EN_MSK       0x00020000
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_IPD_EN_OFF       17
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_INV_REC_DATA_MSK 0x00010000
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_INV_REC_DATA_OFF 16
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_TED_SEL_MSK      0x00000002
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_1_BIT_TED_SEL_OFF      1

/*---------------------------------------.
 | Register 0x0000020c DELTA8_CDRU_CFG_4 |
 +---------------------------------------+
 | bit  30:21 R/W  LOS_CNT               |
 | bit  20    R/W  LOS_ENABLE            |
 +--------------------------------------*/
#define PMC_DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_UNUSED_MASK    0x800fffff
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_MSK    0x7fe00000
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_CNT_OFF    21
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_ENABLE_MSK 0x00100000
#define DELTA8_CDRU_REG_DELTA8_CDRU_CFG_4_BIT_LOS_ENABLE_OFF 20

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _D8_RX_2X_CDRU_REGS_H */
