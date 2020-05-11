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
*     The head file describes the Ethernet Run-Time configuration functionality.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_ENET_H
#define _DIGI_ADPT_ENET_H
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
* ENUM: digi_enet_fault_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet fault. The
*   element terms have a syntax:
*      DIGI_ENET_FT_<application>
*
* ELEMENTS:
*   DIGI_ENET_FT_CLEAR    - fault clear
*   DIGI_ENET_FT_LFI      - local fault indication
*   DIGI_ENET_FT_RFI      - remote fault indication
*   DIGI_ENET_FT_AUTO     - enable ingress fault egress consequential action
*   DIGI_ENET_FT_UNIDIR   - unidirectional ENET clause 66 compliant
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_FT_CLEAR   = 0x0,
    DIGI_ENET_FT_LFI     = 0x1,
    DIGI_ENET_FT_RFI     = 0x2,
    DIGI_ENET_FT_AUTO    = 0x3,
    DIGI_ENET_FT_UNIDIR  = 0x4,
    
    LAST_DIGI_ENET_FT, /* this is for out of range values error handling */
} digi_enet_fault_t;

/*
** Structures and Unions
*/
/*******************************************************************************
* ENUM: digi_enet_vlan_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet VLAN information. 
*
* ELEMENTS:
*   pkt_type             - Packet type.
*   is_vlan_tagged       - TRUE when packet is VLAN-tagged, otherwise FALSE.
*                          Packet VLAN is replaced when is_vlan_tagged is TRUE.
*   vlan_src_cfg_ptr     - VLAN-tagged packet configuration. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*   vlan_id_repl_en      - Enable packet VLAN ID replacement.
*   vlan_id_repl         - VLAN ID value replaced in forwarded packet. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*
*******************************************************************************/
typedef struct
{
    util_global_pkt_type_t pkt_type;
    BOOL                   is_vlan_tagged;
    util_global_vlan_cfg_t vlan_src_cfg;
    BOOL                   vlan_id_repl_en;
    UINT16                 vlan_id_repl;
}digi_enet_vlan_t;

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_enet_mru_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 max_length);
PUBLIC PMC_ERROR adpt_enet_mru_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 *max_length);
PUBLIC PMC_ERROR adpt_enet_ipg_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                UINT32 ipg_length);
PUBLIC PMC_ERROR adpt_enet_ipg_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                UINT32 *ipg_length);
PUBLIC PMC_ERROR adpt_enet_crc_fwd_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_enet_crc_fwd_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *enable);
PUBLIC PMC_ERROR adpt_enet_crc_inst_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_enet_crc_inst_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *enable);
PUBLIC PMC_ERROR adpt_enet_tx_timing_mode_set(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_tx_timing_mode_t mode);
PUBLIC PMC_ERROR adpt_enet_tx_timing_mode_get(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_tx_timing_mode_t *mode);
PUBLIC PMC_ERROR adpt_enet_rx_fc_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 BOOL fc_enable);
PUBLIC PMC_ERROR adpt_enet_rx_fc_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 BOOL *fc_enable);
PUBLIC PMC_ERROR adpt_enet_tx_fc_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 pause_quanta,
                                 UINT32 pause_quanta_thr,
                                 UINT32 fc_thresh,
                                 BOOL fc_enable);
PUBLIC PMC_ERROR adpt_enet_tx_fc_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 UINT32 *pause_quanta,
                                 UINT32 *pause_quanta_thr,
                                 UINT32 *fc_thresh,
                                 BOOL *fc_enable);
PUBLIC PMC_ERROR adpt_enet_fc_status_get(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     BOOL   *rx_pause_frm);
PUBLIC PMC_ERROR adpt_enet_phy_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL phy_enable);
PUBLIC PMC_ERROR adpt_enet_phy_en_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL *phy_enable);
PUBLIC PMC_ERROR adpt_enet_irig_cfg_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   BOOL ingress_enable,
                                   digi_enet_irig_mode_t ingress_mode,
                                   BOOL egress_enable,
                                   digi_enet_irig_mode_t egress_mode);
PUBLIC PMC_ERROR adpt_enet_fault_inst_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir,
                                     digi_enet_fault_t fault_type);
PUBLIC PMC_ERROR adpt_enet_fault_inst_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_port_dir_t dir,
                                    digi_enet_fault_t *fault_type);
