#include "DanOS.h"
#include "Digi.h"
#include "DigiApi.h"
#include<sys/time.h>

typedef int (*testFunc)(unsigned int id,...);
typedef int (*testFunc2)();

typedef struct{
    const char* str;
    testFunc ptr;  
} funStru;

funStru funArray[] =
{
#ifdef __TARGET__
        {"adpt_reg_read", (testFunc)adpt_reg_read},
        {"adpt_reg_write", (testFunc)adpt_reg_write},
        {"digi_otn_tcm_pid_valid_set", (testFunc)digi_otn_tcm_pid_valid_set},
        {"digi_otn_odu_rx_pm_tcm_get", (testFunc)digi_otn_odu_rx_pm_tcm_get},
        {"digi_mapper_channel_num_get", (testFunc)digi_mapper_channel_num_get},
        {"adpt_traffic_mode_get", (testFunc)adpt_traffic_mode_get},
        {"adpt_traffic_mode_cfg", (testFunc)adpt_traffic_mode_cfg},
        {"adpt_t8_vert_hist", (testFunc)adpt_t8_vert_hist},
        {"adpt_synce_tx_ssm_set", (testFunc)adpt_synce_tx_ssm_set},
        {"adpt_synce_ssm_cfg", (testFunc)adpt_synce_ssm_cfg},
        {"adpt_synce_req_ssm_get", (testFunc)adpt_synce_req_ssm_get},
        {"adpt_synce_ch_en_set", (testFunc)adpt_synce_ch_en_set},
        {"adpt_sifd_pmon_merge", (testFunc)adpt_sifd_pmon_merge},
        {"adpt_sifd_pm_get", (testFunc)adpt_sifd_pm_get},
        {"adpt_sifd_int_merge", (testFunc)adpt_sifd_int_merge},
        {"adpt_serdes_tx_refclk_div_cfg", (testFunc)adpt_serdes_tx_refclk_div_cfg},
        {"adpt_serdes_tx_err_inst_set", (testFunc)adpt_serdes_tx_err_inst_set},
        {"adpt_serdes_t8_values_set", (testFunc)adpt_serdes_t8_values_set},
        {"adpt_serdes_t8_values_get", (testFunc)adpt_serdes_t8_values_get},
        {"adpt_serdes_t8_histogram", (testFunc)adpt_serdes_t8_histogram},
        {"adpt_serdes_start_adaptation", (testFunc)adpt_serdes_start_adaptation},
        {"adpt_serdes_sfi51_tx_swing_set", (testFunc)adpt_serdes_sfi51_tx_swing_set},
        {"adpt_serdes_sfi51_tx_dclk_en", (testFunc)adpt_serdes_sfi51_tx_dclk_en},
        {"adpt_serdes_s16_values_set", (testFunc)adpt_serdes_s16_values_set},
        {"adpt_serdes_s16_values_get", (testFunc)adpt_serdes_s16_values_get},
        {"adpt_serdes_s16_tx_swing_set", (testFunc)adpt_serdes_s16_tx_swing_set}, 
        {"adpt_serdes_rx_state_get", (testFunc)adpt_serdes_rx_state_get},
        {"adpt_serdes_prbs_set", (testFunc)adpt_serdes_prbs_set},
        {"adpt_serdes_ge_tx_swing_set", (testFunc)adpt_serdes_ge_tx_swing_set},
        {"adpt_serdes_ge_tx_err_inst_set", (testFunc)adpt_serdes_ge_tx_err_inst_set},
        {"adpt_serdes_ge_rx_state_get", (testFunc)adpt_serdes_ge_rx_state_get},
        {"adpt_serdes_ge_prbs_set", (testFunc)adpt_serdes_ge_prbs_set},
        {"adpt_serdes_debug_status_get", (testFunc)adpt_serdes_debug_status_get},
        {"adpt_serdes_debug_cfg", (testFunc)adpt_serdes_debug_cfg},
        {"adpt_serdes_cfg_set", (testFunc)adpt_serdes_cfg_set},
        {"adpt_serdes_cfg_get", (testFunc)adpt_serdes_cfg_get},
        {"adpt_serdes_cfg_file_save", (testFunc)adpt_serdes_cfg_file_save},
        {"adpt_serdes_cfg_file_parse", (testFunc)adpt_serdes_cfg_file_parse},
        {"adpt_serdes_c8_values_set", (testFunc)adpt_serdes_c8_values_set},
        {"adpt_serdes_c8_values_get", (testFunc)adpt_serdes_c8_values_get},
        {"adpt_serdes_bp_tx_err_inst_set", (testFunc)adpt_serdes_bp_tx_err_inst_set},
        {"adpt_serdes_bp_rx_state_get", (testFunc)adpt_serdes_bp_rx_state_get},
        {"adpt_serdes_bp_prbs_set", (testFunc)adpt_serdes_bp_prbs_set},
        {"adpt_serdes_adapt_status_get", (testFunc)adpt_serdes_adapt_status_get},
        {"adpt_sdh_tti_mode_set", (testFunc)adpt_sdh_tti_mode_set},
        {"adpt_sdh_tti_mode_get", (testFunc)adpt_sdh_tti_mode_get},
        {"adpt_sdh_rx_tti_get", (testFunc)adpt_sdh_rx_tti_get},
        {"adpt_sdh_prbs_status_get", (testFunc)adpt_sdh_prbs_status_get},
        {"adpt_sdh_prbs_pm_get", (testFunc)adpt_sdh_prbs_pm_get},
        {"adpt_sdh_prbs_en_set", (testFunc)adpt_sdh_prbs_en_set},
        {"adpt_sdh_prbs_en_get", (testFunc)adpt_sdh_prbs_en_get},
        {"adpt_sdh_pm_get", (testFunc)adpt_sdh_pm_get},
        {"adpt_sdh_oof_force", (testFunc)adpt_sdh_oof_force},
        {"adpt_sdh_gais_inst_set", (testFunc)adpt_sdh_gais_inst_set},
        {"adpt_sdh_gais_inst_get", (testFunc)adpt_sdh_gais_inst_get},
        {"adpt_sdh_exp_tti_set", (testFunc)adpt_sdh_exp_tti_set},
        {"adpt_sdh_exp_tti_get", (testFunc)adpt_sdh_exp_tti_get},
        {"adpt_sdh_defect_i_get", (testFunc)adpt_sdh_defect_i_get},
        {"adpt_sdh_defect_get", (testFunc)adpt_sdh_defect_get},
        {"adpt_sdh_bn_type_set", (testFunc)adpt_sdh_bn_type_set},
        {"adpt_sdh_bn_type_get", (testFunc)adpt_sdh_bn_type_get},
        {"adpt_sdh_bn_thr_set", (testFunc)adpt_sdh_bn_thr_set},
        {"adpt_sdh_bn_thr_get", (testFunc)adpt_sdh_bn_thr_get},
        {"adpt_ri_rcp_mon_level_cfg", (testFunc)adpt_ri_rcp_mon_level_cfg},
        {"adpt_ri_rcp_extract_cfg", (testFunc)adpt_ri_rcp_extract_cfg},
        {"adpt_ri_rcp_en_set", (testFunc)adpt_ri_rcp_en_set},
        {"adpt_ri_rcp_dest_get", (testFunc)adpt_ri_rcp_dest_get},
        {"adpt_ri_rcp_aps_inst_cfg", (testFunc)adpt_ri_rcp_aps_inst_cfg},
        {"adpt_ptp_unicast_slave_state_get", (testFunc)adpt_ptp_unicast_slave_state_get},
        {"adpt_ptp_unicast_request_set", (testFunc)adpt_ptp_unicast_request_set},
        {"adpt_ptp_unicast_master_para_set", (testFunc)adpt_ptp_unicast_master_para_set},
        {"adpt_ptp_unicast_cancel_set", (testFunc)adpt_ptp_unicast_cancel_set},
        {"adpt_ptp_time_pro_set", (testFunc)adpt_ptp_time_pro_set},
        {"adpt_ptp_tc_param_set", (testFunc)adpt_ptp_tc_param_set},
        {"adpt_ptp_start_initialize", (testFunc)adpt_ptp_start_initialize},
        {"adpt_ptp_stability_status_get", (testFunc)adpt_ptp_stability_status_get},
        {"adpt_ptp_port_state_set", (testFunc)adpt_ptp_port_state_set},
        {"adpt_ptp_port_state_get", (testFunc)adpt_ptp_port_state_get},
        {"adpt_ptp_port_rx_msg_cnts_get", (testFunc)adpt_ptp_port_rx_msg_cnts_get},
        {"adpt_ptp_port_event_set", (testFunc)adpt_ptp_port_event_set},
        {"adpt_ptp_port_data_set", (testFunc)adpt_ptp_port_data_set},
        {"adpt_ptp_port_asymmetry_set", (testFunc)adpt_ptp_port_asymmetry_set},
        {"adpt_ptp_path_delay_get", (testFunc)adpt_ptp_path_delay_get},
        {"adpt_ptp_passive_port_parent_set", (testFunc)adpt_ptp_passive_port_parent_set},
        {"adpt_ptp_pass_thru_port_param_set", (testFunc)adpt_ptp_pass_thru_port_param_set},
        {"adpt_ptp_pass_thru_general_param_set", (testFunc)adpt_ptp_pass_thru_general_param_set},
        {"adpt_ptp_parent_data_set", (testFunc)adpt_ptp_parent_data_set},
        {"adpt_ptp_master_clock_db_status_get", (testFunc)adpt_ptp_master_clock_db_status_get},
        {"adpt_ptp_master_clock_db_get", (testFunc)adpt_ptp_master_clock_db_get},
        {"adpt_ptp_irig_cfg", (testFunc)adpt_ptp_irig_cfg},
        {"adpt_ptp_ipv6_para_set", (testFunc)adpt_ptp_ipv6_para_set},
        {"adpt_ptp_ipv4_para_set", (testFunc)adpt_ptp_ipv4_para_set},
        {"adpt_ptp_default_pro_set", (testFunc)adpt_ptp_default_pro_set},
        {"adpt_ptp_cfg", (testFunc)adpt_ptp_cfg},
        {"adpt_ptp_acceptable_master_table_set", (testFunc)adpt_ptp_acceptable_master_table_set},
        {"adpt_pmon_trigger", (testFunc)adpt_pmon_trigger},
        {"adpt_pmon_int_set", (testFunc)adpt_pmon_int_set},
        {"adpt_pgmrclk_div_cfg", (testFunc)adpt_pgmrclk_div_cfg},
        {"adpt_parent_segid_get", (testFunc)adpt_parent_segid_get},
        {"adpt_packet_switch_get", (testFunc)adpt_packet_switch_get},
        {"adpt_packet_switch", (testFunc)adpt_packet_switch},
        {"adpt_otu_status_print", (testFunc)adpt_otu_status_print},
        {"adpt_otu_int_merge", (testFunc)adpt_otu_int_merge},
        {"adpt_otu_deskew_get", (testFunc)adpt_otu_deskew_get},
        {"adpt_otu_chnl_get", (testFunc)adpt_otu_chnl_get},
        {"adpt_otn_tx_tti_set", (testFunc)adpt_otn_tx_tti_set},
        {"adpt_otn_tx_tti_get", (testFunc)adpt_otn_tx_tti_get},
        {"adpt_otn_tx_tcm_act_src_set", (testFunc)adpt_otn_tx_tcm_act_src_set},
        {"adpt_otn_tx_tcm_act_src_get", (testFunc)adpt_otn_tx_tcm_act_src_get},
        {"adpt_otn_tx_tcm_act_set", (testFunc)adpt_otn_tx_tcm_act_set},
        {"adpt_otn_tx_tcm_act_get", (testFunc)adpt_otn_tx_tcm_act_get},
        {"adpt_otn_tx_stat_set", (testFunc)adpt_otn_tx_stat_set},
        {"adpt_otn_tx_stat_get", (testFunc)adpt_otn_tx_stat_get},
        {"adpt_otn_tx_res_src_set", (testFunc)adpt_otn_tx_res_src_set},
        {"adpt_otn_tx_res_src_get", (testFunc)adpt_otn_tx_res_src_get},
        {"adpt_otn_tx_res_set", (testFunc)adpt_otn_tx_res_set},
        {"adpt_otn_tx_res_get", (testFunc)adpt_otn_tx_res_get},
        {"adpt_otn_tx_pt_set", (testFunc)adpt_otn_tx_pt_set},
        {"adpt_otn_tx_pt_get", (testFunc)adpt_otn_tx_pt_get},
        {"adpt_otn_tx_otu_oh_set", (testFunc)adpt_otn_tx_otu_oh_set},
        {"adpt_otn_tx_otu_oh_get", (testFunc)adpt_otn_tx_otu_oh_get},
        {"adpt_otn_tx_msi_set", (testFunc)adpt_otn_tx_msi_set},
        {"adpt_otn_tx_msi_get", (testFunc)adpt_otn_tx_msi_get},
        {"adpt_otn_tx_gcc_src_set", (testFunc)adpt_otn_tx_gcc_src_set},
        {"adpt_otn_tx_gcc_src_get", (testFunc)adpt_otn_tx_gcc_src_get},
        {"adpt_otn_tx_gcc_set", (testFunc)adpt_otn_tx_gcc_set},
        {"adpt_otn_tx_gcc_get", (testFunc)adpt_otn_tx_gcc_get},
        {"adpt_otn_tx_ftfl_src_set", (testFunc)adpt_otn_tx_ftfl_src_set},
        {"adpt_otn_tx_ftfl_src_get", (testFunc)adpt_otn_tx_ftfl_src_get},
        {"adpt_otn_tx_ftfl_set", (testFunc)adpt_otn_tx_ftfl_set},
        {"adpt_otn_tx_ftfl_get", (testFunc)adpt_otn_tx_ftfl_get},
        {"adpt_otn_tx_exp_src_set", (testFunc)adpt_otn_tx_exp_src_set},
        {"adpt_otn_tx_exp_src_get", (testFunc)adpt_otn_tx_exp_src_get},
        {"adpt_otn_tx_exp_set", (testFunc)adpt_otn_tx_exp_set},
        {"adpt_otn_tx_exp_get", (testFunc)adpt_otn_tx_exp_get},
        {"adpt_otn_tx_bei_biae_set", (testFunc)adpt_otn_tx_bei_biae_set},
        {"adpt_otn_tx_bei_biae_get", (testFunc)adpt_otn_tx_bei_biae_get},
        {"adpt_otn_tx_bdi_set", (testFunc)adpt_otn_tx_bdi_set},
        {"adpt_otn_tx_bdi_get", (testFunc)adpt_otn_tx_bdi_get},
        {"adpt_otn_tx_aps_src_set", (testFunc)adpt_otn_tx_aps_src_set},
        {"adpt_otn_tx_aps_src_get", (testFunc)adpt_otn_tx_aps_src_get},
        {"adpt_otn_tx_aps_set", (testFunc)adpt_otn_tx_aps_set},
        {"adpt_otn_tx_aps_get", (testFunc)adpt_otn_tx_aps_get},
        {"adpt_otn_tim_det_mode_set", (testFunc)adpt_otn_tim_det_mode_set},
        {"adpt_otn_tim_det_mode_get", (testFunc)adpt_otn_tim_det_mode_get},
        {"adpt_otn_tim_act_dis_set", (testFunc)adpt_otn_tim_act_dis_set},
        {"adpt_otn_tim_act_dis_get", (testFunc)adpt_otn_tim_act_dis_get},
        {"adpt_otn_tcm_pri_set", (testFunc)adpt_otn_tcm_pri_set},
        {"adpt_otn_tcm_pri_get", (testFunc)adpt_otn_tcm_pri_get},
        {"adpt_otn_tcm_mode_set", (testFunc)adpt_otn_tcm_mode_set},
        {"adpt_otn_tcm_mode_get", (testFunc)adpt_otn_tcm_mode_get},
        {"adpt_otn_tcm_bw_act_set", (testFunc)adpt_otn_tcm_bw_act_set},
        {"adpt_otn_tcm_bw_act_get", (testFunc)adpt_otn_tcm_bw_act_get},
        {"adpt_otn_rx_tti_get", (testFunc)adpt_otn_rx_tti_get},
        {"adpt_otn_rx_tcm_act_get", (testFunc)adpt_otn_rx_tcm_act_get},
        {"adpt_otn_rx_stat_get", (testFunc)adpt_otn_rx_stat_get},
        {"adpt_otn_rx_res_get", (testFunc)adpt_otn_rx_res_get},
        {"adpt_otn_rx_pt_get", (testFunc)adpt_otn_rx_pt_get},
        {"adpt_otn_rx_msi_get", (testFunc)adpt_otn_rx_msi_get},
        {"adpt_otn_rx_gcc_get", (testFunc)adpt_otn_rx_gcc_get},
        {"adpt_otn_rx_ftfl_get", (testFunc)adpt_otn_rx_ftfl_get},
        {"adpt_otn_rx_exp_get", (testFunc)adpt_otn_rx_exp_get},
        {"adpt_otn_rx_aps_get", (testFunc)adpt_otn_rx_aps_get},
        {"adpt_otn_prbs_thresh_set", (testFunc)adpt_otn_prbs_thresh_set},
        {"adpt_otn_prbs_thresh_get", (testFunc)adpt_otn_prbs_thresh_get},
        {"adpt_otn_prbs_pm_get", (testFunc)adpt_otn_prbs_pm_get},
        {"adpt_otn_prbs_err_inst_set", (testFunc)adpt_otn_prbs_err_inst_set},
        {"adpt_otn_prbs_err_inst_get", (testFunc)adpt_otn_prbs_err_inst_get},
        {"adpt_otn_prbs_en_set", (testFunc)adpt_otn_prbs_en_set},
        {"adpt_otn_prbs_en_get", (testFunc)adpt_otn_prbs_en_get},
        {"adpt_otn_prbs_dlss_get", (testFunc)adpt_otn_prbs_dlss_get},
        {"adpt_otn_pmon_merge", (testFunc)adpt_otn_pmon_merge},
        {"adpt_otn_pm_get", (testFunc)adpt_otn_pm_get},
        {"adpt_otn_ohp_en_set", (testFunc)adpt_otn_ohp_en_set},
        {"adpt_otn_ohp_cfg", (testFunc)adpt_otn_ohp_cfg},
        {"adpt_otn_null_sig_set", (testFunc)adpt_otn_null_sig_set},
        {"adpt_otn_null_sig_get", (testFunc)adpt_otn_null_sig_get},
        {"adpt_otn_mt_sig_set", (testFunc)adpt_otn_mt_sig_set},
        {"adpt_otn_mt_sig_get", (testFunc)adpt_otn_mt_sig_get},
        {"adpt_otn_mld_i_get", (testFunc)adpt_otn_mld_i_get},
        {"adpt_otn_mld_get", (testFunc)adpt_otn_mld_get},
        {"adpt_otn_ltc_act_set", (testFunc)adpt_otn_ltc_act_set},
        {"adpt_otn_ltc_act_get", (testFunc)adpt_otn_ltc_act_get},
        {"adpt_otn_int_merge", (testFunc)adpt_otn_int_merge},
        {"adpt_otn_int_en_set", (testFunc)adpt_otn_int_en_set},
        {"adpt_otn_int_en_get", (testFunc)adpt_otn_int_en_get},
        {"adpt_otn_fec_type_set", (testFunc)adpt_otn_fec_type_set},
        {"adpt_otn_fec_type_get", (testFunc)adpt_otn_fec_type_get},
        {"adpt_otn_fec_pm_get", (testFunc)adpt_otn_fec_pm_get},
        {"adpt_otn_fec_err_inst_set", (testFunc)adpt_otn_fec_err_inst_set},
        {"adpt_otn_fec_err_inst_get", (testFunc)adpt_otn_fec_err_inst_get},
        {"adpt_otn_fec_en_set", (testFunc)adpt_otn_fec_en_set},
        {"adpt_otn_fec_en_get", (testFunc)adpt_otn_fec_en_get},
        {"adpt_otn_exp_tti_set", (testFunc)adpt_otn_exp_tti_set},
        {"adpt_otn_exp_tti_get", (testFunc)adpt_otn_exp_tti_get},
        {"adpt_otn_exp_pt_set", (testFunc)adpt_otn_exp_pt_set},
        {"adpt_otn_exp_pt_get", (testFunc)adpt_otn_exp_pt_get},
        {"adpt_otn_exp_msi_set", (testFunc)adpt_otn_exp_msi_set},
        {"adpt_otn_exp_msi_get", (testFunc)adpt_otn_exp_msi_get},
        {"adpt_otn_enhanced_deg_set", (testFunc)adpt_otn_enhanced_deg_set},
        {"adpt_otn_enhanced_deg_get", (testFunc)adpt_otn_enhanced_deg_get},
        {"adpt_otn_dm_trigger", (testFunc)adpt_otn_dm_trigger},
        {"adpt_otn_dm_pm_get", (testFunc)adpt_otn_dm_pm_get},
        {"adpt_otn_dm_mode_set", (testFunc)adpt_otn_dm_mode_set},
        {"adpt_otn_dm_mode_get", (testFunc)adpt_otn_dm_mode_get},
        {"adpt_otn_deg_para_set", (testFunc)adpt_otn_deg_para_set},
        {"adpt_otn_deg_para_get", (testFunc)adpt_otn_deg_para_get},
        {"adpt_otn_deg_mon_set", (testFunc)adpt_otn_deg_mon_set},
        {"adpt_otn_deg_mode_set", (testFunc)adpt_otn_deg_mode_set},
        {"adpt_otn_deg_mode_get", (testFunc)adpt_otn_deg_mode_get},
        {"adpt_otn_defect_i_get", (testFunc)adpt_otn_defect_i_get},
        {"adpt_otn_defect_get", (testFunc)adpt_otn_defect_get},
        {"adpt_otn_conseq_act_set", (testFunc)adpt_otn_conseq_act_set},
        {"adpt_otn_conseq_act_get", (testFunc)adpt_otn_conseq_act_get},
        {"adpt_otn_bw_inst_set", (testFunc)adpt_otn_bw_inst_set},
        {"adpt_otn_ais_act_set", (testFunc)adpt_otn_ais_act_set},
        {"adpt_otn_ais_act_get", (testFunc)adpt_otn_ais_act_get},
        {"adpt_opsa_pkt_size_get", (testFunc)adpt_opsa_pkt_size_get},
        {"adpt_opsa_oduksw_fifo_get", (testFunc)adpt_opsa_oduksw_fifo_get},
        {"adpt_oduksw_odu_para_get", (testFunc)adpt_oduksw_odu_para_get},
        {"adpt_oduksw_map_para_get", (testFunc)adpt_oduksw_map_para_get},
        {"adpt_oduksw_ilkn_para_get", (testFunc)adpt_oduksw_ilkn_para_get},
        {"adpt_odu_status_print", (testFunc)adpt_odu_status_print},
        {"adpt_odu_pmon_merge", (testFunc)adpt_odu_pmon_merge},
        {"adpt_odu_mux_get", (testFunc)adpt_odu_mux_get},
        {"adpt_odu_mux_del", (testFunc)adpt_odu_mux_del},
        {"adpt_odu_mux_cfg", (testFunc)adpt_odu_mux_cfg},
        {"adpt_odu_int_merge", (testFunc)adpt_odu_int_merge},
        {"adpt_odu_handle_get", (testFunc)adpt_odu_handle_get},
        {"adpt_odu_chnl_get", (testFunc)adpt_odu_chnl_get},
        {"adpt_msi_len_get", (testFunc)adpt_msi_len_get},
        {"adpt_mld_int_en_set", (testFunc)adpt_mld_int_en_set},
        {"adpt_mld_int_en_get", (testFunc)adpt_mld_int_en_get},
        {"adpt_mapper_pmon_merge", (testFunc)adpt_mapper_pmon_merge},
        {"adpt_mapper_para_get", (testFunc)adpt_mapper_para_get},
        {"adpt_mapper_lane_destroy", (testFunc)adpt_mapper_lane_destroy},
        {"adpt_mapper_lane_create", (testFunc)adpt_mapper_lane_create},
        {"adpt_mapper_int_merge", (testFunc)adpt_mapper_int_merge},
        {"adpt_loopback_set", (testFunc)adpt_loopback_set},
        {"adpt_loopback_get", (testFunc)adpt_loopback_get},
        {"adpt_lo_odu_status_get", (testFunc)adpt_lo_odu_status_get},
        {"adpt_internal_loopback_set", (testFunc)adpt_internal_loopback_set},
        {"adpt_internal_loopback_get", (testFunc)adpt_internal_loopback_get},
        {"adpt_int_check", (testFunc)adpt_int_check},
        {"adpt_int_cb_handle", (testFunc)adpt_int_cb_handle},
#if 0
        {"adpt_ilkn_loopback_set", (testFunc)adpt_ilkn_loopback_set},
        {"adpt_ilkn_loopback_get", (testFunc)adpt_ilkn_loopback_get},
        {"adpt_ilkn_chnl_get", (testFunc)adpt_ilkn_chnl_get},
        {"adpt_ilkn_chnl_cfg", (testFunc)adpt_ilkn_chnl_cfg},
        {"adpt_ilkn_cfg_rx_reset", (testFunc)adpt_ilkn_cfg_rx_reset},
#endif
        {"adpt_handle_query", (testFunc)adpt_handle_query},
        {"adpt_handle_init", (testFunc)adpt_handle_init},
        {"adpt_handle_get", (testFunc)adpt_handle_get},
        {"adpt_handle_free", (testFunc)adpt_handle_free},
        {"adpt_handle_destroy", (testFunc)adpt_handle_destroy},
        {"adpt_handle_create", (testFunc)adpt_handle_create},
        {"adpt_handle_allocate", (testFunc)adpt_handle_allocate},
        {"adpt_gfp_tx_upi_set", (testFunc)adpt_gfp_tx_upi_set},
        {"adpt_gfp_tx_upi_get", (testFunc)adpt_gfp_tx_upi_get},
        {"adpt_gfp_tx_pti_set", (testFunc)adpt_gfp_tx_pti_set},
        {"adpt_gfp_tx_pti_get", (testFunc)adpt_gfp_tx_pti_get},
        {"adpt_gfp_tx_cmf_set", (testFunc)adpt_gfp_tx_cmf_set},
        {"adpt_gfp_rx_cmf_get", (testFunc)adpt_gfp_rx_cmf_get},
        {"adpt_gfp_tx_fcs_mode_set", (testFunc)adpt_gfp_tx_fcs_mode_set},
        {"adpt_gfp_tx_fcs_mode_get", (testFunc)adpt_gfp_tx_fcs_mode_get},
        {"adpt_gfp_tx_exi_set", (testFunc)adpt_gfp_tx_exi_set},
        {"adpt_gfp_tx_exi_get", (testFunc)adpt_gfp_tx_exi_get},
        {"adpt_gfp_tx_ehec_en_set", (testFunc)adpt_gfp_tx_ehec_en_set},
        {"adpt_gfp_tx_ehec_en_get", (testFunc)adpt_gfp_tx_ehec_en_get},
        {"adpt_gfp_rx_fcs_mode_set", (testFunc)adpt_gfp_rx_fcs_mode_set},
        {"adpt_gfp_rx_fcs_mode_get", (testFunc)adpt_gfp_rx_fcs_mode_get},
        {"adpt_gfp_rx_ehec_en_set", (testFunc)adpt_gfp_rx_ehec_en_set},
        {"adpt_gfp_rx_ehec_en_get", (testFunc)adpt_gfp_rx_ehec_en_get},
        {"adpt_gfp_pm_get", (testFunc)adpt_gfp_pm_get},
        {"adpt_gfp_int_en_set", (testFunc)adpt_gfp_int_en_set},
        {"adpt_gfp_int_en_get", (testFunc)adpt_gfp_int_en_get},
        {"adpt_gfp_exp_upi_set", (testFunc)adpt_gfp_exp_upi_set},
        {"adpt_gfp_exp_upi_get", (testFunc)adpt_gfp_exp_upi_get},
        {"adpt_gfp_exp_pti_set", (testFunc)adpt_gfp_exp_pti_set},
        {"adpt_gfp_exp_pti_get", (testFunc)adpt_gfp_exp_pti_get},
        {"adpt_gfp_exp_exi_set", (testFunc)adpt_gfp_exp_exi_set},
        {"adpt_gfp_exp_exi_get", (testFunc)adpt_gfp_exp_exi_get},
        {"adpt_gfp_defect_i_get", (testFunc)adpt_gfp_defect_i_get},
        {"adpt_gfp_defect_get", (testFunc)adpt_gfp_defect_get},
        {"adpt_gfp_chnl_get", (testFunc)adpt_gfp_chnl_get},
        {"adpt_ge_loopback_set", (testFunc)adpt_ge_loopback_set},
        {"adpt_ge_loopback_get", (testFunc)adpt_ge_loopback_get},
#if 0
        {"adpt_fege_rx_fc_set", (testFunc)adpt_fege_rx_fc_set},
        {"adpt_fege_rx_fc_get", (testFunc)adpt_fege_rx_fc_get},
        {"adpt_fege_pkt_fwd_set", (testFunc)adpt_fege_pkt_fwd_set},
        {"adpt_fege_frm_size_set", (testFunc)adpt_fege_frm_size_set},
        {"adpt_fege_frm_size_get", (testFunc)adpt_fege_frm_size_get},
        {"adpt_fege_en_set", (testFunc)adpt_fege_en_set},
        {"adpt_fc800_pm_get", (testFunc)adpt_fc800_pm_get},
        {"adpt_fc1200_pm_get", (testFunc)adpt_fc1200_pm_get},
        {"adpt_fc_tx_lfos_set", (testFunc)adpt_fc_tx_lfos_set},
        {"adpt_fc_tx_lfos_get", (testFunc)adpt_fc_tx_lfos_get},
        {"adpt_fc_tx_corrupt_blk_set", (testFunc)adpt_fc_tx_corrupt_blk_set},
        {"adpt_fc_tx_corrupt_blk_get", (testFunc)adpt_fc_tx_corrupt_blk_get},
        {"adpt_fc_rx_lfos_set", (testFunc)adpt_fc_rx_lfos_set},
        {"adpt_fc_rx_lfos_get", (testFunc)adpt_fc_rx_lfos_get},
        {"adpt_fc_rx_invalid_idle_set", (testFunc)adpt_fc_rx_invalid_idle_set},
        {"adpt_fc_rx_invalid_idle_get", (testFunc)adpt_fc_rx_invalid_idle_get},
        {"adpt_fc_rx_ecb_set", (testFunc)adpt_fc_rx_ecb_set},
        {"adpt_fc_rx_ecb_get", (testFunc)adpt_fc_rx_ecb_get},
        {"adpt_fc_rx_descramble_set", (testFunc)adpt_fc_rx_descramble_set},
        {"adpt_fc_rx_descramble_get", (testFunc)adpt_fc_rx_descramble_get},
        {"adpt_fc_prbs_pm_get", (testFunc)adpt_fc_prbs_pm_get},
        {"adpt_fc_prbs_en_set", (testFunc)adpt_fc_prbs_en_set},
        {"adpt_fc_prbs_en_get", (testFunc)adpt_fc_prbs_en_get},
        {"adpt_fc_lfos_inst_set", (testFunc)adpt_fc_lfos_inst_set},
        {"adpt_fc_lfos_inst_get", (testFunc)adpt_fc_lfos_inst_get},
        {"adpt_fc_invalid_blk_mode_set", (testFunc)adpt_fc_invalid_blk_mode_set},
        {"adpt_fc_invalid_blk_mode_set", (testFunc)adpt_fc_invalid_blk_mode_set},
        {"adpt_fc_dlolb_set", (testFunc)adpt_fc_dlolb_set},
        {"adpt_fc_dlolb_set", (testFunc)adpt_fc_dlolb_set},
        {"adpt_fc_dlolb_inst_set", (testFunc)adpt_fc_dlolb_inst_set},
        {"adpt_fc_dlolb_inst_get", (testFunc)adpt_fc_dlolb_inst_get},
#endif
        {"adpt_enet_tx_timing_mode_set", (testFunc)adpt_enet_tx_timing_mode_set},
        {"adpt_enet_tx_timing_mode_get", (testFunc)adpt_enet_tx_timing_mode_get},
        {"adpt_enet_tx_fc_set", (testFunc)adpt_enet_tx_fc_set},
        {"adpt_enet_tx_fc_get", (testFunc)adpt_enet_tx_fc_get},
        {"adpt_enet_test_pkt_set", (testFunc)adpt_enet_test_pkt_set},
        {"adpt_enet_test_pkt_get", (testFunc)adpt_enet_test_pkt_get},
        {"adpt_enet_rx_fc_set", (testFunc)adpt_enet_rx_fc_set},
        {"adpt_enet_rx_fc_get", (testFunc)adpt_enet_rx_fc_get},
        {"adpt_enet_prbs_set", (testFunc)adpt_enet_prbs_set},
        {"adpt_enet_prbs_pm_get", (testFunc)adpt_enet_prbs_pm_get},
        {"adpt_enet_prbs_get", (testFunc)adpt_enet_prbs_get},
        {"adpt_enet_pmon_merge", (testFunc)adpt_enet_pmon_merge},
        {"adpt_enet_pm_get", (testFunc)adpt_enet_pm_get},
        {"adpt_enet_phy_en_set", (testFunc)adpt_enet_phy_en_set},
        {"adpt_enet_phy_en_get", (testFunc)adpt_enet_phy_en_get},
        {"adpt_enet_pcs_i_get", (testFunc)adpt_enet_pcs_i_get},
        {"adpt_enet_pcs_get", (testFunc)adpt_enet_pcs_get},
        {"adpt_enet_mru_set", (testFunc)adpt_enet_mru_set},
        {"adpt_enet_mru_get", (testFunc)adpt_enet_mru_get},
        {"adpt_enet_irig_cfg_set", (testFunc)adpt_enet_irig_cfg_set},
        {"adpt_enet_ipg_set", (testFunc)adpt_enet_ipg_set},
        {"adpt_enet_ipg_get", (testFunc)adpt_enet_ipg_get},
        {"adpt_enet_int_merge", (testFunc)adpt_enet_int_merge},
        {"adpt_enet_int_en_set", (testFunc)adpt_enet_int_en_set},
        {"adpt_enet_int_en_get", (testFunc)adpt_enet_int_en_get},
        {"adpt_otn_seg_int_en_set", (testFunc)adpt_otn_seg_int_en_set},        
        {"adpt_otn_defect_int_en_set", (testFunc)adpt_otn_defect_int_en_set},
        {"adpt_enet_fc_status_get", (testFunc)adpt_enet_fc_status_get},
        {"adpt_enet_fault_inst_set", (testFunc)adpt_enet_fault_inst_set},
        {"adpt_enet_errors_get", (testFunc)adpt_enet_errors_get},
        {"adpt_enet_defect_i_get", (testFunc)adpt_enet_defect_i_get},
        {"adpt_enet_defect_get", (testFunc)adpt_enet_defect_get},
        {"adpt_enet_crc_inst_set", (testFunc)adpt_enet_crc_inst_set},
        {"adpt_enet_crc_inst_get", (testFunc)adpt_enet_crc_inst_get},
        {"adpt_enet_crc_fwd_set", (testFunc)adpt_enet_crc_fwd_set},
        {"adpt_enet_crc_fwd_get", (testFunc)adpt_enet_crc_fwd_get},
        {"adpt_enet_cpu_pkt_inst_set", (testFunc)adpt_enet_cpu_pkt_inst_set},
        {"adpt_enet_cpu_pkt_ext_set", (testFunc)adpt_enet_cpu_pkt_ext_set},
        //{"adpt_enet_classification_set", (testFunc)adpt_enet_classification_set},
        {"adpt_enet_chnl_get", (testFunc)adpt_enet_chnl_get},
        //{"adpt_enet_auto_fault_inst_set", (testFunc)adpt_enet_auto_fault_inst_set},
        //{"adpt_enet_auto_fault_inst_get", (testFunc)adpt_enet_auto_fault_inst_get},
        {"adpt_enet_cmf_rf_cfg", (testFunc)adpt_enet_cmf_rf_cfg},
        {"adpt_enet_cmf_lf_cfg", (testFunc)adpt_enet_cmf_lf_cfg},
        {"adpt_dump_sifd_pmon", (testFunc)adpt_dump_sifd_pmon},
        {"adpt_dump_sifd_alm_i", (testFunc)adpt_dump_sifd_alm_i},
        {"adpt_dump_sifd_alm", (testFunc)adpt_dump_sifd_alm},
        {"adpt_dump_serdes_st", (testFunc)adpt_dump_serdes_st},
        {"adpt_dump_serdes", (testFunc)adpt_dump_serdes},
        {"adpt_dump_sdk", (testFunc)adpt_dump_sdk},
        {"adpt_dump_sdh_pmon", (testFunc)adpt_dump_sdh_pmon},
        {"adpt_dump_sdh_cfg", (testFunc)adpt_dump_sdh_cfg},
        {"adpt_dump_sdh_alm_i", (testFunc)adpt_dump_sdh_alm_i},
        {"adpt_dump_sdh_alm", (testFunc)adpt_dump_sdh_alm},
        {"adpt_dump_ri_rcp_pmon", (testFunc)adpt_dump_ri_rcp_pmon},
        {"adpt_dump_reg", (testFunc)adpt_dump_reg},
        {"adpt_dump_ptp", (testFunc)adpt_dump_ptp},
        {"adpt_dump_pmon_data", (testFunc)adpt_dump_pmon_data},
        {"adpt_dump_pmon_cfg", (testFunc)adpt_dump_pmon_cfg},
        {"adpt_dump_otn_tti", (testFunc)adpt_dump_otn_tti},
        {"adpt_dump_otn_tcm", (testFunc)adpt_dump_otn_tcm},
        {"adpt_dump_otn_pmon", (testFunc)adpt_dump_otn_pmon},
        {"adpt_dump_otn_int", (testFunc)adpt_dump_otn_int},
        {"adpt_dump_otn_cfg", (testFunc)adpt_dump_otn_cfg},
        {"adpt_dump_otn_alm_i", (testFunc)adpt_dump_otn_alm_i},
        {"adpt_dump_otn_alm", (testFunc)adpt_dump_otn_alm},
        {"adpt_dump_odu_mux", (testFunc)adpt_dump_odu_mux},
        {"adpt_dump_loopback", (testFunc)adpt_dump_loopback},
        {"adpt_dump_int_tree", (testFunc)adpt_dump_int_tree},
        {"adpt_dump_gfp_pmon", (testFunc)adpt_dump_gfp_pmon},
        {"adpt_dump_gfp_int", (testFunc)adpt_dump_gfp_int},
        {"adpt_dump_gfp_cfg", (testFunc)adpt_dump_gfp_cfg},
        {"adpt_dump_gfp_alm_i", (testFunc)adpt_dump_gfp_alm_i},
        {"adpt_dump_gfp_alm", (testFunc)adpt_dump_gfp_alm},
        {"adpt_dump_fw", (testFunc)adpt_dump_fw},
        {"adpt_dump_fw_log", (testFunc)adpt_dump_fw_log},
#if 0
        {"adpt_dump_fw_enable_log", (testFunc)adpt_dump_fw_enable_log},
        {"adpt_dump_fc_pmon", (testFunc)adpt_dump_fc_pmon},
#endif
        {"adpt_dump_port_pmon", (testFunc)adpt_dump_port_pmon},
        {"adpt_dump_enet_pmon", (testFunc)adpt_dump_enet_pmon},
        {"adpt_dump_enet_int", (testFunc)adpt_dump_enet_int},
        {"adpt_dump_enet_cfg", (testFunc)adpt_dump_enet_cfg},
        {"adpt_dump_enet_alm_i", (testFunc)adpt_dump_enet_alm_i},
        {"adpt_dump_enet_alm", (testFunc)adpt_dump_enet_alm},
        {"adpt_dump_di_rcp_pmon", (testFunc)adpt_dump_di_rcp_pmon},
        {"adpt_dump_data", (testFunc)adpt_dump_data},
        {"adpt_dump_basic", (testFunc)adpt_dump_basic},
        {"adpt_dump_all", (testFunc)adpt_dump_all},
#if 0
        {"adpt_pmon_prbs_collection_mode_cfg", (testFunc)adpt_pmon_prbs_collection_mode_cfg},
#endif
        {"adpt_di_rcp_en_set", (testFunc)adpt_di_rcp_en_set},
        {"adpt_di_rcp_sdh_counts_cfg", (testFunc)adpt_di_rcp_sdh_counts_cfg},
        {"adpt_di_rcp_cfg", (testFunc)adpt_di_rcp_cfg},
        {"adpt_dev_info_get", (testFunc)adpt_dev_info_get},
        {"adpt_defect_trigger", (testFunc)adpt_defect_trigger},
        {"adpt_database_save", (testFunc)adpt_database_save},
        {"adpt_database_restore", (testFunc)adpt_database_restore},
        {"adpt_cpb_map_para_get", (testFunc)adpt_cpb_map_para_get},
        {"adpt_cpb_ilkn_para_get", (testFunc)adpt_cpb_ilkn_para_get},
        {"adpt_cpb_eth_para_get", (testFunc)adpt_cpb_eth_para_get},
        {"adpt_cpb_chnl_enable", (testFunc)adpt_cpb_chnl_enable},
        {"adpt_cpb_cbr_para_get", (testFunc)adpt_cpb_cbr_para_get},
        {"adpt_client_map_get", (testFunc)adpt_client_map_get},
        {"adpt_client_map_cfg", (testFunc)adpt_client_map_cfg},
        {"adpt_chnl_index_get", (testFunc)adpt_chnl_index_get},
        {"adpt_chnl_index_destroy", (testFunc)adpt_chnl_index_destroy},
        {"adpt_chnl_index_create", (testFunc)adpt_chnl_index_create},
        {"adpt_cbr_pmon_merge", (testFunc)adpt_cbr_pmon_merge},
        {"adpt_cbr_int_merge", (testFunc)adpt_cbr_int_merge},
        {"adpt_cbr_pn11_mon_set", (testFunc)adpt_cbr_pn11_mon_set},
        {"adpt_cbr_pn11_mon_get", (testFunc)adpt_cbr_pn11_mon_get},
        {"adpt_cbr_pattern_src_set", (testFunc)adpt_cbr_pattern_src_set},
        {"adpt_cbr_pattern_src_get", (testFunc)adpt_cbr_pattern_src_get},
        {"adpt_cbr_pattern_inst_set", (testFunc)adpt_cbr_pattern_inst_set},
        {"adpt_cbr_pattern_inst_get", (testFunc)adpt_cbr_pattern_inst_get},        
        {"adpt_cbr_chnl_get", (testFunc)adpt_cbr_chnl_get},
        {"adpt_cvt_odu_mux_cfg", (testFunc)adpt_cvt_odu_mux_cfg},
        {"adpt_cvt_odu_mux_asym_cfg", (testFunc)adpt_cvt_odu_mux_asym_cfg},
        {"adpt_cvt_enet_test_pkt_set", (testFunc)adpt_cvt_enet_test_pkt_set},
        {"adpt_buffer_reset", (testFunc)adpt_buffer_reset},
        {"adpt_buffer_set", (testFunc)adpt_buffer_set},
        {"adpt_cvt_gfp_tx_cmf_set", (testFunc)adpt_cvt_gfp_tx_cmf_set},
        {"adpt_cvt_gfp_tx_ehec_en_set", (testFunc)adpt_cvt_gfp_tx_ehec_en_set},
        {"adpt_cvt_otn_tx_tti_set", (testFunc)adpt_cvt_otn_tx_tti_set},
        {"adpt_cvt_otn_exp_tti_set", (testFunc)adpt_cvt_otn_exp_tti_set},
        {"adpt_cvt_otn_tx_ftfl_set", (testFunc)adpt_cvt_otn_tx_ftfl_set},
        {"adpt_cvt_otn_tcm_pri_set", (testFunc)adpt_cvt_otn_tcm_pri_set},
        {"adpt_cvt_sdh_exp_tti_set", (testFunc)adpt_cvt_sdh_exp_tti_set},
        {"adpt_cvt_pmon_int_set", (testFunc)adpt_cvt_pmon_int_set},
        {"adpt_cvt_serdes_c8_values_set", (testFunc)adpt_cvt_serdes_c8_values_set},
        {"adpt_cvt_serdes_t8_values_set", (testFunc)adpt_cvt_serdes_t8_values_set},
        {"adpt_cvt_serdes_s16_values_set", (testFunc)adpt_cvt_serdes_s16_values_set},
        {"adpt_cvt_serdes_cfg_set", (testFunc)adpt_cvt_serdes_cfg_set},
        {"adpt_cvt_serdes_cfg_file_save", (testFunc)adpt_cvt_serdes_cfg_file_save},
        {"adpt_cvt_enet_int_en_set", (testFunc)adpt_cvt_enet_int_en_set},
        {"adpt_cvt_otn_int_en_set", (testFunc)adpt_cvt_otn_int_en_set},
        {"adpt_cvt_mld_int_en_set", (testFunc)adpt_cvt_mld_int_en_set},
        {"adpt_cvt_gfp_int_en_set", (testFunc)adpt_cvt_gfp_int_en_set},
        {"adpt_cvt_otn_ohp_en_set", (testFunc)adpt_cvt_otn_ohp_en_set},
        {"adpt_cvt_di_rcp_en_set", (testFunc)adpt_cvt_di_rcp_en_set},
        {"adpt_cvt_otn_ri_rcp_extract_cfg", (testFunc)adpt_cvt_otn_ri_rcp_extract_cfg},
        {"adpt_cvt_ptp_cfg", (testFunc)adpt_cvt_ptp_cfg},
        {"adpt_cvt_ptp_ipv6_para_set", (testFunc)adpt_cvt_ptp_ipv6_para_set},
        {"adpt_cvt_ptp_port_asymmetry_set", (testFunc)adpt_cvt_ptp_port_asymmetry_set},
        {"adpt_cvt_ptp_default_pro_set", (testFunc)adpt_cvt_ptp_default_pro_set},
        {"adpt_cvt_ptp_time_pro_set", (testFunc)adpt_cvt_ptp_time_pro_set},
        {"adpt_cvt_ptp_acceptable_master_table_set", (testFunc)adpt_cvt_ptp_acceptable_master_table_set},
        {"adpt_cvt_ptp_parent_data_set", (testFunc)adpt_cvt_ptp_parent_data_set},
        {"adpt_cvt_ptp_port_data_set", (testFunc)adpt_cvt_ptp_port_data_set},
        {"adpt_cvt_ptp_port_state_set", (testFunc)adpt_cvt_ptp_port_state_set},
        {"adpt_cvt_ptp_unicast_master_para_set", (testFunc)adpt_cvt_ptp_unicast_master_para_set},
        {"adpt_cvt_ptp_unicast_request_set", (testFunc)adpt_cvt_ptp_unicast_request_set},
        {"adpt_cvt_ptp_pass_thru_general_param_set", (testFunc)adpt_cvt_ptp_pass_thru_general_param_set},
        {"adpt_cvt_ptp_pass_thru_port_param_set", (testFunc)adpt_cvt_ptp_pass_thru_port_param_set},
        {"adpt_cvt_synce_ssm_cfg", (testFunc)adpt_cvt_synce_ssm_cfg},
        {"adpt_cvt_synce_tx_ssm_set", (testFunc)adpt_cvt_synce_tx_ssm_set},
        {"adpt_cvt_ri_rcp_set", (testFunc)adpt_cvt_ri_rcp_set},
        {"adpt_cvt_di_rcp_set", (testFunc)adpt_cvt_di_rcp_set},
        {"adpt_cvt_enet_defect_get", (testFunc)adpt_cvt_enet_defect_get},
        {"adpt_cvt_otn_defect_get", (testFunc)adpt_cvt_otn_defect_get},
        {"adpt_cvt_sdh_defect_get", (testFunc)adpt_cvt_sdh_defect_get},
#if 0
        {"adpt_show_pmm_stat", (testFunc)adpt_show_pmm_stat},
        {"adpt_dev_int_en_set", (testFunc)adpt_dev_int_en_set},
        {"adpt_gsup43_7_3_legacy_mode_cfg", (testFunc)adpt_gsup43_7_3_legacy_mode_cfg},
        {"adpt_gsup43_7_3_increase_orderred_set_rate", (testFunc)adpt_gsup43_7_3_increase_orderred_set_rate},
#endif
        {"adpt_gfp_legacy_mode_set", (testFunc)adpt_gfp_legacy_mode_set},
        {"adpt_gfp_os_rate_inc_set", (testFunc)adpt_gfp_os_rate_inc_set},
        {"adpt_gfp_tx_conseq_act_set", (testFunc)adpt_gfp_tx_conseq_act_set},
        {"adpt_init", (testFunc)adpt_init},
        {"adpt_uninit", (testFunc)adpt_uninit},					
#if 0
        {"adpt_fw_reload", (testFunc)adpt_fw_reload},
#endif
        {"adpt_warm_restart_handle", (testFunc)adpt_warm_restart_handle},
        {"adpt_gfp_legacy_mode_get", (testFunc)adpt_gfp_legacy_mode_get},
        {"adpt_oduksw_latency_get", (testFunc)adpt_oduksw_latency_get},        
        {"adpt_serdes_vert_eye_opening", (testFunc)adpt_serdes_vert_eye_opening},        
#endif
};

