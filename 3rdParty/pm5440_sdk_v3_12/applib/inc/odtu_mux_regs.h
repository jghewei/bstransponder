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
 *     odtu_mux block
 *****************************************************************************/
#ifndef _ODTU_MUX_REGS_H
#define _ODTU_MUX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ODTU_MUX_REG_CAL_EN_CTRL( N )           (0x00000000 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL( N )       (0x00000180 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_LO_DPATH_CTRL( N )         (0x00000300 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_CONFIG                     0x00000480
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN( N )  (0x00000500 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_SW_OVFLW_EN( N )           (0x00000520 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT( N ) (0x00000540 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT( N )          (0x00000560 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL( N ) (0x00000580 + (N) * 0x4)
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL( N )      (0x000005a0 + (N) * 0x4)

/* index definitions for PMC_ODTU_MUX_REG_CAL_EN_CTRL */
#define PMC_ODTU_MUX_REG_CAL_EN_CTRL_INDEX_N_MIN    0
#define PMC_ODTU_MUX_REG_CAL_EN_CTRL_INDEX_N_MAX    95
#define PMC_ODTU_MUX_REG_CAL_EN_CTRL_INDEX_N_SIZE   96
#define PMC_ODTU_MUX_REG_CAL_EN_CTRL_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x00000000 + (N) * 0x4) CAL_EN_CTRL |
 +-----------------------------------------------+
 | bit  0 R/W  CAL_EN                            |
 +----------------------------------------------*/
#define PMC_ODTU_MUX_REG_CAL_EN_CTRL_UNUSED_MASK    0xfffffffe
#define ODTU_MUX_REG_CAL_EN_CTRL_BIT_CAL_EN_MSK     0x00000001
#define ODTU_MUX_REG_CAL_EN_CTRL_BIT_CAL_EN_OFF     0

/* index definitions for PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL */
#define PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL_INDEX_N_MIN    0
#define PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL_INDEX_N_MAX    95
#define PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL_INDEX_N_SIZE   96
#define PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL_INDEX_N_OFFSET 0x4

/*---------------------------------------------------.
 | Register (0x00000180 + (N) * 0x4) HO_CH_RSTB_CTRL |
 +---------------------------------------------------+
 | bit  0 R/W  HO_CH_RSTB                            |
 +--------------------------------------------------*/
#define PMC_ODTU_MUX_REG_HO_CH_RSTB_CTRL_UNUSED_MASK    0xfffffffe
#define ODTU_MUX_REG_HO_CH_RSTB_CTRL_BIT_HO_CH_RSTB_MSK 0x00000001
#define ODTU_MUX_REG_HO_CH_RSTB_CTRL_BIT_HO_CH_RSTB_OFF 0

/* index definitions for PMC_ODTU_MUX_REG_LO_DPATH_CTRL */
#define PMC_ODTU_MUX_REG_LO_DPATH_CTRL_INDEX_N_MIN    0
#define PMC_ODTU_MUX_REG_LO_DPATH_CTRL_INDEX_N_MAX    95
#define PMC_ODTU_MUX_REG_LO_DPATH_CTRL_INDEX_N_SIZE   96
#define PMC_ODTU_MUX_REG_LO_DPATH_CTRL_INDEX_N_OFFSET 0x4

/*-------------------------------------------------.
 | Register (0x00000300 + (N) * 0x4) LO_DPATH_CTRL |
 +-------------------------------------------------+
 | bit  16 R/W  PASSTHRU                           |
 | bit  8  R/W  LO_CH_RSTB                         |
 +------------------------------------------------*/
#define PMC_ODTU_MUX_REG_LO_DPATH_CTRL_UNUSED_MASK    0xfffefeff
#define ODTU_MUX_REG_LO_DPATH_CTRL_BIT_PASSTHRU_MSK   0x00010000
#define ODTU_MUX_REG_LO_DPATH_CTRL_BIT_PASSTHRU_OFF   16
#define ODTU_MUX_REG_LO_DPATH_CTRL_BIT_LO_CH_RSTB_MSK 0x00000100
#define ODTU_MUX_REG_LO_DPATH_CTRL_BIT_LO_CH_RSTB_OFF 8

/*-------------------------------.
 | Register 0x00000480 CONFIG    |
 +-------------------------------+
 | bit  8 R/W  FORCE_CONFIG_ADDR |
 | bit  0 R/W  FORCE_CONFIG      |
 +------------------------------*/
#define PMC_ODTU_MUX_REG_CONFIG_UNUSED_MASK           0xfcfefefe
#define ODTU_MUX_REG_CONFIG_BIT_FORCE_CONFIG_ADDR_MSK 0x00000100
#define ODTU_MUX_REG_CONFIG_BIT_FORCE_CONFIG_ADDR_OFF 8
#define ODTU_MUX_REG_CONFIG_BIT_FORCE_CONFIG_MSK      0x00000001
#define ODTU_MUX_REG_CONFIG_BIT_FORCE_CONFIG_OFF      0

/* index definitions for PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN */
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_INDEX_N_MIN             0
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_INDEX_N_MAX             2
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_INDEX_N_SIZE            3
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_INDEX_N_OFFSET          0x4

/*--------------------------------------------------------.
 | Register (0x00000500 + (N) * 0x4) TXJC_RES_OVERFLOW_EN |
 +--------------------------------------------------------+
 | bit  31:0 R/W  TXJC_RES_OVERFLOW_E                     |
 +-------------------------------------------------------*/
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_UNUSED_MASK             0x00000000
#define ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_BIT_TXJC_RES_OVERFLOW_E_MSK 0xffffffff
#define ODTU_MUX_REG_TXJC_RES_OVERFLOW_EN_BIT_TXJC_RES_OVERFLOW_E_OFF 0

/* index definitions for PMC_ODTU_MUX_REG_SW_OVFLW_EN */
#define PMC_ODTU_MUX_REG_SW_OVFLW_EN_INDEX_N_MIN    0
#define PMC_ODTU_MUX_REG_SW_OVFLW_EN_INDEX_N_MAX    2
#define PMC_ODTU_MUX_REG_SW_OVFLW_EN_INDEX_N_SIZE   3
#define PMC_ODTU_MUX_REG_SW_OVFLW_EN_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x00000520 + (N) * 0x4) SW_OVFLW_EN |
 +-----------------------------------------------+
 |                                               |
 | bit  31:0 R/W  SW_OVFLW_E                     |
 |                                               |
 +----------------------------------------------*/
#define PMC_ODTU_MUX_REG_SW_OVFLW_EN_UNUSED_MASK    0x00000000
#define ODTU_MUX_REG_SW_OVFLW_EN_BIT_SW_OVFLW_E_MSK 0xffffffff
#define ODTU_MUX_REG_SW_OVFLW_EN_BIT_SW_OVFLW_E_OFF 0

/* index definitions for PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT */
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_INDEX_N_MIN             0
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_INDEX_N_MAX             2
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_INDEX_N_SIZE            3
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_INDEX_N_OFFSET          0x4

/*---------------------------------------------------------.
 | Register (0x00000540 + (N) * 0x4) TXJC_RES_OVERFLOW_INT |
 +---------------------------------------------------------+
 | bit  31:0 R/W  TXJC_RES_OVERFLOW_I                      |
 +--------------------------------------------------------*/
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_UNUSED_MASK             0x00000000
#define ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_BIT_TXJC_RES_OVERFLOW_I_MSK 0xffffffff
#define ODTU_MUX_REG_TXJC_RES_OVERFLOW_INT_BIT_TXJC_RES_OVERFLOW_I_OFF 0

/* index definitions for PMC_ODTU_MUX_REG_SW_OVFLW_INT */
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_INDEX_N_MIN    0
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_INDEX_N_MAX    2
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_INDEX_N_SIZE   3
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x00000560 + (N) * 0x4) SW_OVFLW_INT |
 +------------------------------------------------+
 |                                                |
 | bit  31:0 R/W  SW_OVFLW_I                      |
 |                                                |
 +-----------------------------------------------*/
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_UNUSED_MASK    0x00000000
#define ODTU_MUX_REG_SW_OVFLW_INT_BIT_SW_OVFLW_I_MSK 0xffffffff
#define ODTU_MUX_REG_SW_OVFLW_INT_BIT_SW_OVFLW_I_OFF 0

/* index definitions for PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL */
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_INDEX_N_MIN             0
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_INDEX_N_MAX             2
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_INDEX_N_SIZE            3
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_INDEX_N_OFFSET          0x4

/*---------------------------------------------------------.
 | Register (0x00000580 + (N) * 0x4) TXJC_RES_OVERFLOW_VAL |
 +---------------------------------------------------------+
 | bit  31:0 R  TXJC_RES_OVERFLOW_V                        |
 +--------------------------------------------------------*/
#define PMC_ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_UNUSED_MASK             0x00000000
#define ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_BIT_TXJC_RES_OVERFLOW_V_MSK 0xffffffff
#define ODTU_MUX_REG_TXJC_RES_OVERFLOW_VAL_BIT_TXJC_RES_OVERFLOW_V_OFF 0

/* index definitions for PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL */
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL_INDEX_N_MIN    0
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL_INDEX_N_MAX    2
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL_INDEX_N_SIZE   3
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL_INDEX_N_OFFSET 0x4

/*----------------------------------------------------.
 | Register (0x000005a0 + (N) * 0x4) SW_OVFLW_INT_VAL |
 +----------------------------------------------------+
 |                                                    |
 | bit  31:0 R  SW_OVFLW_V                            |
 |                                                    |
 +---------------------------------------------------*/
#define PMC_ODTU_MUX_REG_SW_OVFLW_INT_VAL_UNUSED_MASK    0x00000000
#define ODTU_MUX_REG_SW_OVFLW_INT_VAL_BIT_SW_OVFLW_V_MSK 0xffffffff
#define ODTU_MUX_REG_SW_OVFLW_INT_VAL_BIT_SW_OVFLW_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODTU_MUX_REGS_H */
