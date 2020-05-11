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
*     This head file contains the internal coversion API for DIGI Adapter usage.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_API_H
#define _DIGI_ADPT_API_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_mapping_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for client mapping type.  The 
*   element terms have a syntax:
*      DIGI_MAP_<dir>
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_MAP_AMP_BMP_GMP = 0x0,
    DIGI_MAP_GFP         = 0x1,
    DIGI_MAP_7_3_GFP     = 0x2,

    LAST_DIGI_MAPPING_TYPE,       /* this is for out of range values error handling */
} digi_mapping_type_t;

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_handle_create(UINT32 dev_id,
                                 BOOL warm_init,
                                 char *file_name,
                                 digi_cfg_container_t **adpt_handle);
PUBLIC PMC_ERROR adpt_handle_get(UINT32 dev_id, 
                               digi_cfg_container_t **adpt_handle);
PUBLIC PMC_ERROR adpt_handle_init(digi_cfg_container_t *adpt_handle);
PUBLIC PMC_ERROR adpt_handle_query(digi_handle_t *digi_handle, 
                                 digi_cfg_container_t **adpt_handle);
PUBLIC PMC_ERROR adpt_handle_destroy(UINT32 dev_id, 
                                  digi_cfg_container_t **adpt_handle);
PUBLIC PMC_ERROR adpt_ctxt_handle_update(digi_cfg_container_t *adpt_handle,
                                      digi_dev_info_t *dev_info,
                                      digi_handle_t *digi_handle);
PUBLIC PMC_ERROR adpt_chnl_index_create(digi_cfg_container_t *adpt_handle,
                                    UINT32 chnl_id,
                                    UINT32 *index_ptr);
PUBLIC PMC_ERROR adpt_chnl_index_get(digi_cfg_container_t *adpt_handle,
                                  UINT32 chnl_id,
                                  UINT32 *index_ptr);
PUBLIC PMC_ERROR adpt_chnl_index_destroy(digi_cfg_container_t *adpt_handle,
                                     UINT32 chnl_id,
                                     UINT32 index);
PUBLIC PMC_ERROR adpt_rst_chnl_index_create(digi_cfg_container_t *adpt_handle,
                                       UINT32 chnl_id,
                                       UINT32 *index_ptr);
PUBLIC PMC_ERROR adpt_rst_chnl_index_get(digi_cfg_container_t *adpt_handle,
                                     UINT32 chnl_id,
                                     UINT32 *index_ptr);
PUBLIC PMC_ERROR adpt_rst_chnl_index_destroy(digi_cfg_container_t *adpt_handle,
                                        UINT32 chnl_id,
                                        UINT32 index);
PUBLIC const char* adpt_device_name_get(UINT32 digi_dev);
PUBLIC UINT32 adpt_oduflex_ts_get(digi_oduk_t oduk_type, 
                              UINT32 ts_mask[3]);
PUBLIC PMC_ERROR adpt_mapper_para_get(digi_cfg_container_t *adpt_handle,
                                    digi_traffic_mode_t traffic_mode,
                                    digi_mapotn_mode_t mapping_mode,
                                    BOOL *enet_pmon_ptr,
                                    digi_oduk_t *odu_type_ptr,
                                    UINT8 *num_lanes_ptr);
PUBLIC PMC_ERROR adpt_mapper_lane_create(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_mapotn_mode_t mapping_mode,
                                      UINT32 ts_number,
                                      digi_port_usage_grp_struct_t *port_grp_usage,
                                      UINT8 *primary_grp,
                                      UINT8 *secondary_grp);
PUBLIC PMC_ERROR adpt_mapper_lane_destroy(UINT32 dev_id, UINT32 chnl_id);
PUBLIC digi_mapping_mode_t adpt_mapping_mode_cvt(digi_mapotn_mode_t mapping_mode);
PUBLIC digi_mapping_type_t adpt_gfp_mapping_get(digi_mapotn_mode_t mapping_mode);
PUBLIC PMC_ERROR adpt_cpb_map_para_get(digi_mapotn_mode_t mapping_mode,
                                     digi_cpb_stream_t *cpb_stream_ptr,
                                     UINT32 *halt_buffer_ptr);
PUBLIC UINT32 adpt_mapper_ts_get(digi_mapotn_mode_t mapping_mode);
PUBLIC PMC_ERROR adpt_cpb_eth_para_get(digi_traffic_mode_t traffic_mode,
                                    digi_cpb_stream_t *cpb_stream_ptr,
                                    UINT32 *halt_buffer_ptr);
