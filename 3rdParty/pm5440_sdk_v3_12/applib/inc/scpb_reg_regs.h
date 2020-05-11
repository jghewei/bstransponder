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
 *     scpb_reg block
 *****************************************************************************/
#ifndef _SCPB_REG_REGS_H
#define _SCPB_REG_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG( N,M )    (0x00080000 + (N) * 0x100 + (M) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL( N ) (0x00080400 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST( N )     (0x00080800 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL( N ) (0x00080c00 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST( N )     (0x00081000 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL( N ) (0x00081400 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST( N )     (0x00081800 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL( N ) (0x00081c00 + (N) * 0x4)
#define PMC_SCPB_REG_REG_POP_PORT_RST( N )         (0x00082000 + (N) * 0x4)
#define PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG( N ) (0x00082400 + (N) * 0x4)
#define PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG( N )   (0x00082600 + (N) * 0x4)
#define PMC_SCPB_REG_REG_FIFO_INT_REG              0x00082800
#define PMC_SCPB_REG_REG_FIFO_INT_EN_REG           0x00082804
#define PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0( N )  (0x00083000 + (N) * 0x4)
#define PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG( N ) (0x00083100 + (N) * 0x4)
#define PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG( N ) (0x00083200 + (N) * 0x4)
#define PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG( N ) (0x00083400 + (N) * 0x4)
#define PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG( N ) (0x00083500 + (N) * 0x4)
#define PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG       0x00083808
#define PMC_SCPB_REG_REG_FIFO_CFG                  0x00083d00
#define PMC_SCPB_REG_REG_FIFO_CFG_RID              0x00083d04
#define PMC_SCPB_REG_REG_FIFO_CFG_WID              0x00083d08
#define PMC_SCPB_REG_REG_FIFO_CFG_STAT             0x00083d0c
#define PMC_SCPB_REG_REG_DBG_CTRL                  0x00083e00
#define PMC_SCPB_REG_REG_DBG_RDATA                 0x00083e04
#define PMC_SCPB_REG_REG_PAGE_TABLE_REG( N )       (0x00084000 + (N) * 0x4)
#define PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG      0x00084100
#define PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG     0x00084104
#define PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG     0x00084108
#define PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG   0x00084110
#define PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG  0x00084114
#define PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG  0x00084118
#define PMC_SCPB_REG_REG_GPI( N )                  (0x00084120 + (N) * 0x4)
#define PMC_SCPB_REG_REG_GPO( N )                  (0x00084140 + (N) * 0x4)
#define PMC_SCPB_REG_REG_REMAP_REQ                 0x00084200
#define PMC_SCPB_REG_REG_REMAP_OLD_ADDR            0x00084204
#define PMC_SCPB_REG_REG_REMAP_NEW_ADDR            0x00084208
#define PMC_SCPB_REG_REG_REMAP_STAT                0x0008420c
#define PMC_SCPB_REG_REG_REMAP_MARGIN              0x00084210
#define PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG         0x00084214
#define PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG        0x00084218

/* index definitions for PMC_SCPB_REG_REG_BLOCK_TABLE_REG */
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_N_MIN          0
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_N_MAX          149
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_N_SIZE         150
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_N_OFFSET       0x100
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_M_MIN          0
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_M_MAX          63
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_M_SIZE         64
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_INDEX_M_OFFSET       0x4

/*-----------------------------------------------------------------.
 | Register (0x00080000 + (N) * 0x100 + (M) * 0x4) BLOCK_TABLE_REG |
 +-----------------------------------------------------------------+
 | bit  31    R/W  IPT_BLOCK_VALID                                 |
 | bit  30    R    IPT_BLOCK_ACTIVE                                |
 | bit  23:16 R/W  IPT_WQID                                        |
 | bit  0:10  R/W  IPT_LBA                                         |
 | bit  0     R/W  WPTR_RST_PUSH_P0                                |
 +----------------------------------------------------------------*/
