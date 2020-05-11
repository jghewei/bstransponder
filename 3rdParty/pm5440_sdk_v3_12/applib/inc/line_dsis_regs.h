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
 *     line_dsis block
 *****************************************************************************/
#ifndef _LINE_DSIS_REGS_H
#define _LINE_DSIS_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11( N )   (0x00000000 + (N) * 0x4)
#define PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11( N )     (0x00000030 + (N) * 0x4)
#define PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11( N )      (0x00000060 + (N) * 0x4)
#define PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11( N )      (0x00000090 + (N) * 0x4)
#define PMC_DSIS_REG_ENET_TX_CLK_SELECT                         0x000000c0
#define PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11( N )      (0x000000c4 + (N) * 0x4)
#define PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11( N )          (0x000000f4 + (N) * 0x4)
#define PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19( N ) (0x00000124 + (N) * 0x4)
#define PMC_DSIS_REG_SERDES_TX_LANE_EN                          0x00000144
#define PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT( N )              (0x00000148 + (N) * 0x4)
#define PMC_DSIS_REG_TX_FIFO_RESET                              0x00000198
#define PMC_DSIS_REG_TX_FIFO_MODE_SELECT                        0x0000019c
#define PMC_DSIS_REG_TX_FIFO_CENTERING_MODE                     0x000001a0
#define PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST                  0x000001a4
#define PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19( N )  (0x000001a8 + (N) * 0x4)
#define PMC_DSIS_REG_TX_FIFO_0_INT                              0x000001f8
#define PMC_DSIS_REG_TX_FIFO_1_INT                              0x000001fc
#define PMC_DSIS_REG_TX_FIFO_2_INT                              0x00000200
#define PMC_DSIS_REG_TX_FIFO_3_INT                              0x00000204
#define PMC_DSIS_REG_TX_FIFO_4_INT                              0x00000208
#define PMC_DSIS_REG_TX_FIFO_5_INT                              0x0000020c
#define PMC_DSIS_REG_TX_FIFO_6_INT                              0x00000210
#define PMC_DSIS_REG_TX_FIFO_7_INT                              0x00000214
#define PMC_DSIS_REG_TX_FIFO_8_INT                              0x00000218
#define PMC_DSIS_REG_TX_FIFO_9_INT                              0x0000021c
#define PMC_DSIS_REG_TX_FIFO_10_INT                             0x00000220
#define PMC_DSIS_REG_TX_FIFO_11_INT                             0x00000224
#define PMC_DSIS_REG_TX_FIFO_12_INT                             0x00000228
#define PMC_DSIS_REG_TX_FIFO_13_INT                             0x0000022c
#define PMC_DSIS_REG_TX_FIFO_14_INT                             0x00000230
#define PMC_DSIS_REG_TX_FIFO_15_INT                             0x00000234
#define PMC_DSIS_REG_TX_FIFO_16_INT                             0x00000238
#define PMC_DSIS_REG_TX_FIFO_17_INT                             0x0000023c
#define PMC_DSIS_REG_TX_FIFO_18_INT                             0x00000240
#define PMC_DSIS_REG_TX_FIFO_19_INT                             0x00000244
#define PMC_DSIS_REG_TX_FIFO_0_INT_EN                           0x00000248
#define PMC_DSIS_REG_TX_FIFO_1_INT_EN                           0x0000024c
#define PMC_DSIS_REG_TX_FIFO_2_INT_EN                           0x00000250
#define PMC_DSIS_REG_TX_FIFO_3_INT_EN                           0x00000254
#define PMC_DSIS_REG_TX_FIFO_4_INT_EN                           0x00000258
#define PMC_DSIS_REG_TX_FIFO_5_INT_EN                           0x0000025c
#define PMC_DSIS_REG_TX_FIFO_6_INT_EN                           0x00000260
#define PMC_DSIS_REG_TX_FIFO_7_INT_EN                           0x00000264
#define PMC_DSIS_REG_TX_FIFO_8_INT_EN                           0x00000268
#define PMC_DSIS_REG_TX_FIFO_9_INT_EN                           0x0000026c
#define PMC_DSIS_REG_TX_FIFO_10_INT_EN                          0x00000270
#define PMC_DSIS_REG_TX_FIFO_11_INT_EN                          0x00000274
#define PMC_DSIS_REG_TX_FIFO_12_INT_EN                          0x00000278
#define PMC_DSIS_REG_TX_FIFO_13_INT_EN                          0x0000027c
#define PMC_DSIS_REG_TX_FIFO_14_INT_EN                          0x00000280
#define PMC_DSIS_REG_TX_FIFO_15_INT_EN                          0x00000284
#define PMC_DSIS_REG_TX_FIFO_16_INT_EN                          0x00000288
#define PMC_DSIS_REG_TX_FIFO_17_INT_EN                          0x0000028c
#define PMC_DSIS_REG_TX_FIFO_18_INT_EN                          0x00000290
#define PMC_DSIS_REG_TX_FIFO_19_INT_EN                          0x00000294
#define PMC_DSIS_REG_INT_SUMMARY                                0x00000298

/* index definitions for PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 */
#define PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_INDEX_N_MIN    0
#define PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_INDEX_N_MAX    11
#define PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_INDEX_N_SIZE   12
#define PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------------------.
 | Register (0x00000000 + (N) * 0x4) RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 |
 +-----------------------------------------------------------------------+
 | bit  10:8 R/W  RX_EN                                                  |
 | bit  7:4  R/W  RX_LSEL                                                |
 | bit  3:0  R/W  RX_SEL                                                 |
 +----------------------------------------------------------------------*/
#define PMC_DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_UNUSED_MASK    0xfffff800
#define DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_MSK      0x00000700
#define DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_EN_OFF      8
#define DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_MSK    0x000000f0
#define DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_LSEL_OFF    4
#define DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_MSK     0x0000000f
#define DSIS_REG_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_BIT_RX_SEL_OFF     0