PUBLIC PMC_ERROR adpt_cpb_cbr_para_get(digi_traffic_mode_t traffic_mode,
                                    digi_cpb_stream_t *cpb_stream_ptr,
                                    UINT32 *halt_buffer_ptr);
PUBLIC PMC_ERROR adpt_cpb_ilkn_para_get(UINT16 ilkn_info,
                                    digi_cpb_stream_t *cpb_stream_ptr,
                                    UINT32 *halt_buffer_ptr,
                                    UINT32 *ts_number);
PUBLIC PMC_ERROR adpt_oduksw_odu_para_get(digi_oduk_t oduk_type,
                                        DOUBLE oduflex_rate,
                                        UINT32 ts_mask[3],
                                        digi_oduksw_stream_t *oduksw_stream_ptr,
                                        UINT32 *ts_number_ptr);
PUBLIC PMC_ERROR adpt_oduksw_map_para_get(digi_mapotn_mode_t mapping_mode,
                                        digi_oduksw_stream_t *oduksw_stream_ptr,
                                        DOUBLE *oduflex_rate);
PUBLIC PMC_ERROR adpt_oduksw_ilkn_para_get(UINT16 ilkn_info,
                                        digi_oduksw_stream_t *oduksw_stream_ptr,
                                        DOUBLE *oduflex_rate,
                                        UINT32 *ts_number);
PUBLIC PMC_ERROR adpt_oduksw_para_get(UINT32 dev_id,
                                    UINT32 src_chnl_id,
                                    UINT32 dest_chnl_id,
                                    digi_oduksw_stream_t *oduksw_stream,
                                    DOUBLE *oduflex_rate,
                                    UINT32 *ts_number);
PUBLIC PMC_ERROR adpt_gfp_chnl_get(UINT32 dev_id,
                                UINT32 chnl_id, 
                                UINT32 *gfp_chnl,
                                UINT32 *odu_chnl);
PUBLIC PMC_ERROR adpt_otu_chnl_get(UINT32 dev_id, 
                                UINT32 chnl_id, 
                                UINT32 *otu_chnl);
PUBLIC PMC_ERROR adpt_odu_chnl_get(UINT32 dev_id,
                                 UINT32 chnl_id, 
                                 UINT32 *odu_chnl,
                                 digi_odu_struct_odu_level_t *odu_stage);
PUBLIC PMC_ERROR adpt_enet_chnl_get(UINT32 dev_id, 
                                 UINT32 chnl_id, 
                                 UINT32 *enet_chnl);
PUBLIC PMC_ERROR adpt_cbr_chnl_get(UINT32 dev_id, 
                                UINT32 chnl_id, 
                                UINT32 *cbr_chnl);
PUBLIC PMC_ERROR adpt_etrans_chnl_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *enet_chnl);
PUBLIC PMC_ERROR adpt_lo_odu_status_get(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    BOOL *lo_odu);
PUBLIC PMC_ERROR adpt_msi_len_get(UINT32 dev_id, UINT32 chnl_id, UINT32 *msi_len);
PUBLIC const char* adpt_odu_type_get(digi_oduk_t oduk_type);
PUBLIC digi_oduk_t adpt_otu_odu_cvt(digi_otn_server_otuk_t otuk_type);
PUBLIC PMC_ERROR adpt_otu_deskew_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_traffic_mode_t traffic_mode,
                                   digi_sfis_deskew_type_t *deskew_type);