#define PMC_SCPB_REG_REG_BLOCK_TABLE_REG_UNUSED_MASK          0x3f00f800
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_VALID_MSK  0x80000000
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_VALID_OFF  31
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_ACTIVE_MSK 0x40000000
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_BLOCK_ACTIVE_OFF 30
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_MSK         0x00ff0000
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_WQID_OFF         16
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_MSK          0x000007ff
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_IPT_LBA_OFF          0
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_WPTR_RST_PUSH_P0_MSK 0x00000001
#define SCPB_REG_REG_BLOCK_TABLE_REG_BIT_WPTR_RST_PUSH_P0_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL */
#define PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_INDEX_N_MIN              0
#define PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_INDEX_N_MAX              7
#define PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_INDEX_N_SIZE             8
#define PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_INDEX_N_OFFSET           0x4

/*--------------------------------------------------------.
 | Register (0x00080400 + (N) * 0x4) PUSH_PORT_P0_RST_VAL |
 +--------------------------------------------------------+
 | bit  31:0 R  WPTR_RST_PUSH_P0_VAL                      |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_UNUSED_MASK              0x00000000
#define SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_MSK 0xffffffff
#define SCPB_REG_REG_PUSH_PORT_P0_RST_VAL_BIT_WPTR_RST_PUSH_P0_VAL_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P1_RST */
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_INDEX_N_MIN          0
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_INDEX_N_MAX          149
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_INDEX_N_SIZE         150
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_INDEX_N_OFFSET       0x4

/*----------------------------------------------------.
 | Register (0x00080800 + (N) * 0x4) PUSH_PORT_P1_RST |
 +----------------------------------------------------+
 | bit  0 R/W  WPTR_RST_PUSH_P1                       |
 +---------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_UNUSED_MASK          0xfffffffe
#define SCPB_REG_REG_PUSH_PORT_P1_RST_BIT_WPTR_RST_PUSH_P1_MSK 0x00000001
#define SCPB_REG_REG_PUSH_PORT_P1_RST_BIT_WPTR_RST_PUSH_P1_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL */
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_INDEX_N_MIN              0
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_INDEX_N_MAX              7
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_INDEX_N_SIZE             8
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_INDEX_N_OFFSET           0x4

/*--------------------------------------------------------.
 | Register (0x00080c00 + (N) * 0x4) PUSH_PORT_P1_RST_VAL |
 +--------------------------------------------------------+
 | bit  31:0 R  WPTR_RST_PUSH_P1_VAL                      |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_UNUSED_MASK              0x00000000
#define SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_MSK 0xffffffff
#define SCPB_REG_REG_PUSH_PORT_P1_RST_VAL_BIT_WPTR_RST_PUSH_P1_VAL_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P2_RST */
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_INDEX_N_MIN          0
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_INDEX_N_MAX          119
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_INDEX_N_SIZE         120
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_INDEX_N_OFFSET       0x4

/*----------------------------------------------------.
 | Register (0x00081000 + (N) * 0x4) PUSH_PORT_P2_RST |
 +----------------------------------------------------+
 | bit  0 R/W  WPTR_RST_PUSH_P2                       |
 +---------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_UNUSED_MASK          0xfffffffe
#define SCPB_REG_REG_PUSH_PORT_P2_RST_BIT_WPTR_RST_PUSH_P2_MSK 0x00000001
#define SCPB_REG_REG_PUSH_PORT_P2_RST_BIT_WPTR_RST_PUSH_P2_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL */
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_INDEX_N_MIN              0
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_INDEX_N_MAX              7
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_INDEX_N_SIZE             8
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_INDEX_N_OFFSET           0x4