/* index definitions for PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 */
#define PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_INDEX_N_MIN    0
#define PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_INDEX_N_MAX    11
#define PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_INDEX_N_SIZE   12
#define PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------------.
 | Register (0x00000030 + (N) * 0x4) RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 |
 +---------------------------------------------------------------------+
 | bit  5:4 R/W  RX_I_LSEL                                             |
 | bit  1:0 R/W  RX_I_SEL                                              |
 +--------------------------------------------------------------------*/
#define PMC_DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_UNUSED_MASK    0xffffffcc
#define DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_MSK  0x00000030
#define DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_LSEL_OFF  4
#define DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_MSK   0x00000003
#define DSIS_REG_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_BIT_RX_I_SEL_OFF   0

/* index definitions for PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11 */
#define PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_INDEX_N_MIN    0
#define PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_INDEX_N_MAX    11
#define PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_INDEX_N_SIZE   12
#define PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------------.
 | Register (0x00000060 + (N) * 0x4) TX_CLK_SEL_AND_EN_FOR_LANES_0_11 |
 +--------------------------------------------------------------------+
 | bit  6:4 R/W  TX_CLK_EN                                            |
 | bit  3:0 R/W  TX_CLK_SEL                                           |
 +-------------------------------------------------------------------*/
#define PMC_DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_UNUSED_MASK    0xffffff80
#define DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_MSK  0x00000070
#define DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_EN_OFF  4
#define DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_MSK 0x0000000f
#define DSIS_REG_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_BIT_TX_CLK_SEL_OFF 0

/* index definitions for PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11 */
#define PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_INDEX_N_MIN      0
#define PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_INDEX_N_MAX      11
#define PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_INDEX_N_SIZE     12
#define PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_INDEX_N_OFFSET   0x4

/*--------------------------------------------------------------------.
 | Register (0x00000090 + (N) * 0x4) TX_CLK_PREMUX_SEL_FOR_LANES_0_11 |
 +--------------------------------------------------------------------+
 | bit  1:0 R/W  TX_I_CLK_SEL                                         |
 +-------------------------------------------------------------------*/
#define PMC_DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_UNUSED_MASK      0xfffffffc
#define DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_MSK 0x00000003
#define DSIS_REG_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_BIT_TX_I_CLK_SEL_OFF 0

/*----------------------------------------.
 | Register 0x000000c0 ENET_TX_CLK_SELECT |
 +----------------------------------------+
 | bit  11:0 R/W  ENET_TX_CLK_SEL         |
 +---------------------------------------*/
#define PMC_DSIS_REG_ENET_TX_CLK_SELECT_UNUSED_MASK         0xfffff000
#define DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_MSK 0x00000fff
#define DSIS_REG_ENET_TX_CLK_SELECT_BIT_ENET_TX_CLK_SEL_OFF 0

/* index definitions for PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11 */
#define PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_INDEX_N_MIN    0
#define PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_INDEX_N_MAX    11
#define PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_INDEX_N_SIZE   12
#define PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------------.
 | Register (0x000000c4 + (N) * 0x4) TX_AND_ACB_SS_SEL_FOR_LANES_0_11 |
 +--------------------------------------------------------------------+
 | bit  6:4 R/W  ACB_IF_SEL                                           |
 | bit  1:0 R/W  TX_IF_SEL                                            |
 +-------------------------------------------------------------------*/
#define PMC_DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_UNUSED_MASK    0xffffff8c
#define DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_MSK 0x00000070
#define DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_ACB_IF_SEL_OFF 4
#define DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_MSK  0x00000003
#define DSIS_REG_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_BIT_TX_IF_SEL_OFF  0

/* index definitions for PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11 */
#define PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_INDEX_N_MIN    0
#define PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_INDEX_N_MAX    11
#define PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_INDEX_N_SIZE   12
#define PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_INDEX_N_OFFSET 0x4

/*----------------------------------------------------------------.
 | Register (0x000000f4 + (N) * 0x4) TX_SEL_FOR_SERDES_LANES_0_11 |
 +----------------------------------------------------------------+
 | bit  3:0 R/W  TX_SEL                                           |
 +---------------------------------------------------------------*/
#define PMC_DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_UNUSED_MASK    0xfffffff0
#define DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_MSK     0x0000000f
#define DSIS_REG_TX_SEL_FOR_SERDES_LANES_0_11_BIT_TX_SEL_OFF     0

/* index definitions for PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19 */
#define PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_INDEX_N_MIN    0
#define PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_INDEX_N_MAX    7
#define PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_INDEX_N_SIZE   8
#define PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------------------------.
 | Register (0x00000124 + (N) * 0x4) TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19 |
 +-------------------------------------------------------------------------+
 | bit  5:4 R/W  ACB_O_SEL                                                 |
 | bit  1:0 R/W  TX_O_SEL                                                  |
 +------------------------------------------------------------------------*/
#define PMC_DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_UNUSED_MASK    0xffffffcc
#define DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_BIT_ACB_O_SEL_MSK  0x00000030
#define DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_BIT_ACB_O_SEL_OFF  4
#define DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_BIT_TX_O_SEL_MSK   0x00000003
#define DSIS_REG_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_BIT_TX_O_SEL_OFF   0

/*---------------------------------------.
 | Register 0x00000144 SERDES_TX_LANE_EN |
 +---------------------------------------+
 | bit  19:0 R/W  TX_LANE_ENABLE         |
 +--------------------------------------*/
#define PMC_DSIS_REG_SERDES_TX_LANE_EN_UNUSED_MASK        0xfff00000
#define DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_MSK 0x000fffff
#define DSIS_REG_SERDES_TX_LANE_EN_BIT_TX_LANE_ENABLE_OFF 0

