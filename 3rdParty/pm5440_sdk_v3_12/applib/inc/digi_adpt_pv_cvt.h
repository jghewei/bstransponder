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
*     The file provides API conversion function for internal usage. Those APIs
*     with pointer parameter will be included here.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_PV_CVT_H
#define _DIGI_ADPT_PV_CVT_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "digi_adpt.h"

/*
** Constants
*/

/*
** Macro Definitions
*/
#define ADPT_BUFFER_SIZE  256
#ifdef PMC_COMEXPRESS
#define ADPT_MAX_USER_DEFINED_THREADS 100
#endif

/*
** Enumerated Types
*/

/*
** Structures and Unions
*/
#ifdef PMC_COMEXPRESS
typedef struct
{
    UINT64 func_p; 
    UINT32 dev_id;
    UINT32 chnl_id; 
    UINT32 usleep_t;
}polling_para_t;
#endif

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_cvt_odu_mux_cfg(UINT32 dev_id,
                                    UINT32 ho_chnl_id,
                                    UINT32 lo_chnl_id,
                                    digi_oduk_t oduk_type,
                                    digi_otn_mapping_type_t mapping_type,
                                    digi_odu_line_payload_t payload_type,
                                    UINT32 ts_mask0,
                                    UINT32 ts_mask1,
                                    UINT32 ts_mask2,
                                    BOOL tp_enable,
                                    DOUBLE oduflex_rate);
PUBLIC PMC_ERROR adpt_cvt_odu_mux_asym_cfg(UINT32 dev_id,
                                         UINT32 ho_chnl_id,
                                         UINT32 lo_chnl_id,
                                         digi_oduk_t oduk_type,
                                         digi_otn_mapping_type_t mapping_type,
                                         digi_odu_line_payload_t payload_type,
                                         digi_otn_dir_t otn_dir,
                                         UINT32 tx_ts_mask0,
                                         UINT32 tx_ts_mask1,
                                         UINT32 tx_ts_mask2,
                                         UINT32 rx_ts_mask0,
                                         UINT32 rx_ts_mask1,
                                         UINT32 rx_ts_mask2,
                                         BOOL tp_enable,
                                         DOUBLE oduflex_rate);
PUBLIC PMC_ERROR adpt_cvt_enet_test_pkt_set(UINT32 dev_id,
                                       UINT32 chnl_id,
                                       UINT16 packet_size,
                                       UINT16 interval,
                                       UINT16 dest_addr_hi,
                                       UINT32 dest_addr_lo,
                                       UINT16 src_addr_hi,
                                       UINT32 src_addr_lo,
                                       UINT16 ether_type,
                                       UINT32 outer_vlan,
                                       UINT32 inner_vlan,
                                       UINT8 payload,
                                       BOOL enable);
PUBLIC PMC_ERROR adpt_buffer_reset();
PUBLIC PMC_ERROR adpt_buffer_set(UINT32 pos, UINT32 value);
PUBLIC PMC_ERROR adpt_cvt_gfp_tx_cmf_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     UINT32 payload_len_type,
                                     UINT32 upi_value,
                                     BOOL   fcs_en,
                                     UINT64 payload);
PUBLIC PMC_ERROR adpt_cvt_gfp_tx_ehec_en_set(UINT32 dev_id,
                                         UINT32 chnl_id,
                                         UINT32 ehec_len,
                                         BOOL ehec_enable);
PUBLIC PMC_ERROR adpt_cvt_otn_tx_tti_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_seg_t seg_id,
                                    digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_cvt_otn_exp_tti_set(UINT32 dev_id,
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_cvt_otn_tx_ftfl_set(UINT32 dev_id,
                                    UINT32 chnl_id,
                                    digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_cvt_otn_tcm_pri_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_otn_loc_t location,
                                      digi_otn_pos_t position);
PUBLIC PMC_ERROR adpt_cvt_sdh_exp_tti_set(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_port_dir_t direction);
PUBLIC PMC_ERROR adpt_cvt_pmon_int_set(UINT32 dev_id, 
                                    BOOL otn_i, 
                                    BOOL enet_i);
PUBLIC PMC_ERROR adpt_cvt_serdes_c8_values_set(UINT32 dev_id, UINT32 lane_id);
PUBLIC PMC_ERROR adpt_cvt_serdes_t8_values_set(UINT32 dev_id, UINT32 lane_id);
PUBLIC PMC_ERROR adpt_cvt_serdes_s16_values_set(UINT32 dev_id,  
                                            hostmsg_s16_intf_type_t if_type,
                                            UINT32 lane_id);
PUBLIC PMC_ERROR adpt_cvt_serdes_cfg_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_serdes_cfg_file_save(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_enet_int_en_set(UINT32 dev_id, UINT32 chnl_id, digi_port_dir_t dir);
PUBLIC PMC_ERROR adpt_cvt_otn_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_otn_seg_t seg_id,
                                     digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_cvt_mld_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     UINT32 lane_id);
PUBLIC PMC_ERROR adpt_cvt_gfp_int_en_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_sdh_int_en_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     digi_port_dir_t dir);
PUBLIC PMC_ERROR adpt_cvt_fc1200_int_en_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_fc800_int_en_set(UINT32 dev_id, 
                                      UINT32 chnl_id,
                                      digi_port_dir_t dir);
