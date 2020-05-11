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
 *     pkt_dint block
 *****************************************************************************/
#ifndef _PKT_DINT_REGS_H
#define _PKT_DINT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PKT_DINT_REG_CHANNEL_ID_REG( N )           (0x00040400 + (N) * 4)
#define PMC_PKT_DINT_REG_CHANNEL_ID_UPDATE_REG         0x00040430
#define PMC_PKT_DINT_REG_ZONE_CFG_REG( N )             (0x00040434 + (N) * 4)
#define PMC_PKT_DINT_REG_CFC_XOFF_TH_REG( N )          (0x00040464 + (N) * 4)
#define PMC_PKT_DINT_REG_CFC_FIFO_INT_REG              0x00040494
#define PMC_PKT_DINT_REG_CFC_FIFO_INT_EN_REG           0x00040498
#define PMC_PKT_DINT_REG_CFC_FIFO_INT_STAT_REG         0x0004049c
#define PMC_PKT_DINT_REG_PACKET_DEINTERLEAVER_SW_RESET 0x000404b0

/* index definitions for PMC_PKT_DINT_REG_CHANNEL_ID_REG */
#define PMC_PKT_DINT_REG_CHANNEL_ID_REG_INDEX_N_MIN    0
#define PMC_PKT_DINT_REG_CHANNEL_ID_REG_INDEX_N_MAX    11
#define PMC_PKT_DINT_REG_CHANNEL_ID_REG_INDEX_N_SIZE   12
#define PMC_PKT_DINT_REG_CHANNEL_ID_REG_INDEX_N_OFFSET 4

/*------------------------------------------------.
 | Register (0x00040400 + (N) * 4) CHANNEL_ID_REG |
 +------------------------------------------------+
 | bit  3:0 R/W  ID                               |
 +-----------------------------------------------*/
#define PMC_PKT_DINT_REG_CHANNEL_ID_REG_UNUSED_MASK    0xfffffff0
#define PKT_DINT_REG_CHANNEL_ID_REG_BIT_ID_MSK         0x0000000f
#define PKT_DINT_REG_CHANNEL_ID_REG_BIT_ID_OFF         0

/*-------------------------------------------.
 | Register 0x00040430 CHANNEL_ID_UPDATE_REG |
 +-------------------------------------------+
 | bit  0 R/W  ID_UPDATE                     |
 +------------------------------------------*/
#define PMC_PKT_DINT_REG_CHANNEL_ID_UPDATE_REG_UNUSED_MASK   0xfffffffe
#define PKT_DINT_REG_CHANNEL_ID_UPDATE_REG_BIT_ID_UPDATE_MSK 0x00000001
#define PKT_DINT_REG_CHANNEL_ID_UPDATE_REG_BIT_ID_UPDATE_OFF 0

/* index definitions for PMC_PKT_DINT_REG_ZONE_CFG_REG */
#define PMC_PKT_DINT_REG_ZONE_CFG_REG_INDEX_N_MIN                   0
#define PMC_PKT_DINT_REG_ZONE_CFG_REG_INDEX_N_MAX                   11
#define PMC_PKT_DINT_REG_ZONE_CFG_REG_INDEX_N_SIZE                  12
#define PMC_PKT_DINT_REG_ZONE_CFG_REG_INDEX_N_OFFSET                4

/*----------------------------------------------.
 | Register (0x00040434 + (N) * 4) ZONE_CFG_REG |
 +----------------------------------------------+
 | bit  7:6 R/W  ZONE_XOFF_SET_THRESHOLD        |
 | bit  5:4 R/W  ZONE_XOFF_CLEAR_THRESHOLD      |
 | bit  0   R/W  UF_ZONE_MODE                   |
 +---------------------------------------------*/
#define PMC_PKT_DINT_REG_ZONE_CFG_REG_UNUSED_MASK                   0xffffff0e
#define PKT_DINT_REG_ZONE_CFG_REG_BIT_ZONE_XOFF_SET_THRESHOLD_MSK   0x000000c0
#define PKT_DINT_REG_ZONE_CFG_REG_BIT_ZONE_XOFF_SET_THRESHOLD_OFF   6
#define PKT_DINT_REG_ZONE_CFG_REG_BIT_ZONE_XOFF_CLEAR_THRESHOLD_MSK 0x00000030
#define PKT_DINT_REG_ZONE_CFG_REG_BIT_ZONE_XOFF_CLEAR_THRESHOLD_OFF 4
#define PKT_DINT_REG_ZONE_CFG_REG_BIT_UF_ZONE_MODE_MSK              0x00000001
#define PKT_DINT_REG_ZONE_CFG_REG_BIT_UF_ZONE_MODE_OFF              0

