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
 *     coreotn_fo block
 *****************************************************************************/
#ifndef _COREOTN_FO_REGS_H
#define _COREOTN_FO_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_COREOTN_FO_REG_SW_RESET_REG               0x0023c000
#define PMC_COREOTN_FO_REG_CLK_GATING_REG             0x0023c004
#define PMC_COREOTN_FO_REG_RAM_LOW_POWER_REG          0x0023c008
#define PMC_COREOTN_FO_REG_FRAMER_STAGE_CONFIG_REG    0x0023c014
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_1       0x0023c018
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_2       0x0023c01c
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_3       0x0023c020
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_4       0x0023c024
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_5       0x0023c028
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_6       0x0023c02c
#define PMC_COREOTN_FO_REG_GPO_REG                    0x0023c070
#define PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG( N )  (0x0023c078 + (N) * 0x4)
#define PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG( N )   (0x0023c0a8 + (N) * 0x4)
#define PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG( N )     (0x0023c0d8 + (N) * 0x4)
#define PMC_COREOTN_FO_REG_PRBS_BER_TH_LOWER_CFG_REG  0x0023c108
#define PMC_COREOTN_FO_REG_PRBS_BER_TH_UPPER_CFG_REG  0x0023c10c
#define PMC_COREOTN_FO_REG_PRBS_RESEED_REG            0x0023c110
#define PMC_COREOTN_FO_REG_PRBS_TSE_ROLLOVER_STAT_REG 0x0023c11c
#define PMC_COREOTN_FO_REG_PRBS_DLSS_INT_EN_REG       0x0023c120
#define PMC_COREOTN_FO_REG_PRBS_DLSS_INT_STAT_REG     0x0023c124
#define PMC_COREOTN_FO_REG_PRBS_DLSS_INT_VAL_REG      0x0023c128
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_1      0x0023c14c
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_2      0x0023c150
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_3      0x0023c154
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_1    0x0023c158
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_2    0x0023c15c
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_3    0x0023c160
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_1     0x0023c164
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_2     0x0023c168
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_3     0x0023c16c

/*----------------------------------.
 | Register 0x0023c000 SW_RESET_REG |
 +----------------------------------+
 | bit  6 R/W  OHFS_REMOVE_RST      |
 | bit  5 R/W  FRM_RCP_RST          |
 | bit  4 R/W  PRBS_RST             |
 | bit  3 R/W  MPMO_RST             |
 | bit  2 R/W  MPMA_RST             |
 | bit  1 R/W  TFRM_RST             |
 | bit  0 R/W  RFRM_RST             |
 +---------------------------------*/
#define PMC_COREOTN_FO_REG_SW_RESET_REG_UNUSED_MASK         0xffffff80
#define COREOTN_FO_REG_SW_RESET_REG_BIT_OHFS_REMOVE_RST_MSK 0x00000040
#define COREOTN_FO_REG_SW_RESET_REG_BIT_OHFS_REMOVE_RST_OFF 6
#define COREOTN_FO_REG_SW_RESET_REG_BIT_FRM_RCP_RST_MSK     0x00000020
#define COREOTN_FO_REG_SW_RESET_REG_BIT_FRM_RCP_RST_OFF     5
#define COREOTN_FO_REG_SW_RESET_REG_BIT_PRBS_RST_MSK        0x00000010
#define COREOTN_FO_REG_SW_RESET_REG_BIT_PRBS_RST_OFF        4
#define COREOTN_FO_REG_SW_RESET_REG_BIT_MPMO_RST_MSK        0x00000008
#define COREOTN_FO_REG_SW_RESET_REG_BIT_MPMO_RST_OFF        3
#define COREOTN_FO_REG_SW_RESET_REG_BIT_MPMA_RST_MSK        0x00000004
#define COREOTN_FO_REG_SW_RESET_REG_BIT_MPMA_RST_OFF        2
#define COREOTN_FO_REG_SW_RESET_REG_BIT_TFRM_RST_MSK        0x00000002
#define COREOTN_FO_REG_SW_RESET_REG_BIT_TFRM_RST_OFF        1
#define COREOTN_FO_REG_SW_RESET_REG_BIT_RFRM_RST_MSK        0x00000001
#define COREOTN_FO_REG_SW_RESET_REG_BIT_RFRM_RST_OFF        0