/* index definitions for PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT */
#define PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_INDEX_N_MIN               0
#define PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_INDEX_N_MAX               19
#define PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_INDEX_N_SIZE              20
#define PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_INDEX_N_OFFSET            0x4

/*------------------------------------------------------------.
 | Register (0x00000148 + (N) * 0x4) TX_FIFO_WRITE_CLK_SELECT |
 +------------------------------------------------------------+
 | bit  4:0 R/W  TX_FIFO_WRITE_CLK_SEL                        |
 +-----------------------------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_UNUSED_MASK               0xffffffe0
#define DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_MSK 0x0000001f
#define DSIS_REG_TX_FIFO_WRITE_CLK_SELECT_BIT_TX_FIFO_WRITE_CLK_SEL_OFF 0

/*-----------------------------------.
 | Register 0x00000198 TX_FIFO_RESET |
 +-----------------------------------+
 | bit  19:0 R/W  TX_FIFO_RESET      |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_RESET_UNUSED_MASK       0xfff00000
#define DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_MSK 0x000fffff
#define DSIS_REG_TX_FIFO_RESET_BIT_TX_FIFO_RESET_OFF 0

/*-----------------------------------------.
 | Register 0x0000019c TX_FIFO_MODE_SELECT |
 +-----------------------------------------+
 | bit  19:0 R/W  TX_FIFO_MODE_SEL         |
 +----------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_MODE_SELECT_UNUSED_MASK          0xfff00000
#define DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_MSK 0x000fffff
#define DSIS_REG_TX_FIFO_MODE_SELECT_BIT_TX_FIFO_MODE_SEL_OFF 0

/*--------------------------------------------.
 | Register 0x000001a0 TX_FIFO_CENTERING_MODE |
 +--------------------------------------------+
 | bit  19:0 R/W  TX_FIFO_AUTO_CENTER         |
 +-------------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_CENTERING_MODE_UNUSED_MASK             0xfff00000
#define DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_MSK 0x000fffff
#define DSIS_REG_TX_FIFO_CENTERING_MODE_BIT_TX_FIFO_AUTO_CENTER_OFF 0

/*-----------------------------------------------.
 | Register 0x000001a4 TX_FIFO_CENTERING_REQUEST |
 +-----------------------------------------------+
 | bit  19:0 R/W  TX_FIFO_CENTER_REQUEST         |
 +----------------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_CENTERING_REQUEST_UNUSED_MASK                0xfff00000
#define DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_MSK 0x000fffff
#define DSIS_REG_TX_FIFO_CENTERING_REQUEST_BIT_TX_FIFO_CENTER_REQUEST_OFF 0

/* index definitions for PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19 */
#define PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_INDEX_N_MIN          0
#define PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_INDEX_N_MAX          19
#define PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_INDEX_N_SIZE         20
#define PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_INDEX_N_OFFSET       0x4

