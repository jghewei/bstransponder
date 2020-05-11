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
 *     otudg block
 *****************************************************************************/
#ifndef _OTUDG_REGS_H
#define _OTUDG_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OTUDG_REG_FIFO_CTRL_REG          0x00000400
#define PMC_OTUDG_REG_GEN_CFG_REG( N )       (0x00000404 + (N) * 0x10)
#define PMC_OTUDG_REG_SLICE_DSM_NUM_REG( N ) (0x00000408 + (N) * 0x10)
#define PMC_OTUDG_REG_SLICE_DSM_DEN_REG( N ) (0x0000040c + (N) * 0x10)
#define PMC_OTUDG_REG_V_FIFO_OVR_INT_REG     0x000004f0
#define PMC_OTUDG_REG_V_FIFO_UDR_INT_REG     0x000004f4
#define PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG   0x000004f8
#define PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG  0x000004fc

/*-----------------------------------.
 | Register 0x00000400 FIFO_CTRL_REG |
 +-----------------------------------+
 | bit  23:16 R/W  CENTER_THRES      |
 | bit  7:0   R/W  A_FULL_THRES      |
 +----------------------------------*/
#define PMC_OTUDG_REG_FIFO_CTRL_REG_UNUSED_MASK      0xff00ff00
#define OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_MSK 0x00ff0000
#define OTUDG_REG_FIFO_CTRL_REG_BIT_CENTER_THRES_OFF 16
#define OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_MSK 0x000000ff
#define OTUDG_REG_FIFO_CTRL_REG_BIT_A_FULL_THRES_OFF 0

/* index definitions for PMC_OTUDG_REG_GEN_CFG_REG */
#define PMC_OTUDG_REG_GEN_CFG_REG_INDEX_N_MIN    0
#define PMC_OTUDG_REG_GEN_CFG_REG_INDEX_N_MAX    11
#define PMC_OTUDG_REG_GEN_CFG_REG_INDEX_N_SIZE   12
#define PMC_OTUDG_REG_GEN_CFG_REG_INDEX_N_OFFSET 0x10

/*------------------------------------------------.
 | Register (0x00000404 + (N) * 0x10) GEN_CFG_REG |
 +------------------------------------------------+
 | bit  0 R/W  SLICE_EN                           |
 +-----------------------------------------------*/
#define PMC_OTUDG_REG_GEN_CFG_REG_UNUSED_MASK    0xfffffffe
#define OTUDG_REG_GEN_CFG_REG_BIT_SLICE_EN_MSK   0x00000001
#define OTUDG_REG_GEN_CFG_REG_BIT_SLICE_EN_OFF   0

/* index definitions for PMC_OTUDG_REG_SLICE_DSM_NUM_REG */
#define PMC_OTUDG_REG_SLICE_DSM_NUM_REG_INDEX_N_MIN    0
#define PMC_OTUDG_REG_SLICE_DSM_NUM_REG_INDEX_N_MAX    11
#define PMC_OTUDG_REG_SLICE_DSM_NUM_REG_INDEX_N_SIZE   12
#define PMC_OTUDG_REG_SLICE_DSM_NUM_REG_INDEX_N_OFFSET 0x10

/*------------------------------------------------------.
 | Register (0x00000408 + (N) * 0x10) SLICE_DSM_NUM_REG |
 +------------------------------------------------------+
 | bit  23:0 R/W  DSM_NUM                               |
 +-----------------------------------------------------*/
#define PMC_OTUDG_REG_SLICE_DSM_NUM_REG_UNUSED_MASK    0xff000000
#define OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_MSK    0x00ffffff
#define OTUDG_REG_SLICE_DSM_NUM_REG_BIT_DSM_NUM_OFF    0

/* index definitions for PMC_OTUDG_REG_SLICE_DSM_DEN_REG */
#define PMC_OTUDG_REG_SLICE_DSM_DEN_REG_INDEX_N_MIN    0
#define PMC_OTUDG_REG_SLICE_DSM_DEN_REG_INDEX_N_MAX    11
#define PMC_OTUDG_REG_SLICE_DSM_DEN_REG_INDEX_N_SIZE   12
#define PMC_OTUDG_REG_SLICE_DSM_DEN_REG_INDEX_N_OFFSET 0x10

/*------------------------------------------------------.
 | Register (0x0000040c + (N) * 0x10) SLICE_DSM_DEN_REG |
 +------------------------------------------------------+
 | bit  23:0 R/W  DSM_DEN                               |
 +-----------------------------------------------------*/
#define PMC_OTUDG_REG_SLICE_DSM_DEN_REG_UNUSED_MASK    0xff000000
#define OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_MSK    0x00ffffff
#define OTUDG_REG_SLICE_DSM_DEN_REG_BIT_DSM_DEN_OFF    0

/*----------------------------------------.
 | Register 0x000004f0 V_FIFO_OVR_INT_REG |
 +----------------------------------------+
 | bit  11:0 R/W  V_FIFO_OVR_I            |
 +---------------------------------------*/
#define PMC_OTUDG_REG_V_FIFO_OVR_INT_REG_UNUSED_MASK      0xfffff000
#define OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_MSK 0x00000fff
#define OTUDG_REG_V_FIFO_OVR_INT_REG_BIT_V_FIFO_OVR_I_OFF 0

/*----------------------------------------.
 | Register 0x000004f4 V_FIFO_UDR_INT_REG |
 +----------------------------------------+
 | bit  11:0 R/W  V_FIFO_UDR_I            |
 +---------------------------------------*/
#define PMC_OTUDG_REG_V_FIFO_UDR_INT_REG_UNUSED_MASK      0xfffff000
#define OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_MSK 0x00000fff
#define OTUDG_REG_V_FIFO_UDR_INT_REG_BIT_V_FIFO_UDR_I_OFF 0

/*------------------------------------------.
 | Register 0x000004f8 V_FIFO_OVR_INT_ENREG |
 +------------------------------------------+
 | bit  11:0 R/W  V_FIFO_OVR_E              |
 +-----------------------------------------*/
#define PMC_OTUDG_REG_V_FIFO_OVR_INT_ENREG_UNUSED_MASK      0xfffff000
#define OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_MSK 0x00000fff
#define OTUDG_REG_V_FIFO_OVR_INT_ENREG_BIT_V_FIFO_OVR_E_OFF 0

/*-------------------------------------------.
 | Register 0x000004fc V_FIFO_UDR_INT_EN_REG |
 +-------------------------------------------+
 | bit  11:0 R/W  V_FIFO_UDR_E               |
 +------------------------------------------*/
#define PMC_OTUDG_REG_V_FIFO_UDR_INT_EN_REG_UNUSED_MASK      0xfffff000
#define OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_MSK 0x00000fff
#define OTUDG_REG_V_FIFO_UDR_INT_EN_REG_BIT_V_FIFO_UDR_E_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTUDG_REGS_H */