/*--------------------------------------------------------.
 | Register (0x00081400 + (N) * 0x4) PUSH_PORT_P2_RST_VAL |
 +--------------------------------------------------------+
 | bit  31:0 R  WPTR_RST_PUSH_P2_VAL                      |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_UNUSED_MASK              0x00000000
#define SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_MSK 0xffffffff
#define SCPB_REG_REG_PUSH_PORT_P2_RST_VAL_BIT_WPTR_RST_PUSH_P2_VAL_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P3_RST */
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_INDEX_N_MIN          0
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_INDEX_N_MAX          107
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_INDEX_N_SIZE         108
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_INDEX_N_OFFSET       0x4

/*----------------------------------------------------.
 | Register (0x00081800 + (N) * 0x4) PUSH_PORT_P3_RST |
 +----------------------------------------------------+
 | bit  0 R/W  WPTR_RST_PUSH_P3                       |
 +---------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_UNUSED_MASK          0xfffffffe
#define SCPB_REG_REG_PUSH_PORT_P3_RST_BIT_WPTR_RST_PUSH_P3_MSK 0x00000001
#define SCPB_REG_REG_PUSH_PORT_P3_RST_BIT_WPTR_RST_PUSH_P3_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL */
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_INDEX_N_MIN              0
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_INDEX_N_MAX              7
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_INDEX_N_SIZE             8
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_INDEX_N_OFFSET           0x4

/*--------------------------------------------------------.
 | Register (0x00081c00 + (N) * 0x4) PUSH_PORT_P3_RST_VAL |
 +--------------------------------------------------------+
 | bit  31:0 R  WPTR_RST_PUSH_P3_VAL                      |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_UNUSED_MASK              0x00000000
#define SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_MSK 0xffffffff
#define SCPB_REG_REG_PUSH_PORT_P3_RST_VAL_BIT_WPTR_RST_PUSH_P3_VAL_OFF 0

/* index definitions for PMC_SCPB_REG_REG_POP_PORT_RST */
#define PMC_SCPB_REG_REG_POP_PORT_RST_INDEX_N_MIN      0
#define PMC_SCPB_REG_REG_POP_PORT_RST_INDEX_N_MAX      95
#define PMC_SCPB_REG_REG_POP_PORT_RST_INDEX_N_SIZE     96
#define PMC_SCPB_REG_REG_POP_PORT_RST_INDEX_N_OFFSET   0x4

/*------------------------------------------------.
 | Register (0x00082000 + (N) * 0x4) POP_PORT_RST |
 +------------------------------------------------+
 | bit  0 R/W  RPTR_RST_POP                       |
 +-----------------------------------------------*/
#define PMC_SCPB_REG_REG_POP_PORT_RST_UNUSED_MASK      0xfffffffe
#define SCPB_REG_REG_POP_PORT_RST_BIT_RPTR_RST_POP_MSK 0x00000001
#define SCPB_REG_REG_POP_PORT_RST_BIT_RPTR_RST_POP_OFF 0

/* index definitions for PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG */
#define PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG_INDEX_N_MIN          0
#define PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG_INDEX_N_MAX          7
#define PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG_INDEX_N_SIZE         8
#define PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG_INDEX_N_OFFSET       0x4

/*--------------------------------------------------------.
 | Register (0x00082400 + (N) * 0x4) RPTR_RST_POP_VAL_REG |
 +--------------------------------------------------------+
 | bit  31:0 R  RPTR_RST_POP_VAL                          |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_RPTR_RST_POP_VAL_REG_UNUSED_MASK          0x00000000
#define SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_MSK 0xffffffff
#define SCPB_REG_REG_RPTR_RST_POP_VAL_REG_BIT_RPTR_RST_POP_VAL_OFF 0

/* index definitions for PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG */
#define PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG_INDEX_N_MIN        0
#define PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG_INDEX_N_MAX        7
#define PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG_INDEX_N_SIZE       8
#define PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG_INDEX_N_OFFSET     0x4

