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
 *     fecstat block
 *****************************************************************************/
#ifndef _FECSTAT_REGS_H
#define _FECSTAT_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_FECSTAT_REG_FECSTAT_EN                0x00006000
#define PMC_FECSTAT_REG_STAT_SOURCE_EN( N )       (0x00006004 + (N) * 0x20)
#define PMC_FECSTAT_REG_STAT_SOURCE_0_ADR( N )    (0x00006008 + (N) * 0x20)
#define PMC_FECSTAT_REG_STAT_SOURCE_1_ADR( N )    (0x0000600c + (N) * 0x20)
#define PMC_FECSTAT_REG_STAT_SOURCE_2_ADR( N )    (0x00006010 + (N) * 0x20)
#define PMC_FECSTAT_REG_LOGICAL_LANE_ID( N )      (0x00006014 + (N) * 0x20)
#define PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX( N )  (0x00006018 + (N) * 0x20)
#define PMC_FECSTAT_REG_FWI_CLK_DIVIDER           0x00006604
#define PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT 0x00006608
#define PMC_FECSTAT_REG_FEC_ROW_EN_LOWER          0x0000660c
#define PMC_FECSTAT_REG_FEC_ROW_EN_UPPER          0x00006610
#define PMC_FECSTAT_REG_AIR_ERR_INT_EN            0x00006614
#define PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST       0x00006618
#define PMC_FECSTAT_REG_FEC_READ_ERR_VECTOR       0x0000661c
#define PMC_FECSTAT_REG_FWI_WRITE_ERR_VECTOR      0x00006620

/*--------------------------------.
 | Register 0x00006000 FECSTAT_EN |
 +--------------------------------+
 | bit  0 R/W  FECSTAT_EN         |
 +-------------------------------*/
#define PMC_FECSTAT_REG_FECSTAT_EN_UNUSED_MASK    0xfffffffe
#define FECSTAT_REG_FECSTAT_EN_BIT_FECSTAT_EN_MSK 0x00000001
#define FECSTAT_REG_FECSTAT_EN_BIT_FECSTAT_EN_OFF 0

/* index definitions for PMC_FECSTAT_REG_STAT_SOURCE_EN */
#define PMC_FECSTAT_REG_STAT_SOURCE_EN_INDEX_N_MIN        0
#define PMC_FECSTAT_REG_STAT_SOURCE_EN_INDEX_N_MAX        47
#define PMC_FECSTAT_REG_STAT_SOURCE_EN_INDEX_N_SIZE       48
#define PMC_FECSTAT_REG_STAT_SOURCE_EN_INDEX_N_OFFSET     0x20

/*---------------------------------------------------.
 | Register (0x00006004 + (N) * 0x20) STAT_SOURCE_EN |
 +---------------------------------------------------+
 | bit  2:0 R/W  STAT_SOURCE_EN                      |
 +--------------------------------------------------*/
#define PMC_FECSTAT_REG_STAT_SOURCE_EN_UNUSED_MASK        0xfffffff8
#define FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_MSK 0x00000007
#define FECSTAT_REG_STAT_SOURCE_EN_BIT_STAT_SOURCE_EN_OFF 0

/* index definitions for PMC_FECSTAT_REG_STAT_SOURCE_0_ADR */
#define PMC_FECSTAT_REG_STAT_SOURCE_0_ADR_INDEX_N_MIN           0
#define PMC_FECSTAT_REG_STAT_SOURCE_0_ADR_INDEX_N_MAX           47
#define PMC_FECSTAT_REG_STAT_SOURCE_0_ADR_INDEX_N_SIZE          48
#define PMC_FECSTAT_REG_STAT_SOURCE_0_ADR_INDEX_N_OFFSET        0x20

/*------------------------------------------------------.
 | Register (0x00006008 + (N) * 0x20) STAT_SOURCE_0_ADR |
 +------------------------------------------------------+
 | bit  4:0 R/W  STAT_SOURCE0_ADDR                      |
 +-----------------------------------------------------*/
#define PMC_FECSTAT_REG_STAT_SOURCE_0_ADR_UNUSED_MASK           0xffffffe0
#define FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_MSK 0x0000001f
#define FECSTAT_REG_STAT_SOURCE_0_ADR_BIT_STAT_SOURCE0_ADDR_OFF 0

