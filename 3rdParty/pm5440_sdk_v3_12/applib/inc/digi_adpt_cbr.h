/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
* --------------------------------------------------------------------------
*   DESCRIPTION:
*     The file describes the FC800/FC1200/GDPS/CPRI Run-Time functionality of 
*     CBRC subsystem.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_CBR_H
#define _DIGI_ADPT_CBR_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_adpt_com.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_rx_lfos_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for LFOS bit mask in RX direction. The
*   element terms have a syntax:
*      DIGI_FC_RX_LFOS_MASK_<application>
*
* ELEMENTS:
*   DIGI_FC_RX_LFOS_MASK_LINK_FAIL      - link failure mask.
*   DIGI_FC_RX_LFOS_MASK_SYNC_LOST      - sync lost mask.
*   DIGI_FC_RX_LFOS_MASK_HIGH_BER       - high ber mask.
*
*******************************************************************************/
typedef enum
{
    DIGI_FC_RX_LFOS_MASK_LINK_FAIL = CBRC_FC1200_RX_LFOS_CFG_MASK_LINK_FAIL_ENABLE,
    DIGI_FC_RX_LFOS_MASK_SYNC_LOST = CBRC_FC1200_RX_LFOS_CFG_MASK_SYNC_LOST_ENABLE,
    DIGI_FC_RX_LFOS_MASK_HIGH_BER  = CBRC_FC1200_RX_LFOS_CFG_MASK_HIGH_BER_ENABLE,
    
    LAST_DIGI_FC_RX_LFOS, /* this is for out of range values error handling */
} digi_rx_lfos_mask_t;

/*******************************************************************************
* ENUM: digi_tx_lfos_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for LFOS bit mask in TX direction. The
*   element terms have a syntax:
*      DIGI_FC_TX_LFOS_MASK_<application>
*
* ELEMENTS:
*   DIGI_FC_TX_LFOS_MASK_LOS_LF      - Loss-of-Signal local fault mask.
*   DIGI_FC_TX_LFOS_MASK_EXT_LF      - external local fault mask.
*
*******************************************************************************/
typedef enum
{
    DIGI_FC_TX_LFOS_MASK_LOS_LF = CBRC_FC1200_TX_LFOS_CFG_MASK_LOS_ENABLE,
    DIGI_FC_TX_LFOS_MASK_EXT_LF = CBRC_FC1200_TX_LFOS_CFG_MASK_EXT_ENABLE,
    
    LAST_DIGI_FC_TX_LFOS, /* this is for out of range values error handling */
} digi_tx_lfos_mask_t;

/*******************************************************************************
* ENUM: digi_dlolb_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DLOLB bit mask. The
*   element terms have a syntax:
*      DIGI_FC_DLOLB_MASK_<application>
*
* ELEMENTS:
*   DIGI_FC_DLOLB_MASK_LOS            - loss of signal mask.
*   DIGI_FC_DLOLB_MASK_LOSS_SYNC      - loss of sync mask.
*   DIGI_FC_DLOLB_MASK_HIGH_BER       - high ber mask.
*
*******************************************************************************/
typedef enum
{
    DIGI_FC_DLOLB_MASK_LOS       = FC1200_PMON_DLOLB_CFG_MASK_LOS_ENABLE,
    DIGI_FC_DLOLB_MASK_LOSS_SYNC = FC1200_PMON_DLOLB_CFG_MASK_LOSS_SYNC_ENABLE,
    DIGI_FC_DLOLB_MASK_HIGH_BER  = FC1200_PMON_DLOLB_CFG_MASK_HIGH_BER_ENABLE,
    
    LAST_DIGI_FC_DLOLB, /* this is for out of range values error handling */
} digi_dlolb_mask_t;

/*******************************************************************************
* ENUM: digi_invalid_blk_mode_t    
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to configure how blocks with invalid block type encodings are
*   handled when encountered during processing.
*   The element terms have a syntax:
*      DIGI_FC_DLOLB_MASK_<application>
*
* ELEMENTS:
*   DIGI_FC_INVALID_BLK_IGNORED    - Blocks with an invalid block
*                                    type are ignored (not processed).
*   DIGI_FC_INVALID_BLK_CONV_IDLE  - Blocks with an invalid block type
*                                    are converted to idle blocks
*                                    before processing.
*   DIGI_FC_INVALID_BLK_CONV_ERROR - Blocks with an invalid block type
*                                    are converted to error blocks
*                                    before processing.
*
*******************************************************************************/
typedef enum 
{
    DIGI_FC_INVALID_BLK_IGNORED    = FC1200_PMON_INVALID_BLOCK_MODE_IGNORED,
    DIGI_FC_INVALID_BLK_CONV_IDLE  = FC1200_PMON_INVALID_BLOCK_MODE_CONV_IDLE,
    DIGI_FC_INVALID_BLK_CONV_ERROR = FC1200_PMON_INVALID_BLOCK_MODE_CONV_ERROR,

    LAST_digi_FC_INVALID_BLK,
} digi_invalid_blk_mode_t;