/*------------------------------------.
 | Register 0x0023c004 CLK_GATING_REG |
 +------------------------------------+
 | bit  6 R/W  OHFS_REMOVE_LOWPWR     |
 | bit  5 R/W  FRM_RCP_LOWPWR         |
 | bit  4 R/W  PRBS_LOWPWR            |
 | bit  3 R/W  MPMO_LOWPWR            |
 | bit  2 R/W  MPMA_LOWPWR            |
 | bit  1 R/W  TFRM_LOWPWR            |
 | bit  0 R/W  RFRM_LOWPWR            |
 +-----------------------------------*/
#define PMC_COREOTN_FO_REG_CLK_GATING_REG_UNUSED_MASK            0xffffff80
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_OHFS_REMOVE_LOWPWR_MSK 0x00000040
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_OHFS_REMOVE_LOWPWR_OFF 6
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_FRM_RCP_LOWPWR_MSK     0x00000020
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_FRM_RCP_LOWPWR_OFF     5
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_MSK        0x00000010
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_PRBS_LOWPWR_OFF        4
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_MSK        0x00000008
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_MPMO_LOWPWR_OFF        3
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_MSK        0x00000004
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_MPMA_LOWPWR_OFF        2
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_TFRM_LOWPWR_MSK        0x00000002
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_TFRM_LOWPWR_OFF        1
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_RFRM_LOWPWR_MSK        0x00000001
#define COREOTN_FO_REG_CLK_GATING_REG_BIT_RFRM_LOWPWR_OFF        0

/*---------------------------------------.
 | Register 0x0023c008 RAM_LOW_POWER_REG |
 +---------------------------------------+
 | bit  6 R/W  OHFS_REMOVE_RAM_LOWPWR    |
 | bit  4 R/W  PRBS_RAM_LOWPWR           |
 | bit  3 R/W  MPMO_RAM_LOWPWR           |
 | bit  2 R/W  MPMA_RAM_LOWPWR           |
 | bit  1 R/W  TFRM_RAM_LOWPWR           |
 | bit  0 R/W  RFRM_RAM_LOWPWR           |
 +--------------------------------------*/
#define PMC_COREOTN_FO_REG_RAM_LOW_POWER_REG_UNUSED_MASK                0xffffff80
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_OHFS_REMOVE_RAM_LOWPWR_MSK 0x00000040
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_OHFS_REMOVE_RAM_LOWPWR_OFF 6
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_MSK        0x00000010
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_PRBS_RAM_LOWPWR_OFF        4
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_MSK        0x00000008
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_MPMO_RAM_LOWPWR_OFF        3
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_MSK        0x00000004
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_MPMA_RAM_LOWPWR_OFF        2
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_TFRM_RAM_LOWPWR_MSK        0x00000002
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_TFRM_RAM_LOWPWR_OFF        1
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_RFRM_RAM_LOWPWR_MSK        0x00000001
#define COREOTN_FO_REG_RAM_LOW_POWER_REG_BIT_RFRM_RAM_LOWPWR_OFF        0

/*---------------------------------------------.
 | Register 0x0023c014 FRAMER_STAGE_CONFIG_REG |
 +---------------------------------------------+
 | bit  0 R/W  BWR_IND_SEL                     |
 +--------------------------------------------*/
#define PMC_COREOTN_FO_REG_FRAMER_STAGE_CONFIG_REG_UNUSED_MASK     0xfffffffe
#define COREOTN_FO_REG_FRAMER_STAGE_CONFIG_REG_BIT_BWR_IND_SEL_MSK 0x00000001
#define COREOTN_FO_REG_FRAMER_STAGE_CONFIG_REG_BIT_BWR_IND_SEL_OFF 0

