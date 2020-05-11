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
*     The file describes the PTP Run-Time functionality.
*
*   NOTES:
*     None.
*
*******************************************************************************/
#ifndef _DIGI_ADPT_PTP_H
#define _DIGI_ADPT_PTP_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include <stdlib.h>
#include "digi_api.h"
#include "digi_timesync_api.h"

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Enumerated Types
*/

/*
** Structures and Unions
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR adpt_ptp_cfg(UINT32 dev_id, ptp_config_t *ptp_cfg, BOOL enet_line);
PUBLIC PMC_ERROR adpt_ptp_irig_cfg(UINT32 dev_id, 
                               ptp_irig_port_enum port,
                               ptp_irig_mode_enum irig_mode,
                               ptp_irig_adj_mode_enum adj_mode,
                               UINT8 sync_perd,
                               UINT8 sync_rate,
                               BOOL enable);
PUBLIC PMC_ERROR adpt_ptp_ipv4_para_set(UINT32 dev_id, 
                                    UINT8 type_of_service,
                                    UINT8 time_to_live,
                                    UINT32 src_addr,
                                    UINT32 mst_dest_addr);
PUBLIC PMC_ERROR adpt_ptp_ipv6_para_set(UINT32 dev_id, ptp_ipv6_param_t *ptp_ipv6);
PUBLIC PMC_ERROR adpt_ptp_port_asymmetry_set(UINT32 dev_id, 
                                         UINT32 chnl_id, 
                                         ptp_port_asymmetry_t *ptp_port);
PUBLIC PMC_ERROR adpt_ptp_port_rx_msg_cnts_get(UINT32 dev_id, 
                                           UINT32 chnl_id,
                                           ptp_rx_msg_cnts_t *ptp_rx_msg);
PUBLIC PMC_ERROR adpt_ptp_path_delay_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     ptp_path_delay_t *ptp_path_delay);
PUBLIC PMC_ERROR adpt_ptp_default_pro_set(UINT32 dev_id, ptp_default_properties_t *default_pro);
PUBLIC PMC_ERROR adpt_ptp_time_pro_set(UINT32 dev_id, ptp_set_time_param_t *time_pro);
PUBLIC PMC_ERROR adpt_ptp_acceptable_master_table_set(UINT32 dev_id, 
                                                ptp_acceptable_master_clocks_t *clock_para,
                                                ptp_acceptable_master_clocks_data_t *clock_data);
PUBLIC PMC_ERROR adpt_ptp_start_initialize(UINT32 dev_id, 
                                    UINT32 chnl_id[DIGI_ENET_CHNL_MAX],
                                    ptp_start_init_param_t *start_init);
PUBLIC PMC_ERROR adpt_ptp_master_clock_db_status_get(UINT32 dev_id, ptp_master_clock_db_status_t *status);
PUBLIC PMC_ERROR adpt_ptp_master_clock_db_get(UINT32 dev_id, ptp_master_clock_db_t *clock_db);
PUBLIC PMC_ERROR adpt_ptp_parent_data_set(UINT32 dev_id, ptp_parent_data_t *parent_data);
PUBLIC PMC_ERROR adpt_ptp_port_data_set(UINT32 dev_id, ptp_port_data_t *port_data);
PUBLIC PMC_ERROR adpt_ptp_port_event_set(UINT32 dev_id, UINT32 chnl_id, BOOL enable);
PUBLIC PMC_ERROR adpt_ptp_port_state_set(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     port_state_t *port_state);
PUBLIC PMC_ERROR adpt_ptp_port_state_get(UINT32 dev_id, 
                                     UINT32 chnl_id,
                                     port_state_t *port_state);
PUBLIC PMC_ERROR adpt_ptp_passive_port_parent_set(UINT32 dev_id,
                                             UINT32 chnl_id,
                                             UINT64 clock_id,
                                             UINT16 port_num);
PUBLIC PMC_ERROR adpt_ptp_unicast_master_para_set(UINT32 dev_id, ptp_master_unicast_param_t *unicast_para);
PUBLIC PMC_ERROR adpt_ptp_unicast_request_set(UINT32 dev_id, 
                                         UINT32 chnl_id, 
                                         ptp_unicast_request_t *request_para);
PUBLIC PMC_ERROR adpt_ptp_unicast_cancel_set(UINT32 dev_id, UINT32 chnl_id, UINT8 msg_type);
PUBLIC PMC_ERROR adpt_ptp_unicast_slave_state_get(UINT32 dev_id, 
                                            UINT32 chnl_id,
                                            ptp_slave_unicast_param_t *unicast_para);
PUBLIC PMC_ERROR adpt_ptp_pass_thru_general_param_set(UINT32 dev_id, ptp_pass_thru_general_param_t *ptp_para);
PUBLIC PMC_ERROR adpt_ptp_pass_thru_port_param_set(UINT32 dev_id, 
                                              UINT32 chnl_id, 
                                              ptp_pass_thru_port_param_t *ptp_para);
PUBLIC PMC_ERROR adpt_ptp_tc_param_set(UINT32 dev_id, 
                                    UINT32 chnl_id[DIGI_ENET_CHNL_MAX],
                                    ptp_tc_param_t *ptp_tc_para);
PUBLIC PMC_ERROR adpt_ptp_tc_port_set(UINT32 dev_id, 
                                  UINT32 chnl_id[DIGI_ENET_CHNL_MAX], 
                                  UINT8 req_interval);
PUBLIC PMC_ERROR adpt_ptp_stability_status_get(UINT32 dev_id, 
                                        UINT32 chnl_id,
                                        ptp_stability_status_t *status);
PUBLIC PMC_ERROR adpt_synce_ssm_cfg(UINT32 dev_id, 
                                  UINT32 chnl_id, 
                                  digi_synce_ch_cfg_t *synce_ctrl);
PUBLIC PMC_ERROR adpt_synce_ch_en_set(UINT32 dev_id, UINT32 chnl_id, BOOL enable);
PUBLIC PMC_ERROR adpt_synce_req_ssm_get(UINT32 dev_id, 
                                     UINT32 chnl_id, 
                                     digi_synce_ch_status_t *synce_status);
PUBLIC PMC_ERROR adpt_synce_tx_ssm_set(UINT32 dev_id, 
                                    UINT32 chnl_id, 
                                    digi_synce_ch_cfg_t *synce_ctrl);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ADPT_PTP_H */

/* 
** end of file 
*/