/* index definitions for PMC_PKT_DINT_REG_CFC_XOFF_TH_REG */
#define PMC_PKT_DINT_REG_CFC_XOFF_TH_REG_INDEX_N_MIN            0
#define PMC_PKT_DINT_REG_CFC_XOFF_TH_REG_INDEX_N_MAX            11
#define PMC_PKT_DINT_REG_CFC_XOFF_TH_REG_INDEX_N_SIZE           12
#define PMC_PKT_DINT_REG_CFC_XOFF_TH_REG_INDEX_N_OFFSET         4

/*-------------------------------------------------.
 | Register (0x00040464 + (N) * 4) CFC_XOFF_TH_REG |
 +-------------------------------------------------+
 | bit  3:0 R/W  CFC_XOFF_THRESHOLD                |
 +------------------------------------------------*/
#define PMC_PKT_DINT_REG_CFC_XOFF_TH_REG_UNUSED_MASK            0xfffffff0
#define PKT_DINT_REG_CFC_XOFF_TH_REG_BIT_CFC_XOFF_THRESHOLD_MSK 0x0000000f
#define PKT_DINT_REG_CFC_XOFF_TH_REG_BIT_CFC_XOFF_THRESHOLD_OFF 0

/*--------------------------------------.
 | Register 0x00040494 CFC_FIFO_INT_REG |
 +--------------------------------------+
 | bit  23:12 R/W  CFC_FIFO_UDR_I       |
 | bit  11:0  R/W  CFC_FIFO_OVR_I       |
 +-------------------------------------*/
#define PMC_PKT_DINT_REG_CFC_FIFO_INT_REG_UNUSED_MASK        0xff000000
#define PKT_DINT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_MSK 0x00fff000
#define PKT_DINT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_UDR_I_OFF 12
#define PKT_DINT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_MSK 0x00000fff
#define PKT_DINT_REG_CFC_FIFO_INT_REG_BIT_CFC_FIFO_OVR_I_OFF 0

/*-----------------------------------------.
 | Register 0x00040498 CFC_FIFO_INT_EN_REG |
 +-----------------------------------------+
 | bit  23:12 R/W  CFC_FIFO_UDR_E          |
 | bit  11:0  R/W  CFC_FIFO_OVR_E          |
 +----------------------------------------*/
#define PMC_PKT_DINT_REG_CFC_FIFO_INT_EN_REG_UNUSED_MASK        0xff000000
#define PKT_DINT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_MSK 0x00fff000
#define PKT_DINT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_UDR_E_OFF 12
#define PKT_DINT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_MSK 0x00000fff
#define PKT_DINT_REG_CFC_FIFO_INT_EN_REG_BIT_CFC_FIFO_OVR_E_OFF 0

/*-------------------------------------------.
 | Register 0x0004049c CFC_FIFO_INT_STAT_REG |
 +-------------------------------------------+
 | bit  23:12 R  CFC_FIFO_UDR_V              |
 | bit  11:0  R  CFC_FIFO_OVR_V              |
 +------------------------------------------*/
#define PMC_PKT_DINT_REG_CFC_FIFO_INT_STAT_REG_UNUSED_MASK        0xff000000
#define PKT_DINT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_MSK 0x00fff000
#define PKT_DINT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_UDR_V_OFF 12
#define PKT_DINT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_MSK 0x00000fff
#define PKT_DINT_REG_CFC_FIFO_INT_STAT_REG_BIT_CFC_FIFO_OVR_V_OFF 0

/*---------------------------------------------------.
 | Register 0x000404b0 PACKET_DEINTERLEAVER_SW_RESET |
 +---------------------------------------------------+
 | bit  0 R/W  SOFT_RST                              |
 +--------------------------------------------------*/
#define PMC_PKT_DINT_REG_PACKET_DEINTERLEAVER_SW_RESET_UNUSED_MASK  0xfffffffe
#define PKT_DINT_REG_PACKET_DEINTERLEAVER_SW_RESET_BIT_SOFT_RST_MSK 0x00000001
#define PKT_DINT_REG_PACKET_DEINTERLEAVER_SW_RESET_BIT_SOFT_RST_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PKT_DINT_REGS_H */
