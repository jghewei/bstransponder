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
 *     cbr_dint block
 *****************************************************************************/
#ifndef _CBR_DINT_REGS_H
#define _CBR_DINT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG( N ) (0x00040000 + (N) * 4)
#define PMC_CBR_DINT_REG_CHANNEL_UPDATE_REG                   0x0004000c
#define PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG( N )             (0x00040010 + (N) * 4)
#define PMC_CBR_DINT_REG_FIFO_UF_OVF_INT_VAL                  0x00040040
#define PMC_CBR_DINT_REG_FIFO_UF_OVF_INT_EN                   0x00040044
#define PMC_CBR_DINT_REG_FIFO_UF_OVF_INT_ACTIVE               0x00040048
#define PMC_CBR_DINT_REG_STAGING_BUF_OVF_INT_VAL              0x0004005c
#define PMC_CBR_DINT_REG_STAGING_BUF_OVF_INT_EN               0x00040060
#define PMC_CBR_DINT_REG_STAGING_BUF_OVF_INT_ACTIVE           0x00040064

/* index definitions for PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG */
#define PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_MIN            0
#define PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_MAX            2
#define PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_SIZE           3
#define PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_INDEX_N_OFFSET         4

/*-----------------------------------------------------------------.
 | Register (0x00040000 + (N) * 4) MULTI_LANE_40G_GROUP_CONFIG_REG |
 +-----------------------------------------------------------------+
 | bit  0 R/W  MULTI_LANE_40G_SEL                                  |
 +----------------------------------------------------------------*/
#define PMC_CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_UNUSED_MASK            0xfffffffe
#define CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_MSK 0x00000001
#define CBR_DINT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_OFF 0

/*----------------------------------------.
 | Register 0x0004000c CHANNEL_UPDATE_REG |
 +----------------------------------------+
 | bit  0 R/W  CH_UPDATE                  |
 +---------------------------------------*/
#define PMC_CBR_DINT_REG_CHANNEL_UPDATE_REG_UNUSED_MASK   0xfffffffe
#define CBR_DINT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_MSK 0x00000001
#define CBR_DINT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_OFF 0

/* index definitions for PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG */
#define PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG_INDEX_N_MIN     0
#define PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG_INDEX_N_MAX     11
#define PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG_INDEX_N_SIZE    12
#define PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG_INDEX_N_OFFSET  4

/*-----------------------------------------------------.
 | Register (0x00040010 + (N) * 4) CHANNELN_CONFIG_REG |
 +-----------------------------------------------------+
 | bit  15:11 R/W  CHN_FIFO_TH                         |
 | bit  7:4   R/W  CHN_ID                              |
 | bit  1     R/W  CHN_EN                              |
 | bit  0     R/W  CHN_RESET                           |
 +----------------------------------------------------*/
#define PMC_CBR_DINT_REG_CHANNELN_CONFIG_REG_UNUSED_MASK     0xfff0070c
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_FIFO_TH_MSK 0x0000f800
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_FIFO_TH_OFF 11
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_MSK      0x000000f0
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF      4
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_MSK      0x00000002
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_OFF      1
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_RESET_MSK   0x00000001
#define CBR_DINT_REG_CHANNELN_CONFIG_REG_BIT_CHN_RESET_OFF   0

/*-----------------------------------------.
 | Register 0x00040040 FIFO_UF_OVF_INT_VAL |
 +-----------------------------------------+
 | bit  27:16 R  FIFO_OVF_INT_V            |
 | bit  11:0  R  FIFO_UF_INT_V             |
 +----------------------------------------*/
#define PMC_CBR_DINT_REG_FIFO_UF_OVF_INT_VAL_UNUSED_MASK        0xf000f000
#define CBR_DINT_REG_FIFO_UF_OVF_INT_VAL_BIT_FIFO_OVF_INT_V_MSK 0x0fff0000
#define CBR_DINT_REG_FIFO_UF_OVF_INT_VAL_BIT_FIFO_OVF_INT_V_OFF 16
#define CBR_DINT_REG_FIFO_UF_OVF_INT_VAL_BIT_FIFO_UF_INT_V_MSK  0x00000fff
#define CBR_DINT_REG_FIFO_UF_OVF_INT_VAL_BIT_FIFO_UF_INT_V_OFF  0