/*******************************************************************************
* ENUM: digi_cbr_pattern_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Fault patterns for insertion.
*
* ELEMENTS:
*   DIGI_CBR_PATTERN_PN11       - PN-11 pattern
*   DIGI_CBR_PATTERN_MS_AIS     - MS-AIS pattern
*   DIGI_CBR_PATTERN_8B_10B     - 8b/10b pattern. Only valid for 10G
*   DIGI_CBR_PATTERN_64B_66B    - 64b/66b pattern. Only valid for 10G
*
*******************************************************************************/
typedef enum 
{
    DIGI_CBR_PATTERN_PN11    = CBR_FSGM_PATTERN_PN11,
    DIGI_CBR_PATTERN_MS_AIS  = CBR_FSGM_PATTERN_MS_AIS,
    DIGI_CBR_PATTERN_8B_10B  = CBR_FSGM_PATTERN_8B_10B,
    DIGI_CBR_PATTERN_64B_66B = CBR_FSGM_PATTERN_64B_66B,
   
    LAST_DIGI_CBR_PATTERN,
} digi_cbr_pattern_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_fc_prbs_en_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL prbs_enable);
PUBLIC PMC_ERROR adpt_fc_prbs_en_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  BOOL *prbs_enable);
PUBLIC PMC_ERROR adpt_fc_rx_descramble_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_fc_rx_descramble_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_rx_ecb_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 low_ecb,
                                 UINT32 high_ecb,
                                 BOOL   ecb_enable);
PUBLIC PMC_ERROR adpt_fc_rx_ecb_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 *low_ecb,
                                 UINT32 *high_ecb,
                                 BOOL   *ecb_enable);
PUBLIC PMC_ERROR adpt_fc_tx_corrupt_blk_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL conv_enable);
PUBLIC PMC_ERROR adpt_fc_tx_corrupt_blk_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *conv_enable);
PUBLIC PMC_ERROR adpt_fc_rx_invalid_idle_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_fc_rx_invalid_idle_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_rx_lfos_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_rx_lfos_mask_t lfos_mask,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_fc_rx_lfos_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_rx_lfos_mask_t lfos_mask,
                                 BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_tx_lfos_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_tx_lfos_mask_t lfos_mask,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_fc_tx_lfos_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_tx_lfos_mask_t lfos_mask,
                                 BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_lfos_inst_set(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT32 low_lfos,
                                  UINT32 high_lfos,
                                  BOOL enable);
PUBLIC PMC_ERROR adpt_fc_lfos_inst_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  UINT32 *low_lfos,
                                  UINT32 *high_lfos,
                                  BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_dlolb_set(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_port_dir_t dir,
                               digi_dlolb_mask_t dlolb_mask,
                               BOOL enable);
PUBLIC PMC_ERROR adpt_fc_dlolb_get(UINT32 dev_id,
                               UINT32 chnl_id,
                               digi_port_dir_t dir,
                               digi_dlolb_mask_t dlolb_mask,
                               BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_dlolb_inst_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_fc_dlolb_inst_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL *enable);
PUBLIC PMC_ERROR adpt_fc_invalid_blk_mode_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         digi_invalid_blk_mode_t mode);
PUBLIC PMC_ERROR adpt_fc_invalid_blk_mode_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         digi_invalid_blk_mode_t *mode);
PUBLIC PMC_ERROR adpt_fc_dlolb_gen_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL lcv,
                                   BOOL xdet,
                                   BOOL asd,
                                   BOOL density);
PUBLIC PMC_ERROR adpt_fc_dlolb_gen_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL *lcv,
                                   BOOL *xdet,
                                   BOOL *asd,
                                   BOOL *density);
PUBLIC PMC_ERROR adpt_cbr_pn11_mon_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     BOOL enable);
PUBLIC PMC_ERROR adpt_cbr_pn11_mon_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     BOOL *enable);
PUBLIC PMC_ERROR adpt_cbr_pattern_src_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      digi_cbr_pattern_t pattern);
PUBLIC PMC_ERROR adpt_cbr_pattern_src_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      digi_cbr_pattern_t *pattern);
PUBLIC PMC_ERROR adpt_cbr_user_pattern_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      cbr_fsgm_user_fault_pattern_t pattern);
PUBLIC PMC_ERROR adpt_cbr_user_pattern_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       cbr_fsgm_user_fault_pattern_t *pattern);
PUBLIC PMC_ERROR adpt_cbr_pattern_inst_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      BOOL enable);
PUBLIC PMC_ERROR adpt_cbr_pattern_inst_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir,
                                      BOOL *enable);
PUBLIC PMC_ERROR adpt_cbr_cmf_fault_cfg(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_cmf_sw_control_action_t action);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_CBR_H */

/* 
** end of file 
*/

