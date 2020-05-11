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
 *     sdh_pmg block
 *****************************************************************************/
#ifndef _SDH_PMG_REGS_H
#define _SDH_PMG_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SDH_PMG_REG_TOP_CFG_REG                     0x00000000
#define PMC_SDH_PMG_REG_TOP_SW_RESET                    0x00000004
#define PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET( N ) (0x00000008 + (N) * 0x4)
#define PMC_SDH_PMG_REG_CHANNEL_CFG_REG( N )            (0x00000018 + (N) * 0x4)
#define PMC_SDH_PMG_REG_MTSB_INT_EN                     0x00000028
#define PMC_SDH_PMG_REG_INT_SUMMARY                     0x0000002c
#define PMC_SDH_PMG_REG_TIP_SUMMARY                     0x00000030

/*---------------------------------.
 | Register 0x00000000 TOP_CFG_REG |
 +---------------------------------+
 | bit  12 R/W  RRMP_U_ASCLK       |
 | bit  9  R/W  LBIP_BER_SEL       |
 | bit  0  R/W  OC_768_MODE        |
 +--------------------------------*/
#define PMC_SDH_PMG_REG_TOP_CFG_REG_UNUSED_MASK      0xffff60fe
#define SDH_PMG_REG_TOP_CFG_REG_BIT_RRMP_U_ASCLK_MSK 0x00001000
#define SDH_PMG_REG_TOP_CFG_REG_BIT_RRMP_U_ASCLK_OFF 12
#define SDH_PMG_REG_TOP_CFG_REG_BIT_LBIP_BER_SEL_MSK 0x00000200
#define SDH_PMG_REG_TOP_CFG_REG_BIT_LBIP_BER_SEL_OFF 9
#define SDH_PMG_REG_TOP_CFG_REG_BIT_OC_768_MODE_MSK  0x00000001
#define SDH_PMG_REG_TOP_CFG_REG_BIT_OC_768_MODE_OFF  0

/*----------------------------------.
 | Register 0x00000004 TOP_SW_RESET |
 +----------------------------------+
 | bit  0 R/W  TOP_SW_RST           |
 +---------------------------------*/
#define PMC_SDH_PMG_REG_TOP_SW_RESET_UNUSED_MASK    0xfffffffe
#define SDH_PMG_REG_TOP_SW_RESET_BIT_TOP_SW_RST_MSK 0x00000001
#define SDH_PMG_REG_TOP_SW_RESET_BIT_TOP_SW_RST_OFF 0

/* index definitions for PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET */
#define PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_INDEX_N_MIN          0
#define PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_INDEX_N_MAX          3
#define PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_INDEX_N_SIZE         4
#define PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_INDEX_N_OFFSET       0x4

/*--------------------------------------------------------------.
 | Register (0x00000008 + (N) * 0x4) CHANNEL_LOW_POWER_SW_RESET |
 +--------------------------------------------------------------+
 | bit  10 R/W  CH_SBER_U_SW_RST                                |
 | bit  9  R/W  CH_RTTP_U_SW_RST                                |
 | bit  8  R/W  CH_RRMP_U_SW_RST                                |
 | bit  0  R/W  CH_LOW_PWR                                      |
 +-------------------------------------------------------------*/
#define PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_UNUSED_MASK          0xfffff8fe
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_SBER_U_SW_RST_MSK 0x00000400
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_SBER_U_SW_RST_OFF 10
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RTTP_U_SW_RST_MSK 0x00000200
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RTTP_U_SW_RST_OFF 9
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RRMP_U_SW_RST_MSK 0x00000100
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RRMP_U_SW_RST_OFF 8
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_LOW_PWR_MSK       0x00000001
#define SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_LOW_PWR_OFF       0

/* index definitions for PMC_SDH_PMG_REG_CHANNEL_CFG_REG */
#define PMC_SDH_PMG_REG_CHANNEL_CFG_REG_INDEX_N_MIN         0
#define PMC_SDH_PMG_REG_CHANNEL_CFG_REG_INDEX_N_MAX         3
#define PMC_SDH_PMG_REG_CHANNEL_CFG_REG_INDEX_N_SIZE        4
#define PMC_SDH_PMG_REG_CHANNEL_CFG_REG_INDEX_N_OFFSET      0x4

/*---------------------------------------------------.
 | Register (0x00000018 + (N) * 0x4) CHANNEL_CFG_REG |
 +---------------------------------------------------+
 | bit  9 R/W  LOF_PN11_INSERT                       |
 | bit  8 R/W  LOS_PN11_INSERT                       |
 | bit  0 R/W  CHNL_EN                               |
 +--------------------------------------------------*/
#define PMC_SDH_PMG_REG_CHANNEL_CFG_REG_UNUSED_MASK         0xfffffcf8
#define SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOF_PN11_INSERT_MSK 0x00000200
#define SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOF_PN11_INSERT_OFF 9
#define SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOS_PN11_INSERT_MSK 0x00000100
#define SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOS_PN11_INSERT_OFF 8
#define SDH_PMG_REG_CHANNEL_CFG_REG_BIT_CHNL_EN_MSK         0x00000001
#define SDH_PMG_REG_CHANNEL_CFG_REG_BIT_CHNL_EN_OFF         0

/*---------------------------------.
 | Register 0x00000028 MTSB_INT_EN |
 +---------------------------------+
 | bit  11:8 R/W  SBER_U_INT_EN    |
 | bit  7:4  R/W  RTTP_U_INT_EN    |
 | bit  3:0  R/W  RRMP_U_INT_EN    |
 +--------------------------------*/
#define PMC_SDH_PMG_REG_MTSB_INT_EN_UNUSED_MASK       0xfffff000
#define SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_MSK 0x00000f00
#define SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF 8
#define SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_MSK 0x000000f0
#define SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF 4
#define SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_MSK 0x0000000f
#define SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF 0

/*---------------------------------.
 | Register 0x0000002c INT_SUMMARY |
 +---------------------------------+
 | bit  11:8 R  SBER_U_INT         |
 | bit  7:4  R  RTTP_U_INT         |
 | bit  3:0  R  RRMP_U_INT         |
 +--------------------------------*/
#define PMC_SDH_PMG_REG_INT_SUMMARY_UNUSED_MASK    0xfffff000
#define SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_MSK 0x00000f00
#define SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_OFF 8
#define SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_MSK 0x000000f0
#define SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_OFF 4
#define SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_MSK 0x0000000f
#define SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_OFF 0

/*---------------------------------.
 | Register 0x00000030 TIP_SUMMARY |
 +---------------------------------+
 | bit  15:0 R  RRMP_U_TIP         |
 +--------------------------------*/
#define PMC_SDH_PMG_REG_TIP_SUMMARY_UNUSED_MASK    0xffff0000
#define SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_MSK 0x0000ffff
#define SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SDH_PMG_REGS_H */