/* index definitions for PMC_FECSTAT_REG_STAT_SOURCE_1_ADR */
#define PMC_FECSTAT_REG_STAT_SOURCE_1_ADR_INDEX_N_MIN           0
#define PMC_FECSTAT_REG_STAT_SOURCE_1_ADR_INDEX_N_MAX           47
#define PMC_FECSTAT_REG_STAT_SOURCE_1_ADR_INDEX_N_SIZE          48
#define PMC_FECSTAT_REG_STAT_SOURCE_1_ADR_INDEX_N_OFFSET        0x20

/*------------------------------------------------------.
 | Register (0x0000600c + (N) * 0x20) STAT_SOURCE_1_ADR |
 +------------------------------------------------------+
 | bit  4:0 R/W  STAT_SOURCE1_ADDR                      |
 +-----------------------------------------------------*/
#define PMC_FECSTAT_REG_STAT_SOURCE_1_ADR_UNUSED_MASK           0xffffffe0
#define FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_MSK 0x0000001f
#define FECSTAT_REG_STAT_SOURCE_1_ADR_BIT_STAT_SOURCE1_ADDR_OFF 0

/* index definitions for PMC_FECSTAT_REG_STAT_SOURCE_2_ADR */
#define PMC_FECSTAT_REG_STAT_SOURCE_2_ADR_INDEX_N_MIN           0
#define PMC_FECSTAT_REG_STAT_SOURCE_2_ADR_INDEX_N_MAX           47
#define PMC_FECSTAT_REG_STAT_SOURCE_2_ADR_INDEX_N_SIZE          48
#define PMC_FECSTAT_REG_STAT_SOURCE_2_ADR_INDEX_N_OFFSET        0x20

/*------------------------------------------------------.
 | Register (0x00006010 + (N) * 0x20) STAT_SOURCE_2_ADR |
 +------------------------------------------------------+
 | bit  4:0 R/W  STAT_SOURCE2_ADDR                      |
 +-----------------------------------------------------*/
#define PMC_FECSTAT_REG_STAT_SOURCE_2_ADR_UNUSED_MASK           0xffffffe0
#define FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_MSK 0x0000001f
#define FECSTAT_REG_STAT_SOURCE_2_ADR_BIT_STAT_SOURCE2_ADDR_OFF 0

/* index definitions for PMC_FECSTAT_REG_LOGICAL_LANE_ID */
#define PMC_FECSTAT_REG_LOGICAL_LANE_ID_INDEX_N_MIN    0
#define PMC_FECSTAT_REG_LOGICAL_LANE_ID_INDEX_N_MAX    47
#define PMC_FECSTAT_REG_LOGICAL_LANE_ID_INDEX_N_SIZE   48
#define PMC_FECSTAT_REG_LOGICAL_LANE_ID_INDEX_N_OFFSET 0x20

/*----------------------------------------------------.
 | Register (0x00006014 + (N) * 0x20) LOGICAL_LANE_ID |
 +----------------------------------------------------+
 | bit  7:0 R/W  LLID                                 |
 +---------------------------------------------------*/
#define PMC_FECSTAT_REG_LOGICAL_LANE_ID_UNUSED_MASK    0xffffff00
#define FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_MSK       0x000000ff
#define FECSTAT_REG_LOGICAL_LANE_ID_BIT_LLID_OFF       0

/* index definitions for PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX */
#define PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX_INDEX_N_MIN    0
#define PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX_INDEX_N_MAX    47
#define PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX_INDEX_N_SIZE   48
#define PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX_INDEX_N_OFFSET 0x20

/*--------------------------------------------------------.
 | Register (0x00006018 + (N) * 0x20) PHYSICAL_LANE_INDEX |
 +--------------------------------------------------------+
 | bit  7:0 R/W  PHY_LN                                   |
 +-------------------------------------------------------*/
#define PMC_FECSTAT_REG_PHYSICAL_LANE_INDEX_UNUSED_MASK    0xffffff00
#define FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_MSK     0x000000ff
#define FECSTAT_REG_PHYSICAL_LANE_INDEX_BIT_PHY_LN_OFF     0

