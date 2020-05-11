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
 *     cpb_ctrl block
 *****************************************************************************/
#ifndef _CPB_CTRL_REGS_H
#define _CPB_CTRL_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CPB_CTRL_REG_INTSUM              0x00040110
#define PMC_CPB_CTRL_REG_MASK_DCS_0_REG( N ) (0x00040180 + (N) * 0x4)
#define PMC_CPB_CTRL_REG_MASK_DCS_1_REG( N ) (0x000401a0 + (N) * 0x4)
#define PMC_CPB_CTRL_REG_MASK_DCS_2_REG( N ) (0x000401c0 + (N) * 0x4)
#define PMC_CPB_CTRL_REG_MASK_DCS_3_REG( N ) (0x000401e0 + (N) * 0x4)
#define PMC_CPB_CTRL_REG_MCPB_IMUX           0x00041000
#define PMC_CPB_CTRL_REG_MCPB_EMUX           0x00041004
#define PMC_CPB_CTRL_REG_DCPB_MUX            0x00041014

/*-------------------------------.
 | Register 0x00040110 INTSUM    |
 +-------------------------------+
 | bit  23:16 R  SLV_DPI_INT_SUM |
 +------------------------------*/
#define PMC_CPB_CTRL_REG_INTSUM_UNUSED_MASK         0x0000ff80
#define CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_MSK 0x00ff0000
#define CPB_CTRL_REG_INTSUM_BIT_SLV_DPI_INT_SUM_OFF 16

/* index definitions for PMC_CPB_CTRL_REG_MASK_DCS_0_REG */
#define PMC_CPB_CTRL_REG_MASK_DCS_0_REG_INDEX_N_MIN         0
#define PMC_CPB_CTRL_REG_MASK_DCS_0_REG_INDEX_N_MAX         7
#define PMC_CPB_CTRL_REG_MASK_DCS_0_REG_INDEX_N_SIZE        8
#define PMC_CPB_CTRL_REG_MASK_DCS_0_REG_INDEX_N_OFFSET      0x4

/*--------------------------------------------------.
 | Register (0x00040180 + (N) * 0x4) MASK_DCS_0_REG |
 +--------------------------------------------------+
 | bit  31:0 R/W  XOFF_MASK_DCS_0                   |
 +-------------------------------------------------*/
#define PMC_CPB_CTRL_REG_MASK_DCS_0_REG_UNUSED_MASK         0x00000000
#define CPB_CTRL_REG_MASK_DCS_0_REG_BIT_XOFF_MASK_DCS_0_MSK 0xffffffff
#define CPB_CTRL_REG_MASK_DCS_0_REG_BIT_XOFF_MASK_DCS_0_OFF 0

/* index definitions for PMC_CPB_CTRL_REG_MASK_DCS_1_REG */
#define PMC_CPB_CTRL_REG_MASK_DCS_1_REG_INDEX_N_MIN         0
#define PMC_CPB_CTRL_REG_MASK_DCS_1_REG_INDEX_N_MAX         7
#define PMC_CPB_CTRL_REG_MASK_DCS_1_REG_INDEX_N_SIZE        8
#define PMC_CPB_CTRL_REG_MASK_DCS_1_REG_INDEX_N_OFFSET      0x4

/*--------------------------------------------------.
 | Register (0x000401a0 + (N) * 0x4) MASK_DCS_1_REG |
 +--------------------------------------------------+
 | bit  31:0 R/W  XOFF_MASK_DCS_1                   |
 +-------------------------------------------------*/
#define PMC_CPB_CTRL_REG_MASK_DCS_1_REG_UNUSED_MASK         0x00000000
#define CPB_CTRL_REG_MASK_DCS_1_REG_BIT_XOFF_MASK_DCS_1_MSK 0xffffffff
#define CPB_CTRL_REG_MASK_DCS_1_REG_BIT_XOFF_MASK_DCS_1_OFF 0

/* index definitions for PMC_CPB_CTRL_REG_MASK_DCS_2_REG */
#define PMC_CPB_CTRL_REG_MASK_DCS_2_REG_INDEX_N_MIN         0
#define PMC_CPB_CTRL_REG_MASK_DCS_2_REG_INDEX_N_MAX         7
#define PMC_CPB_CTRL_REG_MASK_DCS_2_REG_INDEX_N_SIZE        8
#define PMC_CPB_CTRL_REG_MASK_DCS_2_REG_INDEX_N_OFFSET      0x4

/*--------------------------------------------------.
 | Register (0x000401c0 + (N) * 0x4) MASK_DCS_2_REG |
 +--------------------------------------------------+
 | bit  31:0 R/W  XOFF_MASK_DCS_2                   |
 +-------------------------------------------------*/
