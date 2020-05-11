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
 *     odtu_dmx block
 *****************************************************************************/
#ifndef _ODTU_DMX_REGS_H
#define _ODTU_DMX_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_ODTU_DMX_REG_HO_CHANNEL_RESET( N )          (0x00000000 + (N) * 0x10)
#define PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG( N )      (0x00000004 + (N) * 0x10)
#define PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE( N )        (0x00000008 + (N) * 0x10)
#define PMC_ODTU_DMX_REG_LO_CHANNEL_RESET( N )          (0x00000600 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG( N )         (0x00000780 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_FORCE_CONFIG_REG               0x00000900
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN( N )      (0x00000904 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_EN( N )           (0x00000910 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DPLM_INT_EN( N )               (0x0000091c + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DMSIM_INT_EN( N )              (0x00000928 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN( N )       (0x00000934 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DLOOMFI_INT_EN                 0x00000940
#define PMC_ODTU_DMX_REG_OOM_OM_INT_EN                  0x00000944
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN( N )        (0x0000094c + (N) * 0x4)
#define PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN      0x00000958
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN( N )        (0x0000095c + (N) * 0x4)
#define PMC_ODTU_DMX_REG_CND_MAX_INT_EN( N )            (0x00000968 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E( N )       (0x00000974 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN( N )         (0x00000980 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN( N )       (0x0000098c + (N) * 0x4)
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN( N )       (0x00000998 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT( N )     (0x000009a4 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_HAO_STAT_INT( N )              (0x000009b0 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DPLM_INT( N )                  (0x000009bc + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DMSIM_INT( N )                 (0x000009c8 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT( N )          (0x000009d4 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DLOOMFI_INT                    0x000009e0
#define PMC_ODTU_DMX_REG_OOM_IM_INT                     0x000009e4
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT( N )           (0x000009ec + (N) * 0x4)
#define PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT    0x000009f8
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT( N )       (0x000009fc + (N) * 0x4)
#define PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT( N ) (0x00000a08 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT( N )    (0x00000a14 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT( N )        (0x00000a20 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT( N )      (0x00000a2c + (N) * 0x4)
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT( N )      (0x00000a38 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DPLM_INT_VAL( N )              (0x00000a44 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DMSIM_INT_VAL( N )             (0x00000a50 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_DLOOMFI_INT_VAL                0x00000a68
#define PMC_ODTU_DMX_REG_OOM_IM_INT_VAL                 0x00000a6c
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL( N )       (0x00000a74 + (N) * 0x4)
#define PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1            0x00000a80
#define PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2            0x00000a84
#define PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG        0x00000a88
#define PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_REG             0x00000a8c
#define PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG( N )      (0x00000a94 + (N) * 0x4)

/* index definitions for PMC_ODTU_DMX_REG_HO_CHANNEL_RESET */
#define PMC_ODTU_DMX_REG_HO_CHANNEL_RESET_INDEX_N_MIN      0
#define PMC_ODTU_DMX_REG_HO_CHANNEL_RESET_INDEX_N_MAX      95
#define PMC_ODTU_DMX_REG_HO_CHANNEL_RESET_INDEX_N_SIZE     96
#define PMC_ODTU_DMX_REG_HO_CHANNEL_RESET_INDEX_N_OFFSET   0x10

/*-----------------------------------------------------.
 | Register (0x00000000 + (N) * 0x10) HO_CHANNEL_RESET |
 +-----------------------------------------------------+
 | bit  0 R/W  HO_CHNL_RSTB                            |
 +----------------------------------------------------*/
#define PMC_ODTU_DMX_REG_HO_CHANNEL_RESET_UNUSED_MASK      0xfffffffe
#define ODTU_DMX_REG_HO_CHANNEL_RESET_BIT_HO_CHNL_RSTB_MSK 0x00000001
#define ODTU_DMX_REG_HO_CHANNEL_RESET_BIT_HO_CHNL_RSTB_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG */
#define PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG_INDEX_N_MAX    95
#define PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG_INDEX_N_SIZE   96
#define PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG_INDEX_N_OFFSET 0x10

/*---------------------------------------------------------.
 | Register (0x00000004 + (N) * 0x10) PASS_THROUGH_CFG_REG |
 +---------------------------------------------------------+
 | bit  0 R/W  PASSTHRU                                    |
 +--------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_PASS_THROUGH_CFG_REG_UNUSED_MASK    0xfffffffe
#define ODTU_DMX_REG_PASS_THROUGH_CFG_REG_BIT_PASSTHRU_MSK   0x00000001
#define ODTU_DMX_REG_PASS_THROUGH_CFG_REG_BIT_PASSTHRU_OFF   0

/* index definitions for PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE */
#define PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE_INDEX_N_MIN           0
#define PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE_INDEX_N_MAX           95
#define PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE_INDEX_N_SIZE          96
#define PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE_INDEX_N_OFFSET        0x10

/*-------------------------------------------------------.
 | Register (0x00000008 + (N) * 0x10) NON_INTRUSIVE_MODE |
 +-------------------------------------------------------+
 | bit  0 R/W  NON_INTRUSVE_MODE                         |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_NON_INTRUSIVE_MODE_UNUSED_MASK           0xfffffffe
#define ODTU_DMX_REG_NON_INTRUSIVE_MODE_BIT_NON_INTRUSVE_MODE_MSK 0x00000001
#define ODTU_DMX_REG_NON_INTRUSIVE_MODE_BIT_NON_INTRUSVE_MODE_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_LO_CHANNEL_RESET */
#define PMC_ODTU_DMX_REG_LO_CHANNEL_RESET_INDEX_N_MIN      0
#define PMC_ODTU_DMX_REG_LO_CHANNEL_RESET_INDEX_N_MAX      95
#define PMC_ODTU_DMX_REG_LO_CHANNEL_RESET_INDEX_N_SIZE     96
#define PMC_ODTU_DMX_REG_LO_CHANNEL_RESET_INDEX_N_OFFSET   0x4

/*----------------------------------------------------.
 | Register (0x00000600 + (N) * 0x4) LO_CHANNEL_RESET |
 +----------------------------------------------------+
 | bit  0 R/W  LO_CHNL_RSTB                           |
 +---------------------------------------------------*/
#define PMC_ODTU_DMX_REG_LO_CHANNEL_RESET_UNUSED_MASK      0xfffffffe
#define ODTU_DMX_REG_LO_CHANNEL_RESET_BIT_LO_CHNL_RSTB_MSK 0x00000001
#define ODTU_DMX_REG_LO_CHANNEL_RESET_BIT_LO_CHNL_RSTB_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG */
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG_INDEX_N_MIN       0
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG_INDEX_N_MAX       95
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG_INDEX_N_SIZE      96
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG_INDEX_N_OFFSET    0x4

/*-----------------------------------------------------.
 | Register (0x00000780 + (N) * 0x4) CONFIG_UPDATE_REG |
 +-----------------------------------------------------+
 | bit  1 R/W  CONFIG_MODE                             |
 | bit  0 R/W  CONFIG_UPDATE                           |
 +----------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_REG_UNUSED_MASK       0xfffffffc
#define ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_MSK   0x00000002
#define ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_MODE_OFF   1
#define ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_MSK 0x00000001
#define ODTU_DMX_REG_CONFIG_UPDATE_REG_BIT_CONFIG_UPDATE_OFF 0

/*--------------------------------------.
 | Register 0x00000900 FORCE_CONFIG_REG |
 +--------------------------------------+
 | bit  1 R/W  FORCE_CONFIG_ADDR        |
 | bit  0 R/W  FORCE_CONFIG             |
 +-------------------------------------*/
#define PMC_ODTU_DMX_REG_FORCE_CONFIG_REG_UNUSED_MASK           0xfffffffc
#define ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_ADDR_MSK 0x00000002
#define ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_ADDR_OFF 1
#define ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_MSK      0x00000001
#define ODTU_DMX_REG_FORCE_CONFIG_REG_BIT_FORCE_CONFIG_OFF      0

/* index definitions for PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN */
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_INDEX_N_MIN             0
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_INDEX_N_MAX             2
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_INDEX_N_SIZE            3
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_INDEX_N_OFFSET          0x4

/*--------------------------------------------------------.
 | Register (0x00000904 + (N) * 0x4) CONFIG_UPDATE_DNE_EN |
 +--------------------------------------------------------+
 | bit  31:0 R/W  CONFIG_UPDATE_DNE_E                     |
 +-------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_UNUSED_MASK             0x00000000
#define ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_BIT_CONFIG_UPDATE_DNE_E_MSK 0xffffffff
#define ODTU_DMX_REG_CONFIG_UPDATE_DNE_EN_BIT_CONFIG_UPDATE_DNE_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_HAO_STAT_INT_EN */
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_EN_INDEX_N_MIN          0
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_EN_INDEX_N_MAX          2
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_EN_INDEX_N_SIZE         3
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_EN_INDEX_N_OFFSET       0x4

/*---------------------------------------------------.
 | Register (0x00000910 + (N) * 0x4) HAO_STAT_INT_EN |
 +---------------------------------------------------+
 | bit  31:0 R/W  HAO_STATUS_AVL_E                   |
 +--------------------------------------------------*/
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_EN_UNUSED_MASK          0x00000000
#define ODTU_DMX_REG_HAO_STAT_INT_EN_BIT_HAO_STATUS_AVL_E_MSK 0xffffffff
#define ODTU_DMX_REG_HAO_STAT_INT_EN_BIT_HAO_STATUS_AVL_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_DPLM_INT_EN */
#define PMC_ODTU_DMX_REG_DPLM_INT_EN_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_DPLM_INT_EN_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_DPLM_INT_EN_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_DPLM_INT_EN_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x0000091c + (N) * 0x4) DPLM_INT_EN |
 +-----------------------------------------------+
 | bit  31:0 R/W  DPLM_E                         |
 +----------------------------------------------*/
#define PMC_ODTU_DMX_REG_DPLM_INT_EN_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_DPLM_INT_EN_BIT_DPLM_E_MSK     0xffffffff
#define ODTU_DMX_REG_DPLM_INT_EN_BIT_DPLM_E_OFF     0

/* index definitions for PMC_ODTU_DMX_REG_DMSIM_INT_EN */
#define PMC_ODTU_DMX_REG_DMSIM_INT_EN_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_DMSIM_INT_EN_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_DMSIM_INT_EN_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_DMSIM_INT_EN_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x00000928 + (N) * 0x4) DMSIM_INT_EN |
 +------------------------------------------------+
 | bit  31:0 R/W  DMSIM_E                         |
 +-----------------------------------------------*/
#define PMC_ODTU_DMX_REG_DMSIM_INT_EN_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_DMSIM_INT_EN_BIT_DMSIM_E_MSK    0xffffffff
#define ODTU_DMX_REG_DMSIM_INT_EN_BIT_DMSIM_E_OFF    0

/* index definitions for PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN */
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_INDEX_N_MIN        0
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_INDEX_N_MAX        2
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_INDEX_N_SIZE       3
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_INDEX_N_OFFSET     0x4

/*-------------------------------------------------------.
 | Register (0x00000934 + (N) * 0x4) STS_SW_OVFLW_INT_EN |
 +-------------------------------------------------------+
 |                                                       |
 | bit  31:0 R/W  STS_SW_OVFLW_E                         |
 |                                                       |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_UNUSED_MASK        0x00000000
#define ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_BIT_STS_SW_OVFLW_E_MSK 0xffffffff
#define ODTU_DMX_REG_STS_SW_OVFLW_INT_EN_BIT_STS_SW_OVFLW_E_OFF 0

/*------------------------------------.
 | Register 0x00000940 DLOOMFI_INT_EN |
 +------------------------------------+
 | bit  0 R/W  DLOOMFI_E              |
 +-----------------------------------*/
#define PMC_ODTU_DMX_REG_DLOOMFI_INT_EN_UNUSED_MASK   0xfffffffe
#define ODTU_DMX_REG_DLOOMFI_INT_EN_BIT_DLOOMFI_E_MSK 0x00000001
#define ODTU_DMX_REG_DLOOMFI_INT_EN_BIT_DLOOMFI_E_OFF 0

/*-----------------------------------.
 | Register 0x00000944 OOM_OM_INT_EN |
 +-----------------------------------+
 | bit  0 R/W  OOM_IM_E              |
 +----------------------------------*/
#define PMC_ODTU_DMX_REG_OOM_OM_INT_EN_UNUSED_MASK  0xfffffffe
#define ODTU_DMX_REG_OOM_OM_INT_EN_BIT_OOM_IM_E_MSK 0x00000001
#define ODTU_DMX_REG_OOM_OM_INT_EN_BIT_OOM_IM_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN */
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_INDEX_N_MIN       0
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_INDEX_N_MAX       2
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_INDEX_N_SIZE      3
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_INDEX_N_OFFSET    0x4

/*------------------------------------------------------.
 | Register (0x0000094c + (N) * 0x4) SSF_SF_RCVD_INT_EN |
 +------------------------------------------------------+
 | bit  31:0 R/W  SSF_SF_RCVD_E                         |
 +-----------------------------------------------------*/
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_UNUSED_MASK       0x00000000
#define ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_BIT_SSF_SF_RCVD_E_MSK 0xffffffff
#define ODTU_DMX_REG_SSF_SF_RCVD_INT_EN_BIT_SSF_SF_RCVD_E_OFF 0

/*-----------------------------------------------.
 | Register 0x00000958 RCOH_STAT_TRANSFR_DONE_EN |
 +-----------------------------------------------+
 | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_E            |
 +----------------------------------------------*/
#define PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_UNUSED_MASK                0xfffffffe
#define ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_MSK 0x00000001
#define ODTU_DMX_REG_RCOH_STAT_TRANSFR_DONE_EN_BIT_RCOH_TRNSFR_SHDW_DNE_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN */
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_INDEX_N_MIN        0
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_INDEX_N_MAX        2
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_INDEX_N_SIZE       3
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_INDEX_N_OFFSET     0x4

/*------------------------------------------------------.
 | Register (0x0000095c + (N) * 0x4) CM_OUT_OF_LIMIT_EN |
 +------------------------------------------------------+
 | bit  31:0 R/W  CM_OUT_LIMIT_E                        |
 +-----------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_UNUSED_MASK        0x00000000
#define ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_BIT_CM_OUT_LIMIT_E_MSK 0xffffffff
#define ODTU_DMX_REG_CM_OUT_OF_LIMIT_EN_BIT_CM_OUT_LIMIT_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CND_MAX_INT_EN */
#define PMC_ODTU_DMX_REG_CND_MAX_INT_EN_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_CND_MAX_INT_EN_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_CND_MAX_INT_EN_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_CND_MAX_INT_EN_INDEX_N_OFFSET 0x4

/*--------------------------------------------------.
 | Register (0x00000968 + (N) * 0x4) CND_MAX_INT_EN |
 +--------------------------------------------------+
 | bit  31:0 R/W  CND_MAX_E                         |
 +-------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CND_MAX_INT_EN_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_CND_MAX_INT_EN_BIT_CND_MAX_E_MSK  0xffffffff
#define ODTU_DMX_REG_CND_MAX_INT_EN_BIT_CND_MAX_E_OFF  0

/* index definitions for PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E */
#define PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_INDEX_N_MIN             0
#define PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_INDEX_N_MAX             2
#define PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_INDEX_N_SIZE            3
#define PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_INDEX_N_OFFSET          0x4

/*-------------------------------------------------------.
 | Register (0x00000974 + (N) * 0x4) JC_NOMTCH_LOSSYNC_E |
 +-------------------------------------------------------+
 | bit  31:0 R/W  JC_NOMTCH_LOSSYNC_E                    |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_UNUSED_MASK             0x00000000
#define ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_BIT_JC_NOMTCH_LOSSYNC_E_MSK 0xffffffff
#define ODTU_DMX_REG_JC_NOMTCH_LOSSYNC_E_BIT_JC_NOMTCH_LOSSYNC_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN */
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN_INDEX_N_MIN          0
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN_INDEX_N_MAX          2
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN_INDEX_N_SIZE         3
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN_INDEX_N_OFFSET       0x4

/*-----------------------------------------------------.
 | Register (0x00000980 + (N) * 0x4) CM_CND_CRC_ERR_EN |
 +-----------------------------------------------------+
 | bit  31:0 R/W  CM_CND_CRC_ERR_E                     |
 +----------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_EN_UNUSED_MASK          0x00000000
#define ODTU_DMX_REG_CM_CND_CRC_ERR_EN_BIT_CM_CND_CRC_ERR_E_MSK 0xffffffff
#define ODTU_DMX_REG_CM_CND_CRC_ERR_EN_BIT_CM_CND_CRC_ERR_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN */
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_INDEX_N_MIN            0
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_INDEX_N_MAX            2
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_INDEX_N_SIZE           3
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_INDEX_N_OFFSET         0x4

/*-------------------------------------------------------.
 | Register (0x0000098c + (N) * 0x4) ERLY_PSERV_EXPRY_EN |
 +-------------------------------------------------------+
 | bit  31:0 R/W  ERLY_PSERV_EXPRY_E                     |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_UNUSED_MASK            0x00000000
#define ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_BIT_ERLY_PSERV_EXPRY_E_MSK 0xffffffff
#define ODTU_DMX_REG_ERLY_PSERV_EXPRY_EN_BIT_ERLY_PSERV_EXPRY_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN */
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_INDEX_N_MIN            0
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_INDEX_N_MAX            2
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_INDEX_N_SIZE           3
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_INDEX_N_OFFSET         0x4

/*-------------------------------------------------------.
 | Register (0x00000998 + (N) * 0x4) LATE_PSERV_EXPRY_EN |
 +-------------------------------------------------------+
 | bit  31:0 R/W  LATE_PSERV_EXPRY_E                     |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_UNUSED_MASK            0x00000000
#define ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_BIT_LATE_PSERV_EXPRY_E_MSK 0xffffffff
#define ODTU_DMX_REG_LATE_PSERV_EXPRY_EN_BIT_LATE_PSERV_EXPRY_E_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT */
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_INDEX_N_MIN             0
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_INDEX_N_MAX             2
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_INDEX_N_SIZE            3
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_INDEX_N_OFFSET          0x4

/*---------------------------------------------------------.
 | Register (0x000009a4 + (N) * 0x4) CONFIG_UPDATE_DNE_INT |
 +---------------------------------------------------------+
 | bit  31:0 R/W  CONFIG_UPDATE_DNE_I                      |
 +--------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_UNUSED_MASK             0x00000000
#define ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_BIT_CONFIG_UPDATE_DNE_I_MSK 0xffffffff
#define ODTU_DMX_REG_CONFIG_UPDATE_DNE_INT_BIT_CONFIG_UPDATE_DNE_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_HAO_STAT_INT */
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_INDEX_N_MIN          0
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_INDEX_N_MAX          2
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_INDEX_N_SIZE         3
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_INDEX_N_OFFSET       0x4

/*------------------------------------------------.
 | Register (0x000009b0 + (N) * 0x4) HAO_STAT_INT |
 +------------------------------------------------+
 | bit  31:0 R/W  HAO_STATUS_AVL_I                |
 +-----------------------------------------------*/
#define PMC_ODTU_DMX_REG_HAO_STAT_INT_UNUSED_MASK          0x00000000
#define ODTU_DMX_REG_HAO_STAT_INT_BIT_HAO_STATUS_AVL_I_MSK 0xffffffff
#define ODTU_DMX_REG_HAO_STAT_INT_BIT_HAO_STATUS_AVL_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_DPLM_INT */
#define PMC_ODTU_DMX_REG_DPLM_INT_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_DPLM_INT_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_DPLM_INT_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_DPLM_INT_INDEX_N_OFFSET 0x4

/*--------------------------------------------.
 | Register (0x000009bc + (N) * 0x4) DPLM_INT |
 +--------------------------------------------+
 | bit  31:0 R/W  DPLM_I                      |
 +-------------------------------------------*/
#define PMC_ODTU_DMX_REG_DPLM_INT_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_DPLM_INT_BIT_DPLM_I_MSK     0xffffffff
#define ODTU_DMX_REG_DPLM_INT_BIT_DPLM_I_OFF     0

/* index definitions for PMC_ODTU_DMX_REG_DMSIM_INT */
#define PMC_ODTU_DMX_REG_DMSIM_INT_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_DMSIM_INT_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_DMSIM_INT_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_DMSIM_INT_INDEX_N_OFFSET 0x4

/*---------------------------------------------.
 | Register (0x000009c8 + (N) * 0x4) DMSIM_INT |
 +---------------------------------------------+
 | bit  31:0 R/W  DMSIM_I                      |
 +--------------------------------------------*/
#define PMC_ODTU_DMX_REG_DMSIM_INT_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_DMSIM_INT_BIT_DMSIM_I_MSK    0xffffffff
#define ODTU_DMX_REG_DMSIM_INT_BIT_DMSIM_I_OFF    0

/* index definitions for PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT */
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_INDEX_N_MIN        0
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_INDEX_N_MAX        2
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_INDEX_N_SIZE       3
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_INDEX_N_OFFSET     0x4

/*----------------------------------------------------.
 | Register (0x000009d4 + (N) * 0x4) STS_SW_OVFLW_INT |
 +----------------------------------------------------+
 |                                                    |
 | bit  31:0 R/W  STS_SW_OVFLW_I                      |
 |                                                    |
 +---------------------------------------------------*/
#define PMC_ODTU_DMX_REG_STS_SW_OVFLW_INT_UNUSED_MASK        0x00000000
#define ODTU_DMX_REG_STS_SW_OVFLW_INT_BIT_STS_SW_OVFLW_I_MSK 0xffffffff
#define ODTU_DMX_REG_STS_SW_OVFLW_INT_BIT_STS_SW_OVFLW_I_OFF 0

/*---------------------------------.
 | Register 0x000009e0 DLOOMFI_INT |
 +---------------------------------+
 | bit  0 R/W  DLOOMFI_I           |
 +--------------------------------*/
#define PMC_ODTU_DMX_REG_DLOOMFI_INT_UNUSED_MASK   0xfffffffe
#define ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_MSK 0x00000001
#define ODTU_DMX_REG_DLOOMFI_INT_BIT_DLOOMFI_I_OFF 0

/*--------------------------------.
 | Register 0x000009e4 OOM_IM_INT |
 +--------------------------------+
 | bit  0 R/W  OOM_IM_I           |
 +-------------------------------*/
#define PMC_ODTU_DMX_REG_OOM_IM_INT_UNUSED_MASK  0xfffffffe
#define ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_MSK 0x00000001
#define ODTU_DMX_REG_OOM_IM_INT_BIT_OOM_IM_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT */
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_INDEX_N_MIN       0
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_INDEX_N_MAX       2
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_INDEX_N_SIZE      3
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_INDEX_N_OFFSET    0x4

/*---------------------------------------------------.
 | Register (0x000009ec + (N) * 0x4) SSF_SF_RCVD_INT |
 +---------------------------------------------------+
 | bit  31:0 R/W  SSF_SF_RCVD_I                      |
 +--------------------------------------------------*/
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_UNUSED_MASK       0x00000000
#define ODTU_DMX_REG_SSF_SF_RCVD_INT_BIT_SSF_SF_RCVD_I_MSK 0xffffffff
#define ODTU_DMX_REG_SSF_SF_RCVD_INT_BIT_SSF_SF_RCVD_I_OFF 0

/*-------------------------------------------------.
 | Register 0x000009f8 RCOH_STAT_TRANSFER_DONE_INT |
 +-------------------------------------------------+
 | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_I              |
 +------------------------------------------------*/
#define PMC_ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_UNUSED_MASK                0xfffffffe
#define ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_MSK 0x00000001
#define ODTU_DMX_REG_RCOH_STAT_TRANSFER_DONE_INT_BIT_RCOH_TRNSFR_SHDW_DNE_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT */
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_INDEX_N_MIN        0
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_INDEX_N_MAX        2
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_INDEX_N_SIZE       3
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_INDEX_N_OFFSET     0x4

/*-------------------------------------------------------.
 | Register (0x000009fc + (N) * 0x4) CM_OUT_OF_LIMIT_INT |
 +-------------------------------------------------------+
 | bit  31:0 R/W  CM_OUT_LIMIT_I                         |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_UNUSED_MASK        0x00000000
#define ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_BIT_CM_OUT_LIMIT_I_MSK 0xffffffff
#define ODTU_DMX_REG_CM_OUT_OF_LIMIT_INT_BIT_CM_OUT_LIMIT_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT */
#define PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_INDEX_N_OFFSET 0x4

/*-------------------------------------------------------------.
 | Register (0x00000a08 + (N) * 0x4) CND_MAX_VAL_VIOLATION_INT |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  CND_MAX_I                                    |
 +------------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_BIT_CND_MAX_I_MSK  0xffffffff
#define ODTU_DMX_REG_CND_MAX_VAL_VIOLATION_INT_BIT_CND_MAX_I_OFF  0

/* index definitions for PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT */
#define PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_INDEX_N_MIN             0
#define PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_INDEX_N_MAX             2
#define PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_INDEX_N_SIZE            3
#define PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_INDEX_N_OFFSET          0x4

/*----------------------------------------------------------.
 | Register (0x00000a14 + (N) * 0x4) JC_NOMATCH_LOSSYNC_INT |
 +----------------------------------------------------------+
 | bit  31:0 R/W  JC_NOMTCH_LOSSYNC_I                       |
 +---------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_UNUSED_MASK             0x00000000
#define ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_BIT_JC_NOMTCH_LOSSYNC_I_MSK 0xffffffff
#define ODTU_DMX_REG_JC_NOMATCH_LOSSYNC_INT_BIT_JC_NOMTCH_LOSSYNC_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT */
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT_INDEX_N_MIN          0
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT_INDEX_N_MAX          2
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT_INDEX_N_SIZE         3
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT_INDEX_N_OFFSET       0x4

/*------------------------------------------------------.
 | Register (0x00000a20 + (N) * 0x4) CM_CND_CRC_ERR_INT |
 +------------------------------------------------------+
 | bit  31:0 R/W  CM_CND_CRC_ERR_I                      |
 +-----------------------------------------------------*/
#define PMC_ODTU_DMX_REG_CM_CND_CRC_ERR_INT_UNUSED_MASK          0x00000000
#define ODTU_DMX_REG_CM_CND_CRC_ERR_INT_BIT_CM_CND_CRC_ERR_I_MSK 0xffffffff
#define ODTU_DMX_REG_CM_CND_CRC_ERR_INT_BIT_CM_CND_CRC_ERR_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT */
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_INDEX_N_MIN            0
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_INDEX_N_MAX            2
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_INDEX_N_SIZE           3
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00000a2c + (N) * 0x4) ERLY_PSERV_EXPRY_INT |
 +--------------------------------------------------------+
 | bit  31:0 R/W  ERLY_PSERV_EXPRY_I                      |
 +-------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_UNUSED_MASK            0x00000000
#define ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_BIT_ERLY_PSERV_EXPRY_I_MSK 0xffffffff
#define ODTU_DMX_REG_ERLY_PSERV_EXPRY_INT_BIT_ERLY_PSERV_EXPRY_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT */
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_INDEX_N_MIN            0
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_INDEX_N_MAX            2
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_INDEX_N_SIZE           3
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_INDEX_N_OFFSET         0x4

/*--------------------------------------------------------.
 | Register (0x00000a38 + (N) * 0x4) LATE_PSERV_EXPRY_INT |
 +--------------------------------------------------------+
 | bit  31:0 R/W  LATE_PSERV_EXPRY_I                      |
 +-------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_UNUSED_MASK            0x00000000
#define ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_BIT_LATE_PSERV_EXPRY_I_MSK 0xffffffff
#define ODTU_DMX_REG_LATE_PSERV_EXPRY_INT_BIT_LATE_PSERV_EXPRY_I_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_DPLM_INT_VAL */
#define PMC_ODTU_DMX_REG_DPLM_INT_VAL_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_DPLM_INT_VAL_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_DPLM_INT_VAL_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_DPLM_INT_VAL_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x00000a44 + (N) * 0x4) DPLM_INT_VAL |
 +------------------------------------------------+
 | bit  31:0 R  DPLM_V                            |
 +-----------------------------------------------*/
#define PMC_ODTU_DMX_REG_DPLM_INT_VAL_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_DPLM_INT_VAL_BIT_DPLM_V_MSK     0xffffffff
#define ODTU_DMX_REG_DPLM_INT_VAL_BIT_DPLM_V_OFF     0

/* index definitions for PMC_ODTU_DMX_REG_DMSIM_INT_VAL */
#define PMC_ODTU_DMX_REG_DMSIM_INT_VAL_INDEX_N_MIN    0
#define PMC_ODTU_DMX_REG_DMSIM_INT_VAL_INDEX_N_MAX    2
#define PMC_ODTU_DMX_REG_DMSIM_INT_VAL_INDEX_N_SIZE   3
#define PMC_ODTU_DMX_REG_DMSIM_INT_VAL_INDEX_N_OFFSET 0x4

/*-------------------------------------------------.
 | Register (0x00000a50 + (N) * 0x4) DMSIM_INT_VAL |
 +-------------------------------------------------+
 | bit  31:0 R  DMSIM_V                            |
 +------------------------------------------------*/
#define PMC_ODTU_DMX_REG_DMSIM_INT_VAL_UNUSED_MASK    0x00000000
#define ODTU_DMX_REG_DMSIM_INT_VAL_BIT_DMSIM_V_MSK    0xffffffff
#define ODTU_DMX_REG_DMSIM_INT_VAL_BIT_DMSIM_V_OFF    0

/*-------------------------------------.
 | Register 0x00000a68 DLOOMFI_INT_VAL |
 +-------------------------------------+
 | bit  0 R  DLOOMFI_V                 |
 +------------------------------------*/
#define PMC_ODTU_DMX_REG_DLOOMFI_INT_VAL_UNUSED_MASK   0xfffffffe
#define ODTU_DMX_REG_DLOOMFI_INT_VAL_BIT_DLOOMFI_V_MSK 0x00000001
#define ODTU_DMX_REG_DLOOMFI_INT_VAL_BIT_DLOOMFI_V_OFF 0

/*------------------------------------.
 | Register 0x00000a6c OOM_IM_INT_VAL |
 +------------------------------------+
 | bit  0 R  OOM_IM_V                 |
 +-----------------------------------*/
#define PMC_ODTU_DMX_REG_OOM_IM_INT_VAL_UNUSED_MASK  0xfffffffe
#define ODTU_DMX_REG_OOM_IM_INT_VAL_BIT_OOM_IM_V_MSK 0x00000001
#define ODTU_DMX_REG_OOM_IM_INT_VAL_BIT_OOM_IM_V_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL */
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_MIN       0
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_MAX       2
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_SIZE      3
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_OFFSET    0x4

/*-------------------------------------------------------.
 | Register (0x00000a74 + (N) * 0x4) SSF_SF_RCVD_INT_VAL |
 +-------------------------------------------------------+
 | bit  31:0 R  SSF_SF_RCVD_V                            |
 +------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_UNUSED_MASK       0x00000000
#define ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_BIT_SSF_SF_RCVD_V_MSK 0xffffffff
#define ODTU_DMX_REG_SSF_SF_RCVD_INT_VAL_BIT_SSF_SF_RCVD_V_OFF 0

/*-----------------------------------------.
 | Register 0x00000a80 INTERRUPT_SUMMARY_1 |
 +-----------------------------------------+
 | bit  31:0 R  INTR_SUMMARY_1             |
 +----------------------------------------*/
#define PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_1_UNUSED_MASK        0x00000000
#define ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_MSK 0xffffffff
#define ODTU_DMX_REG_INTERRUPT_SUMMARY_1_BIT_INTR_SUMMARY_1_OFF 0

/*-----------------------------------------.
 | Register 0x00000a84 INTERRUPT_SUMMARY_2 |
 +-----------------------------------------+
 | bit  6:0 R  INTR_SUMMARY_2              |
 +----------------------------------------*/
#define PMC_ODTU_DMX_REG_INTERRUPT_SUMMARY_2_UNUSED_MASK        0xffffff80
#define ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_MSK 0x0000007f
#define ODTU_DMX_REG_INTERRUPT_SUMMARY_2_BIT_INTR_SUMMARY_2_OFF 0

/*---------------------------------------------.
 | Register 0x00000a88 CONFIG_ADDR_ST_CTRL_REG |
 +---------------------------------------------+
 | bit  14:8 R/W  LO_CONFIG_ADDR_ST_ID         |
 | bit  6:0  R/W  HO_CONFIG_ADDR_ST_ID         |
 +--------------------------------------------*/
#define PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_UNUSED_MASK              0xffff8080
#define ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_MSK 0x00007f00
#define ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_LO_CONFIG_ADDR_ST_ID_OFF 8
#define ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_MSK 0x0000007f
#define ODTU_DMX_REG_CONFIG_ADDR_ST_CTRL_REG_BIT_HO_CONFIG_ADDR_ST_ID_OFF 0

/*----------------------------------------.
 | Register 0x00000a8c CONFIG_ADDR_ST_REG |
 +----------------------------------------+
 | bit  8 R  LO_CONFIG_ADDR_ST            |
 | bit  0 R  HO_CONFIG_ADDR_ST            |
 +---------------------------------------*/
#define PMC_ODTU_DMX_REG_CONFIG_ADDR_ST_REG_UNUSED_MASK           0xfffffefe
#define ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_LO_CONFIG_ADDR_ST_MSK 0x00000100
#define ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_LO_CONFIG_ADDR_ST_OFF 8
#define ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_HO_CONFIG_ADDR_ST_MSK 0x00000001
#define ODTU_DMX_REG_CONFIG_ADDR_ST_REG_BIT_HO_CONFIG_ADDR_ST_OFF 0

/* index definitions for PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG */
#define PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_INDEX_N_MIN           0
#define PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_INDEX_N_MAX           3
#define PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_INDEX_N_SIZE          4
#define PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_INDEX_N_OFFSET        0x4

/*--------------------------------------------------------.
 | Register (0x00000a94 + (N) * 0x4) DUMMY_SCHDL_CTRL_REG |
 +--------------------------------------------------------+
 | bit  22:16 R/W  DUMMY_SCHDL_ID                         |
 | bit  14:8  R/W  DUMMY_SCHDL_CYCLE                      |
 | bit  0     R/W  DUMMY_SCHDL_EN                         |
 +-------------------------------------------------------*/
#define PMC_ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_UNUSED_MASK           0xff8080fe
#define ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_MSK    0x007f0000
#define ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_ID_OFF    16
#define ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_MSK 0x00007f00
#define ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_CYCLE_OFF 8
#define ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_EN_MSK    0x00000001
#define ODTU_DMX_REG_DUMMY_SCHDL_CTRL_REG_BIT_DUMMY_SCHDL_EN_OFF    0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODTU_DMX_REGS_H */