PUBLIC PMC_ERROR adpt_cvt_otn_ohp_en_set(UINT32 dev_id, 
                                      odu_ohp_inst_t ohp,
                                      odu_ohp_port_framer_t port_a_framer,
                                      UINT32 port_a_src_addr,
                                      UINT32 port_a_dest_addr,
                                      odu_ohp_port_framer_t port_b_framer,
                                      UINT32 port_b_src_addr,
                                      UINT32 port_b_dest_addr,
                                      BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_di_rcp_en_set(UINT32 dev_id, BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_otn_ri_rcp_extract_cfg(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           digi_otn_seg_t seg_id,
                                           UINT8 dst_digi,
                                           UINT8 dst_frm,
                                           UINT8 dst_channel,
                                           BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_ptp_cfg(UINT32 dev_id, BOOL enet_line);
PUBLIC PMC_ERROR adpt_cvt_ptp_ipv6_para_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_port_asymmetry_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_default_pro_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_time_pro_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_acceptable_master_table_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_start_initialize(UINT32 dev_id, 
                                       UINT16 out_vlan_tag,
                                       UINT16 in_vlan_tag,
                                       UINT16 ptpChnlBitmask);
PUBLIC PMC_ERROR adpt_cvt_ptp_parent_data_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_port_data_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_port_state_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_unicast_master_para_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_unicast_request_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_pass_thru_general_param_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_pass_thru_port_param_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_tc_param_set(UINT32 dev_id);
PUBLIC PMC_ERROR adpt_cvt_ptp_tc_port_set(UINT32 dev_id, UINT8 req_interval);
PUBLIC PMC_ERROR adpt_cvt_synce_ssm_cfg(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_synce_tx_ssm_set(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_ri_rcp_set(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 digi_otn_seg_t seg_id, 
                                 UINT32 eth_mac_oui,
                                 UINT8  eth_mac_id,
                                 UINT32 eth_etype,
                                 BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_di_rcp_set(UINT32 dev_id, 
                                 UINT32 src_mac, 
                                 UINT32 dest_mac,
                                 UINT32 eth_etype,
                                 UINT32 pdu_mask,
                                 BOOL enable);
PUBLIC BOOL adpt_cvt_otn_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_otn_seg_t seg_id,
                                digi_otn_loc_t location,
                                digi_otn_defect_t defect_id);
PUBLIC BOOL adpt_cvt_otn_mld_get(UINT32 dev_id,
                              UINT32 chnl_id,
                              UINT32 lane_id,
                              digi_otn_defect_t defect_id);
PUBLIC BOOL adpt_cvt_enet_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_enet_defect_t defect_id);
PUBLIC BOOL adpt_cvt_sdh_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_port_dir_t dir,
                                digi_sdh_defect_t defect_id);
PUBLIC BOOL adpt_cvt_gfp_defect_get(UINT32 dev_id,
                                UINT32 chnl_id,
                                digi_gfp_defect_t defect_id);
PUBLIC BOOL adpt_cvt_fc1200_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_fc1200_defect_t defect_id);
PUBLIC BOOL adpt_cvt_fc800_defect_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_port_dir_t dir,
                                  digi_fc800_defect_t defect_id);
PUBLIC PMC_ERROR adpt_cvt_otn_cm_status_get(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_ghao_en_set(UINT32 dev_id, 
                                   UINT32 chnl_id,
                                   UINT32 new_rx_ts_mask0,
                                   UINT32 new_rx_ts_mask1,
                                   UINT32 new_rx_ts_mask2,
                                   UINT32 new_tx_ts_mask0,
                                   UINT32 new_tx_ts_mask1,
                                   UINT32 new_tx_ts_mask2,
                                   BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_enet_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            digi_port_dir_t dir,
                                            util_global_pkt_type_t pkt_type,
                                            BOOL is_vlan_tagged,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            UINT16 vlan_id_repl,
                                            BOOL vlan_id_repl_en,
                                            BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_mgmt_enet_pkt_fwd_cfg(UINT32 dev_id,
                                            UINT32 chnl_id,
                                            digi_port_dir_t dir,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            BOOL vlan_pri_ow,
                                            BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_cpu_mgmt_pkt_fwd_cfg(UINT32 dev_id,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            UINT16 vlan_id_repl,
                                            BOOL vlan_id_repl_en,
                                            BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_mgmt_cpu_pkt_fwd_cfg(UINT32 dev_id,
                                            util_global_vlan_type_t vlan_type,
                                            UINT8 vlan_pcp,
                                            UINT8 vlan_cfi, 
                                            UINT16 vlan_id,
                                            BOOL enable);
PUBLIC PMC_ERROR adpt_cvt_oduksw_latency_get(UINT32 dev_id, UINT32 chnl_id);
PUBLIC PMC_ERROR adpt_cvt_enet_eclass_filter_set(UINT32 dev_id,
                                          UINT32 chnl_id,
                                          digi_enet_eclass_mode_t mode,
                                          digi_enet_eclass_target_t target,
                                          digi_enet_eclass_encap_t encap,
                                          digi_enet_eclass_dir_t dir);
#ifdef PMC_COMEXPRESS
PUBLIC PMC_ERROR adpt_cvt_thread_create(UINT32 dev_id, 
                                        UINT32 chnl_id, 
                                        UINT64 func_p,
                                        UINT32 usleep_t);
#endif
PUBLIC PMC_ERROR adpt_cvt_serdes_port_recfg(UINT32 dev_id,
                                        UINT32 serdes_id,
                                        digi_serdes_intf_t serdes_type,
                                        UINT8 los_pin,
                                        digi_op_mode_t op_mode);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_PV_CVT_H */

/* 
** end of file 
*/