/*------------------------------------------------------.
 | Register (0x00082600 + (N) * 0x4) FIFO_PTR_EMPTY_REG |
 +------------------------------------------------------+
 | bit  31:0 R  FIFO_PKT_EMPTY                          |
 +-----------------------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_PTR_EMPTY_REG_UNUSED_MASK        0x00000000
#define SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_MSK 0xffffffff
#define SCPB_REG_REG_FIFO_PTR_EMPTY_REG_BIT_FIFO_PKT_EMPTY_OFF 0

/*----------------------------------.
 | Register 0x00082800 FIFO_INT_REG |
 +----------------------------------+
 | bit  8   R/W  FIFO_UDR_I         |
 | bit  3:0 R/W  FIFO_OVR_PUSH_I    |
 +---------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_INT_REG_UNUSED_MASK         0xfffffef0
#define SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_MSK      0x00000100
#define SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_UDR_I_OFF      8
#define SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_MSK 0x0000000f
#define SCPB_REG_REG_FIFO_INT_REG_BIT_FIFO_OVR_PUSH_I_OFF 0

/*-------------------------------------.
 | Register 0x00082804 FIFO_INT_EN_REG |
 +-------------------------------------+
 | bit  8   R/W  FIFO_UDR_E            |
 | bit  3:0 R/W  FIFO_OVR_PUSH_E       |
 +------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_INT_EN_REG_UNUSED_MASK         0xfffffef0
#define SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_UDR_E_MSK      0x00000100
#define SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_UDR_E_OFF      8
#define SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_MSK 0x0000000f
#define SCPB_REG_REG_FIFO_INT_EN_REG_BIT_FIFO_OVR_PUSH_E_OFF 0

/* index definitions for PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0 */
#define PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0_INDEX_N_MIN         0
#define PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0_INDEX_N_MAX         7
#define PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0_INDEX_N_SIZE        8
#define PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0_INDEX_N_OFFSET      0x4

/*-------------------------------------------------------.
 | Register (0x00083000 + (N) * 0x4) FIFO_UDR_STAT_REG_0 |
 +-------------------------------------------------------+
 | bit  31:0 R  FIFO_UDR_STATUS                          |
 +------------------------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_UDR_STAT_REG_0_UNUSED_MASK         0x00000000
#define SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_MSK 0xffffffff
#define SCPB_REG_REG_FIFO_UDR_STAT_REG_0_BIT_FIFO_UDR_STATUS_OFF 0

/* index definitions for PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG */
#define PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_INDEX_N_MIN            0
#define PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_INDEX_N_MAX            7
#define PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_INDEX_N_SIZE           8
#define PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00083100 + (N) * 0x4) P0_FIFO_OVR_STAT_REG |
 +--------------------------------------------------------+
 | bit  31:0 R  P0_FIFO_OVR_STATUS                        |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_UNUSED_MASK            0x00000000
#define SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_MSK 0xffffffff
#define SCPB_REG_REG_P0_FIFO_OVR_STAT_REG_BIT_P0_FIFO_OVR_STATUS_OFF 0

/* index definitions for PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG */
#define PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_INDEX_N_MIN            0
#define PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_INDEX_N_MAX            7
#define PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_INDEX_N_SIZE           8
#define PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00083200 + (N) * 0x4) P1_FIFO_OVR_STAT_REG |
 +--------------------------------------------------------+
 | bit  31:0 R  P1_FIFO_OVR_STATUS                        |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_UNUSED_MASK            0x00000000
#define SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_MSK 0xffffffff
#define SCPB_REG_REG_P1_FIFO_OVR_STAT_REG_BIT_P1_FIFO_OVR_STATUS_OFF 0

/* index definitions for PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG */
#define PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_INDEX_N_MIN            0
#define PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_INDEX_N_MAX            7
#define PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_INDEX_N_SIZE           8
#define PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00083400 + (N) * 0x4) P2_FIFO_OVR_STAT_REG |
 +--------------------------------------------------------+
 | bit  31:0 R  P2_FIFO_OVR_STATUS                        |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_UNUSED_MASK            0x00000000
