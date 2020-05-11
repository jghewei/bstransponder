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
 *     d8_tx_2x block
 *****************************************************************************/
#ifndef _D8_TX_2X_REGS_H
#define _D8_TX_2X_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL                   0x00000000
#define PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL                0x00000004
#define PMC_SFI51_TX_2X_SLICE_REG_INT_REG                         0x00000008
#define PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG                      0x0000000c
#define PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG                    0x00000010
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1( N )    (0x00000014 + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2( N )    (0x0000001c + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG( N )            (0x00000024 + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG( N )               (0x0000002c + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1( N )    (0x00000034 + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1( N ) (0x0000003c + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2( N ) (0x00000044 + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1( N )          (0x0000004c + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1( N ) (0x00000054 + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2( N ) (0x0000005c + (N) * 0x4)
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1( N )      (0x00000064 + (N) * 0x4)

/*-----------------------------------------.
 | Register 0x00000000 SW_RESET_CTRL       |
 +-----------------------------------------+
 | bit  12 R/W  PSLB_CLK_DEGLITCH_B_RSTB   |
 | bit  11 R/W  PSLB_CLK_DEGLITCH_A_RSTB   |
 | bit  10 R/W  TPCOUT_CLK_DEGLITCH_B_RSTB |
 | bit  9  R/W  TPCOUT_CLK_DEGLITCH_A_RSTB |
 | bit  8  R/W  PATT_MON_B_RSTB            |
 | bit  7  R/W  PATT_MON_A_RSTB            |
 | bit  6  R/W  PATT_GEN_B_RSTB            |
 | bit  5  R/W  PATT_GEN_A_RSTB            |
 | bit  4  R/W  GEN_PISO_B_RSTB            |
 | bit  3  R/W  GEN_PISO_A_RSTB            |
 | bit  2  R/W  DESKEW_FIFO_B_RSTB         |
 | bit  1  R/W  DESKEW_FIFO_A_RSTB         |
 | bit  0  R/W  DIGITAL_RSTB               |
 +----------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_UNUSED_MASK                    0xffffe000
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_B_RSTB_MSK   0x00001000
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_B_RSTB_OFF   12
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_A_RSTB_MSK   0x00000800
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PSLB_CLK_DEGLITCH_A_RSTB_OFF   11
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_B_RSTB_MSK 0x00000400
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_B_RSTB_OFF 10
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_A_RSTB_MSK 0x00000200
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_TPCOUT_CLK_DEGLITCH_A_RSTB_OFF 9
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_B_RSTB_MSK            0x00000100
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_B_RSTB_OFF            8
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_A_RSTB_MSK            0x00000080
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_MON_A_RSTB_OFF            7
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_B_RSTB_MSK            0x00000040
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_B_RSTB_OFF            6
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_A_RSTB_MSK            0x00000020
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_PATT_GEN_A_RSTB_OFF            5
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_B_RSTB_MSK            0x00000010
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_B_RSTB_OFF            4
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_A_RSTB_MSK            0x00000008
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_GEN_PISO_A_RSTB_OFF            3
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_B_RSTB_MSK         0x00000004
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_B_RSTB_OFF         2
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_A_RSTB_MSK         0x00000002
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DESKEW_FIFO_A_RSTB_OFF         1
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_MSK               0x00000001
#define SFI51_TX_2X_SLICE_REG_SW_RESET_CTRL_BIT_DIGITAL_RSTB_OFF               0

/*--------------------------------------.
 | Register 0x00000004 ANALOG_TEST_CTRL |
 +--------------------------------------+
 | bit  13:12 R/W  TX_JTAG_IN           |
 | bit  11:10 R/W  TX_JTAG_EN           |
 | bit  8     R/W  DLB_SLICE_EN         |
 | bit  6     R/W  DLB_CH_SEL           |
 | bit  5     R/W  TS_ATMSB             |
 | bit  4:0   R/W  ATIN                 |
 +-------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_UNUSED_MASK      0xffffc280
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_MSK   0x00003000
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_IN_OFF   12
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_MSK   0x00000c00
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TX_JTAG_EN_OFF   10
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_SLICE_EN_MSK 0x00000100
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_SLICE_EN_OFF 8
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_CH_SEL_MSK   0x00000040
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_DLB_CH_SEL_OFF   6
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TS_ATMSB_MSK     0x00000020
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_TS_ATMSB_OFF     5
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_MSK         0x0000001f
#define SFI51_TX_2X_SLICE_REG_ANALOG_TEST_CTRL_BIT_ATIN_OFF         0

/*------------------------------.
 | Register 0x00000008 INT_REG  |
 +------------------------------+
 | bit  1 R/W  PATT_MON_ERR_B_I |
 | bit  0 R/W  PATT_MON_ERR_A_I |
 +-----------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_INT_REG_UNUSED_MASK          0xfffffffc
#define SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_MSK 0x00000002
#define SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_B_I_OFF 1
#define SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_MSK 0x00000001
#define SFI51_TX_2X_SLICE_REG_INT_REG_BIT_PATT_MON_ERR_A_I_OFF 0

/*--------------------------------.
 | Register 0x0000000c INT_EN_REG |
 +--------------------------------+
 | bit  1 R/W  PATT_MON_ERR_B_E   |
 | bit  0 R/W  PATT_MON_ERR_A_E   |
 +-------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_INT_EN_REG_UNUSED_MASK          0xfffffffc
#define SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_B_E_MSK 0x00000002
#define SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_B_E_OFF 1
#define SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_A_E_MSK 0x00000001
#define SFI51_TX_2X_SLICE_REG_INT_EN_REG_BIT_PATT_MON_ERR_A_E_OFF 0

/*----------------------------------.
 | Register 0x00000010 INT_STAT_REG |
 +----------------------------------+
 | bit  1 R  PATT_MON_ERR_B_V       |
 | bit  0 R  PATT_MON_ERR_A_V       |
 +---------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_INT_STAT_REG_UNUSED_MASK          0xfffffffc
#define SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_B_V_MSK 0x00000002
#define SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_B_V_OFF 1
#define SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_A_V_MSK 0x00000001
#define SFI51_TX_2X_SLICE_REG_INT_STAT_REG_BIT_PATT_MON_ERR_A_V_OFF 0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1 */
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_INDEX_N_MIN            0
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_INDEX_N_MAX            1
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_INDEX_N_SIZE           2
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_INDEX_N_OFFSET         0x4

/*-----------------------------------------------------------.
 | Register (0x00000014 + (N) * 0x4) SFI5_1_TX_2X_MABC_CFG_1 |
 +-----------------------------------------------------------+
 | bit  17    R/W  TX_SQUELCH_OVR                            |
 | bit  16    R/W  TX_SQUELCH_OVR_EN                         |
 | bit  15:11 R/W  TX_SWING                                  |
 | bit  8     R/W  TX_ATMSB                                  |
 | bit  4     R/W  TX_TEST_EN                                |
 | bit  3     R/W  TX_PISO_PBUS_WIDTH                        |
 | bit  2     R/W  PISO_RSTB                                 |
 | bit  1     R/W  PISO_ENB                                  |
 | bit  0     R/W  TX_ENB                                    |
 +----------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_UNUSED_MASK            0xfffc06e0
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_MSK     0x00020000
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_OFF     17
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_EN_MSK  0x00010000
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SQUELCH_OVR_EN_OFF  16
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_MSK           0x0000f800
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_SWING_OFF           11
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ATMSB_MSK           0x00000100
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ATMSB_OFF           8
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_TEST_EN_MSK         0x00000010
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_TEST_EN_OFF         4
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_PISO_PBUS_WIDTH_MSK 0x00000008
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_PISO_PBUS_WIDTH_OFF 3
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_RSTB_MSK          0x00000004
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_RSTB_OFF          2
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_ENB_MSK           0x00000002
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_PISO_ENB_OFF           1
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ENB_MSK             0x00000001
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_1_BIT_TX_ENB_OFF             0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2 */
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_INDEX_N_MIN    0
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_INDEX_N_MAX    1
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_INDEX_N_SIZE   2
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------.
 | Register (0x0000001c + (N) * 0x4) SFI5_1_TX_2X_MABC_CFG_2 |
 +-----------------------------------------------------------+
 | bit  31:8 R/W  TX_MODE                                    |
 | bit  7:0  R/W  TX_CTRL                                    |
 +----------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_UNUSED_MASK    0x00000000
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_MSK    0xffffff00
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_MODE_OFF    8
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_MSK    0x000000ff
#define SFI51_TX_2X_SLICE_REG_SFI5_1_TX_2X_MABC_CFG_2_BIT_TX_CTRL_OFF    0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG */
#define PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_INDEX_N_MIN         0
#define PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_INDEX_N_MAX         1
#define PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_INDEX_N_SIZE        2
#define PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_INDEX_N_OFFSET      0x4

/*---------------------------------------------------.
 | Register (0x00000024 + (N) * 0x4) DESKEW_FIFO_CFG |
 +---------------------------------------------------+
 | bit  7:4 R/W  RD_PTR_SYNC_VAL                     |
 | bit  0   R/W  RD_PTR_SYNC                         |
 +--------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_UNUSED_MASK         0xffffff0e
#define SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_MSK 0x000000f0
#define SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_VAL_OFF 4
#define SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_MSK     0x00000001
#define SFI51_TX_2X_SLICE_REG_DESKEW_FIFO_CFG_BIT_RD_PTR_SYNC_OFF     0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG */
#define PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_INDEX_N_MIN                0
#define PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_INDEX_N_MAX                1
#define PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_INDEX_N_SIZE               2
#define PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_INDEX_N_OFFSET             0x4

/*------------------------------------------------.
 | Register (0x0000002c + (N) * 0x4) DATAPATH_CFG |
 +------------------------------------------------+
 | bit  10  R/W  INV_DAT_POL                      |
 | bit  9   R/W  TCLKOUT_SEL                      |
 | bit  8   R/W  PSLB_CLK_DEGLITCH_EN             |
 | bit  7   R/W  TPCOUT_CLK_DEGLITCH_EN           |
 | bit  6   R/W  PATT_INSERT_EN                   |
 | bit  5   R/W  P2S_DLB_EN                       |
 | bit  4   R/W  RX2TX_DLB_EN                     |
 | bit  3   R/W  GEN_PISO_BUS_WIDTH               |
 | bit  2:0 R/W  GEN_PISO_DIV_RATIO               |
 +-----------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_UNUSED_MASK                0xfffff800
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_INV_DAT_POL_MSK            0x00000400
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_INV_DAT_POL_OFF            10
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TCLKOUT_SEL_MSK            0x00000200
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TCLKOUT_SEL_OFF            9
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PSLB_CLK_DEGLITCH_EN_MSK   0x00000100
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PSLB_CLK_DEGLITCH_EN_OFF   8
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TPCOUT_CLK_DEGLITCH_EN_MSK 0x00000080
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_TPCOUT_CLK_DEGLITCH_EN_OFF 7
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PATT_INSERT_EN_MSK         0x00000040
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_PATT_INSERT_EN_OFF         6
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_P2S_DLB_EN_MSK             0x00000020
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_P2S_DLB_EN_OFF             5
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_RX2TX_DLB_EN_MSK           0x00000010
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_RX2TX_DLB_EN_OFF           4
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_BUS_WIDTH_MSK     0x00000008
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_BUS_WIDTH_OFF     3
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_MSK     0x00000007
#define SFI51_TX_2X_SLICE_REG_DATAPATH_CFG_BIT_GEN_PISO_DIV_RATIO_OFF     0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_INDEX_N_MIN           0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_INDEX_N_MAX           1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_INDEX_N_SIZE          2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------------.
 | Register (0x00000034 + (N) * 0x4) PATTERN_GENERATOR_CFG_1 |
 +-----------------------------------------------------------+
 | bit  13:8 R/W  GEN_MON_BUS_WIDTH                          |
 | bit  7:4  R/W  GEN_MODE                                   |
 | bit  2    R/W  GEN_ERR_INJ                                |
 | bit  1    R/W  GEN_INV                                    |
 | bit  0    R/W  GEN_EN                                     |
 +----------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_UNUSED_MASK           0xffffc008
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_MSK 0x00003f00
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MON_BUS_WIDTH_OFF 8
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_MSK          0x000000f0
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_MODE_OFF          4
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_ERR_INJ_MSK       0x00000004
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_ERR_INJ_OFF       2
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_INV_MSK           0x00000002
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_INV_OFF           1
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_EN_MSK            0x00000001
#define SFI51_TX_2X_SLICE_REG_PATTERN_GENERATOR_CFG_1_BIT_GEN_EN_OFF            0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_INDEX_N_MIN         0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_INDEX_N_MAX         1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_INDEX_N_SIZE        2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_INDEX_N_OFFSET      0x4

/*--------------------------------------------------------------.
 | Register (0x0000003c + (N) * 0x4) PATTERN_GEN_USER_PATTERN_1 |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  GEN_USR_PAT_LSB                               |
 +-------------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_UNUSED_MASK         0x00000000
#define SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_BIT_GEN_USR_PAT_LSB_MSK 0xffffffff
#define SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_1_BIT_GEN_USR_PAT_LSB_OFF 0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_INDEX_N_MIN         0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_INDEX_N_MAX         1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_INDEX_N_SIZE        2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_INDEX_N_OFFSET      0x4

/*--------------------------------------------------------------.
 | Register (0x00000044 + (N) * 0x4) PATTERN_GEN_USER_PATTERN_2 |
 +--------------------------------------------------------------+
 | bit  7:0 R/W  GEN_USR_PAT_MSB                                |
 +-------------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_UNUSED_MASK         0xffffff00
#define SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_MSK 0x000000ff
#define SFI51_TX_2X_SLICE_REG_PATTERN_GEN_USER_PATTERN_2_BIT_GEN_USR_PAT_MSB_OFF 0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_INDEX_N_MIN    0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_INDEX_N_MAX    1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_INDEX_N_SIZE   2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------.
 | Register (0x0000004c + (N) * 0x4) PATTERN_MON_CFG_1 |
 +-----------------------------------------------------+
 | bit  7:4 R/W  MON_MODE                              |
 | bit  2   R/W  MON_READ                              |
 | bit  1   R/W  MON_INV                               |
 | bit  0   R/W  MON_EN                                |
 +----------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_UNUSED_MASK    0xffffff08
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_MSK   0x000000f0
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_MODE_OFF   4
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_READ_MSK   0x00000004
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_READ_OFF   2
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_INV_MSK    0x00000002
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_INV_OFF    1
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_EN_MSK     0x00000001
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_CFG_1_BIT_MON_EN_OFF     0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_INDEX_N_MIN         0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_INDEX_N_MAX         1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_INDEX_N_SIZE        2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_INDEX_N_OFFSET      0x4

/*--------------------------------------------------------------.
 | Register (0x00000054 + (N) * 0x4) PATTERN_MON_USER_PATTERN_1 |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  MON_USR_PAT_LSB                               |
 +-------------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_UNUSED_MASK         0x00000000
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_BIT_MON_USR_PAT_LSB_MSK 0xffffffff
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_1_BIT_MON_USR_PAT_LSB_OFF 0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_INDEX_N_MIN         0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_INDEX_N_MAX         1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_INDEX_N_SIZE        2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_INDEX_N_OFFSET      0x4

/*--------------------------------------------------------------.
 | Register (0x0000005c + (N) * 0x4) PATTERN_MON_USER_PATTERN_2 |
 +--------------------------------------------------------------+
 | bit  7:0 R/W  MON_USR_PAT_MSB                                |
 +-------------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_UNUSED_MASK         0xffffff00
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_MSK 0x000000ff
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_USER_PATTERN_2_BIT_MON_USR_PAT_MSB_OFF 0

/* index definitions for PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1 */
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_INDEX_N_MIN     0
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_INDEX_N_MAX     1
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_INDEX_N_SIZE    2
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_INDEX_N_OFFSET  0x4

/*---------------------------------------------------------.
 | Register (0x00000064 + (N) * 0x4) PATTERN_MON_ERR_CNT_1 |
 +---------------------------------------------------------+
 | bit  31:0 R  MON_ERR_CNT                                |
 +--------------------------------------------------------*/
#define PMC_SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_UNUSED_MASK     0x00000000
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_BIT_MON_ERR_CNT_MSK 0xffffffff
#define SFI51_TX_2X_SLICE_REG_PATTERN_MON_ERR_CNT_1_BIT_MON_ERR_CNT_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _D8_TX_2X_REGS_H */
