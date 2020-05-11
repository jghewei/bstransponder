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
 *     otn_mld block
 *****************************************************************************/
#ifndef _OTN_MLD_REGS_H
#define _OTN_MLD_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_OTN_MLD_REG_MASTER_CONFIG                0x00010000
#define PMC_OTN_MLD_REG_LOWPWR_CONFIG                0x00010004
#define PMC_OTN_MLD_REG_LANE_CONFIG( N )             (0x00010008 + (N) * 0x04)
#define PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG( N ) (0x00010038 + (N) * 0x04)
#define PMC_OTN_MLD_REG_MUX0_CONFIG                  0x00010068
#define PMC_OTN_MLD_REG_MUX1_CONFIG                  0x0001006c
#define PMC_OTN_MLD_REG_MUX2_CONFIG                  0x00010070
#define PMC_OTN_MLD_REG_MUX3_CONFIG                  0x00010074
#define PMC_OTN_MLD_REG_MUX4_CONFIG                  0x00010078
#define PMC_OTN_MLD_REG_MUX5_CONFIG                  0x0001007c
#define PMC_OTN_MLD_REG_MUX6_CONFIG                  0x00010080
#define PMC_OTN_MLD_REG_MUX7_CONFIG                  0x00010084
#define PMC_OTN_MLD_REG_MUX8_CONFIG                  0x00010088
#define PMC_OTN_MLD_REG_MUX9_CONFIG                  0x0001008c
#define PMC_OTN_MLD_REG_MUX10_CONFIG                 0x00010090
#define PMC_OTN_MLD_REG_MUX11_CONFIG                 0x00010094
#define PMC_OTN_MLD_REG_MUX12_CONFIG                 0x00010098
#define PMC_OTN_MLD_REG_MUX13_CONFIG                 0x0001009c
#define PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG( N )  (0x000100a0 + (N) * 0x04)
#define PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG( N )  (0x000100ac + (N) * 0x4)
#define PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING( N )   (0x000100dc + (N) * 0x4)
#define PMC_OTN_MLD_REG_INT_SUMMARY                  0x0001010c
#define PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY           0x00010110
#define PMC_OTN_MLD_REG_RINGOSC_32_CONFIG( N )       (0x00010114 + (N) * 0x4)
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG1( N )        (0x000101a4 + (N) * 0x4)
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG2( N )        (0x000101d4 + (N) * 0x4)
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG3( N )        (0x00010204 + (N) * 0x4)
#define PMC_OTN_MLD_REG_HADAPT40G0_CONFIG            0x00010318
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE0             0x0001031c
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE1             0x00010320
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE2             0x00010324
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE3             0x00010328
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE4             0x0001032c
#define PMC_OTN_MLD_REG_HADAPT40G0_INT_EN            0x00010330
#define PMC_OTN_MLD_REG_HADAPT40G0_INT               0x00010334
#define PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL           0x00010338
#define PMC_OTN_MLD_REG_HADAPT40G0_STAT              0x0001033c
#define PMC_OTN_MLD_REG_HADAPT40G1_CONFIG            0x00010340
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE0             0x00010344
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE1             0x00010348
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE2             0x0001034c
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE3             0x00010350
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE4             0x00010354
#define PMC_OTN_MLD_REG_HADAPT40G1_INT_EN            0x00010358
#define PMC_OTN_MLD_REG_HADAPT40G1_INT               0x0001035c
#define PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL           0x00010360
#define PMC_OTN_MLD_REG_HADAPT40G1_STAT              0x00010364
#define PMC_OTN_MLD_REG_HADAPT100G_CONFIG            0x00010368
#define PMC_OTN_MLD_REG_HADAPT100G_MODE0             0x0001036c
#define PMC_OTN_MLD_REG_HADAPT100G_MODE1             0x00010370
#define PMC_OTN_MLD_REG_HADAPT100G_MODE2             0x00010374
#define PMC_OTN_MLD_REG_HADAPT100G_MODE3             0x00010378
#define PMC_OTN_MLD_REG_HADAPT100G_MODE4             0x0001037c
#define PMC_OTN_MLD_REG_HADAPT100G_MODE5             0x00010380
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN0          0x00010384
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN1          0x00010388
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN2          0x0001038c
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN3          0x00010390
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN4          0x00010394
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN5          0x00010398
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN6          0x0001039c
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN7          0x000103a0
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN8          0x000103a4
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN9          0x000103a8
#define PMC_OTN_MLD_REG_HADAPT100G_INT_EN            0x000103ac
#define PMC_OTN_MLD_REG_HADAPT100G_INT               0x000103b0
#define PMC_OTN_MLD_REG_HADAPT100G_INT_VAL           0x000103b4
#define PMC_OTN_MLD_REG_HADAPT100G_STAT              0x000103b8