#define SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_MSK 0xffffffff
#define SCPB_REG_REG_P2_FIFO_OVR_STAT_REG_BIT_P2_FIFO_OVR_STATUS_OFF 0

/* index definitions for PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG */
#define PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_INDEX_N_MIN            0
#define PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_INDEX_N_MAX            7
#define PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_INDEX_N_SIZE           8
#define PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00083500 + (N) * 0x4) P3_FIFO_OVR_STAT_REG |
 +--------------------------------------------------------+
 | bit  31:0 R  P3_FIFO_OVR_STATUS                        |
 +-------------------------------------------------------*/
#define PMC_SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_UNUSED_MASK            0x00000000
#define SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_MSK 0xffffffff
#define SCPB_REG_REG_P3_FIFO_OVR_STAT_REG_BIT_P3_FIFO_OVR_STATUS_OFF 0

/*-----------------------------------------.
 | Register 0x00083808 FIFO_IPT_BYPASS_REG |
 +-----------------------------------------+
 | bit  0 R/W  IPT_BYPASS                  |
 +----------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_IPT_BYPASS_REG_UNUSED_MASK    0xfffffffe
#define SCPB_REG_REG_FIFO_IPT_BYPASS_REG_BIT_IPT_BYPASS_MSK 0x00000001
#define SCPB_REG_REG_FIFO_IPT_BYPASS_REG_BIT_IPT_BYPASS_OFF 0

/*----------------------------------------.
 | Register 0x00083d00 FIFO_CFG           |
 +----------------------------------------+
 | bit  28   R/W  FIFO_END_ADDR_CHG_ABORT |
 | bit  24   R/W  FIFO_END_ADDR_CHG_REQ   |
 | bit  16:0 R/W  FIFO_END_ADDR           |
 +---------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_CFG_UNUSED_MASK                 0xeefe0000
#define SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_MSK 0x10000000
#define SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_ABORT_OFF 28
#define SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_MSK   0x01000000
#define SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_CHG_REQ_OFF   24
#define SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_MSK           0x0001ffff
#define SCPB_REG_REG_FIFO_CFG_BIT_FIFO_END_ADDR_OFF           0

/*-------------------------------------.
 | Register 0x00083d04 FIFO_CFG_RID    |
 +-------------------------------------+
 | bit  7:0 R/W  FIFO_END_ADDR_CHG_RID |
 +------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_CFG_RID_UNUSED_MASK               0xffffff00
#define SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_MSK 0x000000ff
#define SCPB_REG_REG_FIFO_CFG_RID_BIT_FIFO_END_ADDR_CHG_RID_OFF 0

/*-------------------------------------.
 | Register 0x00083d08 FIFO_CFG_WID    |
 +-------------------------------------+
 | bit  9:0 R/W  FIFO_END_ADDR_CHG_WID |
 +------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_CFG_WID_UNUSED_MASK               0xfffffc00
#define SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_MSK 0x000003ff
#define SCPB_REG_REG_FIFO_CFG_WID_BIT_FIFO_END_ADDR_CHG_WID_OFF 0

/*-------------------------------------.
 | Register 0x00083d0c FIFO_CFG_STAT   |
 +-------------------------------------+
 | bit  0 R  FIFO_END_ADDR_CHG_SUCCESS |
 +------------------------------------*/
#define PMC_SCPB_REG_REG_FIFO_CFG_STAT_UNUSED_MASK                   0xfffffffe
#define SCPB_REG_REG_FIFO_CFG_STAT_BIT_FIFO_END_ADDR_CHG_SUCCESS_MSK 0x00000001
#define SCPB_REG_REG_FIFO_CFG_STAT_BIT_FIFO_END_ADDR_CHG_SUCCESS_OFF 0

/*------------------------------.
 | Register 0x00083e00 DBG_CTRL |
 +------------------------------+
 | bit  16   R/W  DBG_TRIG      |
 | bit  10:0 R/W  DBG_RADDR     |
 +-----------------------------*/
