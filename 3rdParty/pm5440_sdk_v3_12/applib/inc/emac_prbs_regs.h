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
 *     emac_prbs block
 *****************************************************************************/
#ifndef _EMAC_PRBS_REGS_H
#define _EMAC_PRBS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_PRBS_REG_PRBS_CTRL                0x0000f800
#define PMC_PRBS_REG_LANE_PRBS31_ERR_CNT( N ) (0x0000f804 + (N) * 4)
#define PMC_PRBS_REG_LANE_PRBS9_ERR_CNT( N )  (0x0000f834 + (N) * 4)
#define PMC_PRBS_REG_PRBS_STATUS              0x0000f864

/*-------------------------------.
 | Register 0x0000f800 PRBS_CTRL |
 +-------------------------------+
 | bit  23:12 R/W  PRBS9_ENA     |
 | bit  11:0  R/W  PRBS31_ENA    |
 +------------------------------*/
#define PMC_PRBS_REG_PRBS_CTRL_UNUSED_MASK    0xff000000
#define PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_MSK  0x00fff000
#define PRBS_REG_PRBS_CTRL_BIT_PRBS9_ENA_OFF  12
#define PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_MSK 0x00000fff
#define PRBS_REG_PRBS_CTRL_BIT_PRBS31_ENA_OFF 0

/* index definitions for PMC_PRBS_REG_LANE_PRBS31_ERR_CNT */
#define PMC_PRBS_REG_LANE_PRBS31_ERR_CNT_INDEX_N_MIN        0
#define PMC_PRBS_REG_LANE_PRBS31_ERR_CNT_INDEX_N_MAX        11
#define PMC_PRBS_REG_LANE_PRBS31_ERR_CNT_INDEX_N_SIZE       12
#define PMC_PRBS_REG_LANE_PRBS31_ERR_CNT_INDEX_N_OFFSET     4

/*-----------------------------------------------------.
 | Register (0x0000f804 + (N) * 4) LANE_PRBS31_ERR_CNT |
 +-----------------------------------------------------+
 | bit  15:0 R/W  PRBS31_ERR_CNT                       |
 +----------------------------------------------------*/
#define PMC_PRBS_REG_LANE_PRBS31_ERR_CNT_UNUSED_MASK        0xffff0000
#define PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_MSK 0x0000ffff
#define PRBS_REG_LANE_PRBS31_ERR_CNT_BIT_PRBS31_ERR_CNT_OFF 0

/* index definitions for PMC_PRBS_REG_LANE_PRBS9_ERR_CNT */
#define PMC_PRBS_REG_LANE_PRBS9_ERR_CNT_INDEX_N_MIN       0
#define PMC_PRBS_REG_LANE_PRBS9_ERR_CNT_INDEX_N_MAX       11
#define PMC_PRBS_REG_LANE_PRBS9_ERR_CNT_INDEX_N_SIZE      12
#define PMC_PRBS_REG_LANE_PRBS9_ERR_CNT_INDEX_N_OFFSET    4

/*----------------------------------------------------.
 | Register (0x0000f834 + (N) * 4) LANE_PRBS9_ERR_CNT |
 +----------------------------------------------------+
 | bit  15:0 R/W  PRBS9_ERR_CNT                       |
 +---------------------------------------------------*/
#define PMC_PRBS_REG_LANE_PRBS9_ERR_CNT_UNUSED_MASK       0xffff0000
#define PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_MSK 0x0000ffff
#define PRBS_REG_LANE_PRBS9_ERR_CNT_BIT_PRBS9_ERR_CNT_OFF 0

/*---------------------------------.
 | Register 0x0000f864 PRBS_STATUS |
 +---------------------------------+
 | bit  11:0 R  PRBS_ALL_ONES      |
 +--------------------------------*/
#define PMC_PRBS_REG_PRBS_STATUS_UNUSED_MASK       0xfffff000
#define PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_MSK 0x00000fff
#define PRBS_REG_PRBS_STATUS_BIT_PRBS_ALL_ONES_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EMAC_PRBS_REGS_H */