PUBLIC const char* adpt_prbs_status_str(util_patt_status_t status);
PUBLIC const char* adpt_ge_serdes_type_str(digi_ge_serdes_id_t ge_serdes);
PUBLIC const char* adpt_mt_type_str(digi_mt_type_t mt_type);
PUBLIC const char* adpt_tcm_mode_str(digi_tcm_mode_t tcm_mode);
PUBLIC const char* adpt_tcm_pri_str(digi_tcm_pri_t tcm_pri);
PUBLIC const char* adpt_tim_mode_str(digi_otn_tim_mode_t tim_mode);
PUBLIC const char* adpt_fec_type_str(digi_otn_server_fec_t fec_type);
PUBLIC const char* adpt_seg_str(digi_otn_seg_t seg_id);
PUBLIC const char* adpt_intf_type_str(hostmsg_adapt_intf_type_t intf_type);
PUBLIC const char* adpt_deg_mode_str(digi_otn_ddeg_mode_t deg_mode);
PUBLIC const char* adpt_deg_mon_seg_str(digi_otn_ddeg_mon_seg_t deg_seg);
PUBLIC const char* adpt_deg_interval_str(digi_deg_interval_t interval);
PUBLIC const char* adpt_fcs_mode_str(digi_gfp_fcs_mode_t fcs_mode);
PUBLIC const char* adpt_rx_fcs_mode_str(digi_gfp_rx_fcs_mode_t fcs_mode);
PUBLIC const char* adpt_timing_mode_str(digi_enet_tx_timing_mode_t timing_mode);
PUBLIC const char* adpt_tti_len_str(digi_sdh_tti_len_t tti_len);
PUBLIC const char* adpt_bn_type_str(digi_sdh_pm_t bn_type);
PUBLIC const char* adpt_bn_thr_str(digi_sdh_thr_t bn_thr);
PUBLIC const char* adpt_bip_mode_str(digi_bip_mode_t bip_mode);
PUBLIC const char* adpt_dev_version_str(UINT8 dev_version);
PUBLIC const char* adpt_otn_dir_str(digi_otn_dir_t otn_dir);
PUBLIC const char* adpt_sgmii_port_str(digi_sgmii_port_type_t port_type);
PUBLIC const char* adpt_enet_fault_str(digi_enet_fault_t fault_type);
PUBLIC const char* adpt_dcsu_str(digi_dcsu_inst_t dcsu_type);
PUBLIC const char* adpt_odu_level_str(odu_struct_odu_level_t odu_level);
PUBLIC const char* adpt_dm_mode_str(digi_dm_mode_t dm_mode);
PUBLIC const char* adpt_ts_type_str(digi_odu_line_payload_t ts_type);
PUBLIC digi_otn_asym_deprov_t adpt_otn_dir_cvt(digi_otn_dir_t otn_dir);
PUBLIC PMC_ERROR adpt_parent_segid_get(UINT32 dev_id, 
                                   UINT32 chnl_id, 
                                   digi_otn_seg_t seg_id,
                                   digi_otn_loc_t location,
                                   digi_otn_pos_t position,
                                   digi_otn_seg_t *p_seg_id);
PUBLIC PMC_ERROR adpt_otu_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   digi_otn_server_chnl_t **chnl_handle);
PUBLIC PMC_ERROR adpt_odu_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_otn_loc_t location,
                                   digi_handle_t **digi_handle,
                                   digi_otn_odu_chnl_t **odu_handle);
PUBLIC PMC_ERROR adpt_enet_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   util_global_switch_data_t **chnl_handle);
PUBLIC PMC_ERROR adpt_cbr_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   digi_cbr_client_chnl_t **chnl_handle);
PUBLIC PMC_ERROR adpt_map_handle_get(UINT32 dev_id,
                                   UINT32 chnl_id,
                                   digi_handle_t **digi_handle,
                                   digi_mapper_chnl_t **chnl_handle);
PUBLIC PMC_ERROR adpt_ilkn_handle_get(UINT32 dev_id,
                                  UINT32 chnl_id,
                                  digi_handle_t **digi_handle,
                                  digi_ilkn_chnl_t **chnl_handle);
PUBLIC PMC_ERROR adpt_enet_pmon_merge(digi_enet_pmon_t *src_pmon,
                                    digi_enet_pmon_t *dest_pmon);
PUBLIC PMC_ERROR adpt_odu_pmon_merge(digi_otn_odu_pmon_chnl_data_t *src_pmon,
                                    digi_otn_odu_pmon_chnl_data_t *dest_pmon);
PUBLIC PMC_ERROR adpt_otn_pmon_merge(digi_otn_pmon_t *src_pmon,
                                    digi_otn_pmon_t *dest_pmon);
PUBLIC PMC_ERROR adpt_mapper_pmon_merge(digi_mapper_pmon_t *src_pmon,
                                       digi_mapper_pmon_t *dest_pmon);
PUBLIC PMC_ERROR adpt_mapper_otn_pmon_merge(digi_mapper_pmon_fo2_t *src_pmon,
                                           digi_mapper_pmon_fo2_t *dest_pmon);
PUBLIC PMC_ERROR adpt_sifd_pmon_merge(digi_sifd_pmon_t *src_pmon,
                                    digi_sifd_pmon_t *dest_pmon);
PUBLIC PMC_ERROR adpt_cbr_pmon_merge(digi_cbr_pmon_t *src_pmon,
                                    digi_cbr_pmon_t *dest_pmon);
PUBLIC PMC_ERROR adpt_enet_chnl_int_merge(enet_int_chnl_t *src_int,
                                      enet_int_chnl_t *dest_int);