/*-----------------------------------.
 | Register 0x00010000 MASTER_CONFIG |
 +-----------------------------------+
 |                                   |
 | bit  6   R/W  SFIS_N10_ENABLE_N5  |
 |                                   |
 | bit  4   R/W  OTU4_SEL            |
 |                                   |
 | bit  3:2 R/W  HADAPT40G_RESET     |
 |                                   |
 |                                   |
 | bit  1   R/W  HADAPT100G_RESET    |
 |                                   |
 | bit  0   R/W  MLD_TOP_RESET       |
 +----------------------------------*/
#define PMC_OTN_MLD_REG_MASTER_CONFIG_UNUSED_MASK            0xffffff80
#define OTN_MLD_REG_MASTER_CONFIG_BIT_SFIS_N10_ENABLE_N5_MSK 0x00000040
#define OTN_MLD_REG_MASTER_CONFIG_BIT_SFIS_N10_ENABLE_N5_OFF 6
#define OTN_MLD_REG_MASTER_CONFIG_BIT_OTU4_SEL_MSK           0x00000010
#define OTN_MLD_REG_MASTER_CONFIG_BIT_OTU4_SEL_OFF           4
#define OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_MSK    0x0000000c
#define OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT40G_RESET_OFF    2
#define OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT100G_RESET_MSK   0x00000002
#define OTN_MLD_REG_MASTER_CONFIG_BIT_HADAPT100G_RESET_OFF   1
#define OTN_MLD_REG_MASTER_CONFIG_BIT_MLD_TOP_RESET_MSK      0x00000001
#define OTN_MLD_REG_MASTER_CONFIG_BIT_MLD_TOP_RESET_OFF      0

/*-----------------------------------.
 | Register 0x00010004 LOWPWR_CONFIG |
 +-----------------------------------+
 | bit  13:12 R/W  SFI51_LOWPWR      |
 | bit  11:8  R/W  SFIS_LOWPWR       |
 |                                   |
 | bit  7:6   R/W  HADAPT40G_LOWPWR  |
 |                                   |
 |                                   |
 | bit  5     R/W  ADAPT100G_LOWPWR  |
 |                                   |
 | bit  4:2   R/W  OPSM23_LOWPWR     |
 | bit  1     R/W  OPSM4_LOWPWR      |
 | bit  0     R/W  MLD_LOWPWR        |
 +----------------------------------*/
#define PMC_OTN_MLD_REG_LOWPWR_CONFIG_UNUSED_MASK          0xffffc000
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_MSK     0x00003000
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFI51_LOWPWR_OFF     12
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_MSK      0x00000f00
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_SFIS_LOWPWR_OFF      8
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_MSK 0x000000c0
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_HADAPT40G_LOWPWR_OFF 6
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_ADAPT100G_LOWPWR_MSK 0x00000020
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_ADAPT100G_LOWPWR_OFF 5
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_MSK    0x0000001c
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM23_LOWPWR_OFF    2
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM4_LOWPWR_MSK     0x00000002
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_OPSM4_LOWPWR_OFF     1
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_MLD_LOWPWR_MSK       0x00000001
#define OTN_MLD_REG_LOWPWR_CONFIG_BIT_MLD_LOWPWR_OFF       0

/* index definitions for PMC_OTN_MLD_REG_LANE_CONFIG */
#define PMC_OTN_MLD_REG_LANE_CONFIG_INDEX_N_MIN               0
#define PMC_OTN_MLD_REG_LANE_CONFIG_INDEX_N_MAX               11
#define PMC_OTN_MLD_REG_LANE_CONFIG_INDEX_N_SIZE              12
#define PMC_OTN_MLD_REG_LANE_CONFIG_INDEX_N_OFFSET            0x04

/*------------------------------------------------.
 | Register (0x00010008 + (N) * 0x04) LANE_CONFIG |
 +------------------------------------------------+
 | bit  2 R/W  PRECODER_32_RX_BYPASS              |
 | bit  1 R/W  PRECODER_32_TX_BYPASS              |
 | bit  0 R/W  BERGEN_32_BYPASS                   |
 +-----------------------------------------------*/