/*-------------------------------------.
 | Register 0x00006604 FWI_CLK_DIVIDER |
 +-------------------------------------+
 | bit  4 R/W  FWI_INV_GL_CLK          |
 +------------------------------------*/
#define PMC_FECSTAT_REG_FWI_CLK_DIVIDER_UNUSED_MASK        0xffffffe0
#define FECSTAT_REG_FWI_CLK_DIVIDER_BIT_FWI_INV_GL_CLK_MSK 0x00000010
#define FECSTAT_REG_FWI_CLK_DIVIDER_BIT_FWI_INV_GL_CLK_OFF 4

/*-----------------------------------------------.
 | Register 0x00006608 FEC_COLLECTION_PERIOD_CNT |
 +-----------------------------------------------+
 | bit  27:0 R/W  FEC_COLLCT_CNT                 |
 +----------------------------------------------*/
#define PMC_FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_UNUSED_MASK        0xf0000000
#define FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_MSK 0x0fffffff
#define FECSTAT_REG_FEC_COLLECTION_PERIOD_CNT_BIT_FEC_COLLCT_CNT_OFF 0

/*--------------------------------------.
 | Register 0x0000660c FEC_ROW_EN_LOWER |
 +--------------------------------------+
 | bit  31:0 R/W  FEC_ROW_EN            |
 +-------------------------------------*/
#define PMC_FECSTAT_REG_FEC_ROW_EN_LOWER_UNUSED_MASK    0x00000000
#define FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_MSK 0xffffffff
#define FECSTAT_REG_FEC_ROW_EN_LOWER_BIT_FEC_ROW_EN_OFF 0

/*--------------------------------------.
 | Register 0x00006610 FEC_ROW_EN_UPPER |
 +--------------------------------------+
 | bit  15:0 R/W  FEC_ROW_EN            |
 +-------------------------------------*/
#define PMC_FECSTAT_REG_FEC_ROW_EN_UPPER_UNUSED_MASK    0xffff0000
#define FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_MSK 0x0000ffff
#define FECSTAT_REG_FEC_ROW_EN_UPPER_BIT_FEC_ROW_EN_OFF 0

/*------------------------------------.
 | Register 0x00006614 AIR_ERR_INT_EN |
 +------------------------------------+
 | bit  2:0 R/W  AIR_ERR_INT_E        |
 +-----------------------------------*/
#define PMC_FECSTAT_REG_AIR_ERR_INT_EN_UNUSED_MASK       0xfffffff8
#define FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_MSK 0x00000007
#define FECSTAT_REG_AIR_ERR_INT_EN_BIT_AIR_ERR_INT_E_OFF 0

/*-----------------------------------------.
 | Register 0x00006618 AIR_ERR_INT_REQUEST |
 +-----------------------------------------+
 | bit  2:0 R/W  AIR_ERR_INT_I             |
 +----------------------------------------*/
#define PMC_FECSTAT_REG_AIR_ERR_INT_REQUEST_UNUSED_MASK       0xfffffff8
#define FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_MSK 0x00000007
#define FECSTAT_REG_AIR_ERR_INT_REQUEST_BIT_AIR_ERR_INT_I_OFF 0

/*-----------------------------------------.
 | Register 0x0000661c FEC_READ_ERR_VECTOR |
 +-----------------------------------------+
 | bit  15:0 R  FEC_RD_ERR_VECTOR          |
 +----------------------------------------*/
#define PMC_FECSTAT_REG_FEC_READ_ERR_VECTOR_UNUSED_MASK           0xffff0000
#define FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_MSK 0x0000ffff
#define FECSTAT_REG_FEC_READ_ERR_VECTOR_BIT_FEC_RD_ERR_VECTOR_OFF 0

/*------------------------------------------.
 | Register 0x00006620 FWI_WRITE_ERR_VECTOR |
 +------------------------------------------+
 | bit  15:0 R  FWI_WR_ERR_VECTOR           |
 +-----------------------------------------*/
#define PMC_FECSTAT_REG_FWI_WRITE_ERR_VECTOR_UNUSED_MASK           0xffff0000
#define FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_MSK 0x0000ffff
#define FECSTAT_REG_FWI_WRITE_ERR_VECTOR_BIT_FWI_WR_ERR_VECTOR_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FECSTAT_REGS_H */