#define PMC_SCPB_REG_REG_DBG_CTRL_UNUSED_MASK   0xfffef800
#define SCPB_REG_REG_DBG_CTRL_BIT_DBG_TRIG_MSK  0x00010000
#define SCPB_REG_REG_DBG_CTRL_BIT_DBG_TRIG_OFF  16
#define SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_MSK 0x000007ff
#define SCPB_REG_REG_DBG_CTRL_BIT_DBG_RADDR_OFF 0

/*-------------------------------.
 | Register 0x00083e04 DBG_RDATA |
 +-------------------------------+
 | bit  31:0 R  DBG_RDATA        |
 +------------------------------*/
#define PMC_SCPB_REG_REG_DBG_RDATA_UNUSED_MASK   0x00000000
#define SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_MSK 0xffffffff
#define SCPB_REG_REG_DBG_RDATA_BIT_DBG_RDATA_OFF 0

/* index definitions for PMC_SCPB_REG_REG_PAGE_TABLE_REG */
#define PMC_SCPB_REG_REG_PAGE_TABLE_REG_INDEX_N_MIN    0
#define PMC_SCPB_REG_REG_PAGE_TABLE_REG_INDEX_N_MAX    31
#define PMC_SCPB_REG_REG_PAGE_TABLE_REG_INDEX_N_SIZE   32
#define PMC_SCPB_REG_REG_PAGE_TABLE_REG_INDEX_N_OFFSET 0x4

/*--------------------------------------------------.
 | Register (0x00084000 + (N) * 0x4) PAGE_TABLE_REG |
 +--------------------------------------------------+
 | bit  24    R/W  PAGE_VALID                       |
 | bit  17:16 R/W  IPT_QS_ID                        |
 | bit  9:8   R/W  IPT_WP_ID                        |
 +-------------------------------------------------*/
#define PMC_SCPB_REG_REG_PAGE_TABLE_REG_UNUSED_MASK    0xfefcfcff
#define SCPB_REG_REG_PAGE_TABLE_REG_BIT_PAGE_VALID_MSK 0x01000000
#define SCPB_REG_REG_PAGE_TABLE_REG_BIT_PAGE_VALID_OFF 24
#define SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_MSK  0x00030000
#define SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_QS_ID_OFF  16
#define SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_MSK  0x00000300
#define SCPB_REG_REG_PAGE_TABLE_REG_BIT_IPT_WP_ID_OFF  8

/*------------------------------------------.
 | Register 0x00084100 PAGE_IPT_ERR_INT_REG |
 +------------------------------------------+
 | bit  0 R/W  PAGE_IPT_ERR_I               |
 +-----------------------------------------*/
#define PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_UNUSED_MASK        0xfffffffe
#define SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_MSK 0x00000001
#define SCPB_REG_REG_PAGE_IPT_ERR_INT_REG_BIT_PAGE_IPT_ERR_I_OFF 0

/*-------------------------------------------.
 | Register 0x00084104 BLOCK_IPT_ERR_INT_REG |
 +-------------------------------------------+
 | bit  31:0 R/W  BLOCK_IPT_ERR_I            |
 +------------------------------------------*/
#define PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_UNUSED_MASK         0x00000000
#define SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_MSK 0xffffffff
#define SCPB_REG_REG_BLOCK_IPT_ERR_INT_REG_BIT_BLOCK_IPT_ERR_I_OFF 0

/*-------------------------------------------.
 | Register 0x00084108 PREFETCH_FIFO_INT_REG |
 +-------------------------------------------+
 | bit  19:16 R/W  HOQ_DLY_OVERFLOW_I        |
 | bit  11:8  R/W  HOQ_CFC_UNDERFLOW_I       |
 | bit  3:0   R/W  HOQ_CFC_OVERFLOW_I        |
 +------------------------------------------*/