#define PMC_OTN_MLD_REG_LANE_CONFIG_UNUSED_MASK               0xfffffff8
#define OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_RX_BYPASS_MSK 0x00000004
#define OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_RX_BYPASS_OFF 2
#define OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_TX_BYPASS_MSK 0x00000002
#define OTN_MLD_REG_LANE_CONFIG_BIT_PRECODER_32_TX_BYPASS_OFF 1
#define OTN_MLD_REG_LANE_CONFIG_BIT_BERGEN_32_BYPASS_MSK      0x00000001
#define OTN_MLD_REG_LANE_CONFIG_BIT_BERGEN_32_BYPASS_OFF      0

/* index definitions for PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG */
#define PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_INDEX_N_MIN         0
#define PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_INDEX_N_MAX         11
#define PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_INDEX_N_SIZE        12
#define PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_INDEX_N_OFFSET      0x04

/*------------------------------------------------------------.
 | Register (0x00010038 + (N) * 0x04) DISTRIBUTION_MUX_CONFIG |
 +------------------------------------------------------------+
 | bit  7:0 R/W  LANE_10G_TX_SRC                              |
 +-----------------------------------------------------------*/
#define PMC_OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_UNUSED_MASK         0xffffff00
#define OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_MSK 0x000000ff
#define OTN_MLD_REG_DISTRIBUTION_MUX_CONFIG_BIT_LANE_10G_TX_SRC_OFF 0

/*---------------------------------.
 | Register 0x00010068 MUX0_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX0_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX0_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX0_CONFIG_BIT_MUX0_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x0001006c MUX1_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX1_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX1_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX1_CONFIG_BIT_MUX1_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x00010070 MUX2_CONFIG |
 +---------------------------------+
 | bit  3:1 R/W  MUX2_SYS_STREAM   |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX2_CONFIG_UNUSED_MASK         0xfffffff1
#define OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_MSK 0x0000000e
#define OTN_MLD_REG_MUX2_CONFIG_BIT_MUX2_SYS_STREAM_OFF 1

/*----------------------------------.
 | Register 0x00010074 MUX3_CONFIG  |
 +----------------------------------+
 |                                  |
 | bit  1:0 R/W  MUX3_LINERX_STREAM |
 |                                  |
 +---------------------------------*/
#define PMC_OTN_MLD_REG_MUX3_CONFIG_UNUSED_MASK            0xfffffffc
#define OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX3_CONFIG_BIT_MUX3_LINERX_STREAM_OFF 0

/*---------------------------------.
 | Register 0x00010078 MUX4_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX4_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX4_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX4_CONFIG_BIT_MUX4_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x0001007c MUX5_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX5_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX5_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX5_CONFIG_BIT_MUX5_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x00010080 MUX6_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX6_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX6_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX6_CONFIG_BIT_MUX6_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x00010084 MUX7_CONFIG |
 +---------------------------------+
 | bit  4:2 R/W  MUX7_SYS_STREAM   |
 | bit  1:0 R/W  MUX7_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX7_CONFIG_UNUSED_MASK          0xffffffe0
#define OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_MSK  0x0000001c
#define OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_SYS_STREAM_OFF  2
#define OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX7_CONFIG_BIT_MUX7_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x00010088 MUX8_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX8_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX8_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX8_CONFIG_BIT_MUX8_LINE_STREAM_OFF 0

/*---------------------------------.
 | Register 0x0001008c MUX9_CONFIG |
 +---------------------------------+
 | bit  1:0 R/W  MUX9_LINE_STREAM  |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_MUX9_CONFIG_UNUSED_MASK          0xfffffffc
#define OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX9_CONFIG_BIT_MUX9_LINE_STREAM_OFF 0

/*----------------------------------.
 | Register 0x00010090 MUX10_CONFIG |
 +----------------------------------+
 | bit  1:0 R/W  MUX10_LINE_STREAM  |
 +---------------------------------*/
#define PMC_OTN_MLD_REG_MUX10_CONFIG_UNUSED_MASK           0xfffffffc
#define OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX10_CONFIG_BIT_MUX10_LINE_STREAM_OFF 0

/*----------------------------------.
 | Register 0x00010094 MUX11_CONFIG |
 +----------------------------------+
 | bit  4:2 R/W  MUX11_SYS_STREAM   |
 | bit  1:0 R/W  MUX11_LINE_STREAM  |
 +---------------------------------*/
#define PMC_OTN_MLD_REG_MUX11_CONFIG_UNUSED_MASK           0xffffffe0
#define OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_MSK  0x0000001c
#define OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_SYS_STREAM_OFF  2
#define OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX11_CONFIG_BIT_MUX11_LINE_STREAM_OFF 0