int printMatchFun(char* str)
{
    int count = sizeof(funArray)/sizeof(funStru);
    int i;
    int found = 0;
    for(i = 0; i < count; i++)
    {
        const char* tmp = strstr(funArray[i].str, str);
        if(tmp != NULL)
        {
          found++;
          printf("%s \n", funArray[i].str);
        }
    }
    return found;
}

void char2uint(int argc, char *argv[], unsigned int *a)
{
    for (int i = 0; i < argc; i++)
    {
        a[i] = strtoul(argv[i], NULL, 0);
    }
}

int DigiSdkMenu(int argc, char *argv[])
{
    testFunc tmpFun;
    int count = sizeof(funArray) / sizeof(funStru);
    int i;
    unsigned int a[20] = {0};

    for(i = 0; i < count; i++)
    {
        if(strcasecmp(argv[0], funArray[i].str) == 0)
        {
            //printf("found func %s\n",funArray[i].str);
            tmpFun = funArray[i].ptr;

            // double special process
            if (strcasecmp(argv[0], "adpt_cvt_odu_mux_cfg") == 0 && argc == 12)
            {
                char2uint(argc - 2, argv + 1, a);
                double b11 = atof(argv[11]);
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], b11);
            }  
            else if (strcasecmp(argv[0], "adpt_cvt_odu_mux_asym_cfg") == 0 && argc == 15)
            {
                char2uint(argc - 2, argv + 1, a);
                double b14 = atof(argv[14]);
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], b14);
            }  
            else if(strcasecmp(argv[0], "adpt_serdes_vert_eye_opening") == 0)
            {
                unsigned int b1,b2,b4;
                unsigned long long b3,b5;
                
                if(argc == 6)
                { 
                    struct timeval pmonEndTime1 = {0,0};
                    struct timeval pmonEndTime2 = {0,0};
                
                    PMC_ERROR ret = -1;
                    b1 = strtoul(argv[1], NULL, 0);
                    b2 = strtoul(argv[2], NULL, 0);
                    b3 = strtoul(argv[3], NULL, 0);
                    b4 = strtoul(argv[4], NULL, 0);
                    b5 = strtoul(argv[5], NULL, 0);

                    gettimeofday(&pmonEndTime1, NULL);

                    //ret =  adpt_serdes_vert_eye_opening(b1,b2,b3,b4,b5);
                    ret = tmpFun(b1,b2,b3,b4,b5);

                    gettimeofday(&pmonEndTime2, NULL);

                    printf("tv_sec= %lu.%6lu \n",pmonEndTime1.tv_sec,pmonEndTime1.tv_usec);
                    printf("tv_sec= %lu.%6lu \n",pmonEndTime2.tv_sec,pmonEndTime2.tv_usec);
                    
                    return ret;
                }
            }
            else if (strcasecmp(argv[0], "adpt_otn_prbs_pm_get") == 0 && argc == 3)
            {
                unsigned int prbs = 0;
                char2uint(argc - 1, argv + 1, a);
                int ret = tmpFun(a[0], a[1], &prbs);
                printf("Error count = [0x%08x]\n", prbs);
                return ret;
            }
            else if (strcasecmp(argv[0], "adpt_otn_prbs_dlss_get") == 0 && argc == 3)
            {
                int lock = 0;
                char2uint(argc - 1, argv + 1, a);
                int ret = tmpFun(a[0], a[1], &lock);
                printf("PRBS state = [%s]\n", lock==1?"LOCK":"UNLOCK");
                return ret;
            }

            char2uint(argc - 1, argv + 1, a);

            if (argc == 1)
            {
                testFunc2 tmpPtr= (testFunc2)tmpFun;
                return tmpPtr();
            }
            else if(argc == 2)
            {
                return tmpFun(a[0]);
            }
            else if(argc == 3)
            {
                return tmpFun(a[0], a[1]);
            }
            else if(argc == 4)
            {
                return tmpFun(a[0], a[1], a[2]);
            }
            else if(argc == 5)
            {
                return tmpFun(a[0], a[1], a[2], a[3]);
            }
            else if(argc == 6)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4]);
            }
            else if(argc == 7)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5]);
            }
            else if(argc == 8)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
            }
            else if(argc == 9)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
            }
            else if(argc == 10)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8]);
            }
            else if(argc == 11)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
            }
            else if(argc == 12)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10]);
            }
            else if(argc == 13)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11]);
            }
            else if(argc == 14)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12]);
            }
            else if(argc == 15)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13]);
            }
            else if(argc == 16)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14]);
            }
            else if(argc == 17)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]);
            }
            else if(argc == 18)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16]);
            }
            else if(argc == 19)
            {
                return tmpFun(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16], a[17]);
            }
            else
            {
                printf("don't support more than 18 parameter\n");
            }

            return 0;
        }

    }

    if (printMatchFun(argv[0]) == 0)
    {
        printf("can't find the function %s\n", argv[0]);
    }

	return 0;
}

