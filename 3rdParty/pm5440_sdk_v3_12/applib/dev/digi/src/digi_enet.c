/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION : This file contains C functions for the creation and
*   destruction of DIGI devices and data paths.  Functions for usage in
*   higher level systems have prototypes defined in digi_api.h.  Other functions
*   should be considered private and for the internal use of the SW driver.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_loc.h"

#include "digi_enet_loc.h"
#include "digi_enet_api.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define DIGI_ENET_SYS_ZONE_THRESH_DEFAULT_VAL 3

/* relative MCPB port rate connected to MGMT_FEGE */
#define  DIGI_MGMT_FEGE_MCPB_PORT_RATE            100000000
#define  DIGI_MGMT_FEGE_TO_MCPB_MAX_PORTS         64
#define  DIGI_MCPB_TO_MGMT_FEGE_MAX_PORTS         25
/* VLAN OFFSET */
#define DIGI_VLAN_PCP_OFFSET                      13
#define DIGI_VLAN_CFI_OFFSET                      12

#define DIGI_ENET_FLT_HDL_DIS_MSK 0x4
#define DIGI_ENET_FLT_HDL_DIS_OFF 2
#define DIGI_ENET_SEND_RF_MSK 0x2
#define DIGI_ENET_SEND_RF_OFF 1
#define DIGI_ENET_SEND_LF_MSK 0x1
#define DIGI_ENET_SEND_LF_OFF 0

/*
** Local Macro Definitions
*/
#define DIGI_ENET_PMON_TABLE_DEFINE()           \
    DIGI_ENET_PMON(mib);                        \
    DIGI_ENET_PMON(errors);                     \
    DIGI_ENET_PMON(eclass);                     \

#define CONVERT_MIPS_ADDR_TO_HOST_ADDR(x) ((UINT32)x & 0x1FFFFFF)

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
PRIVATE PMC_ERROR digi_enet_int_chnl_filt_retrieve_internal(digi_handle_t             *digi_handle,
                                                           digi_enet_client_chnl_t   *chnl_handle,   
                                                           digi_enet_int_chnl_t      *filt_table_ptr,
                                                           BOOL8                     fw_retrieve_done,
                                                            digi_enet_int_chnl_t      *int_table_ptr);
PRIVATE BOOL8 digi_enet_is_link_transparent(enet_handle_t *enet_handle,
                                            UINT32 link);

PRIVATE BOOL8 digi_enet_is_link_valid(enet_handle_t *enet_handle,
                                      UINT32 link);

PRIVATE PMC_ERROR digi_enet_line_prov_lane_validate(digi_handle_t *digi_handle,
                                                    UINT32 port_uid,
                                                    digi_enet_mode_t mode);

PRIVATE PMC_ERROR digi_enet_sys_prov_lane_validate(digi_handle_t *digi_handle,
                                                   UINT32 port_uid,
                                                   digi_enet_mode_t mode);

PRIVATE BOOL8 digi_enet_line_db_test_update(digi_handle_t *digi_handle,
                                            UINT32 schd_addr,
                                            UINT32 enet_link,
                                            BOOL mirror_mode);

PRIVATE PMC_ERROR digi_enet_line_db_clear(digi_handle_t *digi_handle,
                                          UINT32 enet_link,
                                          BOOL mirror_mode);
                                     

PRIVATE BOOL8 digi_enet_sys_db_test_update(digi_handle_t *digi_handle,
                                           UINT32 schd_addr,
                                           UINT32 enet_link, 
                                           UINT32 enet_sys_dcs_inst); 
  

PRIVATE PMC_ERROR digi_enet_sys_db_clear(digi_handle_t *digi_handle, 
                                         UINT32 enet_link, 
                                         UINT32 enet_sys_dcs_inst);
                                     
PRIVATE BOOL8 digi_enet_mgmt_fege_vlan_id_check(digi_handle_t          *digi_handle,
                                                UINT16                  vlan_id);
PRIVATE PMC_ERROR digi_enet_mcpb_chnl_cfg(digi_handle_t                     *digi_handle,
                                          cpb_mcpb_egr_output_port_type_t    egr_port,
                                          UINT32                             egr_chnl,
                                          cpb_mcpb_ingr_input_port_type_t    ingr_port,
                                          UINT32                             ingr_chnl,
                                          BOOL8                              enable,
                                          digi_cpb_stream_t                  rate_data,
                                          util_global_mcpb_port_t            dst_cpb_port_type);

PRIVATE BOOL8 digi_enet_mgmt_fege_to_enet_10g_fifo_data_get(digi_handle_t *digi_handle, 
                                                           enet_handle_t *enet_handle,
                                                           cpb_mcpb_egr_output_port_type_t mcpb_egr_port, 
                                                           UINT32 *link_ptr);

PRIVATE BOOL8 digi_enet_10g_to_mgmt_fege_fifo_data_get(digi_handle_t *digi_handle,
                                                      enet_handle_t *enet_handle,
                                                      UINT32 push_port_id,
                                                      UINT32 *link_ptr);

PRIVATE PMC_ERROR digi_enet_insert_chnl_cfg (digi_handle_t                    *digi_handle,
                                             UINT32                           chnl,
                                             cpb_mcpb_egr_output_port_type_t  mcpb_egr_port,
                                             cpb_mcpb_ingr_input_port_type_t  mcpb_ing_port,
                                             BOOL8                            enable,
                                             digi_cpb_stream_t                rate_data);
PRIVATE PMC_ERROR digi_enet_extract_chnl_cfg(digi_handle_t                     *digi_handle,
                                             UINT32                             chnl,
                                             cpb_mcpb_egr_output_port_type_t    mcpb_egr_port,
                                             cpb_mcpb_ingr_input_port_type_t    mcpb_ing_port,
                                             BOOL8                              enable,
                                             digi_cpb_stream_t                  rate_data);

PRIVATE PMC_ERROR digi_enet_int_register_cb(digi_handle_t      *digi_handle,
                                            digi_int_type_t     int_type,
                                            void               *token,
                                            UINT32              token_size,
                                            digi_int_cb_fcn     user_cb);
PRIVATE PMC_ERROR digi_enet_int_chnl_enable(digi_handle_t             *digi_handle,
                                            digi_int_type_t            int_type,
                                            digi_enet_client_chnl_t   *chnl_handle,   
                                            enet_int_chnl_t           *int_table_ptr,
                                            BOOL8                       enable);
PRIVATE PMC_ERROR digi_enet_int_clear(digi_handle_t             *digi_handle,
                                      digi_int_type_t            int_type,
                                      digi_enet_int_t           *int_table_ptr);

PRIVATE PMC_ERROR digi_enet_int_chnl_clear(digi_handle_t             *digi_handle,
                                           digi_int_type_t            int_type,
                                           digi_enet_client_chnl_t   *chnl_handle,
                                           digi_enet_int_chnl_t      *int_table_ptr);

PRIVATE PMC_ERROR digi_enet_int_enabled_get(digi_handle_t       *digi_handle,
                                            digi_int_type_t      int_type,
                                            digi_enet_int_t     *int_table_ptr);
PRIVATE PMC_ERROR digi_enet_int_enabled_check(digi_handle_t      *digi_handle,
                                              digi_int_type_t     int_type);

PRIVATE PMC_ERROR digi_enet_int_summary_get_internal(digi_handle_t            *digi_handle,
                                                     digi_int_type_t           int_type,
                                                     UINT32                    unused,
                                                     digi_enet_int_summary_t  *int_summary_ptr);

PRIVATE PMC_ERROR digi_enet_int_filt_retrieve(digi_handle_t         *digi_handle,
                                              digi_int_type_t        int_type,
                                              digi_enet_int_chnl_t  *filt_table_ptr,
                                              digi_enet_int_t       *int_table_ptr);

PRIVATE PMC_ERROR digi_enet_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                   digi_int_type_t           int_type,
                                                   digi_enet_int_summary_t  *int_summary_ptr);

PRIVATE PMC_ERROR digi_enet_pmon_chnl_retrieve_internal(digi_handle_t              *digi_handle,
                                                        util_global_switch_data_t  *chnl_handle,
                                                        BOOL8                      enet_line,   
                                                        digi_enet_pmon_chnl_t      *pmon_table_ptr);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_line_comp_activate(digi_handle_t* digi_handle,
                                                             util_global_switch_data_def_t* in_util_ptr);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_comp_activate(digi_handle_t* digi_handle,
                                                            util_global_switch_data_def_t* in_util_ptr);
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_conditional_deactivate(digi_handle_t* digi_handle,
                                                                 util_global_switch_data_t* input_pc_ptr);
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_fcs_settings_set(digi_handle_t *digi_handle,
                                                          util_global_switch_data_t *input_pc_ptr);
PRIVATE PMC_ERROR digi_lifd_convert_and_enable_clock_select(digi_handle_t* digi_handle,
                                                            UINT32 enet_link,
                                                            enet_datapath_t enet_datapath,
                                                            BOOL8 enet_clk_enable);
PRIVATE PMC_ERROR digi_sifd_convert_and_enable_clock_select(digi_handle_t* digi_handle,
                                                            UINT32 enet_link,
                                                            enet_datapath_t enet_datapath,
                                                            BOOL8 enet_clk_enable);

PRIVATE UINT32 digi_convert_enet_mode_to_lanes(enet_datapath_t enet_mode);

PRIVATE PMC_ERROR digi_enet_verify_enet_pmon_and_demap(digi_handle_t* digi_handle,
                                                       util_global_switch_data_t* input_pc_ptr,
                                                       util_global_switch_data_t* output_pc_ptr);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_or_line_map_helper(digi_handle_t* digi_handle,
                                                                 util_global_switch_data_t* input_pc_ptr, 
                                                                 digi_cpb_stream_t rate_data,
                                                                 UINT32 client_num_odu0,
                                                                 UINT32 halt_buffer,
                                                                 digi_enet_enhanced_pmon_req_t pmon_req_type,
                                                                 digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                                 enet_datapath_t enet_mode, 
                                                                 digi_enet_mode_t digi_enet_mode,
                                                                 digi_enet_client_chnl_def_t** enet_chnl_mate_pptr);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_map_helper(digi_handle_t* digi_handle,
                                                         util_global_switch_data_t* input_pc_ptr, 
                                                         digi_cpb_stream_t rate_data,
                                                         UINT32 client_num_odu0,
                                                         UINT32 halt_buffer,
                                                         digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                         enet_datapath_t enet_mode, 
                                                         digi_enet_mode_t digi_enet_mode,
                                                         digi_enet_client_chnl_def_t** enet_chnl_mate_pptr);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_line_map_helper(digi_handle_t* digi_handle,
                                                          util_global_switch_data_t* input_pc_ptr, 
                                                          digi_cpb_stream_t rate_data,
                                                          UINT32 client_num_odu0,
                                                          UINT32 halt_buffer,
                                                          digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                          enet_datapath_t enet_mode, 
                                                          digi_enet_mode_t digi_enet_mode,
                                                          digi_enet_client_chnl_def_t** enet_chnl_mate_pptr);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_or_line_demap_helper(digi_handle_t* digi_handle,
                                                                   util_global_switch_data_t* input_pc_ptr,
                                                                   UINT32 link,
                                                                   digi_enet_pmon_type_t* enet_pmon);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_demap_helper(digi_handle_t* digi_handle,
                                                           UINT32 link);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_line_demap_helper(digi_handle_t* digi_handle,
                                                            UINT32 link);

PRIVATE PMC_ERROR digi_enet_determine_result_and_resource_availability(digi_handle_t* digi_handle, 
                                                                       digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                                       digi_group_size_t group_size,
                                                                       digi_enet_enhanced_pmon_req_t pmon_req_type);

PRIVATE util_global_switch_data_t* digi_enet_companion_pmon_get(digi_handle_t *digi_handle,
                                                                util_global_switch_data_t *chnl_handle);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_set_helper(digi_handle_t *digi_handle);

PRIVATE PMC_ERROR digi_enet_enhanced_pmon_map_gsup43_7_3_pmon_ilkn_lane_get(UINT32 ilkn_N1_enable_mask,
                                                                            UINT32* serdes_lane_used);


PRIVATE PMC_ERROR digi_enet_sys_serdes_s16_gsup43_7_3_cfg(digi_handle_t *digi_handle,
                                                          UINT32* serdes_lane_used);

PRIVATE PMC_ERROR digi_enet_sys_serdes_s16_gsup43_7_3_uncfg(digi_handle_t *digi_handle,
                                                            UINT32 serdes_lane_used);

PRIVATE PMC_ERROR digi_enet_insert_crc_internal_set(digi_handle_t *digi_handle,
                                                    util_global_switch_data_t *chnl_handle,
                                                    BOOL8 enable);

PRIVATE PMC_ERROR digi_enet_crc_forwarding_internal_set(digi_handle_t *digi_handle,
                                                        util_global_switch_data_t *chnl_handle,
                                                        BOOL8 enable);

PRIVATE BOOL8 digi_enet_is_far_end_mapotn_mode_gfp_enet(digi_handle_t *digi_handle,
                                                        util_global_switch_data_t *enet_chnl_handle);

PRIVATE PMC_ERROR digi_enet_enhanced_mac_settings_copy(digi_handle_t* digi_handle,
                                                       digi_enet_client_chnl_def_t* enet_chnl_src_ptr,
                                                       digi_enet_client_chnl_def_t* enet_chnl_dest_ptr);

PRIVATE PMC_ERROR digi_enet_rx_flow_control_internal_set(digi_handle_t *digi_handle,
                                                         util_global_switch_data_t *chnl_handle,
                                                         BOOL8 enable);

PRIVATE PMC_ERROR digi_enet_rx_pause_frame_fwd_internal_set(digi_handle_t *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            BOOL8 enable);

PRIVATE PMC_ERROR digi_enet_mru_internal_set(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *chnl_handle,
                                             UINT32 length);

PRIVATE PMC_ERROR digi_enet_ipg_internal_set(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *chnl_handle,
                                             UINT32 gap);

PRIVATE PMC_ERROR digi_enet_phy_internal_set(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *chnl_handle,
                                             BOOL8 enable);

PRIVATE PMC_ERROR digi_enet_irig_configuration_internal_set(digi_handle_t *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            BOOL8 ingress_enable,
                                                            digi_enet_irig_mode_t ingress_mode,
                                                            BOOL8 egress_enable,
                                                            digi_enet_irig_mode_t egress_mode);

PRIVATE PMC_ERROR digi_enet_tx_upi_internal_set(digi_handle_t             *digi_handle,
                                                util_global_switch_data_t *chnl_handle,
                                                UINT8                      data_pkt_upi_val,
                                                UINT8                      os_pkt_upi_val,
                                                BOOL8                      enable_drop);

PRIVATE PMC_ERROR digi_enet_tx_upi_internal_set(digi_handle_t             *digi_handle,
                                                util_global_switch_data_t *chnl_handle,
                                                UINT8                      data_pkt_upi_val,
                                                UINT8                      os_upi_val,
                                                BOOL8                      enable_drop);

PRIVATE PMC_ERROR digi_enet_int_fw_retrieve(digi_handle_t         *digi_handle,
                                            digi_int_type_t        int_type,
                                            digi_enet_int_chnl_t  *filt_table_ptr,
                                            digi_enet_int_t       *int_table_ptr);

PRIVATE PMC_ERROR digi_enet_int_fw_chnl_retrieve(digi_handle_t         *digi_handle,
                                                 digi_int_type_t        int_type,
                                                 digi_enet_int_chnl_t  *filt_table_ptr,
                                                 digi_enet_int_chnl_t  *int_table_ptr);

PRIVATE PMC_ERROR digi_enet_mac_data_rate_get(enet_handle_t *enet_handle,
                                              UINT32 link,
                                              emac_data_rate_t *data_rate_ptr);

PRIVATE PMC_ERROR digi_enet_serdes_port_get(digi_handle_t              *digi_handle,
                                            util_global_switch_data_t  *chnl_handle,
                                            digi_serdes_port_t        **port_ctxt_ptr);

PRIVATE PMC_ERROR digi_enet_int_fw_chnl_exco4m_rx_lobl_i_enabled_check(digi_handle_t        *digi_handle,
                                                                       digi_int_type_t       int_type, 
                                                                       BOOL8                 has_epmm_rx_lobl_en,
                                                                       BOOL8                 has_etrans_rx_lobl_en,
                                                                       enet_int_chnl_t      *int_en_table_ptr,
                                                                       BOOL8                *int_found_ptr);

PRIVATE void digi_enet_toggle_rx_lane_inv(digi_handle_t *digi_handle, UINT8 ch_num);
PRIVATE BOOL digi_enet_check_align_status_v_get(digi_handle_t *digi_handle);
PRIVATE void digi_enet_enable_align_status_loc_fault(digi_handle_t *digi_handle);
PRIVATE void digi_enet_disable_align_status_loc_fault(digi_handle_t *digi_handle);
PRIVATE void digi_enet_clear_align_status_loc_fault(digi_handle_t *digi_handle);
/*
** Deprecated Private Functions
*/

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

PRIVATE PMC_ERROR digi_enet_egress_insert_rate_internal_set(digi_handle_t *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            UINT16 insert_quantum, 
                                                            UINT16 system_quantum);

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */


#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Public Functions
*/

/*******************************************************************************
* digi_enet_pcs_block_alignment
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *enet_pmon_table       - pointer to collected enet pmon table
*   pcs_threshold          - Number of pcs errors to trigger alignement procedure
*   fcs_threshold          - Number of fcs errors to trigger alignement procedure
*
* OUTPUTS:
*
* RETURNS:
*   DIGI_ENET_PCS_NO_ALIGN        - No adjustment was required
*   DIGI_ENET_PCS_ALIGN_OCCURED   - PCS alignment procedure was triggered
*   
*
* NOTES:
*   Only valid for 100GE datapaths: 
*   Call after digi_enet_line_pmon_retrieve()
*
*
*******************************************************************************/
PUBLIC digi_enet_pcs_alignment_status_t digi_enet_pcs_block_alignment(digi_handle_t *digi_handle,
                                                                      digi_enet_pmon_t *enet_pmon_table,
                                                                      UINT64 pcs_threshold,
                                                                      UINT64 fcs_threshold)
{
    UINT8 poll_count = 0;
    enum {ENET_CH_NUM = 0, POLL_CNT_THRESH = 2};
    digi_enet_pcs_alignment_status_t rc = DIGI_ENET_PCS_NO_ALIGN;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

#if defined(PMC_BIG_ENDIAN)
    UINT32 pcs_lsb = PMC_LE_TO_CPU_32(enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_errors.block_error_count_40_bit_lsb);
    UINT32 pcs_msb = PMC_LE_TO_CPU_32(enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_errors.block_error_count_40_bit_msb);
    UINT32 fcs_lsb = PMC_LE_TO_CPU_32(enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_mib.a_frame_check_sequence_errors_lsb);
    UINT32 fcs_msb = PMC_LE_TO_CPU_32(enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_mib.a_frame_check_sequence_errors_msb);
#else
    UINT32 pcs_lsb = enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_errors.block_error_count_40_bit_lsb;
    UINT32 pcs_msb = enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_errors.block_error_count_40_bit_msb;
    UINT32 fcs_lsb = enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_mib.a_frame_check_sequence_errors_lsb;
    UINT32 fcs_msb = enet_pmon_table->enet_chnl[ENET_CH_NUM].enet_mib.a_frame_check_sequence_errors_msb;
#endif

    UINT64 pcs_err_cnt = ((UINT64)pcs_msb << 32) | (UINT64)pcs_lsb;
    UINT64 fcs_err_cnt = ((UINT64)fcs_msb << 32) | (UINT64)fcs_lsb;
 
    enet_pcs_poll_count_get(digi_handle->enet_line_handle, &poll_count);

    if (pcs_err_cnt >= pcs_threshold && digi_enet_check_align_status_v_get(digi_handle))
    {
        poll_count++;
    }
    else if (fcs_err_cnt >= fcs_threshold && digi_enet_check_align_status_v_get(digi_handle))
    {
        poll_count++;
    }
    else
    {
        poll_count = 0;
    }

    if (poll_count >= POLL_CNT_THRESH)
    {
        digi_enet_disable_align_status_loc_fault(digi_handle);
        digi_enet_toggle_rx_lane_inv(digi_handle, ENET_CH_NUM);
        digi_enet_clear_align_status_loc_fault(digi_handle);
        digi_enet_enable_align_status_loc_fault(digi_handle);
        poll_count = 0;
        rc = DIGI_ENET_PCS_ALIGN_OCCURED;
    }
    else
    {
        rc = DIGI_ENET_PCS_NO_ALIGN;
    }

    enet_pcs_poll_count_set(digi_handle->enet_line_handle, poll_count);

    PMC_ATOMIC_RETURN(digi_handle, rc);
}

/*******************************************************************************
* digi_enet_mru_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the MRU of the provisioned channel.
*   Only datapaths through the MAC block can support MRU.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper instance
*
* OUTPUTS:
*   *length_ptr            - a pointer to the MRU
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mru_get(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 *length_ptr)
{
    PMC_ERROR rc;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    util_global_switch_data_t *local_chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle,  ((util_global_switch_data_def_t*) chnl_handle));
    }

    if (rc == DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET) { 
        local_chnl_handle = (util_global_switch_data_t *)(((digi_mapper_chnl_def_t *)chnl_handle)->enet_pmon_data); 
        if (NULL == local_chnl_handle) { 
            PMC_ATOMIC_RETURN(digi_handle, rc); 
        } 
        rc = digi_enet_validate_parms(digi_handle, local_chnl_handle, &enet_handle, &link); 
    } 

    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }

    if (PMC_SUCCESS == rc) 
    {
        rc = emac_max_frm_len_get(enet_handle->emac_handle, link, length_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mru_get */

/*******************************************************************************
* digi_enet_ipg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the IPG of the provisioned channel.
*   LAN mode: IPG is the number of octets in steps of 4.
*   Valid values are 8, 12, 16, ... 100. 
*   The default of 12 conforms to IEEE802.3ae. 
*   Only datapaths through the MAC block can support IPG.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *gap_ptr               - a pointer to the gap between packets (in octets)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_ipg_get(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 *gap_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    } 

    if (PMC_SUCCESS == rc)
    {
        rc = emac_tx_ipg_get(enet_handle->emac_handle, link, gap_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_ipg_get */

/*******************************************************************************
* digi_enet_phy_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the PHY status of the provisioned channel.
*   Only datapaths through the MAC block can have this status.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *phy_enabled_ptr       - a pointer to MAC PHY status
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_phy_status_get(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL                     *phy_enabled_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    BOOL8 int_phy_enabled_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

    if (PMC_SUCCESS == rc) {
         if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_tx_enable_get(enet_handle->emac_handle, link, &int_phy_enabled_ptr);
        }
    }

    if (PMC_SUCCESS == rc) 
    {
        *phy_enabled_ptr = UTIL_GLOBAL_BOOL8_TO_BOOL_CONVERT(int_phy_enabled_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_phy_status_get */

/*******************************************************************************
* digi_enet_crc_forwarding_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the CRC forwarding of the provisioned channel.
*   Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - CRC forwarding enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_crc_forwarding_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    BOOL8 is_map_gfp_enet = FALSE;
    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    is_map_gfp_enet = digi_enet_is_far_end_mapotn_mode_gfp_enet(digi_handle, 
                                                                chnl_handle);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

     if (rc == PMC_SUCCESS) {

        /* internal fuction does error checking */
        rc = digi_enet_crc_forwarding_internal_set(digi_handle, chnl_handle, enable);

        if (rc == PMC_SUCCESS) {

            enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

            if (enet_pmon_chnl_handle != NULL) {

                rc = digi_enet_crc_forwarding_internal_set(digi_handle, enet_pmon_chnl_handle, enable);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_crc_forwarding_set */


/*******************************************************************************
* digi_enet_crc_forwarding_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the CRC forwarding of the provisioned channel.
*   Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *enable                - CRC forwarding enable (TRUE) or disable (FALSE)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_ENET_LINK_NOT_MAC   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_crc_forwarding_get(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 is_enable = FALSE;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (PMC_SUCCESS == rc) {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

     if (rc == PMC_SUCCESS) {
        /* internal fuction does error checking */
        rc = digi_enet_crc_forwarding_internal_get(digi_handle, chnl_handle, &is_enable);
        if (rc == PMC_SUCCESS) {
            *enable = is_enable;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_crc_forwarding_get */

/*******************************************************************************
* digi_enet_insert_crc_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the CRC insertion for traffic from the CPB of
*   the provisioned channel.
*   Only datapaths through the MAC block can have CRC insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - CRC insertion enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_insert_crc_set(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    BOOL8 is_map_gfp_enet = FALSE;
    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    is_map_gfp_enet = digi_enet_is_far_end_mapotn_mode_gfp_enet(digi_handle, 
                                                                chnl_handle);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

    if (is_map_gfp_enet == TRUE) {

        /* If we are in a 6_2_GFP mapping mode, we cannot modify this field */
        rc = DIGI_ERR_ENET_CANNOT_MODIFY;

    } else {

        /* internal fuction does error checking */
        rc = digi_enet_insert_crc_internal_set(digi_handle, chnl_handle, enable);

        if (rc == PMC_SUCCESS) {

            enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

            if (enet_pmon_chnl_handle != NULL) {

                rc = digi_enet_insert_crc_internal_set(digi_handle, enet_pmon_chnl_handle, enable);
            }
        }

    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_insert_crc_set */


/*******************************************************************************
* digi_enet_insert_crc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the CRC insertion for traffic from the CPB of
*   the provisioned channel.
*   Only datapaths through the MAC block can have CRC insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *enable                - CRC insertion enable (TRUE) or disable (FALSE)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_insert_crc_get(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 is_enable = FALSE;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (PMC_SUCCESS == rc) {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

    if (PMC_SUCCESS == rc) {
        /* internal fuction does error checking */
        rc = digi_enet_insert_crc_internal_get(digi_handle, chnl_handle, &is_enable);
        if (rc == PMC_SUCCESS) {
            *enable = is_enable;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_insert_crc_get */


/*******************************************************************************
* digi_enet_no_length_check_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the no length check of the provisioned channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - CRC forwarding enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_no_length_check_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

    if (PMC_SUCCESS == rc)
    {
    
        emac_no_length_check_en(enet_handle->emac_handle, link, enable);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_no_length_check_set */




/*******************************************************************************
* digi_enet_tx_timing_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function configures Ethernet Timing mode for a specified channel.
*  Timing modes are only configurable for MAC terminated datapath. The following 
*  timing modes are supported:
*   - DIGI_ENET_TX_NOMINAL_TIMING_MODE  : the default mode
*   - DIGI_ENET_TX_LOOP_TIMING_MODE     : 
*   - DIGI_ENET_TX_LINE_TIMING_MODE     : digi_m1_rate_monitor_cfg_set shall be 
*      used to configure the rate monitor in the DIGI_M1 module 
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_enet_client_chnl  - pointer to ENET channel handle instance
*   timing_mode             - enum, the requested timing mode
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_timing_mode_set(digi_handle_t *digi_handle,
                                              digi_enet_client_chnl_t *digi_enet_client_chnl,
                                              digi_enet_tx_timing_mode_t timing_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_enet_client_chnl_def_t *enet_chnl_def_handle_ptr = NULL;
    util_global_switch_data_t   *input_pc_ptr = NULL;
    util_global_switch_data_t   *output_pc_ptr = NULL;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_enet_client_chnl, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(timing_mode < LAST_DIGI_ENET_TX_TIMING_MODE, DIGI_ERR_NULL_HANDLE, 0, 0);

    rc = digi_enet_chnl_handle_validate(digi_handle, digi_enet_client_chnl);
    
    if (PMC_SUCCESS == rc)
    {
        enet_chnl_def_handle_ptr = (digi_enet_client_chnl_def_t *) digi_enet_client_chnl;

        output_pc_ptr = (util_global_switch_data_t *)&enet_chnl_def_handle_ptr->switch_data;
        PMC_ASSERT(NULL != output_pc_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
        input_pc_ptr = digi_cpb_input_port_get(digi_handle,output_pc_ptr);
        PMC_ASSERT(NULL != input_pc_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ((util_global_switch_data_def_t*) output_pc_ptr));

        /*
           - timing can only be changed on MAC terminated datapath
           - Through timing nmode is not valid for a MAC terminated datapath
           - check that DIGI_M1, PMM_LITE is configured
        */
        if (DIGI_ENET_SERDES_100GE_MAC != enet_chnl_def_handle_ptr->mode &&
            DIGI_ENET_SERDES_10GE_MAC != enet_chnl_def_handle_ptr->mode &&
            DIGI_ENET_SERDES_40GE_MAC != enet_chnl_def_handle_ptr->mode)
        {
            rc = DIGI_ERR_ENET_CANNOT_CHANGE_TIMING_MODE;
        } else if (timing_mode ==  DIGI_ENET_TX_THROUGH_TIMING_MODE)
        {
            rc = DIGI_ERR_ENET_CANNOT_SET_THROUGH_TIMING_MODE;
        } else if ( DIGI_ENET_TX_LINE_TIMING_MODE == timing_mode && FALSE == digi_handle->var.digi_m1_ctxt.is_pmm_lite_configured)
        {
            rc = DIGI_ERR_ENET_LINE_TIMING_MODE_NOT_CONFIG;
        }
    }

    /* we are ready to apply the new timing if it is not the same ...*/
    if (PMC_SUCCESS == rc && enet_chnl_def_handle_ptr->timing_mode != timing_mode) 
    { 
        /* remove previous CKCTL/ACB configuration */
        if (PMC_SUCCESS == rc)
        {
            rc = digi_non_otn_acb_ckctl_cfg(digi_handle, input_pc_ptr, output_pc_ptr , DIGI_SERDES_DEACTIVATE,enet_chnl_def_handle_ptr->timing_mode);
        }
        if (PMC_SUCCESS == rc)
        {
            rc = digi_non_otn_acb_ckctl_cfg(digi_handle, input_pc_ptr, output_pc_ptr , DIGI_SERDES_DEPROV,enet_chnl_def_handle_ptr->timing_mode);
        }
        
        
        /* provide the new configuration */
        if (PMC_SUCCESS == rc)
        {
            rc = digi_non_otn_acb_ckctl_cfg(digi_handle, input_pc_ptr, output_pc_ptr , DIGI_SERDES_PROV,timing_mode);
        }
        if (PMC_SUCCESS == rc)
        {
            rc = digi_non_otn_acb_ckctl_cfg(digi_handle, input_pc_ptr, output_pc_ptr , DIGI_SERDES_ACTIVATE,timing_mode);
        }

        /* 
           check that timing mode has been properly updated 
        */        
        enet_chnl_def_handle_ptr->timing_mode = timing_mode;
    }


    PMC_ATOMIC_RETURN(digi_handle,rc);
} /*digi_enet_tx_timing_mode_set*/

/*******************************************************************************
* digi_enet_tx_timing_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function returns the Ethernet Timing mode of a specified channel.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_enet_client_chnl  - pointer to ENET channel handle instance
*
* OUTPUTS:
*   *timing_mode             - enum, the requested timing mode
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_timing_mode_get(digi_handle_t *digi_handle,
                                              digi_enet_client_chnl_t *digi_enet_client_chnl,
                                              digi_enet_tx_timing_mode_t *timing_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_enet_client_chnl, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *) digi_enet_client_chnl;

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t*) digi_enet_client_chnl);
    }

    *timing_mode = enet_chnl_handle_ptr->timing_mode;

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /*digi_enet_tx_timing_mode_get*/

/*******************************************************************************
* digi_enet_tx_flow_control_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enable/Disable Link Ethernet TX flow control of a provisioned channel.\n
*  
*  Flow control can be controlled either by local resource (DCPB) or by \n
*  external resource (flow control information from ILKN interface). Whenever\n
*  the DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL mode is set (DCPB resources are\n
*  used to control TX flow control), it is assumed that DCPB buffer has been\n
*  properly sized when it has been allocated with digi_cpb_internal_chnl_map\n
*  API. 
*
*  Only datapaths through the MAC block can have flow control.
*
* INPUTS:
*   *digi_handle                - pointer to DIGI handle instance
*   *chnl_handle                - pointer to ENET or mapper channel handle instance
*   *digi_enet_link_pause_cfg   - pointer to the ENET link pause configuration.\n
*                                 This parameter shall be valid when we enable \n
*                                 TX flow control
*   enable                      - TRUE : Flow control enabled\n
*                                 FALSE: Flow control disabled 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_flow_control_set(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *chnl_handle,
                                               digi_enet_link_pause_cfg_t *digi_enet_link_pause_cfg,
                                               BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    UINT32 link;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(FALSE == enable || digi_enet_link_pause_cfg != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
  
    enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *) chnl_handle;

    /* Retrieve the enet handle & link */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, &enet_chnl_handle_ptr->switch_data);
    }

    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    } 

    if (PMC_SUCCESS == rc) {

        /* tx flow control */
        if (TRUE == enable)
        {
            /* [BT] CPB zone bypass is only used for ENET/MAP -> SIFD data paths. It can't be set here because we
             * also need to know the SIFD port and channel. The method to set zone bypass is to first call
             * digi_ilkn_cpb_chnl_prov and pass in rx_fc_reach_extend = FALSE.
             rc = cpb_dpi_slave_port_chnl_zone_type_cfg(digi_handle->dcpb_handle,
             enet_chnl_handle_ptr->switch_data.cpb_data.port_type,
             enet_chnl_handle_ptr->switch_data.cpb_data.channel,
             (DIGI_ENET_FLOW_CONTROL_BUFFER_EXTERNAL == digi_enet_link_pause_cfg->buffer_mode)?CPB_SLV_CHNL_ZONE_FIFO_AND_MSTR_XOFF:CPB_SLV_CHNL_ZONE_FIFO);
            */  
            if (PMC_SUCCESS == rc && DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL == digi_enet_link_pause_cfg->buffer_mode)
            {
                /* configure DCPB threshold */
                rc = cpb_dpi_slave_port_chnl_zone_thresh_cfg_check(digi_handle->dcpb_handle,
                                                                   enet_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                                   enet_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                                   0,
                                                                   0,                                                                 
                                                                   digi_enet_link_pause_cfg->xon_xoff_threshold);                
            }
        }
        if (PMC_SUCCESS == rc)
        {
            /*check pause_quanta /threshold config*/
            if (TRUE == enable &&
                digi_enet_link_pause_cfg->pause_quanta/2 >= digi_enet_link_pause_cfg->pause_quanta_thresh)
            {
                
                rc = DIGI_ERR_INVALID_PAUSE_QUANTA_THRES;
            }
            if (PMC_SUCCESS == rc )
            {
                /* configure TX flow control */
                rc = enet_flow_control_enable(enet_handle, 
                                              link, 
                                              UTIL_GLOBAL_DIR_TX,
                                              enable, 
                                              TRUE == enable ? digi_enet_link_pause_cfg->pause_quanta : 0xFFFF, 
                                              TRUE == enable ? digi_enet_link_pause_cfg->pause_quanta_thresh : 0xFFFF,
                                              DIGI_ENET_SYS_ZONE_THRESH_DEFAULT_VAL);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_rx_flow_control_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enable/Disable Link Ethernet RX flow control of a provisioned channel.\n
*  
*  By enabling RX flow conbtrol, ENET channel interprets XOFF and XON pause \n
*  frames received on the interface. XOFF and XONN pause respectively stop \n
*  and re-enable the ENET transmitter.
*  Only datapaths through the MAC block can have flow control.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *chnl_handle            - pointer to ENET or mapper channel handle instance
*   enable                  - TRUE : Flow control enabled\n
*                             FALSE: Flow control disabled 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_flow_control_set(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *chnl_handle,
                                               BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    /* internal fuction does error checking */
    rc = digi_enet_rx_flow_control_internal_set(digi_handle, chnl_handle, enable);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_rx_flow_control_set */

/*******************************************************************************
* digi_enet_tx_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Get Ethernet TX link flow control configuration of a provisioned channel.\n
*   
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_enet_client_chnl  - pointer to ENET channel handle instance
*
* OUTPUTS:
*   *digi_enet_link_pause_cfg - pointer to the ENET link pause configuration\n
*                               Only valid when flow control is enabled
*   *enable - TRUE : Flow control enabled\n
*             FALSE: Flow control disabled 
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_flow_control_get(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *digi_enet_client_chnl,      
                                               digi_enet_link_pause_cfg_t *digi_enet_link_pause_cfg,
                                               BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;

    UINT32 link;
    UINT32 dummy;
    UINT32 pause_quanta;
    UINT32 pause_quanta_thresh;
    cpb_dpi_chnl_zone_mode_t mode;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_enet_client_chnl, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_enet_link_pause_cfg,DIGI_ERR_INVALID_ARG, 0, 0);
    
    enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *) digi_enet_client_chnl;
    
    /* check the validaty of provided handle */
    rc = digi_enet_validate_parms(digi_handle, digi_enet_client_chnl,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, &enet_chnl_handle_ptr->switch_data);
    }

    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {
            
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        }

    }
    
    if (PMC_SUCCESS == rc) {

        rc = enet_flow_control_get(enet_handle, 
                                   link,
                                   UTIL_GLOBAL_DIR_TX,
                                   enable, 
                                   &pause_quanta, 
                                   &pause_quanta_thresh);
        digi_enet_link_pause_cfg->pause_quanta = pause_quanta;
        digi_enet_link_pause_cfg->pause_quanta_thresh = pause_quanta_thresh;
        if (PMC_SUCCESS == rc && TRUE == *enable) 
        {   
            rc = cpb_dpi_slave_port_chnl_zone_type_cfg_get(digi_handle->dcpb_handle,
                                                           enet_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                           enet_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                           &mode);
             
            digi_enet_link_pause_cfg->buffer_mode = (mode == CPB_SLV_CHNL_ZONE_FIFO)?DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL:DIGI_ENET_FLOW_CONTROL_BUFFER_EXTERNAL;
            /* when zone_bypass is disabled, get zone thresholds */
            if (PMC_SUCCESS == rc && DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL == digi_enet_link_pause_cfg->buffer_mode)
            {
                rc = cpb_dpi_slave_port_chnl_zone_thresh_cfg_get(digi_handle->dcpb_handle,
                                                                 enet_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                                 enet_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                                 &dummy,
                                                                 &dummy,
                                                                 &digi_enet_link_pause_cfg->xon_xoff_threshold); 
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_rx_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Get Ethernet RX link flow control configuration of a provisioned channel.\n
*   
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_enet_client_chnl  - pointer to ENET channel handle instance
*
* OUTPUTS:
*   *enable - TRUE : Flow control enabled\n
*             FALSE: Flow control disabled 
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_flow_control_get(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *digi_enet_client_chnl,                                   
                                               BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;

    UINT32 link;
    UINT32 dummy_pause_quanta;
    UINT32 dummy_pause_quanta_thresh;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_enet_client_chnl, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check the validaty of provided handle */
    rc = digi_enet_validate_parms(digi_handle, digi_enet_client_chnl,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) digi_enet_client_chnl);
    }

    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {
            rc = enet_flow_control_get(enet_handle, link, UTIL_GLOBAL_DIR_RX, 
                                       enable,
                                       &dummy_pause_quanta,
                                       &dummy_pause_quanta_thresh);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_rx_pause_frame_fwd_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enable/Disable Ethernet RX pause frame forwarding to the client interface.\n
*  
*  Pause frame forwarding is allowed only when RX flow control is disabled.
*  Only datapaths through the MAC block can have flow control.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *chnl_handle            - pointer to ENET or mapper channel handle instance
*   enable                  - TRUE : RX pause frame fowarding is enabled\n
*                             FALSE: RX pause frame fowarding is disabled
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_pause_frame_fwd_set(digi_handle_t             *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  BOOL8                      enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    /* internal fuction does error checking */
    rc = digi_enet_rx_pause_frame_fwd_internal_set(digi_handle, chnl_handle, enable);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_rx_pause_frame_fwd_set */

/*******************************************************************************
* digi_enet_rx_pause_frame_fwd_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This functions returns weather the RX pause frame fowarding is enabled. 
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *chnl_handle            - pointer to ENET or mapper channel handle instance
*
* OUTPUTS:
*  *enabled_ptr            - TRUE : Pause frame forwarding is enabled\n
*                            FALSE: Pause frame forwarding is disabled 
*
* RETURNS:
*  None.
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_pause_frame_fwd_get(digi_handle_t             *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  BOOL8                     *enabled_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != enabled_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);
       
    /* Retrieve the enet handle & link */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;            
        } 
        else 
        {
            enet_emac_pause_frame_fwd_get(enet_handle, link, enabled_ptr);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_pause_frame_fwd_get */

/*******************************************************************************
* digi_enet_flow_control_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function obtains the flow control status of the provisioned channel.
*  Only datapaths through the MAC block can have this status.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *pause_received_ptr    - a pointer to flow control status
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_flow_control_status_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL *pause_received_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT32 pause_status;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } 
    }

    if (PMC_SUCCESS == rc) {

        rc = emac_pause_stat_get(enet_handle->emac_handle, link, &pause_status);

        if (pause_status & DIGI_ENET_CHAN_0_PAUSED) {

            *pause_received_ptr = TRUE;

        } else {

            *pause_received_ptr = FALSE;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_flow_control_status_get */

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

/*******************************************************************************
* digi_enet_unidirectional_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED: This function is removed starting in the 3.04 release.
*   This function is used to support Unidirectional Ethernet (IEEE 802.3 
*   clause 66.3.2.1) for 10G link. Enabling this mode prevent egress traffic to 
*   be squelched upon reception of ordered set in ingress direction 
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet 
*   enable                 - TRUE: enable unidirectional mode 
*                            FALSE: disable unidirectional mode 
*
* OUTPUTS:
*   none
*
 RETURNS:
*   PMC_SUCCESS - Success
*   DIGI_ERR_UNSUPPORTED_REV_A_FEATURE - This feature is not support for rev A.
*   DIGI_ERR_ENET_LINK_NOT_MAC - This feature is only supported on MAC terminated link 
*   DIGI_ERR_ENET_LINK_NOT_MAC - This feature is only supported on 10G link 
*   DIGI_ERR_INVALID_ARG - path type is not in MAC block or length is not valid.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_unidirectional_mode_set(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate;

    PMC_ATOMIC_ENTRY(digi_handle);
    if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
    {
        rc = DIGI_ERR_UNSUPPORTED_REV_A_FEATURE;
    } 

    if(PMC_SUCCESS == rc) {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                        &enet_handle, &link);
    }

    if(PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;                
        } else 
        {
            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
        }
    }
    
    if (PMC_SUCCESS == rc)
    {
        if (data_rate != EMAC_LINK_10G)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_10G;
        } else 
        {                
            if (TRUE == enable)
            {
                rc = enet_emac_lf_rf_cfg(enet_handle, link, FALSE, TRUE,TRUE);
            } else 
            {
                rc = enet_emac_lf_rf_cfg(enet_handle, link, TRUE, FALSE, FALSE);
            
            }
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_unidirectional_mode_set */


/*******************************************************************************
* digi_enet_unidirectional_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED: This function is removed starting in the 3.04 release.
*   This function returns the Unidirectional Ethernet mode (IEEE 802.3 
*   clause 66.3.2.1) for a provided 10G MAC terminated link.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet 
*
* OUTPUTS:
*   *enable                - TRUE: unidirectional mode enabled
*                            FALSE: unidirectional mode disabled
*
 RETURNS:
*   PMC_SUCCESS - Success
*   DIGI_ERR_UNSUPPORTED_REV_A_FEATURE - This feature is not support for rev A.
*   DIGI_ERR_ENET_LINK_NOT_MAC - This feature is only supported on MAC terminated link 
*   DIGI_ERR_ENET_LINK_NOT_MAC - This feature is only supported on 10G link 
*   DIGI_ERR_INVALID_ARG - path type is not in MAC block or length is not valid.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_unidirectional_mode_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate;

    PMC_ATOMIC_ENTRY(digi_handle);
    if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
    {
        rc = DIGI_ERR_UNSUPPORTED_REV_A_FEATURE;
    } 

    if(PMC_SUCCESS == rc) {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                        &enet_handle, &link);
    } 

    if(PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;                
        } else 
        {
            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
        }
    }
    
    if (PMC_SUCCESS == rc)
    {
        if (data_rate != EMAC_LINK_10G)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_10G;
        } else 
        {                  
            rc =  emac_unidirectional_mode_get(enet_handle->emac_handle, link,enable);
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_unidirectional_mode_get */

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */


/*******************************************************************************
* digi_enet_mru_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the MRU of the provisioned channel.
*   Only datapaths through the MAC block can support MRU.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   length                 - length of maximum receive unit
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block or length is not valid
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mru_set(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 length)
{
    PMC_ERROR rc = PMC_SUCCESS;

    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    /* internal fuction does error checking */
    rc = digi_enet_mru_internal_set(digi_handle, chnl_handle, length);

    if (rc == PMC_SUCCESS) {

        enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

        if (enet_pmon_chnl_handle != NULL) {

            rc = digi_enet_mru_internal_set(digi_handle, enet_pmon_chnl_handle, length);
        }
    }
      
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mru_set */

/*******************************************************************************
* digi_enet_ipg_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IPG of the provisioned channel.
*   For 10G link, 
*       the function sets the number of octets in steps of 4.        
*   For 40G and 100G:
*       the function sets the transmit PPM compensation.
* 
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_handle        - pointer to Enet or mapper channel handle instance
*   gap                 - For 10G link:
*                           it is the number of octets in step of 4. Valid values 
*                            are 8, 12, 16, ... 100. 
*                            1) If DIGI is a transit-node, set gap to 8
*                            2) If DIGI is an Ethernet source, set gap to 12
*                            Note: When set to 8, PCS layers may not be able to perform
*                            clock rate compensation.
*                         For 40G and 100G link:
*                            The function sets the transmit PPM compensation.
*                            1) When gap == 2'b00 : Normal mode of operation. Markers compensation 
*                                                   active meaning effective +60ppm.
*                            2) When gap == 2'b01 : Default compensation plus 60ppm (total of +120ppm).
*                            3) When gap == 2'b10 : Default compensation plus 140ppm (total of +200ppm).
*                            4) When gap == 2'b11 : Default compensation plus 200ppm (total of +260ppm).
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_ipg_set(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 gap)
{
    PMC_ERROR rc = PMC_SUCCESS;

    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    /* internal fuction does error checking */
    rc = digi_enet_ipg_internal_set(digi_handle, chnl_handle, gap);

    if (rc == PMC_SUCCESS) {

        enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

        if (enet_pmon_chnl_handle != NULL) {

            rc = digi_enet_ipg_internal_set(digi_handle, enet_pmon_chnl_handle, gap);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_ipg_set */


/*******************************************************************************
* digi_enet_ins_mac_addr_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set TX_ADDR_INS and MAC address configuration.
* 
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   *chnl_handle     - pointer to Enet or mapper channel handle instance
*   tx_addr_ins_en   - The TX_ADDR_INS bit controls the transmit MAC address generation
*   mac_addr_msb     - MSB of MAC address, low 16 bits valid
*   mac_addr_lsb     - LSB of MAC address
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_ins_mac_addr_set(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL8 tx_addr_ins_en,
                                            UINT32 mac_addr_msb,
                                            UINT32 mac_addr_lsb)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }

    if (PMC_SUCCESS == rc) 
    {
        emac_ins_mac_addr_set(enet_handle->emac_handle, 
                              link, 
                              tx_addr_ins_en,
                              mac_addr_msb, 
                              mac_addr_lsb);
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_ins_mac_addr_set */

/*******************************************************************************
* digi_enet_ins_mac_addr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get TX_ADDR_INS and MAC address configuration.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   *chnl_handle     - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *tx_addr_ins_en  - The TX_ADDR_INS bit controls the transmit MAC address generation
*   *mac_addr_msb    - MSB of MAC address
*   *mac_addr_lsb    - LSB of MAC address
*
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_ins_mac_addr_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL8 *tx_addr_ins_en,
                                            UINT32 *mac_addr_msb,
                                            UINT32 *mac_addr_lsb)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }

    if (PMC_SUCCESS == rc)
    {
        emac_ins_mac_addr_get(enet_handle->emac_handle,
                              link,
                              tx_addr_ins_en,
                              mac_addr_msb,
                              mac_addr_lsb);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_ins_mac_addr_get */


/*******************************************************************************
* digi_enet_loopback_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the loopback of the provisioned channel.
*   Loopback is a remote loopback where the MAC TX output is
*   connected to the MAC RX input.  For a local loopback, this
*   must be performed either in the SERDES port or OTN path.
*   Only datapaths through the MAC block can support loopback.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - loopback enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block or loopback is not valid
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_loopback_set(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);


    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_pcs_lpbk_en(enet_handle->emac_handle, link, enable);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_loopback_set */

/*******************************************************************************
* digi_enet_send_test_pattern_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets test pattern generation of the provisioned channel.
*   The 40G & 100G test pattern is a scrambled idle test pattern.
*   The 10G test pattern can be square wave/pseudo-random with a data
*   pattern of zeros/LF.
*   Only datapaths through the MAC block can support test patterns.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   tx_enable              - send test pattern enable (TRUE) or disable (FALSE)
*   rx_enable              - receive test pattern enable (TRUE) or disable (FALSE)
*   use_test_seed_b        - seed selection - use seed B (TRUE) or seed A (FALSE)
*   seed_msb               - high-order 26 bits of seed
*   seed_lsb               - low-order 32 bits of seed
*   use_10g_square_wave    - 10G test pattern is square wave (TRUE) or pseudo-random (FALSE)
*   use_10g_zeros_data     - 10G data pattern is zeros (TRUE) or LF (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_send_test_pattern_set(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 BOOL tx_enable, 
                                                 BOOL rx_enable,
                                                 BOOL use_test_seed_b,
                                                 UINT32 seed_msb, 
                                                 UINT32 seed_lsb,
                                                 BOOL use_10g_square_wave,
                                                 BOOL use_10g_zeros_data)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);


    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_pcs_10g_test_patt_seed_cfg(enet_handle->emac_handle, link, 
                                                 (emac_pcs_seed_sel_t)use_test_seed_b, seed_msb, seed_lsb);
            if ( PMC_SUCCESS == rc)
            {
                rc = emac_pcs_test_patt_cfg(enet_handle->emac_handle, link,
                                            use_10g_square_wave, use_10g_zeros_data,
                                            tx_enable, rx_enable);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_send_test_pattern_set */

/*******************************************************************************
* digi_enet_mon_prbs_resync
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function forces EMAC PRBS monitor resynchronization of the 
*   provisioned channel.
*   Only datapaths through the MAC block can support PRBS.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mon_prbs_resync(digi_handle_t             *digi_handle,
                                           util_global_switch_data_t *chnl_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_mon_prbs_resync(enet_handle->emac_handle, link);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mon_prbs_resync */

/*******************************************************************************
* digi_enet_mon_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates or deactivates PRBS9 and PRBS31 monitor 
*   of the provisioned channel.
*   Only datapaths through the MAC block can support PRBS.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - generate PRBS enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mon_prbs_set(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL                       enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    } 

    if (rc == PMC_SUCCESS) {

        rc = emac_prbs_cfg(enet_handle->emac_handle, 
                           link, 
                           EMAC_DIR_ING, 
                           LAST_UTIL_PATT_MODE, 
                           enable);
    }
    

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mon_prbs_set */

/*******************************************************************************
* digi_enet_mon_prbs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves PRBS9 and PRBS31 monitor of the provisioned channel.
*   Only datapaths through the MAC block can support PRBS.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *enable                - generate PRBS enable (TRUE) or disable (FALSE)
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mon_prbs_get(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL8                     *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    util_patt_mode_t prbs_type;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    } 

    if (rc == PMC_SUCCESS) {
        rc = emac_prbs_get(enet_handle->emac_handle, 
                           link, 
                           EMAC_DIR_ING, 
                           &prbs_type, 
                           enable);
    }
    

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mon_prbs_get */

/*******************************************************************************
* digi_enet_gen_prbs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets PRBS generation of the 
*   provisioned channel.
*   Only datapaths through the MAC block can support PRBS.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - generate PRBS enable (TRUE) or disable (FALSE)
*   prbs_type              - PRBS type, only PRBS9 and PRBS31 are supported.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_gen_prbs_set(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL                       enable,
                                        util_patt_mode_t           prbs_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_prbs_cfg(enet_handle->emac_handle, link, EMAC_DIR_EGR, prbs_type, enable);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_gen_prbs_set */

/*******************************************************************************
* digi_enet_gen_prbs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves PRBS generation of the provisioned channel.
*   Only datapaths through the MAC block can support PRBS.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *enable                - generate PRBS enable (TRUE) or disable (FALSE)
*   *prbs_type             - PRBS type, only PRBS9 and PRBS31 are supported.
*
* RETURNS:
*   PMC_SUCCESS when operation is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_gen_prbs_get(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL8                     *enable,
                                        util_patt_mode_t          *prbs_type)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_prbs_get(enet_handle->emac_handle, link, EMAC_DIR_EGR, prbs_type, enable);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_gen_prbs_get */

/*******************************************************************************
* digi_enet_prbs_error_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves PRBS monitor error counts.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   prbs_type              - PRBS type, only PRBS9 and PRBS31 are supported
*
* OUTPUTS:
*   *err_cnt               - PRBS monitor error counts
*
* RETURNS:
*   PMC_SUCCESS when error_cnt is successfully retrieved otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_prbs_error_cnt_get(digi_handle_t                *digi_handle,
                                              util_global_switch_data_t    *chnl_handle,
                                              util_patt_mode_t              prbs_type,
                                              UINT32                       *err_cnt)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check associated serdes PRBS monitor error_cnt */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } 
        else {
            rc = emac_prbs_error_cnt_get(enet_handle->emac_handle, link, prbs_type, err_cnt);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_prbs_error_cnt_get */

/*******************************************************************************
* digi_enet_prbs_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves PRBS monitor status.
*   
*   This API is not supported for DIGI Revicion A.
*
*   This function should be called prior to digi_enet_prbs_error_cnt_get() 
*   to determine that EMAC PRBS monitor is locked.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *status                - PRBS monitor status. When PRBS is not locked, EMAC
*                            PRBS error counter read with API  digi_enet_prbs_error_cnt_get() 
*                            should be ignored.
*   
*
* RETURNS:
*   PMC_SUCCESS when status is successfully retrieved otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_prbs_status_get(digi_handle_t              *digi_handle,
                                           util_global_switch_data_t  *chnl_handle,
                                           util_patt_status_t         *status)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    

    PMC_ATOMIC_ENTRY(digi_handle);

    /* for rev b only, use EMAC block register bit PRBS_ALL_ONES to determine PRBS status */
    if (FALSE == pmc_is_digi_rev_a_revision(&digi_handle->base))
    {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                      &enet_handle, &link);

        if (PMC_SUCCESS == rc)
        {
            /* check handle provisioning state */
            DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
        }
        if (PMC_SUCCESS == rc) {
            if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
                rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        } 

        if (PMC_SUCCESS == rc)
        {            
            rc = emac_prbs_status_get(enet_handle->emac_handle, link, status);
        }
    }
    else
    {
        rc = DIGI_ERR_UNSUPPORTED_REV_A_FEATURE;
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_prbs_status_get */


/*******************************************************************************
* digi_enet_send_packets_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets packet generation of the provisioned channel.
*   Only datapaths through the MAC block can generate packets.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - packet generation enable (TRUE) or disable (FALSE)
*   *test_pkt_ptr          - pointer to test packet structure
*                            (L2 header, size, payload & interval)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG  - path type is not in MAC block or packet is not valid
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_send_packets_set(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL enable,
                                            digi_enet_test_packet_t *test_pkt_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT32 outer_vlan, inner_vlan;
    UINT16 ether_type;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc)
    {
        if (enable) {
            PMC_ASSERT(NULL != test_pkt_ptr,DIGI_ERR_INVALID_ARG, 0, 0);

            if (test_pkt_ptr->outer_vlan == 0) {
                outer_vlan = ((UINT32)test_pkt_ptr->ether_type << 16)
                    | ((UINT32)test_pkt_ptr->payload << 8) | (UINT32)test_pkt_ptr->payload;
                inner_vlan = ((UINT32)test_pkt_ptr->payload << 24) | ((UINT32)test_pkt_ptr->payload << 16)
                    | ((UINT32)test_pkt_ptr->payload << 8) | (UINT32)test_pkt_ptr->payload;
                ether_type = ((UINT16)test_pkt_ptr->payload << 8) | (UINT32)test_pkt_ptr->payload;
            }
            else {
                outer_vlan = test_pkt_ptr->outer_vlan;
                if (test_pkt_ptr->inner_vlan == 0) {
                    inner_vlan = ((UINT32)test_pkt_ptr->ether_type << 16)
                        | ((UINT32)test_pkt_ptr->payload << 8) | (UINT32)test_pkt_ptr->payload;
                    ether_type = ((UINT16)test_pkt_ptr->payload << 8) | (UINT32)test_pkt_ptr->payload;
                }
                else {
                    inner_vlan = test_pkt_ptr->inner_vlan;
                    ether_type = test_pkt_ptr->ether_type;
                }
            }

        
            rc = empei_e_pkt_gen_cfg(enet_handle->empei_e_handle, 
                                     (UINT32)test_pkt_ptr->packet_size, (UINT32)test_pkt_ptr->payload,
                                     (UINT32)test_pkt_ptr->interval,
                                     (UINT32)test_pkt_ptr->dest_addr_hi, test_pkt_ptr->dest_addr_lo,
                                     (UINT32)test_pkt_ptr->src_addr_hi, test_pkt_ptr->src_addr_lo,
                                     (UINT32)ether_type, outer_vlan, inner_vlan);
        }

        /* Enable/disable packet generator */
        if ( PMC_SUCCESS == rc)
        {
            rc = empei_e_pkt_gen_enable(enet_handle->empei_e_handle, link, enable);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_send_packets_set */


/*******************************************************************************
* digi_enet_send_packets_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets packet generation of the provisioned channel.
*   Only datapaths through the MAC block can generate packets.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *enable                 - packet generation enable (TRUE) or disable (FALSE)
*   *test_pkt_ptr          - pointer to test packet structure
*                            (L2 header, size, payload & interval)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG  - path type is not in MAC block or packet is not valid
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_send_packets_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL *enable,
                                            digi_enet_test_packet_t *test_pkt_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 sw_link;
    UINT32 hw_link;
    UINT32 pkt_size; 
    UINT32 pyld_val; 
    UINT32 pkt_interval; 
    UINT32 da_hi; 
    UINT32 da_low; 
    UINT32 sa_hi; 
    UINT32 sa_low;
    UINT32 len_etype; 
    UINT32 vlan_out; 
    UINT32 vlan_in;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &sw_link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    rc = empei_e_pkt_gen_status_get(enet_handle->empei_e_handle, &hw_link, (BOOL8 *)enable);
    
    if (PMC_SUCCESS == rc && sw_link != hw_link)
    {
        return DIGI_ERR_INVALID_ARG;
    }
    
    if (PMC_SUCCESS == rc && enable)
    {
        rc = empei_e_pkt_gen_cfg_get(enet_handle->empei_e_handle,
                                     &pkt_size,
                                     &pyld_val,
                                     &pkt_interval,
                                     &da_hi,
                                     &da_low,
                                     &sa_hi,
                                     &sa_low,
                                     &len_etype,
                                     &vlan_out,
                                     &vlan_in);
        if (PMC_SUCCESS == rc)
        {
            test_pkt_ptr->packet_size  = pkt_size;
            test_pkt_ptr->interval     = pkt_interval;
            test_pkt_ptr->dest_addr_hi = da_hi;
            test_pkt_ptr->dest_addr_lo = da_low;
            test_pkt_ptr->src_addr_hi  = sa_hi;
            test_pkt_ptr->src_addr_lo  = sa_low;
            test_pkt_ptr->ether_type   = len_etype;
            test_pkt_ptr->outer_vlan   = vlan_out;
            test_pkt_ptr->inner_vlan   = vlan_in;
            test_pkt_ptr->payload      = pyld_val;

            if (len_etype == (((UINT32)pyld_val<<8)|((UINT32)pyld_val)) )
            {
                if (vlan_in == (((UINT32)pyld_val<<24)|((UINT32)pyld_val<<16)|((UINT32)pyld_val<<8)|((UINT32)pyld_val)) )
                {
                    test_pkt_ptr->ether_type = (UINT32)vlan_out>>16;
                    test_pkt_ptr->outer_vlan = 0;
                }
                else
                {
                    test_pkt_ptr->ether_type = (UINT32)vlan_in>>16;
                    test_pkt_ptr->inner_vlan = 0;
                }
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_send_packets_get */

/*******************************************************************************
* digi_enet_send_packets_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the status of the ethernet packet generator.
*
* INPUTS:
*   *digi_handle                - pointer to DIGI handle instance
*   *chnl_handle                - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *enabled_ptr                - Set to TRUE if the packet generator is enabled.
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG        - path type is not in MAC block or packet is not valid
*   DIGI_ERR_ENET_PKT_GEN_LINK  - Packet generator is enabled on a different link.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_send_packets_status_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL *enabled_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT32 pkt_gen_link; 
    BOOL8 int_enabled_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    PMC_ASSERT(rc == PMC_SUCCESS,rc,0,0);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    rc = empei_e_pkt_gen_status_get(enet_handle->empei_e_handle, 
                                    &pkt_gen_link,
                                    &int_enabled_ptr);
    if (int_enabled_ptr) 
    {
        if (pkt_gen_link != link)
        {
            rc = DIGI_ERR_ENET_PKT_GEN_LINK;
        }
    }

    *enabled_ptr = UTIL_GLOBAL_BOOL8_TO_BOOL_CONVERT(int_enabled_ptr);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_send_packets_status_get */

/*******************************************************************************
* digi_enet_phy_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables the PHY of the provisioned channel
*   by enabling the MAC RX.
*   Only datapaths through the MAC block can enable the PHY.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - PHY enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_phy_set(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);


    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    /* internal fuction does error checking */
    rc = digi_enet_phy_internal_set(digi_handle, chnl_handle, enable);

    if (rc == PMC_SUCCESS) {

        enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

        if (enet_pmon_chnl_handle != NULL) {

            rc = digi_enet_phy_internal_set(digi_handle, enet_pmon_chnl_handle, enable);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_phy_set */



/*******************************************************************************
* digi_enet_rx_emac_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables the MAC RX of the provisioned channel
*   by enabling the MAC RX.
*   Only datapaths through the MAC block can enable the PHY.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - MAC RX enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*   DIGI_ERR_NULL_HANDLE
*   DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET - No ENET provisioned as part of MAPOTN channel handle.
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_emac_set(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *chnl_handle,
                                       BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
                                  
    if (PMC_SUCCESS == rc)
    {
    
        rc = emac_rx_enable(enet_handle->emac_handle, link, enable);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_rx_emac_set */


/*******************************************************************************
* digi_enet_tx_mac_fault_insert_ctrl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the behavior of Transmit Fault Insertion.
*   In cases where FW is using the associated register space for
*   consequential actions, this API call will make the required FW
*   calls to ensure no contention between SW driver and FW.\n\n
*
*   This API is used to specify Unidirectional Ethernet mode (IEEE 802.3 
*   clause 66.3.2.1) for a provided 10G MAC terminated link only, other
*   rates return an error.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   mode                   - Enum defining fault insertion mode, see
*
* OUTPUTS:
*   none
*

* RETURNS:
*   PMC_SUCCESS            - if successful otherwise meaningful error code
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_mac_fault_insert_ctrl(digi_handle_t *digi_handle,
                                                    util_global_switch_data_t *chnl_handle,
                                                    digi_enet_tx_mac_fault_t mode)
{
    PMC_ERROR rc = PMC_SUCCESS;

    BOOL8 flt_hdl_dis = (mode & DIGI_ENET_FLT_HDL_DIS_MSK) ? TRUE:FALSE;
    BOOL8 send_rf = (mode & DIGI_ENET_SEND_RF_MSK) ? TRUE:FALSE;
    BOOL8 send_lf = (mode & DIGI_ENET_SEND_LF_MSK) ? TRUE:FALSE;
    BOOL8 etrans_fault_on = FALSE;
    BOOL8 etrans_legacy_mode = FALSE;

    emac_data_rate_t data_rate;
    UINT32 dummy = 0;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    PMC_ATOMIC_ENTRY(digi_handle);


    if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base) && 
        mode == DIGI_ENET_TX_MAC_FAULT_UNIDIRECTIONAL)
    {
        rc = DIGI_ERR_UNSUPPORTED_REV_A_FEATURE;
    } 

    if(PMC_SUCCESS == rc) {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                      &enet_handle, &link);
    }

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    
    if(PMC_SUCCESS == rc) {
        if(digi_enet_is_link_mac_block(enet_handle, link) != TRUE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        } 
        else if(mode == DIGI_ENET_TX_MAC_FAULT_UNIDIRECTIONAL)
        {
            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
            if(rc == PMC_SUCCESS && data_rate != EMAC_LINK_10G)
                rc = DIGI_ERR_ENET_LINK_NOT_10G;
        }
    }

    if(PMC_SUCCESS == rc && 
       enet_is_link_mapotn_test(enet_handle, link, &dummy) == TRUE && 
       (mode == DIGI_ENET_TX_MAC_FAULT_FORCE_LF || mode == DIGI_ENET_TX_MAC_FAULT_FORCE_RF))
        etrans_fault_on = TRUE;

    if(rc == PMC_SUCCESS && !(mode != DIGI_ENET_TX_MAC_FAULT_ILLEGAL_1 && 
                              mode != DIGI_ENET_TX_MAC_FAULT_ILLEGAL_2 && 
                              mode != DIGI_ENET_TX_MAC_FAULT_ILLEGAL_3))
        rc = DIGI_ERR_INVALID_ARG;

    if(rc == PMC_SUCCESS && mode >= LAST_DIGI_ENET_TX_MAC_FAULT)
        rc = DIGI_ERR_INVALID_ARG;

    /* update FW ENET source if needed, otherwise FW operations will 
       be executed on the wrong ENET source */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_enet_fw_enet_client_source_conditional_update(digi_handle,
                                                                 chnl_handle);
    }

    if (rc == PMC_SUCCESS)
    {
        UINT8 data_upi, os_upi;

        etrans_legacy_mode = digi_enet_gsup43_7_3_legacy_mode_chk(digi_handle, chnl_handle, &data_upi, &os_upi);
        rc = digi_cmf_enet_emac_lf_rf_cfg(digi_handle, enet_handle->emac_handle, link, flt_hdl_dis, send_rf, send_lf, etrans_fault_on, etrans_legacy_mode);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_tx_mac_fault_insert_ctrl */

/*******************************************************************************
* digi_enet_robust_dic_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures/Unconfigures serdes of ENET LINE or SYS channel to support
*   Deficit Idle Count (DIC) compensation.
*
*   Note that the API should be called IMMEDIATELY after channel provisoning
*   and BEFORE channel mapping, activation or any channel configuration 
*   with runtime API.
*
*   Note that DIC compensation is supported only for 40G and 100G MAC channels.
*
*   Note that the API should be called when channel is not mapped to DCPB.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet channel handle instance
*    enable                - When TRUE : serdes rate is updated to allow DIC support
*                            When FALSE: serdes rate is updated to its initial
*                                        rate before DIC configuration.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, on success. A specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_robust_dic_cfg(digi_handle_t             *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL8                      enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_serdes_port_t *port_ctxt_ptr = NULL;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate;
    digi_serdes_port_mode_t serdes_mode = DIGI_SERDES_PORT_MODE_ANY_LOWPWR;
    digi_serdes_prov_state_t prov_state = DIGI_SERDES_PROV_STATE_NO_SERDES;
    BOOL8 prov_status;
    digi_enet_client_chnl_def_t *enet_switch_ptr;
    enet_datapath_t e_mode = LAST_ENET_DATAPATH;
    digi_group_size_t group_size = LAST_DIGI_GRP_SIZE;
    digi_enet_mode_t mode;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* check that channel if not mapped to any channel before configuring serdes */
    enet_switch_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    if (enet_switch_ptr->switch_data.cpb_data.op_state.map_state.is_output_mapped == TRUE ||
        enet_switch_ptr->switch_data.cpb_data.op_state.map_state.is_input_mapped == TRUE)
    {
        result = DIGI_ERR_CPB_ALREADY_MAPPED;
    }

    /* check that ENET channel is a MAC 40G or 100G */    
    if (PMC_SUCCESS == result) 
    {
        result = digi_enet_validate_parms(digi_handle, chnl_handle,
                                        &enet_handle, &link);
    }


    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    
    if (PMC_SUCCESS == result &&
        TRUE == enable) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            result = DIGI_ERR_ENET_LINK_NOT_MAC;                
        } 
        else 
        {
            result = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
            if (PMC_SUCCESS == result)
            {
                if (EMAC_LINK_40G != data_rate  &&
                    EMAC_LINK_100G != data_rate)
                {
                    result = DIGI_ERR_ENET_INVALID_DATAPATH;
                }
            }
        }
    }

    /* get serdes port associated to ENET channel */    
    if(PMC_SUCCESS == result) 
    {
        result = digi_enet_serdes_port_get(digi_handle,
                                           chnl_handle,
                                           &port_ctxt_ptr);
    }
    
    /* retrieve serdes current properties */
    if (PMC_SUCCESS == result) 
    {    
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                port_ctxt_ptr,
                                                                TRUE,
                                                                &prov_status,
                                                                &serdes_mode,
                                                                &prov_state);
    }

    digi_enet_line_prov_params_convert(enet_switch_ptr->mode, &e_mode, &group_size);
    
    mode = ((digi_enet_client_chnl_def_t *)chnl_handle)->mode;

    /* rdeactivate and unconfigure and enet link */
    if (PMC_SUCCESS == result)
    {
        result = enet_rx_deactivate(enet_handle, 
                                    link, 
                                    (enet_handle)->var.ch_datapath[link]);
    }
    
    if (result == PMC_SUCCESS)
    {
        result = enet_tx_deactivate(enet_handle, link, enet_handle->var.ch_datapath[link]);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = enet_datapath_uncfg (enet_handle,
                                      link);
    }

    /* reset serdes */
    if (PMC_SUCCESS == result) 
    {
        /* deactivate SerDes Port and put in lowest energy state */
        result = digi_serdes_port_deprov(digi_handle, port_ctxt_ptr);
    }
    
    /* configure new rate */
    if (PMC_SUCCESS == result)
    {
        /* Provision the SERDES with new serdes rate */
        if (TRUE == enable)
        {
            serdes_mode = DIGI_SERDES_PORT_MODE_S16_DIC;
        }
        else
        {
            digi_enet_serdes_mode_get(mode, &serdes_mode);
        }

        result = digi_serdes_port_prov(digi_handle,
                                       port_ctxt_ptr,
                                       serdes_mode);
    }

    /* reconfigure and activate enet link */
    if (PMC_SUCCESS == result)
    {
        result = enet_datapath_cfg (enet_handle,
                                    link,
                                    e_mode,
                                    ENET_MAPOTN_CHANNEL_NONE, 
                                    FALSE, FALSE);
    }

    if (PMC_SUCCESS == result)
    {
        result = enet_rx_activate(enet_handle, 
                                  link, 
                                  e_mode);
    }
    
    if (result == PMC_SUCCESS)
    {
        result = enet_tx_activate(enet_handle, link, e_mode);
    }
    

    /* save back serdes properties */
    if (PMC_SUCCESS == result)
    {        
        result = digi_serdes_port_rate_state_set(digi_handle, port_ctxt_ptr, serdes_mode, prov_state, prov_status == TRUE ? DIGI_SERDES_PORT_RATE_STATE_PROV : DIGI_SERDES_PORT_RATE_STATE_DEPROV);
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_robust_dic_cfg */

/*******************************************************************************
* digi_enet_robust_dic_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check serdes rate to determine if ENET LINE or SYS channel supports
*   Deficit Idle Count (DIC) compensation.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet channel handle instance
*
* OUTPUTS:
*   *enable_ptr           - When TRUE : serdes rate are configured to support ENET DIC compensation.
*                           When FALSE: ENET channel does not support DIC compensation.
*
* RETURNS:
*   PMC_SUCCESS, on success. A specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_robust_dic_status_get(digi_handle_t             *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 BOOL8                      *enable_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_serdes_port_t *port_ctxt_ptr = NULL;
    digi_serdes_port_mode_t serdes_mode = DIGI_SERDES_PORT_MODE_ANY_LOWPWR;
    digi_serdes_prov_state_t prov_state = DIGI_SERDES_PROV_STATE_NO_SERDES;
    BOOL8 prov_status;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != enable_ptr, DIGI_ERR_INVALID_ARG, 0, 0);


    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    *enable_ptr = FALSE;
    /* get serdes port associated to ENET channel */        
    result = digi_enet_serdes_port_get(digi_handle,
                                       chnl_handle,
                                       &port_ctxt_ptr);    
    /* retrieve serdes port status and rate  */
    if (PMC_SUCCESS == result) 
    {
        result = digi_serdes_port_rate_prov_status_internal_get(digi_handle,
                                                                port_ctxt_ptr,
                                                                TRUE,
                                                                &prov_status,
                                                                &serdes_mode,
                                                                &prov_state);
    }
    
    /* check serdes rate to determine if serdes is configured to allow ENET to supprot DIC compensation */
    if (PMC_SUCCESS == result) 
    {
        if (DIGI_SERDES_PORT_MODE_S16_DIC == serdes_mode)
        {
            *enable_ptr = TRUE;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_robust_dic_status_get */


#ifndef DOXYGEN_PUBLIC_ONLY 
/*******************************************************************************
* digi_enet_gsup43_7_3_legacy_mode_chk
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if GSUP43 7.3 legacy mode is enabled. The chnl_ptr 
*   can be a MAPOTN channel ptr for datapaths that use ETRANS or an ENET 
*   channel handle for datapaths that use EPMM.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ptr              - pointer to Enet or mapper channel handle instance
*   *data_upi_ptr          - Data frame UPI value
*   *os_upi_ptr            - Ordered Set UPI value
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS            - if successful otherwise meaningful error code
*
*
*******************************************************************************/
PUBLIC BOOL digi_enet_gsup43_7_3_legacy_mode_chk(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *chnl_ptr,
                                                  UINT8 *data_upi_ptr,
                                                  UINT8 *os_upi_ptr)
{
    PMC_ERROR result;
    BOOL etrans_mode = FALSE, epmm_mode = FALSE;
    UINT32 mapotn_chnl;
    BOOL8 dummy_val;
    digi_mapper_chnl_def_t *mapper_chnl_ptr= (digi_mapper_chnl_def_t *)chnl_ptr;

    PMC_ENTRY();

    result = digi_mapper_chnl_handle_validate(digi_handle,chnl_ptr);

    if (result == PMC_SUCCESS)
    {
        etrans_mode = (mapper_chnl_ptr->mode == DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP);
        mapotn_chnl = mapper_chnl_ptr->mapotn_chnl;
    }
    else 
    {
        digi_enet_client_chnl_def_t *enet_chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_ptr;
        epmm_mode = (enet_chnl_ptr->mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT 
                    || enet_chnl_ptr->mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON);
    }
    if (etrans_mode || epmm_mode)
    {
        result = digi_enet_tx_upi_get(digi_handle,chnl_ptr, data_upi_ptr, os_upi_ptr, &dummy_val);
        PMC_ASSERT(result == PMC_SUCCESS, result, 0, 0);
                
        if (*os_upi_ptr != DIGI_ENET_GSUP43_C73_OS_TYPE || *data_upi_ptr != DIGI_ENET_GSUP43_C73_PKT_TYPE)
        {
            PMC_RETURN(TRUE);
        }
    }
    PMC_RETURN(FALSE);
} /* digi_enet_gsup43_7_3_legacy_mode_chk */

#endif /*  DOXYGEN_PUBLIC_ONLY */

/*******************************************************************************
* digi_enet_tx_mac_fault_insert_ctrl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the configuration mode for Tx MAC
*   fault insertion control.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *mode_ptr              - Enum defining fault insertion mode, see
*
* RETURNS:
*   PMC_SUCCESS            - if successful otherwise meaningful error code
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_mac_fault_insert_ctrl_get(digi_handle_t *digi_handle,
                                                        util_global_switch_data_t *chnl_handle,
                                                        digi_enet_tx_mac_fault_t *mode_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    BOOL8 flt_hdl_dis = FALSE;
    BOOL8 send_rf = FALSE;
    BOOL8 send_lf = FALSE;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);


    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE)
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    }

    if(PMC_SUCCESS == rc) {
        rc = emac_fault_mode_cfg_get(enet_handle->emac_handle, link, &flt_hdl_dis, &send_lf, &send_rf);
    }

    *mode_ptr = (digi_enet_tx_mac_fault_t)((UINT32)flt_hdl_dis << DIGI_ENET_FLT_HDL_DIS_OFF |
                                           (UINT32)send_rf << DIGI_ENET_SEND_RF_OFF |
                                           (UINT32)send_lf << DIGI_ENET_SEND_LF_OFF);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_tx_mac_fault_insert_ctrl */

/*******************************************************************************
* digi_enet_idle_insert_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the XL/CGMII idle insertion of the
*   provisioned channel.  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   idle_enable            - XL/CGMII idle insertion enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES: 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_idle_insert_set(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *chnl_handle,
                                           BOOL idle_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

                rc = emac_idle_cfg(enet_handle->emac_handle, link, idle_enable);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_idle_insert_set */

/*******************************************************************************
* digi_enet_egress_squelch_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the egress squelch of the provisioned channel.
*   Only datapaths through the MAC block can support egress squelch.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - egress squelch enable (TRUE) or disable (FALSE)
*   pattern                - data pattern used in data squelching
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_egress_squelch_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL enable, 
                                              UINT16 pattern)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate = (emac_data_rate_t)0;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
            
            if (rc == PMC_SUCCESS) {

                rc = emac_egr_data_squelching_cfg(enet_handle->emac_handle, link, 
                                                  data_rate, enable, (UINT32)pattern);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_egress_squelch_set */

/*******************************************************************************
* digi_enet_ingress_data_sink_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the ingress data sink of the provisioned channel.
*   Only datapaths through the MAC block can support ingress data sink.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - flow control enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_ingress_data_sink_set(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate = (emac_data_rate_t)0;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }
    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
            if (PMC_SUCCESS == rc)
            {
                rc = emac_ingr_data_sink_cfg(enet_handle->emac_handle, link, 
                                             data_rate, enable);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_ingress_data_sink_set */


/*******************************************************************************
* digi_enet_irig_configuration_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IRIG configuration.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   ingress_enable         - ingress IRIG enable (TRUE) or disable (FALSE)
*   ingress_mode           - ingress IRIG mode
*   egress_enable          - egress IRIG enable (TRUE) or disable (FALSE)
*   egress_mode            - egress IRIG mode
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_irig_configuration_set(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  BOOL ingress_enable,
                                                  digi_enet_irig_mode_t ingress_mode,
                                                  BOOL egress_enable,
                                                  digi_enet_irig_mode_t egress_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;

    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    /* internal fuction does error checking */
    rc = digi_enet_irig_configuration_internal_set(digi_handle, 
                                                   chnl_handle, 
                                                   ingress_enable,
                                                   ingress_mode,
                                                   egress_enable,
                                                   egress_mode);

    if (rc == PMC_SUCCESS) {

        enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

        if (enet_pmon_chnl_handle != NULL) {

            rc = digi_enet_irig_configuration_internal_set(digi_handle, 
                                                           enet_pmon_chnl_handle, 
                                                           ingress_enable,
                                                           ingress_mode,
                                                           egress_enable,
                                                           egress_mode);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_irig_configuration_set */


/*******************************************************************************
* digi_enet_rx_lane_info_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves lane info of ingress direction for 40GE/100GE datapath
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   rxLaneId            - array to store ingress lane ID, the first 4 elements are
*                         valid for 40GE while the whole 20 elements are valid for 100GE.
*   rxLaneSkewFifoAddr  - array to store ingress lane skew figo addr, the first 4 elements are
*                         valid for 40GE while the whole 20 elements are valid for 100GE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*  The "TX" in code actually indicates ingress direction
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_lane_info_get(digi_handle_t               *digi_handle,
                                            util_global_switch_data_t   *chnl_handle,
                                            UINT8 rxLaneId[20], 
                                            UINT8 rxLaneSkewFifoAddr[20])
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } 
        else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc ) 
    {
        rc = enet_tx_lane_info_get (enet_handle,
                                         link,
                                         rxLaneId,
                                         rxLaneSkewFifoAddr);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}


/*******************************************************************************
* digi_enet_epmm_lf_os_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables local fault (LF) ordered sets (OS) insertion\n 
*   in ingress or egress datapath of ENET EPMM block.
*   For the RX_INGRESS direction in Gsup43 C7.3 mode, this API doesn't force LF\n
*   insertion unconditionally. Instead, it enables the hardware Rx_LOS->LF \n
*   consequential action on the ingress data stream. The LF will only be \n
*   inserted upon the Rx_LOS condition.\n\n
*
*   This function should not be used for channels operating in GSUP43 7.3 legacy
*   mode. Instead, a FW consequential action should be configured
*   (INSERT_ENET_ING_GSUP43_C73_LF_LEGACY_MODE). See the
*   digi_enet_gsup43_7_3_legacy_mode_cfg function description for details.\n\n
*
*   For Transparent GE applications, the EGRESS direction will make proxy
*   call to FW operation to ensure no register space contention with 
*   consequential action operations.  \n\n
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which LF OS are inserted or disabled.
*    enable           - when TRUE LF OS are inserted, otherwise there are disaled.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_insert_cfg(digi_handle_t               *digi_handle,
                                                 util_global_switch_data_t   *chnl_handle,
                                                 enet_dir_t                   direction,
                                                 BOOL8                        enable)
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } 
        else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc && direction == ENET_INGRESS) 
    {
        UINT8 data_upi, os_upi;
        BOOL legacy_mode;

        legacy_mode = digi_enet_gsup43_7_3_legacy_mode_chk(digi_handle, chnl_handle, &data_upi, &os_upi);
        if (legacy_mode)
        {
            rc = DIGI_ERR_EPMM_INVALID_OP_LEGACY_MODE;
        }

        if (PMC_SUCCESS == rc ) 
        {
            rc = enet_epmm_lf_cfg (enet_handle,
                                   link,
                                   direction,
                                   enable);
        }
    }

    if (PMC_SUCCESS == rc && direction == ENET_EGRESS) 
    {
        if ( TRUE == 
             ((util_global_switch_data_def_t *)chnl_handle)->cpb_data.op_state.map_state.is_output_mapped)
        {
            digi_cmf_sw_control_action_t trigger = DIGI_SW_START_FORCING_ACTION;
            if (FALSE == enable)
            {
                BOOL8 force_enable = FALSE;

                rc = enet_epmm_lf_is_force_enable(enet_handle,
                                                  link,
                                                  ENET_EGRESS,
                                                  &force_enable);
                /*
                 * Check that force_enable has been enabled if this is a
                 * disable function otherwise return as there is nothing to
                 * do.
                 */
                if (PMC_SUCCESS != rc || FALSE == force_enable)
                {
                    PMC_ATOMIC_RETURN(digi_handle, rc);
                }
                trigger = DIGI_SW_RELEASE_ACTION_CONTROL;
            }
            /*
             * Set the OS_FORCE_EN to the value of the enable flag. In the
             * case of TRUE this may result in it being set twice. For
             * enable is FALSE unset the epmm values to disable in SW and
             * then call the FW to either disable or enable if it is 
             * driving.
             */
            rc = enet_epmm_lf_set_force_enable(enet_handle, link, 
                                               direction, enable);

            if (PMC_SUCCESS == rc && FALSE == enable)
            {
                rc = enet_epmm_lf_cfg(enet_handle, link, direction, FALSE);
            }

            /*
             * Now call the FW to set it to either enable or release the
             * LF.
             */
            if (PMC_SUCCESS == rc)
            {
                rc = digi_cmf_trigger_lf_cfg(digi_handle,
                                             chnl_handle, 
                                             trigger);
            }
        }
        else if (PMC_SUCCESS == rc)
        {
            /*
             * Channel is not mapped so use the EPMM LF settings to 
             * handle this. Once the epmm_lf completes set the OS_FORCE_EN
             * If may be the case that it is already set although it is 
             * not set for all transparent modes so ensure it gets set 
             * here.
             * NOTE: The order of calls here is the same whether enable is
             * TRUE or FALSE.  
             */
            rc = enet_epmm_lf_cfg(enet_handle, link, direction, enable);

            if (PMC_SUCCESS == rc)
            {
                rc = enet_epmm_lf_set_force_enable(enet_handle, link,
                                                   direction, enable);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}


/*******************************************************************************
* digi_enet_epmm_lf_os_insert_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the status of enables or disables local fault (LF) ordered sets (OS) insertion 
*   in ingress or egress  datapath of ENET EPMM block.\n\n
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which RF OS are inserted or disabled.
*
* OUTPUTS:
*    enable           - when TRUE RF OS are inserted, otherwise there are disaled.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_insert_cfg_get(digi_handle_t              *digi_handle,
                                                     util_global_switch_data_t  *chnl_handle,
                                                     enet_dir_t                  direction,
                                                     BOOL8                       *enable)
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc) 
    {            
        rc = enet_epmm_lf_cfg_get(enet_handle,
                                 link,
                                 direction,
                                 enable);
    }


    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_epmm_lf_os_set_auto_insert_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables automatic insertion of local fault (LF) 
*   ordered sets (OS) in ingress or egress datapath of ENET EPMM block.
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which LF OS are inserted or disabled.
*    enable           - when TRUE LF OS are automatic inserted, 
*                       otherwise there are disabled.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_set_auto_insert_enable(digi_handle_t               *digi_handle,
                                                             util_global_switch_data_t   *chnl_handle,
                                                             enet_dir_t                   direction,
                                                             BOOL8                        enable)
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } 
        else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc ) 
    {
        rc = enet_epmm_lf_set_auto_insert_enable(enet_handle,
                                                 link,
                                                 direction,
                                                 enable);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_epmm_lf_os_set_auto_insert_enable_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrives the enables or disables status of automatic insertion of local fault (LF) 
*   ordered sets (OS) in ingress or egress datapath of ENET EPMM block.
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which LF OS are inserted or disabled.
*
* OUTPUTS:
*    enable           - when TRUE LF OS are automatic inserted, 
*                       otherwise there are disabled.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_set_auto_insert_enable_get(digi_handle_t               *digi_handle,
                                                             util_global_switch_data_t   *chnl_handle,
                                                             enet_dir_t                   direction,
                                                             BOOL8                        *enable)
{
  PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
  enet_handle_t *enet_handle = NULL;
  UINT32 link;

  PMC_ATOMIC_ENTRY(digi_handle);

  /* Check arguments */
  PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
  PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
  rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                &enet_handle, &link);

  if (PMC_SUCCESS == rc)
    {
      /* check handle provisioning state */
      DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

  if (PMC_SUCCESS == rc)
    {
      if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
          rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } 
      else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
          rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

  if (PMC_SUCCESS == rc ) 
    {
      rc = enet_epmm_lf_set_auto_insert_enable_get(enet_handle,
                                               link,
                                               direction,
                                               enable);
    }

  PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_epmm_rf_os_insert_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables remote fault (RF) ordered sets (OS) insertion 
*   in ingress or egress  datapath of ENET EPMM block.\n\n
*
*   For the RX_INGRESS direction in Gsup43 C7.3 mode, this API doesn't force RF\n
*   insertion unconditionally. Instead, it enable the hardware Rx_LOS-RF \n
*   consequential action on the ingress data stream. The RF will only be \n
*   inserted upon the Rx_LOS condition.\n\n
*
*   For Transparent GE applications, the EGRESS direction will make proxy
*   call to FW operation to ensure no register space contention with 
*   consequential action operations.  \n\n
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which RF OS are inserted or disabled.
*    enable           - when TRUE RF OS are inserted, otherwise there are disaled.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_rf_os_insert_cfg(digi_handle_t              *digi_handle,
                                                 util_global_switch_data_t  *chnl_handle,
                                                 enet_dir_t                  direction,
                                                 BOOL8                       enable)
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc && direction == ENET_INGRESS) 
    {            
        rc = enet_epmm_rf_cfg   (enet_handle,
                                 link,
                                 direction,
                                 enable);
    }

    if (PMC_SUCCESS == rc && direction == ENET_EGRESS) 
    {
        digi_cmf_sw_control_action_t trigger = enable==TRUE ? DIGI_SW_START_FORCING_ACTION:DIGI_SW_RELEASE_ACTION_CONTROL;
        rc = digi_cmf_trigger_rf_cfg(digi_handle,
                                     chnl_handle, 
                                     trigger);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}


/*******************************************************************************
* digi_enet_epmm_rf_os_insert_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves the status of enables or disables local fault (RF) ordered sets (OS) insertion 
*   in ingress or egress  datapath of ENET EPMM block.\n\n
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which RF OS are inserted or disabled.
*
* OUTPUTS:
*    enable           - when TRUE RF OS are inserted, otherwise there are disaled.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_rf_os_insert_cfg_get(digi_handle_t              *digi_handle,
                                                     util_global_switch_data_t  *chnl_handle,
                                                     enet_dir_t                  direction,
                                                     BOOL8                       *enable)
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc) 
    {            
        rc = enet_epmm_rf_cfg_get(enet_handle,
                                 link,
                                 direction,
                                 enable);
    }


    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_epmm_rf_os_set_auto_insert_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables automatic insertion of remote fault (RF) 
*   ordered sets (OS) in ingress or egress  datapath of ENET EPMM block.
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   *chnl_handle      - pointer to Enet or mapper channel handle instance
*    direction        - datapath direction in which RF OS are inserted or disabled.
*    enable           - when TRUE RF OS are automatic inserted, 
*                       otherwise there are disabled.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_epmm_rf_os_set_auto_insert_enable(digi_handle_t              *digi_handle,
                                                             util_global_switch_data_t  *chnl_handle,
                                                             enet_dir_t                  direction,
                                                             BOOL8                       enable)
{
    PMC_ERROR rc = DIGI_ERR_NO_SW_CONTENT;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        if (digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) == PMC_SUCCESS)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        } else if (digi_enet_is_link_transparent(enet_handle,link) == FALSE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_TRANSPARENT;
        }
    }

    if (PMC_SUCCESS == rc) 
    {            
        rc = enet_epmm_rf_set_auto_insert_enable (enet_handle,
                                                  link,
                                                  direction,
                                                  enable);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_enet_cpu_insert_pkt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets insertion from CPU
*   to the ENET LINE or ENET_SYS output interface..  
*
*   ENET ss is selected according to DIGI device type:
*   ENET SYS when it is a transponder, otherwise ENET LINE is selected.
*   
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS 
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enable                  - TRUE to enable packets insertion
*                             FALSE to disable packets insertion. 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_cpu_insert_pkt_cfg (digi_handle_t             *digi_handle,
                                               enet_dir_t                enet_datapath,
                                               util_global_switch_data_t *chnl_handle,
                                               BOOL8                     enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    cpb_mcpb_egr_output_port_type_t mcpb_egr_port = LAST_CPB_MCPB_EGRESS_PORT;
    enet_handle_t *enet_handle = NULL;
    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    UINT32 link;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT((enet_datapath == ENET_EGRESS||enet_datapath == ENET_INGRESS), DIGI_ERR_ENET_INVALID_DATAPATH, 0, 0);
    /* Get ENET handle and ENET link */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }
    
    if (PMC_SUCCESS == rc)
    {
        switch(enet_handle->var.ch_datapath[link]) 
        {
            case ENET_SERDES_100GE_MAC:
            case ENET_SLAVE_100GE_MAC_TX_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
            case ENET_MAPOTN_GMP_G709_100GE_MAC:
            case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
            case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                rate_data = DIGI_CPB_STREAM_100G;
                break;

            case ENET_SERDES_40GE_MAC:
            case ENET_SLAVE_40GE_MAC_TX_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
            case ENET_MAPOTN_GMP_G709_40GE_MAC:
            case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
            case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                rate_data = DIGI_CPB_STREAM_40G;
                break;

            case ENET_SERDES_10GE_MAC:
            case ENET_SLAVE_10GE_MAC_TX_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
            case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
            case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
            case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
            case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                rate_data = DIGI_CPB_STREAM_10G;
                break;

            default: 
                PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG,0 ,0);
                break;
        }
    }

    if (PMC_SUCCESS == rc) {

        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);

        switch (chnl_ptr->switch_data.cpb_data.port_type) {
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            {
                if (ENET_EGRESS== enet_datapath) {
                    mcpb_egr_port = CPB_MCPB_EGR_LINE_EG;
                } else if (ENET_INGRESS== enet_datapath) {
                    mcpb_egr_port = CPB_MCPB_EGR_LINE_ING;
                }
                break;
            }
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS: 
            {
                if (ENET_EGRESS== enet_datapath) {
                    mcpb_egr_port = CPB_MCPB_EGR_SYS_EG;
                } else if (ENET_INGRESS== enet_datapath) {
                    mcpb_egr_port = CPB_MCPB_EGR_SYS_ING;
                }
                break;
            }
            default: 
            {
                rc = DIGI_ERR_MCPB_INVALID_PORT;
                break;
            }
        }
    }
    /* Configure MCPB to insert packets */
    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_insert_chnl_cfg (digi_handle,
                                        link,
                                        mcpb_egr_port,
                                        CPB_MCPB_ING_CPU_INS, 
                                        enable,
                                        rate_data);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_cpu_insert_pkt_cfg */


/*******************************************************************************
* digi_enet_cpu_extract_pkt_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the packet extraction from ENET LINE or ENET SYS
*   input interface to CPU.
*
*   ENET ss is selected according to DIGI device type: ENET SYS when it is a transponder
*   otherwise ENET LINE is selected.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   enet_datapath        - ENET datapath: ENET_INGRESS or ENET_EGRESS 
*   *chnl_handle         - pointer to Enet or mapper channel handle instance
*   enable               - TRUE to enable a forwarding path
*                          FALSE to disable a forwarding path. 
*   is_reflect           - TRUE: to extract packets using MCPB reflect queue
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*  ENET ECLASS is configured to extract packets to CPU using
*  Timing and Frequency Synchronization API (see digi_timesync_api.h)
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_cpu_extract_pkt_cfg (digi_handle_t             *digi_handle,
                                                enet_dir_t                 enet_datapath,
                                                util_global_switch_data_t *chnl_handle,
                                                BOOL8                      enable,
                                                BOOL8                      is_reflect)
{
    PMC_ERROR rc = PMC_SUCCESS;
    cpb_mcpb_ingr_input_port_type_t mcpb_ing_port = LAST_CPB_MCPB_INGRESS_PORT;
    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    digi_cpb_stream_t rate_data = LAST_DIGI_CPB_STREAM;
    etime_data_rate_t etime_data_rate;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT((enet_datapath == ENET_EGRESS||enet_datapath == ENET_INGRESS), DIGI_ERR_ENET_INVALID_DATAPATH, 0, 0);
    PMC_ASSERT(!(enet_datapath != ENET_EGRESS && is_reflect), DIGI_ERR_ENET_INVALID_DATAPATH, 0, 0);

    /* Get ENET handle and ENET link */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }
    /* identify ETIME and MCPB data rates based on ENET_LINE major processing mode */
    if (PMC_SUCCESS == rc)
    {
        switch(enet_handle->var.ch_datapath[link]) 
        {
            case ENET_SERDES_100GE_MAC:
            case ENET_SLAVE_100GE_MAC_TX_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
            case ENET_MAPOTN_GMP_G709_100GE_MAC:
            case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
            case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                etime_data_rate = ETIME_LINK_100G;
                rate_data = DIGI_CPB_STREAM_100G;
                break;

            case ENET_SERDES_40GE_MAC:
            case ENET_SLAVE_40GE_MAC_TX_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
            case ENET_MAPOTN_GMP_G709_40GE_MAC:
            case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
            case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                etime_data_rate = ETIME_LINK_40G;
                rate_data = DIGI_CPB_STREAM_40G;
                break;

            case ENET_SERDES_10GE_MAC:
            case ENET_SLAVE_10GE_MAC_TX_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
            case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
            case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
            case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
            case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                etime_data_rate = ETIME_LINK_10G;
                rate_data = DIGI_CPB_STREAM_10G;
                break;

            default: 
                PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG,0 ,0);
                break;
        }
    }

    /* Configure the ETIME reflection path scheduler this is only applicable for the EGRESS path */
    if (PMC_SUCCESS == rc && is_reflect && enet_datapath == ENET_EGRESS) 
    {
        /* ETIME_REFL_DST and ETIME_REFL_DROP will be set by FW as part of the PTP feature */
        rc = etime_refl_cfg(enet_handle->emac_handle->etime_handle, link, etime_data_rate, 
                                ETIME_REFL_DROP, ETIME_REFL_DST_CPU, FALSE);
    }

    if (PMC_SUCCESS == rc) 
    {       
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);
        /* select enet handle and MCPB ingress extract port */
        switch (chnl_ptr->switch_data.cpb_data.port_type)
        {

        case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
        {
            if (is_reflect) {
               mcpb_ing_port = CPB_MCPB_ING_LINE_REF;
            } 
            else if (ENET_EGRESS == enet_datapath) 
            {
                mcpb_ing_port = CPB_MCPB_ING_LINE_EM_EXT;
            } 
            else if (ENET_INGRESS == enet_datapath) 
            {
                mcpb_ing_port = CPB_MCPB_ING_LINE_IM_EXT;
            }
            break;
        }
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
        {
            if (is_reflect) {
                mcpb_ing_port = CPB_MCPB_ING_SYS_REF;
            } 
            else if (ENET_EGRESS == enet_datapath) 
            {
                mcpb_ing_port = CPB_MCPB_ING_SYS_EM_EXT;
            } 
            else if (ENET_INGRESS == enet_datapath) 
            {
                mcpb_ing_port = CPB_MCPB_ING_SYS_IM_EXT;
            }
            break;
        }
        default:
        {
            rc = DIGI_ERR_MCPB_INVALID_PORT;
            break;
        }
        }
    }

    if (PMC_SUCCESS == rc)
    {
        /* Configure MCPB to extract packets */
        rc = digi_enet_extract_chnl_cfg (digi_handle,
                                         link,
                                         CPB_MCPB_EGR_CPU,
                                         mcpb_ing_port,
                                         enable,
                                         rate_data);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_cpu_extract_pkt_cfg */

/*******************************************************************************
* digi_enet_mgmt_fege_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function brings up ENET FEGE block and its associated serdes.
*   DIGI device should be initialized prior to this function call.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_init(digi_handle_t *digi_handle)
{
    PMC_ERROR rc = PMC_SUCCESS; 

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that DIGI device is initialized */
    if (FALSE == digi_device_init_status_get(digi_handle))
    {
        rc = DIGI_ERR_DEVICE_NOT_INIT;
    }
    
    /* check if mgmt fege is not already initialized */
    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg)
    {
        rc = DIGI_ERR_MGMT_FEGE_ALREADY_INIT;
    }

    if (PMC_SUCCESS == rc)
    {
        /* put block out of reset mode */
        digi_m1_field_FEGE_SW_RST_set(NULL, digi_handle, 0);

        /* determine state of the CPB SS and if in lowpwr/reset state transition */
        /* to a configurable state */
        digi_mcpb_energy_state_set(digi_handle, PMC_ENERGY_STATE_REQUEST_RUNNING);
        
        /* Initialize D8 serdes instance */
        rc = digi_serdes_ge_port_internal_prov(digi_handle, DIGI_SERDES_INTF_EMGMT);
    }

    if (PMC_SUCCESS == rc)
    {
        /* initialize MGMT FEGE interface */
        rc = mgmt_fege_init(digi_handle->mgmt_fege_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set      (NULL, digi_handle, TRUE);    
        tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set    (NULL, digi_handle, TRUE);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_enet_mgmt_fege_init*/

/*******************************************************************************
* digi_enet_mgmt_fege_uninit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function brings down ENET FEGE block and its associated serdes.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_uninit(digi_handle_t *digi_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that DIGI device is initialized */
    if (FALSE == digi_device_init_status_get(digi_handle))
    {
        rc = DIGI_ERR_DEVICE_NOT_INIT;
    }
    
    if (PMC_SUCCESS == rc)
    {
        /* return error code if there are some MCPB channels still provisioned */
        if (FALSE == mgmt_fege_pkt_all_dpi_deprov_get(digi_handle->mgmt_fege_handle))
        {
            rc = DIGI_ERR_MCPB_CHNL_STILL_PROV;
        }
    }

    if (PMC_SUCCESS == rc)
    {
        /* uninit MGMT_FEGE GE serdes */
        rc = digi_serdes_ge_port_internal_deprov(digi_handle, DIGI_SERDES_INTF_EMGMT);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = mgmt_fege_uninit(digi_handle->mgmt_fege_handle, FALSE);
    }

    if (PMC_SUCCESS == rc)
    {
        tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set      (NULL, digi_handle, FALSE);    
        tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set    (NULL, digi_handle, FALSE);
    }

    /* determine state of the CPB SS and if no resources provisioned bring SS to lowpwr/reset state */
    if(PMC_SUCCESS == rc)
    {
        digi_mcpb_energy_state_set(digi_handle, PMC_ENERGY_STATE_REQUEST_OPTIMAL);
    }


    if (PMC_SUCCESS == rc)
    {
        /* put block in reset mode */
        digi_m1_field_FEGE_SW_RST_set(NULL, digi_handle, 1);
        digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg = FALSE;
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
}/*digi_enet_mgmt_fege_uninit*/

/*******************************************************************************
* digi_enet_mgmt_fege_init_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function gets ENET FEGE block status.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   enable          - return TRUE if FEGE inited or FALSE if FEGE uninited.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_init_get(digi_handle_t *digi_handle,
                                              BOOL8 *enable)
{
    PMC_ERROR rc = PMC_SUCCESS; 

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that DIGI device is initialized */
    if (FALSE == digi_device_init_status_get(digi_handle))
    {
        rc = DIGI_ERR_DEVICE_NOT_INIT;
    }

    rc = digi_serdes_ge_port_internal_prov_get(digi_handle, DIGI_SERDES_INTF_EMGMT, enable);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /*digi_enet_mgmt_fege_init_get*/

/*******************************************************************************
* digi_mgmt_fege_to_enet_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   input interface to ENET LINE or ENET_SYS output interface. Packets are
*   filtered in by their VLAN ID.  
*
*   ENET ss is selected according to DIGI device type: ENET SYS when it is a transponder
*   otherwise ENET LINE is selected.
*   
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   enable                  - TRUE to enable a forwarding path
*                             FALSE to disable a forwarding path. 
*   overwrite_vlan_priority - when TRUE priority bit in VLAN TCI is overwriten by 
*                             zeroes.
* 
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mgmt_fege_to_enet_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    util_global_vlan_cfg_t        *vlan_cfg_ptr,
                                                    BOOL8                          enable,
                                                    BOOL8                          overwrite_vlan_priority)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    cpb_mcpb_egr_output_port_type_t    mcpb_egr_port;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;
    UINT32 num_slots;

    util_global_switch_data_def_t *chnl_ptr = (util_global_switch_data_def_t *)chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

             
    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);
    }
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc && FALSE == digi_enet_is_link_mac_block(enet_handle, link)) 
    {
        rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    }

    if (PMC_SUCCESS == rc)
    {

        rc = digi_cpb_switch_data_slot_get(digi_handle, (util_global_switch_data_def_t *)chnl_handle, &num_slots);

        switch(num_slots)
        {
        case 8:
        {
            /* 10G = 8 entries */
            rate_data = DIGI_CPB_STREAM_10G;
            break;
        }
        case 32:
        {
            /* 40G = 32 entries */
            rate_data = DIGI_CPB_STREAM_40G;
            break;
        }
        case 80:
        {
            /* 100G = 80 entries */
            rate_data = DIGI_CPB_STREAM_100G;
            break;
        } default:
        {
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT,0 ,0);
            break;
        }
        }
    }

    if (PMC_SUCCESS == rc)
    {
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);

        switch (enet_datapath)
        {
        case ENET_INGRESS:
            switch(chnl_ptr->cpb_data.port_type)
            {
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                mcpb_egr_port = CPB_MCPB_EGR_LINE_ING;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                mcpb_egr_port = CPB_MCPB_EGR_SYS_ING;
                break;
            default:
                rc = DIGI_ERR_MCPB_INVALID_PORT;
                break;
            }
            break;
        case ENET_EGRESS:
            switch(chnl_ptr->cpb_data.port_type)
            {
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                mcpb_egr_port = CPB_MCPB_EGR_LINE_EG;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                mcpb_egr_port = CPB_MCPB_EGR_SYS_EG;
                break;
            default:
                rc = DIGI_ERR_MCPB_INVALID_PORT;
                break;
            }
            break;
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
    }

    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_mgmt_fege_ins_chnl_cfg(digi_handle, link,
                                              mcpb_egr_port,
                                              vlan_cfg_ptr, enable,
                                              overwrite_vlan_priority,
                                              rate_data,
                                              TRUE); /* This must be set to true when sending packets to ENET ss so that the FCS is appended*/
    }
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_mgmt_fege_to_enet_pkt_fwd_cfg */

/*******************************************************************************
* digi_enet_cpu_insert_pkt_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves configuration information applied by the
*   digi_enet_cpu_insert_pkt_cfg() API.
*   
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS 
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                - TRUE: MCPB path configured for channel, 
*                          FALSE: MCPB path not configured for channel
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC BOOL8 digi_enet_cpu_insert_pkt_cfg_get(digi_handle_t             *digi_handle,
                                              enet_dir_t                enet_datapath,
                                              util_global_switch_data_t *chnl_handle)
{
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    result = digi_enet_channel_mcpb_clean_filtered_test(digi_handle,
                                                        chnl_handle,
                                                        (enet_datapath == ENET_INGRESS ? 0x1:0x2),
                                                        0x0,
                                                        0x1);

    PMC_ASSERT(result == PMC_SUCCESS || result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ATOMIC_RETURN(digi_handle, (result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED ? TRUE:FALSE));

} /* digi_enet_cpu_insert_pkt_cfg_get */


/*******************************************************************************
* digi_enet_cpu_extract_pkt_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves configuration information applied by the
*   digi_enet_cpu_extract_pkt_cfg() API.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
*   is_reflect              - TRUE: extract packets using MCPB reflect queue
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                - TRUE: MCPB path configured for channel, 
*                          FALSE: MCPB path not configured for channel
*
* NOTES:
*   
*******************************************************************************/
PUBLIC BOOL8 digi_enet_cpu_extract_pkt_cfg_get(digi_handle_t             *digi_handle,
                                               enet_dir_t                 enet_datapath,
                                               util_global_switch_data_t *chnl_handle,
                                               BOOL8                      is_reflect)
{
    UINT32 target_mask = 0;
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    if(enet_datapath == ENET_EGRESS)
    {   
        target_mask = 0x1;
    }
    else
    {
        target_mask = 0x2;
    }

    if(is_reflect == TRUE)
        target_mask = 0x4;


    result = digi_enet_channel_mcpb_clean_filtered_test(digi_handle,
                                                        chnl_handle,
                                                        0x4, /* DCS2 */
                                                        target_mask, /* target_mask */
                                                        0x0);

    PMC_ASSERT(result == PMC_SUCCESS || result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ATOMIC_RETURN(digi_handle, (result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED ? TRUE:FALSE));
} /* digi_enet_cpu_extract_pkt_cfg_get */

/*******************************************************************************
* digi_cpu_to_mgmt_fege_pkt_fwd_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves configuration information applied by the
*   digi_cpu_to_mgmt_fege_pkt_fwd_cfg() API.\n\n
*
*   The output values are only valid if function returns TRUE.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   *vlan_src_cfg_ptr    - VLAN-tagged packet configuration. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*
* OUTPUTS:
*   *vlan_id_repl_en_ptr - enable packet VLAN ID replacement.
*   *vlan_id_repl_ptr    - VLAN ID value replaced in forwarded packet. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*
* RETURNS:
*   BOOL8                - TRUE: MCPB path configured for channel, 
*                          FALSE: MCPB path not configured for channel
*
* NOTES:
*   
*******************************************************************************/
PUBLIC BOOL8 digi_cpu_to_mgmt_fege_pkt_fwd_cfg_get(digi_handle_t                 *digi_handle,
                                                   util_global_vlan_cfg_t        *vlan_src_cfg_ptr,
                                                   BOOL8                         *vlan_id_repl_en_ptr,
                                                   UINT16                        *vlan_id_repl_ptr)
{
    mgmt_fege_fwd_cfg_t fwd_cfg;
    BOOL8 mcpb_start = FALSE;
    BOOL8 mgmt_fege_enable = FALSE;
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif



    mcpb_start = cpb_mcpb_cpu_mgmt_start_state_test(digi_handle->mcpb_handle);


    mgmt_fege_ingr_pkt_fwd_cfg_get(digi_handle->mgmt_fege_handle, 
                                   0x3,
                                   &mgmt_fege_enable, 
                                   &fwd_cfg);
    PMC_MEMCPY(vlan_src_cfg_ptr,
               &(fwd_cfg.vlan_entry),
               sizeof(util_global_vlan_cfg_t));    

    *vlan_id_repl_en_ptr = fwd_cfg.ins_vlan_id;
    *vlan_id_repl_ptr = fwd_cfg.vlan_entry.vlan_id;

    PMC_ASSERT(mcpb_start!=mgmt_fege_enable, DIGI_ERR_CODE_ASSERT, 0, 0);
    
    PMC_ATOMIC_RETURN(digi_handle, mgmt_fege_enable);
} /* digi_cpu_to_mgmt_fege_pkt_fwd_cfg_get */


/*******************************************************************************
* digi_mgmt_fege_to_enet_pkt_fwd_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves configuration information applied by the
*   digi_mgmt_fege_to_enet_pkt_fwd_cfg() API.\n\n
*
*   The output values are only valid if function returns TRUE.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
* 
* OUTPUTS:
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   *overwrite_vlan_priority_ptr - when TRUE priority bit in VLAN TCI is  
*                                  overwriten by zeroes.
*
* RETURNS:
*   BOOL8                - TRUE: MCPB path configured for channel, 
*                          FALSE: MCPB path not configured for channel
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC BOOL8 digi_mgmt_fege_to_enet_pkt_fwd_cfg_get(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    util_global_vlan_cfg_t        *vlan_cfg_ptr,
                                                    BOOL8                         *overwrite_vlan_priority_ptr)
{
    UINT16 dpi_id;
    cpb_mcpb_egr_output_port_type_t  mcpb_egr_port = LAST_CPB_MCPB_EGRESS_PORT;
    BOOL8 enable = FALSE;
    mgmt_fege_fwd_cfg_t  fwd_cfg;    
    BOOL8 fcs_calc_en = FALSE;
    BOOL8 priority_en = FALSE;
    UINT32 link = 0;
    enet_handle_t *enet_handle = NULL;
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    result = digi_enet_channel_mcpb_clean_filtered_test(digi_handle,
                                                        chnl_handle,
                                                        (enet_datapath == ENET_INGRESS ? 0x1:0x2),
                                                        0x0,
                                                        0x2);
    if(result == DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET) PMC_ATOMIC_RETURN(digi_handle, FALSE);

    (void)digi_enet_validate_parms(digi_handle, chnl_handle,
                                   &enet_handle, &link);

    if(enet_handle == digi_handle->enet_line_handle)
    {
        mcpb_egr_port = (enet_datapath == ENET_INGRESS ? CPB_MCPB_EGR_LINE_ING:CPB_MCPB_EGR_LINE_EG);
    }
    else
    {
        mcpb_egr_port = (enet_datapath == ENET_INGRESS ? CPB_MCPB_EGR_SYS_ING:CPB_MCPB_EGR_SYS_EG);
    }

    switch (mcpb_egr_port)
    {
        case CPB_MCPB_EGR_LINE_ING:
            dpi_id = (link * 2);
            break;
        case CPB_MCPB_EGR_SYS_ING:
            dpi_id = (link * 2);
            break;
        case CPB_MCPB_EGR_LINE_EG:
            dpi_id = (link * 2) + 1;
            break;
        case CPB_MCPB_EGR_SYS_EG:
            dpi_id = (link * 2) + 1;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
    }

    mgmt_fege_egr_pkt_fwd_cfg_get(digi_handle->mgmt_fege_handle,
                                  dpi_id,
                                  &enable,
                                  &fwd_cfg,
                                  &fcs_calc_en,
                                  &priority_en);

    *overwrite_vlan_priority_ptr = (priority_en == TRUE ? FALSE:TRUE);

    PMC_MEMCPY(vlan_cfg_ptr,
               &(fwd_cfg.vlan_entry),
               sizeof(util_global_vlan_cfg_t));    

    PMC_ASSERT(result == PMC_SUCCESS || result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED, DIGI_ERR_CODE_ASSERT, 0, 0);
    /* perform two tests to ensure MGMT_FEGE and MCPB configuration are coherent */
    if(enable == TRUE)  PMC_ASSERT(result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED, DIGI_ERR_CODE_ASSERT, 0, 0);
    if(enable == FALSE) PMC_ASSERT(result == PMC_SUCCESS, DIGI_ERR_CODE_ASSERT, 0, 0);
    PMC_ATOMIC_RETURN(digi_handle, (result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED ? TRUE:FALSE));
} /* digi_mgmt_fege_to_enet_pkt_fwd_cfg_get */


/*******************************************************************************
* digi_mgmt_fege_to_enet_pkt_fwd_vlan_pri_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves vlan_cfg_ptr and overwrite_vlan_priority_ptr
*   information applied by the digi_mgmt_fege_to_enet_pkt_fwd_cfg() API.\n\n
*
*   The output values are only valid if function returns TRUE.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
* 
* OUTPUTS:
*   *enable                 - TRUE: MCPB path configured for channel, 
*                             FALSE: MCPB path not configured for channel
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   *overwrite_vlan_priority_ptr - when TRUE priority bit in VLAN TCI is  
*                                  overwriten by zeroes.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mgmt_fege_to_enet_pkt_fwd_vlan_pri_cfg_get(digi_handle_t             *digi_handle,
                                                                 util_global_switch_data_t *chnl_handle,
                                                                 enet_dir_t                 enet_datapath,
                                                                 BOOL8                     *enable,
                                                                 util_global_vlan_cfg_t    *vlan_cfg_ptr,
                                                                 BOOL8                     *overwrite_vlan_priority_ptr)
{
    UINT16 dpi_id;
    cpb_mcpb_egr_output_port_type_t  mcpb_egr_port = LAST_CPB_MCPB_EGRESS_PORT;
    BOOL8 priority_en = FALSE;
    UINT32 link = 0;
    enet_handle_t *enet_handle = NULL;
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    (void)digi_enet_validate_parms(digi_handle, chnl_handle,
                                   &enet_handle, &link);

    if(enet_handle == digi_handle->enet_line_handle)
    {
        mcpb_egr_port = (enet_datapath == ENET_INGRESS ? CPB_MCPB_EGR_LINE_ING:CPB_MCPB_EGR_LINE_EG);
    }
    else
    {
        mcpb_egr_port = (enet_datapath == ENET_INGRESS ? CPB_MCPB_EGR_SYS_ING:CPB_MCPB_EGR_SYS_EG);
    }

    switch (mcpb_egr_port)
    {
        case CPB_MCPB_EGR_LINE_ING:
            dpi_id = (link * 2);
            break;
        case CPB_MCPB_EGR_SYS_ING:
            dpi_id = (link * 2);
            break;
        case CPB_MCPB_EGR_LINE_EG:
            dpi_id = (link * 2) + 1;
            break;
        case CPB_MCPB_EGR_SYS_EG:
            dpi_id = (link * 2) + 1;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
    }

    result = digi_enet_mgmt_fege_ins_chnl_cfg_get(digi_handle, link,
                                                  mcpb_egr_port,
                                                  vlan_cfg_ptr,
                                                  enable,
                                                  &priority_en);

    if (PMC_SUCCESS == result)
    {
        *overwrite_vlan_priority_ptr = (priority_en == TRUE ? FALSE:TRUE);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mgmt_fege_to_enet_pkt_fwd_vlan_pri_cfg_get */


/*******************************************************************************
* digi_enet_to_mgmt_fege_pkt_fwd_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function retrieves configuration information applied by the
*   digi_enet_to_mgmt_fege_pkt_fwd_cfg() API.\n\n
*
*   The output values are only valid if function returns TRUE.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   *chnl_handle         - pointer to Enet or mapper channel handle instance.
*   enet_datapath        - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
*
* OUTPUTS:
*   *extract_cfg_ptr      - Pointer to array of size ECLASS_NUM_SUPPORTED_PKT_PER_LINK
*                           updated with each unique extraction packet type's
*                           data.
*   *vlan_id_repl_en_ptr  - enable packet VLAN ID replacement.
*   *vlan_id_repl_ptr     - VLAN ID value replaced in forwarded packet. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*
* RETURNS:
*   BOOL8                - TRUE: MCPB path configured for channel, 
*                          FALSE: MCPB path not configured for channel
*
* NOTES:
*   
*******************************************************************************/
PUBLIC BOOL8 digi_enet_to_mgmt_fege_pkt_fwd_cfg_get(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    digi_enet_to_mgmt_fege_extract_cfg_t  *extract_cfg_ptr,
                                                    BOOL8                         *vlan_id_repl_en_ptr,
                                                    UINT16                        *vlan_id_repl_ptr)
{
    UINT32 link = 0;
    enet_handle_t *enet_handle = NULL;
    cpb_mcpb_ingr_input_port_type_t    mcpb_ing_port = LAST_CPB_MCPB_INGRESS_PORT;
    mgmt_fege_fwd_cfg_t fwd_cfg;
    BOOL8 mgmt_fege_enable = FALSE;
    UINT16 dpi_id = 0;
    UINT32 target_mask = 0;
    UINT32 i;
    UINT32   array_size = 0;
    UINT32   array_used_size = 0;
    eclass_pkt_extract_cfg_t int_extract_cfg[ECLASS_NUM_SUPPORTED_PKT_PER_LINK];
    PMC_ERROR result = PMC_SUCCESS;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    if(enet_datapath == ENET_EGRESS)
    {   
        target_mask = 0x1;
    }
    else
    {
        target_mask = 0x2;
    }

    result = digi_enet_channel_mcpb_clean_filtered_test(digi_handle,
                                                        chnl_handle,
                                                        0x8, /* DCS3 */
                                                        target_mask, /* target_mask */
                                                        0x0);

    (void)digi_enet_validate_parms(digi_handle, chnl_handle,
                                   &enet_handle, &link);

    if(enet_handle == digi_handle->enet_line_handle)
    {
        mcpb_ing_port = (enet_datapath == ENET_INGRESS ? CPB_MCPB_ING_LINE_IM_EXT:CPB_MCPB_ING_LINE_EM_EXT);
    }
    else
    {
        mcpb_ing_port = (enet_datapath == ENET_INGRESS ? CPB_MCPB_ING_SYS_IM_EXT:CPB_MCPB_ING_SYS_EM_EXT);
    }

    switch (mcpb_ing_port)
    {
        case CPB_MCPB_ING_LINE_IM_EXT:
            dpi_id = (link << 2) + 1;
            break;
        case CPB_MCPB_ING_SYS_IM_EXT:
            dpi_id = (link << 2) + 1;
            break;
        case CPB_MCPB_ING_LINE_EM_EXT:
            dpi_id = (link << 2);
            break;
        case CPB_MCPB_ING_SYS_EM_EXT:
            dpi_id = (link << 2);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }

    PMC_MEMSET(int_extract_cfg, 0, (sizeof(eclass_pkt_extract_cfg_t)*(ECLASS_NUM_SUPPORTED_PKT_PER_LINK)));

    enet_eclass_pkt_extract_cfg_get(enet_handle, 
                                    link, 
                                    enet_datapath,
                                    &array_size,
                                    &array_used_size,
                                    &int_extract_cfg[0]);

    for(i=0; i < ECLASS_NUM_SUPPORTED_PKT_PER_LINK; i++)
    {
        extract_cfg_ptr[i].valid = 0;
        extract_cfg_ptr[i].pkt_type = LAST_UTIL_GLOBAL_PKT_TYPE;
        extract_cfg_ptr[i].is_vlan_tagged = 0;

        PMC_MEMSET(&(extract_cfg_ptr[i].vlan_src_cfg),
                   0,
                   sizeof(util_global_vlan_cfg_t));
    }
    
    for(i=0; i < ECLASS_NUM_SUPPORTED_PKT_PER_LINK; i++)
    {
        extract_cfg_ptr[i].valid = int_extract_cfg[i].valid;
        extract_cfg_ptr[i].pkt_type = int_extract_cfg[i].pkt_type;
        extract_cfg_ptr[i].is_vlan_tagged = int_extract_cfg[i].is_vlan;

        if(i >= ECLASS_SUPPORTED_PKT_TYPE)
        {
            /* this is a record for a valid VLAN ID */
            PMC_MEMCPY(&(extract_cfg_ptr[i].vlan_src_cfg),
                       &(int_extract_cfg[i].vlan_entry),
                       sizeof(util_global_vlan_cfg_t));
        }
    }


    mgmt_fege_ingr_pkt_fwd_cfg_get(digi_handle->mgmt_fege_handle, 
                                   dpi_id,
                                   &mgmt_fege_enable, 
                                   &fwd_cfg);

    *vlan_id_repl_en_ptr = fwd_cfg.ins_vlan_id;
    *vlan_id_repl_ptr = fwd_cfg.vlan_entry.vlan_id;

    PMC_ASSERT(result == PMC_SUCCESS || result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED, DIGI_ERR_CODE_ASSERT, 0, 0);

    if(result == PMC_SUCCESS) PMC_ASSERT(mgmt_fege_enable == FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    if(result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED) PMC_ASSERT(mgmt_fege_enable == TRUE, DIGI_ERR_CODE_ASSERT, 0, 0);

    PMC_ATOMIC_RETURN(digi_handle, (result == DIGI_ERR_MCPB_PATH_STILL_CONNECTED ? TRUE:FALSE));
} /* digi_enet_to_mgmt_fege_pkt_fwd_cfg_get */


/*******************************************************************************
* digi_mgmt_fege_to_10g_enet_pkt_fifo_size_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the increasing of MCPB FIFO size to allow
*   the support of 9600 Byte test packets from the MGMT_FEGE to one 10GE
*   ethernet port for a traffic direction (Rx Insert or Tx Insert).\n\n
*
*   For the FIFO to be decreased in size packets must be flowing on this 
*   path.  If not the MCPB FIFO resizing mechanism will fail.\n\n
*
*   If a different channel for the direction type already has an increased
*   FIFO size, this API call will be rejected with an error.\n\n
*
*   If this API is called with the same setting a channel already has, it
*   will return PMC_SUCCESS.
*
*   This API is only applicable to 10GE serdes ports. MGMT to ENET MCPB path 
*   supports 9600 bytes with the default FIFO sizing for 40GE and 100GE serdes 
*   port rates.  If called for 40GE or 100GE rate ports, the API will silently
*   return PMC_SUCCESS.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
*   enable_9600b            - TRUE:  Set FIFO size to transfer 9600 Byte packet
*                             FALSE: Set FIFO size to transfer default packet
* 
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mgmt_fege_to_10g_enet_pkt_fifo_size_cfg(digi_handle_t *digi_handle,
                                                              util_global_switch_data_t     *chnl_handle,
                                                              enet_dir_t                     enet_datapath,
                                                              BOOL8                          enable_9600b)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link, large_link = 0, pop_port_id = 0;
    cpb_mcpb_egr_output_port_type_t    mcpb_egr_port;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;
    UINT32 num_slots, num_blocks = DIGI_ENET_MCPB_10GE_LARGE_FIFO_NUM_BLOCKS;
    BOOL8 large_enabled = FALSE;

    util_global_switch_data_def_t *chnl_ptr = (util_global_switch_data_def_t *)chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    if(FALSE == enable_9600b)
    {
        num_blocks = DIGI_ENET_MCPB_10GE_NORMAL_FIFO_NUM_BLOCKS;
    }

    
    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);
    }
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, ( (util_global_switch_data_def_t*) chnl_handle));
    }

    if (PMC_SUCCESS == rc && FALSE == digi_enet_is_link_mac_block(enet_handle, link)) 
    {
        rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_ENET_LINK_NOT_MAC );*/
    }

    if (PMC_SUCCESS == rc)
    {

        rc = digi_cpb_switch_data_slot_get(digi_handle, (util_global_switch_data_def_t *)chnl_handle, &num_slots);

        switch(num_slots)
        {
        case 8:
        {
            /* 10G = 8 entries */
            rate_data = DIGI_CPB_STREAM_10G;
            break;
        }
        case 32:
        {
            /* 40G = 32 entries */
            rate_data = DIGI_CPB_STREAM_40G;
            break;
        }
        case 80:
        {
            /* 100G = 80 entries */
            rate_data = DIGI_CPB_STREAM_100G;
            break;
        } default:
        {
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT,0 ,0);
            break;
        }
        }
    }

    if (PMC_SUCCESS == rc)
    {
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);

        switch (enet_datapath)
        {
        case ENET_INGRESS:
            pop_port_id = 0;
            switch(chnl_ptr->cpb_data.port_type)
            {
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                mcpb_egr_port = CPB_MCPB_EGR_LINE_ING;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                mcpb_egr_port = CPB_MCPB_EGR_SYS_ING;
                break;
            default:
                rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_MCPB_INVALID_PORT );*/
                break;
            }
            break;
        case ENET_EGRESS:
            pop_port_id = 1;
            switch(chnl_ptr->cpb_data.port_type)
            {
            case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                mcpb_egr_port = CPB_MCPB_EGR_LINE_EG;
                break;
            case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                mcpb_egr_port = CPB_MCPB_EGR_SYS_EG;
                break;
            default:
                rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_MCPB_INVALID_PORT );*/
                break;
            }
            break;
        default:
            rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_INVALID_ARG );*/
            break;
        }
    }

    if(PMC_SUCCESS == rc)
    {
        /* silent return if ENET resource application rate is not 10G */
        if(rate_data != DIGI_CPB_STREAM_10G)
            PMC_ATOMIC_RETURN(digi_handle,rc);
    }

    if(PMC_SUCCESS == rc)
    {
        large_enabled = digi_enet_mgmt_fege_to_enet_10g_fifo_data_get(digi_handle, 
                                                                      enet_handle,
                                                                      mcpb_egr_port, 
                                                                      &large_link);

        if(TRUE == enable_9600b && TRUE == large_enabled && large_link != link)
        {
            rc = DIGI_ERR_MCPB_10GE_LARGE_FIFO_ALLOCATED;
        }
    }

    if(PMC_SUCCESS == rc)
    {
        /* grow or shrink FIFO if large FIFO is request, or reduce is requested 
           and the requested channel has the large FIFO */
        if((TRUE == enable_9600b && TRUE != large_enabled) ||
           (FALSE == enable_9600b && TRUE == large_enabled && link == large_link))
        {
            UINT32 read_qid = link * 2 + pop_port_id;
            rc = cpb_queue_resize(digi_handle->mcpb_handle,
                                  pop_port_id,
                                  read_qid,
                                  TRUE,
                                  num_blocks);
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_mgmt_fege_to_10g_enet_pkt_fifo_size_cfg */

/*******************************************************************************
* digi_10g_enet_pkt_to_mgmt_fege_fifo_size_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the increasing of MCPB FIFO size to allow
*   the support of 9600 Byte test packets from one 10GE ethernet port to
*   MGMT_FEGE for a traffic direction (Rx Insert or Tx Insert).\n\n
*
*   For the FIFO to be decreased in size packets must be flowing on this 
*   path.  If not the MCPB FIFO resizing mechanism will fail.\n\n
*
*   If a different channel for the direction type already has an increased
*   FIFO size, this API call will be rejected with an error.\n\n
*
*   If this API is called with the same setting a channel already has, it
*   will return PMC_SUCCESS.
*
*   This API is only applicable to 10GE serdes ports. ENET MCPB to MGMT path 
*   supports 9600 bytes with the default FIFO sizing for 40GE and 100GE serdes 
*   port rates.  If called for 40GE or 100GE rate ports, the API will silently
*   return PMC_SUCCESS.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *chnl_handle            - pointer to Enet or mapper channel handle instance
*   enet_datapath           - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
*   enable_9600b            - TRUE:  Set FIFO size to transfer 9600 Byte packet
*                             FALSE: Set FIFO size to transfer default packet
* 
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_10g_enet_pkt_to_mgmt_fege_fifo_size_cfg(digi_handle_t  *digi_handle,
                                                              util_global_switch_data_t      *chnl_handle,
                                                              enet_dir_t                      enet_datapath,
                                                              BOOL8                           enable_9600b)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link, large_link = 0, push_port_id = 0;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;
    UINT32 num_slots, num_blocks = DIGI_ENET_MCPB_10GE_LARGE_FIFO_NUM_BLOCKS;
    BOOL8 large_enabled = FALSE;

    util_global_switch_data_def_t *chnl_ptr = (util_global_switch_data_def_t *)chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    if(FALSE == enable_9600b)
    {
        num_blocks = DIGI_ENET_MCPB_10GE_NORMAL_FIFO_NUM_BLOCKS;
    }


    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);
    }
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc && FALSE == digi_enet_is_link_mac_block(enet_handle, link)) 
    {
        rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_ENET_LINK_NOT_MAC )*/
    }

    if (PMC_SUCCESS == rc)
    {

        rc = digi_cpb_switch_data_slot_get(digi_handle, (util_global_switch_data_def_t *)chnl_handle, &num_slots);

        switch(num_slots)
        {
            case 8: /* 10G = 8 entries */
                rate_data = DIGI_CPB_STREAM_10G;
                break;
            case 32: /* 40G = 32 entries */
                rate_data = DIGI_CPB_STREAM_40G;
                break;
            case 80: /* 100G = 80 entries */
                rate_data = DIGI_CPB_STREAM_100G;
                break;
            default:
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT,0 ,0);
                break;
        }
    }

    if (PMC_SUCCESS == rc)
    {
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);

        switch (enet_datapath)
        {
            case ENET_INGRESS:
                push_port_id = 0;
                switch(chnl_ptr->cpb_data.port_type)
                {
                    case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                        break;
                    case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                        break;
                    default:
                        rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_MCPB_INVALID_PORT )*/
                        break;
                }
                break;
            case ENET_EGRESS:
                push_port_id = 1;
                switch(chnl_ptr->cpb_data.port_type)
                {
                    case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                        break;
                    case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                        break;
                    default:
                        rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_MCPB_INVALID_PORT )*/
                        break;
                }
                break;
            default:
                rc = PMC_ERR_FAIL; /*PMC_RUNTIME_ERROR( DIGI_ERR_INVALID_ARG )*/
                break;
        }
    }

    if(PMC_SUCCESS == rc)
    {
        /* silent return if ENET resource application rate is not 10G */
        if(rate_data != DIGI_CPB_STREAM_10G)
            PMC_ATOMIC_RETURN(digi_handle,rc);
    }

    if(PMC_SUCCESS == rc)
    {
        large_enabled = digi_enet_10g_to_mgmt_fege_fifo_data_get(digi_handle,
                                                                 enet_handle,
                                                                 push_port_id,
                                                                 &large_link);


        if(TRUE == enable_9600b && TRUE == large_enabled && large_link != link)
        {
            rc = DIGI_ERR_MCPB_10GE_LARGE_FIFO_ALLOCATED;
        }
    }

    if(PMC_SUCCESS == rc)
    {
        /* grow or shrink FIFO if large FIFO is request, or reduce is requested 
           and the requested channel has the large FIFO */
        if((TRUE == enable_9600b && TRUE != large_enabled) ||
           (FALSE == enable_9600b && TRUE == large_enabled && link == large_link))
        {
            UINT32 read_qid = link + 12*push_port_id;
            rc = cpb_queue_resize(digi_handle->mcpb_handle,
                                  2,
                                  read_qid,
                                  TRUE,
                                  num_blocks);
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_10g_enet_pkt_to_mgmt_fege_fifo_size_cfg */

/*******************************************************************************
* digi_mgmt_fege_rx_flow_control_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure DIGI MGMT_FEGE RX flow control. When Control flow is enabled, MGMT_FEGE 
*   stops frames transmission when a PAUSE frame is received and it resumes the
*   transmission when pause quanta counter expired.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   enable                   - when TRUE, flow control is activated
*                              otherwise it is deactivated
*   drop_pause_frm           - when TRUE, RX pause frames are terminated in MGMT_FEGE
*                              when FALSE, RX pause frames are forwarded.  
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None..
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_mgmt_fege_rx_flow_control_set(digi_handle_t          *digi_handle,
                                               BOOL8                   enable,
                                               BOOL8                   drop_pause_frm)
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    mgmt_fege_rx_flow_control_set(digi_handle->mgmt_fege_handle, enable, drop_pause_frm);

    PMC_ATOMIC_RETURN(digi_handle);
}/* digi_mgmt_fege_rx_flow_control_get */

/*******************************************************************************
* digi_enet_inbound_fault_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs configuration of a CBRC resource and DCPB datapath
*   so that the CBRC resource may be used as a source of nominal rate
*   client replacement signal for a 10GE CBR application in an ENET SS to 
*   MAPOTN data path.\n\n
*
*   The replacement signal is configured for LF.\n\n
*
*   The ENET to MAPOTN datapath must be mapped and activated.\n\n
*
*   Prior to deactivating the datapath, this feature must be deprovisioned,
*   otherwise an error code is returned.  Deprovisioning will return an
*   error if the inbound fault replacement signal is active.  The forcing
*   of inbound fault signal must be turned off prior to deactivating the
*   channel.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE or ENET_SYS channel handle
*                              instance
*   prov                     - API action, TRUE: provision, FALSE: deprovision
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR                - PMC_SUCCESS upon success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_inbound_fault_prov(digi_handle_t          *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL8 prov)
{

    enet_handle_t *enet_ss_handle_ptr;
    digi_enet_mode_t enet_mode = LAST_DIGI_ENET;
    util_global_cpb_connect_t *conn_ptr = NULL;
    UINT32 link, mapotn_chnl = ~0;
    util_global_cpb_port_t slv_port = LAST_UTIL_GLOBAL_CPB_PORT;
    digi_serdes_intf_t intf_type = LAST_DIGI_SERDES_INTF;

    UINT32 dsi_base_lane, port_uid;
    UINT32 ingress_mux_output_port = 4, egress_mux_input_port = 4; /* initialize to illegal values */
    UINT32 num_push_ports = 1, num_pop_ports = 1; /* number of push/pop port choices*/

    UINT32                      cbrc_link = 0; /* Post MLD link (0..11) */
    cbrc_serdes_type_t          cbrc_serdes = CBRC_SERDES_TYPE_NONE;
    UINT32                      mux_link = 0;  /* Post Mux link */
    digi_group_size_t           group_size = LAST_DIGI_GRP_SIZE;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* confirm ENET_LINE or ENET_SYS channel handle */
    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);
    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
        dsi_base_lane = ((digi_enet_client_chnl_def_t*)chnl_handle)->dsi_base_lane;
        enet_mode = ((digi_enet_client_chnl_def_t*)chnl_handle)->mode;
    }

    PMC_ASSERT(enet_mode == DIGI_ENET_SERDES_10GE_TRANSPARENT || enet_mode == DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON, DIGI_ERR_CODE_ASSERT, 0, 0);

    if(result == PMC_SUCCESS && enet_ss_handle_ptr == digi_handle->enet_line_handle)
    {
        PMC_ERROR temp_result = PMC_SUCCESS;
        slv_port = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
        port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[dsi_base_lane].port_uid;
        intf_type = DIGI_SERDES_LINE_INTF;
        temp_result = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);
        if(temp_result == PMC_SUCCESS) result = DIGI_ERR_INVALID_ARG;
        if(result == PMC_SUCCESS && digi_handle->var.prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) result = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        slv_port = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
        port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[dsi_base_lane].port_uid;
        intf_type = DIGI_SERDES_SYSTEM_INTF;
    }


    /* confirm MAPOTN destination for the channel */
    if(result == PMC_SUCCESS)
    {
        cpb_connect_data_state_get(digi_handle->dcpb_handle, &conn_ptr);
        mapotn_chnl = conn_ptr->chnl[slv_port][link].slv.dest_mst_chnl;
        if(conn_ptr->chnl[slv_port][link].slv.dest_mst_port != UTIL_GLOBAL_CPB_PORT_MAPOTN)
            result = DIGI_ERR_MAPPER_CHNL_NOT_AVAIL;
    }

    if(result == PMC_SUCCESS)
    {
        result = digi_cpb_push_pop_port_get(digi_handle,
                                            slv_port,
                                            UTIL_GLOBAL_CPB_PORT_MAPOTN,
                                            0, /* Serdes Client data source */
                                            1, /* Mapper path to ODUKSW destination */
                                            &ingress_mux_output_port,
                                            &num_push_ports,
                                            &egress_mux_input_port,
                                            &num_pop_ports);
    }

    /* Get CBRC inputs based on port uid and intf type */
    if (PMC_SUCCESS == result)
    {
        result = digi_cbr_info_get(digi_handle, 
                                   intf_type,
                                   port_uid,
                                   &cbrc_serdes,
                                   &cbrc_link,
                                   &mux_link);
    }

    if(prov == TRUE) /* provision */
    {
        /* confirm the connection is operational otherwise return an invalid state error */
        if (PMC_SUCCESS == result)
        {
            if(conn_ptr->chnl[slv_port][link].slv.state != UTIL_GLOBAL_CHNL_OPERATIONAL)
                result = DIGI_ERR_CPB_NOT_ACTIVE;
        }



        /* CBRC Configure database */ 
        if (PMC_SUCCESS == result)
        {
            digi_cbrc_prov_params_convert(DIGI_CBR_CLIENT_10G_STS192, &group_size);
            result = digi_cbrc_db_prov_cfg(digi_handle, group_size, cbrc_link, TRUE);
        } /* end if */

        if (PMC_SUCCESS == result)
        {
            /* determine state of the CBRC SS and if in lowpwr/reset state transition to a configurable state */
            digi_cbrc_energy_state_set(digi_handle, PMC_ENERGY_STATE_REQUEST_RUNNING);
        }

        /* CBR Provision and CBR_FSGM default pattern configuration */
        if (PMC_SUCCESS == result)
        {
            result = cbrc_port_provision(digi_handle->cbrc_handle,
                                         cbrc_link,
                                         cbrc_serdes,
                                         CBRC_PORT_MODE_10G_10GE);
        }

        /* CBR Provision post-mld lanes */
        if (PMC_SUCCESS == result)
        {
            result = cbrc_port_activate(digi_handle->cbrc_handle,
                                        CBRC_PORT_DIR_RX,
                                        cbrc_link);        
        }

        if (PMC_SUCCESS == result)
        {
            result = cbrc_fsgm_fault_force(digi_handle->cbrc_handle,
                                           cbrc_link,
                                           CBRC_PORT_DIR_RX,
                                           1);
        }        

        /* forced pattern activation */
        if (PMC_SUCCESS == result)
        {
            result = cbrc_patt_ins_fault_force(digi_handle->cbrc_handle,
                                               cbrc_link,
                                               CBRC_PORT_DIR_RX,
                                               TRUE);
        } 

        /* SW implementation of MPMA startup procedure */


        if (PMC_SUCCESS == result)
        {
            digi_opsa_cfg_dcpb_mpma_chnl_backup_cfg(digi_handle,
                                                    OPSA_HOSTMSG_CPB_SS_CBRC,
                                                    cbrc_link,
                                                    TRUE);
        }

        /* DCPB conditional CBRC interface configuration */
        if (PMC_SUCCESS == result)
        {
            BOOL8 port_state = cpb_port_init_state_get(digi_handle->dcpb_handle, CPB_PORT_TYPE_DPI_SLAVE, UTIL_GLOBAL_CPB_PORT_MAPOTN);
            if(FALSE == port_state)
            {
                result = digi_cpb_input_port_init(digi_handle, 0, UTIL_GLOBAL_CPB_PORT_MAPOTN,(cpb_xfer_mode_t)0,0);
            }
        }



        /* DCPB datapath provisioning */
        if (PMC_SUCCESS == result)
        {
            UINT32 rate, cpb_data;
            util_schd_chnl_rate_exp_t rate_unit;
            (void)digi_cpb_rate_get(DIGI_CPB_STREAM_10G, 0, &rate, &rate_unit, &cpb_data);
            result = cpb_chnl_prov(digi_handle->dcpb_handle,
                                   UTIL_GLOBAL_CPB_PORT_CBRC,
                                   cbrc_link,
                                   ingress_mux_output_port, 
                                   egress_mux_input_port,
                                   UTIL_GLOBAL_CPB_PORT_MAPOTN,
                                   mapotn_chnl,
                                   rate,
                                   rate_unit,
                                   cpb_data,
                                   0, /* halt_buffer */
                                   1, /* rx_type_header */
                                   TRUE);
        }

    }
    else /* deprovision */
    {
        /* tear down the channel if configured for BACKUP mode otherwise exit */
        if(result == PMC_SUCCESS && conn_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][cbrc_link].slv.state == UTIL_GLOBAL_CHNL_BACKUP)
        {
            BOOL8 use_backup = FALSE;

            if (PMC_SUCCESS == result)
            {
                result = cbrc_fsgm_fault_force(digi_handle->cbrc_handle,
                                               cbrc_link,
                                               CBRC_PORT_DIR_RX,
                                               0);
            }        
            
            /* forced pattern activation */
            if (PMC_SUCCESS == result)
            {
                result = cbrc_patt_ins_fault_force(digi_handle->cbrc_handle,
                                                   cbrc_link,
                                                   CBRC_PORT_DIR_RX,
                                                   FALSE);
            } 
			
            
            /* check current inbound fault application state and shut off if required */
            if(result == PMC_SUCCESS)
            {
                result = cpb_chnl_backup_status(digi_handle->dcpb_handle, (UINT32)slv_port, link, &use_backup);
            }

            if(result == PMC_SUCCESS && use_backup == TRUE)
            {
                cpb_chnl_backup_swap(digi_handle->dcpb_handle, (UINT32)slv_port, link, FALSE);
            }

            /* turn off MPMA OPSA procedure */
            if(result == PMC_SUCCESS)
            {
                digi_opsa_cfg_dcpb_mpma_chnl_backup_cfg(digi_handle,
                                                        OPSA_HOSTMSG_CPB_SS_CBRC,
                                                        cbrc_link,
                                                        FALSE);
            }

            /* DCPB tear down operations */
            if(result == PMC_SUCCESS)
            {
                result = cpb_chnl_deprov(digi_handle->dcpb_handle, UTIL_GLOBAL_CPB_PORT_MAPOTN, mapotn_chnl, TRUE);
            }

            /* CBRC tear down operations */
            if (PMC_SUCCESS == result) 
            {
                result = cbrc_port_deactivate(digi_handle->cbrc_handle,
                                              CBRC_PORT_DIR_RX,
                                              cbrc_link);
            }
            if (PMC_SUCCESS == result)
            {
                result = cbrc_port_unprovision(digi_handle->cbrc_handle,
                                               cbrc_link);
            }
    
            if (PMC_SUCCESS == result) {
                result = digi_cbrc_db_deprov_cfg(digi_handle, cbrc_link);
            }

            if (PMC_SUCCESS == result)
            {
                /* bring CBRC SS to lowest energy state if required */
                digi_cbrc_energy_state_set(digi_handle, PMC_ENERGY_STATE_REQUEST_OPTIMAL);
            }
        }
        else if(result== PMC_SUCCESS && conn_ptr->chnl[UTIL_GLOBAL_CPB_PORT_CBRC][cbrc_link].slv.state == UTIL_GLOBAL_CHNL_OPERATIONAL)
        {
            /* channel is in unexpected state return an error */
            result = DIGI_ERR_FAULT_ACTIVATED;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
}/* digi_enet_inbound_fault_prov */

/*******************************************************************************
* digi_enet_inbound_fault_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls the forced application or removal of fault signal
*   in replacement of the inbound ENET_LINE or ENET_SYS data stream bound for
*   MAPOTN mapper path.\n\n
*
*
*   The replacement signal is configured for LF.  The replacement signal is
*   at nominal rate.\n\n
*
*   Prior to this API call, digi_enet_inbound_fault_prov() must be called to
*   setup the CBRC and CPB for the inbound fault replacement signal.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE or ENET_SYS channel handle
*                              instance that is source of data stream towards
*                              MAPOTN.
*   force                    - API action, TRUE: force fault, FALSE: transfer
*                              regular data
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_inbound_fault_force(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle, BOOL force)
{

    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link;
    util_global_cpb_port_t slv_port = LAST_UTIL_GLOBAL_CPB_PORT;
    BOOL8 use_backup = FALSE;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify this is enet_line or enet_sys handle. */
    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);

    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if(result == PMC_SUCCESS && enet_ss_handle_ptr == digi_handle->enet_line_handle)
    {
        PMC_ERROR temp_result = PMC_SUCCESS;
        slv_port = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
        temp_result = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);
        if(temp_result == PMC_SUCCESS) result = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        slv_port = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
    }

    /* check current state and update if input request is different */
    if(result == PMC_SUCCESS)
    {
        result = cpb_chnl_backup_status(digi_handle->dcpb_handle, (UINT32)slv_port, link, &use_backup);
    }


    if(result == PMC_SUCCESS && use_backup != force)
    {
        cpb_chnl_backup_swap(digi_handle->dcpb_handle, (UINT32)slv_port, link, force);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_enet_inbound_fault_force */

/*******************************************************************************
* digi_enet_inbound_fault_type_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls the type of fault signal generated with the 10GE
*   CBR inbound fault replacement signal generator.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE or ENET_SYS channel handle
*                              instance that is source of data stream towards
*                              MAPOTN.
*   fault_lf                 - TRUE:  Local Fault generated, FALSE: Remote
*                              Fault generated.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_inbound_fault_type_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle, BOOL fault_lf)
{

    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link;
    digi_serdes_intf_t intf_type = LAST_DIGI_SERDES_INTF;
    UINT32 dsi_base_lane, port_uid;
    UINT32                      cbrc_link = 0; /* Post MLD link (0..11) */
    cbrc_serdes_type_t          cbrc_serdes = CBRC_SERDES_TYPE_NONE;
    UINT32                      mux_link = 0;  /* Post Mux link */

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    dsi_base_lane = 0;
    /* verify this is enet_line or enet_sys handle. */
    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);
    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
        dsi_base_lane = ((digi_enet_client_chnl_def_t*)chnl_handle)->dsi_base_lane;
    }



    if(result == PMC_SUCCESS && enet_ss_handle_ptr == digi_handle->enet_line_handle)
    {
        PMC_ERROR temp_result = PMC_SUCCESS;
        port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[dsi_base_lane].port_uid;
        intf_type = DIGI_SERDES_LINE_INTF;
        temp_result = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);
        if(temp_result == PMC_SUCCESS) result = DIGI_ERR_INVALID_ARG;
        if(result == PMC_SUCCESS && digi_handle->var.prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) result = DIGI_ERR_INVALID_ARG;
    }
    else
    {
        port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[dsi_base_lane].port_uid;
        intf_type = DIGI_SERDES_SYSTEM_INTF;
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_cbr_info_get(digi_handle, 
                                   intf_type,
                                   port_uid,
                                   &cbrc_serdes,
                                   &cbrc_link,
                                   &mux_link);
    }



    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_user_fault_pattern_set(digi_handle->cbrc_handle,
                                                  cbrc_link,
                                                  CBRC_PORT_DIR_RX,
                                                  fault_lf==TRUE?CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE:CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE);
    } 

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_enet_inbound_fault_type_cfg */

/*******************************************************************************
* digi_enet_inbound_fault_type_pn_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls the pattern of fault signal generated with the 10GE
*   CBR inbound fault replacement signal generator.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE or ENET_SYS channel handle
*                              instance that is source of data stream towards
*                              MAPOTN.
*   fault_pn11                 - TRUE:  PN11 generated, FALSE: Local
*                              Fault generated
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_inbound_fault_type_pn_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle, BOOL fault_pn11)
{

    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link;
    digi_serdes_intf_t intf_type = LAST_DIGI_SERDES_INTF;
    UINT32 dsi_base_lane, port_uid;
    UINT32                      cbrc_link = 0; /* Post MLD link (0..11) */
    cbrc_serdes_type_t          cbrc_serdes = CBRC_SERDES_TYPE_NONE;
    UINT32                      mux_link = 0;  /* Post Mux link */

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* verify this is enet_line or enet_sys handle. */
    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);
    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
        dsi_base_lane = ((digi_enet_client_chnl_def_t*)chnl_handle)->dsi_base_lane;
    }


    if (PMC_SUCCESS == result)
    {
          if (enet_ss_handle_ptr == digi_handle->enet_line_handle) 
          {
              PMC_ERROR temp_result = PMC_SUCCESS;
              port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[dsi_base_lane].port_uid;
              intf_type = DIGI_SERDES_LINE_INTF;
              temp_result = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);

             if ((PMC_SUCCESS == temp_result) ||(DIGI_PROD_APP_TRANSMUXPONDER_CARD == digi_handle->var.prod_app)) 
             {
                 result = DIGI_ERR_INVALID_ARG;
             }
         }
         else
         {
             port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[dsi_base_lane].port_uid;
             intf_type = DIGI_SERDES_SYSTEM_INTF;
         }

   }


    if (PMC_SUCCESS == result)
    {
        result = digi_cbr_info_get(digi_handle, 
                                   intf_type,
                                   port_uid,
                                   &cbrc_serdes,
                                   &cbrc_link,
                                   &mux_link);
    }



    if (PMC_SUCCESS == result)
    {
        if( TRUE == fault_pn11 )
        {
        result = cbrc_fsgm_pattern_cfg(digi_handle->cbrc_handle,
                                       cbrc_link,
                                       CBRC_PORT_DIR_RX,
                                       CBR_FSGM_PATTERN_PN11);
        }
        else
        {
            result = cbrc_fsgm_pattern_cfg(digi_handle-> cbrc_handle, 
                                               cbrc_link, 
                                               CBRC_PORT_DIR_RX, 
                                               CBR_FSGM_PATTERN_64B_66B); 

            if (PMC_SUCCESS == result)
            {
            result = cbrc_fsgm_user_fault_pattern_set( digi_handle->cbrc_handle,
                                                cbrc_link, 
                                                CBRC_PORT_DIR_RX,
                                                CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE);          
            }      
        }
    } 

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_enet_inbound_fault_type_pn_cfg */


/*******************************************************************************
* digi_enet_inbound_fault_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves configuration detail of the CBRC SS used for
*   inbound fault replacement of 10GE CBR applications.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE or ENET_SYS channel handle
*                              instance that is source of data stream towards
*                              MAPOTN.
*
* OUTPUTS:
*   cfg_state_ptr            - TRUE:  CBRC provisioned for fault replacement, 
*                              FALSE: CBRC no provisioned for fault replacement
*   lf_type_ptr              - If cfg_state_ptr is TRUE, Fault type configured 
*                              in CBRC subsystem TRUE: LF insertion, FALSE: 
*                              RF insertion
*   active_state_ptr         - If cfg_state_ptr is TRUE, TRUE: CBRC generating
*                              fault stream replacement, FALSE: ENET generating
*                              data stream
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_inbound_fault_state_get(digi_handle_t *digi_handle, 
                                                   util_global_switch_data_t *chnl_handle, 
                                                   BOOL8 *cfg_state_ptr,
                                                   BOOL8 *lf_type_ptr,
                                                   BOOL8 *active_state_ptr)
{

    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link;
    digi_serdes_intf_t intf_type = LAST_DIGI_SERDES_INTF;
    UINT32 dsi_base_lane, port_uid;
    BOOL8 use_backup = FALSE;
    util_global_cpb_port_t slv_port = LAST_UTIL_GLOBAL_CPB_PORT;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    dsi_base_lane = 0;
    /* verify this is enet_line or enet_sys handle. */
    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);
    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
        dsi_base_lane = ((digi_enet_client_chnl_def_t*)chnl_handle)->dsi_base_lane;
    }



    if(result == PMC_SUCCESS && enet_ss_handle_ptr == digi_handle->enet_line_handle)
    {
        PMC_ERROR temp_result = PMC_SUCCESS;
        port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[dsi_base_lane].port_uid;
        intf_type = DIGI_SERDES_LINE_INTF;
        temp_result = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);
        if(temp_result == PMC_SUCCESS) result = DIGI_ERR_INVALID_ARG;
        if(result == PMC_SUCCESS && digi_handle->var.prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) result = DIGI_ERR_INVALID_ARG;
        slv_port = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
    }
    else
    {
        port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[dsi_base_lane].port_uid;
        intf_type = DIGI_SERDES_SYSTEM_INTF;
        slv_port = UTIL_GLOBAL_CPB_PORT_ENET_SYS;
    }

    /* check current state and update if input request is different */
    if(result == PMC_SUCCESS)
    {
        result = cpb_chnl_backup_status(digi_handle->dcpb_handle, (UINT32)slv_port, link, &use_backup);
    }

    if(result == CPB_ERR_NO_BACKUP_SETUP)
    {
        *cfg_state_ptr    = FALSE;
        *lf_type_ptr      = FALSE;
        *active_state_ptr = FALSE;
         result = PMC_SUCCESS;
    }
    else if(result == PMC_SUCCESS)
    {
        cbr_fsgm_user_fault_pattern_t user_pattern = LAST_CBR_FSGM_USER_FAULT_PATTERN;
        UINT32                      cbrc_link = 0; /* Post MLD link (0..11) */
        cbrc_serdes_type_t          cbrc_serdes = CBRC_SERDES_TYPE_NONE;
        UINT32                      mux_link = 0;  /* Post Mux link */
        *cfg_state_ptr    = TRUE;
        *active_state_ptr = use_backup;

        result = digi_cbr_info_get(digi_handle, 
                                   intf_type,
                                   port_uid,
                                   &cbrc_serdes,
                                   &cbrc_link,
                                   &mux_link);

        if (PMC_SUCCESS == result)
        {
            result = cbrc_fsgm_user_fault_pattern_get(digi_handle->cbrc_handle,
                                                      cbrc_link,
                                                      CBRC_PORT_DIR_RX,
                                                      &user_pattern);
        }

        if (PMC_SUCCESS == result)
        {
            if(user_pattern == CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE)
            {
                *lf_type_ptr      = TRUE;
            }
            else if(user_pattern == CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE)
            {
                *lf_type_ptr      = FALSE;
            }
            else
            {
                result = DIGI_ERR_INVALID_PATTERN_CFG;
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_enet_inbound_fault_state_get */

/*******************************************************************************
* digi_enet_eclass_filter_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function applies a filter config to the ENET_LINE eclass subsystem.
*   The function can be called mulitple times to keep adding more filters
*   to the eclass, provided there are adequate resources. Eclass filter is
*   applied per enet link
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE channel handle to apply 
*                              filter to
*   cfg                      - enum describing filter mode to apply
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_eclass_filter_config(digi_handle_t *digi_handle,
                                                util_global_switch_data_t *chnl_handle,
                                                digi_enet_eclass_filter_config_t cfg)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 link;
    UINT8 link_id;
    enet_handle_t *enet_ss_handle_ptr;
    UINT32 data[4];

    UINT16 tci_value1_inner = 0x0;
    UINT16 tci_value2_outer = 0x0;

    PMC_ATOMIC_ENTRY(digi_handle);


    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);
    if ( (DIGI_ENET_ECLASS_DIR_INGRESS == cfg.direction) && (DIGI_ENET_ECLASS_MODE_PTP_UCAST_DA == cfg.mode) && (DIGI_ENET_ECLASS_TARGET_CPUH == cfg.target) &&
         (ENET_VLAN_NO_TAG == cfg.vlan_tag_cfg) && (DIGI_ENET_ECLASS_ENCAP_ETH == cfg.encap) )
    {   /*UCAST DA Check over Ethernet*/
        UINT32 da_msb = 0x0;
        UINT32 da_lsb = 0x0;
        da_msb = cfg.param_2;
        da_lsb = cfg.param_1;
        eclass_ptp_unicastdacheck_over_eth_filter_cfg(enet_ss_handle_ptr->empei_i_eclass_handle, link, da_msb, da_lsb, cfg.enable);
    } 
    else if ( (DIGI_ENET_ECLASS_DIR_INGRESS == cfg.direction) && (DIGI_ENET_ECLASS_MODE_PTP_UCAST_DA == cfg.mode) && (DIGI_ENET_ECLASS_TARGET_CPUH == cfg.target) &&
          (ENET_VLAN_NO_TAG == cfg.vlan_tag_cfg) && (DIGI_ENET_ECLASS_ENCAP_UDP_IPV4_ETH == cfg.encap) )
    {   /*UCAST DA Check over IPV4*/
        UINT32 ipv4_da = 0x0;
        ipv4_da = cfg.param_1;
        eclass_ptp_unicastdacheck_over_ipv4_filter_cfg(enet_ss_handle_ptr->empei_i_eclass_handle, link, ipv4_da, cfg.enable);
    }
    else if ( (DIGI_ENET_ECLASS_DIR_INGRESS == cfg.direction) && (DIGI_ENET_ECLASS_MODE_PTP_1VLAN_9200 == cfg.mode) && (DIGI_ENET_ECLASS_TARGET_CPUH == cfg.target) &&
         (ENET_VLAN_SINGLE_TAG == cfg.vlan_tag_cfg) && (DIGI_ENET_ECLASS_ENCAP_ETH == cfg.encap) )
    { /*TCI Check  1VLAN over Ethernet*/
        tci_value1_inner = (UINT16)cfg.param_1;
        tci_value2_outer = (UINT16)cfg.param_2;
        eclass_ptp_vlancheck_tci_filter_cfg(enet_ss_handle_ptr->empei_i_eclass_handle, link, ECLASS_VLAN_TCI_CHECK_OVER_ETHERNET, ECLASS_VLAN_CFG_1VLAN, tci_value1_inner, tci_value2_outer, cfg.enable);
    }
    else if ( (DIGI_ENET_ECLASS_DIR_INGRESS == cfg.direction) && (DIGI_ENET_ECLASS_MODE_PTP_2VLAN_EXTRA == cfg.mode) && (DIGI_ENET_ECLASS_TARGET_CPUH == cfg.target) &&
         (ENET_VLAN_DOUBLE_TAG == cfg.vlan_tag_cfg) && (DIGI_ENET_ECLASS_ENCAP_ETH == cfg.encap) )
    { /*TCI Check 2VLAN over Ethernet*/
        tci_value1_inner = (UINT16)cfg.param_1;
        tci_value2_outer = (UINT16)cfg.param_2;
        eclass_ptp_vlancheck_tci_filter_cfg(enet_ss_handle_ptr->empei_i_eclass_handle, link, ECLASS_VLAN_TCI_CHECK_OVER_ETHERNET, ECLASS_VLAN_CFG_2VLAN, tci_value1_inner, tci_value2_outer, cfg.enable);
    }
     else if ( (DIGI_ENET_ECLASS_DIR_INGRESS == cfg.direction) && (DIGI_ENET_ECLASS_MODE_PTP_1VLAN_9200 == cfg.mode) && (DIGI_ENET_ECLASS_TARGET_CPUH == cfg.target) &&
        (ENET_VLAN_SINGLE_TAG == cfg.vlan_tag_cfg) && (DIGI_ENET_ECLASS_ENCAP_UDP_IPV4_ETH == cfg.encap) )
    { /*TCI Check 1VLAN over IP4*/
        tci_value1_inner = (UINT16)cfg.param_1;
        tci_value2_outer = (UINT16)cfg.param_2;
        eclass_ptp_vlancheck_tci_filter_cfg(enet_ss_handle_ptr->empei_i_eclass_handle, link, ECLASS_VLAN_TCI_CHECK_OVER_IPV4, ECLASS_VLAN_CFG_1VLAN, tci_value1_inner, tci_value2_outer, cfg.enable);
    }
    else if ( (DIGI_ENET_ECLASS_DIR_INGRESS == cfg.direction) && (DIGI_ENET_ECLASS_MODE_PTP_2VLAN_EXTRA == cfg.mode) && (DIGI_ENET_ECLASS_TARGET_CPUH == cfg.target) &&
         (ENET_VLAN_DOUBLE_TAG == cfg.vlan_tag_cfg) && (DIGI_ENET_ECLASS_ENCAP_UDP_IPV4_ETH == cfg.encap) )
    { /*TCI Check 2VLAN over IP4*/
        tci_value1_inner = (UINT16)cfg.param_1;
        tci_value2_outer = (UINT16)cfg.param_2;
        eclass_ptp_vlancheck_tci_filter_cfg(enet_ss_handle_ptr->empei_i_eclass_handle, link, ECLASS_VLAN_TCI_CHECK_OVER_IPV4, ECLASS_VLAN_CFG_2VLAN, tci_value1_inner, tci_value2_outer, cfg.enable);
    }
    else 
    {
      link_id = (UINT8)link;

    /* FILTER_CFG has the following format,
     * Word      bits 24..31          bits 16..23          bits 8..15          bits 0..7
     * 0    |      target       |       encap        |      link_id      |       mode        |
     * 1    |                                 clockIdentity                                  |
     */

    data[0] = ((cfg.target) << 24) |
              ((cfg.encap) << 16) |
              ((cfg.mode) << 8) |
              (link_id);

    data[1] = cfg.param_1;

    data[2] = cfg.direction;

    data[3] = cfg.param_2;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        if (PMC_SUCCESS == result) {
            result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle, MODID_ENET, HOSTMSG_CMD_CFG_ENET_ECLASS_FILTER_CFG_ST, data, 4, stat);
        }
      }

    }
    
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_enet_eclass_filter_config */


/*******************************************************************************
* digi_enet_lldp_config
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable LLDP filtering.\n\n
*   
*   For DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP applications, egress LLDP pmon is retrieved
*   from enet enhanced pmon handle. The enet enhanced pmon handle is associated to 
*   Mapper channel and can be got by API digi_enet_lldp_companion_pmon_get().\n\n
*
*   Parameter setting for egress LLDP monitoring for DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP 
*   applications:\n
*     chnl_handle:  set to enet enhanced pmon handle\n
*     direct:       set to DIGI_ENET_ECLASS_DIR_INGRESS\n
*     enet_tx_pmon: set to 0x1 or 0x2\n\n
*   
*   For DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET applications, egress LLDP pmon is 
*   retrieved from enet companion pmon handle. The enet companion pmon handle can be 
*   got by API digi_enet_lldp_companion_pmon_get().\n\n
*
*   Parameter setting for egress LLDP monitoring for DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET 
*   applications:\n
*     chnl_handle:  set to enet companion pmon handle\n
*     direct:       set to DIGI_ENET_ECLASS_DIR_INGRESS\n
*     enet_tx_pmon: set to 0x1 or 0x2\n\n
*   
*   API sequence for LLDP feature:\n
*   For DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP and DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET
*   applications:\n
*     Ingress LLDP monitoring:\n
*       digi_enet_cpu_extract_pkt_cfg()\n
*       digi_enet_lldp_config()\n
*       digi_enet_lpd_pkt_fetch()\n
*       digi_enet_lpd_release()\n
*     Egress LLDP monitoring:\n
*       digi_enet_lldp_companion_pmon_get()\n
*       digi_enet_cpu_extract_pkt_cfg()\n
*       digi_enet_lldp_config()\n
*       digi_enet_lpd_pkt_fetch()\n
*       digi_enet_lpd_release()\n\n
*   For other applications:\n
*     Ingress/Egress LLDP monitoring:\n
*       digi_enet_cpu_extract_pkt_cfg()\n
*       digi_enet_lldp_config()\n
*       digi_enet_lpd_pkt_fetch()\n
*       digi_enet_lpd_release()\n\n
*
*   To disable Ingress/Egress LLDP monitoring:\n
*       digi_enet_cpu_extract_pkt_cfg()\n
*       digi_enet_lldp_config()\n
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE/ENET_SYS channel handle to apply 
*                              filter to
*   packet_encap_type        - Packet is encapsulated by ethernet or eth over ipv4 or eth over ipv6
*                              0 is to disable LLDP filtering.
*   lldp_filter_mode         - eclass lldp filter mode (only lldp types are valid)
*   eth_type_field           - EtherType value to filter (only needed for LLDP_ETH filter mode)
*   direct                   - eclass datapath direction.
*   enet_tx_pmon             - indicate if the enet handle is used as LLDP Rx pmon or Tx pmon.
*                              0x0 used as LLDP Rx pmon
*                              If > 0, used as LLDP Tx pmon. Valid value as below:
*                               0x1 enet line 
*                               0x2 enet sys 
*                              This parameter is only valid when direct is DIGI_ENET_ECLASS_DIR_INGRESS
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_lldp_config(digi_handle_t *digi_handle,
                                       util_global_switch_data_t * chnl_handle,
                                       digi_enet_eclass_encap_t packet_encap_type,
                                       digi_enet_eclass_mode_t lldp_filter_mode,
                                       UINT32 eth_type_field,
                                       digi_enet_eclass_dir_t direct,
                                       UINT32  enet_tx_pmon)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_enet_eclass_filter_config_t eclass_cfg;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate = (emac_data_rate_t)0;
    UINT32 const no_match_filter_param = 0x00400000;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    digi_enet_fw_enet_client_source_conditional_update(digi_handle, chnl_handle);

    eclass_cfg.encap = packet_encap_type;
    eclass_cfg.direction = direct;
    eclass_cfg.param_1 = 0;
    eclass_cfg.param_2 = enet_tx_pmon;

    if(digi_enet_is_link_transparent(enet_handle, link)) {
        eclass_cfg.mode = DIGI_ENET_ECLASS_MODE_NO_MATCH_ALL;
        eclass_cfg.target = DIGI_ENET_ECLASS_TARGET_MGMTH;

        result = digi_enet_eclass_filter_config(digi_handle, chnl_handle, eclass_cfg);

        if (PMC_SUCCESS == result) {
            switch(enet_handle->var.ch_datapath[link]) {

                case ENET_SERDES_100GE_TRANSPARENT:
                case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
                case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                    data_rate = EMAC_LINK_100G;
                    break;
                case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
                case ENET_SERDES_40GE_TRANSPARENT:
                case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                    data_rate = EMAC_LINK_40G;
                    break;
                case ENET_SERDES_10GE_TRANSPARENT:
                case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
                case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
                case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
                case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
                case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                    data_rate = EMAC_LINK_10G;
                    break;
                default: 
                    result = DIGI_ERR_INVALID_ARG;
                    break;
            }
        }

        if( DIGI_ENET_ECLASS_DIR_INGRESS == direct)
        {
            if (PMC_SUCCESS == result) {
                empei_i_cpb_link_sch_cfg(enet_handle->empei_i_handle, 11, 0, TRUE, enet_handle->var.dpi_sch_slot);
            }

            if (PMC_SUCCESS == result) {
                empei_i_internal_link_sch_cfg(enet_handle->empei_i_handle, 11, TRUE, enet_handle->var.dpi_sch_slot);
            }

            if (PMC_SUCCESS == result) {
                emac_ingr_data_sink_cfg(enet_handle->emac_handle, link, data_rate, FALSE);
            }
        }
        else
        {
            if (PMC_SUCCESS == result) {
                empei_e_cpb_link_sch_cfg(enet_handle->empei_e_handle, 11, 0, TRUE, enet_handle->var.dpi_sch_slot);
            }

            if (PMC_SUCCESS == result) {
                empei_e_internal_link_sch_cfg(enet_handle->empei_e_handle, 11, TRUE, enet_handle->var.dpi_sch_slot);
            }

            if (PMC_SUCCESS == result) {
                emac_egr_data_squelching_cfg(enet_handle->emac_handle, link, data_rate, FALSE, 0);
            }
        }
    }
    else if (digi_enet_is_link_mac_block(enet_handle, link)) {
        eclass_cfg.mode = DIGI_ENET_ECLASS_MODE_NO_MATCH_ALL;
        eclass_cfg.target = DIGI_ENET_ECLASS_TARGET_DCPB;

        result = digi_enet_eclass_filter_config(digi_handle, chnl_handle, eclass_cfg);


        if (PMC_SUCCESS == result) {
            eclass_cfg.mode = lldp_filter_mode;
            eclass_cfg.target = DIGI_ENET_ECLASS_TARGET_DCPB;
            eclass_cfg.param_1 = eth_type_field;
            
            result = digi_enet_eclass_filter_config(digi_handle, chnl_handle, eclass_cfg);
        }

    }
    else {
        result = DIGI_ERR_INVALID_ARG;
    }

    if (PMC_SUCCESS == result) {
        eclass_cfg.mode = DIGI_ENET_ECLASS_MODE_NO_MATCH_CONFIG;
        eclass_cfg.target = DIGI_ENET_ECLASS_TARGET_NA;
        eclass_cfg.param_1 = no_match_filter_param;

        result = digi_enet_eclass_filter_config(digi_handle, chnl_handle, eclass_cfg);
    }

    if (PMC_SUCCESS == result) {
        eclass_cfg.mode = lldp_filter_mode;
        eclass_cfg.target = DIGI_ENET_ECLASS_TARGET_CPUH;
        eclass_cfg.param_1 = eth_type_field;
        
        result = digi_enet_eclass_filter_config(digi_handle, chnl_handle, eclass_cfg);
    }

    if((PMC_SUCCESS == result) && (enet_tx_pmon != 0) && (DIGI_ENET_ECLASS_DIR_INGRESS == direct))
    {
        /* Special operation for egress LLDP monitoring for 10GE C7.1/C7.3, 40GE, 100GE CBR traffic 
           If enet_tx_pmon != 0, this enet handle is used as Tx pmon.
        */
        switch(enet_handle->var.ch_datapath[link]) {
            case ENET_SLAVE_10GE_MAC_TX_PMON:
            case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
            case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
            case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
            case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                data_rate = EMAC_LINK_10G;
                break;
            case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
            case ENET_SLAVE_40GE_MAC_TX_PMON:
            case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                data_rate = EMAC_LINK_40G;
                break;
            case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
            case ENET_SLAVE_100GE_MAC_TX_PMON:
            case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                data_rate = EMAC_LINK_100G;
                break;
            default: 
                result = DIGI_ERR_INVALID_ARG;
                break;
        }
        if(PMC_SUCCESS == result) {
            emac_ingr_data_sink_cfg(enet_handle->emac_handle, link, data_rate, FALSE);
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_lldp_config */

/*******************************************************************************
* digi_enet_lldp_companion_pmon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the companion ENET PMON handle.
*
*   For 10GE G.Sup43 C7.1/7.3 data paths, the LLDP monitoring for the egress 
*   direction are done by the enhanced PMON ENET subsystem. This API is used to 
*   get the companion ENET PMON channel handle and pass it to API 
*   digi_enet_lldp_config() and digi_enet_cpu_extract_pkt_cfg().
*
*   This fucntion is only used for LLDP feature.
*
* INPUTS:
*   digi_handle        - pointer to DIGI handle instance.
*   *chnl_handle       - a pointer to generic util_global_switch_data_t.
*   is_enhanced_pmon   - TRUE: get enhanced pmon handle
*                        FALSE: get companion pmon handle
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   A pointer to the ENET line/sys chnl handle if it has been set, NULL
*   otherwise.
*
* NOTES:
*   NONE.
*
*******************************************************************************/
PUBLIC util_global_switch_data_t*  digi_enet_lldp_companion_pmon_get(digi_handle_t *digi_handle,
                                                                    util_global_switch_data_t * chnl_handle,
                                                                    BOOL8 is_enhanced_pmon)
{
    util_global_switch_data_t *ret_ptr = NULL;

    PMC_ENTRY();

    if(is_enhanced_pmon)
    {
        /* Get pointer to enet line/sys channel to obtain PMON */
        ret_ptr = (util_global_switch_data_def_t*)digi_enet_enhanced_pmon_ptr_get((util_global_switch_data_def_t *)chnl_handle);
    }
    else
    {
        ret_ptr = digi_enet_companion_pmon_get(digi_handle, chnl_handle);
    }

    PMC_RETURN(ret_ptr);
}

/*******************************************************************************
* digi_enet_transparent_lldp_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to enable/disable LLDP filtering on a transparent enet path.
*   digi_enet_lldp_config() needs to have been called first on a transparent enet channel
*   to configure the lldp filters. 
*   
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE channel handle to apply 
*                              filter to
*   enable                   - TRUE to enable feature, FALSE to disable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_transparent_lldp_set(digi_handle_t *digi_handle,
                                                util_global_switch_data_t * chnl_handle,
                                                BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate = (emac_data_rate_t)0;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == result)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == result) {
        switch(enet_handle->var.ch_datapath[link]) {

            case ENET_SERDES_100GE_TRANSPARENT:
            case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
                data_rate = EMAC_LINK_100G;
                break;
            case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            case ENET_SERDES_40GE_TRANSPARENT:
            case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
                data_rate = EMAC_LINK_40G;
                break;
            case ENET_SERDES_10GE_TRANSPARENT:
            case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
            case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
            case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
            case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
                data_rate = EMAC_LINK_10G;
                break;
            default: 
                result = DIGI_ERR_INVALID_ARG;
                break;
        }
    }

    if (digi_enet_is_link_transparent(enet_handle, link)) {
        /* ING_DATA_SINK=0 enables flow of traffic */
        if (enable) {
            emac_ingr_data_sink_cfg(enet_handle->emac_handle, link, data_rate, FALSE);
        }
        else {
            emac_ingr_data_sink_cfg(enet_handle->emac_handle, link, data_rate, TRUE);
        }
    }
    else {
        /* This function does not support MAC terminated lldp disable */
        result = DIGI_ERR_INVALID_ARG;
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_enet_transparent_lldp_disable */

/*******************************************************************************
* digi_enet_lpd_release
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to release the LPD buffer for the given ENET_LINE
*   channel and allow the device to update the buffer with the latest LPD
*   packet
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE channel handle to apply 
*                              filter to
*   direct                   - ingress or egress
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_lpd_release(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *chnl_handle,
                                       digi_enet_eclass_dir_t direct)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 link;
    enet_handle_t *enet_ss_handle_ptr;
    UINT32 data[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link);

    data[0] = link;
    data[1] = direct;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

        if (PMC_SUCCESS == result) {
            result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle, MODID_ENET, HOSTMSG_CMD_CFG_ENET_LDP_RELEASE_ST, data, 2, stat);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_enet_lpd_release */

/*******************************************************************************
* digi_enet_lpd_pkt_fetch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to retrieve the address of the first 32 bytes of the 
*   latest packet from the LPD buffer for the given ENET_LINE channel. If the
*   buffer has no packet, an address of 0 is returned.
*
*   The LLDP packet is stored in the device's internal SRAM which can be 
*   accessed through register read via PCIe. User can call API digi_reg_read()
*   to read packet data.
*   
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE channel handle to apply 
*                              filter to
*   direct                   - ingress or egress
*
* OUTPUTS:
*   pkt_addr                 - pointer to retrieved packet address
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_lpd_pkt_fetch(digi_handle_t *digi_handle,
                                         util_global_switch_data_t *chnl_handle,
                                         digi_enet_eclass_dir_t direct,
                                         UINT32 *pkt_addr)
{
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link_id;
    UINT32 data[2];
    UINT32 response[1];
    HOSTMSG_DECLARE_QUERY_STAT(stat);

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link_id);
    data[0] = link_id;
    data[1] = direct;

    if (PMC_SUCCESS == result) {
        result = hostmsg_perform_fixed_length_query((pmc_handle_t*)digi_handle,
                                                     MODID_ENET,
                                                     HOSTMSG_CMD_CFG_ENET_LDP_PKT_FETCH_ST,
                                                     data, 2,
                                                     response, 1,
                                                     stat);
    }

    if (PMC_SUCCESS == result) {
        *pkt_addr = CONVERT_MIPS_ADDR_TO_HOST_ADDR(response[0]);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);

} /* digi_enet_lpd_pkt_fetch */

/*******************************************************************************
* digi_enet_lldp_cnts_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to retrieve the LLDP packet counts the
*   given ENET_LINE channel.
*
*   
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*   chnl_handle              - pointer to ENET_LINE channel handle to apply 
*                              filter to
*   direct                   - ingress or egress
*
* OUTPUTS:
*   lldp_cnts_ptr            - pointer to the received LLDP packet counts 
*   lldp_cnts_missed_ptr     - pointer to the missed LLDP packet counts
*
* RETURNS:
*   PMC_ERROR               - PMC_SUCCESS on success or meaningful error code
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_lldp_cnts_get(digi_handle_t *digi_handle,
                                         util_global_switch_data_t *chnl_handle,
                                         digi_enet_eclass_dir_t direct,
                                         UINT32 *lldp_cnts_ptr,
                                         UINT32 *lldp_cnts_missed_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link_id;
    UINT32 data[2];
    UINT32 response[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    /*check inputs*/
    PMC_ASSERT(chnl_handle != NULL,DIGI_ERR_NULL_HANDLE , 0, 0);
    PMC_ASSERT(lldp_cnts_ptr != NULL,DIGI_ERR_INVALID_PTR , 0, 0);
    PMC_ASSERT(lldp_cnts_missed_ptr != NULL,DIGI_ERR_INVALID_PTR , 0, 0);

    result = digi_enet_validate_parms(digi_handle,
                                      chnl_handle,
                                      &enet_ss_handle_ptr,
                                      &link_id);
    data[0] = link_id;
    data[1] = direct;

    if (PMC_SUCCESS == result) 
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        
        /* Returns 0 = Packet is available, 1 = No packet available */
        result = hostmsg_perform_fixed_length_query((pmc_handle_t*)enet_ss_handle_ptr,
                                                    MODID_ENET,
                                                    HOSTMSG_CMD_REQ_ENET_LLDP_PKT_CNTS_ST,
                                                    data, sizeof(data)/sizeof(UINT32),
                                                    response, 2,
                                                    stat);
    }

    /* get LLDP packet counts */
    if (PMC_SUCCESS == result) {
        *lldp_cnts_ptr = response[0];
        *lldp_cnts_missed_ptr = response[1];
    } 
    PMC_ATOMIC_RETURN(digi_handle, result);

} /* digi_enet_lldp_cnts_get */

/*******************************************************************************
* digi_mgmt_fege_rx_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Return MGMT_FEGE RX flow control status: enabled or disabled.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *enable                  - RX flow control status. 
*                              when TRUE, flow control is activated
*                              otherwise it is deactivated
*   *drop_pause_frm          - pause frame forwarding status. 
*                              when TRUE, RX pause frames are terminated in MGMT_FEGE
*                              when FALSE, RX pause frames are forwarded.
*
* RETURNS:
*   None.
*   
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_mgmt_fege_rx_flow_control_get(digi_handle_t       *digi_handle,
                                               BOOL8               *enable,
                                               BOOL8               *drop_pause_frm)
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    mgmt_fege_rx_flow_control_get(digi_handle->mgmt_fege_handle, enable, drop_pause_frm);

    PMC_ATOMIC_RETURN(digi_handle);
}/* digi_mgmt_fege_rx_flow_control_get */


/*******************************************************************************
* digi_mgmt_fege_frame_sizes_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MGMT_FEGE TX and RX maximum and minimum frame size.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*    txMaxFrameSize          - TX frames maximum size
*    txMinFrameSize          - TX frames minimum size
*    rxMaxFrameSize          - RX frames maximum size
*    rxMinFrameSize          - RX frames minimum size
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code 
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mgmt_fege_frame_sizes_set(digi_handle_t          *digi_handle,
                                                UINT16                  txMaxFrameSize,
                                                UINT16                  txMinFrameSize,
                                                UINT16                  rxMaxFrameSize,
                                                UINT16                  rxMinFrameSize)
{
    PMC_ERROR    rc = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    rc = mgmt_fege_frame_sizes_set(digi_handle->mgmt_fege_handle, 
                                   txMaxFrameSize,
                                   txMinFrameSize,
                                   rxMaxFrameSize,
                                   rxMinFrameSize);
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
}/* digi_mgmt_fege_frame_sizes_set */


/*******************************************************************************
* digi_mgmt_fege_frame_sizes_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure MGMT_FEGE TX and RX maximum and minimum frame size.
*
* INPUTS:
*   digi_handle              - pointer to DIGI handle instance.
*
* OUTPUTS:
*   *txMaxFrameSize          - TX frames maximum size
*   *txMinFrameSize          - TX frames minimum size
*   *rxMaxFrameSize          - RX frames maximum size
*   *rxMinFrameSize          - RX frames minimum size
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_mgmt_fege_frame_sizes_get(digi_handle_t          *digi_handle,
                                           UINT16                 *txMaxFrameSize,
                                           UINT16                 *txMinFrameSize,
                                           UINT16                 *rxMaxFrameSize,
                                           UINT16                 *rxMinFrameSize)
{
#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    mgmt_fege_frame_sizes_get(digi_handle->mgmt_fege_handle, 
                              txMaxFrameSize,
                              txMinFrameSize,
                              rxMaxFrameSize,
                              rxMinFrameSize);
    PMC_ATOMIC_RETURN(digi_handle);
}/* digi_mgmt_fege_frame_sizes_get */



/*******************************************************************************
* digi_enet_client_source_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function has been deprecated and is no longer supported. The
*  function is a stub for a previous interface and performs no access to
*  the device. For future releases this function should be removed from
*  application SW.
*
* INPUTS:
*   *digi_handle         - Unused
*   *chnl_handle         - Unused
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS only.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_client_source_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle)
{
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
} /* digi_enet_client_source_cfg */

/*******************************************************************************
* digi_enet_to_mgmt_fege_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the forwarding of a specific type of packets 
*   from ENET LINE or ENET SYS input interface to management FEGE output interface.
*   Packet could be also filtered out by their VLAN tag.
*   Incoming packets VLAN ID is replaced by a destination VLAN ID when replace_vlan_id
*   is TRUE.
*
*   ENET ss is selected according to DIGI device type: ENET SYS when it is a transponder
*   otherwise ENET LINE is selected.
*
*   Digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   *chnl_handle         - pointer to Enet or mapper channel handle instance.
*   enet_datapath        - ENET datapath: ENET_INGRESS or ENET_EGRESS. 
*   pkt_type             - Packet type.
*   is_vlan_tagged       - TRUE when packet is VLAN-tagged, otherwise FALSE.
*                          Packet VLAN is replaced when is_vlan_tagged is TRUE.
*   *vlan_src_cfg_ptr    - VLAN-tagged packet configuration. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*   enable               - TRUE to enable a forwarding path
*                          FALSE to disable a forwarding path. 
*   vlan_id_repl_en      - enable packet VLAN ID replacement.
*   vlan_id_repl         - VLAN ID value replaced in forwarded packet. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_to_mgmt_fege_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    util_global_pkt_type_t         pkt_type,
                                                    BOOL8                          is_vlan_tagged,
                                                    util_global_vlan_cfg_t        *vlan_src_cfg_ptr,
                                                    BOOL8                          enable,
                                                    BOOL8                          vlan_id_repl_en,
                                                    UINT16                         vlan_id_repl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 vlan_tci_src;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    cpb_mcpb_ingr_input_port_type_t    ingr_port;
    eclass_pkt_extract_cfg_t eclass_extract_cfg;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;

    util_global_switch_data_def_t *chnl_ptr = (util_global_switch_data_def_t *)chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != vlan_src_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* check VLAN_ID range of value */
    if (FALSE ==  digi_enet_mgmt_fege_vlan_id_check(digi_handle, vlan_src_cfg_ptr->vlan_id))
    {
        rc = DIGI_ERR_VLAN_ID_OUT_OF_RANGE;
    }

    if (PMC_SUCCESS == rc && TRUE == enable && TRUE == vlan_id_repl_en)
    {
        /* check VLAN_ID range of value */
        if (FALSE ==  digi_enet_mgmt_fege_vlan_id_check(digi_handle, vlan_id_repl))
        {
            rc = DIGI_ERR_VLAN_ID_OUT_OF_RANGE;
        }
    }

    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                      &enet_handle, &link);
    }
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc && FALSE == digi_enet_is_link_mac_block(enet_handle, link)) 
    {
        rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    }

    if (PMC_SUCCESS == rc)
    {
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);

        /* select ingress port */
        switch (enet_datapath)
        {
            case ENET_INGRESS:
                switch(chnl_ptr->cpb_data.port_type)
                {
                case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                    ingr_port = CPB_MCPB_ING_LINE_IM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_LINE != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                    ingr_port = CPB_MCPB_ING_SYS_IM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_SYS != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                default:
                    rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                }
                break;
            case ENET_EGRESS:
                switch(chnl_ptr->cpb_data.port_type)
                {
                case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                    ingr_port = CPB_MCPB_ING_LINE_EM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_LINE != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                    ingr_port = CPB_MCPB_ING_SYS_EM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_SYS != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                default:
                    rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                }
                break;
            default:
                rc = DIGI_ERR_INVALID_ARG;
                break;
        }
    }


    if (PMC_SUCCESS == rc) 
    {       
        /* Configure ECLASS */    
        eclass_extract_cfg.valid = enable;
        eclass_extract_cfg.pkt_type = pkt_type;
        eclass_extract_cfg.is_vlan = is_vlan_tagged;
        if (TRUE == is_vlan_tagged)
        {
            PMC_MEMCPY(&eclass_extract_cfg.vlan_entry, vlan_src_cfg_ptr, sizeof(util_global_vlan_cfg_t));
        }
    }

    if (PMC_SUCCESS == rc)
    {
        rc = enet_eclass_pkt_extract_cfg(enet_handle,
                                        link,
                                        enet_datapath,
                                        enable,
                                        &eclass_extract_cfg); 
        PMC_ATOMIC_YIELD(digi_handle,0);                                                                  
                                        
    }
    
    /* update FW ENET source if needed, otherwise FW operations will 
       be executed on the wrong ENET source */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_enet_fw_enet_client_source_conditional_update(digi_handle,
                                                                chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {            
        /* build vlan tag */
        vlan_tci_src = (vlan_src_cfg_ptr->vlan_pcp << DIGI_VLAN_PCP_OFFSET) + (vlan_src_cfg_ptr->vlan_cfi << DIGI_VLAN_CFI_OFFSET) + (vlan_src_cfg_ptr->vlan_id); 
        
        /* configure enet classifier */
        rc = digi_fw_eclass_hwire_cfg(digi_handle, 
                                      DIGI_ENET_PKT_FWD_DST_MGMT_FEGE, 
                                      enet_datapath,
                                      link,
                                      TRUE,
                                      enable,
                                      pkt_type,
                                      FALSE,
                                      is_vlan_tagged,
                                      (UINT16)vlan_src_cfg_ptr->vlan_type,
                                      vlan_tci_src);
        PMC_ATOMIC_YIELD(digi_handle,0);                                                            
    }

    if (PMC_SUCCESS == rc)
    {
        UINT32 num_slots = 0;

        rc = digi_cpb_switch_data_slot_get(digi_handle, (util_global_switch_data_def_t *)chnl_handle, &num_slots);

        switch(num_slots)
        {
        case 8:
        {
            /* 10G = 8 entries */
            rate_data = DIGI_CPB_STREAM_10G;
            break;
        }
        case 32:
        {
            /* 40G = 32 entries */
            rate_data = DIGI_CPB_STREAM_40G;
            break;
        }
        case 80:
        {
            /* 100G = 80 entries */
            rate_data = DIGI_CPB_STREAM_100G;
            break;
        } default:
        {
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT,0 ,0);
            break;
        }
        }
    }

    if (PMC_SUCCESS == rc)
    {
        rc = digi_enet_mgmt_fege_ext_chnl_cfg(digi_handle, link,
                                              ingr_port, vlan_src_cfg_ptr,   
                                              enable,
                                              vlan_id_repl_en, vlan_id_repl, 
                                              rate_data);
        PMC_ATOMIC_YIELD(digi_handle,0);                                                                            
        if (MGMT_FEGE_LOG_ERR_DPI_CHNL_ALREADY_PROV == rc)
        {
            rc = PMC_SUCCESS;
        }
    }
        
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_to_mgmt_fege_pkt_fwd_cfg */

/*******************************************************************************
* digi_enet_to_mgmt_fege_pkt_fwd_cfg_single_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function gets a single config setting of a specific type of packets 
*   from ENET LINE or ENET SYS input interface to management FEGE output interface.
*   Packet could be also filtered out by their VLAN tag.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   *chnl_handle          - pointer to Enet or mapper channel handle instance.
*    enet_datapath        - ENET datapath: ENET_INGRESS or ENET_EGRESS.
*    pkt_type             - Packet type.
*   is_vlan_tagged       - TRUE when packet is VLAN-tagged, otherwise FALSE.
*                           Packet VLAN is replaced when is_vlan_tagged is TRUE.
*   *vlan_src_cfg_ptr     - VLAN-tagged packet configuration.
*                           It is ignored when enable or is_vlan_tagged are FALSE.
*
* OUTPUTS:
*   *enable               - TRUE to enable a forwarding path
*                           FALSE to disable a forwarding path.
*   *vlan_id_repl_en      - enable packet VLAN ID replacement.
*   *vlan_id_repl         - VLAN ID value replaced in forwarded packet.
*                           It is ignored when enable or is_vlan_tagged are FALSE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_to_mgmt_fege_pkt_fwd_cfg_single_get(digi_handle_t          *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            enet_dir_t                 enet_datapath,
                                                            util_global_pkt_type_t     pkt_type,
                                                            BOOL8                      is_vlan_tagged,
                                                            util_global_vlan_cfg_t    *vlan_src_cfg_ptr,
                                                            BOOL8                     *enable,
                                                            BOOL8                     *vlan_id_repl_en,
                                                            UINT16                    *vlan_id_repl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT32 array_size = 0;
    UINT32 array_used_size = 0;
    eclass_pkt_extract_cfg_t int_extract_cfg[ECLASS_NUM_SUPPORTED_PKT_PER_LINK];
    UINT32 index = 0;
    cpb_mcpb_ingr_input_port_type_t    ingr_port;
    util_global_switch_data_def_t *chnl_ptr = (util_global_switch_data_def_t *)chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != vlan_src_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (PMC_SUCCESS == rc) 
    {
        rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                      &enet_handle, &link);
    }
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc && FALSE == digi_enet_is_link_mac_block(enet_handle, link)) 
    {
        rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    }

    if (PMC_SUCCESS == rc)
    {
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);

        /* select ingress port */
        switch (enet_datapath)
        {
            case ENET_INGRESS:
                switch(chnl_ptr->cpb_data.port_type)
                {
                case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                    ingr_port = CPB_MCPB_ING_LINE_IM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_LINE != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                    ingr_port = CPB_MCPB_ING_SYS_IM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_SYS != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                default:
                    rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                }
                break;
            case ENET_EGRESS:
                switch(chnl_ptr->cpb_data.port_type)
                {
                case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
                    ingr_port = CPB_MCPB_ING_LINE_EM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_LINE != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
                    ingr_port = CPB_MCPB_ING_SYS_EM_EXT;
                    if (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init &&
                        UTIL_GLOBAL_MCPB_PORT_ENET_SYS != digi_mcpb_enet_port_type_get(digi_handle))
                        rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                default:
                    rc = DIGI_ERR_MCPB_INVALID_PORT;
                    break;
                }
                break;
            default:
                rc = DIGI_ERR_INVALID_ARG;
                break;
        }
    }

    if (PMC_SUCCESS == rc)
    {

        PMC_MEMSET(int_extract_cfg, 0, (sizeof(eclass_pkt_extract_cfg_t)*(ECLASS_NUM_SUPPORTED_PKT_PER_LINK)));

        enet_eclass_pkt_extract_cfg_get(enet_handle, 
                                        link,
                                        enet_datapath,
                                        &array_size,
                                        &array_used_size,
                                        &int_extract_cfg[0]);
        
        *enable = FALSE;
        for (index = 0; index < 28; index++)
        {
            /* Find a precise matched entry */
            if ((pkt_type == int_extract_cfg[index].pkt_type &&
                 vlan_src_cfg_ptr->vlan_type == int_extract_cfg[index].vlan_entry.vlan_type &&
                 vlan_src_cfg_ptr->vlan_pcp  == int_extract_cfg[index].vlan_entry.vlan_pcp &&
                 vlan_src_cfg_ptr->vlan_cfi  == int_extract_cfg[index].vlan_entry.vlan_cfi &&
                 vlan_src_cfg_ptr->vlan_id   == int_extract_cfg[index].vlan_entry.vlan_id &&
                 is_vlan_tagged == 1 && 1 == int_extract_cfg[index].is_vlan) ||
                (pkt_type == int_extract_cfg[index].pkt_type &&
                 is_vlan_tagged == 0 && 0 == int_extract_cfg[index].is_vlan) )
            {
                *enable = int_extract_cfg[index].valid;
                break;
            }
        }
    }

    if (PMC_SUCCESS == rc)
    {
        rc = digi_enet_mgmt_fege_ext_chnl_cfg_get(digi_handle, link, ingr_port,
                                                  vlan_id_repl_en, vlan_id_repl);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_to_mgmt_fege_pkt_fwd_cfg_single_get */

/*******************************************************************************
* digi_mgmt_fege_to_cpu_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   input interface to CPU output interface. Packets are
*   filtered in by their VLAN ID.  
*
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   enable                  - TRUE to enable a forwarding path.
*                             FALSE to disable a forwarding path. 
* 
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mgmt_fege_to_cpu_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                   util_global_vlan_cfg_t        *vlan_cfg_ptr,
                                                   BOOL8                          enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    cpb_mcpb_egr_output_port_type_t    mcpb_egr_port;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    mcpb_egr_port = CPB_MCPB_EGR_CPU;
    
    rc = digi_enet_mgmt_fege_ins_chnl_cfg(digi_handle, 
                                          0xffffffff,
                                          mcpb_egr_port,
                                          vlan_cfg_ptr,
                                          enable,
                                          TRUE,
                                          rate_data,
                                          FALSE);
    
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_mgmt_fege_to_cpu_pkt_fwd_cfg */

/*******************************************************************************
* digi_mgmt_fege_to_cpu_pkt_fwd_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function gets packets forwarding from management FEGE 
*   input interface to CPU output interface. Packets are
*   filtered in by their VLAN ID.  
*
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance.
* 
* OUTPUTS:
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   *enable                  - TRUE to enable a forwarding path.
*                             FALSE to disable a forwarding path. 
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mgmt_fege_to_cpu_pkt_fwd_cfg_get(digi_handle_t          *digi_handle,
                                                       util_global_vlan_cfg_t *vlan_cfg_ptr,
                                                       BOOL8                  *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 priority_en = FALSE;
    cpb_mcpb_egr_output_port_type_t    mcpb_egr_port;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    mcpb_egr_port = CPB_MCPB_EGR_CPU;

    rc = digi_enet_mgmt_fege_ins_chnl_cfg_get(digi_handle, 
                                              0xffffffff,
                                              mcpb_egr_port,
                                              vlan_cfg_ptr,
                                              enable,
                                              &priority_en);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_mgmt_fege_to_cpu_pkt_fwd_cfg_get */


/*******************************************************************************
* digi_cpu_to_mgmt_fege_pkt_fwd_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables the forwarding of packets from CPU
*   to management FEGE output interface.
*   Packets VLAN ID is replaced by a destination VLAN ID when replace_vlan_id
*   is TRUE.
*
*   Digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance.
*   *vlan_src_cfg_ptr    - VLAN-tagged packet configuration. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
*   enable               - TRUE to enable a forwarding path
*                          FALSE to disable a forwarding path. 
*   vlan_id_repl_en      - enable packet VLAN ID replacement.
*   vlan_id_repl          - VLAN ID value replaced in forwarded packet. 
*                          It is ignored when enable or is_vlan_tagged are FALSE.
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_cpu_to_mgmt_fege_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                   util_global_vlan_cfg_t        *vlan_src_cfg_ptr,
                                                   BOOL8                          enable,
                                                   BOOL8                          vlan_id_repl_en,
                                                   UINT16                         vlan_id_repl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    cpb_mcpb_ingr_input_port_type_t    ingr_port;
    digi_cpb_stream_t rate_data = DIGI_CPB_STREAM_10G;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != vlan_src_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* check VLAN_ID range of value */
    if (FALSE == digi_enet_mgmt_fege_vlan_id_check(digi_handle, vlan_src_cfg_ptr->vlan_id))
    {
        rc = DIGI_ERR_VLAN_ID_OUT_OF_RANGE;
    }

    if (TRUE == enable && TRUE == vlan_id_repl_en)
    {
        /* check VLAN_ID range of value */
        if (FALSE ==  digi_enet_mgmt_fege_vlan_id_check(digi_handle, vlan_id_repl))
        {
            rc = DIGI_ERR_VLAN_ID_OUT_OF_RANGE;
        }
    }

    if (PMC_SUCCESS == rc)
    {             
        ingr_port = CPB_MCPB_ING_CPU_INS;
        
        rc = digi_enet_mgmt_fege_ext_chnl_cfg(digi_handle, 
                                              0xffffffff,
                                              ingr_port, 
                                              vlan_src_cfg_ptr,   
                                              enable,
                                              vlan_id_repl_en, 
                                              vlan_id_repl, 
                                              rate_data);
    }
    
    if (MGMT_FEGE_LOG_ERR_DPI_CHNL_ALREADY_PROV == rc)
    {
        rc = PMC_SUCCESS;
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cpu_to_mgmt_fege_pkt_fwd_cfg */

/*******************************************************************************
* digi_enet_mgmt_pkt_ext_pmon_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables/disables the monitoring of the number of
*   ENET ingress or egress management packets with a particular classification 
*   code that are sent to the CPU. \n
*
*   The extract PMONs must be latched before reading. The latching can either 
*   be done by firmware or by calling the digi_enet_empei_pmon_latch 
*   function. If firmware is used to latch PMONs, the extract PMONs can be 
*   read by calling the digi_enet_mgmt_pkt_ext_pmon_get function from the 
*   PMON interrupt callback routine. If the extract counters are manually
*   latched, the digi_enet_mgmt_pkt_ext_pmon_get function can be called to
*   retrieve the enabled counters.\n
*
*   The chnl_handle can either be an ENET client channel handle or a mapper
*   channel handle depending on how the data path is provisioned.
*
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   *chnl_handle     - pointer to Enet or mapper channel handle instance
*   direction        - DIGI_ENET_EGRESS or DIGI_ENET_INGRESS
*   tag              - classification code 
*   enable           - enable or disable the monitoring of the ENET 
*                      extract PMONs.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS or an error code.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_pkt_ext_pmon_cfg(digi_handle_t                             *digi_handle,
                                                 util_global_switch_data_t                 *chnl_handle,        
                                                 digi_enet_classification_direction_mode_t  direction,
                                                 UINT32                                     tag,
                                                 BOOL                                       enable)
{
    PMC_ERROR rc;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if(PMC_SUCCESS == rc) {
        if (direction == DIGI_ENET_EGRESS)
        {
            rc = (enable ? empei_e_ext_pmon_enable(enet_handle->empei_e_handle, link,tag)
                  : empei_e_ext_pmon_disable(enet_handle->empei_e_handle, link,tag));
        } 
        else 
        {
            rc = (enable ? empei_i_ext_pmon_enable(enet_handle->empei_i_handle, link,tag)
                  : empei_i_ext_pmon_disable(enet_handle->empei_i_handle, link,tag));
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /*digi_enet_mgmt_pkt_ext_pmon_cfg*/

/*******************************************************************************
* digi_enet_mgmt_pkt_ext_pmon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the ethernet management packet extract PMONs that 
*   correspond to the channel indicated by the chnl_handle parameter.
*   The PMONs to be read are enabled by the digi_enet_mgmt_pkt_ext_pmon_cfg 
*   function. The counters must be latched either by firmware or manually 
*   by calling the digi_enet_empei_pmon_latch function before calling this
*   function. \n
*
*   The PMON values are stored in an array pointed to by counters_pptr.
*   The returned tags_pptr array indicates which classification code the
*   corresponding entry in the counters_pptr applies to. Both arrays are of
*   size DIGI_ENET_EXT_PMON_MAX. If an entry in tags_pptr is 0, then the
*   corresponding entry in the counters_pptr is not applicable to this 
*   channel. For example, if the following values are returned:\n
*       counters_pptr[0] = 5; tags_pptr[0] = 2;\n
*       counters_pptr[1] = x; tags_pttr[1] = 0;\n
*      
*   Then the number of packets with classification code 2 is 5 and the
*   second entry is not applicable to this channel.\n
*       
*   The egress counters are retrieved when direction = DIGI_ENET_EGRESS; 
*   otherwise the ingress counters are stored in counters_pptr.  \n
*
*   The accum_mode parameter determines whether the counter values
*   are reset or accumulated between calls to this function. \n
*             
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_handle    - digi_enet_chnl_handle_t or digi_mapper_chnl_t
*   direction       - DIGI_ENET_EGRESS or DIGI_ENET_INGRESS
*   accum_mode      - Counter values will accumulate if TRUE.
*
* OUTPUTS:
*   **counters_pptr - Pointer to an array of counters. An entry in the array is 
*                     valid if the corresponding entry in the tags_ptr contains
*                     a tag (tag > 0).
*   **tags_pptr     - Pointer to an array of tags. Each entry contains the tag
*                     for the corresponding counter stored in counters_ptr.
*
* RETURNS:
*   PMC_SUCCESS or an error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_pkt_ext_pmon_get(digi_handle_t                              *digi_handle, 
                                                 util_global_switch_data_t                  *chnl_handle,        
                                                 digi_enet_classification_direction_mode_t   direction,
                                                 BOOL                                        accum_mode,
                                                 UINT32                                    **counters_pptr,
                                                 UINT8                                     **tags_pptr)
{
    PMC_ERROR rc;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (util_global_switch_data_def_t *) chnl_handle);
    }

    if (PMC_SUCCESS == rc) {
        if (direction == DIGI_ENET_EGRESS)
        {
            rc = empei_e_ext_pmon_get(enet_handle->empei_e_handle, link,accum_mode,counters_pptr,tags_pptr);
        } 
        else 
        {
            rc = empei_i_ext_pmon_get(enet_handle->empei_i_handle, link,accum_mode,counters_pptr,tags_pptr);
        } 
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /*digi_enet_mgmt_pkt_ext_pmon_get*/

/*******************************************************************************
* digi_enet_mgmt_pkt_ext_pmon_disable_all
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function disables the extract counters for all ENET ingress 
*   or egress management packets for the channel indicated by the chnl_handle 
*   parameter.
*
*   The chnl_handle can either be an ENET client channel handle or a mapper
*   channel handle depending on how the data path is provisioned.
*
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   *chnl_handle     - pointer to Enet or mapper channel handle instance
*   direction        - classification direction (ingress/egress)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS or an error code.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_pkt_ext_pmon_disable_all(digi_handle_t *digi_handle,
                                                         util_global_switch_data_t *chnl_handle,        
                                                         digi_enet_classification_direction_mode_t direction)
{
    PMC_ERROR rc;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc) {
        if (direction == DIGI_ENET_EGRESS)
        {
            rc = empei_e_ext_pmon_disable_all(enet_handle->empei_e_handle, link);
        } 
        else 
        {
            rc = empei_i_ext_pmon_disable_all(enet_handle->empei_i_handle, link);
        } 
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mgmt_pkt_ext_pmon_disable_all */

/*******************************************************************************
* digi_enet_empei_pmon_latch
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function latches all EMPEI PMONs. It should not be called
*   if firmware is configured to read PMONs.
*
*   This function can be called to latch the EMPEI extract PMONs before calling
*   digi_enet_mgmt_pkt_ext_pmon_get to read the counters. This function will
*   latch PMONs for all channels, therefore the digi_enet_mgmt_pkt_ext_pmon_get
*   function should be called once for each channel with enabled extract PMONs.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
*   *chnl_handle     - pointer to Enet or mapper channel handle instance
*   direction        - Ingress or Egress EMPEI
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS or an error code.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_empei_pmon_latch(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,        
                                            digi_enet_classification_direction_mode_t direction)
{
    PMC_ERROR rc;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rc) {
        if (direction == DIGI_ENET_EGRESS)
        {
            rc = empei_e_pmon_latch(enet_handle->empei_e_handle);
        } 
        else 
        {
            rc = empei_i_pmon_latch(enet_handle->empei_i_handle);
        } 
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_empei_pmon_latch */




/*******************************************************************************
* FUNCTION: digi_enet_line_pmon_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation PMON table with PMON counters 
*   over multiple channels.
*
*   digi_enet_pmon_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to aggregation PMON table to store counters 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_pmon_retrieve(digi_handle_t      *digi_handle,
                                              digi_enet_pmon_t   *pmon_table_ptr)
{
    PMC_ERROR                       result = PMC_SUCCESS;
    digi_enet_client_chnl_t        *chnl_handle;
    digi_enet_client_chnl_def_t    *chnl_def_handle;
    UINT32                          i;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_enet_pmon_t));

        for (i = 0; (i < DIGI_ENET_CHNL_MAX) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = (digi_enet_client_chnl_t *) &digi_handle->handle_pool.enet_line_chnl[i];
            chnl_def_handle = (digi_enet_client_chnl_def_t *) chnl_handle;
           
            if (util_global_is_chnl_prov(chnl_def_handle))
            {
                pmon_table_ptr->handle[i] = chnl_handle;
                result = digi_enet_pmon_chnl_retrieve_internal(digi_handle,
                                                               chnl_handle,
                                                               TRUE,
                                                               &pmon_table_ptr->enet_chnl[i]);
            }
        } /* end for */
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_pmon_retrieve */


/*******************************************************************************
* FUNCTION: digi_enet_sys_pmon_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation PMON table with PMON counters 
*   over multiple channels.
*
*   digi_enet_pmon_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to aggregation PMON table to store counters 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_pmon_retrieve(digi_handle_t      *digi_handle,
                                             digi_enet_pmon_t   *pmon_table_ptr)
{
    PMC_ERROR                       result = PMC_SUCCESS;
    digi_enet_client_chnl_t        *chnl_handle;
    digi_enet_client_chnl_def_t    *chnl_def_handle;
    UINT32                          i;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_enet_pmon_t));

        for (i = 0; (i < DIGI_ENET_CHNL_MAX) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = (digi_enet_client_chnl_t *) &digi_handle->handle_pool.enet_sys_chnl[i];
            chnl_def_handle = (digi_enet_client_chnl_def_t *) chnl_handle;
           
            if (util_global_is_chnl_prov(chnl_def_handle))
            {
                pmon_table_ptr->handle[i] = chnl_handle;
                result = digi_enet_pmon_chnl_retrieve_internal(digi_handle,
                                                               chnl_handle,
                                                               FALSE,
                                                               &pmon_table_ptr->enet_chnl[i]);
            }
        } /* end for */
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_pmon_retrieve */

/*******************************************************************************
* FUNCTION: digi_enet_pmon_chnl_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated PMON table, retrieve a pointer
*   to the corresponding channel PMON table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*   pmon_table_ptr  - Aggregated PMON table
*
* OUTPUTS:
*   pmon_chnl_table_pptr - pointer to channel PMON table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_pmon_chnl_get(digi_handle_t                *digi_handle,
                                         digi_enet_client_chnl_t      *chnl_handle,
                                         digi_enet_pmon_t             *pmon_table_ptr,
                                         digi_enet_pmon_chnl_t       **pmon_chnl_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_chnl_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Set return table to NULL */
    *pmon_chnl_table_pptr = NULL;

    /* Get index corresponding to handle */
    result = digi_enet_channel_num_get(digi_handle, chnl_handle, &index);

    if (PMC_SUCCESS == result)
    {
        /* Set return table to channel table */
        if (pmon_table_ptr->handle[index] == chnl_handle)
        {
            *pmon_chnl_table_pptr = &pmon_table_ptr->enet_chnl[index];
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_pmon_chnl_get */

/*******************************************************************************
* FUNCTION: digi_enet_enhanced_pmon_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel PMON table with PMON counters
*   for a single channel in the subsystem.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to mapper or interlaken channel handle to retrieve
*                     ENET enhanced PMON on
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to channel PMON table to store counters 
*                     for a single channel
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_chnl_retrieve(digi_handle_t                  *digi_handle,
                                                       util_global_switch_data_t      *chnl_handle,
                                                       digi_enet_pmon_chnl_t          *pmon_table_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    digi_enet_pmon_mode_t        enet_mode;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check if channel handle is mapper or interlaken */
    if (!DIGI_CPB_PORT_TYPE_IS_MAPOTN(chnl_handle) &&
        !DIGI_CPB_PORT_TYPE_IS_ILKN(chnl_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INCORRECT_HANDLE_FOR_ENH_PMON);
    }
    
    result = digi_enet_enhanced_pmon_chnl_retrieve_internal(digi_handle,
                                                            chnl_handle,
                                                            pmon_table_ptr,
                                                            &enet_mode);

    if ((PMC_SUCCESS == result) &&
        (DIGI_ENET_PMON_MODE_NONE == enet_mode))
    {
        
        result = DIGI_ERR_NO_ENH_PMON;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_enhanced_pmon_chnl_retrieve */

/*******************************************************************************
* FUNCTION: digi_enet_mgmt_fege_pmon_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the MGMT_FEGE port PMON from shared memory aggregated interrupt
*   table.
*
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to aggregation PMON table to store counters 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_pmon_retrieve(digi_handle_t       *digi_handle,
                                                   digi_pmon_coreotn_fege_mstat_t    *pmon_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_pmon_t             *fw_pmon_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr); 
    }

    if (PMC_SUCCESS == result)
    {        
        PMC_MEMCPY(pmon_table_ptr,
                   &fw_pmon_ptr->mgmt_pmon,
                   sizeof(digi_pmon_coreotn_fege_mstat_t));
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_mgmt_fege_pmon_retrieve */


/*******************************************************************************
* FUNCTION: digi_enet_line_int_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   ENET channel.
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one channel.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_register_cb(digi_handle_t      *digi_handle,
                                                void               *token,
                                                UINT32              token_size,
                                                digi_int_cb_fcn     user_cb)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_register_cb(digi_handle,
                                       DIGI_INT_TYPE_ENET_LINE,
                                       token,
                                       token_size,
                                       user_cb);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_int_register_cb */

/*******************************************************************************
* FUNCTION: digi_enet_line_int_chnl_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for channel interrupt table processing.
*
*   For each ENET line channel where an active interrupt is found, the callback
*   will be called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_chnl_register_cb(digi_handle_t      *digi_handle,
                                                     void               *token,
                                                     UINT32              token_size,
                                                     digi_int_cb_fcn     user_cb)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_ENET_LINE;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_int_chnl_register_cb */



/*******************************************************************************
* FUNCTION: digi_enet_line_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_chnl_enable(digi_handle_t             *digi_handle,
                                                digi_enet_client_chnl_t   *chnl_handle,   
                                                enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_enable(digi_handle,
                                       DIGI_INT_TYPE_ENET_LINE,
                                       chnl_handle,
                                       int_table_ptr,
                                       DIGI_ENABLE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_int_chnl_enable */



/*******************************************************************************
* FUNCTION: digi_enet_line_int_chnl_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_chnl_disable(digi_handle_t             *digi_handle,
                                                 digi_enet_client_chnl_t   *chnl_handle,   
                                                 enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_enable(digi_handle,
                                       DIGI_INT_TYPE_ENET_LINE,
                                       chnl_handle,
                                       int_table_ptr,
                                       DIGI_DISABLE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_int_chnl_disable */



/*******************************************************************************
* FUNCTION: digi_enet_line_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.
*                     DIGI_INT_ALL_INTS to clear all interrupts.
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_clear(digi_handle_t             *digi_handle,
                                          digi_enet_int_t           *int_table_ptr)
{
    PMC_ERROR   result;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_clear(digi_handle,
                                 DIGI_INT_TYPE_ENET_LINE,
                                 int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_line_int_clear */


/*******************************************************************************
* FUNCTION: digi_enet_line_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears link interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.
*                     DIGI_INT_ALL_INTS to clear all interrupts.
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_chnl_clear(digi_handle_t             *digi_handle,
                                               digi_enet_client_chnl_t   *chnl_handle,
                                               digi_enet_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR   result;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_clear(digi_handle,
                                      DIGI_INT_TYPE_ENET_LINE,
                                      chnl_handle,
                                      int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle, result);
    
} /* digi_enet_line_int_chnl_clear */


/*******************************************************************************
* FUNCTION: digi_enet_line_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_retrieve(digi_handle_t       *digi_handle,
                                             digi_enet_int_t     *int_table_ptr)
{
    PMC_ERROR   result;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_filt_retrieve(digi_handle,
                                         DIGI_INT_TYPE_ENET_LINE,
                                         DIGI_INT_ALL_INTS,
                                         int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_line_int_retrieve */


/*******************************************************************************
* FUNCTION: digi_enet_line_int_filt_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*   
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_filt_retrieve(digi_handle_t        *digi_handle,
                                                  digi_enet_int_chnl_t *filt_table_ptr,
                                                  digi_enet_int_t      *int_table_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    
    result = digi_enet_int_filt_retrieve(digi_handle,
                                         DIGI_INT_TYPE_ENET_LINE,
                                         filt_table_ptr,
                                         int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_int_filt_retrieve */



/*******************************************************************************
* FUNCTION: digi_enet_line_int_summary_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of ENET LINE interrupts.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   summary_mask    - Bit mask of bits from digi_enet_int_summary_mask_t
*                     to indicate what summary information to retrieve.
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_summary_get(digi_handle_t              *digi_handle,
                                                UINT32                      summary_mask,
                                                digi_enet_int_summary_t    *int_summary_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_summary_get_internal(digi_handle,
                                                DIGI_INT_TYPE_ENET_LINE,
                                                0, /* unused */
                                                int_summary_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_line_int_summary_get */



/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated interrupt table, retrieve a pointer
*   to the corresponding channel interrupt table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   int_table_ptr   - Aggregated interrupt table
*
* OUTPUTS:
*   int_chnl_table_pptr  - pointer to channel interrupt table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_int_chnl_get(digi_handle_t                *digi_handle,
                                        digi_enet_client_chnl_t      *chnl_handle,
                                        digi_enet_int_t              *int_table_ptr,
                                        digi_enet_int_chnl_t        **int_chnl_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_chnl_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Set return table to NULL */
    *int_chnl_table_pptr = NULL;

    /* Get index corresponding to handle */
    result = digi_enet_channel_num_get(digi_handle, chnl_handle, &index);

    if (PMC_SUCCESS == result)
    {
        /* Set return table to channel table */
        if (int_table_ptr->handle[index] == chnl_handle)
        {
            *int_chnl_table_pptr = &int_table_ptr->enet_chnl[index];
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_int_chnl_get */

/*******************************************************************************
* FUNCTION: digi_enet_line_int_enabled_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with enabled interrupt bits
*   over multiple channels.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_enabled_get(digi_handle_t       *digi_handle,
                                                digi_enet_int_t     *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_enabled_get(digi_handle,
                                       DIGI_INT_TYPE_ENET_LINE,
                                       int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_line_int_enabled_get */

/*******************************************************************************
* FUNCTION: digi_enet_line_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_enabled_check(digi_handle_t      *digi_handle)
{
    PMC_ERROR   result;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_line_int_enabled_check_internal(digi_handle);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_line_int_enabled_check */


/*******************************************************************************
* FUNCTION: digi_enet_line_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   A callback must be registered for the interrupt type before this API can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_summary_ptr - If digi_enet_line_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                       digi_enet_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    PMC_ATOMIC_ENTRY(digi_handle);


    result = digi_enet_int_chnl_enabled_check(digi_handle,
                                              DIGI_INT_CHNL_TYPE_ENET_LINE,
                                              int_summary_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_line_int_chnl_enabled_check */


/*******************************************************************************
* FUNCTION: digi_enet_sys_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   A callback must be registered for the interrupt type before this API can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_summary_ptr - If digi_enet_sys_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                      digi_enet_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_enabled_check(digi_handle,
                                              DIGI_INT_CHNL_TYPE_ENET_SYS,
                                              int_summary_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_sys_int_chnl_enabled_check */

/*******************************************************************************
* FUNCTION: digi_enet_sys_int_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   ENET channel.
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one channel.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_register_cb(digi_handle_t      *digi_handle,
                                               void               *token,
                                               UINT32              token_size,
                                               digi_int_cb_fcn     user_cb)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_register_cb(digi_handle,
                                       DIGI_INT_TYPE_ENET_SYS,
                                       token,
                                       token_size,
                                       user_cb);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_int_register_cb */


/*******************************************************************************
* FUNCTION: digi_enet_sys_int_chnl_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for channel interrupt table processing.
*
*   For each ENET sys channel where an active interrupt is found, the callback
*   will be called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_chnl_register_cb(digi_handle_t      *digi_handle,
                                                    void               *token,
                                                    UINT32              token_size,
                                                    digi_int_cb_fcn     user_cb)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_ENET_SYS;
    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_int_chnl_register_cb */


/*******************************************************************************
* FUNCTION: digi_enet_sys_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_chnl_enable(digi_handle_t             *digi_handle,
                                               digi_enet_client_chnl_t   *chnl_handle,   
                                               enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_enable(digi_handle,
                                       DIGI_INT_TYPE_ENET_SYS,
                                       chnl_handle,
                                       int_table_ptr,
                                       DIGI_ENABLE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_int_chnl_enable */



/*******************************************************************************
* FUNCTION: digi_enet_sys_int_chnl_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_chnl_disable(digi_handle_t             *digi_handle,
                                                digi_enet_client_chnl_t   *chnl_handle,   
                                                enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_enable(digi_handle,
                                       DIGI_INT_TYPE_ENET_SYS,
                                       chnl_handle,
                                       int_table_ptr,
                                       DIGI_DISABLE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_int_chnl_disable */



/*******************************************************************************
* FUNCTION: digi_enet_sys_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.
*                     DIGI_INT_ALL_INTS to clear all interrupts.
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_clear(digi_handle_t             *digi_handle,
                                         digi_enet_int_t           *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_clear(digi_handle,
                                 DIGI_INT_TYPE_ENET_SYS,
                                 int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_sys_int_clear */


/*******************************************************************************
* FUNCTION: digi_enet_sys_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears link interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.
*                     DIGI_INT_ALL_INTS to clear all interrupts.
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_chnl_clear(digi_handle_t             *digi_handle,
                                              digi_enet_client_chnl_t   *chnl_handle,
                                              digi_enet_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR   result;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_clear(digi_handle,
                                      DIGI_INT_TYPE_ENET_SYS,
                                      chnl_handle,
                                      int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle, result);
    
} /* digi_enet_sys_int_chnl_clear */


/*******************************************************************************
* FUNCTION: digi_enet_sys_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_retrieve(digi_handle_t       *digi_handle,
                                            digi_enet_int_t     *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_filt_retrieve(digi_handle,
                                         DIGI_INT_TYPE_ENET_SYS,
                                         DIGI_INT_ALL_INTS,
                                         int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_sys_int_retrieve */

/*******************************************************************************
* FUNCTION: digi_enet_sys_int_filt_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*   
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_filt_retrieve(digi_handle_t        *digi_handle,
                                                 digi_enet_int_chnl_t *filt_table_ptr,
                                                 digi_enet_int_t      *int_table_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_enet_int_filt_retrieve(digi_handle,
                                         DIGI_INT_TYPE_ENET_SYS,
                                         filt_table_ptr,
                                         int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_int_filt_retrieve */



/*******************************************************************************
* FUNCTION: digi_enet_sys_int_summary_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of ENET SYS interrupts.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   summary_mask    - Bit mask of bits from digi_enet_int_summary_mask_t
*                     to indicate what summary information to retrieve.
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_summary_get(digi_handle_t              *digi_handle,
                                               UINT32                      summary_mask,
                                               digi_enet_int_summary_t    *int_summary_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_summary_get_internal(digi_handle,
                                                DIGI_INT_TYPE_ENET_SYS,
                                                0, /* unused */
                                                int_summary_ptr);


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_sys_int_summary_get */



/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_filt_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an channel interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*   
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_int_chnl_filt_retrieve(digi_handle_t             *digi_handle,
                                                  digi_enet_client_chnl_t   *chnl_handle,   
                                                  digi_enet_int_chnl_t      *filt_table_ptr,
                                                  digi_enet_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_chnl_filt_retrieve_internal(digi_handle,
                                                       chnl_handle,   
                                                       filt_table_ptr,
                                                       FALSE,
                                                       int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_int_chnl_filt_retrieve */


/*******************************************************************************
* FUNCTION: digi_enet_sys_int_enabled_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with enabled interrupt bits
*   over multiple channels.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_enabled_get(digi_handle_t       *digi_handle,
                                               digi_enet_int_t     *int_table_ptr)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_int_enabled_get(digi_handle,
                                       DIGI_INT_TYPE_ENET_SYS,
                                       int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_sys_int_enabled_get */

/*******************************************************************************
* FUNCTION: digi_enet_sys_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_enabled_check(digi_handle_t      *digi_handle)
{
    PMC_ERROR   result;
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_enet_sys_int_enabled_check_internal(digi_handle);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_sys_int_enabled_check */

/*******************************************************************************
* FUNCTION: digi_enet_mgmt_fege_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables MGMT_FEGE interrupts based on bits that are set in the interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_enable(digi_handle_t             *digi_handle,  
                                                mgmt_fege_int_t           *int_table_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_DEVICE;
    digi_device_int_t  *int_en_table_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_DEVICE))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }    

    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    if (PMC_SUCCESS == result)
    {
        result =  digi_enet_mgmt_fege_int_internal_enable(digi_handle, 
                                                          int_table_ptr,
                                                          &int_en_table_ptr->mgmt_fege,
                                                          DIGI_ENABLE);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_mgmt_fege_int_enable */

/*******************************************************************************
* FUNCTION: digi_enet_mgmt_fege_int_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables MGMT_FEGE interrupts based on bits that are set in the interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_disable(digi_handle_t             *digi_handle, 
                                                 mgmt_fege_int_t           *int_table_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_DEVICE;
    digi_device_int_t  *int_en_table_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }  

    if (PMC_SUCCESS == result)
    {
        result = mgmt_fege_int_enable(digi_handle->mgmt_fege_handle,
                                      int_table_ptr,
                                      &int_en_table_ptr->mgmt_fege,
                                      DIGI_DISABLE);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_mgmt_fege_int_disable */


/*******************************************************************************
* FUNCTION: digi_enet_mgmt_fege_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears MGMT_FEGE interrupts based on bits that are set in the interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_clear(digi_handle_t             *digi_handle,
                                               mgmt_fege_int_t           *int_table_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    digi_device_int_t  *int_en_table_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);

    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_DEVICE,
                                       (void**) &int_en_table_ptr);
    }
    if (PMC_SUCCESS == result)
    {
        result = mgmt_fege_int_clear(digi_handle->mgmt_fege_handle,
                                     int_table_ptr,
                                     &int_en_table_ptr->mgmt_fege);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_mgmt_fege_int_clear */


/*******************************************************************************
* FUNCTION: digi_enet_mgmt_fege_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_enet_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_retrieve(digi_handle_t       *digi_handle,
                                                  mgmt_fege_int_t     *int_table_ptr)
{
    PMC_ERROR   result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);

    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = mgmt_fege_int_retrieve(digi_handle->mgmt_fege_handle,
                                        NULL,
                                        int_table_ptr);
    }

    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);

    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_enet_mgmt_fege_int_retrieve */


/*******************************************************************************
* digi_enet_emac_errored_frames_drop_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable EMAC ingress errored frames drop in DCPB for a given type 
*   of error. This is a global ENET configuration.
*
* INPUTS:
*   digi_handle   - pointer to DIGI handle instance.
*   enet_line     - If TRUE, configure ENET LINE. Otherwise configure ENET SYS.
*   error_type    - type of error: FCS, packet length, LF, RF and vlan's.  
*   enable        - when TRUE EMAC is configured to drop errored frames.\n
*                   when FALSE EMAC is configured to forward errored frames.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/  
PUBLIC PMC_ERROR digi_enet_emac_errored_frames_drop_cfg(digi_handle_t   *digi_handle,
                                                        BOOL8           enet_line,
                                                        emac_error_t    error_type,
                                                        BOOL8           enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    enet_handle = (enet_line ? digi_handle->enet_line_handle : digi_handle->enet_sys_handle);

    emac_errored_frames_drop_cfg(enet_handle->emac_handle, error_type, enable);

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /*digi_enet_emac_errored_frames_drop_cfg */

/*******************************************************************************
* digi_enet_emac_errored_frames_drop_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable EMAC ingress errored frames drop in DCPB for a given type 
*   of error. This is a global ENET configuration.
*
* INPUTS:
*   digi_handle   - pointer to DIGI handle instance.
*   enet_line     - If TRUE, configure ENET LINE. Otherwise configure ENET SYS.
*   error_type    - type of error: FCS, packet length, LF, RF and vlan's.  
*   *enabled      - when TRUE EMAC is configured to drop errored frames.\n
*                   when FALSE EMAC is configured to forward errored frames.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/  
PUBLIC PMC_ERROR digi_enet_emac_errored_frames_drop_get(digi_handle_t   *digi_handle,
                                                        BOOL8           enet_line,
                                                        emac_error_t    error_type,
                                                        BOOL8           *enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    enet_handle = (enet_line ? digi_handle->enet_line_handle : digi_handle->enet_sys_handle);

    *enabled = emac_errored_frames_drop_get(enet_handle->emac_handle, error_type);

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /*digi_enet_emac_errored_frames_drop_get */

/*******************************************************************************
* digi_enet_emac_errored_frames_dpi_error_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enable or disable EMAC ingress errored frame drop in DCPB for a given type 
*   of error. This API configures settings common to all links. 
*
* INPUTS:
*   digi_handle    - pointer to DIGI handle instance.
*   enet_line      - If TRUE configure ENET LINE, otherwise configure ENET SYS.
*   error_type     - type of error: FCS, packet length, LF, RF and vlan's.  
*   enable         - when TRUE EMAC is configured to drop FCS errored frames.\n
*                    when FALSE EMAC is configured to forward FCS errored frames.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/  
PUBLIC PMC_ERROR digi_enet_emac_errored_frames_dpi_error_cfg(digi_handle_t   *digi_handle,
                                                             BOOL8           enet_line,
                                                             emac_error_t    error_type,
                                                             BOOL8           enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    enet_handle = (enet_line ? digi_handle->enet_line_handle : digi_handle->enet_sys_handle);

    emac_errored_frames_dpi_error_cfg(enet_handle->emac_handle, error_type, enable);

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /*digi_enet_emac_errored_frames_dpi_error_cfg */

/*******************************************************************************
* digi_enet_emac_errored_frames_dpi_error_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the current configuration whether errored frames will be treated 
*   as DPI errors for a given error type. This setting is common to all links. 
*
* INPUTS:
*   digi_handle    - pointer to DIGI handle instance.
*   enet_line      - If TRUE configure ENET LINE, otherwise configure ENET SYS.
*   error_type     - type of error: FCS, packet length, LF, RF and vlan's.  
*
* OUTPUTS:
*   *enabled       - when TRUE EMAC is configured to drop FCS errored frames.\n
*                    when FALSE EMAC is configured to forward FCS errored frames.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/  
PUBLIC PMC_ERROR digi_enet_emac_errored_frames_dpi_error_get(digi_handle_t *digi_handle,
                                                            BOOL8           enet_line,
                                                            emac_error_t    error_type,
                                                            BOOL8           *enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    enet_handle = (enet_line ? digi_handle->enet_line_handle : digi_handle->enet_sys_handle);

    *enabled = emac_errored_frames_dpi_error_get(enet_handle->emac_handle, error_type);

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /*digi_enet_emac_errored_frames_dpi_error_get */


/*******************************************************************************
*  digi_enet_emac_ingr_dpi_err_mask_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configure the EMAC Ingress Packet FCS and length error mask. If fcs_mask
*   is true, an fcs error on the link will not result in an ingress DPI error or
*   DPI drop indication. If len_mask is true, a length error will not assert a
*   DPI error or drop indication.
*   
*  The err_cfg_t parameter is one of the following values \n
*   EMAC_DPI_ERR_FCS_ENABLE  - FCS error may contribute to DPI error/drop \n
*   EMAC_DPI_ERR_FCS_DISABLE - FCS error will not contribute \n
*   EMAC_DPI_ERR_LEN_ENABLE  - Length error may contribute to DPI error/drop \n
*   EMAC_DPI_ERR_lEN_DISABLE - Length error will not contribute \n

* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance.
*   *chnl_handle   - pointer to Enet or mapper channel handle instance
*   err_cfg        - see above or emac_dpi_err_cfg_t
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*   1) EMAC_DPI_ERR_FCS_ENABLE results in a value of 0 being written
*   to the EMAC_DPI_FCS_MASK register field (i.e. enable error propagation).
*   2) EMAC_DPI_ERR_LEN_ENABLE results in a value of 0 being written
*   to the EMAC_DPI_LEN_MASK register field (i.e. enable error propagation).
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_emac_ingr_dpi_err_mask_cfg(digi_handle_t  *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      emac_dpi_err_cfg_t err_cfg)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate = (emac_data_rate_t)0;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link)) 
        {
            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
            if (PMC_SUCCESS == rc)
            {
                rc = emac_ingr_dpi_err_mask_cfg(enet_handle->emac_handle, link, 
                                                data_rate, err_cfg);
            }
        }
        else
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }
    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_emac_ingr_dpi_err_mask_cfg  */

/*******************************************************************************
*  digi_enet_emac_ingr_dpi_err_mask_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the current configuration of the EMAC Ingress Packet FCS and 
*   length error mask. If fcs_mask is true, an fcs error on the link will not 
*   assert an ingress DPI error or drop indication. If len_mask is true, 
*   a length error will not assert a DPI error or drop indication.
*
* INPUTS:
*   digi_handle         - handle to the EMAC instance to be operated on
*   *chnl_handle        - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *fcs_mask_ptr       - If TRUE, fcs errors do not contribute to 
*                         DPI error/drop. 
*   *len_mask_ptr       - If TRUE, length error do not contribute to  
*                         DPI error/drop. 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_emac_ingr_dpi_err_mask_get(digi_handle_t  *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      BOOL8   *fcs_mask_ptr,
                                                      BOOL8   *len_mask_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate = (emac_data_rate_t)0;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link)) 
        {
            rc = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
            if (PMC_SUCCESS == rc)
            {
                rc = emac_ingr_dpi_err_mask_get(enet_handle->emac_handle, link, 
                                                data_rate, fcs_mask_ptr, len_mask_ptr);
            }
        }
        else
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_emac_ingr_dpi_err_mask_get */

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

/*******************************************************************************
* digi_enet_mgmt_fege_ins_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from management FEGE 
*   to a ENET ss or CPU Packets are
*   filtered in by their VLAN ID.  
*   
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   digi_handle             - pointer to DIGI handle instance.
*   chnl                    - channel ID   
*   mcpb_egr_port           - MCPB egress port
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   enable                  - TRUE to enable channel, otherwise channel is disabled. 
*   ovrd_vlan_prio          - when TRUE priority bit in VLAN TCI is override by 
*                             zeroes.
*   rate_data              - enum, client stream types of signal passing through CPB.
*   fcs_enable             - TRUE: overwrite the PMC external TCI field with FCS\n
*                                   enable bit to append FCS in ENET subsystem 
*                            FALSE: Do not overwrite the TCI field sent in PMC external\n
*                                   packet header - used when sending to CPU
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_ins_chnl_cfg(digi_handle_t                     *digi_handle,
                                                  UINT32                             chnl,
                                                  cpb_mcpb_egr_output_port_type_t    mcpb_egr_port,
                                                  util_global_vlan_cfg_t            *vlan_cfg_ptr,
                                                  BOOL8                              enable,
                                                  BOOL8                              ovrd_vlan_prio,
                                                  digi_cpb_stream_t                  rate_data,
                                                  BOOL8                              fcs_enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    mgmt_fege_fwd_cfg_t fwd_cfg;
    cpb_mcpb_ingr_input_port_type_t mcpb_ing_port = CPB_MCPB_ING_FEGE_INS;
    util_global_mcpb_port_t dst_mcpb_port = LAST_UTIL_GLOBAL_MCPB_PORT;
    UINT32 mcpb_ing_chnl = 0;
    UINT32 mcpb_egr_cnhl = 0;
    BOOL8 keep_vlan_prio = TRUE == ovrd_vlan_prio ? FALSE : TRUE;
    
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);   
    PMC_ASSERT(NULL != vlan_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
             
    /* check VLAN_ID range of value */
    if (FALSE ==  digi_enet_mgmt_fege_vlan_id_check(digi_handle, vlan_cfg_ptr->vlan_id))
    { 
        rc = DIGI_ERR_VLAN_ID_OUT_OF_RANGE;
    }
   
    /* MGMT_FEGE can only  */
    if (PMC_SUCCESS == rc && 
        UTIL_GLOBAL_VLAN_TYPE_IEEE_8021P_SINGLE_TAG != vlan_cfg_ptr->vlan_type)
    {
        rc = DIGI_ERR_VLAN_TYPE_NOT_SUPPORTED;
    }

    /* define MCPB egress channel */
    if (PMC_SUCCESS == rc) 
    {
        switch (mcpb_egr_port)
        {
        case CPB_MCPB_EGR_LINE_ING:
            mcpb_ing_chnl = (chnl * 2);
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
            break;
        case CPB_MCPB_EGR_SYS_ING:
            mcpb_ing_chnl = (chnl * 2);
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
            break;
        case CPB_MCPB_EGR_LINE_EG:
            mcpb_ing_chnl = (chnl * 2) + 1;
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
            break;
        case CPB_MCPB_EGR_SYS_EG:
            mcpb_ing_chnl = (chnl * 2) + 1;
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
            break;
        case CPB_MCPB_EGR_CPU:
            mcpb_ing_chnl = DIGI_MGMT_FEGE_TO_CPU_ING_DPI_ID;
            mcpb_egr_cnhl = DIGI_MGMT_FEGE_TO_CPU_EGR_DPI_ID;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_CPU;
            break;
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
     
        if (PMC_SUCCESS == rc) 
        {       
            /* mgmt fege forwarding configuration */
            fwd_cfg.dpi_id = mcpb_ing_chnl;
            fwd_cfg.ins_vlan_id = FALSE;
            PMC_MEMCPY(&fwd_cfg.vlan_entry, vlan_cfg_ptr, sizeof(util_global_vlan_cfg_t));  
            
            rc = mgmt_fege_egr_pkt_fwd_cfg(digi_handle->mgmt_fege_handle, 
                                           enable,
                                           &fwd_cfg,
                                           fcs_enable,
                                           keep_vlan_prio);        

            if ((PMC_SUCCESS == rc) && (TRUE == enable))
            {        
                digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg = enable;
            }
            /* prov MGMT_FEGE  MCPB port if it hasn't been done yet */
            if (PMC_SUCCESS == rc)
            {
                rc = digi_enet_mcpb_chnl_cfg(digi_handle, 
                                             mcpb_egr_port,
                                             mcpb_egr_cnhl,
                                             mcpb_ing_port,
                                             mcpb_ing_chnl,
                                             enable,
                                             rate_data,
                                             dst_mcpb_port);
            }

            /* save context */
            if (PMC_SUCCESS == rc)
            {
                if ((FALSE == enable) &&  
                    (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg) &&
                    (TRUE == mgmt_fege_pkt_all_dpi_deprov_get(digi_handle->mgmt_fege_handle)))
                {        
                    digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg = enable;
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_mgmt_fege_ins_chnl_cfg */

/*******************************************************************************
* digi_enet_mgmt_fege_ins_chnl_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function gets packets forwarding from management FEGE 
*   to a ENET ss or CPU Packets are
*   filtered in by their VLAN ID.  
*   
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   digi_handle             - pointer to DIGI handle instance.
*   chnl                    - channel ID   
*   mcpb_egr_port           - MCPB egress port
*
* OUTPUTS:
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   *enable                 - TRUE to enable channel, otherwise channel is disabled.
*   *priority_en            - priority (PCP) forwarding status
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_ins_chnl_cfg_get(digi_handle_t                   *digi_handle,
                                                      UINT32                          chnl,
                                                      cpb_mcpb_egr_output_port_type_t mcpb_egr_port,
                                                      util_global_vlan_cfg_t          *vlan_cfg_ptr,
                                                      BOOL8                           *enable,
                                                      BOOL8                           *priority_en)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT16 dpi_id;
    UINT32 mcpb_ing_chnl = 0;
    
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);   
    PMC_ASSERT(NULL != vlan_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    switch (mcpb_egr_port)
    {
    case CPB_MCPB_EGR_LINE_ING:
        mcpb_ing_chnl = (chnl * 2);
        break;
    case CPB_MCPB_EGR_SYS_ING:
        mcpb_ing_chnl = (chnl * 2);
        break;
    case CPB_MCPB_EGR_LINE_EG:
        mcpb_ing_chnl = (chnl * 2) + 1;
        break;
    case CPB_MCPB_EGR_SYS_EG:
        mcpb_ing_chnl = (chnl * 2) + 1;
        break;
    case CPB_MCPB_EGR_CPU:
        mcpb_ing_chnl = DIGI_MGMT_FEGE_TO_CPU_ING_DPI_ID;
        break;
    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    dpi_id = mcpb_ing_chnl;

    rc = mgmt_fege_egr_pkt_fwd_vlan_prio_cfg_get(digi_handle->mgmt_fege_handle,
                                                 dpi_id,
                                                 enable,
                                                 vlan_cfg_ptr,
                                                 priority_en);    

    PMC_RETURN(rc);
} /* digi_enet_mgmt_fege_ins_chnl_cfg_get */


/*******************************************************************************
* digi_enet_mgmt_fege_ext_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from a ENET ss or CPU Packets
*   to management FEGE.
*   
*   
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   digi_handle             - pointer to DIGI handle instance.
*   chnl                    - channel ID   
*   mcpb_ing_port           - MCPB ingress port
*   *vlan_cfg_ptr           - VLAN packet configuration. 
*   enable                  - TRUE to enable channel, otherwise channel is disabled. 
*   vlan_id_repl_en         - enable packet VLAN ID replacement.
*   vlan_id_repl            - VLAN ID value replaced in forwarded packet. 
*                             It is ignored when enable or is_vlan_tagged are FALSE.
*    rate_data              - enum, client stream types of signal passing through CPB.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_ext_chnl_cfg(digi_handle_t                     *digi_handle,
                                                  UINT32                             chnl,
                                                  cpb_mcpb_ingr_input_port_type_t    mcpb_ing_port,
                                                  util_global_vlan_cfg_t            *vlan_cfg_ptr,
                                                  BOOL8                              enable,
                                                  BOOL8                              vlan_id_repl_en,
                                                  UINT16                             vlan_id_repl,
                                                  digi_cpb_stream_t                  rate_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    mgmt_fege_fwd_cfg_t fwd_cfg;
    cpb_mcpb_egr_output_port_type_t mcpb_egr_port = CPB_MCPB_EGR_FEGE;
    util_global_mcpb_port_t dst_mcpb_port = LAST_UTIL_GLOBAL_MCPB_PORT;
    UINT32 mcpb_egr_cnhl;
    UINT32 mcpb_ing_chnl;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != vlan_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* check VLAN_ID range of value */
    if (FALSE ==  digi_enet_mgmt_fege_vlan_id_check(digi_handle, vlan_cfg_ptr->vlan_id))
    { 
        rc = DIGI_ERR_VLAN_ID_OUT_OF_RANGE;
    }

    /* MGMT_FEGE can only  */
    if (PMC_SUCCESS == rc && 
        UTIL_GLOBAL_VLAN_TYPE_IEEE_8021P_SINGLE_TAG != vlan_cfg_ptr->vlan_type)
    {
        rc = DIGI_ERR_VLAN_TYPE_NOT_SUPPORTED;
    }

    /* define MCPB egress dpi ID */
    if (PMC_SUCCESS == rc) 
    {
        switch (mcpb_ing_port)
        {
        case CPB_MCPB_ING_LINE_IM_EXT:
            mcpb_egr_cnhl = (chnl << 2) + 1;
            mcpb_ing_chnl = (chnl << 1) + 1;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
            break;
        case CPB_MCPB_ING_SYS_IM_EXT:
            mcpb_egr_cnhl = (chnl << 2) + 1;
            mcpb_ing_chnl = (chnl << 1) + 1;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
            break;
        case CPB_MCPB_ING_LINE_EM_EXT:
            mcpb_egr_cnhl = (chnl << 2);
            mcpb_ing_chnl = (chnl << 1) + 1;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
            break;
        case CPB_MCPB_ING_SYS_EM_EXT:
            mcpb_egr_cnhl = (chnl << 2);
            mcpb_ing_chnl = (chnl << 1) + 1;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
            break;
        case CPB_MCPB_ING_CPU_INS:
            mcpb_egr_cnhl = DIGI_CPU_TO_MGMT_FEGE_EGR_DPI_ID;
            mcpb_ing_chnl = DIGI_CPU_TO_MGMT_FEGE_ING_DPI_ID;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE;
            break;
        default:
            rc = DIGI_ERR_UNKNOWN;
            break;
        }
        
        if (PMC_SUCCESS == rc)
        {   
            /* mgmt fege forwarding configuration */          
            fwd_cfg.dpi_id = mcpb_egr_cnhl;
            fwd_cfg.ins_vlan_id = vlan_id_repl_en;
            PMC_MEMCPY(&fwd_cfg.vlan_entry, vlan_cfg_ptr, sizeof(util_global_vlan_cfg_t));   
            fwd_cfg.vlan_entry.vlan_id = vlan_id_repl;
            
            /* configure MGMT_FEGE ingress */
            rc = mgmt_fege_ingr_pkt_fwd_cfg(digi_handle->mgmt_fege_handle, 
                                            enable, &fwd_cfg);
            PMC_ATOMIC_YIELD(digi_handle,0);                                

            if ((PMC_SUCCESS == rc) && (TRUE == enable))
            {        
                digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg = enable;
            }            
            if (PMC_SUCCESS == rc)
            {
                rc = digi_enet_mcpb_chnl_cfg(digi_handle, 
                                             mcpb_egr_port,
                                             mcpb_egr_cnhl,
                                             mcpb_ing_port,
                                             mcpb_ing_chnl,
                                             enable,
                                             rate_data,
                                             dst_mcpb_port);
            }
            /* save context */
            if (PMC_SUCCESS == rc)
            {
                if ((FALSE == enable) &&  
                    (TRUE == digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg) &&
                    (TRUE == mgmt_fege_pkt_all_dpi_deprov_get(digi_handle->mgmt_fege_handle)))
                {        
                    digi_handle->var.mcpb_ctxt.mcpb_mgmt_fege_port_cfg = enable;
                }
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_mgmt_fege_ext_chnl_cfg */


/*******************************************************************************
* digi_enet_mgmt_fege_ext_chnl_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function gets packets forwarding from a ENET ss or CPU Packets
*   to management FEGE.
*
*   digi_enet_mgmt_fege_init shall be called at least once prior to this function use.
*
* INPUTS:
*   digi_handle             - pointer to DIGI handle instance.
*   chnl                    - channel ID   
*   mcpb_ing_port           - MCPB ingress port
*
* OUTPUTS:
*   vlan_id_repl_en         - enable packet VLAN ID replacement.
*   vlan_id_repl            - VLAN ID value replaced in forwarded packet. 
*                             It is ignored when enable or is_vlan_tagged are FALSE.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_ext_chnl_cfg_get(digi_handle_t                   *digi_handle,
                                                      UINT32                           chnl,
                                                      cpb_mcpb_ingr_input_port_type_t  mcpb_ing_port,
                                                      BOOL8                           *vlan_id_repl_en,
                                                      UINT16                          *vlan_id_repl)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 enable = FALSE; 
    UINT16 dpi_id;
    UINT32 mcpb_egr_cnhl;
    mgmt_fege_fwd_cfg_t fwd_cfg;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* define MCPB egress dpi ID */
    if (PMC_SUCCESS == rc) 
    {
        switch (mcpb_ing_port)
        {
        case CPB_MCPB_ING_LINE_IM_EXT:
            mcpb_egr_cnhl = (chnl << 2) + 1;
            break;
        case CPB_MCPB_ING_SYS_IM_EXT:
            mcpb_egr_cnhl = (chnl << 2) + 1;
            break;
        case CPB_MCPB_ING_LINE_EM_EXT:
            mcpb_egr_cnhl = (chnl << 2);
            break;
        case CPB_MCPB_ING_SYS_EM_EXT:
            mcpb_egr_cnhl = (chnl << 2);
            break;
        case CPB_MCPB_ING_CPU_INS:
            mcpb_egr_cnhl = DIGI_CPU_TO_MGMT_FEGE_EGR_DPI_ID;
            break;
        default:
            rc = DIGI_ERR_UNKNOWN;
            break;
        }
        
        if (PMC_SUCCESS == rc)
        {   
            /* mgmt fege forwarding configuration */          
            dpi_id = mcpb_egr_cnhl;

            /* configure MGMT_FEGE ingress */
            mgmt_fege_ingr_pkt_fwd_cfg_get(digi_handle->mgmt_fege_handle,
                                           dpi_id,
                                           &enable,
                                           &fwd_cfg);

            *vlan_id_repl_en = fwd_cfg.ins_vlan_id;
            *vlan_id_repl = fwd_cfg.vlan_entry.vlan_id;
       }
    }

    PMC_RETURN(rc);
} /* digi_enet_mgmt_fege_ext_chnl_cfg_get */

/*******************************************************************************
* digi_enet_sys_db_deprov_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API frees database entries used by an ENET SYS link in the releveant
*   databases as part of a higher level link deprovisioning operation. \n\n
*
*   The entries are now available for usage by other links in future
*   provision operations.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   enet_link         - ENET SYS link to be deprovisioned.
*                       Valid values 0 to 11.
*
* OUTPUTS:
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_db_deprov_cfg(digi_handle_t *digi_handle,
                                             UINT32 enet_link)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 dummy;
    UINT32 enet_sys_dcs_inst;

    PMC_ENTRY();
    
    result = digi_cpb_pop_port_get(digi_handle,
                                   UTIL_GLOBAL_CPB_PORT_ENET_SYS,
                                   0,
                                   &enet_sys_dcs_inst,
                                   &dummy);
                                   
    if(PMC_SUCCESS == result)
    {                      
        result = digi_enet_sys_db_clear(digi_handle, enet_link, 
                                        enet_sys_dcs_inst);
    }

    PMC_RETURN(result);

} /* digi_enet_sys_db_deprov_cfg */


/*******************************************************************************
* digi_port_uid_from_enet_link_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to get the port_uid based on the ENET link.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   link               - the ENET link number
*   is_line            - ENET line side 
* 
* OUTPUTS:
*   *port_uid          - the unique identifier of the SERDES port.
*
* RETURNS:
*   PMC_SUCCESS, specific error otherwise.
*
* NOTES:
*   *port_uid is only valid upon return of PMC_SUCCESS.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_port_uid_from_enet_link_get(digi_handle_t *digi_handle,
                                                  UINT32 link,
                                                  BOOL8 is_line,
                                                  UINT32* port_uid)
    
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    digi_serdes_prov_state_t prov_state;

    PMC_ENTRY();
    if (is_line == TRUE)
    {
        prov_state = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[link].prov_state;
        if (prov_state == DIGI_SERDES_PROV_STATE_ENET_LINE) {  

            *port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[link].port_uid;
            rc = PMC_SUCCESS;

            /* If this is a DIGI_PROD_APP_100G_3_X_40G_CFP_CARD application,
             * check the SYSTEM side as well. */
        } else if (digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD) {

            prov_state = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[link].prov_state;

            if (prov_state == DIGI_SERDES_PROV_STATE_ENET_LINE) { 

                *port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[link].port_uid;      
                rc = PMC_SUCCESS;

            }
        }

    } else {

        prov_state = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[link].prov_state;
        if (prov_state == DIGI_SERDES_PROV_STATE_ENET_SYS)
        {      
            *port_uid = digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[link].port_uid;      
            rc = PMC_SUCCESS;
        }
    }
    
    PMC_RETURN(rc);
} /* digi_port_uid_from_enet_link_get */


/*******************************************************************************
* digi_enet_deprov_new_mapper_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to get the new enet line deprov mapper state given 
*   the current state and what the mapper is currently trying to accomplish.
*
* INPUTS:
*   in_map_type        - the current map type that the mapper will use to
*                        determine the next state.
*                        See digi_mapper_alloc_t for more information.
*
* OUTPUTS:
*   out_map_type       - the new map type that the mapper will be set to once
*                        provisioning is successfull. 
*                        See digi_mapper_alloc_t for more information.
*
* RETURNS:
*   DIGI_ERR_UNKNOWN on failure, PMC_SUCCESS otherwise.
*
* NOTES:
*   *out_map_type is only valid upon return of PMC_SUCCESS.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_deprov_new_mapper_state_get(digi_mapper_alloc_t in_map_type,
                                                       digi_mapper_alloc_t *out_map_type)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (in_map_type == DIGI_MAPPER_ALLOC_ENET_LINE) {

        *out_map_type = DIGI_MAPPER_ALLOC_UNUSED;
        rc = PMC_SUCCESS;

    } else if (in_map_type == DIGI_MAPPER_ALLOC_MAPPER_AND_ENET_LINE) {

        *out_map_type = DIGI_MAPPER_ALLOC_MAPPER_WITHOUT_ENET_LINE;
        rc = PMC_SUCCESS;

    } else {
        /* Assert */
        rc = DIGI_ERR_CODE_ASSERT;
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(rc);
} /* digi_enet_deprov_new_mapper_state_get */

/*******************************************************************************
* digi_first_sys_post_mld_dsi_find
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to find the first post mld context of type
*   digi_post_mld_dsi_ctxt_t matching the specified port_uid on the system
*   side.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   port_uid           - the unique identifier of the SERDES port.
*                        To find the first unused one use:
*                        DIGI_SERDES_UNASSIGNED
*
* OUTPUTS:
*
* RETURNS:
*   A pointer to the requested matching digi_post_mld_dsi_ctxt_t structure,
*   or NULL if there are no more available or the requested one could not be
*   found.
*
* NOTES:
*
*******************************************************************************/
PUBLIC digi_post_mld_dsi_ctxt_t* digi_first_sys_post_mld_dsi_find(digi_handle_t *digi_handle,
                                                                  UINT32 port_uid)
{
    digi_post_mld_dsi_ctxt_t* rp = NULL;
    UINT8 i = 0;

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_DSI_LANES_MAX; i ++) {

        /* Check the system side */
        if (digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i].port_uid == port_uid) {

            rp = &(digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[i]);
            break;
        }
    }

    PMC_RETURN(rp);
} /* digi_first_sys_post_mld_dsi_find */

/*******************************************************************************
* digi_enet_prov_new_mapper_state_get 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to get the new enet line prov mapper state given 
*   the current state and what the mapper is currently trying to accomplish.
*
* INPUTS:
*   in_map_type        - the current map type that the mapper will use to
*                        determine the next state.
*                        See digi_mapper_alloc_t for more information.
*
* OUTPUTS:
*   out_map_type       - the new map type that the mapper will be set to once
*                        provisioning is successfull. 
*                        See digi_mapper_alloc_t for more information.
*
* RETURNS:
*   DIGI_ERR_UNKNOWN on failure, PMC_SUCCESS otherwise.
*
* NOTES:
*   *out_map_type is only valid upon return of PMC_SUCCESS.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_prov_new_mapper_state_get(digi_mapper_alloc_t in_map_type,
                                                     digi_mapper_alloc_t *out_map_type)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (in_map_type == DIGI_MAPPER_ALLOC_UNUSED) {

        *out_map_type = DIGI_MAPPER_ALLOC_ENET_LINE;
        rc = PMC_SUCCESS;

    } else if (in_map_type == DIGI_MAPPER_ALLOC_MAPPER_WITHOUT_ENET_LINE) {

        *out_map_type = DIGI_MAPPER_ALLOC_MAPPER_AND_ENET_LINE;
        rc = PMC_SUCCESS;

    } else {
        /* Assert */
        rc = DIGI_ERR_CODE_ASSERT;
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(rc);
} /* digi_enet_prov_new_mapper_state_get */

/*******************************************************************************
* digi_enet_client_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The is a helper function which obtains an enet_client_chnl_def_t and populates
*   the values based on the ethernet link passed in.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   serdes_mode            - Configuration mode for serdes port
*   mode                   - Configuration mode for enet link
*   enet_link              - the ethernet_link
*   port_type              - the util_global_cbp_port_t. Should be
*                            ENET_LINE/ENET_SYS only. The exception is if
*                            we are getting an ETRANS resource. In this case,
*                            it should be set to UTIL_GLOBAL_CPB_PORT_NOT_USED.
*   res_type               - if the reservation type is to be for regular ENET
*                            provisioning, or in an Enhanced PMON case.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   digi_enet_client_chnl_def_t - pointer to the ENET chnl_ctxt.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC digi_enet_client_chnl_def_t* digi_enet_client_chnl_get(digi_handle_t* digi_handle,
                                                              digi_serdes_port_mode_t serdes_mode,
                                                              digi_enet_mode_t mode,
                                                              UINT32 enet_link, 
                                                              util_global_cpb_port_t port_type,
                                                              digi_enet_res_type_t res_type)
{
    digi_enet_client_chnl_def_t* chnl_ctxt_ptr;

    PMC_ENTRY();

    if ((port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) || 
        (port_type == UTIL_GLOBAL_CPB_PORT_NOT_USED)) {

        if (digi_handle->handle_pool.enet_line_chnl[enet_link].switch_data.header.prov_state == UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG) {

            chnl_ctxt_ptr = &(digi_handle->handle_pool.enet_line_chnl[enet_link]);

            if (res_type == DIGI_ENET_RES_PMON) { 
                 
                /* Set the PMON bit here */
                digi_enet_pmon_line_usage_set(digi_handle, enet_link, mode, TRUE);
            }

        } else {

            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        }

    } else if (port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) {

        if (digi_handle->handle_pool.enet_sys_chnl[enet_link].switch_data.header.prov_state == UTIL_GLOBAL_CHNL_PROV_STATE_UNCONFIG) {

            chnl_ctxt_ptr = &(digi_handle->handle_pool.enet_sys_chnl[enet_link]);

            if (res_type == DIGI_ENET_RES_PMON) { 
                 
                /* Set the PMON bit here */
                digi_enet_pmon_sys_usage_set(digi_handle, enet_link, mode, TRUE);
            }

        } else {

            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        }

    } else {

        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    /* Reset the handle to the default state */
    digi_chnl_ptr_initialize(&(chnl_ctxt_ptr->switch_data));

    /* Switch info */
    chnl_ctxt_ptr->switch_data.cpb_data.port_type = port_type;
    chnl_ctxt_ptr->dsi_base_lane = DIGI_SERDES_PIN_NOT_USED;
    chnl_ctxt_ptr->mode = mode;

    /* Only update this information is requested as SERDES, not ENH PMON */

    if (res_type == DIGI_ENET_RES_SERDES) {

        chnl_ctxt_ptr->serdes_mode = serdes_mode;
        chnl_ctxt_ptr->switch_data.header.prov_state = UTIL_GLOBAL_CHNL_PROV_STATE_CONFIG;
        chnl_ctxt_ptr->timing_mode = DIGI_ENET_TX_NOMINAL_TIMING_MODE;
    }
        
    /* Link is 1:1 mapped with channel */
    chnl_ctxt_ptr->switch_data.cpb_data.channel = enet_link;
    chnl_ctxt_ptr->switch_data.cpb_data.data_source = 0;

    PMC_RETURN(chnl_ctxt_ptr);
} /* digi_enet_client_chnl_get */


/*******************************************************************************
* digi_enet_does_state_match
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checkes the state of the enet channel in the device and confirms that 
*   its state matches the state passed to the function.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   enet_link              - the ethernet_link
*   rx_input               - Whether the state check is relative to the
*                            RX channel, TRUE, or the TX channel, FALSE.
*   port_type              - the util_global_cbp_port_t. Should be
*                            ENET_LINE/ENET_SYS only
*   state                  - State to match against.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE:  Device state matches the state parameter.
*   FALSE: Otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC BOOL8 digi_enet_does_state_match(digi_handle_t* digi_handle,
                                        UINT32 enet_link,
                                        BOOL8 rx_input,
                                        util_global_cpb_port_t port_type,
                                        util_global_chnl_state_t state)
{
    BOOL8 result = FALSE;

    PMC_ENTRY();

    if (UTIL_GLOBAL_CPB_PORT_ENET_SYS == port_type)
    {
        /*
         * Need to make a call to the enet module to check the state.
         * This will be enet_datapath_cfg_get. For now return TRUE/
         */
        result = enet_prov_get(digi_handle->enet_sys_handle,
                               enet_link, rx_input, state);
    }
    else
    {
        result = enet_prov_get(digi_handle->enet_line_handle,
                               enet_link, rx_input, state);
    }
    PMC_RETURN(result);
} /* digi_enet_does_state_match */

/*******************************************************************************
* digi_enet_sys_db_prov_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API retrieves scheduling resources avaliable to the ENET SYS link
*   being provisioned.  It tests to ensure no allocation conflict and updates
*   the relevant database set with the appropriate link data.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   group_size        - mumber of 10G secondary groups required for the enet
*                       application see enum type digi_group_size_t
*   enet_link         - ENET SYS link to be configured
*                       Valid values 0 to 11.
*
*
* OUTPUTS:
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_db_prov_cfg(digi_handle_t *digi_handle,
                                           digi_group_size_t group_size,
                                           UINT32 enet_link)
{
    UINT16 dsi_mask;
    BOOL schd_addr_alloc[DIGI_SCHD_96_ENTRY];
    UINT32 schd_addr;
    UINT8 success_update = 0;
    UINT32 num_cal_entry_req = 0;
    BOOL8 update = FALSE;
    UINT32 dummy;
    UINT32 enet_sys_dcs_inst;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(group_size < LAST_DIGI_GRP_SIZE,DIGI_ERR_INVALID_ARG, 0, 0);
    
    

    switch(group_size)
    {
    case DIGI_GRP_SIZE_10G:
        num_cal_entry_req = 8;
        break;
    case DIGI_GRP_SIZE_40G:
        num_cal_entry_req = 32;
        break;
    case DIGI_GRP_SIZE_100G:
        num_cal_entry_req = 80;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }
    
    
    result = digi_cpb_pop_port_get(digi_handle,
                                   UTIL_GLOBAL_CPB_PORT_ENET_SYS,
                                   0,
                                   &enet_sys_dcs_inst,
                                   &dummy);
                                   
    PMC_LOG_TRACE("enet_sys_dcs_inst: %d\n", enet_sys_dcs_inst);                              

    if(PMC_SUCCESS == result)
    {
        /* get entries */
        digi_enet_post_mld_dsi_get(digi_handle, enet_link, group_size, &dsi_mask);
        digi_post_mld_dsi_schd_entry_get(digi_handle, dsi_mask, schd_addr_alloc);

        /* test all db for this entry being available */
        /* write all database with appropriate values */
        for(schd_addr=0; schd_addr<DIGI_SCHD_96_ENTRY; schd_addr++)
        {
            if(schd_addr_alloc[schd_addr] == TRUE)
            {
                PMC_LOG_TRACE("schd_addr:%d  success_update:%d\n", schd_addr, success_update);
                update = digi_enet_sys_db_test_update(digi_handle, schd_addr,
                                                      enet_link, enet_sys_dcs_inst);
                if(TRUE == update)
                {
                    success_update ++;
                }
            }
        }

        if(num_cal_entry_req != success_update)
        {
            /*remove all entries that was added if not enough recources was available  */
            result = digi_enet_sys_db_clear(digi_handle, enet_link, enet_sys_dcs_inst);

            if(PMC_SUCCESS == result)
            {
                result = DIGI_ERR_SCHD_INSUFFICIENT_RESOURCES_AVAILABLE;
            }
        }
    }
    PMC_RETURN(result);

} /* digi_enet_sys_db_prov_cfg */

/*******************************************************************************
* digi_enet_line_db_prov_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API retrieves scheduling resources avaliable to the ENET line link
*   being provisioned.  It tests to ensure no allocation conflict and updates
*   the relevant database set with the appropriate link data.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   group_size        - mumber of 10G secondary groups required for the enet
*                       application see enum type digi_group_size_t
*   enet_link         - ENET line link to be configured
*                       Valid values 0 to 11.
*   mirror_mode       - The enet link is a mirror port.
*
*
* OUTPUTS:
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_db_prov_cfg(digi_handle_t *digi_handle,
                                            digi_group_size_t group_size,
                                            UINT32 enet_link,
                                            BOOL mirror_mode)
{
    UINT16 dsi_mask;
    BOOL schd_addr_alloc[DIGI_SCHD_96_ENTRY];
    UINT32 schd_addr;
    UINT8 success_update = 0;
    UINT32 num_cal_entry_req = 0;
    BOOL8 update = FALSE;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(group_size < LAST_DIGI_GRP_SIZE,DIGI_ERR_INVALID_ARG, 0, 0);

    switch(group_size)
    {
    case DIGI_GRP_SIZE_10G:
        num_cal_entry_req = 8;
        break;
    case DIGI_GRP_SIZE_40G:
        num_cal_entry_req = 32;
        break;
    case DIGI_GRP_SIZE_100G:
        num_cal_entry_req = 80;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    if(PMC_SUCCESS == result)
    {
        /* get entries */
        /*digi_mapper_post_mld_dsi_get(digi_handle, p_group, s_group, group_size, &dsi_mask);*/  /*make an enet specific API to get the dsi_mask for a link and group size.*/
        digi_enet_post_mld_dsi_get(digi_handle, enet_link, group_size, &dsi_mask);
        digi_post_mld_dsi_schd_entry_get(digi_handle, dsi_mask, schd_addr_alloc);

        /* test all db for this entry being available */
        /* write all database with appropriate values */
        for(schd_addr=0; schd_addr<DIGI_SCHD_96_ENTRY; schd_addr++)
        {
            if(schd_addr_alloc[schd_addr] == TRUE)
            {
                PMC_LOG_TRACE("schd_addr:%d  success_update:%d\n", schd_addr, success_update);
                update = digi_enet_line_db_test_update(digi_handle, schd_addr,
                                                       enet_link, mirror_mode);
                if(TRUE == update)
                {
                    success_update ++;
                }
            }
        }

        if(num_cal_entry_req != success_update)
        {
            /*remove all entries that was added if not enough recources was available  */
            result = digi_enet_line_db_clear(digi_handle, enet_link, mirror_mode);

            if(PMC_SUCCESS == result)
            {
                result = DIGI_ERR_SCHD_INSUFFICIENT_RESOURCES_AVAILABLE;
            }
        }
    }
    PMC_RETURN(result);

} /* digi_enet_line_db_prov_cfg */

/*******************************************************************************
* digi_enet_line_db_deprov_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API frees database entries used by an ENET line link in the releveant
*   databases as part of a higher level link deprovisioning operation. \n\n
*
*   The entries are now available for usage by other links in future
*   provision operations.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   enet_link         - ENET line link to be deprovisioned.
*                       Valid values 0 to 11.
*   mirror_mode       - The enet link is a mirror port.
*
* OUTPUTS:
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_db_deprov_cfg(digi_handle_t *digi_handle,
                                              UINT32 enet_link,
                                              BOOL mirror_mode)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    result = digi_enet_line_db_clear(digi_handle, enet_link, mirror_mode);

    PMC_RETURN(result);

} /* digi_enet_line_db_deprov_cfg */

/*******************************************************************************
* digi_enet_line_prov_params_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_line_prov.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_def_ptr          - Port datapath handle defining the physcial SerDes
*                            to configure the ENET_LINE resource against.\n
*   mode                   - Enum, ENET modes when connected to Line
*                            Serdes.  Only modes containing "SERDES" may be used.
*   **chnl_ctxt_pptr       - a pointer to a pointer to a digi_enet_client_chnl_t
*                            structure consisting of a dsi_base_lane, link, and
*                            util_global_cpb_port_data_t that is returned to the
*                            user.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_prov_params_validate(digi_handle_t *digi_handle,
                                                     digi_serdes_port_def_t *port_def_ptr,
                                                     digi_enet_mode_t mode,
                                                     digi_enet_client_chnl_t **chnl_ctxt_pptr)

{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (digi_handle == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else if (digi_handle->enet_line_handle == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else if (digi_serdes_enet_line_port_validate(port_def_ptr) != PMC_SUCCESS &&
               digi_handle->var.prod_app != DIGI_PROD_APP_100G_3_X_40G_CFP_CARD) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (mode > DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (chnl_ctxt_pptr == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else {
        rc = digi_enet_line_prov_lane_validate(digi_handle,
                                               port_def_ptr->port_uid,
                                               mode);
    }

    PMC_RETURN(rc);
} /* digi_enet_line_prov_params_validate */

/*******************************************************************************
* digi_enet_line_prov_params_additional_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_line_prov.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   mode                   - Enum, ENET modes when connected to Line
*                            Serdes.  Only modes containing "SERDES" may be used.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_prov_params_additional_validate(digi_handle_t *digi_handle,
                                                                digi_enet_mode_t mode)

{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (digi_handle == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } 
    else if (FALSE == digi_device_init_status_get(digi_handle))
    {
        /* check that DIGI device is initialized */
        rc = DIGI_ERR_DEVICE_NOT_INIT;
    }
    else if (mode > DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if ((digi_handle->var.prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) &&
               ((mode == DIGI_ENET_SERDES_10GE_MAC) ||
                (mode == DIGI_ENET_SERDES_40GE_MAC) || 
                (mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT))) { 

        rc = DIGI_ERR_ENET_ONLY_TRANSPARENT_MODES_SUPPORTED;

    } else {

        rc = PMC_SUCCESS;

    }

    PMC_RETURN(rc);

} /* digi_enet_line_prov_params_additional_validate */

/*******************************************************************************
* digi_enet_sys_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_sys_prov.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *port_def_ptr          - Port datapath handle defining the physcial SerDes
*                            to configure the ENET_LINE resource against.\n
*   mode                   - Enum, ENET_SYS modes when connected to Line
*                            Serdes.  Only modes containing "SERDES" may be used.
*   **chnl_ctxt_pptr       - a pointer to a pointer to a digi_enet_client_chnl_t
*                            structure consisting of a dsi_base_lane, link, and
*                            util_global_cpb_port_data_t that is returned to
*                            the user.
*
* RETURNS:
*   None.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_prov_params_validate(digi_handle_t *digi_handle,
                                                    digi_serdes_port_def_t *port_def_ptr,
                                                    digi_enet_mode_t mode,
                                                    digi_enet_client_chnl_t **chnl_ctxt_pptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if (digi_handle == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    }
    else if (FALSE == digi_device_init_status_get(digi_handle))
    {
        /* check that DIGI device is initialized */
        rc = DIGI_ERR_DEVICE_NOT_INIT;
    }
    else if (digi_handle->enet_sys_handle == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else if (digi_serdes_enet_sys_port_validate(port_def_ptr) != PMC_SUCCESS) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (mode > DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON) {

        rc = DIGI_ERR_INVALID_ARG;

    } else if (chnl_ctxt_pptr == NULL) {

        rc = DIGI_ERR_NULL_HANDLE;

    } else if (digi_handle->var.prod_app == DIGI_PROD_APP_100G_3_X_40G_CFP_CARD) {

        rc = DIGI_ERR_ENET_SYS_UNAVAILABLE_IN_CARD_MODE;

    } else {

        rc = digi_enet_sys_prov_lane_validate(digi_handle,
                                              port_def_ptr->port_uid, mode);
    }

    PMC_RETURN(rc);
} /* digi_enet_sys_prov_params_validate */


/*******************************************************************************
* digi_enet_line_prov_params_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the adaptaion function and group
*   size for enet line
*
* INPUTS:
*   mode             - the digi enet mode
*
* OUTPUTS:
*   *e_mode          - pointer to storage of equivalent adaptaion function 
*                      enet mode
*   *group_size      - the number of post MLD DSI lanes required for this mode.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_line_prov_params_convert(digi_enet_mode_t mode,
                                               enet_datapath_t* e_mode,
                                               digi_group_size_t* group_size)
{
    PMC_ENTRY();

    switch(mode) {
    case DIGI_ENET_SERDES_100GE_MAC:
        *e_mode = ENET_SERDES_100GE_MAC;
        *group_size = DIGI_GRP_SIZE_100G;
        break;
    case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        *e_mode = ENET_SERDES_100GE_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_100G;
        break;
    case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_100G;
        break;
    case DIGI_ENET_SERDES_40GE_MAC:
        *e_mode = ENET_SERDES_40GE_MAC;
        *group_size = DIGI_GRP_SIZE_40G;
        break;
    case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        *e_mode = ENET_SERDES_40GE_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_40G;
        break;
    case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_40G;
        break;
    case DIGI_ENET_SERDES_10GE_MAC:
        *e_mode = ENET_SERDES_10GE_MAC;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        *e_mode = ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_TRANSPARENT:
        *e_mode = ENET_SERDES_10GE_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN();
} /* digi_enet_line_prov_params_convert */


/*******************************************************************************
* digi_enet_sys_prov_params_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the adaptaion function and group
*   size for enet sys
*
* INPUTS:
*   mode             - the digi enet mode
*
* OUTPUTS:
*   *e_mode          - pointer to storage of equivalent adaptaion function 
*                      enet mode
*   *group_size      - the number of post MLD DSI lanes required for this mode.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_sys_prov_params_convert(digi_enet_mode_t mode,
                                              enet_datapath_t* e_mode, 
                                              digi_group_size_t* group_size)
{
    PMC_ENTRY();

    switch(mode) {
    case DIGI_ENET_SERDES_100GE_MAC:
        *e_mode = ENET_SERDES_100GE_MAC;
        *group_size = DIGI_GRP_SIZE_100G;
        break;
    case DIGI_ENET_SERDES_100GE_TRANSPARENT:
        *e_mode = ENET_SERDES_100GE_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_100G;
        break;
    case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_100G;
        break;
    case DIGI_ENET_SERDES_40GE_MAC:
        *e_mode = ENET_SERDES_40GE_MAC;
        *group_size = DIGI_GRP_SIZE_40G;
        break;
    case DIGI_ENET_SERDES_40GE_TRANSPARENT:
        *e_mode = ENET_SERDES_40GE_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_40G;
        break;
    case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_40G;
        break;
    case DIGI_ENET_SERDES_10GE_MAC:
        *e_mode = ENET_SERDES_10GE_MAC;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        *e_mode = ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_TRANSPARENT:
        *e_mode = ENET_SERDES_10GE_TRANSPARENT;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
        *e_mode = ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
        *group_size = DIGI_GRP_SIZE_10G;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        break;
    }

    PMC_RETURN();
} /* digi_enet_sys_prov_params_convert */


/*******************************************************************************
* digi_enet_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   This function sets the expected UPI values in data or ordered set packet 
*   and the action to be done when incoming packet UPI does not match expected 
*   values: DROP or IGNORE.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*    pkt_upi_val           - value that identifies the transmit data as a 10GE data frame.
*    os_upi_val            - value that identifies the transmit data as 10GE ordered set.
*    enable_drop           - when TRUE, incoming packet that has an UPI TYPE value that
*                            does not match either pkt_upi_val or os_upi_val
*                            are dropped.
*                            when FALSE, incoming data packet will be treated as a 
*                            data packet.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block or length is not valid
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_upi_set(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                      pkt_upi_val,
                                      UINT8                      os_upi_val,
                                      BOOL8                      enable_drop)
{
    PMC_ERROR rc = PMC_SUCCESS;


    PMC_ATOMIC_ENTRY(digi_handle);

    /* internal fuction does error checking */
    rc = digi_enet_tx_upi_internal_set(digi_handle, chnl_handle, 
                                       pkt_upi_val,
                                       os_upi_val,
                                       enable_drop);
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_tx_upi_set */

/*******************************************************************************
* digi_enet_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the expected UPI values in data or ordered set packet 
*   and the action to be done when incoming packet UPI does not match expected 
*   values: DROP or IGNORE.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - value that identifies the transmit data as a 10GE data frame.
*  *os_upi_val_ptr      - value that identifies the transmit data as 10GE ordered set.
*  *enable_drop_ptr     - when TRUE, incoming packet that has an UPI TYPE value that
*                         does not match either pkt_upi_val or os_upi_val
*                         are dropped.
*                         when FALSE, incoming data packet will be treated as a 
*                         data packet.
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block or length is not valid
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_tx_upi_get(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                     *pkt_upi_val_ptr,
                                      UINT8                     *os_upi_val_ptr,
                                      BOOL8                     *enable_drop_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) 
    {
        rc = enet_tx_upi_get(enet_handle, 
                             link, 
                             pkt_upi_val_ptr,
                             os_upi_val_ptr,
                             enable_drop_ptr);
        
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_tx_upi_get */

/*******************************************************************************
* digi_enet_rx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the transmitted GSUP43 7.3 UPI values for data 
*   and ordered set frames. This function should only be used to configure 
*   standard UPI values. To configure a datapath with non-standard (legacy) 
*   UPI values, the digi_gsup43_7_3_legacy_mode_cfg function should be used.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle 
*   *chnl_handle           - pointer to ENET handle
*    pkt_upi_val           - data frame UPI value
*    os_upi_val            - Ordered Set UPI value
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS or an error code
*   
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_upi_set(digi_handle_t             *digi_handle,
                                       util_global_switch_data_t *chnl_handle,
                                       UINT8                      pkt_upi_val,
                                       UINT8                      os_upi_val)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);
    if (PMC_SUCCESS == rc) 
    { 
        rc = enet_rx_upi_set(enet_handle, link, pkt_upi_val, os_upi_val);
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_rx_upi_set */

/*******************************************************************************
* digi_enet_rx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the configured GSUP43 7.3 UPI values for data and 
*   ordered set frames.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_handle        - pointer to ENET handle
*
* OUTPUTS:
*  *pkt_upi_val_ptr     - Data frame UPI value
*  *os_upi_val_ptr      - Ordered Set UPI value
*
* RETURNS:
*   PMC_SUCCESS or an error code.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_rx_upi_get(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                     *pkt_upi_val_ptr,
                                      UINT8                     *os_upi_val_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, &link);
    if (PMC_SUCCESS == rc) 
    {
        rc = enet_rx_upi_get(enet_handle, link, pkt_upi_val_ptr, os_upi_val_ptr);
    }
    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_rx_upi_get */

/*******************************************************************************
* digi_enet_mac_tx_flush
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will flush a CPB FIFO when the destination is a Transmit
*   ENET MAC.\n\n
*
*   The function performs two unique operations controlled by the action 
*   parameter.  An action to be performed on detection of DLOS event and
*   an action to be performed upon clearing of the DLOS event. \n\n
*
*   This API is recommended to be used as follows:\n
*   A) Upon ingress DLOS event being detected: \n
*   1) Force SEND RF condition on egress path \n
*   2) call digi_enet_mac_tx_flush with action set to TRUE \n
*   B) Upon clearing of ingress DLOS event being detected: \n
*   1) Disable forceing of SEND RF condition on egress path \n
*   2) call digi_enet_mac_tx_flush with action set to FALSE \n
*
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *output_pc_ptr   - Destination DPI port / channel pair for the channel to
*                      be flushed.  Accepts the pointer to one of the
*                      digi_enet_client_chnl_t \n
*                      digi_mapper_chnl_t \n
*    action          - Operation to be performed by function:
*                      TRUE:  Action to take upon detection of dLOS
*                      FALSE: Action to take upon recovery from dLOS
*
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mac_tx_flush(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *output_pc_ptr,
                                        BOOL8 action)
{
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT8 qs_ch_en = 0, qs_ch_en_to_update = 0;
    util_global_switch_data_t *input_sw_ptr = NULL;
    util_global_switch_data_def_t *local_input_sw_ptr = NULL;
    util_global_cpb_port_data_t *local_input_pc_ptr = NULL;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* recover input port data */
    digi_opsa_src_chnl_hndl_for_dest_get(digi_handle,
                                         output_pc_ptr,
                                         FALSE,
                                         &input_sw_ptr);

    if (NULL == input_sw_ptr)
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_INVALID_ARG);
    }

    local_input_sw_ptr = (util_global_switch_data_def_t*)input_sw_ptr;

    local_input_pc_ptr = &(local_input_sw_ptr->cpb_data);
    result = digi_enet_validate_parms(digi_handle, output_pc_ptr, &enet_handle, &link);

    if (PMC_SUCCESS == result) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            result = DIGI_ERR_ENET_LINK_NOT_MAC;
        }
    }

    if (PMC_SUCCESS == result) 
    {
        /* Disable QSn_CH_EN for the channel */
        cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                  local_input_pc_ptr->port_type,
                                  local_input_pc_ptr->channel,
                                  &qs_ch_en);

        digi_cpb_chnl_qs_ch_en_get(digi_handle,
                                   local_input_sw_ptr,
                                   output_pc_ptr,
                                   &qs_ch_en_to_update);

        cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                  local_input_pc_ptr->port_type,
                                  local_input_pc_ptr->channel,
                                  qs_ch_en^qs_ch_en_to_update);
    }    

    if (PMC_SUCCESS == result)
    {
        /* Flush CPB for the channel */
        cpb_queue_flush_and_ptr_reset(digi_handle->dcpb_handle,
                                      local_input_pc_ptr->port_type,
                                      local_input_pc_ptr->channel);
    }

    if (PMC_SUCCESS == result && action == TRUE) 
    {
        /* Toggle TX_FLUSH */
        emac_mac_tx_flush_toggle(enet_handle->emac_handle, link);
    }

    if (PMC_SUCCESS == result && action == FALSE) 
    {
        /* Enable QSn_CH_EN for the channel */
        /* Disable QSn_CH_EN for the channel */
        cpb_slv_chnl_qs_ch_en_get(digi_handle->dcpb_handle,
                                  local_input_pc_ptr->port_type,
                                  local_input_pc_ptr->channel,
                                  &qs_ch_en);

        digi_cpb_chnl_qs_ch_en_get(digi_handle,
                                   local_input_sw_ptr,
                                   output_pc_ptr,
                                   &qs_ch_en_to_update);

        cpb_slv_chnl_qs_ch_en_set(digi_handle->dcpb_handle,
                                  local_input_pc_ptr->port_type,
                                  local_input_pc_ptr->channel,
                                  qs_ch_en|qs_ch_en_to_update);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_enet_mac_tx_flush */

#endif /* DOXYGEN_PUBLIC_ONLY */


/*
** Private Functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

/*******************************************************************************
* FUNCTION: digi_enet_toggle_rx_lane_inv()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Toggle the S16 LOS invert register for a given channel.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   ch_num          - lane to invert
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_enet_toggle_rx_lane_inv(digi_handle_t* digi_handle, UINT8 ch_num)
{
    enum {RX_LANE_INV_ADDR = 0x6011C, RX_LANE_INV_POS = 8};

    PMC_ENTRY();

    /* Write to DSIS_PIN_LOS_INV */
    s16_polarity_cfg(digi_handle->s16_line_handle, ch_num, UTIL_GLOBAL_DIR_RX, TRUE);
    PMC_OS_USLEEP(1000);
    s16_polarity_cfg(digi_handle->s16_line_handle, ch_num, UTIL_GLOBAL_DIR_RX, FALSE);

    PMC_RETURN();
}

/*******************************************************************************
* FUNCTION: digi_enet_check_align_status_v_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check value of align_status_v. Note that this API only applies to the 
*   ENET LINE.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* RETURNS:
*   TRUE - align_status_v is set;
*   FALSE - otherwise
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL digi_enet_check_align_status_v_get(digi_handle_t* digi_handle)
{
    BOOL rc;

    PMC_ENTRY();
    rc = enet_100G_check_align_status_v_get(digi_handle->enet_line_handle);
    PMC_RETURN(rc);
}

/*******************************************************************************
* FUNCTION: digi_enet_disable_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear align_status_e and rx_loc_fault_e. Note that this API only applies
*   to the ENET LINE.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_enet_disable_align_status_loc_fault(digi_handle_t* digi_handle)
{
    PMC_ENTRY();
    enet_100G_disable_align_status_loc_fault(digi_handle->enet_line_handle);
    PMC_RETURN();
}

/*******************************************************************************
* FUNCTION: digi_enet_enable_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set align_status_e and rx_loc_fault_e. Note that this API only applies to
*   the ENETLINE.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_enet_enable_align_status_loc_fault(digi_handle_t* digi_handle)
{
    PMC_ENTRY();
    enet_100G_enable_align_status_loc_fault(digi_handle->enet_line_handle);
    PMC_RETURN();
}

/*******************************************************************************
* FUNCTION: digi_enet_clear_align_status_loc_fault()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Write to clear align_status_i and rx_loc_fault_i. Note that this only
*   applies to the ENET LINE.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_enet_clear_align_status_loc_fault(digi_handle_t *digi_handle)
{
    PMC_ENTRY();
    enet_100G_clear_align_status_loc_fault(digi_handle->enet_line_handle);
    PMC_RETURN();
}

/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_filt_retrieve_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an channel interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*   
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*   fw_retrieve_done - true mean status had been retrieved else FALSE
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_chnl_filt_retrieve_internal(digi_handle_t             *digi_handle,
                                                           digi_enet_client_chnl_t   *chnl_handle,   
                                                           digi_enet_int_chnl_t      *filt_table_ptr,
                                                           BOOL8                     fw_retrieve_done,
                                                           digi_enet_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    enet_handle_t                   *enet_handle;
    cpb_handle_t                    *cpb_handle;
    UINT32                           cpb_port;
    UINT32                           cpb_chnl;
    UINT32                           fw_mon_id;
    digi_int_type_t                  int_type;

    PMC_ENTRY();

    if (NULL == chnl_handle)

    {
        PMC_RETURN(DIGI_ERR_NULL_HANDLE);
    }
   
    if (PMC_SUCCESS == result)
    {
        if (DIGI_CPB_PORT_TYPE_IS_ENET_LINE(chnl_handle))
        {
            enet_handle = digi_handle->enet_line_handle;
            int_type    = DIGI_INT_TYPE_ENET_LINE;
            fw_mon_id   = INTR_IPI_ENET_LINE;
            if (PMC_SUCCESS == result && chnl_handle != DIGI_INT_ALL_CHNLS)
            {
                /* check enet_line channel handle to make sure it belongs to digi handle pool */    
                result = digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle);
            }
        }
        else
        {
            enet_handle = digi_handle->enet_sys_handle;
            int_type    = DIGI_INT_TYPE_ENET_SYS;
            fw_mon_id   = INTR_IPI_ENET_SYS;
            if (PMC_SUCCESS == result && chnl_handle != DIGI_INT_ALL_CHNLS)
            {
                /* check enet_line channel handle to make sure it belongs to digi handle pool */    
                result = digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle);
            }
        }
    }

    if (PMC_SUCCESS == result)
    {
       result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
    }

    /* Get link */
    if (PMC_SUCCESS == result)
    {

        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    /* Retrieve FW interrupts for channel */
    if (PMC_SUCCESS == result)
    {
        if (FALSE == fw_retrieve_done)
        {
            /* Note: Read cache is not expected to be enabled here */
            /* Retrieve interrupts */
            result = digi_enet_int_fw_chnl_retrieve(digi_handle,
                                                    int_type,
                                                    filt_table_ptr,
                                                    int_table_ptr);
        }
    }

    if (PMC_SUCCESS == result)
    {
        /* Retreive status for enabled interrupts */
        result = enet_int_chnl_retrieve(enet_handle, 
                                        start_link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, enet),
                                        &int_table_ptr->enet);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = digi_cpb_port_chnl_get(digi_handle, chnl_handle, &cpb_port, &cpb_chnl);    
    }


    /* Retrieve CPB interrupts */
    if (PMC_SUCCESS == result)
    {
        cpb_handle = digi_handle->dcpb_handle;
        result = cpb_int_chnl_retrieve(cpb_handle,
                                       cpb_port,
                                       cpb_chnl,      
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cpb),
                                       &int_table_ptr->cpb);
    }

    PMC_RETURN(result);
} /* digi_enet_int_chnl_filt_retrieve_internal */


/*******************************************************************************
* FUNCTION: digi_enet_enhanced_pmon_mode_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves enhance enet PMON type for Mapper and ILKN handles.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - the input handle
*
* OUTPUTS:
*   pmon_type_ptr   - pointer to channel PMON table to store type of enet PMON
*
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_enhanced_pmon_mode_get(digi_handle_t               *digi_handle,
                                             util_global_switch_data_t   *chnl_handle,
                                             digi_enet_pmon_mode_t       *pmon_type_ptr)
{
    PMC_ENTRY();
    
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pmon_type_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    digi_mapper_chnl_def_t* mapper_chnl;
    digi_ilkn_chnl_def_t* ilkn_chnl;

    util_global_switch_data_def_t* in_util_ptr;
    util_global_switch_data_def_t* opp_end_ptr;

    in_util_ptr = ((util_global_switch_data_def_t*)chnl_handle);

    /* We need to look up the other end of the cpb channel (if there is one). */
    opp_end_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, chnl_handle);

    if (opp_end_ptr == NULL) {

        /* We shouldn't have gotten this far as this indicates things were not
         * mapped properly */

        *pmon_type_ptr = DIGI_ENET_PMON_MODE_NONE;

        /* This is the case for where MAPOTN goes through ETRANS */
        /* We should never get here with a real ENET_LINE channel as 
         * digi_enet_enhanced_pmon_ptr_get() would return NULL and it would
         * be gated from call this function in 
         * digi_enet_enhanced_pmon_chnl_retrieve_internal */
    } else if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) {

        if ((opp_end_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
            (opp_end_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

            /* Case 1 */
            /* This isn't an enhanced case, but this is who is performing PMON
             * on our behalf since we only have one ENET resouurce, we'll use
             * the TX stats */
            *pmon_type_ptr = DIGI_ENET_PMON_MODE_MAPPER_BIDIR;
           
        } else {
            
            *pmon_type_ptr = DIGI_ENET_PMON_MODE_NONE;
        }   
    } else if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

        /* We now now the type so cast appropriately */
        mapper_chnl = (digi_mapper_chnl_def_t*)in_util_ptr;

        if (digi_enet_is_enhanced_pmon_umbilical_case(digi_handle,
                                                      mapper_chnl,
                                                      mapper_chnl->enet_pmon) == TRUE) {

            /* Case 3 */
            *pmon_type_ptr = DIGI_ENET_PMON_MODE_MAPPER_RX_LOCAL;

        } else if (mapper_chnl->enet_pmon_data != NULL) {

            /* Case 2 */
            *pmon_type_ptr = DIGI_ENET_PMON_MODE_MAPPER_RX_REMOTE;

        } else {

            *pmon_type_ptr = DIGI_ENET_PMON_MODE_NONE;
        }

    } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
               (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {
        
        /* We now now the type so cast appropriately */
        ilkn_chnl = (digi_ilkn_chnl_def_t*)in_util_ptr;
        
        /* If the opposite end is ENET or MAP then determine where ENET is
         * taking place */
        if ((opp_end_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) ||
            (opp_end_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) ||
            (opp_end_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN)) {
            
            if (ilkn_chnl->enet_pmon_data != NULL) {
                
                /* Case 4 */
                *pmon_type_ptr = DIGI_ENET_PMON_MODE_ILKN;
                
            } else {
                
                *pmon_type_ptr = DIGI_ENET_PMON_MODE_NONE;
            }

            /* If not ENET or MAP then there is no ENET PMON */
        } 
        else {
            
            *pmon_type_ptr = DIGI_ENET_PMON_MODE_NONE;
        }
    }

    PMC_RETURN();
} /* digi_enet_enhanced_pmon_mode_get */

/*******************************************************************************
* FUNCTION: digi_enet_enhanced_pmon_chnl_retrieve_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves enhanced enet PMON table and enet PMON type for Mapper and ILKN handles.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to mapper or interlaken channel handle to retrieve
*                     ENET enhanced PMON on
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to channel PMON table to store counters 
*                     for a single channel
*   pmon_type_ptr   - pointer to channel PMON table to store type of enet PMON
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_chnl_retrieve_internal(digi_handle_t               *digi_handle,
                                                                util_global_switch_data_t   *chnl_handle,
                                                                digi_enet_pmon_chnl_t       *pmon_table_ptr,
                                                                digi_enet_pmon_mode_t       *pmon_type_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    digi_enet_client_chnl_t     *enet_handle;
    util_global_switch_data_def_t* switch_data_def;
    BOOL8                          enet_line = FALSE;
    digi_mapper_chnl_def_t         *mapper_chnl_def = NULL;
    PMC_ENTRY();
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pmon_table_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pmon_type_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    

    /* Get pointer to enet line/sys channel to obtain PMON */
    enet_handle = digi_enet_enhanced_pmon_ptr_get((util_global_switch_data_def_t*)chnl_handle);

    if (NULL == enet_handle)
    {
        /* Set enhanced PMON type as NONE and return success */
        *pmon_type_ptr = DIGI_ENET_PMON_MODE_NONE;
        PMC_RETURN(PMC_SUCCESS);
    }


    /* Retrieve enet PMON */
    if (PMC_SUCCESS == result)
    {
        switch_data_def = (util_global_switch_data_def_t*)chnl_handle;
        
        if (switch_data_def->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN)
        {
            mapper_chnl_def =  (digi_mapper_chnl_def_t*) chnl_handle;                        
            enet_line = (mapper_chnl_def->enet_pmon == DIGI_ENET_SYS_PMON)?FALSE:TRUE;            
        } else if (switch_data_def->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE ) {
            enet_line = TRUE;
        }  else if  ((switch_data_def->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) || 
                     (switch_data_def->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {
            enet_line = FALSE;
        }
            
        result = digi_enet_pmon_chnl_retrieve_internal(digi_handle, enet_handle, enet_line, pmon_table_ptr);
    }

    /* Retrieve enet PMON type */
    if (PMC_SUCCESS == result)
    {
        /* This function does not return a value. 
         * pmon_type_ptr is always set appropriately */
        digi_enet_enhanced_pmon_mode_get(digi_handle,
                                         chnl_handle,
                                         pmon_type_ptr);
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_chnl_retrieve_internal */

/*******************************************************************************
* digi_enet_channel_num_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the channel number.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to channel handle.
*
* OUTPUTS:
*   *chnl_num_ptr          - location where channel number will be written.
*
* RETURNS:
*   PMC_SUCCESS if channel number is retrieved otherwise a cesdriptive error
*   code will be returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_channel_num_get(digi_handle_t             *digi_handle,
                                           util_global_switch_data_t *chnl_handle,
                                           UINT32                    *chnl_num_ptr)
{
    enet_handle_t *enet_handle = NULL;
    PMC_ERROR rc;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle, &enet_handle, chnl_num_ptr);

    PMC_RETURN(rc);
} /* digi_enet_channel_num_get */


/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   A callback must be registered for the interrupt type before this API can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   int_summary_ptr - Active interrupt summary information (optional)
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                   digi_int_type_t           int_type,
                                                   digi_enet_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       i;
    BOOL                         int_summary_free_req = FALSE;         
    digi_enet_client_chnl_t     *chnl_handle;
    enet_handle_t               *enet_handle;
    digi_enet_client_chnl_def_t *handle_pool_ptr;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    if(!digi_int_is_cb_registered(digi_handle, int_type))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }

    /* Get handle pool ptr */
    switch (int_type)
    {
    case DIGI_INT_CHNL_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        break;

    case DIGI_INT_CHNL_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
         PMC_RETURN(PMC_SUCCESS);
    }


    /* Find active channels */ 
    if ((PMC_SUCCESS == result) &&
        (NULL == int_summary_ptr))
    {
        int_summary_ptr = (digi_enet_int_summary_t *) PMC_MALLOC(sizeof(digi_enet_int_summary_t));
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_enet_int_summary_t));
        int_summary_free_req = TRUE;
        
        result = digi_enet_int_summary_get_internal(digi_handle,
                                                    int_type,
                                                    0, /* unused */
                                                    int_summary_ptr);
    }



    /* Call user callback for all active channels */
    for (i = 0; (PMC_SUCCESS == result) && (i < int_summary_ptr->active_chnl_num); i++)
    {
        chnl_handle = int_summary_ptr->active_chnl[i];

        result = digi_int_callback_run(digi_handle, 
                                       chnl_handle,
                                       int_type);
    }
        
    
    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }

    PMC_RETURN(result);
} /* digi_enet_int_chnl_enabled_check */


/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_enabled_check_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in provided channel of the subsystem.
*   Returns if interrupt is found.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to check interrupts
*   int_type        - ENET line or sys
*
*   
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_chnl_enabled_check_internal(digi_handle_t           *digi_handle,
                                                            digi_enet_client_chnl_t *chnl_handle,
                                                            digi_int_type_t          int_type,
                                                            BOOL                    *int_found_ptr)
{

    PMC_ERROR                    result = PMC_SUCCESS;
    enet_handle_t               *enet_handle;
    digi_enet_int_t             *int_en_table_ptr;
    UINT32                       start_link = 0;
    UINT32                       end_link   = 0;
    UINT32                       i;
    digi_enet_client_chnl_def_t *handle_pool_ptr;
    BOOL8                        multichannels; 
    BOOL8                        bool8_found_ptr = *int_found_ptr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);


    
    /* Get handle pool ptr */
    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        int_type = DIGI_INT_TYPE_ENET_LINE;
        if (DIGI_INT_ALL_CHNLS != chnl_handle)
        {
            result = digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle);
        }
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;  
        int_type = DIGI_INT_TYPE_ENET_SYS;
        if (DIGI_INT_ALL_CHNLS != chnl_handle)
        {
            result = digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle);
        }

        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {

        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    if (IS_DIGI_INT_ALL_CHNLS(chnl_handle))
    {
        multichannels = TRUE;
    }
    else
    {
        multichannels = FALSE;
    }
    
    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result) && (FALSE == bool8_found_ptr); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            /* Check enabled interrupts */
            result = enet_int_chnl_enabled_check(enet_handle, 
                                                 i,
                                                 &int_en_table_ptr->enet_chnl[i].enet,
                                                 &bool8_found_ptr,
                                                 multichannels);
            /* retrieve FW interrupt state */
            if (PMC_SUCCESS == result &&
                FALSE == bool8_found_ptr)
            {
                result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_enabled_check(digi_handle,
                                                                              int_type,
                                                                              TRUE,
                                                                              TRUE,
                                                                              &int_en_table_ptr->enet_chnl[i].enet,
                                                                              &bool8_found_ptr);
                
            }
        }
    }

    *int_found_ptr = bool8_found_ptr;

#ifdef PMC_SW_SIMULATION
        /* Force found interrupt for simulation so that callback runs */
        *int_found_ptr = TRUE;
#endif


    PMC_RETURN(result);
} /* digi_enet_int_chnl_enabled_check_internal */



/*******************************************************************************
* FUNCTION: digi_enet_int_summary_get_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of ENET interrupts
*   
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   int_type        - ENET line or sys
*   unused          - Unused
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_summary_get_internal(digi_handle_t            *digi_handle,
                                                     digi_int_type_t           int_type,
                                                     UINT32                    unused,
                                                     digi_enet_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    digi_otn_int_t                  *int_en_table_ptr;
    UINT32                           i, j;
    enet_handle_t                   *enet_handle;
    digi_enet_client_chnl_def_t     *handle_pool_ptr;
    digi_enet_client_chnl_t         *chnl_handle;
    BOOL                             int_found;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    if (NULL == int_summary_ptr)
    {
        result = DIGI_ERR_NULL_HANDLE;
    }


    /* Get handle pool ptr */
    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
    case DIGI_INT_CHNL_TYPE_ENET_LINE:
        int_type    = DIGI_INT_TYPE_ENET_LINE;
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        break;

    case DIGI_INT_TYPE_ENET_SYS:
    case DIGI_INT_CHNL_TYPE_ENET_SYS:
        int_type    = DIGI_INT_TYPE_ENET_SYS;
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }


    /* Get range of handles */ 
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);

    /* Find channels with active interrupts */
    j = 0;
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        int_found   = FALSE;
        chnl_handle = &handle_pool_ptr[i];

        if (util_global_is_chnl_prov(chnl_handle))
        {
            result = digi_enet_int_chnl_enabled_check_internal(digi_handle,
                                                               chnl_handle,
                                                               int_type,
                                                               &int_found);
        }

        if ((TRUE == int_found) && (PMC_SUCCESS == result))
        {
            int_summary_ptr->active_chnl[j] = &handle_pool_ptr[i];
            j++;
        }
    }

    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);
    
    if (PMC_SUCCESS == result)
    {
        int_summary_ptr->active_chnl_num = j;
        
        int_summary_ptr->mask = DIGI_ENET_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM |
            DIGI_ENET_INT_SUMMARY_MASK_ACTIVE_CHNL;
    }


    PMC_RETURN(result);
} /* digi_enet_int_summary_get_internal */




/*******************************************************************************
* digi_enet_mcpb_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions the MCPB channel without init/de-init of the MGMT FEGE
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*    egr_port           - type of the egress port
*    egr_chnl           - egress channel
*    ingr_port          - type of the ingress port
*    ingr_chnl          - ingres channel 
*    enable             - TRUE to enable channel, otherwise channel is disabled. 
*    rate_data          - enum, client stream types of signal passing through CPB.
*    dst_cpb_port_type  - CPB port to prov/deprov
* 
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_mcpb_chnl_cfg(digi_handle_t                     *digi_handle,
                                          cpb_mcpb_egr_output_port_type_t    egr_port,
                                          UINT32                             egr_chnl,
                                          cpb_mcpb_ingr_input_port_type_t    ingr_port,
                                          UINT32                             ingr_chnl,
                                          BOOL8                              enable,
                                          digi_cpb_stream_t                  rate_data,
                                          util_global_mcpb_port_t            dst_cpb_port_type)
{    
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 rate;
    util_schd_chnl_rate_exp_t rate_unit;
    UINT32 cpb_data;
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (rc == PMC_SUCCESS) {
        if (TRUE == enable)
        {
            /** get MCPB channel rate */
            rc = digi_cpb_rate_get(rate_data, 0, &rate, &rate_unit, &cpb_data);

            /* Initialize MCPB MUX mode to select ENET LINE or ENET SYS, */
            /* both cannot be connected to MCPB at the same time */
            if (rc == PMC_SUCCESS) {                
                rc = digi_mcpb_enet_mux_mode_init(digi_handle,
                                                  dst_cpb_port_type,
                                                  TRUE);
                PMC_ATOMIC_YIELD(digi_handle, 0);                                                                                                  
            }

            /*  provision MCPB port */
            if (rc == PMC_SUCCESS) {                
                rc = digi_mcpb_chnl_prov(digi_handle,
                                         egr_port,
                                         egr_chnl,
                                         ingr_port,
                                         ingr_chnl,
                                         rate);
               PMC_ATOMIC_YIELD(digi_handle, 0);
            }                                            
        }
        else
        {
            /*  de-provision MCPB port */
            rc = digi_mcpb_chnl_deprov(digi_handle,
                                       egr_port,
                                       egr_chnl,
                                       ingr_port,
                                       ingr_chnl);  
            PMC_ATOMIC_YIELD(digi_handle, 0);                                                                                 
            /* Conditionally Uninitialize MCPB MUX mode if no ENET_LINE/SYS 
               connections*/
            if (rc == PMC_SUCCESS) {                
                rc = digi_mcpb_enet_mux_mode_init (digi_handle,
                                           dst_cpb_port_type,
                                           FALSE);
            }  
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_mcpb_chnl_cfg */


/*******************************************************************************
* digi_enet_mgmt_fege_to_enet_10g_fifo_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function tests all 10GE rate enet channels for MCPB FIFO size in 
*   the MGMT_FEGE to ENET packet insertion path.  It identifies if any 
*   path has the larger FIFO assigned to it.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI slice handle instance.
*   *enet_handle            - ENET module handle
*   mcpb_egr_port           - MCPB egress port to test
* 
* OUTPUTS:
*   link_ptr                - If the return value is TRUE, the 10GE rate 
*                             channel link that has a large FIFO.
*
* RETURNS:
*   BOOL8                  TRUE: link_ptr is valid, it is a 10GE rate channel
*                          with large FIFO, FALSE: no 10GE channel with large
*                          FIFO is present
*
* NOTES:
*   
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_mgmt_fege_to_enet_10g_fifo_data_get(digi_handle_t *digi_handle, 
                                                           enet_handle_t *enet_handle,
                                                           cpb_mcpb_egr_output_port_type_t mcpb_egr_port, 
                                                           UINT32 *link_ptr)
{

    UINT32 i = 0, pop_port_id = 0;
    enet_datapath_t datapath = ENET_UNPROV_DATAPATH;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    switch(mcpb_egr_port)
    {
        case CPB_MCPB_EGR_LINE_ING:
        case CPB_MCPB_EGR_SYS_ING:
            pop_port_id = 0;
            break;
        case CPB_MCPB_EGR_LINE_EG:
        case CPB_MCPB_EGR_SYS_EG:
            pop_port_id = 1;
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            break;
    }


    for(i = 0; i < ENET_NUM_LINK_MAX; i++)
    {
        if(TRUE == digi_enet_is_link_mac_block(enet_handle, i))
        {
            enet_convert_link_to_datapath_type(enet_handle, i, &datapath);
            if(ENET_SERDES_10GE_MAC                == datapath ||
               ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC == datapath ||
               ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC == datapath ||
               ENET_MAPOTN_BMP_GSUP43_10GE_MAC     == datapath)
            {
                UINT32 read_qid = i*2 + pop_port_id;
                UINT32 num_current_blocks = 0;
                (void)cpb_queue_num_block_get(digi_handle->mcpb_handle,
                                              pop_port_id,
                                              read_qid,
                                              &num_current_blocks);
                if(DIGI_ENET_MCPB_10GE_LARGE_FIFO_NUM_BLOCKS == num_current_blocks)
                {
                    *link_ptr = i;
                    PMC_RETURN(TRUE);
                }
            }
        }
    }

    PMC_RETURN(FALSE);

} /* digi_enet_mgmt_fege_to_enet_10g_fifo_data_get */

/*******************************************************************************
* digi_enet_10g_to_mgmt_fege_fifo_data_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function tests all 10GE rate enet channels for MCPB FIFO size in 
*   the ENET to MGMT_FEGE packet insertion path.  It identifies if any 
*   path has the larger FIFO assigned to it.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI slice handle instance.
*   *enet_handle            - ENET module handle
*   push_port_id            - Push Port ID, valid values 0, 1
* 
* OUTPUTS:
*   link_ptr                - If the return value is TRUE, the 10GE rate 
*                             channel link that has a large FIFO.
*
* RETURNS:
*   BOOL8                  TRUE: link_ptr is valid, it is a 10GE rate channel
*                          with large FIFO, FALSE: no 10GE channel with large
*                          FIFO is present
*
* NOTES:
*   
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_10g_to_mgmt_fege_fifo_data_get(digi_handle_t *digi_handle,
                                                      enet_handle_t *enet_handle,
                                                      UINT32 push_port_id, 
                                                      UINT32 *link_ptr)
{

    UINT32 i = 0, pop_port_id = 0;
    enet_datapath_t datapath = ENET_UNPROV_DATAPATH;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    PMC_ASSERT(push_port_id < 2, DIGI_ERR_CODE_ASSERT, 0, 0);


    for(i = 0; i < ENET_NUM_LINK_MAX; i++)
    {
        if(TRUE == digi_enet_is_link_mac_block(enet_handle, i))
        {
            enet_convert_link_to_datapath_type(enet_handle, i, &datapath);
            if(ENET_SERDES_10GE_MAC                == datapath ||
               ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC == datapath ||
               ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC == datapath ||
               ENET_MAPOTN_BMP_GSUP43_10GE_MAC     == datapath)
            {
                UINT32 read_qid = i + 12*push_port_id;
                UINT32 num_current_blocks = 0;
                (void)cpb_queue_num_block_get(digi_handle->mcpb_handle,
                                              pop_port_id,
                                              read_qid,
                                              &num_current_blocks);
                if(DIGI_ENET_MCPB_10GE_LARGE_FIFO_NUM_BLOCKS == num_current_blocks)
                {
                    *link_ptr = i;
                    PMC_RETURN(TRUE);
                }
            }
        }
    }

    PMC_RETURN(FALSE);

} /* digi_enet_10g_to_mgmt_fege_fifo_data_get */

/*******************************************************************************
* digi_enet_insert_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from the CPU or MGMT FEGE 
*   to a ENET
*   
*
* INPUTS:
*   digi_handle             - pointer to DIGI handle instance.
*   chnl                    - channel ID   
*   mcpb_egr_port           - MCPB egress port
*   mcpb_ing_port           - MCPB ingress port
*   enable                  - TRUE to enable channel, otherwise channel is disabled.
*   rate_data               - enum, client stream types of signal passing through CPB.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_insert_chnl_cfg(digi_handle_t                     *digi_handle,
                                            UINT32                             chnl,
                                            cpb_mcpb_egr_output_port_type_t    mcpb_egr_port,
                                            cpb_mcpb_ingr_input_port_type_t    mcpb_ing_port,
                                            BOOL8                              enable,
                                            digi_cpb_stream_t                  rate_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mcpb_ing_chnl;
    UINT32 mcpb_egr_cnhl;
    util_global_mcpb_port_t dst_mcpb_port = LAST_UTIL_GLOBAL_MCPB_PORT;
    
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);   

    /* define MCPB ingress and egress channels */
    if (PMC_SUCCESS == rc) 
    {
        switch (mcpb_egr_port)
        {
        case CPB_MCPB_EGR_LINE_ING:
        {
            mcpb_ing_chnl = (chnl * 2) + 1;
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
            break;
        }
        case CPB_MCPB_EGR_SYS_ING: {
            mcpb_ing_chnl = (chnl * 2) + 1;
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
            break;
        }
            
        case CPB_MCPB_EGR_LINE_EG:
        {
            mcpb_ing_chnl = (chnl * 2);
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
            break;
        }
        case CPB_MCPB_EGR_SYS_EG: {
            mcpb_ing_chnl = (chnl * 2);
            mcpb_egr_cnhl = chnl;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
            break;
        }
        case CPB_MCPB_EGR_CPU: {
            mcpb_ing_chnl = DIGI_MGMT_FEGE_TO_CPU_ING_DPI_ID;
            mcpb_egr_cnhl = DIGI_MGMT_FEGE_TO_CPU_EGR_DPI_ID;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE;
            break;
        }
        case CPB_MCPB_EGR_FEGE: {
            mcpb_ing_chnl = DIGI_CPU_TO_MGMT_FEGE_ING_DPI_ID;
            mcpb_egr_cnhl = DIGI_CPU_TO_MGMT_FEGE_EGR_DPI_ID;
            dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_CPU;
            break;
        }
        default: {
            rc = DIGI_ERR_UNKNOWN;
            break;
        }              
        }
    }
    if (PMC_SUCCESS == rc && TRUE == enable)
    {        
        digi_handle->var.mcpb_ctxt.mcpb_cpu_port_cfg = enable;
    }
    /* configure MCPB channels */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_enet_mcpb_chnl_cfg(digi_handle, 
                                     mcpb_egr_port,
                                     mcpb_egr_cnhl,
                                     mcpb_ing_port,
                                     mcpb_ing_chnl,
                                     enable,
                                     rate_data,
                                     dst_mcpb_port);
    }
    /** save context
     *  @bug make sure all DPI channels are de-proved
     */
    if (PMC_SUCCESS == rc && FALSE == enable)
    {
        digi_handle->var.mcpb_ctxt.mcpb_cpu_port_cfg = enable;
    }

    PMC_RETURN(rc);
} /* digi_enet_insert_chnl_cfg */

/*******************************************************************************
* digi_enet_extract_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The function enables or disables packets forwarding from a ENET to CPU or to 
*   the MGMT FEGE.

* INPUTS:
*   digi_handle             - pointer to DIGI handle instance.
*   chnl                    - channel ID   
*   mcpb_egr_port           - MCPB egress port
*   mcpb_ing_port           - MCPB ingress port
*   enable                  - TRUE to enable channel, otherwise channel is disabled. 
*   rate_data               - enum, client stream types of signal passing through CPB.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES:
*   
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_extract_chnl_cfg(digi_handle_t                     *digi_handle,
                                             UINT32                             chnl,
                                             cpb_mcpb_egr_output_port_type_t    mcpb_egr_port,
                                             cpb_mcpb_ingr_input_port_type_t    mcpb_ing_port,
                                             BOOL8                              enable,
                                             digi_cpb_stream_t                  rate_data)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mcpb_egr_chnl;
    UINT32 mcpb_ing_chnl;
    util_global_mcpb_port_t dst_mcpb_port = LAST_UTIL_GLOBAL_MCPB_PORT;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* define MCPB egress dpi ID */
    switch (mcpb_ing_port)
    {
    case CPB_MCPB_ING_LINE_IM_EXT:
    {
        mcpb_egr_chnl = (chnl << 2) + 1;
        mcpb_ing_chnl = (chnl << 1);
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
        break;
    }
    case CPB_MCPB_ING_SYS_IM_EXT:
    {
        mcpb_egr_chnl = (chnl << 2) + 1;
        mcpb_ing_chnl = (chnl << 1);
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
        break;
    }
    case CPB_MCPB_ING_LINE_EM_EXT: 
    {
        mcpb_egr_chnl = (chnl << 2);
        mcpb_ing_chnl = (chnl << 1);
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_LINE;
        break;
    }        
    case CPB_MCPB_ING_SYS_EM_EXT: 
    {
        mcpb_egr_chnl = (chnl << 2);
        mcpb_ing_chnl = (chnl << 1);
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_ENET_SYS;
        break;
    }
    case CPB_MCPB_ING_CPU_INS:
    {
        mcpb_egr_chnl = DIGI_MGMT_FEGE_TO_CPU_ING_DPI_ID;
        mcpb_ing_chnl = DIGI_MGMT_FEGE_TO_CPU_EGR_DPI_ID;
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_MGMT_FEGE;
        break;
    }
    case CPB_MCPB_ING_FEGE_INS: 
    {
        mcpb_egr_chnl = DIGI_CPU_TO_MGMT_FEGE_EGR_DPI_ID;
        mcpb_ing_chnl = DIGI_CPU_TO_MGMT_FEGE_ING_DPI_ID;
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_CPU;
        break;
    }
    case CPB_MCPB_ING_LINE_REF:
    {
        mcpb_egr_chnl = (chnl << 2) + 2;
        mcpb_ing_chnl = chnl;
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_CPU;
        break;
    }
    case CPB_MCPB_ING_SYS_REF:
    {
        mcpb_egr_chnl = (chnl << 2) + 2;
        mcpb_ing_chnl = chnl;
        dst_mcpb_port = UTIL_GLOBAL_MCPB_PORT_CPU;
        break;
    }
    default:
    {
        rc = DIGI_ERR_UNKNOWN;
        break;
    }
    }
    if ((PMC_SUCCESS == rc) && (TRUE == enable))
    {        
        digi_handle->var.mcpb_ctxt.mcpb_cpu_port_cfg = enable;
    }
    /* configure MCPB channels */
    if (PMC_SUCCESS == rc)
    {
        rc = digi_enet_mcpb_chnl_cfg (digi_handle, 
                                      mcpb_egr_port,
                                      mcpb_egr_chnl,
                                      mcpb_ing_port,
                                      mcpb_ing_chnl,
                                      enable,
                                      rate_data,
                                      dst_mcpb_port);
    }
    /** save context
     *  @bug make sure all DPI channels are de-proved
     */
    if (PMC_SUCCESS == rc && FALSE == enable)
    {
        digi_handle->var.mcpb_ctxt.mcpb_cpu_port_cfg = enable;
    }
    PMC_RETURN(rc);
} /* digi_enet_extract_chnl_cfg */

/*******************************************************************************
* FUNCTION: digi_enet_pmon_chnl_retrieve_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel PMON table with PMON counters
*   for a single channel in the subsystem.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*   enet_line       - enet line or sys
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to channel PMON table to store counters 
*                     for a single channel
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR digi_enet_pmon_chnl_retrieve_internal(digi_handle_t              *digi_handle,
                                                digi_enet_client_chnl_t    *chnl_handle,   
                                                BOOL8                       enet_line,
                                                digi_enet_pmon_chnl_t      *pmon_table_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       link;
    digi_pmon_t                 *fw_pmon_ptr;
    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    
    PMC_ENTRY();
    result = digi_pmon_chnl_param_check(digi_handle, chnl_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr); 
    }


    if (PMC_SUCCESS == result)
    {        
        link = chnl_ptr->switch_data.cpb_data.channel;

        if (TRUE == enet_line)
        {
#ifdef DIGI_ENET_PMON 
#undef DIGI_ENET_PMON
#endif
#define DIGI_ENET_PMON(tbl_name)                                        \
            do {                                                        \
                PMC_MEMCPY(&pmon_table_ptr->enet_##tbl_name,            \
                           &fw_pmon_ptr->enet_line_##tbl_name[link],    \
                           sizeof(pmon_table_ptr->enet_##tbl_name));    \
            } while(0)
            DIGI_ENET_PMON_TABLE_DEFINE();
        }
        else
        {
#ifdef DIGI_ENET_PMON 
#undef DIGI_ENET_PMON
#endif
#define DIGI_ENET_PMON(tbl_name)                                        \
            do {                                                        \
                PMC_MEMCPY(&pmon_table_ptr->enet_##tbl_name,            \
                           &fw_pmon_ptr->enet_sys_##tbl_name[link],     \
                           sizeof(pmon_table_ptr->enet_##tbl_name));    \
            } while(0)

            DIGI_ENET_PMON_TABLE_DEFINE();
        }
    }



    
    PMC_RETURN(result);
} /* digi_enet_pmon_chnl_retrieve_internal */




/*******************************************************************************
* FUNCTION: digi_enet_int_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   ENET channel.
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one channel.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_register_cb(digi_handle_t      *digi_handle,
                                            digi_int_type_t     int_type,
                                            void               *token,
                                            UINT32              token_size,
                                            digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
    case DIGI_INT_TYPE_ENET_SYS:
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */
    


    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

    PMC_RETURN(result);
} /* digi_enet_int_register_cb */




/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.\n\n
*
*   When ENET subsystem provisioning mode is 
*   DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON, the interrupt
*   int_table_ptr->epmm.gsup43_c73.hi_ber_i is not available, it must be
*   accessed via int_table_ptr->emac.hi_ber_i.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     enable interrupts
*   enable          - Enable or disable the set bits in the interrupt table
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_chnl_enable(digi_handle_t             *digi_handle,
                                            digi_int_type_t            int_type,
                                            digi_enet_client_chnl_t   *chnl_handle,   
                                            enet_int_chnl_t           *int_table_ptr,
                                            BOOL8                       enable)
{
    PMC_ERROR           result = PMC_SUCCESS;

    enet_handle_t      *enet_handle;
    UINT32              start_link = 0;
    UINT32              end_link = 0;
    UINT32              i;
    digi_enet_int_t     *int_en_table_ptr;
    
    
    PMC_ENTRY();
    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 

    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
      
        /* Callback for interrupt table must be registered before enable ints */
        if (PMC_SUCCESS == result && TRUE == enable)
        {
            if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_ENET_LINE) &&
               !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_ENET_LINE))
            {
                result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
            }
        }
        if (PMC_SUCCESS == result && chnl_handle != DIGI_INT_ALL_CHNLS)
        {
            /* check enet_line channel handle to make sure it belongs to digi handle pool */    
            result = digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle);
        }
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        
        /* Callback for interrupt table must be registered before enable ints */
        if (PMC_SUCCESS == result && TRUE == enable)
        {
            if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_ENET_SYS) &&
               !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_ENET_SYS))
            {
                result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
            }
        }
        if (PMC_SUCCESS == result && chnl_handle != DIGI_INT_ALL_CHNLS)
        {
            /* check enet_line channel handle to make sure it belongs to digi handle pool */    
            result = digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle);
        }
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }


    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        /* Enable interrupts */
        result = enet_int_chnl_enable(enet_handle, 
                                      i,
                                      int_table_ptr,
                                      &int_en_table_ptr->enet_chnl[i].enet,
                                      enable,
                                      IS_DIGI_INT_ALL_CHNLS(chnl_handle));
    }
    
    PMC_RETURN(result);
} /* digi_enet_int_chnl_enable */






/*******************************************************************************
* FUNCTION: digi_enet_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     clear interrupts
*                     DIGI_INT_ALL_INTS to clear all interrupts.
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_clear(digi_handle_t             *digi_handle,
                                      digi_int_type_t            int_type,
                                      digi_enet_int_t           *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    enet_handle_t               *enet_handle;
    cpb_handle_t                *cpb_handle;
    digi_enet_int_t             *int_en_table_ptr;
    UINT32                       start_link = 0;
    UINT32                       end_link = 0;
    UINT32                       i;
    UINT32                       cpb_port;
    UINT32                       cpb_chnl;
    digi_enet_client_chnl_def_t *handle_pool_ptr;
    digi_enet_client_chnl_t     *chnl_handle;

    
    PMC_ENTRY();
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);


    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    cpb_handle   = digi_handle->dcpb_handle;


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         DIGI_INT_CHNL_TYPE_ENET_LINE,
                                         &start_link,
                                         &end_link);
    }
    
    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear interrupts */
    if (IS_DIGI_INT_ALL_INTS(int_table_ptr))
    {
        result = digi_enet_int_chnl_clear(digi_handle,
                                          int_type,
                                          DIGI_INT_ALL_CHNLS,
                                          DIGI_INT_ALL_INTS);
    }
    else
    {
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = int_table_ptr->handle[i];

            if (NULL != chnl_handle)
            {
                if (PMC_SUCCESS == result)
                {
                    result = enet_int_chnl_clear(enet_handle, 
                                                 i,
                                                 UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, enet_chnl[i].enet),
                                                 &int_en_table_ptr->enet_chnl[i].enet,
                                                 TRUE);
                }
                
                if (PMC_SUCCESS == result)
                {
                    result = digi_cpb_port_chnl_get(digi_handle, chnl_handle, &cpb_port, &cpb_chnl);    
                }

                /* Clear CPB interrupts */
                if (PMC_SUCCESS == result)
                {
                    result = cpb_int_chnl_clear(cpb_handle,
                                                cpb_port,
                                                cpb_chnl,      
                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, enet_chnl[i].cpb),
                                                &int_en_table_ptr->enet_chnl[i].cpb);
                }
            }
        }
    }
    
    PMC_RETURN(result);
} /* digi_enet_int_clear */


/*******************************************************************************
* FUNCTION: digi_enet_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears channel interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     clear interrupts
*                     DIGI_INT_ALL_INTS to clear all interrupts.
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_chnl_clear(digi_handle_t             *digi_handle,
                                           digi_int_type_t            int_type,
                                           digi_enet_client_chnl_t   *chnl_handle,
                                           digi_enet_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    enet_handle_t      *enet_handle;
    cpb_handle_t       *cpb_handle;
    digi_enet_int_t    *int_en_table_ptr;
    digi_enet_client_chnl_def_t  *handle_pool_ptr;
    UINT32              start_link = 0;
    UINT32              end_link = 0;
    UINT32              i;
    UINT32              cpb_port;
    UINT32              cpb_chnl;
    BOOL8               multichannels;
    
    
    PMC_ENTRY();
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);


    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    cpb_handle = digi_handle->dcpb_handle;


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    if (IS_DIGI_INT_ALL_CHNLS(chnl_handle))
    {
        multichannels = TRUE;
    }
    else
    {
        multichannels = FALSE;
    }
    
    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear interrupts */
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            /* Clear enet interrupts */
            if (PMC_SUCCESS == result)
            {
                result = enet_int_chnl_clear(enet_handle, 
                                             i,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, enet),
                                             &int_en_table_ptr->enet_chnl[i].enet,
                                             multichannels);
            }

            if (PMC_SUCCESS == result)
            {
                result = digi_cpb_port_chnl_get(digi_handle, chnl_handle, &cpb_port, &cpb_chnl);    
            }

            /* Clear CPB interrupts */
            if (PMC_SUCCESS == result)
            {
                result = cpb_int_chnl_clear(cpb_handle,
                                            cpb_port,
                                            cpb_chnl,      
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cpb),
                                            &int_en_table_ptr->enet_chnl[i].cpb);
            }
        }
    }

    
    PMC_RETURN(result);
} /* digi_enet_int_chnl_clear */


/*******************************************************************************
* FUNCTION: digi_enet_int_filt_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.\n\n
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.\n\n
*
*   When ENET subsystem provisioning mode is 
*   DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON, the interrupt
*   int_table_ptr->epmm.gsup43_c73.hi_ber_i is not available, it must be
*   accessed via int_table_ptr->emac.hi_ber_i.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_filt_retrieve(digi_handle_t         *digi_handle,
                                              digi_int_type_t        int_type,
                                              digi_enet_int_chnl_t  *filt_table_ptr,
                                              digi_enet_int_t       *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;

    enet_handle_t                   *enet_handle;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    UINT32                           fw_mon_id;
    digi_enet_int_t                 *int_en_table_ptr;
    digi_enet_client_chnl_def_t     *handle_pool_ptr;
    digi_enet_client_chnl_t         *chnl_ptr;
    
    PMC_ENTRY();
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_enet_int_t));

    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        fw_mon_id = INTR_IPI_ENET_LINE;
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        fw_mon_id = INTR_IPI_ENET_SYS;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }



    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
   

    /* Retrieve FW interrupts for all channels */
    if (PMC_SUCCESS == result)
    {
        /* Retrieve interrupts */
        result = digi_enet_int_fw_retrieve(digi_handle,
                                           int_type,
                                           filt_table_ptr,
                                           int_table_ptr);
    }
    
    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];

        if (util_global_is_chnl_prov(chnl_ptr))
        {
            int_table_ptr->handle[i] = chnl_ptr; 

            result = digi_enet_int_chnl_filt_retrieve_internal(digi_handle,
                                                               chnl_ptr,
                                                               filt_table_ptr,
                                                               TRUE,
                                                               &int_table_ptr->enet_chnl[i]);
        }
    }

    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);
    
    PMC_RETURN(result);
} /* digi_enet_int_filt_retrieve */

/*******************************************************************************
* FUNCTION: digi_enet_int_enabled_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with enabled interrupt bits
*   over multiple channels.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store enabled interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_enabled_get(digi_handle_t       *digi_handle,
                                            digi_int_type_t      int_type,
                                            digi_enet_int_t     *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;

    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    digi_enet_int_t                 *int_en_table_ptr;
    digi_enet_client_chnl_def_t     *handle_pool_ptr;
    digi_enet_client_chnl_t         *chnl_ptr;
    
    
    PMC_ENTRY();
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_enet_int_t));

    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */





    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
   

    if (PMC_SUCCESS == result)
    {
        /* Copy enabled table to output */
        PMC_MEMCPY(int_table_ptr, int_en_table_ptr, sizeof(*int_table_ptr));
    }

    
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];
        int_table_ptr->handle[i] = NULL;

        if (util_global_is_chnl_prov(chnl_ptr))
        {
            /* Store handle */
            int_table_ptr->handle[i] = chnl_ptr; 
        }
    }

    PMC_RETURN(result);
} /* digi_enet_int_enabled_get */




/*******************************************************************************
* FUNCTION: digi_enet_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_enabled_check(digi_handle_t      *digi_handle,
                                              digi_int_type_t     int_type)
{
    PMC_ERROR                        result = PMC_SUCCESS;

    enet_handle_t                   *enet_handle;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    digi_enet_int_t                 *int_en_table_ptr;
    BOOL                             int_found = FALSE;
    digi_enet_client_chnl_def_t     *handle_pool_ptr;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    

    switch (int_type)
    {
    case DIGI_INT_TYPE_ENET_LINE:
        enet_handle = digi_handle->enet_line_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
        break;

    case DIGI_INT_TYPE_ENET_SYS:
        enet_handle = digi_handle->enet_sys_handle;
        handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
        break;
        
    default:
        PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    } /* end switch */

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, int_type))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Determine if are accessing one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
    
    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);

    if (PMC_SUCCESS == result)
    {
        result = digi_enet_int_chnl_enabled_check_internal(digi_handle,
                                                           DIGI_INT_ALL_CHNLS,
                                                           int_type,
                                                           &int_found);
    }

    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);


    
#ifdef PMC_SW_SIMULATION
    /* Force found interrupt for simulation so that callback runs */
    int_found = TRUE;
#endif

    if ((PMC_SUCCESS == result) &&
        (TRUE == int_found))
    {
        result = digi_int_callback_run(digi_handle, DIGI_INT_ALL_CHNLS, int_type);
    }


    PMC_RETURN(result);
} /* digi_enet_int_enabled_check */


/*******************************************************************************
* digi_enet_post_mld_dsi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine Post MDL DSI based link and the group
*   size.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   link              - link Valid values 0 to 11.
*   group_size        - mumber of 10G secondary groups required for the mapper
*                       application see enum type digi_group_size_t
*
*
* OUTPUTS:
*   *dsi_mask_ptr     - mask to identify the set of post MLD DSI lanes assigned
*                       to for processing a Serdes Signal
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_enet_post_mld_dsi_get(digi_handle_t *digi_handle,
                                       UINT32 link,
                                       digi_group_size_t group_size,
                                       UINT16 *dsi_mask_ptr)
{
    /*
      concern:  we are naming this 'post_mld_dsi', we are asking for entries in the
      'shadow' of this 'post_mld_dsi', is this the right name for the API or do we
      bring in concept of shadow?
    */

    UINT16 dsi_mask = 0;
    PMC_ENTRY();

    PMC_ASSERT(group_size < LAST_DIGI_GRP_SIZE,DIGI_ERR_INVALID_ARG, 0, 0);

    switch(group_size)
    {
    case DIGI_GRP_SIZE_10G:
        dsi_mask |= DIGI_POST_MLD_DSI_10G_BITMASK;
        break;
    case DIGI_GRP_SIZE_40G:
        dsi_mask |= DIGI_POST_MLD_DSI_40G_BITMASK;
        break;
    case DIGI_GRP_SIZE_100G:
        dsi_mask |= DIGI_POST_MLD_DSI_100G_BITMASK;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }
    dsi_mask = dsi_mask << link;
    *dsi_mask_ptr = dsi_mask;
    PMC_LOG_TRACE("dsi_mask_ptr is:%d\n",dsi_mask);

    PMC_RETURN();
} /* digi_enet_post_mld_dsi_get */


/*******************************************************************************
* digi_enet_validate_parms
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the two handles as well as the channel
*             structure.  If these are valid, then then link number is obtained
*             and validated.  This function returns a pointer to the ENET
*             handle (LINE or SYS) and the link number.
*             If any validation fails, this function asserts.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to ENET or MAPOTN channel handle instance
*
* OUTPUTS:
*   **enet_pptr            - pointer to a pointer to ENET handle (LINE or SYS)
*   *link_ptr              - pointer to link number
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_validate_parms(digi_handle_t               *digi_handle,
                                          util_global_switch_data_t   *chnl_handle,
                                          enet_handle_t              **enet_pptr,
                                          UINT32                      *link_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
    UINT32 link;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    
    /* check enet_line channel handle to make sure it belongs to digi handle pool */    
    rc = digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle);

    if (PMC_SUCCESS != rc)
    {
        rc = digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle);
    }

    if (PMC_SUCCESS != rc)
    {
        rc = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);
    }

    if (PMC_SUCCESS == rc)
    {
        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle);
        
        link = chnl_ptr->switch_data.cpb_data.channel;
        
        switch (chnl_ptr->switch_data.cpb_data.port_type) {
        case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
            *enet_pptr = digi_handle->enet_line_handle;
            if (TRUE != digi_enet_is_link_valid(digi_handle->enet_line_handle, link))
            {
                rc = DIGI_ERR_INVALID_ARG;
            }
            break;
        case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
            *enet_pptr = digi_handle->enet_sys_handle;
            if (TRUE != digi_enet_is_link_valid(digi_handle->enet_sys_handle, link))
            {
                rc = DIGI_ERR_INVALID_ARG;
            }
            break;
        case UTIL_GLOBAL_CPB_PORT_MAPOTN:
            rc = DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET; 
            break;
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
        
        *link_ptr = link;
    }

    PMC_RETURN(rc);
} /* digi_enet_validate_parms */

/*******************************************************************************
* digi_enet_channel_mcpb_clean_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function tests a channel handle to determine if ENET SS link resources
*   are being used and if used whether MCPB paths are provisioned against the
*   ENET SS link.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to ENET or MAPOTN channel handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - on success, otherwise descriptive error code
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_channel_mcpb_clean_test(digi_handle_t               *digi_handle,
                                                   util_global_switch_data_t   *chnl_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle,
                                  chnl_handle,
                                  &enet_ss_handle_ptr,
                                  &link);

    /* determine MCPB mux initialization state and if initialized the ENET 
       subsystem being selected */
    if(rc == PMC_SUCCESS)
    {
        if(digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init == TRUE)
        {
            switch(cpb_mcpb_mux_mode_init_get(digi_handle->mcpb_handle))
            {
                case CPB_MCPB_MUX_MODE_ENET_LINE:
                    if(enet_ss_handle_ptr == digi_handle->enet_line_handle)
                    {
                        if(FALSE == cpb_mcpb_enet_channel_start_state_test(digi_handle->mcpb_handle, link))
                        {
                            rc = DIGI_ERR_MCPB_PATH_STILL_CONNECTED;
                        }
                    }
                    break;
                case CPB_MCPB_MUX_MODE_ENET_SYS:
                    if(enet_ss_handle_ptr == digi_handle->enet_sys_handle)
                    {
                        if(FALSE == cpb_mcpb_enet_channel_start_state_test(digi_handle->mcpb_handle, link))
                        {
                            rc = DIGI_ERR_MCPB_PATH_STILL_CONNECTED;
                        }
                    }
                    break;
                default:
                    PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                    break;
            }    
        } 
    }

    if(rc == DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET)
        rc = PMC_SUCCESS;

    PMC_RETURN(rc);
} /* digi_enet_channel_mcpb_clean_test */

/*******************************************************************************
* digi_enet_channel_mcpb_clean_filtered_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function tests a channel handle to determine if ENET SS link resources
*   are being used and if used whether MCPB paths are provisioned against the
*   ENET SS link for the filtered resources defined.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to ENET or MAPOTN channel handle instance
*    dcs_mask              - 4 bits identifying DCS 0, 1, 2, 3 instance
*                            to test
*    target_mask           - 3 bit mask identifying ENET_EGRESS(0),
*                            ENET_INGRESS(1), and REFLECT(2) clarifying
*                            DCS2/3 target
*    source_mask           - Qualify DCS 0/1 data source CPU(0) or MGMT_FEGE(1)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS            - on success, otherwise descriptive error code
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_channel_mcpb_clean_filtered_test(digi_handle_t               *digi_handle,
                                                            util_global_switch_data_t   *chnl_handle,
                                                            UINT32 dcs_mask,
                                                            UINT32 target_mask,
                                                            BOOL8  source_mask)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_ss_handle_ptr;
    UINT32 link;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    
    rc = digi_enet_validate_parms(digi_handle,
                                  chnl_handle,
                                  &enet_ss_handle_ptr,
                                  &link);

    /* determine MCPB mux initialization state and if initialized the ENET 
       subsystem being selected */
    if(rc == PMC_SUCCESS)
    {
        if(digi_handle->var.mcpb_ctxt.mcpb_mux_mode_init == TRUE)
        {
            switch(cpb_mcpb_mux_mode_init_get(digi_handle->mcpb_handle))
            {
                case CPB_MCPB_MUX_MODE_ENET_LINE:
                    if(enet_ss_handle_ptr == digi_handle->enet_line_handle)
                    {
                        if(FALSE == cpb_mcpb_enet_channel_start_state_filtered_test(digi_handle->mcpb_handle, link, dcs_mask, target_mask))
                        {
                            rc = DIGI_ERR_MCPB_PATH_STILL_CONNECTED;
                        }
                    }
                    break;
                case CPB_MCPB_MUX_MODE_ENET_SYS:
                    if(enet_ss_handle_ptr == digi_handle->enet_sys_handle)
                    {
                        if(FALSE == cpb_mcpb_enet_channel_start_state_filtered_test(digi_handle->mcpb_handle, link, dcs_mask, target_mask))
                        {
                            rc = DIGI_ERR_MCPB_PATH_STILL_CONNECTED;
                        }
                    }
                    break;
                default:
                    PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
                    break;
            }    
        } 
    }

    if(rc == DIGI_ERR_MCPB_PATH_STILL_CONNECTED)
    {
        /* when specifying only one DCS mask for DCS 0 or 1, futher test to determine if the link is
           being provisioned for the resource described with source_mask */
        if(dcs_mask == 0x1)
        {
            if((source_mask & 0x1) && 
               ((digi_handle->var.mcpb_ctxt.mcpb_dcs_num_registered_chnl[DIGI_MCPB_DPI_0_1_CPU_CHNLS] & (1 << link)) == FALSE))
            {
                rc = PMC_SUCCESS;
            } 

            if((source_mask & 0x2) &&
               ((digi_handle->var.mcpb_ctxt.mcpb_dcs_num_registered_chnl[DIGI_MCPB_DPI_0_1_MGMT_CHNLS] & (1 << link)) == FALSE))
            {
                rc = PMC_SUCCESS;                                    
            }
        }
        if(dcs_mask == 0x2)
        {
            if((source_mask & 0x1) &&
               ((digi_handle->var.mcpb_ctxt.mcpb_dcs_num_registered_chnl[DIGI_MCPB_DPI_2_3_CPU_CHNLS] & (1 << link)) == FALSE))
            {
                rc = PMC_SUCCESS;                                    
            } 

            if((source_mask & 0x2) &&
               ((digi_handle->var.mcpb_ctxt.mcpb_dcs_num_registered_chnl[DIGI_MCPB_DPI_2_3_MGMT_CHNLS] & (1 << link)) == FALSE))
            {
                rc = PMC_SUCCESS;                                    
            }
        }
    }


    if(rc == DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET)
        rc = PMC_SUCCESS;

    PMC_RETURN(rc);
} /* digi_enet_channel_mcpb_clean_filtered_test */

/*******************************************************************************
* digi_enet_is_link_valid
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the link value as well as the path type
*             associated with the link.
*
* INPUTS:
*   *enet_handle           - pointer to ENET (LINE or SYS) handle instance
*   link                   - link number
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - link is not valid
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_is_link_valid(enet_handle_t *enet_handle,
                                      UINT32 link)
{
    PMC_ERROR rc = FALSE;
    PMC_ENTRY();

    if ((link >= 0) && (link < ENET_NUM_LINK_MAX)) {
        switch (enet_handle->var.ch_datapath[link]) {
        case ENET_SERDES_100GE_MAC:
        case ENET_SERDES_100GE_TRANSPARENT:
        case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
        case ENET_SLAVE_100GE_MAC_TX_PMON:
        case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
        case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
        case ENET_MAPOTN_GMP_G709_100GE_MAC:
        case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
            if (link != 0) {
                rc = FALSE;
            } else {
                rc = TRUE;
            }
            break;

        case ENET_SERDES_40GE_MAC:
        case ENET_SERDES_40GE_TRANSPARENT:
        case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
        case ENET_SLAVE_40GE_MAC_TX_PMON:
        case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
        case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
        case ENET_MAPOTN_GMP_G709_40GE_MAC:
        case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
            if ((link != 0) && (link != 4) && (link != 8)) {
                rc = FALSE;
            } else {
                rc = TRUE;
            }
            break;

        case ENET_SERDES_10GE_MAC:
        case ENET_SERDES_10GE_TRANSPARENT:
        case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
        case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
        case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
        case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
        case ENET_SLAVE_10GE_MAC_TX_PMON:
        case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
        case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
        case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
        case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
        case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
            rc = TRUE;
            break;

        default:
            rc = FALSE;
            break;
        }
    
    } else {
        rc = FALSE;
    }

    PMC_RETURN(rc);
} /* digi_enet_is_link_valid */

/*******************************************************************************
* digi_enet_sys_db_test_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API test/update the scheduler address for all DB (ENET SYS, CPB) to
*   ensure no allocation conflict and updates the relevant database set with the
*   appropriate ENET link data.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   schd_addr         - calendar offset to be read and set for link data.
*                       Valid values 0 to 95.
*   enet_link         - ENET SYS link to be configured
*                       Valid values 0 to 11.
*   enet_sys_dcs_inst - ENET SYS DCS instance 
*
*
* OUTPUTS:
*
*
* RETURNS:
*   BOOL8              - Success indication of updating the potential scheduler
*                       offset:\n
*                       TRUE:  Scheduler entry available and updated with
*                              provided channel data\n
*                       FALSE: Scheduler entry already occupied. No
*                              modification to db entry.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_sys_db_test_update(digi_handle_t *digi_handle,
                                           UINT32 schd_addr,
                                           UINT32 enet_link, 
                                           UINT32 enet_sys_dcs_inst)
{
    UINT32 output_port_ptr;
    UINT32 output_chnl_ptr;
    UINT32 rd_channel_id_ptr;
    UINT32 link;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* test all db for this entry being available */
    result = enet_db_entry_get(digi_handle->enet_sys_handle, schd_addr, &link);
    if(DIGI_SCHD_ENTRY_UNASSGINED != link)
    {
        PMC_RETURN(FALSE);
    }

    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_get(digi_handle->dcpb_handle,
                                      enet_sys_dcs_inst, schd_addr,
                                      &output_port_ptr,
                                      &output_chnl_ptr,
                                      &rd_channel_id_ptr);
        if(DIGI_SCHD_ENTRY_UNASSGINED != rd_channel_id_ptr )
        {
            PMC_RETURN(FALSE);
        }
    }

    /* write all database with appropriate values */
    if(PMC_SUCCESS == result)
    {
        /* write to ENET DB */
        result = enet_db_entry_set(digi_handle->enet_sys_handle, schd_addr, enet_link);
    }

    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_set(digi_handle->dcpb_handle,
                                      enet_sys_dcs_inst,
                                      schd_addr,
                                      UTIL_GLOBAL_CPB_PORT_ENET_SYS,
                                      enet_link);
    }

    if(PMC_SUCCESS != result)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(TRUE);

} /* digi_enet_sys_db_test_update */

/*******************************************************************************
* digi_enet_sys_db_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API clears database entries used by an ENET SYS link in the relevant
*   databases.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   enet_link         - ENET SYS link
*                       Valid values 0 to 11.
*   enet_sys_dcs_inst - ENET SYS DCS instance 
*
*
* OUTPUTS:
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_sys_db_clear(digi_handle_t *digi_handle,
                                         UINT32 enet_link, 
                                         UINT32 enet_sys_dcs_inst)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = enet_db_entry_clear(digi_handle->enet_sys_handle, enet_link);

    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_clear(digi_handle->dcpb_handle,
                                        enet_sys_dcs_inst,
                                        UTIL_GLOBAL_CPB_PORT_ENET_SYS,
                                        enet_link);
    }

    PMC_RETURN(result);

} /* digi_enet_sys_db_clear */


/*******************************************************************************
* digi_enet_line_db_test_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API test/update the scheduler address for all DB (ENET Line, CPB) to
*   ensure no allocation conflict and updates the relevant database set with the
*   appropriate ENET link data.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   schd_addr         - calendar offset to be read and set for link data.
*                       Valid values 0 to 95.
*   enet_link         - ENET line link to be configured
*                       Valid values 0 to 11.
*   mirror_mode       - The enet link is a mirror port.
*
* OUTPUTS:
*
*
* RETURNS:
*   BOOL8              - Success indication of updating the potential scheduler
*                       offset:\n
*                       TRUE:  Scheduler entry available and updated with
*                              provided channel data\n
*                       FALSE: Scheduler entry already occupied. No
*                              modification to db entry.
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_line_db_test_update(digi_handle_t *digi_handle,
                                            UINT32 schd_addr,
                                            UINT32 enet_link,
                                            BOOL mirror_mode)
{
    UINT32 output_port_ptr;
    UINT32 output_chnl_ptr;
    UINT32 rd_channel_id_ptr;
    UINT32 link, dcs_inst;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* test all db for this entry being available */
    result = enet_db_entry_get(digi_handle->enet_line_handle, schd_addr, &link);
    if(DIGI_SCHD_ENTRY_UNASSGINED != link)
    {
        PMC_RETURN(FALSE);
    }

    /* Mirror port will use DCS INST 3 (normally used for mapotn) instead of 2 (enet line) */
    dcs_inst = (mirror_mode ? DIGI_SCHD_MAPPER_DCS_INSTANCE : DIGI_SCHD_ENET_LINE_DCS_INSTANCE);

    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_get(digi_handle->dcpb_handle,
                                      dcs_inst,
                                      schd_addr,
                                      &output_port_ptr,
                                      &output_chnl_ptr,
                                      &rd_channel_id_ptr);
        if(DIGI_SCHD_ENTRY_UNASSGINED != rd_channel_id_ptr )
        {
            PMC_RETURN(FALSE);
        }
    }

    /* write all database with appropriate values */
    if(PMC_SUCCESS == result)
    {
        /* write to ENET DB */
        result = enet_db_entry_set(digi_handle->enet_line_handle, schd_addr, enet_link);
    }

    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_set(digi_handle->dcpb_handle,
                                      dcs_inst,
                                      schd_addr,
                                      UTIL_GLOBAL_CPB_PORT_ENET_LINE,
                                      enet_link);
    }

    if(PMC_SUCCESS != result)
    {
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(TRUE);

} /* digi_enet_line_db_test_update */

/*******************************************************************************
* digi_enet_line_db_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API clears database entries used by an ENET line link in the relevant
*   databases.
*
*
* INPUTS:
*   *digi_handle      - pointer to DIGI handle instance.
*   enet_link         - ENET line link
*                       Valid values 0 to 11.
*   mirror_mode       - The enet link is a mirror port.
*
*
* OUTPUTS:
*
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_line_db_clear(digi_handle_t *digi_handle,
                                          UINT32 enet_link,
                                          BOOL mirror_mode)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 dcs_inst;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = enet_db_entry_clear(digi_handle->enet_line_handle, enet_link);

    dcs_inst = (mirror_mode ? DIGI_SCHD_MAPPER_DCS_INSTANCE : DIGI_SCHD_ENET_LINE_DCS_INSTANCE);

    if(PMC_SUCCESS == result)
    {
        result = cpb_dcs_db_entry_clear(digi_handle->dcpb_handle,
                                        dcs_inst,
                                        UTIL_GLOBAL_CPB_PORT_ENET_LINE,
                                        enet_link);
    }

    PMC_RETURN(result);

} /* digi_enet_line_db_clear */

/*******************************************************************************
* digi_enet_is_link_mac_block
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the link path type is in the MAC block.
*
* INPUTS:
*   *enet_handle           - pointer to ENET (LINE or SYS) handle instance
*   link                   - link number
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC BOOL8 digi_enet_is_link_mac_block(enet_handle_t *enet_handle,
                                         UINT32 link)
{
    BOOL8 rc = FALSE;
    PMC_ENTRY();

    switch (enet_handle->var.ch_datapath[link]) {
    case ENET_SERDES_100GE_MAC:
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:

    case ENET_SERDES_40GE_MAC:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:

    case ENET_SERDES_10GE_MAC:
    case ENET_SLAVE_10GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
        rc = TRUE;
        break;

    default:
        rc = FALSE;
        break;
    }

    PMC_RETURN(rc);
} /* digi_enet_is_link_mac_block */

/*******************************************************************************
* digi_enet_is_link_transparent
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the link path type is in transparent mode
*
* INPUTS:
*   *enet_handle           - pointer to ENET (LINE or SYS) handle instance
*   link                   - link number
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not transparaent
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_is_link_transparent(enet_handle_t *enet_handle,
                                            UINT32 link)
{
    BOOL8 rc = FALSE;
    PMC_ENTRY();

    switch (enet_handle->var.ch_datapath[link]) {

    case ENET_SERDES_100GE_TRANSPARENT:
    case ENET_SERDES_40GE_TRANSPARENT:
    case ENET_SERDES_10GE_TRANSPARENT:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
    case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
    case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:        
    case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_TRANSPARENT_TX_PMON:
    case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
        rc = TRUE;
        break;

    default:
        rc = FALSE;
        break;
    }

    PMC_RETURN(rc);
} /* digi_enet_is_link_transparent */

/*******************************************************************************
* digi_enet_mac_data_rate_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the data rate for the path type in the MAC block.
*
* INPUTS:
*   *enet_handle           - pointer to ENET (LINE or SYS) handle instance
*   link                   - link number
*
* OUTPUTS:
*   *data_rate_ptr         - a pointer to the data rate
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_mac_data_rate_get(enet_handle_t *enet_handle,
                                              UINT32 link,
                                              emac_data_rate_t *data_rate_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ENTRY();

    switch (enet_handle->var.ch_datapath[link]) {
    case ENET_SERDES_100GE_MAC:
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:
        *data_rate_ptr = EMAC_LINK_100G;
        break;

    case ENET_SERDES_40GE_MAC:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:
        *data_rate_ptr = EMAC_LINK_40G;
        break;

    case ENET_SERDES_10GE_MAC:
    case ENET_SLAVE_10GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        *data_rate_ptr = EMAC_LINK_10G;
        break;

    default:
        rc = DIGI_ERR_INVALID_ARG;
        break;
    }

    PMC_RETURN(rc);
} /* digi_enet_mac_data_rate_get */

/*******************************************************************************
* digi_enet_is_multi_lane_serdes_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns TRUE when enet channel is associated to 
*   bundle of serdes, FALSE otherwise.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *enet_chnl_ptr   - pointer to ENET (LINE or SYS) handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE when enet channel is associated to 
*   bundle of serdes, FALSE otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC BOOL8 digi_enet_is_multi_lane_serdes_get(digi_handle_t               *digi_handle,
                                                digi_enet_client_chnl_def_t *enet_chnl_ptr)
{
    BOOL8 is_multi_lane = FALSE;
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    emac_data_rate_t data_rate;

    PMC_ENTRY();

    result = digi_enet_validate_parms(digi_handle, enet_chnl_ptr,
                                      &enet_handle, &link); 
    if (PMC_SUCCESS == result)
    {
        result = digi_enet_mac_data_rate_get(enet_handle, link, &data_rate);
        if (PMC_SUCCESS == result)
        {
            if (data_rate == EMAC_LINK_40G || data_rate == EMAC_LINK_100G)
            {
                is_multi_lane = TRUE;
            }            
        }
    }
    
    PMC_RETURN(is_multi_lane);
} /* digi_enet_is_multi_lane_serdes_get */

/*******************************************************************************
* digi_enet_line_prov_lane_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_line_prov_params_validate
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   port_uid               - The UID corresponding to the port.
*   mode                   - Enum, ENET modes when connected to Line
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_line_prov_lane_validate(digi_handle_t *digi_handle,
                                                    UINT32 port_uid,
                                                    digi_enet_mode_t mode)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 pin_count = 0;
    BOOL8 pin_valid = FALSE;

    PMC_ENTRY();

    pin_count = digi_num_xfi_pins_in_use(digi_handle, port_uid);

    if (pin_count == DIGI_SERDES_LINE_LANES_NONE) {

        /* This could also be mapped from the back */
        pin_count = digi_num_sys_pins_in_use(digi_handle, port_uid);

        if (pin_count == DIGI_SERDES_LINE_LANES_NONE) {

            rc =  DIGI_ERR_SERDES_XFI_NO_PIN_PROV;

        } else {

            pin_valid = TRUE;
        }

    } else {

        pin_valid = TRUE;
    }

    if (pin_valid  == TRUE) {

        if (pin_count == DIGI_SERDES_LINE_LANES_FOR_10G) {

            if ((mode == DIGI_ENET_SERDES_10GE_MAC) ||
                (mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT) ||
                (mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON) ||
                (mode == DIGI_ENET_SERDES_10GE_TRANSPARENT) ||
                (mode == DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON)) {

                rc = PMC_SUCCESS;

            } else {
                rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
            }

        } else if (pin_count == DIGI_SERDES_LINE_LANES_FOR_40G) {

            if ((mode == DIGI_ENET_SERDES_40GE_MAC) ||
                (mode == DIGI_ENET_SERDES_40GE_TRANSPARENT) ||
                (mode == DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON)) {

                rc = PMC_SUCCESS;

            } else {
                rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
            }

        } else if (pin_count == DIGI_SERDES_LINE_LANES_FOR_40G_SFIS) {

            rc = DIGI_ERR_PORT_PROV_RATE_FAIL;

        } else if (pin_count == DIGI_SERDES_LINE_LANES_FOR_100G) {

            if ((mode == DIGI_ENET_SERDES_100GE_MAC) ||
                (mode == DIGI_ENET_SERDES_100GE_TRANSPARENT) ||
                (mode == DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON) ) {

                rc = PMC_SUCCESS;

            } else {
                rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
            }

        } else {

            rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_line_prov_lane_validate */

/*******************************************************************************
* digi_enet_sys_prov_lane_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function performs simple parameter validation for the call to
*   digi_enet_sys_prov_params_validate
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   port_uid               - The UID corresponding to the port.
*   mode                   - Enum, ENET modes when connected to Line
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_sys_prov_lane_validate(digi_handle_t *digi_handle,
                                                   UINT32 port_uid,
                                                   digi_enet_mode_t mode)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    UINT8 pin_count = 0;

    PMC_ENTRY();

    pin_count = digi_num_sys_pins_in_use(digi_handle, port_uid);

    if (pin_count == DIGI_SERDES_SYS_LANES_NONE) {

        rc =  DIGI_ERR_SERDES_SYS_NO_PIN_PROV;

    } else if (pin_count == DIGI_SERDES_SYS_LANES_FOR_10G) {

        if ((mode == DIGI_ENET_SERDES_10GE_MAC) ||
            (mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT) ||
            (mode == DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON) ||
            (mode == DIGI_ENET_SERDES_10GE_TRANSPARENT) ||
            (mode == DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON)) {

            rc = PMC_SUCCESS;

        } else {
            rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
        }

    } else if (pin_count == DIGI_SERDES_SYS_LANES_FOR_40G) {

        if ((mode == DIGI_ENET_SERDES_40GE_MAC) ||
            (mode == DIGI_ENET_SERDES_40GE_TRANSPARENT) ||
            (mode == DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON)) {
            rc = PMC_SUCCESS;

        } else {
            rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
        }

    } else if (pin_count == DIGI_SERDES_SYS_LANES_FOR_40G_SFIS) {

        rc = DIGI_ERR_PORT_PROV_RATE_FAIL;

    } else if (pin_count == DIGI_SERDES_SYS_LANES_FOR_100G) {

        if ((mode == DIGI_ENET_SERDES_100GE_MAC) ||
            (mode == DIGI_ENET_SERDES_100GE_TRANSPARENT)||
            (mode == DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON)) {

            rc = PMC_SUCCESS;

        } else {
            rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
        }

    } else {

        rc = DIGI_ERR_PORT_PROV_RATE_FAIL;
    }

    PMC_RETURN(rc);
} /* digi_enet_sys_prov_lane_validate */

/*******************************************************************************
* digi_enet_enhanced_pmon_map
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON is required, if so, 
*   configure it if the resource is available. 
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel to examine.
*   *output_pc_ptr   - the pointer to the output channel to examine.
*   rate_data        - enum, client stream types of signal passing through CPB.
*                      For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, client_num_odu0 
*                      must be provided
*   client_num_odu0  - For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, number of ODU0 
*                      container associated with ODUFLEX signal.
*   halt_buffer      - CPB FIFO buffer room to capture in flight data in
*                      the event that CPB FIFO generates an upstream flow
*                      control signal.  Used in cases where ENET utilizes
*                      pause flow control and where a channel is fed from
*                      SIFD and asserts XOFF flow control.  halt_buffer is
*                      specified in units of blocks.  The block size in bytes 
*                      per unit is: 12288 bytes.
*                      Applicable to client_data containing:
*                      "DIGI_CPB_STREAM_PKT_"
*   pmon_req_type    - The type of PMON that is required. ENET LINE or SYS
*   *mapper_chnl_ptr - The mapper channel pointer if the input_chnl is
*                      via MAPOTN
*   *ilkn_chnl_ptr   - The ilkn channel pointer if the input_chnl is
*                      via ILKN1/ILKN2
*   *enet_chnl_ptr   - The output handle to the ENET LINE or SYS resource.
*   enet_mode        - The PMON mode that ENET LINE or SYS resource should use.
*   digi_enet_mode   - Equivalent DIGI ENET mode enumerated type
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the setup was
*   required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_map(digi_handle_t* digi_handle,
                                             util_global_switch_data_t* input_pc_ptr, 
                                             util_global_switch_data_t* output_pc_ptr,
                                             digi_cpb_stream_t rate_data,
                                             UINT32 client_num_odu0,
                                             UINT32 halt_buffer,
                                             digi_enet_enhanced_pmon_req_t pmon_req_type,
                                             digi_mapper_chnl_def_t* mapper_chnl_ptr,
                                             digi_ilkn_chnl_def_t* ilkn_chnl_ptr,
                                             digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                             enet_datapath_t enet_mode, 
                                             digi_enet_mode_t digi_enet_mode)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_enet_client_chnl_def_t* enet_chnl_mate_ptr = NULL;

    PMC_ENTRY();

    /* Initialize ENET1/2 DCSU if ENET is in start state */
    digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_ENET1);
    digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_ENET2);
    digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_ENET3);

    if (digi_enet_enhanced_pmon_sys_or_line_map_helper(digi_handle,
                                                       input_pc_ptr, 
                                                       rate_data,
                                                       client_num_odu0,
                                                       halt_buffer,
                                                       pmon_req_type,
                                                       enet_chnl_ptr,
                                                       enet_mode, 
                                                       digi_enet_mode,
                                                       &enet_chnl_mate_ptr) == PMC_SUCCESS) {

        /* Add this information to the ENET_LINE output_ptr? */
        if (pmon_req_type == DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_MAP) {

            if (mapper_chnl_ptr->enet_pmon_data != NULL) {

                /* Assert Bad state*/
                result = DIGI_ERR_CODE_ASSERT;
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

            } else {

                mapper_chnl_ptr->enet_pmon_data = enet_chnl_mate_ptr;
                /*
                 * Register the enet_pmon_data pointer in the context now 
                 * that it will be used to ensure it works correctly after
                 * a restart. Limits the number of actual enet pmon ptrs
                 * registered in the context.
                 */
                PMC_CTXT_REGISTER_PTR(&mapper_chnl_ptr->enet_pmon_data, 
                                      digi_handle);
                result = PMC_SUCCESS;
            }

        } else if ((pmon_req_type == DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_ILKN) ||
                   (pmon_req_type == DIGI_ENHANCED_ENET_LINE_PMON_REQ_AND_REALIZABLE_ILKN)) { 

            if (ilkn_chnl_ptr->enet_pmon_data != NULL) {

                /* Assert Bad state*/
                result = DIGI_ERR_CODE_ASSERT;
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

            } else {
                /*
                 * Register the enet_pmon_data pointer in the registry. 
                 * Register them on as used based to limit the number of
                 * pointers registered in the context registry and ensuring
                 * the pointer will be available after a restart.
                 */
                ilkn_chnl_ptr->enet_pmon_data = enet_chnl_mate_ptr;
                PMC_CTXT_REGISTER_PTR(&ilkn_chnl_ptr->enet_pmon_data, 
                                      digi_handle);
                result = PMC_SUCCESS;
            }

        } else {

            /* Assert Unreachable */
            result = DIGI_ERR_CODE_ASSERT;
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
        }
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_map */

/*******************************************************************************
* digi_enet_enhanced_pmon_demap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON is required, if so, 
*   deconfigure it if the resource is available. 
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel to examine.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the deconstruction 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   The only time this function should fail if the actual datapath provisioning
*   fails. In all other cases, we just log to the user that PMON could not be
*   unrealized.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_demap(digi_handle_t* digi_handle,
                                               util_global_switch_data_t* input_pc_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    digi_mapper_chnl_def_t* mapper_chnl = NULL;
    digi_ilkn_chnl_def_t* ilkn_chnl = NULL;
    util_global_switch_data_def_t* in_util_ptr = NULL;
    util_global_switch_data_t* output_pc_ptr = NULL;

    digi_enet_pmon_type_t enet_pmon = DIGI_NO_PMON;
                
    PMC_ENTRY();

    in_util_ptr = ((util_global_switch_data_def_t*)input_pc_ptr);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(in_util_ptr);
    
    if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

        /* We now now the type so cast appropriately */
        mapper_chnl = (digi_mapper_chnl_def_t*)input_pc_ptr;
        enet_pmon = mapper_chnl->enet_pmon;
    } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
               (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

        /* We now now the type so cast appropriately */
        ilkn_chnl = (digi_ilkn_chnl_def_t*)input_pc_ptr; 
        enet_pmon =  ilkn_chnl->enet_pmon;
    }

    if (enet_pmon == DIGI_NO_PMON) {

        /* No PMON requested. Nothing to do. */
        result = PMC_SUCCESS; 

    } else if (mapper_chnl != NULL) {
            
        if (digi_enet_is_enhanced_pmon_umbilical_case(digi_handle, mapper_chnl, enet_pmon) == TRUE) {

            /* This has been provisiond as part of the mapper case.
             * It will be deprovisioned there. */
            result = PMC_SUCCESS; 

        } else { 
            
            result = digi_enet_verify_enet_pmon_and_demap(digi_handle,
                                                          input_pc_ptr,
                                                          output_pc_ptr);

        }

    } else if (ilkn_chnl != NULL) {

        result = digi_enet_verify_enet_pmon_and_demap(digi_handle,
                                                      input_pc_ptr,
                                                      output_pc_ptr);

    } else {

        /* One of either maper_chnl or ilkn_chnl should be non NULL */

        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    
    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_demap */

/*******************************************************************************
* digi_enet_enhanced_pmon_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to activate ENET PMON if it is required, if so, 
*   activate it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr   - the pointer to the output channel.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the activation 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   The only time this function should fail if the actual datapath activation 
*   fails.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_activate(digi_handle_t* digi_handle,
                                                  util_global_switch_data_t* input_pc_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    util_global_switch_data_def_t* in_util_ptr = NULL;
    digi_prod_app_t prod_app;
    digi_enet_pmon_type_t enet_pmon_type = DIGI_NO_PMON;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr = NULL;
    digi_ilkn_chnl_def_t *ilkn_chnl_handle_ptr = NULL;

    PMC_ENTRY();

    in_util_ptr = (util_global_switch_data_def_t*)input_pc_ptr;
    prod_app = DIGI_PROD_APP_GET(digi_handle);

    switch(in_util_ptr->cpb_data.port_type)
    {
        case UTIL_GLOBAL_CPB_PORT_MAPOTN:
            mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(input_pc_ptr);
            enet_pmon_type = mapper_chnl_handle_ptr->enet_pmon;
            break;
        case UTIL_GLOBAL_CPB_PORT_ILKN1:
        case UTIL_GLOBAL_CPB_PORT_ILKN2:
            ilkn_chnl_handle_ptr = (digi_ilkn_chnl_def_t*)(in_util_ptr);
            enet_pmon_type = ilkn_chnl_handle_ptr->enet_pmon;
            break;
        default:
            enet_pmon_type = DIGI_NO_PMON;
            break;
    } 


    if (enet_pmon_type == DIGI_ENET_SYS_PMON) 
    { 
        /* Perform ENET SYS PMON Companion Activation for MAPOTN or ILKN */
        result = digi_enet_enhanced_pmon_sys_comp_activate(digi_handle, in_util_ptr);
    } 
    else if (enet_pmon_type == DIGI_ENET_LINE_PMON)
    {
        if  (ilkn_chnl_handle_ptr != NULL && prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) 
        {
            /* Perform ENET LINE PMON Companion Activation for ILKN */
            result = digi_enet_enhanced_pmon_line_comp_activate(digi_handle, in_util_ptr);
        }
        else 
        {
            /* The only case left is to perform ENET LINE PMON Local Activation for MAPOTN */
            if (NULL != mapper_chnl_handle_ptr && digi_mapper_is_mapotn_mode_gfp_enet(mapper_chnl_handle_ptr))
            {
                result = digi_enet_enhanced_pmon_fcs_settings_set(digi_handle, in_util_ptr);
            }
        }
    } 
    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_activate */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_comp_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to activate ENET SYS PMON companion if it is 
*   required, if so, activate it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *in_util_ptr     - the pointer to the input channel.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the activation 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_comp_activate(digi_handle_t* digi_handle,
                                                            util_global_switch_data_def_t* in_util_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_enet_client_chnl_def_t* enet_data_ptr = NULL;

    PMC_ENTRY();

    /* If we are one of these types set the pointer */
    enet_data_ptr = digi_enet_enhanced_pmon_ptr_get(in_util_ptr);
    PMC_ASSERT(enet_data_ptr != NULL, DIGI_ERR_CODE_ASSERT, 0, 0);

    /* Only attempt activation of ENET SYS PMON if we have some data here */
    result = digi_cpb_internal_chnl_activate(digi_handle, (util_global_switch_data_def_t*)enet_data_ptr);

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_sys_comp_activate */

/*******************************************************************************
* digi_enet_enhanced_pmon_line_comp_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to activate ENET LINE PMON companion if it is 
*   required, if so, activate it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *in_util_ptr     - the pointer to the input channel.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the activation 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_line_comp_activate(digi_handle_t* digi_handle,
                                                             util_global_switch_data_def_t* in_util_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_enet_client_chnl_def_t* enet_data_ptr = NULL;

    PMC_ENTRY();

    /* If we are one of these types set the pointer */
    enet_data_ptr = digi_enet_enhanced_pmon_ptr_get(in_util_ptr);

    PMC_ASSERT(enet_data_ptr != NULL, DIGI_ERR_CODE_ASSERT, 0, 0);

    result = digi_cpb_internal_chnl_activate(digi_handle,
                                             (util_global_switch_data_def_t*)enet_data_ptr);
    
    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_line_comp_activate */

/*******************************************************************************
* digi_enet_enhanced_pmon_line_local_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to activate ENET LINE PMON LOCAL if it is 
*   required, if so, activate it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *in_util_ptr     - the pointer to the input channel.
*   pointer.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the activation 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_line_local_activate(digi_handle_t* digi_handle,
                                                             util_global_switch_data_def_t* in_util_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_enet_client_chnl_def_t* enet_data_ptr = NULL;
    UINT32 channel;
    PMC_ENTRY();
    
    /* If we are one of these types set the pointer */
    enet_data_ptr = digi_enet_enhanced_pmon_ptr_get(in_util_ptr);

    /* Only attempt activation of ENET LINE PMON if we have some data here */
    PMC_ASSERT(enet_data_ptr != NULL, DIGI_ERR_CODE_ASSERT, 0, 0);

    /* Note, in the local mode, we have local timing, just need to
     * activate but we don't want to do this through the CPB */
    channel = ((util_global_switch_data_def_t*)(enet_data_ptr))->cpb_data.channel;
                                                                        
    result = enet_rx_activate(digi_handle->enet_line_handle,
                              channel,
                              digi_handle->enet_line_handle->var.ch_datapath[channel]);
    if (PMC_SUCCESS == result)
    {
        result = enet_tx_activate(digi_handle->enet_line_handle,
                                  channel,
                                  digi_handle->enet_line_handle->var.ch_datapath[channel]);
    }
    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_line_local_activate */



/*******************************************************************************
* digi_lifd_convert_and_enable_clock_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to obtain how many lanes the ENET requires to configure
*   in the LIFD DSIS and then configures them.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*    enet_link       - ENET link 
*    enet_datapath  - ENET mode 
*    enet_clk_enable  - enable transmit clock from mate DSIS (TRUE) or
*                      from internal lane (FALSE)
*
* OUTPUTS:
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_lifd_convert_and_enable_clock_select(digi_handle_t* digi_handle,
                                                            UINT32 enet_link,
                                                            enet_datapath_t enet_datapath,
                                                            BOOL8 enet_clk_enable)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT8 i = 0; /* counter */
    UINT8 num_lanes = 0; 
    BOOL8 lifd_error = FALSE;

    PMC_ENTRY();

    /* Obtain the number of lanes and the channel */
    num_lanes =  digi_convert_enet_mode_to_lanes(enet_datapath);

    for (i = enet_link; i < enet_link + num_lanes; i++) {
        /* Ensure the clock source is selected from the partner DSIS */
        if (lifd_enet_clock_select(digi_handle->lifd_handle, 
                                   i,
                                   enet_clk_enable) != PMC_SUCCESS) {

            lifd_error = TRUE;
        }
    }

    if (lifd_error == FALSE) {

        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /* digi_lifd_convert_and_enable_clock_select */

/*******************************************************************************
* digi_sifd_convert_and_enable_clock_select
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to obtain how many lanes the ENET requires to configure
*   in the SIFD DSIS and then configures them.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   enet_link       - ENET link 
*   enet_datapath  - ENET mode 
*   enet_clk_enable  - enable transmit clock from mate DSIS (TRUE) or
*                      from internal lane (FALSE)
*
* OUTPUTS:
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_sifd_convert_and_enable_clock_select(digi_handle_t* digi_handle,
                                                            UINT32 enet_link,
                                                            enet_datapath_t enet_datapath,
                                                            BOOL8 enet_clk_enable)
{

    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT8 i = 0; /* counter */
    UINT8 num_lanes = 0; 
    BOOL8 sifd_error = FALSE;

    PMC_ENTRY();

    /* Obtain the number of lanes and the channel */
    num_lanes =  digi_convert_enet_mode_to_lanes(enet_datapath);

    for (i = enet_link; i < enet_link + num_lanes; i++) {

        /* Ensure the clock source is selected from the partner DSIS */
        if (sifd_enet_clock_select(digi_handle->sifd_handle, 
                                   i,
                                   enet_clk_enable) != PMC_SUCCESS) {
            
            sifd_error = TRUE;
        }
    }

    if (sifd_error == FALSE) {

        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /* digi_sifd_convert_and_enable_clock_select */

/*******************************************************************************
* digi_convert_enet_mode_to_lanes 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to obtain how many lanes the ENET requires
*   given the mode.
*
* INPUTS:
*   enet_mode  - the ENET mode.
*
* OUTPUTS:
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   The number of lanes that are required for this mode.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE UINT32 digi_convert_enet_mode_to_lanes(enet_datapath_t enet_mode)
{
    UINT8 num_lanes;
    digi_group_size_t group_size = LAST_DIGI_GRP_SIZE;

    PMC_ENTRY();
    digi_enet_pmon_prov_params_convert(enet_mode,&group_size);
    if (group_size == DIGI_GRP_SIZE_10G)
    {
        num_lanes = ENET_NUM_LINK_10GE;
    } else  if (group_size == DIGI_GRP_SIZE_40G)
    {
        num_lanes = ENET_NUM_LINK_40GE;
    } else 
    {
        num_lanes = ENET_NUM_LINK_100GE;
    }


    PMC_RETURN(num_lanes);
} /* digi_convert_enet_mode_to_lanes */

/*******************************************************************************
* digi_enet_enhanced_pmon_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to deactivate ENET PMON if it was required, if so, 
*   deactivate it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *output_pc_ptr   - the pointer to the output channel.
*
* OUTPUTS:
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON was required to be deactivated or if the 
*   deactivation was required and successful, specific error otherwise.
*
* NOTES:
*   The only time this function should fail if the actual datapath deactivation 
*   fails.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_deactivate(digi_handle_t* digi_handle,
                                                    util_global_switch_data_t* output_pc_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    util_global_switch_data_def_t* out_util_ptr = NULL;
    util_global_switch_data_t* input_pc_ptr = NULL;

    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    out_util_ptr = (util_global_switch_data_def_t*)output_pc_ptr;

    if (output_pc_ptr == NULL) {

        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }

    if (out_util_ptr->cpb_data.op_state.active_state != UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE) {

        PMC_RETURN(DIGI_ERR_CPB_NOT_ACTIVE);
    }

    /* We need to look up the input_chnl based on this output chnl. */
    input_pc_ptr = digi_cpb_input_port_get(digi_handle, output_pc_ptr);

    if (input_pc_ptr == NULL)  {

        /* We shouldn't have gotten this far as this indicates things were not mapped properly */
        result = DIGI_ERR_INVALID_ARG;
        
    } else {

        /* Deactivate PMON if requested */
        result = digi_enet_enhanced_pmon_conditional_deactivate(digi_handle, input_pc_ptr);
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_deactivate */

/*******************************************************************************
* digi_enet_enhanced_pmon_conditional_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to deactivate ENET PMON if it is required, if so, 
*   deactivate it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel.
*
* OUTPUTS:
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the deactivation 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   The only time this function should fail if the actual datapath deactivation 
*   fails.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_conditional_deactivate(digi_handle_t* digi_handle,
                                                                 util_global_switch_data_t* input_pc_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_enet_client_chnl_def_t* enet_data_ptr = NULL;
    util_global_switch_data_def_t* in_util_ptr = NULL;
    digi_enet_pmon_type_t enet_pmon_type = DIGI_NO_PMON;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr = NULL;
    digi_ilkn_chnl_def_t *ilkn_chnl_handle_ptr = NULL;
    digi_prod_app_t prod_app;

    PMC_ENTRY();

    in_util_ptr = (util_global_switch_data_def_t*)input_pc_ptr;
    prod_app = DIGI_PROD_APP_GET(digi_handle);

    if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

        mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(input_pc_ptr);
        enet_pmon_type = mapper_chnl_handle_ptr->enet_pmon;

    } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
               (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

        ilkn_chnl_handle_ptr = (digi_ilkn_chnl_def_t*)(in_util_ptr);
        enet_pmon_type = ilkn_chnl_handle_ptr->enet_pmon;
    }

    if (enet_pmon_type == DIGI_ENET_SYS_PMON) {

        /* If we are one of these types set the pointer */
        enet_data_ptr = digi_enet_enhanced_pmon_ptr_get(in_util_ptr);

        /* Only attempt deactivation of ENET SYS PMON if we have some data here */
        if (enet_data_ptr != NULL) {

            result = digi_cpb_internal_chnl_deactivate(digi_handle, 
                                                       (util_global_switch_data_def_t*)enet_data_ptr);
        } else {

            /* User never requested Enhanced PMON, nothing to do, 
             * so succeed */
            result = PMC_SUCCESS;
        }

    } else if ((prod_app == DIGI_PROD_APP_TRANSMUXPONDER_CARD) && 
               (enet_pmon_type == DIGI_ENET_LINE_PMON) &&
               (ilkn_chnl_handle_ptr != NULL)) {

        /* If we are one of these types set the pointer */
        enet_data_ptr = digi_enet_enhanced_pmon_ptr_get(in_util_ptr);

        /* Only attempt deactivation of ENET LINE PMON if we have some data here */
        if (enet_data_ptr != NULL) {            
            result = digi_cpb_internal_chnl_deactivate(digi_handle, 
                                                       (util_global_switch_data_def_t*)enet_data_ptr);
            
        } else {

            /* User never requested Enhanced PMON, nothing to do, 
             * so succeed */
            result = PMC_SUCCESS;
        }


    } else {

        /* We can't have Enhanced PMON on anything but MAPOTN / ILKN, 
         * so succeed */
        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_conditional_deactivate */

/*******************************************************************************
* digi_enet_is_enhanced_pmon_required
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON is required along
*   the datapath. This is used during both mapping, activation, and 
*   deactivation. It also sets up certain pointer values for later use by
*   the specific application. These may or may not be set depending on the
*   return value.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *input_pc_ptr          - the pointer to the input channel to examine.
*   *output_pc_ptr         - the pointer to the output channel to examine.
*   
* OUTPUTS:
*   **mapper_chnl_ptr      - The mapper channel pointer if the input_chnl is
*                            via MAPOTN
*   **ilkn_chnl_ptr        - The ilkn channel pointer if the input_chnl is
*                            via ILKN1/ILKN2
*   **enet_chnl_ptr        - The output handle to the ENET_LINE resource.
*   *enet_mode_ptr         - The PMON mode that ENET_SYS resource should use.
*   *digi_enet_mode_ptr    - Equivalent DIGI ENET mode enumerated type
*   *pmon_req_type         - The digi_enet_enhanced_pmon_req_t that has been
*                            requested based on the other parameters depending on 
*                            if PMON is realizable or not. If so via which
*                            subsystem the input stream is entering. If not,
*                            this is simply an unrealizable goal, or if PMON is
*                            simply not required.
*                            See digi_enet_enhanced_pmon_req_t for more 
*                            information.
*
* RETURNS:
*   PMC_SUCCESS on success, or specific error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_is_enhanced_pmon_required(digi_handle_t* digi_handle,
                                                     util_global_switch_data_t* input_pc_ptr, 
                                                     util_global_switch_data_t* output_pc_ptr,
                                                     digi_mapper_chnl_def_t** mapper_chnl_ptr,
                                                     digi_ilkn_chnl_def_t** ilkn_chnl_ptr,
                                                     digi_enet_client_chnl_def_t** enet_chnl_ptr,
                                                     enet_datapath_t* enet_mode_ptr, 
                                                     digi_enet_mode_t* digi_enet_mode_ptr,
                                                     digi_enet_enhanced_pmon_req_t* pmon_req_type)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    digi_mapper_chnl_def_t* mapper_chnl = NULL;
    digi_ilkn_chnl_def_t* ilkn_chnl = NULL;
    util_global_switch_data_def_t* in_util_ptr = NULL;
    util_global_switch_data_def_t* out_util_ptr = NULL;
    digi_enet_pmon_type_t enet_pmon = DIGI_NO_PMON;

    enet_datapath_t e_mode = LAST_ENET_DATAPATH;
    digi_group_size_t group_size = LAST_DIGI_GRP_SIZE;


    PMC_ENTRY();

    PMC_ASSERT((digi_handle != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT((input_pc_ptr != NULL), DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT((output_pc_ptr != NULL), DIGI_ERR_INVALID_ARG, 0, 0);

    in_util_ptr = ((util_global_switch_data_def_t*)input_pc_ptr);
    out_util_ptr = ((util_global_switch_data_def_t*)output_pc_ptr);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(in_util_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(out_util_ptr);

    /* Map these back to digi_XXX_chn_def_t. Extract mode, and enet_pmon. */
    
    if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

        /* We now know the type so cast appropriately */
        *mapper_chnl_ptr = (digi_mapper_chnl_def_t*)input_pc_ptr;
        mapper_chnl = (digi_mapper_chnl_def_t*)input_pc_ptr;
        enet_pmon = mapper_chnl->enet_pmon;

    } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
               (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

        /* We now know the type so cast appropriately */
        *ilkn_chnl_ptr = (digi_ilkn_chnl_def_t*)input_pc_ptr; 
        ilkn_chnl = (digi_ilkn_chnl_def_t*)input_pc_ptr; 
        enet_pmon = ilkn_chnl->enet_pmon;
    }

    if (enet_pmon == DIGI_NO_PMON) {

        /* No PMON requested */
        *pmon_req_type = DIGI_ENHANCED_PMON_NO_PMON_REQ;
        result = PMC_SUCCESS;

    } else if (((out_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) ||
                (out_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) ||
                (out_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
                (out_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) &&
               (enet_pmon == DIGI_ENET_LINE_PMON) && /* Requested ENET LINE PMON */
               (mapper_chnl != NULL)) {              /* endpoint is not PIF */

        if (digi_enet_is_enhanced_pmon_umbilical_case(digi_handle, mapper_chnl, enet_pmon) == TRUE) {

            /* We have already realized this on the LINE side
             * as the umbillical case (CASE 3) therefore 
             * there is nothing to do. */
            *pmon_req_type = DIGI_ENHANCED_PMON_NO_PMON_REQ;
            result = PMC_SUCCESS;

        } else {

            /* PMON is not realizable */
            *pmon_req_type = DIGI_ENHANCED_ENET_PMON_REQ_AND_NOT_REALIZABLE;
            result = DIGI_ERR_ENET_PMON_RESOURCE_UNAVAILABLE;
        }

        /* This is the case where we have requested ENET_SYS PMON for PIF
         * in a datapath like: OTN -> GFPDemap -> ETRANS -> DCPB -> PIF, or
         *                     OTN -> GMPDemap -> ETRANS -> DCPB -> PIF, but
         * this is not possible since we don't have the timing for this mode */
    } else if ((out_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) &&  /* ENET pos. ETRANS */
               (enet_pmon == DIGI_ENET_SYS_PMON) &&          /* Requested ENET SYS PMON */
               (ilkn_chnl != NULL) &&                        /* endpoint is PIF */
               (out_util_ptr->cpb_data.data_source == 1)) {  /* data source == ODUK SW therefor ETRANS */
    
        /* PMON is not realizable */
        *pmon_req_type = DIGI_ENHANCED_ENET_PMON_REQ_AND_NOT_REALIZABLE;
        result = DIGI_ERR_ENET_PMON_RESOURCE_UNAVAILABLE;
    
    } else {

        /* At this point we know we need PMON */
        *enet_chnl_ptr = (digi_enet_client_chnl_def_t*)output_pc_ptr; 

        /* If source is MAPOTN and (Adaptation function) and 
         * Card is in DIGI_PROD_APP_LINE_CARD or DIGI_PROD_APP_SYSOTN_CARD mjr mode.
         * **or* source is ILKN and (Adaptation function) and
         * Card is DIGI_PROD_APP_PORTLESS_CARD, DIGI_PROD_APP_LINE_CARD or
         * DIGI_PROD_APP_100G_3_X_40G_CFP_CARD 
         * then need ENET_SYS to do PMON on our behalf */

        /* source is either mapper or ilkn */
        if (mapper_chnl != NULL) {

            /* call mapper helper */
            result = digi_enet_sys_pmon_map_validate_and_mode_set(digi_handle,
                                                                  mapper_chnl->mode,
                                                                  mapper_chnl->odu_data,
                                                                  (*enet_chnl_ptr)->mode, /* digi_enet_mode_t */
                                                                  enet_mode_ptr, 
                                                                  digi_enet_mode_ptr);
     
            if (result == PMC_SUCCESS) {

                *pmon_req_type = DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_MAP;
            }

        } else if (ilkn_chnl != NULL) {

            /* call ilkn helper can be either ENET_SYS or ENET_LINE */
            result = digi_enet_pmon_ilkn_validate_and_mode_set(digi_handle, 
                                                               (*enet_chnl_ptr)->switch_data.cpb_data.channel,
                                                               enet_pmon,
                                                               enet_mode_ptr, 
                                                               digi_enet_mode_ptr);

            if (result == PMC_SUCCESS) {

                if (enet_pmon == DIGI_ENET_SYS_PMON) { 

                    *pmon_req_type = DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_ILKN;

                } else if (enet_pmon == DIGI_ENET_LINE_PMON) {

                    *pmon_req_type = DIGI_ENHANCED_ENET_LINE_PMON_REQ_AND_REALIZABLE_ILKN;

                } else {

                    PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
                }
            }
        }

        if (result == PMC_SUCCESS) {

            digi_enet_line_prov_params_convert(*digi_enet_mode_ptr, &e_mode, &group_size);

            result = digi_enet_determine_result_and_resource_availability(digi_handle, 
                                                                          *enet_chnl_ptr,
                                                                          group_size,
                                                                          *pmon_req_type);
        }
    }

    PMC_RETURN(result);
} /* digi_enet_is_enhanced_pmon_required */

/*******************************************************************************
* digi_enet_determine_result_and_resource_availability
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON resource is available
*   as well as the result code to be returned to the caller.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   *enet_chnl_ptr         - the pointer to the specific enet_client_handle.
*   group_size             - the number of post MLD DSI lanes required for this mode.
*   pmon_req_type          - the type of PMON requested and determines weather
*                            the ENET LINE or ENET SYS resource was requested.
*   
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS on success, or specific error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_determine_result_and_resource_availability(digi_handle_t* digi_handle, 
                                                                       digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                                       digi_group_size_t group_size,
                                                                       digi_enet_enhanced_pmon_req_t pmon_req_type) {

    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    PMC_ENTRY();

    if (((pmon_req_type == DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_MAP) ||
         (pmon_req_type == DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_ILKN)) && 
        (enet_chnl_ptr != NULL)) {

        /* PMON requested */
        result = digi_enet_is_link_in_use(digi_handle, 
                                          enet_chnl_ptr->switch_data.cpb_data.channel,
                                          group_size,
                                          UTIL_GLOBAL_CPB_PORT_ENET_SYS);

    } else if ((pmon_req_type == DIGI_ENHANCED_ENET_LINE_PMON_REQ_AND_REALIZABLE_ILKN) &&
               (enet_chnl_ptr != NULL)) {

        /* PMON requested */
        result = digi_enet_is_link_in_use(digi_handle, 
                                          enet_chnl_ptr->switch_data.cpb_data.channel,
                                          group_size,
                                          UTIL_GLOBAL_CPB_PORT_ENET_LINE);
    } 

    PMC_RETURN(result);
} /* digi_enet_determine_result_and_resource_availability */

/*******************************************************************************
* digi_enet_is_enhanced_pmon_umbilical_case
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON is in the umbilical
*   case. We need to know this in order to deprovision several things.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   mapper_chnl        - the digi level mapping mode we are attempting to 
*                        realize.
*                        See digi_mapping_mode_t for more information.
*   enet_pmon          - The specific PMON type. See digi_enet_pmon_type_t for
*                        more information.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE if this is the umbilical case 3, false otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PUBLIC BOOL8 digi_enet_is_enhanced_pmon_umbilical_case(digi_handle_t* digi_handle,
                                                       digi_mapper_chnl_def_t* mapper_chnl,
                                                       digi_enet_pmon_type_t enet_pmon)
{
    BOOL8 result = FALSE;
    PMC_ENTRY();

    if (mapper_chnl == NULL) {

        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);

    } else if (enet_pmon != DIGI_ENET_LINE_PMON) {
    } else if (mapper_chnl->mode == DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP) {

        result = TRUE;
    }  else if (DIGI_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) == TRUE || DIGI60_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) == TRUE 
               || DIGI60_PROD_APP_IS_TRANSMUXPONDER_SYS_CLIENT_CARD(digi_handle) == TRUE) {
    
        switch(mapper_chnl->mode) {

        case DIGI_MAP_ODU4P_100_GE_GFP_ENET:
        case DIGI_MAP_ODU3P_40_GE_GFP_ENET:
        case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET:
        case DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET:
        case DIGI_MAP_ODU3P_CBR_40_GE_GMP:
        case DIGI_MAP_ODU4P_CBR_100_GE_GMP:
        case DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP:
        case DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP:
            /* We have already realized this on the LINE side
             * as the umbillical case (CASE 3) */
            result = TRUE;
            break;
        default:
            result = FALSE;
            break;
        }
    }

    PMC_RETURN(result);

} /* digi_enet_is_enhanced_pmon_umbilical_case */

/*******************************************************************************
* digi_enet_pmon_is_available_and_case_determine
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if we can obtain ENET PMON and 
*   where it should be provisioned, either on ENET LINE or wait for a mapping
*   for the ENET SYS case.
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance.
*   mode               - the digi level mapping mode we are attempting to 
*                        realize.
*                        See digi_mapping_mode_t for more information.
*   enet_pmon          - TRUE: if the user requested PMON.
*                        FALSE: otherwise.
*   mapotn_dest        - an enum specifying the MAPOTN destination.
*                        See mapotn_src_dest_t for more information.
*                        See digi_enet_pmon_type_t for more information.
*
* OUTPUTS:
*   *enet_line_as_pmon  - Set the enet line db up if enet line is to only be used
*                         as pmon.
*   *enet_pmon_type_ptr - a pointer to an enum specifying the ENET PMON type 
*                         that we are attempting to provision. 
*                         See digi_enet_pmon_type_t for more information.
*   *cpb_port_type_ptr  - a pointer to the CPB output port. 
*                         See enum type util_global_cpb_port_t for more
*                         information.
*
* RETURNS:
*   PMC_SUCCESS if the outputs could successfully be determined.
*   A specific error code is returned otherwise.
*
* NOTES:
*   The outputs are only valid upon a return of PMC_SUCCESS and there exists
*   a case where even if the user does not request PMON, they will have it 
*   provisioned regardless.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_pmon_is_available_and_case_determine(digi_handle_t* digi_handle,
                                                                digi_mapping_mode_t mode,
                                                                BOOL8 enet_pmon,
                                                                mapotn_src_dest_t mapotn_dest,
                                                                BOOL8* enet_line_as_pmon,
                                                                digi_enet_pmon_type_t* enet_pmon_type_ptr,
                                                                util_global_cpb_port_t* cpb_port_type_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    enet_datapath_t e_mode = LAST_ENET_DATAPATH;      /* This is only used as a placeholder */
    digi_enet_mode_t digi_enet_mode = LAST_DIGI_ENET; /* This is only used as a placeholder */

    PMC_ENTRY();
    PMC_ASSERT(enet_pmon_type_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(cpb_port_type_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);


    /* ENET_SYS cannot be used as PMON in TRANSMUXPONDER application */
    if ((mapotn_dest == MAPOTN_DEST_CPB) && (enet_pmon == TRUE) &&
        (DIGI_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) != TRUE)) {

        /* call mapper helper */
        result = digi_enet_sys_pmon_map_validate_and_mode_set(digi_handle,
                                                              mode, 
                                                              DIGI_ODUK_ODU2, /* does not matter */
                                                              LAST_DIGI_ENET, /* does not matter */
                                                              &e_mode, 
                                                              &digi_enet_mode); /* Values of output parameters are not used */

        /* Case 1 -> Case 4 */
        *enet_line_as_pmon = FALSE;
        *enet_pmon_type_ptr = DIGI_ENET_SYS_PMON;
        *cpb_port_type_ptr = UTIL_GLOBAL_CPB_PORT_MAPOTN;

    } else if (mapotn_dest == MAPOTN_DEST_ENET) {

        /* Case 2 */
        /* Regardless of weather we want PMON, it is part of the natural
         * datapath so you get it even if you didn't ask for it. */
        *enet_line_as_pmon = TRUE;
        *enet_pmon_type_ptr = DIGI_NO_PMON;
        *cpb_port_type_ptr = UTIL_GLOBAL_CPB_PORT_ENET_LINE;
        result = PMC_SUCCESS;

    } else if ((mapotn_dest == MAPOTN_DEST_CPB_ENET) && 
               (enet_pmon == TRUE) &&
               (DIGI_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) == TRUE || 
                DIGI60_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) == TRUE || 
                DIGI60_PROD_APP_IS_TRANSMUXPONDER_SYS_CLIENT_CARD(digi_handle) == TRUE)) {
            /** this case - datapath is using ENET(SYS), etrans path is used for PMON */
        /* Case 3 */
        /* This case can only occur in a transmuxponder application or 
         * if we are in this mapper mode: DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP */
        *enet_line_as_pmon = TRUE;
        *enet_pmon_type_ptr = DIGI_ENET_LINE_PMON;
        *cpb_port_type_ptr = UTIL_GLOBAL_CPB_PORT_MAPOTN;
        result = PMC_SUCCESS;

    } else if (enet_pmon == TRUE) {

        result = DIGI_ERR_ENET_PMON_NOT_SUPPORTED;

    } else {

        /* Case 1 */
        *enet_pmon_type_ptr = DIGI_NO_PMON;
        *cpb_port_type_ptr = UTIL_GLOBAL_CPB_PORT_MAPOTN;
        result = PMC_SUCCESS;
    }

    PMC_RETURN(result);
} /* digi_enet_pmon_is_available_and_case_determine */

/*******************************************************************************
* digi_enet_enhanced_pmon_ptr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to return the pointer to the ENET line/sys chnl
*   handle if it is being used in a PMON fashion.
*
* INPUTS:
*   *util_sw_ptr       - a pointer to generic util_global_swich_data_def_t.
*                        See util_global_swich_data_def_t for more information.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   A pointer to the ENET line/sys chnl handle if it has been set, NULL
*   otherwise.
*
* NOTES:
*   NONE.
*
*******************************************************************************/
PUBLIC digi_enet_client_chnl_def_t* digi_enet_enhanced_pmon_ptr_get(util_global_switch_data_def_t* util_sw_ptr)
{
    digi_enet_client_chnl_def_t* ret_enet_ptr = NULL;
    PMC_ENTRY();

    if (util_sw_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {
        if (DIGI_ENET_LINE_PMON == ((digi_mapper_chnl_def_t*)util_sw_ptr)->enet_pmon ||
            DIGI_ENET_SYS_PMON == ((digi_mapper_chnl_def_t*)util_sw_ptr)->enet_pmon)
        {            
            ret_enet_ptr = ((digi_mapper_chnl_def_t*)util_sw_ptr)->enet_pmon_data;
        }

    } else if ((util_sw_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) || 
               (util_sw_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {
        ret_enet_ptr = ((digi_ilkn_chnl_def_t*)util_sw_ptr)->enet_pmon_data;
    } else if (util_sw_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) {

        /* This is the case for where MAPOTN goes through ETRANS */
        /* If this is trully an ENET channel, this should be NULL */
        ret_enet_ptr = ((digi_mapper_chnl_def_t*)util_sw_ptr)->enet_pmon_data;
            
    }

    PMC_RETURN(ret_enet_ptr);
} /* digi_enet_enhanced_pmon_ptr_get */

/*******************************************************************************
* digi_enet_sys_pmon_map_validate_and_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if the application is the correct
*   state for ENET_SYS PMON and then map the map_mode to the correct enet_mode.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   map_mode               - the MAPOTN mapper mode in use.
*   odu_data               - used only with DIGI_MAP_ODUKP_PACKET_GFP
*   enet_line_mode         - Mode of enet in datapath, not PMON enet.
*                            Used only with DIGI_MAP_ODUKP_PACKET_GFP.
*
* OUTPUTS:
*   *enet_mode             - the ENET PMON to set corresponding to the map_mode.
*   *digi_enet_mode        - the equivalent DIGI ENET mode. Mode of PMON enet.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the setup was
*   required and successful, specific error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_pmon_map_validate_and_mode_set(digi_handle_t *digi_handle,
                                                              digi_mapping_mode_t map_mode, 
                                                              digi_oduk_t odu_data,
                                                              digi_enet_mode_t enet_line_mode,
                                                              enet_datapath_t* enet_mode, 
                                                              digi_enet_mode_t* digi_enet_mode)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    PMC_ENTRY();

    /* Cannot use ENET SYS for PMON if *both* SIFD1 and SFID2 are in use */

    if (digi_handle->var.sifd_dcpb_in_use == DIGI_SIFD_1_AND_2_USED) {

        result = DIGI_ERR_ENET_PMON_RESOURCE_UNAVAILABLE;

    } else {

        /* For the mapper case the card must be in 
         * DIGI_PROD_APP_LINE_CARD or DIGI_PROD_APP_SYSOTN_CARD major mode to
         * have PMON enabled via ENET_SYS 
         * We don't support Enhanced PMON for the Transmuxponder card 
         * as we will potentially have resource conflicts */
        switch(digi_handle->var.prod_app)
        {
            /* The two cases are meant to fall through to a success */
        case DIGI_PROD_APP_LINE_CARD:
        case DIGI_PROD_APP_SYSOTN_CARD:
        case DIGI_PROD_APP_100G_3_X_40G_CFP_CARD:
            result = PMC_SUCCESS;   
            break;
            /* The following cases are meant to fall through to a failure */
        case DIGI_PROD_APP_PORTLESS_CARD:
        case DIGI_PROD_APP_TRANSMUXPONDER_CARD:
        case DIGI60_PROD_APP_LINE_CARD:
        case DIGI60_PROD_APP_TRANSMUXPONDER_CARD:
        case DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD:
        default:
            result = DIGI_ERR_ENET_PMON_NOT_SUPPORTED;
            break;
        }

        if (result == PMC_SUCCESS) {

            switch (map_mode) {

                /* In the mapotn case, we're looking at the source MAP type
                 * to determine the ENET_SYS PMON type */

            case DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET:
                *enet_mode = ENET_SLAVE_10GE_MAC_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP: 
            case DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP:
                *enet_mode = ENET_SLAVE_10GE_TRANSPARENT_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP:
            case DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET:
                *enet_mode = ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODU3P_CBR_40_GE_GMP:
                *enet_mode = ENET_SLAVE_40GE_TRANSPARENT_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODU3P_40_GE_GFP_ENET:
                *enet_mode = ENET_SLAVE_40GE_MAC_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODU4P_CBR_100_GE_GMP:
                *enet_mode = ENET_SLAVE_100GE_TRANSPARENT_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODU4P_100_GE_GFP_ENET:
                *enet_mode = ENET_SLAVE_100GE_MAC_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
                break;
            case DIGI_MAP_ODUKP_PACKET_GFP:
                *enet_mode = ENET_SLAVE_10GE_MAC_TX_PMON;
                *digi_enet_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
                if (DIGI_ODUK_ODU3 == odu_data) {
                    *enet_mode = ENET_SLAVE_40GE_MAC_TX_PMON;
                    *digi_enet_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
                } else if (DIGI_ODUK_ODU4 == odu_data) {
                    *enet_mode = ENET_SLAVE_100GE_MAC_TX_PMON;
                    *digi_enet_mode = DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
                } else if (DIGI_ODUK_ODUFLEX == odu_data) {
                    switch (enet_line_mode) {
                        case DIGI_ENET_SERDES_10GE_MAC:
                        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
                        case DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
                        case DIGI_ENET_SERDES_10GE_TRANSPARENT:
                        case DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
                            *enet_mode = ENET_SLAVE_10GE_MAC_TX_PMON;
                            *digi_enet_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
                            break;

                        case DIGI_ENET_SERDES_40GE_MAC:
                        case DIGI_ENET_SERDES_40GE_TRANSPARENT:
                        case DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
                            *enet_mode = ENET_SLAVE_40GE_MAC_TX_PMON;
                            *digi_enet_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
                            break;

                        case DIGI_ENET_SERDES_100GE_MAC:
                        case DIGI_ENET_SERDES_100GE_TRANSPARENT:
                        case DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
                            *enet_mode = ENET_SLAVE_100GE_MAC_TX_PMON;
                            *digi_enet_mode = DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
                            break;

                        default:
                            result = DIGI_ERR_ENET_PMON_NOT_SUPPORTED;
                            break;
                    }
                }
                break;
            default:
                result = DIGI_ERR_ENET_PMON_NOT_SUPPORTED;
                break;

            }
        }
    }

    PMC_RETURN(result);

} /* digi_enet_sys_pmon_map_validate_and_mode_set */

/*******************************************************************************
* digi_enet_pmon_ilkn_validate_and_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if the application is the correct
*   state for ENET_SYS or ENET_LINE PMON and then map the map_mode to the 
*   correct enet_mode.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   chnl_link              - the ENET LINE chnl/link to request the datapath
*                            from.
*   enet_pmon              - the type of digi_enet_pmon_type_t pmon that is
*                            being requeste here.
*   
* OUTPUTS:
*   *enet_mode             - the ENET PMON to set corresponding to the map_mode.
*   *digi_enet_mode        - the equivalent DIGI ENET mode 
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the setup was
*   required and successful, specific error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_pmon_ilkn_validate_and_mode_set(digi_handle_t *digi_handle,
                                                           UINT32 chnl_link, 
                                                           digi_enet_pmon_type_t enet_pmon,
                                                           enet_datapath_t* enet_mode, 
                                                           digi_enet_mode_t* digi_enet_mode)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    enet_datapath_t datapath = ENET_UNPROV_DATAPATH;

    PMC_ENTRY();

    /* For the ilkn case the card must be in 
     * DIGI_PROD_APP_PORTLESS_CARD, DIGI_PROD_APP_LINE_CARD or 
     * DIGI_PROD_APP_100G_3_X_40G_CFP_CARD major mode to
     * have PMON enabled via ENET_SYS */
    switch(digi_handle->var.prod_app)
    {
        /* The three cases are meant to fall through to a success */
    case DIGI_PROD_APP_LINE_CARD:
    case DIGI60_PROD_APP_LINE_CARD:
    case DIGI_PROD_APP_PORTLESS_CARD:
    case DIGI_PROD_APP_100G_3_X_40G_CFP_CARD:
        result = PMC_SUCCESS;   
        break;
        /* The following cases are meant to fall through to a failure */
    case DIGI_PROD_APP_TRANSMUXPONDER_CARD:
        if (enet_pmon == DIGI_ENET_LINE_PMON) {
            result = PMC_SUCCESS;
        }
        break;
    case DIGI60_PROD_APP_TRANSMUXPONDER_CARD:
    case DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD:
        if (enet_pmon == DIGI_ENET_LINE_PMON) {
            result = PMC_SUCCESS;
        }
        break;
    case DIGI_PROD_APP_SYSOTN_CARD:
    default:
        result = DIGI_ERR_ENET_PMON_NOT_SUPPORTED;
        break;
    }

    if (result == PMC_SUCCESS) {

        enet_handle_t *enet_handle = NULL;

        if (enet_pmon == DIGI_ENET_SYS_PMON) { 

            /* Ask for the opposite handle as we are looking up the companion
             * from where we want the PMON to actualy reside */
            enet_handle = digi_handle->enet_line_handle;

        } else if (enet_pmon == DIGI_ENET_LINE_PMON) {

            /* Ask for the opposite handle as we are looking up the companion
             * from where we want the PMON to actualy reside */
            enet_handle = digi_handle->enet_sys_handle;

        } else {

            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        }

        
        /* Looking at the companion handle destination type to determine 
         * the PMON type */
        enet_convert_link_to_datapath_type(enet_handle, 
                                           chnl_link, 
                                           &datapath);

        switch (datapath) {
        case ENET_SERDES_100GE_MAC:
            *enet_mode = ENET_SLAVE_100GE_MAC_TX_PMON;
            *digi_enet_mode = DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
            break;
        case ENET_SERDES_100GE_TRANSPARENT:
        case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
            *enet_mode = ENET_SLAVE_100GE_TRANSPARENT_TX_PMON;
            *digi_enet_mode = DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON;
            break;
        case ENET_SERDES_40GE_MAC:
            *enet_mode = ENET_SLAVE_40GE_MAC_TX_PMON; 
            *digi_enet_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
            break;
        case ENET_SERDES_40GE_TRANSPARENT:
        case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
            *enet_mode = ENET_SLAVE_40GE_TRANSPARENT_TX_PMON; 
            *digi_enet_mode = DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON;
            break;
        case ENET_SERDES_10GE_MAC:
            *enet_mode = ENET_SLAVE_10GE_MAC_TX_PMON;
            *digi_enet_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
            break;
        case ENET_SERDES_10GE_TRANSPARENT:
        case ENET_SERDES_10GE_TRANSPARENT_RX_PMON:
            *enet_mode = ENET_SLAVE_10GE_TRANSPARENT_TX_PMON;
            *digi_enet_mode = DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON;
            break;
        case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
            *enet_mode = ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON;
            *digi_enet_mode = DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON;
            break;
        default:
            result = DIGI_ERR_ENET_PMON_NOT_SUPPORTED;
            break;
        } 
    }

    PMC_RETURN(result);

} /* digi_enet_pmon_ilkn_validate_and_mode_set */

/*******************************************************************************
* digi_enet_mgmt_fege_vlan_id_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks VLAN_ID is in rage of values [0..4095]. 
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*    vlan_id              - VLAN_ID
*
* OUTPUTS:
*   None.
*   
*
* RETURNS:
*   TRUE when vlan_id is in range of values, otherwise FALSE.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_mgmt_fege_vlan_id_check(digi_handle_t          *digi_handle,
                                                UINT16                  vlan_id)
{
    BOOL8 rc;
    PMC_ENTRY();

    rc = vlan_id > 4095 ? FALSE : TRUE;
    PMC_RETURN(rc);

} /* digi_enet_mgmt_fege_vlan_id_check */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_or_line_map_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON is required, if so, 
*   weather it is to be configured as the line side companion or the sys side
*   companion.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel to examine.
*   rate_data        - enum, client stream types of signal passing through CPB.
*                      For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, client_num_odu0 
*                      must be provided
*   client_num_odu0  - For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, number of ODU0 
*                      container associated with ODUFLEX signal.
*   halt_buffer      - CPB FIFO buffer room to capture in flight data in
*                      the event that CPB FIFO generates an upstream flow
*                      control signal.  Used in cases where ENET utilizes
*                      pause flow control and where a channel is fed from
*                      SIFD and asserts XOFF flow control.  halt_buffer is
*                      specified in units of blocks.  The block size in bytes 
*                      per unit is: 12288 bytes.
*                      Applicable to client_data containing:
*                      "DIGI_CPB_STREAM_PKT_"
*   pmon_req_type    - The type of PMON that is required. ENET LINE or SYS
*   *enet_chnl_ptr   - The output handle to the ENET LINE or SYS resource.
*   enet_mode        - The PMON mode that ENET LINE or SYS resource should use.
*   digi_enet_mode   - Equivalent DIGI ENET mode enumerated type
*
* OUTPUTS:
*   **enet_chnl_mate_pptr - The pointer to the pointer to the ENET LINE or SYS 
*                           mate channel context so it can be passed back to 
*                           the calling function
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the setup was
*   required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_or_line_map_helper(digi_handle_t* digi_handle,
                                                                 util_global_switch_data_t* input_pc_ptr, 
                                                                 digi_cpb_stream_t rate_data,
                                                                 UINT32 client_num_odu0,
                                                                 UINT32 halt_buffer,
                                                                 digi_enet_enhanced_pmon_req_t pmon_req_type,
                                                                 digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                                 enet_datapath_t enet_mode, 
                                                                 digi_enet_mode_t digi_enet_mode,
                                                                 digi_enet_client_chnl_def_t** enet_chnl_mate_pptr)
{

    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    PMC_ENTRY();

    if ((pmon_req_type == DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_MAP) ||
        (pmon_req_type == DIGI_ENHANCED_ENET_SYS_PMON_REQ_AND_REALIZABLE_ILKN)) {

        result = digi_enet_enhanced_pmon_sys_map_helper(digi_handle,
                                                        input_pc_ptr, 
                                                        rate_data,
                                                        client_num_odu0,
                                                        halt_buffer,
                                                        enet_chnl_ptr,
                                                        enet_mode, 
                                                        digi_enet_mode,
                                                        enet_chnl_mate_pptr);


    } else if (pmon_req_type == DIGI_ENHANCED_ENET_LINE_PMON_REQ_AND_REALIZABLE_ILKN) { 

        result = digi_enet_enhanced_pmon_line_map_helper(digi_handle,
                                                         input_pc_ptr, 
                                                         rate_data,
                                                         client_num_odu0,
                                                         halt_buffer,
                                                         enet_chnl_ptr,
                                                         enet_mode, 
                                                         digi_enet_mode,
                                                         enet_chnl_mate_pptr);

    } else {

        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_sys_or_line_map_helper */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_map_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET SYS PMON is required, if so, 
*   configured it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel to examine.
*   rate_data        - enum, client stream types of signal passing through CPB.
*                      For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, client_num_odu0 
*                      must be provided
*   client_num_odu0  - For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, number of ODU0 
*                      container associated with ODUFLEX signal.
*   halt_buffer      - CPB FIFO buffer room to capture in flight data in
*                      the event that CPB FIFO generates an upstream flow
*                      control signal.  Used in cases where ENET utilizes
*                      pause flow control and where a channel is fed from
*                      SIFD and asserts XOFF flow control.  halt_buffer is
*                      specified in units of blocks.  The block size in bytes 
*                      per unit is: 12288 bytes.
*                      Applicable to client_data containing:
*                      "DIGI_CPB_STREAM_PKT_"
*   *enet_chnl_ptr   - The output handle to the ENET_LINE resource.
*   enet_mode        - The PMON mode that ENET_SYS resource should use.
*   digi_enet_mode   - Equivalent DIGI ENET mode enumerated type
*
* OUTPUTS:
*   **enet_chnl_mate_pptr - The pointer to the pointer to the ENET LINE or SYS 
*                           mate channel context so it can be passed back to 
*                           the calling function
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the setup was
*   required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_map_helper(digi_handle_t* digi_handle,
                                                         util_global_switch_data_t* input_pc_ptr, 
                                                         digi_cpb_stream_t rate_data,
                                                         UINT32 client_num_odu0,
                                                         UINT32 halt_buffer,
                                                         digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                         enet_datapath_t enet_mode, 
                                                         digi_enet_mode_t digi_enet_mode,
                                                         digi_enet_client_chnl_def_t** enet_chnl_mate_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 link = 0;
    digi_serdes_port_mode_t serdes_mode;
    digi_group_size_t group_size = LAST_DIGI_GRP_SIZE;

    PMC_ENTRY();

    /* ENET link is 1:1 mapped with post_mld_dsi */
    link = enet_chnl_ptr->switch_data.cpb_data.channel;
    serdes_mode = enet_chnl_ptr->serdes_mode;

    digi_enet_pmon_prov_params_convert(enet_mode, &group_size);

    if(result == PMC_SUCCESS)
    {
        result = digi_enet_sys_db_prov_cfg(digi_handle, group_size, link);
    }

    if (result == PMC_SUCCESS && enet_mode == ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON)
    {
        /* We need to wire up a clock for this mode to become active. */
        result = digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_set_helper(digi_handle);
    } 

    if (result == PMC_SUCCESS)
    {
        /* Initialize the subsystem */
        digi_enet_sys_energy_state_set(digi_handle,link, enet_mode, PMC_ENERGY_STATE_REQUEST_RUNNING);
    }

    /* enable SERDES clock */
    if (result == PMC_SUCCESS)
    {
        result = digi_sifd_convert_and_enable_clock_select(digi_handle,
                                                           link,
                                                           enet_mode,
                                                           TRUE);
    }
    if (PMC_SUCCESS == result)
    {
        /* Configure ENET_SYS PMON path */
        /* use chnl/link from ENET_LINE as we need the same TX clock */
        result = enet_datapath_cfg(digi_handle->enet_sys_handle, 
                                   link,
                                   enet_mode, 
                                   0, FALSE, FALSE);
        PMC_ASSERT(PMC_SUCCESS==result, result, 0, 0);
    }

    if (result == PMC_SUCCESS)
    {
        /* Configure ENET_SYS PMON path */
        /* use chnl/link from ENET_LINE as we need the same TX clock */
        result = enet_rx_activate(digi_handle->enet_sys_handle, 
                                  link,
                                  enet_mode);
        PMC_ASSERT(PMC_SUCCESS==result, result, 0, 0);
    }

    if (result == PMC_SUCCESS)
    {
        /* Configure ENET_SYS PMON path */
        /* use chnl/link from ENET_LINE as we need the same TX clock */
        result = enet_tx_activate(digi_handle->enet_sys_handle, 
                                  link,
                                  enet_mode);
        PMC_ASSERT(PMC_SUCCESS==result, result, 0, 0);
    }

    if (result == PMC_SUCCESS) 
    {
        /* use the same link on ENET_SYS as on ENET_LINE */
        *enet_chnl_mate_pptr = digi_enet_client_chnl_get(digi_handle, 
                                                         serdes_mode, 
                                                         digi_enet_mode, 
                                                         link, 
                                                         UTIL_GLOBAL_CPB_PORT_ENET_SYS, 
                                                         DIGI_ENET_RES_PMON);
    }

    /* Copy MAC settings from real ENET handle to the companion ENET instance */
    if ((result == PMC_SUCCESS) &&
        (digi_enet_is_link_mac_block(digi_handle->enet_line_handle, link) == TRUE) &&
        (enet_mode != ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON)) { 

        result = digi_enet_enhanced_mac_settings_copy(digi_handle, 
                                                      enet_chnl_ptr, 
                                                      *enet_chnl_mate_pptr);
    }

    if (result == PMC_SUCCESS) {

        /* Now we have to CPB map it! (Bifurcate) */
        result = digi_cpb_internal_chnl_map(digi_handle,
                                            input_pc_ptr,
                                            *enet_chnl_mate_pptr,
                                            rate_data,
                                            client_num_odu0,
                                            halt_buffer);
    }


    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_sys_map_helper */

/*******************************************************************************
* digi_enet_enhanced_pmon_line_map_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET LINE PMON is required, if so, 
*   configured it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel to examine.
*   rate_data        - enum, client stream types of signal passing through CPB.
*                      For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, client_num_odu0 
*                      must be provided
*   client_num_odu0  - For DIGI_CPB_STREAM_PKT_GFPF_ODUFLEX, number of ODU0 
*                      container associated with ODUFLEX signal.
*   halt_buffer      - CPB FIFO buffer room to capture in flight data in
*                      the event that CPB FIFO generates an upstream flow
*                      control signal.  Used in cases where ENET utilizes
*                      pause flow control and where a channel is fed from
*                      SIFD and asserts XOFF flow control.  halt_buffer is
*                      specified in units of blocks.  The block size in bytes 
*                      per unit is: 12288 bytes.
*                      Applicable to client_data containing:
*                      "DIGI_CPB_STREAM_PKT_"
*   *enet_chnl_ptr   - The output handle to the ENET_SYS resource.
*   enet_mode        - The PMON mode that ENET_LINE resource should use.
*   digi_enet_mode   - Equivalent DIGI ENET mode enumerated type
*
* OUTPUTS:
*   **enet_chnl_mate_pptr - The pointer to the pointer to the ENET LINE or SYS 
*                           mate channel context so it can be passed back to 
*                           the calling function
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the setup was
*   required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_line_map_helper(digi_handle_t* digi_handle,
                                                          util_global_switch_data_t* input_pc_ptr, 
                                                          digi_cpb_stream_t rate_data,
                                                          UINT32 client_num_odu0,
                                                          UINT32 halt_buffer,
                                                          digi_enet_client_chnl_def_t* enet_chnl_ptr,
                                                          enet_datapath_t enet_mode, 
                                                          digi_enet_mode_t digi_enet_mode,
                                                          digi_enet_client_chnl_def_t** enet_chnl_mate_pptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 link = 0;
    digi_serdes_port_mode_t serdes_mode;
    digi_group_size_t group_size = LAST_DIGI_GRP_SIZE;

    PMC_ENTRY();

    /* ENET link is 1:1 mapped with post_mld_dsi */
    link = enet_chnl_ptr->switch_data.cpb_data.channel;
    serdes_mode = enet_chnl_ptr->serdes_mode;

    digi_enet_pmon_prov_params_convert(enet_mode, &group_size);

    if (digi_enet_line_db_prov_cfg(digi_handle, group_size, link,FALSE) != PMC_SUCCESS) {

    } else  {

        if (enet_mode == ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON) { 
            /* Unsuported mode */
            result = DIGI_ERR_CODE_ASSERT;
            PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

        } 

        if (result == PMC_SUCCESS) {

            /* Initialize the subsystem */
            digi_enet_line_energy_state_set(digi_handle,link, enet_mode, PMC_ENERGY_STATE_REQUEST_RUNNING);            

            /* Only attempt activation of ENET LINE PMON if we have some data here */
            /* Convert the digi_enet_mode_t to a number of link lanes and
             * select the DSIS clock and enable it */
            result = digi_lifd_convert_and_enable_clock_select(digi_handle, 
                                                               link,
                                                               enet_mode, 
                                                               TRUE);
        }
        
        if (result == PMC_SUCCESS) {

            /* Configure ENET_LINE PMON path */
            /* use chnl/link from ENET_LINE as we need the same TX clock */
            if (enet_datapath_cfg(digi_handle->enet_line_handle, 
                                  link,
                                  enet_mode, 
                                  0, FALSE, FALSE) != PMC_SUCCESS) {


                /* Assert due to bad response */
                result = DIGI_ERR_CODE_ASSERT;
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);

            } else {

                if (result == PMC_SUCCESS) {

                    /* use the same link on ENET_LINE as on ENET_SYS */
                    *enet_chnl_mate_pptr = digi_enet_client_chnl_get(digi_handle, 
                                                                     serdes_mode, 
                                                                     digi_enet_mode, 
                                                                     link, 
                                                                     UTIL_GLOBAL_CPB_PORT_ENET_LINE, 
                                                                     DIGI_ENET_RES_PMON);

                    /* Copy MAC settings from real ENET handle to the companion ENET instance */
                    if ((digi_enet_is_link_mac_block(digi_handle->enet_sys_handle, link) == TRUE) &&
                        (enet_mode != ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON)) { 

                        result = digi_enet_enhanced_mac_settings_copy(digi_handle, 
                                                                      enet_chnl_ptr, 
                                                                      *enet_chnl_mate_pptr);
                    }

                    if (result == PMC_SUCCESS) {

                        /* Now we have to CPB map it! (Bifurcate) */
                        result = digi_cpb_internal_chnl_map(digi_handle,
                                                            input_pc_ptr,
                                                            *enet_chnl_mate_pptr,
                                                            rate_data,
                                                            client_num_odu0,
                                                            halt_buffer);
                    }
                }
            }
        }
    }
    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_line_map_helper */

/*******************************************************************************
* digi_enet_verify_enet_pmon_and_demap
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if ENET PMON pointer is not null,
*   then deconfigure it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the pointer to the input channel to examine.
*   *output_pc_ptr   - the pointer to the output channel to examine.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, if no PMON is required to be set up or if the deconstruction 
*   was required and successful, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_verify_enet_pmon_and_demap(digi_handle_t* digi_handle,
                                                       util_global_switch_data_t* input_pc_ptr,
                                                       util_global_switch_data_t* output_pc_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT32 link = 0;

    digi_mapper_chnl_def_t* mapper_chnl = NULL;
    digi_ilkn_chnl_def_t* ilkn_chnl = NULL;
    digi_enet_client_chnl_def_t *enet_chnl_ptr = NULL;
    digi_enet_pmon_type_t enet_pmon = DIGI_NO_PMON;

    util_global_switch_data_def_t* in_util_ptr = ((util_global_switch_data_def_t*)input_pc_ptr);
    
    PMC_ENTRY();

    enet_chnl_ptr = digi_enet_enhanced_pmon_ptr_get(in_util_ptr);

    if (enet_chnl_ptr != NULL) {

        /* If the input_pc_ptr has an enet_pmon != NULL then demap the PMON
         * path.  */

        output_pc_ptr = (util_global_switch_data_t*)enet_chnl_ptr;

        result = digi_cpb_internal_chnl_demap(digi_handle,
                                              digi_handle->dcpb_handle,
                                              enet_chnl_ptr->switch_data.cpb_data.port_type,
                                              input_pc_ptr,
                                              output_pc_ptr);
        if (result == PMC_SUCCESS) {

            /* ENET link is 1:1 mapped with post_mld_dsi */
            link = enet_chnl_ptr->switch_data.cpb_data.channel;
        }

        if (digi_enet_enhanced_pmon_sys_or_line_demap_helper(digi_handle,
                                                             input_pc_ptr, 
                                                             link,
                                                             &enet_pmon) == PMC_SUCCESS) {

            if (enet_pmon == DIGI_ENET_LINE_PMON) {

                digi_enet_pmon_line_usage_set(digi_handle,
                                              link, 
                                              enet_chnl_ptr->mode,
                                              FALSE);

            } else if (enet_pmon == DIGI_ENET_SYS_PMON) {

                digi_enet_pmon_sys_usage_set(digi_handle,
                                             link, 
                                             enet_chnl_ptr->mode,
                                             FALSE);

            } else {

                /* Bad state */
                PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
            }

            /* Reset the handle to the default state */
            digi_chnl_ptr_initialize(&(enet_chnl_ptr->switch_data));
 
            if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

                /* We now now the type so cast appropriately */
                mapper_chnl = (digi_mapper_chnl_def_t*)input_pc_ptr;

                /* We are going to remove this PMON path, so set the pointer to NULL */
                mapper_chnl->enet_pmon_data = NULL;

            } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
                       (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

                /* We now now the type so cast appropriately */
                ilkn_chnl = (digi_ilkn_chnl_def_t*)input_pc_ptr; 

                /* We are going to remove this PMON path, so set the pointer to NULL */
                ilkn_chnl->enet_pmon_data = NULL;
            }
        }
    }

    PMC_RETURN(result);
} /* digi_enet_verify_enet_pmon_and_demap */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_or_line_demap_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if we need to demap the ENET SYS or
*   ENET LINE resource, then demap it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   *input_pc_ptr    - the input pointer to determine what type it is.
*   link             - the ENET LINE or SYS link number to deprovision.
*
* OUTPUTS:
*enet_pmon       - Pointer to the enet_pmon type.
*   
*
* RETURNS:
*   PMC_SUCCESS, upon success, a specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_or_line_demap_helper(digi_handle_t* digi_handle,
                                                                   util_global_switch_data_t* input_pc_ptr,
                                                                   UINT32 link,
                                                                   digi_enet_pmon_type_t* enet_pmon)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    digi_mapper_chnl_def_t* mapper_chnl = NULL;
    digi_ilkn_chnl_def_t* ilkn_chnl = NULL;

    util_global_switch_data_def_t* in_util_ptr = ((util_global_switch_data_def_t*)input_pc_ptr);
    
    PMC_ENTRY();

    if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

        /* We now know the type so cast appropriately */
        mapper_chnl = (digi_mapper_chnl_def_t*)input_pc_ptr;
        *enet_pmon = mapper_chnl->enet_pmon;

    } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
               (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

        /* We now know the type so cast appropriately */
        ilkn_chnl = (digi_ilkn_chnl_def_t*)input_pc_ptr; 
        *enet_pmon = ilkn_chnl->enet_pmon;
    }

    if (*enet_pmon == DIGI_ENET_SYS_PMON) {

        result = digi_enet_enhanced_pmon_sys_demap_helper(digi_handle, link);

    } else if (*enet_pmon == DIGI_ENET_LINE_PMON) {

        result = digi_enet_enhanced_pmon_line_demap_helper(digi_handle, link);

    } else {

        /* Bad state */
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    PMC_RETURN(result);

} /* digi_enet_enhanced_pmon_sys_or_line_demap_helper */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_demap_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if we need to demap the ENET SYS 
*   resource, then demap it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   link             - the ENET SYS link number to deprovision.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, upon success, a specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_demap_helper(digi_handle_t* digi_handle,
                                                           UINT32 link)
{

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    enet_datapath_t enet_mode;
        
    enet_convert_link_to_datapath_type(digi_handle->enet_sys_handle, link, &enet_mode);

    if (enet_mode == ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON) { 

        /* We need to unwire a potentially used SERDES clock for this to become entirely deactive. */
        result = digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_unset_helper(digi_handle);
    }

    if (result == PMC_SUCCESS) 
    {
        result = enet_rx_deactivate(digi_handle->enet_sys_handle, 
                                    link, 
                                    enet_mode);
    }

    if (result == PMC_SUCCESS) 
    {
        result = enet_tx_deactivate(digi_handle->enet_sys_handle, 
                                    link, 
                                    enet_mode);
    }

    if (result == PMC_SUCCESS) {

        result = enet_datapath_uncfg(digi_handle->enet_sys_handle, link);
    }

    if (result == PMC_SUCCESS) 
    {
        result = digi_sifd_convert_and_enable_clock_select(digi_handle,
                                                           link,
                                                           enet_mode,
                                                           FALSE);
    }

    if (result == PMC_SUCCESS) {
        
        result = digi_enet_sys_db_deprov_cfg(digi_handle, link);
    }

    if (result == PMC_SUCCESS) {

        /* Uninitialize the subsystem if no channels provisioned against it */
        digi_enet_sys_energy_state_set(digi_handle, 0, ENET_UNPROV_DATAPATH, PMC_ENERGY_STATE_REQUEST_OPTIMAL);
    }

    PMC_RETURN(result);

} /* digi_enet_enhanced_pmon_sys_demap_helper */

/*******************************************************************************
* digi_enet_enhanced_pmon_line_demap_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine if we need to demap the ENET LINE 
*   resource, then demap it.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance.
*   link             - the ENET LINE link number to deprovision.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS, upon success, a specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_line_demap_helper(digi_handle_t* digi_handle,
                                                            UINT32 link)
{

    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();
    enet_datapath_t enet_mode;
        
    enet_convert_link_to_datapath_type(digi_handle->enet_line_handle, link, &enet_mode);

    if (enet_mode == ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON) { 
        /* Unsuported mode */
        result = DIGI_ERR_CODE_ASSERT;
        PMC_ASSERT(FALSE, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    if (result == PMC_SUCCESS) {

        result = enet_datapath_uncfg(digi_handle->enet_line_handle, link);

    }

    if (result == PMC_SUCCESS) 
    {
        result = digi_lifd_convert_and_enable_clock_select(digi_handle,
                                                           link,
                                                           enet_mode,
                                                           FALSE);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_line_db_deprov_cfg(digi_handle, link, FALSE);

    }

    if (result == PMC_SUCCESS) {

        /* Uninitialize the subsystem if no channels provisioned against it */
        digi_enet_line_energy_state_set(digi_handle, 0, ENET_UNPROV_DATAPATH, PMC_ENERGY_STATE_REQUEST_OPTIMAL);
    }

    PMC_RETURN(result);

} /* digi_enet_enhanced_pmon_line_demap_helper */

/*******************************************************************************
* FUNCTION: digi_enet_sys_int_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_sys_int_enabled_check_internal(digi_handle_t      *digi_handle)
{
    PMC_ERROR   result;
    PMC_ENTRY();

    result = digi_enet_int_enabled_check(digi_handle,
                                         DIGI_INT_TYPE_ENET_SYS);

    PMC_RETURN(result);
    
} /* digi_enet_sys_int_enabled_check_internal */


/*******************************************************************************
* FUNCTION: digi_enet_line_int_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_line_int_enabled_check_internal(digi_handle_t      *digi_handle)
{
    PMC_ERROR   result;
    PMC_ENTRY();

    result = digi_enet_int_enabled_check(digi_handle,
                                         DIGI_INT_TYPE_ENET_LINE);

    PMC_RETURN(result);
    
} /* digi_enet_line_int_enabled_check_internal */

/*******************************************************************************
* digi_enet_pmon_sys_usage_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   The is a helper function that sets the appropriate bits in the
*   enet_pmon_sys_usage bitfield.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance.
*   enet_link              - the ethernet link
*   mode                   - the ethernet mode that the link is 
*                            provisioned as. This is used to determine the size
*                            of the link.
*   in_use                 - Set to TRUE to turn the bits on/in_use or
*                            FALSE to turn the bits off/not_in_use.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC void digi_enet_pmon_sys_usage_set(digi_handle_t* digi_handle,
                                         UINT32 enet_link, 
                                         digi_enet_mode_t mode,
                                         BOOL8 in_use)
{
    enet_datapath_t e_mode;
    digi_group_size_t group_size;
    UINT16 bit_mask = 0;
    UINT8 num_lanes_req = 0;
    UINT16 i; 

    PMC_ENTRY();

    /* returns void */
    digi_enet_sys_prov_params_convert(mode, &e_mode, &group_size);

    switch(group_size)
    {
    case DIGI_GRP_SIZE_10G:
        num_lanes_req = DIGI_SERDES_SYS_LANES_FOR_10G;
        break;
    case DIGI_GRP_SIZE_40G:
        num_lanes_req = DIGI_SERDES_SYS_LANES_FOR_40G;
        break;
    case DIGI_GRP_SIZE_100G:
        num_lanes_req = DIGI_SERDES_SYS_LANES_FOR_100G;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    for (i = enet_link; i < (enet_link + num_lanes_req); i++) {

        bit_mask = bit_mask | (1 << i);
    }

    if (in_use == TRUE) {

        digi_handle->var.digi_enet_pmon_sys_usage = digi_handle->var.digi_enet_pmon_sys_usage | bit_mask;

    } else {

        digi_handle->var.digi_enet_pmon_sys_usage = digi_handle->var.digi_enet_pmon_sys_usage & ~bit_mask;
    }
    
    PMC_RETURN();
} /* digi_enet_pmon_sys_usage_set */

/*******************************************************************************
* digi_enet_companion_pmon_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the companion ENET PMON handle should
*             there be one.
*             This is used if setting a runtime value on one ENET handle, its
*             companion should also have the same runtime value set.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet channel handle 
*
* OUTPUTS:
*   None
*
* RETURNS:
*   util_global_switch_data_t - a pointer to the companion ENET channel handle, or NULL otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE util_global_switch_data_t* digi_enet_companion_pmon_get(digi_handle_t *digi_handle,
                                                                util_global_switch_data_t *chnl_handle)
{

    util_global_switch_data_t* input_pc_ptr = NULL;
    util_global_switch_data_t* ret_ptr = NULL;
    util_global_switch_data_def_t* in_util_ptr = NULL;
    digi_mapper_chnl_def_t* mapper_chnl;
    digi_ilkn_chnl_def_t* ilkn_chnl;

    PMC_ENTRY();

    /* We need to look up the input_chnl based on this output chnl. */
    input_pc_ptr = digi_cpb_input_port_get(digi_handle, chnl_handle);

    if (input_pc_ptr != NULL) {

        in_util_ptr = ((util_global_switch_data_def_t*)input_pc_ptr);

        if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_MAPOTN) {

            /* We now now the type so cast appropriately */
            mapper_chnl = (digi_mapper_chnl_def_t*)input_pc_ptr;

            if (digi_enet_is_enhanced_pmon_umbilical_case(digi_handle,
                                                          mapper_chnl,
                                                          mapper_chnl->enet_pmon) == FALSE) {

                /* Only set this pointer if we are *NOT* in umbilical mode. */
                ret_ptr = mapper_chnl->enet_pmon_data;
            }

        } else if ((in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN1) ||
                   (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ILKN2)) {

            /* We now now the type so cast appropriately */
            ilkn_chnl = (digi_ilkn_chnl_def_t*)input_pc_ptr; 
            ret_ptr = ilkn_chnl->enet_pmon_data;
        } else if (in_util_ptr->cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_LINE) {
            ret_ptr = input_pc_ptr;
        }

        if (chnl_handle == ret_ptr) {

            /* This is the companion of the original, return NULL */
            ret_ptr = NULL;
        }
    }

    PMC_RETURN(ret_ptr);
} /* digi_enet_companion_pmon_get */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_set_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines if another EPMM has been set up in this 
*             mode. If not, then temporarily route all ENET_SYS clocks.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code, otherwise.
*   serdes_lane_used is only valid on PMC_SUCCESS.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_set_helper(digi_handle_t *digi_handle)
{
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 ilkn1_N1_enable_mask = digi_handle->var.sifd_ilkn_1_cfg.lane.blk_to_N1_enable_mask;
    UINT32 ilkn2_N1_enable_mask = digi_handle->var.sifd_ilkn_2_cfg.lane.blk_to_N1_enable_mask;
    digi_enet_gsup43_pmon_clk_src_type_t pmon_clk_type = DIGI_ENET_GSUP43_PMON_CLK_SRC_NONE;
    UINT32 serdes_lane_used = DIGI_SERDES_PIN_NOT_USED;
    UINT32 i; /* counter */
    digi_serdes_prov_state_t serdes_port_prov_state = LAST_DIGI_SERDES_PROV_STATE;
    BOOL8 keep_sys_serdes = FALSE;
    UINT8 serdes_prov_cnt = 0;
    UINT8 master_lane = 0;
    UINT32 internal_serdes_lane[DIGI_SERDES_SYS_LANES_MAX];
    UINT32 sys_serdes_lane[DIGI_SERDES_SYS_LANES_MAX];

    PMC_ENTRY();

    for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
    {
        internal_serdes_lane[i] = 0xFF;
        sys_serdes_lane[i] = 0xFF;
    }

    if (enet_enhanced_pmon_num_links_set_up_gsup43_pmon(digi_handle->enet_sys_handle) > 0) {

        /* If this is true, we already have EPMM powered up and we should be
         * fine to continue so there is nothing to do */
        PMC_RETURN(PMC_SUCCESS);

    } else if (ilkn1_N1_enable_mask != 0) {

        result = digi_enet_enhanced_pmon_map_gsup43_7_3_pmon_ilkn_lane_get(ilkn1_N1_enable_mask,
                                                                           &serdes_lane_used);
        pmon_clk_type = DIGI_ENET_GSUP43_PMON_CLK_SRC_ILKN1;

    } else if (ilkn2_N1_enable_mask != 0) {

        result = digi_enet_enhanced_pmon_map_gsup43_7_3_pmon_ilkn_lane_get(ilkn2_N1_enable_mask,
                                                                           &serdes_lane_used);
        pmon_clk_type = DIGI_ENET_GSUP43_PMON_CLK_SRC_ILKN2;

    } 
    else 
    {
        if (digi_handle->var.prod_app == DIGI_PROD_APP_SYSOTN_CARD) 
        {
            pmon_clk_type = DIGI_ENET_GSUP43_PMON_CLK_SRC_ENET_SYS;
            /* if in SYSOTN application, check if SYS SERDES has been initialized
            ** and SYSOTN channel is active. If active, then no need to set up
            ** additional timing source for ENET_SYS PMON.
            ** We can borrow the RX clock from master lane SYS SERDES
            */

            /* get SERDES lane mapping for all SYSOTN ports */
            for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
            {
                sys_serdes_lane[i] = digi_serdes_line_and_sys_port_lane_index_for_module_lane_get(digi_handle,
                                                                                                  DIGI_SERDES_SYSTEM_INTF,
                                                                                                  i);
                if (sys_serdes_lane[i] != 0xFF)
                {
                    digi_serdes_port_prov_state_get(digi_handle,
                                                    DIGI_SERDES_SYSTEM_INTF,
                                                    sys_serdes_lane[i],
                                                    &serdes_port_prov_state); 

                    if (DIGI_SERDES_PROV_STATE_SYSOTN == serdes_port_prov_state)
                    {
                        /* SYSOTN has been provisioned already; no need to set up a new SERDES
                        ** port for ENET_SYS PMON. Borrow SYSOTN clock
                        */
                        master_lane = (serdes_prov_cnt == 0)? sys_serdes_lane[i]:master_lane; 
                        serdes_prov_cnt++;  
                                                                            
                    }
                }
            }

            if (serdes_prov_cnt == 10)
            {
                for (i = 0; i < DIGI_SERDES_SYS_LANES_MAX; i++)
                {
                    if (sys_serdes_lane[i] == 0xFF)
                    {
                        internal_serdes_lane[i] = master_lane;
                    }
                    else {
                        internal_serdes_lane[i] = sys_serdes_lane[i];
                    }
                }
                keep_sys_serdes = TRUE;
            }
            else {              
                result = digi_enet_sys_serdes_s16_gsup43_7_3_cfg(digi_handle, &serdes_lane_used);
            }
            
        }
        else {
            result = digi_enet_sys_serdes_s16_gsup43_7_3_cfg(digi_handle, &serdes_lane_used);
        }
    }

    if (result == PMC_SUCCESS) {

        if (digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage == DIGI_SERDES_PIN_NOT_USED) {

            digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage = serdes_lane_used;
            digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_clk_type = pmon_clk_type;
        }

        /* Wire SERDES clk to all locations */ 
        if (keep_sys_serdes) 
        {
            for (i=0; i < DIGI_SERDES_SYS_LANES_MAX; i++) 
            {
                sifd_serdes_dsis_rx_sel_and_rx_en_set(digi_handle->sifd_handle, 
                                                      i, /*<- enet channel number */
                                                      internal_serdes_lane[i], /*<- internal lane number */
                                                      SIFD_OTN_CLK_ENABLE | SIFD_ENET_CLK_ENABLE);
            }
        }
        else {
        
            for (i=0; i < DIGI_SERDES_SYS_LANES_MAX; i++) 
            {
                sifd_serdes_dsis_rx_sel_and_rx_en_set(digi_handle->sifd_handle, 
                                                      i,
                                                      serdes_lane_used,
                                                      SIFD_OTN_CLK_ENABLE | SIFD_ENET_CLK_ENABLE);

            }
        }
        /* Enable the lane on DSIS Receive interface */
        sifd_serdes_dft_dsis_rx_lane_dis_set(digi_handle->sifd_handle, serdes_lane_used, 0);
    }

    PMC_RETURN(result);

} /* digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_set_helper */

/*******************************************************************************
* digi_enet_sys_serdes_s16_gsup43_7_3_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets up the s16 SERDES for use as a clk so that
*             GSUP43_7_3 an be provisioned in ENET.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
*   *serdes_lane_used      - the SERDES lane used and enabled that is used for 
*                            temporary timing purposes only.
*                            This is required to be retained so we can reverse
*                            this operation.
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code, otherwise.
*   serdes_lane_used is only valid on PMC_SUCCESS.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_sys_serdes_s16_gsup43_7_3_cfg(digi_handle_t *digi_handle,
                                                          UINT32* serdes_lane_used) 
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    digi_serdes_port_mode_t serdes_rate = DIGI_SERDES_PORT_MODE_S16_10_GE;
    hostmsg_s16_intf_type_t s16_intf_type = HOSTMSG_S16_SERDES_INTF_SYS;
    UINT16 enable_mask = 0;
    UINT16 disable_mask = 0;
    UINT16 i = 0; /* counter */

    PMC_ENTRY();
 
    /* Find a SERDES pin that is not in use - Only one PIN (10G) is
     * required as we only require the timing */
    for (i=0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

        if (digi_handle->var.pin_bus_ctxt.xfi_sys_pin_ctxt[i].port_uid == DIGI_SERDES_UNASSIGNED) 
        {
            /* Store pin as value serdes_lane_used */
            *serdes_lane_used = i;
            break;
        }
    }

    /* Use value 'serdes_lane_used' as a shift modifier to create enable_mask */
    enable_mask = 1 << *serdes_lane_used;

    /* Provisioning this type of SERDES consists of 2 steps */

    /* Step 1) Setting the rate */
    result = digi_serdes_s16_cfg(digi_handle, 
                                 s16_intf_type, 
                                 enable_mask, 
                                 serdes_rate, 
                                 disable_mask);

    /* Step 2) Squelch the TX so this doesn't interfere with other SS */
    if (result == PMC_SUCCESS) {

        s16_tx_squelch_enable(digi_handle->s16_sys_handle, *serdes_lane_used);
    }

    PMC_RETURN(result);
} /* digi_enet_sys_serdes_s16_gsup43_7_3_cfg */

/*******************************************************************************
* digi_enet_sys_serdes_s16_gsup43_7_3_uncfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets up the s16 SERDES for use as a clk so that
*             GSUP43_7_3 an be provisioned in ENET.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
*   serdes_lane_used       - the SERDES lane used and to be disabled that was
*                            used temporarily for timing purposes only.
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code, otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_sys_serdes_s16_gsup43_7_3_uncfg(digi_handle_t *digi_handle,
                                                            UINT32 serdes_lane_used) 
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    digi_serdes_port_mode_t serdes_rate = DIGI_SERDES_PORT_MODE_S16_10_GE;
    hostmsg_s16_intf_type_t s16_intf_type = HOSTMSG_S16_SERDES_INTF_SYS;
    UINT16 enable_mask = 0;
    UINT16 disable_mask = 0;

    PMC_ENTRY();

    /* Use value 'serdes_lane_used' as a shift modifier to create disable_mask */
    disable_mask = 1 << serdes_lane_used;

    /* De provisioning this type of SERDES consists of 2 steps */

    /* Step 1)  Turn off the TX Squelching */
    s16_tx_squelch_disable(digi_handle->s16_sys_handle, serdes_lane_used);

    /* Step 2) Setting the rate */
    result = digi_serdes_s16_cfg(digi_handle, 
                                 s16_intf_type, 
                                 enable_mask, 
                                 serdes_rate, 
                                 disable_mask);

    PMC_RETURN(result);
} /* digi_enet_sys_serdes_s16_gsup43_7_3_uncfg */

/*******************************************************************************
* digi_enet_enhanced_pmon_map_gsup43_7_3_pmon_ilkn_lane_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines which ILKN lane has been set up to use 
*             an S16 SERDES. If not, then temporarily route all ENET_SYS clocks.
*
* INPUTS:
*   ilkn_N1_enable_mask    - The N1/S16 SERDES Mask that is used by ILKN1 or 
*                            ILKN2
*
* OUTPUTS:
*   *serdes_lane_used      - the SERDES lane used and enabled that is used for 
*                            temporary timing purposes only.
*                            This is required to be retained so we can reverse
*                            this operation.
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code, otherwise.
*   serdes_lane_used is only valid on PMC_SUCCESS.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_map_gsup43_7_3_pmon_ilkn_lane_get(UINT32 ilkn_N1_enable_mask,
                                                                            UINT32* serdes_lane_used) 
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;
    UINT16 i = 0; /* counter */

    PMC_ENTRY();

    /* Find a SERDES pin that is not in use - Only one PIN (10G) is
     * required as we only require the timing */
    for (i=0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

        if ((ilkn_N1_enable_mask & (1 << i)) > 0) {

            /* Store pin as value serdes_lane_used */
            *serdes_lane_used = i;
            result = PMC_SUCCESS;
            break;
        }
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_map_gsup43_7_3_pmon_ilkn_lane_get */

/*******************************************************************************
* digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_unset_helper
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines if another EPMM has been set up in this 
*             mode. If not, then temporarily route all ENET_SYS clocks.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code, otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_unset_helper(digi_handle_t *digi_handle)
{
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 serdes_lane_used = digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage;
    UINT16 i = 0; /* counter */

    PMC_ENTRY();

    if (serdes_lane_used == DIGI_SERDES_PIN_NOT_USED) {

        result = PMC_SUCCESS;

    } else if (enet_enhanced_pmon_num_links_set_up_gsup43_pmon(digi_handle->enet_sys_handle) > 1) {

        result = PMC_SUCCESS;

    } else {

        if (digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_clk_type == DIGI_ENET_GSUP43_PMON_CLK_SRC_ENET_SYS) 
        {
            result = digi_enet_sys_serdes_s16_gsup43_7_3_uncfg(digi_handle, serdes_lane_used);

        }

        if (result == PMC_SUCCESS) {

            digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_usage = DIGI_SERDES_PIN_NOT_USED;
            digi_handle->var.digi_enet_pmon_GSUP43_7_3_sys_clk_type = DIGI_ENET_GSUP43_PMON_CLK_SRC_NONE;

            /* Un-Wire SIFD from all locations */ 
            for (i=0; i < DIGI_SERDES_SYS_LANES_MAX; i++) {

                sifd_serdes_dsis_rx_sel_and_rx_en_set(digi_handle->sifd_handle, 
                                                      i,
                                                      serdes_lane_used,
                                                      SIFD_CLK_DISABLE);

            }

            /* Disable the lane on DSIS Receive interface */
            sifd_serdes_dft_dsis_rx_lane_dis_set(digi_handle->sifd_handle, serdes_lane_used, 1);
        }
    }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_sys_map_gsup43_7_3_pmon_clk_unset_helper */

/*******************************************************************************
* digi_enet_enhanced_pmon_fcs_settings_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function used to correctly set the FCS settings
*             in ENET_SYS and MAPOTN when we are using Enhanced ENET PMON and
*             we are in the DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET mode.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *input_pc_ptr          - pointer to the mapper channel handle instance
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code, otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_pmon_fcs_settings_set(digi_handle_t *digi_handle,
                                                          util_global_switch_data_t *input_pc_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_prod_app_t prod_app;
    BOOL crc_ins_enable = FALSE;
    BOOL crc_fw_enable = TRUE;

    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr = NULL;

    PMC_ENTRY();
    prod_app = DIGI_PROD_APP_GET(digi_handle);
    /* Get other end (ENET_SYS) chnl_handle_ptr */
    /* We need to look up the other end of the cpb channel (if there is one). */
    if(prod_app == DIGI60_PROD_APP_TRANSMUXPONDER_CARD || prod_app == DIGI60_PROD_APP_LINE_CARD
        || prod_app == DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD)
    {
        digi_mapper_chnl_def_t *mapper_chnl = (digi_mapper_chnl_def_t*)input_pc_ptr;
        enet_chnl_handle_ptr = mapper_chnl->enet_pmon_data;
    }
    else
    {
        enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *)digi_cpb_input_port_get(digi_handle, input_pc_ptr);
    }
    PMC_ASSERT(enet_chnl_handle_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);


    if(prod_app == DIGI60_PROD_APP_TRANSMUXPONDER_CARD || prod_app == DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD)
    {
        crc_ins_enable = TRUE;
        crc_fw_enable = FALSE;
    }
        /*
            2. , if the above PMON path is used in addition to a 
               "normal datapath" of MAPOTN -> DCPB -> ENET(sys) Tx, then we would 
               have no choice but to disable FCS append in the Tx MAC 
               (since the MAPOTN will have left the FCS intact).

            - Disable enet crc insertion on proper channel.
        */
        result = digi_enet_insert_crc_internal_set(digi_handle, enet_chnl_handle_ptr, crc_ins_enable);

        if (result == PMC_SUCCESS) {

            /* 
               3a. Finally, since it is desirable to have mirrored settings in the 
               reverse datapath, [ENET(sys) Rx -> DCPB -> MAPOTN] then we should 
               keep the FCS intact for that whole datapath as well. 
               (i.e. disable FCS stripping in the Rx MAC and disable addition 
               of the FCS in the MAPOTN TX).

               - Enable CRC fowarding to disable FCS stripping:
            */
            result = digi_enet_crc_forwarding_internal_set(digi_handle, enet_chnl_handle_ptr, crc_fw_enable);
        }

    PMC_RETURN(result);
} /* digi_enet_enhanced_pmon_fcs_settings_set */
/*******************************************************************************
* digi_enet_insert_crc_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the CRC insertion for traffic from the CPB of
*             the provisioned channel.
*             Only datapaths through the MAC block can have CRC insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *is_enabled            - TRUE if inabled, FALSE otherwise
*
* RETURNS:
*   PMC_SUCCESS, Specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_insert_crc_internal_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL8 *is_enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            *is_enabled = empei_e_sys_fcs_is_enabled(enet_handle->empei_e_handle, link);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_insert_crc_internal_get */

/*******************************************************************************
* digi_enet_insert_crc_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the CRC insertion for traffic from the CPB of
*             the provisioned channel.
*             Only datapaths through the MAC block can have CRC insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - CRC insertion enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS, on success. Specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_insert_crc_internal_set(digi_handle_t *digi_handle,
                                                    util_global_switch_data_t *chnl_handle,
                                                    BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = empei_e_sys_fcs_enable(enet_handle->empei_e_handle, link, enable);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_insert_crc_internal_set */

/*******************************************************************************
* digi_enet_crc_forwarding_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the CRC forwarding of the provisioned channel.
*             Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *is_enabled            - TRUE if inabled, FALSE otherwise
*
* RETURNS:
*   PMC_SUCCESS, Specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_crc_forwarding_internal_get(digi_handle_t *digi_handle,
                                                       util_global_switch_data_t *chnl_handle,
                                                       BOOL8 *is_enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            *is_enabled = emac_crc_fwd_is_enabled(enet_handle->emac_handle, link);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_crc_forwarding_internal_get */

/*******************************************************************************
* digi_enet_crc_forwarding_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the CRC forwarding of the provisioned channel.
*             Only datapaths through the MAC block can have CRC forwarding.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - CRC forwarding enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_crc_forwarding_internal_set(digi_handle_t *digi_handle,
                                                        util_global_switch_data_t *chnl_handle,
                                                        BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_crc_fwd_en(enet_handle->emac_handle, link, enable);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_crc_forwarding_internal_set */

/*******************************************************************************
* digi_enet_is_far_end_mapotn_mode_gfp_enet
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines if the far end mapped through the DCPB
*             is a MAPOTN channel, and if so, returns TRUE if the MAPOTN
*             channel is one fo the following:
*
*             DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET
*             DIGI_MAP_ODU3P_40_GE_GFP_ENET
*             DIGI_MAP_ODU4P_100_GE_GFP_ENET
*
*             It returns FALSE for any other mapping mode.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *enet_chnl_handle      - pointer to Enet channel handle instance
*
* OUTPUTS:
*   none
*
* RETURNS:
*   TRUE if the mapping mode is as described in the description.
*   It returns FALSE otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_enet_is_far_end_mapotn_mode_gfp_enet(digi_handle_t *digi_handle,
                                                        util_global_switch_data_t *enet_chnl_handle)
{
    BOOL8 rc = FALSE;

    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr = NULL;
    digi_mapper_chnl_def_t* mapper_chnl_handle_ptr = NULL;

    PMC_ENTRY();

    enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t*)enet_chnl_handle;

    if (enet_chnl_handle_ptr->switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) {

        /* Get other end (ENET_SYS) chnl_handle_ptr */
        /* We need to look up the other end of the cpb channel (if there is one). */
        mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)digi_cpb_input_port_get(digi_handle, enet_chnl_handle);

        if (mapper_chnl_handle_ptr == NULL) {

        } else {
            

            if (mapper_chnl_handle_ptr->switch_data.cpb_data.port_type != UTIL_GLOBAL_CPB_PORT_MAPOTN) {

            } else { 

                rc = digi_mapper_is_mapotn_mode_gfp_enet(mapper_chnl_handle_ptr); 
            }
        }
    }
       
    PMC_RETURN(rc);
} /* digi_enet_is_far_end_mapotn_mode_gfp_enet */

/*******************************************************************************
* digi_enet_enhanced_mac_settings_copy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function used to copy any applied runtime MAC
*   configuration from one ENET handle and apply it to the companion handle.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance.
*   *enet_chnl_src_ptr  - The source handle to obtain the ENET MAC settings.
*   *enet_chnl_dest_ptr - The destination handle to set the ENET MAC setttings.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS, on success, specific error otherwise.
*
* NOTES:
*   None.
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_enhanced_mac_settings_copy(digi_handle_t* digi_handle, 
                                                       digi_enet_client_chnl_def_t* enet_chnl_src_ptr,
                                                       digi_enet_client_chnl_def_t* enet_chnl_dest_ptr)
{
    PMC_ERROR result = DIGI_ERR_UNKNOWN;

    BOOL8 is_map_gfp_enet = FALSE;

    BOOL8 is_crc_ins_enabled;
    BOOL8 is_crc_fwd_enabled;

    UINT32 gap_value;

    UINT32 mru_value;

    BOOL8 is_phy_enabled;

    BOOL8 is_ingress_enable;
    digi_enet_irig_mode_t ingress_mode;
    BOOL8 is_egress_enable;
    digi_enet_irig_mode_t egress_mode;

    PMC_ENTRY();

    is_map_gfp_enet = digi_enet_is_far_end_mapotn_mode_gfp_enet(digi_handle, 
                                                                enet_chnl_src_ptr);

    if (is_map_gfp_enet == FALSE) {

        result = digi_enet_insert_crc_internal_get(digi_handle, enet_chnl_src_ptr, &is_crc_ins_enabled);

        if (result == PMC_SUCCESS) {

            result = digi_enet_insert_crc_internal_set(digi_handle, enet_chnl_dest_ptr, is_crc_ins_enabled);
        }

        if (result == PMC_SUCCESS) {

            result = digi_enet_crc_forwarding_internal_get(digi_handle, enet_chnl_src_ptr, &is_crc_fwd_enabled);
        }

        if (result == PMC_SUCCESS) {

            result = digi_enet_crc_forwarding_internal_set(digi_handle, enet_chnl_dest_ptr, is_crc_fwd_enabled);
        }
    }
    
    if (result == PMC_SUCCESS) {

        result = digi_enet_ipg_get(digi_handle, enet_chnl_src_ptr, &gap_value);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_ipg_internal_set(digi_handle, enet_chnl_dest_ptr, gap_value);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_mru_get(digi_handle, enet_chnl_src_ptr, &mru_value);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_mru_internal_set(digi_handle, enet_chnl_dest_ptr, mru_value);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_phy_internal_get(digi_handle, enet_chnl_src_ptr, &is_phy_enabled);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_phy_internal_set(digi_handle, enet_chnl_dest_ptr, is_phy_enabled);
    }

    /* The following functions have been deprecated
    if (result == PMC_SUCCESS) {

        result = digi_enet_egress_insert_rate_internal_get(digi_handle, 
                                                           enet_chnl_src_ptr, 
                                                           &insert_quantum, 
                                                           &system_quantum);
    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_egress_insert_rate_internal_set(digi_handle, 
                                                           enet_chnl_dest_ptr, 
                                                           insert_quantum, 
                                                           system_quantum);
    }
    */
    if (result == PMC_SUCCESS) {

        result = digi_enet_irig_configuration_internal_get(digi_handle, 
                                                           enet_chnl_src_ptr, 
                                                           &is_ingress_enable,
                                                           &ingress_mode,
                                                           &is_egress_enable,
                                                           &egress_mode);

    }

    if (result == PMC_SUCCESS) {

        result = digi_enet_irig_configuration_internal_set(digi_handle, 
                                                           enet_chnl_dest_ptr, 
                                                           is_ingress_enable,
                                                           ingress_mode,
                                                           is_egress_enable,
                                                           egress_mode);
    }

    PMC_RETURN(result);

} /* digi_enet_enhanced_mac_settings_copy */

/*******************************************************************************
* digi_enet_rx_flow_control_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enable/Disable Link Ethernet RX flow control of a provisioned channel.\n
*  
*  By enabling RX flow conbtrol, ENET channel interprets XOFF and XON pause \n
*  frames received on the interface. XOFF and XONN pause respectively stop \n
*  and re-enable the ENET transmitter.
*  Only datapaths through the MAC block can have flow control.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *chnl_handle            - pointer to ENET or mapper channel handle instance
*   enable                  - TRUE : Flow control enabled\n
*                             FALSE: Flow control disabled 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_rx_flow_control_internal_set(digi_handle_t *digi_handle,
                                                         util_global_switch_data_t *chnl_handle,
                                                         BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;

    UINT32 link;
    
    PMC_ENTRY();

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
       
    /* Retrieve the enet handle & link */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
            
        } 
        else 
        {
            enet_flow_control_enable(enet_handle, link, UTIL_GLOBAL_DIR_RX, enable, 0, 0, 0);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_rx_flow_control_internal_set */


/*******************************************************************************
* digi_enet_rx_pause_frame_fwd_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures RX pause frames fowarding
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *chnl_handle            - pointer to ENET or mapper channel handle instance
*   enable                  - TRUE : Pause frame forwarding enabled\n
*                             FALSE: Pause frame forwarding disabled 
*
* OUTPUTS:
*  None
*
* RETURNS:
*   PMC_SUCCESS - when API execution is successful otherwise a descriptive\n
*                 error is returned.
*
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_rx_pause_frame_fwd_internal_set(digi_handle_t *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    BOOL8 rx_flow_control_enabled;
    UINT32 dummy_pause_quanta;
    UINT32 dummy_pause_quanta_thresh;
    
    PMC_ENTRY();

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
       
    /* Retrieve the enet handle & link */
    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) 
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;            
        } 
        else 
        {
            if (TRUE == enable)
            {
                rc = enet_flow_control_get(enet_handle, link, UTIL_GLOBAL_DIR_RX, 
                                           &rx_flow_control_enabled,
                                           &dummy_pause_quanta,
                                           &dummy_pause_quanta_thresh);
                
                if (PMC_SUCCESS == rc)
                {
                    if (TRUE == rx_flow_control_enabled)
                    {
                        rc = DIGI_ENET_RX_FLOW_CONTROL_ENABLED;
                    }
                }
            }

            if (PMC_SUCCESS == rc)
            {
                enet_emac_pause_frame_fwd_enable(enet_handle, link, enable);
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_pause_frame_fwd_internal_set */

/*******************************************************************************
* digi_enet_mru_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the MRU of the provisioned channel.
*   Only datapaths through the MAC block can support MRU.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   length                 - length of maximum receive unit
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block or length is not valid
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_mru_internal_set(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *chnl_handle,
                                             UINT32 length)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    util_global_switch_data_t *local_chnl_handle=NULL;
    
    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                 &enet_handle, &link);

    if (rc == DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET) {
        local_chnl_handle = (util_global_switch_data_t *)(((digi_mapper_chnl_def_t *)chnl_handle)->enet_pmon_data);
        if (NULL == local_chnl_handle) {
            PMC_RETURN(rc);
        }
        rc = digi_enet_validate_parms(digi_handle, local_chnl_handle, &enet_handle, &link);
    }
    
    if(rc == PMC_SUCCESS)
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE)
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
    }
                
    if(rc == PMC_SUCCESS)
    {

        if ((length >= 64) && (length <=16352)) {

            rc = emac_max_frm_len_cfg(enet_handle->emac_handle, link, length);

        } else {

            rc = DIGI_ERR_INVALID_ARG;
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_mru_internal_set */


/*******************************************************************************
* digi_enet_tx_upi_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:  
*   This function sets the expected UPI values in data or ordered set packet 
*   and the action to be done when incoming packet UPI does not match expected 
*   values: DROP or IGNORE.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*    pkt_upi_val           - value that identifies the transmit data as a 10GE data frame.
*    os_upi_val            - value that identifies the transmit data as 10GE ordered set.
*    enable_drop           - when TRUE, incoming packet that has an UPI TYPE value that
*                            does not match either pkt_upi_val or os_upi_val
*                            are dropped.
*                            when FALSE, incoming data packet will be treated as a 
*                            data packet.
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block or length is not valid
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_tx_upi_internal_set(digi_handle_t             *digi_handle,
                                                util_global_switch_data_t *chnl_handle,
                                                UINT8                      pkt_upi_val,
                                                UINT8                      os_upi_val,
                                                BOOL8                      enable_drop)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) { 

        rc = enet_tx_upi_set(enet_handle, 
                             link, 
                             pkt_upi_val,
                             os_upi_val,
                             enable_drop);

    }

    PMC_RETURN(rc);
} /* digi_enet_tx_upi_internal_set */

/*******************************************************************************
* digi_enet_ipg_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IPG of the provisioned channel.
*   For 10G link, 
*       the function sets the number of octets in steps of 4.        
*   For 40G and 100G:
*       the function sets the transmit PPM compensation.
* 
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_handle        - pointer to Enet or mapper channel handle instance
*   gap                 - For 10G link:
*                           it is the number of octets in step of 4. Valid values 
*                            are 8, 12, 16, ... 100. 
*                            1) If DIGI is a transit-node, set gap to 8
*                            2) If DIGI is an Ethernet source, set gap to 12
*                            Note: When set to 8, PCS layers may not be able to perform
*                            clock rate compensation.
*                         For 40G and 100G link:
*                            The function sets the transmit PPM compensation.
*                            1) When gap == 2'b00 : Normal mode of operation. Markers compensation 
*                                                   active meaning effective +60ppm.
*                            2) When gap == 2'b01 : Default compensation plus 60ppm (total of +120ppm).
*                            3) When gap == 2'b10 : Default compensation plus 140ppm (total of +200ppm).
*                            4) When gap == 2'b11 : Default compensation plus 200ppm (total of +260ppm).
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when operation is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_ipg_internal_set(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *chnl_handle,
                                             UINT32 gap)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
                    
        } else {

            rc = emac_tx_ipg_cfg(enet_handle->emac_handle, link, gap);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_ipg_internal_set */

/*******************************************************************************
* digi_enet_phy_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns the enabled state of the PHY of the 
*   provisioned channel.
*   Only datapaths through the MAC block can obtain this status.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper instance
*
* OUTPUTS:
*   *is_enabled            - TRUE if inabled, FALSE otherwise
*
* RETURNS:
*   PMC_SUCCESS, Specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_phy_internal_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL8 *is_enabled)
{
    PMC_ERROR rc = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE)
        {
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;
        } else {
            *is_enabled = emac_tx_is_enabled(enet_handle->emac_handle, link);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_phy_internal_get */

/*******************************************************************************
* digi_enet_phy_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function enables the PHY of the provisioned channel
*   by enabling the MAC RX.
*   Only datapaths through the MAC block can enable the PHY.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   enable                 - PHY enable (TRUE) or disable (FALSE)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_phy_internal_set(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *chnl_handle,
                                             BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_tx_enable(enet_handle->emac_handle, link, enable);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_phy_internal_set */


/*******************************************************************************
* digi_enet_irig_configuration_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IRIG configuration.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *is_ingress_enable      - ingress IRIG enable (TRUE) or disable (FALSE)
*   *ingress_mode           - ingress IRIG mode
*   *is_egress_enable       - egress IRIG enable (TRUE) or disable (FALSE)
*   *egress_mode            - egress IRIG mode
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_irig_configuration_internal_get(digi_handle_t *digi_handle,
                                                           util_global_switch_data_t *chnl_handle,
                                                           BOOL8* is_ingress_enable,
                                                           digi_enet_irig_mode_t* ingress_mode,
                                                           BOOL8* is_egress_enable,
                                                           digi_enet_irig_mode_t* egress_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    etime_irig_mode_t mode;
    BOOL8 int_is_ingress_enable;
    BOOL8 int_is_egress_enable;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

    } 

    if (PMC_SUCCESS == rc) {
       
        etime_irigi_config_get(enet_handle->emac_handle->etime_handle,
                               &mode, 
                               &int_is_ingress_enable);


        if (mode == ETIME_IRIG_TOD) {

            *ingress_mode = DIGI_ENET_IRIG_TOD;

        } else {

            *ingress_mode = DIGI_ENET_IRIG_B00X;
        }
         
        etime_irigo_config_get(enet_handle->emac_handle->etime_handle,
                               &mode, &int_is_egress_enable);

        if (mode == ETIME_IRIG_TOD) {

            *egress_mode = DIGI_ENET_IRIG_TOD;

        } else {

            *egress_mode = DIGI_ENET_IRIG_B00X;
        }

    }

    if (PMC_SUCCESS == rc) 
    {
        *is_ingress_enable = UTIL_GLOBAL_BOOL8_TO_BOOL_CONVERT(int_is_ingress_enable);
        *is_egress_enable = UTIL_GLOBAL_BOOL8_TO_BOOL_CONVERT(int_is_egress_enable);
    }

    PMC_RETURN(rc);
} /* digi_enet_irig_configuration_internal_get */

/*******************************************************************************
* digi_enet_irig_configuration_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the IRIG configuration.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   ingress_enable         - ingress IRIG enable (TRUE) or disable (FALSE)
*   ingress_mode           - ingress IRIG mode
*   egress_enable          - egress IRIG enable (TRUE) or disable (FALSE)
*   egress_mode            - egress IRIG mode
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_irig_configuration_internal_set(digi_handle_t *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            BOOL8 ingress_enable,
                                                            digi_enet_irig_mode_t ingress_mode,
                                                            BOOL8   egress_enable,
                                                            digi_enet_irig_mode_t egress_mode)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    etime_irig_mode_t mode;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {
        
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

    }  
    if (PMC_SUCCESS == rc) {

        if (ingress_mode == DIGI_ENET_IRIG_TOD) {

            mode = ETIME_IRIG_TOD;

        } else {

            mode = ETIME_IRIG_B00X;

        }
        
        rc = etime_irigi_config_set(enet_handle->emac_handle->etime_handle,
                                    mode, ingress_enable);
        
        if (PMC_SUCCESS == rc) {

            if (egress_mode == DIGI_ENET_IRIG_TOD) {

                mode = ETIME_IRIG_TOD;

            } else {

                mode = ETIME_IRIG_B00X;
            }

            rc = etime_irigo_config_set(enet_handle->emac_handle->etime_handle,
                                        mode, egress_enable);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_irig_configuration_internal_set */


/*******************************************************************************
* FUNCTION: digi_enet_mgmt_fege_int_internal_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/Disables MGMT_FEGE interrupts based on bits that are set in the interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to interrupt table with bits set to
*                     enable interrupts
*   enable          - When TRUE interrupts are enabled, otherwise there are
*                     disabled
*
* OUTPUTS:
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_internal_enable(digi_handle_t             *digi_handle,  
                                                         mgmt_fege_int_t           *int_table_ptr,
                                                         mgmt_fege_int_t           *int_en_table_ptr,
                                                         BOOL                       enable)
{
    PMC_ERROR   result = PMC_SUCCESS;

    PMC_ENTRY();

    /* enable or disable mgmt_fege interrupts */
    if (PMC_SUCCESS == result)
    {
        result = mgmt_fege_int_enable(digi_handle->mgmt_fege_handle,
                                      int_table_ptr,
                                      int_en_table_ptr,
                                      enable);
    }

    PMC_RETURN(result);
} /* digi_enet_mgmt_fege_int_internal enable */


/*******************************************************************************
*  digi_enet_int_fw_retrieve
* ______________________________________________________________________________
*
*  DESCRIPTION:
*   Retrieve the ENET interrupts that are handled by FW. The following
*   interrupts are retrieved for all the activated links:
*     ENET_LINE:INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL
*     ENET_LINE:INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL
*     ENET_SYS:INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL
*  
*  INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
*  OUTPUTS: 
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_fw_retrieve(digi_handle_t         *digi_handle,
                                            digi_int_type_t        int_type,
                                            digi_enet_int_chnl_t  *filt_table_ptr,
                                            digi_enet_int_t       *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        

    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           lane;
    UINT32                           i;
    UINT32                           fw_mon_id;
    digi_enet_int_chnl_t            *enet_int_chnl = NULL;
    digi_enet_client_chnl_def_t     *handle_pool_ptr;
    digi_enet_client_chnl_t         *chnl_ptr;
    BOOL8                           enet_fw_retrieve_done = FALSE;

    PMC_ENTRY();
 
    /* Get handle pool ptr */
    switch (int_type)
    {
        case DIGI_INT_TYPE_ENET_LINE:
            handle_pool_ptr = digi_handle->handle_pool.enet_line_chnl;
            fw_mon_id = INTR_IPI_ENET_LINE;
            break;

        case DIGI_INT_TYPE_ENET_SYS:
            handle_pool_ptr = digi_handle->handle_pool.enet_sys_chnl;
            fw_mon_id = INTR_IPI_ENET_SYS;
            break;

        default:
            PMC_ASSERT(NULL, DIGI_ERR_INVALID_ARG, 0, 0);
            break;
    } /* end switch */


    /* Allow retrieving FW interrupts for all channels */
    enet_fw_retrieve_done = FALSE;

    /* Determine link range */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];
        if (util_global_is_chnl_prov(chnl_ptr))
        {
            if (FALSE == enet_fw_retrieve_done)
            {
                /* Retieve interrupts from FW */
                if (PMC_SUCCESS == result)
                {
                    result = digi_enet_int_fw_chnl_retrieve(digi_handle,
                                                            int_type,
                                                            filt_table_ptr,
                                                            &int_table_ptr->enet_chnl[i]);
                    enet_int_chnl = &int_table_ptr->enet_chnl[i];

                    /* Avoid retrieving FW interrupts for each channel */
                    enet_fw_retrieve_done = TRUE;
                }
            }
            else
            {
                /* Copy interrupts to this link */
                for (lane = 0; lane < EXCO4M_N_LANES; ++lane)
                {
                    int_table_ptr->enet_chnl[i].enet.epmm.exco4m.rx_lobl_i[lane] = enet_int_chnl->enet.epmm.exco4m.rx_lobl_i[lane];
                    if (int_type == DIGI_INT_CHNL_TYPE_ENET_LINE)
                    {
                        int_table_ptr->enet_chnl[i].enet.etrans.exco4m.rx_lobl_i[lane] = enet_int_chnl->enet.etrans.exco4m.rx_lobl_i[lane];
                    }
                }
            }
        }
    }

    PMC_RETURN(result);
} /* End of digi_enet_int_fw_retrieve*/   


/*******************************************************************************
*  digi_enet_int_fw_chnl_retrieve
* ______________________________________________________________________________
*
*  DESCRIPTION:
*   Retrieve the ENET interrupts that are handled by FW. The following
*   interrupts are retrieved for the specified link:
*     ENET_LINE:INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL
*     ENET_LINE:INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL
*     ENET_SYS:INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL
*  
*  INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_type        - ENET LINE or SYS interrupts to operate on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
*  OUTPUTS: 
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_int_fw_chnl_retrieve(digi_handle_t         *digi_handle,
                                                 digi_int_type_t        int_type,
                                                 digi_enet_int_chnl_t  *filt_table_ptr,
                                                 digi_enet_int_chnl_t   *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    BOOL has_epmm_rx_lobl_en;
    BOOL has_etrans_rx_lobl_en;   
    UINT32 lane;

    PMC_ENTRY();


    has_epmm_rx_lobl_en = FALSE;
    has_etrans_rx_lobl_en = FALSE;
    if (filt_table_ptr == DIGI_INT_ALL_INTS)
    {
        has_epmm_rx_lobl_en = TRUE;
        has_etrans_rx_lobl_en = TRUE;
    }
    else
    {
        for (lane = 0; lane < EXCO4M_N_LANES; ++lane)
        {
            if (filt_table_ptr->enet.epmm.exco4m.rx_lobl_i[lane] == TRUE)
            {
                has_epmm_rx_lobl_en = TRUE;
            }
            if (filt_table_ptr->enet.etrans.exco4m.rx_lobl_i[lane] == TRUE)
            {
                has_etrans_rx_lobl_en = TRUE;
            }
        }
    }

    result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                             int_type,
                                                             has_epmm_rx_lobl_en,
                                                             has_etrans_rx_lobl_en,
                                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, enet),
                                                             &int_table_ptr->enet);
    
    PMC_RETURN(result);
} /* End of digi_enet_int_fw_chnl_retrieve*/   


/*******************************************************************************
*  digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve
* ______________________________________________________________________________
*
*  DESCRIPTION:
*   Retrieve the ENET interrupts that are handled by FW. The following
*   interrupts are retrieved for the specified link:
*     ENET_LINE:INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL
*     ENET_LINE:INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL
*     ENET_SYS:INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL
*  
*  INPUTS:
*   digi_handle            - pointer to DIGI handle instance.
*   int_type               - ENET LINE or SYS interrupts to operate on
*   has_epmm_rx_lobl_en    - when TRUE EPMM rx_lobl_i interrupt is retrieved
*   has_etrans_rx_lobl_en  - when TRUE ETRANS rx_lobl_i interrupt is retrieved
*   filt_table_ptr         - pointer to filter table. Mark fields as TRUE within this
*                            table to indicate that the interrupt/status will be retrieved
*                            to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                            interrupt/status.
*
*  OUTPUTS: 
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle_t         *digi_handle,
                                                                 digi_int_type_t        int_type, 
                                                                 BOOL8                  has_epmm_rx_lobl_en,
                                                                 BOOL8                  has_etrans_rx_lobl_en,
                                                                 enet_int_chnl_t       *filt_table_ptr,
                                                                 enet_int_chnl_t       *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;     
    UINT32 lane;
    BOOL8 int_val;
    UINT32 epmm_cbr_rx_lobl_i;
    UINT32 etrans_cbr_rx_lobl_i;

    PMC_ENTRY();

    if (int_type == DIGI_INT_TYPE_ENET_LINE &&
        digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
    {
        /* Read ENET LINE interrupts from FW */
        if (has_epmm_rx_lobl_en)
        {
            /* Read EPMM_CBR_RX_LOBL */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_enet_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL,
                                                              TRUE,
                                                              &epmm_cbr_rx_lobl_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (lane = 0; lane < EXCO4M_N_LANES; lane += 1)
                {
                    int_val = (epmm_cbr_rx_lobl_i >> lane) & ((NULL == filt_table_ptr) ? 0x1 : filt_table_ptr->epmm.exco4m.rx_lobl_i[lane]);
                    int_table_ptr->epmm.exco4m.rx_lobl_i[lane] = int_val;
                }
            }
        }

        if (has_etrans_rx_lobl_en)
        {
            /* Read EPMM_CBR_RX_LOBL */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_enet_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL,
                                                              TRUE,
                                                              &etrans_cbr_rx_lobl_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (lane = 0; lane < EXCO4M_N_LANES; lane += 1)
                {
                    int_val = (etrans_cbr_rx_lobl_i >> lane) &  ((NULL == filt_table_ptr) ? 0x1 : filt_table_ptr->etrans.exco4m.rx_lobl_i[lane]);
                    int_table_ptr->etrans.exco4m.rx_lobl_i[lane] = int_val;
                }
            }
        }
    }
    else if (int_type == DIGI_INT_TYPE_ENET_SYS &&
             digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == TRUE)
    {
        /* Read ENET LINE interrupts from FW */
        if (has_epmm_rx_lobl_en)
        {
            /* Read EPMM_CBR_RX_LOBL */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_enet_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL,
                                                              TRUE,
                                                              &epmm_cbr_rx_lobl_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (lane = 0; lane < EXCO4M_N_LANES; lane += 1)
                {
                    int_val = (epmm_cbr_rx_lobl_i >> lane) & ((NULL == filt_table_ptr) ? 0x1 : filt_table_ptr->epmm.exco4m.rx_lobl_i[lane]);
                    int_table_ptr->epmm.exco4m.rx_lobl_i[lane] = int_val;
                }
            }
        }
    }
    
    PMC_RETURN(result);
}/* digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve */

/*******************************************************************************
*  digi_enet_int_fw_chnl_exco4m_rx_lobl_i_enabled_check
* ______________________________________________________________________________
*
*  DESCRIPTION:
*   Check that FW active exco4m rx_lobl_i interrupt.
*  
*  INPUTS:
*   digi_handle            - pointer to DIGI handle instance.
*   int_type               - ENET LINE or SYS interrupts to operate on
*   has_epmm_rx_lobl_en    - when TRUE EPMM rx_lobl_i interrupt is retrieved
*   has_etrans_rx_lobl_en  - when TRUE ETRANS rx_lobl_i interrupt is retrieved
*   int_en_table_ptr       - pointer to enable table. Mark fields as TRUE within this
*                           table to indicate that the interrupt/status will be checked.
*
*  OUTPUTS: 
*   int_found_ptr  - stores TRUE when an active interrupt is found, otherwise FALSE
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_int_fw_chnl_exco4m_rx_lobl_i_enabled_check(digi_handle_t        *digi_handle,
                                                                      digi_int_type_t       int_type, 
                                                                      BOOL8                 has_epmm_rx_lobl_en,
                                                                      BOOL8                 has_etrans_rx_lobl_en,
                                                                      enet_int_chnl_t      *int_en_table_ptr,
                                                                      BOOL8                *int_found_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;     
    UINT32 lane;
    BOOL8 int_val;
    UINT32 epmm_cbr_rx_lobl_i;
    UINT32 etrans_cbr_rx_lobl_i;

    PMC_ENTRY();
    *int_found_ptr = FALSE;

    PMC_ASSERT(NULL != int_en_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    if (int_type == DIGI_INT_TYPE_ENET_LINE &&
        digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
    {
        /* Read ENET LINE interrupts from FW */
        if (has_epmm_rx_lobl_en)
        {
            /* Read EPMM_CBR_RX_LOBL */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_enet_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_ENET_LINE_EPMM_CBR_RX_LOBL,
                                                              FALSE,
                                                              &epmm_cbr_rx_lobl_i);
            }
            /* check interrupt status */
            if (PMC_SUCCESS == result)
            {
                for (lane = 0; lane < EXCO4M_N_LANES && FALSE == *int_found_ptr; lane+=1)
                {
                    int_val = (epmm_cbr_rx_lobl_i >> lane) & int_en_table_ptr->epmm.exco4m.rx_lobl_i[lane];
                    if (0x1 == int_val)
                    {
                        *int_found_ptr = TRUE;
                    }
                }
            }
        }

        if (has_etrans_rx_lobl_en && FALSE == *int_found_ptr)
        {
            /* Read EPMM_CBR_RX_LOBL */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_enet_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_ENET_LINE_ETRANS_CBR_RX_LOBL,
                                                              FALSE,
                                                              &etrans_cbr_rx_lobl_i);
            }
            /* check interrupt status */
            if (PMC_SUCCESS == result)
            {
                for (lane = 0; lane < EXCO4M_N_LANES && FALSE == *int_found_ptr; lane += 1)
                {
                    int_val = (etrans_cbr_rx_lobl_i >> lane) & int_en_table_ptr->etrans.exco4m.rx_lobl_i[lane];
                    if (0x1 == int_val)
                    {
                        *int_found_ptr = TRUE;
                    }
                }
            }
        }
    }
    else if (int_type == DIGI_INT_TYPE_ENET_SYS &&
             digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == TRUE)
    {
        /* Read ENET LINE interrupts from FW */
        if (has_epmm_rx_lobl_en)
        {
            /* Read EPMM_CBR_RX_LOBL */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_enet_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_ENET_SYS_EPMM_CBR_RX_LOBL,
                                                              FALSE,
                                                              &epmm_cbr_rx_lobl_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (lane = 0; lane < EXCO4M_N_LANES && FALSE == *int_found_ptr; lane += 1)
                {
                    int_val = (epmm_cbr_rx_lobl_i >> lane) & int_en_table_ptr->epmm.exco4m.rx_lobl_i[lane];
                    if (0x1 == int_val)
                    {
                        *int_found_ptr = TRUE;
                    }
                }
            }
        }
    }
    
    PMC_RETURN(result);
}/* digi_enet_int_fw_chnl_exco4m_rx_lobl_i_enabled_check */

/*******************************************************************************
* digi_enet_serdes_port_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Check serdes rate to determine if ENET LINE or SYS channel support
*   Deficit Idle Count (DIC) compensation.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet channel handle instance
*
* OUTPUTS:
*   *port_ctxt_ptr         - pointer to serdes port associated to ENET channel.
*
* RETURNS:
*   PMC_SUCCESS, on success. A specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_serdes_port_get(digi_handle_t              *digi_handle,
                                            util_global_switch_data_t  *chnl_handle,
                                            digi_serdes_port_t        **port_ctxt_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 port_uid = DIGI_SERDES_UNASSIGNED;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr;
    UINT32 link;
    digi_serdes_prov_state_t prov_state = DIGI_SERDES_PROV_STATE_NO_SERDES;
    BOOL8 is_enet_line = FALSE;
    digi_serdes_prov_state_t exp_prov_state = DIGI_SERDES_PROV_STATE_ENET_SYS;
    PMC_ENTRY();
    
    /* Validate handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    if (PMC_SUCCESS == result)
    {
        result = digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle);
    }
    if (PMC_SUCCESS != result)
    {
        result = digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle);
    }
    else
    {
        is_enet_line = TRUE;
        exp_prov_state = DIGI_SERDES_PROV_STATE_ENET_LINE;
    }
    
    *port_ctxt_ptr = NULL;
    /* retrieve port uid */
    if (PMC_SUCCESS == result)
    {
        enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;
        
        result = digi_port_uid_from_enet_link_get(digi_handle,
                                                  enet_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                  is_enet_line,
                                                  &port_uid);
    }

    /* retrieve serdes port state */
    if (PMC_SUCCESS == result) 
    {
        link = enet_chnl_handle_ptr->switch_data.cpb_data.channel;
        
        result = digi_by_port_uid_post_mld_prov_state_get (digi_handle,
                                                           port_uid,
                                                           &prov_state);
    }

    /* retrieve serdes port pointer */
    if ((PMC_SUCCESS == result) && (prov_state == exp_prov_state)) 
    {           
        result = digi_port_ctxt_from_channel_number_get(digi_handle,
                                                        prov_state,
                                                        link,
                                                        port_ctxt_ptr);
    }
    else
    {
        result = DIGI_ERR_SERDES_PORT_NOT_PROV;
    }
    
    PMC_RETURN(result);
} /* digi_enet_enet_port_get */

/*******************************************************************************
*  digi_enet_fw_enet_client_source_conditional_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update FW ENET client source if different to ENET SS associated 
*   to given handle.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet channel handle instance
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS, on success. A specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_fw_enet_client_source_conditional_update(digi_handle_t              *digi_handle,
                                                                    util_global_switch_data_t  *chnl_handle)
{
    PMC_ERROR rc = PMC_SUCCESS;
    BOOL8 is_enet_line_handle = FALSE;
    digi_enet_client_chnl_def_t *chnl_ptr = (digi_enet_client_chnl_def_t *)chnl_handle;

    PMC_ENTRY();
    
    if (PMC_SUCCESS == digi_enet_line_chnl_handle_validate(digi_handle, chnl_handle))
    {
        is_enet_line_handle = TRUE;
    }
    else if (PMC_SUCCESS == digi_mapper_chnl_handle_validate(digi_handle, chnl_handle) &&
             (UTIL_GLOBAL_CPB_PORT_ENET_LINE == ((digi_enet_client_chnl_def_t *)chnl_ptr)->switch_data.cpb_data.port_type))
    {        
        is_enet_line_handle = TRUE;
    }
    else if (PMC_SUCCESS != digi_enet_sys_chnl_handle_validate(digi_handle, chnl_handle))
    {
        rc = DIGI_ERR_INVALID_ARG;
    }

    if (PMC_SUCCESS == rc)
    {
        rc =  digi_fw_enet_client_source_cfg(digi_handle, (TRUE == is_enet_line_handle) ? UTIL_GLOBAL_CPB_PORT_ENET_LINE : UTIL_GLOBAL_CPB_PORT_ENET_SYS);
    }
    
    PMC_RETURN(rc);
} /* digi_enet_fw_enet_client_source_conditional_update */

/*******************************************************************************
*  digi_enet_update_dsi_base_lane
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is a porting function that ensures the dsi_base_lane defined in
*  digi_enet_client_chnl_def_t is valid.  The use of this field was added
*  as part of 3.06 and any updates that proceeds this will not get valid
*  values set for this field.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC void digi_enet_update_dsi_base_lane(digi_handle_t *digi_handle)
{
    BOOL8 base_lane_set = FALSE;
    UINT32 i = 0;

    PMC_ENTRY();

    for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX; i++)
    {
        if (digi_handle->handle_pool.enet_line_chnl[i].dsi_base_lane != DIGI_SERDES_PIN_NOT_USED &&
            digi_handle->handle_pool.enet_line_chnl[i].dsi_base_lane > 0 &&
            digi_handle->handle_pool.enet_line_chnl[i].dsi_base_lane < DIGI_ENET_LINE_CHNL_MAX)
        {
            base_lane_set = TRUE;
            break;
        }
    }

    if (FALSE == base_lane_set)
    {
        UINT32 link;
        /*
         * dsi_base_lane if unused will be 0 for all values. If anything 
         * else then there is no point in setting this as it was already
         * done. Namely we are updating from a 3.06 or later release.
         * If false then need to construct the value from the post_mld 
         * lane_index.
         */
        for (link = 0; link < DIGI_ENET_CHNL_MAX; link++)
        {
            if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED ==
                digi_handle->handle_pool.enet_line_chnl[link].switch_data.header.prov_state)
            {
                UINT32 index;

                for (index = 0; index < DIGI_SERDES_DSI_LANES_MAX; index++)
                {
                    if (DIGI_SERDES_PROV_STATE_ENET_LINE == digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[index].prov_state &&
                        link == digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_line_ctxt[index].lane_index)
                    {
                        digi_handle->handle_pool.enet_line_chnl[link].dsi_base_lane = link;
                    }
                }
            }
                
        }
    }

    base_lane_set = FALSE;

    for (i = 0; i < DIGI_ENET_SYS_CHNL_MAX; i++)
    {
        if (digi_handle->handle_pool.enet_sys_chnl[i].dsi_base_lane != DIGI_SERDES_PIN_NOT_USED &&
            digi_handle->handle_pool.enet_sys_chnl[i].dsi_base_lane > 0 &&
            digi_handle->handle_pool.enet_sys_chnl[i].dsi_base_lane < DIGI_ENET_LINE_CHNL_MAX)
        {
            base_lane_set = TRUE;
            break;
        }
    }

    if (FALSE == base_lane_set)
    {
        UINT32 link;
        /*
         * dsi_base_lane if unused will be 0 for all values. If anything 
         * else then there is no point in setting this as it was already
         * done. Namely we are updating from a 3.06 or later release.
         * If false then need to construct the value from the post_mld 
         * lane_index.
         */
        for (link = 0; link < DIGI_ENET_CHNL_MAX; link++)
        {
            if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED ==
                digi_handle->handle_pool.enet_sys_chnl[link].switch_data.header.prov_state)
            {
                UINT32 index;

                for (index = 0; index < DIGI_SERDES_DSI_LANES_MAX; index++)
                {
                    if (DIGI_SERDES_PROV_STATE_ENET_SYS == digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[index].prov_state &&
                        link == digi_handle->var.post_mld_dsi_ctxt.post_mld_dsi_sys_ctxt[index].lane_index)
                    {
                        digi_handle->handle_pool.enet_sys_chnl[link].dsi_base_lane = link;
                    }
                }
            }
                
        }
    }

    PMC_RETURN();
} /* digi_enet_update_dsi_base_lane */


/*******************************************************************************
*  digi_enet_lf_force_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Function is called to enable or disable LF at the EPMM and/or in FW.
*  The operation checks whether OS_FORCE_EN has been set and then sets the
*  FW and/or epmm registers based on the fw_enable, epmm_enable flags.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  enet_chnl_ptr      - Reference to the enet channel handle, line or sys
*  fw_activated       - Whether fw_enable flag should be examined and a 
*                       call to digi_cmf_trigger_lf_cfg is required.
*  fw_enable          - Whether FW should be enabled or disabled.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS   - Operation was successful
*   PMC_ERROR     - Otherwise
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_lf_force_set(digi_handle_t *digi_handle,
                                        digi_enet_client_chnl_def_t *enet_chnl_ptr,
                                        BOOL8 fw_activated,
                                        BOOL8 fw_enable)
{
    BOOL8 force_enable = FALSE;
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_link_handle = digi_handle->enet_line_handle;

    PMC_ENTRY();

    if (UTIL_GLOBAL_CPB_PORT_ENET_SYS == enet_chnl_ptr->switch_data.cpb_data.port_type)
    {
        enet_link_handle = digi_handle->enet_sys_handle;
    }
        
    result = enet_epmm_lf_is_force_enable(enet_link_handle,
                                          enet_chnl_ptr->switch_data.cpb_data.channel,
                                          ENET_EGRESS,
                                          &force_enable);

    if (FALSE == force_enable)
    {
        PMC_RETURN(result);
    }
    /*
     * If fw_activated is FALSE then the enet handle is being deprovisioned
     * and want to ensure the epmm lf value is reset to defaults.
     */
    if (FALSE == fw_activated)
    {
        result = enet_epmm_lf_cfg(enet_link_handle,
                                  enet_chnl_ptr->switch_data.cpb_data.channel,
                                  ENET_EGRESS, FALSE);

        if (PMC_SUCCESS == result)
        {
            result = enet_epmm_lf_set_force_enable(enet_link_handle,
                                                   enet_chnl_ptr->switch_data.cpb_data.channel,
                                                   ENET_EGRESS, FALSE);
        }
    }
    else if (TRUE == fw_enable)
    {
        /*
         * If FW is enabled then set the FW value however do not disable
         * the SW as this may disable the OS_FORCE_EN which is 
         * undesirable.
         */
        result = digi_cmf_trigger_lf_cfg(digi_handle,
                                         (digi_enet_client_chnl_t *)enet_chnl_ptr,
                                         DIGI_SW_START_FORCING_ACTION);
    }
    else if (FALSE == fw_enable)
    {
        /*
         * Reset the FW from controlling the local fault. 
         */
        result = digi_cmf_trigger_lf_cfg(digi_handle,
                                         (digi_enet_client_chnl_t *)enet_chnl_ptr,
                                         DIGI_SW_RELEASE_ACTION_CONTROL);
        /*
         * Since OS_FORCE_EN is TRUE then set the EPMM registers to
         * indicate SW is driving. This may result in the SW settings being
         * set again.
         */
        if (PMC_SUCCESS == result)
        {
            result = enet_epmm_lf_cfg(enet_link_handle,
                                      enet_chnl_ptr->switch_data.cpb_data.channel,
                                      ENET_EGRESS, TRUE);
        }
    }

    PMC_RETURN(result);
} /* digi_enet_lf_force_set */

#endif /* DOXYGEN_PUBLIC_ONLY */



/* 
** Deprecated Functions 
*/


#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

/*******************************************************************************
* digi_enet_gsup43_c7point3_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the Gsup43 C7.3 status of the provisioned channel.
*   Some of the status bits are the interrupt status.  Interrupt status
*   is cleared by writing a 1 to the interrupt status bit.  If the
*   interrupt status bits should be cleared as part of this API call,
*   then the clear_interrupt flag should be set to TRUE.
*   Only OTN 10GE datapaths sent through the ETRANS can have Gsup43 C7.3
*   status.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   clear_interrupt        - flag to clear interrupt status
*
* OUTPUTS:
*   *status_ptr            - a pointer to Gsup43 C7.3 status containing the following bits:\n
*                              - LOS transition  (LSB)\n
*                              - LOS status\n
*                              - RX synchronization transition\n
*                              - RX synchronization status\n
*                              - high BER transition\n
*                              - high BER status\n
*                              - link fail transition\n
*                              - link fail status\n
*                              - link fault transition\n
*                              - link fault status\n
*                              - 64B/66B type field error detected\n
*                              - PRBS31 error detected\n
*                              - input pattern of all ones or zeros transition\n
*                              - input pattern of all ones or zeros detected\n
*                              - LF detected\n
*                              - RF detected\n
*                              - FIFO underrun occurred\n
*                              - FIFO overrun occurred\n
*                              - UPI type error detected\n
*                              - alarm - sending RF\n
*                              - alarm - sending LF\n
*                              - alarm - sending Idle control characters  (MSB)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in Gsup43 C7.3 block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_gsup43_c7point3_status_get(digi_handle_t *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      BOOL clear_interrupt,
                                                      UINT32 *status_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {
        switch (enet_handle->var.ch_datapath[link]) {
        case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT:
        case ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON:
        case ENET_SLAVE_10GE_GSUP43_7_3_TRANSPARENT_TX_PMON:
            rc = gsup43_c73_global_status_get(enet_handle->epmm_handle->gsup43_c73_handle[link],
                                              clear_interrupt, status_ptr);
            break;
        
        case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
        case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
            rc = gsup43_c73_global_status_get(enet_handle->etrans_handle->gsup43_c73_handle[link],
                                              clear_interrupt, status_ptr);
            break;
        
        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_gsup43_c7point3_status_get */

/*******************************************************************************
* digi_enet_gsup43_c6point2_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the Gsup43 C6.2 status of the provisioned channel.
*   Some of the status bits are the interrupt status.  Interrupt status
*   is cleared by writing a 1 to the interrupt status bit.  If the
*   interrupt status bits should be cleared as part of this API call,
*   then the clear_interrupt flag should be set to TRUE.
*   Only OTN 40GE & 100GE datapaths sent through the ETRANS can have
*   Gsup43 C6.2 status.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   clear_interrupt        - flag to clear interrupt status
*
* OUTPUTS:
*   *status_ptr            - a pointer to Gsup43 C6.2 status containing the following bits:\n
*                              - high BER transition  (LSB)\n
*                              - high BER status\n
*                              - PCS error block detected\n
*                              - PCS invalid block detected\n
*                              - skew range violation detected\n
*                              - idle test pattern removed\n
*                              - RF detected\n
*                              - LF detected\n
*                              - LOS transition\n
*                              - LOS status  (MSB)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in Gsup43 C6.2 block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_gsup43_c6point2_status_get(digi_handle_t *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      BOOL clear_interrupt,
                                                      UINT32 *status_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {
        UINT32 index_40g = link >> 2;           /* convert 0,4,8 to 0,1,2 */

        switch (enet_handle->var.ch_datapath[link]) {
        case ENET_SERDES_100GE_TRANSPARENT:
        case ENET_SERDES_100GE_TRANSPARENT_RX_PMON:
        case ENET_SLAVE_100GE_TRANSPARENT_TX_PMON:
            rc = exco4m_status_get(enet_handle->epmm_handle->exco4m_handle,
                                   clear_interrupt, status_ptr);
            break;
            
        case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
            rc = exco4m_status_get(enet_handle->etrans_handle->exco4m_handle,
                                   clear_interrupt, status_ptr);
            break;
            
        case ENET_SERDES_40GE_TRANSPARENT:
        case ENET_SERDES_40GE_TRANSPARENT_RX_PMON:
        case ENET_SLAVE_40GE_TRANSPARENT_TX_PMON:
            rc = exco3m_status_get(enet_handle->epmm_handle->exco3m_handle[index_40g],
                                   clear_interrupt, status_ptr);
            break;

        case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
            rc = exco3m_status_get(enet_handle->etrans_handle->exco3m_handle[index_40g],
                                   clear_interrupt, status_ptr);
            break;

        default:
            rc = DIGI_ERR_INVALID_ARG;
            break;
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_gsup43_c6point2_status_get */


/*******************************************************************************
* digi_enet_mac_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function obtains the MAC status of the provisioned channel.
*   Some of the status bits are the interrupt status.  Interrupt status
*   is cleared by writing a 1 to the interrupt status bit.  If the
*   interrupt status bits should be cleared as part of this API call,
*   then the clear_interrupt flag should be set to TRUE.
*   Only datapaths through the MAC block can have this status.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   clear_interrupt        - flag to clear interrupt status
*
* OUTPUTS:
*   *status_ptr            - a pointer to MAC status containing the following bits:\n
*                              - PHY LOS transition  (LSB)\n
*                              - PHY LOS status\n
*                              - LF transition\n
*                              - LF status\n
*                              - RF transition\n
*                              - RF status\n
*                              - high BER transition\n
*                              - high BER status\n
*                              - PCS lock transition to received blocks\n
*                              - PCS lock status\n
*                              - multi-lane alignment transition (40G & 100G only)\n
*                              - multi-lane alignment status (40G & 100G only)\n
*                              - egress pause status\n
*                              - link status (up/down)\n
*                              - AGB egress overflow occurred\n
*                              - AGB ingress overflow occurred\n
*                              - MAC egress overflow occurred  (MSB)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_mac_status_get(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL clear_interrupt,
                                          UINT32 *status_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {
        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = emac_status_get(enet_handle->emac_handle, link, clear_interrupt, status_ptr);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_mac_status_get */

/*******************************************************************************
* digi_enet_classification_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.01 release
*   This function sets the packet classification on the provisioned channel.
*   The classification controls extraction of packets in either the ingress
*   or egress direction - the packet type can be none, PTP or all packets.
*   Only datapaths through the MAC block can support classification.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   direction              - classification direction (ingress/egress)
*   type                   - classification packet type (none, all or PTP)
*   enable                 - classification enable (TRUE) or disable (FALSE)
* 
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES: 
*  * This function has been marked deprecated starting in the 3.01 release
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_classification_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              digi_enet_classification_direction_mode_t direction,
                                              digi_enet_classification_type_mode_t type,
                                              BOOL enable)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    enet_dir_t extract_direction = ENET_INGRESS;
    enet_extract_pkt_t extract_type = ENET_EXTRACT_NO_PKTS;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

         if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) 
            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

    }
    
    if (PMC_SUCCESS == rc) {

        if (direction == DIGI_ENET_EGRESS)
            extract_direction = ENET_EGRESS;

        if (enable) {
            if (type == DIGI_ENET_ALL_PACKETS)
                extract_type = ENET_EXTRACT_ALL_PKTS_MGMT;
            else if (type == DIGI_ENET_PTP_PACKETS)
                extract_type = ENET_EXTRACT_PTP_PKTS;
        }

        rc = enet_extract_packet(digi_handle->enet_line_handle, link,
                                 extract_direction, extract_type);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_classification_set */


/*******************************************************************************
* digi_enet_egress_insert_rate_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.01 release
*   This function sets the egress insertion rate of the provisioned channel
*   for traffic that is inserted in the egress transmission path.
*   Only datapaths through the MAC block can support egress insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   insert_quantum         - quantum for Insert path (default is 256)
*   system_quantum         - quantum for System Data path (default is 512)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS, on success. A specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_egress_insert_rate_set(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  UINT16 insert_quantum, 
                                                  UINT16 system_quantum)
{
    PMC_ERROR rc = PMC_SUCCESS;

    util_global_switch_data_t *enet_pmon_chnl_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* internal fuction does error checking */
    rc = digi_enet_egress_insert_rate_internal_set(digi_handle, 
                                                   chnl_handle, 
                                                   insert_quantum, 
                                                   system_quantum);

    if (rc == PMC_SUCCESS) {

        enet_pmon_chnl_handle = digi_enet_companion_pmon_get(digi_handle, chnl_handle);

        if (enet_pmon_chnl_handle != NULL) {

            rc = digi_enet_egress_insert_rate_internal_set(digi_handle, 
                                                           enet_pmon_chnl_handle, 
                                                           insert_quantum, 
                                                           system_quantum);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_egress_insert_rate_set */

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* digi_enet_egress_insert_rate_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.01 release
*   This function sets the egress insertion rate of the provisioned channel
*   for traffic that is inserted in the egress transmission path.
*   Only datapaths through the MAC block can support egress insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*   insert_quantum         - quantum for Insert path (default is 256)
*   system_quantum         - quantum for System Data path (default is 512)
*
* OUTPUTS:
*   none
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_enet_egress_insert_rate_internal_set(digi_handle_t *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            UINT16 insert_quantum, 
                                                            UINT16 system_quantum)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            rc = empei_e_drr_quantum_cfg(enet_handle->empei_e_handle, 
                                         link,
                                         (UINT32)system_quantum, 
                                         (UINT32)insert_quantum);
        }
    }

    PMC_RETURN(rc);
} /* digi_enet_egress_insert_rate_internal_set */

/*******************************************************************************
* digi_enet_egress_insert_rate_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED-Function has been marked as deprecated starting in the 3.01 release
*   This function sets the egress insertion rate of the provisioned channel
*   for traffic that is inserted in the egress transmission path.
*   Only datapaths through the MAC block can support egress insertion.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to Enet or mapper channel handle instance
*
* OUTPUTS:
*   *insert_quantum         - quantum for Insert path
*   *system_quantum         - quantum for System Data path
*
* RETURNS:
*   PMC_SUCCESS, Specific error code otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_egress_insert_rate_internal_get(digi_handle_t *digi_handle,
                                                           util_global_switch_data_t *chnl_handle,
                                                           UINT32* insert_quantum, 
                                                           UINT32* system_quantum)
{
    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                  &enet_handle, &link);
    if (PMC_SUCCESS == rc) {

        if (digi_enet_is_link_mac_block(enet_handle, link) != TRUE) {

            rc = DIGI_ERR_ENET_LINK_NOT_MAC;

        } else {

            empei_e_drr_quantum_get(enet_handle->empei_e_handle, 
                                    link,
                                    system_quantum, 
                                    insert_quantum);
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
} /* digi_enet_egress_insert_rate_internal_get */

/*******************************************************************************
* digi_enet_eclass_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is the ENET module wrapper for the digi_fw_enet_eclass_init() function,
*   which initialzes the ENET ECLASS.
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance.
*   datapath              - ENET datapath: Ingress or Egress
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS when execution is successful otherwise a descriptive error code
*   is returned.
*
* NOTES: It is recommended to initialize ECLASS after the ENET reset.
*        Note that, ENET MUST be provisioned before this function is called.
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_eclass_init(digi_handle_t *digi_handle,
                                       enet_dir_t    datapath)
{
    PMC_ERROR rc;

    PMC_ATOMIC_ENTRY(digi_handle);

    /** Initialize ENET ECLASS */
    rc = digi_fw_enet_eclass_init (digi_handle,
                                   datapath);

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_enet_eclass_init */

#endif /* PMC_DEPRECATED */

#endif/* DOXYGEN_PUBLIC_ONLY*/

/*******************************************************************************
* digi_enet_is_link_mac_block_without_transparent_pmon
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function validates the link path type is in the MAC block.
*
* INPUTS:
*   *digi_handle           - digi handle
*   *chnl_handle           - ENET channel handle
*
* OUTPUTS:
*   is_mac_block           - TRUE : MAC terminated
*                            FALSE : not MAC terminated (includes transparent PMON)
*
* RETURNS:
*   PMC_SUCCESS
*   DIGI_ERR_INVALID_ARG   - path type is not in MAC block
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_enet_is_link_mac_block_without_transparent_pmon(digi_handle_t *digi_handle,
                                                                      util_global_switch_data_t *chnl_handle,
                                                                      BOOL *is_mac_block)
{

    PMC_ERROR rc = PMC_SUCCESS;

    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ENTRY();

    rc = digi_enet_validate_parms(digi_handle, chnl_handle,
                                 &enet_handle, &link);


    switch (enet_handle->var.ch_datapath[link]) {
    case ENET_SERDES_100GE_MAC:
    case ENET_SLAVE_100GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC:
    case ENET_MAPOTN_GMP_G709_100GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_100GE_MAC_PMON:

    case ENET_SERDES_40GE_MAC:
    case ENET_SLAVE_40GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC:
    case ENET_MAPOTN_GMP_G709_40GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_40GE_MAC_PMON:

    case ENET_SERDES_10GE_MAC:
    case ENET_SLAVE_10GE_MAC_TX_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC:
    case ENET_MAPOTN_BMP_GSUP43_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC_PMON:
    case ENET_MAPOTN_GFP_GSUP43_6_2_10GE_MAC_PMON:
        *is_mac_block = TRUE;
        break;

    default:
        *is_mac_block = FALSE;
        break;
    }

    PMC_RETURN(rc);
} /* digi_enet_is_link_mac_block_without_transparent_pmon */


/* LCOV_EXCL_STOP */
#endif/* DOXYGEN_PUBLIC_ONLY*/


/*
** End of file
*/