/*----------------------------------.
 | Register 0x00010098 MUX12_CONFIG |
 +----------------------------------+
 | bit  1:0 R/W  MUX12_LINE_STREAM  |
 +---------------------------------*/
#define PMC_OTN_MLD_REG_MUX12_CONFIG_UNUSED_MASK           0xfffffffc
#define OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_MSK 0x00000003
#define OTN_MLD_REG_MUX12_CONFIG_BIT_MUX12_LINE_STREAM_OFF 0

/*----------------------------------.
 | Register 0x0001009c MUX13_CONFIG |
 +----------------------------------+
 | bit  3:1 R/W  MUX13_SYS_STREAM   |
 +---------------------------------*/
#define PMC_OTN_MLD_REG_MUX13_CONFIG_UNUSED_MASK          0xfffffff1
#define OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_MSK 0x0000000e
#define OTN_MLD_REG_MUX13_CONFIG_BIT_MUX13_SYS_STREAM_OFF 1

/* index definitions for PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG */
#define PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_INDEX_N_MIN                0
#define PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_INDEX_N_MAX                2
#define PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_INDEX_N_SIZE               3
#define PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_INDEX_N_OFFSET             0x04

/*-----------------------------------------------------------.
 | Register (0x000100a0 + (N) * 0x04) OPSM23_SFI_PATH_CONFIG |
 +-----------------------------------------------------------+
 | bit  2 R/W  PRECODER_128_RX_BYPASS                        |
 | bit  1 R/W  PRECODER_128_TX_BYPASS                        |
 +----------------------------------------------------------*/
#define PMC_OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_UNUSED_MASK                0xfffffff9
#define OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_RX_BYPASS_MSK 0x00000004
#define OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_RX_BYPASS_OFF 2
#define OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_TX_BYPASS_MSK 0x00000002
#define OTN_MLD_REG_OPSM23_SFI_PATH_CONFIG_BIT_PRECODER_128_TX_BYPASS_OFF 1

/* index definitions for PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG */
#define PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_INDEX_N_MIN         0
#define PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_INDEX_N_MAX         11
#define PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_INDEX_N_SIZE        12
#define PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_INDEX_N_OFFSET      0x4

/*----------------------------------------------------------.
 | Register (0x000100ac + (N) * 0x4) DEFECT_RCP_CHNL_CONFIG |
 +----------------------------------------------------------+
 |                                                          |
 | bit  3:0 R/W  DEFECT_RCP_CHNL                            |
 |                                                          |
 +---------------------------------------------------------*/
#define PMC_OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_UNUSED_MASK         0xfffffff0
#define OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_MSK 0x0000000f
#define OTN_MLD_REG_DEFECT_RCP_CHNL_CONFIG_BIT_DEFECT_RCP_CHNL_OFF 0

/* index definitions for PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING */
#define PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_INDEX_N_MIN               0
#define PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_INDEX_N_MAX               11
#define PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_INDEX_N_SIZE              12
#define PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_INDEX_N_OFFSET            0x4

/*---------------------------------------------------------.
 | Register (0x000100dc + (N) * 0x4) DLOM_TO_FFAIL_MAPPING |
 +---------------------------------------------------------+
 | bit  0 R/W  DLOM_TO_FFAIL_MAPPING                       |
 +--------------------------------------------------------*/
#define PMC_OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_UNUSED_MASK               0xfffffffe
#define OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_BIT_DLOM_TO_FFAIL_MAPPING_MSK 0x00000001
#define OTN_MLD_REG_DLOM_TO_FFAIL_MAPPING_BIT_DLOM_TO_FFAIL_MAPPING_OFF 0

/*---------------------------------.
 | Register 0x0001010c INT_SUMMARY |
 +---------------------------------+
 | bit  10:0 R  INT_SUMMARY        |
 +--------------------------------*/
#define PMC_OTN_MLD_REG_INT_SUMMARY_UNUSED_MASK     0xfffff800
#define OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_MSK 0x000007ff
#define OTN_MLD_REG_INT_SUMMARY_BIT_INT_SUMMARY_OFF 0

