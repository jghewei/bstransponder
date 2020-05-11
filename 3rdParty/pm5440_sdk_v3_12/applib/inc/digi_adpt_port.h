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
*     This file describes the peripheral port configuration Run-Time function.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_PORT_H
#define _DIGI_ADPT_PORT_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_otn_api.h"
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
* ENUM: digi_ohp_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OHP interface direction. The
*   element terms have a syntax:
*      DIGI_OHP_DIR_<application>
*
* ELEMENTS:
*   DIGI_OHP_DIR_RX         - received direction
*   DIGI_OHP_DIR_TX         - transmitted direction
*   DIGI_OHP_DIR_RX_TX      - received and transmitted direction
*
*******************************************************************************/
typedef enum 
{
    DIGI_OHP_DIR_RX    = 0x0,
    DIGI_OHP_DIR_TX    = 0x1,
    DIGI_OHP_DIR_RX_TX = 0x2,
    
    LAST_DIGI_OHP_DIR
} digi_ohp_dir_t;

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_otn_ohp_en_set(UINT32 dev_id, 
                                   odu_ohp_cfg_t *ohp_cfg,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_otn_ohp_en_get(UINT32 dev_id, 
                                   odu_ohp_inst_t ohp_inst,
                                   odu_ohp_cfg_t *ohp_cfg,
                                   BOOL *enable);
PUBLIC PMC_ERROR adpt_otn_ohp_cfg(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_loc_t location,
                                digi_ohp_dir_t dir,
                                UINT16 oh_mask,
                                BOOL enable);
PUBLIC PMC_ERROR adpt_otn_ohp_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_loc_t location,
                                digi_ohp_dir_t dir,
                                UINT16 *oh_mask,
                                BOOL *enable);
PUBLIC PMC_ERROR adpt_otn_ohp_resize(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_otn_loc_t location,
                                  INT32 resize);
PUBLIC PMC_ERROR adpt_otn_ohp_chnl_num_get(UINT32 dev_id,
                                        UINT32 chnl_id,
                                        digi_otn_loc_t location,
                                        UINT32 *chnl_num,
                                        odu_ohp_port_framer_t *framer_num);
PUBLIC PMC_ERROR adpt_di_rcp_en_set(UINT32 dev_id, 
                                 odu_di_rcp_cfg_t *di_rcp_cfg,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_di_rcp_en_get(UINT32 dev_id,
                                 odu_di_rcp_cfg_t *di_rcp_cfg,
                                 BOOL *enable);
PUBLIC PMC_ERROR adpt_di_rcp_cfg(UINT32 dev_id, UINT32 di_rcp_pdu, BOOL enable);
PUBLIC PMC_ERROR adpt_di_rcp_sdh_counts_cfg(UINT32 dev_id, 
                                        UINT16 cnt_sel, 
                                        BOOL sdh_cnt, 
                                        BOOL enable);
PUBLIC PMC_ERROR adpt_di_rcp_otn_counts_cfg(UINT32 dev_id, BOOL enable);
PUBLIC PMC_ERROR adpt_di_rcp_prbs_cycle_set(UINT32 dev_id, UINT32 prbs_cycle);
PUBLIC PMC_ERROR adpt_tcm_pool_id_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_pos_t position,
                                   UINT32 *tcm_pool_id);
PUBLIC PMC_ERROR adpt_ri_rcp_en_set(UINT32 dev_id, 
                                 UINT32 eth_mac_oui,
                                 UINT8 eth_mac_id,
                                 UINT32 eth_etype,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_ri_rcp_en_get(UINT32 dev_id, 
                                 UINT32 *eth_mac_oui,
                                 UINT8 *eth_mac_id,
                                 UINT32 *eth_etype,
                                 BOOL *enable);
PUBLIC PMC_ERROR adpt_ri_rcp_extract_cfg(UINT32 dev_id, 
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_ri_rcp_extract_get(UINT32 dev_id, 
                                    UINT32 chnl_id,
                                    digi_otn_seg_t *seg_id,
                                    digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg,
                                    BOOL *enable);
PUBLIC PMC_ERROR adpt_ri_rcp_mon_level_cfg(UINT32 dev_id, 
                                       UINT32 chnl_id,
                                       digi_otn_seg_t seg_id);
PUBLIC PMC_ERROR adpt_ri_rcp_dest_get(UINT32 dev_id, 
                                  UINT32 chnl_id,
                                  digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg);
PUBLIC PMC_ERROR adpt_ri_rcp_aps_inst_cfg(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     BOOL inst_src);
PUBLIC PMC_ERROR adpt_ri_rcp_aps_inst_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     BOOL *inst_src);
PUBLIC PMC_ERROR adpt_fege_en_set(UINT32 dev_id, BOOL enable);
PUBLIC PMC_ERROR adpt_fege_en_get(UINT32 dev_id, BOOL *enable);
PUBLIC PMC_ERROR adpt_fege_rx_fc_set(UINT32 dev_id, 
                                 BOOL drop_pause_frm,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_fege_rx_fc_get(UINT32 dev_id, 
                                 BOOL *drop_pause_frm,
                                 BOOL *enable);
PUBLIC PMC_ERROR adpt_fege_frm_size_set(UINT32 dev_id, 
                                    UINT16 txMaxFrameSize,
                                    UINT16 txMinFrameSize,
                                    UINT16 rxMaxFrameSize,
                                    UINT16 rxMinFrameSize);
PUBLIC PMC_ERROR adpt_fege_frm_size_get(UINT32 dev_id, 
                                    UINT16 *txMaxFrameSize,
                                    UINT16 *txMinFrameSize,
                                    UINT16 *rxMaxFrameSize,
                                    UINT16 *rxMinFrameSize);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_PORT_H */

/* 
** end of file 
*/