/*------------------------------------------.
 | Register 0x0023c018 FRM_RCP_CONFIG_REG_1 |
 +------------------------------------------+
 | bit  31:0 R/W  REMOTE_RCP_CFG            |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_1_UNUSED_MASK        0x00000000
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_1_BIT_REMOTE_RCP_CFG_MSK 0xffffffff
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_1_BIT_REMOTE_RCP_CFG_OFF 0

/*------------------------------------------.
 | Register 0x0023c01c FRM_RCP_CONFIG_REG_2 |
 +------------------------------------------+
 | bit  31:0 R/W  REMOTE_RCP_CFG            |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_2_UNUSED_MASK        0x00000000
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_2_BIT_REMOTE_RCP_CFG_MSK 0xffffffff
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_2_BIT_REMOTE_RCP_CFG_OFF 0

/*------------------------------------------.
 | Register 0x0023c020 FRM_RCP_CONFIG_REG_3 |
 +------------------------------------------+
 | bit  31:0 R/W  REMOTE_RCP_CFG            |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_3_UNUSED_MASK        0x00000000
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_3_BIT_REMOTE_RCP_CFG_MSK 0xffffffff
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_3_BIT_REMOTE_RCP_CFG_OFF 0

/*------------------------------------------.
 | Register 0x0023c024 FRM_RCP_CONFIG_REG_4 |
 +------------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_CLRB_CFG         |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_4_UNUSED_MASK           0x00000000
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_4_BIT_CFC_FIFO_CLRB_CFG_MSK 0xffffffff
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_4_BIT_CFC_FIFO_CLRB_CFG_OFF 0

/*------------------------------------------.
 | Register 0x0023c028 FRM_RCP_CONFIG_REG_5 |
 +------------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_CLRB_CFG         |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_5_UNUSED_MASK           0x00000000
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_5_BIT_CFC_FIFO_CLRB_CFG_MSK 0xffffffff
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_5_BIT_CFC_FIFO_CLRB_CFG_OFF 0

/*------------------------------------------.
 | Register 0x0023c02c FRM_RCP_CONFIG_REG_6 |
 +------------------------------------------+
 | bit  31:0 R/W  CFC_FIFO_CLRB_CFG         |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_FRM_RCP_CONFIG_REG_6_UNUSED_MASK           0x00000000
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_6_BIT_CFC_FIFO_CLRB_CFG_MSK 0xffffffff
#define COREOTN_FO_REG_FRM_RCP_CONFIG_REG_6_BIT_CFC_FIFO_CLRB_CFG_OFF 0

/*-----------------------------.
 | Register 0x0023c070 GPO_REG |
 +-----------------------------+
 | bit  31:0 R/W  SPARE_GPO    |
 +----------------------------*/
#define PMC_COREOTN_FO_REG_GPO_REG_UNUSED_MASK   0x00000000
#define COREOTN_FO_REG_GPO_REG_BIT_SPARE_GPO_MSK 0xffffffff
#define COREOTN_FO_REG_GPO_REG_BIT_SPARE_GPO_OFF 0

/* index definitions for PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG */
#define PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_MIN             0
#define PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_MAX             11
#define PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_SIZE            12
#define PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_INDEX_N_OFFSET          0x4

/*--------------------------------------------------------.
 | Register (0x0023c078 + (N) * 0x4) PRBS_CHANNEL_CFG_REG |
 +--------------------------------------------------------+
 | bit  14:8 R/W  PRBS_CHID                               |
 | bit  3    R/W  PRBS_SEL_INV_NONINV                     |
 | bit  2    R/W  PRBS_SEL_31_11                          |
 | bit  1    R/W  PRBS_MON_EN                             |
 | bit  0    R/W  PRBS_GEN_EN                             |
 +-------------------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_UNUSED_MASK             0xffff80f0
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_MSK           0x00007f00
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_CHID_OFF           8
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_MSK 0x00000008
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_INV_NONINV_OFF 3
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_MSK      0x00000004
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_SEL_31_11_OFF      2
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_MSK         0x00000002
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_MON_EN_OFF         1
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_MSK         0x00000001
#define COREOTN_FO_REG_PRBS_CHANNEL_CFG_REG_BIT_PRBS_GEN_EN_OFF         0

/* index definitions for PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG */
#define PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_INDEX_N_MIN         0
#define PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_INDEX_N_MAX         11
#define PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_INDEX_N_SIZE        12
#define PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_INDEX_N_OFFSET      0x4

