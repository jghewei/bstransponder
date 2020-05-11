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
 *     mapotn_rgfpf block
 *****************************************************************************/
#ifndef _MAPOTN_RGFPF_REGS_H
#define _MAPOTN_RGFPF_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RGFPF96_CORE_REG_GENERAL_CONFIGURATION                          0x00030000
#define PMC_RGFPF96_CORE_REG_CHANNEL_RESET( N )                             (0x00030004 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_LSB                    0x00030184
#define PMC_RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_MSB                    0x00030188
#define PMC_RGFPF96_CORE_REG_ORDERED_SET_UPI                                0x0003018c
#define PMC_RGFPF96_CORE_REG_PMON_LATCH_CONTROL_REGISTER                    0x00030190
#define PMC_RGFPF96_CORE_REG_TRIG_PAUSE_CAP_REGISTER                        0x00030194
#define PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER( N )                   (0x00030198 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD( N )                        (0x000301a4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS                  0x000301bc
#define PMC_RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS                             0x000301c0
#define PMC_RGFPF96_CORE_REG_CHID_MISMATCH_CAPTURE                          0x000301c4
#define PMC_RGFPF96_CORE_REG_CSF_STATUS                                     0x000301c8
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_RPT                                   0x000301cc
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE( N )                      (0x000301e8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE( N )         (0x000301f4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE( N )                (0x00030200 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE( N )    (0x0003020c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE( N )                   (0x00030218 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE( N )  (0x00030224 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE( N )               (0x00030230 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE( N )               (0x0003023c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE( N )               (0x00030248 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE( N )  (0x00030254 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE( N )               (0x00030260 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE( N )        (0x0003026c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE( N )           (0x00030284 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE( N )    (0x00030290 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE( N )                       (0x0003029c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CSF_STATUS_AV_INTR_ENABLE                      0x000302a8
#define PMC_RGFPF96_CORE_REG_CSF_STATUS_FIFO_OF_INTR_ENABLE                 0x000302ac
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE( N )             (0x000302b0 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE( N )         (0x000302bc + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE( N )                         (0x000302c8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE( N )                 (0x000302d4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE( N )                 (0x000302e0 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE( N,M )                (0x000302ec + (N) * 0x4 + (M) * 0xc)
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE( N )                   (0x00030340 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE( N )                (0x0003034c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE( N )              (0x00030358 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE( N )        (0x00030364 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_PMON_TRANSFER_INTR_ENABLE                      0x00030370
#define PMC_RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE                     0x00030374
#define PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN( N )                             (0x00030384 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN( N )                           (0x00030390 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_DPLM_INT_EN( N )                               (0x0003039c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN( N )                            (0x000303a8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_RCOH_STAT_TRANSFR_DONE_EN                      0x000303b8
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR( N )                             (0x000303bc + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR( N )                (0x000303c8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR( N )                       (0x000303d4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR( N )           (0x000303e0 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR( N )                          (0x000303ec + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR( N )                      (0x000303f8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR( N )                (0x00030404 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR( N )         (0x00030410 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR( N )                      (0x0003041c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR( N )         (0x00030428 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR( N )                (0x00030434 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR( N )               (0x00030440 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR( N )                  (0x00030458 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR( N )           (0x00030464 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR( N )                              (0x00030470 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CSF_ST_AV_INTR                                 0x0003047c
#define PMC_RGFPF96_CORE_REG_CSF_ST_FIFO_OF_INTR                            0x00030480
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR( N )                    (0x00030484 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR( N )                (0x00030490 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR( N )                                (0x0003049c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR( N )       (0x000304a8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR( N )       (0x000304b4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR( N,M )                       (0x000304c0 + (N) * 0x4 + (M) * 0xc)
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR( N )                          (0x00030514 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR( N )                     (0x00030520 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR( N )                     (0x0003052c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR( N )               (0x00030538 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_PMON_TRANSFER_INTR                             0x00030544
#define PMC_RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR                            0x00030548
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT( N )                            (0x00030558 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT( N )                              (0x00030564 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_DPLM_INT( N )                                  (0x00030570 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT( N )                               (0x0003057c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_RCOH_STAT_TRANSFER_DONE_INT                    0x0003058c
#define PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE( N ) (0x00030590 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE( N )           (0x0003059c + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE( N )                   (0x000305a8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE( N )         (0x000305b4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE( N )            (0x000305c0 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL( N )                          (0x000305cc + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL( N )                       (0x000305d8 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_DPLM_INT_VAL( N )                              (0x000305e4 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL( N )                           (0x000305f0 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER( N )                    (0x00030600 + (N) * 0x4)
#define PMC_RGFPF96_CORE_REG_MUTEX_CPU_DATA_FIFO                            0x00030700
#define PMC_RGFPF96_CORE_REG_MUTEX_CSF_STATUS_FIFO                          0x00030880
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0( N )               (0x00031000 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1( N )               (0x00031004 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2( N )               (0x00031008 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3( N )               (0x0003100c + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4( N )               (0x00031010 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG( N,M )             (0x00031020 + (N) * 0x4 + (M) * 0x100)
#define PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION( N )          (0x0003103c + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET( N )                (0x00031040 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA( N )                        (0x00031044 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG( N )                            (0x00031048 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0( N )                   (0x0003104c + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1( N )                   (0x00031050 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2( N )                   (0x00031054 + (N) * 0x100)
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0( N,M )                  (0x00031060 + (N) * 0x8 + (M) * 0x100)
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1( N,M )                  (0x00031064 + (N) * 0x8 + (M) * 0x100)
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB( N )                      (0x00037000 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB( N )                      (0x00037004 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_RX_FRM_CNT( N )                                (0x00037008 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON( N )                         (0x0003700c + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON( N )                         (0x00037010 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT( N )                              (0x00037014 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON( N )              (0x00037018 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON( N )                  (0x0003701c + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON( N )                         (0x00037020 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON( N )                (0x00037024 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON( N )                  (0x00037028 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON( N )                        (0x0003702c + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON( N )                             (0x00037030 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON( N )                             (0x00037034 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON( N )                          (0x00037038 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON( N )                          (0x0003703c + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON( N )                        (0x00037040 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON( N )                          (0x00037048 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON( N )                           (0x0003704c + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON( N )                           (0x00037050 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON( N )                        (0x00037054 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON( N )                        (0x00037058 + (N) * 0x80)
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON( N,M )                   (0x00037060 + (N) * 0x4 + (M) * 0x80)

/*-------------------------------------------.
 | Register 0x00030000 GENERAL_CONFIGURATION |
 +-------------------------------------------+
 | bit  24   R/W  UNPACK_AFIFO_CLR           |
 | bit  16   R/W  CSF_FIFO_CLR               |
 | bit  14:8 R/W  CSF_ST_I_TRSHLD            |
 +------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GENERAL_CONFIGURATION_UNUSED_MASK          0xfefe80f0
#define RGFPF96_CORE_REG_GENERAL_CONFIGURATION_BIT_UNPACK_AFIFO_CLR_MSK 0x01000000
#define RGFPF96_CORE_REG_GENERAL_CONFIGURATION_BIT_UNPACK_AFIFO_CLR_OFF 24
#define RGFPF96_CORE_REG_GENERAL_CONFIGURATION_BIT_CSF_FIFO_CLR_MSK     0x00010000
#define RGFPF96_CORE_REG_GENERAL_CONFIGURATION_BIT_CSF_FIFO_CLR_OFF     16
#define RGFPF96_CORE_REG_GENERAL_CONFIGURATION_BIT_CSF_ST_I_TRSHLD_MSK  0x00007f00
#define RGFPF96_CORE_REG_GENERAL_CONFIGURATION_BIT_CSF_ST_I_TRSHLD_OFF  8

/* index definitions for PMC_RGFPF96_CORE_REG_CHANNEL_RESET */
#define PMC_RGFPF96_CORE_REG_CHANNEL_RESET_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_CHANNEL_RESET_INDEX_N_MAX    95
#define PMC_RGFPF96_CORE_REG_CHANNEL_RESET_INDEX_N_SIZE   96
#define PMC_RGFPF96_CORE_REG_CHANNEL_RESET_INDEX_N_OFFSET 0x4

/*-------------------------------------------------.
 | Register (0x00030004 + (N) * 0x4) CHANNEL_RESET |
 +-------------------------------------------------+
 | bit  0 R/W  CHN_RST                             |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CHANNEL_RESET_UNUSED_MASK    0xfffffffe
#define RGFPF96_CORE_REG_CHANNEL_RESET_BIT_CHN_RST_MSK    0x00000001
#define RGFPF96_CORE_REG_CHANNEL_RESET_BIT_CHN_RST_OFF    0

/*-------------------------------------------------.
 | Register 0x00030184 MAC_DESTINATION_ADDRESS_LSB |
 +-------------------------------------------------+
 | bit  31:0 R/W  MAC_DA_LSB                       |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_LSB_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_LSB_BIT_MAC_DA_LSB_MSK 0xffffffff
#define RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_LSB_BIT_MAC_DA_LSB_OFF 0

/*-------------------------------------------------.
 | Register 0x00030188 MAC_DESTINATION_ADDRESS_MSB |
 +-------------------------------------------------+
 | bit  15:0 R/W  MAC_DA_MSB                       |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_MSB_UNUSED_MASK    0xffff0000
#define RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_MSB_BIT_MAC_DA_MSB_MSK 0x0000ffff
#define RGFPF96_CORE_REG_MAC_DESTINATION_ADDRESS_MSB_BIT_MAC_DA_MSB_OFF 0

/*-------------------------------------.
 | Register 0x0003018c ORDERED_SET_UPI |
 +-------------------------------------+
 | bit  7:0 R/W  ORD_SET_UPI           |
 +------------------------------------*/
#define PMC_RGFPF96_CORE_REG_ORDERED_SET_UPI_UNUSED_MASK     0xffffff00
#define RGFPF96_CORE_REG_ORDERED_SET_UPI_BIT_ORD_SET_UPI_MSK 0x000000ff
#define RGFPF96_CORE_REG_ORDERED_SET_UPI_BIT_ORD_SET_UPI_OFF 0

/*-------------------------------------------------.
 | Register 0x00030190 PMON_LATCH_CONTROL_REGISTER |
 +-------------------------------------------------+
 | bit  0 R/W  PMON_LATCH_TRIG                     |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PMON_LATCH_CONTROL_REGISTER_UNUSED_MASK         0xfffffffe
#define RGFPF96_CORE_REG_PMON_LATCH_CONTROL_REGISTER_BIT_PMON_LATCH_TRIG_MSK 0x00000001
#define RGFPF96_CORE_REG_PMON_LATCH_CONTROL_REGISTER_BIT_PMON_LATCH_TRIG_OFF 0

/*---------------------------------------------.
 | Register 0x00030194 TRIG_PAUSE_CAP_REGISTER |
 +---------------------------------------------+
 | bit  0 R/W  PAUSE_CAP_TRIG                  |
 +--------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_TRIG_PAUSE_CAP_REGISTER_UNUSED_MASK        0xfffffffe
#define RGFPF96_CORE_REG_TRIG_PAUSE_CAP_REGISTER_BIT_PAUSE_CAP_TRIG_MSK 0x00000001
#define RGFPF96_CORE_REG_TRIG_PAUSE_CAP_REGISTER_BIT_PAUSE_CAP_TRIG_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER */
#define PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_INDEX_N_MIN           0
#define PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_INDEX_N_MAX           2
#define PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_INDEX_N_SIZE          3
#define PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_INDEX_N_OFFSET        0x4

/*-----------------------------------------------------------.
 | Register (0x00030198 + (N) * 0x4) AUTO_FLOW_CTRL_REGISTER |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  AUTO_FLOW_CTRL_EN                          |
 +----------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_UNUSED_MASK           0x00000000
#define RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_BIT_AUTO_FLOW_CTRL_EN_MSK 0xffffffff
#define RGFPF96_CORE_REG_AUTO_FLOW_CTRL_REGISTER_BIT_AUTO_FLOW_CTRL_EN_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD */
#define PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_INDEX_N_MAX        5
#define PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_INDEX_N_SIZE       6
#define PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_INDEX_N_OFFSET     0x4

/*------------------------------------------------------.
 | Register (0x000301a4 + (N) * 0x4) CPB_ZONE_THRESHOLD |
 +------------------------------------------------------+
 | bit  31:0 R/W  CPB_ZONE_THRLD                        |
 +-----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_UNUSED_MASK        0x00000000
#define RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_BIT_CPB_ZONE_THRLD_MSK 0xffffffff
#define RGFPF96_CORE_REG_CPB_ZONE_THRESHOLD_BIT_CPB_ZONE_THRLD_OFF 0

/*---------------------------------------------------.
 | Register 0x000301bc EXTENSION_HDR_MIS_FAIL_STATUS |
 +---------------------------------------------------+
 | bit  31:16 R    DEXM_TYPEHDR                      |
 | bit  15:8  R    DEXM_CID                          |
 | bit  7:1   R    DEXM_CHID                         |
 | bit  0     R/W  DEXM_STATUS_REG_FREE              |
 +--------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_UNUSED_MASK              0x00000000
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_TYPEHDR_MSK         0xffff0000
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_TYPEHDR_OFF         16
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_CID_MSK             0x0000ff00
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_CID_OFF             8
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_CHID_MSK            0x000000fe
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_CHID_OFF            1
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_STATUS_REG_FREE_MSK 0x00000001
#define RGFPF96_CORE_REG_EXTENSION_HDR_MIS_FAIL_STATUS_BIT_DEXM_STATUS_REG_FREE_OFF 0

/*----------------------------------------.
 | Register 0x000301c0 PTI_UPI_MIS_STATUS |
 +----------------------------------------+
 | bit  23:8 R    DUPM_TYPEHDR            |
 | bit  7:1  R    DUPM_CHID               |
 | bit  0    R/W  DUPM_STATUS_REG_FREE    |
 +---------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_UNUSED_MASK              0xff000000
#define RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_BIT_DUPM_TYPEHDR_MSK         0x00ffff00
#define RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_BIT_DUPM_TYPEHDR_OFF         8
#define RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_BIT_DUPM_CHID_MSK            0x000000fe
#define RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_BIT_DUPM_CHID_OFF            1
#define RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_BIT_DUPM_STATUS_REG_FREE_MSK 0x00000001
#define RGFPF96_CORE_REG_PTI_UPI_MIS_STATUS_BIT_DUPM_STATUS_REG_FREE_OFF 0

/*-------------------------------------------.
 | Register 0x000301c4 CHID_MISMATCH_CAPTURE |
 +-------------------------------------------+
 | bit  14:8 R/W  DUPM_CAP_CHID              |
 | bit  6:0  R/W  DEXM_CAP_CHID              |
 +------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CHID_MISMATCH_CAPTURE_UNUSED_MASK       0xffff8080
#define RGFPF96_CORE_REG_CHID_MISMATCH_CAPTURE_BIT_DUPM_CAP_CHID_MSK 0x00007f00
#define RGFPF96_CORE_REG_CHID_MISMATCH_CAPTURE_BIT_DUPM_CAP_CHID_OFF 8
#define RGFPF96_CORE_REG_CHID_MISMATCH_CAPTURE_BIT_DEXM_CAP_CHID_MSK 0x0000007f
#define RGFPF96_CORE_REG_CHID_MISMATCH_CAPTURE_BIT_DEXM_CAP_CHID_OFF 0

/*--------------------------------.
 | Register 0x000301c8 CSF_STATUS |
 +--------------------------------+
 | bit  15:9 R  CSF_ST_CHID       |
 | bit  8:1  R  CSF_ST_UPI        |
 | bit  0    R  CSF_ST_VALID      |
 +-------------------------------*/
#define PMC_RGFPF96_CORE_REG_CSF_STATUS_UNUSED_MASK      0xffff0000
#define RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_CHID_MSK  0x0000fe00
#define RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_CHID_OFF  9
#define RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_UPI_MSK   0x000001fe
#define RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_UPI_OFF   1
#define RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_VALID_MSK 0x00000001
#define RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_VALID_OFF 0

/*----------------------------------.
 | Register 0x000301cc CPU_FIFO_RPT |
 +----------------------------------+
 | bit  24:16 R/W  WR_RPT_THRLD     |
 | bit  12:8  R/W  RD_RPT_THRLD     |
 | bit  1     R/W  WR_RPT_FIFOCLR   |
 | bit  0     R/W  RD_RPT_FIFOCLR   |
 +---------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_RPT_UNUSED_MASK        0xfe00e0fc
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_WR_RPT_THRLD_MSK   0x01ff0000
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_WR_RPT_THRLD_OFF   16
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_RD_RPT_THRLD_MSK   0x00001f00
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_RD_RPT_THRLD_OFF   8
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_WR_RPT_FIFOCLR_MSK 0x00000002
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_WR_RPT_FIFOCLR_OFF 1
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_RD_RPT_FIFOCLR_MSK 0x00000001
#define RGFPF96_CORE_REG_CPU_FIFO_RPT_BIT_RD_RPT_FIFOCLR_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------.
 | Register (0x000301e8 + (N) * 0x4) IN_SFAIL_INTR_ENABLE |
 +--------------------------------------------------------+
 | bit  31:0 R/W  IN_SFAIL_E                              |
 +-------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_BIT_IN_SFAIL_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_IN_SFAIL_INTR_ENABLE_BIT_IN_SFAIL_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------------.
 | Register (0x000301f4 + (N) * 0x4) LOSS_FRME_DELINEATION_INTR_ENABLE |
 +---------------------------------------------------------------------+
 | bit  31:0 R/W  LOFD_E                                               |
 +--------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_BIT_LOFD_E_MSK     0xffffffff
#define RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_ENABLE_BIT_LOFD_E_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------.
 | Register (0x00030200 + (N) * 0x4) GFP_IDLE_FRAME_INTR_ENABLE |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  IDLE_E                                        |
 +-------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_BIT_IDLE_E_MSK     0xffffffff
#define RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_ENABLE_BIT_IDLE_E_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------------------.
 | Register (0x0003020c + (N) * 0x4) GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE |
 +--------------------------------------------------------------------------+
 | bit  31:0 R/W  CTRLFRM_E                                                 |
 +-------------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_BIT_CTRLFRM_E_MSK  0xffffffff
#define RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_BIT_CTRLFRM_E_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_INDEX_N_MIN            0
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_INDEX_N_MAX            2
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_INDEX_N_SIZE           3
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_INDEX_N_OFFSET         0x4

/*-----------------------------------------------------------.
 | Register (0x00030218 + (N) * 0x4) UNPACK_DROP_INTR_ENABLE |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  UNPACK_FIFO_DROP_E                         |
 +----------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_UNUSED_MASK            0x00000000
#define RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_BIT_UNPACK_FIFO_DROP_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_UNPACK_DROP_INTR_ENABLE_BIT_UNPACK_FIFO_DROP_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*----------------------------------------------------------------------------.
 | Register (0x00030224 + (N) * 0x4) GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE |
 +----------------------------------------------------------------------------+
 | bit  31:0 R/W  UCHEC_E                                                     |
 +---------------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_BIT_UCHEC_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_BIT_UCHEC_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------.
 | Register (0x00030230 + (N) * 0x4) GFP_CCHEC_ERROR_INTR_ENABLE |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  CCHEC_E                                        |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_BIT_CCHEC_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_CCHEC_ERROR_INTR_ENABLE_BIT_CCHEC_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------.
 | Register (0x0003023c + (N) * 0x4) GFP_UTHEC_ERROR_INTR_ENABLE |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  UTHEC_E                                        |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_BIT_UTHEC_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_UTHEC_ERROR_INTR_ENABLE_BIT_UTHEC_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------.
 | Register (0x00030248 + (N) * 0x4) GFP_CTHEC_ERROR_INTR_ENABLE |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  CTHEC_E                                        |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_BIT_CTHEC_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_ENABLE_BIT_CTHEC_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*----------------------------------------------------------------------------.
 | Register (0x00030254 + (N) * 0x4) GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE |
 +----------------------------------------------------------------------------+
 | bit  31:0 R/W  UEHEC_E                                                     |
 +---------------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_BIT_UEHEC_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_BIT_UEHEC_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------.
 | Register (0x00030260 + (N) * 0x4) GFP_CEHEC_ERROR_INTR_ENABLE |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  CEHEC_E                                        |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_BIT_CEHEC_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_CEHEC_ERROR_INTR_ENABLE_BIT_CEHEC_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*----------------------------------------------------------------------.
 | Register (0x0003026c + (N) * 0x4) GFP_EXI_MISMATCH_ERROR_INTR_ENABLE |
 +----------------------------------------------------------------------+
 | bit  31:0 R/W  EXI_E                                                 |
 +---------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_BIT_EXI_E_MSK      0xffffffff
#define RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_BIT_EXI_E_OFF      0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_INDEX_N_MIN         0
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_INDEX_N_MAX         2
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_INDEX_N_SIZE        3
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_INDEX_N_OFFSET      0x4

/*-------------------------------------------------------------------.
 | Register (0x00030284 + (N) * 0x4) GFP_PTI_UPI_MSM_ERR_INTR_ENABLE |
 +-------------------------------------------------------------------+
 | bit  31:0 R/W  PTI_UPI_MSMCH_E                                    |
 +------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_UNUSED_MASK         0x00000000
#define RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_BIT_PTI_UPI_MSMCH_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_BIT_PTI_UPI_MSMCH_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------------------.
 | Register (0x00030290 + (N) * 0x4) GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE |
 +--------------------------------------------------------------------------+
 | bit  31:0 R/W  GENERIC_E                                                 |
 +-------------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_BIT_GENERIC_E_MSK  0xffffffff
#define RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_BIT_GENERIC_E_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------.
 | Register (0x0003029c + (N) * 0x4) GFP_CSF_INTR_ENABLE |
 +-------------------------------------------------------+
 | bit  31:0 R/W  CSF_E                                  |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_BIT_CSF_E_MSK      0xffffffff
#define RGFPF96_CORE_REG_GFP_CSF_INTR_ENABLE_BIT_CSF_E_OFF      0

/*-----------------------------------------------.
 | Register 0x000302a8 CSF_STATUS_AV_INTR_ENABLE |
 +-----------------------------------------------+
 | bit  0 R/W  CSF_ST_AV_E                       |
 +----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CSF_STATUS_AV_INTR_ENABLE_UNUSED_MASK     0xfffffffe
#define RGFPF96_CORE_REG_CSF_STATUS_AV_INTR_ENABLE_BIT_CSF_ST_AV_E_MSK 0x00000001
#define RGFPF96_CORE_REG_CSF_STATUS_AV_INTR_ENABLE_BIT_CSF_ST_AV_E_OFF 0

/*----------------------------------------------------.
 | Register 0x000302ac CSF_STATUS_FIFO_OF_INTR_ENABLE |
 +----------------------------------------------------+
 | bit  0 R/W  CSF_ST_FIFO_OF_E                       |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CSF_STATUS_FIFO_OF_INTR_ENABLE_UNUSED_MASK          0xfffffffe
#define RGFPF96_CORE_REG_CSF_STATUS_FIFO_OF_INTR_ENABLE_BIT_CSF_ST_FIFO_OF_E_MSK 0x00000001
#define RGFPF96_CORE_REG_CSF_STATUS_FIFO_OF_INTR_ENABLE_BIT_CSF_ST_FIFO_OF_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------------.
 | Register (0x000302b0 + (N) * 0x4) GFP_DROPPED_FRAME_INTR_ENABLE |
 +-----------------------------------------------------------------+
 | bit  31:0 R/W  DROP_E                                           |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_BIT_DROP_E_MSK     0xffffffff
#define RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_ENABLE_BIT_DROP_E_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------------.
 | Register (0x000302bc + (N) * 0x4) GFP_CLIENT_DATA_FRAME_INTR_ENABLE |
 +---------------------------------------------------------------------+
 | bit  31:0 R/W  DATAFRM_E                                            |
 +--------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_BIT_DATAFRM_E_MSK  0xffffffff
#define RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_BIT_DATAFRM_E_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------.
 | Register (0x000302c8 + (N) * 0x4) PAUSE_INTR_ENABLE |
 +-----------------------------------------------------+
 | bit  31:0 R/W  PAUSE_E                              |
 +----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_BIT_PAUSE_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_PAUSE_INTR_ENABLE_BIT_PAUSE_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_INDEX_N_OFFSET    0x4

/*-------------------------------------------------------------.
 | Register (0x000302d4 + (N) * 0x4) MAX_FL_VIOALT_INTR_ENABLE |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  MAX_FRM_LEN_E                                |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_BIT_MAX_FRM_LEN_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_MAX_FL_VIOALT_INTR_ENABLE_BIT_MAX_FRM_LEN_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_INDEX_N_OFFSET    0x4

/*-------------------------------------------------------------.
 | Register (0x000302e0 + (N) * 0x4) MIN_FL_VIOALT_INTR_ENABLE |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  MIN_FRM_LEN_E                                |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_BIT_MIN_FRM_LEN_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_MIN_FL_VIOALT_INTR_ENABLE_BIT_MIN_FRM_LEN_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_N_MIN             0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_N_MAX             2
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_N_SIZE            3
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_N_OFFSET          0x4
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_M_MIN             0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_M_MAX             6
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_M_SIZE            7
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_INDEX_M_OFFSET          0xc

/*------------------------------------------------------------------------.
 | Register (0x000302ec + (N) * 0x4 + (M) * 0xc) FRM_LEN_STAT_INTR_ENABLE |
 +------------------------------------------------------------------------+
 | bit  31:0 R/W  FRM_LEN_STAT_INTR_E                                     |
 +-----------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_UNUSED_MASK             0x00000000
#define RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_BIT_FRM_LEN_STAT_INTR_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_ENABLE_BIT_FRM_LEN_STAT_INTR_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------.
 | Register (0x00030340 + (N) * 0x4) PAYLOAD_FCS_INTR_ENABLE |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  PFCSE_E                                    |
 +----------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_BIT_PFCSE_E_MSK    0xffffffff
#define RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_ENABLE_BIT_PFCSE_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_INDEX_N_OFFSET    0x4

/*--------------------------------------------------------------.
 | Register (0x0003034c + (N) * 0x4) DOWNSTRM_FRAME_INTR_ENABLE |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  DWNSTRM_FRM_E                                 |
 +-------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_BIT_DWNSTRM_FRM_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_DOWNSTRM_FRAME_INTR_ENABLE_BIT_DWNSTRM_FRM_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_INDEX_N_MIN            0
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_INDEX_N_MAX            2
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_INDEX_N_SIZE           3
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_INDEX_N_OFFSET         0x4

/*----------------------------------------------------------------.
 | Register (0x00030358 + (N) * 0x4) CPU_FIFO_DATA_AV_INTR_ENABLE |
 +----------------------------------------------------------------+
 | bit  31:0 R/W  CPU_FIFO_DATA_AV_E                              |
 +---------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_UNUSED_MASK            0x00000000
#define RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_BIT_CPU_FIFO_DATA_AV_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_ENABLE_BIT_CPU_FIFO_DATA_AV_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE */
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_INDEX_N_MAX        2
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_INDEX_N_SIZE       3
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_INDEX_N_OFFSET     0x4

/*----------------------------------------------------------------------.
 | Register (0x00030364 + (N) * 0x4) CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE |
 +----------------------------------------------------------------------+
 | bit  31:0 R/W  CPU_FIFO_OVF_E                                        |
 +---------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_UNUSED_MASK        0x00000000
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_BIT_CPU_FIFO_OVF_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_BIT_CPU_FIFO_OVF_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00030370 PMON_TRANSFER_INTR_ENABLE |
 +-----------------------------------------------+
 | bit  1 R/W  PMON_TRFR_END_E                   |
 | bit  0 R/W  PMON_TRFR_START_E                 |
 +----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PMON_TRANSFER_INTR_ENABLE_UNUSED_MASK           0xfffffffc
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_ENABLE_BIT_PMON_TRFR_END_E_MSK   0x00000002
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_ENABLE_BIT_PMON_TRFR_END_E_OFF   1
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_ENABLE_BIT_PMON_TRFR_START_E_MSK 0x00000001
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_ENABLE_BIT_PMON_TRFR_START_E_OFF 0

/*------------------------------------------------.
 | Register 0x00030374 ASYNC_FIFO_OVF_INTR_ENABLE |
 +------------------------------------------------+
 | bit  2 R/W  CPU_WRRPT_AFIFO_OVF_E              |
 | bit  1 R/W  CPU_RDRPT_AFIFO_OVF_E              |
 | bit  0 R/W  UNPACK_AFIFO_OVF_E                 |
 +-----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_UNUSED_MASK               0xfffffff8
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_BIT_CPU_WRRPT_AFIFO_OVF_E_MSK 0x00000004
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_BIT_CPU_WRRPT_AFIFO_OVF_E_OFF 2
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_BIT_CPU_RDRPT_AFIFO_OVF_E_MSK 0x00000002
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_BIT_CPU_RDRPT_AFIFO_OVF_E_OFF 1
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_BIT_UNPACK_AFIFO_OVF_E_MSK    0x00000001
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_ENABLE_BIT_UNPACK_AFIFO_OVF_E_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN */
#define PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN_INDEX_N_OFFSET    0x4

/*-------------------------------------------------.
 | Register (0x00030384 + (N) * 0x4) SSF_SF_INT_EN |
 +-------------------------------------------------+
 | bit  31:0 R/W  SSF_SF_RCVD_E                    |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_SSF_SF_INT_EN_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_SSF_SF_INT_EN_BIT_SSF_SF_RCVD_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_SSF_SF_INT_EN_BIT_SSF_SF_RCVD_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN */
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN_INDEX_N_MIN          0
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN_INDEX_N_MAX          2
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN_INDEX_N_SIZE         3
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN_INDEX_N_OFFSET       0x4

/*---------------------------------------------------.
 | Register (0x00030390 + (N) * 0x4) HAO_STAT_INT_EN |
 +---------------------------------------------------+
 | bit  31:0 R/W  HAO_STATUS_AVL_E                   |
 +--------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_EN_UNUSED_MASK          0x00000000
#define RGFPF96_CORE_REG_HAO_STAT_INT_EN_BIT_HAO_STATUS_AVL_E_MSK 0xffffffff
#define RGFPF96_CORE_REG_HAO_STAT_INT_EN_BIT_HAO_STATUS_AVL_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_DPLM_INT_EN */
#define PMC_RGFPF96_CORE_REG_DPLM_INT_EN_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_DPLM_INT_EN_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_DPLM_INT_EN_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_DPLM_INT_EN_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x0003039c + (N) * 0x4) DPLM_INT_EN |
 +-----------------------------------------------+
 | bit  31:0 R/W  DPLM_E                         |
 +----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DPLM_INT_EN_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_DPLM_INT_EN_BIT_DPLM_E_MSK     0xffffffff
#define RGFPF96_CORE_REG_DPLM_INT_EN_BIT_DPLM_E_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN */
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN_INDEX_N_OFFSET 0x4

/*--------------------------------------------------.
 | Register (0x000303a8 + (N) * 0x4) OPU_CSF_INT_EN |
 +--------------------------------------------------+
 | bit  31:0 R/W  OPU_CSF_E                         |
 +-------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_EN_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_OPU_CSF_INT_EN_BIT_OPU_CSF_E_MSK  0xffffffff
#define RGFPF96_CORE_REG_OPU_CSF_INT_EN_BIT_OPU_CSF_E_OFF  0

/*-----------------------------------------------.
 | Register 0x000303b8 RCOH_STAT_TRANSFR_DONE_EN |
 +-----------------------------------------------+
 | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_E            |
 +----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_RCOH_STAT_TRANSFR_DONE_EN_UNUSED_MASK                0xfffffffe
#define RGFPF96_CORE_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_MSK 0x00000001
#define RGFPF96_CORE_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR */
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_INDEX_N_OFFSET 0x4

/*-------------------------------------------------.
 | Register (0x000303bc + (N) * 0x4) IN_SFAIL_INTR |
 +-------------------------------------------------+
 | bit  31:0 R/W  IN_SFAIL_I                       |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_IN_SFAIL_INTR_BIT_IN_SFAIL_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_IN_SFAIL_INTR_BIT_IN_SFAIL_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR */
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------.
 | Register (0x000303c8 + (N) * 0x4) LOSS_FRME_DELINEATION_INTR |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  LOFD_I                                        |
 +-------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_BIT_LOFD_I_MSK     0xffffffff
#define RGFPF96_CORE_REG_LOSS_FRME_DELINEATION_INTR_BIT_LOFD_I_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------.
 | Register (0x000303d4 + (N) * 0x4) GFP_IDLE_FRAME_INTR |
 +-------------------------------------------------------+
 | bit  31:0 R/W  IDLE_I                                 |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_BIT_IDLE_I_MSK     0xffffffff
#define RGFPF96_CORE_REG_GFP_IDLE_FRAME_INTR_BIT_IDLE_I_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------------------.
 | Register (0x000303e0 + (N) * 0x4) GFP_NON_IDLE_CONTROL_FRAME_INTR |
 +-------------------------------------------------------------------+
 | bit  31:0 R/W  CTRLFRM_I                                          |
 +------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_BIT_CTRLFRM_I_MSK  0xffffffff
#define RGFPF96_CORE_REG_GFP_NON_IDLE_CONTROL_FRAME_INTR_BIT_CTRLFRM_I_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR */
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_INDEX_N_MIN            0
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_INDEX_N_MAX            2
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_INDEX_N_SIZE           3
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_INDEX_N_OFFSET         0x4

/*----------------------------------------------------.
 | Register (0x000303ec + (N) * 0x4) UNPACK_DROP_INTR |
 +----------------------------------------------------+
 | bit  31:0 R/W  UNPACK_FIFO_DROP_I                  |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_INTR_UNUSED_MASK            0x00000000
#define RGFPF96_CORE_REG_UNPACK_DROP_INTR_BIT_UNPACK_FIFO_DROP_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_UNPACK_DROP_INTR_BIT_UNPACK_FIFO_DROP_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------.
 | Register (0x000303f8 + (N) * 0x4) GFP_UCHEC_ERROR_INTR |
 +--------------------------------------------------------+
 | bit  31:0 R/W  UCHEC_I                                 |
 +-------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_BIT_UCHEC_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_UCHEC_ERROR_INTR_BIT_UCHEC_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------.
 | Register (0x00030404 + (N) * 0x4) GFP_CCHEC_ERROR_ERROR_INTR |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  CCHEC_I                                       |
 +-------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_BIT_CCHEC_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_CCHEC_ERROR_ERROR_INTR_BIT_CCHEC_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------------.
 | Register (0x00030410 + (N) * 0x4) GFP_UNCORRECTABLE_THEC_ERROR_INTR |
 +---------------------------------------------------------------------+
 | bit  31:0 R/W  UTHEC_I                                              |
 +--------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_BIT_UTHEC_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_THEC_ERROR_INTR_BIT_UTHEC_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------.
 | Register (0x0003041c + (N) * 0x4) GFP_CTHEC_ERROR_INTR |
 +--------------------------------------------------------+
 | bit  31:0 R/W  CTHEC_I                                 |
 +-------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_BIT_CTHEC_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_CTHEC_ERROR_INTR_BIT_CTHEC_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------------.
 | Register (0x00030428 + (N) * 0x4) GFP_UNCORRECTABLE_EHEC_ERROR_INTR |
 +---------------------------------------------------------------------+
 | bit  31:0 R/W  UEHEC_I                                              |
 +--------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_BIT_UEHEC_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_BIT_UEHEC_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------.
 | Register (0x00030434 + (N) * 0x4) GFP_CEHEC_ERROR_ERROR_INTR |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  CEHEC_I                                       |
 +-------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_BIT_CEHEC_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_GFP_CEHEC_ERROR_ERROR_INTR_BIT_CEHEC_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------.
 | Register (0x00030440 + (N) * 0x4) GFP_EXI_MISMATCH_ERROR_INTR |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  EXI_I                                          |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_BIT_EXI_I_MSK      0xffffffff
#define RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_BIT_EXI_I_OFF      0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_INDEX_N_MIN         0
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_INDEX_N_MAX         2
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_INDEX_N_SIZE        3
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_INDEX_N_OFFSET      0x4

/*------------------------------------------------------------.
 | Register (0x00030458 + (N) * 0x4) GFP_PTI_UPI_MSM_ERR_INTR |
 +------------------------------------------------------------+
 | bit  31:0 R/W  PTI_UPI_MSMCH_I                             |
 +-----------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_UNUSED_MASK         0x00000000
#define RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_BIT_PTI_UPI_MSMCH_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_BIT_PTI_UPI_MSMCH_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------------------.
 | Register (0x00030464 + (N) * 0x4) GFP_GENERIC_MISMATCH_ERROR_INTR |
 +-------------------------------------------------------------------+
 | bit  31:0 R/W  GENERIC_I                                          |
 +------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_BIT_GENERIC_I_MSK  0xffffffff
#define RGFPF96_CORE_REG_GFP_GENERIC_MISMATCH_ERROR_INTR_BIT_GENERIC_I_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CSF_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x00030470 + (N) * 0x4) GFP_CSF_INTR |
 +------------------------------------------------+
 | bit  31:0 R/W  CSF_I                           |
 +-----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CSF_INTR_BIT_CSF_I_MSK      0xffffffff
#define RGFPF96_CORE_REG_GFP_CSF_INTR_BIT_CSF_I_OFF      0

/*------------------------------------.
 | Register 0x0003047c CSF_ST_AV_INTR |
 +------------------------------------+
 | bit  0 R/W  CSF_ST_AV_I            |
 +-----------------------------------*/
#define PMC_RGFPF96_CORE_REG_CSF_ST_AV_INTR_UNUSED_MASK     0xfffffffe
#define RGFPF96_CORE_REG_CSF_ST_AV_INTR_BIT_CSF_ST_AV_I_MSK 0x00000001
#define RGFPF96_CORE_REG_CSF_ST_AV_INTR_BIT_CSF_ST_AV_I_OFF 0

/*-----------------------------------------.
 | Register 0x00030480 CSF_ST_FIFO_OF_INTR |
 +-----------------------------------------+
 | bit  0 R/W  CSF_ST_FIFO_OF_I            |
 +----------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CSF_ST_FIFO_OF_INTR_UNUSED_MASK          0xfffffffe
#define RGFPF96_CORE_REG_CSF_ST_FIFO_OF_INTR_BIT_CSF_ST_FIFO_OF_I_MSK 0x00000001
#define RGFPF96_CORE_REG_CSF_ST_FIFO_OF_INTR_BIT_CSF_ST_FIFO_OF_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_INDEX_N_OFFSET 0x4

/*----------------------------------------------------------.
 | Register (0x00030484 + (N) * 0x4) GFP_DROPPED_FRAME_INTR |
 +----------------------------------------------------------+
 | bit  31:0 R/W  DROP_I                                    |
 +---------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_BIT_DROP_I_MSK     0xffffffff
#define RGFPF96_CORE_REG_GFP_DROPPED_FRAME_INTR_BIT_DROP_I_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR */
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_INDEX_N_OFFSET 0x4

/*--------------------------------------------------------------.
 | Register (0x00030490 + (N) * 0x4) GFP_CLIENT_DATA_FRAME_INTR |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  DATAFRM_I                                     |
 +-------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_BIT_DATAFRM_I_MSK  0xffffffff
#define RGFPF96_CORE_REG_GFP_CLIENT_DATA_FRAME_INTR_BIT_DATAFRM_I_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_PAUSE_INTR */
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_INDEX_N_OFFSET 0x4

/*----------------------------------------------.
 | Register (0x0003049c + (N) * 0x4) PAUSE_INTR |
 +----------------------------------------------+
 | bit  31:0 R/W  PAUSE_I                       |
 +---------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PAUSE_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_PAUSE_INTR_BIT_PAUSE_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_PAUSE_INTR_BIT_PAUSE_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR */
#define PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_OFFSET    0x4

/*-----------------------------------------------------------------------.
 | Register (0x000304a8 + (N) * 0x4) MAXIMUM_FRAME_LENGTH_VIOLATION_INTR |
 +-----------------------------------------------------------------------+
 | bit  31:0 R/W  MAX_FRM_LEN_I                                          |
 +----------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_BIT_MAX_FRM_LEN_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_BIT_MAX_FRM_LEN_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR */
#define PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_INDEX_N_OFFSET    0x4

/*-----------------------------------------------------------------------.
 | Register (0x000304b4 + (N) * 0x4) MINIMUM_FRAME_LENGTH_VIOLATION_INTR |
 +-----------------------------------------------------------------------+
 | bit  31:0 R/W  MIN_FRM_LEN_I                                          |
 +----------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_BIT_MIN_FRM_LEN_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_BIT_MIN_FRM_LEN_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR */
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_N_MIN             0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_N_MAX             2
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_N_SIZE            3
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_N_OFFSET          0x4
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_M_MIN             0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_M_MAX             6
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_M_SIZE            7
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_INDEX_M_OFFSET          0xc

/*-----------------------------------------------------------------.
 | Register (0x000304c0 + (N) * 0x4 + (M) * 0xc) FRM_LEN_STAT_INTR |
 +-----------------------------------------------------------------+
 | bit  31:0 R/W  FRM_LEN_STAT_INTR_I                              |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_UNUSED_MASK             0x00000000
#define RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_BIT_FRM_LEN_STAT_INTR_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_FRM_LEN_STAT_INTR_BIT_FRM_LEN_STAT_INTR_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR */
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_INDEX_N_OFFSET 0x4

/*----------------------------------------------------.
 | Register (0x00030514 + (N) * 0x4) PAYLOAD_FCS_INTR |
 +----------------------------------------------------+
 | bit  31:0 R/W  PFCSE_I                             |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_BIT_PFCSE_I_MSK    0xffffffff
#define RGFPF96_CORE_REG_PAYLOAD_FCS_INTR_BIT_PFCSE_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR */
#define PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_INDEX_N_OFFSET    0x4

/*---------------------------------------------------------.
 | Register (0x00030520 + (N) * 0x4) DOWNSTREAM_FRAME_INTR |
 +---------------------------------------------------------+
 | bit  31:0 R/W  DWNSTRM_FRM_I                            |
 +--------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_BIT_DWNSTRM_FRM_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_DOWNSTREAM_FRAME_INTR_BIT_DWNSTRM_FRM_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR */
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_INDEX_N_MIN            0
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_INDEX_N_MAX            2
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_INDEX_N_SIZE           3
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_INDEX_N_OFFSET         0x4

/*---------------------------------------------------------.
 | Register (0x0003052c + (N) * 0x4) CPU_FIFO_DATA_AV_INTR |
 +---------------------------------------------------------+
 | bit  31:0 R/W  CPU_FIFO_DATA_AV_I                       |
 +--------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_UNUSED_MASK            0x00000000
#define RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_BIT_CPU_FIFO_DATA_AV_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_CPU_FIFO_DATA_AV_INTR_BIT_CPU_FIFO_DATA_AV_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR */
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_INDEX_N_MAX        2
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_INDEX_N_SIZE       3
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_INDEX_N_OFFSET     0x4

/*---------------------------------------------------------------.
 | Register (0x00030538 + (N) * 0x4) CPU_DATA_FIFO_OVERFLOW_INTR |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  CPU_FIFO_OVF_I                                 |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_UNUSED_MASK        0x00000000
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_BIT_CPU_FIFO_OVF_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_OVERFLOW_INTR_BIT_CPU_FIFO_OVF_I_OFF 0

/*----------------------------------------.
 | Register 0x00030544 PMON_TRANSFER_INTR |
 +----------------------------------------+
 | bit  1 R/W  PMON_TRFR_END_I            |
 | bit  0 R/W  PMON_TRFR_START_I          |
 +---------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PMON_TRANSFER_INTR_UNUSED_MASK           0xfffffffc
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_BIT_PMON_TRFR_END_I_MSK   0x00000002
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_BIT_PMON_TRFR_END_I_OFF   1
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_BIT_PMON_TRFR_START_I_MSK 0x00000001
#define RGFPF96_CORE_REG_PMON_TRANSFER_INTR_BIT_PMON_TRFR_START_I_OFF 0

/*-----------------------------------------.
 | Register 0x00030548 ASYNC_FIFO_OVF_INTR |
 +-----------------------------------------+
 | bit  2 R/W  CPU_WRRPT_AFIFO_OVF_I       |
 | bit  1 R/W  CPU_RDRPT_AFIFO_OVF_I       |
 | bit  0 R/W  UNPACK_AFIFO_OVF_I          |
 +----------------------------------------*/
#define PMC_RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_UNUSED_MASK               0xfffffff8
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_BIT_CPU_WRRPT_AFIFO_OVF_I_MSK 0x00000004
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_BIT_CPU_WRRPT_AFIFO_OVF_I_OFF 2
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_BIT_CPU_RDRPT_AFIFO_OVF_I_MSK 0x00000002
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_BIT_CPU_RDRPT_AFIFO_OVF_I_OFF 1
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_BIT_UNPACK_AFIFO_OVF_I_MSK    0x00000001
#define RGFPF96_CORE_REG_ASYNC_FIFO_OVF_INTR_BIT_UNPACK_AFIFO_OVF_I_OFF    0

/* index definitions for PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT */
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT_INDEX_N_OFFSET    0x4

/*--------------------------------------------------.
 | Register (0x00030558 + (N) * 0x4) SSF_SF_RCVDINT |
 +--------------------------------------------------+
 | bit  31:0 R/W  SSF_SF_RCVD_I                     |
 +-------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVDINT_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_SSF_SF_RCVDINT_BIT_SSF_SF_RCVD_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_SSF_SF_RCVDINT_BIT_SSF_SF_RCVD_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_HAO_STAT_INT */
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_INDEX_N_MIN          0
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_INDEX_N_MAX          2
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_INDEX_N_SIZE         3
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_INDEX_N_OFFSET       0x4

/*------------------------------------------------.
 | Register (0x00030564 + (N) * 0x4) HAO_STAT_INT |
 +------------------------------------------------+
 | bit  31:0 R/W  HAO_STATUS_AVL_I                |
 +-----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_HAO_STAT_INT_UNUSED_MASK          0x00000000
#define RGFPF96_CORE_REG_HAO_STAT_INT_BIT_HAO_STATUS_AVL_I_MSK 0xffffffff
#define RGFPF96_CORE_REG_HAO_STAT_INT_BIT_HAO_STATUS_AVL_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_DPLM_INT */
#define PMC_RGFPF96_CORE_REG_DPLM_INT_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_DPLM_INT_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_DPLM_INT_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_DPLM_INT_INDEX_N_OFFSET 0x4

/*--------------------------------------------.
 | Register (0x00030570 + (N) * 0x4) DPLM_INT |
 +--------------------------------------------+
 | bit  31:0 R/W  DPLM_I                      |
 +-------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DPLM_INT_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_DPLM_INT_BIT_DPLM_I_MSK     0xffffffff
#define RGFPF96_CORE_REG_DPLM_INT_BIT_DPLM_I_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_OPU_CSF_INT */
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x0003057c + (N) * 0x4) OPU_CSF_INT |
 +-----------------------------------------------+
 | bit  31:0 R/W  OPU_CSF_I                      |
 +----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_OPU_CSF_INT_BIT_OPU_CSF_I_MSK  0xffffffff
#define RGFPF96_CORE_REG_OPU_CSF_INT_BIT_OPU_CSF_I_OFF  0

/*-------------------------------------------------.
 | Register 0x0003058c RCOH_STAT_TRANSFER_DONE_INT |
 +-------------------------------------------------+
 | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_I              |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_RCOH_STAT_TRANSFER_DONE_INT_UNUSED_MASK                0xfffffffe
#define RGFPF96_CORE_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_MSK 0x00000001
#define RGFPF96_CORE_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE */
#define PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------------------------.
 | Register (0x00030590 + (N) * 0x4) LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE |
 +-----------------------------------------------------------------------------+
 | bit  31:0 R  LOFD_V                                                         |
 +----------------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_BIT_LOFD_V_MSK     0xffffffff
#define RGFPF96_CORE_REG_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_BIT_LOFD_V_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE */
#define PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------------------.
 | Register (0x0003059c + (N) * 0x4) GFP_FRAMER_IDLE_INTERRUPT_VALUE |
 +-------------------------------------------------------------------+
 | bit  31:0 R  IDLE_V                                               |
 +------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_BIT_IDLE_V_MSK     0xffffffff
#define RGFPF96_CORE_REG_GFP_FRAMER_IDLE_INTERRUPT_VALUE_BIT_IDLE_V_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE */
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_INDEX_N_OFFSET 0x4

/*-----------------------------------------------------------.
 | Register (0x000305a8 + (N) * 0x4) GFP_CSF_INTERRUPT_VALUE |
 +-----------------------------------------------------------+
 | bit  31:0 R  CSF_V                                        |
 +----------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_BIT_CSF_V_MSK      0xffffffff
#define RGFPF96_CORE_REG_GFP_CSF_INTERRUPT_VALUE_BIT_CSF_V_OFF      0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE */
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_INDEX_N_OFFSET 0x4

/*---------------------------------------------------------------------.
 | Register (0x000305b4 + (N) * 0x4) GFP_EXI_MISMATCH_ERROR_INTR_VALUE |
 +---------------------------------------------------------------------+
 | bit  31:0 R  EXI_V                                                  |
 +--------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_BIT_EXI_V_MSK      0xffffffff
#define RGFPF96_CORE_REG_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_BIT_EXI_V_OFF      0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE */
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_INDEX_N_MIN         0
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_INDEX_N_MAX         2
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_INDEX_N_SIZE        3
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_INDEX_N_OFFSET      0x4

/*------------------------------------------------------------------.
 | Register (0x000305c0 + (N) * 0x4) GFP_PTI_UPI_MSM_ERR_INTR_VALUE |
 +------------------------------------------------------------------+
 | bit  31:0 R  PTI_UPI_MSMCH_V                                     |
 +-----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_UNUSED_MASK         0x00000000
#define RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_BIT_PTI_UPI_MSMCH_V_MSK 0xffffffff
#define RGFPF96_CORE_REG_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_BIT_PTI_UPI_MSMCH_V_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL */
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_INDEX_N_OFFSET 0x4

/*----------------------------------------------------.
 | Register (0x000305cc + (N) * 0x4) IN_SFAIL_INT_VAL |
 +----------------------------------------------------+
 | bit  31:0 R  IN_SFAIL_V                            |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_BIT_IN_SFAIL_V_MSK 0xffffffff
#define RGFPF96_CORE_REG_IN_SFAIL_INT_VAL_BIT_IN_SFAIL_V_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL */
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_OFFSET    0x4

/*-------------------------------------------------------.
 | Register (0x000305d8 + (N) * 0x4) SSF_SF_RCVD_INT_VAL |
 +-------------------------------------------------------+
 | bit  31:0 R  SSF_SF_RCVD_V                            |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_BIT_SSF_SF_RCVD_V_MSK 0xffffffff
#define RGFPF96_CORE_REG_SSF_SF_RCVD_INT_VAL_BIT_SSF_SF_RCVD_V_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_DPLM_INT_VAL */
#define PMC_RGFPF96_CORE_REG_DPLM_INT_VAL_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_DPLM_INT_VAL_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_DPLM_INT_VAL_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_DPLM_INT_VAL_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x000305e4 + (N) * 0x4) DPLM_INT_VAL |
 +------------------------------------------------+
 | bit  31:0 R  DPLM_V                            |
 +-----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DPLM_INT_VAL_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_DPLM_INT_VAL_BIT_DPLM_V_MSK     0xffffffff
#define RGFPF96_CORE_REG_DPLM_INT_VAL_BIT_DPLM_V_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL */
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL_INDEX_N_MAX    2
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL_INDEX_N_SIZE   3
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL_INDEX_N_OFFSET 0x4

/*---------------------------------------------------.
 | Register (0x000305f0 + (N) * 0x4) OPU_CSF_INT_VAL |
 +---------------------------------------------------+
 | bit  31:0 R  OPU_CSF_V                            |
 +--------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_OPU_CSF_INT_VAL_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_OPU_CSF_INT_VAL_BIT_OPU_CSF_V_MSK  0xffffffff
#define RGFPF96_CORE_REG_OPU_CSF_INT_VAL_BIT_OPU_CSF_V_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER */
#define PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_INDEX_N_MAX       2
#define PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_INDEX_N_SIZE      3
#define PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_INDEX_N_OFFSET    0x4

/*----------------------------------------------------------.
 | Register (0x00030600 + (N) * 0x4) DCSF_DEASSERT_REGISTER |
 +----------------------------------------------------------+
 | bit  31:0 R/W  DCSF_DEASSERT                             |
 +---------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_BIT_DCSF_DEASSERT_MSK 0xffffffff
#define RGFPF96_CORE_REG_DCSF_DEASSERT_REGISTER_BIT_DCSF_DEASSERT_OFF 0

/*-----------------------------------------.
 | Register 0x00030700 MUTEX_CPU_DATA_FIFO |
 +-----------------------------------------+
 | bit  0 R/W  MTX_CPU_DATA_FIFO           |
 +----------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MUTEX_CPU_DATA_FIFO_UNUSED_MASK           0xfffffffe
#define RGFPF96_CORE_REG_MUTEX_CPU_DATA_FIFO_BIT_MTX_CPU_DATA_FIFO_MSK 0x00000001
#define RGFPF96_CORE_REG_MUTEX_CPU_DATA_FIFO_BIT_MTX_CPU_DATA_FIFO_OFF 0

/*-------------------------------------------.
 | Register 0x00030880 MUTEX_CSF_STATUS_FIFO |
 +-------------------------------------------+
 | bit  0 R/W  MTX_CSF_STATUS_FIFO           |
 +------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MUTEX_CSF_STATUS_FIFO_UNUSED_MASK             0xfffffffe
#define RGFPF96_CORE_REG_MUTEX_CSF_STATUS_FIFO_BIT_MTX_CSF_STATUS_FIFO_MSK 0x00000001
#define RGFPF96_CORE_REG_MUTEX_CSF_STATUS_FIFO_BIT_MTX_CSF_STATUS_FIFO_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0 */
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_INDEX_N_MIN                0
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_INDEX_N_MAX                95
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_INDEX_N_SIZE               96
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_INDEX_N_OFFSET             0x100

/*-----------------------------------------------------------------.
 | Register (0x00031000 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_0 |
 +-----------------------------------------------------------------+
 | bit  30    R/W  SHUT_DATA                                       |
 | bit  29    R/W  DATA_REV_IBIT                                   |
 | bit  28    R/W  DC_BALANCE                                      |
 | bit  27    R/W  CHEC_ECC                                        |
 | bit  26:25 R/W  DELTA                                           |
 | bit  23:16 R/W  UNPACK_DROP_THRLD2_MAX                          |
 | bit  15:8  R/W  UNPACK_DROP_THRLD2_MIN                          |
 | bit  7:0   R/W  UNPACK_DROP_THRLD1                              |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_UNUSED_MASK                0x80000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_SHUT_DATA_MSK              0x40000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_SHUT_DATA_OFF              30
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_DATA_REV_IBIT_MSK          0x20000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_DATA_REV_IBIT_OFF          29
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_DC_BALANCE_MSK             0x10000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_DC_BALANCE_OFF             28
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_CHEC_ECC_MSK               0x08000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_CHEC_ECC_OFF               27
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_DELTA_MSK                  0x06000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_DELTA_OFF                  25
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_UNPACK_DROP_THRLD2_MAX_MSK 0x00ff0000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_UNPACK_DROP_THRLD2_MAX_OFF 16
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_UNPACK_DROP_THRLD2_MIN_MSK 0x0000ff00
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_UNPACK_DROP_THRLD2_MIN_OFF 8
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_UNPACK_DROP_THRLD1_MSK     0x000000ff
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_0_BIT_UNPACK_DROP_THRLD1_OFF     0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1 */
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_INDEX_N_MIN           0
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_INDEX_N_MAX           95
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_INDEX_N_SIZE          96
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_INDEX_N_OFFSET        0x100

/*-----------------------------------------------------------------.
 | Register (0x00031004 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_1 |
 +-----------------------------------------------------------------+
 | bit  30    R/W  PAUSE_MODE                                      |
 | bit  29    R/W  PAUSE_FILTER                                    |
 | bit  28    R/W  PAUSE_ENABLE                                    |
 | bit  27:26 R/W  PREAMBLE_SFD_MODE                               |
 | bit  25:20 R/W  EXI_LENGTH2                                     |
 | bit  19:16 R/W  EXI_VALUE2                                      |
 | bit  15:10 R/W  EXI_LENGTH1                                     |
 | bit  9:6   R/W  EXI_VALUE1                                      |
 | bit  5     R/W  EHEC_ECC                                        |
 | bit  4     R/W  EHEC_CHK                                        |
 | bit  3     R/W  THEC_ECC                                        |
 | bit  2     R/W  THEC_CHK                                        |
 | bit  1     R/W  DESCRMBL                                        |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_UNUSED_MASK           0x80000001
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PAUSE_MODE_MSK        0x40000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PAUSE_MODE_OFF        30
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PAUSE_FILTER_MSK      0x20000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PAUSE_FILTER_OFF      29
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PAUSE_ENABLE_MSK      0x10000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PAUSE_ENABLE_OFF      28
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PREAMBLE_SFD_MODE_MSK 0x0c000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_PREAMBLE_SFD_MODE_OFF 26
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_LENGTH2_MSK       0x03f00000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_LENGTH2_OFF       20
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_VALUE2_MSK        0x000f0000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_VALUE2_OFF        16
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_LENGTH1_MSK       0x0000fc00
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_LENGTH1_OFF       10
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_VALUE1_MSK        0x000003c0
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EXI_VALUE1_OFF        6
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EHEC_ECC_MSK          0x00000020
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EHEC_ECC_OFF          5
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EHEC_CHK_MSK          0x00000010
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_EHEC_CHK_OFF          4
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_THEC_ECC_MSK          0x00000008
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_THEC_ECC_OFF          3
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_THEC_CHK_MSK          0x00000004
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_THEC_CHK_OFF          2
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_DESCRMBL_MSK          0x00000002
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_1_BIT_DESCRMBL_OFF          1

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2 */
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_INDEX_N_MIN            0
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_INDEX_N_MAX            95
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_INDEX_N_SIZE           96
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_INDEX_N_OFFSET         0x100

/*-----------------------------------------------------------------.
 | Register (0x00031008 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_2 |
 +-----------------------------------------------------------------+
 | bit  30    R/W  CPU_ONLY1BUF                                    |
 | bit  29    R/W  DATA_REV_OBIT                                   |
 | bit  28    R/W  EGRESS_FCS_DISCARD                              |
 | bit  27    R/W  FC1200_TRAFFIC                                  |
 | bit  26:25 R/W  EGRESS_FORMAT                                   |
 | bit  24:23 R/W  CPU_FIFO_FORMAT                                 |
 | bit  20    R/W  EGRESS_EFCS_STRIP                               |
 | bit  19    R/W  EGRESS_PFCS_STRIP                               |
 | bit  18    R/W  CNT_UDF_FCS                                     |
 | bit  17:16 R/W  GFP_FCS_CHK_MODE                                |
 | bit  15    R/W  PFCS_REV_FCSBYTE                                |
 | bit  14    R/W  PFCS_REV_FCSBIT                                 |
 | bit  13    R/W  PFCS_REV_PLDBIT                                 |
 | bit  12    R/W  PFCS_INV                                        |
 | bit  11    R/W  PFCS_INIT                                       |
 | bit  10:3  R/W  PAUSE_UPI                                       |
 | bit  2:0   R/W  PAUSE_PTI                                       |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_UNUSED_MASK            0x80400000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_CPU_ONLY1BUF_MSK       0x40000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_CPU_ONLY1BUF_OFF       30
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_DATA_REV_OBIT_MSK      0x20000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_DATA_REV_OBIT_OFF      29
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_FCS_DISCARD_MSK 0x10000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_FCS_DISCARD_OFF 28
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_FC1200_TRAFFIC_MSK     0x08000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_FC1200_TRAFFIC_OFF     27
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_FORMAT_MSK      0x06000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_FORMAT_OFF      25
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_CPU_FIFO_FORMAT_MSK    0x01800000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_CPU_FIFO_FORMAT_OFF    23
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_EFCS_STRIP_MSK  0x00100000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_EFCS_STRIP_OFF  20
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_PFCS_STRIP_MSK  0x00080000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_EGRESS_PFCS_STRIP_OFF  19
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_CNT_UDF_FCS_MSK        0x00040000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_CNT_UDF_FCS_OFF        18
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_GFP_FCS_CHK_MODE_MSK   0x00030000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_GFP_FCS_CHK_MODE_OFF   16
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_REV_FCSBYTE_MSK   0x00008000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_REV_FCSBYTE_OFF   15
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_REV_FCSBIT_MSK    0x00004000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_REV_FCSBIT_OFF    14
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_REV_PLDBIT_MSK    0x00002000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_REV_PLDBIT_OFF    13
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_INV_MSK           0x00001000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_INV_OFF           12
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_INIT_MSK          0x00000800
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PFCS_INIT_OFF          11
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PAUSE_UPI_MSK          0x000007f8
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PAUSE_UPI_OFF          3
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PAUSE_PTI_MSK          0x00000007
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_2_BIT_PAUSE_PTI_OFF          0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3 */
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_INDEX_N_MIN           0
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_INDEX_N_MAX           95
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_INDEX_N_SIZE          96
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_INDEX_N_OFFSET        0x100

/*-----------------------------------------------------------------.
 | Register (0x0003100c + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_3 |
 +-----------------------------------------------------------------+
 | bit  8 R/W  I_SFAIL_PROPAGATE                                   |
 | bit  7 R/W  DLFD_SFAIL                                          |
 | bit  4 R/W  DEXM_SFAIL                                          |
 | bit  3 R/W  DLFD_EN                                             |
 | bit  2 R/W  DCSF_EN                                             |
 | bit  1 R/W  DUPM_EN                                             |
 | bit  0 R/W  DEXM_EN                                             |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_UNUSED_MASK           0xfffffe00
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_I_SFAIL_PROPAGATE_MSK 0x00000100
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_I_SFAIL_PROPAGATE_OFF 8
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DLFD_SFAIL_MSK        0x00000080
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DLFD_SFAIL_OFF        7
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DEXM_SFAIL_MSK        0x00000010
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DEXM_SFAIL_OFF        4
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DLFD_EN_MSK           0x00000008
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DLFD_EN_OFF           3
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DCSF_EN_MSK           0x00000004
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DCSF_EN_OFF           2
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DUPM_EN_MSK           0x00000002
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DUPM_EN_OFF           1
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DEXM_EN_MSK           0x00000001
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_3_BIT_DEXM_EN_OFF           0

/* index definitions for PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4 */
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_INDEX_N_MIN            0
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_INDEX_N_MAX            95
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_INDEX_N_SIZE           96
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_INDEX_N_OFFSET         0x100

/*-----------------------------------------------------------------.
 | Register (0x00031010 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_4 |
 +-----------------------------------------------------------------+
 | bit  24:16 R/W  UNPACK_XOFF_THRLD2                              |
 | bit  15:8  R/W  EGRESS_XOFF_THRLD                               |
 | bit  7:0   R/W  UNPACK_XOFF_THRLD                               |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_UNUSED_MASK            0xfe000000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_BIT_UNPACK_XOFF_THRLD2_MSK 0x01ff0000
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_BIT_UNPACK_XOFF_THRLD2_OFF 16
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_BIT_EGRESS_XOFF_THRLD_MSK  0x0000ff00
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_BIT_EGRESS_XOFF_THRLD_OFF  8
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_BIT_UNPACK_XOFF_THRLD_MSK  0x000000ff
#define RGFPF96_CORE_REG_GFP_CHANNEL_CONFIGURATION_4_BIT_UNPACK_XOFF_THRLD_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG */
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_N_MAX    6
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_N_SIZE   7
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_N_OFFSET 0x4
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_M_MIN    0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_M_MAX    95
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_M_SIZE   96
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_INDEX_M_OFFSET 0x100

/*-----------------------------------------------------------------------------.
 | Register (0x00031020 + (N) * 0x4 + (M) * 0x100) FRM_LEN_STAT_COUNTER_CONFIG |
 +-----------------------------------------------------------------------------+
 | bit  31:16 R/W  MINFL_TR                                                    |
 | bit  15:0  R/W  MAXFL_TR                                                    |
 +----------------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_BIT_MINFL_TR_MSK   0xffff0000
#define RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_BIT_MINFL_TR_OFF   16
#define RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_BIT_MAXFL_TR_MSK   0x0000ffff
#define RGFPF96_CORE_REG_FRM_LEN_STAT_COUNTER_CONFIG_BIT_MAXFL_TR_OFF   0

/* index definitions for PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION */
#define PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_INDEX_N_MAX    95
#define PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_INDEX_N_SIZE   96
#define PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_INDEX_N_OFFSET 0x100

/*----------------------------------------------------------------------.
 | Register (0x0003103c + (N) * 0x100) FRAME_LENGTH_CHECK_CONFIGURATION |
 +----------------------------------------------------------------------+
 | bit  31:16 R/W  MAXFL                                                |
 | bit  15:0  R/W  MINFL                                                |
 +---------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_BIT_MAXFL_MSK      0xffff0000
#define RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_BIT_MAXFL_OFF      16
#define RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_BIT_MINFL_MSK      0x0000ffff
#define RGFPF96_CORE_REG_FRAME_LENGTH_CHECK_CONFIGURATION_BIT_MINFL_OFF      0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET */
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_INDEX_N_OFFSET   0x100

/*----------------------------------------------------------------.
 | Register (0x00031040 + (N) * 0x100) CPU_DATA_FIFO_STATUS_RESET |
 +----------------------------------------------------------------+
 | bit  31   R/W  CPU_RST                                         |
 | bit  15:8 R    CPU_NUMBYTES                                    |
 | bit  3    R    CPU_ERR_AV                                      |
 | bit  2    R    CPU_EOF_AV                                      |
 | bit  1    R    CPU_SOF_AV                                      |
 | bit  0    R    CPU_DATA_AV                                     |
 +---------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_UNUSED_MASK      0x7fff00f0
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_RST_MSK      0x80000000
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_RST_OFF      31
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_NUMBYTES_MSK 0x0000ff00
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_NUMBYTES_OFF 8
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_ERR_AV_MSK   0x00000008
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_ERR_AV_OFF   3
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_EOF_AV_MSK   0x00000004
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_EOF_AV_OFF   2
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_SOF_AV_MSK   0x00000002
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_SOF_AV_OFF   1
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_DATA_AV_MSK  0x00000001
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_STATUS_RESET_BIT_CPU_DATA_AV_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA */
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_INDEX_N_MAX    95
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_INDEX_N_SIZE   96
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_INDEX_N_OFFSET 0x100

/*--------------------------------------------------------.
 | Register (0x00031044 + (N) * 0x100) CPU_DATA_FIFO_DATA |
 +--------------------------------------------------------+
 | bit  31:24 R  CPU_BYTE3                                |
 | bit  23:16 R  CPU_BYTE2                                |
 | bit  15:8  R  CPU_BYTE1                                |
 | bit  7:0   R  CPU_BYTE0                                |
 +-------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE3_MSK  0xff000000
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE3_OFF  24
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE2_MSK  0x00ff0000
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE2_OFF  16
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE1_MSK  0x0000ff00
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE1_OFF  8
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE0_MSK  0x000000ff
#define RGFPF96_CORE_REG_CPU_DATA_FIFO_DATA_BIT_CPU_BYTE0_OFF  0

/* index definitions for PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG */
#define PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG_INDEX_N_MAX    95
#define PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG_INDEX_N_SIZE   96
#define PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG_INDEX_N_OFFSET 0x100

/*----------------------------------------------------.
 | Register (0x00031048 + (N) * 0x100) PAUSE_TIME_REG |
 +----------------------------------------------------+
 | bit  15:0 R  PAUSE_TIME                            |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PAUSE_TIME_REG_UNUSED_MASK    0xffff0000
#define RGFPF96_CORE_REG_PAUSE_TIME_REG_BIT_PAUSE_TIME_MSK 0x0000ffff
#define RGFPF96_CORE_REG_PAUSE_TIME_REG_BIT_PAUSE_TIME_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0 */
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_INDEX_N_MAX        95
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_INDEX_N_SIZE       96
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_INDEX_N_OFFSET     0x100

/*-------------------------------------------------------------.
 | Register (0x0003104c + (N) * 0x100) GENERIC_FILTER_CONFIG_0 |
 +-------------------------------------------------------------+
 | bit  6:0 R/W  GENERIC_OFFSET                                |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_UNUSED_MASK        0xffffff80
#define RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_BIT_GENERIC_OFFSET_MSK 0x0000007f
#define RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_0_BIT_GENERIC_OFFSET_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1 */
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_INDEX_N_OFFSET   0x100

/*-------------------------------------------------------------.
 | Register (0x00031050 + (N) * 0x100) GENERIC_FILTER_CONFIG_1 |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  GENERIC_MASK                                 |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_BIT_GENERIC_MASK_MSK 0xffffffff
#define RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_1_BIT_GENERIC_MASK_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2 */
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_INDEX_N_MAX       95
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_INDEX_N_SIZE      96
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_INDEX_N_OFFSET    0x100

/*-------------------------------------------------------------.
 | Register (0x00031054 + (N) * 0x100) GENERIC_FILTER_CONFIG_2 |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  GENERIC_VALUE                                |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_BIT_GENERIC_VALUE_MSK 0xffffffff
#define RGFPF96_CORE_REG_GENERIC_FILTER_CONFIG_2_BIT_GENERIC_VALUE_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0 */
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_N_MAX        9
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_N_SIZE       10
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_N_OFFSET     0x8
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_M_MIN        0
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_M_MAX        95
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_M_SIZE       96
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_INDEX_M_OFFSET     0x100

/*------------------------------------------------------------------------.
 | Register (0x00031060 + (N) * 0x8 + (M) * 0x100) FILTER_CONFIGURATION_0 |
 +------------------------------------------------------------------------+
 | bit  31:24 R/W  UPI_VALUE                                              |
 | bit  22:20 R/W  PTI_VALUE                                              |
 | bit  19:16 R/W  EXI_VALUE                                              |
 | bit  15:8  R/W  UPI_MASK                                               |
 | bit  7:5   R/W  PTI_MASK                                               |
 | bit  4:1   R/W  EXI_MASK                                               |
 | bit  0     R/W  SELECT_GENERIC                                         |
 +-----------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_UNUSED_MASK        0x00800000
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_UPI_VALUE_MSK      0xff000000
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_UPI_VALUE_OFF      24
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_PTI_VALUE_MSK      0x00700000
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_PTI_VALUE_OFF      20
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_EXI_VALUE_MSK      0x000f0000
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_EXI_VALUE_OFF      16
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_UPI_MASK_MSK       0x0000ff00
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_UPI_MASK_OFF       8
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_PTI_MASK_MSK       0x000000e0
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_PTI_MASK_OFF       5
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_EXI_MASK_MSK       0x0000001e
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_EXI_MASK_OFF       1
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_SELECT_GENERIC_MSK 0x00000001
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_0_BIT_SELECT_GENERIC_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1 */
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_N_MIN               0
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_N_MAX               9
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_N_SIZE              10
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_N_OFFSET            0x8
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_M_MIN               0
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_M_MAX               95
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_M_SIZE              96
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_INDEX_M_OFFSET            0x100

/*------------------------------------------------------------------------.
 | Register (0x00031064 + (N) * 0x8 + (M) * 0x100) FILTER_CONFIGURATION_1 |
 +------------------------------------------------------------------------+
 | bit  16:13 R/W  MATCH_INDICATOR                                        |
 | bit  12:9  R/W  NEXT_FILTER_OFFSET                                     |
 | bit  8     R/W  CONTINUATION_OPERATOR                                  |
 | bit  7:6   R/W  ACTION                                                 |
 | bit  5:0   R/W  COMPARE_OPERATOR                                       |
 +-----------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_UNUSED_MASK               0xfffe0000
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_MATCH_INDICATOR_MSK       0x0001e000
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_MATCH_INDICATOR_OFF       13
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_NEXT_FILTER_OFFSET_MSK    0x00001e00
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_NEXT_FILTER_OFFSET_OFF    9
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_CONTINUATION_OPERATOR_MSK 0x00000100
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_CONTINUATION_OPERATOR_OFF 8
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_ACTION_MSK                0x000000c0
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_ACTION_OFF                6
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_COMPARE_OPERATOR_MSK      0x0000003f
#define RGFPF96_CORE_REG_FILTER_CONFIGURATION_1_BIT_COMPARE_OPERATOR_OFF      0

/* index definitions for PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB */
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_INDEX_N_MIN         0
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_INDEX_N_MAX         95
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_INDEX_N_SIZE        96
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_INDEX_N_OFFSET      0x80

/*---------------------------------------------------------.
 | Register (0x00037000 + (N) * 0x80) RX_BYTE_CNT_PMON_LSB |
 +---------------------------------------------------------+
 | bit  31:0 R  RX_BYTE_CNT_LSB                            |
 +--------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_UNUSED_MASK         0x00000000
#define RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_BIT_RX_BYTE_CNT_LSB_MSK 0xffffffff
#define RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_LSB_BIT_RX_BYTE_CNT_LSB_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB */
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_INDEX_N_MIN         0
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_INDEX_N_MAX         95
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_INDEX_N_SIZE        96
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_INDEX_N_OFFSET      0x80

/*---------------------------------------------------------.
 | Register (0x00037004 + (N) * 0x80) RX_BYTE_CNT_PMON_MSB |
 +---------------------------------------------------------+
 | bit  7:0 R  RX_BYTE_CNT_MSB                             |
 +--------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_UNUSED_MASK         0xffffff00
#define RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_BIT_RX_BYTE_CNT_MSB_MSK 0x000000ff
#define RGFPF96_CORE_REG_RX_BYTE_CNT_PMON_MSB_BIT_RX_BYTE_CNT_MSB_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_RX_FRM_CNT */
#define PMC_RGFPF96_CORE_REG_RX_FRM_CNT_INDEX_N_MIN    0
#define PMC_RGFPF96_CORE_REG_RX_FRM_CNT_INDEX_N_MAX    95
#define PMC_RGFPF96_CORE_REG_RX_FRM_CNT_INDEX_N_SIZE   96
#define PMC_RGFPF96_CORE_REG_RX_FRM_CNT_INDEX_N_OFFSET 0x80

/*-----------------------------------------------.
 | Register (0x00037008 + (N) * 0x80) RX_FRM_CNT |
 +-----------------------------------------------+
 | bit  31:0 R  RX_FRM_CNT                       |
 +----------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_RX_FRM_CNT_UNUSED_MASK    0x00000000
#define RGFPF96_CORE_REG_RX_FRM_CNT_BIT_RX_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_RX_FRM_CNT_BIT_RX_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_INDEX_N_OFFSET   0x80

/*------------------------------------------------------.
 | Register (0x0003700c + (N) * 0x80) IDLE_FRM_CNT_PMON |
 +------------------------------------------------------+
 | bit  31:0 R  IDLE_FRM_CNT                            |
 +-----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_BIT_IDLE_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_IDLE_FRM_CNT_PMON_BIT_IDLE_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_INDEX_N_OFFSET   0x80

/*------------------------------------------------------.
 | Register (0x00037010 + (N) * 0x80) CTRL_FRM_CNT_PMON |
 +------------------------------------------------------+
 | bit  31:0 R  CTRL_FRM_CNT                            |
 +-----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_BIT_CTRL_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_CTRL_FRM_CNT_PMON_BIT_CTRL_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT */
#define PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT_INDEX_N_OFFSET   0x80

/*-------------------------------------------------.
 | Register (0x00037014 + (N) * 0x80) ERR_CHEC_CNT |
 +-------------------------------------------------+
 | bit  31:0 R  ERR_CHEC_CNT                       |
 +------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_ERR_CHEC_CNT_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_ERR_CHEC_CNT_BIT_ERR_CHEC_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_ERR_CHEC_CNT_BIT_ERR_CHEC_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_INDEX_N_MIN                 0
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_INDEX_N_MAX                 95
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_INDEX_N_SIZE                96
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_INDEX_N_OFFSET              0x80

/*-----------------------------------------------------------------.
 | Register (0x00037018 + (N) * 0x80) SINGL_ERR_CORR_CHEC_CNT_PMON |
 +-----------------------------------------------------------------+
 | bit  31:0 R  SINGL_ERR_CORR_CHEC_CNT                            |
 +----------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_UNUSED_MASK                 0x00000000
#define RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_BIT_SINGL_ERR_CORR_CHEC_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_SINGL_ERR_CORR_CHEC_CNT_PMON_BIT_SINGL_ERR_CORR_CHEC_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_INDEX_N_MIN             0
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_INDEX_N_MAX             95
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_INDEX_N_SIZE            96
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_INDEX_N_OFFSET          0x80

/*-------------------------------------------------------------.
 | Register (0x0003701c + (N) * 0x80) UNPACK_DROP_FRM_CNT_PMON |
 +-------------------------------------------------------------+
 | bit  31:0 R  UNPACK_DROP_FRM_CNT                            |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_UNUSED_MASK             0x00000000
#define RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_BIT_UNPACK_DROP_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_UNPACK_DROP_FRM_CNT_PMON_BIT_UNPACK_DROP_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_INDEX_N_OFFSET   0x80

/*------------------------------------------------------.
 | Register (0x00037020 + (N) * 0x80) ERR_THEC_CNT_PMON |
 +------------------------------------------------------+
 | bit  31:0 R  ERR_THEC_CNT                            |
 +-----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_BIT_ERR_THEC_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_ERR_THEC_CNT_PMON_BIT_ERR_THEC_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_INDEX_N_MIN               0
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_INDEX_N_MAX               95
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_INDEX_N_SIZE              96
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_INDEX_N_OFFSET            0x80

/*---------------------------------------------------------------.
 | Register (0x00037024 + (N) * 0x80) SINGL_ERR_CORR_TE_CNT_PMON |
 +---------------------------------------------------------------+
 | bit  31:0 R  SINGL_ERR_CORR_TE_CNT                            |
 +--------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_UNUSED_MASK               0x00000000
#define RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_BIT_SINGL_ERR_CORR_TE_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_SINGL_ERR_CORR_TE_CNT_PMON_BIT_SINGL_ERR_CORR_TE_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_INDEX_N_MIN             0
#define PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_INDEX_N_MAX             95
#define PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_INDEX_N_SIZE            96
#define PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_INDEX_N_OFFSET          0x80

/*-------------------------------------------------------------.
 | Register (0x00037028 + (N) * 0x80) FILTER_DROP_FRM_CNT_PMON |
 +-------------------------------------------------------------+
 | bit  31:0 R  FILTER_DROP_FRM_CNT                            |
 +------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_UNUSED_MASK             0x00000000
#define RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_BIT_FILTER_DROP_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_FILTER_DROP_FRM_CNT_PMON_BIT_FILTER_DROP_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_INDEX_N_MAX       95
#define PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_INDEX_N_SIZE      96
#define PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_INDEX_N_OFFSET    0x80

/*-------------------------------------------------------.
 | Register (0x0003702c + (N) * 0x80) DEXM_EHEC_CNT_PMON |
 +-------------------------------------------------------+
 | bit  31:0 R  DEXM_EHEC_CNT                            |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_BIT_DEXM_EHEC_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_DEXM_EHEC_CNT_PMON_BIT_DEXM_EHEC_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON_INDEX_N_OFFSET   0x80

/*--------------------------------------------------.
 | Register (0x00037030 + (N) * 0x80) DUPM_CNT_PMON |
 +--------------------------------------------------+
 | bit  31:0 R  DUPM_FRM_CNT                        |
 +-------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DUPM_CNT_PMON_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_DUPM_CNT_PMON_BIT_DUPM_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_DUPM_CNT_PMON_BIT_DUPM_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON_INDEX_N_MIN      0
#define PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON_INDEX_N_MAX      95
#define PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON_INDEX_N_SIZE     96
#define PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON_INDEX_N_OFFSET   0x80

/*--------------------------------------------------.
 | Register (0x00037034 + (N) * 0x80) DGEM_CNT_PMON |
 +--------------------------------------------------+
 | bit  31:0 R  DGEM_FRM_CNT                        |
 +-------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_DGEM_CNT_PMON_UNUSED_MASK      0x00000000
#define RGFPF96_CORE_REG_DGEM_CNT_PMON_BIT_DGEM_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_DGEM_CNT_PMON_BIT_DGEM_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_INDEX_N_MIN     0
#define PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_INDEX_N_MAX     95
#define PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_INDEX_N_SIZE    96
#define PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_INDEX_N_OFFSET  0x80

/*-----------------------------------------------------.
 | Register (0x00037038 + (N) * 0x80) CSF_FRM_CNT_PMON |
 +-----------------------------------------------------+
 | bit  7:0 R  CSF_FRM_CNT                             |
 +----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_UNUSED_MASK     0xffffff00
#define RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_BIT_CSF_FRM_CNT_MSK 0x000000ff
#define RGFPF96_CORE_REG_CSF_FRM_CNT_PMON_BIT_CSF_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_INDEX_N_MIN     0
#define PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_INDEX_N_MAX     95
#define PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_INDEX_N_SIZE    96
#define PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_INDEX_N_OFFSET  0x80

/*-----------------------------------------------------.
 | Register (0x0003703c + (N) * 0x80) ERR_FCS_CNT_PMON |
 +-----------------------------------------------------+
 | bit  31:0 R  ERR_FCS_CNT                            |
 +----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_UNUSED_MASK     0x00000000
#define RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_BIT_ERR_FCS_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_ERR_FCS_CNT_PMON_BIT_ERR_FCS_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_INDEX_N_MAX       95
#define PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_INDEX_N_SIZE      96
#define PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_INDEX_N_OFFSET    0x80

/*-------------------------------------------------------.
 | Register (0x00037040 + (N) * 0x80) PAUSE_FRM_CNT_PMON |
 +-------------------------------------------------------+
 | bit  31:0 R  PAUSE_FRM_CNT                            |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_BIT_PAUSE_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_PAUSE_FRM_CNT_PMON_BIT_PAUSE_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_INDEX_N_MIN     0
#define PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_INDEX_N_MAX     95
#define PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_INDEX_N_SIZE    96
#define PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_INDEX_N_OFFSET  0x80

/*-----------------------------------------------------.
 | Register (0x00037048 + (N) * 0x80) CPU_OVF_CNT_PMON |
 +-----------------------------------------------------+
 | bit  31:0 R  CPU_OVF_CNT                            |
 +----------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_UNUSED_MASK     0x00000000
#define RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_BIT_CPU_OVF_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_CPU_OVF_CNT_PMON_BIT_CPU_OVF_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON_INDEX_N_MAX        95
#define PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON_INDEX_N_SIZE       96
#define PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON_INDEX_N_OFFSET     0x80

/*----------------------------------------------------.
 | Register (0x0003704c + (N) * 0x80) EGRESS_CNT_PMON |
 +----------------------------------------------------+
 | bit  31:0 R  EGRESS_FRM_CNT                        |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_EGRESS_CNT_PMON_UNUSED_MASK        0x00000000
#define RGFPF96_CORE_REG_EGRESS_CNT_PMON_BIT_EGRESS_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_EGRESS_CNT_PMON_BIT_EGRESS_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON_INDEX_N_MIN        0
#define PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON_INDEX_N_MAX        95
#define PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON_INDEX_N_SIZE       96
#define PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON_INDEX_N_OFFSET     0x80

/*----------------------------------------------------.
 | Register (0x00037050 + (N) * 0x80) UNPACK_CNT_PMON |
 +----------------------------------------------------+
 | bit  31:0 R  UNPACK_FRM_CNT                        |
 +---------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_UNPACK_CNT_PMON_UNUSED_MASK        0x00000000
#define RGFPF96_CORE_REG_UNPACK_CNT_PMON_BIT_UNPACK_FRM_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_UNPACK_CNT_PMON_BIT_UNPACK_FRM_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_INDEX_N_MAX       95
#define PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_INDEX_N_SIZE      96
#define PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_INDEX_N_OFFSET    0x80

/*-------------------------------------------------------.
 | Register (0x00037054 + (N) * 0x80) MINFL_CHK_CNT_PMON |
 +-------------------------------------------------------+
 | bit  31:0 R  MINFL_CHK_CNT                            |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_BIT_MINFL_CHK_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_MINFL_CHK_CNT_PMON_BIT_MINFL_CHK_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_INDEX_N_MIN       0
#define PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_INDEX_N_MAX       95
#define PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_INDEX_N_SIZE      96
#define PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_INDEX_N_OFFSET    0x80

/*-------------------------------------------------------.
 | Register (0x00037058 + (N) * 0x80) MAXFL_CHK_CNT_PMON |
 +-------------------------------------------------------+
 | bit  31:0 R  MAXFL_CHK_CNT                            |
 +------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_UNUSED_MASK       0x00000000
#define RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_BIT_MAXFL_CHK_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_MAXFL_CHK_CNT_PMON_BIT_MAXFL_CHK_CNT_OFF 0

/* index definitions for PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON */
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_N_MIN         0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_N_MAX         6
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_N_SIZE        7
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_N_OFFSET      0x4
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_M_MIN         0
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_M_MAX         95
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_M_SIZE        96
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_INDEX_M_OFFSET      0x80

/*----------------------------------------------------------------------.
 | Register (0x00037060 + (N) * 0x4 + (M) * 0x80) FRM_LEN_STAT_CNT_PMON |
 +----------------------------------------------------------------------+
 | bit  31:0 R  FL_CHK_STAT_CNT                                         |
 +---------------------------------------------------------------------*/
#define PMC_RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_UNUSED_MASK         0x00000000
#define RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_BIT_FL_CHK_STAT_CNT_MSK 0xffffffff
#define RGFPF96_CORE_REG_FRM_LEN_STAT_CNT_PMON_BIT_FL_CHK_STAT_CNT_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_RGFPF_REGS_H */