#define PMC_CPB_CTRL_REG_MASK_DCS_2_REG_UNUSED_MASK         0x00000000
#define CPB_CTRL_REG_MASK_DCS_2_REG_BIT_XOFF_MASK_DCS_2_MSK 0xffffffff
#define CPB_CTRL_REG_MASK_DCS_2_REG_BIT_XOFF_MASK_DCS_2_OFF 0

/* index definitions for PMC_CPB_CTRL_REG_MASK_DCS_3_REG */
#define PMC_CPB_CTRL_REG_MASK_DCS_3_REG_INDEX_N_MIN         0
#define PMC_CPB_CTRL_REG_MASK_DCS_3_REG_INDEX_N_MAX         7
#define PMC_CPB_CTRL_REG_MASK_DCS_3_REG_INDEX_N_SIZE        8
#define PMC_CPB_CTRL_REG_MASK_DCS_3_REG_INDEX_N_OFFSET      0x4

/*--------------------------------------------------.
 | Register (0x000401e0 + (N) * 0x4) MASK_DCS_3_REG |
 +--------------------------------------------------+
 | bit  31:0 R/W  XOFF_MASK_DCS_3                   |
 +-------------------------------------------------*/
#define PMC_CPB_CTRL_REG_MASK_DCS_3_REG_UNUSED_MASK         0x00000000
#define CPB_CTRL_REG_MASK_DCS_3_REG_BIT_XOFF_MASK_DCS_3_MSK 0xffffffff
#define CPB_CTRL_REG_MASK_DCS_3_REG_BIT_XOFF_MASK_DCS_3_OFF 0

/*-------------------------------.
 | Register 0x00041000 MCPB_IMUX |
 +-------------------------------+
 | bit  16 R/W  IM_IGR_EXT_SEL   |
 | bit  8  R/W  IM_EGR_EXT_SEL   |
 | bit  0  R/W  IM_REFLECT_SEL   |
 +------------------------------*/
#define PMC_CPB_CTRL_REG_MCPB_IMUX_UNUSED_MASK        0xfffefefe
#define CPB_CTRL_REG_MCPB_IMUX_BIT_IM_IGR_EXT_SEL_MSK 0x00010000
#define CPB_CTRL_REG_MCPB_IMUX_BIT_IM_IGR_EXT_SEL_OFF 16
#define CPB_CTRL_REG_MCPB_IMUX_BIT_IM_EGR_EXT_SEL_MSK 0x00000100
#define CPB_CTRL_REG_MCPB_IMUX_BIT_IM_EGR_EXT_SEL_OFF 8
#define CPB_CTRL_REG_MCPB_IMUX_BIT_IM_REFLECT_SEL_MSK 0x00000001
#define CPB_CTRL_REG_MCPB_IMUX_BIT_IM_REFLECT_SEL_OFF 0

/*-------------------------------.
 | Register 0x00041004 MCPB_EMUX |
 +-------------------------------+
 | bit  16 R/W  EM_EGR_EXT_SEL   |
 | bit  8  R/W  EM_IGR_EXT_SEL   |
 +------------------------------*/
#define PMC_CPB_CTRL_REG_MCPB_EMUX_UNUSED_MASK        0xfffefeff
#define CPB_CTRL_REG_MCPB_EMUX_BIT_EM_EGR_EXT_SEL_MSK 0x00010000
#define CPB_CTRL_REG_MCPB_EMUX_BIT_EM_EGR_EXT_SEL_OFF 16
#define CPB_CTRL_REG_MCPB_EMUX_BIT_EM_IGR_EXT_SEL_MSK 0x00000100
#define CPB_CTRL_REG_MCPB_EMUX_BIT_EM_IGR_EXT_SEL_OFF 8

/*------------------------------.
 | Register 0x00041014 DCPB_MUX |
 +------------------------------+
 | bit  9 R/W  IGR_SIFD2_SEL    |
 | bit  8 R/W  EGR_SIFD2_SEL    |
 | bit  1 R/W  IGR_SIFD1_SEL    |
 | bit  0 R/W  EGR_SIFD1_SEL    |
 +-----------------------------*/
#define PMC_CPB_CTRL_REG_DCPB_MUX_UNUSED_MASK       0xfffffcfc
#define CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD2_SEL_MSK 0x00000200
#define CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD2_SEL_OFF 9
#define CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD2_SEL_MSK 0x00000100
#define CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD2_SEL_OFF 8
#define CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD1_SEL_MSK 0x00000002
#define CPB_CTRL_REG_DCPB_MUX_BIT_IGR_SIFD1_SEL_OFF 1
#define CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD1_SEL_MSK 0x00000001
#define CPB_CTRL_REG_DCPB_MUX_BIT_EGR_SIFD1_SEL_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CPB_CTRL_REGS_H */