/*-------------------------------------------------------.
 | Register (0x0023c0a8 + (N) * 0x4) PRBS_DEFECT_CFG_REG |
 +-------------------------------------------------------+
 | bit  0 R/W  PRBS_INS_DEFECT                           |
 +------------------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_UNUSED_MASK         0xfffffffe
#define COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_MSK 0x00000001
#define COREOTN_FO_REG_PRBS_DEFECT_CFG_REG_BIT_PRBS_INS_DEFECT_OFF 0

/* index definitions for PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG */
#define PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG_INDEX_N_MIN           0
#define PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG_INDEX_N_MAX           11
#define PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG_INDEX_N_SIZE          12
#define PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------.
 | Register (0x0023c0d8 + (N) * 0x4) PRBS_TSE_STAT_REG |
 +-----------------------------------------------------+
 | bit  31:0 R  PRBS_TSE_CHAN_REG                      |
 +----------------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_TSE_STAT_REG_UNUSED_MASK           0x00000000
#define COREOTN_FO_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_MSK 0xffffffff
#define COREOTN_FO_REG_PRBS_TSE_STAT_REG_BIT_PRBS_TSE_CHAN_REG_OFF 0

/*-----------------------------------------------.
 | Register 0x0023c108 PRBS_BER_TH_LOWER_CFG_REG |
 +-----------------------------------------------+
 | bit  31:0 R/W  PRBS_BER_THRES                 |
 +----------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_BER_TH_LOWER_CFG_REG_UNUSED_MASK        0x00000000
#define COREOTN_FO_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_MSK 0xffffffff
#define COREOTN_FO_REG_PRBS_BER_TH_LOWER_CFG_REG_BIT_PRBS_BER_THRES_OFF 0

/*-----------------------------------------------.
 | Register 0x0023c10c PRBS_BER_TH_UPPER_CFG_REG |
 +-----------------------------------------------+
 | bit  2:0 R/W  PRBS_BER_THRES                  |
 +----------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_BER_TH_UPPER_CFG_REG_UNUSED_MASK        0xfffffff8
#define COREOTN_FO_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_MSK 0x00000007
#define COREOTN_FO_REG_PRBS_BER_TH_UPPER_CFG_REG_BIT_PRBS_BER_THRES_OFF 0

/*--------------------------------------.
 | Register 0x0023c110 PRBS_RESEED_REG  |
 +--------------------------------------+
 | bit  31:16 R/W  PRBS_RESEED_THR      |
 | bit  15:0  R/W  PRBS_RESEED_INTERVAL |
 +-------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_RESEED_REG_UNUSED_MASK              0x00000000
#define COREOTN_FO_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_MSK      0xffff0000
#define COREOTN_FO_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_THR_OFF      16
#define COREOTN_FO_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_MSK 0x0000ffff
#define COREOTN_FO_REG_PRBS_RESEED_REG_BIT_PRBS_RESEED_INTERVAL_OFF 0

/*------------------------------------------------.
 | Register 0x0023c11c PRBS_TSE_ROLLOVER_STAT_REG |
 +------------------------------------------------+
 | bit  11:0 R  PRBS_TSE_ROVER                    |
 +-----------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_TSE_ROLLOVER_STAT_REG_UNUSED_MASK        0xfffff000
#define COREOTN_FO_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_MSK 0x00000fff
#define COREOTN_FO_REG_PRBS_TSE_ROLLOVER_STAT_REG_BIT_PRBS_TSE_ROVER_OFF 0

/*------------------------------------------.
 | Register 0x0023c120 PRBS_DLSS_INT_EN_REG |
 +------------------------------------------+
 | bit  11:0 R/W  PRBS_DLSS_E               |
 +-----------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_DLSS_INT_EN_REG_UNUSED_MASK     0xfffff000
#define COREOTN_FO_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_MSK 0x00000fff
#define COREOTN_FO_REG_PRBS_DLSS_INT_EN_REG_BIT_PRBS_DLSS_E_OFF 0

/*--------------------------------------------.
 | Register 0x0023c124 PRBS_DLSS_INT_STAT_REG |
 +--------------------------------------------+
 | bit  11:0 R/W  PRBS_DLSS_I                 |
 +-------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_DLSS_INT_STAT_REG_UNUSED_MASK     0xfffff000
#define COREOTN_FO_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_MSK 0x00000fff
#define COREOTN_FO_REG_PRBS_DLSS_INT_STAT_REG_BIT_PRBS_DLSS_I_OFF 0

/*-------------------------------------------.
 | Register 0x0023c128 PRBS_DLSS_INT_VAL_REG |
 +-------------------------------------------+
 | bit  11:0 R  PRBS_DLSS_V                  |
 +------------------------------------------*/