#define PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_REG_UNUSED_MASK             0xfff0f0f0
#define SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_MSK  0x000f0000
#define SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_DLY_OVERFLOW_I_OFF  16
#define SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_MSK 0x00000f00
#define SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_UNDERFLOW_I_OFF 8
#define SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_MSK  0x0000000f
#define SCPB_REG_REG_PREFETCH_FIFO_INT_REG_BIT_HOQ_CFC_OVERFLOW_I_OFF  0

/*---------------------------------------------.
 | Register 0x00084110 PAGE_IPT_ERR_INT_EN_REG |
 +---------------------------------------------+
 | bit  0 R/W  PAGE_IPT_ERR_E                  |
 +--------------------------------------------*/
#define PMC_SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_UNUSED_MASK        0xfffffffe
#define SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_BIT_PAGE_IPT_ERR_E_MSK 0x00000001
#define SCPB_REG_REG_PAGE_IPT_ERR_INT_EN_REG_BIT_PAGE_IPT_ERR_E_OFF 0

/*----------------------------------------------.
 | Register 0x00084114 BLOCK_IPT_ERR_INT_EN_REG |
 +----------------------------------------------+
 | bit  31:0 R/W  BLOCK_IPT_ERR_E               |
 +---------------------------------------------*/
#define PMC_SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_UNUSED_MASK         0x00000000
#define SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_MSK 0xffffffff
#define SCPB_REG_REG_BLOCK_IPT_ERR_INT_EN_REG_BIT_BLOCK_IPT_ERR_E_OFF 0

/*----------------------------------------------.
 | Register 0x00084118 PREFETCH_FIFO_INT_EN_REG |
 +----------------------------------------------+
 | bit  19:16 R/W  HOQ_DLY_OVERFLOW_E           |
 | bit  11:8  R/W  HOQ_CFC_UNDERFLOW_E          |
 | bit  3:0   R/W  HOQ_CFC_OVERFLOW_E           |
 +---------------------------------------------*/
#define PMC_SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_UNUSED_MASK             0xfff0f0f0
#define SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_MSK  0x000f0000
#define SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_DLY_OVERFLOW_E_OFF  16
#define SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_MSK 0x00000f00
#define SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_UNDERFLOW_E_OFF 8
#define SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_MSK  0x0000000f
#define SCPB_REG_REG_PREFETCH_FIFO_INT_EN_REG_BIT_HOQ_CFC_OVERFLOW_E_OFF  0

/* index definitions for PMC_SCPB_REG_REG_GPI */
#define PMC_SCPB_REG_REG_GPI_INDEX_N_MIN    0
#define PMC_SCPB_REG_REG_GPI_INDEX_N_MAX    3
#define PMC_SCPB_REG_REG_GPI_INDEX_N_SIZE   4
#define PMC_SCPB_REG_REG_GPI_INDEX_N_OFFSET 0x4

/*---------------------------------------.
 | Register (0x00084120 + (N) * 0x4) GPI |
 +---------------------------------------+
 | bit  31:0 R  GPI                      |
 +--------------------------------------*/
#define PMC_SCPB_REG_REG_GPI_UNUSED_MASK    0x00000000
#define SCPB_REG_REG_GPI_BIT_GPI_MSK        0xffffffff
#define SCPB_REG_REG_GPI_BIT_GPI_OFF        0

/* index definitions for PMC_SCPB_REG_REG_GPO */
#define PMC_SCPB_REG_REG_GPO_INDEX_N_MIN    0
#define PMC_SCPB_REG_REG_GPO_INDEX_N_MAX    3
#define PMC_SCPB_REG_REG_GPO_INDEX_N_SIZE   4
#define PMC_SCPB_REG_REG_GPO_INDEX_N_OFFSET 0x4

/*---------------------------------------.
 | Register (0x00084140 + (N) * 0x4) GPO |
 +---------------------------------------+
 | bit  31:0 R/W  GPO                    |
 +--------------------------------------*/