/*----------------------------------------.
 | Register 0x00040044 FIFO_UF_OVF_INT_EN |
 +----------------------------------------+
 | bit  27:16 R/W  FIFO_OVF_INT_E         |
 | bit  11:0  R/W  FIFO_UF_INT_E          |
 +---------------------------------------*/
#define PMC_CBR_DINT_REG_FIFO_UF_OVF_INT_EN_UNUSED_MASK        0xf000f000
#define CBR_DINT_REG_FIFO_UF_OVF_INT_EN_BIT_FIFO_OVF_INT_E_MSK 0x0fff0000
#define CBR_DINT_REG_FIFO_UF_OVF_INT_EN_BIT_FIFO_OVF_INT_E_OFF 16
#define CBR_DINT_REG_FIFO_UF_OVF_INT_EN_BIT_FIFO_UF_INT_E_MSK  0x00000fff
#define CBR_DINT_REG_FIFO_UF_OVF_INT_EN_BIT_FIFO_UF_INT_E_OFF  0

/*--------------------------------------------.
 | Register 0x00040048 FIFO_UF_OVF_INT_ACTIVE |
 +--------------------------------------------+
 | bit  27:16 R/W  FIFO_OVF_INT_I             |
 | bit  11:0  R/W  FIFO_UF_INT_I              |
 +-------------------------------------------*/
#define PMC_CBR_DINT_REG_FIFO_UF_OVF_INT_ACTIVE_UNUSED_MASK        0xf000f000
#define CBR_DINT_REG_FIFO_UF_OVF_INT_ACTIVE_BIT_FIFO_OVF_INT_I_MSK 0x0fff0000
#define CBR_DINT_REG_FIFO_UF_OVF_INT_ACTIVE_BIT_FIFO_OVF_INT_I_OFF 16
#define CBR_DINT_REG_FIFO_UF_OVF_INT_ACTIVE_BIT_FIFO_UF_INT_I_MSK  0x00000fff
#define CBR_DINT_REG_FIFO_UF_OVF_INT_ACTIVE_BIT_FIFO_UF_INT_I_OFF  0

/*---------------------------------------------.
 | Register 0x0004005c STAGING_BUF_OVF_INT_VAL |
 +---------------------------------------------+
 | bit  11:0 R  STAG_BUF_OVF_INT_V             |
 +--------------------------------------------*/
#define PMC_CBR_DINT_REG_STAGING_BUF_OVF_INT_VAL_UNUSED_MASK            0xfffff000
#define CBR_DINT_REG_STAGING_BUF_OVF_INT_VAL_BIT_STAG_BUF_OVF_INT_V_MSK 0x00000fff
#define CBR_DINT_REG_STAGING_BUF_OVF_INT_VAL_BIT_STAG_BUF_OVF_INT_V_OFF 0

/*--------------------------------------------.
 | Register 0x00040060 STAGING_BUF_OVF_INT_EN |
 +--------------------------------------------+
 | bit  11:0 R/W  STAG_BUF_OVF_INT_E          |
 +-------------------------------------------*/
#define PMC_CBR_DINT_REG_STAGING_BUF_OVF_INT_EN_UNUSED_MASK            0xfffff000
#define CBR_DINT_REG_STAGING_BUF_OVF_INT_EN_BIT_STAG_BUF_OVF_INT_E_MSK 0x00000fff
#define CBR_DINT_REG_STAGING_BUF_OVF_INT_EN_BIT_STAG_BUF_OVF_INT_E_OFF 0

/*------------------------------------------------.
 | Register 0x00040064 STAGING_BUF_OVF_INT_ACTIVE |
 +------------------------------------------------+
 | bit  11:0 R/W  STAG_BUF_OVF_INT_I              |
 +-----------------------------------------------*/
#define PMC_CBR_DINT_REG_STAGING_BUF_OVF_INT_ACTIVE_UNUSED_MASK            0xfffff000
#define CBR_DINT_REG_STAGING_BUF_OVF_INT_ACTIVE_BIT_STAG_BUF_OVF_INT_I_MSK 0x00000fff
#define CBR_DINT_REG_STAGING_BUF_OVF_INT_ACTIVE_BIT_STAG_BUF_OVF_INT_I_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBR_DINT_REGS_H */