#define PMC_COREOTN_FO_REG_PRBS_DLSS_INT_VAL_REG_UNUSED_MASK     0xfffff000
#define COREOTN_FO_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_MSK 0x00000fff
#define COREOTN_FO_REG_PRBS_DLSS_INT_VAL_REG_BIT_PRBS_DLSS_V_OFF 0

/*-------------------------------------------.
 | Register 0x0023c14c OPU_DCSF_INT_EN_REG_1 |
 +-------------------------------------------+
 | bit  31:0 R/W  OPU_DCSF_E                 |
 +------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_1_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_1_BIT_OPU_DCSF_E_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_1_BIT_OPU_DCSF_E_OFF 0

/*-------------------------------------------.
 | Register 0x0023c150 OPU_DCSF_INT_EN_REG_2 |
 +-------------------------------------------+
 | bit  31:0 R/W  OPU_DCSF_E                 |
 +------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_2_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_2_BIT_OPU_DCSF_E_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_2_BIT_OPU_DCSF_E_OFF 0

/*-------------------------------------------.
 | Register 0x0023c154 OPU_DCSF_INT_EN_REG_3 |
 +-------------------------------------------+
 | bit  31:0 R/W  OPU_DCSF_E                 |
 +------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_3_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_3_BIT_OPU_DCSF_E_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_EN_REG_3_BIT_OPU_DCSF_E_OFF 0

/*---------------------------------------------.
 | Register 0x0023c158 OPU_DCSF_INT_STAT_REG_1 |
 +---------------------------------------------+
 | bit  31:0 R/W  OPU_DCSF_I                   |
 +--------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_1_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_1_BIT_OPU_DCSF_I_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_1_BIT_OPU_DCSF_I_OFF 0

/*---------------------------------------------.
 | Register 0x0023c15c OPU_DCSF_INT_STAT_REG_2 |
 +---------------------------------------------+
 | bit  31:0 R/W  OPU_DCSF_I                   |
 +--------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_2_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_2_BIT_OPU_DCSF_I_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_2_BIT_OPU_DCSF_I_OFF 0

/*---------------------------------------------.
 | Register 0x0023c160 OPU_DCSF_INT_STAT_REG_3 |
 +---------------------------------------------+
 | bit  31:0 R/W  OPU_DCSF_I                   |
 +--------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_3_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_3_BIT_OPU_DCSF_I_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_STAT_REG_3_BIT_OPU_DCSF_I_OFF 0

/*--------------------------------------------.
 | Register 0x0023c164 OPU_DCSF_INT_VAL_REG_1 |
 +--------------------------------------------+
 | bit  31:0 R  OPU_DCSF_V                    |
 +-------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_1_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_1_BIT_OPU_DCSF_V_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_1_BIT_OPU_DCSF_V_OFF 0

/*--------------------------------------------.
 | Register 0x0023c168 OPU_DCSF_INT_VAL_REG_2 |
 +--------------------------------------------+
 | bit  31:0 R  OPU_DCSF_V                    |
 +-------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_2_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_2_BIT_OPU_DCSF_V_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_2_BIT_OPU_DCSF_V_OFF 0

/*--------------------------------------------.
 | Register 0x0023c16c OPU_DCSF_INT_VAL_REG_3 |
 +--------------------------------------------+
 | bit  31:0 R  OPU_DCSF_V                    |
 +-------------------------------------------*/
#define PMC_COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_3_UNUSED_MASK    0x00000000
#define COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_3_BIT_OPU_DCSF_V_MSK 0xffffffff
#define COREOTN_FO_REG_OPU_DCSF_INT_VAL_REG_3_BIT_OPU_DCSF_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _COREOTN_FO_REGS_H */