/*------------------------------------------------------------------------.
 | Register (0x000001a8 + (N) * 0x4) TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19 |
 +------------------------------------------------------------------------+
 | bit  2:0 R/W  TX_FIFO_READ_PTR                                         |
 +-----------------------------------------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_UNUSED_MASK          0xfffffff8
#define DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_BIT_TX_FIFO_READ_PTR_MSK 0x00000007
#define DSIS_REG_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_BIT_TX_FIFO_READ_PTR_OFF 0

/*-----------------------------------.
 | Register 0x000001f8 TX_FIFO_0_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_0_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_0_I    |
 | bit  2 R/W  RP_ABOVE_EXP_0_I      |
 | bit  1 R/W  RP_BELOW_EXP_0_I      |
 | bit  0 R/W  RP_IN_SYNC_0_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_0_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_NOT_CENTERED_0_I_OFF 4
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_OUT_OF_SYNC_0_I_OFF  3
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_ABOVE_EXP_0_I_OFF    2
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_BELOW_EXP_0_I_OFF    1
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_0_INT_BIT_RP_IN_SYNC_0_I_OFF      0

/*-----------------------------------.
 | Register 0x000001fc TX_FIFO_1_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_1_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_1_I    |
 | bit  2 R/W  RP_ABOVE_EXP_1_I      |
 | bit  1 R/W  RP_BELOW_EXP_1_I      |
 | bit  0 R/W  RP_IN_SYNC_1_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_1_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_NOT_CENTERED_1_I_OFF 4
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_OUT_OF_SYNC_1_I_OFF  3
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_ABOVE_EXP_1_I_OFF    2
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_BELOW_EXP_1_I_OFF    1
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_1_INT_BIT_RP_IN_SYNC_1_I_OFF      0

/*-----------------------------------.
 | Register 0x00000200 TX_FIFO_2_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_2_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_2_I    |
 | bit  2 R/W  RP_ABOVE_EXP_2_I      |
 | bit  1 R/W  RP_BELOW_EXP_2_I      |
 | bit  0 R/W  RP_IN_SYNC_2_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_2_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_NOT_CENTERED_2_I_OFF 4
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_OUT_OF_SYNC_2_I_OFF  3
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_ABOVE_EXP_2_I_OFF    2
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_BELOW_EXP_2_I_OFF    1
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_2_INT_BIT_RP_IN_SYNC_2_I_OFF      0

/*-----------------------------------.
 | Register 0x00000204 TX_FIFO_3_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_3_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_3_I    |
 | bit  2 R/W  RP_ABOVE_EXP_3_I      |
 | bit  1 R/W  RP_BELOW_EXP_3_I      |
 | bit  0 R/W  RP_IN_SYNC_3_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_3_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_NOT_CENTERED_3_I_OFF 4
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_OUT_OF_SYNC_3_I_OFF  3
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_ABOVE_EXP_3_I_OFF    2
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_BELOW_EXP_3_I_OFF    1
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_3_INT_BIT_RP_IN_SYNC_3_I_OFF      0

/*-----------------------------------.
 | Register 0x00000208 TX_FIFO_4_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_4_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_4_I    |
 | bit  2 R/W  RP_ABOVE_EXP_4_I      |
 | bit  1 R/W  RP_BELOW_EXP_4_I      |
 | bit  0 R/W  RP_IN_SYNC_4_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_4_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_NOT_CENTERED_4_I_OFF 4
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_OUT_OF_SYNC_4_I_OFF  3
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_ABOVE_EXP_4_I_OFF    2
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_BELOW_EXP_4_I_OFF    1
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_4_INT_BIT_RP_IN_SYNC_4_I_OFF      0

/*-----------------------------------.
 | Register 0x0000020c TX_FIFO_5_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_5_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_5_I    |
 | bit  2 R/W  RP_ABOVE_EXP_5_I      |
 | bit  1 R/W  RP_BELOW_EXP_5_I      |
 | bit  0 R/W  RP_IN_SYNC_5_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_5_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_NOT_CENTERED_5_I_OFF 4
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_OUT_OF_SYNC_5_I_OFF  3
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_ABOVE_EXP_5_I_OFF    2
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_BELOW_EXP_5_I_OFF    1
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_5_INT_BIT_RP_IN_SYNC_5_I_OFF      0

/*-----------------------------------.
 | Register 0x00000210 TX_FIFO_6_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_6_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_6_I    |
 | bit  2 R/W  RP_ABOVE_EXP_6_I      |
 | bit  1 R/W  RP_BELOW_EXP_6_I      |
 | bit  0 R/W  RP_IN_SYNC_6_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_6_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_NOT_CENTERED_6_I_OFF 4
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_OUT_OF_SYNC_6_I_OFF  3
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_ABOVE_EXP_6_I_OFF    2
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_BELOW_EXP_6_I_OFF    1
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_6_INT_BIT_RP_IN_SYNC_6_I_OFF      0

/*-----------------------------------.
 | Register 0x00000214 TX_FIFO_7_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_7_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_7_I    |
 | bit  2 R/W  RP_ABOVE_EXP_7_I      |
 | bit  1 R/W  RP_BELOW_EXP_7_I      |
 | bit  0 R/W  RP_IN_SYNC_7_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_7_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_NOT_CENTERED_7_I_OFF 4
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_OUT_OF_SYNC_7_I_OFF  3
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_ABOVE_EXP_7_I_OFF    2
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_BELOW_EXP_7_I_OFF    1
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_7_INT_BIT_RP_IN_SYNC_7_I_OFF      0

/*-----------------------------------.
 | Register 0x00000218 TX_FIFO_8_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_8_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_8_I    |
 | bit  2 R/W  RP_ABOVE_EXP_8_I      |
 | bit  1 R/W  RP_BELOW_EXP_8_I      |
 | bit  0 R/W  RP_IN_SYNC_8_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_8_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_NOT_CENTERED_8_I_OFF 4
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_OUT_OF_SYNC_8_I_OFF  3
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_ABOVE_EXP_8_I_OFF    2
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_BELOW_EXP_8_I_OFF    1
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_8_INT_BIT_RP_IN_SYNC_8_I_OFF      0

/*-----------------------------------.
 | Register 0x0000021c TX_FIFO_9_INT |
 +-----------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_9_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_9_I    |
 | bit  2 R/W  RP_ABOVE_EXP_9_I      |
 | bit  1 R/W  RP_BELOW_EXP_9_I      |
 | bit  0 R/W  RP_IN_SYNC_9_I        |
 +----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_9_INT_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_NOT_CENTERED_9_I_OFF 4
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_OUT_OF_SYNC_9_I_OFF  3
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_ABOVE_EXP_9_I_OFF    2
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_BELOW_EXP_9_I_OFF    1
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_9_INT_BIT_RP_IN_SYNC_9_I_OFF      0

/*------------------------------------.
 | Register 0x00000220 TX_FIFO_10_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_10_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_10_I    |
 | bit  2 R/W  RP_ABOVE_EXP_10_I      |
 | bit  1 R/W  RP_BELOW_EXP_10_I      |
 | bit  0 R/W  RP_IN_SYNC_10_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_10_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_NOT_CENTERED_10_I_OFF 4
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_OUT_OF_SYNC_10_I_OFF  3
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_ABOVE_EXP_10_I_OFF    2
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_BELOW_EXP_10_I_OFF    1
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_10_INT_BIT_RP_IN_SYNC_10_I_OFF      0

/*------------------------------------.
 | Register 0x00000224 TX_FIFO_11_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_11_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_11_I    |
 | bit  2 R/W  RP_ABOVE_EXP_11_I      |
 | bit  1 R/W  RP_BELOW_EXP_11_I      |
 | bit  0 R/W  RP_IN_SYNC_11_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_11_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_NOT_CENTERED_11_I_OFF 4
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_OUT_OF_SYNC_11_I_OFF  3
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_ABOVE_EXP_11_I_OFF    2
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_BELOW_EXP_11_I_OFF    1
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_11_INT_BIT_RP_IN_SYNC_11_I_OFF      0

/*------------------------------------.
 | Register 0x00000228 TX_FIFO_12_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_12_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_12_I    |
 | bit  2 R/W  RP_ABOVE_EXP_12_I      |
 | bit  1 R/W  RP_BELOW_EXP_12_I      |
 | bit  0 R/W  RP_IN_SYNC_12_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_12_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_NOT_CENTERED_12_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_NOT_CENTERED_12_I_OFF 4
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_OUT_OF_SYNC_12_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_OUT_OF_SYNC_12_I_OFF  3
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_ABOVE_EXP_12_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_ABOVE_EXP_12_I_OFF    2
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_BELOW_EXP_12_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_BELOW_EXP_12_I_OFF    1
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_IN_SYNC_12_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_12_INT_BIT_RP_IN_SYNC_12_I_OFF      0

/*------------------------------------.
 | Register 0x0000022c TX_FIFO_13_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_13_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_13_I    |
 | bit  2 R/W  RP_ABOVE_EXP_13_I      |
 | bit  1 R/W  RP_BELOW_EXP_13_I      |
 | bit  0 R/W  RP_IN_SYNC_13_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_13_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_NOT_CENTERED_13_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_NOT_CENTERED_13_I_OFF 4
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_OUT_OF_SYNC_13_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_OUT_OF_SYNC_13_I_OFF  3
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_ABOVE_EXP_13_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_ABOVE_EXP_13_I_OFF    2
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_BELOW_EXP_13_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_BELOW_EXP_13_I_OFF    1
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_IN_SYNC_13_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_13_INT_BIT_RP_IN_SYNC_13_I_OFF      0

/*------------------------------------.
 | Register 0x00000230 TX_FIFO_14_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_14_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_14_I    |
 | bit  2 R/W  RP_ABOVE_EXP_14_I      |
 | bit  1 R/W  RP_BELOW_EXP_14_I      |
 | bit  0 R/W  RP_IN_SYNC_14_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_14_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_NOT_CENTERED_14_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_NOT_CENTERED_14_I_OFF 4
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_OUT_OF_SYNC_14_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_OUT_OF_SYNC_14_I_OFF  3
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_ABOVE_EXP_14_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_ABOVE_EXP_14_I_OFF    2
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_BELOW_EXP_14_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_BELOW_EXP_14_I_OFF    1
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_IN_SYNC_14_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_14_INT_BIT_RP_IN_SYNC_14_I_OFF      0

/*------------------------------------.
 | Register 0x00000234 TX_FIFO_15_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_15_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_15_I    |
 | bit  2 R/W  RP_ABOVE_EXP_15_I      |
 | bit  1 R/W  RP_BELOW_EXP_15_I      |
 | bit  0 R/W  RP_IN_SYNC_15_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_15_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_NOT_CENTERED_15_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_NOT_CENTERED_15_I_OFF 4
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_OUT_OF_SYNC_15_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_OUT_OF_SYNC_15_I_OFF  3
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_ABOVE_EXP_15_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_ABOVE_EXP_15_I_OFF    2
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_BELOW_EXP_15_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_BELOW_EXP_15_I_OFF    1
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_IN_SYNC_15_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_15_INT_BIT_RP_IN_SYNC_15_I_OFF      0

/*------------------------------------.
 | Register 0x00000238 TX_FIFO_16_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_16_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_16_I    |
 | bit  2 R/W  RP_ABOVE_EXP_16_I      |
 | bit  1 R/W  RP_BELOW_EXP_16_I      |
 | bit  0 R/W  RP_IN_SYNC_16_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_16_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_NOT_CENTERED_16_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_NOT_CENTERED_16_I_OFF 4
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_OUT_OF_SYNC_16_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_OUT_OF_SYNC_16_I_OFF  3
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_ABOVE_EXP_16_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_ABOVE_EXP_16_I_OFF    2
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_BELOW_EXP_16_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_BELOW_EXP_16_I_OFF    1
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_IN_SYNC_16_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_16_INT_BIT_RP_IN_SYNC_16_I_OFF      0

/*------------------------------------.
 | Register 0x0000023c TX_FIFO_17_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_17_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_17_I    |
 | bit  2 R/W  RP_ABOVE_EXP_17_I      |
 | bit  1 R/W  RP_BELOW_EXP_17_I      |
 | bit  0 R/W  RP_IN_SYNC_17_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_17_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_NOT_CENTERED_17_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_NOT_CENTERED_17_I_OFF 4
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_OUT_OF_SYNC_17_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_OUT_OF_SYNC_17_I_OFF  3
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_ABOVE_EXP_17_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_ABOVE_EXP_17_I_OFF    2
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_BELOW_EXP_17_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_BELOW_EXP_17_I_OFF    1
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_IN_SYNC_17_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_17_INT_BIT_RP_IN_SYNC_17_I_OFF      0

/*------------------------------------.
 | Register 0x00000240 TX_FIFO_18_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_18_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_18_I    |
 | bit  2 R/W  RP_ABOVE_EXP_18_I      |
 | bit  1 R/W  RP_BELOW_EXP_18_I      |
 | bit  0 R/W  RP_IN_SYNC_18_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_18_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_NOT_CENTERED_18_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_NOT_CENTERED_18_I_OFF 4
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_OUT_OF_SYNC_18_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_OUT_OF_SYNC_18_I_OFF  3
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_ABOVE_EXP_18_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_ABOVE_EXP_18_I_OFF    2
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_BELOW_EXP_18_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_BELOW_EXP_18_I_OFF    1
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_IN_SYNC_18_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_18_INT_BIT_RP_IN_SYNC_18_I_OFF      0

/*------------------------------------.
 | Register 0x00000244 TX_FIFO_19_INT |
 +------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_19_I   |
 | bit  3 R/W  RP_OUT_OF_SYNC_19_I    |
 | bit  2 R/W  RP_ABOVE_EXP_19_I      |
 | bit  1 R/W  RP_BELOW_EXP_19_I      |
 | bit  0 R/W  RP_IN_SYNC_19_I        |
 +-----------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_19_INT_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_NOT_CENTERED_19_I_MSK 0x00000010
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_NOT_CENTERED_19_I_OFF 4
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_OUT_OF_SYNC_19_I_MSK  0x00000008
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_OUT_OF_SYNC_19_I_OFF  3
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_ABOVE_EXP_19_I_MSK    0x00000004
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_ABOVE_EXP_19_I_OFF    2
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_BELOW_EXP_19_I_MSK    0x00000002
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_BELOW_EXP_19_I_OFF    1
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_IN_SYNC_19_I_MSK      0x00000001
#define DSIS_REG_TX_FIFO_19_INT_BIT_RP_IN_SYNC_19_I_OFF      0

/*--------------------------------------.
 | Register 0x00000248 TX_FIFO_0_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_0_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_0_E       |
 | bit  2 R/W  RP_ABOVE_EXP_0_E         |
 | bit  1 R/W  RP_BELOW_EXP_0_E         |
 | bit  0 R/W  RP_IN_SYNC_0_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_0_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_NOT_CENTERED_0_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_NOT_CENTERED_0_E_OFF 4
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_OUT_OF_SYNC_0_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_OUT_OF_SYNC_0_E_OFF  3
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_ABOVE_EXP_0_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_ABOVE_EXP_0_E_OFF    2
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_BELOW_EXP_0_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_BELOW_EXP_0_E_OFF    1
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_IN_SYNC_0_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_0_INT_EN_BIT_RP_IN_SYNC_0_E_OFF      0

/*--------------------------------------.
 | Register 0x0000024c TX_FIFO_1_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_1_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_1_E       |
 | bit  2 R/W  RP_ABOVE_EXP_1_E         |
 | bit  1 R/W  RP_BELOW_EXP_1_E         |
 | bit  0 R/W  RP_IN_SYNC_1_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_1_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_NOT_CENTERED_1_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_NOT_CENTERED_1_E_OFF 4
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_OUT_OF_SYNC_1_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_OUT_OF_SYNC_1_E_OFF  3
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_ABOVE_EXP_1_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_ABOVE_EXP_1_E_OFF    2
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_BELOW_EXP_1_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_BELOW_EXP_1_E_OFF    1
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_IN_SYNC_1_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_1_INT_EN_BIT_RP_IN_SYNC_1_E_OFF      0

/*--------------------------------------.
 | Register 0x00000250 TX_FIFO_2_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_2_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_2_E       |
 | bit  2 R/W  RP_ABOVE_EXP_2_E         |
 | bit  1 R/W  RP_BELOW_EXP_2_E         |
 | bit  0 R/W  RP_IN_SYNC_2_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_2_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_NOT_CENTERED_2_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_NOT_CENTERED_2_E_OFF 4
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_OUT_OF_SYNC_2_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_OUT_OF_SYNC_2_E_OFF  3
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_ABOVE_EXP_2_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_ABOVE_EXP_2_E_OFF    2
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_BELOW_EXP_2_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_BELOW_EXP_2_E_OFF    1
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_IN_SYNC_2_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_2_INT_EN_BIT_RP_IN_SYNC_2_E_OFF      0

/*--------------------------------------.
 | Register 0x00000254 TX_FIFO_3_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_3_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_3_E       |
 | bit  2 R/W  RP_ABOVE_EXP_3_E         |
 | bit  1 R/W  RP_BELOW_EXP_3_E         |
 | bit  0 R/W  RP_IN_SYNC_3_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_3_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_NOT_CENTERED_3_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_NOT_CENTERED_3_E_OFF 4
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_OUT_OF_SYNC_3_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_OUT_OF_SYNC_3_E_OFF  3
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_ABOVE_EXP_3_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_ABOVE_EXP_3_E_OFF    2
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_BELOW_EXP_3_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_BELOW_EXP_3_E_OFF    1
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_IN_SYNC_3_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_3_INT_EN_BIT_RP_IN_SYNC_3_E_OFF      0

/*--------------------------------------.
 | Register 0x00000258 TX_FIFO_4_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_4_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_4_E       |
 | bit  2 R/W  RP_ABOVE_EXP_4_E         |
 | bit  1 R/W  RP_BELOW_EXP_4_E         |
 | bit  0 R/W  RP_IN_SYNC_4_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_4_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_NOT_CENTERED_4_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_NOT_CENTERED_4_E_OFF 4
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_OUT_OF_SYNC_4_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_OUT_OF_SYNC_4_E_OFF  3
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_ABOVE_EXP_4_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_ABOVE_EXP_4_E_OFF    2
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_BELOW_EXP_4_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_BELOW_EXP_4_E_OFF    1
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_IN_SYNC_4_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_4_INT_EN_BIT_RP_IN_SYNC_4_E_OFF      0

/*--------------------------------------.
 | Register 0x0000025c TX_FIFO_5_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_5_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_5_E       |
 | bit  2 R/W  RP_ABOVE_EXP_5_E         |
 | bit  1 R/W  RP_BELOW_EXP_5_E         |
 | bit  0 R/W  RP_IN_SYNC_5_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_5_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_NOT_CENTERED_5_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_NOT_CENTERED_5_E_OFF 4
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_OUT_OF_SYNC_5_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_OUT_OF_SYNC_5_E_OFF  3
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_ABOVE_EXP_5_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_ABOVE_EXP_5_E_OFF    2
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_BELOW_EXP_5_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_BELOW_EXP_5_E_OFF    1
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_IN_SYNC_5_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_5_INT_EN_BIT_RP_IN_SYNC_5_E_OFF      0

/*--------------------------------------.
 | Register 0x00000260 TX_FIFO_6_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_6_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_6_E       |
 | bit  2 R/W  RP_ABOVE_EXP_6_E         |
 | bit  1 R/W  RP_BELOW_EXP_6_E         |
 | bit  0 R/W  RP_IN_SYNC_6_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_6_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_NOT_CENTERED_6_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_NOT_CENTERED_6_E_OFF 4
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_OUT_OF_SYNC_6_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_OUT_OF_SYNC_6_E_OFF  3
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_ABOVE_EXP_6_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_ABOVE_EXP_6_E_OFF    2
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_BELOW_EXP_6_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_BELOW_EXP_6_E_OFF    1
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_IN_SYNC_6_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_6_INT_EN_BIT_RP_IN_SYNC_6_E_OFF      0

/*--------------------------------------.
 | Register 0x00000264 TX_FIFO_7_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_7_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_7_E       |
 | bit  2 R/W  RP_ABOVE_EXP_7_E         |
 | bit  1 R/W  RP_BELOW_EXP_7_E         |
 | bit  0 R/W  RP_IN_SYNC_7_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_7_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_NOT_CENTERED_7_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_NOT_CENTERED_7_E_OFF 4
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_OUT_OF_SYNC_7_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_OUT_OF_SYNC_7_E_OFF  3
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_ABOVE_EXP_7_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_ABOVE_EXP_7_E_OFF    2
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_BELOW_EXP_7_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_BELOW_EXP_7_E_OFF    1
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_IN_SYNC_7_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_7_INT_EN_BIT_RP_IN_SYNC_7_E_OFF      0

/*--------------------------------------.
 | Register 0x00000268 TX_FIFO_8_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_8_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_8_E       |
 | bit  2 R/W  RP_ABOVE_EXP_8_E         |
 | bit  1 R/W  RP_BELOW_EXP_8_E         |
 | bit  0 R/W  RP_IN_SYNC_8_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_8_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_NOT_CENTERED_8_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_NOT_CENTERED_8_E_OFF 4
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_OUT_OF_SYNC_8_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_OUT_OF_SYNC_8_E_OFF  3
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_ABOVE_EXP_8_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_ABOVE_EXP_8_E_OFF    2
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_BELOW_EXP_8_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_BELOW_EXP_8_E_OFF    1
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_IN_SYNC_8_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_8_INT_EN_BIT_RP_IN_SYNC_8_E_OFF      0

/*--------------------------------------.
 | Register 0x0000026c TX_FIFO_9_INT_EN |
 +--------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_9_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_9_E       |
 | bit  2 R/W  RP_ABOVE_EXP_9_E         |
 | bit  1 R/W  RP_BELOW_EXP_9_E         |
 | bit  0 R/W  RP_IN_SYNC_9_E           |
 +-------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_9_INT_EN_UNUSED_MASK             0xffffffe0
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_NOT_CENTERED_9_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_NOT_CENTERED_9_E_OFF 4
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_OUT_OF_SYNC_9_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_OUT_OF_SYNC_9_E_OFF  3
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_ABOVE_EXP_9_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_ABOVE_EXP_9_E_OFF    2
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_BELOW_EXP_9_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_BELOW_EXP_9_E_OFF    1
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_IN_SYNC_9_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_9_INT_EN_BIT_RP_IN_SYNC_9_E_OFF      0

/*---------------------------------------.
 | Register 0x00000270 TX_FIFO_10_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_10_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_10_E       |
 | bit  2 R/W  RP_ABOVE_EXP_10_E         |
 | bit  1 R/W  RP_BELOW_EXP_10_E         |
 | bit  0 R/W  RP_IN_SYNC_10_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_10_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_NOT_CENTERED_10_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_NOT_CENTERED_10_E_OFF 4
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_OUT_OF_SYNC_10_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_OUT_OF_SYNC_10_E_OFF  3
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_ABOVE_EXP_10_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_ABOVE_EXP_10_E_OFF    2
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_BELOW_EXP_10_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_BELOW_EXP_10_E_OFF    1
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_IN_SYNC_10_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_10_INT_EN_BIT_RP_IN_SYNC_10_E_OFF      0

/*---------------------------------------.
 | Register 0x00000274 TX_FIFO_11_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_11_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_11_E       |
 | bit  2 R/W  RP_ABOVE_EXP_11_E         |
 | bit  1 R/W  RP_BELOW_EXP_11_E         |
 | bit  0 R/W  RP_IN_SYNC_11_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_11_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_NOT_CENTERED_11_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_NOT_CENTERED_11_E_OFF 4
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_OUT_OF_SYNC_11_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_OUT_OF_SYNC_11_E_OFF  3
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_ABOVE_EXP_11_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_ABOVE_EXP_11_E_OFF    2
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_BELOW_EXP_11_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_BELOW_EXP_11_E_OFF    1
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_IN_SYNC_11_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_11_INT_EN_BIT_RP_IN_SYNC_11_E_OFF      0

/*---------------------------------------.
 | Register 0x00000278 TX_FIFO_12_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_12_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_12_E       |
 | bit  2 R/W  RP_ABOVE_EXP_12_E         |
 | bit  1 R/W  RP_BELOW_EXP_12_E         |
 | bit  0 R/W  RP_IN_SYNC_12_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_12_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_NOT_CENTERED_12_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_NOT_CENTERED_12_E_OFF 4
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_OUT_OF_SYNC_12_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_OUT_OF_SYNC_12_E_OFF  3
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_ABOVE_EXP_12_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_ABOVE_EXP_12_E_OFF    2
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_BELOW_EXP_12_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_BELOW_EXP_12_E_OFF    1
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_IN_SYNC_12_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_12_INT_EN_BIT_RP_IN_SYNC_12_E_OFF      0

/*---------------------------------------.
 | Register 0x0000027c TX_FIFO_13_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_13_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_13_E       |
 | bit  2 R/W  RP_ABOVE_EXP_13_E         |
 | bit  1 R/W  RP_BELOW_EXP_13_E         |
 | bit  0 R/W  RP_IN_SYNC_13_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_13_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_NOT_CENTERED_13_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_NOT_CENTERED_13_E_OFF 4
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_OUT_OF_SYNC_13_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_OUT_OF_SYNC_13_E_OFF  3
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_ABOVE_EXP_13_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_ABOVE_EXP_13_E_OFF    2
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_BELOW_EXP_13_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_BELOW_EXP_13_E_OFF    1
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_IN_SYNC_13_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_13_INT_EN_BIT_RP_IN_SYNC_13_E_OFF      0

/*---------------------------------------.
 | Register 0x00000280 TX_FIFO_14_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_14_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_14_E       |
 | bit  2 R/W  RP_ABOVE_EXP_14_E         |
 | bit  1 R/W  RP_BELOW_EXP_14_E         |
 | bit  0 R/W  RP_IN_SYNC_14_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_14_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_NOT_CENTERED_14_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_NOT_CENTERED_14_E_OFF 4
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_OUT_OF_SYNC_14_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_OUT_OF_SYNC_14_E_OFF  3
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_ABOVE_EXP_14_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_ABOVE_EXP_14_E_OFF    2
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_BELOW_EXP_14_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_BELOW_EXP_14_E_OFF    1
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_IN_SYNC_14_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_14_INT_EN_BIT_RP_IN_SYNC_14_E_OFF      0

/*---------------------------------------.
 | Register 0x00000284 TX_FIFO_15_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_15_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_15_E       |
 | bit  2 R/W  RP_ABOVE_EXP_15_E         |
 | bit  1 R/W  RP_BELOW_EXP_15_E         |
 | bit  0 R/W  RP_IN_SYNC_15_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_15_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_NOT_CENTERED_15_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_NOT_CENTERED_15_E_OFF 4
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_OUT_OF_SYNC_15_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_OUT_OF_SYNC_15_E_OFF  3
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_ABOVE_EXP_15_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_ABOVE_EXP_15_E_OFF    2
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_BELOW_EXP_15_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_BELOW_EXP_15_E_OFF    1
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_IN_SYNC_15_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_15_INT_EN_BIT_RP_IN_SYNC_15_E_OFF      0

/*---------------------------------------.
 | Register 0x00000288 TX_FIFO_16_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_16_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_16_E       |
 | bit  2 R/W  RP_ABOVE_EXP_16_E         |
 | bit  1 R/W  RP_BELOW_EXP_16_E         |
 | bit  0 R/W  RP_IN_SYNC_16_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_16_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_NOT_CENTERED_16_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_NOT_CENTERED_16_E_OFF 4
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_OUT_OF_SYNC_16_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_OUT_OF_SYNC_16_E_OFF  3
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_ABOVE_EXP_16_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_ABOVE_EXP_16_E_OFF    2
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_BELOW_EXP_16_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_BELOW_EXP_16_E_OFF    1
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_IN_SYNC_16_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_16_INT_EN_BIT_RP_IN_SYNC_16_E_OFF      0

/*---------------------------------------.
 | Register 0x0000028c TX_FIFO_17_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_17_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_17_E       |
 | bit  2 R/W  RP_ABOVE_EXP_17_E         |
 | bit  1 R/W  RP_BELOW_EXP_17_E         |
 | bit  0 R/W  RP_IN_SYNC_17_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_17_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_NOT_CENTERED_17_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_NOT_CENTERED_17_E_OFF 4
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_OUT_OF_SYNC_17_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_OUT_OF_SYNC_17_E_OFF  3
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_ABOVE_EXP_17_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_ABOVE_EXP_17_E_OFF    2
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_BELOW_EXP_17_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_BELOW_EXP_17_E_OFF    1
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_IN_SYNC_17_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_17_INT_EN_BIT_RP_IN_SYNC_17_E_OFF      0

/*---------------------------------------.
 | Register 0x00000290 TX_FIFO_18_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_18_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_18_E       |
 | bit  2 R/W  RP_ABOVE_EXP_18_E         |
 | bit  1 R/W  RP_BELOW_EXP_18_E         |
 | bit  0 R/W  RP_IN_SYNC_18_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_18_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_NOT_CENTERED_18_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_NOT_CENTERED_18_E_OFF 4
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_OUT_OF_SYNC_18_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_OUT_OF_SYNC_18_E_OFF  3
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_ABOVE_EXP_18_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_ABOVE_EXP_18_E_OFF    2
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_BELOW_EXP_18_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_BELOW_EXP_18_E_OFF    1
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_IN_SYNC_18_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_18_INT_EN_BIT_RP_IN_SYNC_18_E_OFF      0

/*---------------------------------------.
 | Register 0x00000294 TX_FIFO_19_INT_EN |
 +---------------------------------------+
 | bit  4 R/W  RP_NOT_CENTERED_19_E      |
 | bit  3 R/W  RP_OUT_OF_SYNC_19_E       |
 | bit  2 R/W  RP_ABOVE_EXP_19_E         |
 | bit  1 R/W  RP_BELOW_EXP_19_E         |
 | bit  0 R/W  RP_IN_SYNC_19_E           |
 +--------------------------------------*/
#define PMC_DSIS_REG_TX_FIFO_19_INT_EN_UNUSED_MASK              0xffffffe0
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_NOT_CENTERED_19_E_MSK 0x00000010
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_NOT_CENTERED_19_E_OFF 4
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_OUT_OF_SYNC_19_E_MSK  0x00000008
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_OUT_OF_SYNC_19_E_OFF  3
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_ABOVE_EXP_19_E_MSK    0x00000004
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_ABOVE_EXP_19_E_OFF    2
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_BELOW_EXP_19_E_MSK    0x00000002
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_BELOW_EXP_19_E_OFF    1
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_IN_SYNC_19_E_MSK      0x00000001
#define DSIS_REG_TX_FIFO_19_INT_EN_BIT_RP_IN_SYNC_19_E_OFF      0

/*---------------------------------.
 | Register 0x00000298 INT_SUMMARY |
 +---------------------------------+
 | bit  19:0 R  TX_FIFO_INT        |
 +--------------------------------*/
#define PMC_DSIS_REG_INT_SUMMARY_UNUSED_MASK     0xfff00000
#define DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_MSK 0x000fffff
#define DSIS_REG_INT_SUMMARY_BIT_TX_FIFO_INT_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINE_DSIS_REGS_H */