/*----------------------------------------.
 | Register 0x00010110 HADAPT_INT_SUMMARY |
 +----------------------------------------+
 |                                        |
 | bit  2 R  HADAPT100G_INT_SUMMARY       |
 |                                        |
 |                                        |
 | bit  1 R  HADAPT40G1_INT_SUMMARY       |
 |                                        |
 |                                        |
 | bit  0 R  HADAPT40G0_INT_SUMMARY       |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT_INT_SUMMARY_UNUSED_MASK                0xfffffff8
#define OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT100G_INT_SUMMARY_MSK 0x00000004
#define OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT100G_INT_SUMMARY_OFF 2
#define OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G1_INT_SUMMARY_MSK 0x00000002
#define OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G1_INT_SUMMARY_OFF 1
#define OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G0_INT_SUMMARY_MSK 0x00000001
#define OTN_MLD_REG_HADAPT_INT_SUMMARY_BIT_HADAPT40G0_INT_SUMMARY_OFF 0

/* index definitions for PMC_OTN_MLD_REG_RINGOSC_32_CONFIG */
#define PMC_OTN_MLD_REG_RINGOSC_32_CONFIG_INDEX_N_MIN           0
#define PMC_OTN_MLD_REG_RINGOSC_32_CONFIG_INDEX_N_MAX           35
#define PMC_OTN_MLD_REG_RINGOSC_32_CONFIG_INDEX_N_SIZE          36
#define PMC_OTN_MLD_REG_RINGOSC_32_CONFIG_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------.
 | Register (0x00010114 + (N) * 0x4) RINGOSC_32_CONFIG |
 +-----------------------------------------------------+
 | bit  1 R/W  RINGOSC_32_BYPASS                       |
 | bit  0 R/W  RINGOSC_32_ENABLE                       |
 +----------------------------------------------------*/
#define PMC_OTN_MLD_REG_RINGOSC_32_CONFIG_UNUSED_MASK           0xfffffffc
#define OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_BYPASS_MSK 0x00000002
#define OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_BYPASS_OFF 1
#define OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_ENABLE_MSK 0x00000001
#define OTN_MLD_REG_RINGOSC_32_CONFIG_BIT_RINGOSC_32_ENABLE_OFF 0

/* index definitions for PMC_OTN_MLD_REG_BERGEN32_CONFIG1 */
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG1_INDEX_N_MIN                        0
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG1_INDEX_N_MAX                        11
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG1_INDEX_N_SIZE                       12
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG1_INDEX_N_OFFSET                     0x4

/*----------------------------------------------------.
 | Register (0x000101a4 + (N) * 0x4) BERGEN32_CONFIG1 |
 +----------------------------------------------------+
 | bit  31:8 R/W  BERGEN32_BER_IN                     |
 | bit  7    R/W  BERGEN32_USE_BITLANE_ERR_LIMIT      |
 | bit  6:3  R/W  BERGEN32_BITLANE_ERR_LIMIT          |
 | bit  2:1  R/W  BERGEN32_ERR_MODE                   |
 | bit  0    R/W  BERGEN32_ENABLE                     |
 +---------------------------------------------------*/
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG1_UNUSED_MASK                        0x00000000
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_MSK                0xffffff00
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BER_IN_OFF                8
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_USE_BITLANE_ERR_LIMIT_MSK 0x00000080
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_USE_BITLANE_ERR_LIMIT_OFF 7
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_MSK     0x00000078
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_BITLANE_ERR_LIMIT_OFF     3
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_MSK              0x00000006
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ERR_MODE_OFF              1
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ENABLE_MSK                0x00000001
#define OTN_MLD_REG_BERGEN32_CONFIG1_BIT_BERGEN32_ENABLE_OFF                0

/* index definitions for PMC_OTN_MLD_REG_BERGEN32_CONFIG2 */
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG2_INDEX_N_MIN                 0
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG2_INDEX_N_MAX                 11
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG2_INDEX_N_SIZE                12
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG2_INDEX_N_OFFSET              0x4

/*----------------------------------------------------.
 | Register (0x000101d4 + (N) * 0x4) BERGEN32_CONFIG2 |
 +----------------------------------------------------+
 | bit  31:0 R/W  BERGEN32_ENABLE_BITLANE             |
 +---------------------------------------------------*/
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG2_UNUSED_MASK                 0x00000000
#define OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_MSK 0xffffffff
#define OTN_MLD_REG_BERGEN32_CONFIG2_BIT_BERGEN32_ENABLE_BITLANE_OFF 0

/* index definitions for PMC_OTN_MLD_REG_BERGEN32_CONFIG3 */
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG3_INDEX_N_MIN    0
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG3_INDEX_N_MAX    11
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG3_INDEX_N_SIZE   12
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG3_INDEX_N_OFFSET 0x4