PUBLIC PMC_ERROR adpt_enet_int_merge(digi_enet_int_t *src_int,
                                  digi_enet_int_t *dest_int);
PUBLIC PMC_ERROR adpt_fege_int_merge(enet_fege_int_t *src_int, 
                                  enet_fege_int_t *dest_int);
PUBLIC PMC_ERROR adpt_ohp_int_merge(odu_ohp_int_t *src_int, 
                                  odu_ohp_int_t *dest_int);
PUBLIC PMC_ERROR adpt_rcp_int_merge(odu_rcp_int_t *src_int, 
                                 odu_rcp_int_t *dest_int);
PUBLIC PMC_ERROR adpt_otu_int_merge(lineotn_int_chnl_t *src_int,
                                 lineotn_int_chnl_t *dest_int);
PUBLIC PMC_ERROR adpt_odu_int_merge(coreotn_int_chnl_t *src_int,
                                  coreotn_int_chnl_t *dest_int);
PUBLIC PMC_ERROR adpt_otn_int_merge(digi_otn_int_t *src_int,
                                 digi_otn_int_t *dest_int);
PUBLIC PMC_ERROR adpt_mapper_int_merge(digi_mapper_int_t *src_int,
                                     digi_mapper_int_t *dest_int);
PUBLIC PMC_ERROR adpt_sifd_int_merge(digi_sifd_int_t *src_int,
                                  digi_sifd_int_t *dest_int);
PUBLIC PMC_ERROR adpt_cbr_int_merge(digi_cbr_int_t *src_int,
                                 digi_cbr_int_t *dest_int);
PUBLIC PMC_ERROR adpt_otu_status_print(lineotn_int_chnl_t *otu_chnl);
PUBLIC PMC_ERROR adpt_odu_status_print(coreotn_int_chnl_t *odu_chnl);
PUBLIC PMC_ERROR adpt_t8_vert_hist(UINT32 dev_id,
                               UINT32 slice, 
                               UINT32 subch, 
                               UINT64 *buffer);
#ifdef PMC_COMEXPRESS
PUBLIC UINT32 adpt_time_get();
#endif
PUBLIC PMC_ERROR adpt_oduksw_latency_get(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      UINT32 *debug_latency,
                                      UINT32 *min_latency,
                                      UINT32 *max_latency);
PUBLIC PMC_ERROR adpt_tcm_pool_reset(UINT32 dev_id, UINT32 chnl_id);
PUBLIC odu_struct_odu_level_t adpt_otn_odu_level_get(UINT32 dev_id, 
                                                  UINT32 chnl_id,
                                                  digi_otn_loc_t location);
PUBLIC PMC_ERROR adpt_tx_conseq_act_reset(UINT32 dev_id,
                                      UINT32 chnl_id,
                                      digi_client2trunk_conseq_action_t *conseq_act);
PUBLIC PMC_ERROR adpt_rx_conseq_act_reset(UINT32 dev_id,
                                      UINT32 chnl_id, 
                                      cmf_trunk2client_conseqAction_t *conseq_act);
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_ex_set(UINT32 dev_id,
                                           UINT32 chnl_id,
                                           UINT32 alm_msk,
                                           digi_gfp_rx_act_t alm_act1,
                                           digi_gfp_rx_act_t alm_act2,
                                           UINT16 holdoff_time,
                                           BOOL enable);
PUBLIC PMC_ERROR adpt_gfp_rx_conseq_act_ex_get(UINT32 dev_id,
                                           UINT32 chnl_id,
                                           digi_rx_alm_act_mask_t alm_item,
                                           digi_gfp_rx_act_t *alm_act1,
                                           digi_gfp_rx_act_t *alm_act2,
                                           UINT16 *holdoff_time,
                                           BOOL *enable);
PUBLIC PMC_ERROR adpt_serdes_bp_lane_prbs_set(UINT32 dev_id,
                                          digi_serdes_dir_t dir,
                                          digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg,
                                          digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg,
                                          util_serdes_patt_cfg_t *patt_cfg);
PUBLIC PMC_ERROR adpt_serdes_bp_lane_tx_err_inst_set(UINT32 dev_id, 
                                               digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg,
                                               digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg);
PUBLIC PMC_ERROR adpt_serdes_bp_lane_rx_state_get(UINT32 dev_id,
                                             digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg,
                                             digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg,
                                             util_patt_status_t *status,
                                             UINT32 *err_cnt);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_CUSTOMER_H */

/* 
** end of file 
*/