PUBLIC PMC_ERROR adpt_enet_test_pkt_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_enet_test_packet_t *test_pkt,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_enet_test_pkt_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *enable);
PUBLIC PMC_ERROR adpt_enet_prbs_set(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_enet_prbs_t prbs_mode,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_enet_prbs_get(UINT32 dev_id,
                                 UINT32 chnl_id,
                                 digi_enet_prbs_t *prbs_mode,
                                 BOOL *enable);
PUBLIC PMC_ERROR adpt_enet_prbs_status_get(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       BOOL *prbs_syn,
                                       UINT32 *err_cnt);
PUBLIC PMC_ERROR adpt_enet_cpu_pkt_inst_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_enet_cpu_pkt_ext_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       BOOL is_reflect,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_enet_cmf_lf_cfg(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_cmf_sw_control_action_t trigger);
PUBLIC PMC_ERROR adpt_enet_cmf_rf_cfg(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_cmf_sw_control_action_t trigger);
PUBLIC PMC_ERROR adpt_enet_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_pkt_type_t pkt_type,
                                         BOOL is_vlan_tagged,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 vlan_id_repl,
                                         BOOL vlan_id_repl_en,
                                         BOOL enable);
PUBLIC PMC_ERROR adpt_enet_mgmt_pkt_fwd_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_pkt_type_t pkt_type,
                                         BOOL *is_vlan_tagged,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 *vlan_id_repl,
                                         BOOL *vlan_id_repl_en,
                                         BOOL *enable);
PUBLIC PMC_ERROR adpt_mgmt_enet_pkt_fwd_cfg(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL vlan_pri_ow,
                                         BOOL enable);
PUBLIC PMC_ERROR adpt_mgmt_enet_pkt_fwd_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         digi_port_dir_t dir,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL *vlan_pri_ow,
                                         BOOL *enable);
PUBLIC PMC_ERROR adpt_cpu_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 vlan_id_repl,
                                         BOOL vlan_id_repl_en,
                                         BOOL enable);
PUBLIC PMC_ERROR adpt_cpu_mgmt_pkt_fwd_get(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         UINT16 *vlan_id_repl,
                                         BOOL *vlan_id_repl_en,
                                         BOOL *enable);
PUBLIC PMC_ERROR adpt_mgmt_cpu_pkt_fwd_cfg(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL enable);
PUBLIC PMC_ERROR adpt_mgmt_cpu_pkt_fwd_get(UINT32 dev_id,
                                         util_global_vlan_cfg_t *vlan_src_cfg,
                                         BOOL *enable);
PUBLIC PMC_ERROR adpt_enet_inbound_ft_en_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         BOOL fault_lf,
                                         BOOL enable);
PUBLIC PMC_ERROR adpt_enet_inbound_ft_en_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         BOOL *fault_lf,
                                         BOOL *enable);
PUBLIC PMC_ERROR adpt_enet_eclass_filter_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_enet_eclass_filter_config_t eclass_cfg);
PUBLIC PMC_ERROR adpt_enet_lpd_pkt_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   UINT32 *pkt_addr);
PUBLIC PMC_ERROR adpt_enet_lpd_release(UINT32 dev_id, 
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir);
PUBLIC PMC_ERROR adpt_enet_lldp_pkt_ext_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       digi_port_dir_t dir,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_enet_lldp_set(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_enet_eclass_encap_t pkt_encap_type,
                                digi_enet_eclass_mode_t lldp_filter_mode,
                                UINT32 eth_type);
PUBLIC PMC_ERROR adpt_enet_lldp_en_set(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_port_dir_t dir,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_enet_idle_inst_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL enable);
PUBLIC PMC_ERROR adpt_enet_err_frm_set(UINT32 dev_id,
                                   BOOL enet_line,
                                   emac_error_t error_type,
                                   BOOL dpi_err_en,
                                   BOOL drop_en);
PUBLIC PMC_ERROR adpt_enet_err_frm_get(UINT32 dev_id,
                                   BOOL enet_line,
                                   emac_error_t error_type,
                                   BOOL *dpi_err_en,
                                   BOOL *drop_en);
PUBLIC PMC_ERROR adpt_enet_err_msk_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL err_fcs_en,
                                    BOOL err_len_en);
PUBLIC PMC_ERROR adpt_enet_err_msk_get(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    BOOL *err_fcs_en,
                                    BOOL *err_len_en);
PUBLIC PMC_ERROR adpt_enet_mac_addr_inst_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         UINT64 mac_addr,
                                         BOOL inst_en);
PUBLIC PMC_ERROR adpt_enet_mac_addr_inst_get(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         UINT64 *mac_addr,
                                         BOOL *inst_en);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_ENET_H */

/* 
** end of file 
*/