/*----------------------------------------------------.
 | Register (0x00010204 + (N) * 0x4) BERGEN32_CONFIG3 |
 +----------------------------------------------------+
 +---------------------------------------------------*/
#define PMC_OTN_MLD_REG_BERGEN32_CONFIG3_UNUSED_MASK    0xff000000

/*----------------------------------------.
 | Register 0x00010318 HADAPT40G0_CONFIG  |
 +----------------------------------------+
 |                                        |
 | bit  1 R/W  HADAPT40G0_DPQPSK_MODE     |
 |                                        |
 |                                        |
 | bit  0 R/W  HADAPT40G0_PMC_TX_DATA_SEL |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_CONFIG_UNUSED_MASK                    0xfffffffc
#define OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_DPQPSK_MODE_MSK     0x00000002
#define OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_DPQPSK_MODE_OFF     1
#define OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_PMC_TX_DATA_SEL_MSK 0x00000001
#define OTN_MLD_REG_HADAPT40G0_CONFIG_BIT_HADAPT40G0_PMC_TX_DATA_SEL_OFF 0

/*--------------------------------------.
 | Register 0x0001031c HADAPT40G0_MODE0 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G0_MODE0      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE0_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_MODE0_BIT_HADAPT40G0_MODE0_OFF 0

/*--------------------------------------.
 | Register 0x00010320 HADAPT40G0_MODE1 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G0_MODE1      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE1_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_MODE1_BIT_HADAPT40G0_MODE1_OFF 0

/*--------------------------------------.
 | Register 0x00010324 HADAPT40G0_MODE2 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G0_MODE2      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE2_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_MODE2_BIT_HADAPT40G0_MODE2_OFF 0

/*--------------------------------------.
 | Register 0x00010328 HADAPT40G0_MODE3 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G0_MODE3      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE3_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_MODE3_BIT_HADAPT40G0_MODE3_OFF 0

/*--------------------------------------.
 | Register 0x0001032c HADAPT40G0_MODE4 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G0_MODE4      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_MODE4_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_MODE4_BIT_HADAPT40G0_MODE4_OFF 0

/*---------------------------------------.
 | Register 0x00010330 HADAPT40G0_INT_EN |
 +---------------------------------------+
 |                                       |
 | bit  31:0 R/W  HADAPT40G0_INT_E       |
 |                                       |
 +--------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_INT_EN_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_INT_EN_BIT_HADAPT40G0_INT_E_OFF 0

/*------------------------------------.
 | Register 0x00010334 HADAPT40G0_INT |
 +------------------------------------+
 |                                    |
 | bit  31:0 R/W  HADAPT40G0_INT_I    |
 |                                    |
 +-----------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_INT_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_INT_BIT_HADAPT40G0_INT_I_OFF 0

/*----------------------------------------.
 | Register 0x00010338 HADAPT40G0_INT_VAL |
 +----------------------------------------+
 |                                        |
 | bit  31:0 R  HADAPT40G0_INT_V          |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_INT_VAL_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_INT_VAL_BIT_HADAPT40G0_INT_V_OFF 0

/*-------------------------------------.
 | Register 0x0001033c HADAPT40G0_STAT |
 +-------------------------------------+
 |                                     |
 | bit  31:0 R  HADAPT40G0_STATUS      |
 |                                     |
 +------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G0_STAT_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G0_STAT_BIT_HADAPT40G0_STATUS_OFF 0

/*----------------------------------------.
 | Register 0x00010340 HADAPT40G1_CONFIG  |
 +----------------------------------------+
 |                                        |
 | bit  1 R/W  HADAPT40G1_DPQPSK_MODE     |
 |                                        |
 |                                        |
 | bit  0 R/W  HADAPT40G1_PMC_TX_DATA_SEL |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_CONFIG_UNUSED_MASK                    0xfffffffc
#define OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_DPQPSK_MODE_MSK     0x00000002
#define OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_DPQPSK_MODE_OFF     1
#define OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_PMC_TX_DATA_SEL_MSK 0x00000001
#define OTN_MLD_REG_HADAPT40G1_CONFIG_BIT_HADAPT40G1_PMC_TX_DATA_SEL_OFF 0

/*--------------------------------------.
 | Register 0x00010344 HADAPT40G1_MODE0 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G1_MODE0      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE0_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_MODE0_BIT_HADAPT40G1_MODE0_OFF 0

/*--------------------------------------.
 | Register 0x00010348 HADAPT40G1_MODE1 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G1_MODE1      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE1_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_MODE1_BIT_HADAPT40G1_MODE1_OFF 0

/*--------------------------------------.
 | Register 0x0001034c HADAPT40G1_MODE2 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G1_MODE2      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE2_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_MODE2_BIT_HADAPT40G1_MODE2_OFF 0

/*--------------------------------------.
 | Register 0x00010350 HADAPT40G1_MODE3 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G1_MODE3      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE3_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_MODE3_BIT_HADAPT40G1_MODE3_OFF 0

/*--------------------------------------.
 | Register 0x00010354 HADAPT40G1_MODE4 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT40G1_MODE4      |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_MODE4_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_MODE4_BIT_HADAPT40G1_MODE4_OFF 0

/*---------------------------------------.
 | Register 0x00010358 HADAPT40G1_INT_EN |
 +---------------------------------------+
 |                                       |
 | bit  31:0 R/W  HADAPT40G1_INT_E       |
 |                                       |
 +--------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_INT_EN_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_INT_EN_BIT_HADAPT40G1_INT_E_OFF 0

/*------------------------------------.
 | Register 0x0001035c HADAPT40G1_INT |
 +------------------------------------+
 |                                    |
 | bit  31:0 R/W  HADAPT40G1_INT_I    |
 |                                    |
 +-----------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_INT_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_INT_BIT_HADAPT40G1_INT_I_OFF 0

/*----------------------------------------.
 | Register 0x00010360 HADAPT40G1_INT_VAL |
 +----------------------------------------+
 |                                        |
 | bit  31:0 R  HADAPT40G1_INT_V          |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_INT_VAL_UNUSED_MASK          0x00000000
#define OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_INT_VAL_BIT_HADAPT40G1_INT_V_OFF 0

/*-------------------------------------.
 | Register 0x00010364 HADAPT40G1_STAT |
 +-------------------------------------+
 |                                     |
 | bit  31:0 R  HADAPT40G1_STATUS      |
 |                                     |
 +------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT40G1_STAT_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT40G1_STAT_BIT_HADAPT40G1_STATUS_OFF 0

/*---------------------------------------.
 | Register 0x00010368 HADAPT100G_CONFIG |
 +---------------------------------------+
 |                                       |
 | bit  0 R/W  HADAPT100G_SFI_MODE_100G  |
 |                                       |
 +--------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_CONFIG_UNUSED_MASK                  0xfffffffe
#define OTN_MLD_REG_HADAPT100G_CONFIG_BIT_HADAPT100G_SFI_MODE_100G_MSK 0x00000001
#define OTN_MLD_REG_HADAPT100G_CONFIG_BIT_HADAPT100G_SFI_MODE_100G_OFF 0

/*--------------------------------------.
 | Register 0x0001036c HADAPT100G_MODE0 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT100G_MODE_0     |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_MODE0_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_MODE0_BIT_HADAPT100G_MODE_0_OFF 0

/*--------------------------------------.
 | Register 0x00010370 HADAPT100G_MODE1 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT100G_MODE_1     |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_MODE1_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_MODE1_BIT_HADAPT100G_MODE_1_OFF 0

/*--------------------------------------.
 | Register 0x00010374 HADAPT100G_MODE2 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT100G_MODE_2     |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_MODE2_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_MODE2_BIT_HADAPT100G_MODE_2_OFF 0

/*--------------------------------------.
 | Register 0x00010378 HADAPT100G_MODE3 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT100G_MODE_3     |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_MODE3_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_MODE3_BIT_HADAPT100G_MODE_3_OFF 0

/*--------------------------------------.
 | Register 0x0001037c HADAPT100G_MODE4 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT100G_MODE_4     |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_MODE4_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_MODE4_BIT_HADAPT100G_MODE_4_OFF 0

/*--------------------------------------.
 | Register 0x00010380 HADAPT100G_MODE5 |
 +--------------------------------------+
 |                                      |
 | bit  31:0 R/W  HADAPT100G_MODE_5     |
 |                                      |
 +-------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_MODE5_UNUSED_MASK           0x00000000
#define OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_MODE5_BIT_HADAPT100G_MODE_5_OFF 0

/*-----------------------------------------.
 | Register 0x00010384 HADAPT100G_PATTERN0 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_0     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN0_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN0_BIT_HADAPT100G_PATTERN_0_OFF 0

/*-----------------------------------------.
 | Register 0x00010388 HADAPT100G_PATTERN1 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_1     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN1_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN1_BIT_HADAPT100G_PATTERN_1_OFF 0

/*-----------------------------------------.
 | Register 0x0001038c HADAPT100G_PATTERN2 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_2     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN2_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN2_BIT_HADAPT100G_PATTERN_2_OFF 0

/*-----------------------------------------.
 | Register 0x00010390 HADAPT100G_PATTERN3 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_3     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN3_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN3_BIT_HADAPT100G_PATTERN_3_OFF 0

/*-----------------------------------------.
 | Register 0x00010394 HADAPT100G_PATTERN4 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_4     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN4_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN4_BIT_HADAPT100G_PATTERN_4_OFF 0

/*-----------------------------------------.
 | Register 0x00010398 HADAPT100G_PATTERN5 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_5     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN5_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN5_BIT_HADAPT100G_PATTERN_5_OFF 0

/*-----------------------------------------.
 | Register 0x0001039c HADAPT100G_PATTERN6 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_6     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN6_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN6_BIT_HADAPT100G_PATTERN_6_OFF 0

/*-----------------------------------------.
 | Register 0x000103a0 HADAPT100G_PATTERN7 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_7     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN7_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN7_BIT_HADAPT100G_PATTERN_7_OFF 0

/*-----------------------------------------.
 | Register 0x000103a4 HADAPT100G_PATTERN8 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_8     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN8_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN8_BIT_HADAPT100G_PATTERN_8_OFF 0

/*-----------------------------------------.
 | Register 0x000103a8 HADAPT100G_PATTERN9 |
 +-----------------------------------------+
 |                                         |
 | bit  31:0 R/W  HADAPT100G_PATTERN_9     |
 |                                         |
 +----------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_PATTERN9_UNUSED_MASK              0x00000000
#define OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_MSK 0xffffffff
#define OTN_MLD_REG_HADAPT100G_PATTERN9_BIT_HADAPT100G_PATTERN_9_OFF 0

/*----------------------------------------.
 | Register 0x000103ac HADAPT100G_INT_EN  |
 +----------------------------------------+
 |                                        |
 | bit  15:8 R/W  HADAPT100G_RXCLK2_INT_E |
 |                                        |
 |                                        |
 | bit  7:0  R/W  HADAPT100G_RXCLK1_INT_E |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_INT_EN_UNUSED_MASK                 0xffff0000
#define OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_MSK 0x0000ff00
#define OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK2_INT_E_OFF 8
#define OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_MSK 0x000000ff
#define OTN_MLD_REG_HADAPT100G_INT_EN_BIT_HADAPT100G_RXCLK1_INT_E_OFF 0

/*----------------------------------------.
 | Register 0x000103b0 HADAPT100G_INT     |
 +----------------------------------------+
 |                                        |
 | bit  15:8 R/W  HADAPT100G_RXCLK2_INT_I |
 |                                        |
 |                                        |
 | bit  7:0  R/W  HADAPT100G_RXCLK1_INT_I |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_INT_UNUSED_MASK                 0xffff0000
#define OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_MSK 0x0000ff00
#define OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK2_INT_I_OFF 8
#define OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_MSK 0x000000ff
#define OTN_MLD_REG_HADAPT100G_INT_BIT_HADAPT100G_RXCLK1_INT_I_OFF 0

/*----------------------------------------.
 | Register 0x000103b4 HADAPT100G_INT_VAL |
 +----------------------------------------+
 |                                        |
 | bit  15:8 R  HADAPT100G_RXCLK2_INT_V   |
 |                                        |
 |                                        |
 | bit  7:0  R  HADAPT100G_RXCLK1_INT_V   |
 |                                        |
 +---------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_INT_VAL_UNUSED_MASK                 0xffff0000
#define OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_MSK 0x0000ff00
#define OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK2_INT_V_OFF 8
#define OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_MSK 0x000000ff
#define OTN_MLD_REG_HADAPT100G_INT_VAL_BIT_HADAPT100G_RXCLK1_INT_V_OFF 0

/*-------------------------------------.
 | Register 0x000103b8 HADAPT100G_STAT |
 +-------------------------------------+
 |                                     |
 | bit  15:0 R  HADAPT100G_STATUS      |
 |                                     |
 +------------------------------------*/
#define PMC_OTN_MLD_REG_HADAPT100G_STAT_UNUSED_MASK           0xffff0000
#define OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_MSK 0x0000ffff
#define OTN_MLD_REG_HADAPT100G_STAT_BIT_HADAPT100G_STATUS_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OTN_MLD_REGS_H */