#define PMC_SCPB_REG_REG_GPO_UNUSED_MASK    0x00000000
#define SCPB_REG_REG_GPO_BIT_GPO_MSK        0xffffffff
#define SCPB_REG_REG_GPO_BIT_GPO_OFF        0

/*-------------------------------.
 | Register 0x00084200 REMAP_REQ |
 +-------------------------------+
 | bit  28 R/W  REMAP_ABORT      |
 | bit  24 R/W  REMAP_REQ        |
 +------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_REQ_UNUSED_MASK     0xeeffffff
#define SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_MSK 0x10000000
#define SCPB_REG_REG_REMAP_REQ_BIT_REMAP_ABORT_OFF 28
#define SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_MSK   0x01000000
#define SCPB_REG_REG_REMAP_REQ_BIT_REMAP_REQ_OFF   24

/*-------------------------------------.
 | Register 0x00084204 REMAP_OLD_ADDR  |
 +-------------------------------------+
 | bit  15:0 R/W  REMAP_OLD_BLOCK_ADDR |
 +------------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_OLD_ADDR_UNUSED_MASK              0xffff0000
#define SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_MSK 0x0000ffff
#define SCPB_REG_REG_REMAP_OLD_ADDR_BIT_REMAP_OLD_BLOCK_ADDR_OFF 0

/*-------------------------------------.
 | Register 0x00084208 REMAP_NEW_ADDR  |
 +-------------------------------------+
 | bit  15:0 R/W  REMAP_NEW_BLOCK_ADDR |
 +------------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_NEW_ADDR_UNUSED_MASK              0xffff0000
#define SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_MSK 0x0000ffff
#define SCPB_REG_REG_REMAP_NEW_ADDR_BIT_REMAP_NEW_BLOCK_ADDR_OFF 0

/*--------------------------------.
 | Register 0x0008420c REMAP_STAT |
 +--------------------------------+
 | bit  0 R  REMAP_SUCCESS        |
 +-------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_STAT_UNUSED_MASK       0xfffffffe
#define SCPB_REG_REG_REMAP_STAT_BIT_REMAP_SUCCESS_MSK 0x00000001
#define SCPB_REG_REG_REMAP_STAT_BIT_REMAP_SUCCESS_OFF 0

/*----------------------------------.
 | Register 0x00084210 REMAP_MARGIN |
 +----------------------------------+
 | bit  7:0 R/W  REMAP_WPTR_MARGIN  |
 +---------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_MARGIN_UNUSED_MASK           0xffffff00
#define SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_MSK 0x000000ff
#define SCPB_REG_REG_REMAP_MARGIN_BIT_REMAP_WPTR_MARGIN_OFF 0

/*---------------------------------------.
 | Register 0x00084214 REMAP_OLD_BLK_CFG |
 +---------------------------------------+
 | bit  23:16 R/W  REMAP_WQID            |
 | bit  8:0   R/W  REMAP_LBA             |
 +--------------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_OLD_BLK_CFG_UNUSED_MASK    0xff00fe00
#define SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_MSK 0x00ff0000
#define SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_WQID_OFF 16
#define SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_MSK  0x000001ff
#define SCPB_REG_REG_REMAP_OLD_BLK_CFG_BIT_REMAP_LBA_OFF  0

/*----------------------------------------.
 | Register 0x00084218 REMAP_OLD_PAGE_CFG |
 +----------------------------------------+
 | bit  17:16 R/W  REMAP_QS_ID            |
 | bit  9:8   R/W  REMAP_WP_ID            |
 +---------------------------------------*/
#define PMC_SCPB_REG_REG_REMAP_OLD_PAGE_CFG_UNUSED_MASK     0xfffcfcff
#define SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_MSK 0x00030000
#define SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_QS_ID_OFF 16
#define SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_MSK 0x00000300
#define SCPB_REG_REG_REMAP_OLD_PAGE_CFG_BIT_REMAP_WP_ID_OFF 8

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SCPB_REG_REGS_H */
